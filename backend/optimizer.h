#ifndef _OPTIMIZER_H_
#define _OPTIMIZER_H_
#include "../defs/defs.h"
#include "../IR/IR.h"

bool optimizer(IR_MODULE* irm);
bool reg_alloc(IR_MODULE* irm);
#endif