#ifndef _AST_H_
#define _AST_H_
#include "../tools/symbol_table.h"
#include "../tools/vector.h"
#include "../tools/token.h"
#include "../tools/MEM.h"
#include "../tools/EBNF.h"
#include "../tools/m_type.h"
#define AST_CHILD_NUM(ast_node) ( (ast_node&& (ast_node->child)) ? (ast_node->child->len) : 0 )
#define AST_GET_CHILD(ast_node,i) ( (ast_node && (ast_node->child) && (i<ast_node->child->len) ) ? (AST_BASE*)ast_node->child->data[i]: NULL )
typedef struct{
    DATA_VALUE
    bool is_lvalue:1;
    bool complete:1;
    bool is_bit_field:1;
}EXPR_NODE_ATTR;
typedef struct{
    size_t off;
    size_t size;
    M_TYPE* scalar_type;    /*only terminal initializer reserve this type, in order to fill in the data*/
}INIT_NODE_ATTR;
typedef struct{
    VEC* decl_symbol_item_list;
}DECL_NODE_ATTR;
typedef struct ast_base AST_BASE;
struct ast_base{
    enum rule_type_enum type;
    VEC* child;
    AST_BASE* father;
    SYM* symbol_table;
    TOKEN* token;
    size_t ter_num;
    NMSP name_space;
    union{  /*some node need extral attributes,but they cannot appear at the same time*/
        EXPR_NODE_ATTR* expr_attribute;
        INIT_NODE_ATTR* init_attribute;
        DECL_NODE_ATTR* decl_attribute;
        SYM_ITEM* func_attribute;
    };
};

inline AST_BASE* creat_ast_node(enum rule_type_enum type,VEC* child,SYM* symbol_table,TOKEN* token,size_t ter_num,NMSP name_space){
    AST_BASE* node=(AST_BASE*)m_alloc(sizeof(AST_BASE));
    node->type=type;
    node->child=child;
    node->symbol_table=symbol_table;
    node->token=token;
    node->ter_num=ter_num;
    node->name_space=name_space;
    node->father=NULL;
    node->expr_attribute=NULL;
    node->init_attribute=NULL;
    return node;
}
void destory_ast_tree(AST_BASE* node);
void print_ast_tree(AST_BASE* node,int level);
void destory_subast_sym_table(AST_BASE* node,SYM* top_sym_table);
void destory_ast_symbol(AST_BASE* node);
#endif