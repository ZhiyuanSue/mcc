#include "trans.h"

IR_MODULE* trans_to_IR(AST_BASE* ast_node)
{   /*do some init work*/
    if(!ast_node||ast_node->type!=translation_unit)
        goto error;
    IR_MODULE* res=m_alloc(sizeof(IR_MODULE));
    res->global_and_external_symbols=InitVEC(DEFAULT_CAPICITY);
    res->func_list=InitVEC(DEFAULT_CAPICITY);
    for(size_t i=0;i<AST_CHILD_NUM(ast_node);++i)
    {
        AST_BASE* external_decl_node=AST_GET_CHILD(ast_node,i);
        AST_BASE* ast_child=AST_GET_CHILD(external_decl_node,0);
        if(!ast_child)
            goto error;
        switch (ast_child->type)
        {
        case declaration:
        {
            if(ast_child->decl_attribute)
            {
                VEC* tmpsi_vec=ast_child->decl_attribute->decl_symbol_item_list;
                for(size_t i=0;i<VECLEN(tmpsi_vec);++i)
                {
                    VECinsert(res->global_and_external_symbols,VEC_GET_ITEM(tmpsi_vec,i));
                }
            }
            break;
        }
        case function_definition:
        {
            IR_FUNC* ir_new_func=add_new_func(res);
            if(!trans_func(ast_child,ir_new_func))
                goto error;
            break;
        }
        default:
            goto error;
        }
    }
    return res;
error:
    return NULL;
}
bool trans_func(AST_BASE* ast_node,IR_FUNC* ir_func)
{
    if(!ast_node||!ir_func||ast_node->type!=function_definition)
        goto error;
    
    return true;
error:
    return false;
}