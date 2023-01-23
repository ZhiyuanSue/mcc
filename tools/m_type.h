#ifndef _M_TYPE_H_
#define _M_TYPE_H_
#include "mcc_error.h"
#include "vector.h"
#include "hash.h"
#include "namespace.h"
#define STORAGE_SPEC_NUM 6
#define STORAGE_NUM 8
#define TYPE_SPEC_NUM 15
#define TYPE_NUM 30
#define TYPE_QUAL_NUM 4
#define QUAL_NUM 16
#define FUNCTION_SPEC_NUM 2
#define FUNCTION_NUM 4
#define TYPE_CHAR_NAME_LEN 32
#define TP_COMMON bool complete;\
        bool modifier;
#define TP_BASE_COMMON enum TP_CATEGORY typ_category;\
            unsigned int data_size;
#define DATA_VALUE VEC* type_vec;   \
            size_t data_size;   \
            VALUE_DATA* data_field;   \
            bool const_expr:1;
enum FUNC_CALL_CONVERSION{
    CDECL_DEFAULT,
    STDCALL,
    MICROSOFT_x64,
    SYSTEM_V_AMD64,
};
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

    /*the following type is just used temporarily,so TYPE_NUM is still 28,not more*/
    TP_NULL_POINTER_CONSTANT,
    TP_LABEL
};
#define IS_INT_TYPE(TP_type)   ((TP_type>=TP_SCHAR&&TP_type<=TP_USLONGLONG)||TP_type==TP_BOOL||TP_type==TP_NULL_POINTER_CONSTANT)
#define IS_FLOAT_TYPE(TP_type)  (TP_type>=TP_FLOAT&&TP_type<=TP_LONG_DOUBLE)
#define IS_COMPLEX_TYPE(TP_type)    (TP_type>=TP_FLOAT_COMPLEX&&TP_type<=TP_LONG_DOUBLE_COMPLEX)
#define IS_BASE_TYPE(TP_type)   (TP_type>=TP_SCHAR&&TP_type<=TP_LONG_DOUBLE_COMPLEX)
#define IS_SCALAR_TYPE(TP_type) ((TP_type>=TP_SCHAR&&TP_type<=TP_POINT)||TP_type==TP_NULL_POINTER_CONSTANT)
#define IS_ARTH_TYPE(TP_type)   ((TP_type>=TP_SCHAR&&TP_type<=TP_LONG_DOUBLE_COMPLEX)||TP_type==TP_NULL_POINTER_CONSTANT)
#define IS_REAL_TYPE(TP_type)   ((TP_type>=TP_SCHAR&&TP_type<=TP_BOOL)||TP_type==TP_NULL_POINTER_CONSTANT)
#define IS_CHARACTER_TYPE(TP_type)  (TP_type==TP_SCHAR||TP_type==TP_USCHAR)
typedef struct {
    HASH_COMMON
    TP_COMMON
    union{
        struct{
            enum TP_STORAGE_SPEC typ_stor;
            unsigned char type_qual;
            unsigned char func_spec;
            unsigned long int align_spec;
        };
        struct{
            TP_BASE_COMMON
        };
    };
}M_TYPE;
typedef struct {
    char* member_name;
    NMSP name_space;
    size_t offset;
    VEC* type_vec;  /*if this is a named member, type_vec is the same with the symbol item*/
    bool bit_field;
    size_t bit_field_size;
    size_t bit_field_offset;
}TP_SU_MEMBER;
/*struct/union*/
typedef struct {
    HASH_COMMON
    TP_COMMON
    TP_BASE_COMMON
    char* tag;  /*if tag==NULL it means a anonymous struct or union,and the namespace must be 2*/
    VEC* members;
    TP_SU_MEMBER* curr_designated_member;   /*only union use this field, for the storage is overlaped,so it must point out which member be used*/
    size_t data_align;
    size_t total_data_size;
    NMSP namespece;
    bool can_be_modified_lvalue; /*a lvalue struct cannot have any recursively const member*/
    bool have_flex_array;
}TP_SU;

/*array*/
typedef struct {
    HASH_COMMON
    TP_COMMON
    TP_BASE_COMMON
    bool is_vla;
    bool is_star;
    union{
        size_t axis_size;
        void* assign_expr_node;
    };
    void* axis_modify;
}TP_ARR;
/*enum*/
typedef struct {
    HASH_COMMON
    TP_COMMON
    TP_BASE_COMMON
    char* tag;
    VEC* members;
}TP_ENUMERA;
/*function*/
typedef struct {
    HASH_COMMON
    TP_COMMON
    TP_BASE_COMMON
    char* func_name;
    VEC* func_parameters;
    bool have_ellipsis;
    bool is_old_style;
    enum FUNC_CALL_CONVERSION fcc;
}TP_FUNC;
typedef struct {
    char* para_name;
    VEC* type_vec;
}TP_FUNC_PARA;
/*typedef name*/
typedef struct {
    HASH_COMMON
    TP_COMMON
    TP_BASE_COMMON
    VEC* typedef_name_type;
}TP_DEF_TYPE;
/*const expression return value*/
typedef struct {
    _Bool databool;
    signed char schar;
    unsigned char uschar;
    signed short sshort;
    unsigned short usshort;
    signed int sint;
    unsigned int usint;
    signed long int slong;
    unsigned long int uslong;
    signed long long int sllong;
    unsigned long long int usllong;
    float datafloat;
    double datadouble;
    long double datalongdouble;
    float floatcomplex[2];
    double doublecomplex[2];
    long double longdoublecomplex[2];
    void* pointer;
}VALUE_DATA;
#define TP_INT_CAST_TYPE(tp,data_field) \
    ((tp==TP_SCHAR)?(data_field->schar):    \
    ((tp==TP_USCHAR)?(data_field->uschar):  \
    ((tp==TP_SSHORT)?(data_field->sshort):  \
    ((tp==TP_USHORT)?(data_field->usshort): \
    ((tp==TP_SINT)?(data_field->sint):    \
    ((tp==TP_USINT)?(data_field->usint):    \
    ((tp==TP_SLONG)?(data_field->slong):    \
    ((tp==TP_USLONG)?(data_field->uslong):    \
    ((tp==TP_SLONGLONG)?(data_field->sllong):    \
    ((tp==TP_USLONGLONG)?(data_field->usllong):0   \
    ))))))))))

typedef struct VALUE{
    enum TP_CATEGORY const_expr_type;
    VALUE_DATA* const_value;
}CONST_VALUE;

M_TYPE* build_modify_type(
    enum TP_STORAGE_SPEC storage_spec_type,
    int type_qual_cnt[TYPE_QUAL_NUM],
    int func_spec_cnt[FUNCTION_SPEC_NUM],
    unsigned long int align_spec_cnt
);
M_TYPE* build_base_type(
    enum TP_CATEGORY spec_type
);
unsigned long long int StrToKey(char* str);
unsigned long long int TypeToKey(M_TYPE* t);
#define VALID_ALIGN(type_vec) (type_vec? min( _ALIGN_SIZE_(_MCC_ALIGN_PACK_),Type_align(type_vec) ) :0)
void print_type(M_TYPE* tmpt);

#endif