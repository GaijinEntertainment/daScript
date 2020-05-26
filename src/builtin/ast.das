options indenting=4

require ast
require rtti public

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
def describe ( decl : smart_ptr<TypeDecl>; extra, contracts, modules : bool = true )
    return ast_describe_typedecl(decl,extra,contracts,modules)

[unsafe]
def describe ( expr : smart_ptr<Expression> )
    return ast_describe_expression(expr)

[unsafe]
def describe ( expr : smart_ptr<Function> )
    return ast_describe_function(expr)

[generic,generic]
def describe(list:AnnotationArgumentList)
    return join([{ for arg in list; "{arg.name}={get_annotation_argument_value(arg)}" }],",")

[generic]
def describe(ann:AnnotationDeclaration)
    if ann.arguments.length != 0
        return "{ann.annotation.name}({describe(ann.arguments)})"
    else
        return "{ann.annotation.name}"

[generic]
def describe(list:AnnotationList)
    return join([{ for arg in list; describe(*arg) }], ",")

def describe_bitfield(bf;merger:string = "")
    static_if typeinfo(is_bitfield bf)
        let bftd <- typeinfo(ast_typedecl bf)
        return join( [{ for name,bit in bftd.argNames,range(0,32); "[{name}]";
            where (int(bf) & (1<<bit)) != 0 }], merger )
    else
        concept_assert(false,"requires bitfield")

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

class AstVisitor
// program
    def abstract preVisitProgram(prog:ProgramPtr) : void
    def abstract visitProgram(porg:ProgramPtr) : void
    def abstract preVisitProgramBody(prog:ProgramPtr) : void
// type
    def abstract preVisitTypeDecl(typ:TypeDeclPtr) : void
    def abstract visitTypeDecl(typ:TypeDeclPtr) : TypeDeclPtr
// type alias
    def abstract preVisitAlias(typ:TypeDeclPtr;name:das_string) : void
    def abstract visitAlias(typ:TypeDeclPtr;name:das_string) : TypeDeclPtr
// enumeration
    def abstract preVisitEnumeration(enu:EnumerationPtr) : void
    def abstract preVisitEnumerationValue(enu:EnumerationPtr;name:das_string;value:ExpressionPtr;last:bool) : void
    def abstract visitEnumerationValue(enu:EnumerationPtr;name:das_string;value:ExpressionPtr;last:bool) : ExpressionPtr
    def abstract visitEnumeration(enu:EnumerationPtr) : EnumerationPtr
// structure
    def abstract preVisitStructure(str:StructurePtr) : void
    def abstract preVisitStructureField(str:StructurePtr;decl:FieldDeclaration;last:bool) : void
    def abstract visitStructureField(str:StructurePtr;decl:FieldDeclaration;last:bool) : void
    def abstract visitStructure(str:StructurePtr) : StructurePtr
// function
    def abstract preVisitFunction(fun:FunctionPtr) : void
    def abstract visitFunction(fun:FunctionPtr) : FunctionPtr
    def abstract preVisitFunctionArgument(fun:FunctionPtr;arg:VariablePtr;lastArg:bool) : void
    def abstract visitFunctionArgument(fun:FunctionPtr;arg:VariablePtr;lastArg:bool) : VariablePtr
    def abstract preVisitFunctionArgumentInit(fun:FunctionPtr;arg:VariablePtr;value:ExpressionPtr) : void
    def abstract visitFunctionArgumentInit(fun:FunctionPtr;arg:VariablePtr;value:ExpressionPtr) : ExpressionPtr
    def abstract preVisitFunctionBody(fun:FunctionPtr) : void
    def abstract visitFunctionBody(fun:FunctionPtr) : void
// any expression there is
    def abstract preVisitExpression(expr:ExpressionPtr) : void
    def abstract visitExpression(expr:ExpressionPtr) : ExpressionPtr
