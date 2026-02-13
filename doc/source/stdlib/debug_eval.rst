
.. _stdlib_debug_eval:

==========================
Debug expression evaluator
==========================

The DEBUG_EVAL module provides runtime expression evaluation for debugging
purposes. It can evaluate daScript expressions in the context of a running
program, supporting variable inspection and interactive debugging.

All functions and symbols are in "debug_eval" module, use require to get access to it. ::

    require daslib/debug_eval

++++++++++
Structures
++++++++++

.. _struct-debug_eval-Result:

.. das:attribute:: Result

|detail/structure-debug_eval-Result|

+++++++++++++
Uncategorized
+++++++++++++

.. _function-debug_eval_debug_eval_table_ls_string,_Result_gr__string:

.. das:function:: debug_eval(context: table<string, Result>; expr: string) : Result

|detail/function-debug_eval-debug_eval-0x647185423590d05f|

:Arguments: * **context** : table<string; :ref:`Result <struct-debug_eval-Result>` >

            * **expr** : string


