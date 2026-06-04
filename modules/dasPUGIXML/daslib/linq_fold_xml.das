options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options _comment_hygiene = true

module linq_fold_xml shared public

//! linq_fold XML source adapter: ``XmlAdapter`` + ``extract_xml_source``. Lets ``_fold`` over a
//! ``from_xml_node(node[, name], type<Row>)`` chain emit an inlined DOM child-element walk (mirroring
//! ``PUGIXML_boost``'s generator body) instead of driving the generator + an intermediate array.
//! Optional companion to ``daslib/linq_fold`` — pulled in via ``require ?pugixml`` and gated by
//! ``static_if (typeinfo builtin_module_exists(pugixml))``. See daslib/linq_fold.md.

require pugixml public
require pugixml/PUGIXML_boost
require daslib/ast_boost
require daslib/ast_match
require daslib/templates_boost
require strings
require daslib/linq_fold_common public

// Field-pruning row-usage scanning (RowUsageScanner / collect_row_usage / RowFieldFlattener /
// flatten_row_to_locals / hoist_prelude) is shared with the JSON adapter — see daslib/linq_fold_common.das.

// Materialize-under-guard: when the whole row escapes only inside a bare `if (fieldCond) { ... }`, read
// just the predicate's fields cheaply and build the full row (its `name` clone) only for matching elements.
// Returns false (caller does the unconditional full build) unless the body is exactly that guarded shape.
[macro_function]
def private try_xml_under_guard_materialize(var body : Expression?; bindName : string; declT : TypeDeclPtr;
                                            defName, chName : string; at : LineInfo;
                                            var prelude : array<Expression?>; var inner : array<Expression?>) : bool {
    if (!(body is ExprIfThenElse)) return false
    var ite = body as ExprIfThenElse
    if (ite.if_false != null || ite.cond == null || ite.if_true == null) return false
    var (condAllUsed, condFields) = collect_row_usage(ite.cond, bindName)
    if (condAllUsed || empty(condFields)) return false   // whole-`it` in the guard, or no field to prune
    sort(condFields)
    var fieldToLocal <- {for (f in condFields); f => qn("xf_{f}", at)}
    prelude |> push <| qmacro_expr() {
        let $i(defName) = default<$t(declT)>
    }
    inner |> reserve(length(inner) + length(condFields) + 1)
    for (f in condFields) {
        let local = fieldToLocal[f]
        // peek, not read: string guard fields come back as borrowed string# (no clone); the cond reads them, storing one is a compile error.
        inner |> push <| qmacro_expr() {
            let $i(local) = peek_xml_field($i(chName), $v(f), $i(defName).$f(f))
        }
    }
    ite.cond = flatten_row_to_locals(ite.cond, bindName, fieldToLocal)
    // Prepend the full materialize to the guarded branch so only matching elements pay build_xml_row.
    var guarded : array<Expression?>   // nolint:STYLE012 — array-literal-of-qmacro_expr is parse-fragile
    guarded |> push <| qmacro_expr() {
        var $i(bindName) = default<$t(declT)>
    }
    guarded |> push <| qmacro_expr() {
        build_xml_row($i(chName), $i(bindName))
    }
    guarded |> push(ite.if_true)
    ite.if_true = stmts_to_expr(guarded)
    inner |> push(body)
    return true
}

// Fills `prelude` (the hoisted default-row decl, flatten path only) + `inner` (per-element, ends with
// `body`): allUsed -> full build_xml_row; no field refs -> body only; pruned -> one `let xf_<f>` read
// per referenced field with body's `it.<f>` rewritten to it (struct dropped). See linq_fold.md.
[macro_function]
def private build_xml_materializer(var body : Expression?; bindName : string; declT : TypeDeclPtr;
                                   defName, chName : string; at : LineInfo;
                                   var prelude : array<Expression?>; var inner : array<Expression?>) {
    var (allUsed, usedFields) = collect_row_usage(body, bindName)
    if (allUsed) {
        if (try_xml_under_guard_materialize(body, bindName, declT, defName, chName, at, prelude, inner)) {
            return
        }
        inner |> push <| qmacro_expr() {
            var $i(bindName) = default<$t(declT)>
        }
        inner |> push <| qmacro_expr() {
            build_xml_row($i(chName), $i(bindName))
        }
        inner |> push(body)
        return
    }
    if (empty(usedFields)) {
        inner |> push(body)
        return
    }
    sort(usedFields)   // deterministic emission (AOT hash / test stability)
    var fieldToLocal <- {for (f in usedFields); f => qn("xf_{f}", at)}
    prelude |> push <| qmacro_expr() {
        let $i(defName) = default<$t(declT)>
    }
    inner |> reserve(length(inner) + length(usedFields) + 1)
    for (f in usedFields) {
        let local = fieldToLocal[f]
        inner |> push <| qmacro_expr() {
            let $i(local) = read_xml_field($i(chName), $v(f), $i(defName).$f(f))
        }
    }
    body = flatten_row_to_locals(body, bindName, fieldToLocal)
    inner |> push(body)
}

