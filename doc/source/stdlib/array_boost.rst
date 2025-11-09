
.. _stdlib_array_boost:

====================================
Boost package for array manipulation
====================================

The array_boost module implements collection of array manipulation routines.

All functions and symbols are in "array_boost" module, use require to get access to it. ::

    require daslib/array_boost

++++++++++++++++
Temporary arrays
++++++++++++++++

  *  :ref:`temp_array (var arr: auto implicit ==const) : auto <function-array_boost_temp_array__auto_implicit__eq__eq_const>` 
  *  :ref:`temp_array (arr: auto const implicit ==const) : auto <function-array_boost_temp_array_auto_const_implicit__eq__eq_const>` 
  *  :ref:`temp_array (var data: auto? ==const; lenA: int; a: auto(TT)) : array\<TT\> <function-array_boost_temp_array__auto_q___eq__eq_const_int_autoTT>` 
  *  :ref:`temp_array (data: auto? ==const; lenA: int; a: auto(TT)) : array\<TT\> <function-array_boost_temp_array_auto_q___eq__eq_const_int_autoTT>` 

.. _function-array_boost_temp_array__auto_implicit__eq__eq_const:

.. das:function:: temp_array(arr: auto implicit ==const) : auto

.. warning:: 
  This is unsafe operation.

Creates temporary array from the given object.
Important requirements are:
    * object memory is linear
    * each element follows the next one directly, with the stride equal to size of the element
    * object memory does not change within the lifetime of the returned array

:Arguments: * **arr** : auto implicit!

.. _function-array_boost_temp_array_auto_const_implicit__eq__eq_const:

.. das:function:: temp_array(arr: auto const implicit ==const) : auto

.. warning:: 
  This is unsafe operation.

|detail/function-array_boost-temp_array-0xf0cc4ff83b0f1369|

:Arguments: * **arr** : auto implicit!

.. _function-array_boost_temp_array__auto_q___eq__eq_const_int_autoTT:

.. das:function:: temp_array(data: auto? ==const; lenA: int; a: auto(TT)) : array<TT>

.. warning:: 
  This is unsafe operation.

|detail/function-array_boost-temp_array-0xf367cd2b42cc8761|

:Arguments: * **data** : auto?!

            * **lenA** : int

            * **a** : auto(TT)

.. _function-array_boost_temp_array_auto_q___eq__eq_const_int_autoTT:

.. das:function:: temp_array(data: auto? ==const; lenA: int; a: auto(TT)) : array<TT>

.. warning:: 
  This is unsafe operation.

|detail/function-array_boost-temp_array-0x13facae9301ce1a9|

:Arguments: * **data** : auto?!

            * **lenA** : int

            * **a** : auto(TT)

+++++++++++
Empty check
+++++++++++

  *  :ref:`empty (v: auto(VecT)) : auto <function-array_boost_empty_autoVecT>` 

.. _function-array_boost_empty_autoVecT:

.. das:function:: empty(v: auto(VecT)) : auto

returns true if 'v' has 0 elements. this also implies that `length(v)` is defined.

:Arguments: * **v** : auto(VecT)

++++++++++++++
Sub-array view
++++++++++++++

  *  :ref:`array_view (bytes: array\<auto(TT)\>; offset: int; length: int; blk: block\<(view:array\<TT\>#):void\>) : auto <function-array_boost_array_view_array_ls_autoTT_gr__int_int_block_ls_view_c_array_ls_TT_gr__hh__c_void_gr_>` 
  *  :ref:`array_view (var bytes: array\<auto(TT)\>; offset: int; length: int; blk: block\<(var view:array\<TT\>#):void\>) : auto <function-array_boost_array_view__array_ls_autoTT_gr__int_int_block_ls_var_view_c_array_ls_TT_gr__hh__c_void_gr_>` 

.. _function-array_boost_array_view_array_ls_autoTT_gr__int_int_block_ls_view_c_array_ls_TT_gr__hh__c_void_gr_:

.. das:function:: array_view(bytes: array<auto(TT)>; offset: int; length: int; blk: block<(view:array<TT>#):void>) : auto

creates a view of the array, which is a temporary array that is valid only within the block

:Arguments: * **bytes** : array<auto(TT)>!

            * **offset** : int

            * **length** : int

            * **blk** : block<(view:array<TT>#):void>

.. _function-array_boost_array_view__array_ls_autoTT_gr__int_int_block_ls_var_view_c_array_ls_TT_gr__hh__c_void_gr_:

.. das:function:: array_view(bytes: array<auto(TT)>; offset: int; length: int; blk: block<(var view:array<TT>#):void>) : auto

creates a view of the array, which is a temporary array that is valid only within the block

:Arguments: * **bytes** : array<auto(TT)>!

            * **offset** : int

            * **length** : int

            * **blk** : block<(view:array<TT>#):void>


