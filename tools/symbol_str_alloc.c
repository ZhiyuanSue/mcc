#include "symbol_str_alloc.h"
#include "../defs/defs.h"
#include "MEM.h"
#define SYM_STR_LEN 64 /*consider the translation limitation*/
char tmp_sym_alloc[SYM_STR_LEN];
void str_allocator_init()
{
    for(size_t i=0;i<SYM_STR_LEN;++i){
        tmp_sym_alloc[i]='0';
    }
}
char char_add(char tmpc,bool* carry ){
    if(tmpc>='0'&&tmpc<='8')
        return (tmpc+1);
    else if(tmpc=='9')
        return 'A';
    else if(tmpc>='A'&&tmpc<='Y')
        return (tmpc+1);
    else if(tmpc=='Z')
        return 'a';
    else if(tmpc>='a'&&tmpc<='y')
        return (tmpc+1);
    else if(tmpc=='z')
    {
        (*carry)=true;
        return '0';
    }
    else
        return '0';
}
char* tmp_symbol_str_alloc(char* prefix)
{
    char* res=m_alloc(sizeof(char)*(SYM_STR_LEN+1));
    int i;
    for(i=SYM_STR_LEN-1;i>=0;i--){
        bool carry=false;
        tmp_sym_alloc[i]=char_add(tmp_sym_alloc[i],&carry);
        if(!carry)
            break;
        if(i==0&&carry)
        {
            printf("symbol string allocator overflow\n");
            return NULL;
        }
    }
    for(i=0;i<strlen(prefix)&&i<SYM_STR_LEN;++i)
    {
        res[i]=prefix[i];
    }
    for(;i<SYM_STR_LEN;++i){
        res[i]=tmp_sym_alloc[i];
    }
    res[SYM_STR_LEN]='\0';
    return res;
}