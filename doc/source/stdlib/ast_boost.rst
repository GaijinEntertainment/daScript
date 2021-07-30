
.. _stdlib_ast_boost:

=========================
Boost package for the AST
=========================

.. include:: detail/ast_boost.rst

The AST boost module implements collection of helper macros and functions to accompany :ref:`AST <stdlib_ast>`.

All functions and symbols are in "ast" module, use require to get access to it. ::

    require daslib/ast_boost


+++++++
Classes
+++++++

.. _struct-ast_boost-BetterRttiVisitor:

.. das:attribute:: BetterRttiVisitor : AstVariantMacro

|class-ast_boost-BetterRttiVisitor|

.. das:function:: BetterRttiVisitor.visitExprIsVariant(self: AstVariantMacro; prog: ProgramPtr; mod: rtti::Module? const; expr: smart_ptr<ast::ExprIsVariant> const)

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


|method-ast_boost-BetterRttiVisitor.visitExprIsVariant|

.. das:function:: BetterRttiVisitor.visitExprAsVariant(self: AstVariantMacro; prog: ProgramPtr; mod: rtti::Module? const; expr: smart_ptr<ast::ExprAsVariant> const)

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


|method-ast_boost-BetterRttiVisitor.visitExprAsVariant|

.. das:function:: BetterRttiVisitor.visitExprSafeAsVariant(self: AstVariantMacro; prog: ProgramPtr; mod: rtti::Module? const; expr: smart_ptr<ast::ExprSafeAsVariant> const)

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


|method-ast_boost-BetterRttiVisitor.visitExprSafeAsVariant|

.. _struct-ast_boost-MacroMacro:

.. das:attribute:: MacroMacro : AstFunctionAnnotation

|class-ast_boost-MacroMacro|

.. das:function:: MacroMacro.transform(self: AstFunctionAnnotation; call: smart_ptr<ast::ExprCallFunc>; errors: das_string)

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


|method-ast_boost-MacroMacro.transform|

.. das:function:: MacroMacro.apply(self: AstFunctionAnnotation; func: FunctionPtr; group: ModuleGroup; args: AnnotationArgumentList const; errors: das_string)

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


|method-ast_boost-MacroMacro.apply|

.. das:function:: MacroMacro.finish(self: AstFunctionAnnotation; func: FunctionPtr; group: ModuleGroup; args: AnnotationArgumentList const; progArgs: AnnotationArgumentList const; errors: das_string)

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


|method-ast_boost-MacroMacro.finish|

.. das:function:: MacroMacro.isCompatible(self: AstFunctionAnnotation; func: FunctionPtr; types: VectorTypeDeclPtr; decl: AnnotationDeclaration const; errors: das_string)

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


|method-ast_boost-MacroMacro.isCompatible|

.. das:function:: MacroMacro.isSpecialized(self: AstFunctionAnnotation)

isSpecialized returns bool

|method-ast_boost-MacroMacro.isSpecialized|

.. _struct-ast_boost-SetupAnyAnnotation:

.. das:attribute:: SetupAnyAnnotation : AstStructureAnnotation

|class-ast_boost-SetupAnyAnnotation|

it defines as follows

  annotation_function_call : string
  name                     : string

.. das:function:: SetupAnyAnnotation.apply(self: AstStructureAnnotation; st: StructurePtr; group: ModuleGroup; args: AnnotationArgumentList const; errors: das_string)

apply returns bool

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+self    + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+--------+--------------------------------------------------------------------------------+
+st      + :ref:`StructurePtr <alias-StructurePtr>`                                       +
+--------+--------------------------------------------------------------------------------+
+group   + :ref:`rtti::ModuleGroup <handle-rtti-ModuleGroup>`                             +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`                         +
+--------+--------------------------------------------------------------------------------+


|method-ast_boost-SetupAnyAnnotation.apply|

.. das:function:: SetupAnyAnnotation.finish(self: AstStructureAnnotation; st: StructurePtr; group: ModuleGroup; args: AnnotationArgumentList const; errors: das_string)

finish returns bool

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+self    + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+--------+--------------------------------------------------------------------------------+
+st      + :ref:`StructurePtr <alias-StructurePtr>`                                       +
+--------+--------------------------------------------------------------------------------+
+group   + :ref:`rtti::ModuleGroup <handle-rtti-ModuleGroup>`                             +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`                         +
+--------+--------------------------------------------------------------------------------+


|method-ast_boost-SetupAnyAnnotation.finish|

.. das:function:: SetupAnyAnnotation.setup_call(self: SetupAnyAnnotation; st: StructurePtr; cll: smart_ptr<ast::ExprCall>)

+--------+----------------------------------------------------------------------------+
+argument+argument type                                                               +
+========+============================================================================+
+self    + :ref:`ast_boost::SetupAnyAnnotation <struct-ast_boost-SetupAnyAnnotation>` +
+--------+----------------------------------------------------------------------------+
+st      + :ref:`StructurePtr <alias-StructurePtr>`                                   +
+--------+----------------------------------------------------------------------------+
+cll     +smart_ptr< :ref:`ast::ExprCall <handle-ast-ExprCall>` >                     +
+--------+----------------------------------------------------------------------------+


|method-ast_boost-SetupAnyAnnotation.setup_call|

.. _struct-ast_boost-SetupCallMacro:

