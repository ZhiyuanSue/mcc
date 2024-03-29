#include "transexpr.h"
extern size_t type_data_size[TYPE_NUM];
bool expr_trans_dispatch(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    bool (*expr_translation[EXPR_NUM])(AST_BASE* ast_node,IR_BB* ir_bb)={
        expr_trans,assign_expr_trans,cond_expr_trans,logical_or_expr_trans,
        logical_and_expr_trans,bit_inclusive_or_expr_trans,bit_exclusive_or_expr_trans,and_expr_trans,
        equal_expr_trans,relation_expr_trans,shift_expr_trans,add_expr_trans,
        mul_expr_trans,cast_expr_trans,unary_expr_trans,postfix_expr_trans,
        pri_expr_trans
    };
    bool res=false;
    if(ast_node->type==expression)
        res=expr_translation[0](ast_node,ir_bb);
    else if(ast_node->type>=assignment_expr&&ast_node->type<=primary_expression){
        res=expr_translation[ast_node->type-assignment_expr+1](ast_node,ir_bb);
    }
    else
        goto error;
    if(res==false)
        printf("expr trans dispatch %s\n",rule_type_str[ ast_node->type]);
    return res;
error:
    return false;
}
bool cond_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=conditional_expr)
        goto error;
    if(ast_node->symbol->const_expr)
        return true;
    return true;
error:
    return false;
}
bool expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    if(ast_node->symbol->const_expr)
        return true;
    return true;
error:
    return false;
}
bool assign_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=assignment_expr)
        goto error;
    if(ast_node->symbol->const_expr)
        return true;
    return true;
error:
    return false;
}
bool assign_trans(SYM_ITEM* src_symbol,
    SYM_ITEM* dst_symbol,
    IR_BB* ir_bb)
{
    /*at semantic stage,the const expr value case have been dealed*/
    if(src_symbol->const_expr&&dst_symbol->const_expr)
        return true;
    else if(src_symbol->const_expr||dst_symbol->const_expr)
        return false;   /*only one is const_expr,the semantic must have error*/
    else{
        M_TYPE* src_base_type=Type_VEC_get_actual_base_type(src_symbol->type_vec);
        M_TYPE* dst_base_type=Type_VEC_get_actual_base_type(dst_symbol->type_vec);
        if(!src_base_type||!dst_base_type)
        {
            printf("symbol type error!\n"); /*impossible if everything OK*/
            return false;
        }
        IR_INS* ir_ins=add_new_ins(ir_bb);
        /*
            for arthmatic case,do some conversion
            but the target type have been decided by previous stage,so no need to do it
            besides, at the int type case,you must consider the bit-field problem
            but,for one is a bit-field of a struct(as a int type,it cannot be a float),another one is a float case
            I haven't find any word in C11 std(maybe it's my fault,so if you find it,please tell me)
            I also didn't find any warning or error in gcc(e.x. assign a double to an i1 type)
            so I just convert the float to int and then trunc it and assign,as other case did
        */
    }
    return true;
}
bool logical_or_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=logical_or_expr)
        goto error;
    if(ast_node->symbol->const_expr)
        return true;
    bool prefix_const_expr=true;
    AST_BASE* prefix_expr_node=NULL;
    AST_BASE* right_expr_node=NULL;
    SYM_ITEM* prefix_symbol=NULL;
    SYM_ITEM* right_symbol=NULL;
    for(size_t i=1;i+1<AST_CHILD_NUM(ast_node);i+=2)
    {
        AST_BASE* operator_node=AST_GET_CHILD(ast_node,i);
        /*if the beginning sub expr are const expr, just use them directly*/    
        if(prefix_const_expr)
        {
            if(operator_node->symbol->const_expr)
            {
                prefix_expr_node=operator_node;
                continue;
            }
            else
                prefix_const_expr=false;
        }
        if(prefix_expr_node==NULL)
            prefix_expr_node=AST_GET_CHILD(ast_node,i-1);
        right_expr_node=AST_GET_CHILD(ast_node,i+1);
        if(!expr_trans_dispatch(right_expr_node,ir_bb))
            goto error;
        /*if not a bool type,do some cast*/
        prefix_symbol=prefix_expr_node->symbol;
        right_symbol=right_expr_node->symbol;
        M_TYPE* tmp_prefix_expr_type=Type_VEC_get_actual_base_type(prefix_expr_node->symbol->type_vec);
        if(tmp_prefix_expr_type->typ_category!=TP_BOOL)
        {
            prefix_symbol=Create_symbol_item(tmp_symbol_str_alloc(".reg."),NMSP_DEFAULT);
            prefix_symbol->count=HASH_CNT_IST;
            insert_symbol(ast_node->symbol_table,prefix_symbol);
            tmp_prefix_expr_type=build_base_type(TP_BOOL);
            VECinsert(prefix_symbol->type_vec,(void*)tmp_prefix_expr_type);
            type_cast_trans(prefix_symbol,prefix_expr_node->symbol,ir_bb);
        }
        M_TYPE* tmp_right_expr_type=Type_VEC_get_actual_base_type(right_expr_node->symbol->type_vec);
        if(tmp_right_expr_type->typ_category!=TP_BOOL)
        {
            right_symbol=Create_symbol_item(tmp_symbol_str_alloc(".reg."),NMSP_DEFAULT);
            right_symbol->count=HASH_CNT_IST;
            insert_symbol(ast_node->symbol_table,right_symbol);
            tmp_right_expr_type=build_base_type(TP_BOOL);
            VECinsert(right_symbol->type_vec,(void*)tmp_right_expr_type);
            type_cast_trans(right_symbol,right_expr_node->symbol,ir_bb);
        }
        IR_INS* logical_or_ins=add_new_ins(ir_bb);
        insert_ins_to_bb(logical_or_ins,ir_bb);
        GenINS(logical_or_ins,OP_OR,operator_node->symbol,prefix_symbol,right_symbol);
        prefix_expr_node=operator_node;
    }
    AST_BASE* res_node=AST_GET_CHILD(ast_node,AST_CHILD_NUM(ast_node)-2);
    type_cast_trans(ast_node->symbol,res_node->symbol,ir_bb);
    return true;
