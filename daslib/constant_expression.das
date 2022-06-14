options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true

module constant_expression shared private

require daslib/ast_boost
require daslib/algorithm
require daslib/strings_boost
require daslib/templates_boost

def public isConstantExpression ( expr:ExpressionPtr ) : bool
    if expr is ExprRef2Value
        return isConstantExpression( (expr as ExprRef2Value).subexpr )
    else
        return expr.__rtti |> starts_with("ExprConst")

[function_macro(name="constant_expression")]
class ConstantExpressionMacro : AstFunctionAnnotation
    def override transform ( var expr : smart_ptr<ExprCallFunc>; var errors : das_string ) : ExpressionPtr
        if expr.func==null
            compiling_program() |> macro_error(expr.at,"expecting inferred function")
            return [[ExpressionPtr]]
        var argi : array<int>
        var anni = -1
        for ann,idd in expr.func.annotations,range(100500)
            if ann.annotation.name=="constant_expression"
                anni = idd
                for arg in ann.arguments
                    if arg.basicType==Type tBool
                        for farg,fidx in expr.func.arguments,range(100500)
                            if farg.name==arg.name
                                if !isConstantExpression(expr.arguments[fidx])
                                    if arg.bValue
                                        compiling_program() |> macro_error(expr.at,"function {expr.func.name} argument {arg.name} is not a compilation time constant")
                                    return [[ExpressionPtr]]
                                argi |> push(fidx)
                    else
                        compiling_program() |> macro_error(expr.at,"unsupported argument type {arg.basicType}")
                        return [[ExpressionPtr]]
                break
        argi |> sort_unique
        argi |> reverse
        if empty(argi)
            return [[ExpressionPtr]]
        var func_copy <- clone_function(FunctionPtr(expr.func))
        var func_name = "{func_copy.name}`constant_expression"
        for i in argi
            let fhash = hash(describe(expr.arguments[i]))
            func_name += "`{fhash}"
        func_copy.name := func_name
        if expr.func.fromGeneric != null
            func_copy.body := null
            func_copy.result := null
            unsafe
                func_copy.body <- clone_expression(expr.func.fromGeneric.body)
                func_copy.result <- clone_type(expr.func.fromGeneric.result)

        apply_template(func_copy.body) <| $ ( rules )
            for i in argi
                rules |> replaceVariable(string(expr.func.arguments[i].name)) <| clone_expression(expr.arguments[i])
        for i in argi
            func_copy.arguments |> erase(i)
        // func_copy.flags |= FunctionFlags exports
        func_copy.flags |= FunctionFlags generated
        func_copy.annotations |> erase(anni)
        compiling_module() |> add_function(func_copy)
        var new_call <- clone_expression(expr)
        var ncall = new_call as ExprCall
        ncall.name := func_name
        for i in argi
            ncall.arguments |> erase(i)
        return <- new_call

