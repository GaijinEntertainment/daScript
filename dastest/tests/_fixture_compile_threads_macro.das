options gen2

module _fixture_compile_threads_macro shared public

require daslib/ast_boost

//! a fixture for uncategorized.das: `[three_compile_threads]` on a
//! function fails the compile unless the program's `compile_threads`
//! policy is 3
[function_macro(name = "three_compile_threads")]
class ThreeCompileThreads : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
        let threads = compiling_program().policies.compile_threads
        return true if (threads == 3)
        errors := "compile_threads is {threads}, not 3"
        return false
    }
}
