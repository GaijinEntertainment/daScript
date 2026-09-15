options gen2
options indenting = 4
options strict_smart_pointers = true

module fold_rules shared public

//! Declarative fold rules — a LuaJIT-style peephole engine whose rules are written as daslang
//! source patterns instead of compiler code.
//!
//! A rule module subclasses ``FoldRules``, annotates the class ``[fold_rules]``, and writes one
//! ``qmatch`` ladder arm per rewrite; the arm body returns the replacement expression and ``null``
//! means "no fold". The engine runs as an ``[optimization_macro]``, so it sees the tree the C++
//! optimizer passes have already reduced (constants propagated, calls inlined, references folded),
//! and it types every replacement itself — there is no re-inference at that phase.
//!
//! ```
//! [fold_rules]
//! class GameFolds : FoldRules {
//!     def override fold(var expr : ExpressionPtr) : ExpressionPtr {
//!         qmatch (expr) {
//!             if (pow($e(x), 2.0)) { return qmacro($e(x) * $e(x)) }
//!             if (lerp($e(a), $e(b), 0.0)) { if (is_pure(b)) return a }
//!         }
//!         return null
//!     }
//! }
//! ```

require daslib/ast public
require daslib/ast_match public
require daslib/templates_boost public
require daslib/ast_boost
require daslib/macro_boost
require daslib/rtti
require daslib/flatten_opt_common

let private MAX_RETRY = 16
let private MAX_ROUNDS = 32

let private CONST_BASE_TYPE = {
    "ExprConstBool" => Type.tBool,
    "ExprConstInt" => Type.tInt,
    "ExprConstInt8" => Type.tInt8,
    "ExprConstInt16" => Type.tInt16,
    "ExprConstInt64" => Type.tInt64,
    "ExprConstUInt" => Type.tUInt,
    "ExprConstUInt8" => Type.tUInt8,
    "ExprConstUInt16" => Type.tUInt16,
    "ExprConstUInt64" => Type.tUInt64,
    "ExprConstFloat" => Type.tFloat,
    "ExprConstDouble" => Type.tDouble,
    "ExprConstString" => Type.tString,
    "ExprConstInt2" => Type.tInt2,
    "ExprConstInt3" => Type.tInt3,
    "ExprConstInt4" => Type.tInt4,
    "ExprConstUInt2" => Type.tUInt2,
    "ExprConstUInt3" => Type.tUInt3,
    "ExprConstUInt4" => Type.tUInt4,
    "ExprConstFloat2" => Type.tFloat2,
    "ExprConstFloat3" => Type.tFloat3,
    "ExprConstFloat4" => Type.tFloat4,
    "ExprConstRange" => Type.tRange,
    "ExprConstURange" => Type.tURange,
    "ExprConstRange64" => Type.tRange64,
    "ExprConstURange64" => Type.tURange64
}

class private NodeCounter : AstVisitor {
    count : int
    def override preVisitExpression(expr : ExpressionPtr) : void {
        count ++
    }
}

// ===== Rule-author helpers =====

def public is_pure(e : ExpressionPtr) : bool {
    //! True when `e` provably has no side effects, so a rule may drop or duplicate it.
    return !has_sideeffects(e)
}

def public same_pure(a, b : ExpressionPtr) : bool {
    //! True when `a` and `b` are the same pure expression, so a rule may collapse the pair.
    if (a == null || b == null) return false
    return !has_sideeffects(a) && describe(a) == describe(b)
}

def public type_of(var e : ExpressionPtr) : TypeDeclPtr {
    //! The expression's resolved type, or null before it has one.
    if (e == null) {
        return null
    }
    return e._type
}

def public base_type_of(e : ExpressionPtr) : Type {
    //! The expression's base type, `Type.none` when it has no resolved type.
    return expr_bt(e)
}

def public is_signed_family(bt : Type) : bool {
    //! True for the base types on which negation is defined.
    if (is_float_family(bt) || bt == Type.tDouble) return true
    return (bt == Type.tInt || bt == Type.tInt8 || bt == Type.tInt16 || bt == Type.tInt64 ||
        bt == Type.tInt2 || bt == Type.tInt3 || bt == Type.tInt4)
}

