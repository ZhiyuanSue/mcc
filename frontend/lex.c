#include "lex.h"
extern VEC* c_error;
extern char* filename;
VEC* lex(char* buf,VEC* svec,VEC* rvec,VEC* lines){
#ifdef _TEST_
#if 0
    printf("%s",buf);
    printf("\nend of test file\n");
#endif
#endif
    LINE_NUM line_number=0;
    char* file_start=buf;
    size_t line_char_count=0;

    VEC* tv=InitVEC(DEFAULT_CAPICITY);
    int max_match_len,max_error_len;
    int match_num,error_num;
    while(*buf!='\0'){
        if(is_end_of_line(buf)||(((buf-file_start+1)<BUFSIZE)&&*(buf+1)=='\0')){
            LINE* tl=m_alloc(sizeof(LINE));
            tl->line_number=line_number;
            tl->start=line_char_count;
            tl->end=buf-file_start;
#ifdef _SRC_WIN_
            line_char_count=tl->end+1;
#else
            line_char_count=tl->end;
#endif
            line_number++;
            VECinsert(lines,(void*)tl);
        }
        max_match_len=max_error_len=0;
        match_num=error_num=-1;
        for(size_t i=0;i<VECLEN(rvec);++i){
            RULE* tmpr=(RULE*)VEC_GET_ITEM(rvec,i);
            if(*(tmpr->base.name)=='&'){    /* means a child rule */
                continue;}
            int match_len=l_match(buf,tmpr);
            if(match_len>max_match_len){        /*only bigger, the matched rule should be replaced, */
                max_match_len=match_len;        /*for example, a keyword can also be matched to an identifier */
                match_num=i;                    /* so the keyword rule should before the identifier's */
            }
            if(match_len<max_error_len)
            {
                max_error_len=match_len;
                error_num=i;
            }
        }
        if(max_match_len>0){    /* match success */
            if(match_num==0)  /* we have to deal with the whitespaces */
            {
                buf+=1;
                continue;
            }
            TOKEN* t=(TOKEN*)m_alloc(sizeof(TOKEN));
            t->type=((RULE*)VEC_GET_ITEM(rvec,match_num))->type;
            t->value=(char*)m_alloc(max_match_len+1);      /* we need a '\0' at last*/
            memcpy(t->value,buf,max_match_len);
            t->value[max_match_len]='\0';
            t->value_len=(size_t)max_match_len; /*it is promised >0*/
            t->line=line_number;
            t->symbol_item=NULL;
            t->parser_token_mem=InitVEC(DEFAULT_CAPICITY);
            buf+=max_match_len;
            VECinsert(tv,(void*)t);
        }
        else{       /* match fail */
            char* error_char=NULL;
            if(max_error_len<0){        /* match fail but some rule matched a bit */
                /* deal with the error */
                max_error_len=-max_error_len;
                error_char=(char*)m_alloc(sizeof(char)*(max_error_len+1));
                memcpy(error_char,buf,max_error_len);
                error_char[max_error_len]='\0';
                buf+=max_error_len;
            }
            else    /* just no any rule match*/
                buf++;
            ERROR_ITEM* tei=(ERROR_ITEM*)m_alloc(sizeof(ERROR_ITEM));
            tei->ce=C0003_ERR_LEX_ERROR;
            tei->filename=filename;
            tei->error_line=line_number;
            tei->error_token_value=error_char;
            VECinsert(c_error,(void*)tei);
        }
    }
    if(VECLEN(c_error))
    {
        printf("lex error number:%ld\n",VECLEN(c_error));
        return NULL;
    }
    TOKEN* eof=(TOKEN*)m_alloc(sizeof(TOKEN));
    eof->type=end_of_file;
    eof->value=NULL;
    eof->value_len=0;
    eof->line=line_number;
    eof->symbol_item=NULL;
    eof->parser_token_mem=InitVEC(DEFAULT_CAPICITY);
    VECinsert(tv,(void*)eof);
#ifdef _TEST_LEXER_
    m_test("line split");
    char line_buf[BUFSIZE];
    for(size_t i=0;i<VECLEN(lines);++i){
        LINE* tl=VEC_GET_ITEM(lines,i);
        memcpy(line_buf,file_start+tl->start,tl->end-tl->start+1);
        line_buf[tl->end-tl->start+1]='\0';
        printf("(%ld,%ld)\t",tl->start,tl->end);
        printf("%s\n",line_buf);
    }
    printf("total %ld chars\n",buf-file_start+1);
    m_test("token split");
    for(size_t i=0;i<VECLEN(tv);++i){
        printf("index: %ld ",i);
        m_test_token(((TOKEN*)VEC_GET_ITEM(tv,i)));
    }
#endif
    return tv;
}
int l_match(char* buf,void* rule){
    RULE* tmpr=(RULE*)rule;
    int match_len=0;
    int error_len=0;
    int sub_len;
    int* type=(int*)rule;
    int match_time=0;
    switch(*type){
        case 0:
        case 1:
            if(is_Match_state((STATE*)rule,*buf)){
                return 1;
            }
            else
            {
                return 0;
            }
            break;
        case 2:
            for(size_t i=0;i<VECLEN(tmpr->rule_value);++i){
                sub_len=l_match(buf+match_len,VEC_GET_ITEM(tmpr->rule_value,i));
                int* sub_type=(int*)VEC_GET_ITEM(tmpr->rule_value,i);
                /*if type is 4,5,means a star or a question, the sub_len can be 0, otherwise, for a link type 
                    if one character is missed , the link type should be wrong*/
                
                if((*sub_type)<4&&sub_len<=0){
                    if(sub_len==0)
                    {
                        error_len=-match_len;
                    }
                    else if(sub_len<0){
                        error_len=-match_len+sub_len;   /*sub_len<0,so plus it*/
                    }
                    return error_len;
                }
                if((*sub_type)==4&&sub_len<0)
                    sub_len=0;
                if((*sub_type)==4&&sub_len<0){
                    return -match_len+sub_len;
                }
                match_len+=sub_len;
            }
            break;
        case 3:
            for(size_t i=0;i<VECLEN(tmpr->rule_value);++i){
                sub_len=l_match(buf,VEC_GET_ITEM(tmpr->rule_value,i));
                if(sub_len>match_len){
                    match_len=sub_len;
                }
                if(sub_len<error_len)
                    error_len=sub_len;
            }
            if(match_len==0&&error_len<0)
                return error_len;
            break;
        case 4:
            while((sub_len=l_match(buf+match_len,VEC_GET_ITEM(tmpr->rule_value,0)))>0){
                match_len+=sub_len;
            }
            break;
        case 5:
            while((sub_len=l_match(buf+match_len,VEC_GET_ITEM(tmpr->rule_value,0)))>0){
                if(sub_len<0)
                    return -match_len+sub_len;
                match_len+=sub_len;
                match_time++;
            }
            if(match_time==1)
                return match_len;
            else if(match_time==0)
                return 0;
            else
                return -match_len;
            break;
        default:
            m_error("lex.c","match","undefined type");
            return 0;
            break;
    }
    return match_len;
}