
typedef long unsigned int size_t;

typedef long int __off64_t;
typedef long int __off_t;

typedef struct
{
  int __count;
  union
  {
    unsigned int __wch;
    
  } __value;
} __mbstate_t;

typedef struct _G_fpos_t
{
  __off_t __pos;
  __mbstate_t __state;
} __fpos_t;
typedef __fpos_t fpos_t;

typedef struct _G_fpos64_t
{
  __off64_t __pos;
  __mbstate_t __state;
} __fpos64_t;

struct _IO_marker;
struct _IO_FILE;
typedef struct _IO_FILE FILE;
typedef void _IO_lock_t;
struct _IO_FILE
{
  int _flags;


  char *_IO_read_ptr;
  char *_IO_read_end;
  char *_IO_read_base;
  char *_IO_write_base;
  char *_IO_write_ptr;
  char *_IO_write_end;
  char *_IO_buf_base;
  char *_IO_buf_end;


  char *_IO_save_base;
  char *_IO_backup_base;
  char *_IO_save_end;

  struct _IO_marker *_markers;

  struct _IO_FILE *_chain;

  int _fileno;
  int _flags2;
  __off_t _old_offset;

  unsigned short _cur_column;
  signed char _vtable_offset;
  

  _IO_lock_t *_lock;

  __off64_t _offset;

  struct _IO_codecvt *_codecvt;
  struct _IO_wide_data *_wide_data;
  struct _IO_FILE *_freeres_list;
  void *_freeres_buf;
  size_t __pad5;
  int _mode;

  
};
extern int remove(const char *filename);

extern int rename(const char *old, const char *new);

extern FILE *tmpfile(void);

extern char *tmpnam(char *s);

extern int fclose(FILE *stream);

extern int fflush(FILE *stream);

extern FILE *fopen(const char * restrict filename,
    const char * restrict mode);

extern FILE *freopen(const char * restrict filename,
    const char * restrict mode,
    FILE * restrict stream);

extern void setbuf(FILE * restrict stream,
    char * restrict buf);

extern int setvbuf(FILE * restrict stream,
    char * restrict buf,
    int mode, size_t size);

extern int fprintf(FILE * restrict stream,
    const char * restrict format, ...);

extern int fscanf(FILE * restrict stream,
    const char * restrict format, ...);

extern int printf(const char * restrict format, ...);

extern int scanf(const char * restrict format, ...);

extern int snprintf(char * restrict s, size_t n,
    const char * restrict format, ...);

extern int sprintf(char * restrict s,
    const char * restrict format, ...);

extern int sscanf(const char * restrict s,
    const char * restrict format, ...);

extern char *fgets(char * restrict s, int n,
    FILE * restrict stream);

extern int fputc(int c, FILE *stream);

extern int fputs(const char * restrict s,
    FILE * restrict stream);

extern int getc(FILE *stream);

extern int getchar(void);

extern int putc(int c, FILE *stream);

extern int putchar(int c);

extern int puts(const char *s);

extern int ungetc(int c, FILE *stream);

extern size_t fread(void * restrict ptr,
    size_t size, size_t nmemb,
    FILE * restrict stream);

extern size_t fwrite(const void * restrict ptr,
    size_t size, size_t nmemb,
    FILE * restrict stream);

extern int fgetpos(FILE * restrict stream,
    fpos_t * restrict pos);

extern int fseek(FILE *stream, long int offset, int whence);

extern int fsetpos(FILE *stream, const fpos_t *pos);

extern long int ftell(FILE *stream);

extern void rewind(FILE *stream);

extern void clearerr(FILE *stream);

extern int feof(FILE *stream);

extern int ferror(FILE *stream);

extern void perror(const char *s);

extern int fprintf_s(FILE * restrict stream,
    const char * restrict format, ...);

extern int fscanf_s(FILE * restrict stream,
    const char * restrict format, ...);

extern int printf_s(const char * restrict format, ...);

extern int scanf_s(const char * restrict format, ...);

extern int sscanf_s(const char * restrict s,
    const char * restrict format, ...);
typedef long unsigned int size_t;

extern void *memset(void *s, int c, size_t n);

extern int strcmp(const char *s1, const char *s2);

extern void *memcpy(void * restrict s1,
    const void * restrict s2, size_t n);
