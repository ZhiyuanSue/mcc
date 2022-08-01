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
int testa(void){
  struct _G_fpos_t{
    char* name;
  };
}