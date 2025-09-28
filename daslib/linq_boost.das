options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module linq_boost shared public

/*
LINQ_BOOST
    - provides macros for some LINQ functions to reduce boilerplate

Transform operations
    - _where(it, expr)          => where_(it, $(_) => expr)
    - _select(it, expr)        => select(it, $(_) => expr)
Aggregation operations
    - _min_by(it, expr)         => min_by(it, $(_) => expr)
    - _max_by(it, expr)         => max_by(it, $(_) => expr)
    - _min_max_by(it, expr)     => min_max_by(it, $(_) => expr)
    - _min_max_average_by(it, expr) => min_max_average_by(it, $(_) => expr)
Comparators and keys
    - _sequence_equal_by(it1, it2, expr) => sequence_equal_by(it1, it2, $(_) => expr)
Partitioning data
    - _skip_while(it, expr)     => skip_while(it, $(_) => expr)
    - _take_while(it, expr)     => take_while(it, $(_) => expr)
Querying data
    - _all(it, expr)            => all(it, $(_) => expr)
Set operations
    - _distinct_by(it, expr)     => distinct_by(it, $(_) => expr)
    _ _union_by(it1, it2, expr) => union_by(it1, it2, $(_) => expr)
    - _except_by(it1, it2, expr) => except_by(it1, it2, $(_) => expr)
    - _intersect_by(it1, it2, expr) => intersect_by(it1, it2, $(_) => expr)
Sorting data
    - _order_by(it, expr)       => order_by(it, $(_) => expr)
    - _order_by_descending(it, expr) => order_by_descending(it, $(_) => expr)
*/

require daslib/linq public

require daslib/ast_boost
require daslib/templates_boost
require daslib/macro_boost


def private clone_iterator_type(it : TypeDeclPtr) : TypeDeclPtr {
    var inscope res <- clone_type(it.firstType)
    res.flags &= ~TypeDeclFlags.ref
    res.flags |= TypeDeclFlags.constant
    return <- res
}

class private AstCallMacro_LinqPred2 : AstCallMacro {
    predName = "where_"
    //! This macro converts _where(iterator, expresion) into::
    //!     where_(iterator, $(_) => expression)
    //!
    def override visit(prog : ProgramPtr; mod : Module?; var call : smart_ptr<ExprCallMacro>) : ExpressionPtr {
        // verifying arguments
        macro_verify(call.arguments |> length == 2, prog, call.at, "expecting {predName}(iterator, predicate)")
        assume arg0 = call.arguments[0]
        assume arg0type = call.arguments[0]._type
        macro_verify(arg0type != null, prog, call.at, "input argument is not inferred yet")
        macro_verify(arg0type.isIterator || arg0type.isGoodArrayType, prog, call.at, "expecting iterator or array")
        macro_verify(!arg0type.firstType.isAutoOrAlias, prog, call.at, "iterable type cannot be auto or alias")
        // replacing function
        var inscope iterType <- clone_iterator_type(arg0type)
        var inscope res <- qmacro($c(predName)($e(arg0), $(_ : $t(iterType)) => $e(call.arguments[1])))
        res.force_at(call.at)
        return <- res
    }
}

[call_macro(name="_where")]
class private LinqWhere : AstCallMacro_LinqPred2 {
    override predName = "where_"
}

[call_macro(name="_where_to_array")]
class private LinqWhereToArray : AstCallMacro_LinqPred2 {
    override predName = "where_to_array"
}

[call_macro(name="_where_to_sequence")]
class private LinqWhereToSequence : AstCallMacro_LinqPred2 {
    override predName = "where_to_sequence"
}

[call_macro(name="_select")]
class private LinqSelect : AstCallMacro_LinqPred2 {
    override predName = "select"
}

[call_macro(name="_select_to_array")]
class private LinqSelectToArray : AstCallMacro_LinqPred2 {
    override predName = "select_to_array"
}

[call_macro(name="_select_to_sequence")]
class private LinqSelectToSequence : AstCallMacro_LinqPred2 {
    override predName = "select_to_sequence"
}

[call_macro(name="_min_by")]
class private LinqMinBy : AstCallMacro_LinqPred2 {
    override predName = "min_by"
}

