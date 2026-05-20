options gen2
options rtti
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

[test]
def test_from_decs(t : T?) {
    for (i in range(3)) {
        create_entity() @(eid, cmp) {
            cmp.eid := eid
            cmp.index := i
            cmp.text := "{i}"
        }
    }
    commit()
    t |> run("from_decs simple") @(t : T?) {
        var qq = (
            from_decs($(index : int; text : string){})
                ._where(_.index < 2)
                .reverse()
                .to_array()
        )
        t |> equal(qq.length(), 2)
        var expected = [
            (index = 1, text = "1"),
            (index = 0, text = "0")
        ]
        for (c, e in qq, expected) {
            t |> equal(c.index, e.index)
            t |> equal(c.text, e.text)
        }
    }
}

[decs_template(prefix = "ftd_default_")]
struct FromTemplateDefault {
    index : int
    text  : string
}

[decs_template]
struct FromTemplateImplicit {
    iv : int
    sv : string
}

[decs_template(prefix)]
struct FromTemplateEmpty {
    ev_index : int
    ev_text  : string
}

[test]
def test_from_decs_template(t : T?) {
    restart()
    for (i in range(3)) {
        create_entity() @(eid, cmp) {
            cmp.eid := eid
            cmp.ftd_default_index := i
            cmp.ftd_default_text := "ftd{i}"
            cmp.FromTemplateImplicit_iv := i * 10
            cmp.FromTemplateImplicit_sv := "imp{i}"
            cmp.ev_index := i * 100
            cmp.ev_text := "emp{i}"
        }
    }
    commit()
    t |> run("from_decs_template custom prefix parity") @(tt : T?) {
        var via_template = (
            from_decs_template(type<FromTemplateDefault>)
                ._where(_.index < 2)
                .reverse()
                .to_array()
        )
        var via_explicit = (
            from_decs($(ftd_default_index : int; ftd_default_text : string){})
                ._where(_.ftd_default_index < 2)
                .reverse()
                .to_array()
        )
        tt |> equal(via_template.length(), via_explicit.length())
        tt |> equal(via_template.length(), 2)
        for (a, b in via_template, via_explicit) {
            tt |> equal(a.index, b.ftd_default_index)
            tt |> equal(a.text, b.ftd_default_text)
        }
    }
    t |> run("from_decs_template default-prefix parity (struct name)") @(tt : T?) {
        let via_template = (
            from_decs_template(type<FromTemplateImplicit>)._select(_.iv).sum()
        )
        let via_explicit = (
            from_decs($(FromTemplateImplicit_iv : int; FromTemplateImplicit_sv : string){})._select(_.FromTemplateImplicit_iv).sum()
        )
        tt |> equal(via_template, via_explicit)
        tt |> equal(via_template, 0 + 10 + 20)
    }
    t |> run("from_decs_template empty-prefix parity") @(tt : T?) {
        let via_template = (
            from_decs_template(type<FromTemplateEmpty>).count()
        )
        let via_explicit = (
            from_decs($(ev_index : int; ev_text : string){}).count()
        )
        tt |> equal(via_template, via_explicit)
        tt |> equal(via_template, 3)
    }
    t |> run("from_decs_template projection by field name") @(tt : T?) {
        let sum_idx = (
            from_decs_template(type<FromTemplateDefault>)._select(_.index).sum()
        )
        tt |> equal(sum_idx, 0 + 1 + 2)
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Approach Z splice (plan_decs_unroll) — Slice 1: bare count via arch.size
// ─────────────────────────────────────────────────────────────────────────────

[decs_template(prefix = "unroll_")]
struct UnrollCountRow {
    val : int
}

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

// Structural ExprFor counter — robust against describe() formatting changes (Copilot review #2).
def private count_expr_for(expr : Expression?) : int {
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

[export, marker(no_coverage)]
def target_unroll_count_fold() : int {
    return _fold(from_decs_template(type<UnrollCountRow>).count())
}

[test]
def test_unroll_count_parity(t : T?) {
    restart()
    for (i in range(7)) {
        create_entity() @(eid, cmp) {
            cmp.eid := eid
            cmp.unroll_val := i
        }
    }
    commit()
    t |> equal(target_unroll_count_fold(), 7, "Slice 1 count splice via arch.size")
}

[test]
def test_unroll_count_empty(t : T?) {
    restart()
    commit()
    t |> equal(target_unroll_count_fold(), 0, "empty archetypes count to 0")
}

[test]
def test_unroll_count_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_unroll_count_fold)
        t |> success(func != null, "RTTI must resolve target_unroll_count_fold")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        // arch.size shortcut: no inner for-loop (per-entity walk), no to_sequence.
        t |> equal(describe_count(body_expr, "to_sequence"), 0, "count splice must NOT call to_sequence")
        t |> equal(describe_count(body_expr, "for_each_archetype"), 1, "count splice emits exactly one for_each_archetype")
        t |> equal(count_expr_for(body_expr), 0, "count splice with no chain ops must NOT emit inner per-entity for-loop")
        t |> equal(describe_count(body_expr, ".size"), 1, "count splice reads arch.size")
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Slice 2: chain-aware count/long_count/sum via for_each_archetype + named-tuple bind
// ─────────────────────────────────────────────────────────────────────────────

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
def target_unroll_select_sum_fold() : int {
    return _fold(from_decs_template(type<UnrollChainRow>)._select(_.val).sum())
}

[export, marker(no_coverage)]
def target_unroll_where_count_fold() : int {
    return _fold(from_decs_template(type<UnrollChainRow>)._where(_.flag == 1).count())
}

[export, marker(no_coverage)]
def target_unroll_where_select_sum_fold() : int {
    return _fold(from_decs_template(type<UnrollChainRow>)._where(_.flag == 1)._select(_.val).sum())
}

[export, marker(no_coverage)]
def target_unroll_long_count_fold() : int64 {
    return _fold(from_decs_template(type<UnrollChainRow>).long_count())
}

[test]
def test_unroll_select_sum_parity(t : T?) {
    fixture_unroll2(5)
    // 0+1+2+3+4 = 10
    t |> equal(target_unroll_select_sum_fold(), 10, "select+sum splice parity")
}

[test]
def test_unroll_where_count_parity(t : T?) {
    fixture_unroll2(5)
    // flags: 0,1,0,1,0 → flag==1 hits 2 rows
    t |> equal(target_unroll_where_count_fold(), 2, "where+count splice parity")
}

[test]
def test_unroll_where_select_sum_parity(t : T?) {
    fixture_unroll2(5)
    // flag==1 rows: vals 1,3 → sum 4
    t |> equal(target_unroll_where_select_sum_fold(), 4, "where+select+sum splice parity")
}

[test]
def test_unroll_long_count_parity(t : T?) {
    fixture_unroll2(7)
    t |> equal(target_unroll_long_count_fold(), 7l, "long_count splice parity")
}

[test]
def test_unroll_select_sum_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_unroll_select_sum_fold)
        t |> success(func != null, "RTTI must resolve target_unroll_select_sum_fold")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        t |> equal(describe_count(body_expr, "to_sequence"), 0, "select+sum splice must NOT call to_sequence")
        t |> equal(describe_count(body_expr, "for_each_archetype"), 1, "exactly one for_each_archetype")
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Slice 3a: min/max/average accumulator extensions
// ─────────────────────────────────────────────────────────────────────────────

[export, marker(no_coverage)]
def target_unroll_min_fold() : int {
    return _fold(from_decs_template(type<UnrollChainRow>)._select(_.val).min())
}

[export, marker(no_coverage)]
def target_unroll_max_fold() : int {
    return _fold(from_decs_template(type<UnrollChainRow>)._select(_.val).max())
}

[export, marker(no_coverage)]
def target_unroll_average_fold() : double {
    return _fold(from_decs_template(type<UnrollChainRow>)._select(_.val).average())
}

[export, marker(no_coverage)]
def target_unroll_where_max_fold() : int {
    return _fold(from_decs_template(type<UnrollChainRow>)._where(_.flag == 1)._select(_.val).max())
}

[test]
def test_unroll_min_parity(t : T?) {
    fixture_unroll2(5)
    // vals 0,1,2,3,4 → min = 0
    t |> equal(target_unroll_min_fold(), 0, "min splice parity")
}

[test]
def test_unroll_max_parity(t : T?) {
    fixture_unroll2(5)
    // vals 0,1,2,3,4 → max = 4
    t |> equal(target_unroll_max_fold(), 4, "max splice parity")
}

[test]
def test_unroll_average_parity(t : T?) {
    fixture_unroll2(5)
    // vals 0,1,2,3,4 → avg = 10/5 = 2.0
    t |> equal(target_unroll_average_fold(), 2.0lf, "average splice parity")
}

[test]
def test_unroll_average_empty_returns_zero(t : T?) {
    // Regression for emit_decs_accumulator.average — empty archetype set must return 0.0lf (matches linq.das:1545 cnt==0 guard), not NaN from 0.0lf / 0.0lf.
    fixture_unroll2(0)
    t |> equal(target_unroll_average_fold(), 0.0lf, "average splice empty-source guard")
}

[test]
def test_unroll_where_max_parity(t : T?) {
    fixture_unroll2(5)
    // flag==1 rows: vals 1,3 → max 3
    t |> equal(target_unroll_where_max_fold(), 3, "where+select+max splice parity")
}

[test]
def test_unroll_max_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_unroll_max_fold)
        t |> success(func != null, "RTTI must resolve target_unroll_max_fold")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        t |> equal(describe_count(body_expr, "to_sequence"), 0, "max splice must NOT call to_sequence")
        t |> equal(describe_count(body_expr, "for_each_archetype"), 1, "exactly one for_each_archetype")
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Slice 3b: early-exit terminators (first/first_or_default/any/all/contains)
// ─────────────────────────────────────────────────────────────────────────────

