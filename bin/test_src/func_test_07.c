int a(void){
    return 1;
}
int b(void){
    return 2;
}
int (*f[2])(void)={a,b};
int (*f)(void)=a;
struct fs{
    int (*c)(void);
    int d;
    _Bool e:1;
    int f:12;
} tmpfs={a,2,1,33};
int cdef=1234;