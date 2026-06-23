options gen2
options indenting = 4
options strict_smart_pointers = true
options _comment_hygiene = true

module flatten_opt_fuse shared private

//! PHASE_FUSED finishing pass: re-pack a*b +/- c into mad, the expanded lerp shape into lerp, lane sums into dot, and ctor lane patterns.

require daslib/ast
require daslib/ast_boost
require daslib/ast_match
require daslib/templates_boost
require daslib/macro_boost
require daslib/match
require daslib/rtti
require strings
require daslib/flatten_opt_common


// A negated copy of a scalar/vector float/int const literal; null for any other node
// (uint excluded — modular negation is value-correct but backend-hostile).
def private negate_const(e : ExpressionPtr) : ExpressionPtr {
    if (e is ExprConstDouble) return new ExprConstDouble(at = e.at, value = -(e as ExprConstDouble).value)   // double: outside the 32-bit grid
    var L = const_lanes(e)
    if (L.kind == CK_FLOAT) { L.f = -L.f; return make_const_lanes(L, e.at) }
    if (L.kind == CK_INT) { L.i = -L.i; return make_const_lanes(L, e.at) }
    return null   // uint excluded — modular negation is value-correct but backend-hostile
}

def private has_arity_named(mod : Module?; fname : string; argc : int) : bool {
    var found = false
    for_each_function(mod, fname) $(fn) {
        if (length(fn.arguments) == argc) {
            found = true
        }
    }
    for_each_generic(mod, fname) $(fn) {
        if (length(fn.arguments) == argc) {
            found = true
        }
    }
    return found
}

def public fuse_callable(mod : Module?; fname : string; argc : int = 3) : bool {
    if (mod == null) return false
    if (has_arity_named(mod, fname, argc)) return true
    var found = false
    module_for_each_dependency(mod) $(dep, _isPub) {
        if (!found && has_arity_named(dep, fname, argc)) {
            found = true
        }
    }
    return found
}

// Match `m ± c` / `c ± m` (m = a*b; `-` needs a const-negatable side) against the das mad shapes;
// out-params carry (a, b, c) with the vec-scalar swap + negation applied. The ONE gate for fuser AND
// residual; no purity gate — flat-body calls are pure by contract (and has_sideeffects is timing-dependent).
def private match_mad(var that : ExprOp2?; var ma, mb, mc : ExpressionPtr&) : bool {
    if (that == null) return false
    var fa, fb, other : ExpressionPtr
    var negOther = false        // `m - C`  → c = -C
    var negFactor = false       // `C - m`  → negate a const factor of m
    if (qmatch(that, $e(fa) * $e(fb) + $e(other)).matched) {
        pass
    } elif (qmatch(that, $e(other) + $e(fa) * $e(fb)).matched) {
        pass
    } elif (qmatch(that, $e(fa) * $e(fb) - $e(other)).matched) {
        negOther = true
    } elif (qmatch(that, $e(other) - $e(fa) * $e(fb)).matched) {
        negFactor = true
    } else {
        return false
    }
    // types captured BEFORE negation — a fresh negated literal is untyped until re-infer
    var bta = expr_bt(fa)
    var btb = expr_bt(fb)
    let btc = expr_bt(other)
    if (negFactor) {
        var nf = negate_const(fa)
        if (nf != null) {
            fa = nf
        } else {
            nf = negate_const(fb)
            if (nf == null) return false
            fb = nf
        }
    }
    if (negOther) {
        other = negate_const(other)
        if (other == null) return false
    }
    // a scalar-left mul swaps to the MadS (vec, scalar, vec) form
    if (vec_elem_bt(btb) != Type.none && bta == vec_elem_bt(btb)) {
        var tmp = fa
        fa = fb
        fb = tmp
        let tmpbt = bta
        bta = btb
        btb = tmpbt
    }
    if (!mad_shape(bta, btb, btc)) return false
    ma = fa
    mb = fb
    mc = other
    return true
}

// `(-a) + b` / `b + (-a)` → `b - a`: one sub node instead of neg + add. Reassoc no longer emits
// the form (positives-first canonical order), so this catches the other producers — the `0 - x`
// and `*-1` fold arms. Shared by fuser AND residual.
def private match_neg_add(var that : ExprOp2?; var pos, neg : ExpressionPtr&) : bool {
    if (that == null || !is_mad_elem(expr_bt(that))) return false
    return (qmatch(that, -$e(neg) + $e(pos)).matched ||    // nolint:STYLE016 (qmatch macro expansion)
        qmatch(that, $e(pos) + -$e(neg)).matched)           // nolint:STYLE016 (qmatch macro expansion)
}

