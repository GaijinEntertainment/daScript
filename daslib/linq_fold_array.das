options gen2
options indenting = 4
options _comment_hygiene = true

module linq_fold_array shared public

//! linq_fold array source adapters: ArrayAdapter / ZipAdapter / ArrayJoinAdapter plus the
//! array-shaped emit lanes (counter / accumulator / early-exit / loop-or-count / zip) and
//! their row-builders. Requires linq_fold_common. See daslib/linq_fold.md.

require daslib/linq public
require daslib/ast_boost
require daslib/ast_match
require daslib/templates_boost
require daslib/macro_boost
require strings
require daslib/linq_fold_common public

// 2-source lockstep zip payload — feeds `for (itA, itB in srcA, srcB) { body }` and the 2-source outer invoke.
struct ZipShape {
    srcAExpr, srcBExpr : Expression?
    srcAName, srcBName : string
    srcAType, srcBType : TypeDeclPtr
}

// Array mirror of DecsJoinShape — same role (per-pair result bind for the cross-arm group_by case), 2 array sources iterated via plain `for` loops + a 2-source outer invoke. srcAType / srcBType are the FULL source types (array<T> / iterator<T>) for invoke-param spelling; tupAType / tupBType are the element types post strip_const_ref.
struct ArrayJoinShape {
    srcAExpr, srcBExpr : Expression?
    srcAName, srcBName : string
    srcAType, srcBType : TypeDeclPtr
    tupAType, tupBType : TypeDeclPtr
    keyaLam, keybLam, resultLam : Expression?
    keyType, resultType : TypeDeclPtr
    resBindName : string
}

class ArrayAdapter : SourceAdapter {
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
    def override count_shortcut(opName : string; at : LineInfo) : Expression? {
        return emit_length_shortcut(opName, top, srcName, at)
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

class ZipAdapter : SourceAdapter {
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

class ArrayJoinAdapter : SourceAdapter {
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

[clone(top), macro_function, unused_argument(itName)]
def emit_counter_lane(var top : Expression?; srcName, accName, itName : string; names : RangeStateNames;
                      var skipExpr, takeExpr, skipWhileCond : Expression?;
                      var loopBody : Expression?; at : LineInfo;
                      var adapterOverride : SourceAdapter? = null) : Expression? {
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
    ), build_lane_adapter(laneTops, laneSrcs, adapterOverride), at)
}

// ===== Emission-lane unification (Phase F — see linq_fold.md) =====

// Bridge for orchestrators that still take parallel (topExprs, srcNames) arrays. 1-source → Array;
// 2-source → Zip. adapterOverride (when set) wins outright — lets a non-array single-source adapter
// (e.g. XmlAdapter) ride the array lanes; topExprs/srcNames are then ignored.
[macro_function]
def build_lane_adapter(var topExprs : array<Expression?>; srcNames : array<string>; var adapterOverride : SourceAdapter? = null) : SourceAdapter? {
    if (adapterOverride != null) return adapterOverride
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
def emit_accumulator_lane(
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
                          at : LineInfo;
                          var adapterOverride : SourceAdapter? = null
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
    // Accumulator type: projection-typed if present, else source's element type, const/ref stripped.
    var accType = mut_result_type(projection, elementType)
    let valBindName = qn("val", at)
    let firstName = qn("first", at)
    let cntName = qn("cnt", at)
    let workhorse = ((projection != null && projection._type != null && projection._type.isWorkhorseType)
        || (projection == null && elementType != null && elementType.isWorkhorseType))
    let isDoubleAccType = accType != null && accType.baseType == Type.tDouble
    var spec = build_accumulator_spec(opName, accName, cntName, firstName, accType, at)
    if (spec.returnExpr == null) return null
    var perElementStmts : array<Expression?>
    // long_count: value is discarded, but bind the projection when it has side effects (keeps the AST visit).
    if (opName == "long_count" && projection != null && has_sideeffects(projection)) {
        let finalBindName = qn("vfinal", at)
        perElementStmts |> push <| qmacro_expr() {
            var $i(finalBindName) = $e(projection)
        }
    }
    perElementStmts |> push_from <| build_accumulator_perelement_stmts(opName, accName, valBindName, firstName, cntName, valueExpr, workhorse, isDoubleAccType)
    var tailStmts : array<Expression?>
    tailStmts |> push <| qmacro_expr() {
        return $e(spec.returnExpr)
    }
    return emit_terminator_lane(TerminatorSpec(
        prologue <- spec.prologue,
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
    ), build_lane_adapter(topExprs, srcNames, adapterOverride), at)
}

[macro_function]
def emit_early_exit_lane(
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
                         at : LineInfo;
                         var adapterOverride : SourceAdapter? = null
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
        var retType = mut_result_type(projection, elementType)
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
        var retType = mut_result_type(projection, elementType)
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
        var retType = mut_result_type(projection, elementType)
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
        var retType = mut_result_type(projection, elementType)
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
    ), build_lane_adapter(topExprs, srcNames, adapterOverride), at)
}

[macro_function]
def emit_loop_or_count_lane(var c : Captures; var ctx : EmitCtx; at : LineInfo) : Expression? {
    // PR C Decs-arm: route to existing emit_decs_* lane fns. State hoists above for_each_archetype (different invoke shape from array side); unifying lane fns is out of scope per D1.
    if (ctx.src->kind() == AdapterKind.k_decs) return ctx.src->emit_loop_or_count(c, ctx, at)
    // Non-array single-source adapters (XML) ride this whole lane via the override — it wins over the
    // ArrayAdapter that build_lane_adapter / the buffer lane would otherwise reconstruct from top+srcName.
    var adapterOverride : SourceAdapter? = ctx.src->kind() == AdapterKind.k_xml ? ctx.src : null
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
        return ctx.src->count_shortcut(lastName, at)
    if (lane == LinqLane.ACCUMULATOR) {
        var laneTops : array<Expression?>
        laneTops |> push(top)
        var laneSrcs : array<string>
        laneSrcs |> push(srcName)
        return emit_accumulator_lane(lastName, laneTops, projection, whereCond, postTakeWhereCond,
            intermediateBinds, preCondStmts, elementType, laneSrcs, accName, itName, names,
            skipExpr, takeExpr, skipWhileCond, takeWhileCond, at, adapterOverride)
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
            skipExpr, takeExpr, skipWhileCond, takeWhileCond, at, adapterOverride)
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
        var bufAdapter : SourceAdapter? = adapterOverride
        if (bufAdapter == null) {
            bufAdapter = new ArrayAdapter(top = top, srcName = srcName)
        }
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
        ), bufAdapter, at)
    }
    return emit_counter_lane(top, srcName, accName, itName, names,
        skipExpr, takeExpr, skipWhileCond, loopBody, at, adapterOverride)
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
def emit_zip(var c : Captures; var ctx : EmitCtx; at : LineInfo) : Expression? {
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

