options gen2
options indenting = 4
options rtti
// this file pins linq-fold codegen shapes; splicing would dissolve
// them
options disable_auto_inline

require daslib/ast
require daslib/rtti
require daslib/ast_match
require daslib/ast_boost
require daslib/templates_boost
require daslib/linq
require daslib/linq_boost
require dastest/testing_boost public

var g_select_count_proj_hits = 0

def side_effect_select_proj(x : int) : int {
    g_select_count_proj_hits ++
    return x * 2
}

// Resolves the invoke wrapper's *effective* outer block. When the
// body uses `var inscope` (smart_ptr / table lifetimes), the typer
// wraps the whole body in an inner ExprBlock so its finally clause is
// scoped correctly. This helper returns the inner block in that case
// so for/let counting works the same way regardless of whether the
// splice used `var inscope`. Returns null if the body isn't the
// canonical invoke($block, $src) shape.
def invoke_outer_list(body_expr : Expression?) : ExprBlock? {
    if (!(body_expr is ExprInvoke)) return null
    let inv = body_expr as ExprInvoke
    if (empty(inv.arguments) || !(inv.arguments[0] is ExprMakeBlock)) return null
    let mb = inv.arguments[0] as ExprMakeBlock
    var outer = mb._block as ExprBlock
    if (outer == null) return null
    // Typer-inserted single-nested-block for `var inscope` finally
    // scoping: unwrap once.
    if (length(outer.list) == 1 && outer.list[0] is ExprBlock) {
        outer = outer.list[0] as ExprBlock
    }
    return outer
}

// Returns the number of ExprLet/ExprFor statements in the
// counter-lane invoke's inner block. Pure shortcut: `[var src, return
// length(src)]` → 0 for-loops. Impure loop: `[var src, var acc=0, for
// {...}, return acc]` → 1 for-loop.
def count_inner_for_loops(body_expr : Expression?) : int {
    let outer = invoke_outer_list(body_expr)
    if (outer == null) return -1
    var n = 0
    for (stmt in outer.list) {
        if (stmt is ExprFor) {
            n ++
        }
    }
    return n
}

def count_op2(expr : Expression?; op : string) : int {   // nolint:STYLE037 - one arm per node
    if (expr == null) return 0
    var n = 0
    if (expr is ExprOp2 && (expr as ExprOp2).op == op) {
        n ++
    }
    if (expr is ExprBlock) {
        let b = expr as ExprBlock
        for (s in b.list) {
            n += count_op2(s, op)
        }
        for (s in b.finalList) {
            n += count_op2(s, op)
        }
    } elif (expr is ExprFor) {
        let f = expr as ExprFor
        for (s in f.sources) {
            n += count_op2(s, op)
        }
        n += count_op2(f.body, op)
    } elif (expr is ExprIfThenElse) {
        let i = expr as ExprIfThenElse
        n += count_op2(i.cond, op)
        n += count_op2(i.if_true, op)
        n += count_op2(i.if_false, op)
    } elif (expr is ExprOp2) {
        let o = expr as ExprOp2
        n += count_op2(o.left, op)
        n += count_op2(o.right, op)
    } elif (expr is ExprCall) {
        let c = expr as ExprCall
        for (a in c.arguments) {
            n += count_op2(a, op)
        }
    } elif (expr is ExprMakeBlock) {
        let mb = expr as ExprMakeBlock
        n += count_op2(mb._block, op)
    } elif (expr is ExprInvoke) {
        let inv = expr as ExprInvoke
        for (a in inv.arguments) {
            n += count_op2(a, op)
        }
    } elif (expr is ExprReturn) {
        let r = expr as ExprReturn
        n += count_op2(r.subexpr, op)
    } elif (expr is ExprCopy) {
        let c = expr as ExprCopy
        n += count_op2(c.left, op)
        n += count_op2(c.right, op)
    } elif (expr is ExprMove) {
        let m = expr as ExprMove
        n += count_op2(m.left, op)
        n += count_op2(m.right, op)
    } elif (expr is ExprClone) {
        let c = expr as ExprClone
        n += count_op2(c.left, op)
        n += count_op2(c.right, op)
    } elif (expr is ExprOp1) {
        let o = expr as ExprOp1
        n += count_op2(o.subexpr, op)
    } elif (expr is ExprLet) {
        let l = expr as ExprLet
        for (v in l.variables) {
            if (v != null && v.init != null) {
                n += count_op2(v.init, op)
            }
        }
    }
    return n
}

def count_call(expr : Expression?; funcName : string) : int {   // nolint:STYLE037 - one arm each
    if (expr == null) return 0
    var n = 0
    if (expr is ExprCall) {
        let c = expr as ExprCall
        if (c.func != null) {
            // Generic instances carry a mangled `name` (e.g. `less`17876...`);
            // the un-mangled function name lives in `func.fromGeneric.name`.
            // Check both. `das_string` supports `==` directly so no `string()`
            // cast needed (PERF007).
            if (c.func.fromGeneric != null) {
                if (c.func.fromGeneric.name == funcName) {
                    n ++
                }
            } elif (c.func.name == funcName) {
                n ++
            }
        }
    }
    if (expr is ExprBlock) {
        let b = expr as ExprBlock
        for (s in b.list) {
            n += count_call(s, funcName)
        }
        for (s in b.finalList) {
            n += count_call(s, funcName)
        }
    } elif (expr is ExprFor) {
        let f = expr as ExprFor
        for (s in f.sources) {
            n += count_call(s, funcName)
        }
        n += count_call(f.body, funcName)
    } elif (expr is ExprIfThenElse) {
        let i = expr as ExprIfThenElse
        n += count_call(i.cond, funcName)
        n += count_call(i.if_true, funcName)
        n += count_call(i.if_false, funcName)
    } elif (expr is ExprOp2) {
        let o = expr as ExprOp2
        n += count_call(o.left, funcName)
        n += count_call(o.right, funcName)
    } elif (expr is ExprCall) {
        let c = expr as ExprCall
        for (a in c.arguments) {
            n += count_call(a, funcName)
        }
    } elif (expr is ExprMakeBlock) {
        let mb = expr as ExprMakeBlock
        n += count_call(mb._block, funcName)
    } elif (expr is ExprInvoke) {
        let inv = expr as ExprInvoke
        for (a in inv.arguments) {
            n += count_call(a, funcName)
        }
    } elif (expr is ExprReturn) {
        let r = expr as ExprReturn
        n += count_call(r.subexpr, funcName)
    } elif (expr is ExprOp1) {
        let o = expr as ExprOp1
        n += count_call(o.subexpr, funcName)
    } elif (expr is ExprLet) {
        let l = expr as ExprLet
        for (v in l.variables) {
            if (v != null && v.init != null) {
                n += count_call(v.init, funcName)
            }
        }
    }
    return n
}

def count_op1(expr : Expression?; op : string) : int {   // nolint:STYLE037 - one arm per node
    if (expr == null) return 0
    var n = 0
    if (expr is ExprOp1 && (expr as ExprOp1).op == op) {
        n ++
    }
    if (expr is ExprBlock) {
        let b = expr as ExprBlock
        for (s in b.list) {
            n += count_op1(s, op)
        }
        for (s in b.finalList) {
            n += count_op1(s, op)
        }
    } elif (expr is ExprFor) {
        let f = expr as ExprFor
        for (s in f.sources) {
            n += count_op1(s, op)
        }
        n += count_op1(f.body, op)
    } elif (expr is ExprIfThenElse) {
        let i = expr as ExprIfThenElse
        n += count_op1(i.cond, op)
        n += count_op1(i.if_true, op)
        n += count_op1(i.if_false, op)
    } elif (expr is ExprOp2) {
        let o = expr as ExprOp2
        n += count_op1(o.left, op)
        n += count_op1(o.right, op)
    } elif (expr is ExprCall) {
        let c = expr as ExprCall
        for (a in c.arguments) {
            n += count_op1(a, op)
        }
    } elif (expr is ExprMakeBlock) {
        let mb = expr as ExprMakeBlock
        n += count_op1(mb._block, op)
    } elif (expr is ExprInvoke) {
        let inv = expr as ExprInvoke
        for (a in inv.arguments) {
            n += count_op1(a, op)
        }
    } elif (expr is ExprReturn) {
        let r = expr as ExprReturn
        n += count_op1(r.subexpr, op)
    } elif (expr is ExprOp1) {
        let o = expr as ExprOp1
        n += count_op1(o.subexpr, op)
    } elif (expr is ExprLet) {
        let l = expr as ExprLet
        for (v in l.variables) {
            if (v != null && v.init != null) {
                n += count_op1(v.init, op)
            }
        }
    }
    return n
}

// Count ExprConstInt(value) occurrences anywhere in the expression
// tree. Used by the select+where dedup tests to prove that a pure
// projection's signature constant (`_ * 7919`) appears exactly once
// in the emission (in the projection-bind statement), rather than
// twice (inlined into predicate + cloned into valueExpr).
def count_const_int(expr : Expression?; value : int) : int {   // nolint:STYLE037 — exact-type `is` ladder, one arm per AST node kind
    if (expr == null) return 0
    var n = 0
    if (expr is ExprConstInt && (expr as ExprConstInt).value == value) {
        n ++
    }
    if (expr is ExprBlock) {
        let b = expr as ExprBlock
        for (s in b.list) {
            n += count_const_int(s, value)
        }
        for (s in b.finalList) {
            n += count_const_int(s, value)
        }
    } elif (expr is ExprFor) {
        let f = expr as ExprFor
        for (s in f.sources) {
            n += count_const_int(s, value)
        }
        n += count_const_int(f.body, value)
    } elif (expr is ExprIfThenElse) {
        let i = expr as ExprIfThenElse
        n += count_const_int(i.cond, value)
        n += count_const_int(i.if_true, value)
        n += count_const_int(i.if_false, value)
    } elif (expr is ExprOp2) {
        let o = expr as ExprOp2
        n += count_const_int(o.left, value)
        n += count_const_int(o.right, value)
    } elif (expr is ExprOp1) {
        let o = expr as ExprOp1
        n += count_const_int(o.subexpr, value)
    } elif (expr is ExprCall) {
        let c = expr as ExprCall
        for (a in c.arguments) {
            n += count_const_int(a, value)
        }
    } elif (expr is ExprMakeBlock) {
        let mb = expr as ExprMakeBlock
        n += count_const_int(mb._block, value)
    } elif (expr is ExprInvoke) {
        let inv = expr as ExprInvoke
        for (a in inv.arguments) {
            n += count_const_int(a, value)
        }
    } elif (expr is ExprReturn) {
        let r = expr as ExprReturn
        n += count_const_int(r.subexpr, value)
    } elif (expr is ExprLet) {
        let l = expr as ExprLet
        for (v in l.variables) {
            if (v != null && v.init != null) {
                n += count_const_int(v.init, value)
            }
        }
    }
    return n
}

// Counts top-level `var` declarations in the outer block of an invoke
// wrapper. sum/long_count emit one accumulator; min/max emit two
// (first flag + best); average emits two (sum acc + count).
def count_outer_let_vars(body_expr : Expression?) : int {
    let outer = invoke_outer_list(body_expr)
    if (outer == null) return -1
    var n = 0
    for (stmt in outer.list) {
        if (stmt is ExprLet) n ++
    }
    return n
}

[export, marker(no_coverage)]
def target_where_take_count_fold() : int =>
    _fold(each([1, 2, 3, 4, 5, 6, 7, 8, 9, 10])._where(_ % 2 == 0).take(3).count())

[export, marker(no_coverage)]
def target_skip_take_to_array_fold() : array<int> =>
    <- _fold(each([1, 2, 3, 4, 5, 6, 7]).skip(2).take(3).to_array())

[export, marker(no_coverage)]
def target_order_by_take_falls_through() : array<int> {
    // Buffer-required (order_by) → splice planner returns null → silent
    // fallback to raw linq.
    return <- _fold([1, 2, 3, 4, 5].to_sequence()._select(_ * 2)._order_by(_).take(2).to_array())
}

[export, marker(no_coverage)]
def target_distinct_to_array_fold() : array<int> {
    // Phase 3+ / D1: plan_distinct splices into streaming dedup (seen-set
    // + buffer populated in a single pass). Prior to D1 this chain
    // cascaded to tier 2.
    return <- _fold([1, 2, 2, 3, 3, 3].to_sequence().distinct().to_array())
}