[export, marker(no_coverage)]
def target_unroll_any_bare_fold() : bool {
    return _fold(from_decs_template(type<UnrollChainRow>).any())
}

[export, marker(no_coverage)]
def target_unroll_any_pred_fold() : bool {
    return _fold(from_decs_template(type<UnrollChainRow>)._select(_.val)._any(_ > 100))
}

[export, marker(no_coverage)]
def target_unroll_all_fold() : bool {
    return _fold(from_decs_template(type<UnrollChainRow>)._select(_.val)._all(_ >= 0))
}

[export, marker(no_coverage)]
def target_unroll_contains_fold() : bool {
    return _fold(from_decs_template(type<UnrollChainRow>)._select(_.val).contains(3))
}

[export, marker(no_coverage)]
def target_unroll_first_fold() : int {
    return _fold(from_decs_template(type<UnrollChainRow>)._where(_.flag == 1)._select(_.val).first())
}

[export, marker(no_coverage)]
def target_unroll_first_or_default_fold() : int {
    return _fold(from_decs_template(type<UnrollChainRow>)._where(_.val > 1000)._select(_.val).first_or_default(-7))
}

[test]
def test_unroll_any_bare_parity(t : T?) {
    fixture_unroll2(3)
    t |> equal(target_unroll_any_bare_fold(), true, "bare any with entities")
    restart()
    commit()
    t |> equal(target_unroll_any_bare_fold(), false, "bare any with no entities")
}

