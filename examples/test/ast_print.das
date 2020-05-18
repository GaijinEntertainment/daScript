options rtti = true

require ast
require rtti

typedef
    ExpressionPtr = smart_ptr<Expression>
    ProgramPtr = smart_ptr<Program>
    TypeDeclPtr = smart_ptr<TypeDecl>
    EnumerationPtr = smart_ptr<Enumeration>
    StructurePtr = smart_ptr<Structure>
    FunctionPtr = smart_ptr<Function>
    VariablePtr = smart_ptr<Variable>
    MakeFieldDeclPtr = smart_ptr<MakeFieldDecl>

[unsafe]
def ExpressionPtr ( expr : smart_ptr<auto(TT)> ) : ExpressionPtr
    static_if typeinfo(is_handle type<TT>)
        if starts_with(typeinfo(fulltypename expr),"smart_ptr<ast::Expr")
            return reinterpret<smart_ptr<Expression>>(expr)
        else
            concept_assert(false, "not derived from ast::Expression")
            return reinterpret<ExpressionPtr> null
    else
        concept_assert(false, "not a handled type, derived from ast::Expression")
        return reinterpret<ExpressionPtr> null

[unsafe]
def make_visitor ( someClass ) : smart_ptr<VisitorAdapter>
    static_if typeinfo(is_class someClass)
        let classPtr = addr(someClass)
        let classInfo = class_info(someClass)
        return make_visitor(classPtr, classInfo)
    else
        concept_assert(false,"can't make visitor of non-class")
        return reinterpret<smart_ptr<VisitorAdapter>> null

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
        return join( [{ for name,bit in bftd.argNames,range(0,32); "[{name}]";
            where (int(bf) & (1<<bit)) != 0 }], merger )
    else
        concept_assert(false,"requires bitfield")

class AstVisitor
// program
    def preVisitProgram(prog:ProgramPtr) : void
        pass
    def visitProgram(porg:ProgramPtr) : void
        pass
    def preVisitProgramBody(prog:ProgramPtr) : void
        pass
// type
    def preVisitTypeDecl(typ:TypeDeclPtr) : void
        pass
    def visitTypeDecl(typ:TypeDeclPtr) : TypeDeclPtr
        return typ
// type alias
    def preVisitAlias(typ:TypeDeclPtr;name:das_string) : void
        pass
    def visitAlias(typ:TypeDeclPtr;name:das_string)
        return typ
// enumeration
    def preVisitEnumeration(enu:EnumerationPtr) : void
        pass
    def preVisitEnumerationValue(enu:EnumerationPtr;name:das_string;value:ExpressionPtr;last:bool) : void
        pass
    def visitEnumerationValue(enu:EnumerationPtr;name:das_string;value:ExpressionPtr;last:bool) : ExpressionPtr
        return value
    def visitEnumeration(enu:EnumerationPtr) : EnumerationPtr
        return enu
// structure
    def preVisitStructure(str:StructurePtr) : void
        pass
    def preVisitStructureField(str:StructurePtr;decl:FieldDeclaration;last:bool) : void
        pass
    def visitStructureField(str:StructurePtr;decl:FieldDeclaration;last:bool) : void
        pass
    def visitStructure(str:StructurePtr) : StructurePtr
        return str
// function
    def preVisitFunction(fun:FunctionPtr) : void
        pass
    def visitFunction(fun:FunctionPtr) : FunctionPtr
        return fun
    def preVisitFunctionArgument(fun:FunctionPtr;arg:VariablePtr;lastArg:bool) : void
        pass
    def visitFunctionArgument(fun:FunctionPtr;arg:VariablePtr;lastArg:bool) : VariablePtr
        return arg
    def preVisitFunctionArgumentInit(fun:FunctionPtr;arg:VariablePtr;value:ExpressionPtr) : void
        pass
    def visitFunctionArgumentInit(fun:FunctionPtr;arg:VariablePtr;value:ExpressionPtr) : ExpressionPtr
        return value
    def preVisitFunctionBody(fun:FunctionPtr) : void
        pass
    def visitFunctionBody(fun:FunctionPtr) : void
        pass
// any expression there is
    def preVisitExpression(expr:ExpressionPtr) : void
        pass
    def visitExpression(expr:ExpressionPtr) : ExpressionPtr
        return expr
// block
    def preVisitExprBlock(blk:smart_ptr<ExprBlock>)
        pass
    def visitExprBlock(blk:smart_ptr<ExprBlock>) : ExpressionPtr
        return ExpressionPtr(blk)
    def preVisitExprBlockArgument(blk:smart_ptr<ExprBlock>;arg:VariablePtr;lastArg:bool)
        pass
    def visitExprBlockArgument(blk:smart_ptr<ExprBlock>;arg:VariablePtr;lastArg:bool) : VariablePtr
        return arg
    def preVisitExprBlockArgumentInit(blk:smart_ptr<ExprBlock>;arg:VariablePtr;expr:ExpressionPtr)
        pass
    def visitExprBlockArgumentInit(blk:smart_ptr<ExprBlock>;arg:VariablePtr;expr:ExpressionPtr) : ExpressionPtr
        return expr
    def preVisitExprBlockExpression(blk:smart_ptr<ExprBlock>;expr:ExpressionPtr)
        pass
    def visitExprBlockExpression(blk:smart_ptr<ExprBlock>;expr:ExpressionPtr) : ExpressionPtr
        return expr
    def preVisitExprBlockFinal(blk:smart_ptr<ExprBlock>)
        pass
    def visitExprBlockFinal(blk:smart_ptr<ExprBlock>)
        pass
    def preVisitExprBlockFinalExpression(blk:smart_ptr<ExprBlock>;expr:ExpressionPtr)
        pass
    def visitExprBlockFinalExpression(blk:smart_ptr<ExprBlock>;expr:ExpressionPtr) : ExpressionPtr
        return expr
