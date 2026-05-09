.. _tutorial_integration_c_passing_arrays:

.. index::
   single: Tutorial; C Integration; Passing Arrays
   single: Tutorial; C Integration; das_array
   single: Tutorial; C Integration; das_table

=============================================
 C Integration: Passing Arrays
=============================================

This tutorial covers two ownership patterns for handing ``array<T>`` and
``table<K;V>`` values to daslang from C, plus the helper APIs that make
either path safe.

   * **Borrowed.** The C side owns the backing buffer; daslang reads
     it for the duration of one call without taking ownership and is
     forbidden from resizing it.
   * **Context-owned.** daslang allocates the backing storage on the
     context heap (using the same growth path the daslang runtime
     uses internally). C reads the result through ``das_array`` /
     ``das_table`` and releases it with ``das_array_clear`` /
     ``das_table_clear``.

The same three pieces of C API support both flavors and tables:

  * ``das_context_allocate`` / ``_reallocate`` / ``_free`` — raw
    context-heap blocks. Tables/arrays use these under the hood, but
    you can also call them directly if you need a scratch buffer with
    daslang lifetime.
  * ``das_array_init`` / ``_init_borrowed`` / ``_reserve`` /
    ``_resize`` / ``_clear`` / ``_at`` / ``_lock`` / ``_unlock`` —
    array construction and population.
  * ``das_table_init`` / ``_reserve`` / ``_clear`` / ``_find`` /
    ``_insert`` / ``_erase`` / ``_lock`` / ``_unlock`` — table
    construction and population, with a ``DAS_TYPE_*`` argument
    selecting the key type.


The daslang script
==================

.. code-block:: das

   options gen2

   [export]
   def sum_array(arr : array<int>#) : int {
       var total = 0
       for (v in arr) {
           total += v
       }
       return total
   }

   [export]
   def fill_squares(var arr : array<int>; n : int) {
       arr |> resize(n)
       for (i in range(n)) {
           arr[i] = i * i
       }
   }

   [export]
   def count_values(var counts : table<int; int>; data : array<int>#) {
       for (v in data) {
           if (key_exists(counts, v)) {
               counts[v] = counts[v] + 1
           } else {
               counts[v] = 1
           }
       }
   }


Part 1 — borrowed array
=======================

The C buffer lives on the stack. ``das_array_init_borrowed`` sets up the
``das_array`` struct so daslang sees it as already locked: ``lock = 1``,
``magic`` set to ``DAS_ARRAY_MAGIC``. Any attempt by daslang to grow,
resize, or delete the array would panic — exactly what you want when the
backing buffer doesn't belong to the runtime.

The matching daslang signature uses the **temporary array** type
``array<int>#``: a non-owning view that cannot be retained or extended.
Marking the parameter explicitly tells the type checker to reject any
``resize`` / ``push`` / ``delete`` at compile time, so the runtime panic is
a backstop rather than the only line of defense.

