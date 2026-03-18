options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4
options strict_smart_pointers = true

module live_vars shared public

//! Auto-serialization of global variables across live reloads.
//!
//! Tag globals with ``@live`` to auto-generate ``[before_reload]`` and
//! ``[after_reload]`` functions that serialize them via Archive::
//!
//!     require live/live_vars
//!
//!     var @live paddle_x = 0.0
//!     var @live score = 0
//!     var @live game_state = GameState.menu

require ast
require rtti
require daslib/ast_boost
require daslib/templates_boost

[infer_macro]
class LiveVarsPass : AstPassMacro {
    //! Infer pass macro that generates serialization for @live globals.
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        // Skip self
        if (mod.name == "live_vars") {
            return false
        }
        // Idempotency: skip if already generated
        var found = false
        mod |> for_each_function("__before_reload_live_vars") <| $(fn) {
            found = true
        }
        if (found) {
            return false
        }
        // Collect @live-tagged globals
        var live_var_names : array<string>
        mod |> for_each_global() <| $(g) {
            if (g.annotation |> find_arg("live") ?as tBool ?? false) {
                live_var_names |> push(string(g.name))
            }
        }
        if (length(live_var_names) == 0) {
            return false
        }
        // Ensure archive module is available
        var archiveModule : Module?
        program_for_each_module(prog) <| $(m) {
            if (m.name == "archive") {
                archiveModule = m
            }
        }
        if (archiveModule != null) {
            add_module_require(mod, archiveModule, false)
        }
        // Store key is module-specific
        let storeKey = "__live_vars_{mod.name}"
        // Build serialize call list
        var inscope serList : array<ExpressionPtr>
        for (varName in live_var_names) {
            serList |> emplace_new <| qmacro(arch |> _::serialize($i(varName)))
        }
        // Generate __before_reload_live_vars (save)
        var inscope saveFn <- qmacro_function("__before_reload_live_vars") <| $() {
            var ser = new MemSerializer()
            var arch = Archive(reading = false, stream = ser)
            $b(serList)
            var data <- ser->extractData()
            live_store_bytes($v(storeKey), data)
            unsafe { delete ser; }
        }
        saveFn.flags |= FunctionFlags.exports | FunctionFlags.generated
        compiling_module() |> add_function(saveFn)
        // Build deserialize call list (same calls — Archive is bidirectional)
        var inscope desList : array<ExpressionPtr>
        for (varName in live_var_names) {
            desList |> emplace_new <| qmacro(arch |> _::serialize($i(varName)))
        }
        // Generate __after_reload_live_vars (load)
        var inscope loadFn <- qmacro_function("__after_reload_live_vars") <| $() {
            var data : array<uint8>
            if (live_load_bytes($v(storeKey), data) && length(data) > 0) {
                try {
                    var ser = new MemSerializer(data)
                    var arch = Archive(reading = true, stream = ser)
                    $b(desList)
                    unsafe { delete ser; }
                } recover {
                    print("live_vars: failed to restore state (format changed?)\n")
                }
            }
        }
        loadFn.flags |= FunctionFlags.exports | FunctionFlags.generated
        compiling_module() |> add_function(loadFn)
        return true
    }
}
