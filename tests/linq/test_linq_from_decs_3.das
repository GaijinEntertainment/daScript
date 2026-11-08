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
require math
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

// Structural ExprFor counter — robust against describe() formatting
// changes (Copilot review #2).
def private count_expr_for(expr : Expression?) : int { // nolint:STYLE037 - one arm per node class
    if (expr == null) return 0
    var n = 0
    if (expr is ExprFor) {
        n ++
        var ef = expr as ExprFor
        for (s in ef.sources) {
            n += count_expr_for(s)
        }
        n += count_expr_for(ef.body)
    } elif (expr is ExprBlock) {
        var eb = expr as ExprBlock
        for (s in eb.list) {
            n += count_expr_for(s)
        }
    } elif (expr is ExprMakeBlock) {
        var emb = expr as ExprMakeBlock
        n += count_expr_for(emb._block)
    } elif (expr is ExprInvoke) {
        var ei = expr as ExprInvoke
        for (a in ei.arguments) {
            n += count_expr_for(a)
        }
    } elif (expr is ExprCall) {
        var ec = expr as ExprCall
        for (a in ec.arguments) {
            n += count_expr_for(a)
        }
    } elif (expr is ExprLet) {
        var el = expr as ExprLet
        for (v in el.variables) {
            n += count_expr_for(v.init)
        }
    } elif (expr is ExprReturn) {
        var er = expr as ExprReturn
        n += count_expr_for(er.subexpr)
    } elif (expr is ExprIfThenElse) {
        var ei = expr as ExprIfThenElse
        n += count_expr_for(ei.cond)
        n += count_expr_for(ei.if_true)
        n += count_expr_for(ei.if_false)
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
def target_unroll_select_sum_fold() : int =>
    _fold(from_decs_template(type<UnrollChainRow>)._select(_.val).sum())

[export, marker(no_coverage)]
def target_unroll_where_select_sum_fold() : int =>
    _fold(from_decs_template(type<UnrollChainRow>)._where(_.flag == 1)._select(_.val).sum())

[decs_template(prefix = "unroll5d_")]
struct Unroll5dRow {
    id    : int
    price : int
    flag  : int
}

[export, marker(no_coverage)]
def target_unroll5d_reverse_take_fold() : array<tuple<id : int; price : int; flag : int>> =>
    <- _fold(from_decs_template(type<Unroll5dRow>).reverse().take(3).to_array())

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
def target_wave4_pruned_where_count_fold() : int =>
    _fold(from_decs_template(type<Wave4Row>)._where(_.price > 50).count())

[export, marker(no_coverage)]
def target_wave4_pruned_select_where_to_array_fold() : array<int> =>
    <- _fold(from_decs_template(type<Wave4Row>)._where(_.price > 50)._select(_.brand).to_array())

[test(description = "select, where and to_array over a decs source touching some fields give the right elements with pruning")]
def test_wave4_pruned_select_where_to_array_parity(t : T?) {
    fixture_wave4(10)
    // Survivors: i=6,7,8,9 → brand = 6%4=2, 7%4=3, 8%4=0, 9%4=1
    let got <- target_wave4_pruned_select_where_to_array_fold()
    t |> equal(got |> length, 4, "4 surviving brands")
    t |> equal(got |> sum, 2 + 3 + 0 + 1, "brand sum independent of order")
}

[export, marker(no_coverage)]
def target_wave4_unpruned_bare_to_array_fold() : array<tuple<brand : int; price : int; year : int>> =>
    <- _fold(from_decs_template(type<Wave4Row>).to_array())

[test(description = "a bare to_array over a decs source returns whole rows, so no component is pruned")]
def test_wave4_unpruned_bare_to_array_parity(t : T?) {
    fixture_wave4(3)
    // Bare to_array: walker sees `push_clone(decs_tup)` whole-var ref →
    // no pruning → full tuple preserved
    let got <- target_wave4_unpruned_bare_to_array_fold()
    t |> equal(got |> length, 3, "3 rows materialized")
    // Sum each field independently — order-agnostic full-tuple check
    var brand_sum = 0
    var price_sum = 0
    var year_sum = 0
    for (c in got) {
        brand_sum += c.brand
        price_sum += c.price
        year_sum += c.year
    }
    t |> equal(brand_sum, 0 + 1 + 2, "brand sum (full tuple preserved)")
    t |> equal(price_sum, 0 + 10 + 20, "price sum (full tuple preserved)")
    t |> equal(year_sum, 2000 + 2001 + 2002, "year sum (full tuple preserved)")
}

def private wave4_pred_uses_whole_tup(t : tuple<brand : int; price : int; year : int>) : bool =>
    (t.brand + t.price + t.year) > 0

[export, marker(no_coverage)]
def target_wave4_unpruned_pass_to_fn_fold() : int =>
    _fold(from_decs_template(type<Wave4Row>)._where(wave4_pred_uses_whole_tup(_)).count())

[test(description = "passing the whole row to a user function over a decs source reads every field correctly")]
def test_wave4_unpruned_pass_to_fn_parity(t : T?) {
    fixture_wave4(3)
    // _where peel renames `_` to `decs_tup`; the predicate body becomes
    // `wave4_pred_uses_whole_tup(decs_tup)` — decs_tup referenced as a
    // whole-var arg → walker sets usesWholeRow=true → pruning disabled.
    // All 3 fields materialized. All 3 rows pass the > 0 predicate (year
    // always ≥ 2000), so count = 3.
    t |> equal(target_wave4_unpruned_pass_to_fn_fold(), 3,
               "pass-to-fn predicate must see full tuple")
}

[test(description = "passing the whole row to a user function disables pruning in the emitted walk")]
def test_wave4_unpruned_pass_to_fn_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_wave4_unpruned_pass_to_fn_fold)
        t |> success(func != null, "RTTI must resolve target")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched, "qmatch must capture body")
        // Whole-tup reference in the where predicate disables pruning — all 3
        // get_ros present.
        t |> equal(describe_count(body_expr, "get_ro"), 3,
                   "pass-to-fn must emit all 3 get_ros (no pruning)")
    }
}

[test(description = "where then count over a decs source emits a walk that reads only the referenced components")]
def test_wave4_pruned_where_count_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_wave4_pruned_where_count_fold)
        t |> success(func != null, "RTTI must resolve target")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched, "qmatch must capture body")
        // Wave 4 contract: chain reads only `price`; helper prunes brand+year
        // out of the multi-iter for-loop.
        t |> equal(describe_count(body_expr, "get_ro"), 1,
                   "_where(_.price>50).count() must emit exactly 1 get_ro (price)")
        t |> success(describe_count(body_expr, "wave4_price") >= 1, "kept slot is for price")
        t |> equal(describe_count(body_expr, "wave4_brand"), 0, "brand pruned out")
        t |> equal(describe_count(body_expr, "wave4_year"), 0, "year pruned out")
        t |> equal(describe_count(body_expr, "for_each_archetype"), 1,
                   "exactly one for_each_archetype (no _find — no take/early-exit)")
    }
}

