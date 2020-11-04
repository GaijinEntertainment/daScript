options indenting = 4
options no_aot = true

module assert_boost

require ast
require rtti
require daslib/ast_boost

[tag_function(asset_once_tag)]
def assert_once ( expr:bool; message:string="" ) {}

// convert assert_once(expr,message) to
//  var
//      __assert_once_I = true
//  if __assert_once_I && !expr
//      __assert_once_I = false
//      assert(false,message)
[tag_function_macro(tag="asset_once_tag")]
class AssertOnceMacro : AstFunctionAnnotation
    once_index : int = 0
    def override transform ( var call : smart_ptr<ExprCallFunc>; var errors : das_string ) : ExpressionPtr
        var once_name = "__assert_once_{once_index++}"
        // let __asset_once_I = true
        var vvar <- new [[Variable() at=call.at,
            name:=once_name,
            _type <- new [[TypeDecl() at=call.at, baseType=Type tBool]],
            init <- new [[ExprConstBool() at=call.at, value=true]]
        ]]
        if !(compiling_module() |> add_variable(vvar))
            panic("can't setup")
        // assert (false, message)
        var ass <- new [[ExprAssert() at=call.at, name:="assert"]]
        ass.arguments |> push(new [[ExprConstBool() at=call.at, value=false]])
        if call.arguments.length==2
            var msg <- clone_expression(call.arguments[1])
            ass.arguments |> push(msg)
        // __assert_once_I = false
        var seto <- new [[ExprCopy() at=call.at,
            left <- new [[ExprVar() at=call.at, name:=once_name]],
            right <- new [[ExprConstBool() at=call.at, value=false]]
        ]]
        // block
        var blk <- new [[ExprBlock() at=call.at]]
        blk.list |> push(seto)
        blk.list |> push(ass)
        // if __assert_once_I && !expr
        return <- new [[ExprIfThenElse() at=call.at,
            cond <- new [[ExprOp2() at=call.at,
                op:="&&",
                left <- new [[ExprVar() at=call.at, name:=once_name]],
                right <- new [[ExprOp1() at=call.at,
                    op:="!",
                    subexpr <- clone_expression(call.arguments[0])
                ]]
            ]],
            if_true <- blk
        ]]
