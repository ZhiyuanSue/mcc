
typedef long unsigned int size_t;

typedef long int __off64_t;
typedef long int __off_t;

typedef struct
{
  int __count;
  union
  {
    unsigned int __wch;
    char __wchb[4];
  } __value;
} __mbstate_t;

typedef struct _G_fpos_t
{
  __off_t __pos;
  __mbstate_t __state;
} __fpos_t;
typedef __fpos_t fpos_t;

typedef struct _G_fpos64_t
{
  __off64_t __pos;
  __mbstate_t __state;
} __fpos64_t;

struct _IO_marker;
struct _IO_FILE;
typedef struct _IO_FILE FILE;
typedef void _IO_lock_t;
struct _IO_FILE
{
  int _flags;


  char *_IO_read_ptr;
  char *_IO_read_end;
  char *_IO_read_base;
  char *_IO_write_base;
  char *_IO_write_ptr;
  char *_IO_write_end;
  char *_IO_buf_base;
  char *_IO_buf_end;


  char *_IO_save_base;
  char *_IO_backup_base;
  char *_IO_save_end;

  struct _IO_marker *_markers;

  struct _IO_FILE *_chain;

  int _fileno;
  int _flags2;
  __off_t _old_offset;

  unsigned short _cur_column;
  signed char _vtable_offset;
  char _shortbuf[1];

  _IO_lock_t *_lock;

  __off64_t _offset;

  struct _IO_codecvt *_codecvt;
  struct _IO_wide_data *_wide_data;
  struct _IO_FILE *_freeres_list;
  void *_freeres_buf;
  size_t __pad5;
  int _mode;

  char _unused2[15 * sizeof (int) - 4 * sizeof (void *) - sizeof (size_t)];
};
extern int remove(const char *filename);

extern int rename(const char *old, const char *new);

extern FILE *tmpfile(void);

extern char *tmpnam(char *s);

extern int fclose(FILE *stream);

extern int fflush(FILE *stream);

extern FILE *fopen(const char * restrict filename,
    const char * restrict mode);

extern FILE *freopen(const char * restrict filename,
    const char * restrict mode,
    FILE * restrict stream);

extern void setbuf(FILE * restrict stream,
    char * restrict buf);

extern int setvbuf(FILE * restrict stream,
    char * restrict buf,
    int mode, size_t size);

extern int fprintf(FILE * restrict stream,
    const char * restrict format, ...);

extern int fscanf(FILE * restrict stream,
    const char * restrict format, ...);

extern int printf(const char * restrict format, ...);

extern int scanf(const char * restrict format, ...);

extern int snprintf(char * restrict s, size_t n,
    const char * restrict format, ...);

extern int sprintf(char * restrict s,
    const char * restrict format, ...);

extern int sscanf(const char * restrict s,
    const char * restrict format, ...);

extern char *fgets(char * restrict s, int n,
    FILE * restrict stream);

extern int fputc(int c, FILE *stream);

extern int fputs(const char * restrict s,
    FILE * restrict stream);

extern int getc(FILE *stream);

extern int getchar(void);

extern int putc(int c, FILE *stream);

extern int putchar(int c);

extern int puts(const char *s);

extern int ungetc(int c, FILE *stream);

extern size_t fread(void * restrict ptr,
    size_t size, size_t nmemb,
    FILE * restrict stream);

extern size_t fwrite(const void * restrict ptr,
    size_t size, size_t nmemb,
    FILE * restrict stream);

extern int fgetpos(FILE * restrict stream,
    fpos_t * restrict pos);

extern int fseek(FILE *stream, long int offset, int whence);

extern int fsetpos(FILE *stream, const fpos_t *pos);

extern long int ftell(FILE *stream);

extern void rewind(FILE *stream);

extern void clearerr(FILE *stream);

extern int feof(FILE *stream);

extern int ferror(FILE *stream);

extern void perror(const char *s);

extern int fprintf_s(FILE * restrict stream,
    const char * restrict format, ...);

extern int fscanf_s(FILE * restrict stream,
    const char * restrict format, ...);

extern int printf_s(const char * restrict format, ...);

extern int scanf_s(const char * restrict format, ...);

extern int sscanf_s(const char * restrict s,
    const char * restrict format, ...);
typedef long unsigned int size_t;

extern void *memset(void *s, int c, size_t n);

extern int strcmp(const char *s1, const char *s2);

extern void *memcpy(void * restrict s1,
    const void * restrict s2, size_t n);
extern int *__errno_location(void);
typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef signed char int_least8_t;
typedef short int_least16_t;
typedef int int_least32_t;
typedef long long int_least64_t;
typedef unsigned char uint_least8_t;
typedef unsigned short uint_least16_t;
typedef unsigned int uint_least32_t;
typedef unsigned long long uint_least64_t;

typedef signed char int_fast8_t;
typedef int int_fast16_t;
typedef int int_fast32_t;
typedef long long int_fast64_t;
typedef unsigned char uint_fast8_t;
typedef unsigned int uint_fast16_t;
typedef unsigned int uint_fast32_t;
typedef unsigned long long uint_fast64_t;

typedef long long intmax_t;
typedef unsigned long long uintmax_t;
    typedef unsigned long int uintptr_t;


typedef int wchar_t;
typedef uint_least16_t char16_t;
typedef uint_least32_t char32_t;






typedef struct
{
  int count;
  union
  {
    unsigned int wch;
    char wchb[4];
  } value;
} mbstate_t;




extern size_t mbrtoc16 (char16_t *restrict pc16,
                        const char *restrict s, size_t n,
                        mbstate_t *restrict p);


extern size_t c16rtomb (char *restrict s, char16_t c16,
                        mbstate_t *restrict ps);



extern size_t mbrtoc32 (char32_t *restrict pc32,
                        const char *restrict s, size_t n,
                        mbstate_t *restrict p);


extern size_t c32rtomb (char *restrict s, char32_t c32,
                        mbstate_t *restrict ps);




extern size_t __ctype_get_mb_cur_max (void);

typedef long unsigned int size_t;

extern void *malloc(size_t size);

extern void free(void *ptr);

extern _Noreturn void exit(int status);

extern long int strtol(
    const char * restrict nptr,
    char ** restrict endptr,
    int base);

extern long long int strtoll(
    const char * restrict nptr,
    char ** restrict endptr,
    int base);

extern unsigned long int strtoul(
    const char * restrict nptr,
    char ** restrict endptr,
    int base);

extern unsigned long long int strtoull(
    const char * restrict nptr,
    char ** restrict endptr,
    int base);

extern double strtod(const char * restrict nptr,
    char ** restrict endptr);

extern float strtof(const char * restrict nptr,
    char ** restrict endptr);

extern long double strtold(const char * restrict nptr,
    char ** restrict endptr);

extern int mbtowc(wchar_t *pwc, const char *str, size_t n);

extern int mblen (const char *s, size_t n);
enum compiler_error{
    ERR_NONE,ERR_UNKNOWN,

    ERR_LEX_ERROR,

    ERR_NAMESPACE,ERR_REDEFINE,ERR_NOT_IN_SYMBOL_TABLE,
    ERR_UNEXPECTED_TOKEN,


    ERR_TOO_MANY_STOR_SPEC,
    ERR_THREAD_LOCAL_PREFIX,
    ERR_NEED_MORE_TYPE_SPEC,
    ERR_WRONG_TYPE_SPEC,
    ERR_COMPLEX_NOT_SUPPORT,
    ERR_REDEFINATION,
    ERR_UNDECLARATION,
    ERR_TYPEDEF_OVERLAP,
    ERR_STRUCT_INCOMPLETE_FUNCTION_TYPE,
    ERR_ILLEGAL_CONST_EXPR,
    ERR_CONST_EXPR_RANGE,
    ERR_STATIC_ASSERT,

    ERR_BIT_FIELD_TYPE,
    ERR_BIT_FIELD_WIDTH_EXCEED,
    ERR_BIT_FIELD_WIDTH_INT,
    ERR_BIT_FIELD_WIDTH_NEGATIVE,
    ERR_BIT_FIELD_ZERO_WITH_DECLERATOR,

    ERR_ENUM_NOT_INT,
    ERR_ENUM_RANGE,

    ERR_LKA_EXTERN_FOLLOW_NO,
    ERR_LKA_NO_FOLLOW_EXTERN,
    ERR_CONFICT_TYPE,
    ERR_CONFICT_LINKAGE,
    ERR_CONFICT_THREAD_LOCAL,

    ERR_ARR_STAR_SCOPE,
    ERR_ARR_QUAL_STATIC_OUTMOST,
    ERR_ARR_QUAL_STATIC_IN_PARA,
    ERR_ARR_STATIC_THREAD_WITH_VLA,
    ERR_ARR_SIZE_INTEGER,
    ERR_ARR_SIZE_ZERO,
    ERR_ARR_FUNCTIONS_INCOMPLETE,
    ERR_VM_ID,

    ERR_FUN_IDENTIFIER_LIST,
    ERR_FUN_RETURN_TYPE,
    ERR_FUNC_DEF_DECL_TYPE,
    ERR_FUNC_PARA_INCOMPLETE_TYPE,
    ERR_FUNC_VOID_MORE_THAN_ONE,
    ERR_FUNC_STORAGE,