[test(description = "a bare to_array over a decs source emits a walk that keeps every component")]
def test_wave4_unpruned_bare_to_array_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_wave4_unpruned_bare_to_array_fold)
        t |> success(func != null, "RTTI must resolve target")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched, "qmatch must capture body")
        // Whole-tup-rewriter contract: push_clone(decs_tup) →
        // push_clone((brand=wave4_brand, price=wave4_price, year=wave4_year))
        // — all 3 iter vars survive so the pruner keeps all 3 slots, but the
        // named-tuple bind is elided (no per-iter `var decs_tup = ...`). Test
        // name says "unpruned" historically — slot pruning still doesn't
        // fire, but bind elision does.
        t |> equal(describe_count(body_expr, "get_ro"), 3,
                   "bare to_array must emit all 3 get_ros (no slot pruning)")
        t |> equal(describe_count(body_expr, "decs_tup"), 0, "named-tuple bind elided — whole-var rewriter inlined (brand=iter,...) literal")
        t |> success(describe_count(body_expr, "wave4_brand") >= 1, "brand slot present")
        t |> success(describe_count(body_expr, "wave4_price") >= 1, "price slot present")
        t |> success(describe_count(body_expr, "wave4_year") >= 1, "year slot present")
    }
}

[export, marker(no_coverage)]
def target_wave4_take_count_no_field_ref_fold() : int =>
    _fold(from_decs_template(type<Wave4Row>).take(5).count())

[test(description = "take(5) then count that references no field over a decs source still returns 5")]
def test_wave4_take_count_no_field_ref_parity(t : T?) {
    fixture_wave4(10)
    // .take(5).count() — the per-element body only references the take
    // counter + acc; no tuple field refs. Walker reports empty usedNames
    // → helper short-circuits to unpruned emission (still safe; no field
    // referenced → no real difference).
    t |> equal(target_wave4_take_count_no_field_ref_fold(), 5,
               "take(5).count() splice must still return 5")
}

[export, marker(no_coverage)]
def target_wave4_all_fields_via_access_fold() : int {
    // All 3 fields referenced via field access (chained single-field
    // where_s — same all-fields trigger as a compound predicate, but each
    // peeled body has a single field-access node, matching the predicate
    // shape the rest of the suite uses).
    return _fold(from_decs_template(type<Wave4Row>)
        ._where(_.brand >= 0)
        ._where(_.price >= 0)
        ._where(_.year > 0)
        .count())
}

[test(description = "a chain that reads every field through member access over a decs source gives the right result")]
def test_wave4_all_fields_via_access_parity(t : T?) {
    fixture_wave4(10)
    // brand = i%4 (always >= 0), price = i*10 (always >= 0), year =
    // 2000+(i%25) (always > 0) → all 10 rows match.
    t |> equal(target_wave4_all_fields_via_access_fold(), 10,
               "all-fields-access predicate must return all rows")
}

[test(description = "a chain that reads every field through member access keeps all components in the walk")]
def test_wave4_all_fields_via_access_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_wave4_all_fields_via_access_fold)
        t |> success(func != null, "RTTI must resolve target")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched, "qmatch must capture body")
        // No slots pruned (all 3 fields read), but the bind is elided and
        // body references iter vars directly.
        t |> equal(describe_count(body_expr, "get_ro"), 3,
                   "all 3 get_ros present (no slot pruning)")
        t |> equal(describe_count(body_expr, "decs_tup"), 0,
                   "named-tuple bind elided when no whole-var ref")
        t |> success(describe_count(body_expr, "wave4_brand") >= 2,
                     "brand iter var read directly (slot + body ref)")
        t |> success(describe_count(body_expr, "wave4_price") >= 2, "price iter var read directly")
        t |> success(describe_count(body_expr, "wave4_year") >= 2, "year iter var read directly")
    }
}

[export, marker(no_coverage)]
def target_wave4_unpruned_reverse_to_array_fold() : array<tuple<brand : int; price : int; year : int>> =>
    <- _fold(from_decs_template(type<Wave4Row>).reverse().to_array())

[test(description = "reverse then to_array over a decs source returns whole rows")]
def test_wave4_unpruned_reverse_to_array_parity(t : T?) {
    fixture_wave4(3)
    // reverse + to_array (no _select): buffer holds full tuples →
    // push_clone(decs_tup) → walker sees whole-var → pruning disabled.
    // All 3 fields preserved.
    let got <- target_wave4_unpruned_reverse_to_array_fold()
    t |> equal(got |> length, 3, "3 rows after reverse")
    var brand_sum = 0
    var price_sum = 0
    var year_sum = 0
    for (c in got) {
        brand_sum += c.brand
        price_sum += c.price
        year_sum += c.year
    }
    t |> equal(brand_sum, 0 + 1 + 2, "brand sum preserved (reverse + bare to_array)")
    t |> equal(price_sum, 0 + 10 + 20, "price sum preserved")
    t |> equal(year_sum, 2000 + 2001 + 2002, "year sum preserved")
}

[export, marker(no_coverage)]
def target_wave4_unpruned_distinct_to_array_fold() : array<tuple<brand : int; price : int; year : int>> =>
    <- _fold(from_decs_template(type<Wave4Row>).distinct().to_array())

[test(description = "distinct then to_array over a decs source returns whole rows")]
def test_wave4_unpruned_distinct_to_array_parity(t : T?) {
    fixture_wave4(3)
    // distinct + to_array (no _select): each row distinct by full-tuple →
    // push_clone(decs_tup) → walker sees whole-var → pruning disabled.
    let got <- target_wave4_unpruned_distinct_to_array_fold()
    t |> equal(got |> length, 3, "3 distinct rows preserved as full tuples")
}

[export, marker(no_coverage)]
def target_wave4_pruned_select_reverse_to_array_fold() : array<int> =>
    <- _fold(from_decs_template(type<Wave4Row>)._select(_.brand).reverse().to_array())

[test(description = "select, reverse and to_array over a decs source give the right elements with pruning")]
def test_wave4_pruned_select_reverse_to_array_parity(t : T?) {
    fixture_wave4(4)
    // _select(_.brand).reverse().to_array() — chain rebinds to a scalar;
    // the reverse buffer holds projected brands (not the full tuple).
    // Pruning safe — only brand referenced. Brands for i=0..3 are
    // 0,1,2,3; reverse order = 3,2,1,0; sum = 6.
    let got <- target_wave4_pruned_select_reverse_to_array_fold()
    t |> equal(got |> length, 4, "4 brands after reverse")
    t |> equal(got |> sum, 6, "sum preserved across projection + reverse")
}

[export, marker(no_coverage)]
def target_wave4_pruned_select_distinct_count_fold() : int =>
    _fold(from_decs_template(type<Wave4Row>)._select(_.brand).distinct().count())

