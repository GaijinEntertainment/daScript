options gen2
options indenting = 4
options rtti
options disable_auto_inline    // this file pins linq-fold codegen shapes; splicing would dissolve them

require daslib/ast
require daslib/rtti
require daslib/ast_match
require daslib/ast_boost
require daslib/templates_boost
require daslib/linq
require daslib/linq_boost
require dastest/testing_boost public

// `_fold` targets — used by behavioral *_fold_result tests + new loop-AST tests.
[export, marker(no_coverage)]
def target_where_fold() : array<int> => <- [1, 2, 3, 4, 5]._where(_ > 3)._fold()

[export, marker(no_coverage)]
def target_select_fold() : array<int> => <- [1, 2, 3, 4, 5]._select(_ * 2)._fold()

// Phase 2 Z1 targets — bare zip (no upstream select) hits plan_zip splice.
[export, marker(no_coverage)]
def target_zip_bare_arr_fold() : array<tuple<int; int>> => <- [1, 2, 3].zip([10, 20, 30])._fold()

[export, marker(no_coverage)]
def target_zip_bare_iter_fold() : array<tuple<int; int>> =>
    <- zip([1, 2, 3].each(), [10, 20, 30].each()).to_array()._fold()

[export, marker(no_coverage)]
def target_zip_bare_iter_seq_fold() : iterator<tuple<int; int>> =>
    <- zip([1, 2, 3].each(), [10, 20, 30].each())._fold()

[export, marker(no_coverage)]
def target_zip_bare_unequal_fold() : array<tuple<int; int>> =>
    <- [1, 2, 3, 4, 5].zip([10, 20, 30])._fold()

// Phase 2 Z2 targets — count / long_count terminators on bare zip.
[export, marker(no_coverage)]
def target_zip_count_fold() : int => _fold([1, 2, 3].zip([10, 20, 30]).count())

[export, marker(no_coverage)]
def target_zip_long_count_fold() : int64 => _fold([1, 2, 3].zip([10, 20, 30]).long_count())

[export, marker(no_coverage)]
def target_zip_count_iter_fold() : int {
    // to_sequence_move yields a real iterator (no peel, no length) → counter loop fallback.
    return _fold(zip([1, 2, 3, 4, 5].to_sequence_move(), [10, 20, 30].to_sequence_move()).count())
}

[export, marker(no_coverage)]
def target_zip_count_unequal_fold() : int => _fold([1, 2, 3, 4, 5].zip([10, 20, 30]).count())

// Phase 2 Z3 targets — chain ops between zip and terminator (where_/select/take/skip/take_while/skip_while).
// All use the `$(t : tuple<int;int>)` block form so the typer accepts the chain at the inner-arg level (needed before plan_zip fires).
[export, marker(no_coverage)]
def target_zip_where_arr_fold() : array<tuple<int; int>> =>
    <- [10, 20, 30, 40, 50].zip([1, 2, 3, 4, 5]) |> where_($(t : tuple<int; int>) => t._0 > 20) |> _fold()

[export, marker(no_coverage)]
def target_zip_select_arr_fold() : array<int> =>
    <- [10, 20, 30, 40, 50].zip([1, 2, 3, 4, 5]) |> select($(t : tuple<int; int>) => t._0 + t._1) |> _fold()

[export, marker(no_coverage)]
def target_zip_where_select_fold() : array<int> =>
    <- [10, 20, 30, 40, 50].zip([1, 2, 3, 4, 5]) |> where_($(t : tuple<int; int>) => t._0 > 20) |> select($(t : tuple<int; int>) => t._0 + t._1) |> _fold()

[export, marker(no_coverage)]
def target_zip_take_fold() : array<tuple<int; int>> =>
    <- [10, 20, 30, 40, 50].zip([1, 2, 3, 4, 5]) |> take(2) |> _fold()

[export, marker(no_coverage)]
def target_zip_skip_fold() : array<tuple<int; int>> =>
    <- [10, 20, 30, 40, 50].zip([1, 2, 3, 4, 5]) |> skip(3) |> _fold()

[export, marker(no_coverage)]
def target_zip_take_while_fold() : array<tuple<int; int>> =>
    <- [10, 20, 30, 40, 50].zip([1, 2, 3, 4, 5]) |> take_while($(t : tuple<int; int>) => t._0 < 30) |> _fold()

[export, marker(no_coverage)]
def target_zip_skip_while_fold() : array<tuple<int; int>> =>
    <- [10, 20, 30, 40, 50].zip([1, 2, 3, 4, 5]) |> skip_while($(t : tuple<int; int>) => t._0 < 30) |> _fold()

[export, marker(no_coverage)]
def target_zip_where_count_fold() : int =>
    _fold([10, 20, 30, 40, 50].zip([1, 2, 3, 4, 5]) |> where_($(t : tuple<int; int>) => t._0 > 20) |> count())

[export, marker(no_coverage)]
def target_zip_take_count_fold() : int =>
    _fold([10, 20, 30, 40, 50].zip([1, 2, 3, 4, 5]) |> take(3) |> count())

[export, marker(no_coverage)]
def target_zip_select_count_fold() : int {
    // Pure projection + count: the counter lane skips evaluating projection (no side effect to preserve), so this still beats a per-elem proj eval.
    return _fold([10, 20, 30, 40, 50].zip([1, 2, 3, 4, 5]) |> select($(t : tuple<int; int>) => t._0 + t._1) |> count())
}

// Phase 2 Z6 bail target — result-selector zip form. plan_zip bails on zipArgCount != 2; tier-2 cascade (fold_linq_default) handles it. Test only that result is correct end-to-end.
[export, marker(no_coverage)]
def target_zip_result_selector_bail() : array<int> =>
    <- [10, 20, 30, 40, 50].zip([1, 2, 3, 4, 5], $(p : int; q : int) => p + q) |> _fold()

