options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true

module debug_macro shared private

require ast
require rtti
require daslib/ast_boost
require daslib/templates_boost

[tag_function(debug_function)]
def public debug_print ( message:string="" )
    print(message)

[tag_function_macro(tag="debug_function")]
class TestDebugMacro : AstFunctionAnnotation
    def override transform ( var call : smart_ptr<ExprCallFunc>; var errors : das_string ) : ExpressionPtr
        var qb <- qmacro_expr() <|
            print($e(call.arguments[0]))
        return qb


