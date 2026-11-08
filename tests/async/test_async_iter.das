options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost public
require daslib/async_boost

// Test: iterating async generators
// Tests for-loop over typed async generators, zip with count(),
// bool-typed async, and await_next_frame inside iteration.



// ---- basic async range ----

[async]
def int_range() : int {
    yield 10
    yield 20
    yield 30
}

[test(description = "a for loop over a typed int async collects every yielded value")]
def test_async_int_range(t : T?) {
    t |> run("iterate typed async with for loop") @(t : T?) {
        let vals <- [for (v in int_range()); v as res; where v is res]
        t |> equal(length(vals), 3)
        t |> equal(vals[0], 10)
        t |> equal(vals[1], 20)
        t |> equal(vals[2], 30)
    }
}

// ---- bool async range ----

[async]
def bool_range() : bool {
    yield true
    yield false
    yield true
}

[test(description = "a for loop over a bool async collects every yielded value in order")]
def test_async_bool_range(t : T?) {
    t |> run("iterate bool async") @(t : T?) {
        let vals <- [for (v in bool_range()); v as res; where v is res]
        t |> equal(length(vals), 3)
        t |> equal(vals[0], true)
        t |> equal(vals[1], false)
        t |> equal(vals[2], true)
    }
}

// ---- zip with count ----

[async]
def indexed_range() : int {
    yield 100
    yield 200
    yield 300
}

[test(description = "an async generator zips with count() in a for loop")]
def test_zip_with_count(t : T?) {
    t |> run("async generator zipped with count()") @(t : T?) {
        var indices, values : array<int>
        for (idx, v in count(), indexed_range()) {
            indices |> push(idx)
            if (v is res) {
                values |> push(v as res)
            }
        }
        t |> equal(length(indices), 3)
        // indices include wait frames too, so check values
        t |> equal(length(values), 3)
        t |> equal(values[0], 100)
        t |> equal(values[1], 200)
        t |> equal(values[2], 300)
    }
}

// ---- async range with frame pauses ----

[async]
def spaced_range() : int {
    yield 1
    await_next_frame()
    yield 2
    await_next_frame()
    yield 3
}

[test(description = "an async with await_next_frame between yields still delivers every value, spread over the waited frames")]
def test_spaced_range(t : T?) {
    t |> run("async range with await_next_frame between yields") @(t : T?) {
        var vals : array<int>
        var total_frames = 0
        for (v in spaced_range()) {
            total_frames ++
            if (v is res) {
                vals |> push(v as res)
            }
        }
        t |> equal(length(vals), 3)
        t |> equal(vals[0], 1)
        t |> equal(vals[1], 2)
        t |> equal(vals[2], 3)
        // 3 values + 2 waits = 5 total iterations
        t |> equal(total_frames, 5)
    }
}

// ---- empty async range ----

[async]
def empty_range() : int {
    return false
}

[test(description = "an async that yields nothing leaves a for loop body unexecuted")]
def test_empty_range(t : T?) {
    t |> run("empty async range produces no values") @(t : T?) {
        var count = 0
        for (_v in empty_range()) {
            count ++
        }
        t |> equal(count, 0)
    }
}

// ---- single yield ----

[async]
def single_value() : int {
    yield 42
}

[test(description = "an async with one yield produces exactly that value")]
def test_single_value(t : T?) {
    t |> run("async with single yield") @(t : T?) {
        let vals <- [for (v in single_value()); v as res; where v is res]
        t |> equal(length(vals), 1)
        t |> equal(vals[0], 42)
    }
}

// ---- string async range ----

[async]
def string_range() : string {
    yield "alpha"
    yield "beta"
    yield "gamma"
}

[test(description = "a string-typed async yields its strings in order")]
def test_string_range(t : T?) {
    t |> run("string-typed async range") @(t : T?) {
        let vals <- [for (v in string_range()); v as res; where v is res]
        t |> equal(length(vals), 3)
        t |> equal(vals[0], "alpha")
        t |> equal(vals[1], "beta")
        t |> equal(vals[2], "gamma")
    }
}

// Test: per-step coroutine advance pattern.
//
// `for (v in iter) { ... break }` is **destructive** — `SimNode_Seq2Iter`
// moves the iter out of the Sequence into the for-loop's local state, and
// the for-loop's loopend calls `close()` on it. After break+exit, the
// Sequence has `iter == nullptr` AND the iterator state itself is gone, so
// a subsequent `for (v in iter)` panics with "iterator is empty or already
// consumed". This is by design — `for-in` owns the iterator for the
// duration of the loop.
//
// The non-destructive per-step advance is `next(iter, value) : bool` from
// `daslib/builtin`, plus `empty(iter)` to detect exhaustion. That pattern
// preserves the Sequence's internal iter pointer across calls; daslib's
// own `cr_run_all` (`daslib/coroutines.das:142`) uses it. The dasImgui
// Phase 2 runtime's `advance_coroutines` follows the same pattern.
//
// This test pins the working pattern as a regression target.



[async]
def three_yield() : void {
    await_next_frame()
    await_next_frame()
    await_next_frame()
}

[test(description = "next and empty advance a stored async one step at a time without consuming it, unlike a for loop that breaks")]
def test_next_per_step_advance(t : T?) {
    t |> run("next/empty pattern advances one step at a time, repeatedly") @(t : T?) {
        var it <- three_yield()
        var advances = 0
        var loop_iter = 0
        while (loop_iter < 10) {
            loop_iter++
            var v : bool
            next(it, v)
            if (empty(it)) {
                break
            }
            advances++
        }
        // 3 await_next_frame calls before the implicit `return false` from
        // AsyncMacro — three successful next() yields, then iter goes empty.
        t |> equal(advances, 3, "next/empty: 3 yields before exhaustion")
    }
}