// Phase 2 Z6 bail target — chain op (order) that plan_zip doesn't fuse. Tier-2 cascade handles it.
[export, marker(no_coverage)]
def target_zip_order_bail() : array<int> =>
    <- [30, 10, 40, 20, 50].zip([3, 1, 4, 2, 5]) |> select($(t : tuple<int; int>) => t._0 + t._1) |> order() |> _fold()

// PR #2741 Copilot review thread #r3269663361 — impure select projection ahead of a range op. Eager pipeline runs proj on every element; spliced order would skip it on dropped items, so plan_zip bails. Track proj-eval count via a global counter to assert tier-2 semantics survived.
var g_zip_proj_hits = 0

def private side_effect_zip_proj(p : int) : int {
    g_zip_proj_hits++
    return p
}

[export, marker(no_coverage)]
def target_zip_impure_select_skip_bails() : array<int> =>
    <- [10, 20, 30, 40, 50].zip([1, 2, 3, 4, 5]) |> select($(t : tuple<int; int>) => side_effect_zip_proj(t._0)) |> skip(2) |> _fold()

// ── PR Phase 2B+ — accumulator (sum/min/max/average) terminators on zip ──
// sum/min/max/average require a `select` projection to scalarize the tuple element (accumulator ops aren't defined on tuples). `long_count` is the exception: it routes through the COUNTER path locally (length shortcut + counter loop) since the accumulator is just `int64 acc++` regardless of element type.
[export, marker(no_coverage)]
def target_zip_sum_proj_fold() : int =>
    _fold([10, 20, 30, 40, 50].zip([1, 2, 3, 4, 5]) |> select($(t : tuple<int; int>) => t._0 + t._1) |> sum())

[export, marker(no_coverage)]
def target_zip_min_proj_fold() : int =>
    _fold([10, 20, 30, 40, 50].zip([1, 2, 3, 4, 5]) |> select($(t : tuple<int; int>) => t._0 - t._1) |> min())

[export, marker(no_coverage)]
def target_zip_max_proj_fold() : int =>
    _fold([10, 20, 30, 40, 50].zip([1, 2, 3, 4, 5]) |> select($(t : tuple<int; int>) => t._0 * t._1) |> max())

[export, marker(no_coverage)]
def target_zip_average_proj_fold() : double =>
    _fold([10, 20, 30, 40, 50].zip([1, 2, 3, 4, 5]) |> select($(t : tuple<int; int>) => t._0 + t._1) |> average())

[export, marker(no_coverage)]
def target_zip_where_sum_proj_fold() : int =>
    _fold([10, 20, 30, 40, 50].zip([1, 2, 3, 4, 5]) |> where_($(t : tuple<int; int>) => t._0 > 20) |> select($(t : tuple<int; int>) => t._0 + t._1) |> sum())

[export, marker(no_coverage)]
def target_zip_where_long_count_fold() : int64 =>
    _fold([10, 20, 30, 40, 50].zip([1, 2, 3, 4, 5]) |> where_($(t : tuple<int; int>) => t._0 > 20) |> long_count())

// ── PR Phase 2B+ — early-exit (first/first_or_default/any/all/contains) terminators on zip ──
[export, marker(no_coverage)]
def target_zip_first_no_proj_fold() : tuple<int; int> =>
    _fold([10, 20, 30, 40, 50].zip([1, 2, 3, 4, 5]) |> first())

[export, marker(no_coverage)]
def target_zip_first_proj_fold() : int =>
    _fold([10, 20, 30, 40, 50].zip([1, 2, 3, 4, 5]) |> select($(t : tuple<int; int>) => t._0 + t._1) |> first())

[export, marker(no_coverage)]
def target_zip_where_first_fold() : tuple<int; int> =>
    _fold([10, 20, 30, 40, 50].zip([1, 2, 3, 4, 5]) |> where_($(t : tuple<int; int>) => t._0 > 20) |> first())

[export, marker(no_coverage)]
def target_zip_first_or_default_proj_fold() : int =>
    _fold([10, 20, 30, 40, 50].zip([1, 2, 3, 4, 5]) |> select($(t : tuple<int; int>) => t._0 + t._1) |> where_($(v : int) => v > 1000) |> first_or_default(-1))

[export, marker(no_coverage)]
def target_zip_any_no_pred_fold() : bool =>
    _fold([10, 20, 30, 40, 50].zip([1, 2, 3, 4, 5]) |> any())

[export, marker(no_coverage)]
def target_zip_any_no_pred_empty_fold() : bool {
    var emptyA : array<int>
    return _fold(emptyA.zip([1, 2, 3]) |> any())
}

[export, marker(no_coverage)]
def target_zip_any_pred_fold() : bool =>
    _fold([10, 20, 30, 40, 50].zip([1, 2, 3, 4, 5]) |> any($(t : tuple<int; int>) => t._0 > 30))

[export, marker(no_coverage)]
def target_zip_all_pred_true_fold() : bool =>
    _fold([10, 20, 30, 40, 50].zip([1, 2, 3, 4, 5]) |> all($(t : tuple<int; int>) => t._0 > 0))

[export, marker(no_coverage)]
def target_zip_all_pred_false_fold() : bool =>
    _fold([10, 20, 30, 40, 50].zip([1, 2, 3, 4, 5]) |> all($(t : tuple<int; int>) => t._0 > 30))

[export, marker(no_coverage)]
def target_zip_contains_proj_fold() : bool =>
    _fold([10, 20, 30, 40, 50].zip([1, 2, 3, 4, 5]) |> select($(t : tuple<int; int>) => t._0 + t._1) |> contains(33))