// block
    def abstract preVisitExprBlock(blk:smart_ptr<ExprBlock>) : void
    def abstract visitExprBlock(blk:smart_ptr<ExprBlock>) : ExpressionPtr
    def abstract preVisitExprBlockArgument(blk:smart_ptr<ExprBlock>;arg:VariablePtr;lastArg:bool): void
    def abstract visitExprBlockArgument(blk:smart_ptr<ExprBlock>;arg:VariablePtr;lastArg:bool) : VariablePtr
    def abstract preVisitExprBlockArgumentInit(blk:smart_ptr<ExprBlock>;arg:VariablePtr;expr:ExpressionPtr): void
    def abstract visitExprBlockArgumentInit(blk:smart_ptr<ExprBlock>;arg:VariablePtr;expr:ExpressionPtr) : ExpressionPtr
    def abstract preVisitExprBlockExpression(blk:smart_ptr<ExprBlock>;expr:ExpressionPtr): void
    def abstract visitExprBlockExpression(blk:smart_ptr<ExprBlock>;expr:ExpressionPtr) : ExpressionPtr
    def abstract preVisitExprBlockFinal(blk:smart_ptr<ExprBlock>): void
    def abstract visitExprBlockFinal(blk:smart_ptr<ExprBlock>): void
    def abstract preVisitExprBlockFinalExpression(blk:smart_ptr<ExprBlock>;expr:ExpressionPtr): void
    def abstract visitExprBlockFinalExpression(blk:smart_ptr<ExprBlock>;expr:ExpressionPtr) : ExpressionPtr
// let
    def abstract preVisitExprLet(expr:smart_ptr<ExprLet>) : void
    def abstract visitExprLet(expr:smart_ptr<ExprLet>) : ExpressionPtr
    def abstract preVisitExprLetVariable(expr:smart_ptr<ExprLet>;arg:VariablePtr;lastArg:bool) : void
    def abstract visitExprLetVariable(expr:smart_ptr<ExprLet>;arg:VariablePtr;lastArg:bool) : VariablePtr
    def abstract preVisitExprLetVariableInit(blk:smart_ptr<ExprLet>;arg:VariablePtr;expr:ExpressionPtr): void
    def abstract visitExprLetVariableInit(blk:smart_ptr<ExprLet>;arg:VariablePtr;expr:ExpressionPtr) : ExpressionPtr
// global let
    def abstract preVisitGlobalLet(prog:ProgramPtr) : void
    def abstract visitGlobalLet(prog:ProgramPtr) : void
    def abstract preVisitGlobalLetVariable(arg:VariablePtr;lastArg:bool) : void
    def abstract visitGlobalLetVariable(arg:VariablePtr;lastArg:bool) : VariablePtr
    def abstract preVisitGlobalLetVariableInit(arg:VariablePtr;expr:ExpressionPtr): void
    def abstract visitGlobalLetVariableInit(arg:VariablePtr;expr:ExpressionPtr) : ExpressionPtr
// string builder
    def abstract preVisitExprStringBuilder(expr:smart_ptr<ExprStringBuilder>): void
    def abstract visitExprStringBuilder(expr:smart_ptr<ExprStringBuilder>) : ExpressionPtr
    def abstract preVisitExprStringBuilderElement(expr:smart_ptr<ExprStringBuilder>;elem:ExpressionPtr;last:bool): void
    def abstract visitExprStringBuilderElement(expr:smart_ptr<ExprStringBuilder>;elem:ExpressionPtr;last:bool) : ExpressionPtr
// new
    def abstract preVisitExprNew(expr:smart_ptr<ExprNew>): void
    def abstract visitExprNew(expr:smart_ptr<ExprNew>) : ExpressionPtr
    def abstract preVisitExprNewArgument(expr:smart_ptr<ExprNew>;arg:ExpressionPtr;last:bool): void
    def abstract visitExprNewArgument(expr:smart_ptr<ExprNew>;arg:ExpressionPtr;last:bool) : ExpressionPtr
// named call (this one only exists before or during the infer)
    def abstract preVisitExprNamedCall(expr:smart_ptr<ExprNamedCall>): void
    def abstract visitExprNamedCall(expr:smart_ptr<ExprNamedCall>) : ExpressionPtr
    def abstract preVisitExprNamedCallArgument(expr:smart_ptr<ExprNamedCall>;arg:MakeFieldDeclPtr;last:bool): void
    def abstract visitExprNamedCallArgument(expr:smart_ptr<ExprNamedCall>;arg:MakeFieldDeclPtr;last:bool) : MakeFieldDeclPtr
