#ifndef _TRANS_EXPR_H_
#define _TRANS_EXPR_H_
#include "trans.h"
#include "../frontend/expr_semantics.h"
bool assign_trans(SYM_ITEM* src_symbol,
    SYM_ITEM* dst_symbol,
    IR_BB* ir_bb);
#endif