#ifndef _INS_H_
#define _INS_H_
#define IR_DATA_TYPE \
    int ir_d_type; \
    int ir_d_len; /* 0-no,1-signed (i),2-unsigned (u),3-float (f)*/
enum ins_op
{
    OP_NONE
};
typedef struct {
    enum ins_op op;

}IR_INS;
IR_INS* GenINS();
#endif