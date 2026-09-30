options gen2
options rtti
require daslib/cpp_bind
require daslib/ast_boost
require strings
class AdapterMissingProbe {
    def defaulted : int { return 1 }
}
[export]
def main {
    ast_gc_guard() {
        var declaration = typeinfo ast_typedecl(type<AdapterMissingProbe>)
        let output = build_string() $(var writer) {
            log_cpp_class_adapter(writer, "Adapter", declaration, ["missing"])
        }
        print(output)
    }
}
