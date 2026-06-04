options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options _comment_hygiene = true

module linq_fold_json shared public

//! linq_fold JSON source adapter: ``JsonAdapter`` + ``extract_json_source``. Lets ``_fold`` over a
//! ``from_json(jv, type<Row>)`` chain emit an inlined ``for (e in jv.value as _array)`` walk that reads
//! only the fields the chain touches (via ``read_json_field``), instead of materializing every element
//! with ``from_JV``. In-tree companion to ``daslib/linq_fold`` (required unconditionally — JSON is always
//! present). Emits ``from_json`` / ``from_JV`` / ``read_json_field`` BY NAME (resolved at the user's
//! splice site, like linq_fold_decs emits ``for_each_archetype``), so this module pulls in no json
//! dependency. See daslib/linq_fold.md.

require daslib/ast_boost
require daslib/ast_match
require daslib/templates_boost
require strings
require daslib/linq_fold_common public

// Field-pruning row-usage scanning (RowUsageScanner / collect_row_usage / RowFieldFlattener /
// flatten_row_to_locals / hoist_prelude) is shared with the XML adapter — see daslib/linq_fold_common.das.

// Materialize-under-guard: when the whole row escapes only inside a bare `if (fieldCond) { ... }`, read
// the predicate's fields cheaply (read_json_field — no peek/borrow split, JSON is heap-owned) and build the
// full from_JV row only for matches. False (caller does the full build) unless the body is that exact shape.
[macro_function]
def private try_json_under_guard_materialize(var body : Expression?; bindName : string; declT : TypeDeclPtr;
                                             defName, curName : string; at : LineInfo;
                                             var prelude : array<Expression?>; var inner : array<Expression?>) : bool {
    if (!(body is ExprIfThenElse)) return false
    var ite = body as ExprIfThenElse
    if (ite.if_false != null || ite.cond == null || ite.if_true == null) return false
    var (condAllUsed, condFields) = collect_row_usage(ite.cond, bindName)
    if (condAllUsed || empty(condFields)) return false   // whole-`it` in the guard, or no field to prune
    sort(condFields)
    var fieldToLocal <- {for (f in condFields); f => qn("jf_{f}", at)}
    prelude |> push <| qmacro_expr() {
        let $i(defName) = default<$t(declT)>
    }
    inner |> reserve(length(inner) + length(condFields) + 1)
    for (f in condFields) {
        let local = fieldToLocal[f]
        inner |> push <| qmacro_expr() {
            let $i(local) = read_json_field($i(curName), $v(f), $i(defName).$f(f))
        }
    }
    ite.cond = flatten_row_to_locals(ite.cond, bindName, fieldToLocal)
    // Prepend the full materialize to the guarded branch so only matching elements pay from_JV.
    var guarded : array<Expression?>   // nolint:STYLE012 — array-literal-of-qmacro_expr is parse-fragile
    guarded |> push <| qmacro_expr() {
        var $i(bindName) <- from_JV($i(curName), type<$t(declT)>)
    }
    guarded |> push(ite.if_true)
    ite.if_true = stmts_to_expr(guarded)
    inner |> push(body)
    return true
}

// Fills `prelude` (the hoisted default-row decl, prune path only) + `inner` (per-element, ends with
// `body`): allUsed -> full from_JV(it); no field refs -> body only; pruned -> one `let jf_<f>` read per
// referenced field with body's `it.<f>` rewritten to it (struct dropped). See linq_fold.md.
[macro_function]
def private build_json_materializer(var body : Expression?; bindName : string; declT : TypeDeclPtr;
                                    defName, curName : string; at : LineInfo;
                                    var prelude : array<Expression?>; var inner : array<Expression?>) {
    var (allUsed, usedFields) = collect_row_usage(body, bindName)
    if (allUsed) {
        if (try_json_under_guard_materialize(body, bindName, declT, defName, curName, at, prelude, inner)) {
            return
        }
        inner |> push <| qmacro_expr() {
            var $i(bindName) <- from_JV($i(curName), type<$t(declT)>)
        }
        inner |> push(body)
        return
    }
    if (empty(usedFields)) {
        inner |> push(body)
        return
    }
    sort(usedFields)   // deterministic emission (AOT hash / test stability)
    var fieldToLocal <- {for (f in usedFields); f => qn("jf_{f}", at)}
    prelude |> push <| qmacro_expr() {
        let $i(defName) = default<$t(declT)>
    }
    inner |> reserve(length(inner) + length(usedFields) + 1)
    for (f in usedFields) {
        let local = fieldToLocal[f]
        inner |> push <| qmacro_expr() {
            let $i(local) = read_json_field($i(curName), $v(f), $i(defName).$f(f))
        }
    }
    body = flatten_row_to_locals(body, bindName, fieldToLocal)
    inner |> push(body)
}

// Build the JSON array walk shared by wrap_source_loop and emit_distinct_take_loop. `for` over the array
// makes a `continue` (skip/skip_while) land on the next element; non-null breakGuard emits `break if (guard)`
// first (distinct+take early-exit). The `is _array` guard yields nothing for a null/non-array source.
[macro_function]
def private build_json_array_walk(srcName : string; curName : string; var inner : array<Expression?>;
                                  var breakGuard : Expression?) : Expression? {
    var elemBody : array<Expression?>
    if (breakGuard != null) {
        elemBody |> push <| qmacro_expr() {
            break if ($e(breakGuard))
        }
    }
    elemBody |> push_from(inner)
    return qmacro_block() {
        if ($i(srcName) != null && $i(srcName).value is _array) {
            for ($i(curName) in $i(srcName).value as _array) {
                $b(elemBody)
            }
        }
    }
}

