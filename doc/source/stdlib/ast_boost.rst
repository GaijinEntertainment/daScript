
.. _stdlib_ast_boost:

=========================
Boost package for the AST
=========================

.. include:: detail/ast_boost.rst

|module-ast_boost|

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

.. das:function:: MacroMacro.transform(self: AstFunctionAnnotation; call: smart_ptr<ast::ExprCall>; errors: das_string)

transform returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+----------------------------------------------------------------------+
+argument+argument type                                                         +
+========+======================================================================+
+self    + :ref:`ast::AstFunctionAnnotation <struct-ast-AstFunctionAnnotation>` +
+--------+----------------------------------------------------------------------+
+call    +smart_ptr< :ref:`ast::ExprCall <handle-ast-ExprCall>` >               +
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
+group   + :ref:`ast::ModuleGroup <handle-ast-ModuleGroup>`                               +
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
+group   + :ref:`ast::ModuleGroup <handle-ast-ModuleGroup>`                               +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+progArgs+ :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`                         +
+--------+--------------------------------------------------------------------------------+


|method-ast_boost-MacroMacro.finish|

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
+group   + :ref:`ast::ModuleGroup <handle-ast-ModuleGroup>`                               +
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
+group   + :ref:`ast::ModuleGroup <handle-ast-ModuleGroup>`                               +
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
+group   + :ref:`ast::ModuleGroup <handle-ast-ModuleGroup>`                               +
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
+group   + :ref:`ast::ModuleGroup <handle-ast-ModuleGroup>`                               +
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
+group   + :ref:`ast::ModuleGroup <handle-ast-ModuleGroup>`                               +
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
+group   + :ref:`ast::ModuleGroup <handle-ast-ModuleGroup>`                               +
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
+group   + :ref:`ast::ModuleGroup <handle-ast-ModuleGroup>`                               +
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
+group   + :ref:`ast::ModuleGroup <handle-ast-ModuleGroup>`                               +
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
+group   + :ref:`ast::ModuleGroup <handle-ast-ModuleGroup>`                               +
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
+group   + :ref:`ast::ModuleGroup <handle-ast-ModuleGroup>`                               +
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

.. das:function:: TagFunctionAnnotation.transform(self: AstFunctionAnnotation; call: smart_ptr<ast::ExprCall>; errors: das_string)

transform returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+----------------------------------------------------------------------+
+argument+argument type                                                         +
+========+======================================================================+
+self    + :ref:`ast::AstFunctionAnnotation <struct-ast-AstFunctionAnnotation>` +
+--------+----------------------------------------------------------------------+
+call    +smart_ptr< :ref:`ast::ExprCall <handle-ast-ExprCall>` >               +
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
+group   + :ref:`ast::ModuleGroup <handle-ast-ModuleGroup>`                               +
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
+group   + :ref:`ast::ModuleGroup <handle-ast-ModuleGroup>`                               +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+progArgs+ :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`                         +
+--------+--------------------------------------------------------------------------------+


|method-ast_boost-TagFunctionAnnotation.finish|

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
+group   + :ref:`ast::ModuleGroup <handle-ast-ModuleGroup>`                               +
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
+group   + :ref:`ast::ModuleGroup <handle-ast-ModuleGroup>`                               +
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
+group   + :ref:`ast::ModuleGroup <handle-ast-ModuleGroup>`                               +
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
+group   + :ref:`ast::ModuleGroup <handle-ast-ModuleGroup>`                               +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`                         +
+--------+--------------------------------------------------------------------------------+


|method-ast_boost-TagStructureAnnotation.finish|

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_ast_boost_c__c_describe__hh_handle_hh_AnnotationArgumentList_hh_const:

.. das:function:: describe(list: AnnotationArgumentList const)

describe returns string const

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+list    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+


|function-ast_boost-describe|

.. _function-_at_ast_boost_c__c_describe__hh_handle_hh_AnnotationDeclaration_hh_const:

.. das:function:: describe(ann: AnnotationDeclaration const)

describe returns string

+--------+------------------------------------------------------------------------------+
+argument+argument type                                                                 +
+========+==============================================================================+
+ann     + :ref:`rtti::AnnotationDeclaration <handle-rtti-AnnotationDeclaration>`  const+
+--------+------------------------------------------------------------------------------+


|function-ast_boost-describe|

.. _function-_at_ast_boost_c__c_describe__hh_handle_hh_AnnotationList_hh_const:

.. das:function:: describe(list: AnnotationList const)

describe returns string const

+--------+----------------------------------------------------------------+
+argument+argument type                                                   +
+========+================================================================+
+list    + :ref:`rtti::AnnotationList <handle-rtti-AnnotationList>`  const+
+--------+----------------------------------------------------------------+


|function-ast_boost-describe|

.. _function-_at_ast_boost_c__c_describe_function_short__hh_smart_ptr_hh__hh_handle_hh_Function_hh_const:

.. das:function:: describe_function_short(func: FunctionPtr)

describe_function_short returns string

+--------+----------------------------------------+
+argument+argument type                           +
+========+========================================+
+func    + :ref:`FunctionPtr <alias-FunctionPtr>` +
+--------+----------------------------------------+


|function-ast_boost-describe_function_short|

.. _function-_at_ast_boost_c__c_find_arg_string_hh_const__hh_handle_hh_AnnotationArgumentList_hh_const:

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

.. _function-_at_ast_boost_c__c_isExpression__hh_smart_ptr_hh__hh_handle_hh_TypeDecl_hh_const_bool_hh_const:

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

.. _function-_at_ast_boost_c__c_is_class_method__hh_smart_ptr_hh__hh_handle_hh_Structure_hh_const__hh_smart_ptr_hh__hh_handle_hh_TypeDecl_hh_const:

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

.. _function-_at_ast_boost_c__c_is_same_or_inherited__hh_ptr_hh__hh_handle_hh_Structure_hh_const__hh_ptr_hh__hh_handle_hh_Structure_hh_const:

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

.. _function-_at_ast_boost_c__c_setup_macro_string_hh_const__hh_handle_hh_LineInfo_hh_const:

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

.. _function-_at_ast_boost_c__c_convert_to_expression__hh_auto_hh_ref__hh_handle_hh_LineInfo_hh_const:

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

.. _function-_at_ast_boost_c__c_describe_bitfield__hh_auto_hh_const_string_hh_const:

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

.. _function-_at_ast_boost_c__c_setup_tag_annotation_string_hh_const_string_hh_const__hh_auto_hh_const:

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


