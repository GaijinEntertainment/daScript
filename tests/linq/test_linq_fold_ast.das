options gen2
options indenting = 4
options rtti

require daslib/ast
require daslib/rtti
require daslib/ast_match
require daslib/ast_boost
require daslib/templates_boost
require daslib/linq
require daslib/linq_boost
require dastest/testing_boost public

// ── Target functions (fold happens at macro time) ──────────────────────

// `_fold` targets — used by behavioral *_fold_result tests + new loop-AST tests.
[export, marker(no_coverage)]
def target_where_fold() : array<int> {
    return <- [1, 2, 3, 4, 5]._where(_ > 3)._fold()
}

[export, marker(no_coverage)]
def target_select_fold() : array<int> {
    return <- [1, 2, 3, 4, 5]._select(_ * 2)._fold()
}

[export, marker(no_coverage)]
def target_where_select_fold() : array<int> {
    return <- [1, 2, 3, 4, 5]._where(_ > 3)._select(_ * 2)._fold()
}

[export, marker(no_coverage)]
def target_select_where_fold() : array<int> {
    return <- [1, 2, 3, 4, 5]._select(_ * 2)._where(_ > 6)._fold()
}

[export, marker(no_coverage)]
def target_reverse_where_fold() : array<int> {
    return <- [1, 2, 3, 4, 5].to_sequence().reverse()._where(_ > 3).to_array()._fold()
}

[export, marker(no_coverage)]
def target_zip_fold() : array<tuple<int; int>> {
    return <- [1, 2, 3]._select(_ * 2).zip([10, 20, 30]._select(_ + 1))._fold()
}

[export, marker(no_coverage)]
def target_zip3_fold() : array<tuple<int; int; int>> {
    return <- [1, 2, 3]._select(_ * 2).zip([10, 20, 30]._select(_ + 1), [100, 200, 300]._select(_ / 10))._fold()
}

[export, marker(no_coverage)]
def target_zip3_predicate_fold() : array<int> {
    return <- [1, 2, 3]._select(_ * 2).zip([10, 20, 30]._select(_ + 1), [100, 200, 300]._select(_ / 10), $(a, b, c : int) => a + b + c)._fold()
}

// `_old_fold` targets — used by retargeted AST tests that document the frozen comprehension contract.
[export, marker(no_coverage)]
def target_where_old_fold() : array<int> {
    return <- [1, 2, 3, 4, 5]._where(_ > 3)._old_fold()
}

[export, marker(no_coverage)]
def target_select_old_fold() : array<int> {
    return <- [1, 2, 3, 4, 5]._select(_ * 2)._old_fold()
}

[export, marker(no_coverage)]
def target_where_select_old_fold() : array<int> {
    return <- [1, 2, 3, 4, 5]._where(_ > 3)._select(_ * 2)._old_fold()
}

[export, marker(no_coverage)]
def target_select_where_old_fold() : array<int> {
    return <- [1, 2, 3, 4, 5]._select(_ * 2)._where(_ > 6)._old_fold()
}

[export, marker(no_coverage)]
def target_reverse_where_old_fold() : array<int> {
    return <- [1, 2, 3, 4, 5].to_sequence().reverse()._where(_ > 3).to_array()._old_fold()
}

[export, marker(no_coverage)]
def target_zip_old_fold() : array<tuple<int; int>> {
    return <- [1, 2, 3]._select(_ * 2).zip([10, 20, 30]._select(_ + 1))._old_fold()
}

[export, marker(no_coverage)]
def target_zip3_old_fold() : array<tuple<int; int; int>> {
    return <- [1, 2, 3]._select(_ * 2).zip([10, 20, 30]._select(_ + 1), [100, 200, 300]._select(_ / 10))._old_fold()
}

[export, marker(no_coverage)]
def target_zip3_predicate_old_fold() : array<int> {
    return <- [1, 2, 3]._select(_ * 2).zip([10, 20, 30]._select(_ + 1), [100, 200, 300]._select(_ / 10), $(a, b, c : int) => a + b + c)._old_fold()
}

// ── Tests: _old_fold contract — comprehension emission (frozen baseline) ──
// These tests retain the pre-rewrite AST shape that `_fold` used to emit.
// `_fold` itself has diverged (Phase 2A loop planner); see test_*_fold_emits_loop
// below for the current `_fold` shape contract. The pair documents the
// comprehension-vs-loop split between the two macros.

[test]
def test_where_old_fold_produces_comprehension(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_where_old_fold)
        t |> success(func != null, "should find target_where_old_fold")
        if (func == null) return
        // fold_where output: invoke($(var source) .. var pass_0 <- COMP; return <- pass_0 .., src)
        var comp_expr : ExpressionPtr
        var source_expr : ExpressionPtr
        let r = qmatch_function(func) $() { // nolint:STYLE016
            return <- invoke($(var source : array<int>) {
                var pass_0 : array<int> <- $e(comp_expr)
                return <- pass_0
            }, $e(source_expr))
        }
        t |> success(r.matched, "should match fold invoke structure, error={int(r.error)}")
        if (!r.matched) return
        // Verify the captured expression is a comprehension with where
        var resolved <- qm_resolve_comprehension(comp_expr)
        t |> success(resolved != null, "inner expression should be a comprehension")
        if (resolved == null) return
        let ac = resolved as ExprArrayComprehension
        t |> success(ac.exprWhere != null, "comprehension should have where clause")
    }
}

