options gen2

module smt_expr shared

require smt/daslib/smt_boost public

require daslib/safe_addr
require daslib/strings_boost
require strings

//! Typed symbolic values over the Z3 C API — Z3py-style ergonomics.
//! Arithmetic and comparisons are plain das operators; `&&`/`||`/`!` are not
//! overloadable in das, so use `s_and`/`s_or`/`s_not` (or the `constrain`
//! macro from smt/daslib/smt_macro, which rewrites them).

//! A symbolic boolean expression.
struct public SBool {
    ast : Z3_ast const
    ctx : Z3_context const
}

//! A symbolic integer (SMT Int — unbounded).
struct public SInt {
    ast : Z3_ast const
    ctx : Z3_context const
}

//! A symbolic real (SMT Real — exact rational, not IEEE float).
struct public SReal {
    ast : Z3_ast const
    ctx : Z3_context const
}

//! A symbolic string (SMT sequence of characters).
struct public SString {
    ast : Z3_ast const
    ctx : Z3_context const
}

//! Result of `check`.
enum public SmtResult {
    UNSAT
    UNKNOWN
    SAT
}

//! Solver handle; create with `make_solver` or scoped `with_solver`.
struct public Solver {
    ctx : Z3_context const
    solver : Z3_solver
    owns_ctx : bool
}

//! Model handle; obtain via `get_model` / `with_model`.
struct public Model {
    ctx : Z3_context const
    model : Z3_model const
}

// ===== context & solver lifecycle =====

//! Solver owning a private context (released by `release`).
def public make_solver : Solver {
    var ctx = z3_create_context()
    var sol = Z3_mk_solver(ctx)
    Z3_solver_inc_ref(ctx, sol)
    return Solver(ctx = ctx, solver = sol, owns_ctx = true)
}

//! Solver on an explicit context (context stays caller-owned).
def public make_solver(ctx : Z3_context) : Solver {
    var sol = Z3_mk_solver(ctx)
    Z3_solver_inc_ref(ctx, sol)
    return Solver(ctx = ctx, solver = sol, owns_ctx = false)
}

def public release(var s : Solver) {
    if (s.solver != null) {
        Z3_solver_dec_ref(s.ctx, s.solver)
        if (s.owns_ctx) {
            Z3_del_context(s.ctx)
        }
        s.solver = null
    }
}

//! Scoped solver on a private context.
def public with_solver(blk : block<(var s : Solver) : void>) {
    var ctx = z3_create_context()
    var s = Solver(ctx = ctx, solver = Z3_mk_solver(ctx), owns_ctx = true)
    Z3_solver_inc_ref(ctx, s.solver)
    invoke(blk, s)
    release(s)
}

def public check_error(s : Solver) {
    z3_check_error(s.ctx)
}

// ===== constants (unknowns) =====

def public int_const(s : Solver; name : string) : SInt => int_const(s.ctx, name)

def public int_const(ctx : Z3_context; name : string) : SInt =>
    SInt(ast = Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, name), Z3_mk_int_sort(ctx)), ctx = ctx)

//! Batch form: `int_consts(s, "x y z")` (Z3py `Ints`).
def public int_consts(s : Solver; names : string) : array<SInt> =>
    <- [for (n in split(names, " ")); int_const(s.ctx, n)]

def public bool_const(s : Solver; name : string) : SBool => bool_const(s.ctx, name)

def public bool_const(ctx : Z3_context; name : string) : SBool =>
    SBool(ast = Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, name), Z3_mk_bool_sort(ctx)), ctx = ctx)

def public real_const(s : Solver; name : string) : SReal => real_const(s.ctx, name)

def public real_const(ctx : Z3_context; name : string) : SReal =>
    SReal(ast = Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, name), Z3_mk_real_sort(ctx)), ctx = ctx)

def public string_const(s : Solver; name : string) : SString => string_const(s.ctx, name)

def public string_const(ctx : Z3_context; name : string) : SString =>
    SString(ast = Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, name), Z3_mk_string_sort(ctx)), ctx = ctx)

// ===== values (numerals & literals) =====

def public int_val(ctx : Z3_context; v : int) : SInt => SInt(ast = z3_int(ctx, v), ctx = ctx)

def public real_val(ctx : Z3_context; v : double) : SReal {
    // exact when v is integral; fractional doubles go through a scaled rational
    let num = int64(v * 1000000.0lf)
    return SReal(ast = Z3_mk_real_int64(ctx, num, 1000000l), ctx = ctx)
}

def public bool_val(ctx : Z3_context; v : bool) : SBool =>
    SBool(ast = v ? Z3_mk_true(ctx) : Z3_mk_false(ctx), ctx = ctx)

def public string_val(ctx : Z3_context; v : string) : SString =>
    SString(ast = Z3_mk_string(ctx, v), ctx = ctx)

// handle-const laundering for array literals (das arrays can't hold const
// pointers); the pointees are opaque to das — never dereferenced, so the
// const strip cannot license a miscompile
def private dc(p : Z3_ast const) : Z3_ast => unsafe(reinterpret<Z3_ast>(p))

// ===== SInt arithmetic =====

def public operator +(a, b : SInt) : SInt {
    var args <- [dc(a.ast), dc(b.ast)]
    return SInt(ast = Z3_mk_add(a.ctx, args), ctx = a.ctx)
}

def public operator +(a : SInt; b : int) : SInt => a + int_val(a.ctx, b)

def public operator +(a : int; b : SInt) : SInt => int_val(b.ctx, a) + b

def public operator -(a, b : SInt) : SInt {
    var args <- [dc(a.ast), dc(b.ast)]
    return SInt(ast = Z3_mk_sub(a.ctx, args), ctx = a.ctx)
}

