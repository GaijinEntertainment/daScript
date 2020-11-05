options indenting = 4
options no_aot = true

module template

require ast
require daslib/ast_boost

class Template
    var2expr : table<string; smart_ptr<Expression>>
    def replaceVariable ( name:string; var expr : smart_ptr<ast::Expression> )
        var2expr[name] <- expr
    def renameVariable ( name, newName:string )
        self->replaceVariable(name) <| ExpressionPtr(new [[ExprVar() name:=newName]])

class TemplateVisitor : AstVisitor
    rules : Template?
    def TemplateVisitor ( r : Template? )
        rules = r
    def override visitExprVar(expr:smart_ptr<ExprVar>) : ExpressionPtr
        let vn = "{expr.name}"
        if key_exists(rules.var2expr,vn)
            return <- clone_expression(rules.var2expr[vn])
        return expr


def apply_template ( rules : Template?; var expr : smart_ptr<Expression> ) : smart_ptr<Expression>
    var astVisitor = new TemplateVisitor(rules)
    var astVisitorAdapter <- make_visitor(*astVisitor)
    var cexpr <- clone_expression(expr)
    visit(cexpr, astVisitorAdapter)
    unsafe
        delete astVisitorAdapter
        delete astVisitor
    return <- cexpr

def unquote_block ( expr:ExpressionPtr ) : smart_ptr<ExprBlock>
    unsafe
        var mkb <- reinterpret<smart_ptr<ExprMakeBlock>>(expr)
        var blk <- mkb._block
        return reinterpret<smart_ptr<ExprBlock>>(blk)
