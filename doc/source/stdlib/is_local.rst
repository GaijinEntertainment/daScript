
.. _stdlib_is_local:

========================
is_local_xxx ast helpers
========================

The is_local module exposes collection of helper routines to establish locality of expression.

All functions and symbols are in "is_local" module, use require to get access to it. ::

    require daslib/is_local

++++++++++++
Scope checks
++++++++++++

  *  :ref:`is_local_expr (expr: ExpressionPtr) : bool <function-is_local_is_local_expr_ExpressionPtr>` 
  *  :ref:`is_local_or_global_expr (expr: ExpressionPtr) : bool <function-is_local_is_local_or_global_expr_ExpressionPtr>` 
  *  :ref:`is_scope_expr (expr: ExpressionPtr) : bool <function-is_local_is_scope_expr_ExpressionPtr>` 

.. _function-is_local_is_local_expr_ExpressionPtr:

.. das:function:: is_local_expr(expr: ExpressionPtr) : bool

Returns true if the expression is local to the current scope.

:Arguments: * **expr** :  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

.. _function-is_local_is_local_or_global_expr_ExpressionPtr:

.. das:function:: is_local_or_global_expr(expr: ExpressionPtr) : bool

Returns true if expression is local the current scope or global scope.

:Arguments: * **expr** :  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

.. _function-is_local_is_scope_expr_ExpressionPtr:

.. das:function:: is_scope_expr(expr: ExpressionPtr) : bool

Returns true if the expression is a scoped expression, i.e. eventually points to a variable.

:Arguments: * **expr** :  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+++++++++++++
Uncategorized
+++++++++++++

.. _function-is_local_is_temp_safe_ExpressionPtr:

.. das:function:: is_temp_safe(expr: ExpressionPtr) : bool

Returns true if the exression had no calls, [] or table [] operators of any kind.
This is used to check expression can be safely casted to temp type.

:Arguments: * **expr** :  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

.. _function-is_local_is_shared_expr_ExpressionPtr:

.. das:function:: is_shared_expr(expr: ExpressionPtr) : bool

Returns true if the expression is local to the current scope.

:Arguments: * **expr** :  :ref:`ExpressionPtr <alias-ExpressionPtr>` 