[test]
def test_unroll_any_pred_parity(t : T?) {
    fixture_unroll2(5)
    // vals 0..4, none > 100
    t |> equal(target_unroll_any_pred_fold(), false, "any(pred) — no match")
    fixture_unroll2(150)
    // vals 0..149, some > 100
    t |> equal(target_unroll_any_pred_fold(), true, "any(pred) — has match")
}

[test]
def test_unroll_all_parity(t : T?) {
    fixture_unroll2(5)
    t |> equal(target_unroll_all_fold(), true, "all(>=0) on 0..4")
}

[test]
def test_unroll_contains_parity(t : T?) {
    fixture_unroll2(5)
    t |> equal(target_unroll_contains_fold(), true, "contains(3) in 0..4")
    fixture_unroll2(2)
    t |> equal(target_unroll_contains_fold(), false, "contains(3) in 0..1")
}

[test]
def test_unroll_first_parity(t : T?) {
    fixture_unroll2(5)
    // flag==1 first: i=1 → val=1
    t |> equal(target_unroll_first_fold(), 1, "first(where flag==1)")
}

[test]
def test_unroll_first_or_default_parity(t : T?) {
    fixture_unroll2(5)
    // val>1000 → none match → default -7
    t |> equal(target_unroll_first_or_default_fold(), -7, "first_or_default empty match")
}

[test]
def test_unroll_first_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_unroll_first_fold)
        t |> success(func != null, "RTTI must resolve target_unroll_first_fold")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        t |> equal(describe_count(body_expr, "to_sequence"), 0, "first splice must NOT call to_sequence")
        t |> equal(describe_count(body_expr, "for_each_archetype_find"), 1, "first splice uses for_each_archetype_find")
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Slice 3c: to_array buffer terminator
// ─────────────────────────────────────────────────────────────────────────────

