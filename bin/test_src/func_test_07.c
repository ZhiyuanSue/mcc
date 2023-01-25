int a(void){
    return 1;
}
int b(void){
    return 2;
}
int (*f[2])(void)={a,b};
struct fs{
    int (*c)(void);
    int d;
    int f:12;
} tmpfs={a,1,33};