typedef long unsigned int size_t;

extern void *malloc(size_t size);

extern void free(void *ptr);

extern _Noreturn void exit(int status);
enum compiler_error{
    ERR_NONE,ERR_UNKNOWN,

    ERR_LEX_ERROR,

    ERR_NAMESPACE,ERR_REDEFINE,ERR_NOT_IN_SYMBOL_TABLE,
    ERR_UNEXPECTED_TOKEN

};
typedef unsigned int LINE_NUM;
typedef struct line{
    LINE_NUM line_number;
    size_t start;
    size_t end;
}LINE;
typedef struct mcc_error_item{
    enum compiler_error ce;
    char* filename;
    LINE_NUM error_line;
    char* error_token_value;
}ERROR_ITEM;

void m_error(char filename[],char function[],char reason[]);
void m_error_arg_char(char* arg);
void m_error_arg_int(int arg);

void m_test(char test[]);
void m_test_item(char test_item_name[]);
void m_test_arg_char(char* item_arg);
void m_test_arg_int(int item_arg);

void mcc_error(ERROR_ITEM* e,char* file_buf,LINE* line);

typedef struct malloc_node M_NODE;
struct malloc_node{
    void* p;
    M_NODE* next;
};

typedef struct mem_item{
    unsigned int next;
    unsigned int size;
}MEM_ITEM;

typedef struct mem_block MEM_BLOCK;
struct mem_block{
    MEM_BLOCK* next;
    unsigned int free_item_header;
    void* data;
};

typedef struct mem_pool{
    M_NODE* sys_alloc;
}MEM_POOL;

inline size_t MEM_GET_SIZE(size_t n){
    if(n<=0)
        return 0;
    if(n<=4)
        return 2;
    if(n>1024)
        return 11;
    int nlevel=0;
    while(n>0){
        n>>=1;
        nlevel++;
    }
    return nlevel;
}
MEM_POOL* MEMInit(char name[]);
MEM_BLOCK* MEMInitBlock(size_t chain_num);
void MEMDelBlock(MEM_BLOCK* block);
void MEMDel(MEM_POOL* pool);
void TestMem();

void* m_alloc(size_t n);
void m_free(void* p);

typedef struct vector{
    void** data;
    size_t len;
    size_t capicity;
}VEC;




VEC* InitVEC(size_t len);
void VECinsert(VEC* v,void* p);
void VECremove(VEC* v,size_t low,size_t high);
void VECpopback(VEC* v);
void VECexpand(VEC* v);
void VECshrink(VEC* v);
void DelVEC(VEC* v);

VEC* InitVEC(size_t len)
{
    VEC* v=m_alloc(sizeof(VEC));
    v->data=m_alloc(len*sizeof(void*));
    v->capicity=3;
    v->len=0;
    return v;
}
void VECinsert(VEC* v,void* p)
{
    if(v==0) return;
    VECexpand(v);
    v->data[(v->len)]=p;
    (v->len)++;
}
void VECremove(VEC* v,size_t low,size_t high)
{
    if(v==0) return;
    if(low==high) return;
    while(high<(v->len)) v->data[low++]=v->data[high++];
    (v->len)=low;
    VECshrink(v);
}
void VECpopback(VEC* v){
    if(v==0) return;
    if((v->len)==0) return;
    (v->len)--;
}
void VECexpand(VEC* v)
{
    if(v==0) return;
    if((v->len)<v->capicity) return;
    if(v->capicity<3) v->capicity=3;
    void* old=v->data;
    v->data=m_alloc((v->capicity<<1)*sizeof(void*));
    memcpy(v->data,old,(v->len)*sizeof(void*));
    v->capicity=v->capicity<<1;
    m_free(old);
}
void VECshrink(VEC* v)
{
    if(v==0) return;
    if(v->capicity<3<<1) return;
    if((v->len)<<2>v->capicity) return;
    void* old=v->data;
    v->data=m_alloc((v->capicity>>1)*sizeof(void*));
    memcpy(v->data,old,(v->len)*sizeof(void*));
    v->capicity=v->capicity>>1;
    m_free(old);
}
void DelVEC(VEC* v)
{
    if(v==0) return;
    VECremove(v,0,(v->len));
    m_free(v);
}
