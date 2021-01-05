options indenting = 4
options no_aot = true

module jobque_boost shared

require jobque

require daslib/rtti
require daslib/ast
require daslib/ast_boost

[tag_function(pfork_tag)]
def pfork ( var l : lambda )
    invoke(l)   // note, this is never called if job-que is there

[tag_function_macro(tag="pfork_tag")]
class PforkMacro : AstFunctionAnnotation
    once_index : int = 0
    def override transform ( var call : smart_ptr<ExprCallFunc>; var errors : das_string ) : ExpressionPtr
        unsafe
            if !call.arguments[0] is ExprAscend
                compiling_program() |>macro_error(call.at,"expecting lambda declaration, ExprAscend")
                return [[ExpressionPtr]]
            let asc = reinterpret<ExprAscend?> call.arguments[0]
            if !asc.subexpr is ExprMakeStruct
                compiling_program() |>macro_error(call.at,"expecting lambda declaration, ExprMakeStruct")
                return [[ExpressionPtr]]
            let mks = reinterpret<ExprMakeStruct?> asc.subexpr
            if mks._type==null || mks._type.baseType!=Type tStructure
                compiling_program() |>macro_error(call.at,"expecting lambda declaration, not a structure")
                return [[ExpressionPtr]]
            var stype = mks._type.structType
            // make fields non-constant, so that they can be cloned
            for fld in stype.fields
                fld._type.flags &= ~(TypeDeclFlags constant)
                fld.flags &= ~(FieldDeclarationFlags capturedConstant)
            var pclone <- make_clone_structure(stype)
            compiling_module() |> add_function(pclone)
            // make an @@<var L;L> clone
            var ftype <- new [[TypeDecl() at=call.at, baseType=Type tFunction ]]
            ftype.firstType <- new [[TypeDecl() at=call.at, baseType=Type tVoid]]
            ftype.argTypes |> emplace_new <| clone_type(mks._type)
            ftype.argTypes |> emplace_new <| clone_type(mks._type)
            ftype.argTypes[0].flags &= ~(TypeDeclFlags constant)
            ftype.argTypes[1].flags |= TypeDeclFlags constant
            var cadd <- new [[ExprAddr() at=call.at, target:="clone", funcType <- ftype]]
            // make a pfork_invoke call
            var ncall <- new [[ExprCall() at=call.at, name:="pfork_invoke"]]
            ncall.arguments |> emplace_new <| clone_expression(call.arguments[0])
            ncall.arguments |> emplace <| cadd
            ncall.arguments |> emplace_new <| new [[ExprConstInt() at=call.at, value=int(mks._type.sizeOf)]]
            return <- ncall
