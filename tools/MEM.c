#include "MEM.h"

extern MEM_POOL* curr_heap;
unsigned int item_size[MEM_BLOCK_CHAIN_NUM]={4,8,16,32,64,128,256,512,1024};
unsigned int real_item_size[MEM_BLOCK_CHAIN_NUM];
unsigned int item_num[MEM_BLOCK_CHAIN_NUM];
unsigned int block_size[MEM_BLOCK_CHAIN_NUM];
#ifdef DEFAULT_MEM
#if DEFAULT_MEM==0
void* m_alloc(size_t n){
    curr_heap->sys_malloc_cnt++;
    void* p=malloc(n);
    if(p==NULL){
        m_error("MEM.h","m_alloc","malloc a memory fail");
        exit(1);
    }
    return p;
}
void m_free(void* p){
    curr_heap->sys_free_cnt++;
    if(p==NULL)
        return;
    free(p);
    p=NULL;
}
#else

//here is my memory strategy
void* m_alloc(size_t n){
    if(n<=0)
        return NULL;
    void* res;
    size_t chain_num=MEM_GET_SIZE(n)-2;
    if(chain_num==MEM_BLOCK_CHAIN_NUM){
        res=malloc(n);
        M_NODE* tmpnode=malloc(sizeof(M_NODE));
        tmpnode->p=res;
        tmpnode->next=curr_heap->sys_alloc->next;
        curr_heap->sys_alloc->next=tmpnode;
        curr_heap->sys_malloc_cnt++;
        return res;
    }
    MEM_ITEM* res_item;
    MEM_BLOCK* res_block=curr_heap->free_block_chain[chain_num];
    if(!res_block||res_block->free_item_header==item_num[chain_num]){
        res_block=MEMInitBlock(chain_num);
        res_block->next=curr_heap->block_chain[chain_num];
        res_block->next_free=NULL;
        curr_heap->block_chain[chain_num]=res_block;
        curr_heap->free_block_chain[chain_num]=res_block;
    }
    if(res_block==NULL)
        return NULL;
    size_t off=(res_block->free_item_header)*real_item_size[chain_num];
    res_item=(MEM_ITEM*)((uintptr_t)(res_block->data)+off);
    res_block->free_item_header=res_item->next;
    if(res_block->free_item_header==item_num[chain_num])
    {
        curr_heap->free_block_chain[chain_num]=res_block->next_free;
    }
    res=(void*)(((MEM_ITEM*)res_item)+1);
    /*
    unsigned int mod_res=((uintptr_t)res)%(_ALIGN_SIZE_(_MCC_ALIGN_PACK_)*2);
    //printf("%ld %d %ld %ld\n",(uintptr_t)res,mod_res,n,MEM_GET_SIZE(n));
    if(mod_res!=0)
    {
        printf("Complier error:m_alloc alignment\n");
    }*/
    curr_heap->malloc_cnt++;
    return res;
}
void m_free(void* p){
    if(p==NULL)
        return;
    M_NODE* pre=curr_heap->sys_alloc;
    M_NODE* curr=curr_heap->sys_alloc->next;
    bool free_succ=false;
    /*find in system*/
    while(curr){
        if(curr->p==p){
            pre->next=curr->next;
            free(curr->p);
            free(curr);
            free_succ=true;
            curr_heap->sys_free_cnt++;
            break;
        }
        pre=curr;
        curr=curr->next;
    }
    /*use mem pool*/
    if(!free_succ){
        MEM_ITEM* curr_item=((MEM_ITEM*)p)-1;
        MEM_BLOCK* res_block;
        void* data_ptr=(void*)((uintptr_t)curr_item-(curr_item->item_id)*(real_item_size[curr_item->chain_id]));
        MEM_BLOCK** head_block_ptr=(MEM_BLOCK**)(((MEM_BLOCK**)data_ptr)-1);
        res_block=*head_block_ptr;
        curr_heap->free_cnt++;
        if(res_block->free_item_header==item_num[curr_item->chain_id])
        {
            res_block->next_free=curr_heap->free_block_chain[curr_item->chain_id];
            curr_heap->free_block_chain[curr_item->chain_id]=res_block;
        }
        curr_item->next=res_block->free_item_header;
        res_block->free_item_header=curr_item->item_id;
    }
    p=NULL;
}
#endif
MEM_POOL* MEMInit(char name[])
{
    MEMInit_size_array();
    MEM_POOL* pool=malloc(sizeof(MEM_POOL));
    for(int i=0;i<8;++i){
        if(name[i]=='\0')
            break;
        pool->name[i]=name[i];
    }
    pool->name[7]='\0';
    for(int i=0;i<MEM_BLOCK_CHAIN_NUM;++i)
        pool->block_chain[i]=pool->free_block_chain[i]=NULL;
    pool->sys_alloc=malloc(sizeof(M_NODE));
    pool->sys_alloc->p=NULL;
    pool->sys_alloc->next=NULL;
    pool->malloc_cnt=pool->free_cnt=pool->sys_malloc_cnt=pool->sys_free_cnt=0;
    return pool;
}
void MEMInit_size_array(void){
    for(size_t i=0;i<MEM_BLOCK_CHAIN_NUM;++i){
        item_size[i]=max(item_size[i],_ALIGN_SIZE_(_MCC_ALIGN_PACK_));
        real_item_size[i]=ITEM_HEAD_SIZE+item_size[i];
        item_num[i]=DEFAULT_BLOCK_SIZE/real_item_size[i];
        block_size[i]=DEFAULT_BLOCK_SIZE+item_num[i]*ITEM_HEAD_SIZE;
    }
}
MEM_BLOCK* MEMInitBlock(size_t chain_num)
{
    MEM_BLOCK* block=malloc(sizeof(MEM_BLOCK));
    unsigned int sys_malloc_size=MCC_ALIGN( block_size[chain_num], _ALIGN_SIZE_(_MCC_ALIGN_PACK_))+_ALIGN_SIZE_(_MCC_ALIGN_PACK_)+sizeof(MEM_BLOCK*);
    block->sys_malloc_res=malloc(sys_malloc_size);
    memset(block->sys_malloc_res,0,sys_malloc_size);
    block->data=(void*)((uintptr_t)(block->sys_malloc_res)+sizeof(MEM_BLOCK*));
    if(((uintptr_t)(block->data))/_ALIGN_SIZE_(_MCC_ALIGN_PACK_)!=0)
        block->data=(void*)MCC_ALIGN( block->data, _ALIGN_SIZE_(_MCC_ALIGN_PACK_));
    block->free_item_header=0;
    block->next=NULL;
    for(int i=0;i<item_num[chain_num];++i){
        MEM_ITEM* item_head=(MEM_ITEM*)((uintptr_t)(block->data)+i*real_item_size[chain_num]);
        item_head->chain_id=chain_num;
        item_head->item_id=i;
        item_head->next=i+1;
        item_head->size=item_size[chain_num];
    }
    MEM_BLOCK** head_block_ptr=(MEM_BLOCK**)((MEM_BLOCK**)(block->data)-1);
    *head_block_ptr=block;
    return block;
}
void MEMDelBlock(MEM_BLOCK* block){
    if(!block)
        return;
    free(block->sys_malloc_res);
    free(block);
    block=NULL;
}
void MEMDel(MEM_POOL* pool){
    if(!pool)
        return;
    for(int i=0;i<MEM_BLOCK_CHAIN_NUM;++i){
        MEM_BLOCK* block=pool->block_chain[i];
        MEM_BLOCK* next;
        while(next){
            next=block->next;
            MEMDelBlock(block);
            block=next;
        }
    }
#ifdef _TEST_
    printf("mempool %s alloc/free: alloc %lld times,free %lld times\n",pool->name,pool->malloc_cnt,pool->free_cnt);
    printf("\tuse sys alloc/free: alloc %lld times,free %lld times\n",pool->sys_malloc_cnt,pool->sys_free_cnt);
#endif
    M_NODE* pre=curr_heap->sys_alloc;
    M_NODE* curr=pre->next;
    while(curr){
        pre->next=curr->next;
        free(curr->p);
        free(curr);
        curr=pre->next;
    }
    free(pool);
    pool=NULL;
}
#endif