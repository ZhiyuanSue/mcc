#include "extern_def_semantics.h"
extern VEC* c_error;
extern char* filename;
extern size_t type_data_size[TYPE_NUM];
#ifdef _TEST_SEMANTICS_
extern int semantics_level;
#endif
bool function_definition_type(AST_BASE* ast_node)
{
    if(!ast_node||ast_node->type!=function_definition)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    int declaration_cnt=0;
    bool typedef_declaration=false;
    VEC* type_vec=NULL;
    char* declarator_char_name;
    AST_BASE* declaration_spec_node=AST_GET_CHILD(ast_node,0);
    AST_BASE* declarator_node=AST_GET_CHILD(ast_node,1);
    AST_BASE* compound_stmt_node=VEC_BACK(ast_node->child);
    VEC* declaration_list_opt=InitVEC(DEFAULT_CAPICITY);
    for(size_t i=2;i<AST_CHILD_NUM(ast_node)-1;++i)
        VECinsert(declaration_list_opt,(void*)AST_GET_CHILD(ast_node,i));
    if((type_vec=declaration_spec_qual_list_type(
        declaration_spec_node,
        &declaration_cnt,
        &typedef_declaration,
        true))==NULL)
        goto error;
    M_TYPE* tmpt=Type_VEC_get_spec_other(type_vec);
    if(tmpt&&tmpt->typ_stor!=TP_STATIC&&tmpt->typ_stor!=TP_EXTERN&&tmpt->typ_stor!=TP_STOR_NONE)
    {
        C_ERROR(C0084_ERR_FUNC_STORAGE,declaration_spec_node);
        goto error;
    }
    if(declarator_type(declarator_node,
        type_vec,
        &declaration_cnt,
        &typedef_declaration,
        declarator_node->name_space,
        &declarator_char_name
    )==false)
        goto error;
    
    SYM_ITEM* tmpsi=find_symbol_curr_table(ast_node->symbol_table,declarator_char_name,declarator_node->name_space);
    /*check the old styles and build the type for identifiers*/
    VEC* symbol_item_type=tmpsi->type_vec;
    tmpt=Type_VEC_get_actual_base_type(symbol_item_type);
    if(tmpt->typ_category!=TP_FUNCTION){
        C_ERROR(C0044_ERR_FUNC_DEF_DECL_TYPE,ast_node);
        goto error;
    }
    if(((TP_FUNC*)tmpt)->is_old_style)
    {
        VEC* func_para_list=((TP_FUNC*)tmpt)->func_parameters;
        for(size_t i=0;i<VECLEN(declaration_list_opt);++i)
        {
            AST_BASE* para_declaration_node=VEC_GET_ITEM(declaration_list_opt,i);
            VEC* para_declarator_list=InitVEC(DEFAULT_CAPICITY);
            if(!declaration_type(para_declaration_node,para_declarator_list))
                goto error;
            for(size_t j=0;j<VECLEN(para_declarator_list);++j)
            {
                SYM_ITEM* tmp_dec_si=VEC_GET_ITEM(para_declarator_list,j);
                M_TYPE* tmp_dec_t=Type_VEC_get_spec_other(tmp_dec_si->type_vec);
                if(tmp_dec_t&&tmp_dec_t->typ_stor!=TP_STOR_NONE&&tmp_dec_t->typ_stor!=TP_REGISTER)
                {
                    C_ERROR(C0087_ERR_FUNC_PARA_OLD_STOR,para_declaration_node);
                    goto error;
                }
                if(tmp_dec_t&&tmp_dec_t->align_spec>0)
                {
                    C_ERROR(C0089_ERR_ALIGN_CANNOT_SPECIFIE,para_declaration_node);
                    goto error;
                }
                bool find=false;
                if(tmp_dec_si)
                {
                    /*adjustment*/
                    tmp_dec_t=Type_VEC_get_actual_base_type(tmp_dec_si->type_vec);
                    if(tmp_dec_t->typ_category==TP_ARRAY){
                        VECpopback(tmp_dec_si->type_vec);
                        M_TYPE* tmp_pointer=build_base_type(TP_POINT);
                        VECinsert(tmp_dec_si->type_vec,(void*)tmp_pointer);
                        VEC* arr_type_vec=(VEC*)(((TP_ARR*)tmp_dec_t)->axis_modify);
                        if(arr_type_vec&&VECLEN(arr_type_vec)){
                            M_TYPE* arr_modify_type=VEC_GET_ITEM(arr_type_vec,0);
                            VECinsert(tmp_dec_si->type_vec,(void*)arr_modify_type);
                        }
                    }
                    else if(tmp_dec_t->typ_category==TP_FUNCTION){
                        M_TYPE* tmp_pointer=build_base_type(TP_POINT);
                        VECinsert(tmp_dec_si->type_vec,(void*)tmp_pointer);
                    }
                    /*after adjustment,check incomplete type*/
                    tmp_dec_t=Type_VEC_get_actual_base_type(tmp_dec_si->type_vec);
                    if(tmp_dec_t&&tmp_dec_t->complete==false){
                        C_ERROR(C0045_ERR_FUNC_PARA_INCOMPLETE_TYPE,declarator_node);
                        goto error;
                    }
                    for(size_t k=0;k<VECLEN(func_para_list);++k)
                    {
                        TP_FUNC_PARA* tmp_para=VEC_GET_ITEM(func_para_list,k);
                        if(strcmp(tmp_dec_si->value,tmp_para->para_name)==0&&tmp_para->type_vec==NULL)
                        {
                            tmp_para->type_vec=tmp_dec_si->type_vec;
                            find=true;
                            break;
                        }
                    }
                }
                if(!find)
                {
                    C_ERROR(C0085_ERR_FUNC_PARA_OLD_NOT_FIND,para_declaration_node);
                    goto error;
                }
            }
        }
        for(size_t i=0;i<VECLEN(func_para_list);++i)
        {
            TP_FUNC_PARA* tmp_para=VEC_GET_ITEM(func_para_list,i);
            if(tmp_para->type_vec==NULL)
            {
                C_ERROR(C0086_ERR_FUNC_PARA_OLD_NOT_DEC,declarator_node);
                goto error;
            }
        }
    }
    else    /*if parameter is not an old style,but still have declaration list, it's an error*/
    {
        if(VECLEN(declaration_list_opt)>0)
        {
            C_ERROR(C0082_ERR_FUNC_NO_ID_LIST_BUT_DEC,ast_node);
            goto error;
        }
    }
    /*finish check old style*/
    VEC* tmpv=Type_VEC_get_func_return_type(symbol_item_type,true);
    tmpt=Type_VEC_get_actual_base_type(tmpv);
    if(tmpt->typ_category==TP_ARRAY)
    {
        C_ERROR(C0043_ERR_FUN_RETURN_TYPE,ast_node);
        goto error;
    }
    /*the symbol item must be insert to the upper file scope, the false should not happen,so no error tips*/
    SYM_ITEM* findsi=NULL;
    bool should_insert=true;
    if(ast_node->father)
        findsi=find_symbol_curr_table(ast_node->father->symbol_table,declarator_char_name,declarator_node->name_space);
    if(HASH_ITEM_EXIST(findsi))
    {
        if(!findsi->declared)
            findsi->declared=true;
        else{
            if(!findsi->defined)
                findsi->defined=true;
            else
            {
                C_ERROR(C0005_ERR_REDEFINE,declarator_node);
                goto error;
            }
            /*TODO:the insert should like declarator type ,check the parameter and (if necessery) build composite function type*/
            if(compatible_types(findsi->type_vec,type_vec))
            {
                should_insert=false;
                findsi->type_vec=composite_types(findsi->type_vec,type_vec,true);
            }
            else{
                C_ERROR(C0069_ERR_NEED_COMPATIBLE_TYPE,declarator_node);
                goto error;
            }
        }
    }
    tmpsi->declared=tmpsi->defined=true;
    ((SYMBOL_TABLE_FUNC_ATTR*)(ast_node->symbol_table->st_attr))->function_symbol=tmpsi;
    SYM_ITEM* newsi=Create_symbol_item(declarator_char_name,declarator_node->name_space);
    if(!Copy_sym_item(newsi,tmpsi))
        goto error;
    del_symbol(ast_node->symbol_table,declarator_char_name,declarator_node->name_space);
    if(should_insert)
    {
        SYM* father_table=ast_node->symbol_table->father;
        if(!insert_symbol(father_table,newsi))
            goto error;
    }
    /*start compound statement*/
    if(!compound_statement(compound_stmt_node))
        goto error;
    ast_node->func_attribute=tmpsi;
    m_free(tei);
    return true;
error:
    return false;
}