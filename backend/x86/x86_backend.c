#include "x86_backend.h"
bool gen_asm(IR_MODULE* irm,FILE* fp)
{
    if(!irm||!fp)
        goto error;
    for(size_t i=0;i<VECLEN(irm->func_list);++i)
    {
        if(!gen_function((IR_FUNC*)(VEC_GET_ITEM(irm->func_list,i)),fp))
            goto error;
    }
    /*generate the static storage data*/
    return true;
error:
    return false;
}
bool gen_function(IR_FUNC* func,FILE* fp)
{
    if(!func||!fp)
        goto error;
    fprintf(fp,"\t.globl\t%s\n",func->func_name);
#ifdef _UNIX_
    fprintf(fp,"\t.section\t%s @_function\n",func->func_name);
#endif
#ifdef _MAC_
    fprintf(fp,"\t.p2align\t4, 0x90\n");
#endif
    LIST_NODE* curr_bb=func->BB_list;
    if(curr_bb){
        fprintf(fp,"%s:\n",((IR_BB*)curr_bb)->symbol->value);
    /* the first one means the function,should not be ignore*/
    /*
        Here,something like cfi need take into consideration
    */
        curr_bb=curr_bb->next;
        while(curr_bb!=func->BB_list)
        {
            if(((IR_BB*)curr_bb)->ignore)
                fprintf(fp,"## %s:\n",((IR_BB*)curr_bb)->symbol->value);
            else
                fprintf(fp,"%s:\n",((IR_BB*)curr_bb)->symbol->value);
            curr_bb=curr_bb->next;
        }
    }
    return true;
error:
    return false;
}
bool gen_ins(IR_INS* ins,FILE* fp)
{
    if(!ins||!fp)
        goto error;
    return true;
error:
    return false;
}