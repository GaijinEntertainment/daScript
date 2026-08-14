options gen2
options indenting = 4
options strict_smart_pointers = true
options _comment_hygiene = true

module flatten_opt_straightline shared private

//! Straight-line cleanup of a lowered twin body (pre-infer): copy-prop, dead-store elimination, mask const-prop, SSA-rename.

require daslib/ast
require daslib/ast_boost
require daslib/ast_match
require daslib/templates_boost
require daslib/macro_boost
require daslib/match
require daslib/rtti
require daslib/flatten_opt_common


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

// Collapse `&&` / `||` once a side has folded to a const bool (`true && c → c`, `false && c → false`,
// …); null when neither side is const, so the caller rebuilds the node from `l`/`r`.
def private fold_bool_short_circuit(o : ExprOp2?; var l, r : ExpressionPtr) : ExpressionPtr {
    var lb = false
    var rb = false
    let lc = const_bool(l, lb)
    let rc = const_bool(r, rb)
    if (o.op == "&&") {
        if (lc) {
            return r if (lb)
            return new ExprConstBool(at = o.at, value = false)
        }
        if (rc) {
            return l if (rb)
            return new ExprConstBool(at = o.at, value = false)
        }
    } elif (o.op == "||") {
        if (lc) {
            return new ExprConstBool(at = o.at, value = true) if (lb)
            return r
        }
        if (rc) {
            return new ExprConstBool(at = o.at, value = true) if (rb)
            return l
        }
    }
    return null
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
        var collapsed = fold_bool_short_circuit(o, l, r)
        if (collapsed != null) return collapsed
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

struct private UseSpan {
    firstRef : int     // earliest referencing statement (n when there is none)
    maxUseIdx : int    // latest referencing statement (-1 when there is none)
}

// Every store target's statement indices, in order.
def private collect_store_indices(out : array<ExpressionPtr>; var storeIdx : table<uint64; array<int>>) {
    for (j in range(length(out))) {
        if (out[j] is ExprCopy && (out[j] as ExprCopy).left is ExprVar) {
            storeIdx[hash(((out[j] as ExprCopy).left as ExprVar).name)] |> push(j)
        }
    }
}

// Statements referencing `hk` — the def `k` and its store `si` excluded — straight from the
// precomputed index; refills `useJs` and returns the span they cover.
def private collect_use_indices(var ub : FlatUsesBuilder?; hk : uint64; k, si, n : int;
                                var useJs : array<int>) : UseSpan {
    var span = UseSpan(firstRef = n, maxUseIdx = -1)
    clear(useJs)
    if (key_exists(ub.nameUses, hk)) {
        for (j in ub.nameUses[hk]) {
            if (j == k || j == si) continue
            useJs |> push(j)
            if (j < span.firstRef) {
                span.firstRef = j
            }
            if (j > span.maxUseIdx) {
                span.maxUseIdx = j
            }
        }
    }
    return span
}

// Position-aware guard: true when a var the source reads is reassigned strictly between the def
// and the last use (a reassignment fully before the def leaves the captured value intact).
def private reassigned_between(var storeIdx : table<uint64; array<int>>; srcReads : table<uint64>;
                               defIdx, maxUseIdx : int) : bool {
    for (rn in keys(srcReads)) {
        if (!key_exists(storeIdx, rn)) continue
        for (r in storeIdx[rn]) {
            if (r > defIdx && r <= maxUseIdx) return true
        }
    }
    return false
}

// Inline `src` at every use of `nm`, then drop the def at `k` (and its store at `si`, if any).
def private inline_and_drop(var out : array<ExpressionPtr>; useJs : array<int>; nm : string;
                            var src : ExpressionPtr; k, si : int; constMap : table<uint64; bool>) {
    var rules : Template
    rules |> replaceVariable(nm, src)   // store src directly; apply_template clones per use
    let constSrc = src is ExprConstBool
    for (j in useJs) {
        out[j] = apply_template(rules, out[j].at, out[j], false)   // only statements that reference nm
        if (constSrc) {
            out[j] = fold_stmt(out[j], constMap)   // collapse the now-literal mask: true && c → c
        }
    }
    unsafe { delete rules }
    // drop IN PLACE (a comprehension strands `out`'s old buffer); larger index first, so it cannot shift the smaller
    if (si > k) {
        out |> erase(si)
    }
    out |> erase(k)
    if (si >= 0 && si < k) {
        out |> erase(si)
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
        // index.
        var storeIdx : table<uint64; array<int>>
        collect_store_indices(out, storeIdx)
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
            // nameCount includes the store's lvalue occurrence (1), which is not a use.
            let refCount = (key_exists(ub.nameCount, hk) ? ub.nameCount[hk] : 0) - (si >= 0 ? 1 : 0)
            let span = collect_use_indices(ub, hk, k, si, n, useJs)
            if ((refCount > 0 && span.firstRef < defIdx) || (srcRisky && refCount > 1) ||
                    reassigned_between(storeIdx, srcReads, defIdx, span.maxUseIdx)) continue
            inline_and_drop(out, useJs, "{lc.variables[0].name}", src, k, si, emptyConstMap)
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

// One backward liveness step for a flat-temp def at `i` (`removable` is false when the decl must
// survive an unreferenced init): drop the statement when its target is dead afterward and `rhs` is
// pure, else kill the target and revive everything `rhs` reads. `reads` is the caller's scratch.
def private dse_step(i : int; h : uint64; var rhs : ExpressionPtr; removable : bool;
                     var live : table<uint64>; var dead : table<int>; var reads : table<uint64>) {
    var risky = false
    scan_flat(rhs, reads, risky)
    if (removable && !key_exists(live, h) && !risky) {
        dead |> insert(i)
    } else {
        live |> erase(h)
        for (nm in keys(reads)) {
            live |> insert(nm)
        }
    }
}

// Flat temps that are ever an assignment target: their decl must stay even if the init value is
// dead (only fully-unreferenced decls are removable).
def private collect_store_targets(out : array<ExpressionPtr>; var storeTargets : table<uint64>) {
    for (s in out) {
        if (s is ExprCopy && (s as ExprCopy).left is ExprVar) {
            let lv = (s as ExprCopy).left as ExprVar
            if (is_flat_temp(lv.name)) storeTargets |> insert(hash(lv.name))
        }
    }
}

def public dse_pass(var out : array<ExpressionPtr>) {
    let n = length(out)
    var storeTargets : table<uint64>
    collect_store_targets(out, storeTargets)
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
                handled = true   // a store dies when its target is not live afterward
                dse_step(i, hash((cp.left as ExprVar).name), cp.right, true, live, dead, reads)
            }
        }
        if (!handled && stmt is ExprLet) {
            var lc = stmt as ExprLet
            if (length(lc.variables) == 1 && is_flat_temp(lc.variables[0].name)) {
                handled = true   // a decl dies only when the temp is fully unreferenced
                let dh = hash(lc.variables[0].name)
                dse_step(i, dh, lc.variables[0].init, !key_exists(storeTargets, dh), live, dead, reads)
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
    // compact IN PLACE — a fresh comprehension would strand `out`'s old buffer
    var kept = 0
    for (j in range(n)) {
        if (key_exists(dead, j)) continue
        out[kept] = out[j]
        kept ++
    }
    out |> resize(kept)
    delete dead
}

// ===== Flow-sensitive const-bool propagation of loop/iter masks =====
// Break/continue masks are multi-def, so single-def copy_prop skips them; a forward scan instead
// carries each mask's known const value to its uses, collapsing narrows and leaving spent masks dead.

// True if `e` reads any flat-mask currently in `known` (fills `reads`, cleared first). Cheap
// O(size) scan gating the cloning fold_const so mask-free statements are skipped.
def private reads_known(var e : ExpressionPtr; known : table<uint64; bool>; var reads : table<uint64>) : bool {
    clear(reads)
    var risky = false
    scan_flat(e, reads, risky)
    for (h in keys(reads)) {
        if (key_exists(known, h)) return true
    }
    return false
}

def public mask_const_prop_pass(var out : array<ExpressionPtr>) {
    var known : table<uint64; bool>   // flat-mask name-hash -> currently-known const bool value
    var reads : table<uint64>         // scratch, cleared per statement
    let n = length(out)
    for (i in range(n)) {
        var s = out[i]
        // (1) flat-mask decl `var M = <init>`: fold init under known masks, then (re)seed M.
        if (s is ExprLet && length((s as ExprLet).variables) == 1 && is_flat_temp((s as ExprLet).variables[0].name)) {
            var lc = s as ExprLet
            let h = hash(lc.variables[0].name)
            if (lc.variables[0].init != null) {
                if (!empty(known) && reads_known(lc.variables[0].init, known, reads)) {
                    lc.variables[0].init = fold_const(lc.variables[0].init, known)
                }
                var bv = false
                if (const_bool(lc.variables[0].init, bv)) {
                    known[h] = bv
                } else {
                    known |> erase(h)
                }
            } else {
                known |> erase(h)
            }
            continue
        }
        // (2) flat-mask store `M = <rhs>`: fold rhs (the `M = M && X` collapse), then update M.
        if (s is ExprCopy && (s as ExprCopy).left is ExprVar && is_flat_temp(((s as ExprCopy).left as ExprVar).name)) {
            var cp = s as ExprCopy
            let h = hash((cp.left as ExprVar).name)
            if (!empty(known) && reads_known(cp.right, known, reads)) {
                cp.right = fold_const(cp.right, known)
            }
            var bv = false
            if (const_bool(cp.right, bv)) {
                known[h] = bv
            } else {
                known |> erase(h)
            }
            continue
        }
        // (3) any other statement: fold its mask reads (select predicates) under known masks.
        if (!empty(known) && reads_known(s, known, reads)) {
            out[i] = fold_stmt(s, known)
        }
    }
    delete known
    delete reads
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

// Locals to SSA-rename: declared single-var WITH an initializer (user locals, plus inline-renamed
// `__flat_loc_` accumulators — NOT the masks) and later reassigned by a plain `var = ...`.
def private collect_ssa_names(out : array<ExpressionPtr>; var ssaNames : table<uint64>) {
    var declaredInit : table<uint64>
    for (s in out) {
        if (s is ExprLet && length((s as ExprLet).variables) == 1) {
            let v = (s as ExprLet).variables[0]
            if ((!is_flat_temp(v.name) || is_flat_inlined_local(v.name)) && v.init != null) declaredInit |> insert(hash(v.name))
        }
    }
    if (!empty(declaredInit)) {
        for (s in out) {
            if (s is ExprCopy && (s as ExprCopy).left is ExprVar) {
                let h = hash(((s as ExprCopy).left as ExprVar).name)
                if (key_exists(declaredInit, h)) ssaNames |> insert(h)
            }
        }
    }
    delete declaredInit
}

// Mint the next `__ssa_` version of `orig` bound to `value`, and make it the current version.
def private ssa_redefine(orig : string; var value : ExpressionPtr; var counter : int&;
                         var current : table<string; string>) : ExpressionPtr {
    counter ++
    let vn = "__ssa_{orig}_{counter}"
    current[orig] = vn
    return qmacro_expr(${ var $i(vn) = $e(value); })
}

def public ssa_rename_pass(var out : array<ExpressionPtr>) {
    var ssaNames : table<uint64>
    collect_ssa_names(out, ssaNames)
    if (empty(ssaNames)) {
        delete ssaNames
        return
    }
    // forward rename: each read maps to the current version; each write mints a new version
    // (after resolving the RHS reads, so a self-ref reads the prior version).
    var current : table<string; string>
    var counter = 0
    var i = 0
    while (i < length(out)) {
        var s = out[i]
        if (s is ExprLet && length((s as ExprLet).variables) == 1 &&
                key_exists(ssaNames, hash((s as ExprLet).variables[0].name))) {
            let orig = "{(s as ExprLet).variables[0].name}"
            out[i] = ssa_redefine(orig, rename_reads((s as ExprLet).variables[0].init, current), counter, current)
        } elif (s is ExprCopy && (s as ExprCopy).left is ExprVar &&
                key_exists(ssaNames, hash(((s as ExprCopy).left as ExprVar).name))) {
            let orig = "{((s as ExprCopy).left as ExprVar).name}"
            out[i] = ssa_redefine(orig, rename_reads((s as ExprCopy).right, current), counter, current)
        } else {
            out[i] = rename_reads(s, current)
        }
        i ++
    }
    delete ssaNames
    delete current
}
