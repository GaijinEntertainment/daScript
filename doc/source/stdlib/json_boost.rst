
.. _stdlib_json_boost:

======================
Boost package for JSON
======================

+++++++
Classes
+++++++

.. das:class:: BetterJsonMacro : AstVariantMacro

to be documented

.. das:function:: BetterJsonMacro->__finalize (self) 

to be documented


.. das:function:: BetterJsonMacro->visitExprIsVariant (self;prog;mod;expr) 

visitExprIsVariant returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+prog+smart_ptr<rtti::Program> const     +
+----+-----------------------------------+
+mod +rtti::Module? const                +
+----+-----------------------------------+
+expr+smart_ptr<ast::ExprIsVariant> const+
+----+-----------------------------------+


to be documented


.. das:function:: BetterJsonMacro->visitExprAsVariant (self;prog;mod;expr) 

visitExprAsVariant returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+prog+smart_ptr<rtti::Program> const     +
+----+-----------------------------------+
+mod +rtti::Module? const                +
+----+-----------------------------------+
+expr+smart_ptr<ast::ExprAsVariant> const+
+----+-----------------------------------+


to be documented


.. das:function:: BetterJsonMacro->visitExprSafeAsVariant (self;prog;mod;expr) 

visitExprSafeAsVariant returns ::

 smart_ptr<ast::Expression>

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

.. das:function:: JsonReader->__finalize (self) 

to be documented


.. das:function:: JsonReader->accept (self;prog;mod;expr;ch;info) 

accept returns ::

 bool

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


.. das:function:: JsonReader->visit (self;prog;mod;expr) 

visit returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------+
+prog+smart_ptr<rtti::Program> const  +
+----+--------------------------------+
+mod +rtti::Module? const             +
+----+--------------------------------+
+expr+smart_ptr<ast::ExprReader> const+
+----+--------------------------------+


to be documented




