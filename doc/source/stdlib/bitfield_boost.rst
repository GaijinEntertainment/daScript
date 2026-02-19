
.. _stdlib_bitfield_boost:

===========================
Bitfield operator overloads
===========================

.. das:module:: bitfield_boost

The BITFIELD_BOOST module provides utility macros for working with bitfield types
including conversion between bitfield values and strings, and iteration over
set bits.

All functions and symbols are in "bitfield_boost" module, use require to get access to it.

.. code-block:: das

    require daslib/bitfield_boost



+++++++++++++++++++++++
Bitfield element access
+++++++++++++++++++++++

  *  :ref:`auto(TT)&[]&&= (var b: auto(TT)&; i: int; v: bool) : auto <function-bitfield_boost__lb__rb__ref__ref__eq__autoTT_ref__int_bool_0x25>`
  *  :ref:`auto(TT)&[]= (var b: auto(TT)&; i: int; v: bool) : auto <function-bitfield_boost__lb__rb__eq__autoTT_ref__int_bool_0x17>`
  *  :ref:`auto(TT)&[]^^= (var b: auto(TT)&; i: int; v: bool) : auto <function-bitfield_boost__lb__rb_^^_eq__autoTT_ref__int_bool_0x3d>`
  *  :ref:`auto(TT)&[]||= (var b: auto(TT)&; i: int; v: bool) : auto <function-bitfield_boost__lb__rb__eq__autoTT_ref__int_bool_0x31>`
  *  :ref:`auto[] (b: auto; i: int) : bool <function-bitfield_boost__lb__rb__auto_int_0x11>`

.. _function-bitfield_boost__lb__rb__ref__ref__eq__autoTT_ref__int_bool_0x25:

.. das:function:: auto(TT)&[]&&=(b: auto(TT)&; i: int; v: bool) : auto

&& assignment for bitfield bit at index i


:Arguments: * **b** : auto(TT)\ &

            * **i** : int

            * **v** : bool

.. _function-bitfield_boost__lb__rb__eq__autoTT_ref__int_bool_0x17:

.. das:function:: auto(TT)&[]=(b: auto(TT)&; i: int; v: bool) : auto

set bitfield bit at index i to v


:Arguments: * **b** : auto(TT)\ &

            * **i** : int

            * **v** : bool

.. _function-bitfield_boost__lb__rb_^^_eq__autoTT_ref__int_bool_0x3d:

.. das:function:: auto(TT)&[]^^=(b: auto(TT)&; i: int; v: bool) : auto

toggle bitfield bit at index i if v is true


:Arguments: * **b** : auto(TT)\ &

            * **i** : int

            * **v** : bool

.. _function-bitfield_boost__lb__rb__eq__autoTT_ref__int_bool_0x31:

.. das:function:: auto(TT)&[]||=(b: auto(TT)&; i: int; v: bool) : auto

|| assignment for bitfield bit at index i


:Arguments: * **b** : auto(TT)\ &

            * **i** : int

            * **v** : bool

.. _function-bitfield_boost__lb__rb__auto_int_0x11:

.. das:function:: auto[](b: auto; i: int) : bool

get bitfield bit at index i


:Arguments: * **b** : auto

            * **i** : int


+++++++++
Iteration
+++++++++

  *  :ref:`each_bit (b: auto) : iterator\<bool\> <function-bitfield_boost_each_bit_auto_0x49>`

.. _function-bitfield_boost_each_bit_auto_0x49:

.. das:function:: each_bit(b: auto) : iterator<bool>

Iterates over each bit of a bitfield value, yielding true or false for each bit.


:Arguments: * **b** : auto


