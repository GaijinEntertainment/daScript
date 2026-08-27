options gen2

module optimize_breaker shared

require daslib/ast
require daslib/ast_boost

var private g_broken = false

class Breaker : AstVisitor {
    def override visitExprOp2(var expr : ExprOp2?) : ExpressionPtr {
        if (!g_broken && expr.right != null) {
            expr.right = null
            g_broken = true
        }
        return expr
    }
}

[optimization_macro]
class BreakAtOptimizeMacro : AstPassMacro {
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        if (mod == null || !empty(mod.name) || g_broken) {
            return false
        }
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
