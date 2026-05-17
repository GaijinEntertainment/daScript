.. _tutorial_algorithm:

========================
Algorithm
========================

.. index::
    single: Tutorial; Algorithm
    single: Tutorial; Binary Search
    single: Tutorial; Sort
    single: Tutorial; Partial Sort
    single: Tutorial; Heap Operations
    single: Tutorial; Top-N Selection
    single: Tutorial; Set Operations
    single: Tutorial; Topological Sort

This tutorial covers the ``algorithm`` module — a collection of general-purpose
algorithms for searching, sorting, manipulating arrays, and performing set
operations on tables. All functions live in ``daslib/algorithm`` and many also
support fixed-size arrays via ``[expect_any_array]`` overloads.

.. code-block:: das

    require daslib/algorithm

Binary search family
====================

The module provides the classic binary search family for sorted arrays:

* ``lower_bound`` — first element ``>= val``
* ``upper_bound`` — first element ``> val``
* ``binary_search`` — ``true`` if ``val`` exists
* ``equal_range`` — ``int2(lower_bound, upper_bound)``

Each comes with overloads for full or sub-range search and custom comparators:

.. code-block:: das

    var a <- [1, 2, 2, 2, 3, 5, 8, 13]
    print("lower_bound(2) = {lower_bound(a, 2)}\n")    // 1
    print("upper_bound(2) = {upper_bound(a, 2)}\n")    // 4
    print("equal_range(2)  = {equal_range(a, 2)}\n")   // (1, 4)
    print("binary_search(4) = {binary_search(a, 4)}\n") // false

Custom comparators let you search in non-default order:

.. code-block:: das

    var desc <- [9, 7, 5, 3, 1]
    let idx = lower_bound(desc, 5) $(lhs, rhs : int const) {
        return lhs > rhs
    }
    print("{idx}\n") // 2

Sort
====

``sort`` performs an in-place ascending sort using ``<`` by default. A block
argument supplies a custom comparator:

.. code-block:: das

    var a <- [5, 2, 8, 1, 9, 3, 7]
    sort(a)
    // [1, 2, 3, 5, 7, 8, 9]

    var b <- [5, 2, 8, 1, 9]
    sort(b) $(x, y : int) : bool {
        return x > y
    }
    // [9, 8, 5, 2, 1]

Today's ``sort`` is full ``O(n log n)`` on the whole array. The next section
shows what to use when you only need the first ``N`` sorted (``partial_sort``)
or only the ``k``-th element (``nth_element``).

partial_sort / nth_element
==========================

These live in ``daslib/sort_boost``. ``partial_sort(arr, N)`` sorts only the
first ``N`` elements ascending; the remainder is left in unspecified order:

.. code-block:: das

    require daslib/sort_boost

    var a <- [5, 2, 8, 1, 9, 3, 7, 4, 6, 0]
    sort_boost::partial_sort(a, 3)
    // a[0..3] = [0, 1, 2]; a[3..] is some permutation of the rest

``nth_element(arr, k)`` places the ``k``-th-smallest at position ``k``.
Elements before are ``<= a[k]``; elements after are ``>= a[k]``. Neither side
is fully sorted, but the ``k``-th position is correctly the ``k``-th-smallest:

.. code-block:: das

    var b <- [7, 3, 1, 9, 5, 8, 0, 4, 2]
    sort_boost::nth_element(b, 4)
    // b[4] == 4 (median of 9 elements)

Both accept a custom comparator block — same shape as ``sort``:

.. code-block:: das

    var c <- [PricePoint(item_id=1, price=50), PricePoint(item_id=2, price=20), ...]
    sort_boost::partial_sort(c, 2) $(x, y : PricePoint) : bool {
        return x.price < y.price
    }

Heap operations
===============

``daslib/sort_boost`` exposes the classic binary max-heap operators:
``make_heap`` builds a heap from an unordered array, ``push_heap`` sifts up
the just-appended element, ``pop_heap`` moves the heap max to the last slot
(caller drops it afterward):

.. code-block:: das

    var a <- [5, 2, 8, 1, 9, 3, 7]
    sort_boost::make_heap(a)
    // a[0] = 9 (max)

    a |> push(12)
    sort_boost::push_heap(a)
    // a[0] = 12 (new max)

    sort_boost::pop_heap(a)
    let max = a[length(a) - 1]
    a |> resize(length(a) - 1)
    // max = 12; remaining heap still valid

Repeatedly popping the max yields a descending sequence (heap sort):

.. code-block:: das

    var b <- [5, 2, 8, 1, 9, 3, 7]
    sort_boost::make_heap(b)
    var descending : array<int>
    while (length(b) > 0) {
        sort_boost::pop_heap(b)
        descending |> push(b[length(b) - 1])
        b |> resize(length(b) - 1)
    }
    // descending = [9, 8, 7, 5, 3, 2, 1]

top_n / top_n_by
================

