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
        printf("var:");
        STOR_VALUE* stor_value=(STOR_VALUE*)m_alloc(sizeof(STOR_VALUE));
        stor_value->value_vec=InitVEC(DEFAULT_CAPICITY);
        stor_value->sym_item=tmpsi;
        if(AST_CHILD_NUM(init_decl_node)==1)
        {
            if(tmpsi->stor_type==IR_STOR_STATIC||tmpsi->stor_type==IR_STOR_THREAD){
                /*all set to zero*/
                printf("static\n");
            }
            else if(tmpsi->stor_type==IR_STOR_STACK){
                if(!ir_func||!ir_bb)
                {
                    printf("try to alloca but not find basic block\n");
                    goto error;
                }
                printf("stack\n");
            }
            printf("%s\n",tmpsi->value);
        }
        else if(AST_CHILD_NUM(init_decl_node)==3)
        {
            if(tmpsi->stor_type==IR_STOR_STATIC||tmpsi->stor_type==IR_STOR_THREAD){
                printf("static\n");
                fill_in_init_value(tmpsi,stor_value,true,NULL,NULL);
            }
            else if(tmpsi->stor_type==IR_STOR_STACK){
                if(!ir_func||!ir_bb)
                {
                    printf("try to alloca but not find basic block\n");
                    goto error;
                }
                printf("stack\n");
                SYM_ITEM* alloc_reg=alloca_on_stack_value(ir_bb,tmpsi);
                fill_in_init_value(tmpsi,stor_value,false,alloc_reg,ir_bb);
            }
            printf("%s\n",tmpsi->value);
            for(size_t i=0;i<VECLEN(((VEC*)(tmpsi->data_field->pointer)));++i)
            {
                AST_BASE* init_node=VEC_GET_ITEM(((VEC*)(tmpsi->data_field->pointer)),i);
                print_type_vec(init_node->init_attribute->type_vec);
                printf("%ld %ld\n",init_node->init_attribute->off,init_node->init_attribute->size);
            }
        }
        VECinsert(irm->static_stor_symbols,(void*)stor_value);
    }
    m_free(tei);
    return true;
error:
    return false;
}
bool fill_in_init_value(SYM_ITEM* symbol,STOR_VALUE* value,bool static_stor,SYM_ITEM* alloc_reg,IR_BB* ir_bb)
{
    printf("fill in init\n");
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
    size_t veclen=VECLEN(((VEC*)symbol->data_field->pointer));
    for(size_t i=0;i<veclen;++i)
    {
        printf("i%ld\n",i);
        AST_BASE* initializer_node=VEC_GET_ITEM( ((VEC*)symbol->data_field->pointer) ,i);
        AST_BASE* sub_node=AST_GET_CHILD(initializer_node,0);
        if(!IS_EXPR_NODE(sub_node->type))
        {
            printf("initializer node not have a expr\n");  /*impossible*/
            goto error;
        }
        STOR_VALUE_ELEM* elem=m_alloc(sizeof(STOR_VALUE_ELEM));
        elem->bit_field_init_attr=NULL;
        elem->value_data_type=SSVT_NONE;
        elem->byte_width=elem->data=0;
        VECinsert(value->value_vec,(void*)elem);
        if(initializer_node->symbol->const_expr)
        {
            elem->byte_width=Type_size(initializer_node->symbol->type_vec);
            elem->value_data_type=SSVT_NONE;
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
            /*get the load position reg*/
            SYM_ITEM* load_posi_symbol=Create_symbol_item(tmp_symbol_str_alloc(".reg."),NMSP_DEFAULT);
            if(!insert_symbol(alloc_reg->symbol_table,load_posi_symbol))
                goto error;
            VECappend(symbol->type_vec,load_posi_symbol->type_vec);
            M_TYPE* pointer_type=build_base_type(TP_POINT);
            VECinsert(load_posi_symbol->type_vec,(void*)pointer_type);
            load_posi_symbol->stor_type=IR_STOR_STACK;
            load_posi_symbol->data_field->slong=(signed long int)(alloc_reg->data_field->slong+(initializer_node->init_attribute->off)/8);
            
            IR_INS* store_ins=add_new_ins(ir_bb);
            insert_ins_to_bb(store_ins,ir_bb);
            if(initializer_node->init_attribute->size!=Type_size(initializer_node->init_attribute->type_vec))    /*trunc*/
            {
                IR_INS* trunc_ins=add_new_ins(ir_bb);
                insert_ins_to_bb(trunc_ins,ir_bb);
                //GenINS(trunc_ins,OP_TRUNC,,,NULL);
            }
            if((initializer_node->init_attribute->off)%8!=0)
            {   /*shift to left*/
                IR_INS* shift_ins=add_new_ins(ir_bb);
                insert_ins_to_bb(shift_ins,ir_bb);

                //GenINS(shift_ins,OP_SHL,,,NULL);
            }
            GenINS(store_ins,OP_STORE,NULL,load_posi_symbol,NULL);
        }
        /*
            if two init nodes or a init and the end have a padding,just insert zero space
            but that only used for static storage
        */
        if(static_stor)
        {
            size_t off=initializer_node->init_attribute->off+initializer_node->init_attribute->size;
            off=MCC_ALIGN(off,8);
            size_t next_begin_off;
            if(i==veclen-1)
                next_begin_off=Type_size(symbol->type_vec)*8;
            else
            {
                AST_BASE* next_initializer_node=VEC_GET_ITEM( ((VEC*)symbol->data_field->pointer) ,i);
                next_begin_off=next_initializer_node->init_attribute->off;
            }
            if(next_begin_off-off>8)
            {
                elem=m_alloc(sizeof(STOR_VALUE_ELEM));
                elem->byte_width=0;
                elem->value_data_type=SSVT_NONE;
                elem->data=(next_begin_off-off)/8;
                VECinsert(value->value_vec,(void*)elem);
            }
        }
        
    }
    /*if a const expr, and a bit field, merge it*/
    for(size_t i=0;i<VECLEN(value->value_vec);++i)
    {
        STOR_VALUE_ELEM* tmp_elem=VEC_GET_ITEM(value->value_vec,i);
        if(!tmp_elem->bit_field_init_attr)
            continue;
        
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