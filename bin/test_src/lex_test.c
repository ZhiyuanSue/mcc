int main(void)
{
    int _auto=\u2345;
    /*test integer constant*/
    int x1=1256;
    unsigned int x2=345u;
    unsigned long int x3=234ul;
    long long int x4=145ll;
    long long int x5=145LL;
    unsigned long long int x6=456ull;
    unsigned long long int x7=456ullull;    //error
    int hex_1=0xAF;
    int hex_2=0XBE;
    int hex_3=0x;
    int oct_1=0123;
    int oct_2=0FF;  //error
    double d1=000.E000;
    double d2=000.f;
    auto _int=_\U34ab45ef;
    char a0="";
    char a00='';
    char a1='"';
    char a11='';
    char a2="'";
    char a3='\xa';
    printf("this is test file\n");
    return 0;
}