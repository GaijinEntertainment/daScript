
.. _stdlib_json_boost:

======================
Boost package for JSON
======================

+++++++
Classes
+++++++

.. _struct-json_boost-BetterJsonMacro:

.. das:class:: BetterJsonMacro : AstVariantMacro

to be documented

.. das:method:: BetterJsonMacro.visitExprIsVariant ( self:AstVariantMacro; prog:ProgramPtr; mod:rtti::Module? const; expr:smart_ptr<ast::ExprIsVariant> const )  : ExpressionPtr 

visitExprIsVariant returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVariantMacro <struct-ast-AstVariantMacro>`              +
+----+-----------------------------------------------------------------------+
+prog+ :ref:`ProgramPtr <alias_ProgramPtr>`                                  +
+----+-----------------------------------------------------------------------+
+mod + :ref:`rtti::Module <handle_rtti_Module>` ? const                      +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprIsVariant <handle_ast_ExprIsVariant>` > const+
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: BetterJsonMacro.visitExprAsVariant ( self:AstVariantMacro; prog:ProgramPtr; mod:rtti::Module? const; expr:smart_ptr<ast::ExprAsVariant> const )  : ExpressionPtr 

visitExprAsVariant returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVariantMacro <struct-ast-AstVariantMacro>`              +
+----+-----------------------------------------------------------------------+
+prog+ :ref:`ProgramPtr <alias_ProgramPtr>`                                  +
+----+-----------------------------------------------------------------------+
+mod + :ref:`rtti::Module <handle_rtti_Module>` ? const                      +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprAsVariant <handle_ast_ExprAsVariant>` > const+
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: BetterJsonMacro.visitExprSafeAsVariant ( self:AstVariantMacro; prog:ProgramPtr; mod:rtti::Module? const; expr:smart_ptr<ast::ExprSafeAsVariant> const )  : ExpressionPtr 

visitExprSafeAsVariant returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVariantMacro <struct-ast-AstVariantMacro>`                      +
+----+-------------------------------------------------------------------------------+
+prog+ :ref:`ProgramPtr <alias_ProgramPtr>`                                          +
+----+-------------------------------------------------------------------------------+
+mod + :ref:`rtti::Module <handle_rtti_Module>` ? const                              +
+----+-------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprSafeAsVariant <handle_ast_ExprSafeAsVariant>` > const+
+----+-------------------------------------------------------------------------------+


to be documented



.. _struct-json_boost-JsonReader:

.. das:class:: JsonReader : AstReaderMacro

to be documented

.. das:method:: JsonReader.accept ( self:AstReaderMacro; prog:ProgramPtr; mod:rtti::Module? const; expr:ast::ExprReader? const; ch:int const; info:LineInfo const )  : bool 

accept returns bool

method arguments are

+----+--------------------------------------------------------+
+self+ :ref:`ast::AstReaderMacro <struct-ast-AstReaderMacro>` +
+----+--------------------------------------------------------+
+prog+ :ref:`ProgramPtr <alias_ProgramPtr>`                   +
+----+--------------------------------------------------------+
+mod + :ref:`rtti::Module <handle_rtti_Module>` ? const       +
+----+--------------------------------------------------------+
+expr+ :ref:`ast::ExprReader <handle_ast_ExprReader>` ? const +
+----+--------------------------------------------------------+
+ch  +int const                                               +
+----+--------------------------------------------------------+
+info+ :ref:`rtti::LineInfo <handle_rtti_LineInfo>`  const    +
+----+--------------------------------------------------------+


to be documented


.. das:method:: JsonReader.visit ( self:AstReaderMacro; prog:ProgramPtr; mod:rtti::Module? const; expr:smart_ptr<ast::ExprReader> const )  : ExpressionPtr 

visit returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------------+
+self+ :ref:`ast::AstReaderMacro <struct-ast-AstReaderMacro>`          +
+----+-----------------------------------------------------------------+
+prog+ :ref:`ProgramPtr <alias_ProgramPtr>`                            +
+----+-----------------------------------------------------------------+
+mod + :ref:`rtti::Module <handle_rtti_Module>` ? const                +
+----+-----------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprReader <handle_ast_ExprReader>` > const+
+----+-----------------------------------------------------------------+


to be documented




