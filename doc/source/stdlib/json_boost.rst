
.. _stdlib_json_boost:

======================
Boost package for JSON
======================

+++++++
Classes
+++++++

.. das:class:: BetterJsonMacro : AstVariantMacro

to be documented

.. das:method:: BetterJsonMacro.visitExprIsVariant (self:ast::AstVariantMacro; prog:smart_ptr<rtti::Program> const; mod:rtti::Module? const; expr:smart_ptr<ast::ExprIsVariant> const) 

visitExprIsVariant returns smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+prog+smart_ptr<rtti::Program> const     +
+----+-----------------------------------+
+mod +rtti::Module? const                +
+----+-----------------------------------+
+expr+smart_ptr<ast::ExprIsVariant> const+
+----+-----------------------------------+


to be documented


.. das:method:: BetterJsonMacro.visitExprAsVariant (self:ast::AstVariantMacro; prog:smart_ptr<rtti::Program> const; mod:rtti::Module? const; expr:smart_ptr<ast::ExprAsVariant> const) 

visitExprAsVariant returns smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+prog+smart_ptr<rtti::Program> const     +
+----+-----------------------------------+
+mod +rtti::Module? const                +
+----+-----------------------------------+
+expr+smart_ptr<ast::ExprAsVariant> const+
+----+-----------------------------------+


to be documented


.. das:method:: BetterJsonMacro.visitExprSafeAsVariant (self:ast::AstVariantMacro; prog:smart_ptr<rtti::Program> const; mod:rtti::Module? const; expr:smart_ptr<ast::ExprSafeAsVariant> const) 

visitExprSafeAsVariant returns smart_ptr<ast::Expression>

function arguments are

+----+---------------------------------------+
+prog+smart_ptr<rtti::Program> const         +
+----+---------------------------------------+
+mod +rtti::Module? const                    +
+----+---------------------------------------+
+expr+smart_ptr<ast::ExprSafeAsVariant> const+
+----+---------------------------------------+


to be documented



.. das:class:: JsonReader : AstReaderMacro

to be documented

.. das:method:: JsonReader.accept (self:ast::AstReaderMacro; prog:smart_ptr<rtti::Program> const; mod:rtti::Module? const; expr:ast::ExprReader? const; ch:int const; info:rtti::LineInfo const) 

accept returns bool

function arguments are

+----+------------------------------+
+prog+smart_ptr<rtti::Program> const+
+----+------------------------------+
+mod +rtti::Module? const           +
+----+------------------------------+
+expr+ast::ExprReader? const        +
+----+------------------------------+
+ch  +int const                     +
+----+------------------------------+
+info+rtti::LineInfo const          +
+----+------------------------------+


to be documented


.. das:method:: JsonReader.visit (self:ast::AstReaderMacro; prog:smart_ptr<rtti::Program> const; mod:rtti::Module? const; expr:smart_ptr<ast::ExprReader> const) 

visit returns smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------+
+prog+smart_ptr<rtti::Program> const  +
+----+--------------------------------+
+mod +rtti::Module? const             +
+----+--------------------------------+
+expr+smart_ptr<ast::ExprReader> const+
+----+--------------------------------+


to be documented




