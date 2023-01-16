#include "list.h"

void init_list_node(LIST_NODE* p)
{
    p->prev=p->next=p;
}
void _add_before(LIST_NODE *p,LIST_NODE *new_node)
{
    new_node->prev=p->prev;
    new_node->next=p;
    p->prev->next=new_node;
    p->prev=new_node;
}
void _add_after(LIST_NODE *p,LIST_NODE *new_node)
{
    new_node->prev=p;
    new_node->next=p->next;
    p->next->prev=new_node;
    p->next=new_node;
}
void _del_node(LIST_NODE* p)
{
    if(p->next==p&&p->prev==p)  /*only one node*/
        return;
    p->next->prev=p->prev;
    p->prev->next=p->next;
}