[export, marker(no_coverage)]
def target_take_sum_fold() : int => _fold([1, 2, 3, 4, 5]._select(_ * 10).take(3).sum())

// AST helper — total break/continue count anywhere in the subtree.
// Used to assert that the take-limit `break` and skip-counter
// `continue` actually got spliced into the per-element body.
def count_break_continue(expr : Expression?; want_break : bool) : int {
    if (expr == null) return 0
    var n = 0
    if (want_break && expr is ExprBreak) {
        n ++
    } elif (!want_break && expr is ExprContinue) {
        n ++
    }
    if (expr is ExprBlock) {
        let b = expr as ExprBlock
        for (s in b.list) {
            n += count_break_continue(s, want_break)
        }
        for (s in b.finalList) {
            n += count_break_continue(s, want_break)
        }
    } elif (expr is ExprFor) {
        let f = expr as ExprFor
        for (s in f.sources) {
            n += count_break_continue(s, want_break)
        }
        n += count_break_continue(f.body, want_break)
    } elif (expr is ExprIfThenElse) {
        let i = expr as ExprIfThenElse
        n += count_break_continue(i.cond, want_break)
        n += count_break_continue(i.if_true, want_break)
        n += count_break_continue(i.if_false, want_break)
    } elif (expr is ExprMakeBlock) {
        let mb = expr as ExprMakeBlock
        n += count_break_continue(mb._block, want_break)
    } elif (expr is ExprInvoke) {
        let inv = expr as ExprInvoke
        for (a in inv.arguments) {
            n += count_break_continue(a, want_break)
        }
    }
    return n
}

[test(description = "where, take and count fold into one for loop whose take guard breaks")]
def test_take_splices_break_into_loop(t : T?) {
    // where.take.count: counter lane with take guard → one for-loop + at
    // least one break (the `if (taken == N) break` guard).
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_where_take_count_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper for splice")
        t |> equal(1, count_inner_for_loops(body_expr), "single fused for-loop")
        t |> success(count_break_continue(body_expr, true) >= 1, "take guard must emit a break")
    }
}

[test(description = "skip then take then to_array fold into one for loop with a continue for the skip and a break for the take")]
def test_skip_take_splices_continue_and_break(t : T?) {
    // skip.take.to_array: array lane with both guards → continue (skip) +
    // break (take).
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_skip_take_to_array_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper")
        t |> equal(1, count_inner_for_loops(body_expr), "single fused for-loop")
        t |> success(count_break_continue(body_expr, true) >= 1, "take guard must emit a break")
        t |> success(count_break_continue(body_expr, false) >= 1, "skip guard must emit a continue")
    }
}

[test(description = "take then sum fold into one for loop with an accumulator and a break")]
def test_take_sum_splices_in_accumulator(t : T?) {
    // select.take.sum: accumulator lane with take → one for-loop + acc +=
    // ... + break.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_take_sum_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper")
        t |> equal(1, count_inner_for_loops(body_expr), "single fused for-loop")
        t |> success(count_op2(body_expr, "+=") >= 1, "sum must emit `+=` in accumulator")
        t |> success(count_break_continue(body_expr, true) >= 1, "take guard must emit a break")
    }
}

[test(description = "an order_by and take chain that the splice does not cover falls back to the unfused path")]
def test_order_by_take_cascades_to_tier2(t : T?) {
    // `order_by |> take` — Phase 3 wires a BufferTopN splice arm via
    // top_n_by. Until then, the splice planner returns null and the
    // cascade falls to tier 2 (fold_linq_default), which emits the
    // array-shape invoke wrapper.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_order_by_take_falls_through)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched, "should have return expression")
        t |> success(body_expr is ExprInvoke,
            "order_by |> take should cascade to fold_linq_default (invoke wrapper)")
    }
}

[test(description = "distinct then to_array folds into one for loop that dedups as it streams")]
def test_distinct_to_array_emits_streaming_dedup(t : T?) {
    // D1: bare distinct + to_array splices into streaming dedup — single
    // fused for-loop with one key_exists check + one insert + one
    // push_clone per source element. No call to the runtime
    // distinct/distinct_inplace helpers. Anti-cascade gate: exactly two
    // outer let-vars (the seen-set + the buffer).
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_distinct_to_array_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper for splice")
        t |> equal(1, count_inner_for_loops(body_expr), "single fused for-loop")
        t |> equal(0, count_call(body_expr, "distinct"),
            "distinct runtime helper must NOT be called (splice elides it)")
        t |> equal(0, count_call(body_expr, "distinct_inplace"),
            "distinct_inplace must NOT be called (streaming dedup, not eager)")
        t |> success(count_call(body_expr, "key_exists") >= 1,
            "must emit key_exists check on the seen-set")
        t |> success(count_call(body_expr, "push_clone") >= 1,
            "must push unique elements into the buffer")
        t |> equal(2, count_outer_let_vars(body_expr),
            "exactly two outer vars (seen-set + buffer) — anti-cascade gate")
    }
}

[export, marker(no_coverage)]
def target_order_by_take_splices_fold() : array<int> =>
    <- _fold(each([10, 20, 5, 8, 30, 15, 2, 25])._order_by(_).take(3).to_array())

[export, marker(no_coverage)]
def target_order_by_descending_take_splices_fold() : array<int> =>
    <- _fold(each([10, 20, 5, 8, 30, 15, 2, 25])._order_by_descending(_).take(3).to_array())

[export, marker(no_coverage)]
def target_bare_order_by_splices_fold() : array<int> =>
    <- _fold(each([10, 20, 5, 8, 30])._order_by(_).to_array())

[export, marker(no_coverage)]
def target_where_order_by_splices_fold() : array<int> =>
    <- _fold(each([10, 20, 5, 8, 30, 15, 2, 25])._where(_ > 5)._order_by(_).to_array())

[export, marker(no_coverage)]
def target_where_order_by_take_splices_fold() : array<int> =>
    <- _fold(each([10, 20, 5, 8, 30, 15, 2, 25])._where(_ > 5)._order_by(_).take(2).to_array())

struct private MkRec {
    a : int
    b : int
}

[export, marker(no_coverage)]
def target_where_order_by_keys_splices_fold() : array<MkRec> {
    // where + multi-key order_by_keys (a asc, b desc) over pure field
    // keys → fused buffer + one composite stable_sort
    return <- _fold(
                    each(
                         [MkRec(a = 2, b = 1), MkRec(a = 1, b = 2), MkRec(a = 1, b = 1), MkRec(a = 3, b = 0)]) ._where(_.a >= 0)._order_by_keys((_.a, _.b), 2u).to_array()) } [test(description = "order_by then take folds into a bounded heap with a final in-place order and no top_n_by or order_by call")]
def test_order_by_take_emits_top_n_by_with_cmp(t : T?) {
    // `order_by(key) |> take(K)` with an inlineable key now splices to
    // the bounded-heap walk: `spliced_push_heap` fill+replace pair + a
    // `_::less(it, buf[0])` replace-test on the heap top,
    // `order_inplace(buf, cmp)` at the end. No
    // top_n_by/top_n_by_with_cmp.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_order_by_take_splices_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched, "should have return expression")
        t |> equal(count_call(body_expr, "spliced_push_heap"), 2,
                   "bounded-heap fill + replace each call spliced_push_heap")
        t |> equal(count_call(body_expr, "spliced_pop_heap"), 1, "bounded-heap replace pops first")
        t |> success(count_call(body_expr, "order_inplace") >= 1,
                     "final order_inplace on the bounded buf")
        t |> equal(0, count_call(body_expr, "top_n_by_with_cmp"),
                   "bounded-heap supersedes top_n_by_with_cmp")
        t |> equal(0, count_call(body_expr, "top_n_by"), "should not emit non-cmp top_n_by")
        t |> equal(0, count_call(body_expr, "order_by"), "should not emit order_by")
        t |> equal(0, count_call(body_expr, "take"), "should not emit take")
    }
}

[test(description = "order_by_descending then take folds into the same bounded heap path")]
def test_order_by_descending_take_emits_top_n_by_with_cmp(t : T?) {
    // `order_by_descending(key) |> take(K)` with an inlineable key hits
    // the same bounded-heap entry as ascending — direction is encoded by
    // flipping the inline less-test's operand order (`_::less(buf[0],
    // it)`).
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
            @@target_order_by_descending_take_splices_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched, "should have return expression")
        t |> equal(count_call(body_expr, "spliced_push_heap"), 2, "bounded-heap fill + replace each call spliced_push_heap (descending shares the same path)")
        t |> equal(count_call(body_expr, "spliced_pop_heap"), 1, "bounded-heap replace pops first")
        t |> equal(0, count_call(body_expr, "top_n_by_with_cmp"),
                   "bounded-heap supersedes top_n_by_with_cmp")
        t |> equal(0, count_call(body_expr, "top_n_by_descending"),
            "should not emit the key-taking top_n_by_descending")
        t |> equal(0, count_call(body_expr, "order_by_descending"),
            "should not emit order_by_descending")
    }
}

[test(description = "a bare order_by folds to an order call with an inline comparator, not the key-taking order_by or top_n_by")]
def test_bare_order_by_emits_direct_call(t : T?) {
    // Bare `order_by(key)` with an inlineable key dispatches to the asc
    // `order(src, cmp)` block-taking overload — the key body is spliced
    // into the comparator twice.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_bare_order_by_splices_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched, "should have return expression")
        t |> success(count_call(body_expr, "order") >= 1, "should emit an order(src, cmp) call")
        t |> equal(0, count_call(body_expr, "order_by"), "should not emit the key-taking order_by")
        t |> equal(0, count_call(body_expr, "top_n_by"), "should not emit top_n_by")
    }
}

[test(description = "where then order_by folds into one prefilter loop and an in-place order with an inline comparator")]
def test_where_order_by_emits_fused_loop_with_inline_cmp(t : T?) {
    // `where |> order_by(key)` splices into a fused prefilter loop +
    // order_inplace(buf, cmp) when the key is inlineable — the comparator
    // embeds the key body twice.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_where_order_by_splices_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke,
                     "expected invoke wrapper for fused emission")
        t |> equal(1, count_inner_for_loops(body_expr), "single fused prefilter loop")
        t |> success(count_call(body_expr, "order_inplace") >= 1,
                     "should call order_inplace(buf, cmp)")
        t |> equal(0, count_call(body_expr, "order_by_inplace"),
                   "should not emit the key-taking order_by_inplace")
        t |> success(count_call(body_expr, "push_clone") >= 1, "should push_clone into the buffer")
    }
}

[test(description = "where, order_by and take fold into one walk with a bounded heap and a final in-place order")]
def test_where_order_by_take_emits_fused_top_n_with_cmp(t : T?) {
    // `where |> order_by(key) |> take(K)` with an inlineable key splices
    // into the bounded-heap walk wrapped in `if (whereCond)`. Single
    // fused loop — no prefilter buffer, the heap IS the buffer, and at
    // most K elements ever live in it.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
            @@target_where_order_by_take_splices_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke,
                     "expected invoke wrapper for fused emission")
        t |> equal(1, count_inner_for_loops(body_expr), "single fused walk over source")
        t |> equal(count_call(body_expr, "spliced_push_heap"), 2,
                   "bounded-heap fill + replace each call spliced_push_heap")
        t |> equal(count_call(body_expr, "spliced_pop_heap"), 1, "bounded-heap replace pops first")
        t |> success(count_call(body_expr, "order_inplace") >= 1,
                     "final order_inplace on the bounded buf")
        t |> equal(0, count_call(body_expr, "top_n_by_with_cmp"),
                   "bounded-heap supersedes top_n_by_with_cmp")
        t |> equal(0, count_call(body_expr, "top_n_by"), "should not emit non-cmp top_n_by")
        t |> success(count_call(body_expr, "push_clone") >= 1,
                     "should push_clone into the heap during fill")
    }
}

