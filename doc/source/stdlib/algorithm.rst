
.. _stdlib_algorithm:

=======================
Miscelanious algorithms
=======================

The ALGORITHM module exposes collection of miscellaneous array manipulation algorithms.

All functions and symbols are in "algorithm" module, use require to get access to it. ::

    require daslib/algorithm

++++++
Search
++++++

  *  :ref:`lower_bound (a: array\<auto(TT)\>; f: int; l: int; val: TT) : auto <function-algorithm_lower_bound_array_ls_autoTT_gr__int_int_TT>` 
  *  :ref:`lower_bound (a: array\<auto(TT)\>; val: TT) : auto <function-algorithm_lower_bound_array_ls_autoTT_gr__TT>` 
  *  :ref:`lower_bound (a: array\<auto(TT)\>; f: int; l: int; value: auto(QQ); less: block\<(a:TT;b:QQ):bool\>) : auto <function-algorithm_lower_bound_array_ls_autoTT_gr__int_int_autoQQ_block_ls_a_c_TT;b_c_QQ_c_bool_gr_>` 
  *  :ref:`lower_bound (a: array\<auto(TT)\>; value: auto(QQ); less: block\<(a:TT;b:QQ):bool\>) : auto <function-algorithm_lower_bound_array_ls_autoTT_gr__autoQQ_block_ls_a_c_TT;b_c_QQ_c_bool_gr_>` 
  *  :ref:`binary_search (a: array\<auto(TT)\>; val: TT) : auto <function-algorithm_binary_search_array_ls_autoTT_gr__TT>` 
  *  :ref:`binary_search (a: array\<auto(TT)\>; f: int; last: int; val: TT) : auto <function-algorithm_binary_search_array_ls_autoTT_gr__int_int_TT>` 
  *  :ref:`binary_search (a: array\<auto(TT)\>; val: TT; less: block\<(a:TT;b:TT):bool\>) : auto <function-algorithm_binary_search_array_ls_autoTT_gr__TT_block_ls_a_c_TT;b_c_TT_c_bool_gr_>` 
  *  :ref:`binary_search (a: array\<auto(TT)\>; f: int; last: int; val: TT; less: block\<(a:TT;b:TT):bool\>) : auto <function-algorithm_binary_search_array_ls_autoTT_gr__int_int_TT_block_ls_a_c_TT;b_c_TT_c_bool_gr_>` 
  *  :ref:`lower_bound (a: auto; f: int; l: int; val: auto) : auto <function-algorithm_lower_bound_auto_int_int_auto>` 
  *  :ref:`lower_bound (a: auto; val: auto) : auto <function-algorithm_lower_bound_auto_auto>` 
  *  :ref:`lower_bound (a: auto; f: int; l: int; val: auto(TT); less: block\<(a:TT;b:TT):bool\>) : auto <function-algorithm_lower_bound_auto_int_int_autoTT_block_ls_a_c_TT;b_c_TT_c_bool_gr_>` 
  *  :ref:`lower_bound (a: auto; val: auto(TT); less: block\<(a:TT;b:TT):bool\>) : auto <function-algorithm_lower_bound_auto_autoTT_block_ls_a_c_TT;b_c_TT_c_bool_gr_>` 
  *  :ref:`binary_search (a: auto; val: auto) : auto <function-algorithm_binary_search_auto_auto>` 
  *  :ref:`binary_search (a: auto; f: int; last: int; val: auto) : auto <function-algorithm_binary_search_auto_int_int_auto>` 
  *  :ref:`binary_search (a: auto; val: auto(TT); less: block\<(a:TT;b:TT):bool\>) : auto <function-algorithm_binary_search_auto_autoTT_block_ls_a_c_TT;b_c_TT_c_bool_gr_>` 
  *  :ref:`binary_search (a: auto; f: int; last: int; val: auto(TT); less: block\<(a:TT;b:TT):bool\>) : auto <function-algorithm_binary_search_auto_int_int_autoTT_block_ls_a_c_TT;b_c_TT_c_bool_gr_>` 

.. _function-algorithm_lower_bound_array_ls_autoTT_gr__int_int_TT:

.. das:function:: lower_bound(a: array<auto(TT)>; f: int; l: int; val: TT) : auto

