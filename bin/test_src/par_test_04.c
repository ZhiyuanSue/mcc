_Thread_local struct c;
_Thread_local int tl=0x1234567;
_Thread_local int tb=0x654321;
struct c{
    int cc;
};
struct stest{
    int y;
    int c;
    struct sstest{
        int e;
        int f;
    }t;
};
typedef struct stest ccc;
struct b{
    int ccc;
};
ccc cc;
enum scope_type{
    SPT_NONE=0,SPT_BLOCK,SPT_FUN=5,SPT_FILE,SPT_FUN_PROTOTYPE
};
void g(signed long int ccc);
void g(signed long int ccc){
    extern _Thread_local struct thread_c;
    ccc++;
    int b=ccc,z=2.0;
    int *p;
    int x[20][30];
    
    char* a="a is a bool";
    b++;
    b+=SPT_FILE;
    b+=tl;
    if(0){
        int a=1;
        goto label_test;
    }
    else{
        return;
    }
    switch (b){
    case 1:
        break;
    case 2:
        break;
    default:
        break;
    }
    
label_test:
    return;
}
void k(void){
    ccc gg;
    signed long int ccc;
    g(ccc);
}