[test(description = "where then a multi-key order_by folds into one loop and a single composite stable sort, not the unstable in-place order")]
def test_where_order_by_keys_emits_stable_sort(t : T?) {
    // where + multi-key order_by_keys with an inlineable tuple key →
    // fused prefilter loop + a SINGLE composite stable_sort(buf, cmp). It
    // must NOT use the unstable single-key order_inplace path, and an
    // inlineable key must splice (no cascade to the eager
    // order_by_keys_inplace).
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_where_order_by_keys_splices_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke,
                     "expected invoke wrapper for fused emission")
        t |> equal(1, count_inner_for_loops(body_expr), "single fused prefilter loop")
        t |> equal(1, count_call(body_expr, "stable_sort"),
                   "exactly one composite stable_sort on the fused buffer")
        t |> equal(0, count_call(body_expr, "order_inplace"),
                   "multi-key must NOT use the unstable order_inplace")
        t |> equal(0, count_call(body_expr, "order_by_keys_inplace"),
                   "inlineable key splices — no eager cascade")
        t |> success(count_call(body_expr, "push_clone") >= 1, "should push_clone into the buffer")
    }
}

[test(description = "a single-key order_by uses the in-place order and never a stable sort")]
def test_single_key_order_by_stays_unstable(t : T?) {
    // Regression guard: single-key order_by keeps the unstable
    // sort/order_inplace path. The multi-key stable_sort change must NOT
    // leak into single-key orderby (no perf regression on the common
    // case).
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_where_order_by_splices_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched, "should have return expression")
        t |> success(count_call(body_expr, "order_inplace") >= 1, "single-key uses order_inplace")
        t |> equal(0, count_call(body_expr, "stable_sort"), "single-key must NOT emit stable_sort")
    }
}

[test(description = "order_by then take returns the smallest elements in order")]
def test_order_by_take_correct_result(t : T?) {
    t |> run("order_by + take splice produces N smallest ascending") @(t : T?) {
        let got <- target_order_by_take_splices_fold()
        let expected = [2, 5, 8]
        t |> equal(length(got), length(expected))
        for (v, e in got, expected) {
            t |> equal(v, e)
        }
    }
}

[test(description = "order_by_descending then take returns the largest elements in order")]
def test_order_by_descending_take_correct_result(t : T?) {
    t |> run("order_by_descending + take splice produces N largest descending") @(t : T?) {
        let got <- target_order_by_descending_take_splices_fold()
        let expected = [30, 25, 20]
        t |> equal(length(got), length(expected))
        for (v, e in got, expected) {
            t |> equal(v, e)
        }
    }
}

[test(description = "where, order_by and take return the smallest passing elements in order")]
def test_where_order_by_take_correct_result(t : T?) {
    t |> run("where + order_by + take splice prefilters then takes top N") @(t : T?) {
        let got <- target_where_order_by_take_splices_fold()
        // src = [10, 20, 5, 8, 30, 15, 2, 25], where _>5 → [10, 20, 8, 30,
        // 15, 25] order_by(_).take(2) → [8, 10]
        let expected = [8, 10]
        t |> equal(length(got), length(expected))
        for (v, e in got, expected) {
            t |> equal(v, e)
        }
    }
}

[export, marker(no_coverage)]
def target_select_where_count_splices_fold() : int =>
    _fold(each([1, 2, 3, 4, 5, 6, 7, 8])._select(_ * 2)._where(_ > 5).count())

[export, marker(no_coverage)]
def target_select_where_sum_splices_fold() : int =>
    _fold(each([1, 2, 3, 4, 5])._select(_ * 10)._where(_ > 15).sum())

[export, marker(no_coverage)]
def target_select_where_any_splices_fold() : bool =>
    _fold(each([1, 2, 3])._select(_ * 100)._where(_ > 50).any())

[export, marker(no_coverage)]
def target_select_where_to_array_splices_fold() : array<int> =>
    <- _fold(each([1, 2, 3, 4, 5])._select(_ * 2)._where(_ > 4).to_array())

[test(description = "select, where and count fold into one for loop with both calls inlined")]
def test_select_where_count_emits_fused_loop(t : T?) {
    // select |> where |> count: peel-substitution inlines projection into
    // pred; loop emits counter increments. NOT tier 2 (no pass_0/pass_1
    // binding shape).
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
            @@target_select_where_count_splices_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper for splice")
        t |> equal(1, count_inner_for_loops(body_expr),
                   "single fused for-loop (not pass_0/pass_1 chain)")
        // tier 2 fold_linq_default would emit `var pass_0 = ...; var pass_1 =
        // ...`. tier 1 splice emits just the loop body + accumulator.
        t |> equal(0, count_call(body_expr, "where_"), "where_ should be inlined, not called")
        t |> equal(0, count_call(body_expr, "select"), "select should be inlined, not called")
    }
}

[test(description = "select, where and count return the right count")]
def test_select_where_count_correct_result(t : T?) {
    t |> run("select(x*2) |> where(>5) |> count") @(t : T?) {
        // src = [1..8], projected = [2,4,6,8,10,12,14,16], filtered >5 =
        // [6,8,10,12,14,16], count = 6
        t |> equal(6, target_select_where_count_splices_fold())
    }
}

[test(description = "select, where and sum return the right total")]
def test_select_where_sum_correct_result(t : T?) {
    t |> run("select(x*10) |> where(>15) |> sum") @(t : T?) {
        // src = [1..5], projected = [10,20,30,40,50], filtered >15 =
        // [20,30,40,50], sum = 140
        t |> equal(140, target_select_where_sum_splices_fold())
    }
}

[test(description = "select, where and any return the right boolean")]
def test_select_where_any_correct_result(t : T?) {
    t |> run("select(x*100) |> where(>50) |> any") @(t : T?) {
        // src = [1,2,3], projected = [100,200,300], filtered >50 =
        // [100,200,300], any = true
        t |> equal(true, target_select_where_any_splices_fold())
    }
}

[test(description = "select, where and to_array return the right elements")]
def test_select_where_to_array_correct_result(t : T?) {
    t |> run("select(x*2) |> where(>4) |> to_array") @(t : T?) {
        // src = [1..5], projected = [2,4,6,8,10], filtered >4 = [6,8,10]
        let got <- target_select_where_to_array_splices_fold()
        let expected = [6, 8, 10]
        t |> equal(length(got), length(expected))
        for (v, e in got, expected) {
            t |> equal(v, e)
        }
    }
}

[export, marker(no_coverage)]
def target_select_where_sum_dedup() : int {
    // Pure projection with a distinctive constant (7919, prime). After
    // the splice the constant appears EXACTLY ONCE in the body — in the
    // projection-bind statement. Without the dedup the constant would
    // appear twice: once inlined into the predicate via
    // peel-substitution, and once cloned into the sum's valueExpr.
    return _fold(each([1, 2, 3, 4, 5])._select(_ * 7919)._where(_ > 0).sum())
}

[test(description = "select, where and sum fold into one loop with select and where inlined")]
def test_select_where_projection_dedup(t : T?) {
    // Single-eval splice: the projection constant 7919 must appear
    // exactly once in the body — proves the projection is bound to a
    // local once per element rather than inlined twice (once in
    // predicate, once in valueExpr).
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_select_where_sum_dedup)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched, "should have return expression")
        t |> equal(1, count_const_int(body_expr, 7919),
            "projection constant should appear exactly once (single-eval dedup)")
        t |> equal(1, count_inner_for_loops(body_expr), "single fused for-loop")
        t |> equal(0, count_call(body_expr, "select"), "select should be inlined")
        t |> equal(0, count_call(body_expr, "where_"), "where_ should be inlined")
    }
}

[test(description = "select, where and sum return the right total")]
def test_select_where_dedup_correct_result(t : T?) {
    t |> run("select+where dedup preserves arithmetic") @(t : T?) {
        // [1,2,3,4,5] * 7919 = [7919, 15838, 23757, 31676, 39595], all > 0,
        // sum = 7919 * (1+2+3+4+5) = 7919 * 15 = 118785
        t |> equal(118785, target_select_where_sum_dedup())
    }
}

[export, marker(no_coverage)]
def target_select_where_impure_falls_to_tier2() : int {
    // Impure projection (side-effecting function call) must NOT be moved
    // outside the where if-wrap — that would visibly fire side effects on
    // filter-rejected elements. The splice arm bails to tier 2 cascade in
    // this case.
    return _fold(each([1, 2, 3, 4, 5])._select(side_effect_select_proj(_))._where(_ > 5).count())
}

[test(description = "an impure select before a where fires once per source element")]
def test_select_where_impure_correctness(t : T?) {
    t |> run("impure projection in select+where preserves per-element call count") @(t : T?) {
        // Plain LINQ semantics: select runs the projection for EVERY element
        // before the where filter sees the result. 5 input elements → 5
        // projection invocations, regardless of how many survive the filter.
        g_select_count_proj_hits = 0
        let n = target_select_where_impure_falls_to_tier2()
        // projected: [2,4,6,8,10]; filtered >5: [6,8,10]; count = 3
        t |> equal(3, n)
        t |> equal(5, g_select_count_proj_hits, "projection must fire once per source element")
    }
}

[export, marker(no_coverage)]
def target_select_distinct_impure() : array<int> {
    // Side-effecting projection before distinct. Splice MUST bind the
    // projected value once per source element (and use the bind for both
    // the seen-key and the buffer push) so the projection fires once per
    // element — matching plain LINQ's iterator-pipeline behavior.
    return <- _fold([1, 2, 3, 4, 5].to_sequence()._select(side_effect_select_proj(_)).distinct().to_array())
}

[test(description = "an impure select before distinct fires once per source element, not twice")]
def test_select_distinct_impure_correctness(t : T?) {
    t |> run("impure projection in select+distinct fires once per source element") @(t : T?) {
        // Naive emission evaluates `side_effect_select_proj(it)` twice per
        // element (once for the unique_key lookup, once for push_clone into
        // the buffer) → counter would hit 5 * 2 = 10. The per-element bind
        // fix keeps it at 5.
        g_select_count_proj_hits = 0
        let out <- target_select_distinct_impure()
        t |> equal(5, length(out))
        t |> equal(5, g_select_count_proj_hits,
                   "projection must fire once per source element (not twice)")
    }
}

var g_order_key_hits = 0

def side_effect_order_key(x : int) : int {
    g_order_key_hits ++
    return -x
}

[export, marker(no_coverage)]
def target_order_by_take_side_effecting_key() : array<int> {
    // Side-effecting key — try_make_inline_cmp's has_sideeffects gate
    // must bail; the splice falls back to the keyed top_n_by entry point
    // (not _with_cmp).
    return <- _fold(each([3, 1, 4, 1, 5, 9, 2, 6])._order_by(side_effect_order_key(_)).take(3).to_array())
}

[test(description = "order_by with a side-effecting key and take falls back to the unfused path")]
def test_order_by_take_side_effecting_key_falls_back(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
            @@target_order_by_take_side_effecting_key)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched, "should have return expression")
        t |> success(count_call(body_expr, "top_n_by") >= 1,
            "side-effecting key should fall back to keyed top_n_by")
        t |> equal(0, count_call(body_expr, "top_n_by_with_cmp"),
            "side-effecting key must NOT use the inline-cmp variant")
    }
}

[test(description = "order_by with a side-effecting key and take returns the right elements")]
def test_order_by_take_side_effecting_key_correct_result(t : T?) {
    t |> run("side-effecting order_by key produces correct top-N by -key") @(t : T?) {
        let got <- target_order_by_take_side_effecting_key()
        // -key sorts ascending by -x ⇒ descending by x; take 3 largest.
        let expected = [9, 6, 5]
        t |> equal(length(got), length(expected))
        for (v, e in got, expected) {
            t |> equal(v, e)
        }
    }
}

[export, marker(no_coverage)]
def target_reverse_to_array_fold() : array<int> =>
    <- _fold(each([1, 2, 3, 4, 5]) |> reverse() |> to_array())

[test(description = "reverse then to_array folds into one for loop followed by an in-place reverse")]
def test_reverse_to_array_emits_single_loop_plus_inplace(t : T?) {
    // R1: bare reverse + to_array splices into a single fused for-loop
    // that pushes the source into a buffer, then calls reverse_inplace.
    // Anti-cascade gate: tier-2 cascade would emit `var pass_0 = ...;
    // delete pass_0; var pass_1 = ...` with multiple outer vars, so the
    // single-buffer assertion proves splice fired.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_reverse_to_array_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper for splice")
        t |> equal(1, count_inner_for_loops(body_expr), "single fused for-loop")
        t |> success(count_call(body_expr, "reverse_inplace") >= 1,
            "must emit reverse_inplace call on the buffer")
        t |> equal(0, count_call(body_expr, "reverse"),
            "reverse runtime helper must NOT be called (splice elides it)")
        t |> equal(1, count_outer_let_vars(body_expr),
            "single outer var (the buffer) — anti-cascade gate")
    }
}

