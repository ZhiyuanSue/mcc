#ifndef _IR_H_
#define _IR_H_
#include "../tools/vector.h"
#include "../tools/list.h"
#include "../tools/symbol_table.h"
#include "IR_ENUM.h"
#define IR_DATA_TYPE \
    enum data_type ir_d_type; \
    int ir_d_len; 
typedef struct module IR_MODULE;
typedef struct function IR_FUNC;
typedef struct basic_block IR_BB;
typedef struct instruction IR_INS;
typedef struct operand IR_OPERAND;
typedef struct operand{
    enum operand_type type;
    enum operand_flow direction;
    union{
        struct {
            size_t reg_id;
            IR_INS* op;
        } operand_reg;
        struct {
            IR_DATA_TYPE
        } operand_imm;
        struct {
            IR_BB* op;
        } operand_label;
        struct {
            void* p;
        } operand_pointer;
        struct {
            IR_FUNC* function;
        } operand_func;
        struct {
            size_t start_off_sfp;      /*the start of array*/
            IR_OPERAND* array_element;
            bool is_vla;
            union{
                size_t length;
                void* length_assign_expr_node;
            };
        } operand_vector;
        struct {
            size_t start_off_sfp;
            size_t totol_bytes; /*it must use align,so use bytes,not bit*/
            void* initializer_value;
        } operand_obj;
    } operand_data;
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
bool GenINS(IR_INS* ins);   /*use add_new_ins to get a new one and fill in the data*/

typedef struct module{
    VEC* func_list;
    VEC* global_and_external_symbols;
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
void print_IR(IR_MODULE* irm);
#endif