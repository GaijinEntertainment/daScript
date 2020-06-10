options indenting = 4

module ast_boost

require rtti
require ast

require daslib/strings

[export]
def describe(list:AnnotationArgumentList)
    return join([{ for arg in list; "{arg.name}={get_annotation_argument_value(arg)}" }],",")

[export]
def describe(ann:AnnotationDeclaration)
    if ann.arguments.length != 0
        return "{ann.annotation.name}({describe(ann.arguments)})"
    else
        return "{ann.annotation.name}"

[export]
def describe(list:AnnotationList)
    return join([{ for arg in list; describe(*arg) }], ",")

[export]
def describe_bitfield(bf;merger:string = "")
    static_if typeinfo(is_bitfield bf)
        let bftd <- typeinfo(ast_typedecl bf)
        return join( [{ for name,bit in bftd.argNames,range(0,32); "[{name}]";
            where (int(bf) & (1<<bit)) != 0 }], merger )
    else
        concept_assert(false,"requires bitfield")

[export]
def isExpression(t:TypeDeclPtr; top:bool=true) : bool
    if t==null
        return false
    elif t.dim.length!=0
        return false
    elif t.baseType==Type tHandle
        if "{t.annotation.annotationModule.name}" == "ast"
            return starts_with("{t.annotation.name}","Expr")
        else
            return false
    elif t.baseType==Type tPointer & top
        return isExpression(t.firstType,false)
    else
        return false