    WARN_NONE,WARN_UNKNOWN,
    WARN_NO_DECLARATION_ANY_THING,
    WARN_DUPLICATE_QUAL,
};
typedef unsigned int LINE_NUM;
typedef struct line{
    LINE_NUM line_number;
    size_t start;
    size_t end;
}LINE;
typedef struct mcc_error_item{
    enum compiler_error ce;
    char* filename;
    LINE_NUM error_line;
    char* error_token_value;
}ERROR_ITEM;

void m_error(char filename[],char function[],char reason[]);
void m_error_arg_char(char* arg);
void m_error_arg_int(int arg);

void m_test(char test[]);
void m_test_item(char test_item_name[]);
void m_test_arg_char(char* item_arg);
void m_test_arg_int(int item_arg);

void mcc_error(ERROR_ITEM* e,char* file_buf,LINE* line);
inline void* m_error_item(
    ERROR_ITEM* tei,
    enum compiler_error ce,
    char* filename,
    LINE_NUM error_line,
    char* error_token_value)
{
    tei->ce=ce;
    tei->filename=filename;
    tei->error_line=error_line;
    tei->error_token_value=error_token_value;
    return (void*)tei;
}
typedef struct malloc_node M_NODE;
struct malloc_node{
    void* p;
    M_NODE* next;
};

typedef union mem_item{
    struct {
        unsigned short chain_id;
        unsigned short item_id;
        unsigned short next;
        unsigned short size;
    };
    unsigned char ALIGN[((8!=1&&8!=2&&8!=4&&8!=8&&8!=16)?8:8)];
}MEM_ITEM;

typedef struct mem_block MEM_BLOCK;
struct mem_block{
    MEM_BLOCK* next;
    MEM_BLOCK* next_free;
    unsigned short free_item_header;
    void* sys_malloc_res;
    void* data;
};

typedef struct mem_pool{
    char name[8];
    MEM_BLOCK* block_chain[9];
    M_NODE* sys_alloc;
    MEM_BLOCK* free_block_chain[9];
    unsigned long long int malloc_cnt;
    unsigned long long int free_cnt;
    unsigned long long int sys_malloc_cnt;
    unsigned long long int sys_free_cnt;
}MEM_POOL;

inline size_t MEM_GET_SIZE(size_t n){
    if(n<=0)
        return 0;
    if(n<=4)
        return 2;
    if(n>1024)
        return 11;
    int nlevel=0;
    while(n>0){
        n>>=1;
        nlevel++;
    }
    return nlevel;
}

MEM_POOL* MEMInit(char name[]);
void MEMInit_size_array(void);
MEM_BLOCK* MEMInitBlock(size_t chain_num);
void MEMDelBlock(MEM_BLOCK* block);
void MEMDel(MEM_POOL* pool);
void TestMem(void);

void* m_alloc(size_t n);
void m_free(void* p);


typedef unsigned int NMSP;



NMSP GET_NEW_NAMESPACE(void);
void RESET_NAMESPACE(void);


typedef struct hash_table{
    void** data;
    unsigned long long int capicity;
    unsigned long long int capicity_index;
    unsigned long long int record_num;
}HASH;


typedef struct hash_item{
    unsigned long long int key; unsigned int count;
}HASH_ITEM;
inline unsigned long long int HASHFunction(unsigned long long int key,HASH* h)
{
    return key%(h->capicity);
}
HASH* InitHASH(void);
_Bool HSAHExpand(HASH* h,_Bool (*cmp)(void* a,void* b));
void* HASHInsert(HASH* h,HASH_ITEM* item,_Bool (*cmp)(void* a,void* b));
HASH_ITEM* HASHFind(HASH* h,void* a,_Bool (*cmp)(void* a,void* b),_Bool delete_item,_Bool insert_item);
HASH* HASHCOPY(HASH* h);
void DelHASH(HASH* h);
void TestHASH(void);





typedef struct vector{
    void** data;
    size_t len;
    size_t capicity;
}VEC;





VEC* InitVEC(size_t len);
void VECinsert(VEC* v,void* p);
void VECremove(VEC* v,size_t low,size_t high);
void VECpopback(VEC* v);
void VECexpand(VEC* v);
void VECshrink(VEC* v);
void DelVEC(VEC* v);
enum TP_STORAGE_SPEC{
    TP_STOR_NONE,
    TP_TYPE_DEF,
    TP_EXTERN,
    TP_STATIC,
    TP_EXTERN_THREAD_LOCAL,
    TP_AUTO,
    TP_REGISTER,
    TP_STATIC_THREAD_LOCAL
};
enum TP_CATEGORY{
    TP_SPEC_NONE,

    TP_VOID,
    TP_SCHAR,
    TP_USCHAR,
    TP_SSHORT,
    TP_USHORT,
    TP_SINT,
    TP_USINT,
    TP_SLONG,
    TP_USLONG,
    TP_SLONGLONG,
    TP_USLONGLONG,
    TP_FLOAT,
    TP_DOUBLE,
    TP_LONG_DOUBLE,
    TP_BOOL,
    TP_FLOAT_COMPLEX,
    TP_DOUBLE_COMPLEX,
    TP_LONG_DOUBLE_COMPLEX,

    TP_POINT,
    TP_UNION_STRUCT,
    TP_UNION,
    TP_STRUCT,
    TP_ARRAY,
    TP_ENUM,
    TP_FUNCTION,
    TP_TYPE_DEF_TYPE,
    TP_SPEC_ATOMIC,

};
typedef struct {
    unsigned long long int key; unsigned int count;
    _Bool complete; _Bool modifier;
    union{
        struct{
            enum TP_STORAGE_SPEC typ_stor;
            _Bool align_spec;
            unsigned char type_qual;
            unsigned char func_spec;
        };
        struct{
            enum TP_CATEGORY typ_category; unsigned int data_size;
        };
    };
}M_TYPE;

typedef struct {
    unsigned long long int key; unsigned int count;
    _Bool complete; _Bool modifier;
    enum TP_CATEGORY typ_category; unsigned int data_size;
    char* tag;
    VEC* members;
    size_t data_align;
    size_t total_data_size;
    NMSP namespece;
    _Bool can_be_lvalue;
    _Bool have_flex_array;
}TP_SU;
typedef struct {
    char* member_name;
    NMSP name_space;
    size_t offset;
    VEC* type_vec;
    _Bool bit_field;
    size_t bit_field_size;
    size_t bit_field_offset;
}TP_SU_MEMBER;

typedef struct {
    unsigned long long int key; unsigned int count;
    _Bool complete; _Bool modifier;
    enum TP_CATEGORY typ_category; unsigned int data_size;
    _Bool is_vla;
    _Bool is_star;
    union{
        signed long long int axis_size;
        void* assign_expr_node;
    };
    void* axis_modify;
}TP_ARR;

typedef struct {
    unsigned long long int key; unsigned int count;
    _Bool complete; _Bool modifier;
    enum TP_CATEGORY typ_category; unsigned int data_size;
    char* tag;
    VEC* members;
}TP_ENUMERA;

typedef struct {
    unsigned long long int key; unsigned int count;
    _Bool complete; _Bool modifier;
    enum TP_CATEGORY typ_category; unsigned int data_size;
    char* func_name;
    VEC* func_parameters;
    _Bool have_ellipsis;
    _Bool is_old_style;
}TP_FUNC;
typedef struct {
    char* para_name;
    VEC* type_vec;
}TP_FUNC_PARA;

typedef struct {
    unsigned long long int key; unsigned int count;
    _Bool complete; _Bool modifier;
    enum TP_CATEGORY typ_category; unsigned int data_size;
    VEC* typedef_name_type;
}TP_DEF_TYPE;

typedef struct VALUE{
    enum TP_CATEGORY const_expr_type;
    void* const_value;
}CONST_VALUE;

M_TYPE* build_modify_type(
    enum TP_STORAGE_SPEC storage_spec_type,
    int type_qual_cnt[4],
    int func_spec_cnt[2],
    _Bool align_spec_cnt
);
M_TYPE* build_base_type(
    enum TP_CATEGORY spec_type
);
inline CONST_VALUE* create_const_value(enum TP_CATEGORY type,void* value){
    CONST_VALUE* tmpc=m_alloc(sizeof(CONST_VALUE));
    tmpc->const_expr_type=type;
    tmpc->const_value=value;
    return tmpc;
}
_Bool const_value_succ_or_false(CONST_VALUE* cv);
unsigned long long int StrToKey(char* str);
unsigned long long int TypeToKey(M_TYPE* t);
_Bool Type_VEC_cmp(VEC* a,VEC* b);
_Bool Type_item_cmp(void* a,void* b);
_Bool Type_VEC_VM(VEC* type_vec);
_Bool Type_VEC_VLA(VEC* type_vec);
size_t Type_size(VEC* type_vec);
size_t Type_align(VEC* type_vec);

void print_type(M_TYPE* tmpt);