// looks like call (debug,assert,verify,erase,find,key_exists,keys,values,invoke,memzero etc)
    def abstract preVisitExprLooksLikeCall(expr:smart_ptr<ExprLooksLikeCall>): void
    def abstract visitExprLooksLikeCall(expr:smart_ptr<ExprLooksLikeCall>) : ExpressionPtr
    def abstract preVisitExprLooksLikeCallArgument(expr:smart_ptr<ExprLooksLikeCall>;arg:ExpressionPtr;last:bool): void
    def abstract visitExprLooksLikeCallArgument(expr:smart_ptr<ExprLooksLikeCall>;arg:ExpressionPtr;last:bool) : ExpressionPtr
// call
    def abstract preVisitExprCall(expr:smart_ptr<ExprCall>): void
    def abstract visitExprCall(expr:smart_ptr<ExprCall>) : ExpressionPtr
    def abstract preVisitExprCallArgument(expr:smart_ptr<ExprCall>;arg:ExpressionPtr;last:bool): void
    def abstract visitExprCallArgument(expr:smart_ptr<ExprCall>;arg:ExpressionPtr;last:bool) : ExpressionPtr
// null coaelescing
    def abstract preVisitExprNullCoalescing(expr:smart_ptr<ExprNullCoalescing>): void
    def abstract visitExprNullCoalescing(expr:smart_ptr<ExprNullCoalescing>) : ExpressionPtr
    def abstract preVisitExprNullCoalescingDefault(expr:smart_ptr<ExprNullCoalescing>;defval:ExpressionPtr): void
// at
    def abstract preVisitExprAt(expr:smart_ptr<ExprAt>): void
    def abstract visitExprAt(expr:smart_ptr<ExprAt>) : ExpressionPtr
    def abstract preVisitExprAtIndex(expr:smart_ptr<ExprAt>;index:ExpressionPtr): void
// safe at
    def abstract preVisitExprSafeAt(expr:smart_ptr<ExprSafeAt>): void
    def abstract visitExprSafeAt(expr:smart_ptr<ExprSafeAt>) : ExpressionPtr
    def abstract preVisitExprSafeAtIndex(expr:smart_ptr<ExprAt>;index:ExpressionPtr): void
// is
    def abstract preVisitExprIs(expr:smart_ptr<ExprIs>): void
    def abstract visitExprIs(expr:smart_ptr<ExprIs>) : ExpressionPtr
    def abstract preVisitExprIsType(expr:smart_ptr<ExprAt>;typeDecl:TypeDeclPtr): void
// op2
    def abstract preVisitExprOp2(expr:smart_ptr<ExprOp2>): void
    def abstract visitExprOp2(expr:smart_ptr<ExprOp2>) : ExpressionPtr
    def abstract preVisitExprOp2Right(expr:smart_ptr<ExprOp2>;right:ExpressionPtr): void
// op3
    def abstract preVisitExprOp3(expr:smart_ptr<ExprOp3>): void
    def abstract visitExprOp3(expr:smart_ptr<ExprOp3>) : ExpressionPtr
    def abstract preVisitExprOp3Left(expr:smart_ptr<ExprOp3>;left:ExpressionPtr): void
    def abstract preVisitExprOp3Right(expr:smart_ptr<ExprOp3>;right:ExpressionPtr): void
// copy
    def abstract preVisitExprCopy(expr:smart_ptr<ExprCopy>): void
    def abstract visitExprCopy(expr:smart_ptr<ExprCopy>) : ExpressionPtr
    def abstract preVisitExprCopyRight(expr:smart_ptr<ExprCopy>;right:ExpressionPtr): void
// move
    def abstract preVisitExprMove(expr:smart_ptr<ExprMove>): void
    def abstract visitExprMove(expr:smart_ptr<ExprMove>) : ExpressionPtr
    def abstract preVisitExprMoveRight(expr:smart_ptr<ExprMove>;right:ExpressionPtr) : void
// clone
    def abstract preVisitExprClone(expr:smart_ptr<ExprClone>) : void
    def abstract visitExprClone(expr:smart_ptr<ExprClone>) : ExpressionPtr
    def abstract preVisitExprCloneRight(expr:smart_ptr<ExprClone>;right:ExpressionPtr) : void
