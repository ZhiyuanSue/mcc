#include "transstmt.h"
extern IR_BB* curr_bb;
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
    //printf("%s\n",rule_type_str[ast_node->type]);
    if(IS_STMT_NODE(ast_node->type))
        return stmt_trans_dis[ast_node->type-labeled_stmt](ast_node,ir_bb);
    if(ast_node->type==statement)
    {
        return stmt_trans_dispatch(AST_GET_CHILD(ast_node,0),ir_bb);
    }
error:
#ifdef _TEST_IR_
    printf("stmt_trans_dispatch fail\n");
#endif
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
        /*find the switch begin expr*/
    }
    else if(label_node->type==KW_default){
        /*do nothing*/
        
    }
    else{   /*identifier case*/
        char* id_name=label_node->token->value;
        LIST_NODE* id_bb=ir_bb->func->BB_list;
        while(id_bb->next!=ir_bb->func->BB_list) /*the last bb must be func return bb,so it can work*/
        {
            if(!strcmp(((IR_BB*)id_bb)->symbol->value,id_name))
                break;
            id_bb=id_bb->next;
        }
        curr_bb=(IR_BB*)id_bb;
        stmt_trans_dispatch(statement_node,(IR_BB*)id_bb);
    }
    m_free(tei);
    return true;
error:
#ifdef _TEST_IR_
    printf("labeled stmt fail\n");
#endif
    return false;
}
bool compound_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=compound_stmt)
        goto error;
    for(size_t i=1;i<AST_CHILD_NUM(ast_node)-1;++i){
        AST_BASE* sub_ast=AST_GET_CHILD(ast_node,i);
        if(sub_ast){
            if(sub_ast->type==declaration&&!declaration_trans(sub_ast,ir_bb->IR_module,ir_bb->func,curr_bb))
                goto error;
            else if(sub_ast->type==statement&&!stmt_trans_dispatch(sub_ast,curr_bb))
                goto error;
        }
    }
    return true;
error:
#ifdef _TEST_IR_
    printf("compound stmt fail\n");
#endif
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
#ifdef _TEST_IR_
    printf("expr_stmt fail\n");
#endif
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
    IR_BB* expr_bb=add_new_bb(ir_bb->func,".label.if.expr.",true,ast_node->symbol_table);
    _add_after((LIST_NODE*)ir_bb,(LIST_NODE*)expr_bb);
    curr_bb=expr_bb;
    if(!expr_trans_dispatch(expr_node,expr_bb))
        goto error;
    /*deal with stmt part*/
    IR_BB* if_stmt_bb=add_new_bb(ir_bb->func,".label.if.stmt.",true,ast_node->symbol_table);
    curr_bb=if_stmt_bb;
    _add_after((LIST_NODE*)expr_bb,(LIST_NODE*)if_stmt_bb);
    if(!stmt_trans_dispatch(stmt_node,if_stmt_bb))
        goto error;
        
    /*add a if end block*/
    IR_BB* if_end_bb=add_new_bb(ir_bb->func,".label.if.end.",true,ast_node->symbol_table);
    curr_bb=if_end_bb;
    _add_after((LIST_NODE*)if_stmt_bb,(LIST_NODE*)if_end_bb);
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
#ifdef _TEST_IR_
    printf("if_stmt fail\n");
