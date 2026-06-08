options gen2
options indenting = 4
options strict_smart_pointers = true

module flatten_opt shared private

//! Internal optimization passes for daslib/flatten: constant folding, constant
//! propagation, copy propagation, and dead-store elimination over a flattened
//! twin's straight-line body, plus the post-infer select-fold (`fold_body`).
//! Not a public API — `require`d by daslib/flatten; the lowering machinery stays
//! in flatten.das.

require daslib/ast
require daslib/ast_boost
require daslib/templates_boost
require daslib/macro_boost
require daslib/rtti
require strings

def private is_flat_temp(name : das_string) : bool {
    return name |> starts_with("__flat_")
}

// ===== Dead-store elimination =====
//
// The lowered body is straight-line (no branches), so liveness is a single
// backward scan. We only ever touch flatten-introduced `__flat_*` locals — never
// user locals or globals (a `g_out = ...` is observable output, never "dead").
// A `__flat_*` store is removed when its target is not live afterward AND its RHS
// is side-effect-free / non-faulting (no call, index, or division) — which is
// exactly the live-mask kills (`__flat_live = false`, per-frame mask updates).

class private FlatScan : AstVisitor {
    reads : table<uint64>
    risky : bool
    def override preVisitExprVar(expr : ExprVar?) : void {
        if (is_flat_temp(expr.name)) reads |> insert(hash(expr.name))
    }
    def override preVisitExprCall(expr : ExprCall?) : void {
        risky = true
    }
    def override preVisitExprAt(expr : ExprAt?) : void {
        risky = true
    }
    def override preVisitExprOp2(expr : ExprOp2?) : void {
        if (expr.op == "/" || expr.op == "%") {
            risky = true
        }
    }
}

// Scan a subtree for the `__flat_*` names it reads and whether it carries a
// fault-capable / side-effecting node (call, index, division).
def private scan_flat(var e : ExpressionPtr; var reads : table<uint64>; var risky : bool&) {
    if (e == null) return
    var v = new FlatScan()
    make_visitor(*v) $(adapter) {
        visit(e, adapter)
    }
    for (nm in keys(v.reads)) {
        reads |> insert(nm)
    }
    if (v.risky) {
        risky = true
    }
    unsafe {
        delete v
    }
}

// ===== Constant propagation of always-true live masks =====
//
// When a function has no early returns, `__flat_live` is never reassigned, so it
// is constant `true` — and so is every per-frame `__flat_live_N` derived from it.
// Propagating that and folding the selects (`true ? a : b → a`, `true && x → x`)
// collapses the whole mask scaffolding, leaving clean dataflow. We only ever
// substitute `__flat_*` bool temps proven constant — never user/global vars.

def private const_bool(e : ExpressionPtr; var val : bool&) : bool {
    if (e == null) return false
    if (e is ExprConstBool) {
        val = (e as ExprConstBool).value
        return true
    }
    return false
}

def private fold_const(e : ExpressionPtr; constmap : table<uint64; bool>) : ExpressionPtr {
    if (e == null) return null
    if (e is ExprVar) {
        let h = hash((e as ExprVar).name)
        if (key_exists(constmap, h)) return new ExprConstBool(at = e.at, value = constmap?[h] ?? false)
        return clone_expression(e)
    }
    if (e is ExprOp1) {
        let o = e as ExprOp1
        var sub = fold_const(o.subexpr, constmap)
        var bv = false
        if (o.op == "!" && const_bool(sub, bv)) {
            return new ExprConstBool(at = e.at, value = !bv)
        }
        return new ExprOp1(at = e.at, atEnclosure = o.atEnclosure, genFlags = o.genFlags,
            op := o.op, subexpr = sub)
    }
    if (e is ExprOp2) {
        let o = e as ExprOp2
        var l = fold_const(o.left, constmap)
        var r = fold_const(o.right, constmap)
        var lb = false
        var rb = false
        let lc = const_bool(l, lb)
        let rc = const_bool(r, rb)
        if (o.op == "&&") {
            if (lc) {
                return r if (lb)
                return new ExprConstBool(at = e.at, value = false)
            }
            if (rc) {
                return l if (rb)
                return new ExprConstBool(at = e.at, value = false)
            }
        } elif (o.op == "||") {
            if (lc) {
                return new ExprConstBool(at = e.at, value = true) if (lb)
                return r
            }
            if (rc) {
                return new ExprConstBool(at = e.at, value = true) if (rb)
                return l
            }
        }
        return new ExprOp2(at = e.at, atEnclosure = o.atEnclosure, genFlags = o.genFlags,
            op := o.op, left = l, right = r)
    }
    if (e is ExprOp3) {
        let o = e as ExprOp3
        var c = fold_const(o.subexpr, constmap)
        var cb = false
        if (const_bool(c, cb)) {
            return cb ? fold_const(o.left, constmap) : fold_const(o.right, constmap)
        }
        return new ExprOp3(at = e.at, atEnclosure = o.atEnclosure, genFlags = o.genFlags,
            op := o.op, subexpr = c,
            left = fold_const(o.left, constmap),
            right = fold_const(o.right, constmap))
    }
    if (e is ExprCall) {
        let o = e as ExprCall
        var n = new ExprCall(at = e.at, atEnclosure = o.atEnclosure, genFlags = o.genFlags, name := o.name)
        for (ai in range(length(o.arguments))) {
            emplace_new(n.arguments) <| fold_const(o.arguments[ai], constmap)
        }
        return n
    }
    return clone_expression(e)
}