.. das:attribute:: SetupCallMacro : SetupAnyAnnotation

|class-ast_boost-SetupCallMacro|

it defines as follows

  annotation_function_call : string
  name                     : string

.. das:function:: SetupCallMacro.apply(self: AstStructureAnnotation; st: StructurePtr; group: ModuleGroup; args: AnnotationArgumentList const; errors: das_string)

apply returns bool

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+self    + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+--------+--------------------------------------------------------------------------------+
+st      + :ref:`StructurePtr <alias-StructurePtr>`                                       +
+--------+--------------------------------------------------------------------------------+
+group   + :ref:`rtti::ModuleGroup <handle-rtti-ModuleGroup>`                             +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`                         +
+--------+--------------------------------------------------------------------------------+


|method-ast_boost-SetupCallMacro.apply|

.. das:function:: SetupCallMacro.finish(self: AstStructureAnnotation; st: StructurePtr; group: ModuleGroup; args: AnnotationArgumentList const; errors: das_string)

finish returns bool

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+self    + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+--------+--------------------------------------------------------------------------------+
+st      + :ref:`StructurePtr <alias-StructurePtr>`                                       +
+--------+--------------------------------------------------------------------------------+
+group   + :ref:`rtti::ModuleGroup <handle-rtti-ModuleGroup>`                             +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`                         +
+--------+--------------------------------------------------------------------------------+


|method-ast_boost-SetupCallMacro.finish|

.. das:function:: SetupCallMacro.setup_call(self: SetupAnyAnnotation; st: StructurePtr; cll: smart_ptr<ast::ExprCall>)

+--------+----------------------------------------------------------------------------+
+argument+argument type                                                               +
+========+============================================================================+
+self    + :ref:`ast_boost::SetupAnyAnnotation <struct-ast_boost-SetupAnyAnnotation>` +
+--------+----------------------------------------------------------------------------+
+st      + :ref:`StructurePtr <alias-StructurePtr>`                                   +
+--------+----------------------------------------------------------------------------+
+cll     +smart_ptr< :ref:`ast::ExprCall <handle-ast-ExprCall>` >                     +
+--------+----------------------------------------------------------------------------+


|method-ast_boost-SetupCallMacro.setup_call|

.. _struct-ast_boost-SetupContractAnnotatoin:

.. das:attribute:: SetupContractAnnotatoin : SetupAnyAnnotation

|class-ast_boost-SetupContractAnnotatoin|

it defines as follows

  annotation_function_call : string
  name                     : string

.. das:function:: SetupContractAnnotatoin.apply(self: AstStructureAnnotation; st: StructurePtr; group: ModuleGroup; args: AnnotationArgumentList const; errors: das_string)

apply returns bool

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+self    + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+--------+--------------------------------------------------------------------------------+
+st      + :ref:`StructurePtr <alias-StructurePtr>`                                       +
+--------+--------------------------------------------------------------------------------+
+group   + :ref:`rtti::ModuleGroup <handle-rtti-ModuleGroup>`                             +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`                         +
+--------+--------------------------------------------------------------------------------+


|method-ast_boost-SetupContractAnnotatoin.apply|

.. das:function:: SetupContractAnnotatoin.finish(self: AstStructureAnnotation; st: StructurePtr; group: ModuleGroup; args: AnnotationArgumentList const; errors: das_string)

finish returns bool

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+self    + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+--------+--------------------------------------------------------------------------------+
+st      + :ref:`StructurePtr <alias-StructurePtr>`                                       +
+--------+--------------------------------------------------------------------------------+
+group   + :ref:`rtti::ModuleGroup <handle-rtti-ModuleGroup>`                             +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`                         +
+--------+--------------------------------------------------------------------------------+


|method-ast_boost-SetupContractAnnotatoin.finish|

.. das:function:: SetupContractAnnotatoin.setup_call(self: SetupAnyAnnotation; st: StructurePtr; cll: smart_ptr<ast::ExprCall>)

+--------+----------------------------------------------------------------------------+
+argument+argument type                                                               +
+========+============================================================================+
+self    + :ref:`ast_boost::SetupAnyAnnotation <struct-ast_boost-SetupAnyAnnotation>` +
+--------+----------------------------------------------------------------------------+
+st      + :ref:`StructurePtr <alias-StructurePtr>`                                   +
+--------+----------------------------------------------------------------------------+
+cll     +smart_ptr< :ref:`ast::ExprCall <handle-ast-ExprCall>` >                     +
+--------+----------------------------------------------------------------------------+


|method-ast_boost-SetupContractAnnotatoin.setup_call|

.. _struct-ast_boost-SetupFunctionAnnotatoin:

.. das:attribute:: SetupFunctionAnnotatoin : SetupAnyAnnotation

|class-ast_boost-SetupFunctionAnnotatoin|

it defines as follows

  annotation_function_call : string
  name                     : string

.. das:function:: SetupFunctionAnnotatoin.apply(self: AstStructureAnnotation; st: StructurePtr; group: ModuleGroup; args: AnnotationArgumentList const; errors: das_string)