// `mad(b - a, t, a)` → `lerp(a, b, t)`: the re-packed expansion shape, `a` matched by
// describe-equality. The neg-add form is matched too (defense — reassoc no longer emits it).
// Deliberately misses a CSE-shared `b - a` (behind a `_cse_` var — keeping the share IS the win).
def private match_lerp(var cll : ExprCall?; var la, lb, lt : ExpressionPtr&) : bool {
    if (cll == null) return false
    var da, db, aa, tt : ExpressionPtr
    if (!qmatch(cll, mad($e(db) - $e(da), $e(tt), $e(aa))).matched &&     // nolint:STYLE016 (qmatch macro expansion)
        !qmatch(cll, mad(-$e(da) + $e(db), $e(tt), $e(aa))).matched &&    // nolint:STYLE016 (qmatch macro expansion)
        !qmatch(cll, mad($e(db) + -$e(da), $e(tt), $e(aa))).matched) return false  // nolint:STYLE016 (qmatch macro expansion)
    if (describe(da) != describe(aa) ||
        !lerp_shape(expr_bt(da), expr_bt(db), expr_bt(tt))) return false
    la = aa
    lb = db
    lt = tt
    return true
}

let private SIDE_NONE = 0
let private SIDE_EQUAL = 1       // every lane carries the SAME scalar (describe-equal)
let private SIDE_COMPONENT = 2   // lane i reads component_i of one shared vector base
let private SIDE_GATHER = 3      // arbitrary scalars — packed into a fresh ctor (or const literal)

// Vectorize one side (lefts or rights) of a ctor whose lanes are same-op ExprOp2: classify the
// d side-exprs and build the vector replacement from CLONES. SIDE_COMPONENT with the natural
// full mask collapses to the base itself (no swizzle node).
def private vectorize_side(var that : ExprCall?; d : int; takeLeft : bool; var node : ExpressionPtr&) : int {
    var sides : array<ExpressionPtr>
    sides |> reserve(d)
    for (i in range(d)) {
        var li = that.arguments[i] as ExprOp2
        sides |> push(takeLeft ? li.left : li.right)
    }
    // EQUAL — all lanes the same pure scalar
    if (expr_bt(sides[0]) == Type.tFloat) {
        let key0 = describe(sides[0])
        var eq = true
        for (i in range(1, d)) {
            if (describe(sides[i]) != key0) {
                eq = false
                break
            }
        }
        if (eq) {
            node = clone_expression(sides[0])
            unsafe {
                delete sides
            }
            return SIDE_EQUAL
        }
    }
    // COMPONENT — lane i reads a component of one shared base; the masks concatenate to a swizzle
    var baseKey = ""
    var base : ExpressionPtr
    var mask = ""
    var isComp = true
    for (i in range(d)) {
        var bk = ""
        var b : ExpressionPtr
        var c = ""
        if (!component_read_of(sides[i], bk, b, c) || (i > 0 && bk != baseKey)) {
            isComp = false
            break
        }
        baseKey = bk
        base = b
        mask += c // nolint:PERF001 — at most 4 single-char appends
    }
    if (isComp) {
        let dim = expr_bt(base) == Type.tFloat2 ? 2 : (expr_bt(base) == Type.tFloat3 ? 3 : 4)
        if (dim == d && mask == slice("xyzw", 0, d)) {
            node = clone_expression(base)
        } else {
            node = new ExprField(at = that.at, value = clone_expression(base), name := mask)
        }
        unsafe {
            delete sides
        }
        return SIDE_COMPONENT
    }
    // GATHER — pack the scalars into a fresh ctor (a const literal when every lane is const)
    var lanes <- [for (sd in sides); clone_expression(sd)]
    node = make_float_ctor(d, that.at, lanes)
    unsafe {
        delete lanes
        delete sides
    }
    return SIDE_GATHER
}

