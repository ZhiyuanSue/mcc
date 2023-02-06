#include "m_type_vec.h"
extern size_t type_data_size[TYPE_NUM];
int int_rank[TYPE_NUM+1]={
        255,255,2,2,
        3,3,4,4,
        5,5,6,6,
        255,255,255,1,
        255,255,255,255,
        255,255,255,255,
        255,255,255,255,
        4
};
bool Type_VEC_cmp(VEC* a,VEC* b){
    M_TYPE* tmpa=VEC_GET_ITEM(a,0);
    if(tmpa->typ_category==TP_TYPE_DEF_TYPE){
        VEC* tmpavec=((TP_DEF_TYPE*)tmpa)->typedef_name_type;
        return Type_VEC_cmp(tmpavec,b);
    }
    M_TYPE* tmpb=VEC_GET_ITEM(b,0);
    if(tmpb->typ_category==TP_TYPE_DEF_TYPE){
        VEC* tmpbvec=((TP_DEF_TYPE*)tmpb)->typedef_name_type;
        return Type_VEC_cmp(a,tmpbvec);
    }
    if(VECLEN(a)!=VECLEN(b))
        return false;
    for(size_t i=0;i<VECLEN(a);++i){
        bool cmp=Type_item_cmp(VEC_GET_ITEM(a,i),VEC_GET_ITEM(b,i));
        if(!cmp)
            return false;
    }
    return true;
}
bool Type_item_cmp(void* a,void* b){
    if(a==NULL||b==NULL)
        return false;
    M_TYPE* x=(M_TYPE*)a;
    M_TYPE* y=(M_TYPE*)b;
    if(x->modifier!=y->modifier)
        return false;
    else{
        if(x->modifier){
            if(((x->typ_stor<=TP_TYPE_DEF&&y->typ_stor<=TP_TYPE_DEF)||x->typ_stor==y->typ_stor)&&x->align_spec==y->align_spec&&x->type_qual==y->type_qual&&x->func_spec==y->func_spec)
                return true;
            else
                return false;
        }
        else{
            if(x->typ_category!=y->typ_category)
                return false;
            else{
                switch (x->typ_category){
                    case TP_ENUM:
                    {
                        TP_ENUMERA* enumx=(TP_ENUMERA*)x;
                        TP_ENUMERA* enumy=(TP_ENUMERA*)y;
                        if(enumx->tag==NULL||enumy->tag==NULL)
                            return false;
                        else
                            return !strcmp(enumx->tag,enumy->tag);
                    }
                    case TP_UNION_STRUCT:
                    case TP_UNION:
                    case TP_STRUCT:
                    {
                        /*just need to cmp tag*/
                        TP_SU* tmpsux=(TP_SU*)x;
                        TP_SU* tmpsuy=(TP_SU*)y;
                        if(tmpsux->tag==NULL||tmpsuy->tag==NULL)
                            return false;
                        else
                            return !strcmp(tmpsux->tag,tmpsuy->tag);    /*if same,strcmp return value is 0,remember that*/
                    }
                    case TP_ARRAY:
                    {
                        TP_ARR* tmparrx=(TP_ARR*)x;
                        TP_ARR* tmparry=(TP_ARR*)y;
                        if(tmparrx->axis_size!=tmparry->axis_size)
                            return false;
                        return true;
                    }
                    case TP_FUNCTION:
                    {
                        TP_FUNC* tmpfuncx=(TP_FUNC*)x;
                        TP_FUNC* tmpfuncy=(TP_FUNC*)y;
                        if(tmpfuncx->func_name&&tmpfuncy->func_name){
                            if(strcmp(tmpfuncx->func_name,tmpfuncy->func_name))
                                return false;
                            else
                            {
                                if(VECLEN(tmpfuncx->func_parameters)!=VECLEN(tmpfuncy->func_parameters))
                                    return false;
                                for(size_t i=0;i<VECLEN(tmpfuncx->func_parameters);++i){
                                    TP_FUNC_PARA* parax=(TP_FUNC_PARA*)VEC_GET_ITEM(tmpfuncx->func_parameters,i);
                                    TP_FUNC_PARA* paray=(TP_FUNC_PARA*)VEC_GET_ITEM(tmpfuncy->func_parameters,i);
                                    if(!Type_VEC_cmp(parax->type_vec,paray->type_vec))
                                        return false;
                                }
                            }
                        }
                        else    /*a function type is promised to have name,just for strcmp*/
                            return false;
                        break;
                    }
                    case TP_TYPE_DEF_TYPE:
                    {
                        TP_DEF_TYPE* defx=(TP_DEF_TYPE*)x;
                        TP_DEF_TYPE* defy=(TP_DEF_TYPE*)y;
                        if(!Type_VEC_cmp(defx->typedef_name_type,defy->typedef_name_type))
                            return false;
                        break;
                    }
                    case TP_SPEC_ATOMIC:
                        return false;
                    default:
                        return true;
                }
            }
        }
    }
    return true;
}
bool Type_VEC_VM(VEC* type_vec)
{
    if(!type_vec||VECLEN(type_vec)==0){
        return false;
    }
    /*have to scan all*/
    for(size_t i=0;i<VECLEN(type_vec);++i){
        M_TYPE* tmpt=VEC_GET_ITEM(type_vec,i);
        if(tmpt->typ_category==TP_ARRAY&&((TP_ARR*)tmpt)->is_vla){
            return true;
        }
    }
    return false;
}
bool Type_VEC_VLA(VEC* type_vec)
{
    if(!type_vec||VECLEN(type_vec)==0){
        return false;
    }
    M_TYPE* tmpt=Type_VEC_get_actual_base_type(type_vec);
    if(!tmpt)
        return false;
    if(tmpt->typ_category!=TP_ARRAY)    /*a vla must be an array whose length is uncertainty*/
        return false;
    VEC* tmpv=Type_VEC_get_Array_TO(type_vec,true);
    for(int i=VECLEN(tmpv)-1;i>=0;i--){
        M_TYPE* tmpt=VEC_GET_ITEM(tmpv,i);
        if(tmpt->modifier)
            continue;
        if(tmpt->typ_category==TP_ARRAY&&((TP_ARR*)tmpt)->is_vla){
            return true;
        }
        if(tmpt->typ_category!=TP_ARRAY){
            if(tmpt->complete)
                return false;
            else
                return true;
        }
    }
    return false;
}
bool Type_VEC_modify_lvalue(VEC* type_vec,bool is_lvalue)
{
    if(!type_vec||VECLEN(type_vec)==0)
        return false;
    if(!is_lvalue)
        return false;
    M_TYPE* tmpt=Type_VEC_get_actual_base_type(type_vec);
    if(tmpt&&(tmpt->typ_category==TP_ARRAY||tmpt->typ_category==TP_FUNCTION))
        return false;
    /*
        Althought it is not said that cannot be a function,the modify lvalue must be a lvalue,which cannot be a function
    */
    if(tmpt&&(tmpt->typ_category==TP_STRUCT||tmpt->typ_category==TP_UNION)){
        if(((TP_SU*)tmpt)->can_be_modified_lvalue==false)
            return false;
    }
    if(tmpt&&!tmpt->complete)
        return false;
    tmpt=Type_VEC_get_qual(type_vec);
    if(tmpt&&tmpt->type_qual>=8)
        return false;
    return true;
}
size_t Type_size(VEC* type_vec)
{
    if(!type_vec)
        return 0;
    size_t res=0;
    for(size_t i=0;i<VECLEN(type_vec);++i){
        /*
            to get the size you have to scan all the node
            the scan order must be buttom to top,because the pointer type might reset the size
        */
        M_TYPE* tmpt=VEC_GET_ITEM(type_vec,i);
        if(tmpt->modifier)
            continue;
        if(tmpt->typ_category==TP_SPEC_NONE||tmpt->typ_category==TP_VOID)
            res=0;
        else if(IS_BASE_TYPE(tmpt->typ_category)||tmpt->typ_category==TP_POINT)
        {
            res=type_data_size[tmpt->typ_category];
        }
        else if(tmpt->typ_category==TP_UNION_STRUCT||tmpt->typ_category==TP_UNION||tmpt->typ_category==TP_STRUCT){
            TP_SU* tmpt_su=(TP_SU*)tmpt;
            res=tmpt_su->total_data_size;
        }
        else if(tmpt->typ_category==TP_ARRAY){
            TP_ARR* tmpt_arr=(TP_ARR*)tmpt;
            res*=tmpt_arr->axis_size;
        }
        else if(tmpt->typ_category==TP_ENUM){
            res=type_data_size[TP_SINT];
        }
    }
    return res;
}
size_t Type_align(VEC* type_vec)
{
    if(!type_vec||VECLEN(type_vec)==0)
        return 0;
    size_t res=0;
    size_t spec_res=0;
    /*first check the alignas keyword case*/
    M_TYPE* tmpt=Type_VEC_get_spec_other(type_vec);
    if(tmpt&&tmpt->align_spec!=0)
        spec_res = tmpt->align_spec;
    tmpt=Type_VEC_get_actual_base_type(type_vec);
    if(!tmpt)
        return 0;
    if(tmpt->typ_category==TP_SPEC_NONE||tmpt->typ_category==TP_VOID)
        res=0;
    else if(IS_BASE_TYPE(tmpt->typ_category)||tmpt->typ_category==TP_POINT)
    {
        res=type_data_size[tmpt->typ_category];
    }
    else if(tmpt->typ_category==TP_UNION_STRUCT||tmpt->typ_category==TP_UNION||tmpt->typ_category==TP_STRUCT){
        TP_SU* tmpt_su=(TP_SU*)tmpt;
        res=tmpt_su->data_align;
    }
    else if(tmpt->typ_category==TP_ENUM)
    {
        res=type_data_size[TP_SINT];
    }
    else if(tmpt->typ_category==TP_ARRAY)
    {
        VEC* tmp_type_vec=Type_VEC_get_Array_TO(type_vec,true);
        tmpt=Type_VEC_get_actual_base_type(type_vec);
        res=Type_align(tmp_type_vec);
    }
    if(res<spec_res)
        res=spec_res;
    return res;
}
M_TYPE* Type_VEC_get_spec_other(VEC* type_vec)
{
    if(!type_vec||VECLEN(type_vec)==0)
        return NULL;
    M_TYPE* tmpt=VEC_GET_ITEM(type_vec,0);
    if(tmpt&&tmpt->modifier)
        goto final;
    if(VECLEN(type_vec)>=2){
        tmpt=VEC_GET_ITEM(type_vec,1);
        if(tmpt->modifier)
            goto final;
    }
    return NULL;
final:
    if(!tmpt)
        return NULL;
    if(tmpt->align_spec||tmpt->func_spec||tmpt->typ_stor)
        return tmpt;
    return NULL;
}
M_TYPE* Type_VEC_get_qual(VEC* type_vec)
{
    if(!type_vec||VECLEN(type_vec)==0)
        return NULL;
    M_TYPE* tmpt=VEC_BACK(type_vec);
    if(!tmpt)
        return NULL;
    if(tmpt->modifier){
        if(tmpt->type_qual)
            return tmpt;
    }
    return NULL;
}
M_TYPE* Type_composite_qual(M_TYPE* qual_a,M_TYPE* qual_b)
{
    if(!qual_a&&!qual_b)
        return NULL;
    if(!qual_a)
        return qual_b;
    if(!qual_b)
        return qual_a;
    if(!qual_a->modifier||!qual_b->modifier)
        return NULL;
    if(qual_a->typ_stor!=qual_b->typ_stor)
        return NULL;
    int type_qual_cnt[TYPE_QUAL_NUM];
    int func_spec_cnt[FUNCTION_SPEC_NUM];
    for(size_t i=0;i<TYPE_QUAL_NUM;++i)
    {
        type_qual_cnt[i]=((qual_a->type_qual)&0x01)&((qual_b->type_qual)&0x01);
        (qual_a->type_qual)>>=1;
        (qual_b->type_qual)>>=1;
    }
    for(size_t i=0;i<FUNCTION_SPEC_NUM;++i)
    {
        func_spec_cnt[i]=((qual_a->func_spec)&0x01)&((qual_b->func_spec)&0x01);
        (qual_a->func_spec)>>=1;
        (qual_b->func_spec)>>=1;
    }
    int align_spec_cnt=max(qual_a->align_spec,qual_b->align_spec);
    M_TYPE* res_type=build_modify_type(
        qual_a->typ_stor,
        type_qual_cnt,
        func_spec_cnt,
        align_spec_cnt
    );
    return res_type;
}
M_TYPE* Type_VEC_get_actual_base_type(VEC* type_vec)
{
    if(!type_vec||VECLEN(type_vec)==0)
        return NULL;
    for(int i=VECLEN(type_vec)-1;i>=0;i--){
        M_TYPE* tmpt=VEC_GET_ITEM(type_vec,i);
        if(tmpt->modifier)
        {
            continue;
        }
        else{
            if(tmpt->typ_category==TP_SPEC_ATOMIC){
                continue;
            }
            else
            {
                return tmpt;
            }
        }
    }
    return NULL;
}
bool Type_VEC_change_actual_base_type(VEC* type_vec,M_TYPE* tmp_type)
{
    /*This function assume that the type vec exist and have a base type,if not ,build one*/
    if(!type_vec||VECLEN(type_vec)==0)
        return false;
    if(!tmp_type)
        return false;
    for(int i=VECLEN(type_vec)-1;i>=0;i--){
        M_TYPE* tmpt=VEC_GET_ITEM(type_vec,i);
        if(tmpt->modifier)
            continue;
        else{
            if(tmpt->typ_category==TP_SPEC_ATOMIC){
                continue;
            }
            else
            {
                type_vec->data[i]=(void*)tmp_type;
                return true;
            }
        }
    }
    return false;
}
M_TYPE* Type_VEC_get_Atomic(VEC* type_vec)
{
    if(!type_vec||VECLEN(type_vec)==0)
        return NULL;
    for(int i=VECLEN(type_vec)-1;i>=0;i--){
        M_TYPE* tmpt=VEC_GET_ITEM(type_vec,i);
        if(tmpt->modifier)
            continue;
        else{
            if(tmpt->typ_category==TP_SPEC_ATOMIC){
                return tmpt;
            }
            else
                return NULL;
        }
    }
    return NULL;
}
VEC* Type_VEC_get_Atomic_TO(VEC* type_vec,bool is_atomic)
{
    if(!type_vec||VECLEN(type_vec)==0){
        return NULL;
    }
    VEC* res_vec=NULL;
    VECcpy(type_vec,&res_vec);
    if(!res_vec)
        return NULL;
    if(!is_atomic)
        return res_vec;
    for(int i=VECLEN(res_vec)-1;i>=0;i--){
        M_TYPE* tmpt=VEC_GET_ITEM(res_vec,i);
        if(tmpt->modifier)
            continue;
        else{
            if(tmpt->typ_category==TP_SPEC_ATOMIC){
                VECremove(res_vec,i,VECLEN(res_vec));
                break;
            }
            else{   /*if there's a judge of atomic,it's impossible*/
                DelVEC(res_vec);
                return NULL;
            }
        }
    }
    return res_vec;
}

