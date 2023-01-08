#include "expr_semantics.h"
#ifndef CROSS_CMPL
extern VEC* c_error;
extern char* filename;
extern size_t type_data_size[TYPE_NUM];
#ifdef _TEST_SEMANTICS_
extern int semantics_level;
#endif
bool expr_dispatch(AST_BASE* expr_node)
{
    if(!expr_node)
        return false;
    bool (*expr[EXPR_NUM])(AST_BASE* ast_node)={
        expr_value,assign_expr_value,cond_expr_value,logical_or_expr_value,
        logical_and_expr_value,bit_inclusive_or_expr_value,bit_exclusive_or_expr_value,
        and_expr_value,equal_expr_value,relation_expr_value,shift_expr_value,
        add_expr_value,mul_expr_value,cast_expr_value,unary_expr_value,
        postfix_expr_value,pri_expr_value
    };
    bool res=false;
    if(expr_node->type==expression)
        res=expr[0](expr_node);
    else if(expr_node->type>=assignment_expr&&expr_node->type<=primary_expression){
        res=expr[expr_node->type-assignment_expr+1](expr_node);
    }
    return res;
}

bool cond_expr_value(AST_BASE* ast_node)
{
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start condition expr\n");
#endif
    if(!ast_node||ast_node->type!=conditional_expr)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    AST_BASE* operand_one=AST_GET_CHILD(ast_node,2);
    AST_BASE* operand_two=AST_GET_CHILD(ast_node,4);
    AST_BASE* logical_or_expr_node=AST_GET_CHILD(ast_node,0);
    if(!expr_dispatch(logical_or_expr_node))
        goto error;
    M_TYPE* tmpt=Type_VEC_get_actual_base_type(logical_or_expr_node->expr_attribute->type_vec);
    if(!IS_SCALAR_TYPE(tmpt->typ_category))
    {
        C_ERROR(C0062_ERR_OPERAND_SCALAR_TYPE,ast_node);
        goto error;
    }
    /*start check the second and the third operand*/
    bool legal=false;
    bool point_qual=false;
    if(!expr_dispatch(operand_one)||!expr_dispatch(operand_two))
        goto error;
    VEC* tmp_type_vec=InitVEC(DEFAULT_CAPICITY);
    M_TYPE* res_type=NULL;
    M_TYPE* one_type,* two_type;
    one_type=Type_VEC_get_actual_base_type(operand_one->expr_attribute->type_vec);
    if(IS_INT_TYPE(one_type->typ_category)&&operand_one->expr_attribute->const_expr){
        long long int null_pointer_value=TP_INT_CAST_TYPE(one_type->typ_category,operand_one->expr_attribute->data_field);
        if(null_pointer_value==0)
        {
            one_type->typ_category=TP_NULL_POINTER_CONSTANT;
        }
    }
    two_type=Type_VEC_get_actual_base_type(operand_two->expr_attribute->type_vec);
    if(IS_INT_TYPE(two_type->typ_category)&&operand_two->expr_attribute->const_expr){
        long long int null_pointer_value=TP_INT_CAST_TYPE(two_type->typ_category,operand_two->expr_attribute->data_field);
        if(null_pointer_value==0)
        {
            two_type->typ_category=TP_NULL_POINTER_CONSTANT;
        }
    }
    if(IS_ARTH_TYPE(one_type->typ_category)&&IS_ARTH_TYPE(two_type->typ_category))
    {
        if(one_type->typ_category==TP_NULL_POINTER_CONSTANT)
        {
            operand_one->expr_attribute->data_field->sint=0;
            one_type->typ_category=TP_SINT;
        }
        if(two_type->typ_category==TP_NULL_POINTER_CONSTANT)
        {
            operand_two->expr_attribute->data_field->sint=0;
            two_type->typ_category=TP_SINT;
        }
        res_type=usual_arith_conversion(&one_type,&two_type);
        VECinsert(tmp_type_vec,(void*)res_type);
        legal=true;
    }
    else if((one_type->typ_category==TP_UNION&&two_type->typ_category==TP_UNION)||(one_type->typ_category==TP_STRUCT&&two_type->typ_category==TP_STRUCT))
    {
        if(!strcmp(((TP_SU*)one_type)->tag,((TP_SU*)two_type)->tag))
        {
            res_type=one_type;
            VECinsert(tmp_type_vec,(void*)res_type);
            legal=true;
        }
    }
    else if(one_type->typ_category==TP_VOID&&two_type->typ_category==TP_VOID)
    {
        res_type=build_base_type(TP_VOID);
        VECinsert(tmp_type_vec,(void*)res_type);
        legal=true;
    }
    else if(one_type->typ_category==TP_POINT&&two_type->typ_category==TP_POINT)
    {
        VEC* point_one=Type_VEC_get_Pointer_TO(operand_one->expr_attribute->type_vec,true);
        VEC* point_two=Type_VEC_get_Pointer_TO(operand_two->expr_attribute->type_vec,true);
        M_TYPE* point_base_one=Type_VEC_get_actual_base_type(point_one);
        M_TYPE* point_base_two=Type_VEC_get_actual_base_type(point_two);
        if(point_base_one->typ_category==TP_VOID&&point_base_two->typ_category!=TP_VOID)
        {
            res_type=build_base_type(TP_VOID);
            VECinsert(tmp_type_vec,(void*)res_type);
        }
        else if(point_base_one->typ_category!=TP_VOID&&point_base_two->typ_category==TP_VOID)
        {
            res_type=build_base_type(TP_VOID);
            VECinsert(tmp_type_vec,(void*)res_type);
        }
        else{
            if(!compatible_types(point_one,point_two))
            {
                C_ERROR(C0069_ERR_NEED_COMPATIBLE_TYPE,ast_node);
                goto error;
            }
            DelVEC(tmp_type_vec);
            tmp_type_vec=composite_types(point_one,point_two,true);
            res_type=build_base_type(TP_POINT);
            VECinsert(tmp_type_vec,(void*)res_type);
        }
        point_qual=true;
        DelVEC(point_one);
        DelVEC(point_two);
        legal=true;
    }
    else if(one_type->typ_category==TP_POINT&&two_type->typ_category==TP_NULL_POINTER_CONSTANT)
    {
        DelVEC(tmp_type_vec);
        VECcpy(operand_one->expr_attribute->type_vec,&tmp_type_vec);
        legal=true;
    }
    else if(two_type->typ_category==TP_POINT&&one_type->typ_category==TP_NULL_POINTER_CONSTANT)
    {
        DelVEC(tmp_type_vec);
        VECcpy(operand_two->expr_attribute->type_vec,&tmp_type_vec);
        legal=true;
    }
    if(!legal)
    {
        C_ERROR(C0071_ERR_CONDITION_OPERAND,ast_node);
        goto error;
    }
    if(point_qual)
    {
        M_TYPE* tmp_qual=Type_composite_qual(Type_VEC_get_qual(operand_one->expr_attribute->type_vec),Type_VEC_get_qual(operand_two->expr_attribute->type_vec));
        if(tmp_qual)
            VECinsert(tmp_type_vec,(void*)tmp_qual);
    }
    ast_node->expr_attribute->type_vec=tmp_type_vec;
    /*as for const expr,if logical_or_expr_node result is true ,only the first one is const,no need to consider the second,if false, the same*/
    /*TODO:const expr*/
    bool const_expr=false;
    
    if(logical_or_expr_node->expr_attribute->const_expr){
        int one_or_two=logical_const(tmpt->typ_category,logical_or_expr_node->expr_attribute->data_field);
        if(one_or_two==0&&operand_two->expr_attribute->const_expr)
        {
            const_expr=true;
            memcpy(ast_node->expr_attribute->data_field,operand_two->expr_attribute->data_field,sizeof(VALUE_DATA));
        }
        else if(one_or_two==1&&operand_one->expr_attribute->const_expr)
        {
            const_expr=true;
            memcpy(ast_node->expr_attribute->data_field,operand_one->expr_attribute->data_field,sizeof(VALUE_DATA));
        }
    }
    ast_node->expr_attribute->const_expr=const_expr;
    m_free(tei);
#ifdef _TEST_SEMANTICS_
    print_expr_type(ast_node);
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end condition expr\n");
    semantics_level--;
#endif
    return true;
error:
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end condition fail\n");
    semantics_level--;
#endif
    return false;
}
bool expr_value(AST_BASE* ast_node)
{
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start expr\n");
#endif
    /*a constant should not contain comma op*/
    if(!ast_node||ast_node->type!=expression)
        goto error;
    AST_BASE* assignment_expr_node=NULL;
    for(size_t i=0;i<AST_CHILD_NUM(ast_node);i+=2)
    {
        assignment_expr_node=AST_GET_CHILD(ast_node,i);
        if(!expr_dispatch(assignment_expr_node))
            goto error;
    }
    VECcpy(assignment_expr_node->expr_attribute->type_vec,&(ast_node->expr_attribute->type_vec));
    if(assignment_expr_node&&assignment_expr_node->expr_attribute->const_expr)
    {
        ast_node->expr_attribute->const_expr=true;
        ast_node->expr_attribute->data_size=assignment_expr_node->expr_attribute->data_size;
        memcpy(ast_node->expr_attribute->data_field,assignment_expr_node->expr_attribute->data_field,sizeof(VALUE_DATA));
    }
#ifdef _TEST_SEMANTICS_
    print_expr_type(ast_node);
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end condition expr\n");
    semantics_level--;
#endif
    return true;
error:
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end condition fail\n");
    semantics_level--;
#endif
    return false;
}
bool assign_expr_value(AST_BASE* ast_node)
{
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start assignment expr\n");
#endif
    if(!ast_node||ast_node->type!=assignment_expr)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    AST_BASE* unary_expr_node=AST_GET_CHILD(ast_node,0);
    AST_BASE* assign_operator=AST_GET_CHILD(ast_node,1);
    AST_BASE* assign_expr_node=AST_GET_CHILD(ast_node,2);
    if(!expr_dispatch(unary_expr_node))
        goto error;
    if(!expr_dispatch(assign_expr_node))
        goto error;
    VEC* unary_type_vec=lvalue_convertion(unary_expr_node->expr_attribute->type_vec);
    VEC* assign_type_vec=lvalue_convertion(assign_expr_node->expr_attribute->type_vec);
    M_TYPE* assign_base_type=Type_VEC_get_actual_base_type(assign_type_vec);
    if(IS_INT_TYPE(assign_base_type->typ_category)&&assign_expr_node->expr_attribute->const_expr){
        long long int null_pointer_value=TP_INT_CAST_TYPE(assign_base_type->typ_category,assign_expr_node->expr_attribute->data_field);
        if(null_pointer_value==0)
            assign_base_type->typ_category=TP_NULL_POINTER_CONSTANT;
    }
    if(!unary_expr_node->expr_attribute->is_lvalue)
    {
        C_ERROR(C0051_ERR_LVALUE_REQUIRED,ast_node);
        goto error;
    }
    if(!Type_VEC_modify_lvalue(unary_type_vec,true))
    {
        C_ERROR(C0056_ERR_EXPR_MODIFIABLE_LVALUE,ast_node);
        goto error;
    }
    if(assign_operator->type==equal){
        if(!assignment_type_check(unary_type_vec,assign_type_vec))
        {
            C_ERROR(C0072_ERR_ASSIGN_OPERAND,ast_node);
            goto error;
        }
    }
    else{
        ;/*no any other operators,which is changed in parser stage*/
    }
    ast_node->expr_attribute->type_vec=unary_type_vec;
    if(unary_expr_node->expr_attribute->const_expr)
    {
        ast_node->expr_attribute->const_expr=true;
        memcpy(ast_node->expr_attribute->data_field,unary_expr_node->expr_attribute->data_field,sizeof(VALUE_DATA));
    }
    m_free(tei);
#ifdef _TEST_SEMANTICS_
    print_expr_type(ast_node);
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end assignment expr\n");
    semantics_level--;
#endif
    return true;
error:
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end assignment fail\n");
    semantics_level--;
#endif
    return false;
}
bool assignment_type_check(VEC* unary_type_vec,VEC* assign_type_vec)
{
    M_TYPE* unary_base_type=Type_VEC_get_actual_base_type(unary_type_vec);
    if(unary_base_type->typ_category==TP_ENUM)
    {
        M_TYPE* tmpt=build_base_type(TP_SINT);
        Type_VEC_change_actual_base_type(unary_type_vec,tmpt);
        unary_base_type=Type_VEC_get_actual_base_type(unary_type_vec);
    }
    M_TYPE* assign_base_type=Type_VEC_get_actual_base_type(assign_type_vec);
    if(assign_base_type->typ_category==TP_ENUM)
    {
        M_TYPE* tmpt=build_base_type(TP_SINT);
        Type_VEC_change_actual_base_type(assign_type_vec,tmpt);
        assign_base_type=Type_VEC_get_actual_base_type(assign_type_vec);
    }
    bool legal=false;
    if(IS_ARTH_TYPE(unary_base_type->typ_category)&&IS_ARTH_TYPE(assign_base_type->typ_category))
    {
        legal=true;
    }
    else if((unary_base_type->typ_category==TP_UNION&&assign_base_type->typ_category==TP_UNION)||(unary_base_type->typ_category==TP_STRUCT&&assign_base_type->typ_category==TP_STRUCT))
    {
        if(compatible_types(unary_type_vec,assign_type_vec))
            legal=true;
    }
    else if(unary_base_type->typ_category==TP_POINT&&assign_base_type->typ_category==TP_POINT)
    {
        bool qual_legal=false;
        M_TYPE* qual_a=Type_VEC_get_qual(unary_type_vec);
        M_TYPE* qual_b=Type_VEC_get_qual(assign_type_vec);
        if(qual_b==NULL&&qual_a==NULL)
            qual_legal=true;
        else if(qual_a&&!qual_b)
            qual_legal=true;
        else if(qual_a&&qual_b)
        {
            qual_legal=(qual_a->typ_stor==qual_b->typ_stor)&&((qual_a->type_qual)&((qual_a->type_qual)|(qual_b->type_qual)))&&((qual_a->func_spec)&((qual_a->func_spec)|(qual_b->func_spec)));
        }
        if(qual_legal)
        {
            VEC* point_a=Type_VEC_get_Pointer_TO(unary_type_vec,true);
            VEC* point_b=Type_VEC_get_Pointer_TO(assign_type_vec,true);
            qual_a=Type_VEC_get_actual_base_type(point_a);
            qual_b=Type_VEC_get_actual_base_type(point_b);
            if(qual_a->typ_category==TP_VOID||qual_b->typ_category==TP_VOID)
                legal=true;
            else
            {
                if(compatible_types(point_a,point_b))
                    legal=true;
            }
        }
        else
            legal=false;
    }
    else if(unary_base_type->typ_category==TP_POINT&&assign_base_type->typ_category==TP_NULL_POINTER_CONSTANT)
    {
        legal=true;
    }
    else if(unary_base_type->typ_category==TP_BOOL&&assign_base_type->typ_category==TP_POINT)
    {
        legal=true;
    }
    return legal;
}
bool logical_or_expr_value(AST_BASE* ast_node)
{
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start logical and expr\n");
#endif
    if(!ast_node||ast_node->type!=logical_or_expr)
        goto error;
    bool const_expr=true;
    bool const_or_value=false;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    VEC* tmp_type_vec=InitVEC(DEFAULT_CAPICITY);
    for(size_t i=0;i<AST_CHILD_NUM(ast_node);i+=2){
        AST_BASE* logical_or_expr_node=AST_GET_CHILD(ast_node,i);
        if(!expr_dispatch(logical_or_expr_node))
            goto error;
        M_TYPE* tmpt=Type_VEC_get_actual_base_type(logical_or_expr_node->expr_attribute->type_vec);
        if(!IS_SCALAR_TYPE(tmpt->typ_category))
        {
            C_ERROR(C0062_ERR_OPERAND_SCALAR_TYPE,ast_node);
            goto error;
        }
        if(!logical_or_expr_node->expr_attribute->const_expr){
            const_expr=false;
        }
        else{
            const_or_value=logical_const(tmpt->typ_category,logical_or_expr_node->expr_attribute->data_field);
            if(const_or_value)
                break;
        }
    }
    M_TYPE* tmpt=build_base_type(TP_SINT);
    VECinsert(tmp_type_vec,(void*)tmpt);
    ast_node->expr_attribute->type_vec=tmp_type_vec;
    ast_node->expr_attribute->const_expr=const_expr;
    if(const_expr){
        ast_node->expr_attribute->data_size=type_data_size[TP_SINT];
        ast_node->expr_attribute->data_field->sint=const_or_value;
    }
    m_free(tei);
#ifdef _TEST_SEMANTICS_
    print_expr_type(ast_node);
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end logical and expr\n");
    semantics_level--;
#endif
    return true;
error:
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end logical and expr fail\n");
    semantics_level--;
#endif
    return false;
}
bool logical_and_expr_value(AST_BASE* ast_node)
{
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start logical and expr\n");
#endif
    if(!ast_node||ast_node->type!=logical_and_expr)
        goto error;
    bool const_expr=true;
    bool const_and_value=true;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    VEC* tmp_type_vec=InitVEC(DEFAULT_CAPICITY);
    for(size_t i=0;i<AST_CHILD_NUM(ast_node);i+=2){
        AST_BASE* logical_and_expr_node=AST_GET_CHILD(ast_node,i);
        if(!expr_dispatch(logical_and_expr_node))
            goto error;
        M_TYPE* tmpt=Type_VEC_get_actual_base_type(logical_and_expr_node->expr_attribute->type_vec);
        if(!IS_SCALAR_TYPE(tmpt->typ_category))
        {
            C_ERROR(C0062_ERR_OPERAND_SCALAR_TYPE,ast_node);
            goto error;
        }
        if(!logical_and_expr_node->expr_attribute->const_expr){
            const_expr=false;
        }
        else{
            const_and_value=logical_const(tmpt->typ_category,logical_and_expr_node->expr_attribute->data_field);
            if(!const_and_value)
                break;
        }
    }
    M_TYPE* tmpt=build_base_type(TP_SINT);
    VECinsert(tmp_type_vec,(void*)tmpt);
    ast_node->expr_attribute->type_vec=tmp_type_vec;
    ast_node->expr_attribute->const_expr=const_expr;
    if(const_expr){
        ast_node->expr_attribute->data_size=type_data_size[TP_SINT];
        ast_node->expr_attribute->data_field->sint=const_and_value;
    }
    m_free(tei);
#ifdef _TEST_SEMANTICS_
    print_expr_type(ast_node);
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end logical and expr\n");
    semantics_level--;
#endif
    return true;
error:
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end logical and expr fail\n");
    semantics_level--;
#endif
    return false;
}
bool bit_inclusive_or_expr_value(AST_BASE* ast_node)
{
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start inor expr\n");
#endif
    if(!ast_node||ast_node->type!=inclusive_or_expr)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    bool const_expr=false;
    long long int const_value=0;
    VEC* tmp_type_vec=NULL;
    M_TYPE* res_type=NULL;
    for(size_t i=0;i<AST_CHILD_NUM(ast_node);i+=2)
    {
        AST_BASE* inor_expr_node=AST_GET_CHILD(ast_node,i);
        if(!expr_dispatch(inor_expr_node))
            goto error;
        M_TYPE* tmpt=Type_VEC_get_actual_base_type(inor_expr_node->expr_attribute->type_vec);
        if(!(IS_INT_TYPE(tmpt->typ_category)))
        {
            C_ERROR(C0060_ERR_OPERAND_INTEGER_TYPE,ast_node);
            goto error;
        }
        if(i==0){
            VECcpy(inor_expr_node->expr_attribute->type_vec,&tmp_type_vec);
            res_type=tmpt;
            if(inor_expr_node->expr_attribute->const_expr)
            {
                const_expr=true;
                const_value=TP_INT_CAST_TYPE(tmpt->typ_category,inor_expr_node->expr_attribute->data_field);
            }
            continue;
        }
        usual_arith_conversion(&res_type,&tmpt);
        DelVEC(tmp_type_vec);
        tmp_type_vec=InitVEC(DEFAULT_CAPICITY);
        VECinsert(tmp_type_vec,(void*)res_type);
        if(const_expr&&inor_expr_node->expr_attribute->const_expr)
        {
            const_expr=true;
            const_value=const_value | TP_INT_CAST_TYPE(tmpt->typ_category,inor_expr_node->expr_attribute->data_field);
        }
    }
    ast_node->expr_attribute->type_vec=tmp_type_vec;
    ast_node->expr_attribute->const_expr=const_expr;
    if(const_expr)
    {
        ast_node->expr_attribute->data_size=type_data_size[res_type->typ_category];
        /*const_value*/
        bitwise_const(res_type->typ_category,ast_node->expr_attribute->data_field,const_value);
    }
    m_free(tei);
#ifdef _TEST_SEMANTICS_
    print_expr_type(ast_node);
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end bit inor expr\n");
    semantics_level--;
#endif
    return true;
error:
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end bit inor expr fail\n");
    semantics_level--;
#endif
    return false;
}
bool bit_exclusive_or_expr_value(AST_BASE* ast_node)
{
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start exor expr\n");
#endif
    if(!ast_node||ast_node->type!=exclusive_or_expr)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    bool const_expr=false;
    long long int const_value=0;
    VEC* tmp_type_vec=NULL;
    M_TYPE* res_type=NULL;
    for(size_t i=0;i<AST_CHILD_NUM(ast_node);i+=2)
    {
        AST_BASE* exor_expr_node=AST_GET_CHILD(ast_node,i);
        if(!expr_dispatch(exor_expr_node))
            goto error;
        M_TYPE* tmpt=Type_VEC_get_actual_base_type(exor_expr_node->expr_attribute->type_vec);
        if(!(IS_INT_TYPE(tmpt->typ_category)))
        {
            C_ERROR(C0060_ERR_OPERAND_INTEGER_TYPE,ast_node);
            goto error;
        }
        if(i==0){
            VECcpy(exor_expr_node->expr_attribute->type_vec,&tmp_type_vec);
            res_type=tmpt;
            if(exor_expr_node->expr_attribute->const_expr)
            {
                const_expr=true;
                const_value=TP_INT_CAST_TYPE(tmpt->typ_category,exor_expr_node->expr_attribute->data_field);
            }
            continue;
        }
        usual_arith_conversion(&res_type,&tmpt);
        DelVEC(tmp_type_vec);
        tmp_type_vec=InitVEC(DEFAULT_CAPICITY);
        VECinsert(tmp_type_vec,(void*)res_type);
        if(const_expr&&exor_expr_node->expr_attribute->const_expr)
        {
            const_expr=true;
            const_value=const_value ^ TP_INT_CAST_TYPE(tmpt->typ_category,exor_expr_node->expr_attribute->data_field);
        }
    }
    ast_node->expr_attribute->type_vec=tmp_type_vec;
    ast_node->expr_attribute->const_expr=const_expr;
    if(const_expr)
    {
        ast_node->expr_attribute->data_size=type_data_size[res_type->typ_category];
        /*TODO:const_value DONE*/
        bitwise_const(res_type->typ_category,ast_node->expr_attribute->data_field,const_value);
    }
    m_free(tei);
#ifdef _TEST_SEMANTICS_
    print_expr_type(ast_node);
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end bit exor expr\n");
    semantics_level--;
#endif
    return true;
error:
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end bit exor expr fail\n");
    semantics_level--;
#endif
    return false;
}
bool and_expr_value(AST_BASE* ast_node)
{
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start and expr\n");
#endif
    if(!ast_node||ast_node->type!=and_expression)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    bool const_expr=false;
    long long int const_value=0;
    VEC* tmp_type_vec=NULL;
    M_TYPE* res_type=NULL;
    for(size_t i=0;i<AST_CHILD_NUM(ast_node);i+=2)
    {
        AST_BASE* and_expr_node=AST_GET_CHILD(ast_node,i);
        if(!expr_dispatch(and_expr_node))
            goto error;
        M_TYPE* tmpt=Type_VEC_get_actual_base_type(and_expr_node->expr_attribute->type_vec);
        if(!(IS_INT_TYPE(tmpt->typ_category)))
        {
            C_ERROR(C0060_ERR_OPERAND_INTEGER_TYPE,ast_node);
            goto error;
        }
        if(i==0){
            VECcpy(and_expr_node->expr_attribute->type_vec,&tmp_type_vec);
            res_type=tmpt;
            if(and_expr_node->expr_attribute->const_expr)
            {
                const_expr=true;
                const_value=TP_INT_CAST_TYPE(tmpt->typ_category,and_expr_node->expr_attribute->data_field);
            }
            continue;
        }
        usual_arith_conversion(&res_type,&tmpt);
        DelVEC(tmp_type_vec);
        tmp_type_vec=InitVEC(DEFAULT_CAPICITY);
        VECinsert(tmp_type_vec,(void*)res_type);
        if(const_expr&&and_expr_node->expr_attribute->const_expr)
        {
            const_expr=true;
            const_value=const_value & TP_INT_CAST_TYPE(tmpt->typ_category,and_expr_node->expr_attribute->data_field);
        }
    }
    ast_node->expr_attribute->type_vec=tmp_type_vec;
    ast_node->expr_attribute->const_expr=const_expr;
    if(const_expr)
    {
        ast_node->expr_attribute->data_size=type_data_size[res_type->typ_category];
        /*const_value*/
        bitwise_const(res_type->typ_category,ast_node->expr_attribute->data_field,const_value);
    }
    m_free(tei);
#ifdef _TEST_SEMANTICS_
    print_expr_type(ast_node);
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end and expr\n");
    semantics_level--;
#endif
    return true;
error:
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end and expr fail\n");
    semantics_level--;
#endif
    return false;
}
bool equal_expr_value(AST_BASE* ast_node)
{
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start equal expr\n");
#endif
    if(!ast_node||ast_node->type!=equality_expr)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    AST_BASE* equal_expr_node=AST_GET_CHILD(ast_node,0);
    if(!expr_dispatch(equal_expr_node))
        goto error;
    VEC* tmp_type_vec=NULL;
    bool const_expr=equal_expr_node->expr_attribute->const_expr;
    /*TODO*/
    bool equal_value=false;
    VECcpy(equal_expr_node->expr_attribute->type_vec,&tmp_type_vec);
    VALUE_DATA* tmp_data_field=m_alloc(sizeof(VALUE_DATA));
    memcpy(tmp_data_field,equal_expr_node->expr_attribute->data_field,sizeof(VALUE_DATA));
    for(size_t i=0;i+2<AST_CHILD_NUM(ast_node);i+=2){
        AST_BASE* operator=AST_GET_CHILD(ast_node,i+1);
        AST_BASE* relation_expression_node=AST_GET_CHILD(ast_node,i+2);
        if(!expr_dispatch(relation_expression_node))
            goto error;
        M_TYPE* equal_base_type=Type_VEC_get_actual_base_type(tmp_type_vec);
        M_TYPE* rela_base_type=Type_VEC_get_actual_base_type(relation_expression_node->expr_attribute->type_vec);
        if(IS_INT_TYPE(rela_base_type->typ_category)&&relation_expression_node->expr_attribute->const_expr){
            long long int null_pointer_value=TP_INT_CAST_TYPE(rela_base_type->typ_category,relation_expression_node->expr_attribute->data_field);
            if(null_pointer_value==0)
                rela_base_type->typ_category=TP_NULL_POINTER_CONSTANT;
        }
        bool legal=false;
        if(IS_ARTH_TYPE(equal_base_type->typ_category)&&IS_ARTH_TYPE(rela_base_type->typ_category))
        {
            if(const_expr&&relation_expression_node->expr_attribute->const_expr){
                const_expr=true;
                /*TODO:const value*/
                /*I feels sad about this code,*/
                if(IS_INT_TYPE(equal_base_type->typ_category)&&IS_INT_TYPE(rela_base_type->typ_category))
                {
                    long long int equal_base_value=get_int_const(equal_base_type->typ_category,tmp_data_field,true);
                    long long int rela_base_value=get_int_const(rela_base_type->typ_category,relation_expression_node->expr_attribute->data_field,true);
                    equal_value=(equal_base_value==rela_base_value);
                }
                else if(IS_INT_TYPE(equal_base_type->typ_category)&&IS_FLOAT_TYPE(rela_base_type->typ_category))
                {
                    long long int equal_base_value=get_int_const(equal_base_type->typ_category,tmp_data_field,true);
                    long double rela_base_value=get_float_const(rela_base_type->typ_category,relation_expression_node->expr_attribute->data_field,true);
                    equal_value=(equal_base_value==rela_base_value);
                }
                else if(IS_FLOAT_TYPE(equal_base_type->typ_category)&&IS_INT_TYPE(rela_base_type->typ_category))
                {
                    long double equal_base_value=get_float_const(equal_base_type->typ_category,tmp_data_field,true);
                    long long int rela_base_value=get_int_const(rela_base_type->typ_category,relation_expression_node->expr_attribute->data_field,true);
                    equal_value=(equal_base_value==rela_base_value);
                }
                else if(IS_FLOAT_TYPE(equal_base_type->typ_category)&&IS_FLOAT_TYPE(rela_base_type->typ_category))
                {
                    long double equal_base_value=get_float_const(equal_base_type->typ_category,tmp_data_field,true);
                    long double rela_base_value=get_float_const(rela_base_type->typ_category,relation_expression_node->expr_attribute->data_field,true);
                    equal_value=(equal_base_value==rela_base_value);
                }
#if _CPLX_SUPPORT==1 
                else if(IS_INT_TYPE(equal_base_type->typ_category)&&IS_COMPLEX_TYPE(rela_base_type->typ_category))
                {
                    long long int equal_base_value=get_int_const(equal_base_type->typ_category,tmp_data_field,true);
                    long double* rela_base_value=get_complex_const(rela_base_type->typ_category,relation_expression_node->expr_attribute->data_field,true);
                    equal_value=((equal_base_value==*rela_base_value)&&(*(rela_base_value+1)==0));
                }
                else if(IS_COMPLEX_TYPE(equal_base_type->typ_category)&&IS_INT_TYPE(rela_base_type->typ_category))
                {
                    long double* equal_base_value=get_complex_const(equal_base_type->typ_category,tmp_data_field,true);
                    long long int rela_base_value=get_int_const(rela_base_type->typ_category,relation_expression_node->expr_attribute->data_field,true);
                    equal_value=((*equal_base_value==rela_base_value)&&(*(equal_base_value+1)==0));
                }
                else if(IS_COMPLEX_TYPE(equal_base_type->typ_category)&&IS_FLOAT_TYPE(rela_base_type->typ_category))
                {
                    long double* equal_base_value=get_complex_const(equal_base_type->typ_category,tmp_data_field,true);
                    long double rela_base_value=get_float_const(rela_base_type->typ_category,relation_expression_node->expr_attribute->data_field,true);
                    equal_value=((*equal_base_value==rela_base_value)&&(*(equal_base_value+1)==0));
                }
                else if(IS_FLOAT_TYPE(equal_base_type->typ_category)&&IS_COMPLEX_TYPE(rela_base_type->typ_category))
                {
                    long double equal_base_value=get_float_const(equal_base_type->typ_category,tmp_data_field,true);
                    long double* rela_base_value=get_complex_const(rela_base_type->typ_category,relation_expression_node->expr_attribute->data_field,true);
                    equal_value=((equal_base_value==*rela_base_value)&&(*(rela_base_value+1)==0));
                }
                else if(IS_COMPLEX_TYPE(equal_base_type->typ_category)&&IS_COMPLEX_TYPE(rela_base_type->typ_category))
                {
                    long double* equal_base_value=get_complex_const(equal_base_type->typ_category,tmp_data_field,true);
                    long double* rela_base_value=get_complex_const(rela_base_type->typ_category,relation_expression_node->expr_attribute->data_field,true);
                    equal_value=(*equal_base_value==*rela_base_value)&&(*(equal_base_value+1)==*(rela_base_value+1));
                }
#endif
                if(operator->type==double_equal)
                {
                    if(equal_value)
                        tmp_data_field->sint=1;
                    else
                        tmp_data_field->sint=0;
                }
                else if(operator->type==un_equal)
                {
                    if(equal_value)
                        tmp_data_field->sint=0;
                    else
                        tmp_data_field->sint=1;
                }
            }
            else{
                const_expr=false;
            }
            legal=true;
        }
        else if(equal_base_type->typ_category==TP_POINT&&rela_base_type->typ_category==TP_POINT)
        {
            const_expr=false;
            VEC* tmp_equal_vec=Type_VEC_get_Pointer_TO(tmp_type_vec,true);
            VEC* tmp_rela_vec=Type_VEC_get_Pointer_TO(relation_expression_node->expr_attribute->type_vec,true);
            if(compatible_types(tmp_equal_vec,tmp_rela_vec))
            {
                legal=true;
            }
            M_TYPE* tmpt=Type_VEC_get_actual_base_type(tmp_equal_vec);
            if(tmpt->typ_category==TP_VOID)
            {
                legal=true;
            }
            tmpt=Type_VEC_get_actual_base_type(tmp_rela_vec);
            if(tmpt->typ_category==TP_VOID)
            {
                legal=true;
            }
            DelVEC(tmp_equal_vec);
            DelVEC(tmp_rela_vec);
        }
        else if(equal_base_type->typ_category==TP_POINT&&rela_base_type->typ_category==TP_NULL_POINTER_CONSTANT){
            const_expr=false;
            legal=true;
        }
        else if(rela_base_type->typ_category==TP_POINT)
        {
            const_expr=false;
            if(equal_value==0)
            {
                legal=true;
            }
        }
        if(!legal)
        {
            C_ERROR(C0070_ERR_EQUAL_OPERAND,ast_node);
            goto error;
        }
        DelVEC(tmp_type_vec);
        tmp_type_vec=InitVEC(DEFAULT_CAPICITY);
        M_TYPE* tmpt=build_base_type(TP_SINT);
        VECinsert(tmp_type_vec,(void*)tmpt);
    }
    ast_node->expr_attribute->type_vec=tmp_type_vec;
    ast_node->expr_attribute->const_expr=const_expr;
    if(const_expr)
    {
        ast_node->expr_attribute->data_size=type_data_size[TP_SINT];
        ast_node->expr_attribute->data_field->sint=tmp_data_field->sint;  /*only const_expr is true that this field is meaningful*/
    }
    m_free(tei);
    m_free(tmp_data_field);
#ifdef _TEST_SEMANTICS_
    print_expr_type(ast_node);
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end equal expr\n");
    semantics_level--;
#endif
    return true;
error:
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end equal expr fail\n");
    semantics_level--;
#endif
    return false;
}
bool relation_expr_value(AST_BASE* ast_node)
{
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start relation expr\n");
#endif
    if(!ast_node||ast_node->type!=relational_expr)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    AST_BASE* relation_expr_node=AST_GET_CHILD(ast_node,0);
    VEC* tmp_type_vec=NULL;
    if(!expr_dispatch(relation_expr_node)){
        goto error;
    }
    bool const_expr=relation_expr_node->expr_attribute->const_expr;
    VALUE_DATA* tmp_data_field=m_alloc(sizeof(VALUE_DATA));
    VECcpy(relation_expr_node->expr_attribute->type_vec,&tmp_type_vec);
    memcpy(tmp_data_field,relation_expr_node->expr_attribute->data_field,sizeof(VALUE_DATA));
    for(size_t i=0;i+2<AST_CHILD_NUM(ast_node);i+=2){
        AST_BASE* operator=AST_GET_CHILD(ast_node,i+1);
        AST_BASE* shift_expr_node=AST_GET_CHILD(ast_node,i+2);
        if(!expr_dispatch(shift_expr_node))
            goto error;
        M_TYPE* rela_base_type=Type_VEC_get_actual_base_type(tmp_type_vec);
        M_TYPE* shift_base_type=Type_VEC_get_actual_base_type(shift_expr_node->expr_attribute->type_vec);
        bool legal=false;
        bool pointcmp=false;
        if(IS_REAL_TYPE(rela_base_type->typ_category)&&IS_REAL_TYPE(shift_base_type->typ_category))
        {
            legal=true;
            if(const_expr&&shift_expr_node->expr_attribute->const_expr)
            {
                const_expr=true;
                if(IS_INT_TYPE(rela_base_type->typ_category)&&IS_INT_TYPE(shift_base_type->typ_category))
                {
                    long long int rela_base_value=get_int_const(rela_base_type->typ_category,tmp_data_field,true);
                    long long int shift_base_value=get_int_const(shift_base_type->typ_category,shift_expr_node->expr_attribute->data_field,true);
                    if(operator->type==less_than)
                        tmp_data_field->sint=((rela_base_value<shift_base_value)?1:0);
                    else if(operator->type==greater_than)
                        tmp_data_field->sint=((rela_base_value>shift_base_value)?1:0);
                    else if(operator->type==less_equal)
                        tmp_data_field->sint=((rela_base_value<=shift_base_value)?1:0);
                    else if(operator->type==greater_equal)
                        tmp_data_field->sint=((rela_base_value>=shift_base_value)?1:0);
                }
                else if(IS_INT_TYPE(rela_base_type->typ_category)&&IS_FLOAT_TYPE(shift_base_type->typ_category))
                {
                    long long int rela_base_value=get_int_const(rela_base_type->typ_category,tmp_data_field,true);
                    long double shift_base_value=get_float_const(shift_base_type->typ_category,shift_expr_node->expr_attribute->data_field,true);
                    if(operator->type==less_than)
                        tmp_data_field->sint=((rela_base_value<shift_base_value)?1:0);
                    else if(operator->type==greater_than)
                        tmp_data_field->sint=((rela_base_value>shift_base_value)?1:0);
                    else if(operator->type==less_equal)
                        tmp_data_field->sint=((rela_base_value<=shift_base_value)?1:0);
                    else if(operator->type==greater_equal)
                        tmp_data_field->sint=((rela_base_value>=shift_base_value)?1:0);
                }
                else if(IS_FLOAT_TYPE(rela_base_type->typ_category)&&IS_INT_TYPE(shift_base_type->typ_category))
                {
                    long double rela_base_value=get_float_const(rela_base_type->typ_category,tmp_data_field,true);
                    long long int shift_base_value=get_int_const(shift_base_type->typ_category,shift_expr_node->expr_attribute->data_field,true);
                    if(operator->type==less_than)
                        tmp_data_field->sint=(rela_base_value<shift_base_value)?1:0;
                    else if(operator->type==greater_than)
                        tmp_data_field->sint=(rela_base_value>shift_base_value)?1:0;
                    else if(operator->type==less_equal)
                        tmp_data_field->sint=(rela_base_value<=shift_base_value)?1:0;
                    else if(operator->type==greater_equal)
                        tmp_data_field->sint=(rela_base_value>=shift_base_value)?1:0;
                }
                else if(IS_FLOAT_TYPE(rela_base_type->typ_category)&&IS_FLOAT_TYPE(shift_base_type->typ_category))
                {
                    long double rela_base_value=get_float_const(rela_base_type->typ_category,tmp_data_field,true);
                    long double shift_base_value=get_float_const(shift_base_type->typ_category,shift_expr_node->expr_attribute->data_field,true);
                    if(operator->type==less_than)
                        tmp_data_field->sint=(rela_base_value<shift_base_value)?1:0;
                    else if(operator->type==greater_than)
                        tmp_data_field->sint=(rela_base_value>shift_base_value)?1:0;
                    else if(operator->type==less_equal)
                        tmp_data_field->sint=(rela_base_value<=shift_base_value)?1:0;
                    else if(operator->type==greater_equal)
                        tmp_data_field->sint=(rela_base_value>=shift_base_value)?1:0;
                }
            }
            else
                const_expr=false;
        }
        else if(rela_base_type->typ_category==TP_POINT&&shift_base_type->typ_category==TP_POINT)
        {
            pointcmp=true;
            VEC* tmp_rela_vec=Type_VEC_get_Pointer_TO(tmp_type_vec,true);
            VEC* tmp_shift_vec=Type_VEC_get_Pointer_TO(shift_expr_node->expr_attribute->type_vec,true);
            if(compatible_types(tmp_rela_vec,tmp_shift_vec))
                legal=true;
            DelVEC(tmp_rela_vec);
            DelVEC(tmp_shift_vec);
        }
        if(!legal)
        {
            if(pointcmp){
                C_ERROR(C0069_ERR_NEED_COMPATIBLE_TYPE,ast_node);
                goto error;
            }
            else{
                C_ERROR(C0063_ERR_OPERAND_REAL_TYPE,ast_node);
                goto error; 
            }
        }
        
        M_TYPE* tmpt=usual_arith_conversion(&rela_base_type,&shift_base_type);
        tmpt=build_base_type(TP_SINT);
        DelVEC(tmp_type_vec);
        tmp_type_vec=InitVEC(DEFAULT_CAPICITY);
        VECinsert(tmp_type_vec,tmpt);
    }
    ast_node->expr_attribute->type_vec=tmp_type_vec;
    if(const_expr){
        ast_node->expr_attribute->const_expr=true;
        ast_node->expr_attribute->data_size=type_data_size[TP_SINT];
        ast_node->expr_attribute->data_field->sint=tmp_data_field->sint;
    }
    m_free(tei);
    m_free(tmp_data_field);
    /*lvalue = false*/
#ifdef _TEST_SEMANTICS_
    print_expr_type(ast_node);
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end relation expr\n");
    semantics_level--;
#endif
    return true;
error:
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end relation expr fail\n");
    semantics_level--;
#endif
    return false;
}
bool shift_expr_value(AST_BASE* ast_node)
{
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start shift\n");
#endif
    if(!ast_node||ast_node->type!=shift_expr)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM*));
    AST_BASE* shift_expr_node=AST_GET_CHILD(ast_node,0);
    VEC* tmp_type_vec=NULL;
    VALUE_DATA* tmp_data_field=m_alloc(sizeof(VALUE_DATA));
    if(!expr_dispatch(shift_expr_node))
        goto error;
    VECcpy(shift_expr_node->expr_attribute->type_vec,&tmp_type_vec);
    memcpy(tmp_data_field,shift_expr_node->expr_attribute->data_field,sizeof(VALUE_DATA));
    bool const_expr=shift_expr_node->expr_attribute->const_expr;
    for(size_t i=0;i+2<AST_CHILD_NUM(ast_node);i+=2){
        AST_BASE* shift_op_node=AST_GET_CHILD(ast_node,i+1);
        AST_BASE* add_expression_node=AST_GET_CHILD(ast_node,i+2);
        if(!expr_dispatch(add_expression_node))
            goto error;
        M_TYPE* shift_base_type=Type_VEC_get_actual_base_type(tmp_type_vec);
        M_TYPE* add_base_type=Type_VEC_get_actual_base_type(add_expression_node->expr_attribute->type_vec);
        if((!IS_INT_TYPE(shift_base_type->typ_category))||(!IS_INT_TYPE(add_base_type->typ_category)))
        {
            C_ERROR(C0060_ERR_OPERAND_INTEGER_TYPE,ast_node);
            goto error;
        }
        if(integer_promotion(&shift_base_type))
            Type_VEC_change_actual_base_type(tmp_type_vec,shift_base_type);
        integer_promotion(&add_base_type);
        if(const_expr&&add_expression_node->expr_attribute->const_expr){
            long long int shift_base_int=get_int_const(shift_base_type->typ_category,tmp_data_field,true);
            long long int add_base_int=get_int_const(add_base_type->typ_category,add_expression_node->expr_attribute->data_field,true);
            /*for unsigned long long and signed long long ,shift op is the same*/
            if(shift_op_node->type==left_shift)
            {
                tmp_data_field->sllong=shift_base_int<<add_base_int;
            }
            else if(shift_op_node->type==right_shift)
            {
                tmp_data_field->sllong=shift_base_int>>add_base_int;
            }
            cast_const(shift_base_type->typ_category,tmp_data_field,TP_SLONGLONG,tmp_data_field);
        }
    }
    M_TYPE* tmpt=Type_VEC_get_actual_base_type(tmp_type_vec);
    if(const_expr){
        ast_node->expr_attribute->const_expr=true;
        ast_node->expr_attribute->data_size=type_data_size[tmpt->typ_category];
        cast_const(tmpt->typ_category,ast_node->expr_attribute->data_field,TP_SLONGLONG,tmp_data_field);
    }
    ast_node->expr_attribute->type_vec=tmp_type_vec;
    m_free(tei);
    m_free(tmp_data_field);
