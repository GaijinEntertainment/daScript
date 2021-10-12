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
    for_each_archetype ( ERQ_HASH, @@ => [[EcsRequest req <- [[string "pos"; "vel"]] ]] ) <| $ ( arch )
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

[macro_function]
def private decs_prefix ( arg:AnnotationArgumentList )
    var p = find_arg ( "prefix", arg)
    if p is tString
        return [[ItCheck yes=p as tString]]
    elif p is tBool
        return [[ItCheck yes=""]]
    else
        return [[ItCheck no=true]]

[macro_function]
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

[macro_function]
def build_req_from_args ( qblk:ExprBlock? )
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
    return <- req

[macro_function]
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
        elif a._type.isRef && !a._type.isRefType
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
class DecsQueryMacro : AstCallMacro
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
        let arch_name = "{prefix}_arch"
        var req <- build_req_from_args(qblk)
        let vreq = verify_request(req)
        if !vreq.ok
            macro_error(prog,expr.at,"incorrect query, {vreq.error}")
            return [[ExpressionPtr]]
        var qblock <- quote() <|
            for_each_archetype (tag_req, tag_erq) <| $ ( tag_arch )
                tag_loop
        // @@ => [[EcsQuery ...]]
        var erq_blk <- new [[ExprBlock() at=expr.at,
            returnType <- new [[TypeDecl() at=expr.at, baseType=Type autoinfer]],
            blockFlags = ExprBlockFlags isClosure
        ]]
        erq_blk.list |> emplace_new <| new [[ExprReturn() at=expr.at,
            subexpr <- convert_to_expression(req, expr.at),
            returnFlags = ExprReturnFlags moveSemantics
        ]]
        var erq_fun <- new [[ExprMakeBlock() at=expr.at,
            mmFlags=ExprMakeBlockFlags isLocalFunction,
            _block <- erq_blk
        ]]
        // for s1, s2 ...
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
            rules |> replaceVariable("tag_erq") <| erq_fun
            rules |> replaceBlockArgument("tag_arch") <| arch_name
            rules |> replaceVariable("tag_req") <| new [[ExprConstUInt() at=expr.at, value=req.hash]]
            rules |> replaceVariable("tag_loop") <| qloop
        var qres <- move_unquote_block(qblock)
        (qres as ExprBlock).blockFlags ^= ExprBlockFlags isClosure
        return <- qres

[macro_function]
def private append_index_lookup ( prog:ProgramPtr; arch_name:string; var qblock:smart_ptr<ExprBlock>; a; prefix:string; const_parent : bool = false )
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
                macro_error(prog,qblock.at,"argument {a.name} has default value, it can't be & or var")
                return false
            getter = "get_default_ro"
        elif a._type.isRef && !a._type.isRefType
            if a._type.flags.constant
                macro_error(prog,qblock.at,"argument {a.name} is both & and constant")
                return false
            getter = "get"
    var iget : ExpressionPtr
    if getter=="get_default_ro"
        unsafe
            iget <- clone_expression(a.init)
    else
        var cget <- new [[ExprCall() name:=getter, at=a.at]]
        cget.arguments |> emplace_new <| new [[ExprVar() at=a.at, name:=arch_name]]
        cget.arguments |> emplace_new <| new [[ExprConstString() at=a.at, value:=a.name]]
        cget.arguments |> emplace_new <| new [[ExprTypeDecl() at=a.at, typeexpr<-ftype]]
        iget <- new [[ExprAt() at=a.at,
                subexpr <- cget,
                index <- new [[ExprVar() at=a.at, name:="entity_index"]]
            ]]
    var vlet <- new [[ExprLet() at=a.at, atInit=a.at]]
    vlet.variables |> emplace_new() <| new [[Variable() at = a.at,
        name := "{prefix}{a.name}",
        _type <- clone_type(a._type),
        init <- iget
        ]]
    qblock.list |> emplace(vlet)
    return true

