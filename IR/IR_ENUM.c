#include "IR_ENUM.h"
char operand_name_string[OPERAND_NUM][OPERAND_STR_LEN]=
{
    "NONE",
    "DATA",
    "IMM",
    "CODE",
    "REG",
};
char data_type_name_string[OPERAND_NUM][OPERAND_STR_LEN]=
{
    "NONE",
    "INTEGER",
    "FLOAT",
    "COMPLEX",
    "POINT_INTEGER",
    "POINT_FLOAT",
    "POINT_COMPLEX",
    "POINT_INT_ARR",
    "POINT_FLOAT_ARR",
    "POINT_OTHER_ARR",
    "POINT_SU",
    "POINT_FUNCTION",
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

    "OP_ICMP","OP_FCMP","OP_PHI","OP_MOV",
};

char ICMP_str[OP_NUM][OP_STRING_LEN]={
    "ICMP_NONE",
    "ICMP_EQ","ICMP_NE",
    "ICMP_UGT","ICMP_UGE","ICMP_ULT","ICMP_ULE",
    "ICMP_SGT","ICMP_SGE","ICMP_SLT","ICMP_SLE",
};
char FCMP_str[OP_NUM][OP_STRING_LEN]={
    "FCMP_NONE",
    "FCMP_FALSE","FCMP_TRUE",
    "FCMP_OEQ","FCMP_OGT","FCMP_OGE","FCMP_ONE","FCMP_ORD",
    "FCMP_UEQ","FCMP_UGT","FCMP_UGE","FCMP_ULT","FCMP_ULE","FCMP_UNE","FCMP_UNO"
};