error:
    return false;
}
bool logical_and_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=logical_and_expr)
        goto error;
    if(ast_node->symbol->const_expr)
        return true; 
    bool prefix_const_expr=true;
    AST_BASE* prefix_expr_node=NULL;
    AST_BASE* right_expr_node=NULL;
    SYM_ITEM* prefix_symbol=NULL;
    SYM_ITEM* right_symbol=NULL;
    for(size_t i=1;i+1<AST_CHILD_NUM(ast_node);i+=2)
    {
        AST_BASE* operator_node=AST_GET_CHILD(ast_node,i);
        /*if the beginning sub expr are const expr, just use them directly*/    
        if(prefix_const_expr)
        {
            if(operator_node->symbol->const_expr)
            {
                prefix_expr_node=operator_node;
                continue;
            }
            else
                prefix_const_expr=false;
        }
        if(prefix_expr_node==NULL)
            prefix_expr_node=AST_GET_CHILD(ast_node,i-1);
        right_expr_node=AST_GET_CHILD(ast_node,i+1);
        if(!expr_trans_dispatch(right_expr_node,ir_bb))
            goto error;
        prefix_symbol=prefix_expr_node->symbol;
        right_symbol=right_expr_node->symbol;
        M_TYPE* tmp_prefix_expr_type=Type_VEC_get_actual_base_type(prefix_expr_node->symbol->type_vec);
        if(tmp_prefix_expr_type->typ_category!=TP_BOOL)
        {
            prefix_symbol=Create_symbol_item(tmp_symbol_str_alloc(".reg."),NMSP_DEFAULT);
            prefix_symbol->count=HASH_CNT_IST;
            insert_symbol(ast_node->symbol_table,prefix_symbol);
            tmp_prefix_expr_type=build_base_type(TP_BOOL);
            VECinsert(prefix_symbol->type_vec,(void*)tmp_prefix_expr_type);
            type_cast_trans(prefix_symbol,prefix_expr_node->symbol,ir_bb);
        }
        M_TYPE* tmp_right_expr_type=Type_VEC_get_actual_base_type(right_expr_node->symbol->type_vec);
        if(tmp_right_expr_type->typ_category!=TP_BOOL)
        {
            right_symbol=Create_symbol_item(tmp_symbol_str_alloc(".reg."),NMSP_DEFAULT);
            right_symbol->count=HASH_CNT_IST;
            insert_symbol(ast_node->symbol_table,right_symbol);
            tmp_right_expr_type=build_base_type(TP_BOOL);
            VECinsert(right_symbol->type_vec,(void*)tmp_right_expr_type);
            type_cast_trans(right_symbol,right_expr_node->symbol,ir_bb);
        }
        IR_INS* logical_and_ins=add_new_ins(ir_bb);
        insert_ins_to_bb(logical_and_ins,ir_bb);
        GenINS(logical_and_ins,OP_AND,operator_node->symbol,prefix_expr_node->symbol,right_expr_node->symbol);
        prefix_expr_node=operator_node;
    }
    AST_BASE* res_node=AST_GET_CHILD(ast_node,AST_CHILD_NUM(ast_node)-2);
    type_cast_trans(ast_node->symbol,res_node->symbol,ir_bb);
    return true;
