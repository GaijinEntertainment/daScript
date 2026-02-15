
.. _stdlib_algorithm:

========================
Miscellaneous algorithms
========================

The ALGORITHM module provides array and collection manipulation algorithms including
sorting, searching, set operations, element removal, and more.

Key features:

* **Search**: `lower_bound`, `upper_bound`, `binary_search`, `equal_range`
* **Array manipulation**: `unique`, `sort_unique`, `reverse`, `combine`, `fill`, `rotate`, `erase_all`, `min_element`, `max_element`, `is_sorted`, `topological_sort`
* **Set operations** (on tables): `intersection`, `union`, `difference`, `symmetric_difference`, `identical`, `is_subset`

Most functions also support fixed-size arrays via `[expect_any_array]` overloads.

All functions and symbols are in "algorithm" module, use require to get access to it. ::

    require daslib/algorithm

Example: ::

    require daslib/algorithm

        [export]
        def main() {
            var arr <- [3, 1, 4, 1, 5, 9, 2, 6, 5]
            sort_unique(arr)
            print("sort_unique: {arr}\n")
            print("has 4: {binary_search(arr, 4)}\n")
            print("has 7: {binary_search(arr, 7)}\n")
            print("lower_bound(4): {lower_bound(arr, 4)}\n")
            print("upper_bound(4): {upper_bound(arr, 4)}\n")
            let er = equal_range(arr, 5)
            print("equal_range(5): {er}\n")
            print("min index: {min_element(arr)}\n")
            print("is_sorted: {is_sorted(arr)}\n")
        }

