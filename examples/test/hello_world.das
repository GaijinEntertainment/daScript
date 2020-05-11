//options log = true
options rtti = true

require ast
require rtti

typedef
    ExpressionPtr = smart_ptr<Expression>
    ProgramPtr = smart_ptr<Program>
    TypeDeclPtr = smart_ptr<TypeDecl>
    EnumerationPtr = smart_ptr<Enumeration>
    StructurePtr = smart_ptr<Structure>

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

class AstVisitor
// program
    def preVisitProgram(prog:ProgramPtr) : void
        pass
    def visitProgram(porg:ProgramPtr) : void
        pass
// type
    def preVisitTypeDecl(typ:TypeDeclPtr) : void
        pass
    def visitTypeDecl(typ:TypeDeclPtr) : TypeDeclPtr
        return typ
// type alias
    def preVisitAlias(name:das_string;typ:TypeDeclPtr) : void
        pass
    def visitAlias(name:das_string;typ:TypeDeclPtr)
        return typ
    def preVisitExpression(expr:ExpressionPtr) : void
        pass
    def visitExpression(expr:ExpressionPtr) : ExpressionPtr
        return expr
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

// example

class PrintVisitor : AstVisitor
// program
    def override preVisitProgram(prog:ProgramPtr)
        print("// program\n")
    def override visitProgram(prog:ProgramPtr)
        print("// end program\n")
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
    def override preVisitAlias(name:das_string;typ:TypeDeclPtr)
        print("typedef\n\t{name} = {describe([decl=typ,extra=false])}\n\n")
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
        print("{decl.name} : {describe([decl=decl.typeDecl,extra=false])}")
        if decl.flags.parentType
            print(" /* from {str.parent.name} */")
        if decl.init != null
            print("{decl.flags.moveSemantic ? " <- " : " = "}")
    def override visitStructureField(str:StructurePtr;decl:FieldDeclaration;last:bool) : void
        print("\n")
    def override visitStructure(str:StructurePtr) : StructurePtr
        print("\n")
        return str

// every expression there is

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
