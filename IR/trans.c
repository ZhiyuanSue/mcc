#include "trans.h"

IR_MODULE* trans_to_IR(AST_BASE* ast_node)
{
    if(!ast_node||ast_node->type!=translation_unit)
        goto error;
    IR_MODULE* res=m_alloc(sizeof(IR_MODULE));
    res->external_symbols=InitVEC(DEFAULT_CAPICITY);
    res->func_list=InitVEC(DEFAULT_CAPICITY);
    
    return res;
error:
    return NULL;
}