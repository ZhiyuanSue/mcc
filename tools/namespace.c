#include "namespace.h"
unsigned int max_namespace_id=2;
/*
0-unknown
1-default
2-struct union and enum tag
4 to end-new
*/
unsigned int GET_NEW_NAMESPACE(void)
{
    max_namespace_id++;
    return max_namespace_id;
}
void RESET_NAMESPACE(void)
{
    max_namespace_id=2;
}