[test(description = "select, distinct and count over a decs source give the right count with pruning")]
def test_wave4_pruned_select_distinct_count_parity(t : T?) {
    fixture_wave4(10)
    // _select(_.brand).distinct().count() — brand cycles 0..3 (4 distinct
    // values) across 10 rows. Chain references only brand → walker
    // collects {brand} → pruning fires → only get_ro("wave4_brand").
    t |> equal(target_wave4_pruned_select_distinct_count_fold(), 4, "4 distinct brand values")
}

[test(description = "select, distinct and count over a decs source emit a walk that reads only the referenced components")]
def test_wave4_pruned_select_distinct_count_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_wave4_pruned_select_distinct_count_fold)
        t |> success(func != null, "RTTI must resolve target")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched, "qmatch must capture body")
        // Pruned via C2 (plan_decs_distinct path): only brand referenced.
        t |> equal(describe_count(body_expr, "get_ro"), 1,
                   "distinct.count chain emits exactly 1 get_ro")
        t |> success(describe_count(body_expr, "wave4_brand") >= 1, "kept slot is brand")
        t |> equal(describe_count(body_expr, "wave4_price"), 0, "price pruned out")
        t |> equal(describe_count(body_expr, "wave4_year"), 0, "year pruned out")
    }
}

[export, marker(no_coverage)]
def target_wave4_pruned_groupby_count_fold() : array<tuple<Brand : int; N : int>> =>
    <- _fold(from_decs_template(type<Wave4Row>)._group_by(_.brand)._select((Brand = _._0, N = _._1 |> length)).to_array())

[test(description = "group_by with a count reducer over a decs source gives the right buckets with pruning")]
def test_wave4_pruned_groupby_count_parity(t : T?) {
    fixture_wave4(10)
    // brand cycles 0..3 across 10 rows → 4 distinct buckets with sizes
    // [3,3,2,2].
    let got <- target_wave4_pruned_groupby_count_fold()
    t |> equal(got |> length, 4, "4 distinct brand buckets")
    var total = 0
    for (b in got) {
        total += b.N
    }
    t |> equal(total, 10, "buckets sum to source count")
}

[export, marker(no_coverage)]
def target_wave4_pruned_groupby_sum_fold() : array<tuple<Brand : int; S : int>> =>
    <- _fold(
             from_decs_template(type<Wave4Row>)._group_by(_.brand)._select(
        (Brand = _._0, S = _._1 |> _select(_.price) |> sum)).to_array())

[test(description = "group_by with a sum reducer over a decs source gives the right buckets with pruning")]
def test_wave4_pruned_groupby_sum_parity(t : T?) {
    fixture_wave4(10)
    // _group_by_lazy(_.brand) + inner-select-sum(_.price). Same safety
    // rule as the count case: plan_group_by_core bucket-build pushes the
    // whole `decs_tup` → walker disables pruning → all 3 get_ros emit,
    // even though the chain only reads brand + price.
    let got <- target_wave4_pruned_groupby_sum_fold()
    t |> equal(got |> length, 4, "4 distinct brand buckets")
    // Sum of all prices across all buckets = 10*(0+1+...+9) = 450 —
    // fixture independent of bucket layout.
    var total = 0
    for (b in got) {
        total += b.S
    }
    t |> equal(total, 450, "all-bucket price sum = full-table price sum")
}

[test(description = "group_by with a count reducer over a decs source emits a walk that reads only the referenced components")]
def test_wave4_pruned_groupby_count_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_wave4_pruned_groupby_count_fold)
        t |> success(func != null, "RTTI must resolve target")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched, "qmatch must capture body")
        // plan_group_by_core peels the key block (`_.brand` →
        // `decs_tup.brand`) inline; the count-by-group shape avoids
        // materializing the bucket — only `brand` survives in the body, so
        // pruning drops `price`+`year`.
        t |> equal(describe_count(body_expr, "get_ro"), 1,
                   "count-by-group reads only brand → pruning keeps 1 get_ro")
        t |> success(describe_count(body_expr, "wave4_brand") >= 1, "brand slot present")
        t |> equal(describe_count(body_expr, "wave4_price"), 0, "price pruned out")
        t |> equal(describe_count(body_expr, "wave4_year"), 0, "year pruned out")
    }
}

[export, marker(no_coverage)]
def target_lambda_count_fold() : int => _fold(from_decs($(unroll_val : int) {}).count())

[export, marker(no_coverage)]
def target_lambda_where_count_fold() : int =>
    _fold(from_decs($(unroll_val : int) {})._where(_.unroll_val >= 3).count())

[export, marker(no_coverage)]
def target_lambda_select_sum_fold() : int =>
    _fold(from_decs($(unroll_val : int) {})._select(_.unroll_val).sum())

[export, marker(no_coverage)]
def target_lambda_take_count_fold() : int =>
    _fold(from_decs($(unroll_val : int) {}).take(3).count())

[export, marker(no_coverage)]
def target_lambda_distinct_count_fold() : int =>
    _fold(from_decs($(unroll_val : int) {})._select(_.unroll_val).distinct().count())

[export, marker(no_coverage)]
def target_lambda_to_array_fold() : array<tuple<unroll_val : int>> =>
    <- _fold(from_decs($(unroll_val : int) {}).to_array())

[export, marker(no_coverage)]
def target_lambda_groupby_count_fold() : array<tuple<Bucket : int; N : int>> =>
    <- _fold(from_decs($(unroll_val : int) {})._group_by(_.unroll_val % 3)._select((Bucket = _._0, N = _._1 |> length)).to_array())

def private populate_unroll(n : int) {
    restart()
    for (i in range(n)) {
        create_entity() @(eid, cmp) {
            cmp.eid := eid
            cmp.unroll_val := i
        }
    }
    commit()
}

[test(description = "count over a lambda-argument from_decs source matches the type-argument form")]
def test_lambda_count_parity(t : T?) {
    populate_unroll(7)
    t |> equal(target_lambda_count_fold(), 7, "lambda-arg count matches type-arg")
}

[test(description = "where then count over a lambda-argument from_decs source matches the type-argument form")]
def test_lambda_where_count_parity(t : T?) {
    populate_unroll(7)
    // vals 0..6, _where(_>=3) → 3,4,5,6 → 4 matches
    t |> equal(target_lambda_where_count_fold(), 4, "lambda-arg where+count matches type-arg")
}

[test(description = "select then sum over a lambda-argument from_decs source matches the type-argument form")]
def test_lambda_select_sum_parity(t : T?) {
    populate_unroll(7)
    // sum(0..6) = 21
    t |> equal(target_lambda_select_sum_fold(), 21, "lambda-arg select+sum matches type-arg")
}

[test(description = "take then count over a lambda-argument from_decs source matches the type-argument form")]
def test_lambda_take_count_parity(t : T?) {
    populate_unroll(7)
    // take(3).count() — early-exit at 3, regardless of source size
    t |> equal(target_lambda_take_count_fold(), 3, "lambda-arg take+count matches type-arg")
}