[test]
def test_where_old_fold_comprehension_pattern(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_where_old_fold)
        if (func == null) return
        // Match the full structure including comprehension pattern
        var where_cond : ExpressionPtr
        var source_expr : ExpressionPtr
        let r = qmatch_function(func) $() { // nolint:STYLE016
            return <- invoke($(var source : array<int>) {
                var pass_0 : array<int> <- [for (it in source); it; where $e(where_cond)]
                return <- pass_0
            }, $e(source_expr))
        }
        t |> success(r.matched, "should match comprehension with where, error={int(r.error)}")
    }
}

[test]
def test_select_old_fold_produces_comprehension(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_select_old_fold)
        t |> success(func != null, "should find target_select_old_fold")
        if (func == null) return
        var comp_expr : ExpressionPtr
        var source_expr : ExpressionPtr
        let r = qmatch_function(func) $() { // nolint:STYLE016
            return <- invoke($(var source : array<int>) {
                var pass_0 : array<int> <- $e(comp_expr)
                return <- pass_0
            }, $e(source_expr))
        }
        t |> success(r.matched, "should match fold structure, error={int(r.error)}")
        if (!r.matched) return
        var resolved <- qm_resolve_comprehension(comp_expr)
        t |> success(resolved != null, "inner should be a comprehension")
        if (resolved == null) return
        let ac = resolved as ExprArrayComprehension
        t |> success(ac.exprWhere == null, "select-only comprehension should have no where clause")
    }
}

[test]
def test_select_old_fold_comprehension_pattern(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_select_old_fold)
        if (func == null) return
        var select_expr : ExpressionPtr
        var source_expr : ExpressionPtr
        let r = qmatch_function(func) $() { // nolint:STYLE016
            return <- invoke($(var source : array<int>) {
                var pass_0 : array<int> <- [for (it in source); $e(select_expr)]
                return <- pass_0
            }, $e(source_expr))
        }
        t |> success(r.matched, "should match comprehension without where, error={int(r.error)}")
        if (!r.matched) return
        // Verify the select expression is a multiplication: it * 2
        let r2 = qmatch(select_expr, it * 2)
        t |> success(r2.matched, "select expression should be it * 2")
    }
}

[test]
def test_where_select_old_fold_comprehension(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_where_select_old_fold)
        t |> success(func != null, "should find target_where_select_old_fold")
        if (func == null) return
        var select_expr : ExpressionPtr
        var where_cond : ExpressionPtr
        var source_expr : ExpressionPtr
        let r = qmatch_function(func) $() { // nolint:STYLE016
            return <- invoke($(var source : array<int>) {
                var pass_0 : array<int> <- [for (it in source); $e(select_expr); where $e(where_cond)]
                return <- pass_0
            }, $e(source_expr))
        }
        t |> success(r.matched, "should match comprehension with where+select, error={int(r.error)}")
        if (!r.matched) return
        // Verify select is multiplication and where is comparison
        let r_sel = qmatch(select_expr, it * 2)
        t |> success(r_sel.matched, "select should be it * 2")
        let r_where = qmatch(where_cond, it > 3)
        t |> success(r_where.matched, "where should be it > 3")
    }
}

[test]
def test_select_where_old_fold_structure(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_select_where_old_fold)
        t |> success(func != null, "should find target_select_where_old_fold")
        if (func == null) return
        // select_where fold produces an invoke with a lambda that has a for loop + if
        // It is NOT a simple comprehension - verify the fold still happened
        var inner_expr : ExpressionPtr
        var source_expr : ExpressionPtr
        let r = qmatch_function(func) $() { // nolint:STYLE016
            return <- invoke($(var source : array<int>) {
                var pass_0 : array<int> <- $e(inner_expr)
                return <- pass_0
            }, $e(source_expr))
        }
        t |> success(r.matched, "should match fold invoke structure, error={int(r.error)}")
        if (!r.matched) return
        // The inner expression should NOT be a comprehension (select_where uses a different strategy)
        var resolved <- qm_resolve_comprehension(inner_expr)
        t |> success(resolved == null, "select_where should not produce a simple comprehension")
    }
}

[test]
def test_reverse_where_old_fold_structure(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_reverse_where_old_fold)
        t |> success(func != null, "should find target_reverse_where_old_fold")
        if (func == null) return
        // Multi-step fold: reverse_to_array + where comprehension
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched, "should have a return expression")
        if (!r.matched) return
        t |> success(body_expr is ExprInvoke, "fold should produce invoke wrapper")
    }
}

