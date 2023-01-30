#ifndef _LIST_H_
#define _LIST_H_
#include "mcc_error.h"
#include "MEM.h"
typedef struct list_node LIST_NODE;
struct list_node{
    LIST_NODE *prev,*next;
};
void init_list_node(LIST_NODE* p);
void _add_before(LIST_NODE *p,LIST_NODE *new_node);
void _add_after(LIST_NODE *p,LIST_NODE *new_node);
void _del_node(LIST_NODE* p);

#endif