def public is_integer_family(bt : Type) : bool {
    //! True for the integer base types, where every arithmetic identity is exact.
    return (bt == Type.tInt || bt == Type.tInt8 || bt == Type.tInt16 || bt == Type.tInt64 ||
        bt == Type.tUInt || bt == Type.tUInt8 || bt == Type.tUInt16 || bt == Type.tUInt64 ||
        bt == Type.tInt2 || bt == Type.tInt3 || bt == Type.tInt4 ||
        bt == Type.tUInt2 || bt == Type.tUInt3 || bt == Type.tUInt4)
}

def public is_const_val(e : ExpressionPtr; want : double) : bool {
    //! True when `e` is a scalar or splat constant whose every lane equals `want`.
    return (is_real_val(e, want) || is_real_vec_val(e, want) ||
        is_int_val(e, int64(want)) || is_int_vec_val(e, int64(want)))
}

def public is_const(e : ExpressionPtr) : bool {
    //! True when `e` is already a constant node.
    return e != null && e |> is_expr_const()
}

def public typed_const(var k : ExpressionPtr; like : ExpressionPtr) : ExpressionPtr {
    //! Stamp a freshly built constant with the type and location of the expression it replaces.
    if (k == null || like == null) return k
    k.at = like.at
    if (like._type != null) {
        k._type = clone_type(like._type)
        k._type.flags.ref = false
        k._type.flags.constant = false
    }
    k.flags.constexpression = true
    return k
}

def public zero_like(e : ExpressionPtr) : ExpressionPtr {
    //! The typed zero constant of `e`'s type, null when that type has no constant form.
    if (e == null || e._type == null) return null
    return typed_const(zero_const_of(e._type, e.at), e)
}

def public fold_fast_math() : bool {
    //! True when the compiling module allows value-changing float rewrites. The program is read
    //! through the accessor each time: binding it to an `inscope` local releases a pointer the
    //! compiler still owns.
    return (compiling_program()._options |> find_arg("fast_math") ?as tBool ??
        compiling_program().policies.fast_math)
}

def public signed_zero_lax_ok(bt : Type) : bool {
    //! True when a sign-of-zero-only difference is acceptable for this base type.
    //! Mirrors the compiler's own laxity: float32 always, double only under fast_math.
    return bt == Type.tDouble ? fold_fast_math() : true
}

def public value_change_ok(bt : Type) : bool {
    //! True when a rewrite that changes inf/NaN results is acceptable for this base type.
    return is_float_family(bt) ? fold_fast_math() : true
}

def public impure(e : ExpressionPtr) : bool {
    //! Conservative side-effect test, one step less conservative than the shared one: a call to a
    //! function the compiler proved side-effect free is pure, where `has_sideeffects` trusts only
    //! builtins. Everything else it reports, this reports.
    if (e == null) return true
    if (!has_sideeffects(e)) return false
    if (!(e is ExprCall)) return true
    var c = e as ExprCall
    if (c.func == null || c.func.sideEffectFlags != FunctionSideEffectFlags(0)) return true
    for (a in c.arguments) {
        if (impure(a)) return true
    }
    return false
}

def public node_count(e : ExpressionPtr) : int {
    //! Expression nodes in the subtree, the size a duplication would pay for.
    if (e == null) return 0
    var v = new NodeCounter()
    make_visitor(*v) $(adapter) {
        visit(e, adapter)
    }
    let n = v.count
    unsafe {
        delete v
    }
    return n
}

def public repeat_budget() : int {
    //! How large an expression `_repeatable` will duplicate. `options _fold_repeat_budget = N`
    //! per module; the default keeps duplication to a handful of nodes.
    let opt = compiling_program()._options |> find_arg("_fold_repeat_budget")
    if (opt is tInt) {
        return opt as tInt
    }
    return 6
}

// ===== Pattern predicates =====
// A `_name(...)` in a pattern is a guard: fail it and the pattern does not match.

def public _droppable(e : ExpressionPtr) : bool {
    //! Safe to evaluate ZERO times: nothing observable happens in the expression, so a rule may drop
    //! it. A call qualifies when the compiler proved the callee side-effect free.
    return !impure(e)
}

def public _repeatable(e : ExpressionPtr) : bool {
    //! Safe to evaluate MORE than once: droppable, and small enough that duplicating it is not a
    //! pessimization. Purity alone is not enough - `pow(a[0]*a[1] + a[2]*a[3], 2)` is pure and
    //! doubling it costs four loads.
    return !impure(e) && node_count(e) <= repeat_budget()
}

def public _const(e : ExpressionPtr) : bool {
    //! The node is a constant.
    return is_const(e)
}

