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
        t |> equal(describe_count(body_expr, "for ( "), 0, "count splice with no chain ops must NOT emit inner per-entity for-loop")
        t |> equal(describe_count(body_expr, ".size"), 1, "count splice reads arch.size")
    }
}
