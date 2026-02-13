
.. _stdlib_functional:

==============================
Functional programming library
==============================

The FUNCTIONAL module implements higher-order functions and itertools-style
patterns including ``filter``, ``map``, ``reduce``, ``zip``, ``flatten``,
``islice``, ``cycle``, ``repeat``, and ``sorted``.

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

+++++++++++
Map, reduce
+++++++++++

  *  :ref:`filter (var src: iterator\<auto(TT)\>; blk: lambda\<(what:TT):bool\>) : auto <function-functional_filter_iterator_ls_autoTT_gr__lambda_ls_what_c_TT_c_bool_gr_>` 
  *  :ref:`filter (var src: iterator\<auto(TT)\>; blk: function\<(what:TT):bool\>) : auto <function-functional_filter_iterator_ls_autoTT_gr__function_ls_what_c_TT_c_bool_gr_>` 
  *  :ref:`map (var src: iterator\<auto(TT)\>; blk: lambda\<(what:TT):auto(QQ)\>) : auto <function-functional_map_iterator_ls_autoTT_gr__lambda_ls_what_c_TT_c_autoQQ_gr_>` 
  *  :ref:`map (var src: iterator\<auto(TT)\>; blk: function\<(what:TT):auto(QQ)\>) : auto <function-functional_map_iterator_ls_autoTT_gr__function_ls_what_c_TT_c_autoQQ_gr_>` 
  *  :ref:`reduce (var it: iterator\<auto(TT)\>; blk: lambda\<(left:TT;right:TT):TT\>) : auto <function-functional_reduce_iterator_ls_autoTT_gr__lambda_ls_left_c_TT;right_c_TT_c_TT_gr_>` 
  *  :ref:`reduce (var it: iterator\<auto(TT)\>; blk: function\<(left:TT;right:TT):TT\>) : auto <function-functional_reduce_iterator_ls_autoTT_gr__function_ls_left_c_TT;right_c_TT_c_TT_gr_>` 
  *  :ref:`reduce (var it: iterator\<auto(TT)\>; blk: block\<(left:TT;right:TT):TT\>) : auto <function-functional_reduce_iterator_ls_autoTT_gr__block_ls_left_c_TT;right_c_TT_c_TT_gr_>` 
  *  :ref:`sum (var it: iterator\<auto(TT)\>) : auto <function-functional_sum_iterator_ls_autoTT_gr_>` 
  *  :ref:`any (it: auto) : auto <function-functional_any_auto>` 
  *  :ref:`any (var it: iterator\<auto(TT)\>) : auto <function-functional_any_iterator_ls_autoTT_gr_>` 
  *  :ref:`all (it: auto) : auto <function-functional_all_auto>` 
  *  :ref:`all (var it: iterator\<auto(TT)\>) : auto <function-functional_all_iterator_ls_autoTT_gr_>` 
  *  :ref:`cycle (var src: iterator\<auto(TT)\>) : auto <function-functional_cycle_iterator_ls_autoTT_gr_>` 
  *  :ref:`islice (var src: iterator\<auto(TT)\>; start: int; stop: int) : auto <function-functional_islice_iterator_ls_autoTT_gr__int_int>` 
  *  :ref:`repeat_ref (value: auto(TT); var total: int) : auto <function-functional_repeat_ref_autoTT_int>` 
  *  :ref:`repeat (value: auto(TT); var count: int = 0) : auto <function-functional_repeat_autoTT_int>` 
  *  :ref:`not (x: auto) : auto <function-functional_not_auto>` 
  *  :ref:`echo (var x: auto; extra: string = "\n") : auto <function-functional_echo_auto_string>` 
  *  :ref:`flatten (var it: iterator\<auto(TT)\>) : auto <function-functional_flatten_iterator_ls_autoTT_gr_>` 
  *  :ref:`sorted (var arr: array\<auto\>) : auto <function-functional_sorted_array_ls_auto_gr_>` 
  *  :ref:`sorted (var it: iterator\<auto(TT)\>) : auto <function-functional_sorted_iterator_ls_autoTT_gr_>` 

.. _function-functional_filter_iterator_ls_autoTT_gr__lambda_ls_what_c_TT_c_bool_gr_:

.. das:function:: filter(src: iterator<auto(TT)>; blk: lambda<(what:TT):bool>) : auto

iterates over `src` and yields only those elements for which `blk` returns true

:Arguments: * **src** : iterator<auto(TT)>

            * **blk** : lambda<(what:TT):bool>

.. _function-functional_filter_iterator_ls_autoTT_gr__function_ls_what_c_TT_c_bool_gr_:

.. das:function:: filter(src: iterator<auto(TT)>; blk: function<(what:TT):bool>) : auto

iterates over `src` and yields only those elements for which `blk` returns true

:Arguments: * **src** : iterator<auto(TT)>

            * **blk** : function<(what:TT):bool>

.. _function-functional_map_iterator_ls_autoTT_gr__lambda_ls_what_c_TT_c_autoQQ_gr_:

.. das:function:: map(src: iterator<auto(TT)>; blk: lambda<(what:TT):auto(QQ)>) : auto

iterates over `src` and yields the result of `blk` for each element

