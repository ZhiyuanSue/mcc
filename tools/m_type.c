#include "m_type.h"
#ifndef CROSS_CMPL
size_t type_data_size[TYPE_NUM]={
    0,0,sizeof(signed char),sizeof(unsigned char),
    sizeof(signed short),sizeof(unsigned short),
    sizeof(signed int),sizeof(unsigned int),
    sizeof(signed long int),sizeof(unsigned long int),
    sizeof(signed long long int),sizeof(unsigned long long int),
    sizeof(float),sizeof(double),sizeof(long double),sizeof(_Bool),
    sizeof(float _Complex),sizeof(double _Complex),sizeof(long double _Complex),
    sizeof(void*),
    0,0,0,0,sizeof(signed int),0,0,0,0,0   /*to be decided*/
};
#else
/*for a cross complier, the size of int and long double must be redefined*/
#endif
char storage_spec_char_name[STORAGE_NUM][TYPE_CHAR_NAME_LEN]={
    "","typedef","extern","static",
    "extern thread local","auto","register","static thread local"
};
char base_type_char_name[TYPE_NUM][TYPE_CHAR_NAME_LEN]={
    "none","void","signed_char","unsigned_char",
    "signed_short","unsigned_short","signed_int","unsigned_int",
    "signed_long","unsigned_long","signed_long_long_int","unsigned_long_long_int",
    "float","double","long_double","bool",
    "float_complex","double_complex","long_double_complex","point",
    "union/struct","union","struct","array",
    "enum","function","typedef_name","spec_atomic",
    "NULL pointer constant","label"
};
char qual_char_name[QUAL_NUM][TYPE_CHAR_NAME_LEN]={
    /*may be some kind of keyword is unusable*/
    "","atomic","volatile","volatile atomic",
    "restrict","restrict atomic","restrict volatile","restrict volatile atomic",
    "const","const atomic","const volatile","const volatile atomic",
    "const restrict","const restrict atomic","const restrict volatile","const restrict volatile atomic",
};
char func_char_name[FUNCTION_NUM][TYPE_CHAR_NAME_LEN]={
    "","noreturn","inline","inline noreturn"
};

