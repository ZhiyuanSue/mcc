#include "m_type.h"
bool Type_VEC_cmp(VEC* a,VEC* b);
bool Type_item_cmp(void* a,void* b);
bool Type_VEC_VM(VEC* type_vec);
bool Type_VEC_VLA(VEC* type_vec);
bool Type_VEC_modify_lvalue(VEC* type_vec,bool is_lvalue);
size_t Type_size(VEC* type_vec);
size_t Type_align(VEC* type_vec);
M_TYPE* Type_VEC_get_qual(VEC* type_vec);
M_TYPE* Type_composite_qual(M_TYPE* qual_a,M_TYPE* qual_b);
M_TYPE* Type_VEC_get_spec_other(VEC* type_vec); /*get the storage class/ align spec/ function spec type node*/
M_TYPE* Type_VEC_get_actual_base_type(VEC* type_vec);
bool Type_VEC_change_actual_base_type(VEC* type_vec,M_TYPE* tmptype);
M_TYPE* Type_VEC_get_Atomic(VEC* type_vec);

VEC* Type_VEC_get_Atomic_TO(VEC* type_vec,bool is_atomic);
VEC* Type_VEC_get_Pointer_TO(VEC* type_vec,bool is_pointer);
VEC* Type_VEC_get_Array_TO(VEC* type_vec,bool is_array);
VEC* Type_VEC_get_func_return_type(VEC* type_vec,bool is_function);
VEC* Type_VEC_unqualifier(VEC* type_vec,bool is_qual);

VEC* lvalue_convertion(VEC* tmp_type_vec);
bool compatible_types(VEC* type_vec_a,VEC* type_vec_b);
VEC* composite_types(VEC* type_vec_a,VEC* type_vec_b,bool compatible_type);
M_TYPE* usual_arith_conversion(M_TYPE** operand_a,M_TYPE** operand_b);
bool integer_promotion(M_TYPE** integer_operand);
bool float_promotion(M_TYPE** float_operand);
void print_type_vec(VEC* type_vec);