``top_n(arr, N)`` returns the ``N`` smallest elements as a sorted ascending
array. For array sources it uses ``partial_sort`` under the hood
(``O(M log N)``):

.. code-block:: das

    require daslib/linq

    let arr <- [10, 20, 5, 8, 30, 15, 2, 25]
    let smallest3 <- top_n(arr, 3)
    // [2, 5, 8]

For iterator sources, ``top_n`` maintains a bounded heap of size ``N`` during
the scan — at most ``N+1`` elements are ever resident:

.. code-block:: das

    let arr2 <- [10, 20, 5, 8, 30, 15, 2, 25]
    let smallest3_iter <- top_n(arr2.to_sequence(), 3)
    // [2, 5, 8]

``top_n_by`` takes a key function. Useful for sorting structs by field
without writing a full comparator block:

.. code-block:: das

    struct Person {
        name : string
        age  : int
    }
    let people <- [Person(name = "Alice", age = 30), Person(name = "Bob", age = 25), ...]
    let youngest3 <- top_n_by(people, 3, @@(p : Person -&) => p.age)
    // sorted ascending by age

Sorting and deduplication
=========================

``sort_unique`` sorts an array in place and removes duplicates. ``unique``
removes only *adjacent* duplicates (like C++ ``std::unique``), so the array
should be sorted first for full deduplication:

.. code-block:: das

    var a <- [5, 3, 1, 3, 5, 1, 2]
    sort_unique(a)
    print("{a}\n")  // [1, 2, 3, 5]

    var b <- [3, 1, 3, 1]
    var c <- unique(b)
    print("{c}\n")  // [3, 1, 3, 1] — no adjacent dups removed

Array manipulation
==================

.. code-block:: das

    // reverse — in place
    var a <- [1, 2, 3, 4, 5]
    reverse(a)          // [5, 4, 3, 2, 1]

    // combine — concatenate into a new array
    var both <- combine([1, 2], [3, 4])  // [1, 2, 3, 4]

    // fill — set all elements
    fill(a, 0)          // [0, 0, 0, 0, 0]

    // rotate — mid becomes first
    var b <- [1, 2, 3, 4, 5]
    rotate(b, 2)        // [3, 4, 5, 1, 2]

    // erase_all — remove all occurrences of a value
    var c <- [1, 2, 3, 2, 4]
    erase_all(c, 2)     // [1, 3, 4]

Querying arrays
===============

.. code-block:: das

    var a <- [3, 1, 4, 1, 5, 9, 2, 6]
    print("is_sorted: {is_sorted(a)}\n")       // false
    print("min index: {min_element(a)}\n")      // 1 (value 1)
    print("max index: {max_element(a)}\n")      // 5 (value 9)

``min_element`` and ``max_element`` return the *index* (not the value) of the
minimum/maximum element, or ``-1`` for empty arrays. Both accept optional custom
comparators.

Set operations
==============

Tables with no value type serve as sets. The module provides:

.. code-block:: das

    var a <- { 1, 2, 3, 4 }
    var b <- { 3, 4, 5, 6 }

    var inter <- intersection(a, b)           // {3, 4}
    var uni   <- union(a, b)                  // {1, 2, 3, 4, 5, 6}
    var diff  <- difference(a, b)             // {1, 2}
    var sdiff <- symmetric_difference(a, b)   // {1, 2, 5, 6}

    print("identical: {identical(a, a)}\n")   // true
    print("is_subset({2, 3}, a): {is_subset({2, 3}, a)}\n") // true

Topological sort
================

``topological_sort`` orders nodes respecting dependency edges. Each node must
have an ``id`` field and a ``before`` table listing which ids must come first:

.. code-block:: das

    struct TsNode {
        id     : int
        before : table<int>
    }

    var nodes <- [TsNode(
        id=2, before <- {1}), TsNode(
        id=1, before <- {0}), TsNode(
        id=0
    )]
    var sorted <- topological_sort(nodes)
    // sorted: [0, 1, 2]

If the graph contains a cycle, ``topological_sort`` calls ``panic``.

Fixed-size arrays
=================

Most functions (``reverse``, ``fill``, ``lower_bound``, ``binary_search``,
``upper_bound``, ``min_element``, ``max_element``, ``is_sorted``, ``rotate``,
``erase_all``, ``combine``) also work on fixed-size arrays:

.. code-block:: das

    var a = fixed_array<int>(5, 3, 1, 4, 2)
    print("min: {min_element(a)}\n")    // 2 (value 1)
    reverse(a)                          // [2, 4, 1, 3, 5]

.. seealso::

   Full source: :download:`tutorials/language/33_algorithm.das <../../../../tutorials/language/33_algorithm.das>`

   Previous tutorial: :ref:`tutorial_operator_overloading`

   Next tutorial: :ref:`tutorial_decs`

   :ref:`Arrays <arrays>` — array language reference.

   :ref:`Tables <tables>` — table language reference.
