options indenting=4

require rtti

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

def arg_names ( info : TypeInfo ) : iterator<string>
    var it : iterator<string>
    _builtin_make_arg_names_iterator(it,info)
    return <- it

def arg_types ( info : TypeInfo ) : iterator<TypeInfo&>
    var it : iterator<TypeInfo&>
    _builtin_make_arg_types_iterator(it,info)
    return <- it

def arg_names ( info : VarInfo ) : iterator<string>
    var it : iterator<string>
    _builtin_make_arg_names_iterator(it,info)
    return <- it

def arg_types ( info : VarInfo ) : iterator<TypeInfo&>
    var it : iterator<TypeInfo&>
    _builtin_make_arg_types_iterator(it,info)
    return <- it

def context_for_each_function( blk:block<(info:FuncInfo):void> )
	for fni in range(0,get_total_functions())
		invoke(blk,get_function_info(fni))

def context_for_each_variable( blk:block<(info:VarInfo):void> )
	for vi in range(0,get_total_variables())
		invoke(blk,get_variable_info(vi))

[unsafe]
def each_dim(info)
	var pinfo = addr(info)
	return <- generator<int>() <| $()
		for index in range(0,int(pinfo.dimSize))
			yield get_dim(*pinfo,index)
		return false

