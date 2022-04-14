options rtti=true

require fio
require rtti
require ast
require math
require uriparser
require strings
require jobque
require daslib/ast_boost
require daslib/strings_boost
require daslib/rst_comment
require daslib/rst
require daslib/functional
require daslib/json
require daslib/json_boost
require daslib/regex
require daslib/regex_boost
require daslib/apply
require daslib/algorithm
require daslib/jobque_boost
require daslib/apply_in_context
require daslib/contracts
require daslib/defer
require daslib/if_not_null
require daslib/templates
require daslib/templates_boost
require daslib/instance_function
require daslib/is_local
require daslib/safe_addr
require daslib/static_let
require daslib/lpipe
require daslib/random
require daslib/math_bits
require daslib/math_boost

def document_module_math(root:string)
    var mod = get_module("math")
    var groups <- [{DocGroup
        group_by_regex("all numerics (uint*, int*, float*, double)", mod, %regex~(min|max)$%%);
        group_by_regex("float* and double", mod, %regex~(sign|abs|acos|asin|atan|atan2|cos|sin|tan|exp|log|exp2|
log2|pow|sqrt|rcp|ceil|floor|abs|saturate|sincos|is_finite|is_nan)$%%);
        group_by_regex("float* only", mod, %regex~(atan2_est|rcp_est|ceili|floori|roundi|trunci|rsqrt|rsqrt_est)$%%);
        group_by_regex("float3 only", mod, %regex~(cross|reflect|refract|distance|distance_sq|
inv_distance|inv_distance_sq)$%%);
        group_by_regex("float2, float3, float4", mod, %regex~(length|length_sq|inv_length|inv_length_sq|
dot|normalize|fast_normalize)$%%);
        group_by_regex("Noise functions", mod, %regex~(uint32_hash|uint_noise_1D|uint_noise_2D|uint_noise_3D)$%%);
        group_by_regex("lerp/mad/clamp", mod, %regex~(lerp|mad|clamp)$%%);
        group_by_regex("Matrix operations", mod, %regex~(\=\=|\!\=|\*)$%%);
        group_by_regex("Matrix initializers", mod, %regex~(float3x4|float4x4|float3x3|identity3x4|identity4x4|identity3x3)$%%);
        group_by_regex("Matrix manipulation", mod, %regex~(identity|inverse|rotate|transpose|translation|compose|decompose|look_at|orthonormal_inverse|persp_forward|persp_reverse)$%%);
        group_by_regex("Quaternion operations", mod, %regex~(quat_conjugate|quat_mul|quat_mul_vec|un_quat|un_quat_from_unit_arc|un_quat_from_unit_vec_ang)$%%);
        group_by_regex("Packing and unpacking", mod, %regex~(pack_float_to_byte|unpack_byte_to_float)$%%)
    }]
    document("Math library",mod,"{root}/math.rst","{root}/detail/math.rst",groups)

def document_module_builtin(root:string)
    var mod = get_module("$")
    var groups <- [{DocGroup
        hide_group(group_by_regex("Internal builtin functions", mod, %regex~builtin%%));
        hide_group(group_by_regex("Internal pointer arithmetics", mod, %regex~i_das_%%));
        hide_group(group_by_regex("Internal clone infrastructure", mod, %regex~clone%%));
        hide_group(group_by_regex("Internal finalize infrastructure", mod, %regex~finalize%%));
        group_by_regex("Containers", mod, %regex~(capacity|clear|length|resize|reserve|each|emplace|erase|find|
find_for_edit|find_if_exists|find_index|find_index_if|has_value|key_exists|keys|values|lock|each_enum|each_ref|
find_for_edit_if_exists|lock_forever|next|nothing|pop|push|push_clone|back|sort|to_array|to_table|to_array_move|
to_table_move|empty|subarray)$%%);
        group_by_regex("Character set groups", mod, %regex~(is_alpha|is_number|is_white_space|is_char_in_set)$%%);
        group_by_regex("das::string manipulation", mod, %regex~(peek|set)$%%);
        group_by_regex("String builder", mod, %regex~(build_string|write|write_char|write_chars|write_escape_string)$%%);
        group_by_regex("Heap reporting", mod, %regex~(heap|string_heap|memory_report)%%);
        group_by_regex("Vector and matrix math", mod, %regex~(float3x4|float4x4|identity|inverse|rotate|translation|transpose)$%%);
        group_by_regex("GC0 infrastructure", mod, %regex~gc0%%);
        group_by_regex("Smart ptr infrastructure", mod, %regex~(smart_ptr|get_const_ptr|get_ptr$)%%);
        group_by_regex("Macro infrastructure", mod, %regex~is_compiling%%);
        group_by_regex("Profiler", mod, %regex~(profile|reset_profiler|dump_profile_info|collect_profile_info)$%%);
        group_by_regex("System infastructure", mod, %regex~(panic|print|sprint|to_log|error|terminate|breakpoint|stackwalk|get_das_root|is_in_aot)$%%);
        group_by_regex("Memory manipulation", mod, %regex~(intptr|memcmp|variant_index|set_variant_index|hash|memcpy|lock_data|map_to_array|map_to_ro_array)$%%);
        group_by_regex("Binary serializer", mod, %regex~(binary_load|binary_save)$%%);
        group_by_regex("Path and command line", mod, %regex~(get_command_line_arguments)$%%);
        group_by_regex("Time and date", mod, %regex~(get_time_usec|ref_time_ticks|get_clock)$%%);
        group_by_regex("Algorithms", mod, %regex~(swap|iter_range)$%%)
    }]
    document("Built-in runtime",mod,"{root}/builtin.rst","{root}/detail/builtin.rst",groups)