[export, marker(no_coverage)]
def target_zip_contains_proj_miss_fold() : bool =>
    _fold([10, 20, 30, 40, 50].zip([1, 2, 3, 4, 5]) |> select($(t : tuple<int; int>) => t._0 + t._1) |> contains(999))

// Empty-source average target. Verifies the 2-site emit_accumulator_lane.average + emit_decs_accumulator.average alignment with linq.das (double accumulator + cnt==0 guard → 0.0lf). See test_zip_average_empty_returns_zero below.
[export, marker(no_coverage)]
def target_zip_average_empty_fold() : double {
    var emptyA, emptyB : array<int>
    return _fold(emptyA.zip(emptyB) |> select($(t : tuple<int; int>) => t._0 + t._1) |> average())
}

[export, marker(no_coverage)]
def target_zip3_fold() : array<tuple<int; int; int>> =>
    <- [1, 2, 3]._select(_ * 2).zip([10, 20, 30]._select(_ + 1), [100, 200, 300]._select(_ / 10))._fold()

[export, marker(no_coverage)]
def target_zip3_predicate_fold() : array<int> =>
    <- [1, 2, 3]._select(_ * 2).zip([10, 20, 30]._select(_ + 1), [100, 200, 300]._select(_ / 10), $(a, b, c : int) => a + b + c)._fold()

[test(description = "a folded three-source zip returns the combined tuples")]
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

[test(description = "a folded three-source zip with a predicate returns only the combinations that pass")]
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

[test(description = "a folded bare zip of two arrays returns the paired tuples")]
def test_zip_bare_arr_fold_result(t : T?) {
    t |> run("bare zip(arr, arr) produces matched tuples") @(t : T?) {
        let result <- target_zip_bare_arr_fold()
        t |> equal(length(result), 3)
        t |> equal(result[0]._0, 1);  t |> equal(result[0]._1, 10)
        t |> equal(result[1]._0, 2);  t |> equal(result[1]._1, 20)
        t |> equal(result[2]._0, 3);  t |> equal(result[2]._1, 30)
    }
}

[test(description = "a folded bare zip of two iterators returns the paired tuples as an array")]
def test_zip_bare_iter_fold_result(t : T?) {
    t |> run("bare zip(iter, iter) |> to_array produces matched tuples") @(t : T?) {
        let result <- target_zip_bare_iter_fold()
        t |> equal(length(result), 3)
        t |> equal(result[0]._0, 1);  t |> equal(result[0]._1, 10)
        t |> equal(result[2]._0, 3);  t |> equal(result[2]._1, 30)
    }
}

[test(description = "a folded bare zip of two iterators returns the paired tuples as an iterator")]
def test_zip_bare_iter_seq_fold_result(t : T?) {
    t |> run("bare zip(iter, iter) returns iterator that yields matched tuples") @(t : T?) {
        var n = 0
        for (pair in target_zip_bare_iter_seq_fold()) {
            t |> equal(pair._0, n + 1)
            t |> equal(pair._1, (n + 1) * 10)
            n ++
        }
        t |> equal(n, 3)
    }
}

[test(description = "a folded bare zip of arrays of unequal length stops at the shorter one")]
def test_zip_bare_unequal_fold_result(t : T?) {
    t |> run("bare zip truncates to shorter source") @(t : T?) {
        let result <- target_zip_bare_unequal_fold()
        t |> equal(length(result), 3)
        t |> equal(result[0]._0, 1);  t |> equal(result[0]._1, 10)
        t |> equal(result[2]._0, 3);  t |> equal(result[2]._1, 30)
    }
}

[test(description = "a folded bare array zip emits exactly one multi-iterator for loop with the zip call inlined")]
def test_zip_bare_arr_emits_single_for(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_zip_bare_arr_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        let n = count_inner_for_loops(body_expr)
        t |> equal(n, 1, "bare zip splice must emit exactly one (multi-iter) for-loop")
        let zipCalls = count_call(body_expr, "zip")
        t |> equal(zipCalls, 0, "zip call must be inlined into the for-loop, not preserved")
    }
}

[test(description = "a folded bare iterator zip emits exactly one for loop with the zip call inlined")]
def test_zip_bare_iter_emits_single_for(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_zip_bare_iter_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        let n = count_inner_for_loops(body_expr)
        t |> equal(n, 1, "iter+iter zip splice must emit exactly one for-loop")
        let zipCalls = count_call(body_expr, "zip")
        t |> equal(zipCalls, 0, "zip call must be inlined, not preserved")
    }
}

[test(description = "a folded zip then count returns the pair count")]
def test_zip_count_fold_result(t : T?) {
    t |> run("zip(arr, arr).count() returns matched length") @(t : T?) {
        t |> equal(target_zip_count_fold(), 3)
    }
}

[test(description = "a folded zip then long_count returns the pair count as int64")]
def test_zip_long_count_fold_result(t : T?) {
    t |> run("zip(arr, arr).long_count() returns int64 matched length") @(t : T?) {
        t |> equal(target_zip_long_count_fold(), 3l)
    }
}

[test(description = "a folded zip of two real iterators then count returns the pair count")]
def test_zip_count_iter_fold_result(t : T?) {
    t |> run("zip(iter, iter).count() walks both and returns min length") @(t : T?) {
        t |> equal(target_zip_count_iter_fold(), 3)
    }
}

[test(description = "a folded zip count of unequal arrays returns the shorter length")]
def test_zip_count_unequal_fold_result(t : T?) {
    t |> run("zip(arr, arr).count() truncates to shorter source") @(t : T?) {
        t |> equal(target_zip_count_unequal_fold(), 3)
    }
}

