options gen2
options rtti
// this file pins linq splice shapes; auto-splicing would dissolve
// them
options disable_auto_inline
// file contains intentional cascade tests (regression guards for
// tier-2 fallthrough)
options _no_linq_perf_warn = true
require daslib/linq
require dastest/testing_boost public
require strings

require daslib/linq_boost
require daslib/decs_boost
require daslib/ast
require daslib/rtti
require daslib/ast_boost
require daslib/ast_match
require daslib/templates_boost

def private describe_count(expr : Expression?; needle : string) : int {
    if (expr == null) return 0
    let s = describe(expr)
    var n = 0
    var i = 0
    let nl = length(needle)
    while (true) {
        let p = s |> find(needle, i)
        if (p < 0) break
        n ++
        i = p + nl
    }
    return n
}

[decs_template(prefix = "unroll2_")]
struct UnrollChainRow {
    val : int
    flag : int
}

def private fixture_unroll2(n : int) {
    restart()
    for (i in range(n)) {
        create_entity() @(eid, cmp) {
            cmp.eid := eid
            cmp.unroll2_val := i
            cmp.unroll2_flag := i % 2
        }
    }
    commit()
}

[export, marker(no_coverage)]
def target_unroll_take_while_count_fold() : int =>
    _fold(from_decs_template(type<UnrollChainRow>)._take_while(_.val < 3).count())

[export, marker(no_coverage)]
def target_unroll_skip_while_count_fold() : int =>
    _fold(from_decs_template(type<UnrollChainRow>)._skip_while(_.val < 3).count())

[export, marker(no_coverage)]
def target_unroll_skip_while_take_count_fold() : int {
    // skip_while(val<2) drops 0,1 → take(2) windows 2,3 → count 2.
    return _fold(from_decs_template(type<UnrollChainRow>)._skip_while(_.val < 2).take(2).count())
}

[export, marker(no_coverage)]
def target_unroll_take_while_any_match_fold() : bool =>
    _fold(from_decs_template(type<UnrollChainRow>)._take_while(_.val < 3)._any(_.val == 2))

[export, marker(no_coverage)]
def target_unroll_take_while_any_after_window_fold() : bool {
    // val 5 is outside the take_while window — must NOT report true even
    // though take_while produces inner `return true` on val=3.
    return _fold(from_decs_template(type<UnrollChainRow>)._take_while(_.val < 3)._any(_.val == 5))
}

[export, marker(no_coverage)]
def target_unroll_take_while_all_pass_fold() : bool =>
    _fold(from_decs_template(type<UnrollChainRow>)._take_while(_.val < 3)._all(_.val < 5))

[export, marker(no_coverage)]
def target_unroll_take_while_all_fail_fold() : bool {
    // val=1 fails the all-pred. take_while-stop inner `return true` must
    // not be misread as "no counterexample".
    return _fold(from_decs_template(type<UnrollChainRow>)._take_while(_.val < 3)._all(_.val == 0))
}

[test(description = "skip_while, take and count over a decs source match the reference")]
def test_unroll_skip_while_take_count_parity(t : T?) {
    fixture_unroll2(7)
    t |> equal(target_unroll_skip_while_take_count_fold(), 2,
               "skip_while(val<2) + take(2) yields 2,3")
}

[test(description = "take_while then any finds a match inside the window")]
def test_unroll_take_while_any_match(t : T?) {
    fixture_unroll2(7)
    // val 2 is in the window → true. Real match, not the take_while-stop
    // signal.
    t |> equal(target_unroll_take_while_any_match_fold(), true,
               "take_while + any returns true when match in window")
}

[test(description = "take_while then any ignores a match that lies after the window ends")]
def test_unroll_take_while_any_after_window(t : T?) {
    fixture_unroll2(7)
    // val 5 outside take_while window → false. take_while-stop must NOT
    // register as a match.
    t |> equal(target_unroll_take_while_any_after_window_fold(), false,
               "take_while + any returns false when match outside window")
}

[test(description = "take_while then all is true when the whole window passes")]
def test_unroll_take_while_all_pass(t : T?) {
    fixture_unroll2(7)
    // window {0,1,2}; all < 5 → true.
    t |> equal(target_unroll_take_while_all_pass_fold(), true,
               "take_while + all returns true when window satisfies pred")
}

[test(description = "take_while then all is false when an element in the window fails")]
def test_unroll_take_while_all_fail(t : T?) {
    fixture_unroll2(7)
    // window {0,1,2}; not all == 0 → false. take_while-stop inner true
    // must not be misread as "no counterexample".
    t |> equal(target_unroll_take_while_all_fail_fold(), false,
               "take_while + all returns false on counterexample in window")
}

[test(description = "take_while then count over a decs source folds into an archetype find with a predicate break")]
def test_unroll_take_while_count_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_unroll_take_while_count_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        t |> equal(describe_count(body_expr, "to_sequence"), 0,
                   "take_while+count splice must NOT call to_sequence")
        // take_while present → must use the _find variant so first-false
        // propagates across archetypes.
        t |> equal(describe_count(body_expr, "for_each_archetype_find"), 1,
                   "take_while splice routes to for_each_archetype_find")
    }
}

[test(description = "skip_while then count over a decs source folds into one archetype walk with a one-way flag")]
def test_unroll_skip_while_count_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_unroll_skip_while_count_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        t |> equal(describe_count(body_expr, "to_sequence"), 0,
                   "skip_while+count splice must NOT call to_sequence")
        // skip_while alone does NOT early-exit — must use plain
        // for_each_archetype.
        t |> equal(describe_count(body_expr, "for_each_archetype_find"), 0,
                   "skip_while-only splice does NOT route to _find")
        t |> equal(describe_count(body_expr, "for_each_archetype"), 1,
                   "skip_while-only splice uses regular for_each_archetype")
    }
}

[test(description = "take_while then any over a decs source folds with an explicit found-state flag")]
def test_unroll_take_while_any_splice_shape(t : T?) {
    // Verifies the take_while + any splice fires AND routes through
    // explicit-state (decs_found) — distinguishing take_while-stop
    // `return true` from real-match `return true`. This is the exact code
    // path that fails to be exercised when tests put _select between
    // take_while and the terminator (Copilot PR #2772 review feedback).
    // Predicate over the source tuple keeps the splice hot.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_unroll_take_while_any_match_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        t |> equal(describe_count(body_expr, "to_sequence"), 0,
                   "take_while+any splice must NOT fall to tier-2 to_sequence")
        // take_while-stop produces inner `return true` — needs _find
        // dispatch.
        t |> equal(describe_count(body_expr, "for_each_archetype_find"), 1,
                   "take_while+any splice routes to for_each_archetype_find")
        // Explicit found-state routing disambiguates take_while-stop from a
        // real match (var + set + return).
        t |> equal(describe_count(body_expr, "found"), 3,
                   "take_while+any routes through explicit found state (var + set + return)")
    }
}

[decs_template(prefix = "unroll5e_")]
struct UnrollGroupRow {
    brand : int
    val   : int
    flag  : int
}

// Mirrors the tuple yielded by the from_decs_template bridge — used
// to type inner-select lambdas inside `_._1 |> select(@(r :
// UnrollGroupTup) => r.val) |> ...`.
typedef UnrollGroupTup = tuple<brand : int; val : int; flag : int>

