options indenting = 4
options no_aot = true

module instance_function shared

require ast
require rtti
require daslib/ast_boost

class InstanceFunctionAnnotation : AstFunctionAnnotation
    def override apply ( var func:FunctionPtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        if args.length != 1
            errors := "expecting template name"
            return false
        let val = get_annotation_argument_value(args[0])
        if !(val is tBool)
            errors := "expecting generic name"
            return false
        var fn <- compiling_module() |> find_unique_generic("{args[0].name}")
        if fn == null
            errors := "generic not found {args[0].name}"
            return false
        func.body := clone_expression(fn.body)
        return true

[macro,private]
def init
    add_new_function_annotation("instance_function", new InstanceFunctionAnnotation())