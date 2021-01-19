options indenting = 4
options no_aot = true

module assert_once shared

require ast
require rtti
require daslib/ast_boost
require daslib/template_boost

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
    def override transform ( var call : smart_ptr<ExprCallFunc>; var errors : das_string ) : ExpressionPtr
        var once_name = make_unique_private_name("__assert_once",call.at)
        compiling_module() |> add_global_private_var(once_name, call.at) <| quote(true)
        // if ..
        var qblock <- quote() <|
            if __assert_once_I && !expr
                __assert_once_I = false
                assert(false, message)
        // rename
        var rules : Template
        rules |> renameVariable("__assert_once_I") <| once_name
        rules |> replaceVariable("expr") <| clone_expression(call.arguments[0])
        rules |> replaceVariable("message") <| clone_expression(call.arguments[1])
        apply_template(rules, call.at, qblock)
        delete rules
        return move_unquote_block(qblock)
