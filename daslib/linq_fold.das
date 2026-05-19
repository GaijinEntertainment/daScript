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
require daslib/templates_boost
require daslib/macro_boost
require strings

def private is_call_or_generic(var expr : Expression?, name, moduleName : string) : ExprCall? {
    if (expr is ExprCall) {
        var call = expr as ExprCall
        if ((call.func.name == name && call.func._module.name == moduleName)
                || (call.func.fromGeneric != null && call.func.fromGeneric.name == name && call.func.fromGeneric._module.name == moduleName)) return call
    }
    return null
}

def private is_call_or_generic_arg0(var expr : Expression?, name, moduleName : string) : Expression? {
    var call = expr.is_call_or_generic(name, moduleName)
    return call.arguments[0] if (call != null)
    return null
}

def private fold_linq_cond(var expr : Expression?; argName : string) : Expression? {
    // Peel single-return lambda by renaming its bound var; otherwise emit invoke.
    if (expr is ExprMakeBlock) {
        var mblk = expr as ExprMakeBlock
        var blk = mblk._block as ExprBlock
        if (blk.arguments |> length == 1 && blk.list |> length == 1 && blk.list[0] is ExprReturn) {
            var ret = blk.list[0] as ExprReturn
            if (ret.subexpr != null) {
                var res = clone_expression(ret.subexpr)
                var rules : Template
                rules |> renameVariable(string(blk.arguments[0].name), argName)
                apply_template(rules, res.at, res)
                return res
            }
        }
    }
    return qmacro(invoke($e(expr), $i(argName)))
}

def private fold_linq_cond_peel(var expr : Expression?; var replacement : Expression?) : Expression? {
    // Variant of fold_linq_cond using peel-aware substitution (strips typer ExprRef2Value wrappers on already-typed AST).
    if (expr is ExprMakeBlock) {
        var mblk = expr as ExprMakeBlock
        var blk = mblk._block as ExprBlock
        if (blk.arguments |> length == 1 && blk.list |> length == 1 && blk.list[0] is ExprReturn) {
            var ret = blk.list[0] as ExprReturn
            if (ret.subexpr != null) {
                var res = clone_expression(ret.subexpr)
                var rules : Template
                rules |> replaceVariablePeeling(string(blk.arguments[0].name), replacement)
                apply_template(rules, res.at, res)
                return res
            }
        }
    }
    return qmacro(invoke($e(expr), $e(replacement)))
}

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

[macro_function]
def private is_each_call(call : ExprCall?) : bool {
    if (call == null || call.func == null) return false
    return (call.func.name == "each"
        || (call.func.fromGeneric != null && call.func.fromGeneric.name == "each"))
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
    // take/skip trailing (after to_array strip) → ARRAY lane with implicit materialization.
    if (name == "where_" || name == "select" || name == "take" || name == "skip") return LinqLane.ARRAY
    if (name == "sum" || name == "min" || name == "max" || name == "average" || name == "long_count") return LinqLane.ACCUMULATOR
    if (name == "first" || name == "first_or_default" || name == "any" || name == "all" || name == "contains") return LinqLane.EARLY_EXIT
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
        || name == "zip"
        || name == "join" || name == "left_join" || name == "group_join")
}

[macro_function]
def private peel_each(var top : Expression?) : Expression? {
    // Unwrap `each(<arr>)` so downstream length/reserve hints fire; only safe when arg is a real array (iterator sources lack indexed length).
    if (!(top is ExprCall)) return top
    var topCall = top as ExprCall
    if (!is_each_call(topCall) || topCall.arguments |> length != 1) return top
    let argExpr = topCall.arguments[0]
    if ((argExpr == null || argExpr._type == null)
            || (!argExpr._type.isGoodArrayType && !argExpr._type.isArray)) return top
    return clone_expression(argExpr)
}