// with
    def abstract preVisitExprWith(expr:smart_ptr<ExprWith>) : void
    def abstract visitExprWith(expr:smart_ptr<ExprWith>) : ExpressionPtr
    def abstract preVisitExprWithBody(expr:smart_ptr<ExprWith>;right:ExpressionPtr) : void
// while
    def abstract preVisitExprWhile(expr:smart_ptr<ExprWhile>) : void
    def abstract visitExprWhile(expr:smart_ptr<ExprWhile>) : ExpressionPtr
    def abstract preVisitExprWhileBody(expr:smart_ptr<ExprWhile>;right:ExpressionPtr) : void
// try-catch
    def abstract preVisitExprTryCatch(expr:smart_ptr<ExprTryCatch>) : void
    def abstract visitExprTryCatch(expr:smart_ptr<ExprTryCatch>) : ExpressionPtr
    def abstract preVisitExprTryCatchCatch(expr:smart_ptr<ExprTryCatch>;right:ExpressionPtr) : void
// if-then-else
    def abstract preVisitExprIfThenElse(expr:smart_ptr<ExprIfThenElse>) : void
    def abstract visitExprIfThenElse(expr:smart_ptr<ExprIfThenElse>) : ExpressionPtr
    def abstract preVisitExprIfThenElseIfBlock(expr:smart_ptr<ExprIfThenElse>;ifBlock:ExpressionPtr) : void
    def abstract preVisitExprIfThenElseElseBlock(expr:smart_ptr<ExprIfThenElse>;elseBlock:ExpressionPtr) : void
// for
    def abstract preVisitExprFor(expr:smart_ptr<ExprFor>) : void
    def abstract visitExprFor(expr:smart_ptr<ExprFor>) : ExpressionPtr
    def abstract preVisitExprForVariable(expr:smart_ptr<ExprFor>;svar:VariablePtr;last:bool) : void
    def abstract visitExprForVariable(expr:smart_ptr<ExprFor>;svar:VariablePtr;last:bool) : VariablePtr
    def abstract preVisitExprForSource(expr:smart_ptr<ExprFor>;source:ExpressionPtr;last:bool) : void
    def abstract visitExprForSource(expr:smart_ptr<ExprFor>;source:ExpressionPtr;last:bool) : ExpressionPtr
    def abstract preVisitExprForStack(expr:smart_ptr<ExprFor>) : void
    def abstract preVisitExprForBody(expr:smart_ptr<ExprFor>) : void
// make variant
    def abstract preVisitExprMakeVariant(expr:smart_ptr<ExprMakeVariant>) : void
    def abstract visitExprMakeVariant(expr:smart_ptr<ExprMakeVariant>) : ExpressionPtr
    def abstract preVisitExprMakeVariantField(expr:smart_ptr<ExprMakeVariant>;index:int;decl:MakeFieldDeclPtr;last:bool) : void
    def abstract visitExprMakeVariantField(expr:smart_ptr<ExprMakeVariant>;index:int;decl:MakeFieldDeclPtr;last:bool) : MakeFieldDeclPtr
// make structure
    def abstract preVisitExprMakeStruct(expr:smart_ptr<ExprMakeStruct>) : void
    def abstract visitExprMakeStruct(expr:smart_ptr<ExprMakeStruct>) : ExpressionPtr
    def abstract preVisitExprMakeStructIndex(expr:smart_ptr<ExprMakeStruct>;index:int;last:bool) : void
    def abstract visitExprMakeStructIndex(expr:smart_ptr<ExprMakeStruct>;index:int;last:bool) : void
    def abstract preVisitExprMakeStructField(expr:smart_ptr<ExprMakeStruct>;index:int;decl:MakeFieldDeclPtr;last:bool) : void
    def abstract visitExprMakeStructField(expr:smart_ptr<ExprMakeStruct>;index:int;decl:MakeFieldDeclPtr;last:bool) : MakeFieldDeclPtr
