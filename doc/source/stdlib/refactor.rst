
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

|detail/function_annotation-refactor-ExtractMethodMacro|

.. _handle-refactor-ExtractVariableFunction:

.. das:attribute:: ExtractVariableFunction

|detail/function_annotation-refactor-ExtractVariableFunction|

+++++++++++
Call macros
+++++++++++

.. _call-macro-refactor-extract_variable:

.. das:attribute:: extract_variable

|detail/function_annotation-refactor-extract_variable|

++++++++++++++++++++++
Refactoring operations
++++++++++++++++++++++

  *  :ref:`extract_method (method_name: string; blk: block\<():void\>) <function-refactor_extract_method_string_block_ls__c_void_gr_>` 

.. _function-refactor_extract_method_string_block_ls__c_void_gr_:

.. das:function:: extract_method(method_name: string; blk: block<():void>)

|detail/function-refactor-extract_method-0x19a04db32e32647f|

:Arguments: * **method_name** : string

            * **blk** : block<void>

+++++++++++++
Uncategorized
+++++++++++++

.. _function-refactor_extract_expression_string_auto:

.. das:function:: extract_expression(method_name: string; expr: auto) : auto

|detail/function-refactor-extract_expression-0x5b119e3f7dc9a25f|

:Arguments: * **method_name** : string

            * **expr** : auto

.. _function-refactor_extract_variable_nonref_string_auto:

.. das:function:: extract_variable_nonref(method_name: string; expr: auto) : auto

|detail/function-refactor-extract_variable_nonref-0x7a2d840d254f4fba|

:Arguments: * **method_name** : string

            * **expr** : auto

.. _function-refactor_extract_variable_ref_string__autoTT__eq__eq_const:

.. das:function:: extract_variable_ref(method_name: string; expr: auto(TT)& ==const) : TT&

|detail/function-refactor-extract_variable_ref-0x945f97301b2d6178|

:Arguments: * **method_name** : string

            * **expr** : auto(TT)&!


