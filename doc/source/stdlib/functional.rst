
.. _stdlib_functional:

==============================
Functional programming library
==============================

The functional module implements a collection of high-order functions and patters to expose functional programming patters to Daslang.

All functions and symbols are in "functional" module, use require to get access to it. ::

    require daslib/functional

+++++++++++
Map, reduce
+++++++++++

  *  :ref:`filter (var src: iterator\<auto(TT)\>; blk: lambda\<(what:TT const):bool\>) : auto <function-_at_functional_c__c_filter_1_ls_Y_ls_TT_gr__dot__gr_G_CN_ls_what_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__at_>` 
  *  :ref:`filter (var src: iterator\<auto(TT)\>; blk: function\<(what:TT const):bool\>) : auto <function-_at_functional_c__c_filter_1_ls_Y_ls_TT_gr__dot__gr_G_CN_ls_what_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__at__at_>` 
  *  :ref:`map (var src: iterator\<auto(TT)\>; blk: lambda\<(what:TT const):auto(QQ)\>) : auto <function-_at_functional_c__c_map_1_ls_Y_ls_TT_gr__dot__gr_G_CN_ls_what_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_Y_ls_QQ_gr__dot__gr__at_>` 
  *  :ref:`map (var src: iterator\<auto(TT)\>; blk: function\<(what:TT const):auto(QQ)\>) : auto <function-_at_functional_c__c_map_1_ls_Y_ls_TT_gr__dot__gr_G_CN_ls_what_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_Y_ls_QQ_gr__dot__gr__at__at_>` 
  *  :ref:`reduce (it: iterator\<auto(TT)\>; blk: lambda\<(left:TT const;right:TT const):TT const\>) : auto <function-_at_functional_c__c_reduce_C1_ls_Y_ls_TT_gr__dot__gr_G_CN_ls_left;right_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_CY_ls_TT_gr_L_gr__at_>` 
  *  :ref:`reduce (it: iterator\<auto(TT)\>; blk: function\<(left:TT const;right:TT const):TT const\>) : auto <function-_at_functional_c__c_reduce_C1_ls_Y_ls_TT_gr__dot__gr_G_CN_ls_left;right_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_CY_ls_TT_gr_L_gr__at__at_>` 
  *  :ref:`reduce (it: iterator\<auto(TT)\>; blk: block\<(left:TT const;right:TT const):TT const\>) : auto <function-_at_functional_c__c_reduce_C1_ls_Y_ls_TT_gr__dot__gr_G_CN_ls_left;right_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_CY_ls_TT_gr_L_gr__builtin_>` 
  *  :ref:`sum (it: iterator\<auto(TT)\>) : auto <function-_at_functional_c__c_sum_C1_ls_Y_ls_TT_gr__dot__gr_G>` 
  *  :ref:`any (it: auto) : auto <function-_at_functional_c__c_any_C_dot_>` 
  *  :ref:`all (it: auto) : auto <function-_at_functional_c__c_all_C_dot_>` 
  *  :ref:`cycle (var src: iterator\<auto(TT)\>) : auto <function-_at_functional_c__c_cycle_1_ls_Y_ls_TT_gr__dot__gr_G>` 
  *  :ref:`islice (var src: iterator\<auto(TT)\>; start: int; stop: int) : auto <function-_at_functional_c__c_islice_1_ls_Y_ls_TT_gr__dot__gr_G_Ci_Ci>` 
  *  :ref:`repeat_ref (value: auto(TT); var total: int) : auto <function-_at_functional_c__c_repeat_ref_CY_ls_TT_gr__dot__i>` 
  *  :ref:`repeat (value: auto(TT); var count: int = 0) : auto <function-_at_functional_c__c_repeat_CY_ls_TT_gr__dot__i>` 
  *  :ref:`not (x: auto) : auto <function-_at_functional_c__c_not_C_dot_>` 
  *  :ref:`echo (var x: auto; extra: string = "\n") : auto <function-_at_functional_c__c_echo__dot__Cs>` 
  *  :ref:`flatten (var it: iterator\<auto(TT)\>) : auto <function-_at_functional_c__c_flatten_1_ls_Y_ls_TT_gr__dot__gr_G>` 

.. _function-_at_functional_c__c_filter_1_ls_Y_ls_TT_gr__dot__gr_G_CN_ls_what_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__at_:

.. das:function:: filter(src: iterator<auto(TT)>; blk: lambda<(what:TT const):bool>) : auto

iterates over `src` and yields only those elements for which `blk` returns true


:Arguments: * **src** : iterator<auto(TT)>

            * **blk** : lambda<(what:TT):bool>

.. _function-_at_functional_c__c_filter_1_ls_Y_ls_TT_gr__dot__gr_G_CN_ls_what_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__at__at_:

.. das:function:: filter(src: iterator<auto(TT)>; blk: function<(what:TT const):bool>) : auto

|detail/function-functional-filter-0x4748a090d6e00f84|

:Arguments: * **src** : iterator<auto(TT)>

            * **blk** : function<(what:TT):bool>

.. _function-_at_functional_c__c_map_1_ls_Y_ls_TT_gr__dot__gr_G_CN_ls_what_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_Y_ls_QQ_gr__dot__gr__at_:

.. das:function:: map(src: iterator<auto(TT)>; blk: lambda<(what:TT const):auto(QQ)>) : auto

iterates over `src` and yields the result of `blk` for each element


:Arguments: * **src** : iterator<auto(TT)>

            * **blk** : lambda<(what:TT):auto(QQ)>

.. _function-_at_functional_c__c_map_1_ls_Y_ls_TT_gr__dot__gr_G_CN_ls_what_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_Y_ls_QQ_gr__dot__gr__at__at_:

