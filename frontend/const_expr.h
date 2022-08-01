#ifndef _CONST_EXPR_H_
#define _CONST_EXPR_H_
#include "semantics.h"
extern void const_illegal_error(AST_BASE* ast_node);
long long int get_int_const(enum TP_CATEGORY type,VALUE_DATA* data_field,bool is_int);
long double get_float_const(enum TP_CATEGORY type,VALUE_DATA* data_field,bool is_float);
long double* get_complex_const(enum TP_CATEGORY type,VALUE_DATA* data_field,bool is_complex);
bool logical_const(enum TP_CATEGORY type,VALUE_DATA* data_field);
void bitwise_const(enum TP_CATEGORY type,VALUE_DATA* data_field,long long int const_value);
void cast_const(enum TP_CATEGORY dst_type,VALUE_DATA* dst_data_field,enum TP_CATEGORY src_type,VALUE_DATA* src_data_field);
void plus_const(enum TP_CATEGORY type,VALUE_DATA* dst_data_field,VALUE_DATA* src_data_field);
void minus_const(enum TP_CATEGORY type,VALUE_DATA* dst_data_field,VALUE_DATA* src_data_field);
void tilde_const(enum TP_CATEGORY type,VALUE_DATA* dst_data_field,VALUE_DATA* src_data_field);
int exclamation_const(enum TP_CATEGORY type,VALUE_DATA* data_field);
#endif