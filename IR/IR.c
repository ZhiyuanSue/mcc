#include "IR.h"
extern char operand_name_string[OPERAND_NUM][OPERAND_STR_LEN];
extern char op_name_string[OP_NUM][OP_STRING_LEN];
bool GenINS(IR_INS* ins,
    enum ins_op op,
    IR_OPERAND* dst,
    IR_OPERAND* src1,
    IR_OPERAND* src2){
    if(!ins)
        goto error;
    /*check the operand type for op*/

    /*fill in*/
    ins->op=op;
    if(dst==NULL)
        ins->dst->type=OPERAND_NONE;
    else
        ins->dst=dst;
    if(src1==NULL)
        ins->src1->type=OPERAND_NONE;
    else
        ins->src1=src1;
    if(src2==NULL)
        ins->src2->type=OPERAND_NONE;
    else
        ins->src2=src2;    
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
    new_bb->node.next=new_bb->node.prev=(LIST_NODE*)new_bb;
    new_bb->Instruction_list=NULL;
    new_bb->bb_label=NULL;
    new_bb->func=func;
    new_bb->IR_module=func->IR_module;
    return new_bb;
error:
    return NULL;
}
IR_INS* add_new_ins(IR_BB* bb)
{
    if(!bb)
        goto error;
    IR_INS* new_ins=(IR_INS*)m_alloc(sizeof(IR_INS));
    new_ins->node.next=new_ins->node.prev=(LIST_NODE*)new_ins;
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
    return new_ins;
error:
    return NULL;
}
static size_t label_id=0;
char* label_allocator(void)
{
    label_id++;

    return NULL;
}
static size_t next_alloc_reg_id=0;
IR_REG* reg_allocator(IR_MODULE* irm,enum reg_type type,IR_INS* op)
{
    IR_REG* new_reg=(IR_REG*)m_alloc(sizeof(IR_REG));
    new_reg->type=type;
    new_reg->reg_id=next_alloc_reg_id;
    next_alloc_reg_id++;
    VECinsert(irm->reg_list,(void*)new_reg);
    return new_reg;
}
void print_IR(IR_MODULE* irm)
{
    printf("<external declarations>:%ld declarations in total\n",VECLEN(irm->global_and_external_symbols));
    for(size_t i=0;i<VECLEN(irm->global_and_external_symbols);++i)
    {
        SYM_ITEM* tmpsi=(SYM_ITEM*)VEC_GET_ITEM(irm->global_and_external_symbols,i);
        print_symbol(tmpsi,0);
    }
    printf("<function definitions>:%ld functions in total\n",VECLEN(irm->func_list));
    if(irm->func_list)
    {
        for(size_t i=0;i<VECLEN(irm->func_list);++i)
        {
            printf(".globl function:\n");
            /*print function*/
            IR_FUNC* curr_func=VEC_GET_ITEM(irm->func_list,i);
            printf("{\n");
            LIST_NODE* bb_p=curr_func->BB_list;
            while(bb_p)
            {
                IR_BB* curr_bb=(IR_BB*)bb_p;
                LIST_NODE* ins_p=curr_bb->Instruction_list;
                if(ins_p)
                    printf("<basic block label:%s>\n",curr_bb->bb_label);
                while (ins_p)
                {
                    print_INS((IR_INS*)ins_p,1);
                    ins_p=ins_p->next;
                    if(ins_p==curr_bb->Instruction_list)
                        break;
                }
                bb_p=bb_p->next;
                if(bb_p==curr_func->BB_list)
                    break;
            }
            printf("}\n");
        }
    }
}
void print_INS(IR_INS* ins,size_t indentation)
{
    if(!ins)
        return;
    for(size_t i=0;i<indentation+1;++i){
        printf(" ");
    }
    printf("<op:%s,",op_name_string[ins->op]);
    printf("dst:%s,",operand_name_string[ins->dst->type]);
    printf("src1:%s,",operand_name_string[ins->src1->type]);
    printf("src2:%s>\n",operand_name_string[ins->src2->type]);
}