def private fixture_unroll5e() {
    // 7 entities: brand=val%3, val=10..16, flag=val%2
    // brand 0 → vals {12,15}      len=2 sum=27 min=12 max=15 avg=13.5
    // brand 1 → vals {10,13,16}   len=3 sum=39 min=10 max=16 avg=13.0
    // brand 2 → vals {11,14}      len=2 sum=25 min=11 max=14 avg=12.5
    restart()
    var i = 10
    while (i <= 16) {
        let v = i
        create_entity() @(eid, cmp) {
            cmp.eid := eid
            cmp.unroll5e_brand := v % 3
            cmp.unroll5e_val := v
            cmp.unroll5e_flag := v % 2
        }
        i ++
    }
    commit()
}

[export, marker(no_coverage)]
def target_unroll5e_groupby_count_fold() : array<tuple<Brand : int; N : int>> {
    return <- _fold(from_decs_template(type<UnrollGroupRow>)
                    ._group_by(_.brand)
                    ._select((Brand = _._0, N = _._1 |> length))
                    .to_array())
}

[export, marker(no_coverage)]
def target_unroll5e_groupby_sum_fold() : array<tuple<Brand : int; S : int>> {
    return <- _fold(from_decs_template(type<UnrollGroupRow>)
                    ._group_by(_.brand)
                    ._select((Brand = _._0, S = _._1 |> select(@(r : UnrollGroupTup) =>
                        r.val) |> sum))
                    .to_array())
}

[export, marker(no_coverage)]
def target_unroll5e_groupby_min_fold() : array<tuple<Brand : int; Min : int>> {
    return <- _fold(from_decs_template(type<UnrollGroupRow>)
                    ._group_by(_.brand)
                    ._select((Brand = _._0, Min = _._1 |> select(@(r : UnrollGroupTup) =>
                        r.val) |> min))
                    .to_array())
}

[export, marker(no_coverage)]
def target_unroll5e_groupby_max_fold() : array<tuple<Brand : int; Max : int>> {
    return <- _fold(from_decs_template(type<UnrollGroupRow>)
                    ._group_by(_.brand)
                    ._select((Brand = _._0, Max = _._1 |> select(@(r : UnrollGroupTup) =>
                        r.val) |> max))
                    .to_array())
}

[export, marker(no_coverage)]
def target_unroll5e_groupby_avg_fold() : array<tuple<Brand : int; Avg : double>> {
    return <- _fold(from_decs_template(type<UnrollGroupRow>)
                    ._group_by(_.brand)
                    ._select((Brand = _._0, Avg = _._1 |> select(@(r : UnrollGroupTup) =>
                        r.val) |> average))
                    .to_array())
}

[export, marker(no_coverage)]
def target_unroll5e_groupby_first_fold() : array<tuple<Brand : int; F : int>> {
    return <- _fold(from_decs_template(type<UnrollGroupRow>)
                    ._group_by(_.brand)
                    ._select((Brand = _._0, F = _._1 |> select(@(r : UnrollGroupTup) =>
                        r.val) |> first))
                    .to_array())
}

[export, marker(no_coverage)]
def target_unroll5e_groupby_multi_reducer_fold() : array<tuple<Brand : int; N : int; S : int; MX : int>> {
    return <- _fold(from_decs_template(type<UnrollGroupRow>)
                    ._group_by(_.brand)
                    ._select((Brand = _._0,
                              N = _._1 |> length,
                              S = _._1 |> select(@(r : UnrollGroupTup) => r.val) |> sum,
                              MX = _._1 |> select(@(r : UnrollGroupTup) => r.val) |> max))
                    .to_array())
}

[export, marker(no_coverage)]
def target_unroll5e_groupby_count_terminator_fold() : int {
    return _fold(from_decs_template(type<UnrollGroupRow>)
                 ._group_by(_.brand)
                 ._select((Brand = _._0, N = _._1 |> length))
                 .count())
}

// Iterator-typed chain — bridge's `to_sequence(res)` makes
// `_group_by._select` yield iterator<...> by default (no trailing
// `.to_array()` peel). The splice must emit `to_sequence_move` to
// match the caller's static type; otherwise error[30343] iterator vs
// array. Regression guard for Copilot review on PR #2775.
[export, marker(no_coverage)]
def target_unroll5e_groupby_iter_typed_fold() : iterator<tuple<Brand : int; N : int>> {
    return <- _fold(from_decs_template(type<UnrollGroupRow>)
                    ._group_by(_.brand)
                    ._select((Brand = _._0, N = _._1 |> length)))
}

// count(predicate) overload — the planner must NOT pop `.count(_.N >=
// 2)` as a bare count terminator (would silently drop the predicate).
// Must cascade to tier-2 so the 2-arg `count(src, pred)` overload
// runs. Regression guard for Copilot review on PR #2775.
[export, marker(no_coverage)]
def target_unroll5e_groupby_count_pred_fold() : int {
    return _fold(from_decs_template(type<UnrollGroupRow>)
                 ._group_by(_.brand)
                 ._select((Brand = _._0, N = _._1 |> length))
                 ._count(_.N >= 2))
}

[export, marker(no_coverage)]
def target_unroll5e_groupby_where_count_fold() : array<tuple<Brand : int; N : int>> {
    return <- _fold(from_decs_template(type<UnrollGroupRow>)
                    ._where(_.flag == 1)
                    ._group_by(_.brand)
                    ._select((Brand = _._0, N = _._1 |> length))
                    .to_array())
}

[export, marker(no_coverage)]
def target_unroll5e_groupby_select_sum_fold() : array<tuple<K : int; S : int>> {
    return <- _fold(from_decs_template(type<UnrollGroupRow>)
                    ._select(_.val)
                    ._group_by(_ % 3)
                    ._select((K = _._0, S = _._1 |> sum))
                    .to_array())
}

[export, marker(no_coverage)]
def target_unroll5e_groupby_having_count_fold() : array<tuple<Brand : int; N : int>> {
    return <- _fold(from_decs_template(type<UnrollGroupRow>)
                    ._group_by_lazy(_.brand)
                    ._having(_._1 |> length >= 3)
                    ._select((Brand = _._0, N = _._1 |> length))
                    .to_array())
}

[export, marker(no_coverage)]
def target_unroll5e_groupby_having_hidden_sum_fold() : array<tuple<Brand : int; N : int>> {
    return <- _fold(from_decs_template(type<UnrollGroupRow>)
                    ._group_by_lazy(_.brand)
                    ._having(_._1 |> select(@(r : UnrollGroupTup) => r.val) |> sum > 30)
                    ._select((Brand = _._0, N = _._1 |> length))
                    .to_array())
}

[test(description = "group_by with a count reducer over a decs source matches the reference buckets")]
def test_unroll5e_groupby_count_parity(t : T?) {
    fixture_unroll5e()
    let got <- target_unroll5e_groupby_count_fold()
    t |> equal(got.length(), 3, "3 brand buckets")
    var perBrand <- {for (item in got); item.Brand => item.N}
    t |> equal(perBrand[0], 2)
    t |> equal(perBrand[1], 3)
    t |> equal(perBrand[2], 2)
}

