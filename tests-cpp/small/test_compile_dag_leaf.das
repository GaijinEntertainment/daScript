options gen2

module test_compile_dag_leaf public

require daslib/ast_boost
require daslib/cross_context

[function_macro(name="deadlock_leaf_probe")]
class DeadlockLeafProbe : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       var args : AnnotationArgumentList; var errors : das_string) : bool =>
                           length(args) <= 1000
}
