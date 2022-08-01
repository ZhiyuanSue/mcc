#ifndef _SEARCH_MEM_H_
#define _SEARCH_MEM_H_
#include "../tools/vector.h"
#include "../tools/mcc_error.h"
#include "../tools/EBNF.h"

void build_mem(void* rule);
void dfs_search(void* rule);
void update_table(void* rule);
void insert_mem(VEC* v,void* rule);
void insert_rule_to_mem(size_t src_rule_index,size_t dst_rule_index,void* next_hop);
void print_mem(void);
#endif