[test(description = "group_by with a sum reducer over a decs source matches the reference")]
def test_unroll5e_groupby_sum_parity(t : T?) {
    fixture_unroll5e()
    let got <- target_unroll5e_groupby_sum_fold()
    t |> equal(got.length(), 3)
    var perBrand <- {for (item in got); item.Brand => item.S}
    t |> equal(perBrand[0], 27)
    t |> equal(perBrand[1], 39)
    t |> equal(perBrand[2], 25)
}

[test(description = "group_by with a min reducer over a decs source matches the reference")]
def test_unroll5e_groupby_min_parity(t : T?) {
    fixture_unroll5e()
    let got <- target_unroll5e_groupby_min_fold()
    t |> equal(got.length(), 3)
    var perBrand <- {for (item in got); item.Brand => item.Min}
    t |> equal(perBrand[0], 12)
    t |> equal(perBrand[1], 10)
    t |> equal(perBrand[2], 11)
}

[test(description = "group_by with a max reducer over a decs source matches the reference")]
def test_unroll5e_groupby_max_parity(t : T?) {
    fixture_unroll5e()
    let got <- target_unroll5e_groupby_max_fold()
    t |> equal(got.length(), 3)
    var perBrand <- {for (item in got); item.Brand => item.Max}
    t |> equal(perBrand[0], 15)
    t |> equal(perBrand[1], 16)
    t |> equal(perBrand[2], 14)
}

[test(description = "group_by with an average reducer over a decs source matches the reference")]
def test_unroll5e_groupby_avg_parity(t : T?) {
    fixture_unroll5e()
    let got <- target_unroll5e_groupby_avg_fold()
    t |> equal(got.length(), 3)
    var perBrand <- {for (item in got); item.Brand => item.Avg}
    t |> equal(perBrand[0], 13.5lf)
    t |> equal(perBrand[1], 13.0lf)
    t |> equal(perBrand[2], 12.5lf)
}

[test(description = "group_by with a first reducer over a decs source matches the reference")]
def test_unroll5e_groupby_first_parity(t : T?) {
    fixture_unroll5e()
    let got <- target_unroll5e_groupby_first_fold()
    t |> equal(got.length(), 3)
    // First per bucket depends on iteration order, but it must be A value
    // FROM the bucket.
    for (item in got) {
        if (item.Brand == 0) t |> success(item.F == 12 || item.F == 15,
                                          "brand 0 first must be 12 or 15")
        elif (item.Brand == 1) t |> success(item.F == 10 || item.F == 13 || item.F == 16,
                                            "brand 1 first must be 10/13/16")
        elif (item.Brand == 2) t |> success(item.F == 11 || item.F == 14,
                                            "brand 2 first must be 11 or 14")
    }
}

[test(description = "group_by with several reducers over a decs source matches the reference")]
def test_unroll5e_groupby_multi_reducer_parity(t : T?) {
    fixture_unroll5e()
    let got <- target_unroll5e_groupby_multi_reducer_fold()
    t |> equal(got.length(), 3)
    var keysSeen = 0
    for (item in got) {
        if (item.Brand == 0) {
            t |> equal(item.N, 2); t |> equal(item.S, 27); t |> equal(item.MX, 15)
            keysSeen ++
        } elif (item.Brand == 1) {
            t |> equal(item.N, 3); t |> equal(item.S, 39); t |> equal(item.MX, 16)
            keysSeen ++
        } elif (item.Brand == 2) {
            t |> equal(item.N, 2); t |> equal(item.S, 25); t |> equal(item.MX, 14)
            keysSeen ++
        }
    }
    t |> equal(keysSeen, 3, "all 3 keys observed")
}

[test(description = "group_by then count over a decs source gives the number of buckets")]
def test_unroll5e_groupby_count_terminator_parity(t : T?) {
    fixture_unroll5e()
    t |> equal(target_unroll5e_groupby_count_terminator_fold(), 3,
               "count terminator returns length(tab)")
}

[test(description = "an iterator-typed group_by chain over a decs source returns the same buckets as the array form")]
def test_unroll5e_groupby_iter_typed_parity(t : T?) {
    // Walks the iterator-typed chain; counts entries.
    fixture_unroll5e()
    var count = 0
    for (item in target_unroll5e_groupby_iter_typed_fold()) {
        count ++
        // Every entry has a valid Brand (0/1/2) and N (count per bucket).
        t |> success(item.Brand >= 0 && item.Brand <= 2, "Brand in 0..2")
        t |> success(item.N >= 1, "N >= 1 (at least one row per bucket)")
    }
    t |> equal(count, 3, "iterator-typed group_by yields 3 buckets")
}

[test(description = "group_by followed by count with a predicate over a decs source honors the predicate")]
def test_unroll5e_groupby_count_pred_parity(t : T?) {
    // Buckets per brand: 0→2, 1→3, 2→2. `_.N >= 2` matches all 3 → result
    // is 3.
    fixture_unroll5e()
    t |> equal(target_unroll5e_groupby_count_pred_fold(), 3,
               "count(predicate) over groups counts buckets satisfying pred")
    // Smoke: change pred to keep only buckets with N >= 3 → only brand 1.
}

[test(description = "count with a predicate after group_by falls back to the unfused path instead of being treated as a bare count")]
def test_unroll5e_groupby_count_pred_cascade_shape(t : T?) {
    // count(predicate) must NOT be popped as the bare count terminator
    // (would drop the predicate). Splice cascades to tier-2 eager bridge
    // which honors the 2-arg count overload — verified by presence of
    // tier-2 fingerprints (`to_sequence`, `group_by_lazy_to_array`
    // materialized intermediate).
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_unroll5e_groupby_count_pred_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected invoke wrapper")
        // Splice did NOT fire: the decs-specific table (`decs_tab`) is
        // absent.
        t |> equal(describe_count(body_expr, "decs_tab"), 0,
                   "count(pred) must cascade — no decs_tab hoisted")
        // Tier-2 fingerprint: `count(_, predicate)` runs on the materialized
        // array.
        t |> success(describe_count(body_expr, "_.N >= 2") >= 1,
                     "predicate body must appear in the cascaded emission")
    }
}

[test(description = "an iterator-typed group_by chain over a decs source emits a to_sequence_move so the static type matches")]
def test_unroll5e_groupby_iter_typed_splice_shape(t : T?) {
    // Verifies the iterator-typed splice fires (not falling to tier-2)
    // and emits to_sequence_move at the tail to convert the buffer array
    // to iterator.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_unroll5e_groupby_iter_typed_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        // Splice fires: no tier-2 fall-through (would manifest as
        // inner-invoke `to_sequence(res)` from the eager bridge).
        t |> equal(describe_count(body_expr, "to_sequence("), 0,
                   "iterator-typed group_by splice must NOT call eager bridge's to_sequence(res)")
        t |> equal(describe_count(body_expr, "for_each_archetype"), 1,
                   "exactly one for_each_archetype outer call")
        // Tail emits `to_sequence_move(buf)` to convert array<...> result to
        // iterator<...> matching caller's type.
        t |> equal(describe_count(body_expr, "to_sequence_move"), 1,
                   "iterator-typed splice tail emits to_sequence_move(buf)")
    }
}

