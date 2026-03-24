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
//!
//! Each ``@live`` variable is stored independently under its own key.
//! The serialized data includes a hash of the initialization expression.
//! If the default value in code changes, the hash mismatches on load and
//! the old persisted value is discarded — no full reload needed.

require daslib/ast
require daslib/rtti
require daslib/ast_boost
require daslib/templates_boost
require daslib/archive

[lint_macro]
class LiveVarsLint : AstPassMacro {
    //! Lint pass that checks for @live on constant (let) variables.
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        if (mod.name == "live_vars") {
            return false
        }
        mod |> for_each_global() <| $(g) {
            if (g.annotation |> find_arg("live") ?as tBool ?? false) {
                if (g._type.flags.constant) {
                    macro_error(prog, g.at, "@live cannot be used on constant (let) variables — they never change: {g.name}")
                }
            }
        }
        return false
    }
}

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
        // Collect @live-tagged globals with their init-expression hashes
        var live_var_names : array<string>
        var live_var_keys : array<string>
        var live_var_hashes : array<int64>
        mod |> for_each_global() <| $(g) {
            if (g.annotation |> find_arg("live") ?as tBool ?? false) {
                if (!g._type.flags.constant) {
                    live_var_names |> push(string(g.name))
                    live_var_keys |> push("__live_vars_{mod.name}_{g.name}")
                    let initHash = g.init != null ? int64(hash(describe(g.init))) : int64(0)
                    live_var_hashes |> push(initHash)
                }
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
        // Build per-variable save blocks
        var inscope saveBlocks : array<ExpressionPtr>
        for (varName, varKey, varHash in live_var_names, live_var_keys, live_var_hashes) {
            saveBlocks |> emplace_new <| qmacro_block() {
                var ser = new MemSerializer()
                var arch = Archive(reading = false, stream = ser)
                var init_hash = $v(varHash)
                arch |> _::serialize(init_hash)
                arch |> _::serialize($i(varName))
                let data <- ser->extractData()
                live_store_bytes($v(varKey), data)
                unsafe { delete ser; }
            }
        }
        // Generate __before_reload_live_vars (save)
        var inscope saveFn <- qmacro_function("__before_reload_live_vars") <| $() {
            $b(saveBlocks)
        }
        saveFn.flags |= FunctionFlags.exports | FunctionFlags.generated
        compiling_module() |> add_function(saveFn)
        // Build per-variable load blocks
        var inscope loadBlocks : array<ExpressionPtr>
        for (varName, varKey, varHash in live_var_names, live_var_keys, live_var_hashes) {
            loadBlocks |> emplace_new <| qmacro_block() {
                var data : array<uint8>
                if (live_load_bytes($v(varKey), data) && length(data) > 0) {
                    var ser = new MemSerializer(data)
                    var arch = Archive(reading = true, stream = ser)
                    var stored_hash : int64
                    arch |> _::serialize(stored_hash)
                    if (stored_hash == $v(varHash)) {
                        arch |> _::serialize($i(varName))
                    }
                    unsafe { delete ser; }
                }
            }
        }
        // Generate __after_reload_live_vars (load)
        var inscope loadFn <- qmacro_function("__after_reload_live_vars") <| $() {
            $b(loadBlocks)
        }
        loadFn.flags |= FunctionFlags.exports | FunctionFlags.generated
        compiling_module() |> add_function(loadFn)
        return true
    }
}
