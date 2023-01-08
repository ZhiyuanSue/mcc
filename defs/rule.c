#include "rule.h"
const char preprocess_token_rules[MAX_RULE_NUM][RULE_CHAR_LEN]={
    /*preprocess has 7 part
    header-name
    identifier
    pp-number
    char-const
    string-literal
    punctuator
    ?? each non-white-space character that connot be one of the above */
    "whitespace:${whitespace}",
    /*header_names 7*/
    /* If the characters ', \, ", //, or '/'* occur in the sequence between the < and > delimiters,the behavior is undefined */
    "header_name:&{&header_name1}|&{&header_name2}",
    "&header_name1:${less_than}&{&h_char_sequence}${greater_than}",
    "&header_name2:${double_quote}&{&p_char_sequence}${double_quote}",

    "&h_char_sequence:${h_char_set}&{&h_char1}",
    "&h_char1:${h_char_set}*",

    "&p_char_sequence:${p_char_set}&{&p_char1}",
    "&p_char1:${p_char_set}*"
    /*Preprocessing numbers ???
        I find very little knowledge about preprocessing number
        accroding to the rule,a pp_num can start with a dot and end with a dot?? strange... */

    /*character const 5 */
    "char_const:&{&c_char_prefix_opt}${single_quote}&{&c_char_sequence_opt}${single_quote}",
    "&c_char_prefix_opt:${c_char_prefix}\?",
    
    "&c_char_sequence_opt:&{&c_char_sequence}\?",
    "&c_char_sequence:&{&c_char}&{&c_char1}",
    "&c_char:${c_char_set}|&{&esc_sequence}",
    "&c_char1:&{&c_char}*",
    
    /*escape sequence 4 */
    "&esc_sequence:&{&simple_esc}|&{&oct_esc}|&{&hex_esc}|&{universal_char_name}",
    "&simple_esc:&{&esc_single_quote}|&{&esc_double_quote}|&{&esc_question}|&{&esc_back_slash}|&{&esc_alert}|&{&esc_backspace}|&{&esc_form_feed}|&{&esc_new_line}|&{&esc_return}|&{&esc_h_tab}|&{&esc_v_tab}",
    "&oct_esc:${back_slash}&{&oct_digit}",
    "&hex_esc:${back_slash}${lx}${hex_digit}&{&hex_esc_seq1}",

    /* simple_escape_sequence 11*/
    "&esc_single_quote:${back_slash}${single_quote}",
    "&esc_double_quote:${back_slash}${double_quote}",
    "&esc_question:${back_slash}${question}",
    "&esc_back_slash:${back_slash}${back_slash}",
    "&esc_alert:${back_slash}${la}",
    "&esc_backspace:${back_slash}${lb}",
    "&esc_form_feed:${back_slash}${lf}",
    "&esc_new_line:${back_slash}${ln}",
    "&esc_return:${back_slash}${ln}",
    "&esc_h_tab:${back_slash}${lt}",
    "&esc_v_tab:${back_slash}${lv}",

    /*octal_escape_sequence 4 */
    "&oct_digit:&{&oct_digit1}|&{&oct_digit2}|&{&oct_digit3}",
    "&oct_digit1:${octal_digit}",
    "&oct_digit2:${octal_digit}${octal_digit}",
    "&oct_digit3:${octal_digit}${octal_digit}${octal_digit}",
    /*hex_escape_sequence */
    "&hex_esc_seq1:${hex_digit}*",
    /*universal_character_name 4 lines */
    "universal_char_name:&{&u_char_name1}|&{&u_char_name2}",
    "&u_char_name1:${back_slash}${lu}&{&hex_quad}",
    "&u_char_name2:${back_slash}${bu}&{&hex_quad}&{&hex_quad}",
    "&hex_quad:${hex_digit}${hex_digit}${hex_digit}${hex_digit}",
    /* string literal 7 */
    "string:&{&encoding_prefix_opt}${double_quote}&{&s_char_seq_opt}${double_quote}",

    "&encoding_prefix_opt:&{&encoding_prefix_opt1}\?",
    "&encoding_prefix_opt1:&{&encoding_prefix_opt2}|${c_char_prefix}",
    "&encoding_prefix_opt2:${lu}${eight}",

    "&s_char_seq_opt:&{&s_char_seq}\?",
    "&s_char_seq:&{&s_char}&{&s_char_seq1}",
    "&s_char_seq1:&{&s_char}*",
    "&s_char:${s_char_set}|&{&esc_sequence}",
    /* punctuator 54  I think ,you'd better don't use '&' at the beginning */
    "[:${left_bracket}",
    "]:${right_bracket}",
    "(:${left_parenthesis}",
    "):${right_parenthesis}",
    "{:${left_brace}",
    "}:${right_brace}",
    ".:${dot}",
    "->:${minus}${greater_than}",
    "++:${plus}${plus}",
    "--:${minus}${minus}",
    "ampersand:${ampersand}",
    "*:${asterisk}",
    "+:${plus}",
    "-:${minus}",
    "~:${tilde}",
    "!:${exclamation}",
    "/:${forward_slash}",
    "%:${percent}",
    "<<:${less_than}${less_than}",
    ">>:${greater_than}${greater_than}",
    "<:${less_than}",
    ">:${greater_than}",
    "<=:${less_than}${equal}",
    ">=:${greater_than}${equal}",
    "==:${equal}${equal}",
    "!=:${exclamation}${equal}",
    "^:${caret}",
    "|:${vertical_bar}",
    "double_ampersand:${ampersand}${ampersand}",
    "||:${vertical_bar}${vertical_bar}",
    "\?:${question}",
    "colon:${colon}",
    ";:${semi_colon}",
    "...:${dot}${dot}${dot}",
    "=:${equal}",
    "*=:${asterisk}${equal}",
    "/=:${forward_slash}${equal}",
    "%=:${percent}${equal}",
    "+=:${plus}${equal}",
    "-=:${minus}${equal}",
    "<<=:${less_than}${less_than}${equal}",
    ">>=:${greater_than}${greater_than}${equal}",
    "ampersand=:${ampersand}${equal}",
    "^=:${caret}${equal}",
    "|=:${vertical_bar}${equal}",
    ",:${comma}",
    "#:${number_sign}",
    "##:${number_sign}${number_sign}",
    "<colon:${less_than}${colon}",
    "colon>:${colon}${greater_than}",
    "<%:${less_than}${percent}",
    "%>:${percent}${greater_than}",
    "%colon:${percent}${colon}",
    "double_%colon:${percent}${colon}${percent}${colon}"
};