[macro_function]
def private finalize_invoke(var res : Expression?; at : LineInfo) : Expression? {
    // Post-emit cleanup: stamp loc+generated for diagnostics; set can_shadow so gensym src coexists with user scope.
    res.force_at(at)
    res.force_generated(true)
    let blk = (res as ExprInvoke).arguments[0] as ExprMakeBlock
    (blk._block as ExprBlock).arguments[0].flags.can_shadow = true
    return res
}

[macro_function]
def private emit_length_shortcut(opName : string; var top : Expression?; srcName : string; at : LineInfo) : Expression? {
    // Count-shaped shortcut: emit `length(src)` (count) or `int64(length(src))` (long_count)
    var topExpr = clone_expression(top)
    topExpr.genFlags.alwaysSafe = true
    let castName = opName == "long_count" ? "int64" : "int"
    var res = qmacro(invoke($($i(srcName) : typedecl($e(topExpr))) {
        return $c(castName)(length($i(srcName)))
    }, $e(topExpr)))
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
    prefixed |> reserve(length(intermediateBinds) + length(stmts))
    for (b in intermediateBinds) {
        prefixed |> push(b)
    }
    for (s in stmts) {
        prefixed |> push(s)
    }
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
    stmts |> reserve(length(preCondStmts) + 1)
    for (s in preCondStmts) {
        stmts |> push(s)
    }
    stmts |> push(body)
    return stmts_to_expr(stmts)
}

[macro_function]
def private append_skip_take_prelude(var preludeStmts : array<Expression?>; var skipExpr : Expression?; var takeExpr : Expression?;
                                     skipName, takeCountName : string) {
    // Loop-level counters for skip/take. Both live in the outer invoke block, alongside any
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
}

