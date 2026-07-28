options gen2
options indenting = 4
options strict_smart_pointers = true

module smt_lint shared private

//! SMT-backed reachability lint. Symbolically executes each function body,
//! accumulating a path condition, and asks Z3 whether each branch condition can
//! hold on the paths that reach it.
//!
//! Rules:
//!   SMT001 — a branch whose condition is unsatisfiable on every path reaching it (suppress with '// nolint:SMT001')
//!   SMT002 — a condition that is always true, with no else branch: a redundant guard (default-off; enable with 'options _enable_default_off_rules = true' or .lint_config)
//!
//! Only an UNSAT verdict is ever reported. Timeouts, unmodeled types, unproven
//! integer overflow and anything the translation cannot represent all yield
//! silence, so the pass under-reports by construction rather than guessing.

require smt/daslib/smt_expr
require daslib/ast_boost
require daslib/lint_config
require daslib/rtti
require strings

let private INT_MAX_V = 2147483647
let private INT_MIN_V = -2147483647 - 1
let private DEFAULT_TIMEOUT_MS = 250u
let private DEFAULT_MAX_QUERIES = 200
let private MAX_TRANSLATE_DEPTH = 64

enum private SymKind {
    UNKNOWN
    BOOL
    INT
    F32
    F64
}

// A translated expression: a Z3 term plus the das type family it came from.
// UNKNOWN means "not modeled" — it poisons any operator it feeds.
struct private Sym {
    kind : SymKind = SymKind.UNKNOWN
    ast : Z3_ast
}

def private unknown_sym() : Sym {
    return Sym(kind = SymKind.UNKNOWN, ast = null)
}

// Z3 handles are opaque to das and never dereferenced, so stripping const to build
// an argument array cannot license a miscompile. Same reasoning as smt_expr's dc.
def private dc(p : Z3_ast const) : Z3_ast {
    return unsafe(reinterpret<Z3_ast>(p))
}

def private dcc(p : Z3_context const) : Z3_context {
    return unsafe(reinterpret<Z3_context>(p))
}

class SmtLintVisitor : AstVisitor {
    sv : Solver
    ctx : Z3_context

    compile_time_errors : bool = false
    collect_warnings : bool = false
    warnings : array<string>
    issues : array<LintIssue>
    warning_count : int = 0
    reported_locations : table<uint64>
    disabled_codes : table<string>
    enabled_codes : table<string>

    @do_not_delete current_function : Function? = null
    skip_function : bool = true

    // Declared params and locals only. An ExprVar with no entry here gets a fresh
    // symbol per occurrence, so nothing is ever assumed equal to anything else.
    env : table<uint64; Sym>
    // Locals whose address escaped: every read is a fresh symbol from now on.
    volatile_vars : table<uint64>

    // True once some condition already folded into the path condition contained
    // integer arithmetic we could not prove overflow-free.
    pc_arith_unproven : bool = false
    walk_budget : int = 0
    // Set while translating if any compiler-synthesized variable was read. Inlining
    // rewrites a call into `var __inl0_ret; ...; if (!__inl0_ret) ...`, and reporting
    // on that guard blames the user for code they never wrote.
    saw_generated_var : bool = false

    queries : int = 0
    max_queries : int = DEFAULT_MAX_QUERIES
    timeout_ms : uint = DEFAULT_TIMEOUT_MS

    def SmtLintVisitor() {
        pass
    }

    // ===== diagnostics =====

    def private location_key(at : LineInfo) : uint64 {
        return uint64(at.line) | (uint64(at.column) << uint64(20))
    }

    def private is_suppressed(text : string; at : LineInfo) : bool {
        let code = extract_lint_code(text)
        if (!empty(code)) {
            return true if (key_exists(disabled_codes, code)
                            || (!empty(enabled_codes) && !key_exists(enabled_codes, code)))
        }
        return is_lint_suppressed(at, code)
    }

    def smt_warning(text : string; at : LineInfo) : void {
        if (current_function != null && current_function.flags.generated) return
        let key = location_key(at)
        if (key_exists(reported_locations, key)) return
        reported_locations |> insert(key)
        if (is_suppressed(text, at)) return
        warning_count++
        if (compile_time_errors) {
            compiling_program() |> macro_style_warning(at, text)
        } elif (collect_warnings) {
            warnings |> push("smt warning: {text} at {describe(at)}")
            issues |> push(make_lint_issue(text, at))
        } else {
            to_log(LOG_WARNING, "smt warning: {text} at {describe(at)}\n")
        }
    }

    // ===== solver plumbing =====

    def private sbool(a : Z3_ast) : SBool {
        return SBool(ast = a, ctx = ctx)
    }