const char token_rules[MAX_RULE_NUM][RULE_CHAR_LEN]={
    /*C token has 5 part
        keyword
        identifier
        constant
        string-literal
        punctuator */
    "whitespace:${whitespace}",
    /*keyword 44 lines*/
    "KW_auto:${la}${lu}${lt}${lo}",
    "KW_break:${lb}${lr}${le}${la}${lk}",
    "KW_case:${lc}${la}${ls}${le}",
    "KW_char:${lc}${lh}${la}${lr}",
    "KW_const:${lc}${lo}${ln}${ls}${lt}",
    "KW_continue:${lc}${lo}${ln}${lt}${li}${ln}${lu}${le}",
    "KW_default:${ld}${le}${lf}${la}${lu}${ll}${lt}",
    "KW_do:${ld}${lo}",
    "KW_double:${ld}${lo}${lu}${lb}${ll}${le}",
    "KW_else:${le}${ll}${ls}${le}",
    "KW_enum:${le}${ln}${lu}${lm}",
    "KW_extern:${le}${lx}${lt}${le}${lr}${ln}",
    "KW_float:${lf}${ll}${lo}${la}${lt}",
    "KW_for:${lf}${lo}${lr}",
    "KW_goto:${lg}${lo}${lt}${lo}",
    "KW_if:${li}${lf}",
    "KW_inline:${li}${ln}${ll}${li}${ln}${le}",
    "KW_int:${li}${ln}${lt}",
    "KW_long:${ll}${lo}${ln}${lg}",
    "KW_register:${lr}${le}${lg}${li}${ls}${lt}${le}${lr}",
    "KW_restrict:${lr}${le}${ls}${lt}${lr}${li}${lc}${lt}",
    "KW_return:${lr}${le}${lt}${lu}${lr}${ln}",
    "KW_short:${ls}${lh}${lo}${lr}${lt}",
    "KW_signed:${ls}${li}${lg}${ln}${le}${ld}",
    "KW_sizeof:${ls}${li}${lz}${le}${lo}${lf}",
    "KW_static:${ls}${lt}${la}${lt}${li}${lc}",
    "KW_struct:${ls}${lt}${lr}${lu}${lc}${lt}",
    "KW_switch:${ls}${lw}${li}${lt}${lc}${lh}",
    "KW_typedef:${lt}${ly}${lp}${le}${ld}${le}${lf}",
    "KW_union:${lu}${ln}${li}${lo}${ln}",
    "KW_unsigned:${lu}${ln}${ls}${li}${lg}${ln}${le}${ld}",
    "KW_void:${lv}${lo}${li}${ld}",
    "KW_volatile:${lv}${lo}${ll}${la}${lt}${li}${ll}${le}",
    "KW_while:${lw}${lh}${li}${ll}${le}",
    "KW__Alignas:${underscore}${ba}${ll}${li}${lg}${ln}${la}${ls}",
    "KW__Alignof:${underscore}${ba}${ll}${li}${lg}${ln}${lo}${lf}",
    "KW__Atomic:${underscore}${ba}${lt}${lo}${lm}${li}${lc}",
    "KW__Bool:${underscore}${bb}${lo}${lo}${ll}",
    "KW__Complex:${underscore}${bc}${lo}${lm}${lp}${ll}${le}${lx}",
    "KW__Generic:${underscore}${bg}${le}${ln}${le}${lr}${li}${lc}",
    "KW__Imaginary:${underscore}${bi}${lm}${la}${lg}${li}${ln}${la}${lr}${ly}",
    "KW__Noreturn:${underscore}${bn}${lo}${lr}${le}${lt}${lu}${lr}${ln}",
    "KW__Static_assert:${underscore}${bs}${lt}${la}${lt}${li}${lc}${underscore}${la}${ls}${ls}${le}${lr}${lt}",
    "KW__Thread_local:${underscore}${bt}${lh}${lr}${le}${la}${ld}${underscore}${ll}${lo}${lc}${la}${ll}",
    "KW_asm:${underscore}${underscore}${la}${ls}${lm}${underscore}${underscore}",
    "KW_asm_volatile:${underscore}${underscore}${lv}${lo}${ll}${la}${lt}${li}${ll}${le}${underscore}${underscore}",
    /*identifier 4 lines*/
    "identifier:&{&identifier-nondigit}&{&identifier2}",
    "&identifier-nondigit:${nondigit}|&{universal_char_name}",
    "&identifier1:&{&identifier-nondigit}|${digit}",
    "&identifier2:&{&identifier1}*",
    /*universal_character_name 4 lines*/
    "universal_char_name:&{&u_char_name1}|&{&u_char_name2}",
    "&u_char_name1:${back_slash}${lu}&{&hex_quad}",
    "&u_char_name2:${back_slash}${bu}&{&hex_quad}&{&hex_quad}",
    "&hex_quad:${hex_digit}${hex_digit}${hex_digit}${hex_digit}",
    /*integer_constants 11 lines*/
    "integer_constant:&{&int_constant}&{&int_suffix}",
    "&int_constant:&{&dec_constant}|&{&oct_constant}|&{&hex_constant}",
    
    "&dec_constant:${nonzero_digit}&{&dec_con1}",
    "&dec_con1:${digit}*",

    "&oct_constant:${zero}&{&oct_con1}",
    "&oct_con1:${octal_digit}*",

    "&hex_constant:&{&hex_prefix}&{&hex_con1}",
    "&hex_con1:${hex_digit}&{&hex_con2}",
    "&hex_con2:${hex_digit}*",
    "&hex_prefix:${zero}&{&hex_pre1}",
    "&hex_pre1:${lx}|${bx}",
    /*integer_constants suffix 13 lines*/
    "&int_suffix:&{&int_suffix0}\?",
    "&int_suffix0:&{&int_suffix1}|&{&int_suffix2}|&{&int_suffix3}|&{&int_suffix4}",
    "&int_suffix1:&{&unsigned_suffix}&{&long_suffix_opt}",
    "&int_suffix2:&{&unsigned_suffix}&{&long_long_suffix}",
    "&int_suffix3:&{&long_suffix}&{&unsigned_suffix_opt}",
    "&int_suffix4:&{&long_long_suffix}&{&unsigned_suffix_opt}",

    "&long_suffix_opt:&{&long_suffix}\?",
    "&unsigned_suffix_opt:&{&unsigned_suffix}\?",

    "&unsigned_suffix:${lu}|${bu}",
    "&long_suffix:${ll}|${bl}",
    "&long_long_suffix:&{&ll_suffix1}|&{&ll_suffix2}",
    "&ll_suffix1:${ll}${ll}",
    "&ll_suffix2:${bl}${bl}",
    /*floating constant*/
    "floating_constant:&{&dec_float_const}|&{&hex_float_const}",

    /*dec 8*/
    "&dec_float_const:&{&dec_float_const1}|&{&dec_float_const2}",
    "&dec_float_const1:&{&frac_const}&{&exp_part_opt}&{&float_suffix_opt}",
    "&dec_float_const2:&{&digit_sequence}&{&exp_part}&{&float_suffix_opt}",
    
    "&frac_const:&{&frac_const1}|&{&frac_const2}",
    "&frac_const1:&{&digit_sequence_opt}${dot}&{&digit_sequence}",
    "&frac_const2:&{&digit_sequence}${dot}",

    "&exp_part_opt:&{&exp_part}\?",
    "&exp_part:${exp_char}&{&sign_opt}&{&digit_sequence}",
    /*hex 7*/
    "&hex_float_const:&{&hex_float_const1}|&{&hex_float_const2}",
    "&hex_float_const1:&{&hex_prefix}&{&hex_frac_const}&{&bi_exp_part}&{&float_suffix_opt}",
    "&hex_float_const2:&{&hex_prefix}&{&hex_digit_sequence}&{&bi_exp_part}&{&float_suffix_opt}",

    "&hex_frac_const:&{&hex_frac_const1}|&{&hex_frac_const2}",
    "&hex_frac_const1:&{&hex_digit_sequence_opt}${dot}&{&hex_digit_sequence}",
    "&hex_frac_const2:&{&hex_digit_sequence}${dot}",

    "&bi_exp_part:${bi_exp_char}&{&sign_opt}&{&digit_sequence}",
    /*other 8*/
    "&float_suffix_opt:${float_suffix}\?",
    "&sign_opt:${sign}\?",
    
    "&digit_sequence_opt:&{&digit_sequence}\?",
    "&digit_sequence:${digit}&{&digit_sequence1}",
    "&digit_sequence1:${digit}*",

    "&hex_digit_sequence_opt:&{&hex_digit_sequence}\?",
    "&hex_digit_sequence:${hex_digit}&{&hex_digit_sequence1}",
    "&hex_digit_sequence1:${hex_digit}*",
    /*enum ,emm I don't know ,I change this rule to parser rule*/
    /*"enum_const:&{identifier}",*/
    /*character const 5*/
    "char_const:&{&c_char_prefix_opt}${single_quote}&{&c_char_sequence_opt}${single_quote}",
    "&c_char_prefix_opt:${c_char_prefix}\?",
    
    "&c_char_sequence_opt:&{&c_char_sequence}\?",
    "&c_char_sequence:&{&c_char1}",
    "&c_char:${c_char_set}|&{&esc_sequence}",
    "&c_char1:&{&c_char}*",

    /*escape sequence 4*/
    "&esc_sequence:&{&simple_esc}|&{&oct_esc}|&{&hex_esc}|&{universal_char_name}",
    "&simple_esc:&{&esc_single_quote}|&{&esc_double_quote}|&{&esc_question}|&{&esc_back_slash}|&{&esc_alert}|&{&esc_backspace}|&{&esc_form_feed}|&{&esc_new_line}|&{&esc_return}|&{&esc_h_tab}|&{&esc_v_tab}",
    "&oct_esc:${back_slash}&{&oct_digit}",
    "&hex_esc:${back_slash}${lx}${hex_digit}&{&hex_esc_seq1}",

    /*simple_escape_sequence 11 */
    "&esc_single_quote:${back_slash}${single_quote}",
    "&esc_double_quote:${back_slash}${double_quote}",
    "&esc_question:${back_slash}${question}",
    "&esc_back_slash:${back_slash}${back_slash}",
    "&esc_alert:${back_slash}${la}",
    "&esc_backspace:${back_slash}${lb}",
    "&esc_form_feed:${back_slash}${lf}",
    "&esc_new_line:${back_slash}${ln}",
    "&esc_return:${back_slash}${ln}",
    "&esc_h_tab:${back_slash}${lt}",
    "&esc_v_tab:${back_slash}${lv}",

    /*octal_escape_sequence 4 */
    "&oct_digit:&{&oct_digit1}|&{&oct_digit2}|&{&oct_digit3}",
    "&oct_digit1:${octal_digit}",
    "&oct_digit2:${octal_digit}${octal_digit}",
    "&oct_digit3:${octal_digit}${octal_digit}${octal_digit}",
    /*hex_escape_sequence */
    "&hex_esc_seq1:${hex_digit}*",

    /*string literal 7*/
    "string:&{&encoding_prefix_opt}${double_quote}&{&s_char_seq_opt}${double_quote}",

    "&encoding_prefix_opt:&{&encoding_prefix_opt1}\?",
    "&encoding_prefix_opt1:&{&encoding_prefix_opt2}|${c_char_prefix}",
    "&encoding_prefix_opt2:${lu}${eight}",

    "&s_char_seq_opt:&{&s_char_seq}\?",
    "&s_char_seq:&{&s_char_seq1}",
    "&s_char_seq1:&{&s_char}*",
    "&s_char:${s_char_set}|&{&esc_sequence}",

    /*punctuator 54  I think ,you'd better don't use '&' at the beginning */
    "[:${left_bracket}",
    "]:${right_bracket}",
    "(:${left_parenthesis}",
    "):${right_parenthesis}",
    "left_brace:${left_brace}",
    "right_brace:${right_brace}",
    ".:${dot}",
    "->:${minus}${greater_than}",
    "++:${plus}${plus}",
    "--:${minus}${minus}",
    "ampersand:${ampersand}",
    "*:${asterisk}",
    "+:${plus}",
    "-:${minus}",
    "~:${tilde}",
    "!:${exclamation}",
    "/:${forward_slash}",
    "%:${percent}",
    "<<:${less_than}${less_than}",
    ">>:${greater_than}${greater_than}",
    "<:${less_than}",
    ">:${greater_than}",
    "<=:${less_than}${equal}",
    ">=:${greater_than}${equal}",
    "==:${equal}${equal}",
    "!=:${exclamation}${equal}",
    "^:${caret}",
    "|:${vertical_bar}",
    "double_ampersand:${ampersand}${ampersand}",
    "||:${vertical_bar}${vertical_bar}",
    "\?:${question}",
    "colon:${colon}",
    ";:${semi_colon}",
    "...:${dot}${dot}${dot}",
    "=:${equal}",
    "*=:${asterisk}${equal}",
    "/=:${forward_slash}${equal}",
    "%=:${percent}${equal}",
    "+=:${plus}${equal}",
    "-=:${minus}${equal}",
    "<<=:${less_than}${less_than}${equal}",
    ">>=:${greater_than}${greater_than}${equal}",
    "ampersand=:${ampersand}${equal}",
    "^=:${caret}${equal}",
    "|=:${vertical_bar}${equal}",
    ",:${comma}",
    "#:${number_sign}",
    "##:${number_sign}${number_sign}",
    "<colon:${less_than}${colon}",
    "colon>:${colon}${greater_than}",
    "<%:${less_than}${percent}",
    "%>:${percent}${greater_than}",
    "%colon:${percent}${colon}",
    "double_%colon:${percent}${colon}${percent}${colon}"
};