[export, marker(no_coverage)]
def target_unroll_to_array_fold() : array<int> {
    return <- _fold(from_decs_template(type<UnrollChainRow>)._where(_.flag == 1)._select(_.val).to_array())
}

[test]
def test_unroll_to_array_parity(t : T?) {
    fixture_unroll2(6)
    // flag==1 rows: i=1,3,5 → vals 1,3,5
    let got <- target_unroll_to_array_fold()
    t |> equal(got |> length, 3, "to_array length")
    if (length(got) == 3) {
        t |> equal(got[0], 1, "to_array[0]")
        t |> equal(got[1], 3, "to_array[1]")
        t |> equal(got[2], 5, "to_array[2]")
    }
}

[test]
def test_unroll_to_array_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_unroll_to_array_fold)
        t |> success(func != null, "RTTI must resolve target_unroll_to_array_fold")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        t |> equal(describe_count(body_expr, "to_sequence"), 0, "to_array splice must NOT call to_sequence")
        t |> equal(describe_count(body_expr, "for_each_archetype"), 1, "exactly one for_each_archetype")
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Slice 4: chained _select + interleaved _where + _count(pred) + _min_by / _max_by
// ─────────────────────────────────────────────────────────────────────────────

[export, marker(no_coverage)]
def target_unroll_chained_select_sum_fold() : int {
    return _fold(from_decs_template(type<UnrollChainRow>)._select(_.val)._select(_ * 10).sum())
}

[export, marker(no_coverage)]
def target_unroll_select_then_where_sum_fold() : int {
    return _fold(from_decs_template(type<UnrollChainRow>)._select(_.val)._where(_ > 1).sum())
}

[export, marker(no_coverage)]
def target_unroll_where_select_where_sum_fold() : int {
    return _fold(from_decs_template(type<UnrollChainRow>)._where(_.flag == 1)._select(_.val)._where(_ > 1).sum())
}

[export, marker(no_coverage)]
def target_unroll_count_pred_fold() : int {
    return _fold(from_decs_template(type<UnrollChainRow>)._count(_.val > 2))
}

[export, marker(no_coverage)]
def target_unroll_long_count_pred_fold() : int64 {
    return _fold(from_decs_template(type<UnrollChainRow>)._long_count(_.val >= 3))
}

[export, marker(no_coverage)]
def target_unroll_chained_select_to_array_fold() : array<int> {
    return <- _fold(from_decs_template(type<UnrollChainRow>)._select(_.val)._select(_ + 100).to_array())
}

[export, marker(no_coverage)]
def target_unroll_min_by_fold() : int {
    return _fold(from_decs_template(type<UnrollChainRow>)._select(_.val)._min_by(-_))
}

[export, marker(no_coverage)]
def target_unroll_max_by_fold() : int {
    return _fold(from_decs_template(type<UnrollChainRow>)._select(_.val)._max_by(-_))
}

[test]
def test_unroll_chained_select_sum_parity(t : T?) {
    fixture_unroll2(5)
    // (0+1+2+3+4)*10 = 100
    t |> equal(target_unroll_chained_select_sum_fold(), 100, "chained select+sum splice parity")
}

[test]
def test_unroll_select_then_where_sum_parity(t : T?) {
    fixture_unroll2(5)
    // select val → 0,1,2,3,4 → where >1 → 2,3,4 → sum 9
    t |> equal(target_unroll_select_then_where_sum_fold(), 9, "select→where→sum splice parity")
}

[test]
def test_unroll_where_select_where_sum_parity(t : T?) {
    fixture_unroll2(6)
    // flag==1 rows: i=1,3,5 (vals 1,3,5) → select val → 1,3,5 → where >1 → 3,5 → sum 8
    t |> equal(target_unroll_where_select_where_sum_parity_compute(), 8, "where→select→where→sum splice parity")
}

def private target_unroll_where_select_where_sum_parity_compute() : int {
    return target_unroll_where_select_where_sum_fold()
}

[test]
def test_unroll_count_pred_parity(t : T?) {
    fixture_unroll2(6)
    // vals 0..5, pred val>2 → 3,4,5 → 3 hits
    t |> equal(target_unroll_count_pred_fold(), 3, "_count(pred) splice parity")
}

