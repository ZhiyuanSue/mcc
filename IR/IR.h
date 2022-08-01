#ifndef _IR_H_
#define _IR_H_
#include "../tools/vector.h"
#include "INS.h"

typedef struct {
    VEC* func_list;
}IR_MODULE;
typedef struct {
    VEC* BB_list;
}IR_FUNC;
typedef struct {
    VEC* Instruction_list;
}IR_BB;

void print_IR(IR_MODULE* irm);
#endif