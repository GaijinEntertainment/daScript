options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

options strict_smart_pointers

module with_boost shared private

//! ``with_`` macro: bind one or more array / table element references inside a block,
//! with an automatic ``lock`` around the body so push/erase/resize/clear panic at
//! runtime instead of silently dangling. The block-arg constness is inferred from
//! the container; workhorse element types are bound by reference so the body can
//! mutate them. See ``tutorials/macros/18_with_boost.das`` for usage examples.
//!
//! Restrictions:
//!
//! * Each container arg must be an ``ExprAt`` (``arr[i]`` / ``tab[k]``). Plain
//!   locals, struct fields on locals, and function-call results are refused —
//!   use built-in ``with`` for those.
//! * At most one table-keyed arg per call (any second table arg refused —
//!   rehash on insert invalidates earlier pinned entries).
//! * Multi-arg + ``return`` from the body is refused in this phase; use the
//!   single-arg form for value-returning bodies.
//! * Bodies that ``panic`` leak the lock until issue #2532 lands; matches
//!   existing daslib behaviour (``daslib/array_boost::array_view`` etc.).

require daslib/ast
require daslib/ast_boost
require daslib/templates_boost
require daslib/macro_boost

// ──────────────────────────────────────────────────────────────────────────────
// Helper functions emitted by the macro. ``_mutable`` lock variants accept both
// const and var containers (the C++ side ``const_cast``-s) so a single generic
// overload covers every const mode.
// ──────────────────────────────────────────────────────────────────────────────

def public _with_locked_arr1_v(
                               var a : array<auto(TT)>; idx : int;
                               b : block<(var x : TT) : void>) {
    __builtin_array_lock_mutable(a)
    invoke(b, a[idx])
} finally {
    __builtin_array_unlock_mutable(a)
}

def public _with_locked_arr1_r(
                               var a : array<auto(TT)>; idx : int;
                               b : block<(var x : TT)>) {
    __builtin_array_lock_mutable(a)
    return invoke(b, a[idx])
} finally {
    __builtin_array_unlock_mutable(a)
}

def public _with_locked_tab1_v(
                               var t : table<auto(KT); auto(VT)>; key : KT;
                               b : block<(var x : VT) : void>) {
    // Resolve the element ref BEFORE locking — t[key] would otherwise hit
    // the insert path inside a locked table and panic. The lock then pins
    // the table (no rehash) for the duration of the body, keeping `elem`
    // valid. `unsafe(...)` quiets the local-ref-to-non-local-expression
    // safety check, which the lock makes a no-op.
    unsafe {
        var elem & = t[key]
        __builtin_table_lock_mutable(t)
        invoke(b, elem)
    }
} finally {
    __builtin_table_unlock_mutable(t)
}

def public _with_locked_tab1_r(
                               var t : table<auto(KT); auto(VT)>; key : KT;
                               b : block<(var x : VT)>) {
    unsafe {
        var elem & = t[key]
        __builtin_table_lock_mutable(t)
        return invoke(b, elem)
    }
} finally {
    __builtin_table_unlock_mutable(t)
}

def public _with_locked_arr2_v(
                               var a1 : array<auto(T1)>; var a2 : array<auto(T2)>;
                               i1 : int; i2 : int;
                               b : block<(var x : T1; var y : T2) : void>) {
    __builtin_array_lock_mutable(a1)
    __builtin_array_lock_mutable(a2)
    invoke(b, a1[i1], a2[i2])
} finally {
    __builtin_array_unlock_mutable(a2)
    __builtin_array_unlock_mutable(a1)
}

def public _with_locked_arr3_v(
                               var a1 : array<auto(T1)>; var a2 : array<auto(T2)>; var a3 : array<auto(T3)>;
                               i1 : int; i2 : int; i3 : int;
                               b : block<(var x : T1; var y : T2; var z : T3) : void>) {
    __builtin_array_lock_mutable(a1)
    __builtin_array_lock_mutable(a2)
    __builtin_array_lock_mutable(a3)
    invoke(b, a1[i1], a2[i2], a3[i3])
} finally {
    __builtin_array_unlock_mutable(a3)
    __builtin_array_unlock_mutable(a2)
    __builtin_array_unlock_mutable(a1)
}