++++++
Search
++++++

  *  :ref:`binary_search (a: auto; f: int; last: int; val: auto) : auto <function-algorithm_binary_search_auto_int_int_auto_0x12a>` 
  *  :ref:`binary_search (a: array\<auto(TT)\>; val: TT) : auto <function-algorithm_binary_search_array_ls_autoTT_gr__TT>` 
  *  :ref:`binary_search (a: auto; f: int; last: int; val: auto(TT); less: block\<(a:TT;b:TT):bool\>) : auto <function-algorithm_binary_search_auto_int_int_autoTT_block_ls_a_c_TT;b_c_TT_c_bool_gr__0x144>` 
  *  :ref:`binary_search (a: auto; val: auto) : auto <function-algorithm_binary_search_auto_auto_0x11d>` 
  *  :ref:`binary_search (a: array\<auto(TT)\>; val: TT; less: block\<(a:TT;b:TT):bool\>) : auto <function-algorithm_binary_search_array_ls_autoTT_gr__TT_block_ls_a_c_TT;b_c_TT_c_bool_gr_>` 
  *  :ref:`binary_search (a: array\<auto(TT)\>; f: int; last: int; val: TT) : auto <function-algorithm_binary_search_array_ls_autoTT_gr__int_int_TT>` 
  *  :ref:`binary_search (a: array\<auto(TT)\>; f: int; last: int; val: TT; less: block\<(a:TT;b:TT):bool\>) : auto <function-algorithm_binary_search_array_ls_autoTT_gr__int_int_TT_block_ls_a_c_TT;b_c_TT_c_bool_gr_>` 
  *  :ref:`binary_search (a: auto; val: auto(TT); less: block\<(a:TT;b:TT):bool\>) : auto <function-algorithm_binary_search_auto_autoTT_block_ls_a_c_TT;b_c_TT_c_bool_gr__0x137>` 
  *  :ref:`equal_range (a: array\<auto(TT)\>; f: int; l: int; val: TT) : auto <function-algorithm_equal_range_array_ls_autoTT_gr__int_int_TT>` 
  *  :ref:`equal_range (a: array\<auto(TT)\>; val: TT) : auto <function-algorithm_equal_range_array_ls_autoTT_gr__TT>` 
  *  :ref:`lower_bound (a: array\<auto(TT)\>; value: auto(QQ); less: block\<(a:TT;b:QQ):bool\>) : auto <function-algorithm_lower_bound_array_ls_autoTT_gr__autoQQ_block_ls_a_c_TT;b_c_QQ_c_bool_gr__0x70>` 
  *  :ref:`lower_bound (a: array\<auto(TT)\>; val: TT) : auto <function-algorithm_lower_bound_array_ls_autoTT_gr__TT>` 
  *  :ref:`lower_bound (a: auto; val: auto(TT); less: block\<(a:TT;b:TT):bool\>) : auto <function-algorithm_lower_bound_auto_autoTT_block_ls_a_c_TT;b_c_TT_c_bool_gr__0x110>` 
  *  :ref:`lower_bound (a: array\<auto(TT)\>; f: int; l: int; val: TT) : auto <function-algorithm_lower_bound_array_ls_autoTT_gr__int_int_TT>` 
  *  :ref:`lower_bound (a: array\<auto(TT)\>; f: int; l: int; value: auto(QQ); less: block\<(a:TT;b:QQ):bool\>) : auto <function-algorithm_lower_bound_array_ls_autoTT_gr__int_int_autoQQ_block_ls_a_c_TT;b_c_QQ_c_bool_gr__0x5d>` 
  *  :ref:`lower_bound (a: auto; f: int; l: int; val: auto) : auto <function-algorithm_lower_bound_auto_int_int_auto_0xe9>` 
  *  :ref:`lower_bound (a: auto; val: auto) : auto <function-algorithm_lower_bound_auto_auto_0xf6>` 
  *  :ref:`lower_bound (a: auto; f: int; l: int; val: auto(TT); less: block\<(a:TT;b:TT):bool\>) : auto <function-algorithm_lower_bound_auto_int_int_autoTT_block_ls_a_c_TT;b_c_TT_c_bool_gr__0x103>` 
  *  :ref:`upper_bound (a: array\<auto(TT)\>; f: int; l: int; value: auto(QQ); less: block\<(a:TT;b:QQ):bool\>) : auto <function-algorithm_upper_bound_array_ls_autoTT_gr__int_int_autoQQ_block_ls_a_c_TT;b_c_QQ_c_bool_gr__0xac>` 
  *  :ref:`upper_bound (a: array\<auto(TT)\>; val: TT) : auto <function-algorithm_upper_bound_array_ls_autoTT_gr__TT>` 
  *  :ref:`upper_bound (a: auto; f: int; l: int; val: auto) : auto <function-algorithm_upper_bound_auto_int_int_auto_0x151>` 
  *  :ref:`upper_bound (a: array\<auto(TT)\>; value: auto(QQ); less: block\<(a:TT;b:QQ):bool\>) : auto <function-algorithm_upper_bound_array_ls_autoTT_gr__autoQQ_block_ls_a_c_TT;b_c_QQ_c_bool_gr__0xbf>` 
  *  :ref:`upper_bound (a: auto; val: auto) : auto <function-algorithm_upper_bound_auto_auto_0x15e>` 
  *  :ref:`upper_bound (a: array\<auto(TT)\>; f: int; l: int; val: TT) : auto <function-algorithm_upper_bound_array_ls_autoTT_gr__int_int_TT>` 
  *  :ref:`upper_bound (a: auto; val: auto(TT); less: block\<(a:TT;b:TT):bool\>) : auto <function-algorithm_upper_bound_auto_autoTT_block_ls_a_c_TT;b_c_TT_c_bool_gr__0x178>` 
  *  :ref:`upper_bound (a: auto; f: int; l: int; val: auto(TT); less: block\<(a:TT;b:TT):bool\>) : auto <function-algorithm_upper_bound_auto_int_int_autoTT_block_ls_a_c_TT;b_c_TT_c_bool_gr__0x16b>` 


binary_search
^^^^^^^^^^^^^

.. _function-algorithm_binary_search_auto_int_int_auto_0x12a:

.. das:function:: binary_search(a: auto; f: int; last: int; val: auto) : auto

Returns true if val appears within the range [f, last).

:Arguments: * **a** : auto

            * **f** : int

            * **last** : int

            * **val** : auto

.. _function-algorithm_binary_search_array_ls_autoTT_gr__TT:

.. das:function:: binary_search(a: array<auto(TT)>; val: TT) : auto

.. _function-algorithm_binary_search_auto_int_int_autoTT_block_ls_a_c_TT;b_c_TT_c_bool_gr__0x144:

.. das:function:: binary_search(a: auto; f: int; last: int; val: auto(TT); less: block<(a:TT;b:TT):bool>) : auto

.. _function-algorithm_binary_search_auto_auto_0x11d:

.. das:function:: binary_search(a: auto; val: auto) : auto

.. _function-algorithm_binary_search_array_ls_autoTT_gr__TT_block_ls_a_c_TT;b_c_TT_c_bool_gr_:

.. das:function:: binary_search(a: array<auto(TT)>; val: TT; less: block<(a:TT;b:TT):bool>) : auto

.. _function-algorithm_binary_search_array_ls_autoTT_gr__int_int_TT:

.. das:function:: binary_search(a: array<auto(TT)>; f: int; last: int; val: TT) : auto

.. _function-algorithm_binary_search_array_ls_autoTT_gr__int_int_TT_block_ls_a_c_TT;b_c_TT_c_bool_gr_:

.. das:function:: binary_search(a: array<auto(TT)>; f: int; last: int; val: TT; less: block<(a:TT;b:TT):bool>) : auto

.. _function-algorithm_binary_search_auto_autoTT_block_ls_a_c_TT;b_c_TT_c_bool_gr__0x137:

.. das:function:: binary_search(a: auto; val: auto(TT); less: block<(a:TT;b:TT):bool>) : auto

----


equal_range
^^^^^^^^^^^

.. _function-algorithm_equal_range_array_ls_autoTT_gr__int_int_TT:

.. das:function:: equal_range(a: array<auto(TT)>; f: int; l: int; val: TT) : auto

Returns a pair of indices [lower, upper) bounding the range of elements equal to val within [f, l).
The array must be sorted.

:Arguments: * **a** : array<auto(TT)>

            * **f** : int

            * **l** : int

            * **val** : TT

.. _function-algorithm_equal_range_array_ls_autoTT_gr__TT:

.. das:function:: equal_range(a: array<auto(TT)>; val: TT) : auto

----


lower_bound
^^^^^^^^^^^

.. _function-algorithm_lower_bound_array_ls_autoTT_gr__autoQQ_block_ls_a_c_TT;b_c_QQ_c_bool_gr__0x70:

.. das:function:: lower_bound(a: array<auto(TT)>; value: auto(QQ); less: block<(a:TT;b:QQ):bool>) : auto

Returns the index of the first element in the array for which less returns false, or length(a) if no such element is found.

:Arguments: * **a** : array<auto(TT)>

            * **value** : auto(QQ)

            * **less** : block<(a:TT;b:QQ):bool>

.. _function-algorithm_lower_bound_array_ls_autoTT_gr__TT:

.. das:function:: lower_bound(a: array<auto(TT)>; val: TT) : auto

.. _function-algorithm_lower_bound_auto_autoTT_block_ls_a_c_TT;b_c_TT_c_bool_gr__0x110:

.. das:function:: lower_bound(a: auto; val: auto(TT); less: block<(a:TT;b:TT):bool>) : auto

.. _function-algorithm_lower_bound_array_ls_autoTT_gr__int_int_TT:

.. das:function:: lower_bound(a: array<auto(TT)>; f: int; l: int; val: TT) : auto

.. _function-algorithm_lower_bound_array_ls_autoTT_gr__int_int_autoQQ_block_ls_a_c_TT;b_c_QQ_c_bool_gr__0x5d:

.. das:function:: lower_bound(a: array<auto(TT)>; f: int; l: int; value: auto(QQ); less: block<(a:TT;b:QQ):bool>) : auto

.. _function-algorithm_lower_bound_auto_int_int_auto_0xe9:

.. das:function:: lower_bound(a: auto; f: int; l: int; val: auto) : auto

.. _function-algorithm_lower_bound_auto_auto_0xf6:

.. das:function:: lower_bound(a: auto; val: auto) : auto

.. _function-algorithm_lower_bound_auto_int_int_autoTT_block_ls_a_c_TT;b_c_TT_c_bool_gr__0x103:

.. das:function:: lower_bound(a: auto; f: int; l: int; val: auto(TT); less: block<(a:TT;b:TT):bool>) : auto

----


upper_bound
^^^^^^^^^^^

.. _function-algorithm_upper_bound_array_ls_autoTT_gr__int_int_autoQQ_block_ls_a_c_TT;b_c_QQ_c_bool_gr__0xac:

.. das:function:: upper_bound(a: array<auto(TT)>; f: int; l: int; value: auto(QQ); less: block<(a:TT;b:QQ):bool>) : auto

Returns the index of the first element in the range [f, l) for which less(val, element) returns true, or l if no such element is found.

:Arguments: * **a** : array<auto(TT)>

            * **f** : int

            * **l** : int

            * **value** : auto(QQ)

            * **less** : block<(a:TT;b:QQ):bool>

.. _function-algorithm_upper_bound_array_ls_autoTT_gr__TT:

.. das:function:: upper_bound(a: array<auto(TT)>; val: TT) : auto

.. _function-algorithm_upper_bound_auto_int_int_auto_0x151:

.. das:function:: upper_bound(a: auto; f: int; l: int; val: auto) : auto

