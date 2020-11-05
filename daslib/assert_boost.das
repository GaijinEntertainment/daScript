options indenting = 4
options no_aot = true

module assert_boost

require ast
require rtti
require daslib/ast_boost
require daslib/template

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
        //  make new global variable _asset_once_I = true
        var vvar <- new [[Variable() at=call.at,
            name:=once_name,
            _type <- new [[TypeDecl() at=call.at, baseType=Type tBool]],
            init <- clone_expression() <| quote(true)
        ]]
        if !(compiling_module() |> add_variable(vvar))
            panic("can't add variable {once_name}")
        // if ..
        var qblock <- quote() <|
            if __assert_once_I && !expr
                __assert_once_I = false
                assert(false, message)
        // rename
        var rules = new Template()
        rules->renameVariable("__assert_once_I") <| once_name
        rules->replaceVariable("expr") <| clone_expression(call.arguments[0])
        rules->replaceVariable("message") <| clone_expression(call.arguments[1])
        var res_expr <- apply_template(rules, qblock)
        // return 1-st element from the block (i.e if)
        var res <- clone_expression(unquote_block(res_expr).list[0])
        unsafe
            delete res_expr
        return <- res