#endif
    return false;
}
bool switch_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=switch_stmt)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    /*add a end bb*/
    IR_BB* end_bb=add_new_bb(ir_bb->func,".label.switch.end.",true,ast_node->symbol_table);
    ast_node->symbol_table->switch_end_bb=end_bb;
    _add_after((LIST_NODE*)ir_bb,(LIST_NODE*)end_bb);

    /*add expr bb*/
    IR_BB* expr_bb=add_new_bb(ir_bb->func,".label.switch.expr.",true,ast_node->symbol_table);
    ast_node->symbol_table->switch_begin_bb=expr_bb;
    _add_after((LIST_NODE*)ir_bb,(LIST_NODE*)expr_bb);
    curr_bb=expr_bb;
    AST_BASE* expr_node=AST_GET_CHILD(ast_node,2);
    if(!expr_trans_dispatch(expr_node,expr_bb))
        goto error;
    
    /*add stmt bb*/
    IR_BB* stmt_bb=add_new_bb(ir_bb->func,".label.switch.stmt.",true,ast_node->symbol_table);
    _add_after((LIST_NODE*)expr_bb,(LIST_NODE*)stmt_bb);
    curr_bb=stmt_bb;
    AST_BASE* stmt_node=AST_GET_CHILD(ast_node,4);
    if(!stmt_trans_dispatch(stmt_node,stmt_bb))
        goto error;
    curr_bb=end_bb;
    m_free(tei);
    return true;
error:
#ifdef _TEST_IR_
    printf("switch stmt fail\n");
#endif
    return false;
}
bool while_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=while_stmt)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    /*add while end bb*/
    IR_BB* end_bb=add_new_bb(ir_bb->func,".label.while.end.",true,ast_node->symbol_table);
    ast_node->symbol_table->loop_end_bb=end_bb;
    _add_after((LIST_NODE*)ir_bb,(LIST_NODE*)end_bb);
    
    /*add expr bb*/
    IR_BB* expr_bb=add_new_bb(ir_bb->func,".label.while.expr.",true,ast_node->symbol_table);
    ast_node->symbol_table->loop_begin_bb=expr_bb;
    _add_after((LIST_NODE*)ir_bb,(LIST_NODE*)expr_bb);
    curr_bb=expr_bb;
    AST_BASE* expr_node=AST_GET_CHILD(ast_node,2);
    if(!expr_trans_dispatch(expr_node,expr_bb))
        goto error;
    
    /*add stmt bb*/
    IR_BB* stmt_bb=add_new_bb(ir_bb->func,".label.while.stmt.",true,ast_node->symbol_table);
    _add_after((LIST_NODE*)expr_bb,(LIST_NODE*)stmt_bb);
    curr_bb=stmt_bb;
    AST_BASE* stmt_node=AST_GET_CHILD(ast_node,4);
    if(!stmt_trans_dispatch(stmt_node,stmt_bb))
        goto error;
    
    m_free(tei);
    return true;
error:
#ifdef _TEST_IR_
    printf("whild stmt fail\n");
#endif
    return false;
}
bool do_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=do_stmt)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    /*add do end bb*/
    IR_BB* end_bb=add_new_bb(ir_bb->func,".label.do.end.",true,ast_node->symbol_table);
    ast_node->symbol_table->loop_end_bb=end_bb;
    _add_after((LIST_NODE*)ir_bb,(LIST_NODE*)end_bb);
    
    /*add stmt bb*/
    IR_BB* stmt_bb=add_new_bb(ir_bb->func,".label.do.stmt.",true,ast_node->symbol_table);
    ast_node->symbol_table->loop_begin_bb=stmt_bb;
    _add_after((LIST_NODE*)ir_bb,(LIST_NODE*)stmt_bb);
    curr_bb=stmt_bb;
    AST_BASE* stmt_node=AST_GET_CHILD(ast_node,4);
    if(!stmt_trans_dispatch(stmt_node,stmt_bb))
        goto error;
    
    /*add expr bb*/
    IR_BB* expr_bb=add_new_bb(ir_bb->func,".label.do.expr.",true,ast_node->symbol_table);
    _add_after((LIST_NODE*)stmt_bb,(LIST_NODE*)expr_bb);
    curr_bb=expr_bb;
    AST_BASE* expr_node=AST_GET_CHILD(ast_node,2);
    if(!expr_trans_dispatch(expr_node,expr_bb))
        goto error;
    m_free(tei);
    return true;
error:
#ifdef _TEST_IR_
    printf("do stmt fail\n");
