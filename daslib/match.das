options gen2
options indenting = 4
options strict_smart_pointers = true

module match shared private

//! Pattern matching macro for daslang.
//!
//! Provides ``match`` and ``static_match`` macros
//! that decompose values by structure, variant, tuple, array, or constant comparison.
//! Also provides ``match_type`` and ``match_expr`` helpers, guard expressions (``&&``),
//! alternation (``||``), wildcard (``_``), and variable capture (``$v(name)``).
//! Structures annotated with ``[match_as_is]`` or ``[match_copy]`` enable custom matching.

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

struct MatchError {
    //! Stores a single pattern-matching compilation error with its source location.
    msg : string                        //! error message text
    @do_not_delete at : LineInfo const? //! source location of the error
}

struct MatchTo {
    //! Accumulates the result of pattern analysis for one match arm.
    conditions   : array<Expression?>             //! boolean conditions that must all be true for the arm to match
    declarations : table<string; Expression?>        //! captured variables (name → access expression)
    prefix       : array<Expression?>             //! prefix statements (e.g. temporaries for match_copy)
    errors       : array<MatchError>                //! errors encountered during pattern analysis
    failed       : bool                             //! true if a hard error occurred (unresolved types etc.)
}

[macro_function, unused_argument(msg)]
def log_m(msg : string) {
    static_if (LOG_MATCH) {
        to_log(LOG_INFO, msg)
    }
}

[macro_function]
def match_error(var errors : array<MatchError>; msg : string; at : LineInfo) {
    errors |> push(MatchError(msg = msg, at = unsafe(addr(at))))
}

enum MatchType {
    //! Type of pattern match clause.
    none
    as_is
    copy
}

[macro_function]
def is_string_like(typ : TypeDeclPtr) : bool {
    if (typ == null) return false
    if (typ.baseType == Type.tString) return true
    return typ.baseType == Type.tHandle && typ.annotation != null && typ.annotation.name == "das_string"
}

[macro_function]
def get_match_type(typ : TypeDeclPtr) {
    if (!typ.isStructure || typ.structType == null) return MatchType.none
    for (ann in typ.structType.annotations) {
        if (ann.annotation.name == "match_as_is") {
            return MatchType.as_is
        } elif (ann.annotation.name == "match_copy") return MatchType.copy
    }
    return MatchType.none
}

[macro_function]
def match_custom(mt : MatchType; wths : Expression?; makeType : TypeDeclPtr; access : Expression?; var to : MatchTo) : bool {
    //! A ``[match_as_is]`` or ``[match_copy]`` structure matched against a value of another type.
    log_m("\tmatch_custom {mt} {describe(makeType)} {describe(access)}")
    if (mt == MatchType.as_is) {
        let name = string(makeType.isHandle ? makeType.annotation.name : makeType.structType.name)
        return match_any(makeType, wths, variant_access(access, name, wths.at, to), to)
    }
    let tempName = make_unique_private_name("__`match`copy", wths.at)
    let prefixType = clone_type(makeType)
    var prefix = qmacro_expr() {
        let $i(tempName) : $t(prefixType)
    }
    prefix |> force_at(wths.at)
    to.prefix |> emplace <| prefix
    let access_new = qmacro($i(tempName))
    access_new |> force_at(wths.at)
    var cond_nz = qmacro(match_copy($i(tempName), $e(access))); cond_nz |> force_at(wths.at)
    to.conditions |> emplace(cond_nz)
    return match_any(makeType, wths, access_new, to)
}

def public match_peel_r2v(var e : Expression?) : Expression? {
    //! Strip post-typer ``ExprRef2Value`` wrappers. The ``match`` macro emits this around the source side of AST class patterns so a clean pattern (``ExprField(...)``) matches a typer-wrapped source, mirroring ``qmatch``'s transparency rule. ``$v`` captures of Expression-typed values go through it too, so they bind the peeled node.
    while (e != null && e is ExprRef2Value) {
        e = (e as ExprRef2Value).subexpr
    }
    return e
}

