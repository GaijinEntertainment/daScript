
.. _stdlib_if_not_null:

=================
if_not_null macro
=================

.. include:: detail/if_not_null.rst

|module-if_not_null|

+++++++
Classes
+++++++

.. _struct-if_not_null-ApplyMacro:

.. das:attribute:: ApplyMacro : AstCallMacro

|class-if_not_null-ApplyMacro|

it defines as follows

  if_not_null_index : int

.. das:function:: ApplyMacro.visit(self: AstCallMacro; prog: ProgramPtr; mod: rtti::Module? const; expr: smart_ptr<ast::ExprCallMacro> const)

visit returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstCallMacro <struct-ast-AstCallMacro>`                    +
+--------+-----------------------------------------------------------------------+
+prog    + :ref:`ProgramPtr <alias-ProgramPtr>`                                  +
+--------+-----------------------------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const                      +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprCallMacro <handle-ast-ExprCallMacro>` > const+
+--------+-----------------------------------------------------------------------+


|method-if_not_null-ApplyMacro.visit|