    def private sort_of(k : SymKind) : Z3_sort {
        if (k == SymKind.BOOL) return Z3_mk_bool_sort(ctx)
        if (k == SymKind.INT) return Z3_mk_int_sort(ctx)
        if (k == SymKind.F32) return Z3_mk_fpa_sort_32(ctx)
        if (k == SymKind.F64) return Z3_mk_fpa_sort_64(ctx)
        return null
    }

    def private fresh(k : SymKind) : Sym {
        if (k == SymKind.UNKNOWN) return unknown_sym()
        return Sym(kind = k, ast = Z3_mk_fresh_const(ctx, "u", sort_of(k)))
    }

    //! Only `int` among the integer types is modeled: the overflow bound below is
    //! written for 32-bit signed, and das `%` / `/` truncate toward zero, which
    //! neither Z3_mk_mod nor Z3_mk_rem reproduces.
    def private kind_of(td : TypeDecl?) : SymKind {
        if (td == null) return SymKind.UNKNOWN
        let bt = td.baseType
        if (bt == Type.tBool) return SymKind.BOOL
        if (bt == Type.tInt) return SymKind.INT
        if (bt == Type.tFloat) return SymKind.F32
        if (bt == Type.tDouble) return SymKind.F64
        return SymKind.UNKNOWN
    }

    def private query_sat(cond : Z3_ast) : SmtResult {
        if (queries >= max_queries || cond == null) return SmtResult.UNKNOWN
        queries++
        sv |> push()
        sv |> add(sbool(cond))
        let r = sv |> check()
        sv |> pop()
        return r
    }

    // An int term is safe to reason about only if it cannot leave the 32-bit
    // signed range on any path reaching it — SMT Int does not wrap, so an
    // overflowing term would make a reachable branch look impossible.
    def private no_overflow(a : Z3_ast) : bool {
        let t = SInt(ast = a, ctx = ctx)
        var alts <- [dc((t < INT_MIN_V).ast), dc((t > INT_MAX_V).ast)]
        let out_of_range = Z3_mk_or(ctx, alts)
        return query_sat(out_of_range) == SmtResult.UNSAT
    }

    // Every integer add/sub/mul/negate inside the term, wherever it came from —
    // walking the Z3 term rather than tracking das-side means arithmetic that
    // reached the condition through a tracked local is still found.
    def private collect_int_arith(a : Z3_ast; var acc : array<Z3_ast>) : bool {
        if (walk_budget <= 0 || a == null) return false
        walk_budget--
        if (Z3_get_ast_kind(ctx, a) != Z3_ast_kind.Z3_APP_AST) return true
        var app = Z3_to_app(ctx, a)
        let dk = Z3_get_decl_kind(ctx, Z3_get_app_decl(ctx, app))
        if (dk == Z3_decl_kind.Z3_OP_ADD || dk == Z3_decl_kind.Z3_OP_SUB
            || dk == Z3_decl_kind.Z3_OP_MUL || dk == Z3_decl_kind.Z3_OP_UMINUS) {
            if (Z3_get_sort_kind(ctx, Z3_get_sort(ctx, a)) == Z3_sort_kind.Z3_INT_SORT) {
                acc |> push(dc(a))
            }
        }
        let n = Z3_get_app_num_args(ctx, app)
        for (i in urange(n)) {
            if (!collect_int_arith(Z3_get_app_arg(ctx, app, i), acc)) return false
        }
        return true
    }

    def private overflow_free(a : Z3_ast) : bool {
        var nodes : array<Z3_ast>
        walk_budget = 400
        if (!collect_int_arith(a, nodes)) return false
        for (n in nodes) {
            if (!no_overflow(n)) return false
        }
        return true
    }

    // Asserting a term into the path condition: remember whether its arithmetic is
    // trustworthy. Checked here, under the looser enclosing path condition, which
    // is the conservative direction.
    def private assume_term(term : Z3_ast) : void {
        pc_arith_unproven = pc_arith_unproven || !overflow_free(term)
        sv |> add(sbool(term))
    }

    // ===== expression translation =====

    def private var_key(v : Variable?) : uint64 {
        return intptr(v)
    }

    def private var_sym(evar : ExprVar?) : Sym {
        if (evar.variable == null) return fresh(kind_of(evar._type))
        if (evar.variable.flags.generated) {
            saw_generated_var = true
        }
        let key = var_key(evar.variable)
        if (key_exists(volatile_vars, key)) return fresh(kind_of(evar._type))
        return env?[key] ?? fresh(kind_of(evar._type))
    }

    // A literal condition is constant folding, not a path-sensitive contradiction:
    // STYLE010 owns that, and a `$v(...)` qmacro splice specializing to a constant is
    // the macro author's intent, not a defect.
    def private is_const_cond(cond : ExpressionPtr) : bool {
        var e = cond
        if (e != null && e is ExprRef2Value) {
            e = (e as ExprRef2Value).subexpr
        }
        return e != null && e is ExprConstBool
    }

