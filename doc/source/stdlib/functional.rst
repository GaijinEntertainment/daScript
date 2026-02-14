
.. _stdlib_functional:

==============================
Functional programming library
==============================

The FUNCTIONAL module implements lazy iterator adapters and higher-order
function utilities including ``filter``, ``map``, ``reduce``, ``fold``,
``scan``, ``flatten``, ``flat_map``, ``enumerate``, ``chain``, ``pairwise``,
``iterate``, ``islice``, ``cycle``, ``repeat``, ``sorted``, ``sum``,
``any``, ``all``, ``tap``, ``for_each``, ``find``, ``find_index``, and
``partition``.

All functions and symbols are in "functional" module, use require to get access to it. ::

    require daslib/functional

Example: ::

    require daslib/functional

        [export]
        def main() {
            var src <- [iterator for (x in range(6)); x]
            var evens <- filter(src, @(x : int) : bool { return x % 2 == 0; })
            for (v in evens) {
                print("{v} ")
            }
            print("\n")
        }
        // output:
        // 0 2 4

+++++++++++++++
Transformations
+++++++++++++++

  *  :ref:`filter (var src: iterator\<auto(TT)\>; blk: function\<(what:TT):bool\>) : auto <function-functional_filter_iterator_ls_autoTT_gr__function_ls_what_c_TT_c_bool_gr_>` 
  *  :ref:`filter (var src: iterator\<auto(TT)\>; blk: lambda\<(what:TT):bool\>) : auto <function-functional_filter_iterator_ls_autoTT_gr__lambda_ls_what_c_TT_c_bool_gr_>` 
  *  :ref:`flat_map (var src: iterator\<auto(TT)\>; blk: lambda\<(what:TT):auto(QQ)\>) : auto <function-functional_flat_map_iterator_ls_autoTT_gr__lambda_ls_what_c_TT_c_autoQQ_gr_>` 
  *  :ref:`flat_map (var src: iterator\<auto(TT)\>; blk: function\<(what:TT):auto(QQ)\>) : auto <function-functional_flat_map_iterator_ls_autoTT_gr__function_ls_what_c_TT_c_autoQQ_gr_>` 
  *  :ref:`flatten (var it: iterator\<auto(TT)\>) : auto <function-functional_flatten_iterator_ls_autoTT_gr_>` 
  *  :ref:`map (var src: iterator\<auto(TT)\>; blk: function\<(what:TT):auto(QQ)\>) : auto <function-functional_map_iterator_ls_autoTT_gr__function_ls_what_c_TT_c_autoQQ_gr_>` 
  *  :ref:`map (var src: iterator\<auto(TT)\>; blk: lambda\<(what:TT):auto(QQ)\>) : auto <function-functional_map_iterator_ls_autoTT_gr__lambda_ls_what_c_TT_c_autoQQ_gr_>` 
  *  :ref:`scan (var src: iterator\<auto(TT)\>; seed: auto(AGG); blk: function\<(acc:AGG;x:TT):AGG\>) : auto <function-functional_scan_iterator_ls_autoTT_gr__autoAGG_function_ls_acc_c_AGG;x_c_TT_c_AGG_gr__0x1cd>` 
  *  :ref:`scan (var src: iterator\<auto(TT)\>; seed: auto(AGG); blk: lambda\<(acc:AGG;x:TT):AGG\>) : auto <function-functional_scan_iterator_ls_autoTT_gr__autoAGG_lambda_ls_acc_c_AGG;x_c_TT_c_AGG_gr__0x1c8>` 
  *  :ref:`sorted (var it: iterator\<auto(TT)\>) : auto <function-functional_sorted_iterator_ls_autoTT_gr_>` 
  *  :ref:`sorted (var arr: array\<auto\>) : auto <function-functional_sorted_array_ls_auto_gr_>` 


filter
^^^^^^

.. _function-functional_filter_iterator_ls_autoTT_gr__function_ls_what_c_TT_c_bool_gr_:

.. das:function:: filter(src: iterator<auto(TT)>; blk: function<(what:TT):bool>) : auto

iterates over `src` and yields only those elements for which `blk` returns true

:Arguments: * **src** : iterator<auto(TT)>

            * **blk** : function<(what:TT):bool>

