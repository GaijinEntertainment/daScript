// Tutorial 33: Algorithm
//
// This tutorial covers:
//   - Searching sorted arrays: lower_bound, upper_bound, binary_search, equal_range
//   - Sorting: sort (full sort), partial_sort (first N), nth_element (kth-smallest)
//   - Deduplication: sort_unique, unique
//   - Heap operations: make_heap, push_heap, pop_heap (binary max-heap)
//   - Top-N selection: top_n / top_n_by (N smallest via partial-sort or bounded heap)
//   - Array manipulation: reverse, combine, fill, rotate, erase_all
//   - Querying arrays: is_sorted, min_element, max_element
//   - Set operations on tables: intersection, union, difference,
//     symmetric_difference, identical, is_subset
//   - Topological sort
//
// requires: daslib/algorithm, daslib/sort_boost, daslib/linq
// Run: daslang.exe tutorials/language/33_algorithm.das

options gen2
options persistent_heap

require daslib/algorithm
require daslib/sort_boost
require daslib/linq
require math

struct PricePoint {
    item_id : int
    price   : int
}

struct Person {
    name : string
    age  : int
}

// === Binary search family ===

def binary_search_demo() {
    print("\n=== Binary search family ===\n")

    // lower_bound: first element >= value
    // upper_bound: first element >  value
    // binary_search: returns true if value exists
    // equal_range: [lower_bound, upper_bound) as int2

    let a <- [1, 2, 2, 2, 3, 5, 8, 13]

    let lb = lower_bound(a, 2)
    let ub = upper_bound(a, 2)
    let er = equal_range(a, 2)
    print("array: {a}\n")
    print("lower_bound(2) = {lb}\n")    // 1
    print("upper_bound(2) = {ub}\n")    // 4
    print("equal_range(2)  = {er}\n")   // (1, 4) — three 2's at indices 1..3
    print("binary_search(2) = {binary_search(a, 2)}\n")  // true
    print("binary_search(4) = {binary_search(a, 4)}\n")  // false

    // With a range  [f, l)
    print("lower_bound(2, range 0..4) = {lower_bound(a, 0, 4, 2)}\n")

    // Custom comparator — descending order
    let desc <- [9, 7, 5, 3, 1]
    let idx = lower_bound(desc, 5) $(lhs, rhs : int const) {
        return lhs > rhs
    }
    print("lower_bound in descending [9,7,5,3,1] for 5 = {idx}\n") // 2
}

// === sort ===

def sort_demo() {
    print("\n=== sort ===\n")

    // Default sort: ascending by `<`. Works on int/float arrays directly.
    var a <- [5, 2, 8, 1, 9, 3, 7]
    sort(a)
    print("sort: {a}\n")  // [1, 2, 3, 5, 7, 8, 9]

    // Custom comparator (descending) via block argument.
    var b <- [5, 2, 8, 1, 9]
    sort(b) $(x, y : int) : bool {
        return x > y
    }
    print("sort desc: {b}\n")  // [9, 8, 5, 2, 1]

    // Today's sort is full O(n log n) on the whole array. If you only need
    // the first N sorted, use partial_sort. If you only need the k-th element,
    // use nth_element. Both are demonstrated in the next section.
}

// === partial_sort / nth_element ===

def partial_sort_demo() {
    print("\n=== partial_sort / nth_element ===\n")

    // partial_sort(arr, N) sorts only the first N elements ascending; the
    // remainder is left in unspecified order. Wins big when N << length(arr).
    var a <- [5, 2, 8, 1, 9, 3, 7, 4, 6, 0]
    sort_boost::partial_sort(a, 3)
    print("partial_sort(N=3) first 3: {a[0]},{a[1]},{a[2]}\n")  // 0,1,2

    // nth_element(arr, k) places the k-th smallest at position k. Elements
    // before are <= it; elements after are >=. No full ordering on either
    // side, but the k-th position is correctly the k-th-smallest.
    var b <- [7, 3, 1, 9, 5, 8, 0, 4, 2]
    sort_boost::nth_element(b, 4)  // median of 9 elements is position 4
    print("nth_element(k=4) → b[4]={b[4]}\n")  // 4

    // Custom comparator works for both (e.g. by struct field):
    var c <- [PricePoint(item_id=1, price=50), PricePoint(item_id=2, price=20), PricePoint(item_id=3, price=80), PricePoint(item_id=4, price=10), PricePoint(item_id=5, price=30)]
    sort_boost::partial_sort(c, 2) $(x, y : PricePoint) : bool {
        return x.price < y.price
    }
    print("partial_sort by price (N=2): id={c[0].item_id}/price={c[0].price}, id={c[1].item_id}/price={c[1].price}\n")
}

