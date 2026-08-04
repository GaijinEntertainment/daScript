options gen2

module verify_breaker shared

// Self-test fixture: a deliberately buggy pass macro that nulls the first
// `ExprOp2.right` it finds (an invariant violation a real macro could produce),
// then calls verify_module to prove the verifier detects it WITHOUT crashing —
// the detect-and-repair scan reports the violation and repairs the tree so the
// compiler does not segfault. Not shipped behavior; a regression fixture.

require daslib/ast
require daslib/ast_boost
require daslib/ast_verify

class Breaker : AstVisitor {
    done : bool = false
    def override visitExprOp2(var expr : ExprOp2?) : ExpressionPtr {
        if (!done && expr.right != null) {
            expr.right = null
            done = true
        }
        return expr
    }
}

[infer_macro]
class BreakMacro : AstPassMacro {
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        if (mod == null || !empty(mod.name)) {
            return false
        }
        var v = new Breaker()
        make_visitor(*v) $(adapter) {
            visit_module(prog, adapter, mod)
        }
        unsafe {
            delete v
        }
        verify_module(prog, mod)
        return false
    }
}