def private ctor_fuse_rewrite(var that : ExprCall?; no_fast_math : bool) : ExpressionPtr {
    let d = ctor_dim_by_name(that)
    if (d < 2 || !is_float_vec(expr_bt(that))) return null
    // re-vectorize same-op lanes: `floatN(v.x + c1, v.y + c2, …) → v + floatN(c1,c2,…)` (exact)
    var sameOp = ""
    if (that.arguments[0] is ExprOp2) {
        let op0 = "{(that.arguments[0] as ExprOp2).op}"
        if (op0 == "+" || op0 == "-" || op0 == "*" || op0 == "/") {
            sameOp = op0
            for (i in range(1, d)) {
                if (!(that.arguments[i] is ExprOp2) || "{(that.arguments[i] as ExprOp2).op}" != op0) {
                    sameOp = ""
                    break
                }
            }
        }
    }
    if (sameOp != "") {
        var aN : ExpressionPtr
        var bN : ExpressionPtr
        let ka = vectorize_side(that, d, true, aN)
        let kb = vectorize_side(that, d, false, bN)
        // profitability bar: a component pattern on a side (EQUAL/GATHER pairs are node-neutral)
        if (ka == SIDE_COMPONENT || kb == SIDE_COMPONENT) {
            // a lone EQUAL scalar broadcasts natively under * and /; + and - need a splat ctor
            if (ka == SIDE_EQUAL && (sameOp == "+" || sameOp == "-")) {
                aN = make_float_splat(d, that.at, aN)
            }
            if (kb == SIDE_EQUAL && (sameOp == "+" || sameOp == "-")) {
                bN = make_float_splat(d, that.at, bN)
            }
            return new ExprOp2(at = that.at, op := sameOp, left = aN, right = bN)
        }
    }
    // shared factor / divisor, keyed on lane 0's candidates (describe equality across lanes)
    if (that.arguments[0] is ExprOp2) {
        var l0 = that.arguments[0] as ExprOp2
        if (l0.op == "*" || l0.op == "/") {
            for (ci in range(l0.op == "*" ? 2 : 1)) {
                var cand = (l0.op == "/" || ci == 1) ? l0.right : l0.left
                if (expr_bt(cand) != Type.tFloat) {
                    continue
                }
                let key = describe(cand)
                var rests : array<ExpressionPtr>
                var allMatch = true
                for (i in range(d)) {
                    var ok = false
                    if (that.arguments[i] is ExprOp2) {
                        var li = that.arguments[i] as ExprOp2
                        if (li.op == l0.op) {
                            if (li.op == "*" && describe(li.left) == key && expr_bt(li.left) == Type.tFloat) {
                                rests |> push(clone_expression(li.right))
                                ok = true
                            } elif (describe(li.right) == key && expr_bt(li.right) == Type.tFloat) {
                                rests |> push(clone_expression(li.left))
                                ok = true
                            }
                        }
                    }
                    if (!ok) {
                        allMatch = false
                        break
                    }
                }
                if (allMatch) {
                    var nc = make_float_ctor(d, that.at, rests)
                    var sc = clone_expression(cand)
                    unsafe {
                        delete rests
                    }
                    return new ExprOp2(at = that.at, op := l0.op, left = nc, right = sc)
                }
                unsafe {
                    delete rests
                }
            }
        }
    }
    // majority const factor with compensation (fast-math): most frequent lane factor C ≠ 0,1 wins
    if (no_fast_math) return null
    var laneC : array<float>
    var laneV : array<float>
    var laneE : array<ExpressionPtr>
    for (i in range(d)) {
        var cv = 0.0
        if (scalar_float_const(that.arguments[i], cv)) {
            laneC |> push(0.0)
            laneV |> push(cv)
            laneE |> push(null)
            continue
        }
        var ci = 1.0
        var ei = that.arguments[i]
        if (that.arguments[i] is ExprOp2) {
            var li = that.arguments[i] as ExprOp2
            if (li.op == "*") {
                if (scalar_float_const(li.right, cv) && !(li.left |> is_expr_const())) {
                    ci = cv
                    ei = li.left
                } elif (scalar_float_const(li.left, cv) && !(li.right |> is_expr_const())) {
                    ci = cv
                    ei = li.right
                }
            }
        }
        laneC |> push(ci)
        laneV |> push(0.0)
        laneE |> push(ei)
    }
    var bestC = 0.0
    var bestN = 0
    for (i in range(d)) {
        if (laneE[i] == null || laneC[i] == 1.0 || laneC[i] == 0.0) {
            continue
        }
        var n = 0
        for (j in range(d)) {
            if (laneE[j] != null && laneC[j] == laneC[i]) {
                n ++
            }
        }
        if (n > bestN) {
            bestN = n
            bestC = laneC[i]
        }
    }
    if (bestN < 2) {
        delete laneC
        delete laneV
        unsafe {
            delete laneE
        }
        return null
    }
    var before = 0
    var after = 1
    for (i in range(d)) {
        if (laneE[i] != null && laneC[i] != 1.0) {
            before ++
        }
        if (laneE[i] != null && laneC[i] != bestC) {
            after ++
        }
    }
    if (after >= before) {
        delete laneC
        delete laneV
        unsafe {
            delete laneE
        }
        return null
    }
    var lanes : array<ExpressionPtr>
    for (i in range(d)) {
        if (laneE[i] == null) {
            lanes |> push(new ExprConstFloat(at = that.at, value = laneV[i] / bestC))
        } elif (laneC[i] == bestC) {
            lanes |> push(clone_expression(laneE[i]))
        } else {
            var comp : ExpressionPtr = new ExprConstFloat(at = that.at, value = laneC[i] / bestC)
            lanes |> push(new ExprOp2(at = that.at, op := "*", left = clone_expression(laneE[i]), right = comp))
        }
    }
    var nc = make_float_ctor(d, that.at, lanes)
    delete laneC
    delete laneV
    unsafe {
        delete lanes
        delete laneE
    }
    return new ExprOp2(at = that.at, op := "*", left = nc, right = new ExprConstFloat(at = that.at, value = bestC))
}