// let
    def preVisitExprLet(expr:smart_ptr<ExprLet>) : void
        pass
    def visitExprLet(expr:smart_ptr<ExprLet>) : ExpressionPtr
        return ExpressionPtr(expr)
    def preVisitExprLetVariable(expr:smart_ptr<ExprLet>;arg:VariablePtr;lastArg:bool) : void
        pass
    def visitExprLetVariable(expr:smart_ptr<ExprLet>;arg:VariablePtr;lastArg:bool) : VariablePtr
        return arg
    def preVisitExprLetVariableInit(blk:smart_ptr<ExprLet>;arg:VariablePtr;expr:ExpressionPtr)
        pass
    def visitExprLetVariableInit(blk:smart_ptr<ExprLet>;arg:VariablePtr;expr:ExpressionPtr) : ExpressionPtr
        return expr
// global let
    def preVisitGlobalLet(prog:ProgramPtr) : void
        pass
    def visitGlobalLet(prog:ProgramPtr) : void
        pass
    def preVisitGlobalLetVariable(arg:VariablePtr;lastArg:bool) : void
        pass
    def visitGlobalLetVariable(arg:VariablePtr;lastArg:bool) : VariablePtr
        return arg
    def preVisitGlobalLetVariableInit(arg:VariablePtr;expr:ExpressionPtr)
        pass
    def visitGlobalLetVariableInit(arg:VariablePtr;expr:ExpressionPtr) : ExpressionPtr
        return expr
// string builder
    def preVisitExprStringBuilder(expr:smart_ptr<ExprStringBuilder>)
        pass
    def visitExprStringBuilder(expr:smart_ptr<ExprStringBuilder>) : ExpressionPtr
        return ExpressionPtr(expr)
    def preVisitExprStringBuilderElement(expr:smart_ptr<ExprStringBuilder>;elem:ExpressionPtr;last:bool)
        pass
    def visitExprStringBuilderElement(expr:smart_ptr<ExprStringBuilder>;elem:ExpressionPtr;last:bool) : ExpressionPtr
        return elem
// new
    def preVisitExprNew(expr:smart_ptr<ExprNew>)
        pass
    def visitExprNew(expr:smart_ptr<ExprNew>) : ExpressionPtr
        return ExpressionPtr(expr)
    def preVisitExprNewArgument(expr:smart_ptr<ExprNew>;arg:ExpressionPtr;last:bool)
        pass
    def visitExprNewArgument(expr:smart_ptr<ExprNew>;arg:ExpressionPtr;last:bool) : ExpressionPtr
        return arg
// named call (this one only exists before or during the infer)
    def preVisitExprNamedCall(expr:smart_ptr<ExprNamedCall>)
        pass
    def visitExprNamedCall(expr:smart_ptr<ExprNamedCall>) : ExpressionPtr
        return ExpressionPtr(expr)
    def preVisitExprNamedCallArgument(expr:smart_ptr<ExprNamedCall>;arg:MakeFieldDeclPtr;last:bool)
        pass
    def visitExprNamedCallArgument(expr:smart_ptr<ExprNamedCall>;arg:MakeFieldDeclPtr;last:bool) : MakeFieldDeclPtr
        return arg
// looks like call (debug,assert,verify,erase,find,key_exists,keys,values,invoke,memzero etc)
    def preVisitExprLooksLikeCall(expr:smart_ptr<ExprLooksLikeCall>)
        pass
    def visitExprLooksLikeCall(expr:smart_ptr<ExprLooksLikeCall>) : ExpressionPtr
        return ExpressionPtr(expr)
    def preVisitExprLooksLikeCallArgument(expr:smart_ptr<ExprLooksLikeCall>;arg:ExpressionPtr;last:bool)
        pass
    def visitExprLooksLikeCallArgument(expr:smart_ptr<ExprLooksLikeCall>;arg:ExpressionPtr;last:bool) : ExpressionPtr
        return arg
// call
    def preVisitExprCall(expr:smart_ptr<ExprCall>)
        pass
    def visitExprCall(expr:smart_ptr<ExprCall>) : ExpressionPtr
        return ExpressionPtr(expr)
    def preVisitExprCallArgument(expr:smart_ptr<ExprCall>;arg:ExpressionPtr;last:bool)
        pass
    def visitExprCallArgument(expr:smart_ptr<ExprCall>;arg:ExpressionPtr;last:bool) : ExpressionPtr
        return arg
// null coaelescing
    def preVisitExprNullCoalescing(expr:smart_ptr<ExprNullCoalescing>)
        pass
    def visitExprNullCoalescing(expr:smart_ptr<ExprNullCoalescing>) : ExpressionPtr
        return ExpressionPtr(expr)
    def preVisitExprNullCoalescingDefault(expr:smart_ptr<ExprNullCoalescing>;defval:ExpressionPtr)
        pass
// at
    def preVisitExprAt(expr:smart_ptr<ExprAt>)
        pass
    def visitExprAt(expr:smart_ptr<ExprAt>) : ExpressionPtr
        return ExpressionPtr(expr)
    def preVisitExprAtIndex(expr:smart_ptr<ExprAt>;index:ExpressionPtr)
        pass
// safe at
    def preVisitExprSafeAt(expr:smart_ptr<ExprSafeAt>)
        pass
    def visitExprSafeAt(expr:smart_ptr<ExprSafeAt>) : ExpressionPtr
        return ExpressionPtr(expr)
    def preVisitExprSafeAtIndex(expr:smart_ptr<ExprAt>;index:ExpressionPtr)
        pass
