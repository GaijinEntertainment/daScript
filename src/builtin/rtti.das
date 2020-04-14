options indenting=4

require rtti

def compile(mod_name,text:string;subexpr:block<(ok:bool;prog:RttiProgram;issues:das_string):void>)
    rtti_builtin_compile(mod_name,text,subexpr)

def compile_file(mod_name:string;subexpr:block<(ok:bool;prog:RttiProgram;issues:das_string):void>)
    rtti_builtin_compile_file(mod_name,subexpr)

def module_for_each_enumeration(pmod:RttiModule const?;subexpr:block<(info:EnumInfo):void>)
    rtti_builtin_module_for_each_enumeration(pmod,subexpr)

def module_for_each_structure(pmod:RttiModule const?;subexpr:block<(info:StructInfo):void>)
    rtti_builtin_module_for_each_structure(pmod,subexpr)

def module_for_each_function(pmod:RttiModule const?;subexpr:block<(info:FuncInfo):void>)
    rtti_builtin_module_for_each_function(pmod,subexpr)

def module_for_each_generic(pmod:RttiModule const?;subexpr:block<(info:FuncInfo):void>)
    rtti_builtin_module_for_each_generic(pmod,subexpr)

def structure_for_each_annotation(st:StructInfo;subexpr:block<(ann:Annotation;args:AnnotationArguments):void>)
    rtti_builtin_structure_for_each_annotation(st,subexpr)

[unsafe]
def is_same_type( a,b:TypeInfo;
        refMatters:RefMatters=RefMatters yes;
        constMatters:ConstMatters=ConstMatters yes;
        temporaryMatters:TemporaryMatters=TemporaryMatters yes;
        topLevel:bool=true )
    return builtin_is_same_type(reinterpret<TypeInfo const?>(a),reinterpret<TypeInfo const?>(b),refMatters,constMatters,temporaryMatters,topLevel)

[unsafe]
def is_compatible_cast( a,b:StructInfo )
    return builtin_is_compatible_cast(reinterpret<StructInfo const?>(a),reinterpret<StructInfo const?>(b))

def rtti_arg_names ( info : TypeInfo ) : iterator<string>
    var it : iterator<string>
    _builtin_make_arg_names_iterator(it,info)
    return <- it

def rtti_arg_types ( info : TypeInfo ) : iterator<TypeInfo&>
    var it : iterator<TypeInfo&>
    _builtin_make_arg_types_iterator(it,info)
    return <- it
