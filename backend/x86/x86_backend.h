#ifndef _X86_BACKEND_H_
#define _X86_BACKEND_H_
#include "../../defs/defs.h"
#include "../../tools/vector.h"
#define MAX_INS_RAW_NUM 1200
struct x86_register{
    char* reg_name;
};

struct x86_ins_raw{
    char pf[4];
    char _0F[4];
    char po[8];	
    char so[4];	
    char o[2];	
    char proc[8];	
    char st[8];	
    char m[2];
    char rl[4];
    char x[2];
    char mnemonic[16];	
    char op1[16];	
    char op2[16];
    char op3[16];
    char op4[16];
    char iext[8];
    char tested_f[16];
    char modif_f[16];
    char def_f[16];
    char undef_f[16];
    char f_values[16];	
    char description[128];
};

struct x86_ins{
    struct x86_ins_raw* raw;
    
};

extern struct x86_ins_raw x86_32_ins_raw[MAX_INS_RAW_NUM];
extern struct x86_ins_raw x86_64_ins_raw[MAX_INS_RAW_NUM];

VEC* x86_ins_gen(int bit_len);

#endif