def public _not_const(e : ExpressionPtr) : bool {
    //! The node is not a constant - the other half of a canonicalization, which must fire in one
    //! direction only or the rule set never settles.
    return !is_const(e)
}

def public _same(e, other : ExpressionPtr) : bool {
    //! The same droppable expression as one the pattern already captured.
    return same_pure(e, other)
}

def public _bool_typed(e : ExpressionPtr) : bool {
    //! A boolean-typed node.
    return base_type_of(e) == Type.tBool
}

def public _int(e : ExpressionPtr) : bool {
    //! An integer-typed node, where every arithmetic identity is exact.
    return is_integer_family(base_type_of(e))
}

def public _signed(e : ExpressionPtr) : bool {
    //! A node whose type has negation.
    return is_signed_family(base_type_of(e))
}

def public _float(e : ExpressionPtr) : bool {
    //! A float-typed node - the one that can be NaN.
    return is_float_family(base_type_of(e))
}

def public _not_float(e : ExpressionPtr) : bool {
    //! A node that cannot be NaN, so self-comparison folds.
    return !is_float_family(base_type_of(e))
}

def public _value_safe(e : ExpressionPtr) : bool {
    //! A rewrite that changes an infinity or NaN result is allowed for this node's type.
    return value_change_ok(base_type_of(e))
}

def public _zero_sign_safe(e : ExpressionPtr) : bool {
    //! A rewrite whose only effect is the sign of a zero is allowed for this node's type.
    return signed_zero_lax_ok(base_type_of(e))
}

// ===== Typing a right-hand side =====

def private func_is_pure_builtin(f : FunctionPtr) : bool {
    if (f == null || !f.flags.builtIn || f.moreFlags.isTemplate) return false
    let se = f.sideEffectFlags
    return (!se._unsafe && !se.userScenario && !se.modifyExternal &&
        !se.modifyArgument && !se.accessGlobal && !se.invoke)
}

def private same_arg_type(l, r : TypeDeclPtr) : bool {
    if (l == null || r == null) return false
    return is_same_type(l, r, RefMatters.no, ConstMatters.no, TemporaryMatters.no)
}

def private is_fake_arg_type(t : TypeDeclPtr) : bool {
    if (t == null) return false
    return t.baseType == Type.fakeContext || t.baseType == Type.fakeLineInfo
}

def private result_type_of(f : FunctionPtr) : TypeDeclPtr {
    if (f == null || f.result == null) return null
    var td = clone_type(f.result)
    td.flags.ref = false
    td.flags.constant = false
    return td
}

def private resolve_pure_builtin(program : Module?; name : string; argc : int;
                                 t0, t1, t2 : TypeDeclPtr) : FunctionPtr {
    var found : FunctionPtr
    for_each_function(program, name) $(f) {
        if (found != null || !func_is_pure_builtin(f)) return
        var real = 0
        var ok = true
        for (a in f.arguments) {
            if (is_fake_arg_type(a._type)) continue
            let want = real == 0 ? t0 : (real == 1 ? t1 : t2)
            if (real >= argc || !same_arg_type(a._type, want)) {
                ok = false
                break
            }
            real ++
        }
        if (ok && real == argc) {
            found = f
        }
    }
    return found
}

def private find_pure_builtin(prog : Program?; name : string; argc : int;
                              t0, t1, t2 : TypeDeclPtr) : FunctionPtr {
    //! The one pure builtin named `name` whose declared argument types match. Mirrors the
    //! compiler's own `findBuiltinOperator`, extended to plain calls.
    if (t0 == null) return null
    var found : FunctionPtr
    for_each_module(prog) $(mod) {
        if (found == null) {
            found = resolve_pure_builtin(mod, name, argc, t0, t1, t2)
        }
    }
    return found
}

def private append_fake_args(var call : ExprCall?) {
    //! Re-attach the fakeContext / fakeLineInfo arguments inference would have appended.
    for (a in call.func.arguments) {
        if (a._type == null) continue
        if (a._type.baseType == Type.fakeContext) {
            call.arguments |> emplace_new <| new ExprFakeContext(at = call.at)
        } elif (a._type.baseType == Type.fakeLineInfo) {
            call.arguments |> emplace_new <| new ExprFakeLineInfo(at = call.at)
        }
    }
}

def private type_const(var e : ExpressionPtr) : bool {
    let bt = CONST_BASE_TYPE?["{e.__rtti}"] ?? Type.none
    if (bt == Type.none) return false
    e._type = new TypeDecl(baseType = bt, at = e.at)
    e.flags.constexpression = true
    return true
}

