options gen2
module _hot_path_macro shared private

require daslib/ast_boost
require daslib/templates_boost

//! a block only the macro flags generated; its statement is its own
[call_macro(name="macro_logged_block")]
class private MacroLoggedBlock : AstCallMacro {
    def override visit(prog : ProgramPtr; mod : Module?;
                       var expr : ExprCallMacro?) : ExpressionPtr {
        var blk = qmacro_block() {
            print("macro internals\n")
        }
        blk.genFlags.generated = true
        return blk
    }
}

//! a `with` only the macro flags generated, around its own statement
[call_macro(name="macro_logged_with")]
class private MacroLoggedWith : AstCallMacro {
    def override visit(prog : ProgramPtr; mod : Module?;
                       var expr : ExprCallMacro?) : ExpressionPtr {
        var blk = qmacro_block() {
            with ($e(expr.arguments[0])) {
                print("macro with\n")
            }
        }
        var w = (blk as ExprBlock).list[0]
        w.genFlags.generated = true
        return w
    }
}
