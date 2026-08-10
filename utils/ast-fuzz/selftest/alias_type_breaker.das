options gen2

module alias_type_breaker shared

// Self-test fixture: builds two operands that share one TypeDecl in `expr.type`.
// Nothing walks that slot, so only the type tree recorded from preVisitExpression
// sees the alias. The subtree is verified on its own, so the module it is compiled
// from stays well formed - the report is the whole point, a broken module is not.

require daslib/ast
require daslib/ast_boost
require daslib/ast_verify

var private fired = false

[infer_macro]
class AliasTypeMacro : AstPassMacro {
    def override apply(_prog : ProgramPtr; mod : Module?) : bool {
        if (mod == null || !empty(mod.name) || fired) {
            return false
        }
        fired = true
        var shared_type = new TypeDecl(baseType = Type.tInt)
        var op = new ExprOp2(op := "+",
            left = new ExprConstInt(value = 1, _type = shared_type),
            right = new ExprConstInt(value = 2, _type = shared_type))
        op._type = new TypeDecl(baseType = Type.tInt)
        verify_expression(op)
        return false
    }
}
