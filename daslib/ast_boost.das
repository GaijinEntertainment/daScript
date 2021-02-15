options indenting = 4
options no_aot = true

module ast_boost shared

require rtti
require ast
require strings
require daslib/strings_boost

def describe(list:AnnotationArgumentList)
    return join([{ for arg in list; "{arg.name}={get_annotation_argument_value(arg)}" }],",")

def describe(ann:AnnotationDeclaration)
    if ann.arguments.length != 0
        return "{ann.annotation.name}({describe(ann.arguments)})"
    else
        return "{ann.annotation.name}"

def describe(list:AnnotationList)
    return join([{ for arg in list; describe(*arg) }], ",")

def describe_bitfield(bf;merger:string = "")
    static_if typeinfo(is_bitfield bf)
        let bftd <- typeinfo(ast_typedecl bf)
        return join( [{ for name,bit in bftd.argNames,range(32); "[{name}]";
            where (int(bf) & (1<<bit)) != 0 }], merger )
    else
        concept_assert(false,"requires bitfield")

def describe_function_short(func:FunctionPtr)
    var args <- [{for x in func.arguments; "{x.name}:{describe(x._type,false)}" }]
    var res = "{func.name} ({join(args,";")}) : {describe(func.result,false)}"
    delete args
    return res

def isExpression(t:TypeDeclPtr; top:bool=true) : bool
    if t==null
        return false
    elif t.dim.length!=0
        return false
    elif t.baseType==Type tHandle
        if "{t.annotation._module.name}" == "ast"
            return starts_with("{t.annotation.name}","Expr")
        else
            return false
    elif t.baseType==Type tPointer && top
        return isExpression(t.firstType,false)
    else
        return false

def is_same_or_inherited ( parent,child: Structure? )
    var ch = child
    while ch != null
        if parent == ch
            return true
        ch = ch.parent
    return false

def is_class_method ( cinfo:StructurePtr; finfo:TypeDeclPtr )
    if finfo.baseType != Type tFunction
        return false
    if finfo.dim.length !=0
        return false
    if finfo.argTypes.length == 0
        return false
    if finfo.argTypes[0].baseType != Type tStructure
        return false
    if finfo.argTypes[0].dim.length != 0
        return false
    if !is_same_or_inherited(finfo.argTypes[0].structType,get_ptr(cinfo))
        return false
    return true

def emplace_new ( var vec:dasvector`smart_ptr`Expression; var ptr:smart_ptr<Expression> )
    emplace(vec,ptr)

def emplace_new ( var vec:dasvector`smart_ptr`TypeDecl; var ptr:smart_ptr<TypeDecl> )
    emplace(vec,ptr)

def emplace_new ( var vec:dasvector`smart_ptr`Variable; var ptr:smart_ptr<Variable> )
    emplace(vec,ptr)

def emplace_new ( var vec:MakeStruct; var ptr:smart_ptr<MakeFieldDecl> )
    emplace(vec,ptr)

