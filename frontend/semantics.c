#include "semantics.h"
extern VEC* c_error;
#ifdef _TEST_SEMANTICS_
extern int semantics_level;
#endif
bool semantics(AST_BASE* ast_node){
#ifdef _TEST_SEMANTICS_
    semantics_level=0;
#endif
    if(!ast_node)
        return false;
    if(ast_node->type==end_of_file)
        return true;
    switch (ast_node->type)
    {
    case declaration:{
        VEC* tmp_sym_item=InitVEC(DEFAULT_CAPICITY);
        return declaration_type(ast_node,tmp_sym_item);
        DelVEC(tmp_sym_item);
    }
    case function_definition:{
        return function_definition_type(ast_node);
    }
    default:
        break;
    }
    for(size_t i=0;i<AST_CHILD_NUM(ast_node);++i){
        AST_BASE* ast_child=AST_GET_CHILD(ast_node,i);
        if(!semantics(ast_child))
            return false;
    }
#ifdef _TEST_
#endif
    return true;
}