// === Heap operations ===

def heap_ops_demo() {
    print("\n=== Heap operations ===\n")

    // make_heap turns the array into a binary max-heap (largest at root).
    // push_heap assumes the new element was just appended at the end.
    // pop_heap moves the heap max to the last slot — caller drops it after.

    // Build a max-heap from an unordered array.
    var a <- [5, 2, 8, 1, 9, 3, 7]
    sort_boost::make_heap(a)
    print("make_heap: root (max) = {a[0]}\n")  // 9

    // Push a new element: append, then sift-up.
    a |> push(12)
    sort_boost::push_heap(a)
    print("after push(12): new root = {a[0]}\n")  // 12

    // Pop the max: sift-down then drop the last slot.
    sort_boost::pop_heap(a)
    let popped = a[length(a) - 1]
    a |> resize(length(a) - 1)
    print("popped max: {popped}, new root: {a[0]}\n")  // 12 popped, root now 9

    // Heap-sort by repeatedly popping the max gives a descending sequence.
    var b <- [5, 2, 8, 1, 9, 3, 7]
    sort_boost::make_heap(b)
    var descending : array<int>
    while (!empty(b)) {
        sort_boost::pop_heap(b)
        descending |> push(b[length(b) - 1])
        b |> resize(length(b) - 1)
    }
    print("heap-sort descending: {descending}\n")  // [9, 8, 7, 5, 3, 2, 1]
}

// === top_n / top_n_by ===

def top_n_demo() {
    print("\n=== top_n / top_n_by ===\n")

    // top_n returns the N smallest elements as a sorted ascending array.
    // For array sources it uses partial_sort internally (O(M log N)).
    let arr <- [10, 20, 5, 8, 30, 15, 2, 25]
    let smallest3 <- top_n(arr, 3)
    print("top_n(arr, 3): {smallest3}\n")  // [2, 5, 8]

    // For iterator sources, top_n uses a bounded heap of size N during the
    // scan — at most N+1 elements ever resident, O(M log N) total.
    let arr2 <- [10, 20, 5, 8, 30, 15, 2, 25]
    let smallest3_iter <- top_n(arr2.to_sequence(), 3)
    print("top_n(iter, 3): {smallest3_iter}\n")  // [2, 5, 8]

    // top_n_by takes a key function. Useful for sorting structs by field
    // without writing a full comparator block.
    let people <- [
        Person(name = "Alice", age = 30),
        Person(name = "Bob",   age = 25),
        Person(name = "Carol", age = 35),
        Person(name = "Dave",  age = 22),
        Person(name = "Eve",   age = 40)
    ]
    let youngest3 <- top_n_by(people, 3, @@(p : Person -&) => p.age)
    print("youngest 3: {youngest3[0].name}({youngest3[0].age}), {youngest3[1].name}({youngest3[1].age}), {youngest3[2].name}({youngest3[2].age})\n")
    // Dave(22), Bob(25), Alice(30)
}

// === sort_unique / unique ===

def sort_unique_demo() {
    print("\n=== sort_unique / unique ===\n")

    var a <- [5, 3, 1, 3, 5, 1, 2]
    sort_unique(a)
    print("sort_unique: {a}\n")   // [1, 2, 3, 5]

    // unique only removes ADJACENT duplicates (like C++ std::unique).
    // That means the array should be sorted first for full dedup.
    var b <- [3, 1, 3, 1]
    let c <- algorithm::unique(b)
    print("unique([3,1,3,1]) = {c}\n") // [3,1,3,1] — no adjacent dups!

    var d <- [1, 1, 2, 2, 3, 3]
    let e <- algorithm::unique(d)
    print("unique([1,1,2,2,3,3]) = {e}\n") // [1,2,3]
}

// === Array manipulation ===

def array_manipulation_demo() {
    print("\n=== reverse / combine / fill / rotate ===\n")

    // reverse — in place
    var a <- [1, 2, 3, 4, 5]
    algorithm::reverse(a)
    print("reverse: {a}\n")  // [5, 4, 3, 2, 1]

    // combine — concatenate two arrays into a new one
    let left <- [1, 2, 3]
    let right <- [4, 5, 6]
    let both <- combine(left, right)
    print("combine: {both}\n")  // [1, 2, 3, 4, 5, 6]

    // fill — set all elements to a value
    var b : array<int>
    resize(b, 5)
    fill(b, 42)
    print("fill: {b}\n")  // [42, 42, 42, 42, 42]

    // rotate — element at `mid` becomes first
    var c <- [1, 2, 3, 4, 5]
    rotate(c, 2)
    print("rotate(2): {c}\n")  // [3, 4, 5, 1, 2]
}