[test(description = "distinct then count over a lambda-argument from_decs source matches the type-argument form")]
def test_lambda_distinct_count_parity(t : T?) {
    populate_unroll(7)
    // 7 unique vals → 7 distinct
    t |> equal(target_lambda_distinct_count_fold(), 7,
               "lambda-arg select+distinct+count matches type-arg")
}

[test(description = "to_array over a lambda-argument from_decs source matches the type-argument form")]
def test_lambda_to_array_parity(t : T?) {
    populate_unroll(5)
    let got <- target_lambda_to_array_fold()
    t |> equal(got |> length, 5, "lambda-arg to_array preserves whole tuple")
    var sum = 0
    for (item in got) {
        sum += item.unroll_val
    }
    t |> equal(sum, 0 + 1 + 2 + 3 + 4, "lambda-arg to_array element values match")
}

[test(description = "group_by with a count reducer over a lambda-argument from_decs source matches the type-argument form")]
def test_lambda_groupby_count_parity(t : T?) {
    populate_unroll(10)
    // vals 0..9, key = val % 3 → buckets {0:[0,3,6,9], 1:[1,4,7],
    // 2:[2,5,8]} → 3 buckets sized 4,3,3
    let got <- target_lambda_groupby_count_fold()
    t |> equal(got |> length, 3, "lambda-arg group_by yields 3 distinct keys")
    var total = 0
    for (b in got) {
        total += b.N
    }
    t |> equal(total, 10, "lambda-arg group_by buckets sum to source count")
}

[test(description = "count over a lambda-argument from_decs source emits the same splice shape as the type-argument form")]
def test_lambda_count_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_lambda_count_fold)
        t |> success(func != null, "RTTI resolves count target")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched, "qmatch captures body")
        // bare count goes through the arch.size shortcut: one
        // for_each_archetype, no inner per-entity for-loop, no eager-bridge
        // to_sequence. Identical to the type-arg form.
        t |> equal(describe_count(body_expr, "for_each_archetype"), 1,
                   "lambda-arg count emits one for_each_archetype")
        t |> equal(describe_count(body_expr, "to_sequence"), 0,
                   "lambda-arg count does NOT eager-bridge")
        t |> equal(count_expr_for(body_expr), 0, "lambda-arg count has no inner per-entity for")
        t |> equal(describe_count(body_expr, ".size"), 1, "lambda-arg count reads arch.size")
    }
}

[test(description = "where then count over a lambda-argument from_decs source emits the same splice shape as the type-argument form")]
def test_lambda_where_count_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_lambda_where_count_fold)
        t |> success(func != null, "RTTI resolves where+count target")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched, "qmatch captures body")
        // where+count emits one for_each_archetype with an inner multi-iter
        // for-loop; the predicate's matching counter accumulates inline. NO
        // to_sequence.
        t |> equal(describe_count(body_expr, "for_each_archetype"), 1,
                   "lambda-arg where+count emits one for_each_archetype")
        t |> equal(describe_count(body_expr, "to_sequence"), 0,
                   "lambda-arg where+count does NOT eager-bridge")
    }
}

[export, marker(no_coverage)]
def target_lambda_pruned_select_sum_fold() : int {
    // Multi-component query, but chain only references `wave4_price`.
    // Pruning must drop wave4_brand + wave4_year get_ros to match the
    // type-arg pruning behavior.
    return _fold(
                 from_decs(
                           $(wave4_brand : int; wave4_price : int;
                             wave4_year : int) {})._select(_.wave4_price).sum())
}

[test(description = "select then sum over a lambda-argument from_decs source gives the right total with pruning")]
def test_lambda_pruned_select_sum_parity(t : T?) {
    fixture_wave4(10)
    // i*10 for i in 0..9 = 0+10+20+...+90 = 450
    t |> equal(target_lambda_pruned_select_sum_fold(), 450,
               "lambda-arg pruning preserves sum semantics")
}

[test(description = "select then sum over a lambda-argument from_decs source prunes components like the type-argument form")]
def test_lambda_pruned_select_sum_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_lambda_pruned_select_sum_fold)
        t |> success(func != null, "RTTI resolves pruned target")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched, "qmatch captures body")
        // Lambda-arg declared 3 components but chain reads only wave4_price →
        // pruning keeps a single get_ro slot. Same gate the type-arg Wave 4
        // tests assert.
        t |> equal(describe_count(body_expr, "get_ro"), 1,
                   "lambda-arg pruning drops unused get_ros (3 declared → 1 emitted)")
        t |> success(describe_count(body_expr, "wave4_price") >= 1, "wave4_price retained")
        t |> equal(describe_count(body_expr, "wave4_brand"), 0, "wave4_brand pruned")
        t |> equal(describe_count(body_expr, "wave4_year"), 0, "wave4_year pruned")
    }
}

[export, marker(no_coverage)]
def target_unroll_last_fold() : int =>
    _fold(from_decs_template(type<UnrollChainRow>)._select(_.val).last())

[export, marker(no_coverage)]
def target_unroll_last_or_default_fold() : int =>
    _fold(from_decs_template(type<UnrollChainRow>)._where(_.val > 1000)._select(_.val).last_or_default(-9))

[export, marker(no_coverage)]
def target_unroll_single_fold() : int =>
    _fold(from_decs_template(type<UnrollChainRow>)._where(_.val == 3)._select(_.val).single())

[export, marker(no_coverage)]
def target_unroll_single_or_default_fold() : int =>
    _fold(from_decs_template(type<UnrollChainRow>)._where(_.val > 1000)._select(_.val).single_or_default(-7))

[export, marker(no_coverage)]
def target_unroll_aggregate_fold() : int =>
    _fold(from_decs_template(type<UnrollChainRow>)._select(_.val).aggregate(0) <| $(a, x) =>
        a + x * x)

[export, marker(no_coverage)]
def target_unroll_element_at_fold() : int =>
    _fold(from_decs_template(type<UnrollChainRow>)._select(_.val).element_at(3))

[export, marker(no_coverage)]
def target_unroll_element_at_or_default_fold() : int {
    // element_at_or_default returns default<int> (0) on out-of-range; no
    // user-supplied default.
    return _fold(from_decs_template(type<UnrollChainRow>)._select(_.val).element_at_or_default(999))
}

[test(description = "last over a decs source matches the reference")]
def test_unroll_last_parity(t : T?) {
    fixture_unroll2(5)
    // vals 0..4 → last is 4
    t |> equal(target_unroll_last_fold(), 4, "last splice parity")
}

[test(description = "last_or_default over an empty decs source returns the default")]
def test_unroll_last_or_default_empty(t : T?) {
    fixture_unroll2(5)
    // val>1000 → none → default -9
    t |> equal(target_unroll_last_or_default_fold(), -9, "last_or_default empty source")
}

[test(description = "single over a one-entity decs source returns that entity")]
def test_unroll_single_parity(t : T?) {
    fixture_unroll2(5)
    // val==3 matches exactly once → 3
    t |> equal(target_unroll_single_fold(), 3, "single splice parity")
}

