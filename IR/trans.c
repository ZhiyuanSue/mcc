#include "trans.h"
IR_BB* curr_bb=NULL;
extern size_t stack_off;
IR_MODULE* trans_to_IR(AST_BASE* ast_node)
{   /*do some init work*/
    if(!ast_node||ast_node->type!=translation_unit)
        goto error;
    IR_MODULE* res=m_alloc(sizeof(IR_MODULE));
    res->static_stor_symbols=InitVEC(DEFAULT_CAPICITY);
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
            if (!declaration_trans(ast_child,res,NULL,NULL))
                goto error;
            break;
        }
        case function_definition:
        {
            IR_FUNC* ir_new_func=add_new_func(res,ast_child->symbol);
            if(!trans_func(ast_child,ir_new_func))
                goto error;
            break;
        }
        default:
            goto error;
        }
    }
    for(size_t i=0;i<AST_CHILD_NUM(ast_node);++i)
    {
        AST_BASE* external_decl_node=AST_GET_CHILD(ast_node,i);
        AST_BASE* ast_child=AST_GET_CHILD(external_decl_node,0);
        if(ast_child->type==function_definition)
        {
            /*return to the first bb and start*/
            SYM_ITEM* tmpsi=ast_child->symbol;
            M_TYPE* tmpt=Type_VEC_get_actual_base_type(tmpsi->type_vec);
            TP_FUNC* function_type=(TP_FUNC*)tmpt;
            curr_bb=(IR_BB*)(function_type->ir_func->BB_list);
            stack_off=0;    /*for every function begin,the stack base come back to 0*/
            AST_BASE* compount_stmt_node=AST_GET_CHILD(ast_child,AST_CHILD_NUM(ast_child)-1);
            if(!compound_stmt_trans(compount_stmt_node,curr_bb))
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
    SYM_ITEM* tmpsi=ast_node->symbol;
    M_TYPE* tmpt=Type_VEC_get_actual_base_type(tmpsi->type_vec);
    if(!(tmpt->typ_category==TP_FUNCTION))
        goto error;
    ir_func->func_name=((TP_FUNC*)tmpt)->func_name;
    ir_func->symbol=ast_node->symbol;
    ((TP_FUNC*)tmpt)->ir_func=ir_func;
    /*do some work for arguments*/
    /*add a return basic block,in which only one ins, ret*/
    IR_BB* last_bb=add_new_bb(ir_func,".func.end.",true,ast_node->symbol_table);
    IR_INS* ret_ins=add_new_ins(last_bb);
    last_bb->Instruction_list=(LIST_NODE*)ret_ins;

    GenINS(ret_ins,OP_RET,NULL,NULL,NULL);

    ast_node->symbol_table->func_end_bb=last_bb;

    /*compound stmt part*/
    IR_BB* compound_bb=add_new_bb(ir_func,((TP_FUNC*)tmpt)->func_name,false,ast_node->symbol_table);
    _add_before((LIST_NODE*)last_bb,(LIST_NODE*)compound_bb);
    curr_bb=compound_bb;
    ir_func->BB_list=(LIST_NODE*)compound_bb;
    /*
        if there's some labels in the function, it must dealed first,
        otherwise the goto stmt might cannot find the bb
    */
    VEC* label_si_vec=get_symbol_hash(ast_node->symbol_table->sym_hash_table);
    for(size_t i=0;i<VECLEN(label_si_vec);++i)
    {
        tmpsi=VEC_GET_ITEM(label_si_vec,i);
        if(!tmpsi||tmpsi->count<=0)
            continue;
        IR_BB* label_bb=add_new_bb(ir_func,tmpsi->value,false,ast_node->symbol_table);
        _add_after((LIST_NODE*)curr_bb,(LIST_NODE*)label_bb);
        curr_bb=label_bb;
        tmpt=Type_VEC_get_actual_base_type(tmpsi->type_vec);
    }
    return true;
error:
    return false;
}