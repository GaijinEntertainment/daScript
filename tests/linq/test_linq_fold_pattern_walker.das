options gen2

require daslib/ast_boost
require daslib/linq_fold
require dastest/testing_boost public

// PR A — pattern-table refactor: walker + prefix-conflict lint tests.
// Per-archetype emit tests are covered end-to-end by the existing test_linq_fold_*.das suite (each user chain
// exercises the emit fns through `_fold`). This file tests the kernel directly: walker semantics and table integrity.

// ─── Walker — synthetic patterns + synthetic call lists ───
//
// Walker requires real ExprCall/LinqCall pairs in the calls array. Building those at runtime is impractical
// for a unit test (the macro pipeline owns ExprCall construction). The lint/integrity tests below use the
// already-populated per-plan pattern tables, which is enough to validate the table machinery.

[test]
def test_plan_reverse_table_reachable(t : T?) {
    t |> run("plan_reverse_patterns: every pattern is reachable (no strict prefix shadowing)") @(tt : T?) {
        tt |> equal(check_pattern_table_reachable("plan_reverse_patterns", plan_reverse_patterns), true)
    }
}

[test]
def test_plan_distinct_table_reachable(t : T?) {
    t |> run("plan_distinct_patterns: every pattern is reachable (no strict prefix shadowing)") @(tt : T?) {
        tt |> equal(check_pattern_table_reachable("plan_distinct_patterns", plan_distinct_patterns), true)
    }
}

[test]
def test_plan_reverse_table_populated(t : T?) {
    t |> run("plan_reverse_patterns: populated with the 5 PR A rows in expected order") @(tt : T?) {
        tt |> equal(length(plan_reverse_patterns), 5)
        tt |> equal(plan_reverse_patterns[0].name, "reverse_distinct_backward_walk")
        tt |> equal(plan_reverse_patterns[1].name, "reverse_take_backward_index")
        tt |> equal(plan_reverse_patterns[2].name, "reverse_counter")
        tt |> equal(plan_reverse_patterns[3].name, "reverse_walk_overwrite")
        tt |> equal(plan_reverse_patterns[4].name, "reverse_buffer_inplace")
    }
}

[test]
def test_plan_distinct_table_populated(t : T?) {
    t |> run("plan_distinct_patterns: populated with the 2 PR A rows in expected order") @(tt : T?) {
        tt |> equal(length(plan_distinct_patterns), 2)
        tt |> equal(plan_distinct_patterns[0].name, "distinct_take")
        tt |> equal(plan_distinct_patterns[1].name, "distinct_main")
    }
}

[test]
def test_alias_table_resolves(t : T?) {
    t |> run("alias_table: PR A aliases populated") @(tt : T?) {
        tt |> equal(alias_table |> key_exists("distinct_family"), true)
        tt |> equal(alias_table |> key_exists("first_family"), true)
        tt |> equal(alias_table |> key_exists("count_family"), true)
        tt |> equal(alias_table |> key_exists("any_terminator_family"), true)
        tt |> equal(length(alias_table["distinct_family"]), 2)
        tt |> equal(length(alias_table["first_family"]), 2)
    }
}

// ─── Prefix-conflict helper: positive case (synthetic patterns we KNOW shadow) ───

[test]
def test_chain_prefix_of_positive(t : T?) {
    t |> run("chain_prefix_of: A is a strict prefix of B when A's slots structurally match B's first N") @(tt : T?) {
        let prefix : array<Slot> <- [
            Slot(matcher = m_literal("reverse"), cardinality = c_one())
        ]
        let longer : array<Slot> <- [
            Slot(matcher = m_literal("reverse"), cardinality = c_one()),
            Slot(matcher = m_literal("take"), cardinality = c_one(), capture_name = "take")
        ]
        tt |> equal(chain_prefix_of(prefix, longer), true)
        tt |> equal(chain_prefix_of(longer, prefix), false)  // longer is not a prefix of shorter
    }
}

[test]
def test_chain_prefix_of_negative(t : T?) {
    t |> run("chain_prefix_of: different matchers at any position break the prefix") @(tt : T?) {
        let a : array<Slot> <- [
            Slot(matcher = m_literal("reverse"), cardinality = c_one()),
            Slot(matcher = m_literal("count"), cardinality = c_one())
        ]
        let b : array<Slot> <- [
            Slot(matcher = m_literal("reverse"), cardinality = c_one()),
            Slot(matcher = m_literal("take"), cardinality = c_one()),
            Slot(matcher = m_literal("first"), cardinality = c_one())
        ]
        tt |> equal(chain_prefix_of(a, b), false)
    }
}

[test]
def test_chain_prefix_of_alias_equality(t : T?) {
    t |> run("chain_prefix_of: alias matchers compare by alias name") @(tt : T?) {
        let a : array<Slot> <- [
            Slot(matcher = m_alias("distinct_family"), cardinality = c_one())
        ]
        let b : array<Slot> <- [
            Slot(matcher = m_alias("distinct_family"), cardinality = c_one()),
            Slot(matcher = m_literal("take"), cardinality = c_opt())
        ]
        let c : array<Slot> <- [
            Slot(matcher = m_alias("first_family"), cardinality = c_one()),
            Slot(matcher = m_literal("take"), cardinality = c_opt())
        ]
        tt |> equal(chain_prefix_of(a, b), true)
        tt |> equal(chain_prefix_of(a, c), false)
    }
}

def null_emit(var c : Captures; var top : Expression?; src : SourceAdapter; at : LineInfo) : Expression? {
    return null
}

[test]
def test_check_pattern_table_reachable_catches_shadow(t : T?) {
    t |> run("check_pattern_table_reachable: returns false for a table with a strict-prefix shadow") @(tt : T?) {
        var bad : array<SplicePattern>
        bad |> emplace <| SplicePattern(
            name = "short",
            chain <- [
                Slot(matcher = m_literal("reverse"), cardinality = c_one())
            ],
            emit = @@ < EmitFn > null_emit
        )
        bad |> emplace <| SplicePattern(
            name = "long_shadowed",
            chain <- [
                Slot(matcher = m_literal("reverse"), cardinality = c_one()),
                Slot(matcher = m_literal("take"), cardinality = c_opt())
            ],
            emit = @@ < EmitFn > null_emit
        )
        tt |> equal(check_pattern_table_reachable("synthetic-shadowed", bad), false)
    }
}
