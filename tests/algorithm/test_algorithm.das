options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require daslib/algorithm
require dastest/testing_boost public
require daslib/cmp
require daslib/functional

// ---- unique ----

[test(description = "unique drops adjacent duplicates only, so unsorted input keeps its non-adjacent repeats")]
def test_unique_sorted(t : T?) {
    t |> run("removes adjacent duplicates from sorted array") @(t : T?) {
        var a <- [1, 1, 2, 2, 3, 3]
        let b <- unique(a)
        t |> equal(length(b), 3)
        t |> equal(b[0], 1)
        t |> equal(b[1], 2)
        t |> equal(b[2], 3)
    }
    t |> run("empty array") @(t : T?) {
        var a : array<int>
        let b <- unique(a)
        t |> equal(length(b), 0)
    }
    t |> run("single element") @(t : T?) {
        var a <- [42]
        let b <- unique(a)
        t |> equal(length(b), 1)
        t |> equal(b[0], 42)
    }
    t |> run("all same elements") @(t : T?) {
        var a <- [5, 5, 5, 5]
        let b <- unique(a)
        t |> equal(length(b), 1)
        t |> equal(b[0], 5)
    }
    t |> run("no duplicates") @(t : T?) {
        var a <- [1, 2, 3, 4]
        let b <- unique(a)
        t |> equal(length(b), 4)
    }
    t |> run("unsorted input keeps non-adjacent duplicates") @(t : T?) {
        // unique only removes ADJACENT duplicates
        var a <- [3, 1, 3, 1]
        let b <- unique(a)
        t |> equal(length(b), 4)
    }
}

// ---- sort_unique ----

[test(description = "sort_unique leaves the array sorted with every duplicate removed, on empty and single-element input too")]
def test_sort_unique(t : T?) {
    t |> run("sorts and deduplicates") @(t : T?) {
        var a <- [3, 1, 2, 1, 3, 2]
        sort_unique(a)
        t |> equal(length(a), 3)
        t |> equal(a[0], 1)
        t |> equal(a[1], 2)
        t |> equal(a[2], 3)
    }
    t |> run("empty array") @(t : T?) {
        var a : array<int>
        sort_unique(a)
        t |> equal(length(a), 0)
    }
    t |> run("single element") @(t : T?) {
        var a <- [10]
        sort_unique(a)
        t |> equal(length(a), 1)
        t |> equal(a[0], 10)
    }
    t |> run("already sorted and unique") @(t : T?) {
        var a <- [1, 2, 3]
        sort_unique(a)
        t |> equal(length(a), 3)
    }
    t |> run("all same") @(t : T?) {
        var a <- [7, 7, 7]
        sort_unique(a)
        t |> equal(length(a), 1)
        t |> equal(a[0], 7)
    }
}

// ---- reverse ----

[test(description = "reverse flips an array in place for even, odd, empty, single, two-element and fixed-size inputs")]
def test_reverse(t : T?) {
    t |> run("reverses even-length array") @(t : T?) {
        var a <- [1, 2, 3, 4]
        reverse(a)
        t |> equal(a[0], 4)
        t |> equal(a[1], 3)
        t |> equal(a[2], 2)
        t |> equal(a[3], 1)
    }
    t |> run("reverses odd-length array") @(t : T?) {
        var a <- [1, 2, 3, 4, 5]
        reverse(a)
        t |> equal(a[0], 5)
        t |> equal(a[2], 3)
        t |> equal(a[4], 1)
    }
    t |> run("empty array") @(t : T?) {
        var a : array<int>
        reverse(a)
        t |> equal(length(a), 0)
    }
    t |> run("single element") @(t : T?) {
        var a <- [99]
        reverse(a)
        t |> equal(a[0], 99)
    }
    t |> run("two elements") @(t : T?) {
        var a <- [1, 2]
        reverse(a)
        t |> equal(a[0], 2)
        t |> equal(a[1], 1)
    }
    t |> run("fixed-size array") @(t : T?) {
        var a = [1, 2, 3, 4]
        reverse(a)
        t |> equal(a[0], 4)
        t |> equal(a[3], 1)
    }
}

// ---- combine ----

