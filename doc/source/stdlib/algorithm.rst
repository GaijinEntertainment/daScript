
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

  *  :ref:`lower_bound (a: array\<auto(TT)\>; f: int; l: int; val: TT) : auto <function-_at_algorithm_c__c_lower_bound_C1_ls_Y_ls_TT_gr__dot__gr_A_Ci_Ci_CY_ls_TT_gr_L>` 
  *  :ref:`lower_bound (a: array\<auto(TT)\>; val: TT) : auto <function-_at_algorithm_c__c_lower_bound_C1_ls_Y_ls_TT_gr__dot__gr_A_CY_ls_TT_gr_L>` 
  *  :ref:`lower_bound (a: array\<auto(TT)\>; f: int; l: int; value: TT; less: block\<(a:TT const;b:TT const):bool\>) : auto <function-_at_algorithm_c__c_lower_bound_C1_ls_Y_ls_TT_gr__dot__gr_A_Ci_Ci_CY_ls_TT_gr_L_CN_ls_a;b_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`lower_bound (a: array\<auto(TT)\>; value: TT; less: block\<(a:TT const;b:TT const):bool\>) : auto <function-_at_algorithm_c__c_lower_bound_C1_ls_Y_ls_TT_gr__dot__gr_A_CY_ls_TT_gr_L_CN_ls_a;b_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`binary_search (a: array\<auto(TT)\>; val: TT) : auto <function-_at_algorithm_c__c_binary_search_C1_ls_Y_ls_TT_gr__dot__gr_A_CY_ls_TT_gr_L>` 
  *  :ref:`binary_search (a: array\<auto(TT)\>; f: int; last: int; val: TT) : auto <function-_at_algorithm_c__c_binary_search_C1_ls_Y_ls_TT_gr__dot__gr_A_Ci_Ci_CY_ls_TT_gr_L>` 
  *  :ref:`binary_search (a: array\<auto(TT)\>; val: TT; less: block\<(a:TT const;b:TT const):bool\>) : auto <function-_at_algorithm_c__c_binary_search_C1_ls_Y_ls_TT_gr__dot__gr_A_CY_ls_TT_gr_L_CN_ls_a;b_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`binary_search (a: array\<auto(TT)\>; f: int; last: int; val: TT; less: block\<(a:TT const;b:TT const):bool\>) : auto <function-_at_algorithm_c__c_binary_search_C1_ls_Y_ls_TT_gr__dot__gr_A_Ci_Ci_CY_ls_TT_gr_L_CN_ls_a;b_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`lower_bound (a: auto; f: int; l: int; val: auto) : auto <function-_at_algorithm_c__c_lower_bound_C_dot__Ci_Ci_C_dot__%_ls_IsAnyArrayMacro_c_expect_any_array(a_eq_true)_gr_>` 
  *  :ref:`lower_bound (a: auto; val: auto) : auto <function-_at_algorithm_c__c_lower_bound_C_dot__C_dot__%_ls_IsAnyArrayMacro_c_expect_any_array(a_eq_true)_gr_>` 
  *  :ref:`lower_bound (a: auto; f: int; l: int; val: auto(TT); less: block\<(a:TT const;b:TT const):bool\>) : auto <function-_at_algorithm_c__c_lower_bound_C_dot__Ci_Ci_CY_ls_TT_gr__dot__CN_ls_a;b_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin__%_ls_IsAnyArrayMacro_c_expect_any_array(a_eq_true)_gr_>` 
  *  :ref:`lower_bound (a: auto; val: auto(TT); less: block\<(a:TT const;b:TT const):bool\>) : auto <function-_at_algorithm_c__c_lower_bound_C_dot__CY_ls_TT_gr__dot__CN_ls_a;b_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin__%_ls_IsAnyArrayMacro_c_expect_any_array(a_eq_true)_gr_>` 
  *  :ref:`binary_search (a: auto; val: auto) : auto <function-_at_algorithm_c__c_binary_search_C_dot__C_dot__%_ls_IsAnyArrayMacro_c_expect_any_array(a_eq_true)_gr_>` 
  *  :ref:`binary_search (a: auto; f: int; last: int; val: auto) : auto <function-_at_algorithm_c__c_binary_search_C_dot__Ci_Ci_C_dot__%_ls_IsAnyArrayMacro_c_expect_any_array(a_eq_true)_gr_>` 
  *  :ref:`binary_search (a: auto; val: auto(TT); less: block\<(a:TT const;b:TT const):bool\>) : auto <function-_at_algorithm_c__c_binary_search_C_dot__CY_ls_TT_gr__dot__CN_ls_a;b_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin__%_ls_IsAnyArrayMacro_c_expect_any_array(a_eq_true)_gr_>` 
  *  :ref:`binary_search (a: auto; f: int; last: int; val: auto(TT); less: block\<(a:TT const;b:TT const):bool\>) : auto <function-_at_algorithm_c__c_binary_search_C_dot__Ci_Ci_CY_ls_TT_gr__dot__CN_ls_a;b_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin__%_ls_IsAnyArrayMacro_c_expect_any_array(a_eq_true)_gr_>` 

