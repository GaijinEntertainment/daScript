options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot

module testing_boost shared

require daslib/ast_boost
require testing public

[function_macro(name="test")]
class TestFunctionAnnotation : AstFunctionAnnotation
    def override apply(var func: FunctionPtr; var group: ModuleGroup; args: AnnotationArgumentList; var errors: das_string): bool
        func.flags |= FunctionFlags exports
        return true
