#include "transdecl.h"
bool static_storage_decl(AST_BASE* ast_node,IR_MODULE* irm)
{
    return true;
}
bool stack_decl(AST_BASE* ast_node,IR_BB* ir_bb){
    if(!ast_node||!ir_bb||ast_node->type!=declaration)
        goto error;
    return true;
error:
    return false;
}