options indenting = 4
options no_aot = true

module instance_function shared

require ast
require rtti
require daslib/ast_boost
require daslib/templates_boost
require daslib/defer
require strings

// [instance_function(generic_name,type1=type1r,type2=type2r,...)]
class InstanceFunctionAnnotation : AstFunctionAnnotation
    def override apply ( var func:FunctionPtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        var generic_name = ""
        var rules : Template
        defer_delete(rules)
        for argv in args
            let val = get_annotation_argument_value(argv)
            if val is tBool
                if !empty(generic_name)
                    errors := "unexpected {argv.name}, name of the generic or function is already specified as {generic_name}"
                    return false
                generic_name = "{argv.name}"
            elif val is tString
                rules |> replaceType("{argv.name}", "{val as tString}")
            else
                errors := "invalid argument type {argv.name}"
                return false
        if empty(generic_name)
            errors := "expecting name of the generic or function"
            return false
        var fn <- compiling_module() |> find_unique_generic(generic_name)
        if fn == null
            errors := "generic not found {generic_name}"
            return false
        func.body := null
        unsafe
            func.body <- clone_expression(fn.body)
        apply_template(rules, func.at, func.body)
        return true

[macro,private]
def init
    add_new_function_annotation("instance_function", new InstanceFunctionAnnotation())