[call_macro(name="_max_by")]
class private LinqMaxBy : AstCallMacro_LinqPred2 {
    override predName = "max_by"
}

[call_macro(name="_min_max_by")]
class private LinqMinMaxBy : AstCallMacro_LinqPred2 {
    override predName = "min_max_by"
}

[call_macro(name="_min_max_average_by")]
class private LinqMinMaxAverageBy : AstCallMacro_LinqPred2 {
    override predName = "min_max_average_by"
}

[call_macro(name="_skip_while")]
class private LinqSkipWhile : AstCallMacro_LinqPred2 {
    override predName = "skip_while"
}

[call_macro(name="_take_while")]
class private LinqTakeWhile : AstCallMacro_LinqPred2 {
    override predName = "take_while"
}

[call_macro(name="_all")]
class private LinqAll : AstCallMacro_LinqPred2 {
    override predName = "all"
}

[call_macro(name="_any")]
class private LinqAny : AstCallMacro_LinqPred2 {
    override predName = "any"
}

[call_macro(name="_distinct_by")]
class private LinqDistinctBy : AstCallMacro_LinqPred2 {
    override predName = "distinct_by"
}

[call_macro(name="_order_by")]
class private LinqOrderBy : AstCallMacro_LinqPred2 {
    override predName = "order_by"
}

[call_macro(name="_order_by_descending")]
class private LinqOrderByDescending : AstCallMacro_LinqPred2 {
    override predName = "order_by_descending"
}

class private AstCallMacro_LinqPredII2 : AstCallMacro {
    predName = "sequence_equal_by"
    //! This macro converts sequence_equal_by(i1, i2, expresion) into::
    //!     _sequence_equal_by(i1, i2, $(_) => expression)
    //!
    def override visit(prog : ProgramPtr; mod : Module?; var call : smart_ptr<ExprCallMacro>) : ExpressionPtr {
        // verifying arguments
        macro_verify(call.arguments |> length == 3, prog, call.at, "expecting {predName}(iterator, iterator, predicate)")
        macro_verify(call.arguments[0]._type != null, prog, call.at, "expecting iterator")
        macro_verify(call.arguments[0]._type.isIterator, prog, call.at, "expecting iterator")
        macro_verify(call.arguments[1]._type != null, prog, call.at, "expecting iterator")
        macro_verify(call.arguments[1]._type.isIterator, prog, call.at, "expecting iterator")
        macro_verify(!call.arguments[0]._type.firstType.isAutoOrAlias, prog, call.at, "iterator type cannot be auto or alias")
        // replacing function
        var inscope iterType <- clone_iterator_type(call.arguments[0]._type)
        var inscope res <- qmacro($c(predName)($e(call.arguments[0]), $e(call.arguments[1]), $(_ : $t(iterType)) => $e(call.arguments[2])))
        res.force_at(call.at)
        return <- res
    }
}

[call_macro(name="_sequence_equal_by")]
class private LinqPred3 : AstCallMacro_LinqPredII2 {
    override predName = "sequence_equal_by"
}

[call_macro(name="_except_by")]
class private LinqExceptBy : AstCallMacro_LinqPredII2 {
    override predName = "except_by"
}

[call_macro(name="_intersect_by")]
class private LinqIntersectBy : AstCallMacro_LinqPredII2 {
    override predName = "intersect_by"
}

[call_macro(name="_union_by")]
class private LinqUnionBy : AstCallMacro_LinqPredII2 {
    override predName = "union_by"
}

def private is_call_or_generic(var expr : Expression?, name, moduleName : string) : ExprCall? {
    if (expr is ExprCall) {
        var call = expr as ExprCall
        if (call.func.name == name && call.func._module.name == moduleName) {
            return call
        } elif (call.func.fromGeneric != null && call.func.fromGeneric.name == name && call.func.fromGeneric._module.name == moduleName) {
            return call
        }
    }
    return null
}

def private is_call_or_generic_arg0(var expr : Expression?, name, moduleName : string) : Expression? {
    //! returns null if not to_array, otherwise returns the argument of to_array
    var call = expr.is_call_or_generic(name, moduleName)
    return call.arguments[0].get_ptr() if (call != null)
    return null
}

