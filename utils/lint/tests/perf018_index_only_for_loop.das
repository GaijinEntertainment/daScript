options gen2
options auto_inline_functions = false   // lint fixtures assert SOURCE shapes; splices rewrite them
// PERF018: for (i in range(length(arr))) where i only indexes arr
//
// Problem:
//   `for (i in range(length(arr))) { ... arr[i] ... }` adds a layer of
//   indirection: the loop counter `i` is only used to subscript `arr`. The
//   direct form `for (c in arr)` iterates the elements without the index.
//
// Bad pattern:
//   for (i in range(length(arr))) {
//       use(arr[i])
//   }
//
// Good pattern:
//   for (c in arr) {
//       use(c)
//   }

expect 31208:3

require daslib/perf_lint

// --- Bad: index-only iteration ---

def bad_array_sum(arr : array<int>) : int {
    var s = 0
    for (i in range(length(arr))) {
        s += arr[i]
    }
    return s
}

def bad_array_print(arr : array<int>) {
    for (i in range(length(arr))) {
        print("{arr[i]}\n")
    }
}

// Multiple qualifying uses on the same arr in one body still warn once.
def bad_array_double_use(arr : array<int>) : int {
    var hi = 0
    for (i in range(length(arr))) {
        if (arr[i] > hi) {
            hi = arr[i]
        }
    }
    return hi
}

// --- Good: index actually used ---

def good_index_used(arr : array<int>) : int {
    var s = 0
    for (i in range(length(arr))) {
        s += arr[i] * i   // i used in arithmetic — disqualifies
    }
    return s
}

def good_index_passed_to_func(arr : array<int>) {
    for (i in range(length(arr))) {
        print("[{i}] = {arr[i]}\n")   // i used as standalone — disqualifies
    }
}

// NOTE: two arrays subscripted in lockstep is no longer a silent disqualifier -
// that shape is PERF029 now (zip the arrays); see perf029_parallel_array_zip.das
def good_array_and_table(a : array<int>; t : table<int; int>) : int {
    var s = 0
    for (i in range(length(a))) {
        s += a[i] + (t?[i] ?? 0)   // t[i] is a key lookup, not a position - disqualifies
    }
    return s
}

def good_sliding_window(arr : array<int>) : int {
    var s = 0
    for (i in range(length(arr) - 1)) {   // not range(length(arr)) — doesn't match
        s += arr[i] + arr[i + 1]
    }
    return s
}

def good_for_each(arr : array<int>) : int {
    var s = 0
    for (c in arr) {   // already direct iteration — no warning
        s += c
    }
    return s
}

def good_string_iter(s : string) : int {
    var n = 0
    for (c in s) {   // direct iteration, no PERF018
        n++
    }
    return n
}