def public operator -(a : SInt; b : int) : SInt => a - int_val(a.ctx, b)

def public operator -(a : int; b : SInt) : SInt => int_val(b.ctx, a) - b

def public operator *(a, b : SInt) : SInt {
    var args <- [dc(a.ast), dc(b.ast)]
    return SInt(ast = Z3_mk_mul(a.ctx, args), ctx = a.ctx)
}

def public operator *(a : SInt; b : int) : SInt => a * int_val(a.ctx, b)

def public operator *(a : int; b : SInt) : SInt => int_val(b.ctx, a) * b

def public operator /(a, b : SInt) : SInt => SInt(ast = Z3_mk_div(a.ctx, a.ast, b.ast), ctx = a.ctx)

def public operator /(a : SInt; b : int) : SInt => a / int_val(a.ctx, b)

def public operator %(a, b : SInt) : SInt => SInt(ast = Z3_mk_mod(a.ctx, a.ast, b.ast), ctx = a.ctx)

def public operator %(a : SInt; b : int) : SInt => a % int_val(a.ctx, b)

def public operator -(a : SInt) : SInt => SInt(ast = Z3_mk_unary_minus(a.ctx, a.ast), ctx = a.ctx)

// ===== SInt comparisons =====

def public operator ==(a, b : SInt) : SBool =>
    SBool(ast = Z3_mk_eq(a.ctx, a.ast, b.ast), ctx = a.ctx)

def public operator ==(a : SInt; b : int) : SBool => a == int_val(a.ctx, b)

def public operator ==(a : int; b : SInt) : SBool => int_val(b.ctx, a) == b

def public operator !=(a, b : SInt) : SBool => s_not(a == b)

def public operator !=(a : SInt; b : int) : SBool => s_not(a == b)

def public operator !=(a : int; b : SInt) : SBool => s_not(a == b)

def public operator <(a, b : SInt) : SBool =>
    SBool(ast = Z3_mk_lt(a.ctx, a.ast, b.ast), ctx = a.ctx)

def public operator <(a : SInt; b : int) : SBool => a < int_val(a.ctx, b)

def public operator <(a : int; b : SInt) : SBool => int_val(b.ctx, a) < b

def public operator <=(a, b : SInt) : SBool =>
    SBool(ast = Z3_mk_le(a.ctx, a.ast, b.ast), ctx = a.ctx)

def public operator <=(a : SInt; b : int) : SBool => a <= int_val(a.ctx, b)

def public operator <=(a : int; b : SInt) : SBool => int_val(b.ctx, a) <= b

def public operator >(a, b : SInt) : SBool =>
    SBool(ast = Z3_mk_gt(a.ctx, a.ast, b.ast), ctx = a.ctx)

def public operator >(a : SInt; b : int) : SBool => a > int_val(a.ctx, b)

def public operator >(a : int; b : SInt) : SBool => int_val(b.ctx, a) > b

def public operator >=(a, b : SInt) : SBool =>
    SBool(ast = Z3_mk_ge(a.ctx, a.ast, b.ast), ctx = a.ctx)

def public operator >=(a : SInt; b : int) : SBool => a >= int_val(a.ctx, b)

def public operator >=(a : int; b : SInt) : SBool => int_val(b.ctx, a) >= b

// ===== SReal arithmetic & comparisons =====

def public to_real(a : SInt) : SReal => SReal(ast = Z3_mk_int2real(a.ctx, a.ast), ctx = a.ctx)

def public operator +(a, b : SReal) : SReal {
    var args <- [dc(a.ast), dc(b.ast)]
    return SReal(ast = Z3_mk_add(a.ctx, args), ctx = a.ctx)
}

def public operator +(a : SReal; b : double) : SReal => a + real_val(a.ctx, b)

def public operator +(a : double; b : SReal) : SReal => real_val(b.ctx, a) + b

def public operator -(a, b : SReal) : SReal {
    var args <- [dc(a.ast), dc(b.ast)]
    return SReal(ast = Z3_mk_sub(a.ctx, args), ctx = a.ctx)
}

def public operator -(a : SReal; b : double) : SReal => a - real_val(a.ctx, b)

def public operator -(a : double; b : SReal) : SReal => real_val(b.ctx, a) - b

def public operator *(a, b : SReal) : SReal {
    var args <- [dc(a.ast), dc(b.ast)]
    return SReal(ast = Z3_mk_mul(a.ctx, args), ctx = a.ctx)
}

def public operator *(a : SReal; b : double) : SReal => a * real_val(a.ctx, b)

def public operator *(a : double; b : SReal) : SReal => real_val(b.ctx, a) * b

def public operator /(a, b : SReal) : SReal =>
    SReal(ast = Z3_mk_div(a.ctx, a.ast, b.ast), ctx = a.ctx)

def public operator -(a : SReal) : SReal =>
    SReal(ast = Z3_mk_unary_minus(a.ctx, a.ast), ctx = a.ctx)

def public operator ==(a, b : SReal) : SBool =>
    SBool(ast = Z3_mk_eq(a.ctx, a.ast, b.ast), ctx = a.ctx)

def public operator ==(a : SReal; b : double) : SBool => a == real_val(a.ctx, b)

def public operator <(a, b : SReal) : SBool =>
    SBool(ast = Z3_mk_lt(a.ctx, a.ast, b.ast), ctx = a.ctx)

def public operator <(a : SReal; b : double) : SBool => a < real_val(a.ctx, b)

def public operator <(a : double; b : SReal) : SBool => real_val(b.ctx, a) < b

def public operator <=(a, b : SReal) : SBool =>
    SBool(ast = Z3_mk_le(a.ctx, a.ast, b.ast), ctx = a.ctx)

def public operator <=(a : SReal; b : double) : SBool => a <= real_val(a.ctx, b)

