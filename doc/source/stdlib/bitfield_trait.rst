
.. _stdlib_bitfield_trait:

====================
Bitfield name traits
====================

The BITFIELD_TRAIT module implements reflection utilities for bitfield types:
converting bitfield values to and from human-readable strings, iterating
over individual set bits, and constructing bitfield values from string names.

All functions and symbols are in "bitfield_trait" module, use require to get access to it. ::

    require daslib/bitfield_trait

++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-bitfield_trait-EachBitfieldMacro:

.. das:attribute:: EachBitfieldMacro

This macro converts each(bitfield) to the following code::
    generator<string>() <|
        yield "field1"
        yield "field2"
        ...
        return false

+++++++++++++
Uncategorized
+++++++++++++

.. _function-bitfield_trait_each_auto:

.. das:function:: each(argT: auto) : auto

Same as assert, only the check will be not be repeated after the asseretion failed the first time.

:Arguments: * **argT** : auto


