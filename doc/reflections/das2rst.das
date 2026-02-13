options gen2
options rtti = true

require fio
require rtti
require ast
require math
require uriparser
require strings
require jobque
require daslib/ast_boost
require daslib/rst_comment
require daslib/strings_boost
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
require daslib/array_boost
require daslib/archive
require daslib/unroll
require daslib/assert_once
require daslib/ast_block_to_loop
require daslib/ast_used
require daslib/constant_expression
require daslib/uriparser_boost
require daslib/sort_boost
require daslib/enum_trait
require daslib/cpp_bind
require daslib/decs
require daslib/decs_boost
require daslib/utf8_utils
require daslib/coroutines
require daslib/interfaces
require daslib/export_constructor
require daslib/faker
require daslib/fuzzer
require daslib/match
require daslib/linq
require daslib/linq_boost
require daslib/flat_hash_table
require daslib/cuckoo_hash_table
require daslib/macro_boost
require daslib/class_boost
require daslib/soa
require daslib/base64
// require daslib/linked_list                  // spoof template, not documentable
require daslib/bool_array
require daslib/bitfield_boost
require daslib/bitfield_trait
require daslib/async_boost
require daslib/consume
require daslib/dap
require daslib/das_source_formatter
require daslib/das_source_formatter_fio
// require daslib/debug                          // uses persistent_heap, gc, complex dependencies
require daslib/debug_eval
require daslib/decs_state
require daslib/dynamic_cast_rtti
require daslib/generic_return
// require daslib/heartbeat                     // qmacro(heartbeat()) conflicts when used in das2rst context
require daslib/lint
// require daslib/lint_everything               // global_lint_macro causes lint errors in other modules
// require daslib/live                            // uses multiple_contexts
require daslib/profiler
require daslib/profiler_boost
require daslib/quote
require daslib/refactor
require daslib/remove_call_args
// require daslib/raster_boost                    // depends on external raster module
// require daslib/spoof                           // depends on peg module
require daslib/stringify
require daslib/temp_strings
require daslib/type_traits
require daslib/typemacro_boost
require daslib/validate_code

def document_module_math(root : string) {
    var mod = get_module("math")
    var groups <- array<DocGroup>(
        group_by_regex("all numerics (uint*, int*, float*, double)", mod, %regex~(min|max)$%%),
        group_by_regex("float* and double", mod, %regex~(sign|abs|acos|asin|safe_acos|safe_asin|atan|atan2|cos|sin|tan|exp|log|exp2|
log2|pow|sqrt|rcp|ceil|floor|abs|saturate|sincos|is_finite|is_nan)$%%),
        group_by_regex("float* only", mod, %regex~(atan_est|atan2_est|rcp_est|ceili|floori|roundi|trunci|round|rsqrt|rsqrt_est|fract|\-)$%%),
        group_by_regex("float3 only", mod, %regex~(cross|reflect|refract|distance|distance_sq|
inv_distance|inv_distance_sq)$%%),
        group_by_regex("float2, float3, float4", mod, %regex~(length|length_sq|inv_length|inv_length_sq|
dot|normalize|fast_normalize)$%%),
        group_by_regex("Noise functions", mod, %regex~(uint32_hash|uint_noise_1D|uint_noise_2D|uint_noise_3D)$%%),
        group_by_regex("lerp/mad/clamp", mod, %regex~(lerp|mad|clamp)$%%),
        group_by_regex("Matrix element access", mod, %regex~\.\[\]$%%),
        group_by_regex("Matrix operations", mod, %regex~(\=\=|\!\=|\*).*$%%),
        group_by_regex("Matrix initializers", mod, %regex~(float3x4|float4x4|float3x3|identity3x4|identity4x4|identity3x3)$%%),
        group_by_regex("Matrix manipulation", mod, %regex~(identity|inverse|rotate|transpose|translation|compose|decompose|look_at|orthonormal_inverse|persp_forward|persp_reverse|determinant)$%%),
        group_by_regex("Quaternion operations", mod, %regex~(quat_conjugate|quat_mul|quat_mul_vec|quat_from_unit_arc|quat_from_unit_vec_ang|quat_from_euler|euler_from_quat|quat$|quat_slerp)%%),
        group_by_regex("Packing and unpacking", mod, %regex~(pack_float_to_byte|unpack_byte_to_float)$%%)
    );
    document("Math library", mod, "math.rst", groups);
}

def document_module_builtin(root : string) {
    var mod = get_module("$")
    var groups <- array<DocGroup>(
        hide_group(group_by_regex("Internal builtin functions", mod, %regex~builtin%%)),
        hide_group(group_by_regex("Internal pointer arithmetics", mod, %regex~i_das_%%)),
        hide_group(group_by_regex("Internal clone infrastructure", mod, %regex~clone%%)),
        hide_group(group_by_regex("Internal finalize infrastructure", mod, %regex~finalize%%)),
        group_by_regex("Containers", mod, %regex~(capacity|clear|length|resize|resize_no_init|reserve|each|emplace|erase|find|
find_for_edit|find_if_exists|find_index|find_index_if|has_value|key_exists|keys|values|lock|each_enum|each_ref|
find_for_edit_if_exists|lock_forever|next|nothing|pop|push|push_clone|back|sort|to_array|to_table|to_array_move|
to_table_move|empty|subarray|insert|move_to_ref|copy_to_local|move_to_local|get|remove_value|erase_if|resize_and_init|
get_value|insert_clone|emplace_new|insert_default|emplace_default|get_with_default|modify)$%%),
        group_by_regex("Character set groups", mod, %regex~(is_alpha|is_number|is_white_space|is_char_in_set)$%%),
        group_by_regex("das::string manipulation", mod, %regex~(peek|set)$%%),
        group_by_regex("String builder", mod, %regex~(build_string|write|write_char|write_chars|write_escape_string)$%%),
        group_by_regex("Heap reporting", mod, %regex~(heap|string_heap|memory_report)%%),
        group_by_regex("Vector and matrix math", mod, %regex~(float3x4|float4x4|identity|inverse|rotate|translation|transpose)$%%),
        group_by_regex("GC0 infrastructure", mod, %regex~gc0%%),
        group_by_regex("Smart ptr infrastructure", mod, %regex~(add_ptr_ref|smart_ptr|get_const_ptr|move|move_new|get_ptr$)%%),
        group_by_regex("Macro infrastructure", mod, %regex~(is_folding|is_compiling|is_in_completion|is_reporting_compilation_errors)%%),
        group_by_regex("Profiler", mod, %regex~(profile|reset_profiler|dump_profile_info|collect_profile_info)$%%),
        group_by_regex("System infrastructure", mod, %regex~(panic|print|sprint|sprint_json|to_log|to_compiler_log|error|terminate|breakpoint|stackwalk|get_das_root|is_in_aot|aot_enabled|is_intern_strings|eval_main_loop)$%%),
        group_by_regex("Memory manipulation", mod, %regex~(intptr|memcmp|variant_index|set_variant_index|hash|memcpy|lock_data|map_to_array|map_to_ro_array)$%%),
        group_by_regex("Binary serializer", mod, %regex~(binary_load|binary_save)$%%),
        group_by_regex("Path and command line", mod, %regex~(get_command_line_arguments)$%%),
        group_by_regex("Time and date", mod, %regex~(get_time_usec|ref_time_ticks|get_clock|get_time_nsec|mktime)$%%),
        group_by_regex("Platform queries", mod, %regex~(get_context_share_counter|das_is_dll_build|get_platform_name|get_architecture_name)$%%),
        group_by_regex("String formatting", mod, %regex~(fmt)$%%),
        group_by_regex("Argument consumption", mod, %regex~(consume_argument)$%%),
        group_by_regex("Lock checking", mod, %regex~(lock_count|set_verify_array_locks|set_verify_table_locks)$%%),
        group_by_regex("Lock checking internals", mod, %regex~(_move_with_lockcheck|_return_with_lockcheck|_at_with_lockcheck)$%%),
        group_by_regex("Bit operations", mod, %regex~(popcnt|clz|ctz|mul128|__bit_set)$%%),
        group_by_regex("Intervals", mod, %regex~(interval)$%%),
        group_by_regex("RTTI", mod, %regex~(class_rtti_size)$%%),
        hide_group(group_by_regex("Jit", mod,%regex~(invoke_code|.*jit.*)$%%)),
        group_by_regex("Lock verification", mod, %regex~(set_verify_context_locks)$%%),
        group_by_regex("Initialization and finalization", mod, %regex~(using|clone|finalize)$%%),
        group_by_regex("Algorithms", mod, %regex~(swap|iter_range|count|ucount)$%%),
        group_by_regex("Memset", mod, %regex~(memset.*)$%%),
        group_by_regex("Malloc", mod, %regex~(malloc|free|malloc_usable_size)$%%),
        group_by_regex("Compilation and AOT", mod, %regex~(is_aot_enabled|set_aot|reset_aot|compiling_file_name|compiling_module_name)$%%))
    document("Built-in runtime", mod, "builtin.rst", groups);
}