#endif
    return false;
}
bool for_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=for_stmt)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    /*decide which one is expression node*/
    AST_BASE* expr_node[3]={NULL,NULL,NULL};
    int curr_expr_node=0;
    SYM_ITEM* symbol=NULL;
    for(size_t i=2;i<AST_CHILD_NUM(ast_node)-2;i++)
    {
        AST_BASE* tmp_ast=AST_GET_CHILD(ast_node,i);
        if(tmp_ast->type==declaration)
        {
            expr_node[0]=tmp_ast;
            curr_expr_node=1;
        }
        else if(IS_EXPR_NODE(tmp_ast->type))
        {
            expr_node[curr_expr_node]=tmp_ast;
        }
        else if(tmp_ast->type==semi_colon)
        {
            curr_expr_node+=1;
        }
    }
    /*the for expr have 3 part: init, condition,step*/
    /*add for init bb*/
    IR_BB* init_bb=add_new_bb(ir_bb->func,".label.for.init.",true,ast_node->symbol_table);
    _add_after((LIST_NODE*)ir_bb,(LIST_NODE*)init_bb);

    /*add expr condition bb,which is the loop begin bb*/
    IR_BB* cond_bb=add_new_bb(ir_bb->func,".label.for.cond.",true,ast_node->symbol_table);
    _add_after((LIST_NODE*)init_bb,(LIST_NODE*)cond_bb);
    ast_node->symbol_table->loop_begin_bb=cond_bb;

    /*add stmt bb*/
    IR_BB* stmt_bb=add_new_bb(ir_bb->func,".label.for.stmt.",true,ast_node->symbol_table);
    _add_after((LIST_NODE*)cond_bb,(LIST_NODE*)stmt_bb);

    /*add step bb*/
    IR_BB* step_bb=add_new_bb(ir_bb->func,".label.for.step.",true,ast_node->symbol_table);
    _add_after((LIST_NODE*)stmt_bb,(LIST_NODE*)step_bb);

    /*add end bb*/
    IR_BB* end_bb=add_new_bb(ir_bb->func,".label.for.end.",true,ast_node->symbol_table);
    _add_after((LIST_NODE*)step_bb,(LIST_NODE*)end_bb);
    ast_node->symbol_table->loop_end_bb=end_bb;
    
    if(expr_node[0]){
        /*maybe a declaration*/
        if(expr_node[0]->type==declaration)
        {
            if(!declaration_trans(expr_node[0],init_bb->IR_module,init_bb->func,init_bb))
                goto error;
        }
        else if(IS_EXPR_NODE(expr_node[0]->type))
        {
            if(!expr_trans_dispatch(expr_node[0],init_bb))
                goto error;
        }
        else
            goto error;
    }
    if(expr_node[1]){
        if(!expr_trans_dispatch(expr_node[1],cond_bb))
            goto error;
        symbol=expr_node[1]->symbol;
    }
    else{
        /* An omitted expression-2 is replaced by a nonzero constant.*/
        symbol=Create_symbol_item(tmp_symbol_str_alloc(".imm."),NMSP_DEFAULT);
        if(!insert_symbol(ast_node->symbol_table,symbol))
            goto error;
        symbol->const_expr=true;
        symbol->data_field->sint=1;
        M_TYPE* tmp_type=build_base_type(TP_SINT);
        symbol->type_vec=InitVEC(DEFAULT_CAPICITY);
        VECinsert(symbol->type_vec,(void*)tmp_type);
    }
    if(expr_node[2]){
        if(!expr_trans_dispatch(expr_node[2],step_bb))
            goto error;
    }
    /*insert a jmp ins if the control expression is true*/
    IR_INS* br_ins=add_new_ins(step_bb);
    GenINS(br_ins,OP_BR,NULL,symbol,cond_bb->symbol);
    m_free(tei);
    return true;
error:
#ifdef _TEST_IR_
    printf("for stmt fail\n");
