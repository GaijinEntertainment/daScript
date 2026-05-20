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
