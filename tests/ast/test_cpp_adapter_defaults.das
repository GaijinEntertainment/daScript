// Compiler AST inspection/code generation is tested in the interpreter.
options gen2
options rtti
options indenting = 4
require dastest/testing_boost public
require daslib/cpp_bind
require daslib/ast_boost
require strings
require daslib/strings_boost

class AdapterProbe {
    def abstract required(value : int) : int
    def defaulted(value : int) : int { return value + 1 }
    def hidden(value : int) : int { return value - 1 }
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