.. _function-functional_filter_iterator_ls_autoTT_gr__lambda_ls_what_c_TT_c_bool_gr_:

.. das:function:: filter(src: iterator<auto(TT)>; blk: lambda<(what:TT):bool>) : auto

----


flat_map
^^^^^^^^

.. _function-functional_flat_map_iterator_ls_autoTT_gr__lambda_ls_what_c_TT_c_autoQQ_gr_:

.. das:function:: flat_map(src: iterator<auto(TT)>; blk: lambda<(what:TT):auto(QQ)>) : auto

maps each element to an iterator, then flattens the results one level

:Arguments: * **src** : iterator<auto(TT)>

            * **blk** : lambda<(what:TT):auto(QQ)>

.. _function-functional_flat_map_iterator_ls_autoTT_gr__function_ls_what_c_TT_c_autoQQ_gr_:

.. das:function:: flat_map(src: iterator<auto(TT)>; blk: function<(what:TT):auto(QQ)>) : auto

----

.. _function-functional_flatten_iterator_ls_autoTT_gr_:

.. das:function:: flatten(it: iterator<auto(TT)>) : auto

iterates over `it`, then iterates over each element of each element of `it` and yields it

:Arguments: * **it** : iterator<auto(TT)>


map
^^^

.. _function-functional_map_iterator_ls_autoTT_gr__function_ls_what_c_TT_c_autoQQ_gr_:

.. das:function:: map(src: iterator<auto(TT)>; blk: function<(what:TT):auto(QQ)>) : auto

iterates over `src` and yields the result of `blk` for each element

:Arguments: * **src** : iterator<auto(TT)>

            * **blk** : function<(what:TT):auto(QQ)>

.. _function-functional_map_iterator_ls_autoTT_gr__lambda_ls_what_c_TT_c_autoQQ_gr_:

.. das:function:: map(src: iterator<auto(TT)>; blk: lambda<(what:TT):auto(QQ)>) : auto

----


scan
^^^^

.. _function-functional_scan_iterator_ls_autoTT_gr__autoAGG_function_ls_acc_c_AGG;x_c_TT_c_AGG_gr__0x1cd:

.. das:function:: scan(src: iterator<auto(TT)>; seed: auto(AGG); blk: function<(acc:AGG;x:TT):AGG>) : auto

yields every intermediate accumulator value, starting from `seed`

:Arguments: * **src** : iterator<auto(TT)>

            * **seed** : auto(AGG)

            * **blk** : function<(acc:AGG;x:TT):AGG>

.. _function-functional_scan_iterator_ls_autoTT_gr__autoAGG_lambda_ls_acc_c_AGG;x_c_TT_c_AGG_gr__0x1c8:

.. das:function:: scan(src: iterator<auto(TT)>; seed: auto(AGG); blk: lambda<(acc:AGG;x:TT):AGG>) : auto

----


sorted
^^^^^^

.. _function-functional_sorted_iterator_ls_autoTT_gr_:

.. das:function:: sorted(it: iterator<auto(TT)>) : auto

iterates over input and returns it sorted version

:Arguments: * **it** : iterator<auto(TT)>

.. _function-functional_sorted_array_ls_auto_gr_:

.. das:function:: sorted(arr: array<auto>) : auto

----