struct private DotGroup {
    base : ExpressionPtr        // first lane's base node, reused as the dot's vector arg
    dim : int
    weights : float4
    nterms : int
}

// One additive term as a (possibly const-weighted) lane read: `v.x`, `v.x * c`, `c * v.x`.
def private dot_term_lane(var e : ExpressionPtr; var baseKey : string&; var base : ExpressionPtr&;
                          var lane : int&; var w : float&) : bool {
    var comp = ""
    if (component_read_of(e, baseKey, base, comp)) {
        lane = lane_index(comp)
        w = 1.0
        return true
    }
    var l, r : ExpressionPtr
    if (!qmatch(e, $e(l) * $e(r)).matched) return false    // nolint:STYLE016 (qmatch macro expansion)
    var c = 0.0
    if (scalar_float_const(r, c) && component_read_of(l, baseKey, base, comp)) {
        lane = lane_index(comp)
        w = c
        return true
    }
    if (scalar_float_const(l, c) && component_read_of(r, baseKey, base, comp)) {
        lane = lane_index(comp)
        w = c
        return true
    }
    return false
}

// Bucket a chain's terms into per-base lane groups; `tKey[i]` carries term i's group key ("" for
// a non-lane term). Shared by the fuser and the residual oracle — ONE gate, so a survivor is
// always a fuser miss.
def private dot_chain_groups(var terms : array<ReTerm>; var groups : table<string; DotGroup>;
                             var tKey : array<string>) : bool {
    tKey |> resize(length(terms))
    for (i in range(length(terms))) {
        var baseKey = ""
        var base : ExpressionPtr
        var lane = 0
        var w = 0.0
        if (!dot_term_lane(terms[i].e, baseKey, base, lane, w)) {
            continue
        }
        tKey[i] = baseKey
        if (terms[i].neg) {
            w = -w
        }
        if (!key_exists(groups, baseKey)) {
            groups[baseKey] = DotGroup(base = base, dim = float_vec_dim(expr_bt(base)))
        }
        groups[baseKey].weights[lane] += w
        groups[baseKey].nterms ++
    }
    var any = false
    for (g in values(groups)) {
        if (g.nterms >= 2) {
            any = true
        }
    }
    return any
}

def private has_unfused_lane_dot(var that : ExprOp2?) : bool {
    if (that == null || !(that.op == "+" || that.op == "-") || expr_bt(that) != Type.tFloat) return false
    var terms : array<ReTerm>
    collect_add(that, false, terms)
    var groups : table<string; DotGroup>
    var tKey : array<string>
    let any = dot_chain_groups(terms, groups, tKey)
    unsafe {
        delete terms
        delete groups
    }
    delete tKey
    return any
}

