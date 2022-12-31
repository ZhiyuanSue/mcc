#include "symbol_table.h"
static const char* scope_type_str[5]={
    "no_scope_type",
    "block_scope",
    "function_scope",
    "file_scope",
    "function_prototype_scope"
};
SYM* Create_symbol_table(SYM* father,enum scope_type type){
    SYM* symbol_table=(SYM*)m_alloc(sizeof(SYM));
    symbol_table->father=father;
    if(father!=NULL){
        VECinsert(father->child_table,(void*)symbol_table);
    }
    if(father==NULL)
        symbol_table->level=0;
    else
        symbol_table->level=father->level+1;
    symbol_table->child_table=InitVEC(DEFAULT_CAPICITY);
    symbol_table->sym_hash_table=InitHASH();
    symbol_table->sp_type=type;
    symbol_table->name_space=0;
    symbol_table->typedef_name_table=InitHASH();
    symbol_table->enum_const_table=InitHASH();
    if(father)
        symbol_table->st_attr_type=father->st_attr_type;
    else
        symbol_table->st_attr_type=SA_ATTR_NONE;
    symbol_table->st_attr=NULL;
    return symbol_table;
}
SYM_ITEM* Create_symbol_item(char* symbol,NMSP name_space){
    if(symbol==NULL)
        return NULL;
    SYM_ITEM* tmp=(SYM_ITEM*)m_alloc(sizeof(SYM_ITEM));
    tmp->value=symbol;
    tmp->name_space=name_space;
    tmp->key=SymbolCharToKey(symbol,name_space);
    tmp->count=HASH_CNT_FD;
    tmp->declared=tmp->defined=false;
    tmp->align_size=0;
    tmp->linkage=LKA_NONE;
    tmp->fspec_type=FSPEC_NONE;
    tmp->Thread_local=false;
    tmp->data_size=0;
    tmp->data_field=(VALUE_DATA*)m_alloc(sizeof(VALUE_DATA));
    m_memset(tmp->data_field,'\0',sizeof(VALUE_DATA));
    tmp->type_vec=InitVEC(DEFAULT_CAPICITY);
    tmp->init_value_ptr=NULL;
    tmp->const_expr=false;
    return tmp;
}
bool insert_symbol(SYM* symbol_table,SYM_ITEM* symbol_item){
    return HASHInsert(symbol_table->sym_hash_table,(HASH_ITEM*)symbol_item,symbol_item_cmp);
}
SYM_ITEM* find_symbol(SYM* symbol_table,char* symbol,NMSP name_space){
    if(!symbol_table||!symbol)
        return NULL;
    SYM_ITEM* item;
    SYM_ITEM* find_item=Create_symbol_item(symbol,name_space);
    SYM* curr=symbol_table;
    while(curr&&(item=(SYM_ITEM*)HASHFind(curr->sym_hash_table,find_item,symbol_item_cmp,false,false))==NULL){
        curr=curr->father;
    }
    m_free(find_item);
    return item;
}
SYM_ITEM* find_symbol_curr_table(SYM* symbol_table,char* symbol,NMSP name_space)
{
    if(!symbol_table||!symbol)
        return NULL;
    SYM_ITEM* find_item=Create_symbol_item(symbol,name_space);
    SYM_ITEM* item=(SYM_ITEM*)HASHFind(symbol_table->sym_hash_table,find_item,symbol_item_cmp,false,false);
    m_free(find_item);
    return item;
}
void del_symbol(SYM* symbol_table,char* symbol,NMSP name_space){
    SYM_ITEM* find_item=Create_symbol_item(symbol,name_space);
    HASHFind(symbol_table->sym_hash_table,find_item,symbol_item_cmp,true,false);
    m_free(find_item);
}
void Del_symbol_table(SYM* symbol_table){
    if(!symbol_table)
        return;
    DelHASH(symbol_table->sym_hash_table);
    DelHASH(symbol_table->typedef_name_table);
    DelHASH(symbol_table->enum_const_table);
    DelVEC(symbol_table->child_table);
    m_free(symbol_table);
}
void print_symbol_table(SYM* symbol_table)  /*just a print for test,don't care too much about it*/
{
    VEC* symbol_vec=NULL;
    SYM_ITEM* tmp=NULL;
    if(symbol_table==NULL)
    {
        printf("no symbol table\n");
        return;
    }
    for(int i=0;i<symbol_table->level;++i){
        printf("    ");
    }
    printf("scope type:%s with child %ld and namespace %d\n",scope_type_str[symbol_table->sp_type],VECLEN(symbol_table->child_table),symbol_table->name_space);
#if PRINT_SYMBOLE_TABLE==1
    for(int i=0;i<=symbol_table->level;++i){
        printf("    ");
    }
    printf("<symbol table>\n");
    symbol_vec=get_symbol_hash(symbol_table->sym_hash_table);
    for(size_t i=0;i<VECLEN(symbol_vec);++i){
        
        tmp=VEC_GET_ITEM(symbol_vec,i);
        if(!tmp||tmp->count<=0)
            continue;
        for(int j=0;j<=symbol_table->level+1;++j){
            printf("    ");
        }
        printf("<symbol:%s ns:%d linkage:%d>\n",tmp->value,tmp->name_space,tmp->linkage);
        if(tmp->init_value_ptr) /*this symbol have init value, print it*/
        {
            for(int j=0;j<=symbol_table->level+2;++j){
                printf("    ");
            }
            printf("this symbol have init value:");
            INIT_VALUE* tmp_init_value_ptr=tmp->init_value_ptr;
            /*as we don't care about the type, just print in hex format*/
            printf("0x");
            for(size_t j=0;j<tmp_init_value_ptr->data_len;++j){
                printf("%X ",tmp_init_value_ptr->data[j]);
            }
            printf("\n");
        }
        for(int j=0;j<=symbol_table->level+2;++j){
            printf("    ");
        }
        printf("type::<");
        M_TYPE* tmpfunct=NULL;
        M_TYPE* tmpt=NULL;
        
        if(tmp->type_vec){
            for(int j=VECLEN(tmp->type_vec)-1;j>=0;--j){
                tmpt=VEC_GET_ITEM(tmp->type_vec,j);
                if(tmpt){
                    if(tmpfunct==NULL&&tmpt->typ_category==TP_FUNCTION)
                        tmpfunct=tmpt;
                    print_type(tmpt);
                }
                if(j)
                    printf(",");
            }
        }
        printf(">\n");
        tmpt=Type_VEC_get_actual_base_type(tmp->type_vec);
        if(tmpfunct){
            VEC* paravec=((TP_FUNC*)tmpfunct)->func_parameters;
            if(paravec==NULL||VECLEN(paravec)==0){
                for(int k=0;k<=symbol_table->level+3;++k){
                    printf("    ");
                }
                printf("no parameter\n");
                continue;
            }
            for(int j=0;j<=symbol_table->level+3;++j){
                printf("    ");
            }
            printf("function have %ld parameters are:\n",VECLEN(paravec));
            for(size_t j=0;j<VECLEN(paravec);++j){
                TP_FUNC_PARA* tmpfuncpara=VEC_GET_ITEM(paravec,j);
                for(int k=0;k<=symbol_table->level+4;++k){
                    printf("    ");
                }
                if(tmpfuncpara->para_name)
                    printf("%s\n",tmpfuncpara->para_name);
                else
                    printf("(no parameter name)\n");
            }
        }
        else if(tmpt&&tmpt->modifier==false){
            if(tmp->name_space==NMSP_SU_TAG&&(tmpt->typ_category==TP_UNION||tmpt->typ_category==TP_STRUCT)){
                if(((TP_SU*)tmpt)->members==NULL)
                    continue;
                for(int j=0;j<=symbol_table->level+3;++j){
                    printf("    ");
                }
                printf("struct have %ld members are:\n",VECLEN(((TP_SU*)tmpt)->members));
                for(size_t j=0;j<VECLEN(((TP_SU*)tmpt)->members);++j){
                    TP_SU_MEMBER* tmpm=VEC_GET_ITEM(((TP_SU*)tmpt)->members,j);
                    if(tmpm){
                        for(int k=0;k<=symbol_table->level+4;++k){
                            printf("    ");
                        }
                        if(tmpm->member_name)
                        {
                            if(tmpm->bit_field)
                                printf("< %s off:%ld bit_off:%ld bit_size:%ld >\n",tmpm->member_name,tmpm->offset,tmpm->bit_field_offset,tmpm->bit_field_size);
                            else
                                printf("< %s off:%ld >\n",tmpm->member_name,tmpm->offset);
                        }
                        else
                        {
                            if(tmpm->bit_field)
                                printf("< anonymous member off:%ld bit_off:%ld bit_size:%ld >\n",tmpm->offset,tmpm->bit_field_offset,tmpm->bit_field_size);
                            else
                                printf("< anonymous member off:%ld >\n",tmpm->offset);
                        }
                    }
                }
                for(int j=0;j<=symbol_table->level+3;++j){
                    printf("    ");
                }
                printf("total size:%ld align size:%ld\n",((TP_SU*)tmpt)->total_data_size,((TP_SU*)tmpt)->data_align);
            }
            if(tmpt->typ_category==TP_ARRAY)
            {
                for(int j=0;j<=symbol_table->level+3;++j){
                    printf("    ");
                }
                printf("array len is: %lld\n",((TP_ARR*)tmpt)->axis_size);
            }
        }
    }
#endif
#if PRINT_TYPEDEF_TABLE == 1
    for(int i=0;i<=symbol_table->level;++i){
        printf("    ");
    }
    printf("<typedef table>\n");
    symbol_vec=get_symbol_hash(symbol_table->typedef_name_table);
    for(int i=0;i<VECLEN(symbol_vec);++i){
        for(int j=0;j<=symbol_table->level+1;++j){
            printf("    ");
        }
        tmp=VEC_GET_ITEM(symbol_vec,i);
        if(!tmp)
            continue;
        printf("<symbol:%s>\n",tmp->value);
    }
#endif
    for(size_t i=0;i<VECLEN(symbol_table->child_table);++i){
        SYM* child=(SYM*)VEC_GET_ITEM(symbol_table->child_table,i);
        if(child!=NULL)
            print_symbol_table(child);
    }
}
VEC* get_symbol_hash(HASH* h){
    VEC* symbol_vec=InitVEC(DEFAULT_CAPICITY);
    SYM_ITEM* tmp;
    for(size_t i=0;i<h->capicity;++i){
        if((h->data[i])){
            tmp=(SYM_ITEM*)HASH_GET_ITEM(h,i);
            VECinsert(symbol_vec,(void*)tmp);
        }
    }
    return symbol_vec;
}
unsigned long long int SymbolCharToKey(char* symbol,NMSP name_space){
    if(!symbol)
        return name_space;
    unsigned long long int res=name_space;
    while(*symbol!='\0'){
        res*=26;
        res+=(*symbol)-'\0';
        symbol++;
    }
    return res;
}
bool symbol_item_cmp(void* a,void* b){
    if(((SYM_ITEM*)a)->name_space==((SYM_ITEM*)b)->name_space&&strcmp(((SYM_ITEM*)a)->value,((SYM_ITEM*)b)->value)==0){
        return true;
    }
    else
        return false;
}