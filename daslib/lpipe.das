options indenting = 4
options no_aot = true

module lpipe shared

require ast
require rtti
require daslib/ast_boost
require daslib/templates_boost

[private]
def lpipe_expr ( var fnCall:ExpressionPtr; var arg:ExpressionPtr ) : ExpressionPtr
    if fnCall |> is_expr_like_call()
        unsafe
            var pCall = reinterpret<ExprLooksLikeCall?> fnCall
            if string(pCall.name)=="lpipe"  // not piping pipe into pipe
                return [[ExpressionPtr]]
            pCall.arguments |> emplace(arg)
        return fnCall
    elif fnCall is ExprVar
        unsafe
            var pVar = reinterpret<ExprVar?> fnCall
            var pCall <- make_call(pVar.at, "{pVar.name}")
            delete fnCall
            var pCallFunc = reinterpret<ExprLooksLikeCall?> pCall
            pCallFunc.arguments |> emplace(arg)
            return pCall
    else
        unsafe
            delete arg
        return [[ExpressionPtr]]


[call_macro(name="lpipe")]  // apply(expr)
class LpipeMacro : AstCallMacro
    def override visit ( prog:ProgramPtr; mod:Module?; var call:smart_ptr<ExprCallMacro> ) : ExpressionPtr
        if call.arguments.length != 1
            macro_error(prog,call.at,"expecting lpipe(value)")
            return [[ExpressionPtr]]
        var success = false
        compiling_program() |> get_ast_context(call) <| $(valid, astc)
            if !valid
                macro_error(prog,call.at,"can't get valid program context")
                return
            if astc.scopes.length<=0
                macro_error(prog,call.at,"missing scope")
                return
            unsafe
                var blk = reinterpret<ExprBlock?> astc.scopes[astc.scopes.length-1]
                var eidx = -1
                for i,expr in range(100500),blk.list
                    if expr==call
                        eidx = i
                        break
                if eidx==-1
                    macro_error(prog,call.at,"can't find lpipe in the block")
                    return
                if eidx==0
                    macro_error(prog,call.at,"can't pipe first expression in the block")
                    return
                let pidx = eidx - 1
                var pipe <- lpipe_expr(blk.list[pidx], clone_expression(call.arguments[0]) )
                if pipe==null
                    macro_error(prog,call.at,"can only pipe to a function call")
                    return
                blk.list[pidx] <- pipe
                blk.list[eidx] := null
            success = true
        if !success
            return [[ExpressionPtr]]
        return quote <|
            pass