    def private op1(expr : ExprOp1?; depth : int) : Sym {
        let a = to_sym(expr.subexpr, depth + 1)
        if (a.kind == SymKind.UNKNOWN) return fresh(kind_of(expr._type))
        if (expr.op == "!" && a.kind == SymKind.BOOL) {
            return Sym(kind = SymKind.BOOL, ast = Z3_mk_not(ctx, a.ast))
        }
        if (expr.op == "-") {
            if (a.kind == SymKind.INT) {
                var r = Z3_mk_unary_minus(ctx, a.ast)
                return Sym(kind = SymKind.INT, ast = r)
            }
            if (a.kind == SymKind.F32 || a.kind == SymKind.F64) {
                return Sym(kind = a.kind, ast = Z3_mk_fpa_neg(ctx, a.ast))
            }
        }
        if (expr.op == "+" && (a.kind == SymKind.INT || a.kind == SymKind.F32 || a.kind == SymKind.F64)) {
            return Sym(kind = a.kind, ast = dc(a.ast))
        }
        return fresh(kind_of(expr._type))
    }

    // `/` and `%` are deliberately absent: das truncates toward zero, and neither
    // Z3_mk_div/Z3_mk_mod (Euclidean) nor Z3_mk_rem matches that for negatives.
    def private op2_int(op : string; a, b : Sym) : Sym {
        let x = SInt(ast = a.ast, ctx = ctx)
        let y = SInt(ast = b.ast, ctx = ctx)
        if (op == "==") return Sym(kind = SymKind.BOOL, ast = dc((x == y).ast))
        if (op == "!=") return Sym(kind = SymKind.BOOL, ast = dc((x != y).ast))
        if (op == "<") return Sym(kind = SymKind.BOOL, ast = dc((x < y).ast))
        if (op == "<=") return Sym(kind = SymKind.BOOL, ast = dc((x <= y).ast))
        if (op == ">") return Sym(kind = SymKind.BOOL, ast = dc((x > y).ast))
        if (op == ">=") return Sym(kind = SymKind.BOOL, ast = dc((x >= y).ast))
        var res : Z3_ast = null
        if (op == "+") {
            res = dc((x + y).ast)
        } elif (op == "-") {
            res = dc((x - y).ast)
        } elif (op == "*") {
            res = dc((x * y).ast)
        }
        if (res == null) return unknown_sym()
        return Sym(kind = SymKind.INT, ast = res)
    }

    // One function for both widths: the terms carry their own sort, and the caller
    // has already established that both sides share a kind.
    def private op2_fp(op : string; a, b : Sym) : Sym {
        if (op == "==") return Sym(kind = SymKind.BOOL, ast = Z3_mk_fpa_eq(ctx, a.ast, b.ast))
        if (op == "!=") return Sym(kind = SymKind.BOOL, ast = Z3_mk_not(ctx, Z3_mk_fpa_eq(ctx, a.ast, b.ast)))
        if (op == "<") return Sym(kind = SymKind.BOOL, ast = Z3_mk_fpa_lt(ctx, a.ast, b.ast))
        if (op == "<=") return Sym(kind = SymKind.BOOL, ast = Z3_mk_fpa_leq(ctx, a.ast, b.ast))
        if (op == ">") return Sym(kind = SymKind.BOOL, ast = Z3_mk_fpa_gt(ctx, a.ast, b.ast))
        if (op == ">=") return Sym(kind = SymKind.BOOL, ast = Z3_mk_fpa_geq(ctx, a.ast, b.ast))
        let rm = Z3_mk_fpa_rne(ctx)
        if (op == "+") return Sym(kind = a.kind, ast = Z3_mk_fpa_add(ctx, rm, a.ast, b.ast))
        if (op == "-") return Sym(kind = a.kind, ast = Z3_mk_fpa_sub(ctx, rm, a.ast, b.ast))
        if (op == "*") return Sym(kind = a.kind, ast = Z3_mk_fpa_mul(ctx, rm, a.ast, b.ast))
        if (op == "/") return Sym(kind = a.kind, ast = Z3_mk_fpa_div(ctx, rm, a.ast, b.ast))
        return unknown_sym()
    }

    def private op2_bool(op : string; a, b : Sym) : Sym {
        if (op == "&&") {
            var args <- [dc(a.ast), dc(b.ast)]
            return Sym(kind = SymKind.BOOL, ast = Z3_mk_and(ctx, args))
        }
        if (op == "||") {
            var args <- [dc(a.ast), dc(b.ast)]
            return Sym(kind = SymKind.BOOL, ast = Z3_mk_or(ctx, args))
        }
        if (op == "==") return Sym(kind = SymKind.BOOL, ast = Z3_mk_eq(ctx, a.ast, b.ast))
        if (op == "!=") return Sym(kind = SymKind.BOOL, ast = Z3_mk_not(ctx, Z3_mk_eq(ctx, a.ast, b.ast)))
        if (op == "^^") return Sym(kind = SymKind.BOOL, ast = Z3_mk_xor(ctx, a.ast, b.ast))
        return unknown_sym()
    }

