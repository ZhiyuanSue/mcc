#ifndef _VECTOR_H_
#define _VECTOR_H_
#include "mcc_error.h"
#include "MEM.h"

typedef struct vector{
    void** data;
    size_t capicity;
    size_t len;
}VEC;
#define VECLEN(v) (v->len)
#define VEC_GET_ITEM(v,i) ((v!=NULL&&i<VECLEN(v))?(v->data[i]):NULL)
#define VEC_HEAD(v) (v&&v->len>0?v->data[0]:NULL)
#define VEC_BACK(v) (v&&v->len>0?v->data[v->len-1]:NULL)

VEC* InitVEC(size_t len);
void VECinsert(VEC* v,void* p);
void VECremove(VEC* v,size_t low,size_t high);
void VECpopback(VEC* v);
void VECexpand(VEC* v);
void VECshrink(VEC* v);
void VECcpy(VEC* src,VEC** dst);
inline void VECappend(VEC* src,VEC* dst){
    if(!src||!dst)
        return;
    for(size_t i=0;i<VECLEN(src);++i){
        VECinsert(dst,VEC_GET_ITEM(src,i));
    }
}
void DelVEC(VEC* v);
#endif