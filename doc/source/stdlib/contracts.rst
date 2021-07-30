
.. _stdlib_contracts:

=================================
Miscelanious contract annotations
=================================

.. include:: detail/contracts.rst

|module-contracts|

+++++++
Classes
+++++++

.. _struct-contracts-IsAnyArrayMacro:

.. das:attribute:: IsAnyArrayMacro : IsAnyType

|class-contracts-IsAnyArrayMacro|

.. das:function:: IsAnyArrayMacro.transform(self: AstFunctionAnnotation; call: smart_ptr<ast::ExprCallFunc>; errors: das_string)

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


|method-contracts-IsAnyArrayMacro.transform|

.. das:function:: IsAnyArrayMacro.apply(self: AstFunctionAnnotation; func: FunctionPtr; group: ModuleGroup; args: AnnotationArgumentList const; errors: das_string)

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


|method-contracts-IsAnyArrayMacro.apply|

.. das:function:: IsAnyArrayMacro.finish(self: AstFunctionAnnotation; func: FunctionPtr; group: ModuleGroup; args: AnnotationArgumentList const; progArgs: AnnotationArgumentList const; errors: das_string)

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


|method-contracts-IsAnyArrayMacro.finish|

.. das:function:: IsAnyArrayMacro.isCompatible(self: AstFunctionAnnotation; func: FunctionPtr; types: VectorTypeDeclPtr; decl: AnnotationDeclaration const; errors: das_string)

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


|method-contracts-IsAnyArrayMacro.isCompatible|

.. das:function:: IsAnyArrayMacro.isSpecialized(self: AstFunctionAnnotation)

isSpecialized returns bool

|method-contracts-IsAnyArrayMacro.isSpecialized|

.. _struct-contracts-IsAnyEnumMacro:

.. das:attribute:: IsAnyEnumMacro : IsAnyType

|class-contracts-IsAnyEnumMacro|

.. das:function:: IsAnyEnumMacro.transform(self: AstFunctionAnnotation; call: smart_ptr<ast::ExprCallFunc>; errors: das_string)

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


|method-contracts-IsAnyEnumMacro.transform|

.. das:function:: IsAnyEnumMacro.apply(self: AstFunctionAnnotation; func: FunctionPtr; group: ModuleGroup; args: AnnotationArgumentList const; errors: das_string)

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


|method-contracts-IsAnyEnumMacro.apply|

.. das:function:: IsAnyEnumMacro.finish(self: AstFunctionAnnotation; func: FunctionPtr; group: ModuleGroup; args: AnnotationArgumentList const; progArgs: AnnotationArgumentList const; errors: das_string)

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


|method-contracts-IsAnyEnumMacro.finish|

.. das:function:: IsAnyEnumMacro.isCompatible(self: AstFunctionAnnotation; func: FunctionPtr; types: VectorTypeDeclPtr; decl: AnnotationDeclaration const; errors: das_string)

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


|method-contracts-IsAnyEnumMacro.isCompatible|

.. das:function:: IsAnyEnumMacro.isSpecialized(self: AstFunctionAnnotation)

isSpecialized returns bool

|method-contracts-IsAnyEnumMacro.isSpecialized|

.. _struct-contracts-IsAnyType:

.. das:attribute:: IsAnyType : AstFunctionAnnotation

|class-contracts-IsAnyType|

.. das:function:: IsAnyType.transform(self: AstFunctionAnnotation; call: smart_ptr<ast::ExprCallFunc>; errors: das_string)

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


|method-contracts-IsAnyType.transform|

.. das:function:: IsAnyType.apply(self: AstFunctionAnnotation; func: FunctionPtr; group: ModuleGroup; args: AnnotationArgumentList const; errors: das_string)

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


|method-contracts-IsAnyType.apply|

.. das:function:: IsAnyType.finish(self: AstFunctionAnnotation; func: FunctionPtr; group: ModuleGroup; args: AnnotationArgumentList const; progArgs: AnnotationArgumentList const; errors: das_string)

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


|method-contracts-IsAnyType.finish|

.. das:function:: IsAnyType.isCompatible(self: AstFunctionAnnotation; func: FunctionPtr; types: VectorTypeDeclPtr; decl: AnnotationDeclaration const; errors: das_string)

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


|method-contracts-IsAnyType.isCompatible|

.. das:function:: IsAnyType.isSpecialized(self: AstFunctionAnnotation)

isSpecialized returns bool

|method-contracts-IsAnyType.isSpecialized|

.. _struct-contracts-IsAnyVectorType:

.. das:attribute:: IsAnyVectorType : IsAnyType

|class-contracts-IsAnyVectorType|

.. das:function:: IsAnyVectorType.transform(self: AstFunctionAnnotation; call: smart_ptr<ast::ExprCallFunc>; errors: das_string)

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


|method-contracts-IsAnyVectorType.transform|

.. das:function:: IsAnyVectorType.apply(self: AstFunctionAnnotation; func: FunctionPtr; group: ModuleGroup; args: AnnotationArgumentList const; errors: das_string)

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


|method-contracts-IsAnyVectorType.apply|

.. das:function:: IsAnyVectorType.finish(self: AstFunctionAnnotation; func: FunctionPtr; group: ModuleGroup; args: AnnotationArgumentList const; progArgs: AnnotationArgumentList const; errors: das_string)

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


|method-contracts-IsAnyVectorType.finish|

.. das:function:: IsAnyVectorType.isCompatible(self: AstFunctionAnnotation; func: FunctionPtr; types: VectorTypeDeclPtr; decl: AnnotationDeclaration const; errors: das_string)

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


|method-contracts-IsAnyVectorType.isCompatible|

.. das:function:: IsAnyVectorType.isSpecialized(self: AstFunctionAnnotation)

isSpecialized returns bool

|method-contracts-IsAnyVectorType.isSpecialized|