def private type_rewrite(prog : Program?; var e : ExpressionPtr) : string {   // nolint:STYLE037 - one arm per node class the right-hand-side grammar admits
    //! Fill `_type` and `func` on every untyped node of a freshly built replacement, answering ""
    //! on success and the reason otherwise. The grammar is constants, operators and pure-builtin
    //! calls; anything else is refused, so a partial tree never reaches the optimizer.
    if (e == null || e._type != null) return ""
    if (e |> is_expr_const()) {
        if (!type_const(e)) return "no base type for constant {e.__rtti}"
        return ""
    }
    if (e is ExprOp1) {
        var o = e as ExprOp1
        let sub = type_rewrite(prog, o.subexpr)
        if (!empty(sub)) return sub
        var f = find_pure_builtin(prog, "{o.op}", 1, type_of(o.subexpr), null, null)
        if (f == null) return "no pure builtin operator `{o.op}` for {describe(type_of(o.subexpr))}"
        o.func = f
        o._type = result_type_of(f)
        return ""
    }
    if (e is ExprOp2) {
        var o = e as ExprOp2
        let l = type_rewrite(prog, o.left)
        if (!empty(l)) return l
        let r = type_rewrite(prog, o.right)
        if (!empty(r)) return r
        var f = find_pure_builtin(prog, "{o.op}", 2, type_of(o.left), type_of(o.right), null)
        if (f == null) return "no pure builtin operator `{o.op}` for {describe(type_of(o.left))} and {describe(type_of(o.right))}"
        o.func = f
        o._type = result_type_of(f)
        return ""
    }
    if (e is ExprOp3) {
        var o = e as ExprOp3
        let c = type_rewrite(prog, o.subexpr)
        if (!empty(c)) return c
        let l = type_rewrite(prog, o.left)
        if (!empty(l)) return l
        let r = type_rewrite(prog, o.right)
        if (!empty(r)) return r
        if (base_type_of(o.subexpr) != Type.tBool) return "ternary condition is {describe(type_of(o.subexpr))}, not bool"
        if (!same_arg_type(type_of(o.left), type_of(o.right))) {
            return "ternary arms differ: {describe(type_of(o.left))} and {describe(type_of(o.right))}"
        }
        o._type = clone_type(type_of(o.left))
        o._type.flags.ref = false
        o._type.flags.constant = false
        return ""
    }
    if (e is ExprCall) {
        var c = e as ExprCall
        var ts : array<TypeDeclPtr>
        for (a in c.arguments) {
            let ae = type_rewrite(prog, a)
            if (!empty(ae)) return ae
            ts |> push(type_of(a))
        }
        let argc = length(ts)
        if (argc == 0 || argc > 3) {
            return "call `{c.name}` takes {argc} arguments; the rewrite grammar admits one to three"
        }
        var f = find_pure_builtin(prog, "{c.name}", argc, ts[0], argc > 1 ? ts[1] : null,
            argc > 2 ? ts[2] : null)
        if (f == null) return "no pure builtin `{c.name}` for those argument types"
        c.func = f
        c._type = result_type_of(f)
        append_fake_args(c)
        return ""
    }
    return "{e.__rtti} is not part of the fold-rule rewrite grammar"
}

// ===== The engine =====

class private FoldVisitor : AstVisitor {
    @do_not_delete owner : FoldRules?
    @do_not_delete prog : Program?
    changed : bool
    fires : int
    failed : bool
    errorText : string
    @safe_when_uninitialized errorAt : LineInfo

