#include "IR_ENUM.h"
char operand_name_string[OPERAND_NUM][OPERAND_STR_LEN]=
{
    "OPERAND_NONE",
    "OPERAND_VOID",
    "OPERAND_REG",
    "OPERAND_IMM",
    "OPERAND_POINTER",
    "OPERAND_LABEL",
    "OPERAND_FUNC",
    "OPERAND_VECTOR",
    "OPERAND_OBJ", 
};
char op_name_string[OP_NUM][OP_STRING_LEN]=
{
    "OP_NONE",
    "OP_RET","OP_BR","OP_CALL","OP_RECV",

    "OP_ASS","OP_FADD","OP_SUB","OP_FSUB",
    "OP_MUL","OP_FMUL","OP_UDIV","OP_SDIV",
    "OP_FDIV","OP_UREM","OP_SREM","OP_FREM",

    "OP_SHL","OP_SHR","OP_SAL","OP_SAR",
    "OP_ROL","OP_ROR","OP_RCL","OP_RCR",

    "OP_AND","OP_OR","OP_XOR","OP_NOT",

    "OP_ALLOCA","OP_LOAD","OP_STORE",

    "OP_TRUNC","OP_ZEXT","OP_SEXT","OP_FTRUNC",
    "OP_FEXT","OP_FTOUI","OP_FTOSI","OP_UITOF",
    "OP_SITOF",

    "OP_ICMP","OP_FCMP","OP_PHI"
};