.. _function-_at_algorithm_c__c_lower_bound_C1_ls_Y_ls_TT_gr__dot__gr_A_Ci_Ci_CY_ls_TT_gr_L:

.. das:function:: lower_bound(a: array<auto(TT)>; f: int; l: int; val: TT) : auto

Returns an iterator pointing to the first element in the range [first, last) that is not less than (i.e. greater or equal to) value, or last if no such element is found.


:Arguments: * **a** : array<auto(TT)>

            * **f** : int

            * **l** : int

            * **val** : TT

.. _function-_at_algorithm_c__c_lower_bound_C1_ls_Y_ls_TT_gr__dot__gr_A_CY_ls_TT_gr_L:

.. das:function:: lower_bound(a: array<auto(TT)>; val: TT) : auto

|detail/function-algorithm-lower_bound-0xb469fc2f44d14195|

:Arguments: * **a** : array<auto(TT)>

            * **val** : TT

.. _function-_at_algorithm_c__c_lower_bound_C1_ls_Y_ls_TT_gr__dot__gr_A_Ci_Ci_CY_ls_TT_gr_L_CN_ls_a;b_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: lower_bound(a: array<auto(TT)>; f: int; l: int; value: TT; less: block<(a:TT const;b:TT const):bool>) : auto

|detail/function-algorithm-lower_bound-0xb409d83f5b261b19|

:Arguments: * **a** : array<auto(TT)>

            * **f** : int

            * **l** : int

            * **value** : TT

            * **less** : block<(a:TT;b:TT):bool>

.. _function-_at_algorithm_c__c_lower_bound_C1_ls_Y_ls_TT_gr__dot__gr_A_CY_ls_TT_gr_L_CN_ls_a;b_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: lower_bound(a: array<auto(TT)>; value: TT; less: block<(a:TT const;b:TT const):bool>) : auto

|detail/function-algorithm-lower_bound-0x2b8e7291f67a8e78|

:Arguments: * **a** : array<auto(TT)>

            * **value** : TT

            * **less** : block<(a:TT;b:TT):bool>

.. _function-_at_algorithm_c__c_binary_search_C1_ls_Y_ls_TT_gr__dot__gr_A_CY_ls_TT_gr_L:

.. das:function:: binary_search(a: array<auto(TT)>; val: TT) : auto

|detail/function-algorithm-binary_search-0xad4ffd6c53c3193|

:Arguments: * **a** : array<auto(TT)>

            * **val** : TT

.. _function-_at_algorithm_c__c_binary_search_C1_ls_Y_ls_TT_gr__dot__gr_A_Ci_Ci_CY_ls_TT_gr_L:

.. das:function:: binary_search(a: array<auto(TT)>; f: int; last: int; val: TT) : auto

Returns true if an val appears within the range [f, last).
Array a must be sorted.


:Arguments: * **a** : array<auto(TT)>

            * **f** : int

            * **last** : int

            * **val** : TT

.. _function-_at_algorithm_c__c_binary_search_C1_ls_Y_ls_TT_gr__dot__gr_A_CY_ls_TT_gr_L_CN_ls_a;b_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: binary_search(a: array<auto(TT)>; val: TT; less: block<(a:TT const;b:TT const):bool>) : auto

|detail/function-algorithm-binary_search-0xcc2e024c8b0f03de|

:Arguments: * **a** : array<auto(TT)>

            * **val** : TT

            * **less** : block<(a:TT;b:TT):bool>

.. _function-_at_algorithm_c__c_binary_search_C1_ls_Y_ls_TT_gr__dot__gr_A_Ci_Ci_CY_ls_TT_gr_L_CN_ls_a;b_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: binary_search(a: array<auto(TT)>; f: int; last: int; val: TT; less: block<(a:TT const;b:TT const):bool>) : auto

|detail/function-algorithm-binary_search-0xe20f86b7b968e14f|

:Arguments: * **a** : array<auto(TT)>

            * **f** : int

            * **last** : int

            * **val** : TT

            * **less** : block<(a:TT;b:TT):bool>

.. _function-_at_algorithm_c__c_lower_bound_C_dot__Ci_Ci_C_dot__%_ls_IsAnyArrayMacro_c_expect_any_array(a_eq_true)_gr_:

.. das:function:: lower_bound(a: auto; f: int; l: int; val: auto) : auto

|detail/function-algorithm-lower_bound-0x9e462d5940438790|

:Arguments: * **a** : auto

            * **f** : int

            * **l** : int

            * **val** : auto

