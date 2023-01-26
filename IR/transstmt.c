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
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    AST_BASE* label_node=AST_GET_CHILD(ast_node,0);
    AST_BASE* statement_node=(AST_BASE*)VEC_BACK(ast_node->child);
    if(label_node->type==KW_case){

    }
    else if(label_node->type==KW_default){

    }
    else{   /*identifier case*/
        IR_BB* new_bb=add_new_bb(ir_bb->func);
        _add_after((LIST_NODE*)ir_bb,(LIST_NODE*)new_bb);
        new_bb->bb_label=label_node->token->value;
        stmt_trans_dispatch(statement_node,new_bb);
    }
    m_free(tei);
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
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    AST_BASE* expr_node=AST_GET_CHILD(ast_node,2);
    AST_BASE* stmt_node=AST_GET_CHILD(ast_node,4);
    /*deal with expr part*/
    IR_BB* expr_bb=add_new_bb(ir_bb->func);
    _add_after((LIST_NODE*)ir_bb,(LIST_NODE*)expr_bb);
    expr_bb->bb_label=label_allocator("if expr");
    if(!expr_trans_dispatch(expr_node,expr_bb))
        goto error;
    
    /*deal with stmt part*/
    IR_BB* if_stmt_bb=add_new_bb(ir_bb->func);
    _add_after((LIST_NODE*)expr_bb,(LIST_NODE*)if_stmt_bb);
    if_stmt_bb->bb_label=label_allocator("if stmt");
    stmt_node=AST_GET_CHILD(ast_node,6);
    if(!stmt_trans_dispatch(stmt_node,if_stmt_bb))
        goto error;
    /*add a if end block*/
    IR_BB* if_end_bb=add_new_bb(ir_bb->func);
    _add_after((LIST_NODE*)if_stmt_bb,(LIST_NODE*)if_end_bb);
    if_end_bb->bb_label=label_allocator("if end");
    if(AST_CHILD_NUM(ast_node)==7)  /*else part*/
    {
        stmt_node=AST_GET_CHILD(ast_node,6);
        if(!stmt_trans_dispatch(stmt_node,if_end_bb))
            goto error;
    }
    /*TODO*/
    /*change the jmp position*/
    /*find the last ins of expr bb*/

    /*add a condition jmp ins at the last of the expr bb*/

    m_free(tei);
    return true;
error:
    return false;
}
bool switch_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=switch_stmt)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    /*add a end bb*/
    IR_BB* end_bb=add_new_bb(ir_bb->func);
    ast_node->symbol_table->switch_end_bb=end_bb;
    _add_after((LIST_NODE*)ir_bb,(LIST_NODE*)end_bb);
    end_bb->bb_label=label_allocator("switch end");

    /*add expr bb*/
    IR_BB* expr_bb=add_new_bb(ir_bb->func);
    ast_node->symbol_table->switch_begin_bb=expr_bb;
    _add_after((LIST_NODE*)ir_bb,(LIST_NODE*)expr_bb);
    expr_bb->bb_label=label_allocator("switch expr");
    AST_BASE* expr_node=AST_GET_CHILD(ast_node,2);
    if(!expr_trans_dispatch(expr_node,expr_bb))
        goto error;

    /*add stmt bb*/
    IR_BB* stmt_bb=add_new_bb(ir_bb->func);
    _add_after((LIST_NODE*)expr_bb,(LIST_NODE*)stmt_bb);
    expr_bb->bb_label=label_allocator("switch stmt");
    AST_BASE* stmt_node=AST_GET_CHILD(ast_node,4);
    if(!stmt_trans_dispatch(stmt_node,stmt_bb))
        goto error;
    
    m_free(tei);
    return true;
error:
    return false;
}
bool while_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=while_stmt)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    
    m_free(tei);
    return true;
error:
    return false;
}
bool do_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=do_stmt)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    
    m_free(tei);
    return true;
error:
    return false;
}
bool for_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=for_stmt)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    
    m_free(tei);
    return true;
error:
    return false;
}
bool goto_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=goto_stmt)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    
    m_free(tei);
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