    def private op2(expr : ExprOp2?; depth : int) : Sym {
        let a = to_sym(expr.left, depth + 1)
        let b = to_sym(expr.right, depth + 1)
        if (a.kind == SymKind.UNKNOWN || b.kind == SymKind.UNKNOWN || a.kind != b.kind) {
            return fresh(kind_of(expr._type))
        }
        let op = "{expr.op}"
        var r = unknown_sym()
        if (a.kind == SymKind.INT) {
            r = op2_int(op, a, b)
        } elif (a.kind == SymKind.BOOL) {
            r = op2_bool(op, a, b)
        } else {
            r = op2_fp(op, a, b)
        }
        if (r.kind == SymKind.UNKNOWN) return fresh(kind_of(expr._type))
        return r
    }

    def private op3(expr : ExprOp3?; depth : int) : Sym {
        let c = to_sym(expr.subexpr, depth + 1)
        let t = to_sym(expr.left, depth + 1)
        let f = to_sym(expr.right, depth + 1)
        if (c.kind != SymKind.BOOL || t.kind == SymKind.UNKNOWN || t.kind != f.kind) {
            return fresh(kind_of(expr._type))
        }
        return Sym(kind = t.kind, ast = Z3_mk_ite(ctx, c.ast, t.ast, f.ast))
    }

    // `float(i)` / `double(i)` are NOT translated: unbounded Int -> Real -> FP is
    // measurably undecidable (it times out even on trivial goals), so it would
    // burn the whole query budget to answer UNKNOWN anyway.
    def private call_sym(call : ExprCall?; depth : int) : Sym {
        let k = kind_of(call._type)
        let name = "{call.name}"
        if (length(call.arguments) == 1) {
            let a = to_sym(call.arguments[0], depth + 1)
            if (a.kind == k && k == SymKind.INT && name == "abs") {
                let x = SInt(ast = a.ast, ctx = ctx)
                return Sym(kind = SymKind.INT, ast = dc(abs(x).ast))
            }
            if (a.kind == k && (k == SymKind.F32 || k == SymKind.F64)) {
                if (name == "abs") return Sym(kind = k, ast = Z3_mk_fpa_abs(ctx, a.ast))
                if (name == "sqrt") return Sym(kind = k, ast = Z3_mk_fpa_sqrt(ctx, Z3_mk_fpa_rne(ctx), a.ast))
            }
            // float <-> double is exact and cheap; int <-> float is not (see above).
            if (k == SymKind.F64 && a.kind == SymKind.F32 && name == "double") {
                return Sym(kind = k, ast = Z3_mk_fpa_to_fp_float(ctx, Z3_mk_fpa_rne(ctx), a.ast, Z3_mk_fpa_sort_64(ctx)))
            }
            if (k == SymKind.F32 && a.kind == SymKind.F64 && name == "float") {
                return Sym(kind = k, ast = Z3_mk_fpa_to_fp_float(ctx, Z3_mk_fpa_rne(ctx), a.ast, Z3_mk_fpa_sort_32(ctx)))
            }
        }
        if (length(call.arguments) == 2 && (name == "min" || name == "max")) {
            let a = to_sym(call.arguments[0], depth + 1)
            let b = to_sym(call.arguments[1], depth + 1)
            if (a.kind == k && b.kind == k) {
                if (k == SymKind.INT) {
                    let x = SInt(ast = a.ast, ctx = ctx)
                    let y = SInt(ast = b.ast, ctx = ctx)
                    return Sym(kind = k, ast = name == "min" ? dc(min(x, y).ast) : dc(max(x, y).ast))
                }
                if (k == SymKind.F32 || k == SymKind.F64) {
                    return Sym(kind = k, ast = name == "min"
                        ? Z3_mk_fpa_min(ctx, a.ast, b.ast)
                        : Z3_mk_fpa_max(ctx, a.ast, b.ast))
                }
            }
        }
        return fresh(k)
    }