.. code-block:: c

   int data[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
   das_array borrowed;
   das_array_init_borrowed(&borrowed, data, 8, 8);

   vec4f args[1];
   args[0] = das_result_array(&borrowed);

   vec4f ret = das_context_eval_with_catch(ctx, fn_sum, args);
   int total = das_argument_int(ret);   // 36

No cleanup is needed — daslang never owned ``data``.


Part 2 — context-owned array
============================

Hand daslang an empty array and let the function grow it. The runtime
allocates on the context heap (the same path now exposed via
``das_context_allocate``), and ``das_array.data`` points into that heap
after the call. C reads through ``das_array_at`` and releases the block
with ``das_array_clear``.

.. code-block:: c

   das_array owned;
   das_array_init(&owned);

   vec4f args[2];
   args[0] = das_result_array(&owned);
   args[1] = das_result_int(6);

   das_context_eval_with_catch(ctx, fn_fill, args);
   // owned.size == 6, owned.data is on the context heap.

   for (uint32_t i = 0; i < owned.size; i++) {
       int v = *(int*)das_array_at(&owned, i, sizeof(int));
       // 0, 1, 4, 9, 16, 25
   }

   das_array_clear(ctx, &owned, sizeof(int));

``das_array_clear`` requires the element stride because the C side has
no type information at this layer — the runtime needs ``capacity *
stride`` to compute the heap block size to free.


Part 3 — table fill
===================

The same lifecycle applies to ``table<K;V>`` values. ``das_table_init``
zeroes the struct; daslang's ``counts[v] = ...`` triggers the runtime
to allocate the contiguous ``(values | keys | hashes)`` block.
``das_table_clear`` needs the **key base type** (``DAS_TYPE_INT``,
``DAS_TYPE_STRING``, …) and **value size** to compute the block size to
free, since key and value types are erased at the C boundary.

.. code-block:: c

   int input[10] = { 7, 3, 7, 1, 3, 7, 1, 1, 1, 3 };
   das_array data_view;
   das_array_init_borrowed(&data_view, input, 10, 10);

   das_table counts;
   das_table_init(&counts);

   vec4f args[2];
   args[0] = das_result_table(&counts);
   args[1] = das_result_array(&data_view);

   das_context_eval_with_catch(ctx, fn_count, args);

   int k = 7;
   int * pv = (int*)das_table_find(ctx, &counts, DAS_TYPE_INT, &k, sizeof(int));
   // *pv == 3

   das_table_clear(ctx, &counts, DAS_TYPE_INT, sizeof(int));

Supported table key types (matching the runtime's
``table_reserve_impl`` dispatch — every operation accepts the same set):

* **Scalars**: ``BOOL``, ``INT8`` / ``UINT8``, ``INT16`` / ``UINT16``,
  ``INT`` / ``UINT``, ``INT64`` / ``UINT64``, ``FLOAT``, ``DOUBLE``.
* **Enumerations**: ``ENUMERATION``, ``ENUMERATION8``, ``ENUMERATION16``,
  ``ENUMERATION64`` — pass a pointer to the underlying ``int{8,16,32,64}``
  value.
* **Bitfields**: ``BITFIELD``, ``BITFIELD8``, ``BITFIELD16``,
  ``BITFIELD64`` — pass a pointer to the underlying ``uint{8,16,32,64}``
  value.
* **Vectors**: ``INT2`` / ``INT3`` / ``INT4``, ``UINT2`` / ``UINT3`` /
  ``UINT4``, ``FLOAT2`` / ``FLOAT3`` / ``FLOAT4``.
* **Ranges**: ``RANGE``, ``URANGE``, ``RANGE64``, ``URANGE64``.
* **Pointers**: ``STRING`` (``char*``), ``POINTER`` (``void*``).

Other ``DAS_TYPE_*`` values raise an "unsupported table key type"
exception.


ABI safety: ``das_array`` and ``das_table`` mirror the runtime exactly
======================================================================

``das_array`` and ``das_table`` in ``daScriptC.h`` mirror the C++
``das::Array`` and ``das::Table`` byte-for-byte (including the runtime's
internal ``magic`` field at offset 16). Compile-time ``static_assert``
checks in ``daScriptC.cpp`` guard the layout — if the C++ runtime adds,
removes, or reorders a field, the build breaks until ``daScriptC.h`` is
updated to match. Do not assume offsets match across versions without
re-checking the asserts.


Build & run
===========

Build::

   cmake --build build --config Release --target integration_c_14

Run from the project root so the script path resolves correctly::

   bin/Release/integration_c_14

Expected output::

   sum_array([1..8]) = 36
   fill_squares(_, 6) -> size=6, contents=0,1,4,9,16,25
   counts[1] = 4
   counts[3] = 3
   counts[7] = 3
   counts[99] = (missing)


.. seealso::

   Full source:
   :download:`14_passing_arrays.c <../../../../tutorials/integration/c/14_passing_arrays.c>`,
   :download:`14_passing_arrays.das <../../../../tutorials/integration/c/14_passing_arrays.das>`

   Previous tutorial: :ref:`tutorial_integration_c_shared_module`

   Tutorial :ref:`tutorial_integration_c_calling_functions` — argument
   packing for primitive types

   daScriptC.h API header: ``include/daScript/daScriptC.h``