[export, marker(no_coverage)]
def target_where_reverse_to_array_fold() : array<int> =>
    <- _fold(each([1, 2, 3, 4, 5])._where(_ > 2) |> reverse() |> to_array())

[test(description = "where then reverse then to_array folds into one for loop")]
def test_reverse_with_where_fuses_into_loop(t : T?) {
    // R2: where_ before reverse fuses into the prefilter loop. Buffer
    // accumulates only matching elements; reverse_inplace then mutates
    // that buffer.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_where_reverse_to_array_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper for splice")
        t |> equal(1, count_inner_for_loops(body_expr), "single fused for-loop")
        t |> success(count_call(body_expr, "reverse_inplace") >= 1,
            "must emit reverse_inplace call")
        t |> equal(0, count_call(body_expr, "where_"),
            "where_ runtime helper must NOT be called (predicate inlined)")
        t |> equal(1, count_outer_let_vars(body_expr),
            "single outer var (the buffer) — anti-cascade gate")
    }
}

[export, marker(no_coverage)]
def target_select_reverse_to_array_fold() : array<int> =>
    <- _fold(each([1, 2, 3, 4, 5])._select(_ * 10) |> reverse() |> to_array())

[export, marker(no_coverage)]
def target_reverse_take_to_array_fold() : array<int> =>
    <- _fold(each([1, 2, 3, 4, 5, 6, 7, 8]) |> reverse() |> take(3) |> to_array())

[test(description = "reverse then take then to_array folds into one backward index loop")]
def test_reverse_take_emits_backward_index_loop(t : T?) {
    // R6: array-typed source + reverse |> take(N) — backward index loop
    // instead of (push all → reverse_inplace → resize). Visits only the
    // last N indices, skipping the O(length) push and O(length) reverse
    // pass. Closes the m3 lazy-yield regression.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_reverse_take_to_array_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper for splice")
        t |> equal(1, count_inner_for_loops(body_expr), "single fused for-loop")
        t |> equal(0, count_call(body_expr, "reverse_inplace"),
            "must NOT emit reverse_inplace (backward index loop replaces it)")
        t |> equal(0, count_call(body_expr, "resize"),
            "must NOT emit resize (reserve(takeN) replaces it)")
        t |> equal(0, count_call(body_expr, "take"),
            "take runtime helper must NOT be called")
        t |> success(count_call(body_expr, "reserve") >= 1,
            "must emit reserve(takeN) to pre-size the buffer")
        t |> success(count_call(body_expr, "length") >= 1,
            "must call length(src) to bound the backward walk")
        t |> equal(3, count_outer_let_vars(body_expr),
            "three outer vars: source-length, clamped-takeN, buffer")
    }
}

[export, marker(no_coverage)]
def target_reverse_take_iterator_fold() : array<int> {
    // Iterator source (range) — backward index loop is NOT applicable,
    // falls back to R1-R4 path (push all → reverse_inplace → resize).
    return <- _fold(each(range(1, 9)) |> reverse() |> take(3) |> to_array())
}

[test(description = "reverse then take over an iterator source folds into one prefilter loop with an in-place reverse")]
def test_reverse_take_iterator_source_uses_reverse_inplace(t : T?) {
    // R6 negative: when source is not indexable (range / iterator), the
    // R1-R4 fallback path stays in place. Pins the full fallback shape
    // (not just reverse_inplace/resize presence) so the test catches both
    // directions of drift: (a) R6 starts mis-firing on iterator sources,
    // (b) the R1-R4 emission shape changes (e.g. cascades to take
    // helper).
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_reverse_take_iterator_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper for splice")
        t |> equal(1, count_inner_for_loops(body_expr), "single fused prefilter loop (R1-R4)")
        t |> success(count_call(body_expr, "reverse_inplace") >= 1,
            "iterator source must use reverse_inplace fallback")
        t |> success(count_call(body_expr, "resize") >= 1,
            "iterator source must use resize fallback")
        t |> equal(0, count_call(body_expr, "take"),
            "take runtime helper must NOT be called (resize replaces it)")
        t |> equal(1, count_outer_let_vars(body_expr),
            "single outer var (the buffer) — anti-cascade gate")
    }
}

[export, marker(no_coverage)]
def target_reverse_count_fold() : int =>
    _fold(each([1, 2, 3, 4, 5])._where(_ > 2) |> reverse() |> count())

[test(description = "reverse then count folds into a counter increment with no buffer and no reverse")]
def test_reverse_count_elides_buffer_and_reverse(t : T?) {
    // R5: reverse |> count — reverse is identity for count. No buffer, no
    // reverse_inplace, just a counter loop over surviving elements.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_reverse_count_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper for splice")
        t |> equal(0, count_call(body_expr, "reverse_inplace"),
            "reverse_inplace must NOT be emitted — reverse is identity for count")
        t |> equal(0, count_call(body_expr, "push_clone"),
            "no buffer pushes — count elides the buffer entirely")
        t |> success(count_op1(body_expr, "++") >= 1, "counter increment must be present")
    }
}

[export, marker(no_coverage)]
def target_reverse_first_fold() : int =>
    _fold(each([1, 2, 3, 4, 5])._where(_ > 2) |> reverse() |> first())

[test(description = "reverse then first folds into a single pass that tracks the last survivor")]
def test_reverse_first_uses_last_survivor_tracker(t : T?) {
    // R5: reverse |> first = LAST survivor in source order. Walk forward,
    // overwrite `last` on every match, return `last` (panic if no
    // matches). No buffer, no reverse.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_reverse_first_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper for splice")
        t |> equal(0, count_call(body_expr, "reverse_inplace"),
            "reverse_inplace must NOT be emitted — last-survivor walk replaces it")
        t |> equal(0, count_call(body_expr, "push_clone"),
            "no buffer pushes — first elides the buffer entirely")
        t |> success(count_call(body_expr, "panic") >= 1,
            "first must emit panic for the empty-sequence case")
    }
}

[export, marker(no_coverage)]
def target_reverse_first_or_default_fold() : int =>
    _fold(each([1, 2, 3, 4, 5])._where(_ > 100) |> reverse() |> first_or_default(-1))

[test(description = "reverse then first_or_default folds into a single pass that falls back to the default")]
def test_reverse_first_or_default_returns_default_when_empty(t : T?) {
    // R5: reverse |> first_or_default returns the default expression when
    // no element survives the filter. No panic.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_reverse_first_or_default_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper for splice")
        t |> equal(0, count_call(body_expr, "reverse_inplace"),
            "reverse_inplace must NOT be emitted")
        t |> equal(0, count_call(body_expr, "panic"),
            "first_or_default must NOT panic")
    }
}

[export, marker(no_coverage)]
def target_where_distinct_to_array_fold() : array<int> =>
    <- _fold([1, 2, 3, 2, 4, 1, 5].to_sequence()._where(_ > 1).distinct().to_array())

[test(description = "where then distinct then to_array folds into one loop with a seen set and a buffer and no runtime helpers")]
def test_distinct_with_where_fuses_into_loop(t : T?) {
    // D2: where_ before distinct fuses into the prefilter loop. Single
    // for, single seen-set + buffer.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_where_distinct_to_array_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper for splice")
        t |> equal(1, count_inner_for_loops(body_expr), "single fused for-loop")
        t |> equal(0, count_call(body_expr, "where_"),
                   "where_ runtime helper must NOT be called (predicate inlined)")
        t |> equal(0, count_call(body_expr, "distinct"),
                   "distinct runtime helper must NOT be called")
        t |> equal(2, count_outer_let_vars(body_expr), "exactly seen-set + buffer")
    }
}

[export, marker(no_coverage)]
def target_select_distinct_to_array_fold() : array<int> =>
    <- _fold([1, 2, 3, 4].to_sequence()._select(_ % 3).distinct().to_array())

[test(description = "select then distinct then to_array folds into one loop keyed on the inlined projection")]
def test_distinct_select_uses_projection_as_key(t : T?) {
    // D3: select before distinct — projection inlined into both
    // unique_key and push_clone.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_select_distinct_to_array_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper for splice")
        t |> equal(1, count_inner_for_loops(body_expr), "single fused for-loop")
        t |> equal(0, count_call(body_expr, "select"), "select runtime helper must NOT be called")
        t |> equal(0, count_call(body_expr, "distinct"),
                   "distinct runtime helper must NOT be called")
        t |> success(count_op2(body_expr, "%") >= 1, "projection modulo must appear inline in body")
    }
}

[export, marker(no_coverage)]
def target_distinct_by_to_array_fold() : array<int> =>
    <- _fold([10, 21, 32, 11, 23, 42].to_sequence()._distinct_by(_ % 10).to_array())

[test(description = "distinct_by then to_array folds into one loop with the key function inlined")]
def test_distinct_by_with_key_substitutes_keyfn(t : T?) {
    // D4: distinct_by — key block invoked per element; key drives dedup
    // but the raw (or projected) value is what gets pushed to the buffer.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_distinct_by_to_array_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper for splice")
        t |> equal(1, count_inner_for_loops(body_expr), "single fused for-loop")
        t |> equal(0, count_call(body_expr, "distinct_by"),
                   "distinct_by runtime helper must NOT be called")
        t |> success(count_op2(body_expr, "%") >= 1,
                     "key modulo operation must appear inline (via let init walk)")
        t |> equal(2, count_outer_let_vars(body_expr), "exactly seen-set + buffer")
    }
}

[export, marker(no_coverage)]
def target_distinct_take_to_array_fold() : array<int> =>
    <- _fold([1, 1, 2, 2, 3, 3, 4, 4, 5, 5].to_sequence().distinct().take(3).to_array())

[test(description = "distinct then take then to_array folds into one loop with a break at the take limit")]
def test_distinct_take_emits_early_exit(t : T?) {
    // D5: take(N) on distinct early-exits the prefilter loop after N
    // distinct hits. Anti-cascade gate: tier-2 cascade would emit
    // multiple outer vars + delete stmts.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_distinct_take_to_array_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper for splice")
        t |> equal(1, count_inner_for_loops(body_expr), "single fused for-loop")
        t |> success(count_break_continue(body_expr, true) >= 1, "take guard must emit a break")
        t |> equal(0, count_call(body_expr, "take"), "take runtime helper must NOT be called")
    }
}

[export, marker(no_coverage)]
def target_distinct_count_fold() : int => _fold([1, 2, 2, 3, 3, 3].to_sequence().distinct().count())

[test(description = "distinct then count folds without the runtime distinct helper")]
def test_distinct_count_elides_buffer(t : T?) {
    // D6: distinct → count returns length(seen). No buffer, no push_clone
    // — the seen-set IS the answer source.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_distinct_count_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper for splice")
        t |> equal(0, count_call(body_expr, "push_clone"),
            "no buffer pushes — count elides the buffer entirely")
        t |> equal(0, count_call(body_expr, "distinct"),
                   "distinct runtime helper must NOT be called")
    }
}

[export, marker(no_coverage)]
def target_distinct_sum_fold() : int => _fold([1, 2, 2, 3, 3, 3, 4].to_sequence().distinct().sum())

[test(description = "distinct then sum folds into an inline accumulator")]
def test_distinct_sum_emits_inline_accumulator(t : T?) {
    // D7: distinct → sum maintains seen-set + accumulator; each fresh
    // element contributes to acc via `+=`. No buffer.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_distinct_sum_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper for splice")
        t |> success(count_op2(body_expr, "+=") >= 1, "sum accumulator must emit +=")
        t |> equal(0, count_call(body_expr, "push_clone"),
            "no buffer pushes — sum folds inline")
    }
}

[export, marker(no_coverage)]
def target_group_by_count_bare_reducer_fold() : array<int> {
    // Bare reducer body — group_proj is just `_._1 |> length`, no
    // named-tuple wrap. Result is array<int> of per-bucket counts. Bucket
    // count is 3 (mod 3 = 0,1,2). _group_by on array → array<tuple<K;
    // array<V>>>; _select then yields array<int>; no trailing .to_array()
    // since the chain already produces an array.
    return <- _fold([1, 2, 3, 4, 5, 6, 7]._group_by(_ % 3)._select(_._1 |> length))
}

