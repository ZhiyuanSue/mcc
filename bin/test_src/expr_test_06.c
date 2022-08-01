int ttt(void)
{
    struct aaa{
        int c:3;
    };
    int a=sizeof((struct aaa).a);
    return 0;
}