apply returns bool

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+self    + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+--------+--------------------------------------------------------------------------------+
+st      + :ref:`StructurePtr <alias-StructurePtr>`                                       +
+--------+--------------------------------------------------------------------------------+
+group   + :ref:`rtti::ModuleGroup <handle-rtti-ModuleGroup>`                             +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`                         +
+--------+--------------------------------------------------------------------------------+


|method-ast_boost-SetupFunctionAnnotatoin.apply|

.. das:function:: SetupFunctionAnnotatoin.finish(self: AstStructureAnnotation; st: StructurePtr; group: ModuleGroup; args: AnnotationArgumentList const; errors: das_string)

finish returns bool

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+self    + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+--------+--------------------------------------------------------------------------------+
+st      + :ref:`StructurePtr <alias-StructurePtr>`                                       +
+--------+--------------------------------------------------------------------------------+
+group   + :ref:`rtti::ModuleGroup <handle-rtti-ModuleGroup>`                             +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`                         +
+--------+--------------------------------------------------------------------------------+


|method-ast_boost-SetupFunctionAnnotatoin.finish|

.. das:function:: SetupFunctionAnnotatoin.setup_call(self: SetupAnyAnnotation; st: StructurePtr; cll: smart_ptr<ast::ExprCall>)

+--------+----------------------------------------------------------------------------+
+argument+argument type                                                               +
+========+============================================================================+
+self    + :ref:`ast_boost::SetupAnyAnnotation <struct-ast_boost-SetupAnyAnnotation>` +
+--------+----------------------------------------------------------------------------+
+st      + :ref:`StructurePtr <alias-StructurePtr>`                                   +
+--------+----------------------------------------------------------------------------+
+cll     +smart_ptr< :ref:`ast::ExprCall <handle-ast-ExprCall>` >                     +
+--------+----------------------------------------------------------------------------+


|method-ast_boost-SetupFunctionAnnotatoin.setup_call|

.. _struct-ast_boost-SetupReaderMacro:

.. das:attribute:: SetupReaderMacro : SetupAnyAnnotation

|class-ast_boost-SetupReaderMacro|

it defines as follows

  annotation_function_call : string
  name                     : string

.. das:function:: SetupReaderMacro.apply(self: AstStructureAnnotation; st: StructurePtr; group: ModuleGroup; args: AnnotationArgumentList const; errors: das_string)

apply returns bool

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+self    + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+--------+--------------------------------------------------------------------------------+
+st      + :ref:`StructurePtr <alias-StructurePtr>`                                       +
+--------+--------------------------------------------------------------------------------+
+group   + :ref:`rtti::ModuleGroup <handle-rtti-ModuleGroup>`                             +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`                         +
+--------+--------------------------------------------------------------------------------+


|method-ast_boost-SetupReaderMacro.apply|

.. das:function:: SetupReaderMacro.finish(self: AstStructureAnnotation; st: StructurePtr; group: ModuleGroup; args: AnnotationArgumentList const; errors: das_string)

finish returns bool

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+self    + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+--------+--------------------------------------------------------------------------------+
+st      + :ref:`StructurePtr <alias-StructurePtr>`                                       +
+--------+--------------------------------------------------------------------------------+
+group   + :ref:`rtti::ModuleGroup <handle-rtti-ModuleGroup>`                             +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`                         +
+--------+--------------------------------------------------------------------------------+


|method-ast_boost-SetupReaderMacro.finish|

.. das:function:: SetupReaderMacro.setup_call(self: SetupAnyAnnotation; st: StructurePtr; cll: smart_ptr<ast::ExprCall>)

+--------+----------------------------------------------------------------------------+
+argument+argument type                                                               +
+========+============================================================================+
+self    + :ref:`ast_boost::SetupAnyAnnotation <struct-ast_boost-SetupAnyAnnotation>` +
+--------+----------------------------------------------------------------------------+
+st      + :ref:`StructurePtr <alias-StructurePtr>`                                   +
+--------+----------------------------------------------------------------------------+
+cll     +smart_ptr< :ref:`ast::ExprCall <handle-ast-ExprCall>` >                     +
+--------+----------------------------------------------------------------------------+


|method-ast_boost-SetupReaderMacro.setup_call|

.. _struct-ast_boost-SetupStructureAnnotatoin:

.. das:attribute:: SetupStructureAnnotatoin : SetupAnyAnnotation

|class-ast_boost-SetupStructureAnnotatoin|

it defines as follows

  annotation_function_call : string
  name                     : string

.. das:function:: SetupStructureAnnotatoin.apply(self: AstStructureAnnotation; st: StructurePtr; group: ModuleGroup; args: AnnotationArgumentList const; errors: das_string)

apply returns bool

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+self    + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+--------+--------------------------------------------------------------------------------+
+st      + :ref:`StructurePtr <alias-StructurePtr>`                                       +
+--------+--------------------------------------------------------------------------------+
+group   + :ref:`rtti::ModuleGroup <handle-rtti-ModuleGroup>`                             +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`                         +
+--------+--------------------------------------------------------------------------------+


|method-ast_boost-SetupStructureAnnotatoin.apply|

.. das:function:: SetupStructureAnnotatoin.finish(self: AstStructureAnnotation; st: StructurePtr; group: ModuleGroup; args: AnnotationArgumentList const; errors: das_string)

finish returns bool

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+self    + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+--------+--------------------------------------------------------------------------------+
+st      + :ref:`StructurePtr <alias-StructurePtr>`                                       +
+--------+--------------------------------------------------------------------------------+
+group   + :ref:`rtti::ModuleGroup <handle-rtti-ModuleGroup>`                             +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`                         +
+--------+--------------------------------------------------------------------------------+