def public match_peel_r2v(e : Expression const?) : Expression const? {
    //! Pointee-const flavor — same peel, preserves the caller's const view.
    var c : Expression const? = e
    while (c != null && c is ExprRef2Value) {
        c = (c as ExprRef2Value).subexpr
    }
    return c
}

[macro_function]
def private is_wildcard(e : Expression const?) : bool {
    let x = match_peel_r2v(e)
    return x is ExprVar && (x as ExprVar).name == "_"
}

[macro_function]
def private is_expression_ptr(typ : TypeDeclPtr) : bool => typ != null && typ.isPointer && typ.firstType != null && isExpression(typ.firstType)

[macro_function]
def private compatible(what, typ : TypeDeclPtr) : bool {
    //! The value type and a constant's or expression's type compare with ``==``: the same type, or both string-like.
    return is_same_type(what, typ, RefMatters.no, ConstMatters.no, TemporaryMatters.no) || (is_string_like(what) && is_string_like(typ))
}

[macro_function]
def private require_non_null(e : Expression?; at : LineInfo; var to : MatchTo) : void {
    var cond = qmacro($e(e) != null)
    cond |> force_at(at)
    to.conditions |> emplace(cond)
}

[macro_function]
def private variant_access(access : Expression?; name : string; at : LineInfo; var to : MatchTo) : Expression? {
    //! Requires ``access`` to hold alternative ``name`` and returns the access to it.
    var cond = new ExprIsVariant(at = at, value = clone_expression(access), name := name)
    cond |> force_at(at)
    to.conditions |> emplace(cond)
    return new ExprAsVariant(at = at, value = clone_expression(access), name := name)
}

[macro_function]
def private rest_index(values; kind : string; at : LineInfo; var to : MatchTo) : int {
    //! Where ``...`` sits in a tuple or array pattern: ``INT_MAX`` without one, -1 (an error) for a second one.
    var index = INT_MAX
    for (v, i in values, count()) {
        continue if (!is_rest(v))
        if (index != INT_MAX) {
            to.errors |> match_error("... can only apper once per {kind} match", at)
            return -1
        }
        index = i
    }
    return index
}

[macro_function]
def private is_rest(p : Expression const?) : bool {
    let x = match_peel_r2v(p)
    return x is ExprTag && (x as ExprTag).name == "..."
}

[macro_function]
def match_struct(what : TypeDeclPtr; wths : ExprMakeStruct?; access : Expression?; var to : MatchTo) {
    if (wths.makeType.isHandle && isExpression(wths.makeType) && is_expression_ptr(what)) {
        let subject = (wths.makeType.annotation.name != "ExprRef2Value" ? qmacro(match_peel_r2v($e(access))) : clone_expression(access))
        subject |> force_at(wths.at)
        require_non_null(subject, wths.at, to)
        return match_custom(MatchType.as_is, wths, wths.makeType, subject, to)
    }
    if (what.baseType == Type.tPointer && what.firstType != null && (what.firstType.isStructure || what.firstType.isHandle)) {
        require_non_null(access, wths.at, to)
        let access_nz = qmacro(*($e(access)))
        access_nz |> force_at(wths.at)
        return match_struct(what.firstType, wths, access_nz, to)
    }
    if (!(what.isStructure || what.isHandle)) {
        to.errors |> match_error("{describe(what)} does not match [[ ... ]]", wths.at)
        return false
    }
    if (wths.makeType != null && !wths.makeType.isAuto && !is_same_type(what, wths.makeType, RefMatters.no, ConstMatters.no, TemporaryMatters.no)) {
        let mtype = wths.makeType.isStructure ? get_match_type(wths.makeType) : MatchType.none
        return match_custom(mtype, wths, wths.makeType, access, to) if (mtype != MatchType.none)
        to.errors |> match_error("{describe(what)} type does not match [[{describe(wths.makeType)} ... ]] structure", wths.at)
        return false
    }
    let wthsLength = length(wths.structs)
    if (wthsLength != 1) {
        to.errors |> match_error("{describe(what)} number of elements does not match [[describe(wths.makeType)}[{wthsLength}] ... ]]", wths.at)
        return false
    }
    var inscope fields : table<string>
    for (mf in *wths.structs[0]) {
        let mfname = string(mf.name)
        if (fields |> key_exists(mfname)) {
            to.errors |> match_error("duplicate field {mf.name}", mf.at)
            return false
        }
        fields |> insert(mfname)
    }
    for (mf in *wths.structs[0]) {
        let new_access = qmacro($e(access).$f(mf.name))
        new_access |> force_at(wths.at)
        let ftype = get_field_type(what, string(mf.name), what.flags.constant)
        if (ftype == null) {
            to.errors |> match_error("field {mf.name} not found", mf.at)
            return false
        }
        log_m("\t[[structure {mf.name}]] match passed\n")
        if (!match_any(ftype, mf.value, new_access, to)) return false
    }
    return true
}

