#include "transdecl.h"
bool decl_dispatch(AST_BASE* ast_node,IR_BB* ir_bb,bool external_decl)
{   /*whether one symbol is global or not depends on the storage and linkage of that symbol*/

}
bool global_decl(AST_BASE* ast_node,IR_MODULE* irm)
{

}
bool local_decl(AST_BASE* ast_node,IR_BB* ir_bb){
    if(!ast_node||!ir_bb||ast_node->type!=declaration)
        goto error;
    return true;
error:
    return false;
}