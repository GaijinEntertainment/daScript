
.. _stdlib_refactor:

===========================
Automated refactoring tools
===========================

The REFACTOR module implements automated code refactoring transformations.
It provides tools for renaming symbols, extracting functions, and other
structural code changes that preserve program semantics.

All functions and symbols are in "refactor" module, use require to get access to it. ::

    require daslib/refactor

++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-refactor-ExtractMethodMacro:

.. das:attribute:: ExtractMethodMacro

Function annotation implementing extract-method refactoring.

.. _handle-refactor-ExtractVariableFunction:

.. das:attribute:: ExtractVariableFunction

Function annotation for extract-variable target functions.

+++++++++++
Call macros
+++++++++++

.. _call-macro-refactor-extract_variable:

.. das:attribute:: extract_variable

Call macro implementing extract-variable refactoring.

++++++++++++++++++++++
Refactoring operations
++++++++++++++++++++++

  *  :ref:`extract_method (method_name: string; blk: block\<():void\>) <function-refactor_extract_method_string_block_ls__c_void_gr_>` 
  *  :ref:`extract_expression (method_name: string; expr: auto) : auto <function-refactor_extract_expression_string_auto_0x1f>` 
  *  :ref:`extract_variable_nonref (method_name: string; expr: auto) : auto <function-refactor_extract_variable_nonref_string_auto_0x8a>` 
  *  :ref:`extract_variable_ref (method_name: string; var expr: auto(TT)& ==const) : TT& <function-refactor_extract_variable_ref_string__autoTT__eq__eq_const_0x90>` 

.. _function-refactor_extract_method_string_block_ls__c_void_gr_:

.. das:function:: extract_method(method_name: string; blk: block<():void>)

Marks a block of code for method extraction refactoring.

:Arguments: * **method_name** : string

            * **blk** : block<void>

.. _function-refactor_extract_expression_string_auto_0x1f:

.. das:function:: extract_expression(method_name: string; expr: auto) : auto

Marks an expression for expression extraction refactoring.

:Arguments: * **method_name** : string

            * **expr** : auto

.. _function-refactor_extract_variable_nonref_string_auto_0x8a:

.. das:function:: extract_variable_nonref(method_name: string; expr: auto) : auto

Marks an expression for variable extraction by value.

:Arguments: * **method_name** : string

            * **expr** : auto

.. _function-refactor_extract_variable_ref_string__autoTT__eq__eq_const_0x90:

.. das:function:: extract_variable_ref(method_name: string; expr: auto(TT)& ==const) : TT&

Marks an expression for variable extraction by reference.

:Arguments: * **method_name** : string

            * **expr** : auto(TT)&!


