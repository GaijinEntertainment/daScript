options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options _comment_hygiene = true

module linq_fold_table shared public

//! linq_fold table source adapter: ``TableAdapter`` + ``extract_table_source``. Lets ``_fold`` over an
//! ``each_kv(tab)`` / ``keys(tab)`` / ``values(tab)`` chain emit a fused slot-walk loop over the table
//! instead of riding the generic iterator tier. The kv lane scans which sides of the pair the chain
//! touches and prunes the walk to a keys-only / values-only single iterator (the table analog of XML
//! field-pruning); bare ``count()`` folds to O(1) ``length(tab)``; a plain ``distinct`` over raw keys/kv
//! elements is dropped (keys are unique by construction). In-tree companion to ``daslib/linq_fold``
//! (required unconditionally; the matcher returns null for non-table chains). Emits ``keys`` / ``values``
//! / the kv zip BY NAME at the user's splice site. See benchmarks/sql/LINQ_TO_TABLE.md.

require daslib/ast_boost
require daslib/ast_match
require daslib/templates_boost
require daslib/macro_boost
require daslib/linq_fold_common public

enum TableLane {
    KV
    KEYS
    VALUES
}

// Per-element table walk; the KV lane prunes to the cheapest iterator set from the body's
// `it.key` / `it.value` usage (the table analog of XML field-pruning, see LINQ_TO_TABLE.md).
[macro_function]
def private build_table_walk(lane : TableLane; srcName, bindName : string; var body : Expression?;
                             var breakGuard : Expression?; at : LineInfo) : Expression? {
    var inner : array<Expression?>
    if (breakGuard != null) {
        inner |> push <| qmacro_expr() {
            break if ($e(breakGuard))
        }
    }
    // Literal loop-var names below: the qmacro grammar only allows $i() in the FIRST iterator slot of a multi-source for, so the zip header uses fixed names (same trade ZipAdapter makes with itA/itB) — they live only inside the generated invoke. keys() deliberately yields NON-const elements (writable temp copies), so the engine-visible bind is a `let` rebind of the loop var — keys are workhorse types, the copy is free, and downstream ==const composition (push_clone of a bare projected key) needs the const.
    if (lane == TableLane.KEYS) {
        inner |> push <| qmacro_expr() {
            let $i(bindName) = _tab_kv_key_
        }
        inner |> push(body)
        return <- qmacro_block() {
            for (_tab_kv_key_ in keys($i(srcName))) {
                $b(inner)
            }
        }
    }
    if (lane == TableLane.VALUES) {
        // values over the const table param yield `V& const` — bind directly, no rebind copy
        inner |> push(body)
        return <- qmacro_block() {
            for ($i(bindName) in values($i(srcName))) {
                $b(inner)
            }
        }
    }
    // KV lane
    let vName = "_tab_kv_value_"
    var (allUsed, usedFields) = collect_row_usage(body, bindName)
    if (allUsed) {
        inner |> push <| qmacro_expr() {
            let $i(bindName) = (key = _tab_kv_key_, value = _tab_kv_value_)
        }
        inner |> push(body)
        return <- qmacro_block() {
            for (_tab_kv_key_, _tab_kv_value_ in keys($i(srcName)), values($i(srcName))) { // nolint:LINT002 parallel-for must bind both vars
                $b(inner)
            }
        }
    }
    let useKey = usedFields |> has_value("key")
    let useValue = usedFields |> has_value("value")
    let kLocal = qn("tkey", at)
    var fieldToLocal <- { "key" => kLocal, "value" => vName }
    body = flatten_row_to_locals(body, bindName, fieldToLocal)
    if (useKey) {
        inner |> push <| qmacro_expr() {
            let $i(kLocal) = _tab_kv_key_
        }
    }
    inner |> push(body)
    if (useValue && !useKey) {
        return <- qmacro_block() {
            for ($i(vName) in values($i(srcName))) {
                $b(inner)
            }
        }
    }
    if (useKey && useValue) {
        return <- qmacro_block() {
            for (_tab_kv_key_, _tab_kv_value_ in keys($i(srcName)), values($i(srcName))) { // nolint:LINT002 parallel-for must bind both vars
                $b(inner)
            }
        }
    }
    // key-only AND no-field-touched (e.g. bare count walk) both ride the cheaper keys iterator
    return <- qmacro_block() {
        for (_tab_kv_key_ in keys($i(srcName))) { // nolint:LINT002 body may not read the key (bare-count walk)
            $b(inner)
        }
    }
}