def document_module_fio(root : string) {
    var mod = get_module("fio")
    var groups <- array<DocGroup>(
        hide_group(group_by_regex("Internal builtin functions", mod, %regex~builtin%%)),
        group_by_regex("File manipulation", mod, %regex~(f|stat$|getchar|remove|rename)%%),
        group_by_regex("Path manipulation", mod, %regex~(base_name|dir_name|get_full_file_name)$%%),
        group_by_regex("Directory manipulation", mod, %regex~(dir|mkdir|mkdir_rec|chdir|getcwd)$%%),
        group_by_regex("OS specific routines", mod, %regex~(sleep|exit|popen|popen_binary|get_env_variable|sanitize_command_line|has_env_variable)$%%),
        group_by_regex("Dynamic modules", mod, %regex~(register_dynamic_module|register_native_path)$%%)
    )
    document("File input output library", mod, "fio.rst", groups)
}

def document_module_random(root : string) {
    var mod = get_module("random")
    var groups <- array<DocGroup>(
        group_by_regex("Seed and basic generators", mod, %regex~(random_seed|random_seed2D|random_float|random_float4|random_int|random_big_int|random_int4|random_uint)$%%),
        group_by_regex("Random iterators", mod, %regex~(each_random_.*)$%%),
        group_by_regex("Specific distributions", mod, %regex~(random_unit_vector|random_in_unit_disk|random_in_unit_sphere)$%%)
    )
    document("Random generator library", mod, "random.rst", groups)
}

def document_module_network(root : string) {
    var mod = get_module("network")
    var groups <- array<DocGroup>(
        group_by_regex("Low level NetworkServer IO", mod, %regex~(make_server|server_init|server_is_open|server_is_connected|server_tick|server_send|server_restore)$%%)
    )
    document("Network socket library", mod, "network.rst", groups)
}

def document_module_uriparser(root : string) {
    var mod = get_module("uriparser")
    var groups <- array<DocGroup>(
        group_by_regex("Initialization and finalization", mod, %regex~(Uri|using|clone|finalize)$%%),
        group_by_regex("Escape and unescape", mod, %regex~(escape_uri|unescape_uri)$%%),
        group_by_regex("Uri manipulations", mod, %regex~(normalize_uri|unescape_uri|string|uri_for_each_query_kv|remove_base_uri|add_base_uri|strip_uri|normalize)$%%),
        group_by_regex("File name conversions", mod, %regex~(to_unix_file_name|to_windows_file_name|to_file_name|uri_from_file_name|uri_from_windows_file_name|uri_from_unix_file_name|uri_to_unix_file_name|uri_to_windows_file_name|unix_file_name_to_uri|windows_file_name_to_uri|uri_to_file_name|file_name_to_uri)$%%),
        group_by_regex("GUID", mod, %regex~(make_new_guid)$%%)
    )
    document("URI manipulation library based on UriParser", mod, "uriparser.rst", groups)
}

def document_module_rtti(root : string) {
    var mod = get_module("rtti")
    var groups <- array<DocGroup>(
        group_by_regex("Initialization and finalization", mod, %regex~(using|LineInfo|CodeOfPolicies|RttiValue_nothing)$%%),
        group_by_regex("Type access", mod, %regex~(get_dim|builtin_is_same_type|is_compatible_cast|get_das_type_name|is_same_type|each_dim|arg_types|arg_names|get_type_size|get_type_align)$%%),
        group_by_regex("Rtti context access", mod, %regex~(get_total_functions|get_total_variables|get_function_info|get_variable_info|get_variable_value|get_function_by_mnh|get_line_info|this_context|context_for_each_function|context_for_each_variable|class_info|type_info)$%%),
        group_by_regex("Program access", mod, %regex~(program_for_each_module|program_for_each_registered_module|get_this_module|get_module)$%%),
        group_by_regex("Module access", mod, %regex~(module_for_each_structure|module_for_each_enumeration|module_for_each_function|module_for_each_generic|module_for_each_global|module_for_each_annotation|module_for_each_dependency)$%%),
        group_by_regex("Annotation access", mod, %regex~(get_annotation_argument_value|add_annotation_argument)$%%),
        group_by_regex("Compilation and simulation", mod, %regex~(compile|compile_file|for_each_expected_error|for_each_require_declaration|simulate)$%%),
        group_by_regex("File access", mod, %regex~(make_file_access|set_file_source|add_file_access_root)$%%),
        group_by_regex("Structure access", mod, %regex~(rtti_builtin_structure_for_each_annotation|basic_struct_for_each_field|structure_for_each_annotation|basic_struct_for_each_parent)$%%),
        group_by_regex("Data walking and printing", mod, %regex~(sprint_data|describe|get_mangled_name)$%%),
        group_by_regex("Function and mangled name hash", mod, %regex~(get_function_by_mangled_name_hash|get_function_mangled_name_hash|get_function_address)$%%),
        group_by_regex("Context and mutex locking", mod, %regex~(lock_this_context|lock_context|lock_mutex)$%%),
        group_by_regex("Runtime data access", mod, %regex~(get_table_key_index)$%%),
        group_by_regex("Tuple and variant access", mod, %regex~(get_tuple_field_offset|get_variant_field_offset)$%%),
        group_by_regex("Iteration", mod, %regex~(each)$%%))
    document("Runtime type information library", mod, "rtti.rst", groups)
}

