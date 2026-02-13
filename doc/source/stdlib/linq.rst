
.. _stdlib_linq:

====
LINQ
====

The LINQ module provides query-style operations on sequences: filtering
(``where_``), projection (``select``), sorting (``order``, ``order_by``),
deduplication (``distinct``), pagination (``skip``, ``take``), aggregation
(``sum``, ``average``, ``aggregate``), and element access (``first``, ``last``).

See also :doc:`linq_boost` for pipe-syntax macros with underscore shorthand.

All functions and symbols are in "linq" module, use require to get access to it. ::

    require daslib/linq

Example: ::

    require daslib/linq

        [export]
        def main() {
            var src <- [iterator for (x in range(10)); x]
            var evens <- where_(src, $(x : int) : bool { return x % 2 == 0; })
            for (v in evens) {
                print("{v} ")
            }
            print("\n")
        }
        // output:
        // 0 2 4 6 8

++++++++++++
Sorting data
++++++++++++

  *  :ref:`order (var a: iterator\<auto(TT)\>) : iterator\<TT\> <function-linq_order_iterator_ls_autoTT_gr_>` 
  *  :ref:`order (var a: iterator\<auto(TT)\>; fun: block\<(v1:TT;v2:TT):bool\>) : iterator\<TT\> <function-linq_order_iterator_ls_autoTT_gr__block_ls_v1_c_TT;v2_c_TT_c_bool_gr_>` 
  *  :ref:`order (a: array\<auto(TT)\>; fun: block\<(v1:TT;v2:TT):bool\>) : array\<TT\> <function-linq_order_array_ls_autoTT_gr__block_ls_v1_c_TT;v2_c_TT_c_bool_gr_>` 
  *  :ref:`order (arr: array\<auto(TT)\>) : array\<TT\> <function-linq_order_array_ls_autoTT_gr_>` 
  *  :ref:`order_by (var a: iterator\<auto(TT)\>; key: auto) : iterator\<TT\> <function-linq_order_by_iterator_ls_autoTT_gr__auto_0x1a9>` 
  *  :ref:`order_by (a: array\<auto(TT)\>; key: auto) : array\<TT\> <function-linq_order_by_array_ls_autoTT_gr__auto_0x1b1>` 
  *  :ref:`order_by_descending (a: array\<auto(TT)\>; key: auto) : array\<TT\> <function-linq_order_by_descending_array_ls_autoTT_gr__auto_0x1cf>` 
  *  :ref:`order_by_descending (var a: iterator\<auto(TT)\>; key: auto) : iterator\<TT\> <function-linq_order_by_descending_iterator_ls_autoTT_gr__auto_0x1c7>` 
  *  :ref:`order_by_descending_inplace (var buffer: array\<auto(TT)\>; key: auto) : auto <function-linq_order_by_descending_inplace_array_ls_autoTT_gr__auto_0x1c1>` 
  *  :ref:`order_by_descending_to_array (var a: iterator\<auto(TT)\>; key: auto) : array\<TT\> <function-linq_order_by_descending_to_array_iterator_ls_autoTT_gr__auto_0x1d7>` 
  *  :ref:`order_by_inplace (var buffer: array\<auto(TT)\>; key: auto) : auto <function-linq_order_by_inplace_array_ls_autoTT_gr__auto_0x1a3>` 
  *  :ref:`order_by_to_array (var a: iterator\<auto(TT)\>; key: auto) : array\<TT\> <function-linq_order_by_to_array_iterator_ls_autoTT_gr__auto_0x1b9>` 
  *  :ref:`order_descending (var a: iterator\<auto(TT)\>; fun: block\<(v1:TT;v2:TT):bool\>) : iterator\<TT\> <function-linq_order_descending_iterator_ls_autoTT_gr__block_ls_v1_c_TT;v2_c_TT_c_bool_gr_>` 
  *  :ref:`order_descending (a: array\<auto(TT)\>; fun: block\<(v1:TT;v2:TT):bool\>) : array\<TT\> <function-linq_order_descending_array_ls_autoTT_gr__block_ls_v1_c_TT;v2_c_TT_c_bool_gr_>` 
  *  :ref:`order_descending_inplace (var buffer: array\<auto(TT)\>; fun: block\<(v1:TT;v2:TT):bool\>) : auto <function-linq_order_descending_inplace_array_ls_autoTT_gr__block_ls_v1_c_TT;v2_c_TT_c_bool_gr_>` 
  *  :ref:`order_descending_to_array (var a: iterator\<auto(TT)\>; fun: block\<(v1:TT;v2:TT):bool\>) : array\<TT\> <function-linq_order_descending_to_array_iterator_ls_autoTT_gr__block_ls_v1_c_TT;v2_c_TT_c_bool_gr_>` 
  *  :ref:`order_inplace (var buffer: array\<auto(TT)\>) : auto <function-linq_order_inplace_array_ls_autoTT_gr_>` 
  *  :ref:`order_inplace (var buffer: array\<auto(TT)\>; fun: block\<(v1:TT;v2:TT):bool\>) : auto <function-linq_order_inplace_array_ls_autoTT_gr__block_ls_v1_c_TT;v2_c_TT_c_bool_gr_>` 
  *  :ref:`order_to_array (var a: iterator\<auto(TT)\>; fun: block\<(v1:TT;v2:TT):bool\>) : array\<TT\> <function-linq_order_to_array_iterator_ls_autoTT_gr__block_ls_v1_c_TT;v2_c_TT_c_bool_gr_>` 
  *  :ref:`order_to_array (var a: iterator\<auto(TT)\>) : array\<TT\> <function-linq_order_to_array_iterator_ls_autoTT_gr_>` 
  *  :ref:`order_unique_folded (var a: array\<auto(TT)\>) : array\<TT\> <function-linq_order_unique_folded_array_ls_autoTT_gr_>` 
  *  :ref:`order_unique_folded (var a: iterator\<auto(TT)\>) : array\<TT\> <function-linq_order_unique_folded_iterator_ls_autoTT_gr_>` 
  *  :ref:`order_unique_folded_inplace (var a: array\<auto(TT)\>) : auto <function-linq_order_unique_folded_inplace_array_ls_autoTT_gr_>` 
  *  :ref:`reverse (a: array\<auto(TT)\>) : array\<TT\> <function-linq_reverse_array_ls_autoTT_gr_>` 
  *  :ref:`reverse (var a: iterator\<auto(TT)\>) : iterator\<TT\> <function-linq_reverse_iterator_ls_autoTT_gr_>` 
  *  :ref:`reverse_inplace (var buffer: array\<auto(TT)\>) : auto <function-linq_reverse_inplace_array_ls_autoTT_gr_>` 
  *  :ref:`reverse_to_array (var a: iterator\<auto(TT)\>) : array\<TT\> <function-linq_reverse_to_array_iterator_ls_autoTT_gr_>` 


order
^^^^^

.. _function-linq_order_iterator_ls_autoTT_gr_:

.. das:function:: order(a: iterator<auto(TT)>) : iterator<TT>

Sorts an iterator

:Arguments: * **a** : iterator<auto(TT)>

.. _function-linq_order_iterator_ls_autoTT_gr__block_ls_v1_c_TT;v2_c_TT_c_bool_gr_:

.. das:function:: order(a: iterator<auto(TT)>; fun: block<(v1:TT;v2:TT):bool>) : iterator<TT>

.. _function-linq_order_array_ls_autoTT_gr__block_ls_v1_c_TT;v2_c_TT_c_bool_gr_:

.. das:function:: order(a: array<auto(TT)>; fun: block<(v1:TT;v2:TT):bool>) : array<TT>

.. _function-linq_order_array_ls_autoTT_gr_:

.. das:function:: order(arr: array<auto(TT)>) : array<TT>

----


order_by
^^^^^^^^

.. _function-linq_order_by_iterator_ls_autoTT_gr__auto_0x1a9:

.. das:function:: order_by(a: iterator<auto(TT)>; key: auto) : iterator<TT>

Sorts an iterator

:Arguments: * **a** : iterator<auto(TT)>

            * **key** : auto

.. _function-linq_order_by_array_ls_autoTT_gr__auto_0x1b1:

.. das:function:: order_by(a: array<auto(TT)>; key: auto) : array<TT>

----


order_by_descending
^^^^^^^^^^^^^^^^^^^

.. _function-linq_order_by_descending_array_ls_autoTT_gr__auto_0x1cf:

.. das:function:: order_by_descending(a: array<auto(TT)>; key: auto) : array<TT>

Sorts an array in descending order

:Arguments: * **a** : array<auto(TT)>

            * **key** : auto

.. _function-linq_order_by_descending_iterator_ls_autoTT_gr__auto_0x1c7:

.. das:function:: order_by_descending(a: iterator<auto(TT)>; key: auto) : iterator<TT>

----

.. _function-linq_order_by_descending_inplace_array_ls_autoTT_gr__auto_0x1c1:

.. das:function:: order_by_descending_inplace(buffer: array<auto(TT)>; key: auto) : auto

Sorts an array in descending order in place

:Arguments: * **buffer** : array<auto(TT)>

            * **key** : auto

.. _function-linq_order_by_descending_to_array_iterator_ls_autoTT_gr__auto_0x1d7:

.. das:function:: order_by_descending_to_array(a: iterator<auto(TT)>; key: auto) : array<TT>

Sorts an iterator in descending order and returns an array

:Arguments: * **a** : iterator<auto(TT)>

            * **key** : auto

.. _function-linq_order_by_inplace_array_ls_autoTT_gr__auto_0x1a3:

.. das:function:: order_by_inplace(buffer: array<auto(TT)>; key: auto) : auto

Sorts an array in place

:Arguments: * **buffer** : array<auto(TT)>

            * **key** : auto

.. _function-linq_order_by_to_array_iterator_ls_autoTT_gr__auto_0x1b9:

.. das:function:: order_by_to_array(a: iterator<auto(TT)>; key: auto) : array<TT>

Sorts an iterator and returns an array

:Arguments: * **a** : iterator<auto(TT)>

            * **key** : auto


order_descending
^^^^^^^^^^^^^^^^

.. _function-linq_order_descending_iterator_ls_autoTT_gr__block_ls_v1_c_TT;v2_c_TT_c_bool_gr_:

.. das:function:: order_descending(a: iterator<auto(TT)>; fun: block<(v1:TT;v2:TT):bool>) : iterator<TT>

Sorts an iterator in descending order

:Arguments: * **a** : iterator<auto(TT)>

            * **fun** : block<(v1:TT;v2:TT):bool>

.. _function-linq_order_descending_array_ls_autoTT_gr__block_ls_v1_c_TT;v2_c_TT_c_bool_gr_:

.. das:function:: order_descending(a: array<auto(TT)>; fun: block<(v1:TT;v2:TT):bool>) : array<TT>

----

.. _function-linq_order_descending_inplace_array_ls_autoTT_gr__block_ls_v1_c_TT;v2_c_TT_c_bool_gr_:

.. das:function:: order_descending_inplace(buffer: array<auto(TT)>; fun: block<(v1:TT;v2:TT):bool>) : auto

Sorts an array in descending order in place

:Arguments: * **buffer** : array<auto(TT)>

            * **fun** : block<(v1:TT;v2:TT):bool>

.. _function-linq_order_descending_to_array_iterator_ls_autoTT_gr__block_ls_v1_c_TT;v2_c_TT_c_bool_gr_:

.. das:function:: order_descending_to_array(a: iterator<auto(TT)>; fun: block<(v1:TT;v2:TT):bool>) : array<TT>

Sorts an iterator in descending order and returns an array

:Arguments: * **a** : iterator<auto(TT)>

            * **fun** : block<(v1:TT;v2:TT):bool>


order_inplace
^^^^^^^^^^^^^

.. _function-linq_order_inplace_array_ls_autoTT_gr_:

.. das:function:: order_inplace(buffer: array<auto(TT)>) : auto

Sorts an array in place

:Arguments: * **buffer** : array<auto(TT)>

.. _function-linq_order_inplace_array_ls_autoTT_gr__block_ls_v1_c_TT;v2_c_TT_c_bool_gr_:

.. das:function:: order_inplace(buffer: array<auto(TT)>; fun: block<(v1:TT;v2:TT):bool>) : auto

----


order_to_array
^^^^^^^^^^^^^^