enum scope_type{
    SPT_NONE,SPT_BLOCK,SPT_FUN,SPT_FILE,SPT_FUN_PROTOTYPE
};
enum linkage_type{
    LKA_NONE,LKA_INTERN,LKA_EXTERN
};
typedef struct sym_table SYM;
struct sym_table{
    unsigned int level;
    SYM* father;
    VEC* child_table;
    HASH* sym_hash_table;
    enum scope_type sp_type;
    NMSP name_space;
    HASH* typedef_name_table;
    HASH* enum_const_table;
    HASH* Type_table;
};
typedef struct sym_item{
    unsigned long long int key; unsigned int count;
    VEC* type_vec; size_t data_size; void* data_field;
    char* value;
    NMSP name_space;
    enum linkage_type linkage;
    _Bool defined;
    _Bool declared;
}SYM_ITEM;
SYM* Create_symbol_table(SYM* father,enum scope_type type);
_Bool insert_symbol(SYM* symbol_table,SYM_ITEM* symbol_item);
SYM_ITEM* find_symbol(SYM* symbol_table,char* symbol,NMSP name_space);
SYM_ITEM* find_symbol_curr_table(SYM* symbol_table,char* symbol,NMSP name_space);
void del_symbol(SYM* symbol_table,char* symbol,NMSP name_space);
void Del_symbol_table(SYM* symbol_table);
void print_symbol_table(SYM* symbol_table);
VEC* print_symbol_hash(HASH* h);
unsigned long long int SymbolCharToKey(char* symbol,NMSP name_space);
_Bool symbol_item_cmp(void* a,void* b);
inline SYM_ITEM* Create_symbol_item(char* symbol,NMSP name_space){
    if(symbol==0)
        return 0;
    SYM_ITEM* tmp=m_alloc(sizeof(SYM_ITEM));
    tmp->value=symbol;
    tmp->name_space=name_space;
    tmp->key=SymbolCharToKey(symbol,name_space);
    tmp->count=0;
    tmp->declared=tmp->defined=0;
    tmp->linkage=LKA_NONE;
    tmp->data_size=0;
    tmp->data_field=0;
    tmp->type_vec=InitVEC(3);
    return tmp;
}
inline _Bool Copy_sym_item(SYM_ITEM* new,SYM_ITEM* old){
    if(!old)
        return 0;
    if(!new)
        new=m_alloc(sizeof(SYM_ITEM));
    new->count=old->count;
    new->data_field=old->data_field;
    new->data_size=old->data_size;
    new->declared=old->declared;
    new->defined=old->defined;
    new->key=old->key;
    new->linkage=old->linkage;
    new->name_space=old->name_space;
    new->type_vec=old->type_vec;
    new->value=old->value;
    return 1;
}
enum rule_type_enum{

    child,

    whitespace,KW_auto,KW_break,KW_case,
    KW_char,KW_const,KW_continue,KW_default,
    KW_do,KW_double,KW_else,KW_enum,
    KW_extern,KW_float,KW_for,KW_goto,
    KW_if,KW_inline,KW_int,KW_long,
    KW_register,KW_restrict,KW_return,KW_short,
    KW_signed,KW_sizeof,KW_static,KW_struct,
    KW_switch,KW_type_def,KW_union,KW_unsigned,
    KW_void,KW_volatile,KW_while,KW__Alignas,
    KW__Alignof,KW__Atomic,KW__Bool,KW__Complex,
    KW__Generic,KW__Imaginary,KW__Noreturn,KW__Static_assert,
    KW__Thread_local,KW_asm,KW_asm_volatile,

    identifier,
    integer_constant,
    floating_constant,
    enum_const,
    char_const,
    string,

    left_bracket,right_bracket,left_parenthesis,right_parenthesis,
    left_brace,right_brace,dot,point,
    double_plus,double_minus,ampersand,star,
    plus,minus,tilde,exclamation,
    forward_slash,percent,left_shift,right_shift,
    less_than,greater_than,less_equal,greater_equal,
    double_equal,un_equal,caret,vertical_bar,
    double_ampersand,double_ver_bar,question,colon,
    semi_colon,three_dot,equal,mul_equal,
    forward_equal,percent_equal,plus_equal,minus_equal,
    left_shift_equal,right_shift_equal,ampersand_equal,caret_equal,
    ver_bar_equal,comma,number_sign,double_number_sign,
    less_colon,colon_greater,less_percent,percent_greater,
    percent_colon,double_percent_colon,

    translation_unit,external_decl,function_definition,expression,
    declaration,statement,

    assignment_expr,conditional_expr,
    logical_or_expr,logical_and_expr,inclusive_or_expr,exclusive_or_expr,
    and_expression,equality_expr,relational_expr,shift_expr,
    additive_expr,multi_expr,cast_expr,unary_expr,
    postfix_expr,primary_expression,generic_selection,
    constant_expr,

    declaration_spec,init_decl_list,init_decl,storage_class_spec,
    type_spec,atomic_type_spec,struct_union_spec,struct_union,
    struct_decl_list,struct_decl,spec_qual_list,struct_declarator_list,
    struct_declarator,enum_spec,enum_list,enumerator,enumerator_const,
    type_qual,function_spec,alignment_spec,declarator,
    pointer,direct_declarator,type_qual_list,parameter_type_list,
    parameter_list,parameter_declaration,identifier_list,
    type_name,abstract_declarator,direct_abstract_declarator,type_def_name,
    initializer,initializer_list,designation,designator_list,
    designator,static_assert_declaration,

    labeled_stmt,compound_stmt,expr_stmt,if_stmt,
    switch_stmt,while_stmt,do_stmt,for_stmt,
    goto_stmt,continue_stmt,break_stmt,return_stmt,

    asm_stmt,ASM_extend,
    ASM_output,ASM_input,ASM_register,ASM_restrict,

    end_of_file
};




extern char rule_type_str[256][256];

typedef struct base{
    int type;
    char* name;
}BASE;
typedef struct statement{
    BASE base;
    char* value;
}STATE;


typedef struct rule{
    BASE base;
    enum rule_type_enum type;
    VEC* rule_value;
    unsigned int rule_index;
}RULE;

char* readin_name(char* buf,int length);
char* readin_value(char* buf);
_Bool is_Match_state(STATE* s,char target);





void* match_name(char* str,int strlen,VEC* v);
VEC* readin_rule_value(char* buf,VEC* source_v,VEC* dest_v,int* type);

VEC* LexInitSTATE(VEC* sv,char statements[256][256]);
VEC* InitRULE(VEC* sv,VEC* rv,char rules[1024][256]);

void test_statement(VEC* sv);
void test_rule(VEC* sv,VEC* rv);



typedef char* TK_VALUE;
typedef struct token{
    enum rule_type_enum type;
    size_t value_len;
    TK_VALUE value;
    LINE_NUM line;
    void* symbol_item;
    VEC* parser_token_mem;
}TOKEN;
typedef VEC TOKEN_VEC;
void m_test_token(TOKEN* t);

CONST_VALUE* token_to_const_value(TOKEN* t);
CONST_VALUE* int_const_to_const_value(TOKEN* t);
CONST_VALUE* int_const_int(TOKEN* t);
CONST_VALUE* int_const_uint(TOKEN* t);
CONST_VALUE* int_const_l(TOKEN* t);
CONST_VALUE* int_const_ul(TOKEN* t);
CONST_VALUE* int_const_ll(TOKEN* t);
CONST_VALUE* int_const_ull(TOKEN* t);
CONST_VALUE* float_const_to_const_value(TOKEN* t);
CONST_VALUE* char_const_to_const_value(TOKEN* t);


typedef struct ast_base AST_BASE;
struct ast_base{
    enum rule_type_enum type;
    VEC* child;
    AST_BASE* father;
    SYM* symbol_table;
    TOKEN* token;
    size_t ter_num;
    NMSP name_space;
    VEC* type_vec; size_t data_size; void* data_field;
};

inline AST_BASE* creat_ast_node(enum rule_type_enum type,VEC* child,SYM* symbol_table,TOKEN* token,size_t ter_num,NMSP name_space){
    AST_BASE* node=(AST_BASE*)m_alloc(sizeof(AST_BASE));
    node->type=type;
    node->child=child;
    node->symbol_table=symbol_table;
    node->token=token;
    node->ter_num=ter_num;
    node->name_space=name_space;
    node->father=0;
    node->type_vec=0;
    return node;
}
void destory_ast_tree(AST_BASE* node);
void print_ast_tree(AST_BASE* node,int level);
void destory_subast_sym_table(AST_BASE* node,SYM* top_sym_table);
void destory_ast_symbol(AST_BASE* node);



typedef struct IR_module{
    VEC* func_list;
}IR_MODULE;
typedef struct IR_function{
    VEC* BB_list;
}IR_FUNC;
typedef struct IR_basicblock{
    VEC* Instruction_list;
}IR_BB;




_Bool semantics(AST_BASE* ast_node);


_Bool declaration_type(AST_BASE* ast_node);
_Bool function_definition_type(AST_BASE* ast_node);
_Bool declarator_type(AST_BASE* declarator_node,
        VEC* type_vec,
        int* declarator_cnt,
        _Bool* typedef_declaration,
        NMSP name_space,
        char** declarator_char_name);
_Bool abs_declarator_type(AST_BASE* abstract_declarator_node,
        VEC* type_vec);
VEC* declaration_spec_qual_list_type(AST_BASE* dec_sq_list_node,
        int* declaration_cnt,
        _Bool* typedef_declaration,
        _Bool need_type_spec);
TP_SU* struct_union_type(AST_BASE* struct_decl_list_node,
        AST_BASE* tag_node,
        enum TP_CATEGORY struct_union);