// make array
    def abstract preVisitExprMakeArray(expr:smart_ptr<ExprMakeArray>) : void
    def abstract visitExprMakeArray(expr:smart_ptr<ExprMakeArray>) : ExpressionPtr
    def abstract preVisitExprMakeArrayIndex(expr:smart_ptr<ExprMakeArray>;index:int;init:ExpressionPtr;last:bool) : void
    def abstract visitExprMakeArrayIndex(expr:smart_ptr<ExprMakeArray>;index:int;init:ExpressionPtr;last:bool) : ExpressionPtr
// make tuple
    def abstract preVisitExprMakeTuple(expr:smart_ptr<ExprMakeTuple>) : void
    def abstract visitExprMakeTuple(expr:smart_ptr<ExprMakeTuple>) : ExpressionPtr
    def abstract preVisitExprMakeTupleIndex(expr:smart_ptr<ExprMakeTuple>;index:int;init:ExpressionPtr;last:bool) : void
    def abstract visitExprMakeTupleIndex(expr:smart_ptr<ExprMakeTuple>;index:int;init:ExpressionPtr;last:bool) : ExpressionPtr
// array comprehension
    def abstract preVisitExprArrayComprehension(expr:smart_ptr<ExprArrayComprehension>) : void
    def abstract visitExprArrayComprehension(expr:smart_ptr<ExprArrayComprehension>) : ExpressionPtr
    def abstract preVisitExprArrayComprehensionSubexpr(expr:smart_ptr<ExprArrayComprehension>;subexrp:ExpressionPtr) : void
    def abstract preVisitExprArrayComprehensionWhere(expr:smart_ptr<ExprArrayComprehension>;filter:ExpressionPtr) : void
// type info
    def abstract preVisitExprTypeInfo(expr:smart_ptr<ExprTypeInfo>) : void
    def abstract visitExprTypeInfo(expr:smart_ptr<ExprTypeInfo>) : ExpressionPtr
// ptr to ref
    def abstract preVisitExprPtr2Ref(expr:smart_ptr<ExprPtr2Ref>) : void
    def abstract visitExprPtr2Ref(expr:smart_ptr<ExprPtr2Ref>) : ExpressionPtr
// label
    def abstract preVisitExprLabel(expr:smart_ptr<ExprLabel>) : void
    def abstract visitExprLabel(expr:smart_ptr<ExprLabel>) : ExpressionPtr
// goto
    def abstract preVisitExprGoto(expr:smart_ptr<ExprGoto>) : void
    def abstract visitExprGoto(expr:smart_ptr<ExprGoto>) : ExpressionPtr
// ref to value
    def abstract preVisitExprRef2Value(expr:smart_ptr<ExprRef2Value>) : void
    def abstract visitExprRef2Value(expr:smart_ptr<ExprRef2Value>) : ExpressionPtr
// ref to ptr
    def abstract preVisitExprRef2Ptr(expr:smart_ptr<ExprRef2Ptr>) : void
    def abstract visitExprRef2Ptr(expr:smart_ptr<ExprRef2Ptr>) : ExpressionPtr
// @@
    def abstract preVisitExprAddr(expr:smart_ptr<ExprAddr>) : void
    def abstract visitExprAddr(expr:smart_ptr<ExprAddr>) : ExpressionPtr
// assert / verify
    def abstract preVisitExprAssert(expr:smart_ptr<ExprAssert>) : void
    def abstract visitExprAssert(expr:smart_ptr<ExprAssert>) : ExpressionPtr
// static_assert
    def abstract preVisitExprStaticAssert(expr:smart_ptr<ExprStaticAssert>) : void
    def abstract visitExprStaticAssert(expr:smart_ptr<ExprStaticAssert>) : ExpressionPtr
// debug
    def abstract preVisitExprDebug(expr:smart_ptr<ExprDebug>) : void
    def abstract visitExprDebug(expr:smart_ptr<ExprDebug>) : ExpressionPtr
// invoke
    def abstract preVisitExprInvoke(expr:smart_ptr<ExprInvoke>) : void
    def abstract visitExprInvoke(expr:smart_ptr<ExprInvoke>) : ExpressionPtr
// erase
    def abstract preVisitExprErase(expr:smart_ptr<ExprErase>) : void
    def abstract visitExprErase(expr:smart_ptr<ExprErase>) : ExpressionPtr
