options indenting = 4

module hlsl_internal shared private

require ast
require rtti
require strings

require daslib/ast_boost
require daslib/templates_boost
require daslib/strings_boost

[structure_macro(name="cbuffer")]
class HlslCBuffer : AstStructureAnnotation
    def override apply ( var st:StructurePtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        for field in st.fields
            add_global_var(compiling_module(),"{field.name}", clone_type(field._type), field.at, true) <| $ ( var vvar )
                vvar.annotation |> add_annotation_argument("cbuffer", "{st.name}")
        return true

class HlslShader : AstFunctionAnnotation
    def override apply ( var func:FunctionPtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        func.flags |= FunctionFlags exports // note: this is temporary, until we are done with dependency collecting etc
        let argName = find_arg("name", args)
        if !(argName is tString)
            errors := "name must be string constant"
            return false
        add_global_var(compiling_module(),"{argName}", new [[TypeDecl() at=func.at, baseType=Type tString]], func.at, false)
        return true
    def override patch ( var func:FunctionPtr; var group:ModuleGroup; args,progArgs:AnnotationArgumentList; var errors : das_string ) : bool
        let argName = find_arg("name", args)
        if !(argName is tString)
            errors := "name must be string constant"
            return false
        // TODO: 'find_global(name)'
        var err : array<string>
        for_each_global(compiling_module()) <| $ ( var glob )
            peek(glob.name) <| $ ( gname )
                if gname == argName as tString
                    glob.init <- new [[ExprConstString() at=func.at,
                        value:=generate_hlsl(func, err),
                        _type <- new [[TypeDecl() at=func.at, baseType=Type tString]]
                    ]]
        if !err |> empty()
            errors := join(err, "\n")
            return false
        return true

[function_macro(name="pixel_shader")]
class HlslPixelShader : HlslShader {}

let private supported_structure_field_annotations <- {{
    "POSITION"  => Type tBool;
    "NORMAL"    => Type tBool;
    "TEXCOORD"  => Type tBool
}}

let private hlsl_types <- {{
    Type tString    => "string";    // NOTE - this is temporary until we remove non hlsl-related goo

    Type tFloat     => "float";
    Type tFloat2    => "float2";
    Type tFloat3    => "float3";
    Type tFloat4    => "float4";
    Type tInt       => "int";
    Type tInt2      => "int2";
    Type tInt3      => "int3";
    Type tInt4      => "int4";
    Type tBool      => "bool";
    Type tVoid      => "void"
}}

class HlslExport : AstVisitor
    errors : array<string>
    writer : StringBuilderWriter?
    tab : int = 0
    depFun : table<uint64;bool>
    depVar : table<uint64;bool>
    def HlslExport ( var w : StringBuilderWriter )
        unsafe
            writer = addr(w)
    // TODO: fixme
    def newLine()
        *writer |> write("\n")
    def error ( txt:string; at:LineInfo )
        let fname = at.fileInfo!=null ? "{at.fileInfo.name} " : ""
        errors |> push("\tHLSL: {txt} at {fname}line {int(at.line)}")
// HLSL TYPE
    def describe_hlsl_type ( t : TypeDeclPtr )
        let st = build_string() <| $ ( var tw )
            if t.flags.ref
                tw |> write("& ")
            if t.baseType==Type tStructure
                tw |> write("{t.structType.name}")
            elif hlsl_types |> key_exists(t.baseType)
                tw |> write(hlsl_types?[t.baseType] ?? "error")
            else
                self->error ("unsupported type {t.baseType}", t.at)
                tw |> write("error")
        return st
// type
    ET : bool = false
    def override preVisitTypeDecl(typ:TypeDeclPtr)
        ET = typ.isExprType
        if ET
            *writer |> write("/*[")
    def override visitTypeDecl(typ:TypeDeclPtr)
        if ET
            *writer |> write("]*/")
        return typ
// alias
    def override preVisitAlias(typ:TypeDeclPtr;name:das_string)
        *writer |> write("typedef\n\t{name} = {self->describe_hlsl_type(typ)}\n\n")
// enumeration
    def override preVisitEnumeration(enu:EnumerationPtr)
        *writer |> write("enum {enu.name} : {das_to_string(enu.baseType)}\n")
    def override preVisitEnumerationValue(enu:EnumerationPtr;name:das_string;value:ExpressionPtr;last:bool)
        *writer |> write("\t{name} =")
    def override visitEnumerationValue(enu:EnumerationPtr;name:das_string;value:ExpressionPtr;last:bool)
        *writer |> write("\n")
        return value
    def override visitEnumeration(enu:EnumerationPtr)
        *writer |> write("\n")
        return enu
// structure
    isCBuffer : bool
    def override preVisitStructure(str:StructurePtr)
        if str.flags.isClass
            self->error("classes are not supported", str.at)
        isCBuffer = false
        for ann in str.annotations
            if "{ann.annotation.name}"=="cbuffer"
                isCBuffer = true
                *writer |> write("cbuffer {str.name}")
                var reg =  find_arg("register",ann.arguments)
                if reg is tString
                    *writer |> write(" : register({reg as tString})")
                *writer |> write("\n\{\n")
        if !isCBuffer
            *writer |> write("struct {str.name}\n\{\n")
    def override preVisitStructureField(str:StructurePtr;decl:FieldDeclaration;last:bool)
        *writer |> write("\t")
        //if decl.annotation.length != 0
        //    *writer |> write("[{describe(decl.annotation)}] ")
        *writer |> write("{self->describe_hlsl_type(decl._type)} {decl.name}")
        if isCBuffer
            var poffset = find_arg("packoffset", decl.annotation)
            if poffset is tString
                *writer |> write(" : packoffset({poffset as tString})")
        else
            for ann in decl.annotation
                if ann.basicType==Type tBool
                    if supported_structure_field_annotations |> key_exists("{ann.name}")
                        *writer |> write ( " : {ann.name}")
                    else
                        self->error("unsupported field annotation {ann.name}", decl.at)
                else
                    self->error("unsupported field annotation {ann}", decl.at)
        if decl.init != null
            self->error("structure initialization is not supported", decl.init.at)
    def override visitStructureField(str:StructurePtr;decl:FieldDeclaration;last:bool)
        *writer |> write(";\n")
    def override visitStructure(str:StructurePtr) : StructurePtr
        *writer |> write("};\n\n")
        return str
// function
    def override canVisitFunction(arg:Function?):bool
        if !depFun |> key_exists(intptr(arg))
            return false
        return true
    def override preVisitFunction(fun:FunctionPtr)
        *writer |> write("{self->describe_hlsl_type(fun.result)} {fun.name} ( ")
    def override preVisitFunctionBody(fun:FunctionPtr)
        *writer |> write(" )")
        for ann in fun.annotations
            peek(ann.annotation.name) <| $ ( aname )
                if aname=="pixel_shader" // TODO: other ann
                    let argo = find_arg("output",ann.arguments)
                    if argo is tString
                        *writer |> write(" : {argo as tString}")
        *writer |> write("\n")
    def override visitFunction(fun:FunctionPtr) : FunctionPtr
        *writer |> write("\n")
        return fun
    def override preVisitFunctionArgument(fun:FunctionPtr;arg:VariablePtr;last:bool)
        if arg.annotation.length != 0
            *writer |> write("[{describe(arg.annotation)}] ")
        if !arg._type.flags.constant
            *writer |> write("var ")
        if arg.isAccessUnused
            *writer |> write(" /*unsued*/ ")
        *writer |> write("{arg.name} : {self->describe_hlsl_type(arg._type)}")
    def override visitFunctionArgument(fun:FunctionPtr;arg:VariablePtr;last:bool)
        if !last
            *writer |> write("; ")
        return arg
    def override preVisitFunctionArgumentInit(fun:FunctionPtr;arg:VariablePtr;value:ExpressionPtr)
        *writer |> write(" = ")
// block
    def override preVisitExprBlock(blk:smart_ptr<ExprBlock>)
        if blk.blockFlags.isClosure
            if blk.returnType!=null || blk.arguments.length!=0
                *writer |> write("$(")
                for arg,argIndex in blk.arguments,range(blk.arguments.length)
                    if arg.annotation.length != 0
                        *writer |> write("[{describe(arg.annotation)}] ")
                    if arg._type.flags.constant
                        *writer |> write("var ")
                    *writer |> write("{arg.name} : {self->describe_hlsl_type(arg._type)}")
                    if argIndex != blk.arguments.length-1
                        *writer |> write("; ")
                *writer |> write(")")
                if blk.returnType!=null
                    *writer |> write(":{self->describe_hlsl_type(blk.returnType)}")
                *writer |> write("\n")
        *writer |> write("{repeat("\t",tab)}\{\n")
        tab ++
    def override visitExprBlock(blk:smart_ptr<ExprBlock>) : ExpressionPtr
        tab --
        *writer |> write("{repeat("\t",tab)}\}\n")
        return blk
    def override preVisitExprBlockExpression(blk:smart_ptr<ExprBlock>;expr:ExpressionPtr)
        *writer |> write("{repeat("\t",tab)}")
    def override visitExprBlockExpression(blk:smart_ptr<ExprBlock>;expr:ExpressionPtr)
        *writer |> write(";")
        self->newLine()
        return expr
    def override visitExprBlockFinal(blk:smart_ptr<ExprBlock>)
        *writer |> write("{repeat("\t",tab-1)}\} finally \{\n")
    def override preVisitExprBlockFinalExpression(blk:smart_ptr<ExprBlock>;expr:ExpressionPtr)
        *writer |> write("{repeat("\t",tab)}")
    def override visitExprBlockFinalExpression(blk:smart_ptr<ExprBlock>;expr:ExpressionPtr)
        *writer |> write(";")
        self->newLine()
        return expr
// let
    def override preVisitExprLetVariable(expr:smart_ptr<ExprLet>;arg:VariablePtr;lastArg:bool)
        *writer |> write("{self->describe_hlsl_type(arg._type)} {arg.name}")
    def override visitExprLetVariable(expr:smart_ptr<ExprLet>;arg:VariablePtr;lastArg:bool)
        if !lastArg
            *writer |> write(";")
        return arg
    def override preVisitExprLetVariableInit(blk:smart_ptr<ExprLet>;arg:VariablePtr;expr:ExpressionPtr)
        // if arg.flags.init_via_move
        // elif arg.flags.init_via_clone
        *writer |> write(" = ")
// global let
    def override canVisitGlobalVariable(arg:Variable?):bool
        if !depVar |> key_exists(intptr(arg))
            return false
        if !(find_arg("cbuffer",arg.annotation) is nothing)
            return false
        return true
    def override preVisitGlobalLetVariable(arg:VariablePtr;lastArg:bool)
        *writer |> write("{self->describe_hlsl_type(arg._type)} {arg.name}")
    def override visitGlobalLetVariable(arg:VariablePtr;lastArg:bool)
        let reg = find_arg("register", arg.annotation)
        if reg is tString
            *writer |> write(": register({reg as tString})")
        *writer |> write(";\n\n")
        return arg
    def override preVisitGlobalLetVariableInit(arg:VariablePtr;expr:ExpressionPtr)
        // if arg.flags.init_via_move
        // elif arg.flags.init_via_clone
        *writer |> write(" = ")
// string builder
    def override preVisitExprStringBuilder(expr:smart_ptr<ExprStringBuilder>)
        *writer |> write("string_builder(")
    def override visitExprStringBuilder(expr:smart_ptr<ExprStringBuilder>) : ExpressionPtr
        *writer |> write(")")
        return expr
    def override visitExprStringBuilderElement(expr:smart_ptr<ExprStringBuilder>;elem:ExpressionPtr;last:bool)
        if !last
            *writer |> write(",")
        return elem
// new
    def override preVisitExprNew(expr:smart_ptr<ExprNew>)
        *writer |> write("new {self->describe_hlsl_type(expr.typeexpr)}")
        if expr.initializer
            *writer |> write("(")
    def override visitExprNew(expr:smart_ptr<ExprNew>) : ExpressionPtr
        if expr.initializer
            *writer |> write(")")
        return expr
    def override visitExprNewArgument(expr:smart_ptr<ExprNew>;arg:ExpressionPtr;last:bool)
        if !last
            *writer |> write(",")
        return arg
// named call
    def override preVisitExprNamedCall(expr:smart_ptr<ExprNamedCall>)
        *writer |> write("{expr.name}([")
    def override visitExprNamedCall(expr:smart_ptr<ExprNamedCall>) : ExpressionPtr
        *writer |> write("])")
        return expr
    def override preVisitExprNamedCallArgument(expr:smart_ptr<ExprNamedCall>;arg:MakeFieldDeclPtr;last:bool)
        *writer |> write("{arg.name}{arg.flags.moveSemantics ? "<-" : "="}")
    def override visitExprNamedCallArgument(expr:smart_ptr<ExprNamedCall>;arg:MakeFieldDeclPtr;last:bool)
        if !last
            *writer |> write(",")
        return arg
// looks like call (debug,assert,verify,erase,find,key_exists,keys,values,invoke,memzero etc)
    def isInvokeMethod(expr:smart_ptr<ExprLooksLikeCall>)
        if expr is ExprInvoke
            unsafe
                let einvoke = reinterpret<ExprInvoke?> expr
                return einvoke.isInvokeMethod
        return false
    def override preVisitExprLooksLikeCall(expr:smart_ptr<ExprLooksLikeCall>)
        if self->isInvokeMethod(expr)
            pass
        else
            *writer |> write("{expr.name}(")
    def override visitExprLooksLikeCall(expr:smart_ptr<ExprLooksLikeCall>) : ExpressionPtr
        *writer |> write(")")
        return expr
    def override preVisitExprLooksLikeCallArgument(expr:smart_ptr<ExprLooksLikeCall>;arg:ExpressionPtr;last:bool)
        let isInvoke = self->isInvokeMethod(expr)
        if isInvoke && arg==expr.arguments[1]
            *writer |> write("/*")
    def override visitExprLooksLikeCallArgument(expr:smart_ptr<ExprLooksLikeCall>;arg:ExpressionPtr;last:bool)
        let isInvoke = self->isInvokeMethod(expr)
        if isInvoke && arg==expr.arguments[0]
            *writer |> write("(")
        elif isInvoke && arg==expr.arguments[1]
            *writer |> write("*/")
        elif !last
            *writer |> write(",")
        return arg
// call
    def override preVisitExprCall(expr:smart_ptr<ExprCall>)
        *writer |> write("{expr.name}(")
    def override visitExprCall(expr:smart_ptr<ExprCall>) : ExpressionPtr
        *writer |> write(")")
        return expr
    def override visitExprCallArgument(expr:smart_ptr<ExprCall>;arg:ExpressionPtr;last:bool)
        if !last
            *writer |> write(",")
        return arg
// null coaelescing
    def override preVisitExprNullCoalescingDefault(expr:smart_ptr<ExprNullCoalescing>;defval:ExpressionPtr)
        *writer |> write(" ?? ")
// at
    def override visitExprAt(expr:smart_ptr<ExprAt>) : ExpressionPtr
        *writer |> write("]")
        return expr
    def override preVisitExprAtIndex(expr:smart_ptr<ExprAt>;index:ExpressionPtr)
        *writer |> write("[")
// safe at
    def override visitExprSafeAt(expr:smart_ptr<ExprSafeAt>) : ExpressionPtr
        *writer |> write("]")
        return expr
    def override preVisitExprSafeAtIndex(expr:smart_ptr<ExprAt>;index:ExpressionPtr)
        *writer |> write("?[")
// is
    def override preVisitExprIsType(expr:smart_ptr<ExprAt>;typeDecl:TypeDeclPtr)
        *writer |> write("is type<{describe([decl=typeDecl,extra=false,contracts=true])}>")
// op2
    def override preVisitExprOp2(expr:smart_ptr<ExprOp2>)
        *writer |> write("(")
    def override visitExprOp2(expr:smart_ptr<ExprOp2>) : ExpressionPtr
        *writer |> write(")")
        return expr
    def override preVisitExprOp2Right(expr:smart_ptr<ExprOp2>;right:ExpressionPtr)
        *writer |> write(" {expr.op} ")
// op3
    def override preVisitExprOp3(expr:smart_ptr<ExprOp3>)
        *writer |> write("(")
    def override visitExprOp3(expr:smart_ptr<ExprOp3>) : ExpressionPtr
        *writer |> write(")")
        return expr
    def override preVisitExprOp3Left(expr:smart_ptr<ExprOp3>;left:ExpressionPtr)
        *writer |> write(" ? ")
    def override preVisitExprOp3Right(expr:smart_ptr<ExprOp3>;right:ExpressionPtr)
        *writer |> write(" : ")
// copy
    def override preVisitExprCopyRight(expr:smart_ptr<ExprCopy>;right:ExpressionPtr)
        *writer |> write(" = ")
// move
    def override preVisitExprMoveRight(expr:smart_ptr<ExprMove>;right:ExpressionPtr)
        *writer |> write(" <- ")
// clone
    def override preVisitExprCloneRight(expr:smart_ptr<ExprClone>;right:ExpressionPtr)
        *writer |> write(" := ")
// with
    def override preVisitExprWith(expr:smart_ptr<ExprWith>)
        *writer |> write("with ")
    def override preVisitExprWithBody(expr:smart_ptr<ExprWith>;right:ExpressionPtr)
        *writer |> write("\n")
// while
    def override preVisitExprWhile(expr:smart_ptr<ExprWhile>)
        *writer |> write("while ")
    def override preVisitExprWhileBody(expr:smart_ptr<ExprWhile>;right:ExpressionPtr)
        *writer |> write("\n")
// try-catch
    def override preVisitExprTryCatch(expr:smart_ptr<ExprTryCatch>)
        *writer |> write("try\n")
    def override preVisitExprTryCatchCatch(expr:smart_ptr<ExprTryCatch>;right:ExpressionPtr)
        *writer |> write("{repeat("\t",tab)}recover\n")
// if-then-else
    def override preVisitExprIfThenElse(expr:smart_ptr<ExprIfThenElse>)
        *writer |> write("if ")
    def override preVisitExprIfThenElseIfBlock(expr:smart_ptr<ExprIfThenElse>;ifBlock:ExpressionPtr)
        *writer |> write("\n")
    def override preVisitExprIfThenElseElseBlock(expr:smart_ptr<ExprIfThenElse>;elseBlock:ExpressionPtr)
        *writer |> write("{repeat("\t",tab)}")
        if elseBlock.__rtti=="ExprIfThenElse"
            *writer |> write("else ")
        else
            *writer |> write("else\n")
// for
    def override preVisitExprFor(expr:smart_ptr<ExprFor>)
        *writer |> write("for ")
    def override preVisitExprForVariable(expr:smart_ptr<ExprFor>;svar:VariablePtr;last:bool)
        *writer |> write("{svar.name}{last ? " in " : ","}")
    def override visitExprForSource(expr:smart_ptr<ExprFor>;source:ExpressionPtr;last:bool) : ExpressionPtr
        if !last
            *writer |> write(",")
        return source
    def override preVisitExprForBody(expr:smart_ptr<ExprFor>)
        *writer |> write("\n")
// make variant
    def override preVisitExprMakeVariant(expr:smart_ptr<ExprMakeVariant>)
        *writer |> write("[[")
        if expr._type != null
            *writer |> write("{describe(expr._type)} ")
    def override visitExprMakeVariant(expr:smart_ptr<ExprMakeVariant>) : ExpressionPtr
        *writer |> write("]]")
        return expr
    def override preVisitExprMakeVariantField(expr:smart_ptr<ExprMakeVariant>;index:int;decl:MakeFieldDeclPtr;last:bool)
        *writer |> write("{decl.name} {decl.flags.moveSemantics ? "<-" : "=" } ")
    def override visitExprMakeVariantField(expr:smart_ptr<ExprMakeVariant>;index:int;decl:MakeFieldDeclPtr;last:bool)
        if !last
            *writer |> write("; ")
        return decl
// make structure
    def override preVisitExprMakeStruct(expr:smart_ptr<ExprMakeStruct>)
        *writer |> write("[[")
        if expr._type != null
            *writer |> write("{describe(expr._type)}{expr.makeStructFlags.useInitializer ? "()" : ""} ")
    def override visitExprMakeStruct(expr:smart_ptr<ExprMakeStruct>) : ExpressionPtr
        *writer |> write("]]")
        return expr
    def override visitExprMakeStructIndex(expr:smart_ptr<ExprMakeStruct>;index:int;last:bool)
        if !last
            *writer |> write("; ")
    def override preVisitExprMakeStructField(expr:smart_ptr<ExprMakeStruct>;index:int;decl:MakeFieldDeclPtr;last:bool)
        *writer |> write("{decl.name} {decl.flags.moveSemantics ? "<-" : "=" } ")
    def override visitExprMakeStructField(expr:smart_ptr<ExprMakeStruct>;index:int;decl:MakeFieldDeclPtr;last:bool)
        if !last
            *writer |> write(", ")
        return decl
// make array
    def override preVisitExprMakeArray(expr:smart_ptr<ExprMakeArray>)
        *writer |> write("[[")
        if expr._type != null
            *writer |> write("{describe(expr._type)} ")
    def override visitExprMakeArray(expr:smart_ptr<ExprMakeArray>) : ExpressionPtr
        *writer |> write("]]")
        return expr
    def override visitExprMakeArrayIndex(expr:smart_ptr<ExprMakeArray>;index:int;init:ExpressionPtr;last:bool) : ExpressionPtr
        if !last
            *writer |> write("; ")
        return init
// make tuple
    def override preVisitExprMakeTuple(expr:smart_ptr<ExprMakeTuple>)
        *writer |> write("[[")
        if expr._type != null
            *writer |> write("{describe(expr._type)} ")
    def override visitExprMakeTuple(expr:smart_ptr<ExprMakeTuple>) : ExpressionPtr
        *writer |> write("]]")
        return expr
    def override visitExprMakeTupleIndex(expr:smart_ptr<ExprMakeTuple>;index:int;init:ExpressionPtr;last:bool) : ExpressionPtr
        if !last
            *writer |> write(", ")
        return init
// array comprehension
    def override preVisitExprArrayComprehension(expr:smart_ptr<ExprArrayComprehension>)
        *writer |> write("{expr.generatorSyntax ? "[[" : "[\{"}")
    def override visitExprArrayComprehension(expr:smart_ptr<ExprArrayComprehension>) : ExpressionPtr
        *writer |> write("{expr.generatorSyntax ? "]]" : "\}]"}")
        return expr
    def override preVisitExprArrayComprehensionSubexpr(expr:smart_ptr<ExprArrayComprehension>;subexrp:ExpressionPtr)
        *writer |> write("; ")
    def override preVisitExprArrayComprehensionWhere(expr:smart_ptr<ExprArrayComprehension>;filter:ExpressionPtr)
        *writer |> write("; where ")
// type info
    def override preVisitExprTypeInfo(expr:smart_ptr<ExprTypeInfo>)
        *writer |> write("typeinfo({expr.trait}")
        if !empty(expr.subtrait)
            *writer |> write("<{expr.subtrait}")
            if !empty(expr.extratrait)
                *writer |> write(";{expr.extratrait}")
        *writer |> write(">")
        if expr.subexpr == null
            *writer |> write("type<{describe(expr.typeexpr)}>")
    def override visitExprTypeInfo(expr:smart_ptr<ExprTypeInfo>) : ExpressionPtr
        *writer |> write(")")
        return expr
// ptr to ref
    def override preVisitExprPtr2Ref(expr:smart_ptr<ExprPtr2Ref>)
        *writer |> write("deref(")
    def override visitExprPtr2Ref(expr:smart_ptr<ExprPtr2Ref>) : ExpressionPtr
        *writer |> write(")")
        return expr
// label
    def override preVisitExprLabel(expr:smart_ptr<ExprLabel>)
        *writer |> write("label {expr.labelName}:")
        if !empty(expr.comment)
            *writer |> write("/*{expr.comment}*/")
// goto
    def override preVisitExprGoto(expr:smart_ptr<ExprGoto>)
        *writer |> write("goto ")
        if expr.subexpr==null
            *writer |> write("label {expr.labelName}")
// ref to value
    def override preVisitExprRef2Value(expr:smart_ptr<ExprRef2Value>)
        *writer |> write("/*r2v*/ (")
    def override visitExprRef2Value(expr:smart_ptr<ExprRef2Value>) : ExpressionPtr
        *writer |> write(")")
        return expr
// ref 2 ptr
    def override preVisitExprRef2Ptr(expr:smart_ptr<ExprRef2Ptr>)
        *writer |> write("addr(")
    def override visitExprRef2Ptr(expr:smart_ptr<ExprRef2Ptr>) : ExpressionPtr
        *writer |> write(")")
        return expr
// @@
    def override preVisitExprAddr(expr:smart_ptr<ExprAddr>)
        *writer |> write("@@")
        if expr.funcType != null
            *writer |> write("<{describe(expr.funcType)}>")
        *writer |> write("{expr.target}")
// ascend
    def override preVisitExprAscend(expr:smart_ptr<ExprAscend>)
        if expr.ascType != null
            *writer |> write("new<{describe(expr.ascType)}> ")
        else
            *writer |> write("new ")
// cast
    def override preVisitExprCast(expr:smart_ptr<ExprCast>)
        if expr.castFlags.reinterpretCast
            *writer |> write("reinterpret")
        elif expr.castFlags.upcastCast
            *writer |> write("upcast")
        else
            *writer |> write("cast")
        *writer |> write("<{describe(expr.castType)}> ")
// delete
    def override preVisitExprDelete(expr:smart_ptr<ExprDelete>)
        *writer |> write("delete ")
        if expr.native
            *writer |> write("/*native*/ ")
// var
    def override preVisitExprVar(expr:smart_ptr<ExprVar>)
        *writer |> write("{expr.name}")
// field
    def override visitExprField(expr:smart_ptr<ExprField>) : ExpressionPtr
        *writer |> write(".{expr.name}")
        return expr
// safe field
    def override visitExprSafeField(expr:smart_ptr<ExprSafeField>) : ExpressionPtr
        *writer |> write("?.{expr.name}")
        return expr
// swizzle
    def override visitExprSwizzle(expr:smart_ptr<ExprSwizzle>) : ExpressionPtr
        *writer |> write(".")
        let f2name = [[string "x";"y";"z";"w"]]
        for fch in expr.fields
            let f = int(fch)
            *writer |> write("{f>=0 && f<=3 ? f2name[f] : "?"}")
        return expr
// is variant
    def override visitExprIsVariant(expr:smart_ptr<ExprIsVariant>) : ExpressionPtr
        *writer |> write(" is {expr.name}")
        return expr
// as variant
    def override visitExprAsVariant(expr:smart_ptr<ExprAsVariant>) : ExpressionPtr
        *writer |> write(" as {expr.name}")
        return expr
// safe as variant
    def override visitExprSafeAsVariant(expr:smart_ptr<ExprSafeAsVariant>) : ExpressionPtr
        *writer |> write(" ?as {expr.name}")
        return expr
// op1
    def override preVisitExprOp1(expr:smart_ptr<ExprOp1>) : void
        let op = "{expr.op}"
        if op!="+++" && op!="---"
            *writer |> write(op)
        *writer |> write("(")
    def override visitExprOp1(expr:smart_ptr<ExprOp1>) : ExpressionPtr
        let op = "{expr.op}"
        *writer |> write(")")
        if op=="+++" || op=="---"
            *writer |> write("{slice(op,0,-1)}")
        return expr
// return
    def override preVisitExprReturn(expr:smart_ptr<ExprReturn>) : void
        *writer |> write("return ")
        if expr.returnFlags.fromYield
            *writer |> write("/* from yield */ ")
        if expr.returnFlags.moveSemantics
            *writer |> write("<- ")
// yield
    def override preVisitExprYield(expr:smart_ptr<ExprYield>) : void
        *writer |> write("yield ")
        if expr.returnFlags.moveSemantics
            *writer |> write("<- ")
// break
    def override preVisitExprBreak(expr:smart_ptr<ExprBreak>) : void
        *writer |> write("break")
// continue
    def override preVisitExprContinue(expr:smart_ptr<ExprContinue>) : void
        *writer |> write("continue")
// const ptr
    def override preVisitExprConstPtr(expr:smart_ptr<ExprConstPtr>) : void
        *writer |> write("{expr.value}")
// const int 8
    def override preVisitExprConstInt8(expr:smart_ptr<ExprConstInt8>) : void
        *writer |> write("{expr.value}")
// const int 16
    def override preVisitExprConstInt16(expr:smart_ptr<ExprConstInt16>) : void
        *writer |> write("{expr.value}")
// const int 64
    def override preVisitExprConstInt64(expr:smart_ptr<ExprConstInt64>) : void
        *writer |> write("{expr.value}")
// const int
    def override preVisitExprConstInt(expr:smart_ptr<ExprConstInt>) : void
        *writer |> write("{expr.value}")
// const int2
    def override preVisitExprConstInt2(expr:smart_ptr<ExprConstInt2>) : void
        *writer |> write("int2({expr.value})")
// const int3
    def override preVisitExprConstInt3(expr:smart_ptr<ExprConstInt3>) : void
        *writer |> write("int3({expr.value})")
// const int4
    def override preVisitExprConstInt4(expr:smart_ptr<ExprConstInt4>) : void
        *writer |> write("int4({expr.value})")
// const uint 8
    def override preVisitExprConstUInt8(expr:smart_ptr<ExprConstUInt8>) : void
        *writer |> write("{expr.value}")
// const uint 16
    def override preVisitExprConstUInt16(expr:smart_ptr<ExprConstUInt16>) : void
        *writer |> write("{expr.value}")
// const uint 64
    def override preVisitExprConstUInt64(expr:smart_ptr<ExprConstUInt64>) : void
        *writer |> write("{expr.value}")
// const uint
    def override preVisitExprConstUInt(expr:smart_ptr<ExprConstUInt>) : void
        *writer |> write("{expr.value}")
// const uint2
    def override preVisitExprConstUInt2(expr:smart_ptr<ExprConstUInt2>) : void
        *writer |> write("uint2({expr.value})")
// const uint3
    def override preVisitExprConstUInt3(expr:smart_ptr<ExprConstUInt3>) : void
        *writer |> write("uint3({expr.value})")
// const uint4
    def override preVisitExprConstUInt4(expr:smart_ptr<ExprConstUInt4>) : void
        *writer |> write("uint4({expr.value})")
// const range
    def override preVisitExprConstRange(expr:smart_ptr<ExprConstRange>) : void
        *writer |> write("range({expr.value})")
// const urnage
    def override preVisitExprConstURange(expr:smart_ptr<ExprConstURange>) : void
        *writer |> write("urange({expr.value})")
// const bool
    def override preVisitExprConstBool(expr:smart_ptr<ExprConstBool>) : void
        *writer |> write("{expr.value}")
// const float
    def override preVisitExprConstFloat(expr:smart_ptr<ExprConstFloat>) : void
        *writer |> write("{expr.value}")
// const float2
    def override preVisitExprConstFloat2(expr:smart_ptr<ExprConstFloat2>) : void
        *writer |> write("float2({expr.value})")
// const float3
    def override preVisitExprConstFloat3(expr:smart_ptr<ExprConstFloat3>) : void
        *writer |> write("float3({expr.value})")
// const float4
    def override preVisitExprConstFloat4(expr:smart_ptr<ExprConstFloat4>) : void
        *writer |> write("float4({expr.value})")
// const double
    def override preVisitExprConstDouble(expr:smart_ptr<ExprConstDouble>) : void
        *writer |> write("{expr.value}lf")
// fake context
    def override preVisitExprFakeContext(expr:smart_ptr<ExprFakeContext>) : void
        *writer |> write("__context__")
// fake line info
    def override preVisitExprFakeLineInfo(expr:smart_ptr<ExprFakeLineInfo>) : void
        *writer |> write("__lineinfo__")
// string
    def override preVisitExprConstString(expr:smart_ptr<ExprConstString>) : void
        peek(expr.value) <| $(str)
            *writer |> write("\"{escape(str)}\"")
// const enumeraiton
    def override preVisitExprConstEnumeration(expr:smart_ptr<ExprConstEnumeration>) : void
        let enumModule = expr.enumType._module
        if enumModule!=null && !empty(enumModule.name)
            *writer |> write("{enumModule.name}::")
        *writer |> write("{expr.enumType.name} {expr.value}")
// const bitfield
    def override preVisitExprConstBitfield(expr:smart_ptr<ExprConstBitfield>) : void
        var name : string
        if expr.bitfieldType!=null && !empty(expr.bitfieldType.alias)
            name = find_bitfield_name(expr.bitfieldType,expr.value)
        if !empty(name)
            *writer |> write("{expr.bitfieldType.alias} {name}")
        else
            *writer |> write("bitfield({expr.value})")


def generate_hlsl(fnMain:FunctionPtr; var errors:array<string> )
    var st = build_string() <| $ (var writer)
        var astVisitor = new HlslExport(writer)
        var astVisitorAdapter <- make_visitor(*astVisitor)
        collect_dependencies(fnMain) <| $ ( vfun, vvar )
            for f in vfun
                astVisitor.depFun[intptr(f)] = true
            for v in vvar
                astVisitor.depVar[intptr(v)] = true
            visit(compiling_program(), astVisitorAdapter)
        errors <- astVisitor.errors
        astVisitorAdapter := null
        unsafe
            delete astVisitor
    return st

