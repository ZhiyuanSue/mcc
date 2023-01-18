#include "stat_semantics.h"
#ifdef _TEST_SEMANTICS_
extern int semantics_level;
#endif
extern VEC* c_error;
extern char* filename;
bool statement_dispatch(AST_BASE* ast_node)
{
    if(!ast_node)
        return false;
    bool (*statement_dis[STMT_NUM])(AST_BASE*)={
        labeled_statement,compound_statement,expr_statement,
        if_statement,switch_statement,while_statement,
        do_statement,for_statement,goto_statement,
        continue_statement,break_statement,return_statement,
        asm_statement
    };
    if(IS_STMT_NODE(ast_node->type))
        return statement_dis[ast_node->type-labeled_stmt](ast_node);
    if(ast_node->type==statement)
    {
        return statement_dispatch(AST_GET_CHILD(ast_node,0));
    }
    return false;
}
bool labeled_statement(AST_BASE* ast_node)
{
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start label expr\n");
#endif
    if(!ast_node||ast_node->type!=labeled_stmt)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    AST_BASE* label_node=AST_GET_CHILD(ast_node,0);
    AST_BASE* statement_node=(AST_BASE*)VEC_BACK(ast_node->child);
    /*insert and check label*/
    if(label_node->type==KW_case){
        AST_BASE* const_expr_node=AST_GET_CHILD(ast_node,1);
        if(!const_value(const_expr_node))
            goto error;
        SYM* target_symbol_table=ast_node->symbol_table;
        while(1)
        {
            if(target_symbol_table->father==NULL||((target_symbol_table->father->st_attr_type)&SA_SWITCH)==0)
                break;
            else
                target_symbol_table=target_symbol_table->father;
        }
        if(((target_symbol_table->st_attr_type)&SA_SWITCH)==0)
        {
            C_ERROR(C0076_ERR_IN_SWITCH,label_node);
            goto error;
        }
        SYMBOL_TABLE_SWITCH_ATTR* tmp_switch_attr=(SYMBOL_TABLE_SWITCH_ATTR*)target_symbol_table->st_attr;
        unsigned long long int* tmp_value;
        M_TYPE* const_node_type=Type_VEC_get_actual_base_type(const_expr_node->expr_attribute->type_vec);
        if(!IS_INT_TYPE(const_node_type->typ_category))
        {
            C_ERROR(C0060_ERR_OPERAND_INTEGER_TYPE,const_expr_node);
            goto error;
        }
        cast_const(TP_USLONGLONG,const_expr_node->expr_attribute->data_field,const_node_type->typ_category,const_expr_node->expr_attribute->data_field);
        unsigned long long int const_value=const_expr_node->expr_attribute->data_field->usllong;
        for(size_t i=0;i<VECLEN(tmp_switch_attr->switch_value);++i)
        {
            tmp_value=(unsigned long long int*)VEC_GET_ITEM(tmp_switch_attr->switch_value,i);
            if(const_value==*tmp_value)
            {
                C_ERROR(C0077_ERR_SWITCH_SAME_CASE,const_expr_node);
                goto error;
            }
        }
        tmp_value=m_alloc(sizeof(unsigned long long int));
        *tmp_value=const_value;
        VECinsert(tmp_switch_attr->switch_value,(void*)tmp_value);
    }
    else if(label_node->type==KW_default){
        SYM* target_symbol_table=ast_node->symbol_table;
        while(1)
        {
            if(target_symbol_table->father==NULL||((target_symbol_table->father->st_attr_type)&SA_SWITCH)==0)
                break;
            else
                target_symbol_table=target_symbol_table->father;
        }
        if(((target_symbol_table->st_attr_type)&SA_SWITCH)==0)
        {
            C_ERROR(C0076_ERR_IN_SWITCH,label_node);
            goto error;
        }
        SYMBOL_TABLE_SWITCH_ATTR* tmp_switch_attr=(SYMBOL_TABLE_SWITCH_ATTR*)target_symbol_table->st_attr;
        if(!tmp_switch_attr->have_default)
            tmp_switch_attr->have_default=true;
        else
        {
            C_ERROR(C0005_ERR_REDEFINE,label_node);
            goto error;
        }
    }
    else if(label_node->type==identifier){
        ;
    }
    else
        goto error;
    /*deal with the statement*/
    if(!statement_dispatch(statement_node))
        goto error;
    m_free(tei);
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end label stmt\n");
    semantics_level--;
#endif
    return true;
error:
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end label stmt fail\n");
    semantics_level--;
#endif
    return false;
}
bool compound_statement(AST_BASE* ast_node)
{
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start compound expr\n");
#endif
    if(!ast_node||ast_node->type!=compound_stmt)
        goto error;
    for(size_t i=1;i<AST_CHILD_NUM(ast_node)-1;++i){
        AST_BASE* sub_ast=AST_GET_CHILD(ast_node,i);
        VEC* tmp_sym_item=InitVEC(DEFAULT_CAPICITY);
        if(sub_ast){
            if(sub_ast->type==declaration&&!declaration_type(sub_ast,tmp_sym_item))
                goto error;
            else if(sub_ast->type==statement&&!statement_dispatch(sub_ast))
                goto error;
        }
        DelVEC(tmp_sym_item);
    }
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end comopound stmt\n");
    semantics_level--;
#endif
    return true;
error:
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end comopound stmt fail\n");
    semantics_level--;
#endif
    return false;
}
bool expr_statement(AST_BASE* ast_node)
{
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start expr expr\n");
#endif
    if(!ast_node||ast_node->type!=expr_stmt)
        goto error;
    if(AST_CHILD_NUM(ast_node)==1)
        return true;
    else if(AST_CHILD_NUM(ast_node)==2){
        AST_BASE* expr_node=AST_GET_CHILD(ast_node,0);
        return expr_dispatch(expr_node);
    }
    else
        goto error;
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end expr stmt\n");
    semantics_level--;
#endif
    return true;
error:
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end expr stmt fail\n");
    semantics_level--;
#endif
    return false;
}
bool if_statement(AST_BASE* ast_node)
{
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start if expr\n");
#endif
    if(!ast_node||ast_node->type!=if_stmt)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    AST_BASE* expr_node=AST_GET_CHILD(ast_node,2);
    AST_BASE* stmt_node=AST_GET_CHILD(ast_node,4);
    if(!expr_dispatch(expr_node))
        goto error;
    M_TYPE* tmpt=Type_VEC_get_actual_base_type(expr_node->expr_attribute->type_vec);
    if(!IS_SCALAR_TYPE(tmpt->typ_category))
    {
        C_ERROR(C0062_ERR_OPERAND_SCALAR_TYPE,expr_node);
        goto error;
    }
    if(!statement_dispatch(stmt_node))
        goto error;
    if(AST_CHILD_NUM(ast_node)==7)
    {
        stmt_node=AST_GET_CHILD(ast_node,6);
        if(!statement_dispatch(stmt_node))
            goto error;
    }
    m_free(tei);
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end if stmt\n");
    semantics_level--;
#endif
    return true;
error:
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end if stmt fail\n");
    semantics_level--;
#endif
    return false;
}
bool switch_statement(AST_BASE* ast_node)
{
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start switch expr\n");
#endif
    if(!ast_node||ast_node->type!=switch_stmt)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    AST_BASE* expr_node=AST_GET_CHILD(ast_node,2);
    if(!expr_dispatch(expr_node))
        goto error;
    M_TYPE* tmpt=Type_VEC_get_actual_base_type(expr_node->expr_attribute->type_vec);
    if(!IS_INT_TYPE(tmpt->typ_category))
    {
        C_ERROR(C0060_ERR_OPERAND_INTEGER_TYPE,expr_node);
        goto error;
    }
    m_free(tei);
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end switch stmt\n");
    semantics_level--;
#endif
    return true;
error:
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end switch stmt fail\n");
    semantics_level--;
#endif
    return false;
}
bool while_statement(AST_BASE* ast_node)
{
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start while expr\n");
#endif
    if(!ast_node||ast_node->type!=while_stmt)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    AST_BASE* expr_node=AST_GET_CHILD(ast_node,2);
    AST_BASE* stmt_node=AST_GET_CHILD(ast_node,4);
    if(!expr_dispatch(expr_node))
        goto error;
    if(!statement_dispatch(stmt_node))
        goto error;
    M_TYPE* tmpt=Type_VEC_get_actual_base_type(expr_node->expr_attribute->type_vec);
    if(!IS_SCALAR_TYPE(tmpt->typ_category))
    {
        C_ERROR(C0062_ERR_OPERAND_SCALAR_TYPE,ast_node);
        goto error;
    }
    m_free(tei);
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end while stmt\n");
    semantics_level--;
#endif
    return true;
error:
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end while stmt fail\n");
    semantics_level--;
#endif
    return false;
}
bool do_statement(AST_BASE* ast_node)
{
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start do expr\n");
#endif
    if(!ast_node||ast_node->type!=do_stmt)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    AST_BASE* expr_node=AST_GET_CHILD(ast_node,4);
    AST_BASE* stmt_node=AST_GET_CHILD(ast_node,1);
    if(!expr_dispatch(expr_node))
        goto error;
    if(!statement_dispatch(stmt_node))
        goto error;
    M_TYPE* tmpt=Type_VEC_get_actual_base_type(expr_node->expr_attribute->type_vec);
    if(!IS_SCALAR_TYPE(tmpt->typ_category))
    {
        C_ERROR(C0062_ERR_OPERAND_SCALAR_TYPE,ast_node);
        goto error;
    }
    m_free(tei);
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end do stmt\n");
    semantics_level--;
#endif
    return true;
error:
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end do stmt fail\n");
    semantics_level--;
#endif
    return false;
}
bool for_statement(AST_BASE* ast_node)
{
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start for expr\n");
#endif
    if(!ast_node||ast_node->type!=for_stmt)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    for(size_t i=2;i<AST_CHILD_NUM(ast_node)-2;i++)
    {
        AST_BASE* tmp_ast=AST_GET_CHILD(ast_node,i);
        if(tmp_ast->type==declaration)
        {
            VEC* tmp_sym_item=InitVEC(DEFAULT_CAPICITY);
            if(!declaration_type(tmp_ast,tmp_sym_item))
                goto error;
            for(size_t i=0;i<VECLEN(tmp_sym_item);++i)
            {
                SYM_ITEM* tmpsi=VEC_GET_ITEM(tmp_sym_item,i);
                M_TYPE* tmpt=Type_VEC_get_spec_other(tmpsi->type_vec);
                if(tmpt)    /*it is very possibly be a NULL*/
                {
                    if(tmpt->typ_stor!=TP_AUTO&&tmpt->typ_stor!=TP_REGISTER)
                    {
                        C_ERROR(C0075_ERR_FOR_DECLARATION_STOR,tmp_ast);
                        goto error;
                    }
                }
            }
        }
        else if(IS_EXPR_NODE(tmp_ast->type))
        {
            if(!expr_dispatch(tmp_ast))
                goto error;
        }
    }
    AST_BASE* stmt_node=VEC_BACK(ast_node->child);
    if(!statement_dispatch(stmt_node))
        goto error;
    m_free(tei);
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end for stmt\n");
    semantics_level--;
#endif
    return true;
error:
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end for stmt fail\n");
    semantics_level--;
#endif
    return false;
}
bool goto_statement(AST_BASE* ast_node)
{
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start goto expr\n");
#endif
    if(!ast_node||ast_node->type!=goto_stmt)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    AST_BASE* label_node=AST_GET_CHILD(ast_node,1);
    SYM* target_symbol_table=ast_node->symbol_table;
    while(target_symbol_table->father&&target_symbol_table->sp_type!=SPT_FUN)
        target_symbol_table=target_symbol_table->father;
    SYM_ITEM* find_tmpsi=Create_symbol_item(label_node->token->value,NMSP_UNKNOWN);
    HASH_ITEM* tmphi=HASHFind(target_symbol_table->sym_hash_table,
        find_tmpsi,
        symbol_item_cmp,
        false,
        false
    );
    if(!HASH_ITEM_EXIST(tmphi))
    {
        C_ERROR(C0016_ERR_UNDECLARATION,label_node);
        goto error;
    }
    m_free(tei);
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end goto stmt\n");
    semantics_level--;
#endif
    return true;
error:
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end goto stmt fail\n");
    semantics_level--;
#endif
    return false;
}
bool continue_statement(AST_BASE* ast_node)
{
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start continue expr\n");
#endif
    if(!ast_node||ast_node->type!=continue_stmt)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    SYM* target_symbol_table=ast_node->symbol_table;
    if(((target_symbol_table->st_attr_type)&SA_LOOP)==0)
    {
        C_ERROR(C0078_ERR_IN_LOOP,ast_node);
        goto error;
    }
    m_free(tei);
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end continue stmt\n");
    semantics_level--;
#endif
    return true;
error:
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end continue stmt fail\n");
    semantics_level--;
#endif
    return false;
}
bool break_statement(AST_BASE* ast_node)
{
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start break expr\n");
#endif
    if(!ast_node||ast_node->type!=break_stmt)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    SYM* target_symbol_table=ast_node->symbol_table;
    if(((target_symbol_table->father->st_attr_type)&SA_LOOP)==0&&((target_symbol_table->father->st_attr_type)&SA_SWITCH)==0)
    {
        C_ERROR(C0079_ERR_IN_SWITCH_LOOP,ast_node);
        goto error;
    }
    m_free(tei);
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end break stmt\n");
    semantics_level--;
#endif
    return true;
error:
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end break stmt fail\n");
    semantics_level--;
#endif
    return false;
}
bool return_statement(AST_BASE* ast_node)
{
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start return expr\n");
#endif
    if(!ast_node||ast_node->type!=return_stmt)
        goto error;
    bool have_expr=false;
    AST_BASE* expr_node=AST_GET_CHILD(ast_node,1);
    if(AST_CHILD_NUM(ast_node)==2)
        have_expr=false;
    else if(AST_CHILD_NUM(ast_node)==3)
    {
        have_expr=true;
        if(!expr_dispatch(expr_node))
            goto error;
    }
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    SYM* target_symbol_table=ast_node->symbol_table;
    while(1)
    {
        if(target_symbol_table->father==NULL||((target_symbol_table->father->st_attr_type)&SA_FUNC)==0)
            break;
        else
            target_symbol_table=target_symbol_table->father;
    }
    if(((target_symbol_table->st_attr_type)&SA_FUNC)==0)
    {
        goto error; /*just for test,impossible*/
    }
    SYM_ITEM* tmpsi=((SYMBOL_TABLE_FUNC_ATTR*)(target_symbol_table->st_attr))->function_symbol;
    M_TYPE* func_spec=Type_VEC_get_spec_other(tmpsi->type_vec);
    if(func_spec&&func_spec->func_spec==1)
    {
        /*a _Noreturn function have a return*/
        ERROR_ITEM* twi=m_alloc(sizeof(ERROR_ITEM));
        C_WARN(W0006_WARN_NO_RETURN,ast_node);
    }
    ((SYMBOL_TABLE_FUNC_ATTR*)(target_symbol_table->st_attr))->have_ret=true;
    VEC* func_ret_type_vec=Type_VEC_get_func_return_type(tmpsi->type_vec,true);
    M_TYPE* ret_type=Type_VEC_get_actual_base_type(func_ret_type_vec);
    if(ret_type->typ_category==TP_VOID&&have_expr==true)
    {
        C_ERROR(C0080_ERR_FUNC_RET_VOID,expr_node);
        goto error;
    }
    else if(ret_type->typ_category!=TP_VOID&&have_expr==false)
    {
        C_ERROR(C0081_ERR_FUNC_RET_NONE_VOID,ast_node);
        goto error;
    }
    m_free(tei);
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end return stmt\n");
    semantics_level--;
#endif
    return true;
error:
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end return stmt fail\n");
    semantics_level--;
#endif
    return false;
}
bool asm_statement(AST_BASE* ast_node)
{
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start asm expr\n");
#endif
    if(!ast_node||ast_node->type!=asm_stmt)
        goto error;
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end asm stmt\n");
    semantics_level--;
#endif
    return true;
error:
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("end asm stmt fail\n");
    semantics_level--;
#endif
    return false;
}