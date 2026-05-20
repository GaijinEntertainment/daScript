options gen2
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

// ═══════════════════════════════════════════════════════════════════════════
// Phase 4 sub-PR 4a — plan_decs_eager_bridge splice (count + to_array)
// ═══════════════════════════════════════════════════════════════════════════

// AST-gate helpers — describe-substring counting. Stable for shape assertions:
// the splice's emission is deterministic and we just need to spot known call
// patterns (e.g., `to_sequence`, `for_each_archetype`, `for (`).
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

[decs_template(prefix = "splice4a_")]
struct Splice4a {
    val : int
}

[export, marker(no_coverage)]
def target_splice_count_template_fold() : int {
    return _fold(from_decs_template(type<Splice4a>).count())
}

[export, marker(no_coverage)]
def target_splice_count_from_decs_fold() : int {
    return _fold(from_decs($(splice4a_val : int){}).count())
}

[export, marker(no_coverage)]
def target_splice_to_array_template_fold() : array<tuple<val : int>> {
    return <- _fold(from_decs_template(type<Splice4a>).to_array())
}

[export, marker(no_coverage)]
def target_splice_cascade_select_count_fold() : int {
    // _select() between source and terminator is NOT yet supported by 4a — must cascade.
    return _fold(from_decs_template(type<Splice4a>)._select(_.val).count())
}

[test]
def test_splice_count_template_parity(t : T?) {
    restart()
    for (i in range(7)) {
        create_entity() @(eid, cmp) {
            cmp.eid := eid
            cmp.splice4a_val := i
        }
    }
    commit()
    t |> equal(target_splice_count_template_fold(), 7, "count splice over from_decs_template")
}

[test]
def test_splice_count_from_decs_parity(t : T?) {
    restart()
    for (i in range(4)) {
        create_entity() @(eid, cmp) {
            cmp.eid := eid
            cmp.splice4a_val := i
        }
    }
    commit()
    t |> equal(target_splice_count_from_decs_fold(), 4, "count splice over from_decs(block) form")
}

[test]
def test_splice_to_array_template_parity(t : T?) {
    restart()
    for (i in range(3)) {
        create_entity() @(eid, cmp) {
            cmp.eid := eid
            cmp.splice4a_val := i * 100
        }
    }
    commit()
    let arr <- target_splice_to_array_template_fold()
    t |> equal(arr.length(), 3)
    t |> equal(arr[0].val, 0)
    t |> equal(arr[2].val, 200)
}

[test]
def test_splice_empty_archetype(t : T?) {
    restart()   // no entities created
    commit()
    t |> equal(target_splice_count_template_fold(), 0, "empty count is 0")
    let arr <- target_splice_to_array_template_fold()
    t |> equal(arr.length(), 0, "empty to_array is empty")
}

// AST-shape gates — confirm the splice actually fires (not falling back to eager bridge tier-2).

[test]
def test_splice_count_uses_arch_size_shortcut(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_splice_count_template_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        // count splice: arch.size shortcut, no inner for-loop, no push, no to_sequence.
        t |> equal(describe_count(body_expr, "to_sequence"), 0, "count splice must NOT call to_sequence")
        t |> equal(describe_count(body_expr, "for_each_archetype"), 1, "count splice must emit exactly one for_each_archetype")
        t |> equal(describe_count(body_expr, "for ( "), 0, "count splice with no chain ops must NOT emit an inner for (arch.size shortcut)")
        t |> equal(describe_count(body_expr, ".size"), 1, "count splice must read arch.size")
    }
}

[test]
def test_splice_to_array_no_to_sequence(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_splice_to_array_template_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched && body_expr is ExprInvoke, "expected splice invoke wrapper")
        // to_array splice: skips the to_sequence roundtrip (direct buffer return).
        t |> equal(describe_count(body_expr, "to_sequence"), 0, "to_array splice must NOT call to_sequence")
        t |> equal(describe_count(body_expr, "for_each_archetype"), 1, "to_array splice must emit exactly one for_each_archetype")
        t |> equal(describe_count(body_expr, "for ( "), 1, "to_array splice has exactly one inner multi-iter for (one per archetype)")
    }
}

[test]
def test_splice_unsupported_chain_cascades(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_splice_cascade_select_count_fold)
        if (func == null) return
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            return <- $e(body_expr)
        }
        t |> success(r.matched, "function body must qmatch")
        // _select between source and terminator is NOT supported by 4a — must cascade to tier-2.
        // The eager bridge (to_sequence call) must be present in the final AST.
        t |> success(describe_count(body_expr, "to_sequence") > 0, "_select chain must cascade to tier-2 (eager bridge present)")
    }
}