class MacroMacro : AstFunctionAnnotation
    def override apply ( var func:FunctionPtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        compiling_program().flags |= ProgramFlags needMacroModule
        func.flags |= FunctionFlags init
        var blk <- new [[ExprBlock() at=func.at]]
        var ifm <- new [[ExprCall() at=func.at, name:="is_compiling_macros"]]
        var ife <- new [[ExprIfThenElse() at=func.at, cond<-ifm, if_true<-func.body]]
        emplace(blk.list,ife)
        func.body <- blk
        return true

class TagFunctionAnnotation : AstFunctionAnnotation
    def override apply ( var func:FunctionPtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        for arg in args
            let val = get_annotation_argument_value(arg)
            if !(val is tBool)
                errors := "tag_function annotation expects only tag names"
                return false
        func.flags |= FunctionFlags exports
        return true

class TagStructureAnnotation : AstStructureAnnotation
    def override apply ( var str:StructurePtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        for arg in args
            let val = get_annotation_argument_value(arg)
            if !(val is tBool)
                errors := "tag_structure annotation expects only tag names"
                return false
        return true

[private]
def for_each_tag_function(mod:Module?; tag:string; blk : block<(var func:FunctionPtr):void> )
    for_each_function(mod, "") <| $ ( func )
        for ann in func.annotations
            if ann!=null && "{ann.annotation.name}" == "tag_function"
                for t in ann.arguments
                    if "{t.name}" == tag
                        invoke(blk, func)
    for_each_generic(mod, "") <| $ ( func )
        for ann in func.annotations
            if ann!=null && "{ann.annotation.name}" == "tag_function"
                for t in ann.arguments
                    if "{t.name}" == tag
                        invoke(blk, func)

def find_arg(argn:string; args:AnnotationArgumentList) : RttiValue
    for a in args
        if "{a.name}" == argn
            return get_annotation_argument_value(a)
    return RttiValue_nothing()

[private]
def apply_tag_annotation(tag:string; ann:FunctionAnnotationPtr)
    for_each_tag_function(this_module(), tag) <| $ ( func )
        var tagAnn := ann
        add_function_annotation(func, tagAnn)

def setup_tag_annotation(name,tag:string; classPtr)
    var ann <- make_function_annotation(name, classPtr)
    apply_tag_annotation(tag, ann)
    this_module() |> add_function_annotation(ann)

def find_unique_function ( mod : Module?; name : string )
    var res : FunctionPtr
    var count = 0
    for_each_function(mod, name) <| $ ( func )
        res <- func
        count ++
    if count > 1
        panic("more than one function encountered")
    return res

def find_unique_generic ( mod : Module?; name : string )
    var res : FunctionPtr
    var count = 0
    for_each_generic(mod, name) <| $ ( func )
        res <- func
        count ++
    if count > 1
        panic("more than one generic function encountered")
    return res

def setup_macro(name: string; at:LineInfo) : ExprBlock?
    compiling_program().flags |= ProgramFlags needMacroModule
    var fn <- compiling_module() |> find_unique_function(name)
    if fn == null
        fn <- new [[Function() at=at, atDecl=at, name:="{name}"]]
        fn.flags |= FunctionFlags generated
        fn.flags |= FunctionFlags init
        fn.result <- new [[TypeDecl() baseType=Type autoinfer, at=at]]
        var blk <- new [[ExprBlock() at=at]]
        var iblk <- new [[ExprBlock() at=at]]
        let res = get_ptr(iblk)
        var ifm <- new [[ExprCall() at=at, name:="is_compiling_macros_in_module"]]
        var ifmn <- new [[ExprConstString() at=at, value:=compiling_module().name]]
        emplace(ifm.arguments,ifmn)
        var ife <- new [[ExprIfThenElse() at=at, cond<-ifm, if_true<-iblk]]
        emplace(blk.list,ife)
        fn.body <- blk
        if !(compiling_module() |> add_function(fn))
            panic("can't setup")
        return res
    else
        if fn.body.__rtti != "ExprBlock"
            panic("expecting func.ExprBlock")
        unsafe
            var blk = reinterpret<ExprBlock?> fn.body
            if blk.list.length!=1 || blk.list[0].__rtti!="ExprIfThenElse"
                panic("expecting is_compiling_macros")
            var ife = reinterpret<ExprIfThenElse?> blk.list[0]
            if ife.if_true.__rtti != "ExprBlock"
                panic("expecting ife.if_true.ExprBlock")
            return reinterpret<ExprBlock?> ife.if_true

class SetupAnyAnnotation : AstStructureAnnotation
    annotation_function_call : string = ""
    name : string
    def setup_call(var st:StructurePtr;var cll:smart_ptr<ExprCall>)
        emplace_new(cll.arguments,new [[ExprConstString() at=st.at, value:=name]])
    def override apply ( var st:StructurePtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        let argName = find_arg("name", args)
        if argName is nothing
            name = "{st.name}"
        elif !(argName is tString)
            panic("name must be string")
        else
            name = argName as tString
        var blk <- setup_macro("__setup_macros", st.at)
        var cll <- new [[ExprCall() at=st.at, name:=annotation_function_call]]
        self->setup_call(st,cll)
        var nee <- new [[ExprNew() at=st.at, initializer=true]]
        nee.typeexpr <- new [[TypeDecl() baseType=Type tStructure, structType=get_ptr(st), at=st.at]]
        emplace(cll.arguments,nee)
        emplace(blk.list,cll)
        return true

class SetupFunctionAnnotatoin : SetupAnyAnnotation
    override annotation_function_call : string = "add_new_function_annotation"

class SetupReaderMacro : SetupAnyAnnotation
    override annotation_function_call : string = "add_new_reader_macro"

class SetupVariantMacro : SetupAnyAnnotation
    override annotation_function_call : string = "add_new_variant_macro"

class SetupCallMacro : SetupAnyAnnotation
    override annotation_function_call : string = "add_new_call_macro"

class TagFunctionMacro : SetupAnyAnnotation
    override annotation_function_call : string = "setup_tag_annotation"
    tag : string
    def override setup_call(var st:StructurePtr;var cll:smart_ptr<ExprCall>)
        emplace_new(cll.arguments,new [[ExprConstString() at=st.at, value:=name]])
        emplace_new(cll.arguments,new [[ExprConstString() at=st.at, value:=tag]])
    def override apply ( var st:StructurePtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        let argTag = find_arg("tag", args)
        if argTag is nothing
            panic("missing tag argument")
        elif !(argTag is tString)
            panic("tag must be string")
        tag = argTag as tString
        return SetupAnyAnnotation`apply(cast<SetupAnyAnnotation> self,st,group,args,errors)

// replacing ExprIsVariant(value,name) => ExprOp2('==",value.__rtti,"name")
// if value is ast::Expr*
class BetterRttiVisitor : AstVariantMacro
    def override visitExprIsVariant(prog:ProgramPtr; mod:Module?;expr:smart_ptr<ExprIsVariant>) : ExpressionPtr
        if isExpression(expr.value._type)
            var vdr <- new [[ExprField() at=expr.at, name:="__rtti", value <- clone_expression(expr.value)]]
            var cna <- new [[ExprConstString() at=expr.at, value:=expr.name]]
            var veq <- new [[ExprOp2() at=expr.at, op:="==", left<-vdr, right<-cna]]
            return veq
        return [[ExpressionPtr]]

[private]
def walk_and_convert_array ( data : uint8?; info:TypeDeclPtr; at:LineInfo ) : ExpressionPtr
    let total = any_array_size(data)
    if total != 0
        let stride = info.firstType.sizeOf
        var mkArr <- new [[ExprMakeArray() at=at, makeType<-clone_type(info.firstType)]]
        any_array_foreach(data, stride) <| $ ( value )
            var elem <- walk_and_convert(value, info.firstType, at)
            emplace(mkArr.values, elem)
        push(mkArr.makeType.dim,total)
        var mkToArrayMove <- new [[ExprCall() at=at, name:="to_array_move"]]
        emplace(mkToArrayMove.arguments,mkArr)
        return mkToArrayMove
    else
        var mkArr <- new [[ExprMakeStruct() at=at, makeType<-clone_type(info)]]
        return mkArr

[private]
def walk_and_convert_dim ( data : uint8?; info:TypeDeclPtr; at:LineInfo ) : ExpressionPtr
	let stride = info.baseSizeOf
	let total = info.countOf
	var einfo <- clone_type(info)
	clear(einfo.dim)
	var mkArr <- new [[ExprMakeArray() at=at, makeType := info]]
	for x in range(total)
        unsafe
            var elem <- walk_and_convert(data + stride*x, einfo, at)
            emplace(mkArr.values, elem)
    einfo := null
	return mkArr

[private]
def walk_and_convert_pointer ( data : uint8?; info:TypeDeclPtr; at:LineInfo ) : ExpressionPtr
    unsafe
    	let pdata = *(reinterpret<uint8??> data)
        var elem <- walk_and_convert(pdata, info.firstType, at)
        var mkAsc <- new [[ExprAscend() at=at, subexpr <- elem]]
        return mkAsc

[private]
def walk_and_convert_tuple ( data : uint8?; info:TypeDeclPtr; at:LineInfo ) : ExpressionPtr
    var mkArr <- new [[ExprMakeTuple() at=at, recordType<-clone_type(info)]]
    for idx in range( info.argTypes.length)
        let offset = get_tuple_field_offset(info,idx)
        unsafe
            var elem <- walk_and_convert(data + offset, info.argTypes[idx], at)
            emplace(mkArr.values, elem)
    return mkArr

[private]
def walk_and_convert_structure ( data : uint8?; info:TypeDeclPtr; at:LineInfo ) : ExpressionPtr
	var mkStruct <- new [[ExprMakeStruct() at=at, makeType<-clone_type(info)]]
	let stype = info.structType
    var mkS <- new [[ MakeStruct() ]]
	for x in stype.fields
        if find_arg("do_not_convert",x.annotation) is nothing
            unsafe
                let fdata = data + x.offset
                var elem <- walk_and_convert ( fdata, x._type, at)
                var mkF <- new [[MakeFieldDecl() at=at, name := x.name, value <- elem]]
                if !x._type.canCopy
                    mkF.flags = FieldDeclarationFlags moveSemantics
                emplace_new(*mkS, mkF)
    emplace(mkStruct.structs, mkS)
	return mkStruct

[private]
def walk_and_convert_variant ( data : uint8?; info:TypeDeclPtr; at:LineInfo ) : ExpressionPtr
    unsafe
        let vindex = * reinterpret<int?> data
        let offset = get_variant_field_offset(info,vindex)
        var mkVariant <- new [[ExprMakeVariant() at=at, makeType<-clone_type(info)]]
        var elem <- walk_and_convert ( data + offset, info.argTypes[vindex], at)
        var mkS <- new [[MakeFieldDecl() at=at, name:=info.argNames[vindex], value <- elem]]
        if !info.argTypes[vindex].canCopy
            mkS.flags = FieldDeclarationFlags moveSemantics
        emplace(mkVariant.variants, mkS)
        return mkVariant

[private]
def walk_and_convert_table ( data : uint8?; info:TypeDeclPtr; at:LineInfo ) : ExpressionPtr
    let total = any_table_size(data)
    if total != 0
        var tupT <- new [[TypeDecl() baseType=Type tTuple, at=at]]
        emplace_new(tupT.argTypes,clone_type(info.firstType))
        emplace_new(tupT.argTypes,clone_type(info.secondType))
        var arrT <- clone_type(tupT)
        var mkArr <- new [[ExprMakeArray() at=at, makeType <- arrT]]
        var key_stride = info.firstType.sizeOf
        var value_stride = info.secondType.sizeOf
        any_table_foreach ( data, key_stride, value_stride ) <| $ ( pkey, pvalue )
            var key <- walk_and_convert ( pkey, info.firstType, at)
            var value <- walk_and_convert ( pvalue, info.secondType, at)
            var mkTup <- new [[ExprMakeTuple() at=at, recordType<-clone_type(tupT), isKeyValue=true]]
            emplace(mkTup.values,key)
            emplace(mkTup.values,value)
            emplace(mkArr.values,mkTup)
        push(mkArr.makeType.dim,total)
        var mkToTableMove <- new [[ExprCall() at=at, name:="to_table_move"]]
        emplace(mkToTableMove.arguments,mkArr)
        tupT := null
        return mkToTableMove
    else
        var mkArr <- new [[ExprMakeStruct() at=at, makeType<-clone_type(info)]]
        return mkArr

[private]
def walk_and_convert_basic ( data : uint8?; info:TypeDeclPtr; at:LineInfo ) : ExpressionPtr
    unsafe
        if info.baseType == Type tInt
            return new [[ExprConstInt() at=at, value=*reinterpret<int?> data]]
        elif info.baseType == Type tUInt
            return new [[ExprConstUInt() at=at, value=*reinterpret<uint?> data]]
        elif info.baseType == Type tFloat
            return new [[ExprConstFloat() at=at, value=*reinterpret<float?> data]]
        elif info.baseType == Type tDouble
            return new [[ExprConstDouble() at=at, value=*reinterpret<double?> data]]
        elif info.baseType == Type tBool
            return new [[ExprConstBool() at=at, value=*reinterpret<bool?> data]]
        elif info.baseType == Type tRange
            return new [[ExprConstRange() at=at, value=*reinterpret<range?> data]]
        elif info.baseType == Type tURange
            return new [[ExprConstURange() at=at, value=*reinterpret<urange?> data]]
        elif info.baseType == Type tString
            return new [[ExprConstString() at=at, value:=*reinterpret<string?> data]]
        else
            panic("unsupported {info.baseType}")
            return [[ExpressionPtr]]

[private]
def walk_and_convert_enumeration ( data : uint8?; info:TypeDeclPtr; at:LineInfo ) : ExpressionPtr
    var eval = 0
    if info.baseType == Type tEnumeration
        unsafe
            eval = int(* reinterpret<uint?> data)
    elif info.baseType == Type tEnumeration8
        eval = int(* data)
    elif info.baseType == Type tEnumeration16
        unsafe
            eval = int(* reinterpret<uint16?> data)
    else
        panic("unsupported enumeration")
    return new [[ExprConstEnumeration() enumType:=info.enumType, value:=info.enumType.list[eval].name]]

[private]
def walk_and_convert ( data : uint8?; info:TypeDeclPtr; at:LineInfo ) : ExpressionPtr
	// print("0x{intptr(data)} {describe(info)}\n")
	if info.dim.length != 0
		return walk_and_convert_dim(data,info,at)
	elif info.baseType == Type tArray
		return walk_and_convert_array(data,info,at)
	elif info.baseType == Type tPointer
        unsafe
            if *reinterpret<uint8??>data == null
                return new [[ExprConstPtr() at=at]]
            elif info.firstType==null || info.firstType.isVoid
                return new [[ExprConstPtr() at=at]]
            else
                return walk_and_convert_pointer(data,info,at)
	elif info.baseType == Type tStructure
		return walk_and_convert_structure(data,info,at)
	elif info.baseType == Type tVariant
		return walk_and_convert_variant(data,info,at)
	elif info.baseType == Type tTuple
		return walk_and_convert_tuple(data,info,at)
	elif info.baseType == Type tTable
		return walk_and_convert_table(data,info,at)
    elif info.baseType == Type tEnumeration8 || info.baseType == Type tEnumeration16 || info.baseType == Type tEnumeration
        return walk_and_convert_enumeration(data,info,at)
    else
        return walk_and_convert_basic(data,info,at)

def convert_to_expression ( var value : auto&; at:LineInfo )
	let info <- typeinfo(ast_typedecl value)
    unsafe
	    let pval = addr(value)
	    return walk_and_convert(reinterpret<uint8?> pval, info, at)

[_macro,private]
def setup
    if is_compiling_macros_in_module("ast_boost")
        add_new_function_annotation("macro", new MacroMacro())
        add_new_function_annotation("tag_function",new TagFunctionAnnotation())
        add_new_structure_annotation("function_macro",new SetupFunctionAnnotatoin())
        add_new_structure_annotation("reader_macro",new SetupReaderMacro())
        add_new_structure_annotation("call_macro",new SetupCallMacro())
        add_new_structure_annotation("variant_macro",new SetupVariantMacro())
        add_new_structure_annotation("tag_structure",new TagStructureAnnotation())
        add_new_structure_annotation("tag_function_macro",new TagFunctionMacro())
        add_new_variant_macro("better_rtti_in_expr", new BetterRttiVisitor())