[test(description = "zip of two arrays then count folds to a length shortcut with no for loop")]
def test_zip_count_uses_length_shortcut(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_zip_count_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        let n = count_inner_for_loops(body_expr)
        t |> equal(n, 0, "zip(arr,arr).count() must use length shortcut (no for-loop)")
    }
}

[test(description = "zip of two arrays then long_count folds to a length shortcut with no for loop and no runtime call")]
def test_zip_long_count_uses_length_shortcut(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_zip_long_count_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        let n = count_inner_for_loops(body_expr)
        t |> equal(n, 0, "zip(arr,arr).long_count() must use length shortcut (no for-loop)")
        // Distinguish length-shortcut splice from tier-2 cascade by elimination: for_loops==0 rules out the counter-loop splice path; long_count==0 rules out tier-2 (which leaves the long_count call in place). Both together ⇒ length shortcut. (Can't grep for length() directly: count_call doesn't recurse into ExprOp3 ternary where length() lives in the min(...) expression.)
        let longCountCalls = count_call(body_expr, "long_count")
        t |> equal(longCountCalls, 0, "long_count must be inlined into the splice (no runtime call)")
    }
}

[test(description = "zip of two real iterators then count emits one counter loop with zip and count inlined")]
def test_zip_count_iter_emits_counter_loop(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_zip_count_iter_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        let n = count_inner_for_loops(body_expr)
        t |> equal(n, 1, "iter+iter source: count must walk both via counter loop (1 for)")
        let zipCalls = count_call(body_expr, "zip")
        t |> equal(zipCalls, 0, "zip must be inlined")
        let countCalls = count_call(body_expr, "count")
        t |> equal(countCalls, 0, "count must be inlined into acc++")
    }
}

[test(description = "a folded zip then where returns the pairs passing the filter")]
def test_zip_where_arr_fold_result(t : T?) {
    t |> run("zip + where_ filters by tuple element") @(t : T?) {
        let result <- target_zip_where_arr_fold()
        // a=[10,20,30,40,50], b=[1..5]: pairs where a>20 → [(30,3),(40,4),(50,5)]
        t |> equal(length(result), 3)
        t |> equal(result[0]._0, 30)
        t |> equal(result[0]._1, 3)
        t |> equal(result[2]._0, 50)
        t |> equal(result[2]._1, 5)
    }
}

[test(description = "a folded zip then select returns the projected pairs")]
def test_zip_select_arr_fold_result(t : T?) {
    t |> run("zip + select projects per element") @(t : T?) {
        let result <- target_zip_select_arr_fold()
        // sum each pair: [11, 22, 33, 44, 55]
        t |> equal(length(result), 5)
        t |> equal(result[0], 11)
        t |> equal(result[4], 55)
    }
}

[test(description = "a folded zip with where and select returns the filtered projections")]
def test_zip_where_select_fold_result(t : T?) {
    t |> run("zip + where + select chains predicate and projection") @(t : T?) {
        let result <- target_zip_where_select_fold()
        // filter a>20, then sum: [33, 44, 55]
        t |> equal(length(result), 3)
        t |> equal(result[0], 33)
        t |> equal(result[2], 55)
    }
}

[test(description = "a folded zip then take returns the first pairs")]
def test_zip_take_fold_result(t : T?) {
    t |> run("zip + take limits to N pairs") @(t : T?) {
        let result <- target_zip_take_fold()
        t |> equal(length(result), 2)
        t |> equal(result[0]._0, 10)
        t |> equal(result[1]._0, 20)
    }
}

[test(description = "a folded zip then skip returns the pairs after the skipped ones")]
def test_zip_skip_fold_result(t : T?) {
    t |> run("zip + skip drops K pairs from the front") @(t : T?) {
        let result <- target_zip_skip_fold()
        t |> equal(length(result), 2)
        t |> equal(result[0]._0, 40)
        t |> equal(result[1]._0, 50)
    }
}

[test(description = "a folded zip then take_while returns the leading pairs passing the predicate")]
def test_zip_take_while_fold_result(t : T?) {
    t |> run("zip + take_while yields prefix that matches predicate") @(t : T?) {
        let result <- target_zip_take_while_fold()
        // take while a < 30 → [(10,1),(20,2)]
        t |> equal(length(result), 2)
        t |> equal(result[0]._0, 10)
        t |> equal(result[1]._0, 20)
    }
}

[test(description = "a folded zip then skip_while returns the pairs after the leading run passing the predicate")]
def test_zip_skip_while_fold_result(t : T?) {
    t |> run("zip + skip_while yields tail after first false") @(t : T?) {
        let result <- target_zip_skip_while_fold()
        // skip while a < 30 → [(30,3),(40,4),(50,5)]
        t |> equal(length(result), 3)
        t |> equal(result[0]._0, 30)
        t |> equal(result[2]._0, 50)
    }
}

[test(description = "a folded zip with where then count counts the passing pairs")]
def test_zip_where_count_fold_result(t : T?) {
    t |> run("zip + where + count returns matched count") @(t : T?) {
        t |> equal(target_zip_where_count_fold(), 3)
    }
}

[test(description = "a folded zip with take then count counts the taken pairs")]
def test_zip_take_count_fold_result(t : T?) {
    t |> run("zip + take + count returns min(N, len)") @(t : T?) {
        t |> equal(target_zip_take_count_fold(), 3)
    }
}

[test(description = "a folded zip with select then count returns the pair count")]
def test_zip_select_count_fold_result(t : T?) {
    t |> run("zip + select + count returns full length (pure projection)") @(t : T?) {
        t |> equal(target_zip_select_count_fold(), 5)
    }
}