[test(description = "group_by with a count reducer folds into a table update and a result build with one hash per element and no bucket arrays")]
def test_group_by_count_emits_table_no_bucket_arrays(t : T?) {
    // G2: group_by(key) + select(_._1|>length) + to_array splices into a
    // table-driven emission with NO per-bucket array materialization.
    // Table value is `tuple<KeyT; int>` (Acc is int for count) rather
    // than `tuple<KeyT; array<elem>>`.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_group_by_count_bare_reducer_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper for splice")
        // No call to the runtime group_by_lazy / group_by_lazy_impl helpers;
        // the splice replaces them with a per-element table update.
        t |> equal(0, count_call(body_expr, "group_by_lazy"), "must NOT call group_by_lazy runtime")
        t |> equal(0, count_call(body_expr, "group_by_lazy_impl"), "must NOT call impl")
        // unique_key must be invoked per element to derive the seen-table
        // key.
        t |> success(count_call(body_expr, "unique_key") >= 1,
                     "must call _::unique_key per element")
        // Single-hash hot path: key_exists must NOT appear (the `tab?[uk] ??
        // dummy` + addr-compare pattern avoids the extra membership check).
        // See PR #2721.
        t |> equal(0, count_call(body_expr, "key_exists"), "single-hash hot path: no key_exists")
        // For-loop count: table-update loop + result-build loop = 2.
        t |> equal(2, count_inner_for_loops(body_expr), "table-update + result-build loops")
    }
}

[export, marker(no_coverage)]
def target_group_by_count_named_tuple_fold() : array<tuple<K : int; N : int>> {
    // Named-tuple wrap: select(@(g) => (K=g._0, N=g._1|>length))
    // preserves both the key AND the aggregate. Splice uses the user's
    // group_proj body type as the table value type, so the K/N field
    // names survive end-to-end (see test below).
    return <- _fold([1, 2, 3, 4, 5, 6, 7]._group_by(_ % 3)._select((K = _._0, N = _._1 |> length)))
}

[test(description = "group_by with a named-tuple count reducer folds the same way and keeps the field names")]
def test_group_by_count_named_tuple_preserves_keys(t : T?) {
    // G3: named-tuple wrap recognized; output type preserves user shape.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_group_by_count_named_tuple_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper for splice")
        t |> equal(0, count_call(body_expr, "group_by_lazy"), "must NOT call group_by_lazy runtime")
        // For-loops: table-update + result-build = 2 (incremental-aggregate's
        // tell).
        t |> equal(2, count_inner_for_loops(body_expr), "table-update + result-build loops")
        // unique_key per element — splice's per-element hash. (Note: not
        // asserting absence of `length` here — `length(tab)` legitimately
        // appears in the reserve hint and would false-positive.)
        t |> success(count_call(body_expr, "unique_key") >= 1,
                     "must call _::unique_key per element")
        // Single-hash hot path: no key_exists. See PR #2721.
        t |> equal(0, count_call(body_expr, "key_exists"), "single-hash hot path: no key_exists")
    }
}

[export, marker(no_coverage)]
def target_group_by_count_terminator_fold() : int {
    // G8: count terminator — total number of buckets. Should return
    // length(tab) directly.
    return _fold([1, 2, 3, 4, 5, 6, 7]._group_by(_ % 3)._select((K = _._0, N = _._1 |> length)).count())
}

[test(description = "group_by then count folds into a single table-update loop and answers with the table length")]
def test_group_by_count_terminator_returns_table_length(t : T?) {
    // G8: count terminator on group_by + select — return length(tab)
    // without materializing the result buffer.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_group_by_count_terminator_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper for splice")
        t |> equal(0, count_call(body_expr, "group_by_lazy"), "must NOT call group_by_lazy")
        // Only ONE for-loop (the table-update). Result-build loop is elided
        // when terminator is count (we just return length(tab)).
        t |> equal(1, count_inner_for_loops(body_expr), "single table-update loop, no result loop")
        // Single-hash hot path: no key_exists. See PR #2721.
        t |> equal(0, count_call(body_expr, "key_exists"), "single-hash hot path: no key_exists")
    }
}

[export, marker(no_coverage)]
def target_group_by_inner_select_sum_bare_fold() : array<int> {
    // G4 (PR-A1): bare inner-select-sum body. is_bucket_reducer_call's
    // new arm matches `sum(select(<bind>._1, <lambda>))`; the inner
    // lambda body is peeled by fold_linq_cond and inlined into the
    // per-element `entry._1 += <body>` site.
    return <- _fold(
                    [10, 20, 30, 11, 21, 31, 12]._group_by(_ % 3)._select(_._1 |> select(@(x : int) => x * 2) |> sum)) } [test(description = "group_by with a per-bucket sum of a projection folds with the inner select inlined")]
def test_group_by_inner_select_sum_emits_table_no_inner_select_call(t : T?) {
    // G4: bare inner-select-sum splices through the new arm. Body must
    // NOT contain a runtime call to `select` (the inner select is peeled
    // + inlined) and must preserve the single-hash hot-path fingerprint
    // from PR #2721.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_group_by_inner_select_sum_bare_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper for splice")
        t |> equal(0, count_call(body_expr, "group_by_lazy"), "must NOT call group_by_lazy runtime")
        t |> equal(0, count_call(body_expr, "select"), "inner select must be inlined, not called")
        t |> success(count_call(body_expr, "unique_key") >= 1,
                     "must call _::unique_key per element")
        t |> equal(2, count_inner_for_loops(body_expr), "table-update + result-build loops")
        t |> equal(0, count_call(body_expr, "key_exists"), "single-hash hot path: no key_exists")
    }
}

[export, marker(no_coverage)]
def target_group_by_inner_select_sum_named_fold() : array<tuple<K : int; T : int>> {
    // G4b: named-tuple-wrap form `(K=_._0,
    // T=_._1|>select(<inner>)|>sum)`. Splice fires via Case B
    // (named-tuple recognizer); table value type = user's named tuple,
    // accumulator slot = inner projection result type.
    return <- _fold(
                    [10, 20, 30, 11, 21, 31, 12]._group_by(_ % 3)._select((K = _._0, T = _._1 |> select(@(x : int) => x * 2) |> sum))) } [test(description = "the named per-bucket sum of a projection keeps the key field and inlines the select")]
def test_group_by_inner_select_sum_named_preserves_key(t : T?) {
    // G4b: named-tuple wrap with inner-select-sum splices and keeps the
    // user's field names.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_group_by_inner_select_sum_named_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper for splice")
        t |> equal(0, count_call(body_expr, "group_by_lazy"), "must NOT call group_by_lazy runtime")
        t |> equal(0, count_call(body_expr, "select"), "inner select must be inlined, not called")
        t |> success(count_call(body_expr, "unique_key") >= 1,
                     "must call _::unique_key per element")
        t |> equal(2, count_inner_for_loops(body_expr), "table-update + result-build loops")
        t |> equal(0, count_call(body_expr, "key_exists"), "single-hash hot path: no key_exists")
    }
}

[export, marker(no_coverage)]
def target_group_by_min_workhorse_fold() : array<int> {
    // G5a (PR-A2): bare min on workhorse int → direct `<` compare, no
    // `_::less`.
    return <- _fold([10, 20, 30, 11, 21, 31, 12]._group_by(_ % 3)._select(_._1 |> min))
}

[test(description = "a per-bucket min on a workhorse type compares directly, not through a less call")]
def test_group_by_min_workhorse_emits_direct_compare(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_group_by_min_workhorse_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper for splice")
        t |> equal(0, count_call(body_expr, "group_by_lazy"), "must NOT call group_by_lazy runtime")
        t |> equal(0, count_call(body_expr, "less"),
                   "workhorse min must use direct `<`, not _::less")
        t |> success(count_call(body_expr, "unique_key") >= 1,
                     "must call _::unique_key per element")
        t |> equal(2, count_inner_for_loops(body_expr), "table-update + result-build loops")
        t |> equal(0, count_call(body_expr, "key_exists"), "single-hash hot path: no key_exists")
    }
}

[export, marker(no_coverage)]
def target_group_by_min_non_workhorse_fold() : array<tuple<int; int>> {
    // G5b (PR-A2): bare min on `tuple<int;int>` (non-workhorse) →
    // `_::less` dispatch.
    return <- _fold(
                    [(1, 2), (0, 5), (2, 3), (0, 1), (1, 4)]._group_by(_._0 % 2)._select(_._1 |> min)) } [test(description = "a per-bucket min on a non-workhorse type compares through the less call")]
def test_group_by_min_non_workhorse_uses_less(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_group_by_min_non_workhorse_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper for splice")
        t |> equal(0, count_call(body_expr, "group_by_lazy"), "must NOT call group_by_lazy runtime")
        t |> success(count_call(body_expr, "less") >= 1,
                     "non-workhorse min: at least one _::less call")
        t |> equal(0, count_call(body_expr, "key_exists"), "single-hash hot path: no key_exists")
    }
}

[export, marker(no_coverage)]
def target_group_by_first_fold() : array<int> {
    // G6 (PR-A2): bare first → miss-branch captures, hit-branch is empty
    // (no compare).
    return <- _fold([10, 20, 30, 11, 21, 31, 12]._group_by(_ % 3)._select(_._1 |> first))
}

[test(description = "a per-bucket first has an empty hit branch and no runtime first call")]
def test_group_by_first_no_hit_compare(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_group_by_first_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper for splice")
        t |> equal(0, count_call(body_expr, "group_by_lazy"), "must NOT call group_by_lazy runtime")
        t |> equal(0, count_call(body_expr, "less"), "first hit branch is empty: no _::less call")
        t |> equal(0, count_call(body_expr, "first"), "must NOT call runtime first()")
        t |> success(count_call(body_expr, "unique_key") >= 1,
                     "must call _::unique_key per element")
        t |> equal(0, count_call(body_expr, "key_exists"), "single-hash hot path: no key_exists")
        t |> equal(2, count_inner_for_loops(body_expr), "table-update + result-build loops")
    }
}

[export, marker(no_coverage)]
def target_group_by_multi_reducer_fold() : array<tuple<K : int; N : int; S : int; MX : int>> {
    // G7 (PR-A2): 4-slot named tuple → multi-reducer specs array fuses
    // count+sum+max into one pass.
    return <- _fold([10, 20, 30, 11, 21, 31, 12]._group_by(_ % 3)._select(
        (K = _._0, N = _._1 |> length, S = _._1 |> sum, MX = _._1 |> max)))
}

[test(description = "several reducers per bucket fold into one table-update loop with no runtime sum or max call")]
def test_group_by_multi_reducer_emits_fused_pass(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_group_by_multi_reducer_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper for splice")
        t |> equal(0, count_call(body_expr, "group_by_lazy"), "must NOT call group_by_lazy runtime")
        t |> equal(0, count_call(body_expr, "sum"), "must NOT call runtime sum()")
        t |> equal(0, count_call(body_expr, "max"), "must NOT call runtime max()")
        // Note: 1 `length(tab)` survives for reserve() — that's not the
        // reducer.
        t |> success(count_call(body_expr, "unique_key") >= 1,
                     "must call _::unique_key per element")
        t |> equal(2, count_inner_for_loops(body_expr),
                   "single fused table-update loop + result-build")
        t |> equal(0, count_call(body_expr, "key_exists"), "single-hash hot path: no key_exists")
    }
}

[export, marker(no_coverage)]
def target_group_by_first_or_default_falls_through() : array<int> {
    // G10 (PR-A2): `first_or_default(_._1, def)` has 2 args → recognizer
    // rejects (only 1-arg bucket reducers are spliced). Cascades to tier
    // 2.
    return <- _fold([10, 20, 30, 11, 21, 31, 12]._group_by(_ % 3)._select(_._1 |> first_or_default(-1)))
}

[test(description = "group_by with first_or_default falls back to the unfused path")]
def test_group_by_first_or_default_cascades_to_tier2(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_group_by_first_or_default_falls_through)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper from cascade")
        // Cascade emits pass_0/pass_1/... — splice would have exactly 1 outer
        // let (the table).
        t |> success(count_outer_let_vars(body_expr) >= 2,
            "cascade fingerprint: multiple pass_N outer vars")
    }
}

[export, marker(no_coverage)]
def target_group_by_upstream_where_fold() : array<int> {
    // G11 (PR-B): upstream where_ fuses into the per-element loop. No
    // runtime where_ call; body contains `if (predicate) { ... }`
    // guarding the key + table-update splice.
    return <- _fold([1, 2, 3, 4, 5, 6, 7, 8, 9, 10]._where(_ % 2 == 0)._group_by(_ % 3)._select(_._1 |> length))
}

