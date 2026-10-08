options gen2
options indenting = 4
options strict_smart_pointers = true

module match shared private

//! Pattern matching macro for daslang.
//!
//! Provides ``match`` and ``static_match`` macros
//! that decompose values by structure, variant, tuple, array, or constant comparison.
//! Each arm is ``pattern => body``: a bare name binds, ``_`` matches anything, ``&&`` adds a guard,
//! ``|`` separates alternatives, ``&`` requires both patterns, and ``match (a, b)`` matches several
//! values at once.
//! In value position ``match`` is an expression. Unreachable arms and a match over a finite type
//! (enum, bool, variant, struct pointer) that misses a value are compile errors.
//! Also provides ``match_type`` and ``match_expr``.
//! The ``if (pattern)`` arm form with ``$v(name)`` capture still compiles.
//! Structures annotated with ``[match_as_is]`` or ``[match_copy]`` enable custom matching.

require daslib/ast_boost
require daslib/templates_boost
require daslib/macro_boost
require daslib/strings_boost
require daslib/defer
require daslib/option
require daslib/result

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
    bind_names   : bool                             //! a `=>` arm, where a bare `x` in the pattern binds; an `if (pattern)` arm binds only `$v(x)`
    by_ref       : table<string>                    //! bound names the body reads through the access itself, not a `let` copy: pointers and types that can't be copied
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

let private MATCH_BIND_SUFFIX = "`match`bind"

[macro_function]
def bound_name(name : string) : string =>
    name |> ends_with(MATCH_BIND_SUFFIX) ? slice(name, 0, length(name) - length(MATCH_BIND_SUFFIX)) : name

[macro_function]
def describe_pattern(pat : Expression?) : string => replace(describe(pat), MATCH_BIND_SUFFIX, "")

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
def is_arrow_arm(e : Expression?) => e is ExprMakeTuple && length((e as ExprMakeTuple).values) == 2

[macro_function]
def private is_binder(p : Expression?) : bool => p is ExprVar || (p is ExprTag && (p as ExprTag).name == "v")

[macro_function]
def private shape_part(var cell : Expression?) : Expression? {
    //! The destructuring side of a cell: ``n & Point(...)`` is ``Point(...)``; a binder or ``_`` is null (matches anything).
    var p = match_peel_r2v(cell)
    return null if (p == null || is_binder(p))
    if (p is ExprOp2 && (p as ExprOp2).op == "&") {
        var op2 = p as ExprOp2
        return shape_part(op2.right) if (is_binder(match_peel_r2v(op2.left)))
        return shape_part(op2.left) if (is_binder(match_peel_r2v(op2.right)))
    }
    return p
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
    rules |> replaceVarTag("_") <| clone_expression(access) if (access != null)
}

[macro_function]
def private add_guard(guard : Expression?; var to : MatchTo; access : Expression?) : bool {
    var g = clone_expression(guard)
    g = apply_template(g) $(rules) {
        bind_match_rules(rules, to, access)
    }
    to.conditions |> emplace(g)
    return true
}

