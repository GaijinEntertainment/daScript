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

def private is_flat_temp(name : string) : bool {
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
    reads : table<string>
    risky : bool
    def override preVisitExprVar(expr : ExprVar?) : void {
        let nm = "{expr.name}"
        if (is_flat_temp(nm)) reads |> insert(nm)
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
def private scan_flat(var e : ExpressionPtr; var reads : table<string>; var risky : bool&) {
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

def private fold_const(e : ExpressionPtr; constmap : table<string; bool>) : ExpressionPtr {
    if (e == null) return null
    if (e is ExprVar) {
        let nm = "{(e as ExprVar).name}"
        if (key_exists(constmap, nm)) return new ExprConstBool(at = e.at, value = constmap?[nm] ?? false)
        return clone_expression(e)
    }
    if (e is ExprOp1) {
        var sub = fold_const((e as ExprOp1).subexpr, constmap)
        var bv = false
        if ((e as ExprOp1).op == "!" && const_bool(sub, bv)) {
            return new ExprConstBool(at = e.at, value = !bv)
        }
        var n = clone_expression(e)
        (n as ExprOp1).subexpr = sub
        return n
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
                if (lb) return r
                return new ExprConstBool(at = e.at, value = false)
            }
            if (rc) {
                if (rb) return l
                return new ExprConstBool(at = e.at, value = false)
            }
        } elif (o.op == "||") {
            if (lc) {
                if (lb) return new ExprConstBool(at = e.at, value = true)
                return r
            }
            if (rc) {
                if (rb) return new ExprConstBool(at = e.at, value = true)
                return l
            }
        }
        var n = clone_expression(e)
        (n as ExprOp2).left = l
        (n as ExprOp2).right = r
        return n
    }
    if (e is ExprOp3) {
        let o = e as ExprOp3
        var c = fold_const(o.subexpr, constmap)
        var cb = false
        if (const_bool(c, cb)) {
            return cb ? fold_const(o.left, constmap) : fold_const(o.right, constmap)
        }
        var n = clone_expression(e)
        (n as ExprOp3).subexpr = c
        (n as ExprOp3).left = fold_const(o.left, constmap)
        (n as ExprOp3).right = fold_const(o.right, constmap)
        return n
    }
    if (e is ExprCall) {
        let o = e as ExprCall
        var n = clone_expression(e)
        var nc = n as ExprCall
        for (ai in range(length(o.arguments))) {
            nc.arguments[ai] = fold_const(o.arguments[ai], constmap)
        }
        return n
    }
    return clone_expression(e)
}

def public const_prop_pass(var out : array<ExpressionPtr>) {
    // A `__flat_*` bool temp is constant when it is declared with a const-foldable
    // init and never reassigned. Reassigned temps (early-return live masks) stay.
    var reassigned : table<string>
    for (s in out) {
        if (s is ExprCopy && (s as ExprCopy).left is ExprVar) {
            let nm = "{((s as ExprCopy).left as ExprVar).name}"
            if (is_flat_temp(nm)) reassigned |> insert(nm)
        }
    }
    var constmap : table<string; bool>
    var changed = true
    while (changed) {
        changed = false
        for (s in out) {
            if (!(s is ExprLet)) continue
            let lc = s as ExprLet
            if (length(lc.variables) != 1) continue
            let v = lc.variables[0]
            let nm = "{v.name}"
            if (!is_flat_temp(nm) || key_exists(reassigned, nm) || key_exists(constmap, nm) || v.init == null) {
                continue
            }
            var bv = false
            if (const_bool(fold_const(v.init, constmap), bv)) {
                constmap[nm] = bv
                changed = true
            }
        }
    }
    if (empty(constmap)) return
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
        if ("{expr.name}" == name) count ++
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
    reads : table<string>
    risky : bool
    def override preVisitExprVar(expr : ExprVar?) : void {
        reads |> insert("{expr.name}")
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
def private scan_src(var e : ExpressionPtr; var reads : table<string>; var risky : bool&) {
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

def public copy_prop_pass(var out : array<ExpressionPtr>) {
    var changed = true
    while (changed) {
        changed = false
        let n = length(out)
        // Map every assignment target to its store indices. A source reading var V
        // is only unsafe if V is reassigned BETWEEN this temp's def and a use — a
        // reassignment fully before the def (e.g. an accumulator finalized into the
        // return temp) leaves the captured value intact, so it must not block.
        var storeIdx : table<string; array<int>>
        for (j in range(n)) {
            if (out[j] is ExprCopy && (out[j] as ExprCopy).left is ExprVar) {
                storeIdx["{((out[j] as ExprCopy).left as ExprVar).name}"] |> push(j)
            }
        }
        for (k in range(n)) {
            if (!(out[k] is ExprLet)) continue
            let lc = out[k] as ExprLet
            if (length(lc.variables) != 1) continue
            let nm = "{lc.variables[0].name}"
            if (!is_flat_temp(nm)) continue
            // Single definition: decl-init + no stores, OR no-init decl + one store.
            let stores <- [for (j in range(n)); j;
                where out[j] is ExprCopy && (out[j] as ExprCopy).left is ExprVar &&
                    "{((out[j] as ExprCopy).left as ExprVar).name}" == nm]
            var src : ExpressionPtr = null
            var si = -1
            if (lc.variables[0].init != null && empty(stores)) {
                src = clone_expression(lc.variables[0].init)   // decl-init is the single definition
            } elif (lc.variables[0].init == null && length(stores) == 1) {
                si = stores[0]
                src = clone_expression((out[si] as ExprCopy).right)   // no-init decl + single store
            } else {
                continue                                   // multi-def (e.g. early-return mask) — leave it
            }
            let defIdx = si >= 0 ? si : k
            var srcReads : table<string>
            var srcRisky = false
            scan_src(src, srcReads, srcRisky)
            if (key_exists(srcReads, nm)) continue         // self-referential source — never fold
            // Uses must follow the def; track the last one for the reassignment window.
            var refCount = 0
            var firstRef = n
            var maxUseIdx = -1
            for (j in range(n)) {
                if (j == k || j == si) continue
                let c = count_refs(out[j], nm)
                if (c > 0) {
                    refCount += c
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
            var rules : Template
            rules |> replaceVariable(nm, clone_expression(src))
            for (j in range(n)) {
                if (j == k || j == si) continue
                out[j] = apply_template(rules, out[j].at, out[j], false)
            }
            var dead : table<int>
            dead |> insert(k)
            if (si >= 0) dead |> insert(si)
            out <- [for (j in range(n)); out[j]; where !key_exists(dead, j)]
            changed = true
            break
        }
    }
}

def public dse_pass(var out : array<ExpressionPtr>) {
    let n = length(out)
    // Names that are ever an assignment target: their decl must stay even if the
    // init value is dead (only fully-unreferenced decls are removable).
    var storeTargets : table<string>
    for (s in out) {
        if (s is ExprCopy && (s as ExprCopy).left is ExprVar) {
            let nm = "{((s as ExprCopy).left as ExprVar).name}"
            if (is_flat_temp(nm)) storeTargets |> insert(nm)
        }
    }
    var live : table<string>
    var dead : table<int>
    var i = n - 1
    while (i >= 0) {
        var stmt = out[i]
        var handled = false
        if (stmt is ExprCopy) {
            var cp = stmt as ExprCopy
            if (cp.left is ExprVar && is_flat_temp("{(cp.left as ExprVar).name}")) {
                handled = true
                let lnm = "{(cp.left as ExprVar).name}"
                var reads : table<string>
                var risky = false
                scan_flat(cp.right, reads, risky)
                if (!key_exists(live, lnm) && !risky) {
                    dead |> insert(i)   // target not live afterward, RHS pure → dead
                } else {
                    live |> erase(lnm)
                    for (nm in keys(reads)) {
                        live |> insert(nm)
                    }
                }
            }
        }
        if (!handled && stmt is ExprLet) {
            var lc = stmt as ExprLet
            if (length(lc.variables) == 1 && is_flat_temp("{lc.variables[0].name}")) {
                handled = true
                let dnm = "{lc.variables[0].name}"
                var reads : table<string>
                var risky = false
                scan_flat(lc.variables[0].init, reads, risky)
                if (!key_exists(live, dnm) && !key_exists(storeTargets, dnm) && !risky) {
                    dead |> insert(i)   // declared temp fully unreferenced, init pure → dead
                } else {
                    live |> erase(dnm)
                    for (nm in keys(reads)) {
                        live |> insert(nm)
                    }
                }
            }
        }
        if (!handled) {
            // Decl / return / global write / bare expr — over-approximate: every
            // flat var it mentions becomes live (never wrongly removes a store).
            var reads : table<string>
            var risky = false
            scan_flat(stmt, reads, risky)
            for (nm in keys(reads)) {
                live |> insert(nm)
            }
        }
        i --
    }
    if (empty(dead)) return
    out <- [for (j in range(n)); out[j]; where !key_exists(dead, j)]
}

class private FoldVisitor : AstVisitor {
    changed : bool
    def override visitExprOp2(var that : ExprOp2?) : ExpressionPtr {
        // Integer-only algebraic identities. Float is excluded on purpose: `x * 0.0`
        // is NaN for x=±inf/NaN, and `x + 0.0` flips -0.0 to +0.0 — the identities only
        // hold for two's-complement integers (where `e * -1 == -e` even at INT_MIN).
        // Only `* 0` DROPS an operand, so only it needs the side-effect gate.
        if (that.op == "*") {
            if (is_int_val(that.right, 0l) && !has_sideeffects(that.left)) { changed = true; return that.right }
            if (is_int_val(that.left, 0l) && !has_sideeffects(that.right)) { changed = true; return that.left }
            if (is_int_val(that.right, 1l)) { changed = true; return that.left }
            if (is_int_val(that.left, 1l)) { changed = true; return that.right }
            if (is_int_neg_one(that.right)) { changed = true; return negate(that.left) }
            if (is_int_neg_one(that.left)) { changed = true; return negate(that.right) }
        } elif (that.op == "+") {
            if (is_int_val(that.right, 0l)) { changed = true; return that.left }
            if (is_int_val(that.left, 0l)) { changed = true; return that.right }
        } elif (that.op == "-") {
            if (is_int_val(that.right, 0l)) { changed = true; return that.left }
            if (is_int_val(that.left, 0l)) { changed = true; return negate(that.right) }
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
        // Equal arms (`c ? a : a → a`) — e.g. after `acc + 0 → acc` makes both arms `acc`.
        // Gated on a pure condition (dropping it must not lose a side effect).
        if (describe(that.left) == describe(that.right) && !has_sideeffects(that.subexpr)) {
            changed = true
            return that.left
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

def private negate(var e : ExpressionPtr) : ExpressionPtr {
    return new ExprOp1(at = e.at, op := "-", subexpr = e)
}

def public fold_body(var func : FunctionPtr) : bool {
    //! Post-infer fold over a flattened body: collapse `const ? a : b` to the live
    //! arm, drop the side-effect-free `lhs = lhs` self-assigns a folded false-select
    //! leaves behind, and strip redundant zero/default inits (`var acc = 0` →
    //! `var acc : int`). Call AFTER re-inference (conditions must be `ExprConstBool`).
    if (!(func.body is ExprBlock)) return false
    var v = new FoldVisitor()
    make_visitor(*v) $(adapter) {
        visit(func.body, adapter)
    }
    var changed = v.changed
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
