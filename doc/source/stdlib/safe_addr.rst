
.. _stdlib_safe_addr:

===============
safe_addr macro
===============

.. include:: detail/safe_addr.rst

|module-safe_addr|

++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-safe_addr-SafeAddrMacro:

.. das:attribute:: SafeAddrMacro

|function_annotation-safe_addr-SafeAddrMacro|

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


|function-safe_addr-safe_addr|

.. _function-_at_safe_addr_c__c_safe_addr_C&_eq_Y_ls_T_gr_.:

.. das:function:: safe_addr(x: auto(T) const& =const)

safe_addr returns T? const#

+--------+---------------+
+argument+argument type  +
+========+===============+
+x       +auto(T) const&!+
+--------+---------------+


|function-safe_addr-safe_addr|


