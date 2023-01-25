#include "decl_semantics.h"
#ifdef _TEST_SEMANTICS_
extern int semantics_level;
#endif
extern VEC* c_error;
extern char* filename;
extern size_t type_data_size[TYPE_NUM];
const int storage_spec_enum[STORAGE_SPEC_NUM]={
    KW_type_def,KW_extern,KW_static,KW__Thread_local,KW_auto,KW_register
    };
const int type_spec_enum[TYPE_SPEC_NUM]={
    KW_void,KW_char,KW_short,KW_int,KW_long,KW_float,KW_double,KW_signed,KW_unsigned,KW__Bool,KW__Complex,
    atomic_type_spec,struct_union_spec,enum_spec,type_def_name
    };
const int type_qual_enum[TYPE_QUAL_NUM]={
    KW_const,KW_restrict,KW_volatile,KW__Atomic
    };
const int func_spec_enum[FUNCTION_SPEC_NUM]={
    KW_inline,KW__Noreturn
    };
/*for type spec, the situation is complex,and it can in any order ,so we have to list all the possible case and cmp.*/
const int type_spec_case[TYPE_SPEC_CASE_NUM][TYPE_SPEC_NUM]={
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},    /*void*/
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},    /*char*/
    {0,1,0,0,0,0,0,1,0,0,0,0,0,0,0},    /*signed char*/
    {0,1,0,0,0,0,0,0,1,0,0,0,0,0,0},    /*unsigned char*/
    {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0},    /*short*/
    {0,0,1,0,0,0,0,1,0,0,0,0,0,0,0},    /*signed short*/
    {0,0,1,1,0,0,0,0,0,0,0,0,0,0,0},    /*short int*/
    {0,0,1,1,0,0,0,1,0,0,0,0,0,0,0},    /*signed short int*/
    {0,0,1,0,0,0,0,0,1,0,0,0,0,0,0},    /*unsigned short*/
    {0,0,1,1,0,0,0,0,1,0,0,0,0,0,0},    /*unsigned short int*/
    {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},    /*int*/
    {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},    /*signed*/
    {0,0,0,1,0,0,0,1,0,0,0,0,0,0,0},    /*signed int*/
    {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},    /*unsigned*/
    {0,0,0,1,0,0,0,0,1,0,0,0,0,0,0},    /*unsigned int*/
    {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},    /*long*/
    {0,0,0,0,1,0,0,1,0,0,0,0,0,0,0},    /*signed long*/
    {0,0,0,1,1,0,0,0,0,0,0,0,0,0,0},    /*long int*/
    {0,0,0,1,1,0,0,1,0,0,0,0,0,0,0},    /*signed long int*/
    {0,0,0,0,1,0,0,0,1,0,0,0,0,0,0},    /*unsigned long*/
    {0,0,0,1,1,0,0,0,1,0,0,0,0,0,0},    /*unsigned long int*/
    {0,0,0,0,2,0,0,0,0,0,0,0,0,0,0},    /*long long*/
    {0,0,0,0,2,0,0,1,0,0,0,0,0,0,0},    /*signed long long*/
    {0,0,0,1,2,0,0,0,0,0,0,0,0,0,0},    /*long long int*/
    {0,0,0,1,2,0,0,1,0,0,0,0,0,0,0},    /*signed long long int*/
    {0,0,0,0,2,0,0,0,1,0,0,0,0,0,0},    /*unsigned long long*/
    {0,0,0,1,2,0,0,0,1,0,0,0,0,0,0},    /*unsigned long long int*/
    {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},    /*float*/
    {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},    /*double*/
    {0,0,0,0,1,0,1,0,0,0,0,0,0,0,0},    /*long double, but for the gcc or vs implement, it's the same with the double*/
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},    /*_Bool*/
    {0,0,0,0,0,1,0,0,0,0,1,0,0,0,0},    /*float _Complex*/
    {0,0,0,0,0,0,1,0,0,0,1,0,0,0,0},    /*double _Complex*/
    {0,0,0,0,1,0,1,0,0,0,1,0,0,0,0},    /*long double _Complex*/
    {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},    /*atomic*/
    {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},    /*struct/union*/
    {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},    /*enum*/
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}     /*typedef*/
};
enum TP_CATEGORY type_spec_case_type[TYPE_SPEC_CASE_NUM]={
    TP_VOID,
#if _FSIGN == 0
    TP_SCHAR,TP_SCHAR,
    TP_USCHAR,
#elif _FSIGN == 1
    TP_USCHAR,
    TP_SCHAR,
    TP_USCHAR,
#endif
    TP_SSHORT,TP_SSHORT,TP_SSHORT,TP_SSHORT,
    TP_USHORT,TP_USHORT,
    TP_SINT,TP_SINT,TP_SINT,
    TP_USINT,TP_USINT,
    TP_SLONG,TP_SLONG,TP_SLONG,TP_SLONG,
    TP_USLONG,TP_USLONG,
    TP_SLONGLONG,TP_SLONGLONG,TP_SLONGLONG,TP_SLONGLONG,
    TP_USLONGLONG,TP_USLONGLONG,
    TP_FLOAT,
    TP_DOUBLE,
    TP_LONG_DOUBLE,
    TP_BOOL,
    TP_FLOAT_COMPLEX,
    TP_DOUBLE_COMPLEX,
    TP_LONG_DOUBLE_COMPLEX,
    TP_SPEC_ATOMIC,
    TP_UNION_STRUCT,
    TP_ENUM,
    TP_TYPE_DEF_TYPE
};
bool declaration_type(AST_BASE* ast_node,VEC* dec_symbol_item_list)
{
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start declaration line:%d\n",ast_node->token->line);
#endif
    if(!ast_node||ast_node->type!=declaration)
        return false;
    ERROR_ITEM* tei=(ERROR_ITEM*)m_alloc(sizeof(ERROR_ITEM));
    AST_BASE* first_child_node=AST_GET_CHILD(ast_node,0);
    enum scope_type curr_scope=ast_node->symbol_table->sp_type;
    if(first_child_node->type==static_assert_declaration){
        return static_assert_type(first_child_node);
    }
    else{
        int declaration_cnt=0;  /*a declaration must have a declarator a tag or a enumrator*/
        VEC* type_vec=NULL;
        bool typedef_declaration=false; /*remember, typedef declaration and typedef name is different*/
        AST_BASE* declaration_spec_node=first_child_node;
        AST_BASE* init_declarator_list_node=AST_GET_CHILD(ast_node,1);
        bool have_declarator=(init_declarator_list_node->type!=semi_colon);
        if((type_vec=declaration_spec_qual_list_type(
            declaration_spec_node,
            &declaration_cnt,
            &typedef_declaration,
            true))==NULL)
            return false;   /*a length==0 type vec means no type,but NULL means an error*/
        if(have_declarator){
            for(size_t i=0;i<AST_CHILD_NUM(init_declarator_list_node);i++){
                VEC* tmp_type_vec=InitVEC(DEFAULT_CAPICITY);
                VECappend(type_vec,tmp_type_vec);
                char* declarator_char_name;
                AST_BASE* init_dec_node=AST_GET_CHILD(init_declarator_list_node,i);
                /*declarator part*/
                AST_BASE* declarator_node=AST_GET_CHILD(init_dec_node,0);
                if(declarator_type(declarator_node,
                    tmp_type_vec,
                    &declaration_cnt,
                    &typedef_declaration,
                    declarator_node->name_space,
                    &declarator_char_name
                )==false)
                    return false;
                SYM_ITEM* tmpsi=find_symbol_curr_table(declarator_node->symbol_table,declarator_char_name,declarator_node->name_space);
                if(!tmpsi)
                    return false;
                VECinsert(dec_symbol_item_list,(void*)tmpsi);
                
                M_TYPE* tmpt=Type_VEC_get_spec_other(tmpsi->type_vec);
                if(tmpt){
                    tmpsi->fspec_type=tmpt->func_spec;
                    tmpsi->align_size=tmpt->align_spec;
                }
                /*initialize part*/
                if(AST_CHILD_NUM(init_dec_node)==3){
                    if(declarator_node->symbol_table->sp_type==SPT_BLOCK
                        &&(tmpsi->linkage==LKA_INTERN||tmpsi->linkage==LKA_EXTERN))
                    {
                        C_ERROR(C0095_ERR_INIT_SCOPE_AND_LINKAGE,declarator_node);
                        return false;
                    }
                    AST_BASE* initializer_node=AST_GET_CHILD(init_dec_node,2);
                    if(!initializer_semantic(initializer_node,tmpsi->type_vec,0,0)){
                        return false;
                    }
                }
                else{
                    /*Default_implict_initialization part,do nothing*/
                }
                if(tmpsi&&Type_VEC_VM(tmpsi->type_vec)){
                    if(tmpsi->linkage!=LKA_NONE
                        ||(curr_scope!=SPT_BLOCK
                        &&curr_scope!=SPT_FUN_PROTOTYPE))
                    {
                        C_ERROR(C0041_ERR_VM_ID,declarator_node);
                        return false;
                    }
                }
            }
        }
        else{
            /*this node is opt,so it might be a ';',do some check and return true*/

        }
        if(declaration_cnt==0){
            ERROR_ITEM* twi=(ERROR_ITEM*)m_alloc(sizeof(ERROR_ITEM));
            C_WARN(W0003_WARN_NO_DECLARATION_ANY_THING,ast_node);
        }   
    }
    ast_node->decl_attribute->decl_symbol_item_list=dec_symbol_item_list;
    m_free(tei);
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("finish declaration\n");
    semantics_level--;
#endif
    return true;
}
bool declarator_type(AST_BASE* declarator_node,
    VEC* type_vec,
    int* declarator_cnt,
    bool* typedef_declaration,
    NMSP name_space,
    char** declarator_char_name)
{
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start declarator\n");
#endif
    if(!declarator_node||declarator_node->type!=declarator)
        return false;
    AST_BASE* point_node=NULL;   /*I changed the syntax,so use the 'rule.c' version, the pointer syntax is a bit different*/
    AST_BASE* direct_dec_node=NULL;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    for(size_t i=0;i<AST_CHILD_NUM(declarator_node)-1;++i){
        point_node=AST_GET_CHILD(declarator_node,i);
        M_TYPE* point_type=build_base_type(TP_POINT);
        VECinsert(type_vec,(void*)point_type);
        AST_BASE* type_qual_list_node=AST_GET_CHILD(point_node,1);
        if(type_qual_list_node!=NULL){
            VEC* tmpv=InitVEC(DEFAULT_CAPICITY);
            tmpv=declaration_spec_qual_list_type(type_qual_list_node,
                declarator_cnt,
                typedef_declaration,
                false);
            if(tmpv==NULL)
                return false;
            VECappend(tmpv,type_vec);
        }
    }
    direct_dec_node=AST_GET_CHILD(declarator_node,AST_CHILD_NUM(declarator_node)-1);
    int direct_dec_index=AST_CHILD_NUM(direct_dec_node)-1;
    int direct_dec_begin=0,direct_dec_end=0;
    bool is_vm=true;
    SYM* curr_table=declarator_node->symbol_table;
    enum scope_type curr_scope=curr_table->sp_type;
    while(direct_dec_index>=0){
        direct_dec_begin=direct_dec_end=direct_dec_index;
        int function_array=-1;    /*0-array,1-funciton*/
        enum rule_type_enum dec_end_type=AST_GET_CHILD(direct_dec_node,direct_dec_end)->type;
        if(dec_end_type==identifier){
            (*declarator_cnt)++;
            TOKEN* tmp_token=AST_GET_CHILD(direct_dec_node,direct_dec_end)->token;
            *declarator_char_name=tmp_token->value;
            /*TODO:check redefinition and linkage*/
            SYM_ITEM* tmpsi;
            tmpsi=find_symbol_curr_table(curr_table,
                *declarator_char_name,
                declarator_node->name_space);
            enum linkage_type tmpl=LKA_NONE;
            bool should_insert=true;
            M_TYPE* tmpt=Type_VEC_get_spec_other(type_vec);
            if(tmpt){
                if(tmpt->typ_stor==TP_STATIC||tmpt->typ_stor==TP_STATIC_THREAD_LOCAL)
                    tmpl=LKA_INTERN;
                else if(tmpt->typ_stor==TP_EXTERN||tmpt->typ_stor==TP_EXTERN_THREAD_LOCAL)
                    tmpl=LKA_EXTERN;
            }
            /*if a function type*/
            tmpt=Type_VEC_get_actual_base_type(type_vec);
            /*check whether this identifier is a definition*/
            bool is_definition=false;
            if(Type_size(type_vec)>0&&tmpt&&tmpt->typ_category!=TP_FUNCTION&&tmpt->typ_category!=TP_ENUM&&tmpt->typ_category!=TP_TYPE_DEF_TYPE)
                is_definition=true;
            else if(tmpt&&tmpt->typ_category==TP_TYPE_DEF_TYPE)
                is_definition=true;
            else if(tmpt&&tmpt->typ_category==TP_ENUM)
            {
                if(((TP_ENUMERA*)tmpt)->members&&VECLEN(((TP_ENUMERA*)tmpt)->members))
                    is_definition=true;
            }
            if(tmpt&&tmpt->typ_category==TP_FUNCTION)
            {
                M_TYPE* tmptt=Type_VEC_get_spec_other(type_vec);
                //if(tmptt&&tmptt->typ_stor==TP_STOR_NONE)
                //    tmptt->typ_stor=TP_EXTERN;
                if(tmptt&&tmptt->align_spec>0)
                {
                    C_ERROR(C0089_ERR_ALIGN_CANNOT_SPECIFIE,direct_dec_node);
                    return false;
                }
                tmpl=LKA_EXTERN;
                /*set the function type's name*/
                ((TP_FUNC*)tmpt)->func_name=*declarator_char_name;
            }
            /*start set linkage according to some information*/
            if(curr_scope==SPT_FILE){
                if(tmpl==LKA_INTERN)
                    tmpl=LKA_INTERN;
                else{
                    tmpl=LKA_EXTERN;
                }
                if(HASH_ITEM_EXIST(tmpsi)){
                    enum linkage_type prel=tmpsi->linkage;
                    if(tmpl!=prel){
                        C_ERROR(C0032_ERR_CONFICT_LINKAGE,declarator_node);
                        return false;
                    }
                    if(!Type_VEC_cmp(tmpsi->type_vec,type_vec))
                    {
                        if(!compatible_types(tmpsi->type_vec,type_vec)){
                            C_ERROR(C0031_ERR_CONFICT_TYPE,declarator_node);
                            return false;
                        }
                        else
                        {
                            M_TYPE* tmp_align_type=Type_VEC_get_spec_other(type_vec);
                            M_TYPE* tmp_si_align_type=Type_VEC_get_spec_other(tmpsi->type_vec);
                            if(tmp_align_type&&tmp_si_align_type)
                            {
                                if(tmp_si_align_type->align_spec>0
                                &&tmp_align_type->align_spec!=0
                                &&tmp_align_type->align_spec!=tmp_si_align_type->align_spec)
                                {
                                    C_ERROR(C0090_ERR_ALIGN_DIFFERENT_DECLARATION,direct_dec_node);
                                    return false;
                                }
                                else if(tmp_si_align_type->align_spec==0
                                &&tmp_align_type->align_spec!=0)
                                {
                                    C_ERROR(C0090_ERR_ALIGN_DIFFERENT_DECLARATION,direct_dec_node);
                                    return false;
                                }
                            }
                            tmpsi->type_vec=composite_types(tmpsi->type_vec,type_vec,true);
                        }
                    }
                    should_insert=false;
                    declarator_node->token->symbol_item=tmpsi;
                }
            }
            else if(curr_scope==SPT_BLOCK){
                if(tmpl!=LKA_EXTERN)
                    tmpl=LKA_NONE;
                if(HASH_ITEM_EXIST(tmpsi)){      /*find a same identifier at the curr scope*/
                    /*only extern/extern extern/no no/extern no/no four cases*/
                    enum linkage_type prel=tmpsi->linkage;
                    bool redefine=true;
                    /*check same identifier in the curr scope*/
                    if(tmpl==LKA_EXTERN){
                        if(prel!=LKA_EXTERN){
                            C_ERROR(C0029_ERR_LKA_EXTERN_FOLLOW_NO,declarator_node);
                            return false;
                        }
                        /*if both are extern, not an error and no need to insert*/
                        /*the type must be same,check it*/
                        if(!Type_VEC_cmp(tmpsi->type_vec,type_vec))
                        {
                            if(!compatible_types(tmpsi->type_vec,type_vec)){
                                C_ERROR(C0031_ERR_CONFICT_TYPE,declarator_node);
                                return false;
                            }
                            else
                            {
                                M_TYPE* tmp_align_type=Type_VEC_get_spec_other(type_vec);
                                M_TYPE* tmp_si_align_type=Type_VEC_get_spec_other(tmpsi->type_vec);
                                if(tmp_si_align_type->align_spec>0
                                &&tmp_align_type->align_spec!=0
                                &&tmp_align_type->align_spec!=tmp_si_align_type->align_spec)
                                {
                                    C_ERROR(C0090_ERR_ALIGN_DIFFERENT_DECLARATION,direct_dec_node);
                                    return false;
                                }
                                else if(tmp_si_align_type->align_spec==0
                                &&tmp_align_type->align_spec!=0)
                                {
                                    C_ERROR(C0090_ERR_ALIGN_DIFFERENT_DECLARATION,direct_dec_node);
                                    return false;
                                }
                                tmpsi->type_vec=composite_types(tmpsi->type_vec,type_vec,true);
                            }
                        }
                        should_insert=false;
                        declarator_node->token->symbol_item=tmpsi;
                    }
                    else if(tmpl==LKA_NONE&&prel==LKA_EXTERN){
                        C_ERROR(C0030_ERR_LKA_NO_FOLLOW_EXTERN,declarator_node);
                        return false;
                    }
                    else{
                        /*then is the no/no case*/
                        /*if a redeclaration of typedef at the same scope and type is the same ,it's not an error*/
                        if(*typedef_declaration){
                            TP_DEF_TYPE* typedef_name_type=(TP_DEF_TYPE*)Type_VEC_get_actual_base_type(tmpsi->type_vec);
                            if(typedef_name_type&&typedef_name_type->typedef_name_type){
                                if(Type_VEC_cmp(typedef_name_type->typedef_name_type,type_vec)&&compatible_types(typedef_name_type->typedef_name_type,type_vec))
                                    redefine=false;
                            }
                            /*the type must not be a variably modified type*/
                            if(is_vm){
                                redefine=true;
                            }   /*I tried the following codes on gcc in a block scope,
                                'typedef int iarr[testarr];
                                typedef int iarr[testarr];'
                                get an error: redefinition of typedef ‘iarr’ with variably modified type
                            */
                        }
                        if(redefine){
                            C_ERROR(C0015_ERR_REDEFINATION,declarator_node);
                            return false;
                        }
                    }
                }
                else{
                    if(tmpl==LKA_EXTERN)   /*if extern or function,check the prior declarations*/
                    {
                        /*try to find*/
                        curr_table=curr_table->father;
                        while (curr_table)
                        {
                            tmpsi=find_symbol_curr_table(curr_table,
                                *declarator_char_name,
                                declarator_node->name_space);
                            if(tmpsi)
                                break;
                            curr_table=curr_table->father;
                        }
                        /*set linkage*/
                        if(HASH_ITEM_EXIST(tmpsi)){
                            if(tmpsi->linkage==LKA_INTERN)
                                tmpl=LKA_INTERN;
                            /*check the type*/
                            tmpt=NULL;
                            enum TP_STORAGE_SPEC prestor=TP_STOR_NONE,tmpstor=TP_STOR_NONE;
                            tmpt=Type_VEC_get_spec_other(tmpsi->type_vec);
                            if(tmpt)
                                prestor=tmpt->typ_stor;
                            tmpt=Type_VEC_get_spec_other(type_vec);
                            if(tmpt)
                                tmpstor=tmpt->typ_stor;
                            //if(tmpt&&tmpt->modifier)
                            //    tmpt->typ_stor=prestor;
                            if((prestor==TP_EXTERN_THREAD_LOCAL&&tmpstor!=TP_EXTERN_THREAD_LOCAL)
                                ||(prestor==TP_STATIC_THREAD_LOCAL&&tmpstor!=TP_STATIC_THREAD_LOCAL))
                            {
                                C_ERROR(C0033_ERR_CONFICT_THREAD_LOCAL,declarator_node);
                                return false;
                            }
                            if(!Type_VEC_cmp(tmpsi->type_vec,type_vec))
                            {
                                if(!compatible_types(tmpsi->type_vec,type_vec)){
                                    C_ERROR(C0031_ERR_CONFICT_TYPE,declarator_node);
                                    return false;
                                }
                                else
                                {
                                    M_TYPE* tmp_align_type=Type_VEC_get_spec_other(type_vec);
                                    M_TYPE* tmp_si_align_type=Type_VEC_get_spec_other(tmpsi->type_vec);
                                    if(tmp_si_align_type->align_spec>0
                                    &&tmp_align_type->align_spec!=0
                                    &&tmp_align_type->align_spec!=tmp_si_align_type->align_spec)
                                    {
                                        C_ERROR(C0090_ERR_ALIGN_DIFFERENT_DECLARATION,direct_dec_node);
                                        return false;
                                    }
                                    else if(tmp_si_align_type->align_spec==0
                                    &&tmp_align_type->align_spec!=0)
                                    {
                                        C_ERROR(C0090_ERR_ALIGN_DIFFERENT_DECLARATION,direct_dec_node);
                                        return false;
                                    }
                                    tmpsi->type_vec=composite_types(tmpsi->type_vec,type_vec,true);
                                }
                            }
                            //if(tmpt&&tmpt->modifier)
                            //    tmpt->typ_stor=tmpstor;
                            if(insert_symbol(declarator_node->symbol_table,tmpsi))
                                tmp_token->symbol_item=tmpsi;
                            should_insert=false;
                        }   /*still need insert,but do not build a new one, insert the previous one*/
                    }
                }
            }
            else{
                tmpl=LKA_NONE;
            }
            /*finish check linkage and redefine,insert*/
            if(Type_VEC_VLA(type_vec)){
                printf("%s is a vla\n",AST_GET_CHILD(direct_dec_node,direct_dec_end)->token->value);
                bool have_static_thread=false;
                tmpt=Type_VEC_get_spec_other(type_vec);
                if(tmpt&&
                (tmpt->typ_stor==TP_STATIC||
                tmpt->typ_stor==TP_EXTERN_THREAD_LOCAL||
                tmpt->typ_stor==TP_STATIC_THREAD_LOCAL))
                    have_static_thread=true;
                if(have_static_thread){
                    C_ERROR(C0037_ERR_ARR_STATIC_THREAD_WITH_VLA,declarator_node);
                    return false;
                }
            }
            if(should_insert)
            {
                tmpsi=Create_symbol_item(*declarator_char_name,declarator_node->name_space);
                tmpsi->linkage=tmpl;
                tmpsi->count=HASH_CNT_IST;
                tmpsi->declared=true;
                tmpsi->defined=is_definition;
                if(tmpt&&tmpt->typ_category==TP_FUNCTION)
                {
                    tmpsi->defined=false;
                }
                if(*typedef_declaration){
                    VEC* tmp_type_vec=InitVEC(DEFAULT_CAPICITY);
                    TP_DEF_TYPE* typedef_type=(TP_DEF_TYPE*)build_base_type(TP_TYPE_DEF_TYPE);
                    typedef_type->typedef_name_type=type_vec;
                    VECinsert(tmp_type_vec,(void*)typedef_type);
                    tmpsi->type_vec=tmp_type_vec;
                }
                else
                    tmpsi->type_vec=type_vec;
                if(insert_symbol(declarator_node->symbol_table,tmpsi))
                    tmp_token->symbol_item=tmpsi;
            }
            break;
        }
        for(int i=direct_dec_end;i>=0;i--){
            enum rule_type_enum tmp_begin_type=AST_GET_CHILD(direct_dec_node,i)->type;
            if(tmp_begin_type==left_bracket&&dec_end_type==right_bracket){
                direct_dec_begin=i;
                function_array=0;
                break;
            }
            else if(tmp_begin_type==left_parenthesis&&dec_end_type==right_parenthesis){
                direct_dec_begin=i;
                function_array=1;
                break;
            }
        }
        if(direct_dec_begin==0&&direct_dec_end==2){
            AST_BASE* sub_declarator_node=AST_GET_CHILD(direct_dec_node,1);
            if(sub_declarator_node->type==declarator){
                if(!declarator_type(sub_declarator_node,
                    type_vec,
                    declarator_cnt,
                    typedef_declaration,
                    name_space,
                    declarator_char_name
                ))
                    return false;
                break;
            }
        }
        /*deal with function and array type*/
        M_TYPE* tmpt=NULL;
        VEC* tmp_node_vec=InitVEC(DEFAULT_CAPICITY);

        if(direct_dec_begin+1<=direct_dec_end){
            for(size_t i=direct_dec_begin+1;i<direct_dec_end;++i)
                VECinsert(tmp_node_vec,AST_GET_CHILD(direct_dec_node,i));
            if(function_array==1)      /*function*/
            {
                tmpt=(M_TYPE*)function_type(tmp_node_vec);
                if(!tmpt)
                    return false;
                /*check the return type of function*/
                M_TYPE* tmpfunct=Type_VEC_get_actual_base_type(type_vec);
                if(tmpfunct&&(tmpfunct->typ_category==TP_FUNCTION||tmpfunct->typ_category==TP_ARRAY))
                {
                    C_ERROR(C0043_ERR_FUN_RETURN_TYPE,direct_dec_node);
                    return false;
                }
            }
            else if(function_array==0){       /*array*/
                tmpt=(M_TYPE*)array_type(tmp_node_vec);
                if(!tmpt)
                    return false;
                if(((TP_ARR*)tmpt)->axis_modify){
                    if(declarator_node->father->type!=parameter_declaration){
                        C_ERROR(C0036_ERR_ARR_QUAL_STATIC_IN_PARA,declarator_node);
                        return false;
                    }
                    for(int i=VECLEN(type_vec)-1;i>=0;i--){
                        M_TYPE* tmparrt=VEC_GET_ITEM(type_vec,i);
                        if(tmparrt&&tmparrt->typ_category==TP_ARRAY&&((TP_ARR*)tmparrt)->axis_modify){
                            C_ERROR(C0035_ERR_ARR_QUAL_STATIC_OUTMOST,declarator_node);
                            return false;
                        }
                    }
                }
                for(int i=VECLEN(type_vec)-1;i>=0;i--){
                    M_TYPE* tmparrt=VEC_GET_ITEM(type_vec,i);
                    if(tmparrt&&tmparrt->complete==false){
                        C_ERROR(C0040_ERR_ARR_FUNCTIONS_INCOMPLETE,declarator_node);
                        return false;
                    }
                }
                M_TYPE* tmparrt=Type_VEC_get_actual_base_type(type_vec);
                if(tmparrt&&tmparrt->typ_category==TP_FUNCTION){
                    C_ERROR(C0040_ERR_ARR_FUNCTIONS_INCOMPLETE,declarator_node);
                    return false;
                }
                if(((TP_ARR*)tmpt)->is_vla)
                    is_vm=true;
                if(((TP_ARR*)tmpt)->is_star)
                {
                    if(curr_scope!=SPT_FUN_PROTOTYPE){
                        C_ERROR(C0034_ERR_ARR_STAR_SCOPE,declarator_node);
                        return false;
                    }
                }
            }
            else{
                ;   /*impossible,promised by parser syntex */
            }
        }
        if(tmpt)
            VECinsert(type_vec,(void*)tmpt);
        else{   /*TODO:a [] or () without anything inside case*/
            ;
        }
        direct_dec_index=direct_dec_begin-1;
    }
    /*TODO:check the array and function defination,and set the datas like function name*/
    /*TODO:for function type,no linkage means external linkage*/
    
    m_free(tei);
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("finish declarator\n");
    semantics_level--;
#endif
    return true;
}
VEC* declaration_spec_qual_list_type(AST_BASE* dec_sq_list_node,
    int* declaration_cnt,
    bool* typedef_declaration,
    bool need_type_spec)
{
    /*
        This function is a bit long, I agree.
        But I will note every step.
        So it might not very hard to understand.
        This function can be reuse by the 
            declaration-specifiers(in declaration)/
            specifier-qualifier-list(in struct/union)/
            type-qualifier-list(in declarator)
        The reason why I do not split this function is that it is a bit hard to deal with the return type.
        ——A lot of informations need to consider.
        Besides, there's no module can reuse,so it's meaningless to split this function.
    */
#ifdef _TEST_SEMANTICS_
    semantics_level++;
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("start specify or qualify node\n");
#endif
    if(!dec_sq_list_node||
        (dec_sq_list_node->type!=declaration_spec
        &&dec_sq_list_node->type!=spec_qual_list
        &&dec_sq_list_node->type!=type_qual_list)
    )
        return NULL;
    VEC* type_vec=InitVEC(DEFAULT_CAPICITY);
    enum TP_STORAGE_SPEC storage_spec_type=TP_STOR_NONE;
    enum TP_CATEGORY spec_type=TP_SPEC_NONE;
    M_TYPE* modify_type_ptr=NULL;
    M_TYPE* base_type_ptr=NULL;
    AST_BASE* Atomic_SPEC_node=NULL;
    /*First,check the spec or qual number and decide use what kind of type to use*/
    int storage_spec_total_cnt=0,storage_spec_cnt[STORAGE_SPEC_NUM]={0,0,0,0,0,0};
    int type_spec_total_cnt=0,type_spec_cnt[TYPE_SPEC_NUM]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int type_qual_total_cnt=0,type_qual_cnt[TYPE_QUAL_NUM]={0,0,0,0};
    int func_spec_total_cnt=0,func_spec_cnt[FUNCTION_SPEC_NUM]={0,0};
    unsigned long int align_spec_cnt=0;
    ERROR_ITEM* tei=(ERROR_ITEM*)m_alloc(sizeof(ERROR_ITEM));
    for(size_t i=0;i<AST_CHILD_NUM(dec_sq_list_node);++i){
        AST_BASE* spec_qual_node=AST_GET_CHILD(dec_sq_list_node,i);
        AST_BASE* spec_qual_sub_node=AST_GET_CHILD(spec_qual_node,0);
        if(spec_qual_node->type==storage_class_spec){
            for(size_t j=0;j<STORAGE_SPEC_NUM;++j){
                if(spec_qual_sub_node->type==storage_spec_enum[j])
                {
                    storage_spec_cnt[j]++;
                    storage_spec_total_cnt++;
                }
            }
        }
        else if(spec_qual_node->type==type_spec){
            for(size_t j=0;j<TYPE_SPEC_NUM;++j){
                if(spec_qual_sub_node->type==type_spec_enum[j])
                {
                    type_spec_cnt[j]++;
                    type_spec_total_cnt++;
                }
                if(spec_qual_sub_node->type==atomic_type_spec){
                    Atomic_SPEC_node=spec_qual_sub_node;
                }
            }
        }
        else if(spec_qual_node->type==type_qual){
            for(size_t j=0;j<TYPE_QUAL_NUM;++j){
                if(spec_qual_sub_node->type==type_qual_enum[j])
                {
                    type_qual_cnt[j]++;
                    type_qual_total_cnt++;
                }
            }
        }
        else if(spec_qual_node->type==function_spec){
            for(size_t j=0;j<FUNCTION_SPEC_NUM;++j){
                if(spec_qual_sub_node->type==func_spec_enum[j])
                {
                    func_spec_cnt[j]++;
                    func_spec_total_cnt++;
                }
            }
        }
        else if(spec_qual_node->type==alignment_spec){
            AST_BASE* Align_SPEC_node=spec_qual_node;
            /*TODO:calculate the alignment*/
            AST_BASE* align_num_node=AST_GET_CHILD(Align_SPEC_node,2);
            if(align_num_node->type==type_name)
            {
                VEC* align_type_vec=type_name_type(align_num_node);
                unsigned long int tmp_align_spec_cnt=Type_align(align_type_vec);
                if(align_spec_cnt==0)
                    align_spec_cnt=tmp_align_spec_cnt;
                else 
                    align_spec_cnt=max(tmp_align_spec_cnt,align_spec_cnt);
            }
            else if(align_num_node->type==constant_expr)
            {
                if(!const_value(align_num_node))
                    return NULL;
                M_TYPE* tmpt=Type_VEC_get_actual_base_type(align_num_node->expr_attribute->type_vec);
                if(!IS_INT_TYPE(tmpt->typ_category))
                {
                    C_ERROR(C0060_ERR_OPERAND_INTEGER_TYPE,align_num_node);
                    return NULL;
                }
                align_spec_cnt=(size_t)TP_INT_CAST_TYPE(tmpt->typ_category,align_num_node->expr_attribute->data_field);
            }
            if(!is_legal_align(align_spec_cnt))
            {
                C_ERROR(C0088_ERR_ALIGN_TWO_POWER,align_num_node);
                return NULL;
            }
        }
    }
    /*OK, we counted it,check the count*/
    /*First,check storage class spec number*/
    if(storage_spec_total_cnt>1){
        if(storage_spec_total_cnt==2 \
            &&storage_spec_cnt[3]==1 \
            &&(storage_spec_cnt[1]==1 \
            ||storage_spec_cnt[2]==1
        ))
        {
            storage_spec_type=(storage_spec_cnt[1])? \
            TP_EXTERN_THREAD_LOCAL:TP_STATIC_THREAD_LOCAL;/*right*/
        }
        else
        {
            /*wrong,insert to error list and return false*/
            AST_BASE* storage_spec_node=AST_GET_CHILD(dec_sq_list_node,0);
            C_ERROR(C0008_ERR_TOO_MANY_STOR_SPEC,storage_spec_node);
            return NULL;
        }
    }
    else if(storage_spec_total_cnt==1){   /*total is 1,but is _Thread_Local, it will be wrong*/
        if(storage_spec_cnt[3]==1 \
            &&dec_sq_list_node->symbol_table \
            &&(dec_sq_list_node->symbol_table->sp_type==SPT_BLOCK)){
            /*wrong,insert to error list and return false*/
            AST_BASE* storage_spec_node=AST_GET_CHILD(dec_sq_list_node,0);
            C_ERROR(C0009_ERR_THREAD_LOCAL_PREFIX,storage_spec_node);
            return NULL;
        }
        /*else is right,decide the storage class type*/
        for(size_t i=0;i<STORAGE_SPEC_NUM;++i){
            if(storage_spec_cnt[i]){
                storage_spec_type=i+1;
            }
        }
    }
    else{
        ;/*for 0, do nothing,I just don't want to see a warning*/
    }
    if(storage_spec_type==TP_TYPE_DEF){
        *typedef_declaration=true;
    }
    /*Second,check spec number*/
    if(type_spec_total_cnt==0&&need_type_spec)
    {
        /*wrong,but in gcc , the case like 'const a;' only get a warning and the type is given 'int'*/
        C_ERROR(C0010_ERR_NEED_MORE_TYPE_SPEC,dec_sq_list_node);
        return NULL;
    }
    else if(need_type_spec){
        for(size_t i=0;i<TYPE_SPEC_CASE_NUM;++i){
            bool succ=true;
            for(size_t j=0;j<TYPE_SPEC_NUM;++j){
                if(type_spec_cnt[j]!=type_spec_case[i][j]){
                    succ=false;
                    break;
                }
            }
            if(succ){
                spec_type=type_spec_case_type[i];
                break;
            }
        }
        if(spec_type==TP_SPEC_NONE){
            /*wrong*/
            C_ERROR(C0011_ERR_WRONG_TYPE_SPEC,dec_sq_list_node);
            return NULL;
        }
        /*find the first type spec node*/
        AST_BASE* type_spec_first_node=NULL;
        for(size_t i=0;i<AST_CHILD_NUM(dec_sq_list_node);++i){
            if((type_spec_first_node=AST_GET_CHILD(dec_sq_list_node,i))->type==type_spec){
                break;
            }
        }
        /*check the struct/union or enum type*/
        if(spec_type==TP_UNION_STRUCT||spec_type==TP_ENUM){
            AST_BASE* spec_node=AST_GET_CHILD(type_spec_first_node,0);
            AST_BASE* kw_spec_node=AST_GET_CHILD(spec_node,0);
            if(spec_type==TP_UNION_STRUCT){
                AST_BASE* kw_node=AST_GET_CHILD(kw_spec_node,0);
                /*decide a struct or an union*/
                if(kw_node->type==KW_struct){
                    spec_type=TP_STRUCT;
                }
                else if(kw_node->type==KW_union){
                    spec_type=TP_UNION;
                }
            }
            /*count the declarator tag,enum,and judge whether an anonymous struct or union*/
            AST_BASE* id_node=AST_GET_CHILD(spec_node,1);
            if(id_node->type!=left_brace){
                (*declaration_cnt)++;
            }
            else    /*an anonymous struct or union*/
                id_node=NULL;
            /*get struct decl_list*/
            AST_BASE* struct_decl_enum_list_node=NULL;
            for(size_t i=0;i<AST_CHILD_NUM(spec_node);++i){
                AST_BASE* tmp_node=AST_GET_CHILD(spec_node,i);
                if(tmp_node->type==struct_decl_list||tmp_node->type==enum_list){
                    struct_decl_enum_list_node=tmp_node;
                    break;
                }
            }
            if(struct_decl_enum_list_node){
                if(spec_type==TP_STRUCT||spec_type==TP_UNION){
                    base_type_ptr=(M_TYPE*)struct_union_type(struct_decl_enum_list_node,id_node,spec_type);
                    if(base_type_ptr)
                        ((TP_SU*)base_type_ptr)->typ_category=spec_type;
                }
                else if(spec_type==TP_ENUM){
                    base_type_ptr=(M_TYPE*)enum_type(struct_decl_enum_list_node,id_node);
                }
                if(base_type_ptr==NULL){
                    return NULL;   /*the error has been inserted*/
                }
            }
            else{
                if(!id_node)
                {
                    return NULL;   /*A struct/union/enum without tag and declaration list, actually impossible*/
                }
                char* tag=(id_node)?(id_node->token->value):NULL;
                SYM_ITEM* tag_symbol_item=find_symbol(dec_sq_list_node->symbol_table,tag,NMSP_SU_TAG);
                if(HASH_ITEM_EXIST(((HASH_ITEM*)tag_symbol_item))){
                    base_type_ptr=Type_VEC_get_actual_base_type(tag_symbol_item->type_vec);
                    if(!base_type_ptr){
                        printf("unfind tag\n"); /*An impossible case,just for test*/
                        return NULL;
                    }
                }
                else{   /*insert symbol table and type table*/
                    base_type_ptr=build_base_type(spec_type);
                    tag_symbol_item=Create_symbol_item(tag,NMSP_SU_TAG);
                    if(tag_symbol_item){    /*tag is no linkage*/
                        VECinsert(tag_symbol_item->type_vec,(void*)base_type_ptr);
                        tag_symbol_item->declared=true;
                        tag_symbol_item->defined=false; /*it declared but not defined*/
                        tag_symbol_item->value=tag;
                        tag_symbol_item->key=SymbolCharToKey(tag,NMSP_SU_TAG);
                        tag_symbol_item->count=HASH_CNT_IST;
                        if(insert_symbol(dec_sq_list_node->symbol_table,tag_symbol_item))
                            id_node->token->symbol_item=tag_symbol_item;
                    }
                }
                id_node->token->symbol_item=(void*)tag_symbol_item;
            }
        }
        else if(spec_type==TP_TYPE_DEF_TYPE){
            /*if a typedef name type,use symbol table find the type struct and copy the type vec to the curr type vec*/
            AST_BASE* type_def_name_node=AST_GET_CHILD(type_spec_first_node,0);
            char* type_def_name_char=type_def_name_node->token->value;
            SYM_ITEM* find_tmphi=Create_symbol_item(type_def_name_char,NMSP_DEFAULT);
            SYM* curr_symbol_table=dec_sq_list_node->symbol_table;
            while(curr_symbol_table){
                HASH_ITEM* tmphi=HASHFind(curr_symbol_table->sym_hash_table,
                    find_tmphi,
                    symbol_item_cmp,
                    false,
                    false
                );
                if(!HASH_ITEM_EXIST(tmphi))
                    curr_symbol_table=curr_symbol_table->father;
                else
                    break;
            }
            m_free(find_tmphi);
            if(curr_symbol_table){
                SYM_ITEM* find_tmpsi=find_symbol_curr_table(curr_symbol_table,
                    type_def_name_char,
                    NMSP_DEFAULT    /*a typedef name must have namespace 1*/
                );
                type_def_name_node->token->symbol_item=find_tmpsi;
                TP_DEF_TYPE* typedef_name_type=(TP_DEF_TYPE*)Type_VEC_get_actual_base_type(find_tmpsi->type_vec);
                if(!typedef_name_type||typedef_name_type->typ_category!=TP_TYPE_DEF_TYPE){
                    C_ERROR(C0017_ERR_TYPEDEF_OVERLAP,dec_sq_list_node);
                    return NULL;
                }
                for(size_t i=0;i<VECLEN(typedef_name_type->typedef_name_type);++i){
                    VECinsert(type_vec,VEC_GET_ITEM(typedef_name_type->typedef_name_type,i));
                }
            }
        }
        else if(spec_type==TP_SPEC_ATOMIC&&Atomic_SPEC_node){
            AST_BASE* type_name_node=AST_GET_CHILD(Atomic_SPEC_node,2);
            VEC* tmp_type_vec=NULL;
            if(!(tmp_type_vec=type_name_type(type_name_node))){
                return NULL;
            }
            /*check the type*/
#if __ATOMIC_SUPPORT==0
            C_ERROR(C0013_ERR_ATOMIC_UNSUPPORT,Atomic_SPEC_node);
            return NULL;
#endif
            M_TYPE* tmpt=NULL;
            tmpt=Type_VEC_get_qual(tmp_type_vec);
            if(tmpt){
                C_ERROR(C0014_ERR_ATOMIC_TYPE_NAME,Atomic_SPEC_node);
                return NULL;
            }
            tmpt=Type_VEC_get_Atomic(tmp_type_vec);
            if(tmpt){
                C_ERROR(C0014_ERR_ATOMIC_TYPE_NAME,Atomic_SPEC_node);
                return NULL;
            }
            tmpt=Type_VEC_get_actual_base_type(tmp_type_vec);
            if(tmpt&&
            (tmpt->typ_category==TP_FUNCTION
            ||tmpt->typ_category==TP_ARRAY))
            {
                C_ERROR(C0014_ERR_ATOMIC_TYPE_NAME,Atomic_SPEC_node);
                return NULL;
            }
            VECappend(tmp_type_vec,type_vec);
            DelVEC(tmp_type_vec);
            base_type_ptr=build_base_type(spec_type);
        }
        else{
            base_type_ptr=build_base_type(spec_type);
        }
    }
#if _CPLX_SUPPORT == 0  /*The implement don't support the _Complex type case*/
    if(type_spec_cnt[10]>0){
        C_ERROR(C0012_ERR_COMPLEX_NOT_SUPPORT,declaration_spec_node);
        return NULL;
    }
#endif
    if(align_spec_cnt>0&&spec_type==TP_TYPE_DEF_TYPE)
    {
        C_ERROR(C0089_ERR_ALIGN_CANNOT_SPECIFIE,dec_sq_list_node);
        return NULL;
    }
    if(align_spec_cnt>0&&storage_spec_cnt[5]!=0)
    {
        C_ERROR(C0089_ERR_ALIGN_CANNOT_SPECIFIE,dec_sq_list_node);
        return NULL;
    }
    /*  
        Third,check qual number and function spec number,
        but this step is meaningless becauese the number of qual or function spec can be multiple
        GCC gives a warning when I try to use 'const const int'
        But for function spec, it seems no such a warning,so "inline inline func()" seems right. Strange
    */
    for(int i=0;i<TYPE_QUAL_NUM;++i){
        if(type_qual_cnt[i]>1){
            AST_BASE* type_qual_node=NULL;
            for(size_t j=0;j<AST_CHILD_NUM(dec_sq_list_node);++j){
                type_qual_node=AST_GET_CHILD(dec_sq_list_node,j);
                type_qual_node=AST_GET_CHILD(type_qual_node,0);
                if(type_qual_node->type==type_qual_enum[i]){
                    break;
                }
            }
            ERROR_ITEM* twi=m_alloc(sizeof(ERROR_ITEM));
            C_WARN(W0004_WARN_DUPLICATE_QUAL,type_qual_node);
            break;
        }
    }
    /*Fourth,build basic type and modify type*/
    if(base_type_ptr!=NULL){
        VECinsert(type_vec,(void*)base_type_ptr);
    }
    modify_type_ptr=build_modify_type(
        storage_spec_type,
        type_qual_cnt,
        func_spec_cnt,
        align_spec_cnt
    );
    if(modify_type_ptr){
        VECinsert(type_vec,(void*)modify_type_ptr);
    }
    /*...finish declaration_spec node check and type build*/
    m_free(tei);
#ifdef _TEST_SEMANTICS_
    for(size_t i=0;i<semantics_level;++i)
        printf("    ");
    printf("finish specify or qualify node\n");
    semantics_level--;
#endif
    return type_vec;
}