def public const_prop_pass(var out : array<ExpressionPtr>) {
    // A `__flat_*` bool temp is constant when it is declared with a const-foldable
    // init and never reassigned. Reassigned temps (early-return live masks) stay.
    var reassigned : table<uint64>
    for (s in out) {
        if (s is ExprCopy && (s as ExprCopy).left is ExprVar) {
            let lv = (s as ExprCopy).left as ExprVar
            if (is_flat_temp(lv.name)) reassigned |> insert(hash(lv.name))
        }
    }
    var constmap : table<uint64; bool>
    var changed = true
    while (changed) {
        changed = false
        for (s in out) {
            if (!(s is ExprLet)) continue
            let lc = s as ExprLet
            if (length(lc.variables) != 1) continue
            let v = lc.variables[0]
            let h = hash(v.name)
            if (!is_flat_temp(v.name) || key_exists(reassigned, h) || key_exists(constmap, h) || v.init == null) {
                continue
            }
            var bv = false
            var probe = fold_const(v.init, constmap)
            let isConst = const_bool(probe, bv)
            if (isConst) {
                constmap[h] = bv
                changed = true
            }
        }
    }
    delete reassigned
    if (empty(constmap)) {
        delete constmap
        return
    }
    var i = 0
    while (i < length(out)) {
        var s = out[i]
        if (s is ExprCopy) {
            var cp = s as ExprCopy
            cp.right = fold_const(cp.right, constmap)
        } elif (s is ExprLet) {
            var lc = s as ExprLet
            for (v in lc.variables) {
                if (v.init != null) {
                    v.init = fold_const(v.init, constmap)
                }
            }
        } elif (s is ExprReturn) {
            var r = s as ExprReturn
            if (r.subexpr != null) {
                r.subexpr = fold_const(r.subexpr, constmap)
            }
        } else {
            out[i] = fold_const(s, constmap)
        }
        i ++
    }
    delete constmap
}

// ===== Copy propagation of single-assignment flatten temps =====
//
// A `__flat_*` temp with exactly one definition (a decl-init, or a no-init decl
// plus a single store) and whose source reads no later-reassigned variable is a
// pure forward copy: inline its source at every use and drop the temp. This folds
// the output scaffold away — `__flat_ret = c; return __flat_ret` → `return c`, and
// `var __flat_arg = x` → `x` — so a branchless shader flattens byte-identical to
// its source (the parity goal). Substitution is delegated to `apply_template`
// (peel-aware, clones the source per hit). A risky source (call / index / divide)
// is inlined only at a single use, never duplicated.

class private FlatVarRef : AstVisitor {
    name  : string
    count : int
    def override preVisitExprVar(expr : ExprVar?) : void {
        // das_string compares with string directly — never build a temp string here:
        // count_refs walks the whole tree per candidate, so a per-ExprVar allocation
        // is O(n^2) persistent strings on large bodies (the heap-overflow amplifier).
        if (expr.name == name) count ++
    }
}

def private count_refs(var e : ExpressionPtr; name : string) : int {
    if (e == null) return 0
    var v = new FlatVarRef(name = name, count = 0)
    make_visitor(*v) $(adapter) {
        visit(e, adapter)
    }
    let c = v.count
    unsafe {
        delete v
    }
    return c
}

class private FlatScanAll : AstVisitor {
    reads : table<uint64>
    risky : bool
    def override preVisitExprVar(expr : ExprVar?) : void {
        reads |> insert(hash(expr.name))
    }
    def override preVisitExprCall(expr : ExprCall?) : void {
        risky = true
    }
    def override preVisitExprAt(expr : ExprAt?) : void {
        risky = true
    }
    def override preVisitExprOp2(expr : ExprOp2?) : void {
        if (expr.op == "/" || expr.op == "%") {
            risky = true
        }
    }
}

// Collect every variable name read by `e` (not just flatten temps) and whether it
// carries a fault-capable / re-evaluation-sensitive node.
def private scan_src(var e : ExpressionPtr; var reads : table<uint64>; var risky : bool&) {
    if (e == null) return
    var v = new FlatScanAll()
    make_visitor(*v) $(adapter) {
        visit(e, adapter)
    }
    for (nm in keys(v.reads)) {
        reads |> insert(nm)
    }
    if (v.risky) {
        risky = true
    }
    unsafe {
        delete v
    }
}

// Per-pass reference index: name-hash -> the statement indices that reference it
// (deduped per statement) plus the total occurrence count. Built once per copy_prop
// pass in a single walk, replacing the old per-candidate count_refs sweep that made
// the pass O(n^3) on large bodies (one full-body walk per candidate per fold-restart).
class private FlatUsesBuilder : AstVisitor {
    nameUses : table<uint64; array<int>>
    nameCount : table<uint64; int>
    seen : table<uint64>
    j : int
    def override preVisitExprVar(expr : ExprVar?) : void {
        let h = hash(expr.name)
        nameCount[h] ++
        if (!key_exists(seen, h)) {
            seen |> insert(h)
            nameUses[h] |> push(j)
        }
    }
}