// Build the DOM child-element walk shared by wrap_source_loop and emit_distinct_take_loop. `xch` advances
// before the body so a `continue` (skip/skip_while) lands on the next sibling; the body reads the captured
// `xcur`. Non-null breakGuard emits `break if (guard)` first (distinct+take early-exit); nameArg null = 2-arg overload, non-null = 3-arg named.
[macro_function]
def private build_xml_dom_walk(sn : string; var nameArg : Expression?; curName, chName : string;
                               var inner : array<Expression?>; var breakGuard : Expression?; at : LineInfo) : Expression? {
    var elemBody : array<Expression?>
    if (breakGuard != null) {
        elemBody |> push <| qmacro_expr() {
            break if ($e(breakGuard))
        }
    }
    elemBody |> push_from(inner)
    if (nameArg == null) {
        return qmacro_block() {
            var $i(chName) = $i(sn).first_child
            while ($i(chName).ok) {
                let $i(curName) = $i(chName)
                $i(chName) = $i(chName).next_sibling
                if ($i(curName)._type == xml_node_type.node_element) {
                    $b(elemBody)
                }
            }
        }
    }
    let nmName = qn("xname", at)
    return qmacro_block() {
        let $i(nmName) = $e(nameArg)
        var $i(chName) = child($i(sn), $i(nmName))
        while ($i(chName).ok) {
            let $i(curName) = $i(chName)
            $i(chName) = next_sibling($i(chName), $i(nmName))
            $b(elemBody)
        }
    }
}

// 2-source invoke binding the XML node (param 1, named `srcName` so the DOM walk in bodyStmts reads it)
// + the in-memory srcB array (param 2). Shared by XmlAdapter.emit_join_hook (standalone) and
// XmlJoinAdapter.wrap_invoke (group_by-after-join). Mirrors XmlAdapter.wrap_invoke with the extra param.
[macro_function]
def private build_xml_join_invoke(var nodeExpr : Expression?; srcName : string; var bodyStmts : array<Expression?>;
                                  srcBName : string; srcBType : TypeDeclPtr; var srcBExpr : Expression?;
                                  retType : TypeDeclPtr; wrapIter : bool; at : LineInfo) : Expression? {
    var nodeType = strip_const_ref(clone_type(nodeExpr._type))
    var nodeClone = clone_expression(nodeExpr)
    nodeClone.genFlags.alwaysSafe = true
    var srcbClone = clone_expression(srcBExpr)
    srcbClone.genFlags.alwaysSafe = true
    let sn = srcName
    var emission : Expression?
    if (retType != null) {
        emission = qmacro(invoke($($i(sn) : $t(nodeType), $i(srcBName) : $t(srcBType)) : $t(retType) {
            $b(bodyStmts)
        }, $e(nodeClone), $e(srcbClone)))
    } else {
        emission = qmacro(invoke($($i(sn) : $t(nodeType), $i(srcBName) : $t(srcBType)) {
            $b(bodyStmts)
        }, $e(nodeClone), $e(srcbClone)))
    }
    emission = finalize_invoke(emission, at)
    if (wrapIter) {
        emission = qmacro($e(emission).to_sequence_move())
        emission.force_generated(true)
    }
    return emission
}

// Materialize the per-element row (field-pruned) + emit the DOM child-element walk + hoist the prelude.
// Shared by XmlAdapter.wrap_source_loop / emit_distinct_take_loop and XmlJoinAdapter.wrap_source_loop.
// Non-null breakGuard emits `break if (guard)` before the body (take early-exit).
[macro_function]
def private build_pruned_dom_walk(srcName : string; var nameArg : Expression?; elemType : TypeDeclPtr;
                                  bindName : string; var body : Expression?; var breakGuard : Expression?;
                                  at : LineInfo) : Expression? {
    let curName = qn("xcur", at)
    let defName = qn("xdef", at)
    var declT = strip_const_ref(clone_type(elemType))
    var prelude : array<Expression?>
    var inner : array<Expression?>
    build_xml_materializer(body, bindName, declT, defName, curName, at, prelude, inner)
    var loop = build_xml_dom_walk(srcName, nameArg, curName, qn("xch", at), inner, breakGuard, at)
    return hoist_prelude(prelude, loop, at)
}

