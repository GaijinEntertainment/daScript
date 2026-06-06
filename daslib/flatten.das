options gen2
options indenting = 4
options strict_smart_pointers = true

module flatten shared private

//! Function flattening macro (cond-only).
//!
//! ``[flatten]`` on a function generates a semantically equivalent flattened
//! twin ``<name>_flat`` in which every user call is inlined away and all
//! control flow is lowered to predicated ``cond ? a : b`` selects (early
//! ``return`` becomes a runtime "live mask"). The output contains no user
//! calls and no branches — the form a branchless GPU-shader backend requires.
//!
//! The original function is left intact. ``apply`` declares the twin early so
//! its symbol exists for inference and callers; ``patch`` transforms the
//! twin's body across compiler re-infer passes.

require daslib/ast
require daslib/ast_boost
require daslib/templates_boost
require daslib/macro_boost
require daslib/rtti
require strings

// Phase token carried as the "phase" argument of the [flatten] annotation on
// the ORIGINAL function (which we own and may mutate in place). Cycle 1 lowers
// the twin and parks it at PHASE_LOWERED (then re-infers so call.func resolves);
// cycle 2 runs the post-infer select fold and parks at PHASE_FOLDED (re-infers
// again if it changed anything); the final cycle runs the shape scan and parks at
// PHASE_DONE so the entry never re-transforms.
let private PHASE_LOWERED = 1
let private PHASE_FOLDED = 2
let private PHASE_DONE = 999

// Leaf builtins permitted to survive into flattened output: pure, branch-safe,
// GPU-expressible. Everything else (print, allocation, I/O, ...) is rejected.
// Hardcoded for now; intended to become a FlattenConfig argument.
let private STD_WHITELIST : table<string> <- {
    "min", "max", "abs", "clamp", "saturate", "sign", "floor", "ceil", "round", "trunc", "frac",
    "sqrt", "rsqrt", "pow", "exp", "exp2", "log", "log2",
    "sin", "cos", "tan", "asin", "acos", "atan", "atan2", "sincos",
    "dot", "cross", "length", "normalize", "distance", "reflect", "refract",
    "lerp", "mix", "step", "smoothstep", "rcp", "mad",
    "float2", "float3", "float4", "int2", "int3", "int4", "uint2", "uint3", "uint4"
}

// Compound-assignment operators → their base binary op. `acc += rhs` survives infer
// as an `ExprOp2` (op="+="), not an `ExprCopy`; predicated lowering rewrites it to
// `acc = P ? (acc + rhs) : acc` using the base op. This is the shader accumulator idiom.
let private ASSIGN_BASE : table<string; string> <- {
    "+=" => "+", "-=" => "-", "*=" => "*", "/=" => "/", "%=" => "%",
    "&=" => "&", "|=" => "|", "^=" => "^", "<<=" => "<<", ">>=" => ">>"
}

def private twin_name(func : FunctionPtr) : string {
    return "{func.name}_flat"
}

def private get_flatten_phase(func : FunctionPtr) : int {
    for (ann in func.annotations) {
        if (ann.annotation.name == "flatten") {
            let pv = find_arg(ann.arguments, "phase")
            if (pv is tInt) return pv as tInt
        }
    }
    return -1
}

def private set_flatten_phase(var func : FunctionPtr; ph : int) {
    for (ann in func.annotations) {
        if (ann.annotation.name != "flatten") continue
        var found = false
        for (a in ann.arguments) {
            if ("{a.name}" == "phase") {
                a.iValue = ph
                found = true
            }
        }
        if (!found) ann.arguments |> add_annotation_argument("phase", ph)
    }
}

// ===== Predicated lowering (return-elim + if-conversion + call inlining) =====
//
// Shared state lives on FlatCtx (one instance per twin, threaded by pointer so
// the gensym counter and recursion worklist are shared across inline frames).
// liveName/retName/hasRet are the CURRENT frame; inline_call saves & restores
// them when it descends into a callee. structPred is the AND of enclosing
// if-conditions on the path (null == statically true).

