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

struct private LinqCall {
    name : string
    moduleName : string = "linq"
    skip : bool = false             // if specified, expression is skipped in folding
    inplace : bool = false          // if specified, expression is inplace and does not create new variable
    noToArrayVariant : bool = false // if specified, no `_to_array` variant exists (function already returns array regardless of input shape)
    recursive : array<int>          // indices of arguments to apply fold_linq_default on
}

var private linqCalls = {
// filtering data
    "where_" => LinqCall(name = "where_"),
    "where_to_array" => LinqCall(name = "where_"),
// transform operations
    "select" => LinqCall(name = "select"),
    "select_to_array" => LinqCall(name = "select"),
    "select_many" => LinqCall(name = "select_many"),
    "select_many_to_array" => LinqCall(name = "select_many"),
    "zip" => LinqCall(name = "zip", recursive = [1, 2]),
    "zip_to_array" => LinqCall(name = "zip", recursive = [1, 2]),
// sorting data
    "reverse" => LinqCall(name = "reverse", inplace = true),
    "reverse_to_array" => LinqCall(name = "reverse", inplace = true),
    "order" => LinqCall(name = "order", inplace = true),
    "order_to_array" => LinqCall(name = "order", inplace = true),
    "order_descending" => LinqCall(name = "order_descending", inplace = true),
    "order_descending_to_array" => LinqCall(name = "order_descending", inplace = true),
    "order_by" => LinqCall(name = "order_by", inplace = true),
    "order_by_to_array" => LinqCall(name = "order_by", inplace = true),
    "order_by_descending" => LinqCall(name = "order_by_descending", inplace = true),
    "order_by_descending_to_array" => LinqCall(name = "order_by_descending", inplace = true),
// top-N terminal forms — always return array regardless of input shape, so no
    "top_n" => LinqCall(name = "top_n", noToArrayVariant = true),
    "top_n_by" => LinqCall(name = "top_n_by", noToArrayVariant = true),
    "top_n_descending" => LinqCall(name = "top_n_descending", noToArrayVariant = true),
    "top_n_by_descending" => LinqCall(name = "top_n_by_descending", noToArrayVariant = true),
    "top_n_by_with_cmp" => LinqCall(name = "top_n_by_with_cmp", noToArrayVariant = true),
// aggregate
    "count" => LinqCall(name = "count"),
    "long_count" => LinqCall(name = "long_count"),
    "average" => LinqCall(name = "average"),
    "min" => LinqCall(name = "min"),
    "min_by" => LinqCall(name = "min_by"),
    "max" => LinqCall(name = "max"),
    "max_by" => LinqCall(name = "max_by"),
    "min_max" => LinqCall(name = "min_max"),
    "min_max_by" => LinqCall(name = "min_max_by"),
    "min_max_average" => LinqCall(name = "min_max_average"),
    "min_max_average_by" => LinqCall(name = "min_max_average_by"),
    "aggregate" => LinqCall(name = "aggregate"),
    "sum" => LinqCall(name = "sum"),
// ranges
    "skip" => LinqCall(name = "skip", inplace = true),
    "skip_to_array" => LinqCall(name = "skip", inplace = true),
    "skip_while" => LinqCall(name = "skip_while"),
    "skip_while_to_array" => LinqCall(name = "skip_while"),
    "take" => LinqCall(name = "take", inplace = true),
    "take_to_array" => LinqCall(name = "take", inplace = true),
    "take_while" => LinqCall(name = "take_while"),
    "take_while_to_array" => LinqCall(name = "take_while"),
    "skip_last" => LinqCall(name = "skip_last", inplace = true),
    "skip_last_to_array" => LinqCall(name = "skip_last", inplace = true),
    "take_last" => LinqCall(name = "take_last", inplace = true),
    "take_last_to_array" => LinqCall(name = "take_last", inplace = true),
    "chunk" => LinqCall(name = "chunk"),
    "chunk_to_array" => LinqCall(name = "chunk"),
// concatenation operations
    "concat" => LinqCall(name = "concat", recursive = [1], inplace = true),
    "concat_to_array" => LinqCall(name = "concat", recursive = [1], inplace = true),
    "prepend" => LinqCall(name = "prepend", inplace = true),
    "prepend_to_array" => LinqCall(name = "prepend", inplace = true),
    "append" => LinqCall(name = "append", inplace = true),
    "append_to_array" => LinqCall(name = "append", inplace = true),
// set operations
    "distinct" => LinqCall(name = "distinct", inplace = true),
    "distinct_to_array" => LinqCall(name = "distinct", inplace = true),
    "distinct_by" => LinqCall(name = "distinct_by", inplace = true),
    "distinct_by_to_array" => LinqCall(name = "distinct_by", inplace = true),
    "union" => LinqCall(name = "union", recursive = [1]),
    "union_to_array" => LinqCall(name = "union", recursive = [1]),
    "union_by" => LinqCall(name = "union_by", recursive = [1]),
    "union_by_to_array" => LinqCall(name = "union_by", recursive = [1]),
    "except" => LinqCall(name = "except", recursive = [1]),
    "except_to_array" => LinqCall(name = "except", recursive = [1]),
    "except_by" => LinqCall(name = "except_by", recursive = [1]),
    "except_by_to_array" => LinqCall(name = "except_by", recursive = [1]),
    "intersect" => LinqCall(name = "intersect", recursive = [1]),
    "intersect_to_array" => LinqCall(name = "intersect", recursive = [1]),
    "intersect_by" => LinqCall(name = "intersect_by", recursive = [1]),
    "intersect_by_to_array" => LinqCall(name = "intersect_by", recursive = [1]),
    "unique" => LinqCall(name = "unique", inplace = true),
    "unique_to_array" => LinqCall(name = "unique", inplace = true),
    "unique_by" => LinqCall(name = "unique_by", inplace = true),
    "unique_by_to_array" => LinqCall(name = "unique_by", inplace = true),
// element operations
    "element_at" => LinqCall(name = "element_at"),
    "element_at_or_default" => LinqCall(name = "element_at_or_default"),
    "first" => LinqCall(name = "first"),
    "first_or_default" => LinqCall(name = "first_or_default"),
    "last" => LinqCall(name = "last"),
    "last_or_default" => LinqCall(name = "last_or_default"),
    "single" => LinqCall(name = "single"),
    "single_or_default" => LinqCall(name = "single_or_default"),
// join operations
    "join" => LinqCall(name = "join", recursive = [1]),
    "join_to_array" => LinqCall(name = "join", recursive = [1]),
    "left_join" => LinqCall(name = "left_join", recursive = [1]),
    "left_join_to_array" => LinqCall(name = "left_join", recursive = [1]),
    "group_join" => LinqCall(name = "group_join", recursive = [1]),
    "group_join_to_array" => LinqCall(name = "group_join", recursive = [1]),
    "group_by" => LinqCall(name = "group_by"),
    "group_by_to_array" => LinqCall(name = "group_by"),
    "group_by_lazy" => LinqCall(name = "group_by_lazy"),
    "group_by_lazy_to_array" => LinqCall(name = "group_by_lazy"),
    "having_" => LinqCall(name = "having_"),
    "having_to_array" => LinqCall(name = "having_"),
// querying data
    "any" => LinqCall(name = "any"),
    "all" => LinqCall(name = "all"),
    "contains" => LinqCall(name = "contains"),
// skippable
    "to_table" => LinqCall(name = "to_table"),
    "to_array" => LinqCall(name = "to_array", moduleName = "builtin", skip = true),
    "to_sequence" => LinqCall(name = "to_sequence", skip = true),
}

[macro_function]
def private is_linq_call(var expr : Expression?) : bool {
    if (expr is ExprCall) {
        var call = expr as ExprCall
        var funcName = string(call.func.name)
        while (call.func.fromGeneric != null) {
            call.func = call.func.fromGeneric
            funcName = string(call.func.name)
        }
        if (linqCalls.key_exists(funcName)) {
            let linqCall & = unsafe(linqCalls?[funcName] ?? default<LinqCall>)
            if (linqCall.moduleName == call.func._module.name) return true
        }
    }
    return false
}