// is
    def preVisitExprIs(expr:smart_ptr<ExprIs>)
        pass
    def visitExprIs(expr:smart_ptr<ExprIs>) : ExpressionPtr
        return ExpressionPtr(expr)
    def preVisitExprIsType(expr:smart_ptr<ExprAt>;typeDecl:TypeDeclPtr)
        pass
// op2
    def preVisitExprOp2(expr:smart_ptr<ExprOp2>)
        pass
    def visitExprOp2(expr:smart_ptr<ExprOp2>) : ExpressionPtr
        return ExpressionPtr(expr)
    def preVisitExprOp2Right(expr:smart_ptr<ExprOp2>;right:ExpressionPtr)
        pass
// op3
    def preVisitExprOp3(expr:smart_ptr<ExprOp3>)
        pass
    def visitExprOp3(expr:smart_ptr<ExprOp3>) : ExpressionPtr
        return ExpressionPtr(expr)
    def preVisitExprOp3Left(expr:smart_ptr<ExprOp3>;left:ExpressionPtr)
        pass
    def preVisitExprOp3Right(expr:smart_ptr<ExprOp3>;right:ExpressionPtr)
        pass
// copy
    def preVisitExprCopy(expr:smart_ptr<ExprCopy>)
        pass
    def visitExprCopy(expr:smart_ptr<ExprCopy>) : ExpressionPtr
        return ExpressionPtr(expr)
    def preVisitExprCopyRight(expr:smart_ptr<ExprCopy>;right:ExpressionPtr)
        pass
// move
    def preVisitExprMove(expr:smart_ptr<ExprMove>)
        pass
    def visitExprMove(expr:smart_ptr<ExprMove>) : ExpressionPtr
        return ExpressionPtr(expr)
    def preVisitExprMoveRight(expr:smart_ptr<ExprMove>;right:ExpressionPtr)
        pass
// clone
    def preVisitExprClone(expr:smart_ptr<ExprClone>)
        pass
    def visitExprClone(expr:smart_ptr<ExprClone>) : ExpressionPtr
        return ExpressionPtr(expr)
    def preVisitExprCloneRight(expr:smart_ptr<ExprClone>;right:ExpressionPtr)
        pass
// with
    def preVisitExprWith(expr:smart_ptr<ExprWith>)
        pass
    def visitExprWith(expr:smart_ptr<ExprWith>) : ExpressionPtr
        return ExpressionPtr(expr)
    def preVisitExprWithBody(expr:smart_ptr<ExprWith>;right:ExpressionPtr)
        pass
// while
    def preVisitExprWhile(expr:smart_ptr<ExprWhile>)
        pass
    def visitExprWhile(expr:smart_ptr<ExprWhile>) : ExpressionPtr
        return ExpressionPtr(expr)
    def preVisitExprWhileBody(expr:smart_ptr<ExprWhile>;right:ExpressionPtr)
        pass
// try-catch
    def preVisitExprTryCatch(expr:smart_ptr<ExprTryCatch>)
        pass
    def visitExprTryCatch(expr:smart_ptr<ExprTryCatch>) : ExpressionPtr
        return ExpressionPtr(expr)
    def preVisitExprTryCatchCatch(expr:smart_ptr<ExprTryCatch>;right:ExpressionPtr)
        pass
// if-then-else
    def preVisitExprIfThenElse(expr:smart_ptr<ExprIfThenElse>)
        pass
    def visitExprIfThenElse(expr:smart_ptr<ExprIfThenElse>) : ExpressionPtr
        return ExpressionPtr(expr)
    def preVisitExprIfThenElseIfBlock(expr:smart_ptr<ExprIfThenElse>;ifBlock:ExpressionPtr)
        pass
    def preVisitExprIfThenElseElseBlock(expr:smart_ptr<ExprIfThenElse>;elseBlock:ExpressionPtr)
        pass
// for
    def preVisitExprFor(expr:smart_ptr<ExprFor>)
        pass
    def visitExprFor(expr:smart_ptr<ExprFor>) : ExpressionPtr
        return ExpressionPtr(expr)
    def preVisitExprForVariable(expr:smart_ptr<ExprFor>;svar:VariablePtr;last:bool)
        pass
    def visitExprForVariable(expr:smart_ptr<ExprFor>;svar:VariablePtr;last:bool) : VariablePtr
        return svar
    def preVisitExprForSource(expr:smart_ptr<ExprFor>;source:ExpressionPtr;last:bool)
        pass
    def visitExprForSource(expr:smart_ptr<ExprFor>;source:ExpressionPtr;last:bool) : ExpressionPtr
        return ExpressionPtr(source)
    def preVisitExprForStack(expr:smart_ptr<ExprFor>)
        pass
    def preVisitExprForBody(expr:smart_ptr<ExprFor>)
        pass
// make variant
    def preVisitExprMakeVariant(expr:smart_ptr<ExprMakeVariant>)
        pass
    def visitExprMakeVariant(expr:smart_ptr<ExprMakeVariant>) : ExpressionPtr
        return ExpressionPtr(expr)
    def preVisitExprMakeVariantField(expr:smart_ptr<ExprMakeVariant>;index:int;decl:MakeFieldDeclPtr;last:bool)
        pass
    def visitExprMakeVariantField(expr:smart_ptr<ExprMakeVariant>;index:int;decl:MakeFieldDeclPtr;last:bool) : MakeFieldDeclPtr
        return decl