.. _function-linq_order_to_array_iterator_ls_autoTT_gr__block_ls_v1_c_TT;v2_c_TT_c_bool_gr_:

.. das:function:: order_to_array(a: iterator<auto(TT)>; fun: block<(v1:TT;v2:TT):bool>) : array<TT>

Sorts an iterator and returns an array

:Arguments: * **a** : iterator<auto(TT)>

            * **fun** : block<(v1:TT;v2:TT):bool>

.. _function-linq_order_to_array_iterator_ls_autoTT_gr_:

.. das:function:: order_to_array(a: iterator<auto(TT)>) : array<TT>

----


order_unique_folded
^^^^^^^^^^^^^^^^^^^

.. _function-linq_order_unique_folded_array_ls_autoTT_gr_:

.. das:function:: order_unique_folded(a: array<auto(TT)>) : array<TT>

sort and remove duplicate elements from an array

:Arguments: * **a** : array<auto(TT)>

.. _function-linq_order_unique_folded_iterator_ls_autoTT_gr_:

.. das:function:: order_unique_folded(a: iterator<auto(TT)>) : array<TT>

----

.. _function-linq_order_unique_folded_inplace_array_ls_autoTT_gr_:

.. das:function:: order_unique_folded_inplace(a: array<auto(TT)>) : auto

sort and remove duplicate elements from an array

:Arguments: * **a** : array<auto(TT)>


reverse
^^^^^^^

.. _function-linq_reverse_array_ls_autoTT_gr_:

.. das:function:: reverse(a: array<auto(TT)>) : array<TT>

Reverses an array

:Arguments: * **a** : array<auto(TT)>

.. _function-linq_reverse_iterator_ls_autoTT_gr_:

.. das:function:: reverse(a: iterator<auto(TT)>) : iterator<TT>

----

.. _function-linq_reverse_inplace_array_ls_autoTT_gr_:

.. das:function:: reverse_inplace(buffer: array<auto(TT)>) : auto

Reverses an array in place

:Arguments: * **buffer** : array<auto(TT)>

.. _function-linq_reverse_to_array_iterator_ls_autoTT_gr_:

.. das:function:: reverse_to_array(a: iterator<auto(TT)>) : array<TT>

Reverses an iterator and returns an array

:Arguments: * **a** : iterator<auto(TT)>

++++++++++++++
Set operations
++++++++++++++

  *  :ref:`distinct (a: array\<auto(TT)\>) : array\<TT\> <function-linq_distinct_array_ls_autoTT_gr_>` 
  *  :ref:`distinct (var a: iterator\<auto(TT)\>) : iterator\<TT\> <function-linq_distinct_iterator_ls_autoTT_gr_>` 
  *  :ref:`distinct_by (a: array\<auto(TT)\>; key: block\<(arg:TT):auto\>) : array\<TT\> <function-linq_distinct_by_array_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_>` 
  *  :ref:`distinct_by (var a: iterator\<auto(TT)\>; key: block\<(arg:TT):auto\>) : iterator\<TT\> <function-linq_distinct_by_iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_>` 
  *  :ref:`distinct_by_inplace (var a: array\<auto(TT)\>; key: block\<(arg:TT):auto\>) : auto <function-linq_distinct_by_inplace_array_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_>` 
  *  :ref:`distinct_by_to_array (var a: iterator\<auto(TT)\>; key: block\<(arg:TT):auto\>) : array\<TT\> <function-linq_distinct_by_to_array_iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_>` 
  *  :ref:`distinct_inplace (var a: array\<auto(TT)\>) : auto <function-linq_distinct_inplace_array_ls_autoTT_gr_>` 
  *  :ref:`distinct_to_array (var a: iterator\<auto(TT)\>) : array\<TT\> <function-linq_distinct_to_array_iterator_ls_autoTT_gr_>` 
  *  :ref:`except (var src: iterator\<auto(TT)\>; var exclude: iterator\<auto(TT)\>) : iterator\<TT\> <function-linq_except_iterator_ls_autoTT_gr__iterator_ls_autoTT_gr_>` 
  *  :ref:`except (src: array\<auto(TT)\>; exclude: array\<auto(TT)\>) : array\<TT\> <function-linq_except_array_ls_autoTT_gr__array_ls_autoTT_gr_>` 
  *  :ref:`except_by (src: array\<auto(TT)\>; exclude: array\<auto(TT)\>; key: block\<(arg:TT):auto\>) : array\<TT\> <function-linq_except_by_array_ls_autoTT_gr__array_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_>` 
  *  :ref:`except_by (var src: iterator\<auto(TT)\>; var exclude: iterator\<auto(TT)\>; key: block\<(arg:TT):auto\>) : iterator\<TT\> <function-linq_except_by_iterator_ls_autoTT_gr__iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_>` 
  *  :ref:`except_by_to_array (var src: iterator\<auto(TT)\>; var exclude: iterator\<auto(TT)\>; key: block\<(arg:TT):auto\>) : array\<TT\> <function-linq_except_by_to_array_iterator_ls_autoTT_gr__iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_>` 
  *  :ref:`except_to_array (var src: iterator\<auto(TT)\>; var exclude: iterator\<auto(TT)\>) : array\<TT\> <function-linq_except_to_array_iterator_ls_autoTT_gr__iterator_ls_autoTT_gr_>` 
  *  :ref:`intersect (srca: array\<auto(TT)\>; srcb: array\<auto(TT)\>) : array\<TT\> <function-linq_intersect_array_ls_autoTT_gr__array_ls_autoTT_gr_>` 
  *  :ref:`intersect (var srca: iterator\<auto(TT)\>; var srcb: iterator\<auto(TT)\>) : iterator\<TT\> <function-linq_intersect_iterator_ls_autoTT_gr__iterator_ls_autoTT_gr_>` 
  *  :ref:`intersect_by (srca: array\<auto(TT)\>; srcb: array\<auto(TT)\>; key: block\<(arg:TT):auto\>) : array\<TT\> <function-linq_intersect_by_array_ls_autoTT_gr__array_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_>` 
  *  :ref:`intersect_by (var srca: iterator\<auto(TT)\>; var srcb: iterator\<auto(TT)\>; key: block\<(arg:TT):auto\>) : iterator\<TT\> <function-linq_intersect_by_iterator_ls_autoTT_gr__iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_>` 
  *  :ref:`intersect_by_to_array (var srca: iterator\<auto(TT)\>; var srcb: iterator\<auto(TT)\>; key: block\<(arg:TT):auto\>) : array\<TT\> <function-linq_intersect_by_to_array_iterator_ls_autoTT_gr__iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_>` 
  *  :ref:`intersect_to_array (var srca: iterator\<auto(TT)\>; var srcb: iterator\<auto(TT)\>) : array\<TT\> <function-linq_intersect_to_array_iterator_ls_autoTT_gr__iterator_ls_autoTT_gr_>` 
  *  :ref:`union (var srca: iterator\<auto(TT)\>; var srcb: iterator\<auto(TT)\>) : iterator\<TT\> <function-linq_union_iterator_ls_autoTT_gr__iterator_ls_autoTT_gr_>` 
  *  :ref:`union (var srca: array\<auto(TT)\>; var srcb: array\<auto(TT)\>) : array\<TT\> <function-linq_union_array_ls_autoTT_gr__array_ls_autoTT_gr_>` 
  *  :ref:`union_by (srca: array\<auto(TT)\>; srcb: array\<auto(TT)\>; key: block\<(arg:TT):auto\>) : array\<TT\> <function-linq_union_by_array_ls_autoTT_gr__array_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_>` 
  *  :ref:`union_by (var srca: iterator\<auto(TT)\>; var srcb: iterator\<auto(TT)\>; key: block\<(arg:TT):auto\>) : iterator\<TT\> <function-linq_union_by_iterator_ls_autoTT_gr__iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_>` 
  *  :ref:`union_by_to_array (var srca: iterator\<auto(TT)\>; var srcb: iterator\<auto(TT)\>; key: block\<(arg:TT):auto\>) : array\<TT\> <function-linq_union_by_to_array_iterator_ls_autoTT_gr__iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_>` 
  *  :ref:`union_to_array (var srca: iterator\<auto(TT)\>; var srcb: iterator\<auto(TT)\>) : array\<TT\> <function-linq_union_to_array_iterator_ls_autoTT_gr__iterator_ls_autoTT_gr_>` 
  *  :ref:`unique (a: array\<auto(TT)\>) : array\<TT\> <function-linq_unique_array_ls_autoTT_gr_>` 
  *  :ref:`unique (a: iterator\<auto(TT)\>) : iterator\<TT\> <function-linq_unique_iterator_ls_autoTT_gr_>` 
  *  :ref:`unique_by (a: array\<auto(TT)\>; key: block\<(arg:TT):auto\>) : array\<TT\> <function-linq_unique_by_array_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_>` 
  *  :ref:`unique_by (a: iterator\<auto(TT)\>; key: block\<(arg:TT):auto\>) : iterator\<TT\> <function-linq_unique_by_iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_>` 
  *  :ref:`unique_by_inplace (var a: array\<auto(TT)\>; key: block\<(arg:TT):auto\>) : auto <function-linq_unique_by_inplace_array_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_>` 
  *  :ref:`unique_by_to_array (a: iterator\<auto(TT)\>; key: block\<(arg:TT):auto\>) : array\<TT\> <function-linq_unique_by_to_array_iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_>` 
  *  :ref:`unique_inplace (var a: array\<auto(TT)\>) : auto <function-linq_unique_inplace_array_ls_autoTT_gr_>` 
  *  :ref:`unique_key (a: auto) : auto <function-linq_unique_key_auto_0x1de>` 
  *  :ref:`unique_to_array (a: iterator\<auto(TT)\>) : array\<TT\> <function-linq_unique_to_array_iterator_ls_autoTT_gr_>` 


distinct
^^^^^^^^

.. _function-linq_distinct_array_ls_autoTT_gr_:

.. das:function:: distinct(a: array<auto(TT)>) : array<TT>

Returns distinct elements from an array

:Arguments: * **a** : array<auto(TT)>

.. _function-linq_distinct_iterator_ls_autoTT_gr_:

.. das:function:: distinct(a: iterator<auto(TT)>) : iterator<TT>

----


distinct_by
^^^^^^^^^^^

.. _function-linq_distinct_by_array_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_:

.. das:function:: distinct_by(a: array<auto(TT)>; key: block<(arg:TT):auto>) : array<TT>

Returns distinct elements from an array based on a key

:Arguments: * **a** : array<auto(TT)>

            * **key** : block<(arg:TT):auto>

.. _function-linq_distinct_by_iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_:

.. das:function:: distinct_by(a: iterator<auto(TT)>; key: block<(arg:TT):auto>) : iterator<TT>

----

.. _function-linq_distinct_by_inplace_array_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_:

.. das:function:: distinct_by_inplace(a: array<auto(TT)>; key: block<(arg:TT):auto>) : auto

Returns distinct elements from an array based on a key in place

:Arguments: * **a** : array<auto(TT)>

            * **key** : block<(arg:TT):auto>

.. _function-linq_distinct_by_to_array_iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_:

.. das:function:: distinct_by_to_array(a: iterator<auto(TT)>; key: block<(arg:TT):auto>) : array<TT>

Returns distinct elements from an iterator based on a key and returns an array

:Arguments: * **a** : iterator<auto(TT)>

            * **key** : block<(arg:TT):auto>

.. _function-linq_distinct_inplace_array_ls_autoTT_gr_:

.. das:function:: distinct_inplace(a: array<auto(TT)>) : auto

Returns distinct elements from an array in place

:Arguments: * **a** : array<auto(TT)>

.. _function-linq_distinct_to_array_iterator_ls_autoTT_gr_:

.. das:function:: distinct_to_array(a: iterator<auto(TT)>) : array<TT>

Returns distinct elements from an iterator and returns an array

:Arguments: * **a** : iterator<auto(TT)>


except
^^^^^^

.. _function-linq_except_iterator_ls_autoTT_gr__iterator_ls_autoTT_gr_:

.. das:function:: except(src: iterator<auto(TT)>; exclude: iterator<auto(TT)>) : iterator<TT>

Returns elements from the first iterator that are not in the second iterator

:Arguments: * **src** : iterator<auto(TT)>

            * **exclude** : iterator<auto(TT)>

