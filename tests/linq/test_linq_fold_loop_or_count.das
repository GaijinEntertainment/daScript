options gen2

require daslib/linq
require daslib/linq_boost
require daslib/linq_fold
require dastest/testing_boost public

// PR B1 regression coverage — plan_loop_or_count migrated to pattern-table.
//
// The new architecture: `slot_chain_of(["where_", "select"], "head")` greedy-consumes the
// pre-range head; canonical-order positional slots (skip / skip_while / take_while / take /
// post_take_where / term) carry the rest. The emit fn (emit_loop_or_count_lane) walks
// c.many["head"] applying the same where-after-select / chained-select / AND-merge logic the
// imperative loop did, then dispatches to the same lane emit fns.
//
// These tests assert end-to-end correctness across the lane × head-shape matrix.

// ═════════════════════════════════════════════════════════════════════════════
// 1. Canonical chains — one lane per terminator family
// ═════════════════════════════════════════════════════════════════════════════

[test(description = "where plus count fuses into the counter lane with the right total")]
def test_counter_lane(t : T?) {
    t |> run("counter lane: where + count") @(tt : T?) {
        let arr <- [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
        let n = _fold(each(arr)._where(_ > 3).count())
        tt |> equal(n, 7)
    }
}

[test(description = "select with an implicit to_array fuses into the array lane")]
def test_array_lane(t : T?) {
    t |> run("array lane: select + implicit to_array") @(tt : T?) {
        let arr <- [1, 2, 3, 4, 5]
        let buf <- _fold(each(arr)._select(_ * 10) |> to_array())
        tt |> equal(length(buf), 5)
        tt |> equal(buf[0], 10)
        tt |> equal(buf[4], 50)
    }
}

[test(description = "where, select and sum fuse into the accumulator lane with the right sum")]
def test_accumulator_lane(t : T?) {
    t |> run("accumulator lane: where + select + sum") @(tt : T?) {
        let arr <- [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
        let s = _fold(each(arr)._where(_ > 2)._select(_ * 2).sum())
        // 3+4+5+6+7+8+9+10 = 52; ×2 = 104
        tt |> equal(s, 104)
    }
}

[test(description = "where plus any fuses into the early-exit lane")]
def test_early_exit_lane(t : T?) {
    t |> run("early-exit lane: where + any") @(tt : T?) {
        let arr <- [1, 2, 3, 4, 5]
        let yes = _fold(each(arr)._where(_ > 3).any())
        let no = _fold(each(arr)._where(_ > 100).any())
        tt |> equal(yes, true)
        tt |> equal(no, false)
    }
}

// ═════════════════════════════════════════════════════════════════════════════
// 2. Where-after-select rebinding (single & multiple)
// ═════════════════════════════════════════════════════════════════════════════

[test(description = "a where after a select tests the projected value rather than the raw source element")]
def test_where_after_select(t : T?) {
    t |> run("where(p2) after select(f) sees PROJECTED value, not raw source") @(tt : T?) {
        let arr <- [1, 2, 3, 4, 5]
        // _ > 1 keeps [2,3,4,5]; project ×10 → [20,30,40,50]; then keep _%20==0 → [20,40]; count=2.
        let n = _fold(each(arr)._where(_ > 1)._select(_ * 10)._where(_ % 20 == 0).count())
        tt |> equal(n, 2)
    }
}

[test(description = "two wheres after a select compose into one predicate on the projected value")]
def test_multiple_wheres_post_select(t : T?) {
    t |> run("two wheres after a select: collapse_chained_wheres composes them") @(tt : T?) {
        let arr <- [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
        // ×2 → [2..20], _>5 keeps [6,8,10,12,14,16,18,20], _%4==0 keeps [8,12,16,20] = 4.
        let n = _fold(each(arr)._select(_ * 2)._where(_ > 5)._where(_ % 4 == 0).count())
        tt |> equal(n, 4)
    }
}

// ═════════════════════════════════════════════════════════════════════════════
// 3. Range op chains (skip / take / take_while)
// ═════════════════════════════════════════════════════════════════════════════

[test(description = "where, skip(N), take(M) and count fuse into a counter that applies the range to the survivors")]
def test_skip_take_count(t : T?) {
    t |> run("where + skip(N) + take(M) + count") @(tt : T?) {
        let arr <- [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
        // _ > 0 keeps all; skip 3 → [4..10]; take 4 → [4,5,6,7]; count=4.
        let n = _fold(each(arr)._where(_ > 0).skip(3).take(4).count())
        tt |> equal(n, 4)
    }
}

[test(description = "take_while plus sum streams until the predicate fails and sums the prefix")]
def test_take_while_sum(t : T?) {
    t |> run("take_while + sum: streams until predicate fails") @(tt : T?) {
        let arr <- [1, 2, 3, 4, 5, 6]
        // _take_while(_<4): [1,2,3]; sum=6.
        let s = _fold(each(arr)._take_while(_ < 4).sum())
        tt |> equal(s, 6)
    }
}

// ═════════════════════════════════════════════════════════════════════════════
// 4. Post-take where (Theme 2 5c — gates contribution; take cap still ticks)
// ═════════════════════════════════════════════════════════════════════════════

[test(description = "a where after take gates each element's contribution while the take cap still counts every element")]
def test_post_take_where(t : T?) {
    t |> run("take(N)._where(p): take fires unconditionally, where gates per-element acc") @(tt : T?) {
        let arr <- [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
        // take(5) → [1..5]; _%2==0 keeps [2,4]; count=2.
        let n = _fold(each(arr).take(5)._where(_ % 2 == 0).count())
        tt |> equal(n, 2)
    }
}

// ═════════════════════════════════════════════════════════════════════════════
// 5. Fast paths — length shortcut + any-empty shortcut on length-bearing source
// ═════════════════════════════════════════════════════════════════════════════

[test(description = "count on a bare length-bearing source answers from its length")]
def test_length_shortcut(t : T?) {
    t |> run("count() on bare length-bearing source: emit_length_shortcut") @(tt : T?) {
        let arr <- [10, 20, 30, 40, 50]
        let n = _fold(each(arr).count())
        tt |> equal(n, 5)
    }
}

[test(description = "any without a predicate on a bare length-bearing source answers from its length")]
def test_any_empty_shortcut(t : T?) {
    t |> run("any() no-pred on bare length-bearing source: emit_any_empty_shortcut") @(tt : T?) {
        let arr <- [1, 2, 3]
        var empty_arr : array<int>
        tt |> equal(_fold(each(arr).any()), true)
        tt |> equal(_fold(each(empty_arr).any()), false)
    }
}
