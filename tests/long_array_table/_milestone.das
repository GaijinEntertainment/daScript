options gen2
require dastest/testing_boost public

// Phase 0 scaffold for the daslang 64-bit-arrays project.
//
// This file is DISABLED from dastest discovery via the leading underscore
// (per `dastest/fs.das` file-name filter).  It exists to demonstrate the
// compile-time gap that PR #2 closes: on master at b9c255e7c, none of the
// `long_*` builtins / int64 overloads / `arr[i64]` dispatches resolve.
//
// Manual invocation:
//   bin/Release/daslang.exe tests/long_array_table/_milestone.das
// Expected today: compile errors on every body below (no such overloads).
// Expected after PR #2 merges: clean compile + green when run under dastest.
// Expected after PR #4 (AOT) + PR #5 (JIT): same under all three execution
// modes; rename to drop the underscore to enroll in dastest discovery.

[test]
def test_long_length_and_capacity(t : T?) {
    t |> run("array long_length / long_capacity") @(t : T?) {
        var a : array<int>
        a |> reserve(int64(50))                          // reserve int64 overload (NEW)
        a |> resize(int64(10))                           // resize int64 overload (NEW)
        t |> equal(long_length(a), int64(10))            // NEW long_length(arr)
        t |> equal(long_capacity(a) >= int64(50), true)  // NEW long_capacity(arr)
        t |> equal(length(a), 10)                        // existing surface unchanged
    }
    t |> run("table long_length / long_capacity") @(t : T?) {
        var m : table<int; int>
        m |> reserve(int64(32))                          // table reserve int64 overload (NEW)
        m |> insert(1, 100)
        m |> insert(2, 200)
        t |> equal(long_length(m), int64(2))             // NEW long_length(tab)
        t |> equal(long_capacity(m) >= int64(2), true)   // NEW long_capacity(tab)
    }
}

[test]
def test_arr_int64_index(t : T?) {
    t |> run("arr[int64] read+write") @(t : T?) {
        var a : array<int>
        a |> resize(int64(5))
        let idx : int64 = 2_l
        a[idx] = 42                                      // arr[i64] write dispatch (NEW)
        t |> equal(a[idx], 42)                           // arr[i64] read dispatch (NEW)
        // mixed-width indexing in one expression
        t |> equal(a[idx] + a[int64(1)], 42)
    }
    t |> run("arr[uint64] read+write") @(t : T?) {
        var a : array<int>
        a |> resize(int64(5))
        let idx : uint64 = 3_ul
        a[idx] = 77                                      // arr[u64] write dispatch (NEW)
        t |> equal(a[idx], 77)
    }
}

[test]
def test_long_iter_range_and_enumerate(t : T?) {
    t |> run("long_iter_range fills array") @(t : T?) {
        var a : array<int>
        a |> resize(int64(5))
        for (i in long_iter_range(a)) {                  // NEW iterator<int64>
            a[i] = int(i) * 2
        }
        t |> equal(a[0_l], 0)
        t |> equal(a[4_l], 8)
    }
    t |> run("long_enumerate yields (int64, T)") @(t : T?) {
        var a : array<int>
        a |> resize(int64(5))
        for (i in long_iter_range(a)) {
            a[i] = int(i)
        }
        var sum : int64 = 0_l
        for ((i, v) in long_enumerate(each(a))) {        // NEW
            sum += i + int64(v)
        }
        t |> equal(sum, 20_l)  // 2 * (0+1+2+3+4)
    }
}

[test]
def test_long_find_index(t : T?) {
    t |> run("long_find_index returns int64") @(t : T?) {
        var a : array<int>
        a |> resize(int64(5))
        for (i in long_iter_range(a)) {
            a[i] = int(i) * 10
        }
        t |> equal(long_find_index(a, 30), 3_l)          // NEW long_find_index
        t |> equal(long_find_index(a, 999), -1_l)        // not-found sentinel
    }
}

[test]
def test_int64_mutation_overloads(t : T?) {
    t |> run("erase(arr, int64) and erase(arr, int64, int64)") @(t : T?) {
        var a : array<int>
        a |> resize(int64(5))
        for (i in long_iter_range(a)) {
            a[i] = int(i)
        }
        a |> erase(int64(2))                             // erase int64 overload (NEW)
        t |> equal(long_length(a), 4_l)
        t |> equal(a[2_l], 3)                            // shifted-down
        a |> erase(int64(0), int64(2))                   // erase range int64 overload (NEW)
        t |> equal(long_length(a), 2_l)
        t |> equal(a[0_l], 3)
        t |> equal(a[1_l], 4)
    }
    t |> run("push(arr, val, int64) positional") @(t : T?) {
        var a : array<int>
        a |> resize(int64(3))
        a[0_l] = 10
        a[1_l] = 20
        a[2_l] = 30
        a |> push(99, int64(1))                          // push positional int64 overload (NEW)
        t |> equal(long_length(a), 4_l)
        t |> equal(a[1_l], 99)
        t |> equal(a[2_l], 20)
    }
}