error:
    return false;
}
bool bit_inclusive_or_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=inclusive_or_expr)
        goto error;
    if(ast_node->symbol->const_expr)
        return true;
    bool prefix_const_expr=true;
    AST_BASE* prefix_expr_node=NULL;
    AST_BASE* right_expr_node=NULL;
    SYM_ITEM* prefix_symbol=NULL;
    SYM_ITEM* right_symbol=NULL;
    for(size_t i=1;i+1<AST_CHILD_NUM(ast_node);i+=2)
    {
        AST_BASE* operator_node=AST_GET_CHILD(ast_node,i);
        /*if the beginning sub expr are const expr, just use them directly*/    
        if(prefix_const_expr)
        {
            if(operator_node->symbol->const_expr)
            {
                prefix_expr_node=operator_node;
                continue;
            }
            else
                prefix_const_expr=false;
        }
        if(prefix_expr_node==NULL)
            prefix_expr_node=AST_GET_CHILD(ast_node,i-1);
        right_expr_node=AST_GET_CHILD(ast_node,i+1);
        if(!expr_trans_dispatch(right_expr_node,ir_bb))
            goto error;
        /*use the value directly and no need to cast*/
        prefix_symbol=prefix_expr_node->symbol;
        right_symbol=right_expr_node->symbol;
        IR_INS* bit_inclusive_or_ins=add_new_ins(ir_bb);
        insert_ins_to_bb(bit_inclusive_or_ins,ir_bb);
        GenINS(bit_inclusive_or_ins,OP_OR,operator_node->symbol,prefix_symbol,right_symbol);
        prefix_expr_node=operator_node;
    }
    AST_BASE* res_node=AST_GET_CHILD(ast_node,AST_CHILD_NUM(ast_node)-2);
    type_cast_trans(ast_node->symbol,res_node->symbol,ir_bb);
    return true;
error:
    return false;
}
bool bit_exclusive_or_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=exclusive_or_expr)
        goto error;
    if(ast_node->symbol->const_expr)
        return true;
    bool prefix_const_expr=true;
    AST_BASE* prefix_expr_node=NULL;
    AST_BASE* right_expr_node=NULL;
    SYM_ITEM* prefix_symbol=NULL;
    SYM_ITEM* right_symbol=NULL;
    for(size_t i=1;i+1<AST_CHILD_NUM(ast_node);i+=2)
    {
        AST_BASE* operator_node=AST_GET_CHILD(ast_node,i);
        /*if the beginning sub expr are const expr, just use them directly*/    
        if(prefix_const_expr)
        {
            if(operator_node->symbol->const_expr)
            {
                prefix_expr_node=operator_node;
                continue;
            }
            else
                prefix_const_expr=false;
        }
        if(prefix_expr_node==NULL)
            prefix_expr_node=AST_GET_CHILD(ast_node,i-1);
        right_expr_node=AST_GET_CHILD(ast_node,i+1);
        if(!expr_trans_dispatch(right_expr_node,ir_bb))
            goto error;
        prefix_symbol=prefix_expr_node->symbol;
        right_symbol=right_expr_node->symbol;
        IR_INS* bit_exclusive_or_ins=add_new_ins(ir_bb);
        insert_ins_to_bb(bit_exclusive_or_ins,ir_bb);
        GenINS(bit_exclusive_or_ins,OP_XOR,operator_node->symbol,prefix_symbol,right_symbol);
        prefix_expr_node=operator_node;
    }
    AST_BASE* res_node=AST_GET_CHILD(ast_node,AST_CHILD_NUM(ast_node)-2);
    type_cast_trans(ast_node->symbol,res_node->symbol,ir_bb);
    return true;
error:
    return false;
}
bool and_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=and_expression)
        goto error;
    if(ast_node->symbol->const_expr)
        return true;
    bool prefix_const_expr=true;
    AST_BASE* prefix_expr_node=NULL;
    AST_BASE* right_expr_node=NULL;
    SYM_ITEM* prefix_symbol=NULL;
    SYM_ITEM* right_symbol=NULL;
    for(size_t i=1;i+1<AST_CHILD_NUM(ast_node);i+=2)
    {
        AST_BASE* operator_node=AST_GET_CHILD(ast_node,i);
        /*if the beginning sub expr are const expr, just use them directly*/    
        if(prefix_const_expr)
        {
            if(operator_node->symbol->const_expr)
            {
                prefix_expr_node=operator_node;
                continue;
            }
            else
                prefix_const_expr=false;
        }
        if(prefix_expr_node==NULL)
            prefix_expr_node=AST_GET_CHILD(ast_node,i-1);
        right_expr_node=AST_GET_CHILD(ast_node,i+1);
        if(!expr_trans_dispatch(right_expr_node,ir_bb))
            goto error;
        prefix_symbol=prefix_expr_node->symbol;
        right_symbol=right_expr_node->symbol;
        M_TYPE* tmp_prefix_expr_type=Type_VEC_get_actual_base_type(prefix_expr_node->symbol->type_vec);
        IR_INS* and_ins=add_new_ins(ir_bb);
        insert_ins_to_bb(and_ins,ir_bb);
        GenINS(and_ins,OP_AND,operator_node->symbol,prefix_symbol,right_symbol);
        prefix_expr_node=operator_node;
    }
    AST_BASE* res_node=AST_GET_CHILD(ast_node,AST_CHILD_NUM(ast_node)-2);
    type_cast_trans(ast_node->symbol,res_node->symbol,ir_bb);
    return true;
