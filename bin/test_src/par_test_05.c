int main(void){
    int a,b,c;
    __asm__ ("a");
    __asm__ volatile ("b");
    __asm__ volatile (
        "string"
        :"=a"(a)
    );
    __asm__ volatile (
        "string"
        :"=a"(a)
        :"b"(b)
    );
    __asm__ volatile (
        "string"
        :"=a"(a)
        :"b"(b)
        :"c"
    );
    __asm__ volatile (
        "string"
        :
        :
        :"c"
    );
    __asm__ volatile (
        "string"
        :"=a"(a),"=b"(b),"=c"(c)
        :
        :"d"
    );
    __asm__ volatile (
        "string"
        :"=a"(a),"=b"(b),"=c"(c)
        :"a"(a),"b"(b),"c"(c)
        :"a","b","c"
    );
    return 0;
}