def public copy_prop_pass(var out : array<ExpressionPtr>) {
    var changed = true
    while (changed) {
        changed = false
        let n = length(out)
        // Precompute once per pass (not per candidate): every store target's indices,
        // and a name -> (referencing statements, occurrence count) index. A source
        // reading var V is only unsafe if V is reassigned BETWEEN this temp's def and a
        // use — a reassignment fully before the def leaves the captured value intact.
        var storeIdx : table<uint64; array<int>>
        for (j in range(n)) {
            if (out[j] is ExprCopy && (out[j] as ExprCopy).left is ExprVar) {
                storeIdx[hash(((out[j] as ExprCopy).left as ExprVar).name)] |> push(j)
            }
        }
        var ub = new FlatUsesBuilder()
        make_visitor(*ub) $(adapter) {
            for (j in range(n)) {
                ub.j = j
                clear(ub.seen)
                visit(out[j], adapter)
            }
        }
        var srcReads : table<uint64>
        var useJs : array<int>   // hoisted: cleared per candidate, freed once per pass
        for (k in range(n)) {
            if (!(out[k] is ExprLet)) continue
            var lc = out[k] as ExprLet
            if (length(lc.variables) != 1 || !is_flat_temp(lc.variables[0].name)) continue
            let hk = hash(lc.variables[0].name)
            let nStores = key_exists(storeIdx, hk) ? length(storeIdx[hk]) : 0
            // Single definition: decl-init + no stores, OR no-init decl + one store. Use the
            // ORIGINAL source (no clone) — apply_template clones it per use, and the original
            // lives in out[k]/out[si], reclaimed when we drop them. Cloning here would orphan a
            // copy on every non-folding candidate (re-examined each while-restart → millions).
            var src : ExpressionPtr = null
            var si = -1
            if (lc.variables[0].init != null && nStores == 0) {
                src = lc.variables[0].init                    // decl-init is the single definition
            } elif (lc.variables[0].init == null && nStores == 1) {
                si = storeIdx[hk][0]
                var cp = out[si] as ExprCopy
                src = cp.right                                // no-init decl + single store
            } else {
                continue                                      // multi-def (e.g. early-return mask) — leave it
            }
            let defIdx = si >= 0 ? si : k
            clear(srcReads)
            var srcRisky = false
            scan_src(src, srcReads, srcRisky)
            if (key_exists(srcReads, hk)) continue            // self-referential source — never fold
            // Uses (excluding def k and store si) straight from the precomputed index.
            // nameCount includes the store's lvalue occurrence (1), which is not a use.
            let refCount = (key_exists(ub.nameCount, hk) ? ub.nameCount[hk] : 0) - (si >= 0 ? 1 : 0)
            var firstRef = n
            var maxUseIdx = -1
            clear(useJs)
            if (key_exists(ub.nameUses, hk)) {
                for (j in ub.nameUses[hk]) {
                    if (j == k || j == si) continue
                    useJs |> push(j)
                    if (j < firstRef) {
                        firstRef = j
                    }
                    if (j > maxUseIdx) {
                        maxUseIdx = j
                    }
                }
            }
            if ((refCount > 0 && firstRef < defIdx) || (srcRisky && refCount > 1)) continue
            // Position-aware guard: block only if a source var is reassigned strictly
            // between the def and the last use.
            var blocked = false
            for (rn in keys(srcReads)) {
                if (!key_exists(storeIdx, rn)) continue
                for (r in storeIdx[rn]) {
                    if (r > defIdx && r <= maxUseIdx) {
                        blocked = true
                        break
                    }
                }
                if (blocked) break
            }
            if (blocked) continue
            // Inline the source at every use, then drop the def (+ store).
            let nm = "{lc.variables[0].name}"
            var rules : Template
            rules |> replaceVariable(nm, src)   // store src directly; apply_template clones per use
            for (j in useJs) {
                out[j] = apply_template(rules, out[j].at, out[j], false)   // only statements that reference nm
            }
            unsafe { delete rules }
            out <- [for (j in range(n)); out[j]; where j != k && j != si]
            changed = true
            break
        }
        delete srcReads
        delete storeIdx
        delete useJs
        unsafe {
            delete ub
        }
    }
}

