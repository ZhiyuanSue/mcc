#include "x86_backend.h"
bool gen_asm(IR_MODULE* irm,FILE* fp)
{   
    if(!irm||!fp)
        goto error;
    return true;
error:
    return false;
}
bool gen_function(IR_FUNC* func,FILE* fp)
{
    
}
bool gen_ins(IR_INS* ins,FILE* fp)
{

}