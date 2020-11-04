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
        var vini <- new [[ExprConstBool() at=call.at, value=true]]
        var vvar <- new [[Variable() at=call.at, name:=once_name, init<-vini]]
        vvar._type <- new [[TypeDecl() at=call.at, baseType=Type tBool]]
        if !(compiling_module() |> add_variable(vvar))
            panic("can't setup")
        // assert (false, message)
        var ass <- new [[ExprAssert() at=call.at, name:="assert"]]
        push(ass.arguments,new [[ExprConstBool() at=call.at, value=false]])
        if call.arguments.length==2
            var msg <- clone_expression(call.arguments[1])
            push(ass.arguments,msg)
        // __assert_once_I = false
        var cvar <- new [[ExprVar() at=call.at, name:=once_name]]
        var cfalse <- new [[ExprConstBool() at=call.at, value=false]]
        var seto <- new [[ExprCopy() at=call.at, left<-cvar, right<-cfalse]]
        // block
        var blk <- new [[ExprBlock() at=call.at]]
        push(blk.list, seto)
        push(blk.list, ass)
        // __assert_once_I && !expr
        var evar <- new [[ExprVar() at=call.at, name:=once_name]]
        var expr_clone <- clone_expression(call.arguments[0])
        var enot <- new [[ExprOp1() at=call.at, op:="!", subexpr<-expr_clone]]
        var land <- new [[ExprOp2() at=call.at, op:="&&", left <-evar, right<-enot ]]
        // if
        var ife <- new [[ExprIfThenElse() at=call.at, cond<-land, if_true<-blk]]
        return <- ife
