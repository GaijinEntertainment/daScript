options gen2

module _nested_compile_macro shared

require daslib/ast_boost
require daslib/cross_context

[simulate_macro]
class NestedCompileProbe : AstSimulateMacro {
    def override preSimulate(prog : Program?; ctx : Context?) : bool {
        return true
    }
    def override simulate(prog : Program?; ctx : Context?) : bool {
        return require_module_now("daslib/strings_boost") != null
    }
}
