struct t{
    char a;
    double b;
};
struct struct_offset_test{
    char name;
    int size;
    char* name_p;
    union {
        struct{
            int ta;
            char tb;
        };
        double dl;
    };
    struct a{
        char x;
        int y;
        double z;
    }a1;
    struct t tt;
};