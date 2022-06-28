options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true

module templates_boost shared public

require ast
require daslib/ast_boost
require daslib/algorithm

struct Template
    //! This structure contains collection of subsitution rules for a template.
    kaboomVar : table<string; tuple<prefix:string;suffix:string> >
    call2name : table<string; string>
    field2name : table<string; string>
    var2name : table<string; string>
    var2expr : table<string; smart_ptr<Expression>>
    var2exprList : table<string; array<smart_ptr<Expression>>>
    type2type : table<string; string>
    type2etype : table<string; TypeDeclPtr>
    blockArgName : table<string; string>
    annArg : table<string; lambda<(var ann: AnnotationDeclaration):void> >

def kaboomVarField ( var self:Template; name,prefix,suffix:string )
    //! Adds a rule to to the template to replace a variable field access with a prefix and suffix.
    //! I.e. foo.bar into prefix + bar + suffix
    self.kaboomVar[name] = [[auto prefix,suffix]]

def replaceVariable ( var self:Template; name:string; var expr : smart_ptr<ast::Expression> )
    //! Adds a rule to the template to replace a variable with an expression.
    self.var2expr[name] <- expr

def replaceVariableWithList ( var self:Template; name:string; var expr : array<smart_ptr<ast::Expression>> )
    //! Adds a rule to the template to replace a variable with an expression list.
    self.var2exprList[name] <- expr

def renameVariable ( var self:Template; name, newName:string )
    //! Adds a rule to the template to rename a variable.
    self.var2name[name] = newName

def renameField ( var self:Template; name, newName:string )
    //! Adds a rule to the template to rename any field lookup (., ?., as, is, etc)
    self.field2name[name] = newName

def replaceType ( var self:Template; name,newName:string )
    //! Adds a rule to the template to replace a type alias with another type alias.
    self.type2type[name] = newName

def replaceTypeWithTypeDecl ( var self:Template; name:string; var expr : TypeDeclPtr  )
    //! Adds a rule to the template to replace a type alias with another type alias.
    self.type2etype[name] <- expr

def replaceAnnotationArgument ( var self:Template; name:string; var cb:lambda<(var ann: AnnotationDeclaration):void> )
    //! Adds a rule to the template to replace an annotation argument with the result of a callback.
    self.annArg[name] <- cb

def replaceBlockArgument ( var self:Template; name,newName:string )
    //! Adds a rule to the template to rename a block argument.
    self.blockArgName[name] = newName

def renameCall ( var self:Template; name, newName:string )
    //! Adds a rule to the template to rename a call.
    self.call2name[name] = newName