[test]
def test_unroll_long_count_pred_parity(t : T?) {
    fixture_unroll2(6)
    // vals 0..5, pred val>=3 → 3,4,5 → 3 hits
    t |> equal(target_unroll_long_count_pred_fold(), 3l, "_long_count(pred) splice parity")
}

[test]
def test_unroll_chained_select_to_array_parity(t : T?) {
    fixture_unroll2(3)
    // select val → 0,1,2 → select +100 → 100,101,102
    let got <- target_unroll_chained_select_to_array_fold()
    t |> equal(got |> length, 3, "chained to_array length")
    if (length(got) == 3) {
        t |> equal(got[0], 100, "[0]")
        t |> equal(got[1], 101, "[1]")
        t |> equal(got[2], 102, "[2]")
    }
}

[test]
def test_unroll_min_by_parity(t : T?) {
    fixture_unroll2(5)
    // select val → 0,1,2,3,4; min by -val → max val → 4
    t |> equal(target_unroll_min_by_fold(), 4, "_min_by splice parity")
}

[test]
def test_unroll_max_by_parity(t : T?) {
    fixture_unroll2(5)
    // select val → 0,1,2,3,4; max by -val → min val → 0
    t |> equal(target_unroll_max_by_fold(), 0, "_max_by splice parity")
}

[test]
def test_unroll_chained_select_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_unroll_chained_select_sum_fold)
        t |> success(func != null, "RTTI must resolve target_unroll_chained_select_sum_fold")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        t |> equal(describe_count(body_expr, "to_sequence"), 0, "chained-select sum splice must NOT call to_sequence")
        t |> equal(describe_count(body_expr, "for_each_archetype"), 1, "exactly one for_each_archetype")
    }
}

[test]
def test_unroll_count_pred_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_unroll_count_pred_fold)
        t |> success(func != null, "RTTI must resolve target_unroll_count_pred_fold")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        t |> equal(describe_count(body_expr, "to_sequence"), 0, "_count(pred) splice must NOT call to_sequence")
        t |> equal(describe_count(body_expr, "for_each_archetype"), 1, "exactly one for_each_archetype")
    }
}

[test]
def test_unroll_min_by_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_unroll_min_by_fold)
        t |> success(func != null, "RTTI must resolve target_unroll_min_by_fold")
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        t |> equal(describe_count(body_expr, "to_sequence"), 0, "_min_by splice must NOT call to_sequence")
        t |> equal(describe_count(body_expr, "for_each_archetype"), 1, "exactly one for_each_archetype")
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Slice 5a: take/skip ranges on decs (counters hoisted at invoke scope; take present routes to for_each_archetype_find with bool block returning true on cap-hit)
// ─────────────────────────────────────────────────────────────────────────────

[export, marker(no_coverage)]
def target_unroll_take_count_fold() : int {
    return _fold(from_decs_template(type<UnrollChainRow>).take(3).count())
}

[export, marker(no_coverage)]
def target_unroll_skip_count_fold() : int {
    return _fold(from_decs_template(type<UnrollChainRow>).skip(2).count())
}

[export, marker(no_coverage)]
def target_unroll_skip_take_count_fold() : int {
    return _fold(from_decs_template(type<UnrollChainRow>).skip(1).take(2).count())
}

[export, marker(no_coverage)]
def target_unroll_where_take_count_fold() : int {
    return _fold(from_decs_template(type<UnrollChainRow>)._where(_.flag == 1).take(2).count())
}

[export, marker(no_coverage)]
def target_unroll_select_take_sum_fold() : int {
    return _fold(from_decs_template(type<UnrollChainRow>)._select(_.val).take(3).sum())
}

[export, marker(no_coverage)]
def target_unroll_take_first_fold() : int {
    return _fold(from_decs_template(type<UnrollChainRow>)._select(_.val).take(2).first())
}

[export, marker(no_coverage)]
def target_unroll_take_neg_count_fold() : int {
    // take(-N) must short-circuit (>=-guard) — empty iteration regardless of source size.
    return _fold(from_decs_template(type<UnrollChainRow>).take(-1).count())
}

[export, marker(no_coverage)]
def target_unroll_skip_beyond_count_fold() : int {
    return _fold(from_decs_template(type<UnrollChainRow>).skip(100).count())
}

