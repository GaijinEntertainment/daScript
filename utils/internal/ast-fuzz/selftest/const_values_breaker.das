options gen2

module const_values_breaker shared

// Self-test fixture: sets ExprMakeArray.constValues over a literal with a variable element -
// the flag a macro that rewrites a literal's values must clear, and the post-infer verifier's
// only net for one that did not.

require daslib/ast
require daslib/ast_boost
require daslib/ast_verify

class FlagSetter : AstVisitor {
    def override visitExprMakeArray(var expr : ExprMakeArray?) : ExpressionPtr {
        expr.constValues = true
        return expr
    }
}

[infer_macro]
class ConstValuesMacro : AstPassMacro {
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        if (mod == null || !empty(mod.name)) {
            return false
        }
        var v = new FlagSetter()
        make_visitor(*v) $(adapter) {
            visit_module(prog, adapter, mod)
        }
        unsafe {
            delete v
        }
        verify_module_after_infer(prog, mod)
        return false
    }
}