[macro_function]
def match_guards(what : TypeDeclPtr; wth : ExprOp2?; access : Expression?; var to : MatchTo) : bool =>
    match_any(what, wth.left, access, to) && add_guard(wth.right, to, access)

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
        to.errors |> match_error("the sides of {to.bind_names ? "|" : string(wth.op)} must bind the same names; `{bound_name(k)}` is bound on one side only", wth.at)
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
        sideTo.bind_names = to.bind_names
        sideTo.declarations := to.declarations
        if (!invoke(matcher, side, sideTo)) {
            to.errors |> push_from(sideTo.errors)
            to.failed ||= sideTo.failed
            return false
        }
        for (k in keys(to.declarations)) {
            sideTo.declarations |> erase(k)
        }
        for (k in keys(sideTo.by_ref)) {
            to.by_ref |> insert(k)
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
def match_multi(types : array<TypeDeclPtr>; accesses : array<Expression?>; wth : Expression?; var to : MatchTo) : bool {
    //! Matches one arm against the values: a single value takes the pattern as written, several values take one element of the arm's tuple each.
    return true if (is_wildcard(wth))
    let nwhat = length(types)
    return match_any(types[0], wth, accesses[0], to) if (nwhat == 1)
    var op2 = wth is ExprOp2 ? wth as ExprOp2 : null
    if (op2 != null && op2.op == "&&") return match_multi(types, accesses, op2.left, to) && add_guard(op2.right, to, null)
    if (op2 != null && op2.op == "||") {
        return match_alternatives(op2, to) $(side : Expression?; var sideTo : MatchTo) {
            return match_multi(types, accesses, side, sideTo)
        }
    }
    if (!(wth is ExprMakeTuple) || length((wth as ExprMakeTuple).values) != nwhat) {
        to.errors |> match_error("match on {nwhat} values expects a {nwhat}-element pattern `(p1, ..., p{nwhat})`", wth.at)
        return false
    }
    let mkt = wth as ExprMakeTuple
    for (i in range(nwhat)) {
        return false if (!match_any(types[i], mkt.values[i], accesses[i], to))
    }
    return true
}

[macro_function]
def match_bind(what : TypeDeclPtr; name : ExprVar?; access : Expression?; var to : MatchTo) {
    let tname = string(name.name)
    if (to.declarations |> key_exists(tname)) {
        to.errors |> match_error("duplicate variable {bound_name(tname)}", name.at)
        return false
    }
    var cap = clone_expression(access)
    if (is_expression_ptr(what)) {
        cap = qmacro(match_peel_r2v($e(cap)))
        cap |> force_at(name.at)
    }
    unsafe(to.declarations[tname]) = cap
    to.by_ref |> insert(tname) if (what == null || !what.canCopy || what.isPointer)
    log_m("\tadd variable {tname} as {describe(access)}\n")
    return true
}

[macro_function]
def match_tag(what : TypeDeclPtr; wth : Expression?; access : Expression?; var to : MatchTo) {
    let tag = wth as ExprTag
    if (tag.name != "v") {
        to.errors |> match_error("unsupported match tag ${tag.name}", tag.at)
        return false
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
def is_range_call(pat : Expression?) : bool => pat is ExprCall && (pat as ExprCall).name == "interval" && length((pat as ExprCall).arguments) == 2

[macro_function]
def is_range_const(pat : Expression?) : bool => pat is ExprConstRange || pat is ExprConstURange || pat is ExprConstRange64 || pat is ExprConstURange64

[macro_function]
def range_element_type(typ : TypeDeclPtr) : Type {
    return (typ.baseType == Type.tRange ? Type.tInt : typ.baseType == Type.tURange ? Type.tUInt
        : typ.baseType == Type.tRange64 ? Type.tInt64 : typ.baseType == Type.tURange64 ? Type.tUInt64 : Type.none)
}

[macro_function]
def match_range(what : TypeDeclPtr; wth : Expression?; access : Expression?) : Result<Expression?; string> {
    //! ``(lo..hi)`` matches ``lo <= value && value < hi`` - half-open, like a ``for`` range: the condition, or why the range does not apply.
    if (wth._type == null) {
        return err("range pattern `{describe_pattern(wth)}` failed to compile; its bounds must be constants or variables in scope", type<Expression?>)
    }
    let elem = range_element_type(wth._type)
    if (what.baseType != elem || what.isPointer) {
        return err("range pattern `{describe_pattern(wth)}` matches {das_to_string(elem)} values, not `{describe(what)}`", type<Expression?>)
    }
    let rng = unsafe(reinterpret<Expression?>(wth))
    var cond = (is_range_call(wth)
        ? qmacro($e(access) >= $e((wth as ExprCall).arguments[0]) && $e(access) < $e((wth as ExprCall).arguments[1]))
        : qmacro($e(access) >= $e(rng).x && $e(access) < $e(rng).y))
    cond |> force_at(wth.at)
    return move_ok(cond, type<string>)
}

[macro_function]
def private add_condition(var r : Result<Expression?; string>; wth : Expression?; var to : MatchTo) : bool {
    //! A leaf pattern's condition joins the arm; its error is reported instead - a hard one when the pattern itself did not compile.
    if (is_err(r)) {
        to.errors |> match_error(unwrap_err(r), wth.at)
        to.failed ||= wth._type == null
        return false
    }
    to.conditions |> push(move_unwrap(r))
    return true
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
    var cond = (wth is ExprConstBool && what.baseType == Type.tBool
        ? ((wth as ExprConstBool).value ? qmacro($e(access)) : qmacro(!$e(access)))
        : qmacro($e(access) == $e(ee)))
    cond |> force_at(wth.at)
    to.conditions |> emplace(cond)
    return true
}

[macro_function]
def match_any(what : TypeDeclPtr; wth : Expression?; access : Expression?; var to : MatchTo) {   // nolint:STYLE037 - exact-type `is` ladder over TypeDecl shapes
    if (LOG_MATCH) {
        log_m("match {describe(what)} {debug_expression(wth)}:\n")
    }
    if (to.bind_names) {
        let bare = match_peel_r2v(wth)
        if ((bare is ExprVar) && (bare as ExprVar).name != "_") return match_bind(what, bare as ExprVar, access, to)
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
    } elif (is_range_const(wth) || is_range_call(wth)) {
        return add_condition(match_range(what, wth, access), wth, to)
    } elif (wth.__rtti |> starts_with("ExprConst")) {
        return match_const(what, wth, access, to)
    } elif (to.bind_names && (wth is ExprOp2) && (wth as ExprOp2).op == "&") {
        return match_any(what, (wth as ExprOp2).left, access, to) && match_any(what, (wth as ExprOp2).right, access, to)
    } elif (to.bind_names) {
        to.errors |> match_error("`{describe_pattern(wth)}` is not a pattern; compare against a computed value with `match_expr({describe_pattern(wth)})`", wth.at)
        return false
    } elif (wth._type != null && compatible(what, wth._type)) {
        log_m("\tadd condition {describe(access)} == {describe(wth)}\n")
        let ee = unsafe(reinterpret<Expression?>(copy_to_local(wth)))
        var cond = qmacro($e(access) == $e(ee))
        cond |> force_at(wth.at)
        to.conditions |> emplace(cond)
        return true
    } else {
        to.errors |> match_error("unsupported match syntax {wth.__rtti} in {describe(wth)}", wth.at)
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

[macro_function]
def private bound_let(name : string; value : Expression?) : Expression? {
    //! A name a ``=>`` arm binds, at the start of the arm: a read-only copy of the matched part.
    var decl = qmacro_expr() {
        let $i(name) = $e(value)
    }
    decl |> force_at(value.at)
    return decl
}

[macro_function]
def arm_body(arm : Expression?; as_value : bool; binds : array<Expression?>) : ExprBlock? {
    //! The arm's body as a fresh block, after the ``let`` of every name the arm binds.
    var nblk = new ExprBlock(at = arm.at)
    for (b in binds) {
        nblk.list |> emplace_new <| clone_expression(b)
    }
    var body = arm is ExprIfThenElse ? (arm as ExprIfThenElse).if_true : (arm as ExprMakeTuple).values[1]
    if (as_value) {
        nblk.list |> emplace_new <| new ExprReturn(at = arm.at, subexpr = clone_expression(body))
        return nblk
    }
    body = body is ExprMakeBlock ? (body as ExprMakeBlock)._block : body
    if (!(body is ExprBlock)) {
        nblk.list |> emplace_new <| clone_expression(body)
        return nblk
    }
    let src = body as ExprBlock
    for (l in src.list) {
        nblk.list |> emplace_new <| clone_expression(l)
    }
    for (l in src.finalList) {
        nblk.finalList |> push(clone_expression(l))
    }
    return nblk
}

[macro_function]
def prepare_sub_patterns(var pat : Expression?; bind_bare : bool; var names : array<string>) : void {
    if (pat is ExprMakeTuple) {
        for (v in (pat as ExprMakeTuple).values) {
            prepare_pattern(v, bind_bare, names)
        }
    } elif (pat is ExprMakeArray) {
        for (v in (pat as ExprMakeArray).values) {
            prepare_pattern(v, bind_bare, names)
        }
    } elif (pat is ExprMakeStruct) {
        for (st in (pat as ExprMakeStruct).structs) {
            for (mf in *st) {
                prepare_pattern(mf.value, bind_bare, names)
            }
        }
    } elif (pat is ExprMakeVariant) {
        for (mf in (pat as ExprMakeVariant).variants) {
            prepare_pattern(mf.value, bind_bare, names)
        }
    } elif (pat is ExprAsVariant) {
        prepare_pattern((pat as ExprAsVariant).value, bind_bare, names)
    } elif (pat is ExprCall && !is_match_call(pat, "expr") && !is_range_call(pat)) {
        for (a in (pat as ExprCall).arguments) {
            prepare_pattern(a, bind_bare, names)
        }
    }
}

[macro_function]
def prepare_pattern(var pat : Expression?; bind_bare : bool; var names : array<string>) : void {
    //! Before infer: rewrites pattern-level ``|`` into ``||`` (infer would const-fold ``1 | 2`` into ``3``) and collects the names the pattern binds.
    if (pat is ExprOp2) {
        var op2 = pat as ExprOp2
        if (op2.op == "|") {
            op2.op := "||"
        }
        prepare_pattern(op2.left, bind_bare, names)
        prepare_pattern(op2.right, bind_bare, names) if (op2.op == "||" || (bind_bare && op2.op == "&"))
    } elif (pat is ExprVar) {
        let vname = string((pat as ExprVar).name)
        names |> push(vname) if (bind_bare && vname != "_")
    } elif (pat is ExprTag) {
        let tag = pat as ExprTag
        names |> push(string((tag.subexpr as ExprVar).name)) if (tag.name == "v" && tag.subexpr is ExprVar)
    } else {
        prepare_sub_patterns(pat, bind_bare, names)
    }
}

class private MatchBindRenamer : AstVisitor {
    names : table<string>
    @do_not_delete adapter : VisitorAdapter?
    def hidden(var name : das_string) : void {
        let n = string(name)
        if (key_exists(names, n)) {
            name := n + MATCH_BIND_SUFFIX
        }
    }
    def override visitExprVar(var expr : ExprVar?) : Expression? {
        hidden(expr.name)
        return expr
    }
    def override visitExprAddr(var expr : ExprAddr?) : Expression? {
        hidden(expr.target)
        return expr
    }
    def override visitExprLet(var expr : ExprLet?) : Expression? {
        for (v in expr.variables) {
            hidden(v.name)
        }
        return expr
    }
    def override visitExprTag(var expr : ExprTag?) : Expression? {
        if (expr.subexpr != null && expr.subexpr is ExprVar) {
            hidden((expr.subexpr as ExprVar).name)
        }
        return expr
    }
    def override preVisitExprCallMacro(var expr : ExprCallMacro?) : void {
        //! Reification macros (``qmacro`` and friends) keep their arguments away from visitors; their ``$v(name)`` tags still read the bound name.
        for (a in expr.arguments) {
            visit(a, adapter)
        }
    }
}

[macro_function]
def hide_bound_names(arm : Expression?; var pattern : Expression?; bind_bare : bool) : void {
    //! Before infer: gives every name the arm binds a private spelling across the whole arm - qmacro tags included - so infer can not resolve (and const-fold) it as an outer variable of the same name.
    var names : array<string>
    prepare_pattern(pattern, bind_bare, names)
    var renamer = new MatchBindRenamer()
    for (n in names) {
        renamer.names |> insert(n) if (!ends_with(n, MATCH_BIND_SUFFIX))
    }
    if (!empty(renamer.names)) {
        make_visitor(*renamer) $(adapter) {
            renamer.adapter = adapter
            visit(arm, adapter)
            renamer.adapter = null
        }
    }
    unsafe {
        delete renamer
    }
}

class private MatchParentFinder : AstVisitor {
    //! Finds where a ``match`` call sits: a block statement, or a value - typed when it is a function-level ``return`` of a function with a declared result, or a ``let`` with a declared type.
    target : Expression const?
    is_statement : bool
    @do_not_delete result_type : TypeDecl?
    @do_not_delete value_type : TypeDecl?
    block_depth : int
    def is_target(expr : Expression const?) : bool => intptr(expr) == intptr(target)
    def override preVisitFunction(var fun : FunctionPtr) : void {
        if (fun.result != null && !fun.result.isAuto) {
            result_type = fun.result
        }
    }
    def override preVisitExprBlockExpression(blk : ExprBlock?; expr : ExpressionPtr) : void {
        is_statement ||= is_target(expr)
    }
    def override preVisitExprMakeBlock(expr : ExprMakeBlock?) : void {
        block_depth ++
    }
    def override visitExprMakeBlock(var expr : ExprMakeBlock?) : Expression? {
        block_depth --
        return expr
    }
    def override preVisitExprReturn(expr : ExprReturn?) : void {
        if (block_depth == 0 && is_target(expr.subexpr)) {
            value_type = result_type
        }
    }
    def override visitExprLet(var expr : ExprLet?) : Expression? {
        for (v in expr.variables) {
            if (is_target(v.init) && v._type != null && !v._type.isAuto) {
                value_type = v._type
            }
        }
        return expr
    }
}

struct private MatchParent {
    is_statement : bool
    value_type : TypeDeclPtr
}

[macro_function]
def match_parent(expr : ExprCallMacro?) : MatchParent {
    //! Where the ``match`` call sits; ``value_type`` is ``auto`` unless the value's type is declared.
    var res = MatchParent(value_type = new TypeDecl(baseType = Type.autoinfer, at = expr.at))
    return res if (expr.inFunction == null)
    var finder = new MatchParentFinder(target = expr)
    make_visitor(*finder) $(adapter) {
        visit(expr.inFunction, adapter)
    }
    res.is_statement = finder.is_statement
    if (finder.value_type != null) {
        res.value_type = clone_type(finder.value_type)
        res.value_type.flags.constant = false
        res.value_type.flags.ref = false
    }
    unsafe {
        delete finder
    }
    return <- res
}

let private MATCH_ANY_OTHER = "_"
let private MATCH_NON_NULL = "non-null"

struct private MatchCell {
    any : bool
    opaque : bool
    keys : array<string>
}

struct private MatchDomain {
    finite : bool
    values : array<string>
}

struct private PatternRow {
    arm : int
    @do_not_delete cells : array<Expression?>
}

struct private MatchTable {
    columns : array<TypeDeclPtr>
    labels : array<string>
    rows : array<array<MatchCell>>
    arm_rows : array<array<int>>
    domains : array<MatchDomain>
}

struct private MatchCoverage {
    unreachable : array<int>
    dead_alternatives : array<tuple<arm : int; what : string>>
    all_finite : bool
    has_guards : bool
    missing : Option<string>
}


[macro_function]
def private bind_subject(var what : Expression?; idx : int; var prefix : array<Expression?>) : Expression? {
    //! Evaluates the scrutinee once, into a temp: a reference to an lvalue, otherwise the value copied or moved.
    let tname = make_unique_private_name("__`match`subject`{idx}", what.at)
    let lvalue = what._type.flags.ref || match_peel_r2v(what) is ExprVar
    var decl : Expression?
    if (lvalue && what._type.flags.constant) {
        decl = qmacro_expr() {
            let $i(tname) & = unsafe($e(what))
        }
    } elif (lvalue) {
        decl = qmacro_expr() {
            var $i(tname) & = unsafe($e(what))
        }
    } elif (what._type.canCopy) {
        decl = qmacro_expr() {
            let $i(tname) = $e(what)
        }
    } else {
        decl = qmacro_expr() {
            let $i(tname) <- $e(what)
        }
    }
    decl |> force_at(what.at)
    var init = (decl as ExprLet).variables[0].init
    init.genFlags.alwaysSafe = true
    init.genFlags.userSaidItsSafe = true
    prefix |> emplace(decl)
    var access = qmacro($i(tname))
    access |> force_at(what.at)
    return access
}


[macro_function]
def private constant_key(p : Expression?) : string {
    if (p is ExprConstEnumeration) {
        let ce = p as ExprConstEnumeration
        return "{ce.enumType.name}.{ce.value}"
    }
    return p.__rtti |> starts_with("ExprConst") && !is_range_const(p) ? describe(p) : ""
}

[macro_function]
def private int_value(var e : Expression?) : Option<int64> {
    let p = match_peel_r2v(e)
    return some(int64((p as ExprConstInt).value)) if (p is ExprConstInt)
    return some(int64((p as ExprConstUInt).value)) if (p is ExprConstUInt)
    return some((p as ExprConstInt64).value) if (p is ExprConstInt64)
    return none(type<int64>)
}

[macro_function]
def private constant_span(var e : Expression?) : Option<range64> {
    //! The half-open span of integers a constant range - or a single integer constant - matches.
    var p = match_peel_r2v(e)
    if (p is ExprConstRange) {
        let r = (p as ExprConstRange).value
        return some(interval(int64(r.x), int64(r.y)))
    } elif (p is ExprConstURange) {
        let r = (p as ExprConstURange).value
        return some(interval(int64(r.x), int64(r.y)))
    } elif (p is ExprConstRange64) {
        return some((p as ExprConstRange64).value)
    } elif (is_range_call(p)) {
        let lo = int_value((p as ExprCall).arguments[0])
        let hi = int_value((p as ExprCall).arguments[1])
        return is_some(lo) && is_some(hi) ? some(interval(unwrap(lo), unwrap(hi))) : none(type<range64>)
    }
    let v = int_value(p)
    return is_some(v) ? some(interval(unwrap(v), unwrap(v) + 1l)) : none(type<range64>)
}

[macro_function]
def private cell_spans(var e : Expression?) : array<range64> {
    //! The constant spans in a cell - a constant range or an integer constant each - through ``|`` and ``&``.
    var p = match_peel_r2v(e)
    if (p != null && p is ExprOp2 && ((p as ExprOp2).op == "||" || (p as ExprOp2).op == "&")) {
        var spans <- cell_spans((p as ExprOp2).left)
        spans |> push_from(cell_spans((p as ExprOp2).right))
        return <- spans
    }
    let span = p == null ? none(type<range64>) : constant_span(p)
    return <- [for (s in [span]); unwrap(s); where is_some(s)]
}

[macro_function]
def private column_cuts(var rows : array<PatternRow>; c : int) : array<int64> {
    //! A column with constant ranges splits the integers at every constant end, so a range and a constant name the same pieces; empty without ranges.
    var spans : array<range64>
    for (r in rows) {
        spans |> push_from(cell_spans(r.cells[c]))
    }
    let ranged = has_value([for (s in spans); s.y - s.x != 1l], true)
    var cuts : array<int64>
    for (s in spans) {
        cuts |> push_from([s.x, s.y])
    }
    sort(cuts)
    return <- [for (v, i in cuts, count()); v; where ranged && (i == 0 || cuts[i - 1] != v)]
}

[macro_function]
def private span_keys(var p : Expression?; cuts : array<int64>) : array<string> {
    //! The pieces between ``cuts`` a constant span covers, named ``lo..hi`` (``v`` for one value).
    let span = constant_span(p)
    return <- [for (i in range(length(cuts) - 1)); cuts[i + 1] == cuts[i] + 1l ? "{cuts[i]}" : "{cuts[i]}..{cuts[i + 1]}";
        where is_some(span) && cuts[i] >= unwrap(span).x && cuts[i + 1] <= unwrap(span).y]
}

[macro_function]
def private variant_name(typ : TypeDeclPtr) : string {
    //! The name a variant is declared with: its alias, or the ``aka`` name ``describe`` prints for a copied type.
    return string(typ.alias) if (!empty(typ.alias))
    let d = describe(typ)
    let aka_pos = find(d, " aka ")
    return "variant" if (aka_pos < 0)
    let name = slice(d, aka_pos + 5)
    let stop = find(name, " ")
    return stop < 0 ? name : slice(name, 0, stop)
}

[macro_function]
def private variant_key(typ : TypeDeclPtr; alt : das_string) : string => "{variant_name(typ)}({alt} = _)"

[macro_function]
def private is_struct_of(typ : TypeDeclPtr; p : Expression?) : bool {
    //! ``p`` is a one-element struct pattern of exactly ``typ``.
    return false if (typ == null || !typ.isStructure || !(p is ExprMakeStruct))
    let ms = p as ExprMakeStruct
    return (length(ms.structs) == 1 && ms.makeType != null && ms.makeType.isStructure
        && is_same_type(typ, ms.makeType, RefMatters.no, ConstMatters.no, TemporaryMatters.no))
}

[macro_function]
def private is_irrefutable(typ : TypeDeclPtr; var pat : Expression?) : bool {
    //! Irrefutable: the pattern can not fail on any value of ``typ`` (on a pointer, any non-null one) - ``x``, ``_``, ``(a, _)``, ``Point(x = a, y = _)``. Such an arm covers its whole column.
    var p = match_peel_r2v(pat)
    return true if (is_binder(p))
    if (p is ExprOp2 && (p as ExprOp2).op == "&") return is_irrefutable(typ, (p as ExprOp2).left) && is_irrefutable(typ, (p as ExprOp2).right)
    if (p is ExprMakeTuple) {
        for (v in (p as ExprMakeTuple).values) {
            return false if (!is_rest(v) && !is_irrefutable(null, v))
        }
        return true
    }
    return false if (typ == null || !is_struct_of(typ.isPointer ? typ.firstType : typ, p))
    for (mf in *(p as ExprMakeStruct).structs[0]) {
        return false if (!is_irrefutable(null, mf.value))
    }
    return true
}

[macro_function]
def private either(var l : MatchCell; var r : MatchCell) : MatchCell {
    //! ``p1 | p2`` covers what either side covers.
    l.any ||= r.any
    l.opaque ||= r.opaque
    l.keys |> push_from(r.keys)
    return <- l
}

[macro_function]
def private both(var l : MatchCell; var r : MatchCell) : MatchCell {
    //! ``p1 & p2`` covers what both sides cover.
    return <- r if (l.any)
    return <- l if (r.any)
    return <- MatchCell(opaque = true) if (l.opaque || r.opaque)
    return <- MatchCell(keys <- [for (k in l.keys); k; where has_value(r.keys, k)])
}

[macro_function]
def private classify_cell(typ : TypeDeclPtr; var pat : Expression?; cuts : array<int64>) : MatchCell {
    //! What one pattern covers of one column: everything, named values, or values the analysis can't name.
    var p = match_peel_r2v(pat)
    return <- MatchCell(any = true) if (is_binder(p))
    if (p is ExprOp2) {
        var op2 = p as ExprOp2
        return <- either(classify_cell(typ, op2.left, cuts), classify_cell(typ, op2.right, cuts)) if (op2.op == "||")
        return <- both(classify_cell(typ, op2.left, cuts), classify_cell(typ, op2.right, cuts)) if (op2.op == "&")
        return <- MatchCell(opaque = true)
    }
    return <- MatchCell(keys <- span_keys(p, cuts)) if (!empty(cuts) && is_some(constant_span(p)))
    let key = constant_key(p)
    return <- MatchCell(keys <- [key]) if (!empty(key))
    if (typ != null && typ.isVariant && p is ExprMakeVariant && length((p as ExprMakeVariant).variants) == 1
            && is_irrefutable(null, (p as ExprMakeVariant).variants[0].value)) {
        return <- MatchCell(keys <- [variant_key(typ, (p as ExprMakeVariant).variants[0].name)])
    }
    if (typ != null && typ.isVariant && p is ExprAsVariant && is_irrefutable(null, (p as ExprAsVariant).value)) {
        return <- MatchCell(keys <- [variant_key(typ, (p as ExprAsVariant).name)])
    }
    return <- MatchCell(opaque = true) if (!is_irrefutable(typ, p))
    return <- MatchCell(keys <- [MATCH_NON_NULL]) if (typ != null && typ.isPointer)
    return <- MatchCell(any = true)
}

[macro_function]
def private finite_domain(typ : TypeDeclPtr) : MatchDomain {
    if (typ != null && typ.isEnum && typ.enumType != null) {
        return <- MatchDomain(finite = true, values <- [for (e in typ.enumType.list); "{typ.enumType.name}.{e.name}"])
    } elif (typ != null && typ.baseType == Type.tBool) {
        return <- MatchDomain(finite = true, values <- ["true", "false"])
    } elif (typ != null && typ.isVariant) {
        return <- MatchDomain(finite = true, values <- [for (n in typ.argNames); variant_key(typ, n)])
    } elif (typ != null && typ.isPointer && typ.firstType != null && typ.firstType.isStructure) {
        return <- MatchDomain(finite = true, values <- ["null", MATCH_NON_NULL])
    }
    return <- MatchDomain()
}

[macro_function]
def private column_domain(col : TypeDeclPtr; c : int; rows : array<array<MatchCell>>) : MatchDomain {
    //! A finite type's values; for any other type, the values the arms name plus "anything else".
    var dom <- finite_domain(col)
    return <- dom if (dom.finite)
    for (r in rows) {
        for (k in r[c].keys) {
            dom.values |> push(k) if (!has_value(dom.values, k))
        }
    }
    dom.values |> push(MATCH_ANY_OTHER)
    return <- dom
}

[macro_function]
def private cell_covers(cell : MatchCell; value : string) : bool => cell.any || cell.keys |> has_value(value)

[macro_function]
def private is_useful(tab : MatchTable; live : array<int>; cand : array<MatchCell>; col : int) : bool {
    //! True when ``cand`` matches some value none of the ``live`` rows matches.
    return empty(live) if (col == length(cand))
    assume cell = cand[col]
    for (v in cell.any || cell.opaque ? tab.domains[col].values : cell.keys) {
        var inscope sub <- [for (ri in live); ri; where cell_covers(tab.rows[ri][col], v)]
        return true if (is_useful(tab, sub, cand, col + 1))
    }
    return false
}

[macro_function]
def private format_witness(values : array<string>; labels : array<string>) : string {
    //! One missing combination: positional ``(a, b)``, or ``(field = value, ...)`` once a column was split into parts.
    if (!has_value([for (l in labels); empty(l)], false)) {
        return length(values) == 1 ? values[0] : "({join(values, ", ")})"
    }
    let items <- [for (v, l in values, labels); empty(l) ? v : "{l} = {v}"; where v != MATCH_ANY_OTHER]
    return empty(items) ? MATCH_ANY_OTHER : "({join(items, ", ")})"
}

[macro_function]
def private missing_values(tab : MatchTable; live : array<int>; col : int; limit : int; var path : array<string>; var found : array<string>) : void {
    //! Collects value combinations no ``live`` row matches - one more than are shown, so the message can say there are more.
    return if (length(found) > limit)
    if (col == length(tab.domains) || empty(live)) {
        if (empty(live)) {
            var inscope full := path
            full |> push_from([for (_c in range(col, length(tab.domains))); MATCH_ANY_OTHER])
            found |> push(format_witness(full, tab.labels))
        }
        return
    }
    if (!has_value([for (ri in live); tab.rows[ri][col].any], false)) {
        path |> push(MATCH_ANY_OTHER)
        missing_values(tab, live, col + 1, limit, path, found)
        path |> pop()
        return
    }
    path |> reserve(length(tab.domains))
    for (v in tab.domains[col].values) {
        var inscope sub <- [for (ri in live); ri; where cell_covers(tab.rows[ri][col], v)]
        path |> push(v)
        missing_values(tab, sub, col + 1, limit, path, found)
        path |> pop()
        return if (length(found) > limit)
    }
}

[macro_function]
def private pattern_part(var pat : Expression?) : Expression? {
    //! The pattern of an arm without its top-level ``&&`` guard.
    var p = match_peel_r2v(pat)
    while (p is ExprOp2 && (p as ExprOp2).op == "&&") {
        p = match_peel_r2v((p as ExprOp2).left)
    }
    return p
}

[macro_function]
def private is_guarded(var pat : Expression?) : bool {
    let p = match_peel_r2v(pat)
    return p is ExprOp2 && (p as ExprOp2).op == "&&"
}

[macro_function]
def private describe_row(row : array<MatchCell>) : string {
    let cells <- [for (c in row); c.any || c.opaque || empty(c.keys) ? "_" : join(c.keys, " | ")]
    return length(cells) == 1 ? cells[0] : "({join(cells, ", ")})"
}

[macro_function]
def private dead_keys(tab : MatchTable; prev : array<int>; row : array<MatchCell>) : array<string> {
    //! In a single-column row, the named alternatives an earlier arm already matches.
    var dead : array<string>
    return <- dead if (length(row) != 1 || row[0].any || row[0].opaque || length(row[0].keys) < 2)
    dead <- [for (k in row[0].keys); k; where !is_useful(tab, prev, [MatchCell(keys <- [k])], 0)]
    return <- dead
}

[macro_function]
def private alternatives_of(var cell : Expression?) : array<Expression?> {
    //! A cell's top-level ``|`` alternatives, each without a binding ``n &``; null for one that matches anything.
    return <- [for (a in op_sides(shape_part(cell), "||")); shape_part(a)]
}

[macro_function]
def private column_parts(var typ : TypeDeclPtr) : array<tuple<name : string; typ : TypeDeclPtr>> {
    //! The parts a tuple or struct column splits into; none for any other type.
    var parts : array<tuple<name : string; typ : TypeDeclPtr>>
    if (typ != null && typ.baseType == Type.tTuple) {
        parts <- [for (t, i in typ.argTypes, count()); (name = "_{i}", typ = t)]
    } elif (typ != null && typ.isStructure && !typ.isPointer && typ.structType != null) {
        parts <- [for (f in typ.structType.fields); (name = string(f.name), typ = f._type)]
    }
    return <- parts
}

[macro_function]
def private splits(typ : TypeDeclPtr; alt : Expression?) : bool {
    //! The cell spells the whole value part by part: a struct pattern of exactly the column type, or a full tuple.
    return true if (alt == null || is_struct_of(typ, alt))
    return false if (!(alt is ExprMakeTuple) || typ.baseType != Type.tTuple || length((alt as ExprMakeTuple).values) != length(typ.argTypes))
    return !has_value([for (v in (alt as ExprMakeTuple).values); is_rest(v)], true)
}

[macro_function]
def private part_of(var alt : Expression?; idx : int; name : string) : Expression? {
    return null if (alt == null)
    return (alt as ExprMakeTuple).values[idx] if (alt is ExprMakeTuple)
    for (mf in *(alt as ExprMakeStruct).structs[0]) {
        return mf.value if (mf.name == name)
    }
    return null
}

[macro_function]
def private split_column(var tab : MatchTable; var rows : array<PatternRow>; c : int) : bool {
    //! Splits column ``c`` into its parts when every arm spells it part by part; an alternative becomes one row each.
    var parts <- column_parts(tab.columns[c])
    return false if (empty(parts))
    var alts <- [for (r in rows); alternatives_of(r.cells[c])]
    for (ra in alts) {
        return false if (has_value([for (a in ra); splits(tab.columns[c], a)], false))
    }
    var split : array<PatternRow>
    split |> reserve(length(rows))
    for (r, ra in rows, alts) {
        for (a in ra) {
            var nr <- PatternRow(arm = r.arm, cells := r.cells)
            nr.cells |> erase(c)
            nr.cells |> reserve(length(nr.cells) + length(parts))
            for (p, i in parts, count()) {
                nr.cells |> push(part_of(a, i, p.name), c + i)
            }
            split |> emplace(nr)
        }
    }
    delete rows
    rows <- split
    let parent = tab.labels[c]
    tab.columns |> erase(c)
    tab.labels |> erase(c)
    for (p, i in parts, count()) {
        tab.columns |> push(p.typ, c + i)
        tab.labels |> push(empty(parent) ? p.name : "{parent}.{p.name}", c + i)
    }
    return true
}

[macro_function]
def private pattern_rows(var types : array<TypeDeclPtr>; var patterns : array<Expression?>) : array<PatternRow> {
    //! One row of cells per arm, per top-level alternative of a several-value match; a dropped arm has none.
    var rows : array<PatternRow>
    rows |> reserve(length(patterns))
    for (pat, ai in patterns, count()) {
        continue if (pat == null)
        var part = pattern_part(pat)
        var alts <- length(types) > 1 ? alternatives_of(part) : [part]
        for (a in alts) {
            var r <- PatternRow(arm = ai)
            if (length(types) > 1 && a != null && a is ExprMakeTuple && length((a as ExprMakeTuple).values) == length(types)) {
                r.cells <- [for (v in (a as ExprMakeTuple).values); v]
            } else {
                r.cells <- [for (_t in types); a]
            }
            rows |> emplace(r)
        }
    }
    return <- rows
}

[macro_function]
def private match_table(var types : array<TypeDeclPtr>; var patterns : array<Expression?>) : MatchTable {
    //! Columns (struct and tuple columns split into their parts), the rows every arm contributes (by index), and each column's value domain.
    var tab <- MatchTable(columns := types, labels <- [for (_t in types); ""])
    var rows <- pattern_rows(types, patterns)
    var c = 0
    while (c < length(tab.columns)) {
        c += split_column(tab, rows, c) ? 0 : 1
    }
    tab.arm_rows <- [for (ai in range(length(patterns))); [for (r, ri in rows, count()); ri; where r.arm == ai]]
    let cuts <- [for (ci in range(length(tab.columns))); column_cuts(rows, ci)]
    tab.rows <- [for (r in rows); [for (cell, ci in r.cells, count()); cell == null ? MatchCell(any = true) : classify_cell(tab.columns[ci], cell, cuts[ci])]]
    tab.domains <- [for (col, ci in tab.columns, count()); column_domain(col, ci, tab.rows)]
    return <- tab
}

bitfield private GuardCover {
    //! The outcomes a guard covers: of a comparison between two values, or of a bool.
    less
    equal
    greater
    holds
    fails
}

struct private GuardFact {
    key : string
    mask : GuardCover
}

[macro_function]
def private is_ordered_type(typ : TypeDeclPtr) : bool {
    //! Comparisons over these types are total: exactly one of ``<``, ``==``, ``>`` holds. Floats are not (NaN).
    return false if (typ == null)
    return (typ.isEnum || typ.baseType == Type.tString || typ.baseType == Type.tInt || typ.baseType == Type.tUInt
        || typ.baseType == Type.tInt8 || typ.baseType == Type.tUInt8 || typ.baseType == Type.tInt16
        || typ.baseType == Type.tUInt16 || typ.baseType == Type.tInt64 || typ.baseType == Type.tUInt64)
}

[macro_function]
def private operand_type(var e : Expression?; var names : table<string; TypeDeclPtr>) : TypeDeclPtr {
    //! The type of a guard operand: a name the pattern binds at a column, or an already typed variable or constant.
    var x = match_peel_r2v(e)
    return null if (x == null || !(x is ExprVar || x.__rtti |> starts_with("ExprConst")))
    if (x is ExprVar) {
        let n = string((x as ExprVar).name)
        return names[n] if (names |> key_exists(n))
    }
    return x._type
}

[macro_function]
def private guard_fact(var guard : Expression?; pattern_key : string; var names : table<string; TypeDeclPtr>) : Option<GuardFact> {
    //! What a single comparison guard covers of the relation between its two operands; none when it can't tell.
    var g = match_peel_r2v(guard)
    let negated = g is ExprOp1 && (g as ExprOp1).op == "!"
    g = negated ? match_peel_r2v((g as ExprOp1).subexpr) : g
    if (g is ExprOp2) {
        var op2 = g as ExprOp2
        let masks <- { "<" => GuardCover.less, "<=" => GuardCover.less | GuardCover.equal, "==" => GuardCover.equal,
            "!=" => GuardCover.less | GuardCover.greater, ">=" => GuardCover.greater | GuardCover.equal, ">" => GuardCover.greater }
        var mask = masks?[string(op2.op)] ?? default<GuardCover>
        if (mask != default<GuardCover> && is_ordered_type(operand_type(op2.left, names)) && is_ordered_type(operand_type(op2.right, names))) {
            var l = describe(match_peel_r2v(op2.left))
            var r = describe(match_peel_r2v(op2.right))
            if (l > r) {
                swap(l, r)
                let less = mask.less
                mask.less = mask.greater
                mask.greater = less
            }
            let full = GuardCover.less | GuardCover.equal | GuardCover.greater
            return some(GuardFact(key = "{pattern_key} | {l} ? {r}", mask = negated ? full & ~mask : mask))
        }
    }
    let gt = operand_type(g, names)
    return some(GuardFact(key = "{pattern_key} | {describe(g)}", mask = negated ? GuardCover.fails : GuardCover.holds)) if (gt != null && gt.baseType == Type.tBool)
    return none(type<GuardFact>)
}

[macro_function]
def private bound_column_types(var columns : array<TypeDeclPtr>; var pat : Expression?) : table<string; TypeDeclPtr> {
    //! Names the pattern binds directly at a column, with that column's type; a single tuple column binds per element.
    var names : table<string; TypeDeclPtr>
    var p = match_peel_r2v(pat)
    if (length(columns) == 1 && p is ExprVar) {
        names[string((p as ExprVar).name)] = columns[0]
    }
    let per_element = length(columns) == 1 && columns[0].baseType == Type.tTuple
    var cols <- per_element ? [for (t in columns[0].argTypes); t] : [for (t in columns); t]
    return <- names if (!(p is ExprMakeTuple) || length((p as ExprMakeTuple).values) != length(cols))
    var vars <- [for (v in (p as ExprMakeTuple).values); match_peel_r2v(v)]
    return <- {for (v, col in vars, cols); string((v as ExprVar).name) => col; where v is ExprVar}
}

[macro_function]
def private guards_complete(var types : array<TypeDeclPtr>; var pat : Expression?; var guard_masks : table<string; GuardCover>) : bool {
    //! Arms with one pattern and comparison guards over the same operands cover that pattern once their guards cover every outcome.
    var p = match_peel_r2v(pat)
    return false if (is_guarded((p as ExprOp2).left))
    var part = pattern_part(p)
    var names <- bound_column_types(types, part)
    let found = guard_fact((p as ExprOp2).right, describe(part), names)
    return false if (is_none(found))
    let fact = unwrap(found)
    let mask = (guard_masks?[fact.key] ?? default<GuardCover>) | fact.mask
    guard_masks[fact.key] = mask
    return mask == (fact.mask.holds || fact.mask.fails ? GuardCover.holds | GuardCover.fails : GuardCover.less | GuardCover.equal | GuardCover.greater)
}

[macro_function]
def private match_coverage(var types : array<TypeDeclPtr>; var patterns : array<Expression?>) : MatchCoverage {
    //! Which arms or alternatives no value can reach, and whether every value reaches some arm; ``patterns`` is in source order, null for an arm that was dropped.
    var res : MatchCoverage
    var tab <- match_table(types, patterns)
    res.all_finite = !has_value([for (d in tab.domains); d.finite], false)
    var inscope prev : array<int>
    var inscope guard_masks : table<string; GuardCover>
    for (pat, ai in patterns, count()) {
        continue if (pat == null)
        let guarded = is_guarded(pat)
        res.has_guards ||= guarded
        var useful = false
        var inscope dead : array<string>
        for (ri in tab.arm_rows[ai]) {
            if (is_useful(tab, prev, tab.rows[ri], 0)) {
                useful = true
                dead |> push_from(dead_keys(tab, prev, tab.rows[ri]))
            } else {
                dead |> push(describe_row(tab.rows[ri]))
            }
        }
        if (useful) {
            res.dead_alternatives |> push_from([for (d in dead); (arm = ai, what = d)])
        } else {
            res.unreachable |> push(ai)
        }
        prev |> push_from(tab.arm_rows[ai]) if (!guarded || guards_complete(types, pat, guard_masks))
    }
    var inscope path, found : array<string>
    let shown_limit = 3
    missing_values(tab, prev, 0, shown_limit, path, found)
    let shown <- [for (f in found); "`{f}`"; where f != MATCH_ANY_OTHER]
    res.missing = (empty(found) ? none(type<string>) : some(found[0] == MATCH_ANY_OTHER ? MATCH_ANY_OTHER
        : join([for (f, i in shown, count()); f; where i < shown_limit], ", ") + (length(found) > shown_limit ? " and more" : "")))
    return <- res
}

[macro_function]
def private drop_last_condition(var chain : Expression?) : Expression? {
    //! An exhaustive chain's last test is redundant: the last arm becomes the final ``else``, so every path returns. Coverage guarantees that arm is unguarded - a guarded arm after full coverage is unreachable.
    var node = chain as ExprIfThenElse
    return node.if_true if (node.if_false == null)
    while ((node.if_false as ExprIfThenElse).if_false != null) {
        node = node.if_false as ExprIfThenElse
    }
    node.if_false = (node.if_false as ExprIfThenElse).if_true
    return chain
}

[macro_function]
def private report_match_failure(prog : ProgramPtr; to : MatchTo; pattern : Expression?; typ : TypeDeclPtr) : void {
    for (e in to.errors) {
        macro_error(prog, *e.at, e.msg)
    }
    if (empty(to.errors)) {
        macro_error(prog, pattern.at, "pattern `{describe_pattern(pattern)}` does not match `{describe(typ)}`")
    }
}

[call_macro(name="match")]
class MatchMacro : AstCallMacro {
    //! Implements `match` macro.
    report_match_errors = true
    def override canVisitArgument(expr : ExprCallMacro?; argIndex : int) : bool =>
        !is_reporting_compilation_errors()
    def override canFoldReturnResult(expr : ExprCallMacro?) : bool => false
    def override preVisit(prog : ProgramPtr; mod : Module?; expr : ExprCallMacro?) : void {
        return if (length(expr.arguments) < 2)
        assume blk = expr.arguments[length(expr.arguments) - 1]
        return if (!(blk is ExprMakeBlock))
        var eblk = ((blk as ExprMakeBlock)._block as ExprBlock)
        for (ee in eblk.list) {
            if (ee is ExprIfThenElse) {
                (ee as ExprIfThenElse).if_flags.doNotFold = true
                hide_bound_names(ee, (ee as ExprIfThenElse).cond, false)
            } elif (is_arrow_arm(ee)) {
                hide_bound_names(ee, (ee as ExprMakeTuple).values[0], true)
            }
        }
    }
    def add_arm_branch(var arm : Expression?; var to : MatchTo; as_value : bool; access : Expression?; var iff : Expression?&;
                       var catch_all : bool&; var prefix : array<Expression?>) : void {
        //! Adds one matched arm (or one alternative of a split arm) to the if-chain, innermost first.
        let binds <- [for (dv, dn in keys(to.declarations), values(to.declarations)); bound_let(dv, dn); where to.bind_names && !key_exists(to.by_ref, dv)]
        var nblk = arm_body(arm, as_value, binds)
        apply_template(nblk) $(rules) {
            for (dv, dn in keys(to.declarations), values(to.declarations)) {
                rules |> replaceVariable(dv) <| clone_expression(dn) if (!to.bind_names || key_exists(to.by_ref, dv))
            }
            rules |> replaceVarTag("_") <| clone_expression(access)
        }
        if (iff == null && empty(to.conditions)) {
            catch_all = true
            iff = nblk
        } else {
            var new_iff = new ExprIfThenElse(at = arm.at, cond = join_conditions(to.conditions, arm.at))
            new_iff.if_true = nblk
            new_iff.if_false = iff
            iff = new_iff
        }
        prefix |> push_from(to.prefix)
    }
    def report_coverage(prog : ProgramPtr; expr : ExprCallMacro?; eblk : ExprBlock?; var types : array<TypeDeclPtr>; var arm_patterns : array<Expression?>;
                        as_value, catch_all : bool) : bool {
        //! Reports unreachable arms and alternatives, and values an exhaustive match misses; false when it reported any.
        var cov <- match_coverage(types, arm_patterns)
        for (ai in cov.unreachable) {
            macro_error(prog, eblk.list[ai].at, "unreachable arm: the arms before it already match everything it matches")
        }
        for (da in cov.dead_alternatives) {
            macro_error(prog, eblk.list[da.arm].at, "unreachable alternative `{da.what}`: an earlier arm already matches it")
        }
        return false if (!empty(cov.unreachable) || !empty(cov.dead_alternatives))
        if (!catch_all && is_some(cov.missing) && (as_value || (report_match_errors && cov.all_finite))) {
            let guard_note = (cov.has_guards
                ? " (a guarded arm covers nothing, unless arms with the same pattern guard by comparing the same two int, enum, string or bool values and cover every outcome)"
                : "")
            macro_error(prog, expr.at, (cov.missing == MATCH_ANY_OTHER
                ? "match is not exhaustive: values no arm names are not matched; add a `_ => ...` arm{guard_note}"
                : "match is not exhaustive: no arm matches {unwrap(cov.missing)}; add arms for them, or `_ => ...`{guard_note}"))
            return false
        }
        return true
    }
    def assemble(var prefix : array<Expression?>; var iff : Expression?; as_value : bool; var parent : MatchParent; at : LineInfo) : Expression? {
        //! The match's code: the prefix, then the if-chain of the arms - inside an invoke when the match is a value.
        prefix |> emplace(iff)
        return qmacro($b(prefix)) if (!as_value)
        var res = qmacro(invoke($() : $t(parent.value_type) {
            $b(prefix)
        }))
        res |> force_at(at)
        return res
    }
    def override visit(prog : ProgramPtr; mod : Module?; var expr : ExprCallMacro?) : Expression? {   // nolint:STYLE037 - exact-type `is` ladder over pattern node classes
        let nargs = length(expr.arguments)
        macro_verify(nargs >= 2, prog, expr.at, "expecting match (what) <| block")
        for (i in range(nargs - 1)) {
            assume what = expr.arguments[i]
            macro_verify(what._type != null && !what._type.isAutoOrAlias, prog, what.at, "match argument {what.describe()} failed to compile")
        }
        assume what = expr.arguments[0]
        assume blk = expr.arguments[nargs - 1]
        macro_verify(blk._type.isGoodBlockType, prog, expr.at, "match `block` argument did not resolve")
        macro_verify(blk is ExprMakeBlock, prog, expr.at, "match `block` argument must be immediate block declaration")
        var eblk = ((blk as ExprMakeBlock)._block as ExprBlock)
        macro_verify(empty(eblk.finalList), prog, expr.at, "match `block` argument can't have finally section")
        var arrows = 0
        for (ee in eblk.list) {
            if (ee is ExprIfThenElse) {
                let eite = ee as ExprIfThenElse
                macro_verify(eite.if_false == null, prog, expr.at, "match to `if` can't have `else` section")
            } elif (is_arrow_arm(ee)) {
                arrows ++
            } else {
                macro_error(prog, ee.at, "match `block` argument can only contain `pattern => body` arms, not {ee.__rtti}; an assignment body binds looser than `=>`, write `pattern => $ \{ a = b \}`")
                return null
            }
        }
        macro_verify(arrows == 0 || arrows == length(eblk.list), prog, expr.at, "match can't mix `if (pattern)` and `pattern => body` arms")
        macro_verify(nargs == 2 || arrows > 0, prog, expr.at, "match on several values needs `pattern => body` arms")
        var parent <- match_parent(expr)
        let as_value = arrows > 0 && !parent.is_statement
        if (as_value) {
            for (ee in eblk.list) {
                if ((ee as ExprMakeTuple).values[1] is ExprMakeBlock) {
                    macro_error(prog, ee.at, "match used as a value takes `pattern => expression` arms, not `$`-block bodies")
                    return null
                }
            }
        }
        var catch_all = false
        var iff : Expression?
        let eli = length(eblk.list)
        var any_errors = false
        var prefix : array<Expression?>
        var types <- [for (i in range(nargs - 1)); expr.arguments[i]._type]
        var accesses <- [for (i in range(nargs - 1)); bind_subject(expr.arguments[i], i, prefix)]
        let access = accesses[0]
        var arm_patterns : array<Expression?>
        arm_patterns |> resize(eli)
        for (ei in range(eli)) {
            {
                let arm_index = eli - ei - 1
                var arm = eblk.list[arm_index]
                var pattern = arrows > 0 ? (arm as ExprMakeTuple).values[0] : (arm as ExprIfThenElse).cond
                var to : MatchTo
                to.bind_names = arrows > 0
                let matched = match_multi(types, accesses, pattern, to)
                if (matched) {
                    arm_patterns[arm_index] = pattern
                    add_arm_branch(arm, to, as_value, access, iff, catch_all, prefix)
                } elif (report_match_errors || to.failed) {
                    any_errors = true
                    report_match_failure(prog, to, pattern, what._type)
                    break
                }
            }
        }
        return null if (any_errors || (arrows > 0 && !report_coverage(prog, expr, eblk, types, arm_patterns, as_value, catch_all)))
        iff = as_value && !catch_all ? drop_last_condition(iff) : iff
        return assemble(prefix, iff, as_value, parent, expr.at)
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
    //! a condition ``value == expr``. Names bound earlier in the pattern are substituted before comparison.
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
