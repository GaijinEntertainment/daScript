options gen2
options indenting = 4

module mc_apply_macro shared private

require daslib/ast
require daslib/ast_boost

//! the call macro `[mc_kind]` registers into the module it is compiling: `<kind>(x)` becomes 42
class McKindCallMacro : AstCallMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var expr : ExprCallMacro?) : ExpressionPtr {
        return new ExprConstInt(at = expr.at, value = 42)
    }
}

//! the annotation whose apply registers a call macro at compile time, into the compiling module -
//! the registration a cache record never replays (src/ast/ARCHITECTURE.md#module-cache-read)
[function_macro(name = "mc_kind")]
class McKindAnnotation : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup; var args : AnnotationArgumentList; var errors : das_string) : bool {
        compiling_module() |> add_call_macro(make_call_macro(string(func.name), new McKindCallMacro()))
        return true
    }
}
