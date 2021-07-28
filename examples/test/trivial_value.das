options indenting = 4
options no_aot = true

module trivial_value

require ast
require rtti
require daslib/ast_boost
require daslib/templates_boost

// macro, which replaces trivial_value(VAL)
// with:
//  @@ (effects : int) // effects : ArmyEffects
//      return <- [{ auto[] val => 1.0 }]
[call_macro(name="trivial_value")]
class TrivialValueMacro : AstCallMacro
    def override visit ( prog:ProgramPtr; mod:Module?; var call:smart_ptr<ExprCallMacro> ) : ExpressionPtr
        // check if we  get correct number of arguments
        if call.arguments.length!=1
            prog |> macro_error(call.at,"expecting trivalValue(constant_value)")
            return [[ExpressionPtr]]
        // check if argument is constant value (not expression). i.e. "blah" or 1234
        if !(call.arguments[0] is ExprConstString) && !(call.arguments[0] is ExprConstInt)
            prog |> macro_error(call.at,"expecting constant value, got {call.arguments[0].__rtti}")
            return [[ExpressionPtr]]
        // build a block which we are planning to return
        var qblock <- quote() <| @@ (effects : int) // effects : ArmyEffects
            return <- [{ auto[] val => 1.0 }]
        // replace ExprVar("val") with the first argument
        apply_template(call.at, qblock) <| $ ( rules )
            rules |> replaceVariable("val") <| clone_expression(call.arguments[0])
        // return said block
        return qblock
