
.. _stdlib_if_not_null:

=================
if_not_null macro
=================

The IF_NOT_NULL module provides a null-safe call macro. The expression
``ptr |> if_not_null <| call(args)`` expands to a null check followed by
a dereferenced call: ``if (ptr != null) { call(*ptr, args) }``.

All functions and symbols are in "if_not_null" module, use require to get access to it. ::

    require daslib/if_not_null

+++++++++++
Call macros
+++++++++++

.. _call-macro-if_not_null-if_not_null:

.. das:attribute:: if_not_null

This macro transforms::

    ptr |> if_not_null <| call(...)

to::

    var _ptr_var = ptr
    if _ptr_var
        call(*_ptr_var,...)



