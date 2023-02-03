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
#ifdef _MAC_
    fprintf(fp,"\t.section\t__TEXT,__text,regular,pure_instructions\n");
#endif
#ifdef _UNIX_
    char split_ch='/';
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
#endif
    if(!optimizer(irm))
        goto error;
    if(!gen_asm(irm,fp))
        goto error;
    if(!gen_static_stor_data(irm,fp))
        goto error;
    /*
    finally, it seems that the gnu might fill in some characters in the end
    but the format might different on mac and linux.
    I have no idea of what to do, just copy it.
    */    
#ifdef _MAC_
    fprintf(fp,".subsections_via_symbols\n");
#endif
#ifdef _UNIX_
    
    /*
    I don't know what it did,but it seems like that code might used for the linker
    may be you need to check the gcc source code.
    so I just copied that here.
    besides,the align of data is different between 32 and 64 bit target file
    */
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
#endif
    fclose(fp);
    return true;
error:
    return false;
}
bool gen_static_stor_data(IR_MODULE* irm,FILE* fp)
{
    for(size_t i=0;i<VECLEN(irm->static_stor_symbols);++i)
    {
        STOR_VALUE* value=(STOR_VALUE*)VEC_GET_ITEM(irm->static_stor_symbols,i);
        fprintf(fp,"\t.globl\t%s\n",value->sym_item->value);
        fprintf(fp,"%s:\n",value->sym_item->value);
        for(size_t j=0;j<VECLEN(value->value_vec);++j)
        {
            STOR_VALUE_ELEM* elem=VEC_GET_ITEM(value->value_vec,j);
            if(elem->value_data_type==SSVT_NONE)
            {
                if(elem->byte_width==0)
#ifdef _MAC_
                    fprintf(fp,"\t.space\t%lld\n",elem->data);
#endif
#ifdef _UNIX_
                    fprintf(fp,"\t.zero\t%lld\n",elem->data);
#endif
                else if(elem->byte_width==1)
                    fprintf(fp,"\t.byte\t%lld\n",elem->data);
                else if(elem->byte_width==2)
                    fprintf(fp,"\t.word\t%lld\n",elem->data);
                else if(elem->byte_width==4)
                    fprintf(fp,"\t.long\t%lld\n",elem->data);
                else if(elem->byte_width==8)
                    fprintf(fp,"\t.quad\t%lld\n",elem->data); 
            }
            else if(elem->value_data_type==SSVT_POINTER)
            {
                fprintf(fp,"\t.quad\t%s\n",((SYM_ITEM*)elem->data)->value); 
            }
        }
    }
    return true;
}