TP_ENUMERA* enum_type(AST_BASE* enumerator_list_node,AST_BASE* tag_node);
TP_FUNC* function_type(VEC* tmp_node_vec);
TP_ARR* array_type(VEC* tmp_node_vec);
VEC* type_name_type(AST_BASE* type_name_node);
_Bool static_assert_type(AST_BASE* static_assert_declaration_node);

_Bool const_value(AST_BASE* ast_node);

_Bool expr_dispatch(AST_BASE* expr_node,_Bool* const_expr);
_Bool cond_expr_value(AST_BASE* ast_node, _Bool* const_expr);
_Bool expr_value(AST_BASE* ast_node, _Bool* const_expr);
_Bool assign_expr_value(AST_BASE* ast_node, _Bool* const_expr);
_Bool logical_or_expr_value(AST_BASE* ast_node, _Bool* const_expr);
_Bool logical_and_expr_value(AST_BASE* ast_node, _Bool* const_expr);
_Bool bit_inclusive_or_expr_value(AST_BASE* ast_node, _Bool* const_expr);
_Bool bit_exclusive_or_expr_value(AST_BASE* ast_node, _Bool* const_expr);
_Bool and_expr_value(AST_BASE* ast_node, _Bool* const_expr);
_Bool equal_expr_value(AST_BASE* ast_node, _Bool* const_expr);
_Bool relation_expr_value(AST_BASE* ast_node, _Bool* const_expr);
_Bool shift_expr_value(AST_BASE* ast_node, _Bool* const_expr);
_Bool add_expr_value(AST_BASE* ast_node, _Bool* const_expr);
_Bool mul_expr_value(AST_BASE* ast_node, _Bool* const_expr);
_Bool cast_expr_value(AST_BASE* ast_node, _Bool* const_expr);
_Bool unary_expr_value(AST_BASE* ast_node, _Bool* const_expr);
_Bool postfix_expr_value(AST_BASE* ast_node, _Bool* const_expr);
_Bool pri_expr_value(AST_BASE* ast_node, _Bool* const_expr);
extern int semantics_level;
extern VEC* c_error;
extern char* filename;
extern size_t type_data_size[28];
const int storage_spec_enum[6]={
    KW_type_def,KW_extern,KW_static,KW__Thread_local,KW_auto,KW_register
    };
const int type_spec_enum[15]={
    KW_void,KW_char,KW_short,KW_int,KW_long,KW_float,KW_double,KW_signed,KW_unsigned,KW__Bool,KW__Complex,
    atomic_type_spec,struct_union_spec,enum_spec,type_def_name
    };
const int type_qual_enum[4]={
    KW_const,KW_restrict,KW_volatile,KW__Atomic
    };
const int func_spec_enum[2]={
    KW_inline,KW__Noreturn
    };

