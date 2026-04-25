options gen2

module clargs shared private

require daslib/ast_boost
require daslib/templates_boost
require daslib/strings_boost
require strings public

require daslib/enum_trait public

struct public CommandArgumentInfo {
    field_name : string
    flag_name : string
    doc_string : string
    value_type : Type
    is_array : bool
    is_required : bool
    enum_values : array<string>
}

struct public CommandInfo {
    args : array<CommandArgumentInfo>
}

def public get_cli_arguments() : array<string> {
    var all_args <- get_command_line_arguments()
    var arg_index = find_index(all_args, "--")
    var result : array<string>
    if (arg_index == -1) {
        return <- result
    }
    arg_index++ // Skip the "--" itself
    while (arg_index < length(all_args)) {
        result |> push(all_args[arg_index])
        arg_index++
    }
    return <- result
}

def clargs_flag_name(field_name : string) : string {
    return "--" + replace_multiple(field_name, [("_", "-")])
}

def public find_bool_flag_raw_value(args : array<string>; flag_name : string) : tuple<string, bool> {
    let prefix = "{flag_name}="
    for (i in range(length(args))) {
        if (args[i] == flag_name) {
            return ("true", true)
        }
        if (starts_with(args[i], prefix)) {
            return (slice(args[i], length(prefix)), true)
        }
    }
    return ("", false)
}

def public find_flag_raw_value(args : array<string>; flag_name : string) : tuple<string, bool> {
    let prefix = "{flag_name}="
    for (i in range(length(args))) {
        if (args[i] == flag_name) {
            if (i + 1 < length(args)) {
                return (args[i + 1], true)
            }
        }
        if (starts_with(args[i], prefix)) {
            return (slice(args[i], length(prefix)), true)
        }
    }
    return ("", false)
}

// Returns true if any form of flag_name (--flag, --flag=value, --flag value)
// is present in args.  Used for required-flag validation.
def public flag_present(args : array<string>; flag_name : string) : bool {
    let prefix = "{flag_name}="
    for (a in args) {
        if (a == flag_name) {
            return true
        }
        if (starts_with(a, prefix)) {
            return true
        }
    }
    return false
}

def public find_array_flag_raw_values(args : array<string>; flag_name : string) : array<string> {
    var result : array<string>
    let prefix = "{flag_name}="
    var i = 0
    while (i < length(args)) {
        let arg = args[i]
        i++
        if (arg == flag_name) {
            if (i < length(args)) {
                result |> push(args[i])
                i++
            }
            continue
        }
        if (starts_with(arg, prefix)) {
            result |> push(slice(arg, length(prefix)))
        }
    }
    return <- result
}

def public parse_argument(var dst : string&; args : array<string>; arg_info : CommandArgumentInfo) : string {
    let (raw_value, found) = find_flag_raw_value(args, arg_info.flag_name)
    if (found) {
        dst = raw_value
    }
    return ""
}

def public parse_argument(var dst : int&; args : array<string>; arg_info : CommandArgumentInfo) : string {
    let (raw_value, found) = find_flag_raw_value(args, arg_info.flag_name)
    if (found) {
        var result : ConversionResult
        var offset = 0
        let parsed = int(raw_value, result, offset)
        // Reject both outright parse failures and trailing garbage ("42abc").
        if (result != ConversionResult.ok || offset != length(raw_value)) {
            return "invalid int value '{raw_value}'"
        }
        dst = parsed
    }
    return ""
}


def public parse_argument(var dst : float&; args : array<string>; arg_info : CommandArgumentInfo) : string {
    let (raw_value, found) = find_flag_raw_value(args, arg_info.flag_name)
    if (found) {
        var result : ConversionResult
        var offset = 0
        let parsed = float(raw_value, result, offset)
        if (result != ConversionResult.ok || offset != length(raw_value)) {
            return "invalid float value '{raw_value}'"
        }
        dst = parsed
    }
    return ""
}

def public parse_argument(var dst : array<string>&; args : array<string>; arg_info : CommandArgumentInfo) : string {
    var raw_values = find_array_flag_raw_values(args, arg_info.flag_name)
    if (!empty(raw_values)) {
        dst <- raw_values
    }
    return ""
}

def public parse_argument(var dst : bool&; args : array<string>; arg_info : CommandArgumentInfo) : string {
    let (raw_value, found) = find_bool_flag_raw_value(args, arg_info.flag_name)
    if (found) {
        if (raw_value == "true") {
            dst = true
        } elif (raw_value == "false") {
            dst = false
        } else {
            return "invalid bool value: '{raw_value}'"
        }
    }
    return ""
}

[structure_macro(name=CommandLineArgs)]
class ArgsMacro : AstStructureAnnotation {
    def override apply(var st : StructurePtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        let command_info = collect_command_info(st)

        var command_info_fn = make_command_info_fn(st, command_info)
        compiling_module() |> add_function(command_info_fn)

        var parse_args_fn = make_parse_fn(st, command_info)
        compiling_module() |> add_function(parse_args_fn)

        var parse_args_default_fn = make_parse_default_fn(st)
        compiling_module() |> add_function(parse_args_default_fn)

        return true
    }