// Rebuild the chain with every ≥2-lane group collapsed to `dot(base, mask)` — emitted at the
// group's first term, term order otherwise preserved; null when no group qualifies. Operand
// nodes are reused (each lands in one slot); the fused lane reads just drop.
def private dot_fold_chain(var that : ExpressionPtr; var terms : array<ReTerm>) : ExpressionPtr {
    var groups : table<string; DotGroup>
    var tKey : array<string>
    if (!dot_chain_groups(terms, groups, tKey)) {
        unsafe {
            delete groups
        }
        delete tKey
        return null
    }
    var emitted : table<string>
    var newTerms : array<ReTerm>
    for (i in range(length(terms))) {
        let k = tKey[i]
        if (k == "") {
            newTerms |> push(terms[i])
            continue
        }
        var g = groups?[k] ?? default<DotGroup>
        if (g.nterms < 2) {
            newTerms |> push(terms[i])
            continue
        }
        if (key_exists(emitted, k)) {
            continue
        }
        emitted |> insert(k)
        var lanes : array<ExpressionPtr>
        lanes |> reserve(g.dim)
        for (li in range(g.dim)) {
            lanes |> push(new ExprConstFloat(at = that.at, value = g.weights[li]))
        }
        var cll = new ExprCall(at = that.at, name := "dot")
        emplace(cll.arguments, g.base)
        var mask = make_float_ctor(g.dim, that.at, lanes)
        emplace(cll.arguments, mask)
        newTerms |> push(ReTerm(neg = false, e = cll))
        unsafe {
            delete lanes
        }
    }
    var r = build_chain(newTerms, that.at, "+", "-")
    unsafe {
        delete groups
        delete newTerms
    }
    delete tKey
    delete emitted
    return r
}

// Top-down rewrite over the closed post-flatten node set: fold each maximal `+`/`-` chain once
// (a bottom-up visitor would fold a sub-chain first and strand the outer lanes), recursing into
// chain leaves and every other child slot.
def private dot_fold_expr(var e : ExpressionPtr; var changed : bool&) : ExpressionPtr {
    if (e == null) return null
    if (e is ExprOp2) {
        var o = e as ExprOp2
        if ((o.op == "+" || o.op == "-") && expr_bt(o) == Type.tFloat) {
            var terms : array<ReTerm>
            collect_add(e, false, terms)
            for (t in terms) {
                // a chain leaf is never itself a chain, so the result is always `t.e` back
                dot_fold_expr(t.e, changed)
            }
            var r = dot_fold_chain(e, terms)
            unsafe {
                delete terms
            }
            if (r != null) {
                changed = true
                return r
            }
            return e
        }
    }
    rewrite_children(e) $(var c : ExpressionPtr) { return dot_fold_expr(c, changed) }
    return e
}

def private dot_fold_stmt(var s : ExpressionPtr; var changed : bool&) {
    if (s is ExprLet) {
        var lc = s as ExprLet
        for (lv in lc.variables) {
            if (lv.init != null) {
                lv.init = dot_fold_expr(lv.init, changed)
            }
        }
    } elif (s is ExprCopy) {
        var cp = s as ExprCopy
        cp.right = dot_fold_expr(cp.right, changed)
    } elif (s is ExprReturn) {
        var rt = s as ExprReturn
        if (rt.subexpr != null) {
            rt.subexpr = dot_fold_expr(rt.subexpr, changed)
        }
    }
}

class private CtorFuseVisitor : AstVisitor {
    changed : bool
    noFastMath : bool
    def override visitExprCall(var that : ExprCall?) : ExpressionPtr {
        var r = ctor_fuse_rewrite(that, noFastMath)
        if (r != null) {
            changed = true
            return r
        }
        return that
    }
}

class private MadFuseVisitor : AstVisitor {
    changed : bool
    def override visitExprOp2(var that : ExprOp2?) : ExpressionPtr {
        var ma : ExpressionPtr
        var mb : ExpressionPtr
        var mc : ExpressionPtr
        if (match_mad(that, ma, mb, mc)) {
            changed = true
            var cll = new ExprCall(at = that.at, name := "mad")
            emplace(cll.arguments, ma)
            emplace(cll.arguments, mb)
            emplace(cll.arguments, mc)
            return cll
        }
        var pos : ExpressionPtr
        var neg : ExpressionPtr
        if (match_neg_add(that, pos, neg)) {
            changed = true
            return new ExprOp2(at = that.at, op := "-", left = pos, right = neg)
        }
        return that
    }
}

