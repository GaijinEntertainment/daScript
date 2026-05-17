options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module linq_fold shared public

//! LINQ ``_fold`` and ``_old_fold`` macro pair — fuse-time-aware pipeline rewriting.
//!
//! ``_fold(expr)`` is the active fusion macro. ``_old_fold(expr)`` is a frozen
//! baseline that preserves the pre-rewrite behavior; benchmarks compare the two
//! to track performance work as ``_fold`` evolves toward splice-mode fusion.
//!
//! Both macros share the same dispatch infrastructure (``linqCalls`` dict,
//! ``flatten_linq``, ``g_foldSeq``, ``fold_*`` helpers, ``fold_linq_default``);
//! the only difference today is the macro-name string they pass into the
//! recursive sub-fold call inside ``fold_linq_default``.
//!
//! Requires the ``linq`` module for the backing operator functions.

require daslib/linq public

require daslib/ast_boost
require daslib/templates_boost
require daslib/macro_boost

def private is_call_or_generic(var expr : Expression?, name, moduleName : string) : ExprCall? {
    if (expr is ExprCall) {
        var call = expr as ExprCall
        if ((call.func.name == name && call.func._module.name == moduleName)
                || (call.func.fromGeneric != null && call.func.fromGeneric.name == name && call.func.fromGeneric._module.name == moduleName)) return call
    }
    return null
}

def private is_call_or_generic_arg0(var expr : Expression?, name, moduleName : string) : Expression? {
    //! returns null if not to_array, otherwise returns the argument of to_array
    var call = expr.is_call_or_generic(name, moduleName)
    return call.arguments[0] if (call != null)
    return null
}

