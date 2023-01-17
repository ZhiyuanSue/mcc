/*This file is used to store the enum types in IR, for the scalar of this is large*/
#ifndef _IR_ENUM_H_
#define _IR_ENUM_H_
enum data_type{
    DATA_NONE,
    DATA_SIGNED,
    DATA_UNSIGNED,
    DATA_FLOAT
};
enum operand_type{
    OPERAND_NONE,   /*no operand*/
    OPERAND_VOID,   /*void operand type*/
    OPERAND_REG, /*the operand is a register*/
    OPERAND_IMM, /*immediate*/
    OPERAND_POINTER,    /*pointer*/
    OPERAND_LABEL, /*label position*/
    OPERAND_FUNC, /*function position*/
    OPERAND_VECTOR, /*an array type,which might have special machine instructions to accelerate it*/
    OPERAND_OBJ, /*an object,including struct union or another complex type*/
};
enum operand_flow{
    FLOW_IN,
    FLOW_OUT,
};
/*instruction operation type*/
enum ins_op
{
    OP_NONE,
    OP_RET,
    OP_BR,
    OP_CALL,

    OP_ASS,
    OP_FADD,
    OP_SUB,
    OP_FSUB,
    OP_MUL,
    OP_FMUL,
    OP_UDIV,
    OP_SDIV,
    OP_FDIV,
    OP_UREM,
    OP_SREM,
    OP_FREM,

    OP_SHL,
    OP_SHR,
    OP_SAL,
    OP_SAR,
    OP_ROL,
    OP_ROR,
    OP_RCL,
    OP_RCR,

    OP_AND,
    OP_OR,
    OP_XOR,
    OP_NOT,

    OP_ALLOCA,
    OP_LOAD,
    OP_STORE,

    OP_TRUNC,
    OP_ZEXT,
    OP_SEXT,
    OP_FTRUNC,
    OP_FEXT,
    OP_FTOUI,
    OP_FTOSI,
    OP_UITOF,
    OP_SITOF,

    OP_ICMP,
    OP_FCMP,
    OP_PHI
};
enum OP_CMP_RES{
    RES_NONE,
};

#endif