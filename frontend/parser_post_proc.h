#ifndef _PARSER_POST_PROC_H_
#define _PARSER_POST_PROC_H_
#include "../defs/defs.h"
#include "../tools/vector.h"
#include "../tools/mcc_error.h"
#include "../tools/token.h"
#include "../tools/symbol_table.h"
#include "../tools/namespace.h"
#include "../tools/MEM.h"
#include "../tools/EBNF.h"
#include "ast.h"
void post_proc_init();
AST_BASE* post_processing(AST_BASE* curr_ast);
AST_BASE* declarator_node(AST_BASE* declarator_ast);
bool is_type_def_name(char* symbol,SYM* curr_sym_table);
#endif