def document_module_ast(root : string) {
    var mod = get_module("ast")
    var groups <- array<DocGroup>(
        group_by_regex("Call generation", mod, %regex~(make_call)$%%),
        group_by_regex("Visitor pattern", mod, %regex~visit.*%%),
        group_by_regex("Expression generation", mod, %regex~(force_generated|get_expression_annotation|make_type_info_structure)%%),
        group_by_regex("Adapter generation", mod, %regex~make_.*%%),
        group_by_regex("Adapter application", mod, %regex~(add_.*_(annotation|macro)|add_new_comment_reader|add_comment_reader|add_module_option)%%),
        group_by_regex("Adding objects to objects", mod, %regex~add_.*%%),
        group_by_regex("Program and module access", mod, %regex~(this_program|this_module|compiling_program|compiling_module)$%%),
        group_by_regex("Textual descriptions of the objects", mod, %regex~(describe.*|das_to_string)%%),
        group_by_regex("Searching", mod, %regex~find.*%%),
        group_by_regex("Iterating", mod, %regex~(for_each.*|.*_foreach)%%),
        group_by_regex("Cloning", mod, %regex~clone.*%%),
        group_by_regex("Mangled name", mod, %regex~(parse_mangled_name|get_mangled_name)%%),
        group_by_regex("Size and offset", mod, %regex~.*_(size|offset)%%),
        group_by_regex("Pointer conversion", mod, %regex~(ExpressionPtr|FunctionPtr|StructurePtr)%%),
        group_by_regex("Evaluations", mod, %regex~(eval_single_expression)%%),
        group_by_regex("Error reporting", mod, %regex~(macro_error)%%),
        group_by_regex("Location and context", mod, %regex~(force_at|collect_dependencies|get_ast_context)%%),
        // hide_group(group_by_regex("Jit support",mod,%regex~(make_interop_node|get_builtin_function_address)%%));
        group_by_regex("Use queries", mod, %regex~(get_use_global_variables|get_use_functions)%%),
        group_by_regex("Log", mod, %regex~(to_compilation_log)%%),
        group_by_regex("Removal", mod, %regex~(remove.*)%%),
        group_by_regex("Properties", mod, %regex~(is|has|get|can)_.*%%),
        group_by_regex("Infer", mod, %regex~(infer_generic_type|update_alias_map)$%%),
        group_by_regex("Module queries", mod, %regex~(module_find_annotation|module_find_type_annotation|module_find_structure|not_inferred)$%%),
        group_by_regex("Debug info helpers", mod, %regex~(debug_helper_.*)%%),
        group_by_regex("AOT support", mod, %regex~(aot_require|aot_type_ann_get_field_ptr|aot_need_type_info|aot_previsit_get_field_ptr|aot_previsit_get_field|aot_visit_get_field|write_aot_body|write_aot_suffix|write_aot_macro_suffix|write_aot_macro_prefix|macro_aot_infix|getInitSemanticHashWithDep)$%%),
        group_by_regex("String builder writer", mod, %regex~(string_builder_str|string_builder_clear)$%%),
        hide_group(group_by_regex("Internal ast infrastructure", mod, %regex~(builtin_ast_.*)%%))
    )
    document("AST manipulation library", mod, "ast.rst", groups)
}

def document_module_ast_boost(root : string) {
    var mod = find_module("ast_boost")
    var groups <- array<DocGroup>(
        group_by_regex("Containers", mod, %regex~(emplace_new)%%),
        group_by_regex("Textual descriptions of the objects", mod, %regex~(describe.*|debug_expression)%%),
        group_by_regex("Queries", mod, %regex~(is.*|find.*|get.*)%%),
        group_by_regex("Annotations", mod, %regex~(append.*|add.*)%%),
        group_by_regex("Expression generation", mod, %regex~(override_method|panic_expr_as|make_static_assert_false|convert_to_expression)%%),
        group_by_regex("Visitors", mod, %regex~(visit.*)%%),
        group_by_regex("Type generation", mod, %regex~(function_to_type)%%),
        group_by_regex("Type casts", mod, %regex~(`is`|`as`|walk_and_convert)%%),
        hide_group(group_by_regex("Clonning", mod, %regex~(clone)%%)),
        group_by_regex("Setup", mod, %regex~setup.*$%%)
    )
    document("Boost package for the AST", mod, "ast_boost.rst", groups)
}

def document_module_strings(root : string) {
    var mod = find_module("strings")
    var groups <- array<DocGroup>(
        hide_group(group_by_regex("Internal builtin functions", mod, %regex~builtin%%)),
        group_by_regex("Character set", mod, %regex~(set_total|set_element|is_char_in_set)$%%),
        group_by_regex("Character groups", mod, %regex~is.+%%),
        group_by_regex("Character by index", mod, %regex~(character_at|character_uat)$%%),
        group_by_regex("String properties", mod, %regex~(empty|ends_with|length|starts_with)$%%),
        group_by_regex("String builder", mod, %regex~(build_string|format|write|write_char|write_chars|write_escape_string|build_hash)$%%),
        group_by_regex("das::string manipulation", mod, %regex~(append|resize)$%%),
        group_by_regex("String modifications", mod, %regex~(chop|escape|unescape|repeat|replace|reverse|slice|
strip|strip_left|strip_right|to_lower|to_lower_in_place|to_upper|to_upper_in_place|rtrim|safe_unescape|ltrim|trim)$%%),
        group_by_regex("Search substrings", mod, %regex~find.*%%),
        group_by_regex("String conversion routines", mod, %regex~(double|float|int|uint|int64|uint64|to_double|to_float|to_int|to_uint|to_int64|to_uint64|string|to_char|fmt|int8|to_int8|uint8|to_uint8|int16|to_int16|uint16|to_uint16|to_cpp_float)$%%),
        group_by_regex("String as array", mod, %regex~(modify_data|peek_data)$%%),
        group_by_regex("Low level memory allocation", mod, %regex~(delete_string|reserve_string_buffer)$%%)
    );
    document("String manipulation library", mod, "strings.rst", groups);
}

def document_module_strings_boost(root : string) {
    var mod = find_module("strings_boost")
    var groups <- array<DocGroup>(
        group_by_regex("Split and join", mod, %regex~(split|split_by_chars|join)$%%),
        group_by_regex("Formatting", mod, %regex~(wide)$%%),
        group_by_regex("Queries and comparisons", mod, %regex~(is_character_at|eq)$%%),
        group_by_regex("Replace", mod, %regex~(replace_multiple)$%%),
        group_by_regex("Levenshtein distance", mod, %regex~(levenshtein_distance|levenshtein_distance_fast)$%%),
        group_by_regex("Character traits", mod, %regex~(is_hex|is_tab_or_space)$%%))
    document("Boost package for string manipulation library", mod, "strings_boost.rst", groups)
}


def document_module_functional(root : string) {
    var mod = find_module("functional")
    var groups <- array<DocGroup>(
        group_by_regex("Map, reduce", mod, %regex~(filter|map|reduce|any|sum|all|cycle|islice|count|repeat_ref|repeat|not|echo|flatten|sorted)$%%),
        group_by_regex("Queries", mod, %regex~(is_equal|is_not_equal)$%%)
    )
    document("Functional programming library", mod, "functional.rst", groups)
}