+++++++++++
Aggregation
+++++++++++

  *  :ref:`all (var it: iterator\<auto(TT)\>) : auto <function-functional_all_iterator_ls_autoTT_gr_>` 
  *  :ref:`all (it: auto) : auto <function-functional_all_auto_0xb5>` 
  *  :ref:`any (it: auto) : auto <function-functional_any_auto_0x9b>` 
  *  :ref:`any (var it: iterator\<auto(TT)\>) : auto <function-functional_any_iterator_ls_autoTT_gr_>` 
  *  :ref:`fold (var it: iterator\<auto(TT)\>; seed: auto(AGG); blk: lambda\<(acc:AGG;x:TT):AGG\>) : auto <function-functional_fold_iterator_ls_autoTT_gr__autoAGG_lambda_ls_acc_c_AGG;x_c_TT_c_AGG_gr__0x1a4>` 
  *  :ref:`fold (var it: iterator\<auto(TT)\>; seed: auto(AGG); blk: block\<(acc:AGG;x:TT):AGG\>) : auto <function-functional_fold_iterator_ls_autoTT_gr__autoAGG_block_ls_acc_c_AGG;x_c_TT_c_AGG_gr__0x1ae>` 
  *  :ref:`fold (var it: iterator\<auto(TT)\>; seed: auto(AGG); blk: function\<(acc:AGG;x:TT):AGG\>) : auto <function-functional_fold_iterator_ls_autoTT_gr__autoAGG_function_ls_acc_c_AGG;x_c_TT_c_AGG_gr__0x1a9>` 
  *  :ref:`reduce (var it: iterator\<auto(TT)\>; blk: function\<(left:TT;right:TT):TT\>) : auto <function-functional_reduce_iterator_ls_autoTT_gr__function_ls_left_c_TT;right_c_TT_c_TT_gr_>` 
  *  :ref:`reduce (var it: iterator\<auto(TT)\>; blk: lambda\<(left:TT;right:TT):TT\>) : auto <function-functional_reduce_iterator_ls_autoTT_gr__lambda_ls_left_c_TT;right_c_TT_c_TT_gr_>` 
  *  :ref:`reduce (var it: iterator\<auto(TT)\>; blk: block\<(left:TT;right:TT):TT\>) : auto <function-functional_reduce_iterator_ls_autoTT_gr__block_ls_left_c_TT;right_c_TT_c_TT_gr_>` 
  *  :ref:`reduce_or_default (var it: iterator\<auto(TT)\>; blk: function\<(left:TT;right:TT):TT\>; default_value: TT) : auto <function-functional_reduce_or_default_iterator_ls_autoTT_gr__function_ls_left_c_TT;right_c_TT_c_TT_gr__TT>` 
  *  :ref:`reduce_or_default (var it: iterator\<auto(TT)\>; default_value: TT; blk: block\<(left:TT;right:TT):TT\>) : auto <function-functional_reduce_or_default_iterator_ls_autoTT_gr__TT_block_ls_left_c_TT;right_c_TT_c_TT_gr_>` 
  *  :ref:`reduce_or_default (var it: iterator\<auto(TT)\>; blk: lambda\<(left:TT;right:TT):TT\>; default_value: TT) : auto <function-functional_reduce_or_default_iterator_ls_autoTT_gr__lambda_ls_left_c_TT;right_c_TT_c_TT_gr__TT>` 
  *  :ref:`sum (var it: iterator\<auto(TT)\>) : auto <function-functional_sum_iterator_ls_autoTT_gr_>` 


all
^^^

.. _function-functional_all_iterator_ls_autoTT_gr_:

.. das:function:: all(it: iterator<auto(TT)>) : auto

iterates over `it` and yields true if all elements are true

:Arguments: * **it** : iterator<auto(TT)>

.. _function-functional_all_auto_0xb5:

.. das:function:: all(it: auto) : auto

----


any
^^^

.. _function-functional_any_auto_0x9b:

.. das:function:: any(it: auto) : auto

iterates over `it` and yields true if any element is true

:Arguments: * **it** : auto

.. _function-functional_any_iterator_ls_autoTT_gr_:

.. das:function:: any(it: iterator<auto(TT)>) : auto

----


fold
^^^^

.. _function-functional_fold_iterator_ls_autoTT_gr__autoAGG_lambda_ls_acc_c_AGG;x_c_TT_c_AGG_gr__0x1a4:

.. das:function:: fold(it: iterator<auto(TT)>; seed: auto(AGG); blk: lambda<(acc:AGG;x:TT):AGG>) : auto

combines elements left-to-right starting from `seed`

:Arguments: * **it** : iterator<auto(TT)>

            * **seed** : auto(AGG)

            * **blk** : lambda<(acc:AGG;x:TT):AGG>

.. _function-functional_fold_iterator_ls_autoTT_gr__autoAGG_block_ls_acc_c_AGG;x_c_TT_c_AGG_gr__0x1ae:

.. das:function:: fold(it: iterator<auto(TT)>; seed: auto(AGG); blk: block<(acc:AGG;x:TT):AGG>) : auto