// Single flat loop, like ArrayAdapter (unlike decs's 2-level archetype walk). nameArg null = the 2-arg
// `from_xml_node(node, type<Row>)` overload (walk all child elements); non-null = the 3-arg named overload
// (walk child elements whose tag matches the name).
class XmlAdapter : SourceAdapter {
    nodeExpr : Expression?
    srcName  : string
    elemType : TypeDeclPtr
    nameArg  : Expression?
    def override name_prefix() : string {
        return "xml_"
    }
    def override supports_direct_return() : bool {
        return true   // single flat DOM while-walk inside the invoke (not decs's 2-level archetype callback)
    }
    def override const can_group_by() : bool {
        return true
    }
    def override const can_join() : bool {
        return true
    }
    def override emit_join_hook(var c : Captures; var ctx : EmitCtx; at : LineInfo) : Expression? {
        // XML standalone join: hashed equi-join collecting srcB into a table, then probing from the
        // field-pruned DOM walk (mirrors emit_array_join, but srcA is the XML node, not an array param).
        // group join over an XML source isn't spliced yet — defer to tier-2 (the array adapter handles it).
        if (c.single_name |> key_exists("join") && c.single_name["join"] == "group_join") return null
        var joinCall = c.single["join"]
        if (!srcb_is_array_shaped(c, "join")) return null
        var whereLam : Expression?
        if (c.single |> key_exists("trailing_where")) {
            whereLam = c.single["trailing_where"] != null ? c.single["trailing_where"].arguments[1] : null
            if (whereLam == null) return null
        }
        var selectLam : Expression?
        if (c.single |> key_exists("trailing_select")) {
            var selCall = c.single["trailing_select"]
            if (selCall == null || selCall._type == null || selCall._type.firstType == null) return null
            selectLam = selCall.arguments[1]
            if (selectLam == null) return null
        }
        var leadWhereLam = extract_join_lead_where(c)
        if (c.single |> key_exists("lead_where") && leadWhereLam == null) return null
        let countOnly = c.single |> key_exists("term")
        // Iterator-typed result (no count terminator): the fused single-pass build is the measured
        // winner over a streaming generator (benchmarks/micro/join_select_shapes.das) — materialize
        // the field-pruned result array, then wrap as a sequence to honor the iterator contract.
        let wrapIter = !countOnly && ctx.expr_is_iterator
        var srcbSrc = joinCall.arguments[1]
        if (srcbSrc == null || srcbSrc._type == null || srcbSrc._type.firstType == null) return null
        var keyaLam = joinCall.arguments[2]
        var keybLam = joinCall.arguments[3]
        if (keyaLam == null || keybLam == null || keyaLam._type == null) return null
        let keyType = strip_const_ref(clone_type(keyaLam._type.firstType))
        if (!is_primitive_join_key_type(keyType)) return null
        let tupAName = bind_name(at)   // = "it"; keya/result peel to read it.<f> so the materializer prunes
        let srcBName = qn("xjoin_srcB", at)
        let tupBName = qn("xjoin_tup_b", at)
        let hashName = qn("xjoin_hash", at)
        var keyaBody = peel_lambda_rename_var(keyaLam, tupAName)
        var keybBody = peel_lambda_rename_var(keybLam, tupBName)
        if (keyaBody == null || keybBody == null) return null
        let tupBType = strip_const_ref(clone_type(srcbSrc._type.firstType))
        var pieces = build_join_standalone_pieces(joinCall, whereLam, selectLam, leadWhereLam, countOnly, keyaBody, tupAName, hashName, tupBType, false, "xjoin", at)
        if (pieces == null) return null
        var allStmts : array<Expression?>
        allStmts |> push_from(pieces.preludeStmts)
        allStmts |> push <| qmacro_expr() {
            var $i(hashName) : table<$t(keyType); array<$t(tupBType)>>
        }
        allStmts |> push <| qmacro_expr() {
            for ($i(tupBName) in $i(srcBName)) {
                // nolint:PERF006 per-key bucket size unknown ahead of time
                $i(hashName)[$e(keybBody)] |> push_clone($i(tupBName))
            }
        }
        allStmts |> push(wrap_source_loop(LoopDispatch(Each = null), pieces.probeOuter, at))
        allStmts |> push(pieces.returnStmt)
        return build_xml_join_invoke(nodeExpr, srcName, allStmts, srcBName, invoke_src_param_type(srcbSrc), srcbSrc, pieces.invokeRetType, wrapIter, at)
    }
    def override build_group_by_adapter(var c : Captures; var ctx : EmitCtx; at : LineInfo) : SourceAdapter? {
        if (c.single |> key_exists("upstream_join")) {
            // join |> group_by over XML: srcA = the XML node (DOM-walked), srcB = join's second arg.
            // plan_group_by_core drives the bucket-fill through XmlJoinAdapter.wrap_source_loop (pruning free).
            var joinCall = c.single["upstream_join"]
            var srcbSrc = joinCall.arguments[1]
            if (srcbSrc == null || srcbSrc._type == null || srcbSrc._type.firstType == null
                    || !(srcbSrc._type.isGoodArrayType || srcbSrc._type.isArray || srcbSrc._type.isIterator)) {
                return null
            }
            var keyaLam = joinCall.arguments[2]
            var keybLam = joinCall.arguments[3]
            var resultLam = joinCall.arguments[4]
            if (keyaLam == null || keybLam == null || resultLam == null
                    || keyaLam._type == null || resultLam._type == null
                    || resultLam._type.firstType == null) {
                return null
            }
            var keyType = strip_const_ref(clone_type(keyaLam._type.firstType))
            if (!is_primitive_join_key_type(keyType)) {
                return null
            }
            var resultType = strip_const_ref(clone_type(resultLam._type.firstType))
            var tupBType = strip_const_ref(clone_type(srcbSrc._type.firstType))
            var srcbClone = clone_expression(srcbSrc)
            srcbClone.genFlags.alwaysSafe = true
            var nameJoinClone : Expression? = nameArg != null ? clone_expression(nameArg) : null
            return new XmlJoinAdapter(shape = XmlJoinShape(
                nodeExpr = clone_expression(nodeExpr),
                srcName = qn("xnode", at),
                elemType = clone_type(elemType),
                nameArg = nameJoinClone,
                srcBExpr = srcbClone,
                srcBName = qn("xjoin_srcB", at),
                srcBType = invoke_src_param_type(srcbSrc),
                keyaLam = keyaLam,
                keybLam = keybLam,
                resultLam = resultLam,
                keyType = keyType,
                tupBType = tupBType,
                resultType = resultType,
                resBindName = qn("xjoin_jres", at)))
        }
        var nameClone : Expression? = nameArg != null ? clone_expression(nameArg) : null
        return new XmlAdapter(nodeExpr = clone_expression(nodeExpr), srcName = qn("xnode", at),
                              elemType = clone_type(elemType), nameArg = nameClone)
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
        return build_pruned_dom_walk(srcName, nameArg, elemType, bind_name(at), body, null, at)
    }
    def override emit_distinct_take_loop(bindName : string; takenName : string; takeLimName : string; var perElement : Expression?; at : LineInfo) : Expression? {
        // distinct(...) |> take(N): same field-pruned DOM walk, breaking once N distinct elements have been
        // taken (perElement's distinct gate bumps `taken`).
        var breakGuard = qmacro($i(takenName) >= $i(takeLimName))
        return build_pruned_dom_walk(srcName, nameArg, elemType, bindName, perElement, breakGuard, at)
    }
    def override wrap_invoke(var stmts : array<Expression?>; retType : TypeDeclPtr; wrapIter : bool; at : LineInfo) : Expression? {
        var nodeType = strip_const_ref(clone_type(nodeExpr._type))
        var nodeClone = clone_expression(nodeExpr)
        nodeClone.genFlags.alwaysSafe = true
        let sn = srcName
        var emission : Expression?
        if (retType != null) {
            emission = qmacro(invoke($($i(sn) : $t(nodeType)) : $t(retType) {
                $b(stmts)
            }, $e(nodeClone)))
        } else {
            emission = qmacro(invoke($($i(sn) : $t(nodeType)) {
                $b(stmts)
            }, $e(nodeClone)))
        }
        emission = finalize_invoke(emission, at)
        if (wrapIter) {
            emission = qmacro($e(emission).to_sequence_move())
            emission.force_generated(true)
        }
        return emission
    }
    // Deferred materialization: a buffered reducer (order/take, distinct) holds `(orderKey, xml_node)`
    // surrogates and runs build_xml_row only for the K survivors — `from_xml_node` otherwise materializes
    // all N. The handle is the per-element node `xcur` bound inside build_pruned_dom_walk's walk.
    def override const defers_materialization() : bool {
        return true
    }
    def override handle_type() : TypeDeclPtr {
        return strip_const_ref(clone_type(nodeExpr._type))
    }
    def override current_handle_expr(at : LineInfo) : Expression? {
        let cn = qn("xcur", at)
        return qmacro($i(cn))
    }
    def override materialize_handle(var handleExpr : Expression?; outBind : string; at : LineInfo) : Expression? {
        return qmacro_expr() {
            build_xml_row($e(handleExpr), $i(outBind))
        }
    }
}

