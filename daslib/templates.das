options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true

module templates shared private

require ast
require rtti
require daslib/ast_boost

[call_macro(name="decltype")]
class DecltypeMacro : AstCallMacro
    def override visit ( prog:ProgramPtr; mod:Module?; var expr:smart_ptr<ExprCallMacro> ) : ExpressionPtr
        if expr.arguments.length !=1
            macro_error(prog,expr.at,"decltype expecting 1 argument")
            return [[ExpressionPtr]]
        if expr.arguments[0]._type==null || expr.arguments[0]._type.isAutoOrAlias
            macro_error(prog,expr.at,"decltype requires fully inferred type")
            return [[ExpressionPtr]]
        var mkt <- new [[ExprTypeDecl() at=expr.at,
            typeexpr <- clone_type(expr.arguments[0]._type)
        ]]
        return <- mkt

class TemplateMacro : AstFunctionAnnotation
    def override transform ( var expr : smart_ptr<ExprCallFunc>; var errors : das_string ) : ExpressionPtr
        if expr.func==null
            compiling_program() |> macro_error(expr.at,"expecting inferred function")
            return expr
        let taidx = find_index_if(each(expr.func.annotations)) <| $ ( ann )
            return ann.annotation.name == "template"
        if taidx == -1
            return expr
        var cfunc = expr.func
        var ffunc : FunctionPtr
        unsafe
            ffunc <- reinterpret<FunctionPtr> cfunc
        // new call
        var newcallexpr <- clone_expression(expr)
        var newcall : smart_ptr<ExprCallFunc>
        unsafe
            newcall <- reinterpret<smart_ptr<ExprCallFunc>> newcallexpr
        // new func
        var fclone <- clone_function(ffunc)
        fclone.fromGeneric = expr.func
        fclone.flags |= FunctionFlags privateFunction
        newcall.func = null
        var extra = ""
        for t in fclone.annotations[taidx].arguments
            var argidx = find_index_if(each(fclone.arguments)) <| $ ( farg )
                return farg.name == t.name
            if argidx==-1
                errors := "can't find template argument {t.name} (or duplicate)"
                fclone := [[FunctionPtr]]
                newcall := [[smart_ptr<ExprCallFunc>]]
                return [[ExpressionPtr]]
            var argExpr := newcall.arguments[argidx]
            if (argExpr is ExprRef2Value)
                let r2v = newcall.arguments[argidx] as ExprRef2Value
                argExpr := r2v.subexpr
            let canStrip = t.basicType==Type tBool ? t.bValue : true
            if canStrip && !(argExpr is ExprTypeDecl)
                errors := "template argument must be type<...> and not {describe(newcall.arguments[argidx])}. {newcall.arguments[argidx].__rtti}"
                fclone := [[FunctionPtr]]
                newcall := [[smart_ptr<ExprCallFunc>]]
                return [[ExpressionPtr]]
            var tname = describe_typedecl(argExpr._type,true,true,true)
            argExpr := null
            extra += "`{hash(tname)}"
            fclone.arguments |> erase(argidx)
            newcall.arguments |> erase(argidx)
        fclone.annotations |> erase(taidx)
        // name it
        fclone.name := "`template{extra}`{fclone.name}"
        newcall.name := "_::{fclone.name}"
        // result
        compiling_module() |> add_function(fclone)
        return newcall

[macro]
def init
    if is_compiling_macros_in_module("templates")
        add_new_function_annotation("template", new TemplateMacro())
