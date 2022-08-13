#ifndef _MEM_H_
#define _MEM_H_
#include "mcc_error.h"
#define MIN_ITEM_SIZE 4  /*for align*/
#define MAX_ITEM_SIZE 1024
#define DEFAULT_BLOCK_SIZE 4096
#define ITEM_HEAD_SIZE sizeof(MEM_ITEM)
#define MEM_BLOCK_CHAIN_NUM 9
#define DEFAULT_CAPICITY 3

/*system alloc hash table*/
/*
typedef struct malloc_node M_NODE;
struct malloc_node{
    void* p;
    M_NODE* next;
};*/
/*item*/
/*
typedef union mem_item{
    struct {
        unsigned short chain_id;
        unsigned short item_id;
        unsigned short next;
        unsigned short size;
    };
    unsigned char ALIGN[_ALIGN_SIZE_(_MCC_ALIGN_PACK_)];
}MEM_ITEM;
*/
/*block*/
/*
typedef struct mem_block MEM_BLOCK;
struct mem_block{
    MEM_BLOCK* next;
    MEM_BLOCK* next_free;
    unsigned short free_item_header;
    void* sys_malloc_res;
    void* data;
};*/
/*pool*/
typedef struct mem_pool{
    char name[8];   /* this size is not because of any special reason,just for fun*/
    //MEM_BLOCK* block_chain[MEM_BLOCK_CHAIN_NUM];
    //M_NODE* sys_alloc;
    //MEM_BLOCK* free_block_chain[MEM_BLOCK_CHAIN_NUM];
    unsigned long long int malloc_cnt;
    unsigned long long int free_cnt;
    unsigned long long int sys_malloc_cnt;
    unsigned long long int sys_free_cnt;
}MEM_POOL;
/*
inline size_t MEM_GET_SIZE(size_t n){
    if(n<=0)
        return 0;
    if(n<=MIN_ITEM_SIZE)
        return 2;
    if(n>MAX_ITEM_SIZE)
        return 11;
    int nlevel=0;
    while(n>0){
        n>>=1;
        nlevel++;
    }
    return nlevel;
}*/

MEM_POOL* MEMInit(char name[]);
/*
void TestMem(void);
void MEMInit_size_array(void);
MEM_BLOCK* MEMInitBlock(size_t chain_num);
void MEMDelBlock(MEM_BLOCK* block);*/
void MEMDel(MEM_POOL* pool);


void* m_alloc(size_t n);
void m_free(void* p);
#if DEFAULT_MEM==0
inline void m_memset(void* p,char value,size_t len)
{
    memset(p,value,len);
}
#else
inline void m_memset(void* p,char value,size_t len)
{
    memset(p,value,len);
    /*
    printf("%p %ld %ld\n",p,value,len);
    for(char* tp=p;
        (size_t)tp<(size_t)(p)+len;
        ++p)
    {
        *tp=value;
    }*/
}
#endif
#endif