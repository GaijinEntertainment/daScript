options indenting=4

require rtti

typedef
    FileAccessPtr = smart_ptr<FileAccess>
    AnnotationPtr = smart_ptr<Annotation>
    TypeAnnotationPtr = smart_ptr<TypeAnnotation>

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
    return is_compatible_cast(reinterpret<StructInfo const?>(a),reinterpret<StructInfo const?>(b))

def context_for_each_function( blk:block<(info:FuncInfo):void> )
    for fni in range(0,get_total_functions())
        invoke(blk,get_function_info(fni))

def context_for_each_variable( blk:block<(info:VarInfo):void> )
    for vi in range(0,get_total_variables())
        invoke(blk,get_variable_info(vi))

[unsafe,private]
def each_dim_any(info)
    let pinfo = addr(info)
    return <- [[ for index in range(0,int(pinfo.dimSize)); get_dim(*pinfo,index) ]]

def each_dim(info:TypeInfo)
    return <- each_dim_any(info)

def each_dim(info:VarInfo)
    return <- each_dim_any(info)

[unsafe,private]
def arg_types_any(info)
    let pinfo = addr(info)
    return <- generator<TypeInfo const&>() <| $()
        if pinfo.argTypes!=null
            for index in range(0,int(pinfo.argCount))
                yield *(pinfo.argTypes[index])
        return false

def arg_types(info:TypeInfo)
    return <- arg_types_any(info)

def arg_types(info:VarInfo)
    return <- arg_types_any(info)

[unsafe,private]
def arg_names_any(info)
    let pinfo = addr(info)
    return <- generator<string>() <| $()
        if pinfo.argNames!=null
            for index in range(0,int(pinfo.argCount))
                yield pinfo.argNames[index]
        return false

def arg_names(info:TypeInfo)
    return <- arg_names_any(info)

def arg_names(info:VarInfo)
    return <- arg_names_any(info)

[unsafe]
def class_info ( cl ) : StructInfo const?
    static_if typeinfo(is_pointer cl)
        static_if typeinfo(is_class cl)
            return reinterpret<StructInfo const?>((reinterpret<TypeInfo?>(cl.__rtti)).structType)
        else
            concept_assert(false,"not a pointer to a class")
            return null
    static_if typeinfo(is_class cl)
        return reinterpret<StructInfo const?>((reinterpret<TypeInfo?>(cl.__rtti)).structType)
    else
        concept_assert(false,"not a class")
        return null

[unsafe]
def RttiValue_nothing : RttiValue
    var t : RttiValue
    set_variant_index(t, typeinfo(variant_index<nothing> t))
    return t