VEC* type_name_type(AST_BASE* type_name_node){
    if(!type_name_node||type_name_node->type!=type_name)
        return NULL;
    /*direct declarator is replaced by abstract declarator*/
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    VEC* type_vec=NULL;
    AST_BASE* spec_qual_list_node=AST_GET_CHILD(type_name_node,0);
    AST_BASE* abstract_declarator_node=NULL;
    if(AST_CHILD_NUM(type_name_node)==2)
        abstract_declarator_node=AST_GET_CHILD(type_name_node,1);
    int spec_qual_cnt=0;
    bool type_def_dec=false;
    type_vec=declaration_spec_qual_list_type(
        spec_qual_list_node,
        &spec_qual_cnt,
        &type_def_dec,
        true);
    if(abstract_declarator_node){
        if(!abs_declarator_type(abstract_declarator_node,type_vec))
        {
            return NULL;
        }
    }
    M_TYPE* base_type=Type_VEC_get_actual_base_type(type_vec);
    if(base_type->typ_category==TP_ENUM)
    {
        M_TYPE* new_base_type=build_base_type(TP_SINT);
        if(!Type_VEC_change_actual_base_type(type_vec,new_base_type))
            return NULL;
    }
    m_free(tei);
    return type_vec;
}
bool abs_declarator_type(AST_BASE* abstract_declarator_node,
        VEC* type_vec)
{
    if(!abstract_declarator_node||abstract_declarator_node->type!=abstract_declarator)
        return false;
    if(!type_vec)
        type_vec=InitVEC(DEFAULT_CAPICITY);
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    int spec_qual_cnt=0;
    bool type_def_dec=false;
    AST_BASE* point_node=NULL;
    AST_BASE* direct_abs_dec_node=NULL;
    SYM* curr_table=abstract_declarator_node->symbol_table;
    enum scope_type curr_scope=curr_table->sp_type;
    /*
        pointer part
        here something is different from the declarator part,it might be:
            pointer
            pointer(opt) direct-abstract-declarator
        but the declatator have only :
            pointer(opt) direct-declarator
    */
    for(size_t i=0;i<AST_CHILD_NUM(abstract_declarator_node);++i){
        point_node=AST_GET_CHILD(abstract_declarator_node,i);
        if(point_node->type!=pointer)
            break;
        M_TYPE* point_type=build_base_type(TP_POINT);
        VECinsert(type_vec,(void*)point_type);
        AST_BASE* type_qual_list_node=AST_GET_CHILD(point_node,1);
        if(type_qual_list_node!=NULL){
            VEC* tmpv=InitVEC(DEFAULT_CAPICITY);
            tmpv=declaration_spec_qual_list_type(type_qual_list_node,
                &spec_qual_cnt,
                &type_def_dec,
                false);
            if(tmpv==NULL)
                return false;
            VECappend(tmpv,type_vec);
        }
    }
    /*direct-abstract-declarator part*/
    direct_abs_dec_node=VEC_BACK(abstract_declarator_node->child);
    if(direct_abs_dec_node&&direct_abs_dec_node->type!=pointer){
        int direct_abs_dec_index=AST_CHILD_NUM(direct_abs_dec_node)-1;
        int direct_abs_dec_begin,direct_abs_dec_end;
        while(direct_abs_dec_index>=0){
            direct_abs_dec_begin=direct_abs_dec_end=direct_abs_dec_index;
            int function_array=-1;
            enum rule_type_enum dec_end_type=AST_GET_CHILD(direct_abs_dec_node,direct_abs_dec_end)->type;
            for(int i=direct_abs_dec_end;i>=0;i--){
                enum rule_type_enum tmp_begin_type=AST_GET_CHILD(direct_abs_dec_node,i)->type;
                if(tmp_begin_type==left_bracket&&dec_end_type==right_bracket){
                    direct_abs_dec_begin=i;
                    function_array=0;
                    break;
                }
                else if(tmp_begin_type==left_parenthesis&&dec_end_type==right_parenthesis){
                    direct_abs_dec_begin=i;
                    function_array=1;
                    break;
                }
            }
            direct_abs_dec_index=direct_abs_dec_begin-1;
            if(direct_abs_dec_begin==0&&direct_abs_dec_end==2){
                AST_BASE* sub_abs_declarator_node=AST_GET_CHILD(direct_abs_dec_node,1);
                if(sub_abs_declarator_node->type==abstract_declarator){
                    if(!abs_declarator_type(sub_abs_declarator_node,type_vec))
                    {
                        return NULL;
                    }
                }
            }
            /*deal with function and array type*/
            M_TYPE* tmpt=NULL;
            VEC* tmp_node_vec=InitVEC(DEFAULT_CAPICITY);
            if(direct_abs_dec_begin+1<=direct_abs_dec_end){
                for(size_t i=direct_abs_dec_begin+1;i<direct_abs_dec_end;++i)
                    VECinsert(tmp_node_vec,AST_GET_CHILD(direct_abs_dec_node,i));
                if(function_array==1)   /*function*/
                {
                    tmpt=(M_TYPE*)function_type(tmp_node_vec);
                    if(!tmpt)
                        return false;
                    M_TYPE* tmpfunct=Type_VEC_get_actual_base_type(type_vec);
                    if(tmpfunct->typ_category==TP_FUNCTION||tmpfunct->typ_category==TP_ARRAY)
                    {
                        C_ERROR(C0043_ERR_FUN_RETURN_TYPE,direct_abs_dec_node);
                        return false;
                    }
                }
                else if(function_array==0)  /*array*/
                {
                    tmpt=(M_TYPE*)array_type(tmp_node_vec);
                    if(!tmpt)
                        return false;
                    if(((TP_ARR*)tmpt)->axis_modify){
                        if(abstract_declarator_node->father->type!=parameter_declaration){
                            C_ERROR(C0036_ERR_ARR_QUAL_STATIC_IN_PARA,abstract_declarator_node);
                            return false;
                        }
                        for(int i=VECLEN(type_vec)-1;i>=0;i--){
                            M_TYPE* tmparrt=VEC_GET_ITEM(type_vec,i);
                            if(tmparrt&&tmparrt->typ_category==TP_ARRAY&&((TP_ARR*)tmparrt)->axis_modify){
                                C_ERROR(C0035_ERR_ARR_QUAL_STATIC_OUTMOST,abstract_declarator_node);
                                return false;
                            }
                        }
                    }
                    for(int i=VECLEN(type_vec)-1;i>=0;i--){
                        M_TYPE* tmparrt=VEC_GET_ITEM(type_vec,i);
                        if(tmparrt&&tmparrt->complete==false){
                            C_ERROR(C0040_ERR_ARR_FUNCTIONS_INCOMPLETE,abstract_declarator_node);
                            return false;
                        }
                    }
                    M_TYPE* tmparrt=Type_VEC_get_actual_base_type(type_vec);
                    if(tmparrt&&tmparrt->typ_category==TP_FUNCTION){
                        C_ERROR(C0040_ERR_ARR_FUNCTIONS_INCOMPLETE,abstract_declarator_node);
                        return false;
                    }
                    if(((TP_ARR*)tmpt)->is_star)
                    {
                        if(curr_scope!=SPT_FUN_PROTOTYPE){
                            C_ERROR(C0034_ERR_ARR_STAR_SCOPE,abstract_declarator_node);
                            return false;
                        }
                    }
                }
                else
                    return NULL;
            }
            if(tmpt)
                VECinsert(type_vec,(void*)tmpt);
            else
                ;
        }
    }
    m_free(tei);
    return type_vec;
}
bool initializer_semantic(AST_BASE* initializer_node,VEC* target_type_vec,size_t off,size_t curr_obj_off)
{
    if(!initializer_node||initializer_node->type!=initializer||!target_type_vec)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    AST_BASE* sub_node=AST_GET_CHILD(initializer_node,0);
    /*check the target type, which must be a complete or an array of unknown length type*/
    M_TYPE* tmp_type=Type_VEC_get_actual_base_type(target_type_vec);
    bool type_error=true;
    if((!tmp_type->complete)&&(tmp_type->typ_category==TP_ARRAY)&&((TP_ARR*)tmp_type)->is_vla)
        type_error=false;
    if(tmp_type->complete)
        type_error=false;
    if(type_error)
    {
        C_ERROR(C0091_ERR_INIT_TYPE_COMPLETE,initializer_node);
        goto error;
    }
    if(sub_node->type!=left_brace){
        /*use simple assignment rules to judge the type between target and source*/
        if(!expr_dispatch(sub_node))
            goto error;
        VEC* sub_node_type=sub_node->expr_attribute->type_vec;
        VEC* unary_type_vec=lvalue_convertion(target_type_vec);
        VEC* assign_type_vec=lvalue_convertion(sub_node_type);
        M_TYPE* assign_base_type=Type_VEC_get_actual_base_type(assign_type_vec);
        if(IS_INT_TYPE(assign_base_type->typ_category)&&sub_node->expr_attribute->const_expr){
            long long int null_pointer_value=TP_INT_CAST_TYPE(assign_base_type->typ_category,sub_node->expr_attribute->data_field);
            if(null_pointer_value==0)
                assign_base_type->typ_category=TP_NULL_POINTER_CONSTANT;
        }
        /*please consider the null pointer constant*/
        if(!assignment_type_check(unary_type_vec,assign_type_vec))
            goto error;
    }
    else{
        sub_node=AST_GET_CHILD(initializer_node,1);
        if(!initializer_list_semantic(sub_node,target_type_vec,off,curr_obj_off))
            goto error;
    }
    m_free(tei);
    return true;
error:
    return false;
}
bool initializer_list_semantic(AST_BASE* initializer_list_node,VEC* type_vec,size_t off,size_t curr_obj_off){
    if(!initializer_list_node||initializer_list_node->type!=initializer_list||!type_vec)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    M_TYPE* tmp_type=Type_VEC_get_actual_base_type(type_vec);
    size_t total_size=off+Type_size(type_vec)*8;
    bool array_unknown_size=false;
    bool variable_length_array=false;
    if((!tmp_type->complete)&&(tmp_type->typ_category==TP_ARRAY)&&((TP_ARR*)tmp_type)->is_vla)
        array_unknown_size=true;
    if(tmp_type->complete&&(tmp_type->typ_category==TP_ARRAY)&&((TP_ARR*)tmp_type)->is_vla)
        variable_length_array=true;
    for(size_t i=0;i<AST_CHILD_NUM(initializer_list_node);++i){
        AST_BASE* sub_node=AST_GET_CHILD(initializer_list_node,i);
        if(!variable_length_array&&!array_unknown_size&&off>=total_size)
            break;
        if(sub_node->type==initializer){
            /*test the sub initializer begin with left brace or not*/
            if(!initializer_search(sub_node,type_vec,&off,curr_obj_off,false,0,0))
            {
                C_ERROR(C0072_ERR_ASSIGN_OPERAND,initializer_list_node);
                goto error;
            }
        }
        else if(sub_node->type==designation){
            AST_BASE* designator_list_node=AST_GET_CHILD(sub_node,0);
            /*check the designation*/
            VEC* curr_type_vec=type_vec;
            off=0;
            for(size_t j=0;j<AST_CHILD_NUM(designator_list_node);++j)
            {
                AST_BASE* designator_node=AST_GET_CHILD(designator_list_node,j);
                if(AST_CHILD_NUM(designator_node)==3)   /* '[const expr]' case*/
                {
                    /*check the closest enclose obj type*/
                    M_TYPE* curr_base_type=Type_VEC_get_actual_base_type(curr_type_vec);
                    if(curr_base_type->typ_category!=TP_ARRAY)
                    {
                        C_ERROR(C0093_ERR_INIT_DESIGNATOR_CONST_EXPR,designator_node);
                        goto error;
                    }
                    /*check const expr and integer nonnegative value*/
                    AST_BASE* const_expr_node=AST_GET_CHILD(designator_node,1);
                    if(!const_value(const_expr_node)||
                        !const_expr_node->expr_attribute->const_expr)
                    {   /*const*/
                        C_ERROR(C0093_ERR_INIT_DESIGNATOR_CONST_EXPR,designator_node);
                        goto error;
                    }
                    M_TYPE* const_expr_type=Type_VEC_get_actual_base_type(const_expr_node->expr_attribute->type_vec);
                    if(!IS_INT_TYPE(const_expr_type->typ_category))
                    {   /*integer, you have to check the const first,then to get the type*/
                        C_ERROR(C0093_ERR_INIT_DESIGNATOR_CONST_EXPR,designator_node);
                        goto error;
                    }
                    /*nonnegative*/
                    VALUE_DATA* tmp_data_field=const_expr_node->expr_attribute->data_field;
                    if(
                        ((const_expr_type->typ_category==TP_SCHAR)&&tmp_data_field->schar <=0)
                        ||((const_expr_type->typ_category==TP_SSHORT)&&tmp_data_field->sshort <=0)
                        ||((const_expr_type->typ_category==TP_SINT)&&tmp_data_field->sint <=0)
                        ||((const_expr_type->typ_category==TP_SLONG)&&tmp_data_field->slong <=0)
                        ||((const_expr_type->typ_category==TP_SLONGLONG)&&tmp_data_field->sllong <=0)
                    )
                    {
                        C_ERROR(C0093_ERR_INIT_DESIGNATOR_CONST_EXPR,designator_node);
                        goto error;
                    }
                    /*finish check*/
                    /*change the size and the off and the current type vec*/
                    curr_type_vec=Type_VEC_get_Array_TO(curr_type_vec,true);
                    size_t const_expr_value=(size_t)get_int_const(const_expr_type->typ_category,tmp_data_field,true);
                    off+=const_expr_value*Type_size(curr_type_vec)*8;
                }
                else{       /* '.identifier' case*/
                    /*check the closest enclose obj type*/
                    AST_BASE* identifier_node=AST_GET_CHILD(designator_node,1);
                    M_TYPE* curr_base_type=Type_VEC_get_actual_base_type(curr_type_vec);
                    if(curr_base_type->typ_category!=TP_STRUCT&&curr_base_type->typ_category!=TP_UNION)
                    {
                        C_ERROR(C0094_ERR_INIT_DESIGNATOR_IDENTIFIER,designator_node);
                        goto error;
                    }
                    /*check whether the identifier is one of the member*/
                    VEC* struct_member_list=((TP_SU*)curr_base_type)->members;
                    bool struct_have_identifier_member=false;
                    for(size_t k=0;k<VECLEN(struct_member_list);++k)
                    {
                        TP_SU_MEMBER* tmpmem=VEC_GET_ITEM(struct_member_list,k);
                        if(strcmp(identifier_node->token->value,tmpmem->member_name)==0)
                        {
                            struct_have_identifier_member=true;
                            curr_type_vec=tmpmem->type_vec;
                            off+=tmpmem->offset*8;
                            if(tmpmem->bit_field)
                                off+=tmpmem->bit_field_offset;
                            if(curr_base_type->typ_category==TP_UNION)
                                ((TP_SU*)curr_base_type)->curr_designated_member=tmpmem;
                            break;
                        }
                    }
                    if(!struct_have_identifier_member)
                    {
                        C_ERROR(C0094_ERR_INIT_DESIGNATOR_IDENTIFIER,designator_node);
                        goto error;
                    }
                    /*finish check*/
                }
            }
        }
    }
    if(array_unknown_size)
    {
        VEC* sub_type_vec=Type_VEC_get_Array_TO(type_vec,true);
        size_t sub_type_size=Type_size(sub_type_vec)*8;
        tmp_type->complete=true;
        ((TP_ARR*)tmp_type)->is_vla=false;
        ((TP_ARR*)tmp_type)->axis_size=(off%sub_type_size==0)?(off/sub_type_size):(off/sub_type_size+1);
    }
    m_free(tei);
    return true;
error:
    return false;
}
bool initializer_search(
    AST_BASE* initializer_node,
    VEC* type_vec,
    size_t* off,
    size_t curr_obj_off,
    bool bit_field,
    size_t bit_field_size,
    size_t level)
{
    if(!initializer_node||!type_vec||!off)
        goto error;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    M_TYPE* tmp_type=Type_VEC_get_actual_base_type(type_vec);
    if(IS_SCALAR_TYPE(tmp_type->typ_category)){ /*recursive base*/
        if((*off)!=curr_obj_off)    /*goto the terminal, cannot search deeper*/
        {
            goto error;
        }
        if(!initializer_semantic(initializer_node,type_vec,(*off),curr_obj_off))
        {
            goto error;
        }
        initializer_node->init_attribute->off=curr_obj_off;
        if(bit_field)
        {
            initializer_node->init_attribute->size=bit_field_size;
            (*off)+=bit_field_size;
        }
        else{
            initializer_node->init_attribute->size=Type_size(type_vec)*8;
            (*off)+=Type_size(type_vec)*8;
        }
    }
    else if(tmp_type->typ_category==TP_ENUM)
    {   /*recursive base*/
        if((*off)!=curr_obj_off)    /*goto the terminal, cannot search deeper*/
            goto error;
        VEC* sub_obj_type=InitVEC(DEFAULT_CAPICITY);
        M_TYPE* sint_type=build_base_type(TP_SINT);
        VECinsert(sub_obj_type,sint_type);
        if(!initializer_semantic(initializer_node,sub_obj_type,(*off),curr_obj_off))
            goto error;
        /* a enum cannot be a bit field,so add the sint size*/
        initializer_node->init_attribute->off=curr_obj_off;
        initializer_node->init_attribute->size=Type_size(sub_obj_type)*8;
        (*off)+=Type_size(sub_obj_type)*8;
    }
    else if(tmp_type->typ_category==TP_UNION)
    {
        size_t union_size=Type_size(type_vec)*8;
        VEC* su_member_list=((TP_SU*)tmp_type)->members;
        TP_SU_MEMBER* member=NULL;
        VEC* sub_obj_type_vec=NULL;
        if(((TP_SU*)tmp_type)->curr_designated_member)
            member=((TP_SU*)tmp_type)->curr_designated_member;
        else
        {
            for(size_t j=0;j<VECLEN(su_member_list);++j)
            {
                member=(TP_SU_MEMBER*)VEC_GET_ITEM(su_member_list,j);
                if(member&&member->member_name!=NULL)
                    break;
            }
        }
        if(member&&member->member_name!=NULL)
        {
            sub_obj_type_vec=member->type_vec;
            if(curr_obj_off!=*off)
            {
                if(!initializer_search(
                    initializer_node,
                    sub_obj_type_vec,
                    off,
                    curr_obj_off,
                    member->bit_field,
                    member->bit_field_size,
                    level+1
                ))
                    goto error;
                if((*off)>=curr_obj_off+union_size){
                    ((TP_SU*)tmp_type)->curr_designated_member=NULL;
                }
            }
            else{
                if(initializer_semantic(initializer_node,sub_obj_type_vec,(*off),curr_obj_off))
                {
                    initializer_node->init_attribute->off=curr_obj_off;
                    initializer_node->init_attribute->size=union_size;
                    (*off)+=union_size;
                    ((TP_SU*)tmp_type)->curr_designated_member=NULL;
                    goto succ;
                }
                else{
                    if(!initializer_search(
                        initializer_node,
                        sub_obj_type_vec,
                        off,
                        curr_obj_off,
                        member->bit_field,
                        member->bit_field_size,
                        level+1
                    ))
                        goto error;
                    if((*off)>=curr_obj_off+union_size){
                        ((TP_SU*)tmp_type)->curr_designated_member=NULL;
                    }
                }
            }
        }
        else
        {   /*no such a named member,so no entity to initialize*/
            C_ERROR(C0092_ERR_INIT_ENTITY,initializer_node);
            goto error;
        }
    }
    else if(tmp_type->typ_category==TP_STRUCT)
    {
        bool find_member=false;
        size_t index=0;
        size_t member_bit_field;
        size_t member_bit_field_size;
        TP_SU_MEMBER* member=NULL;
        size_t struct_size=Type_size(type_vec)*8;
        VEC* su_member_list=((TP_SU*)tmp_type)->members;
        VEC* sub_obj_type_vec=NULL;
        size_t struct_start_off=curr_obj_off;
        for(size_t i=0;i<VECLEN(su_member_list);++i)
        {
            member=(TP_SU_MEMBER*)VEC_GET_ITEM(su_member_list,i);
            member_bit_field=0;
            member_bit_field_size=0;
            /*get the member's actual off*/
            curr_obj_off+=member->offset*8;
            if(member->bit_field){
                curr_obj_off+=member->bit_field_offset;
                member_bit_field=true;
                member_bit_field_size=member->bit_field_size;
            }
            size_t member_end=curr_obj_off;
            if(bit_field)
                member_end+=member_bit_field_size;
            else
                member_end+=Type_size(member->type_vec)*8;
            if((curr_obj_off<=(*off)&&member_end>(*off))||(((TP_SU*)tmp_type)->curr_designated_member==member))
            {   /*
                    unlike the union,the struct must calculate some data,so it have to check all
                    actually,I think just record the index in member list might works.
                */
                find_member=true;
                index=i;
                break;
            }
        }
        if(find_member&&member){
            sub_obj_type_vec=member->type_vec;
            if((*off)!=curr_obj_off){
                if(!initializer_search(
                    initializer_node,
                    sub_obj_type_vec,
                    off,
                    curr_obj_off,
                    member_bit_field,
                    member_bit_field_size,
                    level+1
                ))
                    goto error;
                if((*off)>=struct_start_off+struct_size){
                    ((TP_SU*)tmp_type)->curr_designated_member=NULL;
                }
            }
            else{
                if(initializer_semantic(initializer_node,sub_obj_type_vec,(*off),curr_obj_off))
                {
                    initializer_node->init_attribute->off=curr_obj_off;
                    if(index==VECLEN(su_member_list)-1)
                    {
                        initializer_node->init_attribute->size=struct_size;
                        (*off)+=struct_size;
                    }
                    else{
                        TP_SU_MEMBER* next_member=NULL;
                        size_t next_member_off=struct_start_off;
                        /*
                            consider unnamed member case,
                            you have to check all the following members,
                            until you find one that the start bit is diffient with the current one or all the next member are checked 
                            e.x.:
                                struct a{
                                    union{
                                        int b;
                                        int c;
                                    };
                                } d;
                                this union is a unnamed one ,and in member list, only have member b and c,
                                but b and c start from the same position. 
                                so ,if you try to init this struct , only one int is needed
                        */
                        for(size_t j=index+1;j<VECLEN(su_member_list);++j)
                        {
                            next_member=(TP_SU_MEMBER*)VEC_GET_ITEM(su_member_list,index+1);
                            size_t tmp_next_member_off=next_member_off+(next_member->offset)*8; /*start test*/
                            if(next_member->bit_field)
                                tmp_next_member_off+=next_member->bit_field_offset;
                            if(tmp_next_member_off!=curr_obj_off)
                            {
                                next_member_off=tmp_next_member_off;
                                break;
                            }
                        }
                        if(next_member&&next_member_off!=curr_obj_off)
                        {
                            if(member->bit_field)
                                initializer_node->init_attribute->size=member->bit_field_size;
                            else
                                initializer_node->init_attribute->size=Type_size(member->type_vec);
                            (*off)=next_member->offset*8;
                            if(next_member->bit_field)
                                off+=member->bit_field_offset;
                        }
                        else    /*it means no next member start with different offset*/
                        {
                            initializer_node->init_attribute->size=struct_size;
                            (*off)+=struct_size;
                        }
                    }
                }
                else{
                    if(!initializer_search(
                        initializer_node,
                        sub_obj_type_vec,
                        off,
                        curr_obj_off,
                        member_bit_field,
                        member_bit_field_size,
                        level+1
                    ))
                        goto error;
                    if((*off)>=struct_start_off+struct_size){
                        ((TP_SU*)tmp_type)->curr_designated_member=NULL;
                    }
                }
            }
        }
        else
            goto error;
    }
    else if(tmp_type->typ_category==TP_ARRAY)
    {
        VEC* sub_obj_type_vec=Type_VEC_get_Array_TO(type_vec,true);
        size_t sub_type_size=Type_size(sub_obj_type_vec)*8;
        curr_obj_off+=sub_type_size*(((*off)-curr_obj_off)/sub_type_size);
        if(curr_obj_off!=(*off)){
            if(!initializer_search(
                initializer_node,
                sub_obj_type_vec,
                off,
                curr_obj_off,
                false,
                0,
                level+1
            ))
                goto error;
        }
        else{
            if(initializer_semantic(initializer_node,sub_obj_type_vec,(*off),curr_obj_off))
            {
                initializer_node->init_attribute->off=curr_obj_off;
                initializer_node->init_attribute->size=sub_type_size;
                (*off)+=sub_type_size;
                goto succ;
            }
            else{
                if(!initializer_search(
                    initializer_node,
                    sub_obj_type_vec,
                    off,
                    curr_obj_off,
                    false,
                    0,
                    level+1
                ))
                    goto error;
            }
        }
    }
    else
    {
        C_ERROR(C0092_ERR_INIT_ENTITY,initializer_node);
        goto error;
    }
succ:
    m_free(tei);
    return true;
error:
    return false;
}
bool static_assert_type(AST_BASE* static_assert_declaration_node)
{
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    if(!static_assert_declaration_node||static_assert_declaration_node->type!=static_assert_declaration)
        goto error;
    /*TODO*/
    m_free(tei);
    return true;
error:
    C_ERROR(C0021_ERR_STATIC_ASSERT,static_assert_declaration_node);
    return false;
}