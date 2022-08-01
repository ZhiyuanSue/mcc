#ifndef _LEX_STATE_H_
#define _LEX_STATE_H_
#define STATE_NUM 256
#define STATE_CHAR_LEN 256  //ascii code has only 128
#define TERMINATOR_NUM 107
#define NONTER_NUM 238
#define PARSER_NONTER_NUM 80
#define IS_EXPR_NODE(rule_type) (rule_type==expression||(rule_type>=assignment_expr&&rule_type<=primary_expression))
#define IS_STMT_NODE(rule_type) (rule_type>=labeled_stmt&&rule_type<=asm_stmt)
enum rule_type_enum{
    /*child*/
    child,
    /*lex,or terminator 103*/
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
    /*parser rule,nonterminal 74*/
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
    /*asm statement*/
    asm_stmt,ASM_extend,
    ASM_output,ASM_input,ASM_register,ASM_restrict,
    /*end of file*/
    end_of_file
};
#endif