// make structure
    def preVisitExprMakeStruct(expr:smart_ptr<ExprMakeStruct>)
        pass
    def visitExprMakeStruct(expr:smart_ptr<ExprMakeStruct>) : ExpressionPtr
        return ExpressionPtr(expr)
    def preVisitExprMakeStructIndex(expr:smart_ptr<ExprMakeStruct>;index:int;last:bool)
        pass
    def visitExprMakeStructIndex(expr:smart_ptr<ExprMakeStruct>;index:int;last:bool)
        pass
    def preVisitExprMakeStructField(expr:smart_ptr<ExprMakeStruct>;index:int;decl:MakeFieldDeclPtr;last:bool)
        pass
    def visitExprMakeStructField(expr:smart_ptr<ExprMakeStruct>;index:int;decl:MakeFieldDeclPtr;last:bool) : MakeFieldDeclPtr
        return decl
// make array
    def preVisitExprMakeArray(expr:smart_ptr<ExprMakeArray>)
        pass
    def visitExprMakeArray(expr:smart_ptr<ExprMakeArray>) : ExpressionPtr
        return ExpressionPtr(expr)
    def preVisitExprMakeArrayIndex(expr:smart_ptr<ExprMakeArray>;index:int;init:ExpressionPtr;last:bool)
        pass
    def visitExprMakeArrayIndex(expr:smart_ptr<ExprMakeArray>;index:int;init:ExpressionPtr;last:bool) : ExpressionPtr
        return ExpressionPtr(expr)
// make tuple
    def preVisitExprMakeTuple(expr:smart_ptr<ExprMakeTuple>)
        pass
    def visitExprMakeTuple(expr:smart_ptr<ExprMakeTuple>) : ExpressionPtr
        return ExpressionPtr(expr)
    def preVisitExprMakeTupleIndex(expr:smart_ptr<ExprMakeTuple>;index:int;init:ExpressionPtr;last:bool)
        pass
    def visitExprMakeTupleIndex(expr:smart_ptr<ExprMakeTuple>;index:int;init:ExpressionPtr;last:bool) : ExpressionPtr
        return ExpressionPtr(expr)
// array comprehension
    def preVisitExprArrayComprehension(expr:smart_ptr<ExprArrayComprehension>)
        pass
    def visitExprArrayComprehension(expr:smart_ptr<ExprArrayComprehension>) : ExpressionPtr
        return ExpressionPtr(expr)
    def preVisitExprArrayComprehensionSubexpr(expr:smart_ptr<ExprArrayComprehension>;subexrp:ExpressionPtr)
        pass
    def preVisitExprArrayComprehensionWhere(expr:smart_ptr<ExprArrayComprehension>;filter:ExpressionPtr)
        pass
// type info
    def preVisitExprTypeInfo(expr:smart_ptr<ExprTypeInfo>)
        pass
    def visitExprTypeInfo(expr:smart_ptr<ExprTypeInfo>) : ExpressionPtr
        return ExpressionPtr(expr)
// ptr to ref
    def preVisitExprPtr2Ref(expr:smart_ptr<ExprPtr2Ref>)
        pass
    def visitExprPtr2Ref(expr:smart_ptr<ExprPtr2Ref>) : ExpressionPtr
        return ExpressionPtr(expr)
// label
    def preVisitExprLabel(expr:smart_ptr<ExprLabel>)
        pass
    def visitExprLabel(expr:smart_ptr<ExprLabel>) : ExpressionPtr
        return ExpressionPtr(expr)
// goto
    def preVisitExprGoto(expr:smart_ptr<ExprGoto>)
        pass
    def visitExprGoto(expr:smart_ptr<ExprGoto>) : ExpressionPtr
        return ExpressionPtr(expr)
// ref to value
    def preVisitExprRef2Value(expr:smart_ptr<ExprRef2Value>)
        pass
    def visitExprRef2Value(expr:smart_ptr<ExprRef2Value>) : ExpressionPtr
        return ExpressionPtr(expr)
// ref to ptr
    def preVisitExprRef2Ptr(expr:smart_ptr<ExprRef2Ptr>)
        pass
    def visitExprRef2Ptr(expr:smart_ptr<ExprRef2Ptr>) : ExpressionPtr
        return ExpressionPtr(expr)

//////////
// example

let
    function_annotation_flags = (
        (FunctionFlags fastCall) |
        (FunctionFlags exports) |
        (FunctionFlags privateFunction) |
        (FunctionFlags unsafeDeref) |
        (FunctionFlags unsafe) |
        (FunctionFlags unsafeOperation) |
        (FunctionFlags generatorFunction) )

// TODO: fixme
def noBracket(expr)
    return false

class PrintVisitor : AstVisitor
    extraTypeInfo : bool = true
    printCStyle : bool = true
    tab : int = 0
    // TODO: fixme
    def newLine()
        print("\n")
// program
    def override preVisitProgram(prog:ProgramPtr)
        print("// program\n")
    def override visitProgram(prog:ProgramPtr)
        print("// end program\n")
    def override preVisitProgramBody(prog:ProgramPtr)
        print("// program body\n")
// type
    ET : bool = false
    def override preVisitTypeDecl(typ:TypeDeclPtr)
        ET = typ.isExprType
        if ET
            print("/*[")
    def override visitTypeDecl(typ:TypeDeclPtr)
        if ET
            print("]*/")
        return typ
// alias
    def override preVisitAlias(typ:TypeDeclPtr;name:das_string)
        print("typedef\n\t{name} = {describe([decl=typ,extra=extraTypeInfo])}\n\n")
// enumeration
    def override preVisitEnumeration(enu:EnumerationPtr)
        print("enum {enu.name} : {ast_das_to_string(enu.baseType)}\n")
    def override preVisitEnumerationValue(enu:EnumerationPtr;name:das_string;value:ExpressionPtr;last:bool)
        print("\t{name} =")
    def override visitEnumerationValue(enu:EnumerationPtr;name:das_string;value:ExpressionPtr;last:bool)
        print("\n")
        return value
    def override visitEnumeration(enu:EnumerationPtr)
        print("\n")
        return enu