[macro_function]
def private wrap_with_skip_take(var stmts : array<Expression?>; var skipExpr : Expression?; var takeExpr : Expression?;
                                skipName, takeCountName : string) {
    // Per-match-block wrapping. Order at the head of the block is:
    if (skipExpr == null && takeExpr == null) return
    var prefixed : array<Expression?>
    prefixed |> reserve(length(stmts) + 3)
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
    if (takeExpr != null) {
        prefixed |> push <| qmacro_expr() {
            $i(takeCountName) ++
        }
    }
    for (s in stmts) {
        prefixed |> push(s)
    }
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
def private emit_counter_lane(var top : Expression?; srcName, accName, itName, skipName, takeCountName : string;
                              var skipExpr : Expression?; var takeExpr : Expression?;
                              var loopBody : Expression?; at : LineInfo) : Expression? {
    // Counter lane: `[skip/take init]; var acc = 0; for (it in src) { $loopBody }; return acc`
    var topExpr = clone_expression(top)
    topExpr.genFlags.alwaysSafe = true
    var srcParamType = invoke_src_param_type(top)
    var bodyStmts : array<Expression?>
    append_skip_take_prelude(bodyStmts, skipExpr, takeExpr, skipName, takeCountName)
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
                            srcName, accName, itName, skipName, takeCountName : string;
                            var skipExpr : Expression?; var takeExpr : Expression?;
                            at : LineInfo) : Expression? {
    // Array lane: `[skip/take init]; var acc : array<T>; [reserve]; for (it in src) { $loopBody }; return <- acc`
    let isIter = expr._type.isIterator
    let sourceHasLength = type_has_length(top._type)
    var topExpr = clone_expression(top)
    topExpr.genFlags.alwaysSafe = true
    var srcParamType = invoke_src_param_type(top)
    var bodyStmts : array<Expression?>
    append_skip_take_prelude(bodyStmts, skipExpr, takeExpr, skipName, takeCountName)
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
                                  var top : Expression?;
                                  var projection : Expression?;
                                  var whereCond : Expression?;
                                  var intermediateBinds : array<Expression?>;
                                  var preCondStmts : array<Expression?>;
                                  var elementType : TypeDeclPtr;
                                  srcName, accName, itName, skipName, takeCountName : string;
                                  var skipExpr : Expression?; var takeExpr : Expression?;
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
        preludeStmts <- qmacro_block_to_array() {
            var $i(accName) : $t(accType) = default<$t(accType)>
            var $i(cntName) = 0
        }
        perMatchStmts <- qmacro_block_to_array() {
            $i(accName) += $e(valueExpr)
            $i(cntName) ++
        }
        returnExpr = qmacro_expr() {
            double($i(accName)) / double($i(cntName))
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
    wrap_with_skip_take(perMatchStmts, skipExpr, takeExpr, skipName, takeCountName)
    var loopBody = prepend_precond(wrap_with_condition(stmts_to_expr(perMatchStmts), whereCond), preCondStmts)
    // Collect all body statements into one list so they share scope when spliced via $b.
    var bodyStmts : array<Expression?>
    bodyStmts |> reserve(length(preludeStmts) + 4)
    append_skip_take_prelude(bodyStmts, skipExpr, takeExpr, skipName, takeCountName)
    for (s in preludeStmts) {
        bodyStmts |> push(s)
    }
    bodyStmts |> push <| qmacro_expr() {
        for ($i(itName) in $i(srcName)) {
            $e(loopBody)
        }
    }
    bodyStmts |> push <| qmacro_expr() {
        return $e(returnExpr)
    }
    var topExpr = clone_expression(top)
    topExpr.genFlags.alwaysSafe = true
    var srcParamType = invoke_src_param_type(top)
    var res = qmacro(invoke($($i(srcName) : $t(srcParamType)) {
        $b(bodyStmts)
    }, $e(topExpr)))
    return finalize_invoke(res, at)
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
                                 var top : Expression?;
                                 var projection : Expression?;
                                 var whereCond : Expression?;
                                 var intermediateBinds : array<Expression?>;
                                 var preCondStmts : array<Expression?>;
                                 var elementType : TypeDeclPtr;
                                 terminatorCall : ExprCall?;
                                 srcName, itName, skipName, takeCountName : string;
                                 var skipExpr : Expression?; var takeExpr : Expression?;
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
            var predExpr = fold_linq_cond(clone_expression(terminatorCall.arguments[1]), valueName)
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
        var predExpr = fold_linq_cond(clone_expression(terminatorCall.arguments[1]), valueName)
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
    } else {
        return null
    }
    prepend_binds(perMatchStmts, intermediateBinds)
    wrap_with_skip_take(perMatchStmts, skipExpr, takeExpr, skipName, takeCountName)
    var loopBody = prepend_precond(wrap_with_condition(stmts_to_expr(perMatchStmts), whereCond), preCondStmts)
    // Single-$b body so all stmts (skip/take counters + prelude + for + tail) share scope
    var bodyStmts : array<Expression?>
    bodyStmts |> reserve(length(preludeStmts) + length(tailStmts) + 3)
    append_skip_take_prelude(bodyStmts, skipExpr, takeExpr, skipName, takeCountName)
    for (s in preludeStmts) {
        bodyStmts |> push(s)
    }
    bodyStmts |> push <| qmacro_expr() {
        for ($i(itName) in $i(srcName)) {
            $e(loopBody)
        }
    }
    for (s in tailStmts) {
        bodyStmts |> push(s)
    }
    var topExpr = clone_expression(top)
    topExpr.genFlags.alwaysSafe = true
    var srcParamType = invoke_src_param_type(top)
    var res = qmacro(invoke($($i(srcName) : $t(srcParamType)) {
        $b(bodyStmts)
    }, $e(topExpr)))
    return finalize_invoke(res, at)
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
    var hasOrder = false
    let at = calls[0]._0.at
    let itName = "`it`{at.line}`{at.column}"
    for (i in 0 .. length(calls)) {
        var cll & = unsafe(calls[i])
        let name = cll._1.name
        if (name == "where_") {
            if (hasOrder) return null   // where-after-order not in scope
            var pred = fold_linq_cond(cll._0.arguments[1], itName)
            if (whereCond == null) {
                whereCond = pred
            } else {
                whereCond = qmacro($e(whereCond) && $e(pred))
            }
        } elif (name == "order" || name == "order_descending"
                || name == "order_by" || name == "order_by_descending") {
            if (hasOrder) return null
            hasOrder = true
            orderName = name
            if ((cll._0.arguments |> length) >= 2) {
                orderKey = clone_expression(cll._0.arguments[1])
            }
            orderElemType = clone_type(cll._0._type.firstType)
        } elif (name == "take") {
            if (!hasOrder || takeExpr != null) return null
            var arg = cll._0.arguments[1]
            if (arg == null || arg._type == null || arg._type.baseType != Type.tInt) return null
            takeExpr = clone_expression(arg)
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
    if (whereCond == null) {
        // No prefilter — direct call to daslib helper.
        var topExpr = clone_expression(top)
        topExpr.genFlags.alwaysSafe = true
        var emission : Expression?
        if (takeExpr == null) {
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
        let emissionIsArray = takeExpr != null || top._type.isGoodArrayType
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
    if (takeExpr == null) {
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
    var whereCond : Expression?
    var projection : Expression?
    var intermediateBinds : array<Expression?>
    // preConditionStmts evaluate UNCONDITIONALLY per element, BEFORE the where filter —
    var preCondStmts : array<Expression?>
    var skipExpr : Expression?
    var takeExpr : Expression?
    var seenSelect = false
    var seenSkip = false
    var seenTake = false
    var allProjectionsPure = true
    var elementType = clone_type(top._type.firstType)
    var lastBindName = itName
    for (i in 0 .. intermediateCount) {
        var cll & = unsafe(calls[i])
        let opName = cll._1.name
        if (opName == "where_") {
            // skip/take-after-where is rejected — canonical chain order is
            if (seenSkip || seenTake) return null
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
                predicate = fold_linq_cond_peel(cll._0.arguments[1], projection)
            } else {
                predicate = fold_linq_cond(cll._0.arguments[1], itName)
            }
            if (whereCond == null) {
                whereCond = predicate
            } else {
                whereCond = qmacro($e(whereCond) && $e(predicate))
            }
        } elif (opName == "select") {
            if (seenSkip || seenTake) return null
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
            projection = fold_linq_cond(cll._0.arguments[1], lastBindName)
            elementType = clone_type(cll._0._type.firstType)
            seenSelect = true
        } elif (opName == "skip") {
            // Canonical chain: at most one skip, before any take. Multiple skips / skip-after-
            if (seenSkip || seenTake) return null
            var skipArg = cll._0.arguments[1]
            if (skipArg == null || skipArg._type == null || skipArg._type.baseType != Type.tInt) return null
            skipExpr = clone_expression(skipArg)
            seenSkip = true
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
    let noLimits = skipExpr == null && takeExpr == null
    // Count-shaped shortcut: when terminator is `count` (→ int) or `long_count` (→ int64),
    let isCountShaped = (lane == LinqLane.COUNTER
        || (lane == LinqLane.ACCUMULATOR && lastName == "long_count"))
    if (isCountShaped && whereCond == null && allProjectionsPure && noLimits
            && type_has_length(top._type))
        return emit_length_shortcut(lastName, top, srcName, at)
    // Ring 1: accumulator lane builds its own per-op loop body (typed accumulator, optional
    if (lane == LinqLane.ACCUMULATOR)
        return emit_accumulator_lane(lastName, top, projection, whereCond,
            intermediateBinds, preCondStmts, elementType, srcName, accName, itName, skipName, takeCountName,
            skipExpr, takeExpr, at)
    // Ring 2: early-exit lane — `any` no-pred + no upstream work + no limits + length-bearing
    if (lane == LinqLane.EARLY_EXIT) {
        let terminatorCall = calls.back()._0
        let isAnyNoPred = lastName == "any" && length(terminatorCall.arguments) == 1
        if (isAnyNoPred && whereCond == null && allProjectionsPure && noLimits
                && type_has_length(top._type))
            return emit_any_empty_shortcut(top, srcName, at)
        return emit_early_exit_lane(lastName, top, projection, whereCond,
            intermediateBinds, preCondStmts, elementType, terminatorCall, srcName, itName, skipName,
            takeCountName, skipExpr, takeExpr, at)
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
        wrap_with_skip_take(stmts, skipExpr, takeExpr, skipName, takeCountName)
        loopBody = prepend_precond(wrap_with_condition(stmts_to_expr(stmts), whereCond), preCondStmts)
    } else {
        // Array lane. `push_clone` is the safe append everywhere: for workhorse types it's a
        var stmts : array<Expression?>
        if (projection != null) {
            stmts |> push <| qmacro_expr() {
                $i(accName) |> push_clone($e(projection))
            }
        } elif (whereCond != null || skipExpr != null || takeExpr != null) {
            // Identity push: `it` aliases the source element. Reached when chain is bare
            stmts |> push <| qmacro_expr() {
                $i(accName) |> push_clone($i(itName))
            }
        } else {
            // identity chain — nothing to fuse; let the caller fall through.
            return null
        }
        prepend_binds(stmts, intermediateBinds)
        wrap_with_skip_take(stmts, skipExpr, takeExpr, skipName, takeCountName)
        loopBody = prepend_precond(wrap_with_condition(stmts_to_expr(stmts), whereCond), preCondStmts)
    }
    if (counterLane) {
        return emit_counter_lane(top, srcName, accName, itName, skipName, takeCountName,
            skipExpr, takeExpr, loopBody, at)
    } else {
        return emit_array_lane(top, expr, loopBody, elementType, srcName, accName, itName,
            skipName, takeCountName, skipExpr, takeExpr, at)
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
            whereCond = merge_where_cond(whereCond, fold_linq_cond(cll._0.arguments[1], itName))
        } elif (name == "select") {
            if (hasReverse || seenSelect) return null
            seenSelect = true
            projection = fold_linq_cond(cll._0.arguments[1], itName)
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
        if (lastName == "count" || lastName == "sum" || lastName == "long_count") {
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
            whereCond = merge_where_cond(whereCond, fold_linq_cond(cll._0.arguments[1], itName))
        } elif (name == "select") {
            if (hasDistinct || seenSelect) return null
            seenSelect = true
            projection = fold_linq_cond(cll._0.arguments[1], itName)
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
def private is_bind_field_access(expr : Expression?; bindName : string; fieldIndex : int) : bool {
    // Returns true when `expr` matches `<bindName>._<fieldIndex>` (tuple field read).
    if (expr == null) return false
    var sub = expr
    // Peel ExprRef2Value wrappers the typer inserts around `var`-bound reads.
    while (sub is ExprRef2Value) {
        sub = (sub as ExprRef2Value).subexpr
    }
    if (!(sub is ExprField)) return false
    let fld = sub as ExprField
    let fieldExpected = "_{fieldIndex}"
    if (fld.name != fieldExpected) return false
    var src = fld.value
    while (src is ExprRef2Value) {
        src = (src as ExprRef2Value).subexpr
    }
    if (!(src is ExprVar)) return false
    let v = src as ExprVar
    return v.name == bindName
}

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
        if (is_bind_field_access(c.arguments[0], bindName, 1)) return (true, fnName, null)
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
                        && is_bind_field_access(innerCall.arguments[0], bindName, 1)) return (true, "{fnName}_inner_select", innerCall.arguments[1])
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
            innerBody = fold_linq_cond(clone_expression(innerBare), itName)
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
    if (n < 2 || !is_bind_field_access(mt.values[0], bindName, 0)) return <- (<- specs, false)
    specs |> reserve(n - 1)
    for (i in range(1, n)) {
        let (ok, rn, innerLam) = is_bucket_reducer_call(mt.values[i], bindName)
        if (!ok) {
            specs |> clear
            return <- (<- specs, false)
        }
        var innerBody : Expression?
        if (innerLam != null) {
            innerBody = fold_linq_cond(clone_expression(innerLam), itName)
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
def private rewrite_having_pred(var expr : Expression?; hbName, kvName : string; specs : array<ReducerSpec>) : Expression? {
    // Substitutes `<hb>._0` → `kv._0` and `<reducer>(<hb>._1)` / `<reducer>(select(<hb>._1, <lam>))` → `kv._{slot}` (or divide expr for average) against matching specs; any other use of `<hb>` returns null and cascades.
    if (expr == null) return expr
    // Peel typer-inserted ExprRef2Value wrappers (same pattern as is_bind_field_access).
    while (expr is ExprRef2Value) {
        expr = (expr as ExprRef2Value).subexpr
    }
    if (expr is ExprCall) {
        var c = expr as ExprCall
        if (c.func != null && (c.arguments |> length) == 1) {
            var fnName = string(c.func.name)
            if (c.func.fromGeneric != null) {
                fnName = string(c.func.fromGeneric.name)
            }
            if (is_bind_field_access(c.arguments[0], hbName, 1)) {
                for (s in specs) {
                    if (s.innerBody == null && s.name == fnName) return mk_slot_output_expr(expr.at, kvName, s)
                }
                return null
            }
            let inner = c.arguments[0]
            if (inner != null && inner is ExprCall) {
                let ic = inner as ExprCall
                if (ic.func != null && (ic.arguments |> length) == 2 && is_bind_field_access(ic.arguments[0], hbName, 1)) {
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

[macro_function]
def private plan_group_by(var expr : Expression?) : Expression? {
    var (top, calls) = flatten_linq(expr)
    if (empty(calls)) return null
    top = peel_each(top)
    // Pop count terminator from chain tail; bare select-after-group_by → ARRAY lane.
    var terminatorName : string  = ""
    {
        let lastName = calls.back()._1.name
        if (lastName == "count") {
            terminatorName = lastName
            calls |> pop
        }
    }
    // Required tail: select(group_proj) — without it the chain yields raw buckets (no fusion).
    if (empty(calls) || calls.back()._1.name != "select") return null
    var groupProjCall = calls.back()._0
    calls |> pop
    // Optional: having_ between select and group_by_lazy (SQL HAVING shape) — rewritten below against accumulator slots.
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
    // Setup names + walk upstream segment (where_/select*) for fusion into per-element loop.
    let at = groupByCall.at
    let srcName   = "`source`{at.line}`{at.column}"
    let srcItName = "`it`{at.line}`{at.column}"
    let tabName   = "`tab`{at.line}`{at.column}"
    let kName     = "`k`{at.line}`{at.column}"
    let ukName    = "`uk`{at.line}`{at.column}"
    let entryName = "`entry`{at.line}`{at.column}"
    let kvName    = "`kv`{at.line}`{at.column}"
    let bufName   = "`buf`{at.line}`{at.column}"
    let bindName  = "`gpb`{at.line}`{at.column}"   // group_proj lambda's bound variable
    // Walk upstream where_/select into segments. Each where guards everything AFTER it (lazy LINQ semantics) — a select that follows a where gets its bind emitted INSIDE that where's guard.
    var segBinds : array<array<Expression?>>
    var segWheres : array<Expression?>
    var currentBinds : array<Expression?>
    var currentWhere : Expression?
    var projection : Expression?
    var lastBindName = srcItName
    var elemType = strip_const_ref(clone_type(top._type.firstType))
    var totalBindCount = 0
    for (i in 0 .. (calls |> length)) {
        let opName = calls[i]._1.name
        if (opName == "where_") {
            // Bind any pending projection before the where so the predicate references a stable name (no double-eval).
            if (projection != null) {
                let bn = "`v`{at.line}`{at.column}`{totalBindCount}"
                totalBindCount ++
                currentBinds |> push <| qmacro_expr() {
                    var $i(bn) := $e(projection)
                }
                lastBindName = bn
                projection = null
            }
            var predicate = fold_linq_cond(calls[i]._0.arguments[1], lastBindName)
            if (predicate == null) return null
            currentWhere = merge_where_cond(currentWhere, predicate)
        } elif (opName == "select") {
            // Select after a where belongs in a NEW segment (must execute INSIDE the where's guard) — flush first.
            if (currentWhere != null) {
                segBinds |> emplace(currentBinds)
                segWheres |> push(currentWhere)
                currentWhere = null
            }
            // Bind previous projection (if any) before computing the next.
            if (projection != null) {
                let bn = "`v`{at.line}`{at.column}`{totalBindCount}"
                totalBindCount ++
                currentBinds |> push <| qmacro_expr() {
                    var $i(bn) := $e(projection)
                }
                lastBindName = bn
            }
            projection = fold_linq_cond(calls[i]._0.arguments[1], lastBindName)
            if (projection == null) return null
            elemType = strip_const_ref(clone_type(calls[i]._0._type.firstType))
        } else {
            return null
        }
    }
    // Final bind: stable itName for the recognizer/emission to reference.
    if (projection != null) {
        let bn = "`v`{at.line}`{at.column}`{totalBindCount}"
        totalBindCount ++
        currentBinds |> push <| qmacro_expr() {
            var $i(bn) := $e(projection)
        }
        lastBindName = bn
    }
    // Flush trailing segment.
    segBinds |> emplace(currentBinds)
    segWheres |> push(currentWhere)
    let itName = lastBindName
    // Recognize the group_proj body's reducer shape (uses post-projection itName for inner-select fold).
    var groupProjBody = fold_linq_cond(groupProjCall.arguments[1], bindName)
    if (groupProjBody == null || groupProjBody._type == null) return null
    var (specs, usesNamedTuple) = recognize_reducer_specs(groupProjBody, bindName, itName)
    if (empty(specs)) return null
    var hasAvg = false
    for (s in specs) {
        if (is_average_spec(s)) {
            hasAvg = true
        }
    }
    // Rewrite optional having predicate against accumulator slots; bails on anything other than _._0 / reducer(_._1) matched to a select slot.
    var havingPred : Expression?
    if (havingCall != null) {
        let hbName = "`hb`{at.line}`{at.column}"
        var rawPred = fold_linq_cond(havingCall.arguments[1], hbName)
        if (rawPred == null) return null
        havingPred = rewrite_having_pred(rawPred, hbName, kvName, specs)
        // Safety net: a node type rewrite_having_pred didn't recurse into would leak hbName through to the splice (compile error at the splice site). Bail.
        if (havingPred == null || expr_uses_var(havingPred, hbName)) return null
    }
    // Named-tuple wrap reuses the user's body type (preserves field-name hints); bare reducer synthesizes tuple<KeyT; AccT>.
    var keyBlk1 = clone_expression(keyBlock)
    var keyBlk2 = clone_expression(keyBlock)
    var keyBlkD = clone_expression(keyBlock)
    var stmts : array<Expression?>
    let dummyName = "`dummy`{at.line}`{at.column}"
    // Table + matching dummy. Dummy is bound on miss so first-key-wins is detected via addr-compare (one hash op/elem).
    if (usesNamedTuple) {
        var tabValueType = strip_const_ref(clone_type(groupProjBody._type))
        // Average slots store the 2-tuple <sum;count> internally; result-build divides at output.
        if (hasAvg) {
            for (s in specs) {
                if (is_average_spec(s) && s.slot < (tabValueType.argTypes |> length)) {
                    tabValueType.argTypes[s.slot] = mk_avg_acc_type(at)
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
    // `tab?[uk] ?? dummy` returns ref to existing entry OR to dummy on miss — single hash op per element. addr-compare splits into miss-branch (init+commit) and hit-branch (incremental update); each reducer slot contributes one init stmt and (optionally) one update stmt.
    var keyBlk3 = clone_expression(keyBlock)
    var missInitStmts : array<Expression?>
    var hitUpdateStmts : array<Expression?>
    for (s in specs) {
        // Clone to drop const propagated through tuple destructure.
        let (mi, hu) = emit_reducer_branches(s, at, entryName, itName)
        if (mi != null) {
            missInitStmts |> push(clone_expression(mi))
        }
        if (hu != null) {
            hitUpdateStmts |> push(clone_expression(hu))
        }
    }
    if (empty(missInitStmts)) return null
    var missInit  = stmts_to_expr(missInitStmts)
    let hasHitUpdate = !empty(hitUpdateStmts)
    // Per-element table-update splice — variant on hasHitUpdate (first* chains have no else branch).
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
    stmts |> push <| qmacro_block() {
        for ($i(srcItName) in $i(srcName)) {
            $e(body)
        }
    }
    // Lane: count terminator returns length(tab) directly (number of groups).
    if (terminatorName == "count") {
        if (havingPred != null) {
            let cntName = "`cnt`{at.line}`{at.column}"
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
        // to_array lane: build result buffer by walking the table. Each output element
        var outputExpr : Expression?
        if (!usesNamedTuple) {
            // Bare reducer: avg slot needs sum/count divide; everything else is the raw acc value.
            if (hasAvg) {
                outputExpr = mk_avg_divide_expr(at, kvName, 1)
            } else {
                outputExpr = qmacro_expr() {
                    $i(kvName)._1
                }
            }
        } elif (hasAvg) {
            // Named-tuple with avg slot(s): table value type stores tuple<double;uint64> per avg slot, so re-synthesize the user's named tuple at result-build, dividing avg slots.
            var mt = new ExprMakeTuple(at = at)
            let slotCount = (groupProjBody._type.argNames |> length)
            mt.recordNames |> resize(slotCount)
            mt.values |> push(mk_slot_ref(at, kvName, 0))
            mt.recordNames[0] := string(groupProjBody._type.argNames[0])
            for (i in 1 .. slotCount) {
                var v : Expression?
                for (sp in specs) {
                    if (sp.slot == i) {
                        v = mk_slot_output_expr(at, kvName, sp)
                    }
                }
                mt.values |> push(v)
                mt.recordNames[i] := string(groupProjBody._type.argNames[i])
            }
            outputExpr = mt
        } else {
            // Named-tuple wrap `(K = bind._0, V = bind._1|>length)` — table value type already shaped via tabValueType; emit the entry directly.
            outputExpr = qmacro_expr() {
                $i(kvName)
            }
        }
        // Buffer type: deduce from group_proj body type (already shaped as the named-tuple / reducer result).
        var bufElemType = clone_type(groupProjBody._type)
        if (bufElemType != null) {
            bufElemType.flags.constant = false
            bufElemType.flags.ref = false
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
        stmts |> push(buffer_return(bufName, expr._type.isIterator))
    }
    return finalize_emission_stmts(top, srcName, at, stmts)
}

[call_macro(name="_fold")]
class private LinqFold : AstCallMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        macro_verify(call.arguments |> length == 1, prog, call.at, "expecting _fold(expression)")
        macro_verify(call.arguments[0]._type != null, prog, call.at, "expecting linq expression")
        // Tier 1 splice — try the dedicated order-family planner first (handles
        var res : Expression? = plan_order_family(call.arguments[0])
        if (res != null) return res
        res = plan_reverse(call.arguments[0])
        if (res != null) return res
        res = plan_distinct(call.arguments[0])
        if (res != null) return res
        res = plan_group_by(call.arguments[0])
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