VEC* Type_VEC_get_Pointer_TO(VEC* type_vec,bool is_pointer)
{
    if(!type_vec||VECLEN(type_vec)==0)
        return NULL;
    VEC* res_vec=NULL;
    VECcpy(type_vec,&res_vec);
    if(!res_vec)
        return NULL;
    if(!is_pointer)
        return res_vec;
    for(int i=VECLEN(res_vec)-1;i>=0;i--){
        M_TYPE* tmpt=VEC_GET_ITEM(res_vec,i);
        if(tmpt->modifier)
            continue;
        else{
            if(tmpt->typ_category==TP_SPEC_ATOMIC){
                continue;
            }
            else if(tmpt->typ_category==TP_POINT){
                VECremove(res_vec,i,VECLEN(res_vec));
                break;
            }
            else{
                DelVEC(res_vec);
                return NULL;
            }
        }
    }
    return res_vec;
}
VEC* Type_VEC_get_Array_TO(VEC* type_vec,bool is_array)
{
    if(!type_vec||VECLEN(type_vec)==0)
        return NULL;
    VEC* res_vec=NULL;
    VECcpy(type_vec,&res_vec);
    if(!res_vec)
        return NULL;
    if(!is_array)
        return res_vec;
    for(int i=VECLEN(res_vec)-1;i>=0;i--){
        M_TYPE* tmpt=VEC_GET_ITEM(res_vec,i);
        if(tmpt->modifier)
            continue;
        else{
            if(tmpt->typ_category==TP_SPEC_ATOMIC){
                continue;
            }
            else if(tmpt->typ_category==TP_ARRAY){
                VECremove(res_vec,i,VECLEN(res_vec));
                break;
            }
            else{
                DelVEC(res_vec);
                return NULL;
            }
        }
    }
    return res_vec;
}
VEC* Type_VEC_get_func_return_type(VEC* type_vec,bool is_function)
{
    if(!type_vec||VECLEN(type_vec)==0)
        return NULL;
    VEC* res_vec=NULL;
    VECcpy(type_vec,&res_vec);
    if(!res_vec)
        return NULL;
    if(!is_function)
        return res_vec;
    for(int i=VECLEN(res_vec)-1;i>=0;i--){
        M_TYPE* tmpt=VEC_GET_ITEM(res_vec,i);
        if(tmpt->modifier)
            continue;
        else{
            if(tmpt->typ_category==TP_SPEC_ATOMIC){
                continue;
            }
            else if(tmpt->typ_category==TP_FUNCTION){
                VECremove(res_vec,i,VECLEN(res_vec));
                break;
            }
            else{
                DelVEC(res_vec);
                return NULL;
            }
        }
    }
    return res_vec;
}
VEC* Type_VEC_unqualifier(VEC* type_vec,bool is_qual)
{
    if(!type_vec||VECLEN(type_vec)==0)
        return NULL;
    VEC* res_vec=NULL;
    VECcpy(type_vec,&res_vec);
    if(!is_qual)
        return res_vec;
    if(!res_vec)
        return NULL;
    M_TYPE* tmpt=VEC_BACK(res_vec);
    if(!tmpt)
        return NULL;
    if(tmpt->modifier){
        VECpopback(res_vec);
    }
    return res_vec;
}
VEC* lvalue_convertion(VEC* tmp_type_vec)
{
    if(!tmp_type_vec||VECLEN(tmp_type_vec)==0)
        return NULL;
    VEC* res=Type_VEC_unqualifier(tmp_type_vec,Type_VEC_get_qual(tmp_type_vec));
    if(!res)
        return NULL;
    VEC* oldres=res;
    res=Type_VEC_get_Atomic_TO(res,Type_VEC_get_Atomic(res));
    if(oldres!=res)
        DelVEC(oldres);
    M_TYPE* tmpt=Type_VEC_get_actual_base_type(res);
    if(tmpt->typ_category==TP_ARRAY){
        oldres=res;
        res=Type_VEC_get_Array_TO(res,true);
        if(oldres!=res)
            DelVEC(oldres);
        M_TYPE* pointer=build_base_type(TP_POINT);
        VECinsert(res,(void*)pointer);
    }
    else if(tmpt->typ_category==TP_FUNCTION){
        M_TYPE* pointer=build_base_type(TP_POINT);
        VECinsert(res,(void*)pointer);
    }
    return res;
}
bool compatible_types(VEC* type_vec_a,VEC* type_vec_b)
{
    if(!type_vec_a||!type_vec_b)
        return false;
    printf("compatible_type part still need to do\n");
    print_type_vec(type_vec_a);
    print_type_vec(type_vec_b);
    /*TODO*/
    return true;
}
VEC* composite_types(VEC* type_vec_a,VEC* type_vec_b,bool compatible_type)
{
    /*TODO*/
    if(!type_vec_a||!type_vec_b||!compatible_type)
        return NULL;
    printf("composite_type part still need to do\n");
    print_type_vec(type_vec_a);
    print_type_vec(type_vec_b);
    
    VEC* tmpv;
    VECcpy(type_vec_a,&tmpv);
    return tmpv;
}
M_TYPE* usual_arith_conversion(M_TYPE** operand_a,M_TYPE** operand_b){
    /*
    Unless explicitly stated otherwise, the common real type is also the corresponding real type of the result
    so no need to change the operand_a and operand_b type
    */
    if(!operand_a||!operand_b)
        return NULL;
    enum TP_CATEGORY type_a=(*operand_a)->typ_category,type_b=(*operand_b)->typ_category;
    if((!IS_ARTH_TYPE(type_a))||(!IS_ARTH_TYPE(type_b)))
        return NULL;
    if(type_a==TP_LONG_DOUBLE||type_b==TP_LONG_DOUBLE)
    {
        return build_base_type(TP_LONG_DOUBLE);
    }
    else if(type_a==TP_DOUBLE||type_b==TP_DOUBLE)
    {
        return build_base_type(TP_DOUBLE);
    }
    else if(type_a==TP_FLOAT||type_b==TP_FLOAT)
    {
        return build_base_type(TP_FLOAT);
    }
    if(IS_INT_TYPE(type_a)&&IS_INT_TYPE(type_b))
    {
        integer_promotion(operand_a);
        integer_promotion(operand_b);
        type_a=(*operand_a)->typ_category;
        type_b=(*operand_b)->typ_category;
        if(type_a==type_b)
            return build_base_type(type_a);
        else if(type_a%2==type_b%2)
        {
            if(int_rank[type_a]<int_rank[type_b]){
                *operand_a=build_base_type(type_b);
                return build_base_type(type_b);
            }
            else if(int_rank[type_a]>int_rank[type_b])
            {
                *operand_b=build_base_type(type_a);
                return build_base_type(type_a);
            }
            else if(int_rank[type_a]==int_rank[type_b])
            {
                /*only the null pointer const case*/
                if(type_a==TP_NULL_POINTER_CONSTANT)
                    return build_base_type(type_b);
                else if(type_b==TP_NULL_POINTER_CONSTANT)
                    return build_base_type(type_a);
            }
        }
        else{
            if(int_rank[type_a]<=int_rank[type_b]&&type_b%2==1)
            {
                *operand_a=build_base_type(type_b);
                return build_base_type(type_b);
            }
            else if(int_rank[type_a]>=int_rank[type_b]&&type_a%2==1)
            {
                *operand_b=build_base_type(type_a);
                return build_base_type(type_a);
            }
            else if(int_rank[type_a]<int_rank[type_b]&&type_data_size[type_b]>type_data_size[type_a]){
                *operand_a=build_base_type(type_b);
                return build_base_type(type_b);
            }
            else if(int_rank[type_a]>int_rank[type_b]&&type_data_size[type_b]<type_data_size[type_a]){
                *operand_b=build_base_type(type_a);
                return build_base_type(type_a);
            }
            else{
                enum TP_CATEGORY return_type=TP_SPEC_NONE;
                if(type_a%2==0)
                {
                    return_type=type_a+1;
                }
                else if(type_b%2==0)
                {
                    return_type=type_b+1;
                }
                *operand_b=build_base_type(return_type);
                *operand_a=build_base_type(return_type);
                return build_base_type(return_type);
            }
        }
    }
#if _CPLX_SUPPORT==1
    else    /*TODO:complex case,which might not support by some compliers*/
    {
        return NULL;
    }
#endif
    return NULL;
}