// structure
    def override preVisitStructure(str:StructurePtr)
        if str.annotations.length != 0
            print("[{describe(str.annotations)}]\n")
        print("{str.flags.isClass ? "class" : "struct"} {str.name}\n")
    def override preVisitStructureField(str:StructurePtr;decl:FieldDeclaration;last:bool)
        print("\t")
        if decl.annotation.length != 0
            print("[{describe(decl.annotation)}] ")
        print("{decl.name} : {describe([decl=decl.typeDecl,extra=extraTypeInfo])}")
        if decl.flags.parentType
            print(" /* from {str.parent.name} */")
        if decl.init != null
            print("{decl.flags.moveSemantic ? " <- " : " = "}")
    def override visitStructureField(str:StructurePtr;decl:FieldDeclaration;last:bool)
        print("\n")
    def override visitStructure(str:StructurePtr) : StructurePtr
        print("\n")
        return str
// function
    def override preVisitFunction(fun:FunctionPtr)
        if fun.flags.knownSideEffects
            if fun.sideEffectFlags == bitfield(0)
                print("// [nosideeffects]\n")
            else
                print("// {describe_bitfield(fun.sideEffectFlags)}\n")
        let bfa : FunctionFlags = fun.flags & function_annotation_flags
        print("{describe_bitfield(bfa,"\n")}")
        if fun.annotations.length != 0
            print("[{describe(fun.annotations)}]\n")
        print("def {fun.name}")
        if fun.arguments.length != 0
            print(" ( ")
    def override preVisitFunctionBody(fun:FunctionPtr)
        if fun.arguments.length != 0
            print(" ) ")
        if fun.result != null & !fun.result.isVoid
            print(" : {describe([decl=fun.result,extra=extraTypeInfo])}")
        print("\n")
    def override visitFunction(fun:FunctionPtr) : FunctionPtr
        print("\n")
        return fun
    def override preVisitFunctionArgument(fun:FunctionPtr;arg:VariablePtr;last:bool)
        if arg.annotation.length != 0
            print("[{describe(arg.annotation)}] ")
        if !arg.typeDecl.flags.constant
            print("var ")
        if arg.isAccessUnused
            print(" /*unsued*/ ")
        print("{arg.name} : {describe([decl=arg.typeDecl,extra=extraTypeInfo])}")
    def override visitFunctionArgument(fun:FunctionPtr;arg:VariablePtr;last:bool)
        if !last
            print("; ")
        return arg
    def override preVisitFunctionArgumentInit(fun:FunctionPtr;arg:VariablePtr;value:ExpressionPtr)
        print(" = ")
// block
    def override preVisitExprBlock(blk:smart_ptr<ExprBlock>)
        if blk.blockFlags.isClosure
            if blk.returnType!=null | blk.arguments.length!=0
                print("$(")
                for arg,argIndex in blk.arguments,range(0,blk.arguments.length)
                    if arg.annotation.length != 0
                        print("[{describe(arg.annotation)}] ")
                    if arg.typeDecl.flags.constant
                        print("var ")
                    print("{arg.name} : {describe([decl=arg.typeDecl,extra=extraTypeInfo])}")
                    if argIndex != blk.arguments.length-1
                        print("; ")
                print(")")
                if blk.returnType!=null
                    print(":{describe([decl=blk.returnType,extra=extraTypeInfo])}")
                print("\n")
        if printCStyle | blk.blockFlags.isClosure
            print("{repeat("\t",tab)}\{\n")
        tab ++
    def override visitExprBlock(blk:smart_ptr<ExprBlock>)
        tab --
        if printCStyle | blk.blockFlags.isClosure
            print("{repeat("\t",tab)}\}\n")
        return ExpressionPtr(blk)
    def override preVisitExprBlockExpression(blk:smart_ptr<ExprBlock>;expr:ExpressionPtr)
        print("{repeat("\t",tab)}")
    def override visitExprBlockExpression(blk:smart_ptr<ExprBlock>;expr:ExpressionPtr)
        if printCStyle | blk.blockFlags.isClosure
            print(";")
        self->newLine()
        return expr
    def override visitExprBlockFinal(blk:smart_ptr<ExprBlock>)
        if printCStyle | blk.blockFlags.isClosure
            print("{repeat("\t",tab-1)}\} finally \{\n")
        else
            print("{repeat("\t",tab-1)}finally\n")
    def override preVisitExprBlockFinalExpression(blk:smart_ptr<ExprBlock>;expr:ExpressionPtr)
        print("{repeat("\t",tab)}")
    def override visitExprBlockFinalExpression(blk:smart_ptr<ExprBlock>;expr:ExpressionPtr)
        if printCStyle | blk.blockFlags.isClosure
            print(";")
        self->newLine()
        return expr
// let
    def override preVisitExprLet(expr:smart_ptr<ExprLet>)
        var isLet = true
        for pv in expr.variables
            if pv.typeDecl!=null & pv.typeDecl.flags.constant
                isLet = false
                break
        print("{isLet ? "let" : "var"} {expr.inScope ? "in_scope " : ""}")
    def override preVisitExprLetVariable(expr:smart_ptr<ExprLet>;arg:VariablePtr;lastArg:bool)
        if arg.isAccessUnused
            print(" /*unused*/ ")
        print("{arg.name} : {describe([decl=arg.typeDecl,extra=extraTypeInfo])}")
    def override visitExprLetVariable(expr:smart_ptr<ExprLet>;arg:VariablePtr;lastArg:bool)
        if !lastArg
            print(";")
        return arg
    def override preVisitExprLetVariableInit(blk:smart_ptr<ExprLet>;arg:VariablePtr;expr:ExpressionPtr)
        if arg.flags.init_via_move
            print(" <- ")
        elif arg.flags.init_via_clone
            print(" := ")
        else
            print(" = ")
