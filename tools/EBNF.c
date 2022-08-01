#include "EBNF.h"
VEC* LexInitSTATE(VEC* sv,char statements[STATE_NUM][STATE_CHAR_LEN]){
    sv=InitVEC(DEFAULT_CAPICITY);
    /*read in state*/
    char buf[STATE_CHAR_LEN];
    for(int i=0;i<STATE_NUM;++i){
        memset(buf,'\0',STATE_CHAR_LEN);
        memcpy(buf,statements[i],STATE_CHAR_LEN);
        if((*buf)=='\0')
            continue;
        STATE* tmps=(struct statement*)m_alloc(sizeof(struct statement));
        char* p=buf;
        while((*p)!=':'){ p++; }
        if((*(p+2))=='-')
            tmps->base.type=1;
        else
            tmps->base.type=0;
        tmps->base.name=readin_name((char*)&buf,p-buf);
        tmps->value=readin_value((p+1));
        VECinsert(sv,(void*)tmps);
    }
    /*test*/
#ifdef _TEST_
#if 0
    test_statement(sv);
#endif
#endif
    return sv;
}
VEC* InitRULE(VEC* sv,VEC* rv,char rules[MAX_RULE_NUM][RULE_CHAR_LEN]){
    rv=InitVEC(DEFAULT_CAPICITY);
    /*readin rules*/
    char buf[RULE_CHAR_LEN];
    unsigned int rule_index=0;
    for(int i=0;i<MAX_RULE_NUM;++i){
        memset(buf,'\0',RULE_CHAR_LEN);
        memcpy(buf,rules[i],RULE_CHAR_LEN);
        if((*buf)=='\0')
            continue;
        RULE* tmpr=(RULE*)m_alloc(sizeof(struct rule));
        if(tmpr==NULL){
            m_error("EBNF.c","InitRULE","m_alloc_error");
            exit(1);
        }
        char* p=buf;
        while((*p)!=':'){ p++; }
        tmpr->base.name=readin_name((char*)&buf,p-buf);    /*it's same,no need to build another one*/
        int index;
        int match_succ=-1;
        for(index=0;index<STATE_NUM;++index){
            if(strcmp(rule_type_str[index],tmpr->base.name)==0)
            {
                match_succ=0;
                break;
            }
        }
        if(match_succ==-1)
            tmpr->type=0;
        else
            tmpr->type=(enum rule_type_enum)index;
        tmpr->rule_index=rule_index;
        rule_index++;
        VECinsert(rv,(void*)tmpr);
    }
    RULE* tmpr;
    for(int i=0;i<VECLEN(rv);++i){
        memset(buf,'\0',RULE_CHAR_LEN);
        memcpy(buf,rules[i],RULE_CHAR_LEN);
        char* p=buf;
        while((*p)!=':'){ p++; }
        tmpr=(RULE*)VEC_GET_ITEM(rv,i);
        if(tmpr==NULL){
            m_error("EBNF.c","InitRULE","vector error");
            exit(1);
        }
        tmpr->rule_value=readin_rule_value((p+1),sv,rv,&tmpr->base.type);
    }
#ifdef _TEST_
#if 0
    test_rule(sv,rv);
#endif
#endif
    return rv;
}
char* readin_name(char* buf,int length){
    char* res;
    res=(char*)m_alloc(sizeof(char)*(length+1));
    memcpy(res,buf,length*sizeof(char));
    res[length]='\0';
    return res;
}
char* readin_value(char* buf){
    char* res;
    if((*(buf+1))=='-'){        /*because even the empty one ,it still have '\n' and '\0',so it's safe*/
        res=(char*)m_alloc(sizeof(char)*3);
        (*res)=(*buf);
        (*(res+1))=(*(buf+2));
        (*(res+2))='\0';
    }
    else{
        char tmp[BUFSIZE/2];
        memset(tmp,'\0',BUFSIZE/2);
        char* p=buf;
        int plen=0;
        while((*p)!='\0'){
            tmp[plen]=(*p);
            plen++;
            p++;
        }
        res=(char*)m_alloc(sizeof(char)*(plen+1));
        memcpy(res,&tmp,plen+1);
    }
    return res;
}
bool is_Match_state(STATE* s,char target){
    if(s->base.type==0){
        int i=0;
        while(s->value[i]!='\0'){
            if(s->value[i]==target)
                return true;
            i++;
        }
        return false;
    }
    else if(s->base.type==1){
        if(target>=s->value[0]&&target<=s->value[1]){
            return true;
        }
        else
            return false;
    }
    else
        return false;
}
void* match_name(char* str,int strlen,VEC* v){
    BASE* q;
    if(strlen==-1){ /*-1 means the string is end with a '\0'*/
        for(size_t i=0;i<VECLEN(v);++i){
            q=(BASE*)VEC_GET_ITEM(v,i);
            if(strcmp(q->name,str)==0)
                return q;
        }
    }
    else{
        for(size_t i=0;i<VECLEN(v);++i){
            q=(BASE*)VEC_GET_ITEM(v,i);
            bool match=true;
            int p=0;
            while(q->name[p]!='\0'&&p<=strlen){
                if(q->name[p]!=str[p])
                {
                    match=false;
                    break;
                }
                p++;
            }
            if(p<strlen)   /*we need max length match*/
                match=false;
            if(match){
                return q;
            }
        }
    }
    
    return NULL;
}

