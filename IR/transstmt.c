#include "transstmt.h"
/*as semantic has been done,so ,we needn't do too much checks*/
bool stmt_trans_dispatch(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    bool (*stmt_trans_dis[STMT_NUM])(AST_BASE* ast_node,IR_BB* ir_bb)={
        labeled_stmt_trans,compound_stmt_trans,expr_stmt_trans,if_stmt_trans,
        switch_stmt_trans,while_stmt_trans,do_stmt_trans,for_stmt_trans,
        goto_stmt_trans,continue_stmt_trans,break_stmt_trans,return_stmt_trans,
        asm_stmt_trans
    };
    if(IS_STMT_NODE(ast_node->type))
        return stmt_trans_dis[ast_node->type-labeled_stmt](ast_node,ir_bb);
    if(ast_node->type==statement)
    {
        return stmt_trans_dispatch(AST_GET_CHILD(ast_node,0),ir_bb);
    }
error:
    return false;
}
bool labeled_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=labeled_stmt)
        goto error;
    
    return true;
error:
    return false;
}
bool compound_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=compound_stmt)
        goto error;
    for(size_t i=1;i<AST_CHILD_NUM(ast_node)-1;++i){
        AST_BASE* sub_ast=AST_GET_CHILD(ast_node,i);
        if(sub_ast){
            if(sub_ast->type==declaration&&!declaration_trans(sub_ast,ir_bb->IR_module,ir_bb->func,ir_bb))
                goto error;
            else if(sub_ast->type==statement&&!stmt_trans_dispatch(sub_ast,ir_bb))
                goto error;
        }
    }
    return true;
error:
    return false;
}
bool expr_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=expr_stmt)
        goto error;
    if(AST_CHILD_NUM(ast_node)==1)
        return true;
    else if(AST_CHILD_NUM(ast_node)==2){
        AST_BASE* expr_node=AST_GET_CHILD(ast_node,0);
        return expr_trans_dispatch(expr_node,ir_bb);
    }
    else
        goto error;
    return true;
error:
    return false;
}
bool if_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=if_stmt)
        goto error;
    return true;
error:
    return false;
}
bool switch_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=switch_stmt)
        goto error;
    return true;
error:
    return false;
}
bool while_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=while_stmt)
        goto error;
    return true;
error:
    return false;
}
bool do_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=do_stmt)
        goto error;
    return true;
error:
    return false;
}
bool for_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=for_stmt)
        goto error;
    return true;
error:
    return false;
}
bool goto_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=goto_stmt)
        goto error;
    return true;
error:
    return false;
}
bool continue_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{   
    if(!ast_node||!ir_bb||ast_node->type!=continue_stmt)
        goto error;
    return true;
error:
    return false;
}
bool break_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=break_stmt)
        goto error;
    return true;
error:
    return false;
}
bool return_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=return_stmt)
        goto error;
    return true;
error:
    return false;
}
bool asm_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=asm_stmt)
        goto error;
    return true;
error:
    return false;
}