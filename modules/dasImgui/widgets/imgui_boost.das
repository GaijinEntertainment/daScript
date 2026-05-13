options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module imgui_boost_v2 shared

require imgui public
require imgui/imgui_boost_runtime public
require daslib/ast
require daslib/ast_boost
require daslib/strings_boost
require daslib/templates_boost

//! `[widget]` function annotation.
//!
//! Mark `def myWidget(var state : MyState; <args>) : <ret>` with `[widget]`
//! to register it. The macro:
//!   1. Injects `widget_ident : string` between the state arg and the rest
//!      so the body can call `widget_finalize(widget_ident, ...)`.
//!   2. Injects `widget_prelude(widget_ident)` at the body's top (`PushID`).
//!   3. Registers a per-kind call_macro that intercepts user-side
//!      `myWidget(IDENT, ...)` calls, parses the dotted-suffix flag chain
//!      (`SAVE_BTN.PUBLIC` / `RPS.NOTLIVE` etc.), idempotently emits IDENT
//!      as a module-scope state global, and rewrites the call to
//!      `myWidget(IDENT, "IDENT", ...)`.
//!
//! Auto-emit is module-scope only (find_variable gate). UPPER_SNAKE_CASE
//! identifiers by convention (API_REWORK §4.5).

let private VALID_FLAGS : array<string> = ["PUBLIC", "PRIVATE", "NOTLIVE"]

// Named-arg sugar that flows into the auto-emitted state global's
// initializer instead of the widget render call. `init=` is a rename
// (sugar for `value`); `bounds=` matches the state field name verbatim.
// `range=` is reserved in daslang's grammar and unusable as a record
// name, hence the rename pair. Per API_REWORK §4.2 + §9.1.
def private init_arg_to_field(name : string) : string {
    return "value" if (name == "init")
    return "bounds" if (name == "bounds")
    return ""
}

[function_macro(name = "widget")]
class WidgetFunctionMacro : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
        return apply_widget_or_container(func, errors, false)
    }
}

//! `[container]` function annotation — block-arg sibling of `[widget]`.
//!
//! Mark `def myContainer(var state : MyState; <args>; blk : block)` with
//! `[container]` to register a Begin/End-shaped wrapper. The macro:
//!   1. Same widget_ident injection + auto-emit + dotted-flag handling as
//!      `[widget]`.
//!   2. Prepends `container_path_push(widget_ident)` to the body so leaf
//!      widgets rendered inside `invoke(blk)` see this container in
//!      `g_container_path`.
//!   3. Appends `container_path_pop()` to body.finalList so the path is
//!      popped even when the user body returns early.
//!
//! Begin/End semantics — which paint always vs only-on-true — are NOT
//! abstracted. Each container def authors its own Begin → invoke(blk) → End
//! sequence so the imgui-irregular pairing rules stay visible at the call
//! site (Begin/End and BeginChild/EndChild always-pair; everything else
//! End-only-on-true).

[function_macro(name = "container")]
class ContainerFunctionMacro : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
        if (empty(func.arguments) || func.arguments[length(func.arguments) - 1]._type == null
            || func.arguments[length(func.arguments) - 1]._type.baseType != Type.tBlock) {
            errors := "[container] requires the last parameter to be `blk : block`"
            return false
        }
        return apply_widget_or_container(func, errors, true)
    }
}