|method-ast_boost-SetupStructureAnnotatoin.finish|

.. das:function:: SetupStructureAnnotatoin.setup_call(self: SetupAnyAnnotation; st: StructurePtr; cll: smart_ptr<ast::ExprCall>)

+--------+----------------------------------------------------------------------------+
+argument+argument type                                                               +
+========+============================================================================+
+self    + :ref:`ast_boost::SetupAnyAnnotation <struct-ast_boost-SetupAnyAnnotation>` +
+--------+----------------------------------------------------------------------------+
+st      + :ref:`StructurePtr <alias-StructurePtr>`                                   +
+--------+----------------------------------------------------------------------------+
+cll     +smart_ptr< :ref:`ast::ExprCall <handle-ast-ExprCall>` >                     +
+--------+----------------------------------------------------------------------------+


|method-ast_boost-SetupStructureAnnotatoin.setup_call|

.. _struct-ast_boost-SetupTypeInfoMacro:

.. das:attribute:: SetupTypeInfoMacro : SetupAnyAnnotation

|class-ast_boost-SetupTypeInfoMacro|

it defines as follows

  annotation_function_call : string
  name                     : string

.. das:function:: SetupTypeInfoMacro.apply(self: AstStructureAnnotation; st: StructurePtr; group: ModuleGroup; args: AnnotationArgumentList const; errors: das_string)

apply returns bool

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+self    + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+--------+--------------------------------------------------------------------------------+
+st      + :ref:`StructurePtr <alias-StructurePtr>`                                       +
+--------+--------------------------------------------------------------------------------+
+group   + :ref:`rtti::ModuleGroup <handle-rtti-ModuleGroup>`                             +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`                         +
+--------+--------------------------------------------------------------------------------+


|method-ast_boost-SetupTypeInfoMacro.apply|

.. das:function:: SetupTypeInfoMacro.finish(self: AstStructureAnnotation; st: StructurePtr; group: ModuleGroup; args: AnnotationArgumentList const; errors: das_string)

finish returns bool

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+self    + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+--------+--------------------------------------------------------------------------------+
+st      + :ref:`StructurePtr <alias-StructurePtr>`                                       +
+--------+--------------------------------------------------------------------------------+
+group   + :ref:`rtti::ModuleGroup <handle-rtti-ModuleGroup>`                             +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`                         +
+--------+--------------------------------------------------------------------------------+


|method-ast_boost-SetupTypeInfoMacro.finish|

.. das:function:: SetupTypeInfoMacro.setup_call(self: SetupAnyAnnotation; st: StructurePtr; cll: smart_ptr<ast::ExprCall>)

+--------+----------------------------------------------------------------------------+
+argument+argument type                                                               +
+========+============================================================================+
+self    + :ref:`ast_boost::SetupAnyAnnotation <struct-ast_boost-SetupAnyAnnotation>` +
+--------+----------------------------------------------------------------------------+
+st      + :ref:`StructurePtr <alias-StructurePtr>`                                   +
+--------+----------------------------------------------------------------------------+
+cll     +smart_ptr< :ref:`ast::ExprCall <handle-ast-ExprCall>` >                     +
+--------+----------------------------------------------------------------------------+


|method-ast_boost-SetupTypeInfoMacro.setup_call|

.. _struct-ast_boost-SetupVariantMacro:

.. das:attribute:: SetupVariantMacro : SetupAnyAnnotation

|class-ast_boost-SetupVariantMacro|

it defines as follows

  annotation_function_call : string
  name                     : string

.. das:function:: SetupVariantMacro.apply(self: AstStructureAnnotation; st: StructurePtr; group: ModuleGroup; args: AnnotationArgumentList const; errors: das_string)

apply returns bool

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+self    + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+--------+--------------------------------------------------------------------------------+
+st      + :ref:`StructurePtr <alias-StructurePtr>`                                       +
+--------+--------------------------------------------------------------------------------+
+group   + :ref:`rtti::ModuleGroup <handle-rtti-ModuleGroup>`                             +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`                         +
+--------+--------------------------------------------------------------------------------+


|method-ast_boost-SetupVariantMacro.apply|

.. das:function:: SetupVariantMacro.finish(self: AstStructureAnnotation; st: StructurePtr; group: ModuleGroup; args: AnnotationArgumentList const; errors: das_string)

finish returns bool

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+self    + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+--------+--------------------------------------------------------------------------------+
+st      + :ref:`StructurePtr <alias-StructurePtr>`                                       +
+--------+--------------------------------------------------------------------------------+
+group   + :ref:`rtti::ModuleGroup <handle-rtti-ModuleGroup>`                             +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`                         +
+--------+--------------------------------------------------------------------------------+


|method-ast_boost-SetupVariantMacro.finish|

.. das:function:: SetupVariantMacro.setup_call(self: SetupAnyAnnotation; st: StructurePtr; cll: smart_ptr<ast::ExprCall>)