.. _function-functional_fold_iterator_ls_autoTT_gr__autoAGG_function_ls_acc_c_AGG;x_c_TT_c_AGG_gr__0x1a9:

.. das:function:: fold(it: iterator<auto(TT)>; seed: auto(AGG); blk: function<(acc:AGG;x:TT):AGG>) : auto

----


reduce
^^^^^^

.. _function-functional_reduce_iterator_ls_autoTT_gr__function_ls_left_c_TT;right_c_TT_c_TT_gr_:

.. das:function:: reduce(it: iterator<auto(TT)>; blk: function<(left:TT;right:TT):TT>) : auto

iterates over `it` and yields the reduced (combined) result of `blk` for each element
and previous reduction result

:Arguments: * **it** : iterator<auto(TT)>

            * **blk** : function<(left:TT;right:TT):TT>

.. _function-functional_reduce_iterator_ls_autoTT_gr__lambda_ls_left_c_TT;right_c_TT_c_TT_gr_:

.. das:function:: reduce(it: iterator<auto(TT)>; blk: lambda<(left:TT;right:TT):TT>) : auto

.. _function-functional_reduce_iterator_ls_autoTT_gr__block_ls_left_c_TT;right_c_TT_c_TT_gr_:

.. das:function:: reduce(it: iterator<auto(TT)>; blk: block<(left:TT;right:TT):TT>) : auto

----


reduce_or_default
^^^^^^^^^^^^^^^^^

.. _function-functional_reduce_or_default_iterator_ls_autoTT_gr__function_ls_left_c_TT;right_c_TT_c_TT_gr__TT:

.. das:function:: reduce_or_default(it: iterator<auto(TT)>; blk: function<(left:TT;right:TT):TT>; default_value: TT) : auto

like reduce, but returns `default_value` on empty input

:Arguments: * **it** : iterator<auto(TT)>

            * **blk** : function<(left:TT;right:TT):TT>

            * **default_value** : TT

.. _function-functional_reduce_or_default_iterator_ls_autoTT_gr__TT_block_ls_left_c_TT;right_c_TT_c_TT_gr_:

.. das:function:: reduce_or_default(it: iterator<auto(TT)>; default_value: TT; blk: block<(left:TT;right:TT):TT>) : auto

.. _function-functional_reduce_or_default_iterator_ls_autoTT_gr__lambda_ls_left_c_TT;right_c_TT_c_TT_gr__TT:

.. das:function:: reduce_or_default(it: iterator<auto(TT)>; blk: lambda<(left:TT;right:TT):TT>; default_value: TT) : auto

----

.. _function-functional_sum_iterator_ls_autoTT_gr_:

.. das:function:: sum(it: iterator<auto(TT)>) : auto

iterates over `it` and yields the sum of all elements
same as reduce(it, @(a,b) => a + b)

:Arguments: * **it** : iterator<auto(TT)>

++++++++++++++++
Search and split
++++++++++++++++

  *  :ref:`find (var src: iterator\<auto(TT)\>; blk: function\<(what:TT):bool\>; default_value: TT) : auto <function-functional_find_iterator_ls_autoTT_gr__function_ls_what_c_TT_c_bool_gr__TT>` 
  *  :ref:`find (var src: iterator\<auto(TT)\>; blk: lambda\<(what:TT):bool\>; default_value: TT) : auto <function-functional_find_iterator_ls_autoTT_gr__lambda_ls_what_c_TT_c_bool_gr__TT>` 
  *  :ref:`find (var src: iterator\<auto(TT)\>; default_value: TT; blk: block\<(what:TT):bool\>) : auto <function-functional_find_iterator_ls_autoTT_gr__TT_block_ls_what_c_TT_c_bool_gr_>` 
  *  :ref:`find_index (var src: iterator\<auto(TT)\>; blk: lambda\<(what:TT):bool\>) : int <function-functional_find_index_iterator_ls_autoTT_gr__lambda_ls_what_c_TT_c_bool_gr_>` 
  *  :ref:`find_index (var src: iterator\<auto(TT)\>; blk: block\<(what:TT):bool\>) : int <function-functional_find_index_iterator_ls_autoTT_gr__block_ls_what_c_TT_c_bool_gr_>` 
  *  :ref:`find_index (var src: iterator\<auto(TT)\>; blk: function\<(what:TT):bool\>) : int <function-functional_find_index_iterator_ls_autoTT_gr__function_ls_what_c_TT_c_bool_gr_>` 
  *  :ref:`partition (var src: iterator\<auto(TT)\>; blk: lambda\<(what:TT):bool\>) : tuple\<array\<TT\>;array\<TT\>\> <function-functional_partition_iterator_ls_autoTT_gr__lambda_ls_what_c_TT_c_bool_gr_>` 
  *  :ref:`partition (var src: iterator\<auto(TT)\>; blk: function\<(what:TT):bool\>) : tuple\<array\<TT\>;array\<TT\>\> <function-functional_partition_iterator_ls_autoTT_gr__function_ls_what_c_TT_c_bool_gr_>` 
  *  :ref:`partition (var src: iterator\<auto(TT)\>; blk: block\<(what:TT):bool\>) : tuple\<array\<TT\>;array\<TT\>\> <function-functional_partition_iterator_ls_autoTT_gr__block_ls_what_c_TT_c_bool_gr_>` 


