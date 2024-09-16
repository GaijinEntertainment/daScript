
.. _stdlib_safe_addr:

===============
safe_addr macro
===============

The safe_addr module implements safe_addr pattern, which returns temporary address of local expression.

All functions and symbols are in "safe_addr" module, use require to get access to it. ::

    require daslib/safe_addr


++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-safe_addr-SafeAddrMacro:

.. das:attribute:: SafeAddrMacro

|detail/function_annotation-safe_addr-SafeAddrMacro|

.. _handle-safe_addr-SharedAddrMacro:

.. das:attribute:: SharedAddrMacro

|detail/function_annotation-safe_addr-SharedAddrMacro|

++++++++++++++++++++++
Safe temporary address
++++++++++++++++++++++

  *  :ref:`safe_addr (var x: auto(T)& ==const) : T?# <function-_at_safe_addr_c__c_safe_addr_&_eq_Y_ls_T_gr__dot_>` 
  *  :ref:`safe_addr (x: auto(T) const& ==const) : T?# <function-_at_safe_addr_c__c_safe_addr_C&_eq_Y_ls_T_gr__dot_>` 
  *  :ref:`shared_addr (tab: table\<auto(KEY);auto(VAL)\>; k: KEY) : auto <function-_at_safe_addr_c__c_shared_addr_C1_ls_Y_ls_KEY_gr__dot__gr_2_ls_Y_ls_VAL_gr__dot__gr_T_CY_ls_KEY_gr_L>` 
  *  :ref:`shared_addr (val: auto(VALUE) const&) : auto <function-_at_safe_addr_c__c_shared_addr_C&Y_ls_VALUE_gr__dot_>` 

.. _function-_at_safe_addr_c__c_safe_addr_&_eq_Y_ls_T_gr__dot_:

.. das:function:: safe_addr(x: auto(T)& ==const) : T?#

returns temporary pointer to the given expression


:Arguments: * **x** : auto(T)&!

.. _function-_at_safe_addr_c__c_safe_addr_C&_eq_Y_ls_T_gr__dot_:

.. das:function:: safe_addr(x: auto(T) const& ==const) : T?#

returns temporary pointer to the given expressio


:Arguments: * **x** : auto(T)&!

.. _function-_at_safe_addr_c__c_shared_addr_C1_ls_Y_ls_KEY_gr__dot__gr_2_ls_Y_ls_VAL_gr__dot__gr_T_CY_ls_KEY_gr_L:

.. das:function:: shared_addr(tab: table<auto(KEY);auto(VAL)>; k: KEY) : auto

returns address of the given shared variable. it's safe because shared variables never go out of scope


:Arguments: * **tab** : table<auto(KEY);auto(VAL)>

            * **k** : KEY

.. _function-_at_safe_addr_c__c_shared_addr_C&Y_ls_VALUE_gr__dot_:

.. das:function:: shared_addr(val: auto(VALUE) const&) : auto

returns address of the given shared variable. it's safe because shared variables never go out of scope


:Arguments: * **val** : auto(VALUE)&

++++++++++++++++++
Temporary pointers
++++++++++++++++++

  *  :ref:`temp_ptr (x: auto(T)? const implicit ==const) : T?# <function-_at_safe_addr_c__c_temp_ptr_CI_eq_1_ls_Y_ls_T_gr__dot__gr__qm_>` 
  *  :ref:`temp_ptr (var x: auto(T)? implicit ==const) : T?# <function-_at_safe_addr_c__c_temp_ptr_I_eq_1_ls_Y_ls_T_gr__dot__gr__qm_>` 

.. _function-_at_safe_addr_c__c_temp_ptr_CI_eq_1_ls_Y_ls_T_gr__dot__gr__qm_:

.. das:function:: temp_ptr(x: auto(T)? const implicit ==const) : T?#

returns temporary pointer from a given pointer


:Arguments: * **x** : auto(T)? implicit!

.. _function-_at_safe_addr_c__c_temp_ptr_I_eq_1_ls_Y_ls_T_gr__dot__gr__qm_:

.. das:function:: temp_ptr(x: auto(T)? implicit ==const) : T?#

returns temporary pointer from a given pointer


:Arguments: * **x** : auto(T)? implicit!