def public dse_pass(var out : array<ExpressionPtr>) {
    let n = length(out)
    // Names that are ever an assignment target: their decl must stay even if the
    // init value is dead (only fully-unreferenced decls are removable).
    var storeTargets : table<uint64>
    for (s in out) {
        if (s is ExprCopy && (s as ExprCopy).left is ExprVar) {
            let lv = (s as ExprCopy).left as ExprVar
            if (is_flat_temp(lv.name)) storeTargets |> insert(hash(lv.name))
        }
    }
    var live : table<uint64>
    var dead : table<int>
    var reads : table<uint64>   // hoisted: one scratch table, cleared per statement (was n allocations)
    var i = n - 1
    while (i >= 0) {
        var stmt = out[i]
        var handled = false
        clear(reads)
        if (stmt is ExprCopy) {
            var cp = stmt as ExprCopy
            if (cp.left is ExprVar && is_flat_temp((cp.left as ExprVar).name)) {
                handled = true
                let lh = hash((cp.left as ExprVar).name)
                var risky = false
                scan_flat(cp.right, reads, risky)
                if (!key_exists(live, lh) && !risky) {
                    dead |> insert(i)   // target not live afterward, RHS pure → dead
                } else {
                    live |> erase(lh)
                    for (nm in keys(reads)) {
                        live |> insert(nm)
                    }
                }
            }
        }
        if (!handled && stmt is ExprLet) {
            var lc = stmt as ExprLet
            if (length(lc.variables) == 1 && is_flat_temp(lc.variables[0].name)) {
                handled = true
                let dh = hash(lc.variables[0].name)
                var risky = false
                scan_flat(lc.variables[0].init, reads, risky)
                if (!key_exists(live, dh) && !key_exists(storeTargets, dh) && !risky) {
                    dead |> insert(i)   // declared temp fully unreferenced, init pure → dead
                } else {
                    live |> erase(dh)
                    for (nm in keys(reads)) {
                        live |> insert(nm)
                    }
                }
            }
        }
        if (!handled) {
            // Decl / return / global write / bare expr — over-approximate: every
            // flat var it mentions becomes live (never wrongly removes a store).
            var risky = false
            scan_flat(stmt, reads, risky)
            for (nm in keys(reads)) {
                live |> insert(nm)
            }
        }
        i --
    }
    delete storeTargets
    delete live
    delete reads
    if (empty(dead)) {
        delete dead
        return
    }
    out <- [for (j in range(n)); out[j]; where !key_exists(dead, j)]
    delete dead
}

// ===== SSA-rename of reassigned user locals =====
//
// flatten's body is straight-line (if → ?:, loops unrolled, no user calls), so a
// reassigned local has a single linear chain of definitions — no control-flow merges,
// hence no phi nodes. Renaming each write to a fresh version (`acc = acc + 3` →
// `var acc__s2 = acc__s1 + 3`) makes every local single-definition, which lets the
// post-infer const-prop (fold_body) fold the chain through the existing single-def
// path. We rename only user locals that are declared WITH an initializer (single-var)
// and reassigned at least once — never `__flat_*` masks (handled by const_prop_pass),
// globals (observable), or params (not declared here). A self-referential select
// (`acc = c ? e : acc`) resolves correctly: its read picks the PRIOR version.

def private rename_reads(var e : ExpressionPtr; current : table<string; string>) : ExpressionPtr {
    if (e == null) return null
    var tmpl : Template
    var any = false
    for (orig in keys(current)) {
        let vn = current?[orig] ?? ""
        tmpl |> replaceVariable(orig, qmacro($i(vn)))
        any = true
    }
    if (!any) return e
    var res = apply_template(tmpl, e.at, e, false)
    unsafe { delete tmpl }
    return res
}

def public ssa_rename_pass(var out : array<ExpressionPtr>) {
    // (1) user locals declared (single-var) with an initializer
    var declaredInit : table<uint64>
    for (s in out) {
        if (s is ExprLet && length((s as ExprLet).variables) == 1) {
            let v = (s as ExprLet).variables[0]
            if (!is_flat_temp(v.name) && v.init != null) declaredInit |> insert(hash(v.name))
        }
    }
    if (empty(declaredInit)) {
        delete declaredInit
        return
    }
    // (2) of those, the ones reassigned via a plain `var = ...` (multi-def)
    var ssaNames : table<uint64>
    for (s in out) {
        if (s is ExprCopy && (s as ExprCopy).left is ExprVar) {
            let h = hash(((s as ExprCopy).left as ExprVar).name)
            if (key_exists(declaredInit, h)) ssaNames |> insert(h)
        }
    }
    delete declaredInit
    if (empty(ssaNames)) {
        delete ssaNames
        return
    }
    // (3) forward rename: each read maps to the current version; each write mints a new
    // version (after resolving the RHS reads, so a self-ref reads the prior version).
    var current : table<string; string>
    var counter = 0
    var i = 0
    while (i < length(out)) {
        var s = out[i]
        if (s is ExprLet && length((s as ExprLet).variables) == 1 &&
                key_exists(ssaNames, hash((s as ExprLet).variables[0].name))) {
            let orig = "{(s as ExprLet).variables[0].name}"
            let newInit = rename_reads((s as ExprLet).variables[0].init, current)
            counter ++
            let vn = "{orig}__s{counter}"
            out[i] = qmacro_expr(${ var $i(vn) = $e(newInit); })
            current[orig] = vn
        } elif (s is ExprCopy && (s as ExprCopy).left is ExprVar &&
                key_exists(ssaNames, hash(((s as ExprCopy).left as ExprVar).name))) {
            let orig = "{((s as ExprCopy).left as ExprVar).name}"
            let newRhs = rename_reads((s as ExprCopy).right, current)
            counter ++
            let vn = "{orig}__s{counter}"
            out[i] = qmacro_expr(${ var $i(vn) = $e(newRhs); })
            current[orig] = vn
        } else {
            out[i] = rename_reads(s, current)
        }
        i ++
    }
    delete ssaNames
    delete current
}

