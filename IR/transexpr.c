#include "transexpr.h"
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
    return res;
error:
    return false;
}
bool cond_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    return true;
error:
    return false;
}
bool expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    return true;
error:
    return false;
}
bool assign_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    return true;
error:
    return false;
}
bool logical_or_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    return true;
error:
    return false;
}
bool logical_and_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    return true;
error:
    return false;
}
bool bit_inclusive_or_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    return true;
error:
    return false;
}
bool bit_exclusive_or_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    return true;
error:
    return false;
}
bool and_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    return true;
error:
    return false;
}
bool equal_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    return true;
error:
    return false;
}
bool relation_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    return true;
error:
    return false;
}
bool shift_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    return true;
error:
    return false;
}
bool add_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    return true;
error:
    return false;
}
bool mul_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    return true;
error:
    return false;
}
bool cast_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    return true;
error:
    return false;
}
bool unary_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    return true;
error:
    return false;
}
bool postfix_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    
    return true;
error:
    return false;
}
bool pri_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    AST_BASE* child_node=NULL;
    if(AST_CHILD_NUM(ast_node)==1)
    {
        child_node=AST_GET_CHILD(ast_node,0);
        switch(child_node->type){
            case identifier:
            {
                SYM_ITEM* find_tmpsi=find_symbol(ast_node->symbol_table,child_node->token->value,NMSP_DEFAULT);
                VEC* tmpsi_type_vec=find_tmpsi->type_vec;
                M_TYPE* tmp_type=Type_VEC_get_actual_base_type(tmpsi_type_vec);
                if(IS_BASE_TYPE(tmp_type->typ_category))
                {

                }
                else if(tmp_type->typ_category==TP_FUNCTION)
                {
                    
                }
            }
            case integer_constant:
            {

            }
            case floating_constant:
            {

            }
            case enum_const:
            {
            }
            case char_const:
            {

            }
            case string:
            {
                
            }
            case generic_selection:
            {

            }
            default:
                goto error;
        }
    }
    else if(AST_CHILD_NUM(ast_node)==3)
    {
        child_node=AST_GET_CHILD(ast_node,1);
        if(!expr_trans_dispatch(child_node,ir_bb))
            goto error;
    }
    else
        goto error;
    return true;
error:
    return false;
}