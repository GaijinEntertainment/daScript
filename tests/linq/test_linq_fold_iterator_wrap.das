options gen2

require daslib/linq
require daslib/linq_boost
require daslib/linq_fold
require dastest/testing_boost public

// Regression — PR A pattern-table refactor lost the `buffer_return(..., expr._type.isIterator)` wiring on
// every emit fn. All `buffer_return` calls now consult `ctx.expr_is_iterator`, sourced from the outer
// `_fold(...)` expression's type at the stub. Without these wraps, iterator-typed contexts get an array
// where the chain would have produced an iterator — type mismatch.
//
// `typeinfo typename(got)` is the load-bearing assertion — under the bug, the splice returns `array<int>`
// instead of `iterator<int>`. Covers all five no-terminator emit paths:
//   - emit_reverse_buffer_inplace        (catch-all R1-R4 — `reverse`)
//   - emit_hashtable_dedup               (`distinct[_by]`, no take)
//   - emit_reverse_backward_walk_dset_gate  (R-2a — array source, `reverse + distinct[_by]`)
//   - emit_reverse_backward_index_walk      (R6 — array source, `reverse + take`)

[test]
def test_iterwrap_reverse_no_terminator(t : T?) {
    t |> run("reverse() with iterator source, no to_array — splice must return iterator<int>") @(tt : T?) {
        var got <- _fold([1, 2, 3, 4, 5].to_sequence() |> reverse())
        tt |> equal(typeinfo typename(got), "iterator<int>")
        var total = 0
        for (v in got) {
            total += v
        }
        tt |> equal(total, 15)
    }
}

[test]
def test_iterwrap_distinct_no_terminator(t : T?) {
    t |> run("distinct() with iterator source, no to_array — splice must return iterator<int>") @(tt : T?) {
        var got <- _fold([1, 2, 1, 3, 2, 3, 1].to_sequence() |> distinct())
        tt |> equal(typeinfo typename(got), "iterator<int>")
        var seen : array<int>
        for (v in got) {
            seen |> push(v)
        }
        tt |> equal(length(seen), 3)
    }
}

[test]
def test_iterwrap_distinct_by_no_terminator(t : T?) {
    t |> run("distinct_by(_) with iterator source, no to_array — splice must return iterator<int>") @(tt : T?) {
        var got <- _fold([10, 21, 32, 11, 23, 30, 13].to_sequence()._distinct_by(_ % 10))
        tt |> equal(typeinfo typename(got), "iterator<int>")
        var count = 0
        for (_v in got) {
            count ++
        }
        tt |> equal(count, 4)   // mod-10 keys: {0,1,2,3} from 10,21,32,_,23,_,_
    }
}

// ─── R-2a / R6 — array-source paths through emit_reverse_backward_walk_dset_gate / emit_reverse_backward_index_walk ───

[test]
def test_iterwrap_reverse_distinct_array_source(t : T?) {
    t |> run("each(arr) |> reverse() |> distinct() — array-source R-2a path returns iterator") @(tt : T?) {
        let arr = [3, 1, 2, 1, 3, 2]
        var got <- _fold(each(arr) |> reverse() |> distinct())
        tt |> equal(typeinfo typename(got), "iterator<int>")
        var collected : array<int>
        for (v in got) {
            collected |> push(v)
        }
        // reverse → [2,3,1,2,1,3]; distinct keeps first-seen → [2,3,1]
        tt |> equal(length(collected), 3)
        tt |> equal(collected[0], 2)
        tt |> equal(collected[1], 3)
        tt |> equal(collected[2], 1)
    }
}

[test]
def test_iterwrap_reverse_take_array_source(t : T?) {
    t |> run("each(arr) |> reverse() |> take(N) — array-source R6 path returns iterator") @(tt : T?) {
        let arr = [10, 20, 30, 40, 50]
        var got <- _fold(each(arr) |> reverse() |> take(2))
        tt |> equal(typeinfo typename(got), "iterator<int>")
        var collected : array<int>
        for (v in got) {
            collected |> push(v)
        }
        // reverse → [50,40,30,20,10]; take(2) → [50, 40]
        tt |> equal(length(collected), 2)
        tt |> equal(collected[0], 50)
        tt |> equal(collected[1], 40)
    }
}