[macro_function]
def match_tuple(what : TypeDeclPtr; wths : ExprMakeTuple?; access : Expression?; var to : MatchTo) {
    if (what.baseType != Type.tTuple) {
        to.errors |> match_error("{describe(what)} does not match [[ ... ]] tuple", wths.at)
        return false
    }
    if (wths.makeType != null && !wths.makeType.isAuto && !is_same_type(what, wths.makeType, RefMatters.no, ConstMatters.no, TemporaryMatters.no)) {
        to.errors |> match_error("{describe(what)} type does not match [[{describe(wths.makeType)} ... ]] tuple", wths.at)
        return false
    }
    let tagIndex = rest_index(wths.values, "tuple", wths.at, to)
    return false if (tagIndex < 0)
    let wthsLength = length(wths.values)
    let whatDim = length(what.argTypes)
    if (tagIndex == INT_MAX) {
        if (whatDim != wthsLength) {
            to.errors |> match_error("{describe(what)} number of elements does not match [[describe(wths.makeType)} ... ]] tuple", wths.at)
            return false
        }
    } else {
        if (whatDim < wthsLength - 1) {
            to.errors |> match_error("{describe(what)} needs at least {wthsLength-1} tuple elements to match", wths.at)
            return false
        }
    }
    log_m("\tuple match passed\n")
    for (i in range(wthsLength)) {
        if (i == tagIndex) continue
        let ei = i <= tagIndex ? i : whatDim - (wthsLength - i)
        {
            let new_access = qmacro($e(access).$f("_{ei}")); new_access |> force_at(wths.at)
            if (!match_any(what.argTypes[ei], wths.values[i], new_access, to)) return false
        }
    }
    return true
}

[macro_function]
def match_array(what : TypeDeclPtr; wths : ExprMakeArray?; dynamic : bool; access : Expression?; var to : MatchTo) {
    let isFA = what.baseType == Type.tFixedArray
    var recT : TypeDeclPtr
    let tagIndex = rest_index(wths.values, "array", wths.at, to)
    return false if (tagIndex < 0)
    let wthsLength = length(wths.values)
    if (isFA) {
        if (dynamic) {
            to.errors |> match_error("{describe(what)} is not a dynamic array", wths.at)
            return false
        }
        let whatDim = what.fixedDim
        if (tagIndex == INT_MAX) {
            if (whatDim != wthsLength) {
                to.errors |> match_error("{describe(what)} number of elements does not match [[describe(wths.makeType)}[{wthsLength}] ... ]] array", wths.at)
                return false
            }
        } else {
            if (whatDim < wthsLength - 1) {
                to.errors |> match_error("{describe(what)} needs at least {wthsLength-1} array elements to match", wths.at)
                return false
            }
        }
        recT = clone_type(what.firstType)
        if (!wths.makeType.isAuto && !is_same_type(recT, wths.makeType, RefMatters.no, ConstMatters.no, TemporaryMatters.no)) {
            to.errors |> match_error("{describe(what)} type does not match [[{describe(wths.recordType)}[{wthsLength}] ... ]] array", wths.at)
            return false
        }
    } elif (!what.isGoodArrayType) {
        to.errors |> match_error("{describe(what)} is not an array", wths.at)
        return false
    } else {
        if (!dynamic) {
            to.errors |> match_error("{describe(what)} is not a static array", wths.at)
            return false
        }
        recT = clone_type(what.firstType)
        var lenCond = (tagIndex != INT_MAX ?
            qmacro(length($e(access)) >= $v(wthsLength - 1)) :
            qmacro(length($e(access)) == $v(wthsLength)))
        lenCond |> force_at(wths.at)
        lenCond.genFlags.generated = true
        to.conditions |> emplace(lenCond)
    }
    log_m("\tarray match passed\n")
    for (i in range(wthsLength)) {
        if (i != tagIndex) {
            let ei = i <= tagIndex ? i : wthsLength - i
            let new_access = i < tagIndex ? qmacro($e(access)[$v(ei)]) : qmacro($e(access)[length($e(access)) - $v(ei)])
            new_access |> force_at(wths.at)
            if (!match_any(recT, wths.values[i], new_access, to)) return false
        }
    }
    return true
}

