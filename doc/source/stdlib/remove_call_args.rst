
.. _stdlib_remove_call_args:

================================
Call argument removal annotation
================================

The REMOVE_CALL_ARGS module provides AST transformation macros that remove
specific arguments from function calls at compile time. Used for implementing
optional parameter patterns and compile-time argument stripping.

All functions and symbols are in "remove_call_args" module, use require to get access to it. ::

    require daslib/remove_call_args

++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-remove_call_args-remove_call_args:

.. das:attribute:: remove_call_args

This macro remove all arguments by given indeces [remove_call_args(arg=(1,2,3))]


