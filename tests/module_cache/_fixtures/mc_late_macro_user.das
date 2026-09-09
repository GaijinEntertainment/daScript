options gen2
options indenting = 4

module mc_late_macro_user shared private

//! a call macro that issues a late require from inside its requirer's compile: the walk is
//! nested in a parse, so its modules come from source and never touch the host's stream

require daslib/ast_boost
require daslib/templates_boost
require daslib/cross_context

[call_macro(name = "late_probe_call")]
class LateProbeCall : AstCallMacro {
    def override visit(prog : ProgramPtr; mod : Module?; expr : ExprCallMacro?) : ExpressionPtr {
        var answer = 0
        let ctx = macro_context_of("%/tests/module_cache/_fixtures/mc_late_mod.das")
        if (ctx != null) {
            unsafe {
                invoke_in_context(ctx, "late_answer", addr(answer))
            }
        }
        return <- qmacro($v(answer))
    }
}