[macro_function]
def match_variant(what : TypeDeclPtr; wths : ExprMakeVariant?; access : Expression?; var to : MatchTo) {
    if (what.baseType != Type.tVariant) {
        to.errors |> match_error("{describe(what)} does not match [[ ... ]] variant", wths.at)
        return false
    }
    if (wths.makeType == null || !(wths.makeType.baseType == Type.autoinfer || wths.makeType.baseType == Type.tVariant)) {
        to.errors |> match_error("variant type has not been resolved", wths.at)
        return false
    }
    if (!wths.makeType.isAuto && !is_same_type(what, wths.makeType, RefMatters.no, ConstMatters.no, TemporaryMatters.no)) {
        to.errors |> match_error("{describe(what)} type does not match [[{describe(wths.makeType)} ... ]] variant", wths.at)
        return false
    }
    let wthsLength = length(wths.variants)
    if (wthsLength != 1) {
        to.errors |> match_error("{describe(what)} number of variant elements does not match [[describe(wths.makeType)}[{wthsLength}] ... ]]", wths.at)
        return false
    }
    assume mf = wths.variants[0]
    let ftype = get_field_type(what, string(mf.name), what.flags.constant)
    if (ftype == null) {
        to.errors |> match_error("variant {mf.name} not found", mf.at)
        return false
    }
    log_m("\t[[variant {mf.name}]] match passed\n")
    return match_any(ftype, mf.value, variant_access(access, string(mf.name), wths.at, to), to)
}

[macro_function]
def match_as_variant(what : TypeDeclPtr; wthAs : ExprAsVariant?; access : Expression?; var to : MatchTo) {
    if (what.baseType != Type.tVariant) {
        to.errors |> match_error("{describe(what)} is not a variant", wthAs.at)
        return false
    }
    let ftype = get_field_type(what, string(wthAs.name), what.flags.constant)
    if (ftype == null) {
        to.errors |> match_error("variant {wthAs.name} not found", wthAs.at)
        return false
    }
    log_m("\tas {wthAs.name} match passed\n")
    return match_any(ftype, wthAs.value, variant_access(access, string(wthAs.name), wthAs.at, to), to)
}

[macro_function]
def match_type(what : TypeDeclPtr; wthmt : ExprCall?; access : Expression?; var to : MatchTo) {
    if (length(wthmt.arguments) != 2) {
        to.errors |> match_error("match_type expects 2 arguments", wthmt.at)
        return false
    }
    assume value = wthmt.arguments[1]
    assume mtype = wthmt.arguments[0]._type
    if (mtype == null) {
        to.errors |> match_error("unresolved type", wthmt.arguments[0].at)
        to.failed = true
        return false
    }
    if (is_same_type(what, mtype, RefMatters.no, ConstMatters.no, TemporaryMatters.no)) {
        log_m("\tmatch_type {describe(what)} passed\n")
        return match_any(what, value, access, to)
    } else {
        let mt = get_match_type(mtype)
        return match_custom(mt, wthmt, mtype, access, to) if (mt != MatchType.none)
        to.errors |> match_error("mismatching type {describe(what)} vs {describe(mtype)}", mtype.at)
        return false
    }
}

