
.. _stdlib_json_boost:

======================
Boost package for JSON
======================

+++++++
Classes
+++++++

.. _struct-json_boost-BetterJsonMacro:

.. das:class:: BetterJsonMacro : AstVariantMacro

class|json_boost|BetterJsonMacro to be documented

.. das:method:: BetterJsonMacro.visitExprIsVariant (self:AstVariantMacro; prog:ProgramPtr; mod:rtti::Module? const; expr:smart_ptr<ast::ExprIsVariant> const )  : ExpressionPtr

visitExprIsVariant returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVariantMacro <struct-ast-AstVariantMacro>`              +
+----+-----------------------------------------------------------------------+
+prog+ :ref:`ProgramPtr <alias-ProgramPtr>`                                  +
+----+-----------------------------------------------------------------------+
+mod + :ref:`rtti::Module <handle-rtti-Module>` ? const                      +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprIsVariant <handle-ast-ExprIsVariant>` > const+
+----+-----------------------------------------------------------------------+


method|json_boost|BetterJsonMacro.visitExprIsVariant to be documented

.. das:method:: BetterJsonMacro.visitExprAsVariant (self:AstVariantMacro; prog:ProgramPtr; mod:rtti::Module? const; expr:smart_ptr<ast::ExprAsVariant> const )  : ExpressionPtr

visitExprAsVariant returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVariantMacro <struct-ast-AstVariantMacro>`              +
+----+-----------------------------------------------------------------------+
+prog+ :ref:`ProgramPtr <alias-ProgramPtr>`                                  +
+----+-----------------------------------------------------------------------+
+mod + :ref:`rtti::Module <handle-rtti-Module>` ? const                      +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprAsVariant <handle-ast-ExprAsVariant>` > const+
+----+-----------------------------------------------------------------------+


method|json_boost|BetterJsonMacro.visitExprAsVariant to be documented

.. das:method:: BetterJsonMacro.visitExprSafeAsVariant (self:AstVariantMacro; prog:ProgramPtr; mod:rtti::Module? const; expr:smart_ptr<ast::ExprSafeAsVariant> const )  : ExpressionPtr

visitExprSafeAsVariant returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVariantMacro <struct-ast-AstVariantMacro>`                      +
+----+-------------------------------------------------------------------------------+
+prog+ :ref:`ProgramPtr <alias-ProgramPtr>`                                          +
+----+-------------------------------------------------------------------------------+
+mod + :ref:`rtti::Module <handle-rtti-Module>` ? const                              +
+----+-------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprSafeAsVariant <handle-ast-ExprSafeAsVariant>` > const+
+----+-------------------------------------------------------------------------------+


method|json_boost|BetterJsonMacro.visitExprSafeAsVariant to be documented

.. _struct-json_boost-JsonReader:

.. das:class:: JsonReader : AstReaderMacro

class|json_boost|JsonReader to be documented

.. das:method:: JsonReader.accept (self:AstReaderMacro; prog:ProgramPtr; mod:rtti::Module? const; expr:ast::ExprReader? const; ch:int const; info:LineInfo const )  : bool

accept returns bool

arguments are

+----+--------------------------------------------------------+
+self+ :ref:`ast::AstReaderMacro <struct-ast-AstReaderMacro>` +
+----+--------------------------------------------------------+
+prog+ :ref:`ProgramPtr <alias-ProgramPtr>`                   +
+----+--------------------------------------------------------+
+mod + :ref:`rtti::Module <handle-rtti-Module>` ? const       +
+----+--------------------------------------------------------+
+expr+ :ref:`ast::ExprReader <handle-ast-ExprReader>` ? const +
+----+--------------------------------------------------------+
+ch  +int const                                               +
+----+--------------------------------------------------------+
+info+ :ref:`rtti::LineInfo <handle-rtti-LineInfo>`  const    +
+----+--------------------------------------------------------+


method|json_boost|JsonReader.accept to be documented

.. das:method:: JsonReader.visit (self:AstReaderMacro; prog:ProgramPtr; mod:rtti::Module? const; expr:smart_ptr<ast::ExprReader> const )  : ExpressionPtr

visit returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------------+
+self+ :ref:`ast::AstReaderMacro <struct-ast-AstReaderMacro>`          +
+----+-----------------------------------------------------------------+
+prog+ :ref:`ProgramPtr <alias-ProgramPtr>`                            +
+----+-----------------------------------------------------------------+
+mod + :ref:`rtti::Module <handle-rtti-Module>` ? const                +
+----+-----------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprReader <handle-ast-ExprReader>` > const+
+----+-----------------------------------------------------------------+


method|json_boost|JsonReader.visit to be documented


