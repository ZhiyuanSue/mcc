#ifndef _IR_H_
#define _IR_H_
#include "../tools/vector.h"
#include "../tools/symbol_table.h"
#define IR_DATA_TYPE \
    int ir_d_type; \
    int ir_d_len; 
    /* type: 0-no,1-signed (i),2-unsigned (u),3-float (f)*/
/*operand*/
enum operand_type{
    OPERAND_REG, /*the operand is a register*/
    OPERAND_IMM, /*immediate*/
    OPERAND_LABEL, /*label position*/
    OPERAND_FUNC, /*function position*/
    OPERAND_OBJ, /*an object*/
};
enum operand_flow{
    FLOW_IN,
    FLOW_OUT,
};
typedef struct function IR_FUNC;
typedef struct basic_block IR_BB;
typedef struct instruction IR_INS;
typedef struct {
    enum operand_type type;
    enum operand_flow direction;
    union{
        struct {
            size_t reg_id;
            IR_INS* op;
        } reg;
        struct {
            IR_DATA_TYPE
        } imm;
        struct {
            IR_BB* op;
        } label;
        struct {
            IR_FUNC* function;
        } func;
        struct {
            size_t start_of_stack_deepth;
            size_t bytes;
            void* initializer_value;
        } obj;
    } operand_data;
}IR_OPERAND;

/*instruction*/
enum ins_op
{
    OP_NONE,
    OP_PHI,
};
typedef struct instruction{
    enum ins_op op;
    VEC* operands;
    IR_FUNC* func;
    IR_BB* block;
}IR_INS;
IR_INS* GenINS();


typedef struct {
    VEC* func_list;
    VEC* external_symbols;
}IR_MODULE;
typedef struct function{
    char* func_name;
    VEC* register_list;
    VEC* BB_list;
}IR_FUNC;
typedef struct basic_block{
    VEC* Instruction_list;
}IR_BB;

void print_IR(IR_MODULE* irm);
#endif