enum test {QWERT,REWQT};
struct test_bit_field_offset{
    char a:7;
    char b:6;
    int c:14;
    enum test TTT;
    char :7;
    char d:4;
    int e;
    char f:2;
    char :0;
    char g:2;
    enum test TT:3;
    char h[2];
} tbfo;