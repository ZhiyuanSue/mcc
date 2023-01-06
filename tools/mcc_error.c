#include "mcc_error.h"
void m_error(char filename[],char function[],char reason[]){
    printf("mcc:ERROR\n\tIn file :%s,in function :%s\n\tERROR reason:%s\n\tArgument:\n",filename,function,reason);
}
void m_error_arg_char(char* arg){
    printf("\t%s",arg);
}
void m_error_arg_int(int arg){
    printf("\t%d",arg);
}
void m_test(char test[]){
    printf("\t\t========================\n");
    printf("\t\tmcc:TEST %s\n",test);
    printf("\t\t========================\n");
}
void m_test_item(char test_item_name[]){
    printf("%s:\n",test_item_name);
}
void m_test_arg_char(char* item_arg){
    printf("\t%s\n",item_arg);
}
void m_test_arg_int(int item_arg){
    printf("\t%d\n",item_arg);
}

const char error_describe[ERROR_NUMBER][ERROR_DES_LEN]={
    "C0001:",
    "C0002:Unknown error",
    "C0003:Invalid lexical element",
    "C0004:Unexpected namespace",
    "C0005:Redefined identifier",
    "C0006:Token used not defined",
    "C0007:Unexpected token(Attention:the error might happened at the later place)",
    /*semantic check*/
    
    "C0008:Too many storage class specifiers",
    "C0009:The _Thread_Local_ keyword must after keywork static or extern",
    "C0010:At least one type specifier is needed",
    "C0011:The type specifier have an error, unknown type",
    "C0012:The complex type is not support in this complier,sorry for that",
    "C0013:The _Atomic type is not support in this complier,sorry for that",
    "C0014:The type name in an atomic type specifier shall not refer to an array type, a function type,an atomic type, or a qualified type.",
    
    "C0015:Redeclaration with no linkage or redefinition of typedef with variably modify type",
    "C0016:The identifier undeclaration here",
    "C0017:This typedef name has overlaped with another type",
    "C0018:A struct cannot have a member with incomplete or function type",
    "C0019:Illegal constant expression:Constant expressions shall not contain assignment, increment, decrement, function-call,or comma operators, except when they are contained within a subexpression that is not evaluated",
    "C0020:Integer constant is too large for its type",
    "C0021:Error static assert declaration",
    "C0022:Bit field has invalid type",
    "C0023:Bit field has a width out of its type",
    "C0024:Bit field need an integer const width",
    "C0025:Bit field has a negative width",
    "C0026:Bit field with width zero should not have a declarator",
    "C0027:Enumerator value is not an integer value",
    "C0028:Enumerator value is overflow",
    "C0029:Extern declaration follows declaration with no linkage",
    "C0030:Declaration with no linkage follows extern declaration",
    "C0031:Confict types of the object or function",
    "C0032:Confict linkage of the same identifier(Tips: an extern follows a static declaration might be right in C++)",
    "C0033:Confict thread local declaration",
    "C0034:'[*]' or '[]' not allowed in other than function prototype scope",
    "C0035:Static or type qualifiers in [] must be an outmost one",
    "C0036:Static or type qualifiers in non-parameter array declarator",
    "C0037:A varibly length array should not have storage class type 'static' or '_Thread_local'",
    "C0038:Size of array has non-integer type",
    "C0039:The size of the array is defined equal or less than 0.(Tips1:0 is legal in gcc.Tips2:use '[]' to build a flexible array type)",
    "C0040:A declaration of array of functions or incomplete type",
    "C0041:An identifier with varibly modify type should be an ordinary identifier ,no linkage ,and block or prototype scope",
    "C0042:An identifier list in a function declarator that is not part of a definition of that function shall be empty",
    "C0043:A function declared with a return type of function or array",
    "C0044:A identifier in a function definition should have a function type",
    "C0045:A parameter of function has incomplete type",
    "C0046:Void must be the only parameter",
    "C0047:Only register is allowed in function parameter's storage class specifier",
    "C0048:This array operand must be a pointer to complete object",
    "C0049:The operands of a postfix array must be a pointer to type and a integer",
    "C0050:The left operand must be a function",
    "C0051:Lvalue is required",
    "C0052:The type of the operand is wrong",
    "C0053:Must be a struct/union and it's member",
    "C0054:Must be a pointer to struct/union and it's member",
    "C0055:The postfix operator ++/-- expect atomic, qualified,or unqualified real or pointer type, and shall be a modifiable lvalue.",
    "C0056:Here need a modifiable lvalue",
    "C0057:This operand cannot be a bit-field",
    "C0058:This operand cannot be a register lvalue",
    "C0059:This operand must be a pointer type",
    "C0060:Here need an integer type",
    "C0061:Here need an arithmetic type",
    "C0062:Here need a scalar type",
    "C0063:Here need a real type",
    "C0064:The sizeof or Alignof should not apply on an incomplete type",
    "C0065:A sizeof or Alignof applied on a function is not allowed",
    "C0066:The cast between floating type and pointer type is not allowed",
    "C0067:The additive operantor need both are arthematic types or one have a pointer to complete type and another have an integer",
    "C0068:The minus operator have an error operand type",
    "C0069:Here need two types are compatible",
    "C0070:Equal expr have illegal operands",
    "C0071:Condition expr have illegal operand typep",
    "C0072:Assignment expr have illegal operand type",
    "C0073:Parameter number is wrong,please check",
    "C0074:The parameter type is not compatible with that defined in prototype",
    "C0075:The for statement should only have a declaration with storage class 'auto' or 'register'",
    "C0076:A case or default label should in switch statement",
    "C0077:There's a same case in this switch statement",
    "C0078:This statement should only in a loop statement",
    "C0079:This statement should only in a loop or switch statement",
    "C0080:A function with a void return type should not return an expression",
    "C0081:A function with a return type should not have a return statement without expression",
    "C0082:This function definition is not an old style funciton ,but it have declaration list",
    "C0083:This parameter should not be initilized",
    "C0084:If a function have storage class specifier, it must be external or static",
    "C0085:This old style function have a declaration not in identifier list",
    "C0086:One of the identifier in identifier list is not declarated",
    "C0087:The old style function parameter shall contain no storage-class specifier other than register",
    "C0088:The size of alignment specified should be a positive power of 2",
    "C0089:An alignment attribute shall not be specified in a declaration of a typedef, or a bit-field, or a function, or a parameter, or an object declared with the register storage-class specifier.",
    "C0090:Two declaration or definition with incompatible alignment",
    "C0091:The initialization of an identifier need complete type or an array with unknown length",
    "C0092:No initializer shall attempt to provide a value for an object not contained within the entity being initialized.",
    "C0093:a designator has the form '[ constant-expression ]',but not a array type or not a nonnegative const integer expression",
    "C0094:a designator has the form '.identifier',but not a struct or union type or one member of current struct or union",
    /*warning*/
    "W0001:",
    "W0002:Unknown warning",
    "W0003:A declaration must declare a declarator,a tag,or an enumerator",
    "W0004:Duplicate use of qualifier keyword",
    "W0005:Implicit function decleration",
    "W0006:A function declarated '_Noreturn' with a return statement"
};
void mcc_error(ERROR_ITEM* e,char* file_buf,LINE* line)
{   
    if(e->ce<W0001_WARN_NONE)
        printf("Compile Error:(file %s: line %d: token %s )\n",e->filename,e->error_line,e->error_token_value);
    else
        printf("Compile Warning:(file %s: line %d: token %s )\n",e->filename,e->error_line,e->error_token_value);
    char buf[BUFSIZE];
    memcpy(buf,file_buf+line->start,line->end-line->start);
    buf[line->end-line->start]='\0';
    printf("%s\nreason:\t%s\n",buf,error_describe[e->ce]);
}