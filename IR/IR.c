#include "IR.h"
extern char operand_name_string[OPERAND_NUM][OPERAND_STR_LEN];
extern char op_name_string[OP_NUM][OP_STRING_LEN];
extern char data_type_name_string[OPERAND_NUM][OPERAND_STR_LEN];
extern char ICMP_str[OP_NUM][OP_STRING_LEN];
extern char FCMP_str[OP_NUM][OP_STRING_LEN];
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
    ins->dst=dst;
    ins->src1=src1;
    ins->src2=src2;
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
    new_bb->ignore=false;
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
        STOR_VALUE* value=(STOR_VALUE*)VEC_GET_ITEM(irm->static_stor_symbols,i);
        print_static_stor_value(value);
    }
    printf("<function definitions>:%ld functions in total\n",VECLEN(irm->func_list));
    if(irm->func_list)
    {
        for(size_t i=0;i<VECLEN(irm->func_list);++i)
        {
            /*print function*/
            IR_FUNC* curr_func=VEC_GET_ITEM(irm->func_list,i);
            printf(".globl function:%s\n",curr_func->func_name);
            printf("{\n");
            LIST_NODE* bb_p=curr_func->BB_list;
            while(bb_p)
            {
                IR_BB* curr_bb=(IR_BB*)bb_p;
                LIST_NODE* ins_p=curr_bb->Instruction_list;
                printf("\t<bb :%s>\n",curr_bb->symbol->value);
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
    printf("<op:%s ",op_name_string[ins->op]);
    if(ins->op==OP_ICMP)
        printf("%s\t",ICMP_str[((CMP_COND_ATTR*)(ins->other_attr))->icmp_cond]);
    else if(ins->op==OP_FCMP)
        printf("%s\t",FCMP_str[((CMP_COND_ATTR*)(ins->other_attr))->fcmp_cond]);
    printf(":");
    print_OPERAND(ins->dst,indentation+2);
    printf(":");
    print_OPERAND(ins->src1,indentation+2);
    printf(":");
    print_OPERAND(ins->src2,indentation+2);
    printf(">\n");
}
void print_OPERAND(SYM_ITEM* operand,size_t indentation)
{
    if(!operand)
        printf("<None>");
    else
        printf("<%s>",operand->value);
}
void print_static_stor_value(STOR_VALUE* value)
{
    printf("\t.globl\t%s\n",value->sym_item->value);
    printf("%s:\n",value->sym_item->value);
    for(size_t i=0;i<VECLEN(value->value_vec);++i)
    {
        STOR_VALUE_ELEM* elem=VEC_GET_ITEM(value->value_vec,i);
        if(!elem||elem->ignore)
            continue;
        if(elem->value_data_type==SVT_NONE)
        {
            if(elem->byte_width==0)
#ifdef _MAC_
                printf("\t.space\t%llu\n",elem->idata);
#endif
#ifdef _UNIX_
                printf("\t.zero\t%llu\n",elem->idata);
#endif
            else if(elem->byte_width==1)
                printf("\t.byte\t%llu\n",elem->idata);
            else if(elem->byte_width==2)
                printf("\t.word\t%llu\n",elem->idata);
            else if(elem->byte_width==4)
                printf("\t.long\t%llu\n",elem->idata);
            else if(elem->byte_width==8)
                printf("\t.quad\t%llu\n",elem->idata); 
        }
        else if(elem->value_data_type==SVT_POINTER)
        {
            printf("\t.quad\t%s\n",((SYM_ITEM*)(elem->idata))->value); 
        }
    }
}
bool type_cast_trans(SYM_ITEM* dst,SYM_ITEM* src,IR_BB* ir_bb)
{
    if(!dst||!src)
        goto error;
    VEC* dst_type_vec=dst->type_vec;
    VEC* src_type_vec=src->type_vec;
    M_TYPE* dst_base_type=Type_VEC_get_actual_base_type(dst_type_vec);
    M_TYPE* src_base_type=Type_VEC_get_actual_base_type(src_type_vec);
    enum TP_CATEGORY dst_type_category=dst_base_type->typ_category;
    enum TP_CATEGORY src_type_category=src_base_type->typ_category;
    if(dst_type_category==src_type_category)
        return true;
    IR_INS* trans_ins=add_new_ins(ir_bb);
    insert_ins_to_bb(trans_ins,ir_bb);
    enum ins_op op=OP_NONE;
    if(IS_INT_TYPE(dst_type_category)&&IS_INT_TYPE(src_type_category))
    {
        /*int trunc case*/

        /*int zero extern*/

        /*int signed extern*/

    }
    else if(IS_INT_TYPE(dst_type_category)&&IS_FLOAT_TYPE(src_type_category))
    {
        /*float to unsigned int*/

        /*float to signed int*/

    }
    else if(IS_FLOAT_TYPE(dst_type_category)&&IS_INT_TYPE(src_type_category))
    {
        /*unsigned int to float*/

        /*signed int to float*/

    }
    else if(IS_FLOAT_TYPE(dst_type_category)&&IS_FLOAT_TYPE(src_type_category))
    {
        /*float trunc*/

        /*float extern*/

    }
    GenINS(trans_ins,op,dst,src,NULL);
    return true;
error:
    return false;
}
M_TYPE* usual_arth_conversion_trans(SYM_ITEM** operand_a,SYM_ITEM** operand_b,IR_BB* ir_bb)
{
    if(!operand_a||!operand_b||!ir_bb)
        goto error;
    M_TYPE* tmp_prefix_expr_type=Type_VEC_get_actual_base_type((*operand_a)->type_vec);
    M_TYPE* tmp_right_expr_type=Type_VEC_get_actual_base_type((*operand_b)->type_vec);
    if(!IS_ARTH_TYPE(tmp_prefix_expr_type->typ_category)||!IS_ARTH_TYPE(tmp_right_expr_type->typ_category))
        goto error;
    SYM_ITEM* old_a=*operand_a;
    SYM_ITEM* old_b=*operand_b;
    SYM* symbol_table_a=(*operand_a)->symbol_table;
    SYM* symbol_table_b=(*operand_b)->symbol_table;
    M_TYPE* cmp_type=usual_arith_conversion(&tmp_prefix_expr_type,&tmp_right_expr_type);
    if(tmp_prefix_expr_type->typ_category!=cmp_type->typ_category)
    {
        (*operand_a)=Create_symbol_item(tmp_symbol_str_alloc(".reg."),NMSP_DEFAULT);
        (*operand_a)->count=HASH_CNT_IST;
        insert_symbol(symbol_table_a,(*operand_a));
        tmp_prefix_expr_type=build_base_type(cmp_type->typ_category);
        VECinsert((*operand_a)->type_vec,(void*)tmp_prefix_expr_type);
        type_cast_trans((*operand_a),old_a,ir_bb);
    }
    if(tmp_right_expr_type->typ_category!=cmp_type->typ_category)
    {
        (*operand_b)=Create_symbol_item(tmp_symbol_str_alloc(".reg."),NMSP_DEFAULT);
        (*operand_b)->count=HASH_CNT_IST;
        insert_symbol(symbol_table_b,(*operand_b));
        tmp_right_expr_type=build_base_type(cmp_type->typ_category);
        VECinsert((*operand_b)->type_vec,(void*)tmp_right_expr_type);
        type_cast_trans((*operand_b),old_b,ir_bb);
    }
    return cmp_type;
error:
    return NULL;
}