[test]
def test_zip_old_fold_structure(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_zip_old_fold)
        t |> success(func != null, "should find target_zip_old_fold")
        if (func == null) return
        // zip fold recursively folds the second argument
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched, "should match return expression")
        if (!r.matched) return
        t |> success(body_expr is ExprInvoke, "fold should produce invoke wrapper")
    }
}

// ── Behavioral verification (fold produces correct results) ────────────

[test]
def test_where_fold_result(t : T?) {
    t |> run("where fold produces correct values") @(t : T?) {
        let result <- target_where_fold()
        t |> equal(length(result), 2)
        t |> equal(result[0], 4)
        t |> equal(result[1], 5)
    }
}

[test]
def test_select_fold_result(t : T?) {
    t |> run("select fold produces correct values") @(t : T?) {
        let result <- target_select_fold()
        t |> equal(length(result), 5)
        let expected = [2, 4, 6, 8, 10]
        for (i, v in 0..5, result) {
            t |> equal(expected[i], v)
        }
    }
}

[test]
def test_where_select_fold_result(t : T?) {
    t |> run("where_select fold produces correct values") @(t : T?) {
        let result <- target_where_select_fold()
        t |> equal(length(result), 2)
        t |> equal(result[0], 8)
        t |> equal(result[1], 10)
    }
}

[test]
def test_select_where_fold_result(t : T?) {
    t |> run("select_where fold produces correct values") @(t : T?) {
        let result <- target_select_where_fold()
        t |> equal(length(result), 2)
        t |> equal(result[0], 8)
        t |> equal(result[1], 10)
    }
}

[test]
def test_zip_fold_result(t : T?) {
    t |> run("zip fold produces correct values") @(t : T?) {
        let result <- target_zip_fold()
        t |> equal(length(result), 3)
        t |> equal(result[0]._0, 2)
        t |> equal(result[0]._1, 11)
        t |> equal(result[1]._0, 4)
        t |> equal(result[1]._1, 21)
        t |> equal(result[2]._0, 6)
        t |> equal(result[2]._1, 31)
    }
}

// ── Tests: zip3 fold — all 3 subexpressions fold ──────────────────────

[test]
def test_zip3_old_fold_structure(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_zip3_old_fold)
        t |> success(func != null, "should find target_zip3_old_fold")
        if (func == null) return
        // zip3 fold: all three sources should be folded into invoke wrappers
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched, "should match return expression")
        if (!r.matched) return
        t |> success(body_expr is ExprInvoke, "zip3 fold should produce invoke wrapper")
    }
}

[test]
def test_zip3_predicate_old_fold_structure(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_zip3_predicate_old_fold)
        t |> success(func != null, "should find target_zip3_predicate_old_fold")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched, "should match return expression")
        if (!r.matched) return
        t |> success(body_expr is ExprInvoke, "zip3 predicate fold should produce invoke wrapper")
    }
}

[test]
def test_zip3_fold_result(t : T?) {
    t |> run("zip3 fold produces correct tuples") @(t : T?) {
        let result <- target_zip3_fold()
        t |> equal(length(result), 3)
        // [1,2,3]._select(_*2) = [2,4,6]
        // [10,20,30]._select(_+1) = [11,21,31]
        // [100,200,300]._select(_/10) = [10,20,30]
        t |> equal(result[0]._0, 2)
        t |> equal(result[0]._1, 11)
        t |> equal(result[0]._2, 10)
        t |> equal(result[1]._0, 4)
        t |> equal(result[1]._1, 21)
        t |> equal(result[1]._2, 20)
        t |> equal(result[2]._0, 6)
        t |> equal(result[2]._1, 31)
        t |> equal(result[2]._2, 30)
    }
}

[test]
def test_zip3_predicate_fold_result(t : T?) {
    t |> run("zip3 with predicate fold produces correct sums") @(t : T?) {
        let result <- target_zip3_predicate_fold()
        t |> equal(length(result), 3)
        // (2+11+10), (4+21+20), (6+31+30)
        t |> equal(result[0], 23)
        t |> equal(result[1], 45)
        t |> equal(result[2], 67)
    }
}

// ── Targets for `_fold` Phase-2A loop planner ──────────────────────────

[export, marker(no_coverage)]
def target_chained_where_fold() : array<int> {
    return <- [1, 2, 3, 4, 5]._where(_ > 1)._where(_ < 5)._fold()
}

[export, marker(no_coverage)]
def target_chained_select_fold() : array<int> {
    return <- [1, 2, 3, 4, 5]._select(_ * 2)._select(_ + 1)._fold()
}

[export, marker(no_coverage)]
def target_where_count_fold() : int {
    return _fold(each([1, 2, 3, 4, 5])._where(_ > 2).count())
}

[export, marker(no_coverage)]
def target_chained_where_count_fold() : int {
    return _fold(each([1, 2, 3, 4, 5])._where(_ > 1)._where(_ < 5).count())
}

[export, marker(no_coverage)]
def target_count_fold() : int {
    return _fold(each([1, 2, 3, 4, 5]).count())
}

