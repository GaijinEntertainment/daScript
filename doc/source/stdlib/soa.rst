
.. _stdlib_soa:

========================================
SOA (Structure of Arrays) transformation
========================================

The SOA (Structure of Arrays) module transforms array-of-structures data layouts
into structure-of-arrays layouts for better cache performance. It provides macros
that generate parallel arrays for each field of a structure, enabling SIMD-friendly
data access patterns.

All functions and symbols are in "soa" module, use require to get access to it. ::

    require daslib/soa

++++++++++
Structures
++++++++++

.. _struct-soa-SOA_INDEX:

.. das:attribute:: SOA_INDEX

Proxy type returned by the ``[]`` operator on an SOA structure.
Field access on this proxy is rewritten by `SoaCallMacro` to index into the correct column array.


++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-soa-SoaCallMacro:

.. das:attribute:: SoaCallMacro

Rewrites ``soa[index].field`` into ``soa.field[index]`` at compile time.
This is the core of the SOA access pattern — it transforms AOS-style element access
into column-wise array indexing for better cache locality.

++++++++++++++++
Structure macros
++++++++++++++++

.. _handle-soa-soa:

.. das:attribute:: soa

Generates a Structure-of-Arrays layout from a regular struct.
For a struct ``Foo`` with fields ``x : float`` and ``y : float``, generates:
  * ``Foo`SOA`` — struct where every field is ``array<FieldType>``
  * ``operator []`` returning ``SOA_INDEX`` proxy
  * ``length``, ``push``, ``emplace``, ``push_clone``, ``erase`` functions

++++++++++++++++
SOA field access
++++++++++++++++

  *  :ref:`SOA_INDEX. (src: SOA_INDEX; field: string) <function-soa__dot__SOA_INDEX_string>` 

.. _function-soa__dot__SOA_INDEX_string:

.. das:function:: SOA_INDEX.(src: SOA_INDEX; field: string)

Field access operator for SOA_INDEX; rewritten by SoaCallMacro to convert soa[index].field into soa.field[index].

:Arguments: * **src** :  :ref:`SOA_INDEX <struct-soa-SOA_INDEX>` 

            * **field** : string