def public operator >(a, b : SReal) : SBool =>
    SBool(ast = Z3_mk_gt(a.ctx, a.ast, b.ast), ctx = a.ctx)

def public operator >(a : SReal; b : double) : SBool => a > real_val(a.ctx, b)

def public operator >=(a, b : SReal) : SBool =>
    SBool(ast = Z3_mk_ge(a.ctx, a.ast, b.ast), ctx = a.ctx)

def public operator >=(a : SReal; b : double) : SBool => a >= real_val(a.ctx, b)

// mixed SInt ⊗ SReal — int side promoted via Z3_mk_int2real
def public operator <(a : SInt; b : SReal) : SBool => to_real(a) < b

def public operator <(a : SReal; b : SInt) : SBool => a < to_real(b)

def public operator <=(a : SInt; b : SReal) : SBool => to_real(a) <= b

def public operator <=(a : SReal; b : SInt) : SBool => a <= to_real(b)

def public operator >(a : SInt; b : SReal) : SBool => to_real(a) > b

def public operator >(a : SReal; b : SInt) : SBool => a > to_real(b)

def public operator ==(a : SInt; b : SReal) : SBool => to_real(a) == b

def public operator ==(a : SReal; b : SInt) : SBool => a == to_real(b)

// ===== SString =====

def public operator ==(a, b : SString) : SBool =>
    SBool(ast = Z3_mk_eq(a.ctx, a.ast, b.ast), ctx = a.ctx)

def public operator ==(a : SString; b : string) : SBool => a == string_val(a.ctx, b)

def public operator ==(a : string; b : SString) : SBool => string_val(b.ctx, a) == b

def public operator +(a, b : SString) : SString {
    var args <- [dc(a.ast), dc(b.ast)]
    return SString(ast = Z3_mk_seq_concat(a.ctx, uint(length(args)), array_data_ptr(args)), ctx = a.ctx)
}

def public operator +(a : SString; b : string) : SString => a + string_val(a.ctx, b)

def public length(a : SString) : SInt => SInt(ast = Z3_mk_seq_length(a.ctx, a.ast), ctx = a.ctx)

def public contains(a : SString; b : SString) : SBool =>
    SBool(ast = Z3_mk_seq_contains(a.ctx, a.ast, b.ast), ctx = a.ctx)

def public contains(a : SString; b : string) : SBool => contains(a, string_val(a.ctx, b))

def public starts_with(a : SString; b : SString) : SBool =>
    SBool(ast = Z3_mk_seq_prefix(a.ctx, b.ast, a.ast), ctx = a.ctx)

def public starts_with(a : SString; b : string) : SBool => starts_with(a, string_val(a.ctx, b))

// ===== boolean connectives (das can't overload && / || / !) =====

def public s_and(a, b : SBool) : SBool {
    var args <- [dc(a.ast), dc(b.ast)]
    return SBool(ast = Z3_mk_and(a.ctx, args), ctx = a.ctx)
}

def public s_and(a : SBool; b : bool) : SBool => s_and(a, bool_val(a.ctx, b))

def public s_and(a : bool; b : SBool) : SBool => s_and(bool_val(b.ctx, a), b)

def public s_or(a, b : SBool) : SBool {
    var args <- [dc(a.ast), dc(b.ast)]
    return SBool(ast = Z3_mk_or(a.ctx, args), ctx = a.ctx)
}

def public s_or(a : SBool; b : bool) : SBool => s_or(a, bool_val(a.ctx, b))

def public s_or(a : bool; b : SBool) : SBool => s_or(bool_val(b.ctx, a), b)

def public s_not(a : SBool) : SBool => SBool(ast = Z3_mk_not(a.ctx, a.ast), ctx = a.ctx)

def public implies(a, b : SBool) : SBool =>
    SBool(ast = Z3_mk_implies(a.ctx, a.ast, b.ast), ctx = a.ctx)

def public iff(a, b : SBool) : SBool => SBool(ast = Z3_mk_iff(a.ctx, a.ast, b.ast), ctx = a.ctx)

def public operator ==(a, b : SBool) : SBool => iff(a, b)

def public s_all(var conds : array<SBool>) : SBool {
    assert(!empty(conds))
    var args <- [for (c in conds); dc(c.ast)]
    return SBool(ast = Z3_mk_and(conds[0].ctx, args), ctx = conds[0].ctx)
}

def public s_any(var conds : array<SBool>) : SBool {
    assert(!empty(conds))
    var args <- [for (c in conds); dc(c.ast)]
    return SBool(ast = Z3_mk_or(conds[0].ctx, args), ctx = conds[0].ctx)
}

def public distinct(var vals : array<SInt>) : SBool {
    assert(!empty(vals))
    var args <- [for (v in vals); dc(v.ast)]
    return SBool(ast = Z3_mk_distinct(vals[0].ctx, args), ctx = vals[0].ctx)
}

// ===== ite & the symbolic-generics enablers =====

def public ite(c : SBool; t, f : SInt) : SInt =>
    SInt(ast = Z3_mk_ite(c.ctx, c.ast, t.ast, f.ast), ctx = c.ctx)

def public ite(c : SBool; t : SInt; f : int) : SInt => ite(c, t, int_val(c.ctx, f))

def public ite(c : SBool; t : int; f : SInt) : SInt => ite(c, int_val(c.ctx, t), f)

def public ite(c : SBool; t, f : SReal) : SReal =>
    SReal(ast = Z3_mk_ite(c.ctx, c.ast, t.ast, f.ast), ctx = c.ctx)

def public ite(c : SBool; t, f : SBool) : SBool =>
    SBool(ast = Z3_mk_ite(c.ctx, c.ast, t.ast, f.ast), ctx = c.ctx)

