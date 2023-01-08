#include "token.h"
void m_test_token(TOKEN* t){
    printf("line %d <%s : %s>\n",t->line,rule_type_str[t->type],t->value);
}
#ifndef CROSS_CMPL
/*
    for cross complie, things would be different
    emmm,maybe you should redefine all kinds of 'string to a type' function
    because the represent of a value,even the length of the represent would be different
*/
extern size_t type_data_size[TYPE_NUM];
extern VEC* c_error;
extern char* filename;
CONST_VALUE* token_to_const_value(TOKEN* t){
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    if(t==NULL||t->value[0]=='\0'){
        m_free(tei);
        return NULL;
    }
    CONST_VALUE* cv=m_alloc(sizeof(CONST_VALUE));
    cv->const_value=m_alloc(sizeof(VALUE_DATA));
    m_memset(cv->const_value,'\0',sizeof(VALUE_DATA));
    switch(t->type){
        case integer_constant:
            int_const_to_const_value(t,cv);
            break;
        case floating_constant:
            float_const_to_const_value(t,cv);
            break;
        case char_const:
            char_const_to_const_value(t,cv);
            break;
        case enum_const:
            /*
            it will not happen, that's some designation error ,I'm sorry for that
            if you don't understand what I means, just try it and print the ast
            ——the cost of fix this bug seems high,but it affects nothing
            */
        default:{
            /*
            if one identifier has 'const int' type,it should not be tested here
            in this file, the symbol table cannot be accessed
            */
            return NULL;
        }
    }
    return cv;
}

