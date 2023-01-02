#ifndef _MCC_ERROR_H_
#define _MCC_ERROR_H_
#define ERROR_NUMBER 2048
#define ERROR_DES_LEN 2048
#include "../defs/defs.h"
#define C_ERROR(err_type,err_node) VECinsert(c_error,m_error_item( \
            tei, \
            err_type, \
            filename, \
            err_node->token->line, \
            err_node->token->value))
#define C_WARN(err_type,err_node) VECinsert(c_error,m_error_item( \
            twi, \
            err_type, \
            filename, \
            err_node->token->line, \
            err_node->token->value))
enum compiler_error{
    C0001_ERR_NONE,C0002_ERR_UNKNOWN,
    /*lex*/
    C0003_ERR_LEX_ERROR,
    /*parser*/
    C0004_ERR_NAMESPACE,
    C0005_ERR_REDEFINE,
    C0006_ERR_NOT_IN_SYMBOL_TABLE,
    C0007_ERR_UNEXPECTED_TOKEN,
    /*semantic*/
    
    C0008_ERR_TOO_MANY_STOR_SPEC,
    C0009_ERR_THREAD_LOCAL_PREFIX,
    C0010_ERR_NEED_MORE_TYPE_SPEC,
    C0011_ERR_WRONG_TYPE_SPEC,
    C0012_ERR_COMPLEX_NOT_SUPPORT,
    C0013_ERR_ATOMIC_UNSUPPORT,
    C0014_ERR_ATOMIC_TYPE_NAME,
    
    C0015_ERR_REDEFINATION,
    C0016_ERR_UNDECLARATION,
    C0017_ERR_TYPEDEF_OVERLAP,
    C0018_ERR_STRUCT_INCOMPLETE_FUNCTION_TYPE,
    C0019_ERR_ILLEGAL_CONST_EXPR,
    C0020_ERR_CONST_EXPR_RANGE,
    C0021_ERR_STATIC_ASSERT,
    

    C0022_ERR_BIT_FIELD_TYPE,
    C0023_ERR_BIT_FIELD_WIDTH_EXCEED,
    C0024_ERR_BIT_FIELD_WIDTH_INT,
    C0025_ERR_BIT_FIELD_WIDTH_NEGATIVE,
    C0026_ERR_BIT_FIELD_ZERO_WITH_DECLERATOR,

    C0027_ERR_ENUM_NOT_INT,
    C0028_ERR_ENUM_RANGE,

    C0029_ERR_LKA_EXTERN_FOLLOW_NO,
    C0030_ERR_LKA_NO_FOLLOW_EXTERN,
    C0031_ERR_CONFICT_TYPE,
    C0032_ERR_CONFICT_LINKAGE,
    C0033_ERR_CONFICT_THREAD_LOCAL,

    C0034_ERR_ARR_STAR_SCOPE,
    C0035_ERR_ARR_QUAL_STATIC_OUTMOST,
    C0036_ERR_ARR_QUAL_STATIC_IN_PARA,
    C0037_ERR_ARR_STATIC_THREAD_WITH_VLA,
    C0038_ERR_ARR_SIZE_INTEGER,
    C0039_ERR_ARR_SIZE_ZERO,
    C0040_ERR_ARR_FUNCTIONS_INCOMPLETE,
    C0041_ERR_VM_ID,

    C0042_ERR_FUN_IDENTIFIER_LIST,
    C0043_ERR_FUN_RETURN_TYPE,
    C0044_ERR_FUNC_DEF_DECL_TYPE,
    C0045_ERR_FUNC_PARA_INCOMPLETE_TYPE,
    C0046_ERR_FUNC_VOID_MORE_THAN_ONE,
    C0047_ERR_FUNC_PARA_STORAGE,

    C0048_ERR_LVALUE_ARR_TYPE_COMPLETE,
    C0049_ERR_LVALUE_ARR_TYPE_WRONG,
    C0050_ERR_LVALUE_FUNC_TYPE_WRONG,
    C0051_ERR_LVALUE_REQUIRED,