#endif
    return false;
}
bool goto_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=goto_stmt)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    AST_BASE* identifier_node=AST_GET_CHILD(ast_node,1);
    char* id_name=identifier_node->token->value;
    LIST_NODE* id_bb=ir_bb->func->BB_list;
    while(id_bb->next!=ir_bb->func->BB_list) /*the last bb must be func return bb,so it can work*/
    {
        if(!strcmp(((IR_BB*)id_bb)->symbol->value,id_name))
            break;
        id_bb=id_bb->next;
    }
    /*add a unconditional jmp instruction*/
    IR_INS* ret_ins=add_new_ins(ir_bb);
    insert_ins_to_bb(ret_ins,ir_bb);
    GenINS(ret_ins,OP_BR,NULL,((IR_BB*)id_bb)->symbol,NULL);
    m_free(tei);
    return true;
error:
#ifdef _TEST_IR_
    printf("goto stmt fail\n");
#endif
    return false;
}
bool continue_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{   
    if(!ast_node||!ir_bb||ast_node->type!=continue_stmt)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    /*continue have only one case,jmp to the loop start block*/
    SYM* target_symbol_table=ast_node->symbol_table;
    while(1){
        if(target_symbol_table->father==NULL||(target_symbol_table->father->st_attr_type[1])!=(target_symbol_table->st_attr_type[1]))
            break;
        else
            target_symbol_table=target_symbol_table->father;
    }
    IR_BB* target_bb=target_symbol_table->loop_begin_bb;
    /*jmp to the function end bb*/
    IR_INS* jmp_ins=add_new_ins(ir_bb);
    insert_ins_to_bb(jmp_ins,ir_bb);
    GenINS(jmp_ins,OP_BR,NULL,target_bb->symbol,NULL);
    m_free(tei);
    return true;
error:
#ifdef _TEST_IR_
    printf("continue stmt fail\n");
#endif
    return false;
}
bool break_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=break_stmt)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    /*break might have two case ,break a switch or a loop*/
    SYM* target_symbol_table=ast_node->symbol_table;
    IR_BB* target_bb=NULL;
    while(1){
        if(target_symbol_table->father==NULL)
            break;
        else if((target_symbol_table->father->st_attr_type[0])!=(target_symbol_table->st_attr_type[0]))
        {
            target_bb=target_symbol_table->switch_begin_bb;
            break;
        }
        else if((target_symbol_table->father->st_attr_type[1])!=(target_symbol_table->st_attr_type[1]))
        {
            target_bb=target_symbol_table->loop_begin_bb;
            break;
        }
        else
            target_symbol_table=target_symbol_table->father;
    }
    if(!target_bb)
        goto error;
    /*jmp to the function end bb*/
    IR_INS* jmp_ins=add_new_ins(ir_bb);
    insert_ins_to_bb(jmp_ins,ir_bb);
    GenINS(jmp_ins,OP_BR,NULL,target_bb->symbol,NULL);
    m_free(tei);
    return true;
error:
#ifdef _TEST_IR_
    printf("break stmt fail\n");
#endif
    return false;
}
bool return_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=return_stmt)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    AST_BASE* expr_node=AST_GET_CHILD(ast_node,1);
    if(AST_CHILD_NUM(ast_node)==3)
    {
        if(!expr_trans_dispatch(expr_node,ir_bb))
            goto error;
        /*TODO:write in the return value*/
    }
    /*jmp to the function end bb*/
    IR_INS* ret_ins=add_new_ins(ir_bb);
    insert_ins_to_bb(ret_ins,ir_bb);
    GenINS(ret_ins,OP_BR,NULL,ast_node->symbol,NULL);
    m_free(tei);
    return true;
error:
#ifdef _TEST_IR_
    printf("return stmt fail\n");
#endif
    return false;
}
bool asm_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb||ast_node->type!=asm_stmt)
        goto error;
    /*I don't want to deal with it...emm*/
    return true;
error:
#ifdef _TEST_IR_
    printf("asm stmt fail\n");
#endif
    return false;
}