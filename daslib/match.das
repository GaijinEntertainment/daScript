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

/*
options debugger
require daslib/debug
*/

let LOG_MATCH = false

struct MatchError
    msg : string
    at : LineInfo const?

struct MatchTo
    conditions   : array<ExpressionPtr>
    declarations : table<string; ExpressionPtr>
    errors       : array<MatchError>
    failed       : bool

def log_m ( msg:string )
    static_if LOG_MATCH
        to_log(LOG_INFO, msg)

def match_error ( var errors : array<MatchError>; msg:string; at:LineInfo ) // !!!do not put access.at in there
    errors |> push([[MatchError msg=msg, at=unsafe(addr(at))]])

// [[ ExprMakeStruct ]]
def match_struct ( what:TypeDeclPtr; wths:ExprMakeStruct?; access:ExpressionPtr; var to:MatchTo )
    if what.baseType!=Type tStructure
        to.errors |> match_error("{describe(what)} does not match [[ ... ]]", wths.at)
        return false
    if !wths.makeType.isAuto && what.structType!=wths.makeType.structType
        to.errors |> match_error("{describe(what)} type does not match [[{describe(wths.makeType)} ... ]]", wths.at)
        return false
    let wdl = length(what.dim)
    let wthsLength = length(wths.structs)
    let whatDim = wdl>0 ? what.dim[wdl-1] : 1
    if wthsLength != whatDim
        to.errors |> match_error("{describe(what)} number of elements does not match [[describe(wths.makeType)}[{wthsLength}] ... ]]", wths.at)
        return false
    for i in range(wthsLength)
        var fields : table<string>
        for mf in *wths.structs[i]
            let mfname = string(mf.name)
            if fields |> key_exists(mfname)
                to.errors |> match_error("duplicate field {mf.name}", mf.at)
                return false
            fields |> insert(mfname)
        delete fields
    for i in range(wthsLength)
        for mf in *wths.structs[i]
            var newAccess <- whatDim==1 ? qmacro($e(access).$f(mf.name)) : qmacro($e(access)[$v(i)].$f(mf.name))
            newAccess |> force_at(wths.at)
            defer_delete(newAccess)
            var fdecl = find_structure_field(what.structType, string(mf.name))
            if fdecl == null
                to.errors |> match_error("field {mf.name} not found", mf.at)
                return false
            log_m("\t[[structure[{i}] {mf.name}]] match passed\n")
            if !match_any(fdecl._type, mf.value, newAccess, to)
                return false
    return true

// [[ ExprMakeTuple ]]
def match_tuple ( what:TypeDeclPtr; wths:ExprMakeTuple?; access:ExpressionPtr; var to:MatchTo )
    if what.baseType!=Type tTuple
        to.errors |> match_error("{describe(what)} does not match [[ ... ]] tuple", wths.at)
        return false
    if wths.makeType!=null && !wths.makeType.isAuto && !is_same_type(what, wths.makeType, RefMatters no, ConstMatters no, TemporaryMatters no)
        to.errors |> match_error("{describe(what)} type does not match [[{describe(wths.makeType)} ... ]] tuple", wths.at)
        return false
    // find if there is ..., get its index
    var tagIndex = INT_MAX
    for v,i in wths.values,count()
        if (v is ExprTag) && (v as ExprTag).name=="..."
            if tagIndex!=INT_MAX
                to.errors |> match_error("... can only apper once per tuple match", wths.at)
                return false
            tagIndex = i
    // check lengths
    let wthsLength = length(wths.values)
    let whatDim = length(what.argTypes)
    if tagIndex==INT_MAX
        if whatDim != wthsLength
            to.errors |> match_error("{describe(what)} number of elements does not match [[describe(wths.makeType)} ... ]] tuple", wths.at)
            return false
    else
        if whatDim < wthsLength-1
            to.errors |> match_error("{describe(what)} needs at least {wthsLength-1} tuple elements to match", wths.at)
            return false
    log_m("\tuple match passed\n")
    for i in range(wthsLength)
        if i == tagIndex    // skip ...
            continue
        var ei = i<= tagIndex ? i : whatDim - (wthsLength - i)
        var newAccess <- qmacro($e(access).$f("_{ei}"))
        newAccess |> force_at(wths.at)
        defer_delete(newAccess)
        if !match_any(what.argTypes[ei], wths.values[i], newAccess, to)
            return false
    return true

