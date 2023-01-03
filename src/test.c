/*This file is just to test whether some cases can be successfully complied by gcc*/
#include "test.h"
#include "../defs/defs.h"
void test(void)
{
    struct{
        int a[3],b;
    } w[]={ {1},2,3,4,{5,6},[3]=7,8 };
    printf("%d\n",w[3].a[0]);
    printf("%d\n",w[3].a[1]);
}