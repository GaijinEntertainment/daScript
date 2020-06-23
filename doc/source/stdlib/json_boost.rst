
.. _stdlib_json_boost:

======================
Boost package for JSON
======================

.. include:: detail/json_boost.rst

|module-json_boost|

+++++++
Classes
+++++++

.. _struct-json_boost-BetterJsonMacro:

.. das:attribute:: BetterJsonMacro : AstVariantMacro

|class-json_boost-BetterJsonMacro|

.. das:function:: BetterJsonMacro.visitExprIsVariant(self: AstVariantMacro; prog: ProgramPtr; mod: rtti::Module? const; expr: smart_ptr<ast::ExprIsVariant> const)

visitExprIsVariant returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVariantMacro <struct-ast-AstVariantMacro>`              +
+--------+-----------------------------------------------------------------------+
+prog    + :ref:`ProgramPtr <alias-ProgramPtr>`                                  +
+--------+-----------------------------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const                      +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprIsVariant <handle-ast-ExprIsVariant>` > const+
+--------+-----------------------------------------------------------------------+


|method-json_boost-BetterJsonMacro.visitExprIsVariant|

.. das:function:: BetterJsonMacro.visitExprAsVariant(self: AstVariantMacro; prog: ProgramPtr; mod: rtti::Module? const; expr: smart_ptr<ast::ExprAsVariant> const)

visitExprAsVariant returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVariantMacro <struct-ast-AstVariantMacro>`              +
+--------+-----------------------------------------------------------------------+
+prog    + :ref:`ProgramPtr <alias-ProgramPtr>`                                  +
+--------+-----------------------------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const                      +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprAsVariant <handle-ast-ExprAsVariant>` > const+
+--------+-----------------------------------------------------------------------+


|method-json_boost-BetterJsonMacro.visitExprAsVariant|

.. das:function:: BetterJsonMacro.visitExprSafeAsVariant(self: AstVariantMacro; prog: ProgramPtr; mod: rtti::Module? const; expr: smart_ptr<ast::ExprSafeAsVariant> const)

visitExprSafeAsVariant returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-------------------------------------------------------------------------------+
+argument+argument type                                                                  +
+========+===============================================================================+
+self    + :ref:`ast::AstVariantMacro <struct-ast-AstVariantMacro>`                      +
+--------+-------------------------------------------------------------------------------+
+prog    + :ref:`ProgramPtr <alias-ProgramPtr>`                                          +
+--------+-------------------------------------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const                              +
+--------+-------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprSafeAsVariant <handle-ast-ExprSafeAsVariant>` > const+
+--------+-------------------------------------------------------------------------------+


|method-json_boost-BetterJsonMacro.visitExprSafeAsVariant|

.. _struct-json_boost-JsonReader:

.. das:attribute:: JsonReader : AstReaderMacro

|class-json_boost-JsonReader|

.. das:function:: JsonReader.accept(self: AstReaderMacro; prog: ProgramPtr; mod: rtti::Module? const; expr: ast::ExprReader? const; ch: int const; info: LineInfo const)

accept returns bool

+--------+--------------------------------------------------------+
+argument+argument type                                           +
+========+========================================================+
+self    + :ref:`ast::AstReaderMacro <struct-ast-AstReaderMacro>` +
+--------+--------------------------------------------------------+
+prog    + :ref:`ProgramPtr <alias-ProgramPtr>`                   +
+--------+--------------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const       +
+--------+--------------------------------------------------------+
+expr    + :ref:`ast::ExprReader <handle-ast-ExprReader>` ? const +
+--------+--------------------------------------------------------+
+ch      +int const                                               +
+--------+--------------------------------------------------------+
+info    + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`  const    +
+--------+--------------------------------------------------------+


|method-json_boost-JsonReader.accept|

.. das:function:: JsonReader.visit(self: AstReaderMacro; prog: ProgramPtr; mod: rtti::Module? const; expr: smart_ptr<ast::ExprReader> const)

visit returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+self    + :ref:`ast::AstReaderMacro <struct-ast-AstReaderMacro>`          +
+--------+-----------------------------------------------------------------+
+prog    + :ref:`ProgramPtr <alias-ProgramPtr>`                            +
+--------+-----------------------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const                +
+--------+-----------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprReader <handle-ast-ExprReader>` > const+
+--------+-----------------------------------------------------------------+


|method-json_boost-JsonReader.visit|