.. _function-algorithm_upper_bound_array_ls_autoTT_gr__autoQQ_block_ls_a_c_TT;b_c_QQ_c_bool_gr__0xbf:

.. das:function:: upper_bound(a: array<auto(TT)>; value: auto(QQ); less: block<(a:TT;b:QQ):bool>) : auto

.. _function-algorithm_upper_bound_auto_auto_0x15e:

.. das:function:: upper_bound(a: auto; val: auto) : auto

.. _function-algorithm_upper_bound_array_ls_autoTT_gr__int_int_TT:

.. das:function:: upper_bound(a: array<auto(TT)>; f: int; l: int; val: TT) : auto

.. _function-algorithm_upper_bound_auto_autoTT_block_ls_a_c_TT;b_c_TT_c_bool_gr__0x178:

.. das:function:: upper_bound(a: auto; val: auto(TT); less: block<(a:TT;b:TT):bool>) : auto

.. _function-algorithm_upper_bound_auto_int_int_autoTT_block_ls_a_c_TT;b_c_TT_c_bool_gr__0x16b:

.. das:function:: upper_bound(a: auto; f: int; l: int; val: auto(TT); less: block<(a:TT;b:TT):bool>) : auto

----

++++++++++++++++++
Array manipulation
++++++++++++++++++

  *  :ref:`combine (a: array\<auto(TT)\>; b: array\<auto(TT)\>) : auto <function-algorithm_combine_array_ls_autoTT_gr__array_ls_autoTT_gr_>` 
  *  :ref:`combine (a: auto; b: auto) : auto <function-algorithm_combine_auto_auto_0xe1>` 
  *  :ref:`erase_all (var arr: auto; value: auto) : auto <function-algorithm_erase_all_auto_auto_0x185>` 
  *  :ref:`fill (var a: array\<auto(TT)\>; value: TT) : auto <function-algorithm_fill_array_ls_autoTT_gr__TT>` 
  *  :ref:`fill (var a: auto; value: auto) : auto <function-algorithm_fill_auto_auto_0x1a3>` 
  *  :ref:`is_sorted (a: auto) : bool <function-algorithm_is_sorted_auto_0x1c5>` 
  *  :ref:`is_sorted (a: array\<auto(TT)\>) : bool <function-algorithm_is_sorted_array_ls_autoTT_gr_>` 
  *  :ref:`is_sorted (a: array\<auto(TT)\>; less: block\<(a:TT;b:TT):bool\>) : bool <function-algorithm_is_sorted_array_ls_autoTT_gr__block_ls_a_c_TT;b_c_TT_c_bool_gr_>` 
  *  :ref:`max_element (a: auto) : int <function-algorithm_max_element_auto_0x230>` 
  *  :ref:`max_element (a: array\<auto(TT)\>; less: block\<(a:TT;b:TT):bool\>) : int <function-algorithm_max_element_array_ls_autoTT_gr__block_ls_a_c_TT;b_c_TT_c_bool_gr_>` 
  *  :ref:`max_element (a: array\<auto(TT)\>) : int <function-algorithm_max_element_array_ls_autoTT_gr_>` 
  *  :ref:`min_element (a: array\<auto(TT)\>; less: block\<(a:TT;b:TT):bool\>) : int <function-algorithm_min_element_array_ls_autoTT_gr__block_ls_a_c_TT;b_c_TT_c_bool_gr_>` 
  *  :ref:`min_element (a: auto) : int <function-algorithm_min_element_auto_0x228>` 
  *  :ref:`min_element (a: array\<auto(TT)\>) : int <function-algorithm_min_element_array_ls_autoTT_gr_>` 
  *  :ref:`reverse (var a: auto) : auto <function-algorithm_reverse_auto_0xd9>` 
  *  :ref:`reverse (var a: array\<auto\>) : auto <function-algorithm_reverse_array_ls_auto_gr_>` 
  *  :ref:`rotate (var a: array\<auto\>; mid: int) : auto <function-algorithm_rotate_array_ls_auto_gr__int>` 
  *  :ref:`rotate (var a: auto; mid: int) : auto <function-algorithm_rotate_auto_int_0x1e6>` 
  *  :ref:`sort_unique (var a: array\<auto(TT)\>) : auto <function-algorithm_sort_unique_array_ls_autoTT_gr_>` 
  *  :ref:`topological_sort (nodes: array\<auto(Node)\>) : auto <function-algorithm_topological_sort_array_ls_autoNode_gr_>` 
  *  :ref:`unique (var a: array\<auto(TT)\>) : auto <function-algorithm_unique_array_ls_autoTT_gr_>` 


