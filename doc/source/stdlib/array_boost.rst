
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

  *  :ref:`temp_array (var arr: auto implicit ==const) : auto <function-_at_array_boost_c__c_temp_array_I_eq__dot_>` 
  *  :ref:`temp_array (arr: auto const implicit ==const) : auto <function-_at_array_boost_c__c_temp_array_CI_eq__dot_>` 
  *  :ref:`temp_array (var data: auto? ==const; lenA: int; a: auto(TT)) : array\<TT\> <function-_at_array_boost_c__c_temp_array__eq_1_ls__dot__gr__qm__Ci_CY_ls_TT_gr__dot_>` 
  *  :ref:`temp_array (data: auto? const ==const; lenA: int; a: auto(TT)) : array\<TT\> <function-_at_array_boost_c__c_temp_array_C_eq_1_ls__dot__gr__qm__Ci_CY_ls_TT_gr__dot_>` 

.. _function-_at_array_boost_c__c_temp_array_I_eq__dot_:

.. das:function:: temp_array(arr: auto implicit ==const) : auto

.. warning:: 
  This is unsafe operation.

Creates temporary array from the given object.
Important requirements are:
    * object memory is linear
    * each element follows the next one directly, with the stride equal to size of the element
    * object memory does not change within the lifetime of the returned array


:Arguments: * **arr** : auto implicit!

.. _function-_at_array_boost_c__c_temp_array_CI_eq__dot_:

.. das:function:: temp_array(arr: auto const implicit ==const) : auto

.. warning:: 
  This is unsafe operation.

|detail/function-array_boost-temp_array-0xf0cc4ff83b0f1369|

:Arguments: * **arr** : auto implicit!

.. _function-_at_array_boost_c__c_temp_array__eq_1_ls__dot__gr__qm__Ci_CY_ls_TT_gr__dot_:

.. das:function:: temp_array(data: auto? ==const; lenA: int; a: auto(TT)) : array<TT>

.. warning:: 
  This is unsafe operation.

|detail/function-array_boost-temp_array-0x13facae9301ce1a9|

:Arguments: * **data** : auto?!

            * **lenA** : int

            * **a** : auto(TT)

.. _function-_at_array_boost_c__c_temp_array_C_eq_1_ls__dot__gr__qm__Ci_CY_ls_TT_gr__dot_:

.. das:function:: temp_array(data: auto? const ==const; lenA: int; a: auto(TT)) : array<TT>

.. warning:: 
  This is unsafe operation.

|detail/function-array_boost-temp_array-0x1e6eb6af5af0ff2c|

:Arguments: * **data** : auto?!

            * **lenA** : int

            * **a** : auto(TT)

+++++++++++
Empty check
+++++++++++

  *  :ref:`empty (v: auto(VecT)) : auto <function-_at_array_boost_c__c_empty_CY_ls_VecT_gr__dot__%_ls_IsAnyArrayMacro_c_expect_any_array(v_eq_true)_gr_>` 

.. _function-_at_array_boost_c__c_empty_CY_ls_VecT_gr__dot__%_ls_IsAnyArrayMacro_c_expect_any_array(v_eq_true)_gr_:

.. das:function:: empty(v: auto(VecT)) : auto

returns true if 'v' has 0 elements. this also implies that `length(v)` is defined.


:Arguments: * **v** : auto(VecT)

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_array_boost_c__c_array_view_C_eq_1_ls_Y_ls_TT_gr__dot__gr_A_Ci_Ci_CN_ls_view_gr_0_ls_C_hh_1_ls_Y_ls_TT_gr_L_gr_A_gr_1_ls_v_gr__builtin_:

.. das:function:: array_view(bytes: array<auto(TT)> const ==const; offset: int; length: int; blk: block<(view:array<TT> const#):void>) : auto

creates a view of the array, which is a temporary array that is valid only within the block


:Arguments: * **bytes** : array<auto(TT)>!

            * **offset** : int

            * **length** : int

            * **blk** : block<(view:array<TT>#):void>

.. _function-_at_array_boost_c__c_array_view__eq_1_ls_Y_ls_TT_gr__dot__gr_A_Ci_Ci_CN_ls_view_gr_0_ls__hh_1_ls_Y_ls_TT_gr_L_gr_A_gr_1_ls_v_gr__builtin_:

.. das:function:: array_view(bytes: array<auto(TT)> ==const; offset: int; length: int; blk: block<(var view:array<TT>#):void>) : auto

creates a view of the array, which is a temporary array that is valid only within the block


:Arguments: * **bytes** : array<auto(TT)>!

            * **offset** : int

            * **length** : int

            * **blk** : block<(view:array<TT>#):void>