[test(description = "zip then where emits one for loop with the where inlined into an if")]
def test_zip_where_emits_single_for_with_filter(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_zip_where_arr_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        let n = count_inner_for_loops(body_expr)
        t |> equal(n, 1, "zip+where must emit exactly one for-loop")
        let whereCalls = count_call(body_expr, "where_")
        t |> equal(whereCalls, 0, "where_ call must be inlined into if-wrap, not preserved")
    }
}

[test(description = "zip then select emits one for loop with the projection inlined into the push")]
def test_zip_select_emits_inline_projection(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_zip_select_arr_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        let n = count_inner_for_loops(body_expr)
        t |> equal(n, 1, "zip+select must emit one for-loop with inline projection")
        let selectCalls = count_call(body_expr, "select")
        t |> equal(selectCalls, 0, "select call must be inlined into push_clone(projection)")
    }
}

[test(description = "zip then take emits one for loop with a counter and a break")]
def test_zip_take_emits_break_in_loop(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_zip_take_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        let n = count_inner_for_loops(body_expr)
        t |> equal(n, 1, "zip+take emits a single for-loop with break-guard")
        let breaks = count_break_continue(body_expr, true)
        t |> success(breaks >= 1, "zip+take must splice a `break` into the for-body")
        let takeCalls = count_call(body_expr, "take")
        t |> equal(takeCalls, 0, "take call must be inlined as counter+break")
    }
}

[test(description = "a zip with a result selector falls back to the unfused path and returns the right values")]
def test_zip_result_selector_bail_correct(t : T?) {
    t |> run("zip(a, b, selector) form bails to tier-2 cascade and computes correct result") @(t : T?) {
        let result <- target_zip_result_selector_bail()
        // [10..50].zip([1..5], p+q) = [11, 22, 33, 44, 55]
        t |> equal(length(result), 5)
        t |> equal(result[0], 11)
        t |> equal(result[4], 55)
    }
}

[test(description = "a zip followed by order falls back to the unfused path and returns the right values")]
def test_zip_order_bail_correct(t : T?) {
    t |> run("zip + select + order bails to tier-2 cascade and produces sorted result") @(t : T?) {
        let result <- target_zip_order_bail()
        // a=[30,10,40,20,50] b=[3,1,4,2,5]: pair sums [33,11,44,22,55] sorted ascending → [11,22,33,44,55]
        t |> equal(length(result), 5)
        t |> equal(result[0], 11)
        t |> equal(result[4], 55)
    }
}

// PR #2741 Copilot review #r3269663361: impure projection + range op must bail so the projection runs on every element (eager pipeline semantics).
[test(description = "a zip with an impure select and a skip falls back to the unfused path with the right result")]
def test_zip_impure_select_skip_bails_to_tier2(t : T?) {
    t |> run("impure select + skip bails to tier-2; projection runs on every element") @(t : T?) {
        g_zip_proj_hits = 0
        let result <- target_zip_impure_select_skip_bails()
        // skip(2) on a length-5 stream → 3 surviving elements [30, 40, 50]
        t |> equal(length(result), 3)
        // Critical: projection must have fired 5 times (once per input), not 3 (only survivors)
        t |> equal(g_zip_proj_hits, 5)
    }
}

// PR #2741 Copilot review #r3269663397: pure `zip+select+count` skips the `let it = (itA, itB)` body bind. Inner for-body should be a single stmt (`acc++`), not 2 (bind + acc++).
[test(description = "zip, a pure select and count emit a counter loop whose body is just the increment, without binding the pair")]
def test_zip_select_count_pure_skips_it_bind(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_zip_select_count_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        let nfor = count_inner_for_loops(body_expr)
        t |> equal(nfor, 1, "pure zip+select+count uses counter for-loop")
        let nbody = count_for_body_stmts(body_expr)
        t |> equal(nbody, 1, "pure zip+select+count counter body is just `acc++` — no `let it = (itA, itB)` bind")
    }
}

[test(description = "a folded zip with a projection then sum returns the total")]
def test_zip_sum_proj_fold_result(t : T?) {
    t |> run("zip+select+sum returns scalar accumulator") @(t : T?) {
        // (10+1)+(20+2)+(30+3)+(40+4)+(50+5) = 11+22+33+44+55 = 165
        t |> equal(target_zip_sum_proj_fold(), 165)
    }
}

[test(description = "a folded zip with a projection then min returns the smallest")]
def test_zip_min_proj_fold_result(t : T?) {
    t |> run("zip+select+min picks smallest projected value") @(t : T?) {
        // diffs: 9,18,27,36,45 → min 9
        t |> equal(target_zip_min_proj_fold(), 9)
    }
}

[test(description = "a folded zip with a projection then max returns the largest")]
def test_zip_max_proj_fold_result(t : T?) {
    t |> run("zip+select+max picks largest projected value") @(t : T?) {
        // products: 10,40,90,160,250 → max 250
        t |> equal(target_zip_max_proj_fold(), 250)
    }
}

[test(description = "a folded zip with a projection then average returns the mean")]
def test_zip_average_proj_fold_result(t : T?) {
    t |> run("zip+select+average returns double") @(t : T?) {
        // (11+22+33+44+55)/5 = 33.0
        t |> equal(target_zip_average_proj_fold(), 33.0lf)
    }
}

[test(description = "a folded zip with where, projection and sum returns the total of the passing pairs")]
def test_zip_where_sum_proj_fold_result(t : T?) {
    t |> run("zip+where+select+sum filters then sums") @(t : T?) {
        // survivors where t._0>20: (30,3)(40,4)(50,5) → 33+44+55 = 132
        t |> equal(target_zip_where_sum_proj_fold(), 132)
    }
}

[test(description = "a folded zip with where then long_count returns the count as int64")]
def test_zip_where_long_count_fold_result(t : T?) {
    t |> run("zip+where+long_count returns int64 survivor count") @(t : T?) {
        // 3 survivors
        t |> equal(target_zip_where_long_count_fold(), 3l)
    }
}

