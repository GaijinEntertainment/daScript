options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module coroutines shared private

require daslib/ast_boost
require daslib/templates_boost
require daslib/macro_boost

[call_macro(name="yeild_from")]
class YieldFrom : AstCallMacro
    //! This macro converts yield_from(THAT) expression
    //! into for t in THAT { yield t; }
    def override visit ( prog:ProgramPtr; mod:Module?; var call:smart_ptr<ExprCallMacro> ) : ExpressionPtr
        macro_verify( call.arguments.length==1,prog,call.at,"expecting yeild_from(iterator)" )
        macro_verify( call.arguments[0]._type.isIterator,prog,call.at,"expecting iterator" )
        let iname = make_unique_private_name("_yield_from_iterator",call.at)
        return qmacro_block <|
            for $i(iname) in $e(call.arguments[0])
                yield $i(iname)

