require dasbind public
options gen2
module z3_func shared
require smt/bindings/z3_struct
require smt/bindings/z3_enum
let @extern_library z3 = "libz3.dll"

[extern(cdecl, name="Z3_global_param_set", lib=z3)]
def Z3_global_param_set(param_id : string implicit; param_value : string implicit) : void {}
[extern(cdecl, name="Z3_global_param_reset_all", lib=z3)]
def Z3_global_param_reset_all() : void {}
[extern(cdecl, name="Z3_global_param_get", lib=z3)]
def Z3_global_param_get(param_id : string implicit; param_value : string? implicit) : bool {}
[extern(cdecl, name="Z3_mk_config", lib=z3)]
def Z3_mk_config() : _Z3_config? {}
[extern(cdecl, name="Z3_del_config", lib=z3)]
def Z3_del_config(c : _Z3_config? implicit) : void {}
[extern(cdecl, name="Z3_set_param_value", lib=z3)]
def Z3_set_param_value(c : _Z3_config? implicit; param_id : string implicit; param_value : string implicit) : void {}
[extern(cdecl, name="Z3_mk_context", lib=z3)]
def Z3_mk_context(c : _Z3_config? implicit) : _Z3_context? {}
[extern(cdecl, name="Z3_mk_context_rc", lib=z3)]
def Z3_mk_context_rc(c : _Z3_config? implicit) : _Z3_context? {}
[extern(cdecl, name="Z3_del_context", lib=z3)]
def Z3_del_context(c : _Z3_context? implicit) : void {}
[extern(cdecl, name="Z3_inc_ref", lib=z3)]
def Z3_inc_ref(c : _Z3_context? implicit; a : _Z3_ast? implicit) : void {}
[extern(cdecl, name="Z3_dec_ref", lib=z3)]
def Z3_dec_ref(c : _Z3_context? implicit; a : _Z3_ast? implicit) : void {}
[extern(cdecl, name="Z3_update_param_value", lib=z3)]
def Z3_update_param_value(c : _Z3_context? implicit; param_id : string implicit; param_value : string implicit) : void {}
[extern(cdecl, name="Z3_get_global_param_descrs", lib=z3)]
def Z3_get_global_param_descrs(c : _Z3_context? implicit) : _Z3_param_descrs? {}
[extern(cdecl, name="Z3_interrupt", lib=z3)]
def Z3_interrupt(c : _Z3_context? implicit) : void {}
[extern(cdecl, name="Z3_enable_concurrent_dec_ref", lib=z3)]
def Z3_enable_concurrent_dec_ref(c : _Z3_context? implicit) : void {}
[extern(cdecl, name="Z3_mk_params", lib=z3)]
def Z3_mk_params(c : _Z3_context? implicit) : _Z3_params? {}
[extern(cdecl, name="Z3_params_inc_ref", lib=z3)]
def Z3_params_inc_ref(c : _Z3_context? implicit; p : _Z3_params? implicit) : void {}
[extern(cdecl, name="Z3_params_dec_ref", lib=z3)]
def Z3_params_dec_ref(c : _Z3_context? implicit; p : _Z3_params? implicit) : void {}
[extern(cdecl, name="Z3_params_set_bool", lib=z3)]
def Z3_params_set_bool(c : _Z3_context? implicit; p : _Z3_params? implicit; k : _Z3_symbol? implicit; v : bool) : void {}
[extern(cdecl, name="Z3_params_set_uint", lib=z3)]
def Z3_params_set_uint(c : _Z3_context? implicit; p : _Z3_params? implicit; k : _Z3_symbol? implicit; v : uint) : void {}
[extern(cdecl, name="Z3_params_set_double", lib=z3)]
def Z3_params_set_double(c : _Z3_context? implicit; p : _Z3_params? implicit; k : _Z3_symbol? implicit; v : double) : void {}
[extern(cdecl, name="Z3_params_set_symbol", lib=z3)]
def Z3_params_set_symbol(c : _Z3_context? implicit; p : _Z3_params? implicit; k : _Z3_symbol? implicit; v : _Z3_symbol? implicit) : void {}
[extern(cdecl, name="Z3_params_to_string", lib=z3)]
def Z3_params_to_string(c : _Z3_context? implicit; p : _Z3_params? implicit) : string {}
[extern(cdecl, name="Z3_params_validate", lib=z3)]
def Z3_params_validate(c : _Z3_context? implicit; p : _Z3_params? implicit; d : _Z3_param_descrs? implicit) : void {}
[extern(cdecl, name="Z3_param_descrs_inc_ref", lib=z3)]
def Z3_param_descrs_inc_ref(c : _Z3_context? implicit; p : _Z3_param_descrs? implicit) : void {}
[extern(cdecl, name="Z3_param_descrs_dec_ref", lib=z3)]
def Z3_param_descrs_dec_ref(c : _Z3_context? implicit; p : _Z3_param_descrs? implicit) : void {}
[extern(cdecl, name="Z3_param_descrs_get_kind", lib=z3)]
def Z3_param_descrs_get_kind(c : _Z3_context? implicit; p : _Z3_param_descrs? implicit; n : _Z3_symbol? implicit) : Z3_param_kind {}
[extern(cdecl, name="Z3_param_descrs_size", lib=z3)]
def Z3_param_descrs_size(c : _Z3_context? implicit; p : _Z3_param_descrs? implicit) : uint {}
[extern(cdecl, name="Z3_param_descrs_get_name", lib=z3)]
def Z3_param_descrs_get_name(c : _Z3_context? implicit; p : _Z3_param_descrs? implicit; i : uint) : _Z3_symbol? {}
[extern(cdecl, name="Z3_param_descrs_get_documentation", lib=z3)]
def Z3_param_descrs_get_documentation(c : _Z3_context? implicit; p : _Z3_param_descrs? implicit; s : _Z3_symbol? implicit) : string {}
[extern(cdecl, name="Z3_param_descrs_to_string", lib=z3)]
def Z3_param_descrs_to_string(c : _Z3_context? implicit; p : _Z3_param_descrs? implicit) : string {}
[extern(cdecl, name="Z3_mk_int_symbol", lib=z3)]
def Z3_mk_int_symbol(c : _Z3_context? implicit; i : int) : _Z3_symbol? {}
[extern(cdecl, name="Z3_mk_string_symbol", lib=z3)]
def Z3_mk_string_symbol(c : _Z3_context? implicit; s : string implicit) : _Z3_symbol? {}
[extern(cdecl, name="Z3_mk_uninterpreted_sort", lib=z3)]
def Z3_mk_uninterpreted_sort(c : _Z3_context? implicit; s : _Z3_symbol? implicit) : _Z3_sort? {}
[extern(cdecl, name="Z3_mk_type_variable", lib=z3)]
def Z3_mk_type_variable(c : _Z3_context? implicit; s : _Z3_symbol? implicit) : _Z3_sort? {}
[extern(cdecl, name="Z3_mk_bool_sort", lib=z3)]
def Z3_mk_bool_sort(c : _Z3_context? implicit) : _Z3_sort? {}
[extern(cdecl, name="Z3_mk_int_sort", lib=z3)]
def Z3_mk_int_sort(c : _Z3_context? implicit) : _Z3_sort? {}
[extern(cdecl, name="Z3_mk_real_sort", lib=z3)]
def Z3_mk_real_sort(c : _Z3_context? implicit) : _Z3_sort? {}
[extern(cdecl, name="Z3_mk_bv_sort", lib=z3)]
def Z3_mk_bv_sort(c : _Z3_context? implicit; sz : uint) : _Z3_sort? {}
[extern(cdecl, name="Z3_mk_finite_domain_sort", lib=z3)]
def Z3_mk_finite_domain_sort(c : _Z3_context? implicit; name : _Z3_symbol? implicit; size : uint64) : _Z3_sort? {}
[extern(cdecl, name="Z3_mk_array_sort", lib=z3)]
def Z3_mk_array_sort(c : _Z3_context? implicit; domain : _Z3_sort? implicit; _range : _Z3_sort? implicit) : _Z3_sort? {}
[extern(cdecl, name="Z3_mk_array_sort_n", lib=z3)]
def Z3_mk_array_sort_n(c : _Z3_context? implicit; n : uint; domain : _Z3_sort? const? implicit; _range : _Z3_sort? implicit) : _Z3_sort? {}
[extern(cdecl, name="Z3_mk_tuple_sort", lib=z3)]
def Z3_mk_tuple_sort(c : _Z3_context? implicit; mk_tuple_name : _Z3_symbol? implicit; num_fields : uint; field_names : void? const; field_sorts : void? const; mk_tuple_decl : _Z3_func_decl ?? implicit; proj_decl : void?) : _Z3_sort? {}
[extern(cdecl, name="Z3_mk_enumeration_sort", lib=z3)]
def Z3_mk_enumeration_sort(c : _Z3_context? implicit; name : _Z3_symbol? implicit; n : uint; enum_names : void? const; enum_consts : void?; enum_testers : void?) : _Z3_sort? {}
[extern(cdecl, name="Z3_mk_list_sort", lib=z3)]
def Z3_mk_list_sort(c : _Z3_context? implicit; name : _Z3_symbol? implicit; elem_sort : _Z3_sort? implicit; nil_decl : _Z3_func_decl ?? implicit; is_nil_decl : _Z3_func_decl ?? implicit; cons_decl : _Z3_func_decl ?? implicit; is_cons_decl : _Z3_func_decl ?? implicit; head_decl : _Z3_func_decl ?? implicit; tail_decl : _Z3_func_decl ?? implicit) : _Z3_sort? {}
[extern(cdecl, name="Z3_mk_constructor", lib=z3)]
def Z3_mk_constructor(c : _Z3_context? implicit; name : _Z3_symbol? implicit; recognizer : _Z3_symbol? implicit; num_fields : uint; field_names : void? const; sorts : void? const; sort_refs : void?) : _Z3_constructor? {}
[extern(cdecl, name="Z3_constructor_num_fields", lib=z3)]
def Z3_constructor_num_fields(c : _Z3_context? implicit; constr : _Z3_constructor? implicit) : uint {}
[extern(cdecl, name="Z3_del_constructor", lib=z3)]
def Z3_del_constructor(c : _Z3_context? implicit; constr : _Z3_constructor? implicit) : void {}
[extern(cdecl, name="Z3_mk_datatype", lib=z3)]
def Z3_mk_datatype(c : _Z3_context? implicit; name : _Z3_symbol? implicit; num_constructors : uint; constructors : void?) : _Z3_sort? {}
[extern(cdecl, name="Z3_mk_polymorphic_datatype", lib=z3)]
def Z3_mk_polymorphic_datatype(c : _Z3_context? implicit; name : _Z3_symbol? implicit; num_parameters : uint; parameters : void?; num_constructors : uint; constructors : void?) : _Z3_sort? {}
[extern(cdecl, name="Z3_mk_datatype_sort", lib=z3)]
def Z3_mk_datatype_sort(c : _Z3_context? implicit; name : _Z3_symbol? implicit; num_params : uint; params : void? const) : _Z3_sort? {}
[extern(cdecl, name="Z3_mk_constructor_list", lib=z3)]
def Z3_mk_constructor_list(c : _Z3_context? implicit; num_constructors : uint; constructors : void? const) : _Z3_constructor_list? {}
[extern(cdecl, name="Z3_del_constructor_list", lib=z3)]
def Z3_del_constructor_list(c : _Z3_context? implicit; clist : _Z3_constructor_list? implicit) : void {}
[extern(cdecl, name="Z3_mk_datatypes", lib=z3)]
def Z3_mk_datatypes(c : _Z3_context? implicit; num_sorts : uint; sort_names : void? const; sorts : void?; constructor_lists : void?) : void {}
[extern(cdecl, name="Z3_query_constructor", lib=z3)]
def Z3_query_constructor(c : _Z3_context? implicit; constr : _Z3_constructor? implicit; num_fields : uint; constructor : _Z3_func_decl ?? implicit; tester : _Z3_func_decl ?? implicit; accessors : void?) : void {}
[extern(cdecl, name="Z3_mk_func_decl", lib=z3)]
def Z3_mk_func_decl(c : _Z3_context? implicit; s : _Z3_symbol? implicit; domain_size : uint; domain : void? const; _range : _Z3_sort? implicit) : _Z3_func_decl? {}
[extern(cdecl, name="Z3_mk_app", lib=z3)]
def Z3_mk_app(c : _Z3_context? implicit; d : _Z3_func_decl? implicit; num_args : uint; args : void? const) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_const", lib=z3)]
def Z3_mk_const(c : _Z3_context? implicit; s : _Z3_symbol? implicit; ty : _Z3_sort? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fresh_func_decl", lib=z3)]
def Z3_mk_fresh_func_decl(c : _Z3_context? implicit; prefix : string implicit; domain_size : uint; domain : void? const; _range : _Z3_sort? implicit) : _Z3_func_decl? {}
[extern(cdecl, name="Z3_mk_fresh_const", lib=z3)]
def Z3_mk_fresh_const(c : _Z3_context? implicit; prefix : string implicit; ty : _Z3_sort? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_rec_func_decl", lib=z3)]
def Z3_mk_rec_func_decl(c : _Z3_context? implicit; s : _Z3_symbol? implicit; domain_size : uint; domain : void? const; _range : _Z3_sort? implicit) : _Z3_func_decl? {}
[extern(cdecl, name="Z3_add_rec_def", lib=z3)]
def Z3_add_rec_def(c : _Z3_context? implicit; f : _Z3_func_decl? implicit; n : uint; args : void?; body : _Z3_ast? implicit) : void {}
[extern(cdecl, name="Z3_mk_true", lib=z3)]
def Z3_mk_true(c : _Z3_context? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_false", lib=z3)]
def Z3_mk_false(c : _Z3_context? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_eq", lib=z3)]
def Z3_mk_eq(c : _Z3_context? implicit; l : _Z3_ast? implicit; r : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_distinct", lib=z3)]
def Z3_mk_distinct(c : _Z3_context? implicit; num_args : uint; args : void? const) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_not", lib=z3)]
def Z3_mk_not(c : _Z3_context? implicit; a : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_ite", lib=z3)]
def Z3_mk_ite(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit; t3 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_iff", lib=z3)]
def Z3_mk_iff(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_implies", lib=z3)]
def Z3_mk_implies(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_xor", lib=z3)]
def Z3_mk_xor(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_and", lib=z3)]
def Z3_mk_and(c : _Z3_context? implicit; num_args : uint; args : void? const) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_or", lib=z3)]
def Z3_mk_or(c : _Z3_context? implicit; num_args : uint; args : void? const) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_add", lib=z3)]
def Z3_mk_add(c : _Z3_context? implicit; num_args : uint; args : void? const) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_mul", lib=z3)]
def Z3_mk_mul(c : _Z3_context? implicit; num_args : uint; args : void? const) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_sub", lib=z3)]
def Z3_mk_sub(c : _Z3_context? implicit; num_args : uint; args : void? const) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_unary_minus", lib=z3)]
def Z3_mk_unary_minus(c : _Z3_context? implicit; arg : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_div", lib=z3)]
def Z3_mk_div(c : _Z3_context? implicit; arg1 : _Z3_ast? implicit; arg2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_mod", lib=z3)]
def Z3_mk_mod(c : _Z3_context? implicit; arg1 : _Z3_ast? implicit; arg2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_rem", lib=z3)]
def Z3_mk_rem(c : _Z3_context? implicit; arg1 : _Z3_ast? implicit; arg2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_power", lib=z3)]
def Z3_mk_power(c : _Z3_context? implicit; arg1 : _Z3_ast? implicit; arg2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_abs", lib=z3)]
def Z3_mk_abs(c : _Z3_context? implicit; arg : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_lt", lib=z3)]
def Z3_mk_lt(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_le", lib=z3)]
def Z3_mk_le(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_gt", lib=z3)]
def Z3_mk_gt(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_ge", lib=z3)]
def Z3_mk_ge(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_divides", lib=z3)]
def Z3_mk_divides(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_int2real", lib=z3)]
def Z3_mk_int2real(c : _Z3_context? implicit; t1 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_real2int", lib=z3)]
def Z3_mk_real2int(c : _Z3_context? implicit; t1 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_is_int", lib=z3)]
def Z3_mk_is_int(c : _Z3_context? implicit; t1 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvnot", lib=z3)]
def Z3_mk_bvnot(c : _Z3_context? implicit; t1 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvredand", lib=z3)]
def Z3_mk_bvredand(c : _Z3_context? implicit; t1 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvredor", lib=z3)]
def Z3_mk_bvredor(c : _Z3_context? implicit; t1 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvand", lib=z3)]
def Z3_mk_bvand(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvor", lib=z3)]
def Z3_mk_bvor(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvxor", lib=z3)]
def Z3_mk_bvxor(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvnand", lib=z3)]
def Z3_mk_bvnand(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvnor", lib=z3)]
def Z3_mk_bvnor(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvxnor", lib=z3)]
def Z3_mk_bvxnor(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvneg", lib=z3)]
def Z3_mk_bvneg(c : _Z3_context? implicit; t1 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvadd", lib=z3)]
def Z3_mk_bvadd(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvsub", lib=z3)]
def Z3_mk_bvsub(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvmul", lib=z3)]
def Z3_mk_bvmul(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvudiv", lib=z3)]
def Z3_mk_bvudiv(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvsdiv", lib=z3)]
def Z3_mk_bvsdiv(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvurem", lib=z3)]
def Z3_mk_bvurem(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvsrem", lib=z3)]
def Z3_mk_bvsrem(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvsmod", lib=z3)]
def Z3_mk_bvsmod(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvult", lib=z3)]
def Z3_mk_bvult(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvslt", lib=z3)]
def Z3_mk_bvslt(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvule", lib=z3)]
def Z3_mk_bvule(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvsle", lib=z3)]
def Z3_mk_bvsle(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvuge", lib=z3)]
def Z3_mk_bvuge(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvsge", lib=z3)]
def Z3_mk_bvsge(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvugt", lib=z3)]
def Z3_mk_bvugt(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvsgt", lib=z3)]
def Z3_mk_bvsgt(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_concat", lib=z3)]
def Z3_mk_concat(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_extract", lib=z3)]
def Z3_mk_extract(c : _Z3_context? implicit; high : uint; low : uint; t1 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_sign_ext", lib=z3)]
def Z3_mk_sign_ext(c : _Z3_context? implicit; i : uint; t1 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_zero_ext", lib=z3)]
def Z3_mk_zero_ext(c : _Z3_context? implicit; i : uint; t1 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_repeat", lib=z3)]
def Z3_mk_repeat(c : _Z3_context? implicit; i : uint; t1 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bit2bool", lib=z3)]
def Z3_mk_bit2bool(c : _Z3_context? implicit; i : uint; t1 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvshl", lib=z3)]
def Z3_mk_bvshl(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvlshr", lib=z3)]
def Z3_mk_bvlshr(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvashr", lib=z3)]
def Z3_mk_bvashr(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_rotate_left", lib=z3)]
def Z3_mk_rotate_left(c : _Z3_context? implicit; i : uint; t1 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_rotate_right", lib=z3)]
def Z3_mk_rotate_right(c : _Z3_context? implicit; i : uint; t1 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_ext_rotate_left", lib=z3)]
def Z3_mk_ext_rotate_left(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_ext_rotate_right", lib=z3)]
def Z3_mk_ext_rotate_right(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_int2bv", lib=z3)]
def Z3_mk_int2bv(c : _Z3_context? implicit; n : uint; t1 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bv2int", lib=z3)]
def Z3_mk_bv2int(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; is_signed : bool) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvadd_no_overflow", lib=z3)]
def Z3_mk_bvadd_no_overflow(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit; is_signed : bool) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvadd_no_underflow", lib=z3)]
def Z3_mk_bvadd_no_underflow(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvsub_no_overflow", lib=z3)]
def Z3_mk_bvsub_no_overflow(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvsub_no_underflow", lib=z3)]
def Z3_mk_bvsub_no_underflow(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit; is_signed : bool) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvsdiv_no_overflow", lib=z3)]
def Z3_mk_bvsdiv_no_overflow(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvneg_no_overflow", lib=z3)]
def Z3_mk_bvneg_no_overflow(c : _Z3_context? implicit; t1 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvmul_no_overflow", lib=z3)]
def Z3_mk_bvmul_no_overflow(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit; is_signed : bool) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bvmul_no_underflow", lib=z3)]
def Z3_mk_bvmul_no_underflow(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_select", lib=z3)]
def Z3_mk_select(c : _Z3_context? implicit; a : _Z3_ast? implicit; i : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_select_n", lib=z3)]
def Z3_mk_select_n(c : _Z3_context? implicit; a : _Z3_ast? implicit; n : uint; idxs : _Z3_ast? const? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_store", lib=z3)]
def Z3_mk_store(c : _Z3_context? implicit; a : _Z3_ast? implicit; i : _Z3_ast? implicit; v : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_store_n", lib=z3)]
def Z3_mk_store_n(c : _Z3_context? implicit; a : _Z3_ast? implicit; n : uint; idxs : _Z3_ast? const? implicit; v : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_const_array", lib=z3)]
def Z3_mk_const_array(c : _Z3_context? implicit; domain : _Z3_sort? implicit; v : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_map", lib=z3)]
def Z3_mk_map(c : _Z3_context? implicit; f : _Z3_func_decl? implicit; n : uint; args : _Z3_ast? const? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_array_default", lib=z3)]
def Z3_mk_array_default(c : _Z3_context? implicit; _array : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_as_array", lib=z3)]
def Z3_mk_as_array(c : _Z3_context? implicit; f : _Z3_func_decl? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_set_has_size", lib=z3)]
def Z3_mk_set_has_size(c : _Z3_context? implicit; set : _Z3_ast? implicit; k : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_set_sort", lib=z3)]
def Z3_mk_set_sort(c : _Z3_context? implicit; ty : _Z3_sort? implicit) : _Z3_sort? {}
[extern(cdecl, name="Z3_mk_empty_set", lib=z3)]
def Z3_mk_empty_set(c : _Z3_context? implicit; domain : _Z3_sort? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_full_set", lib=z3)]
def Z3_mk_full_set(c : _Z3_context? implicit; domain : _Z3_sort? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_set_add", lib=z3)]
def Z3_mk_set_add(c : _Z3_context? implicit; set : _Z3_ast? implicit; elem : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_set_del", lib=z3)]
def Z3_mk_set_del(c : _Z3_context? implicit; set : _Z3_ast? implicit; elem : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_set_union", lib=z3)]
def Z3_mk_set_union(c : _Z3_context? implicit; num_args : uint; args : void? const) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_set_intersect", lib=z3)]
def Z3_mk_set_intersect(c : _Z3_context? implicit; num_args : uint; args : void? const) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_set_difference", lib=z3)]
def Z3_mk_set_difference(c : _Z3_context? implicit; arg1 : _Z3_ast? implicit; arg2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_set_complement", lib=z3)]
def Z3_mk_set_complement(c : _Z3_context? implicit; arg : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_set_member", lib=z3)]
def Z3_mk_set_member(c : _Z3_context? implicit; elem : _Z3_ast? implicit; set : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_set_subset", lib=z3)]
def Z3_mk_set_subset(c : _Z3_context? implicit; arg1 : _Z3_ast? implicit; arg2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_array_ext", lib=z3)]
def Z3_mk_array_ext(c : _Z3_context? implicit; arg1 : _Z3_ast? implicit; arg2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_numeral", lib=z3)]
def Z3_mk_numeral(c : _Z3_context? implicit; numeral : string implicit; ty : _Z3_sort? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_real", lib=z3)]
def Z3_mk_real(c : _Z3_context? implicit; num : int; den : int) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_real_int64", lib=z3)]
def Z3_mk_real_int64(c : _Z3_context? implicit; num : int64; den : int64) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_int", lib=z3)]
def Z3_mk_int(c : _Z3_context? implicit; v : int; ty : _Z3_sort? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_unsigned_int", lib=z3)]
def Z3_mk_unsigned_int(c : _Z3_context? implicit; v : uint; ty : _Z3_sort? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_int64", lib=z3)]
def Z3_mk_int64(c : _Z3_context? implicit; v : int64; ty : _Z3_sort? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_unsigned_int64", lib=z3)]
def Z3_mk_unsigned_int64(c : _Z3_context? implicit; v : uint64; ty : _Z3_sort? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_bv_numeral", lib=z3)]
def Z3_mk_bv_numeral(c : _Z3_context? implicit; sz : uint; bits : bool const? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_seq_sort", lib=z3)]
def Z3_mk_seq_sort(c : _Z3_context? implicit; s : _Z3_sort? implicit) : _Z3_sort? {}
[extern(cdecl, name="Z3_is_seq_sort", lib=z3)]
def Z3_is_seq_sort(c : _Z3_context? implicit; s : _Z3_sort? implicit) : bool {}
[extern(cdecl, name="Z3_get_seq_sort_basis", lib=z3)]
def Z3_get_seq_sort_basis(c : _Z3_context? implicit; s : _Z3_sort? implicit) : _Z3_sort? {}
[extern(cdecl, name="Z3_mk_re_sort", lib=z3)]
def Z3_mk_re_sort(c : _Z3_context? implicit; seq : _Z3_sort? implicit) : _Z3_sort? {}
[extern(cdecl, name="Z3_is_re_sort", lib=z3)]
def Z3_is_re_sort(c : _Z3_context? implicit; s : _Z3_sort? implicit) : bool {}
[extern(cdecl, name="Z3_get_re_sort_basis", lib=z3)]
def Z3_get_re_sort_basis(c : _Z3_context? implicit; s : _Z3_sort? implicit) : _Z3_sort? {}
[extern(cdecl, name="Z3_mk_string_sort", lib=z3)]
def Z3_mk_string_sort(c : _Z3_context? implicit) : _Z3_sort? {}
[extern(cdecl, name="Z3_mk_char_sort", lib=z3)]
def Z3_mk_char_sort(c : _Z3_context? implicit) : _Z3_sort? {}
[extern(cdecl, name="Z3_is_string_sort", lib=z3)]
def Z3_is_string_sort(c : _Z3_context? implicit; s : _Z3_sort? implicit) : bool {}
[extern(cdecl, name="Z3_is_char_sort", lib=z3)]
def Z3_is_char_sort(c : _Z3_context? implicit; s : _Z3_sort? implicit) : bool {}
[extern(cdecl, name="Z3_mk_string", lib=z3)]
def Z3_mk_string(c : _Z3_context? implicit; s : string implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_lstring", lib=z3)]
def Z3_mk_lstring(c : _Z3_context? implicit; len : uint; s : string implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_u32string", lib=z3)]
def Z3_mk_u32string(c : _Z3_context? implicit; len : uint; chars : void? const) : _Z3_ast? {}
[extern(cdecl, name="Z3_is_string", lib=z3)]
def Z3_is_string(c : _Z3_context? implicit; s : _Z3_ast? implicit) : bool {}
[extern(cdecl, name="Z3_get_string", lib=z3)]
def Z3_get_string(c : _Z3_context? implicit; s : _Z3_ast? implicit) : string {}
[extern(cdecl, name="Z3_get_lstring", lib=z3)]
def Z3_get_lstring(c : _Z3_context? implicit; s : _Z3_ast? implicit; length : uint? implicit) : string {}
[extern(cdecl, name="Z3_get_string_length", lib=z3)]
def Z3_get_string_length(c : _Z3_context? implicit; s : _Z3_ast? implicit) : uint {}
[extern(cdecl, name="Z3_get_string_contents", lib=z3)]
def Z3_get_string_contents(c : _Z3_context? implicit; s : _Z3_ast? implicit; length : uint; contents : void?) : void {}
[extern(cdecl, name="Z3_mk_seq_empty", lib=z3)]
def Z3_mk_seq_empty(c : _Z3_context? implicit; seq : _Z3_sort? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_seq_unit", lib=z3)]
def Z3_mk_seq_unit(c : _Z3_context? implicit; a : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_seq_concat", lib=z3)]
def Z3_mk_seq_concat(c : _Z3_context? implicit; n : uint; args : void? const) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_seq_prefix", lib=z3)]
def Z3_mk_seq_prefix(c : _Z3_context? implicit; prefix : _Z3_ast? implicit; s : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_seq_suffix", lib=z3)]
def Z3_mk_seq_suffix(c : _Z3_context? implicit; suffix : _Z3_ast? implicit; s : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_seq_contains", lib=z3)]
def Z3_mk_seq_contains(c : _Z3_context? implicit; container : _Z3_ast? implicit; containee : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_str_lt", lib=z3)]
def Z3_mk_str_lt(c : _Z3_context? implicit; prefix : _Z3_ast? implicit; s : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_str_le", lib=z3)]
def Z3_mk_str_le(c : _Z3_context? implicit; prefix : _Z3_ast? implicit; s : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_seq_extract", lib=z3)]
def Z3_mk_seq_extract(c : _Z3_context? implicit; s : _Z3_ast? implicit; offset : _Z3_ast? implicit; length : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_seq_replace", lib=z3)]
def Z3_mk_seq_replace(c : _Z3_context? implicit; s : _Z3_ast? implicit; src : _Z3_ast? implicit; dst : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_seq_at", lib=z3)]
def Z3_mk_seq_at(c : _Z3_context? implicit; s : _Z3_ast? implicit; index : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_seq_nth", lib=z3)]
def Z3_mk_seq_nth(c : _Z3_context? implicit; s : _Z3_ast? implicit; index : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_seq_length", lib=z3)]
def Z3_mk_seq_length(c : _Z3_context? implicit; s : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_seq_index", lib=z3)]
def Z3_mk_seq_index(c : _Z3_context? implicit; s : _Z3_ast? implicit; substr : _Z3_ast? implicit; offset : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_seq_last_index", lib=z3)]
def Z3_mk_seq_last_index(c : _Z3_context? implicit; s : _Z3_ast? implicit; substr : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_seq_map", lib=z3)]
def Z3_mk_seq_map(c : _Z3_context? implicit; f : _Z3_ast? implicit; s : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_seq_mapi", lib=z3)]
def Z3_mk_seq_mapi(c : _Z3_context? implicit; f : _Z3_ast? implicit; i : _Z3_ast? implicit; s : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_seq_foldl", lib=z3)]
def Z3_mk_seq_foldl(c : _Z3_context? implicit; f : _Z3_ast? implicit; a : _Z3_ast? implicit; s : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_seq_foldli", lib=z3)]
def Z3_mk_seq_foldli(c : _Z3_context? implicit; f : _Z3_ast? implicit; i : _Z3_ast? implicit; a : _Z3_ast? implicit; s : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_str_to_int", lib=z3)]
def Z3_mk_str_to_int(c : _Z3_context? implicit; s : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_int_to_str", lib=z3)]
def Z3_mk_int_to_str(c : _Z3_context? implicit; s : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_string_to_code", lib=z3)]
def Z3_mk_string_to_code(c : _Z3_context? implicit; a : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_string_from_code", lib=z3)]
def Z3_mk_string_from_code(c : _Z3_context? implicit; a : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_ubv_to_str", lib=z3)]
def Z3_mk_ubv_to_str(c : _Z3_context? implicit; s : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_sbv_to_str", lib=z3)]
def Z3_mk_sbv_to_str(c : _Z3_context? implicit; s : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_seq_to_re", lib=z3)]
def Z3_mk_seq_to_re(c : _Z3_context? implicit; seq : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_seq_in_re", lib=z3)]
def Z3_mk_seq_in_re(c : _Z3_context? implicit; seq : _Z3_ast? implicit; re : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_re_plus", lib=z3)]
def Z3_mk_re_plus(c : _Z3_context? implicit; re : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_re_star", lib=z3)]
def Z3_mk_re_star(c : _Z3_context? implicit; re : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_re_option", lib=z3)]
def Z3_mk_re_option(c : _Z3_context? implicit; re : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_re_union", lib=z3)]
def Z3_mk_re_union(c : _Z3_context? implicit; n : uint; args : void? const) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_re_concat", lib=z3)]
def Z3_mk_re_concat(c : _Z3_context? implicit; n : uint; args : void? const) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_re_range", lib=z3)]
def Z3_mk_re_range(c : _Z3_context? implicit; lo : _Z3_ast? implicit; hi : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_re_allchar", lib=z3)]
def Z3_mk_re_allchar(c : _Z3_context? implicit; regex_sort : _Z3_sort? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_re_loop", lib=z3)]
def Z3_mk_re_loop(c : _Z3_context? implicit; r : _Z3_ast? implicit; lo : uint; hi : uint) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_re_power", lib=z3)]
def Z3_mk_re_power(c : _Z3_context? implicit; re : _Z3_ast? implicit; n : uint) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_re_intersect", lib=z3)]
def Z3_mk_re_intersect(c : _Z3_context? implicit; n : uint; args : void? const) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_re_complement", lib=z3)]
def Z3_mk_re_complement(c : _Z3_context? implicit; re : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_re_diff", lib=z3)]
def Z3_mk_re_diff(c : _Z3_context? implicit; re1 : _Z3_ast? implicit; re2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_re_empty", lib=z3)]
def Z3_mk_re_empty(c : _Z3_context? implicit; re : _Z3_sort? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_re_full", lib=z3)]
def Z3_mk_re_full(c : _Z3_context? implicit; re : _Z3_sort? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_char", lib=z3)]
def Z3_mk_char(c : _Z3_context? implicit; ch : uint) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_char_le", lib=z3)]
def Z3_mk_char_le(c : _Z3_context? implicit; ch1 : _Z3_ast? implicit; ch2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_char_to_int", lib=z3)]
def Z3_mk_char_to_int(c : _Z3_context? implicit; ch : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_char_to_bv", lib=z3)]
def Z3_mk_char_to_bv(c : _Z3_context? implicit; ch : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_char_from_bv", lib=z3)]
def Z3_mk_char_from_bv(c : _Z3_context? implicit; bv : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_char_is_digit", lib=z3)]
def Z3_mk_char_is_digit(c : _Z3_context? implicit; ch : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_linear_order", lib=z3)]
def Z3_mk_linear_order(c : _Z3_context? implicit; a : _Z3_sort? implicit; id : uint) : _Z3_func_decl? {}
[extern(cdecl, name="Z3_mk_partial_order", lib=z3)]
def Z3_mk_partial_order(c : _Z3_context? implicit; a : _Z3_sort? implicit; id : uint) : _Z3_func_decl? {}
[extern(cdecl, name="Z3_mk_piecewise_linear_order", lib=z3)]
def Z3_mk_piecewise_linear_order(c : _Z3_context? implicit; a : _Z3_sort? implicit; id : uint) : _Z3_func_decl? {}
[extern(cdecl, name="Z3_mk_tree_order", lib=z3)]
def Z3_mk_tree_order(c : _Z3_context? implicit; a : _Z3_sort? implicit; id : uint) : _Z3_func_decl? {}
[extern(cdecl, name="Z3_mk_transitive_closure", lib=z3)]
def Z3_mk_transitive_closure(c : _Z3_context? implicit; f : _Z3_func_decl? implicit) : _Z3_func_decl? {}
[extern(cdecl, name="Z3_mk_pattern", lib=z3)]
def Z3_mk_pattern(c : _Z3_context? implicit; num_patterns : uint; terms : void? const) : _Z3_pattern? {}
[extern(cdecl, name="Z3_mk_bound", lib=z3)]
def Z3_mk_bound(c : _Z3_context? implicit; index : uint; ty : _Z3_sort? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_forall", lib=z3)]
def Z3_mk_forall(c : _Z3_context? implicit; weight : uint; num_patterns : uint; patterns : void? const; num_decls : uint; sorts : void? const; decl_names : void? const; body : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_exists", lib=z3)]
def Z3_mk_exists(c : _Z3_context? implicit; weight : uint; num_patterns : uint; patterns : void? const; num_decls : uint; sorts : void? const; decl_names : void? const; body : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_quantifier", lib=z3)]
def Z3_mk_quantifier(c : _Z3_context? implicit; is_forall : bool; weight : uint; num_patterns : uint; patterns : void? const; num_decls : uint; sorts : void? const; decl_names : void? const; body : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_quantifier_ex", lib=z3)]
def Z3_mk_quantifier_ex(c : _Z3_context? implicit; is_forall : bool; weight : uint; quantifier_id : _Z3_symbol? implicit; skolem_id : _Z3_symbol? implicit; num_patterns : uint; patterns : void? const; num_no_patterns : uint; no_patterns : void? const; num_decls : uint; sorts : void? const; decl_names : void? const; body : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_forall_const", lib=z3)]
def Z3_mk_forall_const(c : _Z3_context? implicit; weight : uint; num_bound : uint; bound : void? const; num_patterns : uint; patterns : void? const; body : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_exists_const", lib=z3)]
def Z3_mk_exists_const(c : _Z3_context? implicit; weight : uint; num_bound : uint; bound : void? const; num_patterns : uint; patterns : void? const; body : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_quantifier_const", lib=z3)]
def Z3_mk_quantifier_const(c : _Z3_context? implicit; is_forall : bool; weight : uint; num_bound : uint; bound : void? const; num_patterns : uint; patterns : void? const; body : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_quantifier_const_ex", lib=z3)]
def Z3_mk_quantifier_const_ex(c : _Z3_context? implicit; is_forall : bool; weight : uint; quantifier_id : _Z3_symbol? implicit; skolem_id : _Z3_symbol? implicit; num_bound : uint; bound : void? const; num_patterns : uint; patterns : void? const; num_no_patterns : uint; no_patterns : void? const; body : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_lambda", lib=z3)]
def Z3_mk_lambda(c : _Z3_context? implicit; num_decls : uint; sorts : void? const; decl_names : void? const; body : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_lambda_const", lib=z3)]
def Z3_mk_lambda_const(c : _Z3_context? implicit; num_bound : uint; bound : void? const; body : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_get_symbol_kind", lib=z3)]
def Z3_get_symbol_kind(c : _Z3_context? implicit; s : _Z3_symbol? implicit) : Z3_symbol_kind {}
[extern(cdecl, name="Z3_get_symbol_int", lib=z3)]
def Z3_get_symbol_int(c : _Z3_context? implicit; s : _Z3_symbol? implicit) : int {}
[extern(cdecl, name="Z3_get_symbol_string", lib=z3)]
def Z3_get_symbol_string(c : _Z3_context? implicit; s : _Z3_symbol? implicit) : string {}
[extern(cdecl, name="Z3_get_sort_name", lib=z3)]
def Z3_get_sort_name(c : _Z3_context? implicit; d : _Z3_sort? implicit) : _Z3_symbol? {}
[extern(cdecl, name="Z3_get_sort_id", lib=z3)]
def Z3_get_sort_id(c : _Z3_context? implicit; s : _Z3_sort? implicit) : uint {}
[extern(cdecl, name="Z3_sort_to_ast", lib=z3)]
def Z3_sort_to_ast(c : _Z3_context? implicit; s : _Z3_sort? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_is_eq_sort", lib=z3)]
def Z3_is_eq_sort(c : _Z3_context? implicit; s1 : _Z3_sort? implicit; s2 : _Z3_sort? implicit) : bool {}
[extern(cdecl, name="Z3_get_sort_kind", lib=z3)]
def Z3_get_sort_kind(c : _Z3_context? implicit; t : _Z3_sort? implicit) : Z3_sort_kind {}
[extern(cdecl, name="Z3_get_bv_sort_size", lib=z3)]
def Z3_get_bv_sort_size(c : _Z3_context? implicit; t : _Z3_sort? implicit) : uint {}
[extern(cdecl, name="Z3_get_finite_domain_sort_size", lib=z3)]
def Z3_get_finite_domain_sort_size(c : _Z3_context? implicit; s : _Z3_sort? implicit; r : uint64? implicit) : bool {}
[extern(cdecl, name="Z3_get_array_arity", lib=z3)]
def Z3_get_array_arity(c : _Z3_context? implicit; s : _Z3_sort? implicit) : uint {}
[extern(cdecl, name="Z3_get_array_sort_domain", lib=z3)]
def Z3_get_array_sort_domain(c : _Z3_context? implicit; t : _Z3_sort? implicit) : _Z3_sort? {}
[extern(cdecl, name="Z3_get_array_sort_domain_n", lib=z3)]
def Z3_get_array_sort_domain_n(c : _Z3_context? implicit; t : _Z3_sort? implicit; idx : uint) : _Z3_sort? {}
[extern(cdecl, name="Z3_get_array_sort_range", lib=z3)]
def Z3_get_array_sort_range(c : _Z3_context? implicit; t : _Z3_sort? implicit) : _Z3_sort? {}
[extern(cdecl, name="Z3_get_tuple_sort_mk_decl", lib=z3)]
def Z3_get_tuple_sort_mk_decl(c : _Z3_context? implicit; t : _Z3_sort? implicit) : _Z3_func_decl? {}
[extern(cdecl, name="Z3_get_tuple_sort_num_fields", lib=z3)]
def Z3_get_tuple_sort_num_fields(c : _Z3_context? implicit; t : _Z3_sort? implicit) : uint {}
[extern(cdecl, name="Z3_get_tuple_sort_field_decl", lib=z3)]
def Z3_get_tuple_sort_field_decl(c : _Z3_context? implicit; t : _Z3_sort? implicit; i : uint) : _Z3_func_decl? {}
[extern(cdecl, name="Z3_is_recursive_datatype_sort", lib=z3)]
def Z3_is_recursive_datatype_sort(c : _Z3_context? implicit; s : _Z3_sort? implicit) : bool {}
[extern(cdecl, name="Z3_get_datatype_sort_num_constructors", lib=z3)]
def Z3_get_datatype_sort_num_constructors(c : _Z3_context? implicit; t : _Z3_sort? implicit) : uint {}
[extern(cdecl, name="Z3_get_datatype_sort_constructor", lib=z3)]
def Z3_get_datatype_sort_constructor(c : _Z3_context? implicit; t : _Z3_sort? implicit; idx : uint) : _Z3_func_decl? {}
[extern(cdecl, name="Z3_get_datatype_sort_recognizer", lib=z3)]
def Z3_get_datatype_sort_recognizer(c : _Z3_context? implicit; t : _Z3_sort? implicit; idx : uint) : _Z3_func_decl? {}
[extern(cdecl, name="Z3_get_datatype_sort_constructor_accessor", lib=z3)]
def Z3_get_datatype_sort_constructor_accessor(c : _Z3_context? implicit; t : _Z3_sort? implicit; idx_c : uint; idx_a : uint) : _Z3_func_decl? {}
[extern(cdecl, name="Z3_datatype_update_field", lib=z3)]
def Z3_datatype_update_field(c : _Z3_context? implicit; field_access : _Z3_func_decl? implicit; t : _Z3_ast? implicit; value : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_get_relation_arity", lib=z3)]
def Z3_get_relation_arity(c : _Z3_context? implicit; s : _Z3_sort? implicit) : uint {}
[extern(cdecl, name="Z3_get_relation_column", lib=z3)]
def Z3_get_relation_column(c : _Z3_context? implicit; s : _Z3_sort? implicit; col : uint) : _Z3_sort? {}
[extern(cdecl, name="Z3_mk_atmost", lib=z3)]
def Z3_mk_atmost(c : _Z3_context? implicit; num_args : uint; args : void? const; k : uint) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_atleast", lib=z3)]
def Z3_mk_atleast(c : _Z3_context? implicit; num_args : uint; args : void? const; k : uint) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_pble", lib=z3)]
def Z3_mk_pble(c : _Z3_context? implicit; num_args : uint; args : void? const; coeffs : void? const; k : int) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_pbge", lib=z3)]
def Z3_mk_pbge(c : _Z3_context? implicit; num_args : uint; args : void? const; coeffs : void? const; k : int) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_pbeq", lib=z3)]
def Z3_mk_pbeq(c : _Z3_context? implicit; num_args : uint; args : void? const; coeffs : void? const; k : int) : _Z3_ast? {}
[extern(cdecl, name="Z3_func_decl_to_ast", lib=z3)]
def Z3_func_decl_to_ast(c : _Z3_context? implicit; f : _Z3_func_decl? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_is_eq_func_decl", lib=z3)]
def Z3_is_eq_func_decl(c : _Z3_context? implicit; f1 : _Z3_func_decl? implicit; f2 : _Z3_func_decl? implicit) : bool {}
[extern(cdecl, name="Z3_get_func_decl_id", lib=z3)]
def Z3_get_func_decl_id(c : _Z3_context? implicit; f : _Z3_func_decl? implicit) : uint {}
[extern(cdecl, name="Z3_get_decl_name", lib=z3)]
def Z3_get_decl_name(c : _Z3_context? implicit; d : _Z3_func_decl? implicit) : _Z3_symbol? {}
[extern(cdecl, name="Z3_get_decl_kind", lib=z3)]
def Z3_get_decl_kind(c : _Z3_context? implicit; d : _Z3_func_decl? implicit) : Z3_decl_kind {}
[extern(cdecl, name="Z3_get_domain_size", lib=z3)]
def Z3_get_domain_size(c : _Z3_context? implicit; d : _Z3_func_decl? implicit) : uint {}
[extern(cdecl, name="Z3_get_arity", lib=z3)]
def Z3_get_arity(c : _Z3_context? implicit; d : _Z3_func_decl? implicit) : uint {}
[extern(cdecl, name="Z3_get_domain", lib=z3)]
def Z3_get_domain(c : _Z3_context? implicit; d : _Z3_func_decl? implicit; i : uint) : _Z3_sort? {}
[extern(cdecl, name="Z3_get_range", lib=z3)]
def Z3_get_range(c : _Z3_context? implicit; d : _Z3_func_decl? implicit) : _Z3_sort? {}
[extern(cdecl, name="Z3_get_decl_num_parameters", lib=z3)]
def Z3_get_decl_num_parameters(c : _Z3_context? implicit; d : _Z3_func_decl? implicit) : uint {}
[extern(cdecl, name="Z3_get_decl_parameter_kind", lib=z3)]
def Z3_get_decl_parameter_kind(c : _Z3_context? implicit; d : _Z3_func_decl? implicit; idx : uint) : Z3_parameter_kind {}
[extern(cdecl, name="Z3_get_decl_int_parameter", lib=z3)]
def Z3_get_decl_int_parameter(c : _Z3_context? implicit; d : _Z3_func_decl? implicit; idx : uint) : int {}
[extern(cdecl, name="Z3_get_decl_double_parameter", lib=z3)]
def Z3_get_decl_double_parameter(c : _Z3_context? implicit; d : _Z3_func_decl? implicit; idx : uint) : double {}
[extern(cdecl, name="Z3_get_decl_symbol_parameter", lib=z3)]
def Z3_get_decl_symbol_parameter(c : _Z3_context? implicit; d : _Z3_func_decl? implicit; idx : uint) : _Z3_symbol? {}
[extern(cdecl, name="Z3_get_decl_sort_parameter", lib=z3)]
def Z3_get_decl_sort_parameter(c : _Z3_context? implicit; d : _Z3_func_decl? implicit; idx : uint) : _Z3_sort? {}
[extern(cdecl, name="Z3_get_decl_ast_parameter", lib=z3)]
def Z3_get_decl_ast_parameter(c : _Z3_context? implicit; d : _Z3_func_decl? implicit; idx : uint) : _Z3_ast? {}
[extern(cdecl, name="Z3_get_decl_func_decl_parameter", lib=z3)]
def Z3_get_decl_func_decl_parameter(c : _Z3_context? implicit; d : _Z3_func_decl? implicit; idx : uint) : _Z3_func_decl? {}
[extern(cdecl, name="Z3_get_decl_rational_parameter", lib=z3)]
def Z3_get_decl_rational_parameter(c : _Z3_context? implicit; d : _Z3_func_decl? implicit; idx : uint) : string {}
[extern(cdecl, name="Z3_app_to_ast", lib=z3)]
def Z3_app_to_ast(c : _Z3_context? implicit; a : _Z3_app? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_get_app_decl", lib=z3)]
def Z3_get_app_decl(c : _Z3_context? implicit; a : _Z3_app? implicit) : _Z3_func_decl? {}
[extern(cdecl, name="Z3_get_app_num_args", lib=z3)]
def Z3_get_app_num_args(c : _Z3_context? implicit; a : _Z3_app? implicit) : uint {}
[extern(cdecl, name="Z3_get_app_arg", lib=z3)]
def Z3_get_app_arg(c : _Z3_context? implicit; a : _Z3_app? implicit; i : uint) : _Z3_ast? {}
[extern(cdecl, name="Z3_is_eq_ast", lib=z3)]
def Z3_is_eq_ast(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : bool {}
[extern(cdecl, name="Z3_get_ast_id", lib=z3)]
def Z3_get_ast_id(c : _Z3_context? implicit; t : _Z3_ast? implicit) : uint {}
[extern(cdecl, name="Z3_get_ast_hash", lib=z3)]
def Z3_get_ast_hash(c : _Z3_context? implicit; a : _Z3_ast? implicit) : uint {}
[extern(cdecl, name="Z3_get_sort", lib=z3)]
def Z3_get_sort(c : _Z3_context? implicit; a : _Z3_ast? implicit) : _Z3_sort? {}
[extern(cdecl, name="Z3_is_well_sorted", lib=z3)]
def Z3_is_well_sorted(c : _Z3_context? implicit; t : _Z3_ast? implicit) : bool {}
[extern(cdecl, name="Z3_get_bool_value", lib=z3)]
def Z3_get_bool_value(c : _Z3_context? implicit; a : _Z3_ast? implicit) : Z3_lbool {}
[extern(cdecl, name="Z3_get_ast_kind", lib=z3)]
def Z3_get_ast_kind(c : _Z3_context? implicit; a : _Z3_ast? implicit) : Z3_ast_kind {}
[extern(cdecl, name="Z3_is_app", lib=z3)]
def Z3_is_app(c : _Z3_context? implicit; a : _Z3_ast? implicit) : bool {}
[extern(cdecl, name="Z3_is_ground", lib=z3)]
def Z3_is_ground(c : _Z3_context? implicit; a : _Z3_ast? implicit) : bool {}
[extern(cdecl, name="Z3_get_depth", lib=z3)]
def Z3_get_depth(c : _Z3_context? implicit; a : _Z3_ast? implicit) : uint {}
[extern(cdecl, name="Z3_is_numeral_ast", lib=z3)]
def Z3_is_numeral_ast(c : _Z3_context? implicit; a : _Z3_ast? implicit) : bool {}
[extern(cdecl, name="Z3_is_algebraic_number", lib=z3)]
def Z3_is_algebraic_number(c : _Z3_context? implicit; a : _Z3_ast? implicit) : bool {}
[extern(cdecl, name="Z3_to_app", lib=z3)]
def Z3_to_app(c : _Z3_context? implicit; a : _Z3_ast? implicit) : _Z3_app? {}
[extern(cdecl, name="Z3_to_func_decl", lib=z3)]
def Z3_to_func_decl(c : _Z3_context? implicit; a : _Z3_ast? implicit) : _Z3_func_decl? {}
[extern(cdecl, name="Z3_get_numeral_string", lib=z3)]
def Z3_get_numeral_string(c : _Z3_context? implicit; a : _Z3_ast? implicit) : string {}
[extern(cdecl, name="Z3_get_numeral_binary_string", lib=z3)]
def Z3_get_numeral_binary_string(c : _Z3_context? implicit; a : _Z3_ast? implicit) : string {}
[extern(cdecl, name="Z3_get_numeral_decimal_string", lib=z3)]
def Z3_get_numeral_decimal_string(c : _Z3_context? implicit; a : _Z3_ast? implicit; precision : uint) : string {}
[extern(cdecl, name="Z3_get_numeral_double", lib=z3)]
def Z3_get_numeral_double(c : _Z3_context? implicit; a : _Z3_ast? implicit) : double {}
[extern(cdecl, name="Z3_get_numerator", lib=z3)]
def Z3_get_numerator(c : _Z3_context? implicit; a : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_get_denominator", lib=z3)]
def Z3_get_denominator(c : _Z3_context? implicit; a : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_get_numeral_small", lib=z3)]
def Z3_get_numeral_small(c : _Z3_context? implicit; a : _Z3_ast? implicit; num : int64? implicit; den : int64? implicit) : bool {}
[extern(cdecl, name="Z3_get_numeral_int", lib=z3)]
def Z3_get_numeral_int(c : _Z3_context? implicit; v : _Z3_ast? implicit; i : int? implicit) : bool {}
[extern(cdecl, name="Z3_get_numeral_uint", lib=z3)]
def Z3_get_numeral_uint(c : _Z3_context? implicit; v : _Z3_ast? implicit; u : uint? implicit) : bool {}
[extern(cdecl, name="Z3_get_numeral_uint64", lib=z3)]
def Z3_get_numeral_uint64(c : _Z3_context? implicit; v : _Z3_ast? implicit; u : uint64? implicit) : bool {}
[extern(cdecl, name="Z3_get_numeral_int64", lib=z3)]
def Z3_get_numeral_int64(c : _Z3_context? implicit; v : _Z3_ast? implicit; i : int64? implicit) : bool {}
[extern(cdecl, name="Z3_get_numeral_rational_int64", lib=z3)]
def Z3_get_numeral_rational_int64(c : _Z3_context? implicit; v : _Z3_ast? implicit; num : int64? implicit; den : int64? implicit) : bool {}
[extern(cdecl, name="Z3_get_algebraic_number_lower", lib=z3)]
def Z3_get_algebraic_number_lower(c : _Z3_context? implicit; a : _Z3_ast? implicit; precision : uint) : _Z3_ast? {}
[extern(cdecl, name="Z3_get_algebraic_number_upper", lib=z3)]
def Z3_get_algebraic_number_upper(c : _Z3_context? implicit; a : _Z3_ast? implicit; precision : uint) : _Z3_ast? {}
[extern(cdecl, name="Z3_pattern_to_ast", lib=z3)]
def Z3_pattern_to_ast(c : _Z3_context? implicit; p : _Z3_pattern? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_get_pattern_num_terms", lib=z3)]
def Z3_get_pattern_num_terms(c : _Z3_context? implicit; p : _Z3_pattern? implicit) : uint {}
[extern(cdecl, name="Z3_get_pattern", lib=z3)]
def Z3_get_pattern(c : _Z3_context? implicit; p : _Z3_pattern? implicit; idx : uint) : _Z3_ast? {}
[extern(cdecl, name="Z3_get_index_value", lib=z3)]
def Z3_get_index_value(c : _Z3_context? implicit; a : _Z3_ast? implicit) : uint {}
[extern(cdecl, name="Z3_is_quantifier_forall", lib=z3)]
def Z3_is_quantifier_forall(c : _Z3_context? implicit; a : _Z3_ast? implicit) : bool {}
[extern(cdecl, name="Z3_is_quantifier_exists", lib=z3)]
def Z3_is_quantifier_exists(c : _Z3_context? implicit; a : _Z3_ast? implicit) : bool {}
[extern(cdecl, name="Z3_is_lambda", lib=z3)]
def Z3_is_lambda(c : _Z3_context? implicit; a : _Z3_ast? implicit) : bool {}
[extern(cdecl, name="Z3_get_quantifier_weight", lib=z3)]
def Z3_get_quantifier_weight(c : _Z3_context? implicit; a : _Z3_ast? implicit) : uint {}
[extern(cdecl, name="Z3_get_quantifier_skolem_id", lib=z3)]
def Z3_get_quantifier_skolem_id(c : _Z3_context? implicit; a : _Z3_ast? implicit) : _Z3_symbol? {}
[extern(cdecl, name="Z3_get_quantifier_id", lib=z3)]
def Z3_get_quantifier_id(c : _Z3_context? implicit; a : _Z3_ast? implicit) : _Z3_symbol? {}
[extern(cdecl, name="Z3_get_quantifier_num_patterns", lib=z3)]
def Z3_get_quantifier_num_patterns(c : _Z3_context? implicit; a : _Z3_ast? implicit) : uint {}
[extern(cdecl, name="Z3_get_quantifier_pattern_ast", lib=z3)]
def Z3_get_quantifier_pattern_ast(c : _Z3_context? implicit; a : _Z3_ast? implicit; i : uint) : _Z3_pattern? {}
[extern(cdecl, name="Z3_get_quantifier_num_no_patterns", lib=z3)]
def Z3_get_quantifier_num_no_patterns(c : _Z3_context? implicit; a : _Z3_ast? implicit) : uint {}
[extern(cdecl, name="Z3_get_quantifier_no_pattern_ast", lib=z3)]
def Z3_get_quantifier_no_pattern_ast(c : _Z3_context? implicit; a : _Z3_ast? implicit; i : uint) : _Z3_ast? {}
[extern(cdecl, name="Z3_get_quantifier_num_bound", lib=z3)]
def Z3_get_quantifier_num_bound(c : _Z3_context? implicit; a : _Z3_ast? implicit) : uint {}
[extern(cdecl, name="Z3_get_quantifier_bound_name", lib=z3)]
def Z3_get_quantifier_bound_name(c : _Z3_context? implicit; a : _Z3_ast? implicit; i : uint) : _Z3_symbol? {}
[extern(cdecl, name="Z3_get_quantifier_bound_sort", lib=z3)]
def Z3_get_quantifier_bound_sort(c : _Z3_context? implicit; a : _Z3_ast? implicit; i : uint) : _Z3_sort? {}
[extern(cdecl, name="Z3_get_quantifier_body", lib=z3)]
def Z3_get_quantifier_body(c : _Z3_context? implicit; a : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_simplify", lib=z3)]
def Z3_simplify(c : _Z3_context? implicit; a : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_simplify_ex", lib=z3)]
def Z3_simplify_ex(c : _Z3_context? implicit; a : _Z3_ast? implicit; p : _Z3_params? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_simplify_get_help", lib=z3)]
def Z3_simplify_get_help(c : _Z3_context? implicit) : string {}
[extern(cdecl, name="Z3_simplify_get_param_descrs", lib=z3)]
def Z3_simplify_get_param_descrs(c : _Z3_context? implicit) : _Z3_param_descrs? {}
[extern(cdecl, name="Z3_update_term", lib=z3)]
def Z3_update_term(c : _Z3_context? implicit; a : _Z3_ast? implicit; num_args : uint; args : void? const) : _Z3_ast? {}
[extern(cdecl, name="Z3_substitute", lib=z3)]
def Z3_substitute(c : _Z3_context? implicit; a : _Z3_ast? implicit; num_exprs : uint; from : void? const; to : void? const) : _Z3_ast? {}
[extern(cdecl, name="Z3_substitute_vars", lib=z3)]
def Z3_substitute_vars(c : _Z3_context? implicit; a : _Z3_ast? implicit; num_exprs : uint; to : void? const) : _Z3_ast? {}
[extern(cdecl, name="Z3_substitute_funs", lib=z3)]
def Z3_substitute_funs(c : _Z3_context? implicit; a : _Z3_ast? implicit; num_funs : uint; from : void? const; to : void? const) : _Z3_ast? {}
[extern(cdecl, name="Z3_translate", lib=z3)]
def Z3_translate(source : _Z3_context? implicit; a : _Z3_ast? implicit; target : _Z3_context? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_model", lib=z3)]
def Z3_mk_model(c : _Z3_context? implicit) : _Z3_model? {}
[extern(cdecl, name="Z3_model_inc_ref", lib=z3)]
def Z3_model_inc_ref(c : _Z3_context? implicit; m : _Z3_model? implicit) : void {}
[extern(cdecl, name="Z3_model_dec_ref", lib=z3)]
def Z3_model_dec_ref(c : _Z3_context? implicit; m : _Z3_model? implicit) : void {}
[extern(cdecl, name="Z3_model_eval", lib=z3)]
def Z3_model_eval(c : _Z3_context? implicit; m : _Z3_model? implicit; t : _Z3_ast? implicit; model_completion : bool; v : _Z3_ast ?? implicit) : bool {}
[extern(cdecl, name="Z3_model_get_const_interp", lib=z3)]
def Z3_model_get_const_interp(c : _Z3_context? implicit; m : _Z3_model? implicit; a : _Z3_func_decl? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_model_has_interp", lib=z3)]
def Z3_model_has_interp(c : _Z3_context? implicit; m : _Z3_model? implicit; a : _Z3_func_decl? implicit) : bool {}
[extern(cdecl, name="Z3_model_get_func_interp", lib=z3)]
def Z3_model_get_func_interp(c : _Z3_context? implicit; m : _Z3_model? implicit; f : _Z3_func_decl? implicit) : _Z3_func_interp? {}
[extern(cdecl, name="Z3_model_get_num_consts", lib=z3)]
def Z3_model_get_num_consts(c : _Z3_context? implicit; m : _Z3_model? implicit) : uint {}
[extern(cdecl, name="Z3_model_get_const_decl", lib=z3)]
def Z3_model_get_const_decl(c : _Z3_context? implicit; m : _Z3_model? implicit; i : uint) : _Z3_func_decl? {}
[extern(cdecl, name="Z3_model_get_num_funcs", lib=z3)]
def Z3_model_get_num_funcs(c : _Z3_context? implicit; m : _Z3_model? implicit) : uint {}
[extern(cdecl, name="Z3_model_get_func_decl", lib=z3)]
def Z3_model_get_func_decl(c : _Z3_context? implicit; m : _Z3_model? implicit; i : uint) : _Z3_func_decl? {}
[extern(cdecl, name="Z3_model_get_num_sorts", lib=z3)]
def Z3_model_get_num_sorts(c : _Z3_context? implicit; m : _Z3_model? implicit) : uint {}
[extern(cdecl, name="Z3_model_get_sort", lib=z3)]
def Z3_model_get_sort(c : _Z3_context? implicit; m : _Z3_model? implicit; i : uint) : _Z3_sort? {}
[extern(cdecl, name="Z3_model_get_sort_universe", lib=z3)]
def Z3_model_get_sort_universe(c : _Z3_context? implicit; m : _Z3_model? implicit; s : _Z3_sort? implicit) : _Z3_ast_vector? {}
[extern(cdecl, name="Z3_model_translate", lib=z3)]
def Z3_model_translate(c : _Z3_context? implicit; m : _Z3_model? implicit; dst : _Z3_context? implicit) : _Z3_model? {}
[extern(cdecl, name="Z3_is_as_array", lib=z3)]
def Z3_is_as_array(c : _Z3_context? implicit; a : _Z3_ast? implicit) : bool {}
[extern(cdecl, name="Z3_get_as_array_func_decl", lib=z3)]
def Z3_get_as_array_func_decl(c : _Z3_context? implicit; a : _Z3_ast? implicit) : _Z3_func_decl? {}
[extern(cdecl, name="Z3_add_func_interp", lib=z3)]
def Z3_add_func_interp(c : _Z3_context? implicit; m : _Z3_model? implicit; f : _Z3_func_decl? implicit; default_value : _Z3_ast? implicit) : _Z3_func_interp? {}
[extern(cdecl, name="Z3_add_const_interp", lib=z3)]
def Z3_add_const_interp(c : _Z3_context? implicit; m : _Z3_model? implicit; f : _Z3_func_decl? implicit; a : _Z3_ast? implicit) : void {}
[extern(cdecl, name="Z3_func_interp_inc_ref", lib=z3)]
def Z3_func_interp_inc_ref(c : _Z3_context? implicit; f : _Z3_func_interp? implicit) : void {}
[extern(cdecl, name="Z3_func_interp_dec_ref", lib=z3)]
def Z3_func_interp_dec_ref(c : _Z3_context? implicit; f : _Z3_func_interp? implicit) : void {}
[extern(cdecl, name="Z3_func_interp_get_num_entries", lib=z3)]
def Z3_func_interp_get_num_entries(c : _Z3_context? implicit; f : _Z3_func_interp? implicit) : uint {}
[extern(cdecl, name="Z3_func_interp_get_entry", lib=z3)]
def Z3_func_interp_get_entry(c : _Z3_context? implicit; f : _Z3_func_interp? implicit; i : uint) : _Z3_func_entry? {}
[extern(cdecl, name="Z3_func_interp_get_else", lib=z3)]
def Z3_func_interp_get_else(c : _Z3_context? implicit; f : _Z3_func_interp? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_func_interp_set_else", lib=z3)]
def Z3_func_interp_set_else(c : _Z3_context? implicit; f : _Z3_func_interp? implicit; else_value : _Z3_ast? implicit) : void {}
[extern(cdecl, name="Z3_func_interp_get_arity", lib=z3)]
def Z3_func_interp_get_arity(c : _Z3_context? implicit; f : _Z3_func_interp? implicit) : uint {}
[extern(cdecl, name="Z3_func_interp_add_entry", lib=z3)]
def Z3_func_interp_add_entry(c : _Z3_context? implicit; fi : _Z3_func_interp? implicit; args : _Z3_ast_vector? implicit; value : _Z3_ast? implicit) : void {}
[extern(cdecl, name="Z3_func_entry_inc_ref", lib=z3)]
def Z3_func_entry_inc_ref(c : _Z3_context? implicit; e : _Z3_func_entry? implicit) : void {}
[extern(cdecl, name="Z3_func_entry_dec_ref", lib=z3)]
def Z3_func_entry_dec_ref(c : _Z3_context? implicit; e : _Z3_func_entry? implicit) : void {}
[extern(cdecl, name="Z3_func_entry_get_value", lib=z3)]
def Z3_func_entry_get_value(c : _Z3_context? implicit; e : _Z3_func_entry? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_func_entry_get_num_args", lib=z3)]
def Z3_func_entry_get_num_args(c : _Z3_context? implicit; e : _Z3_func_entry? implicit) : uint {}
[extern(cdecl, name="Z3_func_entry_get_arg", lib=z3)]
def Z3_func_entry_get_arg(c : _Z3_context? implicit; e : _Z3_func_entry? implicit; i : uint) : _Z3_ast? {}
[extern(cdecl, name="Z3_open_log", lib=z3)]
def Z3_open_log(filename : string implicit) : bool {}
[extern(cdecl, name="Z3_append_log", lib=z3)]
def Z3_append_log(_string : string implicit) : void {}
[extern(cdecl, name="Z3_close_log", lib=z3)]
def Z3_close_log() : void {}
[extern(cdecl, name="Z3_toggle_warning_messages", lib=z3)]
def Z3_toggle_warning_messages(enabled : bool) : void {}
[extern(cdecl, name="Z3_set_ast_print_mode", lib=z3)]
def Z3_set_ast_print_mode(c : _Z3_context? implicit; mode : Z3_ast_print_mode) : void {}
[extern(cdecl, name="Z3_ast_to_string", lib=z3)]
def Z3_ast_to_string(c : _Z3_context? implicit; a : _Z3_ast? implicit) : string {}
[extern(cdecl, name="Z3_pattern_to_string", lib=z3)]
def Z3_pattern_to_string(c : _Z3_context? implicit; p : _Z3_pattern? implicit) : string {}
[extern(cdecl, name="Z3_sort_to_string", lib=z3)]
def Z3_sort_to_string(c : _Z3_context? implicit; s : _Z3_sort? implicit) : string {}
[extern(cdecl, name="Z3_func_decl_to_string", lib=z3)]
def Z3_func_decl_to_string(c : _Z3_context? implicit; d : _Z3_func_decl? implicit) : string {}
[extern(cdecl, name="Z3_model_to_string", lib=z3)]
def Z3_model_to_string(c : _Z3_context? implicit; m : _Z3_model? implicit) : string {}
[extern(cdecl, name="Z3_benchmark_to_smtlib_string", lib=z3)]
def Z3_benchmark_to_smtlib_string(c : _Z3_context? implicit; name : string implicit; logic : string implicit; status : string implicit; attributes : string implicit; num_assumptions : uint; assumptions : void? const; formula : _Z3_ast? implicit) : string {}
[extern(cdecl, name="Z3_parse_smtlib2_string", lib=z3)]
def Z3_parse_smtlib2_string(c : _Z3_context? implicit; str : string implicit; num_sorts : uint; sort_names : void? const; sorts : void? const; num_decls : uint; decl_names : void? const; decls : void? const) : _Z3_ast_vector? {}
[extern(cdecl, name="Z3_parse_smtlib2_file", lib=z3)]
def Z3_parse_smtlib2_file(c : _Z3_context? implicit; file_name : string implicit; num_sorts : uint; sort_names : void? const; sorts : void? const; num_decls : uint; decl_names : void? const; decls : void? const) : _Z3_ast_vector? {}
[extern(cdecl, name="Z3_eval_smtlib2_string", lib=z3)]
def Z3_eval_smtlib2_string(c : _Z3_context? implicit; str : string implicit) : string {}
[extern(cdecl, name="Z3_mk_parser_context", lib=z3)]
def Z3_mk_parser_context(c : _Z3_context? implicit) : _Z3_parser_context? {}
[extern(cdecl, name="Z3_parser_context_inc_ref", lib=z3)]
def Z3_parser_context_inc_ref(c : _Z3_context? implicit; pc : _Z3_parser_context? implicit) : void {}
[extern(cdecl, name="Z3_parser_context_dec_ref", lib=z3)]
def Z3_parser_context_dec_ref(c : _Z3_context? implicit; pc : _Z3_parser_context? implicit) : void {}
[extern(cdecl, name="Z3_parser_context_add_sort", lib=z3)]
def Z3_parser_context_add_sort(c : _Z3_context? implicit; pc : _Z3_parser_context? implicit; s : _Z3_sort? implicit) : void {}
[extern(cdecl, name="Z3_parser_context_add_decl", lib=z3)]
def Z3_parser_context_add_decl(c : _Z3_context? implicit; pc : _Z3_parser_context? implicit; f : _Z3_func_decl? implicit) : void {}
[extern(cdecl, name="Z3_parser_context_from_string", lib=z3)]
def Z3_parser_context_from_string(c : _Z3_context? implicit; pc : _Z3_parser_context? implicit; s : string implicit) : _Z3_ast_vector? {}
[extern(cdecl, name="Z3_get_error_code", lib=z3)]
def Z3_get_error_code(c : _Z3_context? implicit) : Z3_error_code {}
[extern(cdecl, name="Z3_set_error_handler", lib=z3)]
def Z3_set_error_handler(c : _Z3_context? implicit; h : void?) : void {}
[extern(cdecl, name="Z3_set_error", lib=z3)]
def Z3_set_error(c : _Z3_context? implicit; e : Z3_error_code) : void {}
[extern(cdecl, name="Z3_get_error_msg", lib=z3)]
def Z3_get_error_msg(c : _Z3_context? implicit; err : Z3_error_code) : string {}
[extern(cdecl, name="Z3_get_version", lib=z3)]
def Z3_get_version(major : uint? implicit; minor : uint? implicit; build_number : uint? implicit; revision_number : uint? implicit) : void {}
[extern(cdecl, name="Z3_get_full_version", lib=z3)]
def Z3_get_full_version() : string {}
[extern(cdecl, name="Z3_enable_trace", lib=z3)]
def Z3_enable_trace(tag : string implicit) : void {}
[extern(cdecl, name="Z3_disable_trace", lib=z3)]
def Z3_disable_trace(tag : string implicit) : void {}
[extern(cdecl, name="Z3_reset_memory", lib=z3)]
def Z3_reset_memory() : void {}
[extern(cdecl, name="Z3_finalize_memory", lib=z3)]
def Z3_finalize_memory() : void {}
[extern(cdecl, name="Z3_mk_goal", lib=z3)]
def Z3_mk_goal(c : _Z3_context? implicit; models : bool; unsat_cores : bool; proofs : bool) : _Z3_goal? {}
[extern(cdecl, name="Z3_goal_inc_ref", lib=z3)]
def Z3_goal_inc_ref(c : _Z3_context? implicit; g : _Z3_goal? implicit) : void {}
[extern(cdecl, name="Z3_goal_dec_ref", lib=z3)]
def Z3_goal_dec_ref(c : _Z3_context? implicit; g : _Z3_goal? implicit) : void {}
[extern(cdecl, name="Z3_goal_precision", lib=z3)]
def Z3_goal_precision(c : _Z3_context? implicit; g : _Z3_goal? implicit) : Z3_goal_prec {}
[extern(cdecl, name="Z3_goal_assert", lib=z3)]
def Z3_goal_assert(c : _Z3_context? implicit; g : _Z3_goal? implicit; a : _Z3_ast? implicit) : void {}
[extern(cdecl, name="Z3_goal_inconsistent", lib=z3)]
def Z3_goal_inconsistent(c : _Z3_context? implicit; g : _Z3_goal? implicit) : bool {}
[extern(cdecl, name="Z3_goal_depth", lib=z3)]
def Z3_goal_depth(c : _Z3_context? implicit; g : _Z3_goal? implicit) : uint {}
[extern(cdecl, name="Z3_goal_reset", lib=z3)]
def Z3_goal_reset(c : _Z3_context? implicit; g : _Z3_goal? implicit) : void {}
[extern(cdecl, name="Z3_goal_size", lib=z3)]
def Z3_goal_size(c : _Z3_context? implicit; g : _Z3_goal? implicit) : uint {}
[extern(cdecl, name="Z3_goal_formula", lib=z3)]
def Z3_goal_formula(c : _Z3_context? implicit; g : _Z3_goal? implicit; idx : uint) : _Z3_ast? {}
[extern(cdecl, name="Z3_goal_num_exprs", lib=z3)]
def Z3_goal_num_exprs(c : _Z3_context? implicit; g : _Z3_goal? implicit) : uint {}
[extern(cdecl, name="Z3_goal_is_decided_sat", lib=z3)]
def Z3_goal_is_decided_sat(c : _Z3_context? implicit; g : _Z3_goal? implicit) : bool {}
[extern(cdecl, name="Z3_goal_is_decided_unsat", lib=z3)]
def Z3_goal_is_decided_unsat(c : _Z3_context? implicit; g : _Z3_goal? implicit) : bool {}
[extern(cdecl, name="Z3_goal_translate", lib=z3)]
def Z3_goal_translate(source : _Z3_context? implicit; g : _Z3_goal? implicit; target : _Z3_context? implicit) : _Z3_goal? {}
[extern(cdecl, name="Z3_goal_convert_model", lib=z3)]
def Z3_goal_convert_model(c : _Z3_context? implicit; g : _Z3_goal? implicit; m : _Z3_model? implicit) : _Z3_model? {}
[extern(cdecl, name="Z3_goal_to_string", lib=z3)]
def Z3_goal_to_string(c : _Z3_context? implicit; g : _Z3_goal? implicit) : string {}
[extern(cdecl, name="Z3_goal_to_dimacs_string", lib=z3)]
def Z3_goal_to_dimacs_string(c : _Z3_context? implicit; g : _Z3_goal? implicit; include_names : bool) : string {}
[extern(cdecl, name="Z3_mk_tactic", lib=z3)]
def Z3_mk_tactic(c : _Z3_context? implicit; name : string implicit) : _Z3_tactic? {}
[extern(cdecl, name="Z3_tactic_inc_ref", lib=z3)]
def Z3_tactic_inc_ref(c : _Z3_context? implicit; t : _Z3_tactic? implicit) : void {}
[extern(cdecl, name="Z3_tactic_dec_ref", lib=z3)]
def Z3_tactic_dec_ref(c : _Z3_context? implicit; g : _Z3_tactic? implicit) : void {}
[extern(cdecl, name="Z3_mk_probe", lib=z3)]
def Z3_mk_probe(c : _Z3_context? implicit; name : string implicit) : _Z3_probe? {}
[extern(cdecl, name="Z3_probe_inc_ref", lib=z3)]
def Z3_probe_inc_ref(c : _Z3_context? implicit; p : _Z3_probe? implicit) : void {}
[extern(cdecl, name="Z3_probe_dec_ref", lib=z3)]
def Z3_probe_dec_ref(c : _Z3_context? implicit; p : _Z3_probe? implicit) : void {}
[extern(cdecl, name="Z3_tactic_and_then", lib=z3)]
def Z3_tactic_and_then(c : _Z3_context? implicit; t1 : _Z3_tactic? implicit; t2 : _Z3_tactic? implicit) : _Z3_tactic? {}
[extern(cdecl, name="Z3_tactic_or_else", lib=z3)]
def Z3_tactic_or_else(c : _Z3_context? implicit; t1 : _Z3_tactic? implicit; t2 : _Z3_tactic? implicit) : _Z3_tactic? {}
[extern(cdecl, name="Z3_tactic_par_or", lib=z3)]
def Z3_tactic_par_or(c : _Z3_context? implicit; num : uint; ts : void? const) : _Z3_tactic? {}
[extern(cdecl, name="Z3_tactic_par_and_then", lib=z3)]
def Z3_tactic_par_and_then(c : _Z3_context? implicit; t1 : _Z3_tactic? implicit; t2 : _Z3_tactic? implicit) : _Z3_tactic? {}
[extern(cdecl, name="Z3_tactic_try_for", lib=z3)]
def Z3_tactic_try_for(c : _Z3_context? implicit; t : _Z3_tactic? implicit; ms : uint) : _Z3_tactic? {}
[extern(cdecl, name="Z3_tactic_when", lib=z3)]
def Z3_tactic_when(c : _Z3_context? implicit; p : _Z3_probe? implicit; t : _Z3_tactic? implicit) : _Z3_tactic? {}
[extern(cdecl, name="Z3_tactic_cond", lib=z3)]
def Z3_tactic_cond(c : _Z3_context? implicit; p : _Z3_probe? implicit; t1 : _Z3_tactic? implicit; t2 : _Z3_tactic? implicit) : _Z3_tactic? {}
[extern(cdecl, name="Z3_tactic_repeat", lib=z3)]
def Z3_tactic_repeat(c : _Z3_context? implicit; t : _Z3_tactic? implicit; max : uint) : _Z3_tactic? {}
[extern(cdecl, name="Z3_tactic_skip", lib=z3)]
def Z3_tactic_skip(c : _Z3_context? implicit) : _Z3_tactic? {}
[extern(cdecl, name="Z3_tactic_fail", lib=z3)]
def Z3_tactic_fail(c : _Z3_context? implicit) : _Z3_tactic? {}
[extern(cdecl, name="Z3_tactic_fail_if", lib=z3)]
def Z3_tactic_fail_if(c : _Z3_context? implicit; p : _Z3_probe? implicit) : _Z3_tactic? {}
[extern(cdecl, name="Z3_tactic_fail_if_not_decided", lib=z3)]
def Z3_tactic_fail_if_not_decided(c : _Z3_context? implicit) : _Z3_tactic? {}
[extern(cdecl, name="Z3_tactic_using_params", lib=z3)]
def Z3_tactic_using_params(c : _Z3_context? implicit; t : _Z3_tactic? implicit; p : _Z3_params? implicit) : _Z3_tactic? {}
[extern(cdecl, name="Z3_mk_simplifier", lib=z3)]
def Z3_mk_simplifier(c : _Z3_context? implicit; name : string implicit) : _Z3_simplifier? {}
[extern(cdecl, name="Z3_simplifier_inc_ref", lib=z3)]
def Z3_simplifier_inc_ref(c : _Z3_context? implicit; t : _Z3_simplifier? implicit) : void {}
[extern(cdecl, name="Z3_simplifier_dec_ref", lib=z3)]
def Z3_simplifier_dec_ref(c : _Z3_context? implicit; g : _Z3_simplifier? implicit) : void {}
[extern(cdecl, name="Z3_solver_add_simplifier", lib=z3)]
def Z3_solver_add_simplifier(c : _Z3_context? implicit; solver : _Z3_solver? implicit; simplifier : _Z3_simplifier? implicit) : _Z3_solver? {}
[extern(cdecl, name="Z3_simplifier_and_then", lib=z3)]
def Z3_simplifier_and_then(c : _Z3_context? implicit; t1 : _Z3_simplifier? implicit; t2 : _Z3_simplifier? implicit) : _Z3_simplifier? {}
[extern(cdecl, name="Z3_simplifier_using_params", lib=z3)]
def Z3_simplifier_using_params(c : _Z3_context? implicit; t : _Z3_simplifier? implicit; p : _Z3_params? implicit) : _Z3_simplifier? {}
[extern(cdecl, name="Z3_get_num_simplifiers", lib=z3)]
def Z3_get_num_simplifiers(c : _Z3_context? implicit) : uint {}
[extern(cdecl, name="Z3_get_simplifier_name", lib=z3)]
def Z3_get_simplifier_name(c : _Z3_context? implicit; i : uint) : string {}
[extern(cdecl, name="Z3_simplifier_get_help", lib=z3)]
def Z3_simplifier_get_help(c : _Z3_context? implicit; t : _Z3_simplifier? implicit) : string {}
[extern(cdecl, name="Z3_simplifier_get_param_descrs", lib=z3)]
def Z3_simplifier_get_param_descrs(c : _Z3_context? implicit; t : _Z3_simplifier? implicit) : _Z3_param_descrs? {}
[extern(cdecl, name="Z3_simplifier_get_descr", lib=z3)]
def Z3_simplifier_get_descr(c : _Z3_context? implicit; name : string implicit) : string {}
[extern(cdecl, name="Z3_probe_const", lib=z3)]
def Z3_probe_const(x : _Z3_context? implicit; val : double) : _Z3_probe? {}
[extern(cdecl, name="Z3_probe_lt", lib=z3)]
def Z3_probe_lt(x : _Z3_context? implicit; p1 : _Z3_probe? implicit; p2 : _Z3_probe? implicit) : _Z3_probe? {}
[extern(cdecl, name="Z3_probe_gt", lib=z3)]
def Z3_probe_gt(x : _Z3_context? implicit; p1 : _Z3_probe? implicit; p2 : _Z3_probe? implicit) : _Z3_probe? {}
[extern(cdecl, name="Z3_probe_le", lib=z3)]
def Z3_probe_le(x : _Z3_context? implicit; p1 : _Z3_probe? implicit; p2 : _Z3_probe? implicit) : _Z3_probe? {}
[extern(cdecl, name="Z3_probe_ge", lib=z3)]
def Z3_probe_ge(x : _Z3_context? implicit; p1 : _Z3_probe? implicit; p2 : _Z3_probe? implicit) : _Z3_probe? {}
[extern(cdecl, name="Z3_probe_eq", lib=z3)]
def Z3_probe_eq(x : _Z3_context? implicit; p1 : _Z3_probe? implicit; p2 : _Z3_probe? implicit) : _Z3_probe? {}
[extern(cdecl, name="Z3_probe_and", lib=z3)]
def Z3_probe_and(x : _Z3_context? implicit; p1 : _Z3_probe? implicit; p2 : _Z3_probe? implicit) : _Z3_probe? {}
[extern(cdecl, name="Z3_probe_or", lib=z3)]
def Z3_probe_or(x : _Z3_context? implicit; p1 : _Z3_probe? implicit; p2 : _Z3_probe? implicit) : _Z3_probe? {}
[extern(cdecl, name="Z3_probe_not", lib=z3)]
def Z3_probe_not(x : _Z3_context? implicit; p : _Z3_probe? implicit) : _Z3_probe? {}
[extern(cdecl, name="Z3_get_num_tactics", lib=z3)]
def Z3_get_num_tactics(c : _Z3_context? implicit) : uint {}
[extern(cdecl, name="Z3_get_tactic_name", lib=z3)]
def Z3_get_tactic_name(c : _Z3_context? implicit; i : uint) : string {}
[extern(cdecl, name="Z3_get_num_probes", lib=z3)]
def Z3_get_num_probes(c : _Z3_context? implicit) : uint {}
[extern(cdecl, name="Z3_get_probe_name", lib=z3)]
def Z3_get_probe_name(c : _Z3_context? implicit; i : uint) : string {}
[extern(cdecl, name="Z3_tactic_get_help", lib=z3)]
def Z3_tactic_get_help(c : _Z3_context? implicit; t : _Z3_tactic? implicit) : string {}
[extern(cdecl, name="Z3_tactic_get_param_descrs", lib=z3)]
def Z3_tactic_get_param_descrs(c : _Z3_context? implicit; t : _Z3_tactic? implicit) : _Z3_param_descrs? {}
[extern(cdecl, name="Z3_tactic_get_descr", lib=z3)]
def Z3_tactic_get_descr(c : _Z3_context? implicit; name : string implicit) : string {}
[extern(cdecl, name="Z3_probe_get_descr", lib=z3)]
def Z3_probe_get_descr(c : _Z3_context? implicit; name : string implicit) : string {}
[extern(cdecl, name="Z3_probe_apply", lib=z3)]
def Z3_probe_apply(c : _Z3_context? implicit; p : _Z3_probe? implicit; g : _Z3_goal? implicit) : double {}
[extern(cdecl, name="Z3_tactic_apply", lib=z3)]
def Z3_tactic_apply(c : _Z3_context? implicit; t : _Z3_tactic? implicit; g : _Z3_goal? implicit) : _Z3_apply_result? {}
[extern(cdecl, name="Z3_tactic_apply_ex", lib=z3)]
def Z3_tactic_apply_ex(c : _Z3_context? implicit; t : _Z3_tactic? implicit; g : _Z3_goal? implicit; p : _Z3_params? implicit) : _Z3_apply_result? {}
[extern(cdecl, name="Z3_apply_result_inc_ref", lib=z3)]
def Z3_apply_result_inc_ref(c : _Z3_context? implicit; r : _Z3_apply_result? implicit) : void {}
[extern(cdecl, name="Z3_apply_result_dec_ref", lib=z3)]
def Z3_apply_result_dec_ref(c : _Z3_context? implicit; r : _Z3_apply_result? implicit) : void {}
[extern(cdecl, name="Z3_apply_result_to_string", lib=z3)]
def Z3_apply_result_to_string(c : _Z3_context? implicit; r : _Z3_apply_result? implicit) : string {}
[extern(cdecl, name="Z3_apply_result_get_num_subgoals", lib=z3)]
def Z3_apply_result_get_num_subgoals(c : _Z3_context? implicit; r : _Z3_apply_result? implicit) : uint {}
[extern(cdecl, name="Z3_apply_result_get_subgoal", lib=z3)]
def Z3_apply_result_get_subgoal(c : _Z3_context? implicit; r : _Z3_apply_result? implicit; i : uint) : _Z3_goal? {}
[extern(cdecl, name="Z3_mk_solver", lib=z3)]
def Z3_mk_solver(c : _Z3_context? implicit) : _Z3_solver? {}
[extern(cdecl, name="Z3_mk_simple_solver", lib=z3)]
def Z3_mk_simple_solver(c : _Z3_context? implicit) : _Z3_solver? {}
[extern(cdecl, name="Z3_mk_solver_for_logic", lib=z3)]
def Z3_mk_solver_for_logic(c : _Z3_context? implicit; logic : _Z3_symbol? implicit) : _Z3_solver? {}
[extern(cdecl, name="Z3_mk_solver_from_tactic", lib=z3)]
def Z3_mk_solver_from_tactic(c : _Z3_context? implicit; t : _Z3_tactic? implicit) : _Z3_solver? {}
[extern(cdecl, name="Z3_solver_translate", lib=z3)]
def Z3_solver_translate(source : _Z3_context? implicit; s : _Z3_solver? implicit; target : _Z3_context? implicit) : _Z3_solver? {}
[extern(cdecl, name="Z3_solver_import_model_converter", lib=z3)]
def Z3_solver_import_model_converter(ctx : _Z3_context? implicit; src : _Z3_solver? implicit; dst : _Z3_solver? implicit) : void {}
[extern(cdecl, name="Z3_solver_get_help", lib=z3)]
def Z3_solver_get_help(c : _Z3_context? implicit; s : _Z3_solver? implicit) : string {}
[extern(cdecl, name="Z3_solver_get_param_descrs", lib=z3)]
def Z3_solver_get_param_descrs(c : _Z3_context? implicit; s : _Z3_solver? implicit) : _Z3_param_descrs? {}
[extern(cdecl, name="Z3_solver_set_params", lib=z3)]
def Z3_solver_set_params(c : _Z3_context? implicit; s : _Z3_solver? implicit; p : _Z3_params? implicit) : void {}
[extern(cdecl, name="Z3_solver_inc_ref", lib=z3)]
def Z3_solver_inc_ref(c : _Z3_context? implicit; s : _Z3_solver? implicit) : void {}
[extern(cdecl, name="Z3_solver_dec_ref", lib=z3)]
def Z3_solver_dec_ref(c : _Z3_context? implicit; s : _Z3_solver? implicit) : void {}
[extern(cdecl, name="Z3_solver_interrupt", lib=z3)]
def Z3_solver_interrupt(c : _Z3_context? implicit; s : _Z3_solver? implicit) : void {}
[extern(cdecl, name="Z3_solver_push", lib=z3)]
def Z3_solver_push(c : _Z3_context? implicit; s : _Z3_solver? implicit) : void {}
[extern(cdecl, name="Z3_solver_pop", lib=z3)]
def Z3_solver_pop(c : _Z3_context? implicit; s : _Z3_solver? implicit; n : uint) : void {}
[extern(cdecl, name="Z3_solver_reset", lib=z3)]
def Z3_solver_reset(c : _Z3_context? implicit; s : _Z3_solver? implicit) : void {}
[extern(cdecl, name="Z3_solver_get_num_scopes", lib=z3)]
def Z3_solver_get_num_scopes(c : _Z3_context? implicit; s : _Z3_solver? implicit) : uint {}
[extern(cdecl, name="Z3_solver_assert", lib=z3)]
def Z3_solver_assert(c : _Z3_context? implicit; s : _Z3_solver? implicit; a : _Z3_ast? implicit) : void {}
[extern(cdecl, name="Z3_solver_assert_and_track", lib=z3)]
def Z3_solver_assert_and_track(c : _Z3_context? implicit; s : _Z3_solver? implicit; a : _Z3_ast? implicit; p : _Z3_ast? implicit) : void {}
[extern(cdecl, name="Z3_solver_from_file", lib=z3)]
def Z3_solver_from_file(c : _Z3_context? implicit; s : _Z3_solver? implicit; file_name : string implicit) : void {}
[extern(cdecl, name="Z3_solver_from_string", lib=z3)]
def Z3_solver_from_string(c : _Z3_context? implicit; s : _Z3_solver? implicit; str : string implicit) : void {}
[extern(cdecl, name="Z3_solver_get_assertions", lib=z3)]
def Z3_solver_get_assertions(c : _Z3_context? implicit; s : _Z3_solver? implicit) : _Z3_ast_vector? {}
[extern(cdecl, name="Z3_solver_get_units", lib=z3)]
def Z3_solver_get_units(c : _Z3_context? implicit; s : _Z3_solver? implicit) : _Z3_ast_vector? {}
[extern(cdecl, name="Z3_solver_get_trail", lib=z3)]
def Z3_solver_get_trail(c : _Z3_context? implicit; s : _Z3_solver? implicit) : _Z3_ast_vector? {}
[extern(cdecl, name="Z3_solver_get_non_units", lib=z3)]
def Z3_solver_get_non_units(c : _Z3_context? implicit; s : _Z3_solver? implicit) : _Z3_ast_vector? {}
[extern(cdecl, name="Z3_solver_get_levels", lib=z3)]
def Z3_solver_get_levels(c : _Z3_context? implicit; s : _Z3_solver? implicit; literals : _Z3_ast_vector? implicit; sz : uint; levels : void?) : void {}
[extern(cdecl, name="Z3_solver_congruence_root", lib=z3)]
def Z3_solver_congruence_root(c : _Z3_context? implicit; s : _Z3_solver? implicit; a : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_solver_congruence_next", lib=z3)]
def Z3_solver_congruence_next(c : _Z3_context? implicit; s : _Z3_solver? implicit; a : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_solver_congruence_explain", lib=z3)]
def Z3_solver_congruence_explain(c : _Z3_context? implicit; s : _Z3_solver? implicit; a : _Z3_ast? implicit; b : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_solver_solve_for", lib=z3)]
def Z3_solver_solve_for(c : _Z3_context? implicit; s : _Z3_solver? implicit; variables : _Z3_ast_vector? implicit; terms : _Z3_ast_vector? implicit; guards : _Z3_ast_vector? implicit) : void {}
[extern(cdecl, name="Z3_solver_next_split", lib=z3)]
def Z3_solver_next_split(c : _Z3_context? implicit; cb : _Z3_solver_callback? implicit; t : _Z3_ast? implicit; idx : uint; phase : Z3_lbool) : bool {}
[extern(cdecl, name="Z3_solver_set_initial_value", lib=z3)]
def Z3_solver_set_initial_value(c : _Z3_context? implicit; s : _Z3_solver? implicit; v : _Z3_ast? implicit; val : _Z3_ast? implicit) : void {}
[extern(cdecl, name="Z3_solver_check", lib=z3)]
def Z3_solver_check(c : _Z3_context? implicit; s : _Z3_solver? implicit) : Z3_lbool {}
[extern(cdecl, name="Z3_solver_check_assumptions", lib=z3)]
def Z3_solver_check_assumptions(c : _Z3_context? implicit; s : _Z3_solver? implicit; num_assumptions : uint; assumptions : void? const) : Z3_lbool {}
[extern(cdecl, name="Z3_get_implied_equalities", lib=z3)]
def Z3_get_implied_equalities(c : _Z3_context? implicit; s : _Z3_solver? implicit; num_terms : uint; terms : void? const; class_ids : void?) : Z3_lbool {}
[extern(cdecl, name="Z3_solver_get_consequences", lib=z3)]
def Z3_solver_get_consequences(c : _Z3_context? implicit; s : _Z3_solver? implicit; assumptions : _Z3_ast_vector? implicit; variables : _Z3_ast_vector? implicit; consequences : _Z3_ast_vector? implicit) : Z3_lbool {}
[extern(cdecl, name="Z3_solver_cube", lib=z3)]
def Z3_solver_cube(c : _Z3_context? implicit; s : _Z3_solver? implicit; vars : _Z3_ast_vector? implicit; backtrack_level : uint) : _Z3_ast_vector? {}
[extern(cdecl, name="Z3_solver_get_model", lib=z3)]
def Z3_solver_get_model(c : _Z3_context? implicit; s : _Z3_solver? implicit) : _Z3_model? {}
[extern(cdecl, name="Z3_solver_get_proof", lib=z3)]
def Z3_solver_get_proof(c : _Z3_context? implicit; s : _Z3_solver? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_solver_get_unsat_core", lib=z3)]
def Z3_solver_get_unsat_core(c : _Z3_context? implicit; s : _Z3_solver? implicit) : _Z3_ast_vector? {}
[extern(cdecl, name="Z3_solver_get_reason_unknown", lib=z3)]
def Z3_solver_get_reason_unknown(c : _Z3_context? implicit; s : _Z3_solver? implicit) : string {}
[extern(cdecl, name="Z3_solver_get_statistics", lib=z3)]
def Z3_solver_get_statistics(c : _Z3_context? implicit; s : _Z3_solver? implicit) : _Z3_stats? {}
[extern(cdecl, name="Z3_solver_to_string", lib=z3)]
def Z3_solver_to_string(c : _Z3_context? implicit; s : _Z3_solver? implicit) : string {}
[extern(cdecl, name="Z3_solver_to_dimacs_string", lib=z3)]
def Z3_solver_to_dimacs_string(c : _Z3_context? implicit; s : _Z3_solver? implicit; include_names : bool) : string {}
[extern(cdecl, name="Z3_stats_to_string", lib=z3)]
def Z3_stats_to_string(c : _Z3_context? implicit; s : _Z3_stats? implicit) : string {}
[extern(cdecl, name="Z3_stats_inc_ref", lib=z3)]
def Z3_stats_inc_ref(c : _Z3_context? implicit; s : _Z3_stats? implicit) : void {}
[extern(cdecl, name="Z3_stats_dec_ref", lib=z3)]
def Z3_stats_dec_ref(c : _Z3_context? implicit; s : _Z3_stats? implicit) : void {}
[extern(cdecl, name="Z3_stats_size", lib=z3)]
def Z3_stats_size(c : _Z3_context? implicit; s : _Z3_stats? implicit) : uint {}
[extern(cdecl, name="Z3_stats_get_key", lib=z3)]
def Z3_stats_get_key(c : _Z3_context? implicit; s : _Z3_stats? implicit; idx : uint) : string {}
[extern(cdecl, name="Z3_stats_is_uint", lib=z3)]
def Z3_stats_is_uint(c : _Z3_context? implicit; s : _Z3_stats? implicit; idx : uint) : bool {}
[extern(cdecl, name="Z3_stats_is_double", lib=z3)]
def Z3_stats_is_double(c : _Z3_context? implicit; s : _Z3_stats? implicit; idx : uint) : bool {}
[extern(cdecl, name="Z3_stats_get_uint_value", lib=z3)]
def Z3_stats_get_uint_value(c : _Z3_context? implicit; s : _Z3_stats? implicit; idx : uint) : uint {}
[extern(cdecl, name="Z3_stats_get_double_value", lib=z3)]
def Z3_stats_get_double_value(c : _Z3_context? implicit; s : _Z3_stats? implicit; idx : uint) : double {}
[extern(cdecl, name="Z3_get_estimated_alloc_size", lib=z3)]
def Z3_get_estimated_alloc_size() : uint64 {}
[extern(cdecl, name="Z3_mk_ast_vector", lib=z3)]
def Z3_mk_ast_vector(c : _Z3_context? implicit) : _Z3_ast_vector? {}
[extern(cdecl, name="Z3_ast_vector_inc_ref", lib=z3)]
def Z3_ast_vector_inc_ref(c : _Z3_context? implicit; v : _Z3_ast_vector? implicit) : void {}
[extern(cdecl, name="Z3_ast_vector_dec_ref", lib=z3)]
def Z3_ast_vector_dec_ref(c : _Z3_context? implicit; v : _Z3_ast_vector? implicit) : void {}
[extern(cdecl, name="Z3_ast_vector_size", lib=z3)]
def Z3_ast_vector_size(c : _Z3_context? implicit; v : _Z3_ast_vector? implicit) : uint {}
[extern(cdecl, name="Z3_ast_vector_get", lib=z3)]
def Z3_ast_vector_get(c : _Z3_context? implicit; v : _Z3_ast_vector? implicit; i : uint) : _Z3_ast? {}
[extern(cdecl, name="Z3_ast_vector_set", lib=z3)]
def Z3_ast_vector_set(c : _Z3_context? implicit; v : _Z3_ast_vector? implicit; i : uint; a : _Z3_ast? implicit) : void {}
[extern(cdecl, name="Z3_ast_vector_resize", lib=z3)]
def Z3_ast_vector_resize(c : _Z3_context? implicit; v : _Z3_ast_vector? implicit; n : uint) : void {}
[extern(cdecl, name="Z3_ast_vector_push", lib=z3)]
def Z3_ast_vector_push(c : _Z3_context? implicit; v : _Z3_ast_vector? implicit; a : _Z3_ast? implicit) : void {}
[extern(cdecl, name="Z3_ast_vector_translate", lib=z3)]
def Z3_ast_vector_translate(s : _Z3_context? implicit; v : _Z3_ast_vector? implicit; t : _Z3_context? implicit) : _Z3_ast_vector? {}
[extern(cdecl, name="Z3_ast_vector_to_string", lib=z3)]
def Z3_ast_vector_to_string(c : _Z3_context? implicit; v : _Z3_ast_vector? implicit) : string {}
[extern(cdecl, name="Z3_mk_ast_map", lib=z3)]
def Z3_mk_ast_map(c : _Z3_context? implicit) : _Z3_ast_map? {}
[extern(cdecl, name="Z3_ast_map_inc_ref", lib=z3)]
def Z3_ast_map_inc_ref(c : _Z3_context? implicit; m : _Z3_ast_map? implicit) : void {}
[extern(cdecl, name="Z3_ast_map_dec_ref", lib=z3)]
def Z3_ast_map_dec_ref(c : _Z3_context? implicit; m : _Z3_ast_map? implicit) : void {}
[extern(cdecl, name="Z3_ast_map_contains", lib=z3)]
def Z3_ast_map_contains(c : _Z3_context? implicit; m : _Z3_ast_map? implicit; k : _Z3_ast? implicit) : bool {}
[extern(cdecl, name="Z3_ast_map_find", lib=z3)]
def Z3_ast_map_find(c : _Z3_context? implicit; m : _Z3_ast_map? implicit; k : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_ast_map_insert", lib=z3)]
def Z3_ast_map_insert(c : _Z3_context? implicit; m : _Z3_ast_map? implicit; k : _Z3_ast? implicit; v : _Z3_ast? implicit) : void {}
[extern(cdecl, name="Z3_ast_map_erase", lib=z3)]
def Z3_ast_map_erase(c : _Z3_context? implicit; m : _Z3_ast_map? implicit; k : _Z3_ast? implicit) : void {}
[extern(cdecl, name="Z3_ast_map_reset", lib=z3)]
def Z3_ast_map_reset(c : _Z3_context? implicit; m : _Z3_ast_map? implicit) : void {}
[extern(cdecl, name="Z3_ast_map_size", lib=z3)]
def Z3_ast_map_size(c : _Z3_context? implicit; m : _Z3_ast_map? implicit) : uint {}
[extern(cdecl, name="Z3_ast_map_keys", lib=z3)]
def Z3_ast_map_keys(c : _Z3_context? implicit; m : _Z3_ast_map? implicit) : _Z3_ast_vector? {}
[extern(cdecl, name="Z3_ast_map_to_string", lib=z3)]
def Z3_ast_map_to_string(c : _Z3_context? implicit; m : _Z3_ast_map? implicit) : string {}
[extern(cdecl, name="Z3_algebraic_is_value", lib=z3)]
def Z3_algebraic_is_value(c : _Z3_context? implicit; a : _Z3_ast? implicit) : bool {}
[extern(cdecl, name="Z3_algebraic_is_pos", lib=z3)]
def Z3_algebraic_is_pos(c : _Z3_context? implicit; a : _Z3_ast? implicit) : bool {}
[extern(cdecl, name="Z3_algebraic_is_neg", lib=z3)]
def Z3_algebraic_is_neg(c : _Z3_context? implicit; a : _Z3_ast? implicit) : bool {}
[extern(cdecl, name="Z3_algebraic_is_zero", lib=z3)]
def Z3_algebraic_is_zero(c : _Z3_context? implicit; a : _Z3_ast? implicit) : bool {}
[extern(cdecl, name="Z3_algebraic_sign", lib=z3)]
def Z3_algebraic_sign(c : _Z3_context? implicit; a : _Z3_ast? implicit) : int {}
[extern(cdecl, name="Z3_algebraic_add", lib=z3)]
def Z3_algebraic_add(c : _Z3_context? implicit; a : _Z3_ast? implicit; b : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_algebraic_sub", lib=z3)]
def Z3_algebraic_sub(c : _Z3_context? implicit; a : _Z3_ast? implicit; b : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_algebraic_mul", lib=z3)]
def Z3_algebraic_mul(c : _Z3_context? implicit; a : _Z3_ast? implicit; b : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_algebraic_div", lib=z3)]
def Z3_algebraic_div(c : _Z3_context? implicit; a : _Z3_ast? implicit; b : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_algebraic_root", lib=z3)]
def Z3_algebraic_root(c : _Z3_context? implicit; a : _Z3_ast? implicit; k : uint) : _Z3_ast? {}
[extern(cdecl, name="Z3_algebraic_power", lib=z3)]
def Z3_algebraic_power(c : _Z3_context? implicit; a : _Z3_ast? implicit; k : uint) : _Z3_ast? {}
[extern(cdecl, name="Z3_algebraic_lt", lib=z3)]
def Z3_algebraic_lt(c : _Z3_context? implicit; a : _Z3_ast? implicit; b : _Z3_ast? implicit) : bool {}
[extern(cdecl, name="Z3_algebraic_gt", lib=z3)]
def Z3_algebraic_gt(c : _Z3_context? implicit; a : _Z3_ast? implicit; b : _Z3_ast? implicit) : bool {}
[extern(cdecl, name="Z3_algebraic_le", lib=z3)]
def Z3_algebraic_le(c : _Z3_context? implicit; a : _Z3_ast? implicit; b : _Z3_ast? implicit) : bool {}
[extern(cdecl, name="Z3_algebraic_ge", lib=z3)]
def Z3_algebraic_ge(c : _Z3_context? implicit; a : _Z3_ast? implicit; b : _Z3_ast? implicit) : bool {}
[extern(cdecl, name="Z3_algebraic_eq", lib=z3)]
def Z3_algebraic_eq(c : _Z3_context? implicit; a : _Z3_ast? implicit; b : _Z3_ast? implicit) : bool {}
[extern(cdecl, name="Z3_algebraic_neq", lib=z3)]
def Z3_algebraic_neq(c : _Z3_context? implicit; a : _Z3_ast? implicit; b : _Z3_ast? implicit) : bool {}
[extern(cdecl, name="Z3_algebraic_roots", lib=z3)]
def Z3_algebraic_roots(c : _Z3_context? implicit; p : _Z3_ast? implicit; n : uint; a : void?) : _Z3_ast_vector? {}
[extern(cdecl, name="Z3_algebraic_eval", lib=z3)]
def Z3_algebraic_eval(c : _Z3_context? implicit; p : _Z3_ast? implicit; n : uint; a : void?) : int {}
[extern(cdecl, name="Z3_algebraic_get_poly", lib=z3)]
def Z3_algebraic_get_poly(c : _Z3_context? implicit; a : _Z3_ast? implicit) : _Z3_ast_vector? {}
[extern(cdecl, name="Z3_algebraic_get_i", lib=z3)]
def Z3_algebraic_get_i(c : _Z3_context? implicit; a : _Z3_ast? implicit) : uint {}
[extern(cdecl, name="Z3_polynomial_subresultants", lib=z3)]
def Z3_polynomial_subresultants(c : _Z3_context? implicit; p : _Z3_ast? implicit; q : _Z3_ast? implicit; x : _Z3_ast? implicit) : _Z3_ast_vector? {}
[extern(cdecl, name="Z3_rcf_del", lib=z3)]
def Z3_rcf_del(c : _Z3_context? implicit; a : _Z3_rcf_num? implicit) : void {}
[extern(cdecl, name="Z3_rcf_mk_rational", lib=z3)]
def Z3_rcf_mk_rational(c : _Z3_context? implicit; val : string implicit) : _Z3_rcf_num? {}
[extern(cdecl, name="Z3_rcf_mk_small_int", lib=z3)]
def Z3_rcf_mk_small_int(c : _Z3_context? implicit; val : int) : _Z3_rcf_num? {}
[extern(cdecl, name="Z3_rcf_mk_pi", lib=z3)]
def Z3_rcf_mk_pi(c : _Z3_context? implicit) : _Z3_rcf_num? {}
[extern(cdecl, name="Z3_rcf_mk_e", lib=z3)]
def Z3_rcf_mk_e(c : _Z3_context? implicit) : _Z3_rcf_num? {}
[extern(cdecl, name="Z3_rcf_mk_infinitesimal", lib=z3)]
def Z3_rcf_mk_infinitesimal(c : _Z3_context? implicit) : _Z3_rcf_num? {}
[extern(cdecl, name="Z3_rcf_mk_roots", lib=z3)]
def Z3_rcf_mk_roots(c : _Z3_context? implicit; n : uint; a : void? const; roots : void?) : uint {}
[extern(cdecl, name="Z3_rcf_add", lib=z3)]
def Z3_rcf_add(c : _Z3_context? implicit; a : _Z3_rcf_num? implicit; b : _Z3_rcf_num? implicit) : _Z3_rcf_num? {}
[extern(cdecl, name="Z3_rcf_sub", lib=z3)]
def Z3_rcf_sub(c : _Z3_context? implicit; a : _Z3_rcf_num? implicit; b : _Z3_rcf_num? implicit) : _Z3_rcf_num? {}
[extern(cdecl, name="Z3_rcf_mul", lib=z3)]
def Z3_rcf_mul(c : _Z3_context? implicit; a : _Z3_rcf_num? implicit; b : _Z3_rcf_num? implicit) : _Z3_rcf_num? {}
[extern(cdecl, name="Z3_rcf_div", lib=z3)]
def Z3_rcf_div(c : _Z3_context? implicit; a : _Z3_rcf_num? implicit; b : _Z3_rcf_num? implicit) : _Z3_rcf_num? {}
[extern(cdecl, name="Z3_rcf_neg", lib=z3)]
def Z3_rcf_neg(c : _Z3_context? implicit; a : _Z3_rcf_num? implicit) : _Z3_rcf_num? {}
[extern(cdecl, name="Z3_rcf_inv", lib=z3)]
def Z3_rcf_inv(c : _Z3_context? implicit; a : _Z3_rcf_num? implicit) : _Z3_rcf_num? {}
[extern(cdecl, name="Z3_rcf_power", lib=z3)]
def Z3_rcf_power(c : _Z3_context? implicit; a : _Z3_rcf_num? implicit; k : uint) : _Z3_rcf_num? {}
[extern(cdecl, name="Z3_rcf_lt", lib=z3)]
def Z3_rcf_lt(c : _Z3_context? implicit; a : _Z3_rcf_num? implicit; b : _Z3_rcf_num? implicit) : bool {}
[extern(cdecl, name="Z3_rcf_gt", lib=z3)]
def Z3_rcf_gt(c : _Z3_context? implicit; a : _Z3_rcf_num? implicit; b : _Z3_rcf_num? implicit) : bool {}
[extern(cdecl, name="Z3_rcf_le", lib=z3)]
def Z3_rcf_le(c : _Z3_context? implicit; a : _Z3_rcf_num? implicit; b : _Z3_rcf_num? implicit) : bool {}
[extern(cdecl, name="Z3_rcf_ge", lib=z3)]
def Z3_rcf_ge(c : _Z3_context? implicit; a : _Z3_rcf_num? implicit; b : _Z3_rcf_num? implicit) : bool {}
[extern(cdecl, name="Z3_rcf_eq", lib=z3)]
def Z3_rcf_eq(c : _Z3_context? implicit; a : _Z3_rcf_num? implicit; b : _Z3_rcf_num? implicit) : bool {}
[extern(cdecl, name="Z3_rcf_neq", lib=z3)]
def Z3_rcf_neq(c : _Z3_context? implicit; a : _Z3_rcf_num? implicit; b : _Z3_rcf_num? implicit) : bool {}
[extern(cdecl, name="Z3_rcf_num_to_string", lib=z3)]
def Z3_rcf_num_to_string(c : _Z3_context? implicit; a : _Z3_rcf_num? implicit; compact : bool; html : bool) : string {}
[extern(cdecl, name="Z3_rcf_num_to_decimal_string", lib=z3)]
def Z3_rcf_num_to_decimal_string(c : _Z3_context? implicit; a : _Z3_rcf_num? implicit; prec : uint) : string {}
[extern(cdecl, name="Z3_rcf_get_numerator_denominator", lib=z3)]
def Z3_rcf_get_numerator_denominator(c : _Z3_context? implicit; a : _Z3_rcf_num? implicit; n : _Z3_rcf_num ?? implicit; d : _Z3_rcf_num ?? implicit) : void {}
[extern(cdecl, name="Z3_rcf_is_rational", lib=z3)]
def Z3_rcf_is_rational(c : _Z3_context? implicit; a : _Z3_rcf_num? implicit) : bool {}
[extern(cdecl, name="Z3_rcf_is_algebraic", lib=z3)]
def Z3_rcf_is_algebraic(c : _Z3_context? implicit; a : _Z3_rcf_num? implicit) : bool {}
[extern(cdecl, name="Z3_rcf_is_infinitesimal", lib=z3)]
def Z3_rcf_is_infinitesimal(c : _Z3_context? implicit; a : _Z3_rcf_num? implicit) : bool {}
[extern(cdecl, name="Z3_rcf_is_transcendental", lib=z3)]
def Z3_rcf_is_transcendental(c : _Z3_context? implicit; a : _Z3_rcf_num? implicit) : bool {}
[extern(cdecl, name="Z3_rcf_extension_index", lib=z3)]
def Z3_rcf_extension_index(c : _Z3_context? implicit; a : _Z3_rcf_num? implicit) : uint {}
[extern(cdecl, name="Z3_rcf_transcendental_name", lib=z3)]
def Z3_rcf_transcendental_name(c : _Z3_context? implicit; a : _Z3_rcf_num? implicit) : _Z3_symbol? {}
[extern(cdecl, name="Z3_rcf_infinitesimal_name", lib=z3)]
def Z3_rcf_infinitesimal_name(c : _Z3_context? implicit; a : _Z3_rcf_num? implicit) : _Z3_symbol? {}
[extern(cdecl, name="Z3_rcf_num_coefficients", lib=z3)]
def Z3_rcf_num_coefficients(c : _Z3_context? implicit; a : _Z3_rcf_num? implicit) : uint {}
[extern(cdecl, name="Z3_rcf_coefficient", lib=z3)]
def Z3_rcf_coefficient(c : _Z3_context? implicit; a : _Z3_rcf_num? implicit; i : uint) : _Z3_rcf_num? {}
[extern(cdecl, name="Z3_rcf_interval", lib=z3)]
def Z3_rcf_interval(c : _Z3_context? implicit; a : _Z3_rcf_num? implicit; lower_is_inf : int? implicit; lower_is_open : int? implicit; lower : _Z3_rcf_num ?? implicit; upper_is_inf : int? implicit; upper_is_open : int? implicit; upper : _Z3_rcf_num ?? implicit) : int {}
[extern(cdecl, name="Z3_rcf_num_sign_conditions", lib=z3)]
def Z3_rcf_num_sign_conditions(c : _Z3_context? implicit; a : _Z3_rcf_num? implicit) : uint {}
[extern(cdecl, name="Z3_rcf_sign_condition_sign", lib=z3)]
def Z3_rcf_sign_condition_sign(c : _Z3_context? implicit; a : _Z3_rcf_num? implicit; i : uint) : int {}
[extern(cdecl, name="Z3_rcf_num_sign_condition_coefficients", lib=z3)]
def Z3_rcf_num_sign_condition_coefficients(c : _Z3_context? implicit; a : _Z3_rcf_num? implicit; i : uint) : uint {}
[extern(cdecl, name="Z3_rcf_sign_condition_coefficient", lib=z3)]
def Z3_rcf_sign_condition_coefficient(c : _Z3_context? implicit; a : _Z3_rcf_num? implicit; i : uint; j : uint) : _Z3_rcf_num? {}
[extern(cdecl, name="Z3_mk_fixedpoint", lib=z3)]
def Z3_mk_fixedpoint(c : _Z3_context? implicit) : _Z3_fixedpoint? {}
[extern(cdecl, name="Z3_fixedpoint_inc_ref", lib=z3)]
def Z3_fixedpoint_inc_ref(c : _Z3_context? implicit; d : _Z3_fixedpoint? implicit) : void {}
[extern(cdecl, name="Z3_fixedpoint_dec_ref", lib=z3)]
def Z3_fixedpoint_dec_ref(c : _Z3_context? implicit; d : _Z3_fixedpoint? implicit) : void {}
[extern(cdecl, name="Z3_fixedpoint_add_rule", lib=z3)]
def Z3_fixedpoint_add_rule(c : _Z3_context? implicit; d : _Z3_fixedpoint? implicit; rule : _Z3_ast? implicit; name : _Z3_symbol? implicit) : void {}
[extern(cdecl, name="Z3_fixedpoint_add_fact", lib=z3)]
def Z3_fixedpoint_add_fact(c : _Z3_context? implicit; d : _Z3_fixedpoint? implicit; r : _Z3_func_decl? implicit; num_args : uint; args : void?) : void {}
[extern(cdecl, name="Z3_fixedpoint_assert", lib=z3)]
def Z3_fixedpoint_assert(c : _Z3_context? implicit; d : _Z3_fixedpoint? implicit; axiom : _Z3_ast? implicit) : void {}
[extern(cdecl, name="Z3_fixedpoint_query", lib=z3)]
def Z3_fixedpoint_query(c : _Z3_context? implicit; d : _Z3_fixedpoint? implicit; query : _Z3_ast? implicit) : Z3_lbool {}
[extern(cdecl, name="Z3_fixedpoint_query_relations", lib=z3)]
def Z3_fixedpoint_query_relations(c : _Z3_context? implicit; d : _Z3_fixedpoint? implicit; num_relations : uint; relations : void? const) : Z3_lbool {}
[extern(cdecl, name="Z3_fixedpoint_get_answer", lib=z3)]
def Z3_fixedpoint_get_answer(c : _Z3_context? implicit; d : _Z3_fixedpoint? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_fixedpoint_get_reason_unknown", lib=z3)]
def Z3_fixedpoint_get_reason_unknown(c : _Z3_context? implicit; d : _Z3_fixedpoint? implicit) : string {}
[extern(cdecl, name="Z3_fixedpoint_update_rule", lib=z3)]
def Z3_fixedpoint_update_rule(c : _Z3_context? implicit; d : _Z3_fixedpoint? implicit; a : _Z3_ast? implicit; name : _Z3_symbol? implicit) : void {}
[extern(cdecl, name="Z3_fixedpoint_get_num_levels", lib=z3)]
def Z3_fixedpoint_get_num_levels(c : _Z3_context? implicit; d : _Z3_fixedpoint? implicit; pred : _Z3_func_decl? implicit) : uint {}
[extern(cdecl, name="Z3_fixedpoint_get_cover_delta", lib=z3)]
def Z3_fixedpoint_get_cover_delta(c : _Z3_context? implicit; d : _Z3_fixedpoint? implicit; level : int; pred : _Z3_func_decl? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_fixedpoint_add_cover", lib=z3)]
def Z3_fixedpoint_add_cover(c : _Z3_context? implicit; d : _Z3_fixedpoint? implicit; level : int; pred : _Z3_func_decl? implicit; property : _Z3_ast? implicit) : void {}
[extern(cdecl, name="Z3_fixedpoint_get_statistics", lib=z3)]
def Z3_fixedpoint_get_statistics(c : _Z3_context? implicit; d : _Z3_fixedpoint? implicit) : _Z3_stats? {}
[extern(cdecl, name="Z3_fixedpoint_register_relation", lib=z3)]
def Z3_fixedpoint_register_relation(c : _Z3_context? implicit; d : _Z3_fixedpoint? implicit; f : _Z3_func_decl? implicit) : void {}
[extern(cdecl, name="Z3_fixedpoint_set_predicate_representation", lib=z3)]
def Z3_fixedpoint_set_predicate_representation(c : _Z3_context? implicit; d : _Z3_fixedpoint? implicit; f : _Z3_func_decl? implicit; num_relations : uint; relation_kinds : void? const) : void {}
[extern(cdecl, name="Z3_fixedpoint_get_rules", lib=z3)]
def Z3_fixedpoint_get_rules(c : _Z3_context? implicit; f : _Z3_fixedpoint? implicit) : _Z3_ast_vector? {}
[extern(cdecl, name="Z3_fixedpoint_get_assertions", lib=z3)]
def Z3_fixedpoint_get_assertions(c : _Z3_context? implicit; f : _Z3_fixedpoint? implicit) : _Z3_ast_vector? {}
[extern(cdecl, name="Z3_fixedpoint_set_params", lib=z3)]
def Z3_fixedpoint_set_params(c : _Z3_context? implicit; f : _Z3_fixedpoint? implicit; p : _Z3_params? implicit) : void {}
[extern(cdecl, name="Z3_fixedpoint_get_help", lib=z3)]
def Z3_fixedpoint_get_help(c : _Z3_context? implicit; f : _Z3_fixedpoint? implicit) : string {}
[extern(cdecl, name="Z3_fixedpoint_get_param_descrs", lib=z3)]
def Z3_fixedpoint_get_param_descrs(c : _Z3_context? implicit; f : _Z3_fixedpoint? implicit) : _Z3_param_descrs? {}
[extern(cdecl, name="Z3_fixedpoint_to_string", lib=z3)]
def Z3_fixedpoint_to_string(c : _Z3_context? implicit; f : _Z3_fixedpoint? implicit; num_queries : uint; queries : void?) : string {}
[extern(cdecl, name="Z3_fixedpoint_from_string", lib=z3)]
def Z3_fixedpoint_from_string(c : _Z3_context? implicit; f : _Z3_fixedpoint? implicit; s : string implicit) : _Z3_ast_vector? {}
[extern(cdecl, name="Z3_fixedpoint_from_file", lib=z3)]
def Z3_fixedpoint_from_file(c : _Z3_context? implicit; f : _Z3_fixedpoint? implicit; s : string implicit) : _Z3_ast_vector? {}
[extern(cdecl, name="Z3_fixedpoint_init", lib=z3)]
def Z3_fixedpoint_init(c : _Z3_context? implicit; d : _Z3_fixedpoint? implicit; state : void? implicit) : void {}
[extern(cdecl, name="Z3_fixedpoint_set_reduce_assign_callback", lib=z3)]
def Z3_fixedpoint_set_reduce_assign_callback(c : _Z3_context? implicit; d : _Z3_fixedpoint? implicit; cb : void?) : void {}
[extern(cdecl, name="Z3_fixedpoint_set_reduce_app_callback", lib=z3)]
def Z3_fixedpoint_set_reduce_app_callback(c : _Z3_context? implicit; d : _Z3_fixedpoint? implicit; cb : void?) : void {}
[extern(cdecl, name="Z3_fixedpoint_add_constraint", lib=z3)]
def Z3_fixedpoint_add_constraint(c : _Z3_context? implicit; d : _Z3_fixedpoint? implicit; e : _Z3_ast? implicit; lvl : uint) : void {}
[extern(cdecl, name="Z3_mk_optimize", lib=z3)]
def Z3_mk_optimize(c : _Z3_context? implicit) : _Z3_optimize? {}
[extern(cdecl, name="Z3_optimize_inc_ref", lib=z3)]
def Z3_optimize_inc_ref(c : _Z3_context? implicit; d : _Z3_optimize? implicit) : void {}
[extern(cdecl, name="Z3_optimize_dec_ref", lib=z3)]
def Z3_optimize_dec_ref(c : _Z3_context? implicit; d : _Z3_optimize? implicit) : void {}
[extern(cdecl, name="Z3_optimize_assert", lib=z3)]
def Z3_optimize_assert(c : _Z3_context? implicit; o : _Z3_optimize? implicit; a : _Z3_ast? implicit) : void {}
[extern(cdecl, name="Z3_optimize_assert_and_track", lib=z3)]
def Z3_optimize_assert_and_track(c : _Z3_context? implicit; o : _Z3_optimize? implicit; a : _Z3_ast? implicit; t : _Z3_ast? implicit) : void {}
[extern(cdecl, name="Z3_optimize_assert_soft", lib=z3)]
def Z3_optimize_assert_soft(c : _Z3_context? implicit; o : _Z3_optimize? implicit; a : _Z3_ast? implicit; weight : string implicit; id : _Z3_symbol? implicit) : uint {}
[extern(cdecl, name="Z3_optimize_maximize", lib=z3)]
def Z3_optimize_maximize(c : _Z3_context? implicit; o : _Z3_optimize? implicit; t : _Z3_ast? implicit) : uint {}
[extern(cdecl, name="Z3_optimize_minimize", lib=z3)]
def Z3_optimize_minimize(c : _Z3_context? implicit; o : _Z3_optimize? implicit; t : _Z3_ast? implicit) : uint {}
[extern(cdecl, name="Z3_optimize_push", lib=z3)]
def Z3_optimize_push(c : _Z3_context? implicit; d : _Z3_optimize? implicit) : void {}
[extern(cdecl, name="Z3_optimize_pop", lib=z3)]
def Z3_optimize_pop(c : _Z3_context? implicit; d : _Z3_optimize? implicit) : void {}
[extern(cdecl, name="Z3_optimize_set_initial_value", lib=z3)]
def Z3_optimize_set_initial_value(c : _Z3_context? implicit; o : _Z3_optimize? implicit; v : _Z3_ast? implicit; val : _Z3_ast? implicit) : void {}
[extern(cdecl, name="Z3_optimize_check", lib=z3)]
def Z3_optimize_check(c : _Z3_context? implicit; o : _Z3_optimize? implicit; num_assumptions : uint; assumptions : void? const) : Z3_lbool {}
[extern(cdecl, name="Z3_optimize_get_reason_unknown", lib=z3)]
def Z3_optimize_get_reason_unknown(c : _Z3_context? implicit; d : _Z3_optimize? implicit) : string {}
[extern(cdecl, name="Z3_optimize_get_model", lib=z3)]
def Z3_optimize_get_model(c : _Z3_context? implicit; o : _Z3_optimize? implicit) : _Z3_model? {}
[extern(cdecl, name="Z3_optimize_get_unsat_core", lib=z3)]
def Z3_optimize_get_unsat_core(c : _Z3_context? implicit; o : _Z3_optimize? implicit) : _Z3_ast_vector? {}
[extern(cdecl, name="Z3_optimize_set_params", lib=z3)]
def Z3_optimize_set_params(c : _Z3_context? implicit; o : _Z3_optimize? implicit; p : _Z3_params? implicit) : void {}
[extern(cdecl, name="Z3_optimize_get_param_descrs", lib=z3)]
def Z3_optimize_get_param_descrs(c : _Z3_context? implicit; o : _Z3_optimize? implicit) : _Z3_param_descrs? {}
[extern(cdecl, name="Z3_optimize_get_lower", lib=z3)]
def Z3_optimize_get_lower(c : _Z3_context? implicit; o : _Z3_optimize? implicit; idx : uint) : _Z3_ast? {}
[extern(cdecl, name="Z3_optimize_get_upper", lib=z3)]
def Z3_optimize_get_upper(c : _Z3_context? implicit; o : _Z3_optimize? implicit; idx : uint) : _Z3_ast? {}
[extern(cdecl, name="Z3_optimize_get_lower_as_vector", lib=z3)]
def Z3_optimize_get_lower_as_vector(c : _Z3_context? implicit; o : _Z3_optimize? implicit; idx : uint) : _Z3_ast_vector? {}
[extern(cdecl, name="Z3_optimize_get_upper_as_vector", lib=z3)]
def Z3_optimize_get_upper_as_vector(c : _Z3_context? implicit; o : _Z3_optimize? implicit; idx : uint) : _Z3_ast_vector? {}
[extern(cdecl, name="Z3_optimize_to_string", lib=z3)]
def Z3_optimize_to_string(c : _Z3_context? implicit; o : _Z3_optimize? implicit) : string {}
[extern(cdecl, name="Z3_optimize_from_string", lib=z3)]
def Z3_optimize_from_string(c : _Z3_context? implicit; o : _Z3_optimize? implicit; s : string implicit) : void {}
[extern(cdecl, name="Z3_optimize_from_file", lib=z3)]
def Z3_optimize_from_file(c : _Z3_context? implicit; o : _Z3_optimize? implicit; s : string implicit) : void {}
[extern(cdecl, name="Z3_optimize_get_help", lib=z3)]
def Z3_optimize_get_help(c : _Z3_context? implicit; t : _Z3_optimize? implicit) : string {}
[extern(cdecl, name="Z3_optimize_get_statistics", lib=z3)]
def Z3_optimize_get_statistics(c : _Z3_context? implicit; d : _Z3_optimize? implicit) : _Z3_stats? {}
[extern(cdecl, name="Z3_optimize_get_assertions", lib=z3)]
def Z3_optimize_get_assertions(c : _Z3_context? implicit; o : _Z3_optimize? implicit) : _Z3_ast_vector? {}
[extern(cdecl, name="Z3_optimize_get_objectives", lib=z3)]
def Z3_optimize_get_objectives(c : _Z3_context? implicit; o : _Z3_optimize? implicit) : _Z3_ast_vector? {}
[extern(cdecl, name="Z3_optimize_register_model_eh", lib=z3)]
def Z3_optimize_register_model_eh(c : _Z3_context? implicit; o : _Z3_optimize? implicit; m : _Z3_model? implicit; ctx : void? implicit; model_eh : void?) : void {}
[extern(cdecl, name="Z3_mk_fpa_rounding_mode_sort", lib=z3)]
def Z3_mk_fpa_rounding_mode_sort(c : _Z3_context? implicit) : _Z3_sort? {}
[extern(cdecl, name="Z3_mk_fpa_round_nearest_ties_to_even", lib=z3)]
def Z3_mk_fpa_round_nearest_ties_to_even(c : _Z3_context? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_rne", lib=z3)]
def Z3_mk_fpa_rne(c : _Z3_context? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_round_nearest_ties_to_away", lib=z3)]
def Z3_mk_fpa_round_nearest_ties_to_away(c : _Z3_context? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_rna", lib=z3)]
def Z3_mk_fpa_rna(c : _Z3_context? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_round_toward_positive", lib=z3)]
def Z3_mk_fpa_round_toward_positive(c : _Z3_context? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_rtp", lib=z3)]
def Z3_mk_fpa_rtp(c : _Z3_context? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_round_toward_negative", lib=z3)]
def Z3_mk_fpa_round_toward_negative(c : _Z3_context? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_rtn", lib=z3)]
def Z3_mk_fpa_rtn(c : _Z3_context? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_round_toward_zero", lib=z3)]
def Z3_mk_fpa_round_toward_zero(c : _Z3_context? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_rtz", lib=z3)]
def Z3_mk_fpa_rtz(c : _Z3_context? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_sort", lib=z3)]
def Z3_mk_fpa_sort(c : _Z3_context? implicit; ebits : uint; sbits : uint) : _Z3_sort? {}
[extern(cdecl, name="Z3_mk_fpa_sort_half", lib=z3)]
def Z3_mk_fpa_sort_half(c : _Z3_context? implicit) : _Z3_sort? {}
[extern(cdecl, name="Z3_mk_fpa_sort_16", lib=z3)]
def Z3_mk_fpa_sort_16(c : _Z3_context? implicit) : _Z3_sort? {}
[extern(cdecl, name="Z3_mk_fpa_sort_single", lib=z3)]
def Z3_mk_fpa_sort_single(c : _Z3_context? implicit) : _Z3_sort? {}
[extern(cdecl, name="Z3_mk_fpa_sort_32", lib=z3)]
def Z3_mk_fpa_sort_32(c : _Z3_context? implicit) : _Z3_sort? {}
[extern(cdecl, name="Z3_mk_fpa_sort_double", lib=z3)]
def Z3_mk_fpa_sort_double(c : _Z3_context? implicit) : _Z3_sort? {}
[extern(cdecl, name="Z3_mk_fpa_sort_64", lib=z3)]
def Z3_mk_fpa_sort_64(c : _Z3_context? implicit) : _Z3_sort? {}
[extern(cdecl, name="Z3_mk_fpa_sort_quadruple", lib=z3)]
def Z3_mk_fpa_sort_quadruple(c : _Z3_context? implicit) : _Z3_sort? {}
[extern(cdecl, name="Z3_mk_fpa_sort_128", lib=z3)]
def Z3_mk_fpa_sort_128(c : _Z3_context? implicit) : _Z3_sort? {}
[extern(cdecl, name="Z3_mk_fpa_nan", lib=z3)]
def Z3_mk_fpa_nan(c : _Z3_context? implicit; s : _Z3_sort? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_inf", lib=z3)]
def Z3_mk_fpa_inf(c : _Z3_context? implicit; s : _Z3_sort? implicit; negative : bool) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_zero", lib=z3)]
def Z3_mk_fpa_zero(c : _Z3_context? implicit; s : _Z3_sort? implicit; negative : bool) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_fp", lib=z3)]
def Z3_mk_fpa_fp(c : _Z3_context? implicit; sgn : _Z3_ast? implicit; exp : _Z3_ast? implicit; sig : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_numeral_float", lib=z3)]
def Z3_mk_fpa_numeral_float(c : _Z3_context? implicit; v : float; ty : _Z3_sort? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_numeral_double", lib=z3)]
def Z3_mk_fpa_numeral_double(c : _Z3_context? implicit; v : double; ty : _Z3_sort? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_numeral_int", lib=z3)]
def Z3_mk_fpa_numeral_int(c : _Z3_context? implicit; v : int; ty : _Z3_sort? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_numeral_int_uint", lib=z3)]
def Z3_mk_fpa_numeral_int_uint(c : _Z3_context? implicit; sgn : bool; exp : int; sig : uint; ty : _Z3_sort? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_numeral_int64_uint64", lib=z3)]
def Z3_mk_fpa_numeral_int64_uint64(c : _Z3_context? implicit; sgn : bool; exp : int64; sig : uint64; ty : _Z3_sort? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_abs", lib=z3)]
def Z3_mk_fpa_abs(c : _Z3_context? implicit; t : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_neg", lib=z3)]
def Z3_mk_fpa_neg(c : _Z3_context? implicit; t : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_add", lib=z3)]
def Z3_mk_fpa_add(c : _Z3_context? implicit; rm : _Z3_ast? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_sub", lib=z3)]
def Z3_mk_fpa_sub(c : _Z3_context? implicit; rm : _Z3_ast? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_mul", lib=z3)]
def Z3_mk_fpa_mul(c : _Z3_context? implicit; rm : _Z3_ast? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_div", lib=z3)]
def Z3_mk_fpa_div(c : _Z3_context? implicit; rm : _Z3_ast? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_fma", lib=z3)]
def Z3_mk_fpa_fma(c : _Z3_context? implicit; rm : _Z3_ast? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit; t3 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_sqrt", lib=z3)]
def Z3_mk_fpa_sqrt(c : _Z3_context? implicit; rm : _Z3_ast? implicit; t : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_rem", lib=z3)]
def Z3_mk_fpa_rem(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_round_to_integral", lib=z3)]
def Z3_mk_fpa_round_to_integral(c : _Z3_context? implicit; rm : _Z3_ast? implicit; t : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_min", lib=z3)]
def Z3_mk_fpa_min(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_max", lib=z3)]
def Z3_mk_fpa_max(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_leq", lib=z3)]
def Z3_mk_fpa_leq(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_lt", lib=z3)]
def Z3_mk_fpa_lt(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_geq", lib=z3)]
def Z3_mk_fpa_geq(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_gt", lib=z3)]
def Z3_mk_fpa_gt(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_eq", lib=z3)]
def Z3_mk_fpa_eq(c : _Z3_context? implicit; t1 : _Z3_ast? implicit; t2 : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_is_normal", lib=z3)]
def Z3_mk_fpa_is_normal(c : _Z3_context? implicit; t : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_is_subnormal", lib=z3)]
def Z3_mk_fpa_is_subnormal(c : _Z3_context? implicit; t : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_is_zero", lib=z3)]
def Z3_mk_fpa_is_zero(c : _Z3_context? implicit; t : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_is_infinite", lib=z3)]
def Z3_mk_fpa_is_infinite(c : _Z3_context? implicit; t : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_is_nan", lib=z3)]
def Z3_mk_fpa_is_nan(c : _Z3_context? implicit; t : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_is_negative", lib=z3)]
def Z3_mk_fpa_is_negative(c : _Z3_context? implicit; t : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_is_positive", lib=z3)]
def Z3_mk_fpa_is_positive(c : _Z3_context? implicit; t : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_to_fp_bv", lib=z3)]
def Z3_mk_fpa_to_fp_bv(c : _Z3_context? implicit; bv : _Z3_ast? implicit; s : _Z3_sort? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_to_fp_float", lib=z3)]
def Z3_mk_fpa_to_fp_float(c : _Z3_context? implicit; rm : _Z3_ast? implicit; t : _Z3_ast? implicit; s : _Z3_sort? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_to_fp_real", lib=z3)]
def Z3_mk_fpa_to_fp_real(c : _Z3_context? implicit; rm : _Z3_ast? implicit; t : _Z3_ast? implicit; s : _Z3_sort? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_to_fp_signed", lib=z3)]
def Z3_mk_fpa_to_fp_signed(c : _Z3_context? implicit; rm : _Z3_ast? implicit; t : _Z3_ast? implicit; s : _Z3_sort? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_to_fp_unsigned", lib=z3)]
def Z3_mk_fpa_to_fp_unsigned(c : _Z3_context? implicit; rm : _Z3_ast? implicit; t : _Z3_ast? implicit; s : _Z3_sort? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_to_ubv", lib=z3)]
def Z3_mk_fpa_to_ubv(c : _Z3_context? implicit; rm : _Z3_ast? implicit; t : _Z3_ast? implicit; sz : uint) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_to_sbv", lib=z3)]
def Z3_mk_fpa_to_sbv(c : _Z3_context? implicit; rm : _Z3_ast? implicit; t : _Z3_ast? implicit; sz : uint) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_to_real", lib=z3)]
def Z3_mk_fpa_to_real(c : _Z3_context? implicit; t : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_fpa_get_ebits", lib=z3)]
def Z3_fpa_get_ebits(c : _Z3_context? implicit; s : _Z3_sort? implicit) : uint {}
[extern(cdecl, name="Z3_fpa_get_sbits", lib=z3)]
def Z3_fpa_get_sbits(c : _Z3_context? implicit; s : _Z3_sort? implicit) : uint {}
[extern(cdecl, name="Z3_fpa_is_numeral_nan", lib=z3)]
def Z3_fpa_is_numeral_nan(c : _Z3_context? implicit; t : _Z3_ast? implicit) : bool {}
[extern(cdecl, name="Z3_fpa_is_numeral_inf", lib=z3)]
def Z3_fpa_is_numeral_inf(c : _Z3_context? implicit; t : _Z3_ast? implicit) : bool {}
[extern(cdecl, name="Z3_fpa_is_numeral_zero", lib=z3)]
def Z3_fpa_is_numeral_zero(c : _Z3_context? implicit; t : _Z3_ast? implicit) : bool {}
[extern(cdecl, name="Z3_fpa_is_numeral_normal", lib=z3)]
def Z3_fpa_is_numeral_normal(c : _Z3_context? implicit; t : _Z3_ast? implicit) : bool {}
[extern(cdecl, name="Z3_fpa_is_numeral_subnormal", lib=z3)]
def Z3_fpa_is_numeral_subnormal(c : _Z3_context? implicit; t : _Z3_ast? implicit) : bool {}
[extern(cdecl, name="Z3_fpa_is_numeral_positive", lib=z3)]
def Z3_fpa_is_numeral_positive(c : _Z3_context? implicit; t : _Z3_ast? implicit) : bool {}
[extern(cdecl, name="Z3_fpa_is_numeral_negative", lib=z3)]
def Z3_fpa_is_numeral_negative(c : _Z3_context? implicit; t : _Z3_ast? implicit) : bool {}
[extern(cdecl, name="Z3_fpa_get_numeral_sign_bv", lib=z3)]
def Z3_fpa_get_numeral_sign_bv(c : _Z3_context? implicit; t : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_fpa_get_numeral_significand_bv", lib=z3)]
def Z3_fpa_get_numeral_significand_bv(c : _Z3_context? implicit; t : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_fpa_get_numeral_sign", lib=z3)]
def Z3_fpa_get_numeral_sign(c : _Z3_context? implicit; t : _Z3_ast? implicit; sgn : int? implicit) : bool {}
[extern(cdecl, name="Z3_fpa_get_numeral_significand_string", lib=z3)]
def Z3_fpa_get_numeral_significand_string(c : _Z3_context? implicit; t : _Z3_ast? implicit) : string {}
[extern(cdecl, name="Z3_fpa_get_numeral_significand_uint64", lib=z3)]
def Z3_fpa_get_numeral_significand_uint64(c : _Z3_context? implicit; t : _Z3_ast? implicit; n : uint64? implicit) : bool {}
[extern(cdecl, name="Z3_fpa_get_numeral_exponent_string", lib=z3)]
def Z3_fpa_get_numeral_exponent_string(c : _Z3_context? implicit; t : _Z3_ast? implicit; biased : bool) : string {}
[extern(cdecl, name="Z3_fpa_get_numeral_exponent_int64", lib=z3)]
def Z3_fpa_get_numeral_exponent_int64(c : _Z3_context? implicit; t : _Z3_ast? implicit; n : int64? implicit; biased : bool) : bool {}
[extern(cdecl, name="Z3_fpa_get_numeral_exponent_bv", lib=z3)]
def Z3_fpa_get_numeral_exponent_bv(c : _Z3_context? implicit; t : _Z3_ast? implicit; biased : bool) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_to_ieee_bv", lib=z3)]
def Z3_mk_fpa_to_ieee_bv(c : _Z3_context? implicit; t : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_mk_fpa_to_fp_int_real", lib=z3)]
def Z3_mk_fpa_to_fp_int_real(c : _Z3_context? implicit; rm : _Z3_ast? implicit; exp : _Z3_ast? implicit; sig : _Z3_ast? implicit; s : _Z3_sort? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_fixedpoint_query_from_lvl", lib=z3)]
def Z3_fixedpoint_query_from_lvl(c : _Z3_context? implicit; d : _Z3_fixedpoint? implicit; query : _Z3_ast? implicit; lvl : uint) : Z3_lbool {}
[extern(cdecl, name="Z3_fixedpoint_get_ground_sat_answer", lib=z3)]
def Z3_fixedpoint_get_ground_sat_answer(c : _Z3_context? implicit; d : _Z3_fixedpoint? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_fixedpoint_get_rules_along_trace", lib=z3)]
def Z3_fixedpoint_get_rules_along_trace(c : _Z3_context? implicit; d : _Z3_fixedpoint? implicit) : _Z3_ast_vector? {}
[extern(cdecl, name="Z3_fixedpoint_get_rule_names_along_trace", lib=z3)]
def Z3_fixedpoint_get_rule_names_along_trace(c : _Z3_context? implicit; d : _Z3_fixedpoint? implicit) : _Z3_symbol? {}
[extern(cdecl, name="Z3_fixedpoint_add_invariant", lib=z3)]
def Z3_fixedpoint_add_invariant(c : _Z3_context? implicit; d : _Z3_fixedpoint? implicit; pred : _Z3_func_decl? implicit; property : _Z3_ast? implicit) : void {}
[extern(cdecl, name="Z3_fixedpoint_get_reachable", lib=z3)]
def Z3_fixedpoint_get_reachable(c : _Z3_context? implicit; d : _Z3_fixedpoint? implicit; pred : _Z3_func_decl? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_qe_model_project", lib=z3)]
def Z3_qe_model_project(c : _Z3_context? implicit; m : _Z3_model? implicit; num_bounds : uint; bound : void? const; body : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_qe_model_project_skolem", lib=z3)]
def Z3_qe_model_project_skolem(c : _Z3_context? implicit; m : _Z3_model? implicit; num_bounds : uint; bound : void? const; body : _Z3_ast? implicit; map : _Z3_ast_map? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_qe_model_project_with_witness", lib=z3)]
def Z3_qe_model_project_with_witness(c : _Z3_context? implicit; m : _Z3_model? implicit; num_bounds : uint; bound : void? const; body : _Z3_ast? implicit; map : _Z3_ast_map? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_model_extrapolate", lib=z3)]
def Z3_model_extrapolate(c : _Z3_context? implicit; m : _Z3_model? implicit; fml : _Z3_ast? implicit) : _Z3_ast? {}
[extern(cdecl, name="Z3_qe_lite", lib=z3)]
def Z3_qe_lite(c : _Z3_context? implicit; vars : _Z3_ast_vector? implicit; body : _Z3_ast? implicit) : _Z3_ast? {}