class private LerpFuseVisitor : AstVisitor {
    changed : bool
    def override visitExprCall(var that : ExprCall?) : ExpressionPtr {
        var la : ExpressionPtr
        var lb : ExpressionPtr
        var lt : ExpressionPtr
        if (match_lerp(that, la, lb, lt)) {
            changed = true
            var cll = new ExprCall(at = that.at, name := "lerp")
            emplace(cll.arguments, la)
            emplace(cll.arguments, lb)
            emplace(cll.arguments, lt)
            return cll
        }
        return that
    }
}

// Fusion residuals — mirrors the fuser's matchers EXACTLY (same gates, same shapes), so a
// survivor means the PHASE_FUSED pass missed, never that the gate was narrower here.
class public FuseResidualVisitor : AstVisitor {
    checkMad : bool
    checkLerp : bool
    checkDot : bool
    noFastMath : bool
    foundMad : bool
    foundLerp : bool
    foundNegAdd : bool
    foundCtor : bool
    foundDot : bool
    def override preVisitExprOp2(var expr : ExprOp2?) : void {
        if (checkDot && has_unfused_lane_dot(expr)) {
            foundDot = true
        }
        if (!checkMad) {
            return
        }
        var ma : ExpressionPtr
        var mb : ExpressionPtr
        var mc : ExpressionPtr
        if (match_mad(expr, ma, mb, mc)) {
            foundMad = true
        }
        var pos : ExpressionPtr
        var neg : ExpressionPtr
        if (match_neg_add(expr, pos, neg)) {
            foundNegAdd = true
        }
    }
    def override preVisitExprCall(var expr : ExprCall?) : void {
        if (checkLerp) {
            var la : ExpressionPtr
            var lb : ExpressionPtr
            var lt : ExpressionPtr
            if (match_lerp(expr, la, lb, lt)) {
                foundLerp = true
            }
        }
        if (ctor_fuse_rewrite(expr, noFastMath) != null) {
            foundCtor = true
        }
    }
}

def public fuse_body(var func : FunctionPtr; no_fast_math : bool = false) : bool {
    //! PHASE_FUSED finishing pass: collapse same-vector lane sums into `dot(v, mask)`, re-pack
    //! ctor lane patterns (re-vectorize / shared factor), `a*b ± c` into `mad(a,b,c)`, and the
    //! expanded lerp shape back into `lerp(a, b, t)`. Caller re-infers after each round.
    if (!(func.body is ExprBlock)) return false
    var changed = false
    // hadd → dot first — the mad walk would otherwise steal a weighted lane (`v.x * w + rest`)
    if (!no_fast_math && fuse_callable(func._module, "dot", 2)) {
        var blk = func.body as ExprBlock
        var dchanged = false
        for (s in blk.list) {
            dot_fold_stmt(s, dchanged)
        }
        if (dchanged) {
            changed = true
        }
    }
    // ctor re-pack next — its `ctor * s` / vector-op output feeds the mad walk this same round
    var cv = new CtorFuseVisitor()
    cv.noFastMath = no_fast_math
    make_visitor(*cv) $(adapter) {
        visit(func.body, adapter)
    }
    if (cv.changed) {
        changed = true
    }
    unsafe {
        delete cv
    }
    if (!fuse_callable(func._module, "mad")) return changed
    var v = new MadFuseVisitor()
    make_visitor(*v) $(adapter) {
        visit(func.body, adapter)
    }
    if (v.changed) {
        changed = true
    }
    unsafe {
        delete v
    }
    // lerp re-fusion is a separate walk over the fresh mads — a visitor never descends into
    // the node it just returned, so the mad → lerp chain can't happen in one pass.
    if (fuse_callable(func._module, "lerp")) {
        var lv = new LerpFuseVisitor()
        make_visitor(*lv) $(adapter) {
            visit(func.body, adapter)
        }
        if (lv.changed) {
            changed = true
        }
        unsafe {
            delete lv
        }
    }
    return changed
}
