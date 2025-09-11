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
        macro_verify(call.arguments[0]._type != null, prog, call.at, "expecting iterator")
        macro_verify(call.arguments[0]._type.isIterator, prog, call.at, "expecting iterator")
        macro_verify(!call.arguments[0]._type.firstType.isAutoOrAlias, prog, call.at, "iterator type cannot be auto or alias")
        // replacing function
        var inscope iterType <- clone_iterator_type(call.arguments[0]._type)
        var inscope res <- qmacro($c(predName)($e(call.arguments[0]), $(_ : $t(iterType)) => $e(call.arguments[1])))
        res.force_at(call.at)
        return <- res
    }
}

[call_macro(name="_where")]
class private LinqWhere : AstCallMacro_LinqPred2 {
    override predName = "where_"
}

[call_macro(name="_select")]
class private LinqSelect : AstCallMacro_LinqPred2 {
    override predName = "select"
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