// global let
    def override preVisitGlobalLetVariable(arg:VariablePtr;lastArg:bool)
        print("{arg.typeDecl.flags.constant ? "let" : "var"}{arg.flags.global_shared ? " shared" : ""}\n\t")
        if arg.isAccessUnused
            print(" /*unused*/ ")
        print("{arg.name} : {describe([decl=arg.typeDecl,extra=extraTypeInfo])}")
    def override visitGlobalLetVariable(arg:VariablePtr;lastArg:bool)
        print("\n\n")
        return arg
    def override preVisitGlobalLetVariableInit(arg:VariablePtr;expr:ExpressionPtr)
        if arg.flags.init_via_move
            print(" <- ")
        elif arg.flags.init_via_clone
            print(" := ")
        else
            print(" = ")
// string builder
    def override preVisitExprStringBuilder(expr:smart_ptr<ExprStringBuilder>)
        print("string_builder(")
    def override visitExprStringBuilder(expr:smart_ptr<ExprStringBuilder>)
        print(")")
        return ExpressionPtr(expr)
    def override visitExprStringBuilderElement(expr:smart_ptr<ExprStringBuilder>;elem:ExpressionPtr;last:bool)
        if !last
            print(",")
        return elem
// new
    def override preVisitExprNew(expr:smart_ptr<ExprNew>)
        print("new {describe(expr.typeexpr)}")
        if expr.initializer
            print("(")
    def override visitExprNew(expr:smart_ptr<ExprNew>)
        if expr.initializer
            print(")")
        return ExpressionPtr(expr)
    def override visitExprNewArgument(expr:smart_ptr<ExprNew>;arg:ExpressionPtr;last:bool)
        if !last
            print(",")
        return arg
// named call
    def override preVisitExprNamedCall(expr:smart_ptr<ExprNamedCall>)
        print("{expr.name}([")
    def override visitExprNamedCall(expr:smart_ptr<ExprNamedCall>)
        print("])")
        return ExpressionPtr(expr)
    def override preVisitExprNamedCallArgument(expr:smart_ptr<ExprNamedCall>;arg:MakeFieldDeclPtr;last:bool)
        print("{arg.name}{arg.moveSemantic ? "<-" : "="}")
    def override visitExprNamedCallArgument(expr:smart_ptr<ExprNamedCall>;arg:MakeFieldDeclPtr;last:bool)
        if !last
            print(",")
        return arg
// looks like call (debug,assert,verify,erase,find,key_exists,keys,values,invoke,memzero etc)
    def override preVisitExprLooksLikeCall(expr:smart_ptr<ExprLooksLikeCall>)
        print("{expr.name}(")
    def override visitExprLooksLikeCall(expr:smart_ptr<ExprLooksLikeCall>)
        print(")")
        return ExpressionPtr(expr)
    def override visitExprLooksLikeCallArgument(expr:smart_ptr<ExprLooksLikeCall>;arg:ExpressionPtr;last:bool)
        if !last
            print(",")
        return arg
// call
    def override preVisitExprCall(expr:smart_ptr<ExprCall>)
        print("{expr.name}(")
    def override visitExprCall(expr:smart_ptr<ExprCall>)
        print(")")
        return ExpressionPtr(expr)
    def override visitExprCallArgument(expr:smart_ptr<ExprCall>;arg:ExpressionPtr;last:bool)
        if !last
            print(",")
        return arg
// null coaelescing
    def override preVisitExprNullCoalescingDefault(expr:smart_ptr<ExprNullCoalescing>;defval:ExpressionPtr)
        print(" ?? ")
// at
    def override visitExprAt(expr:smart_ptr<ExprAt>)
        print("]")
        return ExpressionPtr(expr)
    def override preVisitExprAtIndex(expr:smart_ptr<ExprAt>;index:ExpressionPtr)
        print("[")
// safe at
    def override visitExprSafeAt(expr:smart_ptr<ExprSafeAt>)
        print("]")
        return ExpressionPtr(expr)
    def override preVisitExprSafeAtIndex(expr:smart_ptr<ExprAt>;index:ExpressionPtr)
        print("?[")
// is
    def override preVisitExprIsType(expr:smart_ptr<ExprAt>;typeDecl:TypeDeclPtr)
        print("is type<{describe([decl=typeDecl,extra=false,contracts=true])}>")
// op2
    def override preVisitExprOp2(expr:smart_ptr<ExprOp2>)
        if !noBracket(expr)
            print("(")
    def override visitExprOp2(expr:smart_ptr<ExprOp2>)
        if !noBracket(expr)
            print(")")
        return ExpressionPtr(expr)
    def override preVisitExprOp2Right(expr:smart_ptr<ExprOp2>;right:ExpressionPtr)
        print(" {expr.op} ")
// op3
    def override preVisitExprOp3(expr:smart_ptr<ExprOp3>)
        if !noBracket(expr)
            print("(")
    def override visitExprOp3(expr:smart_ptr<ExprOp3>)
        if !noBracket(expr)
            print(")")
        return ExpressionPtr(expr)
    def override preVisitExprOp3Left(expr:smart_ptr<ExprOp3>;left:ExpressionPtr)
        print(" ? ")
    def override preVisitExprOp3Right(expr:smart_ptr<ExprOp3>;right:ExpressionPtr)
        print(" : ")
// copy
    def override preVisitExprCopyRight(expr:smart_ptr<ExprCopy>;right:ExpressionPtr)
        print(" = ")