// long_count + chain op (where) must emit a single multi-iter counter for-loop, not cascade to tier-2. Regression guard against routing long_count through the ACCUMULATOR projection-required branch (PR #2742 Copilot review #r3270242609).
[test(description = "zip, where and long_count emit one multi-iterator counter loop with long_count fused")]
def test_zip_where_long_count_emits_counter_loop(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_zip_where_long_count_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        let nfor = count_inner_for_loops(body_expr)
        t |> equal(nfor, 1, "zip+where+long_count must emit one multi-iter counter for-loop")
        let zipCalls = count_call(body_expr, "zip")
        t |> equal(zipCalls, 0, "zip call must be inlined into the splice")
        let longCountCalls = count_call(body_expr, "long_count")
        t |> equal(longCountCalls, 0, "long_count terminator must be fused (no runtime call)")
    }
}

[test(description = "a folded zip then first returns the first pair")]
def test_zip_first_no_proj_fold_result(t : T?) {
    t |> run("zip+first (no projection) returns first tuple") @(t : T?) {
        let result = target_zip_first_no_proj_fold()
        t |> equal(result._0, 10)
        t |> equal(result._1, 1)
    }
}

[test(description = "a folded zip with a projection then first returns the first projection")]
def test_zip_first_proj_fold_result(t : T?) {
    t |> run("zip+select+first returns first projected scalar") @(t : T?) {
        // first pair (10,1) projected → 11
        t |> equal(target_zip_first_proj_fold(), 11)
    }
}

[test(description = "a folded zip with where then first returns the first passing pair")]
def test_zip_where_first_fold_result(t : T?) {
    t |> run("zip+where+first returns first survivor tuple") @(t : T?) {
        // first pair where t._0>20: (30,3)
        let result = target_zip_where_first_fold()
        t |> equal(result._0, 30)
        t |> equal(result._1, 3)
    }
}

[test(description = "a folded zip with a projection then first_or_default returns the first projection")]
def test_zip_first_or_default_proj_fold_result(t : T?) {
    t |> run("zip+select+where+first_or_default returns default when no survivor") @(t : T?) {
        // no projected sum > 1000, so default -1 fires
        t |> equal(target_zip_first_or_default_proj_fold(), -1)
    }
}

[test(description = "a folded zip then any is true for a non-empty zip")]
def test_zip_any_no_pred_fold_result(t : T?) {
    t |> run("zip+any (no pred) on non-empty pair → true") @(t : T?) {
        t |> equal(target_zip_any_no_pred_fold(), true)
    }
}

[test(description = "a folded zip then any is false for an empty zip")]
def test_zip_any_no_pred_empty_fold_result(t : T?) {
    t |> run("zip+any (no pred) on empty source → false") @(t : T?) {
        // emptyA.zip(...) → min(0,3) = 0 iters → any returns false
        t |> equal(target_zip_any_no_pred_empty_fold(), false)
    }
}

[test(description = "a folded zip then any with a predicate finds a passing pair")]
def test_zip_any_pred_fold_result(t : T?) {
    t |> run("zip+any(pred) returns true if any pair matches") @(t : T?) {
        // (40,4) and (50,5) satisfy t._0>30 → true
        t |> equal(target_zip_any_pred_fold(), true)
    }
}

[test(description = "a folded zip then all is true when every pair passes")]
def test_zip_all_pred_true_fold_result(t : T?) {
    t |> run("zip+all(pred) returns true when all match") @(t : T?) {
        t |> equal(target_zip_all_pred_true_fold(), true)
    }
}

[test(description = "a folded zip then all is false when a pair fails")]
def test_zip_all_pred_false_fold_result(t : T?) {
    t |> run("zip+all(pred) returns false when any fails") @(t : T?) {
        // 10, 20, 30 fail t._0>30
        t |> equal(target_zip_all_pred_false_fold(), false)
    }
}

[test(description = "a folded zip with a projection then contains finds a present value")]
def test_zip_contains_proj_fold_result(t : T?) {
    t |> run("zip+select+contains returns true on hit") @(t : T?) {
        // 30+3 = 33 matches
        t |> equal(target_zip_contains_proj_fold(), true)
    }
}

[test(description = "a folded zip with a projection then contains misses an absent value")]
def test_zip_contains_proj_miss_fold_result(t : T?) {
    t |> run("zip+select+contains returns false on miss") @(t : T?) {
        t |> equal(target_zip_contains_proj_miss_fold(), false)
    }
}

// Regression guard for the 2-site average semantics fix (emit_accumulator_lane + emit_decs_accumulator align with linq.das:1545 — double accumulator + cnt==0 guard → 0.0lf on empty).
[test(description = "a folded zip average over an empty zip returns zero")]
def test_zip_average_empty_returns_zero(t : T?) {
    t |> run("zip+select+average on empty source returns 0.0lf") @(t : T?) {
        t |> equal(target_zip_average_empty_fold(), 0.0lf)
    }
}

// AST-shape: zip+accumulator splices into a single for-loop with no surviving runtime `sum`/`zip` calls.
[test(description = "zip, a projection and sum emit one multi-iterator loop with the sum fused")]
def test_zip_sum_proj_emits_inline_accumulator(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_zip_sum_proj_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        let nfor = count_inner_for_loops(body_expr)
        t |> equal(nfor, 1, "zip+select+sum must emit exactly one multi-iter for-loop")
        let zipCalls = count_call(body_expr, "zip")
        t |> equal(zipCalls, 0, "zip call must be inlined into the splice")
        let sumCalls = count_call(body_expr, "sum")
        t |> equal(sumCalls, 0, "sum terminator must be fused (no runtime sum call)")
    }
}