bool integer_promotion(M_TYPE** integer_operand){
    M_TYPE* operand=*integer_operand;
    if(!integer_operand||!operand||!IS_INT_TYPE(operand->typ_category))
        return false;
    int rank=int_rank[operand->typ_category];
    if(rank>=int_rank[TP_SINT]) /*no need to promotion*/
        return false;
    M_TYPE* new_o=build_base_type(TP_SINT);
    integer_operand=&new_o;
    return true;
}
bool float_promotion(M_TYPE** float_operand){
    M_TYPE* operand=*float_operand;
    if(!float_operand||!operand||!IS_FLOAT_TYPE(operand->typ_category))
        return false;
    if(operand->typ_category!=TP_FLOAT)
        return true;
    M_TYPE* new_o=build_base_type(TP_FLOAT);
    *float_operand=new_o;
    return true;
}

size_t Type_VEC_get_element_size(VEC* current_obj_type)
/*
    This function is used to count a given type vec , how many elements in this type
    e.x.
    struct {
        int a[3];
        double b;
    };
    have 3+1 =4 elements
*/
{
    size_t element_size=0;
    M_TYPE* base_type=Type_VEC_get_actual_base_type(current_obj_type);
    if(IS_SCALAR_TYPE(base_type->typ_category))
    {
        element_size=1;
    }
    else if(base_type->typ_category==TP_ARRAY){
        if(!((TP_ARR*)base_type)->is_vla)
        {
            VEC* sub_obj_type=Type_VEC_get_Array_TO(current_obj_type,true);
            element_size=(((TP_ARR*)base_type)->axis_size)*Type_VEC_get_element_size(sub_obj_type);
        }
    }
    else if(base_type->typ_category==TP_STRUCT)
    {
        VEC* struct_member_list=((TP_SU*)base_type)->members;
        for(size_t i=0;i<VECLEN(struct_member_list);++i){
            TP_SU_MEMBER* sub_obj=VEC_GET_ITEM(struct_member_list,i);
            element_size+=Type_VEC_get_element_size(sub_obj->type_vec);
        }
    }
    else if(base_type->typ_category==TP_UNION){
        VEC* struct_member_list=((TP_SU*)base_type)->members;
        TP_SU_MEMBER* sub_obj=VEC_GET_ITEM(struct_member_list,0);
        element_size+=Type_VEC_get_element_size(sub_obj->type_vec);
    }
    return element_size;
}
VEC* Type_VEC_get_sub_obj_type(VEC* current_obj_type,size_t sub_obj_size)
{
    if(!current_obj_type)
        return NULL;
    VEC* res_vec=NULL;
    M_TYPE* actual_type=Type_VEC_get_actual_base_type(current_obj_type);
    if(IS_SCALAR_TYPE(actual_type->typ_category))
    {
        VECcpy(current_obj_type,&res_vec);
    }
    else if(actual_type->typ_category==TP_ENUM)
    {
        M_TYPE* tmpt=build_base_type(TP_SINT);
        res_vec=InitVEC(DEFAULT_CAPICITY);
        VECinsert(res_vec,tmpt);
    }
    else if(actual_type->typ_category==TP_ARRAY)
    {
        res_vec=Type_VEC_get_Array_TO(current_obj_type,true);
    }
    else if(actual_type->typ_category==TP_STRUCT||actual_type->typ_category==TP_UNION)
    {
        VEC* struct_member_list=((TP_SU*)actual_type)->members;
        TP_SU_MEMBER* sub_obj=VEC_GET_ITEM(struct_member_list,sub_obj_size);
        VECcpy(sub_obj->type_vec,&res_vec);
    }
    return res_vec;
}
VEC* Type_VEC_get_sub_obj_off_element_type(VEC* sub_obj_type,size_t sub_obj_off)
/*
    let me use the previous example to explain the function:
    struct {
        int a[3];
        double b;
    };
    sub_obj_type is TP_STRUCT
    if sub_obj_off is 0,1,2 the return type is int ——for the first 3 elements is int type
    if the sub_obj_off is 3 , the return type is double ——the b's type
*/
{
    if(!sub_obj_type)
        return NULL;
    VEC* res_vec=NULL;
    M_TYPE* actual_type=Type_VEC_get_actual_base_type(sub_obj_type);
    if(IS_SCALAR_TYPE(actual_type->typ_category))
    {
        VECcpy(sub_obj_type,&res_vec);
    }
    else if(actual_type->typ_category==TP_ENUM)
    {
        M_TYPE* tmpt=build_base_type(TP_SINT);
        res_vec=InitVEC(DEFAULT_CAPICITY);
        VECinsert(res_vec,tmpt);
    }
    else if(actual_type->typ_category==TP_ARRAY)
    {
        /*Note:it's sub obj type, which must not be a vla*/
        VEC* sub_sub_obj_type=Type_VEC_get_Array_TO(sub_obj_type,true);
        size_t sub_sub_obj_element_size=Type_VEC_get_element_size(sub_sub_obj_type);
        res_vec=Type_VEC_get_sub_obj_off_element_type(sub_sub_obj_type,sub_obj_off%sub_sub_obj_element_size);
    }
    else if(actual_type->typ_category==TP_STRUCT)
    {
        VEC* struct_member_list=((TP_SU*)actual_type)->members;
        for(size_t i=0;i<VECLEN(struct_member_list);++i)
        {
            TP_SU_MEMBER* struct_member=VEC_GET_ITEM(struct_member_list,i);
            size_t struct_member_size=Type_VEC_get_element_size(struct_member->type_vec);
            if(struct_member_size>sub_obj_off)
            {
                res_vec=Type_VEC_get_sub_obj_off_element_type(struct_member->type_vec,sub_obj_off);
            }
            else{
                sub_obj_off-=struct_member_size;
            }
        }
    }
    else if(actual_type->typ_category==TP_UNION)
    {
        VEC* struct_member_list=((TP_SU*)actual_type)->members;
        TP_SU_MEMBER* struct_member=VEC_GET_ITEM(struct_member_list,0);
        res_vec=Type_VEC_get_sub_obj_off_element_type(struct_member->type_vec,sub_obj_off);
    }
    return res_vec;
}

void print_type_vec(VEC* type_vec)
{
    printf("<");
    if(type_vec==NULL)
        printf("(nil)");
    else{
        for(int i=VECLEN(type_vec)-1;i>=0;i--){
            M_TYPE* tmpt=(M_TYPE*)VEC_GET_ITEM(type_vec,i);
            if(tmpt)
                print_type(tmpt);
            if(i)
                printf(",");
        }
    }
    printf(">\n");
}
