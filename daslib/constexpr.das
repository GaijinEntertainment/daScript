options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true

module constexpr shared private

require ast
require daslib/ast_boost
require daslib/strings_boost

[function_macro(name=constexpr)]
class ConstExprAnnotation : AstFunctionAnnotation
    //! This macro implements a constexpr function argument checker. Given list of arguments to verify, it will fail for every one where non-constant expression is passed. For example::
    //!
    //!     [constexpr (a)]
    //!     def foo ( t:string; a : int )
    //!         print("{t} = {a}\n")
    //!     var BOO = 13
    //!     [export]
    //!     def main
    //!         foo("blah", 1)
    //!         foo("ouch", BOO)    // comilation error: `a is not a constexpr, BOO`
    def override verifyCall ( var call : smart_ptr<ExprCallFunc>; args,progArgs:AnnotationArgumentList; var errors : das_string ) : bool
        for aa in args
            if aa.basicType == Type tBool
                for ca,ce in call.func.arguments,call.arguments
                    if ca.name==aa.name
                        if !ce.__rtti |> starts_with("ExprConst")
                            errors := "{aa.name} is not a constexpr, {describe(ce)}"
                            return false
        return true