find
^^^^

.. _function-functional_find_iterator_ls_autoTT_gr__function_ls_what_c_TT_c_bool_gr__TT:

.. das:function:: find(src: iterator<auto(TT)>; blk: function<(what:TT):bool>; default_value: TT) : auto

returns the first element for which `blk` returns true, or `default_value`

:Arguments: * **src** : iterator<auto(TT)>

            * **blk** : function<(what:TT):bool>

            * **default_value** : TT

.. _function-functional_find_iterator_ls_autoTT_gr__lambda_ls_what_c_TT_c_bool_gr__TT:

.. das:function:: find(src: iterator<auto(TT)>; blk: lambda<(what:TT):bool>; default_value: TT) : auto

.. _function-functional_find_iterator_ls_autoTT_gr__TT_block_ls_what_c_TT_c_bool_gr_:

.. das:function:: find(src: iterator<auto(TT)>; default_value: TT; blk: block<(what:TT):bool>) : auto

----


find_index
^^^^^^^^^^

.. _function-functional_find_index_iterator_ls_autoTT_gr__lambda_ls_what_c_TT_c_bool_gr_:

.. das:function:: find_index(src: iterator<auto(TT)>; blk: lambda<(what:TT):bool>) : int

returns the index of the first element for which `blk` returns true, or -1

:Arguments: * **src** : iterator<auto(TT)>

            * **blk** : lambda<(what:TT):bool>

.. _function-functional_find_index_iterator_ls_autoTT_gr__block_ls_what_c_TT_c_bool_gr_:

.. das:function:: find_index(src: iterator<auto(TT)>; blk: block<(what:TT):bool>) : int

.. _function-functional_find_index_iterator_ls_autoTT_gr__function_ls_what_c_TT_c_bool_gr_:

.. das:function:: find_index(src: iterator<auto(TT)>; blk: function<(what:TT):bool>) : int

----


partition
^^^^^^^^^

.. _function-functional_partition_iterator_ls_autoTT_gr__lambda_ls_what_c_TT_c_bool_gr_:

.. das:function:: partition(src: iterator<auto(TT)>; blk: lambda<(what:TT):bool>) : tuple<array<TT>;array<TT>>

splits elements into `(matching, non_matching)` arrays

:Arguments: * **src** : iterator<auto(TT)>

            * **blk** : lambda<(what:TT):bool>

.. _function-functional_partition_iterator_ls_autoTT_gr__function_ls_what_c_TT_c_bool_gr_:

.. das:function:: partition(src: iterator<auto(TT)>; blk: function<(what:TT):bool>) : tuple<array<TT>;array<TT>>

.. _function-functional_partition_iterator_ls_autoTT_gr__block_ls_what_c_TT_c_bool_gr_:

.. das:function:: partition(src: iterator<auto(TT)>; blk: block<(what:TT):bool>) : tuple<array<TT>;array<TT>>

----

