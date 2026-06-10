options gen2
options indenting = 4
options strict_smart_pointers = true
options _comment_hygiene = true

module flatten_opt shared private

//! Internal optimization passes for daslib/flatten: constant folding, constant
//! propagation, copy propagation, and dead-store elimination over a flattened
//! twin's straight-line body, plus the post-infer select-fold (`fold_body`),
//! the preshader/CSE optimize pass and the mad/lerp re-fusion finishing pass.
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
// Straight-line body → liveness is one backward scan. Touches only `__flat_*` locals (never
// user locals/globals); drops a store whose target is dead afterward and whose RHS is pure.

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

// ===== Boolean const-fold of live masks =====
// With no early return `__flat_live` stays constant `true`; copy_prop inlines the single-def
// mask and fold_stmt collapses the selects it exposes (`true ? a:b → a`), unwinding the scaffold.

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

// Apply fold_const at the STATEMENT level: fold the sub-expression of an assignment /
// decl-init / return in place (fold_const itself only folds expression trees, not the
// ExprCopy/ExprLet/ExprReturn wrappers). Used by copy_prop after it inlines a const mask.
def private fold_stmt(var s : ExpressionPtr; constmap : table<uint64; bool>) : ExpressionPtr {
    if (s is ExprCopy) {
        var cp = s as ExprCopy
        cp.right = fold_const(cp.right, constmap)
        return s
    } elif (s is ExprLet) {
        var lc = s as ExprLet
        for (v in lc.variables) {
            if (v.init != null) {
                v.init = fold_const(v.init, constmap)
            }
        }
        return s
    } elif (s is ExprReturn) {
        var r = s as ExprReturn
        if (r.subexpr != null) {
            r.subexpr = fold_const(r.subexpr, constmap)
        }
        return s
    }
    return fold_const(s, constmap)
}

// ===== Copy propagation of single-assignment flatten temps =====
// A single-def `__flat_*` temp whose source reads no later-reassigned var is a pure forward copy:
// inline the source at every use, drop the temp (a risky call/index/divide source only at one use).

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

