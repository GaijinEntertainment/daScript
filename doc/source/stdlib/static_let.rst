
.. _stdlib_static_let:

================
static_let macro
================

The static_let module implements static_let pattern, which allows declaration of private global variables which are local to a scope.

All functions and symbols are in "static_let" module, use require to get access to it. ::

    require daslib/static_let


++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-static_let-StaticLetMacro:

.. das:attribute:: StaticLetMacro

|detail/function_annotation-static_let-StaticLetMacro|

++++++++++++++++++++++++++++
Static variable declarations
++++++++++++++++++++++++++++

  *  :ref:`static_let (blk: block\<\>) <function-_at_static_let_c__c_static_let_C_builtin_>` 
  *  :ref:`static_let_finalize (blk: block\<\>) <function-_at_static_let_c__c_static_let_finalize_C_builtin_>` 

.. _function-_at_static_let_c__c_static_let_C_builtin_:

.. das:function:: static_let(blk: block<>)

Given a scope with the variable declarations, this function will make those variables global.
Variable will be renamed under the hood, and all local access to it will be renamed as well.

:Arguments: * **blk** : block<void>

.. _function-_at_static_let_c__c_static_let_finalize_C_builtin_:

.. das:function:: static_let_finalize(blk: block<>)

This is very similar to regular static_let, but additionally the variable will be deleted on the context shutdown.

:Arguments: * **blk** : block<void>


