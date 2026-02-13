
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

Replaces generic_return(expr) with a block that calls expr and returns its result, handling void, copyable, and movable return types.


