options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module algorithm shared public

//! General-purpose algorithms.
//!
//! Collection of functional-style helpers: `unique`, `unique_pairs`, `top_sort`,
//! `binary_search`, `lower_bound`, `upper_bound`, `has_value`, `reverse`, `flatten`,
//! and more. Works on arrays and iterators.

require daslib/array_boost
require daslib/contracts

def unique(var a : array<auto(TT)>) {
    //! Returns an array of elements from a with duplicates removed.
    var pidx = -1
    var b : array<TT -const -&>
    for (e, eidx in a, range(length(a))) {
        if (pidx == -1 || a[pidx] != e) {
            pidx = eidx
            b |> push_clone(e)
        }
    }
    return <- b
}

def sort_unique(var a : array<auto(TT)>) {
    //! Returns an array of elements from a, sorted and with duplicates removed.
    //! The elements are sorted in ascending order.
    //! The resulting array has only unique elements.
    if (empty(a)) {
        return
    }
    sort(a)
    var b <- unique(a)
    delete a
    a <- b
}

def reverse(var a : array<auto>) {
    //! Reverses the elements of a in place.
    let l = length(a)
    let half = l / 2
    let lm1 = l - 1
    for (i in range(half)) {
        swap(a[i], a[lm1 - i])
    }
}

def combine(a, b : array<auto(TT)>) {
    //! Returns a new array containing elements from a followed by b.
    var c : array<TT>
    reserve(c, length(a) + length(b))
    for (t in a) {
        c |> push(t)
    }
    for (t in b) {
        c |> push(t)
    }
    return <- c
}

def lower_bound(a : array<auto(TT)>; f, l : int; val : TT const -&) {
    //! Returns the index of the first element in the range [f, l) that is not less than val, or l if no such element is found.
    assert(f >= 0 && f <= l, "lower bound first out of range")
    assert(l >= f && l <= length(a), "lower bound last out of range")
    var count = l - f
    var first = f
    while (count > 0) {
        let step = count / 2
        let it = first + step
        if (a[it] < val) {
            first += step + 1
            count -= step + 1
        } else {
            count = step
        }
    }
    return first
}

def lower_bound(a : array<auto(TT)>; val : TT const -&) {
    //! Returns the index of the first element in the array that is not less than val, or length(a) if no such element is found.
    return lower_bound(a, 0, length(a), val)
}

def lower_bound(a : array<auto(TT)>; f, l : int; value : auto(QQ); less : block<(a : TT const -&, b : QQ) : bool>) {
    //! Returns the index of the first element in the range [f, l) for which less returns false, or l if no such element is found.
    assert(f >= 0 && f <= l, "lower bound first out of range")
    assert(l >= f && l <= length(a), "lower bound last out of range")
    var count = l - f
    var first = f
    while (count > 0) {
        let step = count / 2
        let it = first + step
        if (invoke(less, a[it], value)) {
            first += step + 1
            count -= step + 1
        } else {
            count = step
        }
    }
    return first
}

def lower_bound(a : array<auto(TT)>; value : auto(QQ); less : block<(a : TT const -&, b : QQ) : bool>) {
    //! Returns the index of the first element in the array for which less returns false, or length(a) if no such element is found.
    return lower_bound(a, 0, length(a), value, less)
}

def binary_search(a : array<auto(TT)>; val : TT const -&) {
    //! Returns true if val appears within the array a.
    //! The array must be sorted.
    let first = lower_bound(a, val)
    return (first != length(a)) && (!(val < a[first]))
}

def binary_search(a : array<auto(TT)>; f, last : int; val : TT const -&) {
    //! Returns true if val appears within the range [f, last).
    //! The array must be sorted.
    let first = lower_bound(a, f, last, val)
    return (first != last) && (!(val < a[first]))
}

def binary_search(a : array<auto(TT)>; val : TT const -&; less : block<(a, b : TT const -&) : bool>) {
    //! Returns true if val appears within the array a.
    //! The array must be sorted according to the provided less function.
    let first = lower_bound(a, val, less)
    let last = length(a)
    return (first != last) && (!invoke(less, val, a[first]))
}