    def to_sym(expr : ExpressionPtr; depth : int) : Sym {   // nolint:STYLE037 — exact-type 'is' ladder over the expression node kinds; one arm per kind
        if (expr == null) return unknown_sym()
        if (depth > MAX_TRANSLATE_DEPTH) return fresh(kind_of(expr._type))
        if (expr is ExprRef2Value) return to_sym((expr as ExprRef2Value).subexpr, depth + 1)
        if (expr is ExprConstInt) return Sym(kind = SymKind.INT, ast = dc(int_val(ctx, (expr as ExprConstInt).value).ast))
        if (expr is ExprConstBool) return Sym(kind = SymKind.BOOL, ast = dc(bool_val(ctx, (expr as ExprConstBool).value).ast))
        if (expr is ExprConstFloat) return Sym(kind = SymKind.F32, ast = dc(float_val(ctx, (expr as ExprConstFloat).value).ast))
        if (expr is ExprConstDouble) return Sym(kind = SymKind.F64, ast = dc(double_val(ctx, (expr as ExprConstDouble).value).ast))
        if (expr is ExprVar) return var_sym(expr as ExprVar)
        if (expr is ExprOp1) return op1(expr as ExprOp1, depth)
        if (expr is ExprOp2) return op2(expr as ExprOp2, depth)
        if (expr is ExprOp3) return op3(expr as ExprOp3, depth)
        if (expr is ExprCall) return call_sym(expr as ExprCall, depth)
        return fresh(kind_of(expr._type))
    }

    // ===== havoc =====

    def private havoc(v : Variable?) : void {
        if (v == null) return
        let key = var_key(v)
        if (key_exists(env, key)) {
            env |> erase(key)
        }
    }

    def private make_volatile(v : Variable?) : void {
        if (v == null) return
        havoc(v)
        let key = var_key(v)
        if (!key_exists(volatile_vars, key)) {
            volatile_vars |> insert(key)
        }
    }

    // Every local a closure could observe or write. A closure runs later, maybe
    // never, maybe repeatedly, so nothing tracked survives one being created.
    def private havoc_everything() : void {
        env |> clear()
    }

    def private collect_written(expr : ExpressionPtr; var acc : array<uint64>) : void {
        if (expr == null) return
        var collector <- new WriteCollector()
        make_visitor(*collector) $(adapter) {
            visit(expr, adapter)
        }
        acc |> push_from(collector.hits)
        unsafe {
            delete collector
        }
    }

    // ===== statement execution; returns true when the statement always exits =====

    def private exec_block(blk : ExprBlock?) : bool {
        for (stmt in blk.list) {
            if (exec(stmt)) return true
        }
        return false
    }

    def private exec_let(lt : ExprLet?) : bool {
        for (v in lt.variables) {
            if (v._type != null && v._type.flags.ref) {
                make_volatile(v)
            } elif (v.init != null) {
                var s = to_sym(v.init, 0)
                if (s.kind == SymKind.UNKNOWN) {
                    havoc(v)
                } else {
                    env[var_key(v)] = s
                }
            } else {
                havoc(v)
            }
        }
        return false
    }

    def private exec_store(lhs, rhs : ExpressionPtr) : bool {
        if (lhs != null && lhs is ExprVar) {
            let ev = lhs as ExprVar
            if (ev.variable != null && !key_exists(volatile_vars, var_key(ev.variable))) {
                var s = to_sym(rhs, 0)
                if (s.kind == SymKind.UNKNOWN) {
                    havoc(ev.variable)
                } else {
                    env[var_key(ev.variable)] = s
                }
                return false
            }
            havoc(ev.variable)
            return false
        }
        // A field, index or deref write could alias any tracked local.
        havoc_everything()
        return false
    }

    def private exec_if(ift : ExprIfThenElse?) : bool {
        saw_generated_var = false
        let c = to_sym(ift.cond, 0)
        let may_report = (c.kind == SymKind.BOOL && !ift.genFlags.generated
            && !saw_generated_var && !is_const_cond(ift.cond)
            && !pc_arith_unproven && overflow_free(c.ast))
        if (c.kind != SymKind.BOOL) {
            // Unmodeled condition: both branches stay possible, pc learns nothing.
            var t_term = false
            if (ift.if_true != null) {
                t_term = exec(ift.if_true)
            }
            var f_term = false
            if (ift.if_false != null) {
                f_term = exec(ift.if_false)
            }
            return t_term && f_term
        }
        let not_c = Z3_mk_not(ctx, c.ast)
        let then_res = query_sat(c.ast)
        let else_res = query_sat(not_c)
        let has_else = ift.if_false != null
        if (may_report) {
            if (then_res == SmtResult.UNSAT) {
                smt_warning("SMT001: this branch is unreachable — the condition cannot hold on any path that reaches it", ift.at)
            } elif (else_res == SmtResult.UNSAT) {
                if (has_else) {
                    smt_warning("SMT001: the else branch is unreachable — the condition always holds on every path that reaches it", ift.at)
                } else {
                    smt_warning("SMT002: this condition is always true here; the guard is redundant", ift.at)
                }
            }
        }
        let saved_unproven = pc_arith_unproven

        var then_env : table<uint64; Sym>
        var else_env : table<uint64; Sym>
        var then_term = false
        var else_term = false
        var then_live = false
        var else_live = false

        if (then_res != SmtResult.UNSAT && ift.if_true != null) {
            then_live = true
            var base : table<uint64; Sym>
            base := env
            sv |> push()
            assume_term(c.ast)
            then_term = exec(ift.if_true)
            sv |> pop()
            then_env := env
            env := base
        }
        if (else_res != SmtResult.UNSAT && has_else) {
            else_live = true
            var base : table<uint64; Sym>
            base := env
            sv |> push()
            assume_term(not_c)
            else_term = exec(ift.if_false)
            sv |> pop()
            else_env := env
            env := base
        }
        pc_arith_unproven = saved_unproven

        merge_after_if(c, then_env, else_env, then_live && !then_term, else_live && !else_term, has_else)

        // A guard that always exits tells us its condition is false below.
        if (then_live && then_term && !has_else) {
            assume_term(not_c)
        }
        return then_term && else_term && has_else
    }