[test(description = "a where before group_by is inlined into the table-update loop")]
def test_group_by_upstream_where_no_runtime_where_call(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_group_by_upstream_where_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper for splice")
        t |> equal(0, count_call(body_expr, "group_by_lazy"), "must NOT call group_by_lazy runtime")
        t |> equal(0, count_call(body_expr, "where_"),
                   "upstream where_ must be inlined, not called")
        t |> success(count_call(body_expr, "unique_key") >= 1,
                     "must call _::unique_key per element")
        t |> equal(0, count_call(body_expr, "key_exists"), "single-hash hot path: no key_exists")
        t |> equal(2, count_inner_for_loops(body_expr), "table-update + result-build loops")
    }
}

[export, marker(no_coverage)]
def target_group_by_upstream_select_fold() : array<tuple<K : int; S : int>> {
    // G12 (PR-B): upstream select_ fuses; the key references the
    // projected value via an intermediate var bind inside the per-element
    // loop.
    return <- _fold([10, 20, 30, 11, 21, 31, 12]._select(_ * 2)._group_by(_ % 3)._select((K = _._0, S = _._1 |> sum)))
}

[test(description = "a select before group_by is inlined into the table-update loop")]
def test_group_by_upstream_select_no_runtime_select_call(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_group_by_upstream_select_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper for splice")
        t |> equal(0, count_call(body_expr, "group_by_lazy"), "must NOT call group_by_lazy runtime")
        t |> equal(0, count_call(body_expr, "select"),
                   "upstream select must be inlined, not called")
        t |> success(count_call(body_expr, "unique_key") >= 1,
                     "must call _::unique_key per element")
        t |> equal(0, count_call(body_expr, "key_exists"), "single-hash hot path: no key_exists")
        t |> equal(2, count_inner_for_loops(body_expr), "table-update + result-build loops")
    }
}

[export, marker(no_coverage)]
def target_group_by_upstream_where_select_fold() : array<tuple<K : int; S : int>> {
    // G13 (PR-B): chained upstream where_ + select_ fuse together. Where
    // guards, select binds.
    return <- _fold([1, 2, 3, 4, 5, 6, 7, 8, 9, 10]._where(_ > 3)._select(_ * 10)._group_by(_ % 3)._select((K = _._0, S = _._1 |> sum)))
}

[test(description = "a where and a select before group_by are both inlined into the table-update loop")]
def test_group_by_upstream_where_select_combined(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_group_by_upstream_where_select_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper for splice")
        t |> equal(0, count_call(body_expr, "group_by_lazy"), "must NOT call group_by_lazy runtime")
        t |> equal(0, count_call(body_expr, "where_"), "upstream where_ must be inlined")
        t |> equal(0, count_call(body_expr, "select"), "upstream select must be inlined")
        t |> equal(0, count_call(body_expr, "key_exists"), "single-hash hot path: no key_exists")
        t |> equal(2, count_inner_for_loops(body_expr), "table-update + result-build loops")
    }
}

[export, marker(no_coverage)]
def target_group_by_upstream_distinct_falls_through() : array<int> {
    // BAIL (PR-B): distinct upstream is NOT where_/select* →
    // plan_group_by rejects; cascades to tier 2.
    return <- _fold([1, 2, 3, 4, 5, 6, 7].to_sequence().distinct()._group_by(_ % 3)._select(_._1 |> length).to_array())
}

[test(description = "a distinct before group_by falls back to the unfused path")]
def test_group_by_upstream_distinct_cascades_to_tier2(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_group_by_upstream_distinct_falls_through)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper from cascade")
        t |> success(count_outer_let_vars(body_expr) >= 2,
            "cascade fingerprint: multiple pass_N outer vars")
    }
}

[export, marker(no_coverage)]
def target_group_by_having_falls_through() : array<int> {
    // BAIL: where_ between group_by and select (HAVING clause shape) →
    // cascade to tier 2.
    return <- _fold([1, 2, 3, 4, 5, 6, 7]._group_by(_ % 3)._where(_._1 |> length >= 2)._select(_._1 |> length))
}

[test(description = "a general having after group_by falls back to the unfused path")]
def test_group_by_having_cascades_to_tier2(t : T?) {
    // Bail-trigger: where_ between group_by and select — plan_group_by
    // must reject; chain cascades to tier 2 (which emits the array-shape
    // pipeline with pass_N vars + delete intermediates).
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_group_by_having_falls_through)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper from cascade")
        // Tier 2 cascade emits pass_0/pass_1/... let-vars. Splice would have
        // exactly 1 (the table). So having_ → tier 2 produces 3+ outer
        // let-vars (where, group, select).
        t |> success(count_outer_let_vars(body_expr) >= 2,
            "cascade fingerprint: multiple pass_N outer vars")
    }
}

[export, marker(no_coverage)]
def target_group_by_having_length_match_fold() : array<tuple<K : int; N : int>> {
    // PR-D splice: `_having(_._1|>length>=N)` between group_by_lazy and
    // select where the select has a matching length slot — predicate
    // rewrites to `kv._{slot}>=N` and wraps push_clone.
    return <- _fold([1, 2, 3, 4, 5, 6, 7, 8, 9, 10]._group_by_lazy(_ % 3)._having(_._1 |> length >= 4)._select((K = _._0, N = _._1 |> length)))
}

[test(description = "a having that compares the bucket length folds into the table-update loop")]
def test_group_by_having_length_match_splices(t : T?) {
    // Single-hash hot path (no key_exists) + no runtime having_/select
    // call in the splice body.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_group_by_having_length_match_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched, "expected splice wrapper")
        t |> equal(0, count_call(body_expr, "key_exists"))
        t |> success(count_call(body_expr, "unique_key") >= 1, "table-update uses unique_key")
        t |> equal(0, count_call(body_expr, "having_"))
        t |> equal(0, count_call(body_expr, "group_by_lazy"))
    }
}

[export, marker(no_coverage)]
def target_group_by_having_count_terminator_fold() : int {
    // PR-D count terminator: result-build counts only the buckets passing
    // the rewritten predicate.
    return _fold([1, 2, 3, 4, 5, 6, 7, 8, 9, 10]._group_by_lazy(_ % 3)._having(_._1 |> length >= 4)._select((K = _._0, N = _._1 |> length)) |> count())
}

[test(description = "a having followed by count folds into the group_by splice")]
def test_group_by_having_count_terminator_splices(t : T?) {
    // count + having: no length(tab) shortcut (we have to inspect each
    // entry); the splice emits a counter loop instead — `having_` and
    // `group_by_lazy` should still be absent.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_group_by_having_count_terminator_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched, "expected splice wrapper")
        t |> equal(0, count_call(body_expr, "having_"))
        t |> equal(0, count_call(body_expr, "group_by_lazy"))
        t |> equal(0, count_call(body_expr, "key_exists"))
    }
}

[export, marker(no_coverage)]
def target_group_by_having_hidden_sum_fold() : array<tuple<K : int; N : int>> {
    // PR-E: having uses sum but select has only length — planner
    // synthesizes a hidden sum slot in the internal table value type;
    // result-build projects only the user-visible (K, N).
    return <- _fold([1, 2, 3, 4, 5, 6]._group_by_lazy(_ % 2)._having(_._1 |> sum > 10)._select((K = _._0, N = _._1 |> length)))
}

[test(description = "a having on a hidden sum folds with the sum inlined into the hit branch and one hash per element")]
def test_group_by_having_hidden_sum_splices(t : T?) {
    // PR-E splice fingerprint: no group_by_lazy or having_ runtime calls;
    // single-hash hot path.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_group_by_having_hidden_sum_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched, "expected splice wrapper")
        t |> equal(0, count_call(body_expr, "group_by_lazy"))
        t |> equal(0, count_call(body_expr, "having_"))
        t |> equal(0, count_call(body_expr, "sum"), "hidden sum inlined into += hit branch")
        t |> equal(0, count_call(body_expr, "key_exists"), "single-hash hot path")
        t |> success(count_call(body_expr, "unique_key") >= 1, "per-element unique_key")
    }
}

[export, marker(no_coverage)]
def target_group_by_having_two_same_named_inner_select_cascades() : array<tuple<K : int; N : int>> {
    // PR-E bail: two same-named inner-select reducers in the same having
    // clause can't be distinguished by name alone — splicing would
    // silently conflate the two lambdas onto one hidden slot. Bail to
    // tier 2 so each select() is evaluated separately.
    return <- _fold([1, 2, 3, 4, 5, 6, 7, 8]._group_by_lazy(_ % 3)
                       ._having((_._1 |> select($(x : int) => x * 2) |> sum)
                           + (_._1 |> select($(x : int) => x * 3) |> sum) > 50)
                       ._select((K = _._0, N = _._1 |> length)))
}

[test(description = "a having with two same-named inner selects falls back to the unfused path")]
def test_group_by_having_two_same_named_inner_select_cascades_to_tier2(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_group_by_having_two_same_named_inner_select_cascades)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper from cascade")
        t |> success(count_outer_let_vars(body_expr) >= 2,
            "cascade fingerprint: multiple pass_N outer vars")
    }
}

[export, marker(no_coverage)]
def target_group_by_having_hidden_bare_form_cascades() : array<int> {
    // PR-E bail: bare-form group_proj (`_._1 |> length`, not a named
    // tuple) combined with a having reducer that needs a hidden slot —
    // the bare table layout uses `tuple<KeyT; AccT>` synthesized inside a
    // qmacro with embedded `typedecl(invoke(...))`, which can't be
    // extended with a dynamic count of additional acc slots. Falls back
    // to tier 2.
    return <- _fold([1, 2, 3, 4, 5, 6]._group_by_lazy(_ % 2)._having(_._1 |> sum > 10)._select(_._1 |> length))
}

[test(description = "a bare-form select with a hidden reducer in the having falls back to the unfused path")]
def test_group_by_having_hidden_bare_form_cascades_to_tier2(t : T?) {
    // Cascade fingerprint: multiple pass_N let-vars (group, having,
    // select all separate).
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_group_by_having_hidden_bare_form_cascades)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper from cascade")
        t |> success(count_outer_let_vars(body_expr) >= 2,
            "cascade fingerprint: multiple pass_N outer vars")
    }
}

[export, marker(no_coverage)]
def target_group_by_having_raw_bucket_cascades() : array<tuple<K : int; N : int>> {
    // BAIL: predicate touches _._1 outside any matched reducer (raw
    // bucket-array length probe). is_bind_field_access on _._1 alone
    // (used as condition value) is not in the recognized set.
    return <- _fold([1, 2, 3, 4, 5, 6]._group_by_lazy(_ % 2)._having(empty(_._1))._select((K = _._0, N = _._1 |> length)))
}

[test(description = "a having that reads the raw bucket falls back to the unfused path")]
def test_group_by_having_raw_bucket_cascades_to_tier2(t : T?) {
    // Cascade fingerprint: multiple pass_N let-vars.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_group_by_having_raw_bucket_cascades)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper from cascade")
        t |> success(count_outer_let_vars(body_expr) >= 2,
            "cascade fingerprint: multiple pass_N outer vars")
    }
}

[export, marker(no_coverage)]
def target_group_by_average_bare_fold() : array<double> {
    // PR-A2 follow-up: bare `_._1 |> average` splices via
    // tuple<double;uint64> per-key acc.
    return <- _fold([10, 20, 30, 11, 21, 31, 12]._group_by(_ % 3)._select(_._1 |> average))
}

[test(description = "a bare per-bucket average folds with an inlined accumulator and no runtime average call")]
def test_group_by_average_bare_emits_avg_acc(t : T?) {
    // Average splice fingerprint: no runtime average call (inlined into
    // += / count loop), single-hash hot path (no key_exists), per-element
    // unique_key.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_group_by_average_bare_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper for splice")
        t |> equal(0, count_call(body_expr, "group_by_lazy"), "must NOT call group_by_lazy")
        t |> equal(0, count_call(body_expr, "average"), "reducer inlined; no runtime average call")
        t |> equal(0, count_call(body_expr, "key_exists"), "single-hash hot path")
        t |> success(count_call(body_expr, "unique_key") >= 1, "per-element unique_key")
        t |> equal(2, count_inner_for_loops(body_expr), "table-update + result-build loops")
    }
}