Returns an iterator pointing to the first element in the range [first, last) that is not less than (i.e. greater or equal to) value, or last if no such element is found.

:Arguments: * **a** : array<auto(TT)>

            * **f** : int

            * **l** : int

            * **val** : TT

.. _function-algorithm_lower_bound_array_ls_autoTT_gr__TT:

.. das:function:: lower_bound(a: array<auto(TT)>; val: TT) : auto

|detail/function-algorithm-lower_bound-0xb469fc2f44d14195|

:Arguments: * **a** : array<auto(TT)>

            * **val** : TT

.. _function-algorithm_lower_bound_array_ls_autoTT_gr__int_int_autoQQ_block_ls_a_c_TT;b_c_QQ_c_bool_gr_:

.. das:function:: lower_bound(a: array<auto(TT)>; f: int; l: int; value: auto(QQ); less: block<(a:TT;b:QQ):bool>) : auto

|detail/function-algorithm-lower_bound-0x65d7ba53126067c3|

:Arguments: * **a** : array<auto(TT)>

            * **f** : int

            * **l** : int

            * **value** : auto(QQ)

            * **less** : block<(a:TT;b:QQ):bool>

.. _function-algorithm_lower_bound_array_ls_autoTT_gr__autoQQ_block_ls_a_c_TT;b_c_QQ_c_bool_gr_:

.. das:function:: lower_bound(a: array<auto(TT)>; value: auto(QQ); less: block<(a:TT;b:QQ):bool>) : auto

|detail/function-algorithm-lower_bound-0x1c9077e733599e3c|

:Arguments: * **a** : array<auto(TT)>

            * **value** : auto(QQ)

            * **less** : block<(a:TT;b:QQ):bool>

.. _function-algorithm_binary_search_array_ls_autoTT_gr__TT:

.. das:function:: binary_search(a: array<auto(TT)>; val: TT) : auto

|detail/function-algorithm-binary_search-0xad4ffd6c53c3193|

:Arguments: * **a** : array<auto(TT)>

            * **val** : TT

.. _function-algorithm_binary_search_array_ls_autoTT_gr__int_int_TT:

.. das:function:: binary_search(a: array<auto(TT)>; f: int; last: int; val: TT) : auto

Returns true if an val appears within the range [f, last).
Array a must be sorted.

:Arguments: * **a** : array<auto(TT)>

            * **f** : int

            * **last** : int

            * **val** : TT

.. _function-algorithm_binary_search_array_ls_autoTT_gr__TT_block_ls_a_c_TT;b_c_TT_c_bool_gr_:

.. das:function:: binary_search(a: array<auto(TT)>; val: TT; less: block<(a:TT;b:TT):bool>) : auto

|detail/function-algorithm-binary_search-0x289919af628da4e1|

:Arguments: * **a** : array<auto(TT)>

            * **val** : TT

            * **less** : block<(a:TT;b:TT):bool>

.. _function-algorithm_binary_search_array_ls_autoTT_gr__int_int_TT_block_ls_a_c_TT;b_c_TT_c_bool_gr_:

.. das:function:: binary_search(a: array<auto(TT)>; f: int; last: int; val: TT; less: block<(a:TT;b:TT):bool>) : auto

|detail/function-algorithm-binary_search-0x1f96034a82a36c98|

:Arguments: * **a** : array<auto(TT)>

            * **f** : int

            * **last** : int

            * **val** : TT

            * **less** : block<(a:TT;b:TT):bool>

.. _function-algorithm_lower_bound_auto_int_int_auto:

.. das:function:: lower_bound(a: auto; f: int; l: int; val: auto) : auto

|detail/function-algorithm-lower_bound-0x9e462d5940438790|

:Arguments: * **a** : auto

            * **f** : int

            * **l** : int

            * **val** : auto

.. _function-algorithm_lower_bound_auto_auto:

.. das:function:: lower_bound(a: auto; val: auto) : auto

|detail/function-algorithm-lower_bound-0x8cf240a3a8ad3c21|

:Arguments: * **a** : auto

            * **val** : auto

.. _function-algorithm_lower_bound_auto_int_int_autoTT_block_ls_a_c_TT;b_c_TT_c_bool_gr_:

.. das:function:: lower_bound(a: auto; f: int; l: int; val: auto(TT); less: block<(a:TT;b:TT):bool>) : auto

|detail/function-algorithm-lower_bound-0x1cbae16136742bf7|

:Arguments: * **a** : auto

            * **f** : int

            * **l** : int

            * **val** : auto(TT)

            * **less** : block<(a:TT;b:TT):bool>

.. _function-algorithm_lower_bound_auto_autoTT_block_ls_a_c_TT;b_c_TT_c_bool_gr_:

.. das:function:: lower_bound(a: auto; val: auto(TT); less: block<(a:TT;b:TT):bool>) : auto

|detail/function-algorithm-lower_bound-0xc56e85976e59df7c|

:Arguments: * **a** : auto

            * **val** : auto(TT)

            * **less** : block<(a:TT;b:TT):bool>

.. _function-algorithm_binary_search_auto_auto:

.. das:function:: binary_search(a: auto; val: auto) : auto

|detail/function-algorithm-binary_search-0x6140c5dea5a22cf|

:Arguments: * **a** : auto

            * **val** : auto

.. _function-algorithm_binary_search_auto_int_int_auto:

.. das:function:: binary_search(a: auto; f: int; last: int; val: auto) : auto

|detail/function-algorithm-binary_search-0x1cebf128180ac16e|

:Arguments: * **a** : auto

            * **f** : int

            * **last** : int

            * **val** : auto

.. _function-algorithm_binary_search_auto_autoTT_block_ls_a_c_TT;b_c_TT_c_bool_gr_:

.. das:function:: binary_search(a: auto; val: auto(TT); less: block<(a:TT;b:TT):bool>) : auto

|detail/function-algorithm-binary_search-0x9cc7c2707d845b4a|

:Arguments: * **a** : auto

            * **val** : auto(TT)

            * **less** : block<(a:TT;b:TT):bool>

.. _function-algorithm_binary_search_auto_int_int_autoTT_block_ls_a_c_TT;b_c_TT_c_bool_gr_:

.. das:function:: binary_search(a: auto; f: int; last: int; val: auto(TT); less: block<(a:TT;b:TT):bool>) : auto

|detail/function-algorithm-binary_search-0x1de8f19e6c44a56d|

:Arguments: * **a** : auto

            * **f** : int

            * **last** : int

            * **val** : auto(TT)

            * **less** : block<(a:TT;b:TT):bool>

++++++++++++++++++
Array manipulation
++++++++++++++++++

  *  :ref:`unique (var a: array\<auto(TT)\>) : auto <function-algorithm_unique_array_ls_autoTT_gr_>` 
  *  :ref:`sort_unique (var a: array\<auto(TT)\>) : auto <function-algorithm_sort_unique_array_ls_autoTT_gr_>` 
  *  :ref:`reverse (var a: array\<auto\>) : auto <function-algorithm_reverse_array_ls_auto_gr_>` 
  *  :ref:`combine (a: array\<auto(TT)\>; b: array\<auto(TT)\>) : auto <function-algorithm_combine_array_ls_autoTT_gr__array_ls_autoTT_gr_>` 
  *  :ref:`reverse (var a: auto) : auto <function-algorithm_reverse_auto>` 
  *  :ref:`combine (a: auto; b: auto) : auto <function-algorithm_combine_auto_auto>` 
  *  :ref:`erase_all (var arr: auto; value: auto) : auto <function-algorithm_erase_all_auto_auto>` 
  *  :ref:`topological_sort (nodes: array\<auto(Node)\>) : auto <function-algorithm_topological_sort_array_ls_autoNode_gr_>` 

.. _function-algorithm_unique_array_ls_autoTT_gr_:

.. das:function:: unique(a: array<auto(TT)>) : auto

Returns array of the elements of a with duplicates removed.

:Arguments: * **a** : array<auto(TT)>

.. _function-algorithm_sort_unique_array_ls_autoTT_gr_:

.. das:function:: sort_unique(a: array<auto(TT)>) : auto

Returns array of the elements of a, sorted and with duplicates removed.
The elements of a are sorted in ascending order.
The resulted array has only unqiue elements.