    def private merge_after_if(c : Sym; var then_env, else_env : table<uint64; Sym>;
                               then_flows, else_flows : bool; has_else : bool) : void {
        if (!then_flows && !else_flows) return
        if (then_flows && !else_flows) {
            // With no else, the fall-through path still carries the pre-if values.
            if (has_else) {
                env := then_env
            } else {
                merge_two(c, then_env, env)
            }
            return
        }
        if (!then_flows && else_flows) {
            env := else_env
            return
        }
        merge_two(c, then_env, else_env)
    }

    def private merge_two(c : Sym; var a, b : table<uint64; Sym>) : void {
        var merged : table<uint64; Sym>
        // `keys()` locks the table, so read with ?[] — plain [] may insert and panics.
        for (k in keys(a)) {
            var av = a?[k] ?? Sym()
            var bv = b?[k] ?? Sym()
            if (av.kind != bv.kind || av.kind == SymKind.UNKNOWN) continue
            if (av.ast == bv.ast) {
                merged[k] = av
            } else {
                merged[k] = Sym(kind = av.kind, ast = Z3_mk_ite(ctx, c.ast, av.ast, bv.ast))
            }
        }
        env := merged
    }

    def private exec_loop_body(cond : ExpressionPtr; body : ExpressionPtr) : bool {
        var written : array<uint64>
        collect_written(body, written)
        for (k in written) {
            if (key_exists(env, k)) {
                env |> erase(k)
            }
        }
        var pushed = false
        if (cond != null) {
            let c = to_sym(cond, 0)
            if (c.kind == SymKind.BOOL) {
                let saved = pc_arith_unproven
                sv |> push()
                assume_term(c.ast)
                pushed = true
                exec(body)
                sv |> pop()
                pc_arith_unproven = saved
            }
        }
        if (!pushed && body != null) {
            exec(body)
        }
        // The loop may run zero times, so nothing it established survives it.
        for (k in written) {
            if (key_exists(env, k)) {
                env |> erase(k)
            }
        }
        return false
    }

    def exec(stmt : ExpressionPtr) : bool {   // nolint:STYLE037,STYLE038 — exact-type dispatch over the statement node kinds; one arm per kind
        if (stmt == null || queries >= max_queries) return false

        if (stmt is ExprBlock) {
            let blk = stmt as ExprBlock
            if (blk.blockFlags.isClosure) {
                havoc_everything()
                return false
            }
            return exec_block(blk)
        }
        if (stmt is ExprLet) return exec_let(stmt as ExprLet)
        if (stmt is ExprCopy) {
            let cp = stmt as ExprCopy
            return exec_store(cp.left, cp.right)
        }
        if (stmt is ExprMove) {
            let mv = stmt as ExprMove
            return exec_store(mv.left, mv.right)
        }
        if (stmt is ExprClone) {
            let cl = stmt as ExprClone
            return exec_store(cl.left, cl.right)
        }
        if (stmt is ExprIfThenElse) {
            let ift = stmt as ExprIfThenElse
            if (ift.if_flags.isStatic) return false
            return exec_if(ift)
        }
        if (stmt is ExprWhile) {
            let wh = stmt as ExprWhile
            return exec_loop_body(wh.cond, wh.body)
        }
        if (stmt is ExprFor) {
            let ef = stmt as ExprFor
            for (it in ef.iteratorVariables) {
                havoc(it)
            }
            return exec_loop_body(null, ef.body)
        }
        if (stmt is ExprReturn || stmt is ExprBreak || stmt is ExprContinue) return true
        if (stmt is ExprAssert) {
            // Execution continues only if the assertion held.
            let ea = stmt as ExprAssert
            if (!empty(ea.arguments)) {
                let c = to_sym(ea.arguments[0], 0)
                if (c.kind == SymKind.BOOL) {
                    assume_term(c.ast)
                }
            }
            return false
        }
        if (stmt is ExprCall) {
            let call = stmt as ExprCall
            return (call.func != null && "{call.name}" == "panic"
                && call.func._module != null && "{call.func._module.name}" == "builtin")
        }
        if (stmt is ExprOp2) {
            let op = stmt as ExprOp2
            let ops = "{op.op}"
            if (ops == "+=" || ops == "-=" || ops == "*=" || ops == "/=" || ops == "%=") {
                // Only + - * rebuild exactly; anything else becomes unknown.
                if (op.left != null && op.left is ExprVar) {
                    let ev = op.left as ExprVar
                    let cur = var_sym(ev)
                    let rhs = to_sym(op.right, 0)
                    var done = false
                    if (cur.kind == rhs.kind && (ops == "+=" || ops == "-=" || ops == "*=")) {
                        let plain = slice(ops, 0, 1)
                        var r = unknown_sym()
                        if (cur.kind == SymKind.INT) {
                            r = op2_int(plain, cur, rhs)
                        } elif (cur.kind == SymKind.F32 || cur.kind == SymKind.F64) {
                            r = op2_fp(plain, cur, rhs)
                        }
                        if (r.kind != SymKind.UNKNOWN && ev.variable != null
                            && !key_exists(volatile_vars, var_key(ev.variable))) {
                            env[var_key(ev.variable)] = r
                            done = true
                        }
                    }
                    if (!done && ev.variable != null) {
                        havoc(ev.variable)
                    }
                    return false
                }
                havoc_everything()
            }
            return false
        }
        if (stmt is ExprMakeBlock) {
            havoc_everything()
            return false
        }
        // Unmodeled statement: forget values (escapes are already volatile).
        havoc_everything()
        return false
    }