// find
    def abstract preVisitExprFind(expr:smart_ptr<ExprFind>) : void
    def abstract visitExprFind(expr:smart_ptr<ExprFind>) : ExpressionPtr
// key exists
    def abstract preVisitExprKeyExists(expr:smart_ptr<ExprKeyExists>) : void
    def abstract visitExprKeyExists(expr:smart_ptr<ExprKeyExists>) : ExpressionPtr
// ascend
    def abstract preVisitExprAscend(expr:smart_ptr<ExprAscend>) : void
    def abstract visitExprAscend(expr:smart_ptr<ExprAscend>) : ExpressionPtr
// cast
    def abstract preVisitExprCast(expr:smart_ptr<ExprCast>) : void
    def abstract visitExprCast(expr:smart_ptr<ExprCast>) : ExpressionPtr
// delete
    def abstract preVisitExprDelete(expr:smart_ptr<ExprDelete>) : void
    def abstract visitExprDelete(expr:smart_ptr<ExprDelete>) : ExpressionPtr
// var
    def abstract preVisitExprVar(expr:smart_ptr<ExprVar>) : void
    def abstract visitExprVar(expr:smart_ptr<ExprVar>) : ExpressionPtr
// field
    def abstract preVisitExprField(expr:smart_ptr<ExprField>) : void
    def abstract visitExprField(expr:smart_ptr<ExprField>) : ExpressionPtr
// safe field
    def abstract preVisitExprSafeField(expr:smart_ptr<ExprSafeField>) : void
    def abstract visitExprSafeField(expr:smart_ptr<ExprSafeField>) : ExpressionPtr
// swizzle
    def abstract preVisitExprSwizzle(expr:smart_ptr<ExprSwizzle>) : void
    def abstract visitExprSwizzle(expr:smart_ptr<ExprSwizzle>) : ExpressionPtr
// is variant
    def abstract preVisitExprIsVariant(expr:smart_ptr<ExprIsVariant>) : void
    def abstract visitExprIsVariant(expr:smart_ptr<ExprIsVariant>) : ExpressionPtr
// as variant
    def abstract preVisitExprAsVariant(expr:smart_ptr<ExprAsVariant>) : void
    def abstract visitExprAsVariant(expr:smart_ptr<ExprAsVariant>) : ExpressionPtr
// safe as variant
    def abstract preVisitExprSafeAsVariant(expr:smart_ptr<ExprSafeAsVariant>) : void
    def abstract visitExprSafeAsVariant(expr:smart_ptr<ExprSafeAsVariant>) : ExpressionPtr
// op1
    def abstract preVisitExprOp1(expr:smart_ptr<ExprOp1>) : void
    def abstract visitExprOp1(expr:smart_ptr<ExprOp1>) : ExpressionPtr
// return
    def abstract preVisitExprReturn(expr:smart_ptr<ExprReturn>) : void
    def abstract visitExprReturn(expr:smart_ptr<ExprReturn>) : ExpressionPtr
// yield
    def abstract preVisitExprYield(expr:smart_ptr<ExprYield>) : void
    def abstract visitExprYield(expr:smart_ptr<ExprYield>) : ExpressionPtr
// break
    def abstract preVisitExprBreak(expr:smart_ptr<ExprBreak>) : void
    def abstract visitExprBreak(expr:smart_ptr<ExprBreak>) : ExpressionPtr
// continue
    def abstract preVisitExprContinue(expr:smart_ptr<ExprContinue>) : void
    def abstract visitExprContinue(expr:smart_ptr<ExprContinue>) : ExpressionPtr
// make block
    def abstract preVisitExprMakeBlock(expr:smart_ptr<ExprMakeBlock>) : void
    def abstract visitExprMakeBlock(expr:smart_ptr<ExprMakeBlock>) : ExpressionPtr
// make generator
    def abstract preVisitExprMakeGenerator(expr:smart_ptr<ExprMakeGenerator>) : void
    def abstract visitExprMakeGenerator(expr:smart_ptr<ExprMakeGenerator>) : ExpressionPtr
// memzero
    def abstract preVisitExprMemZero(expr:smart_ptr<ExprMemZero>) : void
    def abstract visitExprMemZero(expr:smart_ptr<ExprMemZero>) : ExpressionPtr

