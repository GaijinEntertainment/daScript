// Compiler AST inspection/code generation is tested in the interpreter.
options gen2
options rtti
options indenting = 4
require dastest/testing_boost public
require daslib/cpp_bind
require daslib/ast_boost
require strings
require daslib/strings_boost
require daslib/fio
require daslib/clargs

class AdapterProbe {
    def abstract required(value : int) : int
    def defaulted(value : int) : int => value + 1
    def hidden(value : int) : int => value - 1
}

[test]
def adapter_explicitly_includes_default_callbacks(t : T?) {
    ast_gc_guard() {
        var declaration = typeinfo ast_typedecl(type<AdapterProbe>)
        let output = build_string() $(var writer) {
            log_cpp_class_adapter(writer, "Adapter", declaration, ["defaulted"])
        }
        t |> success(contains(output, "invoke_required"))
        t |> success(contains(output, "invoke_defaulted"))
        t |> success(!contains(output, "invoke_hidden"))
        let legacy = build_string() $(var writer) {
            log_cpp_class_adapter(writer, "Adapter", declaration)
        }
        t |> success(contains(legacy, "invoke_required"))
        t |> success(!contains(legacy, "invoke_defaulted"))
    }
}

[test]
def adapter_unknown_default_callback_is_an_error(t : T?) {
    var output : string
    let code = run_and_capture([get_host_binary(), "-dasroot", get_das_root(), "{get_das_root()}/tests/ast/_fixture_cpp_adapter_missing.das"], output, 15.0)
    t |> success(code != 0)
    t |> success(contains(output, "adapter method missing does not exist"))
}
