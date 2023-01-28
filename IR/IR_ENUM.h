/*This file is used to store the enum types in IR, for the scalar of this is large*/
#ifndef _IR_ENUM_H_
#define _IR_ENUM_H_
#define OP_STRING_LEN 16
#define OP_NUM 256
#define OPERAND_STR_LEN 16
#define OPERAND_NUM 16
enum data_type{
    DATA_NONE,
    DATA_INTEGER,
    DATA_FLOAT,
    DATA_POINTER_INTEGER,   /*if a pointer to a pointer ,also use int*/
    DATA_POINTER_FLOAT,
    DATA_POINTER_INTEGER_ARRAY,
    DATA_POINTER_FLOAT_ARRAY,
    DATA_POINTER_OTHER_ARRAY,   /*include array ,struct/union*/
    DATA_POINTER_STRUCT_UNION
};
enum operand_type{
    OPERAND_NONE,
    OPERAND_DATA,
    OPERAND_IMM,
    OPERAND_CODE,
    OPERAND_REG,
};
enum data_storage_type{
    IR_STOR_NONE,
    IR_STOR_STATIC,
    IR_STOR_THREAD,
    IR_STOR_STACK,
};
enum operand_flow{
    FLOW_IN,
    FLOW_OUT,
};
/*instruction operation type*/
enum ins_op
{
    OP_NONE,
    OP_RET,OP_BR,OP_CALL,OP_RECV,

    OP_ASS,OP_FADD,OP_SUB,OP_FSUB,
    OP_MUL,OP_FMUL,OP_UDIV,OP_SDIV,
    OP_FDIV,OP_UREM,OP_SREM,OP_FREM,

    OP_SHL,OP_SHR,OP_SAL,OP_SAR,
    OP_ROL,OP_ROR,OP_RCL,OP_RCR,

    OP_AND,OP_OR,OP_XOR,OP_NOT,

    OP_ALLOCA,OP_LOAD,OP_STORE,

    OP_TRUNC,OP_ZEXT,OP_SEXT,OP_FTRUNC,
    OP_FEXT,OP_FTOUI,OP_FTOSI,OP_UITOF,
    OP_SITOF,

    OP_ICMP,OP_FCMP,OP_PHI
};
enum OP_CMP_RES{
    RES_NONE,
};
enum IR_OP_ICMP_COND{
    ICMP_NONE,
    ICMP_EQ,ICMP_NE,
    ICMP_UGT,ICMP_UGE,ICMP_ULT,ICMP_ULE,
    ICMP_SGT,ICMP_SGE,ICMP_SLT,ICMP_SLE,
};
enum IR_OP_FCMP_COND{
    FCMP_NONE
};

#endif