    // ===== per-function driver =====

    def override preVisitFunction(var fn : FunctionPtr) : void {
        current_function = fn
        // Generics skipped: one instantiation's verdict cannot speak for the template.
        skip_function = (fn.flags.generated || fn.moreFlags.isTemplate
            || fn.fromGeneric != null || has_no_lint(fn))
        env |> clear()
        volatile_vars |> clear()
        pc_arith_unproven = false
        queries = 0
        if (skip_function || fn.body == null) return
        Z3_solver_reset(ctx, sv.solver)
        mark_escapes(fn.body)
        for (arg in fn.arguments) {
            let k = kind_of(arg._type)
            if (k != SymKind.UNKNOWN) {
                env[var_key(arg)] = Sym(kind = k, ast = Z3_mk_fresh_const(ctx, "{arg.name}", sort_of(k)))
            }
        }
        exec(fn.body)
        Z3_solver_reset(ctx, sv.solver)
    }

    def override visitFunction(var fn : FunctionPtr) : FunctionPtr {
        current_function = null
        return <- fn
    }

    def private mark_escapes(body : ExpressionPtr) : void {
        var scanner <- new EscapeScanner()
        make_visitor(*scanner) $(adapter) {
            visit(body, adapter)
        }
        for (k in scanner.escaped) {
            if (!key_exists(volatile_vars, k)) {
                volatile_vars |> insert(k)
            }
        }
        unsafe {
            delete scanner
        }
    }

    def private has_no_lint(fn : FunctionPtr) : bool {
        for (ann in fn.annotations) {
            if (ann.annotation != null && "{ann.annotation.name}" == "no_lint") return true
        }
        return false
    }
}

// Locals that cannot be tracked: address taken, passed where a callee could write
// them, or reachable from a closure. Collected up front so the answer does not
// depend on the order statements are visited in.
class private EscapeScanner : AstVisitor {
    escaped : array<uint64>
    closure_depth : int = 0

    def private note(v : Variable?) : void {
        if (v != null) {
            escaped |> push(intptr(v))
        }
    }

    def private note_arg(a : ExpressionPtr) : void {
        var inner = a
        if (inner != null && inner is ExprRef2Value) {
            inner = (inner as ExprRef2Value).subexpr
        }
        if (inner != null && inner is ExprVar) {
            note((inner as ExprVar).variable)
        }
    }

    def override preVisitExprCall(expr : ExprCall?) : void {
        let name = "{expr.name}"
        if (name == "addr" || name == "safe_addr") {
            for (a in expr.arguments) {
                note_arg(a)
            }
            return
        }
        for (i in range(length(expr.arguments))) {
            var mutable_param = true
            if (expr.func != null && i < length(expr.func.arguments)) {
                let pt = expr.func.arguments[i]._type
                mutable_param = pt != null && pt.flags.ref && !pt.flags.constant
            }
            if (mutable_param) {
                note_arg(expr.arguments[i])
            }
        }
    }

    def override preVisitExprBlock(blk : ExprBlock?) : void {
        if (blk.blockFlags.isClosure) {
            closure_depth++
        }
    }

    def override visitExprBlock(var blk : ExprBlock?) : ExpressionPtr {
        if (blk.blockFlags.isClosure) {
            closure_depth--
        }
        return <- blk
    }

    def override preVisitExprVar(expr : ExprVar?) : void {
        if (closure_depth > 0) {
            note(expr.variable)
        }
    }
}