class private FoldVisitor : AstVisitor {
    changed : bool
    def override visitExprOp2(var that : ExprOp2?) : ExpressionPtr {
        // Algebraic identities. Integer arms are always exact (two's-complement,
        // `e*-1==-e` even at INT_MIN). Float arms fire under flatten's fast-math
        // assumption — every consumer is a shader backend: `x*1`/`x*-1` are exact for
        // all floats (incl. NaN/inf/±0), while `x*0`/`x+0`/`x-0` are lossy (NaN/inf·0
        // = NaN; +0.0 flips -0.0) but enabled by decision. Only `*0` DROPS an operand
        // (side-effect gate) and RETURNS the scalar zero, so both the int and float arms
        // gate it to a scalar result (`same_base_type`) — a vector `vec*0` would otherwise
        // mistype to the scalar zero; full-const eval handles a constant vector operand.
        if (that.op == "*") {
            if (is_int_val(that.right, 0l) && !has_sideeffects(that.left) && same_base_type(that, that.right)) { changed = true; return that.right }
            if (is_int_val(that.left, 0l) && !has_sideeffects(that.right) && same_base_type(that, that.left)) { changed = true; return that.left }
            if (is_int_val(that.right, 1l)) { changed = true; return that.left }
            if (is_int_val(that.left, 1l)) { changed = true; return that.right }
            if (is_int_neg_one(that.right)) { changed = true; return negate(that.left) }
            if (is_int_neg_one(that.left)) { changed = true; return negate(that.right) }
            if (is_real_val(that.right, 1.0lf)) { changed = true; return that.left }
            if (is_real_val(that.left, 1.0lf)) { changed = true; return that.right }
            if (is_real_val(that.right, -1.0lf)) { changed = true; return negate(that.left) }
            if (is_real_val(that.left, -1.0lf)) { changed = true; return negate(that.right) }
            if (is_real_val(that.right, 0.0lf) && !has_sideeffects(that.left) && same_base_type(that, that.right)) { changed = true; return that.right }
            if (is_real_val(that.left, 0.0lf) && !has_sideeffects(that.right) && same_base_type(that, that.left)) { changed = true; return that.left }
        } elif (that.op == "+") {
            if (is_int_val(that.right, 0l)) { changed = true; return that.left }
            if (is_int_val(that.left, 0l)) { changed = true; return that.right }
            if (is_real_val(that.right, 0.0lf)) { changed = true; return that.left }
            if (is_real_val(that.left, 0.0lf)) { changed = true; return that.right }
        } elif (that.op == "-") {
            if (is_int_val(that.right, 0l)) { changed = true; return that.left }
            if (is_int_val(that.left, 0l)) { changed = true; return negate(that.right) }
            if (is_real_val(that.right, 0.0lf)) { changed = true; return that.left }
            if (is_real_val(that.left, 0.0lf)) { changed = true; return negate(that.right) }
        } elif (that.op == "&&") {
            // Short-circuit `&&`: `false && x` never evaluates x, so dropping x is
            // safe; `x && false` DOES evaluate x, so that drop is gated on purity.
            if (is_bool_val(that.left, true)) { changed = true; return that.right }
            if (is_bool_val(that.right, true)) { changed = true; return that.left }
            if (is_bool_val(that.left, false)) { changed = true; return that.left }
            if (is_bool_val(that.right, false) && !has_sideeffects(that.left)) { changed = true; return that.right }
        } elif (that.op == "||") {
            // Mirror of `&&`: `true || x` never evaluates x; `x || true` does.
            if (is_bool_val(that.left, false)) { changed = true; return that.right }
            if (is_bool_val(that.right, false)) { changed = true; return that.left }
            if (is_bool_val(that.left, true)) { changed = true; return that.left }
            if (is_bool_val(that.right, true) && !has_sideeffects(that.left)) { changed = true; return that.right }
        }
        return that
    }
    def override visitExprOp1(var that : ExprOp1?) : ExpressionPtr {
        // `!true → false`, `!false → true` — const operand only (already folded
        // post-order). Double-negation `!!x → x` is intentionally out of scope.
        if (that.op == "!") {
            if (is_bool_val(that.subexpr, true)) { changed = true; return new ExprConstBool(at = that.at, value = false) }
            if (is_bool_val(that.subexpr, false)) { changed = true; return new ExprConstBool(at = that.at, value = true) }
        }
        return that
    }
    def override visitExprOp3(var that : ExprOp3?) : ExpressionPtr {
        // Post-order: subexpr/arms already folded, so a now-constant condition
        // collapses to its live arm (the dead arm + condition become unreachable).
        if (that.subexpr is ExprConstBool) {
            changed = true
            return (that.subexpr as ExprConstBool).value ? that.left : that.right
        }
        // Bool-const arms collapse to the condition: `c ? true : false → c`,
        // `c ? false : true → !c`. Common after if-conversion of a bool-valued branch.
        if (is_bool_val(that.left, true) && is_bool_val(that.right, false)) {
            changed = true
            return that.subexpr
        }
        if (is_bool_val(that.left, false) && is_bool_val(that.right, true)) {
            changed = true
            return new ExprOp1(at = that.at, op := "!", subexpr = that.subexpr)
        }
        // Equal arms (`c ? a : a → a`) — e.g. after `acc + 0 → acc` makes both arms `acc`.
        // Gated on a pure condition (dropping it must not lose a side effect).
        if (describe(that.left) == describe(that.right) && !has_sideeffects(that.subexpr)) {
            changed = true
            return that.left
        }
        return that
    }
    def override visitExprCall(var that : ExprCall?) : ExpressionPtr {
        // A fully-constant, side-effect-free call — a vector constructor (`float3(0)`
        // stays `float3(0f,0f,0f)`, never a const) or a pure builtin over const args —
        // folds to a single const literal. The typer folds scalar const ARITHMETIC
        // during re-infer (`2*3 → 6`) but leaves const CONSTRUCTORS; this collapses
        // them. Post-order, so const-folded args are visible here.
        if (is_all_const(that)) {
            var folded = eval_to_const(that)
            if (!(folded is ExprCall)) {   // eval_to_const returns the call unchanged on failure
                changed = true
                return folded
            }
        }
        return that
    }
}