def private fold_linq_cond(var expr : ExpressionPtr; argName : string) : ExpressionPtr {
    // if its a block with a single argument and a single return
    // we rename argument to specified value, and give back the return value
    // otherwise its an invoke
    if (expr is ExprMakeBlock) {
        var mblk = expr as ExprMakeBlock
        var blk = mblk._block as ExprBlock
        if (blk.arguments |> length == 1 && blk.list |> length == 1 && blk.list[0] is ExprReturn) {
            var ret = blk.list[0] as ExprReturn
            if (ret.subexpr != null) {
                var inscope res <- clone_expression(ret.subexpr)
                var inscope rules : Template
                rules |> renameVariable(string(blk.arguments[0].name), argName)
                apply_template(rules, res.at, res)
                return <- res
            }
        }
    }
    return <- qmacro(invoke($e(expr), $i(argName)))
}

struct private LinqCall {
    name : string
    moduleName : string = "linq"
    skip : bool = false             // if specified, expression is skipped in folding
    inplace : bool = false          // if specified, expression is inplace and does not create new variable
    recursive : array<int>          // indices of arguments to apply fold_linq_default on
}

var private linqCalls = {
// where
    "where_" => LinqCall(name = "where_"),
    "where_to_array" => LinqCall(name = "where_"),
    "where_to_sequence" => LinqCall(name = "where_"),
// select
    "select" => LinqCall(name = "select"),
    "select_to_array" => LinqCall(name = "select"),
    "select_to_sequence" => LinqCall(name = "select"),
// reverse
    "reverse" => LinqCall(name = "reverse", inplace = true),
    "reverse_to_array" => LinqCall(name = "reverse", inplace = true),
    "reverse_to_sequence" => LinqCall(name = "reverse", inplace = true),
// order
    "order" => LinqCall(name = "order", inplace = true),
    "order_to_array" => LinqCall(name = "order", inplace = true),
    "order_to_sequence" => LinqCall(name = "order", inplace = true),
// order_descending
    "order_descending" => LinqCall(name = "order_descending", inplace = true),
    "order_descending_to_array" => LinqCall(name = "order_descending", inplace = true),
    "order_descending_to_sequence" => LinqCall(name = "order_descending", inplace = true),
// order_by
    "order_by" => LinqCall(name = "order_by", inplace = true),
    "order_by_to_array" => LinqCall(name = "order_by", inplace = true),
    "order_by_to_sequence" => LinqCall(name = "order_by", inplace = true),
// order_by_descending
    "order_by_descending" => LinqCall(name = "order_by_descending", inplace = true),
    "order_by_descending_to_array" => LinqCall(name = "order_by_descending", inplace = true),
    "order_by_descending_to_sequence" => LinqCall(name = "order_by_descending", inplace = true),
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
    "skip_to_sequence" => LinqCall(name = "skip", inplace = true),
    "skip_while" => LinqCall(name = "skip_while"),
    "skip_while_to_array" => LinqCall(name = "skip_while"),
    "skip_while_to_sequence" => LinqCall(name = "skip_while"),
    "take" => LinqCall(name = "take", inplace = true),
    "take_to_array" => LinqCall(name = "take", inplace = true),
    "take_to_sequence" => LinqCall(name = "take", inplace = true),
    "take_while" => LinqCall(name = "take_while"),
    "take_while_to_array" => LinqCall(name = "take_while"),
    "take_while_to_sequence" => LinqCall(name = "take_while"),
// concatenation operations
    "concat" => LinqCall(name = "concat", recursive = [1], inplace = true),
    "concat_to_array" => LinqCall(name = "concat", recursive = [1], inplace = true),
    "concat_to_sequence" => LinqCall(name = "concat", recursive = [1], inplace = true),
    "prepend" => LinqCall(name = "prepend", inplace = true),
    "prepend_to_array" => LinqCall(name = "prepend", inplace = true),
    "prepend_to_sequence" => LinqCall(name = "prepend", inplace = true),
    "append" => LinqCall(name = "append", inplace = true),
    "append_to_array" => LinqCall(name = "append", inplace = true),
    "append_to_sequence" => LinqCall(name = "append", inplace = true),
// querying data
    "any" => LinqCall(name = "any"),
    "all" => LinqCall(name = "all"),
    "contains" => LinqCall(name = "contains"),
// skippables
    "to_array" => LinqCall(name = "to_array", moduleName = "$", skip = true),
    "to_sequence" => LinqCall(name = "to_sequence", skip = true),
}

