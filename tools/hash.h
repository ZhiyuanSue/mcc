#ifndef _HASH_H_
#define _HASH_H_
#define DEFAULT_CAP 61
#define HASH_CAP_NUM 29
#define HASH_CNT_FD 0
#define HASH_CNT_IST 1 
#include "mcc_error.h"
#include "MEM.h"
#include "namespace.h"
#define HASH_GET_ITEM(h,i) ((i<h->capicity)?(h->data[i]):NULL)
#define HASH_ITEM_EXIST(hi) (hi&&hi->count)
typedef struct hash_table{
    void** data;
    unsigned long long int capicity;
    unsigned long long int capicity_index;   /* indicate the index in hash_capicity*/
    unsigned long long int record_num;
}HASH;
#define HASH_COMMON unsigned long long int key;\
                    unsigned int count;
typedef struct hash_item{
    HASH_COMMON
}HASH_ITEM;
inline unsigned long long int HASHFunction(unsigned long long int key,HASH* h)
{
    return key%(h->capicity);
}
HASH* InitHASH(void);
bool HSAHExpand(HASH* h,bool (*cmp)(void* a,void* b));
void* HASHInsert(HASH* h,HASH_ITEM* item,bool (*cmp)(void* a,void* b));
HASH_ITEM* HASHFind(HASH* h,void* a,bool (*cmp)(void* a,void* b),bool delete_item,bool insert_item);
bool HASHCOPY(HASH* dst,HASH* src,bool (*cmp)(void* a,void* b));
bool HASHDELITEM(HASH* h,HASH_ITEM* del_item,bool (*cmp)(void* a,void* b));
/*
the two bool has following value
find has three type:
insert - false/true
delete - true/false
just find - false/false
so delete_item and insert_item cannot be all true

The hash delete item function :
    for hash find, if you set delete is true, the deletion is lazy way, and it try to change the item's count flag
    but this hash delete item function, it really delete the point to that item,which don't change the item
    multiple hash table may point to a same item, so if I try to lazy delete the item, another hash table will change
    use this function to delete the point in current hash table
*/
void DelHASH(HASH* h);
void TestHASH(void);
#endif