// A scalar zero/default literal: `default<T>` for every workhorse scalar is the
// zero bit-pattern (int 0, float 0.0, bool false, …). A `var x = <this>` initializer
// is therefore redundant with daslang's guaranteed zero-init of a bare `var x : T`,
// so it can be dropped. Non-scalar inits (vector/struct ctors like `float3(0)`) are
// NOT literals here and are left alone — conservative by design.
def private is_zero_const(e : ExpressionPtr) : bool {
    if (e == null) return false
    if (e is ExprConstInt) return (e as ExprConstInt).value == 0
    if (e is ExprConstInt8) return (e as ExprConstInt8).value == int8(0)
    if (e is ExprConstInt16) return (e as ExprConstInt16).value == int16(0)
    if (e is ExprConstInt64) return (e as ExprConstInt64).value == 0l
    if (e is ExprConstUInt) return (e as ExprConstUInt).value == 0u
    if (e is ExprConstUInt8) return (e as ExprConstUInt8).value == uint8(0)
    if (e is ExprConstUInt16) return (e as ExprConstUInt16).value == uint16(0)
    if (e is ExprConstUInt64) return (e as ExprConstUInt64).value == 0ul
    if (e is ExprConstFloat) return (e as ExprConstFloat).value == 0.0f
    if (e is ExprConstDouble) return (e as ExprConstDouble).value == 0.0lf
    if (e is ExprConstBool) return !(e as ExprConstBool).value
    return false
}

// Extract an integer constant's value (widened to int64) and whether its type is
// signed. Float/double/bool consts are NOT integers and return false — the algebraic
// identities only hold for integer arithmetic.
def private int_const(e : ExpressionPtr; var v : int64&; var signed : bool&) : bool {
    if (e == null) return false
    if (e is ExprConstInt) { v = int64((e as ExprConstInt).value); signed = true; return true }
    if (e is ExprConstInt8) { v = int64((e as ExprConstInt8).value); signed = true; return true }
    if (e is ExprConstInt16) { v = int64((e as ExprConstInt16).value); signed = true; return true }
    if (e is ExprConstInt64) { v = (e as ExprConstInt64).value; signed = true; return true }
    if (e is ExprConstUInt) { v = int64((e as ExprConstUInt).value); signed = false; return true }
    if (e is ExprConstUInt8) { v = int64((e as ExprConstUInt8).value); signed = false; return true }
    if (e is ExprConstUInt16) { v = int64((e as ExprConstUInt16).value); signed = false; return true }
    if (e is ExprConstUInt64) { v = int64((e as ExprConstUInt64).value); signed = false; return true }
    return false
}

def private is_int_val(e : ExpressionPtr; want : int64) : bool {
    var v = 0l
    var signed = false
    return int_const(e, v, signed) && v == want
}

def private is_int_neg_one(e : ExpressionPtr) : bool {
    // Signed only — an unsigned "-1" is a large positive, not a negation factor.
    var v = 0l
    var signed = false
    return int_const(e, v, signed) && signed && v == -1l
}

// A scalar float/double const equal to `want`. `want` is exact (0/1/-1) in both
// float and double, so the widening compare is exact. Vector consts (ExprConstFloat3
// …) are NOT scalars and return false — the float identities are scalar-operand only.
def private is_real_val(e : ExpressionPtr; want : double) : bool {
    if (e == null) return false
    if (e is ExprConstFloat) return double((e as ExprConstFloat).value) == want
    if (e is ExprConstDouble) return (e as ExprConstDouble).value == want
    return false
}

// A const bool equal to `want`. Backs the boolean algebraic identities
// (`true && x → x`, `c ? true : false → c`, `!false → true`, …).
def private is_bool_val(e : ExpressionPtr; want : bool) : bool {
    if (e == null) return false
    if (e is ExprConstBool) return (e as ExprConstBool).value == want
    return false
}

