#include "backend.h"
bool MCC_backend(IR_MODULE* irm,char* src_file,char* target_file)
{
    if(!irm)
        goto error;
    if(!target_file)
    {
        printf("need a correct target file name\n");
        goto error;
    }
    FILE* fp;
    if((fp=fopen(target_file,"w"))==NULL){
        m_error("main.c","MCC_backend","open or create target file error");
        exit(1);
    }
    /*first output the src file name to the target file*/
    size_t src_file_name_len=strlen(src_file);
    char* src_file_name=m_alloc(sizeof(char)*src_file_name_len);
    memset(src_file_name,'\0',src_file_name_len);
#ifdef _MAC_        /*different platform have different path seperate*/
    char split_ch='/';
#else
#ifdef _UNIX_
    char split_ch='/';
#else
#ifdef _WIN_
    char split_ch='\\';
#endif
#endif
#endif
    size_t index;
    for(index=src_file_name_len;index>0;index--)
    {
        if(src_file[index]==split_ch)
            break;
    }
    index+=1;
    if(index==1&&src_file[0]!=split_ch)
        index=0;
    for(size_t i=0;i+index<src_file_name_len;++i)
        src_file_name[i]=src_file[i+index];
    fprintf(fp,"\t.file\t\"%s\"\n",src_file_name);
    if(!gen_asm(irm,fp))
        goto error;
    /*finally, it seems that the gnu might fill in some characters in the end*/
#if __WORDSIZE==32
    char align_char='4';
#elif __WORDSIZE==64
    char align_char='8';
#endif
    fprintf(fp,"\t.ident\t\"MCC\"\n");
    fprintf(fp,"\t.section\t.note.GNU-stack,\"\",@progbits\n");
    fprintf(fp,"\t.section\t.note.gnu.property,\"a\"\n");
    fprintf(fp,"\t.align %c\n",align_char);
    fprintf(fp,"\t.long\t 1f - 0f\n");
    fprintf(fp,"\t.long\t 4f - 1f\n");
	fprintf(fp,"\t.long\t 5\n");
    fprintf(fp,"0:\n");
    fprintf(fp,"\t.string\t\"GNU\"\n");
    fprintf(fp,"1:\n");
    fprintf(fp,"\t.align %c\n",align_char);
    fprintf(fp,"\t.long\t 0xc0000002\n");
    fprintf(fp,"\t.long\t 3f - 2f\n");
    fprintf(fp,"2:\n");
    fprintf(fp,"\t.long\t 0x3\n");
    fprintf(fp,"3:\n");
    fprintf(fp,"\t.align %c\n",align_char);
    fprintf(fp,"4:\n");
    fclose(fp);
    return true;
error:
    return false;
}