#include "IR.h"

bool GenINS(IR_INS* ins){
    if(!ins)
        goto error;
    return true;
error:
    return false;
}
IR_FUNC* add_new_func(IR_MODULE* irm)
{
    if(!irm)
        goto error;
    IR_FUNC* new_function=(IR_FUNC*)m_alloc(sizeof(IR_FUNC));
    new_function->BB_list=NULL;
    new_function->func_name=NULL;
    new_function->register_list=NULL;
    new_function->IR_module=irm;
    VECinsert(irm->func_list,(void*)new_function);
    return new_function;
error:
    return NULL;
}
IR_BB* add_new_bb(IR_FUNC* func)
{
    if(!func)
        goto error;
    IR_BB* new_bb=(IR_BB*)m_alloc(sizeof(IR_BB));
    new_bb->Instruction_list=NULL;
    new_bb->bb_label=NULL;
    new_bb->func=func;
    new_bb->IR_module=func->IR_module;
    if(!func->BB_list)
    {
        init_list_node(&(new_bb->node));
        func->BB_list=&(new_bb->node);
    }
    else{
        _add_before(func->BB_list,&(new_bb->node)); /*add to the list tail*/
    }
    return new_bb;
error:
    return NULL;
}
IR_INS* add_new_ins(IR_BB* bb)
{
    if(!bb)
        goto error;
    IR_INS* new_ins=(IR_INS*)m_alloc(sizeof(IR_INS));
    new_ins->block=bb;
    new_ins->func=bb->func;
    new_ins->IR_module=bb->IR_module;
    new_ins->op=OP_NONE;
    new_ins->dst=(IR_OPERAND*)m_alloc(sizeof(IR_OPERAND));
    new_ins->dst->type=OPERAND_NONE;
    new_ins->src1=(IR_OPERAND*)m_alloc(sizeof(IR_OPERAND));
    new_ins->src1->type=OPERAND_NONE;
    new_ins->src2=(IR_OPERAND*)m_alloc(sizeof(IR_OPERAND));
    new_ins->src2->type=OPERAND_NONE;
    if(!bb->Instruction_list)
    {
        init_list_node(&(new_ins->node));
        bb->Instruction_list=&(new_ins->node);
    }
    else{
        _add_before(bb->Instruction_list,&(new_ins->node)); /*add to the list tail*/
    }
    return new_ins;
error:
    return NULL;
}

char* label_allocator(void)
{
    return NULL;
}
void print_IR(IR_MODULE* irm)
{
    printf("<external declarations>:\n");
    for(size_t i=0;i<VECLEN(irm->global_and_external_symbols);++i)
    {
        SYM_ITEM* tmpsi=(SYM_ITEM*)VEC_GET_ITEM(irm->global_and_external_symbols,i);
        print_symbol(tmpsi,0);
    }
    printf("<function definitions>:\n");
    for(size_t i=0;i<VECLEN(irm->func_list);++i)
    {
        printf(".globl function:\n");

        printf("{\n");


        printf("}\n");
    }
}