[export, marker(no_coverage)]
def target_group_by_average_named_fold() : array<tuple<K : int; Avg : double>> =>
    <- _fold([10, 20, 30, 11, 21, 31, 12]._group_by(_ % 3)._select((K = _._0, Avg = _._1 |> average)))

[test(description = "a named per-bucket average folds the same way and keeps the field name")]
def test_group_by_average_named_preserves_field_name(t : T?) {
    // Named form: table stores tuple<K:int;Avg:tuple<double;uint64>>
    // internally; result-build synthesizes user-facing
    // tuple<K;Avg:double> with field names preserved (compiler-checked by
    // the function's declared return type — would fail compilation if
    // shape diverged).
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_group_by_average_named_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper for splice")
        t |> equal(0, count_call(body_expr, "group_by_lazy"), "must NOT call group_by_lazy")
        t |> equal(0, count_call(body_expr, "average"), "reducer inlined; no runtime average call")
        t |> equal(0, count_call(body_expr, "key_exists"), "single-hash hot path")
        t |> equal(2, count_inner_for_loops(body_expr), "table-update + result-build loops")
    }
}

[export, marker(no_coverage)]
def target_group_by_average_inner_select_fold() : array<tuple<K : int; Avg : double>> {
    return <- _fold([10, 20, 30, 11, 21, 31, 12]._group_by(_ % 3)._select(
        (K = _._0, Avg = _._1 |> select(@(x : int) => x * 2) |> average)))
}

[test(description = "a per-bucket average of a projection folds with the reducer and the inner select inlined")]
def test_group_by_average_inner_select_inlines_projection(t : T?) {
    // Inner select inlined — neither inner `select` call nor the reducer
    // survives.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_group_by_average_inner_select_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper for splice")
        t |> equal(0, count_call(body_expr, "group_by_lazy"), "must NOT call group_by_lazy")
        t |> equal(0, count_call(body_expr, "average"), "reducer inlined")
        t |> equal(0, count_call(body_expr, "select"), "inner select inlined")
        t |> equal(0, count_call(body_expr, "key_exists"), "single-hash hot path")
    }
}

[export, marker(no_coverage)]
def target_group_by_multi_reducer_with_avg_fold() : array<tuple<K : int; N : int; S : int; Avg : double>> {
    return <- _fold([10, 20, 30, 11, 21, 31, 12]._group_by(_ % 3)._select(
        (K = _._0, N = _._1 |> length, S = _._1 |> sum, Avg = _._1 |> average)))
}

[test(description = "several reducers including an average fold into one table-update loop")]
def test_group_by_multi_reducer_with_avg_splices(t : T?) {
    // 4-slot named tuple combining length + sum + average — single fused
    // pass, no runtime reducer calls.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_group_by_multi_reducer_with_avg_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper for splice")
        t |> equal(0, count_call(body_expr, "group_by_lazy"), "must NOT call group_by_lazy")
        t |> equal(0, count_call(body_expr, "average"), "reducer inlined")
        t |> equal(0, count_call(body_expr, "sum"), "reducer inlined")
        // length(tab) legitimately appears in the reserve hint — don't assert
        // absence.
        t |> equal(0, count_call(body_expr, "key_exists"), "single-hash hot path")
        t |> equal(2, count_inner_for_loops(body_expr), "table-update + result-build loops")
    }
}

[test(description = "select then reverse then to_array folds into one for loop that pushes the projection")]
def test_reverse_select_pushes_projection_not_source(t : T?) {
    // R3: select before reverse — the loop pushes the projected value,
    // not the iteration variable. ExprMul / ExprOp2 "*" presence in body
    // indicates the projection got inlined into push_clone.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_select_reverse_to_array_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper for splice")
        t |> equal(1, count_inner_for_loops(body_expr), "single fused for-loop")
        t |> success(count_call(body_expr, "reverse_inplace") >= 1,
            "must emit reverse_inplace call")
        t |> equal(0, count_call(body_expr, "select"),
            "select runtime helper must NOT be called (projection inlined)")
        t |> success(count_op2(body_expr, "*") >= 1,
            "projection multiplication must appear inline in push_clone")
        t |> equal(1, count_outer_let_vars(body_expr),
            "single outer var (the buffer) — anti-cascade gate")
    }
}

[export, marker(no_coverage)]
def target_last_fold() : int => _fold(each([1, 2, 3, 4, 5])._where(_ < 4).last())

[export, marker(no_coverage)]
def target_last_or_default_fold() : int =>
    _fold(each([1, 2, 3, 4, 5])._where(_ > 99).last_or_default(-1))

[export, marker(no_coverage)]
def target_single_fold() : int => _fold(each([1, 2, 3, 4, 5])._where(_ == 3).single())

[export, marker(no_coverage)]
def target_single_or_default_fold() : int =>
    _fold(each([1, 2, 3, 4, 5])._where(_ > 99).single_or_default(-1))

[export, marker(no_coverage)]
def target_element_at_fold() : int => _fold(each([10, 20, 30, 40, 50]).element_at(3))

[export, marker(no_coverage)]
def target_element_at_or_default_fold() : int =>
    _fold(each([10, 20, 30]).element_at_or_default(100))

[export, marker(no_coverage)]
def target_aggregate_fold() : int =>
    _fold(each([1, 2, 3, 4, 5]).aggregate(0, $(acc : int, x : int) => acc + x * 2))

[export, marker(no_coverage)]
def target_aggregate_with_where_fold() : int =>
    _fold(each([1, 2, 3, 4, 5])._where(_ % 2 == 1).aggregate(0, $(acc : int, x : int) =>
        acc + x * x))

[test(description = "a folded last emits one for loop with a panic in the tail for the empty case")]
def test_last_splices(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_last_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper")
        t |> equal(1, count_inner_for_loops(body_expr), "last: one for-loop (full walk)")
        // `last` tail panics on empty source; verify the splice emitted it.
        t |> success(count_call(body_expr, "panic") >= 1, "last: panic call in tail for empty case")
    }
}

[test(description = "a folded last_or_default emits one for loop and no panic")]
def test_last_or_default_no_panic(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_last_or_default_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper")
        t |> equal(1, count_inner_for_loops(body_expr), "last_or_default: one for-loop")
        // `last_or_default` returns the bound default; never panics.
        t |> equal(0, count_call(body_expr, "panic"), "last_or_default: no panic call")
    }
}

[test(description = "a folded single emits one for loop with two panic sites")]
def test_single_splices(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_single_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper")
        t |> equal(1, count_inner_for_loops(body_expr), "single: one for-loop")
        // `single` panics twice in the splice: once for >1 match (in-loop),
        // once for empty (tail).
        t |> success(count_call(body_expr, "panic") >= 2, "single: two panic call sites")
    }
}

[test(description = "a folded single_or_default emits one for loop and no panic")]
def test_single_or_default_no_panic(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_single_or_default_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper")
        t |> equal(1, count_inner_for_loops(body_expr), "single_or_default: one for-loop")
        t |> equal(0, count_call(body_expr, "panic"), "single_or_default: no panic")
    }
}

[test(description = "a folded element_at emits one for loop with a counter and the tail panic")]
def test_element_at_splices(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_element_at_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper")
        t |> equal(1, count_inner_for_loops(body_expr), "element_at: one for-loop")
        // `element_at` emits two panic call sites in the splice (pre-loop for
        // `idx < 0`, tail for "index out of range"); const-fold may eliminate
        // the pre-loop branch when the literal index is non-negative, so >= 1
        // is the guarantee.
        t |> success(count_call(body_expr, "panic") >= 1,
                     "element_at: at least the tail panic survives")
        // Counter `++` increments the in-flight match counter.
        t |> success(count_op1(body_expr, "++") >= 1, "element_at: counter increment present")
    }
}

[test(description = "a folded element_at_or_default emits one for loop and no panic")]
def test_element_at_or_default_no_panic(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_element_at_or_default_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper")
        t |> equal(1, count_inner_for_loops(body_expr), "element_at_or_default: one for-loop")
        t |> equal(0, count_call(body_expr, "panic"), "element_at_or_default: no panic")
    }
}

// Count ExprInvoke nodes anywhere in the subtree. The outer `_fold`
// splice wrapper is an ExprInvoke; per-element block calls (when peel
// fails) also emit ExprInvoke. Peeled aggregate bodies inline the
// block expression and emit zero inner invokes.
def count_invoke_nodes(expr : Expression?) : int {   // nolint:STYLE037 — exact-type `is` ladder, one arm per AST node kind
    if (expr == null) return 0
    var n = 0
    if (expr is ExprInvoke) {
        n ++
    }
    if (expr is ExprBlock) {
        let b = expr as ExprBlock
        for (s in b.list) {
            n += count_invoke_nodes(s)
        }
        for (s in b.finalList) {
            n += count_invoke_nodes(s)
        }
    } elif (expr is ExprFor) {
        let f = expr as ExprFor
        for (s in f.sources) {
            n += count_invoke_nodes(s)
        }
        n += count_invoke_nodes(f.body)
    } elif (expr is ExprIfThenElse) {
        let i = expr as ExprIfThenElse
        n += count_invoke_nodes(i.cond)
        n += count_invoke_nodes(i.if_true)
        n += count_invoke_nodes(i.if_false)
    } elif (expr is ExprOp2) {
        let o = expr as ExprOp2
        n += count_invoke_nodes(o.left)
        n += count_invoke_nodes(o.right)
    } elif (expr is ExprCall) {
        let c = expr as ExprCall
        for (a in c.arguments) {
            n += count_invoke_nodes(a)
        }
    } elif (expr is ExprMakeBlock) {
        let mb = expr as ExprMakeBlock
        n += count_invoke_nodes(mb._block)
    } elif (expr is ExprInvoke) {
        let inv = expr as ExprInvoke
        for (a in inv.arguments) {
            n += count_invoke_nodes(a)
        }
    } elif (expr is ExprReturn) {
        let r = expr as ExprReturn
        n += count_invoke_nodes(r.subexpr)
    } elif (expr is ExprOp1) {
        let o = expr as ExprOp1
        n += count_invoke_nodes(o.subexpr)
    } elif (expr is ExprLet) {
        let l = expr as ExprLet
        for (v in l.variables) {
            if (v != null && v.init != null) {
                n += count_invoke_nodes(v.init)
            }
        }
    }
    return n
}

[test(description = "a folded aggregate emits one for loop with the peeled body operators inline")]
def test_aggregate_splices_peeled(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_aggregate_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper")
        t |> equal(1, count_inner_for_loops(body_expr), "aggregate: one for-loop")
        // Single-return block body `acc + x * 2` peeled into `+` and `*` ops;
        // the only ExprInvoke is the outer `_fold` splice wrapper. A
        // non-peeled splice would emit a per-element ExprInvoke against the
        // user's block.
        t |> equal(1, count_invoke_nodes(body_expr),
            "aggregate: only the outer invoke wrapper, NOT a per-element block invoke")
        t |> success(count_op2(body_expr, "+") >= 1, "aggregate: `+` from peeled body")
        t |> success(count_op2(body_expr, "*") >= 1, "aggregate: `*` from peeled body")
    }
}

[test(description = "a folded aggregate with a where emits one for loop with the where inlined")]
def test_aggregate_with_where_fuses(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_aggregate_with_where_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper")
        t |> equal(1, count_inner_for_loops(body_expr), "aggregate+where: one for-loop")
        // Where predicate (`_ % 2 == 1`) inlined alongside peeled aggregate
        // body. The `where_` library helper must NOT appear as a runtime
        // call.
        t |> equal(0, count_call(body_expr, "where_"), "aggregate+where: where_ helper inlined")
        t |> equal(1, count_invoke_nodes(body_expr), "aggregate+where: only outer invoke wrapper")
    }
}

[export, marker(no_coverage)]
def target_take_while_array_fold() : array<int> =>
    <- _fold(each([5, 3, 8, 1, 4])._take_while(_ < 8).to_array())

[export, marker(no_coverage)]
def target_take_while_count_fold() : int => _fold(each([5, 3, 8, 1, 4])._take_while(_ < 8).count())

[export, marker(no_coverage)]
def target_take_while_sum_fold() : int => _fold(each([5, 3, 8, 1, 4])._take_while(_ < 8).sum())

[export, marker(no_coverage)]
def target_take_while_first_fold() : int => _fold(each([5, 3, 8, 1, 4])._take_while(_ < 8).first())

