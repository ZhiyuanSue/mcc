#include "search_mem.h"
VEC* search_mem[NONTER_NUM][TERMINATOR_NUM+1];    /*the memory*/
bool searched[NONTER_NUM];  /*whether the nonterminal is searched*/
static VEC* loop[NONTER_NUM];
static VEC* loop_mem[NONTER_NUM];
static VEC* trace_stack;
void build_mem(void* rule){
    trace_stack=InitVEC(DEFAULT_CAPICITY);
    for(size_t i=0;i<NONTER_NUM;++i){
        for(size_t j=0;j<=TERMINATOR_NUM;++j){
            search_mem[i][j]=InitVEC(DEFAULT_CAPICITY);
        }
        searched[i]=false;
        loop[i]=InitVEC(DEFAULT_CAPICITY);
        loop_mem[i]=InitVEC(DEFAULT_CAPICITY);
    }
    dfs_search(rule);
    DelVEC(trace_stack);
    trace_stack=InitVEC(DEFAULT_CAPICITY);
    for(size_t i=0;i<NONTER_NUM;++i){
        searched[i]=false;
    }
    update_table(rule);
    DelVEC(trace_stack);
    for(size_t i=0;i<NONTER_NUM;++i){
        DelVEC(loop[i]);
    }
    /*print_mem();*/
}
void dfs_search(void* rule){
    RULE* curr_rule=(RULE*)rule;
    VECinsert(trace_stack,rule);
    if(curr_rule->type<=double_percent_colon&&curr_rule->type>0){
        /*a terminal*/
        for(size_t i=0;i<VECLEN(trace_stack)-1;++i){
            RULE* tmpr=VEC_GET_ITEM(trace_stack,i);
            insert_mem(search_mem[tmpr->rule_index][curr_rule->type],VEC_GET_ITEM(trace_stack,i+1));
        }
    }
    else{/*a nonterminal*/
        /*searched, insert to trace*/
        if(searched[curr_rule->rule_index])
        {
            for(size_t i=0;i<VECLEN(trace_stack)-1;++i){
                RULE* tmpr=VEC_GET_ITEM(trace_stack,i);
                insert_rule_to_mem(curr_rule->rule_index,tmpr->rule_index,VEC_GET_ITEM(trace_stack,i+1));
            }
            VECpopback(trace_stack);
            return;
        }
        else
        {
            /*judge whether it's a loop node*/
            bool have_loop=false;
            for(size_t i=0;i<VECLEN(trace_stack)-1;++i){
                if(VEC_GET_ITEM(trace_stack,i)==rule){
                    have_loop=true;
                    break;
                }
            }
            /*have a loop, insert trace*/
            if(have_loop){
                size_t trace_index=VECLEN(trace_stack)-2;
                while(trace_index>=0){
                    RULE* trace_rule=(RULE*)VEC_GET_ITEM(trace_stack,trace_index);
                    if(trace_rule==curr_rule)
                        break;
                    VECinsert(loop[trace_rule->rule_index],rule);
                    VECinsert(loop_mem[trace_rule->rule_index],VEC_GET_ITEM(trace_stack,trace_index+1));
                    trace_index--;
                }
                VECpopback(trace_stack);
                return;
            }
        }
        for(size_t i=0;i<VECLEN(curr_rule->rule_value);++i){
            dfs_search(VEC_GET_ITEM(curr_rule->rule_value,i));
        }
        searched[curr_rule->rule_index]=true;
    }
    VECpopback(trace_stack);
}
void update_table(void* rule){
    RULE* curr_rule=(RULE*)rule;
    VECinsert(trace_stack,rule);
    if(curr_rule->type<=double_percent_colon&&curr_rule->type>0){
        /*a terminal,do nothing*/
        ;
    }
    else{/*a nonterminal*/
        /*have loop return*/
        for(size_t i=0;i<VECLEN(trace_stack)-1;++i){
            if(VEC_GET_ITEM(trace_stack,i)==rule){
                VECpopback(trace_stack);
                return;
            }
        }
        if(searched[curr_rule->rule_index]){
            for(size_t i=0;i<VECLEN(trace_stack)-1;++i){
                RULE* tmpr=VEC_GET_ITEM(trace_stack,i);
                insert_rule_to_mem(curr_rule->rule_index,tmpr->rule_index,VEC_GET_ITEM(trace_stack,i+1));
            }
            VECpopback(trace_stack);
            return;
        }
        if(VECLEN(loop[curr_rule->rule_index])!=0){
            for(size_t i=0;i<VECLEN(loop[curr_rule->rule_index]);++i){
                RULE* tmpr=VEC_GET_ITEM(loop[curr_rule->rule_index],i);
                insert_rule_to_mem(tmpr->rule_index,curr_rule->rule_index,VEC_GET_ITEM(loop_mem[curr_rule->rule_index],i));
            }
        }
        for(size_t i=0;i<VECLEN(curr_rule->rule_value);++i){
            update_table(VEC_GET_ITEM(curr_rule->rule_value,i));
        }
        searched[curr_rule->rule_index]=true;
    }
    VECpopback(trace_stack);
}
void insert_mem(VEC* v,void* rule){
    bool first_meet=true;
    for(size_t i=0;i<VECLEN(v);++i){
        if(VEC_GET_ITEM(v,i)==rule)
        {
            first_meet=false;
            break;
        }
    }
    if(first_meet){
        VECinsert(v,rule);
    }
}
void insert_rule_to_mem(size_t src_rule_index,size_t dst_rule_index,void* next_hop){
    for(size_t i=1;i<=TERMINATOR_NUM;++i){
        if(((RULE*)next_hop)->rule_index==dst_rule_index)
            continue;
        if(VECLEN(search_mem[src_rule_index][i])){
            insert_mem(search_mem[dst_rule_index][i],next_hop);
        }
    }
}
void print_mem(void){
    for(size_t i=0;i<VECLEN(search_mem[12][equal]);++i){
        RULE* tmpr=(RULE*)VEC_GET_ITEM(search_mem[12][equal],i);
        printf("%s\n",tmpr->base.name);
    }
    for(size_t i=0;i<NONTER_NUM;++i){
        printf("rule:%ld\n",i);
        for(size_t j=1;j<=TERMINATOR_NUM;++j)
        {
            printf("<%ld:%ld>\t",j,VECLEN(search_mem[i][j]));
        }
        printf("\n");
    }
}