
.. _stdlib_generic_return:

=================================
Generic return type instantiation
=================================

The GENERIC_RETURN module provides the ``[generic_return]`` annotation that
allows generic functions to automatically deduce their return type from
the body. This simplifies writing generic utility functions by eliminating
explicit return type specifications.

All functions and symbols are in "generic_return" module, use require to get access to it. ::

    require daslib/generic_return

+++++++++++
Call macros
+++++++++++

.. _call-macro-generic_return-generic_return:

.. das:attribute:: generic_return

This macro convert fake_numeric(func_id) to the comprehensive faker test::


