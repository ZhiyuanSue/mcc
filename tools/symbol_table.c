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
    if(father&&father->typedef_name_table){
        symbol_table->typedef_name_table=InitHASH();
        HASHCOPY(symbol_table->typedef_name_table,father->typedef_name_table,symbol_item_cmp);
    }
    else
        symbol_table->typedef_name_table=InitHASH();
    symbol_table->enum_const_table=InitHASH();
    if(father)
    {
        symbol_table->st_attr_type[0]=father->st_attr_type[0];
        symbol_table->st_attr_type[1]=father->st_attr_type[1];
        symbol_table->st_attr_type[2]=father->st_attr_type[2];
    }
    else
    {   
        symbol_table->st_attr_type[0]=0;
        symbol_table->st_attr_type[1]=0;
        symbol_table->st_attr_type[2]=0;
    }
    symbol_table->switch_begin_bb=NULL;
    symbol_table->loop_begin_bb=NULL;
    symbol_table->loop_end_bb=NULL;
    symbol_table->switch_end_bb=symbol_table->func_end_bb=NULL;
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
    tmp->stor_type=IR_STOR_NONE;
    tmp->data_size=0;
    tmp->data_field=(VALUE_DATA*)m_alloc(sizeof(VALUE_DATA));
    tmp->is_lvalue=false;
    tmp->complete=false;
    tmp->is_bit_field=false;
    m_memset(tmp->data_field,'\0',sizeof(VALUE_DATA));
    tmp->type_vec=InitVEC(DEFAULT_CAPICITY);
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
void del_symbol_typedef_table(SYM* symbol_table,char* symbol){
    SYM_ITEM* find_item=Create_symbol_item(symbol,NMSP_UNKNOWN);
    HASH_ITEM* del_item=HASHFind(symbol_table->typedef_name_table,find_item,symbol_item_cmp,false,true);
    HASHDELITEM(symbol_table->typedef_name_table,del_item,symbol_item_cmp);
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
    symbol_vec=get_symbol_hash(symbol_table->sym_hash_table);
    printf("<symbol table have %ld symbols>\n",VECLEN(symbol_vec));
    for(size_t i=0;i<VECLEN(symbol_vec);++i){
        tmp=VEC_GET_ITEM(symbol_vec,i);
        if(!tmp||tmp->count<=0)
            continue;
        print_symbol(tmp,symbol_table->level);
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
void print_symbol(SYM_ITEM* symbol,size_t indentation)
{
    if(!symbol)
        return;
    for(int j=0;j<=indentation+1;++j){
        printf("    ");
    }
    printf("<symbol:%s ns:%d linkage:%d>\n",symbol->value,symbol->name_space,symbol->linkage);
    for(int j=0;j<=indentation+2;++j){
        printf("    ");
    }
    printf("type::<");
    M_TYPE* tmpfunct=NULL;
    M_TYPE* tmpt=NULL;
    if(symbol->type_vec){
        for(int j=VECLEN(symbol->type_vec)-1;j>=0;--j){
            tmpt=VEC_GET_ITEM(symbol->type_vec,j);
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
    
    if(symbol->const_expr)
    {
        for(int j=0;j<=indentation+2;++j){
            printf("    ");
        }
        printf("a const expr symbol\n");
    }
    tmpt=Type_VEC_get_actual_base_type(symbol->type_vec);
    if(tmpfunct){
        VEC* paravec=((TP_FUNC*)tmpfunct)->func_parameters;
        if(paravec==NULL||VECLEN(paravec)==0){
            for(int k=0;k<=indentation+3;++k){
                printf("    ");
            }
            printf("no parameter\n");
            return;
        }
        for(int j=0;j<=indentation+3;++j){
            printf("    ");
        }
        printf("function have %ld parameters are:\n",VECLEN(paravec));
        for(size_t j=0;j<VECLEN(paravec);++j){
            TP_FUNC_PARA* tmpfuncpara=VEC_GET_ITEM(paravec,j);
            for(int k=0;k<=indentation+4;++k){
                printf("    ");
            }
            if(tmpfuncpara->para_name)
                printf("%s\n",tmpfuncpara->para_name);
            else
                printf("(no parameter name)\n");
        }
    }
    else if(tmpt&&tmpt->modifier==false){
        if(symbol->name_space==NMSP_SU_TAG&&(tmpt->typ_category==TP_UNION||tmpt->typ_category==TP_STRUCT)){
            for(int j=0;j<=indentation+3;++j){
                printf("    ");
            }
            printf("total size:%ld align size:%ld\n",((TP_SU*)tmpt)->total_data_size,((TP_SU*)tmpt)->data_align);
            if(((TP_SU*)tmpt)->members==NULL)
                return;
            for(int j=0;j<=indentation+3;++j){
                printf("    ");
            }
            printf("struct have %ld members are:\n",VECLEN(((TP_SU*)tmpt)->members));
            for(size_t j=0;j<VECLEN(((TP_SU*)tmpt)->members);++j){
                TP_SU_MEMBER* tmpm=VEC_GET_ITEM(((TP_SU*)tmpt)->members,j);
                if(tmpm){
                    for(int k=0;k<=indentation+4;++k){
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
        }
        if(tmpt->typ_category==TP_ARRAY)
        {
            for(int j=0;j<=indentation+3;++j){
                printf("    ");
            }
            printf("array len is: %zu\n",((TP_ARR*)tmpt)->axis_size);
        }
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