VEC* readin_rule_value(char* buf,VEC* source_v,VEC* dest_v,int* type)
{
    char* p=buf;
    char* start;
    *type=-1;
    VEC* rule_value=InitVEC(DEFAULT_CAPICITY);
    while((*p)!='\0'){
        switch (*p){
            case '$':
                start=p+2;
                p=start;
                while((*p)!='}'){
                    p++;
                }
                void* tmps=match_name(start,p-start,source_v);
                if(tmps==NULL)
                {
                    m_error("EBNF.c","readin_rule_value","illegal input:need more state:");
                    m_error_arg_char(start);
                    exit(1);
                }
                VECinsert(rule_value,tmps);
                break;
            case '&':
                start=p+2;
                p=start;
                while((*p)!='}'){
                    p++;
                }
                void* tmpd=match_name(start,p-start,dest_v);
                if(tmpd==NULL)
                {
                    m_error("EBNF.c","readin_rule_value","illegal input:need more rule:");
                    m_error_arg_char(buf);
                    exit(1);
                }
                VECinsert(rule_value,tmpd);
                break;
            case '|':
                *type=3;
                break;
            case '*':
                *type=4;
                break;
            case '\?':
                *type=5;
                break;
            default:
                goto final;
                break;
        }
        p++;
    }
final:
    if(*type==-1){
        *type=2;
    }
    return rule_value;
}
void test_statement(VEC* sv){
    m_test("statement table");
    STATE* tsts;
    for(int i=0;i<VECLEN(sv);++i){
        tsts=(STATE*)VEC_GET_ITEM(sv,i);
        m_test_item("state");
        m_test_arg_char(tsts->base.name);
        m_test_item("value");
        m_test_arg_char(tsts->value);
    }
}
void test_rule(VEC* sv,VEC* rv){
    m_test("rule table");
    RULE* tstr;
    size_t max_child=0;
    int index=-1;
    for(int i=0;i<VECLEN(rv);++i){
        tstr=(RULE*)VEC_GET_ITEM(rv,i);
        m_test_item("rule");
        m_test_arg_char(tstr->base.name);
        m_test_item("rule_type");
        if(tstr->base.type==2)
            m_test_arg_char("link");
        else if(tstr->base.type==3)
            m_test_arg_char("or");
        else if(tstr->base.type==4)
            m_test_arg_char("star");
        else if(tstr->base.type==5)
            m_test_arg_char("question");
        else{
            m_error("EBNF.c","InitRE","undefined type");
            m_test_arg_int(tstr->base.type);
        }
        m_test_item("rule_type");
        m_test_arg_int(tstr->type);
        m_test_item("rule_arguments");
        if(tstr->base.type==2)
        {
            if(VECLEN((tstr->rule_value))>max_child){
                max_child=VECLEN((tstr->rule_value));
                index=i;
            }
        }
        for(size_t i=0;i<VECLEN((tstr->rule_value));++i){
            void* tstv=VEC_GET_ITEM((tstr->rule_value),i);
            int* tstp=(int*)tstv;
            if(*tstp==0||*tstp==1){
                m_test_item("\t#state");
                STATE* tstt=(STATE*)tstv;
                m_test_arg_char(tstt->base.name);
            }
            else{
                m_test_item("\t#rule");
                RULE* tstt=(RULE*)tstv;
                m_test_arg_char(tstt->base.name);
            }
        }
    }
    printf("max child len is %lu,index is %d\n",max_child,index);
}