[export, marker(no_coverage)]
def target_select_count_fold() : int {
    return _fold(each([1, 2, 3, 4, 5])._select(_ * 2).count())
}

var g_select_count_proj_hits = 0

def side_effect_select_proj(x : int) : int {
    g_select_count_proj_hits ++
    return x * 2
}

[export, marker(no_coverage)]
def target_select_count_fold_impure() : int {
    return _fold(each([1, 2, 3, 4, 5])._select(side_effect_select_proj(_)).count())
}

// `each(range(...))` — argument is a `range`, not an array. peel_each must NOT fire
// here; we'd otherwise replace the iterator-yielding each call with the raw range
// and downstream length-shortcut / reserve-by-length would silently misbehave.
[export, marker(no_coverage)]
def target_each_range_count() : int {
    return _fold(each(range(10))._where(_ > 5).count())
}

// ── Targets for Phase-2B Ring 1 accumulator lane ───────────────────────

[export, marker(no_coverage)]
def target_sum_fold() : int {
    return _fold(each([1, 2, 3, 4, 5]).sum())
}

[export, marker(no_coverage)]
def target_min_workhorse_fold() : int {
    return _fold(each([5, 3, 8, 1, 4]).min())
}

[export, marker(no_coverage)]
def target_max_workhorse_fold() : int {
    return _fold(each([5, 3, 8, 1, 4]).max())
}

[export, marker(no_coverage)]
def target_min_non_workhorse_fold() : tuple<int; int> {
    // tuple<int;int> is NOT workhorse — min must dispatch to `_::less` for the compare.
    // Single-return body so qmatch_function can extract the expression directly.
    return _fold(each([(1, 20), (3, 5), (2, 10)]).min())
}

[export, marker(no_coverage)]
def target_average_fold() : double {
    return _fold(each([2, 4, 6, 8]).average())
}

[export, marker(no_coverage)]
def target_long_count_loop_fold() : int64 {
    // Where filter forces the loop path (not the length shortcut).
    return _fold(each([1, 2, 3, 4, 5])._where(_ > 2).long_count())
}

[export, marker(no_coverage)]
def target_long_count_shortcut_fold() : int64 {
    // Bare long_count on array source — pure path → length shortcut → int64(length(src)).
    return _fold(each([1, 2, 3, 4, 5]).long_count())
}

[export, marker(no_coverage)]
def target_select_where_sum_fall_through() : int {
    // `_select |> _where |> sum` is select-then-where which Phase 2A/2B planner rejects
    // (where-after-select is blocked on ExprRef2Value substitution). Should fall through
    // unfolded — body is the raw call chain, not an invoke wrapper. Use array-source form
    // (no each) so the unfolded chain stays in safe land.
    return [1, 2, 3, 4, 5]._select(_ * 2)._where(_ > 4).sum()._fold()
}

// ── Targets for Phase-2B Ring 2 early-exit lane ────────────────────────

[export, marker(no_coverage)]
def target_first_fold() : int {
    return _fold(each([1, 2, 3, 4, 5])._where(_ > 2).first())
}

[export, marker(no_coverage)]
def target_first_or_default_fold() : int {
    return _fold(each([1, 2, 3, 4, 5])._where(_ > 99).first_or_default(-1))
}

[export, marker(no_coverage)]
def target_any_loop_fold() : bool {
    return _fold(each([1, 2, 3, 4, 5])._where(_ > 3).any())
}

[export, marker(no_coverage)]
def target_any_shortcut_fold() : bool {
    // No upstream where + no per-element work + length-bearing source → `length(src) > 0`.
    return _fold(each([1, 2, 3, 4, 5]).any())
}

[export, marker(no_coverage)]
def target_all_fold() : bool {
    return _fold(each([1, 2, 3, 4, 5])._all(_ > 0))
}

[export, marker(no_coverage)]
def target_contains_fold() : bool {
    return _fold(each([1, 2, 3, 4, 5]).contains(3))
}

[export, marker(no_coverage)]
def target_early_exit_select_where_fall_through() : bool {
    // select-then-where-then-any is rejected by the planner (where-after-select).
    // Falls through unfolded; body is raw chain. Array-source form for safety.
    return [1, 2, 3, 4, 5]._select(_ * 2)._where(_ > 4).any()._fold()
}

// ── Tests: `_fold` Phase-2A loop emission ──────────────────────────────
// Phase-2A `_fold` emits explicit for-loops inside an `invoke($block, $src)` wrapper
// (no `ExprArrayComprehension` nodes). Each test asserts the invoke wrapper exists
// and the inner body is NOT a comprehension. Out-of-scope shapes fall through
// unfolded — body is the raw chain, not an invoke.

[test]
def test_where_fold_emits_loop(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_where_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched, "should have return expression")
        t |> success(body_expr is ExprInvoke, "_fold should produce invoke wrapper")
        if (!(body_expr is ExprInvoke)) return
        var body_clone = clone_expression(body_expr)
        var maybe_comp <- qm_resolve_comprehension(body_clone)
        t |> success(maybe_comp == null, "loop planner must NOT emit a comprehension")
    }
}