[test(description = "combine returns the concatenation of two arrays and tolerates either or both being empty")]
def test_combine(t : T?) {
    t |> run("combines two arrays") @(t : T?) {
        let a <- [1, 2]
        let b <- [3, 4]
        let c <- combine(a, b)
        t |> equal(length(c), 4)
        t |> equal(c[0], 1)
        t |> equal(c[1], 2)
        t |> equal(c[2], 3)
        t |> equal(c[3], 4)
    }
    t |> run("first empty") @(t : T?) {
        let a : array<int>
        let b <- [1, 2]
        let c <- combine(a, b)
        t |> equal(length(c), 2)
        t |> equal(c[0], 1)
    }
    t |> run("second empty") @(t : T?) {
        let a <- [1, 2]
        let b : array<int>
        let c <- combine(a, b)
        t |> equal(length(c), 2)
        t |> equal(c[1], 2)
    }
    t |> run("both empty") @(t : T?) {
        let a, b : array<int>
        let c <- combine(a, b)
        t |> equal(length(c), 0)
    }
}

// ---- lower_bound ----

[test(description = "lower_bound returns the first position not less than the key, including with a comparator, a range and duplicates")]
def test_lower_bound(t : T?) {
    t |> run("finds insertion point") @(t : T?) {
        let a <- [1, 3, 5, 7, 9]
        t |> equal(lower_bound(a, 5), 2)
        t |> equal(lower_bound(a, 4), 2)
        t |> equal(lower_bound(a, 0), 0)
        t |> equal(lower_bound(a, 10), 5)
    }
    t |> run("range overload") @(t : T?) {
        let a <- [1, 3, 5, 7, 9]
        t |> equal(lower_bound(a, 1, 4, 5), 2)
        t |> equal(lower_bound(a, 2, 4, 7), 3)
    }
    t |> run("custom comparator") @(t : T?) {
        let a <- [9, 7, 5, 3, 1]
        // descending order: "less" means a > b
        let idx = lower_bound(a, 5) <| cmp_by(-_)
        t |> equal(idx, 2)
    }
    t |> run("empty array") @(t : T?) {
        let a : array<int>
        t |> equal(lower_bound(a, 42), 0)
    }
    t |> run("all elements equal") @(t : T?) {
        let a <- [5, 5, 5, 5]
        t |> equal(lower_bound(a, 5), 0)
        t |> equal(lower_bound(a, 6), 4)
    }
    t |> run("duplicates find first") @(t : T?) {
        let a <- [1, 2, 2, 2, 3]
        t |> equal(lower_bound(a, 2), 1)
    }
    t |> run("fixed-size array") @(t : T?) {
        let a = [1, 3, 5, 7, 9]
        t |> equal(lower_bound(a, 5), 2)
    }
}

// ---- binary_search ----

[test(description = "binary_search reports whether an element is present on sorted input, with a comparator and range, and false on empty")]
def test_binary_search(t : T?) {
    t |> run("finds existing element") @(t : T?) {
        let a <- [1, 3, 5, 7, 9]
        t |> equal(binary_search(a, 5), true)
        t |> equal(binary_search(a, 1), true)
        t |> equal(binary_search(a, 9), true)
    }
    t |> run("missing element") @(t : T?) {
        let a <- [1, 3, 5, 7, 9]
        t |> equal(binary_search(a, 4), false)
        t |> equal(binary_search(a, 0), false)
        t |> equal(binary_search(a, 10), false)
    }
    t |> run("range overload") @(t : T?) {
        let a <- [1, 3, 5, 7, 9]
        t |> equal(binary_search(a, 1, 4, 5), true)
        t |> equal(binary_search(a, 1, 4, 9), false)
    }
    t |> run("custom comparator") @(t : T?) {
        let a <- [9, 7, 5, 3, 1]
        let found = binary_search(a, 5) <| cmp_by(-_)
        t |> equal(found, true)
    }
    t |> run("empty array") @(t : T?) {
        let a : array<int>
        t |> equal(binary_search(a, 42), false)
    }
    t |> run("single element found") @(t : T?) {
        let a <- [42]
        t |> equal(binary_search(a, 42), true)
    }
    t |> run("single element not found") @(t : T?) {
        let a <- [42]
        t |> equal(binary_search(a, 99), false)
    }
    t |> run("fixed-size array") @(t : T?) {
        let a = [1, 3, 5, 7, 9]
        t |> equal(binary_search(a, 5), true)
        t |> equal(binary_search(a, 4), false)
    }
}

