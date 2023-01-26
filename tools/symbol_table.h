#ifndef _SYMBOL_TABLE_H_
#define _SYMBOL_TABLE_H_
#include "hash.h"
#include "vector.h"
#include "namespace.h"
#include "m_type.h"
#include "m_type_vec.h"
#include "../IR/IR_ENUM.h"
#define PRINT_SYMBOLE_TABLE 1
#define PRINT_TYPEDEF_TABLE 0
enum scope_type{
    SPT_NONE,SPT_BLOCK,SPT_FUN,SPT_FILE,SPT_FUN_PROTOTYPE
};
enum linkage_type{
    LKA_NONE,LKA_INTERN,LKA_EXTERN
};
enum function_spec_type{
    FSPEC_NONE,FSPEC_INLINE,FSPEC_NORETURN
};
enum static_storage_value_type{
    SSVT_NONE,
    SSVT_POINTER,
};
typedef struct sym_item SYM_ITEM;
typedef struct module IR_MODULE;
typedef struct function IR_FUNC;
typedef struct basic_block IR_BB;
typedef struct instruction IR_INS;
typedef struct operand IR_OPERAND;
typedef struct IR_OPERAND_REG IR_REG;
typedef struct{
    enum static_storage_value_type value_data_type;
    size_t byte_width;  /*1 -byte,2 -word,4 -long(not the long in C),8 -quad*/
    void* data;
}STATIC_STOR_VALUE_ELEM;
typedef struct{
    SYM_ITEM* sym_item;
    void* value_data;   /*use this to alloc a memory*/
    VEC* value_vec;
}STATIC_STOR_VALUE;
typedef struct sym_table SYM;
struct sym_table{
    unsigned int level;
    SYM* father;
    VEC* child_table;
    HASH* sym_hash_table;
    enum scope_type sp_type;
    NMSP name_space;    /*only used for label name space*/
    HASH* typedef_name_table;
    HASH* enum_const_table;
    unsigned int st_attr_type[3];    /*0-switch,1-loop,2-func*/
    IR_BB* switch_begin_bb,*loop_begin_bb;   /*the two others should not be used*/
    IR_BB* switch_end_bb,*loop_end_bb,*func_end_bb;
    void* st_attr;  /*if you need more attributes ,
                        use this to point to a struct, like the following SYMBOL_TABLE_FUNC_ATTR or SYMBOL_TABLE_SWITCH_ATTR*/
};
typedef struct sym_item{
    HASH_COMMON
    DATA_VALUE
    char* value;
    NMSP name_space;
    int align_size;
    enum linkage_type linkage;
    enum function_spec_type fspec_type;
    enum data_storage_type stor_type;
    bool defined;
    bool declared;
    STATIC_STOR_VALUE* init_value;
}SYM_ITEM;
typedef struct{
    bool have_ret;
    SYM_ITEM* function_symbol;
}SYMBOL_TABLE_FUNC_ATTR;
typedef struct{
    VEC* switch_value;
    bool have_default;
}SYMBOL_TABLE_SWITCH_ATTR;
SYM* Create_symbol_table(SYM* father,enum scope_type type);
bool insert_symbol(SYM* symbol_table,SYM_ITEM* symbol_item);
SYM_ITEM* find_symbol(SYM* symbol_table,char* symbol,NMSP name_space);
SYM_ITEM* find_symbol_curr_table(SYM* symbol_table,char* symbol,NMSP name_space);
void del_symbol(SYM* symbol_table,char* symbol,NMSP name_space);
void del_symbol_typedef_table(SYM* symbol_table,char* symbol);
void Del_symbol_table(SYM* symbol_table);
void print_symbol_table(SYM* symbol_table);
void print_symbol(SYM_ITEM* symbol,size_t indentation);
void print_static_stor_value(STATIC_STOR_VALUE* value,size_t data_size,size_t indentation);
VEC* get_symbol_hash(HASH* h);
unsigned long long int SymbolCharToKey(char* symbol,NMSP name_space);
bool symbol_item_cmp(void* a,void* b);
SYM_ITEM* Create_symbol_item(char* symbol,NMSP name_space);
inline bool Copy_sym_item(SYM_ITEM* new,SYM_ITEM* old){
    if(!old)
        return false;
    if(!new)
        new=m_alloc(sizeof(SYM_ITEM));
    new->count=old->count;
    new->data_field=old->data_field;
    new->data_size=old->data_size;
    new->declared=old->declared;
    new->defined=old->defined;
    new->key=old->key;
    new->linkage=old->linkage;
    new->fspec_type=old->fspec_type;
    new->stor_type=old->stor_type;
    new->name_space=old->name_space;
    new->type_vec=old->type_vec;
    new->value=old->value;
    new->align_size=old->align_size;
    new->const_expr=old->const_expr;
    new->init_value=old->init_value;
    return true;
}
#endif