.. _function-_at_algorithm_c__c_lower_bound_C_dot__C_dot__%_ls_IsAnyArrayMacro_c_expect_any_array(a_eq_true)_gr_:

.. das:function:: lower_bound(a: auto; val: auto) : auto

|detail/function-algorithm-lower_bound-0x8cf240a3a8ad3c21|

:Arguments: * **a** : auto

            * **val** : auto

.. _function-_at_algorithm_c__c_lower_bound_C_dot__Ci_Ci_CY_ls_TT_gr__dot__CN_ls_a;b_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin__%_ls_IsAnyArrayMacro_c_expect_any_array(a_eq_true)_gr_:

.. das:function:: lower_bound(a: auto; f: int; l: int; val: auto(TT); less: block<(a:TT const;b:TT const):bool>) : auto

|detail/function-algorithm-lower_bound-0x439eca97a0d0ca4e|

:Arguments: * **a** : auto

            * **f** : int

            * **l** : int

            * **val** : auto(TT)

            * **less** : block<(a:TT;b:TT):bool>

.. _function-_at_algorithm_c__c_lower_bound_C_dot__CY_ls_TT_gr__dot__CN_ls_a;b_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin__%_ls_IsAnyArrayMacro_c_expect_any_array(a_eq_true)_gr_:

.. das:function:: lower_bound(a: auto; val: auto(TT); less: block<(a:TT const;b:TT const):bool>) : auto

|detail/function-algorithm-lower_bound-0x5377c1a90bf7611d|

:Arguments: * **a** : auto

            * **val** : auto(TT)

            * **less** : block<(a:TT;b:TT):bool>

.. _function-_at_algorithm_c__c_binary_search_C_dot__C_dot__%_ls_IsAnyArrayMacro_c_expect_any_array(a_eq_true)_gr_:

.. das:function:: binary_search(a: auto; val: auto) : auto

|detail/function-algorithm-binary_search-0x6140c5dea5a22cf|

:Arguments: * **a** : auto

            * **val** : auto

.. _function-_at_algorithm_c__c_binary_search_C_dot__Ci_Ci_C_dot__%_ls_IsAnyArrayMacro_c_expect_any_array(a_eq_true)_gr_:

.. das:function:: binary_search(a: auto; f: int; last: int; val: auto) : auto

|detail/function-algorithm-binary_search-0x1cebf128180ac16e|

:Arguments: * **a** : auto

            * **f** : int

            * **last** : int

            * **val** : auto

.. _function-_at_algorithm_c__c_binary_search_C_dot__CY_ls_TT_gr__dot__CN_ls_a;b_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin__%_ls_IsAnyArrayMacro_c_expect_any_array(a_eq_true)_gr_:

.. das:function:: binary_search(a: auto; val: auto(TT); less: block<(a:TT const;b:TT const):bool>) : auto

|detail/function-algorithm-binary_search-0xb0c16aa4940f2cb|

:Arguments: * **a** : auto

            * **val** : auto(TT)

            * **less** : block<(a:TT;b:TT):bool>

.. _function-_at_algorithm_c__c_binary_search_C_dot__Ci_Ci_CY_ls_TT_gr__dot__CN_ls_a;b_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin__%_ls_IsAnyArrayMacro_c_expect_any_array(a_eq_true)_gr_:

.. das:function:: binary_search(a: auto; f: int; last: int; val: auto(TT); less: block<(a:TT const;b:TT const):bool>) : auto

|detail/function-algorithm-binary_search-0x488d48e78963f2b4|

:Arguments: * **a** : auto

            * **f** : int

            * **last** : int

            * **val** : auto(TT)

            * **less** : block<(a:TT;b:TT):bool>

++++++++++++++++++
Array manipulation
++++++++++++++++++

  *  :ref:`unique (var a: array\<auto(TT)\>) : auto <function-_at_algorithm_c__c_unique_1_ls_Y_ls_TT_gr__dot__gr_A>` 
  *  :ref:`sort_unique (var a: array\<auto(TT)\>) : auto <function-_at_algorithm_c__c_sort_unique_1_ls_Y_ls_TT_gr__dot__gr_A>` 
  *  :ref:`reverse (var a: array\<auto\>) : auto <function-_at_algorithm_c__c_reverse_1_ls__dot__gr_A>` 
  *  :ref:`combine (a: array\<auto(TT)\>; b: array\<auto(TT)\>) : auto <function-_at_algorithm_c__c_combine_C1_ls_Y_ls_TT_gr__dot__gr_A_C1_ls_Y_ls_TT_gr__dot__gr_A>` 
  *  :ref:`reverse (var a: auto) : auto <function-_at_algorithm_c__c_reverse__dot__%_ls_IsAnyArrayMacro_c_expect_any_array(a_eq_true)_gr_>` 
  *  :ref:`combine (a: auto; b: auto) : auto <function-_at_algorithm_c__c_combine_C_dot__C_dot__%_ls_IsAnyArrayMacro_c_expect_any_array(a_eq_true)_gr_>` 

