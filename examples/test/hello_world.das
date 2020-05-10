//options log = true
options rtti = true

require ast
require rtti

struct Foo
    a : int
    b : array<Foo?>

def add(a,b:int)
    return a + b

[unsafe]
def make_visitor ( someClass ) : smart_ptr<VisitorAdapter>
    static_if typeinfo(is_class someClass)
        let classPtr = addr(someClass)
        let classInfo = class_info(someClass)
        return make_visitor(classPtr, classInfo)
    else
        concept_assert(false,"can't make visitor of non-class")
        return reinterpret<smart_ptr<VisitorAdapter>> null

typedef
    ExpressionPtr = smart_ptr<Expression>

class AstVisitor
    def preVisitExpression(expr:ExpressionPtr)
        print("preVisitExpression({describe(expr)})\n")
    def visitExpression(expr:ExpressionPtr)
        print("visitExpression({describe(expr)})\n")
        return expr

[export]
def test
    var astVisitor = AstVisitor()
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
