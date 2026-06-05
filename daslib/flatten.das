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
require daslib/rtti

// Phase token carried as the "phase" argument of the [flatten] annotation on
// the ORIGINAL function (which we own and may mutate in place). PHASE_DONE
// parks the entry so it never re-transforms.
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
    counter  : int
    liveName : string
    retName  : string
    hasRet   : bool
    worklist : array<string>
    ok       : bool
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
    let retN = fresh(ctx, "__flat_ret")
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
    var crt = clone_type(callee.result)
    crt.flags.constant = false
    out |> push <| qmacro_expr(${ var $i(retN) = default<$t(crt)>; })
    let savedLive = ctx.liveName
    let savedRet = ctx.retName
    let savedHas = ctx.hasRet
    ctx.liveName = liveN
    ctx.retName = retN
    ctx.hasRet = !callee.result.isVoid
    ctx.worklist |> push(cname)
    var cbody = clone_expression(callee.body)
    subst |> apply_template(call.at, cbody, false)
    lower_list(ctx, cbody as ExprBlock, null, out)
    ctx.worklist |> pop()
    ctx.liveName = savedLive
    ctx.retName = savedRet
    ctx.hasRet = savedHas
    return qmacro($i(retN))
}

// Lift every user call out of a value expression (at any depth) into inline
// frames, in evaluation order, returning the call-free rewrite. Plain
// recursion — NOT a visitor — because inlining a call recurses back through
// the lowering pipeline, and nested make_visitor traversals corrupt the
// visitor machinery. Operand order matches daslang left-to-right evaluation.
def private lift_expr(var ctx : FlatCtx?; e : ExpressionPtr; structPred : ExpressionPtr; var out : array<ExpressionPtr>) : ExpressionPtr {
    if (e == null) return null
    if (is_user_call(e)) return inline_call(ctx, e as ExprCall, structPred, out)
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
    if (e is ExprCall) {  // builtin/leaf call — whitelist-gate, then lift args
        let bname = string((e as ExprCall).name)
        if (!key_exists(STD_WHITELIST, bname)) {
            ctx.ok = false
            ctx.err = "flatten: builtin '{bname}' is not on the whitelist (not GPU-expressible / may have side effects)"
            return clone_expression(e)
        }
        var n = clone_expression(e)
        var o = n as ExprCall
        let oe = e as ExprCall
        for (ai in range(length(oe.arguments))) {
            o.arguments[ai] = lift_expr(ctx, oe.arguments[ai], structPred, out)
        }
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
    } elif (stmt is ExprFor || stmt is ExprWhile) {
        ctx.ok = false
        ctx.err = "flatten: loops are not supported yet; unroll to a fixed count first"
    } elif (stmt is ExprMove || stmt is ExprClone) {
        ctx.ok = false
        ctx.err = "flatten: move (<-) / clone (:=) cannot be predicated; only copyable-value assignment is supported"
    } elif (stmt is ExprBlock) {
        // Bare lexical scope — flatten its statements under the same predicate.
        lower_list(ctx, stmt as ExprBlock, structPred, out)
    } else {
        // Bare expression statement (incl. a discarded call) — lift + gate.
        var lifted = lower_value(ctx, stmt, structPred, out)
        out |> emplace(lifted)
    }
}

def private lower_function(var twin : FunctionPtr) : FlatCtx? {
    var ctx = new FlatCtx(counter = 0, liveName = "__flat_live", retName = "__flat_ret",
        hasRet = !twin.result.isVoid, ok = true)
    if (!(twin.body is ExprBlock)) return ctx
    let bodyBlk = twin.body as ExprBlock
    var out : array<ExpressionPtr>
    out |> push <| qmacro_expr(${ var $i(ctx.liveName) = true; })
    if (ctx.hasRet) {
        var rt = clone_type(twin.result)
        rt.flags.constant = false
        out |> push <| qmacro_expr(${ var $i(ctx.retName) = default<$t(rt)>; })
    }
    lower_list(ctx, bodyBlk, null, out)
    if (ctx.hasRet) {
        out |> push <| qmacro_expr(${ return $i(ctx.retName); })
    }
    var newBody = new ExprBlock(at = twin.body.at)
    for (s in out) {
        newBody.list |> emplace(s)
    }
    twin.body = newBody
    return ctx
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
        if (get_flatten_phase(func) == PHASE_DONE) return true
        var twin = find_unique_function(compiling_module(), twin_name(func), true)
        if (twin == null) {
            errors := "flatten: twin {twin_name(func)} was not declared"
            return false
        }
        var ctx = lower_function(twin)
        if (!ctx.ok) {
            errors := ctx.err
            return false
        }
        set_flatten_phase(func, PHASE_DONE)
        astChanged = true
        return true
    }
}
