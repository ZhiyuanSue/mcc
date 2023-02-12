#include "transdecl.h"
extern size_t type_data_size[TYPE_NUM];
size_t stack_off;
/*Hint: the stack address goes down,
but the off set to positive,when you calculate the true address of a symbol on a stack
please don't forget that pointer*/
bool declaration_trans(AST_BASE* ast_node,IR_MODULE* irm,IR_FUNC* ir_func,IR_BB* ir_bb)
{
    ERROR_ITEM* tei=(ERROR_ITEM*)m_alloc(sizeof(ERROR_ITEM));
    if(!ast_node||!irm||ast_node->type!=declaration)
        goto error;
    SYM* curr_sym_table= ast_node->symbol_table;
    AST_BASE* type_def_kw=ast_node;
    while(AST_CHILD_NUM(type_def_kw)>0){
        type_def_kw=AST_GET_CHILD(type_def_kw,0);
    }
    if(type_def_kw->type==KW_type_def)
        return true;    /*do nothing to typedef declarator*/
    AST_BASE* init_decl_list_node=AST_GET_CHILD(ast_node,1);
    for(size_t i=0;i<AST_CHILD_NUM(init_decl_list_node);i+=2){
        AST_BASE* init_decl_node=AST_GET_CHILD(init_decl_list_node,i);
        AST_BASE* decl_node=AST_GET_CHILD(init_decl_node,0);
        AST_BASE* identifier_node=declarator_node(decl_node);
        SYM_ITEM* tmpsi=find_symbol_curr_table(curr_sym_table,
                identifier_node->token->value,
                decl_node->name_space);
        if(!tmpsi)
            goto error;/*actually impossible*/
        /*The extern decl symbol should not alloc space,
        and if one extern appear,the others must have extern ,so return directly*/
        M_TYPE* tmp_m_type=Type_VEC_get_spec_other(tmpsi->type_vec);
        if(curr_sym_table->sp_type==SPT_FILE)
        {
            if(tmp_m_type&&(tmp_m_type->typ_stor==TP_EXTERN||tmp_m_type->typ_stor==TP_EXTERN_THREAD_LOCAL))
                return true;
        }
        /*judge type*/
        M_TYPE* tmp_type=Type_VEC_get_actual_base_type(tmpsi->type_vec);
        if(!((tmp_type->typ_category>=TP_SCHAR&&tmp_type->typ_category<=TP_ARRAY)))
            return true;
        /*judge whether the symbol size is 0*/
        if(Type_size(tmpsi->type_vec)==0)
        {
            print_type_vec(tmpsi->type_vec);
            C_ERROR(C0096_ERR_STOR_SIZE_ISNT_KNOWN,identifier_node);
            goto error;
        }
        /*judge which place to put this symble*/
        if(tmpsi->stor_type!=IR_STOR_NONE)
            continue;
        if(tmp_m_type&&(tmp_m_type->typ_stor==TP_EXTERN_THREAD_LOCAL||tmp_m_type->typ_stor==TP_STATIC_THREAD_LOCAL))
            tmpsi->stor_type=IR_STOR_THREAD;
        else{
            if(curr_sym_table->sp_type==SPT_FILE)
            {
                tmpsi->stor_type=IR_STOR_STATIC;
            }
            else if(curr_sym_table->sp_type==SPT_BLOCK)
            {
                if(tmp_m_type&&tmp_m_type->typ_stor==TP_STATIC)
                    tmpsi->stor_type=IR_STOR_STATIC;
                else if(tmp_m_type&&tmp_m_type->typ_stor==TP_EXTERN)
                    continue;
                else
                    tmpsi->stor_type=IR_STOR_STACK;
            }
            else
                tmpsi->stor_type=IR_STOR_STACK;
        }
        STOR_VALUE* stor_value=(STOR_VALUE*)m_alloc(sizeof(STOR_VALUE));
        stor_value->value_vec=InitVEC(DEFAULT_CAPICITY);
        stor_value->sym_item=tmpsi;
        if(AST_CHILD_NUM(init_decl_node)==1)
        {
            if(tmpsi->stor_type==IR_STOR_STATIC||tmpsi->stor_type==IR_STOR_THREAD){
                /*all set to zero*/
                STOR_VALUE_ELEM* elem=m_alloc(sizeof(STOR_VALUE_ELEM));
                elem->bit_field=false;
                elem->ignore=false;
                elem->byte_width=0;
                elem->value_data_type=SVT_NONE;
                elem->idata=Type_size(tmpsi->type_vec);
                elem->init_attr=NULL;
                VECinsert(irm->static_stor_symbols,(void*)stor_value);
            }
            else if(tmpsi->stor_type==IR_STOR_STACK){
                if(!ir_func||!ir_bb)
                {
                    printf("try to alloca but not find basic block\n");
                    goto error;
                }
            }
        }
        else if(AST_CHILD_NUM(init_decl_node)==3)
        {
            /*sort the initializer node list accroding to the off*/
            VEC* tmpv=(VEC*)(tmpsi->data_field->pointer);
            for(size_t j=0;j<VECLEN(tmpv);++j){
                for(size_t k=j+1;k<VECLEN(tmpv);++k)
                {
                    AST_BASE* tmpj=VEC_GET_ITEM(tmpv,j);
                    AST_BASE* tmpk=VEC_GET_ITEM(tmpv,k);
                    if(tmpj->init_attribute->off>tmpk->init_attribute->off)
                    {
                        VECswapItem(tmpv,j,k);
                    }
                }
            }
            if(tmpsi->stor_type==IR_STOR_STATIC||tmpsi->stor_type==IR_STOR_THREAD){
                fill_in_init_value(tmpsi,stor_value,true,NULL,NULL);
                VECinsert(irm->static_stor_symbols,(void*)stor_value);
            }
            else if(tmpsi->stor_type==IR_STOR_STACK){
                if(!ir_func||!ir_bb)
                {
                    printf("try to alloca but not find basic block\n");
                    goto error;
                }
                SYM_ITEM* alloc_reg=alloca_on_stack_value(ir_bb,tmpsi);
                fill_in_init_value(tmpsi,stor_value,false,alloc_reg,ir_bb);
            }
        }
        
    }
    m_free(tei);
    return true;
error:
    return false;
}
bool fill_in_init_value(SYM_ITEM* symbol,STOR_VALUE* value,bool static_stor,SYM_ITEM* alloc_reg,IR_BB* ir_bb)
{
    /*as we have calculated the off and the size of one initializer node,just fill data in it*/
    if(!symbol||!value)
        goto error;
    if(!static_stor&&!alloc_reg&&!ir_bb)
    {
        printf("a value alloc on stack but without enough info!\n");  /*impossible*/
        goto error;
    }
    if(!(symbol->data_field->pointer))
    {
        printf("need a init list\n");  /*impossible*/
        goto error;
    }
    ERROR_ITEM* tei=(ERROR_ITEM*)m_alloc(sizeof(ERROR_ITEM));
    size_t veclen=VECLEN(((VEC*)(symbol->data_field->pointer)));
    size_t index;
    bool init_have_bit_field=false;
    for(index=0;index < veclen ; ++index)
    {
        AST_BASE* initializer_node=VEC_GET_ITEM( ((VEC*)(symbol->data_field->pointer)) ,index);
        if(!initializer_node)
            break;
        AST_BASE* sub_node=AST_GET_CHILD(initializer_node,0);
        if(!IS_EXPR_NODE(sub_node->type))
        {
            printf("initializer node not have a expr\n");  /*impossible*/
            goto error;
        }
        STOR_VALUE_ELEM* elem=NULL;
        if(static_stor&&index==0&&initializer_node->init_attribute->off!=0)
        {
            /*if the first init node off is not 0,need padding*/
            elem=m_alloc(sizeof(STOR_VALUE_ELEM));
            elem->bit_field=false;
            elem->value_data_type=SVT_NONE;
            elem->byte_width=0;
            elem->idata=initializer_node->init_attribute->off/8;
            elem->init_attr=NULL;
            VECinsert(value->value_vec,(void*)elem);
        }
        /*step1: init all the elements according to the init list*/
        elem=m_alloc(sizeof(STOR_VALUE_ELEM));
        elem->bit_field=false;
        elem->ignore=false;
        elem->init_attr=NULL;
        elem->value_data_type=SVT_NONE;
        elem->byte_width=elem->idata=0;
        elem->init_attr=initializer_node->init_attribute;
        VECinsert(value->value_vec,(void*)elem);
        if(sub_node->symbol->const_expr)
        {
            VEC* type_vec = initializer_node->init_attribute->type_vec;
            M_TYPE* tmpt=Type_VEC_get_actual_base_type(type_vec);
            if(IS_INT_TYPE(tmpt->typ_category))
            {
                elem->byte_width=Type_size(type_vec);
                elem->value_data_type=SVT_NONE;
                M_TYPE* sub_node_type=Type_VEC_get_actual_base_type(sub_node->symbol->type_vec);
                elem->idata=get_int_const(sub_node_type->typ_category,sub_node->symbol->data_field,true);
                /*
                    if a bit field, do shift ,if need trunc ,also do it,
                    only int type or enum need such a trunc
                */
                size_t bit_field_size=0;
                if((initializer_node->init_attribute->size)!=8*Type_size(initializer_node->init_attribute->type_vec))
                    bit_field_size=initializer_node->init_attribute->size;
                size_t off_size=(initializer_node->init_attribute->off)%(8*Type_align(type_vec));
                if(bit_field_size!=0)
                {
                    elem->bit_field=true;
                    init_have_bit_field=true;
                    unsigned long long mask=0;
                    for(size_t i=0;i<bit_field_size;++i)
                        mask=(mask<<1)|((unsigned long long)0x1);
                    elem->idata&=mask;
                }
                if(off_size!=0)
                {
                    elem->bit_field=true;
                    elem->idata<<=off_size;
                    init_have_bit_field=true;
                }
            }
            else if(IS_FLOAT_TYPE(tmpt->typ_category))
            {
                elem->byte_width=Type_size(type_vec);
                elem->value_data_type=SVT_NONE;
                elem->fdata[0]=get_float_const(tmpt->typ_category,sub_node->symbol->data_field,true);
            }
            else if(IS_COMPLEX_TYPE(tmpt->typ_category))
            {
                elem->byte_width=Type_size(type_vec);
                elem->value_data_type=SVT_NONE;
                long double* fp=get_complex_const(tmpt->typ_category,sub_node->symbol->data_field,true);
                elem->fdata[0]=fp[0];
                elem->fdata[1]=fp[1];
            }
            else if(tmpt->typ_category==TP_NULL_POINTER_CONSTANT)
            {
                elem->byte_width=type_data_size[TP_POINT];
                elem->value_data_type=SVT_NONE;
                elem->idata=0;
            }
            else if(tmpt->typ_category==TP_POINT)
            {
                elem->byte_width=type_data_size[TP_POINT];
                elem->value_data_type=SVT_NONE;
                elem->pdata=(void*)get_int_const(tmpt->typ_category,sub_node->symbol->data_field,true);
            }
            else if(tmpt->typ_category==TP_ENUM)
            {
                elem->byte_width=type_data_size[TP_ENUM];
                elem->value_data_type=SVT_NONE;
                elem->idata=get_int_const(TP_SINT,sub_node->symbol->data_field,true);
                size_t bit_field_size=0;
                if((initializer_node->init_attribute->size)!=8*Type_size(initializer_node->init_attribute->type_vec))
                    bit_field_size=initializer_node->init_attribute->size;
                size_t off_size=(initializer_node->init_attribute->off)%(8*Type_align(type_vec));
                if(bit_field_size!=0)
                {
                    elem->bit_field=true;
                    init_have_bit_field=true;
                    unsigned long long mask=0;
                    for(size_t i=0;i<bit_field_size;++i)
                        mask=(mask<<1)|((unsigned long long)0x1);
                    elem->idata&=mask;
                }
                if(off_size!=0)
                {
                    elem->bit_field=true;
                    elem->idata<<=off_size;
                    init_have_bit_field=true;
                }
            }
            else if(tmpt->typ_category==TP_FUNCTION)
            {
                elem->byte_width=type_data_size[TP_POINT];
                elem->value_data_type=SVT_POINTER;
                elem->pdata=((TP_FUNC*)tmpt)->ir_func->symbol;
            }
            else if(tmpt->typ_category==TP_ARRAY)
            {
                
            }
            else if(tmpt->typ_category==TP_UNION||tmpt->typ_category==TP_STRUCT)
            {

            }
        }
        else{
            /*for static storage ,an error*/
            if(static_stor)
            {
                C_ERROR(C0097_ERR_STATIC_STOR_CONST,sub_node);
                goto error;
            }
            if(!expr_trans_dispatch(sub_node,ir_bb))
                goto error;
            elem->byte_width=Type_size(sub_node->symbol->type_vec);
            elem->value_data_type=SVT_REG;
            SYM_ITEM* curr_symbol=sub_node->symbol;
            /*if a bit field, just trunc*/
            if((initializer_node->init_attribute->size)!=8*Type_size(initializer_node->init_attribute->type_vec))
            {
                init_have_bit_field=true;
                SYM_ITEM* trunc_symbol=Create_symbol_item(tmp_symbol_str_alloc(".reg."),NMSP_DEFAULT);
                if(!insert_symbol(alloc_reg->symbol_table,trunc_symbol))
                    goto error;
                VECappend(curr_symbol->type_vec,trunc_symbol->type_vec);
                trunc_symbol->stor_type=IR_STOR_STACK;

                IR_INS* trunc_ins=add_new_ins(ir_bb);
                insert_ins_to_bb(trunc_ins,ir_bb);
                GenINS(trunc_ins,OP_TRUNC,trunc_symbol,curr_symbol,NULL);
                curr_symbol=trunc_symbol;
            }
            /*if need a shifting, generate a shift ins*/
            size_t off_size=(initializer_node->init_attribute->off)%(8*Type_align(curr_symbol->type_vec));
            if(off_size!=0)
            {
                init_have_bit_field=true;
                /*shift res reg*/
                SYM_ITEM* shift_symbol=Create_symbol_item(tmp_symbol_str_alloc(".reg."),NMSP_DEFAULT);
                if(!insert_symbol(alloc_reg->symbol_table,shift_symbol))
                    goto error;
                VECappend(curr_symbol->type_vec,shift_symbol->type_vec);
                shift_symbol->stor_type=IR_STOR_STACK;
                
                /*shift size reg*/
                SYM_ITEM* shift_size_symbol=Create_symbol_item(tmp_symbol_str_alloc(".reg."),NMSP_DEFAULT);
                if(!insert_symbol(alloc_reg->symbol_table,shift_size_symbol))
                    goto error;
                M_TYPE* size_type=build_base_type(TP_USLONG);
                VECinsert(shift_size_symbol->type_vec,(void*)size_type);
                shift_size_symbol->stor_type=IR_STOR_IMM;
                shift_size_symbol->data_field->uslong=off_size;

                /*gen ins*/
                IR_INS* shift_ins=add_new_ins(ir_bb);
                insert_ins_to_bb(shift_ins,ir_bb);
                GenINS(shift_ins,OP_SHL,shift_symbol,curr_symbol,shift_size_symbol);

                curr_symbol=shift_symbol;
            }
            /*change the the elem data*/
            elem->other_data=curr_symbol;
        }
        /*
            step2:if two init nodes or a init node and the end need a padding,just insert zero space
        */
        size_t off=initializer_node->init_attribute->off+initializer_node->init_attribute->size;
        off=MCC_ALIGN(off,8);
        size_t next_begin_off;
        if(index==veclen-1) /*goto the end*/
            next_begin_off=Type_size(symbol->type_vec)*8;
        else
        {
            AST_BASE* next_initializer_node=VEC_GET_ITEM( ((VEC*)symbol->data_field->pointer) ,index+1);
            next_begin_off=next_initializer_node->init_attribute->off;
        }
        if((size_t)(next_begin_off-8)>(size_t)off)
        {
            elem=m_alloc(sizeof(STOR_VALUE_ELEM));
            elem->byte_width=0;
            elem->value_data_type=SVT_NONE;
            elem->idata=(next_begin_off-off)/8;
            VECinsert(value->value_vec,(void*)elem);
        }
    }
    /*
        step3:if bit field and have a static storage ,just merge it
    */
    if(init_have_bit_field)
    {
        /*
            sort,according to the specifier's start off
            let's see an example in the type_test_08.c
            in a struct have such three members:
                char a:7;
                char b:6;
                int c:15;
            and the place in symbol table is:
                < a off:0 bit_off:0 bit_size:7 >
                < b off:1 bit_off:0 bit_size:6 >
                < c off:0 bit_off:14 bit_size:15 >
            and the merge happens in a/c and b/c
            here ,'a' have to skip 'b' to merge with 'c'
            and it's very hard to deal with(the order might be more complex than that case)
            the reason of this problem is the 'b' start off is 1.
            if the order of 'b' and 'c' changed become 'a' 'c' 'b',the merge is much more easy
            so here we need to sort it according to the off given by specifier.
            Hint:such a sort must happen in continuing bit field members
        */
        VEC* tmpv=value->value_vec;
        for(size_t j=0;j<VECLEN(tmpv);++j){
            for(size_t k=j+1;k<VECLEN(tmpv);++k)
            {
                STOR_VALUE_ELEM* tmpj=VEC_GET_ITEM(tmpv,j);
                STOR_VALUE_ELEM* tmpk=VEC_GET_ITEM(tmpv,k);
                if(tmpj->bit_field&&tmpk->bit_field)
                {
                    size_t off_align_j=tmpj->init_attr->off/(Type_align(tmpj->init_attr->type_vec));
                    size_t off_align_k=tmpk->init_attr->off/(Type_align(tmpk->init_attr->type_vec));
                    if(off_align_j>off_align_k)
                        VECswapItem(tmpv,j,k);
                }
            }
        }
        index=0;
        while(index<VECLEN(value->value_vec))
        {
            printf("curr index%ld\n",index);
            STOR_VALUE_ELEM* curr_sve=VEC_GET_ITEM(value->value_vec,index);
            if(curr_sve->bit_field==false||curr_sve->value_data_type==SVT_REG)
            {
                index++;
                continue;
            }
            else{
                size_t next_index=index+1;
                STOR_VALUE_ELEM* next_sve=VEC_GET_ITEM(value->value_vec,next_index);
                while(next_sve&& next_sve->bit_field)
                {
                    printf("next index%ld\n",next_index);
                    if(next_sve->value_data_type!=SVT_REG)
                    {
                        int need_merge=0;   /*0-don't merge 1-next merge into the curr,2-curr merge into the next*/
                        /*check the offset range and judge whether need merge*/
                        size_t curr_begin=(curr_sve->init_attr->off)/Type_align(curr_sve->init_attr->type_vec);
                        size_t curr_end=curr_begin+Type_size(curr_sve->init_attr->type_vec)*8;
                        size_t next_begin=(next_sve->init_attr->off)/Type_align(next_sve->init_attr->type_vec);
                        size_t next_end=next_begin+Type_size(next_sve->init_attr->type_vec)*8;
                        if(curr_begin<=next_begin&&curr_end>=next_end)
                            need_merge=1;
                        else if(curr_begin<=next_begin&&curr_end>=next_end)
                            need_merge=2;
                        if(need_merge==1)
                        {
                            curr_sve->idata=(curr_sve->idata)|(next_sve->idata);
                            next_sve->ignore=true;
                        }
                        else if(need_merge==2)
                        {
                            next_sve->idata=(next_sve->idata)|(curr_sve->idata);
                            curr_sve->ignore=true;
                            curr_sve=next_sve;
                        }
                    }
                    next_index+=1;
                    next_sve=VEC_GET_ITEM(value->value_vec,next_index);
                }
                index=next_index;
            }
        }
        for(size_t i=0;i<VECLEN(tmpv);++i)
        {
            STOR_VALUE_ELEM* tmp=VEC_GET_ITEM(tmpv,i);
            if(tmp->ignore)
            {
                VECremove(tmpv,i,i+1);
                m_free(tmp);
            }
        }
    }
    /*
        step4: if it's a data on stack,gen ins
        first sort,if all bit field,place the const part before the variable part
        as declared before, the order after merge is not always sure,so we must make sure the order
    */
    if(!static_stor)
    {
        VEC* tmpv=value->value_vec;
        for(int j=VECLEN(tmpv)-1;j>=0;--j){
            for(int k=j-1;k>=0;--k)
            {
                STOR_VALUE_ELEM* tmpj=VEC_GET_ITEM(tmpv,j);
                STOR_VALUE_ELEM* tmpk=VEC_GET_ITEM(tmpv,k);
                if(tmpj->bit_field&&tmpk->bit_field)
                    if(tmpk->value_data_type==SVT_REG && tmpj->value_data_type==SVT_NONE)
                        VECswapItem(tmpv,j,k);
            }
        }
        for(size_t i=0;i<VECLEN(value->value_vec);++i)
        {
            /*symbol of store position*/

            /*store size symbol*/

            /*if a bit field, load the value, 'or' it,then store it*/

            /*gen ins*/
            //IR_INS* store_ins=Create_symbol_item(tmp_symbol_str_alloc(".reg."),NMSP_DEFAULT);
            //insert_ins_to_bb(store_ins,ir_bb);
            //GenINS(store_ins,OP_SHL,shift_symbol,curr_symbol,shift_size_symbol);
        }
    }
    
    m_free(tei);
    return true;
error:
    return false;
}
SYM_ITEM* alloca_on_stack_value(IR_BB* ir_bb,SYM_ITEM* symbol)
{
    if(!ir_bb||!symbol)
        goto error;
    /*not static storage,generate code to fill in the data*/
    IR_INS* alloca_ins=add_new_ins(ir_bb);
    insert_ins_to_bb(alloca_ins,ir_bb);
    VEC* tmp_type_vec=symbol->type_vec;

    size_t alloca_size=Type_size(symbol->type_vec); 
    size_t alloca_align=Type_align(symbol->type_vec);

    /*gen a new symbol*/
    SYM_ITEM* res=Create_symbol_item(tmp_symbol_str_alloc(".stack."),NMSP_DEFAULT);
    if(!insert_symbol(symbol->symbol_table,res))
        goto error;
    VECappend(symbol->type_vec,res->type_vec);
    M_TYPE* pointer_type=build_base_type(TP_POINT);
    VECinsert(res->type_vec,(void*)pointer_type);
    GenINS(alloca_ins,OP_ALLOCA,res,symbol,NULL);

    /*fill the data in the reg of pointer */
    stack_off=MCC_ALIGN(stack_off,alloca_align);
    res->stor_type=IR_STOR_STACK;
    res->data_field->slong=(signed long int)stack_off;
    stack_off+=alloca_size;

    return res;
error:
    return NULL;
}