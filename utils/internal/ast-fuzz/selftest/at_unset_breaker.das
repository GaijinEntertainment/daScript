options gen2

module at_unset_breaker shared

// Self-test fixture: a deliberately buggy pass macro that clears `at`, the invariant every node is
// supposed to carry, so the post-infer verify has to guess a location. One mode per shape.

require daslib/ast
require daslib/ast_boost

enum private Mode {
    off
    one_node   // a single ExprOp2 loses its `at`, its operands keep theirs
    subtree    // the whole `a + b` subtree loses its `at`
    whole_body // nothing in the body keeps a location
    assume_in  // the body block and the ExprAssume lose theirs, the alias expression keeps its
}

class Breaker : AstVisitor {
    mode : Mode = Mode.off
    strip : bool = false

    def override preVisitFunction(var fun : FunctionPtr) : void {
        mode = Mode.off
        if (fun.name == "lost_one") {
            mode = Mode.one_node
        } elif (fun.name == "deep_add") {
            mode = Mode.subtree
        } elif (fun.name == "all_gone" || fun.name == "no_args") {
            mode = Mode.whole_body
        } elif (fun.name == "with_assume") {
            mode = Mode.assume_in
        }
        strip = false
    }

    def override preVisitExpression(var expr : ExpressionPtr) : void {
        if (mode == Mode.whole_body || (mode == Mode.subtree && strip)) {
            expr.at = LineInfo()
        }
    }

    def override preVisitExprBlock(var expr : ExprBlock?) : void {
        if (mode == Mode.assume_in && !expr.blockFlags.isClosure) {
            expr.at = LineInfo()
        }
    }

    def override preVisitExprAssume(var expr : ExprAssume?) : void {
        if (mode == Mode.assume_in) {
            expr.at = LineInfo()
        }
    }

    def override preVisitExprOp2(var _expr : ExprOp2?) : void {
        if (mode == Mode.subtree) {
            strip = true
        }
    }

    def override visitExprOp2(var expr : ExprOp2?) : ExpressionPtr {
        if (mode == Mode.one_node || (mode == Mode.subtree && strip)) {
            expr.at = LineInfo()
            strip = false
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
        return false
    }
}
