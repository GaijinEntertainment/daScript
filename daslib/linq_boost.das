options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module linq_boost shared public

require daslib/linq public

require daslib/ast_boost
require daslib/templates_boost
require daslib/macro_boost


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
        // replacing function
        return <- qmacro($c(predName)($e(call.arguments[0]), $(_) => $e(call.arguments[1])))
    }
}

[call_macro(name="_where")]
class private LinqPred2 : AstCallMacro_LinqPred2 {
    override predName = "where_"
}