// ---- erase_all ----

[test(description = "erase_all removes every occurrence of a value and leaves the array unchanged when the value is absent")]
def test_erase_all(t : T?) {
    t |> run("erases all occurrences") @(t : T?) {
        var a <- [1, 2, 3, 2, 4, 2]
        erase_all(a, 2)
        t |> equal(length(a), 3)
        t |> equal(a[0], 1)
        t |> equal(a[1], 3)
        t |> equal(a[2], 4)
    }
    t |> run("value not present") @(t : T?) {
        var a <- [1, 2, 3]
        erase_all(a, 99)
        t |> equal(length(a), 3)
    }
    t |> run("all same elements") @(t : T?) {
        var a <- [5, 5, 5]
        erase_all(a, 5)
        t |> equal(length(a), 0)
    }
    t |> run("empty array") @(t : T?) {
        var a : array<int>
        erase_all(a, 1)
        t |> equal(length(a), 0)
    }
    t |> run("single element match") @(t : T?) {
        var a <- [42]
        erase_all(a, 42)
        t |> equal(length(a), 0)
    }
    t |> run("single element no match") @(t : T?) {
        var a <- [42]
        erase_all(a, 99)
        t |> equal(length(a), 1)
    }
    t |> run("fixed-size array") @(t : T?) {
        var a = [1, 2, 3, 2, 4]
        erase_all(a, 2)
        t |> equal(a[0], 1)
        t |> equal(a[1], 3)
        t |> equal(a[2], 4)
    }
}

// ---- topological_sort ----

struct TsNode {
    id     : int
    before : table<int>
}

[test(description = "topological_sort orders a chain, a diamond, a single node, an empty graph and an edgeless graph consistently with their dependencies")]
def test_topological_sort_cases(t : T?) {
    t |> run("linear chain") @(t : T?) {
        // 0 -> 1 -> 2
        let nodes <- [TsNode(
            id=2, before <- {1}), TsNode(
            id=1, before <- {0}), TsNode(
            id=0
        )]
        let res <- topological_sort(nodes)
        t |> equal(length(res), 3)
        t |> equal(res[0].id, 0)
        t |> equal(res[1].id, 1)
        t |> equal(res[2].id, 2)
    }
    t |> run("diamond graph") @(t : T?) {
        // 0 -> 1 -> 3
        // 0 -> 2 -> 3
        let nodes <- [TsNode(
            id=3, before <- {1, 2}), TsNode(
            id=0), TsNode(
            id=2, before <- {0}), TsNode(
            id=1, before <- {0}
        )]
        let res <- topological_sort(nodes)
        t |> equal(length(res), 4)
        t |> equal(res[0].id, 0)
        t |> equal(res[3].id, 3)
    }
    t |> run("single node") @(t : T?) {
        let nodes <- [TsNode(id=0)]
        let res <- topological_sort(nodes)
        t |> equal(length(res), 1)
        t |> equal(res[0].id, 0)
    }
    t |> run("empty graph") @(t : T?) {
        let nodes : array<TsNode>
        let res <- topological_sort(nodes)
        t |> equal(length(res), 0)
    }
    t |> run("no dependencies") @(t : T?) {
        let nodes <- [TsNode(id=3), TsNode(id=1), TsNode(id=2)]
        let res <- topological_sort(nodes)
        t |> equal(length(res), 3)
    }
}

// ---- set operations ----

[test(description = "intersection of two sets holds exactly the shared keys, empty when nothing is shared or either side is empty")]
def test_intersection(t : T?) {
    t |> run("basic intersection") @(t : T?) {
        let a <- { 1, 2, 3 }
        let b <- { 2, 3, 4 }
        let c <- intersection(a, b)
        t |> success(identical(c, {2, 3}))
    }
    t |> run("no overlap") @(t : T?) {
        let a <- { 1, 2 }
        let b <- { 3, 4 }
        let c <- intersection(a, b)
        t |> equal(length(c), 0)
    }
    t |> run("one empty") @(t : T?) {
        let a <- { 1, 2, 3 }
        let b : table<int>
        let c <- intersection(a, b)
        t |> equal(length(c), 0)
    }
    t |> run("both empty") @(t : T?) {
        let a, b : table<int>
        let c <- intersection(a, b)
        t |> equal(length(c), 0)
    }
    t |> run("identical sets") @(t : T?) {
        let a <- { 1, 2, 3 }
        let b <- { 1, 2, 3 }
        let c <- intersection(a, b)
        t |> success(identical(c, a))
    }
}