+--------+----------------------------------------------------------------------------+
+argument+argument type                                                               +
+========+============================================================================+
+self    + :ref:`ast_boost::SetupAnyAnnotation <struct-ast_boost-SetupAnyAnnotation>` +
+--------+----------------------------------------------------------------------------+
+st      + :ref:`StructurePtr <alias-StructurePtr>`                                   +
+--------+----------------------------------------------------------------------------+
+cll     +smart_ptr< :ref:`ast::ExprCall <handle-ast-ExprCall>` >                     +
+--------+----------------------------------------------------------------------------+


|method-ast_boost-SetupVariantMacro.setup_call|

.. _struct-ast_boost-TagFunctionAnnotation:

.. das:attribute:: TagFunctionAnnotation : AstFunctionAnnotation

|class-ast_boost-TagFunctionAnnotation|

.. das:function:: TagFunctionAnnotation.transform(self: AstFunctionAnnotation; call: smart_ptr<ast::ExprCallFunc>; errors: das_string)

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


|method-ast_boost-TagFunctionAnnotation.transform|

.. das:function:: TagFunctionAnnotation.apply(self: AstFunctionAnnotation; func: FunctionPtr; group: ModuleGroup; args: AnnotationArgumentList const; errors: das_string)

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


|method-ast_boost-TagFunctionAnnotation.apply|

.. das:function:: TagFunctionAnnotation.finish(self: AstFunctionAnnotation; func: FunctionPtr; group: ModuleGroup; args: AnnotationArgumentList const; progArgs: AnnotationArgumentList const; errors: das_string)

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


|method-ast_boost-TagFunctionAnnotation.finish|

.. das:function:: TagFunctionAnnotation.isCompatible(self: AstFunctionAnnotation; func: FunctionPtr; types: VectorTypeDeclPtr; decl: AnnotationDeclaration const; errors: das_string)

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


|method-ast_boost-TagFunctionAnnotation.isCompatible|

.. das:function:: TagFunctionAnnotation.isSpecialized(self: AstFunctionAnnotation)

isSpecialized returns bool

|method-ast_boost-TagFunctionAnnotation.isSpecialized|

.. _struct-ast_boost-TagFunctionMacro:

.. das:attribute:: TagFunctionMacro : SetupAnyAnnotation

|class-ast_boost-TagFunctionMacro|

it defines as follows

  annotation_function_call : string
  name                     : string
  tag                      : string

.. das:function:: TagFunctionMacro.apply(self: AstStructureAnnotation; st: StructurePtr; group: ModuleGroup; args: AnnotationArgumentList const; errors: das_string)

apply returns bool

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+self    + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+--------+--------------------------------------------------------------------------------+
+st      + :ref:`StructurePtr <alias-StructurePtr>`                                       +
+--------+--------------------------------------------------------------------------------+
+group   + :ref:`rtti::ModuleGroup <handle-rtti-ModuleGroup>`                             +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`                         +
+--------+--------------------------------------------------------------------------------+


|method-ast_boost-TagFunctionMacro.apply|

.. das:function:: TagFunctionMacro.finish(self: AstStructureAnnotation; st: StructurePtr; group: ModuleGroup; args: AnnotationArgumentList const; errors: das_string)

finish returns bool

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+self    + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+--------+--------------------------------------------------------------------------------+
+st      + :ref:`StructurePtr <alias-StructurePtr>`                                       +
+--------+--------------------------------------------------------------------------------+
+group   + :ref:`rtti::ModuleGroup <handle-rtti-ModuleGroup>`                             +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`                         +
+--------+--------------------------------------------------------------------------------+


|method-ast_boost-TagFunctionMacro.finish|

.. das:function:: TagFunctionMacro.setup_call(self: SetupAnyAnnotation; st: StructurePtr; cll: smart_ptr<ast::ExprCall>)

+--------+----------------------------------------------------------------------------+
+argument+argument type                                                               +
+========+============================================================================+
+self    + :ref:`ast_boost::SetupAnyAnnotation <struct-ast_boost-SetupAnyAnnotation>` +
+--------+----------------------------------------------------------------------------+
+st      + :ref:`StructurePtr <alias-StructurePtr>`                                   +
+--------+----------------------------------------------------------------------------+
+cll     +smart_ptr< :ref:`ast::ExprCall <handle-ast-ExprCall>` >                     +
+--------+----------------------------------------------------------------------------+


|method-ast_boost-TagFunctionMacro.setup_call|

.. _struct-ast_boost-TagStructureAnnotation:

.. das:attribute:: TagStructureAnnotation : AstStructureAnnotation

|class-ast_boost-TagStructureAnnotation|

.. das:function:: TagStructureAnnotation.apply(self: AstStructureAnnotation; st: StructurePtr; group: ModuleGroup; args: AnnotationArgumentList const; errors: das_string)

apply returns bool

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+self    + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+--------+--------------------------------------------------------------------------------+
+st      + :ref:`StructurePtr <alias-StructurePtr>`                                       +
+--------+--------------------------------------------------------------------------------+
+group   + :ref:`rtti::ModuleGroup <handle-rtti-ModuleGroup>`                             +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`                         +
+--------+--------------------------------------------------------------------------------+


|method-ast_boost-TagStructureAnnotation.apply|

.. das:function:: TagStructureAnnotation.finish(self: AstStructureAnnotation; st: StructurePtr; group: ModuleGroup; args: AnnotationArgumentList const; errors: das_string)

finish returns bool

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+self    + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+--------+--------------------------------------------------------------------------------+
+st      + :ref:`StructurePtr <alias-StructurePtr>`                                       +
+--------+--------------------------------------------------------------------------------+
+group   + :ref:`rtti::ModuleGroup <handle-rtti-ModuleGroup>`                             +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`                         +
+--------+--------------------------------------------------------------------------------+


