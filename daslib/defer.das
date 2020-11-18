options indenting = 4
options no_aot = true

module defer

require ast
require rtti
require daslib/ast_boost
require daslib/template_boost

[tag_function(defer_tag)]
def defer ( blk : block<():void> ) {}

def nada() {}

// covnert defer() <| block expression
// into {}, and move block to the finally section of the current block
[tag_function_macro(tag="defer_tag")]
class DeferMacro : AstFunctionAnnotation
    def override transform ( var call : smart_ptr<ExprCallFunc>; var errors : das_string ) : ExpressionPtr
        // context
        var success = true
        compiling_program() |> get_ast_context(call) <| $(valid, astc)
            if !valid
                compiling_program() |>macro_error(call.at,"can't get valid program context")
                success = false
            elif astc.scopes.length == 0
                compiling_program() |>macro_error(call.at,"defer needs to be in the scope")
                success = false
            else
                var scope : ExprBlock?
                unsafe
                    scope = reinterpret<ExprBlock?>(astc.scopes[astc.scopes.length-1])
                var c_arg <- clone_expression(call.arguments[0])
                var from_block <- move_unquote_block(c_arg)
                from_block.blockFlags ^= ExprBlockFlags isClosure
                scope.finalList |> emplace(from_block,0)
        return !success ? [[ExpressionPtr]] : quote() <| nada()