const char parsers[MAX_RULE_NUM][RULE_CHAR_LEN]={
    /*main translate unit*/
    "translation_unit:&{external_decl}&{&external_decl_opt}",
    "&external_decl_opt:&{external_decl}*",
    "external_decl:&{function_definition}|&{declaration}",
    "function_definition:&{&declaration_spec}&{&declarator}&{&declaration_list_opt}&{compound_stmt}",
    "&declaration_list_opt:&{&-declaration_list}\?",
    "&-declaration_list:&{declaration}&{&-declaration_list_opt}",
    "&-declaration_list_opt:&{declaration}*",
    /* three main kinds */
    "expression:&{&assignment_expr}&{&-expression_opt}",
    
    "declaration:&{&-declaration}|&{&static_assert-declaration}",

    "statement:&{labeled_stmt}|&{compound_stmt}|&{expr_stmt}|&{selection_stmt}|&{iteration_stmt}|&{jump_stmt}|&{asm_stmt}",
    /*expressions*/
    "&-expression_opt:&{&-expression}*",
    "&-expression:${,}&{&assignment_expr}",
    
    "&assignment_expr:&{&conditional_expr}|&{&-assignment_expr}",
    "&-assignment_expr:&{&unary_expr}&{&assignment_operator}&{&assignment_expr}",
    
    "&conditional_expr:&{&logical_or_expr}|&{&-conditional_expr}",
    "&-conditional_expr:&{&logical_or_expr}${\?}&{expression}${colon}&{&conditional_expr}",
    
    "&logical_or_expr:&{&logical_and_expr}&{&-logical_or_expr_opt}",
    "&-logical_or_expr_opt:&{&-logical_or_expr}*",
    "&-logical_or_expr:${||}&{&logical_and_expr}",
    
    "&logical_and_expr:&{&inclusive_or_expr}&{&-logical_and_expr_opt}",
    "&-logical_and_expr_opt:&{&-logical_and_expr}*",
    "&-logical_and_expr:${double_ampersand}&{&inclusive_or_expr}",
    
    "&inclusive_or_expr:&{&exclusive_or_expr}&{&-inclusive_or_expr_opt}",
    "&-inclusive_or_expr_opt:&{&-inclusive_or_expr}*",
    "&-inclusive_or_expr:${|}&{&exclusive_or_expr}",
    
    "&exclusive_or_expr:&{&and_expression}&{&-exclusive_or_expr_opt}",
    "&-exclusive_or_expr_opt:&{&-exclusive_or_expr}*",
    "&-exclusive_or_expr:${^}&{&and_expression}",
    
    "&and_expression:&{&equality_expr}&{&-and_expression_opt}",
    "&-and_expression_opt:&{&-and_expression}*",
    "&-and_expression:${ampersand}&{&equality_expr}",
    
    "&equality_expr:&{&relational_expr}&{&-equality_expr_opt}",
    "&-equality_expr_opt:&{&-equality_expr}*",
    "&-equality_expr:&{&equality_operator}&{&relational_expr}",
    
    "&relational_expr:&{&shift_expr}&{&-relational_expr_opt}",
    "&-relational_expr_opt:&{&-relational_expr}*",
    "&-relational_expr:&{&relational_operator}&{&shift_expr}",
    
    "&shift_expr:&{&additive_expr}&{&-shift_expr_opt}",
    "&-shift_expr_opt:&{&-shift_expr}*",
    "&-shift_expr:&{&shift_operator}&{&additive_expr}",

    "&additive_expr:&{&multi_expr}&{&-additive_expr_opt}",
    "&-additive_expr_opt:&{&-additive_expr}*",
    "&-additive_expr:&{&additive_operator}&{&multi_expr}",
    
    "&multi_expr:&{&cast_expr}&{&-multi_expr_opt}",
    "&-multi_expr_opt:&{&-multi_expr}*",
    "&-multi_expr:&{&multi_operator}&{&cast_expr}",

    "&cast_expr:&{&-cast_expr}|&{&unary_expr}",
    "&-cast_expr:&{&(type_name)}&{&cast_expr}",
    
    "&unary_expr:&{&postfix_expr}|&{&unary_expr1}|&{&unary_expr2}|&{&unary_expr3}",
    "&unary_expr1:&{&unary_op1}&{&unary_expr}",
    "&unary_expr2:&{&unary_operator}&{&cast_expr}",
    "&unary_expr3:&{&unary_op2}&{&(type_name)}",

    "&postfix_expr:&{&postfix_expr_postfix}&{&-postfix_expr_opt}",
    "&postfix_expr_postfix:&{&primary_expression}|&{&-postfix_expr_postfix}",

    "&-postfix_expr_opt:&{&-postfix_expr}*",
    
    "&-postfix_expr:&{&postfix_expr1}|&{&postfix_expr2}|&{&postfix_expr3}|&{&postfix_expr4}",
    "&postfix_expr1:${[}&{expression}${]}",
    "&postfix_expr2:${(}&{&arg_expression_list_opt}${)}",
    "&postfix_expr3:&{&postfix_op1}${identifier}",
    "&postfix_expr4:&{&postfix_op2}",
    
    "&-postfix_expr_postfix:&{&(type_name)}${left_brace}&{&initializer_list}&{&,opt}${right_brace}",
    "&,opt:${,}\?",
    "&arg_expression_list_opt:&{&arg_expression_list}\?",
    "&arg_expression_list:&{&assignment_expr}&{&-arg_expression_list_opt}",
    "&-arg_expression_list_opt:&{&-arg_expression_list}*",
    "&-arg_expression_list:${,}&{&assignment_expr}",

    "&primary_expression:${identifier}|${integer_constant}|${floating_constant}|&{&enum_const}|${char_const}|${string}|&{&-primary_expression}|&{&generic_selection}",
    "&-primary_expression:${(}&{expression}${)}",
    "&generic_selection:${KW__Generic}${(}&{&assignment_expr}${,}&{&generic_assoc_list}${)}",
    "&generic_assoc_list:&{&generic_association}|&{&-generic_assoc_list}",
    "&-generic_assoc_list:&{&generic_assoc_list}${,}&{&generic_association}",
    "&generic_association:&{&generic_asso_head}${colon}&{&assignment_expr}",

    "&assignment_operator:${=}|${*=}|${/=}|${%=}|${+=}|${-=}|${<<=}|${>>=}|${ampersand=}|${^=}|${|=}",
    "&equality_operator:${==}|${!=}",
    "&relational_operator:${<}|${>}|${<=}|${>=}",
    "&shift_operator:${<<}|${>>}",
    "&additive_operator:${+}|${-}",
    "&multi_operator:${*}|${/}|${%}",
    "&(type_name):${(}&{&type_name}${)}",
    "&unary_op1:${++}|${--}|${KW_sizeof}",
    "&unary_op2:${KW_sizeof}|${KW__Alignof}",
    "&unary_operator:${ampersand}|${*}|${+}|${-}|${~}|${!}",
    "&postfix_op1:${.}|${->}",
    "&postfix_op2:${++}|${--}",
    "&generic_asso_head:&{&type_name}|${KW_default}",
    "&constant_expr:&{&conditional_expr}",
    /*declarations*/
    "&-declaration:&{&declaration_spec}&{&init_decl_list_opt}${;}",
    "&init_decl_list_opt:&{&init_decl_list}\?",
    
    "&declaration_spec:&{&decl_spec_postfix}&{&declaration_spec_opt}",
    "&declaration_spec_opt:&{&decl_spec_postfix}*",
    "&decl_spec_postfix:&{&storage_class_spec}|&{&type_spec}|&{&type_qual}|&{&function_spec}|&{&alignment_spec}",
    
    "&init_decl_list:&{&init_decl}&{&-init_decl_list_opt}",
    "&-init_decl_list_opt:&{&-init_decl_list}*",
    "&-init_decl_list:${,}&{&init_decl}",
    "&init_decl:&{&declarator}&{&init_decl_opt}",
    "&init_decl_opt:&{&-init_decl_opt}\?",
    "&-init_decl_opt:${=}&{&initializer}",

    "&storage_class_spec:${KW_typedef}|${KW_extern}|${KW_static}|${KW__Thread_local}|${KW_auto}|${KW_register}",
    
    "&type_spec:${KW_void}|${KW_char}|${KW_short}|${KW_int}|${KW_long}|${KW_float}|${KW_double}|${KW_signed}|${KW_unsigned}|${KW__Bool}|${KW__Complex}|&{&atomic_type_spec}|&{&struct_union_spec}|&{&enum_spec}|&{&typedef_name}",
    "&atomic_type_spec:${KW__Atomic}&{&(type_name)}",
    
    "&struct_union_spec:&{&struct_union}&{&-struct_union_spec}",
    "&-struct_union_spec:&{&struct_union_spec1}|${identifier}",
    "&struct_union_spec1:&{&identifier_opt}${left_brace}&{&struct_decl_list}${right_brace}",
    
    "&struct_union:${KW_struct}|${KW_union}",
    
    "&identifier_opt:${identifier}\?",
    
    "&struct_decl_list:&{&struct_decl}&{&-struct_decl_list}",
    "&-struct_decl_list:&{&struct_decl}*",
    
    "&struct_decl:&{&-struct_decl}|&{&static_assert-declaration}",
    "&-struct_decl:&{&spec_qual_list}&{&struct_declarator_list_opt}${;}",
    
    "&spec_qual_list:&{&-spec_qual_list}&{&spec_qual_list_opt}",
    "&-spec_qual_list:&{&type_spec}|&{&type_qual}",
    "&spec_qual_list_opt:&{&-spec_qual_list}*",
    
    "&struct_declarator_list_opt:&{&struct_declarator_list}\?",
    "&struct_declarator_list:&{&struct_declarator}&{&-struct_declarator_list_opt}",
    "&-struct_declarator_list_opt:&{&-struct_declarator_list}*",
    "&-struct_declarator_list:${,}&{&struct_declarator}",
    "&struct_declarator:&{&declarator}|&{&-struct_declarator}",
    "&-struct_declarator:&{&declarator_opt}${colon}&{&constant_expr}",
    "&declarator_opt:&{&declarator}\?",

    "&enum_spec:${KW_enum}&{&-enum_spec}",
    "&-enum_spec:${identifier}|&{&enum_spec_suffix}",
    "&enum_spec_suffix:&{&identifier_opt}${left_brace}&{&enum_list}&{&,opt}${right_brace}",
    "&enum_list:&{&enumerator}&{&enumerator_opt}",
    "&enumerator_opt:&{&-enumerator_opt}*",
    "&-enumerator_opt:${,}&{&enumerator}",
    "&enumerator:&{&enum_const}&{&enum_init_opt}",
    "&enum_init_opt:&{&enum_init}\?",
    "&enum_init:${=}&{&constant_expr}",
    "&enum_const:${identifier}",
    
    "&type_qual:${KW_const}|${KW_restrict}|${KW_volatile}|${KW__Atomic}",
    
    "&function_spec:${KW_inline}|${KW__Noreturn}",
    
    "&alignment_spec:${KW__Alignas}${(}&{&-alignment_spec}${)}",
    "&-alignment_spec:&{&type_name}|&{&constant_expr}",
    
    "&declarator:&{&pointer_opt}&{&direct_declarator}",

    "&pointer_opt:&{&pointer}*",
    "&pointer:${*}&{&type_qual_list_opt}",
    
    "&direct_declarator:&{&d_declarator_postfix}&{&d_declarator_suffix_opt}",
    "&d_declarator_postfix:${identifier}|&{&-d_declarator_postfix}",
    "&-d_declarator_postfix:${(}&{&declarator}${)}",
    
    "&d_declarator_suffix_opt:&{&d_declarator_suffix}*",
    "&d_declarator_suffix:&{&d_declarator_suffix1}|&{&d_declarator_suffix2}|&{&d_declarator_suffix3}|&{&d_declarator_suffix4}|&{&d_declarator_suffix5}|&{&d_declarator_suffix6}",
    "&d_declarator_suffix1:${[}&{&type_qual_list_opt}&{&assignment_expr_opt}${]}",
    "&d_declarator_suffix2:${[}${KW_static}&{&type_qual_list_opt}&{&assignment_expr}${]}",
    "&d_declarator_suffix3:${[}&{&type_qual_list}${KW_static}&{&assignment_expr}${]}",
    "&d_declarator_suffix4:${[}&{&type_qual_list_opt}${*}${]}",
    "&d_declarator_suffix5:${(}&{&parameter_type_list}${)}",
    "&d_declarator_suffix6:${(}&{&identifier_list_opt}${)}",

    "&assignment_expr_opt:&{&assignment_expr}\?",

    "&type_qual_list_opt:&{&type_qual_list}\?",
    "&type_qual_list:&{&type_qual}&{&-type_qual_list}",
    "&-type_qual_list:&{&type_qual}*",

    "&parameter_type_list:&{&parameter_list}&{&parameter_type_list_suffix}",
    "&parameter_type_list_suffix:&{&-parameter_type_list_suffix}\?",
    "&-parameter_type_list_suffix:${,}${...}",

    "&parameter_list:&{&parameter_declaration}&{&-parameter_list_opt}",
    "&-parameter_list_opt:&{&-parameter_list}*",
    "&-parameter_list:${,}&{&parameter_declaration}",

    "&parameter_declaration:&{&declaration_spec}&{&parameter_decl_suffix}",
    "&parameter_decl_suffix:&{&declarator}|&{&abstract_decl_opt}",
    "&abstract_decl_opt:&{&abstract_declarator}\?",

    "&identifier_list_opt:&{&identifier_list}\?",
    "&identifier_list:${identifier}&{&identifier_list_suffix}",
    "&identifier_list_suffix:&{&-identifier_list_suffix}*",
    "&-identifier_list_suffix:${,}${identifier}",

    "&type_name:&{&spec_qual_list}&{&abstract_decl_opt}",
    "&abstract_declarator:&{&pointer}|&{&-abstract_declarator}",
    "&-abstract_declarator:&{&pointer_opt}&{&direct_abstract_declarator}",
        /* I rewrite the following rules */
    "&direct_abstract_declarator:&{&direct_abs_declarator_postfix_opt}&{&direct_abs_declarator_suffix}",
    "&direct_abs_declarator_postfix_opt:&{&direct_abs_declarator_postfix}\?",
    "&direct_abs_declarator_suffix_opt:&{&direct_abs_declarator_suffix}*",
    "&direct_abs_declarator_postfix:${(}&{&abstract_declarator}${)}",
    "&direct_abs_declarator_suffix:&{&direct_abs_declarator_suffix1}|&{&direct_abs_declarator_suffix2}|&{&direct_abs_declarator_suffix3}|&{&direct_abs_declarator_suffix4}|&{&direct_abs_declarator_suffix5}",
    "&direct_abs_declarator_suffix1:${[}&{&type_qual_list_opt}&{&assignment_expr_opt}${]}",
    "&direct_abs_declarator_suffix2:${[}${KW_static}&{&type_qual_list_opt}&{&assignment_expr}${]}",
    "&direct_abs_declarator_suffix3:${[}&{&type_qual_list}${KW_static}&{&assignment_expr}${]}",
    "&direct_abs_declarator_suffix4:${[}${*}${]}",
    "&direct_abs_declarator_suffix5:${(}&{&type_qual_list_opt}${)}",

    "&typedef_name:${identifier}",

    "&initializer:&{&assignment_expr}|&{&-initializer}",
    "&-initializer:${left_brace}&{&initializer_list}&{&,opt}${right_brace}",
    "&initializer_list:&{&-initializer_list}&{&-initializer_list_opt}",
    "&-initializer_list_opt:&{&,-initializer_list}*",
    "&,-initializer_list:${,}&{&-initializer_list}",
    "&-initializer_list:&{&designation_opt}&{&initializer}",

    "&designation_opt:&{&designation}\?",
    "&designation:&{&designator_list}${=}",

    "&designator_list:&{&designator}&{&designator_opt}",
    "&designator_opt:&{&designator}*",

    "&designator:&{&designator1}|&{&designator2}",
    "&designator1:${[}&{&constant_expr}${]}",
    "&designator2:${.}${identifier}",

    "&static_assert-declaration:${KW__Static_assert}${(}&{&constant_expr}${,}${string}${)}",
    /* statements */
    "labeled_stmt:&{&labeled_id_stmt}|&{&labeled_case_stmt}|&{&labeled_default_stmt}",
    "&labeled_id_stmt:${identifier}${colon}&{statement}",
    "&labeled_case_stmt:${KW_case}&{&constant_expr}${colon}&{statement}",
    "&labeled_default_stmt:${KW_default}${colon}&{statement}",

    "compound_stmt:${left_brace}&{&block_item_list_opt}${right_brace}",
    "&block_item_list_opt:&{&block_item_list}\?",
    "&block_item_list:&{&block_item}&{&-block_item_list}",
    "&-block_item_list:&{&block_item}*",
    "&block_item:&{declaration}|&{statement}",

    "expr_stmt:&{&expression_opt}${;}",
    "&expression_opt:&{expression}\?",
    
    "selection_stmt:&{&if_stmt}|&{&switch_stmt}",
    "&if_stmt:${KW_if}${(}&{expression}${)}&{statement}&{&else_stmt_opt}",
    "&else_stmt_opt:&{&else_stmt}\?",
    "&else_stmt:${KW_else}&{statement}",
    "&switch_stmt:${KW_switch}${(}&{expression}${)}&{statement}",
    
    "iteration_stmt:&{&while_stmt}|&{&do_stmt}|&{&for_stmt}",
    "&while_stmt:${KW_while}${(}&{expression}${)}&{statement}",
    "&do_stmt:${KW_do}&{statement}${(}&{expression}${)}${;}",
    "&for_stmt:${KW_for}${(}&{&for_expr}&{&expression_opt}${;}&{&expression_opt}${)}&{statement}",
    "&for_expr:&{declaration}|&{&-for_expr}",
    "&-for_expr:&{&expression_opt}${;}",
    
    "jump_stmt:&{&goto_stmt}|&{&continue_stmt}|&{&break_stmt}|&{&return_stmt}",
    "&goto_stmt:${KW_goto}${identifier}${;}",
    "&continue_stmt:${KW_continue}${;}",
    "&break_stmt:${KW_break}${;}",
    "&return_stmt:${KW_return}&{&expression_opt}${;}",
    /*Asm*/
    "asm_stmt:${KW_asm}&{&KW_volatile_opt}${(}${string}&{&ASM_extend_opt}${)}${;}",
    "&KW_volatile_opt:${KW_volatile}\?",
    "&ASM_extend_opt:&{&ASM_extend}\?",
    "&ASM_extend:${colon}&{&ASM_output_opt}&{&ASM_extend_postfix_opt}",
    "&ASM_extend_postfix_opt:&{&ASM_extend_postfix}\?",
    "&ASM_extend_postfix:${colon}&{&ASM_input_opt}&{&ASM_register_postfix_opt}",
    "&ASM_register_postfix_opt:&{&ASM_register_postfix}\?",
    "&ASM_register_postfix:${colon}&{&ASM_register_opt}",

    "&ASM_output_opt:&{&ASM_output}\?",
    "&ASM_output:&{&ASM_restrict}&{&ASM_io_postfix_opt}",
    "&ASM_input_opt:&{&ASM_input}\?",
    "&ASM_input:&{&ASM_restrict}&{&ASM_io_postfix_opt}",
    "&ASM_register_opt:&{&ASM_register}\?",
    "&ASM_register:${string}&{&ASM_reg_postfix_opt}",

    "&ASM_restrict:${string}${(}${identifier}${)}",
    "&ASM_io_postfix_opt:&{&ASM_io_postfix}*",
    "&ASM_io_postfix:${,}&{&ASM_restrict}",
    "&ASM_reg_postfix_opt:&{&ASM_reg_postfix}*",
    "&ASM_reg_postfix:${,}${string}"
};