.. _function-linq_except_array_ls_autoTT_gr__array_ls_autoTT_gr_:

.. das:function:: except(src: array<auto(TT)>; exclude: array<auto(TT)>) : array<TT>

----


except_by
^^^^^^^^^

.. _function-linq_except_by_array_ls_autoTT_gr__array_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_:

.. das:function:: except_by(src: array<auto(TT)>; exclude: array<auto(TT)>; key: block<(arg:TT):auto>) : array<TT>

Returns elements from the first array that are not in the second array by key

:Arguments: * **src** : array<auto(TT)>

            * **exclude** : array<auto(TT)>

            * **key** : block<(arg:TT):auto>

.. _function-linq_except_by_iterator_ls_autoTT_gr__iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_:

.. das:function:: except_by(src: iterator<auto(TT)>; exclude: iterator<auto(TT)>; key: block<(arg:TT):auto>) : iterator<TT>

----

.. _function-linq_except_by_to_array_iterator_ls_autoTT_gr__iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_:

.. das:function:: except_by_to_array(src: iterator<auto(TT)>; exclude: iterator<auto(TT)>; key: block<(arg:TT):auto>) : array<TT>

Returns elements from the first iterator that are not in the second iterator by key and returns an array

:Arguments: * **src** : iterator<auto(TT)>

            * **exclude** : iterator<auto(TT)>

            * **key** : block<(arg:TT):auto>

.. _function-linq_except_to_array_iterator_ls_autoTT_gr__iterator_ls_autoTT_gr_:

.. das:function:: except_to_array(src: iterator<auto(TT)>; exclude: iterator<auto(TT)>) : array<TT>

Returns elements from the first iterator that are not in the second iterator and returns an array

:Arguments: * **src** : iterator<auto(TT)>

            * **exclude** : iterator<auto(TT)>


intersect
^^^^^^^^^

.. _function-linq_intersect_array_ls_autoTT_gr__array_ls_autoTT_gr_:

.. das:function:: intersect(srca: array<auto(TT)>; srcb: array<auto(TT)>) : array<TT>

Returns elements that are present in both arrays

:Arguments: * **srca** : array<auto(TT)>

            * **srcb** : array<auto(TT)>

.. _function-linq_intersect_iterator_ls_autoTT_gr__iterator_ls_autoTT_gr_:

.. das:function:: intersect(srca: iterator<auto(TT)>; srcb: iterator<auto(TT)>) : iterator<TT>

----


intersect_by
^^^^^^^^^^^^

.. _function-linq_intersect_by_array_ls_autoTT_gr__array_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_:

.. das:function:: intersect_by(srca: array<auto(TT)>; srcb: array<auto(TT)>; key: block<(arg:TT):auto>) : array<TT>

Returns elements that are present in both arrays by key

:Arguments: * **srca** : array<auto(TT)>

            * **srcb** : array<auto(TT)>

            * **key** : block<(arg:TT):auto>

.. _function-linq_intersect_by_iterator_ls_autoTT_gr__iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_:

.. das:function:: intersect_by(srca: iterator<auto(TT)>; srcb: iterator<auto(TT)>; key: block<(arg:TT):auto>) : iterator<TT>

----

.. _function-linq_intersect_by_to_array_iterator_ls_autoTT_gr__iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_:

.. das:function:: intersect_by_to_array(srca: iterator<auto(TT)>; srcb: iterator<auto(TT)>; key: block<(arg:TT):auto>) : array<TT>

Returns elements that are present in both iterators by key and returns an array

:Arguments: * **srca** : iterator<auto(TT)>

            * **srcb** : iterator<auto(TT)>

            * **key** : block<(arg:TT):auto>

.. _function-linq_intersect_to_array_iterator_ls_autoTT_gr__iterator_ls_autoTT_gr_:

.. das:function:: intersect_to_array(srca: iterator<auto(TT)>; srcb: iterator<auto(TT)>) : array<TT>

Returns elements that are present in both iterators and returns an array

:Arguments: * **srca** : iterator<auto(TT)>

            * **srcb** : iterator<auto(TT)>


union
^^^^^

.. _function-linq_union_iterator_ls_autoTT_gr__iterator_ls_autoTT_gr_:

.. das:function:: union(srca: iterator<auto(TT)>; srcb: iterator<auto(TT)>) : iterator<TT>

Returns distinct elements from the concatenation of two iterators

:Arguments: * **srca** : iterator<auto(TT)>

            * **srcb** : iterator<auto(TT)>

.. _function-linq_union_array_ls_autoTT_gr__array_ls_autoTT_gr_:

.. das:function:: union(srca: array<auto(TT)>; srcb: array<auto(TT)>) : array<TT>

----


union_by
^^^^^^^^

.. _function-linq_union_by_array_ls_autoTT_gr__array_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_:

.. das:function:: union_by(srca: array<auto(TT)>; srcb: array<auto(TT)>; key: block<(arg:TT):auto>) : array<TT>

Returns distinct elements from the concatenation of two arrays by key

:Arguments: * **srca** : array<auto(TT)>

            * **srcb** : array<auto(TT)>

            * **key** : block<(arg:TT):auto>

.. _function-linq_union_by_iterator_ls_autoTT_gr__iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_:

.. das:function:: union_by(srca: iterator<auto(TT)>; srcb: iterator<auto(TT)>; key: block<(arg:TT):auto>) : iterator<TT>

----

.. _function-linq_union_by_to_array_iterator_ls_autoTT_gr__iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_:

.. das:function:: union_by_to_array(srca: iterator<auto(TT)>; srcb: iterator<auto(TT)>; key: block<(arg:TT):auto>) : array<TT>

Returns distinct elements from the concatenation of two iterators by key and returns an array

:Arguments: * **srca** : iterator<auto(TT)>

            * **srcb** : iterator<auto(TT)>

            * **key** : block<(arg:TT):auto>

.. _function-linq_union_to_array_iterator_ls_autoTT_gr__iterator_ls_autoTT_gr_:

.. das:function:: union_to_array(srca: iterator<auto(TT)>; srcb: iterator<auto(TT)>) : array<TT>

Returns distinct elements from the concatenation of two iterators and returns an array

:Arguments: * **srca** : iterator<auto(TT)>

            * **srcb** : iterator<auto(TT)>


unique
^^^^^^

.. _function-linq_unique_array_ls_autoTT_gr_:

.. das:function:: unique(a: array<auto(TT)>) : array<TT>

sort and remove duplicate elements from an array

:Arguments: * **a** : array<auto(TT)>

.. _function-linq_unique_iterator_ls_autoTT_gr_:

.. das:function:: unique(a: iterator<auto(TT)>) : iterator<TT>

----


unique_by
^^^^^^^^^

.. _function-linq_unique_by_array_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_:

.. das:function:: unique_by(a: array<auto(TT)>; key: block<(arg:TT):auto>) : array<TT>

sort and remove duplicate elements from an array based on a key

:Arguments: * **a** : array<auto(TT)>

            * **key** : block<(arg:TT):auto>

.. _function-linq_unique_by_iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_:

.. das:function:: unique_by(a: iterator<auto(TT)>; key: block<(arg:TT):auto>) : iterator<TT>

----

.. _function-linq_unique_by_inplace_array_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_:

.. das:function:: unique_by_inplace(a: array<auto(TT)>; key: block<(arg:TT):auto>) : auto

remove duplicate elements from an array based on a key in place

:Arguments: * **a** : array<auto(TT)>

            * **key** : block<(arg:TT):auto>

.. _function-linq_unique_by_to_array_iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_:

.. das:function:: unique_by_to_array(a: iterator<auto(TT)>; key: block<(arg:TT):auto>) : array<TT>

sort and remove duplicate elements from an iterator based on a key and returns an array

:Arguments: * **a** : iterator<auto(TT)>

            * **key** : block<(arg:TT):auto>

.. _function-linq_unique_inplace_array_ls_autoTT_gr_:

.. das:function:: unique_inplace(a: array<auto(TT)>) : auto

remove duplicate elements from sorted array in place

:Arguments: * **a** : array<auto(TT)>

.. _function-linq_unique_key_auto_0x1de:

.. das:function:: unique_key(a: auto) : auto

generates unique key of workhorse type for the value

:Arguments: * **a** : auto

.. _function-linq_unique_to_array_iterator_ls_autoTT_gr_:

.. das:function:: unique_to_array(a: iterator<auto(TT)>) : array<TT>

sort and remove duplicate elements from an iterator and returns an array

:Arguments: * **a** : iterator<auto(TT)>

++++++++++++++++++++++++
Concatenation operations
++++++++++++++++++++++++

  *  :ref:`append (arr: array\<auto(TT)\>; value: TT) : array\<TT\> <function-linq_append_array_ls_autoTT_gr__TT>` 
  *  :ref:`append (var it: iterator\<auto(TT)\>; value: TT) : iterator\<TT\> <function-linq_append_iterator_ls_autoTT_gr__TT>` 
  *  :ref:`append_inplace (var arr: array\<auto(TT)\>; value: TT) : auto <function-linq_append_inplace_array_ls_autoTT_gr__TT>` 
  *  :ref:`append_to_array (var it: iterator\<auto(TT)\>; value: TT) : array\<TT\> <function-linq_append_to_array_iterator_ls_autoTT_gr__TT>` 
  *  :ref:`concat (var a: iterator\<auto(TT)\>; var b: iterator\<auto(TT)\>) : iterator\<TT\> <function-linq_concat_iterator_ls_autoTT_gr__iterator_ls_autoTT_gr_>` 
  *  :ref:`concat (a: array\<auto(TT)\>; b: array\<auto(TT)\>) : array\<TT\> <function-linq_concat_array_ls_autoTT_gr__array_ls_autoTT_gr_>` 
  *  :ref:`concat_inplace (var a: array\<auto(TT)\>; b: array\<auto(TT)\>) : auto <function-linq_concat_inplace_array_ls_autoTT_gr__array_ls_autoTT_gr_>` 
  *  :ref:`concat_to_array (var a: iterator\<auto(TT)\>; var b: iterator\<auto(TT)\>) : array\<TT\> <function-linq_concat_to_array_iterator_ls_autoTT_gr__iterator_ls_autoTT_gr_>` 
  *  :ref:`prepend (var it: iterator\<auto(TT)\>; value: TT) : iterator\<TT\> <function-linq_prepend_iterator_ls_autoTT_gr__TT>` 
  *  :ref:`prepend (arr: array\<auto(TT)\>; value: TT) : array\<TT\> <function-linq_prepend_array_ls_autoTT_gr__TT>` 
  *  :ref:`prepend_inplace (var arr: array\<auto(TT)\>; value: TT) : auto <function-linq_prepend_inplace_array_ls_autoTT_gr__TT>` 
  *  :ref:`prepend_to_array (var it: iterator\<auto(TT)\>; value: TT) : array\<TT\> <function-linq_prepend_to_array_iterator_ls_autoTT_gr__TT>` 


append
^^^^^^

.. _function-linq_append_array_ls_autoTT_gr__TT:

.. das:function:: append(arr: array<auto(TT)>; value: TT) : array<TT>

Appends a value to the end of an array

:Arguments: * **arr** : array<auto(TT)>

            * **value** : TT

.. _function-linq_append_iterator_ls_autoTT_gr__TT:

.. das:function:: append(it: iterator<auto(TT)>; value: TT) : iterator<TT>

----

.. _function-linq_append_inplace_array_ls_autoTT_gr__TT:

.. das:function:: append_inplace(arr: array<auto(TT)>; value: TT) : auto

Appends a value to the end of an array in place

:Arguments: * **arr** : array<auto(TT)>

            * **value** : TT

.. _function-linq_append_to_array_iterator_ls_autoTT_gr__TT:

.. das:function:: append_to_array(it: iterator<auto(TT)>; value: TT) : array<TT>

Appends a value to the end of an iterator and returns an array

:Arguments: * **it** : iterator<auto(TT)>

            * **value** : TT


concat
^^^^^^

.. _function-linq_concat_iterator_ls_autoTT_gr__iterator_ls_autoTT_gr_:

.. das:function:: concat(a: iterator<auto(TT)>; b: iterator<auto(TT)>) : iterator<TT>

Concatenates two iterators

:Arguments: * **a** : iterator<auto(TT)>

            * **b** : iterator<auto(TT)>

