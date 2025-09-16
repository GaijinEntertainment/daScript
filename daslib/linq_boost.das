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
    - _max_by(it, expr)         => max_by(it, $(_) => expr
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
    return <- res if (top._type == null || !top._type.isIterator)
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
    if (e_where != null) {
        comprehension.exprWhere |> move_new <| fold_linq_cond(e_where.arguments[1], "it")
    }
    comprehension.generatorSyntax = e_to_array == null
    comprehension.force_at(expr.at)
    return <- comprehension
}

[call_macro(name="_fold")]
class private LinqFold : AstCallMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : smart_ptr<ExprCallMacro>) : ExpressionPtr {
        macro_verify(call.arguments |> length == 1, prog, call.at, "expecting _fold(expression)")
        macro_verify(call.arguments[0]._type != null, prog, call.at, "expecting linq expression")
        var inscope res <- fold_linq_where_expr(call.arguments[0])
        return <- (call.arguments[0]) if (res == null)  // could not fold 'where', means we let it just fall in place
        res.force_at(call.at)
        return <- res
    }
}