|method-ast_boost-TagStructureAnnotation.finish|

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_ast_boost_c__c_add_annotation_argument__hh_handle_hh_AnnotationArgumentList__hh_const_hh_string__hh_const_hh_bool:

.. das:function:: add_annotation_argument(arguments: AnnotationArgumentList; argName: string const; val: bool const)

add_annotation_argument returns int const

+---------+--------------------------------------------------------------------------+
+argument +argument type                                                             +
+=========+==========================================================================+
+arguments+ :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` +
+---------+--------------------------------------------------------------------------+
+argName  +string const                                                              +
+---------+--------------------------------------------------------------------------+
+val      +bool const                                                                +
+---------+--------------------------------------------------------------------------+


|function-ast_boost-add_annotation_argument|

.. _function-_at_ast_boost_c__c_add_annotation_argument__hh_handle_hh_AnnotationArgumentList__hh_const_hh_string__hh_const_hh_float:

.. das:function:: add_annotation_argument(arguments: AnnotationArgumentList; argName: string const; val: float const)

add_annotation_argument returns int const

+---------+--------------------------------------------------------------------------+
+argument +argument type                                                             +
+=========+==========================================================================+
+arguments+ :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` +
+---------+--------------------------------------------------------------------------+
+argName  +string const                                                              +
+---------+--------------------------------------------------------------------------+
+val      +float const                                                               +
+---------+--------------------------------------------------------------------------+


|function-ast_boost-add_annotation_argument|

.. _function-_at_ast_boost_c__c_add_annotation_argument__hh_handle_hh_AnnotationArgumentList__hh_const_hh_string__hh_const_hh_int:

.. das:function:: add_annotation_argument(arguments: AnnotationArgumentList; argName: string const; val: int const)

add_annotation_argument returns int const

+---------+--------------------------------------------------------------------------+
+argument +argument type                                                             +
+=========+==========================================================================+
+arguments+ :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` +
+---------+--------------------------------------------------------------------------+
+argName  +string const                                                              +
+---------+--------------------------------------------------------------------------+
+val      +int const                                                                 +
+---------+--------------------------------------------------------------------------+


|function-ast_boost-add_annotation_argument|

.. _function-_at_ast_boost_c__c_add_annotation_argument__hh_handle_hh_AnnotationArgumentList__hh_const_hh_string__hh_const_hh_string:

.. das:function:: add_annotation_argument(arguments: AnnotationArgumentList; argName: string const; val: string const)

add_annotation_argument returns int const

+---------+--------------------------------------------------------------------------+
+argument +argument type                                                             +
+=========+==========================================================================+
+arguments+ :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` +
+---------+--------------------------------------------------------------------------+
+argName  +string const                                                              +
+---------+--------------------------------------------------------------------------+
+val      +string const                                                              +
+---------+--------------------------------------------------------------------------+


|function-ast_boost-add_annotation_argument|

.. _function-_at_ast_boost_c__c_append_annotation__hh_smart_ptr_hh__hh_handle_hh_Function__hh_const_hh_string__hh_const_hh_string__hh_const_hh__hh_array_hh__hh_tuple_hh_string_hh__hh_variant_hh_bool_hh_int_hh_uint_hh_int64_hh_uint64_hh_float_hh_double_hh_string_hh_any_hh__rq_tBool_hh__rq_tInt_hh__rq_tUInt_hh__rq_tInt64_hh__rq_tUInt64_hh__rq_tFloat_hh__rq_tDouble_hh__rq_tString_hh__rq_nothing_hh__rq_argname_hh__rq_argvalue:

.. das:function:: append_annotation(func: FunctionPtr; mod_name: string const; ann_name: string const; args: array<tuple<argname:string;argvalue:variant<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any>>> const)

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+func    + :ref:`FunctionPtr <alias-FunctionPtr>`                                         +
+--------+--------------------------------------------------------------------------------+
+mod_name+string const                                                                    +
+--------+--------------------------------------------------------------------------------+
+ann_name+string const                                                                    +
+--------+--------------------------------------------------------------------------------+
+args    +array<tuple<argname:string;argvalue: :ref:`RttiValue <alias-RttiValue>` >> const+
+--------+--------------------------------------------------------------------------------+


|function-ast_boost-append_annotation|

.. _function-_at_ast_boost_c__c_describe__hh_const_hh__hh_handle_hh_AnnotationArgumentList:

.. das:function:: describe(list: AnnotationArgumentList const)

describe returns string const

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+list    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+


|function-ast_boost-describe|

.. _function-_at_ast_boost_c__c_describe__hh_const_hh__hh_handle_hh_AnnotationDeclaration:

.. das:function:: describe(ann: AnnotationDeclaration const)

describe returns string

+--------+------------------------------------------------------------------------------+
+argument+argument type                                                                 +
+========+==============================================================================+
+ann     + :ref:`rtti::AnnotationDeclaration <handle-rtti-AnnotationDeclaration>`  const+
+--------+------------------------------------------------------------------------------+


|function-ast_boost-describe|