class private TemplateVisitor : AstVisitor
    rules : Template?
    def TemplateVisitor ( r : Template? )
        rules = r
    def override visitExprAsVariant(var expr:smart_ptr<ExprAsVariant>) : ExpressionPtr
        let fname = string(expr.name)
        if rules.field2name |> key_exists(fname)
            expr.name := rules.field2name[fname]
        return expr
    def override visitExprSafeAsVariant(var expr:smart_ptr<ExprSafeAsVariant>) : ExpressionPtr
        let fname = string(expr.name)
        if rules.field2name |> key_exists(fname)
            expr.name := rules.field2name[fname]
        return expr
    def override visitExprSafeField(var expr:smart_ptr<ExprSafeField>) : ExpressionPtr
        let fname = string(expr.name)
        if rules.field2name |> key_exists(fname)
            expr.name := rules.field2name[fname]
        return expr
    def override visitExprField(var expr:smart_ptr<ExprField>) : ExpressionPtr
        let fname = string(expr.name)
        if rules.field2name |> key_exists(fname)
            expr.name := rules.field2name[fname]
        var ev = expr.value ?as ExprVar
        if ev == null
            ev = (expr.value ?as ExprRef2Value) ?as ExprVar
        if ev != null
            let kv = string(ev.name)
            if rules.kaboomVar |> key_exists(kv)
                let ps = rules.kaboomVar[kv]
                let nv <- new [[ExprVar() at=expr.at, name:="{ps.prefix}{expr.name}{ps.suffix}"]]
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
    def replaceAlias ( var typ:TypeDeclPtr& )
        if typ.baseType==Type alias
            let ta = string(typ.alias)
            if key_exists(rules.type2etype, ta)
                typ := null
                unsafe
                    typ <- clone_type(rules.type2etype[ta])
            elif key_exists(rules.type2type, ta)
                typ.alias := rules.type2type[ta]
        if typ.firstType != null
            self->replaceAlias(typ.firstType)
        if typ.secondType != null
            self->replaceAlias(typ.secondType)
        for arg in typ.argTypes
            self->replaceAlias(arg)
    def override visitTypeDecl(var typ:TypeDeclPtr) : TypeDeclPtr
        var tyyp <- clone_type(typ)
        replaceAlias(tyyp)
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
    def preVisitAnythingCall(var arguments:dasvector`smart_ptr`Expression) : void
        if length(rules.var2exprList)==0
            return
        let nargs = arguments.length
        if nargs == 0
            return
        for ai in range(nargs)
            let rai = nargs - ai - 1
            assume arg = arguments[rai]
            if arg is ExprVar
                let vname = string((arg as ExprVar).name)
                if key_exists(rules.var2exprList, vname)
                    var new_args <- [{for a in rules.var2exprList[vname]; clone_expression(a)}]
                    new_args |> reverse
                    arguments |> erase(rai)
                    for na in new_args
                        arguments |> emplace(na, rai)
                    delete new_args
    def override preVisitExprLooksLikeCall(var expr:smart_ptr<ExprLooksLikeCall>): void
        let cname = string(expr.name)
        if rules.call2name |> key_exists(cname)
            expr.name := rules.call2name[cname]
        preVisitAnythingCall(expr.arguments)
    def override preVisitExprCall(var expr:smart_ptr<ExprCall>): void
        let cname = string(expr.name)
        if rules.call2name |> key_exists(cname)
            expr.name := rules.call2name[cname]
        preVisitAnythingCall(expr.arguments)
    def override preVisitExprMakeArray(var expr:smart_ptr<ExprMakeArray>): void
        preVisitAnythingCall(expr.values)


def apply_template ( var rules:Template; at:LineInfo; var expr:smart_ptr<Expression>; forceAt:bool = true )
    //! Applies the template to the given expression. If `forceAt` is set, the resulting expression will have the same line info as 'at'.
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
    //! Returns the corresponding block subexpression expression form the ExprMakeBlock.
    assert(expr is ExprMakeBlock)
    unsafe
        let mkb <- reinterpret<smart_ptr<ExprMakeBlock>>(expr)
        let blk := mkb._block
        return reinterpret<smart_ptr<ExprBlock>>(blk)

def move_unquote_block ( var expr:ExpressionPtr& ) : smart_ptr<ExprBlock>
    //! Moves the corresponding block subexpression expression form the ExprMakeBlock.
    assert(expr is ExprMakeBlock)
    unsafe
        var mkb <- reinterpret<smart_ptr<ExprMakeBlock>>(expr)
        let blk := mkb._block
        let res <- reinterpret<smart_ptr<ExprBlock>>(blk)
        mkb := null
        return res

def make_expression_block ( var exprs:array<ExpressionPtr> ) : smart_ptr<ExprBlock>
    //! Create ExprBlock and move all expressions from expr to the list of the block.
    var blk <- new [[ExprBlock()]]
    for e in exprs
        blk.list |> emplace(e)
    delete exprs
    return blk

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
    //! Adds global variable to the module, given name and initial value.
    //! Global variables type is would be inferred from the initial value.
    //! `priv` specifies if the variable is private to the block.
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
    //! Add global variable to the module, given name and initial value.
    //! Variable type will be constant.
    return add_global_var_any(mod, vname, vat, TypeDeclFlags constant, value, false)

