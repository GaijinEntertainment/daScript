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
//! `[seat_reset]` on an empty-body function: the body gains `g = @@fn` for the same globals, unguarded - the module's seats back
//! to their declared defaults, for the GPU home switch that installs the other home's over them.
//! Either annotation's `apply` runs as the parser reaches the function, so it sees the globals declared above it alone; the
//! `lint` pass, over the whole module, refuses a `= @@fn` global the body missed - the function is the module's last declaration
class private FnGlobalWalkMacro : AstFunctionAnnotation {
    covered : table<string; table<string>>   //! per generated function (module::name), the globals its body covers
    guarded : bool   //! `if (g == null)` around each assignment (boot_restore), or the bare assignment (seat_reset)
    tag : string
    noun : string   //! the generated function's name in the refusal: "the restore function" / "the reset function"

    def override apply(var func : FunctionPtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        var blk = func.body as ExprBlock
        if (blk == null) {
            errors := "{tag}: the function needs a block body"
            return false
        }
        var names : table<string>
        for_each_global(compiling_module()) $(g) {
            if (g.init != null && g.init is ExprAddr) {
                names |> insert("{g.name}")
                if (guarded) {
                    var guard = qmacro_expr(${ if ($i(g.name) == null) { $i(g.name) = $e(g.init); } })
                    force_generated(guard, true)
                    blk.list |> emplace_new(guard)
                } else {
                    var set_back = qmacro_expr(${ $i(g.name) = $e(g.init); })
                    force_generated(set_back, true)
                    blk.list |> emplace_new(set_back)
                }
            }
        }
        covered["{compiling_module().name}::{func.name}"] <- names   //! `func._module` is not attached at apply time; the lint pass reads it
        return true
    }

    def override lint(var func : FunctionPtr; var group : ModuleGroup; var args : AnnotationArgumentList; progArgs : AnnotationArgumentList; var errors : das_string) : bool {
        let key = "{func._module.name}::{func.name}"
        var ok = true
        for_each_global(func._module) $(g) {
            if (g.init != null && g.init is ExprAddr && !key_exists(covered?[key] ?? default<table<string>>, "{g.name}")) {
                errors := "{tag}: the function global `{g.name}` (line {g.at.line}) is declared below {noun} `{func.name}` (line {func.at.line}), where the annotation cannot see it - move {noun} below every `= @@fn` global"
                ok = false
            }
        }
        return ok
    }
}

[function_macro(name = "boot_restore")]
class private BootRestoreMacro : FnGlobalWalkMacro {
    def BootRestoreMacro {
        guarded = true
        tag = "boot_restore"
        noun = "the restore function"
    }
}

[function_macro(name = "seat_reset")]
class private SeatResetMacro : FnGlobalWalkMacro {
    def SeatResetMacro {
        guarded = false
        tag = "seat_reset"
        noun = "the reset function"
    }
}