    // One hook per node class a source-syntax pattern can root at. The generic `visitExpression`
    // cannot serve: the adapter calls it for observation and discards its result (VISIT_EXPR in
    // include/daScript/simulate/aot_builtin_ast.h), so only a per-class hook can replace a node.
    def override visitExprOp1(var that : ExprOp1?) : ExpressionPtr { return fold_visit(unsafe(addr(self)), that) }
    def override visitExprOp2(var that : ExprOp2?) : ExpressionPtr { return fold_visit(unsafe(addr(self)), that) }
    def override visitExprOp3(var that : ExprOp3?) : ExpressionPtr { return fold_visit(unsafe(addr(self)), that) }
    def override visitExprCall(var that : ExprCall?) : ExpressionPtr { return fold_visit(unsafe(addr(self)), that) }
    def override visitExprInvoke(var that : ExprInvoke?) : ExpressionPtr { return fold_visit(unsafe(addr(self)), that) }
    def override visitExprCast(var that : ExprCast?) : ExpressionPtr { return fold_visit(unsafe(addr(self)), that) }
    def override visitExprSwizzle(var that : ExprSwizzle?) : ExpressionPtr { return fold_visit(unsafe(addr(self)), that) }
    def override visitExprStringBuilder(var that : ExprStringBuilder?) : ExpressionPtr { return fold_visit(unsafe(addr(self)), that) }
    def override visitExprNullCoalescing(var that : ExprNullCoalescing?) : ExpressionPtr { return fold_visit(unsafe(addr(self)), that) }
    def override visitExprAt(var that : ExprAt?) : ExpressionPtr { return fold_visit(unsafe(addr(self)), that) }
    def override visitExprSafeAt(var that : ExprSafeAt?) : ExpressionPtr { return fold_visit(unsafe(addr(self)), that) }
    def override visitExprField(var that : ExprField?) : ExpressionPtr { return fold_visit(unsafe(addr(self)), that) }
    def override visitExprSafeField(var that : ExprSafeField?) : ExpressionPtr { return fold_visit(unsafe(addr(self)), that) }
    def override visitExprIs(var that : ExprIs?) : ExpressionPtr { return fold_visit(unsafe(addr(self)), that) }
    def override visitExprIsVariant(var that : ExprIsVariant?) : ExpressionPtr { return fold_visit(unsafe(addr(self)), that) }
    def override visitExprAsVariant(var that : ExprAsVariant?) : ExpressionPtr { return fold_visit(unsafe(addr(self)), that) }
    def override visitExprSafeAsVariant(var that : ExprSafeAsVariant?) : ExpressionPtr { return fold_visit(unsafe(addr(self)), that) }
    def override visitExprVar(var that : ExprVar?) : ExpressionPtr { return fold_visit(unsafe(addr(self)), that) }
    def override visitExprTypeInfo(var that : ExprTypeInfo?) : ExpressionPtr { return fold_visit(unsafe(addr(self)), that) }
    def override visitExprAddr(var that : ExprAddr?) : ExpressionPtr { return fold_visit(unsafe(addr(self)), that) }
    def override visitExprNew(var that : ExprNew?) : ExpressionPtr { return fold_visit(unsafe(addr(self)), that) }
    def override visitExprMakeArray(var that : ExprMakeArray?) : ExpressionPtr { return fold_visit(unsafe(addr(self)), that) }
    def override visitExprMakeStruct(var that : ExprMakeStruct?) : ExpressionPtr { return fold_visit(unsafe(addr(self)), that) }
    def override visitExprMakeTuple(var that : ExprMakeTuple?) : ExpressionPtr { return fold_visit(unsafe(addr(self)), that) }
    def override visitExprKeyExists(var that : ExprKeyExists?) : ExpressionPtr { return fold_visit(unsafe(addr(self)), that) }
    def override visitExprFind(var that : ExprFind?) : ExpressionPtr { return fold_visit(unsafe(addr(self)), that) }
    def override visitExprIfThenElse(var that : ExprIfThenElse?) : ExpressionPtr { return fold_visit(unsafe(addr(self)), that) }
    def override visitExprWhile(var that : ExprWhile?) : ExpressionPtr { return fold_visit(unsafe(addr(self)), that) }
    def override visitExprFor(var that : ExprFor?) : ExpressionPtr { return fold_visit(unsafe(addr(self)), that) }
    def override visitExprAssert(var that : ExprAssert?) : ExpressionPtr { return fold_visit(unsafe(addr(self)), that) }
    def override visitExprReturn(var that : ExprReturn?) : ExpressionPtr { return fold_visit(unsafe(addr(self)), that) }
    def override visitExprCopy(var that : ExprCopy?) : ExpressionPtr { return fold_visit(unsafe(addr(self)), that) }
    def override visitExprMove(var that : ExprMove?) : ExpressionPtr { return fold_visit(unsafe(addr(self)), that) }
    def override visitExprClone(var that : ExprClone?) : ExpressionPtr { return fold_visit(unsafe(addr(self)), that) }

    def override visitExprBlockExpression(var blk : ExprBlock?; var expr : ExpressionPtr) : ExpressionPtr {
        //! A statement a rule replaced with an empty block is dropped outright.
        if (expr is ExprBlock) {
            let b = expr as ExprBlock
            if (empty(b.list) && empty(b.finalList) && empty(b.arguments) && b.genFlags.generated) {
                return null
            }
        }
        return expr
    }
}