error:
    return false;
}
bool equal_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=equality_expr)
        goto error;
    if(ast_node->symbol->const_expr)
        return true;
    bool prefix_const_expr=true;
    AST_BASE* prefix_expr_node=NULL;
    AST_BASE* right_expr_node=NULL;
    SYM_ITEM* prefix_symbol=NULL;
    SYM_ITEM* right_symbol=NULL;
    for(size_t i=1;i+1<AST_CHILD_NUM(ast_node);i+=2)
    {
        AST_BASE* operator_node=AST_GET_CHILD(ast_node,i);
        /*if the beginning sub expr are const expr, just use them directly*/    
        if(prefix_const_expr)
        {
            if(operator_node->symbol->const_expr)
            {
                prefix_expr_node=operator_node;
                continue;
            }
            else
                prefix_const_expr=false;
        }
        if(prefix_expr_node==NULL)
            prefix_expr_node=AST_GET_CHILD(ast_node,i-1);
        right_expr_node=AST_GET_CHILD(ast_node,i+1);
        if(!expr_trans_dispatch(right_expr_node,ir_bb))
            goto error;
        prefix_symbol=prefix_expr_node->symbol;
        right_symbol=right_expr_node->symbol;
        
        M_TYPE* prefix_type=Type_VEC_get_actual_base_type(prefix_symbol->type_vec);
        M_TYPE* right_type=Type_VEC_get_actual_base_type(right_symbol->type_vec);
        if(IS_INT_TYPE(right_type->typ_category)&&right_symbol->const_expr){
            long long int null_pointer_value=TP_INT_CAST_TYPE(right_type->typ_category,right_symbol->data_field);
            if(null_pointer_value==0)
                right_type->typ_category=TP_NULL_POINTER_CONSTANT;
        }
        if(prefix_type->typ_category==TP_ENUM)
            prefix_type=build_base_type(TP_SINT);
        if(right_type->typ_category==TP_ENUM)
            right_type=build_base_type(TP_SINT);
        if(IS_ARTH_TYPE(prefix_type->typ_category)&&IS_ARTH_TYPE(right_type->typ_category))
        {
            M_TYPE* cmp_type=usual_arth_conversion_trans(&prefix_symbol,&right_symbol,ir_bb);
            if(IS_INT_TYPE(cmp_type->typ_category))
            {
                IR_INS* equal_ins=add_new_ins(ir_bb);
                insert_ins_to_bb(equal_ins,ir_bb);
                GenINS(equal_ins,OP_ICMP,operator_node->symbol,prefix_symbol,right_symbol);
                equal_ins->other_attr=m_alloc(sizeof(CMP_COND_ATTR));
                if(operator_node->type==double_equal)
                {
                    ((CMP_COND_ATTR*)(equal_ins->other_attr))->icmp_cond=ICMP_EQ;
                }
                else if(operator_node->type==un_equal)
                {
                    ((CMP_COND_ATTR*)(equal_ins->other_attr))->icmp_cond=ICMP_NE;
                }
            }
            else if(IS_FLOAT_TYPE(cmp_type->typ_category))
            {
                /*the real number cannot compare directly,so if equal ,must be false,and unequal,must be true*/
                if(operator_node->type==double_equal)
                {
                    operator_node->symbol->data_field->databool=false;
                }
                else if(operator_node->type==un_equal)
                {
                    operator_node->symbol->data_field->databool=true;
                }
            }
#if _CPLX_SUPPORT==1
            else if(IS_COMPLEX_TYPE(cmp_type->typ_category))
            {
                /*
                    TODO:
                    only the real part and the complex part are all equal, can that be equal
                */
                ;
            }
#endif

        }
        else if(prefix_type->typ_category==TP_POINT&&(right_type->typ_category==TP_POINT||right_type->typ_category==TP_NULL_POINTER_CONSTANT))
        {
            IR_INS* equal_ins=add_new_ins(ir_bb);
            insert_ins_to_bb(equal_ins,ir_bb);
            GenINS(equal_ins,OP_ICMP,operator_node->symbol,prefix_symbol,right_symbol);
            equal_ins->other_attr=m_alloc(sizeof(CMP_COND_ATTR));
            if(operator_node->type==double_equal)
            {
                ((CMP_COND_ATTR*)(equal_ins->other_attr))->icmp_cond=ICMP_EQ;
            }
            else if(operator_node->type==un_equal)
            {
                ((CMP_COND_ATTR*)(equal_ins->other_attr))->icmp_cond=ICMP_NE;
            }
        }
        else if(right_type->typ_category==TP_POINT)
        {
            /*another one is a void pointer*/
            IR_INS* equal_ins=add_new_ins(ir_bb);
            insert_ins_to_bb(equal_ins,ir_bb);
            GenINS(equal_ins,OP_ICMP,operator_node->symbol,prefix_symbol,right_symbol);
            equal_ins->other_attr=m_alloc(sizeof(CMP_COND_ATTR));
            if(operator_node->type==double_equal)
            {
                ((CMP_COND_ATTR*)(equal_ins->other_attr))->icmp_cond=ICMP_EQ;
            }
            else if(operator_node->type==un_equal)
            {
                ((CMP_COND_ATTR*)(equal_ins->other_attr))->icmp_cond=ICMP_NE;
            }
        }
        prefix_expr_node=operator_node;
    }
    AST_BASE* res_node=AST_GET_CHILD(ast_node,AST_CHILD_NUM(ast_node)-2);
    type_cast_trans(ast_node->symbol,res_node->symbol,ir_bb);
    return true;