class MacroMacro : AstFunctionAnnotation
    [unsafe] def override apply ( var func:FunctionPtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        compiling_program().flags |= ProgramFlags needMacroModule
        func.flags |= FunctionFlags init
        var blk <- new [[ExprBlock() at=func.at]]
        var ifm <- new [[ExprCall() at=func.at, name:="is_compiling_macros"]]
        var ife <- new [[ExprIfThenElse() at=func.at, cond<-ExpressionPtr(ifm), if_true<-func.body]]
        push(blk.list,ExpressionPtr(ife))
        func.body <- ExpressionPtr(blk)
        return true

class TagFunctionAnnotation : AstFunctionAnnotation
    def override apply ( var func:FunctionPtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        for arg in args
            let val = get_annotation_argument_value(arg)
            if !(val is tBool)
                errors := "tag_function annotation expects only tag names"
                return false
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
            if "{ann.annotation.name}" == "tag_function"
                for t in ann.arguments
                    if "{t.name}" == tag
                        invoke(blk, func)

[export]
def find_arg(argn:string; args:AnnotationArgumentList) : RttiValue
    for a in args
        if "{a.name}" == argn
            return get_annotation_argument_value(a)
    return RttiValue_nothing()

[private]
def apply_tag_annotation(tag:string; ann:FunctionAnnotationPtr)
    for_each_tag_function(this_module(), tag) <| $ ( func )
        add_function_annotation(func, ann)

[unsafe,export]
def setup_tag_annotation(name,tag:string; classPtr)
    var ann <- make_function_annotation(name, classPtr)
    apply_tag_annotation(tag, ann)
    this_module() |> add_function_annotation(ann)
    delete ann

[unsafe,private]
def find_unique_function ( mod : Module?; name : string )
    var res : FunctionPtr
    var count = 0
    for_each_function(mod, name) <| $ ( func )
        res <- func
        count ++
    if count > 1
        panic("more than one function encountered")
    return res

[unsafe,export]
def setup_macro(name: string; at:LineInfo) : ExprBlock?
    compiling_program().flags |= ProgramFlags needMacroModule
    var fn <- compiling_module() |> find_unique_function(name)
    if fn == null
        fn <- new [[Function() at=at, atDecl=at, name:="{name}"]]
        fn.flags |= FunctionFlags generated
        fn.flags |= FunctionFlags init
        fn.result <- new [[TypeDecl() baseType=Type tVoid]]
        var blk <- new [[ExprBlock() at=at]]
        var iblk <- new [[ExprBlock() at=at]]
        var ifm <- new [[ExprCall() at=at, name:="is_compiling_macros"]]
        var ife <- new [[ExprIfThenElse() at=at, cond<-ExpressionPtr(ifm), if_true:=ExpressionPtr(iblk)]]
        push(blk.list,ExpressionPtr(ife))
        fn.body <- ExpressionPtr(blk)
        compiling_module() |> add_function(fn)
        return reinterpret<ExprBlock?> iblk
    else
        if fn.body.__rtti != "ExprBlock"
            panic("expecting func.ExprBlock")
        var blk = reinterpret<ExprBlock?> fn.body
        if blk.list.length!=1 | blk.list[0].__rtti!="ExprIfThenElse"
            panic("expecting is_compiling_macros")
        var ife = reinterpret<ExprIfThenElse?> blk.list[0]
        if ife.if_true.__rtti != "ExprBlock"
            panic("expecting ife.if_true.ExprBlock")
        return reinterpret<ExprBlock?> ife.if_true

class SetupAnyAnnotation : AstStructureAnnotation
    annotation_function_call : string = ""
    name : string
    def setup_call(var st:StructurePtr;var cll:smart_ptr<ExprCall>)
        push(cll.arguments,ExpressionPtr(new [[ExprConstString() at=st.at, value:=name]]))
    [unsafe] def override apply ( var st:StructurePtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
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
        nee.typeexpr <- new [[TypeDecl() baseType=Type tStructure, structType=reinterpret<Structure?> st]]
        push(cll.arguments,ExpressionPtr(nee))
        push(blk.list,ExpressionPtr(cll))
        return true

class SetupFunctionAnnotatoin : SetupAnyAnnotation
    override annotation_function_call : string = "add_new_function_annotation"

class SetupReaderMacro : SetupAnyAnnotation
    override annotation_function_call : string = "add_new_reader_macro"

class SetupVariantMacro : SetupAnyAnnotation
    override annotation_function_call : string = "add_new_variant_macro"

class TagFunctionMacro : SetupAnyAnnotation
    override annotation_function_call : string = "setup_tag_annotation"
    tag : string
    def override setup_call(var st:StructurePtr;var cll:smart_ptr<ExprCall>)
        push(cll.arguments,ExpressionPtr(new [[ExprConstString() at=st.at, value:=name]]))
        push(cll.arguments,ExpressionPtr(new [[ExprConstString() at=st.at, value:=tag]]))
    [unsafe] def override apply ( var st:StructurePtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        let argTag = find_arg("tag", args)
        if argTag is nothing
            panic("missing tag argument")
        elif !(argTag is tString)
            panic("tag must be string")
        return SetupAnyAnnotation`apply(cast<SetupAnyAnnotation> self,st,group,args,errors)

// replacing ExprIsVariant(value,name) => ExprOp2('==",value.__rtti,"name")
// if value is ast::Expr*
class BetterRttiVisitor : AstVariantMacro
    def override visitExprIsVariant(prog:ProgramPtr; mod:Module?;expr:smart_ptr<ExprIsVariant>) : ExpressionPtr
        if isExpression(expr.value._type)
            var vdr <- new [[ExprField() at=expr.at, name:="__rtti", value <- clone_expression(expr.value)]]
            var cna <- new [[ExprConstString() at=expr.at, value:=expr.name]]
            var veq <- new [[ExprOp2() at=expr.at, op:="==", left<-ExpressionPtr(vdr), right<-ExpressionPtr(cna)]]
            return ExpressionPtr(veq)
        return [[ExpressionPtr]]

[unsafe,private]
def walk_and_convert_array ( data : uint8?; info:TypeDeclPtr; at:LineInfo ) : ExpressionPtr
    let total = any_array_size(data)
    if total != 0
        let stride = info.firstType.sizeOf
        var mkArr <- new [[ExprMakeArray() at=at, makeType<-clone_type(info.firstType)]]
        any_array_foreach(data, stride) <| $ ( value )
            var elem <- walk_and_convert(value, info.firstType, at)
            push(mkArr.values, elem)
        push(mkArr.makeType.dim,total)
        var mkToArrayMove <- new [[ExprCall() at=at, name:="to_array_move"]]
        push(mkToArrayMove.arguments,ExpressionPtr(mkArr))
        return ExpressionPtr(mkToArrayMove)
    else
        var mkArr <- new [[ExprMakeStruct() at=at, makeType<-clone_type(info)]]
        return ExpressionPtr(mkArr)

[unsafe,private]
def walk_and_convert_dim ( data : uint8?; info:TypeDeclPtr; at:LineInfo ) : ExpressionPtr
	let stride = info.baseSizeOf
	let total = info.countOf
	var einfo <- clone_type(info)
	clear(einfo.dim)
	var mkArr <- new [[ExprMakeArray() at=at, makeType := info]]
	for x in range(0,total)
		var elem <- walk_and_convert(data + stride*x, einfo, at)
		push(mkArr.values, elem)
	delete einfo
	return ExpressionPtr(mkArr)

[unsafe,private]
def walk_and_convert_pointer ( data : uint8?; info:TypeDeclPtr; at:LineInfo ) : ExpressionPtr
	let pdata = *(reinterpret<uint8??> data)
	var elem <- walk_and_convert(pdata, info.firstType, at)
	var mkAsc <- new [[ExprAscend() at=at, subexpr <- elem]]
	return ExpressionPtr(mkAsc)

[unsafe,private]
def walk_and_convert_structure ( data : uint8?; info:TypeDeclPtr; at:LineInfo ) : ExpressionPtr
	var mkStruct <- new [[ExprMakeStruct() at=at, makeType<-clone_type(info)]]
	let stype = info.structType
	for x in stype.fields
		let fdata = data + x.offset
		var elem <- walk_and_convert ( fdata, x._type, at)
		var mkS <- new [[ MakeStruct() ]]
		var mkF <- new [[MakeFieldDecl() at=at, name := x.name, value <- elem]]
        if !x._type.canCopy
            mkF.flags = FieldDeclarationFlags moveSemantics
		push(*mkS, mkF)
		push(mkStruct.structs, mkS)
	return ExpressionPtr(mkStruct)

[unsafe,private]
def walk_and_convert_variant ( data : uint8?; info:TypeDeclPtr; at:LineInfo ) : ExpressionPtr
	let vindex = * reinterpret<int?> data
	let offset = get_variant_field_offset(info,vindex)
	var mkVariant <- new [[ExprMakeVariant() at=at, makeType<-clone_type(info)]]
	var elem <- walk_and_convert ( data + offset, info.argTypes[vindex], at)
	var mkS <- new [[MakeFieldDecl() at=at, name:=info.argNames[vindex], value <- elem]]
    if !info.argTypes[vindex].canCopy
        mkS.flags = FieldDeclarationFlags moveSemantics
	push(mkVariant.variants, mkS)
	return ExpressionPtr(mkVariant)

[unsafe,private]
def walk_and_convert_table ( data : uint8?; info:TypeDeclPtr; at:LineInfo ) : ExpressionPtr
    let total = any_table_size(data)
    if total != 0
        var tupT <- new [[TypeDecl() baseType=Type tTuple]]
        push(tupT.argTypes,clone_type(info.firstType))
        push(tupT.argTypes,clone_type(info.secondType))
        var arrT <- clone_type(tupT)
        var mkArr <- new [[ExprMakeArray() at=at, makeType <- arrT]]
        var key_stride = info.firstType.sizeOf
        var value_stride = info.secondType.sizeOf
        any_table_foreach ( data, key_stride, value_stride ) <| $ ( pkey, pvalue )
            var key <- walk_and_convert ( pkey, info.firstType, at)
            var value <- walk_and_convert ( pvalue, info.secondType, at)
            var mkTup <- new [[ExprMakeTuple() at=at, recordType<-clone_type(tupT), isKeyValue=true]]
            push(mkTup.values,key)
            push(mkTup.values,value)
            push(mkArr.values,ExpressionPtr(mkTup))
        push(mkArr.makeType.dim,total)
        var mkToTableMove <- new [[ExprCall() at=at, name:="to_table_move"]]
        push(mkToTableMove.arguments,ExpressionPtr(mkArr))
        return ExpressionPtr(mkToTableMove)
    else
        var mkArr <- new [[ExprMakeStruct() at=at, makeType<-clone_type(info)]]
        return ExpressionPtr(mkArr)

[unsafe,private]
def walk_and_convert_basic ( data : uint8?; info:TypeDeclPtr; at:LineInfo ) : ExpressionPtr
	if info.baseType == Type tInt
		return ExpressionPtr(new [[ExprConstInt() at=at, value=*reinterpret<int?> data]])
	elif info.baseType == Type tFloat
		return ExpressionPtr(new [[ExprConstFloat() at=at, value=*reinterpret<float?> data]])
	elif info.baseType == Type tDouble
		return ExpressionPtr(new [[ExprConstDouble() at=at, value=*reinterpret<double?> data]])
	elif info.baseType == Type tBool
		return ExpressionPtr(new [[ExprConstBool() at=at, value=*reinterpret<bool?> data]])
	elif info.baseType == Type tString
		return ExpressionPtr(new [[ExprConstString() at=at, value:=*reinterpret<string?> data]])
	else
		panic("unsupported {info.baseType}")
        return [[ExpressionPtr]]

[unsafe,private]
def walk_and_convert ( data : uint8?; info:TypeDeclPtr; at:LineInfo ) : ExpressionPtr
	// print("0x{intptr(data)} {describe(info)}\n")
	if info.dim.length != 0
		return walk_and_convert_dim(data,info,at)
	elif info.baseType == Type tArray
		return walk_and_convert_array(data,info,at)
	elif info.baseType == Type tPointer
        if info.firstType==null | info.firstType.isVoid
            return ExpressionPtr(new [[ExprConstPtr() at=at]])
        else
		    return walk_and_convert_pointer(data,info,at)
	elif info.baseType == Type tStructure
		return walk_and_convert_structure(data,info,at)
	elif info.baseType == Type tVariant
		return walk_and_convert_variant(data,info,at)
	elif info.baseType == Type tTable
		return walk_and_convert_table(data,info,at)
    else
        return walk_and_convert_basic(data,info,at)

[unsafe,export]
def convert_to_expression ( var value : auto&; at:LineInfo )
	let info <- typeinfo(ast_typedecl value)
	let pval = addr(value)
	return walk_and_convert(reinterpret<uint8?> pval, info, at)

[_macro,private]
def setup
    if is_compiling_macros()
        add_new_function_annotation("macro", new MacroMacro())
        add_new_function_annotation("tag_function",new TagFunctionAnnotation())
        add_new_structure_annotation("function_macro",new SetupFunctionAnnotatoin())
        add_new_structure_annotation("reader_macro",new SetupReaderMacro())
        add_new_structure_annotation("variant_macro",new SetupVariantMacro())
        add_new_structure_annotation("tag_structure",new TagStructureAnnotation())
        add_new_structure_annotation("tag_function_macro",new TagFunctionMacro())
        add_new_variant_macro("better_rtti_in_expr", new BetterRttiVisitor())
