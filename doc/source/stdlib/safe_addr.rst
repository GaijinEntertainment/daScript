
.. _stdlib_safe_addr:

===============
safe_addr macro
===============

.. include:: detail/safe_addr.rst

The safe_addr module implements safe_addr pattern, which returns temporary address of local expression.

All functions and symbols are in "safe_addr" module, use require to get access to it. ::

    require daslib/safe_addr


++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-safe_addr-SafeAddrMacro:

.. das:attribute:: SafeAddrMacro

This macro reports an error if safe_addr is attempted on the object, which is not local to the scope.
I.e. if the object can `expire` while in scope, with delete, garbage collection, or on the C++ side.

++++++++++++++++++++++
Safe temporary address
++++++++++++++++++++++

  *  :ref:`safe_addr (x:auto(T)& =const -const) : T -&?# <function-_at_safe_addr_c__c_safe_addr_&_eq_Y_ls_T_gr_.>` 
  *  :ref:`safe_addr (x:auto(T) const& =const) : T -&? const# <function-_at_safe_addr_c__c_safe_addr_C&_eq_Y_ls_T_gr_.>` 

.. _function-_at_safe_addr_c__c_safe_addr_&_eq_Y_ls_T_gr_.:

.. das:function:: safe_addr(x: auto(T)& =const)

safe_addr returns T?#

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +auto(T)&!    +
+--------+-------------+


returns temporary pointer to the given expression

.. _function-_at_safe_addr_c__c_safe_addr_C&_eq_Y_ls_T_gr_.:

.. das:function:: safe_addr(x: auto(T) const& =const)

safe_addr returns T? const#

+--------+---------------+
+argument+argument type  +
+========+===============+
+x       +auto(T) const&!+
+--------+---------------+


returns temporary pointer to the given expression


