options gen2
options no_aot = true

module _llvm_tune_carry_probe shared private

require daslib/ast
require daslib/ast_boost
require llvm/daslib/llvm_tune

// The macro half of llvm_tune.das's carry test: `[carry_twice(from =
// "<function>")]` runs tune_carry_into from the named function onto
// the annotated one twice, the way two passes of [tune]'s patch
// would, and prints the carried counts.

[function_macro(name = "carry_twice")]
class private CarryTwice : AstFunctionAnnotation {
    def override patch(var func : FunctionPtr; var group : ModuleGroup;
                       var args : AnnotationArgumentList; progArgs : AnnotationArgumentList;
                       var errors : das_string; var astChanged : bool&) : bool {
        return true if (find_arg(args, "done") is tBool)
        let from = find_arg(args, "from") ?as tString ?? ""
        var origin = find_unique_function(compiling_module(), from, true)
        if (origin == null) {
            errors := "carry_twice: no function {from}"
            return false
        }
        tune_carry_into(func, origin)
        tune_carry_into(func, origin)
        args |> add_annotation_argument("done", true)
        let nfm = annotation_count(func, "never_fast_math")
        let hints = annotation_count(func, "hint")
        let noinl = func.moreFlags.neverInline
        print("CARRY never_fast_math {nfm} hint {hints} never_inline {noinl}\n")
        return true
    }
}