.. _function-linq_concat_array_ls_autoTT_gr__array_ls_autoTT_gr_:

.. das:function:: concat(a: array<auto(TT)>; b: array<auto(TT)>) : array<TT>

----

.. _function-linq_concat_inplace_array_ls_autoTT_gr__array_ls_autoTT_gr_:

.. das:function:: concat_inplace(a: array<auto(TT)>; b: array<auto(TT)>) : auto

Concatenates two arrays in place

:Arguments: * **a** : array<auto(TT)>

            * **b** : array<auto(TT)>

.. _function-linq_concat_to_array_iterator_ls_autoTT_gr__iterator_ls_autoTT_gr_:

.. das:function:: concat_to_array(a: iterator<auto(TT)>; b: iterator<auto(TT)>) : array<TT>

Concatenates two iterators and returns an array

:Arguments: * **a** : iterator<auto(TT)>

            * **b** : iterator<auto(TT)>


prepend
^^^^^^^

.. _function-linq_prepend_iterator_ls_autoTT_gr__TT:

.. das:function:: prepend(it: iterator<auto(TT)>; value: TT) : iterator<TT>

Prepends a value to the beginning of an iterator

:Arguments: * **it** : iterator<auto(TT)>

            * **value** : TT

.. _function-linq_prepend_array_ls_autoTT_gr__TT:

.. das:function:: prepend(arr: array<auto(TT)>; value: TT) : array<TT>

----

.. _function-linq_prepend_inplace_array_ls_autoTT_gr__TT:

.. das:function:: prepend_inplace(arr: array<auto(TT)>; value: TT) : auto

Prepends a value to the beginning of an array in place

:Arguments: * **arr** : array<auto(TT)>

            * **value** : TT

.. _function-linq_prepend_to_array_iterator_ls_autoTT_gr__TT:

.. das:function:: prepend_to_array(it: iterator<auto(TT)>; value: TT) : array<TT>

Prepends a value to the beginning of an iterator and returns an array

:Arguments: * **it** : iterator<auto(TT)>

            * **value** : TT

+++++++++++++++++++++
Generation operations
+++++++++++++++++++++

  *  :ref:`default_empty (var src: iterator\<auto(TT)\>) : iterator\<TT\> <function-linq_default_empty_iterator_ls_autoTT_gr_>` 
  *  :ref:`empty (var typ: auto(TT)) : iterator\<TT\> <function-linq_empty_autoTT_0x858>` 
  *  :ref:`range_sequence (start: int; count: int) : iterator\<int\> <function-linq_range_sequence_int_int>` 
  *  :ref:`repeat (element: auto(TT); count: int) : iterator\<TT\> <function-linq_repeat_autoTT_int_0x87f>` 

.. _function-linq_default_empty_iterator_ls_autoTT_gr_:

.. das:function:: default_empty(src: iterator<auto(TT)>) : iterator<TT>

Returns the elements of the iterator, or a default value if the iterator is empty

:Arguments: * **src** : iterator<auto(TT)>

.. _function-linq_empty_autoTT_0x858:

.. das:function:: empty(typ: auto(TT)) : iterator<TT>

Returns an empty iterator of the specified type

:Arguments: * **typ** : auto(TT)

.. _function-linq_range_sequence_int_int:

.. das:function:: range_sequence(start: int; count: int) : iterator<int>

Generates a sequence of integers within a specified range

:Arguments: * **start** : int

            * **count** : int

.. _function-linq_repeat_autoTT_int_0x87f:

.. das:function:: repeat(element: auto(TT); count: int) : iterator<TT>

Generates a sequence that contains one repeated value

:Arguments: * **element** : auto(TT)

            * **count** : int

++++++++++++++++++++++
Aggregation operations
++++++++++++++++++++++

  *  :ref:`aggregate (src: array\<auto(TT)\>; seed: auto(AGG); func: block\<(acc:AGG;x:TT):AGG\>) : AGG <function-linq_aggregate_array_ls_autoTT_gr__autoAGG_block_ls_acc_c_AGG;x_c_TT_c_AGG_gr__0x583>` 
  *  :ref:`aggregate (var src: iterator\<auto(TT)\>; seed: auto(AGG); func: block\<(acc:AGG;x:TT):AGG\>) : AGG <function-linq_aggregate_iterator_ls_autoTT_gr__autoAGG_block_ls_acc_c_AGG;x_c_TT_c_AGG_gr__0x579>` 
  *  :ref:`average (src: array\<auto(TT)\>) : TT <function-linq_average_array_ls_autoTT_gr_>` 
  *  :ref:`average (var src: iterator\<auto(TT)\>) : TT <function-linq_average_iterator_ls_autoTT_gr_>` 
  *  :ref:`count (a: array\<auto(TT)\>) : int <function-linq_count_array_ls_autoTT_gr_>` 
  *  :ref:`count (var a: iterator\<auto(TT)\>) : int <function-linq_count_iterator_ls_autoTT_gr_>` 
  *  :ref:`long_count (a: array\<auto(TT)\>) : int64 <function-linq_long_count_array_ls_autoTT_gr_>` 
  *  :ref:`long_count (var a: iterator\<auto(TT)\>) : int64 <function-linq_long_count_iterator_ls_autoTT_gr_>` 
  *  :ref:`max (var src: iterator\<auto(TT)\>) : TT <function-linq_max_iterator_ls_autoTT_gr_>` 
  *  :ref:`max (src: array\<auto(TT)\>) : TT <function-linq_max_array_ls_autoTT_gr_>` 
  *  :ref:`max_by (src: array\<auto(TT)\>; key: auto) : TT <function-linq_max_by_array_ls_autoTT_gr__auto_0x509>` 
  *  :ref:`max_by (var src: iterator\<auto(TT)\>; key: auto) : TT <function-linq_max_by_iterator_ls_autoTT_gr__auto_0x4ff>` 
  *  :ref:`min (src: array\<auto(TT)\>) : TT <function-linq_min_array_ls_autoTT_gr_>` 
  *  :ref:`min (var src: iterator\<auto(TT)\>) : TT <function-linq_min_iterator_ls_autoTT_gr_>` 
  *  :ref:`min_by (var src: iterator\<auto(TT)\>; key: auto) : TT <function-linq_min_by_iterator_ls_autoTT_gr__auto_0x4a7>` 
  *  :ref:`min_by (src: array\<auto(TT)\>; key: auto) : TT <function-linq_min_by_array_ls_autoTT_gr__auto_0x4b1>` 
  *  :ref:`min_max (var src: iterator\<auto(TT)\>) : tuple\<TT;TT\> <function-linq_min_max_iterator_ls_autoTT_gr_>` 
  *  :ref:`min_max (src: array\<auto(TT)\>) : tuple\<TT;TT\> <function-linq_min_max_array_ls_autoTT_gr_>` 
  *  :ref:`min_max_average (src: array\<auto(TT)\>) : tuple\<TT;TT;TT\> <function-linq_min_max_average_array_ls_autoTT_gr_>` 
  *  :ref:`min_max_average (var src: iterator\<auto(TT)\>) : tuple\<TT;TT;TT\> <function-linq_min_max_average_iterator_ls_autoTT_gr_>` 
  *  :ref:`min_max_average_by (src: array\<auto(TT)\>; key: auto) : tuple\<TT;TT;TT\> <function-linq_min_max_average_by_array_ls_autoTT_gr__auto_0x640>` 
  *  :ref:`min_max_average_by (var src: iterator\<auto(TT)\>; key: auto) : tuple\<TT;TT;TT\> <function-linq_min_max_average_by_iterator_ls_autoTT_gr__auto_0x63a>` 
  *  :ref:`min_max_by (src: array\<auto(TT)\>; key: auto) : tuple\<TT;TT\> <function-linq_min_max_by_array_ls_autoTT_gr__auto_0x55d>` 
  *  :ref:`min_max_by (var src: iterator\<auto(TT)\>; key: auto) : tuple\<TT;TT\> <function-linq_min_max_by_iterator_ls_autoTT_gr__auto_0x557>` 
  *  :ref:`sum (var src: iterator\<auto(TT)\>) : TT <function-linq_sum_iterator_ls_autoTT_gr_>` 
  *  :ref:`sum (src: array\<auto(TT)\>) : TT <function-linq_sum_array_ls_autoTT_gr_>` 


aggregate
^^^^^^^^^

.. _function-linq_aggregate_array_ls_autoTT_gr__autoAGG_block_ls_acc_c_AGG;x_c_TT_c_AGG_gr__0x583:

.. das:function:: aggregate(src: array<auto(TT)>; seed: auto(AGG); func: block<(acc:AGG;x:TT):AGG>) : AGG

Aggregates elements in an array using a seed and a function

:Arguments: * **src** : array<auto(TT)>

            * **seed** : auto(AGG)

            * **func** : block<(acc:AGG;x:TT):AGG>

.. _function-linq_aggregate_iterator_ls_autoTT_gr__autoAGG_block_ls_acc_c_AGG;x_c_TT_c_AGG_gr__0x579:

.. das:function:: aggregate(src: iterator<auto(TT)>; seed: auto(AGG); func: block<(acc:AGG;x:TT):AGG>) : AGG

----


average
^^^^^^^

.. _function-linq_average_array_ls_autoTT_gr_:

.. das:function:: average(src: array<auto(TT)>) : TT

Averages elements in an array

:Arguments: * **src** : array<auto(TT)>

.. _function-linq_average_iterator_ls_autoTT_gr_:

.. das:function:: average(src: iterator<auto(TT)>) : TT

----


count
^^^^^

.. _function-linq_count_array_ls_autoTT_gr_:

.. das:function:: count(a: array<auto(TT)>) : int

Counts elements in an array

:Arguments: * **a** : array<auto(TT)>

.. _function-linq_count_iterator_ls_autoTT_gr_:

.. das:function:: count(a: iterator<auto(TT)>) : int

----


long_count
^^^^^^^^^^

.. _function-linq_long_count_array_ls_autoTT_gr_:

.. das:function:: long_count(a: array<auto(TT)>) : int64

Counts elements in an array, using a long integer

:Arguments: * **a** : array<auto(TT)>

.. _function-linq_long_count_iterator_ls_autoTT_gr_:

.. das:function:: long_count(a: iterator<auto(TT)>) : int64

----


max
^^^

.. _function-linq_max_iterator_ls_autoTT_gr_:

.. das:function:: max(src: iterator<auto(TT)>) : TT

Finds the maximum element in an iterator

:Arguments: * **src** : iterator<auto(TT)>

.. _function-linq_max_array_ls_autoTT_gr_:

.. das:function:: max(src: array<auto(TT)>) : TT

----


max_by
^^^^^^

.. _function-linq_max_by_array_ls_autoTT_gr__auto_0x509:

.. das:function:: max_by(src: array<auto(TT)>; key: auto) : TT

Finds the maximum element in an array by key

:Arguments: * **src** : array<auto(TT)>

            * **key** : auto

.. _function-linq_max_by_iterator_ls_autoTT_gr__auto_0x4ff:

.. das:function:: max_by(src: iterator<auto(TT)>; key: auto) : TT

----


min
^^^

.. _function-linq_min_array_ls_autoTT_gr_:

.. das:function:: min(src: array<auto(TT)>) : TT

Finds the minimum element in an array

:Arguments: * **src** : array<auto(TT)>

.. _function-linq_min_iterator_ls_autoTT_gr_:

.. das:function:: min(src: iterator<auto(TT)>) : TT

----


min_by
^^^^^^

.. _function-linq_min_by_iterator_ls_autoTT_gr__auto_0x4a7:

.. das:function:: min_by(src: iterator<auto(TT)>; key: auto) : TT

Finds the minimum element in an iterator by key

:Arguments: * **src** : iterator<auto(TT)>

            * **key** : auto

.. _function-linq_min_by_array_ls_autoTT_gr__auto_0x4b1:

.. das:function:: min_by(src: array<auto(TT)>; key: auto) : TT

----


min_max
^^^^^^^

.. _function-linq_min_max_iterator_ls_autoTT_gr_:

.. das:function:: min_max(src: iterator<auto(TT)>) : tuple<TT;TT>

Finds the minimum and maximum elements in an iterator

:Arguments: * **src** : iterator<auto(TT)>

.. _function-linq_min_max_array_ls_autoTT_gr_:

.. das:function:: min_max(src: array<auto(TT)>) : tuple<TT;TT>

----


min_max_average
^^^^^^^^^^^^^^^

.. _function-linq_min_max_average_array_ls_autoTT_gr_:

.. das:function:: min_max_average(src: array<auto(TT)>) : tuple<TT;TT;TT>

Finds the minimum, maximum, and average elements in an array

:Arguments: * **src** : array<auto(TT)>

.. _function-linq_min_max_average_iterator_ls_autoTT_gr_:

.. das:function:: min_max_average(src: iterator<auto(TT)>) : tuple<TT;TT;TT>

----


min_max_average_by
^^^^^^^^^^^^^^^^^^

.. _function-linq_min_max_average_by_array_ls_autoTT_gr__auto_0x640:

.. das:function:: min_max_average_by(src: array<auto(TT)>; key: auto) : tuple<TT;TT;TT>

Finds the minimum, maximum, and average elements in an array by key

:Arguments: * **src** : array<auto(TT)>

            * **key** : auto

.. _function-linq_min_max_average_by_iterator_ls_autoTT_gr__auto_0x63a:

.. das:function:: min_max_average_by(src: iterator<auto(TT)>; key: auto) : tuple<TT;TT;TT>

----


min_max_by
^^^^^^^^^^

.. _function-linq_min_max_by_array_ls_autoTT_gr__auto_0x55d:

.. das:function:: min_max_by(src: array<auto(TT)>; key: auto) : tuple<TT;TT>

Finds the minimum and maximum elements in an array by key

:Arguments: * **src** : array<auto(TT)>

            * **key** : auto

.. _function-linq_min_max_by_iterator_ls_autoTT_gr__auto_0x557:

.. das:function:: min_max_by(src: iterator<auto(TT)>; key: auto) : tuple<TT;TT>

----


sum
^^^

.. _function-linq_sum_iterator_ls_autoTT_gr_:

.. das:function:: sum(src: iterator<auto(TT)>) : TT

Sums elements in an iterator

:Arguments: * **src** : iterator<auto(TT)>

.. _function-linq_sum_array_ls_autoTT_gr_:

.. das:function:: sum(src: array<auto(TT)>) : TT

----

++++++++++++++
Filtering data
++++++++++++++

  *  :ref:`where_ (src: array\<auto(TT)\>; predicate: block\<(arg:TT):bool\>) : array\<TT\> <function-linq_where__array_ls_autoTT_gr__block_ls_arg_c_TT_c_bool_gr_>` 
  *  :ref:`where_ (var src: iterator\<auto(TT)\>; predicate: block\<(arg:TT):bool\>) : iterator\<TT\> <function-linq_where__iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_bool_gr_>` 
  *  :ref:`where_to_array (var src: iterator\<auto(TT)\>; predicate: block\<(arg:TT):bool\>) : array\<TT\> <function-linq_where_to_array_iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_bool_gr_>` 


where_
^^^^^^

.. _function-linq_where__array_ls_autoTT_gr__block_ls_arg_c_TT_c_bool_gr_:

.. das:function:: where_(src: array<auto(TT)>; predicate: block<(arg:TT):bool>) : array<TT>

Filters elements in an array based on a predicate

:Arguments: * **src** : array<auto(TT)>

            * **predicate** : block<(arg:TT):bool>

.. _function-linq_where__iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_bool_gr_:

.. das:function:: where_(src: iterator<auto(TT)>; predicate: block<(arg:TT):bool>) : iterator<TT>

----

.. _function-linq_where_to_array_iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_bool_gr_:

.. das:function:: where_to_array(src: iterator<auto(TT)>; predicate: block<(arg:TT):bool>) : array<TT>

Filters elements in an iterator based on a predicate and returns an array

:Arguments: * **src** : iterator<auto(TT)>

            * **predicate** : block<(arg:TT):bool>

+++++++++++++++++
Partitioning data
+++++++++++++++++

  *  :ref:`chunk (var src: iterator\<auto(TT)\>; size: int) : iterator\<array\<TT\>\> <function-linq_chunk_iterator_ls_autoTT_gr__int>` 
  *  :ref:`chunk (src: array\<auto(TT)\>; size: int) : array\<array\<TT\>\> <function-linq_chunk_array_ls_autoTT_gr__int>` 
  *  :ref:`chunk_to_array (var src: iterator\<auto(TT)\>; size: int) : array\<array\<TT\>\> <function-linq_chunk_to_array_iterator_ls_autoTT_gr__int>` 
  *  :ref:`skip (var src: iterator\<auto(TT)\>; var total: int) : iterator\<TT\> <function-linq_skip_iterator_ls_autoTT_gr__int>` 
  *  :ref:`skip (arr: array\<auto(TT)\>; var total: int) : array\<TT\> <function-linq_skip_array_ls_autoTT_gr__int>` 
  *  :ref:`skip_inplace (var arr: array\<auto(TT)\>; var total: int) : auto <function-linq_skip_inplace_array_ls_autoTT_gr__int>` 
  *  :ref:`skip_to_array (var src: iterator\<auto(TT)\>; var total: int) : array\<TT\> <function-linq_skip_to_array_iterator_ls_autoTT_gr__int>` 
  *  :ref:`skip_while (src: array\<auto(TT)\>; predicate: block\<(arg:TT):bool\>) : array\<TT\> <function-linq_skip_while_array_ls_autoTT_gr__block_ls_arg_c_TT_c_bool_gr_>` 
  *  :ref:`skip_while (var src: iterator\<auto(TT)\>; predicate: block\<(arg:TT):bool\>) : iterator\<TT\> <function-linq_skip_while_iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_bool_gr_>` 
  *  :ref:`skip_while_to_array (var src: iterator\<auto(TT)\>; predicate: block\<(arg:TT):bool\>) : array\<TT\> <function-linq_skip_while_to_array_iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_bool_gr_>` 
  *  :ref:`take (src: array\<auto(TT)\>; from: range) : array\<TT\> <function-linq_take_array_ls_autoTT_gr__range>` 
  *  :ref:`take (var src: iterator\<auto(TT)\>; from: range) : iterator\<TT\> <function-linq_take_iterator_ls_autoTT_gr__range>` 
  *  :ref:`take (var src: iterator\<auto(TT)\>; var total: int) : iterator\<TT\> <function-linq_take_iterator_ls_autoTT_gr__int>` 
  *  :ref:`take (arr: array\<auto(TT)\>; var total: int) : array\<TT\> <function-linq_take_array_ls_autoTT_gr__int>` 
  *  :ref:`take_inplace (var arr: array\<auto(TT)\>; from: range) : auto <function-linq_take_inplace_array_ls_autoTT_gr__range>` 
  *  :ref:`take_inplace (var arr: array\<auto(TT)\>; var total: int) : auto <function-linq_take_inplace_array_ls_autoTT_gr__int>` 
  *  :ref:`take_to_array (var src: iterator\<auto(TT)\>; var total: int) : array\<TT\> <function-linq_take_to_array_iterator_ls_autoTT_gr__int>` 
  *  :ref:`take_to_array (var src: iterator\<auto(TT)\>; from: range) : array\<TT\> <function-linq_take_to_array_iterator_ls_autoTT_gr__range>` 
  *  :ref:`take_while (src: array\<auto(TT)\>; predicate: block\<(arg:TT):bool\>) : array\<TT\> <function-linq_take_while_array_ls_autoTT_gr__block_ls_arg_c_TT_c_bool_gr_>` 
  *  :ref:`take_while (var src: iterator\<auto(TT)\>; predicate: block\<(arg:TT):bool\>) : auto <function-linq_take_while_iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_bool_gr_>` 
  *  :ref:`take_while_to_array (var src: iterator\<auto(TT)\>; predicate: block\<(arg:TT):bool\>) : array\<TT\> <function-linq_take_while_to_array_iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_bool_gr_>` 


chunk
^^^^^

.. _function-linq_chunk_iterator_ls_autoTT_gr__int:

.. das:function:: chunk(src: iterator<auto(TT)>; size: int) : iterator<array<TT>>

Splits an iterator into chunks of a specified size

:Arguments: * **src** : iterator<auto(TT)>

            * **size** : int

.. _function-linq_chunk_array_ls_autoTT_gr__int:

.. das:function:: chunk(src: array<auto(TT)>; size: int) : array<array<TT>>

----

.. _function-linq_chunk_to_array_iterator_ls_autoTT_gr__int:

.. das:function:: chunk_to_array(src: iterator<auto(TT)>; size: int) : array<array<TT>>

Splits an iterator into chunks of a specified size and returns an array

:Arguments: * **src** : iterator<auto(TT)>

            * **size** : int


skip
^^^^

.. _function-linq_skip_iterator_ls_autoTT_gr__int:

.. das:function:: skip(src: iterator<auto(TT)>; total: int) : iterator<TT>

Yields all but the first `total` elements

:Arguments: * **src** : iterator<auto(TT)>

            * **total** : int

.. _function-linq_skip_array_ls_autoTT_gr__int:

.. das:function:: skip(arr: array<auto(TT)>; total: int) : array<TT>

----

.. _function-linq_skip_inplace_array_ls_autoTT_gr__int:

.. das:function:: skip_inplace(arr: array<auto(TT)>; total: int) : auto

Removes the first `total` elements from an array in place

:Arguments: * **arr** : array<auto(TT)>

            * **total** : int

.. _function-linq_skip_to_array_iterator_ls_autoTT_gr__int:

.. das:function:: skip_to_array(src: iterator<auto(TT)>; total: int) : array<TT>

Yields all but the first `total` elements and returns an array

:Arguments: * **src** : iterator<auto(TT)>

            * **total** : int


skip_while
^^^^^^^^^^

.. _function-linq_skip_while_array_ls_autoTT_gr__block_ls_arg_c_TT_c_bool_gr_:

.. das:function:: skip_while(src: array<auto(TT)>; predicate: block<(arg:TT):bool>) : array<TT>

Skips all elements of an array while the predicate is true

:Arguments: * **src** : array<auto(TT)>

            * **predicate** : block<(arg:TT):bool>

.. _function-linq_skip_while_iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_bool_gr_:

.. das:function:: skip_while(src: iterator<auto(TT)>; predicate: block<(arg:TT):bool>) : iterator<TT>

----

.. _function-linq_skip_while_to_array_iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_bool_gr_:

.. das:function:: skip_while_to_array(src: iterator<auto(TT)>; predicate: block<(arg:TT):bool>) : array<TT>

Skips all elements of an iterator while the predicate is true and returns an array

:Arguments: * **src** : iterator<auto(TT)>

            * **predicate** : block<(arg:TT):bool>


take
^^^^

.. _function-linq_take_array_ls_autoTT_gr__range:

.. das:function:: take(src: array<auto(TT)>; from: range) : array<TT>

Yields a range of elements from an array

:Arguments: * **src** : array<auto(TT)>

            * **from** : range

.. _function-linq_take_iterator_ls_autoTT_gr__range:

.. das:function:: take(src: iterator<auto(TT)>; from: range) : iterator<TT>

.. _function-linq_take_iterator_ls_autoTT_gr__int:

.. das:function:: take(src: iterator<auto(TT)>; total: int) : iterator<TT>

.. _function-linq_take_array_ls_autoTT_gr__int:

.. das:function:: take(arr: array<auto(TT)>; total: int) : array<TT>

----


take_inplace
^^^^^^^^^^^^

.. _function-linq_take_inplace_array_ls_autoTT_gr__range:

.. das:function:: take_inplace(arr: array<auto(TT)>; from: range) : auto

Keeps only a range of elements in an array in place

:Arguments: * **arr** : array<auto(TT)>

            * **from** : range

.. _function-linq_take_inplace_array_ls_autoTT_gr__int:

.. das:function:: take_inplace(arr: array<auto(TT)>; total: int) : auto

----


take_to_array
^^^^^^^^^^^^^

.. _function-linq_take_to_array_iterator_ls_autoTT_gr__int:

.. das:function:: take_to_array(src: iterator<auto(TT)>; total: int) : array<TT>

Yields only the first `total` elements and returns an array

:Arguments: * **src** : iterator<auto(TT)>

            * **total** : int

.. _function-linq_take_to_array_iterator_ls_autoTT_gr__range:

.. das:function:: take_to_array(src: iterator<auto(TT)>; from: range) : array<TT>

----


take_while
^^^^^^^^^^

.. _function-linq_take_while_array_ls_autoTT_gr__block_ls_arg_c_TT_c_bool_gr_:

.. das:function:: take_while(src: array<auto(TT)>; predicate: block<(arg:TT):bool>) : array<TT>

Yields only the elements of an array while the predicate is true

:Arguments: * **src** : array<auto(TT)>

            * **predicate** : block<(arg:TT):bool>

.. _function-linq_take_while_iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_bool_gr_:

.. das:function:: take_while(src: iterator<auto(TT)>; predicate: block<(arg:TT):bool>) : auto

----

.. _function-linq_take_while_to_array_iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_bool_gr_:

.. das:function:: take_while_to_array(src: iterator<auto(TT)>; predicate: block<(arg:TT):bool>) : array<TT>

Yields only the elements of an iterator while the predicate is true and returns an array

:Arguments: * **src** : iterator<auto(TT)>

            * **predicate** : block<(arg:TT):bool>

+++++++++++++++++++++++++
Join and group operations
+++++++++++++++++++++++++

  *  :ref:`group_by (var source: iterator\<auto(TT)\>; key: auto; element_selector: auto; result_selector: auto) : auto <function-linq_group_by_iterator_ls_autoTT_gr__auto_auto_auto_0x6c4>` 
  *  :ref:`group_by (source: array\<auto(TT)\>; key: auto; element_selector: auto; result_selector: auto) : auto <function-linq_group_by_array_ls_autoTT_gr__auto_auto_auto_0x6ca>` 
  *  :ref:`group_by_to_array (var source: iterator\<auto(TT)\>; key: auto; element_selector: auto; result_selector: auto) : auto <function-linq_group_by_to_array_iterator_ls_autoTT_gr__auto_auto_auto_0x6d0>` 
  *  :ref:`group_join (var srca: iterator\<auto(TA)\>; var srcb: iterator\<auto(TB)\>; keya: auto; keyb: auto; result: auto) : iterator\<typedecl(result(type\<TA\>,type\<array\<TB -const -&\>\>))\> <function-linq_group_join_iterator_ls_autoTA_gr__iterator_ls_autoTB_gr__auto_auto_auto_0x694>` 
  *  :ref:`group_join (srca: array\<auto(TA)\>; srcb: array\<auto(TB)\>; keya: auto; keyb: auto; result: auto) : array\<typedecl(result(type\<TA\>,type\<array\<TB -const -&\>\>))\> <function-linq_group_join_array_ls_autoTA_gr__array_ls_autoTB_gr__auto_auto_auto_0x69a>` 
  *  :ref:`group_join_to_array (var srca: iterator\<auto(TA)\>; var srcb: iterator\<auto(TB)\>; keya: auto; keyb: auto; result: auto) : array\<typedecl(result(type\<TA\>,type\<array\<TB -const -&\>\>))\> <function-linq_group_join_to_array_iterator_ls_autoTA_gr__iterator_ls_autoTB_gr__auto_auto_auto_0x6a0>` 
  *  :ref:`join (srca: array\<auto(TA)\>; srcb: array\<auto(TB)\>; keya: auto; keyb: auto; result: auto) : array\<typedecl(result(type\<TA\>,type\<TB\>))\> <function-linq_join_array_ls_autoTA_gr__array_ls_autoTB_gr__auto_auto_auto_0x66b>` 
  *  :ref:`join (var srca: iterator\<auto(TA)\>; var srcb: iterator\<auto(TB)\>; keya: auto; keyb: auto; result: auto) : iterator\<typedecl(result(type\<TA\>,type\<TB\>))\> <function-linq_join_iterator_ls_autoTA_gr__iterator_ls_autoTB_gr__auto_auto_auto_0x665>` 
  *  :ref:`join_to_array (var srca: iterator\<auto(TA)\>; var srcb: iterator\<auto(TB)\>; keya: auto; keyb: auto; result: auto) : array\<typedecl(result(type\<TA\>,type\<TB\>))\> <function-linq_join_to_array_iterator_ls_autoTA_gr__iterator_ls_autoTB_gr__auto_auto_auto_0x671>` 


group_by
^^^^^^^^

.. _function-linq_group_by_iterator_ls_autoTT_gr__auto_auto_auto_0x6c4:

.. das:function:: group_by(source: iterator<auto(TT)>; key: auto; element_selector: auto; result_selector: auto) : auto

Groups the elements of an iterator according to a specified key selector function

:Arguments: * **source** : iterator<auto(TT)>

            * **key** : auto

            * **element_selector** : auto

            * **result_selector** : auto

.. _function-linq_group_by_array_ls_autoTT_gr__auto_auto_auto_0x6ca:

.. das:function:: group_by(source: array<auto(TT)>; key: auto; element_selector: auto; result_selector: auto) : auto

----

.. _function-linq_group_by_to_array_iterator_ls_autoTT_gr__auto_auto_auto_0x6d0:

.. das:function:: group_by_to_array(source: iterator<auto(TT)>; key: auto; element_selector: auto; result_selector: auto) : auto

Groups the elements of an iterator according to a specified key selector function and returns an array

:Arguments: * **source** : iterator<auto(TT)>

            * **key** : auto

            * **element_selector** : auto

            * **result_selector** : auto


group_join
^^^^^^^^^^

.. _function-linq_group_join_iterator_ls_autoTA_gr__iterator_ls_autoTB_gr__auto_auto_auto_0x694:

.. das:function:: group_join(srca: iterator<auto(TA)>; srcb: iterator<auto(TB)>; keya: auto; keyb: auto; result: auto) : iterator<typedecl(result(type<TA>,type<array<TB -const -&>>))>

we pass TA, and sequence of TB to 'result'

:Arguments: * **srca** : iterator<auto(TA)>

            * **srcb** : iterator<auto(TB)>

            * **keya** : auto

            * **keyb** : auto

            * **result** : auto

.. _function-linq_group_join_array_ls_autoTA_gr__array_ls_autoTB_gr__auto_auto_auto_0x69a:

.. das:function:: group_join(srca: array<auto(TA)>; srcb: array<auto(TB)>; keya: auto; keyb: auto; result: auto) : array<typedecl(result(type<TA>,type<array<TB -const -&>>))>

----

.. _function-linq_group_join_to_array_iterator_ls_autoTA_gr__iterator_ls_autoTB_gr__auto_auto_auto_0x6a0:

.. das:function:: group_join_to_array(srca: iterator<auto(TA)>; srcb: iterator<auto(TB)>; keya: auto; keyb: auto; result: auto) : array<typedecl(result(type<TA>,type<array<TB -const -&>>))>

we pass TA, and sequence of TB to 'result'

:Arguments: * **srca** : iterator<auto(TA)>

            * **srcb** : iterator<auto(TB)>

            * **keya** : auto

            * **keyb** : auto

            * **result** : auto


join
^^^^

.. _function-linq_join_array_ls_autoTA_gr__array_ls_autoTB_gr__auto_auto_auto_0x66b:

.. das:function:: join(srca: array<auto(TA)>; srcb: array<auto(TB)>; keya: auto; keyb: auto; result: auto) : array<typedecl(result(type<TA>,type<TB>))>

Joins two arrays based on matching keys (inner join)

:Arguments: * **srca** : array<auto(TA)>

            * **srcb** : array<auto(TB)>

            * **keya** : auto

            * **keyb** : auto

            * **result** : auto

.. _function-linq_join_iterator_ls_autoTA_gr__iterator_ls_autoTB_gr__auto_auto_auto_0x665:

.. das:function:: join(srca: iterator<auto(TA)>; srcb: iterator<auto(TB)>; keya: auto; keyb: auto; result: auto) : iterator<typedecl(result(type<TA>,type<TB>))>

----

.. _function-linq_join_to_array_iterator_ls_autoTA_gr__iterator_ls_autoTB_gr__auto_auto_auto_0x671:

.. das:function:: join_to_array(srca: iterator<auto(TA)>; srcb: iterator<auto(TB)>; keya: auto; keyb: auto; result: auto) : array<typedecl(result(type<TA>,type<TB>))>

Joins two iterators based on matching keys (inner join) and returns an array

:Arguments: * **srca** : iterator<auto(TA)>

            * **srcb** : iterator<auto(TB)>

            * **keya** : auto

            * **keyb** : auto

            * **result** : auto

+++++++++++++
Querying data
+++++++++++++

  *  :ref:`all (src: array\<auto(TT)\>; predicate: block\<(arg:TT):bool\>) : bool <function-linq_all_array_ls_autoTT_gr__block_ls_arg_c_TT_c_bool_gr_>` 
  *  :ref:`all (var src: iterator\<auto(TT)\>; predicate: block\<(arg:TT):bool\>) : bool <function-linq_all_iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_bool_gr_>` 
  *  :ref:`any (src: array\<auto(TT)\>; predicate: block\<(arg:TT):bool\>) : bool <function-linq_any_array_ls_autoTT_gr__block_ls_arg_c_TT_c_bool_gr_>` 
  *  :ref:`any (var src: iterator\<auto(TT)\>) : bool <function-linq_any_iterator_ls_autoTT_gr_>` 
  *  :ref:`any (src: array\<auto(TT)\>) : bool <function-linq_any_array_ls_autoTT_gr_>` 
  *  :ref:`any (var src: iterator\<auto(TT)\>; predicate: block\<(arg:TT):bool\>) : bool <function-linq_any_iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_bool_gr_>` 
  *  :ref:`contains (var src: iterator\<auto(TT)\>; element: TT) : bool <function-linq_contains_iterator_ls_autoTT_gr__TT>` 
  *  :ref:`contains (src: array\<auto(TT)\>; element: TT) : bool <function-linq_contains_array_ls_autoTT_gr__TT>` 


all
^^^

.. _function-linq_all_array_ls_autoTT_gr__block_ls_arg_c_TT_c_bool_gr_:

.. das:function:: all(src: array<auto(TT)>; predicate: block<(arg:TT):bool>) : bool

Returns true if all elements in the array satisfy the predicate

:Arguments: * **src** : array<auto(TT)>

            * **predicate** : block<(arg:TT):bool>

.. _function-linq_all_iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_bool_gr_:

.. das:function:: all(src: iterator<auto(TT)>; predicate: block<(arg:TT):bool>) : bool

----


any
^^^

.. _function-linq_any_array_ls_autoTT_gr__block_ls_arg_c_TT_c_bool_gr_:

.. das:function:: any(src: array<auto(TT)>; predicate: block<(arg:TT):bool>) : bool

Returns true if any element in the array satisfies the predicate

:Arguments: * **src** : array<auto(TT)>

            * **predicate** : block<(arg:TT):bool>

.. _function-linq_any_iterator_ls_autoTT_gr_:

.. das:function:: any(src: iterator<auto(TT)>) : bool

.. _function-linq_any_array_ls_autoTT_gr_:

.. das:function:: any(src: array<auto(TT)>) : bool

.. _function-linq_any_iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_bool_gr_:

.. das:function:: any(src: iterator<auto(TT)>; predicate: block<(arg:TT):bool>) : bool

----


contains
^^^^^^^^

.. _function-linq_contains_iterator_ls_autoTT_gr__TT:

.. das:function:: contains(src: iterator<auto(TT)>; element: TT) : bool

Returns true if the element is present in the iterator

:Arguments: * **src** : iterator<auto(TT)>

            * **element** : TT

.. _function-linq_contains_array_ls_autoTT_gr__TT:

.. das:function:: contains(src: array<auto(TT)>; element: TT) : bool

----

