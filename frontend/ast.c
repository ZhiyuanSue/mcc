#include "ast.h"
void destory_ast_tree(AST_BASE* node){
    if(node==NULL)
        return;
    if(node->child){
        for(size_t i=0;i<VECLEN(node->child);i++){
            AST_BASE* tmpa=VEC_GET_ITEM(node->child,i);
            destory_ast_tree(tmpa);
        }
    }
    DelVEC(node->child);
    m_free(node);
}
void print_ast_tree(AST_BASE* node,int level)
{
    if(node==NULL||node->type==end_of_file)
        return;
    for(int i=0;i<level;++i)
        printf("    ");
    if(AST_CHILD_NUM(node))
        printf("<%s ns:%d>\n",rule_type_str[node->type],node->name_space);
    else
        printf("<%s:%s sym level: %d namespace:%d>\n",rule_type_str[node->type],node->token->value,node->symbol_table->level,node->name_space);
    if(node->child){
        for(size_t i=0;i<VECLEN(node->child);++i){
            print_ast_tree(VEC_GET_ITEM(node->child,i),level+1);
        }
    }
}