// Single flat for-loop over the table inside a 1-param invoke binding the table, like ArrayAdapter over
// an array — except the loop iterator(s) derive from the table param at the splice site.
class TableAdapter : SourceAdapter {
    tabExpr  : Expression?      // the table expression (argument of each_kv/keys/values)
    srcName  : string           // invoke param name binding the table
    elemType : TypeDeclPtr      // KV: tuple<key:K;value:V>; KEYS: K; VALUES: V
    lane     : TableLane
    def override name_prefix() : string {
        return "tab_"
    }
    def override supports_direct_return() : bool {
        return true   // single flat for-loop inside the invoke; a mid-loop `return` exits the invoke
    }
    def override can_reserve_by_length() : bool {
        return true   // length(tab) is O(1); the shared reserve hint reads arrayTop/arraySrcName
    }
    def override arrayTop() : Expression? {
        // Feeds the reserve hint (type_has_length covers tables). The backward-index reverse lanes that
        // also read arrayTop gate on array_source, which is false here — matchTop stays iterator-typed.
        return tabExpr
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
    def override count_shortcut(opName : string; at : LineInfo) : Expression? {
        return emit_length_shortcut(opName, tabExpr, srcName, at)
    }
    def override wrap_source_loop(loopShape : LoopDispatch; var body : Expression?; at : LineInfo) : Expression? {
        return build_table_walk(lane, srcName, bind_name(at), body, null, at)
    }
    def override emit_distinct_take_loop(bindName : string; takenName : string; takeLimName : string; var perElement : Expression?; at : LineInfo) : Expression? {
        var breakGuard = qmacro($i(takenName) >= $i(takeLimName))
        return build_table_walk(lane, srcName, bindName, perElement, breakGuard, at)
    }
    def override wrap_invoke(var stmts : array<Expression?>; retType : TypeDeclPtr; wrapIter : bool; at : LineInfo) : Expression? {
        // Const-accepting param: the source table is often a `let`, and a non-const source adds-const cleanly.
        var tabType = strip_const_ref(clone_type(tabExpr._type))
        tabType.flags.constant = true
        var tabClone = clone_expression(tabExpr)
        tabClone.genFlags.alwaysSafe = true
        let sn = srcName
        var emission : Expression?
        if (retType != null) {
            emission = qmacro(invoke($($i(sn) : $t(tabType)) : $t(retType) {
                $b(stmts)
            }, $e(tabClone)))
        } else {
            emission = qmacro(invoke($($i(sn) : $t(tabType)) {
                $b(stmts)
            }, $e(tabClone)))
        }
        emission = finalize_invoke(emission, at)
        if (wrapIter) {
            emission = qmacro($e(emission).to_sequence_move())
            emission.force_generated(true)
        }
        return emission
    }
}

// ===== Point-lookup folds — `where(kv.key == X)` + terminator → O(1) key probe =====
// any/contains → key_exists, count → key_exists?1:0, first[_or_default] (± select) → tab?[X] probe,
// with the scan's exact semantics. Full shape/decline table: linq_fold_patterns.rst (table source row).

[macro_function]
def private match_key_probe_side(var keySide, otherSide : Expression?; lane : TableLane; bindName : string) : Expression? {
    var k = keySide
    if (k != null && k is ExprRef2Value) {
        k = (k as ExprRef2Value).subexpr
    }
    if (lane == TableLane.KV) {
        if (k == null || !(k is ExprField)) return null
        var f = k as ExprField
        if (f.name != "key") return null
        var base = f.value
        if (base != null && base is ExprRef2Value) {
            base = (base as ExprRef2Value).subexpr
        }
        if (base == null || !(base is ExprVar) || (base as ExprVar).name != bindName) return null
    } else {
        if (k == null || !(k is ExprVar) || (k as ExprVar).name != bindName) return null
    }
    // X must be loop-invariant AND side-effect free — the scan evaluates X per element, a probe once
    if (expr_uses_var(otherSide, bindName) || has_sideeffects(otherSide)) return null
    return clone_expression(otherSide)
}

// Decompose a peeled predicate body (binder renamed to bindName) as `<key-ref> == X`. Returns cloned X.
[macro_function]
def private extract_key_probe(var pred : Expression?; lane : TableLane; bindName : string) : Expression? {
    if (pred == null || !(pred is ExprOp2)) return null
    var op2 = pred as ExprOp2
    if (op2.op != "==") return null
    var probe = match_key_probe_side(op2.left, op2.right, lane, bindName)
    if (probe == null) {
        probe = match_key_probe_side(op2.right, op2.left, lane, bindName)
    }
    return probe
}

[macro_function]
def try_table_point_lookup(var calls : array<tuple<ExprCall?; LinqCall?>>; var adapter : TableAdapter?; at : LineInfo) : Expression? {
    if (adapter.lane == TableLane.VALUES) return null
    let n = length(calls)
    if (n < 1 || n > 3) return null
    var termCall = calls[n - 1]._0
    let termName = calls[n - 1]._1.name
    let termArgs = length(termCall.arguments)
    var selCall : ExprCall?
    var predArg : Expression?
    var keyX : Expression?
    let bindName = qn("plk_it", at)
    if (n == 1) {
        // predicate-form terminators, and the keys-lane contains
        if ((termName == "any" || termName == "count") && termArgs == 2) {
            predArg = termCall.arguments[1]
        } elif (termName == "contains" && termArgs == 2 && adapter.lane == TableLane.KEYS) {
            // element evaluated exactly once on both paths — no invariance/purity gate needed
            keyX = clone_expression(termCall.arguments[1])
        } else {
            return null
        }
    } else {
        if (calls[0]._1.name != "where_" || length(calls[0]._0.arguments) != 2) return null
        predArg = calls[0]._0.arguments[1]
        if (n == 3) {
            if (calls[1]._1.name != "select" || length(calls[1]._0.arguments) != 2
                    || (termName != "first" && termName != "first_or_default")) {
                return null
            }
            selCall = calls[1]._0
        } elif (!((termName == "any" || termName == "count" || termName == "first") && termArgs == 1)
                && !(termName == "first_or_default" && termArgs == 2)) {
            return null
        }
    }
    if (predArg != null) {
        var predBody = peel_lambda_rename_var(predArg, bindName)
        keyX = extract_key_probe(predBody, adapter.lane, bindName)
    }
    if (keyX == null) return null
    let sn = adapter.srcName
    // boolean / counting probes
    if (termName == "any" || termName == "contains") {
        var anyStmts <- qmacro_block_to_array() {
            return key_exists($i(sn), $e(keyX))
        }
        return adapter->wrap_invoke(anyStmts, null, false, at)
    }
    if (termName == "count") {
        var cntStmts <- qmacro_block_to_array() {
            return key_exists($i(sn), $e(keyX)) ? 1 : 0
        }
        return adapter->wrap_invoke(cntStmts, null, false, at)
    }
    // element probes: first / first_or_default, ± trailing select
    var retT = strip_const_ref(clone_type(termCall._type))
    retT.flags.removeConstant = false
    let kName = qn("plk_k", at)
    let dName = qn("plk_d", at)
    var stmts : array<Expression?>
    if (termName == "first_or_default") {
        // eager default bind, matching linq.das argument evaluation order
        stmts |> push <| qmacro_expr() {
            let $i(dName) = $e(termCall.arguments[1])
        }
    }
    stmts |> push <| qmacro_expr() {
        let $i(kName) = $e(keyX)
    }
    var missTail : Expression?
    if (termName == "first_or_default") {
        missTail = qmacro_expr() {
            return $i(dName)
        }
    } else {
        missTail = qmacro_expr() {
            panic("sequence contains no elements")
        }
    }
    if (adapter.lane == TableLane.KEYS) {
        stmts |> push <| qmacro_expr() {
            if (!key_exists($i(sn), $i(kName))) {
                $e(missTail)
            }
        }
        if (selCall != null) {
            var proj = peel_lambda_rename_var(selCall.arguments[1], kName)
            stmts |> push <| qmacro_expr() {
                return $e(proj)
            }
        } else {
            stmts |> push <| qmacro_expr() {
                return $i(kName)
            }
        }
        return adapter->wrap_invoke(stmts, retT, false, at)
    }
    // KV lane: probe the value pointer, materialize the (key, value) pair on hit. Table safe-index is
    // unsafe (the pointer dangles on rehash) — fine here, the generated invoke never mutates the table.
    let pName = qn("plk_p", at)
    stmts |> push_from <| qmacro_block_to_array() {
        let $i(pName) = unsafe($i(sn)?[$i(kName)])
        if ($i(pName) == null) {
            $e(missTail)
        }
    }
    if (selCall != null) {
        let bName = qn("plk_kv", at)
        var proj = peel_lambda_rename_var(selCall.arguments[1], bName)
        stmts |> push_from <| qmacro_block_to_array() {
            let $i(bName) = (key = $i(kName), value = *$i(pName))
            return $e(proj)
        }
    } else {
        stmts |> push <| qmacro_expr() {
            return (key = $i(kName), value = *$i(pName))
        }
    }
    return adapter->wrap_invoke(stmts, retT, false, at)
}

// Recognize an `each_kv(tab)` / `keys(tab)` / `values(tab)` chain top. Returns the call (caller reads
// arguments[0] = the table, `_type.firstType` = element); null otherwise. Name + table-typed-arg match,
// like extract_json_source — the strong arg-type gate keeps an unrelated user `keys` from firing this.
[macro_function]
def extract_table_source(var top : Expression?) : ExprCall? {
    if (top == null || !(top is ExprCall)) return null
    var c = top as ExprCall
    let name = get_call_short_name(c)
    if ((name != "each_kv" && name != "keys" && name != "values")
            || c._type == null || !c._type.isIterator || c._type.firstType == null
            || (c.arguments |> length) != 1) {
        return null
    }
    let srcT = c.arguments[0]._type
    if (srcT == null || !srcT.isGoodTableType) return null
    return c
}

// Drop plain `distinct` over raw keys/kv elements — table keys are unique by construction, so the whole
// dedup-set machinery is a no-op. Only when every call BEFORE the distinct preserves element uniqueness
// (filters/ranges/reorders — never a `select`, which reshapes elements). `distinct_by` keeps its own key.
[macro_function]
def drop_redundant_distinct(var calls : array<tuple<ExprCall?; LinqCall?>>) {
    var dropIdx = -1
    for (i in range(length(calls))) {
        let name = calls[i]._1.name
        if (name == "distinct") {
            dropIdx = i
            break
        }
        // uniqueness-preserving prefix ops only
        if (name != "where_" && name != "skip" && name != "take"
                && name != "skip_while" && name != "take_while" && name != "reverse") {
            return
        }
    }
    if (dropIdx < 0 || length(calls) <= 1) return   // keep a bare `distinct`-only chain for the generic lanes
    calls |> erase(dropIdx)
}
