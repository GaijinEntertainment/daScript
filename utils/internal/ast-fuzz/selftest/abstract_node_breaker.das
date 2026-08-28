options gen2

module abstract_node_breaker shared

// Self-test fixture: puts a bare ExprConst - the base a concrete constant derives from,
// which never overrides visit - into a body. Any walk that reaches it asserts inside
// Expression::visit, so the check has to find it without descending.

require daslib/ast
require daslib/ast_boost
require daslib/ast_verify

[infer_macro]
class AbstractNodeMacro : AstPassMacro {
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        if (mod == null || !empty(mod.name)) {
            return false
        }
        for_each_function(mod, "main") $(var fn) {
            if (fn.body == null || !(fn.body is ExprBlock)) {
                return
            }
            var blk = fn.body as ExprBlock
            var node = new ExprConst(at = fn.at)
            blk.list |> emplace(node)
        }
        verify_module(prog, mod)
        return false
    }
}