// XML analog of ArrayJoinShape / DecsJoinShape — srcA = XML node (DOM-walked), srcB = in-memory array.
struct XmlJoinShape {
    nodeExpr : Expression?
    srcName  : string
    elemType : TypeDeclPtr
    nameArg  : Expression?
    srcBExpr : Expression?
    srcBName : string
    srcBType : TypeDeclPtr
    keyaLam, keybLam, resultLam : Expression?
    keyType, tupBType, resultType : TypeDeclPtr
    resBindName : string
}

// group_by-after-join over XML — returned from XmlAdapter.build_group_by_adapter on the upstream-join arm.
// plan_group_by_core drives the bucket-fill through wrap_source_loop (collect srcB into a hash, probe from
// the field-pruned DOM walk). Mirrors ArrayJoinAdapter / DecsJoinAdapter.
class XmlJoinAdapter : SourceAdapter {
    shape : XmlJoinShape
    def override name_prefix() : string {
        return "xjoin_"
    }
    def override supports_direct_return() : bool {
        return true   // collect for{} + probe DOM while-walk, both single-level inside the invoke
    }
    def override bind_name(at : LineInfo) : string {
        return shape.resBindName
    }
    def override element_type() : TypeDeclPtr {
        return clone_type(shape.resultType)
    }
    def override wrap_source_loop(loopShape : LoopDispatch; var body : Expression?; at : LineInfo) : Expression? {
        let jhashName = qn("xjoin_jhash", at)
        let arrName   = qn("xjoin_jarr", at)
        let tupBName  = qn("xjoin_jtb", at)
        let tupAName  = qn("it", at)   // srcA row bind; keya/result peel to read it.<f> so the materializer prunes
        var pieces = build_join_adapter_pieces(shape.keyaLam, shape.keybLam, shape.resultLam, tupAName, tupBName, jhashName, arrName, shape.resBindName, shape.tupBType, shape.resultType, body, at)
        if (pieces == null) return null
        var probeWalk = build_pruned_dom_walk(shape.srcName, shape.nameArg, shape.elemType, tupAName, pieces.probeOuter, null, at)
        let keyType = shape.keyType
        let tupBType = shape.tupBType
        let srcBName = shape.srcBName
        return <- qmacro_block() {
            var $i(jhashName) : table<$t(keyType); array<$t(tupBType)>>
            for ($i(tupBName) in $i(srcBName)) {
                $e(pieces.collectInner)
            }
            $e(probeWalk)
        }
    }
    def override wrap_invoke(var stmts : array<Expression?>; retType : TypeDeclPtr; wrapIter : bool; at : LineInfo) : Expression? {
        return build_xml_join_invoke(shape.nodeExpr, shape.srcName, stmts, shape.srcBName, shape.srcBType, shape.srcBExpr, retType, wrapIter, at)
    }
}

// Recognize a `from_xml_node(node[, name], type<Row>)` chain top. Returns the call (caller reads
// arguments[0] = node, arguments[1] = name for the 3-arg form, `_type.firstType` = Row); null otherwise.
[macro_function]
def extract_xml_source(var top : Expression?) : ExprCall? {
    if (top == null || !(top is ExprCall)) return null
    var c = top as ExprCall
    if (get_call_short_name(c) != "from_xml_node" || c._type == null || !c._type.isIterator || c._type.firstType == null) return null
    let na = length(c.arguments)
    if (na != 2 && na != 3) return null
    return c
}
