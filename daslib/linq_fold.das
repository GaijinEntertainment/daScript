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
        var res : Expression? = fold_linq_default(call.arguments[0], "_fold")
        if (res == null) {
            prog |> macro_error(call.at, "cannot fold LINQ expression\n{describe(call.arguments[0])}")
            return res
        }
        return res
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
