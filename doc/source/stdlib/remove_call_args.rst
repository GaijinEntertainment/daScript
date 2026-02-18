
.. _stdlib_remove_call_args:

================================
Call argument removal annotation
================================

.. das:module:: remove_call_args

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

This macro removes all arguments by given indices [remove_call_args(arg=(1,2,3))]



