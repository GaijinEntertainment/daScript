options gen2

module test_compile_dag_nesting public

require daslib/ast_boost
require daslib/cross_context

[function_macro(name="deadlock_nesting_probe")]
class DeadlockNestingProbe : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       var args : AnnotationArgumentList; var errors : das_string) : bool {
        if (length(args) > 1000) {
            return require_module_now("daslib/strings_boost") != null
        }
        return true
    }
}
