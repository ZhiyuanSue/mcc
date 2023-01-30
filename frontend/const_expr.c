#include "const_expr.h"
extern size_t type_data_size[TYPE_NUM];
bool const_value(AST_BASE* ast_node)
{
    if(!ast_node||ast_node->type!=constant_expr)
        return false;
    ast_node->symbol=Create_symbol_item(tmp_symbol_str_alloc("reg.const."),NMSP_DEFAULT);
    if(!insert_symbol(ast_node->symbol_table,ast_node->symbol))
        return false;
    ast_node->symbol->data_field=malloc(sizeof(VALUE_DATA));
    AST_BASE* conditional_expr_node=AST_GET_CHILD(ast_node,0);
    if(expr_dispatch(conditional_expr_node)&&conditional_expr_node->symbol->const_expr)
    {
        VECcpy(conditional_expr_node->symbol->type_vec,&(ast_node->symbol->type_vec));
        ast_node->symbol->data_size=conditional_expr_node->symbol->data_size;
        memcpy(ast_node->symbol->data_field,conditional_expr_node->symbol->data_field,sizeof(VALUE_DATA));  
        ast_node->symbol->const_expr=conditional_expr_node->symbol->const_expr;
        ast_node->symbol->is_lvalue=conditional_expr_node->symbol->is_lvalue;
        return true;
    }
    else
    {
        const_illegal_error(ast_node);
        return false;
    }
}
long long int get_int_const(enum TP_CATEGORY type,VALUE_DATA* data_field,bool is_int)
{
    if(!is_int)
        return 0;
    switch (type)
    {
    case TP_SCHAR:
        return (long long int)(data_field->schar);
    case TP_USCHAR:
        return (long long int)(data_field->uschar);
    case TP_SSHORT:
        return (long long int)(data_field->sshort);
    case TP_USHORT:
        return (long long int)(data_field->usshort);
    case TP_SINT:
        return (long long int)(data_field->sint);
    case TP_USINT:
        return (long long int)(data_field->usint);
    case TP_SLONG:
        return (long long int)(data_field->slong);
    case TP_USLONG:
        return (long long int)(data_field->uslong);
    case TP_SLONGLONG:
        return (data_field->sllong);
    case TP_USLONGLONG:
        return (long long int)(data_field->usllong);    
        /*for unsigned long long cast to signed long long, it will not miss data but just means different meanings*/
    case TP_BOOL:
        return (long long int)(data_field->databool);
    case TP_NULL_POINTER_CONSTANT:
        return 0;
    default:
        return 0;
    }
}
long double get_float_const(enum TP_CATEGORY type,VALUE_DATA* data_field,bool is_float)
{
    if(!is_float)
        return 0;
    switch (type)
    {
    case TP_FLOAT:
        return (long double)(data_field->datafloat);
    case TP_DOUBLE:
        return (long double)(data_field->datadouble);
    case TP_LONG_DOUBLE:
        return (data_field->datalongdouble);
    case TP_NULL_POINTER_CONSTANT:
        return (long double)0;
    default:
        return (long double)0;
    }
}
long double* get_complex_const(enum TP_CATEGORY type,VALUE_DATA* data_field,bool is_complex)
{
    if(!is_complex)
        return 0;
    long double* res=(long double*)m_alloc(type_data_size[TP_LONG_DOUBLE_COMPLEX]);
    switch (type)
    {
    case TP_FLOAT_COMPLEX:
    {
        *res=(long double)(data_field->floatcomplex[0]);
        *(res+1)=(long double)(data_field->floatcomplex[1]);
        break;
    }
    case TP_DOUBLE_COMPLEX:
    {
        *res=(long double)(data_field->doublecomplex[0]);
        *(res+1)=(long double)(data_field->doublecomplex[1]);
        break;
    }
    case TP_LONG_DOUBLE_COMPLEX:
    {
        *res=(data_field->longdoublecomplex[0]);
        *(res+1)=(data_field->longdoublecomplex[1]);
        break;
    }
    default:
        return 0;
    }
    return res;
}
bool logical_const(enum TP_CATEGORY type,VALUE_DATA* data_field)
{
    switch (type)
    {
    case TP_SCHAR:
        return (data_field->schar)==0;
    case TP_USCHAR:
        return (data_field->uschar)==0;
    case TP_SSHORT:
        return (data_field->sshort)==0;
    case TP_USHORT:
        return (data_field->usshort)==0;
    case TP_SINT:
        return (data_field->sint)==0;
    case TP_USINT:
        return (data_field->usint)==0;
    case TP_SLONG:
        return (data_field->slong)==0;
    case TP_USLONG:
        return (data_field->uslong)==0;
    case TP_SLONGLONG:
        return (data_field->sllong)==0;
    case TP_USLONGLONG:
        return (data_field->usllong)==0;
    case TP_FLOAT:
        return (data_field->datafloat)==0;
    case TP_DOUBLE:
        return (data_field->datadouble)==0;
    case TP_LONG_DOUBLE:
        return (data_field->datalongdouble)==0;
    case TP_BOOL:
        return (data_field->databool)==0;
    case TP_FLOAT_COMPLEX:
        return (data_field->floatcomplex[0])==0;
    case TP_DOUBLE_COMPLEX:
        return (data_field->doublecomplex[0])==0;
    case TP_LONG_DOUBLE_COMPLEX:
        return (data_field->longdoublecomplex[0])==0;
    case TP_POINT:
        return (data_field->pointer)==0;
    case TP_NULL_POINTER_CONSTANT:
        return false;
    default:
        return 0;
    }
}
void bitwise_const(enum TP_CATEGORY type,VALUE_DATA* data_field,long long int const_value)
{
    switch (type)
    {
    case TP_SCHAR:
        data_field->schar=(signed char)const_value;
        break;
    case TP_USCHAR:
        data_field->uschar=(unsigned char)const_value;
        break;
    case TP_SSHORT:
        data_field->sshort=(signed short)const_value;
        break;
    case TP_USHORT:
        data_field->usshort=(unsigned short)const_value;
        break;
    case TP_SINT:
        data_field->sint=(signed int)const_value;
        break;
    case TP_USINT:
        data_field->usint=(unsigned int)const_value;
        break;
    case TP_SLONG:
        data_field->slong=(signed long)const_value;
        break;
    case TP_USLONG:
        data_field->uslong=(unsigned long)const_value;
        break;
    case TP_SLONGLONG:
        data_field->sllong=(signed long long)const_value;
        break;
    case TP_USLONGLONG:
        data_field->usllong=(unsigned long long)const_value;
        break;
    case TP_BOOL:
        data_field->databool=(_Bool)const_value;
        break;
    default:
        break;
    }
}
/*it is not a graceful way to solve such a problem,but I cannot find a better one*/
void cast_const(enum TP_CATEGORY dst_type,VALUE_DATA* dst_data_field,enum TP_CATEGORY src_type,VALUE_DATA* src_data_field)
{
    switch (dst_type)
    {
    case TP_SCHAR:
    {
        switch(src_type)
        {
            case TP_SCHAR:
                dst_data_field->schar=(signed char)(src_data_field->schar);
                return;
            case TP_USCHAR:
                dst_data_field->schar=(signed char)(src_data_field->uschar);
                return;
            case TP_SSHORT:
                dst_data_field->schar=(signed char)(src_data_field->sshort);
                return;
            case TP_USHORT:
                dst_data_field->schar=(signed char)(src_data_field->usshort);
                return;
            case TP_SINT:
                dst_data_field->schar=(signed char)(src_data_field->sint);
                return;
            case TP_USINT:
                dst_data_field->schar=(signed char)(src_data_field->usint);
                return;
            case TP_SLONG:
                dst_data_field->schar=(signed char)(src_data_field->slong);
                return;
            case TP_USLONG:
                dst_data_field->schar=(signed char)(src_data_field->uslong);
                return;
            case TP_SLONGLONG:
                dst_data_field->schar=(signed char)(src_data_field->sllong);
                return;
            case TP_USLONGLONG:
                dst_data_field->schar=(signed char)(src_data_field->usllong);
                return;
            case TP_FLOAT:
                dst_data_field->schar=(signed char)(src_data_field->datafloat);
                return;
            case TP_DOUBLE:
                dst_data_field->schar=(signed char)(src_data_field->datadouble);
                return;
            case TP_LONG_DOUBLE:
                dst_data_field->schar=(signed char)(src_data_field->datalongdouble);
                return;
            case TP_BOOL:
                dst_data_field->schar=(signed char)(src_data_field->databool);
                return;
            case TP_FLOAT_COMPLEX:
                dst_data_field->schar=(signed char)(src_data_field->floatcomplex[0]);
                return;
            case TP_DOUBLE_COMPLEX:
                dst_data_field->schar=(signed char)(src_data_field->doublecomplex[0]);
                return;
            case TP_LONG_DOUBLE_COMPLEX:
                dst_data_field->schar=(signed char)(src_data_field->longdoublecomplex[0]);
                return;
            case TP_NULL_POINTER_CONSTANT:
                dst_data_field->schar=(signed char)(src_data_field->sint);
                return;
            default:
                return;
        }
    }
    case TP_USCHAR:
    {
        switch(src_type)
        {
            case TP_SCHAR:
                dst_data_field->uschar=(unsigned char)(src_data_field->schar);
                return;
            case TP_USCHAR:
                dst_data_field->uschar=(unsigned char)(src_data_field->uschar);
                return;
            case TP_SSHORT:
                dst_data_field->uschar=(unsigned char)(src_data_field->sshort);
                return;
            case TP_USHORT:
                dst_data_field->uschar=(unsigned char)(src_data_field->usshort);
                return;
            case TP_SINT:
                dst_data_field->uschar=(unsigned char)(src_data_field->sint);
                return;
            case TP_USINT:
                dst_data_field->uschar=(unsigned char)(src_data_field->usint);
                return;
            case TP_SLONG:
                dst_data_field->uschar=(unsigned char)(src_data_field->slong);
                return;
            case TP_USLONG:
                dst_data_field->uschar=(unsigned char)(src_data_field->uslong);
                return;
            case TP_SLONGLONG:
                dst_data_field->uschar=(unsigned char)(src_data_field->sllong);
                return;
            case TP_USLONGLONG:
                dst_data_field->uschar=(unsigned char)(src_data_field->usllong);
                return;
            case TP_FLOAT:
                dst_data_field->uschar=(unsigned char)(src_data_field->datafloat);
                return;
            case TP_DOUBLE:
                dst_data_field->uschar=(unsigned char)(src_data_field->datadouble);
                return;
            case TP_LONG_DOUBLE:
                dst_data_field->uschar=(unsigned char)(src_data_field->datalongdouble);
                return;
            case TP_BOOL:
                dst_data_field->uschar=(unsigned char)(src_data_field->databool);
                return;
            case TP_FLOAT_COMPLEX:
                dst_data_field->uschar=(unsigned char)(src_data_field->floatcomplex[0]);
                return;
            case TP_DOUBLE_COMPLEX:
                dst_data_field->uschar=(unsigned char)(src_data_field->doublecomplex[0]);
                return;
            case TP_LONG_DOUBLE_COMPLEX:
                dst_data_field->uschar=(unsigned char)(src_data_field->longdoublecomplex[0]);
                return;
            case TP_NULL_POINTER_CONSTANT:
                dst_data_field->uschar=(unsigned char)(src_data_field->sint);
                return;
            default:
                return;
        }
    }
    case TP_SSHORT:
    {
        switch(src_type)
        {
            case TP_SCHAR:
                dst_data_field->sshort=(signed short)(src_data_field->schar);
                return;
            case TP_USCHAR:
                dst_data_field->sshort=(signed short)(src_data_field->uschar);
                return;
            case TP_SSHORT:
                dst_data_field->sshort=(signed short)(src_data_field->sshort);
                return;
            case TP_USHORT:
                dst_data_field->sshort=(signed short)(src_data_field->usshort);
                return;
            case TP_SINT:
                dst_data_field->sshort=(signed short)(src_data_field->sint);
                return;
            case TP_USINT:
                dst_data_field->sshort=(signed short)(src_data_field->usint);
                return;
            case TP_SLONG:
                dst_data_field->sshort=(signed short)(src_data_field->slong);
                return;
            case TP_USLONG:
                dst_data_field->sshort=(signed short)(src_data_field->uslong);
                return;
            case TP_SLONGLONG:
                dst_data_field->sshort=(signed short)(src_data_field->sllong);
                return;
            case TP_USLONGLONG:
                dst_data_field->sshort=(signed short)(src_data_field->usllong);
                return;
            case TP_FLOAT:
                dst_data_field->sshort=(signed short)(src_data_field->datafloat);
                return;
            case TP_DOUBLE:
                dst_data_field->sshort=(signed short)(src_data_field->datadouble);
                return;
            case TP_LONG_DOUBLE:
                dst_data_field->sshort=(signed short)(src_data_field->datalongdouble);
                return;
            case TP_BOOL:
                dst_data_field->sshort=(signed short)(src_data_field->databool);
                return;
            case TP_FLOAT_COMPLEX:
                dst_data_field->sshort=(signed short)(src_data_field->floatcomplex[0]);
                return;
            case TP_DOUBLE_COMPLEX:
                dst_data_field->sshort=(signed short)(src_data_field->doublecomplex[0]);
                return;
            case TP_LONG_DOUBLE_COMPLEX:
                dst_data_field->sshort=(signed short)(src_data_field->longdoublecomplex[0]);
                return;
            case TP_NULL_POINTER_CONSTANT:
                dst_data_field->sshort=(signed short)(src_data_field->sint);
                return;
            default:
                return;
        }
    }
    case TP_USHORT:
    {
        switch(src_type)
        {
            case TP_SCHAR:
                dst_data_field->usshort=(unsigned short)(src_data_field->schar);
                return;
            case TP_USCHAR:
                dst_data_field->usshort=(unsigned short)(src_data_field->uschar);
                return;
            case TP_SSHORT:
                dst_data_field->usshort=(unsigned short)(src_data_field->sshort);
                return;
            case TP_USHORT:
                dst_data_field->usshort=(unsigned short)(src_data_field->usshort);
                return;
            case TP_SINT:
                dst_data_field->usshort=(unsigned short)(src_data_field->sint);
                return;
            case TP_USINT:
                dst_data_field->usshort=(unsigned short)(src_data_field->usint);
                return;
            case TP_SLONG:
                dst_data_field->usshort=(unsigned short)(src_data_field->slong);
                return;
            case TP_USLONG:
                dst_data_field->usshort=(unsigned short)(src_data_field->uslong);
                return;
            case TP_SLONGLONG:
                dst_data_field->usshort=(unsigned short)(src_data_field->sllong);
                return;
            case TP_USLONGLONG:
                dst_data_field->usshort=(unsigned short)(src_data_field->usllong);
                return;
            case TP_FLOAT:
                dst_data_field->usshort=(unsigned short)(src_data_field->datafloat);
                return;
            case TP_DOUBLE:
                dst_data_field->usshort=(unsigned short)(src_data_field->datadouble);
                return;
            case TP_LONG_DOUBLE:
                dst_data_field->usshort=(unsigned short)(src_data_field->datalongdouble);
                return;
            case TP_BOOL:
                dst_data_field->usshort=(unsigned short)(src_data_field->databool);
                return;
            case TP_FLOAT_COMPLEX:
                dst_data_field->usshort=(unsigned short)(src_data_field->floatcomplex[0]);
                return;
            case TP_DOUBLE_COMPLEX:
                dst_data_field->usshort=(unsigned short)(src_data_field->doublecomplex[0]);
                return;
            case TP_LONG_DOUBLE_COMPLEX:
                dst_data_field->usshort=(unsigned short)(src_data_field->longdoublecomplex[0]);
                return;
            case TP_NULL_POINTER_CONSTANT:
                dst_data_field->usshort=(unsigned short)(src_data_field->sint);
                return;
            default:
                return;
        }
    }
    case TP_SINT:
    {
        switch(src_type)
        {
            case TP_SCHAR:
                dst_data_field->sint=(signed int)(src_data_field->schar);
                return;
            case TP_USCHAR:
                dst_data_field->sint=(signed int)(src_data_field->uschar);
                return;
            case TP_SSHORT:
                dst_data_field->sint=(signed int)(src_data_field->sshort);
                return;
            case TP_USHORT:
                dst_data_field->sint=(signed int)(src_data_field->usshort);
                return;
            case TP_SINT:
                dst_data_field->sint=(signed int)(src_data_field->sint);
                return;
            case TP_USINT:
                dst_data_field->sint=(signed int)(src_data_field->usint);
                return;
            case TP_SLONG:
                dst_data_field->sint=(signed int)(src_data_field->slong);
                return;
            case TP_USLONG:
                dst_data_field->sint=(signed int)(src_data_field->uslong);
                return;
            case TP_SLONGLONG:
                dst_data_field->sint=(signed int)(src_data_field->sllong);
                return;
            case TP_USLONGLONG:
                dst_data_field->sint=(signed int)(src_data_field->usllong);
                return;
            case TP_FLOAT:
                dst_data_field->sint=(signed int)(src_data_field->datafloat);
                return;
            case TP_DOUBLE:
                dst_data_field->sint=(signed int)(src_data_field->datadouble);
                return;
            case TP_LONG_DOUBLE:
                dst_data_field->sint=(signed int)(src_data_field->datalongdouble);
                return;
            case TP_BOOL:
                dst_data_field->sint=(signed int)(src_data_field->databool);
                return;
            case TP_FLOAT_COMPLEX:
                dst_data_field->sint=(signed int)(src_data_field->floatcomplex[0]);
                return;
            case TP_DOUBLE_COMPLEX:
                dst_data_field->sint=(signed int)(src_data_field->doublecomplex[0]);
                return;
            case TP_LONG_DOUBLE_COMPLEX:
                dst_data_field->sint=(signed int)(src_data_field->longdoublecomplex[0]);
                return;
            case TP_NULL_POINTER_CONSTANT:
                dst_data_field->sint=(signed int)(src_data_field->sint);
                return;
            default:
                return;
        }
    }
    case TP_USINT:
    {
        switch(src_type)
        {
            case TP_SCHAR:
                dst_data_field->usint=(unsigned int)(src_data_field->schar);
                return;
            case TP_USCHAR:
                dst_data_field->usint=(unsigned int)(src_data_field->uschar);
                return;
            case TP_SSHORT:
                dst_data_field->usint=(unsigned int)(src_data_field->sshort);
                return;
            case TP_USHORT:
                dst_data_field->usint=(unsigned int)(src_data_field->usshort);
                return;
            case TP_SINT:
                dst_data_field->usint=(unsigned int)(src_data_field->sint);
                return;
            case TP_USINT:
                dst_data_field->usint=(unsigned int)(src_data_field->usint);
                return;
            case TP_SLONG:
                dst_data_field->usint=(unsigned int)(src_data_field->slong);
                return;
            case TP_USLONG:
                dst_data_field->usint=(unsigned int)(src_data_field->uslong);
                return;
            case TP_SLONGLONG:
                dst_data_field->usint=(unsigned int)(src_data_field->sllong);
                return;
            case TP_USLONGLONG:
                dst_data_field->usint=(unsigned int)(src_data_field->usllong);
                return;
            case TP_FLOAT:
                dst_data_field->usint=(unsigned int)(src_data_field->datafloat);
                return;
            case TP_DOUBLE:
                dst_data_field->usint=(unsigned int)(src_data_field->datadouble);
                return;
            case TP_LONG_DOUBLE:
                dst_data_field->usint=(unsigned int)(src_data_field->datalongdouble);
                return;
            case TP_BOOL:
                dst_data_field->usint=(unsigned int)(src_data_field->databool);
                return;
            case TP_FLOAT_COMPLEX:
                dst_data_field->usint=(unsigned int)(src_data_field->floatcomplex[0]);
                return;
            case TP_DOUBLE_COMPLEX:
                dst_data_field->usint=(unsigned int)(src_data_field->doublecomplex[0]);
                return;
            case TP_LONG_DOUBLE_COMPLEX:
                dst_data_field->usint=(unsigned int)(src_data_field->longdoublecomplex[0]);
                return;
            case TP_NULL_POINTER_CONSTANT:
                dst_data_field->usint=(unsigned int)(src_data_field->sint);
                return;
            default:
                return;
        }
    }
    case TP_SLONG:
    {
        switch(src_type)
        {
            case TP_SCHAR:
                dst_data_field->slong=(signed long)(src_data_field->schar);
                return;
            case TP_USCHAR:
                dst_data_field->slong=(signed long)(src_data_field->uschar);
                return;
            case TP_SSHORT:
                dst_data_field->slong=(signed long)(src_data_field->sshort);
                return;
            case TP_USHORT:
                dst_data_field->slong=(signed long)(src_data_field->usshort);
                return;
            case TP_SINT:
                dst_data_field->slong=(signed long)(src_data_field->sint);
                return;
            case TP_USINT:
                dst_data_field->slong=(signed long)(src_data_field->usint);
                return;
            case TP_SLONG:
                dst_data_field->slong=(signed long)(src_data_field->slong);
                return;
            case TP_USLONG:
                dst_data_field->slong=(signed long)(src_data_field->uslong);
                return;
            case TP_SLONGLONG:
                dst_data_field->slong=(signed long)(src_data_field->sllong);
                return;
            case TP_USLONGLONG:
                dst_data_field->slong=(signed long)(src_data_field->usllong);
                return;
            case TP_FLOAT:
                dst_data_field->slong=(signed long)(src_data_field->datafloat);
                return;
            case TP_DOUBLE:
                dst_data_field->slong=(signed long)(src_data_field->datadouble);
                return;
            case TP_LONG_DOUBLE:
                dst_data_field->slong=(signed long)(src_data_field->datalongdouble);
                return;
            case TP_BOOL:
                dst_data_field->slong=(signed long)(src_data_field->databool);
                return;
            case TP_FLOAT_COMPLEX:
                dst_data_field->slong=(signed long)(src_data_field->floatcomplex[0]);
                return;
            case TP_DOUBLE_COMPLEX:
                dst_data_field->slong=(signed long)(src_data_field->doublecomplex[0]);
                return;
            case TP_LONG_DOUBLE_COMPLEX:
                dst_data_field->slong=(signed long)(src_data_field->longdoublecomplex[0]);
                return;
            case TP_NULL_POINTER_CONSTANT:
                dst_data_field->slong=(signed long)(src_data_field->sint);
                return;
            default:
                return;
        }
    }
    case TP_USLONG:
    {
        switch(src_type)
        {
            case TP_SCHAR:
                dst_data_field->uslong=(unsigned long)(src_data_field->schar);
                return;
            case TP_USCHAR:
                dst_data_field->uslong=(unsigned long)(src_data_field->uschar);
                return;
            case TP_SSHORT:
                dst_data_field->uslong=(unsigned long)(src_data_field->sshort);
                return;
            case TP_USHORT:
                dst_data_field->uslong=(unsigned long)(src_data_field->usshort);
                return;
            case TP_SINT:
                dst_data_field->uslong=(unsigned long)(src_data_field->sint);
                return;
            case TP_USINT:
                dst_data_field->uslong=(unsigned long)(src_data_field->usint);
                return;
            case TP_SLONG:
                dst_data_field->uslong=(unsigned long)(src_data_field->slong);
                return;
            case TP_USLONG:
                dst_data_field->uslong=(unsigned long)(src_data_field->uslong);
                return;
            case TP_SLONGLONG:
                dst_data_field->uslong=(unsigned long)(src_data_field->sllong);
                return;
            case TP_USLONGLONG:
                dst_data_field->uslong=(unsigned long)(src_data_field->usllong);
                return;
            case TP_FLOAT:
                dst_data_field->uslong=(unsigned long)(src_data_field->datafloat);
                return;
            case TP_DOUBLE:
                dst_data_field->uslong=(unsigned long)(src_data_field->datadouble);
                return;
            case TP_LONG_DOUBLE:
                dst_data_field->uslong=(unsigned long)(src_data_field->datalongdouble);
                return;
            case TP_BOOL:
                dst_data_field->uslong=(unsigned long)(src_data_field->databool);
                return;
            case TP_FLOAT_COMPLEX:
                dst_data_field->uslong=(unsigned long)(src_data_field->floatcomplex[0]);
                return;
            case TP_DOUBLE_COMPLEX:
                dst_data_field->uslong=(unsigned long)(src_data_field->doublecomplex[0]);
                return;
            case TP_LONG_DOUBLE_COMPLEX:
                dst_data_field->uslong=(unsigned long)(src_data_field->longdoublecomplex[0]);
                return;
            case TP_NULL_POINTER_CONSTANT:
                dst_data_field->uslong=(unsigned long)(src_data_field->sint);
                return;
            default:
                return;
        }
    }
    case TP_SLONGLONG:
    {
        switch(src_type)
        {
            case TP_SCHAR:
                dst_data_field->sllong=(signed long long)(src_data_field->schar);
                return;
            case TP_USCHAR:
                dst_data_field->sllong=(signed long long)(src_data_field->uschar);
                return;
            case TP_SSHORT:
                dst_data_field->sllong=(signed long long)(src_data_field->sshort);
                return;
            case TP_USHORT:
                dst_data_field->sllong=(signed long long)(src_data_field->usshort);
                return;
            case TP_SINT:
                dst_data_field->sllong=(signed long long)(src_data_field->sint);
                return;
            case TP_USINT:
                dst_data_field->sllong=(signed long long)(src_data_field->usint);
                return;
            case TP_SLONG:
                dst_data_field->sllong=(signed long long)(src_data_field->slong);
                return;
            case TP_USLONG:
                dst_data_field->sllong=(signed long long)(src_data_field->uslong);
                return;
            case TP_SLONGLONG:
                dst_data_field->sllong=(signed long long)(src_data_field->sllong);
                return;
            case TP_USLONGLONG:
                dst_data_field->sllong=(signed long long)(src_data_field->usllong);
                return;
            case TP_FLOAT:
                dst_data_field->sllong=(signed long long)(src_data_field->datafloat);
                return;
            case TP_DOUBLE:
                dst_data_field->sllong=(signed long long)(src_data_field->datadouble);
                return;
            case TP_LONG_DOUBLE:
                dst_data_field->sllong=(signed long long)(src_data_field->datalongdouble);
                return;
            case TP_BOOL:
                dst_data_field->sllong=(signed long long)(src_data_field->databool);
                return;
            case TP_FLOAT_COMPLEX:
                dst_data_field->sllong=(signed long long)(src_data_field->floatcomplex[0]);
                return;
            case TP_DOUBLE_COMPLEX:
                dst_data_field->sllong=(signed long long)(src_data_field->doublecomplex[0]);
                return;
            case TP_LONG_DOUBLE_COMPLEX:
                dst_data_field->sllong=(signed long long)(src_data_field->longdoublecomplex[0]);
                return;
            case TP_NULL_POINTER_CONSTANT:
                dst_data_field->sllong=(signed long long)(src_data_field->sint);
                return;
            default:
                return;
        }
    }
    case TP_USLONGLONG:
    {
        switch(src_type)
        {
            case TP_SCHAR:
                dst_data_field->usllong=(unsigned long long)(src_data_field->schar);
                return;
            case TP_USCHAR:
                dst_data_field->usllong=(unsigned long long)(src_data_field->uschar);
                return;
            case TP_SSHORT:
                dst_data_field->usllong=(unsigned long long)(src_data_field->sshort);
                return;
            case TP_USHORT:
                dst_data_field->usllong=(unsigned long long)(src_data_field->usshort);
                return;
            case TP_SINT:
                dst_data_field->usllong=(unsigned long long)(src_data_field->sint);
                return;
            case TP_USINT:
                dst_data_field->usllong=(unsigned long long)(src_data_field->usint);
                return;
            case TP_SLONG:
                dst_data_field->usllong=(unsigned long long)(src_data_field->slong);
                return;
            case TP_USLONG:
                dst_data_field->usllong=(unsigned long long)(src_data_field->uslong);
                return;
            case TP_SLONGLONG:
                dst_data_field->usllong=(unsigned long long)(src_data_field->sllong);
                return;
            case TP_USLONGLONG:
                dst_data_field->usllong=(unsigned long long)(src_data_field->usllong);
                return;
            case TP_FLOAT:
                dst_data_field->usllong=(unsigned long long)(src_data_field->datafloat);
                return;
            case TP_DOUBLE:
                dst_data_field->usllong=(unsigned long long)(src_data_field->datadouble);
                return;
            case TP_LONG_DOUBLE:
                dst_data_field->usllong=(unsigned long long)(src_data_field->datalongdouble);
                return;
            case TP_BOOL:
                dst_data_field->usllong=(unsigned long long)(src_data_field->databool);
                return;
            case TP_FLOAT_COMPLEX:
                dst_data_field->usllong=(unsigned long long)(src_data_field->floatcomplex[0]);
                return;
            case TP_DOUBLE_COMPLEX:
                dst_data_field->usllong=(unsigned long long)(src_data_field->doublecomplex[0]);
                return;
            case TP_LONG_DOUBLE_COMPLEX:
                dst_data_field->usllong=(unsigned long long)(src_data_field->longdoublecomplex[0]);
                return;
            case TP_NULL_POINTER_CONSTANT:
                dst_data_field->usllong=(unsigned long long)(src_data_field->sint);
                return;
            default:
                return;
        }
    }
    case TP_FLOAT:
    {
        switch(src_type)
        {
            case TP_SCHAR:
                dst_data_field->datafloat=(float)(src_data_field->schar);
                return;
            case TP_USCHAR:
                dst_data_field->datafloat=(float)(src_data_field->uschar);
                return;
            case TP_SSHORT:
                dst_data_field->datafloat=(float)(src_data_field->sshort);
                return;
            case TP_USHORT:
                dst_data_field->datafloat=(float)(src_data_field->usshort);
                return;
            case TP_SINT:
                dst_data_field->datafloat=(float)(src_data_field->sint);
                return;
            case TP_USINT:
                dst_data_field->datafloat=(float)(src_data_field->usint);
                return;
            case TP_SLONG:
                dst_data_field->datafloat=(float)(src_data_field->slong);
                return;
            case TP_USLONG:
                dst_data_field->datafloat=(float)(src_data_field->uslong);
                return;
            case TP_SLONGLONG:
                dst_data_field->datafloat=(float)(src_data_field->sllong);
                return;
            case TP_USLONGLONG:
                dst_data_field->datafloat=(float)(src_data_field->usllong);
                return;
            case TP_FLOAT:
                dst_data_field->datafloat=(float)(src_data_field->datafloat);
                return;
            case TP_DOUBLE:
                dst_data_field->datafloat=(float)(src_data_field->datadouble);
                return;
            case TP_LONG_DOUBLE:
                dst_data_field->datafloat=(float)(src_data_field->datalongdouble);
                return;
            case TP_BOOL:
                dst_data_field->datafloat=(float)(src_data_field->databool);
                return;
            case TP_FLOAT_COMPLEX:
                dst_data_field->datafloat=(float)(src_data_field->floatcomplex[0]);
                return;
            case TP_DOUBLE_COMPLEX:
                dst_data_field->datafloat=(float)(src_data_field->doublecomplex[0]);
                return;
            case TP_LONG_DOUBLE_COMPLEX:
                dst_data_field->datafloat=(float)(src_data_field->longdoublecomplex[0]);
                return;
            case TP_NULL_POINTER_CONSTANT:
                dst_data_field->datafloat=(float)(src_data_field->sint);
                return;
            default:
                return;
        }
    }
    case TP_DOUBLE:
    {
        switch(src_type)
        {
            case TP_SCHAR:
                dst_data_field->datadouble=(double)(src_data_field->schar);
                return;
            case TP_USCHAR:
                dst_data_field->datadouble=(double)(src_data_field->uschar);
                return;
            case TP_SSHORT:
                dst_data_field->datadouble=(double)(src_data_field->sshort);
                return;
            case TP_USHORT:
                dst_data_field->datadouble=(double)(src_data_field->usshort);
                return;
            case TP_SINT:
                dst_data_field->datadouble=(double)(src_data_field->sint);
                return;
            case TP_USINT:
                dst_data_field->datadouble=(double)(src_data_field->usint);
                return;
            case TP_SLONG:
                dst_data_field->datadouble=(double)(src_data_field->slong);
                return;
            case TP_USLONG:
                dst_data_field->datadouble=(double)(src_data_field->uslong);
                return;
            case TP_SLONGLONG:
                dst_data_field->datadouble=(double)(src_data_field->sllong);
                return;
            case TP_USLONGLONG:
                dst_data_field->datadouble=(double)(src_data_field->usllong);
                return;
            case TP_FLOAT:
                dst_data_field->datadouble=(double)(src_data_field->datafloat);
                return;
            case TP_DOUBLE:
                dst_data_field->datadouble=(double)(src_data_field->datadouble);
                return;
            case TP_LONG_DOUBLE:
                dst_data_field->datadouble=(double)(src_data_field->datalongdouble);
                return;
            case TP_BOOL:
                dst_data_field->datadouble=(double)(src_data_field->databool);
                return;
            case TP_FLOAT_COMPLEX:
                dst_data_field->datadouble=(double)(src_data_field->floatcomplex[0]);
                return;
            case TP_DOUBLE_COMPLEX:
                dst_data_field->datadouble=(double)(src_data_field->doublecomplex[0]);
                return;
            case TP_LONG_DOUBLE_COMPLEX:
                dst_data_field->datadouble=(double)(src_data_field->longdoublecomplex[0]);
                return;
            case TP_NULL_POINTER_CONSTANT:
                dst_data_field->datadouble=(double)(src_data_field->sint);
                return;
            default:
                return;
        }
    }
    case TP_LONG_DOUBLE:
    {
        switch(src_type)
        {
            case TP_SCHAR:
                dst_data_field->datalongdouble=(long double)(src_data_field->schar);
                return;
            case TP_USCHAR:
                dst_data_field->datalongdouble=(long double)(src_data_field->uschar);
                return;
            case TP_SSHORT:
                dst_data_field->datalongdouble=(long double)(src_data_field->sshort);
                return;
            case TP_USHORT:
                dst_data_field->datalongdouble=(long double)(src_data_field->usshort);
                return;
            case TP_SINT:
                dst_data_field->datalongdouble=(long double)(src_data_field->sint);
                return;
            case TP_USINT:
                dst_data_field->datalongdouble=(long double)(src_data_field->usint);
                return;
            case TP_SLONG:
                dst_data_field->datalongdouble=(long double)(src_data_field->slong);
                return;
            case TP_USLONG:
                dst_data_field->datalongdouble=(long double)(src_data_field->uslong);
                return;
            case TP_SLONGLONG:
                dst_data_field->datalongdouble=(long double)(src_data_field->sllong);
                return;
            case TP_USLONGLONG:
                dst_data_field->datalongdouble=(long double)(src_data_field->usllong);
                return;
            case TP_FLOAT:
                dst_data_field->datalongdouble=(long double)(src_data_field->datafloat);
                return;
            case TP_DOUBLE:
                dst_data_field->datalongdouble=(long double)(src_data_field->datadouble);
                return;
            case TP_LONG_DOUBLE:
                dst_data_field->datalongdouble=(long double)(src_data_field->datalongdouble);
                return;
            case TP_BOOL:
                dst_data_field->datalongdouble=(long double)(src_data_field->databool);
                return;
            case TP_FLOAT_COMPLEX:
                dst_data_field->datalongdouble=(long double)(src_data_field->floatcomplex[0]);
                return;
            case TP_DOUBLE_COMPLEX:
                dst_data_field->datalongdouble=(long double)(src_data_field->doublecomplex[0]);
                return;
            case TP_LONG_DOUBLE_COMPLEX:
                dst_data_field->datalongdouble=(long double)(src_data_field->longdoublecomplex[0]);
                return;
            case TP_NULL_POINTER_CONSTANT:
                dst_data_field->datalongdouble=(long double)(src_data_field->sint);
                return;
            default:
                return;
        }
    }
    case TP_BOOL:
    {
        switch(src_type)
        {
            case TP_SCHAR:
                dst_data_field->databool=(_Bool)(src_data_field->schar);
                return;
            case TP_USCHAR:
                dst_data_field->databool=(_Bool)(src_data_field->uschar);
                return;
            case TP_SSHORT:
                dst_data_field->databool=(_Bool)(src_data_field->sshort);
                return;
            case TP_USHORT:
                dst_data_field->databool=(_Bool)(src_data_field->usshort);
                return;
            case TP_SINT:
                dst_data_field->databool=(_Bool)(src_data_field->sint);
                return;
            case TP_USINT:
                dst_data_field->databool=(_Bool)(src_data_field->usint);
                return;
            case TP_SLONG:
                dst_data_field->databool=(_Bool)(src_data_field->slong);
                return;
            case TP_USLONG:
                dst_data_field->databool=(_Bool)(src_data_field->uslong);
                return;
            case TP_SLONGLONG:
                dst_data_field->databool=(_Bool)(src_data_field->sllong);
                return;
            case TP_USLONGLONG:
                dst_data_field->databool=(_Bool)(src_data_field->usllong);
                return;
            case TP_FLOAT:
                dst_data_field->databool=(_Bool)(src_data_field->datafloat);
                return;
            case TP_DOUBLE:
                dst_data_field->databool=(_Bool)(src_data_field->datadouble);
                return;
            case TP_LONG_DOUBLE:
                dst_data_field->databool=(_Bool)(src_data_field->datalongdouble);
                return;
            case TP_BOOL:
                dst_data_field->databool=(_Bool)(src_data_field->databool);
                return;
            case TP_FLOAT_COMPLEX:
                dst_data_field->databool=(_Bool)(src_data_field->floatcomplex[0]);
                return;
            case TP_DOUBLE_COMPLEX:
                dst_data_field->databool=(_Bool)(src_data_field->doublecomplex[0]);
                return;
            case TP_LONG_DOUBLE_COMPLEX:
                dst_data_field->databool=(_Bool)(src_data_field->longdoublecomplex[0]);
                return;
            case TP_NULL_POINTER_CONSTANT:
                dst_data_field->databool=(_Bool)(src_data_field->sint);
                return;
            default:
                return;
        }
    }
    case TP_FLOAT_COMPLEX:
    {
        switch(src_type)
        {
            case TP_SCHAR:
                dst_data_field->floatcomplex[0]=(float)(src_data_field->schar);
                return;
            case TP_USCHAR:
                dst_data_field->floatcomplex[0]=(float)(src_data_field->uschar);
                return;
            case TP_SSHORT:
                dst_data_field->floatcomplex[0]=(float)(src_data_field->sshort);
                return;
            case TP_USHORT:
                dst_data_field->floatcomplex[0]=(float)(src_data_field->usshort);
                return;
            case TP_SINT:
                dst_data_field->floatcomplex[0]=(float)(src_data_field->sint);
                return;
            case TP_USINT:
                dst_data_field->floatcomplex[0]=(float)(src_data_field->usint);
                return;
            case TP_SLONG:
                dst_data_field->floatcomplex[0]=(float)(src_data_field->slong);
                return;
            case TP_USLONG:
                dst_data_field->floatcomplex[0]=(float)(src_data_field->uslong);
                return;
            case TP_SLONGLONG:
                dst_data_field->floatcomplex[0]=(float)(src_data_field->sllong);
                return;
            case TP_USLONGLONG:
                dst_data_field->floatcomplex[0]=(float)(src_data_field->usllong);
                return;
            case TP_FLOAT:
                dst_data_field->floatcomplex[0]=(float)(src_data_field->datafloat);
                return;
            case TP_DOUBLE:
                dst_data_field->floatcomplex[0]=(float)(src_data_field->datadouble);
                return;
            case TP_LONG_DOUBLE:
                dst_data_field->floatcomplex[0]=(float)(src_data_field->datalongdouble);
                return;
            case TP_BOOL:
                dst_data_field->floatcomplex[0]=(float)(src_data_field->databool);
                return;
            case TP_FLOAT_COMPLEX:
                dst_data_field->floatcomplex[0]=(src_data_field->floatcomplex[0]);
                dst_data_field->floatcomplex[1]=(src_data_field->floatcomplex[1]);
                return;
            case TP_DOUBLE_COMPLEX:
                dst_data_field->floatcomplex[0]=(float)(src_data_field->doublecomplex[0]);
                dst_data_field->floatcomplex[1]=(float)(src_data_field->doublecomplex[1]);
                return;
            case TP_LONG_DOUBLE_COMPLEX:
                dst_data_field->floatcomplex[0]=(float)(src_data_field->longdoublecomplex[0]);
                dst_data_field->floatcomplex[1]=(float)(src_data_field->longdoublecomplex[1]);
                return;
            case TP_NULL_POINTER_CONSTANT:
                dst_data_field->floatcomplex[0]=(float)(src_data_field->sint);
                return;
            default:
                return;
        }
    }
    case TP_DOUBLE_COMPLEX:
    {
        switch(src_type)
        {
            case TP_SCHAR:
                dst_data_field->doublecomplex[0]=(double)(src_data_field->schar);
                return;
            case TP_USCHAR:
                dst_data_field->doublecomplex[0]=(double)(src_data_field->uschar);
                return;
            case TP_SSHORT:
                dst_data_field->doublecomplex[0]=(double)(src_data_field->sshort);
                return;
            case TP_USHORT:
                dst_data_field->doublecomplex[0]=(double)(src_data_field->usshort);
                return;
            case TP_SINT:
                dst_data_field->doublecomplex[0]=(double)(src_data_field->sint);
                return;
            case TP_USINT:
                dst_data_field->doublecomplex[0]=(double)(src_data_field->usint);
                return;
            case TP_SLONG:
                dst_data_field->doublecomplex[0]=(double)(src_data_field->slong);
                return;
            case TP_USLONG:
                dst_data_field->doublecomplex[0]=(double)(src_data_field->uslong);
                return;
            case TP_SLONGLONG:
                dst_data_field->doublecomplex[0]=(double)(src_data_field->sllong);
                return;
            case TP_USLONGLONG:
                dst_data_field->doublecomplex[0]=(double)(src_data_field->usllong);
                return;
            case TP_FLOAT:
                dst_data_field->doublecomplex[0]=(double)(src_data_field->datafloat);
                return;
            case TP_DOUBLE:
                dst_data_field->doublecomplex[0]=(double)(src_data_field->datadouble);
                return;
            case TP_LONG_DOUBLE:
                dst_data_field->doublecomplex[0]=(double)(src_data_field->datalongdouble);
                return;
            case TP_BOOL:
                dst_data_field->doublecomplex[0]=(double)(src_data_field->databool);
                return;
            case TP_FLOAT_COMPLEX:
                dst_data_field->doublecomplex[0]=(double)(src_data_field->floatcomplex[0]);
                dst_data_field->doublecomplex[1]=(double)(src_data_field->floatcomplex[1]);
                return;
            case TP_DOUBLE_COMPLEX:
                dst_data_field->doublecomplex[0]=(src_data_field->doublecomplex[0]);
                dst_data_field->doublecomplex[1]=(src_data_field->doublecomplex[1]);
                return;
            case TP_LONG_DOUBLE_COMPLEX:
                dst_data_field->doublecomplex[0]=(double)(src_data_field->longdoublecomplex[0]);
                dst_data_field->doublecomplex[1]=(double)(src_data_field->longdoublecomplex[1]);
                return;
            case TP_NULL_POINTER_CONSTANT:
                dst_data_field->doublecomplex[0]=(double)(src_data_field->sint);
                return;
            default:
                return;
        }
    }
    case TP_LONG_DOUBLE_COMPLEX:
    {
        switch(src_type)
        {
            case TP_SCHAR:
                dst_data_field->longdoublecomplex[0]=(long double)(src_data_field->schar);
                return;
            case TP_USCHAR:
                dst_data_field->longdoublecomplex[0]=(long double)(src_data_field->uschar);
                return;
            case TP_SSHORT:
                dst_data_field->longdoublecomplex[0]=(long double)(src_data_field->sshort);
                return;
            case TP_USHORT:
                dst_data_field->longdoublecomplex[0]=(long double)(src_data_field->usshort);
                return;
            case TP_SINT:
                dst_data_field->longdoublecomplex[0]=(long double)(src_data_field->sint);
                return;
            case TP_USINT:
                dst_data_field->longdoublecomplex[0]=(long double)(src_data_field->usint);
                return;
            case TP_SLONG:
                dst_data_field->longdoublecomplex[0]=(long double)(src_data_field->slong);
                return;
            case TP_USLONG:
                dst_data_field->longdoublecomplex[0]=(long double)(src_data_field->uslong);
                return;
            case TP_SLONGLONG:
                dst_data_field->longdoublecomplex[0]=(long double)(src_data_field->sllong);
                return;
            case TP_USLONGLONG:
                dst_data_field->longdoublecomplex[0]=(long double)(src_data_field->usllong);
                return;
            case TP_FLOAT:
                dst_data_field->longdoublecomplex[0]=(long double)(src_data_field->datafloat);
                return;
            case TP_DOUBLE:
                dst_data_field->longdoublecomplex[0]=(long double)(src_data_field->datadouble);
                return;
            case TP_LONG_DOUBLE:
                dst_data_field->longdoublecomplex[0]=(long double)(src_data_field->datalongdouble);
                return;
            case TP_BOOL:
                dst_data_field->longdoublecomplex[0]=(long double)(src_data_field->databool);
                return;
            case TP_FLOAT_COMPLEX:
                dst_data_field->longdoublecomplex[0]=(long double)(src_data_field->floatcomplex[0]);
                dst_data_field->longdoublecomplex[1]=(long double)(src_data_field->floatcomplex[1]);
                return;
            case TP_DOUBLE_COMPLEX:
                dst_data_field->longdoublecomplex[0]=(long double)(src_data_field->doublecomplex[0]);
                dst_data_field->longdoublecomplex[1]=(long double)(src_data_field->doublecomplex[1]);
                return;
            case TP_LONG_DOUBLE_COMPLEX:
                dst_data_field->longdoublecomplex[0]=(src_data_field->longdoublecomplex[0]);
                dst_data_field->longdoublecomplex[1]=(src_data_field->longdoublecomplex[1]);
                return;
            case TP_NULL_POINTER_CONSTANT:
                dst_data_field->longdoublecomplex[0]=(long double)(src_data_field->sint);
                return;
            default:
                return;
        }
    }
    default:
        return;
    }
}
void plus_const(enum TP_CATEGORY type,VALUE_DATA* dst_data_field,VALUE_DATA* src_data_field)
{
    switch (type)
    {
    case TP_SCHAR:
        dst_data_field->schar=+(src_data_field->schar);
        return;
    case TP_USCHAR:
        dst_data_field->uschar=+(src_data_field->uschar);
        return;
    case TP_SSHORT:
        dst_data_field->sshort=+(src_data_field->sshort);
        return;
    case TP_USHORT:
        dst_data_field->usshort=+(src_data_field->usshort);
        return;
    case TP_SINT:
        dst_data_field->sint=+(src_data_field->sint);
        return;
    case TP_USINT:
        dst_data_field->usint=+(src_data_field->usint);
        return;
    case TP_SLONG:
        dst_data_field->slong=+(src_data_field->slong);
        return;
    case TP_USLONG:
        dst_data_field->uslong=+(src_data_field->uslong);
        return;
    case TP_SLONGLONG:
        dst_data_field->sllong=+(src_data_field->sllong);
        return;
    case TP_USLONGLONG:
        dst_data_field->usllong=+(src_data_field->usllong);
        return;
    case TP_FLOAT:
        dst_data_field->datafloat=+(src_data_field->datafloat);
        return;
    case TP_DOUBLE:
        dst_data_field->datadouble=+(src_data_field->datadouble);
        return;
    case TP_LONG_DOUBLE:
        dst_data_field->datalongdouble=+(src_data_field->datalongdouble);
        return;
    case TP_BOOL:
        dst_data_field->databool=+(src_data_field->databool);
        return;
    case TP_FLOAT_COMPLEX:
        dst_data_field->floatcomplex[0]=+(src_data_field->floatcomplex[0]);
        return;
    case TP_DOUBLE_COMPLEX:
        dst_data_field->doublecomplex[0]=+(src_data_field->doublecomplex[0]);
        return;
    case TP_LONG_DOUBLE_COMPLEX:
        dst_data_field->longdoublecomplex[0]=+(src_data_field->longdoublecomplex[0]);
        return;
    case TP_NULL_POINTER_CONSTANT:
        dst_data_field->sint=+(src_data_field->sint);
        return;
    default:
        return;
    }
}
void minus_const(enum TP_CATEGORY type,VALUE_DATA* dst_data_field,VALUE_DATA* src_data_field)
{
    switch (type)
    {
    case TP_SCHAR:
        dst_data_field->schar=-(src_data_field->schar);
        return;
    case TP_USCHAR:
        dst_data_field->uschar=-(src_data_field->uschar);
        return;
    case TP_SSHORT:
        dst_data_field->sshort=-(src_data_field->sshort);
        return;
    case TP_USHORT:
        dst_data_field->usshort=-(src_data_field->usshort);
        return;
    case TP_SINT:
        dst_data_field->sint=-(src_data_field->sint);
        return;
    case TP_USINT:
        dst_data_field->usint=-(src_data_field->usint);
        return;
    case TP_SLONG:
        dst_data_field->slong=-(src_data_field->slong);
        return;
    case TP_USLONG:
        dst_data_field->uslong=-(src_data_field->uslong);
        return;
    case TP_SLONGLONG:
        dst_data_field->sllong=-(src_data_field->sllong);
        return;
    case TP_USLONGLONG:
        dst_data_field->usllong=-(src_data_field->usllong);
        return;
    case TP_FLOAT:
        dst_data_field->datafloat=-(src_data_field->datafloat);
        return;
    case TP_DOUBLE:
        dst_data_field->datadouble=-(src_data_field->datadouble);
        return;
    case TP_LONG_DOUBLE:
        dst_data_field->datalongdouble=-(src_data_field->datalongdouble);
        return;
    case TP_BOOL:
        dst_data_field->databool=-(src_data_field->databool);
        return;
    case TP_FLOAT_COMPLEX:
        dst_data_field->floatcomplex[0]=-(src_data_field->floatcomplex[0]);
        return;
    case TP_DOUBLE_COMPLEX:
        dst_data_field->doublecomplex[0]=-(src_data_field->doublecomplex[0]);
        return;
    case TP_LONG_DOUBLE_COMPLEX:
        dst_data_field->longdoublecomplex[0]=-(src_data_field->longdoublecomplex[0]);
        return;
    case TP_NULL_POINTER_CONSTANT:
        dst_data_field->sint=-(src_data_field->sint);
        return;
    default:
        return;
    }
}
void tilde_const(enum TP_CATEGORY type,VALUE_DATA* dst_data_field,VALUE_DATA* src_data_field)
{
    switch (type)
    {
    case TP_SCHAR:
        dst_data_field->schar=~(src_data_field->schar);
        return;
    case TP_USCHAR:
        dst_data_field->uschar=~(src_data_field->uschar);
        return;
    case TP_SSHORT:
        dst_data_field->sshort=~(src_data_field->sshort);
        return;
    case TP_USHORT:
        dst_data_field->usshort=~(src_data_field->usshort);
        return;
    case TP_SINT:
        dst_data_field->sint=~(src_data_field->sint);
        return;
    case TP_USINT:
        dst_data_field->usint=~(src_data_field->usint);
        return;
    case TP_SLONG:
        dst_data_field->slong=~(src_data_field->slong);
        return;
    case TP_USLONG:
        dst_data_field->uslong=~(src_data_field->uslong);
        return;
    case TP_SLONGLONG:
        dst_data_field->sllong=~(src_data_field->sllong);
        return;
    case TP_USLONGLONG:
        dst_data_field->usllong=~(src_data_field->usllong);
        return;
    case TP_BOOL:
        dst_data_field->databool=!(src_data_field->databool);
        return;
    case TP_NULL_POINTER_CONSTANT:
        dst_data_field->sint=~(src_data_field->sint);
        return;
    default:
        return;
    }
}
int exclamation_const(enum TP_CATEGORY type,VALUE_DATA* data_field)
{
    switch (type)
    {
    case TP_SCHAR:
        return !(data_field->schar);
    case TP_USCHAR:
        return !(data_field->uschar);
    case TP_SSHORT:
        return !(data_field->sshort);
    case TP_USHORT:
        return !(data_field->usshort);
    case TP_SINT:
        return !(data_field->sint);
    case TP_USINT:
        return !(data_field->usint);
    case TP_SLONG:
        return !(data_field->slong);
    case TP_USLONG:
        return !(data_field->uslong);
    case TP_SLONGLONG:
        return !(data_field->sllong);
    case TP_USLONGLONG:
        return !(data_field->usllong);
    case TP_FLOAT:
        return !(data_field->datafloat);
    case TP_DOUBLE:
        return !(data_field->datadouble);
    case TP_LONG_DOUBLE:
        return !(data_field->datalongdouble);
    case TP_BOOL:
        return !(data_field->databool);
    case TP_FLOAT_COMPLEX:
        return !(data_field->floatcomplex[0]);
    case TP_DOUBLE_COMPLEX:
        return !(data_field->doublecomplex[0]);
    case TP_LONG_DOUBLE_COMPLEX:
        return !(data_field->longdoublecomplex[0]);
    case TP_POINT:
        return !(data_field->pointer);
    case TP_NULL_POINTER_CONSTANT:
        return !(data_field->sint);
    default:
        return 0;
    }
}