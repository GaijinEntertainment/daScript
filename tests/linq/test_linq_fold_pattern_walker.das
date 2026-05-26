options gen2

require daslib/ast_boost
require daslib/linq_fold
require dastest/testing_boost public

// PR A — pattern-table refactor: walker + prefix-conflict lint tests.
//
// The per-plan pattern tables (`plan_reverse_patterns`, `plan_distinct_patterns`) are populated by
// `[_macro]` functions at MACRO time, not at runtime — the function pointers in the rows reference
// `[macro_function]` emit fns whose bodies the LLVM JIT can't lower (quote() nodes). So at runtime
// these tables read as empty; the lint helpers below are exercised on synthetic tables instead.
// End-to-end pattern selection is covered by the existing test_linq_fold_*.das suite (each user
// chain exercises the emit fns through `_fold`).

[test]
def test_alias_table_resolves(t : T?) {
    t |> run("alias_table: PR A aliases populated (runtime-initialized via literal)") @(tt : T?) {
        tt |> equal(alias_table |> key_exists("distinct_family"), true)
        tt |> equal(alias_table |> key_exists("first_family"), true)
        tt |> equal(alias_table |> key_exists("count_family"), true)
        tt |> equal(alias_table |> key_exists("distinct_terminator_family"), true)
        tt |> equal(length(alias_table["distinct_family"]), 2)
        tt |> equal(length(alias_table["first_family"]), 2)
        tt |> equal(length(alias_table["distinct_terminator_family"]), 3)   // count / long_count / sum — see linq_fold.das alias_table
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

def null_emit(var c : Captures; var ctx : EmitCtx; at : LineInfo) : Expression? {
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