[macro_function]
def private is_linq_call(var expr : Expression?) : bool {
    if (expr is ExprCall) {
        var call = expr as ExprCall
        var funcName = string(call.func.name)
        while (call.func.fromGeneric != null) {
            call.func = call.func.fromGeneric.get_ptr()
            funcName = string(call.func.name)
        }
        if (linqCalls.key_exists(funcName)) {
            let linqCall & = unsafe(linqCalls?[funcName] ?? default<LinqCall>)
            if (linqCall.moduleName == string(call.func._module.name)) {
                return true
            }
        }
    }
    return false
}

[macro_function]
def private flatten_linq(var expr : ExpressionPtr)  {
    var top = get_ptr(expr)
    var calls : array<tuple<ExprCall?; LinqCall const?>>
    while (top != null) {
        if (top is ExprCall) {
            var topCall = top as ExprCall
            var topFunc = topCall.func
            while (topFunc.fromGeneric != null) {
                topFunc = topFunc.fromGeneric.get_ptr()
            }
            let funcName = string(topFunc.name)
            if (!linqCalls.key_exists(funcName)) {
                break
            }
            let linqCall & = unsafe(linqCalls?[funcName] ?? default<LinqCall>)
            if (linqCall.moduleName != string(topFunc._module.name)) {
                break
            }
            if (!linqCall.skip) {
                calls.push((topCall, unsafe(addr(linqCall))))
            }
            top = topCall.arguments[0].get_ptr()
        } else {
            break
        }
    }
    calls.reverse_inplace()
    // ok, at this point top is source
    return <- (top, calls)
}

[macro_function]
def private fold_linq_default(var expr : ExpressionPtr) : ExpressionPtr {
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
    var inscope blk <- new ExprBlock(at = expr.at)
    var (top, calls) = flatten_linq(expr)
    if (calls |> length == 0) {
        return <- default<ExpressionPtr>
    }
    var inscope topExpr <- clone_expression(unsafe(reinterpret<ExpressionPtr>(top)))
    var inscope topExprType <- clone_type(top._type)
    var inscope topValue = qmacro(source)
    let lastCallIndex = calls |> length - 1
    for (cll, argIndex in calls, count()) {
        if (true) {
            let newArgName = "pass_{argIndex}"
            var callName = cll._1.name
            var inplace = false
            if (cll._0._type.isIterator || cll._0._type.isGoodArrayType) {
                if (argIndex == 0 && top._type.isIterator) {
                    callName = "{callName}_to_array"
                } elif (argIndex == lastCallIndex && expr._type.isIterator) {
                    callName = "{callName}_to_sequence"
                } elif (argIndex != 0 && cll._1.inplace) {
                    callName = "{callName}_inplace"
                    inplace = true
                }
            }
            for (i in cll._1.recursive) {   // this is where we make recursion
                if (i >= 1 && i < cll._0.arguments |> length) {
                    var inscope argExpr <- make_call(cll._0.at, "_fold")
                    (argExpr as ExprCallMacro).arguments |> emplace <| cll._0.arguments[i]
                    cll._0.arguments[i] |> move <| argExpr
                }
            }
            var inscope newCall = qmacro($c(callName)($e(topValue)))
            let numArgs = cll._0.arguments |> length
            for (i in 1..numArgs) {
                (newCall as ExprCall).arguments |> emplace_new <| clone_expression(cll._0.arguments[i])
            }
            if (inplace) {
                blk.list |> emplace(newCall)
            } else {
                blk.list |> emplace_new <| qmacro_expr() {
                    var $i(newArgName) = $e(newCall)
                }
                (blk.list.back() as ExprLet).variables[0].flags |= VariableFlags.can_shadow
                if (argIndex != 0) {
                    blk.list |> emplace_new <| qmacro_expr() {
                        delete $e(topValue)
                    }
                }
                topValue |> move_new <| qmacro($i(newArgName))
            }
        }
    }
    blk.list |> emplace_new <| qmacro_expr() {
        return <- $e(topValue)
    }
    topExpr.genFlags |= ExprGenFlags.alwaysSafe
    var inscope res <- qmacro(invoke($(source : $t(topExprType)) { $e(blk); }, $e(topExpr)))
    (((res as ExprInvoke).arguments[0] as ExprMakeBlock)._block as ExprBlock).arguments[0].flags |= VariableFlags.can_shadow
    res.force_generated(true)
    return <- res
}