:Arguments: * **src** : iterator<auto(TT)>

            * **blk** : lambda<(what:TT):auto(QQ)>

.. _function-functional_map_iterator_ls_autoTT_gr__function_ls_what_c_TT_c_autoQQ_gr_:

.. das:function:: map(src: iterator<auto(TT)>; blk: function<(what:TT):auto(QQ)>) : auto

iterates over `src` and yields the result of `blk` for each element

:Arguments: * **src** : iterator<auto(TT)>

            * **blk** : function<(what:TT):auto(QQ)>

.. _function-functional_reduce_iterator_ls_autoTT_gr__lambda_ls_left_c_TT;right_c_TT_c_TT_gr_:

.. das:function:: reduce(it: iterator<auto(TT)>; blk: lambda<(left:TT;right:TT):TT>) : auto

iterates over `it` and yields the reduced (combined) result of `blk` for each element
and previous reduction result

:Arguments: * **it** : iterator<auto(TT)>

            * **blk** : lambda<(left:TT;right:TT):TT>

.. _function-functional_reduce_iterator_ls_autoTT_gr__function_ls_left_c_TT;right_c_TT_c_TT_gr_:

.. das:function:: reduce(it: iterator<auto(TT)>; blk: function<(left:TT;right:TT):TT>) : auto

iterates over `it` and yields the reduced (combined) result of `blk` for each element
and previous reduction result

:Arguments: * **it** : iterator<auto(TT)>

            * **blk** : function<(left:TT;right:TT):TT>

.. _function-functional_reduce_iterator_ls_autoTT_gr__block_ls_left_c_TT;right_c_TT_c_TT_gr_:

.. das:function:: reduce(it: iterator<auto(TT)>; blk: block<(left:TT;right:TT):TT>) : auto

iterates over `it` and yields the reduced (combined) result of `blk` for each element
and previous reduction result

:Arguments: * **it** : iterator<auto(TT)>

            * **blk** : block<(left:TT;right:TT):TT>

.. _function-functional_sum_iterator_ls_autoTT_gr_:

.. das:function:: sum(it: iterator<auto(TT)>) : auto

iterates over `it` and yields the sum of all elements
same as reduce(it, @(a,b) => a + b)

:Arguments: * **it** : iterator<auto(TT)>

.. _function-functional_any_auto:

.. das:function:: any(it: auto) : auto

iterates over `it` and yields true if any element is true

:Arguments: * **it** : auto

.. _function-functional_any_iterator_ls_autoTT_gr_:

.. das:function:: any(it: iterator<auto(TT)>) : auto

iterates over `it` and yields true if any element is true

:Arguments: * **it** : iterator<auto(TT)>

.. _function-functional_all_auto:

.. das:function:: all(it: auto) : auto

iterates over `it` and yields true if all elements are true

:Arguments: * **it** : auto

.. _function-functional_all_iterator_ls_autoTT_gr_:

.. das:function:: all(it: iterator<auto(TT)>) : auto

iterates over `it` and yields true if all elements are true

:Arguments: * **it** : iterator<auto(TT)>

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

.. _function-functional_repeat_ref_autoTT_int:

.. das:function:: repeat_ref(value: auto(TT); total: int) : auto

yields `value` by reference `count` times

:Arguments: * **value** : auto(TT)

            * **total** : int

.. _function-functional_repeat_autoTT_int:

.. das:function:: repeat(value: auto(TT); count: int = 0) : auto

yields `value` `count` times

:Arguments: * **value** : auto(TT)

            * **count** : int

.. _function-functional_not_auto:

.. das:function:: not(x: auto) : auto

yeilds !x

:Arguments: * **x** : auto

.. _function-functional_echo_auto_string:

.. das:function:: echo(x: auto; extra: string = "\n") : auto

prints contents of the string to the output, with `extra` string appended

:Arguments: * **x** : auto

            * **extra** : string

.. _function-functional_flatten_iterator_ls_autoTT_gr_:

.. das:function:: flatten(it: iterator<auto(TT)>) : auto

iterates over `it`, than iterates over each element of each element of `it` and yields it

:Arguments: * **it** : iterator<auto(TT)>

.. _function-functional_sorted_array_ls_auto_gr_:

.. das:function:: sorted(arr: array<auto>) : auto

iterates over input and returns it sorted version

:Arguments: * **arr** : array<auto>

.. _function-functional_sorted_iterator_ls_autoTT_gr_:

.. das:function:: sorted(it: iterator<auto(TT)>) : auto

iterates over input and returns it sorted version

:Arguments: * **it** : iterator<auto(TT)>

+++++++
Queries
+++++++

  *  :ref:`is_equal (a: auto; b: auto) : auto <function-functional_is_equal_auto_auto>` 
  *  :ref:`is_not_equal (a: auto; b: auto) : auto <function-functional_is_not_equal_auto_auto>` 

.. _function-functional_is_equal_auto_auto:

.. das:function:: is_equal(a: auto; b: auto) : auto

yields true if `a` and `b` are equal

:Arguments: * **a** : auto

            * **b** : auto

.. _function-functional_is_not_equal_auto_auto:

.. das:function:: is_not_equal(a: auto; b: auto) : auto

yields true if `a` and `b` are not equal

:Arguments: * **a** : auto

            * **b** : auto