class private FlatCtx {
    counter   : int
    liveName  : string
    retName   : string
    hasRet    : bool
    worklist  : array<string>
    whitelist : table<string>   // leaf-builtin names allowed to survive (backend-supplied)
    ok        : bool
    err      : string
}

def private fresh(var ctx : FlatCtx?; pfx : string) : string {
    let n = "{pfx}_{ctx.counter}"
    ctx.counter ++
    return n
}

def private contains(a : array<string>; s : string) : bool {
    for (x in a) {
        if (x == s) return true
    }
    return false
}

def private is_flat_temp(name : string) : bool {
    return name |> starts_with("__flat_")
}

// Strip a monomorphized generic's mangling to the bare function name: an OR-type
// instance is named `<scope>`<name>`<hash>` (backtick = char 96). Used so the
// caller-supplied whitelist can match by simple name regardless of instantiation.
def private simple_name(fullname : string) : string {
    let t1 = find(fullname, 96, 0)
    if (t1 < 0) return fullname
    let t2 = find(fullname, 96, t1 + 1)
    let endp = t2 < 0 ? length(fullname) : t2
    return slice(fullname, t1 + 1, endp)
}

def private p_and(a, b : ExpressionPtr) : ExpressionPtr {
    if (a == null && b == null) return null
    if (a == null) return clone_expression(b)
    if (b == null) return clone_expression(a)
    return qmacro($e(a) && $e(b))
}

def private is_user_call(e : ExpressionPtr) : bool {
    if (!(e is ExprCall)) return false
    let c = e as ExprCall
    return c.func != null && !c.func.flags.builtIn && c.func.body != null
}

// Emit a callee frame into `out`; return an expr yielding the callee's result.
def private inline_call(var ctx : FlatCtx?; call : ExprCall?; structPred : ExpressionPtr; var out : array<ExpressionPtr>) : ExpressionPtr {
    let callee = call.func
    let cname = string(callee.name)
    if (ctx.worklist |> contains(cname)) {
        ctx.ok = false
        ctx.err = "flatten: recursion through '{cname}' cannot be flattened"
        return qmacro(0)
    }
    let liveN = fresh(ctx, "__flat_live")
    let isVoidCallee = callee.result.isVoid
    var subst : Template
    for (p, ai in callee.arguments, count()) {
        let argN = fresh(ctx, "__flat_arg")
        out |> push <| qmacro_expr(${ var $i(argN) = $e(call.arguments[ai]); })
        subst |> replaceVariable(string(p.name)) <| qmacro($i(argN))
    }
    // The callee runs where the caller's predicate holds.
    if (structPred == null) {
        out |> push <| qmacro_expr(${ var $i(liveN) = $i(ctx.liveName); })
    } else {
        out |> push <| qmacro_expr(${ var $i(liveN) = $i(ctx.liveName) && $e(structPred); })
    }
    // Result temp only for value-returning callees. A void callee is inlined
    // for its side effects (e.g. predicated writes to output globals); it has
    // no value to bind, so emitting `default<void>` would be a type error.
    var retN = ""
    if (!isVoidCallee) {
        retN = fresh(ctx, "__flat_ret")
        var crt = clone_type(callee.result)
        crt.flags.constant = false
        // Bare decl: das auto-inits to default<T>, so the explicit init is
        // redundant. The first write self-reads the auto-init value.
        out |> push <| qmacro_expr(${ var $i(retN) : $t(crt); })
    }
    let savedLive = ctx.liveName
    let savedRet = ctx.retName
    let savedHas = ctx.hasRet
    ctx.liveName = liveN
    ctx.retName = retN
    ctx.hasRet = !isVoidCallee
    ctx.worklist |> push(cname)
    var cbody = clone_expression(callee.body)
    subst |> apply_template(call.at, cbody, false)
    lower_list(ctx, cbody as ExprBlock, null, out)
    ctx.worklist |> pop()
    ctx.liveName = savedLive
    ctx.retName = savedRet
    ctx.hasRet = savedHas
    if (isVoidCallee) return null
    return qmacro($i(retN))
}