def binary_search(a : array<auto(TT)>; f, last : int; val : TT const -&; less : block<(a, b : TT const -&) : bool>) {
    //! Returns true if val appears within the range [f, last).
    //! The array must be sorted according to the provided less function.
    let first = lower_bound(a, f, last, val, less)
    return (first != last) && (!invoke(less, val, a[first]))
}

// now for all the other types

[expect_any_array(a)]
def reverse(var a) {
    //! Reverses the elements of array a in place.
    unsafe {
        reverse(temp_array(a))
    }
}

[expect_any_array(a)]
def combine(a, b) {
    //! Returns a new array containing elements from a followed by b.
    unsafe {
        return combine(temp_array(a), temp_array(b))
    }
}

[expect_any_array(a)]
def lower_bound(a; f, l : int; val) {
    //! Returns the index of the first element in the range [f, l) that is not less than val, or l if no such element is found.
    static_if (typeinfo stripped_typename(a[0]) != typeinfo stripped_typename(val)) {
        concept_assert(false, "value type {typeinfo stripped_typename(val)} should be the same as array {typeinfo stripped_typename(a)} element type {typeinfo stripped_typename(a[0])}")
        return -1
    } else {
        unsafe {
            return lower_bound(temp_array(a), f, l, val)
        }
    }
}

[expect_any_array(a)]
def lower_bound(a; val) {
    //! Returns the index of the first element in the array that is not less than val, or length(a) if no such element is found.
    static_if (typeinfo stripped_typename(a[0]) != typeinfo stripped_typename(val)) {
        concept_assert(false, "value type {typeinfo stripped_typename(val)} should be the same as array {typeinfo stripped_typename(a)} element type {typeinfo stripped_typename(a[0])}")
        return -1
    } else {
        unsafe {
            return lower_bound(temp_array(a), val)
        }
    }
}

[expect_any_array(a)]
def lower_bound(a; f, l : int; val : auto(TT); less : block<(a, b : TT const -&) : bool>) {
    //! Returns the index of the first element in the range [f, l) for which less returns false, or l if no such element is found.
    static_if (typeinfo stripped_typename(a[0]) != typeinfo stripped_typename(val)) {
        concept_assert(false, "value type {typeinfo stripped_typename(val)} should be the same as array {typeinfo stripped_typename(a)} element type {typeinfo stripped_typename(a[0])}")
        return -1
    } else {
        unsafe {
            return lower_bound(temp_array(a), f, l, val, less)
        }
    }
}

[expect_any_array(a)]
def lower_bound(a; val : auto(TT); less : block<(a, b : TT const -&) : bool>) {
    //! Returns the index of the first element in the array for which less returns false, or length(a) if no such element is found.
    static_if (typeinfo stripped_typename(a[0]) != typeinfo stripped_typename(val)) {
        concept_assert(false, "value type {typeinfo stripped_typename(val)} should be the same as array {typeinfo stripped_typename(a)} element type {typeinfo stripped_typename(a[0])}")
        return -1
    } else {
        unsafe {
            return lower_bound(temp_array(a), val, less)
        }
    }
}

[expect_any_array(a)]
def binary_search(a; val) {
    //! Returns true if val appears within the array a.
    static_if (typeinfo stripped_typename(a[0]) != typeinfo stripped_typename(val)) {
        concept_assert(false, "value type {typeinfo stripped_typename(val)} should be the same as array {typeinfo stripped_typename(a)} element type {typeinfo stripped_typename(a[0])}")
        return -1
    } else {
        unsafe {
            return binary_search(temp_array(a), val)
        }
    }
}

[expect_any_array(a)]
def binary_search(a; f, last : int; val) {
    //! Returns true if val appears within the range [f, last).
    static_if (typeinfo stripped_typename(a[0]) != typeinfo stripped_typename(val)) {
        concept_assert(false, "value type {typeinfo stripped_typename(val)} should be the same as array {typeinfo stripped_typename(a)} element type {typeinfo stripped_typename(a[0])}")
        return -1
    } else {
        unsafe {
            return binary_search(temp_array(a), f, last, val)
        }
    }
}