.. _function-_at_ast_boost_c__c_describe__hh_const_hh__hh_handle_hh_AnnotationList:

.. das:function:: describe(list: AnnotationList const)

describe returns string const

+--------+----------------------------------------------------------------+
+argument+argument type                                                   +
+========+================================================================+
+list    + :ref:`rtti::AnnotationList <handle-rtti-AnnotationList>`  const+
+--------+----------------------------------------------------------------+


|function-ast_boost-describe|

.. _function-_at_ast_boost_c__c_describe_function_short__hh_const_hh__hh_smart_ptr_hh__hh_handle_hh_Function:

.. das:function:: describe_function_short(func: FunctionPtr)

describe_function_short returns string

+--------+----------------------------------------+
+argument+argument type                           +
+========+========================================+
+func    + :ref:`FunctionPtr <alias-FunctionPtr>` +
+--------+----------------------------------------+


|function-ast_boost-describe_function_short|

.. _function-_at_ast_boost_c__c_emplace_new__hh_handle_hh_MakeStruct__hh_smart_ptr_hh__hh_handle_hh_MakeFieldDecl:

.. das:function:: emplace_new(vec: MakeStruct; ptr: smart_ptr<ast::MakeFieldDecl>)

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+vec     + :ref:`ast::MakeStruct <handle-ast-MakeStruct>`                  +
+--------+-----------------------------------------------------------------+
+ptr     +smart_ptr< :ref:`ast::MakeFieldDecl <handle-ast-MakeFieldDecl>` >+
+--------+-----------------------------------------------------------------+


|function-ast_boost-emplace_new|

.. _function-_at_ast_boost_c__c_emplace_new__hh_handle_hh_dasvector_rq_smart_ptr_rq_Expression__hh_smart_ptr_hh__hh_handle_hh_Expression:

.. das:function:: emplace_new(vec: dasvector`smart_ptr`Expression; ptr: smart_ptr<ast::Expression>)

+--------+----------------------------------------------------------------------------------------+
+argument+argument type                                                                           +
+========+========================================================================================+
+vec     + :ref:`ast::dasvector`smart_ptr`Expression <handle-ast-dasvector`smart_ptr`Expression>` +
+--------+----------------------------------------------------------------------------------------+
+ptr     +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >                             +
+--------+----------------------------------------------------------------------------------------+


|function-ast_boost-emplace_new|

.. _function-_at_ast_boost_c__c_emplace_new__hh_handle_hh_dasvector_rq_smart_ptr_rq_TypeDecl__hh_smart_ptr_hh__hh_handle_hh_TypeDecl:

.. das:function:: emplace_new(vec: dasvector`smart_ptr`TypeDecl; ptr: smart_ptr<ast::TypeDecl>)

+--------+------------------------------------------------------------------------------------+
+argument+argument type                                                                       +
+========+====================================================================================+
+vec     + :ref:`ast::dasvector`smart_ptr`TypeDecl <handle-ast-dasvector`smart_ptr`TypeDecl>` +
+--------+------------------------------------------------------------------------------------+
+ptr     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                             +
+--------+------------------------------------------------------------------------------------+


|function-ast_boost-emplace_new|

.. _function-_at_ast_boost_c__c_emplace_new__hh_handle_hh_dasvector_rq_smart_ptr_rq_Variable__hh_smart_ptr_hh__hh_handle_hh_Variable:

.. das:function:: emplace_new(vec: dasvector`smart_ptr`Variable; ptr: smart_ptr<ast::Variable>)

+--------+------------------------------------------------------------------------------------+
+argument+argument type                                                                       +
+========+====================================================================================+
+vec     + :ref:`ast::dasvector`smart_ptr`Variable <handle-ast-dasvector`smart_ptr`Variable>` +
+--------+------------------------------------------------------------------------------------+
+ptr     +smart_ptr< :ref:`ast::Variable <handle-ast-Variable>` >                             +
+--------+------------------------------------------------------------------------------------+


|function-ast_boost-emplace_new|

.. _function-_at_ast_boost_c__c_find_annotation__hh_const_hh_string__hh_const_hh_string:

.. das:function:: find_annotation(mod_name: string const; ann_name: string const)

find_annotation returns  :ref:`rtti::Annotation <handle-rtti-Annotation>`  const?

+--------+-------------+
+argument+argument type+
+========+=============+
+mod_name+string const +
+--------+-------------+
+ann_name+string const +
+--------+-------------+


|function-ast_boost-find_annotation|

.. _function-_at_ast_boost_c__c_find_arg__hh_const_hh_string__hh_const_hh__hh_handle_hh_AnnotationArgumentList:

.. das:function:: find_arg(argn: string const; args: AnnotationArgumentList const)

find_arg returns  :ref:`RttiValue <alias-RttiValue>` 

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+argn    +string const                                                                    +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+


|function-ast_boost-find_arg|

.. _function-_at_ast_boost_c__c_find_unique_function__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module__hh_const_hh_string:

.. das:function:: find_unique_function(mod: rtti::Module? const; name: string const)

find_unique_function returns smart_ptr< :ref:`ast::Function <handle-ast-Function>` >

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const+
+--------+-------------------------------------------------+
+name    +string const                                     +
+--------+-------------------------------------------------+


|function-ast_boost-find_unique_function|

