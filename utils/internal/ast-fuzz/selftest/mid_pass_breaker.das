options gen2

module mid_pass_breaker shared

// Self-test fixture: a [dirty_infer_macro] that nulls one ExprOp2.right between two passes of
// the SAME inference run, so the pre-infer walk of the next pass is the first to see it.
// Fires once, in the main module only.

require daslib/ast
require daslib/ast_boost

var private g_broken = false

class Breaker : AstVisitor {
    inside : bool = false
    def override preVisitFunction(var fun : FunctionPtr) : void {
        inside = fun.name == "lost_mid_pass"
    }
    def override visitExprOp2(var expr : ExprOp2?) : ExpressionPtr {
        if (inside && expr.right != null) {
            expr.right = null
            inside = false
        }
        return expr
    }
}

[dirty_infer_macro]
class BreakMidPass : AstPassMacro {
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        if (g_broken || mod == null || !empty(mod.name)) {
            return false
        }
        g_broken = true
        var v = new Breaker()
        make_visitor(*v) $(adapter) {
            visit_module(prog, adapter, mod)
        }
        unsafe {
            delete v
        }
        return true
    }
}
