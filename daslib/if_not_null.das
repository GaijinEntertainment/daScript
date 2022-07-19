options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true

module if_not_null shared private

require ast
require rtti
require daslib/ast_boost
require daslib/templates_boost
require daslib/defer

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
        assume PTR = expr.arguments[0]
        if PTR._type!=null    // need value inferred
            if !PTR._type.isPointer
                macro_error(prog,expr.at,"first argument must be pointer , expecting ptr |> if_not_null() <| call()")
                return [[ExpressionPtr]]
            let ifnn_name = "__ifnn_ptr_{if_not_null_index++}"
            var fcall = expr.arguments[1] as ExprCall
            var vtype <- clone_type(PTR._type); defer_delete(vtype)
            if !PTR._type.flags.constant
                vtype.flags |= TypeDeclFlags removeConstant
            var tblk <- qmacro_block <|
                let $i(ifnn_name) : $t(vtype) = $e(PTR)
                if $i(ifnn_name)!=null
                    $c(fcall.name)(*$i(ifnn_name),$a(fcall.arguments))
            return tblk
        return [[ExpressionPtr]]

