int abcd[]={1,2};
int *p[2]={&abcd,0};
void test(void){
    int efgh=1;
    static int *ppp[2]={&efgh,0};
}