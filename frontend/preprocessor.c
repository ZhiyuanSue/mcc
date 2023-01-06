#include "preprocessor.h"
char* prepro(char filename[]){
    return load_file(filename);
}
char* load_file(char filename[])
{
#ifdef _TEST_
    m_test_item("read in file name");
    m_test_arg_char(filename);
#endif
    FILE* fp;
    size_t file_size,buf_size;
    if((fp=fopen(filename,"rb"))==NULL){
        m_error("main.c","load_file","open file error");
        exit(1);
    }
    fseek(fp,0,SEEK_END);
    file_size=ftell(fp);
    fseek(fp,0,SEEK_SET);
    int tmp=0;
    int tmpsize=file_size;
    while(tmpsize>0){
        tmpsize>>=1;
        tmp++;
    }
    buf_size=1<<(tmp+1);

    char* buf=(char*)m_alloc(buf_size);
    m_memset(buf,'\0',buf_size);
    
    int readlen=fread(buf,1,file_size,fp);
    if(readlen!=file_size){
        printf("read in file error,read in %d byte.\n",readlen);
        exit(1);
    }
    fclose(fp);
    bool need_clear_line=false;
    char* tmpbuf=buf;
    while(*tmpbuf!='\0'){   /*clear the gcc preprocessor's format*/
        if((*tmpbuf)=='#')
        {
            if(buf==tmpbuf||is_end_of_line(tmpbuf-1)) /*The start of the file*/
            {
                need_clear_line=true;
            }
        }
        if(is_end_of_line(tmpbuf))
        {
            need_clear_line=false;
        }
        if(need_clear_line)
            (*tmpbuf)=' ';
        tmpbuf++;
    }
#if 1
    printf("%s\n",buf);
#endif
    return buf;
}