#ifndef _TRANS_H_
#define _TRANS_H_
#include "IR.h"
#include "../frontend/ast.h"
/*The following trans is used to translate the AST to the IR*/
IR_MODULE* trans_to_IR(AST_BASE* ast_node);
#endif