[macro_function]
def private fold_linq_where_expr(var expr : ExpressionPtr) : ExpressionPtr {
    //! fold the following sequence
    //!   src.optional to_array_move().optional to_sequence().where(...).optional select().optional to_array()
    //! into comprehension. remove unnecessary to_array_move/to_sequence
    //! if to_array() is present, then output is array comprehension, otherwise iterator comprehension
    var inscope res : ExpressionPtr
    //! fold src.where(...).to_array()
    var top : Expression? = expr.get_ptr()
    // check for to_array(foo)
    var e_to_array = top.is_call_or_generic_arg0("to_array", "$")
    top = e_to_array != null ? e_to_array : top
    // check for select
    var e_select = top.is_call_or_generic("select", "linq")
    top = e_select != null ? e_select.arguments[0].get_ptr() : top
    // check for where(foo)
    var e_where = top.is_call_or_generic("where_", "linq")
    top = e_where != null ? e_where.arguments[0].get_ptr() : top
    // source is iterable
    return <- res if (top._type == null || !(top._type.isIterator || top._type.isGoodArrayType))
    return <- res if (is_linq_call(top))    // if  its just begining of the linq chain, bail out
    var inscope iterType <- clone_type(top._type.firstType)
    // now, let's check for to_sequence(foo), and skip
    var e_to_sequence = top.is_call_or_generic_arg0("to_sequence", "linq")
    top = e_to_sequence != null ? e_to_sequence : top
    // to_array_move(foo), and skip
    var e_to_array_move = top.is_call_or_generic_arg0("to_array_move", "$")
    top = e_to_array_move != null ? e_to_array_move : top
    // output is array or iterator
    var inscope top_e <- clone_expression(unsafe(reinterpret<ExpressionPtr>(top)))
    var inscope select_e : ExpressionPtr
    if (e_select != null) {
        select_e |> move_new <| fold_linq_cond(e_select.arguments[1], "it")
    } else {
        select_e |> move_new <| new ExprVar(name := "it", at = expr.at, _type := iterType)
    }
    // if to_array was present, then array comprehension, otherwise iterator comprehension
    var inscope comprehension : smart_ptr<ExprArrayComprehension>
    comprehension |> move_new <| qmacro([for (it in $e(top_e)); $e(select_e)])
    comprehension.force_at(expr.at)
    if (e_where != null) {
        comprehension.exprWhere |> move_new <| fold_linq_cond(e_where.arguments[1], "it")
    }
    let input_is_sequence = e_to_sequence != null || top._type.isIterator
    let output_is_array = e_to_array != null
    comprehension.generatorSyntax = input_is_sequence && !output_is_array
    return <- comprehension
}

[call_macro(name="_fold")]
class private LinqFold : AstCallMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : smart_ptr<ExprCallMacro>) : ExpressionPtr {
        macro_verify(call.arguments |> length == 1, prog, call.at, "expecting _fold(expression)")
        macro_verify(call.arguments[0]._type != null, prog, call.at, "expecting linq expression")
        var inscope res : ExpressionPtr <- fold_linq_where_expr(call.arguments[0])
        if (res == null) {
            res |> move_new <| fold_linq_default(call.arguments[0])
        }
        if (res == null) {
            prog |> macro_error(call.at, "cannot fold LINQ expression")
            return <- call
        }
        return <- res
    }
}