[export, marker(no_coverage)]
def target_unroll_take_to_array_fold() : array<int> {
    return <- _fold(from_decs_template(type<UnrollChainRow>)._select(_.val).take(3).to_array())
}

[test]
def test_unroll_take_count_parity(t : T?) {
    fixture_unroll2(7)
    t |> equal(target_unroll_take_count_fold(), 3, "take(3) caps total")
}

[test]
def test_unroll_skip_count_parity(t : T?) {
    fixture_unroll2(7)
    t |> equal(target_unroll_skip_count_fold(), 5, "skip(2) drops first 2 across archetypes")
}

[test]
def test_unroll_skip_take_count_parity(t : T?) {
    fixture_unroll2(7)
    t |> equal(target_unroll_skip_take_count_fold(), 2, "skip(1)+take(2) windows total count")
}

[test]
def test_unroll_where_take_count_parity(t : T?) {
    fixture_unroll2(7)
    // flag==1 rows are vals 1,3,5 → take(2) → 2 matches
    t |> equal(target_unroll_where_take_count_fold(), 2, "where+take caps post-filter count")
}

[test]
def test_unroll_select_take_sum_parity(t : T?) {
    fixture_unroll2(7)
    // vals 0,1,2,...,6 → take(3) → sum 0+1+2 = 3
    t |> equal(target_unroll_select_take_sum_fold(), 3, "select+take+sum splice parity")
}

[test]
def test_unroll_take_first_parity(t : T?) {
    fixture_unroll2(7)
    // take(2) then first() — first wins before take counter ticks past 1
    t |> equal(target_unroll_take_first_fold(), 0, "take+first returns first element")
}

[test]
def test_unroll_take_neg_zero_count(t : T?) {
    fixture_unroll2(7)
    t |> equal(target_unroll_take_neg_count_fold(), 0, "take(-1) short-circuits to 0")
}

[test]
def test_unroll_skip_beyond_returns_zero(t : T?) {
    fixture_unroll2(7)
    t |> equal(target_unroll_skip_beyond_count_fold(), 0, "skip past end yields 0 count")
}

[test]
def test_unroll_take_to_array_parity(t : T?) {
    fixture_unroll2(7)
    let got <- target_unroll_take_to_array_fold()
    t |> equal(length(got), 3, "take(3).to_array() yields 3 elements")
    if (length(got) >= 3) {
        t |> equal(got[0], 0, "first taken element")
        t |> equal(got[1], 1, "second taken element")
        t |> equal(got[2], 2, "third taken element")
    }
}

[test]
def test_unroll_take_count_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_unroll_take_count_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        t |> equal(describe_count(body_expr, "to_sequence"), 0, "take+count splice must NOT call to_sequence")
        // take present → must use the _find variant so early-exit propagates across archetypes.
        t |> equal(describe_count(body_expr, "for_each_archetype_find"), 1, "take splice routes to for_each_archetype_find")
    }
}