// move
    def override preVisitExprMoveRight(expr:smart_ptr<ExprMove>;right:ExpressionPtr)
        print(" <- ")
// clone
    def override preVisitExprCloneRight(expr:smart_ptr<ExprClone>;right:ExpressionPtr)
        print(" := ")
// with
    def override preVisitExprWith(expr:smart_ptr<ExprWith>)
        print("with ")
    def override preVisitExprWithBody(expr:smart_ptr<ExprWith>;right:ExpressionPtr)
        print("\n")
// while
    def override preVisitExprWhile(expr:smart_ptr<ExprWhile>)
        print("while ")
    def override preVisitExprWhileBody(expr:smart_ptr<ExprWhile>;right:ExpressionPtr)
        print("\n")
// try-catch
    def override preVisitExprTryCatch(expr:smart_ptr<ExprTryCatch>)
        print("try\n")
    def override preVisitExprTryCatchCatch(expr:smart_ptr<ExprTryCatch>;right:ExpressionPtr)
        print("{repeat("\t",tab)}recover\n")
// if-then-else
    def override preVisitExprIfThenElse(expr:smart_ptr<ExprIfThenElse>)
        print("if ")
    def override preVisitExprIfThenElseIfBlock(expr:smart_ptr<ExprIfThenElse>;ifBlock:ExpressionPtr)
        print("\n")
    def override preVisitExprIfThenElseElseBlock(expr:smart_ptr<ExprIfThenElse>;elseBlock:ExpressionPtr)
        print("{repeat("\t",tab)}")
        if elseBlock.__rtti=="ExprIfThenElse"
            print("else ")
        else
            print("else\n")
// for
    def override preVisitExprFor(expr:smart_ptr<ExprFor>)
        print("for ")
    def override preVisitExprForVariable(expr:smart_ptr<ExprFor>;svar:VariablePtr;last:bool)
        print("{svar.name}{last ? " in " : ","}")
    def override visitExprForSource(expr:smart_ptr<ExprFor>;source:ExpressionPtr;last:bool) : ExpressionPtr
        if !last
            print(",")
        return ExpressionPtr(source)
    def override preVisitExprForBody(expr:smart_ptr<ExprFor>)
        print("\n")
// make variant
    def override preVisitExprMakeVariant(expr:smart_ptr<ExprMakeVariant>)
        print("[[")
        if expr.typeDecl != null
            print("{describe(expr.typeDecl)} ")
    def override visitExprMakeVariant(expr:smart_ptr<ExprMakeVariant>)
        print("]]")
        return ExpressionPtr(expr)
    def override preVisitExprMakeVariantField(expr:smart_ptr<ExprMakeVariant>;index:int;decl:MakeFieldDeclPtr;last:bool)
        print("{decl.name} {decl.moveSemantic ? "<-" : "=" } ")
    def override visitExprMakeVariantField(expr:smart_ptr<ExprMakeVariant>;index:int;decl:MakeFieldDeclPtr;last:bool)
        if !last
            print("; ")
        return decl
// make structure
    def override preVisitExprMakeStruct(expr:smart_ptr<ExprMakeStruct>)
        print("[[")
        if expr.typeDecl != null
            print("{describe(expr.typeDecl)}{expr.useInitializer ? "()" : ""} ")
    def override visitExprMakeStruct(expr:smart_ptr<ExprMakeStruct>)
        print("]]")
        return ExpressionPtr(expr)
    def override visitExprMakeStructIndex(expr:smart_ptr<ExprMakeStruct>;index:int;last:bool)
        if !last
            print("; ")
    def override preVisitExprMakeStructField(expr:smart_ptr<ExprMakeStruct>;index:int;decl:MakeFieldDeclPtr;last:bool)
        print("{decl.name} {decl.moveSemantic ? "<-" : "=" } ")
    def override visitExprMakeStructField(expr:smart_ptr<ExprMakeStruct>;index:int;decl:MakeFieldDeclPtr;last:bool)
        if !last
            print(", ")
        return decl
// make array
    def override preVisitExprMakeArray(expr:smart_ptr<ExprMakeArray>)
        print("[[")
        if expr.typeDecl != null
            print("{describe(expr.typeDecl)} ")
    def override visitExprMakeArray(expr:smart_ptr<ExprMakeArray>)
        print("]]")
        return ExpressionPtr(expr)
    def override visitExprMakeArrayIndex(expr:smart_ptr<ExprMakeArray>;index:int;init:ExpressionPtr;last:bool)
        if !last
            print("; ")
        return ExpressionPtr(expr)
// make tuple
    def override preVisitExprMakeTuple(expr:smart_ptr<ExprMakeTuple>)
        print("[[")
        if expr.typeDecl != null
            print("{describe(expr.typeDecl)} ")
    def override visitExprMakeTuple(expr:smart_ptr<ExprMakeTuple>)
        print("]]")
        return ExpressionPtr(expr)
    def override visitExprMakeTupleIndex(expr:smart_ptr<ExprMakeTuple>;index:int;init:ExpressionPtr;last:bool)
        if !last
            print(", ")
        return ExpressionPtr(expr)
// array comprehension
    def override preVisitExprArrayComprehension(expr:smart_ptr<ExprArrayComprehension>)
        print("{expr.generatorSyntax ? "[[" : "[\{"}")
    def override visitExprArrayComprehension(expr:smart_ptr<ExprArrayComprehension>)
        print("{expr.generatorSyntax ? "]]" : "\}]"}")
        return ExpressionPtr(expr)
    def override preVisitExprArrayComprehensionSubexpr(expr:smart_ptr<ExprArrayComprehension>;subexrp:ExpressionPtr)
        print("; ")
    def override preVisitExprArrayComprehensionWhere(expr:smart_ptr<ExprArrayComprehension>;filter:ExpressionPtr)
        print("; where ")