// Collects the keys of every variable written anywhere in a subtree, so a loop can
// forget them before its body is analyzed.
class private WriteCollector : AstVisitor {
    hits : array<uint64>

    def private note(lhs : ExpressionPtr) : void {
        if (lhs != null && lhs is ExprVar) {
            let ev = lhs as ExprVar
            if (ev.variable != null) {
                hits |> push(intptr(ev.variable))
            }
        }
    }

    def override preVisitExprCopy(expr : ExprCopy?) : void {
        note(expr.left)
    }

    def override preVisitExprMove(expr : ExprMove?) : void {
        note(expr.left)
    }

    def override preVisitExprClone(expr : ExprClone?) : void {
        note(expr.left)
    }

    def override preVisitExprVar(expr : ExprVar?) : void {
        if (expr.varFlags.write && expr.variable != null) {
            hits |> push(intptr(expr.variable))
        }
    }
}

def private setup_timeout(s : Solver; ms : uint) : void {
    var p = Z3_mk_params(s.ctx)
    Z3_params_inc_ref(s.ctx, p)
    Z3_params_set_uint(s.ctx, p, Z3_mk_string_symbol(s.ctx, "timeout"), ms)
    Z3_solver_set_params(s.ctx, s.solver, p)
    Z3_params_dec_ref(s.ctx, p)
}

def private run(prog : ProgramPtr; var astVisitor : SmtLintVisitor) : int {
    var s = make_solver()
    setup_timeout(s, astVisitor.timeout_ms)
    astVisitor.sv = s
    astVisitor.ctx = dcc(s.ctx)
    make_visitor(astVisitor) $(adapter) {
        visit(prog, adapter)
    }
    release(s)
    return astVisitor.warning_count
}

//! Reports SMT001/SMT002 as compile-time style warnings (error code 31209).
def public smt_lint(prog : ProgramPtr; compile_time_errors : bool) : int {
    let disabled : table<string>
    return smt_lint(prog, compile_time_errors, disabled)
}

def public smt_lint(prog : ProgramPtr; compile_time_errors : bool; disabled_codes : table<string>) : int {
    var astVisitor <- new SmtLintVisitor()
    astVisitor.compile_time_errors = compile_time_errors
    astVisitor.disabled_codes := disabled_codes
    astVisitor.timeout_ms = timeout_from(prog)
    astVisitor.max_queries = max_queries_from(prog)
    let count = run(prog, *astVisitor)
    unsafe {
        delete astVisitor
    }
    return count
}

//! Collects warnings as strings instead of reporting them — for tests and tools.
def public smt_lint_collect(prog : ProgramPtr; var warnings : array<string>) : int {
    let disabled : table<string>
    let enabled : table<string>
    return smt_lint_collect(prog, warnings, disabled, enabled)
}

def public smt_lint_collect(prog : ProgramPtr; var warnings : array<string>;
                            disabled_codes, enabled_codes : table<string>) : int {
    var astVisitor <- new SmtLintVisitor()
    astVisitor.collect_warnings = true
    astVisitor.disabled_codes := disabled_codes
    astVisitor.enabled_codes := enabled_codes
    astVisitor.timeout_ms = timeout_from(prog)
    astVisitor.max_queries = max_queries_from(prog)
    let count = run(prog, *astVisitor)
    warnings |> push_from(astVisitor.warnings)
    unsafe {
        delete astVisitor
    }
    return count
}

def public smt_lint_collect_issues(prog : ProgramPtr; var issues : array<LintIssue>;
                                   disabled_codes, enabled_codes : table<string>) : int {
    var astVisitor <- new SmtLintVisitor()
    astVisitor.collect_warnings = true
    astVisitor.disabled_codes := disabled_codes
    astVisitor.enabled_codes := enabled_codes
    astVisitor.timeout_ms = timeout_from(prog)
    astVisitor.max_queries = max_queries_from(prog)
    let count = run(prog, *astVisitor)
    issues |> push_from(astVisitor.issues)
    unsafe {
        delete astVisitor
    }
    return count
}

def private timeout_from(prog : ProgramPtr) : uint {
    let ms = prog._options |> find_arg("_smt_lint_timeout_ms") ?as tInt ?? 0
    return ms > 0 ? uint(ms) : DEFAULT_TIMEOUT_MS
}

def private max_queries_from(prog : ProgramPtr) : int {
    let n = prog._options |> find_arg("_smt_lint_max_queries") ?as tInt ?? 0
    return n > 0 ? n : DEFAULT_MAX_QUERIES
}

[lint_macro]
class SmtLintMacro : AstPassMacro {
    def override apply(prog : ProgramPtr; _mod : Module?) : bool {
        let disabled <- build_lint_macro_disabled(prog)
        smt_lint(prog, true, disabled)
        return true
    }
}
