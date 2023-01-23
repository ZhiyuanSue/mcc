#include "transdecl.h"
static size_t global_off=0;
static size_t thread_off=0;
static size_t stack_off=0;
bool declaration_trans(AST_BASE* ast_node,IR_MODULE* irm,IR_FUNC* ir_func,IR_BB* ir_bb)
{
    ERROR_ITEM* tei=(ERROR_ITEM*)m_alloc(sizeof(ERROR_ITEM));
    if(!ast_node||!irm||ast_node->type!=declaration)
        goto error;
    AST_BASE* type_def_kw=ast_node;
    while(AST_CHILD_NUM(type_def_kw)>0){
        type_def_kw=AST_GET_CHILD(type_def_kw,0);
    }
    if(type_def_kw->type==KW_type_def)
        return true;    /*do nothing to typedef declarator*/
    AST_BASE* init_decl_list_node=AST_GET_CHILD(ast_node,1);
    for(size_t i=0;i<AST_CHILD_NUM(init_decl_list_node);i+=2){
        AST_BASE* init_decl_node=AST_GET_CHILD(init_decl_list_node,i);
        AST_BASE* decl_node=AST_GET_CHILD(init_decl_node,0);
        AST_BASE* identifier_node=declarator_node(decl_node);
        SYM_ITEM* tmpsi=find_symbol_curr_table(ast_node->symbol_table,
                identifier_node->token->value,
                decl_node->name_space);
        if(!tmpsi)
            goto error;/*actually impossible*/
        /*judge whether the symbol size is 0*/
        M_TYPE* tmp_m_type=Type_VEC_get_spec_other(tmpsi->type_vec);
        if(tmp_m_type&&(tmp_m_type->typ_stor!=TP_EXTERN||tmp_m_type->typ_stor!=TP_EXTERN_THREAD_LOCAL))
            return true;
        M_TYPE* tmp_type=Type_VEC_get_actual_base_type(tmpsi->type_vec);
        if(tmp_type->typ_category!=TP_FUNCTION&&Type_size(tmpsi->type_vec)==0)
        {
            C_ERROR(C0096_ERR_STOR_SIZE_ISNT_KNOWN,identifier_node);
            goto error;
        }
        /*judge which place to put this symble*/
    }
    m_free(tei);
    return true;
error:
    return false;
}