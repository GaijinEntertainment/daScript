
.. _stdlib_static_let:

================
static_let macro
================

.. include:: detail/static_let.rst

|module-static_let|

+++++++
Classes
+++++++

.. _struct-static_let-StaticLetMacro:

.. das:attribute:: StaticLetMacro : AstFunctionAnnotation

|class-static_let-StaticLetMacro|

.. das:function:: StaticLetMacro.transform(self: AstFunctionAnnotation; call: smart_ptr<ast::ExprCallFunc>; errors: das_string)

transform returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+----------------------------------------------------------------------+
+argument+argument type                                                         +
+========+======================================================================+
+self    + :ref:`ast::AstFunctionAnnotation <struct-ast-AstFunctionAnnotation>` +
+--------+----------------------------------------------------------------------+
+call    +smart_ptr< :ref:`ast::ExprCallFunc <handle-ast-ExprCallFunc>` >       +
+--------+----------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`               +
+--------+----------------------------------------------------------------------+


|method-static_let-StaticLetMacro.transform|

.. das:function:: StaticLetMacro.apply(self: AstFunctionAnnotation; func: FunctionPtr; group: ModuleGroup; args: AnnotationArgumentList const; errors: das_string)

apply returns bool

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+self    + :ref:`ast::AstFunctionAnnotation <struct-ast-AstFunctionAnnotation>`           +
+--------+--------------------------------------------------------------------------------+
+func    + :ref:`FunctionPtr <alias-FunctionPtr>`                                         +
+--------+--------------------------------------------------------------------------------+
+group   + :ref:`rtti::ModuleGroup <handle-rtti-ModuleGroup>`                             +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`                         +
+--------+--------------------------------------------------------------------------------+


|method-static_let-StaticLetMacro.apply|

.. das:function:: StaticLetMacro.finish(self: AstFunctionAnnotation; func: FunctionPtr; group: ModuleGroup; args: AnnotationArgumentList const; progArgs: AnnotationArgumentList const; errors: das_string)

finish returns bool

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+self    + :ref:`ast::AstFunctionAnnotation <struct-ast-AstFunctionAnnotation>`           +
+--------+--------------------------------------------------------------------------------+
+func    + :ref:`FunctionPtr <alias-FunctionPtr>`                                         +
+--------+--------------------------------------------------------------------------------+
+group   + :ref:`rtti::ModuleGroup <handle-rtti-ModuleGroup>`                             +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+progArgs+ :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`                         +
+--------+--------------------------------------------------------------------------------+


|method-static_let-StaticLetMacro.finish|

.. das:function:: StaticLetMacro.isCompatible(self: AstFunctionAnnotation; func: FunctionPtr; types: VectorTypeDeclPtr; decl: AnnotationDeclaration const; errors: das_string)

isCompatible returns bool

+--------+------------------------------------------------------------------------------+
+argument+argument type                                                                 +
+========+==============================================================================+
+self    + :ref:`ast::AstFunctionAnnotation <struct-ast-AstFunctionAnnotation>`         +
+--------+------------------------------------------------------------------------------+
+func    + :ref:`FunctionPtr <alias-FunctionPtr>`                                       +
+--------+------------------------------------------------------------------------------+
+types   + :ref:`VectorTypeDeclPtr <alias-VectorTypeDeclPtr>`                           +
+--------+------------------------------------------------------------------------------+
+decl    + :ref:`rtti::AnnotationDeclaration <handle-rtti-AnnotationDeclaration>`  const+
+--------+------------------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`                       +
+--------+------------------------------------------------------------------------------+


|method-static_let-StaticLetMacro.isCompatible|

.. das:function:: StaticLetMacro.isSpecialized(self: AstFunctionAnnotation)

isSpecialized returns bool

|method-static_let-StaticLetMacro.isSpecialized|

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_static_let_c__c_static_let__hh_const_hh__hh_block:

.. das:function:: static_let(blk: block<> const)

+--------+-------------+
+argument+argument type+
+========+=============+
+blk     +block<> const+
+--------+-------------+


|function-static_let-static_let|

.. _function-_at_static_let_c__c_static_let_finalize__hh_const_hh__hh_block:

.. das:function:: static_let_finalize(blk: block<> const)

+--------+-------------+
+argument+argument type+
+========+=============+
+blk     +block<> const+
+--------+-------------+


|function-static_let-static_let_finalize|