[macro_function]
def match_expr(what : TypeDeclPtr; wthmt : ExprCall?; access : Expression?; var to : MatchTo) {
    if (length(wthmt.arguments) != 1) {
        to.errors |> match_error("match_expr expects 1 argument", wthmt.at)
        return false
    }
    assume value = wthmt.arguments[0]
    if (value._type != null && !compatible(what, value._type)) {
        to.errors |> match_error("mismatching expression type {describe(what)} vs {describe(value._type)}", value.at)
        return false
    }
    var expr = clone_expression(value)
    expr = apply_template(expr) $(rules) {
        bind_match_rules(rules, to, access)
    }
    var cond = qmacro($e(access) == $e(expr))
    cond |> force_at(value.at)
    to.conditions |> emplace(cond)
    return true
}

def private bind_match_rules(var rules : Template; var to : MatchTo; access : Expression?) {
    for (dv, dn in keys(to.declarations), values(to.declarations)) {
        rules |> replaceVariable(dv) <| clone_expression(dn)
    }
    rules |> replaceVarTag("_") <| clone_expression(access)
}

[macro_function]
def match_guards(what : TypeDeclPtr; wth : ExprOp2?; access : Expression?; var to : MatchTo) {
    if (match_any(what, wth.left, access, to)) {
        log_m("\tguards {describe(wth.right)} at {wth.right.at}\n")
        var guards = clone_expression(wth.right)
        guards = apply_template(guards) $(rules) {
            bind_match_rules(rules, to, access)
        }
        to.conditions |> emplace(guards)
        return true
    } else {
        return false
    }
}

[macro_function]
def private op_sides(var e : Expression?; op : string) : array<Expression?> {
    //! The operands of a chain of one binary operator, left to right.
    return <- [e] if (e == null || !(e is ExprOp2) || (e as ExprOp2).op != op)
    var sides <- op_sides((e as ExprOp2).left, op)
    sides |> push_from(op_sides((e as ExprOp2).right, op))
    return <- sides
}

[macro_function]
def private same_names(a, b : table<string; Expression?>; wth : ExprOp2?; var to : MatchTo) : bool {
    //! Every name ``a`` binds, ``b`` binds too.
    for (k in keys(a)) {
        continue if (key_exists(b, k))
        to.errors |> match_error("the sides of {wth.op} must bind the same names; `{k}` is bound on one side only", wth.at)
        return false
    }
    return true
}

[macro_function]
def match_alternatives(var wth : ExprOp2?; var to : MatchTo; matcher : block<(side : Expression?; var sideTo : MatchTo) : bool>) {
    var sides <- op_sides(wth, string(wth.op))
    var firstDecl : table<string; Expression?>
    var conds : array<Expression?>
    var split = false
    for (side, si in sides, count()) {
        var sideTo : MatchTo
        sideTo.declarations := to.declarations
        if (!invoke(matcher, side, sideTo)) {
            to.errors |> push_from(sideTo.errors)
            to.failed ||= sideTo.failed
            return false
        }
        for (k in keys(to.declarations)) {
            sideTo.declarations |> erase(k)
        }
        if (si == 0) {
            firstDecl := sideTo.declarations
        } elif (!same_names(firstDecl, sideTo.declarations, wth, to) || !same_names(sideTo.declarations, firstDecl, wth, to)) {
            return false
        } else {
            for (k, v in keys(sideTo.declarations), values(sideTo.declarations)) {
                split ||= describe(v) != describe(unsafe(firstDecl[k]))
            }
        }
        conds |> push(join_conditions(sideTo.conditions, side.at))
    }
    if (split) {
        to.errors |> match_error("mismatching variable declarations in {wth.op}", wth.at)
        return false
    }
    for (k, v in keys(firstDecl), values(firstDecl)) {
        to.declarations[k] = v
    }
    to.conditions |> push(join_conditions(conds, wth.at, "||"))
    log_m("\tmatch alternatives passed\n")
    return true
}