const int type_spec_case[38][15]={
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,1,0,0,0,0,0,1,0,0,0,0,0,0,0},
    {0,1,0,0,0,0,0,0,1,0,0,0,0,0,0},
    {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,1,0,0,0,0,1,0,0,0,0,0,0,0},
    {0,0,1,1,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,1,1,0,0,0,1,0,0,0,0,0,0,0},
    {0,0,1,0,0,0,0,0,1,0,0,0,0,0,0},
    {0,0,1,1,0,0,0,0,1,0,0,0,0,0,0},
    {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
    {0,0,0,1,0,0,0,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
    {0,0,0,1,0,0,0,0,1,0,0,0,0,0,0},
    {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,1,0,0,1,0,0,0,0,0,0,0},
    {0,0,0,1,1,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,1,1,0,0,1,0,0,0,0,0,0,0},
    {0,0,0,0,1,0,0,0,1,0,0,0,0,0,0},
    {0,0,0,1,1,0,0,0,1,0,0,0,0,0,0},
    {0,0,0,0,2,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,2,0,0,1,0,0,0,0,0,0,0},
    {0,0,0,1,2,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,1,2,0,0,1,0,0,0,0,0,0,0},
    {0,0,0,0,2,0,0,0,1,0,0,0,0,0,0},
    {0,0,0,1,2,0,0,0,1,0,0,0,0,0,0},
    {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
    {0,0,0,0,1,0,1,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
    {0,0,0,0,0,1,0,0,0,0,1,0,0,0,0},
    {0,0,0,0,0,0,1,0,0,0,1,0,0,0,0},
    {0,0,0,0,1,0,1,0,0,0,1,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}
};
enum TP_CATEGORY type_spec_case_type[38]={
    TP_VOID,

    TP_SCHAR,TP_SCHAR,
    TP_USCHAR,





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
_Bool declaration_type(AST_BASE* ast_node)
{






    ERROR_ITEM* tei=(ERROR_ITEM*)m_alloc(sizeof(ERROR_ITEM));
    AST_BASE* first_child_node=( (ast_node && (ast_node->child) && (0<(ast_node->child->len)) ) ? (AST_BASE*)((ast_node->child!=0&&0<(ast_node->child->len))?(ast_node->child->data[0]):0): 0 );
    enum scope_type curr_scope=ast_node->symbol_table->sp_type;
    if(first_child_node->type==static_assert_declaration){
        return static_assert_type(first_child_node);
    }
    else{
        int declaration_cnt=0;
        VEC* type_vec=0;
        _Bool typedef_declaration=0;
        AST_BASE* declaration_spec_node=first_child_node;
        AST_BASE* init_declarator_list_node=( (ast_node && (ast_node->child) && (1<(ast_node->child->len)) ) ? (AST_BASE*)((ast_node->child!=0&&1<(ast_node->child->len))?(ast_node->child->data[1]):0): 0 );
        _Bool have_declarator=(init_declarator_list_node->type!=semi_colon);
        if((type_vec=declaration_spec_qual_list_type(
            declaration_spec_node,
            &declaration_cnt,
            &typedef_declaration,
            1))==0)
            return 0;
        if(have_declarator){
            for(size_t i=0;i<( (init_declarator_list_node&& (init_declarator_list_node->child)) ? ((init_declarator_list_node->child->len)) : 0 );i++){
                VEC* tmp_type_v=InitVEC(3);
                for(size_t j=0;j<(type_vec->len);++j){
                    VECinsert(tmp_type_v,((type_vec!=0&&j<(type_vec->len))?(type_vec->data[j]):0));
                }
                char* declarator_char_name;
                AST_BASE* init_dec_node=( (init_declarator_list_node && (init_declarator_list_node->child) && (i<(init_declarator_list_node->child->len)) ) ? (AST_BASE*)((init_declarator_list_node->child!=0&&i<(init_declarator_list_node->child->len))?(init_declarator_list_node->child->data[i]):0): 0 );
                AST_BASE* declarator_node=( (init_dec_node && (init_dec_node->child) && (0<(init_dec_node->child->len)) ) ? (AST_BASE*)((init_dec_node->child!=0&&0<(init_dec_node->child->len))?(init_dec_node->child->data[0]):0): 0 );
                if(declarator_type(declarator_node,
                    tmp_type_v,
                    &declaration_cnt,
                    &typedef_declaration,
                    declarator_node->name_space,
                    &declarator_char_name
                )==0)
                    return 0;
                SYM_ITEM* tmpsi=find_symbol_curr_table(declarator_node->symbol_table,declarator_char_name,declarator_node->name_space);
                if(tmpsi&&Type_VEC_VM(tmp_type_v)){
                    if(tmpsi->linkage!=LKA_NONE
                        ||(curr_scope!=SPT_BLOCK
                        &&curr_scope!=SPT_FUN_PROTOTYPE))
                    {
                        VECinsert(c_error,m_error_item( tei, ERR_VM_ID, filename, declarator_node->token->line, declarator_node->token->value));
                        return 0;
                    }
                }
            }
        }
        else{

        }
        if(declaration_cnt==0){
            ERROR_ITEM* twi=(ERROR_ITEM*)m_alloc(sizeof(ERROR_ITEM));
            VECinsert(c_error,m_error_item( twi, WARN_NO_DECLARATION_ANY_THING, filename, ast_node->token->line, ast_node->token->value));
        }
    }
    m_free(tei);






    return 1;
}
_Bool declarator_type(AST_BASE* declarator_node,
    VEC* type_vec,
    int* declarator_cnt,
    _Bool* typedef_declaration,
    NMSP name_space,
    char** declarator_char_name)
{






    AST_BASE* point_node=0;
    AST_BASE* direct_dec_node=0;
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    for(size_t i=0;i<( (declarator_node&& (declarator_node->child)) ? ((declarator_node->child->len)) : 0 )-1;++i){
        point_node=( (declarator_node && (declarator_node->child) && (i<(declarator_node->child->len)) ) ? (AST_BASE*)((declarator_node->child!=0&&i<(declarator_node->child->len))?(declarator_node->child->data[i]):0): 0 );
        M_TYPE* point_type=build_base_type(TP_POINT);
        VECinsert(type_vec,(void*)point_type);
        AST_BASE* type_qual_list_node=( (point_node && (point_node->child) && (1<(point_node->child->len)) ) ? (AST_BASE*)((point_node->child!=0&&1<(point_node->child->len))?(point_node->child->data[1]):0): 0 );
        if(type_qual_list_node!=0){
            VEC* tmpv=InitVEC(3);
            tmpv=declaration_spec_qual_list_type(type_qual_list_node,
                declarator_cnt,
                typedef_declaration,
                0);
            if(tmpv==0)
                return 0;
            for(size_t j=0;j<(tmpv->len);++j){
                VECinsert(type_vec,((tmpv!=0&&j<(tmpv->len))?(tmpv->data[j]):0));
            }
        }
    }
    direct_dec_node=( (declarator_node && (declarator_node->child) && (( (declarator_node&& (declarator_node->child)) ? ((declarator_node->child->len)) : 0 )-1<(declarator_node->child->len)) ) ? (AST_BASE*)((declarator_node->child!=0&&( (declarator_node&& (declarator_node->child)) ? ((declarator_node->child->len)) : 0 )-1<(declarator_node->child->len))?(declarator_node->child->data[( (declarator_node&& (declarator_node->child)) ? ((declarator_node->child->len)) : 0 )-1]):0): 0 );
    int direct_dec_index=( (direct_dec_node&& (direct_dec_node->child)) ? ((direct_dec_node->child->len)) : 0 )-1;
    int direct_dec_begin=0,direct_dec_end=0;
    _Bool is_vm=1;
    SYM* curr_table=declarator_node->symbol_table;
    enum scope_type curr_scope=curr_table->sp_type;
    while(direct_dec_index>=0){
        direct_dec_begin=direct_dec_end=direct_dec_index;
        int function_array=-1;
        enum rule_type_enum dec_end_type=( (direct_dec_node && (direct_dec_node->child) && (direct_dec_end<(direct_dec_node->child->len)) ) ? (AST_BASE*)((direct_dec_node->child!=0&&direct_dec_end<(direct_dec_node->child->len))?(direct_dec_node->child->data[direct_dec_end]):0): 0 )->type;
        if(dec_end_type==identifier){
            (*declarator_cnt)++;
            TOKEN* tmp_token=( (direct_dec_node && (direct_dec_node->child) && (direct_dec_end<(direct_dec_node->child->len)) ) ? (AST_BASE*)((direct_dec_node->child!=0&&direct_dec_end<(direct_dec_node->child->len))?(direct_dec_node->child->data[direct_dec_end]):0): 0 )->token;
            *declarator_char_name=tmp_token->value;

            SYM_ITEM* tmpsi;
            tmpsi=find_symbol_curr_table(curr_table,
                *declarator_char_name,
                declarator_node->name_space);
            enum linkage_type tmpl=LKA_NONE;
            _Bool should_insert=1;
            for(size_t i=0;i<(type_vec->len);++i){
                M_TYPE* tmpt=(M_TYPE*)((type_vec!=0&&i<(type_vec->len))?(type_vec->data[i]):0);
                if(tmpt->modifier){
                    if(tmpt->typ_stor==TP_STATIC||tmpt->typ_stor==TP_STATIC_THREAD_LOCAL)
                        tmpl=LKA_INTERN;
                    else if(tmpt->typ_stor==TP_EXTERN||tmpt->typ_stor==TP_EXTERN_THREAD_LOCAL)
                        tmpl=LKA_EXTERN;
                    break;
                }
            }

            M_TYPE* tmpt=(M_TYPE*)(type_vec&&type_vec->len>0?type_vec->data[type_vec->len-1]:0);
            if(tmpt->typ_category==TP_FUNCTION)
            {
                for(size_t i=0;i<(type_vec->len);++i){
                    tmpt=((type_vec!=0&&i<(type_vec->len))?(type_vec->data[i]):0);
                    if(tmpt->modifier){
                        if(tmpt->typ_stor==TP_STOR_NONE){
                            tmpt->typ_stor=TP_EXTERN;
                        }
                        break;
                    }
                }
                tmpl=LKA_EXTERN;

                ((TP_FUNC*)tmpt)->func_name=*declarator_char_name;
            }

            if(curr_scope==SPT_FILE){
                if(tmpl==LKA_INTERN)
                    tmpl=LKA_INTERN;
                else{
                    tmpl=LKA_EXTERN;
                }
                if(tmpsi){
                    enum linkage_type prel=tmpsi->linkage;
                    if(tmpl!=prel){
                        VECinsert(c_error,m_error_item( tei, ERR_CONFICT_LINKAGE, filename, declarator_node->token->line, declarator_node->token->value));
                        return 0;
                    }
                    if(!Type_VEC_cmp(tmpsi->type_vec,type_vec))
                    {
                        VECinsert(c_error,m_error_item( tei, ERR_CONFICT_TYPE, filename, declarator_node->token->line, declarator_node->token->value));
                        return 0;
                    }
                    should_insert=0;
                }
            }
            else if(curr_scope==SPT_BLOCK){
                if(tmpl!=LKA_EXTERN)
                    tmpl=LKA_NONE;
                if(tmpsi){

                    enum linkage_type prel=tmpsi->linkage;
                    _Bool redefine=1;

                    if(tmpl==LKA_EXTERN){
                        if(prel!=LKA_EXTERN){
                            VECinsert(c_error,m_error_item( tei, ERR_LKA_EXTERN_FOLLOW_NO, filename, declarator_node->token->line, declarator_node->token->value));
                            return 0;
                        }


                        if(!Type_VEC_cmp(tmpsi->type_vec,type_vec))
                        {
                            VECinsert(c_error,m_error_item( tei, ERR_CONFICT_TYPE, filename, declarator_node->token->line, declarator_node->token->value));
                            return 0;
                        }
                        should_insert=0;
                    }
                    else if(tmpl==LKA_NONE&&prel==LKA_EXTERN){
                        VECinsert(c_error,m_error_item( tei, ERR_LKA_NO_FOLLOW_EXTERN, filename, declarator_node->token->line, declarator_node->token->value));
                        return 0;
                    }
                    else{


                        if(*typedef_declaration){
                            TP_DEF_TYPE* typedef_name_type=((tmpsi->type_vec!=0&&0<(tmpsi->type_vec->len))?(tmpsi->type_vec->data[0]):0);
                            if(typedef_name_type&&typedef_name_type->typedef_name_type){
                                if(Type_VEC_cmp(typedef_name_type->typedef_name_type,type_vec))
                                    redefine=0;
                            }

                            if(is_vm){
                                redefine=1;
                            }




                        }
                        if(redefine){
                            VECinsert(c_error,m_error_item( tei, ERR_REDEFINATION, filename, declarator_node->token->line, declarator_node->token->value));
                            return 0;
                        }
                    }
                }
                else{
                    if(tmpl==LKA_EXTERN)
                    {

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

                        if(tmpsi){
                            if(tmpsi->linkage==LKA_INTERN)
                                tmpl=LKA_INTERN;

                            tmpt=0;
                            enum TP_STORAGE_SPEC prestor=TP_STOR_NONE,tmpstor=TP_STOR_NONE;
                            for(size_t i=0;i<(tmpsi->type_vec->len);++i){
                                tmpt=((tmpsi->type_vec!=0&&i<(tmpsi->type_vec->len))?(tmpsi->type_vec->data[i]):0);
                                if(tmpt->modifier)
                                {
                                    prestor=tmpt->typ_stor;
                                    break;
                                }
                            }
                            for(size_t i=0;i<(type_vec->len);++i){
                                tmpt=((type_vec!=0&&i<(type_vec->len))?(type_vec->data[i]):0);
                                if(tmpt->modifier)
                                {
                                    tmpstor=tmpt->typ_stor;
                                    break;
                                }
                            }
                            if(tmpt&&tmpt->modifier)
                                tmpt->typ_stor=prestor;
                            if((prestor==TP_EXTERN_THREAD_LOCAL&&tmpstor!=TP_EXTERN_THREAD_LOCAL)
                                ||(prestor==TP_STATIC_THREAD_LOCAL&&tmpstor!=TP_STATIC_THREAD_LOCAL))
                            {
                                VECinsert(c_error,m_error_item( tei, ERR_CONFICT_THREAD_LOCAL, filename, declarator_node->token->line, declarator_node->token->value));
                                return 0;
                            }
                            if(!Type_VEC_cmp(tmpsi->type_vec,type_vec))
                            {
                                VECinsert(c_error,m_error_item( tei, ERR_CONFICT_TYPE, filename, declarator_node->token->line, declarator_node->token->value));
                                return 0;
                            }
                            if(tmpt&&tmpt->modifier)
                                tmpt->typ_stor=tmpstor;
                        }
                    }
                }
            }
            else{
                tmpl=LKA_NONE;
            }

            if(Type_VEC_VLA(type_vec)){
                _Bool have_static_thread=0;
                for(size_t i=0;i<(type_vec->len);++i){
                    tmpt=((type_vec!=0&&i<(type_vec->len))?(type_vec->data[i]):0);
                    if(tmpt->modifier){
                        if(tmpt->typ_stor==TP_STATIC||
                        tmpt->typ_stor==TP_EXTERN_THREAD_LOCAL||
                        tmpt->typ_stor==TP_STATIC_THREAD_LOCAL)
                            have_static_thread=1;
                        break;
                    }
                }
                if(have_static_thread){
                    VECinsert(c_error,m_error_item( tei, ERR_ARR_STATIC_THREAD_WITH_VLA, filename, declarator_node->token->line, declarator_node->token->value));
                    return 0;
                }
            }
            if(should_insert)
            {
                tmpsi=Create_symbol_item(*declarator_char_name,declarator_node->name_space);
                tmpsi->linkage=tmpl;
                tmpsi->count=1;
                if(*typedef_declaration){
                    VEC* tmp_type_v=InitVEC(3);
                    TP_DEF_TYPE* typedef_type=(TP_DEF_TYPE*)build_base_type(TP_TYPE_DEF_TYPE);
                    typedef_type->typedef_name_type=type_vec;
                    VECinsert(tmp_type_v,(void*)typedef_type);
                    tmpsi->type_vec=tmp_type_v;
                }
                else
                    tmpsi->type_vec=type_vec;
                if(insert_symbol(declarator_node->symbol_table,tmpsi))
                    tmp_token->symbol_item=tmpsi;

            }
            break;
        }
        for(int i=direct_dec_end;i>=0;i--){
            enum rule_type_enum tmp_begin_type=( (direct_dec_node && (direct_dec_node->child) && (i<(direct_dec_node->child->len)) ) ? (AST_BASE*)((direct_dec_node->child!=0&&i<(direct_dec_node->child->len))?(direct_dec_node->child->data[i]):0): 0 )->type;
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
            AST_BASE* sub_declarator_node=( (direct_dec_node && (direct_dec_node->child) && (1<(direct_dec_node->child->len)) ) ? (AST_BASE*)((direct_dec_node->child!=0&&1<(direct_dec_node->child->len))?(direct_dec_node->child->data[1]):0): 0 );
            if(sub_declarator_node->type==declarator){
                if(!declarator_type(sub_declarator_node,
                    type_vec,
                    declarator_cnt,
                    typedef_declaration,
                    name_space,
                    declarator_char_name
                ))
                    return 0;
                break;
            }
        }

        M_TYPE* tmpt=0;
        VEC* tmp_node_vec=InitVEC(3);

        if(direct_dec_begin+1<=direct_dec_end){
            for(size_t i=direct_dec_begin+1;i<direct_dec_end;++i)
                VECinsert(tmp_node_vec,( (direct_dec_node && (direct_dec_node->child) && (i<(direct_dec_node->child->len)) ) ? (AST_BASE*)((direct_dec_node->child!=0&&i<(direct_dec_node->child->len))?(direct_dec_node->child->data[i]):0): 0 ));
            if(function_array==1)
            {
                tmpt=(M_TYPE*)function_type(tmp_node_vec);
                if(!tmpt)
                    return 0;

                M_TYPE* tmpfunct=(M_TYPE*)(type_vec&&type_vec->len>0?type_vec->data[type_vec->len-1]:0);
                if(tmpfunct->typ_category==TP_FUNCTION||tmpfunct->typ_category==TP_ARRAY)
                {
                    VECinsert(c_error,m_error_item( tei, ERR_FUN_RETURN_TYPE, filename, direct_dec_node->token->line, direct_dec_node->token->value));
                    return 0;
                }
            }
            else if(function_array==0){
                tmpt=(M_TYPE*)array_type(tmp_node_vec);
                if(!tmpt)
                    return 0;
                if(((TP_ARR*)tmpt)->axis_modify){
                    if(declarator_node->father->type!=parameter_declaration){
                        VECinsert(c_error,m_error_item( tei, ERR_ARR_QUAL_STATIC_IN_PARA, filename, declarator_node->token->line, declarator_node->token->value));
                        return 0;
                    }
                    for(int i=(type_vec->len)-1;i>=0;i--){
                        M_TYPE* tmparrt=((type_vec!=0&&i<(type_vec->len))?(type_vec->data[i]):0);
                        if(tmparrt->typ_category==TP_ARRAY&&((TP_ARR*)tmparrt)->axis_modify){
                            VECinsert(c_error,m_error_item( tei, ERR_ARR_QUAL_STATIC_OUTMOST, filename, declarator_node->token->line, declarator_node->token->value));
                            return 0;
                        }
                    }
                }
                for(int i=(type_vec->len)-1;i>=0;i--){
                    M_TYPE* tmparrt=((type_vec!=0&&i<(type_vec->len))?(type_vec->data[i]):0);
                    if(tmparrt->complete==0){
                        VECinsert(c_error,m_error_item( tei, ERR_ARR_FUNCTIONS_INCOMPLETE, filename, declarator_node->token->line, declarator_node->token->value));
                        return 0;
                    }
                }
                M_TYPE* tmparrt=(M_TYPE*)(type_vec&&type_vec->len>0?type_vec->data[type_vec->len-1]:0);
                if(tmparrt->typ_category==TP_FUNCTION){
                    VECinsert(c_error,m_error_item( tei, ERR_ARR_FUNCTIONS_INCOMPLETE, filename, declarator_node->token->line, declarator_node->token->value));
                    return 0;
                }
                if(((TP_ARR*)tmpt)->is_vla)
                    is_vm=1;
                if(((TP_ARR*)tmpt)->is_star)
                {
                    if(curr_scope!=SPT_FUN_PROTOTYPE){
                        VECinsert(c_error,m_error_item( tei, ERR_ARR_STAR_SCOPE, filename, declarator_node->token->line, declarator_node->token->value));
                        return 0;
                    }
                }
            }
            else{
                ;
            }
        }
        if(tmpt)
            VECinsert(type_vec,(void*)tmpt);
        else{
            ;
        }
        direct_dec_index=direct_dec_begin-1;
    }




    m_free(tei);






    return 1;
}
VEC* declaration_spec_qual_list_type(AST_BASE* dec_sq_list_node,
    int* declaration_cnt,
    _Bool* typedef_declaration,
    _Bool need_type_spec)
{
    VEC* type_vec=InitVEC(3);
    enum TP_STORAGE_SPEC storage_spec_type=TP_STOR_NONE;
    enum TP_CATEGORY spec_type=TP_SPEC_NONE;
    M_TYPE* modify_type_ptr=0;
    M_TYPE* base_type_ptr=0;

    int storage_spec_total_cnt=0,storage_spec_cnt[6]={0,0,0,0,0,0};
    int type_spec_total_cnt=0,type_spec_cnt[15]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int type_qual_total_cnt=0,type_qual_cnt[4]={0,0,0,0};
    int func_spec_total_cnt=0,func_spec_cnt[2]={0,0};
    int align_spec_cnt=0;
    ERROR_ITEM* tei=(ERROR_ITEM*)m_alloc(sizeof(ERROR_ITEM));
    for(size_t i=0;i<( (dec_sq_list_node&& (dec_sq_list_node->child)) ? ((dec_sq_list_node->child->len)) : 0 );++i){
        AST_BASE* spec_qual_node=( (dec_sq_list_node && (dec_sq_list_node->child) && (i<(dec_sq_list_node->child->len)) ) ? (AST_BASE*)((dec_sq_list_node->child!=0&&i<(dec_sq_list_node->child->len))?(dec_sq_list_node->child->data[i]):0): 0 );
        AST_BASE* spec_qual_sub_node=( (spec_qual_node && (spec_qual_node->child) && (0<(spec_qual_node->child->len)) ) ? (AST_BASE*)((spec_qual_node->child!=0&&0<(spec_qual_node->child->len))?(spec_qual_node->child->data[0]):0): 0 );
        if(spec_qual_node->type==storage_class_spec){
            for(size_t j=0;j<6;++j){
                if(spec_qual_sub_node->type==storage_spec_enum[j])
                {
                    storage_spec_cnt[j]++;
                    storage_spec_total_cnt++;
                }
            }
        }
        else if(spec_qual_node->type==type_spec){
            for(size_t j=0;j<15;++j){
                if(spec_qual_sub_node->type==type_spec_enum[j])
                {
                    type_spec_cnt[j]++;
                    type_spec_total_cnt++;
                }
            }
        }
        else if(spec_qual_node->type==type_qual){
            for(size_t j=0;j<4;++j){
                if(spec_qual_sub_node->type==type_qual_enum[j])
                {
                    type_qual_cnt[j]++;
                    type_qual_total_cnt++;
                }
            }
        }
        else if(spec_qual_node->type==function_spec){
            for(size_t j=0;j<2;++j){
                if(spec_qual_sub_node->type==func_spec_enum[j])
                {
                    func_spec_cnt[j]++;
                    func_spec_total_cnt++;
                }
            }
        }
        else if(spec_qual_node->type==alignment_spec){
            align_spec_cnt++;
        }
    }


    if(storage_spec_total_cnt>1){
        if(storage_spec_total_cnt==2
            &&storage_spec_cnt[3]==1
            &&(storage_spec_cnt[1]==1
            ||storage_spec_cnt[2]==1
        ))
        {
            storage_spec_type=(storage_spec_cnt[1])?
            TP_EXTERN_THREAD_LOCAL:TP_STATIC_THREAD_LOCAL;
        }
        else
        {

            AST_BASE* storage_spec_node=( (dec_sq_list_node && (dec_sq_list_node->child) && (0<(dec_sq_list_node->child->len)) ) ? (AST_BASE*)((dec_sq_list_node->child!=0&&0<(dec_sq_list_node->child->len))?(dec_sq_list_node->child->data[0]):0): 0 );
            VECinsert(c_error,m_error_item( tei, ERR_TOO_MANY_STOR_SPEC, filename, storage_spec_node->token->line, storage_spec_node->token->value));
            return 0;
        }
    }
    else if(storage_spec_total_cnt==1){
        if(storage_spec_cnt[3]==1
            &&dec_sq_list_node->symbol_table
            &&(dec_sq_list_node->symbol_table->sp_type==SPT_BLOCK)){

            AST_BASE* storage_spec_node=( (dec_sq_list_node && (dec_sq_list_node->child) && (0<(dec_sq_list_node->child->len)) ) ? (AST_BASE*)((dec_sq_list_node->child!=0&&0<(dec_sq_list_node->child->len))?(dec_sq_list_node->child->data[0]):0): 0 );
            VECinsert(c_error,m_error_item( tei, ERR_THREAD_LOCAL_PREFIX, filename, storage_spec_node->token->line, storage_spec_node->token->value));
            return 0;
        }

        for(size_t i=0;i<6;++i){
            if(storage_spec_cnt[i]){
                storage_spec_type=i+1;
            }
        }
    }
    else{
        ;
    }
    if(storage_spec_type==TP_TYPE_DEF){
        *typedef_declaration=1;
    }

    if(type_spec_total_cnt==0&&need_type_spec)
    {

        VECinsert(c_error,m_error_item( tei, ERR_NEED_MORE_TYPE_SPEC, filename, dec_sq_list_node->token->line, dec_sq_list_node->token->value));
        return 0;
    }
    else if(need_type_spec){
        for(size_t i=0;i<38;++i){
            _Bool succ=1;
            for(size_t j=0;j<15;++j){
                if(type_spec_cnt[j]!=type_spec_case[i][j]){
                    succ=0;
                    break;
                }
            }
            if(succ){
                spec_type=type_spec_case_type[i];
                break;
            }
        }
        if(spec_type==TP_SPEC_NONE){

            VECinsert(c_error,m_error_item( tei, ERR_WRONG_TYPE_SPEC, filename, dec_sq_list_node->token->line, dec_sq_list_node->token->value));
            return 0;
        }

        AST_BASE* type_spec_first_node=0;
        for(size_t i=0;i<( (dec_sq_list_node&& (dec_sq_list_node->child)) ? ((dec_sq_list_node->child->len)) : 0 );++i){
            if((type_spec_first_node=( (dec_sq_list_node && (dec_sq_list_node->child) && (i<(dec_sq_list_node->child->len)) ) ? (AST_BASE*)((dec_sq_list_node->child!=0&&i<(dec_sq_list_node->child->len))?(dec_sq_list_node->child->data[i]):0): 0 ))->type==type_spec){
                break;
            }
        }

        if(spec_type==TP_UNION_STRUCT||spec_type==TP_ENUM){
            AST_BASE* spec_node=( (type_spec_first_node && (type_spec_first_node->child) && (0<(type_spec_first_node->child->len)) ) ? (AST_BASE*)((type_spec_first_node->child!=0&&0<(type_spec_first_node->child->len))?(type_spec_first_node->child->data[0]):0): 0 );
            AST_BASE* kw_spec_node=( (spec_node && (spec_node->child) && (0<(spec_node->child->len)) ) ? (AST_BASE*)((spec_node->child!=0&&0<(spec_node->child->len))?(spec_node->child->data[0]):0): 0 );
            if(spec_type==TP_UNION_STRUCT){
                AST_BASE* kw_node=( (kw_spec_node && (kw_spec_node->child) && (0<(kw_spec_node->child->len)) ) ? (AST_BASE*)((kw_spec_node->child!=0&&0<(kw_spec_node->child->len))?(kw_spec_node->child->data[0]):0): 0 );

                if(kw_node->type==KW_struct){
                    spec_type=TP_STRUCT;
                }
                else if(kw_node->type==KW_union){
                    spec_type=TP_UNION;
                }
            }

            AST_BASE* id_node=( (spec_node && (spec_node->child) && (1<(spec_node->child->len)) ) ? (AST_BASE*)((spec_node->child!=0&&1<(spec_node->child->len))?(spec_node->child->data[1]):0): 0 );
            if(id_node->type!=left_brace){
                (*declaration_cnt)++;
            }
            else
                id_node=0;

            AST_BASE* struct_decl_enum_list_node=0;
            for(size_t i=0;i<( (spec_node&& (spec_node->child)) ? ((spec_node->child->len)) : 0 );++i){
                AST_BASE* tmp_node=( (spec_node && (spec_node->child) && (i<(spec_node->child->len)) ) ? (AST_BASE*)((spec_node->child!=0&&i<(spec_node->child->len))?(spec_node->child->data[i]):0): 0 );
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
                if(base_type_ptr==0){
                    return 0;
                }
            }
            else{
                if(!id_node)
                {
                    return 0;
                }
                char* tag=(id_node)?(id_node->token->value):0;
                SYM_ITEM* tag_symbol_item=find_symbol(dec_sq_list_node->symbol_table,tag,2);
                if((((HASH_ITEM*)tag_symbol_item)&&((HASH_ITEM*)tag_symbol_item)->count)){
                    base_type_ptr=(((tag_symbol_item->type_vec)!=0&&0<((tag_symbol_item->type_vec)->len))?((tag_symbol_item->type_vec)->data[0]):0);
                    if(!base_type_ptr){
                        printf("unfind tag\n");
                        return 0;
                    }
                }
                else{
                    base_type_ptr=build_base_type(spec_type);
                    tag_symbol_item=Create_symbol_item(tag,2);
                    if(tag_symbol_item){
                        VECinsert(tag_symbol_item->type_vec,(void*)base_type_ptr);
                        tag_symbol_item->declared=1;
                        tag_symbol_item->value=tag;
                        tag_symbol_item->key=SymbolCharToKey(tag,2);
                        tag_symbol_item->count=1;
                        if(insert_symbol(dec_sq_list_node->symbol_table,tag_symbol_item))
                            id_node->token->symbol_item=tag_symbol_item;
                    }
                }
            }
        }
        else if(spec_type==TP_TYPE_DEF_TYPE){

            AST_BASE* type_def_name_node=( (type_spec_first_node && (type_spec_first_node->child) && (0<(type_spec_first_node->child->len)) ) ? (AST_BASE*)((type_spec_first_node->child!=0&&0<(type_spec_first_node->child->len))?(type_spec_first_node->child->data[0]):0): 0 );
            char* type_def_name_char=type_def_name_node->token->value;
            SYM_ITEM* find_tmphi=Create_symbol_item(type_def_name_char,1);
            SYM* curr_symbol_table=dec_sq_list_node->symbol_table;
            while(curr_symbol_table){
                HASH_ITEM* tmphi=HASHFind(curr_symbol_table->sym_hash_table,
                    find_tmphi,
                    symbol_item_cmp,
                    0,
                    0
                );
                if(!(tmphi&&tmphi->count))
                    curr_symbol_table=curr_symbol_table->father;
                else
                    break;
            }
            m_free(find_tmphi);
            if(curr_symbol_table){
                SYM_ITEM* find_tmpsi=find_symbol_curr_table(curr_symbol_table,
                    type_def_name_char,
                    1
                );
                TP_DEF_TYPE* typedef_name_type=((find_tmpsi->type_vec!=0&&0<(find_tmpsi->type_vec->len))?(find_tmpsi->type_vec->data[0]):0);
                if(!typedef_name_type||typedef_name_type->typ_category!=TP_TYPE_DEF_TYPE){
                    VECinsert(c_error,m_error_item( tei, ERR_TYPEDEF_OVERLAP, filename, dec_sq_list_node->token->line, dec_sq_list_node->token->value));
                    return 0;
                }
                for(size_t i=0;i<(typedef_name_type->typedef_name_type->len);++i){
                    VECinsert(type_vec,((typedef_name_type->typedef_name_type!=0&&i<(typedef_name_type->typedef_name_type->len))?(typedef_name_type->typedef_name_type->data[i]):0));
                }
            }
        }
        else{
            base_type_ptr=build_base_type(spec_type);
        }
    }
    for(int i=0;i<4;++i){
        if(type_qual_cnt[i]>1){
            AST_BASE* type_qual_node=0;
            for(size_t j=0;j<( (dec_sq_list_node&& (dec_sq_list_node->child)) ? ((dec_sq_list_node->child->len)) : 0 );++j){
                type_qual_node=( (dec_sq_list_node && (dec_sq_list_node->child) && (j<(dec_sq_list_node->child->len)) ) ? (AST_BASE*)((dec_sq_list_node->child!=0&&j<(dec_sq_list_node->child->len))?(dec_sq_list_node->child->data[j]):0): 0 );
                type_qual_node=( (type_qual_node && (type_qual_node->child) && (0<(type_qual_node->child->len)) ) ? (AST_BASE*)((type_qual_node->child!=0&&0<(type_qual_node->child->len))?(type_qual_node->child->data[0]):0): 0 );
                if(type_qual_node->type==type_qual_enum[i]){
                    break;
                }
            }
            ERROR_ITEM* twi=m_alloc(sizeof(ERROR_ITEM));
            VECinsert(c_error,m_error_item( twi, WARN_DUPLICATE_QUAL, filename, type_qual_node->token->line, type_qual_node->token->value));
            break;
        }
    }

    if(base_type_ptr!=0){
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

    m_free(tei);






    return type_vec;
}

VEC* type_name_type(AST_BASE* type_name_node){

    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    VEC* type_vec=0;
    AST_BASE* spec_qual_list_node=( (type_name_node && (type_name_node->child) && (0<(type_name_node->child->len)) ) ? (AST_BASE*)((type_name_node->child!=0&&0<(type_name_node->child->len))?(type_name_node->child->data[0]):0): 0 );
    AST_BASE* abstract_declarator_node=0;
    if(( (type_name_node&& (type_name_node->child)) ? ((type_name_node->child->len)) : 0 )==2)
        abstract_declarator_node=( (type_name_node && (type_name_node->child) && (1<(type_name_node->child->len)) ) ? (AST_BASE*)((type_name_node->child!=0&&1<(type_name_node->child->len))?(type_name_node->child->data[1]):0): 0 );
    int spec_qual_cnt=0;
    _Bool type_def_dec=0;
    type_vec=declaration_spec_qual_list_type(
        spec_qual_list_node,
        &spec_qual_cnt,
        &type_def_dec,
        1);
    if(abstract_declarator_node){
        if(!abs_declarator_type(abstract_declarator_node,type_vec))
        {
            return 0;
        }
    }
    m_free(tei);
    return type_vec;
}
_Bool abs_declarator_type(AST_BASE* abstract_declarator_node,
        VEC* type_vec)
{
    type_vec=InitVEC(3);
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    int spec_qual_cnt=0;
    _Bool type_def_dec=0;
    AST_BASE* point_node=0;
    AST_BASE* direct_abs_dec_node=0;
    SYM* curr_table=abstract_declarator_node->symbol_table;
    enum scope_type curr_scope=curr_table->sp_type;

    for(size_t i=0;i<( (abstract_declarator_node&& (abstract_declarator_node->child)) ? ((abstract_declarator_node->child->len)) : 0 )-1;++i){
        point_node=( (abstract_declarator_node && (abstract_declarator_node->child) && (i<(abstract_declarator_node->child->len)) ) ? (AST_BASE*)((abstract_declarator_node->child!=0&&i<(abstract_declarator_node->child->len))?(abstract_declarator_node->child->data[i]):0): 0 );
        M_TYPE* point_type=build_base_type(TP_POINT);
        VECinsert(type_vec,(void*)point_type);
        AST_BASE* type_qual_list_node=( (point_node && (point_node->child) && (1<(point_node->child->len)) ) ? (AST_BASE*)((point_node->child!=0&&1<(point_node->child->len))?(point_node->child->data[1]):0): 0 );
        if(type_qual_list_node!=0){
            VEC* tmpv=InitVEC(3);
            tmpv=declaration_spec_qual_list_type(type_qual_list_node,
                &spec_qual_cnt,
                &type_def_dec,
                0);
            if(tmpv==0)
                return 0;
            for(size_t j=0;j<(tmpv->len);++j){
                VECinsert(type_vec,((tmpv!=0&&j<(tmpv->len))?(tmpv->data[j]):0));
            }
        }
    }

    direct_abs_dec_node=(abstract_declarator_node->child&&abstract_declarator_node->child->len>0?abstract_declarator_node->child->data[abstract_declarator_node->child->len-1]:0);
    if(direct_abs_dec_node){
        int direct_abs_dec_index=( (direct_abs_dec_node&& (direct_abs_dec_node->child)) ? ((direct_abs_dec_node->child->len)) : 0 )-1;
        int direct_abs_dec_begin,direct_abs_dec_end;
        while(direct_abs_dec_index>=0){
            direct_abs_dec_begin=direct_abs_dec_end=direct_abs_dec_index;
            int function_array=-1;
            enum rule_type_enum dec_end_type=( (direct_abs_dec_node && (direct_abs_dec_node->child) && (direct_abs_dec_end<(direct_abs_dec_node->child->len)) ) ? (AST_BASE*)((direct_abs_dec_node->child!=0&&direct_abs_dec_end<(direct_abs_dec_node->child->len))?(direct_abs_dec_node->child->data[direct_abs_dec_end]):0): 0 )->type;
            for(int i=direct_abs_dec_end;i>=0;i--){
                enum rule_type_enum tmp_begin_type=( (direct_abs_dec_node && (direct_abs_dec_node->child) && (i<(direct_abs_dec_node->child->len)) ) ? (AST_BASE*)((direct_abs_dec_node->child!=0&&i<(direct_abs_dec_node->child->len))?(direct_abs_dec_node->child->data[i]):0): 0 )->type;
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
                AST_BASE* sub_abs_declarator_node=( (direct_abs_dec_node && (direct_abs_dec_node->child) && (1<(direct_abs_dec_node->child->len)) ) ? (AST_BASE*)((direct_abs_dec_node->child!=0&&1<(direct_abs_dec_node->child->len))?(direct_abs_dec_node->child->data[1]):0): 0 );
                if(sub_abs_declarator_node->type==abstract_declarator){
                    if(!abs_declarator_type(sub_abs_declarator_node,type_vec))
                    {
                        return 0;
                    }
                }
            }

            M_TYPE* tmpt=0;
            VEC* tmp_node_vec=InitVEC(3);
            if(direct_abs_dec_begin+1<=direct_abs_dec_end){
                for(size_t i=direct_abs_dec_begin+1;i<direct_abs_dec_end;++i)
                    VECinsert(tmp_node_vec,( (direct_abs_dec_node && (direct_abs_dec_node->child) && (i<(direct_abs_dec_node->child->len)) ) ? (AST_BASE*)((direct_abs_dec_node->child!=0&&i<(direct_abs_dec_node->child->len))?(direct_abs_dec_node->child->data[i]):0): 0 ));
                if(function_array==1)
                {
                    tmpt=(M_TYPE*)function_type(tmp_node_vec);
                    if(!tmpt)
                        return 0;
                    M_TYPE* tmpfunct=(M_TYPE*)(type_vec&&type_vec->len>0?type_vec->data[type_vec->len-1]:0);
                    if(tmpfunct->typ_category==TP_FUNCTION||tmpfunct->typ_category==TP_ARRAY)
                    {
                        VECinsert(c_error,m_error_item( tei, ERR_FUN_RETURN_TYPE, filename, direct_abs_dec_node->token->line, direct_abs_dec_node->token->value));
                        return 0;
                    }
                }
                else if(function_array==0)
                {
                    tmpt=(M_TYPE*)array_type(tmp_node_vec);
                    if(!tmpt)
                        return 0;
                    if(((TP_ARR*)tmpt)->axis_modify){
                        if(abstract_declarator_node->father->type!=parameter_declaration){
                            VECinsert(c_error,m_error_item( tei, ERR_ARR_QUAL_STATIC_IN_PARA, filename, abstract_declarator_node->token->line, abstract_declarator_node->token->value));
                            return 0;
                        }
                        for(int i=(type_vec->len)-1;i>=0;i--){
                            M_TYPE* tmparrt=((type_vec!=0&&i<(type_vec->len))?(type_vec->data[i]):0);
                            if(tmparrt->typ_category==TP_ARRAY&&((TP_ARR*)tmparrt)->axis_modify){
                                VECinsert(c_error,m_error_item( tei, ERR_ARR_QUAL_STATIC_OUTMOST, filename, abstract_declarator_node->token->line, abstract_declarator_node->token->value));
                                return 0;
                            }
                        }
                    }
                    for(int i=(type_vec->len)-1;i>=0;i--){
                        M_TYPE* tmparrt=((type_vec!=0&&i<(type_vec->len))?(type_vec->data[i]):0);
                        if(tmparrt->complete==0){
                            VECinsert(c_error,m_error_item( tei, ERR_ARR_FUNCTIONS_INCOMPLETE, filename, abstract_declarator_node->token->line, abstract_declarator_node->token->value));
                            return 0;
                        }
                    }
                    M_TYPE* tmparrt=(M_TYPE*)(type_vec&&type_vec->len>0?type_vec->data[type_vec->len-1]:0);
                    if(tmparrt->typ_category==TP_FUNCTION){
                        VECinsert(c_error,m_error_item( tei, ERR_ARR_FUNCTIONS_INCOMPLETE, filename, abstract_declarator_node->token->line, abstract_declarator_node->token->value));
                        return 0;
                    }
                    if(((TP_ARR*)tmpt)->is_star)
                    {
                        if(curr_scope!=SPT_FUN_PROTOTYPE){
                            VECinsert(c_error,m_error_item( tei, ERR_ARR_STAR_SCOPE, filename, abstract_declarator_node->token->line, abstract_declarator_node->token->value));
                            return 0;
                        }
                    }
                }
                else
                    return 0;
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
_Bool static_assert_type(AST_BASE* static_assert_declaration_node)
{
    ERROR_ITEM* tei=m_alloc(sizeof(ERROR_ITEM));
    if(!static_assert_declaration_node||static_assert_declaration_node->type!=static_assert_declaration)
        goto error;

    free(tei);
    return 1;
error:
    VECinsert(c_error,m_error_item( tei, ERR_STATIC_ASSERT, filename, static_assert_declaration_node->token->line, static_assert_declaration_node->token->value));
    return 0;
}
