#include "IR.h"
extern char operand_name_string[OPERAND_NUM][OPERAND_STR_LEN];
extern char op_name_string[OP_NUM][OP_STRING_LEN];
bool GenINS(
    IR_INS* ins,
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
    if(op==OP_ICMP||op==OP_FCMP)
        ins->other_attr=(void*)m_alloc(sizeof(CMP_COND_ATTR));
    return true;
error:
    return false;
}
IR_REG* GenREG(
    enum data_type type,
    VEC* reg_list,
    IR_INS* ins,
    size_t data_length
){
    if(!reg_list)
        goto error;
    IR_REG* reg=m_alloc(sizeof(IR_REG));
    reg->d_type=type;
    reg->reg_id=VECLEN(reg_list);
    VECinsert(reg_list,(void*)reg);
    reg->ins=ins;
    reg->data_length=data_length;
    return reg;
error:
    return NULL;
}
bool GenREGPointerType(
    IR_REG* reg,
    VEC* type_vec
){
    M_TYPE* tmpt=Type_VEC_get_actual_base_type(type_vec);
    if(IS_INT_TYPE(tmpt->typ_category)||tmpt->typ_category==TP_POINT||tmpt->typ_category==TP_ENUM)
        reg->d_type=DATA_POINTER_INTEGER;
    else if(IS_FLOAT_TYPE(tmpt->typ_category))
        reg->d_type=DATA_POINTER_FLOAT;
    else if(tmpt->typ_category==TP_UNION_STRUCT||tmpt->typ_category==TP_UNION||tmpt->typ_category==TP_STRUCT)
        reg->d_type=DATA_POINTER_STRUCT_UNION;
    else if(tmpt->typ_category==TP_ARRAY)
    {
        type_vec=Type_VEC_get_Array_TO(type_vec,true);
        tmpt=Type_VEC_get_actual_base_type(type_vec);
        if(IS_INT_TYPE(tmpt->typ_category)||tmpt->typ_category==TP_POINT||tmpt->typ_category==TP_ENUM)
            reg->d_type=DATA_POINTER_INTEGER_ARRAY;
        else if(IS_FLOAT_TYPE(tmpt->typ_category))
            reg->d_type=DATA_POINTER_FLOAT_ARRAY;
        else
            reg->d_type=DATA_POINTER_OTHER_ARRAY;
    }
    return true;
}
IR_OPERAND* GenOPERAND_DATA(
    enum data_storage_type data_stor_type,
    size_t data_length,
    size_t data_align
)
{
    if(data_stor_type==IR_STOR_NONE)
        goto error;
    IR_OPERAND* operand=m_alloc(sizeof(IR_OPERAND));
    operand->type=OPERAND_DATA;
    operand->operand_data.operand_data_type.data_stor_type=data_stor_type;
    operand->operand_data.operand_data_type.data_length=data_length;
    operand->operand_data.operand_data_type.data_align=data_align;
    return operand;
error:
    return NULL;
}
IR_OPERAND* GenOPERAND_IMM(
    enum TP_CATEGORY imm_type,
    signed long long int imm_int_data,
    float imm_float_data,
    double imm_double_data,
    long double imm_long_double_data
)
{
    if(imm_type==TP_SPEC_NONE)
        goto error;
    IR_OPERAND* operand=m_alloc(sizeof(IR_OPERAND));
    operand->type=OPERAND_IMM;
    operand->operand_data.operand_imm_type.imm_type=imm_type;
    if(imm_type==TP_NULL_POINTER_CONSTANT)
        operand->operand_data.operand_imm_type.imm_int_data=0;
    else if(IS_INT_TYPE(imm_type)||imm_type==TP_ENUM||imm_type==TP_POINT)
        operand->operand_data.operand_imm_type.imm_int_data=imm_int_data;
    else if(imm_type==TP_FLOAT)
        operand->operand_data.operand_imm_type.imm_float_data=imm_float_data;
    else if(imm_type==TP_DOUBLE)
        operand->operand_data.operand_imm_type.imm_float_data=imm_float_data;
    else if(imm_type==TP_LONG_DOUBLE)
        operand->operand_data.operand_imm_type.imm_float_data=imm_float_data;
    else
        goto error;
    return operand;
error:
    return NULL;
}
IR_OPERAND* GenOPERAND_CODE(
    IR_BB* code_position
)
{
    if(!code_position)
        goto error;
    IR_OPERAND* operand=m_alloc(sizeof(IR_OPERAND));
    operand->type=OPERAND_CODE;
    operand->operand_data.operand_code_type.code_position=code_position;
    return operand;
error:
    return NULL;
}
IR_OPERAND* GenOPERAND_REG(
    IR_REG* operand_reg
)
{
    if(!operand_reg)
        goto error;
    IR_OPERAND* operand=m_alloc(sizeof(IR_OPERAND));
    operand->type=OPERAND_REG;
    operand->operand_data.operand_reg_type=operand_reg;
    return operand;
error:
    return NULL;
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
    new_function->symbol_table=NULL;
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
char* label_allocator(char* default_name)
{
    if(default_name!=NULL)
        return default_name;
    label_id++;

    return NULL;
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
                printf("\t<basic block label:%s>\n",curr_bb->bb_label);
                while (ins_p)
                {
                    print_INS((IR_INS*)ins_p,2);
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
    printf("<op:%s,",op_name_string[ins->op]);
    printf("dst:%s,",operand_name_string[ins->dst->type]);
    printf("src1:%s,",operand_name_string[ins->src1->type]);
    if(ins->src1->type==OPERAND_CODE)
    {
        printf("(to label {");
        printf("%s",ins->src1->operand_data.operand_code_type.code_position->bb_label);
        printf("})");
    }
    printf("src2:%s",operand_name_string[ins->src2->type]);
    if(ins->src2->type==OPERAND_CODE)
    {
        printf("(to label {");
        printf("%s",ins->src2->operand_data.operand_code_type.code_position->bb_label);
        printf("})");
    }
    printf(">\n");
}