[macro_function]
def match_bind(what : TypeDeclPtr; name : ExprVar?; access : Expression?; var to : MatchTo) {
    let tname = string(name.name)
    if (to.declarations |> key_exists(tname)) {
        to.errors |> match_error("duplicate variable {tname}", name.at)
        return false
    }
    var cap = clone_expression(access)
    if (is_expression_ptr(what)) {
        cap = qmacro(match_peel_r2v($e(cap)))
        cap |> force_at(name.at)
    }
    unsafe(to.declarations[tname]) = cap
    log_m("\tadd variable {tname} as {describe(access)}\n")
    return true
}

[macro_function]
def match_tag(what : TypeDeclPtr; wth : Expression?; access : Expression?; var to : MatchTo) {
    let tag = wth as ExprTag
    if (tag.name != "v") {
        to.errors |> match_error("unsupported match tag ${tag.name}", tag.at)
        return true
    }
    if (!(tag.subexpr is ExprVar)) {
        to.errors |> match_error("invalid $v syntax", tag.at)
        return false
    }
    return match_bind(what, tag.subexpr as ExprVar, access, to)
}

[macro_function]
def is_match_call(wth : Expression?; name : string) {
    if (!(wth is ExprCall)) return false
    let ecall = wth as ExprCall
    if (ecall.name == "match_{name}" || ecall.name == "match::match_{name}") return true
    var res = false
    peek(ecall.name) $(call_name) {
        res = call_name |> starts_with("__::match`match_{name}")
    }
    return res
}

[macro_function]
def is_to_array_move_call(wth : Expression?) {
    if (!(wth is ExprCall)) return false
    let ecall = wth as ExprCall
    if (ecall.name == "to_array_move") return true
    var res = false
    peek(ecall.name) $(call_name) {
        res = call_name |> starts_with("__::builtin`to_array_move")
    }
    return res
}

[macro_function]
def match_const(what : TypeDeclPtr; wth : Expression?; access : Expression?; var to : MatchTo) {
    if (wth._type == null) {
        to.errors |> match_error("unresolved constant type", wth.at)
        return false
    }
    if (!(wth is ExprConstPtr && what.isPointer) && !compatible(what, wth._type)) {
        to.errors |> match_error("condition type mismatch {describe(what)} vs {describe(wth._type)}", wth.at)
        return false
    }
    log_m("\tadd condition {describe(access)} == {describe(wth)}\n")
    let ee = unsafe(reinterpret<Expression?>(wth))
    var cond = qmacro($e(access) == $e(ee))
    cond |> force_at(wth.at)
    to.conditions |> emplace(cond)
    return true
}