// [[ ExprMakeArray ]]
def match_array ( what:TypeDeclPtr; wths:ExprMakeArray?; dynamic:bool; access:ExpressionPtr; var to:MatchTo )
    let wdl = length(what.dim)
    var recT : TypeDeclPtr; defer_delete(recT)
    // find if there is ..., get its index
    var tagIndex = INT_MAX
    for v,i in wths.values,count()
        if (v is ExprTag) && (v as ExprTag).name=="..."
            if tagIndex!=INT_MAX
                to.errors |> match_error("... can only apper once per array match", wths.at)
                return false
            tagIndex = i
    // check lengths
    let wthsLength = length(wths.values)
    if wdl != 0 // check if static array
        if dynamic
            to.errors |> match_error("{describe(what)} is not a dynamic array", wths.at)
            return false
        let whatDim = what.dim[wdl-1]
        if tagIndex==INT_MAX
            if whatDim!=wthsLength
                to.errors |> match_error("{describe(what)} number of elements does not match [[describe(wths.makeType)}[{wthsLength}] ... ]] array", wths.at)
                return false
        else
            if whatDim < wthsLength-1
                to.errors |> match_error("{describe(what)} needs at least {wthsLength-1} array elements to match", wths.at)
                return false
        unsafe
            recT <- clone_type(what)
        recT.dim |> pop()
        if !wths.makeType.isAuto && !is_same_type(recT, wths.makeType, RefMatters no, ConstMatters no, TemporaryMatters no)
            to.errors |> match_error("{describe(what)} type does not match [[{describe(wths.recordType)}[{wthsLength}] ... ]] array", wths.at)
            return false
    elif !what.isGoodArrayType // check if dynamic array
        to.errors |> match_error("{describe(what)} is not an array", wths.at)
        return false
    else
        if !dynamic
            to.errors |> match_error("{describe(what)} is not a static array", wths.at)
            return false
        recT := what.firstType
        var lenCond <- (tagIndex!=INT_MAX ?
            qmacro(length($e(access)) >= $v(wthsLength-1)) :
            qmacro(length($e(access)) == $v(wthsLength)))
        lenCond |> force_at(wths.at)
        to.conditions |> emplace(lenCond)
    log_m("\tarray match passed\n")
    for i in range(wthsLength)
        if i == tagIndex    // skip ...
            continue
        var ei = i<=tagIndex ? i : wthsLength-i
        var newAccess <- i<tagIndex ? qmacro($e(access)[$v(ei)]) : qmacro($e(access)[length($e(access)) - $v(ei)])
        newAccess |> force_at(wths.at)
        defer_delete(newAccess)
        if !match_any(recT, wths.values[i], newAccess, to)
            return false
    return true

// [[ ExprMakeVariant ]]
def match_any ( what:TypeDeclPtr; wths:ExprMakeVariant?; access:ExpressionPtr; var to:MatchTo )
    if what.baseType!=Type tVariant
        to.errors |> match_error("{describe(what)} does not match [[ ... ]] variant", wths.at)
        return false
    if wths.makeType==null || !(wths.makeType.baseType==Type autoinfer || wths.makeType.baseType==Type tVariant)
        to.errors |> match_error("variant type has not been resolved", wths.at)
        return false
    if !wths.makeType.isAuto && !is_same_type(what, wths.makeType, RefMatters no, ConstMatters no, TemporaryMatters no)
        to.errors |> match_error("{describe(what)} type does not match [[{describe(wths.makeType)} ... ]] variant", wths.at)
        return false
    let wdl = length(what.dim)
    let wthsLength = length(wths.variants)
    let whatDim = wdl>0 ? what.dim[wdl-1] : 1
    if wthsLength != whatDim
        to.errors |> match_error("{describe(what)} number of variant elements does not match [[describe(wths.makeType)}[{wthsLength}] ... ]]", wths.at)
        return false
    for mf,i in wths.variants,count()
        var newAccess <- whatDim==1 ? qmacro($e(access) as $f(mf.name)) : qmacro($e(access)[$v(i)] as $f(mf.name))
        newAccess |> force_at(wths.at)
        defer_delete(newAccess)
        var found = false
        for ft,fn in wths.makeType.argTypes, wths.makeType.argNames
            if fn==mf.name
                found = true
                to.conditions |> emplace_new <| new [[ExprIsVariant() at=wths.at,
                    value <- clone_expression(access),
                    name := mf.name
                ]]
                log_m("\t[[variant {mf.name}]] match passed\n")
                if !match_any(ft, mf.value, newAccess, to)
                    return false
                break
        if !found
            to.errors |> match_error("variant {mf.name} not found", mf.at)
            return false
    return true

