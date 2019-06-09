require rtti

def rttiCompile(mod_name,text:string;subexpr:block<(ok:bool;prog:RttiProgram;issues:das_string):void>)
    rtti_builtin_compile(mod_name,text,subexpr)

def rttiCompileFile(mod_name:string;subexpr:block<(ok:bool;prog:RttiProgram;issues:das_string):void>)
    rtti_builtin_compile_file(mod_name,subexpr)

def rttiProgramForEachStructure(prog:RttiProgram;subexpr:block<(info:StructInfo):void>)
    rtti_builtin_program_for_each_structure(prog,subexpr)

def rttiStructureForEachAnnotation(st:StructInfo;subexpr:block<(ann:Annotation;args:AnnotationArguments):void>)
    rtti_builtin_structure_for_each_annotation(st,subexpr)

[unsafe]
def isSameType( a,b:TypeInfo; refMatters:bool=true; constMatters:bool=true; topLevel:bool=true )
    return builtin_isSameType(reinterpret<TypeInfo? const>(a),reinterpret<TypeInfo? const>(b),refMatters,constMatters,topLevel)

[unsafe]
def isCompatibleCast( a,b:StructInfo )
    return builtin_isCompatibleCast(reinterpret<StructInfo? const>(a),reinterpret<StructInfo? const>(b))