combine
^^^^^^^

.. _function-algorithm_combine_array_ls_autoTT_gr__array_ls_autoTT_gr_:

.. das:function:: combine(a: array<auto(TT)>; b: array<auto(TT)>) : auto

Returns a new array containing elements from a followed by b.

:Arguments: * **a** : array<auto(TT)>

            * **b** : array<auto(TT)>

.. _function-algorithm_combine_auto_auto_0xe1:

.. das:function:: combine(a: auto; b: auto) : auto

----

.. _function-algorithm_erase_all_auto_auto_0x185:

.. das:function:: erase_all(arr: auto; value: auto) : auto

Erases all elements equal to value from arr in O(n) time.
Uses swap to support non-copyable types. Removed elements are swapped to the tail
and properly finalized by resize.

:Arguments: * **arr** : auto

            * **value** : auto


fill
^^^^

.. _function-algorithm_fill_array_ls_autoTT_gr__TT:

.. das:function:: fill(a: array<auto(TT)>; value: TT) : auto

Sets all elements of the array to the given value using clone.

:Arguments: * **a** : array<auto(TT)>

            * **value** : TT

.. _function-algorithm_fill_auto_auto_0x1a3:

.. das:function:: fill(a: auto; value: auto) : auto

----


is_sorted
^^^^^^^^^

.. _function-algorithm_is_sorted_auto_0x1c5:

.. das:function:: is_sorted(a: auto) : bool

Returns true if the array is sorted in non-descending order.

:Arguments: * **a** : auto

.. _function-algorithm_is_sorted_array_ls_autoTT_gr_:

.. das:function:: is_sorted(a: array<auto(TT)>) : bool

.. _function-algorithm_is_sorted_array_ls_autoTT_gr__block_ls_a_c_TT;b_c_TT_c_bool_gr_:

.. das:function:: is_sorted(a: array<auto(TT)>; less: block<(a:TT;b:TT):bool>) : bool

----


max_element
^^^^^^^^^^^

.. _function-algorithm_max_element_auto_0x230:

.. das:function:: max_element(a: auto) : int

Returns the index of the maximum element in the array, or -1 if the array is empty.

:Arguments: * **a** : auto

.. _function-algorithm_max_element_array_ls_autoTT_gr__block_ls_a_c_TT;b_c_TT_c_bool_gr_:

.. das:function:: max_element(a: array<auto(TT)>; less: block<(a:TT;b:TT):bool>) : int

.. _function-algorithm_max_element_array_ls_autoTT_gr_:

.. das:function:: max_element(a: array<auto(TT)>) : int

----


min_element
^^^^^^^^^^^

.. _function-algorithm_min_element_array_ls_autoTT_gr__block_ls_a_c_TT;b_c_TT_c_bool_gr_:

.. das:function:: min_element(a: array<auto(TT)>; less: block<(a:TT;b:TT):bool>) : int

Returns the index of the minimum element according to the provided less function, or -1 if the array is empty.

:Arguments: * **a** : array<auto(TT)>

            * **less** : block<(a:TT;b:TT):bool>

.. _function-algorithm_min_element_auto_0x228:

.. das:function:: min_element(a: auto) : int

.. _function-algorithm_min_element_array_ls_autoTT_gr_:

.. das:function:: min_element(a: array<auto(TT)>) : int

----


reverse
^^^^^^^

.. _function-algorithm_reverse_auto_0xd9:

.. das:function:: reverse(a: auto) : auto

Reverses the elements of array a in place.

:Arguments: * **a** : auto

.. _function-algorithm_reverse_array_ls_auto_gr_:

.. das:function:: reverse(a: array<auto>) : auto

----


rotate
^^^^^^

.. _function-algorithm_rotate_array_ls_auto_gr__int:

.. das:function:: rotate(a: array<auto>; mid: int) : auto

Rotates the array so that the element at index mid becomes the first element.
Elements before mid are moved to the end.

:Arguments: * **a** : array<auto>

            * **mid** : int

.. _function-algorithm_rotate_auto_int_0x1e6:

.. das:function:: rotate(a: auto; mid: int) : auto

----

.. _function-algorithm_sort_unique_array_ls_autoTT_gr_:

.. das:function:: sort_unique(a: array<auto(TT)>) : auto

Returns an array of elements from a, sorted and with duplicates removed.
The elements are sorted in ascending order.
The resulting array has only unique elements.

