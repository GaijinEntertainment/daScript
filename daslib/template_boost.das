options indenting = 4
options no_aot = true

module template_boost shared

require ast
require daslib/ast_boost

struct Template
    var2expr : table<string; smart_ptr<Expression>>

def replaceVariable ( var self:Template; name:string; var expr : smart_ptr<ast::Expression> )
    self.var2expr[name] <- expr

def renameVariable ( var self:Template; name, newName:string )
    self |> replaceVariable(name) <| ExpressionPtr(new [[ExprVar() name:=newName]])

class TemplateVisitor : AstVisitor
    rules : Template?
    def TemplateVisitor ( r : Template? )
        rules = r
    def override visitExprVar(expr:smart_ptr<ExprVar>) : ExpressionPtr
        let vn = "{expr.name}"
        if key_exists(rules.var2expr,vn)
            return <- clone_expression(rules.var2expr[vn])
        return expr

def apply_template ( var rules:Template; at:LineInfo; var expr:smart_ptr<Expression> )
    var prules : Template?
    unsafe
        prules = addr(rules)
    var astVisitor = new TemplateVisitor(prules)
    var astVisitorAdapter <- make_visitor(*astVisitor)
    expr |> visit(astVisitorAdapter)
    expr |> force_at(at)
    unsafe
        delete astVisitor
    astVisitorAdapter := null

def unquote_block ( expr:ExpressionPtr ) : smart_ptr<ExprBlock>
    assert(expr is ExprMakeBlock)
    unsafe
        var mkb <- reinterpret<smart_ptr<ExprMakeBlock>>(expr)
        var blk := mkb._block
        return reinterpret<smart_ptr<ExprBlock>>(blk)

def move_unquote_block ( var expr:ExpressionPtr& ) : smart_ptr<ExprBlock>
    assert(expr is ExprMakeBlock)
    unsafe
        var mkb <- reinterpret<smart_ptr<ExprMakeBlock>>(expr)
        var blk := mkb._block
        var res <- reinterpret<smart_ptr<ExprBlock>>(blk)
        mkb := null
        return res

[private]
def add_global_var_any ( mod:Module?; vname:string; vat:LineInfo; vflag:TypeDeclFlags; var value:ExpressionPtr )
    var vvar <- new [[Variable() at=vat,
        name:=vname,
        _type <- new [[TypeDecl() at=vat,
            baseType = Type autoinfer,
            flags = vflag
        ]],
        init <- value
    ]]
    if !add_variable(mod, vvar)
        panic("can't add global variable {vname}")

def add_global_var ( mod:Module?; vname:string; vat:LineInfo; var value:ExpressionPtr )
    add_global_var_any(mod, vname, vat, TypeDeclFlags removeConstant, value)

def add_global_let ( mod:Module?; vname:string; vat:LineInfo; var value:ExpressionPtr )
    add_global_var_any(mod, vname, vat, TypeDeclFlags constant, value)