def document_module_json(root : string) {
    var mod = find_module("json")
    var groups <- array<DocGroup>(
        group_by_regex("Value conversion", mod, %regex~(JV|JVNull)$%%),
        group_by_regex("Read and write", mod, %regex~(read_json|write_json)$%%),
        group_by_regex("JSON properties", mod, %regex~(set_no_trailing_zeros|set_no_empty_arrays|set_allow_duplicate_keys)$%%),
        group_by_regex("Broken JSON", mod, %regex~(try_fixing_broken_json)$%%)
    )
    document("JSON manipulation library", mod, "json.rst", groups)
}

def document_module_json_boost(root : string) {
    var mod = find_module("json_boost")
    var groups <- array<DocGroup>(
        group_by_regex("Value conversion", mod, %regex~(JV|from_JV)$%%),
        group_by_regex("Element access operators", mod, %regex~(\[\]|\?\[\]|\?\.|\.)$%%),
        group_by_regex("Null coalescing operators", mod, %regex~(\?\?)$%%),
        group_by_regex("Value extraction", mod, %regex~(\?\.)?`value$%%),
        group_by_regex("Annotation parsing", mod, %regex~(parse_json_annotation)$%%)
    )
    document("Boost package for JSON", mod, "json_boost.rst", groups)
}

def document_module_regex(root : string) {
    var mod = find_module("regex")
    var groups <- array<DocGroup>(
        group_by_regex("Compilation and validation", mod, %regex~(regex_compile|is_valid|regex_debug|debug_set|visit_top_down)$%%),
        group_by_regex("Access", mod, %regex~(regex_group|regex_foreach)$%%),
        group_by_regex("Match & replace", mod, %regex~(regex_match|regex_replace)$%%),
        group_by_regex("Generation", mod, %regex~(re_gen.*)$%%)
    )
    document("Regular expression library", mod, "regex.rst", groups)
}

def document_module_regex_boost(root : string) {
    var mod = find_module("regex_boost")
    var groups <- array<DocGroup>(
    )
    document("Boost package for REGEX", mod, "regex_boost.rst", groups)
}

def document_module_rst(root : string) {
    var mod = find_module("rst")
    var groups <- array<DocGroup>(
        group_by_regex("Document writers", mod, %regex~document%%),
        group_by_regex("Descriptions", mod, %regex~describe%%),
        group_by_regex("Label makers", mod, %regex~(\w+_label_name|function_label_file)%%),
        group_by_regex("RST section makers", mod, %regex~make%%),
        group_by_regex("Documentation queries", mod, %regex~.+_needs%%),
        group_by_regex("Group operations", mod, %regex~.*group%%),
        group_by_regex("Naming helpers", mod, %regex~(topic|module_name|function_name|safe_function_name)$%%)
    )
    document("Documentation generator", mod, "rst.rst", groups)
}

def document_module_apply(root : string) {
    var mod = find_module("apply")
    var groups <- array<DocGroup>(
    )
    document("Apply reflection pattern", mod, "apply.rst", groups)
}

def document_module_jobque(root : string) {
    var mod = find_module("jobque")
    var groups <- array<DocGroup>(
        group_by_regex("Channel, JobStatus, Lockbox", mod, %regex~(append|notify|join|channel_create|channel_remove|add_ref|release|notify_and_release|lock_box_create|lock_box_remove|job_status_create|job_status_remove)$%%),
        group_by_regex("Queries", mod, %regex~(get_total_hw_jobs|get_total_hw_threads|is_job_que_shutting_down)$%%),
        group_by_regex("Internal invocations", mod, %regex~(new_job_invoke|new_thread_invoke|new_debugger_thread)$%%),
        group_by_regex("Construction", mod, %regex~(with_channel|with_job_status|with_job_que|with_lock_box)$%%),
        group_by_regex("Atomic", mod, %regex~(atomic32_create|atomic32_remove|with_atomic32|atomic64_create|atomic64_remove|with_atomic64|get|set|inc|dec)$%%)
    )
    document("Jobs and threads", mod, "jobque.rst", groups)
}

def document_module_jobque_boost(root : string) {
    var mod = find_module("jobque_boost")
    var groups <- array<DocGroup>(
        group_by_regex("Invocations", mod, %regex~(new_job|new_thread)$%%),
        group_by_regex("Iteration", mod, %regex~(for_each|each|each_clone|for_each_clone)$%%),
        group_by_regex("Passing data", mod, %regex~(push|push_clone|push_batch|push_batch_clone)$%%),
        group_by_regex("Receiving data", mod, %regex~(gather|gather_ex|gather_and_forward|peek|pop_one|pop_and_clone_one)$%%),
        group_by_regex("LockBox operations", mod, %regex~(set|get|update|clear)$%%),
        group_by_regex("Internal capture details", mod, %regex~(capture_jobque_channel|capture_jobque_job_status|release_capture_jobque_channel|release_capture_jobque_job_status|capture_jobque_lock_box|release_capture_jobque_lock_box)$%%)
    )
    document("Boost package for jobs and threads", mod, "jobque_boost.rst", groups)
}

def document_module_apply_in_context(root : string) {
    var mod = find_module("apply_in_context")
    var groups <- array<DocGroup>(
    )
    document("Cross-context evaluation helpers", mod, "apply_in_context.rst", groups)
}

def document_module_algorithm(root : string) {
    var mod = find_module("algorithm")
    var groups <- array<DocGroup>(
        group_by_regex("Search", mod, %regex~(lower_bound|binary_search)$%%),
        group_by_regex("Array manipulation", mod, %regex~(unique|sort_unique|reverse|combine|erase_all|topological_sort)$%%),
        group_by_regex("Table manipulation", mod, %regex~(intersection|union|difference|identical)$%%)
    )
    document("Miscellaneous algorithms", mod, "algorithm.rst", groups)
}

def document_module_contracts(root : string) {
    var mod = find_module("contracts")
    var groups <- array<DocGroup>(
        group_by_regex("Type queries", mod, %regex~(isYetAnotherVectorTemplate)$%%)
    )
    document("Miscellaneous contract annotations", mod, "contracts.rst", groups)
}

def document_module_defer(root : string) {
    var mod = find_module("defer")
    var groups <- array<DocGroup>(
        group_by_regex("Defer", mod, %regex~(defer)$%%),
        group_by_regex("Stub", mod, %regex~(nada)$%%)
    )
    document("defer and defer_delete macros", mod, "defer.rst", groups)
}

def document_module_if_not_null(root : string) {
    var mod = find_module("if_not_null")
    var groups <- array<DocGroup>(
    )
    document("if_not_null macro", mod, "if_not_null.rst", groups)
}

def document_module_instance_function(root : string) {
    var mod = find_module("instance_function")
    var groups <- array<DocGroup>(
    )
    document("instance_function function annotation", mod, "instance_function.rst", groups)
}

def document_module_templates(root : string) {
    var mod = find_module("templates")
    var groups <- array<DocGroup>(
    )
    document("decltype macro and template function annotation", mod, "templates.rst", groups)
}

def document_module_templates_boost(root : string) {
    var mod = find_module("templates_boost")
    var groups <- array<DocGroup>(
        group_by_regex("Template rules", mod, %regex~(kaboomVarField|replaceVariable|renameVariable|replaceType|replaceAnnotationArgument|replaceBlockArgument|replaceVarTag|replaceArgumentWithList|replaceVariableWithList|renameField|replaceStructWithTypeDecl|replaceTypeWithTypeDecl|renameCall)$%%),
        group_by_regex("Template application", mod, %regex~(apply_template)$%%),
        group_by_regex("Expression helpers", mod, %regex~(remove_deref|visit_expression|expression_at)$%%),
        group_by_regex("Expression generation", mod, %regex~(make_expression_block)$%%),
        group_by_regex("Block helpers", mod, %regex~(unquote_block|move_unquote_block)$%%),
        group_by_regex("Global variable helpers", mod, %regex~(add_global_var|add_global_let|add_global_private_var|add_global_private_let)$%%),
        group_by_regex("Hygienic names", mod, %regex~(make_unique_private_name)$%%),
        group_by_regex("Quoting macros", mod, %regex~(apply_qmacro|apply_qblock|apply_qblock_to_array|apply_qblock_expr|apply_qtype|apply_qmacro_function|apply_qmacro_method|apply_qmacro_variable|apply_qmacro_template_class|apply_qmacro_template_function)$%%),
        group_by_regex("Type pointer helpers", mod, %regex~(add_type_ptr_ref|add_array_ptr_ref)$%%),
        group_by_regex("Structure helpers", mod, %regex~(add_structure_field)$%%),
        group_by_regex("Class generation", mod, %regex~(make_class|make_class_constructor|modify_to_class_member|enum_class_type)$%%)
    )
    document("Template application helpers", mod, "templates_boost.rst", groups)
}

def document_module_is_local(root : string) {
    var mod = find_module("is_local")
    var groups <- array<DocGroup>(
        group_by_regex("Scope checks", mod, %regex~(is_local_expr|is_local_or_global_expr|is_scope_expr|is_temp_safe|is_shared_expr)$%%)
    )
    document("is_local_xxx ast helpers", mod, "is_local.rst", groups)
}

def document_module_safe_addr(root : string) {
    var mod = find_module("safe_addr")
    var groups <- array<DocGroup>(
        group_by_regex("Safe temporary address", mod, %regex~(safe_addr|shared_addr)$%%),
        group_by_regex("Temporary pointers", mod, %regex~(temp_ptr)$%%),
        group_by_regex("Temporary values", mod, %regex~(temp_value)$%%)
    )
    document("safe_addr macro", mod, "safe_addr.rst", groups)
}

def document_module_static_let(root : string) {
    var mod = find_module("static_let")
    var groups <- array<DocGroup>(
        group_by_regex("Static variable declarations", mod, %regex~(static_let|static_let_finalize)$%%)
    )
    document("static_let macro", mod, "static_let.rst", groups)
}

def document_module_lpipe(root : string) {
    var mod = find_module("lpipe")
    var groups <- array<DocGroup>(
    )
    document("lpipe macro", mod, "lpipe.rst", groups)
}

def document_module_math_bits(root : string) {
    var mod = find_module("math_bits")
    var groups <- array<DocGroup>(
        group_by_regex("float in int,uint", mod, %regex~(int_bits_to_float|uint_bits_to_float)$%%),
        group_by_regex("int,uint in float", mod, %regex~(float_bits_to_int|float_bits_to_uint)$%%),
        group_by_regex("int64,uint64 in double", mod, %regex~(int64_bits_to_double|uint64_bits_to_double|double_bits_to_int64|double_bits_to_uint64)$%%),
        group_by_regex("bit-cast vec4f", mod, %regex~(cast_to.*)$%%))
    document("Math bit helpers", mod, "math_bits.rst", groups)
}

def document_module_math_boost(root : string) {
    var mod = find_module("math_boost")
    var groups <- array<DocGroup>(
        group_by_regex("Angle conversions", mod, %regex~(degrees|radians)$%%),
        group_by_regex("Intersections", mod, %regex~(is_intersecting)$%%),
        group_by_regex("Matrices", mod, %regex~(look_at_lh|look_at_rh|perspective_lh|perspective_rh|perspective_rh_opengl|ortho_rh|planar_shadow)$%%),
        group_by_regex("Plane", mod, %regex~(plane_dot|plane_normalize|plane_from_point_normal)$%%),
        group_by_regex("Color conversions", mod, %regex~(linear_to_SRGB)$%%),
        group_by_regex("Color packig and unpacking", mod, %regex~(RGBA_TO_UCOLOR|UCOLOR_TO_RGBA|UCOLOR_TO_RGB)$%%)
    )
    document("Boost package for math", mod, "math_boost.rst", groups)
}

def document_module_array_boost(root : string) {
    var mod = find_module("array_boost")
    var groups <- array<DocGroup>(
        group_by_regex("Temporary arrays", mod, %regex~(temp_array)$%%),
        group_by_regex("Empty check", mod, %regex~(empty)$%%),
        group_by_regex("Sub-array view", mod, %regex~(array_view)$%%)
    )
    document("Boost package for array manipulation", mod, "array_boost.rst", groups)
}

def document_module_archive(root : string) {
    var mod = find_module("archive")
    var groups <- array<DocGroup>(
        group_by_regex("Serialization", mod, %regex~(serialize|serialize_raw|read_raw|write_raw)$%%),
        group_by_regex("Memory archive", mod, %regex~(mem_archive_save|mem_archive_load)$%%)
    )
    document("General purpose serialization", mod, "archive.rst", groups)
}

def document_module_unroll(root : string) {
    var mod = find_module("unroll")
    var groups <- array<DocGroup>(
        group_by_regex("Unrolling", mod, %regex~(unroll)$%%)
    )
    document("Loop unrolling", mod, "unroll.rst", groups)
}

def document_module_assert_once(root : string) {
    var mod = find_module("assert_once")
    var groups <- array<DocGroup>(
        group_by_regex("Assertion", mod, %regex~(assert_once)$%%)
    )
    document("Assert once", mod, "assert_once.rst", groups)
}

def document_module_ast_block_to_loop(root : string) {
    var mod = find_module("ast_block_to_loop")
    var groups <- array<DocGroup>(
        group_by_regex("Block to loop conversion", mod, %regex~(convert_block_to_loop)$%%)
    )
    document("DECS, AST block to loop", mod, "ast_block_to_loop.rst", groups)
}

def document_module_ast_used(root : string) {
    var mod = find_module("ast_used")
    var groups <- array<DocGroup>(
        group_by_regex("Collecting type information", mod, %regex~(collect_used_types)$%%)
    )
    document("AST type ussage collection", mod, "ast_used.rst", groups)
}

def document_module_constexpr(root : string) {
    var mod = find_module("constant_expression")
    var groups <- array<DocGroup>(
        group_by_regex("Macro helpers", mod, %regex~(isConstantExpression)$%%)
    )
    document("Constant expression checker and substitution", mod, "constexpr.rst", groups)
}

def document_module_uriparser_boost(root : string) {
    var mod = find_module("uriparser_boost")
    var groups <- array<DocGroup>(
        group_by_regex("Split and compose", mod, %regex~(uri_split_full_path|uri_compose_query|uri_compose_query_in_order|uri_compose)$%%),
        group_by_regex("Component accessors", mod, %regex~(scheme|user_info|host|port|path|query|fragment)$%%)
    )
    document("Boost package for the URI parser", mod, "uriparser_boost.rst", groups)
}

def document_module_sort_boost(root : string) {
    var mod = find_module("sort_boost")
    var groups <- array<DocGroup>(
    )
    document("Boost package for the builtin sort", mod, "sort_boost.rst", groups)
}

def document_module_enum_trait(root : string) {
    var mod = find_module("enum_trait")
    var groups <- array<DocGroup>(
        group_by_regex("Enumeration iteration", mod, %regex~(each|each_name|find_value)$%%),
        group_by_regex("Enumeration conversion", mod, %regex~(string|to_enum|enum_to_table)$%%)
    )
    document("Enumeration traits", mod, "enum_trait.rst", groups)
}

def document_module_cpp_bind(root : string) {
    var mod = find_module("cpp_bind")
    var groups <- array<DocGroup>(
        group_by_regex("Generation of bindings", mod, %regex~(log_cpp_class_adapter)$%%)
    )
    document("C++ bindings generator", mod, "cpp_bind.rst", groups)
}

def document_module_decs(root : string) {
    var mod = find_module("decs")
    var groups <- array<DocGroup>(
        group_by_regex("Comparison and access", mod, %regex~(\=\=|\!\=|\.)$%%),
        group_by_regex("Access (get/set/clone)", mod, %regex~(has|get|set|clone|remove)$%%),
        group_by_regex("Debug and serialization", mod, %regex~(describe|serialize|finalize|debug_dump)$%%),
        group_by_regex("Stages", mod, %regex~(register_decs_stage_call|decs_stage|commit)$%%),
        group_by_regex("Deferred actions", mod, %regex~(update_entity|create_entity|delete_entity)$%%),
        group_by_regex("GC and reset", mod, %regex~(before_gc|after_gc|restart)$%%),
        group_by_regex("Iteration", mod, %regex~(for_each_archetype|for_eid_archetype|for_each_archetype_find|get_ro|decs_array|get_default_ro|get_optional)$%%),
        group_by_regex("Request", mod, %regex~(verify_request|compile_request|lookup_request|EcsRequestPos)$%%)
    )
    document("DECS, Daslang entity component system", mod, "decs.rst", groups)
}

def document_module_decs_boost(root : string) {
    var mod = find_module("decs_boost")
    var groups <- array<DocGroup>(
    )
    document("Boost package for DECS", mod, "decs_boost.rst", groups)
}

def document_module_utf8_utils(root : string) {
    var mod = find_module("utf8_utils")
    var groups <- array<DocGroup>(
        group_by_regex("Encoding and decoding", mod, %regex~(utf8_encode|utf8_decode|utf16_to_utf32|utf32_to_utf16|decode_unicode_escape)$%%),
        group_by_regex("Iteration", mod, %regex~(each_utf32)$%%),
        group_by_regex("Length and measurement", mod, %regex~(utf8_length)$%%),
        group_by_regex("Validation", mod, %regex~(is_first_byte_of_utf8_char|contains_utf8_bom|is_utf8_string_valid)$%%)
    )
    document("UTF-8 utilities", mod, "utf8_utils.rst", groups)
}

def document_module_macro_boost(root : string) {
    var mod = find_module("macro_boost")
    var groups <- array<DocGroup>(
        group_by_regex("Implementation details", mod, %regex~(macro_verify)$%%),
        group_by_regex("Block analysis", mod, %regex~(capture_block|collect_finally|collect_labels)$%%)
    )
    document("Boost package for macro manipulations", mod, "macro_boost.rst", groups)
}

def document_module_coroutines(root : string) {
    var mod = find_module("coroutines")
    var groups <- array<DocGroup>(
        group_by_regex("Top level coroutine evaluation", mod, %regex~(cr_run|cr_run_all)$%%)
    )
    document("Coroutines and additional generator support", mod, "coroutines.rst", groups)
}

def document_module_interfaces(root : string) {
    var mod = find_module("interfaces")
    var groups <- array<DocGroup>(
    )
    document("Interfaces", mod, "interfaces.rst", groups)
}

def document_module_export_constructor(root : string) {
    var mod = find_module("export_constructor")
    var groups <- array<DocGroup>(
    )
    document("Export constructor", mod, "export_constructor.rst", groups)
}

def document_module_faker(root : string) {
    var mod = find_module("faker")
    var groups <- array<DocGroup>(
        group_by_regex("Constructor", mod, %regex~(Faker)$%%),
        group_by_regex("Random values", mod, %regex~random_.*$%%),
        group_by_regex("Random strings", mod, %regex~(any_.*|long_string|number|positive_int)$%%),
        group_by_regex("Date and time", mod, %regex~(month|day|date|is_leap_year|week_day)%%)
    )
    document("Faker", mod, "faker.rst", groups)
}

def document_module_flat_hash_table(root : string) {
    var mod = find_module("flat_hash_table")
    var groups <- array<DocGroup>(
        group_by_regex("Template structure", mod, %regex~(TFlatHashTable)$%%),
        group_by_regex("Basic operations", mod, %regex~(empty|length|clear|key_exists|erase|get)$%%),
        group_by_regex("Memory management", mod, %regex~(grow|rehash|reserve)$%%),
        group_by_regex("Iteration", mod, %regex~(foreach|keys|values)$%%),
        group_by_regex("Internal operations", mod, %regex~(key_index)$%%),
        group_by_regex("Operators", mod, %regex~(\[\]|\?\[\])$%%)
    )
    document("Flat hash table", mod, "flat_hash_table.rst", groups)
}

def document_module_fuzzer(root : string) {
    var mod = find_module("fuzzer")
    var groups <- array<DocGroup>(
        group_by_regex("Fuzzer tests", mod, %regex~fuzz.*$%%)
    )
    document("Fuzzer", mod, "fuzzer.rst", groups)
}

def document_module_match(root : string) {
    var mod = find_module("match")
    var groups <- array<DocGroup>(
        hide_group(group_by_regex("Implementation details", mod, %regex~(match_type|match_expr)$%%))
    )
    document("Pattern matching", mod, "match.rst", groups)
}

def document_module_linq(root : string) {
    var mod = find_module("linq")
    var groups <- array<DocGroup>(
        group_by_regex("Sorting data", mod, %regex~(reverse.*|order.*)$%%),
        group_by_regex("Set operations", mod, %regex~(distinct.*|union.*|intersect.*|except.*|unique.*)$%%),
        group_by_regex("Concatenation operations", mod, %regex~(concat.*|append.*|prepend.*)$%%),
        group_by_regex("Generation operations", mod, %regex~(default_empty|repeat|empty|range_sequence)$%%),
        group_by_regex("Aggregation operations", mod, %regex~(count.*|long_count.*|sum.*|min.*|max.*|average.*|aggregate.*)$%%),
        group_by_regex("Filtering data", mod, %regex~(where_.*)$%%),
        group_by_regex("Partitioning data", mod, %regex~(skip.*|take.*|chunk.*)$%%),
        group_by_regex("Join and group operations", mod, %regex~(join.*|group_join.*|group.*)$%%),
        group_by_regex("Querying data", mod, %regex~(all.*|any.*|contains.*)$%%),
        group_by_regex("Element operations", mod, %regex~(first.*|last.*|single.*|element_at.*)$%%),
        group_by_regex("Transform operations", mod, %regex~(select.*|zip.*)$%%),
        group_by_regex("Convertion operations", mod, %regex~(to_table.*|to_sequence.*)$%%),
        group_by_regex("Comparators and keys", mod, %regex~(less.*|unique_key.*|sequence_equal.*)$%%)
    )
    document("LINQ", mod, "linq.rst", groups)
}

def document_module_linq_boost(root : string) {
    var mod = find_module("linq_boost")
    var groups <- array<DocGroup>(
        hide_group(group_by_regex("Implementation details", mod, %regex~(match_type|match_expr)$%%))
    )
    document("Boost module for LINQ", mod, "linq_boost.rst", groups)
}

def document_module_class_boost(root : string) {
    var mod = find_module("class_boost")
    var groups <- array<DocGroup>(
    )
    document("Class method macros", mod, "class_boost.rst", groups)
}

def document_module_soa(root : string) {
    var mod = find_module("soa")
    var groups <- array<DocGroup>(
        group_by_regex("SOA field access", mod, %regex~\.$%%)
    )
    document("SOA (Structure of Arrays) transformation", mod, "soa.rst", groups)
}

def document_module_base64(root : string) {
    var mod = find_module("base64")
    var groups <- array<DocGroup>(
        group_by_regex("Encoding", mod, %regex~(base64_encode|BASE64_ENCODE_OUT_SIZE)$%%),
        group_by_regex("Decoding", mod, %regex~(base64_decode|BASE64_DECODE_OUT_SIZE)$%%)
    )
    document("Base64 encoding and decoding", mod, "base64.rst", groups)
}

// def document_module_linked_list(root : string) {
//     var mod = find_module("linked_list")
//     var groups <- array<DocGroup>(
//     )
//     document("Intrusive linked list", mod, "linked_list.rst", groups)
// }

def document_module_bool_array(root : string) {
    var mod = find_module("bool_array")
    var groups <- array<DocGroup>(
        group_by_regex("Iteration", mod, %regex~(each)$%%)
    )
    document("Packed boolean array", mod, "bool_array.rst", groups)
}

def document_module_bitfield_boost(root : string) {
    var mod = find_module("bitfield_boost")
    var groups <- array<DocGroup>(
        group_by_regex("Bitfield element access", mod, %regex~(\[\]|\[\]\=|\[\]\&\&\=|\[\]\|\|\=|\[\]\^\^\=)$%%),
        group_by_regex("Iteration", mod, %regex~(each)$%%)
    )
    document("Bitfield operator overloads", mod, "bitfield_boost.rst", groups)
}

def document_module_bitfield_trait(root : string) {
    var mod = find_module("bitfield_trait")
    var groups <- array<DocGroup>(
        group_by_regex("Iteration", mod, %regex~(each)$%%)
    )
    document("Bitfield name traits", mod, "bitfield_trait.rst", groups)
}

def document_module_async_boost(root : string) {
    var mod = find_module("async_boost")
    var groups <- array<DocGroup>(
        group_by_regex("Awaiting", mod, %regex~(await|await_next_frame)$%%),
        group_by_regex("Running async tasks", mod, %regex~(async_run|async_run_all)$%%)
    )
    document("Async/await coroutine macros", mod, "async_boost.rst", groups)
}

def document_module_cuckoo_hash_table(root : string) {
    var mod = find_module("cuckoo_hash_table")
    var groups <- array<DocGroup>(
        group_by_regex("Hash functions", mod, %regex~(hash0|hash1|hash_extra)$%%)
    )
    document("Cuckoo hash table", mod, "cuckoo_hash_table.rst", groups)
}

def document_module_consume(root : string) {
    var mod = find_module("consume")
    var groups <- array<DocGroup>(
    )
    document("Consume argument optimization", mod, "consume.rst", groups)
}

def document_module_dap(root : string) {
    var mod = find_module("dap")
    var groups <- array<DocGroup>(
        group_by_regex("JSON deserialization", mod, %regex~(InitializeRequestArguments|DisconnectArguments|DataBreakpoint|SetDataBreakpointsArguments|DataBreakpointInfoArguments|SourceBreakpoint|Source|SetBreakpointsArguments|StackTraceArguments|ScopesArguments|VariablesArguments|ContinueArguments|PauseArguments|StepInArguments|NextArguments|StepOutArguments|EvaluateArguments)$%%),
        group_by_regex("JSON serialization", mod, %regex~(JV)$%%),
        group_by_regex("JSON field accessors", mod, %regex~(joj|jon|j_s|jos|job)$%%)
    )
    document("Debug Adapter Protocol data structures", mod, "dap.rst", groups)
}

def document_module_das_source_formatter(root : string) {
    var mod = find_module("das_source_formatter")
    var groups <- array<DocGroup>(
        group_by_regex("Formatting", mod, %regex~(format_das_string|format_source_string|format_source)$%%)
    )
    document("Source code formatter", mod, "das_source_formatter.rst", groups)
}

def document_module_das_source_formatter_fio(root : string) {
    var mod = find_module("das_source_formatter_fio")
    var groups <- array<DocGroup>(
        group_by_regex("File formatting", mod, %regex~(format_file|format_files)$%%)
    )
    document("File-based source code formatter", mod, "das_source_formatter_fio.rst", groups)
}

def document_module_debug(root : string) {
    var mod = find_module("debug")
    var groups <- array<DocGroup>(
    )
    document("DAP debug server", mod, "debug.rst", groups)
}

def document_module_debug_eval(root : string) {
    var mod = find_module("debug_eval")
    var groups <- array<DocGroup>(
        group_by_regex("Evaluation", mod, %regex~(eval|debug_eval)$%%),
        group_by_regex("Tokenization", mod, %regex~(each_token)$%%)
    )
    document("Debug expression evaluator", mod, "debug_eval.rst", groups)
}

def document_module_decs_state(root : string) {
    var mod = find_module("decs_state")
    var groups <- array<DocGroup>(
    )
    document("DECS debug state reporting", mod, "decs_state.rst", groups)
}

def document_module_dynamic_cast_rtti(root : string) {
    var mod = find_module("dynamic_cast_rtti")
    var groups <- array<DocGroup>(
        group_by_regex("Dynamic casts", mod, %regex~(is_instance_of|as_instance_of|dynamic_type_cast|force_dynamic_type_cast)$%%)
    )
    document("Dynamic RTTI type casts", mod, "dynamic_cast_rtti.rst", groups)
}

def document_module_generic_return(root : string) {
    var mod = find_module("generic_return")
    var groups <- array<DocGroup>(
    )
    document("Generic return type instantiation", mod, "generic_return.rst", groups)
}

def document_module_heartbeat(root : string) {
    var mod = find_module("heartbeat")
    var groups <- array<DocGroup>(
        group_by_regex("Heartbeat control", mod, %regex~(set_heartbeat|heartbeat)$%%)
    )
    document("Heartbeat callback injection", mod, "heartbeat.rst", groups)
}

def document_module_lint(root : string) {
    var mod = find_module("lint")
    var groups <- array<DocGroup>(
        group_by_regex("Lint operations", mod, %regex~(paranoid)$%%)
    )
    document("Paranoid lint pass", mod, "lint.rst", groups)
}

def document_module_lint_everything(root : string) {
    var mod = find_module("lint_everything")
    var groups <- array<DocGroup>(
    )
    document("Global paranoid lint pass", mod, "lint_everything.rst", groups)
}

def document_module_live(root : string) {
    var mod = find_module("live")
    var groups <- array<DocGroup>(
        group_by_regex("Lifecycle", mod, %regex~(live_init|live_update|live_bind_server|live_main)$%%)
    )
    document("Live reload framework", mod, "live.rst", groups)
}

def document_module_profiler(root : string) {
    var mod = find_module("profiler")
    var groups <- array<DocGroup>(
        group_by_regex("Profiler control", mod, %regex~(set_enable_profiler)$%%)
    )
    document("Instrumenting profiler", mod, "profiler.rst", groups)
}

def document_module_profiler_boost(root : string) {
    var mod = find_module("profiler_boost")
    var groups <- array<DocGroup>(
        group_by_regex("Context profiler control", mod, %regex~(enable_profiler|disable_profiler)$%%)
    )
    document("Profiler cross-context helpers", mod, "profiler_boost.rst", groups)
}

def document_module_coverage(root : string) {
    var mod = find_module("coverage")
    var groups <- array<DocGroup>(
        group_by_regex("Coverage data access", mod, %regex~(get_report|add_coverage)$%%)
    )
    document("Code coverage instrumentation", mod, "coverage.rst", groups)
}

def document_module_quote(root : string) {
    var mod = find_module("quote")
    var groups <- array<DocGroup>(
        group_by_regex("Clone operations", mod, %regex~(clone|clone_line_info|clone_file_info)$%%),
        group_by_regex("Conversion", mod, %regex~(cvt_to_mks)$%%)
    )
    document("AST quasiquoting infrastructure", mod, "quote.rst", groups)
}

def document_module_refactor(root : string) {
    var mod = find_module("refactor")
    var groups <- array<DocGroup>(
        group_by_regex("Refactoring operations", mod, %regex~(extract_method|rename|extract_expression|extract_variable_nonref|extract_variable_ref)$%%)
    )
    document("Automated refactoring tools", mod, "refactor.rst", groups)
}

def document_module_remove_call_args(root : string) {
    var mod = find_module("remove_call_args")
    var groups <- array<DocGroup>(
    )
    document("Call argument removal annotation", mod, "remove_call_args.rst", groups)
}

def document_module_stringify(root : string) {
    var mod = find_module("stringify")
    var groups <- array<DocGroup>(
    )
    document("Long string embedding macro", mod, "stringify.rst", groups)
}

def document_module_temp_strings(root : string) {
    var mod = find_module("temp_strings")
    var groups <- array<DocGroup>(
        group_by_regex("Temporary string builders", mod, %regex~(build_temp_string)$%%),
        group_by_regex("Temporary string conversion", mod, %regex~(temp_string)$%%)
    )
    document("Temporary string utilities", mod, "temp_strings.rst", groups)
}

def document_module_type_traits(root : string) {
    var mod = find_module("type_traits")
    var groups <- array<DocGroup>(
    )
    document("Type trait macros", mod, "type_traits.rst", groups)
}

def document_module_typemacro_boost(root : string) {
    var mod = find_module("typemacro_boost")
    var groups <- array<DocGroup>(
        group_by_regex("Enum helpers", mod, %regex~(int64_to_enum)$%%),
        group_by_regex("Template structure instantiation", mod, %regex~(make_typemacro_template_instance|is_typemacro_template_instance|template_structure_name)$%%),
        group_by_regex("Type inference helpers", mod, %regex~(infer_struct_aliases|infer_template_types|add_structure_aliases|verify_arguments)$%%),
        group_by_regex("String constant access", mod, %regex~(get_string_const)$%%),
        group_by_regex("Work tracking", mod, %regex~(is_custom_work_done|mark_custom_work_done)$%%),
        group_by_regex("Type macro arguments", mod, %regex~(typemacro_argument)$%%)
    )
    document("Type macro and template structure support", mod, "typemacro_boost.rst", groups)
}

def document_module_validate_code(root : string) {
    var mod = find_module("validate_code")
    var groups <- array<DocGroup>(
    )
    document("Code validation annotations", mod, "validate_code.rst", groups)
}

[export]
def main {
    let root = get_das_root() + "/doc/source/stdlib"// todo: modify to output /temp so that we can merge changes
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
    // document daslib modules
    document_module_algorithm(root)
    document_module_apply_in_context(root)
    document_module_apply(root)
    document_module_archive(root)
    document_module_array_boost(root)
    document_module_assert_once(root)
    document_module_ast_block_to_loop(root)
    document_module_ast_boost(root)
    document_module_ast_used(root)
    document_module_async_boost(root)
    document_module_base64(root)
    document_module_bitfield_boost(root)
    document_module_bitfield_trait(root)
    document_module_bool_array(root)
    document_module_class_boost(root)
    document_module_constexpr(root)
    document_module_consume(root)
    document_module_contracts(root)
    document_module_coroutines(root)
    // document_module_coverage(root)              // [infer_macro] modifies AST of all modules
    document_module_cpp_bind(root)
    document_module_cuckoo_hash_table(root)
    document_module_dap(root)
    document_module_das_source_formatter(root)
    document_module_das_source_formatter_fio(root)
    // document_module_debug(root)                 // uses persistent_heap, complex deps
    document_module_debug_eval(root)
    document_module_decs(root)
    document_module_decs_boost(root)
    document_module_decs_state(root)
    document_module_defer(root)
    document_module_dynamic_cast_rtti(root)
    document_module_enum_trait(root)
    document_module_export_constructor(root)
    document_module_faker(root)
    document_module_flat_hash_table(root)
    document_module_functional(root)
    document_module_fuzzer(root)
    document_module_generic_return(root)
    // document_module_heartbeat(root)              // qmacro(heartbeat()) conflicts when used in das2rst context
    document_module_if_not_null(root)
    document_module_instance_function(root)
    document_module_interfaces(root)
    document_module_is_local(root)
    document_module_jobque_boost(root)
    document_module_json_boost(root)
    document_module_json(root)
    // document_module_linked_list(root)         // spoof template, not documentable
    document_module_linq(root)
    document_module_linq_boost(root)
    document_module_lint(root)
    // document_module_lint_everything(root)       // global_lint_macro causes lint errors in other modules
    // document_module_live(root)                  // uses multiple_contexts
    document_module_lpipe(root)
    document_module_macro_boost(root)
    document_module_match(root)
    document_module_math_bits(root)
    document_module_math_boost(root)
    document_module_profiler(root)
    document_module_profiler_boost(root)
    document_module_quote(root)
    document_module_random(root)
    document_module_refactor(root)
    document_module_regex_boost(root)
    document_module_regex(root)
    document_module_remove_call_args(root)
    document_module_rst(root)
    document_module_safe_addr(root)
    document_module_soa(root)
    document_module_sort_boost(root)
    document_module_static_let(root)
    document_module_stringify(root)
    document_module_strings_boost(root)
    document_module_temp_strings(root)
    document_module_templates_boost(root)
    document_module_templates(root)
    document_module_type_traits(root)
    document_module_typemacro_boost(root)
    document_module_unroll(root)
    document_module_uriparser_boost(root)
    document_module_utf8_utils(root)
    document_module_validate_code(root)
}