def private apply_widget_or_container(var func : FunctionPtr; var errors : das_string;
                                      is_container : bool) : bool {
    if (empty(func.arguments)) {
        errors := "[widget]/[container] requires at least one parameter (state struct)"
        return false
    }
    let kind = string(func.name)
    let stateArg = func.arguments[0]
    if (stateArg._type == null || stateArg._type.structType == null) {
        errors := "[widget]/[container] first parameter must be a struct (state)"
        return false
    }
    let stateTypeName = describe(stateArg._type)
    let stateStruct = stateArg._type.structType
    func.flags |= FunctionFlags.generated
    // Inject widget_ident : string at index 1 (between state and rest).
    // The user's body code references `widget_ident` literally when
    // calling widget_finalize / container_finalize — this param makes it in scope.
    var widthIdentVar = new Variable(at = func.at,
        name := "widget_ident",
        _type <- new TypeDecl(baseType = Type.tString, at = func.at))
    widthIdentVar.flags |= VariableFlags.generated
    func.arguments |> emplace(widthIdentVar, 1)
    // Wrap the body: prepend widget_prelude(widget_ident) and, for containers,
    // container_path_push(widget_ident). The container path push runs every
    // frame BEFORE the user body so `invoke(blk)` sees self in g_container_path
    // and any leaves rendered inside get hierarchical registry keys. The
    // matching pop lives in body.finalList (always-runs on normal return).
    // PopID lives in widget_finalize / container_finalize at the bottom.
    var fblk = new ExprBlock(at = func.body.at)
    fblk.list |> push(qmacro(widget_prelude($i("widget_ident"))))
    if (is_container) {
        fblk.list |> push(qmacro(container_path_push($i("widget_ident"))))
    }
    let oldBody = func.body as ExprBlock
    for (el in oldBody.list) {
        fblk.list |> push(clone_expression(el))
    }
    for (ef in oldBody.finalList) {
        fblk.finalList |> push(clone_expression(ef))
    }
    if (is_container) {
        fblk.finalList |> push(qmacro(container_path_pop()))
    }
    func.body = fblk
    // Register the per-kind call macro instance. The same WidgetCallMacro
    // class works for both widgets and containers: it rewrites `kind(IDENT, ...)`
    // to `kind(IDENT, "IDENT", ...)` and auto-emits the global. The body
    // wrapping (path push/pop) is the only macro-side difference between the
    // two annotations. Duplicate-name collisions trigger a C++-side panic in
    // add_call_macro.
    var inst = new WidgetCallMacro(
        kind_name = kind,
        state_type_name = stateTypeName,
        state_struct = stateStruct,
        render_fn_name = kind,
        is_container = is_container)
    // Stash the widget def's user-facing args (positions 2..N, after the
    // injected widget_ident at position 1). The indexed-form wrapper
    // generator mirrors this signature for each (kind, IDENT) pair so the
    // user's call site forwards args unchanged into the underlying widget.
    let userArgN = length(func.arguments) - 2
    if (userArgN > 0) {
        inst.render_fn_args |> reserve(userArgN)
    }
    for (i in range(2, length(func.arguments))) {
        inst.render_fn_args |> push(func.arguments[i])
    }
    // Stash the underlying FunctionPtr (long-lived; lives in the widget's
    // module). The indexed-form wrapper reads .arguments and .result at
    // visit time, sidestepping the lifetime issue we saw when caching a
    // cloned TypeDecl on the WidgetCallMacro instance (the clone aliased
    // to whatever the gc heap reused that slot for, giving the wrapper
    // wrong return types like `double`/`tuple<>`/`GroupState`).
    inst.render_fn = func
    compiling_module() |> add_call_macro(make_call_macro(kind, inst))
    return true
}

class WidgetCallMacro : AstCallMacro {
    kind_name : string
    state_type_name : string
    state_struct : Structure const?
    render_fn_name : string
    is_container : bool
    render_fn_args : array<VariablePtr>
    render_fn : FunctionPtr

    def override canVisitArgument(expr : ExprCallMacro?; argIndex : int) : bool {
        // The first argument is the IDENT (which doesn't yet exist as a
        // global); telling the typer not to pre-resolve it lets us see the
        // raw ExprVar in visit(), parse the dotted-flag chain, emit the
        // global, and rewrite. Other args resolve normally before we see them.
        return argIndex != 0
    }

