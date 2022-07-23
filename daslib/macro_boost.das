options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true

module macro_boost shared private

require daslib/ast_boost
require daslib/templates_boost

[tag_function(macro_verify_tag)]
def public macro_verify ( expr:bool; prog:ProgramPtr; at:LineInfo; message:string )
    //! Same as verify, only the check will produce macro error, followed by return [[ExpressionPtr]]
    pass

[tag_function_macro(tag="macro_verify_tag")]
class MacroVerifyMacro : AstFunctionAnnotation
    //! This macro convert macro_verify(expr,message,prog,at) to the following code::
    //!    if !expr
    //!        macro_error(prog,at,message)
    //!        return [[ExpressionPtr]]
    def override transform ( var call : smart_ptr<ExprCallFunc>; var errors : das_string ) : ExpressionPtr
        // if ..
        return <- qmacro_block() <|
            if !$e(call.arguments[0])
                macro_error($e(call.arguments[1]),$e(call.arguments[2]),$e(call.arguments[3]))
                return [[ExpressionPtr]]


