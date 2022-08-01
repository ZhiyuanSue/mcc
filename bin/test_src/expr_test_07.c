int test(void){
    int a;
    sizeof a;
    struct bbb{
        int ddd;
    };
    sizeof(struct bbb);
    int ccc[a];
    sizeof (ccc);
    _Alignof(struct bbb);
    (*(int*)(&a));
    return 0;
}