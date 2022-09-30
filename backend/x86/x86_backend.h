#include "../../defs/defs.h"
#define MAX_INS_RAW_NUM 1200
struct x86_register{

};
struct x86_ins{

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

