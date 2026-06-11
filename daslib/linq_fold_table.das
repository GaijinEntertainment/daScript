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