// type info
    def override preVisitExprTypeInfo(expr:smart_ptr<ExprTypeInfo>)
        print("typeinfo({expr.trait}")
        if !empty(expr.subtrait)
            print("<{expr.subtrait}")
            if !empty(expr.extratrait)
                print(";{expr.extratrait}")
        print(">")
        if expr.subexpr == null
            print("type<{describe(expr.typeexpr)}>")
    def override visitExprTypeInfo(expr:smart_ptr<ExprTypeInfo>)
        print(")")
        return ExpressionPtr(expr)
// ptr to ref
    def override preVisitExprPtr2Ref(expr:smart_ptr<ExprPtr2Ref>)
        print("deref(")
    def override visitExprPtr2Ref(expr:smart_ptr<ExprPtr2Ref>)
        print(")")
        return ExpressionPtr(expr)
// label
    def override preVisitExprLabel(expr:smart_ptr<ExprLabel>)
        print("label {expr.labelName}:")
        if !empty(expr.comment)
            print("/*{expr.comment}*/")
// goto
    def override preVisitExprGoto(expr:smart_ptr<ExprGoto>)
        print("goto ")
        if expr.subexpr==null
            print("label {expr.labelName}")
// ref to value
    def override preVisitExprRef2Value(expr:smart_ptr<ExprRef2Value>)
        print("/*r2v*/ (")
    def override visitExprRef2Value(expr:smart_ptr<ExprRef2Value>)
        print(")")
        return ExpressionPtr(expr)
// ref 2 ptr
    def override preVisitExprRef2Ptr(expr:smart_ptr<ExprRef2Ptr>)
        print("addr(")
    def override visitExprRef2Ptr(expr:smart_ptr<ExprRef2Ptr>)
        print(")")
        return ExpressionPtr(expr)

typedef
    Integer = int
    IorF = variant<i:int;f:float>

enum FooBar
    Foo
    Bar = 13


[comment(x=5,msg="haha")]
struct Foo
    [[do_not_delete,temp=13]] a : int = 11
    b : array<Foo?>

def Foo(x:int)
    return <- [[Foo a=x]]

def Foo(x,y:int)
    return <- [[Foo a=x+y]]

var
    add_extra = 13

[sideeffects]
def add(a,b:int)
    print("a={a} b={b}")
    return a + b + add_extra

[sideeffects,export,unsafe]
def allExpr(arg:int)
    // ExprStringBuilder
    var sb = "arg = {arg}\n"
    sb = ""
    // ExprNamedCall
    add([a=1,b=2])
    // ExprNew
    var pFoo1 = new Foo
    var pFoo2 = new Foo(1)
    var pFoo3 = new Foo(1,2)
    // ExprCall
    add(1,2)
    // ExprNullCoalescing
    var exprAt <- pFoo1 ?? [[Foo a=1]]
    exprAt <- [[Foo a=2]]
    // ExprAt
    pFoo1.b[0] = pFoo2
    // ExprSafeAt
    var exprSafeAt = pFoo1.b?[0] ?? pFoo2
    exprSafeAt = pFoo2
    // ExprIs
    if exprAt is type<int>
        print("nada\n")
    // ExprOp2
    var aa = 1
    aa = 3
    var bb = 2
    bb = 4
    var cc = aa + bb
    // ExprOp3
    cc = aa==bb ? aa : bb
    // ExprWith
    with exprAt
        aa = 3
    // ExprWhile
    while aa!=100
        aa++
    // ExprTryRecover
    try
        aa = 2
    recover
        aa = 3
    // ExprIfThenElse
    if aa==2
        aa = 2
    if aa==2
        aa = 2
    else
        aa = 3
    if aa==2
        aa = 2
    elif aa==3
        aa = 3
    else
        aa = 4
    // ExprMakeVariant
    var mkv = [[IorF i=5]]
    mkv = [[IorF f=1.0]]
    // ExprMakeStruct
    var mks <- [[Foo() a=5, b<-[{Foo? [1] null}]; a=6]]
    mks[1] <- [[Foo a=5]]
    // ExprMakeArray
    var mka = [[int 1;2;3;4]]
    mka = [[int 5;6;7;8]]
    // ExprMakeTuple
    var mkt = [[auto 1,2.0,"three"]]
    mkt = [[auto 2,3.0,"four"]]
    // ExprArrayCompehension
    var acomp <- [{for x in range(0,3); x*x; where x!=1}]
    var gcomp <- [[for x in range(0,3); x*x; where x!=1]]
    for ta,tg in acomp,gcomp
        verify(ta==tg)
    // ExprTypeInfo
    var tinfo = typeinfo(rtti_typeinfo type<int>).basicType
    print("{tinfo}\n")
    // ExprPtr2Ref
    print("{*pFoo1}")
    // ExprLabel, ExprGoto
    goto label 1
    print("nop")
    label 1:
    // ExprRef2Ptr
    let aacomp = addr(acomp)
    print("{aacomp}")

[export]
def test
    var astVisitor = PrintVisitor()
    var astVisitorAdapter <- make_visitor(astVisitor)
    ast_visit(ast_this_program(), astVisitorAdapter)

/*
    // ast_typedecl
    add_extra = 14
    let td <- typeinfo(ast_typedecl type<Foo>)
    print("td = {describe(td)}\n")
    // ast_expression
    var x = 1
    var y = 2
    x = 3
    y = 4
    let ed <- typeinfo(ast_expression x+y)
    print("ed = {describe(ed)}\n")
    // ast_function
    let fd <- typeinfo(ast_function @@add)
    print("fd = {describe(fd)}\n")
*/

    return true
