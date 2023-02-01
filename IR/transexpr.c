#include "transexpr.h"
extern size_t type_data_size[TYPE_NUM];
bool expr_trans_dispatch(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    bool (*expr_translation[EXPR_NUM])(AST_BASE* ast_node,IR_BB* ir_bb)={
        expr_trans,assign_expr_trans,cond_expr_trans,logical_or_expr_trans,
        logical_and_expr_trans,bit_inclusive_or_expr_trans,bit_exclusive_or_expr_trans,and_expr_trans,
        equal_expr_trans,relation_expr_trans,shift_expr_trans,add_expr_trans,
        mul_expr_trans,cast_expr_trans,unary_expr_trans,postfix_expr_trans,
        pri_expr_trans
    };
    bool res=false;
    if(ast_node->type==expression)
        res=expr_translation[0](ast_node,ir_bb);
    else if(ast_node->type>=assignment_expr&&ast_node->type<=primary_expression){
        res=expr_translation[ast_node->type-assignment_expr+1](ast_node,ir_bb);
    }
    return res;
error:
    return false;
}
bool cond_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    return true;
error:
    return false;
}
bool expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    return true;
error:
    return false;
}
bool assign_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    return true;
error:
    return false;
}
bool assign_trans(SYM_ITEM* src_symbol,
    SYM_ITEM* dst_symbol,
    IR_BB* ir_bb)
{
    /*at semantic stage,the const expr value case have been dealed*/
    if(src_symbol->const_expr&&dst_symbol->const_expr)
        return true;
    else if(src_symbol->const_expr||dst_symbol->const_expr)
        return false;   /*only one is const_expr,the semantic must have error*/
    else{
        M_TYPE* src_base_type=Type_VEC_get_actual_base_type(src_symbol->type_vec);
        M_TYPE* dst_base_type=Type_VEC_get_actual_base_type(dst_symbol->type_vec);
        if(!src_base_type||!dst_base_type)
        {
            printf("symbol type error!\n"); /*impossible if everything OK*/
            return false;
        }
        IR_INS* ir_ins=add_new_ins(ir_bb);
        /*
            for arthmatic case,do some conversion
            but the target type have been decided by previous stage,so no need to do it
            besides, at the int type case,you must consider the bit-field problem
            but,for one is a bit-field of a struct(as a int type,it cannot be a float),another one is a float case
            I haven't find any word in C11 std(maybe it's my fault,so if you find it,please tell me)
            I also didn't find any warning or error in gcc(e.x. assign a double to an i1 type)
            so I just convert the float to int and then trunc it and assign,as other case did
        */
        if(IS_INT_TYPE(src_base_type->typ_category)&&IS_INT_TYPE(dst_base_type->typ_category))
        {

        }
        else if(IS_INT_TYPE(src_base_type->typ_category)&&IS_FLOAT_TYPE(dst_base_type->typ_category))
        {

        }
        else if(IS_FLOAT_TYPE(src_base_type->typ_category)&&IS_INT_TYPE(dst_base_type->typ_category))
        {

        }
        else if(IS_FLOAT_TYPE(src_base_type->typ_category)&&IS_FLOAT_TYPE(dst_base_type->typ_category))
        {

        }
#if _CPLX_SUPPORT==1
        else if(IS_COMPLEX_TYPE(src_base_type->typ_category)&&IS_INT_TYPE(dst_base_type->typ_category))
        {

        }
        else if(IS_COMPLEX_TYPE(src_base_type->typ_category)&&IS_FLOAT_TYPE(dst_base_type->typ_category))
        {

        }
        else if(IS_INT_TYPE(src_base_type->typ_category)&&IS_COMPLEX_TYPE(dst_base_type->typ_category))
        {

        }
        else if(IS_FLOAT_TYPE(src_base_type->typ_category)&&IS_COMPLEX_TYPE(dst_base_type->typ_category))
        {

        }
        else if(IS_COMPLEX_TYPE(src_base_type->typ_category)&&IS_COMPLEX_TYPE(dst_base_type->typ_category))
        {

        }
#endif
        /*then is struct /union case*/
        /*else if()
        {

        }*/
        /*here use lvalue conversion,so array or function also treated as pointer*/
        /*point-point case*/
        /*else if()
        {

        }*/
        /*pointer-null pointer case*/
        /*else if()
        {

        }*/
        /*bool-pointer case*/
        /*else if()
        {

        }*/
    }
    return true;
}
bool logical_or_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    return true;
error:
    return false;
}
bool logical_and_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    return true;
error:
    return false;
}
bool bit_inclusive_or_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    return true;
error:
    return false;
}
bool bit_exclusive_or_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    return true;
error:
    return false;
}
bool and_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    return true;
error:
    return false;
}
bool equal_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    return true;
error:
    return false;
}
bool relation_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    return true;
error:
    return false;
}
bool shift_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    return true;
error:
    return false;
}
bool add_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    return true;
error:
    return false;
}
bool mul_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    return true;
error:
    return false;
}
bool cast_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    return true;
error:
    return false;
}
bool unary_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    return true;
error:
    return false;
}
bool postfix_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    
    return true;
error:
    return false;
}
bool pri_expr_trans(AST_BASE* ast_node,IR_BB* ir_bb)
{
    if(!ast_node||!ir_bb)
        goto error;
    return true;
error:
    return false;
}