#include "x86_backend.h"
static size_t str_len(const char* str)
{
    if(str==NULL){
        return 0;
    }
    const char* start=str;
    while(*str!='\0'){
        str++;
    }
    return str-start;
}
VEC* x86_ins_gen(int bit_len)
{
    VEC* res=InitVEC(DEFAULT_CAPICITY);
    struct x86_ins_raw* _ins_raw;
    /*
        judge 32 or 64    
    */
    if(bit_len==32){
        _ins_raw=x86_32_ins_raw;
    }
    else if(bit_len==64){
        _ins_raw=x86_64_ins_raw;
    }
    else{
        goto error;
    }
    int i=0;
    while(str_len(_ins_raw[i].mnemonic)>0){
        struct x86_ins_raw tmp=_ins_raw[i];
        /*
            consider instruction extern:sse,mmx,vmx etc. support
        */
        if(str_len(tmp.iext)>0){

        }
    }
error:
    printf("Error: instruction generator fail\n");
    DelVEC(res);
    return NULL;
}