++++++++++++++++++
Element operations
++++++++++++++++++

  *  :ref:`element_at (var src: iterator\<auto(TT)\>; index: int) : TT <function-linq_element_at_iterator_ls_autoTT_gr__int>` 
  *  :ref:`element_at (src: array\<auto(TT)\>; index: int) : TT <function-linq_element_at_array_ls_autoTT_gr__int>` 
  *  :ref:`element_at_or_default (var src: iterator\<auto(TT)\>; index: int) : TT <function-linq_element_at_or_default_iterator_ls_autoTT_gr__int>` 
  *  :ref:`element_at_or_default (src: array\<auto(TT)\>; index: int) : TT <function-linq_element_at_or_default_array_ls_autoTT_gr__int>` 
  *  :ref:`first (var src: iterator\<auto(TT)\>) : TT <function-linq_first_iterator_ls_autoTT_gr_>` 
  *  :ref:`first (src: array\<auto(TT)\>) : TT <function-linq_first_array_ls_autoTT_gr_>` 
  *  :ref:`first_or_default (src: array\<auto(TT)\>; defaultValue: TT) : TT <function-linq_first_or_default_array_ls_autoTT_gr__TT>` 
  *  :ref:`first_or_default (var src: iterator\<auto(TT)\>; defaultValue: TT) : TT <function-linq_first_or_default_iterator_ls_autoTT_gr__TT>` 
  *  :ref:`last (var src: iterator\<auto(TT)\>) : TT <function-linq_last_iterator_ls_autoTT_gr_>` 
  *  :ref:`last (src: array\<auto(TT)\>) : TT <function-linq_last_array_ls_autoTT_gr_>` 
  *  :ref:`last_or_default (src: array\<auto(TT)\>; defaultValue: TT) : TT <function-linq_last_or_default_array_ls_autoTT_gr__TT>` 
  *  :ref:`last_or_default (var src: iterator\<auto(TT)\>; defaultValue: TT) : TT <function-linq_last_or_default_iterator_ls_autoTT_gr__TT>` 
  *  :ref:`single (src: array\<auto(TT)\>) : TT <function-linq_single_array_ls_autoTT_gr_>` 
  *  :ref:`single (var src: iterator\<auto(TT)\>) : TT <function-linq_single_iterator_ls_autoTT_gr_>` 
  *  :ref:`single_or_default (var src: iterator\<auto(TT)\>; defaultValue: TT) : TT <function-linq_single_or_default_iterator_ls_autoTT_gr__TT>` 
  *  :ref:`single_or_default (src: array\<auto(TT)\>; defaultValue: TT) : TT <function-linq_single_or_default_array_ls_autoTT_gr__TT>` 


element_at
^^^^^^^^^^

.. _function-linq_element_at_iterator_ls_autoTT_gr__int:

.. das:function:: element_at(src: iterator<auto(TT)>; index: int) : TT

Returns the element at the specified index

:Arguments: * **src** : iterator<auto(TT)>

            * **index** : int

.. _function-linq_element_at_array_ls_autoTT_gr__int:

.. das:function:: element_at(src: array<auto(TT)>; index: int) : TT

----


element_at_or_default
^^^^^^^^^^^^^^^^^^^^^

.. _function-linq_element_at_or_default_iterator_ls_autoTT_gr__int:

.. das:function:: element_at_or_default(src: iterator<auto(TT)>; index: int) : TT

Returns the element at the specified index, or a default value if the index is out of range

:Arguments: * **src** : iterator<auto(TT)>

            * **index** : int

.. _function-linq_element_at_or_default_array_ls_autoTT_gr__int:

.. das:function:: element_at_or_default(src: array<auto(TT)>; index: int) : TT

----


first
^^^^^

.. _function-linq_first_iterator_ls_autoTT_gr_:

.. das:function:: first(src: iterator<auto(TT)>) : TT

Returns the first element of an iterator

:Arguments: * **src** : iterator<auto(TT)>

.. _function-linq_first_array_ls_autoTT_gr_:

.. das:function:: first(src: array<auto(TT)>) : TT

----


first_or_default
^^^^^^^^^^^^^^^^

.. _function-linq_first_or_default_array_ls_autoTT_gr__TT:

.. das:function:: first_or_default(src: array<auto(TT)>; defaultValue: TT) : TT

Returns the first element of an array, or a default value if the array is empty

:Arguments: * **src** : array<auto(TT)>

            * **defaultValue** : TT

.. _function-linq_first_or_default_iterator_ls_autoTT_gr__TT:

.. das:function:: first_or_default(src: iterator<auto(TT)>; defaultValue: TT) : TT

----


last
^^^^

.. _function-linq_last_iterator_ls_autoTT_gr_:

.. das:function:: last(src: iterator<auto(TT)>) : TT

Returns the last element of an iterator

:Arguments: * **src** : iterator<auto(TT)>

.. _function-linq_last_array_ls_autoTT_gr_:

.. das:function:: last(src: array<auto(TT)>) : TT

----


last_or_default
^^^^^^^^^^^^^^^

.. _function-linq_last_or_default_array_ls_autoTT_gr__TT:

.. das:function:: last_or_default(src: array<auto(TT)>; defaultValue: TT) : TT

Returns the last element of an array, or a default value if the array is empty

:Arguments: * **src** : array<auto(TT)>

            * **defaultValue** : TT

.. _function-linq_last_or_default_iterator_ls_autoTT_gr__TT:

.. das:function:: last_or_default(src: iterator<auto(TT)>; defaultValue: TT) : TT

----


single
^^^^^^

.. _function-linq_single_array_ls_autoTT_gr_:

.. das:function:: single(src: array<auto(TT)>) : TT

Returns the only element of an array, and throws if there is not exactly one element

:Arguments: * **src** : array<auto(TT)>

.. _function-linq_single_iterator_ls_autoTT_gr_:

.. das:function:: single(src: iterator<auto(TT)>) : TT

----


single_or_default
^^^^^^^^^^^^^^^^^

.. _function-linq_single_or_default_iterator_ls_autoTT_gr__TT:

.. das:function:: single_or_default(src: iterator<auto(TT)>; defaultValue: TT) : TT

Returns the only element of an iterator, or a default value if there is not exactly one element

:Arguments: * **src** : iterator<auto(TT)>

            * **defaultValue** : TT

.. _function-linq_single_or_default_array_ls_autoTT_gr__TT:

.. das:function:: single_or_default(src: array<auto(TT)>; defaultValue: TT) : TT

----

++++++++++++++++++++
Transform operations
++++++++++++++++++++

  *  :ref:`select (src: array\<auto(TT)\>) : array\<tuple\<int;TT\>\> <function-linq_select_array_ls_autoTT_gr_>` 
  *  :ref:`select (var src: iterator\<auto(TT)\>) : iterator\<tuple\<int;TT\>\> <function-linq_select_iterator_ls_autoTT_gr_>` 
  *  :ref:`select (var src: iterator\<auto(TT)\>; result_selector: auto) : iterator\<typedecl(result_selector(type\<TT\>))\> <function-linq_select_iterator_ls_autoTT_gr__auto_0xad4>` 
  *  :ref:`select (src: array\<auto(TT)\>; result_selector: auto) : array\<typedecl(result_selector(type\<TT\>))\> <function-linq_select_array_ls_autoTT_gr__auto_0xada>` 
  *  :ref:`select_many (var src: iterator\<auto(TT)\>; result_selector: auto) : iterator\<typedecl(result_selector(iter_type(type\<TT\>)))\> <function-linq_select_many_iterator_ls_autoTT_gr__auto_0xb40>` 
  *  :ref:`select_many (src: array\<auto(TT)\>; result_selector: auto) : array\<typedecl(result_selector(iter_type(type\<TT\>)))\> <function-linq_select_many_array_ls_autoTT_gr__auto_0xb46>` 
  *  :ref:`select_many (src: array\<auto(TT)\>; collection_selector: auto; result_selector: auto) : array\<typedecl(result_selector(iter_type(collection_selector(type\<TT\>))))\> <function-linq_select_many_array_ls_autoTT_gr__auto_auto_0xb6f>` 
  *  :ref:`select_many (var src: iterator\<auto(TT)\>; collection_selector: auto; result_selector: auto) : iterator\<typedecl(result_selector(iter_type(collection_selector(type\<TT\>))))\> <function-linq_select_many_iterator_ls_autoTT_gr__auto_auto_0xb69>` 
  *  :ref:`select_many_to_array (var src: iterator\<auto(TT)\>; collection_selector: auto; result_selector: auto) : array\<typedecl(result_selector(iter_type(collection_selector(type\<TT\>))))\> <function-linq_select_many_to_array_iterator_ls_autoTT_gr__auto_auto_0xb75>` 
  *  :ref:`select_many_to_array (var src: iterator\<auto(TT)\>; result_selector: auto) : array\<typedecl(result_selector(iter_type(type\<TT\>)))\> <function-linq_select_many_to_array_iterator_ls_autoTT_gr__auto_0xb4c>` 
  *  :ref:`select_to_array (var src: iterator\<auto(TT)\>; result_selector: auto) : array\<typedecl(result_selector(type\<TT\>))\> <function-linq_select_to_array_iterator_ls_autoTT_gr__auto_0xae0>` 
  *  :ref:`select_to_array (var src: iterator\<auto(TT)\>) : array\<tuple\<int;TT\>\> <function-linq_select_to_array_iterator_ls_autoTT_gr_>` 
  *  :ref:`zip (var a: iterator\<auto(TT)\>; var b: iterator\<auto(UU)\>) : iterator\<tuple\<TT;UU\>\> <function-linq_zip_iterator_ls_autoTT_gr__iterator_ls_autoUU_gr_>` 
  *  :ref:`zip (a: array\<auto(TT)\>; b: array\<auto(UU)\>) : array\<tuple\<TT;UU\>\> <function-linq_zip_array_ls_autoTT_gr__array_ls_autoUU_gr_>` 
  *  :ref:`zip (a: array\<auto(TT)\>; b: array\<auto(UU)\>; result_selector: block\<(l:TT;r:UU):auto\>) : array\<typedecl(result_selector(type\<TT\>,type\<UU\>))\> <function-linq_zip_array_ls_autoTT_gr__array_ls_autoUU_gr__block_ls_l_c_TT;r_c_UU_c_auto_gr_>` 
  *  :ref:`zip (var a: iterator\<auto(TT)\>; var b: iterator\<auto(UU)\>; result_selector: block\<(l:TT;r:UU):auto\>) : iterator\<typedecl(result_selector(type\<TT\>,type\<UU\>))\> <function-linq_zip_iterator_ls_autoTT_gr__iterator_ls_autoUU_gr__block_ls_l_c_TT;r_c_UU_c_auto_gr_>` 
  *  :ref:`zip_to_array (var a: iterator\<auto(TT)\>; var b: iterator\<auto(UU)\>; result_selector: block\<(l:TT;r:UU):auto\>) : array\<typedecl(result_selector(type\<TT\>,type\<UU\>))\> <function-linq_zip_to_array_iterator_ls_autoTT_gr__iterator_ls_autoUU_gr__block_ls_l_c_TT;r_c_UU_c_auto_gr_>` 
  *  :ref:`zip_to_array (var a: iterator\<auto(TT)\>; var b: iterator\<auto(UU)\>) : array\<tuple\<TT;UU\>\> <function-linq_zip_to_array_iterator_ls_autoTT_gr__iterator_ls_autoUU_gr_>` 


select
^^^^^^

.. _function-linq_select_array_ls_autoTT_gr_:

.. das:function:: select(src: array<auto(TT)>) : array<tuple<int;TT>>

Projects each element of an array into a new form

:Arguments: * **src** : array<auto(TT)>

.. _function-linq_select_iterator_ls_autoTT_gr_:

.. das:function:: select(src: iterator<auto(TT)>) : iterator<tuple<int;TT>>

.. _function-linq_select_iterator_ls_autoTT_gr__auto_0xad4:

.. das:function:: select(src: iterator<auto(TT)>; result_selector: auto) : iterator<typedecl(result_selector(type<TT>))>

.. _function-linq_select_array_ls_autoTT_gr__auto_0xada:

.. das:function:: select(src: array<auto(TT)>; result_selector: auto) : array<typedecl(result_selector(type<TT>))>

----


select_many
^^^^^^^^^^^

.. _function-linq_select_many_iterator_ls_autoTT_gr__auto_0xb40:

.. das:function:: select_many(src: iterator<auto(TT)>; result_selector: auto) : iterator<typedecl(result_selector(iter_type(type<TT>)))>

Projects each element of an iterator to an iterator and flattens the resulting iterators into one iterator

:Arguments: * **src** : iterator<auto(TT)>

            * **result_selector** : auto

.. _function-linq_select_many_array_ls_autoTT_gr__auto_0xb46:

.. das:function:: select_many(src: array<auto(TT)>; result_selector: auto) : array<typedecl(result_selector(iter_type(type<TT>)))>

