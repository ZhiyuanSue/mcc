#include "../defs/defs.h"
#include "mcc_error.h"
#include "vector.h"
#include "../defs/rule.h"
#include "../defs/statement.h"
#include "MEM.h"
#ifndef _RE_H_
#define _RE_H_
#define RULE_CHILD_LEN 32
extern char rule_type_str[STATE_NUM][STATE_CHAR_LEN];

typedef struct base{
    int type;
    char* name;
}BASE;
/*
base's type:
0:a statement, and in a enumerate way
1:a statement, and in a interval way
2:a rule, a link
3:a rule, an or
4:a rule, a star
5:a rule, a question
*/
/*state*/
typedef struct statement{
    BASE base;
    char* value;
}STATE;

/*rules*/
typedef struct rule{
    BASE base;
    enum rule_type_enum type;
    VEC* rule_value;
    unsigned int rule_index;
}RULE;

char* readin_name(char* buf,int length);
char* readin_value(char* buf);
bool is_Match_state(STATE* s,char target);

/*the two name match is use the name search the entity
    actually, I think the strcmp function is also usable
    but, the string that char* point to is not end with '\0',
    it is a substring of rule,so I write one directly*/
void* match_name(char* str,int strlen,VEC* v);
VEC* readin_rule_value(char* buf,VEC* source_v,VEC* dest_v,int* type);

VEC* LexInitSTATE(VEC* sv,char statements[STATE_NUM][STATE_CHAR_LEN]);
VEC* InitRULE(VEC* sv,VEC* rv,char rules[MAX_RULE_NUM][RULE_CHAR_LEN]);

void test_statement(VEC* sv);
void test_rule(VEC* sv,VEC* rv);


#endif