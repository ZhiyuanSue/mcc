#include "transdecl.h"
bool local_decl(AST_BASE* ast_node,IR_BB* ir_bb){
    if(!ast_node||!ir_bb||ast_node->type!=declaration)
        goto error;
    return true;
error:
    return false;
}