// AST-shape: zip+early-exit emits the same single-for shape with no surviving terminator call.
[test(description = "zip, a projection and first emit one multi-iterator loop with first fused")]
def test_zip_first_proj_emits_early_return(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_zip_first_proj_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        let nfor = count_inner_for_loops(body_expr)
        t |> equal(nfor, 1, "zip+select+first must emit exactly one multi-iter for-loop")
        let zipCalls = count_call(body_expr, "zip")
        t |> equal(zipCalls, 0, "zip call must be inlined")
        let firstCalls = count_call(body_expr, "first")
        t |> equal(firstCalls, 0, "first terminator must be fused (no runtime first call)")
    }
}

[export, marker(no_coverage)]
def target_chained_where_fold() : array<int> =>
    <- [1, 2, 3, 4, 5]._where(_ > 1)._where(_ < 5)._fold()

[export, marker(no_coverage)]
def target_chained_select_fold() : array<int> =>
    <- [1, 2, 3, 4, 5]._select(_ * 2)._select(_ + 1)._fold()

[export, marker(no_coverage)]
def target_where_count_fold() : int => _fold(each([1, 2, 3, 4, 5])._where(_ > 2).count())

[export, marker(no_coverage)]
def target_chained_where_count_fold() : int =>
    _fold(each([1, 2, 3, 4, 5])._where(_ > 1)._where(_ < 5).count())

[export, marker(no_coverage)]
def target_count_fold() : int => _fold(each([1, 2, 3, 4, 5]).count())

[export, marker(no_coverage)]
def target_select_count_fold() : int => _fold(each([1, 2, 3, 4, 5])._select(_ * 2).count())

var g_select_count_proj_hits = 0

def side_effect_select_proj(x : int) : int {
    g_select_count_proj_hits ++
    return x * 2
}

[export, marker(no_coverage)]
def target_select_count_fold_impure() : int =>
    _fold(each([1, 2, 3, 4, 5])._select(side_effect_select_proj(_)).count())

// `each(range(...))` — argument is a `range`, not an array. peel_each must NOT fire
// here; we'd otherwise replace the iterator-yielding each call with the raw range
// and downstream length-shortcut / reserve-by-length would silently misbehave.
[export, marker(no_coverage)]
def target_each_range_count() : int => _fold(each(range(10))._where(_ > 5).count())

[export, marker(no_coverage)]
def target_sum_fold() : int => _fold(each([1, 2, 3, 4, 5]).sum())

[export, marker(no_coverage)]
def target_min_workhorse_fold() : int => _fold(each([5, 3, 8, 1, 4]).min())

[export, marker(no_coverage)]
def target_max_workhorse_fold() : int => _fold(each([5, 3, 8, 1, 4]).max())

[export, marker(no_coverage)]
def target_min_non_workhorse_fold() : tuple<int; int> {
    // tuple<int;int> is NOT workhorse — min must dispatch to `_::less` for the compare.
    // Single-return body so qmatch_function can extract the expression directly.
    return _fold(each([(1, 20), (3, 5), (2, 10)]).min())
}

[export, marker(no_coverage)]
def target_average_fold() : double => _fold(each([2, 4, 6, 8]).average())

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
def target_first_fold() : int => _fold(each([1, 2, 3, 4, 5])._where(_ > 2).first())

[export, marker(no_coverage)]
def target_first_or_default_fold() : int =>
    _fold(each([1, 2, 3, 4, 5])._where(_ > 99).first_or_default(-1))

[export, marker(no_coverage)]
def target_any_loop_fold() : bool => _fold(each([1, 2, 3, 4, 5])._where(_ > 3).any())

[export, marker(no_coverage)]
def target_any_shortcut_fold() : bool {
    // No upstream where + no per-element work + length-bearing source → `length(src) > 0`.
    return _fold(each([1, 2, 3, 4, 5]).any())
}

[export, marker(no_coverage)]
def target_all_fold() : bool => _fold(each([1, 2, 3, 4, 5])._all(_ > 0))

[export, marker(no_coverage)]
def target_contains_fold() : bool => _fold(each([1, 2, 3, 4, 5]).contains(3))

[test(description = "a folded where emits a loop rather than a comprehension")]
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
        let maybe_comp <- qm_resolve_comprehension(body_clone)
        t |> success(maybe_comp == null, "loop planner must NOT emit a comprehension")
    }
}

[test(description = "a folded select emits a loop rather than a comprehension")]
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
        let maybe_comp <- qm_resolve_comprehension(body_clone)
        t |> success(maybe_comp == null, "loop planner must NOT emit a comprehension")
    }
}

[test(description = "chained wheres fuse into a single invoke loop")]
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

[test(description = "chained selects fuse into a single invoke loop")]
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

[test(description = "where then count fuses into a counter invoke")]
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

[test(description = "chained wheres then count fuse into a single counter invoke")]
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

[test(description = "a bare count fuses into an unconditional counter invoke")]
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

[test(description = "chained wheres give the same elements as the manual filter")]
def test_chained_where_fold_result(t : T?) {
    t |> run("chained where _fold produces correct values") @(t : T?) {
        let result <- target_chained_where_fold()
        t |> equal(length(result), 3)
        t |> equal(result[0], 2)
        t |> equal(result[1], 3)
        t |> equal(result[2], 4)
    }
}

[test(description = "chained selects give the composed projection")]
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

[test(description = "where then count returns the number of matches")]
def test_where_count_fold_result(t : T?) {
    t |> run("where _count _fold produces correct count") @(t : T?) {
        t |> equal(target_where_count_fold(), 3)
    }
}

[test(description = "chained wheres then count return the number of elements passing both")]
def test_chained_where_count_fold_result(t : T?) {
    t |> run("chained where _count _fold produces correct count") @(t : T?) {
        t |> equal(target_chained_where_count_fold(), 3)
    }
}

