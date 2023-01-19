#include "parser.h"
extern VEC* c_error;
extern char* filename;
size_t max_match_len=0;
static VEC* trace_stack;
#ifdef _TEST_PARSER_
    static VEC* trace_name_stack;
#endif
ERROR_ITEM tei;
int max_deepth=0;
VEC* sym_table_list;
long int ast_total_nodes=0,total_search_nodes=0;
char new_sym_table_rec[STATE_NUM];
AST_BASE* parser(VEC* token_list,VEC* pvec){
    if(trace_stack)
        DelVEC(trace_stack);
    trace_stack=InitVEC(DEFAULT_CAPICITY);
    sym_table_list=InitVEC(DEFAULT_CAPICITY);
    void* start=VEC_GET_ITEM(pvec,0);
    tei=*(ERROR_ITEM*)m_alloc(sizeof(ERROR_ITEM));
    tei.filename=filename;
    tei.ce=0;
    tei.error_line=0;
    tei.error_token_value=NULL;
    m_memset(new_sym_table_rec,'\0',sizeof(char)*STATE_NUM);
    new_sym_table_rec[translation_unit]=1;
    new_sym_table_rec[function_definition]=1;
    new_sym_table_rec[declaration]=1;
    new_sym_table_rec[compound_stmt]=1;
    new_sym_table_rec[if_stmt]=1;
    new_sym_table_rec[switch_stmt]=1;
    new_sym_table_rec[while_stmt]=1;
    new_sym_table_rec[do_stmt]=1;
    new_sym_table_rec[for_stmt]=1;
    new_sym_table_rec[statement]=1;
    new_sym_table_rec[parameter_type_list]=1;
    new_sym_table_rec[identifier_list]=1;
#ifdef _TEST_PARSER_
    printf("parser start\n");
    trace_name_stack=InitVEC(DEFAULT_CAPICITY);
#endif
    TOKEN* first_token=(TOKEN*)VEC_GET_ITEM(token_list,0);
    AST_BASE* trans_unit=NULL;
    post_proc_init();
    if(first_token->type==end_of_file){
        trans_unit=creat_ast_node(end_of_file,NULL,NULL,first_token,0,0);
    }
    else
        trans_unit=p_match(token_list,start,0,NULL,false,0);
    if(trans_unit==NULL||trans_unit->ter_num!=VECLEN(token_list)-1)
    {
#ifdef _TEST_PARSER_
        printf("parser max match %ld\n",max_match_len);
#endif
        if(tei.ce!=C0001_ERR_NONE)
            VECinsert(c_error,(void*)&tei);
        return NULL;
    }
    else{
#ifdef _TEST_PARSER_
        printf("parser success with deepth %d!\n",max_deepth);
        printf("parser totally searched %ld nodes\n",total_search_nodes);
        printf("parser totally build an ast with %ld nodes\n",ast_total_nodes);
#endif
        for(size_t i=0;i<VECLEN(sym_table_list);++i){
            Del_symbol_table((SYM*)VEC_GET_ITEM(sym_table_list,i));
        }
        DelVEC(sym_table_list);
        DelVEC(trace_stack);
        trace_stack=InitVEC(DEFAULT_CAPICITY);
        if(!p_symbol_table(trans_unit,NULL))
            return NULL;
        DelVEC(trace_stack);
        return trans_unit;
    }
}
AST_BASE* p_match(VEC* token_list,
    void* rule,
    size_t token_index,
    SYM* curr_sym_table,
    bool build_sym_table,
    NMSP name_space)
{
    AST_BASE* curr_ast=NULL;
    AST_BASE* sub_ast=NULL;
    AST_BASE* match_ast=NULL;
    VEC* tmpv=InitVEC(DEFAULT_CAPICITY);
    size_t sub_tree_ter_num=0;
    int match_time=0;
    bool have_error=false;
    RULE* curr_rule=(RULE*)rule;
    RULE* sub_rule;
    enum rule_type_enum* tmp_rule_type=&(curr_rule->type);
    VECinsert(trace_stack,(void*)tmp_rule_type);
    max_deepth=max(max_deepth,VECLEN(trace_stack));
    bool have_create_symbol_table=false;
    curr_sym_table=parser_creater_symbol_table(curr_sym_table,
                        &have_create_symbol_table,
                        curr_rule->type,
                        &build_sym_table);
    if(have_create_symbol_table){
        VECinsert(sym_table_list,(void*)curr_sym_table);
    }
    name_space=parser_node_name_space(curr_rule->type,
                    token_list,
                    token_index,
                    name_space,
                    curr_sym_table);
    TOKEN* curr_token=VEC_GET_ITEM(token_list,token_index);
    enum compiler_error ce=C0001_ERR_NONE;
#ifdef _TEST_
#ifdef _TEST_PARSER_
    total_search_nodes++;
    for(int i=0;i<VECLEN(trace_stack);++i)
        printf("\t");
    printf("i:%ld t:%s r:%s ns:%d\n",token_index,curr_token->value,curr_rule->base.name,name_space);
    VECinsert(trace_name_stack,(void*)curr_rule->base.name);
#endif
#endif
    if(curr_rule->type<=double_percent_colon&&curr_rule->type>0){
        /*meet a terminal rule*/
        if(curr_token->type==curr_rule->type)
        {
            if(curr_token->type==identifier)
            {
                /*typedef name*/
                enum rule_type_enum father_rule_type=child;
                enum rule_type_enum grand_father_rule_type=child;
                if(VECLEN(trace_stack)>=2)
                    father_rule_type=*(enum rule_type_enum*)VEC_GET_ITEM(trace_stack,VECLEN(trace_stack)-2);
                if(VECLEN(trace_stack)>=3)
                    grand_father_rule_type=*(enum rule_type_enum*)VEC_GET_ITEM(trace_stack,VECLEN(trace_stack)-3);
                if(!is_type_def_name(curr_token->value,curr_sym_table)&&father_rule_type==type_def_name)
                    goto parser_error;
                if(is_type_def_name(curr_token->value,curr_sym_table)&&father_rule_type!=type_def_name)
                {
                    for(size_t j=VECLEN(trace_stack)-1;j>0;j--)
                    {
                        if(*(enum rule_type_enum*)VEC_GET_ITEM(trace_stack,j)==declarator)
                            break;
                        else if(*(enum rule_type_enum*)VEC_GET_ITEM(trace_stack,j)==statement)/*in a statment , a typedef name identifier must have a father node type_def_name*/
                            goto parser_error;
                    }
                }
                if(father_rule_type!=enumerator_const&&grand_father_rule_type==primary_expression){
                    if(is_enum_const(curr_token->value,curr_sym_table))
                        goto parser_error;
                }
                else if(father_rule_type==enumerator_const&&grand_father_rule_type==primary_expression){
                    if(!is_enum_const(curr_token->value,curr_sym_table))
                        goto parser_error;
                }
            }
            curr_ast=creat_ast_node(curr_rule->type,
                        NULL,
                        curr_sym_table,
                        curr_token,
                        1,
                        name_space);
            goto parser_succ;
        }
        else
            goto parser_error;
    }
    else{
        /* meet a nonterminal rule*/
        for(size_t i=0;i<VECLEN(curr_token->parser_token_mem);++i){
            AST_BASE* memtmpa=VEC_GET_ITEM(curr_token->parser_token_mem,i);
            if(memtmpa==NULL){
                VECremove(curr_token->parser_token_mem,i,i+1);
                continue;
            }
            if(curr_rule->type==memtmpa->type&&curr_rule->type>double_percent_colon){
                curr_ast=memtmpa;
                goto final;
            }
        }
        switch(curr_rule->base.type){
            case 2:
                if(curr_token->type==end_of_file \
                    ||VECLEN(search_mem[curr_rule->rule_index][curr_token->type])==0)
                {
                    goto parser_error;
                }
                for(size_t i=0;i<VECLEN(curr_rule->rule_value);++i){
                    sub_rule=VEC_GET_ITEM(curr_rule->rule_value,i);
                    sub_ast=p_match(token_list,
                                sub_rule,
                                token_index+sub_tree_ter_num,
                                curr_sym_table,
                                build_sym_table,
                                name_space);
                    if(sub_ast==NULL){
                        if(i>0){
                            ce=C0007_ERR_UNEXPECTED_TOKEN;
                            have_error=true;
                        }
                        goto parser_error;
                    }
                    VECinsert(tmpv,(void*)sub_ast);
                    sub_tree_ter_num+=sub_ast->ter_num;
                    max_match_len=max(max_match_len,token_index+sub_tree_ter_num);
                }
                curr_ast=creat_ast_node(curr_rule->type,
                            NULL,
                            curr_sym_table,
                            curr_token,
                            sub_tree_ter_num,
                            name_space);
                break;
            case 3:
                if(curr_token->type==end_of_file \
                    ||VECLEN(search_mem[curr_rule->rule_index][curr_token->type])==0)
                {
                    goto parser_error;
                }
                for(size_t i=0;i<VECLEN(search_mem[curr_rule->rule_index][curr_token->type]);++i){
                    sub_ast=p_match(token_list,
                                VEC_GET_ITEM(search_mem[curr_rule->rule_index][curr_token->type],i),
                                token_index,
                                curr_sym_table,
                                build_sym_table,
                                name_space);
                    if(sub_ast==NULL)
                        continue;
                    if(match_ast==NULL)
                        match_ast=sub_ast;
                    else if(sub_ast->ter_num>match_ast->ter_num)
                        match_ast=sub_ast;
                    max_match_len=max(max_match_len,token_index+sub_tree_ter_num);
                }
                if(match_ast==NULL)
                    goto parser_error;
                VECinsert(tmpv,(void*)match_ast);
                curr_ast=creat_ast_node(curr_rule->type,
                            NULL,
                            curr_sym_table,
                            curr_token,
                            match_ast->ter_num,
                            name_space);
                break;
            case 4:
                while((sub_ast=p_match(token_list,
                                VEC_GET_ITEM(curr_rule->rule_value,0),
                                token_index+sub_tree_ter_num,
                                curr_sym_table,
                                build_sym_table,
                                name_space))!=NULL)
                {
                    VECinsert(tmpv,(void*)sub_ast);
                    sub_tree_ter_num+=sub_ast->ter_num;
                    if(sub_ast->ter_num==0)
                        break;
                    max_match_len=max(max_match_len,token_index+sub_tree_ter_num);
                }
                curr_ast=creat_ast_node(curr_rule->type,
                            NULL,
                            curr_sym_table,
                            curr_token,
                            sub_tree_ter_num,
                            name_space);
                break;
            case 5:
                while((sub_ast=p_match(token_list,
                                VEC_GET_ITEM(curr_rule->rule_value,0),
                                token_index+sub_tree_ter_num,
                                curr_sym_table,
                                build_sym_table,name_space))!=NULL)
                {
                    VECinsert(tmpv,(void*)sub_ast);
                    sub_tree_ter_num+=sub_ast->ter_num;
                    match_time++;
                    if(sub_ast->ter_num==0)
                    {
                        match_time=0;
                        break;
                    }
                    max_match_len=max(max_match_len,token_index+sub_tree_ter_num);
                }
                if(match_time>1)
                {
                    goto parser_error;
                }
                else if(match_time==0)
                    curr_ast=creat_ast_node(curr_rule->type,
                        NULL,
                        curr_sym_table,
                        curr_token,
                        0,
                        name_space);
                else if(match_time==1)
                    curr_ast=creat_ast_node(curr_rule->type,
                        NULL,
                        curr_sym_table,
                        NULL,
                        sub_tree_ter_num,
                        name_space);
                break;
            default:
                m_error("lex.c","match","undefined type");
                exit(1);
                break;
        } 
    }
parser_succ:
    curr_ast->child=tmpv;
    curr_ast->child=shorten_tree(tmpv);
    tmpv=NULL;
    curr_ast=post_processing(curr_ast);
    if(curr_ast==NULL){
        have_error=true;
        goto parser_error;
    }
    bool inserted=false;
    bool need_insert=true;
    for(size_t i=0;i<VECLEN(curr_token->parser_token_mem);++i){
        AST_BASE* memtmpa=VEC_GET_ITEM(curr_token->parser_token_mem,i);
        if(memtmpa&&memtmpa->type==curr_ast->type){
            need_insert=false;
            if(curr_ast->ter_num>memtmpa->ter_num){
                curr_token->parser_token_mem->data[i]=curr_ast;
                inserted=true;   
            }
        }
    }
    if(inserted==false&&need_insert==true&&curr_ast->type>double_percent_colon)
        VECinsert(curr_token->parser_token_mem,(void*)curr_ast);
    goto final;
parser_error:
    if(have_error&&(curr_token->type!=end_of_file)){
        if(ce==C0001_ERR_NONE)
            ce=C0002_ERR_UNKNOWN;
        tei.ce=ce;
        tei.error_line=curr_token->line;
        tei.error_token_value=curr_token->value;
    }
    if(tmpv)
        DelVEC(tmpv);
    curr_ast=NULL;
final:
#ifdef _TEST_PARSER_
    for(int i=0;i<VECLEN(trace_stack);++i){
        printf("\t");
    }
    if(VECLEN(trace_stack)>0){
        printf("return to r:%s ",(char*)VEC_GET_ITEM(trace_name_stack,VECLEN(trace_name_stack)-2));
        if(curr_ast==NULL)
            printf("fail\n");
        else
            printf("succ\n");
    }
    VECpopback(trace_name_stack);
#endif
    VECpopback(trace_stack);
    return curr_ast;
}
bool p_symbol_table(AST_BASE* ast_node,SYM* father){
    /*
        As for this p_symbol_table function,emmm,actually I'm confused now
        The reason I use this function is that  \
         I find the when I use memory algorithm might lead to some error.
        The use of the symbol table is necessary for my work to record the typedef name and enumerator name —— so you cannot just ignore it
        I reuse some sub node to reduce the work——thet's the center idea of memory algorithm
        but the upper level node might build a new sym table which have no connect with the sub node's
        and that sym table is very hard to deal with——that makes the error in semantic stage.
        But after I build the AST, rebuild a symbol table tree is easy
        So I just clear the older one symbol table tree and rebuild a new
    */
#ifdef _TEST_PARSER_
    ast_total_nodes++;
#endif
    if(!ast_node)
        return false;
    enum rule_type_enum* tmp_ast_type=&(ast_node->type);
    VECinsert(trace_stack,(void*)(tmp_ast_type));
    bool have_create_symbol_table,build_sym_table;
    ast_node->symbol_table=parser_creater_symbol_table(father,
                        &have_create_symbol_table,
                        ast_node->type,
                        &build_sym_table);
    for(size_t i=0;i<AST_CHILD_NUM(ast_node);++i){
        if(!p_symbol_table(AST_GET_CHILD(ast_node,i),ast_node->symbol_table))
            return false;
    }
    ast_node=post_processing(ast_node);
    if(!ast_node)
    {
        return false;
    }
    if(IS_EXPR_NODE(ast_node->type)){    /*only the expr node have such a field to promise the operation safe*/
        ast_node->expr_attribute=m_alloc(sizeof(EXPR_NODE_ATTR));
        ast_node->expr_attribute->const_expr=false;
        ast_node->expr_attribute->data_field=m_alloc(sizeof(VALUE_DATA));
        m_memset(ast_node->expr_attribute->data_field,'\0',sizeof(VALUE_DATA));
        ast_node->expr_attribute->data_size=0;
        ast_node->expr_attribute->is_lvalue=false;
        ast_node->expr_attribute->type_vec=NULL;
        ast_node->expr_attribute->complete=false;
        ast_node->expr_attribute->is_bit_field=false;
    }
    else if(ast_node->type==initializer)
    {
        ast_node->init_attribute=m_alloc(sizeof(INIT_NODE_ATTR));
        ast_node->init_attribute->off=0;
        ast_node->init_attribute->size=0;
    }
    else if(ast_node->type==declaration)
    {
        ast_node->decl_attribute=m_alloc(sizeof(DECL_NODE_ATTR));
        ast_node->decl_attribute->decl_symbol_item_list=NULL;
    }
    else if(ast_node->type==function_definition)
        ast_node->func_attribute=NULL;
    VECpopback(trace_stack);
    return true;
}
/*scope and namespace*/
SYM* parser_creater_symbol_table(SYM* father,
    bool *have_create_symbol_table,
    enum rule_type_enum rule_type,
    bool *build_sym_table)
{
    if(!new_sym_table_rec[rule_type])
        return father;
    if(rule_type==translation_unit)
    {
        *have_create_symbol_table=true;
        return Create_symbol_table(father,SPT_FILE);
    }
    else if(rule_type==function_definition){
        *have_create_symbol_table=true;
        SYM* tmpsym= Create_symbol_table(father,SPT_FUN);
        tmpsym->st_attr_type[2]=tmpsym->st_attr_type[2]+1;
        tmpsym->st_attr=m_alloc(sizeof(SYMBOL_TABLE_FUNC_ATTR));
        SYMBOL_TABLE_FUNC_ATTR* tmpp=(SYMBOL_TABLE_FUNC_ATTR*)tmpsym->st_attr;
        tmpp->function_symbol=NULL;
        tmpp->have_ret=false;
        return tmpsym;
    }
    else if(rule_type==declaration){
        if(father->sp_type==SPT_FUN){
            if(father&&VECLEN(father->child_table)>0){
                *have_create_symbol_table=false;
                return (SYM*)VEC_GET_ITEM(father->child_table,0);
            }
            else{
                *have_create_symbol_table=true;
                return Create_symbol_table(father,SPT_BLOCK);
            }
        }
    }
    else if(rule_type==compound_stmt){
        if(father->sp_type==SPT_FUN){
            if(father&&VECLEN(father->child_table)>0){
                *have_create_symbol_table=false;
                return (SYM*)VEC_GET_ITEM(father->child_table,0);
            }
        }
        *have_create_symbol_table=true;
        return Create_symbol_table(father,SPT_BLOCK);
    }
    else if(rule_type==if_stmt \
        ||rule_type==switch_stmt \
        ||rule_type==while_stmt \
        ||rule_type==do_stmt \
        ||rule_type==for_stmt)
    {
        *have_create_symbol_table=true;
        *build_sym_table=true;
        SYM* tmpsym= Create_symbol_table(father,SPT_BLOCK);
        if(rule_type==switch_stmt)
        {
            tmpsym->st_attr_type[0]=tmpsym->st_attr_type[0]+1;
            tmpsym->st_attr=m_alloc(sizeof(SYMBOL_TABLE_SWITCH_ATTR));
            SYMBOL_TABLE_SWITCH_ATTR* tmpp=(SYMBOL_TABLE_SWITCH_ATTR*)tmpsym->st_attr;
            tmpp->have_default=false;
            tmpp->switch_value=InitVEC(DEFAULT_CAPICITY);
        }
        else if(rule_type==while_stmt||rule_type==do_stmt||rule_type==for_stmt)
        {
            tmpsym->st_attr_type[1]=tmpsym->st_attr_type[1]+1;
        }
        return tmpsym;
    }
    else if(rule_type==statement&&(*build_sym_table)){
        *have_create_symbol_table=true;
        *build_sym_table=false;
        return Create_symbol_table(father,SPT_BLOCK);
    }
    else if(rule_type==parameter_type_list||rule_type==identifier_list){
        bool have_function_def=false;
        for(int i=VECLEN(trace_stack)-1;i>=0;i--){
            enum rule_type_enum tmpr_type=*(enum rule_type_enum*)VEC_GET_ITEM(trace_stack,i);
            if(tmpr_type==function_definition)
            {
                have_function_def=true;
                break;
            }
            if(tmpr_type==declaration)
            {
                have_function_def=false;
                break;
            }
        }
        if(!have_function_def){
            *have_create_symbol_table=true;
            return Create_symbol_table(father,SPT_FUN_PROTOTYPE);
        }
        else{
            *have_create_symbol_table=true;
            return Create_symbol_table(father,SPT_BLOCK);
        }
    }
    return father;
}
unsigned int parser_node_name_space(enum rule_type_enum rule_type,
    VEC* token_list,
    size_t token_index,
    NMSP name_space,
    SYM* curr_sym_table)
{
    if(rule_type==function_definition){ /*function_def namespace for label*/
        curr_sym_table->name_space=GET_NEW_NAMESPACE();
        return NMSP_DEFAULT;
    }
    else if(rule_type==struct_decl_list){
        return GET_NEW_NAMESPACE();
    }
    else if(rule_type==labeled_stmt)
    {
        goto label_case;
    }
    if(((TOKEN*)VEC_GET_ITEM(token_list,token_index))->type==identifier){
        if(token_index>=1){
            TOKEN* tmpt=(TOKEN*)VEC_GET_ITEM(token_list,token_index-1);
            if(tmpt->type==KW_struct \
                ||tmpt->type==KW_enum \
                ||tmpt->type==KW_union)
            {   /*a union struct enum tag*/
                return NMSP_SU_TAG;
            }
            else if(tmpt->type==dot||tmpt->type==point){    /*a quote of union struct member*/
                return NMSP_UNKNOWN;
            }
            else if(tmpt->type==KW_goto){ /*label case*/
                goto label_case;
            }
        }
    }
    if(name_space>3){   /*a definition of union struct member*/
        if(rule_type==statement)
            return NMSP_DEFAULT;
        return name_space;
    }
    return NMSP_DEFAULT;
label_case:
    while(curr_sym_table->father!=NULL)
    {
        if(curr_sym_table->sp_type==SPT_FUN){
            break;
        }
        curr_sym_table=curr_sym_table->father;
    }
    return curr_sym_table->name_space;
}
VEC* shorten_tree(VEC* v)
{
    if(v==NULL||VECLEN(v)==0)
        return NULL;
    VEC* resv=InitVEC(DEFAULT_CAPICITY);
    AST_BASE* sub_ast_node,*sub_sub_ast_node;
    for(size_t i=0;i<VECLEN(v);++i){
        sub_ast_node=(AST_BASE*)VEC_GET_ITEM(v,i);
        if(!sub_ast_node)
            continue;
        /*sub_ast_node->child=shorten_tree(sub_ast_node->child);*/  /*the child node must have been shorten, so no need to do this*/
        if(sub_ast_node->type==0)
        {
            for(size_t j=0;j<AST_CHILD_NUM(sub_ast_node);++j)
            {
                sub_sub_ast_node=(AST_BASE*)AST_GET_CHILD(sub_ast_node,j);
                if(sub_sub_ast_node)
                    VECinsert(resv,(void*)sub_sub_ast_node);
            }
            DelVEC(sub_ast_node->child);
            m_free(sub_ast_node);
        }
        else if(IS_EXPR_NODE(sub_ast_node->type)&&AST_CHILD_NUM(sub_ast_node)==1)
        {
            AST_BASE* sub_sub_ast_node=AST_GET_CHILD(sub_ast_node,0);
            if(sub_sub_ast_node&&IS_EXPR_NODE(sub_sub_ast_node->type)){
                VECinsert(resv,(void*)sub_sub_ast_node);
                /*here should not delete the node, for memory might reuse*/
            }
            else
                VECinsert(resv,(void*)sub_ast_node);
        }
        else{
            VECinsert(resv,(void*)sub_ast_node);
        }
    }
    DelVEC(v);
    return resv;
}
bool is_enum_const(char* symbol,SYM* curr_sym_table)
{
    HASH_ITEM* tmphi=NULL;
    SYM_ITEM* find_item=Create_symbol_item(symbol,NMSP_UNKNOWN);
    while(curr_sym_table){
        tmphi=HASHFind(curr_sym_table->enum_const_table,
                find_item,
                symbol_item_cmp,
                false,
                false);
        if(HASH_ITEM_EXIST(tmphi))
            return true;
        curr_sym_table=curr_sym_table->father;
    }
    return false;
}