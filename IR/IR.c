#include "IR.h"
extern char operand_name_string[OPERAND_NUM][OPERAND_STR_LEN];
extern char op_name_string[OP_NUM][OP_STRING_LEN];
extern char data_type_name_string[OPERAND_NUM][OPERAND_STR_LEN];
bool GenINS(
    IR_INS* ins,
    enum ins_op op,
    SYM_ITEM* dst,
    SYM_ITEM* src1,
    SYM_ITEM* src2){
    if(!ins)
        goto error;
    /*check the operand type for op*/

    /*fill in*/
    ins->op=op;
    
    if(op==OP_ICMP||op==OP_FCMP)
        ins->other_attr=(void*)m_alloc(sizeof(CMP_COND_ATTR));
    return true;
error:
    return false;
}
IR_FUNC* add_new_func(IR_MODULE* irm,SYM_ITEM* func_symbol)
{
    if(!irm)
        goto error;
    IR_FUNC* new_function=(IR_FUNC*)m_alloc(sizeof(IR_FUNC));
    new_function->BB_list=NULL;
    new_function->func_name=func_symbol->value;
    new_function->IR_module=irm;
    new_function->symbol=func_symbol;
    VECinsert(irm->func_list,(void*)new_function);
    return new_function;
error:
    return NULL;
}
IR_BB* add_new_bb(IR_FUNC* func,char* label_prefix,bool need_gen_new_label,SYM* symbol_table)
{
    if(!func)
        goto error;
    IR_BB* new_bb=(IR_BB*)m_alloc(sizeof(IR_BB));
    new_bb->node.next=new_bb->node.prev=(LIST_NODE*)new_bb;
    new_bb->Instruction_list=NULL;
    if(need_gen_new_label)
        new_bb->symbol=Create_symbol_item(tmp_symbol_str_alloc(label_prefix),NMSP_DEFAULT);
    else
        new_bb->symbol=Create_symbol_item(label_prefix,NMSP_DEFAULT);
    new_bb->symbol->count=HASH_CNT_IST;
    if(symbol_table->sp_type!=SPT_FUN)
        insert_symbol(symbol_table,new_bb->symbol);
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
    new_ins->dst=NULL;
    new_ins->src1=NULL;
    new_ins->src2=NULL;
    return new_ins;
error:
    return NULL;
}
bool insert_ins_to_bb(IR_INS* ins,IR_BB* ir_bb)
{
    if(!ins||!ir_bb)
        goto error;
    if(ir_bb->Instruction_list==NULL)
        ir_bb->Instruction_list=(LIST_NODE*)ins;
    else
        _add_before((LIST_NODE*)(ir_bb->Instruction_list),(LIST_NODE*)ins);
    return true;
error:
    return false;
}
void print_IR(IR_MODULE* irm)
{
    printf("<external declarations>:%ld declarations in total\n",VECLEN(irm->static_stor_symbols));
    for(size_t i=0;i<VECLEN(irm->static_stor_symbols);++i)
    {
        STATIC_STOR_VALUE* value=(STATIC_STOR_VALUE*)VEC_GET_ITEM(irm->static_stor_symbols,i);
        SYM_ITEM* tmpsi=value->sym_item;
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
                printf("\t<basic block label:%s>\n",curr_bb->symbol->value);
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
    for(size_t i=0;i<indentation;++i){
        printf("\t");
    }
    for(size_t i=0;i<indentation;++i){
        printf("\t");
    }
    printf("->\t");
    printf("op:%s\n",op_name_string[ins->op]);

    for(size_t i=0;i<indentation+2;++i){
        printf("\t");
    }
    printf("dst:\t");
    print_OPERAND(ins->dst,indentation+2);
    for(size_t i=0;i<indentation+2;++i){
        printf("\t");
    }
    printf("src1:\t");
    print_OPERAND(ins->src1,indentation+2);
    for(size_t i=0;i<indentation+2;++i){
        printf("\t");
    }
    printf("src2:\t");
    print_OPERAND(ins->src2,indentation+2);
}
void print_OPERAND(SYM_ITEM* operand,size_t indentation)
{
    if(!operand)
        printf("None\n");
}