
.. _stdlib_bool_array:

====================
Packed boolean array
====================

The BOOL_ARRAY module provides a compact boolean array implementation using
bit-packing. Each boolean value uses a single bit instead of a byte,
providing an 8x memory reduction compared to ``array<bool>``.

All functions and symbols are in "bool_array" module, use require to get access to it. ::

    require daslib/bool_array

++++++++++
Structures
++++++++++

.. _struct-bool_array-BoolArray:

.. das:attribute:: BoolArray

A dynamic array of booleans, stored as bits.


+++++++++
Iteration
+++++++++

  *  :ref:`each (self: BoolArray) : iterator\<bool\> <function-bool_array_each_BoolArray>` 

.. _function-bool_array_each_BoolArray:

.. das:function:: each(self: BoolArray) : iterator<bool>

Returns an iterator over all boolean values in the BoolArray.

:Arguments: * **self** :  :ref:`BoolArray <struct-bool_array-BoolArray>` 