+++++++++
Iteration
+++++++++

  *  :ref:`echo (x: auto; extra: string = "\n") : auto <function-functional_echo_auto_string_0x131>` 
  *  :ref:`enumerate (var src: iterator\<auto(TT)\>) : auto <function-functional_enumerate_iterator_ls_autoTT_gr_>` 
  *  :ref:`for_each (var src: iterator\<auto(TT)\>; blk: function\<(what:TT):void\>) : auto <function-functional_for_each_iterator_ls_autoTT_gr__function_ls_what_c_TT_c_void_gr_>` 
  *  :ref:`for_each (var src: iterator\<auto(TT)\>; blk: lambda\<(what:TT):void\>) : auto <function-functional_for_each_iterator_ls_autoTT_gr__lambda_ls_what_c_TT_c_void_gr_>` 
  *  :ref:`for_each (var src: iterator\<auto(TT)\>; blk: block\<(what:TT):void\>) : auto <function-functional_for_each_iterator_ls_autoTT_gr__block_ls_what_c_TT_c_void_gr_>` 
  *  :ref:`tap (var src: iterator\<auto(TT)\>; blk: function\<(what:TT):void\>) : auto <function-functional_tap_iterator_ls_autoTT_gr__function_ls_what_c_TT_c_void_gr_>` 
  *  :ref:`tap (var src: iterator\<auto(TT)\>; blk: lambda\<(what:TT):void\>) : auto <function-functional_tap_iterator_ls_autoTT_gr__lambda_ls_what_c_TT_c_void_gr_>` 

.. _function-functional_echo_auto_string_0x131:

.. das:function:: echo(x: auto; extra: string = "\n") : auto

prints `x` to the output with `extra` appended, then returns `x` unchanged.
Non-destructive — safe to use in expression chains.

:Arguments: * **x** : auto

            * **extra** : string

.. _function-functional_enumerate_iterator_ls_autoTT_gr_:

.. das:function:: enumerate(src: iterator<auto(TT)>) : auto

yields tuples of `(index, element)` for each element in `src`

:Arguments: * **src** : iterator<auto(TT)>


for_each
^^^^^^^^

.. _function-functional_for_each_iterator_ls_autoTT_gr__function_ls_what_c_TT_c_void_gr_:

.. das:function:: for_each(src: iterator<auto(TT)>; blk: function<(what:TT):void>) : auto

invokes `blk` on every element of `src`

:Arguments: * **src** : iterator<auto(TT)>

            * **blk** : function<(what:TT):void>

.. _function-functional_for_each_iterator_ls_autoTT_gr__lambda_ls_what_c_TT_c_void_gr_:

.. das:function:: for_each(src: iterator<auto(TT)>; blk: lambda<(what:TT):void>) : auto

.. _function-functional_for_each_iterator_ls_autoTT_gr__block_ls_what_c_TT_c_void_gr_:

.. das:function:: for_each(src: iterator<auto(TT)>; blk: block<(what:TT):void>) : auto

----


tap
^^^

.. _function-functional_tap_iterator_ls_autoTT_gr__function_ls_what_c_TT_c_void_gr_:

.. das:function:: tap(src: iterator<auto(TT)>; blk: function<(what:TT):void>) : auto

yields every element unchanged, calling `blk` on each as a side-effect

:Arguments: * **src** : iterator<auto(TT)>

            * **blk** : function<(what:TT):void>

.. _function-functional_tap_iterator_ls_autoTT_gr__lambda_ls_what_c_TT_c_void_gr_:

.. das:function:: tap(src: iterator<auto(TT)>; blk: lambda<(what:TT):void>) : auto

----

