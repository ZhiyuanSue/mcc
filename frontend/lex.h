#ifndef _LEX_H_
#define _LEX_H_
#include "../tools/mcc_error.h"
#include "preprocessor.h"
#include "../tools/EBNF.h"
#include "../tools/token.h"
#include "../defs/statement.h"
#include "../defs/rule.h"
VEC* lex(char* buf,VEC* svec,VEC* rvec,VEC* lines);
int l_match(char* buf,void* rule);
#endif