[test(description = "single_or_default over an empty decs source returns the default")]
def test_unroll_single_or_default_empty(t : T?) {
    fixture_unroll2(5)
    // val>1000 → none → default -7
    t |> equal(target_unroll_single_or_default_fold(), -7, "single_or_default empty source")
}

[export, marker(no_coverage)]
def target_unroll_single_or_default_multi_fold() : int =>
    _fold(from_decs_template(type<UnrollChainRow>)._where(_.val < 100)._select(_.val).single_or_default(-7))

[test(description = "single_or_default over a decs source with several entities returns the default")]
def test_unroll_single_or_default_multi(t : T?) {
    fixture_unroll2(5)
    // 5 rows match → single_or_default returns default on 2nd match
    t |> equal(target_unroll_single_or_default_multi_fold(), -7,
               "single_or_default returns default when >1 match")
}

[test(description = "aggregate over a decs source matches the reference fold")]
def test_unroll_aggregate_parity(t : T?) {
    fixture_unroll2(5)
    // 0^2 + 1^2 + 2^2 + 3^2 + 4^2 = 30
    t |> equal(target_unroll_aggregate_fold(), 30, "aggregate sum-of-squares splice parity")
}

[test(description = "aggregate over an empty decs source returns the seed")]
def test_unroll_aggregate_empty(t : T?) {
    fixture_unroll2(0)
    // empty source → returns seed
    t |> equal(target_unroll_aggregate_fold(), 0, "aggregate empty returns seed")
}

[test(description = "element_at over a decs source matches the reference")]
def test_unroll_element_at_parity(t : T?) {
    fixture_unroll2(5)
    // vals 0..4, element_at(3) → 3
    t |> equal(target_unroll_element_at_fold(), 3, "element_at splice parity")
}

[test(description = "element_at_or_default over a decs source returns the default for an out-of-range index")]
def test_unroll_element_at_or_default_out_of_range(t : T?) {
    fixture_unroll2(5)
    // idx 999 way past end → default<int> == 0
    t |> equal(target_unroll_element_at_or_default_fold(), 0,
               "element_at_or_default out-of-range → default<int>")
}

// Range interaction: take + last / take + aggregate
[export, marker(no_coverage)]
def target_unroll_take_last_fold() : int =>
    _fold(from_decs_template(type<UnrollChainRow>)._select(_.val).take(3).last())

[test(description = "take then last over a decs source matches the reference")]
def test_unroll_take_last_parity(t : T?) {
    fixture_unroll2(10)
    // take(3) of 0..9 → 0,1,2 → last is 2
    t |> equal(target_unroll_take_last_fold(), 2, "take+last interaction")
}

[export, marker(no_coverage)]
def target_unroll_where_aggregate_fold() : int =>
    _fold(from_decs_template(type<UnrollChainRow>)._where(_.flag == 1)._select(_.val).aggregate(0) <| $(a, x) => a + x)

[test(description = "where then aggregate over a decs source matches the reference")]
def test_unroll_where_aggregate_parity(t : T?) {
    fixture_unroll2(5)
    // flag==1 rows: vals 1,3 → 1+3 = 4
    t |> equal(target_unroll_where_aggregate_fold(), 4, "where+aggregate interaction")
}

// Splice-shape gates: emit must not hit to_sequence; exact
// for_each_archetype{,_find} dispatch
[test(description = "last over a decs source folds into an archetype walk without to_sequence")]
def test_unroll_last_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_unroll_last_fold)
        t |> success(func != null, "RTTI must resolve target_unroll_last_fold")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        t |> equal(describe_count(body_expr, "to_sequence"), 0,
                   "last splice must NOT call to_sequence")
        t |> equal(describe_count(body_expr, "for_each_archetype"), 1,
                   "last (no range) uses plain for_each_archetype")
        t |> equal(describe_count(body_expr, "for_each_archetype_find"), 0,
                   "last (no range) does NOT need _find")
    }
}

[test(description = "single_or_default over a decs source folds into an exact archetype walk dispatch without to_sequence")]
def test_unroll_single_or_default_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_unroll_single_or_default_multi_fold)
        t |> success(func != null, "RTTI must resolve target_unroll_single_or_default_multi_fold")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        t |> equal(describe_count(body_expr, "to_sequence"), 0,
                   "single_or_default splice must NOT call to_sequence")
        t |> equal(describe_count(body_expr, "for_each_archetype_find"), 1,
                   "single_or_default uses _find for early-exit on 2nd match")
    }
}

[test(description = "aggregate over a decs source folds into an archetype walk without to_sequence")]
def test_unroll_aggregate_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_unroll_aggregate_fold)
        t |> success(func != null, "RTTI must resolve target_unroll_aggregate_fold")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        t |> equal(describe_count(body_expr, "to_sequence"), 0,
                   "aggregate splice must NOT call to_sequence")
        t |> equal(describe_count(body_expr, "for_each_archetype"), 1,
                   "aggregate (no range) uses plain for_each_archetype")
    }
}

[test(description = "element_at over a decs source folds into an archetype walk with a counter and no to_sequence")]
def test_unroll_element_at_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_unroll_element_at_fold)
        t |> success(func != null, "RTTI must resolve target_unroll_element_at_fold")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        t |> equal(describe_count(body_expr, "to_sequence"), 0,
                   "element_at splice must NOT call to_sequence")
        t |> equal(describe_count(body_expr, "for_each_archetype_find"), 1,
                   "element_at uses _find for counter early-exit")
    }
}

[test(description = "take then last over a decs source folds into an archetype find dispatch")]
def test_unroll_take_last_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_unroll_take_last_fold)
        t |> success(func != null, "RTTI must resolve target_unroll_take_last_fold")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        t |> equal(describe_count(body_expr, "to_sequence"), 0,
                   "take+last splice must NOT call to_sequence")
        // take present → bool-lambda dispatch (for_each_archetype_find) so
        // the range's `return true` exits the walk
        t |> equal(describe_count(body_expr, "for_each_archetype_find"), 1,
                   "take+last uses _find when range present")
    }
}

// Range × walk/element_at combos: take present upgrades the
// otherwise-Each walk ops (single / aggregate) to the _find dispatch
// (the range guard's `return true` only works in a bool-lambda), and
// caps the counter-driven element_at. take+last covers `last`; these
// cover the rest of the migrated terminators.
[export, marker(no_coverage)]
def target_unroll_take_single_fold() : int =>
    _fold(from_decs_template(type<UnrollChainRow>)._select(_.val).take(1).single())

[test(description = "take then single over a decs source matches the reference")]
def test_unroll_take_single_parity(t : T?) {
    fixture_unroll2(5)
    // take(1) of 0..4 → exactly [0] → single returns 0 (no >1-match
    // panic)
    t |> equal(target_unroll_take_single_fold(), 0, "take+single returns the sole windowed element")
}