    def override visit(prog : ProgramPtr; mod : Module?; var expr : ExprCallMacro?) : ExpressionPtr {
        if (empty(expr.arguments)) {
            macro_error(prog, expr.at, "{kind_name}(IDENT, ...): missing widget identifier")
            return <- default<ExpressionPtr>
        }
        // Walk the leading-arg AST to extract:
        //   IDENT                  → bareName = IDENT (single-global)
        //   IDENT.FLAG[.FLAG2...]  → bareName = IDENT, flags = [FLAG, FLAG2, ...]
        //   IDENT[expr]            → bareName = IDENT, indexExpr = expr (table form, §4.4)
        // Parse layout: outermost is optionally ExprAt (subscript); inside it
        // is an optional ExprField chain (dotted flags); the leaf is ExprVar.
        // Flags + subscript not allowed together (flags belong on the user's
        // declared table, not the indexed reference).
        var bareName : string
        var rawName : string
        var flagsReversed : array<string>
        var indexExpr : ExpressionPtr
        var cursor : Expression const? = expr.arguments[0]
        if (cursor is ExprAt) {
            let ea = cursor as ExprAt
            // ExprSafeAt (?[]) is a subclass of ExprAt in C++; reject explicitly.
            if (cursor.__rtti != "ExprAt") {
                macro_error(prog, expr.at, "{kind_name}(IDENT[k]): safe subscript ?[k] not supported on indexed widgets")
                return <- default<ExpressionPtr>
            }
            indexExpr = clone_expression(ea.index)
            cursor = ea.subexpr
        }
        while (cursor is ExprField) {
            let fld = cursor as ExprField
            flagsReversed |> push(string(fld.name))
            cursor = fld.value
        }
        if (cursor is ExprVar) {
            bareName = string((cursor as ExprVar).name)
        } else {
            macro_error(prog, expr.at, "{kind_name}(IDENT...): first argument must be an identifier, `IDENT.FLAG[.FLAG...]`, or `IDENT[key]`, got {describe(expr.arguments[0])}")
            return <- default<ExpressionPtr>
        }
        if (indexExpr != null && !empty(flagsReversed)) {
            macro_error(prog, expr.at, "{kind_name}({bareName}[k]): dotted flags (.PUBLIC/.NOTLIVE) not supported on indexed form — declare the table global with required annotations directly")
            return <- default<ExpressionPtr>
        }
        // Indexed (table-form) widget: distinct path from single-global —
        // table global is user-declared, wrapper helper is generated per
        // (kind, IDENT), no init=/bounds= sugar (use [init] for imperative
        // pre-population). Returns the wrapper-call rewrite.
        return <- emit_indexed_call(prog, mod, expr, bareName, indexExpr) if (indexExpr != null)
        // Reconstruct rawName for error messages (source order).
        rawName = build_string() $(var w) {
            w |> write(bareName)
            for (i in range(0, length(flagsReversed))) {
                w |> write(".")
                w |> write(flagsReversed[length(flagsReversed) - 1 - i])
            }
        }
        if (empty(bareName)) {
            macro_error(prog, expr.at, "{kind_name}(IDENT...): identifier must not be empty: '{rawName}'")
            return <- default<ExpressionPtr>
        }
        // Validate + record flags.
        var flagPublic = false
        var flagPrivate = false
        var flagNotLive = false
        for (token in flagsReversed) {
            if (empty(token)) {
                macro_error(prog, expr.at, "{kind_name}(IDENT...): empty flag in '{rawName}'")
                return <- default<ExpressionPtr>
            }
            var known = false
            for (vf in VALID_FLAGS) {
                if (token == vf) {
                    known = true
                }
            }
            if (!known) {
                macro_error(prog, expr.at, "{kind_name}(IDENT...): unknown flag '{token}' in '{rawName}'; valid: PUBLIC, PRIVATE, NOTLIVE")
                return <- default<ExpressionPtr>
            }
            if (token == "PUBLIC") {
                if (flagPublic) {
                    macro_error(prog, expr.at, "{kind_name}(IDENT...): duplicate flag 'PUBLIC' in '{rawName}'")
                    return <- default<ExpressionPtr>
                }
                flagPublic = true
            } elif (token == "PRIVATE") {
                if (flagPrivate) {
                    macro_error(prog, expr.at, "{kind_name}(IDENT...): duplicate flag 'PRIVATE' in '{rawName}'")
                    return <- default<ExpressionPtr>
                }
                flagPrivate = true
            } elif (token == "NOTLIVE") {
                if (flagNotLive) {
                    macro_error(prog, expr.at, "{kind_name}(IDENT...): duplicate flag 'NOTLIVE' in '{rawName}'")
                    return <- default<ExpressionPtr>
                }
                flagNotLive = true
            }
        }
        if (flagPublic && flagPrivate) {
            macro_error(prog, expr.at, "{kind_name}(IDENT...): cannot combine PUBLIC and PRIVATE in '{rawName}'")
            return <- default<ExpressionPtr>
        }
        // Extract init-mapped named-tuple args (e.g. `init=0.5f`, `range=(0,1)`)
        // BEFORE emit / destructure. These flow into the state global's
        // initializer; the call itself never sees them. Collected into
        // initFieldNames/Exprs (parallel arrays so insertion order is
        // preserved — matches source order at the call site).
        var initFieldNames : array<string>
        var initFieldExprs : array<ExpressionPtr>
        // §4.8 per-call overrides — string literals (must be ExprConstString)
        // pulled out of the named-tuple args. `id=` wraps the render call in
        // PushID/PopID (ImGui hash override only). `path=` replaces bareName
        // as the widget_ident second arg (both telemetry path AND ImGui hash
        // change, since widget_prelude does PushID(widget_ident) inside).
        var idOverride : string
        var idOverridePresent = false
        var pathOverride : string
        var pathOverridePresent = false
        for (i in range(1, length(expr.arguments))) {
            let a = expr.arguments[i]
            if (a is ExprMakeTuple) {
                let mt = a as ExprMakeTuple
                if (length(mt.recordNames) == length(mt.values) && !empty(mt.recordNames)) {
                    for (j in range(length(mt.recordNames))) {
                        let recordName = string(mt.recordNames[j])
                        let fieldName = init_arg_to_field(recordName)
                        if (!empty(fieldName)) {
                            initFieldNames |> push(fieldName)
                            initFieldExprs |> push(clone_expression(mt.values[j]))
                        } elif (recordName == "id") {
                            if (!(mt.values[j] is ExprConstString)) {
                                macro_error(prog, mt.values[j].at,
                                            "{kind_name}({bareName}, id=...): `id=` must be a string literal")
                                return <- default<ExpressionPtr>
                            }
                            idOverride = string((mt.values[j] as ExprConstString).value)
                            idOverridePresent = true
                        } elif (recordName == "path") {
                            if (!(mt.values[j] is ExprConstString)) {
                                macro_error(prog, mt.values[j].at,
                                            "{kind_name}({bareName}, path=...): `path=` must be a string literal")
                                return <- default<ExpressionPtr>
                            }
                            pathOverride = string((mt.values[j] as ExprConstString).value)
                            pathOverridePresent = true
                        }
                    }
                }
            }
        }
        // Auto-emit gate: only emit if not already declared at module scope.
        // find_variable is module-scope ONLY — re-uses existing globals.
        // Type built from the captured Structure pointer (NOT Type.alias on
        // the qualified name — that doesn't resolve at the caller's scope
        // when the state struct lives in another module).
        // Initializer = `T()` so struct field defaults fire (without it the
        // global is bare zero-init and `field : T = default` declarations are
        // silently lost — see useInitializer flag, equivalent to gen2 `T()`).
        if (find_variable(mod, bareName) == null) {
            var typeRef <- new TypeDecl(baseType = Type.tStructure,
                                        structType = unsafe(reinterpret<Structure?>(state_struct)),
                                        at = expr.at)
            var initCall = new ExprMakeStruct(at = expr.at, makeType = clone_type(typeRef))
            initCall.makeStructFlags |= ExprMakeStructFlags.useInitializer
            // Seed named-init fields (init=/range= sugar) into the initializer.
            // `useInitializer=true` keeps struct defaults firing for un-seeded
            // fields; field LineInfo points at the call site so a wrong field
            // (e.g. `range=` on a button) reports against the user's arg.
            if (!empty(initFieldNames)) {
                var mkS = new MakeStruct(uninitialized)
                for (j in range(length(initFieldNames))) {
                    var mkF = new MakeFieldDecl(at = expr.at,
                                                name := initFieldNames[j],
                                                value = initFieldExprs[j])
                    emplace_new(*mkS, mkF)
                }
                emplace(initCall.structs, mkS)
            }
            var v = new Variable(at = expr.at,
                name := bareName,
                _type <- typeRef,
                init = initCall)
            v.flags |= VariableFlags.generated
            if (!flagPublic) {
                v.flags |= VariableFlags.private_variable
            }
            if (!flagNotLive) {
                v.annotation |> add_annotation_argument("live", true)
            }
            mod |> add_variable(v)
            // Phase 1: append a register_widget call into the per-module
            // shared [init] function. setup_call_list creates the function
            // on first call and returns its body block on subsequent ones,
            // so all widgets in this module funnel into a single init
            // function. Pattern from daslib/decs_boost.das:585.
            //
            // First widget in this module: prepend a clear_module_widgets
            // call so reload re-fires the init and replaces stale entries
            // (including removed widgets and stale captured lambdas) with
            // fresh ones. Other modules' entries stay untouched.
            var reg <- setup_call_list("register`imgui`widgets", expr.at, true, true)
            let kindLit = kind_name
            let identLit = bareName
            let modLit = string(mod.name)
            if (empty(reg.list)) {
                reg.list |> push(qmacro(
                    imgui_boost_runtime::clear_module_widgets($v(modLit))
                ))
            }
            reg.list |> push(qmacro(
                imgui_boost_runtime::register_widget(
                    $v(modLit),
                    $v(identLit),
                    $v(kindLit),
                    unsafe(addr($i(identLit))),
                    @() : JsonValue? {
                        return JV($i(identLit))
                    }
                )
            ))
        }
        // Rewrite to `<kind>(IDENT, "IDENT", args...)`. The function lives
        // in imgui_widgets_builtin (or user module). After macro expansion
        // the typer resolves the call against the (macro-expanded) signature
        // `def kind(var state : T; widget_ident : string; ...)`.
        //
        // Named-tuple args `(name = val, ...)` carry the call's named-arg
        // ergonomic; daslang's square-bracket `[name = val]` form builds an
        // ExprNamedCall and bypasses call_macro dispatch entirely. Destructure
        // named tuples into positional args (in source order — matches the
        // user-facing parameter order).
        var widgetIdent = bareName
        if (pathOverridePresent) {
            widgetIdent = pathOverride
        }
        var newCall = new ExprCall(at = expr.at, name := render_fn_name)
        newCall.arguments |> push(new ExprVar(at = expr.at, name := bareName))
        newCall.arguments |> push(new ExprConstString(at = expr.at, value := widgetIdent))
        for (i in range(1, length(expr.arguments))) {
            let a = expr.arguments[i]
            if (a is ExprMakeTuple) {
                let mt = a as ExprMakeTuple
                if (length(mt.recordNames) == length(mt.values) && !empty(mt.recordNames)) {
                    for (j in range(length(mt.recordNames))) {
                        let recordName = string(mt.recordNames[j])
                        // Skip init-mapped names (consumed by the global's
                        // initializer above) and id=/path= (consumed at
                        // wrap/widget_ident-substitution time).
                        continue if (!empty(init_arg_to_field(recordName)) ||
                                     recordName == "id" ||
                                     recordName == "path")
                        newCall.arguments |> push(clone_expression(mt.values[j]))
                    }
                    continue
                }
            }
            newCall.arguments |> push(clone_expression(a))
        }
        if (idOverridePresent) {
            // Wrap in a void block: PushID(idLit); call(); PopID(). Because
            // the block evaluates to void, capturing the widget's return
            // value while id= is in play won't compile — that's intentional
            // for first cut (rare combo; users can use `with_id(idLit) { let
            // r = button(...) }` for the bool-return form). Same wrapping
            // shape mirrors `with_id` exactly.
            let idLit = idOverride
            return qmacro_block() {
                PushID($v(idLit))
                $e(newCall)
                PopID()
            }
        }
        return <- newCall
    }

