#include "trans.h"

IR_MODULE* trans_to_IR(AST_BASE* ast_node)
{   /*do some init work*/
    if(!ast_node||ast_node->type!=translation_unit)
        goto error;
    IR_MODULE* res=m_alloc(sizeof(IR_MODULE));
    res->global_and_external_symbols=InitVEC(DEFAULT_CAPICITY);
    res->func_list=InitVEC(DEFAULT_CAPICITY);
    res->reg_list=InitVEC(DEFAULT_CAPICITY);
    res->bind_reg_list=InitVEC(DEFAULT_CAPICITY);
   
    for(size_t i=0;i<6;++i){
        /*push int regs into bind reg list*/
        IR_REG* tmp_reg=reg_allocator(res,DATA_INTEGER,res->bind_reg_list);
    }
    for(size_t i=0;i<8;++i){
        /*push float fegs into bind reg list*/
        IR_REG* tmp_reg=reg_allocator(res,DATA_FLOAT,res->bind_reg_list);
    }
    /*push count reg*/
    IR_REG* tmp_reg=reg_allocator(res,DATA_INTEGER,res->bind_reg_list);
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
            if (!declaration_trans(ast_child,res,NULL,NULL))
                goto error;
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
    SYM_ITEM* tmpsi=ast_node->func_attribute;
    /*do some work for arguments*/

    /*add a return basic block,in which only one ins, ret*/
    IR_BB* last_bb=add_new_bb(ir_func);
    ir_func->BB_list=(LIST_NODE*)last_bb;

    last_bb->bb_label=label_allocator();
    IR_INS* ret_ins=add_new_ins(last_bb);
    last_bb->Instruction_list=(LIST_NODE*)ret_ins;

    GenINS(ret_ins,OP_RET,NULL,NULL,NULL);
    /*compound stmt part*/
    IR_BB* compound_bb=add_new_bb(ir_func);
    _add_before((LIST_NODE*)last_bb,(LIST_NODE*)compound_bb);
    AST_BASE* compount_stmt_node=AST_GET_CHILD(ast_node,AST_CHILD_NUM(ast_node)-1);
    compound_stmt_trans(compount_stmt_node,compound_bb);
    return true;
error:
    return false;
}