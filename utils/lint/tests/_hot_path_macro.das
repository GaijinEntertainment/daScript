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