[test]
def test_select_fold_emits_loop(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_select_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched, "should have return expression")
        t |> success(body_expr is ExprInvoke, "_fold should produce invoke wrapper")
        if (!(body_expr is ExprInvoke)) return
        var body_clone = clone_expression(body_expr)
        var maybe_comp <- qm_resolve_comprehension(body_clone)
        t |> success(maybe_comp == null, "loop planner must NOT emit a comprehension")
    }
}

[test]
def test_chained_where_fold_emits_loop(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_chained_where_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched, "should have return expression")
        // Phase 2A fuses chained _where|_where into a single loop with && predicate
        t |> success(body_expr is ExprInvoke, "chained where should fuse into single invoke loop")
    }
}

[test]
def test_chained_select_fold_emits_loop(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_chained_select_fold)
        if (func == null) return
        // Chained _select|_select fuses via intermediate `var v_N = projection_N` bindings
        // — the next lambda's `_` is renamed straight to the prior binding's name so no
        // expression-substitution (and no ExprRef2Value-wrapping headaches) is needed.
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched, "should have return expression")
        t |> success(body_expr is ExprInvoke, "chained _select|_select should fuse into single invoke loop")
    }
}

[test]
def test_where_count_fold_emits_counter(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_where_count_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched, "should have return expression")
        t |> success(body_expr is ExprInvoke, "_where|_count should fuse into counter invoke")
    }
}

[test]
def test_chained_where_count_fold_emits_counter(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_chained_where_count_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched, "should have return expression")
        t |> success(body_expr is ExprInvoke, "chained where + count should fuse into single counter invoke")
    }
}

[test]
def test_count_fold_emits_counter(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_count_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched, "should have return expression")
        t |> success(body_expr is ExprInvoke, "bare count should fuse into unconditional counter invoke")
    }
}

[test]
def test_select_where_fold_falls_through(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_select_where_fold)
        if (func == null) return
        // _select |> _where is out of Phase 2A scope (where-after-select) — chain falls
        // through unfolded. The function body is the raw `where_(select(...), ...)` call,
        // NOT a generated invoke wrapper.
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched, "should have return expression")
        t |> success(!(body_expr is ExprInvoke), "select_where should fall through unfolded (no invoke wrapper)")
    }
}

// ── Behavioral parity: results of new shapes ───────────────────────────

[test]
def test_chained_where_fold_result(t : T?) {
    t |> run("chained where _fold produces correct values") @(t : T?) {
        let result <- target_chained_where_fold()
        t |> equal(length(result), 3)
        t |> equal(result[0], 2)
        t |> equal(result[1], 3)
        t |> equal(result[2], 4)
    }
}

[test]
def test_chained_select_fold_result(t : T?) {
    t |> run("chained select _fold produces correct values") @(t : T?) {
        let result <- target_chained_select_fold()
        t |> equal(length(result), 5)
        // [1,2,3,4,5] * 2 = [2,4,6,8,10] + 1 = [3,5,7,9,11]
        let expected = [3, 5, 7, 9, 11]
        for (i, v in 0..5, result) {
            t |> equal(expected[i], v)
        }
    }
}

[test]
def test_where_count_fold_result(t : T?) {
    t |> run("where _count _fold produces correct count") @(t : T?) {
        t |> equal(target_where_count_fold(), 3)
    }
}

[test]
def test_chained_where_count_fold_result(t : T?) {
    t |> run("chained where _count _fold produces correct count") @(t : T?) {
        t |> equal(target_chained_where_count_fold(), 3)
    }
}

[test]
def test_count_fold_result(t : T?) {
    t |> run("bare _count _fold produces source length") @(t : T?) {
        t |> equal(target_count_fold(), 5)
    }
}

[test]
def test_select_count_fold_result(t : T?) {
    t |> run("select _count _fold produces correct count (projection does not affect count value)") @(t : T?) {
        t |> equal(target_select_count_fold(), 5)
    }
}

// ── Counter-lane projection elision (has_sideeffects integration) ──────
// For pure counter chains (`_select(_ * 2).count()`, bare `.count()`, etc.) on
// length-supporting sources, the planner emits a `length(source)` shortcut and
// the for-loop is dropped entirely. For impure projections (function call w/
// side effects), the per-element loop is preserved with the discardable bind.

// Returns the number of ExprLet/ExprFor statements in the counter-lane invoke's
// inner block. Pure shortcut: `[var src, return length(src)]` → 0 for-loops.
// Impure loop: `[var src, var acc=0, for {...}, return acc]` → 1 for-loop.
def count_inner_for_loops(body_expr : Expression?) : int {
    if (!(body_expr is ExprInvoke)) return -1
    let inv = body_expr as ExprInvoke
    if (empty(inv.arguments) || !(inv.arguments[0] is ExprMakeBlock)) return -1
    let mb = inv.arguments[0] as ExprMakeBlock
    let outer = mb._block as ExprBlock
    if (outer == null) return -1
    var n = 0
    for (stmt in outer.list) {
        if (stmt is ExprFor) {
            n ++
        }
    }
    return n
}

