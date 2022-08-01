#include "decl_semantics_module.h"
extern VEC* c_error;
extern char* filename;
extern size_t type_data_size[TYPE_NUM];
#ifdef _TEST_SEMANTICS_
extern int semantics_level;
#endif
TP_SU* struct_union_type(AST_BASE* struct_decl_list_node,
    AST_BASE* tag_node,
    enum TP_CATEGORY struct_union)
{
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start struct union\n");
#endif
    if(!struct_decl_list_node||struct_decl_list_node->type!=struct_decl_list)
        return NULL;
    char* tag=(tag_node)?(tag_node->token->value):NULL;
    TP_SU* su_t=NULL;
    ERROR_ITEM* tei=(ERROR_ITEM*)m_alloc(sizeof(ERROR_ITEM));
    SYM_ITEM* find_tmpsi=NULL;
    SYM_ITEM* tmpsi=NULL;
    /*step one*/
    VEC* tmp_su_member_vec=InitVEC(DEFAULT_CAPICITY);
    VEC* su_member_vec=InitVEC(DEFAULT_CAPICITY);
    size_t struct_total_size=0;
    size_t struct_align_size=0;
    bool reset_bit_field_offset=false;
    bool can_be_modified_lvalue=true;
    for(size_t i=0;i<AST_CHILD_NUM(struct_decl_list_node);++i){
        VEC* type_vec=NULL;
        int declaration_cnt=0;
        bool typedef_declaration=false; /*no storage class,it always be false*/
        AST_BASE* struct_decl_node=AST_GET_CHILD(struct_decl_list_node,i);
        AST_BASE* spec_qual_list_node=AST_GET_CHILD(struct_decl_node,0);
        if(spec_qual_list_node->type==static_assert_declaration){
            /*if a static assert, thers's only one child node
            even though I don't known what will happen of a static assert in a struct*/
            if(!static_assert_type(spec_qual_list_node))
                return NULL;
            continue;
        }
        type_vec=declaration_spec_qual_list_type(spec_qual_list_node,
            &declaration_cnt,
            &typedef_declaration,
            true
        );
        if(type_vec==NULL)
        {
            return NULL;
        }
        AST_BASE* su_declarator_list_node=AST_GET_CHILD(struct_decl_node,1);
        if(su_declarator_list_node->type==semi_colon){ 
            /*no need to check the declaration_cnt or typedef_declaration,promised they have at least one*/
            su_declarator_list_node=NULL;
            TP_SU_MEMBER* su_member=m_alloc(sizeof(TP_SU_MEMBER));
            su_member->member_name=NULL;
            su_member->name_space=spec_qual_list_node->name_space;
            su_member->type_vec=type_vec;
            su_member->bit_field=false;
            su_member->bit_field_size=0;
            su_member->bit_field_offset=0;
            if(struct_union==TP_UNION){
                su_member->offset=0;
                struct_total_size=max(struct_total_size,Type_size(type_vec));
            }
            else if(struct_union==TP_STRUCT){
                su_member->offset=MCC_ALIGN(struct_total_size,VALID_ALIGN(type_vec));
                struct_total_size=su_member->offset+Type_size(type_vec);
            }
            struct_align_size=max(struct_align_size,Type_align(type_vec));
            VECinsert(tmp_su_member_vec,(void*)su_member);
            continue;
        }
        for(size_t j=0;j<AST_CHILD_NUM(su_declarator_list_node);j++){
            AST_BASE* su_declarator_node=AST_GET_CHILD(su_declarator_list_node,j);
            AST_BASE* declarator_node=AST_GET_CHILD(su_declarator_node,0);
            int tmp_declaration_cnt=declaration_cnt;
            TP_SU_MEMBER* su_member=m_alloc(sizeof(TP_SU_MEMBER));
            VEC* tmp_type_v=InitVEC(DEFAULT_CAPICITY);
            bool is_bit_field=false;
            if(AST_CHILD_NUM(su_declarator_node)>=2)
                is_bit_field=true;
            VECappend(type_vec,tmp_type_v);
            NMSP nmsp=declarator_node->name_space;
            char* declarator_char_name=NULL;
            if(declarator_node->type!=colon){
                if(!declarator_type(declarator_node,tmp_type_v,
                    &tmp_declaration_cnt,
                    &typedef_declaration,
                    nmsp,
                    &declarator_char_name
                ))
                    return false;
            }
            su_member->member_name=declarator_char_name;
            su_member->name_space=nmsp;
            su_member->type_vec=tmp_type_v;
            SYM_ITEM* tmpsi=find_symbol_curr_table(declarator_node->symbol_table,declarator_char_name,nmsp);
            if(tmpsi)
                tmpsi->linkage=LKA_NONE;
            /*TODO:design offset algorithm:DONE*/
            if(is_bit_field){   /*have bit-field*/ /*TODO:if a declarator with colon*/
                M_TYPE* tmpt=Type_VEC_get_spec_other(type_vec);
                if(tmpt&&tmpt->align_spec>0)
                {
                    C_ERROR(C0089_ERR_ALIGN_CANNOT_SPECIFIE,spec_qual_list_node);
                    return false;
                }
                /*TODO:check type,must be some kind of integer type.Besides, signed and unsigned int type should be same here:DONE*/
                enum TP_CATEGORY bit_field_base_type=TP_SPEC_NONE;
                int base_type_num=0;
                for(size_t i=0;i<VECLEN(tmp_type_v);++i){
                    M_TYPE* tmpt=VEC_GET_ITEM(tmp_type_v,i);
                    if(tmpt->modifier)
                        continue;
                    base_type_num++;
                    bit_field_base_type=tmpt->typ_category;
                }
                if(base_type_num!=1||IS_INT_TYPE(bit_field_base_type)==false)
                {
                    C_ERROR(C0022_ERR_BIT_FIELD_TYPE,su_declarator_node);
                    return false;
                }
                /*get bit-field length*/
                su_member->bit_field=true;
                AST_BASE* field_size_node=NULL;
                if(declarator_node->type!=colon)
                    field_size_node=AST_GET_CHILD(su_declarator_node,2);
                else
                    field_size_node=AST_GET_CHILD(su_declarator_node,1);
                bool const_expr=const_value(field_size_node);
                if(!const_expr)
                    return false;
                /*TODO:get the right const_base_type:DONE*/
                enum TP_CATEGORY const_base_type=(Type_VEC_get_actual_base_type(field_size_node->expr_attribute->type_vec))->typ_category;
                if(!IS_INT_TYPE(const_base_type)){
                    C_ERROR(C0024_ERR_BIT_FIELD_WIDTH_INT,su_declarator_node);
                    return false;
                }
                unsigned long long int bit_field_size=(TP_INT_CAST_TYPE(const_base_type,field_size_node->expr_attribute->data_field));    /*TODO:set the bit field size DONE*/
                if(bit_field_size<0){
                    C_ERROR(C0024_ERR_BIT_FIELD_WIDTH_INT,su_declarator_node);
                    return false;
                }
                if(bit_field_size==0&&declarator_node->type!=colon){
                    C_ERROR(C0026_ERR_BIT_FIELD_ZERO_WITH_DECLERATOR,su_declarator_node);
                    return false;
                }
                su_member->bit_field_size=(size_t)bit_field_size;
                if(su_member->bit_field_size>8*type_data_size[bit_field_base_type]){
                    C_ERROR(C0023_ERR_BIT_FIELD_WIDTH_EXCEED,su_declarator_node);
                    return false;
                }
                if(struct_union==TP_UNION){ /*for union ,no need to consider compress*/
                    su_member->offset=0;
                    struct_total_size=max(struct_total_size,Type_size(tmp_type_v));
                    su_member->bit_field_offset=0;
                }
                else if(struct_union==TP_STRUCT){
                    TP_SU_MEMBER* previous_su_member=NULL;
                    if(VECLEN(tmp_su_member_vec)>0)
                        previous_su_member=VEC_GET_ITEM(tmp_su_member_vec,VECLEN(tmp_su_member_vec)-1);
                    if(previous_su_member){
                        if(bit_field_size==0){  /*if no previous member,do nothing*/
                            reset_bit_field_offset=true;
                            struct_total_size=previous_su_member->offset+Type_size(previous_su_member->type_vec);;
                            DelVEC(tmp_type_v);
                            m_free(su_member);
                            continue;
                        }
                        if(previous_su_member->bit_field==false||reset_bit_field_offset){   /*the previous one is not a bit-field one or a unnamed bit-field with width 0*/
                            su_member->offset=MCC_ALIGN(struct_total_size,VALID_ALIGN(tmp_type_v));
                            struct_total_size=su_member->offset+Type_size(tmp_type_v);
                            su_member->bit_field_offset=0;
                            reset_bit_field_offset=false;
                        }
                        else{
                            bool bit_field_same_base_type=false;    /*check whether the type is the same*/
                            VEC* previous_type_v=previous_su_member->type_vec;
                            enum TP_CATEGORY pre_bit_field_base_type=TP_SPEC_NONE;
                            for(size_t i=0;i<VECLEN(previous_type_v);++i){
                                M_TYPE* tmpt=VEC_GET_ITEM(previous_type_v,i);
                                if(tmpt->modifier)
                                    continue;
                                pre_bit_field_base_type=tmpt->typ_category;
                            }
                            if(pre_bit_field_base_type==bit_field_base_type)
                                bit_field_same_base_type=true;
                            if(bit_field_same_base_type){   /*try to put into the pre one*/
                                if(previous_su_member->bit_field_offset+previous_su_member->bit_field_size+bit_field_size>8*type_data_size[bit_field_base_type])
                                {/*put it to the next unit*/
                                    su_member->offset=MCC_ALIGN(struct_total_size,VALID_ALIGN(tmp_type_v));
                                    struct_total_size=su_member->offset+Type_size(tmp_type_v);
                                    su_member->bit_field_offset=0;
                                }
                                else{/*put it to the previous unit*/
                                    su_member->offset=previous_su_member->offset;
                                    /*struct_total_size is unchanged*/
                                    su_member->bit_field_offset=previous_su_member->bit_field_offset+previous_su_member->bit_field_size;
                                }
                            }
                            else{   /*default is compress way*/
#if _BIT_FIELD_COMPRESS_==1
                                int next_unit_offset=MCC_ALIGN(struct_total_size,VALID_ALIGN(tmp_type_v));
                                int curr_unit_capicity=8*(next_unit_offset-previous_su_member->offset);
                                if(previous_su_member->bit_field_offset+previous_su_member->bit_field_size+bit_field_size>curr_unit_capicity){  /*put into the next unit*/
                                    su_member->offset=next_unit_offset;
                                    struct_total_size=su_member->offset+Type_size(tmp_type_v);
                                    su_member->bit_field_offset=0;
                                }
                                else{
                                    su_member->offset=next_unit_offset-VALID_ALIGN(tmp_type_v);
                                    su_member->bit_field_offset=max(0,previous_su_member->offset-su_member->offset)*8\
                                        +previous_su_member->bit_field_offset+previous_su_member->bit_field_size;
                                    struct_total_size=su_member->offset+MCC_ALIGN(su_member->bit_field_offset+bit_field_size,8)/8;
                                }
#else
                                su_member->offset=MCC_ALIGN(struct_total_size,VALID_ALIGN(tmp_type_v));
                                struct_total_size=su_member->offset+Type_size(tmp_type_v);
                                su_member->bit_field_offset=0;
#endif
                            }
                        }
                    }
                    else{   /*it's the first member*/
                        su_member->offset=0;
                        struct_total_size=Type_size(tmp_type_v);
                        su_member->bit_field_offset=0;
                    }
                }
                struct_align_size=max(struct_align_size,Type_align(tmp_type_v));
            }
            else{
                su_member->bit_field=false;
                su_member->bit_field_size=0;
                su_member->bit_field_offset=0;
                if(struct_union==TP_UNION){
                    su_member->offset=0;
                    struct_total_size=max(struct_total_size,Type_size(tmp_type_v));
                }
                else if(struct_union==TP_STRUCT){
                    su_member->offset=MCC_ALIGN(struct_total_size,VALID_ALIGN(tmp_type_v));
                    struct_total_size=su_member->offset+Type_size(tmp_type_v);
                }
                struct_align_size=max(struct_align_size,Type_align(tmp_type_v));
            }
            /*TODO:recursively change can_be_modified_lvalue:DONE*/
            M_TYPE* tmpt=Type_VEC_get_qual(su_member->type_vec);
            if(tmpt&&tmpt->type_qual>=8)
                can_be_modified_lvalue=false;
            tmpt=Type_VEC_get_actual_base_type(su_member->type_vec);
            if(tmpt&&(tmpt->typ_category==TP_STRUCT||tmpt->typ_category==TP_UNION))
            {
                if(((TP_SU*)tmpt)->can_be_modified_lvalue==false)
                    can_be_modified_lvalue=false;
            }
            VECinsert(tmp_su_member_vec,(void*)su_member);
            if(tmp_declaration_cnt==0&&!is_bit_field){
                ERROR_ITEM* twi=m_alloc(sizeof(ERROR_ITEM));
                C_WARN(W0003_WARN_NO_DECLARATION_ANY_THING,declarator_node);
            }
        }
    }
    struct_align_size=min(struct_align_size,_ALIGN_SIZE_(_MCC_ALIGN_PACK_));
    struct_total_size=MCC_ALIGN(struct_total_size,struct_align_size);
    /*step two*/
    for(size_t i=0;i<VECLEN(tmp_su_member_vec);++i){
        TP_SU_MEMBER* su_member=(TP_SU_MEMBER*)VEC_GET_ITEM(tmp_su_member_vec,i);
        /*check anonymous struct*/
        if(su_member->member_name==NULL){
            M_TYPE* su_member_base_type=Type_VEC_get_actual_base_type(su_member->type_vec);
            if(su_member_base_type->modifier==false \
                &&(su_member_base_type->typ_category==TP_UNION_STRUCT \
                ||su_member_base_type->typ_category==TP_UNION \
                ||su_member_base_type->typ_category==TP_STRUCT))
            {
                if(((TP_SU*)su_member_base_type)->tag==NULL){
                    /*find a anonymous struct/union*/
                    for(size_t j=0;j<VECLEN(((TP_SU*)su_member_base_type)->members);++j){
                        TP_SU_MEMBER* sub_su_member=VEC_GET_ITEM(((TP_SU*)su_member_base_type)->members,j);
                        for(size_t k=1;k<VECLEN(su_member->type_vec);++k){
                            VECinsert(sub_su_member->type_vec,VEC_GET_ITEM(su_member->type_vec,k));
                        }/*if there's some modify type,copy*/
                        sub_su_member->offset=sub_su_member->offset+su_member->offset;
                        /*TODO:design offset algorithm:DONE*/
                        find_tmpsi=Create_symbol_item(sub_su_member->member_name,struct_decl_list_node->name_space);
                        tmpsi=(SYM_ITEM*)HASHFind(
                            struct_decl_list_node->symbol_table->sym_hash_table,
                            find_tmpsi,
                            symbol_item_cmp,
                            false,
                            false
                        );
                        m_free(find_tmpsi);
                        if(tmpsi){
                            C_ERROR(C0015_ERR_REDEFINATION,struct_decl_list_node);
                            return NULL;
                        }
                        VECinsert(su_member_vec,(void*)sub_su_member);
                    }
                    continue;
                }
            }
        }
        VECinsert(su_member_vec,(void*)su_member);
    }
    bool flexible_array_type=false;
    enum scope_type curr_scope=struct_decl_list_node->symbol_table->sp_type;
    for(size_t i=0;i<VECLEN(su_member_vec);++i){
        /*TODO:check the incomplete type or function type:DONE*/
        /*TODO:flexable array type*/
        TP_SU_MEMBER* su_member=(TP_SU_MEMBER*)VEC_GET_ITEM(su_member_vec,i);
        M_TYPE* tmp_base_type=NULL;
        bool is_curr_member_flexible=false;
        tmp_base_type=Type_VEC_get_actual_base_type(su_member->type_vec);
        if(VECLEN(su_member_vec)==1)
            is_curr_member_flexible=false;
        else{
            if(i==VECLEN(su_member_vec)-1){
                if(tmp_base_type&&tmp_base_type->typ_category==TP_ARRAY&&((TP_ARR*)tmp_base_type)->complete==false){
                    is_curr_member_flexible=true;
                    flexible_array_type=true;
                }
            }
        }
        if(tmp_base_type){
            if((is_curr_member_flexible==false \
                &&tmp_base_type->complete==false) \
                ||tmp_base_type->typ_category==TP_FUNCTION){
                    C_ERROR(C0018_ERR_STRUCT_INCOMPLETE_FUNCTION_TYPE,struct_decl_list_node);
                    return NULL;
            }
        }
        else{
            ;   /*impossible,this error have judged at type spec node*/
        }
        /*I tried ,but cannot happen in any case*/
        SYM_ITEM* tmpsi=find_symbol_curr_table(struct_decl_list_node->symbol_table,su_member->member_name,su_member->name_space);
        if(!is_curr_member_flexible&&tmpsi&&Type_VEC_VM(su_member->type_vec)){
            if(tmpsi->linkage!=LKA_NONE
                ||(curr_scope!=SPT_BLOCK
                &&curr_scope!=SPT_FUN_PROTOTYPE))
            {
                C_ERROR(C0041_ERR_VM_ID,struct_decl_list_node);
                return NULL;
            }
        }
    }
    /*step three*/
    find_tmpsi=Create_symbol_item(tag,NMSP_SU_TAG);
    tmpsi=(SYM_ITEM*)HASHFind(
        struct_decl_list_node->symbol_table->sym_hash_table,
        find_tmpsi,
        symbol_item_cmp,
        false,
        false
    );
    m_free(find_tmpsi);
    bool tmpsi_exit=HASH_ITEM_EXIST(tmpsi);
    if(tmpsi_exit){
        if(tmpsi->defined){
            /*redefination error,if a struct_decl_list_node exits*/
            C_ERROR(C0015_ERR_REDEFINATION,struct_decl_list_node);
            return NULL;
        }
        else{
            su_t=(TP_SU*)Type_VEC_get_actual_base_type(tmpsi->type_vec);
            tag_node->token->symbol_item=tmpsi;
        }
    }
    else{
        su_t=(TP_SU*)build_base_type(struct_union);
        su_t->tag=tag;
        su_t->key=TypeToKey((M_TYPE*)su_t);
        VEC* su_type_vec=InitVEC(DEFAULT_CAPICITY);
        VECinsert(su_type_vec,(void*)su_t);
        find_tmpsi=Create_symbol_item(tag,NMSP_SU_TAG);
        if(find_tmpsi){ /*tag is no linkage*/
            find_tmpsi->type_vec=su_type_vec;
            find_tmpsi->defined=true;
            find_tmpsi->declared=true;
            find_tmpsi->count=HASH_CNT_IST;
            if(insert_symbol(struct_decl_list_node->symbol_table,find_tmpsi))
                tag_node->token->symbol_item=find_tmpsi;
        }
    }
    su_t->members=su_member_vec;
    su_t->total_data_size=struct_total_size;
    su_t->data_align=struct_align_size;
    su_t->complete=true;
    su_t->have_flex_array=flexible_array_type;
    su_t->can_be_modified_lvalue=can_be_modified_lvalue;
    m_free(tei);
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("finish struct union\n");
    semantics_level--;
#endif
    return su_t;
}
TP_ENUMERA* enum_type(AST_BASE* enumerator_list_node,AST_BASE* tag_node){
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start enum\n");
#endif
    if(!enumerator_list_node||enumerator_list_node->type!=enum_list)
        return NULL;
    /*deal with tag error*/
    char* tag=(tag_node)?(tag_node->token->value):NULL;
    TP_ENUMERA* enum_t=NULL;
    VEC* member_vec_tmp=InitVEC(DEFAULT_CAPICITY);
    ERROR_ITEM* tei=(ERROR_ITEM*)m_alloc(sizeof(ERROR_ITEM));
    SYM_ITEM* find_tmpsi=NULL;
    SYM_ITEM* tmpsi=NULL;
    /*deal with the enumarator*/
    signed int enum_value=0;    /*if no value ,the value of the first one is 0*/
    enum TP_STORAGE_SPEC storage_spec_type=TP_STOR_NONE;
    int type_qual_cnt[TYPE_QUAL_NUM]={1,0,0,0};
    int func_spec_cnt[FUNCTION_SPEC_NUM]={0,0};
    unsigned long int align_spec_cnt=0;
    M_TYPE* modify_type=build_modify_type(
        storage_spec_type,type_qual_cnt,func_spec_cnt,align_spec_cnt
    );
    M_TYPE* base_type=build_base_type(TP_SINT); /*a enumerator must be a const signed int*/
    VEC* const_int_type_vec=InitVEC(DEFAULT_CAPICITY);
    if(base_type){
        VECinsert(const_int_type_vec,(void*)base_type);
    }
    if(modify_type){
        VECinsert(const_int_type_vec,(void*)modify_type);
    }
    for(size_t i=0;i<AST_CHILD_NUM(enumerator_list_node);i++){
        AST_BASE* enumerator_node=AST_GET_CHILD(enumerator_list_node,i);
        AST_BASE* enumerator_id_node=AST_GET_CHILD(enumerator_node,0);
        find_tmpsi=Create_symbol_item(enumerator_id_node->token->value,NMSP_DEFAULT);
        tmpsi=(SYM_ITEM*)HASHFind(
            enumerator_list_node->symbol_table->sym_hash_table,
            find_tmpsi,
            symbol_item_cmp,
            false,
            false
        );
        if(HASH_ITEM_EXIST(tmpsi)&&tmpsi->defined)
        {
            C_ERROR(C0015_ERR_REDEFINATION,enumerator_list_node);
            return NULL;
        }
        find_tmpsi->count=HASH_CNT_IST;
        find_tmpsi->declared=find_tmpsi->defined=true;
        find_tmpsi->type_vec=const_int_type_vec;
        find_tmpsi->data_size=base_type->data_size;
        find_tmpsi->linkage=LKA_NONE;
        find_tmpsi->const_expr=true;
        if(AST_CHILD_NUM(enumerator_node)==3){
            AST_BASE* constant_expr_node=AST_GET_CHILD(enumerator_node,2);
            bool const_expr=const_value(constant_expr_node);
            if(!const_expr)
                return NULL;
            enum TP_CATEGORY const_base_type=(Type_VEC_get_actual_base_type(constant_expr_node->expr_attribute->type_vec))->typ_category;
            if(!IS_INT_TYPE(const_base_type)){
                C_ERROR(C0027_ERR_ENUM_NOT_INT,enumerator_list_node);
                return NULL;
            }
            find_tmpsi->data_field=constant_expr_node->expr_attribute->data_field;
            /*promised an int type now*/
            signed long long int test_enum_value=TP_INT_CAST_TYPE(const_base_type,constant_expr_node->expr_attribute->data_field);
            if(test_enum_value>INT32_MAX
                ||test_enum_value<INT32_MIN)    /*out of int range*/
            {
                C_ERROR(C0028_ERR_ENUM_RANGE,enumerator_list_node);
                return NULL;
            }
            enum_value=(signed int)TP_INT_CAST_TYPE(const_base_type,find_tmpsi->data_field);
            enum_value++;
        }
        else{
            find_tmpsi->data_field=m_alloc(type_data_size[TP_SINT]);
            find_tmpsi->data_field->sint=enum_value;
            enum_value+=1;
        }
        if(insert_symbol(enumerator_list_node->symbol_table,find_tmpsi))    /*enumerator is no linkage*/
            enumerator_id_node->token->symbol_item=find_tmpsi;
        VECinsert(member_vec_tmp,(void*)find_tmpsi);
    }
    find_tmpsi=Create_symbol_item(tag,NMSP_SU_TAG);
    tmpsi=(SYM_ITEM*)HASHFind(
        enumerator_list_node->symbol_table->sym_hash_table,
        find_tmpsi,
        symbol_item_cmp,
        false,
        false
    );
    m_free(find_tmpsi);
    bool tmpsi_exit=HASH_ITEM_EXIST(tmpsi);
    if(tmpsi_exit){
        if(tmpsi->defined){
            /*redefination error,if a enumerator_list_node exits*/
            C_ERROR(C0015_ERR_REDEFINATION,enumerator_list_node);
            return NULL;
        }else{
            enum_t=(TP_ENUMERA*)Type_VEC_get_actual_base_type(tmpsi->type_vec);
            tag_node->token->symbol_item=tmpsi;
        }
    }
    else{
        enum_t=(TP_ENUMERA*)build_base_type(TP_ENUM);
        enum_t->tag=tag;
        enum_t->key=TypeToKey((M_TYPE*)enum_t);
        enum_t->members=member_vec_tmp;
        VEC* enum_type_vec=InitVEC(DEFAULT_CAPICITY);
        VECinsert(enum_type_vec,(void*)enum_t);
        find_tmpsi=Create_symbol_item(tag,NMSP_SU_TAG);
        if(find_tmpsi){ /*if null, it might be an anonymous enum*/
            find_tmpsi->type_vec=enum_type_vec;
            find_tmpsi->defined=true;
            find_tmpsi->declared=true;
            find_tmpsi->count=HASH_CNT_IST;
            find_tmpsi->linkage=LKA_NONE;
            if(insert_symbol(enumerator_list_node->symbol_table,find_tmpsi))    /*tag is no linkage*/
                tag_node->token->symbol_item=find_tmpsi;
        }
    }
    m_free(tei);  /*if there's an error, the return is earlier,so now it's empty,free it*/
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("finish enum\n");
    semantics_level--;
#endif
    return enum_t;
}
TP_FUNC* function_type(VEC* tmp_node_vec)
{
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start func\n");
#endif
    if(!tmp_node_vec)
        return NULL;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    TP_FUNC* res=(TP_FUNC*)build_base_type(TP_FUNCTION);
    if(VECLEN(tmp_node_vec)==0){
        /*is an old style*/
        res->is_old_style=true;
    }
    else if(VECLEN(tmp_node_vec)==1){
        AST_BASE* parameter_type_list_node=VEC_GET_ITEM(tmp_node_vec,0);
        if(parameter_type_list_node->type==identifier_list){
            if(parameter_type_list_node->symbol_table->sp_type==SPT_FUN_PROTOTYPE){
                C_ERROR(C0042_ERR_FUN_IDENTIFIER_LIST,parameter_type_list_node);
                return NULL;
            }
            res->is_old_style=true;
            for(size_t i=0;i<AST_CHILD_NUM(parameter_type_list_node);++i){
                AST_BASE* id_node=AST_GET_CHILD(parameter_type_list_node,i);
                TP_FUNC_PARA* tmppara=m_alloc(sizeof(TP_FUNC_PARA));
                tmppara->para_name=id_node->token->value;
                tmppara->type_vec=NULL;
                VECinsert(res->func_parameters,(void*)tmppara);
            }   /*just build a parameter and don't insert the symbol,it will be done in the function definition part*/
        }
        else{
            if(AST_CHILD_NUM(parameter_type_list_node)==3){
                res->have_ellipsis=true;
            }
            bool have_void_para=false;
            AST_BASE* parameter_list_node=AST_GET_CHILD(parameter_type_list_node,0);
            for(size_t i=0;i<AST_CHILD_NUM(parameter_list_node);++i){
                if(have_void_para){
                    C_ERROR(C0046_ERR_FUNC_VOID_MORE_THAN_ONE,parameter_list_node);
                    goto error;
                }
                M_TYPE* tmp_type=NULL;
                AST_BASE* parameter_declaration_node=AST_GET_CHILD(parameter_list_node,i);
                int declaration_cnt=0;
                VEC* type_vec=NULL;
                bool typedef_declaration=false;
                AST_BASE* first_child=AST_GET_CHILD(parameter_declaration_node,0);
                TP_FUNC_PARA* tmppara=m_alloc(sizeof(TP_FUNC_PARA));
                tmppara->para_name=NULL;
                if((type_vec=declaration_spec_qual_list_type(
                    first_child,
                    &declaration_cnt,
                    &typedef_declaration,
                    true))==NULL)
                    goto error;
                /*check,the storage class of a function parameter*/
                tmp_type=Type_VEC_get_spec_other(type_vec);
                if(tmp_type&&tmp_type->typ_stor!=TP_STOR_NONE&&tmp_type->typ_stor!=TP_REGISTER){
                    C_ERROR(C0047_ERR_FUNC_PARA_STORAGE,parameter_type_list_node);
                    goto error;
                }
                if(tmp_type&&tmp_type->align_spec>0)
                {
                    C_ERROR(C0089_ERR_ALIGN_CANNOT_SPECIFIE,first_child);
                    goto error;
                }
                if(typedef_declaration){
                    C_ERROR(C0047_ERR_FUNC_PARA_STORAGE,parameter_type_list_node);
                    goto error;
                }
                SYM_ITEM* tmpsi=NULL;
                if(AST_CHILD_NUM(parameter_declaration_node)==2){
                    AST_BASE* second_child=AST_GET_CHILD(parameter_declaration_node,1);
                    char* declarator_char_name;
                    if(second_child->type==declarator){
                        if(declarator_type(second_child,
                            type_vec,
                            &declaration_cnt,
                            &typedef_declaration,
                            second_child->name_space,
                            &declarator_char_name
                        )==false)
                            goto error;
                        tmpsi=find_symbol_curr_table(parameter_type_list_node->symbol_table,declarator_char_name,second_child->name_space);
                        tmppara->para_name=tmpsi->value;
                    }
                    else if(second_child->type==abstract_declarator){
                        if(!abs_declarator_type(second_child,type_vec))
                            goto error;
                    }
                }
                /*set parameter and insert,here, if there's a parameter identifier,it must have been inserted*/
                tmppara->type_vec=type_vec;
                VECinsert(res->func_parameters,(void*)tmppara);
                tmp_type=Type_VEC_get_actual_base_type(tmppara->type_vec);
                /*a void parameter case*/
                if(tmp_type&&tmp_type->typ_category==TP_VOID){
                    have_void_para=true;
                    VECremove(res->func_parameters,0,1);
                }
                else if(tmp_type){
                    /*adjustment*/
                    if(tmp_type->typ_category==TP_ARRAY){
                        VECpopback(tmppara->type_vec);
                        M_TYPE* tmp_pointer=build_base_type(TP_POINT);
                        VECinsert(tmppara->type_vec,(void*)tmp_pointer);
                        VEC* arr_type_vec=(VEC*)(((TP_ARR*)tmp_type)->axis_modify);
                        if(arr_type_vec&&VECLEN(arr_type_vec)){
                            M_TYPE* arr_modify_type=VEC_GET_ITEM(arr_type_vec,0);
                            VECinsert(tmppara->type_vec,(void*)arr_modify_type);
                        }
                    }
                    else if(tmp_type->typ_category==TP_FUNCTION){
                        M_TYPE* tmp_pointer=build_base_type(TP_POINT);
                        VECinsert(tmppara->type_vec,(void*)tmp_pointer);
                    }
                    /*after adjustment,check incomplete type*/
                    tmp_type=Type_VEC_get_actual_base_type(tmppara->type_vec);
                    if(tmp_type&&tmp_type->complete==false&&parameter_type_list_node->symbol_table->sp_type!=SPT_FUN_PROTOTYPE){
                        C_ERROR(C0045_ERR_FUNC_PARA_INCOMPLETE_TYPE,parameter_type_list_node);
                        goto error;
                    }
                }
            }            
        }
    }
    else{   /*impossible*/
        goto error;
    }
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("finish func\n");
    semantics_level--;
#endif
    m_free(tei);
    return res;
error:
    m_free(res);
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("finish func fail\n");
    semantics_level--;
#endif
    return NULL;
}
TP_ARR* array_type(VEC* tmp_node_vec)
{
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start array\n");
#endif
    if(!tmp_node_vec)
        return NULL;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    TP_ARR* res=(TP_ARR*)build_base_type(TP_ARRAY);
    if(VECLEN(tmp_node_vec)==0){
        /*no any nodes*/
        res->complete=false;
    }
    else{
        /*first,find all the nodes*/
        AST_BASE* assign_expr_node=NULL;
        AST_BASE* type_qual_list_node=NULL;
        AST_BASE* static_node=NULL;
        AST_BASE* star_node=NULL;
        for(size_t i=0;i<VECLEN(tmp_node_vec);++i){
            AST_BASE* tmp_node=(AST_BASE*)VEC_GET_ITEM(tmp_node_vec,i);
            if(tmp_node->type==star)
                star_node=tmp_node;
            if(IS_EXPR_NODE(tmp_node->type))
                assign_expr_node=tmp_node;
            if(tmp_node->type==type_qual_list)
                type_qual_list_node=tmp_node;
            if(tmp_node->type==KW_static)
                static_node=tmp_node;
        }
        VEC* type_vec=NULL;
        int declaration_cnt=0;    /*useless,just for the function parameter*/
        bool typedef_declaration=false;
        if(type_qual_list_node)
            type_vec=declaration_spec_qual_list_type(
                type_qual_list_node,
                &declaration_cnt,
                &typedef_declaration,
                false);
        if(static_node){
            if(type_vec&&VECLEN(type_vec)==1){
                M_TYPE* tmpt=VEC_GET_ITEM(type_vec,0);
                tmpt->typ_stor=TP_STATIC;
            }
            else{
                type_vec=InitVEC(DEFAULT_CAPICITY);
                enum TP_STORAGE_SPEC storage_spec_type=TP_STATIC;
                int type_qual_cnt[TYPE_QUAL_NUM]={0,0,0,0};
                int func_spec_cnt[FUNCTION_SPEC_NUM]={0,0};
                unsigned long int align_spec_cnt=0;
                M_TYPE* tmpt=build_modify_type(storage_spec_type,type_qual_cnt,func_spec_cnt,align_spec_cnt);
                VECinsert(type_vec,(void*)tmpt);
            }
        }
        res->axis_modify=type_vec;
        if(assign_expr_node){
            res->is_star=false;
            if(!expr_dispatch(assign_expr_node))
            {
                goto error;
            }
            enum TP_CATEGORY const_base_type=(Type_VEC_get_actual_base_type(assign_expr_node->expr_attribute->type_vec))->typ_category;
            if(!IS_INT_TYPE(const_base_type))
            {
                C_ERROR(C0038_ERR_ARR_SIZE_INTEGER,assign_expr_node);
                goto error;
            }
            if(assign_expr_node->expr_attribute->const_expr){
                res->is_vla=false;
                res->complete=true;
                signed long long int tmp_arr_axis_value=TP_INT_CAST_TYPE(const_base_type,assign_expr_node->expr_attribute->data_field);
                res->axis_size=tmp_arr_axis_value;
                if(res->axis_size<=0){
                    C_ERROR(C0039_ERR_ARR_SIZE_ZERO,assign_expr_node);
                    goto error;
                }
            }
            else{
                res->is_vla=true;
                res->complete=true;
                if(assign_expr_node->symbol_table->sp_type==SPT_FUN_PROTOTYPE){ /*treated as star*/
                    res->is_star=true;
                    res->complete=true;
                }
                res->assign_expr_node=assign_expr_node;
            }
        }
        else{
            if(star_node)
            {
                res->is_star=true;
                res->complete=true;
                res->is_vla=true;
            }
            else{   
            /*in this case only type-qualifier-list exists,
            like 'int arr[const]',but I think it's treated as int arr[*]*/
                res->is_star=true;
                res->complete=true;
                res->is_vla=true;
            }
        }
    }
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("finish array\n");
    semantics_level--;
#endif
    return res;
error:
    m_free(res);
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("finish array fail\n");
    semantics_level--;
#endif
    return NULL;
}