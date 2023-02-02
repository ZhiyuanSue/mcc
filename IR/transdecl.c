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
        if(tmpsi->stor_type==IR_STOR_STATIC||tmpsi->stor_type==IR_STOR_THREAD){
            /*
                if static storage symbol, calculate the const value to fill in data
                and all the init node must be const value
            */
            STATIC_STOR_VALUE* value=(STATIC_STOR_VALUE*)m_alloc(sizeof(STATIC_STOR_VALUE));
            value->sym_item=tmpsi;

            value->value_vec=InitVEC(DEFAULT_CAPICITY);

            size_t value_size=Type_size(tmpsi->type_vec);
            value->value_data=m_alloc(value_size);
            memset(value->value_data,0,value_size);
            if(AST_CHILD_NUM(init_decl_node)==3){
                AST_BASE* initializer_node=AST_GET_CHILD(init_decl_node,2);
                if(!fill_in_static_stor_value(initializer_node,value))
                    goto error;
            }
            VECinsert(irm->static_stor_symbols,(void*)value);
        }
        else if(tmpsi->stor_type==IR_STOR_STACK){
            alloca_on_stack_value(decl_node,irm,ir_func,ir_bb,tmpsi);
        }
    }
    m_free(tei);
    return true;
error:
    return false;
}
bool fill_in_static_stor_value(AST_BASE* initializer_node,STATIC_STOR_VALUE* value)
{
    /*as we have calculated the off and the size of one initializer node,just fill data in it*/
    if(!initializer_node||!value||initializer_node->type!=initializer)
        goto error;
    ERROR_ITEM* tei=(ERROR_ITEM*)m_alloc(sizeof(ERROR_ITEM));
    AST_BASE* sub_node=AST_GET_CHILD(initializer_node,0);
    AST_BASE* initializer_list_node=NULL;
    if(sub_node->type==postfix_expr){
        AST_BASE* sub_sub_node=AST_GET_CHILD(sub_node,1);
        if(sub_sub_node&&sub_sub_node->type==type_name){
            initializer_list_node=AST_GET_CHILD(sub_node,4);
        }
    }
    else if(sub_node->type==left_brace)
        initializer_list_node=AST_GET_CHILD(initializer_node,1);
    if(initializer_list_node){
        for(size_t i=0;i<AST_CHILD_NUM(initializer_list_node);++i){
            AST_BASE* sub_init_node=AST_GET_CHILD(initializer_list_node,i);
            if(sub_init_node->type==initializer){
                fill_in_static_stor_value(sub_init_node,value);
            }
        }
    }
    else{
        if(!IS_EXPR_NODE(sub_node->type)||!(sub_node->symbol->const_expr))
        {
            C_ERROR(C0097_ERR_STATIC_STOR_CONST,sub_node);
            goto error;
        }
        /*then fill in and trunc(if bit field)*/
        INIT_NODE_ATTR* init_attr=initializer_node->init_attribute;
        M_TYPE* tmp_type=Type_VEC_get_actual_base_type(init_attr->type_vec);
        if(!tmp_type||!IS_SCALAR_TYPE(tmp_type->typ_category))
        {
            print_type_vec(init_attr->type_vec);
            printf("not a terminal initializer,error\n");
            goto error; /*actually impossible if everything ok*/
        }
        /*first,try to cast the data*/
        /*not bit field case*/
        size_t data_spec_size=type_data_size[tmp_type->typ_category];
        if((8*data_spec_size)==init_attr->size&&(init_attr->off)%8==0)
        {
            /*fill in*/
            STATIC_STOR_VALUE_ELEM* elem=m_alloc(data_spec_size);
            elem->byte_width=data_spec_size;
            if(tmp_type->typ_category==TP_POINT){
                AST_BASE* sub_sub_node=AST_GET_CHILD(sub_node,0);
                if(sub_node->type==primary_expression&&sub_sub_node&&sub_sub_node->type==identifier){
                    elem->value_data_type=SSVT_POINTER;
                    
                }
                else{
                    elem->value_data_type=SSVT_NONE;
                }
                
            }
            else{
                elem->value_data_type=SSVT_NONE;
                elem->data=value+(init_attr->off)/8;
            }
            /*in struct, there might have some space between two elements*/
            if(init_attr->off!=0){
                
            }
            VECinsert(value->value_vec,(void*)elem);
        }/*bit field case:emmm...actually,I have no idea of how to trunc*/
        else{
            /*use a new tmp data to store the value*/

            /*use 'and' to trunc*/
            
            /*shift it according to the off*/
            
            /*use 'or' to fill in the data*/
        }
    }
    m_free(tei);
    return true;
error:
    return false;
}
bool alloca_on_stack_value(AST_BASE* ast_node,IR_MODULE* irm,IR_FUNC* ir_func,IR_BB* ir_bb,SYM_ITEM* tmpsi)
{
    if(!ast_node||!irm||!ir_func||!ir_bb||!tmpsi)
        goto error;
    /*not static storage,generate code to fill in the data*/
    IR_INS* alloca_ins=add_new_ins(ir_bb);
    insert_ins_to_bb(alloca_ins,ir_bb);
    VEC* tmp_type_vec=tmpsi->type_vec;

    size_t alloca_size=Type_size(tmpsi->type_vec); 
    size_t alloca_align=Type_align(tmpsi->type_vec);

    GenINS(alloca_ins,OP_ALLOCA,ast_node->symbol,ast_node->symbol,NULL);

    /*fill the data in the reg of pointer */
    stack_off=MCC_ALIGN(stack_off,alloca_align);
    ast_node->symbol->stor_type=IR_STOR_REG;
    stack_off+=alloca_size;


    /*TODO:if have initializer part, init it,but this time need a insert instructions*/


    return true;
error:
    return false;
}