    def private emit_indexed_call(prog : ProgramPtr; mod : Module?;
                                  var expr : ExprCallMacro?;
                                  bareName : string;
                                  var indexExpr : ExpressionPtr) : ExpressionPtr {
        //! Indexed (table-form) widget per API_REWORK §4.4. Validates a
        //! pre-declared `variable <bareName> : table<K; StateT>` global,
        //! emits a per-(kind, IDENT) wrapper function lazily, and rewrites
        //! the call site to dispatch through the wrapper. Each `[i]` /
        //! `[key]` resolves to a distinct telemetry path
        //! (`<container>/<bareName>[<k>]`) so multi-render is naturally
        //! distinguished. Init-arg sugar (`init=`/`bounds=`) is rejected on
        //! this path — pre-populate the table from an `[init]` function
        //! instead (V1 limitation, see plan §H).
        let tableVar = find_variable(mod, bareName)
        if (tableVar == null) {
            let msg = "{kind_name}({bareName}[k]): table global '{bareName}' is not declared at module scope. Declare it as:  variable private {bareName} : table<int; {state_type_name}>  (or table<string; ...> for string keys)."
            macro_error(prog, expr.at, msg)
            return <- default<ExpressionPtr>
        }
        let tabType = tableVar._type
        if (tabType == null || tabType.baseType != Type.tTable) {
            macro_error(prog, expr.at,
                "{kind_name}({bareName}[k]): '{bareName}' is not a table (got {describe(tabType)}). Expected table<K; {state_type_name}>.")
            return <- default<ExpressionPtr>
        }
        if (tabType.secondType == null || unsafe(reinterpret<Structure?>(tabType.secondType.structType)) != unsafe(reinterpret<Structure?>(state_struct))) {
            macro_error(prog, expr.at,
                "{kind_name}({bareName}[k]): table value type ({describe(tabType.secondType)}) does not match expected state {state_type_name}.")
            return <- default<ExpressionPtr>
        }
        // Reject init=/bounds= sugar — first-touch initialization for indexed
        // widgets is via [init] function (imperative), not call-site named args.
        for (i in range(1, length(expr.arguments))) {
            let a = expr.arguments[i]
            if (a is ExprMakeTuple) {
                let mt = a as ExprMakeTuple
                if (length(mt.recordNames) == length(mt.values) && !empty(mt.recordNames)) {
                    for (j in range(length(mt.recordNames))) {
                        let recordName = string(mt.recordNames[j])
                        if (!empty(init_arg_to_field(recordName))) {
                            let msg = "{kind_name}({bareName}[k]): named-init sugar ({recordName}=) not supported on indexed form. Pre-populate {bareName} in an [init] function (e.g. `{bareName}[i].bounds = (0,1); {bareName}[i].value = 0.5f`)."
                            macro_error(prog, expr.at, msg)
                            return <- default<ExpressionPtr>
                        }
                        if (recordName == "id" || recordName == "path") {
                            let msg = "{kind_name}({bareName}[k], {recordName}=...): per-call {recordName}= override not supported on indexed form (each [k] already gets its own ImGui hash and path key). Wrap the indexed call in `with_id(s)` for an extra scope segment instead."
                            macro_error(prog, expr.at, msg)
                            return <- default<ExpressionPtr>
                        }
                    }
                }
            }
        }
        let wrapperName = "$imgui$indexed${kind_name}${bareName}"
        let moduleName = string(mod.name)
        let bareNameLit = bareName
        let kindLit = kind_name
        let is_container_lit = is_container
        // Emit the wrapper helper once per (kind, IDENT). find_unique_function
        // probes the user module before add_function fires; re-running visit()
        // for the same wrapper (e.g. a second call site) is a no-op.
        if (find_unique_function(mod, wrapperName, true) == null) {
            var wrapperArgs : array<VariablePtr>
            for (a in render_fn_args) {
                wrapperArgs |> emplace_new <| clone_variable(a)
            }
            // Forward call: <kind>(<bareName>[k], bare, ...args by name)
            var callArgs <- array<ExpressionPtr>(
                qmacro($i(bareNameLit)[$i("k")]),
                qmacro($i("bare")))
            callArgs |> reserve(2 + length(render_fn_args))
            for (a in render_fn_args) {
                let argName = string(a.name)
                callArgs |> push <| qmacro($i(argName))
            }
            let keyType = clone_type(tabType.firstType)
            let retType = clone_type(render_fn.result)
            var bodyStmts : array<ExpressionPtr>
            // bare = "<bareName>[<k>]" — runtime interp builds the path component
            bodyStmts |> push <| qmacro_expr(${
                let bare = "{$v(bareNameLit)}[{k}]"
            })
            // Containers push their per-key path component so leaves see
            // self in g_container_path; pop runs at body's end via finalList.
            if (is_container_lit) {
                bodyStmts |> push <| qmacro_expr(${
                    imgui_boost_runtime::container_path_push(bare)
                })
            }
            // path = container-aware key (matches what widget_finalize computes)
            bodyStmts |> push <| qmacro_expr(${
                let path = imgui_boost_runtime::widget_path_key(bare)
            })
            // First-touch / post-reload register. Serializer captures the
            // key by value; table slot is re-resolved on every snapshot read
            // so any rehashing-induced address churn doesn't desync state.
            bodyStmts |> push <| qmacro_expr(${
                if (!imgui_boost_runtime::widget_registered(path)) {
                    unsafe {
                        let ser <- @ capture(= k) () : JsonValue ? {
                            return JV($i(bareNameLit)[k])
                        }
                        imgui_boost_runtime::register_widget(
                            $v(moduleName), path, $v(kindLit),
                            addr($i(bareNameLit)[k]), ser)
                    }
                }
            })
            // Final call. For containers the path pop runs after the inner
            // body via the trailing statement (no early-return).
            if (retType.baseType == Type.tVoid) {
                bodyStmts |> push <| qmacro_expr(${
                    $c(kindLit)($a(callArgs))
                })
                if (is_container_lit) {
                    bodyStmts |> push <| qmacro_expr(${
                        imgui_boost_runtime::container_path_pop()
                    })
                }
            } else {
                if (is_container_lit) {
                    // Bind result, pop path, return.
                    bodyStmts |> push <| qmacro_expr(${
                        let _indexed_result = $c(kindLit)($a(callArgs))
                    })
                    bodyStmts |> push <| qmacro_expr(${
                        imgui_boost_runtime::container_path_pop()
                    })
                    bodyStmts |> push <| qmacro_expr(${
                        return _indexed_result
                    })
                } else {
                    bodyStmts |> push <| qmacro_expr(${
                        return $c(kindLit)($a(callArgs))
                    })
                }
            }
            var fn = qmacro_function(wrapperName) $(k : $t(keyType); $a(wrapperArgs)) : $t(retType) {
                $b(bodyStmts)
            }
            fn.flags |= FunctionFlags.generated
            fn.flags |= FunctionFlags.privateFunction
            fn.body |> force_at(expr.at)
            if (!(mod |> add_function(fn))) {
                macro_error(prog, expr.at,
                    "{kind_name}({bareName}[k]): failed to add indexed wrapper function '{wrapperName}'")
                return <- default<ExpressionPtr>
            }
        }
        // Rewrite call: wrapperName(indexExpr, ...remaining args)
        var newCall = new ExprCall(at = expr.at, name := wrapperName)
        newCall.arguments |> push(indexExpr)
        for (i in range(1, length(expr.arguments))) {
            let a = expr.arguments[i]
            if (a is ExprMakeTuple) {
                let mt = a as ExprMakeTuple
                if (length(mt.recordNames) == length(mt.values) && !empty(mt.recordNames)) {
                    for (j in range(length(mt.recordNames))) {
                        newCall.arguments |> push(clone_expression(mt.values[j]))
                    }
                    continue
                }
            }
            newCall.arguments |> push(clone_expression(a))
        }
        return <- newCall
    }
}