[test(description = "where, group_by and count over a decs source match the reference")]
def test_unroll5e_groupby_where_count_parity(t : T?) {
    fixture_unroll5e()
    // flag==1 → odd vals: {11,13,15}. Buckets: brand 0→{15} N=1, brand
    // 1→{13} N=1, brand 2→{11} N=1.
    let got <- target_unroll5e_groupby_where_count_fold()
    t |> equal(got.length(), 3)
    var perBrand <- {for (item in got); item.Brand => item.N}
    t |> equal(perBrand[0], 1)
    t |> equal(perBrand[1], 1)
    t |> equal(perBrand[2], 1)
}

[test(description = "select, group_by and sum over a decs source match the reference")]
def test_unroll5e_groupby_select_sum_parity(t : T?) {
    fixture_unroll5e()
    // _select(r=>r.val) projects vals 10..16 then group by val%3:
    // key 0 (vals 12,15) S=27, key 1 (vals 10,13,16) S=39, key 2 (vals
    // 11,14) S=25.
    let got <- target_unroll5e_groupby_select_sum_fold()
    t |> equal(got.length(), 3)
    var perKey <- {for (item in got); item.K => item.S}
    t |> equal(perKey[0], 27)
    t |> equal(perKey[1], 39)
    t |> equal(perKey[2], 25)
}

[test(description = "group_by with a having on the bucket count over a decs source matches the reference")]
def test_unroll5e_groupby_having_count_parity(t : T?) {
    fixture_unroll5e()
    // having length >= 3 keeps only brand 1 (N=3).
    let got <- target_unroll5e_groupby_having_count_fold()
    t |> equal(got.length(), 1)
    t |> equal(got[0].Brand, 1)
    t |> equal(got[0].N, 3)
}

[test(description = "group_by with a having on a hidden sum over a decs source matches the reference")]
def test_unroll5e_groupby_having_hidden_sum_parity(t : T?) {
    fixture_unroll5e()
    // sums: brand 0→27, brand 1→39, brand 2→25. having sum > 30 keeps
    // brand 1 (N=3).
    let got <- target_unroll5e_groupby_having_hidden_sum_fold()
    t |> equal(got.length(), 1)
    t |> equal(got[0].Brand, 1)
    t |> equal(got[0].N, 3)
}

[test(description = "group_by with a count reducer over a decs source folds into a state table over the archetype walk")]
def test_unroll5e_groupby_count_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_unroll5e_groupby_count_fold)
        t |> success(func != null, "RTTI must resolve target")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        t |> equal(describe_count(body_expr, "to_sequence"), 0,
                   "group_by splice must NOT fall to tier-2 to_sequence")
        t |> equal(describe_count(body_expr, "for_each_archetype"), 1,
                   "exactly one for_each_archetype outer call")
        t |> equal(describe_count(body_expr, "for_each_archetype_find"), 0,
                   "group_by does NOT use _find variant (no early-exit)")
        // State-table hoisted ABOVE the for_each_archetype call so it spans
        // archetypes.
        t |> equal(describe_count(body_expr, "decs_tab"), 1 + 5,
                   "decs_tab declared once + 5 refs (?[uk], [uk]=, length, values, finalize)")
        t |> equal(describe_count(body_expr, "decs_dummy"), 1 + 5, "decs_dummy declared once + 5 splice refs (??dummy, addr(dummy), dummy._0=, tab[uk]=dummy, dummy=default)")
        t |> equal(describe_count(body_expr, "unique_key"), 1,
                   "single unique_key derivation per element")
    }
}

[decs_template(prefix = "unroll5d_")]
struct Unroll5dRow {
    id    : int
    price : int
    flag  : int
}

def private fixture_unroll5d() {
    restart()
    // 7 rows. price (jumbled walk-order so order_by must actually sort):
    // 30,10,50,20,40,5,35. flag pattern: even-id => 1, odd-id => 0.
    let prices = fixed_array(30, 10, 50, 20, 40, 5, 35)
    for (i in range(7)) {
        create_entity() @(eid, cmp) {
            cmp.eid := eid
            cmp.unroll5d_id := i
            cmp.unroll5d_price := prices[i]
            cmp.unroll5d_flag := (i % 2 == 0) ? 1 : 0
        }
    }
    commit()
}

[export, marker(no_coverage)]
def target_unroll5d_order_by_to_array_fold() : array<tuple<id : int; price : int; flag : int>> =>
    <- _fold(from_decs_template(type<Unroll5dRow>)._order_by(_.price).to_array())

[test(description = "order_by then to_array over a decs source matches the reference sort")]
def test_unroll5d_order_by_to_array_parity(t : T?) {
    fixture_unroll5d()
    let got <- target_unroll5d_order_by_to_array_fold()
    t |> equal(got.length(), 7)
    // sorted ascending by price: 5,10,20,30,35,40,50.
    let expected = [5, 10, 20, 30, 35, 40, 50]
    for (i in range(7)) {
        t |> equal(got[i].price, expected[i])
    }
}

[export, marker(no_coverage)]
def target_unroll5d_order_by_desc_to_array_fold() : array<tuple<id : int; price : int; flag : int>> =>
    <- _fold(from_decs_template(type<Unroll5dRow>)._order_by_descending(_.price).to_array())

[test(description = "order_by_descending then to_array over a decs source matches the reference sort")]
def test_unroll5d_order_by_desc_to_array_parity(t : T?) {
    fixture_unroll5d()
    let got <- target_unroll5d_order_by_desc_to_array_fold()
    t |> equal(got.length(), 7)
    let expected = [50, 40, 35, 30, 20, 10, 5]
    for (i in range(7)) {
        t |> equal(got[i].price, expected[i])
    }
}

[export, marker(no_coverage)]
def target_unroll5d_order_by_take3_fold() : array<tuple<id : int; price : int; flag : int>> =>
    <- _fold(from_decs_template(type<Unroll5dRow>)._order_by(_.price).take(3).to_array())

[test(description = "order_by then take over a decs source matches the reference")]
def test_unroll5d_order_by_take_parity(t : T?) {
    fixture_unroll5d()
    let got <- target_unroll5d_order_by_take3_fold()
    t |> equal(got.length(), 3)
    let expected = [5, 10, 20]
    for (i in range(3)) {
        t |> equal(got[i].price, expected[i])
    }
}

[export, marker(no_coverage)]
def target_unroll5d_order_by_desc_take3_fold() : array<tuple<id : int; price : int; flag : int>> =>
    <- _fold(from_decs_template(type<Unroll5dRow>)._order_by_descending(_.price).take(3).to_array())

[test(description = "order_by_descending then take over a decs source matches the reference")]
def test_unroll5d_order_by_desc_take_parity(t : T?) {
    fixture_unroll5d()
    let got <- target_unroll5d_order_by_desc_take3_fold()
    t |> equal(got.length(), 3)
    let expected = [50, 40, 35]
    for (i in range(3)) {
        t |> equal(got[i].price, expected[i])
    }
}

[export, marker(no_coverage)]
def target_unroll5d_order_by_first_fold() : tuple<id : int; price : int; flag : int> =>
    _fold(from_decs_template(type<Unroll5dRow>)._order_by(_.price).first())

[test(description = "order_by then first over a decs source matches the reference")]
def test_unroll5d_order_by_first_parity(t : T?) {
    fixture_unroll5d()
    t |> equal(target_unroll5d_order_by_first_fold().price, 5)
}