++++++++++
Generators
++++++++++

  *  :ref:`chain (var a: iterator\<auto(TT)\>; var b: iterator\<auto(TT)\>) : auto <function-functional_chain_iterator_ls_autoTT_gr__iterator_ls_autoTT_gr_>` 
  *  :ref:`cycle (var src: iterator\<auto(TT)\>) : auto <function-functional_cycle_iterator_ls_autoTT_gr_>` 
  *  :ref:`islice (var src: iterator\<auto(TT)\>; start: int; stop: int) : auto <function-functional_islice_iterator_ls_autoTT_gr__int_int>` 
  *  :ref:`iterate (seed: auto(TT); var blk: function\<(what:TT):TT\>) : auto <function-functional_iterate_autoTT_function_ls_what_c_TT_c_TT_gr__0x291>` 
  *  :ref:`iterate (seed: auto(TT); var blk: lambda\<(what:TT):TT\>) : auto <function-functional_iterate_autoTT_lambda_ls_what_c_TT_c_TT_gr__0x280>` 
  *  :ref:`pairwise (var src: iterator\<auto(TT)\>) : auto <function-functional_pairwise_iterator_ls_autoTT_gr_>` 
  *  :ref:`repeat (value: auto(TT); var count: int = -(1)) : auto <function-functional_repeat_autoTT_int_0x113>` 
  *  :ref:`repeat_ref (value: auto(TT); var total: int) : auto <function-functional_repeat_ref_autoTT_int_0x107>` 

.. _function-functional_chain_iterator_ls_autoTT_gr__iterator_ls_autoTT_gr_:

.. das:function:: chain(a: iterator<auto(TT)>; b: iterator<auto(TT)>) : auto

yields all elements of `a`, then all elements of `b`

:Arguments: * **a** : iterator<auto(TT)>

            * **b** : iterator<auto(TT)>

.. _function-functional_cycle_iterator_ls_autoTT_gr_:

.. das:function:: cycle(src: iterator<auto(TT)>) : auto

endlessly iterates over `src`

:Arguments: * **src** : iterator<auto(TT)>

.. _function-functional_islice_iterator_ls_autoTT_gr__int_int:

.. das:function:: islice(src: iterator<auto(TT)>; start: int; stop: int) : auto

iterates over `src` and yields only the elements in the range [start,stop)

:Arguments: * **src** : iterator<auto(TT)>

            * **start** : int

            * **stop** : int


iterate
^^^^^^^

.. _function-functional_iterate_autoTT_function_ls_what_c_TT_c_TT_gr__0x291:

.. das:function:: iterate(seed: auto(TT); blk: function<(what:TT):TT>) : auto

yields `seed`, `f(seed)`, `f(f(seed))`, ... infinitely.

:Arguments: * **seed** : auto(TT)

            * **blk** : function<(what:TT):TT>

.. _function-functional_iterate_autoTT_lambda_ls_what_c_TT_c_TT_gr__0x280:

.. das:function:: iterate(seed: auto(TT); blk: lambda<(what:TT):TT>) : auto

----

.. _function-functional_pairwise_iterator_ls_autoTT_gr_:

.. das:function:: pairwise(src: iterator<auto(TT)>) : auto

yields consecutive pairs: `(a,b)`, `(b,c)`, `(c,d)`, ...

:Arguments: * **src** : iterator<auto(TT)>

.. _function-functional_repeat_autoTT_int_0x113:

.. das:function:: repeat(value: auto(TT); count: int = -(1)) : auto

yields `value` `count` times. If `count` is negative, repeats forever.

:Arguments: * **value** : auto(TT)

            * **count** : int

.. _function-functional_repeat_ref_autoTT_int_0x107:

.. das:function:: repeat_ref(value: auto(TT); total: int) : auto

yields `value` by reference `count` times

:Arguments: * **value** : auto(TT)

            * **total** : int

++++++++++
Predicates
++++++++++

  *  :ref:`is_equal (a: auto; b: auto) : auto <function-functional_is_equal_auto_auto_0x122>` 
  *  :ref:`is_not_equal (a: auto; b: auto) : auto <function-functional_is_not_equal_auto_auto_0x127>` 
  *  :ref:`not (x: auto) : auto <function-functional_not_auto_0x12c>` 

.. _function-functional_is_equal_auto_auto_0x122:

.. das:function:: is_equal(a: auto; b: auto) : auto

yields true if `a` and `b` are equal

:Arguments: * **a** : auto

            * **b** : auto

.. _function-functional_is_not_equal_auto_auto_0x127:

.. das:function:: is_not_equal(a: auto; b: auto) : auto

yields true if `a` and `b` are not equal

:Arguments: * **a** : auto

            * **b** : auto

.. _function-functional_not_auto_0x12c:

.. das:function:: not(x: auto) : auto

yields !x

:Arguments: * **x** : auto