bool int_const_to_const_value(TOKEN* t,CONST_VALUE* cv)
{
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    if(t==NULL||t->value[0]=='\0'){
        m_free(tei);
        return false;
    }
    bool succ=false;
    int prefix=0;   /*0-dec,1-oct,2-hex*/
    char* cp=t->value;
    if(*cp=='0')
    {
        if(*(cp+1)=='\0'){
            cv->const_expr_type=TP_SINT;
            cv->const_value->sint=0;
            return true;
        }
        else if(*(cp+1)=='x'||*(cp+1)=='X'){
            if(*(cp+2)=='\0')   /*'0x' or '0X' is an error,but it can be tested at lex*/{
                m_free(tei);
                return false;
            }
            else
                prefix=2;
        }
        else
            prefix=1;
    }   /*else is dec*/
    if(prefix==0)
    {
        while(is_digit(*cp))
            cp++;
    }
    else if(prefix==1)
    {
        cp++;
        while(is_oct_digit(*cp))
            cp++;
    }
    else if(prefix==2)
    {
        cp+=2;
        while(is_hex_digit(*cp))
            cp++;
    }
    if(*cp=='\0'){
        if(prefix){
            if((succ=int_const_int(t,cv))==NULL)
                succ=int_const_uint(t,cv);
            if(succ==false)
                succ=int_const_l(t,cv);
            if(succ==false)
                succ=int_const_ul(t,cv);
            if(succ==false)
                succ=int_const_ll(t,cv);
            if(succ==false)
                succ=int_const_ull(t,cv);
        }
        else{
            if((succ=int_const_int(t,cv))==NULL){
                succ=int_const_l(t,cv);
            }
            if(succ==false)
                succ=int_const_ll(t,cv);
        }
    }
    else if((*cp)=='u'||(*cp)=='U'){
        if(prefix){
            if((succ=int_const_uint(t,cv))==NULL){
                succ=int_const_ul(t,cv);
            }
            if(succ==false)
                succ=int_const_ull(t,cv);
        }
        else{
            if((succ=int_const_uint(t,cv))==NULL){
                succ=int_const_ul(t,cv);
            }
            if(succ==false)
                succ=int_const_ull(t,cv);
        }
    }
    else if((*cp)=='l'||(*cp)=='L'){
        if(prefix){
            if((succ=int_const_l(t,cv))==NULL){
                succ=int_const_ul(t,cv);
            }
            if(succ==false)
                succ=int_const_ll(t,cv);
            if(succ==false)
                succ=int_const_ull(t,cv);
        }
        else{
            if((succ=int_const_l(t,cv))==NULL){
                succ=int_const_ll(t,cv);
            }
        }
    }
    else if(
        ((*cp)=='u'&&(*(cp+1))=='l')
        ||((*cp)=='u'&&(*(cp+1))=='L')
        ||((*cp)=='U'&&(*(cp+1))=='l')
        ||((*cp)=='U'&&(*(cp+1))=='L')
    )
    {
        if(prefix){
            if((succ=int_const_ul(t,cv))==NULL){
                succ=int_const_ull(t,cv);
            }
        }
        else{
            if((succ=int_const_ul(t,cv))==NULL){
                succ=int_const_ull(t,cv);
            }
        }
    }
    else if(
        ((*cp)=='l'&&(*(cp+1))=='l')
        ||((*cp)=='L'&&(*(cp+1))=='L')
    )
    {
        if(prefix){
            if((succ=int_const_ll(t,cv))==NULL){
                succ=int_const_ull(t,cv);
            }
            
        }
        else{
            succ=int_const_ll(t,cv);
        }
    }
    else if(    /*if cp=='\0',that's first case,if cp+1 =='\0' the upper case is false*/
        (*(cp+1)!='\0')&&   /*and if we don't test cp+1 ,the access of cp+2 will core dump(the lexer promised there must be a '\0' at end)*/
        (((*cp)=='u'&&(*(cp+1))=='l'&&(*(cp+2))=='l')
        ||((*cp)=='u'&&(*(cp+1))=='L'&&(*(cp+2))=='L')
        ||((*cp)=='U'&&(*(cp+1))=='l'&&(*(cp+2))=='l')
        ||((*cp)=='U'&&(*(cp+1))=='L'&&(*(cp+2))=='L'))
    )
    {
        if(prefix){
            succ=int_const_ull(t,cv);
        }
        else{
            succ=int_const_ull(t,cv);
        }
    }
    else{
        m_free(tei);
        return false;
    }
    if(succ==false){
        VECinsert(c_error,m_error_item(
            tei,
            C0020_ERR_CONST_EXPR_RANGE,
            filename,
            t->line,
            t->value
        ));
        return false;
    }
    m_free(tei);
    return succ;
}
bool int_const_int(TOKEN* t,CONST_VALUE* cv)
{
    bool succ=int_const_l(t,cv);
    if(succ){
        if(type_data_size[TP_SINT]==type_data_size[TP_SLONG])
        {
            cv->const_expr_type=TP_SINT;
            return true;
        }
        else{   /*test the value range*/
#ifdef LP64
#if LP64==1
            if((cv->const_value->slong>(long int)INT32_MAX)
                ||(cv->const_value->slong<(long int)INT32_MIN))
            {
                return false;        /*for some case,you have to check whether unsigned int can represent it*/
            }
            else
            {
                cv->const_expr_type=TP_SINT;
                cv->const_value->sint=(int)(cv->const_value->slong);
                return true;
            }
#endif
#else       /*
                in those five models ,only LP64 has different length between int and long,
                so if this case happens,it means you are trying to use another data model
                Just decide it by yourself
            */
            ;
#endif
        }
    }
    else
        return false;
}
bool int_const_uint(TOKEN* t,CONST_VALUE* cv)
{
    bool succ=int_const_ul(t,cv);
    if(succ){
        if(type_data_size[TP_USINT]==type_data_size[TP_USLONG])
        {
            cv->const_expr_type=TP_USINT;
            return true;
        }
        else{   /*test the value range*/
#ifdef LP64
#if LP64==1
            if(cv->const_value->usllong>(unsigned long int)UINT32_MAX)
            {
                return false;
            }
            else
            {
                cv->const_expr_type=TP_USINT;
                cv->const_value->usint=(unsigned int)(cv->const_value->uslong);
                return true;
            }
#endif
#else
            ;
#endif
        }
    }
    else
        return false;
}
bool int_const_l(TOKEN* t,CONST_VALUE* cv)
{
    char* cend=NULL;
    cv->const_expr_type=TP_SLONG;
    cv->const_value->slong=strtol(t->value,&cend,0);
#ifndef _MAC_
    if(errno==ERANGE){
        return false;
    }
#endif    
    return true;
}
bool int_const_ul(TOKEN* t,CONST_VALUE* cv)
{
    char* cend=NULL;
    cv->const_expr_type=TP_USLONG;
    cv->const_value->uslong=strtoul(t->value,&cend,0);
#ifndef _MAC_
    if(errno==ERANGE){
        return false;
    }
#endif
    return true;
}
bool int_const_ll(TOKEN* t,CONST_VALUE* cv)
{
    char* cend=NULL;
    cv->const_expr_type=TP_SLONGLONG;
    cv->const_value->sllong=strtoll(t->value,&cend,0);
#ifndef _MAC_
    if(errno==ERANGE){
        return false;
    }
#endif
    return true;
}
bool int_const_ull(TOKEN* t,CONST_VALUE* cv)
{
    char* cend=NULL;
    cv->const_expr_type=TP_USLONGLONG;
    cv->const_value->usllong=strtoull(t->value,&cend,0);
#ifndef _MAC_
    if(errno==ERANGE||cend){
        return false;
    }
#endif
    return true;
}
bool float_const_to_const_value(TOKEN* t,CONST_VALUE* cv)
{
    char* cp=t->value+t->value_len-1;
    char* endp=NULL;
    if(*cp=='f'||*cp=='F'){
        cv->const_expr_type=TP_FLOAT;
        cv->const_value->datafloat=strtof(t->value,&endp);
        return true;
    }
    else if(*cp=='l'||*cp=='L'){
        cv->const_expr_type=TP_LONG_DOUBLE;
        cv->const_value->datalongdouble=strtold(t->value,&endp);
        return true;
    }
    else
    {
        cv->const_expr_type=TP_DOUBLE;
        cv->const_value->datadouble=strtod(t->value,&endp);
        return true;
    }
}
bool char_const_to_const_value(TOKEN* t,CONST_VALUE* cv)
{
    /*
    the escape chars in source character set has been translated to execution character set at preprossor stage
    */
    /*TODO:test this module*/
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    cv->const_expr_type=TP_SINT;
    char* realchar=m_alloc(sizeof(char)*(t->value_len));
    m_memset(realchar,'\0',(t->value_len));
    char* cp=realchar;
    if(*cp=='L')
    {   /*a wchar_t*/
        for(size_t i=0;i<t->value_len-3;++i){
            realchar[i]=t->value[i+2];
        }
        wchar_t* tmp=m_alloc(sizeof(wchar_t)*(t->value_len));
        int clen=mbtowc(tmp,realchar,(t->value_len));
        if(clen>1)
            goto error;
        cv->const_value->sint=*((int*)tmp); /*do this cast,otherwise the length might less than a length of int*/
    }/*a mbchar16_t */
#ifndef _MAC_
    /*
        I don't know how to say it that I cannot use <uchar.h> under mac OS.
        even I install the 'real' gcc,I still cannot do it
        it might lead to some error
        and if you know how to use the function mbrtoc16 or mbrtoc32,just tell me
        My macOS version is 10.15.7 catalina.
        Both xcode and gcc is invaild here.
    */
    else if(*cp=='u')
    {   
        mbstate_t state;
        for(size_t i=0;i<t->value_len-3;++i){
            realchar[i]=t->value[i+2];
        }
        char16_t* tmp=m_alloc(sizeof(char16_t)*(t->value_len));
        int clen=mbrtoc16(tmp,realchar,MB_CUR_MAX,&state);
        if(clen>1)
            goto error;
        cv->const_value->sint=*((int*)tmp);
    }/*a mbchar32_t */
    else if(*cp=='U')
    {   
        mbstate_t state;
        for(size_t i=0;i<t->value_len-3;++i){
            realchar[i]=t->value[i+2];
        }
        char32_t* tmp=m_alloc(sizeof(char32_t)*(t->value_len));
        int clen=mbrtoc32(tmp,realchar,MB_CUR_MAX,&state);
        if(clen>1)
            goto error;
        cv->const_value->sint=*((int*)tmp);
    }
#endif
    else
    {
        if(t->value_len>7)
            goto error;
        int tmp=0;
        for(size_t i=2;i<t->value_len-1;++i){
            tmp*=0xff+1;
            tmp+=(int)(t->value[i]);
        }
        cv->const_value->sint=tmp;
    }
    return true;
error:
    VECinsert(c_error,m_error_item(
        tei,
        C0020_ERR_CONST_EXPR_RANGE,
        filename,
        t->line,
        t->value
    ));
    return false;
}
#endif