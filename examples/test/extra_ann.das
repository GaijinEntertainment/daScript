options indenting = 4
options no_aot = true

module extra_ann shared

require ast
require rtti
require daslib/ast_boost

[generic]
def find_compiling_module(name:string) : Module?
    var rm : Module?
    program_for_each_module(compiling_program()) <| $ ( mod )
        if string(mod.name)==name
            rm = mod
    verify(rm!=null,"module not found")
    return rm

def find_annotation ( mod_name,ann_name:string ) : Annotation const?
    var mod = find_compiling_module(mod_name)
    if mod==null
        return null
    var ann : Annotation const?
    module_for_each_annotation(mod) <| $ ( value )
        peek(value.name) <| $ ( vname )
            if vname == ann_name
                unsafe
                    ann = addr(value)
    return ann

def append_annotation ( var func:FunctionPtr; mod_name,ann_name:string; args:array< tuple<argname:string;argvalue:RttiValue> > )
    var ann = find_annotation(mod_name, ann_name)
    assert(ann!=null)
    var decl <- new [[AnnotationDeclaration()]]
    unsafe
        decl.annotation := reinterpret<smart_ptr<Annotation>> ann
    // TODO: merge changes
    for argP in args
        let argName & = argP.argname
        let arg & = argP.argvalue
        //  argIndex = add_annotation_argument(argName)
        if arg is tBool
            // TODO:
            //  decl.arguments[newArgIndex].basicType = Type tBool
            //  decl.arguments[newArgIndex].bValue = arg as tBool
            pass
        elif arg is tInt
            pass
        elif arg is tFloat
            pass
        elif arg is tString
            pass
        else
            panic("Invalid rtti value")
    // TODO: invoke apply
    // TODO: accept
    // func.annotations |> emplace(decl)

[function_macro(name="test_extra")]
class TestExtraAnnotation : AstFunctionAnnotation
    def override apply ( var func:FunctionPtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        append_annotation(func,"$","local_only",[{auto
            [[auto "foo", [[RttiValue tBool=true]] ]];
            [[auto "bar", [[RttiValue tInt=13]] ]]
        }])
        return true
