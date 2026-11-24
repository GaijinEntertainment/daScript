options gen2

module _clone_inferred_new_macros

require daslib/ast
require daslib/ast_boost

def private first_return(fn : FunctionPtr) : ExprReturn? {
    if (fn.body == null || !(fn.body is ExprBlock)) return null
    let blk = fn.body as ExprBlock
    if (length(blk.list) == 0 || !(blk.list[0] is ExprReturn)) return null
    return blk.list[0] as ExprReturn
}

[infer_macro]
class CloneInferredNew : AstPassMacro {
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        var src : ExprNew?
        for_each_function(mod, "make_foo") $(fn) {
            let ret = first_return(fn)
            if (ret != null && ret.subexpr != null && ret.subexpr is ExprNew) {
                src = ret.subexpr as ExprNew
            }
        }
        if (src == null || src.func == null) return false
        for (target in ["target_clone", "target_null_func"]) {
            for_each_function(mod, target) $(fn) {
                var ret = first_return(fn)
                if (ret == null || ret.subexpr == null) return
                var cl = clone_expression(src)
                if (target == "target_null_func") {
                    (cl as ExprNew).func = null
                }
                ret.subexpr = cl
            }
        }
        return false
    }
}
