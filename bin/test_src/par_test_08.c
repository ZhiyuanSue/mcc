enum TP_CATEGORY{
    TP_SPEC_NONE,

    TP_VOID,
    TP_SCHAR,
    TP_USCHAR,
    TP_SSHORT,
    TP_USHORT,
    TP_SINT,
    TP_USINT,
    TP_SLONG,
    TP_USLONG,
    TP_SLONGLONG,
    TP_USLONGLONG,
    TP_FLOAT,
    TP_DOUBLE,
    TP_LONG_DOUBLE,
    TP_BOOL,
    TP_FLOAT_COMPLEX,
    TP_DOUBLE_COMPLEX,
    TP_LONG_DOUBLE_COMPLEX,

    TP_POINT,
    TP_UNION_STRUCT,
    TP_UNION,
    TP_STRUCT,
    TP_ARRAY,
    TP_ENUM,
    TP_FUNCTION,
    TP_TYPE_DEF_TYPE,
    TP_SPEC_ATOMIC,

};
void main(void){
    
int t=19;
int a;
a=(t==TP_VOID?1:( t==TP_SCHAR?2:( t==TP_USCHAR?3:( t==TP_SSHORT?4:( t==TP_USHORT?5:( t==TP_SINT?6:( t==TP_USINT?7:( t==TP_SLONG?8:( t==TP_USLONG?9:( t==TP_SLONGLONG?10:( t==TP_USLONGLONG?11:( t==TP_FLOAT?12:( t==TP_DOUBLE?13:0)))))))))))));
}