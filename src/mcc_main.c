#include "mcc_main.h"

extern char token_rules[MAX_RULE_NUM][RULE_CHAR_LEN];
extern char token_statements[STATE_NUM][STATE_CHAR_LEN];
extern char parsers[MAX_RULE_NUM][RULE_CHAR_LEN];
VEC* svec;  /*lex statement*/
VEC* rvec;  /*lex rule*/
VEC* pvec;  /*parser rule*/
VEC* ivec;  /*instruction vec*/
VEC linevec;   /*lines*/

char* filename;
VEC* c_error;   /*errors*/
VEC* tvec;  /*token*/
AST_BASE* ast_root; /*ast*/
IR_MODULE* ir_module; /*IR*/
MEM_POOL* curr_heap=NULL;    /*heap point*/
MEM_POOL* global_heap;  /*global*/
MEM_POOL* file_heap;    /*just for a single file*/
#ifdef _TEST_SEMANTICS_
int semantics_level;
#endif
void test(void){
    enum EE;
}
int mcc_main(char* filename){
    (****test)();
    char* buf=NULL;
    /*init global data*/
    global_heap=MEMInit("global");
    file_heap=MEMInit("file");
    if(global_heap==NULL||file_heap==NULL)
        goto error;
    curr_heap=global_heap;
    svec=LexInitSTATE(svec,token_statements);
    rvec=InitRULE(svec,rvec,token_rules);
    pvec=InitRULE(rvec,pvec,parsers);
    build_mem(VEC_GET_ITEM(pvec,0));
   
    /*change heap*/
    curr_heap=file_heap;
    c_error=InitVEC(DEFAULT_CAPICITY);
    /*preprocess*/
    buf=prepro(filename);
    /*deal with the preprocessor result*/
    
    /*lex*/
    linevec=*InitVEC(DEFAULT_CAPICITY);
    tvec=lex(buf,svec,rvec,&linevec);
    if(!tvec)
        goto error;
    /*parser*/
    printf("parser start\n\n\n");
    ast_root=parser(tvec,pvec);
    if(!ast_root)
        goto error;
#ifdef _TEST_AST_
    printf("start print ast tree\n");
    print_ast_tree(ast_root,0);
    printf("finish print ast tree\n");
#endif
    /*build type system and do semantics check, some checks is done in build type stage*/
    if(!semantics(ast_root))
        goto error;
#ifdef _TEST_SYMBOL_TABLE_
    if(ast_root){
        printf("start print symbol table\n");
        print_symbol_table(ast_root->symbol_table);
        printf("end print symbol table\n");
    }
#endif
    if(!(ir_module=trans_to_IR(ast_root)))
        goto error;
#ifdef _TEST_IR_
    printf("start print ir\n");
    print_IR(ir_module);
    printf("start print ir\n");
#endif
    /*if there's some warnings, it need be print*/
    for(size_t i=0;i<VECLEN(c_error);++i){
        ERROR_ITEM* ei=(ERROR_ITEM*)VEC_GET_ITEM(c_error,i);
        if(ei->ce>=W0001_WARN_NONE){
            LINE* error_line=(LINE*)VEC_GET_ITEM((&linevec),ei->error_line);
            mcc_error(ei,buf,error_line);
        }
    }
    if(tvec){
        for(size_t i=0;i<VECLEN(tvec);++i){
            TOKEN* tmpt=VEC_GET_ITEM(tvec,i);
            if(tmpt->type==identifier&&tmpt->symbol_item==NULL)
            printf("not decided token %s\n",tmpt->value);
        }
    }
    MEMDel(global_heap);
    MEMDel(file_heap);
#ifdef _TEST_
    printf("test pass!\n");
#endif
    return 0;

error:
    printf("error\n");
    for(size_t i=0;i<VECLEN(c_error);++i){
        ERROR_ITEM* ei=(ERROR_ITEM*)VEC_GET_ITEM(c_error,i);
        LINE* error_line=(LINE*)VEC_GET_ITEM((&linevec),ei->error_line);
        mcc_error(ei,buf,error_line);
    }
    MEMDel(global_heap);
    MEMDel(file_heap);
    return 1;
}