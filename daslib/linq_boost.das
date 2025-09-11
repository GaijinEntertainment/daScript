options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module linq_boost shared public

require daslib/linq public

require daslib/ast_boost
require daslib/templates_boost
require daslib/macro_boost


def clone_iterator_type(it : TypeDeclPtr) : TypeDeclPtr {
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