.. _function-_at_ast_boost_c__c_find_unique_generic__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module__hh_const_hh_string:

.. das:function:: find_unique_generic(mod: rtti::Module? const; name: string const)

find_unique_generic returns smart_ptr< :ref:`ast::Function <handle-ast-Function>` >

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const+
+--------+-------------------------------------------------+
+name    +string const                                     +
+--------+-------------------------------------------------+


|function-ast_boost-find_unique_generic|

.. _function-_at_ast_boost_c__c_isExpression__hh_const_hh__hh_smart_ptr_hh__hh_handle_hh_TypeDecl__hh_const_hh_bool:

.. das:function:: isExpression(t: TypeDeclPtr; top: bool const)

isExpression returns bool

+--------+----------------------------------------+
+argument+argument type                           +
+========+========================================+
+t       + :ref:`TypeDeclPtr <alias-TypeDeclPtr>` +
+--------+----------------------------------------+
+top     +bool const                              +
+--------+----------------------------------------+


|function-ast_boost-isExpression|

.. _function-_at_ast_boost_c__c_isVectorType__hh_const_hh__hh_enum_hh_rtti_c__c_Type_hh_int:

.. das:function:: isVectorType(typ: Type const)

isVectorType returns bool

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+typ     + :ref:`rtti::Type <enum-rtti-Type>`  const+
+--------+------------------------------------------+


|function-ast_boost-isVectorType|

.. _function-_at_ast_boost_c__c_is_class_method__hh_const_hh__hh_smart_ptr_hh__hh_handle_hh_Structure__hh_const_hh__hh_smart_ptr_hh__hh_handle_hh_TypeDecl:

.. das:function:: is_class_method(cinfo: StructurePtr; finfo: TypeDeclPtr)

is_class_method returns bool const

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+cinfo   + :ref:`StructurePtr <alias-StructurePtr>` +
+--------+------------------------------------------+
+finfo   + :ref:`TypeDeclPtr <alias-TypeDeclPtr>`   +
+--------+------------------------------------------+


|function-ast_boost-is_class_method|

.. _function-_at_ast_boost_c__c_is_same_or_inherited__hh_const_hh__hh_ptr_hh__hh_handle_hh_Structure__hh_const_hh__hh_ptr_hh__hh_handle_hh_Structure:

.. das:function:: is_same_or_inherited(parent: ast::Structure? const; child: ast::Structure? const)

is_same_or_inherited returns bool const

+--------+-----------------------------------------------------+
+argument+argument type                                        +
+========+=====================================================+
+parent  + :ref:`ast::Structure <handle-ast-Structure>` ? const+
+--------+-----------------------------------------------------+
+child   + :ref:`ast::Structure <handle-ast-Structure>` ? const+
+--------+-----------------------------------------------------+


|function-ast_boost-is_same_or_inherited|

.. _function-_at_ast_boost_c__c_override_method__hh_smart_ptr_hh__hh_handle_hh_Structure__hh_const_hh_string__hh_const_hh_string:

.. das:function:: override_method(str: StructurePtr; name: string const; funcName: string const)

override_method returns bool

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+str     + :ref:`StructurePtr <alias-StructurePtr>` +
+--------+------------------------------------------+
+name    +string const                              +
+--------+------------------------------------------+
+funcName+string const                              +
+--------+------------------------------------------+


|function-ast_boost-override_method|

.. _function-_at_ast_boost_c__c_setup_macro__hh_const_hh_string__hh_const_hh__hh_handle_hh_LineInfo:

.. das:function:: setup_macro(name: string const; at: LineInfo const)

setup_macro returns  :ref:`ast::ExprBlock <handle-ast-ExprBlock>` ?

+--------+----------------------------------------------------+
+argument+argument type                                       +
+========+====================================================+
+name    +string const                                        +
+--------+----------------------------------------------------+
+at      + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`  const+
+--------+----------------------------------------------------+


|function-ast_boost-setup_macro|

.. _function-_at_ast_boost_c__c_convert_to_expression__hh_ref_hh__hh_auto__hh_const_hh__hh_handle_hh_LineInfo:

.. das:function:: convert_to_expression(value: auto&; at: LineInfo const)

convert_to_expression returns auto

+--------+----------------------------------------------------+
+argument+argument type                                       +
+========+====================================================+
+value   +auto&                                               +
+--------+----------------------------------------------------+
+at      + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`  const+
+--------+----------------------------------------------------+


|function-ast_boost-convert_to_expression|

.. _function-_at_ast_boost_c__c_describe_bitfield__hh_const_hh__hh_auto__hh_const_hh_string:

.. das:function:: describe_bitfield(bf: auto const; merger: string const)

describe_bitfield returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+bf      +auto const   +
+--------+-------------+
+merger  +string const +
+--------+-------------+


|function-ast_boost-describe_bitfield|

.. _function-_at_ast_boost_c__c_setup_tag_annotation__hh_const_hh_string__hh_const_hh_string__hh_const_hh__hh_auto:

.. das:function:: setup_tag_annotation(name: string const; tag: string const; classPtr: auto const)

setup_tag_annotation returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+name    +string const +
+--------+-------------+
+tag     +string const +
+--------+-------------+
+classPtr+auto const   +
+--------+-------------+


|function-ast_boost-setup_tag_annotation|


