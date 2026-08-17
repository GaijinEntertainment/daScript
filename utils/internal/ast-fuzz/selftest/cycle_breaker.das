options gen2

module cycle_breaker shared

// Self-test fixture: points an ExprOp2 at itself AFTER its own walk has passed
// that slot, so this visitor does not recurse but every later one would. The
// generator cannot build a cycle, so this is the only guard on that check.

require daslib/ast
require daslib/ast_boost
require daslib/ast_verify

class Cycler : AstVisitor {
    done : bool = false
    def override visitExprOp2(var expr : ExprOp2?) : ExpressionPtr {
        if (!done && expr.right != null) {
            expr.right = expr
            done = true
        }
        return expr
    }
}

[infer_macro]
class CycleMacro : AstPassMacro {
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        if (mod == null || !empty(mod.name)) {
            return false
        }
        var v = new Cycler()
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