// True if `e` is an op2 that FoldVisitor's UNCONDITIONAL algebraic identities would
// have rewritten away: `x*1`/`1*x`/`x*-1`/`-1*x`, `x+0`/`0+x`, `x-0`/`0-x`. These
// fold for any operand type with no side-effect or vector gate (unlike `x*0`, which
// is excluded here — it carries the scalar-result + purity gates), and the typer
// never folds a runtime-operand identity. A survivor of these is a const-fold miss —
// the high-teeth half of the strict_fold verification invariant.
def public is_unfolded_identity_op2(e : ExprOp2?) : bool {
    if (e == null) return false
    if (e.op == "*") {
        return (is_int_val(e.right, 1l) || is_int_val(e.left, 1l) ||
            is_real_val(e.right, 1.0lf) || is_real_val(e.left, 1.0lf) ||
            is_int_neg_one(e.right) || is_int_neg_one(e.left) ||
            is_real_val(e.right, -1.0lf) || is_real_val(e.left, -1.0lf))
    }
    if (e.op == "+" || e.op == "-") {
        return (is_int_val(e.right, 0l) || is_int_val(e.left, 0l) ||
            is_real_val(e.right, 0.0lf) || is_real_val(e.left, 0.0lf))
    }
    return false
}

def private same_base_type(a, b : ExpressionPtr) : bool {
    if (a == null || b == null || a._type == null || b._type == null) return false
    return a._type.baseType == b._type.baseType
}

def private negate(var e : ExpressionPtr) : ExpressionPtr {
    return new ExprOp1(at = e.at, op := "-", subexpr = e)
}

// True if `e` is built only from const literals and pure ops/builtins over them — NO
// variable/global reference and NO user call anywhere. eval_single_expression runs in a
// bare context with no globals (a global read dereferences null) and no user functions
// linked (a user call can't be evaluated), so the tree must bottom out in literals and
// pure builtins. Conservative: an unknown node kind returns false.
def public is_all_const(e : ExpressionPtr) : bool {
    if (e == null) return false
    if (e |> is_expr_const()) return true
    if (e is ExprVar) return false
    if (e is ExprOp1) return is_all_const((e as ExprOp1).subexpr)
    if (e is ExprOp2) {
        let o = e as ExprOp2
        return is_all_const(o.left) && is_all_const(o.right)
    }
    if (e is ExprOp3) {
        let o = e as ExprOp3
        return is_all_const(o.subexpr) && is_all_const(o.left) && is_all_const(o.right)
    }
    if (e is ExprCall) {
        let c = e as ExprCall
        // Reject a user/unresolved call (not evaluable) or any side-effecting builtin.
        if (c.func == null || !c.func.flags.builtIn || has_sideeffects(e)) return false
        for (a in c.arguments) {
            if (!is_all_const(a)) return false
        }
        return true
    }
    return false
}

// True if a constant of this type can be rebuilt as a literal by eval_to_const — the
// float4-based evaluator covers the float/int scalar + vector family. Gates the cycle-1
// const-builtin allowance so only foldable-type calls bypass the whitelist.
def public eval_to_const_supported(t : TypeDeclPtr) : bool {
    if (t == null) return false
    let bt = t.baseType
    return (bt == Type.tFloat || bt == Type.tFloat2 || bt == Type.tFloat3 || bt == Type.tFloat4 ||
        bt == Type.tInt || bt == Type.tInt2 || bt == Type.tInt3 || bt == Type.tInt4)
}

// Evaluate a literal-only (is_all_const) inferred expression to one const literal of
// its type, via the exact runtime evaluator. Returns `e` unchanged if eval fails or
// the type is unsupported. The fresh const is un-inferred; the macro re-infers it.
def private eval_to_const(var e : ExpressionPtr) : ExpressionPtr {
    if (e == null || e._type == null) return e
    var ok = false
    var val : float4
    unsafe {
        val = eval_single_expression(e, ok)
    }
    if (!ok) return e
    let bt = e._type.baseType
    if (bt == Type.tFloat) return new ExprConstFloat(at = e.at, value = val.x)
    if (bt == Type.tFloat2) return new ExprConstFloat2(at = e.at, value = float2(val.x, val.y))
    if (bt == Type.tFloat3) return new ExprConstFloat3(at = e.at, value = float3(val.x, val.y, val.z))
    if (bt == Type.tFloat4) return new ExprConstFloat4(at = e.at, value = val)
    unsafe {
        if (bt == Type.tInt) return new ExprConstInt(at = e.at, value = reinterpret<int>(val.x))
        if (bt == Type.tInt2) return new ExprConstInt2(at = e.at, value = int2(reinterpret<int>(val.x), reinterpret<int>(val.y)))
        if (bt == Type.tInt3) return new ExprConstInt3(at = e.at, value = int3(reinterpret<int>(val.x), reinterpret<int>(val.y), reinterpret<int>(val.z)))
        if (bt == Type.tInt4) return new ExprConstInt4(at = e.at, value = int4(reinterpret<int>(val.x), reinterpret<int>(val.y), reinterpret<int>(val.z), reinterpret<int>(val.w)))
    }
    return e
}

// ===== Generalized const-propagation of single-def user locals =====
//
// After SSA-rename every user local is single-definition, so a `var x = <const>` can
// be substituted at all of x's uses and dropped. Because flatten linearized the body,
// one forward pass folds a whole accumulator chain: each link's init is substituted
// with the prior constants and evaluated exactly via the shared runtime evaluator
// (types are present post-infer), so `var acc__s1=5; var acc__s2=acc__s1+3; …` collapses
// to a single constant in place — no re-infer leg per link. Masks (`__flat_*`) are left
// to const_prop_pass; globals/fields are never substituted (only their read operands).

