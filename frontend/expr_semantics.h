#ifndef _CONST_EXPR_
#define _CONST_EXPR_
#include "semantics.h"
#include "../tools/token.h"
#include "const_expr.h"
#define EXPR_NUM 17

void const_illegal_error(AST_BASE* ast_node);
void print_expr_type(AST_BASE* node);
bool assignment_type_check(VEC* unary_type_vec,VEC* assign_type_vec);
#endif