def add_global_private_var ( mod:Module?; vname:string; vat:LineInfo; var value:ExpressionPtr )
    //! Add global variable to the module, given name and initial value.
    //! It will be private.
    return add_global_var_any(mod, vname, vat, TypeDeclFlags removeConstant, value, true)

def add_global_private_let ( mod:Module?; vname:string; vat:LineInfo; var value:ExpressionPtr )
    //! Add global variable to the module, given name and initial value.
    //! It will be private, and type will be constant.
    return add_global_var_any(mod, vname, vat, TypeDeclFlags constant, value, true)

def make_unique_private_name ( prefix:string; vat:LineInfo )
    //! Generates unique private name for the variable, given prefix and line info.
    //!
    //! .. warning::
    //! The assumption is that line info is unique for the context of the unique name generation.
    //! If it is not, additional measures must be taken to ensure uniqueness of prefix.
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

class private RemoveDerefVisitor : AstVisitor
    vname : string
    def RemoveDerefVisitor ( n : string )
        vname = n
    def override visitExprRef2Value(expr:smart_ptr<ExprRef2Value>) : ExpressionPtr
        if expr.subexpr is ExprVar
            let vvar = expr.subexpr as ExprVar
            if "{vvar.name}"==vname
                return clone_expression(expr.subexpr)
        return expr

def public remove_deref ( varname : string; var expr:smart_ptr<Expression> )
    //! Removes dereferences of the variable `varname` from the expression.
    //! This is typically used when replacing 'workhorse' variable with constant.
    var astVisitor = new RemoveDerefVisitor(varname)
    var astVisitorAdapter <- make_visitor(*astVisitor)
    expr |> visit(astVisitorAdapter)
    unsafe
        delete astVisitor
    astVisitorAdapter := null

def public add_array_ptr_ref ( var a : array<smart_ptr<auto(TT)>> ) : array<smart_ptr<TT>>
    var q := a
    return <- q

