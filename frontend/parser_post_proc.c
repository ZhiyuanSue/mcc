#include "parser_post_proc.h"
/*post-processing 
    in this stage, you should make the parser tree become an abstract syntex tree
    ——including delete some node like useless comma node and shorten the tree of useless node
    and do some symbol table hackery operates for some nodes like typedef name
    or omit some ambiguity of grammar
*/
extern VEC* c_error;
extern char* filename;
char post_proc_node_rec[STATE_NUM];
void post_proc_init()
{
    memset(post_proc_node_rec,0,STATE_NUM*sizeof(char));
    post_proc_node_rec[declaration]=1;
    post_proc_node_rec[struct_declarator_list]=1;
    post_proc_node_rec[identifier_list]=1;
    post_proc_node_rec[parameter_list]=1;
    post_proc_node_rec[initializer_list]=1;
    post_proc_node_rec[init_decl_list]=1;
    post_proc_node_rec[enum_list]=1;
    post_proc_node_rec[spec_qual_list]=1;
    post_proc_node_rec[declaration_spec]=1;
    post_proc_node_rec[assignment_expr]=1;
    post_proc_node_rec[labeled_stmt]=1;
}
AST_BASE* post_processing(AST_BASE* curr_ast)
{
    if(!curr_ast)
        return curr_ast;
    if(!post_proc_node_rec[curr_ast->type])
    {
        goto final;
    }
    ERROR_ITEM * tei=m_alloc(sizeof(ERROR_ITEM));
    bool update_succ=true;
    switch(curr_ast->type){
        case declaration:
        {
            AST_BASE* type_def_kw=curr_ast;
            while(AST_CHILD_NUM(type_def_kw)>0){
                type_def_kw=AST_GET_CHILD(type_def_kw,0);
            }
            AST_BASE* init_decl_list_node=AST_GET_CHILD(curr_ast,1);
            for(size_t i=0;i<AST_CHILD_NUM(init_decl_list_node);i+=2){
                AST_BASE* init_decl_node=AST_GET_CHILD(init_decl_list_node,i);
                AST_BASE* declarator_ast=AST_GET_CHILD(init_decl_node,0);
                if(declarator_ast->ter_num!=0){
                    if(type_def_kw->type==KW_type_def){
                        AST_BASE* type_def_node=declarator_node(declarator_ast);
                        if(type_def_node){  /*I reuse the data struct,but most info is useless*/
                            SYM_ITEM* tmpsi=m_alloc(sizeof(SYM_ITEM));
                            tmpsi->count=HASH_CNT_IST;
                            tmpsi->name_space=NMSP_UNKNOWN;
                            tmpsi->value=(void*)(type_def_node->token->value);
                            tmpsi->key=SymbolCharToKey(tmpsi->value,tmpsi->name_space);
                            tmpsi->defined=true;
                            tmpsi->declared=true;
                            tmpsi->data_size=0;
                            tmpsi->data_field=NULL;
                            HASHInsert(curr_ast->symbol_table->typedef_name_table,(HASH_ITEM*)tmpsi,symbol_item_cmp);
                        }
                    }
                }
            }
            if(curr_ast->father&&curr_ast->father->type==function_definition)
            {
                AST_BASE* init_decl_list_node=NULL;
                for(size_t i=0;i<AST_CHILD_NUM(curr_ast);++i)
                {
                    AST_BASE* tmp_ast=AST_GET_CHILD(curr_ast,i);
                    if(tmp_ast->type==init_decl_list)
                    {    
                        init_decl_list_node=tmp_ast;
                        break;
                    }
                }
                if(init_decl_list_node)
                {
                    for(size_t i=0;i<AST_CHILD_NUM(init_decl_list_node);++i)
                    {
                        AST_BASE* init_decl_node=AST_GET_CHILD(init_decl_list_node,i);
                        if(AST_CHILD_NUM(init_decl_node)==3)
                        {
                            C_ERROR(C0083_ERR_FUNC_PARA_INITILIZATION,curr_ast);
                            update_succ=false;
                        }
                    }
                }
            }
            break;
        }
        case struct_declarator_list:
        case identifier_list:
        case parameter_list:
        case initializer_list:
        case init_decl_list:
        {
            for(size_t i=0;i<AST_CHILD_NUM(curr_ast);++i){
                AST_BASE* tmpnode=AST_GET_CHILD(curr_ast,i);
                if(tmpnode&&tmpnode->type==comma){
                    VECremove(curr_ast->child,i,i+1);
                    m_free(tmpnode);
                }
            }
            break;
        }
        case enum_list:
        {
            AST_BASE* enumerator_node=NULL;
            for(size_t i=0;i<AST_CHILD_NUM(curr_ast);++i){
                AST_BASE* tmpnode=AST_GET_CHILD(curr_ast,i);
                if(tmpnode&&tmpnode->type==comma){
                    VECremove(curr_ast->child,i,i+1);
                    m_free(tmpnode);
                }
            }
            for(size_t i=0;i<AST_CHILD_NUM(curr_ast);i++){
                enumerator_node=AST_GET_CHILD(curr_ast,i);
                AST_BASE* enum_const_node=AST_GET_CHILD(enumerator_node,0);
                SYM_ITEM* tmpsi=m_alloc(sizeof(SYM_ITEM));
                tmpsi->count=HASH_CNT_IST;
                tmpsi->name_space=NMSP_UNKNOWN;
                tmpsi->value=(void*)(enum_const_node->token->value);
                tmpsi->key=SymbolCharToKey(tmpsi->value,tmpsi->name_space);
                tmpsi->defined=true;
                tmpsi->declared=true;
                tmpsi->data_size=0;
                tmpsi->data_field=NULL;
                HASH_ITEM* find_tmphi=HASHFind(curr_ast->symbol_table->enum_const_table,
                    tmpsi,
                    symbol_item_cmp,
                    false,
                    false
                );
                if(HASH_ITEM_EXIST(find_tmphi))
                {
                    C_ERROR(C0005_ERR_REDEFINE,curr_ast);
                    update_succ=false;
                }
                HASHInsert(curr_ast->symbol_table->enum_const_table,(HASH_ITEM*)tmpsi,symbol_item_cmp);
            }
            break;
        }
        case spec_qual_list:
        case declaration_spec:
        {
            int type_spec_count=0,type_def_spec_count=0;
            size_t sub_ter_num=0;
            for(size_t i=0;i<AST_CHILD_NUM(curr_ast);++i){
                AST_BASE* spec_node=AST_GET_CHILD(curr_ast,i);
                if(spec_node->type==type_spec)
                {
                    type_spec_count++;
                    AST_BASE* type_spec_node=AST_GET_CHILD(spec_node,0);
                    if(type_spec_node->type==type_def_name)
                        type_def_spec_count++;
                }
                if(type_def_spec_count==1&&type_spec_count>1)
                {
                    for(size_t j=0;j+i<AST_CHILD_NUM(curr_ast);++j){
                        AST_BASE* del_spec_node=AST_GET_CHILD(curr_ast,i+j);
                        sub_ter_num+=del_spec_node->ter_num;
                        destory_ast_tree(del_spec_node);
                    }
                    VECremove(curr_ast->child,i,AST_CHILD_NUM(curr_ast));
                    curr_ast->ter_num-=sub_ter_num;
                    break;
                }
            }
            break;
        }
        case assignment_expr:
        {
            if(AST_CHILD_NUM(curr_ast)==1)
                break;
            AST_BASE* unary_node=AST_GET_CHILD(curr_ast,0);
            AST_BASE* op=AST_GET_CHILD(curr_ast,1);
            enum rule_type_enum tmp_type=equal;
            enum rule_type_enum sub_unary_type=equal;
            if(op->type!=equal)
            {
                switch (op->type)
                {
                case plus_equal:
                {
                    sub_unary_type=plus;
                    tmp_type=additive_expr;
                    break;
                }
                case minus_equal:
                {
                    sub_unary_type=minus;
                    tmp_type=additive_expr;
                    break;
                }
                case mul_equal:
                {
                    sub_unary_type=star;
                    tmp_type=multi_expr;
                    break;
                }
                case forward_equal:
                {
                    sub_unary_type=forward_slash;
                    tmp_type=multi_expr;
                    break;
                }
                case percent_equal:
                {
                    sub_unary_type=percent;
                    tmp_type=multi_expr;
                    break;
                }
                case left_shift_equal:
                {
                    sub_unary_type=left_shift;
                    tmp_type=shift_expr;
                    break;
                }
                case right_shift_equal:
                {
                    sub_unary_type=right_shift;
                    tmp_type=shift_expr;
                    break;
                }
                case ampersand_equal:
                {
                    sub_unary_type=ampersand;
                    tmp_type=and_expression;
                    break;
                }
                case caret_equal:
                {
                    sub_unary_type=caret;
                    tmp_type=exclusive_or_expr;
                    break;
                }
                case ver_bar_equal:
                {
                    sub_unary_type=vertical_bar;
                    tmp_type=inclusive_or_expr;
                    break;
                }
                default:
                    break;
                }
            }
            else
                break;
            VEC* sub_unary_type_vec=NULL;
            VECcpy(unary_node->child,&sub_unary_type_vec);
            VEC* sub_type_vec=NULL;
            VECcpy(curr_ast->child,&sub_type_vec);
            AST_BASE* tmp_unary_ast=creat_ast_node(
                unary_node->type,
                sub_unary_type_vec,
                unary_node->symbol_table,
                unary_node->token,
                unary_node->ter_num,
                unary_node->name_space
            );
            sub_type_vec->data[0]=(void*)tmp_unary_ast;
            sub_type_vec->data[1]=creat_ast_node(
                sub_unary_type,
                NULL,
                unary_node->symbol_table,
                op->token,
                op->ter_num,
                unary_node->name_space
            );
            AST_BASE* tmp_ast=creat_ast_node(
                tmp_type,
                sub_type_vec,
                unary_node->symbol_table,
                unary_node->token,
                unary_node->ter_num,
                unary_node->name_space
            );
            op->type=equal;
            curr_ast->child->data[2]=(void*)tmp_ast;
            break;
        }
        case labeled_stmt:
        {
            AST_BASE* label_node=AST_GET_CHILD(curr_ast,0);
            if(label_node->type==identifier)
            {
                SYM_ITEM* tmpsi=m_alloc(sizeof(SYM_ITEM));
                tmpsi->count=HASH_CNT_IST;
                tmpsi->name_space=NMSP_UNKNOWN;
                tmpsi->value=(void*)(label_node->token->value);
                tmpsi->key=SymbolCharToKey(tmpsi->value,tmpsi->name_space);
                tmpsi->defined=true;
                tmpsi->declared=true;
                tmpsi->data_size=0;
                tmpsi->data_field=NULL;
                M_TYPE* tmpt=build_base_type(TP_LABEL);
                tmpsi->type_vec=InitVEC(DEFAULT_CAPICITY);
                VECinsert(tmpsi->type_vec,tmpt);
                SYM* target_symbol_table=curr_ast->symbol_table;
                while(1)
                {
                    if(target_symbol_table->father==NULL||((target_symbol_table->father->st_attr_type)&SA_FUNC)==0)
                        break;
                    else
                        target_symbol_table=target_symbol_table->father;
                }
                HASH* h=target_symbol_table->sym_hash_table;
                HASH_ITEM* find_tmphi=HASHFind(h,
                    tmpsi,
                    symbol_item_cmp,
                    false,
                    false
                );
                if(HASH_ITEM_EXIST(find_tmphi))
                {
                    C_ERROR(C0005_ERR_REDEFINE,curr_ast);
                    update_succ=false;
                }
                HASHInsert(h,(HASH_ITEM*)tmpsi,symbol_item_cmp);
            }
            break;
        }
        default:
            break;
    }
    if(!update_succ)
    {
        destory_ast_tree(curr_ast);
        return NULL;
    }
final:
    for(size_t i=0;i<AST_CHILD_NUM(curr_ast);++i){
        AST_BASE* child_ast=AST_GET_CHILD(curr_ast,i);
        child_ast->father=curr_ast;
    }
    return curr_ast;
}
/*just a tool function to deal with the recursively declarator*/
AST_BASE* declarator_node(AST_BASE* declarator_ast)
{
    if(declarator_ast->type!=declarator)
    {
        return NULL;
    }
    AST_BASE* direct_declarator_node=AST_GET_CHILD(declarator_ast,AST_CHILD_NUM(declarator_ast)-1);
    AST_BASE* first_child=AST_GET_CHILD(direct_declarator_node,0);
    if(first_child->type==identifier)
    {
        return first_child;
    }
    else if(first_child->type==left_parenthesis)
    {
        return declarator_node(AST_GET_CHILD(direct_declarator_node,1));
    }
    else return NULL;
}