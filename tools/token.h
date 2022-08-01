#ifndef _TOKEN_H_
#define _TOKEN_H_
#include "vector.h"
#include "../defs/defs.h"
#include "MEM.h"
#include "vector.h"
#include "EBNF.h"
#include "m_type.h"
#define is_digit(c) (c>='0'&&c<='9')
#define is_hex_digit(c) ((c>='0'&&c<='9')||(c>='A'&&c<='F')||(c>='a'&&c<='f'))
typedef char* TK_VALUE;
typedef struct token{
    enum rule_type_enum type;
    size_t value_len;
    TK_VALUE value;
    LINE_NUM line;
    void* symbol_item;  /*only when type==identifier then the symbol_item is not NULL*/
    VEC* parser_token_mem;
}TOKEN;
typedef VEC TOKEN_VEC;
void m_test_token(TOKEN* t);
/*token to value*/
CONST_VALUE* token_to_const_value(TOKEN* t);
bool int_const_to_const_value(TOKEN* t,CONST_VALUE* cv);
bool int_const_int(TOKEN* t,CONST_VALUE* cv);
bool int_const_uint(TOKEN* t,CONST_VALUE* cv);
bool int_const_l(TOKEN* t,CONST_VALUE* cv);
bool int_const_ul(TOKEN* t,CONST_VALUE* cv);
bool int_const_ll(TOKEN* t,CONST_VALUE* cv);
bool int_const_ull(TOKEN* t,CONST_VALUE* cv);
bool float_const_to_const_value(TOKEN* t,CONST_VALUE* cv);
bool char_const_to_const_value(TOKEN* t,CONST_VALUE* cv);
#endif