.. _function-linq_select_many_array_ls_autoTT_gr__auto_auto_0xb6f:

.. das:function:: select_many(src: array<auto(TT)>; collection_selector: auto; result_selector: auto) : array<typedecl(result_selector(iter_type(collection_selector(type<TT>))))>

.. _function-linq_select_many_iterator_ls_autoTT_gr__auto_auto_0xb69:

.. das:function:: select_many(src: iterator<auto(TT)>; collection_selector: auto; result_selector: auto) : iterator<typedecl(result_selector(iter_type(collection_selector(type<TT>))))>

----


select_many_to_array
^^^^^^^^^^^^^^^^^^^^

.. _function-linq_select_many_to_array_iterator_ls_autoTT_gr__auto_auto_0xb75:

.. das:function:: select_many_to_array(src: iterator<auto(TT)>; collection_selector: auto; result_selector: auto) : array<typedecl(result_selector(iter_type(collection_selector(type<TT>))))>

Projects each element of an iterator to an iterator and flattens the resulting iterators into one array

:Arguments: * **src** : iterator<auto(TT)>

            * **collection_selector** : auto

            * **result_selector** : auto

.. _function-linq_select_many_to_array_iterator_ls_autoTT_gr__auto_0xb4c:

.. das:function:: select_many_to_array(src: iterator<auto(TT)>; result_selector: auto) : array<typedecl(result_selector(iter_type(type<TT>)))>

----


select_to_array
^^^^^^^^^^^^^^^

.. _function-linq_select_to_array_iterator_ls_autoTT_gr__auto_0xae0:

.. das:function:: select_to_array(src: iterator<auto(TT)>; result_selector: auto) : array<typedecl(result_selector(type<TT>))>

Projects each element of an iterator into a new form using a selector function and returns an array

:Arguments: * **src** : iterator<auto(TT)>

            * **result_selector** : auto

.. _function-linq_select_to_array_iterator_ls_autoTT_gr_:

.. das:function:: select_to_array(src: iterator<auto(TT)>) : array<tuple<int;TT>>

----


zip
^^^

.. _function-linq_zip_iterator_ls_autoTT_gr__iterator_ls_autoUU_gr_:

.. das:function:: zip(a: iterator<auto(TT)>; b: iterator<auto(UU)>) : iterator<tuple<TT;UU>>

Merges two iterators into an iterator of tuples

:Arguments: * **a** : iterator<auto(TT)>

            * **b** : iterator<auto(UU)>

.. _function-linq_zip_array_ls_autoTT_gr__array_ls_autoUU_gr_:

.. das:function:: zip(a: array<auto(TT)>; b: array<auto(UU)>) : array<tuple<TT;UU>>

.. _function-linq_zip_array_ls_autoTT_gr__array_ls_autoUU_gr__block_ls_l_c_TT;r_c_UU_c_auto_gr_:

.. das:function:: zip(a: array<auto(TT)>; b: array<auto(UU)>; result_selector: block<(l:TT;r:UU):auto>) : array<typedecl(result_selector(type<TT>,type<UU>))>

.. _function-linq_zip_iterator_ls_autoTT_gr__iterator_ls_autoUU_gr__block_ls_l_c_TT;r_c_UU_c_auto_gr_:

.. das:function:: zip(a: iterator<auto(TT)>; b: iterator<auto(UU)>; result_selector: block<(l:TT;r:UU):auto>) : iterator<typedecl(result_selector(type<TT>,type<UU>))>

----


zip_to_array
^^^^^^^^^^^^

.. _function-linq_zip_to_array_iterator_ls_autoTT_gr__iterator_ls_autoUU_gr__block_ls_l_c_TT;r_c_UU_c_auto_gr_:

.. das:function:: zip_to_array(a: iterator<auto(TT)>; b: iterator<auto(UU)>; result_selector: block<(l:TT;r:UU):auto>) : array<typedecl(result_selector(type<TT>,type<UU>))>

Merges two iterators into an array by applying a specified function

:Arguments: * **a** : iterator<auto(TT)>

            * **b** : iterator<auto(UU)>

            * **result_selector** : block<(l:TT;r:UU):auto>

.. _function-linq_zip_to_array_iterator_ls_autoTT_gr__iterator_ls_autoUU_gr_:

.. das:function:: zip_to_array(a: iterator<auto(TT)>; b: iterator<auto(UU)>) : array<tuple<TT;UU>>

----

+++++++++++++++++++++
Convertion operations
+++++++++++++++++++++

  *  :ref:`to_sequence (a: array\<auto(TT)\>) : iterator\<TT\> <function-linq_to_sequence_array_ls_autoTT_gr_>` 
  *  :ref:`to_sequence_move (var a: array\<auto(TT)\>) : iterator\<TT\> <function-linq_to_sequence_move_array_ls_autoTT_gr_>` 
  *  :ref:`to_table (a: array\<auto(TT)\>; key: block\<(v:TT):auto\>; elementSelector: block\<(v:TT):auto\>) : table\<typedecl(_::unique_key(type\<TT\>)), typedecl(elementSelector(type\<TT\>))\> <function-linq_to_table_array_ls_autoTT_gr__block_ls_v_c_TT_c_auto_gr__block_ls_v_c_TT_c_auto_gr_>` 
  *  :ref:`to_table (var a: iterator\<auto(TT)\>; key: block\<(v:TT):auto\>; elementSelector: block\<(v:TT):auto\>) : table\<typedecl(_::unique_key(type\<TT\>)), typedecl(elementSelector(type\<TT\>))\> <function-linq_to_table_iterator_ls_autoTT_gr__block_ls_v_c_TT_c_auto_gr__block_ls_v_c_TT_c_auto_gr_>` 

.. _function-linq_to_sequence_array_ls_autoTT_gr_:

.. das:function:: to_sequence(a: array<auto(TT)>) : iterator<TT>

Converts an array to an iterator

:Arguments: * **a** : array<auto(TT)>

.. _function-linq_to_sequence_move_array_ls_autoTT_gr_:

.. das:function:: to_sequence_move(a: array<auto(TT)>) : iterator<TT>

Converts an array to an iterator, captures input

:Arguments: * **a** : array<auto(TT)>


to_table
^^^^^^^^

.. _function-linq_to_table_array_ls_autoTT_gr__block_ls_v_c_TT_c_auto_gr__block_ls_v_c_TT_c_auto_gr_:

.. das:function:: to_table(a: array<auto(TT)>; key: block<(v:TT):auto>; elementSelector: block<(v:TT):auto>) : table<typedecl(_::unique_key(type<TT>)), typedecl(elementSelector(type<TT>))>

Converts an array to a table

:Arguments: * **a** : array<auto(TT)>

            * **key** : block<(v:TT):auto>

            * **elementSelector** : block<(v:TT):auto>

.. _function-linq_to_table_iterator_ls_autoTT_gr__block_ls_v_c_TT_c_auto_gr__block_ls_v_c_TT_c_auto_gr_:

.. das:function:: to_table(a: iterator<auto(TT)>; key: block<(v:TT):auto>; elementSelector: block<(v:TT):auto>) : table<typedecl(_::unique_key(type<TT>)), typedecl(elementSelector(type<TT>))>

----

++++++++++++++++++++
Comparators and keys
++++++++++++++++++++

  *  :ref:`less (a: tuple\<auto(TT)\>; b: tuple\<auto(TT)\>) : bool <function-linq_less_tuple_ls_autoTT_gr__tuple_ls_autoTT_gr_>` 
  *  :ref:`less (a: auto; b: auto) : bool <function-linq_less_auto_auto_0x17d>` 
  *  :ref:`less (a: tuple\<auto(TT);auto(UU);auto(VV)\>; b: tuple\<auto(TT);auto(UU);auto(VV)\>) : bool <function-linq_less_tuple_ls_autoTT;autoUU;autoVV_gr__tuple_ls_autoTT;autoUU;autoVV_gr_>` 
  *  :ref:`less (a: tuple\<auto(TT);auto(UU)\>; b: tuple\<auto(TT);auto(UU)\>) : bool <function-linq_less_tuple_ls_autoTT;autoUU_gr__tuple_ls_autoTT;autoUU_gr_>` 
  *  :ref:`less (a: tuple\<auto(TT);auto(UU);auto(VV);auto(WW)\>; b: tuple\<auto(TT);auto(UU);auto(VV);auto(WW)\>) : bool <function-linq_less_tuple_ls_autoTT;autoUU;autoVV;autoWW_gr__tuple_ls_autoTT;autoUU;autoVV;autoWW_gr_>` 
  *  :ref:`sequence_equal (var first: iterator\<auto(TT)\>; var second: iterator\<auto(TT)\>) : bool <function-linq_sequence_equal_iterator_ls_autoTT_gr__iterator_ls_autoTT_gr_>` 
  *  :ref:`sequence_equal (first: array\<auto(TT)\>; second: array\<auto(TT)\>) : bool <function-linq_sequence_equal_array_ls_autoTT_gr__array_ls_autoTT_gr_>` 
  *  :ref:`sequence_equal_by (var first: iterator\<auto(TT)\>; var second: iterator\<auto(TT)\>; key: block\<(arg:TT):auto\>) : bool <function-linq_sequence_equal_by_iterator_ls_autoTT_gr__iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_>` 
  *  :ref:`sequence_equal_by (first: array\<auto(TT)\>; second: array\<auto(TT)\>; key: block\<(arg:TT):auto\>) : bool <function-linq_sequence_equal_by_array_ls_autoTT_gr__array_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_>` 


less
^^^^

.. _function-linq_less_tuple_ls_autoTT_gr__tuple_ls_autoTT_gr_:

.. das:function:: less(a: tuple<auto(TT)>; b: tuple<auto(TT)>) : bool

Compares two tuples, returns true if first is less than second

:Arguments: * **a** : tuple<auto(TT)>

            * **b** : tuple<auto(TT)>

.. _function-linq_less_auto_auto_0x17d:

.. das:function:: less(a: auto; b: auto) : bool

.. _function-linq_less_tuple_ls_autoTT;autoUU;autoVV_gr__tuple_ls_autoTT;autoUU;autoVV_gr_:

.. das:function:: less(a: tuple<auto(TT);auto(UU);auto(VV)>; b: tuple<auto(TT);auto(UU);auto(VV)>) : bool

.. _function-linq_less_tuple_ls_autoTT;autoUU_gr__tuple_ls_autoTT;autoUU_gr_:

.. das:function:: less(a: tuple<auto(TT);auto(UU)>; b: tuple<auto(TT);auto(UU)>) : bool

.. _function-linq_less_tuple_ls_autoTT;autoUU;autoVV;autoWW_gr__tuple_ls_autoTT;autoUU;autoVV;autoWW_gr_:

.. das:function:: less(a: tuple<auto(TT);auto(UU);auto(VV);auto(WW)>; b: tuple<auto(TT);auto(UU);auto(VV);auto(WW)>) : bool

----


sequence_equal
^^^^^^^^^^^^^^

.. _function-linq_sequence_equal_iterator_ls_autoTT_gr__iterator_ls_autoTT_gr_:

.. das:function:: sequence_equal(first: iterator<auto(TT)>; second: iterator<auto(TT)>) : bool

Checks if two sequences are equal

:Arguments: * **first** : iterator<auto(TT)>

            * **second** : iterator<auto(TT)>

.. _function-linq_sequence_equal_array_ls_autoTT_gr__array_ls_autoTT_gr_:

.. das:function:: sequence_equal(first: array<auto(TT)>; second: array<auto(TT)>) : bool

----


sequence_equal_by
^^^^^^^^^^^^^^^^^

.. _function-linq_sequence_equal_by_iterator_ls_autoTT_gr__iterator_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_:

.. das:function:: sequence_equal_by(first: iterator<auto(TT)>; second: iterator<auto(TT)>; key: block<(arg:TT):auto>) : bool

Checks if two sequences are equal by key

:Arguments: * **first** : iterator<auto(TT)>

            * **second** : iterator<auto(TT)>

            * **key** : block<(arg:TT):auto>

.. _function-linq_sequence_equal_by_array_ls_autoTT_gr__array_ls_autoTT_gr__block_ls_arg_c_TT_c_auto_gr_:

.. das:function:: sequence_equal_by(first: array<auto(TT)>; second: array<auto(TT)>; key: block<(arg:TT):auto>) : bool

----


