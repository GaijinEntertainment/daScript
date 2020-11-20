options indenting = 4
options no_aot = true

module safe_addr

require ast
require rtti
require daslib/ast_boost
require daslib/is_local

[tag_function(safe_addr_tag)]
def safe_addr(var x : auto(T)&) : T-& ? #
    unsafe
        return reinterpret<T-&?#>(addr(x))

[tag_function_macro(tag="safe_addr_tag")]
class SafeAddrMacro : AstFunctionAnnotation
    def override transform ( var call : smart_ptr<ExprCallFunc>; var errors : das_string ) : ExpressionPtr
        if !is_local_or_global_expr(ExpressionPtr(call.arguments[0]))
            compiling_program() |> macro_error(call.at,"not a local value")
        return [[ExpressionPtr]]