// === erase_all ===

def erase_all_demo() {
    print("\n=== erase_all ===\n")

    var a <- [1, 2, 3, 2, 4, 2, 5]
    erase_all(a, 2)
    print("after erase_all(2): {a}\n")  // [1, 3, 4, 5]
}

// === Querying arrays ===

def query_demo() {
    print("\n=== is_sorted / min_element / max_element ===\n")

    let a <- [1, 2, 3, 4, 5]
    print("is_sorted([1..5]) = {is_sorted(a)}\n")  // true

    let b <- [3, 1, 4, 1, 5, 9, 2, 6]
    print("is_sorted([3,1,4..]) = {is_sorted(b)}\n") // false

    let mi = min_element(b)
    let ma = max_element(b)
    print("min_element = index {mi}, value {b[mi]}\n")  // 1, value 1
    print("max_element = index {ma}, value {b[ma]}\n")  // 5, value 9

    // Custom comparator: find the element closest to zero
    let c <- [-10, 3, -7, 2]
    let idx = min_element(c) $(lhs, rhs : int const) {
        return lhs * lhs < rhs * rhs
    }
    print("min by magnitude: index {idx}, value {c[idx]}\n")  // 3, value 2
}

// === Set operations on tables ===

def set_operations_demo() {
    print("\n=== Set operations ===\n")

    // Tables used as sets (table<KeyType> with no value type)
    let a <- { 1, 2, 3, 4 }
    let b <- { 3, 4, 5, 6 }

    let inter <- intersection(a, b)
    let uni   <- union(a, b)
    let diff  <- difference(a, b)
    let sdiff <- symmetric_difference(a, b)

    // We can't print tables in a deterministic order, so check membership
    print("intersection has 3: {inter |> key_exists(3)}\n")  // true
    print("intersection has 1: {inter |> key_exists(1)}\n")  // false

    print("union has 1: {uni |> key_exists(1)}\n")     // true
    print("union has 6: {uni |> key_exists(6)}\n")     // true
    print("union size: {length(uni)}\n")               // 6

    print("difference has 1: {diff |> key_exists(1)}\n")     // true  (in a, not in b)
    print("difference has 3: {diff |> key_exists(3)}\n")     // false

    print("symmetric_difference has 1: {sdiff |> key_exists(1)}\n") // true
    print("symmetric_difference has 3: {sdiff |> key_exists(3)}\n") // false
    print("symmetric_difference has 5: {sdiff |> key_exists(5)}\n") // true

    // identical — test set equality
    print("identical(a, a): {identical(a, a)}\n")  // true
    print("identical(a, b): {identical(a, b)}\n")  // false

    // is_subset
    let sub <- { 2, 3 }
    print("is_subset(\{2,3}, a): {is_subset(sub, a)}\n")  // true
    print("is_subset(a, sub):    {is_subset(a, sub)}\n")   // false
}

// === Topological sort ===

struct TsNode {
    id     : int
    before : table<int>  // which node ids must come before this one
}

def topological_sort_demo() {
    print("\n=== Topological sort ===\n")

    // Build a dependency graph:
    //   compile -> link -> run
    //   test depends on compile
    //
    //   0=compile, 1=link, 2=run, 3=test
    let nodes <- [TsNode(
        id=2, before <- {1}),    // run depends on link
        TsNode(
        id=1, before <- {0}),    // link depends on compile
        TsNode(
        id=3, before <- {0}),    // test depends on compile
        TsNode(
        id=0)                    // compile has no deps
    ]

    var sorted <- topological_sort(nodes)
    print("topological order: ")
    for (n in sorted) {
        print("{n.id} ")
    }
    print("\n")
    // compile (0) first, then link (1) and test (3) in either order, run (2) last
}

// === Fixed-size arrays ===

def fixed_array_demo() {
    print("\n=== Fixed-size arrays ===\n")

    // Most algorithm functions also work on fixed-size arrays
    // via [expect_any_array] overloads.
    var a = fixed_array<int>(5, 3, 1, 4, 2)
    print("min_element: {min_element(a)}\n")  // index of 1
    print("max_element: {max_element(a)}\n")  // index of 5
    print("is_sorted: {is_sorted(a)}\n")      // false

    algorithm::reverse(a)
    print("reversed: {a}\n")

    fill(a, 0)
    print("filled with 0: {a}\n")
}

[export]
def main() {
    binary_search_demo()
    sort_demo()
    partial_sort_demo()
    heap_ops_demo()
    top_n_demo()
    sort_unique_demo()
    array_manipulation_demo()
    erase_all_demo()
    query_demo()
    set_operations_demo()
    topological_sort_demo()
    fixed_array_demo()
}
