#ifndef _SYMBOL_TABLE_H_
#define _SYMBOL_TABLE_H_
#include "hash.h"
#include "vector.h"
#include "namespace.h"
#include "m_type.h"
#include "m_type_vec.h"
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
enum sym_table_attr_type{
    SA_ATTR_NONE=0,
    SA_SWITCH=0x01,
    SA_LOOP=0x02,
    SA_FUNC=0x04
};
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
    enum sym_table_attr_type st_attr_type;
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
    bool Thread_local;
    bool defined;
    bool declared;
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
void Del_symbol_table(SYM* symbol_table);
void print_symbol_table(SYM* symbol_table);
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
    new->name_space=old->name_space;
    new->type_vec=old->type_vec;
    new->value=old->value;
    return true;
}
#endif