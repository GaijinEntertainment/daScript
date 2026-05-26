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
//! * Block bodies are void (no ``return`` value); ``with_`` is for in-place
//!   mutation, not value-extraction. To compute a value, write to a
//!   local: ``var v : T; with_(arr[0]) { v = _.f }``.
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
                               b : block<(var x : TT&) : void>) {
    __builtin_array_lock_mutable(a)
    invoke(b, a[idx])
} finally {
    __builtin_array_unlock_mutable(a)
}

def public _with_locked_tab1_v(
                               var t : table<auto(KT); auto(VT)>; key : KT;
                               b : block<(var x : VT&) : void>) {
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

def public _with_locked_arr2_v(
                               var a1 : array<auto(T1)>; var a2 : array<auto(T2)>;
                               i1 : int; i2 : int;
                               b : block<(var x : T1&; var y : T2&) : void>) {
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
                               b : block<(var x : T1&; var y : T2&; var z : T3&) : void>) {
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

def private container_kind(atNode : ExprAt?) : string {
    //! Returns "array", "table", or "" if neither.
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
        let cKind = container_kind(atNode)
        macro_verify(cKind != "", prog, call.at,
            "with_ container must be array or table, got {describe(atNode.subexpr._type)}")
        // Block handling. Every block param goes out as `var name : auto`;
        // the helper's `block<(var x : TT&) : void>` signature drives type
        // inference. This keeps the macro emission minimal — daslang sees
        // the same shape it would see if the user had hand-written the
        // helper call and the typer infers `TT&` for every binding,
        // workhorse or struct alike.
        let mblk = blockArg as ExprMakeBlock
        let userBlock = mblk._block as ExprBlock
        let userArgCount = length(userBlock.arguments)
        var rewrittenBlock : ExprMakeBlock?
        var rewrittenInner : ExprBlock?
        if (userArgCount == 0) {
            macro_verify(containerCount == 1, prog, call.at,
                "with_ multi-arg form requires explicit block params: $(a, b, ...)")
            // Match what the parser produces for `$(a)` (no type): a
            // TypeDecl(autoinfer) with ref=false, removeConstant=true.
            // null _type crashes Variable::clone; auto without
            // removeConstant trips the typer's generic instantiation.
            var pT = new TypeDecl(at = call.at, baseType = Type.autoinfer)
            pT.flags.ref = false
            pT.flags.removeConstant = true
            var injected = new Variable(at = call.at, name := "_", _type = pT)
            var newBlock = new ExprBlock(at = userBlock.at,
                returnType = new TypeDecl(at = userBlock.at, baseType = Type.tVoid),
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
            // Reset each user-named param to what the parser produces for
            // `$(a)` (autoinfer + ref=false + removeConstant=true) so the
            // helper's `block<(var x : TT&) : void>` signature drives
            // inference the same way a direct hand-written call does.
            for (i in range(containerCount)) {
                var arg = rewrittenInner.arguments[i]
                arg._type = new TypeDecl(at = arg.at, baseType = Type.autoinfer)
                arg._type.flags.ref = false
                arg._type.flags.removeConstant = true
            }
        }
        // Single-arg dispatch
        if (containerCount == 1) {
            return <- qmacro(_with_locked_arr1_v($e(atNode.subexpr), $e(atNode.index), $e(rewrittenBlock))) if (cKind == "array")
            return <- qmacro(_with_locked_tab1_v($e(atNode.subexpr), $e(atNode.index), $e(rewrittenBlock))) if (cKind == "table")
        }
        // Multi-arg dispatch (containerCount == 2 or 3): validate every arg
        // is an array ExprAt (multi-table refused).
        for (i in range(containerCount)) {
            let ai = call.arguments[i]
            macro_verify(ai is ExprAt, prog, call.at,
                "with_ arg #{i} must be `arr[i]` (got {describe(ai)})")
            let aiNode = ai as ExprAt
            macro_verify(aiNode.subexpr._type != null, prog, call.at,
                "with_ arg #{i} container type not inferred yet")
            let ki = container_kind(aiNode)
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