// as variant
def match_any ( what:TypeDeclPtr; wthAs:ExprAsVariant?; access:ExpressionPtr; var to:MatchTo )
    to.conditions |> emplace_new <| new [[ExprIsVariant() at=wthAs.at,
        value <- clone_expression(access),
        name := wthAs.name
    ]]
    var new_access <- new [[ExprAsVariant() at=wthAs.at,
        value <- clone_expression(access),
        name := wthAs.name
    ]]; defer_delete(new_access)
    log_m("\tas {wthAs.name} match passed\n")
    return match_any(what, wthAs.value, access, to)

// match_type(expr, type)
def match_type ( what:TypeDeclPtr; wthmt:ExprCall?; access:ExpressionPtr; var to:MatchTo )
    if length(wthmt.arguments)!=2
        to.errors |> match_error("match_type expects 2 arguments", wthmt.at)
        return false
    assume value = wthmt.arguments[0]
    assume mtype = wthmt.arguments[1]._type
    if mtype==null
        to.errors |> match_error("unresolved type", wthmt.arguments[1].at)
        to.failed = true
        return false
    if is_same_type(what, mtype, RefMatters no, ConstMatters no, TemporaryMatters no)
        log_m("\tmatch_type {describe(what)} passed\n")
        return match_any(what, value, access, to)
    else
        to.errors |> match_error("mismatching type {describe(what)} vs {describe(mtype)}", wthmt.arguments[1].at)
        return false

// match_expr(expr)
def match_expr ( what:TypeDeclPtr; wthmt:ExprCall?; access:ExpressionPtr; var to:MatchTo )
    if length(wthmt.arguments)!=1
        to.errors |> match_error("match_expr expects 1 argument", wthmt.at)
        return false
    assume value = wthmt.arguments[0]
    if value._type!=null && !is_same_type(what, value._type, RefMatters no, ConstMatters no, TemporaryMatters no)
        to.errors |> match_error("mismatching expression type {describe(what)} vs {describe(value._type)}", value.at)
        return false
    var expr <- clone_expression(value); defer_delete(expr)
    apply_template(expr) <| $ ( rules )
        for dv,dn in keys(to.declarations),values(to.declarations)
            rules |> replaceVariable(dv) <| clone_expression(dn)
    var cond <- qmacro($e(access) == $e(expr))
    cond |> force_at(value.at)
    to.conditions |> emplace(cond)
    return true

// && guards
def match_guards ( what:TypeDeclPtr; wth:ExprOp2?; access:ExpressionPtr; var to:MatchTo )
    if match_any(what, wth.left, access, to)
        log_m("\tguards {describe(wth.right)} at {wth.right.at}\n")
        var guards <- clone_expression(wth.right)
        apply_template(guards) <| $ ( rules )
            for dv,dn in keys(to.declarations),values(to.declarations)
                rules |> replaceVariable(dv) <| clone_expression(dn)
        to.conditions |> emplace(guards)
        return true
    else
        return false

// left || right
def match_or ( what:TypeDeclPtr; wth:ExprOp2?; access:ExpressionPtr; var to:MatchTo )
    var toL : MatchTo; defer_delete(toL)
    toL.declarations := to.declarations
    if !match_any(what, wth.left, access, toL)
        return false
    var toR : MatchTo; defer_delete(toR);
    if !match_any(what, wth.right, access, toR)
        return false
    for k in keys(to.declarations)
        toL.declarations |> erase(k)
        toR.declarations |> erase(k)
    if length(toL.declarations) != length(toR.declarations)
        to.errors |> match_error("mismatching variable declarations in ||", wth.at)
        return false
    for k,vL in keys(toL.declarations),values(toL.declarations)
        if describe(vL) != describe(toR.declarations[k])
            to.errors |> match_error("mismatching variable {k} declaration in ||", wth.at)
            return false
    delete to.declarations
    to.declarations <- toL.declarations // because its a superset
    var lcond <- join_conditions(toL.conditions); defer_delete(lcond)
    var rcond <- join_conditions(toR.conditions); defer_delete(rcond)
    var cond <- qmacro($e(lcond) || $e(rcond))
    to.conditions |> emplace(cond)
    return true

