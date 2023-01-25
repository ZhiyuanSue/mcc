#ifndef _DECL_SEMANTICS_H_
#define _DECL_SEMANTICS_H_

#include "../frontend/semantics.h"
#include "../frontend/expr_semantics.h"
#define TYPE_SPEC_CASE_NUM 38
bool initializer_search(
    AST_BASE* initializer_node,
    VEC* type_vec,
    size_t* off,
    size_t curr_obj_off,
    bool bit_field,
    size_t bit_field_size,
    size_t level);
#endif   