.. das:function:: map(src: iterator<auto(TT)>; blk: function<(what:TT const):auto(QQ)>) : auto

|detail/function-functional-map-0x52cf056d8b677155|

:Arguments: * **src** : iterator<auto(TT)>

            * **blk** : function<(what:TT):auto(QQ)>

.. _function-_at_functional_c__c_reduce_C1_ls_Y_ls_TT_gr__dot__gr_G_CN_ls_left;right_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_CY_ls_TT_gr_L_gr__at_:

.. das:function:: reduce(it: iterator<auto(TT)>; blk: lambda<(left:TT const;right:TT const):TT const>) : auto

iterates over `it` and yields the reduced (combined) result of `blk` for each element
and previous reduction result


:Arguments: * **it** : iterator<auto(TT)>

            * **blk** : lambda<(left:TT;right:TT):TT>

.. _function-_at_functional_c__c_reduce_C1_ls_Y_ls_TT_gr__dot__gr_G_CN_ls_left;right_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_CY_ls_TT_gr_L_gr__at__at_:

.. das:function:: reduce(it: iterator<auto(TT)>; blk: function<(left:TT const;right:TT const):TT const>) : auto

|detail/function-functional-reduce-0x8b19e5b22fda862f|

:Arguments: * **it** : iterator<auto(TT)>

            * **blk** : function<(left:TT;right:TT):TT>

.. _function-_at_functional_c__c_reduce_C1_ls_Y_ls_TT_gr__dot__gr_G_CN_ls_left;right_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_CY_ls_TT_gr_L_gr__builtin_:

.. das:function:: reduce(it: iterator<auto(TT)>; blk: block<(left:TT const;right:TT const):TT const>) : auto

|detail/function-functional-reduce-0xffa23ee0e513940c|

:Arguments: * **it** : iterator<auto(TT)>

            * **blk** : block<(left:TT;right:TT):TT>

.. _function-_at_functional_c__c_sum_C1_ls_Y_ls_TT_gr__dot__gr_G:

.. das:function:: sum(it: iterator<auto(TT)>) : auto

iterates over `it` and yields the sum of all elements
same as reduce(it, @(a,b) => a + b)


:Arguments: * **it** : iterator<auto(TT)>

.. _function-_at_functional_c__c_any_C_dot_:

.. das:function:: any(it: auto) : auto

iterates over `it` and yields true if any element is true


:Arguments: * **it** : auto

.. _function-_at_functional_c__c_all_C_dot_:

.. das:function:: all(it: auto) : auto

iterates over `it` and yields true if all elements are true


:Arguments: * **it** : auto

.. _function-_at_functional_c__c_cycle_1_ls_Y_ls_TT_gr__dot__gr_G:

.. das:function:: cycle(src: iterator<auto(TT)>) : auto

endlessly iterates over `src`


:Arguments: * **src** : iterator<auto(TT)>

.. _function-_at_functional_c__c_islice_1_ls_Y_ls_TT_gr__dot__gr_G_Ci_Ci:

.. das:function:: islice(src: iterator<auto(TT)>; start: int; stop: int) : auto

iterates over `src` and yields only the elements in the range [start,stop)


:Arguments: * **src** : iterator<auto(TT)>

            * **start** : int

            * **stop** : int

.. _function-_at_functional_c__c_repeat_ref_CY_ls_TT_gr__dot__i:

.. das:function:: repeat_ref(value: auto(TT); total: int) : auto

yields `value` by reference `count` times


:Arguments: * **value** : auto(TT)

            * **total** : int

.. _function-_at_functional_c__c_repeat_CY_ls_TT_gr__dot__i:

.. das:function:: repeat(value: auto(TT); count: int = 0) : auto

yields `value` `count` times


:Arguments: * **value** : auto(TT)

            * **count** : int

.. _function-_at_functional_c__c_not_C_dot_:

.. das:function:: not(x: auto) : auto

yeilds !x


:Arguments: * **x** : auto

.. _function-_at_functional_c__c_echo__dot__Cs:

.. das:function:: echo(x: auto; extra: string = "\n") : auto

prints contents of the string to the output, with `extra` string appended


:Arguments: * **x** : auto

            * **extra** : string

.. _function-_at_functional_c__c_flatten_1_ls_Y_ls_TT_gr__dot__gr_G:

.. das:function:: flatten(it: iterator<auto(TT)>) : auto

iterates over `it`, than iterates over each element of each element of `it` and yields it


:Arguments: * **it** : iterator<auto(TT)>

+++++++
Queries
+++++++

  *  :ref:`is_equal (a: auto; b: auto) : auto <function-_at_functional_c__c_is_equal_C_dot__C_dot_>` 
  *  :ref:`is_not_equal (a: auto; b: auto) : auto <function-_at_functional_c__c_is_not_equal_C_dot__C_dot_>` 

.. _function-_at_functional_c__c_is_equal_C_dot__C_dot_:

.. das:function:: is_equal(a: auto; b: auto) : auto

yields true if `a` and `b` are equal


:Arguments: * **a** : auto

            * **b** : auto

.. _function-_at_functional_c__c_is_not_equal_C_dot__C_dot_:

.. das:function:: is_not_equal(a: auto; b: auto) : auto

yields true if `a` and `b` are not equal


:Arguments: * **a** : auto

            * **b** : auto

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_functional_c__c_sorted_1_ls__dot__gr_A:

.. das:function:: sorted(arr: array<auto>) : auto

iterates over input and returns it sorted version


:Arguments: * **arr** : array<auto>

.. _function-_at_functional_c__c_sorted_1_ls_Y_ls_TT_gr__dot__gr_G:

.. das:function:: sorted(it: iterator<auto(TT)>) : auto

iterates over input and returns it sorted version


:Arguments: * **it** : iterator<auto(TT)>


