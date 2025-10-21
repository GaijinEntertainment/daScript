
.. _stdlib_is_local:

========================
is_local_xxx ast helpers
========================

.. include:: detail/is_local.rst

The is_local module exposes collection of helper routines to establish locality of expression.

All functions and symbols are in "is_local" module, use require to get access to it. ::

    require daslib/is_local


++++++++++++
Scope checks
++++++++++++

  *  :ref:`is_local_expr (expr:smart_ptr\<ast::Expression\> const) : bool const <function-_at_is_local_c__c_is_local_expr_CY_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M>` 
  *  :ref:`is_local_or_global_expr (expr:smart_ptr\<ast::Expression\> const) : bool const <function-_at_is_local_c__c_is_local_or_global_expr_CY_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M>` 
  *  :ref:`is_scope_expr (expr:smart_ptr\<ast::Expression\> const) : bool const <function-_at_is_local_c__c_is_scope_expr_CY_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M>` 

.. _function-_at_is_local_c__c_is_local_expr_CY_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M:

.. das:function:: is_local_expr(expr: ExpressionPtr)

is_local_expr returns bool const

+--------+--------------------------------------------+
+argument+argument type                               +
+========+============================================+
+expr    + :ref:`ExpressionPtr <alias-ExpressionPtr>` +
+--------+--------------------------------------------+


Returns true if the expression is local to the current scope.

.. _function-_at_is_local_c__c_is_local_or_global_expr_CY_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M:

.. das:function:: is_local_or_global_expr(expr: ExpressionPtr)

is_local_or_global_expr returns bool const

+--------+--------------------------------------------+
+argument+argument type                               +
+========+============================================+
+expr    + :ref:`ExpressionPtr <alias-ExpressionPtr>` +
+--------+--------------------------------------------+


Returns true if expression is local the current scope or global scope.

.. _function-_at_is_local_c__c_is_scope_expr_CY_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M:

.. das:function:: is_scope_expr(expr: ExpressionPtr)

is_scope_expr returns bool const

+--------+--------------------------------------------+
+argument+argument type                               +
+========+============================================+
+expr    + :ref:`ExpressionPtr <alias-ExpressionPtr>` +
+--------+--------------------------------------------+


Returns true if the expression is a scoped expression, i.e. eventually points to a variable.

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_is_local_c__c_is_temp_safe_CY_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M:

.. das:function:: is_temp_safe(expr: ExpressionPtr)

is_temp_safe returns bool const

+--------+--------------------------------------------+
+argument+argument type                               +
+========+============================================+
+expr    + :ref:`ExpressionPtr <alias-ExpressionPtr>` +
+--------+--------------------------------------------+


Returns true if the exression had no calls, [] or table [] operators of any kind.
This is used to check expression can be safely casted to temp type.
local variables are ok
we can do swizzle on result
we get to foo() which returns something &

.. _function-_at_is_local_c__c_is_shared_expr_CY_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M:

.. das:function:: is_shared_expr(expr: ExpressionPtr)

is_shared_expr returns bool const

+--------+--------------------------------------------+
+argument+argument type                               +
+========+============================================+
+expr    + :ref:`ExpressionPtr <alias-ExpressionPtr>` +
+--------+--------------------------------------------+


Returns true if the expression is local to the current scope.