error:
    return false;
}
bool relation_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=relational_expr)
        goto error;
    if(ast_node->symbol->const_expr)
        return true;
    bool prefix_const_expr=true;
    AST_BASE* prefix_expr_node=NULL;
    AST_BASE* right_expr_node=NULL;
    SYM_ITEM* prefix_symbol=NULL;
    SYM_ITEM* right_symbol=NULL;
    for(size_t i=1;i+1<AST_CHILD_NUM(ast_node);i+=2)
    {
        AST_BASE* operator_node=AST_GET_CHILD(ast_node,i);
        /*if the beginning sub expr are const expr, just use them directly*/    
        if(prefix_const_expr)
        {
            if(operator_node->symbol->const_expr)
            {
                prefix_expr_node=operator_node;
                continue;
            }
            else
                prefix_const_expr=false;
        }
        if(prefix_expr_node==NULL)
            prefix_expr_node=AST_GET_CHILD(ast_node,i-1);
        right_expr_node=AST_GET_CHILD(ast_node,i+1);
        if(!expr_trans_dispatch(right_expr_node,ir_bb))
            goto error;
        prefix_symbol=prefix_expr_node->symbol;
        right_symbol=right_expr_node->symbol;
        
        M_TYPE* prefix_type=Type_VEC_get_actual_base_type(prefix_symbol->type_vec);
        M_TYPE* right_type=Type_VEC_get_actual_base_type(right_symbol->type_vec);
        if(IS_INT_TYPE(right_type->typ_category)&&right_symbol->const_expr){
            long long int null_pointer_value=TP_INT_CAST_TYPE(right_type->typ_category,right_symbol->data_field);
            if(null_pointer_value==0)
                right_type->typ_category=TP_NULL_POINTER_CONSTANT;
        }
        if(prefix_type->typ_category==TP_ENUM)
            prefix_type=build_base_type(TP_SINT);
        if(right_type->typ_category==TP_ENUM)
            right_type=build_base_type(TP_SINT);
        
        if(IS_REAL_TYPE(prefix_type->typ_category)&&IS_REAL_TYPE(right_type->typ_category))
        {
            M_TYPE* cmp_type=usual_arth_conversion_trans(&prefix_symbol,&right_symbol,ir_bb);
            IR_INS* equal_ins=add_new_ins(ir_bb);
            insert_ins_to_bb(equal_ins,ir_bb);
            equal_ins->other_attr=m_alloc(sizeof(CMP_COND_ATTR));
            if(IS_INT_TYPE(cmp_type->typ_category)&&(cmp_type->typ_category)%2==0)
            {
                GenINS(equal_ins,OP_ICMP,operator_node->symbol,prefix_symbol,right_symbol);
                if(operator_node->type==less_than)
                    ((CMP_COND_ATTR*)(equal_ins->other_attr))->icmp_cond=ICMP_SLT;
                else if(operator_node->type==greater_than)
                    ((CMP_COND_ATTR*)(equal_ins->other_attr))->icmp_cond=ICMP_SGT;
                else if(operator_node->type==less_equal)
                    ((CMP_COND_ATTR*)(equal_ins->other_attr))->icmp_cond=ICMP_SLE;
                else if(operator_node->type==greater_equal)
                    ((CMP_COND_ATTR*)(equal_ins->other_attr))->icmp_cond=ICMP_SGE;
            }
            else if(IS_INT_TYPE(cmp_type->typ_category)&&(cmp_type->typ_category)%2==1)
            {
                GenINS(equal_ins,OP_ICMP,operator_node->symbol,prefix_symbol,right_symbol);
                if(operator_node->type==less_than)
                    ((CMP_COND_ATTR*)(equal_ins->other_attr))->icmp_cond=ICMP_ULT;
                else if(operator_node->type==greater_than)
                    ((CMP_COND_ATTR*)(equal_ins->other_attr))->icmp_cond=ICMP_UGT;
                else if(operator_node->type==less_equal)
                    ((CMP_COND_ATTR*)(equal_ins->other_attr))->icmp_cond=ICMP_ULE;
                else if(operator_node->type==greater_equal)
                    ((CMP_COND_ATTR*)(equal_ins->other_attr))->icmp_cond=ICMP_UGE;
            }
            else if(IS_FLOAT_TYPE(cmp_type->typ_category))
            {
                GenINS(equal_ins,OP_FCMP,operator_node->symbol,prefix_symbol,right_symbol);
                if(operator_node->type==less_than)
                    ((CMP_COND_ATTR*)(equal_ins->other_attr))->fcmp_cond=FCMP_ULT;
                else if(operator_node->type==greater_than)
                    ((CMP_COND_ATTR*)(equal_ins->other_attr))->fcmp_cond=FCMP_UGT;
                else if(operator_node->type==less_equal)
                    ((CMP_COND_ATTR*)(equal_ins->other_attr))->fcmp_cond=FCMP_ULE;
                else if(operator_node->type==greater_equal)
                    ((CMP_COND_ATTR*)(equal_ins->other_attr))->fcmp_cond=FCMP_UGE;
            }
        }
        else if(prefix_type->typ_category==TP_POINT&&(right_type->typ_category==TP_POINT||right_type->typ_category==TP_NULL_POINTER_CONSTANT))
        {
            IR_INS* equal_ins=add_new_ins(ir_bb);
            insert_ins_to_bb(equal_ins,ir_bb);
            GenINS(equal_ins,OP_ICMP,operator_node->symbol,prefix_symbol,right_symbol);
            equal_ins->other_attr=m_alloc(sizeof(CMP_COND_ATTR));
            if(operator_node->type==less_than)
                ((CMP_COND_ATTR*)(equal_ins->other_attr))->icmp_cond=ICMP_ULT;
            else if(operator_node->type==greater_than)
                ((CMP_COND_ATTR*)(equal_ins->other_attr))->icmp_cond=ICMP_UGT;
            else if(operator_node->type==less_equal)
                ((CMP_COND_ATTR*)(equal_ins->other_attr))->icmp_cond=ICMP_ULE;
            else if(operator_node->type==greater_equal)
                ((CMP_COND_ATTR*)(equal_ins->other_attr))->icmp_cond=ICMP_UGE;
        }
        else
            goto error;
        prefix_expr_node=operator_node;
    }
    AST_BASE* res_node=AST_GET_CHILD(ast_node,AST_CHILD_NUM(ast_node)-2);
    type_cast_trans(ast_node->symbol,res_node->symbol,ir_bb);
    return true;
