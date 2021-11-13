options indenting = 4
options no_aot

module testing_boost shared

require daslib/ast_boost
require testing public

[function_macro(name="test")]
class TestFunctionAnnotation : AstFunctionAnnotation
    [unused_argument(group, args, errors)] def override apply(var func: FunctionPtr; var group: ModuleGroup; args: AnnotationArgumentList; var errors: das_string): bool
        func.flags |= FunctionFlags exports
        return true
