
.. _stdlib_bitfield_trait:

====================
Bitfield name traits
====================

.. das:module:: bitfield_trait

The BITFIELD_TRAIT module implements reflection utilities for bitfield types:
converting bitfield values to and from human-readable strings, iterating
over individual set bits, and constructing bitfield values from string names.

All functions and symbols are in "bitfield_trait" module, use require to get access to it.

.. code-block:: das

    require daslib/bitfield_trait



++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-bitfield_trait-EachBitfieldMacro:

.. das:attribute:: EachBitfieldMacro

This macro converts each(bitfield) to the following code::
    generator<string>() <|
        yield field1
        yield field2
        ...
        return false


.. _handle-bitfield_trait-EachBitNameBitfieldMacro:

.. das:attribute:: EachBitNameBitfieldMacro

This macro converts each(bitfield) to the following code::
    generator<string>() <|
        yield "field1"
        yield "field2"
        ...
        return false



+++++++++
Iteration
+++++++++

  *  :ref:`each (argT: auto) : auto <function-bitfield_trait_each_auto_0x16>`
  *  :ref:`each_bit_name (argT: auto) : auto <function-bitfield_trait_each_bit_name_auto_0x47>`

.. _function-bitfield_trait_each_auto_0x16:

.. das:function:: each(argT: auto) : auto

Iterates over the names of a bitfield type, yielding each bit as a bitfield value (1ul << bitIndex).


:Arguments: * **argT** : auto

.. _function-bitfield_trait_each_bit_name_auto_0x47:

.. das:function:: each_bit_name(argT: auto) : auto

Iterates over the names of a bitfield type, yielding each bit name as a string.


:Arguments: * **argT** : auto