error:
    return false;
}
bool shift_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=shift_expr)
        goto error;
    if(ast_node->symbol->const_expr)
        return true;
    bool prefix_const_expr=true;
    AST_BASE* prefix_expr_node=NULL;
    AST_BASE* right_expr_node=NULL;
    SYM_ITEM* prefix_symbol=NULL;
    SYM_ITEM* right_symbol=NULL;
    for(size_t i=1;i+1<AST_CHILD_NUM(ast_node);i+=2)
    {
        AST_BASE* operator_node=AST_GET_CHILD(ast_node,i);
        /*if the beginning sub expr are const expr, just use them directly*/    
        if(prefix_const_expr)
        {
            if(operator_node->symbol->const_expr)
            {
                prefix_expr_node=operator_node;
                continue;
            }
            else
                prefix_const_expr=false;
        }
        if(prefix_expr_node==NULL)
            prefix_expr_node=AST_GET_CHILD(ast_node,i-1);
        right_expr_node=AST_GET_CHILD(ast_node,i+1);
        if(!expr_trans_dispatch(right_expr_node,ir_bb))
            goto error;
        /*use the value directly and no need to cast*/
        prefix_symbol=prefix_expr_node->symbol;
        right_symbol=right_expr_node->symbol;
        IR_INS* shift_op_ins=add_new_ins(ir_bb);
        insert_ins_to_bb(shift_op_ins,ir_bb);
        M_TYPE* base_int_type=Type_VEC_get_actual_base_type(operator_node->symbol->type_vec);
        /*
            Hint: logical shift and mathmatical shift
            for C stardard, it haven't say any words about this
            but most compliers have the same implement
        */
        if(base_int_type->typ_category%2==1)
        {   /*unsigned*/
            if(operator_node->type==left_shift)
                GenINS(shift_op_ins,OP_SHL,operator_node->symbol,prefix_symbol,right_symbol);
            else if(operator_node->type==right_shift)
                GenINS(shift_op_ins,OP_SHR,operator_node->symbol,prefix_symbol,right_symbol);
        }
        else if(base_int_type->typ_category%2==0)
        {   /*signed*/
            if(operator_node->type==left_shift)
                GenINS(shift_op_ins,OP_SAL,operator_node->symbol,prefix_symbol,right_symbol);
            else if(operator_node->type==right_shift)
                GenINS(shift_op_ins,OP_SAR,operator_node->symbol,prefix_symbol,right_symbol);
        }
        else
            goto error;
        prefix_expr_node=operator_node;
    }
    AST_BASE* res_node=AST_GET_CHILD(ast_node,AST_CHILD_NUM(ast_node)-2);
    type_cast_trans(ast_node->symbol,res_node->symbol,ir_bb);
    return true;
