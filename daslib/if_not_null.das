options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true

module if_not_null shared private

require ast
require rtti
require daslib/ast_boost
require daslib/templates_boost

[call_macro(name="if_not_null")]
class ApplyMacro : AstCallMacro
    //! This macro transforms::
    //!
    //!     ptr |> if_not_null <| call(...)
    //!
    //! to::
    //!
    //!     var _ptr_var = ptr
    //!     if _ptr_var
    //!         call(*_ptr_var,...)
    //!
    if_not_null_index : int = 0
    def override visit ( prog:ProgramPtr; mod:Module?; var expr:smart_ptr<ExprCallMacro> ) : ExpressionPtr
        if expr.arguments.length !=2
            macro_error(prog,expr.at,"expecting 2 arguments, expecting ptr |> if_not_null() <| call()")
            return [[ExpressionPtr]]
        if !(expr.arguments[1] is ExprCall)
            macro_error(prog,expr.at,"2nd argument must be call(), expecting ptr |> if_not_null() <| call()")
            return [[ExpressionPtr]]
        if expr.arguments[0]._type!=null    // need value inferred
            if !expr.arguments[0]._type.isPointer
                macro_error(prog,expr.at,"first argument must be pointer , expecting ptr |> if_not_null() <| call()")
                return [[ExpressionPtr]]
            let ifnn_name = "__ifnn_ptr_{if_not_null_index++}"
            // var vptr = ptr
            var vlet <- new [[ExprLet() at=expr.at,atInit=expr.at]]
            vlet.variables |> emplace_new() <| new [[Variable() at=expr.at,
                    name := ifnn_name,
                    _type <- new [[TypeDecl() baseType=Type autoinfer, at=expr.at]],
                    init <- clone_expression(expr.arguments[0])
                ]]
            if !expr.arguments[0]._type.flags.constant
                vlet.variables[0]._type.flags |= TypeDeclFlags removeConstant
            // { somecall(ptr,...) }
            var eblk <- new [[ExprBlock() at=expr.at]]
            var fcall = expr.arguments[1] as ExprCall
            // note: this can be clone+push(ecall.arguments,arg,1) but we are not bining 'push at' yet
            var ecall <- new [[ExprCall() at=fcall.at, name:=fcall.name]]
            ecall.arguments |> emplace_new() <| new [[ExprPtr2Ref() at=expr.at,
                    subexpr <- new [[ExprVar() at=expr.at,name:=ifnn_name]]
                ]]
            for arg in fcall.arguments
                ecall.arguments |> emplace_new() <| clone_expression(arg)
            eblk.list |> emplace(ecall)
            // if ptr != null
            var eifnn <- new [[ExprIfThenElse() at=expr.at,
                cond <- new [[ExprOp2() at=expr.at,
                    op := "!=",
                    left <- new [[ExprVar() at=expr.at,name:=ifnn_name]],
                    right <- new [[ExprConstPtr() at=expr.at]]
                ]],
                if_true <- eblk
            ]]
            // tblk
            var tblk <- new [[ExprBlock() at=expr.at]]
            tblk.list |> emplace(vlet)
            tblk.list |> emplace(eifnn)
            return tblk
        return [[ExpressionPtr]]

