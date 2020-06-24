options rtti=true

require fio
require rtti
require ast
require math
require uriparser
require strings
require daslib/rst
require daslib/ast_boost
require daslib/strings_boost
require daslib/functional
require daslib/json
require daslib/json_boost
require daslib/regex
require daslib/regex_boost
require daslib/apply

def document_module_math(root:string)
    var mod = get_module("math")
    var groups <- [{DocGroup
        group_by_regex("all numerics (uint*, int*, float*, double)", mod, %regex~(min|max)$%%);
        group_by_regex("float* and double", mod, %regex~(abs|acos|asin|atan|atan2|cos|sin|tan|exp|log|exp2|
log2|pow|sqrt|rcp|ceil|floor|abs|saturate|sincos)$%%);
        group_by_regex("float* only", mod, %regex~(atan2_est|rcp_est|ceili|floori|roundi|trunci|rsqrt|rsqrt_est)$%%);
        group_by_regex("float3 only", mod, %regex~(cross|reflect|refract|distance|distance_sq|
inv_distance|inv_distance_sq)$%%);
        group_by_regex("float2, float3, float4", mod, %regex~(length|length_sq|inv_length|inv_length_sq|
dot|normalize|fast_normalize)$%%);
        group_by_regex("Noise functions", mod, %regex~(uint32_hash|uint_noise_1D|uint_noise_2D|uint_noise_3D)$%%);
        group_by_regex("lerp/mad/clamp", mod, %regex~(lerp|mad|clamp)$%%);
        group_by_regex("Matrix operations", mod, %regex~(\=\=|\!\=|\*)$%%);
        group_by_regex("Matrix constructors", mod, %regex~(float3x4|float4x4)$%%);
        group_by_regex("Matrix manipulation", mod, %regex~(identity|inverse|rotate|transpose|translation)$%%)
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
find_for_edit_if_exists|lock_forever|next|nothing|pop|push|push_clone|sort|to_array|to_table|to_array_move|
to_table_move|empty)$%%);
        group_by_regex("Character set groups", mod, %regex~(is_alpha|is_number|is_white_space|is_char_in_set)$%%);
        group_by_regex("das::string manipulation", mod, %regex~(peek|set)$%%);
        group_by_regex("String builder", mod, %regex~(build_string|write|write_char|write_chars|write_escape_string)$%%);
        group_by_regex("Heap reporting", mod, %regex~(heap|string_heap)%%);
        group_by_regex("Vector and matrix math", mod, %regex~(float3x4|float4x4|identity|inverse|rotate|translation|transpose)$%%);
        group_by_regex("GC0 infrastructure", mod, %regex~gc0%%);
        group_by_regex("Smart ptr infrastructure", mod, %regex~(smart_ptr|get_ptr$)%%);
        group_by_regex("Macro infrastructure", mod, %regex~is_compiling%%);
        group_by_regex("Profiler", mod, %regex~(profile|reset_profiler|dump_profile_info|collect_profile_info)$%%);
        group_by_regex("System infastructure", mod, %regex~(panic|print|sprint|terminate|breakpoint|stackwalk|get_das_root|get_clock)$%%);
        group_by_regex("Memory manipulation", mod, %regex~(intptr|memcmp|variant_index|set_variant_index|hash)$%%);
        group_by_regex("Binary serializer", mod, %regex~(binary_load|binary_save)$%%);
        group_by_regex("Path and command line", mod, %regex~(get_command_line_arguments)$%%)
    }]
    document("Built-in runtime",mod,"{root}/builtin.rst","{root}/detail/builtin.rst",groups)

def document_module_fio(root:string)
    var mod = get_module("fio")
    var groups <- [{DocGroup
        hide_group(group_by_regex("Internal builtin functions", mod, %regex~builtin%%));
        group_by_regex("File manipulation", mod, %regex~(f|stat$)%%);
        group_by_regex("Path manipulation", mod, %regex~(base_name|dir_name)$%%);
        group_by_regex("Directory manipulation", mod, %regex~(dir|mkdir)$%%);
        group_by_regex("OS thread helper", mod, %regex~(sleep)$%%)
    }]
    document("File input output library",mod,"{root}/fio.rst","{root}/detail/fio.rst",groups)

def document_module_random(root:string)
    var mod = get_module("random")
    var groups <- [{DocGroup
        group_by_regex("Seed and basic generators", mod, %regex~(random_seed|random_float|random_float4|random_int|random_int4)$%%);
        group_by_regex("Specific distributions", mod, %regex~(random_unit_vector|random_in_unit_disk|random_in_unit_sphere)$%%)
    }]
    document("Random generator library",mod,"{root}/random.rst","{root}/detail/random.rst",groups)

