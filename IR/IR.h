#ifndef _IR_H_
#define _IR_H_
#include "../tools/vector.h"
#include "../tools/list.h"
#include "../tools/symbol_table.h"
#include "IR_ENUM.h"
typedef struct module IR_MODULE;
typedef struct function IR_FUNC;
typedef struct basic_block IR_BB;
typedef struct instruction IR_INS;
typedef struct operand IR_OPERAND;
typedef struct IR_OPERAND_REG IR_REG;
struct IR_OPERAND_REG{
    enum data_type d_type;
    size_t reg_id;
    union{
        size_t data_length;
        struct{
            enum data_storage_type pointer_data_stor_type;
            size_t data_length;
            signed long int off;    /*the offset from */
        }pointer_attr;
    }reg_attr;
};
typedef struct operand{
    enum operand_type type;
    union{
        struct IR_OPERAND_DATA{
            enum data_storage_type data_stor_type;
            size_t data_length;
            size_t data_align;
        }operand_data_type;
        struct IR_OPERAND_IMM{
            signed long int imm_data;
        }operand_imm_type;
        struct IR_OPERAND_CODE{
            IR_BB* code_position;   /*it must point to a label position*/
        }operand_code_type;
        IR_REG operand_reg_type;
    }operand_data;
}IR_OPERAND;

typedef struct instruction{
    LIST_NODE node;
    enum ins_op op;
    IR_OPERAND* dst;
    IR_OPERAND* src1;
    IR_OPERAND* src2;
    IR_MODULE* IR_module;
    IR_FUNC* func;
    IR_BB* block;
}IR_INS;
bool GenINS(IR_INS* ins,
    enum ins_op op,
    IR_OPERAND* dst,
    IR_OPERAND* src1,
    IR_OPERAND* src2);   /*use add_new_ins to get a new one and fill in the data*/

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
}IR_FUNC;
typedef struct basic_block{
    LIST_NODE node;
    char* bb_label;
    IR_MODULE* IR_module;
    IR_FUNC* func;
    LIST_NODE* Instruction_list;
}IR_BB;

IR_FUNC* add_new_func(IR_MODULE* irm);
IR_BB* add_new_bb(IR_FUNC* func);
IR_INS* add_new_ins(IR_BB* bb);


char* label_allocator(void);    /*allocate a label name for a bb*/
IR_REG* reg_allocator(IR_MODULE* irm,enum data_type type,VEC* reg_vec);

void print_IR(IR_MODULE* irm);
void print_INS(IR_INS* ins,size_t indentation);
#endif