[export, marker(no_coverage)]
def target_skip_while_array_fold() : array<int> =>
    <- _fold(each([5, 3, 8, 1, 4])._skip_while(_ > 3).to_array())

[export, marker(no_coverage)]
def target_skip_while_count_fold() : int => _fold(each([5, 3, 8, 1, 4])._skip_while(_ > 3).count())

[export, marker(no_coverage)]
def target_skip_while_then_take_while_fold() : int =>
    _fold(each([5, 3, 8, 1, 4])._skip_while(_ > 3)._take_while(_ < 8).count())

[export, marker(no_coverage)]
def target_select_then_take_while_cascades_fold() : array<int> {
    // Cascade fingerprint: select precedes take_while → splice bails,
    // tier-2 cascade runs.
    return <- _fold(each([5, 3, 8, 1])._select(_ * 2)._take_while(_ < 16).to_array())
}

[test(description = "a folded take_while emits one for loop with the helper and its impls inlined")]
def test_take_while_splices(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_take_while_array_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "take_while: expected invoke wrapper")
        t |> equal(1, count_inner_for_loops(body_expr), "take_while: one for-loop")
        // take_while is spliced inline as `if (!pred) break` — the library
        // `take_while_impl` helper must NOT appear as a runtime call.
        t |> equal(0, count_call(body_expr, "take_while"), "take_while: helper inlined")
        t |> equal(0, count_call(body_expr, "take_while_impl"), "take_while: impl inlined")
        t |> equal(0, count_call(body_expr, "take_while_impl_const"),
                   "take_while: impl_const inlined")
    }
}

[test(description = "take_while then count folds into one for loop with the helper inlined")]
def test_take_while_count_splices(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_take_while_count_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "take_while count: invoke wrapper")
        t |> equal(1, count_inner_for_loops(body_expr), "take_while count: one for-loop")
        t |> equal(0, count_call(body_expr, "take_while"), "take_while count: helper inlined")
    }
}

[test(description = "take_while then sum folds into one accumulator loop with sum inlined")]
def test_take_while_sum_splices(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_take_while_sum_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "take_while sum: invoke wrapper")
        t |> equal(1, count_inner_for_loops(body_expr),
                   "take_while sum: one for-loop (accumulator lane)")
        t |> equal(0, count_call(body_expr, "take_while"), "take_while sum: helper inlined")
        t |> equal(0, count_call(body_expr, "sum"), "take_while sum: sum_impl inlined")
    }
}

[test(description = "take_while then first folds into one early-exit loop with first inlined")]
def test_take_while_first_splices(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_take_while_first_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "take_while first: invoke wrapper")
        t |> equal(1, count_inner_for_loops(body_expr),
                   "take_while first: one for-loop (early-exit lane)")
        t |> equal(0, count_call(body_expr, "take_while"), "take_while first: helper inlined")
        t |> equal(0, count_call(body_expr, "first"), "take_while first: first inlined")
    }
}

[test(description = "a folded skip_while emits one for loop with the helper and its impls inlined")]
def test_skip_while_splices(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_skip_while_array_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "skip_while: invoke wrapper")
        t |> equal(1, count_inner_for_loops(body_expr), "skip_while: one for-loop")
        t |> equal(0, count_call(body_expr, "skip_while"), "skip_while: helper inlined")
        t |> equal(0, count_call(body_expr, "skip_while_impl"), "skip_while: impl inlined")
        t |> equal(0, count_call(body_expr, "skip_while_impl_const"),
                   "skip_while: impl_const inlined")
    }
}

[test(description = "skip_while then count folds into one for loop with the helper inlined")]
def test_skip_while_count_splices(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_skip_while_count_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "skip_while count: invoke wrapper")
        t |> equal(1, count_inner_for_loops(body_expr), "skip_while count: one for-loop")
        t |> equal(0, count_call(body_expr, "skip_while"), "skip_while count: helper inlined")
    }
}

[test(description = "skip_while then take_while fold into one for loop with both inlined")]
def test_skip_while_then_take_while_compose_splices(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_skip_while_then_take_while_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "skip_while+take_while: invoke wrapper")
        t |> equal(1, count_inner_for_loops(body_expr), "skip_while+take_while: one for-loop")
        t |> equal(0, count_call(body_expr, "skip_while"),
                   "skip_while+take_while: skip_while inlined")
        t |> equal(0, count_call(body_expr, "take_while"),
                   "skip_while+take_while: take_while inlined")
    }
}

[test(description = "a select before take_while falls back to the unfused path")]
def test_select_then_take_while_cascades_to_tier2(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_select_then_take_while_cascades_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched, "select-then-take_while: target found")
        // Cascade tier-2 fingerprint: multiple outer let-vars from
        // fold_linq_default's intermediate-pass pipeline. Distinct from the
        // single-invoke splice shape.
        t |> success(count_outer_let_vars(body_expr) >= 2,
            "select-then-take_while: tier-2 cascade fingerprint (>=2 outer lets)")
    }
}

[export, marker(no_coverage)]
def target_order_by_first_splices_min_by() : int =>
    _fold(each([10, 20, 5, 8, 30, 15, 2, 25])._order_by(_).first())

[export, marker(no_coverage)]
def target_order_by_descending_first_splices_max_by() : int =>
    _fold(each([10, 20, 5, 8, 30, 15, 2, 25])._order_by_descending(_).first())

[export, marker(no_coverage)]
def target_where_order_by_first_splices_min_by_on_buf() : int =>
    _fold(each([10, 20, 5, 8, 30, 15, 2, 25])._where(_ > 5)._order_by(_).first())

[export, marker(no_coverage)]
def target_order_by_first_or_default_splices_top_n_by() : int =>
    _fold(each([10, 20, 5, 8, 30])._order_by(_).first_or_default(-1))

[export, marker(no_coverage)]
def target_order_with_cmp_first_bails_to_tier2() : int {
    // order(arr, cmp) with a custom comparator — splice must bail
    // (min/max can't honor arbitrary cmp).
    return _fold(each([10, 5, 20, 3]) |> order($(a : int, b : int) => a > b) |> first())
}

[test(description = "order_by then first folds into a streaming minimum with an inline less test and no order_by or first call")]
def test_order_by_first_emits_min_by(t : T?) {
    // order_by + first with an inlineable key now splices to the
    // streaming-min walk — single `var order_best` + `var order_seen`
    // state, per-element `_::less(it, order_best)` direct call. No min_by
    // lambda dispatch, no full materialization.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_order_by_first_splices_min_by)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched, "should have return expression")
        t |> equal(0, count_call(body_expr, "min_by"), "streaming-min supersedes min_by")
        t |> equal(0, count_call(body_expr, "max_by"), "should NOT emit max_by (ascending)")
        t |> equal(0, count_call(body_expr, "order_by"), "should NOT emit order_by")
        t |> equal(0, count_call(body_expr, "first"), "should NOT emit first (splice absorbs)")
        t |> success(count_call(body_expr, "less") >= 1, "streaming-min uses inline less-test")
    }
}

[test(description = "order_by_descending then first folds into a streaming maximum with the less arguments flipped")]
def test_order_by_descending_first_emits_max_by(t : T?) {
    // Streaming-min handles descending by flipping the inline less-test
    // operand order (`_::less(order_best, it)`); same shape as ascending.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_order_by_descending_first_splices_max_by)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched, "should have return expression")
        t |> equal(0, count_call(body_expr, "max_by"), "streaming-min supersedes max_by")
        t |> equal(0, count_call(body_expr, "min_by"), "should NOT emit min_by (descending)")
        t |> equal(0, count_call(body_expr, "order_by_descending"),
                   "should NOT emit order_by_descending")
        t |> equal(0, count_call(body_expr, "first"), "should NOT emit first (splice absorbs)")
        t |> success(count_call(body_expr, "less") >= 1,
                     "streaming-min uses inline less-test (descending flips args)")
    }
}

[test(description = "where, order_by and first fold into one streaming-minimum walk over the source with no prefilter buffer")]
def test_where_order_by_first_emits_min_by_on_prefilter(t : T?) {
    // where + order_by + first with an inlineable key splices to
    // streaming-min with the per-element body wrapped in `if
    // (whereCond)`. Single fused loop — no prefilter buffer.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_where_order_by_first_splices_min_by_on_buf)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched, "should have return expression")
        t |> equal(0, count_call(body_expr, "min_by"), "streaming-min supersedes min_by")
        t |> equal(0, count_call(body_expr, "order_by"), "should NOT emit order_by")
        t |> equal(0, count_call(body_expr, "first"), "should NOT emit first (splice absorbs)")
        t |> equal(1, count_inner_for_loops(body_expr),
                   "single fused walk over source (no prefilter buf)")
        t |> success(count_call(body_expr, "less") >= 1, "streaming-min uses inline less-test")
    }
}

[test(description = "order_by then first_or_default folds into a streaming minimum that absorbs first_or_default")]
def test_order_by_first_or_default_emits_top_n_by_first_or_default(t : T?) {
    // order_by + first_or_default with inlineable key splices to
    // streaming-min + return-default when !seen. No top_n_by(_, 1, _)
    // materialization, no min_by_or_default helper.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_order_by_first_or_default_splices_top_n_by)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched, "should have return expression")
        t |> equal(0, count_call(body_expr, "top_n_by_with_cmp"),
                   "streaming-min supersedes top_n_by_with_cmp")
        t |> equal(0, count_call(body_expr, "top_n_by"), "streaming-min supersedes top_n_by")
        t |> equal(0, count_call(body_expr, "first_or_default"),
                   "streaming-min absorbs first_or_default")
        t |> equal(0, count_call(body_expr, "order_by"), "should NOT emit order_by")
        t |> success(count_call(body_expr, "less") >= 1, "streaming-min uses inline less-test")
    }
}

[test(description = "order with a custom comparator then first does not become a min or max since the comparator could not be honored")]
def test_order_with_cmp_first_bails_no_min_max(t : T?) {
    // `order(arr, cmp)` with a custom comparator block: splice helpers
    // can't honor an arbitrary cmp, so plan_order_family must bail.
    // fold_linq_default takes over and rewrites to `order_to_array` +
    // `first`. The key invariant: NO min/max splice (which would silently
    // drop the cmp).
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_order_with_cmp_first_bails_to_tier2)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched, "should have return expression")
        t |> equal(0, count_call(body_expr, "min_by"),
                   "must NOT emit min_by (cmp can't be honored)")
        t |> equal(0, count_call(body_expr, "max_by"), "must NOT emit max_by")
        t |> equal(0, count_call(body_expr, "min"), "must NOT emit min")
        t |> equal(0, count_call(body_expr, "max"), "must NOT emit max")
        t |> equal(0, count_call(body_expr, "top_n"),
                   "must NOT emit top_n (bare top_n would drop cmp)")
        t |> equal(0, count_call(body_expr, "top_n_by"), "must NOT emit top_n_by")
        // fold_linq_default rewrites the bailed chain to order_to_array +
        // first. Confirm a sort step survives.
        t |> success(count_call(body_expr, "order_to_array") + count_call(body_expr, "order") >= 1,
            "sort step must survive (order_to_array or order)")
        t |> success(count_call(body_expr, "first") >= 1, "first call must survive")
    }
}

[export, marker(no_coverage)]
def target_min_max_fold() : tuple<int; int> => _fold([5, 1, 9]._where(_ > 0) |> min_max())

[export, marker(no_coverage)]
def target_min_by_fold() : int => _fold([5, 1, 9] |> _min_by(_ % 5))

[test(description = "the min_max and min_by folds inline their calls and walk the source once")]
def test_min_max_family_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_min_max_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        t |> equal(count_inner_for_loops(body_expr), 1, "min_max fuses the where into one loop")
        t |> equal(count_call(body_expr, "min_max"), 0, "min_max call must be inlined")
        var func2 = find_module_function_via_rtti(compiling_module(), @@target_min_by_fold)
        if (func2 == null) return
        var body2 : ExpressionPtr
        let r2 = qmatch_function(func2) $() {
            return $e(body2)
        }
        t |> success(r2.matched && body2 is ExprInvoke, "expected splice invoke wrapper (min_by)")
        t |> equal(count_inner_for_loops(body2), 1, "min_by walks once")
        t |> equal(count_call(body2, "min_by"), 0, "min_by call must be inlined")
    }
}
