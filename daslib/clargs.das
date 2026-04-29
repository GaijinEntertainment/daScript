options gen2

module clargs shared private

require daslib/ast_boost
require daslib/templates_boost
require daslib/strings_boost
require strings public

require daslib/option public
require daslib/result public
require daslib/enum_trait public

struct public CommandArgumentInfo {
    field_name : string
    flag_name : string
    short_flag_name : string  // e.g. "-v"; empty if no short flag declared
    doc_string : string
    value_type : Type
    is_array : bool
    is_required : bool
    enum_values : array<string>
}

struct public CommandInfo {
    args : array<CommandArgumentInfo>
}

// Returns the slice of `all_args` after a "--" separator (or empty if no "--").
// Use this for script-style invocation (`daslang script.das -- script_args...`).
def public get_cli_arguments(all_args : array<string>) : array<string> {
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

def public get_cli_arguments() : array<string> {
    return <- get_cli_arguments(get_command_line_arguments())
}

// Returns `all_args[1..]` (skips the program name).
// Use this for standalone-tool invocation where there is no "--" separator
// and argv[1..] are the actual flags.
def public get_program_args(all_args : array<string>) : array<string> {
    var result : array<string>
    var i = 1
    while (i < length(all_args)) {
        result |> push(all_args[i])
        i++
    }
    return <- result
}

def public get_program_args() : array<string> {
    return <- get_program_args(get_command_line_arguments())
}

def clargs_flag_name(field_name : string) : string {
    return "--" + replace_multiple(field_name, [("_", "-")])
}

// Duplicate-flag policy: last occurrence wins.  Matches GNU getopt, Python
// argparse, Go flag, Rust clap, and most other CLI parsers — lets users
// override an earlier flag (typically from an alias or wrapper script) by
// appending a later one.

def public find_bool_flag_raw_value(args : array<string>; flag_name : string) : Option<string> {
    let prefix = "{flag_name}="
    var result = none(type<string>)
    for (i in range(length(args))) {
        if (args[i] == flag_name) {
            result = some("true")
        } elif (starts_with(args[i], prefix)) {
            result = some(slice(args[i], length(prefix)))
        }
    }
    return result
}

def public find_flag_raw_value(args : array<string>; flag_name : string) : Option<string> {
    let prefix = "{flag_name}="
    var result = none(type<string>)
    for (i in range(length(args))) {
        if (args[i] == flag_name) {
            if (i + 1 < length(args)) {
                result = some(args[i + 1])
            }
        } elif (starts_with(args[i], prefix)) {
            result = some(slice(args[i], length(prefix)))
        }
    }
    return result
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

// Short-flag-aware overloads: single pass over args treating long and short
// as aliases.  Last-occurrence wins (same policy as the single-flag overloads
// above), so e.g. `-n Alice --name Bob` resolves to "Bob".

def public find_flag_raw_value(args : array<string>; arg_info : CommandArgumentInfo) : Option<string> {
    let has_short = !empty(arg_info.short_flag_name)
    let long_prefix = "{arg_info.flag_name}="
    let short_prefix = has_short ? "{arg_info.short_flag_name}=" : ""
    var result = none(type<string>)
    for (i in range(length(args))) {
        let arg = args[i]
        if (arg == arg_info.flag_name || (has_short && arg == arg_info.short_flag_name)) {
            if (i + 1 < length(args)) {
                result = some(args[i + 1])
            }
        } elif (starts_with(arg, long_prefix)) {
            result = some(slice(arg, length(long_prefix)))
        } elif (has_short && starts_with(arg, short_prefix)) {
            result = some(slice(arg, length(short_prefix)))
        }
    }
    return result
}

def public find_bool_flag_raw_value(args : array<string>; arg_info : CommandArgumentInfo) : Option<string> {
    let has_short = !empty(arg_info.short_flag_name)
    let long_prefix = "{arg_info.flag_name}="
    let short_prefix = has_short ? "{arg_info.short_flag_name}=" : ""
    var result = none(type<string>)
    for (arg in args) {
        if (arg == arg_info.flag_name || (has_short && arg == arg_info.short_flag_name)) {
            result = some("true")
        } elif (starts_with(arg, long_prefix)) {
            result = some(slice(arg, length(long_prefix)))
        } elif (has_short && starts_with(arg, short_prefix)) {
            result = some(slice(arg, length(short_prefix)))
        }
    }
    return result
}

// Single pass over args so that mixed long/short occurrences are returned
// in command-line order as they appear in args
// (e.g. --tags=a -t b --tags=c -> ["a", "b", "c"]).
def public find_array_flag_raw_values(args : array<string>; arg_info : CommandArgumentInfo) : array<string> {
    var result : array<string>
    let long_prefix = "{arg_info.flag_name}="
    let has_short = !empty(arg_info.short_flag_name)
    let short_prefix = has_short ? "{arg_info.short_flag_name}=" : ""
    var i = 0
    while (i < length(args)) {
        let arg = args[i]
        i++
        if (arg == arg_info.flag_name || (has_short && arg == arg_info.short_flag_name)) {
            if (i < length(args)) {
                result |> push(args[i])
                i++
            }
            continue
        }
        if (starts_with(arg, long_prefix)) {
            result |> push(slice(arg, length(long_prefix)))
            continue
        }
        if (has_short && starts_with(arg, short_prefix)) {
            result |> push(slice(arg, length(short_prefix)))
        }
    }
    return <- result
}

def public flag_present(args : array<string>; arg_info : CommandArgumentInfo) : bool {
    if (flag_present(args, arg_info.flag_name)) {
        return true
    }
    if (!empty(arg_info.short_flag_name) && flag_present(args, arg_info.short_flag_name)) {
        return true
    }
    return false
}

// For value-bearing flags (everything except bool), a bare "--flag" with no
// following value token and no "=value" form is malformed input.  Each
// parse_argument overload below returns Result<Option<T>, string> distinguishing
// three states:
//   - ok(some(v))   — found and parsed a value
//   - ok(none)      — flag absent; caller leaves the field at its default
//   - err("...")    — parse error (e.g. "missing value", "invalid int value '...')

[unused_argument(t)]
def public parse_argument(args : array<string>; arg_info : CommandArgumentInfo; t : type<string>) : Result<Option<string>; string> {
    let opt = find_flag_raw_value(args, arg_info)
    if (is_none(opt) && flag_present(args, arg_info)) {
        return err("missing value", type<Option<string>>)
    }
    return ok(opt, type<string>)
}

[unused_argument(t)]
def public parse_argument(args : array<string>; arg_info : CommandArgumentInfo; t : type<int>) : Result<Option<int>; string> {
    let opt = find_flag_raw_value(args, arg_info)
    if (is_none(opt)) {
        if (flag_present(args, arg_info)) {
            return err("missing value", type<Option<int>>)
        }
        return ok(none(type<int>), type<string>)
    }
    let raw_value = unwrap(opt)
    var result : ConversionResult
    var offset = 0
    let parsed = int(raw_value, result, offset)
    // Reject both outright parse failures and trailing garbage ("42abc").
    if (result != ConversionResult.ok || offset != length(raw_value)) {
        return err("invalid int value '{raw_value}'", type<Option<int>>)
    }
    return ok(some(parsed), type<string>)
}


[unused_argument(t)]
def public parse_argument(args : array<string>; arg_info : CommandArgumentInfo; t : type<float>) : Result<Option<float>; string> {
    let opt = find_flag_raw_value(args, arg_info)
    if (is_none(opt)) {
        if (flag_present(args, arg_info)) {
            return err("missing value", type<Option<float>>)
        }
        return ok(none(type<float>), type<string>)
    }
    let raw_value = unwrap(opt)
    var result : ConversionResult
    var offset = 0
    let parsed = float(raw_value, result, offset)
    if (result != ConversionResult.ok || offset != length(raw_value)) {
        return err("invalid float value '{raw_value}'", type<Option<float>>)
    }
    return ok(some(parsed), type<string>)
}

[unused_argument(t)]
def public parse_argument(args : array<string>; arg_info : CommandArgumentInfo; t : type<array<string>>) : Result<Option<array<string>>; string> {
    var raw_values = find_array_flag_raw_values(args, arg_info)
    if (!empty(raw_values)) {
        var sm <- move_some(raw_values)
        return move_ok(sm, type<string>)
    }
    if (flag_present(args, arg_info)) {
        return err("missing value", type<Option<array<string>>>)
    }
    return ok(none(type<array<string>>), type<string>)
}

[unused_argument(t)]
def public parse_argument(args : array<string>; arg_info : CommandArgumentInfo; t : type<bool>) : Result<Option<bool>; string> {
    let opt = find_bool_flag_raw_value(args, arg_info)
    if (is_none(opt)) {
        return ok(none(type<bool>), type<string>)
    }
    let raw_value = unwrap(opt)
    if (raw_value == "true") {
        return ok(some(true), type<string>)
    } elif (raw_value == "false") {
        return ok(some(false), type<string>)
    } else {
        return err("invalid bool value: '{raw_value}'", type<Option<bool>>)
    }
}

// Help rendering.
//
// `format_help` builds the help text as a string (testable, redirectable).
// `print_help` is a thin wrapper that prints it to stdout.
//
// Layout:
//   Usage: <prog_name> [flags]
//
//   Flags:
//     -X, --long=PLACEHOLDER    doc_string (required) (repeated)
//         --long=PLACEHOLDER    doc_string
//
// Bool flags omit the =PLACEHOLDER. Enum flag docs include `(V1|V2|...)`.

def value_placeholder(arg : CommandArgumentInfo) : string {
    if (arg.value_type == Type.tBool && !arg.is_array) {
        return ""
    }
    if (arg.value_type == Type.tInt) {
        return "=INT"
    }
    if (arg.value_type == Type.tFloat) {
        return "=FLOAT"
    }
    if (arg.value_type == Type.tEnumeration) {
        return "=ENUM"
    }
    return "=STRING"
}

def render_flag_spec(arg : CommandArgumentInfo) : string {
    var spec = ""
    if (!empty(arg.short_flag_name)) {
        spec = "{arg.short_flag_name}, "
    } else {
        spec = "    "
    }
    spec += arg.flag_name
    spec += value_placeholder(arg)
    return spec
}

def render_doc_column(arg : CommandArgumentInfo) : string {
    return build_string() $(var w) {
        var first = true
        if (!empty(arg.doc_string)) {
            write(w, arg.doc_string)
            first = false
        }
        if (!empty(arg.enum_values)) {
            if (!first) {
                write(w, " ")
            }
            first = false
            write(w, "(")
            for (i, v in range(length(arg.enum_values)), arg.enum_values) {
                if (i > 0) {
                    write(w, "|")
                }
                write(w, v)
            }
            write(w, ")")
        }
        if (arg.is_required) {
            if (!first) {
                write(w, " ")
            }
            first = false
            write(w, "(required)")
        }
        if (arg.is_array) {
            if (!first) {
                write(w, " ")
            }
            write(w, "(repeated)")
        }
    }
}

def public format_help(info : CommandInfo; prog_name : string) : string {
    var specs : array<string>
    specs |> reserve(length(info.args))
    var max_spec_len = 0
    for (arg in info.args) {
        let s = render_flag_spec(arg)
        if (length(s) > max_spec_len) {
            max_spec_len = length(s)
        }
        specs |> push(s)
    }
    let gutter = 4
    return build_string() $(var w) {
        write(w, "Usage: {prog_name} [flags]\n\nFlags:\n")
        for (i, arg in range(length(info.args)), info.args) {
            let s = specs[i]
            let pad = max_spec_len - length(s) + gutter
            write(w, "  ")
            write(w, s)
            write(w, repeat(" ", pad))
            write(w, render_doc_column(arg))
            write(w, "\n")
        }
    }
}

def public print_help(info : CommandInfo; prog_name : string) : void {
    print(format_help(info, prog_name))
}

[structure_macro(name=CommandLineArgs)]
class ArgsMacro : AstStructureAnnotation {
    def override apply(var st : StructurePtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        let command_info = collect_command_info(st, errors)
        if (!empty(errors)) {
            return false
        }

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
    //
    // On invalid input (bad @clarg_short value, short-flag collision) populates
    // `errors` and returns the partial result; the caller checks `errors` and
    // aborts macro application.
    def collect_command_info(st : StructurePtr; var errors : das_string&) : CommandInfo {
        var result : CommandInfo
        result.args |> reserve(length(st.fields))
        var seen_short : table<string; string>
        for (field in st.fields) {
            if (find_bool_annotation(field.annotation, "clarg_skip", false)) {
                continue
            }
            var arg = CommandArgumentInfo(
                field_name = "{field.name}",
                flag_name = clargs_flag_name(find_string_annotation(field.annotation, "clarg_name", "{field.name}"))
            )
            let short_char = find_string_annotation(field.annotation, "clarg_short", "")
            if (!empty(short_char)) {
                if (length(short_char) != 1) {
                    errors := "field '{arg.field_name}': @clarg_short must be a single character (got '{short_char}')"
                    return <- result
                }
                // Reject characters that produce ambiguous or unparseable
                // short flags: '-' yields '--' (collides with the script-args
                // separator), '=' yields '-=' (collides with --flag=value
                // form), whitespace can't be typed on the command line.
                if (short_char == "-" || short_char == "=" || short_char == " " || short_char == "\t" || short_char == "\n" || short_char == "\r") {
                    errors := "field '{arg.field_name}': @clarg_short cannot be '-', '=', or whitespace (got '{short_char}')"
                    return <- result
                }
                if (key_exists(seen_short, short_char)) {
                    errors := "field '{arg.field_name}': @clarg_short '-{short_char}' already used by field '{seen_short[short_char]}'"
                    return <- result
                }
                seen_short[short_char] = arg.field_name
                arg.short_flag_name = "-{short_char}"
            }
            arg.is_required = find_bool_annotation(field.annotation, "clarg_required", false)
            arg.value_type = field._type.baseType
            if (field._type.baseType == Type.tArray) {
                arg.value_type = field._type.firstType.baseType
                arg.is_array = true
            }
            if (arg.value_type == Type.tEnumeration) {
                if (arg.is_array) {
                    for (entry in field._type.firstType.enumType.list) {
                        arg.enum_values |> push("{entry.name}")
                    }
                } else {
                    for (entry in field._type.enumType.list) {
                        arg.enum_values |> push("{entry.name}")
                    }
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

    def make_simple_init_expr(var st : StructurePtr; i : int; arg : CommandArgumentInfo; field_index : int) : ExpressionPtr {
        return qmacro_block() {
            {
                var r <- parse_argument(args, command_info.args[$v(i)], type<$t(st.fields[field_index]._type)>)
                if (r |> is_err) {
                    return "{$v(arg.flag_name)}: {r |> unwrap_err}"
                }
                var opt <- r |> move_unwrap   // nolint:LINT003 — non-copyable static_if branch below consumes `opt` via `move_unwrap`, so it must stay `var`
                if (opt |> is_some) {
                    static_if (typeinfo can_copy(type<$t(st.fields[field_index]._type)>)) {
                        dst.$f(arg.field_name) = opt |> unwrap
                    } else {
                        dst.$f(arg.field_name) <- opt |> move_unwrap
                    }
                }
            }
        }
    }

    def make_enum_init_expr(var st : StructurePtr; i : int; arg : CommandArgumentInfo; field_index : int) : ExpressionPtr {
        return qmacro_block() {
            {
                let opt = find_flag_raw_value(args, command_info.args[$v(i)])
                if (is_some(opt)) {
                    let raw_value = unwrap(opt)
                    if (find_index(command_info.args[$v(i)].enum_values, raw_value) == -1) {
                        return "{$v(arg.flag_name)}: invalid enum value '{raw_value}'"
                    }
                    dst.$f(arg.field_name) = to_enum(type<$t(st.fields[field_index]._type)>, raw_value)
                } elif (flag_present(args, command_info.args[$v(i)])) {
                    return "{$v(arg.flag_name)}: missing value"
                }
            }
        }
    }

    def make_parse_fn(var st : StructurePtr; info : CommandInfo) : FunctionPtr {
        var name_to_index <- { for (i in range(length(st.fields))); string(st.fields[i].name) => i }
        var field_index_by_name <- { for (arg in info.args); arg.field_name => name_to_index[arg.field_name]; where key_exists(name_to_index, arg.field_name) }

        var init_exprs : array<ExpressionPtr>
        for (i, arg in range(length(info.args)), info.args) {
            let field_index = field_index_by_name[arg.field_name]
            if (arg.value_type != Type.tEnumeration) {
                init_exprs |> push(make_simple_init_expr(st, i, arg, field_index))
                continue
            }
            init_exprs |> push(make_enum_init_expr(st, i, arg, field_index))
        }

        var fn = qmacro_function("parse_args") $(var dst : $t(st); args : array<string>) : string {
            let command_info <- get_command_info(type<$t(st)>)
            $b(init_exprs)
            for (arg in command_info.args) {
                if (arg.is_required && !flag_present(args, arg)) {
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