#ifdef _TEST_SEMANTICS_
    print_expr_type(ast_node);
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end shift\n");
    semantics_level--;
#endif
    return true;
error:
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end shift fail\n");
    semantics_level--;
#endif
    return false;
}
bool add_expr_value(AST_BASE* ast_node)
{
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start add\n");
#endif
    if(!ast_node||ast_node->type!=additive_expr)
        goto error;
    ERROR_ITEM* tei=malloc(sizeof(ERROR_ITEM));
    VEC* tmp_type_vec=NULL;
    AST_BASE* add_expression_node=AST_GET_CHILD(ast_node,0);
    if(!expr_dispatch(add_expression_node))
        goto error;
    VECcpy(add_expression_node->expr_attribute->type_vec,&tmp_type_vec);
    tmp_type_vec=lvalue_convertion(tmp_type_vec);
    bool const_expr=add_expression_node->expr_attribute->const_expr;
    enum TP_CATEGORY tmp_category=TP_SPEC_NONE;
    VALUE_DATA* tmp_data_field=m_alloc(sizeof(VALUE_DATA));
    memcpy(tmp_data_field,add_expression_node->expr_attribute->data_field,sizeof(VALUE_DATA));
    for(size_t i=0;i+2<AST_CHILD_NUM(ast_node);i+=2)
    {
        AST_BASE* operator=AST_GET_CHILD(ast_node,i+1);
        AST_BASE* mul_expression_node=AST_GET_CHILD(ast_node,i+2);
        bool pointer_add=false,ptrdiff=false;
        if(!expr_dispatch(mul_expression_node))
            goto error;
        if(const_expr&&mul_expression_node->expr_attribute->const_expr)
            const_expr=true;
        else
            const_expr=false;
        M_TYPE* add_base_type=Type_VEC_get_actual_base_type(tmp_type_vec);
        VEC* mul_type_vec=lvalue_convertion(mul_expression_node->expr_attribute->type_vec);
        M_TYPE* mul_base_type=Type_VEC_get_actual_base_type(mul_type_vec);
        if(operator->type==plus){
            bool legal=false;
            if(IS_ARTH_TYPE(add_base_type->typ_category)&&IS_ARTH_TYPE(mul_base_type->typ_category))
                legal=true;
            else{
                const_expr=false;
                pointer_add=true;
                VEC* tmpv=NULL;
                if(IS_INT_TYPE(add_base_type->typ_category)&&mul_base_type->typ_category==TP_POINT)
                {
                    tmpv=Type_VEC_get_Pointer_TO(mul_expression_node->expr_attribute->type_vec,true);
                }
                if(IS_INT_TYPE(mul_base_type->typ_category)&&add_base_type->typ_category==TP_POINT)
                {
                    tmpv=Type_VEC_get_Pointer_TO(tmp_type_vec,true);
                }
                M_TYPE* tmpt=Type_VEC_get_actual_base_type(tmpv);
                if(tmpt&&tmpt->typ_category!=TP_VOID&&tmpt->complete){
                    legal=true;
                }
                if(tmpv)
                   DelVEC(tmpv);
            }
            if(!legal){
                C_ERROR(C0067_ERR_ADD_OPERAND_TYPE,ast_node);
                goto error;
            }
        }
        else if(operator->type==minus){
            bool legal=false;
            if(IS_ARTH_TYPE(add_base_type->typ_category)&&IS_ARTH_TYPE(mul_base_type->typ_category))
                legal=true;
            else if(add_base_type->typ_category==TP_POINT&&mul_base_type->typ_category==TP_POINT){
                printf("both are pointer\n");
                const_expr=false;
                ptrdiff=true;
                VEC* tmpa,*tmpm;
                tmpa=Type_VEC_get_Pointer_TO(tmp_type_vec,true);
                tmpm=Type_VEC_get_Pointer_TO(mul_expression_node->expr_attribute->type_vec,true);
                if(!compatible_types(tmpa,tmpm)){
                    C_ERROR(C0069_ERR_NEED_COMPATIBLE_TYPE,ast_node);
                    goto error;
                }
                legal=true;
            }
            else{
                const_expr=false;
                pointer_add=true;
                VEC* tmpv=NULL;
                if(IS_INT_TYPE(mul_base_type->typ_category)&&add_base_type->typ_category==TP_POINT)
                {
                    tmpv=Type_VEC_get_Pointer_TO(tmp_type_vec,true);
                }
                M_TYPE* tmpt=Type_VEC_get_actual_base_type(tmpv);
                if(tmpt&&tmpt->typ_category!=TP_VOID&&tmpt->complete){
                    legal=true;
                }
                if(tmpv)
                    DelVEC(tmpv);
            }
            if(!legal){
                C_ERROR(C0068_ERR_MINUS_OPERAND_TYPE,ast_node);
                goto error;
            }
        }
        
        if(const_expr)      /*you must calculate this before type cast*/
        {
            /*I feels sad about this code,*/
            if(IS_INT_TYPE(add_base_type->typ_category)&&IS_INT_TYPE(mul_base_type->typ_category))
            {
                long long int add_base_value=get_int_const(add_base_type->typ_category,tmp_data_field,true);
                long long int mul_base_value=get_int_const(mul_base_type->typ_category,mul_expression_node->expr_attribute->data_field,true);
                if(add_base_type->typ_category==TP_USLONGLONG||mul_base_type->typ_category==TP_USLONGLONG)
                {
                    if(operator->type==plus)
                        tmp_data_field->usllong=add_base_value+mul_base_value;
                    else if(operator->type==minus)
                        tmp_data_field->usllong=add_base_value-mul_base_value;
                    tmp_category=TP_USLONGLONG;
                }
                else
                {
                    if(operator->type==plus)
                        tmp_data_field->sllong=add_base_value+mul_base_value;
                    else if(operator->type==minus)
                        tmp_data_field->sllong=add_base_value-mul_base_value;
                    tmp_category=TP_SLONGLONG;
                }
            }
            else if(IS_INT_TYPE(add_base_type->typ_category)&&IS_FLOAT_TYPE(mul_base_type->typ_category))
            {
                long long int add_base_value=get_int_const(add_base_type->typ_category,tmp_data_field,true);
                long double mul_base_value=get_float_const(mul_base_type->typ_category,mul_expression_node->expr_attribute->data_field,true);
                if(operator->type==plus)
                    tmp_data_field->datalongdouble=add_base_value+mul_base_value;
                else if(operator->type==minus)
                    tmp_data_field->datalongdouble=add_base_value-mul_base_value;
                tmp_category=TP_LONG_DOUBLE;
            }
            else if(IS_FLOAT_TYPE(add_base_type->typ_category)&&IS_INT_TYPE(mul_base_type->typ_category))
            {
                long double add_base_value=get_float_const(add_base_type->typ_category,tmp_data_field,true);
                long long int mul_base_value=get_int_const(mul_base_type->typ_category,mul_expression_node->expr_attribute->data_field,true);
                if(operator->type==plus)
                    tmp_data_field->datalongdouble=add_base_value+mul_base_value;
                else if(operator->type==minus)
                    tmp_data_field->datalongdouble=add_base_value-mul_base_value;
                tmp_category=TP_LONG_DOUBLE;
            }
            else if(IS_FLOAT_TYPE(add_base_type->typ_category)&&IS_FLOAT_TYPE(mul_base_type->typ_category))
            {
                long double add_base_value=get_float_const(add_base_type->typ_category,tmp_data_field,true);
                long double mul_base_value=get_float_const(mul_base_type->typ_category,mul_expression_node->expr_attribute->data_field,true);
                if(operator->type==plus)
                    tmp_data_field->datalongdouble=add_base_value+mul_base_value;
                else if(operator->type==minus)
                    tmp_data_field->datalongdouble=add_base_value-mul_base_value;
                tmp_category=TP_LONG_DOUBLE;
            }
#if _CPLX_SUPPORT==1 
            else if(IS_INT_TYPE(add_base_type->typ_category)&&IS_COMPLEX_TYPE(mul_base_type->typ_category))
            {
                long long int add_base_value=get_int_const(add_base_type->typ_category,tmp_data_field,true);
                long double* mul_base_value=get_complex_const(mul_base_type->typ_category,mul_expression_node->expr_attribute->data_field,true);
                if(operator->type==plus)
                    tmp_data_field->longdoublecomplex[0]=(long double)add_base_value+*mul_base_value;
                else if(operator->type==minus)
                    tmp_data_field->longdoublecomplex[0]=(long double)add_base_value-*mul_base_value;
                tmp_data_field->longdoublecomplex[1]=*(mul_base_value+1);
                tmp_category=TP_LONG_DOUBLE_COMPLEX;
            }
            else if(IS_COMPLEX_TYPE(add_base_type->typ_category)&&IS_INT_TYPE(mul_base_type->typ_category))
            {
                long double* add_base_value=get_complex_const(add_base_type->typ_category,tmp_data_field,true);
                long long int mul_base_value=get_int_const(mul_base_type->typ_category,mul_expression_node->expr_attribute->data_field,true);
                if(operator->type==plus)
                    tmp_data_field->longdoublecomplex[0]=*add_base_value+(long double)mul_base_value;
                else if(operator->type==minus)
                    tmp_data_field->longdoublecomplex[0]=*add_base_value-(long double)mul_base_value;
                tmp_data_field->longdoublecomplex[1]=*(add_base_value+1);
                tmp_category=TP_LONG_DOUBLE_COMPLEX;
            }
            else if(IS_COMPLEX_TYPE(add_base_type->typ_category)&&IS_FLOAT_TYPE(mul_base_type->typ_category))
            {
                long double* add_base_value=get_complex_const(add_base_type->typ_category,tmp_data_field,true);
                long double mul_base_value=get_float_const(mul_base_type->typ_category,mul_expression_node->expr_attribute->data_field,true);
                if(operator->type==plus)
                    tmp_data_field->longdoublecomplex[0]=*add_base_value+mul_base_value;
                else if(operator->type==minus)
                    tmp_data_field->longdoublecomplex[0]=*add_base_value-mul_base_value;
                tmp_data_field->longdoublecomplex[1]=*(add_base_value+1);
                tmp_category=TP_LONG_DOUBLE_COMPLEX;
            }
            else if(IS_FLOAT_TYPE(add_base_type->typ_category)&&IS_COMPLEX_TYPE(mul_base_type->typ_category))
            {
                long double add_base_value=get_float_const(add_base_type->typ_category,tmp_data_field,true);
                long double* mul_base_value=get_complex_const(mul_base_type->typ_category,mul_expression_node->expr_attribute->data_field,true);
                if(operator->type==plus)
                    tmp_data_field->longdoublecomplex[0]=add_base_value+*mul_base_value;
                else if(operator->type==minus)
                    tmp_data_field->longdoublecomplex[0]=add_base_value-*mul_base_value;
                tmp_data_field->longdoublecomplex[1]=*(mul_base_value+1);
                tmp_category=TP_LONG_DOUBLE_COMPLEX;
            }
            else if(IS_COMPLEX_TYPE(add_base_type->typ_category)&&IS_COMPLEX_TYPE(mul_base_type->typ_category))
            {
                long double* add_base_value=get_complex_const(add_base_type->typ_category,tmp_data_field,true);
                long double* mul_base_value=get_complex_const(mul_base_type->typ_category,mul_expression_node->expr_attribute->data_field,true);
                if(operator->type==plus)
                {
                    tmp_data_field->longdoublecomplex[0]=*add_base_value+*mul_base_value;
                    tmp_data_field->longdoublecomplex[1]=*(add_base_value+1)+*(mul_base_value+1);
                }
                else if(operator->type==minus)
                {
                    tmp_data_field->longdoublecomplex[0]=*add_base_value-*mul_base_value;
                    tmp_data_field->longdoublecomplex[1]=*(add_base_value+1)-*(mul_base_value+1);
                }
                tmp_category=TP_LONG_DOUBLE_COMPLEX;
            }
#endif
        }
        M_TYPE* tmpt;
        if(pointer_add){
            /*the result should be a pointer*/
            tmpt=build_base_type(TP_POINT);
        }
        else if(ptrdiff){
            /*the result should have ptrdiff_t,which means 'long int',if you change this type,change code here*/
            tmpt=build_base_type(TP_SLONG);
        }
        else{
            /*promotion*/
            tmpt=usual_arith_conversion(&add_base_type,&mul_base_type);
        }
        if(const_expr)
            cast_const(tmpt->typ_category,tmp_data_field,tmp_category,tmp_data_field);
        if(tmpt)
            Type_VEC_change_actual_base_type(tmp_type_vec,tmpt);
    }
    ast_node->expr_attribute->type_vec=tmp_type_vec;
    M_TYPE* tmpt=Type_VEC_get_actual_base_type(tmp_type_vec);
    if(const_expr){
        /*const expr case*/
        ast_node->expr_attribute->const_expr=true;
        ast_node->expr_attribute->data_size=type_data_size[tmpt->typ_category];
        cast_const(tmpt->typ_category,ast_node->expr_attribute->data_field,tmp_category,tmp_data_field);
        /*const expr data*/
    }
    /*The result should not be a lvalue*/
    m_free(tei);
    m_free(tmp_data_field);
#ifdef _TEST_SEMANTICS_
    print_expr_type(ast_node);
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end add\n");
    semantics_level--;
#endif
    return true;
error:
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end add fail\n");
    semantics_level--;
#endif
    return false;
}
bool mul_expr_value(AST_BASE* ast_node)
{
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start mul\n");
#endif
    if(!ast_node||ast_node->type!=multi_expr)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    AST_BASE* mul_expression_node=AST_GET_CHILD(ast_node,0);
    if(!expr_dispatch(mul_expression_node)){
        goto error;
    }
    VEC* tmp_type_vec=NULL;
    bool const_expr=mul_expression_node->expr_attribute->const_expr;
    enum TP_CATEGORY tmp_category=TP_SPEC_NONE;
    VECcpy(mul_expression_node->expr_attribute->type_vec,&tmp_type_vec);
    VALUE_DATA* tmp_data_field=m_alloc(sizeof(VALUE_DATA));
    memcpy(tmp_data_field,mul_expression_node->expr_attribute->data_field,sizeof(VALUE_DATA));
    for(size_t i=0;i+2<AST_CHILD_NUM(ast_node);i+=2){
        AST_BASE* operator=AST_GET_CHILD(ast_node,i+1);
        AST_BASE* cast_expression_node=AST_GET_CHILD(ast_node,i+2);
        M_TYPE* mul_expr_type,*cast_expr_type;
        if(!expr_dispatch(cast_expression_node))
        {
            goto error;
        }
        
        mul_expr_type=Type_VEC_get_actual_base_type(tmp_type_vec);
        if(!IS_ARTH_TYPE(mul_expr_type->typ_category)){
            C_ERROR(C0061_ERR_OPERAND_ARTHMATIC_TYPE,ast_node);
            goto error;
        }
        if(operator->type==percent){
            if(!IS_INT_TYPE(mul_expr_type->typ_category)){
                C_ERROR(C0060_ERR_OPERAND_INTEGER_TYPE,ast_node);
                goto error;
            }
        }
        cast_expr_type=Type_VEC_get_actual_base_type(cast_expression_node->expr_attribute->type_vec);
        if(!IS_ARTH_TYPE(cast_expr_type->typ_category)){
            C_ERROR(C0061_ERR_OPERAND_ARTHMATIC_TYPE,ast_node);
            goto error;
        }
        if(operator->type==percent){
            if(!IS_INT_TYPE(cast_expr_type->typ_category)){
                C_ERROR(C0060_ERR_OPERAND_INTEGER_TYPE,ast_node);
                goto error;
            }
        }
        if(const_expr&&cast_expression_node->expr_attribute->const_expr)
        {
            /*TODO: mul const*/
            if(IS_INT_TYPE(mul_expr_type->typ_category)&&IS_INT_TYPE(cast_expr_type->typ_category))
            {
                long long int mul_expr_value=get_int_const(mul_expr_type->typ_category,tmp_data_field,true);
                long long int cast_expr_value=get_int_const(cast_expr_type->typ_category,cast_expression_node->expr_attribute->data_field,true);
                if(mul_expr_type->typ_category==TP_USLONGLONG||cast_expr_type->typ_category==TP_USLONGLONG)
                {
                    if(operator->type==star)
                        tmp_data_field->usllong=mul_expr_value*cast_expr_value;
                    else if(operator->type==forward_slash)
                        tmp_data_field->usllong=mul_expr_value/cast_expr_value;
                    else if(operator->type==percent)
                        tmp_data_field->usllong=mul_expr_value%cast_expr_value;
                    tmp_category=TP_USLONGLONG;
                }
                else
                {
                    if(operator->type==star)
                        tmp_data_field->sllong=mul_expr_value*cast_expr_value;
                    else if(operator->type==forward_slash)
                        tmp_data_field->sllong=mul_expr_value/cast_expr_value;
                    else if(operator->type==percent)
                        tmp_data_field->usllong=mul_expr_value%cast_expr_value;
                    tmp_category=TP_SLONGLONG;
                }
            }
            else if(IS_INT_TYPE(mul_expr_type->typ_category)&&IS_FLOAT_TYPE(cast_expr_type->typ_category))
            {
                long long int mul_expr_value=get_int_const(mul_expr_type->typ_category,tmp_data_field,true);
                long double cast_expr_value=get_float_const(cast_expr_type->typ_category,cast_expression_node->expr_attribute->data_field,true);
                if(operator->type==star)
                    tmp_data_field->datalongdouble=mul_expr_value*cast_expr_value;
                else if(operator->type==forward_slash)
                    tmp_data_field->datalongdouble=mul_expr_value/cast_expr_value;
                tmp_category=TP_LONG_DOUBLE;
            }
            else if(IS_FLOAT_TYPE(mul_expr_type->typ_category)&&IS_INT_TYPE(cast_expr_type->typ_category))
            {
                long double mul_expr_value=get_float_const(mul_expr_type->typ_category,tmp_data_field,true);
                long long int cast_expr_value=get_int_const(cast_expr_type->typ_category,cast_expression_node->expr_attribute->data_field,true);
                if(operator->type==star)
                    tmp_data_field->datalongdouble=mul_expr_value*cast_expr_value;
                else if(operator->type==forward_slash)
                    tmp_data_field->datalongdouble=mul_expr_value/cast_expr_value;
                tmp_category=TP_LONG_DOUBLE;
            }
            else if(IS_FLOAT_TYPE(mul_expr_type->typ_category)&&IS_FLOAT_TYPE(cast_expr_type->typ_category))
            {
                long double mul_expr_value=get_float_const(mul_expr_type->typ_category,tmp_data_field,true);
                long double cast_expr_value=get_float_const(cast_expr_type->typ_category,cast_expression_node->expr_attribute->data_field,true);
                if(operator->type==star)
                    tmp_data_field->datalongdouble=mul_expr_value*cast_expr_value;
                else if(operator->type==forward_slash)
                    tmp_data_field->datalongdouble=mul_expr_value/cast_expr_value;
                tmp_category=TP_LONG_DOUBLE;
            }
#if _CPLX_SUPPORT==1
            /*
                mul:(a+bi)*(c+di)=(ac-bd)+(ad+bc)i
                div:(a+bi)/(c+di)=(ac+bd)/(c^2+d^2)+(bc-ad)/(c^2+d^2)i
            */
            else if(IS_INT_TYPE(mul_expr_type->typ_category)&&IS_COMPLEX_TYPE(cast_expr_type->typ_category))
            {
                long long int mul_expr_value=get_int_const(mul_expr_type->typ_category,tmp_data_field,true);
                long double* cast_expr_value=get_complex_const(cast_expr_type->typ_category,cast_expression_node->expr_attribute->data_field,true);
                if(operator->type==star)
                {
                    tmp_data_field->longdoublecomplex[0]=( (long double) mul_expr_value ) * (*cast_expr_value);
                    tmp_data_field->longdoublecomplex[1]=( (long double) mul_expr_value ) * (*(cast_expr_value+1));
                }
                else if(operator->type==forward_slash)
                {
                    long double denominator=(*cast_expr_value)*(*cast_expr_value)+(*(cast_expr_value+1))*(*(cast_expr_value+1));
                    tmp_data_field->longdoublecomplex[0]=( (long double) mul_expr_value ) * (*cast_expr_value) / denominator;
                    tmp_data_field->longdoublecomplex[1]=-( (long double) mul_expr_value ) * (*(cast_expr_value+1)) / denominator;
                }
                tmp_category=TP_LONG_DOUBLE_COMPLEX;
            }
            else if(IS_COMPLEX_TYPE(mul_expr_type->typ_category)&&IS_INT_TYPE(cast_expr_type->typ_category))
            {
                long double* mul_expr_value=get_complex_const(mul_expr_type->typ_category,tmp_data_field,true);
                long long int cast_expr_value=get_int_const(cast_expr_type->typ_category,cast_expression_node->expr_attribute->data_field,true);
                if(operator->type==star)
                {
                    tmp_data_field->longdoublecomplex[0]=( *mul_expr_value ) * ( (long double)cast_expr_value );
                    tmp_data_field->longdoublecomplex[1]=( *(mul_expr_value+1) ) * ( (long double)cast_expr_value );
                }
                else if(operator->type==forward_slash)
                {
                    long double denominator=((long double)cast_expr_value)*((long double)cast_expr_value);
                    tmp_data_field->longdoublecomplex[0]=( *mul_expr_value ) * ( (long double)cast_expr_value ) / denominator;
                    tmp_data_field->longdoublecomplex[1]=( *(mul_expr_value+1) ) * ( (long double)cast_expr_value ) / denominator;
                }
                tmp_category=TP_LONG_DOUBLE_COMPLEX;
            }
            else if(IS_COMPLEX_TYPE(mul_expr_type->typ_category)&&IS_FLOAT_TYPE(cast_expr_type->typ_category))
            {
                long double* mul_expr_value=get_complex_const(mul_expr_type->typ_category,tmp_data_field,true);
                long double cast_expr_value=get_float_const(cast_expr_type->typ_category,cast_expression_node->expr_attribute->data_field,true);
                if(operator->type==star)
                {
                    tmp_data_field->longdoublecomplex[0]=( *mul_expr_value ) * ( cast_expr_value );
                    tmp_data_field->longdoublecomplex[1]=( *(mul_expr_value+1) ) * ( cast_expr_value );
                }
                else if(operator->type==forward_slash)
                {
                    long double denominator=(cast_expr_value)*(cast_expr_value);
                    tmp_data_field->longdoublecomplex[0]=( *mul_expr_value ) * ( cast_expr_value ) / denominator;
                    tmp_data_field->longdoublecomplex[1]=( *(mul_expr_value+1) ) * ( cast_expr_value ) / denominator;
                }
                tmp_category=TP_LONG_DOUBLE_COMPLEX;
            }
            else if(IS_FLOAT_TYPE(mul_expr_type->typ_category)&&IS_COMPLEX_TYPE(cast_expr_type->typ_category))
            {
                long double mul_expr_value=get_float_const(mul_expr_type->typ_category,tmp_data_field,true);
                long double* cast_expr_value=get_complex_const(cast_expr_type->typ_category,cast_expression_node->expr_attribute->data_field,true);
                if(operator->type==star)
                {
                    tmp_data_field->longdoublecomplex[0]=( (long double) mul_expr_value ) * (*cast_expr_value);
                    tmp_data_field->longdoublecomplex[1]=( (long double) mul_expr_value ) * (*(cast_expr_value+1));
                }
                else if(operator->type==forward_slash)
                {
                    long double denominator=(*cast_expr_value)*(*cast_expr_value)+(*(cast_expr_value+1))*(*(cast_expr_value+1));
                    tmp_data_field->longdoublecomplex[0]=( (long double) mul_expr_value ) * (*cast_expr_value) / denominator;
                    tmp_data_field->longdoublecomplex[1]=-( (long double) mul_expr_value ) * (*(cast_expr_value+1)) / denominator;
                }
                tmp_category=TP_LONG_DOUBLE_COMPLEX;
            }
            else if(IS_COMPLEX_TYPE(mul_expr_type->typ_category)&&IS_COMPLEX_TYPE(cast_expr_type->typ_category))
            {
                long double* mul_expr_value=get_complex_const(mul_expr_type->typ_category,tmp_data_field,true);
                long double* cast_expr_value=get_complex_const(cast_expr_type->typ_category,cast_expression_node->expr_attribute->data_field,true);
                if(operator->type==star)
                {
                    tmp_data_field->longdoublecomplex[0]=(*mul_expr_value)*(*cast_expr_value)-(*(mul_expr_value+1))*(*(cast_expr_value+1));
                    tmp_data_field->longdoublecomplex[1]=(*mul_expr_value)*(*(cast_expr_value+1))+(*(mul_expr_value+1))*(*cast_expr_value);
                }
                else if(operator->type==forward_slash)
                {
                    long double denominator=(*cast_expr_value)*(*cast_expr_value)+(*(cast_expr_value+1))*(*(cast_expr_value+1));
                    tmp_data_field->longdoublecomplex[0]=((*mul_expr_value)*(*cast_expr_value)+(*(mul_expr_value+1))*(*(cast_expr_value+1)))/denominator;
                    tmp_data_field->longdoublecomplex[1]=((*(mul_expr_value+1))*(*cast_expr_value)-(*mul_expr_value)*(*(cast_expr_value+1)))/denominator;
                }
                tmp_category=TP_LONG_DOUBLE_COMPLEX;
            }
#endif
        }
        else{
            const_expr=false;
        }
        /*promotion*/
        M_TYPE* tmpt=usual_arith_conversion(&mul_expr_type,&cast_expr_type);
        if(const_expr)
            cast_const(tmpt->typ_category,tmp_data_field,tmp_category,tmp_data_field);
        if(tmpt)
            Type_VEC_change_actual_base_type(tmp_type_vec,tmpt);
    }
    ast_node->expr_attribute->type_vec=tmp_type_vec;
    /*obveriously, the lvalue is false*/
    M_TYPE* tmpt=Type_VEC_get_actual_base_type(tmp_type_vec);
    if(const_expr){
        ast_node->expr_attribute->const_expr=true;
        ast_node->expr_attribute->data_size=type_data_size[tmpt->typ_category];
        cast_const(tmpt->typ_category,ast_node->expr_attribute->data_field,tmp_category,tmp_data_field);
        /*TODO:const expr value*/
    }
    m_free(tei);
    m_free(tmp_data_field);
#ifdef _TEST_SEMANTICS_
    print_expr_type(ast_node);
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end mul\n");
    semantics_level--;
#endif
    return true;
error:
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end mul fail\n");
    semantics_level--;
#endif
    return false;
}
bool cast_expr_value(AST_BASE* ast_node)
{
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start cast\n");
#endif
    if(!ast_node||ast_node->type!=cast_expr)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    VEC* type_name_vec;
    VEC* operand_vec;
    AST_BASE* type_name_node=AST_GET_CHILD(ast_node,1);
    AST_BASE* operand_node=AST_GET_CHILD(ast_node,3);
    if(!(type_name_vec=type_name_type(type_name_node)))
        goto error;
    if(!expr_dispatch(operand_node))
        goto error;
    VECcpy(operand_node->expr_attribute->type_vec,&operand_vec);
    /*check types*/
    M_TYPE* tmp_tn_type=Type_VEC_get_actual_base_type(type_name_vec);
    if(!(tmp_tn_type->typ_category==TP_VOID)&&!IS_SCALAR_TYPE(tmp_tn_type->typ_category))
    {
        C_ERROR(C0062_ERR_OPERAND_SCALAR_TYPE,operand_node);
        goto error;
    }
    M_TYPE* tmp_op_type=Type_VEC_get_actual_base_type(operand_vec);
    if(!IS_SCALAR_TYPE(tmp_op_type->typ_category))
    {
        C_ERROR(C0062_ERR_OPERAND_SCALAR_TYPE,operand_node);
        goto error;
    }
    if((tmp_tn_type->typ_category==TP_POINT&&IS_FLOAT_TYPE(tmp_op_type->typ_category))||
    (tmp_op_type->typ_category==TP_POINT&&IS_FLOAT_TYPE(tmp_tn_type->typ_category)))
    {
        C_ERROR(C0066_ERR_CAST_CONVERT_POINT_FLOAT,operand_node);
        goto error;
    }
    DelVEC(operand_vec);
    ast_node->expr_attribute->type_vec=type_name_vec;
    if(operand_node->expr_attribute->const_expr)
    {
        ast_node->expr_attribute->const_expr=true;
        ast_node->expr_attribute->data_size=type_data_size[tmp_tn_type->typ_category];
        cast_const(tmp_tn_type->typ_category,ast_node->expr_attribute->data_field,tmp_op_type->typ_category,operand_node->expr_attribute->data_field);
    }
    /*A cast does not yield an lvalue*/
    m_free(tei);
#ifdef _TEST_SEMANTICS_
    print_expr_type(ast_node);
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end cast\n");
    semantics_level--;
#endif
    return true;
error:
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end cast fail\n");
    semantics_level--;
#endif
    return false;
}
bool unary_expr_value(AST_BASE* ast_node)
{
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start unary\n");
#endif
    if(!ast_node||ast_node->type!=unary_expr)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    AST_BASE* operator_node=AST_GET_CHILD(ast_node,0);
    AST_BASE* operand_node=NULL;
    VEC* tmp_type_vec=NULL;
    M_TYPE* tmp_base_type=NULL;
    bool is_lvalue=false;
    bool const_expr=false;
    switch(operator_node->type){
        case double_plus:
        {
            operand_node=AST_GET_CHILD(ast_node,1);
            if(!expr_dispatch(operand_node))
                goto error;
            VECcpy(operand_node->expr_attribute->type_vec,&tmp_type_vec);
            if(!operand_node->expr_attribute->is_lvalue)
            {
                C_ERROR(C0051_ERR_LVALUE_REQUIRED,operand_node);
                goto error;
            }
            if(!Type_VEC_modify_lvalue(tmp_type_vec,operand_node->expr_attribute->is_lvalue)){
                C_ERROR(C0056_ERR_EXPR_MODIFIABLE_LVALUE,operand_node);
                goto error;
            }
            tmp_base_type=Type_VEC_get_actual_base_type(tmp_type_vec);
            if(!IS_REAL_TYPE(tmp_base_type->typ_category)&&tmp_base_type->typ_category!=TP_POINT)
            {
                C_ERROR(C0052_ERR_EXPR_TYPE_ERROR,operand_node);
                goto error;
            }
            if(integer_promotion(&tmp_base_type))
                Type_VEC_change_actual_base_type(tmp_type_vec,tmp_base_type);
            is_lvalue=false;
            break;
        }
        case double_minus:
        {
            operand_node=AST_GET_CHILD(ast_node,1);
            if(!expr_dispatch(operand_node))
                goto error;
            VECcpy(operand_node->expr_attribute->type_vec,&tmp_type_vec);
            if(!operand_node->expr_attribute->is_lvalue)
            {
                C_ERROR(C0051_ERR_LVALUE_REQUIRED,operand_node);
                goto error;
            }
            if(!Type_VEC_modify_lvalue(tmp_type_vec,operand_node->expr_attribute->is_lvalue)){
                C_ERROR(C0056_ERR_EXPR_MODIFIABLE_LVALUE,operand_node);
                goto error;
            }
            tmp_base_type=Type_VEC_get_actual_base_type(tmp_type_vec);
            if(!IS_REAL_TYPE(tmp_base_type->typ_category)&&tmp_base_type->typ_category!=TP_POINT)
            {
                C_ERROR(C0052_ERR_EXPR_TYPE_ERROR,operand_node);
                goto error;
            }
            if(integer_promotion(&tmp_base_type))
                Type_VEC_change_actual_base_type(tmp_type_vec,tmp_base_type);
            is_lvalue=false;
            break;
        }
        case ampersand:
        {
            operand_node=AST_GET_CHILD(ast_node,1);
            if(!expr_dispatch(operand_node))
                goto error;
            if(!operand_node->expr_attribute->is_lvalue)
            {
                C_ERROR(C0051_ERR_LVALUE_REQUIRED,operand_node);
                goto error;
            }
            VECcpy(operand_node->expr_attribute->type_vec,&tmp_type_vec);
            if(operand_node->expr_attribute->is_bit_field)
            {
                C_ERROR(C0057_ERR_OPERAND_BIT_FIELD,operand_node);
                goto error;
            }
            M_TYPE* tmpt=Type_VEC_get_spec_other(tmp_type_vec);
            if(tmpt&&tmpt->typ_stor==TP_REGISTER){
                C_ERROR(C0058_ERR_OPERAND_REGISTER,operand_node);
                goto error;
            }
            M_TYPE* tmp_star=build_base_type(TP_POINT);
            VECinsert(tmp_type_vec,tmp_star);
            is_lvalue=false;
            break;
        }
        case star:
        {
            operand_node=AST_GET_CHILD(ast_node,1);
            if(!expr_dispatch(operand_node))
                goto error;
            VECcpy(operand_node->expr_attribute->type_vec,&tmp_type_vec);
            tmp_type_vec=lvalue_convertion(tmp_type_vec);
            M_TYPE* tmpt=Type_VEC_get_actual_base_type(tmp_type_vec);
            if(tmpt->typ_category!=TP_POINT)
            {
                C_ERROR(C0059_ERR_OPERAND_SHOULD_POINTER,operand_node);
                goto error;
            }
            VEC* old_l=tmp_type_vec;
            tmp_type_vec=Type_VEC_get_Pointer_TO(tmp_type_vec,true);
            DelVEC(old_l);
            is_lvalue=true;
            break;
        }
        case plus:
        {
            operand_node=AST_GET_CHILD(ast_node,1);
            if(!expr_dispatch(operand_node))
                goto error;
            VECcpy(operand_node->expr_attribute->type_vec,&tmp_type_vec);
            M_TYPE* tmpt=Type_VEC_get_actual_base_type(tmp_type_vec);
            if(!IS_ARTH_TYPE(tmpt->typ_category)){
                C_ERROR(C0061_ERR_OPERAND_ARTHMATIC_TYPE,operand_node);
                goto error;
            }
            if(operand_node->expr_attribute->const_expr)
            {
                const_expr=true;
                plus_const(tmpt->typ_category,ast_node->expr_attribute->data_field,operand_node->expr_attribute->data_field);
            }
            if(integer_promotion(&tmpt))   /*no need to check the return*/
                Type_VEC_change_actual_base_type(tmp_type_vec,tmpt);
            is_lvalue=false;
            break;
        }
        case minus:
        {
            operand_node=AST_GET_CHILD(ast_node,1);
            if(!expr_dispatch(operand_node))
                goto error;
            VECcpy(operand_node->expr_attribute->type_vec,&tmp_type_vec);
            M_TYPE* tmpt=Type_VEC_get_actual_base_type(tmp_type_vec);
            if(!IS_ARTH_TYPE(tmpt->typ_category)){
                C_ERROR(C0061_ERR_OPERAND_ARTHMATIC_TYPE,operand_node);
                goto error;
            }
            if(operand_node->expr_attribute->const_expr)
            {
                const_expr=true;
                minus_const(tmpt->typ_category,ast_node->expr_attribute->data_field,operand_node->expr_attribute->data_field);
            }
            if(integer_promotion(&tmpt))
                Type_VEC_change_actual_base_type(tmp_type_vec,tmpt);
            is_lvalue=false;
            break;
        }
        case tilde:
        {
            operand_node=AST_GET_CHILD(ast_node,1);
            if(!expr_dispatch(operand_node))
                goto error;
            VECcpy(operand_node->expr_attribute->type_vec,&tmp_type_vec);
            M_TYPE* tmpt=Type_VEC_get_actual_base_type(tmp_type_vec);
            if(!IS_INT_TYPE(tmpt->typ_category)){
                C_ERROR(C0060_ERR_OPERAND_INTEGER_TYPE,operand_node);
                goto error;
            }
            if(operand_node->expr_attribute->const_expr)
            {
                const_expr=true;
                tilde_const(tmpt->typ_category,ast_node->expr_attribute->data_field,operand_node->expr_attribute->data_field);
            }
            if(integer_promotion(&tmpt))
                Type_VEC_change_actual_base_type(tmp_type_vec,tmpt);
            is_lvalue=false;
            break;
        }
        case exclamation:
        {
            operand_node=AST_GET_CHILD(ast_node,1);
            if(!expr_dispatch(operand_node))
                goto error;
            VECcpy(operand_node->expr_attribute->type_vec,&tmp_type_vec);
            M_TYPE* tmpt=Type_VEC_get_actual_base_type(tmp_type_vec);
            if(!IS_SCALAR_TYPE(tmpt->typ_category)){
                C_ERROR(C0062_ERR_OPERAND_SCALAR_TYPE,operand_node);
                goto error;
            }
            if(operand_node->expr_attribute->const_expr)
            {
                const_expr=true;
                ast_node->expr_attribute->data_field->sint=exclamation_const(tmpt->typ_category,operand_node->expr_attribute->data_field);
            }
            DelVEC(tmp_type_vec);
            tmp_type_vec=InitVEC(DEFAULT_CAPICITY);
            /*the result type should be int*/
            tmpt=build_base_type(TP_SINT);
            VECinsert(tmp_type_vec,(void*)tmpt);
            is_lvalue=false;
            break;
        }
        case KW_sizeof:
        {
            operand_node=AST_GET_CHILD(ast_node,1);
            if(operand_node->type==left_parenthesis){
                operand_node=AST_GET_CHILD(ast_node,2);
            }
            if(operand_node->type!=type_name){
                if(!expr_dispatch(operand_node))
                    goto error;
                VECcpy(operand_node->expr_attribute->type_vec,&tmp_type_vec);
                if(operand_node->expr_attribute->is_bit_field){
                    C_ERROR(C0057_ERR_OPERAND_BIT_FIELD,operand_node);
                    goto error;
                }
            }
            else{
                tmp_type_vec=type_name_type(operand_node);
            }
            M_TYPE* tmpt=Type_VEC_get_actual_base_type(tmp_type_vec);
            if(tmpt->typ_category==TP_FUNCTION){
                C_ERROR(C0065_ERR_SIZEOF_FUNCTION,operand_node);
                goto error;
            }
            else if(tmpt->typ_category==TP_VOID){
                C_ERROR(C0064_ERR_SIZEOF_INCOMPLETE,operand_node);
                goto error;
            }
            else if((tmpt->typ_category==TP_STRUCT||tmpt->typ_category==TP_UNION)&&tmpt->complete==false){
                C_ERROR(C0064_ERR_SIZEOF_INCOMPLETE,operand_node);
                goto error;
            }
            else if(tmpt->typ_category==TP_ARRAY&&!tmpt->complete)
            {
                C_ERROR(C0064_ERR_SIZEOF_INCOMPLETE,operand_node);
                goto error;
            }
            if(!Type_VEC_VLA(tmp_type_vec)){
                ast_node->expr_attribute->data_size=type_data_size[TP_SINT];
                ast_node->expr_attribute->data_field->sint=(int)Type_size(tmp_type_vec);
                const_expr=true;
            }
            DelVEC(tmp_type_vec);
            tmp_type_vec=InitVEC(DEFAULT_CAPICITY);
            /*the result type should be int*/
            tmpt=build_base_type(TP_SINT);
            VECinsert(tmp_type_vec,(void*)tmpt);
            is_lvalue=false;
            break;
        }
        case KW__Alignof:
        {
            operand_node=AST_GET_CHILD(ast_node,2);
            tmp_type_vec=type_name_type(operand_node);
            M_TYPE* tmpt=Type_VEC_get_actual_base_type(tmp_type_vec);
            if(tmpt->typ_category==TP_FUNCTION){
                C_ERROR(C0065_ERR_SIZEOF_FUNCTION,operand_node);
                goto error;
            }
            else if(tmpt->typ_category==TP_VOID){
                C_ERROR(C0064_ERR_SIZEOF_INCOMPLETE,operand_node);
                goto error;
            }
            else if((tmpt->typ_category==TP_STRUCT||tmpt->typ_category==TP_UNION)&&tmpt->complete==false){
                C_ERROR(C0064_ERR_SIZEOF_INCOMPLETE,operand_node);
                goto error;
            }
            else if(tmpt->typ_category==TP_ARRAY&&!tmpt->complete)
            {
                C_ERROR(C0064_ERR_SIZEOF_INCOMPLETE,operand_node);
                goto error;
            }
            ast_node->expr_attribute->data_size=type_data_size[TP_SINT];
            ast_node->expr_attribute->data_field->sint=(int)Type_align(tmp_type_vec);
            const_expr=true;
            DelVEC(tmp_type_vec);
            tmp_type_vec=InitVEC(DEFAULT_CAPICITY);
            /*the result type should be int*/
            tmpt=build_base_type(TP_SINT);
            VECinsert(tmp_type_vec,(void*)tmpt);
            is_lvalue=false;
            break;
        }
        default:
            /*error,but impossible*/
            goto error;
    }
    ast_node->expr_attribute->type_vec=tmp_type_vec;
    ast_node->expr_attribute->is_lvalue=is_lvalue;
    ast_node->expr_attribute->const_expr=const_expr;
    m_free(tei);
#ifdef _TEST_SEMANTICS_
    print_expr_type(ast_node);
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end unary\n");
    semantics_level--;
#endif
    return true;
error:
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end unary fail\n");
    semantics_level--;
#endif
    return false;
}
bool postfix_expr_value(AST_BASE* ast_node)
{
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start postfix expr\n");
#endif
    if(!ast_node||ast_node->type!=postfix_expr)
        goto error;
    VEC* tmp_l_type_vec=NULL;
    VEC* tmp_r_type_vec=NULL;
    VEC* old_l,*old_r;
    M_TYPE* tmp_l_base_type=NULL;
    M_TYPE* tmp_r_base_type=NULL;
    bool is_lvalue=false;
    bool is_bit_field=false;
    bool primary_fail=false;
    char* implicit_func_name=NULL;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    /*step 1:
        get the postfix part of the postfix expr,it might be a primary expr or a (type-name) {init-list}
        that is the base type part
    */
    AST_BASE* first_node=AST_GET_CHILD(ast_node,0);
    int suffix_start_index=0;
    if(first_node&&first_node->type==primary_expression){
        suffix_start_index=1;
        if(!pri_expr_value(first_node))
        {
            primary_fail=true;
            implicit_func_name=first_node->token->value;
        }
        VECcpy(first_node->expr_attribute->type_vec,&tmp_l_type_vec);
        is_lvalue=first_node->expr_attribute->is_lvalue;
    }
    else if(first_node&&first_node->type==left_parenthesis){
        AST_BASE* type_name_node=NULL;
        AST_BASE* initializer_list_node=NULL;
        for(size_t i=0;i<AST_CHILD_NUM(ast_node);++i){
            AST_BASE* tmp_ast_node=AST_GET_CHILD(ast_node,i);
            if(tmp_ast_node->type==type_name)
                type_name_node=tmp_ast_node;
            if(tmp_ast_node->type==initializer_list)
                initializer_list_node=tmp_ast_node;
            if(tmp_ast_node->type==right_brace){
                suffix_start_index=i+1;
                break;
            }
        }
        /*get the type based on the compound literal*/
        tmp_l_type_vec=type_name_type(type_name_node);
        /*TODO: initializer part*/
        if(initializer_list_node)
        {
            if(!initializer_list_semantic(initializer_list_node,tmp_l_type_vec))
                goto error;
        }
        is_lvalue=true;
    }
    else
        goto error;   /*impossible*/
    /*step 2:
        check the second part of the postfix expr and change the type vec
    */
    while(suffix_start_index<AST_CHILD_NUM(ast_node)){
        /*step 3:
            do some convert of the type
        */
        AST_BASE* tmp_ast=AST_GET_CHILD(ast_node,suffix_start_index);
        if(tmp_ast->type==left_bracket){    /*Array case*/
            if(primary_fail)
                goto error;
            tmp_ast=AST_GET_CHILD(ast_node,suffix_start_index+1);
            if(!expr_dispatch(tmp_ast))
                goto error;
            VECcpy(tmp_ast->expr_attribute->type_vec,&tmp_r_type_vec);
            /*lvalue convertion*/
            old_l=tmp_l_type_vec;
            old_r=tmp_r_type_vec;
            tmp_l_type_vec=lvalue_convertion(tmp_l_type_vec);
            tmp_r_type_vec=lvalue_convertion(tmp_r_type_vec);
            if(old_l!=tmp_l_type_vec){
                DelVEC(old_l);
                old_l=tmp_l_type_vec;
            }
            if(old_r!=tmp_r_type_vec){
                DelVEC(old_r);
                old_r=tmp_r_type_vec;
            }
            /*judge the operands*/
            tmp_l_base_type=Type_VEC_get_actual_base_type(tmp_l_type_vec);
            tmp_r_base_type=Type_VEC_get_actual_base_type(tmp_r_type_vec);
            if(tmp_l_base_type->typ_category==TP_POINT&&IS_INT_TYPE(tmp_r_base_type->typ_category)){
                VEC* pointer_to=Type_VEC_get_Pointer_TO(tmp_l_type_vec,true);
                M_TYPE* pointer_actual_type=Type_VEC_get_actual_base_type(pointer_to);
                if(pointer_actual_type->complete){
                    tmp_l_type_vec=pointer_to;
                }
                else{
                    C_ERROR(C0048_ERR_LVALUE_ARR_TYPE_COMPLETE,tmp_ast);
                    goto error;
                }
            }
            else if(tmp_r_base_type->typ_category==TP_POINT&&IS_INT_TYPE(tmp_l_base_type->typ_category)){
                VEC* pointer_to=Type_VEC_get_Pointer_TO(tmp_r_type_vec,true);
                M_TYPE* pointer_actual_type=Type_VEC_get_actual_base_type(pointer_to);
                if(pointer_actual_type->complete){
                    tmp_l_type_vec=pointer_to;
                }
                else{
                    C_ERROR(C0048_ERR_LVALUE_ARR_TYPE_COMPLETE,tmp_ast);
                    goto error;
                }
            }
            else{
                C_ERROR(C0049_ERR_LVALUE_ARR_TYPE_WRONG,tmp_ast);
                goto error;
            }
            DelVEC(old_l);
            DelVEC(old_r);
            suffix_start_index+=3;
            is_lvalue=true;
        }
        else if(tmp_ast->type==left_parenthesis){   /*Function case:*/
            VEC* parameters=InitVEC(DEFAULT_CAPICITY);
            VEC* parameter_type_vecs=InitVEC(DEFAULT_CAPICITY);
            /*lvalue conversion*/
            old_l=tmp_l_type_vec;
            tmp_l_type_vec=lvalue_convertion(tmp_l_type_vec);
            if(old_l!=tmp_l_type_vec)
                DelVEC(old_l);
            /*get all the parameters*/
            for(size_t i=suffix_start_index;i<AST_CHILD_NUM(ast_node);++i){
                tmp_ast=AST_GET_CHILD(ast_node,i);
                if(IS_EXPR_NODE(tmp_ast->type)){
                    VECinsert(parameters,(void*)tmp_ast);
                    if(!expr_dispatch(tmp_ast))
                        return false;
                    VEC* tmp_type_vec=lvalue_convertion(tmp_ast->expr_attribute->type_vec);
                    VECinsert(parameter_type_vecs,(void*)tmp_type_vec);
                }
                if(tmp_ast->type==right_parenthesis)
                {
                    suffix_start_index=i+1;
                    break;
                }
            }
            if(primary_fail)
            {
                /*means no such a function struct*/
                ERROR_ITEM* twi=m_alloc(sizeof(ERROR_ITEM));
                C_WARN(W0005_WARN_IMPLICIT_DEC,ast_node);
                M_TYPE* new_return_type=build_base_type(TP_SINT);
                TP_FUNC* new_func=m_alloc(sizeof(TP_FUNC));
                tmp_l_type_vec=InitVEC(DEFAULT_CAPICITY);
                VECinsert(tmp_l_type_vec,(void*)new_return_type);
                /*build the func struct and insert it into the current symbol table*/
                new_func->func_name=implicit_func_name;
                new_func->is_old_style=true;
                new_func->modifier=false;
                new_func->typ_category=TP_FUNCTION;
                new_func->have_ellipsis=false;
                new_func->count=HASH_CNT_IST;
                new_func->complete=true;
                new_func->data_size=type_data_size[TP_FUNCTION];
                new_func->func_parameters=NULL; /*for old style ,the func parameter is useless*/
                new_func->key=TypeToKey((M_TYPE*)new_func);
                VECinsert(tmp_l_type_vec,(void*)new_func);
                SYM_ITEM* tmpsi=Create_symbol_item(implicit_func_name,ast_node->name_space);
                tmpsi->count=HASH_CNT_IST;
                tmpsi->linkage=LKA_EXTERN;
                tmpsi->type_vec=InitVEC(DEFAULT_CAPICITY);
                VECinsert(tmpsi->type_vec,(void*)new_return_type);
                VECinsert(tmpsi->type_vec,(void*)new_func);
                insert_symbol(ast_node->symbol_table,tmpsi);
                /*function parameter promotion,default argument promotion*/
                for(size_t i=0;i<VECLEN(parameter_type_vecs);++i)
                {
                    VEC* tmp_type_vec=VEC_GET_ITEM(parameter_type_vecs,i);
                    M_TYPE* tmpt=Type_VEC_get_actual_base_type(tmp_type_vec);
                    if(!integer_promotion(&tmpt)){
                        if(float_promotion(&tmpt))
                            Type_VEC_change_actual_base_type(tmp_type_vec,tmpt);
                    }
                    else
                        Type_VEC_change_actual_base_type(tmp_type_vec,tmpt);
                }
            }
            else{
                tmp_l_base_type=Type_VEC_get_actual_base_type(tmp_l_type_vec);
                if(tmp_l_base_type->typ_category!=TP_POINT){
                    C_ERROR(C0050_ERR_LVALUE_FUNC_TYPE_WRONG,tmp_ast);
                    goto error;
                }
                old_l=tmp_l_type_vec;
                tmp_l_type_vec=Type_VEC_get_Pointer_TO(tmp_l_type_vec,true);
                if(old_l!=tmp_l_type_vec)
                    DelVEC(old_l);
                tmp_l_base_type=Type_VEC_get_actual_base_type(tmp_l_type_vec);
                if(tmp_l_base_type->typ_category!=TP_FUNCTION){
                    C_ERROR(C0050_ERR_LVALUE_FUNC_TYPE_WRONG,tmp_ast);
                    goto error;
                }
                VEC* func_para=((TP_FUNC*)tmp_l_base_type)->func_parameters;
                /*no need to check the return type, it have been done in other places*/
                if(((TP_FUNC*)tmp_l_base_type)->have_ellipsis)  /*it cannot be an old style function*/
                {
                    if(VECLEN(func_para)>VECLEN(parameters))
                    {
                        C_ERROR(C0073_ERR_FUNC_MORE_PARAMETERS,tmp_ast);
                        goto error;
                    }
                    size_t i=0;
                    for(;i<VECLEN(func_para);++i)
                    {
                        TP_FUNC_PARA* tmp_fun_para=VEC_GET_ITEM(func_para,i);
                        VEC* func_para_type_vec=NULL;
                        VECcpy(tmp_fun_para->type_vec,&func_para_type_vec);
                        VEC* tmp_type_vec=VEC_GET_ITEM(parameter_type_vecs,i);
                        /*TODO:promotion*/
                        if(!assignment_type_check(func_para_type_vec,tmp_type_vec))
                        {
                            C_ERROR(C0074_ERR_FUNC_PARA_TYPE,tmp_ast);
                            goto error;
                        }
                    }
                    for(;i<VECLEN(parameter_type_vecs);++i)
                    {
                        VEC* tmp_type_vec=VEC_GET_ITEM(parameter_type_vecs,i);
                        M_TYPE* tmpt=Type_VEC_get_actual_base_type(tmp_type_vec);
                        if(!integer_promotion(&tmpt)){
                            if(float_promotion(&tmpt))
                                Type_VEC_change_actual_base_type(tmp_type_vec,tmpt);
                        }
                        else
                            Type_VEC_change_actual_base_type(tmp_type_vec,tmpt);
                    }
                }
                else{   /*have prototype*/
                    if(((TP_FUNC*)tmp_l_base_type)->is_old_style)   /*it usually occured at an old style function after an old style function*/
                    {
                        /*do not check ,just promotion*/
                        for(size_t i=0;i<VECLEN(parameter_type_vecs);++i)
                        {
                            VEC* tmp_type_vec=VEC_GET_ITEM(parameter_type_vecs,i);
                            M_TYPE* tmpt=Type_VEC_get_actual_base_type(tmp_type_vec);
                            if(!integer_promotion(&tmpt)){
                                if(float_promotion(&tmpt))
                                    Type_VEC_change_actual_base_type(tmp_type_vec,tmpt);
                            }
                            else
                                Type_VEC_change_actual_base_type(tmp_type_vec,tmpt);
                        }
                    }
                    else
                    {
                        if(VECLEN(func_para)!=VECLEN(parameters))
                        {
                            C_ERROR(C0073_ERR_FUNC_MORE_PARAMETERS,tmp_ast);
                            goto error;
                        }
                        for(size_t i=0;i<VECLEN(func_para);++i)
                        {
                            TP_FUNC_PARA* tmp_fun_para=VEC_GET_ITEM(func_para,i);
                            VEC* func_para_type_vec=NULL;
                            VECcpy(tmp_fun_para->type_vec,&func_para_type_vec);
                            VEC* tmp_type_vec=VEC_GET_ITEM(parameter_type_vecs,i);
                            /*TODO:promotion*/
                            if(!assignment_type_check(func_para_type_vec,tmp_type_vec))
                            {
                                C_ERROR(C0074_ERR_FUNC_PARA_TYPE,tmp_ast);
                                goto error;
                            }
                        }
                    }
                }
            }
            old_l=tmp_l_type_vec;
            tmp_l_type_vec=Type_VEC_get_func_return_type(tmp_l_type_vec,true);
            if(old_l!=tmp_l_type_vec)
                DelVEC(old_l);
            is_lvalue=true;
        }
        else if(tmp_ast->type==dot){    /*struct*/
            if(primary_fail)
                goto error;
            old_l=tmp_l_type_vec;
            AST_BASE* member_name_node=AST_GET_CHILD(ast_node,suffix_start_index+1);
            tmp_l_base_type=Type_VEC_get_actual_base_type(tmp_l_type_vec);
            M_TYPE* tmp_l_qual_type=Type_VEC_get_qual(tmp_l_type_vec);
            if(tmp_l_base_type->typ_category!=TP_STRUCT
            &&tmp_l_base_type->typ_category!=TP_UNION
            &&tmp_l_base_type->typ_category!=TP_UNION_STRUCT){
                C_ERROR(C0053_ERR_EXPR_STRUCT_UNION_DOT_TYPE_WRONG,tmp_ast);
                goto error;
            }
            TP_SU* tmp_su=(TP_SU*)tmp_l_base_type;
            bool have_member=false;
            for(size_t i=0;i<VECLEN(tmp_su->members);++i){
                TP_SU_MEMBER* tmp_member=VEC_GET_ITEM(tmp_su->members,i);
                if(!member_name_node->token->value||!tmp_member->member_name)
                    continue;
                if(strcmp(member_name_node->token->value,tmp_member->member_name)==0){
                    VECcpy(tmp_member->type_vec,&tmp_l_type_vec);
                    if(old_l!=tmp_l_type_vec){
                        DelVEC(old_l);
                        old_l=tmp_l_type_vec;
                    }
                    have_member=true;
                    if(tmp_member->bit_field)
                        is_bit_field=true;
                }
            }
            if(!have_member)
            {
                C_ERROR(C0053_ERR_EXPR_STRUCT_UNION_DOT_TYPE_WRONG,tmp_ast);
                goto error;
            }
            if(tmp_l_qual_type){
                M_TYPE* tmp_r_qual_type=Type_VEC_get_qual(tmp_l_type_vec);
                if(tmp_r_qual_type){
                    tmp_r_qual_type->type_qual+=tmp_l_qual_type->type_qual;
                }
                else{
                    VECinsert(tmp_l_type_vec,(void*)tmp_l_qual_type);
                }
            }
            tmp_l_base_type=Type_VEC_get_actual_base_type(tmp_l_type_vec);
            if(tmp_l_base_type->typ_category==TP_ENUM)
            {
                M_TYPE* tmpt=build_base_type(TP_SINT);
                Type_VEC_change_actual_base_type(tmp_l_type_vec,tmpt);
            }
            if(is_lvalue)
                is_lvalue=true;
            suffix_start_index+=2;
        }
        else if(tmp_ast->type==point){  /*point to struct*/
            if(primary_fail)
                goto error;
            old_l=tmp_l_type_vec;
            AST_BASE* member_name_node=AST_GET_CHILD(ast_node,suffix_start_index+1);
            tmp_l_base_type=Type_VEC_get_actual_base_type(tmp_l_type_vec);
            if(tmp_l_base_type->typ_category!=TP_POINT)
            {
                C_ERROR(C0054_ERR_EXPR_STRUCT_UNION_POINT_TYPE_WRONG,tmp_ast);
                goto error;
            }
            tmp_l_type_vec=Type_VEC_get_Pointer_TO(tmp_l_type_vec,true);
            if(old_l!=tmp_l_type_vec){
                DelVEC(old_l);
                old_l=tmp_l_type_vec;
            }
            tmp_l_base_type=Type_VEC_get_actual_base_type(tmp_l_type_vec);
            M_TYPE* tmp_l_qual_type=Type_VEC_get_qual(tmp_l_type_vec);
            if(tmp_l_base_type->typ_category!=TP_STRUCT
            &&tmp_l_base_type->typ_category!=TP_UNION
            &&tmp_l_base_type->typ_category!=TP_UNION_STRUCT){
                C_ERROR(C0054_ERR_EXPR_STRUCT_UNION_POINT_TYPE_WRONG,tmp_ast);
                goto error;
            }
            TP_SU* tmp_su=(TP_SU*)tmp_l_base_type;
            bool have_member=false;
            for(size_t i=0;i<VECLEN(tmp_su->members);++i){
                TP_SU_MEMBER* tmp_member=VEC_GET_ITEM(tmp_su->members,i);
                if(!member_name_node->token->value||!tmp_member->member_name)
                    continue;
                if(strcmp(member_name_node->token->value,tmp_member->member_name)==0){
                    VECcpy(tmp_member->type_vec,&tmp_l_type_vec);
                    if(old_l!=tmp_l_type_vec){
                        DelVEC(old_l);
                        old_l=tmp_l_type_vec;
                    }
                    have_member=true;
                    if(tmp_member->bit_field)
                        is_bit_field=true;
                }
            }
            if(!have_member)
            {
                C_ERROR(C0054_ERR_EXPR_STRUCT_UNION_POINT_TYPE_WRONG,tmp_ast);
                goto error;
            }
            if(tmp_l_qual_type){
                M_TYPE* tmp_r_qual_type=Type_VEC_get_qual(tmp_l_type_vec);
                if(tmp_r_qual_type){
                    tmp_r_qual_type->type_qual+=tmp_l_qual_type->type_qual;
                }
                else{
                    VECinsert(tmp_l_type_vec,(void*)tmp_l_qual_type);
                }
            }
            tmp_l_base_type=Type_VEC_get_actual_base_type(tmp_l_type_vec);
            if(tmp_l_base_type->typ_category==TP_ENUM)
            {
                M_TYPE* tmpt=build_base_type(TP_SINT);
                Type_VEC_change_actual_base_type(tmp_l_type_vec,tmpt);
            }
            is_lvalue=true;
            suffix_start_index+=2;
        }
        else if(tmp_ast->type==double_plus){    /* ++ */
            if(primary_fail)
                goto error;
            tmp_l_base_type=Type_VEC_get_actual_base_type(tmp_l_type_vec);
            if(!IS_REAL_TYPE(tmp_l_base_type->typ_category)&&tmp_l_base_type->typ_category!=TP_POINT){
                C_ERROR(C0055_ERR_EXPR_POSTFIX_DOUBLE_P_M_TYPE,tmp_ast);
                goto error;
            }
            if(integer_promotion(&tmp_l_base_type))
                Type_VEC_change_actual_base_type(tmp_l_type_vec,tmp_l_base_type);
            if(!is_lvalue)
            {
                C_ERROR(C0051_ERR_LVALUE_REQUIRED,tmp_ast);
                goto error;
            }
            if(!Type_VEC_modify_lvalue(tmp_l_type_vec,is_lvalue))
            {
                C_ERROR(C0056_ERR_EXPR_MODIFIABLE_LVALUE,tmp_ast);
                goto error;
            }
            is_lvalue=false;
            is_bit_field=false;
            suffix_start_index+=1;
        }
        else if(tmp_ast->type==double_minus){   /* -- */
            if(primary_fail)
                goto error;
            tmp_l_base_type=Type_VEC_get_actual_base_type(tmp_l_type_vec);
            if(!IS_REAL_TYPE(tmp_l_base_type->typ_category)&&tmp_l_base_type->typ_category!=TP_POINT){
                C_ERROR(C0055_ERR_EXPR_POSTFIX_DOUBLE_P_M_TYPE,tmp_ast);
                goto error;
            }
            if(integer_promotion(&tmp_l_base_type))
                Type_VEC_change_actual_base_type(tmp_l_type_vec,tmp_l_base_type);
            if(!is_lvalue)
            {
                C_ERROR(C0051_ERR_LVALUE_REQUIRED,tmp_ast);
                goto error;
            }
            if(!Type_VEC_modify_lvalue(tmp_l_type_vec,is_lvalue))
            {
                C_ERROR(C0056_ERR_EXPR_MODIFIABLE_LVALUE,tmp_ast);
                goto error;
            }
            is_lvalue=false;
            is_bit_field=false;
            suffix_start_index+=1;
        }
        else
            goto error;
    }
    /*TODO:compound literal part decide the const expr*/
    ast_node->expr_attribute->const_expr=false;
    ast_node->expr_attribute->type_vec=tmp_l_type_vec;
    ast_node->expr_attribute->is_lvalue=is_lvalue;
    ast_node->expr_attribute->is_bit_field=is_bit_field;
    m_free(tei);
#ifdef _TEST_SEMANTICS_
    print_expr_type(ast_node);
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end postfix expr\n");
    semantics_level--;
#endif
    return true;
error:
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end postfix expr fail\n");
    semantics_level--;
#endif
    return false;
}
bool pri_expr_value(AST_BASE* ast_node)
{
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start primary expr\n");
#endif
    if(!ast_node||ast_node->type!=primary_expression)
        goto error;
    AST_BASE* child_node=NULL;
    CONST_VALUE* cv=NULL;
    if(AST_CHILD_NUM(ast_node)==1){
        child_node=AST_GET_CHILD(ast_node,0);
        switch (child_node->type)
        {
        case identifier:    /*find the symbol and use that to fill the ast node*/
        {
            ast_node->expr_attribute->const_expr=false;
            SYM_ITEM* find_tmpsi=find_symbol(ast_node->symbol_table,child_node->token->value,NMSP_DEFAULT);
            /*provided it has been declared as designating an object or a function, the namespace must be NMSP_DEFAULT */
            if(find_tmpsi){
                VEC* tmpsi_type_vec=find_tmpsi->type_vec;
                M_TYPE* tmp_type=Type_VEC_get_actual_base_type(tmpsi_type_vec);
                if(tmp_type->typ_category==TP_TYPE_DEF_TYPE)
                {
                    VECcpy(((TP_DEF_TYPE*)tmp_type)->typedef_name_type,&(ast_node->expr_attribute->type_vec));
                    ast_node->expr_attribute->data_size=find_tmpsi->data_size;
                    memcpy(ast_node->expr_attribute->data_field,find_tmpsi->data_field,sizeof(VALUE_DATA));
                    ast_node->expr_attribute->const_expr=find_tmpsi->const_expr;
                    ast_node->token->symbol_item=(void*)find_tmpsi;
                    ast_node->expr_attribute->is_lvalue=false;
                }
                else if(tmp_type->typ_category==TP_ENUM&&find_tmpsi->name_space!=NMSP_SU_TAG)
                {
                    ast_node->expr_attribute->type_vec=InitVEC(DEFAULT_CAPICITY);
                    M_TYPE* tmpt=build_base_type(TP_SINT);
                    VECinsert(ast_node->expr_attribute->type_vec,tmpt);
                    ast_node->expr_attribute->data_size=find_tmpsi->data_size;
                    ast_node->expr_attribute->data_field->sint=find_tmpsi->data_field->sint;
                    ast_node->expr_attribute->const_expr=find_tmpsi->const_expr;
                    ast_node->token->symbol_item=(void*)find_tmpsi;
                    ast_node->expr_attribute->is_lvalue=true;
                }
                else{
                    VECcpy(find_tmpsi->type_vec,&(ast_node->expr_attribute->type_vec));
                    ast_node->expr_attribute->data_size=find_tmpsi->data_size;
                    memcpy(ast_node->expr_attribute->data_field,find_tmpsi->data_field,sizeof(VALUE_DATA));  
                    ast_node->expr_attribute->const_expr=find_tmpsi->const_expr;
                    ast_node->token->symbol_item=(void*)find_tmpsi;
                    ast_node->expr_attribute->is_lvalue=true;
                }
                goto succ;
            }
            else{
                ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
                VECinsert(c_error,m_error_item(
                    tei,
                    C0016_ERR_UNDECLARATION,
                    filename,
                    ast_node->token->line,
                    ast_node->token->value
                ));
                const_illegal_error(ast_node);
                goto error;
            }
        }
        case integer_constant:
        {
            cv=token_to_const_value(ast_node->token);
            goto fill_ast_data;
        }
        case floating_constant:
        {
            cv=token_to_const_value(ast_node->token);
            goto fill_ast_data;
        }
        case enum_const:
        {
            ast_node->expr_attribute->const_expr=true;
            AST_BASE* enum_const_node=AST_GET_CHILD(ast_node,0);
            SYM_ITEM* tmpsi;
            tmpsi=find_symbol(ast_node->symbol_table,enum_const_node->token->value,NMSP_DEFAULT);
            if(HASH_ITEM_EXIST(((HASH_ITEM*)tmpsi))){
                ast_node->expr_attribute->const_expr=true;
                ast_node->expr_attribute->data_size=type_data_size[TP_SINT];
                ast_node->expr_attribute->data_field->sint=tmpsi->data_field->sint;
                M_TYPE* tmpt=build_base_type(TP_SINT);
                if(tmpt)
                {
                    ast_node->expr_attribute->type_vec=InitVEC(DEFAULT_CAPICITY);
                    VECinsert(ast_node->expr_attribute->type_vec,(void*)tmpt);
                }
            }
            goto error;
        }
        case char_const:
        {
            cv=token_to_const_value(ast_node->token);
            goto fill_ast_data;
        }
        case string:
        {
            ast_node->expr_attribute->const_expr=false;
            ast_node->expr_attribute->is_lvalue=true;
            ast_node->expr_attribute->complete=true;
            ast_node->expr_attribute->is_bit_field=false;
            ast_node->expr_attribute->is_lvalue=false;
            ast_node->expr_attribute->type_vec=InitVEC(DEFAULT_CAPICITY);
            M_TYPE* tmpt=build_base_type(TP_SCHAR);
            VECinsert(ast_node->expr_attribute->type_vec,tmpt);
            tmpt=build_base_type(TP_POINT);
            VECinsert(ast_node->expr_attribute->type_vec,tmpt);
            ast_node->expr_attribute->data_size=child_node->token->value_len*type_data_size[TP_SCHAR];
            
            /*TODO,data field*/
            goto succ;
        }
        case generic_selection:
        {
            ast_node->expr_attribute->const_expr=false;
            /*TODO*/
            goto succ;
        }
        default:
            goto error;
        }
    }
    else if(AST_CHILD_NUM(ast_node)==3){
        child_node=AST_GET_CHILD(ast_node,1);
        if(!expr_dispatch(child_node))
            goto error;
        VECcpy(child_node->expr_attribute->type_vec,&(ast_node->expr_attribute->type_vec));
        ast_node->expr_attribute->is_lvalue=child_node->expr_attribute->is_lvalue;
        ast_node->expr_attribute->complete=child_node->expr_attribute->complete;
        ast_node->expr_attribute->const_expr=child_node->expr_attribute->const_expr;
        ast_node->expr_attribute->is_bit_field=child_node->expr_attribute->is_bit_field;
        ast_node->expr_attribute->data_size=child_node->expr_attribute->data_size;
        memcpy(ast_node->expr_attribute->data_field,child_node->expr_attribute->data_field,sizeof(VALUE_DATA));
    }
succ:
#ifdef _TEST_SEMANTICS_
    print_expr_type(ast_node);
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end primary expr\n");
    semantics_level--;
#endif
        return true;
error:
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end primary expr fail\n");
    semantics_level--;
#endif
    return false;
fill_ast_data:
    if(cv==NULL)
        goto error;
    ast_node->expr_attribute->const_expr=true;
    ast_node->expr_attribute->data_size=type_data_size[cv->const_expr_type];
    memcpy(ast_node->expr_attribute->data_field,cv->const_value,sizeof(VALUE_DATA));
    M_TYPE* tmpt=build_base_type(cv->const_expr_type);
    if(tmpt)
    {
        ast_node->expr_attribute->type_vec=InitVEC(DEFAULT_CAPICITY);
        VECinsert(ast_node->expr_attribute->type_vec,(void*)tmpt);
    }
    else
        goto error;
    goto succ;
}
void const_illegal_error(AST_BASE* ast_node){
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    VECinsert(c_error,m_error_item(
        tei,
        C0019_ERR_ILLEGAL_CONST_EXPR,
        filename,
        ast_node->token->line,
        ast_node->token->value
    ));
}
void print_expr_type(AST_BASE* node){
#ifdef _TEST_SEMANTICS_
    for(int i=0;i<semantics_level;++i)
        printf("    ");
    printf("<Type>:");
    if(!(node->expr_attribute)||!(node->expr_attribute->type_vec)||VECLEN(node->expr_attribute->type_vec)==0)
        printf("this expr node has no type!\n");
    else{
        print_type_vec(node->expr_attribute->type_vec);
    }
#endif
}
#endif