// Returns the number of stmts in the for-body, or -1 if no for loop exists.
def count_for_body_stmts(body_expr : Expression?) : int {
    if (!(body_expr is ExprInvoke)) return -1
    let inv = body_expr as ExprInvoke
    if (empty(inv.arguments) || !(inv.arguments[0] is ExprMakeBlock)) return -1
    let mb = inv.arguments[0] as ExprMakeBlock
    let outer = mb._block as ExprBlock
    if (outer == null) return -1
    for (stmt in outer.list) {
        if (stmt is ExprFor) {
            let fe = stmt as ExprFor
            let fbody = fe.body as ExprBlock
            if (fbody == null) return -1
            return length(fbody.list)
        }
    }
    return -1
}

[test]
def test_pure_projection_uses_length_shortcut(t : T?) {
    // `_select(_ * 2).count()` on a length-supporting source should collapse to
    // `length(source)` — no for-loop emitted at all.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_select_count_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper")
        let n = count_inner_for_loops(body_expr)
        t |> equal(n, 0, "pure select-count must emit length() shortcut (no for loop)")
    }
}

[test]
def test_bare_count_uses_length_shortcut(t : T?) {
    // Bare `.count()` on an array source should also use the length shortcut.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_count_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper")
        let n = count_inner_for_loops(body_expr)
        t |> equal(n, 0, "bare count on length-supporting source must use length() shortcut")
    }
}

[test]
def test_impure_projection_keeps_bind(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_select_count_fold_impure)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected counter-lane invoke wrapper")
        let n = count_for_body_stmts(body_expr)
        t |> equal(n, 2, "impure projection should preserve vfinal bind (for-body has bind + ++acc)")
    }
}

// ── peel_each invariant: each(<array>) must always be peeled ──────────
// The planner's `peel_each` helper unwraps `each(x)` regardless of x's type so
// the emitted block sees the underlying container directly. Without this, the
// length() shortcut would never fire (each returns an iterator, which has no
// length) and array-lane reserve would emit against the iterator wrapper.

// Returns the second arg of the invoke (the source expression passed in). If
// it's still an ExprCall to `each`, peel didn't run.
def invoke_source_is_each_wrapped(body_expr : Expression?) : bool {
    if (!(body_expr is ExprInvoke)) return false
    let inv = body_expr as ExprInvoke
    if (length(inv.arguments) < 2 || !(inv.arguments[1] is ExprCall)) return false
    let src_call = inv.arguments[1] as ExprCall
    if (src_call.func == null) return false
    return (src_call.func.name == "each"
        || (src_call.func.fromGeneric != null && src_call.func.fromGeneric.name == "each"))
}

[test]
def test_peel_each_on_array_source(t : T?) {
    // Sanity: target_select_count_fold uses `each([1,2,3,4,5])`. After peel, the
    // invoke wrapper must NOT receive an each-wrapped source.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_select_count_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper")
        t |> success(!invoke_source_is_each_wrapped(body_expr),
            "peel_each must unwrap each(array) at macro time")
    }
}

[test]
def test_peel_each_on_bare_count(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_count_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper")
        t |> success(!invoke_source_is_each_wrapped(body_expr),
            "peel_each must unwrap each(array) at macro time")
    }
}

// Negative case: `each(range(...))` argument is an iterator-yielding range, not an
// array. peel_each must NOT fire — peeling would drop the each call and put the raw
// range in source position; the downstream length-shortcut and reserve hints would
// then misbehave on a non-indexable source.
[test]
def test_peel_each_skips_non_array_source(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_each_range_count)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper")
        t |> success(invoke_source_is_each_wrapped(body_expr),
            "peel_each must keep each(range) wrapper — only arrays may be peeled")
    }
}

[test]
def test_target_each_range_count_runs(t : T?) {
    // Behavioral: ensure the iterator-source chain still compiles and produces the
    // expected count. range(10) → [0,1,2,3,4,5,6,7,8,9]; filter > 5 → 4 elements.
    t |> equal(target_each_range_count(), 4)
}

// ── Phase 2B Ring 1 — accumulator lane shape assertions ────────────────
// Recursive AST walkers: count ExprOp2 with a given operator and ExprCall with a given
// function name across the entire subtree. These let us verify the workhorse-vs-`_::less`
// branch in min/max emission and the `acc += ...` / `+= 1l` shape in sum/long_count
// without locking into the exact ExprIfThenElse nesting (which is fragile to refactors).

def count_op2(expr : Expression?; op : string) : int {
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
    }
    return n
}

def count_call(expr : Expression?; funcName : string) : int {
    if (expr == null) return 0
    var n = 0
    if (expr is ExprCall) {
        let c = expr as ExprCall
        if (c.func != null) {
            // Generic instances carry a mangled `name` (e.g. `less`17876...`); the
            // un-mangled function name lives in `func.fromGeneric.name`. Check both.
            // `das_string` supports `==` directly so no `string()` cast needed (PERF007).
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
    }
    return n
}

def count_op1(expr : Expression?; op : string) : int {
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
    }
    return n
}

