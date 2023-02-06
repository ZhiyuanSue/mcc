#ifndef _IR_H_
#define _IR_H_
#include "../tools/vector.h"
#include "../tools/list.h"
#include "../tools/symbol_table.h"
#include "IR_ENUM.h"
typedef struct instruction{
    LIST_NODE node;
    enum ins_op op;
    SYM_ITEM* dst;
    SYM_ITEM* src1;
    SYM_ITEM* src2;
    IR_MODULE* IR_module;
    IR_FUNC* func;
    IR_BB* block;
    void* other_attr;
}IR_INS;
typedef union cmp_cond{
    enum IR_OP_ICMP_COND icmp_cond;
    enum IR_OP_FCMP_COND fcmp_cond;
}CMP_COND_ATTR;
bool GenINS(
    IR_INS* ins,
    enum ins_op op,
    SYM_ITEM* dst,
    SYM_ITEM* src1,
    SYM_ITEM* src2);   /*use add_new_ins to get a new one and fill in the data*/
typedef struct module{
    VEC* func_list;
    VEC* static_stor_symbols;
    VEC* reg_list;
    VEC* bind_reg_list;
}IR_MODULE;
typedef struct function{
    char* func_name;
    VEC* register_list;
    IR_MODULE* IR_module;
    LIST_NODE* BB_list;
    SYM_ITEM* symbol;
}IR_FUNC;
typedef struct basic_block{
    LIST_NODE node;
    SYM_ITEM* symbol;
    IR_MODULE* IR_module;
    IR_FUNC* func;
    LIST_NODE* Instruction_list;
    bool ignore;
}IR_BB;

IR_FUNC* add_new_func(IR_MODULE* irm,SYM_ITEM* func_symbol);
IR_BB* add_new_bb(IR_FUNC* func,char* label_prefix,bool need_gen_label,SYM* symbol_table);
IR_INS* add_new_ins(IR_BB* bb);

bool insert_ins_to_bb(IR_INS* ins,IR_BB* ir_bb);

void print_IR(IR_MODULE* irm);
void print_INS(IR_INS* ins,size_t indentation);
void print_OPERAND(SYM_ITEM* operand,size_t indentation);
void print_static_stor_value(STOR_VALUE* value);


bool type_cast_trans(SYM_ITEM* dst,SYM_ITEM* src);
#endif