[test(description = "take then single over a decs source upgrades to the archetype find dispatch")]
def test_unroll_take_single_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_unroll_take_single_fold)
        t |> success(func != null, "RTTI must resolve target_unroll_take_single_fold")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        t |> equal(describe_count(body_expr, "for_each_archetype_find"), 1,
                   "take upgrades single from Each to _find")
    }
}

[export, marker(no_coverage)]
def target_unroll_take_aggregate_fold() : int =>
    _fold(from_decs_template(type<UnrollChainRow>)._select(_.val).take(3).aggregate(0) <| $(a, x) =>
        a + x)

[test(description = "take then aggregate over a decs source matches the reference")]
def test_unroll_take_aggregate_parity(t : T?) {
    fixture_unroll2(10)
    // take(3) of 0..9 → 0,1,2 → 0+0+1+2 = 3
    t |> equal(target_unroll_take_aggregate_fold(), 3, "take+aggregate folds the windowed elements")
}

[test(description = "take then aggregate over a decs source upgrades to the archetype find dispatch")]
def test_unroll_take_aggregate_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_unroll_take_aggregate_fold)
        t |> success(func != null, "RTTI must resolve target_unroll_take_aggregate_fold")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        t |> equal(describe_count(body_expr, "for_each_archetype_find"), 1,
                   "take upgrades aggregate from Each to _find")
    }
}

[export, marker(no_coverage)]
def target_unroll_take_element_at_fold() : int =>
    _fold(from_decs_template(type<UnrollChainRow>)._select(_.val).take(3).element_at(1))

[test(description = "take then element_at over a decs source matches the reference")]
def test_unroll_take_element_at_parity(t : T?) {
    fixture_unroll2(10)
    // take(3) → window 0,1,2; element_at(1) → 1
    t |> equal(target_unroll_take_element_at_fold(), 1, "take+element_at indexes inside the window")
}

[export, marker(no_coverage)]
def target_unroll_take_element_at_or_default_oor_fold() : int {
    // idx 5 is past the take(2) window [0,1] → default<int> == 0
    return _fold(from_decs_template(type<UnrollChainRow>)._select(_.val).take(2).element_at_or_default(5))
}

[test(description = "take then element_at_or_default over a decs source returns the default for an index outside the window")]
def test_unroll_take_element_at_or_default_out_of_window(t : T?) {
    fixture_unroll2(10)
    t |> equal(target_unroll_take_element_at_or_default_oor_fold(), 0,
               "take+element_at_or_default past window → default")
}

[test(description = "a select of a single field then sum over a decs source drops the no-op let binding")]
def test_unroll_select_sum_trivial_let_elision_splice_shape(t : T?) {
    // `_select(_.val).sum()` peels to a single ExprField(decs_tup.val)
    // which the flattener would rewrite to a bare iter var anyway — slice
    // 1 elides the synthetic `let decs_sel_N = car_price` entirely and
    // rewrites finalBind to the iter var, leaving body as `acc +=
    // <iterVar>` with no intermediate binding.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_unroll_select_sum_fold)
        t |> success(func != null, "RTTI must resolve target_unroll_select_sum_fold")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        t |> equal(describe_count(body_expr, "decs_sel"), 0,
                   "trivial single-field _select elided — no synthetic decs_sel binding")
        t |> equal(describe_count(body_expr, "for_each_archetype"), 1,
                   "still exactly one for_each_archetype walk")
    }
}

[test(description = "where, a single-field select and sum over a decs source drop the no-op let binding")]
def test_unroll_where_select_sum_trivial_let_elision_splice_shape(t : T?) {
    // Same elision applies through a where filter:
    // where(_.flag==1).select(_.val).sum() — the select still peels to a
    // single ExprField on decs_tup, so elision fires and the binding is
    // dropped. Asserts the elision is not gated on whether the chain is
    // bare or filtered.
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_unroll_where_select_sum_fold)
        t |> success(func != null, "RTTI must resolve target_unroll_where_select_sum_fold")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        t |> equal(describe_count(body_expr, "decs_sel"), 0,
                   "trivial single-field _select elided after a where_")
    }
}

[test(description = "reverse then take over a decs source sums archetype sizes and skips whole archetypes that lie below the window")]
def test_unroll5d_reverse_take_skip_into_tail_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
                                                 @@target_unroll5d_reverse_take_fold)
        t |> success(func != null, "RTTI must resolve target_unroll5d_reverse_take_fold")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        t |> equal(describe_count(body_expr, "for_each_archetype_find"), 1,
                   "skip-into-tail uses _find for early-exit once buf reaches takeN")
        t |> equal(describe_count(body_expr, "for_each_archetype"), 2,
                   "two walks total: 1 size-sum + 1 _find (describe_count is substring match)")
        // decs_skips is the slice-2 unique skip-counter local
        // (qn(\"decs_skips\", at)). Its presence proves the fast path fired
        // vs. the legacy buffer+reverse_inplace+resize emit.
        t |> success(describe_count(body_expr, "decs_skips") >= 1,
                     "decs_skips local present — slice-2 fast path fired")
    }
}

[decs_template(prefix = "rev2_")]
struct RevTakeMultiArchA {
    id : int
}

[decs_template(prefix = "rev2_b_")]
struct RevTakeMultiArchB {
    bid  : int
    bval : int
}

[test(description = "reverse then take across several archetypes matches the reference")]
def test_reverse_take_multi_archetype_parity(t : T?) {
    // Creates two MATCHING archetypes — both have `rev2_id` so both
    // satisfy `from_decs_template(type<RevTakeMultiArchA>)`. Group A1:
    // just rev2_id. Group A2: rev2_id + extra rev2_b_* components — same
    // query matches, but the extra components land it in a separate
    // archetype. With A1=4 + A2=5 → totalCount=9, take(3) → skip=6: A1
    // (size 4) skipped entirely via the size-sum arm, A2 enters with
    // skipsLeft=2 → drains 2 then pushes 3 then returns true. Exercises
    // both the whole-archetype-skip and partial-archetype skip-counter +
    // early-exit.
    restart()
    for (i in 0..4) {
        create_entity() @(eid, cmp) {
            cmp.eid := eid
            cmp.rev2_id := i
        }
    }
    for (i in 0..5) {
        create_entity() @(eid, cmp) {
            cmp.eid := eid
            cmp.rev2_id := i + 100
            cmp.rev2_b_bid := i + 1000
            cmp.rev2_b_bval := i * 10
        }
    }
    commit()
    let got <- _fold(from_decs_template(type<RevTakeMultiArchA>).reverse().take(3).to_array())
    t |> equal(got.length(), 3, "reverse+take(3) on two matching archetypes returns 3 rows")
    var idSet : table<int>
    for (r in got) {
        idSet |> insert(r.id)
    }
    t |> equal(length(idSet), 3, "all 3 ids are distinct")
    // All returned ids must be from one of the two matching archetypes:
    // A1 (ids 0..3) or A2 (ids 100..104). The exact subset depends on
    // archetype iteration order (a decs-internal detail), so we just
    // check membership.
    for (r in got) {
        let inA1 = r.id >= 0 && r.id < 4
        let inA2 = r.id >= 100 && r.id < 105
        t |> success(inA1 || inA2, "row id {r.id} must be from one of the two matching archetypes")
    }
}

