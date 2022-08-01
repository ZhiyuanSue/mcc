/*
This is part of C11 standard library
and I copy and change some place from GCC's library
*/
#ifndef _STDIO_H_
#define _STDIO_H_
#define SEEK_SET        0       /* Seek from beginning of file.  */
#define SEEK_CUR        1       /* Seek from current position.  */
#define SEEK_END        2       /* Seek from end of file.  */
typedef long unsigned int size_t;

typedef long int __off64_t;
typedef long int __off_t;

typedef struct
{
  int __count;
  union
  {
    unsigned int __wch;
    char __wchb[4];
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
  char _shortbuf[1];

  _IO_lock_t *_lock;

  __off64_t _offset;

  struct _IO_codecvt *_codecvt;
  struct _IO_wide_data *_wide_data;
  struct _IO_FILE *_freeres_list;
  void *_freeres_buf;
  size_t __pad5;
  int _mode;

  char _unused2[15 * sizeof (int) - 4 * sizeof (void *) - sizeof (size_t)];
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
#ifndef EOF 
#define EOF (-1)
#endif
#endif