// Lift every user call out of a value expression (at any depth) into inline
// frames, in evaluation order, returning the call-free rewrite. Plain
// recursion — NOT a visitor — because inlining a call recurses back through
// the lowering pipeline, and nested make_visitor traversals corrupt the
// visitor machinery. Operand order matches daslang left-to-right evaluation.
def private lift_expr(var ctx : FlatCtx?; e : ExpressionPtr; structPred : ExpressionPtr; var out : array<ExpressionPtr>) : ExpressionPtr {
    if (e == null) return null
    if (e is ExprCall) {
        let c = e as ExprCall
        let bname = string(c.name)
        // Whitelist takes precedence over inlining: a whitelisted leaf primitive is
        // KEPT (args lifted), even if it has a body. Backend [hint] primitives are
        // [stub] functions — non-null (empty) body — so without this they'd be
        // treated as user calls and inlined away to nothing. Match the SIMPLE name:
        // OR-type generics monomorphize to `<scope>`<name>`<hash>`, but the backend
        // whitelists by the bare name.
        if (key_exists(ctx.whitelist, simple_name(bname))) {
            var n = clone_expression(e)
            var o = n as ExprCall
            for (ai in range(length(c.arguments))) {
                o.arguments[ai] = lift_expr(ctx, c.arguments[ai], structPred, out)
            }
            return n
        }
        // A real user helper (resolved, non-builtin, has a body) → inline it.
        if (is_user_call(e)) {
            return inline_call(ctx, c, structPred, out)
        }
        // Otherwise: a non-whitelisted leaf/builtin — not backend-expressible.
        ctx.ok = false
        ctx.err = "flatten: builtin '{bname}' is not on the whitelist (not GPU-expressible / may have side effects)"
        return clone_expression(e)
    }
    if (e is ExprOp2) {
        var n = clone_expression(e)
        var o = n as ExprOp2
        let oe = e as ExprOp2
        o.left = lift_expr(ctx, oe.left, structPred, out)
        o.right = lift_expr(ctx, oe.right, structPred, out)
        return n
    }
    if (e is ExprOp1) {
        var n = clone_expression(e)
        var o = n as ExprOp1
        let oe = e as ExprOp1
        o.subexpr = lift_expr(ctx, oe.subexpr, structPred, out)
        return n
    }
    if (e is ExprOp3) {
        var n = clone_expression(e)
        var o = n as ExprOp3
        let oe = e as ExprOp3
        o.subexpr = lift_expr(ctx, oe.subexpr, structPred, out)
        o.left = lift_expr(ctx, oe.left, structPred, out)
        o.right = lift_expr(ctx, oe.right, structPred, out)
        return n
    }
    // Member / index / cast / constructor expressions: a user call can hide inside
    // the operand of a swizzle (`f(x).xyz`), a struct field read (`f(x).a`), a cast,
    // an index, or a constructor field (`Out(emission = f(x))`). Recurse into the
    // operands so the call still gets inlined — without these, the call survives
    // lowering and the post-pass verify scan rejects it as an "internal error".
    if (e is ExprField) {
        var n = clone_expression(e)
        var o = n as ExprField
        o.value = lift_expr(ctx, (e as ExprField).value, structPred, out)
        return n
    }
    if (e is ExprSwizzle) {
        var n = clone_expression(e)
        var o = n as ExprSwizzle
        o.value = lift_expr(ctx, (e as ExprSwizzle).value, structPred, out)
        return n
    }
    if (e is ExprCast) {
        var n = clone_expression(e)
        var o = n as ExprCast
        o.subexpr = lift_expr(ctx, (e as ExprCast).subexpr, structPred, out)
        return n
    }
    if (e is ExprAt) {
        var n = clone_expression(e)
        var o = n as ExprAt
        let oe = e as ExprAt
        o.subexpr = lift_expr(ctx, oe.subexpr, structPred, out)
        o.index = lift_expr(ctx, oe.index, structPred, out)
        return n
    }
    if (e is ExprMakeStruct) {
        var n = clone_expression(e)
        var o = n as ExprMakeStruct
        for (si in range(length(o.structs))) {
            for (fld in *(o.structs[si])) {
                fld.value = lift_expr(ctx, fld.value, structPred, out)
            }
        }
        return n
    }
    if (e is ExprMakeTuple) {
        var n = clone_expression(e)
        var o = n as ExprMakeTuple
        for (vi in range(length(o.values))) {
            o.values[vi] = lift_expr(ctx, o.values[vi], structPred, out)
        }
        return n
    }
    // Typer-inserted ref/value/pointer conversion wrappers (e.g. ExprRef2Value
    // around a field read `f(x).a`). Recurse through to reach the buried call.
    if (e is ExprRef2Value) {
        var n = clone_expression(e)
        var o = n as ExprRef2Value
        o.subexpr = lift_expr(ctx, (e as ExprRef2Value).subexpr, structPred, out)
        return n
    }
    if (e is ExprPtr2Ref) {
        var n = clone_expression(e)
        var o = n as ExprPtr2Ref
        o.subexpr = lift_expr(ctx, (e as ExprPtr2Ref).subexpr, structPred, out)
        return n
    }
    return clone_expression(e)
}

