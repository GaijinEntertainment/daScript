options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
// Intentionally opt in for user-facing docs: keep comments concise and actionable.
options _comment_hygiene = true

module linq_fold shared public

//! LINQ ``_fold`` macro — fuse-time-aware pipeline rewriting.
//!
//! ``_fold(expr)`` is a three-tier cascade:
//!
//! 1. **Splice** — when the chain matches a recognized pattern, emit a single
//!    fused for-loop with predicates and projections inlined (best perf,
//!    no intermediate iterators or buffers).
//! 2. **``fold_linq_default``** — when splice can't fire, emit an
//!    array-shape pipeline (``call → array → call → array``) with
//!    ``_inplace`` variants reusing the same buffer and explicit ``delete``
//!    on the previous stage. One buffer at a time, no iterator overhead.
//! 3. **``clone_expression``** — raw passthrough when the chain has no
//!    recognized linq operators (``flatten_linq`` returns empty).
//!
//! All three tiers preserve semantics — ``_fold(chain)`` is observationally
//! equivalent to ``chain``, just faster when patterns match. The macro is
//! always safe to apply.
//!
//! Requires the ``linq`` module for the backing operator functions.

require daslib/linq public

require daslib/ast_boost
require daslib/ast_match
require daslib/templates_boost
require daslib/macro_boost
require strings
require daslib/linq_fold_common public

// Hashed equi-join payload — feeds the per-pair `let resBindName = result(tupA,tupB)` bind used by both GroupBy's decs-join arm and `emit_decs_join`. The 2 consumers differ on whether the per-pair loop is the right granularity (GroupBy: yes; emit_decs_join keeps a bucket-length fast path for count-no-where).
struct private DecsJoinShape {
    bridgeA, bridgeB : DecsBridgeShape?
    keyaLam, keybLam, resultLam : Expression?
    keyType, tupBType, resultType : TypeDeclPtr
    resBindName : string
}

// 2-source lockstep zip payload — feeds `for (itA, itB in srcA, srcB) { body }` and the 2-source outer invoke.
struct private ZipShape {
    srcAExpr, srcBExpr : Expression?
    srcAName, srcBName : string
    srcAType, srcBType : TypeDeclPtr
}

// Array mirror of DecsJoinShape — same role (per-pair result bind for the cross-arm group_by case), 2 array sources iterated via plain `for` loops + a 2-source outer invoke. srcAType / srcBType are the FULL source types (array<T> / iterator<T>) for invoke-param spelling; tupAType / tupBType are the element types post strip_const_ref.
struct private ArrayJoinShape {
    srcAExpr, srcBExpr : Expression?
    srcAName, srcBName : string
    srcAType, srcBType : TypeDeclPtr
    tupAType, tupBType : TypeDeclPtr
    keyaLam, keybLam, resultLam : Expression?
    keyType, resultType : TypeDeclPtr
    resBindName : string
}

class private ArrayAdapter : SourceAdapter {
    top : Expression?
    srcName : string
    def override kind() : AdapterKind {
        return AdapterKind.k_array
    }
    def override arrayTop() : Expression? {
        return top
    }
    def override arraySrcName() : string {
        return srcName
    }
    def override bind_name(at : LineInfo) : string {
        return qn("it", at)
    }
    def override element_type() : TypeDeclPtr {
        if (top == null || top._type == null) {
            return null
        }
        return clone_type(top._type.firstType)
    }
    def override wrap_source_loop(var body : Expression?; at : LineInfo) : Expression? {
        let bindName = qn("it", at)
        let sn = srcName
        return <- qmacro_block() {
            for ($i(bindName) in $i(sn)) {
                $e(body)
            }
        }
    }
    def override wrap_invoke(var stmts : array<Expression?>; retType : TypeDeclPtr; wrapIter : bool; at : LineInfo) : Expression? {
        var srcParamType = invoke_src_param_type(top)
        var topExpr = clone_expression(top)
        topExpr.genFlags.alwaysSafe = true
        let sn = srcName
        var emission : Expression?
        if (retType != null) {
            emission = qmacro(invoke($($i(sn) : $t(srcParamType)) : $t(retType) {
                $b(stmts)
            }, $e(topExpr)))
        } else {
            emission = qmacro(invoke($($i(sn) : $t(srcParamType)) {
                $b(stmts)
            }, $e(topExpr)))
        }
        emission = finalize_invoke(emission, at)
        if (wrapIter) {
            emission = qmacro($e(emission).to_sequence_move())
            emission.force_generated(true)
        }
        return emission
    }
    def override emit_distinct_take_loop(bindName : string; takenName : string; takeLimName : string; var perElement : Expression?; at : LineInfo) : Expression? {
        let sn = srcName
        return <- qmacro_expr() {
            for ($i(bindName) in $i(sn)) {
                if ($i(takenName) >= $i(takeLimName)) {
                    break
                }
                $e(perElement)
            }
        }
    }
    def override build_group_by_adapter(var c : Captures; var ctx : EmitCtx; at : LineInfo) : SourceAdapter? {
        if (top._type == null) {
            return null
        }
        if (c.single |> key_exists("upstream_join")) {
            // srcA = chain top; srcB = join's second arg. plan_group_by_core consumes the ArrayJoin adapter via wrap_source_loop.
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
            var tupAType = strip_const_ref(clone_type(top._type.firstType))
            var tupBType = strip_const_ref(clone_type(srcbSrc._type.firstType))
            var topClone = clone_expression(top)
            topClone.genFlags.alwaysSafe = true
            var srcbClone = clone_expression(srcbSrc)
            srcbClone.genFlags.alwaysSafe = true
            return new ArrayJoinAdapter(shape = ArrayJoinShape(
                srcAExpr = topClone,
                srcBExpr = srcbClone,
                srcAName = qn("ajoin_srcA", at),
                srcBName = qn("ajoin_srcB", at),
                srcAType = invoke_src_param_type(top),
                srcBType = invoke_src_param_type(srcbSrc),
                tupAType = tupAType,
                tupBType = tupBType,
                keyaLam = keyaLam,
                keybLam = keybLam,
                resultLam = resultLam,
                keyType = keyType,
                resultType = resultType,
                resBindName = qn("ajoin_jres", at)))
        }
        return new ArrayAdapter(top = top, srcName = qn("source", at))
    }
}

