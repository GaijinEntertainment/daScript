
.. _stdlib_bitfield_boost:

===========================
Bitfield operator overloads
===========================

The BITFIELD_BOOST module provides utility macros for working with bitfield types
including conversion between bitfield values and strings, and iteration over
set bits.

All functions and symbols are in "bitfield_boost" module, use require to get access to it. ::

    require daslib/bitfield_boost

+++++++++++++
Uncategorized
+++++++++++++

.. _function-bitfield_boost__auto_int:

.. das:function:: auto[](b: auto; i: int) : bool

get bitfield bit at index i

:Arguments: * **b** : auto

            * **i** : int

.. _function-bitfield_boost__eq__autoTT_int_bool:

.. das:function:: auto(TT)&[]=(b: auto(TT)&; i: int; v: bool) : auto

set bitfield bit at index i to v

:Arguments: * **b** : auto(TT)&

            * **i** : int

            * **v** : bool

.. _function-bitfield_boost__eq__autoTT_int_bool:

.. das:function:: auto(TT)&[]&&=(b: auto(TT)&; i: int; v: bool) : auto

&& assignment for bitfield bit at index i

:Arguments: * **b** : auto(TT)&

            * **i** : int

            * **v** : bool

.. _function-bitfield_boost__eq__autoTT_int_bool:

.. das:function:: auto(TT)&[]||=(b: auto(TT)&; i: int; v: bool) : auto

|| assignment for bitfield bit at index i

:Arguments: * **b** : auto(TT)&

            * **i** : int

            * **v** : bool

.. _function-bitfield_boost_^^_eq__autoTT_int_bool:

.. das:function:: auto(TT)&[]^^=(b: auto(TT)&; i: int; v: bool) : auto

toggle bitfield bit at index i if v is true

:Arguments: * **b** : auto(TT)&

            * **i** : int

            * **v** : bool

.. _function-bitfield_boost_each_auto:

.. das:function:: each(b: auto) : iterator<bool>

|detail/function-bitfield_boost-each-0x6664fece0f7bcdf4|

:Arguments: * **b** : auto


