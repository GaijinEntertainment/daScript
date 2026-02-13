
.. _stdlib_algorithm:

========================
Miscellaneous algorithms
========================

The ALGORITHM module provides array and collection manipulation algorithms including
sorting, searching, set operations, and element removal.

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
            reverse(arr)
            print("reversed: {arr}\n")
        }
        // output:
        // sort_unique: [[ 1; 2; 3; 4; 5; 6; 9]]
        // has 4: true
        // has 7: false
        // lower_bound(4): 3
        // reversed: [[ 9; 6; 5; 4; 3; 2; 1]]

++++++
Search
++++++

  *  :ref:`binary_search (a: auto; val: auto(TT); less: block\<(a:TT;b:TT):bool\>) : auto <function-algorithm_binary_search_auto_autoTT_block_ls_a_c_TT;b_c_TT_c_bool_gr__0xf0>` 
  *  :ref:`binary_search (a: array\<auto(TT)\>; val: TT) : auto <function-algorithm_binary_search_array_ls_autoTT_gr__TT>` 
  *  :ref:`binary_search (a: auto; f: int; last: int; val: auto) : auto <function-algorithm_binary_search_auto_int_int_auto_0xe3>` 
  *  :ref:`binary_search (a: auto; f: int; last: int; val: auto(TT); less: block\<(a:TT;b:TT):bool\>) : auto <function-algorithm_binary_search_auto_int_int_autoTT_block_ls_a_c_TT;b_c_TT_c_bool_gr__0xfd>` 
  *  :ref:`binary_search (a: array\<auto(TT)\>; val: TT; less: block\<(a:TT;b:TT):bool\>) : auto <function-algorithm_binary_search_array_ls_autoTT_gr__TT_block_ls_a_c_TT;b_c_TT_c_bool_gr_>` 
  *  :ref:`binary_search (a: array\<auto(TT)\>; f: int; last: int; val: TT) : auto <function-algorithm_binary_search_array_ls_autoTT_gr__int_int_TT>` 
  *  :ref:`binary_search (a: array\<auto(TT)\>; f: int; last: int; val: TT; less: block\<(a:TT;b:TT):bool\>) : auto <function-algorithm_binary_search_array_ls_autoTT_gr__int_int_TT_block_ls_a_c_TT;b_c_TT_c_bool_gr_>` 
  *  :ref:`binary_search (a: auto; val: auto) : auto <function-algorithm_binary_search_auto_auto_0xd6>` 
  *  :ref:`lower_bound (a: array\<auto(TT)\>; value: auto(QQ); less: block\<(a:TT;b:QQ):bool\>) : auto <function-algorithm_lower_bound_array_ls_autoTT_gr__autoQQ_block_ls_a_c_TT;b_c_QQ_c_bool_gr__0x6d>` 
  *  :ref:`lower_bound (a: array\<auto(TT)\>; val: TT) : auto <function-algorithm_lower_bound_array_ls_autoTT_gr__TT>` 
  *  :ref:`lower_bound (a: auto; val: auto) : auto <function-algorithm_lower_bound_auto_auto_0xaf>` 
  *  :ref:`lower_bound (a: auto; f: int; l: int; val: auto(TT); less: block\<(a:TT;b:TT):bool\>) : auto <function-algorithm_lower_bound_auto_int_int_autoTT_block_ls_a_c_TT;b_c_TT_c_bool_gr__0xbc>` 
  *  :ref:`lower_bound (a: auto; val: auto(TT); less: block\<(a:TT;b:TT):bool\>) : auto <function-algorithm_lower_bound_auto_autoTT_block_ls_a_c_TT;b_c_TT_c_bool_gr__0xc9>` 
  *  :ref:`lower_bound (a: array\<auto(TT)\>; f: int; l: int; val: TT) : auto <function-algorithm_lower_bound_array_ls_autoTT_gr__int_int_TT>` 
  *  :ref:`lower_bound (a: array\<auto(TT)\>; f: int; l: int; value: auto(QQ); less: block\<(a:TT;b:QQ):bool\>) : auto <function-algorithm_lower_bound_array_ls_autoTT_gr__int_int_autoQQ_block_ls_a_c_TT;b_c_QQ_c_bool_gr__0x5a>` 
  *  :ref:`lower_bound (a: auto; f: int; l: int; val: auto) : auto <function-algorithm_lower_bound_auto_int_int_auto_0xa2>` 


binary_search
^^^^^^^^^^^^^

.. _function-algorithm_binary_search_auto_autoTT_block_ls_a_c_TT;b_c_TT_c_bool_gr__0xf0:

.. das:function:: binary_search(a: auto; val: auto(TT); less: block<(a:TT;b:TT):bool>) : auto

Returns true if val appears within the array a.

:Arguments: * **a** : auto

            * **val** : auto(TT)

            * **less** : block<(a:TT;b:TT):bool>

.. _function-algorithm_binary_search_array_ls_autoTT_gr__TT:

.. das:function:: binary_search(a: array<auto(TT)>; val: TT) : auto

.. _function-algorithm_binary_search_auto_int_int_auto_0xe3:

.. das:function:: binary_search(a: auto; f: int; last: int; val: auto) : auto

.. _function-algorithm_binary_search_auto_int_int_autoTT_block_ls_a_c_TT;b_c_TT_c_bool_gr__0xfd:

.. das:function:: binary_search(a: auto; f: int; last: int; val: auto(TT); less: block<(a:TT;b:TT):bool>) : auto

.. _function-algorithm_binary_search_array_ls_autoTT_gr__TT_block_ls_a_c_TT;b_c_TT_c_bool_gr_:

.. das:function:: binary_search(a: array<auto(TT)>; val: TT; less: block<(a:TT;b:TT):bool>) : auto

.. _function-algorithm_binary_search_array_ls_autoTT_gr__int_int_TT:

.. das:function:: binary_search(a: array<auto(TT)>; f: int; last: int; val: TT) : auto

.. _function-algorithm_binary_search_array_ls_autoTT_gr__int_int_TT_block_ls_a_c_TT;b_c_TT_c_bool_gr_:

.. das:function:: binary_search(a: array<auto(TT)>; f: int; last: int; val: TT; less: block<(a:TT;b:TT):bool>) : auto

.. _function-algorithm_binary_search_auto_auto_0xd6:

.. das:function:: binary_search(a: auto; val: auto) : auto

----


lower_bound
^^^^^^^^^^^

.. _function-algorithm_lower_bound_array_ls_autoTT_gr__autoQQ_block_ls_a_c_TT;b_c_QQ_c_bool_gr__0x6d:

.. das:function:: lower_bound(a: array<auto(TT)>; value: auto(QQ); less: block<(a:TT;b:QQ):bool>) : auto

Returns the index of the first element in the array for which less returns false, or length(a) if no such element is found.

:Arguments: * **a** : array<auto(TT)>

            * **value** : auto(QQ)

            * **less** : block<(a:TT;b:QQ):bool>

.. _function-algorithm_lower_bound_array_ls_autoTT_gr__TT:

.. das:function:: lower_bound(a: array<auto(TT)>; val: TT) : auto

.. _function-algorithm_lower_bound_auto_auto_0xaf:

.. das:function:: lower_bound(a: auto; val: auto) : auto

.. _function-algorithm_lower_bound_auto_int_int_autoTT_block_ls_a_c_TT;b_c_TT_c_bool_gr__0xbc:

.. das:function:: lower_bound(a: auto; f: int; l: int; val: auto(TT); less: block<(a:TT;b:TT):bool>) : auto

.. _function-algorithm_lower_bound_auto_autoTT_block_ls_a_c_TT;b_c_TT_c_bool_gr__0xc9:

.. das:function:: lower_bound(a: auto; val: auto(TT); less: block<(a:TT;b:TT):bool>) : auto

.. _function-algorithm_lower_bound_array_ls_autoTT_gr__int_int_TT:

.. das:function:: lower_bound(a: array<auto(TT)>; f: int; l: int; val: TT) : auto

.. _function-algorithm_lower_bound_array_ls_autoTT_gr__int_int_autoQQ_block_ls_a_c_TT;b_c_QQ_c_bool_gr__0x5a:

.. das:function:: lower_bound(a: array<auto(TT)>; f: int; l: int; value: auto(QQ); less: block<(a:TT;b:QQ):bool>) : auto

.. _function-algorithm_lower_bound_auto_int_int_auto_0xa2:

.. das:function:: lower_bound(a: auto; f: int; l: int; val: auto) : auto

----

++++++++++++++++++
Array manipulation
++++++++++++++++++

  *  :ref:`combine (a: array\<auto(TT)\>; b: array\<auto(TT)\>) : auto <function-algorithm_combine_array_ls_autoTT_gr__array_ls_autoTT_gr_>` 
  *  :ref:`combine (a: auto; b: auto) : auto <function-algorithm_combine_auto_auto_0x9a>` 
  *  :ref:`erase_all (var arr: auto; value: auto) : auto <function-algorithm_erase_all_auto_auto_0x10a>` 
  *  :ref:`reverse (var a: auto) : auto <function-algorithm_reverse_auto_0x92>` 
  *  :ref:`reverse (var a: array\<auto\>) : auto <function-algorithm_reverse_array_ls_auto_gr_>` 
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

.. _function-algorithm_combine_auto_auto_0x9a:

.. das:function:: combine(a: auto; b: auto) : auto

----

.. _function-algorithm_erase_all_auto_auto_0x10a:

.. das:function:: erase_all(arr: auto; value: auto) : auto

Erases all elements equal to value from arr.

:Arguments: * **arr** : auto

            * **value** : auto


reverse
^^^^^^^

.. _function-algorithm_reverse_auto_0x92:

.. das:function:: reverse(a: auto) : auto

Reverses the elements of array a in place.

:Arguments: * **a** : auto

.. _function-algorithm_reverse_array_ls_auto_gr_:

.. das:function:: reverse(a: array<auto>) : auto

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

Returns an array of elements from a with duplicates removed.

:Arguments: * **a** : array<auto(TT)>

++++++++++++++++++
Table manipulation
++++++++++++++++++

  *  :ref:`difference (a: table\<auto(TT), void\>; b: table\<auto(TT), void\>) : table\<TT, void\> <function-algorithm_difference_table_ls_autoTT,_void_gr__table_ls_autoTT,_void_gr_>` 
  *  :ref:`identical (a: table\<auto(TT), void\>; b: table\<auto(TT), void\>) : bool <function-algorithm_identical_table_ls_autoTT,_void_gr__table_ls_autoTT,_void_gr_>` 
  *  :ref:`intersection (a: table\<auto(TT), void\>; b: table\<auto(TT), void\>) : table\<TT, void\> <function-algorithm_intersection_table_ls_autoTT,_void_gr__table_ls_autoTT,_void_gr_>` 
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

.. _function-algorithm_union_table_ls_autoTT,_void_gr__table_ls_autoTT,_void_gr_:

.. das:function:: union(a: table<auto(TT), void>; b: table<auto(TT), void>) : table<TT, void>

Returns the union of two sets.

:Arguments: * **a** : table<auto(TT);void>

            * **b** : table<auto(TT);void>