[expect_any_array(a)]
def binary_search(a; val : auto(TT); less : block<(a, b : TT const -&) : bool>) {
    //! Returns true if val appears within the array a.
    static_if (typeinfo stripped_typename(a[0]) != typeinfo stripped_typename(val)) {
        concept_assert(false, "value type {typeinfo stripped_typename(val)} should be the same as array {typeinfo stripped_typename(a)} element type {typeinfo stripped_typename(a[0])}")
        return -1
    } else {
        unsafe {
            return binary_search(temp_array(a), val, less)
        }
    }
}

[expect_any_array(a)]
def binary_search(a; f, last : int; val : auto(TT); less : block<(a, b : TT const -&) : bool>) {
    //! Returns true if val appears within the range [f, last).
    static_if (typeinfo stripped_typename(a[0]) != typeinfo stripped_typename(val)) {
        concept_assert(false, "value type {typeinfo stripped_typename(val)} should be the same as array {typeinfo stripped_typename(a)} element type {typeinfo stripped_typename(a[0])}")
        return -1
    } else {
        unsafe {
            return binary_search(temp_array(a), f, last, val, less)
        }
    }
}

[expect_any_array(arr)]
def erase_all(var arr; value) {
    //! Erases all elements equal to value from arr.
    static_if (typeinfo stripped_typename(arr[0]) != typeinfo stripped_typename(value)) {
        concept_assert(false, "value type {typeinfo stripped_typename(value)} should be the same as array {typeinfo stripped_typename(arr)} element type {typeinfo stripped_typename(arr[0])}")
        return -1
    } else {
        var i = length(arr) - 1
        while (i >= 0) {
            if (arr[i] == value) {
                erase(arr, i)
            }
            i --
        }
    }
}

def topological_sort(nodes : array<auto(Node)>) {
    //! Topological sort of a graph.
    //! Each node has an id, and set (table with no values) of dependencies.
    //! Dependency `before` represents a link from a node, which should appear in the sorted list before the node.
    //! Returns a sorted list of nodes.
    static_if (!typeinfo has_field <before>(type<Node>) || !typeinfo is_table(nodes[0].before) || !typeinfo has_field <id>(type<Node>)) {
        concept_assert(false, "Node type {typeinfo stripped_typename(type<Node>)} should have fields 'before:table<id>' and 'id'")
    } else {
        var sorted : array<Node>
        let lnodes = length(nodes)
        if (lnodes != 0) {
            sorted |> reserve(lnodes)
            var unsorted : array<Node> := nodes
            while (length(unsorted) > 0) {
                var node <- unsorted[0]
                unsorted |> erase(0)
                if (length(node.before) == 0) {
                    for (other in unsorted) {
                        other.before |> erase(node.id)
                    }
                    sorted |> emplace(node)
                } else {
                    unsorted |> emplace(node)
                }
            }
            if (length(sorted) != lnodes) {
                panic("cyclic dependency detected")
            }
            delete unsorted
        }
        return <- sorted
    }
}

def intersection(a, b : table<auto(TT)>) : table<TT> {
    //! Returns the intersection of two sets.
    var result : table<TT>
    if (length(a) < length(b)) {
        for (k in keys(a)) {
            if (b |> key_exists(k)) {
                result |> insert(k)
            }
        }
    } else {
        for (k in keys(b)) {
            if (a |> key_exists(k)) {
                result |> insert(k)
            }
        }
    }
    return <- result
}

def union(a, b : table<auto(TT)>) : table<TT> {
    //! Returns the union of two sets.
    var result : table<TT>
    for (k in keys(a)) {
        result |> insert(k)
    }
    for (k in keys(b)) {
        result |> insert(k)
    }
    return <- result
}

def difference(a, b : table<auto(TT)>) : table<TT> {
    //! Returns the difference of two sets.
    var result : table<TT>
    for (k in keys(a)) {
        if (!b |> key_exists(k)) {
            result |> insert(k)
        }
    }
    return <- result
}

def identical(a, b : table<auto(TT)>) : bool {
    //! Returns true if the two sets are identical.
    if (length(a) != length(b)) {
        return false
    }
    for (k in keys(a)) {
        if (!b |> key_exists(k)) {
            return false
        }
    }
    return true
}