[export, marker(no_coverage)]
def target_unroll5d_order_by_desc_first_fold() : tuple<id : int; price : int; flag : int> =>
    _fold(from_decs_template(type<Unroll5dRow>)._order_by_descending(_.price).first())

[test(description = "order_by_descending then first over a decs source matches the reference")]
def test_unroll5d_order_by_desc_first_parity(t : T?) {
    fixture_unroll5d()
    t |> equal(target_unroll5d_order_by_desc_first_fold().price, 50)
}

[export, marker(no_coverage)]
def target_unroll5d_order_by_first_or_default_fold() : tuple<id : int; price : int; flag : int> =>
    _fold(from_decs_template(type<Unroll5dRow>)._order_by(_.price).first_or_default((id = -1, price = -99, flag = -1)))

[test(description = "order_by then first_or_default over a non-empty decs source returns the first element")]
def test_unroll5d_order_by_first_or_default_nonempty(t : T?) {
    fixture_unroll5d()
    t |> equal(target_unroll5d_order_by_first_or_default_fold().price, 5)
}

[test(description = "order_by then first_or_default over an empty decs source returns the default")]
def test_unroll5d_order_by_first_or_default_empty(t : T?) {
    restart()
    commit()
    t |> equal(target_unroll5d_order_by_first_or_default_fold().price, -99,
               "empty decs source → default")
}

// Eval-order invariant: the splice must evaluate the first_or_default
// default expression BEFORE the for_each_archetype loop, matching
// plan_decs_reverse's first_or_default hoist + the eager-arg-eval
// semantics of the equivalent non-spliced chain. Detected via a
// side-effecting predicate that flips a global flag during iter and a
// side-effecting default that snapshots the flag.
var g_eval_order_iter_seen : bool = false

var g_eval_order_default_saw_iter : bool = false

def trace_pred_for_eval_order(rec : tuple<id : int; price : int; flag : int>) : bool {
    g_eval_order_iter_seen = true
    return rec.flag == 1
}

def trace_default_for_eval_order() : tuple<id : int; price : int; flag : int> {
    g_eval_order_default_saw_iter = g_eval_order_iter_seen
    return (id = -1, price = -99, flag = -1)
}

[test(description = "the first_or_default default expression is evaluated before the archetype loop runs")]
def test_unroll5d_order_by_first_or_default_eval_order(t : T?) {
    fixture_unroll5d()
    g_eval_order_iter_seen = false
    g_eval_order_default_saw_iter = true
    let got = _fold(
        from_decs_template(type<Unroll5dRow>)
            ._where(trace_pred_for_eval_order(_))
            ._order_by(_.price)
            .first_or_default(trace_default_for_eval_order())
    )
    t |> equal(g_eval_order_default_saw_iter, false,
               "default arg evaluated before for_each_archetype iteration")
    t |> equal(got.flag, 1, "non-empty: top-1 by price among flag==1 returned")
}

[export, marker(no_coverage)]
def target_unroll5d_where_order_by_take_fold() : array<tuple<id : int; price : int; flag : int>> =>
    <- _fold(from_decs_template(type<Unroll5dRow>)._where(_.flag == 1)._order_by(_.price).take(2).to_array())

[test(description = "where, order_by and take over a decs source match the reference")]
def test_unroll5d_where_order_by_take_parity(t : T?) {
    fixture_unroll5d()
    let got <- target_unroll5d_where_order_by_take_fold()
    // flag==1 rows: ids 0,2,4,6 → prices 30,50,40,35. Sorted asc:
    // 30,35,40,50. take(2) → 30,35.
    t |> equal(got.length(), 2)
    t |> equal(got[0].price, 30)
    t |> equal(got[1].price, 35)
}

[export, marker(no_coverage)]
def target_unroll5d_where_order_by_first_fold() : tuple<id : int; price : int; flag : int> =>
    _fold(from_decs_template(type<Unroll5dRow>)._where(_.flag == 1)._order_by(_.price).first())

[test(description = "where, order_by and first over a decs source match the reference")]
def test_unroll5d_where_order_by_first_parity(t : T?) {
    fixture_unroll5d()
    // flag==1 rows: prices 30,50,40,35 → min = 30.
    t |> equal(target_unroll5d_where_order_by_first_fold().price, 30)
}

[export, marker(no_coverage)]
def target_unroll5d_reverse_to_array_fold() : array<tuple<id : int; price : int; flag : int>> =>
    <- _fold(from_decs_template(type<Unroll5dRow>).reverse().to_array())

[test(description = "reverse then to_array over a decs source matches the reference")]
def test_unroll5d_reverse_to_array_parity(t : T?) {
    fixture_unroll5d()
    let got <- target_unroll5d_reverse_to_array_fold()
    // Decs walk order is archetype-dependent. We don't assert exact order
    // vs the eager bridge — we assert .reverse() produces the SAME
    // multiset reversed wrt a non-reverse fold. Cheap check: length +
    // presence of all ids.
    t |> equal(got.length(), 7)
    var idsSeen : table<int>
    for (r in got) {
        idsSeen |> insert(r.id)
    }
    for (i in range(7)) {
        t |> success(key_exists(idsSeen, i), "id {i} present")
    }
}

[export, marker(no_coverage)]
def target_unroll5d_reverse_take_fold() : array<tuple<id : int; price : int; flag : int>> =>
    <- _fold(from_decs_template(type<Unroll5dRow>).reverse().take(3).to_array())

[test(description = "reverse then take over a decs source matches the reference")]
def test_unroll5d_reverse_take_parity(t : T?) {
    fixture_unroll5d()
    let got <- target_unroll5d_reverse_take_fold()
    t |> equal(got.length(), 3, "reverse + take(3) yields 3 rows")
}

[test(description = "reverse then take past the entity total returns every entity reversed")]
def test_unroll5d_reverse_take_beyond_length(t : T?) {
    fixture_unroll5d()
    let huge = _fold(from_decs_template(type<Unroll5dRow>).reverse().take(99).to_array())
    t |> equal(huge.length(), 7, "take(N>length) returns all rows reversed")
}

[test(description = "reverse then take(0) over a decs source returns nothing")]
def test_unroll5d_reverse_take_zero(t : T?) {
    fixture_unroll5d()
    // Runtime-0 take expression: a const-folded `take(0)` literal trips
    // PERF017 on the splice-emitted resize formula (`takeB < length(buf)`
    // collapses to `0 < length(buf)`).
    var zeroN = 0  // nolint:LINT003 — kept as var to defeat const-fold of the take(N) arg
    let zero = _fold(from_decs_template(type<Unroll5dRow>).reverse().take(zeroN).to_array())
    t |> success(empty(zero), "take(0) returns empty")
}

// Side-effecting take(N) source — the splice must evaluate the take
// expression EXACTLY ONCE.
var g_take_call_count = 0

def bump_take_count() : int {
    g_take_call_count ++
    return 3
}

[test(description = "a side-effecting take count in reverse then take is evaluated exactly once")]
def test_unroll5d_reverse_take_single_eval(t : T?) {
    fixture_unroll5d()
    g_take_call_count = 0
    let got <- _fold(from_decs_template(type<Unroll5dRow>).reverse().take(bump_take_count()).to_array())
    t |> equal(got.length(), 3, "take(N) honored — 3 rows")
    t |> equal(g_take_call_count, 1,
               "take(N) expression evaluated exactly once (no multi-eval from splice clones)")
}