[test(description = "reverse then take beyond the total across archetypes skips nothing")]
def test_reverse_take_skip_zero_when_take_exceeds_total(t : T?) {
    // takeN > totalCount → skip = 0, returns all rows reversed. Exercises
    // the actualTake = totalCount branch and the early-exit not firing
    // (buf never reaches takeN before iteration completes).
    restart()
    for (i in 0..3) {
        create_entity() @(eid, cmp) {
            cmp.eid := eid
            cmp.rev2_id := i
        }
    }
    commit()
    let got <- _fold(from_decs_template(type<RevTakeMultiArchA>).reverse().take(99).to_array())
    t |> equal(got.length(), 3, "take(N) where N > total returns all rows")
}

[test(description = "reverse then take over an empty decs source returns nothing")]
def test_reverse_take_empty_source(t : T?) {
    // No matching archetypes → totalCount = 0 → early-return empty buf
    // before for_each_archetype_find.
    restart()
    commit()
    let got <- _fold(from_decs_template(type<RevTakeMultiArchA>).reverse().take(3).to_array())
    t |> success(empty(got), "empty source returns empty")
}

[decs_template(prefix = "jt_car_")]
struct JoinTestCar {
    id        : int
    dealer_id : int
}

[decs_template(prefix = "jt_dealer_")]
struct JoinTestDealer {
    id   : int
    name : string
}

def private join_test_fixture(numCars, numDealers : int) {
    restart()
    create_entities(numCars) $(_eid : EntityId; i : int; var cmp : ComponentMap) {
        apply_decs_template(cmp,
                            JoinTestCar(id = i + 100, dealer_id = (i % max(numDealers, 1)) + 1))
    }
    create_entities(numDealers) $(_eid : EntityId; i : int; var cmp : ComponentMap) {
        apply_decs_template(cmp, JoinTestDealer(id = i + 1, name = "D{i}"))
    }
}

[test(description = "a join of two decs sources counts the matching pairs")]
def test_decs_join_count_basic(t : T?) {
    // 10 cars × 5 dealers, each car matches exactly one dealer.
    join_test_fixture(10, 5)
    let c = _fold(from_decs_template(type<JoinTestCar>) |> _join(from_decs_template(type<JoinTestDealer>),
                                                                  $(l, r) => l.dealer_id == r.id,
                                                                  $(l, r) => (l.id, r.name))
                                                         |> count())
    t |> equal(c, 10, "every car matches exactly one dealer")
}

[test(description = "a decs join does not count left entities that have no match")]
def test_decs_join_count_unmatched_cars(t : T?) {
    // 5 cars with dealer_id 1..5, only 3 dealers (ids 1..3) — cars whose
    // dealer_id is 4 or 5 (the 4th and 5th car) are unmatched, so 3 of 5
    // cars contribute.
    restart()
    create_entities(5) $(_eid : EntityId; i : int; var cmp : ComponentMap) {
        apply_decs_template(cmp, JoinTestCar(id = i + 100, dealer_id = i + 1))
    }
    create_entities(3) $(_eid : EntityId; i : int; var cmp : ComponentMap) {
        apply_decs_template(cmp, JoinTestDealer(id = i + 1, name = "D{i}"))
    }
    let c = _fold(from_decs_template(type<JoinTestCar>) |> _join(from_decs_template(type<JoinTestDealer>),
                                                                  $(l, r) => l.dealer_id == r.id,
                                                                  $(l, r) => (l.id, r.name))
                                                         |> count())
    t |> equal(c, 3, "only cars with matching dealer ids contribute")
}

[test(description = "a decs join with an empty side yields no pairs")]
def test_decs_join_empty_sides(t : T?) {
    restart()
    let c = _fold(from_decs_template(type<JoinTestCar>) |> _join(from_decs_template(type<JoinTestDealer>),
                                                                  $(l, r) => l.dealer_id == r.id,
                                                                  $(l, r) => (l.id, r.name))
                                                         |> count())
    t |> equal(c, 0, "no entities → 0 pairs")
}

[test(description = "a decs join counts every left entity that matches the same right entity")]
def test_decs_join_many_to_one(t : T?) {
    // 10 cars distributed across 2 dealers — each car matches one dealer;
    // counts 10.
    join_test_fixture(10, 2)
    let c = _fold(from_decs_template(type<JoinTestCar>) |> _join(from_decs_template(type<JoinTestDealer>),
                                                                  $(l, r) => l.dealer_id == r.id,
                                                                  $(l, r) => (l.id, r.name))
                                                         |> count())
    t |> equal(c, 10, "many-cars-to-one-dealer-each yields one pair per car")
}

[test(description = "a decs join with a result projection and to_array returns the projected pairs")]
def test_decs_join_to_array_projection(t : T?) {
    // to_array path: named-tuple projection picks up CarId / DealerName.
    join_test_fixture(6, 3)
    let arr <- _fold(from_decs_template(type<JoinTestCar>) |> _join(from_decs_template(type<JoinTestDealer>),
                                                                     $(l, r) => l.dealer_id == r.id,
                                                                     $(l, r) => (CarId = l.id, DealerName = r.name))
                                                            |> to_array())
    t |> equal(length(arr), 6, "every car matched")
    // Each row's CarId is 100..105, DealerName is D0..D2 in round-robin
    // order. Order isn't guaranteed across archetypes, so check the set.
    var seen : table<int>
    for (row in arr) {
        seen |> insert(row.CarId)
        t |> success(row.DealerName |> starts_with("D"), "dealer name prefixed D")
    }
    t |> equal(length(seen), 6, "all 6 distinct car ids present")
}

typedef UnrollTuple = tuple<val : int; flag : int>

[export, marker(no_coverage)]
def target_bare_last_fold() : UnrollTuple => _fold(from_decs_template(type<UnrollChainRow>).last())

[export, marker(no_coverage)]
def target_bare_last_cascade() : UnrollTuple => from_decs_template(type<UnrollChainRow>) |> last()

[export, marker(no_coverage)]
def target_bare_last_or_default_fold() : int =>
    _fold(from_decs_template(type<UnrollChainRow>).last_or_default((val = -1, flag = -1))).val

[test(description = "a bare last over a decs source matches the reference")]
def test_bare_last_parity(t : T?) {
    fixture_unroll2(7)
    let fast = target_bare_last_fold()
    let cascade = target_bare_last_cascade()
    // single archetype = insertion order → last is the 7th entity (val 6)
    t |> equal(fast.val, 6, "bare last() reads the last entity")
    t |> equal(fast.val, cascade.val, "bare last val == tier-2 cascade")
    t |> equal(fast.flag, cascade.flag, "bare last flag == tier-2 cascade")
}