[test]
def test_unroll_skip_count_splice_shape(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_unroll_skip_count_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        t |> equal(describe_count(body_expr, "to_sequence"), 0, "skip+count splice must NOT call to_sequence")
        // skip-only (no take) keeps the void for_each_archetype variant.
        t |> equal(describe_count(body_expr, "for_each_archetype_find"), 0, "skip-only splice does NOT route to _find")
        t |> equal(describe_count(body_expr, "for_each_archetype"), 1, "skip-only splice uses regular for_each_archetype")
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Slice 5a Copilot review #1 regression guards — take + any/all/contains must
// distinguish take-cap (inner `return true`) from real match. Without explicit
// foundName/foundCounter state these would false-positive on empty/no-match
// sources whenever the take-cap fires before a real perElement decision.
// ─────────────────────────────────────────────────────────────────────────────

[export, marker(no_coverage)]
def target_unroll_take_any_no_pred_fold() : bool {
    return _fold(from_decs_template(type<UnrollChainRow>).take(0).any())
}

[export, marker(no_coverage)]
def target_unroll_take_any_pred_no_match_fold() : bool {
    return _fold(from_decs_template(type<UnrollChainRow>)._select(_.val).take(3)._any(_ > 100))
}

[export, marker(no_coverage)]
def target_unroll_take_any_pred_match_fold() : bool {
    return _fold(from_decs_template(type<UnrollChainRow>)._select(_.val).take(3)._any(_ == 1))
}

[export, marker(no_coverage)]
def target_unroll_take_all_pass_fold() : bool {
    // All val >= 0 holds; take(3) shouldn't fool `all` into returning false from the take-cap exit.
    return _fold(from_decs_template(type<UnrollChainRow>)._select(_.val).take(3)._all(_ >= 0))
}

[export, marker(no_coverage)]
def target_unroll_take_all_fail_fold() : bool {
    // Some val > 1 → `all <= 1` false.
    return _fold(from_decs_template(type<UnrollChainRow>)._select(_.val).take(3)._all(_ <= 1))
}

[export, marker(no_coverage)]
def target_unroll_take_contains_hit_fold() : bool {
    return _fold(from_decs_template(type<UnrollChainRow>)._select(_.val).take(5).contains(2))
}

[export, marker(no_coverage)]
def target_unroll_take_contains_miss_fold() : bool {
    return _fold(from_decs_template(type<UnrollChainRow>)._select(_.val).take(2).contains(100))
}

[test]
def test_unroll_take_any_no_pred_empty(t : T?) {
    fixture_unroll2(7)
    // take(0).any() on non-empty source → false (the prefix-of-length-0 is empty).
    t |> equal(target_unroll_take_any_no_pred_fold(), false, "take(0).any() returns false on non-empty source")
}

[test]
def test_unroll_take_any_pred_no_match(t : T?) {
    fixture_unroll2(7)
    // take(3) gives vals 0,1,2; none > 100 → false. take-cap fires at element 3 but must not be mistaken for a match.
    t |> equal(target_unroll_take_any_pred_no_match_fold(), false, "take(3).any(_ > 100) returns false when no match in prefix")
}

[test]
def test_unroll_take_any_pred_match(t : T?) {
    fixture_unroll2(7)
    // take(3) gives vals 0,1,2; matches _==1 at element 1 → true.
    t |> equal(target_unroll_take_any_pred_match_fold(), true, "take(3).any(_ == 1) returns true when match in prefix")
}

[test]
def test_unroll_take_all_pass(t : T?) {
    fixture_unroll2(7)
    // vals 0,1,2,...,6 all >= 0; take(3) gives 0,1,2 all pass → true. Take-cap fire at element 3 must not be mistaken for "counterexample found".
    t |> equal(target_unroll_take_all_pass_fold(), true, "take(3).all(_ >= 0) returns true when all prefix elements pass")
}

[test]
def test_unroll_take_all_fail(t : T?) {
    fixture_unroll2(7)
    // take(3) gives 0,1,2; element 2 fails `_ <= 1` → false.
    t |> equal(target_unroll_take_all_fail_fold(), false, "take(3).all(_ <= 1) returns false when prefix contains counterexample")
}

[test]
def test_unroll_take_contains_hit(t : T?) {
    fixture_unroll2(7)
    // take(5) gives 0..4; contains(2) → true.
    t |> equal(target_unroll_take_contains_hit_fold(), true, "take(5).contains(2) returns true when value in prefix")
}

[test]
def test_unroll_take_contains_miss(t : T?) {
    fixture_unroll2(7)
    // take(2) gives 0,1; contains(100) → false. take-cap fires at element 2 but must not be mistaken for a hit.
    t |> equal(target_unroll_take_contains_miss_fold(), false, "take(2).contains(100) returns false when value not in prefix")
}

// Slice 5a Copilot review #2 regression guard — take(N) expression must evaluate
// exactly once per invoke (matches take(src, total:int) call-site semantics).
// Side-effect counter proves the hoist works.

var private take_n_eval_counter : int = 0

def private take_n_with_side_effect() : int {
    take_n_eval_counter ++
    return 3
}

[export, marker(no_coverage)]
def target_unroll_take_with_side_effect_fold() : int {
    return _fold(from_decs_template(type<UnrollChainRow>).take(take_n_with_side_effect()).count())
}

[test]
def test_unroll_take_n_evaluated_once(t : T?) {
    fixture_unroll2(7)
    take_n_eval_counter = 0
    let c = target_unroll_take_with_side_effect_fold()
    t |> equal(c, 3, "take(3).count() returns 3")
    // Single hoisted evaluation — NOT once per element across 7 entities.
    t |> equal(take_n_eval_counter, 1, "take(N) expression must evaluate exactly once per invoke")
}
