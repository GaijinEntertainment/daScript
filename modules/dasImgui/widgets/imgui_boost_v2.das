options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module imgui_boost_v2 shared

require imgui public
require imgui/imgui_lint public
require imgui/imgui_boost_runtime public
require daslib/ast
require daslib/ast_boost
require daslib/strings_boost
require daslib/templates_boost

// `[widget]` / `[container]` function annotations. Architectural detail
// (widget_ident injection, dotted-flag parsing, auto-emit of state
// globals) lives in handmade/module-imgui_boost_v2.rst and API_REWORK §4.5.

let private VALID_FLAGS : array<string> = ["PUBLIC", "PRIVATE", "NOTLIVE"]

// Named-arg sugar flowing into the auto-emitted state global's initializer,
// not the render call. `init=` renames to `value`; `bounds=` is verbatim;
// `range=` is a reserved grammar word, hence the rename. API_REWORK §4.2 + §9.1.
def private init_arg_to_field(name : string) : string {
    return "value" if (name == "init")
    return "bounds" if (name == "bounds")
    return ""
}

[function_macro(name = "widget")]
class WidgetFunctionMacro : AstFunctionAnnotation {
    //! ``[widget]`` annotation — registers ``def fn(var state : ST, ...)`` as a stateful widget. Auto-emits the state global, injects ``widget_ident``, and installs a per-kind call_macro for dotted-flag parsing.
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
        let fwd = args |> find_arg("forward_argument") ?as tBool ?? false
        let verbatim = args |> find_arg("verbatim_path") ?as tBool ?? false
        return apply_widget_or_container(func, errors, false, fwd, verbatim)
    }
}

[function_macro(name = "container")]
class ContainerFunctionMacro : AstFunctionAnnotation {
    //! ``[container]`` annotation — block-arg sibling of ``[widget]``; same identifier auto-emit, plus ``container_path_push``/``pop`` around ``invoke(blk)`` so leaf telemetry paths nest correctly.
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
        if (empty(func.arguments) || func.arguments[length(func.arguments) - 1]._type == null
            || func.arguments[length(func.arguments) - 1]._type.baseType != Type.tBlock) {
            errors := "[container] requires the last parameter to be `blk : block`"
            return false
        }
        let fwd = args |> find_arg("forward_argument") ?as tBool ?? false
        let verbatim = args |> find_arg("verbatim_path") ?as tBool ?? false
        return apply_widget_or_container(func, errors, true, fwd, verbatim)
    }
}

// `[drawlist_prim]` mirrors `[widget]` but skips state-global synthesis: it
// injects `widget_ident : string` at position 0, and DrawlistPrimCallMacro
// prepends a synthesized `<mod>:<line>:<col>` path key at each call site.

class DrawlistPrimCallMacro : AstCallMacro {
    kind_name : string
    target_mod : string  // module that owns the [drawlist_prim] function; qualifies the emit to prevent same-name collisions

    def override visit(prog : ProgramPtr; mod : Module?;
                       var expr : ExprCallMacro?) : ExpressionPtr {
        let modName = mod == null ? "anon" : string(mod.name)
        let path_key = "{modName}:{expr.at.line:d}:{expr.at.column:d}"
        var rewritten = new ExprCall(at = expr.at, name := "{target_mod}::{kind_name}")
        var pathArg = new ExprConstString(at = expr.at, value := path_key)
        rewritten.arguments |> push(pathArg)
        for (a in expr.arguments) {
            rewritten.arguments |> push(clone_expression(a))
        }
        return <- rewritten
    }
}

[function_macro(name = "drawlist_prim")]
class DrawlistPrimFunctionMacro : AstFunctionAnnotation {
    //! ``[drawlist_prim]`` — registers ``def fn(var dl, ...)`` as a drawlist
    //! primitive. Injects ``widget_ident`` at position 0 and a per-kind
    //! call_macro that prepends a synthesized ``<mod>:<line>:<col>`` path key.
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
        if (empty(func.arguments)) {
            errors := "[drawlist_prim] requires at least one parameter (the drawlist pointer)"
            return false
        }
        let kind = string(func.name)
        func.flags.generated = true
        var widthIdentVar = new Variable(at = func.at,
            name := "widget_ident",
            _type <- new TypeDecl(baseType = Type.tString, at = func.at))
        widthIdentVar.flags.generated = true
        func.arguments |> emplace(widthIdentVar, 0)
        // Capture the owning module name (compiling_module() — func._module is
        // not yet set at apply time). Used to qualify the call_macro's emit so
        // a same-named function in another module can't be picked up.
        let cmod = compiling_module()
        var inst = new DrawlistPrimCallMacro(kind_name = kind,
                                             target_mod = string(cmod.name))
        cmod |> add_call_macro(make_call_macro(kind, inst))
        return true
    }
}

