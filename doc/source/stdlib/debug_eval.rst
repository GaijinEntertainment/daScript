
.. _stdlib_debug_eval:

==========================
Debug expression evaluator
==========================

.. das:module:: debug_eval

The DEBUG_EVAL module provides runtime expression evaluation for debugging
purposes. It can evaluate daslang expressions in the context of a running
program, supporting variable inspection and interactive debugging.

All functions and symbols are in "debug_eval" module, use require to get access to it. ::

    require daslib/debug_eval



++++++++++
Structures
++++++++++

.. _struct-debug_eval-Result:

.. das:attribute:: Result

Result of evaluating a debug expression.

:Fields: * **tinfo** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` - Type information of the result.

         * **value** : float4 - Raw value storage for the result.

         * **data** : void? - Pointer to the result data, if available.

         * **error** : string - Error message, empty if evaluation succeeded.




++++++++++
Evaluation
++++++++++

  *  :ref:`debug_eval (context: table\<string, Result\>; expr: string) : Result <function-debug_eval_debug_eval_table_ls_string,_Result_gr__string>`

.. _function-debug_eval_debug_eval_table_ls_string,_Result_gr__string:

.. das:function:: debug_eval(context: table<string, Result>; expr: string) : Result

Evaluates a debug expression string with the given variable context and returns the result.


:Arguments: * **context** : table<string; :ref:`Result <struct-debug_eval-Result>`>

            * **expr** : string


