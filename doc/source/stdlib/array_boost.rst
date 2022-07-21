
.. _stdlib_array_boost:

====================================
Boost package for array manipulation
====================================

.. include:: detail/array_boost.rst

The array_boost module implements collection of array manipulation routines.

All functions and symbols are in "array_boost" module, use require to get access to it. ::

    require daslib/array_boost


++++++++++++++++
Temporary arrays
++++++++++++++++

  *  :ref:`temp_array (arr:auto implicit =const -const) : auto <function-_at_array_boost_c__c_temp_array_I_eq_.>` 
  *  :ref:`temp_array (arr:auto const implicit =const) : auto <function-_at_array_boost_c__c_temp_array_CI_eq_.>` 

.. _function-_at_array_boost_c__c_temp_array_I_eq_.:

.. das:function:: temp_array(arr: auto implicit =const)

temp_array returns auto

.. warning:: 
  This is unsafe operation.

+--------+--------------+
+argument+argument type +
+========+==============+
+arr     +auto implicit!+
+--------+--------------+


Creates temporary array from the given object.
Important requirements are:
    * object memory is linear
    * each element follows the next one directly, with the stride equal to size of the element
    * object memory does not change within the lifetime of the returned array

.. _function-_at_array_boost_c__c_temp_array_CI_eq_.:

.. das:function:: temp_array(arr: auto const implicit =const)

temp_array returns auto

.. warning:: 
  This is unsafe operation.

+--------+--------------------+
+argument+argument type       +
+========+====================+
+arr     +auto const implicit!+
+--------+--------------------+


Creates temporary array from the given object.
Important requirements are:
    * object memory is linear
    * each element follows the next one directly, with the stride equal to size of the element
    * object memory does not change within the lifetime of the returned array

+++++++++++
Empty check
+++++++++++

  *  :ref:`empty (v:auto(VecT) const) : auto <function-_at_array_boost_c__c_empty_CY_ls_VecT_gr_._%_ls_IsAnyArrayMacro_c_expect_any_array(v_eq_true)_gr_>` 

.. _function-_at_array_boost_c__c_empty_CY_ls_VecT_gr_._%_ls_IsAnyArrayMacro_c_expect_any_array(v_eq_true)_gr_:

.. das:function:: empty(v: auto(VecT) const)

empty returns auto

+--------+----------------+
+argument+argument type   +
+========+================+
+v       +auto(VecT) const+
+--------+----------------+


returns true if 'v' has 0 elements. this also implies that `length(v)` is defined.