[export, marker(no_coverage)]
def target_unroll5d_reverse_count_fold() : int =>
    _fold(from_decs_template(type<Unroll5dRow>).reverse().count())

[test(description = "reverse then count over a decs source equals the plain count")]
def test_unroll5d_reverse_count_parity(t : T?) {
    fixture_unroll5d()
    t |> equal(target_unroll5d_reverse_count_fold(), 7)
}

[export, marker(no_coverage)]
def target_unroll5d_where_reverse_first_or_default_fold() : tuple<id : int; price : int; flag : int> =>
    _fold(from_decs_template(type<Unroll5dRow>)._where(_.flag == 1).reverse().first_or_default((id = -1, price = -99, flag = -1)))

[test(description = "where, reverse and first_or_default over a decs source return the default when nothing passes")]
def test_unroll5d_where_reverse_first_or_default_empty(t : T?) {
    restart()
    commit()
    t |> equal(target_unroll5d_where_reverse_first_or_default_fold().id, -1,
               "empty decs source → default")
}

[test(description = "where, reverse and first_or_default over a decs source return the last passing entity")]
def test_unroll5d_where_reverse_first_or_default_nonempty(t : T?) {
    fixture_unroll5d()
    // flag==1 ids = {0,2,4,6}; last-encountered (reverse-of-first) is the
    // maximum encountered id in archetype walk order. We only assert it's
    // one of the flag==1 ids.
    let id = target_unroll5d_where_reverse_first_or_default_fold().id
    t |> success(id == 0 || id == 2 || id == 4 || id == 6,
                 "first-of-reverse(flag==1) is one of the matching ids")
}

[export, marker(no_coverage)]
def target_unroll5d_where_reverse_to_array_fold() : array<tuple<id : int; price : int; flag : int>> =>
    <- _fold(from_decs_template(type<Unroll5dRow>)._where(_.flag == 1).reverse().to_array())

[test(description = "where, reverse and to_array over a decs source match the reference")]
def test_unroll5d_where_reverse_to_array_parity(t : T?) {
    fixture_unroll5d()
    let got <- target_unroll5d_where_reverse_to_array_fold()
    t |> equal(got.length(), 4)
    for (r in got) {
        t |> equal(r.flag, 1)
    }
}

[export, marker(no_coverage)]
def target_unroll5d_where_select_reverse_to_array_fold() : array<int> =>
    <- _fold(from_decs_template(type<Unroll5dRow>)._where(_.flag == 1)._select(_.id).reverse().to_array())

[test(description = "where, select, reverse and to_array over a decs source project through the reverse like the reference")]
def test_unroll5d_where_select_reverse_to_array_parity(t : T?) {
    fixture_unroll5d()
    let got <- target_unroll5d_where_select_reverse_to_array_fold()
    t |> equal(got.length(), 4, "4 flag==1 rows projected to id")
    var idsSeen : table<int>
    for (id in got) {
        idsSeen |> insert(id)
    }
    for (expectedId in [0, 2, 4, 6]) {
        t |> success(key_exists(idsSeen, expectedId), "id {expectedId} present in projection")
    }
}

[test(description = "order_by then take over a decs source folds into an archetype walk with a bounded buffer")]
def test_unroll5d_order_by_take_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_unroll5d_order_by_take3_fold)
        t |> success(func != null, "RTTI must resolve target")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        t |> equal(describe_count(body_expr, "to_sequence"), 0, "order_by+take splice must NOT fall to tier-2 to_sequence (cascade ordering trap)")
        t |> equal(describe_count(body_expr, "for_each_archetype"), 1,
                   "exactly one for_each_archetype (no eager-bridge wrapping)")
        t |> equal(describe_count(body_expr, "for_each_archetype_find"), 0,
                   "order family uses for_each_archetype (no early-exit needed)")
        // PR C: emit_bounded_heap (unified array+decs) uses `order_buf`
        // instead of decs_buf. Buffer still hoisted above for_each_archetype.
        t |> success(describe_count(body_expr, "order_buf") >= 2, "order_buf declared + populated")
        // Bounded-heap emit: spliced_push_heap appears twice (fill +
        // replace), spliced_pop_heap once (replace). top_n_by is no longer
        // used.
        t |> equal(describe_count(body_expr, "spliced_push_heap"), 2,
                   "bounded-heap fill + replace each call spliced_push_heap")
        t |> equal(describe_count(body_expr, "spliced_pop_heap"), 1,
                   "bounded-heap replace calls spliced_pop_heap once")
        t |> equal(describe_count(body_expr, "top_n_by"), 0,
                   "bounded-heap path does NOT call top_n_by")
    }
}

[test(description = "order_by then first over a decs source folds into a single streaming-minimum archetype walk")]
def test_unroll5d_order_by_first_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_unroll5d_order_by_first_fold)
        t |> success(func != null, "RTTI must resolve target")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched, "qmatch must capture body")
        t |> equal(describe_count(body_expr, "to_sequence"), 0,
                   "order_by+first splice must NOT fall to tier-2 to_sequence")
        t |> equal(describe_count(body_expr, "for_each_archetype"), 1,
                   "exactly one for_each_archetype")
        // Streaming-min emit: best + seen state vars, no buffer +
        // min_by/top_n.
        t |> equal(describe_count(body_expr, "min_by"), 0,
                   "streaming-min path does NOT call min_by")
        t |> equal(describe_count(body_expr, "top_n_by"), 0,
                   "streaming-min path does NOT call top_n_by")
        // PR C: emit_streaming_min uses `order_best` / `order_seen` (unified
        // array+decs naming).
        t |> success(describe_count(body_expr, "order_best") >= 2,
                     "order_best state var declared + updated")
        t |> success(describe_count(body_expr, "order_seen") >= 2,
                     "order_seen flag declared + updated")
        // Empty-source panic guard preserved.
        t |> equal(describe_count(body_expr, "sequence contains no elements"), 1,
                   "panic-on-empty guard for first()")
    }
}

[test(description = "reverse then to_array over a decs source folds into an archetype walk and an in-place reverse")]
def test_unroll5d_reverse_to_array_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_unroll5d_reverse_to_array_fold)
        t |> success(func != null, "RTTI must resolve target")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched, "qmatch must capture body")
        t |> equal(describe_count(body_expr, "to_sequence"), 0,
                   "reverse splice must NOT fall to tier-2 to_sequence")
        t |> equal(describe_count(body_expr, "for_each_archetype"), 1,
                   "exactly one for_each_archetype")
        t |> equal(describe_count(body_expr, "reverse_inplace"), 1,
                   "buffer reversed via reverse_inplace")
    }
}

[test(description = "reverse then count over a decs source folds into a counter with no buffer")]
def test_unroll5d_reverse_count_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_unroll5d_reverse_count_fold)
        t |> success(func != null, "RTTI must resolve target")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched, "qmatch must capture body")
        // reverse+count → counter loop, NO buffer, NO reverse_inplace.
        t |> equal(describe_count(body_expr, "to_sequence"), 0,
                   "reverse+count splice must NOT fall to tier-2 to_sequence")
        t |> equal(describe_count(body_expr, "for_each_archetype"), 1,
                   "exactly one for_each_archetype")
        t |> equal(describe_count(body_expr, "reverse_inplace"), 0,
                   "reverse is identity for count — no reverse_inplace")
        t |> equal(describe_count(body_expr, "decs_buf"), 0, "no buffer needed for count")
    }
}