//! `abs`/`min`/`max` let ordinary generic das functions instantiate with
//! symbolic arguments — `def chebyshev(ax, ay, bx, by) => max(abs(ax - bx), abs(ay - by))`
//! builds a formula when called with SInt.
def public abs(a : SInt) : SInt => ite(a >= 0, a, -a)

def public min(a, b : SInt) : SInt => ite(a <= b, a, b)

def public max(a, b : SInt) : SInt => ite(a >= b, a, b)

// ===== SBv8/SBv16/SBv32/SBv64 - bitvectors (wrap-around machine arithmetic) =====
// width is part of the das type: each operator family is written once over the OR-type of all widths, and
// `concept_assert` rejects a width mismatch at the call site; comparisons are unsigned (s_lt/s_le/s_gt/s_ge signed, sar the arithmetic shift)

//! A symbolic 8-bit bitvector.
struct public SBv8 {
    ast : Z3_ast const
    ctx : Z3_context const
}

//! A symbolic 16-bit bitvector.
struct public SBv16 {
    ast : Z3_ast const
    ctx : Z3_context const
}

//! A symbolic 32-bit bitvector.
struct public SBv32 {
    ast : Z3_ast const
    ctx : Z3_context const
}

//! A symbolic 64-bit bitvector.
struct public SBv64 {
    ast : Z3_ast const
    ctx : Z3_context const
}

def public bv8_const(s : Solver; name : string) : SBv8 => bv8_const(s.ctx, name)

def public bv8_const(ctx : Z3_context; name : string) : SBv8 =>
    SBv8(ast = Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, name), Z3_mk_bv_sort(ctx, 8u)), ctx = ctx)

def public bv8_val(ctx : Z3_context; v : uint) : SBv8 =>
    SBv8(ast = Z3_mk_unsigned_int64(ctx, uint64(v), Z3_mk_bv_sort(ctx, 8u)), ctx = ctx)

def private mk_bv(_sample : SBv8; ast : Z3_ast; ctx : Z3_context) : SBv8 =>
    SBv8(ast = ast, ctx = ctx)

def public bv16_const(s : Solver; name : string) : SBv16 => bv16_const(s.ctx, name)

def public bv16_const(ctx : Z3_context; name : string) : SBv16 =>
    SBv16(ast = Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, name), Z3_mk_bv_sort(ctx, 16u)), ctx = ctx)

def public bv16_val(ctx : Z3_context; v : uint) : SBv16 =>
    SBv16(ast = Z3_mk_unsigned_int64(ctx, uint64(v), Z3_mk_bv_sort(ctx, 16u)), ctx = ctx)

def private mk_bv(_sample : SBv16; ast : Z3_ast; ctx : Z3_context) : SBv16 =>
    SBv16(ast = ast, ctx = ctx)

def public bv32_const(s : Solver; name : string) : SBv32 => bv32_const(s.ctx, name)

def public bv32_const(ctx : Z3_context; name : string) : SBv32 =>
    SBv32(ast = Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, name), Z3_mk_bv_sort(ctx, 32u)), ctx = ctx)

def public bv32_val(ctx : Z3_context; v : uint) : SBv32 =>
    SBv32(ast = Z3_mk_unsigned_int64(ctx, uint64(v), Z3_mk_bv_sort(ctx, 32u)), ctx = ctx)

def private mk_bv(_sample : SBv32; ast : Z3_ast; ctx : Z3_context) : SBv32 =>
    SBv32(ast = ast, ctx = ctx)

def public bv64_const(s : Solver; name : string) : SBv64 => bv64_const(s.ctx, name)

def public bv64_const(ctx : Z3_context; name : string) : SBv64 =>
    SBv64(ast = Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, name), Z3_mk_bv_sort(ctx, 64u)), ctx = ctx)

def public bv64_val(ctx : Z3_context; v : uint64) : SBv64 =>
    SBv64(ast = Z3_mk_unsigned_int64(ctx, v, Z3_mk_bv_sort(ctx, 64u)), ctx = ctx)

def private mk_bv(_sample : SBv64; ast : Z3_ast; ctx : Z3_context) : SBv64 =>
    SBv64(ast = ast, ctx = ctx)

// numeral of the same width as `a` — width read from the Z3 sort, so this is generic
def private bv_num(a : SBv8 | SBv16 | SBv32 | SBv64; v : uint64) =>
    mk_bv(a, Z3_mk_unsigned_int64(a.ctx, v, Z3_get_sort(a.ctx, a.ast)), a.ctx)

def public operator +(a, b : SBv8 | SBv16 | SBv32 | SBv64) {
    static_if (typeinfo stripped_typename(a) != typeinfo stripped_typename(b)) {
        concept_assert(false, "bitvector width mismatch")
        return mk_bv(a, a.ast, a.ctx)
    } else {
        return mk_bv(a, Z3_mk_bvadd(a.ctx, a.ast, b.ast), a.ctx)
    }
}

def public operator +(a : SBv8 | SBv16 | SBv32 | SBv64; b : uint) => a + bv_num(a, uint64(b))

def public operator +(a : SBv8 | SBv16 | SBv32 | SBv64; b : uint64) => a + bv_num(a, b)

def public operator -(a, b : SBv8 | SBv16 | SBv32 | SBv64) {
    static_if (typeinfo stripped_typename(a) != typeinfo stripped_typename(b)) {
        concept_assert(false, "bitvector width mismatch")
        return mk_bv(a, a.ast, a.ctx)
    } else {
        return mk_bv(a, Z3_mk_bvsub(a.ctx, a.ast, b.ast), a.ctx)
    }
}

def public operator -(a : SBv8 | SBv16 | SBv32 | SBv64; b : uint) => a - bv_num(a, uint64(b))