// Counts top-level `var` declarations in the outer block of an invoke wrapper.
// sum/long_count emit one accumulator; min/max emit two (first flag + best); average
// emits two (sum acc + count).
def count_outer_let_vars(body_expr : Expression?) : int {
    if (!(body_expr is ExprInvoke)) return -1
    let inv = body_expr as ExprInvoke
    if (empty(inv.arguments) || !(inv.arguments[0] is ExprMakeBlock)) return -1
    let mb = inv.arguments[0] as ExprMakeBlock
    let outer = mb._block as ExprBlock
    if (outer == null) return -1
    var n = 0
    for (stmt in outer.list) {
        if (stmt is ExprLet) n ++
    }
    return n
}

[test]
def test_sum_loop_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_sum_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper")
        // sum emits a single accumulator: `var acc : int = default<int>`
        t |> equal(1, count_outer_let_vars(body_expr), "sum: exactly one accumulator var")
        // Per-element update is `acc += value` — count ExprOp2 with op `+=`
        t |> success(count_op2(body_expr, "+=") >= 1, "sum: at least one '+=' per element")
    }
}

[test]
def test_min_workhorse_uses_lt(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_min_workhorse_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper")
        // Workhorse int → emit direct `<` compare, NOT `_::less` call
        t |> success(count_op2(body_expr, "<") >= 1, "min workhorse: at least one ExprOp2 '<'")
        t |> equal(0, count_call(body_expr, "less"), "min workhorse must NOT emit _::less call")
        // min/max emit two outer vars: `first_iter` flag + `best` accumulator
        t |> equal(2, count_outer_let_vars(body_expr), "min: first_iter flag + best")
    }
}

[test]
def test_max_workhorse_uses_gt(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_max_workhorse_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper")
        // Workhorse int max → direct `>` compare
        t |> success(count_op2(body_expr, ">") >= 1, "max workhorse: at least one ExprOp2 '>'")
        t |> equal(0, count_call(body_expr, "less"), "max workhorse must NOT emit _::less call")
        t |> equal(2, count_outer_let_vars(body_expr), "max: first_iter flag + best")
    }
}

[test]
def test_min_non_workhorse_uses_less(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_min_non_workhorse_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        // Non-workhorse tuple return is move-returned (`return <- ...`) — match that form.
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper")
        // tuple<int;int> is non-workhorse → fall back to _::less for the compare
        t |> success(count_call(body_expr, "less") >= 1, "min non-workhorse: at least one _::less call")
    }
}

[test]
def test_average_emits_sum_and_count(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_average_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper")
        // average emits two accumulators: sum + count
        t |> equal(2, count_outer_let_vars(body_expr), "average: sum + count accumulators")
        // Per-element work: sum += value (ExprOp2 '+=') and cnt ++ (postfix ExprOp1 '+++')
        t |> success(count_op2(body_expr, "+=") >= 1, "average: at least one '+=' for sum acc")
        t |> success(count_op1(body_expr, "++") >= 1, "average: at least one '++' for count")
    }
}

[test]
def test_long_count_loop_shape(t : T?) {
    ast_gc_guard() {
        // The where-filtered chain forces the loop path (length shortcut is gated on
        // whereCond == null) — verifies the int64 accumulator emission.
        var func = find_module_function_via_rtti(compiling_module(), @@target_long_count_loop_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper")
        t |> equal(1, count_outer_let_vars(body_expr), "long_count: exactly one int64 accumulator")
        // long_count emits `acc++` (postfix ExprOp1 '+++') per matched element
        t |> success(count_op1(body_expr, "++") >= 1, "long_count: at least one '++' per matched element")
    }
}

[test]
def test_long_count_length_shortcut(t : T?) {
    ast_gc_guard() {
        // Bare long_count on array source → length shortcut elides the loop entirely.
        var func = find_module_function_via_rtti(compiling_module(), @@target_long_count_shortcut_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper")
        let nForLoops = count_inner_for_loops(body_expr)
        t |> equal(0, nForLoops, "long_count shortcut: no for-loop (length emitted directly)")
    }
}

[test]
def test_accumulator_falls_through_on_select_where(t : T?) {
    ast_gc_guard() {
        // `select |> where_ |> sum` is rejected by the planner (select-then-where blocker).
        // Should fall through unfolded — body is the raw call chain, not an invoke wrapper.
        var func = find_module_function_via_rtti(compiling_module(), @@target_select_where_sum_fall_through)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched, "should have return expression")
        t |> success(!(body_expr is ExprInvoke),
            "select|where|sum should fall through unfolded (no invoke wrapper)")
    }
}

// ── Phase 2B Ring 2 — early-exit lane shape assertions ─────────────────

[test]
def test_first_loop_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_first_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper")
        // first emits a loop with early return + tail panic. Verify presence of a panic call
        // (sentinel for first's "sequence contains no elements" branch).
        t |> success(count_call(body_expr, "panic") >= 1, "first: panic call in tail")
    }
}

