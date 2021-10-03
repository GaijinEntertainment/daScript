options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot

module decs_boost shared private

require daslib/decs public

require daslib/ast_boost
require daslib/templates_boost
require daslib/strings_boost

/*
from:
    query ( ) <| $ ( pos:float3&; vel:float3; col:uint=0x12345678 )
        pos += vel
to:
    var erq <- [[EcsRequest req <- [[string "pos"; "vel"]] ]]
    erq |> for_each_archetype <| $ ( arch )
        for pos, vel in get(arch,"pos",type<float3>), get_ro(arch,"vel",type<float3>), get_default_ro(arch,"col",0x12345678)
            tag
*/

[block_macro(name="decs_require")]
class DecsReq : AstFunctionAnnotation {}

[block_macro(name="decs_require_not")]
class DecsReqN : AstFunctionAnnotation {}

[structure_macro(name="decs_template")]
class DecsTemplate : AstStructureAnnotation {}

variant ItCheck
    yes : string
    no  : bool

def private decs_prefix ( arg:AnnotationArgumentList )
    var p = find_arg ( "prefix", arg)
    if p is tString
        return [[ItCheck yes=p as tString]]
    elif p is tBool
        return [[ItCheck yes=""]]
    else
        return [[ItCheck no=true]]

def private is_decs_template ( v:VariablePtr )
    if v._type.baseType != Type tStructure
        return [[ItCheck no=true]]
    var p = v.annotation |> decs_prefix
    if p is no
        for ann in v._type.structType.annotations
            if eq(ann.annotation.name,"decs_template")
                p = ann.arguments |> decs_prefix
                return p is yes ? p : [[ItCheck yes="{v._type.structType.name}_"]]
    return p

def private append_iterator ( prog:ProgramPtr; arch_name:string; var qloop:smart_ptr<ExprFor>; a; prefix:string; const_parent : bool = false )
    let qli = length(qloop.iterators)
    qloop.iterators |> resize( qli + 1 )
    qloop.iterators[qli] := "{prefix}{a.name}"
    qloop.iteratorsAt |> push(a.at)
    var ftype <- clone_type(a._type)
    ftype.flags &= ~ TypeDeclFlags constant
    ftype.flags &= ~ TypeDeclFlags ref
    var getter = "get_ro"
    if typeinfo(stripped_typename a)=="ast::FieldDeclaration"
        if a.init != null
            getter = "get_default_ro"
        elif !const_parent
            getter = "get"
    else
        if a.init != null
            if a._type.flags.ref || !a._type.flags.constant
                macro_error(prog,qloop.at,"argument {a.name} has default value, it can't be & or var")
                return false
            getter = "get_default_ro"
        elif a._type.isRef
            if a._type.flags.constant
                macro_error(prog,qloop.at,"argument {a.name} is both & and constant")
                return false
            getter = "get"
    var cget <- new [[ExprCall() name:=getter, at=a.at]]
    cget.arguments |> emplace_new <| new [[ExprVar() at=a.at, name:=arch_name]]
    cget.arguments |> emplace_new <| new [[ExprConstString() at=a.at, value:=a.name]]
    if getter=="get_default_ro"
        cget.arguments |> emplace_new <| clone_expression(a.init)
    else
        cget.arguments |> emplace_new <| new [[ExprTypeDecl() at=a.at, typeexpr<-ftype]]
    qloop.sources |> emplace(cget)
    return true

[call_macro(name="query")]
class ApplyMacro : AstCallMacro
    def override visit ( prog:ProgramPtr; mod:Module?; var expr:smart_ptr<ExprCallMacro> ) : ExpressionPtr
        if length(expr.arguments)!=1 || !(expr.arguments[0] is ExprMakeBlock)
            macro_error(prog,expr.at,"expecting query($(block_with_arguments))")
            return [[ExpressionPtr]]
        let mblk = expr.arguments[0] as ExprMakeBlock
        let qblk = mblk._block as ExprBlock
        if length(qblk.arguments)==0
            macro_error(prog,expr.at,"expecting query($(block_with_arguments)), arguments are missing")
            return [[ExpressionPtr]]
        let prefix = "__{expr.at.line}_desc"
        let erq_name = "{prefix}_erq"
        let arch_name = "{prefix}_arch"
        var req : EcsRequest
        for a in qblk.arguments
            if a.init==null
                let detp = a |> is_decs_template
                if detp is yes
                    for f in a._type.structType.fields
                        if f.init == null
                            req.req |> push("{detp as yes}{f.name}")
                else
                    req.req |> push(string(a.name))
        for aa in qblk.annotations
            let isreq = eq(aa.annotation.name,"decs_require")
            let isreqn = eq(aa.annotation.name,"decs_require_not")
            if isreq || isreqn
                for ab in aa.arguments
                    if ab.basicType==Type tBool && ab.bValue
                        (isreq ? req.req : req.reqn) |> push(string(ab.name))
        compile_request(req)
        let vreq = verify_request(req)
        if !vreq.ok
            macro_error(prog,expr.at,"incorrect query, {vreq.error}")
            return [[ExpressionPtr]]
        var qblock <- quote() <|
            tag_req
            tag_erq |> for_each_archetype <| $ ( tag_arch )
                tag_loop
        var ereq <- new [[ExprLet() at=expr.at]]
        ereq.variables |> emplace_new <| new [[Variable() at=expr.at,
                name:=erq_name,
                _type <- new [[TypeDecl() at=expr.at, baseType=Type autoinfer]],
                init <- convert_to_expression(req, expr.at),
                flags = VariableFlags init_via_move
            ]]
        var qloop <- new [[ExprFor() at=qblk.at, visibility=qblk.at]]
        var kaboom : array<tuple<string;string>>
        for a in qblk.arguments
            let detp = a |> is_decs_template
            if detp is yes
                kaboom |> push <| [[auto string(a.name),detp as yes]]
                for f in a._type.structType.fields
                    if !append_iterator(prog, arch_name, qloop, f, detp as yes, a._type.flags.constant)
                        return [[ExpressionPtr]]
            else
                if !append_iterator(prog, arch_name, qloop, a, "")
                    return [[ExpressionPtr]]
        var qlbody <- new [[ExprBlock() at=qblk.at]]
        for l in qblk.list
            qlbody.list |> emplace_new <| clone_expression(l)
        for fl in qblk.finalList
            qlbody.finalList |> emplace_new <| clone_expression(fl)
        qloop.body <- qlbody
        apply_template(qloop) <| $ ( rules )
            for kb in kaboom
                rules |> kaboomVarField(kb._0,kb._1)
        apply_template(expr.at, qblock) <| $ ( rules )
            rules |> renameVariable("tag_erq") <| erq_name
            rules |> replaceBlockArgument("tag_arch") <| arch_name
            rules |> replaceVariable("tag_req_array") <| convert_to_expression(req, expr.at)
            rules |> replaceVariable("tag_req") <| ereq
            rules |> replaceVariable("tag_loop") <| qloop
        var qres <- move_unquote_block(qblock)
        (qres as ExprBlock).blockFlags ^= ExprBlockFlags isClosure
        return <- qres