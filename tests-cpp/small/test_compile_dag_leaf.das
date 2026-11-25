options gen2

module test_compile_dag_leaf public

// a comment a reader loaded before the compile must see

require daslib/ast_boost
require daslib/cross_context
require test_compile_dag_base

[function_macro(name="deadlock_leaf_probe")]
class DeadlockLeafProbe : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       var args : AnnotationArgumentList; var errors : das_string) : bool =>
                           length(args) <= compile_dag_limit + compile_dag_base()
}

def compile_dag_leaf_write() {
    compile_dag_counter ++
    compile_dag_bump(compile_dag_counter)
}
