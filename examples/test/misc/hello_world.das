require daslib/ast_boost

def visit_expression ( var expr:ExpressionPtr&; var adapter:smart_ptr<VisitorAdapter> )
    unsafe
        // note: visit returns smart_ptr_raw, so we need to be careful with lifetime here
        var newExpr <- expr |> visit(adapter)
        if expr != newExpr
            expr := null
        expr <- newExpr

class TestVisitor : AstVisitor
    i = 1
    def override visitExprBreak(var expr:smart_ptr<ExprBreak>) : ExpressionPtr
        if i==1
            return <- expr
        return <- expr

[export]
def main
    var inscope test <- quote <|
        for x in range(10)
            break
    var astVisitor = new TestVisitor()
    var inscope astVisitorAdapter <- make_visitor(*astVisitor)
    visit_expression(test, astVisitorAdapter)
    unsafe
        delete astVisitorAdapter
    print("{describe(test)}\n")


