options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true

module match shared private

require daslib/ast_boost
require daslib/templates_boost
require daslib/macro_boost
require daslib/strings_boost
require daslib/defer

options debugger
require daslib/debug

let LOG_MATCH = false

struct MatchError
    msg : string
    at : LineInfo const?

struct MatchTo
    conditions   : array<ExpressionPtr>
    declarations : array<ExpressionPtr>
    errors       : array<MatchError>

def log_m ( msg:string )
    static_if LOG_MATCH
        to_log(LOG_INFO, msg)

def match_error ( var errors : array<MatchError>; msg:string; at:LineInfo )
    errors |> push([[MatchError msg=msg, at=unsafe(addr(at))]])

// [[ ExprMakeStruct ]]
def match_any ( what:TypeDeclPtr; wths:ExprMakeStruct?; access:ExpressionPtr; var to:MatchTo )
    if what.baseType!=Type tStructure
        to.errors |> match_error("{describe(what)} does not match [[ ... ]]", wths.at)
        return false
    if !wths.makeType.isAuto && what.structType!=wths.makeType.structType
        to.errors |> match_error("{describe(what)} type does not match [[{describe(wths.makeType)} ... ]]", wths.at)
        return false
    let wdl = what.dim|>length
    let whatDim = wdl>0 ? what.dim[wdl-1] : 1
    if wths.structs|>length != whatDim
        to.errors |> match_error("{describe(what)} number of elements does not match [[describe(wths.makeType)}[{wths.structs|>length}] ... ]]", wths.at)
        return false
    for i in range(wths.structs|>length)
        for mf in *wths.structs[i]
            var newAccess <- whatDim==1 ? qmacro($e(access).$f(mf.name)) : qmacro($e(access)[$v(i)].$f(mf.name))
            var fdecl = find_structure_field(what.structType, string(mf.name))
            if fdecl == null
                to.errors |> match_error("field {mf.name} not found", mf.at)
                return false
            if !match_any(fdecl._type, mf.value, newAccess, to)
                return false
    return true

def match_any ( what:TypeDeclPtr; wth:ExpressionPtr; access:ExpressionPtr; var to:MatchTo )
    log_m("match {describe(what)} {debug_expression(wth)}:\n")
    if wth is ExprTag
        var tag = wth as ExprTag
        if tag.subexpr is ExprVar
            let tname & = unsafe((tag.subexpr as ExprVar).name)
            var decl <- qmacro_expr <|
                let $i(tname) & = unsafe($e(access))
            to.declarations |> emplace(decl)
            log_m("\tadd variable {tname} as {describe(decl)}\n")
            return true
        else
            to.errors |> match_error("invalid $v syntax", tag.at)
            return false
    elif wth is ExprMakeStruct
        return match_any(what, wth as ExprMakeStruct, access, to)
    elif string(wth.__rtti) |> starts_with("ExprConst")
        if wth._type == null
            to.errors |> match_error("unresolved constant type", wth.at)
            return false
        if !is_same_type(what, wth._type, RefMatters no, ConstMatters no, TemporaryMatters no)
            to.errors |> match_error("condition type mismatch {describe(what)} vs {describe(wth._type)}", wth.at)
            return false
        log_m("\tadd condition {describe(access)} == {describe(wth)}\n")
        to.conditions |> emplace_new <| qmacro( $e(access) == $e(wth) )
        return true
    else
        to.errors |> match_error("usupported match syntax", wth.at)
        return false

def join_conditions ( var conditions:array<ExpressionPtr> )
    let clen = conditions|>length
    if clen == 0
        return quote(true)
    var tail <- conditions |> back()
    conditions |> pop()
    while conditions |> length > 0
        var last <- conditions |> back()
        conditions |> pop()
        var new_tail <- qmacro($e(last) && $e(tail))
        tail <- new_tail
    return tail

[call_macro(name="match")]
class MatchMacro : AstCallMacro
    report_match_errors = true
    def override visit ( prog:ProgramPtr; mod:Module?; expr:smart_ptr<ExprCallMacro> ) : ExpressionPtr
        macro_verify(expr.arguments |> length == 2, prog, expr.at, "expecting match(what) <| block")
        assume what = expr.arguments[0]
        assume blk = expr.arguments[1]
        macro_verify(what._type!=null && !what._type.isAutoOrAlias, prog, expr.at, "match `what` argument did not resolve")
        macro_verify(blk._type.isGoodBlockType, prog, expr.at, "match `block` argument did not resolve")
        macro_verify(blk is ExprMakeBlock, prog, expr.at, "match `block` argument must be immediate block declaration")
        var eblk = ((blk as ExprMakeBlock)._block as ExprBlock)
        macro_verify(eblk.finalList |> length == 0, prog, expr.at, "match `block` argument can't have finally section")
        for ee in eblk.list
            if ee is ExprCall
                var eto = ee as ExprCall
                macro_verify(eto.name == "__::match`to" || eto.name=="to", prog, ee.at,
                    "match `block` argument can only contain match-to calls, not {eto.name}")
                macro_verify(eto.arguments |> length == 2, prog, ee.at,
                    "match `block` argument can only contain match-to calls, expecting to(with) <| block")
                macro_verify(eto.arguments[1]._type.isGoodBlockType, prog, ee.at,
                    "expecting to(with) <| block")
            else
                macro_error(prog, ee.at, "match `block` argument can only contain match-to calls, not {ee.__rtti}")
        let what_name = make_unique_private_name("__match_what",expr.at)
        var what_type <- clone_type(what._type); defer_delete(what_type)
        if !what_type.isRefType
            what_type.flags &= ~ TypeDeclFlags ref
        var access <- qmacro($i(what_name)); defer_delete(access)
        var iff : smart_ptr<ExprIfThenElse>; defer_delete(iff)
        let eli = length(eblk.list)
        var any_errors = false
        for ei in range(eli)
            var eto = (eblk.list[eli-ei-1]) as ExprCall
            var to : MatchTo
            if match_any(what._type, eto.arguments[0], access, to)
                var cond <- join_conditions(to.conditions)
                var ablk <- unquote_block(eto.arguments[1])
                var new_iff <- new [[ExprIfThenElse() at=eto.at, cond<-cond]]
                var nblk <- new [[ExprBlock() at=eto.at]]
                for d in to.declarations
                    nblk.list |> emplace(d)
                for l in ablk.list
                    nblk.list |> emplace_new <| clone_expression(l)
                new_iff.if_true <- nblk
                ablk := null
                new_iff.if_false <- iff
                iff <- new_iff
            elif report_match_errors
                any_errors = true
                if to.errors|>length != 0
                    for e in to.errors
                        macro_error(prog, *e.at, e.msg)
                else
                    macro_error(prog, eto.at, "unspecified match error")
                break
            delete to
        if any_errors
            return [[ExpressionPtr]]
        var res <- qmacro_block <|
            var $i(what_name) : $t(what_type) = $e(what)
            $e(iff)
        return <- res

[call_macro(name="static_match")]
class StaticMatchMacro : MatchMacro
    override report_match_errors = false

[sideeffects]
def public to(antyhing; anyBlock) {}