def private fold_fail(var v : FoldVisitor?; text : string; at : LineInfo) {
    if (!v.failed) {
        v.failed = true
        v.errorText = text
        v.errorAt = at
    }
}

def private fold_visit(var v : FoldVisitor?; var expr : ExpressionPtr) : ExpressionPtr {
    //! One node: apply the rules, type what they built, and RETRY on the result so a chain of
    //! rewrites settles before the walk moves on.
    if (v.failed || expr == null || v.owner == null) return expr
    var cur = expr
    var steps = 0
    while (steps < MAX_RETRY) {
        var next = v.owner->fold(cur)
        if (next == null || next == cur) return cur
        let err = type_rewrite(v.prog, next)
        if (!empty(err)) {
            fold_fail(v, "fold rule rewrote `{describe(cur)}` into `{describe(next)}`, which the engine cannot type: {err}", cur.at)
            return cur
        }
        stamp_missing_at(next, cur.at)
        if (type_of(cur) != null && type_of(next) != null &&
                !same_arg_type(type_of(cur), type_of(next))) {
            fold_fail(v, "fold rule changed the type of `{describe(cur)}` from {describe(type_of(cur))} to {describe(type_of(next))}", cur.at)
            return cur
        }
        v.changed = true
        v.fires ++
        cur = next
        steps ++
    }
    fold_fail(v, "fold rules do not converge on `{describe(expr)}` after {MAX_RETRY} rewrites", expr.at)
    return cur
}

def public drop_statement(at : LineInfo) : ExpressionPtr {
    //! The replacement a rule returns to delete the statement it matched. Only legal where the
    //! matched expression IS a statement of a block; anywhere else the engine reports it.
    var b = new ExprBlock(at = at)
    b.genFlags.generated = true
    return b
}

class FoldRules : AstPassMacro {
    //! Base class for a declarative fold-rule pass. Subclass, annotate `[fold_rules]`, and override
    //! `fold`: return the replacement expression, or null to leave the node alone. The returned
    //! tree may use captured subtrees plus constants, operators and calls to pure builtins.
    fires : int
    round : int
    moduleName : string

    def abstract fold(var expr : ExpressionPtr) : ExpressionPtr

    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        var thisMod = compiling_module()
        if (thisMod == null || mod == null) return false
        let modName = "{thisMod.name}"
        if (modName != moduleName) {
            moduleName = modName
            round = 0
            fires = 0
        }
        if (!fold_option(prog, "_fold_rules", true)) return false
        round ++
        if (round > MAX_ROUNDS) {
            macro_error(prog, LineInfo(), "fold rules did not settle on module `{modName}` after {MAX_ROUNDS} rounds ({fires} rewrites)")
            return false
        }
        let logging = fold_option(prog, "_fold_rules_log", false)
        var anyChanged = false
        var v = new FoldVisitor()
        unsafe {
            v.owner = addr<FoldRules?>(self)
            v.prog = addr<Program?>(*prog)
        }
        for_each_function(thisMod, "") $(var func : FunctionPtr) {
            if (v.failed || func.flags.generated && func.body == null) return
            let before = v.fires
            make_visitor(*v) $(adapter) {
                visit(func.body, adapter)
            }
            if (v.fires != before) {
                if (func.optimizationRound < round) {
                    func.optimizationRound = round
                }
                anyChanged = true
                if (logging) {
                    to_log(LOG_INFO, "[fold_rules] {modName}::{func.name}: {v.fires - before} rewrite(s) in round {round}\n")
                }
            }
        }
        if (v.failed) {
            macro_error(prog, v.errorAt, "{v.errorText}")
            anyChanged = false
        }
        fires += v.fires
        unsafe {
            delete v
        }
        if (logging && anyChanged) {
            to_log(LOG_INFO, "[fold_rules] {modName}: round {round}, {fires} rewrite(s) total\n")
        }
        return anyChanged
    }
}

def private fold_option(prog : ProgramPtr; name : string; dflt : bool) : bool {
    return prog._options |> find_arg(name) ?as tBool ?? dflt
}

class SetupFoldRules : SetupAnyAnnotation {
    //! Registers an annotated `FoldRules` subclass as an optimization pass macro.
    override annotation_function_call : string = "add_new_optimization_macro"
}

[_macro]
def private setup {
    if (is_compiling_macros_in_module("fold_rules")) {
        add_new_structure_annotation("fold_rules", new SetupFoldRules())
    }
}
