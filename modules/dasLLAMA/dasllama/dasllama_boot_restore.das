options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_boot_restore shared public

require dasllama/dasllama_lint public
require daslib/ast
require daslib/ast_boost
require daslib/templates_boost

//! `[init, boot_restore]` on an empty-body function: the body gains, for every global of the module initialized `= @@fn`,
//! `if (g == null) { g = @@fn }` - a serialized exe restores function globals as null, and a registrar that ran first wins
[function_macro(name = "boot_restore")]
class private BootRestoreMacro : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        var blk = func.body as ExprBlock
        if (blk == null) {
            errors := "boot_restore: the restore function needs a block body"
            return false
        }
        for_each_global(compiling_module()) $(g) {
            if (g.init != null && g.init is ExprAddr) {
                var guard = qmacro_expr(${ if ($i(g.name) == null) { $i(g.name) = $e(g.init); } })
                force_generated(guard, true)
                blk.list |> emplace_new(guard)
            }
        }
        return true
    }
}
