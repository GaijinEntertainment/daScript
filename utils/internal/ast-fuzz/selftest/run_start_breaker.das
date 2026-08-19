options gen2

module run_start_breaker shared

// Self-test fixture: an [infer_macro] that nulls one ExprOp2.right and returns true, so a fresh
// inference run starts on the broken tree - its pass 0 pre-infer walk is the first to see it.
// Fires once, in the main module only.

require daslib/ast
require daslib/ast_boost

var private g_broken = false

class Breaker : AstVisitor {
    inside : bool = false
    def override preVisitFunction(var fun : FunctionPtr) : void {
        inside = fun.name == "lost_at_run_start"
    }
    def override visitExprOp2(var expr : ExprOp2?) : ExpressionPtr {
        if (inside && expr.right != null) {
            expr.right = null
            inside = false
        }
        return expr
    }
}

[infer_macro]
class BreakAtRunStart : AstPassMacro {
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
