#ifndef _TRANS_H_
#define _TRANS_H_
#include "IR.h"
#include "../frontend/ast.h"
#include "../tools/m_type_vec.h"
extern VEC* c_error;
extern char* filename;
/*The following trans is used to translate the AST to the IR*/
IR_MODULE* trans_to_IR(AST_BASE* ast_node);
bool trans_func(AST_BASE* ast_node,IR_FUNC* ir_func);

/*decl*/
bool declaration_trans(AST_BASE* ast_node,IR_MODULE* irm,IR_FUNC* ir_func,IR_BB* ir_bb);
bool fill_in_static_stor_value(AST_BASE* initializer_node,STATIC_STOR_VALUE* value);
/*the global declaration is in module,and the local declaration must in one block*/

/*stmt*/
bool stmt_trans_dispatch(AST_BASE* ast_node,IR_BB* ir_bb);
bool labeled_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb);
bool compound_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb);
bool expr_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb);
bool if_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb);
bool switch_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb);
bool while_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb);
bool do_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb);
bool for_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb);
bool goto_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb);
bool continue_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb);
bool break_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb);
bool return_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb);
bool asm_stmt_trans(AST_BASE* ast_node,IR_BB* ir_bb);

/*expr*/
bool expr_trans_dispatch(AST_BASE* ast_node,IR_BB* ir_bb);
bool cond_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb);
bool expr_trans(AST_BASE* ast_node,IR_BB* ir_bb);
bool assign_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb);
bool logical_or_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb);
bool logical_and_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb);
bool bit_inclusive_or_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb);
bool bit_exclusive_or_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb);
bool and_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb);
bool equal_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb);
bool relation_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb);
bool shift_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb);
bool add_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb);
bool mul_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb);
bool cast_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb);
bool unary_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb);
bool postfix_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb);
bool pri_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb);

#endif