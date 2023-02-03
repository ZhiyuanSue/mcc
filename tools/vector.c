#include "vector.h"

VEC* InitVEC(size_t len)
{
    VEC* v=m_alloc(sizeof(VEC));
    v->data=m_alloc(len*sizeof(void*));
    v->capicity=len;
    v->len=0;
    return v;
}
void VECinsert(VEC* v,void* p)
{
    if(v==NULL) return;
    VECexpand(v);
    v->data[VECLEN(v)]=p;
    VECLEN(v)++;
}
void VECremove(VEC* v,size_t low,size_t high)   //remove [low,high)
{
    if(v==NULL) return;
    if(low==high)   return;
    while(high<VECLEN(v)){
        v->data[low++]=v->data[high++];
    }
    VECLEN(v)=low;
    VECshrink(v);
}
void VECpopback(VEC* v){
    if(v==NULL) return;
    if(VECLEN(v)==0) return;
    VECLEN(v)--;
}
void VECexpand(VEC* v)
{
    if(v==NULL) return;
    if(VECLEN(v)<v->capicity) return;
    if(v->capicity<DEFAULT_CAPICITY) v->capicity=DEFAULT_CAPICITY;
    void* old=v->data;
    v->data=m_alloc((v->capicity<<1)*sizeof(void*));
    memcpy(v->data,old,VECLEN(v)*sizeof(void*));
    v->capicity=v->capicity<<1;
    m_free(old);
}
void VECshrink(VEC* v)
{
    if(v==NULL) return;
    if(v->capicity<DEFAULT_CAPICITY<<1) return;
    if(VECLEN(v)<<2>v->capicity) return;
    void* old=v->data;
    v->data=m_alloc((v->capicity>>1)*sizeof(void*));
    memcpy(v->data,old,VECLEN(v)*sizeof(void*));
    v->capicity=v->capicity>>1;
    m_free(old);
}
void VECcpy(VEC* src,VEC** dst){
    if(src==NULL)
        return;
    (*dst)=InitVEC(src->capicity);
    for(size_t i=0;i<src->len;++i){
        (*dst)->data[i]=src->data[i];
    }
    (*dst)->len=src->len;
}
void VECswapItem(VEC* v,size_t i,size_t j)
{
    if(!v)
        return;
    void* p;
    p=v->data[i];
    v->data[i]=v->data[j];
    v->data[j]=p;
}
void DelVEC(VEC* v)
{
    if(v==NULL) return;
    m_free(v->data);
    m_free(v);
}