.. _function-_at_algorithm_c__c_unique_1_ls_Y_ls_TT_gr__dot__gr_A:

.. das:function:: unique(a: array<auto(TT)>) : auto

Returns array of the elements of a with duplicates removed.


:Arguments: * **a** : array<auto(TT)>

.. _function-_at_algorithm_c__c_sort_unique_1_ls_Y_ls_TT_gr__dot__gr_A:

.. das:function:: sort_unique(a: array<auto(TT)>) : auto

Returns array of the elements of a, sorted and with duplicates removed.
The elements of a are sorted in ascending order.
The resulted array has only unqiue elements.


:Arguments: * **a** : array<auto(TT)>

.. _function-_at_algorithm_c__c_reverse_1_ls__dot__gr_A:

.. das:function:: reverse(a: array<auto>) : auto

Returns array of the elements of a in reverse order.


:Arguments: * **a** : array<auto>

.. _function-_at_algorithm_c__c_combine_C1_ls_Y_ls_TT_gr__dot__gr_A_C1_ls_Y_ls_TT_gr__dot__gr_A:

.. das:function:: combine(a: array<auto(TT)>; b: array<auto(TT)>) : auto

Returns array of the elements of a and then b.


:Arguments: * **a** : array<auto(TT)>

            * **b** : array<auto(TT)>

.. _function-_at_algorithm_c__c_reverse__dot__%_ls_IsAnyArrayMacro_c_expect_any_array(a_eq_true)_gr_:

.. das:function:: reverse(a: auto) : auto

|detail/function-algorithm-reverse-0x6de34e5691612c6c|

:Arguments: * **a** : auto

.. _function-_at_algorithm_c__c_combine_C_dot__C_dot__%_ls_IsAnyArrayMacro_c_expect_any_array(a_eq_true)_gr_:

.. das:function:: combine(a: auto; b: auto) : auto

|detail/function-algorithm-combine-0x9635b3d64daa7cb3|

:Arguments: * **a** : auto

            * **b** : auto

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_algorithm_c__c_erase_all__dot__C_dot__%_ls_IsAnyArrayMacro_c_expect_any_array(arr_eq_true)_gr_:

.. das:function:: erase_all(arr: auto; value: auto) : auto

Erase all elements equal to value from arr


:Arguments: * **arr** : auto

            * **value** : auto

.. _function-_at_algorithm_c__c_topological_sort_C1_ls_Y_ls_Node_gr__dot__gr_A:

.. das:function:: topological_sort(nodes: array<auto(Node)>) : auto

Topological sort of a graph.
Each node has an id, and set (table with no values) of dependencies.
Dependency `before` represents a link from a node, which should appear in the sorted list before the node.
Returns a sorted list of nodes.


:Arguments: * **nodes** : array<auto(Node)>

.. _function-_at_algorithm_c__c_intersection_C1_ls_Y_ls_TT_gr__dot__gr_2_ls_v_gr_T_C1_ls_Y_ls_TT_gr__dot__gr_2_ls_v_gr_T:

.. das:function:: intersection(a: table<auto(TT);void>; b: table<auto(TT);void>) : table<TT;void>

Returns the intersection of two sets


:Arguments: * **a** : table<auto(TT);void>

            * **b** : table<auto(TT);void>

.. _function-_at_algorithm_c__c_union_C1_ls_Y_ls_TT_gr__dot__gr_2_ls_v_gr_T_C1_ls_Y_ls_TT_gr__dot__gr_2_ls_v_gr_T:

.. das:function:: union(a: table<auto(TT);void>; b: table<auto(TT);void>) : table<TT;void>

Returns the union of two sets


:Arguments: * **a** : table<auto(TT);void>

            * **b** : table<auto(TT);void>

.. _function-_at_algorithm_c__c_difference_C1_ls_Y_ls_TT_gr__dot__gr_2_ls_v_gr_T_C1_ls_Y_ls_TT_gr__dot__gr_2_ls_v_gr_T:

.. das:function:: difference(a: table<auto(TT);void>; b: table<auto(TT);void>) : table<TT;void>

Returns the difference of two sets


:Arguments: * **a** : table<auto(TT);void>

            * **b** : table<auto(TT);void>

.. _function-_at_algorithm_c__c_identical_C1_ls_Y_ls_TT_gr__dot__gr_2_ls_v_gr_T_C1_ls_Y_ls_TT_gr__dot__gr_2_ls_v_gr_T:

.. das:function:: identical(a: table<auto(TT);void>; b: table<auto(TT);void>) : bool

Returns true if the two sets are identical


:Arguments: * **a** : table<auto(TT);void>

            * **b** : table<auto(TT);void>