def public operator -(a : SBv8 | SBv16 | SBv32 | SBv64; b : uint64) => a - bv_num(a, b)

def public operator *(a, b : SBv8 | SBv16 | SBv32 | SBv64) {
    static_if (typeinfo stripped_typename(a) != typeinfo stripped_typename(b)) {
        concept_assert(false, "bitvector width mismatch")
        return mk_bv(a, a.ast, a.ctx)
    } else {
        return mk_bv(a, Z3_mk_bvmul(a.ctx, a.ast, b.ast), a.ctx)
    }
}

def public operator *(a : SBv8 | SBv16 | SBv32 | SBv64; b : uint) => a * bv_num(a, uint64(b))

def public operator *(a : SBv8 | SBv16 | SBv32 | SBv64; b : uint64) => a * bv_num(a, b)

def public operator &(a, b : SBv8 | SBv16 | SBv32 | SBv64) {
    static_if (typeinfo stripped_typename(a) != typeinfo stripped_typename(b)) {
        concept_assert(false, "bitvector width mismatch")
        return mk_bv(a, a.ast, a.ctx)
    } else {
        return mk_bv(a, Z3_mk_bvand(a.ctx, a.ast, b.ast), a.ctx)
    }
}

def public operator &(a : SBv8 | SBv16 | SBv32 | SBv64; b : uint) => a & bv_num(a, uint64(b))

def public operator &(a : SBv8 | SBv16 | SBv32 | SBv64; b : uint64) => a & bv_num(a, b)

def public operator |(a, b : SBv8 | SBv16 | SBv32 | SBv64) {
    static_if (typeinfo stripped_typename(a) != typeinfo stripped_typename(b)) {
        concept_assert(false, "bitvector width mismatch")
        return mk_bv(a, a.ast, a.ctx)
    } else {
        return mk_bv(a, Z3_mk_bvor(a.ctx, a.ast, b.ast), a.ctx)
    }
}

def public operator |(a : SBv8 | SBv16 | SBv32 | SBv64; b : uint) => a | bv_num(a, uint64(b))

def public operator |(a : SBv8 | SBv16 | SBv32 | SBv64; b : uint64) => a | bv_num(a, b)

def public operator ^(a, b : SBv8 | SBv16 | SBv32 | SBv64) {
    static_if (typeinfo stripped_typename(a) != typeinfo stripped_typename(b)) {
        concept_assert(false, "bitvector width mismatch")
        return mk_bv(a, a.ast, a.ctx)
    } else {
        return mk_bv(a, Z3_mk_bvxor(a.ctx, a.ast, b.ast), a.ctx)
    }
}

def public operator ^(a : SBv8 | SBv16 | SBv32 | SBv64; b : uint) => a ^ bv_num(a, uint64(b))

def public operator ^(a : SBv8 | SBv16 | SBv32 | SBv64; b : uint64) => a ^ bv_num(a, b)

def public operator <<(a, b : SBv8 | SBv16 | SBv32 | SBv64) {
    static_if (typeinfo stripped_typename(a) != typeinfo stripped_typename(b)) {
        concept_assert(false, "bitvector width mismatch")
        return mk_bv(a, a.ast, a.ctx)
    } else {
        return mk_bv(a, Z3_mk_bvshl(a.ctx, a.ast, b.ast), a.ctx)
    }
}

def public operator <<(a : SBv8 | SBv16 | SBv32 | SBv64; b : uint) => a << bv_num(a, uint64(b))

def public operator <<(a : SBv8 | SBv16 | SBv32 | SBv64; b : uint64) => a << bv_num(a, b)

def public operator >>(a, b : SBv8 | SBv16 | SBv32 | SBv64) {
    static_if (typeinfo stripped_typename(a) != typeinfo stripped_typename(b)) {
        concept_assert(false, "bitvector width mismatch")
        return mk_bv(a, a.ast, a.ctx)
    } else {
        return mk_bv(a, Z3_mk_bvlshr(a.ctx, a.ast, b.ast), a.ctx)
    }
}

def public operator >>(a : SBv8 | SBv16 | SBv32 | SBv64; b : uint) => a >> bv_num(a, uint64(b))

def public operator >>(a : SBv8 | SBv16 | SBv32 | SBv64; b : uint64) => a >> bv_num(a, b)

def public operator -(a : SBv8 | SBv16 | SBv32 | SBv64) =>
    mk_bv(a, Z3_mk_bvneg(a.ctx, a.ast), a.ctx)

def public operator ~(a : SBv8 | SBv16 | SBv32 | SBv64) =>
    mk_bv(a, Z3_mk_bvnot(a.ctx, a.ast), a.ctx)

//! arithmetic (sign-preserving) right shift; `>>` is the logical one
def public sar(a, b : SBv8 | SBv16 | SBv32 | SBv64) {
    static_if (typeinfo stripped_typename(a) != typeinfo stripped_typename(b)) {
        concept_assert(false, "bitvector width mismatch")
        return mk_bv(a, a.ast, a.ctx)
    } else {
        return mk_bv(a, Z3_mk_bvashr(a.ctx, a.ast, b.ast), a.ctx)
    }
}

def public operator ==(a, b : SBv8 | SBv16 | SBv32 | SBv64) : SBool {
    static_if (typeinfo stripped_typename(a) != typeinfo stripped_typename(b)) {
        concept_assert(false, "bitvector width mismatch")
        return SBool(ast = a.ast, ctx = a.ctx)
    } else {
        return SBool(ast = Z3_mk_eq(a.ctx, a.ast, b.ast), ctx = a.ctx)
    }
}

def public operator ==(a : SBv8 | SBv16 | SBv32 | SBv64; b : uint) : SBool =>
    a == bv_num(a, uint64(b))