:Arguments: * **a** : array<auto(TT)>

.. _function-algorithm_topological_sort_array_ls_autoNode_gr_:

.. das:function:: topological_sort(nodes: array<auto(Node)>) : auto

Topological sort of a graph.
Each node has an id, and set (table with no values) of dependencies.
Dependency `before` represents a link from a node, which should appear in the sorted list before the node.
Returns a sorted list of nodes.

:Arguments: * **nodes** : array<auto(Node)>

.. _function-algorithm_unique_array_ls_autoTT_gr_:

.. das:function:: unique(a: array<auto(TT)>) : auto

Returns an array with adjacent duplicate elements removed.
The array should be sorted first if all duplicates need to be removed.

:Arguments: * **a** : array<auto(TT)>

++++++++++++++++++
Table manipulation
++++++++++++++++++

  *  :ref:`difference (a: table\<auto(TT), void\>; b: table\<auto(TT), void\>) : table\<TT, void\> <function-algorithm_difference_table_ls_autoTT,_void_gr__table_ls_autoTT,_void_gr_>` 
  *  :ref:`identical (a: table\<auto(TT), void\>; b: table\<auto(TT), void\>) : bool <function-algorithm_identical_table_ls_autoTT,_void_gr__table_ls_autoTT,_void_gr_>` 
  *  :ref:`intersection (a: table\<auto(TT), void\>; b: table\<auto(TT), void\>) : table\<TT, void\> <function-algorithm_intersection_table_ls_autoTT,_void_gr__table_ls_autoTT,_void_gr_>` 
  *  :ref:`is_subset (a: table\<auto(TT), void\>; b: table\<auto(TT), void\>) : bool <function-algorithm_is_subset_table_ls_autoTT,_void_gr__table_ls_autoTT,_void_gr_>` 
  *  :ref:`symmetric_difference (a: table\<auto(TT), void\>; b: table\<auto(TT), void\>) : table\<TT, void\> <function-algorithm_symmetric_difference_table_ls_autoTT,_void_gr__table_ls_autoTT,_void_gr_>` 
  *  :ref:`union (a: table\<auto(TT), void\>; b: table\<auto(TT), void\>) : table\<TT, void\> <function-algorithm_union_table_ls_autoTT,_void_gr__table_ls_autoTT,_void_gr_>` 

.. _function-algorithm_difference_table_ls_autoTT,_void_gr__table_ls_autoTT,_void_gr_:

.. das:function:: difference(a: table<auto(TT), void>; b: table<auto(TT), void>) : table<TT, void>

Returns the difference of two sets.

:Arguments: * **a** : table<auto(TT);void>

            * **b** : table<auto(TT);void>

.. _function-algorithm_identical_table_ls_autoTT,_void_gr__table_ls_autoTT,_void_gr_:

.. das:function:: identical(a: table<auto(TT), void>; b: table<auto(TT), void>) : bool

Returns true if the two sets are identical.

:Arguments: * **a** : table<auto(TT);void>

            * **b** : table<auto(TT);void>

.. _function-algorithm_intersection_table_ls_autoTT,_void_gr__table_ls_autoTT,_void_gr_:

.. das:function:: intersection(a: table<auto(TT), void>; b: table<auto(TT), void>) : table<TT, void>

Returns the intersection of two sets.

:Arguments: * **a** : table<auto(TT);void>

            * **b** : table<auto(TT);void>

.. _function-algorithm_is_subset_table_ls_autoTT,_void_gr__table_ls_autoTT,_void_gr_:

.. das:function:: is_subset(a: table<auto(TT), void>; b: table<auto(TT), void>) : bool

Returns true if all elements of a are contained in b.

:Arguments: * **a** : table<auto(TT);void>

            * **b** : table<auto(TT);void>

.. _function-algorithm_symmetric_difference_table_ls_autoTT,_void_gr__table_ls_autoTT,_void_gr_:

.. das:function:: symmetric_difference(a: table<auto(TT), void>; b: table<auto(TT), void>) : table<TT, void>

Returns the symmetric difference of two sets (elements in either set but not both).

:Arguments: * **a** : table<auto(TT);void>

            * **b** : table<auto(TT);void>

.. _function-algorithm_union_table_ls_autoTT,_void_gr__table_ls_autoTT,_void_gr_:

.. das:function:: union(a: table<auto(TT), void>; b: table<auto(TT), void>) : table<TT, void>

Returns the union of two sets.

:Arguments: * **a** : table<auto(TT);void>

            * **b** : table<auto(TT);void>


