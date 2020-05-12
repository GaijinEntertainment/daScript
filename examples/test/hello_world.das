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
    def override preVisitStructure(str:StructurePtr) : void
        if str.annotations.length != 0
            print("[{describe(str.annotations)}]\n")
        print("{str.flags.isClass ? "class" : "struct"} {str.name}\n")
    def override preVisitStructureField(str:StructurePtr;decl:FieldDeclaration;last:bool) : void
        print("\t")
        if decl.annotation.length != 0
            print("[{describe(decl.annotation)}] ")
        print("{decl.name} : {describe([decl=decl.typeDecl,extra=extraTypeInfo])}")
        if decl.flags.parentType
            print(" /* from {str.parent.name} */")
        if decl.init != null
            print("{decl.flags.moveSemantic ? " <- " : " = "}")
    def override visitStructureField(str:StructurePtr;decl:FieldDeclaration;last:bool) : void
        print("\n")
    def override visitStructure(str:StructurePtr) : StructurePtr
        print("\n")
        return str
// function
    def override preVisitFunction(fun:FunctionPtr) : void
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
    def override preVisitFunctionBody(fun:FunctionPtr) : void
        if fun.arguments.length != 0
            print(" ) ")
        if fun.result != null & !fun.result.isVoid
            print(" : {describe([decl=fun.result,extra=extraTypeInfo])}")
        print("\n")
    def override visitFunction(fun:FunctionPtr) : FunctionPtr
        print("\n")
        return fun
    def override preVisitFunctionArgument(fun:FunctionPtr;arg:VariablePtr;last:bool) : void
        if arg.annotation.length != 0
            print("[{describe(arg.annotation)}] ")
        if !arg.typeDecl.flags.constant
            print("var ")
        if arg.isAccessUnused
            print(" /*unsued*/ ")
        print("{arg.name} : {describe([decl=arg.typeDecl,extra=extraTypeInfo])}")
    def override visitFunctionArgument(fun:FunctionPtr;arg:VariablePtr;last:bool) : VariablePtr
        if !last
            print("; ")
        return arg
    def override preVisitFunctionArgumentInit(fun:FunctionPtr;arg:VariablePtr;value:ExpressionPtr) : void
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
    def override visitExprBlock(blk:smart_ptr<ExprBlock>) : ExpressionPtr
        tab --
        if printCStyle | blk.blockFlags.isClosure
            print("{repeat("\t",tab)}\}\n")
        return ExpressionPtr(blk)
    def override preVisitExprBlockExpression(blk:smart_ptr<ExprBlock>;expr:ExpressionPtr)
        print("{repeat("\t",tab)}")
    def override visitExprBlockExpression(blk:smart_ptr<ExprBlock>;expr:ExpressionPtr) : ExpressionPtr
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
    def override visitExprBlockFinalExpression(blk:smart_ptr<ExprBlock>;expr:ExpressionPtr) : ExpressionPtr
        if printCStyle | blk.blockFlags.isClosure
            print(";")
        self->newLine()
        return expr
// let
    def override preVisitExprLet(expr:smart_ptr<ExprLet>) : void
        var isLet = true
        for pv in expr.variables
            if pv.typeDecl!=null & pv.typeDecl.flags.constant
                isLet = false
                break
        print("{isLet ? "let" : "var"} {expr.inScope ? "in_scope " : ""}")
    def override preVisitExprLetVariable(expr:smart_ptr<ExprLet>;arg:VariablePtr;lastArg:bool) : void
        if arg.isAccessUnused
            print(" /*unused*/ ")
        print("{arg.name} : {describe([decl=arg.typeDecl,extra=extraTypeInfo])}")
    def override visitExprLetVariable(expr:smart_ptr<ExprLet>;arg:VariablePtr;lastArg:bool) : VariablePtr
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


typedef
    Integer = int

enum FooBar
    Foo
    Bar = 13

[comment(x=5,msg="haha")]
struct Foo
    [[do_not_delete,temp=13]] a : int
    b : array<Foo?>

def add(a,b:int)
    return a + b

[export]
def test
    var astVisitor = PrintVisitor()
    var astVisitorAdapter <- make_visitor(astVisitor)
    ast_visit(ast_this_program(), astVisitorAdapter)

    /*
    // ast_typedecl
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
