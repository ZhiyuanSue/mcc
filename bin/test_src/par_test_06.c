int main(int argc, char * argv[]) {
    _Atomic int hoge;
    int piyo = atomic_load(&hoge);  
    printf("piyo = %d.\n", piyo);
    piyo += 2;
    atomic_store(&hoge, piyo);
    printf("hoge = %d.\n", hoge);

    return 0;
}