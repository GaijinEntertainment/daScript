options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_boot_restore shared public

require dasllama/dasllama_lint public
require daslib/ast
require daslib/ast_boost
require daslib/templates_boost

//! `[init, boot_restore]` on an empty-body function: the body gains, for every global of the module initialized `= @@fn`,
//! `if (g == null) { g = @@fn }` - a serialized exe restores function globals as null, and a registrar that ran first wins.
//! The annotation's `apply` runs as the parser reaches the function, so it sees the globals declared above it alone; the
//! `lint` pass, over the whole module, refuses a `= @@fn` global the guards missed - the function is the module's last declaration
[function_macro(name = "boot_restore")]
class private BootRestoreMacro : AstFunctionAnnotation {
    guarded : table<string; table<string>>   //! per restore function (module::name), the globals its guards cover

    def override apply(var func : FunctionPtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        var blk = func.body as ExprBlock
        if (blk == null) {
            errors := "boot_restore: the restore function needs a block body"
            return false
        }
        var names : table<string>
        for_each_global(compiling_module()) $(g) {
            if (g.init != null && g.init is ExprAddr) {
                names |> insert("{g.name}")
                var guard = qmacro_expr(${ if ($i(g.name) == null) { $i(g.name) = $e(g.init); } })
                force_generated(guard, true)
                blk.list |> emplace_new(guard)
            }
        }
        guarded["{compiling_module().name}::{func.name}"] <- names   //! `func._module` is not attached at apply time; the lint pass reads it
        return true
    }

    def override lint(var func : FunctionPtr; var group : ModuleGroup; var args : AnnotationArgumentList; progArgs : AnnotationArgumentList; var errors : das_string) : bool {
        let key = "{func._module.name}::{func.name}"
        var ok = true
        for_each_global(func._module) $(g) {
            if (g.init != null && g.init is ExprAddr && !key_exists(guarded?[key] ?? default<table<string>>, "{g.name}")) {
                errors := "boot_restore: the function global `{g.name}` (line {g.at.line}) is declared below the restore function `{func.name}` (line {func.at.line}), where the annotation cannot see it - move the restore function below every `= @@fn` global"
                ok = false
            }
        }
        return ok
    }
}
