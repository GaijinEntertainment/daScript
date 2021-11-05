options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true

module templates_boost shared public

require ast
require daslib/ast_boost

struct Template
    kaboomVar : table<string; tuple<prefix:string;suffix:string> >
    var2name : table<string; string>
    var2expr : table<string; smart_ptr<Expression>>
    type2type : table<string; string>
    blockArgName : table<string; string>
    annArg : table<string; lambda<(var ann: AnnotationDeclaration):void> >

def kaboomVarField ( var self:Template; name,prefix,suffix:string )
    self.kaboomVar[name] = [[auto prefix,suffix]]

def replaceVariable ( var self:Template; name:string; var expr : smart_ptr<ast::Expression> )
    self.var2expr[name] <- expr

def renameVariable ( var self:Template; name, newName:string )
    self.var2name[name] = newName

def replaceType ( var self:Template; name,newName:string )
    self.type2type[name] = newName

def replaceAnnotationArgument ( var self:Template; name:string; var cb:lambda<(var ann: AnnotationDeclaration):void> )
    self.annArg[name] <- cb

def replaceBlockArgument ( var self:Template; name,newName:string )
    self.blockArgName[name] = newName

class private TemplateVisitor : AstVisitor
    rules : Template?
    def TemplateVisitor ( r : Template? )
        rules = r
    def override visitExprField(expr:smart_ptr<ExprField>) : ExpressionPtr
        var ev = expr.value ?as ExprVar
        if ev == null
            ev = (expr.value ?as ExprRef2Value) ?as ExprVar
        if ev != null
            let kv = string(ev.name)
            if rules.kaboomVar |> key_exists(kv)
                let ps = rules.kaboomVar[kv]
                var nv <- new [[ExprVar() at=expr.at, name:="{ps.prefix}{expr.name}{ps.suffix}"]]
                return nv
        return expr
    def override visitExprLet( var expr:smart_ptr<ExprLet> ) : ExpressionPtr
        for v in expr.variables
            let vn = string(v.name)
            if key_exists(rules.var2name,vn)
                v.name := rules.var2name[vn]
        return expr
    def override visitExprVar(var expr:smart_ptr<ExprVar>) : ExpressionPtr
        let vn = string(expr.name)
        if key_exists(rules.var2name,vn)
            expr.name := rules.var2name[vn]
        if key_exists(rules.var2expr,vn)
            var rexpr <- clone_expression(rules.var2expr[vn])
            rexpr.at = expr.at
            return rexpr
        return expr
    def replaceAlias ( var typ:TypeDeclPtr )
        if typ.baseType==Type alias
            let ta = string(typ.alias)
            if key_exists(rules.type2type, ta)
                typ.alias := rules.type2type[ta]
        if typ.firstType != null
            self->replaceAlias(typ.firstType)
        if typ.secondType != null
            self->replaceAlias(typ.secondType)
        for arg in typ.argTypes
            self->replaceAlias(arg)
    def override visitTypeDecl(var typ:TypeDeclPtr) : TypeDeclPtr
        var tyyp <- clone_type(typ)
        self->replaceAlias(tyyp)
        return tyyp
    def override preVisitExprBlock(var blk:smart_ptr<ExprBlock>)
        if !blk.blockFlags.isClosure
            return
        for arg in blk.arguments
            let vn = "{arg.name}"
            if key_exists(rules.blockArgName, vn)
                arg.name := rules.blockArgName[vn]
        for ann in blk.annotations
            rules.annArg |> find_if_exists("{ann.annotation.name}") <| $(cb)
                (*cb) |> invoke(*ann)


def apply_template ( var rules:Template; at:LineInfo; var expr:smart_ptr<Expression>; forceAt:bool = true )
    var prules : Template?
    unsafe
        prules = addr(rules)
    var astVisitor = new TemplateVisitor(prules)
    var astVisitorAdapter <- make_visitor(*astVisitor)
    expr |> visit(astVisitorAdapter)
    if forceAt
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

def private add_global_var_any ( mod:Module?; vname:string; vat:LineInfo; vflag:TypeDeclFlags; var value:ExpressionPtr; priv:bool )
    var vvar <- new [[Variable() at=vat,
        name:=vname,
        _type <- new [[TypeDecl() at=vat,
            baseType = Type autoinfer,
            flags = vflag
        ]],
        init <- value
    ]]
    vvar.flags |= VariableFlags generated
    if priv
        vvar.flags |= VariableFlags private_variable
    return add_variable(mod, vvar)

def add_global_var ( mod:Module?; vname:string; vat:LineInfo; var value:ExpressionPtr )
    return add_global_var_any(mod, vname, vat, TypeDeclFlags removeConstant, value, false)

def add_global_var ( mod:Module?; vname:string; var typ:TypeDeclPtr; vat:LineInfo; priv:bool; blk:block<(var v : VariablePtr):void> )
    var vvar <- new [[Variable() at=vat,
        name:=vname,
        _type <- typ
    ]]
    vvar.flags |= VariableFlags generated
    if priv
        vvar.flags |= VariableFlags private_variable
    invoke(blk, vvar)
    return add_variable(mod, vvar)

def add_global_var ( mod:Module?; vname:string; var typ:TypeDeclPtr; vat:LineInfo; priv:bool )
    return add_global_var(mod,vname,typ,vat,priv) <| $ [unused_argument(v)]( var v )
        pass

def add_global_let ( mod:Module?; vname:string; vat:LineInfo; var value:ExpressionPtr )
    return add_global_var_any(mod, vname, vat, TypeDeclFlags constant, value, false)

def add_global_private_var ( mod:Module?; vname:string; vat:LineInfo; var value:ExpressionPtr )
    return add_global_var_any(mod, vname, vat, TypeDeclFlags removeConstant, value, true)

def add_global_private_let ( mod:Module?; vname:string; vat:LineInfo; var value:ExpressionPtr )
    return add_global_var_any(mod, vname, vat, TypeDeclFlags constant, value, true)

def make_unique_private_name ( prefix:string; vat:LineInfo )
    return "{prefix}_{vat.line}_{vat.column}"

def apply_template ( at:LineInfo; var expr:smart_ptr<Expression>; blk:block<(var rules:Template):void> )
    var rules : Template
    invoke(blk, rules)
    apply_template(rules, at, expr, true)
    delete rules

def apply_template ( var expr:smart_ptr<Expression>; blk:block<(var rules:Template):void> )
    var rules : Template
    invoke(blk, rules)
    apply_template(rules,expr.at, expr, false)
    delete rules