def private apply_widget_or_container(var func : FunctionPtr; var errors : das_string;
                                      is_container : bool; forward_argument : bool;
                                      forward_verbatim : bool) : bool {
    if (empty(func.arguments)) {
        errors := "[widget]/[container] requires at least one parameter (state struct)"
        return false
    }
    let kind = string(func.name)
    // forward_argument: arg0 is a forwarded value (entity id), not a state struct — no state global; path key synthesized at runtime in visit().
    var stateTypeName = ""
    var stateStruct : Structure const? = null
    if (!forward_argument) {
        let stateArg = func.arguments[0]
        if (stateArg._type == null || stateArg._type.structType == null) {
            errors := "[widget]/[container] first parameter must be a struct (state)"
            return false
        }
        stateTypeName = describe(stateArg._type)
        stateStruct = stateArg._type.structType
    }
    func.flags.generated = true
    // Inject widget_ident : string — index 1 normally, index 0 for verbatim forward (arg0 IS the path string, no separate forwarded param).
    let identIndex = forward_verbatim ? 0 : 1
    var widthIdentVar = new Variable(at = func.at,
        name := "widget_ident",
        _type <- new TypeDecl(baseType = Type.tString, at = func.at))
    widthIdentVar.flags.generated = true
    func.arguments |> emplace(widthIdentVar, identIndex)
    // Prepend widget_prelude(widget_ident); for containers also container_path_push BEFORE the user body so invoke(blk) leaves nest under it (matching pop in finalList; PopID lives in widget_finalize / container_finalize).
    var fblk = new ExprBlock(at = func.body.at)
    fblk.list |> push(qmacro(imgui_boost_runtime::widget_prelude($i("widget_ident"))))
    if (is_container) {
        fblk.list |> push(qmacro(imgui_boost_runtime::container_path_push($i("widget_ident"))))
    }
    let oldBody = func.body as ExprBlock
    for (el in oldBody.list) {
        fblk.list |> push(clone_expression(el))
    }
    for (ef in oldBody.finalList) {
        fblk.finalList |> push(clone_expression(ef))
    }
    if (is_container) {
        fblk.finalList |> push(qmacro(imgui_boost_runtime::container_path_pop()))
    }
    func.body = fblk
    // Register the per-kind call macro (one WidgetCallMacro serves both widget and container). Duplicate-name collisions trigger a C++-side panic in add_call_macro.
    var inst = new WidgetCallMacro(
        kind_name = kind,
        state_type_name = stateTypeName,
        state_struct = stateStruct,
        render_fn_name = kind,
        is_container = is_container,
        forward_argument = forward_argument,
        forward_verbatim = forward_verbatim)
    // Stash the widget's user-facing args (positions 2..N, after widget_ident) so the indexed-form wrapper mirrors the signature per (kind, IDENT).
    let userArgN = length(func.arguments) - 2
    if (userArgN > 0) {
        inst.render_fn_args |> reserve(userArgN)
    }
    for (i in range(2, length(func.arguments))) {
        inst.render_fn_args |> push(func.arguments[i])
    }
    // Stash the long-lived FunctionPtr (read .arguments/.result at visit) — caching a cloned TypeDecl here gc-aliased and gave the wrapper wrong return types.
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
    forward_argument : bool
    forward_verbatim : bool
    render_fn_args : array<VariablePtr>
    render_fn : FunctionPtr

    def override canVisitArgument(expr : ExprCallMacro?; argIndex : int) : bool {
        // forward_argument: arg0 is a plain value forwarded into the render
        // call — resolve it (and everything else) normally before visit().
        return true if (forward_argument)
        // The first argument is the IDENT (not yet a global); tell the typer
        // not to pre-resolve it so visit() sees the raw ExprVar to parse the
        // dotted-flag chain and emit the global. Other args resolve normally.
        return argIndex != 0
    }

    // Auto-gen state-global identifier for no-ident call forms: `__widget_` +
    // line + column, a unique-within-module legal identifier (same-line calls
    // differ by column so they don't collide).
    def private synth_bare_name(at : LineInfo) : string {
        return "__widget_{at.line:d}_{at.column:d}"
    }

    // Auto-gen telemetry path key `<module>:<line>:<column>`, surfaced in
    // imgui_snapshot. Shifts on edits — use explicit idents when path
    // stability matters (interactive widgets driven by test commands).
    def private synth_path_key(mod : Module?; at : LineInfo) : string {
        let modName = mod == null ? "anon" : string(mod.name)
        return "{modName}:{at.line:d}:{at.column:d}"
    }

    // Positional-string sugar (`text("hello")`) is valid only when the signature
    // after widget_ident injection is exactly (state; widget_ident; text : string)
    // — length 3, last arg string (the narrative-text family).
    def private accepts_positional_string() : bool {
        return false if (render_fn == null || length(render_fn.arguments) != 3)
        let argT = render_fn.arguments[2]._type
        return false if (argT == null)
        return argT.baseType == Type.tString
    }

    // Bare no-arg form (`spacing()`, `same_line()`) is valid when every
    // user-facing arg (positions 2..N after state + widget_ident) has a
    // default initializer — `Variable.init != null`.
    def private all_user_args_defaulted() : bool {
        return false if (render_fn == null)
        for (i in range(2, length(render_fn.arguments))) {
            return false if (render_fn.arguments[i].init == null)
        }
        return true
    }

    def private emit_user_args(prog : ProgramPtr; expr : ExprCallMacro?;
                               var newCall : ExprCall?; firstUserArg : int;
                               renderArgStart : int) : bool {
        // Destructure user call args into render_fn decl order, filling skipped defaults. renderArgStart = first user param index (2 normally; 1 for verbatim forward).
        var namedFields : table<string; ExpressionPtr>
        var hasNamedTuple = false
        var maxNamedArgIdx = -1
        for (i in range(firstUserArg, length(expr.arguments))) {
            let a = expr.arguments[i]
            if (!(a is ExprMakeTuple)) continue
            let mt = a as ExprMakeTuple
            if (length(mt.recordNames) != length(mt.values) || empty(mt.recordNames)) continue
            hasNamedTuple = true
            for (j in range(length(mt.recordNames))) {
                let recordName = string(mt.recordNames[j])
                continue if (!empty(init_arg_to_field(recordName)) ||
                             recordName == "id" ||
                             recordName == "path")
                namedFields[recordName] = clone_expression(mt.values[j])
                // Find this name's argument index in render_fn (positions renderArgStart..N).
                for (k in range(renderArgStart, length(render_fn.arguments))) {
                    if (render_fn.arguments[k].name == recordName) {
                        if (k > maxNamedArgIdx) {
                            maxNamedArgIdx = k
                        }
                        break
                    }
                }
            }
        }
        if (is_container) {
            // Containers end in `blk : block`, and daslang won't auto-fill mid-position defaults around a trailing block — so emit a FULLY-explicit positional config list, then append the block.
            if (length(expr.arguments) <= firstUserArg) {
                macro_error(prog, expr.at, "{kind_name}(...): container requires a body block")
                return false
            }
            let blockIdx = length(expr.arguments) - 1
            let lastConfig = length(render_fn.arguments) - 2   // exclude trailing blk
            var positionalConfig : array<ExpressionPtr>
            for (i in range(firstUserArg, blockIdx)) {
                let a = expr.arguments[i]
                continue if (a is ExprMakeTuple)
                positionalConfig |> push(clone_expression(a))
            }
            var posCursor = 0
            for (k in range(renderArgStart, lastConfig + 1)) {
                let argName = string(render_fn.arguments[k].name)
                if (key_exists(namedFields, argName)) {
                    newCall.arguments |> push(namedFields[argName])
                } elif (posCursor < length(positionalConfig)) {
                    newCall.arguments |> push(positionalConfig[posCursor])
                    posCursor++
                } elif (render_fn.arguments[k].init != null) {
                    newCall.arguments |> push(clone_expression(render_fn.arguments[k].init))
                } else {
                    macro_error(prog, expr.at,
                        "{kind_name}(...): required argument '{argName}' has no default and was not provided")
                    return false
                }
            }
            // Leftover positional config (too-many-args) → append so the typer
            // surfaces the arity error, matching pre-autofill behavior.
            while (posCursor < length(positionalConfig)) {
                newCall.arguments |> push(positionalConfig[posCursor])
                posCursor++
            }
            newCall.arguments |> push(clone_expression(expr.arguments[blockIdx]))
            return true
        }
        if (hasNamedTuple && maxNamedArgIdx >= renderArgStart) {
            // Reorder: fill render_fn.arguments[renderArgStart..maxNamedArgIdx] in
            // order. Each slot: user-provided value if named, otherwise clone the
            // arg's default `init` expression.
            for (k in range(renderArgStart, maxNamedArgIdx + 1)) {
                let argName = string(render_fn.arguments[k].name)
                if (key_exists(namedFields, argName)) {
                    newCall.arguments |> push(namedFields[argName])
                } elif (render_fn.arguments[k].init != null) {
                    newCall.arguments |> push(clone_expression(render_fn.arguments[k].init))
                } else {
                    macro_error(prog, expr.at,
                        "{kind_name}(...): required argument '{argName}' has no default but was skipped by the user's named-tuple call")
                    return false
                }
            }
            // Any remaining expr.arguments that weren't tuples (positional
            // after the tuple) still need forwarding — uncommon but possible.
            for (i in range(firstUserArg, length(expr.arguments))) {
                let a = expr.arguments[i]
                if (a is ExprMakeTuple) continue
                newCall.arguments |> push(clone_expression(a))
            }
        } else {
            // No named-tuple form: keep the original positional forwarding.
            // Destructure any tuple (e.g. empty-tuple call forms) in source order.
            for (i in range(firstUserArg, length(expr.arguments))) {
                let a = expr.arguments[i]
                if (a is ExprMakeTuple) {
                    let mt = a as ExprMakeTuple
                    if (length(mt.recordNames) == length(mt.values) && !empty(mt.recordNames)) {
                        for (j in range(length(mt.recordNames))) {
                            let recordName = string(mt.recordNames[j])
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
        }
        return true
    }

    def private emit_forward_call(prog : ProgramPtr; expr : ExprCallMacro?) : ExpressionPtr {
        // forward_argument rewrite. Prefixed: kind(VAL,…)→render(VAL,"{kind}_{VAL}",…), user args at render index 2. Verbatim: kind(NAME,…)→render(NAME,…), arg0 is widget_ident, user args at index 1.
        if (empty(expr.arguments)) {
            macro_error(prog, expr.at, "{kind_name}(id, ...): missing forwarded first argument")
            return <- default<ExpressionPtr>
        }
        var newCall = new ExprCall(at = expr.at,
                                   name := "{render_fn._module.name}::{render_fn_name}")
        if (forward_verbatim) {
            newCall.arguments |> push(clone_expression(expr.arguments[0]))   // widget_ident = arg0 verbatim
            return <- default<ExpressionPtr> if (!emit_user_args(prog, expr, newCall, 1, 1))
            return <- newCall
        }
        newCall.arguments |> push(clone_expression(expr.arguments[0]))       // forwarded value (id)
        var ident = new ExprStringBuilder(at = expr.at)
        ident.elements |> push(new ExprConstString(at = expr.at, value := "{kind_name}_"))
        ident.elements |> push(clone_expression(expr.arguments[0]))
        newCall.arguments |> push(ident)                                     // widget_ident = "{kind}_{id}"
        return <- default<ExpressionPtr> if (!emit_user_args(prog, expr, newCall, 1, 2))
        return <- newCall
    }

    def override visit(prog : ProgramPtr; mod : Module?; var expr : ExprCallMacro?) : ExpressionPtr {
        return <- emit_forward_call(prog, expr) if (forward_argument)
        // Four call forms: 1 EXPLICIT kind(IDENT,(...)) drives the state global;
        // 2 AUTO-GEN kind((...)) and 3 POSITIONAL kind("str") synth the ident from
        // source location; 4 BARE kind() (all args defaulted). Read state ⇒ form 1.
        var bareName : string
        var widgetIdentOverride : string
        var rawName : string
        var flagsReversed : array<string>
        var indexExpr : ExpressionPtr
        var firstUserArg = 1
        if (empty(expr.arguments)) {
            // Form 4: bare no-arg — valid only when every user arg (positions
            // 2..N) has a default initializer (e.g. same_line / indent, but not
            // separator / spacing which have none).
            if (render_fn == null || !all_user_args_defaulted()) {
                macro_error(prog, expr.at, "{kind_name}(...): missing widget body — pass `(named=arg, …)` or, for narrative widgets, a single string literal")
                return <- default<ExpressionPtr>
            }
            bareName = synth_bare_name(expr.at)
            widgetIdentOverride = synth_path_key(mod, expr.at)
            firstUserArg = 0
        } else {
            let head = expr.arguments[0]
            if (head is ExprConstString || head is ExprStringBuilder) {
                // Form 3: positional-string sugar — a string literal or
                // interpolated string_builder, valid only when the signature is
                // exactly (state; widget_ident; text : string).
                if (!accepts_positional_string()) {
                    macro_error(prog, head.at, "{kind_name}: positional-string form `{kind_name}(\"…\")` is only valid for single-string narrative widgets (text/text_unformatted/text_wrapped/text_disabled/bullet_text/separator_text). Use the named-tuple form `{kind_name}((…))` instead.")
                    return <- default<ExpressionPtr>
                }
                bareName = synth_bare_name(expr.at)
                widgetIdentOverride = synth_path_key(mod, expr.at)
                firstUserArg = 0
                // Rewrite arg0 from the bare string/builder to a synthesized
                // named-tuple `(text = <head>)` so the destructure loop emits a
                // single positional `text` arg.
                var synth = new ExprMakeTuple(at = head.at)
                synth.recordNames.resize(1)
                synth.recordNames[0] := "text"
                synth.values |> push(clone_expression(head))
                expr.arguments[0] = synth
            } elif (head is ExprMakeTuple && (head as ExprMakeTuple) != null
                    && length((head as ExprMakeTuple).recordNames) == length((head as ExprMakeTuple).values)
                    && !empty((head as ExprMakeTuple).recordNames)) {
                // STYLE001: flag verbose `kind((field="lit"))` when positional
                // `kind("lit")` works — a lone string-literal field matching the
                // first user param, unless `_allow_imgui_legacy` is set on the file.
                let mtCheck = head as ExprMakeTuple
                if (length(expr.arguments) == 1 && length(mtCheck.recordNames) == 1
                        && (mtCheck.values[0] is ExprConstString || mtCheck.values[0] is ExprStringBuilder)
                        && accepts_positional_string()) {
                    let firstParamName = string(render_fn.arguments[2].name)
                    let tupleFieldName = string(mtCheck.recordNames[0])
                    let opted_out = prog._options |> find_arg("_allow_imgui_legacy") ?as tBool ?? false
                    if (firstParamName == tupleFieldName && !opted_out) {
                        macro_error(prog, expr.at,
                            "STYLE001: prefer positional form `{kind_name}(\"...\")` over verbose named-tuple `{kind_name}(({tupleFieldName}=...))`. Per-file opt-out: `options _allow_imgui_legacy = true`")
                        return <- default<ExpressionPtr>
                    }
                }
                // Form 2: auto-gen ident from source location. The first arg is a
                // named-tuple — user did not supply an ident.
                bareName = synth_bare_name(expr.at)
                widgetIdentOverride = synth_path_key(mod, expr.at)
                firstUserArg = 0
            }
        }
        // Form 1: explicit ident — leading-arg parser. Skipped when bareName
        // is already populated by the auto-gen / positional-string branches.
        if (empty(bareName)) {
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
                macro_error(prog, expr.at, "{kind_name}(...): first argument must be an identifier, named-tuple `(…)`, dotted-flag `IDENT.FLAG[.FLAG…]`, indexed `IDENT[key]`, or (for narrative widgets) a string literal; got {describe(expr.arguments[0])}")
                return <- default<ExpressionPtr>
            }
            if (indexExpr != null && !empty(flagsReversed)) {
                macro_error(prog, expr.at, "{kind_name}({bareName}[k]): dotted flags (.PUBLIC/.NOTLIVE) not supported on indexed form — declare the table global with required annotations directly")
                return <- default<ExpressionPtr>
            }
            // Indexed (table-form) widget: table global is user-declared, wrapper
            // helper generated per (kind, IDENT), no init=/bounds= sugar (use
            // [init] for imperative pre-population).
            return <- emit_indexed_call(prog, mod, expr, bareName, indexExpr) if (indexExpr != null)
            // Reconstruct rawName for error messages (source order).
            rawName = build_string() $(var w) {
                w |> write(bareName)
                for (i in range(0, length(flagsReversed))) {
                    w |> write(".")
                    w |> write(flagsReversed[length(flagsReversed) - 1 - i])
                }
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
        // Extract init-mapped named-tuple args (`init=`, `range=`) BEFORE emit —
        // these flow into the state global's initializer and the call never sees
        // them. Parallel initFieldNames/Exprs preserve source order.
        var initFieldNames : array<string>
        var initFieldExprs : array<ExpressionPtr>
        // §4.8 per-call overrides (string literals). `id=` wraps the render call
        // in PushID/PopID (ImGui hash only). `path=` replaces widget_ident —
        // changes both telemetry path AND ImGui hash (widget_prelude PushID's it).
        var idOverride : string
        var idOverridePresent = false
        var pathOverride : string
        var pathOverridePresent = false
        for (i in range(firstUserArg, length(expr.arguments))) {
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
        // Auto-emit gate: emit only if not already a module-scope global
        // (find_variable). Type from the captured Structure pointer, not a
        // qualified alias (won't resolve cross-module); init `T()` fires defaults.
        if (find_variable(mod, bareName) == null) {
            var typeRef <- new TypeDecl(baseType = Type.tStructure,
                                        structType = unsafe(reinterpret<Structure?>(state_struct)),
                                        at = expr.at)
            var initCall = new ExprMakeStruct(at = expr.at, makeType = clone_type(typeRef))
            initCall.makeStructFlags.useInitializer = true
            // Seed named-init fields (init=/range=) into the initializer.
            // useInitializer=true keeps defaults firing for un-seeded fields;
            // field LineInfo points at the call site so wrong-field errors are local.
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
            v.flags.generated = true
            if (!flagPublic) {
                v.flags.private_variable = true
            }
            if (!flagNotLive) {
                v.annotation |> add_annotation_argument("live", true)
            }
            mod |> add_variable(v)
            // Append register_widget into the per-module shared [init] via
            // setup_call_list (all widgets funnel into one init function). The
            // first widget prepends clear_module_widgets so reload drops stale entries.
            var reg <- setup_call_list("register`imgui`widgets", expr.at, true, true)
            let kindLit = kind_name
            let identLit = bareName
            let modLit = string(mod.name)
            if (empty(reg.list)) {
                reg.list |> push(qmacro(
                    imgui_boost_runtime::clear_module_widgets($v(modLit))
                ))
            }
            // Pass the global's address + TypeInfo directly to register_widget —
            // no per-widget getter/serializer thunks. Snapshot and dispatcher
            // paths use these via ``sprint_json_at`` / ``sscan_json_at``.
            reg.list |> push(qmacro(
                imgui_boost_runtime::register_widget(
                    $v(modLit),
                    $v(identLit),
                    $v(kindLit),
                    unsafe(addr($i(identLit))),
                    unsafe(reinterpret<TypeInfo const?>(typeinfo rtti_typeinfo($i(identLit))))
                )
            ))
        }
        // Rewrite to positional `<kind>(IDENT,"IDENT",args)`, REORDERing tuple
        // fields into decl order + FILLing skipped defaults (else mis-maps, e.g.
        // spacing→offset_from_start_x). widgetIdent: `path=` → auto-gen → bareName.
        var widgetIdent = bareName
        if (!empty(widgetIdentOverride)) {
            widgetIdent = widgetIdentOverride
        }
        if (pathOverridePresent) {
            widgetIdent = pathOverride
        }
        // Qualify with the widget impl's home module so a same-named function
        // in another loaded module can't shadow the dispatch. render_fn._module
        // is populated by the time visit() runs (post-binding).
        var newCall = new ExprCall(at = expr.at,
                                   name := "{render_fn._module.name}::{render_fn_name}")
        newCall.arguments |> push(new ExprVar(at = expr.at, name := bareName))
        newCall.arguments |> push(new ExprConstString(at = expr.at, value := widgetIdent))
        return <- default<ExpressionPtr> if (!emit_user_args(prog, expr, newCall, firstUserArg, 2))
        if (idOverridePresent) {
            // Wrap in a void block: PushID(idLit); call(); PopID(). Void ⇒ can't
            // capture the widget's return while id= is in play (rare; use
            // `with_id(idLit) { let r = button(...) }` for the bool-return form).
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
        // Qualify the inner call to the widget impl so the wrapper can't pick
        // up a same-named function in another module.
        let qualifiedKind = "{render_fn._module.name}::{kind_name}"
        // Key-type-aware emit: int/string keys use parallel register_widget / register_widget_str overloads; the addr-getter is typed per key and shared per (kind, IDENT).
        let keyIsString = tabType.firstType != null && tabType.firstType.baseType == Type.tString
        if (tabType.firstType == null ||
            (tabType.firstType.baseType != Type.tInt && tabType.firstType.baseType != Type.tString)) {
            macro_error(prog, expr.at,
                "{kind_name}({bareName}[k]): only `table<int; …>` and `table<string; …>` are supported for indexed widgets (got `{describe(tabType)}`).")
            return <- default<ExpressionPtr>
        }
        let indexedAddrFnName = "__widget_addr$indexed${kind_name}${bareName}"
        if (find_unique_function(mod, indexedAddrFnName, true) == null) {
            // Shared per-(kind, IDENT) addr-getter — re-resolves per call because table slots may be erased between frames (erased keys surface as null).
            var addrFn : FunctionPtr
            if (keyIsString) {
                addrFn = qmacro_function(indexedAddrFnName) $(k : string) : void? {
                    return null if (!key_exists($i(bareNameLit), k))
                    return unsafe(addr($i(bareNameLit)[k]))
                }
            } else {
                addrFn = qmacro_function(indexedAddrFnName) $(k : int) : void? {
                    return null if (!key_exists($i(bareNameLit), k))
                    return unsafe(addr($i(bareNameLit)[k]))
                }
            }
            addrFn.flags.generated = true
            addrFn.flags.privateFunction = true
            addrFn.body |> force_at(expr.at)
            mod |> add_function(addrFn)
        }
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
            // Do NOT container_path_push here: the called [container] does it with the same `bare`; a wrapper push would double-nest the path (`.../DYN_TAB[0]/DYN_TAB[0]`).
            var bodyStmts <- [
                qmacro_expr(${
                    let bare = "{$v(bareNameLit)}[{k}]"
                }),
                qmacro_expr(${
                    let path = imgui_boost_runtime::widget_path_key(bare)
                })]
            // First-touch / post-reload register. `ti` from rtti_typeinfo(IDENT[k]) is resolved at compile time (the runtime expression is never evaluated).
            if (keyIsString) {
                bodyStmts |> push <| qmacro_expr(${
                    if (!imgui_boost_runtime::widget_registered(path)) {
                        imgui_boost_runtime::register_widget_str(
                            $v(moduleName), path, $v(kindLit),
                            @@$c(indexedAddrFnName),
                            unsafe(reinterpret<TypeInfo const?>(typeinfo rtti_typeinfo($i(bareNameLit)[k]))),
                            k)
                    }
                })
            } else {
                bodyStmts |> push <| qmacro_expr(${
                    if (!imgui_boost_runtime::widget_registered(path)) {
                        imgui_boost_runtime::register_widget_indexed(
                            $v(moduleName), path, $v(kindLit),
                            @@$c(indexedAddrFnName),
                            unsafe(reinterpret<TypeInfo const?>(typeinfo rtti_typeinfo($i(bareNameLit)[k]))),
                            k)
                    }
                })
            }
            // Final call. The called [container]'s own finally block pops
            // its container_path_push, so no wrapper-side pop is needed.
            if (retType.baseType == Type.tVoid) {
                bodyStmts |> push <| qmacro_expr(${
                    $c(qualifiedKind)($a(callArgs))
                })
            } else {
                bodyStmts |> push <| qmacro_expr(${
                    return $c(qualifiedKind)($a(callArgs))
                })
            }
            var fn = qmacro_function(wrapperName) $(k : $t(keyType); $a(wrapperArgs)) : $t(retType) {
                $b(bodyStmts)
            }
            fn.flags.generated = true
            fn.flags.privateFunction = true
            fn.body |> force_at(expr.at)
            if (!(mod |> add_function(fn))) {
                macro_error(prog, expr.at,
                    "{kind_name}({bareName}[k]): failed to add indexed wrapper function '{wrapperName}'")
                return <- default<ExpressionPtr>
            }
        }
        // Rewrite call to wrapperName(indexExpr, …). `__::` constrains overload resolution to the user module where we just installed the wrapper.
        var newCall = new ExprCall(at = expr.at, name := "__::{wrapperName}")
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

// `[edit_widget]` — external-pointer sibling of `[widget]`: fn takes (ptr : T?,
// ...named...); no state struct / auto-emit / init= sugar. Requires a call-site
// `id="..."` literal; renderer captures `ptr` for live `*ptr` snapshot reads.

[function_macro(name = "edit_widget")]
class EditWidgetFunctionMacro : AstFunctionAnnotation {
    //! ``[edit_widget]`` annotation — registers an external-pointer widget. First parameter is the user-owned pointer (``T?``); macro injects ``widget_ident : string`` at index 1 and installs a call_macro that requires an explicit ``id="..."`` literal at the call site.
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
        return apply_edit_widget_or_container(func, errors, false)
    }
}

[function_macro(name = "edit_container")]
class EditContainerFunctionMacro : AstFunctionAnnotation {
    //! ``[edit_container]`` annotation — block-arg sibling of ``[edit_widget]``. First parameter is the user-owned pointer (``T?``); last parameter must be ``blk : block``. Macro injects ``widget_ident : string`` at index 1 and wraps the body with ``container_path_push``/``pop`` so leaves rendered inside ``invoke(blk)`` nest under the container's path key. Same ``id="..."`` literal requirement as ``[edit_widget]``.
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
        if (empty(func.arguments) || func.arguments[length(func.arguments) - 1]._type == null
            || func.arguments[length(func.arguments) - 1]._type.baseType != Type.tBlock) {
            errors := "[edit_container] requires the last parameter to be `blk : block`"
            return false
        }
        return apply_edit_widget_or_container(func, errors, true)
    }
}

def private apply_edit_widget_or_container(var func : FunctionPtr; var errors : das_string;
                                           is_container : bool) : bool {
    if (empty(func.arguments)) {
        errors := "[edit_widget]/[edit_container] requires at least one parameter (ptr)"
        return false
    }
    let ptrArg = func.arguments[0]
    if (ptrArg._type == null || ptrArg._type.baseType != Type.tPointer) {
        errors := "[edit_widget]/[edit_container] first parameter must be a pointer (e.g. `float?`, `int?`, `bool?`)"
        return false
    }
    let kind = string(func.name)
    func.flags.generated = true
    // Inject widget_ident : string at index 1 (between ptr and the rest), so the user body can reference it for widget_finalize / edit_finalize.
    var widthIdentVar = new Variable(at = func.at,
        name := "widget_ident",
        _type <- new TypeDecl(baseType = Type.tString, at = func.at))
    widthIdentVar.flags.generated = true
    func.arguments |> emplace(widthIdentVar, 1)
    // Prepend widget_prelude(widget_ident); for containers also container_path_push BEFORE the user body so invoke(blk) leaves nest under our ident (matching pop in finalList).
    var fblk = new ExprBlock(at = func.body.at)
    fblk.list |> push(qmacro(imgui_boost_runtime::widget_prelude($i("widget_ident"))))
    if (is_container) {
        fblk.list |> push(qmacro(imgui_boost_runtime::container_path_push($i("widget_ident"))))
    }
    let oldBody = func.body as ExprBlock
    for (el in oldBody.list) {
        fblk.list |> push(clone_expression(el))
    }
    for (ef in oldBody.finalList) {
        fblk.finalList |> push(clone_expression(ef))
    }
    if (is_container) {
        fblk.finalList |> push(qmacro(imgui_boost_runtime::container_path_pop()))
    }
    func.body = fblk
    var inst = new EditExternalCallMacro(
        kind_name = kind,
        render_fn = func)
    let userArgN = length(func.arguments) - 2
    if (userArgN > 0) {
        inst.render_fn_args |> reserve(userArgN)
    }
    for (i in range(2, length(func.arguments))) {
        inst.render_fn_args |> push(func.arguments[i])
    }
    compiling_module() |> add_call_macro(make_call_macro(kind, inst))
    return true
}

class EditExternalCallMacro : AstCallMacro {
    //! Call macro installed by ``[edit_widget]`` — recognizes ``edit_kind(ptr, (id="...", ...))``, requires an explicit ``id=`` string literal, destructures the rest of the named-tuple to positional args (source order), and rewrites to a direct renderer call.
    kind_name : string  //!< Name of the ``[edit_widget]``-decorated function; the rewritten call's target and macro_error subject.
    render_fn : FunctionPtr  //!< Underlying ``[edit_widget]`` function pointer; lives in the user module, read at visit time.
    render_fn_args : array<VariablePtr>  //!< User-facing args (positions 2..N after ``ptr`` + ``widget_ident``); cloned from ``render_fn`` to preserve names/defaults.

    def override visit(prog : ProgramPtr; mod : Module?; var expr : ExprCallMacro?) : ExpressionPtr {
        if (empty(expr.arguments)) {
            macro_error(prog, expr.at, "{kind_name}(ptr, (id=\"...\", ...)): missing pointer argument")
            return <- default<ExpressionPtr>
        }
        // Extract id= from named-tuple args (must be present; string literal).
        var idLit : string
        var idPresent = false
        for (i in range(1, length(expr.arguments))) {
            let a = expr.arguments[i]
            if (a is ExprMakeTuple) {
                let mt = a as ExprMakeTuple
                if (length(mt.recordNames) == length(mt.values) && !empty(mt.recordNames)) {
                    for (j in range(length(mt.recordNames))) {
                        if (mt.recordNames[j] == "id") {
                            if (!(mt.values[j] is ExprConstString)) {
                                macro_error(prog, mt.values[j].at,
                                            "{kind_name}(..., id=...): `id=` must be a string literal")
                                return <- default<ExpressionPtr>
                            }
                            idLit = string((mt.values[j] as ExprConstString).value)
                            idPresent = true
                        }
                    }
                }
            }
        }
        if (!idPresent) {
            macro_error(prog, expr.at,
                "{kind_name}(ptr, (id=\"...\", ...)): external-pointer widgets require an explicit `id=\"...\"` for the telemetry path. No auto-generation — pick a stable unique identifier.")
            return <- default<ExpressionPtr>
        }
        if (empty(idLit)) {
            macro_error(prog, expr.at, "{kind_name}: `id=` must not be empty")
            return <- default<ExpressionPtr>
        }
        // Rewrite to positional: kind_name(ptr, "<idLit>", ...named args destructured).
        // Qualified with the [edit_widget] impl's home module to dodge collisions.
        var newCall = new ExprCall(at = expr.at,
                                   name := "{render_fn._module.name}::{kind_name}")
        newCall.arguments |> push(clone_expression(expr.arguments[0]))
        newCall.arguments |> push(new ExprConstString(at = expr.at, value := idLit))
        for (i in range(1, length(expr.arguments))) {
            let a = expr.arguments[i]
            if (a is ExprMakeTuple) {
                let mt = a as ExprMakeTuple
                if (length(mt.recordNames) == length(mt.values) && !empty(mt.recordNames)) {
                    for (j in range(length(mt.recordNames))) {
                        continue if (mt.recordNames[j] == "id")
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

// ===== [widget_dispatch] — per-state-struct dispatcher registration =====

// Applied to `def <name>(var state : T; action : string; payload : string)`,
// keyed by T (the first param's struct type, NOT the function name) — one body
// covers every widget kind whose state is T. Emits a type-erased wrapper + init register.

[function_macro(name = "widget_dispatch")]
class WidgetDispatchFunctionMacro : AstFunctionAnnotation {
    //! ``[widget_dispatch]`` annotation — registers a per-state-struct dispatcher for HTTP set/click/open/close actions. Required signature: ``def <name>(var state : T; action : string; payload : string)``. Keyed by ``T`` (first parameter's struct type); the function name is irrelevant. One body per state struct covers every kind whose state is ``T``. Renames the user function to private ``__dispatch_body_<T>`` (avoids collision with same-named ``[widget]`` defs), emits a type-erased wrapper ``__dispatch_wrap_<T>``, and pushes an init call to ``register_widget_dispatch_for_type`` so ``dispatch_or_err`` resolves via ``meta.ti.hash`` at runtime.
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
        if (length(func.arguments) != 3) {
            errors := "[widget_dispatch] requires exactly 3 parameters: (var state : T; action : string; payload : string)"
            return false
        }
        let stateArg = func.arguments[0]
        if (stateArg._type == null || stateArg._type.structType == null) {
            errors := "[widget_dispatch] first parameter must be a struct (state)"
            return false
        }
        let actionArg = func.arguments[1]
        if (actionArg._type == null || actionArg._type.baseType != Type.tString) {
            errors := "[widget_dispatch] second parameter must be `action : string`"
            return false
        }
        let payloadArg = func.arguments[2]
        if (payloadArg._type == null || payloadArg._type.baseType != Type.tString) {
            errors := "[widget_dispatch] third parameter must be `payload : string`"
            return false
        }
        // Key the dispatcher by the state struct's name, not the function name
        // — collapses N kinds sharing a struct to one body. Re-applying for the
        // same struct is last-wins (setup_call_list dedup keeps init idempotent).
        let structName = string(stateArg._type.structType.name)
        func.name := "__dispatch_body_{structName}"
        func.flags.generated = true
        func.flags.privateFunction = true
        let mod = compiling_module()
        let wrapperName = "__dispatch_wrap_{structName}"
        let bodyName = "__dispatch_body_{structName}"
        if (find_unique_function(mod, wrapperName, true) == null) {
            // Type-erased wrapper: casts addr to ``T -const?`` and delegates to
            // the renamed user body. One per state struct, registered against
            // the struct's ``TypeInfo`` hash via the init block below.
            var wrapper = qmacro_function(wrapperName) $(state_addr : void?; action : string; payload : string) {
                unsafe {
                    var s & = *reinterpret<$t(stateArg._type) -const?>(state_addr)
                    $c(bodyName)(s, action, payload)
                }
            }
            wrapper.flags.generated = true
            wrapper.flags.privateFunction = true
            wrapper.body |> force_at(func.at)
            mod |> add_function(wrapper)
        }
        // Push init registration into the shared module init block. Pass the
        // state struct's TypeInfo* at registration time; the runtime reads
        // ``ti.hash`` and stores under that key in ``g_dispatchers``.
        var reg <- setup_call_list("register`imgui`widget_kinds", func.at, true, true)
        let wrapperLit = wrapperName
        var typeForTI = clone_type(stateArg._type)
        reg.list |> push(qmacro(
            imgui_boost_runtime::register_widget_dispatch_for_type(
                unsafe(reinterpret<TypeInfo const?>(typeinfo rtti_typeinfo(type<$t(typeForTI)>))),
                @@$c(wrapperLit))
        ))
        return true
    }
}