def public operator ==(a : SBv8 | SBv16 | SBv32 | SBv64; b : uint64) : SBool => a == bv_num(a, b)

def public operator <(a, b : SBv8 | SBv16 | SBv32 | SBv64) : SBool {
    static_if (typeinfo stripped_typename(a) != typeinfo stripped_typename(b)) {
        concept_assert(false, "bitvector width mismatch")
        return SBool(ast = a.ast, ctx = a.ctx)
    } else {
        return SBool(ast = Z3_mk_bvult(a.ctx, a.ast, b.ast), ctx = a.ctx)
    }
}

def public operator <(a : SBv8 | SBv16 | SBv32 | SBv64; b : uint) : SBool =>
    a < bv_num(a, uint64(b))

def public operator <(a : SBv8 | SBv16 | SBv32 | SBv64; b : uint64) : SBool => a < bv_num(a, b)

def public operator <=(a, b : SBv8 | SBv16 | SBv32 | SBv64) : SBool {
    static_if (typeinfo stripped_typename(a) != typeinfo stripped_typename(b)) {
        concept_assert(false, "bitvector width mismatch")
        return SBool(ast = a.ast, ctx = a.ctx)
    } else {
        return SBool(ast = Z3_mk_bvule(a.ctx, a.ast, b.ast), ctx = a.ctx)
    }
}

def public operator <=(a : SBv8 | SBv16 | SBv32 | SBv64; b : uint) : SBool =>
    a <= bv_num(a, uint64(b))

def public operator <=(a : SBv8 | SBv16 | SBv32 | SBv64; b : uint64) : SBool => a <= bv_num(a, b)

def public operator >(a, b : SBv8 | SBv16 | SBv32 | SBv64) : SBool {
    static_if (typeinfo stripped_typename(a) != typeinfo stripped_typename(b)) {
        concept_assert(false, "bitvector width mismatch")
        return SBool(ast = a.ast, ctx = a.ctx)
    } else {
        return SBool(ast = Z3_mk_bvugt(a.ctx, a.ast, b.ast), ctx = a.ctx)
    }
}

def public operator >(a : SBv8 | SBv16 | SBv32 | SBv64; b : uint) : SBool =>
    a > bv_num(a, uint64(b))

def public operator >(a : SBv8 | SBv16 | SBv32 | SBv64; b : uint64) : SBool => a > bv_num(a, b)

def public operator >=(a, b : SBv8 | SBv16 | SBv32 | SBv64) : SBool {
    static_if (typeinfo stripped_typename(a) != typeinfo stripped_typename(b)) {
        concept_assert(false, "bitvector width mismatch")
        return SBool(ast = a.ast, ctx = a.ctx)
    } else {
        return SBool(ast = Z3_mk_bvuge(a.ctx, a.ast, b.ast), ctx = a.ctx)
    }
}

def public operator >=(a : SBv8 | SBv16 | SBv32 | SBv64; b : uint) : SBool =>
    a >= bv_num(a, uint64(b))

def public operator >=(a : SBv8 | SBv16 | SBv32 | SBv64; b : uint64) : SBool => a >= bv_num(a, b)

def public operator !=(a, b : SBv8 | SBv16 | SBv32 | SBv64) : SBool => s_not(a == b)

def public operator !=(a : SBv8 | SBv16 | SBv32 | SBv64; b : uint) : SBool => s_not(a == b)

def public operator !=(a : SBv8 | SBv16 | SBv32 | SBv64; b : uint64) : SBool => s_not(a == b)

def public s_lt(a, b : SBv8 | SBv16 | SBv32 | SBv64) : SBool {
    static_if (typeinfo stripped_typename(a) != typeinfo stripped_typename(b)) {
        concept_assert(false, "bitvector width mismatch")
        return SBool(ast = a.ast, ctx = a.ctx)
    } else {
        return SBool(ast = Z3_mk_bvslt(a.ctx, a.ast, b.ast), ctx = a.ctx)
    }
}

def public s_le(a, b : SBv8 | SBv16 | SBv32 | SBv64) : SBool {
    static_if (typeinfo stripped_typename(a) != typeinfo stripped_typename(b)) {
        concept_assert(false, "bitvector width mismatch")
        return SBool(ast = a.ast, ctx = a.ctx)
    } else {
        return SBool(ast = Z3_mk_bvsle(a.ctx, a.ast, b.ast), ctx = a.ctx)
    }
}

def public s_gt(a, b : SBv8 | SBv16 | SBv32 | SBv64) : SBool {
    static_if (typeinfo stripped_typename(a) != typeinfo stripped_typename(b)) {
        concept_assert(false, "bitvector width mismatch")
        return SBool(ast = a.ast, ctx = a.ctx)
    } else {
        return SBool(ast = Z3_mk_bvsgt(a.ctx, a.ast, b.ast), ctx = a.ctx)
    }
}

def public s_ge(a, b : SBv8 | SBv16 | SBv32 | SBv64) : SBool {
    static_if (typeinfo stripped_typename(a) != typeinfo stripped_typename(b)) {
        concept_assert(false, "bitvector width mismatch")
        return SBool(ast = a.ast, ctx = a.ctx)
    } else {
        return SBool(ast = Z3_mk_bvsge(a.ctx, a.ast, b.ast), ctx = a.ctx)
    }
}

def public ite(c : SBool; t, f : SBv8 | SBv16 | SBv32 | SBv64) {
    static_if (typeinfo stripped_typename(t) != typeinfo stripped_typename(f)) {
        concept_assert(false, "bitvector width mismatch in ite arms")
        return mk_bv(t, t.ast, t.ctx)
    } else {
        return mk_bv(t, Z3_mk_ite(c.ctx, c.ast, t.ast, f.ast), c.ctx)
    }
}