M_TYPE* build_modify_type(
    enum TP_STORAGE_SPEC storage_spec_type,
    int type_qual_cnt[TYPE_QUAL_NUM],
    int func_spec_cnt[FUNCTION_SPEC_NUM],
    unsigned long int align_spec_cnt
){
    M_TYPE* res=m_alloc(sizeof(M_TYPE));
    res->modifier=true;
    res->typ_category=TP_SPEC_NONE;
    res->complete=true;
    res->count=1;
    res->typ_stor=storage_spec_type;
    res->align_spec=align_spec_cnt;
    res->type_qual=0;
    for(size_t i=0;i<TYPE_QUAL_NUM;++i){
        (res->type_qual)<<=1;
        if(type_qual_cnt[i]>0)
            res->type_qual+=1;
    }
    res->func_spec=0;
    for(size_t i=0;i<FUNCTION_SPEC_NUM;++i){
        (res->func_spec)<<=1;
        if(func_spec_cnt[i]>0)
            res->func_spec+=1;
    }
    res->complete=true;
    res->key=TypeToKey(res);
    if((res->typ_stor<=TP_TYPE_DEF)&&res->type_qual==0&&res->func_spec==0&&res->align_spec==0)
        return NULL;  /*modify type might don't exist,so just return NULL,not an empty struct*/
    return res;
}
M_TYPE* build_base_type(
    enum TP_CATEGORY spec_type
){  /*key must be calculated outside,because the member is unknown*/
    if(spec_type==TP_UNION_STRUCT||spec_type==TP_UNION||spec_type==TP_STRUCT){
        TP_SU* res=(TP_SU*)m_alloc(sizeof(TP_SU));
        res->tag=NULL;
        res->modifier=false;
        res->typ_category=spec_type;
        res->count=HASH_CNT_IST;
        res->complete=false;
        res->members=NULL;
        res->can_be_modified_lvalue=true;
        return (M_TYPE*)res;
    }
    else if(spec_type==TP_ARRAY){
        TP_ARR* res=(TP_ARR*)m_alloc(sizeof(TP_ARR));
        res->modifier=false;
        res->typ_category=spec_type;
        res->count=HASH_CNT_IST;
        res->complete=false;
        res->is_star=false;
        res->is_vla=false;
        res->assign_expr_node=NULL;
        res->axis_modify=NULL;
        res->axis_size=0;
        return (M_TYPE*)res;
    }
    else if(spec_type==TP_ENUM){
        TP_ENUMERA* res=(TP_ENUMERA*)m_alloc(sizeof(TP_ENUMERA));
        res->modifier=false;
        res->typ_category=spec_type;
        res->count=HASH_CNT_IST;
        res->members=NULL;
        res->complete=true;
        res->data_size=type_data_size[spec_type];
        return (M_TYPE*)res;
    }
    else if(spec_type==TP_FUNCTION){
        TP_FUNC* res=(TP_FUNC*)m_alloc(sizeof(TP_FUNC));
        res->modifier=false;
        res->typ_category=spec_type;
        res->count=HASH_CNT_IST;
        res->func_name=NULL;
        res->func_parameters=InitVEC(DEFAULT_CAPICITY);
        res->have_ellipsis=false;
        res->is_old_style=false;
        res->complete=true;
        return (M_TYPE*)res;
    }
    else if(spec_type==TP_TYPE_DEF_TYPE){
        TP_DEF_TYPE* res=(TP_DEF_TYPE*)m_alloc(sizeof(TP_DEF_TYPE));
        res->modifier=false;
        res->typ_category=spec_type;
        res->count=HASH_CNT_IST;
        res->typedef_name_type=InitVEC(DEFAULT_CAPICITY);
        res->complete=true;
        return (M_TYPE*)res;
    }
    else{
        M_TYPE* res=(M_TYPE*)malloc(sizeof(M_TYPE));
        res->modifier=false;
        res->typ_category=spec_type;
        res->count=HASH_CNT_IST;
        res->data_size=type_data_size[spec_type];
        res->complete=true;
        if(spec_type==TP_VOID)
            res->complete=false;
        res->key=TypeToKey(res);
        return res;
    }
}
unsigned long long int StrToKey(char* str){
    if(!str){
        return 0;
    }
    unsigned long long int res=0;
    while(*str!='\0'){
        res+=(*str);
        res<<=1;
        str++;
    }
    return res;
}
unsigned long long int TypeToKey(M_TYPE* t)
{
    /*HASH COMMON should not take into consider*/
    unsigned long long int res=0;
    if(t->modifier){
        res=((t->type_qual)<<3)+((t->func_spec)<<1)+((t->align_spec)+t->typ_stor);
    }
    else{
        switch (t->typ_category)
        {
        case TP_UNION_STRUCT:
        case TP_UNION:
        case TP_STRUCT:
        {
            TP_SU* tmpt=(TP_SU*)t;
            return StrToKey(tmpt->tag);
        }
        case TP_ARRAY:{
            TP_ARR* tmpt=(TP_ARR*)t;
            return tmpt->axis_size;
        }
        case TP_ENUM:
        {
            TP_ENUMERA* tmpt=(TP_ENUMERA*)t;
            return StrToKey(tmpt->tag);
        }
        case TP_FUNCTION:
        {
            TP_FUNC* tmpt=(TP_FUNC*)t;
            return StrToKey(tmpt->func_name);
        }
        case TP_TYPE_DEF_TYPE:
        {
            TP_DEF_TYPE* tmpt=(TP_DEF_TYPE*)t;
            for(size_t i=0;i<VECLEN(tmpt->typedef_name_type);++i){
                void* p=VEC_GET_ITEM(tmpt->typedef_name_type,i);
                res+=(unsigned long long int)p;
            }
            return res;
        }
        default:
            return t->typ_category;
        }
    }
    return res;
}


void print_type(M_TYPE* tmpt){
    if(tmpt->modifier){
        if(tmpt->typ_stor&&tmpt->typ_stor>TP_TYPE_DEF){
            printf(" %s ",storage_spec_char_name[tmpt->typ_stor]);
        }
        if(tmpt->type_qual){
            printf(" %s ",qual_char_name[tmpt->type_qual]);
        }
        if(tmpt->func_spec){
            printf(" %s ",func_char_name[tmpt->func_spec]);
        }
        if(tmpt->align_spec)
            printf(" alignment %ld ",tmpt->align_spec);
    }
    else
        printf(" %s ",base_type_char_name[tmpt->typ_category]);
}