[test(description = "a bare last_or_default over an empty decs source returns the default")]
def test_bare_last_or_default_empty(t : T?) {
    restart()
    commit()
    t |> equal(target_bare_last_or_default_fold(), -1,
               "bare last_or_default empty → explicit default")
}

[test(description = "a bare last_or_default over a non-empty decs source returns the last entity")]
def test_bare_last_or_default_nonempty(t : T?) {
    fixture_unroll2(5)
    t |> equal(target_bare_last_or_default_fold(), 4, "bare last_or_default nonempty → last val")
}

// Eager-eval regression: the default expr must be bound BEFORE
// iteration (matches the walk lane), so its side effects fire even
// when the query is non-empty. A lazy tail-only eval would skip them.
var g_bare_default_evals = 0

def private bare_side_effecting_default() : UnrollTuple {
    g_bare_default_evals++
    return (val = -77, flag = -77)
}

[export, marker(no_coverage)]
def target_bare_last_or_default_sidefx() : int =>
    _fold(from_decs_template(type<UnrollChainRow>).last_or_default(bare_side_effecting_default())).val

[test(description = "the last_or_default default expression is evaluated even when the query is non-empty")]
def test_bare_last_or_default_eager_eval(t : T?) {
    fixture_unroll2(5)
    g_bare_default_evals = 0
    let v = target_bare_last_or_default_sidefx()
    t |> equal(v, 4, "non-empty → last val, not the default")
    t |> equal(g_bare_default_evals, 1, "default expr evaluated eagerly even when query non-empty")
}

// Proves the random-index fast path fired: bare last() emits a single
// void for_each_archetype with NO per-entity inner for-loop (the walk
// path clones the bridge for-loop → count_expr_for >= 1). No
// to_sequence, no _find.
[test(description = "a bare last over a decs source reads the last archetype slot directly with no per-entity loop")]
def test_bare_last_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_bare_last_fold)
        t |> success(func != null, "RTTI must resolve target_bare_last_fold")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        t |> equal(describe_count(body_expr, "to_sequence"), 0,
                   "bare last must NOT call to_sequence")
        t |> equal(describe_count(body_expr, "for_each_archetype_find"), 0,
                   "bare last uses void for_each_archetype")
        t |> equal(describe_count(body_expr, "for_each_archetype"), 1,
                   "exactly one for_each_archetype")
        t |> equal(count_expr_for(body_expr), 0,
                   "random-index last has NO per-entity inner for-loop")
    }
}

// Iterator fallback: a default-init field compiles to get_default_ro
// (an iterator, not an indexable array), so decs_can_random_index →
// false. bare last() / reverse.take must still be correct via the
// walk fallback — checked vs the tier-2 cascade.
[decs_template(prefix = "fb_")]
struct FallbackRow {
    val : int
    hp  : int = 100
}

def private fixture_fallback(n : int) {
    restart()
    create_entities(n) $(_eid : EntityId; i : int; var cmp : ComponentMap) {
        apply_decs_template(cmp, FallbackRow(val = i, hp = i + 1))
    }
}

[export, marker(no_coverage)]
def target_fb_last_fold() : tuple<val : int; hp : int> =>
    _fold(from_decs_template(type<FallbackRow>).last())

[export, marker(no_coverage)]
def target_fb_last_cascade() : tuple<val : int; hp : int> =>
    from_decs_template(type<FallbackRow>) |> last()

[export, marker(no_coverage)]
def target_fb_reverse_take_fold() : array<tuple<val : int; hp : int>> =>
    <- _fold(from_decs_template(type<FallbackRow>).reverse().take(2).to_array())

[export, marker(no_coverage)]
def target_fb_reverse_take_cascade() : array<tuple<val : int; hp : int>> =>
    <- (from_decs_template(type<FallbackRow>) |> reverse() |> take(2) |> to_array())

[test(description = "a bare last falls back to a walk over a decs source with a default-init field and still matches the reference")]
def test_fb_last_parity(t : T?) {
    fixture_fallback(6)
    let fast = target_fb_last_fold()
    let cascade = target_fb_last_cascade()
    // assert the default-init field hp too (= i+1 = 6) — that's what
    // forces the iterator-source fallback
    t |> equal(fast.val, 5, "fallback bare last() correct via walk")
    t |> equal(fast.hp, 6, "fallback bare last() default-init field via walk")
    t |> equal(fast.val, cascade.val, "fallback last val == cascade")
    t |> equal(fast.hp, cascade.hp, "fallback last hp == cascade")
}

[test(description = "reverse then take falls back to a walk over a decs source with a default-init field and still matches the reference")]
def test_fb_reverse_take_parity(t : T?) {
    fixture_fallback(6)
    let fast <- target_fb_reverse_take_fold()
    let cascade <- target_fb_reverse_take_cascade()
    t |> equal(length(fast), 2, "fallback reverse.take(2) → 2 elements")
    t |> equal(length(fast), length(cascade), "fallback fast len == cascade len")
    for (a, b in fast, cascade) {
        t |> equal(a.val, b.val, "fallback reverse take val parity")
        // hp is the default-init field forcing the fallback
        t |> equal(a.hp, b.hp, "fallback reverse take hp parity")
    }
    t |> equal(fast[0].val, 5, "fallback reverse take [0] val")
    t |> equal(fast[0].hp, 6, "fallback reverse take [0] hp")
    t |> equal(fast[1].val, 4, "fallback reverse take [1] val")
    t |> equal(fast[1].hp, 5, "fallback reverse take [1] hp")
}

// The fallback emission must take the walk (clones the bridge
// for-loop) — proven by the presence of a per-entity inner for-loop,
// which the random-index path lacks.
[test(description = "the fallback for a bare last over a default-init field keeps the per-entity walk")]
def test_fb_last_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_fb_last_fold)
        t |> success(func != null, "RTTI must resolve target_fb_last_fold")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        t |> success(count_expr_for(body_expr) >= 1,
                     "fallback last walks → has a per-entity inner for-loop")
    }
}

[decs_template(prefix = "mmg_")]
struct MinMaxGuardVal {
    v : int
}

[test(description = "min_max and min_max_average over a decs source fall back to the unfused path and return tuples")]
def test_decs_unknown_terminator_cascades(t : T?) {
    restart()
    for (i in range(5)) {
        create_entity() @(eid, cmp) {
            cmp.eid := eid
            apply_decs_template(cmp, MinMaxGuardVal(v = 10 + i))
        }
    }
    commit()
    t |> run("min_max family over decs returns tuples, not the element array") @(t : T?) {
        let mm = _fold(from_decs_template(type<MinMaxGuardVal>)._select(_.v) |> min_max())
        t |> equal(mm._0, 10)
        t |> equal(mm._1, 14)
        let mma = _fold(from_decs_template(type<MinMaxGuardVal>)._select(_.v) |> min_max_average())
        t |> equal(mma._0, 10)
        t |> equal(mma._1, 14)
        t |> equal(mma._2, 12)
    }
}