    C0052_ERR_EXPR_TYPE_ERROR,
    C0053_ERR_EXPR_STRUCT_UNION_DOT_TYPE_WRONG,
    C0054_ERR_EXPR_STRUCT_UNION_POINT_TYPE_WRONG,
    C0055_ERR_EXPR_POSTFIX_DOUBLE_P_M_TYPE,
    C0056_ERR_EXPR_MODIFIABLE_LVALUE,

    C0057_ERR_OPERAND_BIT_FIELD,
    C0058_ERR_OPERAND_REGISTER,
    C0059_ERR_OPERAND_SHOULD_POINTER,

    C0060_ERR_OPERAND_INTEGER_TYPE,
    C0061_ERR_OPERAND_ARTHMATIC_TYPE,
    C0062_ERR_OPERAND_SCALAR_TYPE,
    C0063_ERR_OPERAND_REAL_TYPE,

    C0064_ERR_SIZEOF_INCOMPLETE,
    C0065_ERR_SIZEOF_FUNCTION,

    C0066_ERR_CAST_CONVERT_POINT_FLOAT,
    C0067_ERR_ADD_OPERAND_TYPE,
    C0068_ERR_MINUS_OPERAND_TYPE,
    C0069_ERR_NEED_COMPATIBLE_TYPE,
    C0070_ERR_EQUAL_OPERAND,
    C0071_ERR_CONDITION_OPERAND,
    C0072_ERR_ASSIGN_OPERAND,

    C0073_ERR_FUNC_MORE_PARAMETERS,
    C0074_ERR_FUNC_PARA_TYPE,

    C0075_ERR_FOR_DECLARATION_STOR,
    C0076_ERR_IN_SWITCH,
    C0077_ERR_SWITCH_SAME_CASE,
    C0078_ERR_IN_LOOP,
    C0079_ERR_IN_SWITCH_LOOP,
    C0080_ERR_FUNC_RET_VOID,
    C0081_ERR_FUNC_RET_NONE_VOID,
    C0082_ERR_FUNC_NO_ID_LIST_BUT_DEC,
    C0083_ERR_FUNC_PARA_INITILIZATION,
    C0084_ERR_FUNC_STORAGE,
    C0085_ERR_FUNC_PARA_OLD_NOT_FIND,
    C0086_ERR_FUNC_PARA_OLD_NOT_DEC,
    C0087_ERR_FUNC_PARA_OLD_STOR,
    C0088_ERR_ALIGN_TWO_POWER,
    C0089_ERR_ALIGN_CANNOT_SPECIFIE,
    C0090_ERR_ALIGN_DIFFERENT_DECLARATION,
    C0091_ERR_INIT_TYPE_COMPLETE,
    /*warning*/
    W0001_WARN_NONE,W0002_WARN_UNKNOWN,
    W0003_WARN_NO_DECLARATION_ANY_THING,
    W0004_WARN_DUPLICATE_QUAL,
    W0005_WARN_IMPLICIT_DEC,
    W0006_WARN_NO_RETURN,
};
typedef unsigned int LINE_NUM;
typedef struct line{
    LINE_NUM line_number;
    size_t start;
    size_t end;
}LINE;
typedef struct mcc_error_item{
    enum compiler_error ce;
    char* filename;
    LINE_NUM error_line;
    char* error_token_value;
}ERROR_ITEM;

void m_error(char filename[],char function[],char reason[]);
void m_error_arg_char(char* arg);
void m_error_arg_int(int arg);

void m_test(char test[]);
void m_test_item(char test_item_name[]);
void m_test_arg_char(char* item_arg);
void m_test_arg_int(int item_arg);

void mcc_error(ERROR_ITEM* e,char* file_buf,LINE* line);
inline void* m_error_item(
    ERROR_ITEM* tei,
    enum compiler_error ce,
    char* filename,
    LINE_NUM error_line,
    char* error_token_value)
{
    tei->ce=ce; 
    tei->filename=filename;
    tei->error_line=error_line;
    tei->error_token_value=error_token_value;
    return (void*)tei;
}
#endif