[macro_function]
def private flatten_linq(var expr : Expression?)  {
    var top = expr
    var calls : array<tuple<ExprCall?; LinqCall?>>
    while (top != null) {
        if (top is ExprCall) {
            var topCall = top as ExprCall
            var topFunc = topCall.func
            while (topFunc.fromGeneric != null) {
                topFunc = topFunc.fromGeneric
            }
            let funcName = string(topFunc.name)
            if (!linqCalls.key_exists(funcName)) break
            var linqCall & = unsafe(linqCalls?[funcName] ?? default<LinqCall>)
            if (linqCall.moduleName != topFunc._module.name) break
            if (!linqCall.skip) {
                calls.push((topCall, unsafe(addr(linqCall))))
            }
            top = topCall.arguments[0]
        } else {
            break
        }
    }
    calls.reverse_inplace()
    // ok, at this point top is source
    return <- (top, calls)
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

[macro_function]
def private type_has_length(t : TypeDecl?) : bool {
    // Gate for macro-emitted `length(src)` — only types with a known overload (excludes lambdas / user iterables).
    if (t == null) return false
    return (t.isGoodArrayType || t.isGoodTableType || t.isString
        || t.isArray || t.isRange)
}

enum private LinqLane {
    UNKNOWN
    ARRAY
    COUNTER
    ACCUMULATOR
    EARLY_EXIT
}

[macro_function]
def private classify_terminator(name : string) : LinqLane {
    if (name == "count") return LinqLane.COUNTER
    // take/skip/take_while/skip_while trailing (after to_array strip) → ARRAY lane with implicit materialization.
    if (name == "where_" || name == "select" || name == "take" || name == "skip"
            || name == "take_while" || name == "skip_while") return LinqLane.ARRAY
    if (name == "sum" || name == "min" || name == "max" || name == "average" || name == "long_count") return LinqLane.ACCUMULATOR
    // EARLY_EXIT is also the dispatch lane for full-walk single-return terminators (last/single/element_at/aggregate) — same emit_early_exit_lane shape, different per-op state.
    if (name == "first" || name == "first_or_default" || name == "any" || name == "all" || name == "contains"
            || name == "last" || name == "last_or_default"
            || name == "single" || name == "single_or_default"
            || name == "element_at" || name == "element_at_or_default"
            || name == "aggregate") return LinqLane.EARLY_EXIT
    return LinqLane.UNKNOWN
}

[macro_function]
def private is_buffer_required_op(name : string) : bool {
    // Defense-in-depth marker: dedicated planners catch these first in the cascade; this arm only fires on shapes they don't recognize.
    return (name == "order_by" || name == "order" || name == "order_descending"
        || name == "order_by_descending"
        || name == "distinct" || name == "distinct_by"
        || name == "reverse"
        || name == "group_by" || name == "group_by_lazy"
        || name == "join" || name == "left_join" || name == "group_join")
}

[macro_function]
def private peel_each(var top : Expression?) : Expression? {
    // Unwrap `each(<arr>)` so downstream length/reserve hints fire; only safe when arg is a real array (iterator sources lack indexed length).
    if (!(top is ExprCall)) return top
    var topCall = top as ExprCall
    if (match_call_in_module(top, "each", "builtin") == null || topCall.arguments |> length != 1) return top
    let argExpr = topCall.arguments[0]
    if ((argExpr == null || argExpr._type == null)
            || (!argExpr._type.isGoodArrayType && !argExpr._type.isArray)) return top
    return clone_expression(argExpr)
}

[macro_function]
def private finalize_invoke(var res : Expression?; at : LineInfo) : Expression? {
    // Post-emit cleanup: stamp loc+generated for diagnostics; set can_shadow on every block arg so gensym sources coexist with user scope. Loop handles 1-source single planners and N-source zip emission uniformly.
    res.force_at(at)
    res.force_generated(true)
    let blk = (res as ExprInvoke).arguments[0] as ExprMakeBlock
    var blkBlock = blk._block as ExprBlock
    for (i in 0 .. length(blkBlock.arguments)) {
        blkBlock.arguments[i].flags.can_shadow = true
    }
    return res
}

[macro_function]
def private finalize_lane_emission(var topExprs : array<Expression?>; srcNames : array<string>;
                                   var bodyStmts : array<Expression?>; at : LineInfo) : Expression? {
    // Multi-source-aware invoke wrap used by emit_accumulator_lane / emit_early_exit_lane. 1-source clones top + derives param type + emits 1-arg invoke; 2-source (zip) does both sides + emits 2-arg invoke. Caller has already pushed the for-loop into bodyStmts.
    let nSrcs = length(srcNames)
    if (nSrcs != 1 && nSrcs != 2) panic("finalize_lane_emission: only 1- or 2-source supported (got {nSrcs}); higher-arity zip planners must extend this branch (or build the invoke wrap directly)")
    if (length(topExprs) != nSrcs) panic("finalize_lane_emission: topExprs length {length(topExprs)} != srcNames length {nSrcs}")
    if (nSrcs == 1) {
        var topExpr = clone_expression(topExprs[0])
        topExpr.genFlags.alwaysSafe = true
        var srcParamType = invoke_src_param_type(topExprs[0])
        var res = qmacro(invoke($($i(srcNames[0]) : $t(srcParamType)) {
            $b(bodyStmts)
        }, $e(topExpr)))
        return finalize_invoke(res, at)
    }
    var topAExpr = clone_expression(topExprs[0])
    topAExpr.genFlags.alwaysSafe = true
    var topBExpr = clone_expression(topExprs[1])
    topBExpr.genFlags.alwaysSafe = true
    var srcAType = invoke_src_param_type(topExprs[0])
    var srcBType = invoke_src_param_type(topExprs[1])
    var res = qmacro(invoke($($i(srcNames[0]) : $t(srcAType), $i(srcNames[1]) : $t(srcBType)) {
        $b(bodyStmts)
    }, $e(topAExpr), $e(topBExpr)))
    return finalize_invoke(res, at)
}

[macro_function]
def private emit_length_shortcut(opName : string; var top : Expression?; srcName : string; at : LineInfo) : Expression? {
    // Count-shaped shortcut: emit `length(src)` (count, int) or `int64(length(src))` (long_count). length returns int, so the int() cast on count would be redundant — interp doesn't fold it and PERF020 fires.
    var topExpr = clone_expression(top)
    topExpr.genFlags.alwaysSafe = true
    var res : Expression?
    if (opName == "long_count") {
        res = qmacro(invoke($($i(srcName) : typedecl($e(topExpr))) {
            return int64(length($i(srcName)))
        }, $e(topExpr)))
    } else {
        res = qmacro(invoke($($i(srcName) : typedecl($e(topExpr))) {
            return length($i(srcName))
        }, $e(topExpr)))
    }
    return finalize_invoke(res, at)
}

[macro_function]
def private invoke_src_param_type(var top : Expression?) : TypeDeclPtr {
    // Compute the invoke block's `$src` parameter type from the chain top:
    var srcParamType = clone_type(top._type)
    if (top._type.isIterator) {
        srcParamType.flags.constant = false
    }
    return srcParamType
}

[macro_function]
def private prepend_binds(var stmts : array<Expression?>; var intermediateBinds : array<Expression?>) {
    // Splice the chain's let-bindings (one per upstream `select`) ahead of the per-element
    if (empty(intermediateBinds)) return
    var prefixed : array<Expression?>
    prefixed |> reserve(long_length(intermediateBinds) + long_length(stmts))
    prefixed |> push_from(intermediateBinds)
    prefixed |> push_from(stmts)
    swap(stmts, prefixed)
}

[macro_function]
def private stmts_to_expr(var stmts : array<Expression?>) : Expression? {
    // Collapse a stmt list to a single expression: pass through when there's only one,
    if (length(stmts) == 1) return stmts[0]
    return <- qmacro_block() {
        $b(stmts)
    }
}

[macro_function]
def private wrap_with_condition(var body : Expression?; var cond : Expression?) : Expression? {
    // Wrap `body` in `if ($cond) { body }` when `cond` is non-null, else return `body` as-is.
    if (cond == null) return body
    return <- qmacro_expr() {
        if ($e(cond)) {
            $e(body)
        }
    }
}

[macro_function]
def private prepend_precond(var body : Expression?; var preCondStmts : array<Expression?>) : Expression? {
    // Splice per-element unconditional binds BEFORE the body. Used by the select+where
    if (empty(preCondStmts)) return body
    var stmts : array<Expression?>
    stmts |> reserve(long_length(preCondStmts) + 1_l)
    stmts |> push_from(preCondStmts)
    stmts |> push(body)
    return stmts_to_expr(stmts)
}

[macro_function]
def private append_ranges_prelude(var preludeStmts : array<Expression?>; var skipExpr, takeExpr, skipWhileCond : Expression?;
                                  skipName, takeCountName, skippingName : string) {
    // Loop-level state for skip/take counters + skip_while flag (one-way, flips on first false-pred).
    if (skipExpr != null) {
        var skipInit = clone_expression(skipExpr)
        preludeStmts |> push <| qmacro_expr() {
            var $i(skipName) = $e(skipInit)
        }
    }
    if (takeExpr != null) {
        preludeStmts |> push <| qmacro_expr() {
            var $i(takeCountName) = 0
        }
    }
    if (skipWhileCond != null) {
        preludeStmts |> push <| qmacro_expr() {
            var $i(skippingName) = true
        }
    }
}

[macro_function]
def private wrap_with_ranges(var stmts : array<Expression?>; var skipExpr, takeExpr, skipWhileCond, takeWhileCond : Expression?;
                             skipName, takeCountName, skippingName : string) {
    // Per-match prefix order: take-guard → skip counter → skip_while flip → take_while break → takeCount++ → body. takeCount++ sits AFTER skip_while/take_while so gated-out elements don't eat the take(N) budget.
    if (skipExpr == null && takeExpr == null && skipWhileCond == null && takeWhileCond == null) return
    var prefixed : array<Expression?>
    prefixed |> reserve(length(stmts) + 5)
    if (takeExpr != null) {
        var takeLimit = clone_expression(takeExpr)
        // `>=` (not `==`) so non-positive N short-circuits on the first iteration —
        prefixed |> push <| qmacro_expr() {
            if ($i(takeCountName) >= $e(takeLimit)) {
                break
            }
        }
    }
    if (skipExpr != null) {
        prefixed |> push <| qmacro_expr() {
            if ($i(skipName) > 0) {
                $i(skipName) --
                continue
            }
        }
    }
    if (skipWhileCond != null) {
        var swPred = clone_expression(skipWhileCond)
        prefixed |> push <| qmacro_expr() {
            if ($i(skippingName)) {
                if ($e(swPred)) {
                    continue
                }
                $i(skippingName) = false
            }
        }
    }
    if (takeWhileCond != null) {
        var twPred = clone_expression(takeWhileCond)
        prefixed |> push <| qmacro_expr() {
            if (!($e(twPred))) {
                break
            }
        }
    }
    if (takeExpr != null) {
        prefixed |> push <| qmacro_expr() {
            $i(takeCountName) ++
        }
    }
    prefixed |> push_from(stmts)
    swap(stmts, prefixed)
}

[macro_function]
def private min_max_compare(workhorse : bool; opName : string; valName, accName : string) : Expression? {
    // Workhorse types use direct `<` / `>` (single-instruction comparison) — this is the
    if (workhorse && opName == "min") return <- qmacro($i(valName) < $i(accName))
    if (workhorse) return <- qmacro($i(valName) > $i(accName))
    if (opName == "min") return <- qmacro(_::less($i(valName), $i(accName)))
    return <- qmacro(_::less($i(accName), $i(valName)))
}

[macro_function]
def private emit_counter_lane(var top : Expression?; srcName, accName, itName, skipName, takeCountName, skippingName : string;
                              var skipExpr, takeExpr, skipWhileCond : Expression?;
                              var loopBody : Expression?; at : LineInfo) : Expression? {
    // Counter lane: `[skip/take init]; var acc = 0; for (it in src) { $loopBody }; return acc`
    var topExpr = clone_expression(top)
    topExpr.genFlags.alwaysSafe = true
    var srcParamType = invoke_src_param_type(top)
    var bodyStmts : array<Expression?>
    append_ranges_prelude(bodyStmts, skipExpr, takeExpr, skipWhileCond, skipName, takeCountName, skippingName)
    bodyStmts |> push <| qmacro_expr() {
        var $i(accName) = 0
    }
    bodyStmts |> push <| qmacro_expr() {
        for ($i(itName) in $i(srcName)) {
            $e(loopBody)
        }
    }
    bodyStmts |> push <| qmacro_expr() {
        return $i(accName)
    }
    var res = qmacro(invoke($($i(srcName) : $t(srcParamType)) {
        $b(bodyStmts)
    }, $e(topExpr)))
    return finalize_invoke(res, at)
}

[macro_function]
def private emit_array_lane(var top : Expression?; var expr : Expression?; var loopBody : Expression?; var elementType : TypeDeclPtr;
                            srcName, accName, itName, skipName, takeCountName, skippingName : string;
                            var skipExpr, takeExpr, skipWhileCond : Expression?;
                            at : LineInfo) : Expression? {
    // Array lane: `[skip/take init]; var acc : array<T>; [reserve]; for (it in src) { $loopBody }; return <- acc`
    let isIter = expr._type.isIterator
    let sourceHasLength = type_has_length(top._type)
    var topExpr = clone_expression(top)
    topExpr.genFlags.alwaysSafe = true
    var srcParamType = invoke_src_param_type(top)
    var bodyStmts : array<Expression?>
    append_ranges_prelude(bodyStmts, skipExpr, takeExpr, skipWhileCond, skipName, takeCountName, skippingName)
    bodyStmts |> push <| qmacro_expr() {
        var $i(accName) : array<$t(elementType)>
    }
    if (sourceHasLength && !isIter) {
        if (takeExpr != null) {
            // Inline `min` so the emission doesn't depend on `require math` at the call site.
            var takeReserveA = clone_expression(takeExpr)
            var takeReserveB = clone_expression(takeExpr)
            bodyStmts |> push <| qmacro_expr() {
                $i(accName) |> reserve($e(takeReserveA) < length($i(srcName)) ? $e(takeReserveB) : length($i(srcName)))
            }
        } else {
            bodyStmts |> push <| qmacro_expr() {
                $i(accName) |> reserve(length($i(srcName)))
            }
        }
    }
    bodyStmts |> push <| qmacro_expr() {
        for ($i(itName) in $i(srcName)) {
            $e(loopBody)
        }
    }
    if (isIter) {
        bodyStmts |> push <| qmacro_expr() {
            return <- $i(accName).to_sequence_move()
        }
    } else {
        bodyStmts |> push <| qmacro_expr() {
            return <- $i(accName)
        }
    }
    var res = qmacro(invoke($($i(srcName) : $t(srcParamType)) {
        $b(bodyStmts)
    }, $e(topExpr)))
    return finalize_invoke(res, at)
}

[macro_function]
def private emit_accumulator_lane(
                                  opName : string;
                                  var topExprs : array<Expression?>;
                                  var projection : Expression?;
                                  var whereCond : Expression?;
                                  var intermediateBinds : array<Expression?>;
                                  var preCondStmts : array<Expression?>;
                                  var elementType : TypeDeclPtr;
                                  srcNames : array<string>;
                                  accName, itName, skipName, takeCountName, skippingName : string;
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
    let valBindName = "`val`{at.line}`{at.column}"
    let firstName = "`first`{at.line}`{at.column}"
    let cntName = "`cnt`{at.line}`{at.column}"
    var preludeStmts : array<Expression?>
    var perMatchStmts : array<Expression?>
    var returnExpr : Expression?
    if (opName == "long_count") {
        preludeStmts <- qmacro_block_to_array() {
            var $i(accName) : int64 = 0l
        }
        // Value is unused; mirror counter-lane discipline and bind only when projection has
        if (projection != null && has_sideeffects(projection)) {
            let finalBindName = "`vfinal`{at.line}`{at.column}"
            perMatchStmts |> push <| qmacro_expr() {
                var $i(finalBindName) = $e(projection)
            }
        }
        perMatchStmts |> push <| qmacro_expr() {
            $i(accName) ++
        }
        returnExpr = qmacro_expr() {
            $i(accName)
        }
    } elif (opName == "sum") {
        preludeStmts <- qmacro_block_to_array() {
            var $i(accName) : $t(accType) = default<$t(accType)>
        }
        perMatchStmts <- qmacro_block_to_array() {
            $i(accName) += $e(valueExpr)
        }
        returnExpr = qmacro_expr() {
            $i(accName)
        }
    } elif (opName == "average") {
        // Matches linq.das:1545 average(iterator) — always-double accumulator, uint64 counter, empty source returns 0.0lf instead of NaN. Source-element cast lifted out at macro time to avoid PERF020 redundant-cast trip when the input is already double.
        preludeStmts <- qmacro_block_to_array() {
            var $i(accName) : double = 0lf
            var $i(cntName) : uint64 = 0ul
        }
        if (accType != null && accType.baseType == Type.tDouble) {
            perMatchStmts <- qmacro_block_to_array() {
                $i(accName) += $e(valueExpr)
                $i(cntName) ++
            }
        } else {
            perMatchStmts <- qmacro_block_to_array() {
                $i(accName) += double($e(valueExpr))
                $i(cntName) ++
            }
        }
        returnExpr = qmacro_expr() {
            $i(cntName) != 0ul ? $i(accName) / double($i(cntName)) : 0lf
        }
    } elif (opName == "min" || opName == "max") {
        preludeStmts <- qmacro_block_to_array() {
            var $i(firstName) = true
            var $i(accName) : $t(accType)
        }
        let workhorse = ((projection != null && projection._type != null && projection._type.isWorkhorseType)
            || (projection == null && elementType != null && elementType.isWorkhorseType))
        var compareExpr = min_max_compare(workhorse, opName, valBindName, accName)
        perMatchStmts <- qmacro_block_to_array() {
            let $i(valBindName) = $e(valueExpr)
            if ($i(firstName)) {
                $i(accName) := $i(valBindName)
                $i(firstName) = false
            } elif ($e(compareExpr)) {
                $i(accName) := $i(valBindName)
            }
        }
        returnExpr = qmacro_expr() {
            $i(accName)
        }
    } else {
        return null
    }
    prepend_binds(perMatchStmts, intermediateBinds)
    wrap_with_ranges(perMatchStmts, skipExpr, takeExpr, skipWhileCond, takeWhileCond, skipName, takeCountName, skippingName)
    var loopBody = prepend_precond(wrap_with_condition(stmts_to_expr(perMatchStmts), whereCond), preCondStmts)
    // Collect all body statements into one list so they share scope when spliced via $b.
    var bodyStmts : array<Expression?>
    bodyStmts |> reserve(length(preludeStmts) + 4)
    append_ranges_prelude(bodyStmts, skipExpr, takeExpr, skipWhileCond, skipName, takeCountName, skippingName)
    bodyStmts |> push_from(preludeStmts)
    // For-loop emission: single-source uses one iter var (itName); 2-source (zip) uses literal `itA, itB` parallel iter vars (qmacro for-loop iter-var position doesn't accept $i(...) splice). Caller (plan_zip) threads `let it = (itA, itB)` via preCondStmts so itName resolves inside the loop body.
    if (length(srcNames) == 1) {
        bodyStmts |> push <| qmacro_expr() {
            for ($i(itName) in $i(srcNames[0])) {
                $e(loopBody)
            }
        }
    } else {
        bodyStmts |> push <| qmacro_expr() {
            for (itA, itB in $i(srcNames[0]), $i(srcNames[1])) {
                $e(loopBody)
            }
        }
    }
    bodyStmts |> push <| qmacro_expr() {
        return $e(returnExpr)
    }
    return finalize_lane_emission(topExprs, srcNames, bodyStmts, at)
}

[macro_function]
def private emit_any_empty_shortcut(var top : Expression?; srcName : string; at : LineInfo) : Expression? {
    // any (no predicate) shortcut: emit `!empty(src)` directly. Caller has verified no
    var topExpr = clone_expression(top)
    topExpr.genFlags.alwaysSafe = true
    var res = qmacro(invoke($($i(srcName) : typedecl($e(topExpr))) {
        return !empty($i(srcName))
    }, $e(topExpr)))
    return finalize_invoke(res, at)
}

[macro_function]
def private emit_early_exit_lane(
                                 opName : string;
                                 var topExprs : array<Expression?>;
                                 var projection : Expression?;
                                 var whereCond : Expression?;
                                 var intermediateBinds : array<Expression?>;
                                 var preCondStmts : array<Expression?>;
                                 var elementType : TypeDeclPtr;
                                 terminatorCall : ExprCall?;
                                 srcNames : array<string>;
                                 itName, skipName, takeCountName, skippingName : string;
                                 var skipExpr, takeExpr, skipWhileCond, takeWhileCond : Expression?;
                                 at : LineInfo
                                 ) : Expression? {
    // Ring 2 early-exit lane: first / first_or_default / any / all / contains.
    var perMatchStmts : array<Expression?>
    var valueName = itName
    let projBindName = "`vproj`{at.line}`{at.column}"
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
        let defaultName = "`dval`{at.line}`{at.column}"
        var defaultExpr = clone_expression(terminatorCall.arguments[1])
        preludeStmts |> push <| qmacro_expr() {
            let $i(defaultName) = $e(defaultExpr)
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
            var predExpr = peel_lambda_rename_var(clone_expression(terminatorCall.arguments[1]), valueName)
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
        var predExpr = peel_lambda_rename_var(clone_expression(terminatorCall.arguments[1]), valueName)
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
        let containsValName = "`cval`{at.line}`{at.column}"
        var valExpr = clone_expression(terminatorCall.arguments[1])
        preludeStmts |> push <| qmacro_expr() {
            let $i(containsValName) = $e(valExpr)
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
        let foundName = "`found`{at.line}`{at.column}"
        let lastBindName = "`lst`{at.line}`{at.column}"
        preludeStmts |> push <| qmacro_expr() {
            var $i(foundName) = false
        }
        preludeStmts |> push <| qmacro_expr() {
            var $i(lastBindName) : $t(retType)
        }
        perMatchStmts |> push <| qmacro_expr() {
            $i(foundName) = true
        }
        perMatchStmts |> push <| qmacro_expr() {
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
            // Bind `d` once at the top (eager, matches linq.das line 2621).
            let defaultName = "`dval`{at.line}`{at.column}"
            var defaultExpr = clone_expression(terminatorCall.arguments[1])
            preludeStmts |> push <| qmacro_expr() {
                let $i(defaultName) = $e(defaultExpr)
            }
            tailStmts <- qmacro_block_to_array() {
                if (!$i(foundName)) {
                    return $i(defaultName)
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
        let foundName = "`found`{at.line}`{at.column}"
        let bindName = "`sng`{at.line}`{at.column}"
        preludeStmts |> push <| qmacro_expr() {
            var $i(foundName) = false
        }
        preludeStmts |> push <| qmacro_expr() {
            var $i(bindName) : $t(retType)
        }
        if (opName == "single") {
            perMatchStmts |> push <| qmacro_expr() {
                if ($i(foundName)) {
                    panic("sequence contains more than one element")
                }
            }
            perMatchStmts |> push <| qmacro_expr() {
                $i(foundName) = true
            }
            perMatchStmts |> push <| qmacro_expr() {
                $i(bindName) := $i(valueName)
            }
            tailStmts <- qmacro_block_to_array() {
                if (!$i(foundName)) {
                    panic("sequence contains no elements")
                }
                return <- $i(bindName)
            }
        } else {
            let defaultName = "`dval`{at.line}`{at.column}"
            var defaultExpr = clone_expression(terminatorCall.arguments[1])
            preludeStmts |> push <| qmacro_expr() {
                let $i(defaultName) = $e(defaultExpr)
            }
            // `single_or_default` early-returns default on the SECOND match (subsequent matches don't matter).
            perMatchStmts |> push <| qmacro_expr() {
                if ($i(foundName)) {
                    return $i(defaultName)
                }
            }
            perMatchStmts |> push <| qmacro_expr() {
                $i(foundName) = true
            }
            perMatchStmts |> push <| qmacro_expr() {
                $i(bindName) := $i(valueName)
            }
            tailStmts <- qmacro_block_to_array() {
                if (!$i(foundName)) {
                    return $i(defaultName)
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
        let idxName = "`idx`{at.line}`{at.column}"
        let cntName = "`ec`{at.line}`{at.column}"
        var idxExpr = clone_expression(terminatorCall.arguments[1])
        preludeStmts |> push <| qmacro_expr() {
            let $i(idxName) = $e(idxExpr)
        }
        if (opName == "element_at") {
            preludeStmts |> push <| qmacro_expr() {
                if ($i(idxName) < 0) {
                    panic("element index out of range")
                }
            }
        } else {
            preludeStmts |> push <| qmacro_expr() {
                if ($i(idxName) < 0) {
                    return default<$t(retType)>
                }
            }
        }
        preludeStmts |> push <| qmacro_expr() {
            var $i(cntName) = 0
        }
        perMatchStmts |> push <| qmacro_expr() {
            if ($i(cntName) == $i(idxName)) {
                return $i(valueName)
            }
        }
        perMatchStmts |> push <| qmacro_expr() {
            $i(cntName) ++
        }
        if (opName == "element_at") {
            tailStmts <- qmacro_block_to_array() {
                panic("element index out of range")
                return default<$t(retType)>
            }
        } else {
            tailStmts |> push <| qmacro_expr() {
                return default<$t(retType)>
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
        let aggAccName = "`agg`{at.line}`{at.column}"
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
    prepend_binds(perMatchStmts, intermediateBinds)
    wrap_with_ranges(perMatchStmts, skipExpr, takeExpr, skipWhileCond, takeWhileCond, skipName, takeCountName, skippingName)
    var loopBody = prepend_precond(wrap_with_condition(stmts_to_expr(perMatchStmts), whereCond), preCondStmts)
    // Single-$b body so all stmts (skip/take counters + prelude + for + tail) share scope
    var bodyStmts : array<Expression?>
    bodyStmts |> reserve(length(preludeStmts) + length(tailStmts) + 3)
    append_ranges_prelude(bodyStmts, skipExpr, takeExpr, skipWhileCond, skipName, takeCountName, skippingName)
    bodyStmts |> push_from(preludeStmts)
    // For-loop emission: 1-source uses itName; 2-source (zip) uses literal `itA, itB` (qmacro for-iter-var position doesn't accept $i splice). Caller (plan_zip) threads `let it = (itA, itB)` via preCondStmts.
    if (length(srcNames) == 1) {
        bodyStmts |> push <| qmacro_expr() {
            for ($i(itName) in $i(srcNames[0])) {
                $e(loopBody)
            }
        }
    } else {
        bodyStmts |> push <| qmacro_expr() {
            for (itA, itB in $i(srcNames[0]), $i(srcNames[1])) {
                $e(loopBody)
            }
        }
    }
    bodyStmts |> push_from(tailStmts)
    return finalize_lane_emission(topExprs, srcNames, bodyStmts, at)
}

[macro_function]
def private merge_where_cond(var curr : Expression?; var pred : Expression?) : Expression? {
    if (curr == null) return pred
    return <- qmacro($e(curr) && $e(pred))
}

[macro_function]
def private strip_const_ref(var t : TypeDeclPtr) : TypeDeclPtr {
    if (t != null) {
        t.flags.constant = false
        t.flags.ref = false
    }
    return t
}

[macro_function]
def private finalize_emission(var top : Expression?; srcName : string; at : LineInfo; var body : Expression?) : Expression? {
    var srcParamType = invoke_src_param_type(top)
    var topExpr = clone_expression(top)
    topExpr.genFlags.alwaysSafe = true
    var emission : Expression? = qmacro(invoke($($i(srcName) : $t(srcParamType)) {
        $e(body)
    }, $e(topExpr)))
    return finalize_invoke(emission, at)
}

[macro_function]
def private finalize_emission_stmts(var top : Expression?; srcName : string; at : LineInfo; var stmts : array<Expression?>) : Expression? {
    var srcParamType = invoke_src_param_type(top)
    var topExpr = clone_expression(top)
    topExpr.genFlags.alwaysSafe = true
    var emission : Expression? = qmacro(invoke($($i(srcName) : $t(srcParamType)) {
        $b(stmts)
    }, $e(topExpr)))
    return finalize_invoke(emission, at)
}

[macro_function]
def private buffer_return(bufName : string; isIterator : bool) : Expression? {
    if (isIterator) return <- qmacro_expr() {
        return <- $i(bufName).to_sequence_move()
    }
    return <- qmacro_expr() {
        return <- $i(bufName)
    }
}

[macro_function]
def private order_top_n_call_name(orderName : string) : string {
    if (orderName == "order") return "top_n"
    if (orderName == "order_descending") return "top_n_descending"
    if (orderName == "order_by") return "top_n_by"
    if (orderName == "order_by_descending") return "top_n_by_descending"
    return ""
}

def private order_min_call_name(orderName : string; hasKey : bool) : string {
    // `order + first` collapses to min/max — asc → min, desc → max. Keyed forms route to min_by/max_by.
    let isDesc = orderName == "order_descending" || orderName == "order_by_descending"
    if (hasKey) return isDesc ? "max_by" : "min_by"
    return isDesc ? "max" : "min"
}

[macro_function]
def private try_make_inline_cmp(orderKey : Expression?; orderName : string;
                                elemType : TypeDeclPtr; at : LineInfo) : Expression? {
    if (orderKey == null || !(orderKey is ExprMakeBlock)) return null
    var mblk = orderKey as ExprMakeBlock
    var blk = mblk._block as ExprBlock
    if (blk.arguments |> length != 1 || blk.list |> length != 1
            || !(blk.list[0] is ExprReturn)) return null
    var ret = blk.list[0] as ExprReturn
    if (ret.subexpr == null || has_sideeffects(ret.subexpr)) return null
    let argName = string(blk.arguments[0].name)
    var b1 = clone_expression(ret.subexpr)
    var b2 = clone_expression(ret.subexpr)
    var r1 : Template
    r1 |> renameVariable(argName, "v1")
    var r2 : Template
    r2 |> renameVariable(argName, "v2")
    apply_template(r1, b1.at, b1)
    apply_template(r2, b2.at, b2)
    var cmpExpr : Expression?
    if (orderName == "order_by_descending") {
        cmpExpr = qmacro(_::less($e(b2), $e(b1)))
    } else {
        cmpExpr = qmacro(_::less($e(b1), $e(b2)))
    }
    // Emit untyped block args (`$(v1, v2) { ... }`). The typer infers v1/v2 types from
    return qmacro($(v1, v2) {
        return $e(cmpExpr)
    })
}

[macro_function]
def private plan_order_family(var expr : Expression?) : Expression? {
    var (top, calls) = flatten_linq(expr)
    if (empty(calls)) return null
    top = peel_each(top)
    var whereCond : Expression?
    var orderName : string
    var orderKey : Expression?
    var orderElemType : TypeDeclPtr
    var takeExpr : Expression?
    var firstName : string
    var firstDefaultExpr : Expression?
    var hasOrder = false
    let at = calls[0]._0.at
    let itName = "`it`{at.line}`{at.column}"
    for (i in 0 .. length(calls)) {
        var cll & = unsafe(calls[i])
        let name = cll._1.name
        if (name == "where_") {
            if (hasOrder) return null   // where-after-order not in scope
            var pred = peel_lambda_rename_var(cll._0.arguments[1], itName)
            if (whereCond == null) {
                whereCond = pred
            } else {
                whereCond = qmacro($e(whereCond) && $e(pred))
            }
        } elif (name == "order" || name == "order_descending"
                || name == "order_by" || name == "order_by_descending") {
            if (hasOrder) return null
            // bail on `order(arr, cmp)` / `order_descending(arr, cmp)` — splice helpers (min/max/top_n) can't honor a user-supplied comparator and would silently drop it.
            let argCount = cll._0.arguments |> length
            if ((name == "order" || name == "order_descending") && argCount >= 2) return null
            hasOrder = true
            orderName = name
            if (argCount >= 2) {
                orderKey = clone_expression(cll._0.arguments[1])
            }
            orderElemType = clone_type(cll._0._type.firstType)
        } elif (name == "take") {
            if (!hasOrder || takeExpr != null || firstName != "") return null
            var arg = cll._0.arguments[1]
            if (arg == null || arg._type == null || arg._type.baseType != Type.tInt) return null
            takeExpr = clone_expression(arg)
        } elif (name == "first" || name == "first_or_default") {
            // order + first → min/max (O(N) instead of sort + index). Must be terminal.
            if (!hasOrder || takeExpr != null || firstName != "" || i != length(calls) - 1) return null
            firstName = name
            if (name == "first_or_default") {
                if ((cll._0.arguments |> length) < 2) return null
                firstDefaultExpr = clone_expression(cll._0.arguments[1])
            }
        } else {
            return null
        }
    }
    if (!hasOrder) return null
    let hasKey = orderName == "order_by" || orderName == "order_by_descending"
    let needIterWrap = expr._type.isIterator
    let topNName = order_top_n_call_name(orderName)
    let inplaceName = "{orderName}_inplace"
    // Inline-key path: when the key is a pure single-expression lambda, splice the body
    var inlineCmp : Expression?
    if (hasKey) {
        inlineCmp = try_make_inline_cmp(orderKey, orderName, orderElemType, at)
    }
    let minMaxName = order_min_call_name(orderName, hasKey)
    if (whereCond == null) {
        // No prefilter — direct call to daslib helper.
        var topExpr = clone_expression(top)
        topExpr.genFlags.alwaysSafe = true
        var emission : Expression?
        if (firstName == "first") {
            // order + first → preserve eager `first()` panic-on-empty. min/max return an uninitialized ref on empty, so wrap in an empty-guard for arrays (zero alloc, O(N) min scan), or use top_n*(_, 1, _) |> first() for iterators (n=1 bounded heap; first() panics on empty).
            if (top._type.isGoodArrayType) {
                var srcParamType = invoke_src_param_type(top)
                let firstSrcName = "`first_src`{at.line}`{at.column}"
                var minMaxCall : Expression?
                if (hasKey) {
                    minMaxCall = qmacro($c(minMaxName)($i(firstSrcName), $e(orderKey)))
                } else {
                    minMaxCall = qmacro($c(minMaxName)($i(firstSrcName)))
                }
                emission = qmacro(invoke($($i(firstSrcName) : $t(srcParamType)) {
                    panic("sequence contains no elements") if (empty($i(firstSrcName)))
                    return $e(minMaxCall)
                }, $e(topExpr)))
                emission = finalize_invoke(emission, at)
            } else {
                var topNCall : Expression?
                if (inlineCmp != null) {
                    topNCall = qmacro(_::top_n_by_with_cmp($e(topExpr), 1, $e(inlineCmp)))
                } elif (hasKey) {
                    topNCall = qmacro($c(topNName)($e(topExpr), 1, $e(orderKey)))
                } else {
                    topNCall = qmacro($c(topNName)($e(topExpr), 1))
                }
                emission = qmacro(_::first($e(topNCall)))
            }
        } elif (firstName == "first_or_default") {
            // No min_by_or_default exists; route through top_n*(_, 1, _) which returns an array (empty or single-element), then first_or_default supplies the default.
            var topNCall : Expression?
            if (inlineCmp != null) {
                topNCall = qmacro(_::top_n_by_with_cmp($e(topExpr), 1, $e(inlineCmp)))
            } elif (hasKey) {
                topNCall = qmacro($c(topNName)($e(topExpr), 1, $e(orderKey)))
            } else {
                topNCall = qmacro($c(topNName)($e(topExpr), 1))
            }
            emission = qmacro(_::first_or_default($e(topNCall), $e(firstDefaultExpr)))
        } elif (takeExpr == null) {
            // Bare order family — emit the direct call. Same shape as plain LINQ, but via
            if (inlineCmp != null) {
                // Inlined comparator dispatches to the asc `order(src, block)` overload —
                emission = qmacro(_::order($e(topExpr), $e(inlineCmp)))
            } elif (hasKey) {
                emission = qmacro($c(orderName)($e(topExpr), $e(orderKey)))
            } else {
                emission = qmacro($c(orderName)($e(topExpr)))
            }
        } else {
            // order + take → top_n* dispatch.
            if (inlineCmp != null) {
                emission = qmacro(_::top_n_by_with_cmp($e(topExpr), $e(takeExpr), $e(inlineCmp)))
            } elif (hasKey) {
                emission = qmacro($c(topNName)($e(topExpr), $e(takeExpr), $e(orderKey)))
            } else {
                emission = qmacro($c(topNName)($e(topExpr), $e(takeExpr)))
            }
        }
        // Wrap with to_sequence_move only when emission is array-shaped: take dispatches to
        let emissionIsArray = takeExpr != null || (firstName == "" && top._type.isGoodArrayType)
        if (needIterWrap && emissionIsArray) {
            emission = qmacro($e(emission).to_sequence_move())
        }
        emission.force_generated(true)
        return emission
    }
    // where_* + order_*[+take] — emit a single fused loop that prefilters into a fresh
    let srcName = "`source`{at.line}`{at.column}"
    let bufName = "`buf`{at.line}`{at.column}"
    var srcParamType = invoke_src_param_type(top)
    var topExpr = clone_expression(top)
    topExpr.genFlags.alwaysSafe = true
    var bufElemType = clone_type(orderElemType)
    var loopBody = qmacro_expr() {
        if ($e(whereCond)) {
            $i(bufName) |> push_clone($i(itName))
        }
    }
    var stmts : array<Expression?>
    stmts |> push <| qmacro_expr() {
        var $i(bufName) : array<$t(bufElemType)>
    }
    if (type_has_length(top._type)) {
        stmts |> push <| qmacro_expr() {
            $i(bufName) |> reserve(length($i(srcName)))
        }
    }
    stmts |> push <| qmacro_expr() {
        for ($i(itName) in $i(srcName)) {
            $e(loopBody)
        }
    }
    if (firstName == "first") {
        // where + order + first → min/max on prefilter buffer. Empty buf must panic to match eager `first()` semantics; min/max return uninitialized refs on empty.
        stmts |> push <| qmacro_expr() {
            panic("sequence contains no elements") if (empty($i(bufName)))
        }
        var minMaxCall : Expression?
        if (hasKey) {
            minMaxCall = qmacro($c(minMaxName)($i(bufName), $e(orderKey)))
        } else {
            minMaxCall = qmacro($c(minMaxName)($i(bufName)))
        }
        stmts |> push <| qmacro_expr() {
            return $e(minMaxCall)
        }
    } elif (firstName == "first_or_default") {
        // No min_by_or_default helper exists; route through top_n*(_, 1, _) + first_or_default for the empty-buf case.
        var topNCall : Expression?
        if (inlineCmp != null) {
            topNCall = qmacro(_::top_n_by_with_cmp($i(bufName), 1, $e(inlineCmp)))
        } elif (hasKey) {
            topNCall = qmacro($c(topNName)($i(bufName), 1, $e(orderKey)))
        } else {
            topNCall = qmacro($c(topNName)($i(bufName), 1))
        }
        stmts |> push <| qmacro_expr() {
            return _::first_or_default($e(topNCall), $e(firstDefaultExpr))
        }
    } elif (takeExpr == null) {
        // Sort the prefilter buffer in place and return it. order*_inplace is void
        var sortCall : Expression?
        if (inlineCmp != null) {
            // Asc `order_inplace(buf, block)` overload + embedded-flip cmp for descending.
            sortCall = qmacro(_::order_inplace($i(bufName), $e(inlineCmp)))
        } elif (hasKey) {
            sortCall = qmacro($c(inplaceName)($i(bufName), $e(orderKey)))
        } else {
            sortCall = qmacro($c(inplaceName)($i(bufName)))
        }
        stmts |> push(sortCall)
        stmts |> push <| qmacro_expr() {
            return <- $i(bufName)
        }
    } else {
        // top_n* on the prefilter buffer.
        var topNCall : Expression?
        if (inlineCmp != null) {
            topNCall = qmacro(_::top_n_by_with_cmp($i(bufName), $e(takeExpr), $e(inlineCmp)))
        } elif (hasKey) {
            topNCall = qmacro($c(topNName)($i(bufName), $e(takeExpr), $e(orderKey)))
        } else {
            topNCall = qmacro($c(topNName)($i(bufName), $e(takeExpr)))
        }
        stmts |> push <| qmacro_expr() {
            return <- $e(topNCall)
        }
    }
    var bodyBlock = new ExprBlock(at = at)
    for (s in stmts) {
        bodyBlock.list |> emplace(s)
    }
    var emission : Expression? = qmacro(invoke($($i(srcName) : $t(srcParamType)) {
        $e(bodyBlock);
    }, $e(topExpr)))
    if (needIterWrap) {
        emission = qmacro($e(emission).to_sequence_move())
    }
    return finalize_invoke(emission, at)
}

[macro_function]
def private plan_loop_or_count(var expr : Expression?) : Expression? {
    // Phase-2C loop planner. Recognizes chains of shape `[where_*][select*][skip?][take?]`
    var (top, calls) = flatten_linq(expr)
    if (empty(calls)) return null
    top = peel_each(top)
    let lastName = calls.back()._1.name
    let lane = classify_terminator(lastName)
    // Marker: future PRs add BufferTopN / BufferDistinct / etc. for `is_buffer_required_op`
    if (lane == LinqLane.UNKNOWN) return null
    let counterLane = lane == LinqLane.COUNTER
    let hasTerminator = lane != LinqLane.ARRAY
    let intermediateCount = hasTerminator ? length(calls) - 1 : length(calls)
    let at = calls[0]._0.at
    let srcName = "`source`{at.line}`{at.column}"
    let itName  = "`it`{at.line}`{at.column}"
    let accName = "`acc`{at.line}`{at.column}"
    let skipName = "`skip`{at.line}`{at.column}"
    let takeCountName = "`tc`{at.line}`{at.column}"
    let skippingName = "`skipping`{at.line}`{at.column}"
    var whereCond : Expression?
    var projection : Expression?
    var intermediateBinds : array<Expression?>
    // preConditionStmts evaluate UNCONDITIONALLY per element, BEFORE the where filter —
    var preCondStmts : array<Expression?>
    var skipExpr : Expression?
    var takeExpr : Expression?
    // skip_while / take_while: predicate-driven ranges. Both peel with itName (source elem); seenSelect bails to tier 2.
    var skipWhileCond : Expression?
    var takeWhileCond : Expression?
    var seenSelect = false
    var seenSkip = false
    var seenSkipWhile = false
    var seenTakeWhile = false
    var seenTake = false
    var allProjectionsPure = true
    var elementType = clone_type(top._type.firstType)
    var lastBindName = itName
    for (i in 0 .. intermediateCount) {
        var cll & = unsafe(calls[i])
        let opName = cll._1.name
        if (opName == "where_") {
            // skip/take/skip_while/take_while-after-where is rejected — canonical chain order is
            if (seenSkip || seenSkipWhile || seenTakeWhile || seenTake) return null
            var predicate : Expression?
            if (seenSelect) {
                // Phase 3d / single-eval: where-after-select. Bind the current projection
                if (has_sideeffects(projection)) return null
                if (lane != LinqLane.COUNTER) {
                    let wbName = "`vw`{at.line}`{at.column}`{length(preCondStmts)}"
                    var projType = clone_type(elementType)
                    preCondStmts |> push <| qmacro_expr() {
                        var $i(wbName) : $t(projType) := $e(projection)
                    }
                    // Replace projection with a typed ExprVar so downstream typer passes
                    var pvar = new ExprVar(at = at, name := wbName)
                    pvar._type = clone_type(elementType)
                    pvar._type.flags.ref = true
                    projection = pvar
                }
                predicate = peel_lambda_replace_var(cll._0.arguments[1], projection)
            } else {
                predicate = peel_lambda_rename_var(cll._0.arguments[1], itName)
            }
            if (whereCond == null) {
                whereCond = predicate
            } else {
                whereCond = qmacro($e(whereCond) && $e(predicate))
            }
        } elif (opName == "select") {
            if (seenSkip || seenSkipWhile || seenTakeWhile || seenTake) return null
            // Chained selects: bind the previous projection to a fresh local now so the next
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
            projection = peel_lambda_rename_var(cll._0.arguments[1], lastBindName)
            elementType = clone_type(cll._0._type.firstType)
            seenSelect = true
        } elif (opName == "skip") {
            // Canonical chain: at most one skip, before any skip_while/take_while/take.
            if (seenSkip || seenSkipWhile || seenTakeWhile || seenTake) return null
            var skipArg = cll._0.arguments[1]
            if (skipArg == null || skipArg._type == null || skipArg._type.baseType != Type.tInt) return null
            skipExpr = clone_expression(skipArg)
            seenSkip = true
        } elif (opName == "skip_while") {
            // pred uses itName; seenSelect bails (chained-bind peel is a follow-up). Canonical: after skip, before take_while/take.
            if (seenSelect || seenSkipWhile || seenTakeWhile || seenTake) return null
            var swArg = cll._0.arguments[1]
            if (swArg == null) return null
            skipWhileCond = peel_lambda_rename_var(swArg, itName)
            if (skipWhileCond == null) return null
            seenSkipWhile = true
        } elif (opName == "take_while") {
            // take_while pred sees source element (itName). Same select-cascade rule as skip_while.
            if (seenSelect || seenTakeWhile || seenTake) return null
            var twArg = cll._0.arguments[1]
            if (twArg == null) return null
            takeWhileCond = peel_lambda_rename_var(twArg, itName)
            if (takeWhileCond == null) return null
            seenTakeWhile = true
        } elif (opName == "take") {
            if (seenTake) return null
            var takeArg = cll._0.arguments[1]
            if (takeArg == null || takeArg._type == null || takeArg._type.baseType != Type.tInt) return null
            takeExpr = clone_expression(takeArg)
            seenTake = true
        } elif (is_buffer_required_op(opName)) {     // nolint:LINT009
            // TODO Phase 2X: BufferTopN (order_by + take/skip), BufferDistinct (distinct/_by),
            return null
        } else {
            return null
        }
    }
    if (projection != null && has_sideeffects(projection)) {
        allProjectionsPure = false
    }
    let noLimits = skipExpr == null && takeExpr == null && skipWhileCond == null && takeWhileCond == null
    // Count-shaped shortcut: when terminator is `count` (→ int) or `long_count` (→ int64),
    let isCountShaped = (lane == LinqLane.COUNTER
        || (lane == LinqLane.ACCUMULATOR && lastName == "long_count"))
    if (isCountShaped && whereCond == null && allProjectionsPure && noLimits
            && type_has_length(top._type))
        return emit_length_shortcut(lastName, top, srcName, at)
    // Ring 1: accumulator lane builds its own per-op loop body (typed accumulator, optional
    if (lane == LinqLane.ACCUMULATOR) {
        var laneTops : array<Expression?>
        laneTops |> push(top)
        var laneSrcs : array<string>
        laneSrcs |> push(srcName)
        return emit_accumulator_lane(lastName, laneTops, projection, whereCond,
            intermediateBinds, preCondStmts, elementType, laneSrcs, accName, itName, skipName, takeCountName,
            skippingName, skipExpr, takeExpr, skipWhileCond, takeWhileCond, at)
    }
    // Ring 2: early-exit lane — `any` no-pred + no upstream work + no limits + length-bearing
    if (lane == LinqLane.EARLY_EXIT) {
        let terminatorCall = calls.back()._0
        let isAnyNoPred = lastName == "any" && length(terminatorCall.arguments) == 1
        if (isAnyNoPred && whereCond == null && allProjectionsPure && noLimits
                && type_has_length(top._type))
            return emit_any_empty_shortcut(top, srcName, at)
        var laneTops : array<Expression?>
        laneTops |> push(top)
        var laneSrcs : array<string>
        laneSrcs |> push(srcName)
        return emit_early_exit_lane(lastName, laneTops, projection, whereCond,
            intermediateBinds, preCondStmts, elementType, terminatorCall, laneSrcs, itName, skipName,
            takeCountName, skippingName, skipExpr, takeExpr, skipWhileCond, takeWhileCond, at)
    }
    // Build the per-element loop body for COUNTER / ARRAY. Both lanes follow the same shape:
    var loopBody : Expression?
    if (counterLane) {
        // Counter lane must evaluate the projection (and any chained intermediates) per
        var stmts : array<Expression?>
        if (projection != null && has_sideeffects(projection)) {
            let finalBindName = "`vfinal`{at.line}`{at.column}"
            stmts |> push <| qmacro_expr() {
                var $i(finalBindName) = $e(projection)
            }
        }
        stmts |> push <| qmacro_expr() {
            $i(accName) ++
        }
        prepend_binds(stmts, intermediateBinds)
        wrap_with_ranges(stmts, skipExpr, takeExpr, skipWhileCond, takeWhileCond, skipName, takeCountName, skippingName)
        loopBody = prepend_precond(wrap_with_condition(stmts_to_expr(stmts), whereCond), preCondStmts)
    } else {
        // Array lane. `push_clone` is the safe append everywhere: for workhorse types it's a
        var stmts : array<Expression?>
        if (projection != null) {
            stmts |> push <| qmacro_expr() {
                $i(accName) |> push_clone($e(projection))
            }
        } elif (whereCond != null || skipExpr != null || takeExpr != null
                || skipWhileCond != null || takeWhileCond != null) {
            // Identity push: `it` aliases the source element. Reached when chain is bare
            stmts |> push <| qmacro_expr() {
                $i(accName) |> push_clone($i(itName))
            }
        } else {
            // identity chain — nothing to fuse; let the caller fall through.
            return null
        }
        prepend_binds(stmts, intermediateBinds)
        wrap_with_ranges(stmts, skipExpr, takeExpr, skipWhileCond, takeWhileCond, skipName, takeCountName, skippingName)
        loopBody = prepend_precond(wrap_with_condition(stmts_to_expr(stmts), whereCond), preCondStmts)
    }
    if (counterLane) {
        return emit_counter_lane(top, srcName, accName, itName, skipName, takeCountName, skippingName,
            skipExpr, takeExpr, skipWhileCond, loopBody, at)
    } else {
        return emit_array_lane(top, expr, loopBody, elementType, srcName, accName, itName,
            skipName, takeCountName, skippingName, skipExpr, takeExpr, skipWhileCond, at)
    }
}

[macro_function]
def private plan_reverse(var expr : Expression?) : Expression? {
    var (top, calls) = flatten_linq(expr)
    if (empty(calls)) return null
    top = peel_each(top)
    var terminatorName : string  = ""
    var terminatorCall : ExprCall?
    {
        let lastName = calls.back()._1.name
        if (lastName == "count" || lastName == "first" || lastName == "first_or_default") {
            terminatorName = lastName
            terminatorCall = calls.back()._0
            calls |> pop
        }
    }
    if (empty(calls)) return null
    var whereCond : Expression?
    var projection : Expression?
    var hasReverse = false
    var seenSelect = false
    var takeExpr : Expression?
    let at = calls[0]._0.at
    let srcName = "`source`{at.line}`{at.column}"
    let itName  = "`it`{at.line}`{at.column}"
    let bufName = "`buf`{at.line}`{at.column}"
    let foundName = "`found`{at.line}`{at.column}"
    let lastName = "`last`{at.line}`{at.column}"
    let cntName = "`cnt`{at.line}`{at.column}"
    let dBindName = "`d`{at.line}`{at.column}"
    var reverseCall : ExprCall?
    for (i in 0 .. length(calls)) {
        var cll & = unsafe(calls[i])
        let name = cll._1.name
        if (name == "where_") {
            if (hasReverse || seenSelect) return null
            whereCond = merge_where_cond(whereCond, peel_lambda_rename_var(cll._0.arguments[1], itName))
        } elif (name == "select") {
            if (hasReverse || seenSelect) return null
            seenSelect = true
            projection = peel_lambda_rename_var(cll._0.arguments[1], itName)
        } elif (name == "reverse") {
            if (hasReverse) return null
            hasReverse = true
            reverseCall = cll._0
        } elif (name == "take") {
            if (!hasReverse || takeExpr != null) return null
            var arg = cll._0.arguments[1]
            if (arg == null || arg._type == null || arg._type.baseType != Type.tInt) return null
            takeExpr = clone_expression(arg)
        } else {
            return null
        }
    }
    if (!hasReverse || (takeExpr != null && terminatorName != "")) return null
    var body : Expression?
    if (terminatorName == "count") {
        // Reverse is identity for count — counter loop, no buffer. Side-effecting projection still fires per match.
        var perElement : Expression?
        if (projection != null && has_sideeffects(projection)) {
            let vfinalName = "`vfinal`{at.line}`{at.column}"
            perElement = qmacro_block() {
                var $i(vfinalName) = $e(projection)
                $i(cntName) ++
            }
        } else {
            perElement = qmacro_expr() {
                $i(cntName) ++
            }
        }
        perElement = wrap_with_condition(perElement, whereCond)
        body = qmacro_block() {
            var $i(cntName) = 0
            for ($i(itName) in $i(srcName)) {
                $e(perElement)
            }
            return $i(cntName)
        }
    } elif (terminatorName == "first" || terminatorName == "first_or_default") {
        // "first of reversed" = LAST surviving element. Walk source, overwrite `last` on each match. No buffer.
        var lastType = strip_const_ref(clone_type(projection != null ? projection._type : top._type.firstType))
        var valueExpr : Expression?
        if (projection != null) {
            valueExpr = clone_expression(projection)
        } else {
            valueExpr = qmacro_expr() {
                $i(itName)
            }
        }
        var matchBlock : Expression? = qmacro_block() {
            $i(lastName) := $e(valueExpr)
            $i(foundName) = true
        }
        var perElement = wrap_with_condition(matchBlock, whereCond)
        if (terminatorName == "first") {
            body = qmacro_block() {
                var $i(foundName) = false
                var $i(lastName) : $t(lastType) = default<$t(lastType)>
                for ($i(itName) in $i(srcName)) {
                    $e(perElement)
                }
                if (!$i(foundName)) {
                    panic("sequence contains no elements")
                }
                return $i(lastName)
            }
        } else {
            var defaultExpr = clone_expression(terminatorCall.arguments[1])
            body = qmacro_block() {
                let $i(dBindName) = $e(defaultExpr)
                var $i(foundName) = false
                var $i(lastName) : $t(lastType) = default<$t(lastType)>
                for ($i(itName) in $i(srcName)) {
                    $e(perElement)
                }
                return $i(foundName) ? $i(lastName) : $i(dBindName)
            }
        }
    } else {
        // R1-R4 path: buffer + reverse_inplace + optional resize + return buffer.
        let needIterWrap = expr._type.isIterator
        var bufElemType = strip_const_ref(clone_type(reverseCall._type.firstType))
        let canBackwardIndex = (takeExpr != null && projection == null && whereCond == null
                && (top._type.isGoodArrayType || top._type.isArray))
        if (canBackwardIndex) {
            // R6: visit only the last takeN indices — skips full-source push + O(length) reverse_inplace.
            let lenName   = "`rlen`{at.line}`{at.column}"
            let takeNName = "`rtn`{at.line}`{at.column}"
            let kName     = "`rk`{at.line}`{at.column}"
            var takeA = clone_expression(takeExpr)
            var takeB = clone_expression(takeExpr)
            var takeC = clone_expression(takeExpr)
            var returnExpr = buffer_return(bufName, needIterWrap)
            body = qmacro_block() {
                let $i(lenName) = length($i(srcName))
                let $i(takeNName) = $e(takeA) <= 0 ? 0 : ($e(takeB) < $i(lenName) ? $e(takeC) : $i(lenName))
                var $i(bufName) : array<$t(bufElemType)>
                $i(bufName) |> reserve($i(takeNName))
                for ($i(kName) in 0 .. $i(takeNName)) {
                    $i(bufName) |> push_clone($i(srcName)[$i(lenName) - 1 - $i(kName)])
                }
                $e(returnExpr)
            }
        } else {
            var pushExpr : Expression?
            if (projection != null) {
                pushExpr = qmacro_expr() {
                    $i(bufName) |> push_clone($e(projection))
                }
            } else {
                pushExpr = qmacro_expr() {
                    $i(bufName) |> push_clone($i(itName))
                }
            }
            pushExpr = wrap_with_condition(pushExpr, whereCond)
            var reserveStmts : array<Expression?>
            if (type_has_length(top._type) && whereCond == null) {
                reserveStmts |> push <| qmacro_expr() {
                    $i(bufName) |> reserve(length($i(srcName)))
                }
            }
            var resizeStmts : array<Expression?>
            if (takeExpr != null) {
                // take(N) of reversed-buffer = last N of source reversed. Three clones since no math::min import.
                var takeA = clone_expression(takeExpr)
                var takeB = clone_expression(takeExpr)
                var takeC = clone_expression(takeExpr)
                resizeStmts |> push <| qmacro_expr() {
                    $i(bufName) |> resize($e(takeA) <= 0 ? 0 : ($e(takeB) < length($i(bufName)) ? $e(takeC) : length($i(bufName))))
                }
            }
            var returnExpr = buffer_return(bufName, needIterWrap)
            body = qmacro_block() {
                var $i(bufName) : array<$t(bufElemType)>
                $b(reserveStmts)
                for ($i(itName) in $i(srcName)) {
                    $e(pushExpr)
                }
                _::reverse_inplace($i(bufName))
                $b(resizeStmts)
                $e(returnExpr)
            }
        }
    }
    return finalize_emission(top, srcName, at, body)
}

[macro_function]
def private plan_distinct(var expr : Expression?) : Expression? {
    var (top, calls) = flatten_linq(expr)
    if (empty(calls)) return null
    top = peel_each(top)
    var terminatorName : string  = ""
    {
        let lastName = calls.back()._1.name
        // 1-arg form only — `count(src, predicate)` / `long_count(src, predicate)` would silently drop the predicate (splice emits `length(seen)`). Bail to tier-2 which correctly evaluates the predicate.
        if ((lastName == "count" || lastName == "sum" || lastName == "long_count")
                && length(calls.back()._0.arguments) == 1) {
            terminatorName = lastName
            calls |> pop
        }
    }
    if (empty(calls)) return null
    var whereCond : Expression?
    var projection : Expression?
    var hasDistinct = false
    var seenSelect = false
    var isDistinctBy = false
    var distinctKeyBlock : Expression?
    var takeExpr : Expression?
    let at = calls[0]._0.at
    let srcName = "`source`{at.line}`{at.column}"
    let itName  = "`it`{at.line}`{at.column}"
    let bufName = "`buf`{at.line}`{at.column}"
    let seenName = "`seen`{at.line}`{at.column}"
    let keyName = "`k`{at.line}`{at.column}"
    let takenName = "`taken`{at.line}`{at.column}"
    let accName = "`acc`{at.line}`{at.column}"
    for (i in 0 .. length(calls)) {
        var cll & = unsafe(calls[i])
        let name = cll._1.name
        if (name == "where_") {
            if (hasDistinct || seenSelect) return null
            whereCond = merge_where_cond(whereCond, peel_lambda_rename_var(cll._0.arguments[1], itName))
        } elif (name == "select") {
            if (hasDistinct || seenSelect) return null
            seenSelect = true
            projection = peel_lambda_rename_var(cll._0.arguments[1], itName)
        } elif (name == "distinct") {
            if (hasDistinct) return null
            hasDistinct = true
        } elif (name == "distinct_by") {
            if (hasDistinct) return null
            hasDistinct = true
            isDistinctBy = true
            distinctKeyBlock = clone_expression(cll._0.arguments[1])
        } elif (name == "take") {
            if (!hasDistinct || takeExpr != null) return null
            var arg = cll._0.arguments[1]
            if (arg == null || arg._type == null || arg._type.baseType != Type.tInt) return null
            takeExpr = clone_expression(arg)
        } else {
            return null
        }
    }
    if (!hasDistinct || (takeExpr != null && terminatorName != "")) return null
    var elemType = strip_const_ref(clone_type(projection != null ? projection._type : top._type.firstType))
    var stmts : array<Expression?>
    if (isDistinctBy) {
        var keyTypeBlock = clone_expression(distinctKeyBlock)
        stmts |> push <| qmacro_expr() {
            var inscope $i(seenName) : table<typedecl(_::unique_key(invoke($e(keyTypeBlock), default<$t(elemType)>)))>
        }
    } else {
        stmts |> push <| qmacro_expr() {
            var inscope $i(seenName) : table<typedecl(_::unique_key(default<$t(elemType)>))>
        }
    }
    let isCountTerminator = terminatorName == "count" || terminatorName == "long_count"
    let isSumTerminator = terminatorName == "sum"
    let needBuffer = !isCountTerminator && !isSumTerminator
    if (needBuffer) {
        stmts |> push <| qmacro_expr() {
            var $i(bufName) : array<$t(elemType)>
        }
    } elif (isSumTerminator) {
        stmts |> push <| qmacro_expr() {
            var $i(accName) : $t(elemType) = default<$t(elemType)>
        }
    }
    // Bind take(N) limit once at outer scope so a side-effecting arg fires once, not on every fresh-key check.
    let takeLimName = "`takeLim`{at.line}`{at.column}"
    if (takeExpr != null) {
        var takeBind = clone_expression(takeExpr)
        stmts |> push <| qmacro_expr() {
            let $i(takeLimName) = $e(takeBind)
        }
        stmts |> push <| qmacro_expr() {
            var $i(takenName) = 0
        }
    }
    // Bind side-effecting projection once per element; key + buffer/acc share the bind (matches original LINQ's one-eval per source elem).
    let pvName = "`pv`{at.line}`{at.column}"
    let bindProjection = projection != null && has_sideeffects(projection)
    var pushExpr : Expression?
    if (projection != null) {
        if (bindProjection) {
            pushExpr = qmacro_expr() {
                $i(pvName)
            }
        } else {
            pushExpr = clone_expression(projection)
        }
    } else {
        pushExpr = qmacro_expr() {
            $i(itName)
        }
    }
    var keyExpr : Expression?
    if (isDistinctBy) {
        var keyBlk = clone_expression(distinctKeyBlock)
        var keyArg = clone_expression(pushExpr)
        keyExpr = qmacro_expr() {
            invoke($e(keyBlk), $e(keyArg))
        }
    } else {
        keyExpr = clone_expression(pushExpr)
    }
    // Per-match consume: only inside the fresh-key branch; take-break is the outer-loop guard below.
    var consumeStmts : array<Expression?>
    if (takeExpr != null) {
        consumeStmts |> push <| qmacro_expr() {
            $i(takenName) ++
        }
    }
    consumeStmts |> push <| qmacro_expr() {
        $i(seenName) |> insert($i(keyName))
    }
    if (needBuffer) {
        var pushClone = clone_expression(pushExpr)
        consumeStmts |> push <| qmacro_expr() {
            $i(bufName) |> push_clone($e(pushClone))
        }
    } elif (isSumTerminator) {
        var sumExpr = clone_expression(pushExpr)
        consumeStmts |> push <| qmacro_expr() {
            $i(accName) += $e(sumExpr)
        }
    }
    var consumeBody = stmts_to_expr(consumeStmts)
    var ifNew : Expression? = qmacro_expr() {
        if (!$i(seenName) |> key_exists($i(keyName))) {
            $e(consumeBody)
        }
    }
    var perMatchStmts : array<Expression?>    // nolint:STYLE012 — array-literal-of-qmacro_expr is parse-fragile
    if (bindProjection) {
        var pvBind = clone_expression(projection)
        perMatchStmts |> push <| qmacro_expr() {
            let $i(pvName) = $e(pvBind)
        }
    }
    perMatchStmts |> push <| qmacro_expr() {
        let $i(keyName) = _::unique_key($e(keyExpr))
    }
    perMatchStmts |> push(ifNew)
    var perElement = stmts_to_expr(perMatchStmts)
    perElement = wrap_with_condition(perElement, whereCond)
    // Outer-loop take guard: top-of-loop break for true O(N)-source streaming exit (skips duplicates after Nth distinct).
    if (takeExpr != null) {
        stmts |> push <| qmacro_expr() {
            for ($i(itName) in $i(srcName)) {
                if ($i(takenName) >= $i(takeLimName)) {
                    break
                }
                $e(perElement)
            }
        }
    } else {
        stmts |> push <| qmacro_expr() {
            for ($i(itName) in $i(srcName)) {
                $e(perElement)
            }
        }
    }
    if (terminatorName == "count") {
        stmts |> push <| qmacro_expr() {
            return length($i(seenName))
        }
    } elif (terminatorName == "long_count") {
        stmts |> push <| qmacro_expr() {
            return int64(length($i(seenName)))
        }
    } elif (isSumTerminator) {
        stmts |> push <| qmacro_expr() {
            return $i(accName)
        }
    } else {
        stmts |> push(buffer_return(bufName, expr._type.isIterator))
    }
    return finalize_emission_stmts(top, srcName, at, stmts)
}

// ── Group-by helpers ──────────────────────────────────────────────────

[macro_function]
def private is_bucket_reducer_call(expr : Expression?; bindName : string) : tuple<bool; string; Expression?> {
    // Returns (matched, reducerName, innerLambda). innerLambda is non-null for *_inner_select forms.
    if (expr == null || !(expr is ExprCall)) return (false, "", null)
    let c = expr as ExprCall
    if (c.func == null) return (false, "", null)
    var fnName = string(c.func.name)
    if (c.func.fromGeneric != null) {
        fnName = string(c.func.fromGeneric.name)
    }
    if ((c.arguments |> length) != 1) return (false, "", null)
    // Bare reducer: `<reducer>(<bind>._1)`
    if (fnName == "length" || fnName == "count" || fnName == "long_count"
            || fnName == "sum" || fnName == "min" || fnName == "max" || fnName == "first" || fnName == "average") {
        if (peel_tuple_field_read(c.arguments[0], bindName, 1)) return (true, fnName, null)
    }
    // Inner-select reducer: `<reducer>(select(<bind>._1, <lambda>))`
    if (fnName == "sum" || fnName == "min" || fnName == "max" || fnName == "first" || fnName == "average") {
        let inner = c.arguments[0]
        if (inner != null && inner is ExprCall) {
            let innerCall = inner as ExprCall
            if (innerCall.func != null) {
                var innerName = string(innerCall.func.name)
                if (innerCall.func.fromGeneric != null) {
                    innerName = string(innerCall.func.fromGeneric.name)
                }
                if (innerName == "select"
                        && (innerCall.arguments |> length) == 2
                        && peel_tuple_field_read(innerCall.arguments[0], bindName, 1)) return (true, "{fnName}_inner_select", innerCall.arguments[1])
            }
        }
    }
    return (false, "", null)
}

struct private ReducerSpec {
    slot : int                  // target tuple slot: entry._{slot}
    name : string               // bare reducer or "<reducer>_inner_select"
    innerBody : Expression?     // peeled inner-select body, null for bare reducer
    accType : TypeDeclPtr       // type of entry._{slot} (for synth table type + workhorse check)
}

[macro_function]
def private is_average_spec(spec : ReducerSpec) : bool {
    return spec.name == "average" || spec.name == "average_inner_select"
}

[macro_function]
def private mk_avg_acc_type(at : LineInfo) : TypeDeclPtr {
    // tuple<double; uint64> — sum + count, lives in one entry slot for cache locality.
    var t = new TypeDecl(baseType = Type.tTuple, at = at)
    t.argTypes |> emplace_new <| new TypeDecl(baseType = Type.tDouble, at = at)
    t.argTypes |> emplace_new <| new TypeDecl(baseType = Type.tUInt64, at = at)
    return t
}

[macro_function]
def private slot_acc_type(bodyType : TypeDeclPtr; slot : int; isBareForm, isAverage : bool; at : LineInfo) : TypeDeclPtr {
    // average overrides the user-facing type (double) with the 2-slot tuple acc.
    if (isAverage) return mk_avg_acc_type(at)
    // Bare form: bodyType IS the reducer's return type. Named-tuple: drill into argTypes[slot].
    if (isBareForm) return strip_const_ref(clone_type(bodyType))
    if (bodyType == null || (bodyType.argTypes |> length) <= slot) return null
    return strip_const_ref(clone_type(bodyType.argTypes[slot]))
}

[macro_function]
def private recognize_reducer_specs(var groupProjBody : Expression?; bindName, itName : string) {
    // Returns (specs, usesNamedTuple). Empty specs = unrecognized → caller cascades.
    var specs : array<ReducerSpec>
    if (groupProjBody == null) return <- (<- specs, false)
    // Bare form: body is the reducer call directly.
    let (okBare, rnBare, innerBare) = is_bucket_reducer_call(groupProjBody, bindName)
    if (okBare) {
        var innerBody : Expression?
        if (innerBare != null) {
            innerBody = peel_lambda_rename_var(clone_expression(innerBare), itName)
            if (innerBody == null) return <- (<- specs, false)
        }
        let isAvgBare = rnBare == "average" || rnBare == "average_inner_select"
        var accType : TypeDeclPtr = slot_acc_type(groupProjBody._type, 1, true, isAvgBare, groupProjBody.at)
        if (accType == null) return <- (<- specs, false)
        var spec = ReducerSpec(slot = 1, name = rnBare, innerBody = innerBody, accType = accType)
        specs |> push(spec)
        return <- (<- specs, false)
    }
    // Named-tuple form: slot 0 = key (must be _._0), slots 1..N = reducers.
    if (!(groupProjBody is ExprMakeTuple)) return <- (<- specs, false)
    let mt = groupProjBody as ExprMakeTuple
    let n = mt.values |> length
    if (n < 2 || !peel_tuple_field_read(mt.values[0], bindName, 0)) return <- (<- specs, false)
    specs |> reserve(n - 1)
    for (i in range(1, n)) {
        let (ok, rn, innerLam) = is_bucket_reducer_call(mt.values[i], bindName)
        if (!ok) {
            specs |> clear
            return <- (<- specs, false)
        }
        var innerBody : Expression?
        if (innerLam != null) {
            innerBody = peel_lambda_rename_var(clone_expression(innerLam), itName)
            if (innerBody == null) {
                specs |> clear
                return <- (<- specs, false)
            }
        }
        let isAvg = rn == "average" || rn == "average_inner_select"
        var accType : TypeDeclPtr = slot_acc_type(groupProjBody._type, i, false, isAvg, groupProjBody.at)
        if (accType == null) {
            specs |> clear
            return <- (<- specs, false)
        }
        var spec = ReducerSpec(slot = i, name = rn, innerBody = innerBody, accType = accType)
        specs |> push(spec)
    }
    return <- (<- specs, true)
}

[macro_function]
def private mk_slot_ref(at : LineInfo; entryName : string; slot : int) : Expression? {
    // Programmatic `entry._{slot}` — qmacro has no dynamic-field-name splice.
    var ev = new ExprVar(at = at)
    ev.name := entryName
    var ef = new ExprField(at = at)
    ef.name := "_{slot}"
    ef.value = ev
    return ef
}

[macro_function]
def private emit_reducer_branches(spec : ReducerSpec; at : LineInfo; entryName, itName : string) : tuple<Expression?; Expression?> {
    // (missInit, hitUpdate); hitUpdate null for first* (subsequent same-key elements ignored).
    var missInit : Expression?
    var hitUpdate : Expression?
    let nm = spec.name
    if (nm == "length" || nm == "count") {
        var l1 = mk_slot_ref(at, entryName, spec.slot)
        var l2 = mk_slot_ref(at, entryName, spec.slot)
        missInit = qmacro_expr() {
            $e(l1) = 1
        }
        hitUpdate = qmacro_expr() {
            $e(l2) ++
        }
    } elif (nm == "long_count") {
        var l1 = mk_slot_ref(at, entryName, spec.slot)
        var l2 = mk_slot_ref(at, entryName, spec.slot)
        missInit = qmacro_expr() {
            $e(l1) = 1l
        }
        hitUpdate = qmacro_expr() {
            $e(l2) ++
        }
    } elif (nm == "sum") {
        var l1 = mk_slot_ref(at, entryName, spec.slot)
        var l2 = mk_slot_ref(at, entryName, spec.slot)
        missInit = qmacro_expr() {
            $e(l1) = $i(itName)
        }
        hitUpdate = qmacro_expr() {
            $e(l2) += $i(itName)
        }
    } elif (nm == "sum_inner_select") {
        var l1 = mk_slot_ref(at, entryName, spec.slot)
        var l2 = mk_slot_ref(at, entryName, spec.slot)
        var b1 = clone_expression(spec.innerBody)
        var b2 = clone_expression(spec.innerBody)
        missInit = qmacro_expr() {
            $e(l1) = $e(b1)
        }
        hitUpdate = qmacro_expr() {
            $e(l2) += $e(b2)
        }
    } elif (nm == "min" || nm == "max") {
        let workhorse = spec.accType != null && spec.accType.isWorkhorseType
        let isMin = nm == "min"
        var lInit = mk_slot_ref(at, entryName, spec.slot)
        var lCmp  = mk_slot_ref(at, entryName, spec.slot)
        var lAsn  = mk_slot_ref(at, entryName, spec.slot)
        missInit = qmacro_expr() {
            $e(lInit) := $i(itName)
        }
        if (workhorse && isMin) {
            hitUpdate = qmacro_expr() {
                if ($i(itName) < $e(lCmp)) {
                    $e(lAsn) := $i(itName)
                }
            }
        } elif (workhorse) {
            hitUpdate = qmacro_expr() {
                if ($i(itName) > $e(lCmp)) {
                    $e(lAsn) := $i(itName)
                }
            }
        } elif (isMin) {
            hitUpdate = qmacro_expr() {
                if (_::less($i(itName), $e(lCmp))) {
                    $e(lAsn) := $i(itName)
                }
            }
        } else {
            hitUpdate = qmacro_expr() {
                if (_::less($e(lCmp), $i(itName))) {
                    $e(lAsn) := $i(itName)
                }
            }
        }
    } elif (nm == "min_inner_select" || nm == "max_inner_select") {
        let workhorse = spec.accType != null && spec.accType.isWorkhorseType
        let isMin = nm == "min_inner_select"
        // Hit branch evaluates innerBody once into a temp — matches reference's single-eval-per-element.
        let tmp = "`vis`{at.line}`{at.column}`{spec.slot}"
        var lInit = mk_slot_ref(at, entryName, spec.slot)
        var lCmp  = mk_slot_ref(at, entryName, spec.slot)
        var lAsn  = mk_slot_ref(at, entryName, spec.slot)
        var bMiss = clone_expression(spec.innerBody)
        var bHit  = clone_expression(spec.innerBody)
        missInit = qmacro_expr() {
            $e(lInit) := $e(bMiss)
        }
        if (workhorse && isMin) {
            hitUpdate = qmacro_block() {
                let $i(tmp) = $e(bHit)
                if ($i(tmp) < $e(lCmp)) {
                    $e(lAsn) := $i(tmp)
                }
            }
        } elif (workhorse) {
            hitUpdate = qmacro_block() {
                let $i(tmp) = $e(bHit)
                if ($i(tmp) > $e(lCmp)) {
                    $e(lAsn) := $i(tmp)
                }
            }
        } elif (isMin) {
            hitUpdate = qmacro_block() {
                let $i(tmp) = $e(bHit)
                if (_::less($i(tmp), $e(lCmp))) {
                    $e(lAsn) := $i(tmp)
                }
            }
        } else {
            hitUpdate = qmacro_block() {
                let $i(tmp) = $e(bHit)
                if (_::less($e(lCmp), $i(tmp))) {
                    $e(lAsn) := $i(tmp)
                }
            }
        }
    } elif (nm == "first") {
        var l1 = mk_slot_ref(at, entryName, spec.slot)
        missInit = qmacro_expr() {
            $e(l1) := $i(itName)
        }
    } elif (nm == "first_inner_select") {
        var l1 = mk_slot_ref(at, entryName, spec.slot)
        var b1 = clone_expression(spec.innerBody)
        missInit = qmacro_expr() {
            $e(l1) := $e(b1)
        }
    } elif (nm == "average") {
        // 2-slot acc: ._0 = sum (double), ._1 = count (uint64). Source-element cast to double once per side.
        var lInit = mk_slot_ref(at, entryName, spec.slot)
        var lSum  = mk_slot_ref(at, entryName, spec.slot)
        var lCnt  = mk_slot_ref(at, entryName, spec.slot)
        missInit = qmacro_expr() {
            $e(lInit) = (double($i(itName)), 1ul)
        }
        hitUpdate = qmacro_block() {
            $e(lSum)._0 += double($i(itName))
            $e(lCnt)._1 ++
        }
    } elif (nm == "average_inner_select") {
        // Inner projection evaluated once into a temp — matches reference single-eval-per-element.
        let tmp = "`vavg`{at.line}`{at.column}`{spec.slot}"
        var lInit = mk_slot_ref(at, entryName, spec.slot)
        var lSum  = mk_slot_ref(at, entryName, spec.slot)
        var lCnt  = mk_slot_ref(at, entryName, spec.slot)
        var bMiss = clone_expression(spec.innerBody)
        var bHit  = clone_expression(spec.innerBody)
        missInit = qmacro_block() {
            let $i(tmp) = $e(bMiss)
            $e(lInit) = (double($i(tmp)), 1ul)
        }
        hitUpdate = qmacro_block() {
            let $i(tmp) = $e(bHit)
            $e(lSum)._0 += double($i(tmp))
            $e(lCnt)._1 ++
        }
    }
    return (missInit, hitUpdate)
}

[macro_function]
def private mk_avg_subfield(at : LineInfo; entryName : string; slot, sub : int) : Expression? {
    // entry._{slot}._{sub} — 2-level field access into a tuple<double; uint64> avg accumulator.
    var ev = new ExprVar(at = at)
    ev.name := entryName
    var ef1 = new ExprField(at = at)
    ef1.name := "_{slot}"
    ef1.value = ev
    var ef2 = new ExprField(at = at)
    ef2.name := "_{sub}"
    ef2.value = ef1
    return ef2
}

[macro_function]
def private mk_avg_divide_expr(at : LineInfo; entryName : string; slot : int) : Expression? {
    // entry._{slot}._0 / double(entry._{slot}._1) — average from the 2-slot per-key acc. Bucket is non-empty by construction (only keys with ≥1 source element enter the table), so count is always ≥1.
    var sumRef = mk_avg_subfield(at, entryName, slot, 0)
    var cntRef = mk_avg_subfield(at, entryName, slot, 1)
    return qmacro_expr() {
        $e(sumRef) / double($e(cntRef))
    }
}

[macro_function]
def private mk_slot_output_expr(at : LineInfo; entryName : string; spec : ReducerSpec) : Expression? {
    // Per-spec output value: divide for average slots, plain field ref otherwise.
    if (is_average_spec(spec)) return mk_avg_divide_expr(at, entryName, spec.slot)
    return mk_slot_ref(at, entryName, spec.slot)
}

[macro_function]
def private mk_hidden_acc_type(reducerName : string; isInnerSelect : bool;
                               bucketElemType, innerBodyType : TypeDeclPtr; at : LineInfo) : TypeDeclPtr {
    // Hidden-slot acc type: like slot_acc_type but takes source types directly (hidden slots have no group_proj body to drill into).
    if (reducerName == "length" || reducerName == "count") return new TypeDecl(baseType = Type.tInt, at = at)
    if (reducerName == "long_count") return new TypeDecl(baseType = Type.tInt64, at = at)
    if (reducerName == "average" || reducerName == "average_inner_select") return mk_avg_acc_type(at)
    let src = isInnerSelect ? innerBodyType : bucketElemType
    if (src == null) return null
    return strip_const_ref(clone_type(src))
}

[macro_function]
def private having_reducer_append_if_missing(var expr : Expression?; hbName, itName : string;
                                             var specs : array<ReducerSpec>; userVisibleSlotCount : int;
                                             bucketElemType : TypeDeclPtr; at : LineInfo) : bool {
    // Append a hidden ReducerSpec when `expr` is a bare/inner-select reducer on the having bind that has no name-match in `specs`. Returns false only on inner-select peel failure or on a second same-named hidden inner-select reducer (caller cascades — the splice can't distinguish two different inner lambdas, so we'd silently route both predicate terms to the first hidden slot).
    if (expr == null || !(expr is ExprCall)) return true
    let c = expr as ExprCall
    if (c.func == null || (c.arguments |> length) != 1) return true
    var fnName = string(c.func.name)
    if (c.func.fromGeneric != null) {
        fnName = string(c.func.fromGeneric.name)
    }
    let isReducer = (fnName == "length" || fnName == "count" || fnName == "long_count"
            || fnName == "sum" || fnName == "min" || fnName == "max"
            || fnName == "first" || fnName == "average")
    if (!isReducer) return true
    if (peel_tuple_field_read(c.arguments[0], hbName, 1)) {
        for (s in specs) {
            if (s.innerBody == null && s.name == fnName) return true
        }
        var accType = mk_hidden_acc_type(fnName, false, bucketElemType, null, at)
        if (accType == null) return true
        let newSlot = (specs |> length) + 1
        specs |> push <| ReducerSpec(slot = newSlot, name = fnName, innerBody = null, accType = accType)
        return true
    }
    let canInnerSelect = (fnName == "sum" || fnName == "min" || fnName == "max"
            || fnName == "first" || fnName == "average")
    if (!canInnerSelect) return true
    let inner = c.arguments[0]
    if (inner == null || !(inner is ExprCall)) return true
    let ic = inner as ExprCall
    if (ic.func == null || (ic.arguments |> length) != 2 || !peel_tuple_field_read(ic.arguments[0], hbName, 1)) return true
    var innerName = string(ic.func.name)
    if (ic.func.fromGeneric != null) {
        innerName = string(ic.func.fromGeneric.name)
    }
    if (innerName != "select") return true
    let comboName = "{fnName}_inner_select"
    for (s in specs) {
        // Match against a select-side spec (visible slot): v1 trusts the user wrote identical lambdas (pre-existing PR-D limitation, documented in LINQ.md). Match against an already-added hidden spec from this having scan would silently route two different inner lambdas to the same slot — bail to tier 2 instead, where each select() is evaluated separately.
        if (s.name == comboName) return s.slot <= userVisibleSlotCount
    }
    var innerBody = peel_lambda_rename_var(clone_expression(ic.arguments[1]), itName)
    if (innerBody == null || innerBody._type == null) return false
    var accType = mk_hidden_acc_type(comboName, true, bucketElemType, innerBody._type, at)
    if (accType == null) return false
    let newSlot = (specs |> length) + 1
    specs |> push <| ReducerSpec(slot = newSlot, name = comboName, innerBody = innerBody, accType = accType)
    return true
}

[macro_function]
def private extend_specs_for_missing_having_reducers(var expr : Expression?; hbName, itName : string;
                                                     var specs : array<ReducerSpec>; userVisibleSlotCount : int;
                                                     bucketElemType : TypeDeclPtr; at : LineInfo) : bool {
    // Pre-rewrite walker that appends hidden ReducerSpecs for having-only reducers; unrecognized shapes leak to rewrite_having_pred (which bails on any surviving raw `<hb>`).
    if (expr == null) return true
    qm_peel_ref2value(expr)
    if (expr is ExprCall) {
        var c = expr as ExprCall
        if (!having_reducer_append_if_missing(expr, hbName, itName, specs, userVisibleSlotCount, bucketElemType, at)) return false
        for (i in 0 .. (c.arguments |> length)) {
            if (!extend_specs_for_missing_having_reducers(c.arguments[i], hbName, itName, specs, userVisibleSlotCount, bucketElemType, at)) return false
        }
        return true
    }
    if (expr is ExprField) {
        var f = expr as ExprField
        return extend_specs_for_missing_having_reducers(f.value, hbName, itName, specs, userVisibleSlotCount, bucketElemType, at)
    }
    if (expr is ExprOp1) {
        var op = expr as ExprOp1
        return extend_specs_for_missing_having_reducers(op.subexpr, hbName, itName, specs, userVisibleSlotCount, bucketElemType, at)
    }
    if (expr is ExprOp2) {
        var op = expr as ExprOp2
        if (!extend_specs_for_missing_having_reducers(op.left, hbName, itName, specs, userVisibleSlotCount, bucketElemType, at)) return false
        return extend_specs_for_missing_having_reducers(op.right, hbName, itName, specs, userVisibleSlotCount, bucketElemType, at)
    }
    if (expr is ExprOp3) {
        var op = expr as ExprOp3
        return (extend_specs_for_missing_having_reducers(op.subexpr, hbName, itName, specs, userVisibleSlotCount, bucketElemType, at)
                && extend_specs_for_missing_having_reducers(op.left, hbName, itName, specs, userVisibleSlotCount, bucketElemType, at)
                && extend_specs_for_missing_having_reducers(op.right, hbName, itName, specs, userVisibleSlotCount, bucketElemType, at))
    }
    return true
}

[macro_function]
def private rewrite_having_pred(var expr : Expression?; hbName, kvName : string; specs : array<ReducerSpec>) : Expression? {
    // Substitutes `<hb>._0` → `kv._0` and `<reducer>(<hb>._1)` / `<reducer>(select(<hb>._1, <lam>))` → `kv._{slot}` (or divide expr for average) against matching specs; any other use of `<hb>` returns null and cascades.
    if (expr == null) return expr
    qm_peel_ref2value(expr)
    if (expr is ExprCall) {
        var c = expr as ExprCall
        if (c.func != null && (c.arguments |> length) == 1) {
            var fnName = string(c.func.name)
            if (c.func.fromGeneric != null) {
                fnName = string(c.func.fromGeneric.name)
            }
            if (peel_tuple_field_read(c.arguments[0], hbName, 1)) {
                for (s in specs) {
                    if (s.innerBody == null && s.name == fnName) return mk_slot_output_expr(expr.at, kvName, s)
                }
                return null
            }
            let inner = c.arguments[0]
            if (inner != null && inner is ExprCall) {
                let ic = inner as ExprCall
                if (ic.func != null && (ic.arguments |> length) == 2 && peel_tuple_field_read(ic.arguments[0], hbName, 1)) {
                    var innerName = string(ic.func.name)
                    if (ic.func.fromGeneric != null) {
                        innerName = string(ic.func.fromGeneric.name)
                    }
                    if (innerName == "select") {
                        let comboName = "{fnName}_inner_select"
                        for (s in specs) {
                            if (s.name == comboName) return mk_slot_output_expr(expr.at, kvName, s)
                        }
                        return null
                    }
                }
            }
        }
        for (i in 0 .. (c.arguments |> length)) {
            var sub = rewrite_having_pred(c.arguments[i], hbName, kvName, specs)
            if (sub == null) return null
            c.arguments[i] = sub
        }
        return expr
    }
    if (expr is ExprField) {
        var f = expr as ExprField
        if (f.value != null && f.value is ExprVar) {
            let v = f.value as ExprVar
            if (v.name == hbName) {
                if (f.name == "_0") return mk_slot_ref(expr.at, kvName, 0)
                return null
            }
        }
        var sub = rewrite_having_pred(f.value, hbName, kvName, specs)
        if (sub == null) return null
        f.value = sub
        return expr
    }
    if (expr is ExprVar) {
        let v = expr as ExprVar
        if (v.name == hbName) return null
        return expr
    }
    if (expr is ExprOp1) {
        var op = expr as ExprOp1
        var sub = rewrite_having_pred(op.subexpr, hbName, kvName, specs)
        if (sub == null) return null
        op.subexpr = sub
        return expr
    }
    if (expr is ExprOp2) {
        var op = expr as ExprOp2
        var l = rewrite_having_pred(op.left, hbName, kvName, specs)
        if (l == null) return null
        op.left = l
        var r = rewrite_having_pred(op.right, hbName, kvName, specs)
        if (r == null) return null
        op.right = r
        return expr
    }
    if (expr is ExprOp3) {
        var op = expr as ExprOp3
        var s = rewrite_having_pred(op.subexpr, hbName, kvName, specs)
        if (s == null) return null
        op.subexpr = s
        var l = rewrite_having_pred(op.left, hbName, kvName, specs)
        if (l == null) return null
        op.left = l
        var r = rewrite_having_pred(op.right, hbName, kvName, specs)
        if (r == null) return null
        op.right = r
        return expr
    }
    return expr
}

class private HbScanner : AstVisitor {
    // Safety net for rewrite_having_pred — flags any ExprVar reference that survived
    // the targeted rewrite (e.g., because the predicate contained a node type the walker
    // doesn't recurse into, like ExprAt or ExprInvoke from a non-peelable lambda).
    vname : string
    hit : bool = false
    def HbScanner(n : string) {
        vname = n
    }
    def override visitExprVar(var expr : ExprVar?) : Expression? {
        if (expr.name == vname) {
            hit = true
        }
        return expr
    }
}

def private expr_uses_var(var e : Expression?; name : string) : bool {
    if (e == null) return false
    var astVisitor = new HbScanner(name)
    make_visitor(*astVisitor) $(astVisitorAdapter) {
        visit_expression(e, astVisitorAdapter)
    }
    let hit = astVisitor.hit
    unsafe {
        delete astVisitor
    }
    return hit
}

// ── group_by core (shared between array and decs sources) ──────────────────────

// Source-loop adapter: dispatches the per-source-shape divergence (initial bind name + element type, per-element source loop emission, final invoke wrap). Used by plan_group_by_core to unify the ~350 LOC shared recognizer/splice machinery between plan_group_by (array source via for(it in src)) and plan_decs_group_by (decs bridge via for_each_archetype + inner multi-iter for + tupBind).
struct private GroupBySourceAdapter {
    // The bind name the upstream segment walk and reducer recognition see as "the element" (srcItName for array, tupName for decs — both alias the post-iterator-bind variable).
    initialBindName : string
    // The element type at that bind point (peel_each-derived from `top._type.firstType` for array, `bridge.elementType` for decs).
    initialElemType : TypeDeclPtr
    // Prefix for all hoisted names ("" for array → tab/dummy/k/uk/…; "decs_" for decs → decs_tab/decs_dummy/…). Lets the same core function generate distinct-but-recognizable names per source.
    namePrefix      : string
    // Discriminator for the two emit functions below.
    isDecs          : bool
    // Array side: the source expression bound by finalize_emission_stmts into srcName.
    arrayTop        : Expression?
    arraySrcName    : string
    // Decs side: bridge metadata used by build_decs_tup_bind / build_decs_inner_for + the outer for_each_archetype call.
    decsBridge      : DecsBridgeShape?
}

[macro_function]
def private adapter_emit_source_loop(adapter : GroupBySourceAdapter; var body : Expression?; at : LineInfo) : Expression? {
    // Per-element source loop. Body has been segment-wrapped (inside-out where_/select) by the caller. For array: simple `for(it in src) { body }`. For decs: tupBind from build_decs_tup_bind + inner multi-iter for via build_decs_inner_for + outer for_each_archetype.
    if (adapter.isDecs) {
        var tupBind = build_decs_tup_bind(adapter.decsBridge, adapter.initialBindName, at)
        var forExprNode = build_decs_inner_for(adapter.decsBridge, tupBind, body, at)
        var reqExpr = clone_expression(adapter.decsBridge.reqHashExpr)
        var erqExpr = clone_expression(adapter.decsBridge.erqExpr)
        let archName = adapter.decsBridge.archName
        return qmacro_expr() {
            for_each_archetype($e(reqExpr), $e(erqExpr), $($i(archName) : Archetype) {
                $e(forExprNode)
            })
        }
    }
    let srcItName = adapter.initialBindName
    let srcName = adapter.arraySrcName
    return qmacro_block() {
        for ($i(srcItName) in $i(srcName)) {
            $e(body)
        }
    }
}

[macro_function]
def private adapter_finalize_emission(adapter : GroupBySourceAdapter; var stmts : array<Expression?>; retType : TypeDeclPtr; at : LineInfo) : Expression? {
    // Wrap accumulated stmts into the emission invoke. Array side: bind source as param (finalize_emission_stmts handles peel + topExpr clone). Decs side: zero-arg invoke with explicit retType (the bridge is already inline via for_each_archetype).
    if (adapter.isDecs) {
        var emission = qmacro(invoke($() : $t(retType) {
            $b(stmts)
        }))
        emission.force_at(at)
        emission.force_generated(true)
        return emission
    }
    var topClone = clone_expression(adapter.arrayTop)
    return finalize_emission_stmts(topClone, adapter.arraySrcName, at, stmts)
}

[macro_function]
def private plan_group_by_core(var calls : array<tuple<ExprCall?; LinqCall?>>;
                               var keyBlock : Expression?;
                               var groupProjCall : ExprCall?;
                               var havingCall : ExprCall?;
                               terminatorName : string;
                               exprIsIterator : bool;
                               at : LineInfo;
                               var adapter : GroupBySourceAdapter) : Expression? {
    // Shared group_by splice machinery. Caller pops terminator/select/having/group_by_lazy + extracts keyBlock + sets up the source adapter; this function does the rest (upstream chain walk, reducer recognition, state-table hoisting, tab?[uk]??dummy splice, output emission, source-loop wrap, terminator emission, final invoke wrap).
    let prefix = adapter.namePrefix
    let tabName    = "`{prefix}tab`{at.line}`{at.column}"
    let kName      = "`{prefix}k`{at.line}`{at.column}"
    let ukName     = "`{prefix}uk`{at.line}`{at.column}"
    let entryName  = "`{prefix}entry`{at.line}`{at.column}"
    let kvName     = "`{prefix}kv`{at.line}`{at.column}"
    let bufName    = "`{prefix}buf`{at.line}`{at.column}"
    let bindName   = "`{prefix}gpb`{at.line}`{at.column}"
    let cntName    = "`{prefix}cnt`{at.line}`{at.column}"
    let dummyName  = "`{prefix}dummy`{at.line}`{at.column}"
    // Walk upstream where_/select* into segments. Each where guards everything AFTER it; a select after a where flushes a new segment so the projection bind lives inside the where's guard.
    var segBinds : array<array<Expression?>>
    var segWheres : array<Expression?>
    var currentBinds : array<Expression?>
    var currentWhere : Expression?
    var projection : Expression?
    var lastBindName = adapter.initialBindName
    var elemType = strip_const_ref(clone_type(adapter.initialElemType))
    var totalBindCount = 0
    for (i in 0 .. (calls |> length)) {
        let opName = calls[i]._1.name
        if (opName == "where_") {
            if (projection != null) {
                let bn = "`{prefix}v`{at.line}`{at.column}`{totalBindCount}"
                totalBindCount ++
                currentBinds |> push <| qmacro_expr() {
                    var $i(bn) := $e(projection)
                }
                lastBindName = bn
                projection = null
            }
            var predicate = peel_lambda_rename_var(calls[i]._0.arguments[1], lastBindName)
            if (predicate == null) return null
            currentWhere = merge_where_cond(currentWhere, predicate)
        } elif (opName == "select") {
            if (currentWhere != null) {
                segBinds |> emplace(currentBinds)
                segWheres |> push(currentWhere)
                currentWhere = null
            }
            if (projection != null) {
                let bn = "`{prefix}v`{at.line}`{at.column}`{totalBindCount}"
                totalBindCount ++
                currentBinds |> push <| qmacro_expr() {
                    var $i(bn) := $e(projection)
                }
                lastBindName = bn
            }
            projection = peel_lambda_rename_var(calls[i]._0.arguments[1], lastBindName)
            if (projection == null) return null
            elemType = strip_const_ref(clone_type(calls[i]._0._type.firstType))
        } else {
            return null
        }
    }
    if (projection != null) {
        let bn = "`{prefix}v`{at.line}`{at.column}`{totalBindCount}"
        totalBindCount ++
        currentBinds |> push <| qmacro_expr() {
            var $i(bn) := $e(projection)
        }
        lastBindName = bn
    }
    segBinds |> emplace(currentBinds)
    segWheres |> push(currentWhere)
    let itName = lastBindName
    // Recognize the group_proj body's reducer shape (uses post-projection itName for inner-select fold).
    var groupProjBody = peel_lambda_rename_var(groupProjCall.arguments[1], bindName)
    if (groupProjBody == null || groupProjBody._type == null) return null
    var (specs, usesNamedTuple) = recognize_reducer_specs(groupProjBody, bindName, itName)
    if (empty(specs)) return null
    let userVisibleSlotCount = specs |> length
    // Rewrite optional having predicate against accumulator slots; PR-E scan adds hidden slots for reducers referenced by having but absent from select.
    var havingPred : Expression?
    if (havingCall != null) {
        let hbName = "`{prefix}hb`{at.line}`{at.column}"
        var rawPred = peel_lambda_rename_var(havingCall.arguments[1], hbName)
        if (rawPred == null || !extend_specs_for_missing_having_reducers(rawPred, hbName, itName, specs, userVisibleSlotCount, elemType, at)) return null
        havingPred = rewrite_having_pred(rawPred, hbName, kvName, specs)
        if (havingPred == null || expr_uses_var(havingPred, hbName)) return null
    }
    let hasHidden = (specs |> length) > userVisibleSlotCount
    // Bare reducer + hidden slot needs typedecl(invoke(...)) growth inside a qmacro — can't grow that dynamically. Cascade.
    if (hasHidden && !usesNamedTuple) return null
    var hasAvg = false
    for (s in specs) {
        if (is_average_spec(s)) {
            hasAvg = true
        }
    }
    // Table + matching dummy. Dummy is bound on miss so first-key-wins is detected via addr-compare (one hash op/elem on hits).
    var keyBlk1 = clone_expression(keyBlock)
    var keyBlk2 = clone_expression(keyBlock)
    var keyBlkD = clone_expression(keyBlock)
    var stmts : array<Expression?>
    if (usesNamedTuple) {
        var tabValueType = strip_const_ref(clone_type(groupProjBody._type))
        if (hasAvg) {
            for (s in specs) {
                if (is_average_spec(s) && s.slot <= userVisibleSlotCount && s.slot < (tabValueType.argTypes |> length)) {
                    tabValueType.argTypes[s.slot] = mk_avg_acc_type(at)
                }
            }
        }
        if (hasHidden) {
            let preserveNames = (tabValueType.argNames |> length) == (tabValueType.argTypes |> length)
            for (s in specs) {
                if (s.slot > userVisibleSlotCount) {
                    tabValueType.argTypes |> push <| clone_type(s.accType)
                    if (preserveNames) {
                        let nameIdx = tabValueType.argNames |> length
                        tabValueType.argNames |> resize(nameIdx + 1)
                        tabValueType.argNames[nameIdx] := "_hidden_{s.slot}"
                    }
                }
            }
        }
        var tabValueType2 = clone_type(tabValueType)
        stmts |> push <| qmacro_expr() {
            var inscope $i(tabName) : table<typedecl(_::unique_key(invoke($e(keyBlk1), default<$t(elemType)>))); $t(tabValueType)>
        }
        stmts |> push <| qmacro_expr() {
            var $i(dummyName) : $t(tabValueType2)
        }
    } else {
        var accType  = clone_type(specs[0].accType)
        var accType2 = clone_type(specs[0].accType)
        stmts |> push <| qmacro_expr() {
            var inscope $i(tabName) : table<typedecl(_::unique_key(invoke($e(keyBlk1), default<$t(elemType)>))); tuple<typedecl(invoke($e(keyBlk2), default<$t(elemType)>)) -const -&; $t(accType)>>
        }
        stmts |> push <| qmacro_expr() {
            var $i(dummyName) : tuple<typedecl(invoke($e(keyBlkD), default<$t(elemType)>)) -const -&; $t(accType2)>
        }
    }
    // `tab?[uk] ?? dummy` + addr-compare: one hash op/elem on hits, two on misses (insert).
    var keyBlk3 = clone_expression(keyBlock)
    var missInitStmts : array<Expression?>
    var hitUpdateStmts : array<Expression?>
    for (s in specs) {
        let (mi, hu) = emit_reducer_branches(s, at, entryName, itName)
        if (mi != null) {
            missInitStmts |> push(clone_expression(mi))
        }
        if (hu != null) {
            hitUpdateStmts |> push(clone_expression(hu))
        }
    }
    if (empty(missInitStmts)) return null
    var missInit = stmts_to_expr(missInitStmts)
    let hasHitUpdate = !empty(hitUpdateStmts)
    var tableUpdate : Expression?
    if (hasHitUpdate) {
        var hitUpdate = stmts_to_expr(hitUpdateStmts)
        tableUpdate = qmacro_expr() {
            unsafe {
                var $i(entryName) & = $i(tabName)?[$i(ukName)] ?? $i(dummyName)
                if (addr($i(entryName)) == addr($i(dummyName))) {
                    $e(missInit)
                    $i(dummyName)._0 = $i(kName)
                    $i(tabName)[$i(ukName)] = $i(dummyName)
                    $i(dummyName) = default<typedecl($i(dummyName))>
                } else {
                    $e(hitUpdate)
                }
            }
        }
    } else {
        tableUpdate = qmacro_expr() {
            unsafe {
                var $i(entryName) & = $i(tabName)?[$i(ukName)] ?? $i(dummyName)
                if (addr($i(entryName)) == addr($i(dummyName))) {
                    $e(missInit)
                    $i(dummyName)._0 = $i(kName)
                    $i(tabName)[$i(ukName)] = $i(dummyName)
                    $i(dummyName) = default<typedecl($i(dummyName))>
                }
            }
        }
    }
    // Per-element core: key derive + unique_key derive + table-update splice.
    var coreBody = qmacro_block() {
        let $i(kName) = invoke($e(keyBlk3), $i(itName))
        let $i(ukName) = _::unique_key($i(kName))
        $e(tableUpdate)
    }
    // Walk segments in reverse, wrapping body inside-out: each where guards everything inside it.
    var body : Expression? = coreBody
    var idx = (segBinds |> length) - 1
    while (idx >= 0) {
        if (segWheres[idx] != null) {
            body = wrap_with_condition(body, clone_expression(segWheres[idx]))
        }
        if (!empty(segBinds[idx])) {
            var thisSeg <- segBinds[idx]
            thisSeg |> push(body)
            body = stmts_to_expr(thisSeg)
        }
        idx --
    }
    // Adapter-specific source loop emission (for(it in src) for array, for_each_archetype + inner for for decs).
    stmts |> push(adapter_emit_source_loop(adapter, body, at))
    // Terminator emission + retType derivation.
    var retType : TypeDeclPtr
    if (terminatorName == "count") {
        retType = new TypeDecl(baseType = Type.tInt)
        if (havingPred != null) {
            stmts |> push <| qmacro_block() {
                var $i(cntName) = 0
                for ($i(kvName) in values($i(tabName))) {
                    if ($e(havingPred)) {
                        $i(cntName) ++
                    }
                }
                return $i(cntName)
            }
        } else {
            stmts |> push <| qmacro_expr() {
                return length($i(tabName))
            }
        }
    } else {
        // to_array lane (implicit when no count terminator): build result buffer by walking the table.
        var outputExpr : Expression?
        if (!usesNamedTuple) {
            if (hasAvg) {
                outputExpr = mk_avg_divide_expr(at, kvName, 1)
            } else {
                outputExpr = qmacro_expr() {
                    $i(kvName)._1
                }
            }
        } elif (hasAvg || hasHidden) {
            // Re-synthesize the user's tuple slot-by-slot: avg slots divide, hidden slots are omitted by iterating only user-visible argTypes; recordNames only when argNames are populated.
            var mt = new ExprMakeTuple(at = at)
            let slotCount = (groupProjBody._type.argTypes |> length)
            let hasNames = (groupProjBody._type.argNames |> length) == slotCount
            if (hasNames) {
                mt.recordNames |> resize(slotCount)
            }
            mt.values |> push(mk_slot_ref(at, kvName, 0))
            if (hasNames) {
                mt.recordNames[0] := string(groupProjBody._type.argNames[0])
            }
            for (i in 1 .. slotCount) {
                var v : Expression?
                for (sp in specs) {
                    if (sp.slot == i) {
                        v = mk_slot_output_expr(at, kvName, sp)
                    }
                }
                mt.values |> push(v)
                if (hasNames) {
                    mt.recordNames[i] := string(groupProjBody._type.argNames[i])
                }
            }
            outputExpr = mt
        } else {
            outputExpr = qmacro_expr() {
                $i(kvName)
            }
        }
        var bufElemType = clone_type(groupProjBody._type)
        if (bufElemType != null) {
            bufElemType.flags.constant = false
            bufElemType.flags.ref = false
        }
        // retType matches exprIsIterator: iterator<bufElemType> via to_sequence_move(buf) tail, or array<bufElemType> via raw buf return. Both paths use buffer_return(bufName, exprIsIterator) for the final stmt.
        if (exprIsIterator) {
            retType = new TypeDecl(baseType = Type.tIterator)
            retType.firstType = clone_type(bufElemType)
        } else {
            retType = new TypeDecl(baseType = Type.tArray)
            retType.firstType = clone_type(bufElemType)
        }
        stmts |> push <| qmacro_expr() {
            var $i(bufName) : array<$t(bufElemType)>
        }
        stmts |> push <| qmacro_expr() {
            $i(bufName) |> reserve(length($i(tabName)))
        }
        if (havingPred != null) {
            stmts |> push <| qmacro_expr() {
                for ($i(kvName) in values($i(tabName))) {
                    if ($e(havingPred)) {
                        $i(bufName) |> push_clone($e(outputExpr))
                    }
                }
            }
        } else {
            stmts |> push <| qmacro_expr() {
                for ($i(kvName) in values($i(tabName))) {
                    $i(bufName) |> push_clone($e(outputExpr))
                }
            }
        }
        stmts |> push(buffer_return(bufName, exprIsIterator))
    }
    return adapter_finalize_emission(adapter, stmts, retType, at)
}

[macro_function]
def private plan_group_by(var expr : Expression?) : Expression? {
    // Array-source entry to plan_group_by_core. Recognizer + adapter setup; all splice machinery lives in the shared core.
    var (top, calls) = flatten_linq(expr)
    if (empty(calls)) return null
    top = peel_each(top)
    if (expr._type == null) return null
    // Pop count terminator from chain tail; bare select-after-group_by → ARRAY lane. Only the 1-arg `count(src)` form — `count(src, predicate)` (a valid linq.das overload) must cascade so the predicate isn't silently dropped.
    var terminatorName : string  = ""
    {
        let lastName = calls.back()._1.name
        if (lastName == "count" && length(calls.back()._0.arguments) == 1) {
            terminatorName = lastName
            calls |> pop
        }
    }
    // Required tail: select(group_proj) — without it the chain yields raw buckets (no fusion).
    if (empty(calls) || calls.back()._1.name != "select") return null
    var groupProjCall = calls.back()._0
    calls |> pop
    // Optional: having_ between select and group_by_lazy (SQL HAVING shape) — rewritten in plan_group_by_core against accumulator slots.
    var havingCall : ExprCall?
    if (!empty(calls) && calls.back()._1.name == "having_") {
        havingCall = calls.back()._0
        calls |> pop
    }
    // The call immediately before must be group_by_lazy with a key arg.
    if (empty(calls) || calls.back()._1.name != "group_by_lazy") return null
    var groupByCall = calls.back()._0
    calls |> pop
    if ((groupByCall.arguments |> length) < 2) return null
    var keyBlock = clone_expression(groupByCall.arguments[1])
    let at = groupByCall.at
    var adapter = GroupBySourceAdapter(
        initialBindName := "`it`{at.line}`{at.column}",
        initialElemType = strip_const_ref(clone_type(top._type.firstType)),
        namePrefix := "",
        isDecs = false,
        arrayTop = top,
        arraySrcName := "`source`{at.line}`{at.column}",
        decsBridge = null
    )
    return plan_group_by_core(calls, keyBlock, groupProjCall, havingCall, terminatorName, expr._type.isIterator, at, adapter)
}

// ── decs eager-bridge unroll (Approach Z — for_each_archetype + nested _fold) ───────

struct private DecsBridgeShape {
    reqHashExpr : ExpressionPtr     // ExprConstUInt64 — cloned, ready to splice
    erqExpr     : ExpressionPtr     // ExprAddr — cloned, ready to splice
    archName    : string            // bridge's arch param name (reused as-is so cloned sources stay valid)
    forExpr     : ExpressionPtr     // cloned ExprFor — inner multi-iter for-loop; body replaced when splicing
    iterNames   : array<string>     // bridge's iter var names (prefixed component names)
    userNames   : array<string>     // user-facing field names from the push tuple — feed the named-tuple bind that lets the user's `_.userName` chain access work
    elementType : TypeDeclPtr       // named-tuple type (from resVar._type.firstType) — used by to_array/first when no projection
}

[macro_function]
def private get_call_short_name(c : ExprCall?) : string {
    // Strip module prefix + mangler hash. "__::linq`to_sequence`hash" → "to_sequence".
    let s = string(c.name)
    let bt = s |> rfind("`")
    if (bt >= 0) {
        let prev = s |> slice(0, bt) |> rfind("`")
        if (prev >= 0) return s |> slice(prev + 1, bt)
    }
    let cc = s |> rfind("::")
    if (cc >= 0) return s |> slice(cc + 2)
    return s
}

[macro_function]
def private extract_decs_bridge(var top : Expression?) : DecsBridgeShape? {
    // Pattern-match the post-expansion from_decs*-eager-bridge shape. Mismatch → null cascades to tier-2.
    if (!(top is ExprInvoke)) return null
    var topInv = top as ExprInvoke
    if (topInv.arguments |> length != 1
            || !(topInv.arguments[0] is ExprMakeBlock)) return null
    var mblk = topInv.arguments[0] as ExprMakeBlock
    if (!(mblk._block is ExprBlock)) return null
    var blk = mblk._block as ExprBlock
    if (blk.list |> length != 3
            || !empty(blk.arguments)
            || !(blk.list[0] is ExprLet)
            || !(blk.list[1] is ExprCall)
            || !(blk.list[2] is ExprReturn)) return null
    // Statement 0: var res : array<tuple<...>>
    var letStmt = blk.list[0] as ExprLet
    if (letStmt.variables |> length != 1) return null
    var resVar = letStmt.variables[0]
    if (!resVar._type.isGoodArrayType
            || resVar._type.firstType.baseType != Type.tTuple) return null
    let resVarName = string(resVar.name)
    // Statement 1: for_each_archetype(req_hash, erq, $(arch) { for(iter_vars in get_ro_sources){push((userNames=iter_vars))} })
    var feaCall = blk.list[1] as ExprCall
    if (feaCall.name != "for_each_archetype"
            || feaCall.arguments |> length != 3
            || !(feaCall.arguments[0] is ExprConstUInt64)
            || !(feaCall.arguments[1] is ExprAddr)
            || !(feaCall.arguments[2] is ExprMakeBlock)) return null
    var feaBlkExpr = feaCall.arguments[2] as ExprMakeBlock
    if (!(feaBlkExpr._block is ExprBlock)) return null
    var feaBlk = feaBlkExpr._block as ExprBlock
    if (feaBlk.arguments |> length != 1
            || feaBlk.list |> length != 1
            || !(feaBlk.list[0] is ExprFor)) return null
    let archName = string(feaBlk.arguments[0].name)
    var forExpr = feaBlk.list[0] as ExprFor
    // Body must be ExprBlock with single push call whose tuple recordNames are the user-facing field names.
    if (empty(forExpr.sources)
            || !(forExpr.body is ExprBlock)) return null
    var forBody = forExpr.body as ExprBlock
    if (forBody.list |> length != 1
            || !(forBody.list[0] is ExprCall)) return null
    var pushCall = forBody.list[0] as ExprCall
    // Push target must be the SAME `res` from stmt 0 — guards a user-written invoke that returns res.to_sequence() but pushes elsewhere.
    if (get_call_short_name(pushCall) != "push"
            || pushCall.arguments |> length != 2
            || !(pushCall.arguments[0] is ExprVar)
            || (pushCall.arguments[0] as ExprVar).name != resVarName
            || !(pushCall.arguments[1] is ExprMakeTuple)) return null
    var mkTup = pushCall.arguments[1] as ExprMakeTuple
    if (mkTup.recordNames |> length != forExpr.sources |> length
            || forExpr.iteratorVariables |> length != forExpr.sources |> length) return null
    var userNames : array<string>
    for (n in mkTup.recordNames) {
        userNames |> push(string(n))
    }
    var iterNames : array<string>
    for (v in forExpr.iteratorVariables) {
        iterNames |> push(string(v.name))
    }
    // Statement 2: return res.to_sequence() — must reference the same `res`.
    var retStmt = blk.list[2] as ExprReturn
    if (retStmt.subexpr == null || !(retStmt.subexpr is ExprCall)) return null
    let retCall = retStmt.subexpr as ExprCall
    if (get_call_short_name(retCall) != "to_sequence"
            || empty(retCall.arguments)
            || !(retCall.arguments[0] is ExprVar)
            || (retCall.arguments[0] as ExprVar).name != resVarName) return null
    var info = new DecsBridgeShape(
        reqHashExpr = clone_expression(feaCall.arguments[0]),
        erqExpr = clone_expression(feaCall.arguments[1]),
        archName := archName,
        forExpr = clone_expression(forExpr),
        iterNames <- iterNames,
        userNames <- userNames,
        elementType = clone_type(resVar._type.firstType)
    )
    return info
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

struct private DecsChainInfo {
    bindAt      : array<string>    // bind name visible at each chain position
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
    var curBind = tupName
    var curType : TypeDeclPtr = clone_type(bridge.elementType)
    for (i in 0 .. intermediateEnd) {
        info.bindAt |> push(curBind)
        var cll & = unsafe(calls[i])
        let opName = cll._1.name
        if (opName == "select") {
            info.selectCount ++
            curBind = "`decs_sel`{at.line}`{at.column}`{info.selectCount}"
            var peeled = peel_lambda_rename_var(cll._0.arguments[1], info.bindAt[i])
            if (peeled == null || peeled._type == null) return null
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
    skipExpr      : Expression?
    takeExpr      : Expression?
    skipWhileCond : Expression?    // Slice 5b: predicate peeled against tupName (source tuple)
    takeWhileCond : Expression?    // Slice 5b: predicate peeled against tupName (source tuple)
    chainEnd      : int
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
                                            skipName, takeCountName, takeLimitName, skippingName : string) : Expression? {
    // Slice 5a/5b: prefix per-element body with range guards in canonical order: take-cap → skip-counter → skip_while flag → take_while break → takeC++. Mirrors array-side wrap_with_ranges (linq_fold.das:556). All early exits (`return true`) work both for plain inner-for break AND for the bool-lambda archetype-find — caller switches outer iterator to for_each_archetype_find when takeExpr != null OR takeWhileCond != null. Counters / flags live at invoke scope (decs_range_prelude). Take limit is read from a hoisted local (takeLimitName) so the user's take(N) expression evaluates exactly once at invoke entry.
    if (rangeInfo.skipExpr == null && rangeInfo.takeExpr == null && rangeInfo.skipWhileCond == null && rangeInfo.takeWhileCond == null) return body
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
        var swPred = clone_expression(rangeInfo.skipWhileCond)
        stmts |> push <| qmacro_expr() {
            if ($i(skippingName)) {
                if ($e(swPred)) {
                    continue
                }
                $i(skippingName) = false
            }
        }
    }
    if (rangeInfo.takeWhileCond != null) {
        var twPred = clone_expression(rangeInfo.takeWhileCond)
        // First false ends iteration globally (across all archetypes). `return true` exits inner-for AND archetype-find lambda.
        stmts |> push <| qmacro_expr() {
            if (!($e(twPred))) {
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
def private decs_range_prelude(rangeInfo : DecsRangeInfo?; skipName, takeCountName, takeLimitName, skippingName : string) : array<Expression?> {
    // Counter / flag init statements hoisted ABOVE the for_each_archetype call so they survive across archetypes (take cap, skip count, skip_while flag are all globally-stateful). takeLimit hoisted to a local so the user's take(N) expression evaluates exactly once (matches take(src, total:int) semantics).
    var stmts : array<Expression?>
    if (rangeInfo.skipExpr != null) {
        var skipInit = clone_expression(rangeInfo.skipExpr)
        stmts |> push <| qmacro_expr() {
            var $i(skipName) = $e(skipInit)
        }
    }
    if (rangeInfo.takeExpr != null) {
        var takeInit = clone_expression(rangeInfo.takeExpr)
        stmts |> push <| qmacro_expr() {
            let $i(takeLimitName) = $e(takeInit)
        }
        stmts |> push <| qmacro_expr() {
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

[macro_function]
def private emit_decs_count_archsize(bridge : DecsBridgeShape?; at : LineInfo) : Expression? {
    // Bare count(): no chain ops, sum arch.size per archetype — skips the per-entity walk entirely.
    let accName = "`decs_acc`{at.line}`{at.column}"
    let archName = "`decs_arch`{at.line}`{at.column}"
    var reqExpr = clone_expression(bridge.reqHashExpr)
    var erqExpr = clone_expression(bridge.erqExpr)
    // arch.size is int64; count() returns int and the += site truncates past INT_MAX — chain `long_count()` when an int64-safe total is required.
    var emission : Expression? = qmacro(invoke($() : int {
        var $i(accName) = 0
        for_each_archetype($e(reqExpr), $e(erqExpr), $($i(archName) : Archetype) {
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
    let accName = "`decs_acc`{at.line}`{at.column}"
    let tupName = "`decs_tup`{at.line}`{at.column}"
    let cntName = "`decs_cnt`{at.line}`{at.column}"
    let firstName = "`decs_first`{at.line}`{at.column}"
    let valBindName = "`decs_val`{at.line}`{at.column}"
    let skipName = "`decs_skip`{at.line}`{at.column}"
    let takeCountName = "`decs_takec`{at.line}`{at.column}"
    let takeLimitName = "`decs_takel`{at.line}`{at.column}"
    let skippingName = "`decs_skipping`{at.line}`{at.column}"
    let finalBind = chainInfo.finalBind
    var perElement : Expression?
    if (opName == "sum") {
        perElement = qmacro_expr() {
            $i(accName) += $i(finalBind)
        }
    } elif (opName == "average") {
        // Matches linq.das:1545 average() — always-double accumulator, uint64 counter, empty source returns 0.0lf. Cast lifted out at macro time so already-double chains don't trip PERF020.
        if (chainInfo.finalType != null && chainInfo.finalType.baseType == Type.tDouble) {
            perElement = qmacro_block() {
                $i(accName) += $i(finalBind)
                $i(cntName) ++
            }
        } else {
            perElement = qmacro_block() {
                $i(accName) += double($i(finalBind))
                $i(cntName) ++
            }
        }
    } elif (opName == "min" || opName == "max") {
        let workhorse = (chainInfo.finalType != null && chainInfo.finalType.isWorkhorseType)
        var compareExpr = min_max_compare(workhorse, opName, valBindName, accName)
        perElement = qmacro_block() {
            let $i(valBindName) = $i(finalBind)
            if ($i(firstName)) {
                $i(accName) := $i(valBindName)
                $i(firstName) = false
            } elif ($e(compareExpr)) {
                $i(accName) := $i(valBindName)
            }
        }
    } else {
        // count, long_count
        perElement = qmacro_expr() {
            $i(accName) ++
        }
    }
    // _count(pred): count/long_count with extra predicate — wrap perElement with `if (pred) {...}`. Predicate peels against finalBind so it sees the post-chain element.
    if ((opName == "count" || opName == "long_count") && length(terminatorCall.arguments) > 1) {
        var predExpr = peel_lambda_rename_var(clone_expression(terminatorCall.arguments[1]), finalBind)
        if (predExpr == null) return null
        perElement = qmacro_expr() {
            if ($e(predExpr)) {
                $e(perElement)
            }
        }
    }
    // Range guards (take-cap + skip + takeC++) wrap perElement BEFORE chain so where_ predicate fires first — ranges apply to the POST-filter stream (mirrors array-side wrap_with_condition(wrap_with_ranges(...))).
    perElement = wrap_inner_for_with_decs_ranges(perElement, rangeInfo, skipName, takeCountName, takeLimitName, skippingName)
    var body = wrap_decs_chain(perElement, chainInfo, calls, intermediateEnd, at)
    if (body == null) return null
    var tupBind = build_decs_tup_bind(bridge, tupName, at)
    var forExprNode = build_decs_inner_for(bridge, tupBind, body, at)
    let archName = bridge.archName
    var reqExpr = clone_expression(bridge.reqHashExpr)
    var erqExpr = clone_expression(bridge.erqExpr)
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
        preludeStmts |> push <| qmacro_expr() {
            var $i(accName) : double = 0lf
        }
        preludeStmts |> push <| qmacro_expr() {
            var $i(cntName) : uint64 = 0ul
        }
        tailStmts |> push <| qmacro_expr() {
            return $i(cntName) != 0ul ? $i(accName) / double($i(cntName)) : 0lf
        }
    } elif (opName == "min" || opName == "max") {
        // No empty-panic — matches non-decs emit_accumulator_lane (returns default-initialized acc).
        preludeStmts |> push <| qmacro_expr() {
            var $i(firstName) = true
        }
        preludeStmts |> push <| qmacro_expr() {
            var $i(accName) : $t(accType)
        }
        tailStmts |> push <| qmacro_expr() {
            return $i(accName)
        }
    } else {
        return null
    }
    var rangeStmts <- decs_range_prelude(rangeInfo, skipName, takeCountName, takeLimitName, skippingName)
    var bodyStmts : array<Expression?>
    bodyStmts |> reserve(length(preludeStmts) + length(rangeStmts) + length(tailStmts) + 1)
    bodyStmts |> push_from(preludeStmts)
    bodyStmts |> push_from(rangeStmts)
    if (rangeInfo.takeExpr != null || rangeInfo.takeWhileCond != null) {
        // Slice 5b: take_while also produces inner `return true` on its first-false (terminates iteration globally), so it needs the bool-lambda archetype-find dispatch too.
        bodyStmts |> push <| qmacro_expr() {
            for_each_archetype_find($e(reqExpr), $e(erqExpr), $($i(archName) : Archetype) : bool {
                $e(forExprNode)
                return false
            })
        }
    } else {
        bodyStmts |> push <| qmacro_expr() {
            for_each_archetype($e(reqExpr), $e(erqExpr), $($i(archName) : Archetype) {
                $e(forExprNode)
            })
        }
    }
    bodyStmts |> push_from(tailStmts)
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
    var emission = qmacro(invoke($() : $t(retType) {
        $b(bodyStmts)
    }))
    emission.force_at(at)
    emission.force_generated(true)
    return emission
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
    let tupName = "`decs_tup`{at.line}`{at.column}"
    let foundName = "`decs_found`{at.line}`{at.column}"
    let resultName = "`decs_result`{at.line}`{at.column}"
    let containsValName = "`decs_cv`{at.line}`{at.column}"
    let defaultName = "`decs_dv`{at.line}`{at.column}"
    let skipName = "`decs_skip`{at.line}`{at.column}"
    let takeCountName = "`decs_takec`{at.line}`{at.column}"
    let takeLimitName = "`decs_takel`{at.line}`{at.column}"
    let skippingName = "`decs_skipping`{at.line}`{at.column}"
    let finalBind = chainInfo.finalBind
    var elemType = clone_type(chainInfo.finalType)
    var perElement : Expression?
    var preludeStmts : array<Expression?>
    var tailStmts : array<Expression?>
    if (opName == "first" || opName == "first_or_default") {
        preludeStmts |> push <| qmacro_expr() {
            var $i(foundName) = false
        }
        preludeStmts |> push <| qmacro_expr() {
            var $i(resultName) : $t(elemType)
        }
        if (opName == "first_or_default") {
            var defaultExpr = clone_expression(terminatorCall.arguments[1])
            preludeStmts |> push <| qmacro_expr() {
                let $i(defaultName) = $e(defaultExpr)
            }
        }
        perElement = qmacro_block() {
            $i(resultName) := $i(finalBind)
            $i(foundName) = true
            return true
        }
        if (opName == "first") {
            tailStmts |> push <| qmacro_expr() {
                if (!$i(foundName)) panic("sequence contains no elements")
            }
            tailStmts |> push <| qmacro_expr() {
                return $i(resultName)
            }
        } else {
            tailStmts |> push <| qmacro_expr() {
                if (!$i(foundName)) return $i(defaultName)
            }
            tailStmts |> push <| qmacro_expr() {
                return $i(resultName)
            }
        }
    } elif (opName == "any") {
        let argCount = length(terminatorCall.arguments)
        // When take OR take_while is present, distinguish "match found" from "iteration-stop signal" — both produce `return true` from the inner block, so route through an explicit foundName state and use the find result only as iteration control (tail returns foundName).
        if (rangeInfo.takeExpr != null || rangeInfo.takeWhileCond != null) {
            preludeStmts |> push <| qmacro_expr() {
                var $i(foundName) = false
            }
            if (argCount > 1) {
                var predExpr = peel_lambda_rename_var(clone_expression(terminatorCall.arguments[1]), finalBind)
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
                var predExpr = peel_lambda_rename_var(clone_expression(terminatorCall.arguments[1]), finalBind)
                if (predExpr == null) return null
                perElement = qmacro_expr() {
                    if ($e(predExpr)) return true
                }
            } else {
                perElement = qmacro_expr() {
                    return true
                }
            }
        }
    } elif (opName == "all") {
        var predExpr = peel_lambda_rename_var(clone_expression(terminatorCall.arguments[1]), finalBind)
        if (predExpr == null) return null
        if (rangeInfo.takeExpr != null || rangeInfo.takeWhileCond != null) {
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
        }
    } elif (opName == "contains") {
        var valExpr = clone_expression(terminatorCall.arguments[1])
        preludeStmts |> push <| qmacro_expr() {
            let $i(containsValName) = $e(valExpr)
        }
        if (rangeInfo.takeExpr != null || rangeInfo.takeWhileCond != null) {
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
        }
    } else {
        return null
    }
    // Range guards (take-cap + skip + takeC++) wrap perElement BEFORE chain so where_ predicate fires first — ranges apply to the POST-filter stream (mirrors array-side wrap_with_condition(wrap_with_ranges(...))).
    perElement = wrap_inner_for_with_decs_ranges(perElement, rangeInfo, skipName, takeCountName, takeLimitName, skippingName)
    var body = wrap_decs_chain(perElement, chainInfo, calls, intermediateEnd, at)
    if (body == null) return null
    var tupBind = build_decs_tup_bind(bridge, tupName, at)
    var forExprNode = build_decs_inner_for(bridge, tupBind, body, at)
    let archName = bridge.archName
    var reqExpr = clone_expression(bridge.reqHashExpr)
    var erqExpr = clone_expression(bridge.erqExpr)
    var rangeStmts <- decs_range_prelude(rangeInfo, skipName, takeCountName, takeLimitName, skippingName)
    // Combine prelude + invocation + tail into ONE bodyStmts list — multiple $b splices in the same qmacro fragment isolate variable scope.
    var bodyStmts : array<Expression?>
    bodyStmts |> reserve(length(preludeStmts) + length(rangeStmts) + length(tailStmts) + 1)
    bodyStmts |> push_from(preludeStmts)
    bodyStmts |> push_from(rangeStmts)
    // When takeExpr OR takeWhileCond is present, any/all/contains route their answer through explicit foundName state (set above) — the find result is unreliable because the iteration-stop signal and "real match" both produce inner `return true`. In that case, treat the find call as iteration control only and emit the return via tailStmts (which already contains `return foundName` / `return !foundName`).
    let useExplicitState = ((rangeInfo.takeExpr != null || rangeInfo.takeWhileCond != null) && (opName == "any" || opName == "all" || opName == "contains"))
    if (useExplicitState) {
        bodyStmts |> push <| qmacro_expr() {
            for_each_archetype_find($e(reqExpr), $e(erqExpr), $($i(archName) : Archetype) : bool {
                $e(forExprNode)
                return false
            })
        }
        bodyStmts |> push_from(tailStmts)
    } elif (opName == "any" || opName == "contains") {
        bodyStmts |> push <| qmacro_expr() {
            return for_each_archetype_find($e(reqExpr), $e(erqExpr), $($i(archName) : Archetype) : bool {
                $e(forExprNode)
                return false
            })
        }
    } elif (opName == "all") {
        // `all` is "no counterexample found" — inner returns true on FAIL, so negate.
        bodyStmts |> push <| qmacro_expr() {
            return !for_each_archetype_find($e(reqExpr), $e(erqExpr), $($i(archName) : Archetype) : bool {
                $e(forExprNode)
                return false
            })
        }
    } else {
        bodyStmts |> push <| qmacro_expr() {
            for_each_archetype_find($e(reqExpr), $e(erqExpr), $($i(archName) : Archetype) : bool {
                $e(forExprNode)
                return false
            })
        }
        bodyStmts |> push_from(tailStmts)
    }
    var emission : Expression?
    if (opName == "first" || opName == "first_or_default") {
        emission = qmacro(invoke($() : $t(elemType) {
            $b(bodyStmts)
        }))
    } else {
        emission = qmacro(invoke($() : bool {
            $b(bodyStmts)
        }))
    }
    emission.force_at(at)
    emission.force_generated(true)
    return emission
}

[macro_function]
def private emit_decs_to_array(bridge : DecsBridgeShape?;
                               chainInfo : DecsChainInfo?;
                               rangeInfo : DecsRangeInfo?;
                               var calls : array<tuple<ExprCall?; LinqCall?>>;
                               intermediateEnd : int;
                               at : LineInfo) : Expression? {
    // Slice 3c/4 to_array: hoist `var buf` above outer for_each_archetype; per-element push_clone of post-chain value at chainInfo.finalBind. Slice 5a adds take/skip ranges via rangeInfo.
    let tupName = "`decs_tup`{at.line}`{at.column}"
    let bufName = "`decs_buf`{at.line}`{at.column}"
    let skipName = "`decs_skip`{at.line}`{at.column}"
    let takeCountName = "`decs_takec`{at.line}`{at.column}"
    let takeLimitName = "`decs_takel`{at.line}`{at.column}"
    let skippingName = "`decs_skipping`{at.line}`{at.column}"
    let finalBind = chainInfo.finalBind
    var elemType = clone_type(chainInfo.finalType)
    var perElement : Expression? = qmacro_expr() {
        $i(bufName) |> push_clone($i(finalBind))
    }
    // Range guards (take-cap + skip + takeC++) wrap perElement BEFORE chain so where_ predicate fires first — ranges apply to the POST-filter stream (mirrors array-side wrap_with_condition(wrap_with_ranges(...))).
    perElement = wrap_inner_for_with_decs_ranges(perElement, rangeInfo, skipName, takeCountName, takeLimitName, skippingName)
    var body = wrap_decs_chain(perElement, chainInfo, calls, intermediateEnd, at)
    if (body == null) return null
    var tupBind = build_decs_tup_bind(bridge, tupName, at)
    var forExprNode = build_decs_inner_for(bridge, tupBind, body, at)
    let archName = bridge.archName
    var reqExpr = clone_expression(bridge.reqHashExpr)
    var erqExpr = clone_expression(bridge.erqExpr)
    var rangeStmts <- decs_range_prelude(rangeInfo, skipName, takeCountName, takeLimitName, skippingName)
    var bodyStmts : array<Expression?>
    bodyStmts |> reserve(length(rangeStmts) + 3)
    bodyStmts |> push <| qmacro_expr() {
        var $i(bufName) : array<$t(elemType)>
    }
    bodyStmts |> push_from(rangeStmts)
    if (rangeInfo.takeExpr != null || rangeInfo.takeWhileCond != null) {
        // Slice 5b: take_while also produces inner `return true` on its first-false (terminates iteration globally), so it needs the bool-lambda archetype-find dispatch too.
        bodyStmts |> push <| qmacro_expr() {
            for_each_archetype_find($e(reqExpr), $e(erqExpr), $($i(archName) : Archetype) : bool {
                $e(forExprNode)
                return false
            })
        }
    } else {
        bodyStmts |> push <| qmacro_expr() {
            for_each_archetype($e(reqExpr), $e(erqExpr), $($i(archName) : Archetype) {
                $e(forExprNode)
            })
        }
    }
    bodyStmts |> push <| qmacro_expr() {
        return <- $i(bufName)
    }
    var emission = qmacro(invoke($() : array<$t(elemType)> {
        $b(bodyStmts)
    }))
    emission.force_at(at)
    emission.force_generated(true)
    return emission
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
    let tupName = "`decs_tup`{at.line}`{at.column}"
    let firstName = "`decs_first`{at.line}`{at.column}"
    let bestKeyName = "`decs_bkey`{at.line}`{at.column}"
    let bestElemName = "`decs_belem`{at.line}`{at.column}"
    let keyBindName = "`decs_key`{at.line}`{at.column}"
    let skipName = "`decs_skip`{at.line}`{at.column}"
    let takeCountName = "`decs_takec`{at.line}`{at.column}"
    let takeLimitName = "`decs_takel`{at.line}`{at.column}"
    let skippingName = "`decs_skipping`{at.line}`{at.column}"
    let finalBind = chainInfo.finalBind
    var elemType = clone_type(chainInfo.finalType)
    var keyExpr = peel_lambda_rename_var(clone_expression(terminatorCall.arguments[1]), finalBind)
    if (keyExpr == null || keyExpr._type == null) return null
    var keyType = clone_type(keyExpr._type)
    keyType.flags.constant = false
    keyType.flags.ref = false
    let workhorse = keyType.isWorkhorseType
    let opCmp = (opName == "min_by") ? "min" : "max"
    var compareExpr = min_max_compare(workhorse, opCmp, keyBindName, bestKeyName)
    var perElement = qmacro_block() {
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
    // Range guards (take-cap + skip + takeC++) wrap perElement BEFORE chain so where_ predicate fires first — ranges apply to the POST-filter stream (mirrors array-side wrap_with_condition(wrap_with_ranges(...))).
    perElement = wrap_inner_for_with_decs_ranges(perElement, rangeInfo, skipName, takeCountName, takeLimitName, skippingName)
    var body = wrap_decs_chain(perElement, chainInfo, calls, intermediateEnd, at)
    if (body == null) return null
    var tupBind = build_decs_tup_bind(bridge, tupName, at)
    var forExprNode = build_decs_inner_for(bridge, tupBind, body, at)
    let archName = bridge.archName
    var reqExpr = clone_expression(bridge.reqHashExpr)
    var erqExpr = clone_expression(bridge.erqExpr)
    var rangeStmts <- decs_range_prelude(rangeInfo, skipName, takeCountName, takeLimitName, skippingName)
    var bodyStmts : array<Expression?>
    bodyStmts |> reserve(length(rangeStmts) + 5)
    bodyStmts |> push <| qmacro_expr() {
        var $i(firstName) = true
    }
    bodyStmts |> push <| qmacro_expr() {
        var $i(bestKeyName) : $t(keyType)
    }
    bodyStmts |> push <| qmacro_expr() {
        var $i(bestElemName) : $t(elemType)
    }
    bodyStmts |> push_from(rangeStmts)
    if (rangeInfo.takeExpr != null || rangeInfo.takeWhileCond != null) {
        // Slice 5b: take_while also produces inner `return true` on its first-false (terminates iteration globally), so it needs the bool-lambda archetype-find dispatch too.
        bodyStmts |> push <| qmacro_expr() {
            for_each_archetype_find($e(reqExpr), $e(erqExpr), $($i(archName) : Archetype) : bool {
                $e(forExprNode)
                return false
            })
        }
    } else {
        bodyStmts |> push <| qmacro_expr() {
            for_each_archetype($e(reqExpr), $e(erqExpr), $($i(archName) : Archetype) {
                $e(forExprNode)
            })
        }
    }
    bodyStmts |> push <| qmacro_expr() {
        return $i(bestElemName)
    }
    var emission = qmacro(invoke($() : $t(elemType) {
        $b(bodyStmts)
    }))
    emission.force_at(at)
    emission.force_generated(true)
    return emission
}

[macro_function]
def private plan_decs_unroll(var expr : Expression?) : Expression? {
    var (top, calls) = flatten_linq(expr)
    let bridge = extract_decs_bridge(top)
    if (bridge == null) return null
    let at = expr.at
    // Slice 1: bare count → arch.size shortcut (no chain ops, no ranges).
    if (length(calls) == 1 && calls.back()._1.name == "count" && length(calls.back()._0.arguments) == 1) return emit_decs_count_archsize(bridge, at)
    // to_array is `skip = true` in linqCalls so it's already peeled — "no recognized terminator" means implicit to_array.
    let lastName = empty(calls) ? "" : calls.back()._1.name
    let isAccum = (lastName == "count" || lastName == "long_count" || lastName == "sum"
        || lastName == "min" || lastName == "max" || lastName == "average")
    let isEarlyExit = (lastName == "first" || lastName == "first_or_default"
        || lastName == "any" || lastName == "all" || lastName == "contains")
    let isMinMaxBy = (lastName == "min_by" || lastName == "max_by")
    let isTerminator = isAccum || isEarlyExit || isMinMaxBy
    let tupName = "`decs_tup`{at.line}`{at.column}"
    let intermediateEnd = isTerminator ? length(calls) - 1 : length(calls)
    // Slice 5a/5b: peel trailing take/skip/take_while/skip_while into rangeInfo. chainEnd is the index past which only range ops live (or == intermediateEnd if none). tupName passed so predicate-driven ranges peel against the source tuple.
    let rangeInfo = extract_decs_ranges(calls, intermediateEnd, tupName, at)
    if (rangeInfo == null) return null
    let chainInfo = compute_decs_chain_info(calls, rangeInfo.chainEnd, tupName, bridge, at)
    if (chainInfo == null) return null
    if (isAccum) {
        // sum/min/max/average need a scalar element — selectCount==0 keeps finalType = bridge.elementType (a tuple) which can't be summed/compared.
        var accType : TypeDeclPtr
        if (lastName == "sum" || lastName == "min" || lastName == "max" || lastName == "average") {
            if (chainInfo.selectCount == 0 || chainInfo.finalType == null) return null
            accType = clone_type(chainInfo.finalType)
        }
        return emit_decs_accumulator(bridge, lastName, chainInfo, rangeInfo, calls, rangeInfo.chainEnd, calls.back()._0, accType, at)
    }
    if (isEarlyExit) return emit_decs_early_exit(bridge, lastName, chainInfo, rangeInfo, calls, rangeInfo.chainEnd, calls.back()._0, at)
    if (isMinMaxBy) return emit_decs_min_max_by(bridge, lastName, chainInfo, rangeInfo, calls, rangeInfo.chainEnd, calls.back()._0, at)
    // Iterator-typed chains cascade to tier-2; only fire to_array when expr is array-typed (user wrote `.to_array()`, peeled by skip=true).
    if (expr._type == null || !expr._type.isGoodArrayType) return null
    return emit_decs_to_array(bridge, chainInfo, rangeInfo, calls, rangeInfo.chainEnd, at)
}

// ── decs group_by splice (Slice 5e — state-table over for_each_archetype) ───────

[macro_function]
def private plan_decs_group_by(var expr : Expression?) : Expression? {
    // Decs-bridge entry to plan_group_by_core. Recognizer + extract_decs_bridge + adapter setup; all splice machinery lives in the shared core (decs adapter dispatches the outer for_each_archetype + inner multi-iter for + tupBind wrap).
    var (top, calls) = flatten_linq(expr)
    if (empty(calls)) return null
    var bridge = extract_decs_bridge(top)
    if (bridge == null || expr._type == null) return null
    // Pop count terminator; bare select-after-group_by → to_array lane. Only the 1-arg `count(src)` form — `count(src, predicate)` (a valid linq.das overload) must cascade so the predicate isn't silently dropped.
    var terminatorName : string  = ""
    {
        let lastName = calls.back()._1.name
        if (lastName == "count" && length(calls.back()._0.arguments) == 1) {
            terminatorName = lastName
            calls |> pop
        }
    }
    // Required tail: select(group_proj) — without it the chain yields raw buckets (no fusion).
    if (empty(calls) || calls.back()._1.name != "select") return null
    var groupProjCall = calls.back()._0
    calls |> pop
    // Optional: having_ between select and group_by_lazy (SQL HAVING shape) — rewritten in plan_group_by_core against accumulator slots.
    var havingCall : ExprCall?
    if (!empty(calls) && calls.back()._1.name == "having_") {
        havingCall = calls.back()._0
        calls |> pop
    }
    // The call immediately before must be group_by_lazy with a key arg.
    if (empty(calls) || calls.back()._1.name != "group_by_lazy") return null
    var groupByCall = calls.back()._0
    calls |> pop
    if ((groupByCall.arguments |> length) < 2) return null
    var keyBlock = clone_expression(groupByCall.arguments[1])
    let at = groupByCall.at
    var adapter = GroupBySourceAdapter(
        initialBindName := "`decs_tup`{at.line}`{at.column}",
        initialElemType = strip_const_ref(clone_type(bridge.elementType)),
        namePrefix := "decs_",
        isDecs = true,
        arrayTop = null,
        arraySrcName := "",
        decsBridge = bridge
    )
    return plan_group_by_core(calls, keyBlock, groupProjCall, havingCall, terminatorName, expr._type.isIterator, at, adapter)
}

// ── decs order family splice (Slice 5d — buffer + order_inplace/top_n/min_by/max_by) ───────

[macro_function]
def private plan_decs_order_family(var expr : Expression?) : Expression? {
    // Decs-bridge entry mirroring plan_order_family. Recognizes [where_*][order|order_descending|order_by|order_by_descending][take(N)|first|first_or_default(d)]? on a from_decs_template source. Always uses a hoisted buffer above for_each_archetype (decs has no random access for the no-where direct path that array-side enjoys), then dispatches to the same daslib helper family (order_inplace / top_n* / min_by / max_by / first_or_default(top_n*(_,1,_),d)).
    var (top, calls) = flatten_linq(expr)
    var bridge = extract_decs_bridge(top)
    if (bridge == null || empty(calls) || expr._type == null) return null
    let at = calls[0]._0.at
    let tupName = "`decs_tup`{at.line}`{at.column}"
    let bufName = "`decs_buf`{at.line}`{at.column}"
    var whereCond : Expression?
    var orderName : string = ""
    var orderKey : Expression?
    var hasOrder = false
    var takeExpr : Expression?
    var firstName : string = ""
    var firstDefaultExpr : Expression?
    for (i in 0 .. length(calls)) {
        var cll & = unsafe(calls[i])
        let name = cll._1.name
        if (name == "where_") {
            if (hasOrder) return null
            var pred = peel_lambda_rename_var(cll._0.arguments[1], tupName)
            if (pred == null) return null
            whereCond = merge_where_cond(whereCond, pred)
        } elif (name == "order" || name == "order_descending"
                || name == "order_by" || name == "order_by_descending") {
            if (hasOrder) return null
            let argCount = cll._0.arguments |> length
            // Bail on `order(arr, cmp)` / `order_descending(arr, cmp)` — splice helpers (min/max/top_n) can't honor a user-supplied comparator and would silently drop it. Matches plan_order_family.
            if ((name == "order" || name == "order_descending") && argCount >= 2) return null
            hasOrder = true
            orderName = name
            if (argCount >= 2) {
                orderKey = clone_expression(cll._0.arguments[1])
            }
        } elif (name == "take") {
            if (!hasOrder || takeExpr != null || firstName != "") return null
            var arg = cll._0.arguments[1]
            if (arg == null || arg._type == null || arg._type.baseType != Type.tInt) return null
            takeExpr = clone_expression(arg)
        } elif (name == "first" || name == "first_or_default") {
            // order + first → min/max (O(N) instead of sort + index). Must be terminal.
            if (!hasOrder || takeExpr != null || firstName != "" || i != length(calls) - 1) return null
            firstName = name
            if (name == "first_or_default") {
                if ((cll._0.arguments |> length) < 2) return null
                firstDefaultExpr = clone_expression(cll._0.arguments[1])
            }
        } else {
            return null
        }
    }
    if (!hasOrder) return null
    let hasKey = orderName == "order_by" || orderName == "order_by_descending"
    let topNName = order_top_n_call_name(orderName)
    let inplaceName = "{orderName}_inplace"
    let minMaxName = order_min_call_name(orderName, hasKey)
    var elemType = strip_const_ref(clone_type(bridge.elementType))
    var inlineCmp : Expression?
    if (hasKey) {
        inlineCmp = try_make_inline_cmp(orderKey, orderName, elemType, at)
    }
    var perElement : Expression? = qmacro_expr() {
        $i(bufName) |> push_clone($i(tupName))
    }
    if (whereCond != null) {
        perElement = qmacro_expr() {
            if ($e(whereCond)) {
                $e(perElement)
            }
        }
    }
    var tupBind = build_decs_tup_bind(bridge, tupName, at)
    var forExprNode = build_decs_inner_for(bridge, tupBind, perElement, at)
    var reqExpr = clone_expression(bridge.reqHashExpr)
    var erqExpr = clone_expression(bridge.erqExpr)
    let archName = bridge.archName
    var bodyStmts : array<Expression?>
    bodyStmts |> reserve(5)
    bodyStmts |> push <| qmacro_expr() {
        var $i(bufName) : array<$t(elemType)>
    }
    bodyStmts |> push <| qmacro_expr() {
        for_each_archetype($e(reqExpr), $e(erqExpr), $($i(archName) : Archetype) {
            $e(forExprNode)
        })
    }
    let needIterWrap = expr._type.isIterator
    var emission : Expression?
    if (firstName == "first") {
        // order + first → min/max on buffer. Empty buf must panic to match eager `first()` semantics.
        bodyStmts |> push <| qmacro_expr() {
            panic("sequence contains no elements") if (empty($i(bufName)))
        }
        var minMaxCall : Expression?
        if (hasKey) {
            minMaxCall = qmacro($c(minMaxName)($i(bufName), $e(orderKey)))
        } else {
            minMaxCall = qmacro($c(minMaxName)($i(bufName)))
        }
        bodyStmts |> push <| qmacro_expr() {
            return $e(minMaxCall)
        }
        emission = qmacro(invoke($() : $t(elemType) {
            $b(bodyStmts)
        }))
    } elif (firstName == "first_or_default") {
        // No min_by_or_default helper exists; route through top_n*(_, 1, _) + first_or_default for the empty-buf case. The default expression is hoisted into a let BEFORE for_each_archetype so its side effects fire ahead of iteration (matches plan_decs_reverse's first_or_default + the eager-arg-eval semantics of the non-spliced chain).
        let dBindName = "`order_d`{at.line}`{at.column}"
        var foDStmts : array<Expression?>
        foDStmts |> reserve(4)
        foDStmts |> push <| qmacro_expr() {
            let $i(dBindName) = $e(firstDefaultExpr)
        }
        foDStmts |> push(bodyStmts[0])
        foDStmts |> push(bodyStmts[1])
        var topNCall : Expression?
        if (inlineCmp != null) {
            topNCall = qmacro(_::top_n_by_with_cmp($i(bufName), 1, $e(inlineCmp)))
        } elif (hasKey) {
            topNCall = qmacro($c(topNName)($i(bufName), 1, $e(orderKey)))
        } else {
            topNCall = qmacro($c(topNName)($i(bufName), 1))
        }
        foDStmts |> push <| qmacro_expr() {
            return _::first_or_default($e(topNCall), $i(dBindName))
        }
        emission = qmacro(invoke($() : $t(elemType) {
            $b(foDStmts)
        }))
    } elif (takeExpr == null) {
        // Bare order family — sort buffer in place and return.
        var sortCall : Expression?
        if (inlineCmp != null) {
            sortCall = qmacro(_::order_inplace($i(bufName), $e(inlineCmp)))
        } elif (hasKey) {
            sortCall = qmacro($c(inplaceName)($i(bufName), $e(orderKey)))
        } else {
            sortCall = qmacro($c(inplaceName)($i(bufName)))
        }
        bodyStmts |> push(sortCall)
        bodyStmts |> push <| qmacro_expr() {
            return <- $i(bufName)
        }
        emission = qmacro(invoke($() : array<$t(elemType)> {
            $b(bodyStmts)
        }))
    } else {
        // order + take → top_n* dispatch on the buffer.
        var topNCall : Expression?
        if (inlineCmp != null) {
            topNCall = qmacro(_::top_n_by_with_cmp($i(bufName), $e(takeExpr), $e(inlineCmp)))
        } elif (hasKey) {
            topNCall = qmacro($c(topNName)($i(bufName), $e(takeExpr), $e(orderKey)))
        } else {
            topNCall = qmacro($c(topNName)($i(bufName), $e(takeExpr)))
        }
        bodyStmts |> push <| qmacro_expr() {
            return <- $e(topNCall)
        }
        emission = qmacro(invoke($() : array<$t(elemType)> {
            $b(bodyStmts)
        }))
    }
    emission.force_at(at)
    emission.force_generated(true)
    // Bare order + take both return array; wrap to iterator when the user's outer context demands it. first/first_or_default return scalar — no wrap.
    let returnsArray = firstName == ""
    if (needIterWrap && returnsArray) {
        emission = qmacro($e(emission).to_sequence_move())
    }
    return emission
}

// ── decs reverse splice (Slice 5d — buffer + reverse_inplace; or counter/walk-last for terminators) ───────

[macro_function]
def private plan_decs_reverse(var expr : Expression?) : Expression? {
    // Decs-bridge entry mirroring plan_reverse. Recognizes [where_*][select?][reverse][take(N)?][count|first|first_or_default(d)]? on a from_decs_template source. Three emit paths: count → counter loop (reverse is identity for count); first/first_or_default → walk + overwrite `last` (reverse-of-last = first-of-reverse); else → buffer fill + reverse_inplace + optional resize. R6 backward-index optimization from array-side doesn't apply: multi-iter for has no random access.
    var (top, calls) = flatten_linq(expr)
    var bridge = extract_decs_bridge(top)
    if (bridge == null || empty(calls) || expr._type == null) return null
    var terminatorName : string = ""
    var terminatorCall : ExprCall?
    {
        let lastName = calls.back()._1.name
        if (lastName == "count" && length(calls.back()._0.arguments) == 1) {
            terminatorName = lastName
            terminatorCall = calls.back()._0
            calls |> pop
        } elif (lastName == "first" || lastName == "first_or_default") {
            terminatorName = lastName
            terminatorCall = calls.back()._0
            calls |> pop
        }
    }
    if (empty(calls)) return null
    let at = calls[0]._0.at
    let tupName = "`decs_tup`{at.line}`{at.column}"
    var whereCond : Expression?
    var projection : Expression?
    var hasReverse = false
    var seenSelect = false
    var takeExpr : Expression?
    for (i in 0 .. length(calls)) {
        var cll & = unsafe(calls[i])
        let name = cll._1.name
        if (name == "where_") {
            if (hasReverse || seenSelect) return null
            var pred = peel_lambda_rename_var(cll._0.arguments[1], tupName)
            if (pred == null) return null
            whereCond = merge_where_cond(whereCond, pred)
        } elif (name == "select") {
            if (hasReverse || seenSelect) return null
            seenSelect = true
            projection = peel_lambda_rename_var(cll._0.arguments[1], tupName)
            if (projection == null) return null
        } elif (name == "reverse") {
            if (hasReverse) return null
            hasReverse = true
        } elif (name == "take") {
            if (!hasReverse || takeExpr != null) return null
            var arg = cll._0.arguments[1]
            if (arg == null || arg._type == null || arg._type.baseType != Type.tInt) return null
            takeExpr = clone_expression(arg)
        } else {
            return null
        }
    }
    if (!hasReverse || (takeExpr != null && terminatorName != "")) return null
    var reqExpr = clone_expression(bridge.reqHashExpr)
    var erqExpr = clone_expression(bridge.erqExpr)
    let archName = bridge.archName
    if (terminatorName == "count") {
        // Reverse is identity for count — counter loop, no buffer. Side-effecting projection still fires per match.
        let cntName = "`decs_cnt`{at.line}`{at.column}"
        var perElement : Expression?
        if (projection != null && has_sideeffects(projection)) {
            let vfinalName = "`decs_vfinal`{at.line}`{at.column}"
            perElement = qmacro_block() {
                var $i(vfinalName) = $e(projection)
                $i(cntName) ++
            }
        } else {
            perElement = qmacro_expr() {
                $i(cntName) ++
            }
        }
        if (whereCond != null) {
            perElement = qmacro_expr() {
                if ($e(whereCond)) {
                    $e(perElement)
                }
            }
        }
        var tupBind = build_decs_tup_bind(bridge, tupName, at)
        var forExprNode = build_decs_inner_for(bridge, tupBind, perElement, at)
        var emission : Expression? = qmacro(invoke($() : int {
            var $i(cntName) = 0
            for_each_archetype($e(reqExpr), $e(erqExpr), $($i(archName) : Archetype) {
                $e(forExprNode)
            })
            return $i(cntName)
        }))
        emission.force_at(at)
        emission.force_generated(true)
        return emission
    }
    if (terminatorName == "first" || terminatorName == "first_or_default") {
        // "first of reversed" = LAST surviving element. Walk all archetypes, overwrite `last` on each match. No buffer.
        let foundName = "`decs_found`{at.line}`{at.column}"
        let lastName = "`decs_last`{at.line}`{at.column}"
        let dBindName = "`decs_d`{at.line}`{at.column}"
        var lastType = strip_const_ref(clone_type(projection != null ? projection._type : bridge.elementType))
        var valueExpr : Expression?
        if (projection != null) {
            valueExpr = clone_expression(projection)
        } else {
            valueExpr = qmacro_expr() {
                $i(tupName)
            }
        }
        var matchBlock : Expression? = qmacro_block() {
            $i(lastName) := $e(valueExpr)
            $i(foundName) = true
        }
        var perElement : Expression? = matchBlock
        if (whereCond != null) {
            perElement = qmacro_expr() {
                if ($e(whereCond)) {
                    $e(perElement)
                }
            }
        }
        var tupBind = build_decs_tup_bind(bridge, tupName, at)
        var forExprNode = build_decs_inner_for(bridge, tupBind, perElement, at)
        var emission : Expression?
        if (terminatorName == "first") {
            emission = qmacro(invoke($() : $t(lastType) {
                var $i(foundName) = false
                var $i(lastName) : $t(lastType) = default<$t(lastType)>
                for_each_archetype($e(reqExpr), $e(erqExpr), $($i(archName) : Archetype) {
                    $e(forExprNode)
                })
                if (!$i(foundName)) {
                    panic("sequence contains no elements")
                }
                return $i(lastName)
            }))
        } else {
            var defaultExpr = clone_expression(terminatorCall.arguments[1])
            emission = qmacro(invoke($() : $t(lastType) {
                let $i(dBindName) = $e(defaultExpr)
                var $i(foundName) = false
                var $i(lastName) : $t(lastType) = default<$t(lastType)>
                for_each_archetype($e(reqExpr), $e(erqExpr), $($i(archName) : Archetype) {
                    $e(forExprNode)
                })
                return $i(foundName) ? $i(lastName) : $i(dBindName)
            }))
        }
        emission.force_at(at)
        emission.force_generated(true)
        return emission
    }
    // Buffer + reverse_inplace + optional resize. Implicit to_array (no terminator).
    let bufName = "`decs_buf`{at.line}`{at.column}"
    let needIterWrap = expr._type.isIterator
    var bufElemType = strip_const_ref(clone_type(projection != null ? projection._type : bridge.elementType))
    var pushExpr : Expression?
    if (projection != null) {
        pushExpr = qmacro_expr() {
            $i(bufName) |> push_clone($e(projection))
        }
    } else {
        pushExpr = qmacro_expr() {
            $i(bufName) |> push_clone($i(tupName))
        }
    }
    var perElement : Expression? = pushExpr
    if (whereCond != null) {
        perElement = qmacro_expr() {
            if ($e(whereCond)) {
                $e(perElement)
            }
        }
    }
    var tupBind = build_decs_tup_bind(bridge, tupName, at)
    var forExprNode = build_decs_inner_for(bridge, tupBind, perElement, at)
    var resizeStmts : array<Expression?>
    if (takeExpr != null) {
        // take(N) of reversed-buffer = last N of source reversed. Hoist once to a let so a side-effecting take expression evaluates exactly once.
        let takeNName = "`take_n`{at.line}`{at.column}"
        resizeStmts |> push <| qmacro_expr() {
            let $i(takeNName) = $e(takeExpr)
        }
        resizeStmts |> push <| qmacro_expr() {
            $i(bufName) |> resize($i(takeNName) <= 0 ? 0 : ($i(takeNName) < length($i(bufName)) ? $i(takeNName) : length($i(bufName))))
        }
    }
    var emission : Expression? = qmacro(invoke($() : array<$t(bufElemType)> {
        var $i(bufName) : array<$t(bufElemType)>
        for_each_archetype($e(reqExpr), $e(erqExpr), $($i(archName) : Archetype) {
            $e(forExprNode)
        })
        _::reverse_inplace($i(bufName))
        $b(resizeStmts)
        return <- $i(bufName)
    }))
    emission.force_at(at)
    emission.force_generated(true)
    if (needIterWrap) {
        emission = qmacro($e(emission).to_sequence_move())
    }
    return emission
}

// ── decs distinct splice (Slice 5c — streaming dedup table hoisted above for_each_archetype) ───────

[macro_function]
def private plan_decs_distinct(var expr : Expression?) : Expression? {
    // Decs-bridge entry mirroring plan_distinct. Recognizes [where_*][select?] |> distinct[_by(key)] [|> take(N)]? [|> {count, long_count, sum, to_array}]?. Two emit shapes: buffer-required (to_array, optionally with take(N)) and buffer-free (count/long_count return length(seen); sum folds inline at fresh-key site). Take(N) flips outer iteration to for_each_archetype_find so the take-cap can stop across archetypes; counter / seen-table hoist above for_each_archetype so state persists.
    var (top, calls) = flatten_linq(expr)
    var bridge = extract_decs_bridge(top)
    if (bridge == null || empty(calls) || expr._type == null) return null
    var terminatorName : string = ""
    {
        let lastName = calls.back()._1.name
        if ((lastName == "count" || lastName == "long_count" || lastName == "sum")
                && length(calls.back()._0.arguments) == 1) {
            // 1-arg form only — `count(src, predicate)` / `long_count(src, predicate)` would silently drop the predicate (sum has no selector overload today, but kept under the same guard for symmetry with plan_distinct). Bail to tier-2 which correctly evaluates the predicate.
            terminatorName = lastName
            calls |> pop
        }
    }
    if (empty(calls)) return null
    let at = calls[0]._0.at
    let tupName     = "`decs_tup`{at.line}`{at.column}"
    let bufName     = "`decs_buf`{at.line}`{at.column}"
    let seenName    = "`decs_seen`{at.line}`{at.column}"
    let keyName     = "`decs_k`{at.line}`{at.column}"
    let takenName   = "`decs_taken`{at.line}`{at.column}"
    let takeLimName = "`decs_takeLim`{at.line}`{at.column}"
    let accName     = "`decs_acc`{at.line}`{at.column}"
    let pvName      = "`decs_pv`{at.line}`{at.column}"
    var whereCond : Expression?
    var projection : Expression?
    var hasDistinct = false
    var seenSelect = false
    var isDistinctBy = false
    var distinctKeyBlock : Expression?
    var takeExpr : Expression?
    for (i in 0 .. length(calls)) {
        var cll & = unsafe(calls[i])
        let name = cll._1.name
        if (name == "where_") {
            if (hasDistinct || seenSelect) return null
            var pred = peel_lambda_rename_var(cll._0.arguments[1], tupName)
            if (pred == null) return null
            whereCond = merge_where_cond(whereCond, pred)
        } elif (name == "select") {
            if (hasDistinct || seenSelect) return null
            seenSelect = true
            projection = peel_lambda_rename_var(cll._0.arguments[1], tupName)
            if (projection == null) return null
        } elif (name == "distinct") {
            if (hasDistinct) return null
            hasDistinct = true
        } elif (name == "distinct_by") {
            if (hasDistinct) return null
            hasDistinct = true
            isDistinctBy = true
            distinctKeyBlock = clone_expression(cll._0.arguments[1])
        } elif (name == "take") {
            if (!hasDistinct || takeExpr != null) return null
            var arg = cll._0.arguments[1]
            if (arg == null || arg._type == null || arg._type.baseType != Type.tInt) return null
            takeExpr = clone_expression(arg)
        } else {
            return null
        }
    }
    // take(N) only composes with the implicit-to_array terminator. count/sum + take would defeat the streaming early-exit (matches array-side bail at plan_distinct).
    if (!hasDistinct || (takeExpr != null && terminatorName != "")) return null
    var elemType = strip_const_ref(clone_type(projection != null ? projection._type : bridge.elementType))
    let isCountTerminator = terminatorName == "count" || terminatorName == "long_count"
    let isSumTerminator = terminatorName == "sum"
    let needBuffer = !isCountTerminator && !isSumTerminator
    // Prelude: seen-table + (buffer | accumulator | take counter). All hoisted at invoke scope so state spans archetypes.
    var preludeStmts : array<Expression?>
    if (isDistinctBy) {
        var keyTypeBlock = clone_expression(distinctKeyBlock)
        preludeStmts |> push <| qmacro_expr() {
            var inscope $i(seenName) : table<typedecl(_::unique_key(invoke($e(keyTypeBlock), default<$t(elemType)>)))>
        }
    } else {
        preludeStmts |> push <| qmacro_expr() {
            var inscope $i(seenName) : table<typedecl(_::unique_key(default<$t(elemType)>))>
        }
    }
    if (needBuffer) {
        preludeStmts |> push <| qmacro_expr() {
            var $i(bufName) : array<$t(elemType)>
        }
    } elif (isSumTerminator) {
        preludeStmts |> push <| qmacro_expr() {
            var $i(accName) : $t(elemType) = default<$t(elemType)>
        }
    }
    if (takeExpr != null) {
        // Bind take(N) limit once at outer scope so a side-effecting arg fires once (matches plan_distinct).
        var takeBind = clone_expression(takeExpr)
        preludeStmts |> push <| qmacro_expr() {
            let $i(takeLimName) = $e(takeBind)
        }
        preludeStmts |> push <| qmacro_expr() {
            var $i(takenName) = 0
        }
    }
    // Bind side-effecting projection once per element; key + buffer/acc share the bind (matches array-side single-eval per source elem).
    let bindProjection = projection != null && has_sideeffects(projection)
    var pushExpr : Expression?
    if (projection != null) {
        if (bindProjection) {
            pushExpr = qmacro_expr() {
                $i(pvName)
            }
        } else {
            pushExpr = clone_expression(projection)
        }
    } else {
        pushExpr = qmacro_expr() {
            $i(tupName)
        }
    }
    var keyExpr : Expression?
    if (isDistinctBy) {
        var keyBlk = clone_expression(distinctKeyBlock)
        var keyArg = clone_expression(pushExpr)
        keyExpr = qmacro_expr() {
            invoke($e(keyBlk), $e(keyArg))
        }
    } else {
        keyExpr = clone_expression(pushExpr)
    }
    // Per-fresh-key consume: only inside the fresh-key branch. take++ first (matches plan_distinct).
    var consumeStmts : array<Expression?>
    if (takeExpr != null) {
        consumeStmts |> push <| qmacro_expr() {
            $i(takenName) ++
        }
    }
    consumeStmts |> push <| qmacro_expr() {
        $i(seenName) |> insert($i(keyName))
    }
    if (needBuffer) {
        var pushClone = clone_expression(pushExpr)
        consumeStmts |> push <| qmacro_expr() {
            $i(bufName) |> push_clone($e(pushClone))
        }
    } elif (isSumTerminator) {
        var sumExpr = clone_expression(pushExpr)
        consumeStmts |> push <| qmacro_expr() {
            $i(accName) += $e(sumExpr)
        }
    }
    var consumeBody = stmts_to_expr(consumeStmts)
    var ifNew : Expression? = qmacro_expr() {
        if (!$i(seenName) |> key_exists($i(keyName))) {
            $e(consumeBody)
        }
    }
    var perMatchStmts : array<Expression?>    // nolint:STYLE012 — array-literal-of-qmacro_expr is parse-fragile
    if (bindProjection) {
        var pvBind = clone_expression(projection)
        perMatchStmts |> push <| qmacro_expr() {
            let $i(pvName) = $e(pvBind)
        }
    }
    perMatchStmts |> push <| qmacro_expr() {
        let $i(keyName) = _::unique_key($e(keyExpr))
    }
    perMatchStmts |> push(ifNew)
    var perElement = stmts_to_expr(perMatchStmts)
    perElement = wrap_with_condition(perElement, whereCond)
    // Top-of-inner-for take-cap guard: `return true` exits both the inner for AND the archetype-find lambda, propagating the stop across archetypes.
    if (takeExpr != null) {
        perElement = qmacro_expr() {
            {
                if ($i(takenName) >= $i(takeLimName)) return true
                $e(perElement)
            }
        }
    }
    var tupBind = build_decs_tup_bind(bridge, tupName, at)
    var forExprNode = build_decs_inner_for(bridge, tupBind, perElement, at)
    var reqExpr = clone_expression(bridge.reqHashExpr)
    var erqExpr = clone_expression(bridge.erqExpr)
    let archName = bridge.archName
    var bodyStmts : array<Expression?>
    bodyStmts |> reserve(length(preludeStmts) + 2)
    bodyStmts |> push_from(preludeStmts)
    if (takeExpr != null) {
        bodyStmts |> push <| qmacro_expr() {
            for_each_archetype_find($e(reqExpr), $e(erqExpr), $($i(archName) : Archetype) : bool {
                $e(forExprNode)
                return false
            })
        }
    } else {
        bodyStmts |> push <| qmacro_expr() {
            for_each_archetype($e(reqExpr), $e(erqExpr), $($i(archName) : Archetype) {
                $e(forExprNode)
            })
        }
    }
    var emission : Expression?
    if (terminatorName == "count") {
        bodyStmts |> push <| qmacro_expr() {
            return length($i(seenName))
        }
        emission = qmacro(invoke($() : int {
            $b(bodyStmts)
        }))
    } elif (terminatorName == "long_count") {
        bodyStmts |> push <| qmacro_expr() {
            return int64(length($i(seenName)))
        }
        emission = qmacro(invoke($() : int64 {
            $b(bodyStmts)
        }))
    } elif (isSumTerminator) {
        bodyStmts |> push <| qmacro_expr() {
            return $i(accName)
        }
        emission = qmacro(invoke($() : $t(elemType) {
            $b(bodyStmts)
        }))
    } else {
        bodyStmts |> push <| qmacro_expr() {
            return <- $i(bufName)
        }
        emission = qmacro(invoke($() : array<$t(elemType)> {
            $b(bodyStmts)
        }))
    }
    emission.force_at(at)
    emission.force_generated(true)
    let needIterWrap = expr._type.isIterator
    if (needIterWrap && needBuffer) {
        emission = qmacro($e(emission).to_sequence_move())
    }
    return emission
}

[macro_function]
def private plan_zip(var expr : Expression?) : Expression? {
    // Phase 2 Z1/Z2/Z3 + accumulator/early-exit: 2-ary lockstep zip splice. Supports bare zip (array/iterator), no-pred count/long_count + accumulator (sum/min/max/average) + early-exit (first/first_or_default/any/all/contains) terminators, and fused where_/select/take/skip/take_while/skip_while chain ops between zip and terminator. Result-selector form (3-arg zip) and chained selects bail to tier-2 cascade.
    var (top, calls) = flatten_linq(expr)
    if (empty(calls) || calls[0]._1.name != "zip") return null
    var zipCall = calls[0]._0
    let zipArgCount = zipCall.arguments |> length
    // Z6 bail: result-selector form (3-arg zip = 2 sources + selector) yields scalar element stream — different splice shape, defer.
    if (zipArgCount != 2) return null
    // Identify recognized terminator. Counter: count/long_count. Accumulator: sum/min/max/average. Early-exit: first/first_or_default/any/all/contains. Anything else: treat as no-terminator (bare → ARRAY lane); unrecognized chain op bails inside the chain walk.
    var lastName = ""
    var intermediateEnd = length(calls)
    if (length(calls) > 1) {
        let candidateName = calls.back()._1.name
        let candidateCall = calls.back()._0
        let candidateArgs = candidateCall.arguments |> length
        if (candidateName == "count" || candidateName == "long_count") {
            // No-pred form only (1 arg = the source). Predicate-form bails (would change per-element work).
            if (candidateArgs != 1) return null
            lastName = candidateName
            intermediateEnd = length(calls) - 1
        } elif (candidateName == "sum" || candidateName == "min" || candidateName == "max" || candidateName == "average") {
            // Accumulator: no-arg form only (1 arg = the source).
            if (candidateArgs != 1) return null
            lastName = candidateName
            intermediateEnd = length(calls) - 1
        } elif (candidateName == "first") {
            if (candidateArgs != 1) return null
            lastName = candidateName
            intermediateEnd = length(calls) - 1
        } elif (candidateName == "first_or_default" || candidateName == "contains") {
            // first_or_default(d): 1 default arg. contains(v): 1 value arg.
            if (candidateArgs != 2) return null
            lastName = candidateName
            intermediateEnd = length(calls) - 1
        } elif (candidateName == "any") {
            // any(): no pred. any(p): 1 predicate arg. Both forms recognized.
            if (candidateArgs != 1 && candidateArgs != 2) return null
            lastName = candidateName
            intermediateEnd = length(calls) - 1
        } elif (candidateName == "all") {
            // all(p): must have predicate.
            if (candidateArgs != 2) return null
            lastName = candidateName
            intermediateEnd = length(calls) - 1
        }
    }
    let at = zipCall.at
    let srcAName = "`srcA`{at.line}`{at.column}"
    let srcBName = "`srcB`{at.line}`{at.column}"
    let bufName  = "`buf`{at.line}`{at.column}"
    let accName  = "`acc`{at.line}`{at.column}"
    let skipName = "`skip`{at.line}`{at.column}"
    let takeCountName = "`tc`{at.line}`{at.column}"
    let skippingName = "`skipping`{at.line}`{at.column}"
    // itName binds the tuple `(itA, itB)` at the top of the for-body; chain ops (where_/select/while-family) reference it by name.
    let itName = "`it`{at.line}`{at.column}"
    var srcAExpr = peel_each(clone_expression(zipCall.arguments[0]))
    var srcBExpr = peel_each(clone_expression(zipCall.arguments[1]))
    if (srcAExpr == null || srcAExpr._type == null || srcBExpr == null || srcBExpr._type == null) return null
    srcAExpr.genFlags.alwaysSafe = true
    srcBExpr.genFlags.alwaysSafe = true
    var srcAType = invoke_src_param_type(srcAExpr)
    var srcBType = invoke_src_param_type(srcBExpr)
    // elementType starts as the tuple-of-refs (zip's natural element) and is rebound by `select`.
    var elementType = clone_type(zipCall._type.firstType)
    var whereCond : Expression?
    var projection : Expression?
    var skipExpr : Expression?
    var takeExpr : Expression?
    var skipWhileCond : Expression?
    var takeWhileCond : Expression?
    var seenSelect = false
    var seenSkip = false
    var seenSkipWhile = false
    var seenTakeWhile = false
    var seenTake = false
    var allProjectionsPure = true
    // Z3 chain walk: fuse where_/select/take/skip/take_while/skip_while between zip and terminator. Predicates/projections receive the tuple element via peel_lambda_replace_var substitution with `(itA, itB)` — typer collapses `t._0/_1` to the raw iter vars.
    for (i in 1 .. intermediateEnd) {
        var cll & = unsafe(calls[i])
        let opName = cll._1.name
        if (opName == "where_") {
            // Canonical chain order: skip/take/while-family come after where_/select. After-select chained where: defer (would require chained-bind dedup, see Phase 3d for single-source analog).
            if (seenSkip || seenSkipWhile || seenTakeWhile || seenTake || seenSelect) return null
            var predicate : Expression? = peel_lambda_rename_var(cll._0.arguments[1], itName)
            if (predicate == null) return null
            if (whereCond == null) {
                whereCond = predicate
            } else {
                whereCond = qmacro($e(whereCond) && $e(predicate))
            }
        } elif (opName == "select") {
            // Skip/take/while come after select; chained selects on zip stream defer (would mirror plan_loop_or_count `intermediateBinds`).
            if (seenSkip || seenSkipWhile || seenTakeWhile || seenTake || seenSelect) return null
            // Pull elementType from the projection block's return expression — the SELECT call's `_type.firstType` is a typedecl(result_selector(...)) formula that doesn't resolve when the chain is wrapped in _fold (call-macro context defers generic typedecl evaluation).
            var projRet = peel_lambda_single_return(cll._0.arguments[1])
            if (projRet == null || projRet._type == null) return null
            var projElemType = clone_type(projRet._type)
            // Strip both constness and ref-ness — `array<T const>` is unwritable, `array<T&>` is invalid as buffer element. Matches the canonical pattern used by plan_reverse/plan_distinct/etc.
            projElemType = strip_const_ref(projElemType)
            projection = peel_lambda_rename_var(cll._0.arguments[1], itName)
            if (projection == null) return null
            elementType = projElemType
            seenSelect = true
            if (has_sideeffects(projection)) {
                allProjectionsPure = false
            }
        } elif (opName == "skip") {
            if (seenSkip || seenSkipWhile || seenTakeWhile || seenTake) return null
            var skipArg = cll._0.arguments[1]
            if (skipArg == null || skipArg._type == null || skipArg._type.baseType != Type.tInt) return null
            skipExpr = clone_expression(skipArg)
            seenSkip = true
        } elif (opName == "skip_while") {
            if (seenSelect || seenSkipWhile || seenTakeWhile || seenTake) return null
            var swArg = cll._0.arguments[1]
            if (swArg == null) return null
            skipWhileCond = peel_lambda_rename_var(swArg, itName)
            if (skipWhileCond == null) return null
            seenSkipWhile = true
        } elif (opName == "take_while") {
            if (seenSelect || seenTakeWhile || seenTake) return null
            var twArg = cll._0.arguments[1]
            if (twArg == null) return null
            takeWhileCond = peel_lambda_rename_var(twArg, itName)
            if (takeWhileCond == null) return null
            seenTakeWhile = true
        } elif (opName == "take") {
            if (seenTake) return null
            var takeArg = cll._0.arguments[1]
            if (takeArg == null || takeArg._type == null || takeArg._type.baseType != Type.tInt) return null
            takeExpr = clone_expression(takeArg)
            seenTake = true
        } else {
            // Any other intermediate op (group_by, distinct, order_*, second zip, etc.): bail to tier-2.
            return null
        }
    }
    let noLimits = skipExpr == null && takeExpr == null && skipWhileCond == null && takeWhileCond == null
    let noChain = whereCond == null && projection == null && noLimits
    let bothHaveLength = type_has_length(srcAExpr._type) && type_has_length(srcBExpr._type)
    let isCounter = lastName == "count" || lastName == "long_count"
    // Semantic guard: impure `select` projection AHEAD of a range op (skip/take/skip_while/take_while). The eager `select(proj)|>skip(K)` pipeline runs proj on every element; our spliced order (skip-then-push) would drop the side effects on skipped items. Bail to tier-2 which preserves eager semantics. Plan_loop_or_count has the analogous gap (see PR #2741 review thread #r3269663361); fixing both planners uniformly is a separate follow-up.
    if (seenSelect && !allProjectionsPure && !noLimits) return null
    // Accumulator + early-exit lane dispatch: reuses the generalized emit_*_lane helpers (parallel-array form). preCondStmts threads `let it = (itA, itB)` so itName resolves inside the loop body when the where/projection/predicate/value references the tuple element. `long_count` is classified ACCUMULATOR but routes through the COUNTER path locally (existing length-shortcut + counter loop already cover it); `!isCounter` excludes it from this branch.
    let lane = classify_terminator(lastName)
    if (lane == LinqLane.ACCUMULATOR && !isCounter) {
        // sum/min/max/average without projection: elementType is tuple<...>, accumulator op would not typecheck — bail to tier-2 (typer rejects anyway, but explicit bail keeps the error inside linq's normal cascade rather than the splice).
        if (projection == null) return null
        var preCondStmts : array<Expression?>
        preCondStmts |> push <| qmacro_expr() {     // nolint:STYLE012
            let $i(itName) = (itA, itB)
        }
        var intermediateBinds : array<Expression?>
        var laneTops <- [srcAExpr, srcBExpr]
        let laneSrcs <- [srcAName, srcBName]
        return emit_accumulator_lane(lastName, laneTops, projection, whereCond,
            intermediateBinds, preCondStmts, elementType, laneSrcs, accName, itName, skipName, takeCountName,
            skippingName, skipExpr, takeExpr, skipWhileCond, takeWhileCond, at)
    }
    if (lane == LinqLane.EARLY_EXIT) {
        var preCondStmts : array<Expression?>
        preCondStmts |> push <| qmacro_expr() {     // nolint:STYLE012
            let $i(itName) = (itA, itB)
        }
        var intermediateBinds : array<Expression?>
        let terminatorCall = calls.back()._0
        var laneTops <- [srcAExpr, srcBExpr]
        let laneSrcs <- [srcAName, srcBName]
        return emit_early_exit_lane(lastName, laneTops, projection, whereCond,
            intermediateBinds, preCondStmts, elementType, terminatorCall, laneSrcs, itName, skipName,
            takeCountName, skippingName, skipExpr, takeExpr, skipWhileCond, takeWhileCond, at)
    }
    // Length shortcut: count/long_count, no chain, both length-bearing → return min(lenA, lenB) without entering the loop.
    if (noChain && bothHaveLength && isCounter) {
        var bodyStmts : array<Expression?>
        if (lastName == "count") {
            // length returns int already; bare `length(...)` matches return type — no cast (PERF020).
            bodyStmts |> push <| qmacro_expr() {
                return length($i(srcAName)) < length($i(srcBName)) ? length($i(srcAName)) : length($i(srcBName))
            }
        } else {
            bodyStmts |> push <| qmacro_expr() {
                return int64(length($i(srcAName)) < length($i(srcBName)) ? length($i(srcAName)) : length($i(srcBName)))
            }
        }
        return finalize_zip_invoke(bodyStmts, srcAName, srcBName, srcAType, srcBType, srcAExpr, srcBExpr, at)
    }
    // Build per-element body inside the for-loop. wrap_with_ranges adds skip/take/while guards; wrap_with_condition wraps with `if (whereCond)`.
    var perStmts : array<Expression?>
    if (isCounter) {
        // Counter lane: if projection has side effects, evaluate it for those effects; then `acc++`.
        if (seenSelect && !allProjectionsPure) {
            let finalBindName = "`vfinal`{at.line}`{at.column}"
            perStmts |> push <| qmacro_expr() {
                var $i(finalBindName) = $e(projection)
            }
        }
        perStmts |> push <| qmacro_expr() {
            $i(accName) ++
        }
    } else {
        // Array lane: push projection (if select fused), bound tuple `it` (when any chain op needs it), or raw `(itA, itB)` tuple (bare zip with no chain). needsItBind below decides whether `it` is bound.
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
    wrap_with_ranges(perStmts, skipExpr, takeExpr, skipWhileCond, takeWhileCond, skipName, takeCountName, skippingName)
    var perBody = wrap_with_condition(stmts_to_expr(perStmts), whereCond)
    // Bind `it = (itA, itB)` only when the emitted body actually reads it. Pure `select(...).count()` is the key opt-out: projection isn't evaluated in counter lane (no side effects to preserve), so the bind would be wasted and would also block the `_itA/_itB` underscore-iter-var fast path.
    let projUsedInBody = projection != null && (!isCounter || !allProjectionsPure)
    let needsItBind = whereCond != null || projUsedInBody || skipWhileCond != null || takeWhileCond != null
    if (needsItBind) {
        var preStmts : array<Expression?>
        preStmts |> reserve(2)
        preStmts |> push <| qmacro_expr() {     // nolint:STYLE012
            let $i(itName) = (itA, itB)
        }
        preStmts |> push(perBody)
        perBody = stmts_to_expr(preStmts)
    }
    // Outer body assembly.
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
        // Reserve: only when noChain (no filters/limits) and both sources length-bearing — otherwise size unknown or smaller than min(len).
        if (noChain && bothHaveLength) {
            bodyStmts |> push <| qmacro_expr() {
                $i(bufName) |> reserve(length($i(srcAName)) < length($i(srcBName)) ? length($i(srcAName)) : length($i(srcBName)))
            }
        }
    }
    append_ranges_prelude(bodyStmts, skipExpr, takeExpr, skipWhileCond, skipName, takeCountName, skippingName)
    // Underscore-prefix iter vars (LINT002) only when neither needsItBind nor identity-tuple push references them.
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
        if (expr._type.isIterator) {
            bodyStmts |> push <| qmacro_expr() {
                return <- $i(bufName).to_sequence_move()
            }
        } else {
            bodyStmts |> push <| qmacro_expr() {
                return <- $i(bufName)
            }
        }
    }
    return finalize_zip_invoke(bodyStmts, srcAName, srcBName, srcAType, srcBType, srcAExpr, srcBExpr, at)
}

[macro_function]
def private finalize_zip_invoke(var bodyStmts : array<Expression?>; srcAName, srcBName : string; var srcAType, srcBType : TypeDeclPtr;
                                var srcAExpr, srcBExpr : Expression?; at : LineInfo) : Expression? {
    var res = qmacro(invoke($($i(srcAName) : $t(srcAType), $i(srcBName) : $t(srcBType)) {
        $b(bodyStmts)
    }, $e(srcAExpr), $e(srcBExpr)))
    res.force_at(at)
    res.force_generated(true)
    let blk = (res as ExprInvoke).arguments[0] as ExprMakeBlock
    (blk._block as ExprBlock).arguments[0].flags.can_shadow = true
    (blk._block as ExprBlock).arguments[1].flags.can_shadow = true
    return res
}

[call_macro(name="_fold")]
class private LinqFold : AstCallMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        macro_verify(call.arguments |> length == 1, prog, call.at, "expecting _fold(expression)")
        macro_verify(call.arguments[0]._type != null, prog, call.at, "expecting linq expression")
        // Decs-bridge planners run BEFORE their array-side equivalents (Slice 5d cascade rule — decs planners have stricter guards; running them second lets the generic planner match on the eager-bridge invoke and silently emit the slow shape).
        var res : Expression? = plan_decs_order_family(call.arguments[0])
        if (res != null) return res
        res = plan_order_family(call.arguments[0])
        if (res != null) return res
        res = plan_decs_reverse(call.arguments[0])
        if (res != null) return res
        res = plan_reverse(call.arguments[0])
        if (res != null) return res
        res = plan_decs_distinct(call.arguments[0])
        if (res != null) return res
        res = plan_distinct(call.arguments[0])
        if (res != null) return res
        res = plan_decs_group_by(call.arguments[0])
        if (res != null) return res
        res = plan_group_by(call.arguments[0])
        if (res != null) return res
        res = plan_decs_unroll(call.arguments[0])
        if (res != null) return res
        res = plan_zip(call.arguments[0])
        if (res != null) return res
        res = plan_loop_or_count(call.arguments[0])
        if (res != null) return res
        // Tier 2 — array-shape pipeline with `_inplace` reuse + explicit `delete`.
        res = fold_linq_default(call.arguments[0])
        if (res != null) return res
        // Tier 3 — raw passthrough.
        return clone_expression(call.arguments[0])
    }
}
