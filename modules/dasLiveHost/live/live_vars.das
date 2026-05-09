options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4
options strict_smart_pointers = true

module live_vars shared public

//! Auto-serialization of global variables (and struct fields) across live reloads.
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
//! ``@live`` also works on struct fields. A non-``@live`` global whose struct
//! has ``@live`` fields is preserved field-by-field; non-marked fields take
//! source defaults on reload::
//!
//!     struct SliderState {
//!         @live value : float    // preserved across reload
//!         @live bounds : tuple<float;float>
//!         pending_value : float  // resets to default on reload
//!     }
//!     var slider_state : SliderState
//!
//! Each ``@live`` target (whole variable or single field) is stored under its
//! own key. The serialized data includes a hash of the initialization
//! expression — if the default in code changes, the hash mismatches on load
//! and the stale value is discarded.

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
        mod |> for_each_global() $(g) {
            if (g.annotation |> find_arg("live") ?as tBool ?? false) {
                if (g._type.flags.constant) {
                    macro_error(prog, g.at, "@live cannot be used on constant (let) variables — they never change: {g.name}")
                }
            }
        }
        for_each_structure(mod) $(st) {
            for (f in st.fields) {
                if (f.annotation |> find_arg("live") ?as tBool ?? false) {
                    if (f._type.flags.constant) {
                        macro_error(prog, f.at, "@live cannot be used on constant fields: {st.name}.{f.name}")
                    }
                }
            }
        }
        return false
    }
}

struct private LiveVarTarget {
    name : string
    key : string
    init_hash : int64
}

struct private LiveFieldTarget {
    var_name : string
    field_name : string
    key : string
    init_hash : int64
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
        mod |> for_each_function("__before_reload_live_vars") $(fn) {
            found = true
        }
        if (found) {
            return false
        }
        // Each target is either a whole @live global or a single @live field
        // of a non-@live global; whole-struct preservation already covers all
        // fields, so field walks happen only when the global itself is not @live.
        var var_targets : array<LiveVarTarget>
        var field_targets : array<LiveFieldTarget>

        mod |> for_each_global() $(g) {
            if (g._type.flags.constant) {
                return  // skip let bindings
            }
            let varLive = g.annotation |> find_arg("live") ?as tBool ?? false
            if (varLive) {
                let initHash = g.init != null ? int64(hash(describe(g.init))) : int64(0)
                var_targets |> emplace(LiveVarTarget(
                    name = string(g.name),
                    key = "__live_vars_{mod.name}_{g.name}",
                    init_hash = initHash))
            } else {
                let st = g._type.structType
                if (st == null) {
                    return
                }
                // Per-field hash combines the global's initializer (if any) with the
                // field's own default. A change to either source-level expression
                // invalidates the persisted value for the field. Without the global
                // part, edits to `var s : S = S(x = 100)` wouldn't bust the cache
                // for `s.x` even though the user-visible source default changed.
                let gInitPart = g.init != null ? int64(hash(describe(g.init))) : int64(0)
                for (f in st.fields) {
                    if (f.annotation |> find_arg("live") ?as tBool ?? false) {
                        if (f._type.flags.constant) {
                            continue  // lint pass already errored
                        }
                        let fInitPart = f.init != null ? int64(hash(describe(f.init))) : int64(0)
                        field_targets |> emplace(LiveFieldTarget(
                            var_name = string(g.name),
                            field_name = string(f.name),
                            key = "__live_vars_{mod.name}_{g.name}__{f.name}",
                            init_hash = gInitPart ^ fInitPart))
                    }
                }
            }
        }
        if (empty(var_targets) && empty(field_targets)) {
            return false
        }
        // Ensure archive module is available
        var archiveModule : Module?
        program_for_each_module(prog) $(m) {
            if (m.name == "archive") {
                archiveModule = m
            }
        }
        if (archiveModule != null) {
            add_module_require(mod, archiveModule, false)
        }
        let totalTargets = length(var_targets) + length(field_targets)
        // Build save blocks (whole-var first, then per-field)
        var saveBlocks : array<ExpressionPtr>
        saveBlocks |> reserve(totalTargets)
        for (t in var_targets) {
            saveBlocks |> push <| qmacro_block() {
                var ser = new MemSerializer()
                var arch = Archive(reading = false, stream = ser)
                var init_hash = $v(t.init_hash)
                arch |> _::serialize(init_hash)
                arch |> _::serialize($i(t.name))
                let data <- ser->extractData()
                live_store_bytes($v(t.key), data)
                unsafe { delete ser; }
            }
        }
        for (t in field_targets) {
            saveBlocks |> push <| qmacro_block() {
                var ser = new MemSerializer()
                var arch = Archive(reading = false, stream = ser)
                var init_hash = $v(t.init_hash)
                arch |> _::serialize(init_hash)
                arch |> _::serialize($i(t.var_name).$f(t.field_name))
                let data <- ser->extractData()
                live_store_bytes($v(t.key), data)
                unsafe { delete ser; }
            }
        }
        var saveFn = qmacro_function("__before_reload_live_vars") $() {
            $b(saveBlocks)
        }
        saveFn.flags |= FunctionFlags.exports | FunctionFlags.generated
        compiling_module() |> add_function(saveFn)
        // Build load blocks (whole-var first, then per-field)
        var loadBlocks : array<ExpressionPtr>
        loadBlocks |> reserve(totalTargets)
        for (t in var_targets) {
            loadBlocks |> push <| qmacro_block() {
                var data : array<uint8>
                if (live_load_bytes($v(t.key), data) && length(data) > 0) {
                    var ser = new MemSerializer(data)
                    var arch = Archive(reading = true, stream = ser)
                    var stored_hash : int64
                    arch |> _::serialize(stored_hash)
                    if (stored_hash == $v(t.init_hash)) {
                        arch |> _::serialize($i(t.name))
                    }
                    unsafe { delete ser; }
                }
            }
        }
        for (t in field_targets) {
            loadBlocks |> push <| qmacro_block() {
                var data : array<uint8>
                if (live_load_bytes($v(t.key), data) && length(data) > 0) {
                    var ser = new MemSerializer(data)
                    var arch = Archive(reading = true, stream = ser)
                    var stored_hash : int64
                    arch |> _::serialize(stored_hash)
                    if (stored_hash == $v(t.init_hash)) {
                        arch |> _::serialize($i(t.var_name).$f(t.field_name))
                    }
                    unsafe { delete ser; }
                }
            }
        }
        var loadFn = qmacro_function("__after_reload_live_vars") $() {
            $b(loadBlocks)
        }
        loadFn.flags |= FunctionFlags.exports | FunctionFlags.generated
        compiling_module() |> add_function(loadFn)
        return true
    }
}