def document_module_fio(root:string)
    var mod = get_module("fio")
    var groups <- [{DocGroup
        hide_group(group_by_regex("Internal builtin functions", mod, %regex~builtin%%));
        group_by_regex("File manipulation", mod, %regex~(f|stat$|getchar)%%);
        group_by_regex("Path manipulation", mod, %regex~(base_name|dir_name|get_full_file_name)$%%);
        group_by_regex("Directory manipulation", mod, %regex~(dir|mkdir)$%%);
        group_by_regex("OS specific routines", mod, %regex~(sleep|exit|popen)$%%)
    }]
    document("File input output library",mod,"{root}/fio.rst","{root}/detail/fio.rst",groups)

def document_module_random(root:string)
    var mod = get_module("random")
    var groups <- [{DocGroup
        group_by_regex("Seed and basic generators", mod, %regex~(random_seed|random_seed2D|random_float|random_float4|random_int|random_big_int|random_int4)$%%);
        group_by_regex("Specific distributions", mod, %regex~(random_unit_vector|random_in_unit_disk|random_in_unit_sphere)$%%)
    }]
    document("Random generator library",mod,"{root}/random.rst","{root}/detail/random.rst",groups)

def document_module_network(root:string)
    var mod = get_module("network")
    var groups <- [{DocGroup
        group_by_regex("Low lever NetworkServer IO", mod, %regex~(make_server|server_init|server_is_open|server_is_connected|server_tick|server_send|server_restore)$%%)
    }]
    document("Network socket library",mod,"{root}/network.rst","{root}/detail/network.rst",groups)

def document_module_uriparser(root:string)
    var mod = get_module("uriparser")
    var groups <- [{DocGroup
        group_by_regex("Initialization and finalization", mod, %regex~(Uri|using|clone|finalize)$%%);
        group_by_regex("Escape and unescape", mod, %regex~(escape_uri|unescape_uri)$%%);
        group_by_regex("Uri manipulations", mod, %regex~(normalize_uri|unescape_uri|string|uri_for_each_query_kv|remove_base_uri|add_base_uri|strip_uri|normalize)$%%);
        group_by_regex("File name conversions", mod, %regex~(to_unix_file_name|to_windows_file_name|to_file_name|uri_from_file_name|uri_from_windows_file_name|uri_from_unix_file_name|uri_to_unix_file_name|uri_to_windows_file_name|unix_file_name_to_uri|windows_file_name_to_uri|uri_to_file_name|file_name_to_uri)$%%);
        group_by_regex("GUID", mod, %regex~(make_new_guid)$%%)
    }]
    document("URI manipulation library based on UriParser",mod,"{root}/uriparser.rst","{root}/detail/uriparser.rst",groups)