[test(description = "a bare folded count returns the element count")]
def test_count_fold_result(t : T?) {
    t |> run("bare _count _fold produces source length") @(t : T?) {
        t |> equal(target_count_fold(), 5)
    }
}

[test(description = "select then count returns the element count")]
def test_select_count_fold_result(t : T?) {
    t |> run("select _count _fold produces correct count (projection does not affect count value)") @(t : T?) {
        t |> equal(target_select_count_fold(), 5)
    }
}

// Resolves the invoke wrapper's *effective* outer block. When the body uses
// `var inscope` (smart_ptr / table lifetimes), the typer wraps the whole body
// in an inner ExprBlock so its finally clause is scoped correctly. This helper
// returns the inner block in that case so for/let counting works the same way
// regardless of whether the splice used `var inscope`. Returns null if the body
// isn't the canonical invoke($block, $src) shape.
def invoke_outer_list(body_expr : Expression?) : ExprBlock? {
    if (!(body_expr is ExprInvoke)) return null
    let inv = body_expr as ExprInvoke
    if (empty(inv.arguments) || !(inv.arguments[0] is ExprMakeBlock)) return null
    let mb = inv.arguments[0] as ExprMakeBlock
    var outer = mb._block as ExprBlock
    if (outer == null) return null
    // Typer-inserted single-nested-block for `var inscope` finally scoping: unwrap once.
    if (length(outer.list) == 1 && outer.list[0] is ExprBlock) {
        outer = outer.list[0] as ExprBlock
    }
    return outer
}

// Returns the number of ExprLet/ExprFor statements in the counter-lane invoke's
// inner block. Pure shortcut: `[var src, return length(src)]` → 0 for-loops.
// Impure loop: `[var src, var acc=0, for {...}, return acc]` → 1 for-loop.
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

// Returns the number of stmts in the for-body, or -1 if no for loop exists.
def count_for_body_stmts(body_expr : Expression?) : int {
    let outer = invoke_outer_list(body_expr)
    if (outer == null) return -1
    for (stmt in outer.list) {
        if (stmt is ExprFor) {
            let fe = stmt as ExprFor
            let fbody = fe.body as ExprBlock
            if (fbody == null) return -1
            if (length(fbody.list) == 1 && fbody.list[0] is ExprBlock) {
                let inner = fbody.list[0] as ExprBlock
                if (inner.genFlags.generated) return length(inner.list)
            }
            return length(fbody.list)
        }
    }
    return -1
}

[test(description = "a pure select then count folds to a length shortcut with no for loop")]
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

[test(description = "a bare count on an array folds to a length shortcut with no for loop")]
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

[test(description = "an impure select then count keeps the projection bind in the counter loop body")]
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

[test(description = "each over an array is peeled so the folded call sees the array itself")]
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

[test(description = "each over an array under a bare count is peeled")]
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
[test(description = "each over a range is not peeled because the source is not an array")]
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

[test(description = "the folded count over each of a range returns the right count")]
def test_target_each_range_count_runs(t : T?) {
    // Behavioral: ensure the iterator-source chain still compiles and produces the
    // expected count. range(10) → [0,1,2,3,4,5,6,7,8,9]; filter > 5 → 4 elements.
    t |> equal(target_each_range_count(), 4)
}

def count_op2(expr : Expression?; op : string) : int {   // nolint:STYLE037 — exact-type `is` ladder, one arm per AST node kind
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

def count_call(expr : Expression?; funcName : string) : int {   // nolint:STYLE037 — exact-type `is` ladder, one arm per AST node kind
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

def count_op1(expr : Expression?; op : string) : int {   // nolint:STYLE037 — exact-type `is` ladder, one arm per AST node kind
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

// Counts top-level `var` declarations in the outer block of an invoke wrapper.
// sum/long_count emit one accumulator; min/max emit two (first flag + best); average
// emits two (sum acc + count).
def count_outer_let_vars(body_expr : Expression?) : int {
    let outer = invoke_outer_list(body_expr)
    if (outer == null) return -1
    var n = 0
    for (stmt in outer.list) {
        if (stmt is ExprLet) n ++
    }
    return n
}

[test(description = "a folded sum emits one accumulator variable and an add per element")]
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

[test(description = "a folded min on a workhorse type compares with the less-than operator and a first-iteration flag, not a less call")]
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

[test(description = "a folded max on a workhorse type compares with the greater-than operator and a first-iteration flag, not a less call")]
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

[test(description = "a folded min on a non-workhorse type compares through the less call")]
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

[test(description = "a folded average emits a sum accumulator and a count accumulator")]
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

[test(description = "a folded long_count emits one int64 accumulator incremented per matching element")]
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

[test(description = "a folded long_count on an array folds to a length shortcut with no for loop")]
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

[test(description = "a folded first emits a loop with a panic in the tail for the empty case")]
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

[test(description = "a folded first_or_default emits no panic and returns the default")]
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

[test(description = "a folded any with a where emits one for loop")]
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

[test(description = "a folded any without a predicate emits only a not-empty test with no for loop")]
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

[test(description = "a folded all emits one for loop")]
def test_all_loop_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_all_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper")
        // all emits `if (!pred) return false`; the optimizer's compare flip folds
        // `!(_ > 0)` into `_ <= 0`, so accept either shape.
        t |> success(count_op1(body_expr, "!") >= 1 || count_op2(body_expr, "<=") >= 1,
            "all: negated predicate present (as `!` or as the flipped compare)")
        t |> equal(1, count_inner_for_loops(body_expr), "all: one for-loop")
    }
}

[test(description = "a folded contains emits one for loop comparing each element")]
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