// Materialize the per-element row (field-pruned) + emit the array walk + hoist the prelude. Shared by
// wrap_source_loop and emit_distinct_take_loop. Non-null breakGuard emits `break if (guard)` first.
[macro_function]
def private build_pruned_array_walk(srcName : string; elemType : TypeDeclPtr; bindName : string;
                                    var body : Expression?; var breakGuard : Expression?; at : LineInfo) : Expression? {
    let curName = qn("jcur", at)
    let defName = qn("jdef", at)
    var declT = strip_const_ref(clone_type(elemType))
    var prelude : array<Expression?>
    var inner : array<Expression?>
    build_json_materializer(body, bindName, declT, defName, curName, at, prelude, inner)
    var loop = build_json_array_walk(srcName, curName, inner, breakGuard)
    return hoist_prelude(prelude, loop, at)
}

// Single flat for-loop over the JSON array, like ArrayAdapter (unlike decs's 2-level archetype walk or
// XML's DOM child-element while-walk). No name variants — the source IS the array.
class JsonAdapter : SourceAdapter {
    jsonExpr : Expression?
    srcName  : string
    elemType : TypeDeclPtr
    def override name_prefix() : string {
        return "json_"
    }
    def override supports_direct_return() : bool {
        return true   // single flat for-loop inside the invoke; a mid-loop `return` exits the invoke
    }
    def override const can_group_by() : bool {
        return true
    }
    def override build_group_by_adapter(var c : Captures; var ctx : EmitCtx; at : LineInfo) : SourceAdapter? {
        // join |> group_by over JSON isn't spliced yet (can_join is false) — group_by alone clones self.
        return new JsonAdapter(jsonExpr = clone_expression(jsonExpr), srcName = qn("jsrc", at),
                               elemType = clone_type(elemType))
    }
    def override arraySrcName() : string {
        return srcName
    }
    def override bind_name(at : LineInfo) : string {
        return qn("it", at)
    }
    def override element_type() : TypeDeclPtr {
        return clone_type(elemType)
    }
    def override wrap_source_loop(loopShape : LoopDispatch; var body : Expression?; at : LineInfo) : Expression? {
        return build_pruned_array_walk(srcName, elemType, bind_name(at), body, null, at)
    }
    def override emit_distinct_take_loop(bindName : string; takenName : string; takeLimName : string; var perElement : Expression?; at : LineInfo) : Expression? {
        // distinct(...) |> take(N): same field-pruned walk, breaking once N distinct elements are taken.
        var breakGuard = qmacro($i(takenName) >= $i(takeLimName))
        return build_pruned_array_walk(srcName, elemType, bindName, perElement, breakGuard, at)
    }
    def override wrap_invoke(var stmts : array<Expression?>; retType : TypeDeclPtr; wrapIter : bool; at : LineInfo) : Expression? {
        // Const-accepting param: the source `jv` is often a `let` (const JsonValue?), and a non-const source
        // adds-const cleanly — so the single fused invoke binds either. Strip ref, force const.
        var jsonType = strip_const_ref(clone_type(jsonExpr._type))
        jsonType.flags.constant = true
        var jsonClone = clone_expression(jsonExpr)
        jsonClone.genFlags.alwaysSafe = true
        let sn = srcName
        var emission : Expression?
        if (retType != null) {
            emission = qmacro(invoke($($i(sn) : $t(jsonType)) : $t(retType) {
                $b(stmts)
            }, $e(jsonClone)))
        } else {
            emission = qmacro(invoke($($i(sn) : $t(jsonType)) {
                $b(stmts)
            }, $e(jsonClone)))
        }
        emission = finalize_invoke(emission, at)
        if (wrapIter) {
            emission = qmacro($e(emission).to_sequence_move())
            emission.force_generated(true)
        }
        return emission
    }
    // Deferred materialization: a buffered reducer (order/take, distinct) holds `(orderKey, JsonValue?)`
    // surrogates and runs from_JV only for the K survivors — from_json otherwise materializes all N. The
    // handle is the per-element array element `jcur`; from_JV's one-call full-row build makes the survivor
    // materialize trivial (no per-attribute walk like XML's build_xml_row).
    def override const defers_materialization() : bool {
        return true
    }
    def override handle_type() : TypeDeclPtr {
        var ht = strip_const_ref(clone_type(jsonExpr._type))
        if (ht.firstType != null) {
            ht.firstType.flags.constant = true   // `JsonValue const?` — matches `jsrc as _array`'s const elements
        }
        return ht
    }
    def override current_handle_expr(at : LineInfo) : Expression? {
        let cn = qn("jcur", at)
        return qmacro($i(cn))
    }
    def override materialize_handle(var handleExpr : Expression?; outBind : string; at : LineInfo) : Expression? {
        var declT = strip_const_ref(clone_type(elemType))
        return qmacro_expr() {
            $i(outBind) <- from_JV($e(handleExpr), type<$t(declT)>)
        }
    }
}

// Recognize a `from_json(jv, type<Row>)` chain top. Returns the call (caller reads arguments[0] = jv,
// `_type.firstType` = Row); null otherwise. Name-only match — no json types referenced here, so a
// JSON-less program never resolves a `from_json` and this returns null (the chain falls to the array tier).
[macro_function]
def extract_json_source(var top : Expression?) : ExprCall? {
    if (top == null || !(top is ExprCall)) return null
    var c = top as ExprCall
    if (get_call_short_name(c) != "from_json" || c._type == null || !c._type.isIterator
            || c._type.firstType == null || length(c.arguments) != 2) return null
    return c
}