[macro_function]
def match_any(what : TypeDeclPtr; wth : Expression?; access : Expression?; var to : MatchTo) {   // nolint:STYLE037 - exact-type `is` ladder over TypeDecl shapes
    if (LOG_MATCH) {
        log_m("match {describe(what)} {debug_expression(wth)}:\n")
    }
    if (wth is ExprTag) {
        return match_tag(what, wth, access, to)
    } elif (is_wildcard(wth)) {
        log_m("\tmatch anything via _n\n")
        return true
    } elif (wth is ExprMakeStruct) {
        return match_struct(what, wth as ExprMakeStruct, access, to)
    } elif (wth is ExprMakeTuple) {
        return match_tuple(what, wth as ExprMakeTuple, access, to)
    } elif (wth is ExprMakeArray) {
        let mka = wth as ExprMakeArray
        if (length(mka.values) == 1 && (mka.values[0] is ExprMakeTuple)) {
            return match_tuple(what, (mka.values[0] as ExprMakeTuple), access, to)
        } else {
            return match_array(what, wth as ExprMakeArray, false, access, to)
        }
    } elif (wth is ExprMakeVariant) {
        return match_variant(what, wth as ExprMakeVariant, access, to)
    } elif (wth is ExprAsVariant) {
        return match_as_variant(what, wth as ExprAsVariant, access, to)
    } elif (wth |> is_match_call("type")) {
        return match_type(what, wth as ExprCall, access, to)
    } elif (wth |> is_match_call("expr")) {
        return match_expr(what, wth as ExprCall, access, to)
    } elif (wth |> is_to_array_move_call) {
        let cll = wth as ExprCall
        if (length(cll.arguments) != 1) {
            to.errors |> match_error("to_array_move expects 1 argument", cll.at)
            return false
        }
        if (cll.arguments[0] is ExprMakeArray) {
            return match_array(what, cll.arguments[0] as ExprMakeArray, true, access, to)
        } else {
            to.errors |> match_error("to_array_move expects array", cll.at)
            return false
        }
    } elif ((wth is ExprOp2) && (wth as ExprOp2).op == "&&") {
        return match_guards(what, wth as ExprOp2, access, to)
    } elif ((wth is ExprOp2) && (wth as ExprOp2).op == "||") {
        return match_alternatives(wth as ExprOp2, to) $(side : Expression?; var sideTo : MatchTo) {
            return match_any(what, side, access, sideTo)
        }
    } elif (wth.__rtti |> starts_with("ExprConst")) {
        return match_const(what, wth, access, to)
    } elif (wth._type != null && compatible(what, wth._type)) {
        log_m("\tadd condition {describe(access)} == {describe(wth)}\n")
        let ee = unsafe(reinterpret<Expression?>(copy_to_local(wth)))
        var cond = qmacro($e(access) == $e(ee))
        cond |> force_at(wth.at)
        to.conditions |> emplace(cond)
        return true
    } else {
        to.errors |> match_error("usupported match syntax {wth.__rtti} in {describe(wth)}", wth.at)
        to.failed = true
        return false
    }
}

[macro_function]
def join_conditions(var conditions : array<Expression?>; at : LineInfo; joiner : string = "&&") : Expression? {
    let clen = length(conditions)
    if (clen == 0) return new ExprConstBool(value = true, at = at)
    var tail = conditions |> back()
    conditions |> pop()
    while (!empty(conditions)) {
        {
            var last = conditions |> back()
            conditions |> pop()
            var new_tail = new ExprOp2(at = last.at, op := joiner, left = last, right = tail)
            tail = new_tail
        }
    }
    return tail
}