class private QRulesVisitor : AstVisitor
    tag_index : int = 0
    failed : bool = false
    [[do_not_delete]] rblock : ExprBlock?
    def QRulesVisitor ( rb:ExprBlock? )
        rblock = rb
    def override visitTypeDecl(var typ:TypeDeclPtr) : TypeDeclPtr
        if (typ.baseType == Type alias) && (typ.flags.isTag)
            if typ.firstType==null || typ.firstType.dimExpr.length!=1
                compiling_program() |> macro_error(typ.at,"unsupported type tag")
                failed = true
                return typ
            let new_tag_name = "temp`type`{tag_index++}`{typ.at.line}`{typ.at.column}"
            var ntyp <- new [[TypeDecl() at=typ.at,
                baseType = Type alias,
                alias := new_tag_name
            ]]
            rblock.list |> emplace_new <| new [[ExprCall() at=typ.at,
                name:="replaceTypeWithTypeDecl",
                arguments:=[{ExpressionPtr
                    new [[ExprVar() at=typ.at, name:="rules"]];
                    new [[ExprConstString() at=typ.at, value:=new_tag_name]];
                    new [[ExprCall() at=typ.at, name:="add_ptr_ref",
                        arguments := [{ExpressionPtr[1] clone_expression(typ.firstType.dimExpr[0]) }]
                    ]]
                }]]]
            return <- ntyp

        return typ
    def override visitExprTag ( expr:smart_ptr<ExprTag> ) : ExpressionPtr
        var new_tag_name = ""
        if expr.name=="c"
            new_tag_name = "temp`call`{tag_index++}`{expr.at.line}`{expr.at.column}"
        elif expr.name=="f"
            new_tag_name = "temp`field`{tag_index++}`{expr.at.line}`{expr.at.column}"
        else
            new_tag_name = "temp`var`{tag_index++}`{expr.at.line}`{expr.at.column}"
        // print("replacing {new_tag_name} <- {describe(expr.subexpr)}\n")
        // deppending on a tag now?
        if expr.name=="v"   // value
            rblock.list |> emplace_new <| new [[ExprCall() at=expr.at,
                name:="replaceVariable",
                arguments:=[{ExpressionPtr
                    new [[ExprVar() at=expr.at, name:="rules"]];
                    new [[ExprConstString() at=expr.at, value:=new_tag_name]];
                    new [[ExprCall() at=expr.at, name:="convert_to_expression",
                        arguments := [{ExpressionPtr[1] clone_expression(expr.subexpr) }]
                    ]]
                }]]]
        elif expr.name=="" || expr.name=="e"    // expression
            rblock.list |> emplace_new <| new [[ExprCall() at=expr.at,
                name:="replaceVariable",
                arguments:=[{ExpressionPtr
                    new [[ExprVar() at=expr.at, name:="rules"]];
                    new [[ExprConstString() at=expr.at, value:=new_tag_name]];
                    new [[ExprCall() at=expr.at, name:="add_ptr_ref",
                        arguments := [{ExpressionPtr[1] clone_expression(expr.subexpr) }]
                    ]]
                }]]]
        elif expr.name=="a"    // arguments
            rblock.list |> emplace_new <| new [[ExprCall() at=expr.at,
                name:="replaceVariableWithList",
                arguments:=[{ExpressionPtr
                    new [[ExprVar() at=expr.at, name:="rules"]];
                    new [[ExprConstString() at=expr.at, value:=new_tag_name]];
                    new [[ExprCall() at=expr.at, name:="add_array_ptr_ref",
                        arguments := [{ExpressionPtr[1] clone_expression(expr.subexpr) }]
                    ]]
                }]]]
        elif expr.name=="b" // expression block
            rblock.list |> emplace_new <| new [[ExprCall() at=expr.at,
                name:="replaceVariable",
                arguments:=[{ExpressionPtr
                    new [[ExprVar() at=expr.at, name:="rules"]];
                    new [[ExprConstString() at=expr.at, value:=new_tag_name]];
                    new [[ExprCall() at=expr.at,name:="make_expression_block",
                        arguments := [{ExpressionPtr[1] new [[ExprCall() at=expr.at, name:="add_array_ptr_ref",
                            arguments := [{ExpressionPtr[1] clone_expression(expr.subexpr) }]
                        ]] }]
                    ]]
                }]]]
        elif expr.name=="i" // identificator
            rblock.list |> emplace_new <| new [[ExprCall() at=expr.at,
                name:="renameVariable",
                arguments:=[{ExpressionPtr
                    new [[ExprVar() at=expr.at, name:="rules"]];
                    new [[ExprConstString() at=expr.at, value:=new_tag_name]];
                    clone_expression(expr.subexpr)
                }]]]
        elif expr.name=="f" // field
            rblock.list |> emplace_new <| new [[ExprCall() at=expr.at,
                name:="renameField",
                arguments:=[{ExpressionPtr
                    new [[ExprVar() at=expr.at, name:="rules"]];
                    new [[ExprConstString() at=expr.at, value:=new_tag_name]];
                    clone_expression(expr.subexpr)
                }]]]
            var fexpr <- clone_expression(expr.value)
            if fexpr is ExprField
                (fexpr as ExprField).name := new_tag_name
            elif fexpr is ExprSafeField
                (fexpr as ExprSafeField).name := new_tag_name
            elif fexpr is ExprAsVariant
                (fexpr as ExprAsVariant).name := new_tag_name
            elif fexpr is ExprSafeAsVariant
                (fexpr as ExprSafeAsVariant).name := new_tag_name
            return fexpr
        elif expr.name=="c" // call
            if expr.value == null
                compiling_program() |> macro_error(expr.at,"expecting $c(name)(...), got null")
                failed = true
            elif !(expr.value is ExprCall)
                compiling_program() |> macro_error(expr.at,"expecting $c(name)(...), got {expr.value.__rtti}")
                failed = true
            else
                rblock.list |> emplace_new <| new [[ExprCall() at=expr.at,
                    name:="renameCall",
                    arguments:=[{ExpressionPtr
                        new [[ExprVar() at=expr.at, name:="rules"]];
                        new [[ExprConstString() at=expr.at, value:=new_tag_name]];
                        clone_expression(expr.subexpr)
                    }]]]
                var ccall <- clone_expression(expr.value)
                (ccall as ExprCall).name := new_tag_name
                return ccall
        else
            compiling_program() |> macro_error(expr.at,"unsupported ExprTag {expr.name}")
            failed = true
        return <- new [[ExprVar() at=expr.at, name:=new_tag_name]]

