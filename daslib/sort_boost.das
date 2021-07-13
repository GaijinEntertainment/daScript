options indenting = 4
options remove_unused_symbols = false

module sort_boost shared

require ast

require daslib/templates public
require daslib/contracts public
require daslib/ast_boost

[private, unsafe_operation, template (a), unused_argument(a)]
def sort_array_helper ( var arr : auto implicit; a : auto(TT) ) : array<TT -#>
    var res : array<TT -#>
    let lenA = length(arr)
    if lenA>=1
        unsafe
            _builtin_make_temp_array(res, addr(arr[0]), lenA)
    return <- res

[unsafe_operation]
def temp_sort_array ( var arr : auto implicit )
    unsafe
        return <- sort_array_helper ( arr, decltype(arr[0]) )

[call_macro(name="qsort")]
class QsortMacro : AstCallMacro
    def override visit ( prog:ProgramPtr; mod:Module?; var expr:smart_ptr<ExprCallMacro> ) : ExpressionPtr
        if expr.arguments.length != 2
            macro_error(prog,expr.at,"expecting qsort(value, block)")
            return [[ExpressionPtr]]
        if expr.arguments[0]._type==null || expr.arguments[1]._type==null
            return [[ExpressionPtr]]
        // todo: verify correct block type?
        if expr.arguments[1]._type.baseType != Type tBlock
            macro_error(prog,expr.at,"expecting 2nd argument to be block(<a,b:auto(TT)>:bool))")
            return [[ExpressionPtr]]
        if expr.arguments[0]._type.dim.length != 0 || expr.arguments[0]._type.baseType==Type tArray
            var call <- new [[ExprCall() name:="sort", at=expr.at]]
            emplace_new(call.arguments) <| clone_expression(expr.arguments[0])
            emplace_new(call.arguments) <| clone_expression(expr.arguments[1])
            return <- call
        elif expr.arguments[0]._type.baseType==Type tHandle
            var call <- new [[ExprCall() name:="sort", at=expr.at]]
            var tsa <- new [[ExprCall() name:="temp_sort_array", at=expr.at]]
            emplace_new(tsa.arguments) <| clone_expression(expr.arguments[0])
            tsa.genFlags |= ExprGenFlags alwaysSafe
            emplace(call.arguments,tsa)
            emplace_new(call.arguments) <| clone_expression(expr.arguments[1])
            return <- call
        else
            macro_error(prog,expr.at,"can only qsort [], array, or handled vector")
            return [[ExpressionPtr]]