def document_module_rtti(root:string)
    var mod = get_module("rtti")
    var groups <- [{DocGroup
        group_by_regex("Initialization and finalization", mod, %regex~(using|LineInfo|CodeOfPolicies|RttiValue_nothing)$%%);
        group_by_regex("Type access", mod, %regex~(get_dim|builtin_is_same_type|is_compatible_cast|get_das_type_name|is_same_type|each_dim|arg_types|arg_names)$%%);
        group_by_regex("Rtti context access", mod, %regex~(get_total_functions|get_total_variables|get_function_info|get_variable_info|get_variable_value|get_function_by_mnh|get_line_info|this_context|context_for_each_function|context_for_each_variable|class_info|type_info)$%%);
        group_by_regex("Program access", mod, %regex~(program_for_each_module|program_for_each_registered_module|get_this_module|get_module)$%%);
        group_by_regex("Module access", mod, %regex~(module_for_each_structure|module_for_each_enumeration|module_for_each_function|module_for_each_generic|module_for_each_global|module_for_each_annotation)$%%);
        group_by_regex("Annotation access", mod, %regex~(get_annotation_argument_value|add_annotation_argument)$%%);
        group_by_regex("Compilation and simulation", mod, %regex~(compile|compile_file|for_each_expected_error|for_each_require_declaration|simulate)$%%);
        group_by_regex("File access", mod, %regex~(make_file_access|set_file_source|add_file_access_root)$%%);
        group_by_regex("Structure access", mod, %regex~(rtti_builtin_structure_for_each_annotation|basic_struct_for_each_field|structure_for_each_annotation)$%%);
        group_by_regex("Data walking and printing", mod, %regex~(sprint_data|describe|get_mangled_name)$%%);
        group_by_regex("Function and mangled name hash", mod, %regex~(get_function_by_mangled_name_hash|get_function_mangled_name_hash)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("Runtime type information library",mod,"{root}/rtti.rst","{root}/detail/rtti.rst",groups)

def document_module_ast(root:string)
    var mod = get_module("ast")
    var groups <- [{DocGroup
        group_by_regex("Call generation", mod, %regex~(make_call)$%%);
        group_by_regex("Visitor pattern", mod, %regex~visit.*%%);
        group_by_regex("Adapter generation", mod, %regex~make_.*%%);
        group_by_regex("Adapter application", mod, %regex~(add_.*_(annotation|macro)|add_new_comment_reader|add_comment_reader)%%);
        group_by_regex("Adding objects to objects", mod, %regex~add_.*%%);
        group_by_regex("Program and module access", mod, %regex~(this_program|this_module|compiling_program|compiling_module)$%%);
        group_by_regex("Textual descriptions of the objects", mod, %regex~(describe.*|das_to_string)%%);
        group_by_regex("Searching", mod, %regex~find.*%%);
        group_by_regex("Iterating", mod, %regex~(for_each.*|.*_foreach)%%);
        group_by_regex("Cloning", mod, %regex~clone.*%%);
        group_by_regex("Mangled name", mod, %regex~(parse_mangled_name|get_mangled_name)%%);
        group_by_regex("Size and offset", mod, %regex~.*_(size|offset)%%);
        group_by_regex("Properties", mod, %regex~(is|has)_.*%%);
        group_by_regex("Pointer conversion", mod, %regex~(ExpressionPtr|FunctionPtr)%%);
        group_by_regex("Evaluations", mod, %regex~(eval_single_expression)%%);
        group_by_regex("Error reporting", mod, %regex~(macro_error)%%);
        group_by_regex("Location and context", mod, %regex~(force_at|collect_dependencies|get_ast_context)%%)
    }]
    document("AST manipulation library",mod,"{root}/ast.rst","{root}/detail/ast.rst",groups)

def document_module_ast_boost(root:string)
    var mod = find_module("ast_boost")
    var groups <- [{DocGroup
        group_by_regex("Containers", mod, %regex~(emplace_new)%%);
        group_by_regex("Textual descriptions of the objects", mod, %regex~describe.*%%);
        group_by_regex("Queries", mod, %regex~(is.*|find.*|get.*)%%);
        group_by_regex("Annotations", mod, %regex~(append.*|add.*)%%);
        group_by_regex("Expression generation", mod, %regex~(override_method|panic_expr_as|make_static_assert_false|convert_to_expression)%%);
        group_by_regex("Setup", mod, %regex~setup.*$%%)
    }]
    document("Boost package for the AST",mod,"{root}/ast_boost.rst","{root}/detail/ast_boost.rst",groups)

def document_module_strings(root:string)
    var mod = find_module("strings")
    var groups <- [{DocGroup
        hide_group(group_by_regex("Internal builtin functions", mod, %regex~builtin%%));
        group_by_regex("Character groups", mod, %regex~is.+%%);
        group_by_regex("Character by index", mod, %regex~(character_at|character_uat)$%%);
        group_by_regex("String properties", mod, %regex~(empty|ends_with|length|starts_with)$%%);
        group_by_regex("String builder", mod, %regex~(build_string|format|write|write_char|write_chars|write_escape_string)$%%);
        group_by_regex("das::string manipulation", mod, %regex~(append|resize)$%%);
        group_by_regex("String modifications", mod, %regex~(chop|escape|unescape|repeat|replace|reverse|slice|
strip|strip_left|strip_right|to_lower|to_lower_in_place|to_upper|to_upper_in_place|rtrim)$%%);
        group_by_regex("Search substrings", mod, %regex~find.*%%);
        group_by_regex("String conversion routines", mod, %regex~(double|float|int|uint|to_float|to_int|to_uint|to_int64|to_uint64|string|to_char)$%%);
        group_by_regex("String as array", mod, %regex~(modify_data|peek_data)$%%);
        group_by_regex("Low level memory allocation", mod, %regex~(delete_string|reserve_string_buffer)$%%)
    }]
    document("String manipulation library",mod,"{root}/strings.rst","{root}/detail/strings.rst",groups)

def document_module_strings_boost(root:string)
    var mod = find_module("strings_boost")
    var groups <- [{DocGroup
        group_by_regex("Split and join", mod, %regex~(split|split_by_chars|join)$%%);
        group_by_regex("Formatting", mod, %regex~(wide)$%%);
        group_by_regex("Queries and comparisons", mod, %regex~(is_character_at|eq)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("Boost package for string manipulation library",mod,"{root}/strings_boost.rst","{root}/detail/strings_boost.rst",groups)


def document_module_functional(root:string)
    var mod = find_module("functional")
    var groups <- [{DocGroup
        group_by_regex("Map, reduce", mod, %regex~(filter|map|reduce|any|sum|all|cycle|islice|count|repeat_ref|repeat|not|echo|flatten)$%%);
        group_by_regex("Queries", mod, %regex~(is_equal|is_not_equal)$%%)
    }]
    document("Functional programming library",mod,"{root}/functional.rst","{root}/detail/functional.rst",groups)

def document_module_json(root:string)
    var mod = find_module("json")
    var groups <- [{DocGroup
        group_by_regex("Value conversion", mod, %regex~(JV|JVNull)$%%);
        group_by_regex("Read and write", mod, %regex~(read_json|write_json)$%%)
    }]
    document("JSON manipulation library",mod,"{root}/json.rst","{root}/detail/json.rst",groups)

def document_module_json_boost(root:string)
    var mod = find_module("json_boost")
    var groups <- [{DocGroup
        group_by_regex("Value conversion", mod, %regex~(JV|from_JV)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("Boost package for JSON",mod,"{root}/json_boost.rst","{root}/detail/json_boost.rst",groups)

def document_module_regex(root:string)
    var mod = find_module("regex")
    var groups <- [{DocGroup
        group_by_regex("Compilation and validation", mod, %regex~(regex_compile|is_valid|regex_debug|debug_set)$%%);
        group_by_regex("Access", mod, %regex~(regex_group|regex_foreach)$%%);
        group_by_regex("Match", mod, %regex~(regex_match)$%%)
    }]
    document("Regular expression library",mod,"{root}/regex.rst","{root}/detail/regex.rst",groups)

def document_module_regex_boost(root:string)
    var mod = find_module("regex_boost")
    var groups <- [{DocGroup
        group_by_regex("stub0", mod, %regex~(stub0)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("Boost package for REGEX",mod,"{root}/regex_boost.rst","{root}/detail/regex_boost.rst",groups)

def document_module_rst(root:string)
    var mod = find_module("rst")
    var groups <- [{DocGroup
        group_by_regex("Document writers", mod, %regex~document%%);
        group_by_regex("Descriptions", mod, %regex~describe%%);
        group_by_regex("Label makers", mod, %regex~\w+_label_name%%);
        group_by_regex("RST section makers", mod, %regex~make%%);
        group_by_regex("Documentaton queries", mod, %regex~.+_needs%%);
        group_by_regex("Group operations", mod, %regex~.*group%%);
        group_by_regex("Naming helpers", mod, %regex~(topic|module_name|function_name)$%%)
    }]
    document("Documentation generator",mod,"{root}/rst.rst","{root}/detail/rst.rst",groups)

def document_module_apply(root:string)
    var mod = find_module("apply")
    var groups <- [{DocGroup
        group_by_regex("stub0", mod, %regex~(stub0)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("Apply reflection pattern",mod,"{root}/apply.rst","{root}/detail/apply.rst",groups)

def document_module_jobque(root:string)
    var mod = find_module("jobque")
    var groups <- [{DocGroup
        group_by_regex("Channel and JobStatus", mod, %regex~(append|notify|join)$%%);
        group_by_regex("Queries", mod, %regex~(get_total_hw_jobs|get_total_hw_threads|is_job_que_shutting_down)$%%);
        group_by_regex("Internal invocations", mod, %regex~(new_job_invoke|new_thread_invoke)$%%);
        group_by_regex("Construction", mod, %regex~(with_channel|with_job_status|with_job_que)$%%)
    }]
    document("Jobs and threads",mod,"{root}/jobque.rst","{root}/detail/jobque.rst",groups)

def document_module_jobque_boost(root:string)
    var mod = find_module("jobque_boost")
    var groups <- [{DocGroup
        group_by_regex("Invocations", mod, %regex~(new_job|new_thread)$%%);
        group_by_regex("Iteration", mod, %regex~(for_each|each)$%%);
        group_by_regex("Passing data", mod, %regex~(push|push_clone)$%%)
    }]
    document("Boost package for jobs and threads",mod,"{root}/jobque_boost.rst","{root}/detail/jobque_boost.rst",groups)

def document_module_apply_in_context(root:string)
    var mod = find_module("apply_in_context")
    var groups <- [{DocGroup
        group_by_regex("stub0", mod, %regex~(stub0)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("Cross-context evaluation helpers",mod,"{root}/apply_in_context.rst","{root}/detail/apply_in_context.rst",groups)

def document_module_algorithm(root:string)
    var mod = find_module("algorithm")
    var groups <- [{DocGroup
        group_by_regex("Search", mod, %regex~(lower_bound|binary_search)$%%);
        group_by_regex("Array manipulation", mod, %regex~(unique|sort_unique|reverse|combine)$%%)
    }]
    document("Miscelanious algorithms",mod,"{root}/algorithm.rst","{root}/detail/algorithm.rst",groups)

def document_module_contracts(root:string)
    var mod = find_module("contracts")
    var groups <- [{DocGroup
        group_by_regex("Type queries", mod, %regex~(isYetAnotherVectorTemplate)$%%)
    }]
    document("Miscelanious contract annotations",mod,"{root}/contracts.rst","{root}/detail/contracts.rst",groups)

def document_module_defer(root:string)
    var mod = find_module("defer")
    var groups <- [{DocGroup
        group_by_regex("Defer", mod, %regex~(defer)$%%);
        group_by_regex("Stub", mod, %regex~(nada)$%%)
    }]
    document("defer and defer_delete macros",mod,"{root}/defer.rst","{root}/detail/defer.rst",groups)

def document_module_if_not_null(root:string)
    var mod = find_module("if_not_null")
    var groups <- [{DocGroup
        group_by_regex("stub0", mod, %regex~(stub0)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("if_not_null macro",mod,"{root}/if_not_null.rst","{root}/detail/if_not_null.rst",groups)

def document_module_instance_function(root:string)
    var mod = find_module("instance_function")
    var groups <- [{DocGroup
        group_by_regex("stub0", mod, %regex~(stub0)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("instance_function function annotation",mod,"{root}/instance_function.rst","{root}/detail/instance_function.rst",groups)

def document_module_templates(root:string)
    var mod = find_module("templates")
    var groups <- [{DocGroup
        group_by_regex("stub0", mod, %regex~(stub0)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("decltype macro and template function annotation",mod,"{root}/templates.rst","{root}/detail/templates.rst",groups)

def document_module_templates_boost(root:string)
    var mod = find_module("templates_boost")
    var groups <- [{DocGroup
        group_by_regex("Template rules", mod, %regex~(kaboomVarField|replaceVariable|renameVariable|replaceType|replaceAnnotationArgument|replaceBlockArgument)$%%);
        group_by_regex("Template application", mod, %regex~(apply_template)$%%);
        group_by_regex("Block helpers", mod, %regex~(unquote_block|move_unquote_block)$%%);
        group_by_regex("Global variable helpers", mod, %regex~(add_global_var|add_global_let|add_global_private_var|add_global_private_let)$%%);
        group_by_regex("Hygenic names", mod, %regex~(make_unique_private_name)$%%)
    }]
    document("Template application helpers",mod,"{root}/templates_boost.rst","{root}/detail/templates_boost.rst",groups)

def document_module_is_local(root:string)
    var mod = find_module("is_local")
    var groups <- [{DocGroup
        group_by_regex("Scope checks", mod, %regex~(is_local_expr|is_local_or_global_expr|is_scope_expr)$%%)
    }]
    document("is_local_xxx ast helpers",mod,"{root}/is_local.rst","{root}/detail/is_local.rst",groups)

def document_module_safe_addr(root:string)
    var mod = find_module("safe_addr")
    var groups <- [{DocGroup
        group_by_regex("Safe temporary address", mod, %regex~(safe_addr)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("safe_addr macro",mod,"{root}/safe_addr.rst","{root}/detail/safe_addr.rst",groups)

def document_module_static_let(root:string)
    var mod = find_module("static_let")
    var groups <- [{DocGroup
        group_by_regex("Static variable declarations", mod, %regex~(static_let|static_let_finalize)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("static_let macro",mod,"{root}/static_let.rst","{root}/detail/static_let.rst",groups)

def document_module_lpipe(root:string)
    var mod = find_module("lpipe")
    var groups <- [{DocGroup
        group_by_regex("stub0", mod, %regex~(stub0)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("lpipe macro",mod,"{root}/lpipe.rst","{root}/detail/lpipe.rst",groups)

def document_module_math_bits(root:string)
    var mod = find_module("math_bits")
    var groups <- [{DocGroup
        group_by_regex("float in int,uint", mod, %regex~(int_bits_to_float|uint_bits_to_float)$%%);
        group_by_regex("int,uint in float", mod, %regex~(float_bits_to_int|float_bits_to_uint)$%%)
    }]
    document("Math bit helpers",mod,"{root}/math_bits.rst","{root}/detail/math_bits.rst",groups)

def document_module_math_boost(root:string)
    var mod = find_module("math_boost")
    var groups <- [{DocGroup
        group_by_regex("Angle conversions", mod, %regex~(degrees|radians)$%%);
        group_by_regex("Intersections", mod, %regex~(is_intersecting)$%%);
        group_by_regex("Matrices", mod, %regex~(look_at_lh|look_at_rh|perspective_lh|perspective_rh|perspective_rh_opengl|ortho_rh|planar_shadow)$%%);
        group_by_regex("Plane", mod, %regex~(plane_dot|plane_normalize|plane_from_point_normal)$%%);
        group_by_regex("Color packig and unpacking", mod, %regex~(RGBA_TO_UCOLOR|UCOLOR_TO_RGBA|UCOLOR_TO_RGB)$%%)
    }]
    document("Boost package for math",mod,"{root}/math_boost.rst","{root}/detail/math_boost.rst",groups)


[export]
def main
    let root = get_das_root()+"/doc/source/stdlib"  // todo: modify to output /temp so that we can merge changes
    topic_root = "{root}/detail"
    // document embedded modules
    document_module_builtin(root)
    document_module_math(root)
    document_module_fio(root)
    document_module_network(root)
    document_module_uriparser(root)
    document_module_rtti(root)
    document_module_ast(root)
    document_module_strings(root)
    document_module_jobque(root)

    document_module_algorithm(root)
    document_module_apply_in_context(root)
    document_module_apply(root)
    // document_module_archive(root)
    // document_module_array_boost(root)
    // document_module_assert_once(root)
    // document_module_ast_block_to_loop(root)
    document_module_ast_boost(root)
    // document_module_used(root)
    // document_module_constexpr(root)
    document_module_contracts(root)
    // document_module_cpp_bind(root)
    // document_module_cpp_gen(root)
    // document_module_dap(root)                    // todo: ask to document
    // document_module_das_source_formatter(root)   // todo: ask to document
    // document_module_debug(root)                  // todo: document???
    // document_module_decs_boost(root)                 // DECS
    // document_module_decs_state(root)                 // DECS
    // document_module_decs(root)                       // DECS
    document_module_defer(root)
    // document_module_enum_trait(root)
    document_module_functional(root)
    document_module_if_not_null(root)
    document_module_instance_function(root)
    document_module_is_local(root)
    document_module_jobque_boost(root)
    document_module_json_boost(root)
    document_module_json(root)
    // document_module_lint_everything(root)        // todo: document?
    // document_module_lint(root)                   // todo: document?
    // document_module_live(root)
    document_module_lpipe(root)
    document_module_math_bits(root)
    document_module_math_boost(root)
    document_module_random(root)
    document_module_regex_boost(root)
    document_module_regex(root)
    document_module_rst(root)
    document_module_safe_addr(root)
    // document_module_sort_boost(root)
    document_module_static_let(root)
    document_module_strings_boost(root)
    document_module_templates_boost(root)
    document_module_templates(root)
    // document_module_unroll(root)
    // document_module_uriparser_boost(root)