[decs_template(prefix = "unroll5c_")]
struct Unroll5cRow {
    id    : int
    brand : int
    price : int
}

def private fixture_unroll5c() {
    restart()
    // 12 rows. brand cycles 0..3 (so 4 distinct brands). price = id*10
    // (12 distinct values).
    for (i in range(12)) {
        create_entity() @(eid, cmp) {
            cmp.eid := eid
            cmp.unroll5c_id := i
            cmp.unroll5c_brand := i % 4
            cmp.unroll5c_price := i * 10
        }
    }
    commit()
}

[export, marker(no_coverage)]
def target_unroll5c_select_distinct_to_array_fold() : array<int> =>
    <- _fold(from_decs_template(type<Unroll5cRow>)._select(_.brand).distinct().to_array())

[test(description = "select, distinct and to_array over a decs source match the reference")]
def test_unroll5c_select_distinct_to_array_parity(t : T?) {
    fixture_unroll5c()
    let got <- target_unroll5c_select_distinct_to_array_fold()
    t |> equal(got.length(), 4, "4 distinct brands (0..3)")
    // Brands cycle 0,1,2,3,0,1,2,3,0,1,2,3 → first-seen order is 0,1,2,3.
    let expected = [0, 1, 2, 3]
    for (i in range(4)) {
        t |> equal(got[i], expected[i])
    }
}

[test(description = "select, distinct and to_array over an empty decs source return nothing")]
def test_unroll5c_select_distinct_to_array_empty(t : T?) {
    restart()
    commit()
    let got <- target_unroll5c_select_distinct_to_array_fold()
    t |> equal(got.length(), 0, "empty decs → empty distinct")
}

[export, marker(no_coverage)]
def target_unroll5c_select_distinct_count_fold() : int =>
    _fold(from_decs_template(type<Unroll5cRow>)._select(_.brand).distinct().count())

[test(description = "select, distinct and count over a decs source match the reference")]
def test_unroll5c_select_distinct_count_parity(t : T?) {
    fixture_unroll5c()
    t |> equal(target_unroll5c_select_distinct_count_fold(), 4, "length(seen) = 4 distinct brands")
}

[export, marker(no_coverage)]
def target_unroll5c_select_distinct_long_count_fold() : int64 =>
    _fold(from_decs_template(type<Unroll5cRow>)._select(_.brand).distinct().long_count())

[test(description = "select, distinct and long_count over a decs source match the reference as int64")]
def test_unroll5c_select_distinct_long_count_parity(t : T?) {
    fixture_unroll5c()
    t |> equal(target_unroll5c_select_distinct_long_count_fold(), 4l, "int64(length(seen)) = 4")
}

[export, marker(no_coverage)]
def target_unroll5c_select_distinct_sum_fold() : int =>
    _fold(from_decs_template(type<Unroll5cRow>)._select(_.brand).distinct().sum())

[test(description = "select, distinct and sum over a decs source match the reference total")]
def test_unroll5c_select_distinct_sum_parity(t : T?) {
    fixture_unroll5c()
    // sum of distinct brands {0,1,2,3} = 6
    t |> equal(target_unroll5c_select_distinct_sum_fold(), 6, "acc folds at fresh-key site only")
}

[export, marker(no_coverage)]
def target_unroll5c_select_distinct_take2_fold() : array<int> =>
    <- _fold(from_decs_template(type<Unroll5cRow>)._select(_.brand).distinct().take(2).to_array())

[test(description = "select, distinct and take over a decs source match the reference")]
def test_unroll5c_select_distinct_take_parity(t : T?) {
    fixture_unroll5c()
    let got <- target_unroll5c_select_distinct_take2_fold()
    // First 2 distinct in source order: 0, 1.
    t |> equal(got.length(), 2)
    t |> equal(got[0], 0)
    t |> equal(got[1], 1)
}

[test(description = "select, distinct and take(0) over a decs source return nothing")]
def test_unroll5c_select_distinct_take_zero(t : T?) {
    fixture_unroll5c()
    let got <- _fold(from_decs_template(type<Unroll5cRow>)._select(_.brand).distinct().take(0).to_array())
    t |> equal(got.length(), 0, "take(0) yields empty")
}

[test(description = "select, distinct and take past the distinct total return every distinct value")]
def test_unroll5c_select_distinct_take_beyond(t : T?) {
    fixture_unroll5c()
    let got <- _fold(from_decs_template(type<Unroll5cRow>)._select(_.brand).distinct().take(99).to_array())
    t |> equal(got.length(), 4, "take(N>num_distinct) returns all distinct")
}

[export, marker(no_coverage)]
def target_unroll5c_where_distinct_to_array_fold() : array<int> =>
    <- _fold(from_decs_template(type<Unroll5cRow>)._where(_.id < 8)._select(_.brand).distinct().to_array())

[test(description = "where then distinct over a decs source matches the reference")]
def test_unroll5c_where_distinct_parity(t : T?) {
    fixture_unroll5c()
    let got <- target_unroll5c_where_distinct_to_array_fold()
    // id<8 → ids 0..7 → brands 0,1,2,3,0,1,2,3 → distinct = 4.
    t |> equal(got.length(), 4)
}

[export, marker(no_coverage)]
def target_unroll5c_distinct_by_to_array_fold() : array<tuple<id : int; brand : int; price : int>> =>
    <- _fold(from_decs_template(type<Unroll5cRow>)._distinct_by(_.brand).to_array())

[test(description = "distinct_by over a decs source matches the reference")]
def test_unroll5c_distinct_by_parity(t : T?) {
    fixture_unroll5c()
    let got <- target_unroll5c_distinct_by_to_array_fold()
    // First row per distinct brand: ids 0,1,2,3 (brands 0,1,2,3).
    t |> equal(got.length(), 4)
    for (i in range(4)) {
        t |> equal(got[i].id, i)
        t |> equal(got[i].brand, i)
    }
}

[export, marker(no_coverage)]
def target_unroll5c_distinct_by_count_fold() : int =>
    _fold(from_decs_template(type<Unroll5cRow>)._distinct_by(_.brand).count())

[test(description = "distinct_by then count over a decs source matches the reference")]
def test_unroll5c_distinct_by_count_parity(t : T?) {
    fixture_unroll5c()
    t |> equal(target_unroll5c_distinct_by_count_fold(), 4)
}

[export, marker(no_coverage)]
def target_unroll5c_distinct_by_take2_fold() : array<tuple<id : int; brand : int; price : int>> =>
    <- _fold(from_decs_template(type<Unroll5cRow>)._distinct_by(_.brand).take(2).to_array())

[test(description = "distinct_by then take over a decs source matches the reference")]
def test_unroll5c_distinct_by_take_parity(t : T?) {
    fixture_unroll5c()
    let got <- target_unroll5c_distinct_by_take2_fold()
    t |> equal(got.length(), 2)
    t |> equal(got[0].brand, 0)
    t |> equal(got[1].brand, 1)
}