def private reads_any(var e : ExpressionPtr; names : table<string; ExpressionPtr>) : bool {
    for (nm in keys(names)) {
        if (count_refs(e, nm) > 0) return true
    }
    return false
}

def private subst_consts(var e : ExpressionPtr; constmap : table<string; ExpressionPtr>) : ExpressionPtr {
    if (e == null) return null
    var tmpl : Template
    for (nm in keys(constmap)) {
        // ?[] yields a const view (non-inserting; keys() locks the table) but the pointed-to
        // node is genuinely mutable. apply_template clones it per use, so const-strip the raw
        // pointer instead of pre-cloning (which would orphan a copy every substitution).
        var ex = unsafe(reinterpret<Expression? -const>(constmap?[nm] ?? null))
        tmpl |> replaceVariable(nm, ex)
    }
    var res = apply_template(tmpl, e.at, e, false)
    unsafe { delete tmpl }
    return res
}

def private body_read_count(var blk : ExprBlock?; nm : string) : int {
    var total = 0
    for (j in range(length(blk.list))) {
        total += count_refs(blk.list[j], nm)
    }
    return total
}

def private const_prop_locals(var blk : ExprBlock?) : bool {
    var changed = false
    var constmap : table<string; ExpressionPtr>
    var i = 0
    while (i < length(blk.list)) {
        // Substitute already-known constants into this statement's reads (the next
        // link's init, a final `g_out = acc`, a `return acc`, …).
        if (!empty(constmap) && reads_any(blk.list[i], constmap)) {
            blk.list[i] = subst_consts(blk.list[i], constmap)
            changed = true
        }
        // A single-def user local whose (now-substituted) init is literal-only folds to
        // one constant and joins the map for the rest of the pass.
        var s = blk.list[i]
        if (s is ExprLet && length((s as ExprLet).variables) == 1) {
            var lc = s as ExprLet
            if (!is_flat_temp(lc.variables[0].name) && lc.variables[0].init != null && is_all_const(lc.variables[0].init)) {
                // eval_single_expression only READS the tree (throwaway context), so fold the
                // init in place — no defensive clone.
                var oldInit = lc.variables[0].init
                var folded = eval_to_const(oldInit)
                if (folded |> is_expr_const() && folded != oldInit) {
                    lc.variables[0].init = folded
                    constmap["{lc.variables[0].name}"] = folded
                    changed = true
                }
            }
        }
        i ++
    }
    // Drop the now-dead decls of fully-propagated locals (every use was substituted).
    if (!empty(constmap)) {
        var dead : table<int>
        for (j in range(length(blk.list))) {
            let s = blk.list[j]
            if (s is ExprLet && length((s as ExprLet).variables) == 1) {
                let nm = "{(s as ExprLet).variables[0].name}"
                if (key_exists(constmap, nm) && body_read_count(blk, nm) == 0) {
                    dead |> insert(j)
                }
            }
        }
        if (!empty(dead)) {
            // blk.list is a managed vector — erase high-to-low so indices stay valid.
            var si = length(blk.list) - 1
            while (si >= 0) {
                if (key_exists(dead, si)) {
                    blk.list |> erase(si)
                }
                si --
            }
            changed = true
        }
        delete dead
    }
    return changed
}

def public fold_body(var func : FunctionPtr) : bool {
    //! Post-infer fold over a flattened body: const-propagate single-def user locals
    //! (accumulator chains → a constant), collapse `const ? a : b` to the live arm,
    //! fold algebraic / boolean identities and const constructors, drop the
    //! side-effect-free `lhs = lhs` self-assigns a folded false-select leaves behind,
    //! and strip redundant zero/default inits (`var acc = 0` → `var acc : int`). Call
    //! AFTER re-inference (conditions must be `ExprConstBool`, types must be settled).
    if (!(func.body is ExprBlock)) return false
    var changed = const_prop_locals(func.body as ExprBlock)
    var v = new FoldVisitor()
    make_visitor(*v) $(adapter) {
        visit(func.body, adapter)
    }
    if (v.changed) {
        changed = true
    }
    unsafe {
        delete v
    }
    // Two statement-level cleanups over the (post-fold) body. Erase in reverse so
    // earlier indices stay valid as we remove:
    //   1. Drop pure self-assigns (`acc = acc`) a collapsed false-select leaves behind.
    //      has_sideeffects gates it — `g[k()] = g[k()]` keeps (the lvalue is not free).
    //   2. Strip a redundant zero/default initializer: `var acc = 0` → `var acc : int`,
    //      since the bare decl already zero-inits. This is always a local decl (you can't
    //      `var` a global), so it never touches observable state. Post-infer the var's
    //      type is settled, so dropping the init keeps the declared type intact.
    var blk = func.body as ExprBlock
    var si = length(blk.list) - 1
    while (si >= 0) {
        var s = blk.list[si]
        if (s is ExprCopy) {
            let cp = s as ExprCopy
            if (describe(cp.left) == describe(cp.right) && !has_sideeffects(cp.left)) {
                blk.list |> erase(si)
                changed = true
            }
        } elif (s is ExprLet) {
            var lt = s as ExprLet
            for (lv in lt.variables) {
                if (is_zero_const(lv.init)) {
                    lv.init = null
                    changed = true
                }
            }
        }
        si --
    }
    return changed
}