//! `s_lift` — value-or-symbolic normalizer used by generated code ([smt_fn],
//! constrain blocks): concrete das values become numerals, symbolic pass through.
def public s_lift(ctx : Z3_context; v : int) : SInt => int_val(ctx, v)

def public s_lift(_ctx : Z3_context; v : SInt) : SInt => SInt(ast = v.ast, ctx = v.ctx)

def public s_lift(ctx : Z3_context; v : bool) : SBool => bool_val(ctx, v)

def public s_lift(_ctx : Z3_context; v : SBool) : SBool => SBool(ast = v.ast, ctx = v.ctx)

def public s_lift(ctx : Z3_context; v : double) : SReal => real_val(ctx, v)

def public s_lift(_ctx : Z3_context; v : SReal) : SReal => SReal(ast = v.ast, ctx = v.ctx)

def public s_lift(ctx : Z3_context; v : uint8) : SBv8 => bv8_val(ctx, uint(v))

def public s_lift(ctx : Z3_context; v : uint16) : SBv16 => bv16_val(ctx, uint(v))

def public s_lift(ctx : Z3_context; v : uint) : SBv32 => bv32_val(ctx, v)

def public s_lift(ctx : Z3_context; v : uint64) : SBv64 => bv64_val(ctx, v)

def public s_lift(_ctx : Z3_context; v : SBv8) : SBv8 => SBv8(ast = v.ast, ctx = v.ctx)

def public s_lift(_ctx : Z3_context; v : SBv16) : SBv16 => SBv16(ast = v.ast, ctx = v.ctx)

def public s_lift(_ctx : Z3_context; v : SBv32) : SBv32 => SBv32(ast = v.ast, ctx = v.ctx)

def public s_lift(_ctx : Z3_context; v : SBv64) : SBv64 => SBv64(ast = v.ast, ctx = v.ctx)

// ===== quantifiers =====

//! Bounded combinators: expand a predicate over concrete indices into one conjunction/disjunction at
//! formula-build time. Not SMT quantifiers (`smt_forall`/`smt_exists` below are); for a fixed small n
//! the expansion is the encoding solvers handle best.
def public all_of(n : int; blk : block<(i : int) : SBool>) : SBool {
    assert(n > 0)
    var conds <- [for (i in range(n)); invoke(blk, i)]
    return s_all(conds)
}

def public any_of(n : int; blk : block<(i : int) : SBool>) : SBool {
    assert(n > 0)
    var conds <- [for (i in range(n)); invoke(blk, i)]
    return s_any(conds)
}

//! All unordered pairs i < j — the no-overlap / pairwise-distance workhorse.
def public all_pairs(n : int; blk : block<(i, j : int) : SBool>) : SBool {
    assert(n > 1)
    var conds : array<SBool>
    conds |> reserve(n * (n - 1) / 2)
    for (i in range(n)) {
        for (j in range(i + 1, n)) {
            conds |> push(invoke(blk, i, j))
        }
    }
    return s_all(conds)
}

//! TRUE SMT quantifiers over symbolic bound variables (Z3_mk_forall_const).
def public smt_forall(ctx : Z3_context; blk : block<(q : SInt) : SBool>) : SBool =>
    quant1(ctx, blk, true)

def public smt_exists(ctx : Z3_context; blk : block<(q : SInt) : SBool>) : SBool =>
    quant1(ctx, blk, false)

def public smt_forall(s : Solver; blk : block<(q : SInt) : SBool>) : SBool =>
    quant1(s.ctx, blk, true)

def public smt_exists(s : Solver; blk : block<(q : SInt) : SBool>) : SBool =>
    quant1(s.ctx, blk, false)

def private quant1(ctx : Z3_context; blk : block<(q : SInt) : SBool>; isForall : bool) : SBool {
    let q = SInt(ast = Z3_mk_fresh_const(ctx, "q", Z3_mk_int_sort(ctx)), ctx = ctx)
    let body = invoke(blk, q)
    var apps <- [Z3_to_app(ctx, dc(q.ast))]
    let bound = unsafe(reinterpret<void?>(array_data_ptr(apps)))
    let ast = isForall ? Z3_mk_forall_const(ctx, 0u, 1u, bound, 0u, null, body.ast) : Z3_mk_exists_const(ctx, 0u, 1u, bound, 0u, null, body.ast)
    return SBool(ast = ast, ctx = ctx)
}

def public smt_forall(ctx : Z3_context; blk : block<(a, b : SInt) : SBool>) : SBool =>
    quant2(ctx, blk, true)

def public smt_exists(ctx : Z3_context; blk : block<(a, b : SInt) : SBool>) : SBool =>
    quant2(ctx, blk, false)

def public smt_forall(s : Solver; blk : block<(a, b : SInt) : SBool>) : SBool =>
    quant2(s.ctx, blk, true)

def public smt_exists(s : Solver; blk : block<(a, b : SInt) : SBool>) : SBool =>
    quant2(s.ctx, blk, false)

def private quant2(ctx : Z3_context; blk : block<(a, b : SInt) : SBool>; isForall : bool) : SBool {
    let qa = SInt(ast = Z3_mk_fresh_const(ctx, "qa", Z3_mk_int_sort(ctx)), ctx = ctx)
    let qb = SInt(ast = Z3_mk_fresh_const(ctx, "qb", Z3_mk_int_sort(ctx)), ctx = ctx)
    let body = invoke(blk, qa, qb)
    var apps <- [Z3_to_app(ctx, dc(qa.ast)), Z3_to_app(ctx, dc(qb.ast))]
    let bound = unsafe(reinterpret<void?>(array_data_ptr(apps)))
    let ast = isForall ? Z3_mk_forall_const(ctx, 0u, 2u, bound, 0u, null, body.ast) : Z3_mk_exists_const(ctx, 0u, 2u, bound, 0u, null, body.ast)
    return SBool(ast = ast, ctx = ctx)
}

