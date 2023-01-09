#ifndef _PARSER_H_
#define _PARSER_H_
#include "../defs/defs.h"
#include "../tools/vector.h"
#include "../tools/mcc_error.h"
#include "../tools/token.h"
#include "../tools/symbol_table.h"
#include "../tools/namespace.h"
#include "../tools/MEM.h"
#include "../tools/EBNF.h"
#include "ast.h"
#include "parser_post_proc.h"
extern VEC* search_mem[NONTER_NUM][TERMINATOR_NUM+1];
AST_BASE* parser(VEC* token_list,VEC* pvec);
AST_BASE* p_match(VEC* token_list,
    void* rule,
    size_t token_index,
    SYM* curr_sym_table,
    bool build_sym_table,
    NMSP name_space);
bool p_symbol_table(AST_BASE* ast_node,SYM* father);
SYM* parser_creater_symbol_table(SYM* father,
    bool *have_creat_symbol_table,
    enum rule_type_enum rule_type,
    bool *build_sym_table);
NMSP parser_node_name_space(enum rule_type_enum rule_type,
    VEC* token_list,
    size_t token_index,
    NMSP name_space,
    SYM* curr_sym_table);

VEC* shorten_tree(VEC* v);
bool is_enum_const(char* symbol,SYM* curr_sym_table);
#endif