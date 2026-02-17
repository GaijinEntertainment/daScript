
.. _stdlib_bool_array:

====================
Packed boolean array
====================

.. das:module:: bool_array

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



.. _function-bool_array_finalize_BoolArray_0x16:

.. das:function:: BoolArray.finalize()

Releases the memory used by the BoolArray.


.. _function-bool_array__lb__rb__BoolArray_int_0x1a:

.. das:function:: BoolArray.[](index: int) : bool

Get the boolean value at the given index.


:Arguments: * **index** : int

.. _function-bool_array__lb__rb__eq__BoolArray_int_bool_0x23:

.. das:function:: BoolArray.[]=(index: int; value: bool)

Set the boolean value at the given index.


:Arguments: * **index** : int

            * **value** : bool

.. _function-bool_array__lb__rb_^^_eq__BoolArray_int_bool_0x30:

.. das:function:: BoolArray.[]^^=(index: int; value: bool)

Perform XOR operation on the boolean value at the given index.


:Arguments: * **index** : int

            * **value** : bool

.. _function-bool_array__lb__rb__ref__ref__eq__BoolArray_int_bool_0x3b:

.. das:function:: BoolArray.[]&&=(index: int; value: bool)

Perform AND operation on the boolean value at the given index.


:Arguments: * **index** : int

            * **value** : bool

.. _function-bool_array__lb__rb__eq__BoolArray_int_bool_0x46:

.. das:function:: BoolArray.[]||=(index: int; value: bool)

Perform OR operation on the boolean value at the given index.


:Arguments: * **index** : int

            * **value** : bool

.. _function-bool_array_BoolArray_rq_clear_BoolArray_0x50:

.. das:function:: BoolArray.clear()

Clear the BoolArray.


.. _function-bool_array_BoolArray_rq_reserve_BoolArray_int_0x57:

.. das:function:: BoolArray.reserve(capacity: int)

Reserve capacity for the BoolArray.


:Arguments: * **capacity** : int

.. _function-bool_array_BoolArray_rq_resize_BoolArray_int_0x60:

.. das:function:: BoolArray.resize(newSize: int)

Resize the BoolArray to the new size.


:Arguments: * **newSize** : int

.. _function-bool_array_BoolArray_rq_push_BoolArray_bool_0x71:

.. das:function:: BoolArray.push(value: bool)

Push a new boolean value to the end of the BoolArray.


:Arguments: * **value** : bool

.. _function-bool_array_BoolArray_rq_push_BoolArray_bool_int_0x83:

.. das:function:: BoolArray.push(value: bool; at: int)

Push a new boolean value at the given index in the BoolArray.


:Arguments: * **value** : bool

            * **at** : int

.. _function-bool_array_BoolArray_rq_pop_BoolArray_0x8c:

.. das:function:: BoolArray.pop() : bool

Pop the last boolean value from the BoolArray and return it.


.. _function-bool_array_BoolArray_rq_length_BoolArray_0x96:

.. das:function:: BoolArray.length() : int

Get the length of the BoolArray.


.. _function-bool_array_BoolArray_rq_erase_BoolArray_int_0x9b:

.. das:function:: BoolArray.erase(index: int)

Erase the boolean value at the given index from the BoolArray.


:Arguments: * **index** : int

.. _function-bool_array_BoolArray_rq_insert_BoolArray_int_bool_0xb6:

.. das:function:: BoolArray.insert(index: int; value: bool)

Insert a boolean value at the given index in the BoolArray.


:Arguments: * **index** : int

            * **value** : bool

.. _function-bool_array_BoolArray_rq_to_string_BoolArray_0xd8:

.. das:function:: BoolArray.to_string() : string

Convert the BoolArray to a string representation.


.. _function-bool_array_BoolArray_rq_data_pointer_BoolArray_0xe4:

.. das:function:: BoolArray.data_pointer() : uint?

.. warning::
  This is unsafe operation.

Get the data pointer of the BoolArray.


.. _function-bool_array_BoolArray_rq_data_pointer_BoolArray_0xea:

.. das:function:: BoolArray.data_pointer() : uint const?

.. warning::
  This is unsafe operation.

Get the data pointer of the BoolArray.



+++++++++
Iteration
+++++++++

  *  :ref:`each (self: BoolArray) : iterator\<bool\> <function-bool_array_each_BoolArray>`

.. _function-bool_array_each_BoolArray:

.. das:function:: each(self: BoolArray) : iterator<bool>

Returns an iterator over all boolean values in the BoolArray.


:Arguments: * **self** :  :ref:`BoolArray <struct-bool_array-BoolArray>`