[call_macro(name="match")]
class MatchMacro : AstCallMacro {
    //! Implements `match` macro.
    report_match_errors = true
    def override canVisitArgument(expr : ExprCallMacro?; argIndex : int) : bool =>
        !is_reporting_compilation_errors()
    def override canFoldReturnResult(expr : ExprCallMacro?) : bool => false
    def override preVisit(prog : ProgramPtr; mod : Module?; expr : ExprCallMacro?) : void {
        return if (length(expr.arguments) != 2)
        assume blk = expr.arguments[1]
        return if (!(blk is ExprMakeBlock))
        var eblk = ((blk as ExprMakeBlock)._block as ExprBlock)
        for (ee in eblk.list) {
            if (ee is ExprIfThenElse) {
                (ee as ExprIfThenElse).if_flags.doNotFold = true
            }
        }
    }
    def override visit(prog : ProgramPtr; mod : Module?; expr : ExprCallMacro?) : Expression? {   // nolint:STYLE037 - exact-type `is` ladder over pattern node classes
        macro_verify(length(expr.arguments) == 2, prog, expr.at, "expecting match (what) <| block")
        assume what = expr.arguments[0]
        assume blk = expr.arguments[1]
        macro_verify(what._type != null && !what._type.isAutoOrAlias, prog, what.at, "match argument {what.describe()} failed to compile")
        macro_verify(blk._type.isGoodBlockType, prog, expr.at, "match `block` argument did not resolve")
        macro_verify(blk is ExprMakeBlock, prog, expr.at, "match `block` argument must be immediate block declaration")
        var eblk = ((blk as ExprMakeBlock)._block as ExprBlock)
        macro_verify(empty(eblk.finalList), prog, expr.at, "match `block` argument can't have finally section")
        for (ee in eblk.list) {
            if (ee is ExprIfThenElse) {
                let eite = ee as ExprIfThenElse
                macro_verify(eite.if_false == null, prog, expr.at, "match to `if` can't have `else` section")
            } else {
                macro_error(prog, ee.at, "match `block` argument can only contain match-to calls, not {ee.__rtti}")
                return null
            }
        }
        let access = clone_expression(what)
        var iff : Expression?
        let eli = length(eblk.list)
        var any_errors = false
        var prefix : array<Expression?>
        for (ei in range(eli)) {
            {
                var eto = (eblk.list[eli - ei - 1]) as ExprIfThenElse
                var to : MatchTo
                if (match_any(what._type, eto.cond, access, to)) {
                    log_m("match_any ok\n\n")
                    var nblk = new ExprBlock(at = eto.at)
                    for (l in (eto.if_true as ExprBlock).list) {
                        nblk.list |> emplace_new <| clone_expression(l)
                    }
                    for (l in (eto.if_true as ExprBlock).finalList) {
                        nblk.finalList |> push(clone_expression(l))
                    }
                    apply_template(nblk) $(rules) {
                        for (dv, dn in keys(to.declarations), values(to.declarations)) {
                            rules |> replaceVariable(dv) <| clone_expression(dn)
                        }
                        rules |> replaceVarTag("_") <| clone_expression(access)
                    }
                    if (iff == null && empty(to.conditions)) {
                        iff = nblk
                    } else {
                        var new_iff = new ExprIfThenElse(at = eto.at, cond = join_conditions(to.conditions, eto.at))
                        new_iff.if_true = nblk
                        new_iff.if_false = iff
                        iff = new_iff
                    }
                } elif (report_match_errors || to.failed) {
                    log_m("match_any failed\n\n")
                    any_errors = true
                    if (!empty(to.errors)) {
                        for (e in to.errors) {
                            macro_error(prog, *e.at, e.msg)
                        }
                    } else {
                        macro_error(prog, eto.at, "pattern `{describe(eto.cond)}` does not match `{describe(what._type)}`")
                    }
                    break
                }
                static_if (LOG_MATCH) {
                    for (e in to.errors) {
                        log_m("\tERROR:: {e.msg} at {describe(*e.at)}\n")
                    }
                }
                if (!empty(to.prefix)) {
                    let p0 = length(prefix)
                    prefix |> resize(p0 + length(to.prefix))
                    for (p, i in to.prefix, count(p0)) {
                        prefix[i] = p
                    }
                }
            }
        }
        if (any_errors) return null
        prefix |> emplace(iff)
        return qmacro($b(prefix))
    }
}

[call_macro(name="static_match")]
class StaticMatchMacro : MatchMacro {
    //! Implements `static_match` macro.
    override report_match_errors = false
}

[structure_macro(name="match_as_is")]
class MatchAsIs : AstStructureAnnotation {
    //! Implements `match_as_is` annotation.
    //! This annotation is used to mark that structure can be matched with different type via is and as machinery.
}

[structure_macro(name="match_copy")]
class MatchCopy : AstStructureAnnotation {
    //! Implements `match_copy` annotation.
    //! This annotation is used to mark that structure can be matched with different type via match_copy machinery.
}


def public match_type(anytype, anything) : void {
    //! Match by type. Inside a ``match`` arm, ``match_type(expr, Type)`` checks whether the
    //! matched value has the given type and decomposes ``expr`` against it.
    pass
}

def public match_expr(anything) : void {
    //! Match by expression equality. Inside a ``match`` arm, ``match_expr(expr)`` generates
    //! a condition ``value == expr``. Captured variables (``$v``) are substituted before comparison.
    pass
}

[_macro]
def private setup {
    if (is_compiling_macros_in_module("match")) {
        compiling_module() |> add_keyword("match", false)
        compiling_module() |> add_keyword("static_match", false)
        compiling_module() |> add_keyword("match_type", true)
    }
}
