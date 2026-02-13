
.. _stdlib_safe_addr:

===============
safe_addr macro
===============

The SAFE_ADDR module provides compile-time checked pointer operations.
``safe_addr`` returns a temporary pointer to a variable only if the compiler
can verify the pointer will not outlive its target. This prevents dangling
pointer bugs without runtime overhead.

All functions and symbols are in "safe_addr" module, use require to get access to it. ::

    require daslib/safe_addr

++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-safe_addr-SafeAddrMacro:

.. das:attribute:: SafeAddrMacro

This macro reports an error if safe_addr is attempted on the object, which is not local to the scope.
I.e. if the object can `expire` while in scope, with delete, garbage collection, or on the C++ side.

.. _handle-safe_addr-SharedAddrMacro:

.. das:attribute:: SharedAddrMacro

This macro reports an error if shared_addr is attempted on anything other that shared global variables.
I.e. only global variables are safe to use with shared_addr.

.. _handle-safe_addr-TempValueMacro:

.. das:attribute:: TempValueMacro

This macro reports an error if temp_value is attempted outside of function arguments.

++++++++++++++++++++++
Safe temporary address
++++++++++++++++++++++

  *  :ref:`safe_addr (var x: auto(T)& ==const) : T?# <function-safe_addr_safe_addr__autoT__eq__eq_const>` 
  *  :ref:`safe_addr (x: auto(T) const& ==const) : T?# <function-safe_addr_safe_addr_autoT_const__eq__eq_const>` 
  *  :ref:`shared_addr (tab: table\<auto(KEY), auto(VAL)\>; k: KEY) : auto <function-safe_addr_shared_addr_table_ls_autoKEY,_autoVAL_gr__KEY>` 
  *  :ref:`shared_addr (val: auto(VALUE)) : auto <function-safe_addr_shared_addr_autoVALUE>` 

.. _function-safe_addr_safe_addr__autoT__eq__eq_const:

.. das:function:: safe_addr(x: auto(T)& ==const) : T?#

returns temporary pointer to the given expression

:Arguments: * **x** : auto(T)&!

.. _function-safe_addr_safe_addr_autoT_const__eq__eq_const:

.. das:function:: safe_addr(x: auto(T) const& ==const) : T?#

returns temporary pointer to the given expressio

:Arguments: * **x** : auto(T)&!

.. _function-safe_addr_shared_addr_table_ls_autoKEY,_autoVAL_gr__KEY:

.. das:function:: shared_addr(tab: table<auto(KEY), auto(VAL)>; k: KEY) : auto

returns address of the given shared variable. it's safe because shared variables never go out of scope

:Arguments: * **tab** : table<auto(KEY);auto(VAL)>

            * **k** : KEY

.. _function-safe_addr_shared_addr_autoVALUE:

.. das:function:: shared_addr(val: auto(VALUE)) : auto

returns address of the given shared variable. it's safe because shared variables never go out of scope

:Arguments: * **val** : auto(VALUE)&

++++++++++++++++++
Temporary pointers
++++++++++++++++++

  *  :ref:`temp_ptr (x: auto(T)? const implicit ==const) : T?# <function-safe_addr_temp_ptr_autoT_q__const_implicit__eq__eq_const>` 
  *  :ref:`temp_ptr (var x: auto(T)? implicit ==const) : T?# <function-safe_addr_temp_ptr__autoT_q__implicit__eq__eq_const>` 

.. _function-safe_addr_temp_ptr_autoT_q__const_implicit__eq__eq_const:

.. das:function:: temp_ptr(x: auto(T)? const implicit ==const) : T?#

returns temporary pointer from a given pointer

:Arguments: * **x** : auto(T)? implicit!

.. _function-safe_addr_temp_ptr__autoT_q__implicit__eq__eq_const:

.. das:function:: temp_ptr(x: auto(T)? implicit ==const) : T?#

returns temporary pointer from a given pointer

:Arguments: * **x** : auto(T)? implicit!

++++++++++++++++
Temporary values
++++++++++++++++

  *  :ref:`temp_value (x: auto(T) const& ==const) : T const&# <function-safe_addr_temp_value_autoT_const__eq__eq_const>` 
  *  :ref:`temp_value (var x: auto(T)& ==const) : T&# <function-safe_addr_temp_value__autoT__eq__eq_const>` 

.. _function-safe_addr_temp_value_autoT_const__eq__eq_const:

.. das:function:: temp_value(x: auto(T) const& ==const) : T const&#

returns temporary pointer to the given expression

:Arguments: * **x** : auto(T)&!

.. _function-safe_addr_temp_value__autoT__eq__eq_const:

.. das:function:: temp_value(x: auto(T)& ==const) : T&#

returns temporary pointer to the given expression

:Arguments: * **x** : auto(T)&!