// $v(variable)
def match_tag ( what:TypeDeclPtr; wth:ExpressionPtr; access:ExpressionPtr; var to:MatchTo )
    var tag = wth as ExprTag
    if tag.name!="v"
        to.errors |> match_error("unsupported match tag ${tag.name}", tag.at)
        return true
    if tag.subexpr is ExprVar
        let tname  = string((tag.subexpr as ExprVar).name)
        if to.declarations |> key_exists(tname)
            to.errors |> match_error("duplicate variable {tname}", tag.at)
            return false
        unsafe
            to.declarations[tname] <- clone_expression(access)
        log_m("\tadd variable {tname} as {describe(access)}\n")
        return true
    else
        to.errors |> match_error("invalid $v syntax", tag.at)
        return false

def is_match_call ( wth:ExpressionPtr; name:string )
    return (wth is ExprCall) && ((wth as ExprCall).name=="match_{name}" || (wth as ExprCall).name=="__::match`match_{name}")

def match_any ( what:TypeDeclPtr; wth:ExpressionPtr; access:ExpressionPtr; var to:MatchTo )
    log_m("match {describe(what)} {debug_expression(wth)}:\n")
    if wth is ExprTag
        return match_tag(what, wth, access, to)
    elif (wth is ExprVar) && (wth as ExprVar).name=="_"
        log_m("\tmatch anything via _n\n")
        return true
    elif wth is ExprMakeStruct
        return match_struct(what, wth as ExprMakeStruct, access, to)
    elif wth is ExprMakeArray
        var mka = wth as ExprMakeArray
        if length(mka.values)==1 && (mka.values[0] is ExprMakeTuple)
            return match_tuple(what, (mka.values[0] as ExprMakeTuple), access, to)
        else
            return match_array(what, wth as ExprMakeArray, false, access, to)
    elif wth is ExprMakeVariant
        return match_any(what, wth as ExprMakeVariant, access, to)
    elif wth is ExprAsVariant
        return match_any(what, wth as ExprAsVariant, access, to)
    elif wth |> is_match_call("type")
        return match_type(what, wth as ExprCall, access, to)
    elif wth |> is_match_call("expr")
        return match_expr(what, wth as ExprCall, access, to)
    elif (wth is ExprCall) && ((wth as ExprCall).name=="to_array_move" || (wth as ExprCall).name=="__::builtin`to_array_move")
        var cll = wth as ExprCall
        if length(cll.arguments)!=1
            to.errors |> match_error("to_array_move expects 1 argument", cll.at)
            return false
        if cll.arguments[0] is ExprMakeArray
            return match_array(what, cll.arguments[0] as ExprMakeArray, true, access, to)
        else
            to.errors |> match_error("to_array_move expects array", cll.at)
            return false
    elif (wth is ExprOp2) && (wth as ExprOp2).op=="&&"
        return match_guards(what, wth as ExprOp2, access, to)
    elif (wth is ExprOp2) && (wth as ExprOp2).op=="||"
        return match_or(what, wth as ExprOp2, access, to)
    elif string(wth.__rtti) |> starts_with("ExprConst")
        if wth._type == null
            to.errors |> match_error("unresolved constant type", wth.at)
            return false
        if !is_same_type(what, wth._type, RefMatters no, ConstMatters no, TemporaryMatters no)
            to.errors |> match_error("condition type mismatch {describe(what)} vs {describe(wth._type)}", wth.at)
            return false
        log_m("\tadd condition {describe(access)} == {describe(wth)}\n")
        var cond <- qmacro($e(access) == $e(wth))
        cond |> force_at(wth.at)
        to.conditions |> emplace(cond)
        return true
    elif wth._type!=null && is_same_type(what,wth._type, RefMatters no, ConstMatters no, TemporaryMatters no)
        log_m("\tadd condition {describe(access)} == {describe(wth)}\n")
        var cond <- qmacro($e(access) == $e(wth))
        cond |> force_at(wth.at)
        to.conditions |> emplace(cond)
        return true
    else
        to.errors |> match_error("usupported match syntax {wth.__rtti}", wth.at)
        to.failed = true
        return false

def join_conditions ( var conditions:array<ExpressionPtr> )
    let clen = length(conditions)
    if clen == 0
        return quote(true)
    var tail <- conditions |> back()
    conditions |> pop()
    while length(conditions) > 0
        var last <- conditions |> back()
        conditions |> pop()
        var new_tail <- qmacro($e(last) && $e(tail))
        new_tail |> force_at(last.at)
        tail <- new_tail
    return tail

