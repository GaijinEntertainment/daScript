options indenting = 4
options no_aot = true

module trivial_value

require ast
require rtti
require daslib/ast_boost
require daslib/template_boost

[tag_function(trivial_value_tag)]
def trivial_value ( val ) {}

[tag_function_macro(tag="trivial_value_tag")]
class AssertOnceMacro : AstFunctionAnnotation
    def override transform ( var call : smart_ptr<ExprCallFunc>; var errors : das_string ) : ExpressionPtr
        // check if argument is constant value (not expression). i.e. "blah" or 1234
        if !(call.arguments[0] is ExprConstString) && !(call.arguments[0] is ExprConstInt)
            compiling_program() |> macro_error(call.at,"expecting constant value")
            return [[ExpressionPtr]]
        // build a block which we are planning to return
        var qblock <- quote() <| @@ (effects : int) // effects : ArmyEffects
            return <- [{ auto[] val => 1.0 }]
        // replace ExprVar("val") with the first argument
        var rules : Template
        rules |> replaceVariable("val") <| clone_expression(call.arguments[0])
        apply_template(rules, call.at, qblock)
        delete rules
        // return said block
        return qblock