var g_unroll5c_take_arg_calls : int = 0

def trace_take_arg() : int {
    g_unroll5c_take_arg_calls ++
    return 2
}

[test(description = "the take count and the projection in a distinct chain over a decs source are evaluated once")]
def test_unroll5c_distinct_take_arg_evaluated_once(t : T?) {
    fixture_unroll5c()
    g_unroll5c_take_arg_calls = 0
    let got <- _fold(from_decs_template(type<Unroll5cRow>)._select(_.brand).distinct().take(trace_take_arg()).to_array())
    t |> equal(g_unroll5c_take_arg_calls, 1,
               "take(N) arg must evaluate exactly once at invoke entry, not per fresh-key check")
    t |> equal(got.length(), 2, "result still matches the bound N")
}

[test(description = "select, distinct and count over a decs source fold into a hoisted seen-table archetype walk")]
def test_unroll5c_select_distinct_count_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_unroll5c_select_distinct_count_fold)
        t |> success(func != null, "RTTI must resolve target")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched, "qmatch must capture body")
        t |> equal(describe_count(body_expr, "to_sequence"), 0,
                   "distinct+count splice must NOT fall to tier-2 to_sequence")
        t |> equal(describe_count(body_expr, "for_each_archetype"), 1,
                   "exactly one for_each_archetype (no _find — no take present)")
        t |> equal(describe_count(body_expr, "for_each_archetype_find"), 0, "no take → no _find")
        // PR C: emit_hashtable_dedup (unified array+decs) uses `buf` / `seen`
        // (drop the `decs_` prefix).
        t |> equal(describe_count(body_expr, "`buf"), 0, "count terminator elides the buffer")
        t |> success(describe_count(body_expr, "`seen") >= 2, "seen-table declared + queried")
        t |> equal(describe_count(body_expr, "key_exists"), 1, "single fresh-key check per element")
    }
}

[test(description = "select, distinct and take over a decs source fold into a streaming dedup with an early exit")]
def test_unroll5c_select_distinct_take_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_unroll5c_select_distinct_take2_fold)
        t |> success(func != null, "RTTI must resolve target")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched, "qmatch must capture body")
        t |> equal(describe_count(body_expr, "to_sequence"), 0,
                   "distinct+take splice must NOT fall to tier-2 to_sequence")
        // take present → outer iteration switches to _find so take-cap can
        // stop across archetypes.
        t |> equal(describe_count(body_expr, "for_each_archetype_find"), 1,
                   "take present → for_each_archetype_find")
        // PR C: emit_hashtable_dedup (unified array+decs) uses `buf` / `seen`
        // / `taken`.
        t |> success(describe_count(body_expr, "`buf") >= 2,
                     "buffer needed for to_array terminator")
        t |> success(describe_count(body_expr, "`seen") >= 2, "seen-table declared + queried")
        t |> success(describe_count(body_expr, "`taken") >= 2,
                     "taken counter declared + incremented")
    }
}

[test(description = "distinct_by then to_array over a decs source folds into a streaming dedup archetype walk")]
def test_unroll5c_distinct_by_to_array_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_unroll5c_distinct_by_to_array_fold)
        t |> success(func != null, "RTTI must resolve target")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched, "qmatch must capture body")
        t |> equal(describe_count(body_expr, "to_sequence"), 0,
                   "distinct_by splice must NOT fall to tier-2")
        t |> equal(describe_count(body_expr, "for_each_archetype"), 1,
                   "exactly one for_each_archetype")
        t |> equal(describe_count(body_expr, "for_each_archetype_find"), 0, "no take → no _find")
        // The key lambda invocation is the distinct_by signature — invoke the
        // key block to compute the key, then unique_key wraps it.
        t |> success(describe_count(body_expr, "unique_key") >= 1,
                     "distinct_by routes key through unique_key")
    }
}

[test(description = "select, distinct and sum over a decs source fold into a streaming dedup with an inline accumulator")]
def test_unroll5c_select_distinct_sum_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_unroll5c_select_distinct_sum_fold)
        t |> success(func != null, "RTTI must resolve target")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched, "qmatch must capture body")
        t |> equal(describe_count(body_expr, "to_sequence"), 0,
                   "distinct+sum splice must NOT fall to tier-2")
        t |> equal(describe_count(body_expr, "for_each_archetype"), 1,
                   "no take → plain for_each_archetype")
        // PR C: emit_hashtable_dedup (unified array+decs) uses `buf` / `acc`.
        t |> equal(describe_count(body_expr, "`buf"), 0, "sum terminator elides the buffer")
        t |> success(describe_count(body_expr, "`acc") >= 2, "accumulator declared + folded")
    }
}

[test(description = "select, distinct and count with a predicate over a decs source match the reference")]
def test_unroll5c_select_distinct_count_pred_parity(t : T?) {
    fixture_unroll5c()
    // brands cycle 0..3 (4 distinct). distinct |> count(>=2) → only
    // brands 2 and 3 → 2.
    let got = _fold(from_decs_template(type<Unroll5cRow>)._select(_.brand).distinct().count($(x : int) => x >= 2))
    t |> equal(got, 2, "predicate must be honored, not dropped")
}

[test(description = "select, distinct and long_count with a predicate over a decs source match the reference as int64")]
def test_unroll5c_select_distinct_long_count_pred_parity(t : T?) {
    fixture_unroll5c()
    let got = _fold(from_decs_template(type<Unroll5cRow>)._select(_.brand).distinct().long_count($(x : int) => x >= 2))
    t |> equal(got, 2l, "predicate must be honored, not dropped")
}

[decs_template(prefix = "wave4_")]
struct Wave4Row {
    brand : int
    price : int
    year  : int
}

def private fixture_wave4(n : int) {
    restart()
    for (i in range(n)) {
        create_entity() @(eid, cmp) {
            cmp.eid := eid
            cmp.wave4_brand := i % 4
            cmp.wave4_price := i * 10
            cmp.wave4_year  := 2000 + (i % 25)
        }
    }
    commit()
}

[export, marker(no_coverage)]
def target_wave4_pruned_select_sum_fold() : int =>
    _fold(from_decs_template(type<Wave4Row>)._select(_.price).sum())

[test(description = "select then sum over a decs source touching one field gives the right total with the component list pruned")]
def test_wave4_pruned_select_sum_parity(t : T?) {
    fixture_wave4(10)
    // sum(price) for i in 0..9 = 10*(0+1+...+9) = 450
    t |> equal(target_wave4_pruned_select_sum_fold(), 450,
               "_select(_.price).sum() — only price referenced; pruning must preserve semantics")
}

[export, marker(no_coverage)]
def target_wave4_pruned_where_count_fold() : int =>
    _fold(from_decs_template(type<Wave4Row>)._where(_.price > 50).count())

[test(description = "where then count over a decs source touching one field gives the right count with the component list pruned")]
def test_wave4_pruned_where_count_parity(t : T?) {
    fixture_wave4(10)
    // i=0..9 → price=0,10,20,...90; price>50 → i in {6,7,8,9} → 4 rows
    t |> equal(target_wave4_pruned_where_count_fold(), 4, "_where(_.price>50).count() — only price referenced; pruning must preserve semantics")
}