def private lower_value(var ctx : FlatCtx?; e : ExpressionPtr; structPred : ExpressionPtr; var out : array<ExpressionPtr>) : ExpressionPtr {
    return lift_expr(ctx, e, structPred, out)
}

def private lower_list(var ctx : FlatCtx?; blk : ExprBlock?; structPred : ExpressionPtr; var out : array<ExpressionPtr>) {
    for (s in blk.list) {
        lower_stmt(ctx, s, structPred, out)
    }
}

def private lower_branch(var ctx : FlatCtx?; br : ExpressionPtr; structPred : ExpressionPtr; var out : array<ExpressionPtr>) {
    if (br is ExprBlock) {
        lower_list(ctx, br as ExprBlock, structPred, out)
    } else {
        lower_stmt(ctx, br, structPred, out)
    }
}

def private lower_stmt(var ctx : FlatCtx?; stmt : ExpressionPtr; structPred : ExpressionPtr; var out : array<ExpressionPtr>) {
    if (stmt is ExprReturn) {
        let r = stmt as ExprReturn
        if (ctx.hasRet && r.subexpr != null) {
            let valExpr = lower_value(ctx, r.subexpr, structPred, out)
            let pred = p_and(qmacro($i(ctx.liveName)), structPred)
            out |> push <| qmacro_expr(${ $i(ctx.retName) = $e(pred) ? $e(valExpr) : $i(ctx.retName); })
        }
        if (structPred == null) {
            out |> push <| qmacro_expr(${ $i(ctx.liveName) = false; })
        } else {
            out |> push <| qmacro_expr(${ $i(ctx.liveName) = $i(ctx.liveName) && !($e(structPred)); })
        }
    } elif (stmt is ExprIfThenElse) {
        let ite = stmt as ExprIfThenElse
        let cN = fresh(ctx, "__flat_c")
        out |> push <| qmacro_expr(${ let $i(cN) = $e(ite.cond); })
        lower_branch(ctx, ite.if_true, p_and(structPred, qmacro($i(cN))), out)
        if (ite.if_false != null) {
            lower_branch(ctx, ite.if_false, p_and(structPred, qmacro(!$i(cN))), out)
        }
    } elif (stmt is ExprCopy) {
        let cp = stmt as ExprCopy
        let rhs = lower_value(ctx, cp.right, structPred, out)
        let pred = p_and(qmacro($i(ctx.liveName)), structPred)
        out |> push <| qmacro_expr(${ $e(cp.left) = $e(pred) ? $e(rhs) : $e(cp.left); })
    } elif (stmt is ExprLet) {
        // A local declaration always executes; inline any user-call initializer
        // (its frame already encodes the path predicate via the live mask).
        var letClone = clone_expression(stmt)
        var lc = letClone as ExprLet
        for (v in lc.variables) {
            if (v.init != null) {
                v.init = lower_value(ctx, v.init, structPred, out)
            }
        }
        out |> push <| letClone
    } elif (stmt is ExprFor) {
        lower_for(ctx, stmt as ExprFor, structPred, out)
    } elif (stmt is ExprWhile) {
        ctx.ok = false
        ctx.err = "flatten: while loops are not supported; use a fixed range(N) or array-literal for loop"
    } elif (stmt is ExprBreak || stmt is ExprContinue) {
        ctx.ok = false
        ctx.err = "flatten: break / continue inside a flattened loop are not supported yet"
    } elif (stmt is ExprMove || stmt is ExprClone) {
        ctx.ok = false
        ctx.err = "flatten: move (<-) / clone (:=) cannot be predicated; only copyable-value assignment is supported"
    } elif (stmt is ExprOp2 && key_exists(ASSIGN_BASE, "{(stmt as ExprOp2).op}")) {
        // Compound assignment `lhs OP= rhs` → `lhs = P ? (lhs OP rhs) : lhs`.
        let o = stmt as ExprOp2
        let bop = ASSIGN_BASE?["{o.op}"] ?? ""
        let pred = p_and(qmacro($i(ctx.liveName)), structPred)
        var combined = new ExprOp2(at = o.at, op := bop, left = clone_expression(o.left),
            right = lower_value(ctx, o.right, structPred, out))
        out |> push <| qmacro_expr(${ $e(o.left) = $e(pred) ? $e(combined) : $e(o.left); })
    } elif (stmt is ExprOp1 && ("{(stmt as ExprOp1).op}" == "++" || "{(stmt as ExprOp1).op}" == "--")) {
        // `x++` / `x--` as a statement → `x = P ? (x +/- 1) : x` (the postfix value
        // is discarded at statement level, so pre/post is irrelevant here). Same
        // drop-if-unhandled hazard as compound assignment; PERF013 nudges toward these.
        let o = stmt as ExprOp1
        let bop = "{o.op}" == "++" ? "+" : "-"
        let pred = p_and(qmacro($i(ctx.liveName)), structPred)
        var combined = new ExprOp2(at = o.at, op := bop, left = clone_expression(o.subexpr),
            right = new ExprConstInt(at = o.at, value = 1))
        out |> push <| qmacro_expr(${ $e(o.subexpr) = $e(pred) ? $e(combined) : $e(o.subexpr); })
    } elif (stmt is ExprBlock) {
        // Bare lexical scope — flatten its statements under the same predicate.
        lower_list(ctx, stmt as ExprBlock, structPred, out)
    } elif (stmt is ExprWith) {
        // `with (x) { body }` is pure infer-time name resolution: post-infer the
        // body's accesses are already bound to x's fields, and the node simulates
        // to its body alone (ast_simulate.cpp: visit(ExprWith) = body). So unwrap
        // it — lower the body under the same predicate, exactly like a bare block.
        lower_branch(ctx, (stmt as ExprWith).body, structPred, out)
    } else {
        // Bare expression statement (a discarded value). Lifting emits any inline
        // frames and their side effects (global writes, etc.) as their own
        // statements; the residual value is unused, so it is NOT emitted. A bare
        // pure expression is dead — and e.g. `x * x` as a statement is a hard
        // compile error (30151 "no side effect operation"). Copy-prop would
        // otherwise fold a discarded result temp into exactly that.
        lower_value(ctx, stmt, structPred, out)
    }
}