class private DecsAdapter : SourceAdapter {
    bridge : DecsBridgeShape?
    tupName : string
    def override kind() : AdapterKind {
        return AdapterKind.k_decs
    }
    def override emit_loop_or_count(var c : Captures; var ctx : EmitCtx; at : LineInfo) : Expression? {
        return emit_loop_or_count_lane_decs(bridge, tupName, c, ctx, at)
    }
    def override emit_reverse_skip_into_tail(var takeArg : Expression?; isIter : bool; var termLam : Expression?; termElemType : TypeDeclPtr; at : LineInfo) : Expression? {
        return emit_decs_reverse_skip_into_tail(takeArg, bridge, tupName, isIter, termLam, termElemType, at)
    }
    def override emit_distinct_take_loop(bindName : string; takenName : string; takeLimName : string; var perElement : Expression?; at : LineInfo) : Expression? {
        // take(N) early-exit: for_each_archetype_find returning bool stops the archetype walk (mirrors plan_decs_distinct).
        let archName = bridge.archName
        var innerBody : Expression? = qmacro_block() {
            break if ($i(takenName) >= $i(takeLimName))
            $e(perElement)
        }
        var prunedFor = build_decs_inner_for_pruned(bridge, tupName, innerBody, at)
        let br = bridge
        return <- qmacro_expr() {
            for_each_archetype_find($e(br.reqHashExpr), $e(br.erqExpr), $($i(archName) : Archetype) : bool {
                $e(prunedFor)
                return $i(takenName) >= $i(takeLimName)
            })
        }
    }
    def override build_group_by_adapter(var c : Captures; var ctx : EmitCtx; at : LineInfo) : SourceAdapter? {
        if (c.single |> key_exists("upstream_join")) {
            var joinCall = c.single["upstream_join"]
            var bridgeB = extract_decs_bridge(joinCall.arguments[1])
            if (bridgeB == null) {
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
            var tupBType = strip_const_ref(clone_type(bridgeB.elementType))
            return new DecsJoinAdapter(shape = DecsJoinShape(
                bridgeA = bridge,
                bridgeB = bridgeB,
                keyaLam = keyaLam,
                keybLam = keybLam,
                resultLam = resultLam,
                keyType = keyType,
                tupBType = tupBType,
                resultType = resultType,
                resBindName = qn("djoin_jres", at)))
        }
        return new DecsAdapter(bridge = bridge, tupName = qn("decs_tup", at))
    }
    def override emit_decs_join_hook(var c : Captures; var ctx : EmitCtx; at : LineInfo) : Expression? {
        return emit_decs_join_impl(bridge, c, ctx, at)
    }
    def override bind_name(at : LineInfo) : string {
        return qn("decs_tup", at)
    }
    def override element_type() : TypeDeclPtr {
        if (bridge == null) {
            return null
        }
        return clone_type(bridge.elementType)
    }
    def override wrap_source_loop(var body : Expression?; at : LineInfo) : Expression? {
        var forExprNode = build_decs_inner_for_pruned(bridge, tupName, body, at)
        let archName = bridge.archName
        let br = bridge
        return <- qmacro_expr() {
            for_each_archetype($e(br.reqHashExpr), $e(br.erqExpr), $($i(archName) : Archetype) {
                $e(forExprNode)
            })
        }
    }
    def override wrap_invoke(var stmts : array<Expression?>; retType : TypeDeclPtr; wrapIter : bool; at : LineInfo) : Expression? {
        var emission : Expression?
        if (retType != null) {
            emission = qmacro(invoke($() : $t(retType) {
                $b(stmts)
            }))
        } else {
            emission = qmacro(invoke($() {
                $b(stmts)
            }))
        }
        return finalize_decs_emission(emission, at, wrapIter)
    }
}

class private DecsJoinAdapter : SourceAdapter {
    shape : DecsJoinShape
    def override kind() : AdapterKind {
        return AdapterKind.k_decs_join
    }
    def override bind_name(at : LineInfo) : string {
        return shape.resBindName
    }
    def override element_type() : TypeDeclPtr {
        return clone_type(shape.resultType)
    }
    def override wrap_source_loop(var body : Expression?; at : LineInfo) : Expression? {
        let jhashName = qn("djoin_jhash", at)
        let arrName   = qn("djoin_jarr", at)
        let tupAName  = qn("djoin_jta", at)
        let tupBName  = qn("djoin_jtb", at)
        let bridgeA = shape.bridgeA
        let bridgeB = shape.bridgeB
        var tupBindA = build_decs_tup_bind(bridgeA, tupAName, at)
        var tupBindB = build_decs_tup_bind(bridgeB, tupBName, at)
        if (tupBindA == null || tupBindB == null) {
            return null
        }
        var pieces = build_join_adapter_pieces(shape.keyaLam, shape.keybLam, shape.resultLam, tupAName, tupBName, jhashName, arrName, shape.resBindName, shape.tupBType, shape.resultType, body, at)
        if (pieces == null) {
            return null
        }
        var collectInner = build_decs_inner_for(bridgeB, tupBindB, pieces.collectInner, at)
        var probeInner = build_decs_inner_for(bridgeA, tupBindA, pieces.probeOuter, at)
        let keyType = shape.keyType
        let tupBType = shape.tupBType
        return <- qmacro_block() {
            var $i(jhashName) : table<$t(keyType); array<$t(tupBType)>>
            for_each_archetype($e(bridgeB.reqHashExpr), $e(bridgeB.erqExpr), $($i(bridgeB.archName) : Archetype) {
                $e(collectInner)
            })
            for_each_archetype($e(bridgeA.reqHashExpr), $e(bridgeA.erqExpr), $($i(bridgeA.archName) : Archetype) {
                $e(probeInner)
            })
        }
    }
    def override wrap_invoke(var stmts : array<Expression?>; retType : TypeDeclPtr; wrapIter : bool; at : LineInfo) : Expression? {
        var emission : Expression?
        if (retType != null) {
            emission = qmacro(invoke($() : $t(retType) {
                $b(stmts)
            }))
        } else {
            emission = qmacro(invoke($() {
                $b(stmts)
            }))
        }
        return finalize_decs_emission(emission, at, wrapIter)
    }
}

class private ZipAdapter : SourceAdapter {
    shape : ZipShape
    def override kind() : AdapterKind {
        return AdapterKind.k_zip
    }
    def override bind_name(at : LineInfo) : string {
        return qn("it", at)
    }
    def override element_type() : TypeDeclPtr {
        return null
    }
    def override wrap_source_loop(var body : Expression?; at : LineInfo) : Expression? {
        let srcAName = shape.srcAName
        let srcBName = shape.srcBName
        return <- qmacro_block() {
            for (itA, itB in $i(srcAName), $i(srcBName)) {
                $e(body)
            }
        }
    }
    def override wrap_invoke(var stmts : array<Expression?>; retType : TypeDeclPtr; wrapIter : bool; at : LineInfo) : Expression? {
        let srcAName = shape.srcAName
        let srcBName = shape.srcBName
        let srcAType = shape.srcAType
        let srcBType = shape.srcBType
        let srcAExpr = shape.srcAExpr
        let srcBExpr = shape.srcBExpr
        var emission : Expression?
        if (retType != null) {
            emission = qmacro(invoke($($i(srcAName) : $t(srcAType), $i(srcBName) : $t(srcBType)) : $t(retType) {
                $b(stmts)
            }, $e(srcAExpr), $e(srcBExpr)))
        } else {
            emission = qmacro(invoke($($i(srcAName) : $t(srcAType), $i(srcBName) : $t(srcBType)) {
                $b(stmts)
            }, $e(srcAExpr), $e(srcBExpr)))
        }
        emission.force_at(at)
        emission.force_generated(true)
        let blk = (emission as ExprInvoke).arguments[0] as ExprMakeBlock
        (blk._block as ExprBlock).arguments[0].flags.can_shadow = true
        (blk._block as ExprBlock).arguments[1].flags.can_shadow = true
        if (wrapIter) {
            emission = qmacro($e(emission).to_sequence_move())
            emission.force_generated(true)
        }
        return emission
    }
}

class private ArrayJoinAdapter : SourceAdapter {
    shape : ArrayJoinShape
    def override kind() : AdapterKind {
        return AdapterKind.k_array_join
    }
    def override bind_name(at : LineInfo) : string {
        return shape.resBindName
    }
    def override element_type() : TypeDeclPtr {
        return clone_type(shape.resultType)
    }
    def override wrap_source_loop(var body : Expression?; at : LineInfo) : Expression? {
        let jhashName = qn("ajoin_jhash", at)
        let arrName   = qn("ajoin_jarr", at)
        let tupAName  = qn("ajoin_jta", at)
        let tupBName  = qn("ajoin_jtb", at)
        var pieces = build_join_adapter_pieces(shape.keyaLam, shape.keybLam, shape.resultLam, tupAName, tupBName, jhashName, arrName, shape.resBindName, shape.tupBType, shape.resultType, body, at)
        if (pieces == null) {
            return null
        }
        let keyType = shape.keyType
        let tupBType = shape.tupBType
        let srcAName = shape.srcAName
        let srcBName = shape.srcBName
        return <- qmacro_block() {
            var $i(jhashName) : table<$t(keyType); array<$t(tupBType)>>
            for ($i(tupBName) in $i(srcBName)) {
                $e(pieces.collectInner)
            }
            for ($i(tupAName) in $i(srcAName)) {
                $e(pieces.probeOuter)
            }
        }
    }
    def override wrap_invoke(var stmts : array<Expression?>; retType : TypeDeclPtr; wrapIter : bool; at : LineInfo) : Expression? {
        let srcAName = shape.srcAName
        let srcBName = shape.srcBName
        let srcAType = shape.srcAType
        let srcBType = shape.srcBType
        let srcAExpr = shape.srcAExpr
        let srcBExpr = shape.srcBExpr
        var emission : Expression?
        if (retType != null) {
            emission = qmacro(invoke($($i(srcAName) : $t(srcAType), $i(srcBName) : $t(srcBType)) : $t(retType) {
                $b(stmts)
            }, $e(srcAExpr), $e(srcBExpr)))
        } else {
            emission = qmacro(invoke($($i(srcAName) : $t(srcAType), $i(srcBName) : $t(srcBType)) {
                $b(stmts)
            }, $e(srcAExpr), $e(srcBExpr)))
        }
        emission.force_at(at)
        emission.force_generated(true)
        let blk = (emission as ExprInvoke).arguments[0] as ExprMakeBlock
        (blk._block as ExprBlock).arguments[0].flags.can_shadow = true
        (blk._block as ExprBlock).arguments[1].flags.can_shadow = true
        if (wrapIter) {
            emission = qmacro($e(emission).to_sequence_move())
            emission.force_generated(true)
        }
        return emission
    }
}

[macro_function]
def private fold_linq_default(var expr : Expression?) : Expression? {
    var blk = new ExprBlock(at = expr.at)
    var (top, calls) = flatten_linq(expr)
    if (empty(calls)) return null
    var topExpr = clone_expression(top)
    var topExprType = clone_type(top._type)
    var topValue = qmacro(source)
    var argIndex = 0
    let argMax = length(calls)
    while (argIndex < argMax) {
        {
            var cll & = unsafe(calls[argIndex])
            let newArgName = "pass_{argIndex}"
            var callName = cll._1.name
            var inplace = false
            if (cll._0._type.isIterator || cll._0._type.isGoodArrayType) {
                // Names ending in `_` (e.g. `where_`) join the suffix without a connector — actual func is `where_to_array`, not `where__to_array`.
                let sep : string = ends_with(callName, "_") ? "" : "_"
                if (argIndex == 0 && top._type.isIterator && !cll._1.noToArrayVariant) {
                    callName = "{callName}{sep}to_array"
                } elif (argIndex != 0 && cll._1.inplace) {
                    callName = "{callName}{sep}inplace"
                    inplace = true
                }
            }
            for (i in cll._1.recursive) {   // recurse into sub-pipeline args (e.g. zip's second arg)
                if (i >= 1 && i < cll._0.arguments |> length && is_linq_call(cll._0.arguments[i])) {
                    var argExpr = make_call(cll._0.at, "_fold")
                    (argExpr as ExprCallMacro).arguments |> emplace <| cll._0.arguments[i]
                    cll._0.arguments[i] = argExpr
                }
            }
            var newCall = qmacro($c(callName)($e(topValue)))
            let numArgs = cll._0.arguments |> length
            for (i in 1..numArgs) {
                (newCall as ExprCall).arguments |> emplace_new <| clone_expression(cll._0.arguments[i])
            }
            if (inplace) {
                blk.list |> emplace(newCall)
            } else {
                // Macro emits `var pass_N = call` and a later `return <- pass_N`;
                blk.list |> emplace_new <| qmacro_expr() {
                    var $i(newArgName) = $e(newCall)  // nolint:PERF009
                }
                (blk.list.back() as ExprLet).variables[0].flags.can_shadow = true
                if (argIndex != 0) {
                    blk.list |> emplace_new <| qmacro_expr() {
                        delete $e(topValue)
                    }
                }
                topValue = qmacro($i(newArgName))
            }
            argIndex ++
        }
    }
    if (expr._type.isIterator) {
        blk.list |> emplace_new <| qmacro_expr() {
            return <- $e(topValue).to_sequence_move()
        }
    } else {
        blk.list |> emplace_new <| qmacro_expr() {
            return <- $e(topValue)
        }
    }
    topExpr.genFlags.alwaysSafe = true
    var res = qmacro(invoke($(source : $t(topExprType)) { $e(blk); }, $e(topExpr)))
    (((res as ExprInvoke).arguments[0] as ExprMakeBlock)._block as ExprBlock).arguments[0].flags.can_shadow = true
    res.force_generated(true)
    return res
}

[clone(top), macro_function]
def private emit_counter_lane(var top : Expression?; srcName, accName, itName : string; names : RangeStateNames;
                              var skipExpr, takeExpr, skipWhileCond : Expression?;
                              var loopBody : Expression?; at : LineInfo) : Expression? {
    // loopBody is caller-pre-wrapped (where/binds/post-take/in-loop ranges); spec only handles prelude+prologue+tail.
    var prologue : array<Expression?>
    append_ranges_prelude(prologue, skipExpr, takeExpr, skipWhileCond, names)
    prologue |> push_from <| qmacro_block_to_array() {
        var $i(accName) = 0
    }
    var perElementStmts : array<Expression?>
    perElementStmts |> push(loopBody)
    var tailStmts <- qmacro_block_to_array() {
        return $i(accName)
    }
    var laneTops : array<Expression?>
    laneTops |> push(top)
    var laneSrcs : array<string>
    laneSrcs |> push(srcName)
    return emit_terminator_lane(TerminatorSpec(
        prologue <- prologue,
        perElementStmts <- perElementStmts,
        tailStmts <- tailStmts
    ), build_lane_adapter(laneTops, laneSrcs), at)
}

// ===== Emission-lane unification (Phase F — see linq_fold.md) =====

// Bridge for orchestrators that still take parallel (topExprs, srcNames) arrays. 1-source → Array;
// 2-source → Zip (types derived via invoke_src_param_type, matches the prior inline-invoke shape).
[macro_function]
def private build_lane_adapter(var topExprs : array<Expression?>; srcNames : array<string>) : SourceAdapter? {
    let nSrcs = length(srcNames)
    if (nSrcs != 1 && nSrcs != 2) panic("build_lane_adapter: only 1- or 2-source supported (got {nSrcs})")
    if (length(topExprs) != nSrcs) panic("build_lane_adapter: topExprs length {length(topExprs)} != srcNames length {nSrcs}")
    if (nSrcs == 1) {
        return new ArrayAdapter(top = topExprs[0], srcName = srcNames[0])
    }
    var zipShape = ZipShape(
        srcAExpr = topExprs[0],
        srcBExpr = topExprs[1],
        srcAName := srcNames[0],
        srcBName := srcNames[1],
        srcAType = invoke_src_param_type(topExprs[0]),
        srcBType = invoke_src_param_type(topExprs[1])
    )
    return new ZipAdapter(shape = zipShape)
}

[macro_function]
def private emit_accumulator_lane(
                                  opName : string;
                                  var topExprs : array<Expression?>;
                                  var projection : Expression?;
                                  var whereCond : Expression?;
                                  var postTakeWhereCond : Expression?;
                                  var intermediateBinds : array<Expression?>;
                                  var preCondStmts : array<Expression?>;
                                  var elementType : TypeDeclPtr;
                                  srcNames : array<string>;
                                  accName, itName : string;
                                  names : RangeStateNames;
                                  var skipExpr, takeExpr, skipWhileCond, takeWhileCond : Expression?;
                                  at : LineInfo
                                  ) : Expression? {
    // Ring 1 single-pass accumulator lane: sum / min / max / average / long_count.
    var valueExpr : Expression?
    if (projection != null) {
        valueExpr = clone_expression(projection)
    } else {
        valueExpr = qmacro_expr() {
            $i(itName)
        }
    }
    // Accumulator type: projection-typed if present, else source's element type. Strip
    var accType : TypeDeclPtr
    if (projection != null) {
        accType = clone_type(projection._type)
    } else {
        accType = clone_type(elementType)
    }
    if (accType != null) {
        accType.flags.constant = false
        accType.flags.ref = false
    }
    let valBindName = qn("val", at)
    let firstName = qn("first", at)
    let cntName = qn("cnt", at)
    let workhorse = ((projection != null && projection._type != null && projection._type.isWorkhorseType)
        || (projection == null && elementType != null && elementType.isWorkhorseType))
    let isDoubleAccType = accType != null && accType.baseType == Type.tDouble
    var prologue : array<Expression?>
    var perElementStmts : array<Expression?>
    var returnExpr : Expression?
    if (opName == "long_count") {
        prologue <- qmacro_block_to_array() {
            var $i(accName) : int64 = 0l
        }
        // Value is unused; mirror counter-lane discipline and bind only when projection has side effects (keeps the AST visit but discards the value).
        if (projection != null && has_sideeffects(projection)) {
            let finalBindName = qn("vfinal", at)
            perElementStmts |> push <| qmacro_expr() {
                var $i(finalBindName) = $e(projection)
            }
        }
        returnExpr = qmacro_expr() {
            $i(accName)
        }
    } elif (opName == "sum") {
        prologue <- qmacro_block_to_array() {
            var $i(accName) : $t(accType) = default<$t(accType)>
        }
        returnExpr = qmacro_expr() {
            $i(accName)
        }
    } elif (opName == "average") {
        // Matches linq.das:1545 average(iterator) — always-double accumulator, uint64 counter, empty source returns 0.0lf instead of NaN.
        prologue <- qmacro_block_to_array() {
            var $i(accName) : double = 0lf
            var $i(cntName) : uint64 = 0ul
        }
        returnExpr = qmacro_expr() {
            $i(cntName) != 0ul ? $i(accName) / double($i(cntName)) : 0lf
        }
    } elif (opName == "min" || opName == "max") {
        prologue <- qmacro_block_to_array() {
            var $i(firstName) = true
            var $i(accName) : $t(accType)
        }
        returnExpr = qmacro_expr() {
            $i(accName)
        }
    } else {
        return null
    }
    perElementStmts |> push_from <| build_accumulator_perelement_stmts(opName, accName, valBindName, firstName, cntName, valueExpr, workhorse, isDoubleAccType)
    var tailStmts : array<Expression?>
    tailStmts |> push <| qmacro_expr() {
        return $e(returnExpr)
    }
    return emit_terminator_lane(TerminatorSpec(
        prologue <- prologue,
        preCondStmts <- preCondStmts,
        intermediateBinds <- intermediateBinds,
        whereCond = whereCond,
        postTakeWhereCond = postTakeWhereCond,
        perElementStmts <- perElementStmts,
        skipExpr = skipExpr,
        takeExpr = takeExpr,
        skipWhileCond = skipWhileCond,
        takeWhileCond = takeWhileCond,
        rangeNames = names,
        tailStmts <- tailStmts
    ), build_lane_adapter(topExprs, srcNames), at)
}

[macro_function]
def private emit_early_exit_lane(
                                 opName : string;
                                 var topExprs : array<Expression?>;
                                 var projection : Expression?;
                                 var whereCond : Expression?;
                                 var postTakeWhereCond : Expression?;
                                 var intermediateBinds : array<Expression?>;
                                 var preCondStmts : array<Expression?>;
                                 var elementType : TypeDeclPtr;
                                 terminatorCall : ExprCall?;
                                 srcNames : array<string>;
                                 itName : string;
                                 names : RangeStateNames;
                                 var skipExpr, takeExpr, skipWhileCond, takeWhileCond : Expression?;
                                 at : LineInfo
                                 ) : Expression? {
    // Ring 2 early-exit lane: first / first_or_default / any / all / contains.
    var perMatchStmts : array<Expression?>
    var valueName = itName
    let projBindName = qn("vproj", at)
    if (projection != null) {
        perMatchStmts |> push <| qmacro_expr() {
            let $i(projBindName) = $e(projection)
        }
        valueName = projBindName
    }
    // Per-op (preludeStmts, per-element work appended to perMatchStmts, tailStmts).
    var preludeStmts : array<Expression?>
    var tailStmts : array<Expression?>
    if (opName == "first") {
        // Return type for first: projection's type (if any) or source's element type.
        var retType : TypeDeclPtr
        if (projection != null) {
            retType = clone_type(projection._type)
        } else {
            retType = clone_type(elementType)
        }
        if (retType != null) {
            retType.flags.constant = false
            retType.flags.ref = false
        }
        perMatchStmts |> push <| qmacro_expr() {
            return $i(valueName)
        }
        // Tail: panic + unreachable default-return (matches linq.das line 2383).
        tailStmts <- qmacro_block_to_array() {
            panic("sequence contains no elements")
            return default<$t(retType)>
        }
    } elif (opName == "first_or_default") {
        // Bind `d` once at the top of the block (eager evaluation, matches linq.das line 2397).
        let defaultName = qn("dval", at)
        preludeStmts |> push <| qmacro_expr() {
            let $i(defaultName) = $e(terminatorCall.arguments[1])
        }
        perMatchStmts |> push <| qmacro_expr() {
            return $i(valueName)
        }
        tailStmts |> push <| qmacro_expr() {
            return $i(defaultName)
        }
    } elif (opName == "any") {
        let argCount = length(terminatorCall.arguments)
        if (argCount > 1) {
            var predExpr = peel_lambda_rename_var(terminatorCall.arguments[1], valueName)
            perMatchStmts |> push <| qmacro_expr() {
                if ($e(predExpr)) {
                    return true
                }
            }
        } else {
            perMatchStmts |> push <| qmacro_expr() {
                return true
            }
        }
        tailStmts |> push <| qmacro_expr() {
            return false
        }
    } elif (opName == "all") {
        var predExpr = peel_lambda_rename_var(terminatorCall.arguments[1], valueName)
        perMatchStmts |> push <| qmacro_expr() {
            if (!$e(predExpr)) {
                return false
            }
        }
        tailStmts |> push <| qmacro_expr() {
            return true
        }
    } elif (opName == "contains") {
        // `v` is bound once at the top of the block; per-element compare uses the bound name
        let containsValName = qn("cval", at)
        preludeStmts |> push <| qmacro_expr() {
            let $i(containsValName) = $e(terminatorCall.arguments[1])
        }
        perMatchStmts |> push <| qmacro_expr() {
            if ($i(valueName) == $i(containsValName)) {
                return true
            }
        }
        tailStmts |> push <| qmacro_expr() {
            return false
        }
    } elif (opName == "last" || opName == "last_or_default") {
        // Full walk; per-match overwrites a single bind; tail returns it after found-check (panic/default for empty).
        var retType : TypeDeclPtr
        if (projection != null) {
            retType = clone_type(projection._type)
        } else {
            retType = clone_type(elementType)
        }
        if (retType != null) {
            retType.flags.constant = false
            retType.flags.ref = false
        }
        let foundName = qn("found", at)
        let lastBindName = qn("lst", at)
        preludeStmts |> push_from <| qmacro_block_to_array() {
            var $i(foundName) = false
            var $i(lastBindName) : $t(retType)
        }
        perMatchStmts |> push_from <| qmacro_block_to_array() {
            $i(foundName) = true
            $i(lastBindName) := $i(valueName)
        }
        if (opName == "last") {
            tailStmts <- qmacro_block_to_array() {
                if (!$i(foundName)) {
                    panic("sequence contains no elements")
                }
                return <- $i(lastBindName)
            }
        } else {
            // Bind `d` once at the top (eager, matches linq.das line 2621); copy/move probe matches linq.das can_copy/clone_to_move branch.
            let defaultName = qn("dval", at)
            let dvCanCopy = (terminatorCall.arguments[1]._type != null && terminatorCall.arguments[1]._type.canCopy)
            preludeStmts |> push(emit_default_bind(defaultName, terminatorCall.arguments[1], dvCanCopy))
            tailStmts <- qmacro_block_to_array() {
                if (!$i(foundName)) {
                    $e(emit_default_return(defaultName, dvCanCopy))
                }
                return <- $i(lastBindName)
            }
        }
    } elif (opName == "single" || opName == "single_or_default") {
        // Full walk; on the SECOND match `single` panics and `single_or_default` returns the default; same for empty source.
        var retType : TypeDeclPtr
        if (projection != null) {
            retType = clone_type(projection._type)
        } else {
            retType = clone_type(elementType)
        }
        if (retType != null) {
            retType.flags.constant = false
            retType.flags.ref = false
        }
        let foundName = qn("found", at)
        let bindName = qn("sng", at)
        preludeStmts |> push_from <| qmacro_block_to_array() {
            var $i(foundName) = false
            var $i(bindName) : $t(retType)
        }
        if (opName == "single") {
            perMatchStmts |> push_from <| qmacro_block_to_array() {
                if ($i(foundName)) {
                    panic("sequence contains more than one element")
                }
                $i(foundName) = true
                $i(bindName) := $i(valueName)
            }
            tailStmts <- qmacro_block_to_array() {
                if (!$i(foundName)) {
                    panic("sequence contains no elements")
                }
                return <- $i(bindName)
            }
        } else {
            let defaultName = qn("dval", at)
            let dvCanCopy = (terminatorCall.arguments[1]._type != null && terminatorCall.arguments[1]._type.canCopy)
            preludeStmts |> push(emit_default_bind(defaultName, terminatorCall.arguments[1], dvCanCopy))
            // `single_or_default` early-returns default on the SECOND match (subsequent matches don't matter).
            perMatchStmts |> push_from <| qmacro_block_to_array() {
                if ($i(foundName)) {
                    $e(emit_default_return(defaultName, dvCanCopy))
                }
                $i(foundName) = true
                $i(bindName) := $i(valueName)
            }
            tailStmts <- qmacro_block_to_array() {
                if (!$i(foundName)) {
                    $e(emit_default_return(defaultName, dvCanCopy))
                }
                return <- $i(bindName)
            }
        }
    } elif (opName == "element_at" || opName == "element_at_or_default") {
        // Counter-style early-exit: return the N-th element that survives upstream where/select. Negative index panics or returns default pre-loop.
        var retType : TypeDeclPtr
        if (projection != null) {
            retType = clone_type(projection._type)
        } else {
            retType = clone_type(elementType)
        }
        if (retType != null) {
            retType.flags.constant = false
            retType.flags.ref = false
        }
        let idxName = qn("idx", at)
        let cntName = qn("ec", at)
        preludeStmts |> push <| qmacro_expr() {
            let $i(idxName) = $e(terminatorCall.arguments[1])
        }
        if (opName == "element_at") {
            preludeStmts |> push <| qmacro_expr() {
                if ($i(idxName) < 0) {
                    panic("element index out of range")
                }
            }
        } else {
            // `var t : TT; return <- t` (move-init) works for both copyable and non-copyable element types; matches linq.das line 2547.
            let tName = qn("emptyT", at)
            preludeStmts |> push_from <| qmacro_block_to_array() {
                if ($i(idxName) < 0) {
                    var $i(tName) : $t(retType)
                    return <- $i(tName)
                }
            }
        }
        preludeStmts |> push <| qmacro_expr() {
            var $i(cntName) = 0
        }
        perMatchStmts |> push_from <| qmacro_block_to_array() {
            if ($i(cntName) == $i(idxName)) {
                return $i(valueName)
            }
            $i(cntName) ++
        }
        if (opName == "element_at") {
            let tName = qn("emptyT", at)
            tailStmts <- qmacro_block_to_array() {
                panic("element index out of range")
                var $i(tName) : $t(retType)
                return <- $i(tName)
            }
        } else {
            let tName = qn("emptyT", at)
            tailStmts |> push_from <| qmacro_block_to_array() {
                var $i(tName) : $t(retType)
                return <- $i(tName)
            }
        }
    } elif (opName == "aggregate") {
        // Peel single-return `block<(acc, x):AGG>` body inline; fall back to invoke on multi-stmt. Workhorse seed picks `=` / `return`; non-workhorse uses `<-` / `return <-` (matches linq.das:1466 static_if).
        var seedExpr = clone_expression(terminatorCall.arguments[1])
        var aggFn = clone_expression(terminatorCall.arguments[2])
        var accType = clone_type(seedExpr._type)
        if (accType != null) {
            accType.flags.constant = false
            accType.flags.ref = false
        }
        let aggAccName = qn("agg", at)
        let aggIsWorkhorse = (seedExpr._type != null && seedExpr._type.isWorkhorseType)
        if (aggIsWorkhorse) {
            preludeStmts |> push <| qmacro_expr() {
                var $i(aggAccName) : $t(accType) = $e(seedExpr)
            }
        } else {
            preludeStmts |> push <| qmacro_expr() {
                var $i(aggAccName) : $t(accType) <- $e(seedExpr)
            }
        }
        var peeled = peel_lambda_rename_2vars(aggFn, aggAccName, valueName)
        if (peeled != null) {
            if (aggIsWorkhorse) {
                perMatchStmts |> push <| qmacro_expr() {
                    $i(aggAccName) = $e(peeled)
                }
            } else {
                perMatchStmts |> push <| qmacro_expr() {
                    $i(aggAccName) <- $e(peeled)
                }
            }
        } else {
            if (aggIsWorkhorse) {
                perMatchStmts |> push <| qmacro_expr() {
                    $i(aggAccName) = invoke($e(aggFn), $i(aggAccName), $i(valueName))
                }
            } else {
                perMatchStmts |> push <| qmacro_expr() {
                    $i(aggAccName) <- invoke($e(aggFn), $i(aggAccName), $i(valueName))
                }
            }
        }
        if (aggIsWorkhorse) {
            tailStmts |> push <| qmacro_expr() {
                return $i(aggAccName)
            }
        } else {
            tailStmts |> push <| qmacro_expr() {
                return <- $i(aggAccName)
            }
        }
    } else {
        return null
    }
    return emit_terminator_lane(TerminatorSpec(
        prologue <- preludeStmts,
        preCondStmts <- preCondStmts,
        intermediateBinds <- intermediateBinds,
        whereCond = whereCond,
        postTakeWhereCond = postTakeWhereCond,
        perElementStmts <- perMatchStmts,
        skipExpr = skipExpr,
        takeExpr = takeExpr,
        skipWhileCond = skipWhileCond,
        takeWhileCond = takeWhileCond,
        rangeNames = names,
        tailStmts <- tailStmts
    ), build_lane_adapter(topExprs, srcNames), at)
}

[macro_function]
def private emit_loop_or_count_lane_decs(bridge : DecsBridgeShape?; tupName : string; var c : Captures; var ctx : EmitCtx; at : LineInfo) : Expression? {
    let hasTerm = c.single |> key_exists("term")
    var termCall : ExprCall?
    var termName : string
    if (hasTerm) {
        termCall = c.single["term"]
        termName = c.single_name["term"]
    }
    // D5 fast path: bare `count()` (1-arg, no chain) → arch.size sum, no entity walk.
    let noRangeOps = (!(c.single |> key_exists("skip")) && !(c.single |> key_exists("skip_while"))
        && !(c.single |> key_exists("take_while")) && !(c.single |> key_exists("take")))
    if (hasTerm && termName == "count" && length(termCall.arguments) == 1
            && (c.many["head"] |> empty) && noRangeOps) return emit_decs_count_archsize(bridge, at)
    // Reconstruct calls = head + range ops (canonical order) + term, so existing helpers (compute_decs_chain_info, extract_decs_ranges) work unchanged. LinqCall records come from the linqCalls registry by call_norm_name; addr() to convert ref→pointer matches flatten_linq's pattern.
    var calls : array<tuple<ExprCall?; LinqCall?>>
    calls |> reserve(length(c.many["head"]) + 5)        // head + up to 4 range slots + term
    for (call in c.many["head"]) {
        let nm = call_norm_name(call)
        if (!(linqCalls |> key_exists(nm))) return null
        var lc & = unsafe(linqCalls?[nm] ?? default<LinqCall>)
        calls |> push((call, unsafe(addr(lc))))
    }
    let rangeSlots = ["skip", "skip_while", "take_while", "take"]
    for (sn in rangeSlots) {
        if (c.single |> key_exists(sn)) {
            let scall = c.single[sn]
            let snm = call_norm_name(scall)
            if (!(linqCalls |> key_exists(snm))) return null
            var lc & = unsafe(linqCalls?[snm] ?? default<LinqCall>)
            calls |> push((scall, unsafe(addr(lc))))
        }
    }
    if (hasTerm) {
        if (!(linqCalls |> key_exists(termName))) return null
        var lc & = unsafe(linqCalls?[termName] ?? default<LinqCall>)
        calls |> push((termCall, unsafe(addr(lc))))
    }
    let intermediateEnd = hasTerm ? length(calls) - 1 : length(calls)
    var rangeInfo = extract_decs_ranges(calls, intermediateEnd, tupName, at)
    if (rangeInfo == null) return null
    let chainInfo = compute_decs_chain_info(calls, rangeInfo.chainEnd, tupName, bridge, at)
    if (chainInfo == null) return null
    if (c.single |> key_exists("post_take_where")) {
        var ptwArg = c.single["post_take_where"].arguments[1]
        if (ptwArg == null) return null
        rangeInfo.postTakeWhereCond = peel_lambda_rename_var(ptwArg, chainInfo.finalBind)
        if (rangeInfo.postTakeWhereCond == null) return null
    }
    // Terminator classification mirrors plan_decs_unroll imperative (linq_fold.das:5781-5791); differs from classify_terminator's 4-lane split because decs has dedicated min_max_by / walk / element_at emit fns with hoisted state.
    let isAccum = (termName == "count" || termName == "long_count" || termName == "sum"
        || termName == "min" || termName == "max" || termName == "average")
    let isEarlyExit = (termName == "first" || termName == "first_or_default"
        || termName == "any" || termName == "all" || termName == "contains")
    let isMinMaxBy = termName == "min_by" || termName == "max_by"
    let isWalk = (termName == "last" || termName == "last_or_default"
        || termName == "single" || termName == "single_or_default"
        || termName == "aggregate")
    let isElementAt = termName == "element_at" || termName == "element_at_or_default"
    if (isAccum) {
        var accType : TypeDeclPtr
        if (termName == "sum" || termName == "min" || termName == "max" || termName == "average") {
            if (chainInfo.selectCount == 0 || chainInfo.finalType == null) return null
            accType = clone_type(chainInfo.finalType)
        }
        return emit_decs_accumulator(bridge, termName, chainInfo, rangeInfo, calls, rangeInfo.chainEnd, termCall, accType, at)
    }
    if (isEarlyExit) return emit_decs_early_exit(bridge, termName, chainInfo, rangeInfo, calls, rangeInfo.chainEnd, termCall, at)
    if (isMinMaxBy) return emit_decs_min_max_by(bridge, termName, chainInfo, rangeInfo, calls, rangeInfo.chainEnd, termCall, at)
    if (isWalk) return emit_decs_walk_lane(bridge, termName, chainInfo, rangeInfo, calls, rangeInfo.chainEnd, termCall, at)
    if (isElementAt) return emit_decs_element_at(bridge, termName, chainInfo, rangeInfo, calls, rangeInfo.chainEnd, termCall, at)
    // Implicit to_array: only when outer expr was array-typed (caller wrote `.to_array()`, peeled by linqCalls skip=true). Iterator-typed chains cascade to tier-2 here, matches imperative plan_decs_unroll line 5813.
    if (ctx.expr_is_iterator) return null
    return emit_decs_to_array(bridge, chainInfo, rangeInfo, calls, rangeInfo.chainEnd, at)
}

[macro_function]
def private emit_loop_or_count_lane(var c : Captures; var ctx : EmitCtx; at : LineInfo) : Expression? {
    // PR C Decs-arm: route to existing emit_decs_* lane fns. State hoists above for_each_archetype (different invoke shape from array side); unifying lane fns is out of scope per D1.
    if (ctx.src->kind() == AdapterKind.k_decs) return ctx.src->emit_loop_or_count(c, ctx, at)
    var top = ctx.top
    let srcName = ctx.src->arraySrcName()
    let itName = qn("it", at)
    let accName = qn("acc", at)
    let names <- make_range_names(at)
    let hasTerminator = c.single |> key_exists("term")
    var lastName : string
    if (hasTerminator) {
        lastName = call_norm_name(c.single["term"])
    }
    let lane = hasTerminator ? classify_terminator(lastName) : LinqLane.ARRAY
    if (lane == LinqLane.UNKNOWN) return null
    let counterLane = lane == LinqLane.COUNTER
    if (top._type == null || top._type.firstType == null) return null
    var whereCond : Expression?
    var postTakeWhereCond : Expression?
    var projection : Expression?
    var intermediateBinds : array<Expression?>
    var preCondStmts : array<Expression?>
    var seenSelect = false
    var allProjectionsPure = true
    var elementType = clone_type(top._type.firstType)
    var lastBindName = itName
    for (call in c.many["head"]) {
        let opName = call_norm_name(call)
        if (opName == "where_") {
            var predicate : Expression?
            if (seenSelect) {
                // where-after-select: bind the running projection to a typed local so the
                if (has_sideeffects(projection)) return null
                if (lane != LinqLane.COUNTER) {
                    let wbName = "`vw`{at.line}`{at.column}`{length(preCondStmts)}"
                    var projType = clone_type(elementType)
                    preCondStmts |> push <| qmacro_expr() {
                        var $i(wbName) : $t(projType) := $e(projection)
                    }
                    var pvar = new ExprVar(at = at, name := wbName)
                    pvar._type = clone_type(elementType)
                    pvar._type.flags.ref = true
                    projection = pvar
                }
                predicate = peel_lambda_replace_var(call.arguments[1], projection)
            } else {
                predicate = peel_lambda_rename_var(call.arguments[1], itName)
            }
            if (whereCond == null) {
                whereCond = predicate
            } else {
                whereCond = qmacro($e(whereCond) && $e(predicate))
            }
        } elif (opName == "select") {
            if (projection != null) {
                if (has_sideeffects(projection)) {
                    allProjectionsPure = false
                }
                let bindName = "`v`{at.line}`{at.column}`{length(intermediateBinds)}"
                intermediateBinds |> push <| qmacro_expr() {
                    var $i(bindName) := $e(projection)
                }
                lastBindName = bindName
            }
            projection = peel_lambda_rename_var(call.arguments[1], lastBindName)
            elementType = clone_type(call._type.firstType)
            seenSelect = true
        } else {
            return null
        }
    }
    if (projection != null && has_sideeffects(projection)) {
        allProjectionsPure = false
    }
    var skipExpr : Expression?
    var skipWhileCond : Expression?
    var takeWhileCond : Expression?
    var takeExpr : Expression?
    if (c.single |> key_exists("skip")) {
        var skipArg = c.single["skip"].arguments[1]
        if (skipArg == null || skipArg._type == null || skipArg._type.baseType != Type.tInt) return null
        skipExpr = clone_expression(skipArg)
    }
    if (c.single |> key_exists("skip_while")) {
        if (seenSelect) return null
        var swArg = c.single["skip_while"].arguments[1]
        if (swArg == null) return null
        skipWhileCond = peel_lambda_rename_var(swArg, itName)
        if (skipWhileCond == null) return null
    }
    if (c.single |> key_exists("take_while")) {
        if (seenSelect) return null
        var twArg = c.single["take_while"].arguments[1]
        if (twArg == null) return null
        takeWhileCond = peel_lambda_rename_var(twArg, itName)
        if (takeWhileCond == null) return null
    }
    if (c.single |> key_exists("take")) {
        var takeArg = c.single["take"].arguments[1]
        if (takeArg == null || takeArg._type == null || takeArg._type.baseType != Type.tInt) return null
        takeExpr = clone_expression(takeArg)
    }
    if (c.single |> key_exists("post_take_where")) {
        var ptwArg = c.single["post_take_where"].arguments[1]
        if (ptwArg == null) return null
        if (seenSelect) {
            if (has_sideeffects(projection)) return null
            if (lane != LinqLane.COUNTER) {
                let wbName = "`vw`{at.line}`{at.column}`{length(preCondStmts)}"
                var projType = clone_type(elementType)
                preCondStmts |> push <| qmacro_expr() {
                    var $i(wbName) : $t(projType) := $e(projection)
                }
                var pvar = new ExprVar(at = at, name := wbName)
                pvar._type = clone_type(elementType)
                pvar._type.flags.ref = true
                projection = pvar
            }
            postTakeWhereCond = peel_lambda_replace_var(ptwArg, projection)
        } else {
            postTakeWhereCond = peel_lambda_rename_var(ptwArg, itName)
        }
    }
    let noLimits = skipExpr == null && takeExpr == null && skipWhileCond == null && takeWhileCond == null
    let isCountShaped = (lane == LinqLane.COUNTER
        || (lane == LinqLane.ACCUMULATOR && lastName == "long_count"))
    if (isCountShaped && whereCond == null && allProjectionsPure && noLimits
            && type_has_length(top._type))
        return emit_length_shortcut(lastName, top, srcName, at)
    if (lane == LinqLane.ACCUMULATOR) {
        var laneTops : array<Expression?>
        laneTops |> push(top)
        var laneSrcs : array<string>
        laneSrcs |> push(srcName)
        return emit_accumulator_lane(lastName, laneTops, projection, whereCond, postTakeWhereCond,
            intermediateBinds, preCondStmts, elementType, laneSrcs, accName, itName, names,
            skipExpr, takeExpr, skipWhileCond, takeWhileCond, at)
    }
    if (lane == LinqLane.EARLY_EXIT) {
        let terminatorCall = c.single["term"]
        let isAnyNoPred = lastName == "any" && length(terminatorCall.arguments) == 1
        if (isAnyNoPred && whereCond == null && allProjectionsPure && noLimits
                && type_has_length(top._type))
            return emit_any_empty_shortcut(top, srcName, at)
        var laneTops : array<Expression?>
        laneTops |> push(top)
        var laneSrcs : array<string>
        laneSrcs |> push(srcName)
        return emit_early_exit_lane(lastName, laneTops, projection, whereCond, postTakeWhereCond,
            intermediateBinds, preCondStmts, elementType, terminatorCall, laneSrcs, itName, names,
            skipExpr, takeExpr, skipWhileCond, takeWhileCond, at)
    }
    var loopBody : Expression?
    if (counterLane) {
        var stmts : array<Expression?>
        if (projection != null && has_sideeffects(projection)) {
            let finalBindName = qn("vfinal", at)
            stmts |> push <| qmacro_expr() {
                var $i(finalBindName) = $e(projection)
            }
        }
        var incExpr = qmacro_expr() {
            $i(accName) ++
        }
        stmts |> push(wrap_with_condition(incExpr, postTakeWhereCond))
        prepend_binds(stmts, intermediateBinds)
        wrap_with_ranges(stmts, skipExpr, takeExpr, skipWhileCond, takeWhileCond, names)
        loopBody = prepend_precond(wrap_with_condition(stmts_to_expr(stmts), whereCond), preCondStmts)
    } else {
        // Range-prelude in prologueStmts so the lane emits it BEFORE bufDecl — matches the emit_array_lane shape.
        var stmts : array<Expression?>
        var pushExpr : Expression?
        if (projection != null) {
            pushExpr = qmacro_expr() {
                $i(accName) |> push_clone($e(projection))
            }
        } elif (whereCond != null || postTakeWhereCond != null || skipExpr != null || takeExpr != null
                || skipWhileCond != null || takeWhileCond != null) {
            pushExpr = qmacro_expr() {
                $i(accName) |> push_clone($i(itName))
            }
        } else {
            return null
        }
        stmts |> push(wrap_with_condition(pushExpr, postTakeWhereCond))
        prepend_binds(stmts, intermediateBinds)
        wrap_with_ranges(stmts, skipExpr, takeExpr, skipWhileCond, takeWhileCond, names)
        var perElementPush = stmts_to_expr(stmts)
        var prologueStmts : array<Expression?>
        append_ranges_prelude(prologueStmts, skipExpr, takeExpr, skipWhileCond, names)
        var reserveStmts : array<Expression?>
        let sourceHasLength = type_has_length(top._type)
        if (sourceHasLength && !ctx.expr_is_iterator) {
            if (takeExpr != null) {
                reserveStmts |> push <| qmacro_expr() {
                    $i(accName) |> reserve($e(takeExpr) < length($i(srcName)) ? $e(takeExpr) : length($i(srcName)))
                }
            } else {
                reserveStmts |> push <| qmacro_expr() {
                    $i(accName) |> reserve(length($i(srcName)))
                }
            }
        }
        var tailStmts : array<Expression?>
        tailStmts |> push(buffer_return(accName, ctx.expr_is_iterator))
        return emit_fold_array_lane(FoldArraySpec(
            prologueStmts <- prologueStmts,
            bufElemType = elementType,
            bufName = accName,
            reserveStmts <- reserveStmts,
            preCondStmts <- preCondStmts,
            whereCond = whereCond,
            perElementPush = perElementPush,
            tailStmts <- tailStmts,
            wrapIter = false
        ), new ArrayAdapter(top = top, srcName = srcName), at)
    }
    return emit_counter_lane(top, srcName, accName, itName, names,
        skipExpr, takeExpr, skipWhileCond, loopBody, at)
}

[macro_function]
def build_loop_or_count_rows {
    // Single row — c_chain head matches the where_/select run (0+ contiguous calls), then canonical-order range ops, optional post-take where, optional terminator from loop_terminator_family.
    splice_patterns |> emplace <| SplicePattern(
        name = "loop_or_count_general",
        chain <- [
            slot_chain_of(["where_", "select"], "head"),
            Slot(matcher = m_literal("skip"),       cardinality = c_opt(), capture_name = "skip"),
            Slot(matcher = m_literal("skip_while"), cardinality = c_opt(), capture_name = "skip_while"),
            Slot(matcher = m_literal("take_while"), cardinality = c_opt(), capture_name = "take_while"),
            Slot(matcher = m_literal("take"),       cardinality = c_opt(), capture_name = "take"),
            Slot(matcher = m_literal("where_"),     cardinality = c_opt(), capture_name = "post_take_where"),
            Slot(matcher = m_alias("loop_terminator_family"), cardinality = c_opt(), capture_name = "term")
        ],
        requires <- array<RequiresPredicate>(),
        emit = @@ < EmitFn > emit_loop_or_count_lane
    )
}

// ===== plan_reverse =====

// Decs `reverse |> take(N) [|> _select(F)] |> to_array` skip-into-tail: 2-pass walk (arch.size sum, then for_each_archetype_find with whole-arch-skip + partial-arch skip-counter + early-exit). Preserves PR #2834's 5.2× gain. When terminalSelectLam != null, projects the K reversed elements at the end (K push_clones of projection vs N push_clones of full tuple in the catch-all path).
// `terminalSelectElemType` must be the typer-resolved element type of the termsel call result (caller reads it from `c.single["termsel"]._type.firstType`) — peel_lambda_rename_var's invoke-fallback shape for non-peelable lambdas has a null `_type` at macro stage, so deriving projType from the peeled body would mis-classify those as untyped and bail. Caller is responsible for ensuring termselElemType is non-null before this fast-path is selected; otherwise the gate falls through to the catch-all which has its own termsel handling.
[macro_function]
def private emit_decs_reverse_skip_into_tail(var takeExpr : Expression?; bridge : DecsBridgeShape?; tupName : string; needIterWrap : bool; var terminalSelectLam : Expression?; terminalSelectElemType : TypeDeclPtr; at : LineInfo) : Expression? {
    let bufName = qn("buf", at)
    let takeNName = qn("take_n", at)
    let totalName = qn("decs_total", at)
    let actualName = qn("decs_actual", at)
    let skipName = qn("decs_skip", at)
    let seenName = qn("decs_seen", at)
    let skipsLeftName = qn("decs_skips", at)
    let projElemName = qn("decs_rts_e", at)
    let projBufName = qn("decs_rts_buf", at)
    let archName = bridge.archName
    var bufElemType = strip_const_ref(clone_type(bridge.elementType))
    let tupBind = build_decs_tup_bind(bridge, tupName, at)
    var projBody : Expression?
    var projType : TypeDeclPtr
    if (terminalSelectLam != null) {
        projBody = peel_lambda_rename_var(terminalSelectLam, projElemName)
        if (projBody == null) return null
        projType = strip_const_ref(clone_type(terminalSelectElemType))
        if (projType == null) return null
    }
    var innerBody : Expression? = qmacro_block() {
        if ($i(skipsLeftName) > 0_l) {
            $i(skipsLeftName) --
            continue
        }
        $e(tupBind)
        $i(bufName) |> push_clone($i(tupName))
        if (long_length($i(bufName)) >= $i(actualName)) {
            break
        }
    }
    var clonedForExpr = clone_expression(bridge.forExpr)
    var clonedFor = clonedForExpr as ExprFor
    var newForBody = new ExprBlock(at = at)
    newForBody.list |> push(innerBody)
    clonedFor.body = newForBody
    var emission : Expression?
    if (terminalSelectLam == null) {
        emission = qmacro(invoke($() : array<$t(bufElemType)> {
            var $i(totalName) = 0_l
            for_each_archetype($e(bridge.reqHashExpr), $e(bridge.erqExpr), $($i(archName) : Archetype) {
                $i(totalName) += $i(archName).size
            })
            let $i(takeNName) = $e(takeExpr)
            let $i(actualName) = ($i(takeNName) <= 0) ? 0_l : ((int64($i(takeNName)) < $i(totalName)) ? int64($i(takeNName)) : $i(totalName))
            let $i(skipName) = $i(totalName) - $i(actualName)
            var $i(bufName) : array<$t(bufElemType)>
            if ($i(actualName) == 0_l) {
                return <- $i(bufName)
            }
            $i(bufName) |> reserve(int($i(actualName)))
            var $i(seenName) = 0_l
            for_each_archetype_find($e(bridge.reqHashExpr), $e(bridge.erqExpr), $($i(archName) : Archetype) : bool {
                if ($i(seenName) + $i(archName).size <= $i(skipName)) {
                    $i(seenName) += $i(archName).size
                    return false
                }
                var $i(skipsLeftName) = ($i(skipName) > $i(seenName)) ? ($i(skipName) - $i(seenName)) : 0_l
                $e(clonedForExpr)
                $i(seenName) += $i(archName).size
                return long_length($i(bufName)) >= $i(actualName)
            })
            _::reverse_inplace($i(bufName))
            return <- $i(bufName)
        }))
    } else {
        emission = qmacro(invoke($() : array<$t(projType)> {
            var $i(totalName) = 0_l
            for_each_archetype($e(bridge.reqHashExpr), $e(bridge.erqExpr), $($i(archName) : Archetype) {
                $i(totalName) += $i(archName).size
            })
            let $i(takeNName) = $e(takeExpr)
            let $i(actualName) = ($i(takeNName) <= 0) ? 0_l : ((int64($i(takeNName)) < $i(totalName)) ? int64($i(takeNName)) : $i(totalName))
            let $i(skipName) = $i(totalName) - $i(actualName)
            var $i(bufName) : array<$t(bufElemType)>
            var $i(projBufName) : array<$t(projType)>
            if ($i(actualName) == 0_l) {
                return <- $i(projBufName)
            }
            $i(bufName) |> reserve(int($i(actualName)))
            var $i(seenName) = 0_l
            for_each_archetype_find($e(bridge.reqHashExpr), $e(bridge.erqExpr), $($i(archName) : Archetype) : bool {
                if ($i(seenName) + $i(archName).size <= $i(skipName)) {
                    $i(seenName) += $i(archName).size
                    return false
                }
                var $i(skipsLeftName) = ($i(skipName) > $i(seenName)) ? ($i(skipName) - $i(seenName)) : 0_l
                $e(clonedForExpr)
                $i(seenName) += $i(archName).size
                return long_length($i(bufName)) >= $i(actualName)
            })
            _::reverse_inplace($i(bufName))
            $i(projBufName) |> reserve(length($i(bufName)))
            for ($i(projElemName) in $i(bufName)) {
                $i(projBufName) |> push_clone($e(projBody))
            }
            return <- $i(projBufName)
        }))
    }
    return finalize_decs_emission(emission, at, needIterWrap)
}

[macro_function]
def private build_decs_tup_bind(bridge : DecsBridgeShape?; tupName : string; at : LineInfo) : Expression? {
    // Named-tuple bind: `var tup = (n1=iter1, n2=iter2, ...)` — peel_lambda_rename_var peels user lambdas substituting `_.userName` → `tup.userName`.
    var mkTup = new ExprMakeTuple(at = at)
    mkTup.recordNames |> resize(length(bridge.userNames))
    for (i in 0 .. length(bridge.userNames)) {
        mkTup.recordNames[i] := bridge.userNames[i]
        mkTup.values |> emplace_new(new ExprVar(at = at, name := bridge.iterNames[i]))
    }
    return qmacro_expr() {
        var $i(tupName) = $e(mkTup)
    }
}

[macro_function]
def private build_decs_inner_for(bridge : DecsBridgeShape?; var tupBind : Expression?; var body : Expression?; at : LineInfo) : Expression? {
    // Clone bridge's inner multi-iter for-loop; substitute body with [tupBind, body]. Reuses bridge.archName so cloned get_ro sources stay valid.
    var forBodyStmts <- [tupBind, body]
    var forBody = stmts_to_expr(forBodyStmts)
    var clonedForExpr = clone_expression(bridge.forExpr)
    var clonedFor = clonedForExpr as ExprFor
    var newForBody = new ExprBlock(at = at)
    newForBody.list |> push(forBody)
    clonedFor.body = newForBody
    return clonedForExpr
}

// Walks a chain body to collect which fields of the decs_tup named-tuple bind are actually referenced. allFieldsUsed flips when the bind is referenced as a whole-var (not via field access) — e.g. push_clone(decs_tup) in a bare to_array, or pass-to-user-fn — so the helper falls back to unpruned emission and preserves the user-visible output shape. Also picks up bare iter-var refs (when `iterToUser` is non-empty) so the trivial-let-elision path — which rewrites `decs_sel_N`-bound projections to the iter var directly, bypassing `decs_tup` entirely — still hits the pruned arm instead of falling through to unpruned-default.
class private DecsTupUsageScanner : AstVisitor {
    tupName       : string
    iterToUser    : table<string; string>
    usedFields    : table<string>
    allFieldsUsed : bool = false
    inTargetField : bool = false
    def DecsTupUsageScanner(n : string; var i2u : table<string; string>) {
        tupName = n
        iterToUser <- i2u
    }
    def override preVisitExprField(expr : ExprField?) : void {
        var v = expr.value
        if (v is ExprRef2Value) {
            v = (v as ExprRef2Value).subexpr
        }
        if (v is ExprVar && (v as ExprVar).name == tupName) {
            usedFields |> insert(string(expr.name))
            inTargetField = true
        }
    }
    def override visitExprField(var expr : ExprField?) : ExpressionPtr {
        inTargetField = false
        return expr
    }
    def override preVisitExprVar(expr : ExprVar?) : void {
        if (expr.name == tupName && !inTargetField) {
            allFieldsUsed = true
            return
        }
        let nm = string(expr.name)
        if (key_exists(iterToUser, nm)) {
            usedFields |> insert(iterToUser[nm])
        }
    }
}

[macro_function]
def private collect_decs_tup_usage(var e : Expression?; tupName : string; bridge : DecsBridgeShape?) : tuple<bool; array<string>> {
    // Returns (allFieldsUsed, usedFieldNames). usedFieldNames is unordered — caller filters bridge.userNames in original order to preserve get_ro emission order. The bridge feeds an iter-var → user-name reverse map so bare iter-var refs (from trivial-let elision) count as field usage too.
    var allUsed = false
    var used : array<string>
    if (e == null) return (allUsed, used)
    var iterToUser <- {for (i in 0 .. length(bridge.iterNames)); bridge.iterNames[i] => bridge.userNames[i]}
    var sc = new DecsTupUsageScanner(tupName, iterToUser)
    make_visitor(*sc) $(astVisitorAdapter) {
        visit_expression(e, astVisitorAdapter)
    }
    allUsed = sc.allFieldsUsed
    for (k in keys(sc.usedFields)) {
        used |> push(k)
    }
    unsafe {
        delete sc
    }
    return (allUsed, used)
}

// Rewrites every whole-var ref to `decs_tup` into a freshly synthesized `(userName1 = iterVar1, ...)` named-tuple literal — same shape build_decs_tup_bind produces, but inlined at each use site. Lets emitters that store the whole element (last/single/aggregate seed of element type, min_by/max_by, to_array push_clone) avoid forcing the unpruned bind path: after rewrite the body contains only `decs_tup.<field>` refs (flattenable) plus iter-var refs in the synthesized literals (picked up by the usage scanner). build_decs_inner_for_pruned then flattens + drops the bind entirely. Inside ExprField the inner ExprVar is left alone so the field path still matches.
class private DecsTupWholeRewriter : AstVisitor {
    tupName       : string
    userNames     : array<string>
    iterNames     : array<string>
    inTargetField : bool = false
    rewroteAny    : bool = false
    def DecsTupWholeRewriter(t : string; var un, in_ : array<string>) {
        tupName = t
        userNames <- un
        iterNames <- in_
    }
    def override preVisitExprField(expr : ExprField?) : void {
        var v = expr.value
        if (v is ExprRef2Value) {
            v = (v as ExprRef2Value).subexpr
        }
        if (v is ExprVar && (v as ExprVar).name == tupName) {
            inTargetField = true
        }
    }
    def override visitExprField(var expr : ExprField?) : ExpressionPtr {
        inTargetField = false
        return expr
    }
    def override visitExprVar(var expr : ExprVar?) : ExpressionPtr {
        if (expr.name != tupName || inTargetField) return expr
        var mkTup = new ExprMakeTuple(at = expr.at)
        mkTup.recordNames |> resize(length(userNames))
        for (i in 0 .. length(userNames)) {
            mkTup.recordNames[i] := userNames[i]
            mkTup.values |> emplace_new(new ExprVar(at = expr.at, name := iterNames[i]))
        }
        rewroteAny = true
        return mkTup
    }
}

[macro_function]
def private rewrite_decs_tup_wholes(var body : Expression?; tupName : string; bridge : DecsBridgeShape?) : bool {
    if (body == null) return false
    var unCopy <- [for (n in bridge.userNames); n]
    var inCopy <- [for (n in bridge.iterNames); n]
    var rw = new DecsTupWholeRewriter(tupName, unCopy, inCopy)
    make_visitor(*rw) $(astVisitorAdapter) {
        visit_expression(body, astVisitorAdapter)
    }
    let any = rw.rewroteAny
    unsafe {
        delete rw
    }
    return any
}

// When the body only references `decs_tup` via field access (no whole-var refs), the named-tuple wrap plus every per-element `decs_tup.<field>` read are pure overhead — rewrite each field access to the matching iter var directly and skip the bind. Drops one tuple-make + one field-read per kept field per iteration.
class private DecsTupFieldFlattener : AstVisitor {
    tupName     : string
    fieldToIter : table<string; string>
    def DecsTupFieldFlattener(t : string; var f : table<string; string>) {
        tupName = t
        fieldToIter <- f
    }
    def override visitExprField(var expr : ExprField?) : ExpressionPtr {
        var v = expr.value
        if (v is ExprRef2Value) {
            v = (v as ExprRef2Value).subexpr
        }
        if (v is ExprVar && (v as ExprVar).name == tupName) {
            let fname = string(expr.name)
            if (key_exists(fieldToIter, fname)) return new ExprVar(at = expr.at, name := fieldToIter[fname])
        }
        return expr
    }
}

[macro_function]
def private flatten_decs_tup_to_iter(var body : Expression?; tupName : string; var fieldToIter : table<string; string>) : void {
    if (body == null) return
    var fl = new DecsTupFieldFlattener(tupName, fieldToIter)
    make_visitor(*fl) $(astVisitorAdapter) {
        visit_expression(body, astVisitorAdapter)
    }
    unsafe {
        delete fl
    }
}

[macro_function]
def private build_decs_inner_for_pruned(bridge : DecsBridgeShape?;
                                        tupName : string;
                                        var body : Expression?;
                                        at : LineInfo) : Expression? {
    // Walks body for `tupName.<field>` references; when pruning is safe + beneficial, emits the inner multi-iter for with unused get_ro slots dropped and a matching shrunk named-tuple bind. Otherwise falls through to the unpruned path so user-visible shape stays intact (bare to_array, push_clone(decs_tup), pass-to-user-fn).
    var (allUsed, usedNames) = collect_decs_tup_usage(body, tupName, bridge)
    // Whole-var refs (`lst := decs_tup`, `push_clone(buf, decs_tup)`, etc) would force the unpruned bind path — but they're all semantically equivalent to a synthesized `(userName1=iter1, ...)` literal. Rewrite them inline so the flatten + bind-elision path unlocks; saves the per-iteration `var decs_tup = (...)` wrap (~12 ns/op on last/single/select_where 100K-row benches).
    if (allUsed && rewrite_decs_tup_wholes(body, tupName, bridge)) {
        var (allUsed2, usedNames2) = collect_decs_tup_usage(body, tupName, bridge)
        allUsed = allUsed2
        usedNames <- usedNames2
    }
    // Fallback to the unpruned bind ONLY for whole-var refs that survived rewrite (pass-to-user-fn) or the edge case where the body never touches decs_tup at all. The "all fields used via field access" case still benefits from flatten + bind elision — no slots dropped, but the per-iter tuple-make and field reads disappear.
    if (allUsed || empty(usedNames)) {
        var tupBind = build_decs_tup_bind(bridge, tupName, at)
        return build_decs_inner_for(bridge, tupBind, body, at)
    }
    var usedSet : table<string>
    for (n in usedNames) {
        usedSet |> insert(n)
    }
    var fieldToIter <- {for (i in 0 .. length(bridge.userNames));
        bridge.userNames[i] => bridge.iterNames[i];
        where key_exists(usedSet, bridge.userNames[i])}
    // Diagnostic-friendly fallback when some `usedNames` entry has no bridge match. Today the typer pipeline checks user `_.<field>` lambdas before plan_decs_unroll runs, so this is unreachable in practice — but if a future macro ever synthesizes `decs_tup.<unknownField>`, the unpruned bind keeps the typer's "unknown field of <named-tuple>" diagnostic intact instead of degrading it to "unknown identifier decs_tup".
    if (length(fieldToIter) < length(usedSet)) {
        var tupBind = build_decs_tup_bind(bridge, tupName, at)
        return build_decs_inner_for(bridge, tupBind, body, at)
    }
    // Pruned path: no whole-var ref → no need for the named tuple. Rewrite every `decs_tup.<userName>` in the body to the matching iter var, then drop the bind entirely. Eliminates one tuple-make + N per-element field reads per iter.
    flatten_decs_tup_to_iter(body, tupName, fieldToIter)
    // Clone forExpr; drop the iter slots not in usedSet from all 5 parallel dasvectors (sources/iterators/iteratorsAt/iteratorsAka/iteratorsTags). iteratorVariables is cleared so the typer rebuilds it during re-type — same pattern as daslib/soa.das:381.
    var clonedForExpr = clone_expression(bridge.forExpr)
    var clonedFor = clonedForExpr as ExprFor
    let nSlots = length(bridge.userNames)
    for (i in 0 .. nSlots) {
        let revIdx = nSlots - 1 - i
        if (!key_exists(usedSet, bridge.userNames[revIdx])) {
            clonedFor.sources       |> erase(revIdx)
            clonedFor.iterators     |> erase(revIdx)
            clonedFor.iteratorsAt   |> erase(revIdx)
            clonedFor.iteratorsAka  |> erase(revIdx)
            clonedFor.iteratorsTags |> erase(revIdx)
        }
    }
    clonedFor.iteratorVariables |> clear()
    // Install body — body now references iter vars directly; no tupBind needed.
    var newForBody = new ExprBlock(at = at)
    newForBody.list |> push(body)
    clonedFor.body = newForBody
    return clonedForExpr
}

struct private DecsChainInfo {
    bindAt      : array<string>    // bind name visible at each chain position
    elidedAt    : array<bool>      // true when the select at position i was elided (peeled to a single decs_tup field access → reused iter var as next bind, no `let decs_sel_N = ...` emit needed)
    finalBind   : string            // bind name AFTER full chain — what terminator references
    finalType   : TypeDeclPtr       // element type AFTER full chain (constant + ref stripped)
    selectCount : int               // number of `select` ops in chain; 0 means finalBind == tupName
}

[macro_function]
def private compute_decs_chain_info(var calls : array<tuple<ExprCall?; LinqCall?>>;
                                    intermediateEnd : int;
                                    tupName : string;
                                    bridge : DecsBridgeShape?;
                                    at : LineInfo) : DecsChainInfo? {
    // Each select introduces a fresh `decs_sel{N}` bind; subsequent ops peel against it. Returns null on any non-where_/select op.
    var info = new DecsChainInfo(
        finalBind := tupName,
        finalType = clone_type(bridge.elementType),
        selectCount = 0
    )
    info.bindAt |> reserve(intermediateEnd)
    info.elidedAt |> reserve(intermediateEnd)
    var curBind = tupName
    var curType : TypeDeclPtr = clone_type(bridge.elementType)
    for (i in 0 .. intermediateEnd) {
        info.bindAt |> push(curBind)
        info.elidedAt |> push(false)
        var cll & = unsafe(calls[i])
        let opName = cll._1.name
        if (opName == "select") {
            info.selectCount ++
            var peeled = peel_lambda_rename_var(cll._0.arguments[1], info.bindAt[i])
            if (peeled == null || peeled._type == null) return null
            // Trivial-let elision: `_select(_.userName)` against decs_tup → iter var directly. The pruned-for keeps the iter var (its name = bridge.iterNames[idx]) and the flattener would otherwise rewrite the synthetic `let decs_sel_N = decs_tup.userName` to `let decs_sel_N = <iterVar>` — a pure copy. Renaming finalBind to the iter var name skips the binding entirely and the action references it natively. Typer wraps both the projection root (`peeled` may be ExprRef2Value) AND the ExprVar inside the ExprField in ExprRef2Value — peel both.
            var elided = false
            if (curBind == tupName) {
                var top = peeled
                if (top is ExprRef2Value) {
                    top = (top as ExprRef2Value).subexpr
                }
                if (top is ExprField) {
                    var pf = top as ExprField
                    var pv = pf.value
                    if (pv is ExprRef2Value) {
                        pv = (pv as ExprRef2Value).subexpr
                    }
                    if (pv is ExprVar && (pv as ExprVar).name == tupName) {
                        let fname = string(pf.name)
                        for (ui in 0 .. length(bridge.userNames)) {
                            if (bridge.userNames[ui] == fname) {
                                curBind = bridge.iterNames[ui]
                                elided = true
                                info.elidedAt[i] = true
                                break
                            }
                        }
                    }
                }
            }
            if (!elided) {
                curBind = "`decs_sel`{at.line}`{at.column}`{info.selectCount}"
            }
            curType = clone_type(peeled._type)
        } elif (opName != "where_") return null
    }
    info.finalBind := curBind
    info.finalType = curType
    if (info.finalType != null) {
        info.finalType.flags.constant = false
        info.finalType.flags.ref = false
    }
    return info
}

[macro_function]
def private wrap_decs_chain(var action : Expression?;
                            chainInfo : DecsChainInfo?;
                            var calls : array<tuple<ExprCall?; LinqCall?>>;
                            intermediateEnd : int;
                            at : LineInfo) : Expression? {
    // Reverse-walk chain, wrapping action with `if (pred) { ... }` for where_ and `let bindN+1 = proj; ...` for select.
    var current = action
    for (rev in 0 .. intermediateEnd) {
        let i = intermediateEnd - 1 - rev
        var cll & = unsafe(calls[i])
        let opName = cll._1.name
        let bindHere = chainInfo.bindAt[i]
        if (opName == "where_") {
            var pred = peel_lambda_rename_var(cll._0.arguments[1], bindHere)
            if (pred == null) return null
            current = qmacro_expr() {
                if ($e(pred)) {
                    $e(current)
                }
            }
        } elif (opName == "select") {
            // Skip emission entirely when this select was elided in compute_decs_chain_info — the next bind is already the iter var the flattener would have produced for `let nextBind = decs_tup.userName`. No copy needed; downstream `current` already references the iter var by the elided name.
            continue if (chainInfo.elidedAt[i])
            var proj = peel_lambda_rename_var(cll._0.arguments[1], bindHere)
            if (proj == null) return null
            let nextBind = (i + 1 < intermediateEnd) ? chainInfo.bindAt[i + 1] : chainInfo.finalBind
            var letStmt = qmacro_expr() {
                let $i(nextBind) = $e(proj)
            }
            var stmts : array<Expression?>
            stmts |> reserve(2)
            stmts |> push(letStmt)
            stmts |> push(current)
            current = stmts_to_expr(stmts)
        } else {
            return null
        }
    }
    return current
}

struct private DecsRangeInfo {
    skipExpr          : Expression?
    takeExpr          : Expression?
    skipWhileCond     : Expression?    // Slice 5b: predicate peeled against tupName (source tuple)
    takeWhileCond     : Expression?    // Slice 5b: predicate peeled against tupName (source tuple)
    postTakeWhereCond : Expression?    // gates per-element work AFTER take cap ticks; peeled against chainInfo.finalBind (mirrors array-side postTakeWhereCond, Theme 2 5c)
    chainEnd          : int
}

struct private DecsRangeStateNames {
    skipName      : string
    takeCountName : string
    takeLimitName : string    // decs adds an explicit take-limit local so the take(N) expression evaluates exactly once at invoke entry.
    skippingName  : string
}

[macro_function]
def private make_decs_range_names(at : LineInfo) : DecsRangeStateNames {
    return <- DecsRangeStateNames(
        skipName      = qn("decs_skip", at),
        takeCountName = qn("decs_takec", at),
        takeLimitName = qn("decs_takel", at),
        skippingName  = qn("decs_skipping", at))
}

[macro_function]
def private extract_decs_ranges(var calls : array<tuple<ExprCall?; LinqCall?>>;
                                intermediateEnd : int;
                                tupName : string;
                                at : LineInfo) : DecsRangeInfo? {
    // Slice 5a/5b: split intermediate chain into (where_/select prefix) + (skip? skip_while? take_while? take? suffix). Canonical order matches array-side (linq_fold.das:1607-1634): skip → skip_while → take_while → take, all after any where_. skip_while/take_while predicates use the SOURCE tuple (tupName); bail when a select appears in the prefix (matches array-side seenSelect bail).
    var info = new DecsRangeInfo(chainEnd = intermediateEnd)
    var firstRangeIdx = intermediateEnd
    for (i in 0 .. intermediateEnd) {
        let nm = calls[i]._1.name
        if (nm == "take" || nm == "skip" || nm == "take_while" || nm == "skip_while") {
            firstRangeIdx = i
            break
        }
    }
    if (firstRangeIdx == intermediateEnd) return info
    // Detect whether the suffix contains any predicate-driven range (skip_while/take_while) — if so, prefix must be select-free (mirrors array-side seenSelect bail at linq_fold.das:1615/1623).
    var hasWhilePred = false
    for (i in firstRangeIdx .. intermediateEnd) {
        let nm = calls[i]._1.name
        if (nm == "skip_while" || nm == "take_while") {
            hasWhilePred = true
            break
        }
    }
    if (hasWhilePred) {
        for (i in 0 .. firstRangeIdx) {
            if (calls[i]._1.name == "select") return null
        }
    }
    for (i in firstRangeIdx .. intermediateEnd) {
        var cll & = unsafe(calls[i])
        let nm = cll._1.name
        if (nm == "skip") {
            if (info.skipExpr != null || info.skipWhileCond != null || info.takeWhileCond != null || info.takeExpr != null || length(cll._0.arguments) < 2) return null
            var arg = cll._0.arguments[1]
            if (arg == null || arg._type == null || arg._type.baseType != Type.tInt) return null
            info.skipExpr = clone_expression(arg)
        } elif (nm == "skip_while") {
            if (info.skipWhileCond != null || info.takeWhileCond != null || info.takeExpr != null || length(cll._0.arguments) < 2) return null
            var pred = peel_lambda_rename_var(cll._0.arguments[1], tupName)
            if (pred == null) return null
            info.skipWhileCond = pred
        } elif (nm == "take_while") {
            if (info.takeWhileCond != null || info.takeExpr != null || length(cll._0.arguments) < 2) return null
            var pred = peel_lambda_rename_var(cll._0.arguments[1], tupName)
            if (pred == null) return null
            info.takeWhileCond = pred
        } elif (nm == "take") {
            if (info.takeExpr != null || length(cll._0.arguments) < 2) return null
            var arg = cll._0.arguments[1]
            if (arg == null || arg._type == null || arg._type.baseType != Type.tInt) return null
            info.takeExpr = clone_expression(arg)
        } else {
            return null
        }
    }
    info.chainEnd = firstRangeIdx
    return info
}

[macro_function]
def private wrap_inner_for_with_decs_ranges(var body : Expression?;
                                            rangeInfo : DecsRangeInfo?;
                                            names : DecsRangeStateNames) : Expression? {
    // Slice 5a/5b: prefix per-element body with range guards in canonical order: take-cap → skip-counter → skip_while flag → take_while break → takeC++. Mirrors array-side wrap_with_ranges (linq_fold.das:556). All early exits (`return true`) work both for plain inner-for break AND for the bool-lambda archetype-find — caller switches outer iterator to for_each_archetype_find when takeExpr != null OR takeWhileCond != null. Counters / flags live at invoke scope (decs_range_prelude). Take limit is read from a hoisted local (takeLimitName) so the user's take(N) expression evaluates exactly once at invoke entry.
    if (rangeInfo.skipExpr == null && rangeInfo.takeExpr == null && rangeInfo.skipWhileCond == null && rangeInfo.takeWhileCond == null) return body
    let skipName      = names.skipName
    let takeCountName = names.takeCountName
    let takeLimitName = names.takeLimitName
    let skippingName  = names.skippingName
    var stmts : array<Expression?>
    stmts |> reserve(6)
    if (rangeInfo.takeExpr != null) {
        // `>=` so non-positive N short-circuits on first iteration. `return true` exits both inner for AND the archetype bool lambda.
        stmts |> push <| qmacro_expr() {
            if ($i(takeCountName) >= $i(takeLimitName)) {
                return true
            }
        }
    }
    if (rangeInfo.skipExpr != null) {
        stmts |> push <| qmacro_expr() {
            if ($i(skipName) > 0) {
                $i(skipName) --
                continue
            }
        }
    }
    if (rangeInfo.skipWhileCond != null) {
        stmts |> push <| qmacro_expr() {
            if ($i(skippingName)) {
                if ($e(rangeInfo.skipWhileCond)) {
                    continue
                }
                $i(skippingName) = false
            }
        }
    }
    if (rangeInfo.takeWhileCond != null) {
        // First false ends iteration globally (across all archetypes). `return true` exits inner-for AND archetype-find lambda.
        stmts |> push <| qmacro_expr() {
            if (!($e(rangeInfo.takeWhileCond))) {
                return true
            }
        }
    }
    if (rangeInfo.takeExpr != null) {
        // takeC++ AFTER skip_while / take_while so gated-out elements don't eat the take(N) budget (matches array-side comment at linq_fold.das:558).
        stmts |> push <| qmacro_expr() {
            $i(takeCountName) ++
        }
    }
    stmts |> push(body)
    return stmts_to_expr(stmts)
}

[macro_function]
def private decs_range_prelude(rangeInfo : DecsRangeInfo?; names : DecsRangeStateNames) : array<Expression?> {
    // Counter / flag init statements hoisted ABOVE the for_each_archetype call so they survive across archetypes (take cap, skip count, skip_while flag are all globally-stateful). takeLimit hoisted to a local so the user's take(N) expression evaluates exactly once (matches take(src, total:int) semantics).
    let skipName      = names.skipName
    let takeCountName = names.takeCountName
    let takeLimitName = names.takeLimitName
    let skippingName  = names.skippingName
    var stmts : array<Expression?>
    if (rangeInfo.skipExpr != null) {
        stmts |> push <| qmacro_expr() {
            var $i(skipName) = $e(rangeInfo.skipExpr)
        }
    }
    if (rangeInfo.takeExpr != null) {
        stmts |> push_from <| qmacro_block_to_array() {
            let $i(takeLimitName) = $e(rangeInfo.takeExpr)
            var $i(takeCountName) = 0
        }
    }
    if (rangeInfo.skipWhileCond != null) {
        // Slice 5b: skip_while flag is one-way (flips false on first non-matching elem, stays). Hoisted so the flag persists across archetypes.
        stmts |> push <| qmacro_expr() {
            var $i(skippingName) = true
        }
    }
    return <- stmts
}

// 4-way decs walk dispatch: covers every for_each_archetype{,_find} shape across the 5 decs terminator emits.
variant private DecsDispatch {
    Each              : void?   // for_each_archetype(...); tail runs
    Find              : void?   // for_each_archetype_find(...) { body; return false } discarded; tail runs (state-driven answer)
    FindReturn        : void?   // return for_each_archetype_find(...) — bool result IS the answer
    FindReturnNegated : void?   // return !for_each_archetype_find(...) — `all` shape (no counterexample)
}

struct private DecsTerminatorSpec {
    preludeStmts : array<Expression?>   // hoisted at invoke scope (op-specific state vars + default binds)
    perElement   : Expression?           // OP-specific per-tup work (caller wraps own predicate gates; lane wraps ranges+chain)
    tailStmts    : array<Expression?>   // post-walk; INCLUDES the return — skipped for FindReturn/FindReturnNegated
    retType      : TypeDeclPtr
    dispatch     : DecsDispatch
    wrapIter     : bool                  // emission wrapped with .to_sequence_move() — for array-returning shapes in iterator-typed contexts
}

[macro_function]
def private emit_decs_terminator_lane(
                                      var spec : DecsTerminatorSpec;
                                      bridge : DecsBridgeShape?;
                                      chainInfo : DecsChainInfo?;
                                      rangeInfo : DecsRangeInfo?;
                                      var calls : array<tuple<ExprCall?; LinqCall?>>;
                                      intermediateEnd : int;
                                      at : LineInfo
                                      ) : Expression? {
    let tupName = qn("decs_tup", at)
    let names <- make_decs_range_names(at)
    let archName = bridge.archName
    var ptwCond : Expression?
    if (rangeInfo.postTakeWhereCond != null) {
        ptwCond = clone_expression(rangeInfo.postTakeWhereCond)
    }
    var gatedPerElement = wrap_with_condition(spec.perElement, ptwCond)
    var perElement = wrap_inner_for_with_decs_ranges(gatedPerElement, rangeInfo, names)
    var body = wrap_decs_chain(perElement, chainInfo, calls, intermediateEnd, at)
    if (body == null) return null
    var forExprNode = build_decs_inner_for_pruned(bridge, tupName, body, at)
    var rangeStmts <- decs_range_prelude(rangeInfo, names)
    var bodyStmts : array<Expression?>
    bodyStmts |> reserve(length(spec.preludeStmts) + length(rangeStmts) + length(spec.tailStmts) + 1)
    bodyStmts |> push_from(spec.preludeStmts)
    bodyStmts |> push_from(rangeStmts)
    if (spec.dispatch is FindReturn) {
        bodyStmts |> push <| qmacro_expr() {
            return for_each_archetype_find($e(bridge.reqHashExpr), $e(bridge.erqExpr), $($i(archName) : Archetype) : bool {
                $e(forExprNode)
                return false
            })
        }
    } elif (spec.dispatch is FindReturnNegated) {
        bodyStmts |> push <| qmacro_expr() {
            return !for_each_archetype_find($e(bridge.reqHashExpr), $e(bridge.erqExpr), $($i(archName) : Archetype) : bool {
                $e(forExprNode)
                return false
            })
        }
    } elif (spec.dispatch is Find) {
        bodyStmts |> push <| qmacro_expr() {
            for_each_archetype_find($e(bridge.reqHashExpr), $e(bridge.erqExpr), $($i(archName) : Archetype) : bool {
                $e(forExprNode)
                return false
            })
        }
        bodyStmts |> push_from(spec.tailStmts)
    } else {
        bodyStmts |> push <| qmacro_expr() {
            for_each_archetype($e(bridge.reqHashExpr), $e(bridge.erqExpr), $($i(archName) : Archetype) {
                $e(forExprNode)
            })
        }
        bodyStmts |> push_from(spec.tailStmts)
    }
    var emission = qmacro(invoke($() : $t(spec.retType) {
        $b(bodyStmts)
    }))
    return finalize_decs_emission(emission, at, spec.wrapIter)
}

[macro_function]
def private emit_decs_count_archsize(bridge : DecsBridgeShape?; at : LineInfo) : Expression? {
    // Bare count(): no chain ops, sum arch.size per archetype — skips the per-entity walk entirely.
    let accName = qn("decs_acc", at)
    let archName = qn("decs_arch", at)
    // arch.size is int64; count() returns int and the += site truncates past INT_MAX — chain `long_count()` when an int64-safe total is required.
    var emission : Expression? = qmacro(invoke($() : int {
        var $i(accName) = 0
        for_each_archetype($e(bridge.reqHashExpr), $e(bridge.erqExpr), $($i(archName) : Archetype) {
            $i(accName) += int($i(archName).size)
        })
        return $i(accName)
    }))
    emission.force_at(at)
    emission.force_generated(true)
    return emission
}

[macro_function]
def private emit_decs_accumulator(bridge : DecsBridgeShape?;
                                  opName : string;
                                  chainInfo : DecsChainInfo?;
                                  rangeInfo : DecsRangeInfo?;
                                  var calls : array<tuple<ExprCall?; LinqCall?>>;
                                  intermediateEnd : int;
                                  terminatorCall : ExprCall?;
                                  var accType : TypeDeclPtr;
                                  at : LineInfo) : Expression? {
    // Slice 2/3a/4 accumulator emission: count / long_count / sum / min / max / average with chained _select + interleaved _where + optional _count(pred). Slice 5a adds take/skip ranges via rangeInfo (counters hoisted at invoke scope, take present → for_each_archetype_find).
    let accName = qn("decs_acc", at)
    let cntName = qn("decs_cnt", at)
    let firstName = qn("decs_first", at)
    let valBindName = qn("decs_val", at)
    let finalBind = chainInfo.finalBind
    // Wrap finalBind as an Expression so the shared per-element builder (lane uses `$e(projection)`, decs uses `$e(qmacro($i(finalBind)))`) sees one shape.
    var valueExpr : Expression? = qmacro($i(finalBind))
    let workhorse = (chainInfo.finalType != null && chainInfo.finalType.isWorkhorseType)
    let isDoubleAccType = (chainInfo.finalType != null && chainInfo.finalType.baseType == Type.tDouble)
    var perElementStmts <- build_accumulator_perelement_stmts(opName, accName, valBindName, firstName, cntName, valueExpr, workhorse, isDoubleAccType)
    var perElement : Expression? = stmts_to_expr(perElementStmts)
    // _count(pred): count/long_count with extra predicate — wrap perElement with `if (pred) {...}`. Predicate peels against finalBind so it sees the post-chain element.
    if ((opName == "count" || opName == "long_count") && length(terminatorCall.arguments) > 1) {
        var predExpr = peel_lambda_rename_var(terminatorCall.arguments[1], finalBind)
        if (predExpr == null) return null
        perElement = qmacro_expr() {
            if ($e(predExpr)) {
                $e(perElement)
            }
        }
    }
    var preludeStmts : array<Expression?>
    var tailStmts : array<Expression?>
    if (opName == "long_count") {
        preludeStmts |> push <| qmacro_expr() {
            var $i(accName) = 0l
        }
        tailStmts |> push <| qmacro_expr() {
            return $i(accName)
        }
    } elif (opName == "count") {
        preludeStmts |> push <| qmacro_expr() {
            var $i(accName) = 0
        }
        tailStmts |> push <| qmacro_expr() {
            return $i(accName)
        }
    } elif (opName == "sum") {
        preludeStmts |> push <| qmacro_expr() {
            var $i(accName) : $t(accType) = default<$t(accType)>
        }
        tailStmts |> push <| qmacro_expr() {
            return $i(accName)
        }
    } elif (opName == "average") {
        preludeStmts |> push_from <| qmacro_block_to_array() {
            var $i(accName) : double = 0lf
            var $i(cntName) : uint64 = 0ul
        }
        tailStmts |> push <| qmacro_expr() {
            return $i(cntName) != 0ul ? $i(accName) / double($i(cntName)) : 0lf
        }
    } elif (opName == "min" || opName == "max") {
        // No empty-panic — matches non-decs emit_accumulator_lane (returns default-initialized acc).
        preludeStmts |> push_from <| qmacro_block_to_array() {
            var $i(firstName) = true
            var $i(accName) : $t(accType)
        }
        tailStmts |> push <| qmacro_expr() {
            return $i(accName)
        }
    } else {
        return null
    }
    var retType : TypeDeclPtr
    if (opName == "long_count") {
        retType = new TypeDecl(baseType = Type.tInt64)
    } elif (opName == "count") {
        retType = new TypeDecl(baseType = Type.tInt)
    } elif (opName == "average") {
        retType = new TypeDecl(baseType = Type.tDouble)
    } else {
        retType = clone_type(accType)
    }
    // Slice 5b: take/take_while produces inner `return true` (terminates iteration globally) so it needs the bool-lambda archetype-find dispatch.
    let dispatch : DecsDispatch = ((rangeInfo.takeExpr != null || rangeInfo.takeWhileCond != null)
        ? DecsDispatch(Find = null)
        : DecsDispatch(Each = null))
    return emit_decs_terminator_lane(DecsTerminatorSpec(
        preludeStmts <- preludeStmts,
        perElement = perElement,
        tailStmts <- tailStmts,
        retType = retType,
        dispatch = dispatch
    ), bridge, chainInfo, rangeInfo, calls, intermediateEnd, at)
}

[macro_function]
def private emit_decs_early_exit(bridge : DecsBridgeShape?;
                                 opName : string;
                                 chainInfo : DecsChainInfo?;
                                 rangeInfo : DecsRangeInfo?;
                                 var calls : array<tuple<ExprCall?; LinqCall?>>;
                                 intermediateEnd : int;
                                 terminatorCall : ExprCall?;
                                 at : LineInfo) : Expression? {
    // Slice 3b/4 early-exit: first / first_or_default / any / all / contains via for_each_archetype_find (block returns true to stop archetype walk). Supports chained _select + interleaved _where via wrap_decs_chain. Slice 5a adds take/skip ranges via rangeInfo (counters hoisted at invoke scope).
    let foundName = qn("decs_found", at)
    let resultName = qn("decs_result", at)
    let containsValName = qn("decs_cv", at)
    let defaultName = qn("decs_dv", at)
    let finalBind = chainInfo.finalBind
    var elemType = clone_type(chainInfo.finalType)
    let takeOrTakeWhile = (rangeInfo.takeExpr != null || rangeInfo.takeWhileCond != null)
    var perElement : Expression?
    var preludeStmts : array<Expression?>
    var tailStmts : array<Expression?>
    var dispatch : DecsDispatch = DecsDispatch(Find = null)
    var retType : TypeDeclPtr
    if (opName == "first" || opName == "first_or_default") {
        retType = elemType
        preludeStmts |> push_from <| qmacro_block_to_array() {
            var $i(foundName) = false
            var $i(resultName) : $t(elemType)
        }
        if (opName == "first_or_default") {
            preludeStmts |> push <| qmacro_expr() {
                let $i(defaultName) = $e(terminatorCall.arguments[1])
            }
        }
        perElement = qmacro_block() {
            $i(resultName) := $i(finalBind)
            $i(foundName) = true
            return true
        }
        if (opName == "first") {
            tailStmts |> push_from <| qmacro_block_to_array() {
                if (!$i(foundName)) panic("sequence contains no elements")
                return $i(resultName)
            }
        } else {
            tailStmts |> push_from <| qmacro_block_to_array() {
                if (!$i(foundName)) return $i(defaultName)
                return $i(resultName)
            }
        }
    } elif (opName == "any") {
        retType = new TypeDecl(baseType = Type.tBool)
        let argCount = length(terminatorCall.arguments)
        // When take OR take_while is present, distinguish "match found" from "iteration-stop signal" — both produce `return true` from the inner block, so route through an explicit foundName state and use the find result only as iteration control (tail returns foundName).
        if (takeOrTakeWhile) {
            preludeStmts |> push <| qmacro_expr() {
                var $i(foundName) = false
            }
            if (argCount > 1) {
                var predExpr = peel_lambda_rename_var(terminatorCall.arguments[1], finalBind)
                if (predExpr == null) return null
                perElement = qmacro_expr() {
                    if ($e(predExpr)) {
                        $i(foundName) = true
                        return true
                    }
                }
            } else {
                perElement = qmacro_block() {
                    $i(foundName) = true
                    return true
                }
            }
            tailStmts |> push <| qmacro_expr() {
                return $i(foundName)
            }
        } else {
            if (argCount > 1) {
                var predExpr = peel_lambda_rename_var(terminatorCall.arguments[1], finalBind)
                if (predExpr == null) return null
                perElement = qmacro_expr() {
                    if ($e(predExpr)) return true
                }
            } else {
                perElement = qmacro_expr() {
                    return true
                }
            }
            dispatch = DecsDispatch(FindReturn = null)
        }
    } elif (opName == "all") {
        retType = new TypeDecl(baseType = Type.tBool)
        var predExpr = peel_lambda_rename_var(terminatorCall.arguments[1], finalBind)
        if (predExpr == null) return null
        if (takeOrTakeWhile) {
            // Take-cap / take_while-stop hits produce inner `return true`; same sentinel as "counterexample found", so route through explicit foundName state ("found a !pred element"). Tail returns !foundName.
            preludeStmts |> push <| qmacro_expr() {
                var $i(foundName) = false
            }
            perElement = qmacro_expr() {
                if (!$e(predExpr)) {
                    $i(foundName) = true
                    return true
                }
            }
            tailStmts |> push <| qmacro_expr() {
                return !$i(foundName)
            }
        } else {
            perElement = qmacro_expr() {
                if (!$e(predExpr)) return true
            }
            // `all` is "no counterexample found" — inner returns true on FAIL, so negate.
            dispatch = DecsDispatch(FindReturnNegated = null)
        }
    } elif (opName == "contains") {
        retType = new TypeDecl(baseType = Type.tBool)
        preludeStmts |> push <| qmacro_expr() {
            let $i(containsValName) = $e(terminatorCall.arguments[1])
        }
        if (takeOrTakeWhile) {
            // Same disambiguation as any: take-cap / take_while-stop and "value matched" both inner-return true, route through foundName.
            preludeStmts |> push <| qmacro_expr() {
                var $i(foundName) = false
            }
            perElement = qmacro_expr() {
                if ($i(finalBind) == $i(containsValName)) {
                    $i(foundName) = true
                    return true
                }
            }
            tailStmts |> push <| qmacro_expr() {
                return $i(foundName)
            }
        } else {
            perElement = qmacro_expr() {
                if ($i(finalBind) == $i(containsValName)) return true
            }
            dispatch = DecsDispatch(FindReturn = null)
        }
    } else {
        return null
    }
    return emit_decs_terminator_lane(DecsTerminatorSpec(
        preludeStmts <- preludeStmts,
        perElement = perElement,
        tailStmts <- tailStmts,
        retType = retType,
        dispatch = dispatch
    ), bridge, chainInfo, rangeInfo, calls, intermediateEnd, at)
}

[macro_function]
def private emit_decs_to_array(bridge : DecsBridgeShape?;
                               chainInfo : DecsChainInfo?;
                               rangeInfo : DecsRangeInfo?;
                               var calls : array<tuple<ExprCall?; LinqCall?>>;
                               intermediateEnd : int;
                               at : LineInfo) : Expression? {
    // Slice 3c/4 to_array: shape is a thin DecsTerminatorSpec — prelude declares the buf, per-element push_clones the post-chain value at chainInfo.finalBind, tail moves it out. take/take_while → Find dispatch (else Each).
    let bufName = qn("decs_buf", at)
    var elemType = clone_type(chainInfo.finalType)
    var preludeStmts : array<Expression?>
    preludeStmts |> push <| qmacro_expr() {
        var $i(bufName) : array<$t(elemType)>
    }
    let finalBind = chainInfo.finalBind
    var perElement : Expression? = qmacro_expr() {
        $i(bufName) |> push_clone($i(finalBind))
    }
    var tailStmts : array<Expression?>
    tailStmts |> push <| qmacro_expr() {
        return <- $i(bufName)
    }
    var retType = new TypeDecl(baseType = Type.tArray)
    retType.firstType = clone_type(elemType)
    let dispatch : DecsDispatch = ((rangeInfo.takeExpr != null || rangeInfo.takeWhileCond != null)
        ? DecsDispatch(Find = null)
        : DecsDispatch(Each = null))
    return emit_decs_terminator_lane(DecsTerminatorSpec(
        preludeStmts <- preludeStmts,
        perElement = perElement,
        tailStmts <- tailStmts,
        retType = retType,
        dispatch = dispatch
    ), bridge, chainInfo, rangeInfo, calls, intermediateEnd, at)
}

[macro_function]
def private emit_decs_min_max_by(bridge : DecsBridgeShape?;
                                 opName : string;
                                 chainInfo : DecsChainInfo?;
                                 rangeInfo : DecsRangeInfo?;
                                 var calls : array<tuple<ExprCall?; LinqCall?>>;
                                 intermediateEnd : int;
                                 terminatorCall : ExprCall?;
                                 at : LineInfo) : Expression? {
    // Slice 4: min_by / max_by — track best (key, element) pair across archetypes. Element retained via `:=`, matches min_by_impl's empty→default semantics. Slice 5a adds take/skip ranges via rangeInfo.
    let firstName = qn("decs_first", at)
    let bestKeyName = qn("decs_bkey", at)
    let bestElemName = qn("decs_belem", at)
    let keyBindName = qn("decs_key", at)
    let finalBind = chainInfo.finalBind
    var elemType = clone_type(chainInfo.finalType)
    var keyExpr = peel_lambda_rename_var(terminatorCall.arguments[1], finalBind)
    if (keyExpr == null || keyExpr._type == null) return null
    var keyType = clone_type(keyExpr._type)
    keyType.flags.constant = false
    keyType.flags.ref = false
    let workhorse = keyType.isWorkhorseType
    let opCmp = (opName == "min_by") ? "min" : "max"
    var compareExpr = min_max_compare(workhorse, opCmp, keyBindName, bestKeyName)
    var perElement : Expression? = qmacro_block() {
        let $i(keyBindName) = $e(keyExpr)
        if ($i(firstName)) {
            $i(bestKeyName) := $i(keyBindName)
            $i(bestElemName) := $i(finalBind)
            $i(firstName) = false
        } elif ($e(compareExpr)) {
            $i(bestKeyName) := $i(keyBindName)
            $i(bestElemName) := $i(finalBind)
        }
    }
    var preludeStmts <- qmacro_block_to_array() {
        var $i(firstName) = true
        var $i(bestKeyName) : $t(keyType)
        var $i(bestElemName) : $t(elemType)
    }
    var tailStmts : array<Expression?>
    tailStmts |> push <| qmacro_expr() {
        return $i(bestElemName)
    }
    let dispatch : DecsDispatch = ((rangeInfo.takeExpr != null || rangeInfo.takeWhileCond != null)
        ? DecsDispatch(Find = null)
        : DecsDispatch(Each = null))
    return emit_decs_terminator_lane(DecsTerminatorSpec(
        preludeStmts <- preludeStmts,
        perElement = perElement,
        tailStmts <- tailStmts,
        retType = elemType,
        dispatch = dispatch
    ), bridge, chainInfo, rangeInfo, calls, intermediateEnd, at)
}

[macro_function]
def private emit_decs_walk_lane(bridge : DecsBridgeShape?;
                                opName : string;
                                chainInfo : DecsChainInfo?;
                                rangeInfo : DecsRangeInfo?;
                                var calls : array<tuple<ExprCall?; LinqCall?>>;
                                intermediateEnd : int;
                                terminatorCall : ExprCall?;
                                at : LineInfo) : Expression? {
    // Slice 5f walk-lane: last/single/aggregate. State (found flag + retained element / accumulator) hoisted at invoke scope so it persists across archetype boundaries. single_or_default uses for_each_archetype_find for early-exit on 2nd match; others use plain for_each_archetype unless take/take_while is present.
    let finalBind = chainInfo.finalBind
    var elemType = clone_type(chainInfo.finalType)
    var preludeStmts : array<Expression?>
    var tailStmts : array<Expression?>
    var perElement : Expression?
    var retType : TypeDeclPtr
    var earlyExitFind = false
    if (opName == "last" || opName == "last_or_default") {
        retType = elemType
        let foundName = qn("decs_found", at)
        let bindName = qn("decs_lst", at)
        preludeStmts |> push_from <| qmacro_block_to_array() {
            var $i(foundName) = false
            var $i(bindName) : $t(retType)
        }
        perElement = qmacro_block() {
            $i(foundName) = true
            $i(bindName) := $i(finalBind)
        }
        if (opName == "last") {
            tailStmts |> push_from <| qmacro_block_to_array() {
                if (!$i(foundName)) panic("sequence contains no elements")
                return <- $i(bindName)
            }
        } else {
            let defaultName = qn("decs_dv", at)
            let dvCanCopy = (terminatorCall.arguments[1]._type != null && terminatorCall.arguments[1]._type.canCopy)
            preludeStmts |> push(emit_default_bind(defaultName, terminatorCall.arguments[1], dvCanCopy))
            tailStmts |> push_from <| qmacro_block_to_array() {
                if (!$i(foundName)) {
                    $e(emit_default_return(defaultName, dvCanCopy))
                }
                return <- $i(bindName)
            }
        }
    } elif (opName == "single") {
        retType = elemType
        let foundName = qn("decs_found", at)
        let bindName = qn("decs_sng", at)
        preludeStmts |> push_from <| qmacro_block_to_array() {
            var $i(foundName) = false
            var $i(bindName) : $t(retType)
        }
        // Panic on 2nd match terminates the invoke regardless of nesting — no need for early-exit dispatch.
        perElement = qmacro_block() {
            if ($i(foundName)) panic("sequence contains more than one element")
            $i(foundName) = true
            $i(bindName) := $i(finalBind)
        }
        tailStmts |> push_from <| qmacro_block_to_array() {
            if (!$i(foundName)) panic("sequence contains no elements")
            return <- $i(bindName)
        }
    } elif (opName == "single_or_default") {
        retType = elemType
        let foundName = qn("decs_found", at)
        let stopName = qn("decs_stop", at)
        let bindName = qn("decs_sng", at)
        let defaultName = qn("decs_dv", at)
        let dvCanCopy = (terminatorCall.arguments[1]._type != null && terminatorCall.arguments[1]._type.canCopy)
        preludeStmts |> push_from <| qmacro_block_to_array() {
            var $i(foundName) = false
            var $i(stopName) = false
            var $i(bindName) : $t(retType)
        }
        preludeStmts |> push(emit_default_bind(defaultName, terminatorCall.arguments[1], dvCanCopy))
        // 2nd match → set stop, return true (exits inner lambda); for_each_archetype_find then stops the walk.
        perElement = qmacro_block() {
            if ($i(foundName)) {
                $i(stopName) = true
                return true
            }
            $i(foundName) = true
            $i(bindName) := $i(finalBind)
        }
        tailStmts |> push_from <| qmacro_block_to_array() {
            if ($i(stopName) || !$i(foundName)) {
                $e(emit_default_return(defaultName, dvCanCopy))
            }
            return <- $i(bindName)
        }
        earlyExitFind = true
    } elif (opName == "aggregate") {
        var seedExpr = clone_expression(terminatorCall.arguments[1])
        var aggFn = clone_expression(terminatorCall.arguments[2])
        if (seedExpr._type == null) return null
        var accType = clone_type(seedExpr._type)
        accType.flags.constant = false
        accType.flags.ref = false
        retType = clone_type(accType)
        let aggAccName = qn("decs_agg", at)
        let aggIsWorkhorse = seedExpr._type.isWorkhorseType
        if (aggIsWorkhorse) {
            preludeStmts |> push <| qmacro_expr() {
                var $i(aggAccName) : $t(accType) = $e(seedExpr)
            }
        } else {
            preludeStmts |> push <| qmacro_expr() {
                var $i(aggAccName) : $t(accType) <- $e(seedExpr)
            }
        }
        var peeled = peel_lambda_rename_2vars(aggFn, aggAccName, finalBind)
        if (peeled != null) {
            if (aggIsWorkhorse) {
                perElement = qmacro_expr() {
                    $i(aggAccName) = $e(peeled)
                }
            } else {
                perElement = qmacro_expr() {
                    $i(aggAccName) <- $e(peeled)
                }
            }
        } else {
            if (aggIsWorkhorse) {
                perElement = qmacro_expr() {
                    $i(aggAccName) = invoke($e(aggFn), $i(aggAccName), $i(finalBind))
                }
            } else {
                perElement = qmacro_expr() {
                    $i(aggAccName) <- invoke($e(aggFn), $i(aggAccName), $i(finalBind))
                }
            }
        }
        if (aggIsWorkhorse) {
            tailStmts |> push <| qmacro_expr() {
                return $i(aggAccName)
            }
        } else {
            tailStmts |> push <| qmacro_expr() {
                return <- $i(aggAccName)
            }
        }
    } else {
        return null
    }
    let dispatch : DecsDispatch = ((earlyExitFind || rangeInfo.takeExpr != null || rangeInfo.takeWhileCond != null)
        ? DecsDispatch(Find = null)
        : DecsDispatch(Each = null))
    return emit_decs_terminator_lane(DecsTerminatorSpec(
        preludeStmts <- preludeStmts,
        perElement = perElement,
        tailStmts <- tailStmts,
        retType = retType,
        dispatch = dispatch
    ), bridge, chainInfo, rangeInfo, calls, intermediateEnd, at)
}

[macro_function]
def private emit_decs_element_at(bridge : DecsBridgeShape?;
                                 opName : string;
                                 chainInfo : DecsChainInfo?;
                                 rangeInfo : DecsRangeInfo?;
                                 var calls : array<tuple<ExprCall?; LinqCall?>>;
                                 intermediateEnd : int;
                                 terminatorCall : ExprCall?;
                                 at : LineInfo) : Expression? {
    // Slice 5f element_at: counter-driven early-exit via for_each_archetype_find. idx + counter + found + result hoisted at invoke scope; perElement writes result + returns true on cnt == idx match.
    let finalBind = chainInfo.finalBind
    var retType = clone_type(chainInfo.finalType)
    let idxName = qn("decs_idx", at)
    let cntName = qn("decs_ec", at)
    let foundName = qn("decs_found", at)
    let resultName = qn("decs_res", at)
    var preludeStmts : array<Expression?>
    var tailStmts : array<Expression?>
    preludeStmts |> push <| qmacro_expr() {
        let $i(idxName) = $e(terminatorCall.arguments[1])
    }
    if (opName == "element_at") {
        preludeStmts |> push <| qmacro_expr() {
            if ($i(idxName) < 0) panic("element index out of range")
        }
        tailStmts |> push_from <| qmacro_block_to_array() {
            if (!$i(foundName)) panic("element index out of range")
            return <- $i(resultName)
        }
    } else {
        // element_at_or_default has no user-supplied default; mirrors linq.das via `var t : TT; return <- t` (move-init, works for both copyable and non-copyable element types).
        let tName = qn("decs_emptyT", at)
        preludeStmts |> push_from <| qmacro_block_to_array() {
            if ($i(idxName) < 0) {
                var $i(tName) : $t(retType)
                return <- $i(tName)
            }
        }
        tailStmts |> push_from <| qmacro_block_to_array() {
            if (!$i(foundName)) {
                var $i(tName) : $t(retType)
                return <- $i(tName)
            }
            return <- $i(resultName)
        }
    }
    preludeStmts |> push_from <| qmacro_block_to_array() {
        var $i(cntName) = 0
        var $i(foundName) = false
        var $i(resultName) : $t(retType)
    }
    var perElement : Expression? = qmacro_block() {
        if ($i(cntName) == $i(idxName)) {
            $i(resultName) := $i(finalBind)
            $i(foundName) = true
            return true
        }
        $i(cntName) ++
    }
    return emit_decs_terminator_lane(DecsTerminatorSpec(
        preludeStmts <- preludeStmts,
        perElement = perElement,
        tailStmts <- tailStmts,
        retType = retType,
        dispatch = DecsDispatch(Find = null)
    ), bridge, chainInfo, rangeInfo, calls, intermediateEnd, at)
}

// ── decs group_by splice (Slice 5e — state-table over for_each_archetype) ───────

[macro_function]
def private emit_decs_join_impl(bridgeA : DecsBridgeShape?; var c : Captures; var ctx : EmitCtx; at : LineInfo) : Expression? {
    // PR D2-A: keep collect+probe inline (not via adapter_wrap_source_loop) to preserve the count-no-where bucket-length fast path.
    if (bridgeA == null) return null    // both sides must be from_decs_template eager bridges
    var joinCall = c.single["join"]
    var whereLam : Expression?
    if (c.single |> key_exists("trailing_where")) {
        whereLam = joinCall != null && c.single["trailing_where"] != null ? c.single["trailing_where"].arguments[1] : null
        if (whereLam == null) return null
    }
    var selectLam : Expression?
    if (c.single |> key_exists("trailing_select")) {
        var selCall = c.single["trailing_select"]
        if (selCall == null || selCall._type == null || selCall._type.firstType == null) return null
        selectLam = selCall.arguments[1]
        if (selectLam == null) return null
    }
    let countOnly = c.single |> key_exists("term")
    // Iterator-typed implicit-to-array: emission returns array<elemType> uniformly; iterator sink would need a wrap that's not applied here. Cascade.
    if (!countOnly && ctx.expr_is_iterator) return null
    var bridgeB = extract_decs_bridge(joinCall.arguments[1])
    if (bridgeB == null) return null
    // keya/keyb are LinqJoin-macro-synthesized: $($i(la_fresh) : iterTypeA) => keya_body — 1-arg single-return. Block return type IS the key type.
    var keyaLam = joinCall.arguments[2]
    var keybLam = joinCall.arguments[3]
    if (keyaLam == null || keybLam == null || keyaLam._type == null) return null
    let keyType = strip_const_ref(clone_type(keyaLam._type.firstType))
    if (!is_primitive_join_key_type(keyType)) return null
    let tupAName = qn("decs_tup_a", at)
    let tupBName = qn("decs_tup_b", at)
    let hashName = qn("decs_hash", at)
    var keyaBody = peel_lambda_rename_var(keyaLam, tupAName)
    var keybBody = peel_lambda_rename_var(keybLam, tupBName)
    if (keyaBody == null || keybBody == null) return null
    var tupBindA = build_decs_tup_bind(bridgeA, tupAName, at)
    var tupBindB = build_decs_tup_bind(bridgeB, tupBName, at)
    if (tupBindA == null || tupBindB == null) return null
    let tupBType = strip_const_ref(clone_type(bridgeB.elementType))
    var pieces = build_join_standalone_pieces(joinCall, whereLam, selectLam, countOnly, keyaBody, tupAName, hashName, tupBType, "decs", at)
    if (pieces == null) return null
    var collectBody <- qmacro_block_to_array() {
        // nolint:PERF006 per-key bucket size unknown ahead of time
        $i(hashName)[$e(keybBody)] |> push_clone($i(tupBName))
    }
    var collectInner = build_decs_inner_for(bridgeB, tupBindB, stmts_to_expr(collectBody), at)
    var probeInner = build_decs_inner_for(bridgeA, tupBindA, pieces.probeOuter, at)
    var allStmts : array<Expression?>
    allStmts |> push_from(pieces.preludeStmts)
    allStmts |> push <| qmacro_expr() {
        var $i(hashName) : table<$t(keyType); array<$t(tupBType)>>
    }
    allStmts |> push <| qmacro_expr() {
        for_each_archetype($e(bridgeB.reqHashExpr), $e(bridgeB.erqExpr), $($i(bridgeB.archName) : Archetype) {
            $e(collectInner)
        })
    }
    allStmts |> push <| qmacro_expr() {
        for_each_archetype($e(bridgeA.reqHashExpr), $e(bridgeA.erqExpr), $($i(bridgeA.archName) : Archetype) {
            $e(probeInner)
        })
    }
    allStmts |> push(pieces.returnStmt)
    var invokeRetType = pieces.invokeRetType
    var emission = qmacro(invoke($() : $t(invokeRetType) {
        $b(allStmts)
    }))
    return finalize_decs_emission(emission, at, false)
}

// ── array join splice (chunk N+2 — array mirror of register_decs_join_rows) ───────

[macro_function]
def build_zip_rows {
    splice_patterns |> emplace <| SplicePattern(
        name = "zip_general",
        chain <- [
            Slot(matcher = m_literal("zip"), cardinality = c_one(), capture_name = "zip"),
            slot_chain_of(["where_", "select"], "head"),
            Slot(matcher = m_literal("skip"),       cardinality = c_opt(), capture_name = "skip", arity = 2),
            Slot(matcher = m_literal("skip_while"), cardinality = c_opt(), capture_name = "skip_while", arity = 2),
            Slot(matcher = m_literal("take_while"), cardinality = c_opt(), capture_name = "take_while", arity = 2),
            Slot(matcher = m_literal("take"),       cardinality = c_opt(), capture_name = "take", arity = 2),
            Slot(matcher = m_literal("reverse"),    cardinality = c_opt(), capture_name = "trailing_reverse"),
            Slot(matcher = m_alias("loop_terminator_family"), cardinality = c_opt(), capture_name = "term")
        ],
        requires <- [
            @@ < RequiresPredicate > zip_arity_2_or_3,
            @@ < RequiresPredicate > zip_no_while_after_select,
            @@ < RequiresPredicate > zip_reverse_compatible_with_term,
            @@ < RequiresPredicate > take_arg_is_int
        ],
        emit = @@ < EmitFn > emit_zip
    )
}

[macro_function]
def private emit_zip(var c : Captures; var ctx : EmitCtx; at : LineInfo) : Expression? {
    // 2-source lockstep zip splice. Bare zip (array/iterator) + fused where_/select/take/skip/take_while/skip_while + count/long_count/sum/min/max/average/first/first_or_default/any/all/contains terminators. 3-arg zip(a,b,sel) pre-lowers the selector. Trailing reverse on array lane emits reverse_inplace (Theme 8 C4). Counter (count/long_count) and array lanes are built inline here; accumulator + early-exit lanes delegate to emit_accumulator_lane / emit_early_exit_lane (parallel-array form).
    var zipCall = c.single["zip"]
    let zipArgCount = zipCall.arguments |> length
    // Terminator name + counter predicate (2-arg count/long_count).
    var lastName = ""
    var counterPred : Expression?
    if (c.single |> key_exists("term")) {
        var termCall = c.single["term"]
        let candidateName = c.single_name["term"]
        let candidateArgs = termCall.arguments |> length
        if (candidateName == "count" || candidateName == "long_count") {
            if (candidateArgs != 1 && candidateArgs != 2) return null
            if (candidateArgs == 2) {
                if (termCall.arguments[1] == null) return null
                counterPred = clone_expression(termCall.arguments[1])
            }
            lastName = candidateName
        } elif (candidateName == "sum" || candidateName == "min" || candidateName == "max" || candidateName == "average" || candidateName == "first") {
            if (candidateArgs != 1) return null
            lastName = candidateName
        } elif (candidateName == "first_or_default" || candidateName == "contains") {
            if (candidateArgs != 2) return null
            lastName = candidateName
        } elif (candidateName == "any") {
            if (candidateArgs != 1 && candidateArgs != 2) return null
            lastName = candidateName
        } elif (candidateName == "all") {
            if (candidateArgs != 2) return null
            lastName = candidateName
        } else {
            // Recognized by loop_terminator_family alias but not by zip splice (e.g. last/single/element_at). Cascade.
            return null
        }
    }
    let srcAName = qn("srcA", at)
    let srcBName = qn("srcB", at)
    let bufName  = qn("buf", at)
    let accName  = qn("acc", at)
    let names <- make_range_names(at)
    let itName = qn("it", at)
    var srcAExpr = peel_each(clone_expression(zipCall.arguments[0]))
    var srcBExpr = peel_each(clone_expression(zipCall.arguments[1]))
    if (srcAExpr == null || srcAExpr._type == null || srcBExpr == null || srcBExpr._type == null) return null
    srcAExpr.genFlags.alwaysSafe = true
    srcBExpr.genFlags.alwaysSafe = true
    var srcAType = invoke_src_param_type(srcAExpr)
    var srcBType = invoke_src_param_type(srcBExpr)
    var elementType = clone_type(zipCall._type.firstType)
    var whereCond : Expression?
    var projection : Expression?
    var skipExpr : Expression?
    var takeExpr : Expression?
    var skipWhileCond : Expression?
    var takeWhileCond : Expression?
    var seenSelect = false
    let seenReverse = c.single |> key_exists("trailing_reverse")
    var allProjectionsPure = true
    if (zipArgCount == 3) {
        var resultLam = zipCall.arguments[2]
        if (resultLam == null || !(resultLam is ExprMakeBlock)) return null
        var mblk = resultLam as ExprMakeBlock
        var blk = mblk._block as ExprBlock
        if (blk == null || blk.arguments |> length != 2 || blk.list |> length != 1
                || !(blk.list[0] is ExprReturn)) return null
        var ret = blk.list[0] as ExprReturn
        if (ret.subexpr == null || ret.subexpr._type == null) return null
        var projBody = clone_expression(ret.subexpr)
        var projElemType = strip_const_ref(clone_type(ret.subexpr._type))
        var zipRules : Template
        zipRules |> replaceVariable(string(blk.arguments[0].name), qmacro($i(itName)._0))
        zipRules |> replaceVariable(string(blk.arguments[1].name), qmacro($i(itName)._1))
        projBody = apply_template(zipRules, projBody.at, projBody)
        projection = projBody
        elementType = projElemType
        seenSelect = true
        if (has_sideeffects(projection)) {
            allProjectionsPure = false
        }
    }
    // Head walk: where_/select left-to-right. After-select where is the same "chained-bind dedup" gap the imperative bailed on (line 6583).
    for (call in c.many["head"]) {
        let opName = call_norm_name(call)
        if (opName == "where_") {
            if (seenSelect) return null
            var predicate : Expression? = peel_lambda_rename_var(call.arguments[1], itName)
            if (predicate == null) return null
            if (whereCond == null) {
                whereCond = predicate
            } else {
                whereCond = qmacro($e(whereCond) && $e(predicate))
            }
        } elif (opName == "select") {
            if (seenSelect) return null
            // selectCall._type.firstType is a typedecl(result_selector(...)) formula that doesn't resolve in _fold context; pull from the projection block's return expression instead.
            var projRet = peel_lambda_single_return(call.arguments[1])
            if (projRet == null || projRet._type == null) return null
            var projElemType = strip_const_ref(clone_type(projRet._type))
            projection = peel_lambda_rename_var(call.arguments[1], itName)
            if (projection == null) return null
            elementType = projElemType
            seenSelect = true
            if (has_sideeffects(projection)) {
                allProjectionsPure = false
            }
        }
    }
    // Range / while ops from c.single (positional slots already enforce canonical order: skip → skip_while → take_while → take).
    if (c.single |> key_exists("skip")) {
        var skipArg = c.single["skip"].arguments[1]
        if (skipArg == null || skipArg._type == null || skipArg._type.baseType != Type.tInt) return null
        skipExpr = clone_expression(skipArg)
    }
    if (c.single |> key_exists("skip_while")) {
        var swArg = c.single["skip_while"].arguments[1]
        if (swArg == null) return null
        skipWhileCond = peel_lambda_rename_var(swArg, itName)
        if (skipWhileCond == null) return null
    }
    if (c.single |> key_exists("take_while")) {
        var twArg = c.single["take_while"].arguments[1]
        if (twArg == null) return null
        takeWhileCond = peel_lambda_rename_var(twArg, itName)
        if (takeWhileCond == null) return null
    }
    if (c.single |> key_exists("take")) {
        var takeArg = c.single["take"].arguments[1]
        if (takeArg == null || takeArg._type == null || takeArg._type.baseType != Type.tInt) return null
        takeExpr = clone_expression(takeArg)
    }
    let vprojName = qn("vproj", at)
    let bindProjForCounter = counterPred != null && projection != null
    if (counterPred != null) {
        if (projection != null) {
            counterPred = peel_lambda_replace_var(counterPred, qmacro($i(vprojName)))
        } else {
            counterPred = peel_lambda_rename_var(counterPred, itName)
        }
        if (counterPred == null) return null
    }
    let noLimits = skipExpr == null && takeExpr == null && skipWhileCond == null && takeWhileCond == null
    let noChain = whereCond == null && projection == null && noLimits && counterPred == null
    let bothHaveLength = type_has_length(srcAExpr._type) && type_has_length(srcBExpr._type)
    let isCounter = lastName == "count" || lastName == "long_count"
    if (seenSelect && !allProjectionsPure && !noLimits) return null
    let lane = classify_terminator(lastName)
    if (lane == LinqLane.ACCUMULATOR && !isCounter) {
        if (projection == null) return null
        var preCondStmts : array<Expression?>
        preCondStmts |> push <| qmacro_expr() {     // nolint:STYLE012
            let $i(itName) = (itA, itB)
        }
        var intermediateBinds : array<Expression?>
        var laneTops <- [srcAExpr, srcBExpr]
        let laneSrcs <- [srcAName, srcBName]
        return emit_accumulator_lane(lastName, laneTops, projection, whereCond, null,
            intermediateBinds, preCondStmts, elementType, laneSrcs, accName, itName, names,
            skipExpr, takeExpr, skipWhileCond, takeWhileCond, at)
    }
    if (lane == LinqLane.EARLY_EXIT) {
        var preCondStmts : array<Expression?>
        preCondStmts |> push <| qmacro_expr() {     // nolint:STYLE012
            let $i(itName) = (itA, itB)
        }
        var intermediateBinds : array<Expression?>
        let terminatorCall = c.single["term"]
        var laneTops <- [srcAExpr, srcBExpr]
        let laneSrcs <- [srcAName, srcBName]
        return emit_early_exit_lane(lastName, laneTops, projection, whereCond, null,
            intermediateBinds, preCondStmts, elementType, terminatorCall, laneSrcs, itName, names,
            skipExpr, takeExpr, skipWhileCond, takeWhileCond, at)
    }
    // ZipShape feeds adapter_wrap_invoke; the for(itA,itB in srcA,srcB) is built locally because emit_zip prepends the optional `let it = (itA,itB)` bind based on needsItBind (D2-D — only emit_zip knows).
    var zipShape = ZipShape(
        srcAExpr = srcAExpr,
        srcBExpr = srcBExpr,
        srcAName := srcAName,
        srcBName := srcBName,
        srcAType = srcAType,
        srcBType = srcBType
    )
    var adapter = new ZipAdapter(shape = zipShape)
    if (noChain && bothHaveLength && isCounter) {
        var bodyStmts : array<Expression?>
        let bothLongLength = type_has_long_length(srcAExpr._type) && type_has_long_length(srcBExpr._type)
        if (lastName == "count") {
            bodyStmts |> push <| qmacro_expr() {
                return length($i(srcAName)) < length($i(srcBName)) ? length($i(srcAName)) : length($i(srcBName))
            }
        } elif (bothLongLength) {
            // long_count on array/table pair: long_length avoids the int→int64 truncation if either source > INT_MAX.
            bodyStmts |> push <| qmacro_expr() {
                return long_length($i(srcAName)) < long_length($i(srcBName)) ? long_length($i(srcAName)) : long_length($i(srcBName))
            }
        } else {
            bodyStmts |> push <| qmacro_expr() {
                return int64(length($i(srcAName)) < length($i(srcBName)) ? length($i(srcAName)) : length($i(srcBName)))
            }
        }
        return adapter->wrap_invoke(bodyStmts, null, false, at)
    }
    var perStmts : array<Expression?>
    if (isCounter) {
        if (bindProjForCounter) {
            perStmts |> push <| qmacro_expr() {
                var $i(vprojName) = $e(projection)
            }
        } elif (seenSelect && !allProjectionsPure) {
            let finalBindName = qn("vfinal", at)
            perStmts |> push <| qmacro_expr() {
                var $i(finalBindName) = $e(projection)
            }
        }
        if (counterPred != null) {
            perStmts |> push <| qmacro_expr() {
                if ($e(counterPred)) {
                    $i(accName) ++
                }
            }
        } else {
            perStmts |> push <| qmacro_expr() {
                $i(accName) ++
            }
        }
    } else {
        let needsItBindLocal = whereCond != null || projection != null || skipWhileCond != null || takeWhileCond != null
        if (projection != null) {
            perStmts |> push <| qmacro_expr() {
                $i(bufName) |> push_clone($e(projection))
            }
        } elif (needsItBindLocal) {
            perStmts |> push <| qmacro_expr() {
                $i(bufName) |> push_clone($i(itName))
            }
        } else {
            perStmts |> push <| qmacro_expr() {
                $i(bufName) |> push_clone((itA, itB))
            }
        }
    }
    wrap_with_ranges(perStmts, skipExpr, takeExpr, skipWhileCond, takeWhileCond, names)
    var perBody = wrap_with_condition(stmts_to_expr(perStmts), whereCond)
    let projUsedInBody = projection != null && (!isCounter || !allProjectionsPure || bindProjForCounter)
    let needsItBind = whereCond != null || projUsedInBody || skipWhileCond != null || takeWhileCond != null || counterPred != null
    if (needsItBind) {
        var preStmts : array<Expression?>
        preStmts |> reserve(2)
        preStmts |> push <| qmacro_expr() {     // nolint:STYLE012
            let $i(itName) = (itA, itB)
        }
        preStmts |> push(perBody)
        perBody = stmts_to_expr(preStmts)
    }
    var bodyStmts : array<Expression?>
    if (isCounter) {
        if (lastName == "count") {
            bodyStmts |> push <| qmacro_expr() {
                var $i(accName) = 0
            }
        } else {
            bodyStmts |> push <| qmacro_expr() {
                var $i(accName) : int64 = 0l
            }
        }
    } else {
        bodyStmts |> push <| qmacro_expr() {
            var $i(bufName) : array<$t(elementType)>
        }
        if (noChain && bothHaveLength) {
            bodyStmts |> push <| qmacro_expr() {
                $i(bufName) |> reserve(length($i(srcAName)) < length($i(srcBName)) ? length($i(srcAName)) : length($i(srcBName)))
            }
        }
    }
    append_ranges_prelude(bodyStmts, skipExpr, takeExpr, skipWhileCond, names)
    let bodyReferencesIterVars = needsItBind || !isCounter
    if (isCounter && !bodyReferencesIterVars) {
        bodyStmts |> push <| qmacro_expr() {
            for (_itA, _itB in $i(srcAName), $i(srcBName)) {
                $e(perBody)
            }
        }
    } else {
        bodyStmts |> push <| qmacro_expr() {
            for (itA, itB in $i(srcAName), $i(srcBName)) {
                $e(perBody)
            }
        }
    }
    if (isCounter) {
        bodyStmts |> push <| qmacro_expr() {
            return $i(accName)
        }
    } else {
        if (seenReverse) {
            bodyStmts |> push <| qmacro_expr() {
                _::reverse_inplace($i(bufName))
            }
        }
        if (ctx.expr_is_iterator) {
            bodyStmts |> push <| qmacro_expr() {
                return <- $i(bufName).to_sequence_move()
            }
        } else {
            bodyStmts |> push <| qmacro_expr() {
                return <- $i(bufName)
            }
        }
    }
    return adapter->wrap_invoke(bodyStmts, null, false, at)
}

// Single registration entry — separate modules run [_macro]s in separate macro contexts that
// can't coordinate, so the engine calls each source's row-builder explicitly. Order is the
// former register_* definition order (preserves first-match priority in `splice_patterns`).
[_macro]
def private register_all_linq_fold_rows {
    if (!is_compiling_macros_in_module("linq_fold")) return
    build_order_family_rows()
    build_loop_or_count_rows()
    build_reverse_rows()
    build_distinct_rows()
    build_group_by_rows()
    build_decs_join_rows()
    build_array_join_rows()
    build_zip_rows()
}

[macro_function]
def private try_splice_patterns(var expr : Expression?) : Expression? {
    var (top, calls) = flatten_linq(expr)
    if (empty(calls) || expr._type == null) return null
    normalize_order_reverse(calls)
    collapse_chained_selects(calls)
    collapse_chained_wheres(calls)
    let at = calls[0]._0.at
    let exprIsIter = expr._type.isIterator
    // Decs adapter first when a from_decs_template bridge is present (Slice 5d cascade rule).
    var bridge = extract_decs_bridge(top)
    if (bridge != null) {
        let tupName = qn("decs_tup", at)
        for (p in splice_patterns) {
            var r <- match_pattern(p, calls, top)
            if (r is matched) {
                var ctx = EmitCtx(top = null,
                                  src = new DecsAdapter(bridge = bridge, tupName = tupName),
                                  expr_is_iterator = exprIsIter)
                var result = invoke(p.emit, r as matched, ctx, at)
                if (result != null) return result
            }
        }
    }
    // Cascaded decs chains fall through to fold_linq_default rather than risk a decs-only row matching here (peel_each doesn't strip the eager-bridge ExprInvoke, so decs_source stays true) and emitting via Array adapter — see linq_fold_patterns.rst "How dispatch works".
    if (bridge != null) return null
    top = peel_each(top)
    let srcName = qn("source", at)
    for (p in splice_patterns) {
        var r <- match_pattern(p, calls, top)
        if (r is matched) {
            var topClone = clone_expression(top)
            var ctx = EmitCtx(top = topClone,
                              src = new ArrayAdapter(top = topClone, srcName = srcName),
                              expr_is_iterator = exprIsIter)
            var result = invoke(p.emit, r as matched, ctx, at)
            if (result != null) return result
        }
    }
    return null
}

[call_macro(name="_fold")]
class private LinqFold : AstCallMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        macro_verify(call.arguments |> length == 1, prog, call.at, "expecting _fold(expression)")
        macro_verify(call.arguments[0]._type != null, prog, call.at, "expecting linq expression")
        var res = try_splice_patterns(call.arguments[0])
        if (res != null) return res
        // Theme 6 — warn when a `from_decs_template` source survives every splice arm with a non-empty chain
        // (the bridge materializes a temp `res` array before the cascade). Suppress with `options _no_decs_perf_warn = true`.
        if (!(prog._options |> find_arg("_no_decs_perf_warn") ?as tBool ?? false)) {
            var (top, calls) = flatten_linq(call.arguments[0])
            if (!empty(calls) && extract_decs_bridge(top) != null) {
                to_compiler_log("{describe(call.at)}: *warning* `_fold`: from_decs_template source survived dispatch — no splice arm claimed this chain, so the bridge materializes a temp `res` array and the tier-2 cascade runs on the materialized buffer. Rewrite the chain to a recognized decs shape (see doc/source/reference/linq_fold_patterns.rst), or suppress with `options _no_decs_perf_warn = true`.\n")
            }
        }
        // Tier 2 — array-shape pipeline with `_inplace` reuse + explicit `delete`.
        res = fold_linq_default(call.arguments[0])
        if (res != null) return res
        // Tier 3 — raw passthrough.
        return clone_expression(call.arguments[0])
    }
}
