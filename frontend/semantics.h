#ifndef _SEMANTICS_H_
#define _SEMANTICS_H_
#include "ast.h"
#include "../tools/m_type.h"
#include "../tools/m_type_vec.h"
#include "../tools/mcc_error.h"
#include "../IR/IR.h"
#include "../tools/symbol_table.h"

bool semantics(AST_BASE* ast_node);

/*declaration*/
bool declaration_type(AST_BASE* ast_node,VEC* dec_symbol_item_list);
bool function_definition_type(AST_BASE* ast_node);
bool declarator_type(AST_BASE* declarator_node,
        VEC* type_vec,
        int* declarator_cnt,
        bool* typedef_declaration,
        NMSP name_space,
        char** declarator_char_name);
bool abs_declarator_type(AST_BASE* abstract_declarator_node,
        VEC* type_vec);
VEC* declaration_spec_qual_list_type(AST_BASE* dec_sq_list_node,
        int* declaration_cnt,
        bool* typedef_declaration,
        bool need_type_spec);
TP_SU* struct_union_type(AST_BASE* struct_decl_list_node,
        AST_BASE* tag_node,
        enum TP_CATEGORY struct_union);
TP_ENUMERA* enum_type(AST_BASE* enumerator_list_node,AST_BASE* tag_node);
TP_FUNC* function_type(VEC* tmp_node_vec);
TP_ARR* array_type(VEC* tmp_node_vec);
VEC* type_name_type(AST_BASE* type_name_node);
bool initializer_semantic(AST_BASE* initializer_node,VEC* target_type_vec,size_t off,size_t curr_obj_off);
bool initializer_list_semantic(AST_BASE* initializer_list_node,VEC* type_vec,size_t off,size_t curr_obj_off);
bool static_assert_type(AST_BASE* static_assert_declaration_node);
/*constant expr*/
bool const_value(AST_BASE* ast_node);
/*expr*/
bool expr_dispatch(AST_BASE* expr_node);
bool cond_expr_value(AST_BASE* ast_node);
bool expr_value(AST_BASE* ast_node);
bool assign_expr_value(AST_BASE* ast_node);
bool logical_or_expr_value(AST_BASE* ast_node);
bool logical_and_expr_value(AST_BASE* ast_node);
bool bit_inclusive_or_expr_value(AST_BASE* ast_node);
bool bit_exclusive_or_expr_value(AST_BASE* ast_node);
bool and_expr_value(AST_BASE* ast_node);
bool equal_expr_value(AST_BASE* ast_node);
bool relation_expr_value(AST_BASE* ast_node);
bool shift_expr_value(AST_BASE* ast_node);
bool add_expr_value(AST_BASE* ast_node);
bool mul_expr_value(AST_BASE* ast_node);
bool cast_expr_value(AST_BASE* ast_node);
bool unary_expr_value(AST_BASE* ast_node);
bool postfix_expr_value(AST_BASE* ast_node);
bool pri_expr_value(AST_BASE* ast_node);
/*statements*/
bool statement_dispatch(AST_BASE* ast_node);

bool labeled_statement(AST_BASE* ast_node);
bool compound_statement(AST_BASE* ast_node);
bool expr_statement(AST_BASE* ast_node);
bool if_statement(AST_BASE* ast_node);
bool switch_statement(AST_BASE* ast_node);
bool while_statement(AST_BASE* ast_node);
bool do_statement(AST_BASE* ast_node);
bool for_statement(AST_BASE* ast_node);
bool goto_statement(AST_BASE* ast_node);
bool continue_statement(AST_BASE* ast_node);
bool break_statement(AST_BASE* ast_node);
bool return_statement(AST_BASE* ast_node);
bool asm_statement(AST_BASE* ast_node);
#endif