[call_macro(name="query_one")]
class DecsQueryOneMacro : AstCallMacro
    def override visit ( prog:ProgramPtr; mod:Module?; var expr:smart_ptr<ExprCallMacro> ) : ExpressionPtr
        if length(expr.arguments)!=2 || !(expr.arguments[1] is ExprMakeBlock)
            macro_error(prog,expr.at,"expecting query_one(eid,$(block_with_arguments))")
            return [[ExpressionPtr]]
        let mblk = expr.arguments[1] as ExprMakeBlock
        let qblk = mblk._block as ExprBlock
        if length(qblk.arguments)==0
            macro_error(prog,expr.at,"expecting query($(block_with_arguments)), arguments are missing")
            return [[ExpressionPtr]]
        let prefix = "__{expr.at.line}_desc"
        let arch_name = "{prefix}_arch"
        var req <- build_req_from_args(qblk)
        let vreq = verify_request(req)
        if !vreq.ok
            macro_error(prog,expr.at,"incorrect query, {vreq.error}")
            return [[ExpressionPtr]]
        var qblock <- quote() <|
            for_eid_archetype (tag_eid, tag_req, tag_erq) <| $ ( tag_arch, entity_index )
                tag_loop
        // @@ => [[EcsQuery ...]]
        var erq_blk <- new [[ExprBlock() at=expr.at,
            returnType <- new [[TypeDecl() at=expr.at, baseType=Type autoinfer]],
            blockFlags = ExprBlockFlags isClosure
        ]]
        erq_blk.list |> emplace_new <| new [[ExprReturn() at=expr.at,
            subexpr <- convert_to_expression(req, expr.at),
            returnFlags = ExprReturnFlags moveSemantics
        ]]
        var erq_fun <- new [[ExprMakeBlock() at=expr.at,
            mmFlags=ExprMakeBlockFlags isLocalFunction,
            _block <- erq_blk
        ]]
        // for s1, s2 ...
        var qlbody <- new [[ExprBlock() at=qblk.at]]
        var kaboom : array<tuple<string;string>>
        for a in qblk.arguments
            let detp = a |> is_decs_template
            if detp is yes
                kaboom |> push <| [[auto string(a.name),detp as yes]]
                for f in a._type.structType.fields
                    if !append_index_lookup(prog, arch_name, qlbody, f, detp as yes, a._type.flags.constant)
                        return [[ExpressionPtr]]
            else
                if !append_index_lookup(prog, arch_name, qlbody, a, "")
                    return [[ExpressionPtr]]

        for l in qblk.list
            qlbody.list |> emplace_new <| clone_expression(l)
        for fl in qblk.finalList
            qlbody.finalList |> emplace_new <| clone_expression(fl)
        apply_template(qlbody) <| $ ( rules )
            for kb in kaboom
                rules |> kaboomVarField(kb._0,kb._1)
        apply_template(expr.at, qblock) <| $ ( rules )
            rules |> replaceVariable("tag_eid") <| clone_expression(expr.arguments[0])
            rules |> replaceVariable("tag_erq") <| erq_fun
            rules |> replaceBlockArgument("tag_arch") <| arch_name
            rules |> replaceVariable("tag_req") <| new [[ExprConstUInt() at=expr.at, value=req.hash]]
            rules |> replaceVariable("tag_loop") <| qlbody
        var qres <- move_unquote_block(qblock)
        (qres as ExprBlock).blockFlags ^= ExprBlockFlags isClosure
        return <- qres

[function_macro(name="decs")]
class DecsEcsMacro : AstFunctionAnnotation
    def override apply ( var func:FunctionPtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        let argPass = find_arg("stage", args)
        if !(argPass is tString)
            errors := "need to specify stage"
            return false
        let passName = argPass as tString
        let passFuncName = "decs`pass`{passName}"
        var blk <- setup_call_list(passFuncName, func.at)
        if length(blk.list)==0
            var reg <- setup_call_list("register`decs`passes", func.at, true)
            var regc <- new [[ExprCall() at=func.at, name:="decs::register_decs_stage_call"]]
            regc.arguments |> emplace_new <| new [[ExprConstString() at=func.at, value:=passName]]
            regc.arguments |> emplace_new <| new [[ExprAddr() at=func.at, target:=passFuncName]]
            reg.list |> emplace(regc)
        blk.list |> emplace_new <| new [[ExprCall() at=func.at, name:="_::{func.name}"]]
        var fblk <- new [[ExprBlock() at=func.body.at]]                 // new function block
        var cqq <- make_call(func.at,"query")
        var cquery = cqq as ExprCallMacro
        var qblk <- new [[ExprBlock() at=func.body.at]]                 // inside the query block
        qblk.blockFlags |= ExprBlockFlags isClosure
        qblk.returnType <- new [[TypeDecl() baseType=Type tVoid, at=func.at]]
        var fnbody = func.body as ExprBlock
        for el in fnbody.list                                           // list goes to inside query
            qblk.list |> emplace_new <| clone_expression(el)
        for ef in fnbody.finalList                                      // finally goes to new finally
            fblk.finalList |> emplace_new <| clone_expression(ef)
        for arg in func.arguments                                       // add function arguments to query arguments
            qblk.arguments |> emplace_new <| new [[Variable() at=arg.at,
                name := arg.name,
                _type <- clone_type(arg._type),
                init <- clone_expression(arg.init)
            ]]
        func.arguments |> clear
        cquery.arguments |> emplace_new <| new [[ExprMakeBlock() _block <- qblk, at=func.at]]
        fblk.list |> emplace(cqq)
        func.body := fblk
        return true