[call_macro(name="match")]
class MatchMacro : AstCallMacro
    report_match_errors = true
    def override canVisitArguments ( expr:smart_ptr<ExprCallMacro> ) : bool
        // if we are reporting errors, and macro did not fold - no point showing errors inside match - there will be too many, and they are meaningless
        return !is_reporting_compilation_errors()
    def override canFoldReturnResult ( expr:smart_ptr<ExprCallMacro> ) : bool
        // function can't decide if its void or not, until match is folded
        return false
    def override preVisit ( prog:ProgramPtr; mod:Module?; expr:smart_ptr<ExprCallMacro> ) : void
        return  if length(expr.arguments) != 2
        assume blk = expr.arguments[1]
        return  if !blk._type.isGoodBlockType
        return  if !(blk is ExprMakeBlock)
        // we mark all if's inside the match to not fold, so that we can fold the match later
        var eblk = ((blk as ExprMakeBlock)._block as ExprBlock)
        for ee in eblk.list
            if ee is ExprIfThenElse
                (ee as ExprIfThenElse).if_flags |= IfFlags doNotFold
    def override visit ( prog:ProgramPtr; mod:Module?; expr:smart_ptr<ExprCallMacro> ) : ExpressionPtr
        macro_verify(length(expr.arguments) == 2, prog, expr.at, "expecting match(what) <| block")
        assume what = expr.arguments[0]
        assume blk = expr.arguments[1]
        macro_verify(what._type!=null && !what._type.isAutoOrAlias, prog, expr.at, "match `what` argument did not resolve")
        macro_verify(blk._type.isGoodBlockType, prog, expr.at, "match `block` argument did not resolve")
        macro_verify(blk is ExprMakeBlock, prog, expr.at, "match `block` argument must be immediate block declaration")
        var eblk = ((blk as ExprMakeBlock)._block as ExprBlock)
        macro_verify(length(eblk.finalList) == 0, prog, expr.at, "match `block` argument can't have finally section")
        for ee in eblk.list
            if ee is ExprIfThenElse
                let eite = ee as ExprIfThenElse
                macro_verify(eite.if_false==null, prog, expr.at, "match to `if` can't have `else` section")
            else
                macro_error(prog, ee.at, "match `block` argument can only contain match-to calls, not {ee.__rtti}")
        let what_name = make_unique_private_name("__match_what",expr.at)
        var what_type <- clone_type(what._type); defer_delete(what_type)
        if !what_type.isRefType
            what_type.flags &= ~ TypeDeclFlags ref
        else
            what_type.flags |= TypeDeclFlags ref
        var access <- qmacro($i(what_name)); defer_delete(access)
        access |> force_at(what.at)
        var iff : smart_ptr<ExprIfThenElse>; defer_delete(iff)
        let eli = length(eblk.list)
        var any_errors = false
        for ei in range(eli)
            var eto = (eblk.list[eli-ei-1]) as ExprIfThenElse
            var to : MatchTo
            if match_any(what._type, eto.cond, access, to)
                log_m("match_any ok\n\n")
                var new_iff <- new [[ExprIfThenElse() at=eto.at, cond<-join_conditions(to.conditions)]]
                var nblk <- new [[ExprBlock() at=eto.at]]
                for dn,de in keys(to.declarations),values(to.declarations)
                    de.genFlags |= ExprGenFlags alwaysSafe
                    var ndecl <- qmacro_expr <|
                        let $i(dn) & = $e(de)
                    ndecl |> force_at(de.at)
                    nblk.list |> emplace(ndecl)
                for l in (eto.if_true as ExprBlock).list
                    nblk.list |> emplace_new <| clone_expression(l)
                new_iff.if_true <- nblk
                new_iff.if_false <- iff
                iff <- new_iff
            elif report_match_errors || to.failed
                log_m("match_any failed\n\n")
                any_errors = true
                if length(to.errors) != 0
                    for e in to.errors
                        macro_error(prog, *e.at, e.msg)
                else
                    macro_error(prog, eto.at, "unspecified match error")
                break
            static_if LOG_MATCH
                for e in to.errors
                    log_m("\tERROR:: {e.msg} at {describe(*e.at)}\n")
            delete to
        if any_errors
            return [[ExpressionPtr]]
        var cwhat <- clone_expression(what); defer_delete(cwhat)
        cwhat.genFlags |= ExprGenFlags alwaysSafe
        var res <- qmacro_block <|
            var $i(what_name) : $t(what_type) = $e(cwhat)
            $e(iff)
        return <- res

[call_macro(name="static_match")]
class StaticMatchMacro : MatchMacro
    override report_match_errors = false

def public match_type(anything,anytype) : void {}

def public match_expr(anything) : void {}