[test(description = "union of two sets holds every key of both, and an empty side adds nothing")]
def test_union(t : T?) {
    t |> run("basic union") @(t : T?) {
        let a <- { 1, 2 }
        let b <- { 2, 3 }
        let c <- union(a, b)
        t |> success(identical(c, {1, 2, 3}))
    }
    t |> run("one empty") @(t : T?) {
        let a <- { 1, 2 }
        let b : table<int>
        let c <- union(a, b)
        t |> success(identical(c, a))
    }
    t |> run("both empty") @(t : T?) {
        let a, b : table<int>
        let c <- union(a, b)
        t |> equal(length(c), 0)
    }
}

[test(description = "difference holds the keys of the first set missing from the second, all of them when disjoint and none for a subset")]
def test_difference(t : T?) {
    t |> run("basic difference") @(t : T?) {
        let a <- { 1, 2, 3 }
        let b <- { 2, 3, 4 }
        let c <- difference(a, b)
        t |> success(identical(c, {1}))
    }
    t |> run("no overlap") @(t : T?) {
        let a <- { 1, 2 }
        let b <- { 3, 4 }
        let c <- difference(a, b)
        t |> success(identical(c, a))
    }
    t |> run("subset") @(t : T?) {
        let a <- { 1, 2 }
        let b <- { 1, 2, 3 }
        let c <- difference(a, b)
        t |> equal(length(c), 0)
    }
}

[test(description = "identical is true only for sets with the same keys, regardless of insertion order")]
def test_identical(t : T?) {
    t |> run("equal sets") @(t : T?) {
        let a <- { 1, 2, 3 }
        let b <- { 3, 2, 1 }
        t |> equal(identical(a, b), true)
    }
    t |> run("different sizes") @(t : T?) {
        let a <- { 1, 2 }
        let b <- { 1, 2, 3 }
        t |> equal(identical(a, b), false)
    }
    t |> run("same size different elements") @(t : T?) {
        let a <- { 1, 2 }
        let b <- { 1, 3 }
        t |> equal(identical(a, b), false)
    }
    t |> run("both empty") @(t : T?) {
        let a, b : table<int>
        t |> equal(identical(a, b), true)
    }
}

// ---- set operations ----

[test(description = "intersection, union and difference of integer sets give the expected key sets in both directions")]
def test_set_operations(t : T?) {
    let a <- { 1, 2, 3 }
    let b <- { 2, 3, 4 }
    let c <- intersection(a, b)
    t |> success(identical(c, {2, 3}))
    let d <- union(a, b)
    t |> success(identical(d, {1, 2, 3, 4}))
    let e <- difference(a, b)
    t |> success(identical(e, {1}))
    let f <- difference(b, a)
    t |> success(identical(f, {4}))
}

// ---- topological sort ----

struct Node {
    id     : int
    before : table<int>
}

[test(description = "topological_sort puts each node after the nodes it must follow in a diamond graph and keeps every node")]
def test_topological_sort(t : T?) {
    // 0 -> 1 -> 3
    // 0 -> 2 -> 3
    let nodes <- [Node(
        id=3, before <- {1, 2}), Node(
        id=0), Node(
        id=2, before <- {0}), Node(
        id=1, before <- {0}
    )]
    var res <- topological_sort(nodes)
    t |> success(length(res) == 4, "expecting all the same nodes")
    var good : bool[4]
    for (n, i in res, range(4)) {
        good[n.id] = true
        if (n.id == 0) {
            t |> success(i == 0, "0 is first")
        } elif (n.id == 3) {
            t |> success(i == 3, "3 is last")
        } elif (n.id == 1 || n.id == 2) {
            t |> success(i == 1 || i == 2, "1 or 2 is second and third")
        }
    }
    t |> success(all(good), "all nodes are present")
}
