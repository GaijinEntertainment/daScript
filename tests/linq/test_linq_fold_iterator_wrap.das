options gen2

require daslib/linq
require daslib/linq_boost
require daslib/linq_fold
require dastest/testing_boost public

// Regression — PR A pattern-table refactor lost the `buffer_return(..., expr._type.isIterator)` wiring on
// `emit_reverse_buffer_inplace` and `emit_hashtable_dedup`. Both now consult `ctx.expr_is_iterator`, sourced
// from the outer `_fold(...)` expression's type at the stub. Without these wraps, `_fold(seq |> reverse())`
// and `_fold(seq.distinct())` (no `to_array`) emit array where the chain emits iterator — type mismatch.
//
// `typeinfo typename(got)` is the load-bearing assertion — under the bug, the splice returns `array<int>`,
// the typename check fails (or the move-assign fails earlier at compile time depending on context).

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
