options gen2
options indenting = 4

module _gate_macro shared private

require daslib/ast
require daslib/ast_boost
require daslib/fio
require strings

class GatedCallMacro : AstCallMacro {
    def override visit(prog : ProgramPtr; mod : Module?;
                       var expr : ExprCallMacro?) : ExpressionPtr =>
        new ExprConstInt(at = expr.at, value = 42)
}

//! registers a call macro into the module it compiles, so that
//! module's cache record is never served and it compiles again in
//! place every run; the compile fails while `verdict.txt` beside
//! the module reads `compile-fail`
[function_macro(name = "gated")]
class GatedAnnotation : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       var args : AnnotationArgumentList; var errors : das_string) : bool {
        let verdict = path_join(dir_name(string(func.at.fileInfo.name)), "verdict.txt")
        if (strip(fread(verdict)) == "compile-fail") {
            errors := "the verdict file asks for a compile failure"
            return false
        }
        let gated = make_call_macro(string(func.name), new GatedCallMacro())
        compiling_module() |> add_call_macro(gated)
        return true
    }
}