error:
    return false;
}
bool add_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=additive_expr)
        goto error;
    if(ast_node->symbol->const_expr)
        return true;
    bool prefix_const_expr=true;
    AST_BASE* prefix_expr_node=NULL;
    AST_BASE* right_expr_node=NULL;
    SYM_ITEM* prefix_symbol=NULL;
    SYM_ITEM* right_symbol=NULL;
    for(size_t i=1;i+1<AST_CHILD_NUM(ast_node);i+=2)
    {
        AST_BASE* operator_node=AST_GET_CHILD(ast_node,i);
        /*if the beginning sub expr are const expr, just use them directly*/    
        if(prefix_const_expr)
        {
            if(operator_node->symbol->const_expr)
            {
                prefix_expr_node=operator_node;
                continue;
            }
            else
                prefix_const_expr=false;
        }
        if(prefix_expr_node==NULL)
            prefix_expr_node=AST_GET_CHILD(ast_node,i-1);
        right_expr_node=AST_GET_CHILD(ast_node,i+1);
        if(!expr_trans_dispatch(right_expr_node,ir_bb))
            goto error;
        prefix_symbol=prefix_expr_node->symbol;
        right_symbol=right_expr_node->symbol;
        
        IR_INS* add_ins=add_new_ins(ir_bb);
        insert_ins_to_bb(add_ins,ir_bb);

        M_TYPE* prefix_type=Type_VEC_get_actual_base_type(prefix_symbol->type_vec);
        M_TYPE* right_type=Type_VEC_get_actual_base_type(right_symbol->type_vec);
        if(IS_INT_TYPE(prefix_type->typ_category)&&IS_INT_TYPE(right_type->typ_category))
        {
            GenINS(add_ins,OP_ADD,operator_node->symbol,prefix_symbol,right_symbol);
        }
        else if(IS_INT_TYPE(prefix_type->typ_category)&&IS_FLOAT_TYPE(right_type->typ_category))
        {
            M_TYPE* tmp_type=usual_arth_conversion_trans(&prefix_symbol,&right_symbol,ir_bb);
            GenINS(add_ins,OP_FADD,operator_node->symbol,prefix_symbol,right_symbol);
        }
        else if(IS_FLOAT_TYPE(prefix_type->typ_category)&&IS_INT_TYPE(right_type->typ_category))
        {
            M_TYPE* tmp_type=usual_arth_conversion_trans(&prefix_symbol,&right_symbol,ir_bb);
            GenINS(add_ins,OP_FADD,operator_node->symbol,prefix_symbol,right_symbol);
        }
        else if(IS_FLOAT_TYPE(prefix_type->typ_category)&&IS_FLOAT_TYPE(right_type->typ_category))
        {
            GenINS(add_ins,OP_FADD,operator_node->symbol,prefix_symbol,right_symbol);
        }



        prefix_expr_node=operator_node;
    }
    AST_BASE* res_node=AST_GET_CHILD(ast_node,AST_CHILD_NUM(ast_node)-2);
    type_cast_trans(ast_node->symbol,res_node->symbol,ir_bb);
    return true;
error:
    return false;
}
bool mul_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=multi_expr)
        goto error;
    if(ast_node->symbol->const_expr)
        return true;
    return true;
error:
    return false;
}
bool cast_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=cast_expr)
        goto error;
    if(ast_node->symbol->const_expr)
        return true;
    return true;
error:
    return false;
}
bool unary_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=unary_expr)
        goto error;
    if(ast_node->symbol->const_expr)
        return true;
    return true;