:Arguments: * **a** : array<auto(TT)>

.. _function-algorithm_reverse_array_ls_auto_gr_:

.. das:function:: reverse(a: array<auto>) : auto

Returns array of the elements of a in reverse order.

:Arguments: * **a** : array<auto>

.. _function-algorithm_combine_array_ls_autoTT_gr__array_ls_autoTT_gr_:

.. das:function:: combine(a: array<auto(TT)>; b: array<auto(TT)>) : auto

Returns array of the elements of a and then b.

:Arguments: * **a** : array<auto(TT)>

            * **b** : array<auto(TT)>

.. _function-algorithm_reverse_auto:

.. das:function:: reverse(a: auto) : auto

|detail/function-algorithm-reverse-0xc81754f6565380cd|

:Arguments: * **a** : auto

.. _function-algorithm_combine_auto_auto:

.. das:function:: combine(a: auto; b: auto) : auto

|detail/function-algorithm-combine-0x9635b3d64daa7cb3|

:Arguments: * **a** : auto

            * **b** : auto

.. _function-algorithm_erase_all_auto_auto:

.. das:function:: erase_all(arr: auto; value: auto) : auto

Erase all elements equal to value from arr

:Arguments: * **arr** : auto

            * **value** : auto

.. _function-algorithm_topological_sort_array_ls_autoNode_gr_:

.. das:function:: topological_sort(nodes: array<auto(Node)>) : auto

Topological sort of a graph.
Each node has an id, and set (table with no values) of dependencies.
Dependency `before` represents a link from a node, which should appear in the sorted list before the node.
Returns a sorted list of nodes.

:Arguments: * **nodes** : array<auto(Node)>

++++++++++++++++++
Table manipulation
++++++++++++++++++

  *  :ref:`intersection (a: table\<auto(TT), void\>; b: table\<auto(TT), void\>) : table\<TT, void\> <function-algorithm_intersection_table_ls_autoTT,_void_gr__table_ls_autoTT,_void_gr_>` 
  *  :ref:`union (a: table\<auto(TT), void\>; b: table\<auto(TT), void\>) : table\<TT, void\> <function-algorithm_union_table_ls_autoTT,_void_gr__table_ls_autoTT,_void_gr_>` 
  *  :ref:`difference (a: table\<auto(TT), void\>; b: table\<auto(TT), void\>) : table\<TT, void\> <function-algorithm_difference_table_ls_autoTT,_void_gr__table_ls_autoTT,_void_gr_>` 
  *  :ref:`identical (a: table\<auto(TT), void\>; b: table\<auto(TT), void\>) : bool <function-algorithm_identical_table_ls_autoTT,_void_gr__table_ls_autoTT,_void_gr_>` 

.. _function-algorithm_intersection_table_ls_autoTT,_void_gr__table_ls_autoTT,_void_gr_:

.. das:function:: intersection(a: table<auto(TT), void>; b: table<auto(TT), void>) : table<TT, void>

Returns the intersection of two sets

:Arguments: * **a** : table<auto(TT);void>

            * **b** : table<auto(TT);void>

.. _function-algorithm_union_table_ls_autoTT,_void_gr__table_ls_autoTT,_void_gr_:

.. das:function:: union(a: table<auto(TT), void>; b: table<auto(TT), void>) : table<TT, void>

Returns the union of two sets

:Arguments: * **a** : table<auto(TT);void>

            * **b** : table<auto(TT);void>

.. _function-algorithm_difference_table_ls_autoTT,_void_gr__table_ls_autoTT,_void_gr_:

.. das:function:: difference(a: table<auto(TT), void>; b: table<auto(TT), void>) : table<TT, void>

Returns the difference of two sets

:Arguments: * **a** : table<auto(TT);void>

            * **b** : table<auto(TT);void>

.. _function-algorithm_identical_table_ls_autoTT,_void_gr__table_ls_autoTT,_void_gr_:

.. das:function:: identical(a: table<auto(TT), void>; b: table<auto(TT), void>) : bool

Returns true if the two sets are identical

:Arguments: * **a** : table<auto(TT);void>

            * **b** : table<auto(TT);void>