[test]
def test_first_or_default_returns_d(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_first_or_default_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper")
        // first_or_default tail returns the bound default — never panics.
        t |> equal(0, count_call(body_expr, "panic"), "first_or_default: no panic call in tail")
    }
}

[test]
def test_any_loop_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_any_loop_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper")
        // any (with where) emits a loop. There IS a for-loop (not shortcut path).
        t |> equal(1, count_inner_for_loops(body_expr), "any (with where): one for-loop")
    }
}

[test]
def test_any_empty_shortcut(t : T?) {
    ast_gc_guard() {
        // Predicate-free `each(arr).any()` on length-bearing source → `!empty(src)`.
        // No for-loop, no per-element work.
        var func = find_module_function_via_rtti(compiling_module(), @@target_any_shortcut_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper")
        t |> equal(0, count_inner_for_loops(body_expr), "any-shortcut: no for-loop emitted")
        t |> success(count_call(body_expr, "empty") >= 1, "any-shortcut: `empty(src)` call present")
        t |> success(count_op1(body_expr, "!") >= 1, "any-shortcut: `!empty(src)` negation present")
    }
}

[test]
def test_all_loop_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_all_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper")
        // all emits `if (!pred) return false` — at least one ExprOp1 with op `!`.
        t |> success(count_op1(body_expr, "!") >= 1, "all: at least one `!` for predicate negation")
        t |> equal(1, count_inner_for_loops(body_expr), "all: one for-loop")
    }
}

[test]
def test_contains_loop_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_contains_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper")
        // contains emits `if (it == v) return true` — at least one ExprOp2 with op `==`.
        t |> success(count_op2(body_expr, "==") >= 1, "contains: at least one `==` per element")
        t |> equal(1, count_inner_for_loops(body_expr), "contains: one for-loop")
    }
}

[test]
def test_early_exit_falls_through_on_select_where(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_early_exit_select_where_fall_through)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched, "should have return expression")
        t |> success(!(body_expr is ExprInvoke),
            "select|where|any should fall through unfolded (no invoke wrapper)")
    }
}

// ── Phase 2C take/skip splice ──────────────────────────────────────────

[export, marker(no_coverage)]
def target_where_take_count_fold() : int {
    return _fold(each([1, 2, 3, 4, 5, 6, 7, 8, 9, 10])._where(_ % 2 == 0).take(3).count())
}

[export, marker(no_coverage)]
def target_skip_take_to_array_fold() : array<int> {
    return <- _fold(each([1, 2, 3, 4, 5, 6, 7]).skip(2).take(3).to_array())
}

[export, marker(no_coverage)]
def target_order_by_take_falls_through() : array<int> {
    // Buffer-required (order_by) → splice planner returns null → silent fallback to raw linq.
    return <- _fold([1, 2, 3, 4, 5].to_sequence()._select(_ * 2)._order_by(_).take(2).to_array())
}

[export, marker(no_coverage)]
def target_distinct_falls_through() : array<int> {
    // distinct is buffer-required — splice planner returns null → silent fallback.
    return <- _fold([1, 2, 2, 3, 3, 3].to_sequence().distinct().to_array())
}

[export, marker(no_coverage)]
def target_take_sum_fold() : int {
    return _fold([1, 2, 3, 4, 5]._select(_ * 10).take(3).sum())
}

// AST helper — total break/continue count anywhere in the subtree. Used to assert that the
// take-limit `break` and skip-counter `continue` actually got spliced into the per-element body.
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

[test]
def test_take_splices_break_into_loop(t : T?) {
    // where.take.count: counter lane with take guard → one for-loop + at least one break
    // (the `if (taken == N) break` guard).
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

[test]
def test_skip_take_splices_continue_and_break(t : T?) {
    // skip.take.to_array: array lane with both guards → continue (skip) + break (take).
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_skip_take_to_array_fold)
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

[test]
def test_take_sum_splices_in_accumulator(t : T?) {
    // select.take.sum: accumulator lane with take → one for-loop + acc += ... + break.
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

[test]
def test_order_by_take_falls_through(t : T?) {
    // order_by + take is BufferTopN territory — recognized as buffer-required, planner returns
    // null, falls through to plain linq. Future PR replaces with a dedicated emit path.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_order_by_take_falls_through)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched, "should have return expression")
        t |> success(!(body_expr is ExprInvoke),
            "order_by chain should fall through unfolded — splice planner doesn't handle buffer mode yet")
    }
}

[test]
def test_distinct_falls_through(t : T?) {
    // distinct is buffer-required (hash set) — fall-through marker.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_distinct_falls_through)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched, "should have return expression")
        t |> success(!(body_expr is ExprInvoke),
            "distinct chain should fall through unfolded — BufferDistinct mode not yet implemented")
    }
}

