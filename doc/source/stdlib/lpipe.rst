
.. _stdlib_lpipe:

===========
lpipe macro
===========

.. include:: detail/lpipe.rst

|module-lpipe|

+++++++
Classes
+++++++

.. _struct-lpipe-LpipeMacro:

.. das:attribute:: LpipeMacro : AstCallMacro

|class-lpipe-LpipeMacro|

.. das:function:: LpipeMacro.visit(self: AstCallMacro; prog: ProgramPtr; mod: rtti::Module? const; expr: smart_ptr<ast::ExprCallMacro> const)

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


|method-lpipe-LpipeMacro.visit|