// ──────────────────────────────────────────────────────────────────────────────
// Call macro
// ──────────────────────────────────────────────────────────────────────────────

def private block_returns_value(blk : ExprBlock?) : bool {
    for (st in blk.list) {
        if (st is ExprReturn) {
            let r = st as ExprReturn
            return true if (r.subexpr != null)
        }
    }
    return false
}

def private container_kind(atNode : ExprAt?; prog : ProgramPtr) : string {
    // Returns "array", "table", or "" if unrecognized
    let subT = atNode.subexpr._type
    return "" if (subT == null)
    return "array" if (subT.baseType == Type.tArray)
    return "table" if (subT.baseType == Type.tTable)
    return ""
}

[call_macro(name="with_")]
class private WithMacro : AstCallMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : ExpressionPtr {
        macro_verify(length(call.arguments) >= 2, prog, call.at,
            "with_ needs at least one container arg plus a block")
        let totalArgs = length(call.arguments)
        let containerCount = totalArgs - 1
        let blockArg = call.arguments[totalArgs - 1]
        macro_verify(blockArg is ExprMakeBlock, prog, call.at,
            "with_ last argument must be a block, got {describe(blockArg)}")
        macro_verify(containerCount >= 1 && containerCount <= 3, prog, call.at,
            "with_ supports 1..3 container args, got {containerCount}")
        let containerExpr = call.arguments[0]
        macro_verify(containerExpr is ExprAt, prog, call.at,
            "with_ container arg must be `arr[i]` or `tab[k]` — use built-in `with` for locals; got {describe(containerExpr)}")
        let atNode = containerExpr as ExprAt
        macro_verify(containerExpr._type != null, prog, call.at,
            "with_ container element type not inferred yet — got null _type on {describe(containerExpr)}")
        macro_verify(atNode.subexpr._type != null, prog, call.at,
            "with_ container type not inferred yet — null _type on subexpr {describe(atNode.subexpr)}")
        let cKind = container_kind(atNode, prog)
        macro_verify(cKind != "", prog, call.at,
            "with_ container must be array or table, got {describe(atNode.subexpr._type)}")
        // Refuse workhorse-element containers: block-arg would be by-value
        // (block<(var x : TT)> with TT = workhorse like `int`), so mutation
        // wouldn't propagate. Direct `arr[i] = value` is the right tool.
        macro_verify(!containerExpr._type.isWorkhorseType, prog, call.at,
            "with_ on workhorse-element container is not supported (mutation would not propagate); use `arr[i] = value` directly")
        // Block handling:
        //   - User wrote 0 params → inject `_` of element type; needs rebuild.
        //   - User wrote N params → pass the user's block through verbatim;
        //     the helper's generic block sig + auto(TT) on the container arg
        //     drives type inference. Rewriting the params (even just renaming)
        //     loses information the typer needs.
        let mblk = blockArg as ExprMakeBlock
        let userBlock = mblk._block as ExprBlock
        let userArgCount = length(userBlock.arguments)
        var rewrittenBlock : ExprMakeBlock?
        var rewrittenInner : ExprBlock?
        if (userArgCount == 0) {
            macro_verify(containerCount == 1, prog, call.at,
                "with_ multi-arg form requires explicit block params: $(a, b, ...)")
            var pT = clone_type(call.arguments[0]._type)
            pT.flags.ref = false
            pT.flags.constant = false
            var injected = new Variable(at = call.at, name := "_", _type = pT)
            let hasReturn = block_returns_value(userBlock)
            var newBlock = new ExprBlock(at = userBlock.at,
                returnType = (hasReturn
                    ? new TypeDecl(at = userBlock.at, baseType = Type.autoinfer)
                    : new TypeDecl(at = userBlock.at, baseType = Type.tVoid)),
                blockFlags = userBlock.blockFlags | ExprBlockFlags.isClosure)
            newBlock.arguments |> emplace_new(injected)
            for (st in userBlock.list) {
                newBlock.list |> emplace_new(clone_expression(st))
            }
            rewrittenBlock = new ExprMakeBlock(at = call.at, _block = newBlock)
            rewrittenInner = newBlock
        } else {
            macro_verify(userArgCount == containerCount, prog, call.at,
                "with_ block-param count must match container-arg count: {containerCount} containers, {userArgCount} block params")
            rewrittenBlock = clone_expression(mblk) as ExprMakeBlock
            rewrittenInner = rewrittenBlock._block as ExprBlock
            // Strip const + force ref on each block param so `$(v)` (which
            // defaults to let-style const) gets `var v : T&` shape that matches
            // the helper's `block<(var x : TT) : void>`. Don't touch the
            // existing param's _type if user spelled it explicitly.
            for (i in range(containerCount)) {
                var arg = rewrittenInner.arguments[i]
                arg._type = clone_type(call.arguments[i]._type)
                arg._type.flags.ref = false
                arg._type.flags.constant = false
            }
        }
        // Single-arg dispatch
        if (containerCount == 1) {
            let returnsValue = block_returns_value(rewrittenInner)
            if (cKind == "array") {
                if (returnsValue) {
                    return <- qmacro(_with_locked_arr1_r($e(atNode.subexpr), $e(atNode.index), $e(rewrittenBlock)))
                } else {
                    return <- qmacro(_with_locked_arr1_v($e(atNode.subexpr), $e(atNode.index), $e(rewrittenBlock)))
                }
            } elif (cKind == "table") {
                if (returnsValue) {
                    return <- qmacro(_with_locked_tab1_r($e(atNode.subexpr), $e(atNode.index), $e(rewrittenBlock)))
                } else {
                    return <- qmacro(_with_locked_tab1_v($e(atNode.subexpr), $e(atNode.index), $e(rewrittenBlock)))
                }
            }
        }
        // Multi-arg dispatch (containerCount == 2 or 3, void only)
        macro_verify(!block_returns_value(rewrittenInner), prog, call.at,
            "with_ with return value is single-arg only; rewrite as nested single-arg with_")
        // Validate every additional arg is an array ExprAt (multi-table refused)
        for (i in range(containerCount)) {
            let ai = call.arguments[i]
            macro_verify(ai is ExprAt, prog, call.at,
                "with_ arg #{i} must be `arr[i]` (got {describe(ai)})")
            let aiNode = ai as ExprAt
            macro_verify(aiNode.subexpr._type != null, prog, call.at,
                "with_ arg #{i} container type not inferred yet")
            let ki = container_kind(aiNode, prog)
            macro_verify(ki == "array", prog, call.at,
                "with_ multi-arg form: arg #{i} must be an array (got {ki}). Tables allowed only in single-arg form.")
        }
        if (containerCount == 2) {
            let a0 = call.arguments[0] as ExprAt
            let a1 = call.arguments[1] as ExprAt
            return <- qmacro(_with_locked_arr2_v(
                $e(a0.subexpr), $e(a1.subexpr),
                $e(a0.index), $e(a1.index),
                $e(rewrittenBlock)))
        } else {  // containerCount == 3
            let a0 = call.arguments[0] as ExprAt
            let a1 = call.arguments[1] as ExprAt
            let a2 = call.arguments[2] as ExprAt
            return <- qmacro(_with_locked_arr3_v(
                $e(a0.subexpr), $e(a1.subexpr), $e(a2.subexpr),
                $e(a0.index), $e(a1.index), $e(a2.index),
                $e(rewrittenBlock)))
        }
    }
}