// Collects the names of every `let`/`var` declared anywhere in a loop body, so
// each unrolled copy can rename them uniquely (they would otherwise all land in
// the single flat scope and shadow each other).
class private LetNameScan : AstVisitor {
    names : array<string>
    def override preVisitExprLet(expr : ExprLet?) : void {
        for (v in expr.variables) {
            names |> push("{v.name}")   // nolint:PERF006 -- macro-time, count is the (small) local set
        }
    }
}

def private collect_let_names(body : ExpressionPtr) : array<string> {
    var v = new LetNameScan()
    make_visitor(*v) $(adapter) {
        visit(body, adapter)
    }
    var result <- v.names
    unsafe {
        delete v
    }
    return <- result
}

// Fixed-count loop unrolling (M4). A `for` over a constant `range(N)` or an array
// literal has a compile-time-known iteration count: clone the body once per
// iteration, substitute the loop variable with that iteration's constant, and lower
// each unrolled copy under the SAME predicate (an unrolled body always executes).
// Per-iteration locals are renamed (`o` → `o_0`, `o_1`, …) so the copies don't
// shadow each other in the flattened scope. Mirrors daslib/unroll.das. break /
// continue are rejected upstream in lower_stmt (deferred to a later cut); while
// loops and non-constant sources error too.
def private lower_for(var ctx : FlatCtx?; efor : ExprFor?; structPred : ExpressionPtr; var out : array<ExpressionPtr>) {
    if (length(efor.iterators) != 1 || length(efor.sources) != 1) {
        ctx.ok = false
        ctx.err = "flatten: only single-variable, single-source for loops can be flattened"
        return
    }
    let src = efor.sources[0]
    // Build one substitution expression per iteration (the loop var's value).
    var substs : array<ExpressionPtr>
    if (src is ExprConstRange) {
        for (i in (src as ExprConstRange).value) {
            var e : ExpressionPtr = new ExprConstInt(at = efor.at, value = i)
            substs |> push(e)   // nolint:PERF006 -- macro-time, count is the (small) unroll bound
        }
    } else {
        // Array-literal source: `for (v in [a, b, c])` parses as
        // `to_array_move([[a, b, c]])` (ExprCall wrapping ExprMakeArray), or a bare
        // ExprMakeArray. Each element expression substitutes for one iteration.
        var mk : ExprMakeArray? = null
        if (src is ExprMakeArray) {
            mk = src as ExprMakeArray
        } elif (src is ExprCall && !empty((src as ExprCall).arguments) &&
                (src as ExprCall).arguments[0] is ExprMakeArray) {
            mk = (src as ExprCall).arguments[0] as ExprMakeArray
        }
        if (mk == null) {
            ctx.ok = false
            ctx.err = "flatten: for loop source must be a constant range(N) or an array literal"
            return
        }
        substs <- [for (v in mk.values); clone_expression(v)]
    }
    let van = "{efor.iterators[0]}"
    let localNames <- collect_let_names(efor.body)
    for (k in range(length(substs))) {
        var iblk = clone_expression(efor.body)
        remove_deref(van, iblk)               // strip ExprRef2Value around the workhorse loop var
        var rules : Template
        rules |> replaceVariable(van) <| substs[k]
        // Rename this copy's locals uniquely so the unrolled bodies don't shadow
        // each other once flattened into one scope.
        for (ln in localNames) {
            rules |> renameVariable(ln, "{ln}_{k}")
        }
        apply_template(rules, efor.at, iblk, false)
        lower_branch(ctx, iblk, structPred, out)
        if (!ctx.ok) return
    }
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

def private const_prop_pass(var out : array<ExpressionPtr>) {
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

def private copy_prop_pass(var out : array<ExpressionPtr>) {
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

def private dse_pass(var out : array<ExpressionPtr>) {
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

[macro_function]
def public flatten_function(var func : FunctionPtr; whitelist : table<string>) : FlatCtx? {
    //! Flatten `func`'s body in place to branchless, call-free form: every user
    //! call inlined, all control flow lowered to predicated `?:` selects, early
    //! `return` to a runtime live mask. `whitelist` is the set of leaf-builtin
    //! names permitted to survive into the output (must be pure / branch-safe /
    //! backend-expressible). Returns the FlatCtx — inspect `.ok` and `.err`.
    //!
    //! Backends call this directly with their own primitive set, before their own
    //! macro walks the result; `[flatten]` calls it with the std default. The
    //! body is uninferred after this — re-infer (e.g. via patch + astChanged)
    //! before reading types or running the shape check.
    var ctx = new FlatCtx(counter = 0, liveName = "__flat_live", retName = "__flat_ret",
        hasRet = !func.result.isVoid, ok = true)
    ctx.whitelist := whitelist
    if (!(func.body is ExprBlock)) return ctx
    let bodyBlk = func.body as ExprBlock
    var out : array<ExpressionPtr>
    out |> push <| qmacro_expr(${ var $i(ctx.liveName) = true; })
    if (ctx.hasRet) {
        var rt = clone_type(func.result)
        rt.flags.constant = false
        out |> push <| qmacro_expr(${ var $i(ctx.retName) : $t(rt); })
    }
    lower_list(ctx, bodyBlk, null, out)
    if (ctx.hasRet) {
        out |> push <| qmacro_expr(${ return $i(ctx.retName); })
    }
    if (ctx.ok) {
        dse_pass(out)          // drop dead mask-kills (`__flat_live = false`) first…
        const_prop_pass(out)   // …so an unreassigned `__flat_live` reads as const true → collapse
        copy_prop_pass(out)    // inline single-def temps (`__flat_ret = c; return __flat_ret` → `return c`)
        dse_pass(out)          // remove anything left dead (collapsed mask decls, folded temps)
    }
    var newBody = new ExprBlock(at = func.body.at)
    for (s in out) {
        newBody.list |> emplace(s)
    }
    func.body = newBody
    return ctx
}

// ===== Post-infer select folding (infer_type_fold) =====
//
// The typer folds constant comparisons during re-infer (`0 >= 2` → false) but
// leaves the wrapping `?:` in place: at infer time it lacks the side-effect flags
// to prove the dead arm droppable, so a const-condition select survives — and a
// predicated self-assign then collapses (post-typer) to `acc = acc`, which a
// dataflow backend cannot resolve (an accumulator that reads itself with no
// producer). This pass runs AFTER re-infer, where the conditions are already
// `ExprConstBool`: it collapses every `const ? a : b` to the live arm (full tree,
// so deeply-buried selects fold too), drops the resulting `lhs = lhs` self-assigns
// whose lvalue is provably side-effect-free, and strips redundant zero/default
// initializers (`var acc = 0` → `var acc : int`). The cleanups the compiler will
// not do at infer time, done here with the lint side-effect estimate.

class private FoldOp3Visitor : AstVisitor {
    changed : bool
    def override visitExprOp3(var that : ExprOp3?) : ExpressionPtr {
        // Post-order: subexpr/arms already folded, so a now-constant condition
        // collapses to its live arm (the dead arm + condition become unreachable).
        if (that.subexpr is ExprConstBool) {
            changed = true
            return (that.subexpr as ExprConstBool).value ? that.left : that.right
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

[macro_function]
def public flatten_fold(var func : FunctionPtr) : bool {
    //! Post-infer select-folding pass: collapse `const ? a : b` to the live arm, drop
    //! the side-effect-free `lhs = lhs` self-assigns left behind by a folded false-select,
    //! and strip redundant zero/default initializers (`var acc = 0` → `var acc : int`).
    //! Run it AFTER re-inference — the constant conditions must already be `ExprConstBool`,
    //! which only the typer produces (and the var types must be settled). Returns true if anything
    //! changed (the caller should then re-infer for clean codegen). `[flatten]` runs
    //! this automatically between lowering and the shape check; backends calling
    //! `flatten_function` run it after their own re-infer, before consuming the twin.
    if (!(func.body is ExprBlock)) return false
    var v = new FoldOp3Visitor()
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

// ===== Post-inference shape verification =====
//
// After lowering + re-infer, the twin must be branchless and call-free: no
// ExprIfThenElse (all if/else became `?:` selects) and no surviving user call
// (all inlined). A violation is a lowering bug — fail loudly rather than hand a
// branchful/callful twin to a backend that cannot represent it. Runs in cycle 2
// (post-infer) because user-call detection needs the resolved `call.func`.

class private FlatVerify : AstVisitor {
    sawBranch : bool
    sawCall   : bool
    callName  : string
    def override preVisitExprIfThenElse(expr : ExprIfThenElse?) : void {
        sawBranch = true
    }
    def override preVisitExprCall(expr : ExprCall?) : void {
        if (expr.func != null && !expr.func.flags.builtIn && expr.func.body != null) {
            sawCall = true
            callName = "{expr.name}"
        }
    }
}

def private verify_flat(var twin : FunctionPtr) : string {
    var v = new FlatVerify()
    make_visitor(*v) $(adapter) {
        visit(twin.body, adapter)
    }
    var msg = ""
    if (v.sawBranch) {
        msg = "flatten: internal error — a branch (if/else) survived lowering in '{twin.name}'"
    } elif (v.sawCall) {
        msg = "flatten: internal error — user call '{v.callName}' survived lowering in '{twin.name}'"
    }
    unsafe {
        delete v
    }
    return msg
}

// ===== Annotation entry point =====

[function_macro(name="flatten")]
class FlattenAnnotation : AstFunctionAnnotation {
    //! ``[flatten]`` entry point.
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
        let tn = twin_name(func)
        if (find_unique_function(compiling_module(), tn, true) == null) {
            var fn = clone_function(func)
            fn.name := tn
            fn.flags |= FunctionFlags.generated | FunctionFlags.exports
            while (fn.annotations |> length != 0) {
                fn.annotations |> erase(0)
            }
            compiling_module() |> add_function(fn)
        }
        return true
    }
    def override patch(var func : FunctionPtr; var group : ModuleGroup;
                       args, progArgs : AnnotationArgumentList; var errors : das_string;
                       var astChanged : bool&) : bool {
        // Lint runs with constant folding off, so `range(N)` is not yet an
        // ExprConstRange and the generated twin is irrelevant to linting the source
        // anyway. Leave the twin as the inert clone from `apply` (mirrors how
        // daslib/unroll.das skips unrolling under is_in_lint_check()).
        if (is_in_lint_check()) return true
        let ph = get_flatten_phase(func)
        if (ph == PHASE_DONE) return true
        var twin = find_unique_function(compiling_module(), twin_name(func), true)
        if (twin == null) {
            errors := "flatten: twin {twin_name(func)} was not declared"
            return false
        }
        if (ph != PHASE_LOWERED && ph != PHASE_FOLDED) {
            // Cycle 1: lower the twin with the std whitelist, then re-infer so
            // call.func resolves. (Backends call flatten_function directly with
            // their own primitive set instead of going through this annotation.)
            var ctx = flatten_function(twin, STD_WHITELIST)
            if (!ctx.ok) {
                errors := ctx.err
                return false
            }
            set_flatten_phase(func, PHASE_LOWERED)
            astChanged = true
            return true
        }
        if (ph == PHASE_LOWERED) {
            // Cycle 2: the twin is inferred — collapse const-condition selects and
            // drop the resulting pure self-assigns. Re-infer if anything changed so
            // the verify scan and codegen see clean types.
            let folded = flatten_fold(twin)
            set_flatten_phase(func, PHASE_FOLDED)
            if (folded) {
                astChanged = true
                return true
            }
            // Nothing to fold — fall through to the shape check this cycle.
        }
        // Final cycle: verify the twin is branchless and call-free.
        let problem = verify_flat(twin)
        if (problem != "") {
            errors := problem
            return false
        }
        set_flatten_phase(func, PHASE_DONE)
        return true
    }
}