    def find_bool_annotation(args : AnnotationArgumentList; argn : string, default_value : bool) : bool {
        let v = find_arg(args, argn)
        if (v is tBool) {
            return v as tBool
        }
        return default_value
    }

    def find_string_annotation(args : AnnotationArgumentList; argn : string, default_value : string) : string {
        let v = find_arg(args, argn)
        if (v is tString) {
            return v as tString
        }
        return default_value
    }

    // collect_command_info turns the structure info into command info.
    // It serves two functions: it makes the code generation easier
    // by pre-processing the raw type data, plus it makes it
    // possible to have an easy-to-use runtime value with all command info.
    // This info can be used to print command's help, for example.
    def collect_command_info(st : StructurePtr) : CommandInfo {
        var result : CommandInfo
        result.args |> reserve(length(st.fields))
        for (field in st.fields) {
            if (find_bool_annotation(field.annotation, "clarg_skip", false)) {
                continue
            }
            var arg : CommandArgumentInfo
            arg.field_name = "{field.name}"
            arg.flag_name = clargs_flag_name(find_string_annotation(field.annotation, "clarg_name", arg.field_name))
            arg.is_required = find_bool_annotation(field.annotation, "clarg_required", false)
            arg.value_type = field._type.baseType
            if (field._type.baseType.isVectorType()) {
                arg.value_type = field._type.baseType.getVectorElementType()
                arg.is_array = true
            }
            if (arg.value_type == Type.tEnumeration) {
                for (entry in field._type.enumType.list) {
                    arg.enum_values |> push("{entry.name}")
                }
            }
            arg.doc_string = find_string_annotation(field.annotation, "clarg_doc", "")
            result.args |> emplace(arg)
        }
        return <- result
    }

    def make_command_info_fn(var st : StructurePtr; info : CommandInfo) : FunctionPtr {
        var fn = qmacro_function("get_command_info") $(typ : $t(st)) : CommandInfo {
            var x := $v(info)
            return <- x
        }
        fn.flags |= FunctionFlags.generated
        fn.body |> force_at(st.at)
        return <- fn
    }

    def make_parse_default_fn(var st : StructurePtr) : FunctionPtr {
        var fn = qmacro_function("parse_args") $(var dst : $t(st)) : string {
            return parse_args(dst, get_cli_arguments())
        }
        fn.flags |= FunctionFlags.generated
        fn.body |> force_at(st.at)
        return <- fn
    }

    def make_simple_init_expr(i : int; arg : CommandArgumentInfo) : ExpressionPtr {
        return qmacro_block() {
            {
                let err = parse_argument(dst.$f(arg.field_name), args, command_info.args[$v(i)])
                if (err != "") {
                    return "{$v(arg.flag_name)}: {err}"
                }
            }
        }
    }

    def make_enum_init_expr(var st : StructurePtr; i : int; arg : CommandArgumentInfo; field_index : int) : ExpressionPtr {
        return qmacro_block() {
            {
                let (raw_value, found) = find_flag_raw_value(args, command_info.args[$v(i)].flag_name)
                if (found) {
                    if (find_index(command_info.args[$v(i)].enum_values, raw_value) == -1) {
                        return "{$v(arg.flag_name)}: invalid enum value '{raw_value}'"
                    }
                    dst.$f(arg.field_name) = to_enum(type<$t(st.fields[field_index]._type)>, raw_value)
                }
            }
        }
    }

    def make_parse_fn(var st : StructurePtr; info : CommandInfo) : FunctionPtr {
        var name_to_index <- { for (i in range(length(st.fields))); string(st.fields[i].name) => i }
        var field_index_by_name <- { for (arg in info.args); arg.field_name => name_to_index[arg.field_name]; where key_exists(name_to_index, arg.field_name) }

        var init_exprs : array<ExpressionPtr>
        for (i, arg in range(length(info.args)), info.args) {
            if (arg.value_type != Type.tEnumeration) {
                init_exprs |> push(make_simple_init_expr(i, arg))
                continue
            }
            let field_index = field_index_by_name[arg.field_name]
            init_exprs |> push(make_enum_init_expr(st, i, arg, field_index))
        }

        var fn = qmacro_function("parse_args") $(var dst : $t(st); args : array<string>) : string {
            let command_info <- get_command_info(type<$t(st)>)
            $b(init_exprs)
            for (arg in command_info.args) {
                if (arg.is_required && !flag_present(args, arg.flag_name)) {
                    return "{arg.flag_name}: missing required flag"
                }
            }
            return ""
        }
        fn.flags |= FunctionFlags.generated
        fn.body |> force_at(st.at)
        return <- fn
    }
}
