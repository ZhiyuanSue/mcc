#ifndef _BACKEND_H_
#define _BACKEND_H_
#include "../defs/defs.h"
#include "../IR/IR.h"
#include "optimizer.h"
#ifdef _ARM_
#include "arm/arm_backend.h"
#endif
#ifdef _RISC_V_
#include "riscv/riscv_backend.h"
#endif
#ifdef _X86_
#include "x86/x86_backend.h"
#endif
bool MCC_backend(IR_MODULE* irm,char* src_file,char* target_file);

#endif