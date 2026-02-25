options gen2

require dastest/testing_boost
require daslib/templates_boost
require daslib/templates

// Test some operations that are known to be faster
// with unlocked arrays. These benchmarks say just how much
// faster they are.
//
// It may also be an indicator to look for when (and if)
// locking is either removed or optimized.

struct Locked {
    xs : array<int>
}

[skip_field_lock_check]
struct Unlocked {
    xs : array<int>
}

// To avoid locked arrays suffer from slowed-down reserve
// in benchmarks that are supposed to measure some
// other operation, we use this function to make benchmarking more precise.
// In a real world, this would cause arrays with Locked elements
// behave even slower (due to the need to lock here too).
def reserve_unlocked(var arr : array<auto(T)>; n : int) {
    unsafe(set_verify_array_locks(arr, false))
    reserve(arr, n)
    unsafe(set_verify_array_locks(arr, true))
}

[template(typ1), unused_argument(typ1), sideeffects]
def run_emplace_bench(var typ1 : auto(ElemT), b : B?, n : int) {
    b |> run("emplace/{n}", n) <| $() {
        var arr : array<ElemT>
        reserve_unlocked(arr, n)
        for (i in range(n)) {
            arr |> emplace(default<ElemT>)
        }
    }
}


[template(typ1), unused_argument(typ1), sideeffects]
def run_emplace_grow_bench(var typ1 : auto(ElemT), b : B?, n : int) {
    b |> run("emplace_grow/{n}") <| $() {
        var arr : array<ElemT>
        for (i in range(n)) {
            arr |> emplace(default<ElemT>)
        }
    }
}

[template(typ1), unused_argument(typ1), sideeffects]
def run_reserve_bench(var typ1 : auto(ElemT), b : B?, n : int) {
    // The reserve on this array is basically a no-op, since
    // it already would have the correct capacity.
    // The reserve() does a lock check though, so it can
    // take some time to do that for arrays with elements that require locking.
    // Note: not passing chunk_size to run() on purpose - we're
    // measuring the entire reserve(), not the time it takes
    // to "touch" every element in arr.
    var arr <- [for (_ in range(n)); default<ElemT> ]
    b |> run("reserve/{n}") <| $() {
        reserve(arr, n)
    }
}

[template(typ1), unused_argument(typ1), sideeffects]
def run_move_bench(var typ1 : auto(ElemT), b : B?, n : int) {
    var arr <- [for (_ in range(n)); default<ElemT> ]
    b |> run("move/{n}", n) <| $() {
        for (i in range(n)) {
            var tmp <- arr
            strictEqual(b, length(arr), 0)
            strictEqual(b, length(tmp), n)
            arr <- tmp
            tmp <- arr
            arr <- tmp
        }
    }
}

[benchmark]
def locked_elems(b : B?) {
    run_move_bench(type<Locked>, b, 10)
    run_move_bench(type<Locked>, b, 100)
    run_move_bench(type<Locked>, b, 1000)
    run_emplace_bench(type<Locked>, b, 10)
    run_emplace_bench(type<Locked>, b, 100)
    run_emplace_bench(type<Locked>, b, 1000)
    run_emplace_grow_bench(type<Locked>, b, 10)
    run_emplace_grow_bench(type<Locked>, b, 1000)
    run_emplace_grow_bench(type<Locked>, b, 2000)
    run_reserve_bench(type<Locked>, b, 10)
    run_reserve_bench(type<Locked>, b, 1000)
}

[benchmark]
def unlocked_elems(b : B?) {
    run_move_bench(type<Unlocked>, b, 10)
    run_move_bench(type<Unlocked>, b, 100)
    run_move_bench(type<Unlocked>, b, 1000)
    run_emplace_bench(type<Unlocked>, b, 10)
    run_emplace_bench(type<Unlocked>, b, 100)
    run_emplace_bench(type<Unlocked>, b, 1000)
    run_emplace_grow_bench(type<Unlocked>, b, 10)
    run_emplace_grow_bench(type<Unlocked>, b, 1000)
    run_emplace_grow_bench(type<Unlocked>, b, 2000)
    run_reserve_bench(type<Unlocked>, b, 10)
    run_reserve_bench(type<Unlocked>, b, 1000)
}
