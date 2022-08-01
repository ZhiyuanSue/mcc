#ifndef _LEX_RULE_H_
#define _LEX_RULE_H_
#define MAX_RULE_NUM 1024
#define RULE_CHAR_LEN 256

/*
    maybe you will say ,the string of rule must be a 'static const char'
    and at that case ,I needn't add a .c file to store the rule strings
    all the declaration of rule should put in this .h file
    but I tried it
    I find if i set some 'static' strings, the speed of gcc would be much much slower than this way
    I don't known why, but trust me ,don't use 'static' keyword on a testing project
    unless you have finished all the work
*/
#endif