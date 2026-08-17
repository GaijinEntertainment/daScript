options gen2

module null_entry_breaker shared

// Self-test fixture: nulls an entry in ExprLet.variables. ExprLet::visit
// dereferences each entry before any per-entry hook runs, so the verifier has to
// drop it in preVisit; the generator only ever builds real Variables, so this is
// the only guard on that check.

require daslib/ast
require daslib/ast_boost
require daslib/ast_verify

class Nuller : AstVisitor {
    done : bool = false
    def override visitExprLet(var expr : ExprLet?) : ExpressionPtr {
        if (!done && !empty(expr.variables)) {
            expr.variables[0] = null
            done = true
        }
        return expr
    }
}

[infer_macro]
class NullEntryMacro : AstPassMacro {
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        if (mod == null || !empty(mod.name)) {
            return false
        }
        var v = new Nuller()
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