error:
    return false;
}
bool postfix_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=postfix_expr)
        goto error;
    /*
        if this postfix expr is a const value,do nothing
    */
    if(ast_node->symbol->const_expr)
        return true;
    /*not const expr*/
    AST_BASE* first_node=AST_GET_CHILD(ast_node,0);
    int suffix_start_index=0;
    SYM_ITEM* curr_symbol=NULL;
    if(first_node&&first_node->type==primary_expression){
        suffix_start_index=1;
        curr_symbol=first_node->symbol;
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
        /*alloc a symbol on stack*/
        curr_symbol=alloca_on_stack_value(ir_bb,type_name_node->symbol);
    }
    else
        goto error;
    while(suffix_start_index<AST_CHILD_NUM(ast_node))
    {
        AST_BASE* tmp_ast=AST_GET_CHILD(ast_node,suffix_start_index);
        if(tmp_ast->type==left_bracket){    /*Array case*/
            AST_BASE* arr_ast=AST_GET_CHILD(ast_node,suffix_start_index+1);
            if(!expr_trans_dispatch(arr_ast,ir_bb))
                goto error;
            IR_INS* pointer_ins=add_new_ins(ir_bb);
            insert_ins_to_bb(pointer_ins,ir_bb);
            GenINS(pointer_ins,OP_ADD,tmp_ast->symbol,curr_symbol,arr_ast->symbol);

            curr_symbol=tmp_ast->symbol;
            suffix_start_index+=3;
        }
        else if(tmp_ast->type==left_parenthesis){   /*Function case:*/
            /*TODO:calculate the parameters and gen ins*/
            

            /*generate the call ins*/
            IR_INS* call_ins=add_new_ins(ir_bb);
            insert_ins_to_bb(call_ins,ir_bb);
            GenINS(call_ins,OP_CALL,tmp_ast->symbol,curr_symbol,NULL);

            curr_symbol=tmp_ast->symbol;
            for(size_t i=suffix_start_index;i<AST_CHILD_NUM(ast_node);++i){
                tmp_ast=AST_GET_CHILD(ast_node,i);
                if(tmp_ast->type==right_parenthesis)
                {
                    suffix_start_index=i+1;
                    break;
                }
            }
        }
        else if(tmp_ast->type==dot){    /*struct*/
            
            curr_symbol=tmp_ast->symbol;
            suffix_start_index+=2;
        }
        else if(tmp_ast->type==point){  /*point to struct*/
            curr_symbol=tmp_ast->symbol;
            suffix_start_index+=2;
        }
        else if(tmp_ast->type==double_plus){    /* ++ */
            /*remember,if the operand is a pointer,must add a size of that operand point to*/
            M_TYPE* tmp_base=Type_VEC_get_actual_base_type(curr_symbol->type_vec);
            
            /*gen a imm symbol with value 1*/
            SYM_ITEM* tmp_symbol=Create_symbol_item(tmp_symbol_str_alloc(".reg."),NMSP_DEFAULT);
            tmp_symbol->count=HASH_CNT_IST;
            insert_symbol(tmp_ast->symbol_table,tmp_symbol);

            IR_INS* add_ins=add_new_ins(ir_bb);
            insert_ins_to_bb(add_ins,ir_bb);
            if(tmp_base->typ_category==TP_POINT)
            {
                VEC* pointer_type_vec=Type_VEC_get_Pointer_TO(curr_symbol->type_vec,true);
                tmp_symbol->data_field->sint=Type_size(pointer_type_vec);
                GenINS(add_ins,OP_ADD,tmp_ast->symbol,curr_symbol,tmp_symbol);
            }
            else if(IS_REAL_TYPE(tmp_base->typ_category))
            {
                if(IS_INT_TYPE(tmp_base->typ_category))
                {
                    M_TYPE* base_type=build_base_type(TP_SINT);
                    VECinsert(tmp_symbol->type_vec,(void*)base_type);
                    tmp_symbol->data_field->sint=1;
                }
                else if(IS_FLOAT_TYPE(tmp_base->typ_category))
                {
                    M_TYPE* base_type=build_base_type(TP_DOUBLE);
                    VECinsert(tmp_symbol->type_vec,(void*)base_type);
                    tmp_symbol->data_field->datadouble=1.0;
                }
                else
                    goto error; /*impossible*/
                GenINS(add_ins,OP_ADD,tmp_ast->symbol,curr_symbol,tmp_symbol);
            }
            else    /*impossible error*/
            {
                goto error;
            }
            curr_symbol=tmp_ast->symbol;
            suffix_start_index+=1;
        }
        else if(tmp_ast->type==double_minus){   /* -- */
            M_TYPE* tmp_base=Type_VEC_get_actual_base_type(curr_symbol->type_vec);
            
            /*gen a imm symbol with value 1*/
            SYM_ITEM* tmp_symbol=Create_symbol_item(tmp_symbol_str_alloc(".reg."),NMSP_DEFAULT);
            tmp_symbol->count=HASH_CNT_IST;
            insert_symbol(tmp_ast->symbol_table,tmp_symbol);

            IR_INS* add_ins=add_new_ins(ir_bb);
            insert_ins_to_bb(add_ins,ir_bb);
            if(tmp_base->typ_category==TP_POINT)
            {
                VEC* pointer_type_vec=Type_VEC_get_Pointer_TO(curr_symbol->type_vec,true);
                tmp_symbol->data_field->sint=-((signed int)Type_size(pointer_type_vec));
                GenINS(add_ins,OP_ADD,tmp_ast->symbol,curr_symbol,tmp_symbol);
            }
            else if(IS_REAL_TYPE(tmp_base->typ_category))
            {
                if(IS_INT_TYPE(tmp_base->typ_category))
                {
                    M_TYPE* base_type=build_base_type(TP_SINT);
                    VECinsert(tmp_symbol->type_vec,(void*)base_type);
                    tmp_symbol->data_field->sint=-1;
                }
                else if(IS_FLOAT_TYPE(tmp_base->typ_category))
                {
                    M_TYPE* base_type=build_base_type(TP_DOUBLE);
                    VECinsert(tmp_symbol->type_vec,(void*)base_type);
                    tmp_symbol->data_field->datadouble=-1.0;
                }
                else
                    goto error; /*impossible*/
                GenINS(add_ins,OP_ADD,tmp_ast->symbol,curr_symbol,tmp_symbol);
            }
            else    /*impossible error*/
            {
                goto error;
            }
            curr_symbol=tmp_ast->symbol;
            suffix_start_index+=1;
        }
    }
    return true;
error:
    return false;
}
bool pri_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||(ast_node->type)!=primary_expression)
        goto error;
    if(ast_node->symbol->const_expr)
        return true;
    return true;
error:
    return false;
}