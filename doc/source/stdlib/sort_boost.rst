
.. _stdlib_sort_boost:

==================================
Boost package for the builtin sort
==================================

.. das:module:: sort_boost

The SORT_BOOST module provides the ``qsort`` macro that uniformly sorts
built-in arrays, dynamic arrays, and C++ handled vectors using the same
syntax. It automatically wraps handled types in ``temp_array`` as needed.

All functions and symbols are in "sort_boost" module, use require to get access to it.

.. code-block:: das

    require daslib/sort_boost



+++++++++++
Call macros
+++++++++++

.. _call-macro-sort_boost-qsort:

.. das:attribute:: qsort

Implements `qsort` macro. It's `qsort(value,block)`.
For the regular array<> or dim it's replaced with `sort(value,block)`.
For the handled types like das`vector its replaced with `sort(temp_array(value),block)`.



