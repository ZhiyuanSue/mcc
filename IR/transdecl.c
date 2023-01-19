#include "transdecl.h"
bool declaration_trans(AST_BASE* ast_node,IR_MODULE* irm,IR_FUNC* ir_func,IR_BB* ir_bb)
{
    if(!ast_node||!irm||ast_node->type!=declaration)
        goto error;
    return true;
error:
    return false;
}
bool static_storage_decl(AST_BASE* ast_node,IR_MODULE* irm)
{
    if(!ast_node||!irm||ast_node->type!=declaration)
        goto error;
    return true;
error:
    return false;
}
bool stack_decl(AST_BASE* ast_node,IR_BB* ir_bb){
    if(!ast_node||!ir_bb||ast_node->type!=declaration)
        goto error;
    return true;
error:
    return false;
}