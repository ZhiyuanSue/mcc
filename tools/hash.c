#include "hash.h"
const unsigned long long int hash_capicity[HASH_CAP_NUM]={
    61,113,211,509,911,
    1627,2909,6901,12281,21841,
    38833,69061,122777,218357,388211,
    690163,1226959,2181271,3877817,6893911,
    12255871,21788233,38734667,68861641,1e9+7,
    217636919,386910137,687840301,1222827239};
HASH* InitHASH(void)
{
    HASH* tmph=(HASH*)m_alloc(sizeof(HASH));
    if(tmph==NULL)
        return NULL;
    tmph->data=m_alloc(sizeof(void*)*DEFAULT_CAP);
    m_memset(tmph->data,'\0',sizeof(void*)*DEFAULT_CAP);
    tmph->capicity=DEFAULT_CAP;
    tmph->capicity_index=tmph->record_num=0;
    return tmph;
}
bool HSAHExpand(HASH* h,bool (*cmp)(void* a,void* b))
{
    if(h==NULL)
        return false;
    void** old=h->data;
    unsigned long long int old_capicity=h->capicity;
    h->capicity_index++;
    if(h->capicity_index>=HASH_CAP_NUM){
        return false;
        /* I don't believe any normal program can see this, 12,2282,7239 is a very big number in one scope*/
    }
    h->capicity=hash_capicity[h->capicity_index];
    h->data=m_alloc(sizeof(void*)*(h->capicity));
    m_memset(h->data,'\0',sizeof(void*)*(h->capicity));
    h->record_num=0;
    for(unsigned long long int i=0;i<old_capicity;i++){
        if(old[i]!=NULL&&((HASH_ITEM*)old[i])->count>=0)
        {
            HASHInsert(h,(HASH_ITEM*)old[i],cmp);
        }
    }
    m_free(old);
    return true;
}
void* HASHInsert(HASH* h,HASH_ITEM* item,bool (*cmp)(void* a,void* b))
{
    unsigned long long int key=item->key;
    if(h==NULL)
        return NULL;
    if(item->count==0)     /*count=0 means a query,should not insert*/
        return (void*)item;
    HASH_ITEM* tmphi=HASHFind(h,(void*)item,cmp,false,true);
    if(tmphi!=NULL){
        m_free(item);
        return (void*)tmphi;
    }
    h->record_num++;
    if((h->record_num)*2>h->capicity)
    {
        bool res=HSAHExpand(h,cmp);
        if(res==false)
            return NULL;
    }
    unsigned long long int off=0;
    unsigned long long int insert_pos=HASHFunction(key,h);
    while(HASH_GET_ITEM(h,insert_pos)!=NULL){
        off++;
        insert_pos=HASHFunction((key+off*off),h);
    }
    h->data[insert_pos]=item;
    return (void*)item;
}
HASH_ITEM* HASHFind(HASH* h,void* a,bool (*cmp)(void* a,void* b),bool delete_item,bool insert_item)
{
    if(h==NULL||a==NULL)
        return NULL;
    if(delete_item&&insert_item)
        return NULL;
    unsigned long long int key=((HASH_ITEM*)a)->key;
    unsigned long long int off=0;
    HASH_ITEM* tmp;
    unsigned long long int insert_pos=HASHFunction(key,h);
    while((tmp=(HASH_ITEM*)(h->data[insert_pos]))!=NULL){
        if(tmp->key==key&&tmp->count>0)
        {
            if(cmp(a,tmp)){
                if(delete_item)
                {
                    if(tmp->count>0)
                        tmp->count--;
                }
                if(insert_item)
                    tmp->count++;
                return tmp;
            }
        }
        off++;
        insert_pos=HASHFunction((key+off*off),h);
    }
    return NULL;
}
HASH* HASHCOPY(HASH* h){
    HASH* tmph=(HASH*)m_alloc(sizeof(HASH));
    if(tmph==NULL)
        return NULL;
    tmph->data=m_alloc(sizeof(void*)*(h->capicity));
    tmph->capicity=h->capicity;
    tmph->capicity_index=h->capicity_index;
    tmph->record_num=h->record_num;
    memcpy(tmph->data,h->data,sizeof(void*)*(h->capicity));
    return tmph;
}
void DelHASH(HASH* h)
{
    if(h==NULL)
        return;
    for(size_t i=0;i<h->capicity;++i){
        if(h->data[i]!=NULL)
            m_free(h->data[i]);
    }
    m_free(h->data);
    m_free(h);
}
void TestHASH(void){
    /*test hash*/
    
}