// Per-pass reference index: name-hash -> referencing statement indices (deduped) + total count.
// Built once per copy_prop pass, replacing the old per-candidate count_refs sweep that was
// O(n^3) on large bodies.
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
    // Empty map: fold_const with no var bindings still simplifies LITERAL const-bools
    // (`true && c → c`, `true ? a : b → a`) freshly inlined below. This is how copy-prop
    // subsumes the old const_prop_pass: inline a proven-const mask, then collapse its uses.
    var emptyConstMap : table<uint64; bool>
    var changed = true
    while (changed) {
        changed = false
        let n = length(out)
        // Precompute once per pass: each store target's indices + a name -> (ref statements, count)
        // index. A source reading V is unsafe only if V is reassigned BETWEEN this temp's def and a
        // use (a reassignment fully before the def leaves the captured value intact).
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
            // Single definition: decl-init + no stores, OR no-init decl + one store. Use the ORIGINAL
            // source (no clone) — apply_template clones per use; pre-cloning would orphan a copy on
            // every non-folding candidate (re-examined each while-restart).
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
            let constSrc = src is ExprConstBool
            for (j in useJs) {
                out[j] = apply_template(rules, out[j].at, out[j], false)   // only statements that reference nm
                if (constSrc) {
                    out[j] = fold_stmt(out[j], emptyConstMap)   // collapse the now-literal mask: true && c → c
                }
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
    delete emptyConstMap
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
// Straight-line body → a reassigned local is a linear def-chain (no phi); renaming each write to a
// fresh single-def `__ssa_` version lets post-infer const-prop fold the chain (declared-with-init only).

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
            let vn = "__ssa_{orig}_{counter}"
            out[i] = qmacro_expr(${ var $i(vn) = $e(newInit); })
            current[orig] = vn
        } elif (s is ExprCopy && (s as ExprCopy).left is ExprVar &&
                key_exists(ssaNames, hash(((s as ExprCopy).left as ExprVar).name))) {
            let orig = "{((s as ExprCopy).left as ExprVar).name}"
            let newRhs = rename_reads((s as ExprCopy).right, current)
            counter ++
            let vn = "__ssa_{orig}_{counter}"
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
    noFastMath : bool
    def override visitExprOp2(var that : ExprOp2?) : ExpressionPtr {
        // A fully-constant ExprOp2 the typer left unfolded — it folds scalar const arithmetic but
        // leaves vector const (`float3(2)+float3(3)`), what reassoc's gathered groups become. Fold
        // to one literal via the evaluator; an un-inferred node is skipped (folded next re-infer).
        if (is_all_const(that) && eval_to_const_supported(that._type)) {
            var folded = eval_to_const(that)
            if (folded |> is_expr_const()) { changed = true; return folded }
        }
        // Algebraic identities over the scalar + vector (float/int/uint) family, under fast-math.
        // `*1`/`*-1` return the non-const operand and gate on `same_base_type` (scalar*vector
        // broadcast must not mistype to scalar); `*0` returns a RESULT-typed zero via zero_const_of.
        if (that.op == "*") {
            if (is_one_const_op(that.right) && same_base_type(that, that.left)) { changed = true; return that.left }
            if (is_one_const_op(that.left) && same_base_type(that, that.right)) { changed = true; return that.right }
            if (is_neg_one_const_op(that.right) && same_base_type(that, that.left)) { changed = true; return negate(that.left) }
            if (is_neg_one_const_op(that.left) && same_base_type(that, that.right)) { changed = true; return negate(that.right) }
            // `*0` changes inf/NaN propagation — float forms are fast-math only (int exact)
            if (is_zero_const_op(that.right) && !has_sideeffects(that.left) && (!noFastMath || !is_float_valued(that))) {
                var z = zero_const_of(that._type, that.at)
                if (z != null) { changed = true; return z }
            }
            if (is_zero_const_op(that.left) && !has_sideeffects(that.right) && (!noFastMath || !is_float_valued(that))) {
                var z = zero_const_of(that._type, that.at)
                if (z != null) { changed = true; return z }
            }
        } elif (that.op == "+") {
            if (is_zero_const_op(that.right)) { changed = true; return that.left }
            if (is_zero_const_op(that.left)) { changed = true; return that.right }
        } elif (that.op == "-") {
            if (is_zero_const_op(that.right)) { changed = true; return that.left }
            if (is_zero_const_op(that.left)) { changed = true; return negate(that.right) }
            // `x - x → 0` — purity-gated like `*0` (and like it, inf/NaN operands aside:
            // fast-math charter). Identical describe ⇒ identical purity, so one check.
            if (describe(that.left) == describe(that.right) && !has_sideeffects(that.left) &&
                    (!noFastMath || !is_float_valued(that))) {
                var z = zero_const_of(that._type, that.at)
                if (z != null) { changed = true; return z }
            }
        } elif (that.op == "/") {
            // `x / C → x * (1/C)` — a mul instead of a div; the reciprocal rounds (fast-math).
            // Float-family only (int division has no reciprocal); zero lanes keep the div.
            if (!noFastMath && is_float_valued(that)) {
                var rec = recip_const(that.right)
                if (rec != null) {
                    changed = true
                    return new ExprOp2(at = that.at, op := "*", left = that.left, right = rec)
                }
            }
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
        // A fully-constant, side-effect-free call (vector constructor or pure builtin over const
        // args) folds to one literal. The typer folds const ARITHMETIC but leaves const
        // CONSTRUCTORS; this collapses them. Post-order, so const-folded args are visible.
        if (is_all_const(that)) {
            var folded = eval_to_const(that)
            if (!(folded is ExprCall)) {   // eval_to_const returns the call unchanged on failure
                changed = true
                return folded
            }
        }
        // DISASSEMBLE lerp/mad into raw arithmetic so the identity/const folds, reassoc and CSE
        // see through them; the PHASE_FUSED finishing pass re-packs survivors. Type-gated to the
        // das math overload set — a loose DSL-stub instantiation must not expand into ill-typed ops.
        if (that.func != null && length(that.arguments) == 3) {
            let fname = psh_simple_name("{that.func.name}")
            let ta = expr_bt(that.arguments[0])
            let tb = expr_bt(that.arguments[1])
            let tc = expr_bt(that.arguments[2])
            if (fname == "lerp" && lerp_shape(ta, tb, tc)) {
                var aArg = that.arguments[0]
                var bArg = that.arguments[1]
                var tArg = that.arguments[2]
                // const-selector short-circuits — no expand/refuse round trip. They DROP an
                // argument (`(b-a)*0` would still propagate b's inf/NaN) → fast-math only.
                if (!noFastMath) {
                    if (is_zero_const_op(tArg) && !has_sideeffects(bArg)) { changed = true; return aArg }
                    if (is_one_const_op(tArg) && !has_sideeffects(aArg)) { changed = true; return bArg }
                    if (describe(aArg) == describe(bArg) && !has_sideeffects(bArg) && !has_sideeffects(tArg)) {
                        changed = true
                        return aArg
                    }
                }
                changed = true
                var sub = new ExprOp2(at = that.at, op := "-", left = bArg, right = clone_expression(aArg))
                var mul = new ExprOp2(at = that.at, op := "*", left = sub, right = tArg)
                return new ExprOp2(at = that.at, op := "+", left = mul, right = aArg)
            }
            if (fname == "mad" && mad_shape(ta, tb, tc)) {
                changed = true
                var mul = new ExprOp2(at = that.at, op := "*", left = that.arguments[0], right = that.arguments[1])
                return new ExprOp2(at = that.at, op := "+", left = mul, right = that.arguments[2])
            }
        }
        return that
    }
}

// A scalar zero/default literal (int 0, float 0.0, bool false, …). A `var x = <this>` init is
// redundant with daslang's zero-init of a bare `var x : T`, so it can be dropped. Non-scalar
// inits (vector/struct ctors) are not literals here and left alone.
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
// …) are NOT scalars and return false — is_real_vec_val (below) is the vector form.
def private is_real_val(e : ExpressionPtr; want : double) : bool {
    if (e == null) return false
    if (e is ExprConstFloat) return double((e as ExprConstFloat).value) == want
    if (e is ExprConstDouble) return (e as ExprConstDouble).value == want
    return false
}

// An all-lanes-equal float vector const (ExprConstFloat2/3/4) equal to `want`. Extends scalar
// is_real_val so the float identities fire for vector-const operands (`v * float3(1)`); `want`
// is exact (0/1/-1) so the widening compare is exact.
def private is_real_vec_val(e : ExpressionPtr; want : double) : bool {
    if (e == null) return false
    if (e is ExprConstFloat2) { let v = (e as ExprConstFloat2).getValue; return double(v.x) == want && double(v.y) == want }
    if (e is ExprConstFloat3) { let v = (e as ExprConstFloat3).getValue; return double(v.x) == want && double(v.y) == want && double(v.z) == want }
    if (e is ExprConstFloat4) { let v = (e as ExprConstFloat4).getValue; return double(v.x) == want && double(v.y) == want && double(v.z) == want && double(v.w) == want }
    return false
}

// An all-lanes-equal int OR uint vector const equal to `want` (want >= 0). Mirrors is_int_val
// covering both signed and unsigned scalar consts — uint vectors fold for 0/1 but not -1.
def private is_int_vec_val(e : ExpressionPtr; want : int64) : bool {
    if (e == null) return false
    if (e is ExprConstInt2) { let v = (e as ExprConstInt2).getValue; return int64(v.x) == want && int64(v.y) == want }
    if (e is ExprConstInt3) { let v = (e as ExprConstInt3).getValue; return int64(v.x) == want && int64(v.y) == want && int64(v.z) == want }
    if (e is ExprConstInt4) { let v = (e as ExprConstInt4).getValue; return int64(v.x) == want && int64(v.y) == want && int64(v.z) == want && int64(v.w) == want }
    if (e is ExprConstUInt2) { let v = (e as ExprConstUInt2).getValue; return int64(v.x) == want && int64(v.y) == want }
    if (e is ExprConstUInt3) { let v = (e as ExprConstUInt3).getValue; return int64(v.x) == want && int64(v.y) == want && int64(v.z) == want }
    if (e is ExprConstUInt4) { let v = (e as ExprConstUInt4).getValue; return int64(v.x) == want && int64(v.y) == want && int64(v.z) == want && int64(v.w) == want }
    return false
}

// An all-lanes -1 signed int vector const (ExprConstInt2/3/4 only). Mirrors is_int_neg_one —
// an unsigned vector "-1" is a large positive, never a negation factor (the "uint ≈ int,
// almost" boundary).
def private is_int_vec_neg_one(e : ExpressionPtr) : bool {
    if (e == null) return false
    if (e is ExprConstInt2) { let v = (e as ExprConstInt2).getValue; return v.x == -1 && v.y == -1 }
    if (e is ExprConstInt3) { let v = (e as ExprConstInt3).getValue; return v.x == -1 && v.y == -1 && v.z == -1 }
    if (e is ExprConstInt4) { let v = (e as ExprConstInt4).getValue; return v.x == -1 && v.y == -1 && v.z == -1 && v.w == -1 }
    return false
}

// Combined const-operand predicates for the algebraic identities: scalar OR all-lanes-equal
// vector const. `*0`/`+0`/`-0` use is_zero_const_op, `*1` uses is_one_const_op, `*-1` uses
// is_neg_one_const_op (signed/float only).
def private is_zero_const_op(e : ExpressionPtr) : bool {
    return is_int_val(e, 0l) || is_real_val(e, 0.0lf) || is_int_vec_val(e, 0l) || is_real_vec_val(e, 0.0lf)
}

def private is_one_const_op(e : ExpressionPtr) : bool {
    return is_int_val(e, 1l) || is_real_val(e, 1.0lf) || is_int_vec_val(e, 1l) || is_real_vec_val(e, 1.0lf)
}

def private is_neg_one_const_op(e : ExpressionPtr) : bool {
    return is_int_neg_one(e) || is_real_val(e, -1.0lf) || is_int_vec_neg_one(e) || is_real_vec_val(e, -1.0lf)
}

// A const bool equal to `want`. Backs the boolean algebraic identities
// (`true && x → x`, `c ? true : false → c`, `!false → true`, …).
def private is_bool_val(e : ExpressionPtr; want : bool) : bool {
    if (e == null) return false
    if (e is ExprConstBool) return (e as ExprConstBool).value == want
    return false
}

// True if `e` is an op2 FoldVisitor's identity arms would rewrite away (`x*1`/`x*-1`/`x+0`/`x-0`,
// scalar or vector const). MIRRORS the fold arms exactly — `*1`/`*-1` carry the `same_base_type`
// gate, `+`/`-` don't, `x*0` is excluded (purity-gated, so a survivor isn't a miss).
def public is_unfolded_identity_op2(e : ExprOp2?) : bool {
    if (e == null) return false
    if (e.op == "*") {
        return (((is_one_const_op(e.right) || is_neg_one_const_op(e.right)) && same_base_type(e, e.left)) ||
            ((is_one_const_op(e.left) || is_neg_one_const_op(e.left)) && same_base_type(e, e.right)))
    }
    if (e.op == "+" || e.op == "-") {
        return (is_zero_const_op(e.right) || is_zero_const_op(e.left))
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

// A fresh zero literal of `t` (the RESULT type of `X * 0`; also the lowering's materialized
// implicit zero-init on bare decls), over the numeric scalar + vector family; null otherwise.
// Keying on the result type lets both `v * 0.0` and `v * float3(0)` fold width-matched.
def public zero_const_of(t : TypeDeclPtr; at : LineInfo) : ExpressionPtr {
    if (t == null) return null
    let bt = t.baseType
    if (bt == Type.tInt) return new ExprConstInt(at = at, value = 0)
    if (bt == Type.tInt8) return new ExprConstInt8(at = at, value = int8(0))
    if (bt == Type.tInt16) return new ExprConstInt16(at = at, value = int16(0))
    if (bt == Type.tInt64) return new ExprConstInt64(at = at, value = 0l)
    if (bt == Type.tUInt) return new ExprConstUInt(at = at, value = 0u)
    if (bt == Type.tUInt8) return new ExprConstUInt8(at = at, value = uint8(0))
    if (bt == Type.tUInt16) return new ExprConstUInt16(at = at, value = uint16(0))
    if (bt == Type.tUInt64) return new ExprConstUInt64(at = at, value = 0ul)
    if (bt == Type.tFloat) return new ExprConstFloat(at = at, value = 0.0f)
    if (bt == Type.tDouble) return new ExprConstDouble(at = at, value = 0.0lf)
    if (bt == Type.tFloat2) return new ExprConstFloat2(at = at, value = float2(0.0))
    if (bt == Type.tFloat3) return new ExprConstFloat3(at = at, value = float3(0.0))
    if (bt == Type.tFloat4) return new ExprConstFloat4(at = at, value = float4(0.0))
    if (bt == Type.tInt2) return new ExprConstInt2(at = at, value = int2(0))
    if (bt == Type.tInt3) return new ExprConstInt3(at = at, value = int3(0))
    if (bt == Type.tInt4) return new ExprConstInt4(at = at, value = int4(0))
    if (bt == Type.tUInt2) return new ExprConstUInt2(at = at, value = uint2(0u))
    if (bt == Type.tUInt3) return new ExprConstUInt3(at = at, value = uint3(0u))
    if (bt == Type.tUInt4) return new ExprConstUInt4(at = at, value = uint4(0u))
    return null
}

def private expr_bt(e : ExpressionPtr) : Type {
    return (e == null || e._type == null) ? Type.none : e._type.baseType
}

// Scalar element of a vector baseType (tFloat3 → tFloat); Type.none for non-vectors.
def private vec_elem_bt(bt : Type) : Type {
    if (bt == Type.tFloat2 || bt == Type.tFloat3 || bt == Type.tFloat4) return Type.tFloat
    if (bt == Type.tInt2 || bt == Type.tInt3 || bt == Type.tInt4) return Type.tInt
    if (bt == Type.tUInt2 || bt == Type.tUInt3 || bt == Type.tUInt4) return Type.tUInt
    return Type.none
}

def private is_float_family(bt : Type) : bool {
    return bt == Type.tFloat || bt == Type.tFloat2 || bt == Type.tFloat3 || bt == Type.tFloat4
}

// Float-valued (float/double scalar or float vector) — the fast-math gates key on this:
// integer/bool identities are exact, floating-point ones change inf/NaN/rounding behavior.
def private is_float_valued(e : ExpressionPtr) : bool {
    let bt = expr_bt(e)
    return is_float_family(bt) || bt == Type.tDouble
}


// The (T,T,T) element family das `mad` covers: float/int/uint scalar+vector, double scalar.
def private is_mad_elem(bt : Type) : bool {
    return (is_float_family(bt) || bt == Type.tDouble ||
        bt == Type.tInt || bt == Type.tInt2 || bt == Type.tInt3 || bt == Type.tInt4 ||
        bt == Type.tUInt || bt == Type.tUInt2 || bt == Type.tUInt3 || bt == Type.tUInt4)
}

// `mad(a, b, c)` overload shapes (das math): (T,T,T) over the numeric family, or the
// vec-scalar-vec broadcast form (MadS). The single gate for expand, fuse AND residual.
def private mad_shape(ta, tb, tc : Type) : bool {
    if (ta == tb && tb == tc) return is_mad_elem(ta)
    return ta == tc && tb != Type.none && vec_elem_bt(ta) == tb
}

// `lerp(a, b, t)` shapes safe to expand AND re-fuse: float family with scalar `t` only — the
// one shape every provider has (das math AND the scalar-`t` backend DSL stubs). A vector-`t`
// re-fusion of raw `(b-a)*tv + a` arithmetic would emit a call a DSL-only module can't resolve.
def private lerp_shape(ta, tb, tt : Type) : bool {
    return ta == tb && is_float_family(ta) && tt == Type.tFloat
}

// True if `e` is built only from const literals and pure ops/builtins — no var/global read,
// no user call. eval_single_expression runs in a bare context (no globals, no user functions),
// so the tree must bottom out in literals and pure builtins. Unknown node → false.
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
// float4-based evaluator covers the float/int/uint scalar + vector family. Gates the cycle-1
// const-builtin allowance so only foldable-type calls bypass the whitelist.
def public eval_to_const_supported(t : TypeDeclPtr) : bool {
    if (t == null) return false
    let bt = t.baseType
    return (bt == Type.tFloat || bt == Type.tFloat2 || bt == Type.tFloat3 || bt == Type.tFloat4 ||
        bt == Type.tInt || bt == Type.tInt2 || bt == Type.tInt3 || bt == Type.tInt4 ||
        bt == Type.tUInt || bt == Type.tUInt2 || bt == Type.tUInt3 || bt == Type.tUInt4)
}

// True if every operator/call node in the subtree carries a settled type. eval_single_expression
// assumes inference is complete (an un-inferred operator has null `.func` → crash), so this guards
// eval against a not-yet-re-inferred subtree (a fold-built operand under an inferred all-const root).
def private fully_inferred(e : ExpressionPtr) : bool {
    if (e == null) return false
    if (e is ExprOp1) {
        return e._type != null && fully_inferred((e as ExprOp1).subexpr)
    }
    if (e is ExprOp2) {
        let o = e as ExprOp2
        return e._type != null && fully_inferred(o.left) && fully_inferred(o.right)
    }
    if (e is ExprOp3) {
        let o = e as ExprOp3
        return e._type != null && fully_inferred(o.subexpr) && fully_inferred(o.left) && fully_inferred(o.right)
    }
    if (e is ExprCall) {
        if (e._type == null) {
            return false
        }
        for (a in (e as ExprCall).arguments) {
            if (!fully_inferred(a)) {
                return false
            }
        }
        return true
    }
    return true
}

// Evaluate a literal-only (is_all_const) inferred expression to one const literal of
// its type, via the exact runtime evaluator. Returns `e` unchanged if eval fails or
// the type is unsupported. The fresh const is un-inferred; the macro re-infers it.
def private eval_to_const(var e : ExpressionPtr) : ExpressionPtr {
    if (e == null || e._type == null || !fully_inferred(e)) return e
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
        if (bt == Type.tUInt) return new ExprConstUInt(at = e.at, value = reinterpret<uint>(val.x))
        if (bt == Type.tUInt2) return new ExprConstUInt2(at = e.at, value = uint2(reinterpret<uint>(val.x), reinterpret<uint>(val.y)))
        if (bt == Type.tUInt3) return new ExprConstUInt3(at = e.at, value = uint3(reinterpret<uint>(val.x), reinterpret<uint>(val.y), reinterpret<uint>(val.z)))
        if (bt == Type.tUInt4) return new ExprConstUInt4(at = e.at, value = uint4(reinterpret<uint>(val.x), reinterpret<uint>(val.y), reinterpret<uint>(val.z), reinterpret<uint>(val.w)))
    }
    return e
}

// ===== Generalized const-propagation of single-def user locals =====
// Post-SSA-rename every user local is single-def, so a `var x = <const>` substitutes at all uses and
// drops. One forward pass folds a whole accumulator chain; masks are copy_prop's job, globals never.

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
        // ?[] yields a const view; const-strip the raw pointer (apply_template clones per use anyway).
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
        // Substitute already-known constants into this statement's reads.
        if (!empty(constmap) && reads_any(blk.list[i], constmap)) {
            blk.list[i] = subst_consts(blk.list[i], constmap)
            changed = true
        }
        // A single-def user local with a now-literal init folds to a constant and joins the map.
        var s = blk.list[i]
        if (s is ExprLet && length((s as ExprLet).variables) == 1) {
            var lc = s as ExprLet
            if (!is_flat_temp(lc.variables[0].name) && lc.variables[0].init != null && is_all_const(lc.variables[0].init)) {
                var oldInit = lc.variables[0].init
                if (oldInit |> is_expr_const()) {
                    // Already a literal (e.g. a fold-produced `0f`): map it directly so its uses inline.
                    constmap["{lc.variables[0].name}"] = oldInit
                    changed = true
                } else {
                    // A const expression (`2.0 * 3.0`): eval to its literal, folding the init in place.
                    var folded = eval_to_const(oldInit)
                    if (folded |> is_expr_const() && folded != oldInit) {
                        lc.variables[0].init = folded
                        constmap["{lc.variables[0].name}"] = folded
                        changed = true
                    }
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

// ===== Constant reassociation =====
// Gather scattered consts in `+`/`-`/`*` chains into one adjacent group (`0.5 + a + 0.6` → `a +
// (0.5+0.6)`) for the all-const arm to fold; also sort var terms into a canonical order (for a later CSE).

struct private ReTerm {
    neg : bool          // subtracted (additive chains); always false for the `*` monoid
    e : ExpressionPtr
}

// Flatten a maximal additive chain into signed terms (left-to-right = source order). Descends
// `+`/`-` ExprOp2 and unary `-`; everything else is a leaf. No same-base-type gate (vector±scalar
// does not compile, so an additive chain is always same-type).
def private collect_add(var e : ExpressionPtr; neg : bool; var terms : array<ReTerm>) {
    if (e == null) return
    if (e is ExprOp2) {
        var o = e as ExprOp2
        if (o.op == "+" || o.op == "-") {
            collect_add(o.left, neg, terms)
            collect_add(o.right, o.op == "-" ? !neg : neg, terms)
            return
        }
    } elif (e is ExprOp1) {
        var o = e as ExprOp1
        if (o.op == "-") {
            collect_add(o.subexpr, !neg, terms)
            return
        }
    }
    terms |> push(ReTerm(neg = neg, e = e))
}

// Flatten a maximal `*` chain into factors (no `/` in v1). Descends `*` ExprOp2 only.
def private collect_mul(var e : ExpressionPtr; var terms : array<ReTerm>) {
    if (e == null) return
    if (e is ExprOp2) {
        var o = e as ExprOp2
        if (o.op == "*") {
            collect_mul(o.left, terms)
            collect_mul(o.right, terms)
            return
        }
    }
    terms |> push(ReTerm(neg = false, e = e))
}

// True if the chain rooted at `e` (walked over `addOp`'s monoid: `+`/`-` for "+", `*` for "*")
// has any const leaf. Backs the already-grouped check.
def private chain_has_const(e : ExpressionPtr; addOp : string) : bool {
    if (e == null) return false
    if (e is ExprOp2) {
        let o = e as ExprOp2
        let isChainOp = addOp == "+" ? (o.op == "+" || o.op == "-") : (o.op == addOp)
        if (isChainOp) {
            return chain_has_const(o.left, addOp) || chain_has_const(o.right, addOp)
        }
    } elif (e is ExprOp1) {
        let o = e as ExprOp1
        if (addOp == "+" && o.op == "-") {
            return chain_has_const(o.subexpr, addOp)
        }
    }
    return is_all_const(e)
}

// The canonical reassoc output is `<vars> addOp <all-const-group>` with no const in the var
// part. Recognizing it makes reassoc idempotent — a fired chain lands here and never re-fires,
// so the macro's fixpoint terminates. (The const group is folded separately by FoldVisitor.)
def private already_grouped(that : ExprOp2?; addOp : string) : bool {
    return that.op == addOp && is_all_const(that.right) && !chain_has_const(that.left, addOp)
}

// Build a left-leaning `<t0> op <t1> …` tree from non-empty `terms`, REUSING the operand nodes
// (each leaf appears once, so it lands in one slot; the old chain wrappers drop when the visitor
// replaces `that`). A negative first term becomes unary `-`; later terms pick addOp/subOp by sign.
def private build_chain(var terms : array<ReTerm>; at : LineInfo; addOp, subOp : string) : ExpressionPtr {
    var acc = terms[0].neg ? negate(terms[0].e) : terms[0].e
    for (i in range(1, length(terms))) {
        acc = new ExprOp2(at = at, op := terms[i].neg ? subOp : addOp, left = acc, right = terms[i].e)
    }
    return acc
}

// True if the var terms are already canonical: positives first (so build_chain emits `b - a`,
// never `-a + b` = neg + add, whose uniform `-a` would hoist into a `_preshader_ = -_preshader_`
// alias), describe-ascending per sign group. Mirrors the regroup sort exactly (idempotence).
def private vars_sorted(vars : array<ReTerm>) : bool {
    for (i in range(1, length(vars))) {
        if ((vars[i - 1].neg && !vars[i].neg) ||
            (vars[i - 1].neg == vars[i].neg && describe(vars[i].e) < describe(vars[i - 1].e))) {
            return false
        }
    }
    return true
}

// Partition `terms` into var/const and rebuild as `<vars> addOp <consts>`. Two canonicalizations:
// gather ≥2 scattered consts into one group, and sort the var terms into a canonical order (for a
// later CSE) — the sort gated on side-effect freedom. Null when neither applies / the chain is all-const.
def private regroup(that : ExprOp2?; var terms : array<ReTerm>; addOp, subOp : string) : ExpressionPtr {
    var vars : array<ReTerm>
    var consts : array<ReTerm>
    for (t in terms) {
        if (is_all_const(t.e)) {
            consts |> push(t)
        } else {
            vars |> push(t)
        }
    }
    if (empty(vars)) {
        return null
    }
    var pure = true
    for (t in vars) {
        if (has_sideeffects(t.e)) {
            pure = false
            break
        }
    }
    let needs_gather = length(consts) >= 2 && !already_grouped(that, addOp)
    let needs_sort = pure && length(vars) >= 2 && !vars_sorted(vars)
    if (!needs_gather && !needs_sort) {
        return null
    }
    if (pure) {
        sort(vars) $(a, b : ReTerm) : bool {
            if (a.neg != b.neg) {
                return !a.neg
            }
            return describe(a.e) < describe(b.e)
        }
    }
    var vt = build_chain(vars, that.at, addOp, subOp)
    if (empty(consts)) {
        return vt
    }
    var ct = build_chain(consts, that.at, addOp, subOp)
    return new ExprOp2(at = that.at, op := addOp, left = vt, right = ct)
}

// Count the var / const terms in a chain (walked over `addOp`'s monoid) without building a term
// list — const-friendly, since the residual oracle only needs the tallies.
def private count_terms(e : ExpressionPtr; addOp : string; var nv : int&; var nc : int&) {
    if (e == null) return
    if (e is ExprOp2) {
        let o = e as ExprOp2
        let isChainOp = addOp == "+" ? (o.op == "+" || o.op == "-") : (o.op == addOp)
        if (isChainOp) {
            count_terms(o.left, addOp, nv, nc)
            count_terms(o.right, addOp, nv, nc)
            return
        }
    } elif (e is ExprOp1) {
        let o = e as ExprOp1
        if (addOp == "+" && o.op == "-") {
            count_terms(o.subexpr, addOp, nv, nc)
            return
        }
    }
    if (is_all_const(e)) {
        nc ++
    } else {
        nv ++
    }
}

// True if reassoc WOULD gather this chain: ≥1 var term, ≥2 scattered consts, not already grouped.
// The fold-completeness oracle (flatten.das FoldResidualVisitor) mirrors the transform's exact gate
// through this — incl. `fast_math` (float chains stay scattered under `_flatten_no_fast_math`).
def public has_scattered_consts(that : ExprOp2?; no_fast_math : bool = false) : bool {
    if (that == null || (no_fast_math && is_float_valued(that))) return false
    var nv = 0
    var nc = 0
    if (that.op == "+" || that.op == "-") {
        if (already_grouped(that, "+")) return false
        count_terms(that, "+", nv, nc)
    } elif (that.op == "*") {
        if (already_grouped(that, "*")) return false
        count_terms(that, "*", nv, nc)
    } else {
        return false
    }
    return nv >= 1 && nc >= 2
}

class private ReassocVisitor : AstVisitor {
    changed : bool
    noFastMath : bool
    def override visitExprOp2(var that : ExprOp2?) : ExpressionPtr {
        // Both the const-gather and the canonical re-sort rebuild the chain — a float
        // sum/product picks up a different association order, so floats are fast-math only.
        if (noFastMath && is_float_valued(that)) {
            return that
        }
        var res : ExpressionPtr = null
        if (that.op == "*") {
            var terms : array<ReTerm>
            collect_mul(that, terms)
            res = regroup(that, terms, "*", "/")
        } elif (that.op == "+" || that.op == "-") {
            var terms : array<ReTerm>
            collect_add(that, false, terms)
            res = regroup(that, terms, "+", "-")
        }
        if (res != null) {
            changed = true
            return res
        }
        return that
    }
}

def public fold_body(var func : FunctionPtr; no_fast_math : bool = false) : bool {
    //! Post-infer fold over a flattened body: const-propagate single-def locals, reassociate
    //! scattered consts, collapse `const ? a : b`, fold identities/const ctors, drop pure self-assigns.
    //! SINGLE pass (caller re-infers and loops); `no_fast_math = true` skips the value-changing arms.
    if (!(func.body is ExprBlock)) return false
    let nfm = no_fast_math
    var changed = const_prop_locals(func.body as ExprBlock)
    // Reassociate BEFORE the fold — gather scattered consts into one adjacent group so the
    // FoldVisitor all-const arm (and the typer's re-infer) can collapse them. After const-prop,
    // which is what scatters consts (a substituted single-def local lands mid-chain).
    var rv = new ReassocVisitor()
    rv.noFastMath = nfm
    make_visitor(*rv) $(adapter) {
        visit(func.body, adapter)
    }
    if (rv.changed) {
        changed = true
    }
    unsafe {
        delete rv
    }
    var v = new FoldVisitor()
    v.noFastMath = nfm
    make_visitor(*v) $(adapter) {
        visit(func.body, adapter)
    }
    if (v.changed) {
        changed = true
    }
    unsafe {
        delete v
    }
    // Cleanup 1 — drop pure self-assigns (`acc = acc`) a collapsed false-select leaves
    // behind. has_sideeffects gates it (`g[k()] = g[k()]` keeps — the lvalue isn't free).
    // Erase in reverse so earlier indices stay valid.
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
        }
        si --
    }
    // Cleanup 2 — strip a redundant zero/default init (`var acc = 0` → `var acc : int`). DEFERRED
    // to a converged pass: a const-folded local must keep its init until const_prop_locals
    // propagates it into uses; nulling it earlier strands the use. Sets changed → caller re-infers.
    if (!changed) {
        si = length(blk.list) - 1
        while (si >= 0) {
            var s = blk.list[si]
            if (s is ExprLet) {
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
    }
    return changed
}

// ===== Preshader extraction + CSE =====
// Two pure-AST passes on the converged body: hoist maximal globals-only subtrees to top-of-body
// `_preshader_N` lets, then CSE-dedup repeated subtrees. See flatten.rst / the plan for the why.

def private psh_simple_name(fullname : string) : string {
    let t1 = find(fullname, 96, 0)
    if (t1 < 0) return fullname
    let t2 = find(fullname, 96, t1 + 1)
    let endp = t2 < 0 ? length(fullname) : t2
    return slice(fullname, t1 + 1, endp)
}

def private psh_reserved(name : das_string) : bool {
    return name |> starts_with("__flat_") || name |> starts_with("__ssa_") || name |> starts_with("_preshader_")
}

// Parse `name` as `prefix` + decimal digits, -1 when it isn't. The round-trip check rejects
// foreign suffixes — `_preshader_cse_3` against prefix `_preshader_` belongs to the cse counter.
def private name_counter_suffix(name : das_string; prefix : string) : int {
    if (!(name |> starts_with(prefix))) return -1
    let s = slice("{name}", length(prefix))
    if (empty(s)) return -1
    let v = to_int(s)
    return "{v}" == s ? v : -1
}

// True if the subtree carries a graph node (operator or call) — i.e. hoisting it saves per-pixel
// work. A bare var / const / member access alone is trivial (no node), so it is inlined as an alias
// rather than hoisted.
def private psh_has_compute(e : ExpressionPtr) : bool {
    if (e == null) return false
    if (e is ExprRef2Value) return psh_has_compute((e as ExprRef2Value).subexpr)
    if (e is ExprOp1 || e is ExprOp2 || e is ExprOp3 || e is ExprCall) return true
    if (e is ExprField) return psh_has_compute((e as ExprField).value)
    if (e is ExprSwizzle) return psh_has_compute((e as ExprSwizzle).value)
    if (e is ExprCast) return psh_has_compute((e as ExprCast).subexpr)
    if (e is ExprPtr2Ref) return psh_has_compute((e as ExprPtr2Ref).subexpr)
    if (e is ExprAt) {
        let o = e as ExprAt
        return psh_has_compute(o.subexpr) || psh_has_compute(o.index)
    }
    return false
}

struct private PEX {
    paramNames : table<string>
    localColor : table<string; bool>    // local name -> uniform (preshader-eligible)?
    mutableVars : table<string>         // reassigned names (incl. written globals) — never uniform
    top : array<ExpressionPtr>          // hoisted `_preshader_N` lets, in dependency order
    dedup : table<string; string>       // describe(init) -> preshader name
    rename : table<string; ExpressionPtr>   // old local name -> replacement (alias inline / preshader ref)
    counter : int
    changed : bool
    noFastMath : bool                       // zero-init false = fast-math ON (the default)
}

// `x / U` rcp-candidate: a varying division by a preshader-eligible scalar-float NON-const
// divisor — shared verbatim by the extraction rewrite and the opt residual oracle. A wholly-
// uniform division hoists intact (maximal arm); a const divisor belongs to the fold arm.
def private is_rcp_candidate(pe : PEX; barriers : table<string>; o : ExprOp2?) : bool {
    return (o.op == "/" && expr_bt(o.right) == Type.tFloat && is_float_valued(o) &&
        !(o.right |> is_expr_const()) && !pex_eligible(pe, barriers, o) &&
        pex_eligible(pe, barriers, o.right))
}

// Uniform = depends only on globals + literals, no barrier (sampler/noise) call. Leaf rule: a param
// read is varying; a local read inherits its colored value; anything else is a module global (uniform).
def private pex_eligible(pe : PEX; barriers : table<string>; e : ExpressionPtr) : bool {
    if (e == null) return false
    if (e |> is_expr_const()) return true
    if (e is ExprVar) {
        let nm = "{(e as ExprVar).name}"
        if (key_exists(pe.mutableVars, nm)) return false   // reassigned (e.g. a written global) → not stable
        if (key_exists(pe.localColor, nm)) return pe.localColor?[nm] ?? false
        return !key_exists(pe.paramNames, nm)   // a non-param, non-local read is a module global → uniform
    }
    if (e is ExprRef2Value) return pex_eligible(pe, barriers, (e as ExprRef2Value).subexpr)
    if (e is ExprField) return pex_eligible(pe, barriers, (e as ExprField).value)
    if (e is ExprSwizzle) return pex_eligible(pe, barriers, (e as ExprSwizzle).value)
    if (e is ExprCast) return pex_eligible(pe, barriers, (e as ExprCast).subexpr)
    if (e is ExprPtr2Ref) return pex_eligible(pe, barriers, (e as ExprPtr2Ref).subexpr)
    if (e is ExprAt) {
        let o = e as ExprAt
        return pex_eligible(pe, barriers, o.subexpr) && pex_eligible(pe, barriers, o.index)
    }
    if (e is ExprMakeTuple) {
        for (v in (e as ExprMakeTuple).values) {
            if (!pex_eligible(pe, barriers, v)) return false
        }
        return true
    }
    if (e is ExprOp1) return pex_eligible(pe, barriers, (e as ExprOp1).subexpr)
    if (e is ExprOp2) {
        let o = e as ExprOp2
        return pex_eligible(pe, barriers, o.left) && pex_eligible(pe, barriers, o.right)
    }
    if (e is ExprOp3) {
        let o = e as ExprOp3
        return pex_eligible(pe, barriers, o.subexpr) && pex_eligible(pe, barriers, o.left) && pex_eligible(pe, barriers, o.right)
    }
    if (e is ExprCall) {
        let c = e as ExprCall
        // Surviving calls are all whitelisted pure GPU math; only the caller's barriers (sampler/noise) bar a preshader.
        if (key_exists(barriers, psh_simple_name("{c.name}"))) return false
        for (a in c.arguments) {
            if (!pex_eligible(pe, barriers, a)) return false
        }
        return true
    }
    return false
}

// Hoist an eligible+nontrivial subtree into a `_preshader_N` let (deduped by describe), returning
// the let name. The fresh let is un-inferred; the macro re-infers it after the pass.
def private pex_hoist_name(var pe : PEX; var e : ExpressionPtr) : string {
    let key = describe(e)
    if (key_exists(pe.dedup, key)) return pe.dedup[key]
    let nm = "_preshader_{pe.counter}"
    pe.counter ++
    pe.dedup[key] = nm
    pe.localColor[nm] = true
    pe.top |> push <| qmacro_expr(${ let $i(nm) = $e(e); })
    return nm
}

// Top-down maximal-subtree extraction: the first eligible+nontrivial node going down is hoisted and
// replaced with a ref; otherwise recurse children. Handles the bounded post-flatten node set.
def private pex_extract(var pe : PEX; barriers : table<string>; var e : ExpressionPtr) : ExpressionPtr {
    if (e == null) return null
    if (psh_has_compute(e) && pex_eligible(pe, barriers, e)) {
        let nm = pex_hoist_name(pe, e)
        pe.changed = true
        return new ExprVar(at = e.at, name := nm)
    }
    if (e is ExprOp1) {
        var o = e as ExprOp1
        o.subexpr = pex_extract(pe, barriers, o.subexpr)
    } elif (e is ExprOp2) {
        var o = e as ExprOp2
        // `x / U` → `x * (1f/U)` — reciprocal hoisted per-draw, dedup'd by describe (fast-math)
        if (!pe.noFastMath && is_rcp_candidate(pe, barriers, o)) {
            var one = new ExprConstFloat(at = o.at, value = 1.0)
            var recip = new ExprOp2(at = o.at, op := "/", left = one, right = o.right)
            let nm = pex_hoist_name(pe, recip)
            pe.changed = true
            return new ExprOp2(at = o.at, op := "*",
                left = pex_extract(pe, barriers, o.left),
                right = new ExprVar(at = o.at, name := nm))
        }
        o.left = pex_extract(pe, barriers, o.left)
        o.right = pex_extract(pe, barriers, o.right)
    } elif (e is ExprOp3) {
        var o = e as ExprOp3
        o.subexpr = pex_extract(pe, barriers, o.subexpr)
        o.left = pex_extract(pe, barriers, o.left)
        o.right = pex_extract(pe, barriers, o.right)
    } elif (e is ExprCall) {
        var c = e as ExprCall
        for (i in range(length(c.arguments))) {
            c.arguments[i] = pex_extract(pe, barriers, c.arguments[i])
        }
    } elif (e is ExprField) {
        var o = e as ExprField
        o.value = pex_extract(pe, barriers, o.value)
    } elif (e is ExprSwizzle) {
        var o = e as ExprSwizzle
        o.value = pex_extract(pe, barriers, o.value)
    } elif (e is ExprRef2Value) {
        var o = e as ExprRef2Value
        o.subexpr = pex_extract(pe, barriers, o.subexpr)
    } elif (e is ExprCast) {
        var o = e as ExprCast
        o.subexpr = pex_extract(pe, barriers, o.subexpr)
    } elif (e is ExprPtr2Ref) {
        var o = e as ExprPtr2Ref
        o.subexpr = pex_extract(pe, barriers, o.subexpr)
    } elif (e is ExprAt) {
        var o = e as ExprAt
        o.subexpr = pex_extract(pe, barriers, o.subexpr)
        o.index = pex_extract(pe, barriers, o.index)
    } elif (e is ExprMakeTuple) {
        var o = e as ExprMakeTuple
        for (vi in range(length(o.values))) {
            o.values[vi] = pex_extract(pe, barriers, o.values[vi])
        }
    } elif (e is ExprMakeStruct) {
        var ms = e as ExprMakeStruct
        for (si in range(length(ms.structs))) {
            for (fld in *(ms.structs[si])) {
                fld.value = pex_extract(pe, barriers, fld.value)
            }
        }
    }
    return e
}

// `counter` is owned by the caller's optimize loop — extraction re-runs per round, and a fresh
// per-call numbering would collide a later round's `_preshader_0` with the surviving first one.
def private extract_preshader(var func : FunctionPtr; var blk : ExprBlock?; barriers : table<string>; var counter : int&; no_fast_math : bool) : bool {
    var pe = PEX(counter = counter, noFastMath = no_fast_math)
    for (a in func.arguments) {
        pe.paramNames |> insert("{a.name}")
    }
    pe.mutableVars <- collect_mutable(blk)
    // re-entry seeding: prefix lets pre-fill dedup + colour (a re-hoist of one's own init would self-collapse)
    var oldPrefix = 0
    for (s in blk.list) {
        if (s is ExprLet && length((s as ExprLet).variables) == 1 &&
                (s as ExprLet).variables[0].name |> starts_with("_preshader_")) {
            let lv = (s as ExprLet).variables[0]
            if (lv.init != null) {
                pe.dedup["{describe(lv.init)}"] = "{lv.name}"
            }
            pe.localColor["{lv.name}"] = true
            // existing names own their suffixes — a fresh caller's counter must not re-mint them
            let cnum = name_counter_suffix(lv.name, "_preshader_")
            if (cnum >= pe.counter) {
                pe.counter = cnum + 1
            }
            oldPrefix ++
        } else {
            break
        }
    }
    var body : array<ExpressionPtr>
    for (s0 in blk.list) {
        var s = s0
        if (!empty(pe.rename) && reads_any(s, pe.rename)) {
            s = subst_consts(s, pe.rename)
        }
        if (s is ExprLet && length((s as ExprLet).variables) == 1 &&
                (s as ExprLet).variables[0].name |> starts_with("_preshader_")) {
            body |> push(s)
            continue
        }
        if (s is ExprLet && length((s as ExprLet).variables) == 1 && !psh_reserved((s as ExprLet).variables[0].name)) {
            var lc = s as ExprLet
            let vname = "{lc.variables[0].name}"
            var init = lc.variables[0].init
            if (init != null && pex_eligible(pe, barriers, init)) {
                if (psh_has_compute(init)) {
                    // Whole uniform let: hoist its init, rename refs to the preshader temp, drop the decl.
                    let nm = pex_hoist_name(pe, init)
                    pe.rename[vname] = new ExprVar(at = init.at, name := nm)
                    pe.changed = true
                } else {
                    // Trivial uniform alias (a bare global / member read): inline it at every use.
                    pe.rename[vname] = clone_expression(init)
                    pe.changed = true
                }
                continue
            }
        }
        // Varying / reserved / multi-var statement: extract buried uniform subtrees, then emit.
        if (s is ExprLet) {
            var lc = s as ExprLet
            for (lv in lc.variables) {
                if (lv.init != null) {
                    lv.init = pex_extract(pe, barriers, lv.init)
                }
                pe.localColor["{lv.name}"] = false
            }
        } elif (s is ExprCopy) {
            var cp = s as ExprCopy
            cp.right = pex_extract(pe, barriers, cp.right)
        } elif (s is ExprReturn) {
            var rt = s as ExprReturn
            rt.subexpr = pex_extract(pe, barriers, rt.subexpr)
        }
        body |> push(s)
    }
    if (pe.changed) {
        // old prefix first, then fresh hoists (they may reference earlier `_preshader_` names), then the body
        while (!empty(blk.list)) {
            blk.list |> erase(length(blk.list) - 1)
        }
        for (i in range(oldPrefix)) {
            blk.list |> emplace(body[i])
        }
        for (t in pe.top) {
            blk.list |> emplace(t)
        }
        for (i in range(oldPrefix, length(body))) {
            blk.list |> emplace(body[i])
        }
    }
    let result = pe.changed
    counter = pe.counter
    delete pe.paramNames
    delete pe.localColor
    delete pe.mutableVars
    delete pe.dedup
    unsafe {
        delete pe.rename
        delete pe.top
        delete body
    }
    return result
}

// ===== CSE (local value numbering) =====
// The body is one basic block, so LVN is complete. Value-number pure subtrees by `describe()`, dedup
// any computed >=2x into one shared let before its first use; uniform dups route to the preshader.

// The base ExprVar of a store target, digging through index/field/swizzle/deref wrappers —
// `G[i] = v` makes G's reads value-unstable exactly like `G = v` does.
def private store_base(e : Expression const?) : ExpressionPtr {
    if (e == null) return null
    if (e is ExprAt) return store_base((e as ExprAt).subexpr)
    if (e is ExprField) return store_base((e as ExprField).value)
    if (e is ExprSwizzle) return store_base((e as ExprSwizzle).value)
    if (e is ExprPtr2Ref) return store_base((e as ExprPtr2Ref).subexpr)
    if (e is ExprRef2Value) return store_base((e as ExprRef2Value).subexpr)
    return e is ExprVar ? unsafe(reinterpret<Expression? -const>(e)) : null
}

// Names reassigned somewhere in the block (live/loop masks `__flat_*`, any reassigned user `var`).
// A subtree reading one of these is NOT value-stable — CSE must not share it (its value differs
// per occurrence), so it is excluded from value numbering. Single-def `__ssa_*` / `let` are safe.
class private MutCollect : AstVisitor {
    names : table<string>
    def override preVisitExprCopy(expr : ExprCopy?) : void {
        let base = store_base(expr.left)
        if (base != null) {
            names |> insert("{(base as ExprVar).name}")
        }
    }
}

def private collect_mutable(blk : ExprBlock?) : table<string> {
    var mc = new MutCollect()
    make_visitor(*mc) $(a) {
        visit(blk, a)
    }
    var r : table<string>
    for (n in keys(mc.names)) {
        r |> insert(n)
    }
    unsafe {
        delete mc
    }
    return <- r
}

// One structural walk (O(size)) over the closed post-flatten node set, replacing the old
// O(#mutableVars * size) per-name count_refs loop. An unknown node kind is conservatively treated
// as reading a mutable (never CSE through it) — correctness over completeness for this safety gate.
def private reads_mutable(e : ExpressionPtr; mutableVars : table<string>) : bool {
    if (e == null || empty(mutableVars) || e |> is_expr_const()) return false
    if (e is ExprVar) return key_exists(mutableVars, "{(e as ExprVar).name}")
    if (e is ExprRef2Value) return reads_mutable((e as ExprRef2Value).subexpr, mutableVars)
    if (e is ExprField) return reads_mutable((e as ExprField).value, mutableVars)
    if (e is ExprSwizzle) return reads_mutable((e as ExprSwizzle).value, mutableVars)
    if (e is ExprCast) return reads_mutable((e as ExprCast).subexpr, mutableVars)
    if (e is ExprPtr2Ref) return reads_mutable((e as ExprPtr2Ref).subexpr, mutableVars)
    if (e is ExprOp1) return reads_mutable((e as ExprOp1).subexpr, mutableVars)
    if (e is ExprOp2) {
        let o = e as ExprOp2
        return reads_mutable(o.left, mutableVars) || reads_mutable(o.right, mutableVars)
    }
    if (e is ExprOp3) {
        let o = e as ExprOp3
        return reads_mutable(o.subexpr, mutableVars) || reads_mutable(o.left, mutableVars) || reads_mutable(o.right, mutableVars)
    }
    if (e is ExprAt) {
        let o = e as ExprAt
        return reads_mutable(o.subexpr, mutableVars) || reads_mutable(o.index, mutableVars)
    }
    if (e is ExprCall) {
        for (a in (e as ExprCall).arguments) {
            if (reads_mutable(a, mutableVars)) return true
        }
        return false
    }
    if (e is ExprMakeTuple) {
        for (v in (e as ExprMakeTuple).values) {
            if (reads_mutable(v, mutableVars)) return true
        }
        return false
    }
    if (e is ExprMakeStruct) {
        var ms = e as ExprMakeStruct
        for (si in range(length(ms.structs))) {
            for (fld in *(ms.structs[si])) {
                if (reads_mutable(fld.value, mutableVars)) return true
            }
        }
        return false
    }
    return true
}

class private CseTally : AstVisitor {
    collectSample : bool        // clone a representative node per key (cse_body only; the oracle just counts)
    counts : table<string; int>
    sample : table<string; ExpressionPtr>
    mutableVars : table<string>
    def override preVisitExpression(expr : ExpressionPtr) : void {
        // ExprRef2Value is describe-transparent: tallying wrapper AND wrappee double-counts every
        // ref-rooted dup (an array/field read), so its count never drops below 2 after sharing and
        // the round loop spins. Tally the wrappee only.
        if (expr is ExprRef2Value) {
            return
        }
        if (psh_has_compute(expr) && !reads_mutable(expr, mutableVars)) {
            let k = "{describe(expr)}"
            counts[k] = (counts?[k] ?? 0) + 1
            if (collectSample && !key_exists(sample, k)) {
                sample[k] = clone_expression(expr)
            }
        }
    }
}

// Replace every subtree whose describe matches `key` with a ref to `refName`. Top-down: a match is
// replaced (no recursion into it); otherwise recurse children (the bounded post-flatten node set).
def private cse_replace_expr(var e : ExpressionPtr; key, refName : string) : ExpressionPtr {
    if (e == null) return null
    if (psh_has_compute(e) && "{describe(e)}" == key) {
        return new ExprVar(at = e.at, name := refName)
    }
    if (e is ExprOp1) {
        var o = e as ExprOp1
        o.subexpr = cse_replace_expr(o.subexpr, key, refName)
    } elif (e is ExprOp2) {
        var o = e as ExprOp2
        o.left = cse_replace_expr(o.left, key, refName)
        o.right = cse_replace_expr(o.right, key, refName)
    } elif (e is ExprOp3) {
        var o = e as ExprOp3
        o.subexpr = cse_replace_expr(o.subexpr, key, refName)
        o.left = cse_replace_expr(o.left, key, refName)
        o.right = cse_replace_expr(o.right, key, refName)
    } elif (e is ExprCall) {
        var c = e as ExprCall
        for (i in range(length(c.arguments))) {
            c.arguments[i] = cse_replace_expr(c.arguments[i], key, refName)
        }
    } elif (e is ExprField) {
        var o = e as ExprField
        o.value = cse_replace_expr(o.value, key, refName)
    } elif (e is ExprSwizzle) {
        var o = e as ExprSwizzle
        o.value = cse_replace_expr(o.value, key, refName)
    } elif (e is ExprRef2Value) {
        var o = e as ExprRef2Value
        o.subexpr = cse_replace_expr(o.subexpr, key, refName)
    } elif (e is ExprCast) {
        var o = e as ExprCast
        o.subexpr = cse_replace_expr(o.subexpr, key, refName)
    } elif (e is ExprPtr2Ref) {
        var o = e as ExprPtr2Ref
        o.subexpr = cse_replace_expr(o.subexpr, key, refName)
    } elif (e is ExprAt) {
        var o = e as ExprAt
        o.subexpr = cse_replace_expr(o.subexpr, key, refName)
        o.index = cse_replace_expr(o.index, key, refName)
    } elif (e is ExprMakeTuple) {
        var o = e as ExprMakeTuple
        for (vi in range(length(o.values))) {
            o.values[vi] = cse_replace_expr(o.values[vi], key, refName)
        }
    } elif (e is ExprMakeStruct) {
        var ms = e as ExprMakeStruct
        for (si in range(length(ms.structs))) {
            for (fld in *(ms.structs[si])) {
                fld.value = cse_replace_expr(fld.value, key, refName)
            }
        }
    }
    return e
}

def private cse_replace_stmt(var s : ExpressionPtr; key, refName : string) {
    if (s is ExprLet) {
        var lc = s as ExprLet
        for (lv in lc.variables) {
            if (lv.init != null) {
                lv.init = cse_replace_expr(lv.init, key, refName)
            }
        }
    } elif (s is ExprCopy) {
        var cp = s as ExprCopy
        cp.right = cse_replace_expr(cp.right, key, refName)
    } elif (s is ExprReturn) {
        var rt = s as ExprReturn
        rt.subexpr = cse_replace_expr(rt.subexpr, key, refName)
    }
}

// Tally EXACTLY the regions cse_replace_stmt rewrites (let inits, copy RHS, return expr) — NOT the
// whole statement. A dup counted in an unrewritable position (a store's lvalue) can never drop
// below 2, so the round loop would re-pick the same key and spin to the guard.
def private cse_tally_stmt(s : ExpressionPtr; a) {
    if (s is ExprLet) {
        for (lv in (s as ExprLet).variables) {
            if (lv.init != null) {
                visit(lv.init, a)
            }
        }
    } elif (s is ExprCopy) {
        visit((s as ExprCopy).right, a)
    } elif (s is ExprReturn) {
        let rt = s as ExprReturn
        if (rt.subexpr != null) {
            visit(rt.subexpr, a)
        }
    }
}

// Count leading contiguous `_preshader_*` lets — the preshader region. A CSE dup whose first use
// falls inside it is uniform (Pass A already pulled every nontrivial uniform subtree out of the
// body), so it routes to the preshader.
def private preshader_prefix_len(blk : ExprBlock?) : int {
    var k = 0
    for (s in blk.list) {
        if (s is ExprLet && length((s as ExprLet).variables) == 1 &&
                (s as ExprLet).variables[0].name |> starts_with("_preshader_")) {
            k ++
        } else {
            break
        }
    }
    return k
}

// `counter` is owned by the caller's cse/alias loop — fresh per-call numbering would collide a
// second call's `_cse_0` with a surviving first-call `_cse_0` (error[30704] shadowing).
def private cse_body(var blk : ExprBlock?; var counter : int&) : bool {
    var anyChange = false
    var guard = 0
    // existing share names own their suffixes — never re-mint them (shares splice mid-body, so scan all)
    for (s in blk.list) {
        if (s is ExprLet && length((s as ExprLet).variables) == 1) {
            var cnum = name_counter_suffix((s as ExprLet).variables[0].name, "_preshader_cse_")
            if (cnum < 0) {
                cnum = name_counter_suffix((s as ExprLet).variables[0].name, "_cse_")
            }
            if (cnum >= counter) {
                counter = cnum + 1
            }
        }
    }
    // Reassigned vars are stable across rounds (cse only ever adds `let`s), so collect once.
    var mutableVars <- collect_mutable(blk)
    while (guard < 4096) {
        guard ++
        // Tally every nontrivial subtree across the block, recording its first statement index.
        var counts : table<string; int>
        var sample : table<string; ExpressionPtr>
        var firstStmt : table<string; int>
        for (i in range(length(blk.list))) {
            var ct = new CseTally()
            ct.collectSample = true
            ct.mutableVars := mutableVars
            make_visitor(*ct) $(a) {
                cse_tally_stmt(blk.list[i], a)
            }
            for (k in keys(ct.counts)) {
                counts[k] = (counts?[k] ?? 0) + (ct.counts?[k] ?? 0)
                if (!key_exists(firstStmt, k)) {
                    firstStmt[k] = i
                    sample[k] = ct.sample?[k] ?? null
                }
            }
            unsafe {
                delete ct
            }
        }
        // Pick the largest subtree computed >=2x (largest → fewer rounds; nested dups surface next).
        var bestKey = ""
        var bestLen = 0
        for (k in keys(counts)) {
            if ((counts?[k] ?? 0) >= 2 && length(k) > bestLen) {
                bestKey = k
                bestLen = length(k)
            }
        }
        if (bestKey == "") {
            delete counts
            delete firstStmt
            unsafe {
                delete sample
            }
            break
        }
        let fi = firstStmt?[bestKey] ?? 0
        let uniform = fi < preshader_prefix_len(blk)
        let nm = uniform ? "_preshader_cse_{counter}" : "_cse_{counter}"
        counter ++
        // `sample` holds a tally-time (pre-replace) clone; cse_replace mutates the tree, not it.
        var src = sample?[bestKey] ?? null
        for (s in blk.list) {
            cse_replace_stmt(s, bestKey, nm)
        }
        var theLet = qmacro_expr(${ let $i(nm) = $e(src); })
        var newlist : array<ExpressionPtr>
        newlist |> reserve(length(blk.list) + 1)
        for (i in range(length(blk.list))) {
            if (i == fi) {
                newlist |> push(theLet)
            }
            newlist |> push(blk.list[i])
        }
        while (!empty(blk.list)) {
            blk.list |> erase(length(blk.list) - 1)
        }
        for (m in newlist) {
            blk.list |> emplace(m)
        }
        delete counts
        delete firstStmt
        unsafe {
            delete sample
            delete newlist
        }
        anyChange = true
    }
    delete mutableVars
    return anyChange
}

// ===== Pure-alias elimination =====
// Collapse `let X = <bare var V>` copies (V not reassigned) into direct refs to V and drop the let —
// clears CSE's `_cse_a = _cse_b` leftovers and unroll/fold's `p_0 = ro`. General, not CSE-specific.

// The bare ExprVar under any chain of ExprRef2Value wrappers, else null.
def private alias_source(e : Expression const?) : ExpressionPtr {
    if (e == null) return null
    if (e is ExprRef2Value) return alias_source((e as ExprRef2Value).subexpr)
    return e is ExprVar ? unsafe(reinterpret<Expression? -const>(e)) : null
}

def private eliminate_aliases(var blk : ExprBlock?) : bool {
    var mutableVars <- collect_mutable(blk)
    var rename : table<string; ExpressionPtr>
    var body : array<ExpressionPtr>
    var changed = false
    for (s0 in blk.list) {
        // Resolve already-collapsed aliases first, so chains (let a = b; let b = c) fold in one pass.
        var s = !empty(rename) && reads_any(s0, rename) ? subst_consts(s0, rename) : s0
        if (s is ExprLet && length((s as ExprLet).variables) == 1) {
            let lvName = "{(s as ExprLet).variables[0].name}"
            let src = alias_source((s as ExprLet).variables[0].init)
            // Skip if either the alias var X or the source V is reassigned — renaming a reassigned X rewrites its stores onto the const source.
            if (src != null && !key_exists(mutableVars, lvName) &&
                    !key_exists(mutableVars, "{(src as ExprVar).name}")) {
                rename[lvName] = clone_expression(src)
                changed = true
                continue
            }
        }
        body |> push(s)
    }
    if (changed) {
        while (!empty(blk.list)) {
            blk.list |> erase(length(blk.list) - 1)
        }
        for (b in body) {
            blk.list |> emplace(b)
        }
    }
    delete mutableVars
    unsafe {
        delete rename
        delete body
    }
    return changed
}

// ===== Completeness oracle (test-framework facing) =====
// Mirrors flatten.das FoldResidualVisitor: flags a uniform subtree still inline in the varying body
// or a pure subtree computed >=2x un-shared. Empty ⇒ complete; checked over the COMPILED twin.

// True if the subtree hides a maximal eligible-nontrivial node (one preshader extraction missed).
def private pex_residual_expr(pe : PEX; barriers : table<string>; e : ExpressionPtr) : bool {
    if (e == null) return false
    if (psh_has_compute(e) && pex_eligible(pe, barriers, e)) return true
    if (e is ExprOp1) return pex_residual_expr(pe, barriers, (e as ExprOp1).subexpr)
    if (e is ExprOp2) {
        let o = e as ExprOp2
        return pex_residual_expr(pe, barriers, o.left) || pex_residual_expr(pe, barriers, o.right)
    }
    if (e is ExprOp3) {
        let o = e as ExprOp3
        return pex_residual_expr(pe, barriers, o.subexpr) || pex_residual_expr(pe, barriers, o.left) || pex_residual_expr(pe, barriers, o.right)
    }
    if (e is ExprCall) {
        for (a in (e as ExprCall).arguments) {
            if (pex_residual_expr(pe, barriers, a)) return true
        }
        return false
    }
    if (e is ExprField) return pex_residual_expr(pe, barriers, (e as ExprField).value)
    if (e is ExprSwizzle) return pex_residual_expr(pe, barriers, (e as ExprSwizzle).value)
    if (e is ExprRef2Value) return pex_residual_expr(pe, barriers, (e as ExprRef2Value).subexpr)
    if (e is ExprCast) return pex_residual_expr(pe, barriers, (e as ExprCast).subexpr)
    if (e is ExprPtr2Ref) return pex_residual_expr(pe, barriers, (e as ExprPtr2Ref).subexpr)
    if (e is ExprAt) {
        let o = e as ExprAt
        return pex_residual_expr(pe, barriers, o.subexpr) || pex_residual_expr(pe, barriers, o.index)
    }
    if (e is ExprMakeTuple) {
        for (v in (e as ExprMakeTuple).values) {
            if (pex_residual_expr(pe, barriers, v)) return true
        }
        return false
    }
    if (e is ExprMakeStruct) {
        var ms = e as ExprMakeStruct
        for (si in range(length(ms.structs))) {
            for (fld in *(ms.structs[si])) {
                if (pex_residual_expr(pe, barriers, fld.value)) return true
            }
        }
    }
    return false
}

// Oracle mirror of the extraction rcp rewrite — any descendant division `is_rcp_candidate`
// would have become `* _preshader(1/U)`. Same closed node set as pex_residual_expr.
def private rcp_residual_expr(pe : PEX; barriers : table<string>; e : ExpressionPtr) : bool {
    if (e == null) return false
    if (e is ExprOp1) return rcp_residual_expr(pe, barriers, (e as ExprOp1).subexpr)
    if (e is ExprOp2) {
        let o = e as ExprOp2
        if (is_rcp_candidate(pe, barriers, o)) return true
        return rcp_residual_expr(pe, barriers, o.left) || rcp_residual_expr(pe, barriers, o.right)
    }
    if (e is ExprOp3) {
        let o = e as ExprOp3
        return rcp_residual_expr(pe, barriers, o.subexpr) || rcp_residual_expr(pe, barriers, o.left) || rcp_residual_expr(pe, barriers, o.right)
    }
    if (e is ExprCall) {
        for (a in (e as ExprCall).arguments) {
            if (rcp_residual_expr(pe, barriers, a)) return true
        }
        return false
    }
    if (e is ExprField) return rcp_residual_expr(pe, barriers, (e as ExprField).value)
    if (e is ExprSwizzle) return rcp_residual_expr(pe, barriers, (e as ExprSwizzle).value)
    if (e is ExprRef2Value) return rcp_residual_expr(pe, barriers, (e as ExprRef2Value).subexpr)
    if (e is ExprCast) return rcp_residual_expr(pe, barriers, (e as ExprCast).subexpr)
    if (e is ExprPtr2Ref) return rcp_residual_expr(pe, barriers, (e as ExprPtr2Ref).subexpr)
    if (e is ExprAt) {
        let o = e as ExprAt
        return rcp_residual_expr(pe, barriers, o.subexpr) || rcp_residual_expr(pe, barriers, o.index)
    }
    if (e is ExprMakeTuple) {
        for (v in (e as ExprMakeTuple).values) {
            if (rcp_residual_expr(pe, barriers, v)) return true
        }
        return false
    }
    if (e is ExprMakeStruct) {
        var ms = e as ExprMakeStruct
        for (si in range(length(ms.structs))) {
            for (fld in *(ms.structs[si])) {
                if (rcp_residual_expr(pe, barriers, fld.value)) return true
            }
        }
    }
    return false
}

def public opt_residuals(var func : FunctionPtr; barriers : table<string>; no_fast_math : bool = false) : array<string> {
    var res : array<string>
    if (func == null || func.body == null || !(func.body is ExprBlock)) return <- res
    var blk = func.body as ExprBlock
    // Post-optimize coloring: a `_preshader_*` let is uniform, every other local is varying.
    var pe : PEX
    for (a in func.arguments) {
        pe.paramNames |> insert("{a.name}")
    }
    pe.mutableVars <- collect_mutable(blk)
    for (s in blk.list) {
        if (s is ExprLet) {
            for (lv in (s as ExprLet).variables) {
                pe.localColor["{lv.name}"] = lv.name |> starts_with("_preshader_")
            }
        }
    }
    // Preshader residual: a uniform subtree still inline in the varying region (index >= prefix).
    let prefix = preshader_prefix_len(blk)
    for (i in range(length(blk.list))) {
        if (i < prefix) {
            continue
        }
        let s = blk.list[i]
        var hit = false
        if (s is ExprLet) {
            for (lv in (s as ExprLet).variables) {
                if (lv.init != null && pex_residual_expr(pe, barriers, lv.init)) {
                    hit = true
                }
            }
        } elif (s is ExprCopy) {
            hit = pex_residual_expr(pe, barriers, (s as ExprCopy).right)
        } elif (s is ExprReturn) {
            hit = pex_residual_expr(pe, barriers, (s as ExprReturn).subexpr)
        }
        if (hit) {
            res |> push("an un-extracted uniform subtree (preshader miss)")
            break
        }
    }
    // rcp residual: a varying division by a preshader-eligible scalar still inline (fast-math only).
    if (!no_fast_math) {
        for (i in range(length(blk.list))) {
            if (i < prefix) {
                continue
            }
            let s = blk.list[i]
            var hit = false
            if (s is ExprLet) {
                for (lv in (s as ExprLet).variables) {
                    if (lv.init != null && rcp_residual_expr(pe, barriers, lv.init)) {
                        hit = true
                    }
                }
            } elif (s is ExprCopy) {
                hit = rcp_residual_expr(pe, barriers, (s as ExprCopy).right)
            } elif (s is ExprReturn) {
                hit = rcp_residual_expr(pe, barriers, (s as ExprReturn).subexpr)
            }
            if (hit) {
                res |> push("an un-rewritten division by a uniform ('x / U' should be 'x * _preshader(1/U)')")
                break
            }
        }
    }
    // CSE residual: any pure subtree (no mutable read) still computed >=2x — tallying the same
    // rewritable regions cse_body does, so an unrewritable lvalue dup is not a false positive.
    var counts : table<string; int>
    for (i in range(length(blk.list))) {
        var ct = new CseTally()
        ct.mutableVars := pe.mutableVars
        make_visitor(*ct) $(a) {
            cse_tally_stmt(blk.list[i], a)
        }
        for (k in keys(ct.counts)) {
            counts[k] = (counts?[k] ?? 0) + (ct.counts?[k] ?? 0)
        }
        unsafe {
            delete ct
        }
    }
    for (k in keys(counts)) {
        if ((counts?[k] ?? 0) >= 2) {
            res |> push("a repeated subtree '{k}' (>=2x) not CSE'd")
            break
        }
    }
    // Alias residual: a pure single-def `let X = <bare var V>` copy (neither X nor V reassigned)
    // the cleanup should collapse. A reassigned X (`var X = V` reused across copies) is a real var.
    for (s in blk.list) {
        if (s is ExprLet && length((s as ExprLet).variables) == 1) {
            let lvName = "{(s as ExprLet).variables[0].name}"
            let src = alias_source((s as ExprLet).variables[0].init)
            if (src != null && !key_exists(pe.mutableVars, lvName) &&
                    !key_exists(pe.mutableVars, "{(src as ExprVar).name}")) {
                res |> push("a pure alias let '{lvName}' = '{(src as ExprVar).name}'")
                break
            }
        }
    }
    // Fusion residual: a fusable mul-add (`a*b ± c`) PHASE_FUSED should have re-packed into
    // `mad`, or a mad still carrying the lerp shape — gated on the same module visibility.
    if (fuse_callable(func._module, "mad")) {
        var fr = new FuseResidualVisitor()
        fr.checkLerp = fuse_callable(func._module, "lerp")
        make_visitor(*fr) $(a) {
            visit(func.body, a)
        }
        if (fr.foundMad) {
            res |> push("an un-fused mul-add ('a*b ± c' should be mad)")
        }
        if (fr.foundLerp) {
            res |> push("an un-refused lerp shape (mad(b-a,t,a))")
        }
        if (fr.foundNegAdd) {
            res |> push("an un-packed neg-add ('-a + b' should be 'b - a')")
        }
        unsafe {
            delete fr
        }
    }
    delete pe.paramNames
    delete pe.localColor
    delete pe.mutableVars
    delete counts
    return <- res
}

def public flatten_preshade_cse(var func : FunctionPtr; barriers : table<string>; no_fast_math : bool = false) : bool {
    //! Post-fold optimize pass: hoist uniform (globals+literals) subtrees to top-of-body
    //! `_preshader_N` lets, CSE-dedup repeats, collapse pure aliases — a joint fixpoint.
    //! `barriers` = sampler/noise names that stay per-pixel. Post-fold, once; caller re-infers.
    if (!(func.body is ExprBlock)) return false
    var blk = func.body as ExprBlock
    // The passes enable each other — an alias subst / CSE share can re-expose uniform compute in
    // the varying region (`!_preshader_0`) that only re-extraction hoists, so iterate the triple
    // to a fixpoint. An exhausted guard means an oscillation regression — warn, never hang.
    var changed = false
    var guard = 0
    var cseCounter = 0
    var preCounter = 0
    while (guard < 16) {
        guard ++
        var round = false
        if (extract_preshader(func, blk, barriers, preCounter, no_fast_math)) {
            round = true
        }
        if (cse_body(blk, cseCounter)) {
            round = true
        }
        if (eliminate_aliases(blk)) {
            round = true
        }
        if (!round) {
            break
        }
        changed = true
    }
    if (guard >= 16) {
        to_log(LOG_WARNING, "flatten: cse/alias loop guard exhausted in {func.name} (oscillation?)\n")
    }
    return changed
}

// ===== mad/lerp re-fusion (PHASE_FUSED finishing pass) =====
// The fold DISASSEMBLES lerp/mad into raw arithmetic (identities fold, reassoc/CSE see through);
// this re-packs survivors: `a*b ± c` → mad, then `mad(b - a, t, a)` → lerp — one node each.

// A reciprocal copy of a scalar/vector float const literal (`C` → `1/C`); null for any other
// node or when any lane is zero (no inf literals). Backs the `x / C → x * (1/C)` fold arm.
def private recip_const(e : ExpressionPtr) : ExpressionPtr {
    if (e == null) return null
    if (e is ExprConstFloat) {
        let v = (e as ExprConstFloat).value
        if (v == 0.0) return null
        return new ExprConstFloat(at = e.at, value = 1.0 / v)
    }
    if (e is ExprConstDouble) {
        let v = (e as ExprConstDouble).value
        if (v == 0.0lf) return null
        return new ExprConstDouble(at = e.at, value = 1.0lf / v)
    }
    if (e is ExprConstFloat2) {
        let v = (e as ExprConstFloat2).value
        if (v.x == 0.0 || v.y == 0.0) return null
        return new ExprConstFloat2(at = e.at, value = float2(1.0 / v.x, 1.0 / v.y))
    }
    if (e is ExprConstFloat3) {
        let v = (e as ExprConstFloat3).value
        if (v.x == 0.0 || v.y == 0.0 || v.z == 0.0) return null
        return new ExprConstFloat3(at = e.at, value = float3(1.0 / v.x, 1.0 / v.y, 1.0 / v.z))
    }
    if (e is ExprConstFloat4) {
        let v = (e as ExprConstFloat4).value
        if (v.x == 0.0 || v.y == 0.0 || v.z == 0.0 || v.w == 0.0) return null
        return new ExprConstFloat4(at = e.at, value = float4(1.0 / v.x, 1.0 / v.y, 1.0 / v.z, 1.0 / v.w))
    }
    return null
}

// Oracle mirror for the `x / C → x * (1/C)` fold arm: true when the (fast-math) rewrite would fire.
def public is_unrcp_const_div(e : ExprOp2?) : bool {
    if (e == null || e.op != "/" || !is_float_valued(e)) return false
    return recip_const(e.right) != null
}

// A negated copy of a scalar/vector float/int const literal; null for any other node
// (uint excluded — modular negation is value-correct but backend-hostile).
def private negate_const(e : ExpressionPtr) : ExpressionPtr {
    if (e == null) return null
    if (e is ExprConstFloat) return new ExprConstFloat(at = e.at, value = -(e as ExprConstFloat).value)
    if (e is ExprConstDouble) return new ExprConstDouble(at = e.at, value = -(e as ExprConstDouble).value)
    if (e is ExprConstInt) return new ExprConstInt(at = e.at, value = -(e as ExprConstInt).value)
    if (e is ExprConstFloat2) return new ExprConstFloat2(at = e.at, value = -(e as ExprConstFloat2).value)
    if (e is ExprConstFloat3) return new ExprConstFloat3(at = e.at, value = -(e as ExprConstFloat3).value)
    if (e is ExprConstFloat4) return new ExprConstFloat4(at = e.at, value = -(e as ExprConstFloat4).value)
    if (e is ExprConstInt2) return new ExprConstInt2(at = e.at, value = -(e as ExprConstInt2).value)
    if (e is ExprConstInt3) return new ExprConstInt3(at = e.at, value = -(e as ExprConstInt3).value)
    if (e is ExprConstInt4) return new ExprConstInt4(at = e.at, value = -(e as ExprConstInt4).value)
    return null
}

def private has_3arg_named(mod : Module?; fname : string) : bool {
    var found = false
    for_each_function(mod, fname) $(fn) {
        if (length(fn.arguments) == 3) {
            found = true
        }
    }
    for_each_generic(mod, fname) $(fn) {
        if (length(fn.arguments) == 3) {
            found = true
        }
    }
    return found
}

def private fuse_callable(mod : Module?; fname : string) : bool {
    if (mod == null) return false
    if (has_3arg_named(mod, fname)) return true
    var found = false
    module_for_each_dependency(mod) $(dep, _isPub) {
        if (!found && has_3arg_named(dep, fname)) {
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
    var mul : ExprOp2?
    var other : ExpressionPtr
    var negOther = false        // `m - C`  → c = -C
    var negFactor = false       // `C - m`  → negate a const factor of m
    if (that.op == "+" || that.op == "-") {
        if (that.left is ExprOp2 && (that.left as ExprOp2).op == "*") {
            mul = that.left as ExprOp2
            other = that.right
            negOther = that.op == "-"
        } elif (that.right is ExprOp2 && (that.right as ExprOp2).op == "*") {
            mul = that.right as ExprOp2
            other = that.left
            negFactor = that.op == "-"
        }
    }
    if (mul == null) return false
    var fa = mul.left
    var fb = mul.right
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
    if (that == null || that.op != "+" || !is_mad_elem(expr_bt(that))) return false
    if (that.left is ExprOp1 && (that.left as ExprOp1).op == "-") {
        pos = that.right
        neg = (that.left as ExprOp1).subexpr
        return true
    }
    if (that.right is ExprOp1 && (that.right as ExprOp1).op == "-") {
        pos = that.left
        neg = (that.right as ExprOp1).subexpr
        return true
    }
    return false
}

// `mad(b - a, t, a)` → `lerp(a, b, t)`: the re-packed expansion shape, `a` matched by
// describe-equality. The neg-add form is matched too (defense — reassoc no longer emits it).
// Deliberately misses a CSE-shared `b - a` (behind a `_cse_` var — keeping the share IS the win).
def private match_lerp(var cll : ExprCall?; var la, lb, lt : ExpressionPtr&) : bool {
    if (cll == null || length(cll.arguments) != 3 ||
        psh_simple_name("{cll.name}") != "mad" || !(cll.arguments[0] is ExprOp2)) return false
    var diff = cll.arguments[0] as ExprOp2
    var db : ExpressionPtr      // b — the minuend
    var da : ExpressionPtr      // a — the subtrahend
    if (diff.op == "-") {
        db = diff.left
        da = diff.right
    } elif (diff.op == "+") {
        if (diff.left is ExprOp1 && (diff.left as ExprOp1).op == "-") {
            da = (diff.left as ExprOp1).subexpr
            db = diff.right
        } elif (diff.right is ExprOp1 && (diff.right as ExprOp1).op == "-") {
            da = (diff.right as ExprOp1).subexpr
            db = diff.left
        }
    }
    if (da == null || describe(da) != describe(cll.arguments[2]) ||
        !lerp_shape(expr_bt(da), expr_bt(db), expr_bt(cll.arguments[1]))) return false
    la = cll.arguments[2]
    lb = db
    lt = cll.arguments[1]
    return true
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
class private FuseResidualVisitor : AstVisitor {
    checkLerp : bool
    foundMad : bool
    foundLerp : bool
    foundNegAdd : bool
    def override preVisitExprOp2(var expr : ExprOp2?) : void {
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
    }
}

def public fuse_body(var func : FunctionPtr) : bool {
    //! PHASE_FUSED finishing pass: re-pack `a*b ± c` into `mad(a,b,c)` and the expanded lerp
    //! shape `mad(b - a, t, a)` back into `lerp(a, b, t)` over the optimized, re-inferred body
    //! (the type gates need settled types). Caller re-infers after to type the new calls.
    if (!(func.body is ExprBlock) || !fuse_callable(func._module, "mad")) return false
    var changed = false
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