def document_module_network(root:string)
    var mod = get_module("network")
    var groups <- [{DocGroup
        group_by_regex("stub0", mod, %regex~(stub0)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("Network socket library",mod,"{root}/network.rst","{root}/detail/network.rst",groups)

def document_module_uriparser(root:string)
    var mod = get_module("uriparser")
    var groups <- [{DocGroup
        group_by_regex("stub0", mod, %regex~(stub0)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("URI manipulation library based on UriParser",mod,"{root}/uriparser.rst","{root}/detail/uriparser.rst",groups)

def document_module_rtti(root:string)
    var mod = get_module("rtti")
    var groups <- [{DocGroup
        group_by_regex("stub0", mod, %regex~(stub0)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("Runtime type information library",mod,"{root}/rtti.rst","{root}/detail/rtti.rst",groups)

def document_module_ast(root:string)
    var mod = get_module("ast")
    var groups <- [{DocGroup
        group_by_regex("stub0", mod, %regex~(stub0)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("AST manipulation library",mod,"{root}/ast.rst","{root}/detail/ast.rst",groups)

def document_module_ast_boost(root:string)
    var mod = find_module("ast_boost")
    var groups <- [{DocGroup
        group_by_regex("stub0", mod, %regex~(stub0)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
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
strip|strip_left|strip_right|to_lower|to_lower_in_place|to_upper|to_upper_in_place)$%%);
        group_by_regex("Search substrings", mod, %regex~find.*%%);
        group_by_regex("String conversion routines", mod, %regex~(double|float|int|uint|to_float|to_int|string|to_char)$%%);
        group_by_regex("Low level memory allocation", mod, %regex~(delete_string)$%%)
    }]
    document("String manipulation library",mod,"{root}/strings.rst","{root}/detail/strings.rst",groups)

def document_module_strings_boost(root:string)
    var mod = find_module("strings_boost")
    var groups <- [{DocGroup
        group_by_regex("stub0", mod, %regex~(stub0)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("Boost package for string manipulation library",mod,"{root}/strings_boost.rst","{root}/detail/strings_boost.rst",groups)


def document_module_functional(root:string)
    var mod = find_module("functional")
    var groups <- [{DocGroup
        group_by_regex("stub0", mod, %regex~(stub0)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("Functional programming library",mod,"{root}/functional.rst","{root}/detail/functional.rst",groups)

def document_module_json(root:string)
    var mod = find_module("json")
    var groups <- [{DocGroup
        group_by_regex("stub0", mod, %regex~(stub0)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("JSON manipulation library",mod,"{root}/json.rst","{root}/detail/json.rst",groups)

def document_module_json_boost(root:string)
    var mod = find_module("json_boost")
    var groups <- [{DocGroup
        group_by_regex("stub0", mod, %regex~(stub0)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("Boost package for JSON",mod,"{root}/json_boost.rst","{root}/detail/json_boost.rst",groups)

def document_module_regex(root:string)
    var mod = find_module("regex")
    var groups <- [{DocGroup
        group_by_regex("stub0", mod, %regex~(stub0)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("Regular expression library",mod,"{root}/regex.rst","{root}/detail/regex.rst",groups)

def document_module_regex_boost(root:string)
    var mod = find_module("regex_boost")
    var groups <- [{DocGroup
        group_by_regex("stub0", mod, %regex~(stub0)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("Boost package for REGEX",mod,"{root}/regex_boost.rst","{root}/detail/regex_boost.rst",groups)

def document_module_regex_rst(root:string)
    var mod = find_module("rst")
    var groups <- [{DocGroup
        group_by_regex("Document writers", mod, %regex~document%%);
        group_by_regex("Descriptions", mod, %regex~describe%%);
        group_by_regex("Label makers", mod, %regex~\w+_label_name%%);
        group_by_regex("RST section makers", mod, %regex~make%%);
        group_by_regex("Documentation queries", mod, %regex~.+_needs%%);
        group_by_regex("Group operations", mod, %regex~.*group%%);
        group_by_regex("Naming helpers", mod, %regex~(topic|module_name)$%%)
    }]
    document("Documentation generator",mod,"{root}/rst.rst","{root}/detail/rst.rst",groups)

def document_module_apply(root:string)
    var mod = find_module("apply")
    var groups <- [{DocGroup
        group_by_regex("stub0", mod, %regex~(stub0)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("Apply reflection pattern",mod,"{root}/apply.rst","{root}/detail/apply.rst",groups)

[export]
def test
    let root = get_das_root()+"/doc/source/stdlib"  // todo: modify to output /temp so that we can merge changes
    topic_root = "{root}/detail"
    // document embedded modules
    document_module_builtin(root)
    document_module_math(root)
    document_module_fio(root)
    document_module_random(root)
    document_module_network(root)
    document_module_uriparser(root)
    document_module_rtti(root)
    document_module_ast(root)
    // document daslib modules
    document_module_ast_boost(root)
    document_module_strings(root)
    document_module_strings_boost(root)
    document_module_functional(root)
    document_module_json(root)
    document_module_json_boost(root)
    document_module_regex(root)
    document_module_regex_boost(root)
    document_module_regex_rst(root)
    document_module_apply(root)
    return true
