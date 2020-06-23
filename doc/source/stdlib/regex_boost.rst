
.. _stdlib_regex_boost:

=======================
Boost package for REGEX
=======================

.. include:: detail/regex_boost.rst

|module-regex_boost|

+++++++
Classes
+++++++

.. _struct-regex_boost-RegexReader:

.. das:attribute:: RegexReader : AstReaderMacro

|class-regex_boost-RegexReader|

.. das:function:: RegexReader.accept(self: AstReaderMacro; prog: ProgramPtr; mod: rtti::Module? const; expr: ast::ExprReader? const; ch: int const; info: LineInfo const)

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


|method-regex_boost-RegexReader.accept|

.. das:function:: RegexReader.visit(self: AstReaderMacro; prog: ProgramPtr; mod: rtti::Module? const; expr: smart_ptr<ast::ExprReader> const)

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


|method-regex_boost-RegexReader.visit|