// ===== solving =====

def public add(s : Solver; e : SBool) {
    Z3_solver_assert(s.ctx, s.solver, e.ast)
}

def public check(s : Solver) : SmtResult {
    let r = Z3_solver_check(s.ctx, s.solver)
    if (r == Z3_lbool.Z3_L_TRUE) {
        return SmtResult.SAT
    } elif (r == Z3_lbool.Z3_L_FALSE) {
        return SmtResult.UNSAT
    }
    return SmtResult.UNKNOWN
}

def public push(s : Solver) {
    Z3_solver_push(s.ctx, s.solver)
}

def public pop(s : Solver) {
    Z3_solver_pop(s.ctx, s.solver, 1u)
}

// ===== models =====

def public with_model(s : Solver; blk : block<(m : Model) : void>) {
    var m = Model(ctx = s.ctx, model = Z3_solver_get_model(s.ctx, s.solver))
    Z3_model_inc_ref(s.ctx, m.model)
    invoke(blk, m)
    Z3_model_dec_ref(s.ctx, m.model)
}

def private eval_ast(m : Model; e : Z3_ast) : Z3_ast {
    var val : Z3_ast
    Z3_model_eval(m.ctx, m.model, e, true, safe_addr(val))
    return val
}

def public eval_int(m : Model; x : SInt) : int =>
    to_int(Z3_get_numeral_string(m.ctx, eval_ast(m, x.ast)))

def public eval_bool(m : Model; x : SBool) : bool =>
    Z3_get_bool_value(m.ctx, eval_ast(m, x.ast)) == Z3_lbool.Z3_L_TRUE

def public eval_real(m : Model; x : SReal) : double =>
    Z3_get_numeral_double(m.ctx, eval_ast(m, x.ast))

def public eval_string(m : Model; x : SString) : string => Z3_get_string(m.ctx, eval_ast(m, x.ast))

def public eval_bv(m : Model; x : SBv8 | SBv16 | SBv32 | SBv64) : uint64 {
    var v = 0ul
    Z3_get_numeral_uint64(m.ctx, eval_ast(m, x.ast), safe_addr(v))
    return v
}

// one-shots on the solver's current model
def public model_int(s : Solver; x : SInt) : int {
    var r = 0
    with_model(s) $(m) {
        r = eval_int(m, x)
    }
    return r
}

def public model_bool(s : Solver; x : SBool) : bool {
    var r = false
    with_model(s) $(m) {
        r = eval_bool(m, x)
    }
    return r
}

def public model_real(s : Solver; x : SReal) : double {
    var r = 0.0lf
    with_model(s) $(m) {
        r = eval_real(m, x)
    }
    return r
}

def public model_string(s : Solver; x : SString) : string {
    var r = ""
    with_model(s) $(m) {
        r = eval_string(m, x)
    }
    return r
}

def public model_bv(s : Solver; x : SBv8 | SBv16 | SBv32 | SBv64) : uint64 {
    var r = 0ul
    with_model(s) $(m) {
        r = eval_bv(m, x)
    }
    return r
}

// name-based readback — for unknowns declared inside `constrain`/`smt_formula`
// blocks, whose das bindings die at block scope end
def private find_const_value(m : Model; name : string) : Z3_ast {
    let n = Z3_model_get_num_consts(m.ctx, m.model)
    for (i in urange(n)) {
        var decl = Z3_model_get_const_decl(m.ctx, m.model, i)
        if (Z3_get_symbol_string(m.ctx, Z3_get_decl_name(m.ctx, decl)) == name) {
            return Z3_model_get_const_interp(m.ctx, m.model, decl)
        }
    }
    return null
}

def public model_int(s : Solver; name : string) : int {
    var r = 0
    with_model(s) $(m) {
        let v = find_const_value(m, name)
        if (v != null) {
            r = to_int(Z3_get_numeral_string(m.ctx, v))
        }
    }
    return r
}

def public model_bool(s : Solver; name : string) : bool {
    var r = false
    with_model(s) $(m) {
        let v = find_const_value(m, name)
        if (v != null) {
            r = Z3_get_bool_value(m.ctx, v) == Z3_lbool.Z3_L_TRUE
        }
    }
    return r
}

def public model_real(s : Solver; name : string) : double {
    var r = 0.0lf
    with_model(s) $(m) {
        let v = find_const_value(m, name)
        if (v != null) {
            r = Z3_get_numeral_double(m.ctx, v)
        }
    }
    return r
}

def public model_string(s : Solver; name : string) : string {
    var r = ""
    with_model(s) $(m) {
        let v = find_const_value(m, name)
        if (v != null) {
            r = Z3_get_string(m.ctx, v)
        }
    }
    return r
}

def public model_bv(s : Solver; name : string) : uint64 {
    var r = 0ul
    with_model(s) $(m) {
        let v = find_const_value(m, name)
        if (v != null) {
            Z3_get_numeral_uint64(m.ctx, v, safe_addr(r))
        }
    }
    return r
}

// ===== debug =====

//! Full solver state as SMT-LIB2 text — the exact formula Z3 sees.
def public to_smtlib(s : Solver) : string => Z3_solver_to_string(s.ctx, s.solver)

def public to_string(e : SBool) : string => Z3_ast_to_string(e.ctx, e.ast)

def public to_string(e : SInt) : string => Z3_ast_to_string(e.ctx, e.ast)

def public to_string(e : SReal) : string => Z3_ast_to_string(e.ctx, e.ast)

def public to_string(e : SString) : string => Z3_ast_to_string(e.ctx, e.ast)