[macro_function]
def private apply_qrules ( var expr:smart_ptr<Expression> ) : smart_ptr<ExprBlock>
    var eblk <- new [[ExprBlock() at=expr.at,
        returnType <- new [[TypeDecl() at=expr.at, baseType=Type tVoid]],
        arguments := [{VariablePtr[1]
            new [[Variable() at=expr.at, name := "rules", _type <- clone_type(typeinfo(ast_typedecl type<Template>)) ]]
        }],
        blockFlags = ExprBlockFlags isClosure
    ]]
    var astVisitor = new QRulesVisitor(get_ptr(eblk))
    var astVisitorAdapter <- make_visitor(*astVisitor)
    expr |> visit(astVisitorAdapter)
    if astVisitor.failed
        eblk := null
    unsafe
        delete astVisitor
    astVisitorAdapter := null
    return eblk

def public apply_qmacro ( var expr:smart_ptr<Expression>; blk:block<(var rules:Template):void> )
    //! Implementation details for the expression reificaiton.
    apply_template(expr, blk)
    return expr

def public apply_qblock ( var expr:smart_ptr<Expression>; blk:block<(var rules:Template):void> )
    //! Implementation details for the expression reificaiton.
    apply_template(expr, blk)
    assert(expr is ExprMakeBlock)
    var qblk <- move_unquote_block(expr)
    // qblk.flags ^= ExprBlockFlags isClosure
    return qblk

class AstQCallMacro : AstCallMacro
    apply_call = "apply_qmacro"
    //! This macro will implement expression reification
    def override canVisitArguments ( call:smart_ptr<ExprCallMacro> ) : bool
        return false    // nop, this one is just like quote
    def wrap_call ( var expr:ExpressionPtr ) : ExpressionPtr
        return expr
    def override visit ( prog:ProgramPtr; mod:Module?; var call:smart_ptr<ExprCallMacro> ) : ExpressionPtr
        if call.arguments.length != 1
            debug(call.arguments)
            macro_error(prog,call.at,"expecting 1 argument, qmacro(expression)")
            return [[ExpressionPtr]]
        if call.arguments[0] is Expression
            macro_error(prog,call.at,"expecting expression, qmacro(expression)")
            return [[ExpressionPtr]]
        var qclone <- clone_expression(call.arguments[0])
        var qblk <- apply_qrules(qclone)
        if qblk==null
            return [[ExpressionPtr]]
        var acall <- new [[ExprCall() at=call.at,
            name:=apply_call,
            arguments := [{ExpressionPtr
                new [[ExprQuote()
                    at=call.at,
                    arguments := [{ExpressionPtr[1] qclone}]
                ]];
                new [[ExprMakeBlock() _block<-qblk, at=call.at]]
            }]
        ]]
        return wrap_call(acall)

[call_macro(name="qmacro")]
class QMacro : AstQCallMacro {}

[call_macro(name="qmacro_block")]
class QBlockMacro : AstQCallMacro
    override apply_call = "apply_qblock"