def private fold_linq_cond(var expr : Expression?; argName : string) : Expression? {
    // if its a block with a single argument and a single return
    // we rename argument to specified value, and give back the return value
    // otherwise its an invoke
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

struct private LinqCall {
    //! Internal struct representing a chained LINQ method call.
    name : string
    moduleName : string = "linq"
    skip : bool = false             // if specified, expression is skipped in folding
    inplace : bool = false          // if specified, expression is inplace and does not create new variable
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

struct private FoldSequence {
    //! Internal struct representing a fold operation sequence.
    calls : array<string>
    folder : function<(
        argIndex : int;
        var topValue : Expression?;
        var blk : ExprBlock?;
        var calls : array<tuple<ExprCall?; LinqCall?>>
    ) : Expression?>
}

var private g_foldSeq = [               // those are applied in order
// order and distinct (for both orders)
    FoldSequence(
        calls = ["order", "distinct" ],
        folder = @@fold_order_distinct
    ),
    FoldSequence(
        calls = ["distinct", "order" ],
        folder = @@fold_order_distinct
    ),
// where + count (single-pass count, no intermediate filter array)
    FoldSequence(
        calls = ["where_", "count"],
        folder = @@fold_where_count
    ),
// select and where
    FoldSequence(
        calls = ["where_", "select" ],
        folder = @@fold_where_select
    ),
    FoldSequence(
        calls = ["select", "where_"],
        folder = @@fold_select_where
    ),
    FoldSequence(
        calls = ["where_"],
        folder = @@fold_where
    ),
    FoldSequence(
        calls = ["select"],
        folder = @@fold_select
    ),
]

[macro_function]
def private append_comprehension(argIndex : int; var topValue : Expression?; var comprehension : Expression?; var blk : ExprBlock?; at : LineInfo) : Expression? {
    comprehension.force_at(at)
    comprehension.force_generated(true)
    let newArgName = "pass_{argIndex}"
    blk.list |> emplace_new <| qmacro_expr() {
        var $i(newArgName) <- $e(comprehension)
    }
    (blk.list.back() as ExprLet).variables[0].flags.can_shadow = true
    if (argIndex != 0) {
        blk.list |> emplace_new <| qmacro_expr() {
            delete $e(topValue)
        }
    }
    return qmacro($i(newArgName))
}

[macro_function]
def private fold_order_distinct(argIndex : int; var topValue : Expression?; var blk : ExprBlock?; var calls : array<tuple<ExprCall?; LinqCall?>>) : Expression? {
    //! replaces order + distinct into a single order + unique
    if (argIndex == 0) {
        var comprehension = qmacro(order_unique_folded($e(topValue)))
        return append_comprehension(argIndex, topValue, comprehension, blk, calls[0]._0.at)
    } else {
        blk.list |> emplace_new <| qmacro(order_unique_folded_inplace($e(topValue)))
        return clone_expression(topValue)
    }
}

[macro_function]
def private fold_where_select(argIndex : int; var topValue : Expression?; var blk : ExprBlock?; var calls : array<tuple<ExprCall?; LinqCall?>>) : Expression? {
    //! folds where + select into a single comprehension
    var eWhere = calls[0]._0
    var eSelect = calls[1]._0
    var iterType = clone_type(eWhere.arguments[0]._type.firstType)
    var whereCond = fold_linq_cond(eWhere.arguments[1], "it")
    var selectExpr : Expression?
    if (eSelect != null) {
        selectExpr = fold_linq_cond(eSelect.arguments[1], "it")
    } else {
        selectExpr = new ExprVar(name := "it", at = topValue.at, _type := iterType)
    }
    var comprehension = qmacro([for (it in $e(topValue)); $e(selectExpr); where $e(whereCond)])
    return append_comprehension(argIndex, topValue, comprehension, blk, calls[0]._0.at)
}

[macro_function]
def private fold_where(argIndex : int; var topValue : Expression?; var blk : ExprBlock?; var calls : array<tuple<ExprCall?; LinqCall?>>) : Expression? {
    //! folds where + select into a single comprehension
    var eWhere = calls[0]._0
    var whereCond = fold_linq_cond(eWhere.arguments[1], "it")
    var comprehension = qmacro([for (it in $e(topValue)); it; where $e(whereCond)])
    return append_comprehension(argIndex, topValue, comprehension, blk, calls[0]._0.at)
}

[macro_function]
def private fold_where_count(argIndex : int; var topValue : Expression?; var blk : ExprBlock?; var calls : array<tuple<ExprCall?; LinqCall?>>) : Expression? {
    //! folds `_where(p) |> count()` into a single-pass loop with the predicate inlined — no intermediate filter array, no block-call overhead
    var eWhere = calls[0]._0
    let srcName = "`source`{argIndex}`{eWhere.at.line}`{eWhere.at.column}"
    let itName  = "`it`{argIndex}`{eWhere.at.line}`{eWhere.at.column}"
    let nName   = "`n`{argIndex}`{eWhere.at.line}`{eWhere.at.column}"
    var whereCond = fold_linq_cond(eWhere.arguments[1], itName)
    var fusedCall : Expression? = qmacro(invoke($($i(srcName) : typedecl($e(topValue)) - const) {
        var $i(nName) = 0
        for ($i(itName) in $i(srcName)) {
            if ($e(whereCond)) {
                $i(nName) ++
            }
        }
        return $i(nName)
    }, $e(topValue)))
    fusedCall.force_at(calls[0]._0.at)
    fusedCall.force_generated(true)
    let newArgName = "pass_{argIndex}"
    blk.list |> emplace_new <| qmacro_expr() {
        var $i(newArgName) = $e(fusedCall)
    }
    (blk.list.back() as ExprLet).variables[0].flags.can_shadow = true
    if (argIndex != 0) {
        blk.list |> emplace_new <| qmacro_expr() {
            delete $e(topValue)
        }
    }
    return qmacro($i(newArgName))
}

[macro_function]
def private fold_select(argIndex : int; var topValue : Expression?; var blk : ExprBlock?; var calls : array<tuple<ExprCall?; LinqCall?>>) : Expression? {
    //! folds select into a single comprehension
    var eSelect = calls[0]._0
    var selectExpr = fold_linq_cond(eSelect.arguments[1], "it")
    var comprehension = qmacro([for (it in $e(topValue)); $e(selectExpr)])
    return append_comprehension(argIndex, topValue, comprehension, blk, calls[0]._0.at)
}

[macro_function]
def private fold_select_where(argIndex : int; var topValue : Expression?; var blk : ExprBlock?; var calls : array<tuple<ExprCall?; LinqCall?>>) : Expression? {
    //! folds where + select into a single comprehension-like expression
    //! note, order of select and where is reversed
    var eWhere = calls[1]._0
    var eSelect = calls[0]._0
    var iterType = clone_type(eWhere.arguments[0]._type.firstType)
    var whereCond : Expression?
    let srcName = "`source`{argIndex}`{eSelect.at.line}`{eSelect.at.column}"
    let valName = "`value`{argIndex}`{eSelect.at.line}`{eSelect.at.column}"
    let itName = "`it`{argIndex}`{eSelect.at.line}`{eSelect.at.column}"
    let arrName = "`arr`{argIndex}`{eSelect.at.line}`{eSelect.at.column}"
    whereCond = fold_linq_cond(eWhere.arguments[1], valName)
    var selectExpr : Expression?
    if (eSelect != null) {
        selectExpr = fold_linq_cond(eSelect.arguments[1], itName)
    } else {
        selectExpr = new ExprVar(name := itName, at = topValue.at, _type := iterType)
    }
    var comprehension = qmacro(invoke($($i(srcName) : typedecl($e(topValue)) - const) {
            var $i(arrName) : array<$t(iterType)>
            for ($i(itName) in $i(srcName)) {
                static_if (typeinfo is_workhorse($e(selectExpr))) {
                    var $i(valName) = $e(selectExpr)
                    if ($e(whereCond)) {
                        $i(arrName).emplace($i(valName))
                    }
                } else {
                    var $i(valName) <- $e(selectExpr)
                    if ($e(whereCond)) {
                        $i(arrName).emplace($i(valName))
                    }
                }
            }
            return <- $i(arrName)
        }, $e(topValue)))
    return append_comprehension(argIndex, topValue, comprehension, blk, calls[0]._0.at)
}

[macro_function]
def private fold_linq_default(var expr : Expression?; recursiveMacroName : string) : Expression? {
    //! fold sequence into
    //!   invoke ( top, $ ( it ) : auto {
    //!      var pass_0 = call0(it,...) // or call0_to_array(it,...)
    //!      var pass_1 = call1(pass_0,...)
    //!      ...
    //!      return <- pass_N // or pass_N.to_sequence() if expr is iterator
    //!   } )
    //!   skip var if call is inplace
    //!   skip delete if call is not first
    //!   if call is first and source is iterator, then call is call_to_array
    //!   if call is last and expr is iterator, then return is pass_N.to_sequence()
    //!   if expr is not linq call, return null
    //!
    //! ``recursiveMacroName`` is the macro name used to wrap sub-pipeline
    //! arguments (e.g. ``zip``'s second arg, ``concat``'s tail) so that each
    //! top-level fold macro keeps recursing into itself: ``_fold`` recurses
    //! into ``_fold``, ``_old_fold`` into ``_old_fold``. Threading the name
    //! through this single call site keeps the frozen baseline truly frozen
    //! once ``_fold`` diverges in later PRs.
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
            // lets find folding sequences
            var found : FoldSequence?
            for (fs in g_foldSeq) {
                if (fs.calls |> length + argIndex - 1 < argMax) {
                    var match = true
                    for (i in 0 .. fs.calls |> length) {
                        if (fs.calls[i] != unsafe(calls[argIndex + i]._1.name)) {
                            match = false
                            break
                        }
                    }
                    if (match) {
                        found = unsafe(addr(fs))
                        break
                    }
                }
            }
            if (found != null) {
                var sub = subarray(calls, argIndex .. argIndex + found.calls |> length)
                topValue = found.folder(argIndex, topValue, blk, sub)
                argIndex += found.calls |> length
            } else {
                if (cll._0._type.isIterator || cll._0._type.isGoodArrayType) {
                    if (argIndex == 0 && top._type.isIterator) {
                        callName = "{callName}_to_array"
                    } elif (argIndex != 0 && cll._1.inplace) {
                        callName = "{callName}_inplace"
                        inplace = true
                    }
                }
                for (i in cll._1.recursive) {   // this is where we make recursion
                    if (i >= 1 && i < cll._0.arguments |> length && is_linq_call(cll._0.arguments[i])) {
                        var argExpr = make_call(cll._0.at, recursiveMacroName)
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
                    // for single-pass chains this triggers PERF009 (move-into-then-return)
                    // at the user call site. Rewriting to direct `return <- call` would
                    // change the historical `_old_fold` baseline that benchmarks compare
                    // against, so we keep the shape and suppress the rule inline below.
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
    // True for types where `length(<expr>)` is statically resolvable: arrays, tables,
    // strings, fixed-arrays (T[N]), and the range family. Lambdas (`def each(lam :
    // lambda<...>)`) and custom user iterables are excluded — they have no length()
    // overload and would make a macro-emitted `reserve(length(src))` fail to compile.
    if (t == null) return false
    return (t.isGoodArrayType || t.isGoodTableType || t.isString
        || t.isArray || t.isRange)
}

[macro_function]
def private is_each_call(call : ExprCall?) : bool {
    //! `each` in daslib/builtin.das is generic, so the resolved `func.name` on a typed
    //! call is the mangled instance name (e.g. `builtin\`each\`30908...`). The generic's
    //! original name lives in `func.fromGeneric.name`. Match either.
    if (call == null || call.func == null) return false
    return (call.func.name == "each"
        || (call.func.fromGeneric != null && call.func.fromGeneric.name == "each"))
}

enum private LinqLane {
    //! Terminator-category classification driving `plan_loop_or_count`'s emit dispatch.
    //! ARRAY: bare or `[where_*][select*]` chain — produces `array<T>` / `iterator<T>`.
    //! COUNTER: `[...] |> count` — produces `int` accumulator.
    //! ACCUMULATOR: `[...] |> {sum,min,max,average,long_count}` — typed single-pass accumulator.
    //! EARLY_EXIT: `[...] |> {first,first_or_default,any,all,contains}` — loop with early return.
    //! UNKNOWN: terminator not recognized by the planner; fall through to plain linq.
    UNKNOWN
    ARRAY
    COUNTER
    ACCUMULATOR
    EARLY_EXIT
}

[macro_function]
def private classify_terminator(name : string) : LinqLane {
    if (name == "count") return LinqLane.COUNTER
    // take/skip as the trailing call (after to_array strip) → implicit array materialization,
    // same lane as bare `[where_*][select*]` chains.
    if (name == "where_" || name == "select" || name == "take" || name == "skip") return LinqLane.ARRAY
    if (name == "sum" || name == "min" || name == "max" || name == "average" || name == "long_count") return LinqLane.ACCUMULATOR
    if (name == "first" || name == "first_or_default" || name == "any" || name == "all" || name == "contains") return LinqLane.EARLY_EXIT
    return LinqLane.UNKNOWN
}

[macro_function]
def private is_buffer_required_op(name : string) : bool {
    // Operators that splice-mode planner can't handle today; each maps to a future emit mode.
    // Recognized by name so the planner can `return null` (silent fallthrough) deliberately
    // instead of falling out the unknown-op default. Future PRs replace each `return null` with
    // a dedicated emit path (BufferTopN / BufferDistinct / BufferReverse / BufferGroupBy /
    // MultiSourceZip / BufferedJoin) without re-walking the chain recognition.
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
    // Unwrap `each(<arr>)` to `<arr>` when `<arr>` is a true array (or fixed-size array).
    // Iteration semantics are preserved: `for it in <arr>` implicitly re-wraps via the
    // same `each` overload. We gate on array-ness because peeling an iterator-typed
    // argument (e.g. `each(range(10))`, `each(generator())`) would put the iterator in
    // place — the downstream length shortcut and reserve-by-length hints assume an
    // indexable source. Only peel when we can prove that's true.
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
    // Shared post-emit cleanup for every `invoke($block, $src)` the planner returns.
    // Stamping the location and the generated-flag keeps diagnostics pointing at the user's
    // chain site, and `can_shadow` lets the gensym source name coexist with whatever the
    // user already has in scope at the splice point.
    res.force_at(at)
    res.force_generated(true)
    let blk = (res as ExprInvoke).arguments[0] as ExprMakeBlock
    (blk._block as ExprBlock).arguments[0].flags.can_shadow = true
    return res
}

[macro_function]
def private emit_length_shortcut(opName : string; var top : Expression?; srcName : string; at : LineInfo) : Expression? {
    // Count-shaped shortcut: emit `length(src)` (count) or `int64(length(src))` (long_count)
    // directly, eliding the loop. Caller has verified count-shaped terminator + no predicate
    // + all projections pure + source has length. `int(int)` is identity, so a single
    // template handles both via `$c(castName)`.
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
    // iterator sources need outer `-const` stripped so the body can advance the handle;
    // length-bearing sources keep modifiers so a `const&` source matches.
    var srcParamType = clone_type(top._type)
    if (top._type.isIterator) {
        srcParamType.flags.constant = false
    }
    return srcParamType
}

[macro_function]
def private prepend_binds(var stmts : array<Expression?>; var intermediateBinds : array<Expression?>) {
    // Splice the chain's let-bindings (one per upstream `select`) ahead of the per-element
    // work. Every lane builds its per-match block this way: prior bindings first, then the
    // op-specific stmts.
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
    // wrap in a qmacro_block otherwise. Used wherever a lane builds an N-stmt block whose
    // length is data-dependent.
    if (length(stmts) == 1) return stmts[0]
    return <- qmacro_block() {
        $b(stmts)
    }
}

[macro_function]
def private wrap_with_condition(var body : Expression?; var cond : Expression?) : Expression? {
    // Wrap `body` in `if ($cond) { body }` when `cond` is non-null, else return `body` as-is.
    // Used by every lane that fuses an upstream `where_` predicate into the per-element work.
    if (cond == null) return body
    return <- qmacro_expr() {
        if ($e(cond)) {
            $e(body)
        }
    }
}

[macro_function]
def private append_skip_take_prelude(var preludeStmts : array<Expression?>; var skipExpr : Expression?; var takeExpr : Expression?;
                                     skipName, takeCountName : string) {
    // Loop-level counters for skip/take. Both live in the outer invoke block, alongside any
    // lane-specific accumulator decl. `skipRem` decrements per skipped match (initial value
    // = the user's K). `takenCount` increments per yielded match; loop breaks when it equals
    // the user's N. Pre-loop init only — emitted by the lane that builds `bodyStmts`.
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
    //   1. take-limit `break` — fires before any projection runs (matches LINQ: `take(N)`
    //      truncates upstream, so the (N+1)th match isn't pulled at all)
    //   2. skip-counter `continue` — drops the first K matched elements
    //   3. take-counter `++` — placed BEFORE the per-match work so early-exit terminators
    //      (`return X` for first/first_or_default/any-no-pred) don't make it unreachable.
    //      LINT001 would flag the trailing-increment placement.
    //   4. existing per-match work
    if (skipExpr == null && takeExpr == null) return
    var prefixed : array<Expression?>
    prefixed |> reserve(length(stmts) + 3)
    if (takeExpr != null) {
        var takeLimit = clone_expression(takeExpr)
        prefixed |> push <| qmacro_expr() {
            if ($i(takeCountName) == $e(takeLimit)) {
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
    // perf-critical branch. Non-workhorse falls back to `_::less` so user / tuple comparator
    // overloads still apply. For max, the operands flip so `_::less` still semantically means
    // "accName is less than valName" (i.e. valName > accName).
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
    // inside invoke. Skip/take counters live alongside `acc` at block scope; the body has
    // the guards/increment baked in by `wrap_with_skip_take` upstream.
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
    // wrapped in invoke. Two orthogonal axes select the body shape:
    //   - whole-pipeline iterator-ness (`expr._type.isIterator`) — drives the `to_sequence_move`
    //     return path so an iterator pipeline still ends in an iterator,
    //   - source-length availability — drives the pre-reserve hint (only useful when source is
    //     array-shaped; iter sources can't reserve cheaply).
    // When `take(N)` is in play the reserve hint tightens to `min(N, length(src))` — at most
    // N elements ever get pushed, so a length-of-source reserve over-allocates by far.
    // Single-$b body shares scope: skip/take counters + `var acc` decl + optional reserve +
    // for-loop + return all live in one block.
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
            // Reserve = min(takeN, length(src)): at most takeN elements ever get pushed.
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
                                  var elementType : TypeDeclPtr;
                                  srcName, accName, itName, skipName, takeCountName : string;
                                  var skipExpr : Expression?; var takeExpr : Expression?;
                                  at : LineInfo
                                  ) : Expression? {
    // Ring 1 single-pass accumulator lane: sum / min / max / average / long_count.
    // Per-op variation lives in (preludeStmts, perMatchStmts, returnExpr) — the invoke shell
    // is one template. For min/max the workhorse-branch on `<` / `>` vs `_::less` is the
    // perf-critical decision (single-instruction vs function call).
    var valueExpr : Expression?
    if (projection != null) {
        valueExpr = clone_expression(projection)
    } else {
        valueExpr = qmacro_expr() {
            $i(itName)
        }
    }
    // Accumulator type: projection-typed if present, else source's element type. Strip
    // `const` and `&` so the accumulator is a mutable value (`+=` / `:=` apply).
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
        // observable side effects so user-visible f(x) calls still fire.
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
    var loopBody = wrap_with_condition(stmts_to_expr(perMatchStmts), whereCond)
    // Collect all body statements into one list so they share scope when spliced via $b.
    // Splitting decls / for / return into separate splice tags would put each in its own
    // sub-block, hiding the accumulator from later statements (caught by AST dump under
    // `options log_infer_passes`). skip/take counter init goes first so the guards inside
    // the perMatch block can name-resolve them.
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
    // upstream where + no impure projection + source has length. Behaviorally identical to
    // running the loop and returning true on first element, but skips the loop entirely.
    // `!empty` over `length > 0` avoids the strlen detour PERF017 flags on string sources.
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
                                 var elementType : TypeDeclPtr;
                                 terminatorCall : ExprCall?;
                                 srcName, itName, skipName, takeCountName : string;
                                 var skipExpr : Expression?; var takeExpr : Expression?;
                                 at : LineInfo
                                 ) : Expression? {
    // Ring 2 early-exit lane: first / first_or_default / any / all / contains.
    // Emits `invoke($block { [arg-binds]; for { ...; return X }; tail }, src)` where the
    // block-level `return` yields the invoke's result without escaping the user's function.
    // Stack-allocated block (no heap alloc), per Boris's confirmation. Per-op variation
    // lives in (preludeStmts, per-element return logic, tailStmts) — the invoke shell is
    // one template; all stmts share scope via a single $b(bodyStmts) splice.
    // If there's a projection chain, bind the final projection to a local so the per-element
    // work names the value directly. Avoids re-evaluating the projection in compares and
    // side-steps the ExprRef2Value substitution trap that splicing typed expressions hits.
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
        // Strip const/ref so `default<T>` (the fallback path) yields a fresh value.
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
        // to avoid re-evaluating an expensive argument.
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
    var loopBody = wrap_with_condition(stmts_to_expr(perMatchStmts), whereCond)
    // Single-$b body so all stmts (skip/take counters + prelude + for + tail) share scope
    // under one wrapping block.
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
def private plan_loop_or_count(var expr : Expression?) : Expression? {
    // Phase-2C loop planner. Recognizes chains of shape `[where_*][select*][skip?][take?]`
    // plus a terminator, dispatched by `classify_terminator` into one of four lanes:
    //   ARRAY        — `[where_*][select*][skip?][take?]` (or trailing take/skip → implicit
    //                  to_array)                                  → array<T> / iterator<T>
    //   COUNTER      — `[...] |> count`                           → int
    //   ACCUMULATOR  — `[...] |> {sum,min,max,average,long_count}` → typed scalar
    //   EARLY_EXIT   — `[...] |> {first,first_or_default,any,all,contains}`
    // Fuses chained wheres into `&&`, chained selects via let-binding composition, and
    // skip/take into bounded-loop counters; emits one inline `invoke($block, $src)`. Returns
    // null for shapes outside scope — caller falls through. Buffer-required ops (`order_by`,
    // `distinct`, `group_by`, `zip`, `join`, `reverse`) are recognized by name and return null
    // with a future-mode marker; future PRs replace each with a dedicated emit path.
    var (top, calls) = flatten_linq(expr)
    if (empty(calls)) return null
    top = peel_each(top)
    let lastName = calls.back()._1.name
    let lane = classify_terminator(lastName)
    // Marker: future PRs add BufferTopN / BufferDistinct / etc. for `is_buffer_required_op`
    // names. Today: silent fallback.
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
            // where-after-select / -after-skip / -after-take is rejected — canonical chain
            // order is [where_*][select*][skip?][take?].
            if (seenSelect || seenSkip || seenTake) return null
            var predicate = fold_linq_cond(cll._0.arguments[1], itName)
            if (whereCond == null) {
                whereCond = predicate
            } else {
                whereCond = qmacro($e(whereCond) && $e(predicate))
            }
        } elif (opName == "select") {
            if (seenSkip || seenTake) return null
            // Chained selects: bind the previous projection to a fresh local now so the next
            // lambda's `_` can be renamed straight to that name — avoids the
            // ExprRef2Value-substitution trap that plain `Template.replaceVariable` hits when
            // splicing a typed expression into another typed expression. Phase 2A only
            // chains workhorse projections; a non-workhorse intermediate binding would need
            // a clone (`:=`) since `<-` (move) can corrupt source for lvalue projections
            // like `_._field`. Deferred to Phase 2B.
            if (projection != null) {
                let prevWorkhorse = projection._type != null && projection._type.isWorkhorseType
                if (!prevWorkhorse) return null   // chained non-workhorse selects — Phase 2B
                if (has_sideeffects(projection)) {
                    allProjectionsPure = false
                }
                let bindName = "`v`{at.line}`{at.column}`{length(intermediateBinds)}"
                intermediateBinds |> push <| qmacro_expr() {
                    var $i(bindName) = $e(projection)
                }
                lastBindName = bindName
            }
            projection = fold_linq_cond(cll._0.arguments[1], lastBindName)
            elementType = clone_type(cll._0._type.firstType)
            seenSelect = true
        } elif (opName == "skip") {
            // Canonical chain: at most one skip, before any take. Multiple skips / skip-after-
            // take falls through (no semantic difficulty, just scope deferral). Range-form
            // `skip(start..end)` is a slice (different operator semantics) — fall through.
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
            // BufferReverse (reverse), BufferGroupBy (group_by/_lazy), MultiSourceZip (zip),
            // BufferedJoin (join/left_join/group_join). Recognized but not yet emitted —
            // future PRs replace each return-null with a dedicated emit path; for now the
            // marker arm fall-throughs identically to the unrecognized-op default.
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
    // there's no filter, no skip/take limits, and every projection is pure, the result is
    // just the source length. Skip the loop entirely. take/skip would truncate the length so
    // the shortcut can't fire there.
    let isCountShaped = (lane == LinqLane.COUNTER
        || (lane == LinqLane.ACCUMULATOR && lastName == "long_count"))
    if (isCountShaped && whereCond == null && allProjectionsPure && noLimits
            && type_has_length(top._type))
        return emit_length_shortcut(lastName, top, srcName, at)
    // Ring 1: accumulator lane builds its own per-op loop body (typed accumulator, optional
    // first-iteration init for min/max, sum+count for average) — dispatch before the
    // COUNTER/ARRAY loopBody construction.
    if (lane == LinqLane.ACCUMULATOR)
        return emit_accumulator_lane(lastName, top, projection, whereCond,
            intermediateBinds, elementType, srcName, accName, itName, skipName, takeCountName,
            skipExpr, takeExpr, at)
    // Ring 2: early-exit lane — `any` no-pred + no upstream work + no limits + length-bearing
    // source gets the empty-shortcut; everything else dispatches to the loop emitter.
    if (lane == LinqLane.EARLY_EXIT) {
        let terminatorCall = calls.back()._0
        let isAnyNoPred = lastName == "any" && length(terminatorCall.arguments) == 1
        if (isAnyNoPred && whereCond == null && allProjectionsPure && noLimits
                && type_has_length(top._type))
            return emit_any_empty_shortcut(top, srcName, at)
        return emit_early_exit_lane(lastName, top, projection, whereCond,
            intermediateBinds, elementType, terminatorCall, srcName, itName, skipName,
            takeCountName, skipExpr, takeExpr, at)
    }
    // Build the per-element loop body for COUNTER / ARRAY. Both lanes follow the same shape:
    // collect per-element stmts (incl. side-effect-preserving projection binds), prepend
    // chain `intermediateBinds`, wrap with skip/take guards, collapse to a single expression,
    // wrap with `whereCond`.
    var loopBody : Expression?
    if (counterLane) {
        // Counter lane must evaluate the projection (and any chained intermediates) per
        // matched element so user-visible side effects fire — `count(select(src, f))` in
        // plain LINQ invokes f per element, and our fold must match. Bind the final
        // projection to a discardable local; daslang macro output bypasses LINT002.
        // Pure projections (the common case — `_._field * 2`) get elided entirely; no need
        // to rely on the optimizer to DCE a dead store afterwards.
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
        loopBody = wrap_with_condition(stmts_to_expr(stmts), whereCond)
    } else {
        // Array lane. `push_clone` is the safe append everywhere: for workhorse types it's a
        // byte copy (same cost as `push`); for non-workhorse it deep-clones, avoiding the
        // mutation that `emplace` would do via move-out — important when the projection
        // returns a ref into the source. For chained selects, `intermediateBinds` carries
        // N-1 prior bindings; splice them in before the push so each lambda body can resolve
        // its renamed parameter to the correct binding name.
        var stmts : array<Expression?>
        if (projection != null) {
            stmts |> push <| qmacro_expr() {
                $i(accName) |> push_clone($e(projection))
            }
        } elif (whereCond != null || skipExpr != null || takeExpr != null) {
            // Identity push: `it` aliases the source element. Reached when chain is bare
            // [where_*][skip?][take?] (no select) — every match (post-filter, post-skip,
            // pre-take) goes into the result array as-is.
            stmts |> push <| qmacro_expr() {
                $i(accName) |> push_clone($i(itName))
            }
        } else {
            // identity chain — nothing to fuse; let the caller fall through.
            return null
        }
        prepend_binds(stmts, intermediateBinds)
        wrap_with_skip_take(stmts, skipExpr, takeExpr, skipName, takeCountName)
        loopBody = wrap_with_condition(stmts_to_expr(stmts), whereCond)
    }
    if (counterLane) {
        return emit_counter_lane(top, srcName, accName, itName, skipName, takeCountName,
            skipExpr, takeExpr, loopBody, at)
    } else {
        return emit_array_lane(top, expr, loopBody, elementType, srcName, accName, itName,
            skipName, takeCountName, skipExpr, takeExpr, at)
    }
}

[call_macro(name="_fold")]
class private LinqFold : AstCallMacro {
    //! implements _fold(expression) that folds LINQ expressions into optimized sequnences
    //! for example::
    //!
    //!     _fold(each(foo)._where(_ > 5)._select(_ * 2))
    //!
    //! expands into a single comprehension that does all operations in one pass
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        //! Visits the _fold macro call and folds LINQ expressions into optimized sequences.
        macro_verify(call.arguments |> length == 1, prog, call.at, "expecting _fold(expression)")
        macro_verify(call.arguments[0]._type != null, prog, call.at, "expecting linq expression")
        var res : Expression? = plan_loop_or_count(call.arguments[0])
        if (res != null) return res
        return clone_expression(call.arguments[0])
    }
}

[call_macro(name="_old_fold")]
class private LinqOldFold : AstCallMacro {
    //! Frozen pre-rewrite baseline of ``_fold``. Same expansion as ``_fold``
    //! today; the two diverge only when later PRs add splice-mode fusion to
    //! ``_fold``. Benchmarks compare ``_fold`` vs ``_old_fold`` to track
    //! performance work — ``_old_fold`` must keep producing the historical
    //! shape so the comparison stays meaningful.
    //! for example::
    //!
    //!     _old_fold(each(foo)._where(_ > 5)._select(_ * 2))
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        macro_verify(call.arguments |> length == 1, prog, call.at, "expecting _old_fold(expression)")
        macro_verify(call.arguments[0]._type != null, prog, call.at, "expecting linq expression")
        var res : Expression? = fold_linq_default(call.arguments[0], "_old_fold")
        if (res == null) {
            prog |> macro_error(call.at, "cannot fold LINQ expression\n{describe(call.arguments[0])}")
            return res
        }
        return res
    }
}
