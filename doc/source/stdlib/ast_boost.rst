
.. _stdlib_ast_boost:

=========================
Boost package for the AST
=========================

.. include:: detail/ast_boost.rst

The AST boost module implements collection of helper macros and functions to accompany :ref:`AST <stdlib_ast>`.

All functions and symbols are in "ast_boost" module, use require to get access to it. ::

    require daslib/ast_boost


++++++++++++
Type aliases
++++++++++++

.. _alias-AnnotationDeclarationPtr:

.. das:attribute:: AnnotationDeclarationPtr = smart_ptr<AnnotationDeclaration>

|typedef-ast_boost-AnnotationDeclarationPtr|

.. _alias-DebugExpressionFlags:

.. das:attribute:: DebugExpressionFlags is a bitfield

+--------+---+-----+
+field   +bit+value+
+========+===+=====+
+refCount+0  +1    +
+--------+---+-----+


|typedef-ast_boost-DebugExpressionFlags|

++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-ast_boost-macro:

.. das:attribute:: macro

|function_annotation-ast_boost-macro|

.. _handle-ast_boost-tag_function:

.. das:attribute:: tag_function

|function_annotation-ast_boost-tag_function|

++++++++++++++
Variant macros
++++++++++++++

.. _call-macro-ast_boost-better_rtti_in_expr:

.. das:attribute:: better_rtti_in_expr

|variant_macro-ast_boost-better_rtti_in_expr|

++++++++++++++++
Structure macros
++++++++++++++++

.. _handle-ast_boost-function_macro:

.. das:attribute:: function_macro

|structure_macro-ast_boost-function_macro|

.. _handle-ast_boost-block_macro:

.. das:attribute:: block_macro

|structure_macro-ast_boost-block_macro|

.. _handle-ast_boost-structure_macro:

.. das:attribute:: structure_macro

|structure_macro-ast_boost-structure_macro|

.. _handle-ast_boost-enumeration_macro:

.. das:attribute:: enumeration_macro

|structure_macro-ast_boost-enumeration_macro|

.. _handle-ast_boost-contract:

.. das:attribute:: contract

|structure_macro-ast_boost-contract|

.. _handle-ast_boost-reader_macro:

.. das:attribute:: reader_macro

|structure_macro-ast_boost-reader_macro|

.. _handle-ast_boost-comment_reader:

.. das:attribute:: comment_reader

|structure_macro-ast_boost-comment_reader|

.. _handle-ast_boost-call_macro:

.. das:attribute:: call_macro

|structure_macro-ast_boost-call_macro|

.. _handle-ast_boost-typeinfo_macro:

.. das:attribute:: typeinfo_macro

|structure_macro-ast_boost-typeinfo_macro|

.. _handle-ast_boost-variant_macro:

.. das:attribute:: variant_macro

|structure_macro-ast_boost-variant_macro|

.. _handle-ast_boost-for_loop_macro:

.. das:attribute:: for_loop_macro

|structure_macro-ast_boost-for_loop_macro|

.. _handle-ast_boost-capture_macro:

.. das:attribute:: capture_macro

|structure_macro-ast_boost-capture_macro|

.. _handle-ast_boost-type_macro:

.. das:attribute:: type_macro

|structure_macro-ast_boost-type_macro|

.. _handle-ast_boost-simulate_macro:

.. das:attribute:: simulate_macro

|structure_macro-ast_boost-simulate_macro|

.. _handle-ast_boost-tag_structure:

.. das:attribute:: tag_structure

|structure_macro-ast_boost-tag_structure|

.. _handle-ast_boost-tag_function_macro:

.. das:attribute:: tag_function_macro

|structure_macro-ast_boost-tag_function_macro|

.. _handle-ast_boost-infer_macro:

.. das:attribute:: infer_macro

|structure_macro-ast_boost-infer_macro|

.. _handle-ast_boost-dirty_infer_macro:

.. das:attribute:: dirty_infer_macro

|structure_macro-ast_boost-dirty_infer_macro|

.. _handle-ast_boost-optimization_macro:

.. das:attribute:: optimization_macro

|structure_macro-ast_boost-optimization_macro|

.. _handle-ast_boost-lint_macro:

.. das:attribute:: lint_macro

|structure_macro-ast_boost-lint_macro|

.. _handle-ast_boost-global_lint_macro:

.. das:attribute:: global_lint_macro

|structure_macro-ast_boost-global_lint_macro|

+++++++
Classes
+++++++

.. _struct-ast_boost-MacroMacro:

.. das:attribute:: MacroMacro : AstFunctionAnnotation

|class-ast_boost-MacroMacro|

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

.. _struct-ast_boost-TagFunctionAnnotation:

.. das:attribute:: TagFunctionAnnotation : AstFunctionAnnotation

|class-ast_boost-TagFunctionAnnotation|

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

.. _struct-ast_boost-SetupAnyAnnotation:

.. das:attribute:: SetupAnyAnnotation : AstStructureAnnotation

|class-ast_boost-SetupAnyAnnotation|

it defines as follows

  | annotation_function_call : string
  | name                     : string

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

.. das:function:: SetupAnyAnnotation.setup_call(self: SetupAnyAnnotation; st: StructurePtr; cll: smart_ptr<ExprCall>)

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

.. _struct-ast_boost-SetupFunctionAnnotation:

.. das:attribute:: SetupFunctionAnnotation : SetupAnyAnnotation

|class-ast_boost-SetupFunctionAnnotation|

it defines as follows

  | annotation_function_call : string
  | name                     : string

.. _struct-ast_boost-SetupBlockAnnotation:

.. das:attribute:: SetupBlockAnnotation : SetupAnyAnnotation

|class-ast_boost-SetupBlockAnnotation|

it defines as follows

  | annotation_function_call : string
  | name                     : string

.. _struct-ast_boost-SetupStructureAnnotation:

.. das:attribute:: SetupStructureAnnotation : SetupAnyAnnotation

|class-ast_boost-SetupStructureAnnotation|

it defines as follows

  | annotation_function_call : string
  | name                     : string

.. _struct-ast_boost-SetupEnumerationAnnotation:

.. das:attribute:: SetupEnumerationAnnotation : SetupAnyAnnotation

|class-ast_boost-SetupEnumerationAnnotation|

it defines as follows

  | annotation_function_call : string
  | name                     : string

.. _struct-ast_boost-SetupContractAnnotation:

.. das:attribute:: SetupContractAnnotation : SetupAnyAnnotation

|class-ast_boost-SetupContractAnnotation|

it defines as follows

  | annotation_function_call : string
  | name                     : string

.. _struct-ast_boost-SetupReaderMacro:

.. das:attribute:: SetupReaderMacro : SetupAnyAnnotation

|class-ast_boost-SetupReaderMacro|

it defines as follows

  | annotation_function_call : string
  | name                     : string

.. _struct-ast_boost-SetupCommentReader:

.. das:attribute:: SetupCommentReader : SetupAnyAnnotation

|class-ast_boost-SetupCommentReader|

it defines as follows

  | annotation_function_call : string
  | name                     : string

.. _struct-ast_boost-SetupVariantMacro:

.. das:attribute:: SetupVariantMacro : SetupAnyAnnotation

|class-ast_boost-SetupVariantMacro|

it defines as follows

  | annotation_function_call : string
  | name                     : string

.. _struct-ast_boost-SetupForLoopMacro:

.. das:attribute:: SetupForLoopMacro : SetupAnyAnnotation

|class-ast_boost-SetupForLoopMacro|

it defines as follows

  | annotation_function_call : string
  | name                     : string

.. _struct-ast_boost-SetupCaptureMacro:

.. das:attribute:: SetupCaptureMacro : SetupAnyAnnotation

|class-ast_boost-SetupCaptureMacro|

it defines as follows

  | annotation_function_call : string
  | name                     : string

.. _struct-ast_boost-SetupTypeMacro:

.. das:attribute:: SetupTypeMacro : SetupAnyAnnotation

|class-ast_boost-SetupTypeMacro|

it defines as follows

  | annotation_function_call : string
  | name                     : string

.. _struct-ast_boost-SetupSimulateMacro:

.. das:attribute:: SetupSimulateMacro : SetupAnyAnnotation

|class-ast_boost-SetupSimulateMacro|

it defines as follows

  | annotation_function_call : string
  | name                     : string

.. _struct-ast_boost-SetupCallMacro:

.. das:attribute:: SetupCallMacro : SetupAnyAnnotation

|class-ast_boost-SetupCallMacro|

it defines as follows

  | annotation_function_call : string
  | name                     : string

.. _struct-ast_boost-SetupTypeInfoMacro:

.. das:attribute:: SetupTypeInfoMacro : SetupAnyAnnotation

|class-ast_boost-SetupTypeInfoMacro|

it defines as follows

  | annotation_function_call : string
  | name                     : string

.. _struct-ast_boost-SetupInferMacro:

.. das:attribute:: SetupInferMacro : SetupAnyAnnotation

|class-ast_boost-SetupInferMacro|

it defines as follows

  | annotation_function_call : string
  | name                     : string

.. _struct-ast_boost-SetupDirtyInferMacro:

.. das:attribute:: SetupDirtyInferMacro : SetupAnyAnnotation

|class-ast_boost-SetupDirtyInferMacro|

it defines as follows

  | annotation_function_call : string
  | name                     : string

.. _struct-ast_boost-SetupLintMacro:

.. das:attribute:: SetupLintMacro : SetupAnyAnnotation

|class-ast_boost-SetupLintMacro|

it defines as follows

  | annotation_function_call : string
  | name                     : string

.. _struct-ast_boost-SetupGlobalLintMacro:

.. das:attribute:: SetupGlobalLintMacro : SetupAnyAnnotation

|class-ast_boost-SetupGlobalLintMacro|

it defines as follows

  | annotation_function_call : string
  | name                     : string

.. _struct-ast_boost-SetupOptimizationMacro:

.. das:attribute:: SetupOptimizationMacro : SetupAnyAnnotation

|class-ast_boost-SetupOptimizationMacro|

it defines as follows

  | annotation_function_call : string
  | name                     : string

.. _struct-ast_boost-TagFunctionMacro:

.. das:attribute:: TagFunctionMacro : SetupAnyAnnotation

|class-ast_boost-TagFunctionMacro|

it defines as follows

  | annotation_function_call : string
  | name                     : string
  | tag                      : string

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

.. das:function:: TagFunctionMacro.setup_call(self: SetupAnyAnnotation; st: StructurePtr; cll: smart_ptr<ExprCall>)

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

.. _struct-ast_boost-BetterRttiVisitor:

.. das:attribute:: BetterRttiVisitor : AstVariantMacro

|class-ast_boost-BetterRttiVisitor|

.. das:function:: BetterRttiVisitor.visitExprIsVariant(self: AstVariantMacro; prog: ProgramPtr; mod: Module? const; expr: smart_ptr<ExprIsVariant> const)

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

.. das:function:: BetterRttiVisitor.visitExprAsVariant(self: AstVariantMacro; prog: ProgramPtr; mod: Module? const; expr: smart_ptr<ExprAsVariant> const)

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

.. das:function:: BetterRttiVisitor.visitExprSafeAsVariant(self: AstVariantMacro; prog: ProgramPtr; mod: Module? const; expr: smart_ptr<ExprSafeAsVariant> const)

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

++++++++++
Containers
++++++++++

  *  :ref:`emplace_new (vec:$::dasvector`smart_ptr`Expression -const;ptr:smart_ptr\<ast::Expression\> -const) : void <function-_at_ast_boost_c__c_emplace_new_H_ls__builtin__c__c_dasvector_rq_smart_ptr_rq_Expression_gr__1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M>` 
  *  :ref:`emplace_new (vec:$::dasvector`smart_ptr`TypeDecl -const;ptr:smart_ptr\<ast::TypeDecl\> -const) : void <function-_at_ast_boost_c__c_emplace_new_H_ls__builtin__c__c_dasvector_rq_smart_ptr_rq_TypeDecl_gr__1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M>` 
  *  :ref:`emplace_new (vec:$::dasvector`smart_ptr`Variable -const;ptr:smart_ptr\<ast::Variable\> -const) : void <function-_at_ast_boost_c__c_emplace_new_H_ls__builtin__c__c_dasvector_rq_smart_ptr_rq_Variable_gr__1_ls_H_ls_ast_c__c_Variable_gr__gr__qm_M>` 
  *  :ref:`emplace_new (vec:ast::MakeStruct -const;ptr:smart_ptr\<ast::MakeFieldDecl\> -const) : void <function-_at_ast_boost_c__c_emplace_new_H_ls_ast_c__c_MakeStruct_gr__1_ls_H_ls_ast_c__c_MakeFieldDecl_gr__gr__qm_M>` 

.. _function-_at_ast_boost_c__c_emplace_new_H_ls__builtin__c__c_dasvector_rq_smart_ptr_rq_Expression_gr__1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M:

.. das:function:: emplace_new(vec: dasvector`smart_ptr`Expression; ptr: smart_ptr<Expression>)

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+vec     +vector<smart_ptr<Expression>>                              +
+--------+-----------------------------------------------------------+
+ptr     +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+--------+-----------------------------------------------------------+


|function-ast_boost-emplace_new|

.. _function-_at_ast_boost_c__c_emplace_new_H_ls__builtin__c__c_dasvector_rq_smart_ptr_rq_TypeDecl_gr__1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M:

.. das:function:: emplace_new(vec: dasvector`smart_ptr`TypeDecl; ptr: smart_ptr<TypeDecl>)

+--------+-------------------------------------------------------+
+argument+argument type                                          +
+========+=======================================================+
+vec     +vector<smart_ptr<TypeDecl>>                            +
+--------+-------------------------------------------------------+
+ptr     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >+
+--------+-------------------------------------------------------+


|function-ast_boost-emplace_new|

.. _function-_at_ast_boost_c__c_emplace_new_H_ls__builtin__c__c_dasvector_rq_smart_ptr_rq_Variable_gr__1_ls_H_ls_ast_c__c_Variable_gr__gr__qm_M:

.. das:function:: emplace_new(vec: dasvector`smart_ptr`Variable; ptr: smart_ptr<Variable>)

+--------+-------------------------------------------------------+
+argument+argument type                                          +
+========+=======================================================+
+vec     +vector<smart_ptr<Variable>>                            +
+--------+-------------------------------------------------------+
+ptr     +smart_ptr< :ref:`ast::Variable <handle-ast-Variable>` >+
+--------+-------------------------------------------------------+


|function-ast_boost-emplace_new|

.. _function-_at_ast_boost_c__c_emplace_new_H_ls_ast_c__c_MakeStruct_gr__1_ls_H_ls_ast_c__c_MakeFieldDecl_gr__gr__qm_M:

.. das:function:: emplace_new(vec: MakeStruct; ptr: smart_ptr<MakeFieldDecl>)

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+vec     + :ref:`ast::MakeStruct <handle-ast-MakeStruct>`                  +
+--------+-----------------------------------------------------------------+
+ptr     +smart_ptr< :ref:`ast::MakeFieldDecl <handle-ast-MakeFieldDecl>` >+
+--------+-----------------------------------------------------------------+


|function-ast_boost-emplace_new|

+++++++++++++++++++++++++++++++++++
Textual descriptions of the objects
+++++++++++++++++++++++++++++++++++

  *  :ref:`describe (list:rtti::AnnotationArgumentList const) : string const <function-_at_ast_boost_c__c_describe_CH_ls_rtti_c__c_AnnotationArgumentList_gr_>` 
  *  :ref:`describe (ann:rtti::AnnotationDeclaration const) : string const <function-_at_ast_boost_c__c_describe_CH_ls_rtti_c__c_AnnotationDeclaration_gr_>` 
  *  :ref:`describe (list:rtti::AnnotationList const) : string const <function-_at_ast_boost_c__c_describe_CH_ls_rtti_c__c_AnnotationList_gr_>` 
  *  :ref:`describe (vvar:smart_ptr\<ast::Variable\> const) : string const <function-_at_ast_boost_c__c_describe_CY_ls_VariablePtr_gr_1_ls_H_ls_ast_c__c_Variable_gr__gr__qm_M>` 
  *  :ref:`debug_expression (expr:smart_ptr\<ast::Expression\> const;deFlags:bitfield\<refCount\> const) : string const <function-_at_ast_boost_c__c_debug_expression_CY_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M_CY_ls_DebugExpressionFlags_gr_N_ls_refCount_gr_t>` 
  *  :ref:`debug_expression (expr:ast::Expression? const) : string const <function-_at_ast_boost_c__c_debug_expression_C1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_>` 
  *  :ref:`describe (expr:ast::Expression? const) : string const <function-_at_ast_boost_c__c_describe_C1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_>` 
  *  :ref:`describe_bitfield (bf:auto const;merger:string const) : auto <function-_at_ast_boost_c__c_describe_bitfield_C._Cs>` 
  *  :ref:`describe_function_short (func:smart_ptr\<ast::Function\> const|ast::Function? const const) : auto <function-_at_ast_boost_c__c_describe_function_short_C0_ls_CY_ls_FunctionPtr_gr_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M;C1_ls_H_ls_ast_c__c_Function_gr__gr__qm__gr_|>` 

.. _function-_at_ast_boost_c__c_describe_CH_ls_rtti_c__c_AnnotationArgumentList_gr_:

.. das:function:: describe(list: AnnotationArgumentList const)

describe returns string const

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+list    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+


|function-ast_boost-describe|

.. _function-_at_ast_boost_c__c_describe_CH_ls_rtti_c__c_AnnotationDeclaration_gr_:

.. das:function:: describe(ann: AnnotationDeclaration const)

describe returns string const

+--------+------------------------------------------------------------------------------+
+argument+argument type                                                                 +
+========+==============================================================================+
+ann     + :ref:`rtti::AnnotationDeclaration <handle-rtti-AnnotationDeclaration>`  const+
+--------+------------------------------------------------------------------------------+


|function-ast_boost-describe|

.. _function-_at_ast_boost_c__c_describe_CH_ls_rtti_c__c_AnnotationList_gr_:

.. das:function:: describe(list: AnnotationList const)

describe returns string const

+--------+----------------------------------------------------------------+
+argument+argument type                                                   +
+========+================================================================+
+list    + :ref:`rtti::AnnotationList <handle-rtti-AnnotationList>`  const+
+--------+----------------------------------------------------------------+


|function-ast_boost-describe|

.. _function-_at_ast_boost_c__c_describe_CY_ls_VariablePtr_gr_1_ls_H_ls_ast_c__c_Variable_gr__gr__qm_M:

.. das:function:: describe(vvar: VariablePtr)

describe returns string const

+--------+----------------------------------------+
+argument+argument type                           +
+========+========================================+
+vvar    + :ref:`VariablePtr <alias-VariablePtr>` +
+--------+----------------------------------------+


|function-ast_boost-describe|

.. _function-_at_ast_boost_c__c_debug_expression_CY_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M_CY_ls_DebugExpressionFlags_gr_N_ls_refCount_gr_t:

.. das:function:: debug_expression(expr: ExpressionPtr; deFlags: DebugExpressionFlags)

debug_expression returns string const

+--------+----------------------------------------------------------+
+argument+argument type                                             +
+========+==========================================================+
+expr    + :ref:`ExpressionPtr <alias-ExpressionPtr>`               +
+--------+----------------------------------------------------------+
+deFlags + :ref:`DebugExpressionFlags <alias-DebugExpressionFlags>` +
+--------+----------------------------------------------------------+


|function-ast_boost-debug_expression|

.. _function-_at_ast_boost_c__c_debug_expression_C1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_:

.. das:function:: debug_expression(expr: Expression? const)

debug_expression returns string const

+--------+-------------------------------------------------------+
+argument+argument type                                          +
+========+=======================================================+
+expr    + :ref:`ast::Expression <handle-ast-Expression>` ? const+
+--------+-------------------------------------------------------+


|function-ast_boost-debug_expression|

.. _function-_at_ast_boost_c__c_describe_C1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_:

.. das:function:: describe(expr: Expression? const)

describe returns string const

+--------+-------------------------------------------------------+
+argument+argument type                                          +
+========+=======================================================+
+expr    + :ref:`ast::Expression <handle-ast-Expression>` ? const+
+--------+-------------------------------------------------------+


|function-ast_boost-describe|

.. _function-_at_ast_boost_c__c_describe_bitfield_C._Cs:

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

.. _function-_at_ast_boost_c__c_describe_function_short_C0_ls_CY_ls_FunctionPtr_gr_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M;C1_ls_H_ls_ast_c__c_Function_gr__gr__qm__gr_|:

.. das:function:: describe_function_short(func: smart_ptr<Function> const|Function? const const)

describe_function_short returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+func    +option const +
+--------+-------------+


|function-ast_boost-describe_function_short|

+++++++
Queries
+++++++

  *  :ref:`isVectorType (typ:rtti::Type const) : bool const <function-_at_ast_boost_c__c_isVectorType_CE_ls_rtti_c__c_Type_gr_>` 
  *  :ref:`isExpression (t:smart_ptr\<ast::TypeDecl\> const;top:bool const) : bool const <function-_at_ast_boost_c__c_isExpression_CY_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_Cb>` 
  *  :ref:`is_same_or_inherited (parent:ast::Structure const? const;child:ast::Structure const? const) : bool const <function-_at_ast_boost_c__c_is_same_or_inherited_C1_ls_CH_ls_ast_c__c_Structure_gr__gr__qm__C1_ls_CH_ls_ast_c__c_Structure_gr__gr__qm_>` 
  *  :ref:`is_class_method (cinfo:smart_ptr\<ast::Structure\> const;finfo:smart_ptr\<ast::TypeDecl\> const) : bool const <function-_at_ast_boost_c__c_is_class_method_CY_ls_StructurePtr_gr_1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_M_CY_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M>` 
  *  :ref:`find_arg (argn:string const;args:rtti::AnnotationArgumentList const) : variant\<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any\> <function-_at_ast_boost_c__c_find_arg_Cs_CH_ls_rtti_c__c_AnnotationArgumentList_gr_>` 
  *  :ref:`find_arg (args:rtti::AnnotationArgumentList const;argn:string const) : variant\<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any\> <function-_at_ast_boost_c__c_find_arg_CH_ls_rtti_c__c_AnnotationArgumentList_gr__Cs>` 
  *  :ref:`find_unique_function (mod:rtti::Module? const;name:string const;canfail:bool const) : smart_ptr\<ast::Function\> <function-_at_ast_boost_c__c_find_unique_function_C1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__Cs_Cb>` 
  *  :ref:`find_unique_generic (mod:rtti::Module? const;name:string const;canfail:bool const) : smart_ptr\<ast::Function\> <function-_at_ast_boost_c__c_find_unique_generic_C1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__Cs_Cb>` 
  *  :ref:`find_annotation (mod_name:string const;ann_name:string const) : rtti::Annotation const? <function-_at_ast_boost_c__c_find_annotation_Cs_Cs>` 
  *  :ref:`get_for_source_index (expr:smart_ptr\<ast::ExprFor\> const;svar:smart_ptr\<ast::Variable\> const) : int const <function-_at_ast_boost_c__c_get_for_source_index_C1_ls_H_ls_ast_c__c_ExprFor_gr__gr__qm_M_CY_ls_VariablePtr_gr_1_ls_H_ls_ast_c__c_Variable_gr__gr__qm_M>` 
  *  :ref:`get_for_source_index (expr:smart_ptr\<ast::ExprFor\> const;source:smart_ptr\<ast::Expression\> const) : int const <function-_at_ast_boost_c__c_get_for_source_index_C1_ls_H_ls_ast_c__c_ExprFor_gr__gr__qm_M_CY_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M>` 
  *  :ref:`isCMRES (fun:smart_ptr\<ast::Function\> const) : bool const <function-_at_ast_boost_c__c_isCMRES_CY_ls_FunctionPtr_gr_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M>` 
  *  :ref:`isCMRES (fun:ast::Function? const) : bool const <function-_at_ast_boost_c__c_isCMRES_C1_ls_H_ls_ast_c__c_Function_gr__gr__qm_>` 
  *  :ref:`isMakeLocal (expr:smart_ptr\<ast::Expression\> const) : bool const <function-_at_ast_boost_c__c_isMakeLocal_CY_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M>` 
  *  :ref:`isExprCallFunc (expr:smart_ptr\<ast::Expression\> const) : bool const <function-_at_ast_boost_c__c_isExprCallFunc_CY_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M>` 
  *  :ref:`get_workhorse_types () : rtti::Type[31] <function-_at_ast_boost_c__c_get_workhorse_types>` 
  *  :ref:`find_argument_index (typ:smart_ptr\<ast::TypeDecl\> const;name:string const) : int const <function-_at_ast_boost_c__c_find_argument_index_CY_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_Cs>` 
  *  :ref:`isCMRESType (blockT:smart_ptr\<ast::TypeDecl\> const) : bool const <function-_at_ast_boost_c__c_isCMRESType_CY_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M>` 
  *  :ref:`getVectorElementCount (bt:rtti::Type const) : int const <function-_at_ast_boost_c__c_getVectorElementCount_CE_ls_rtti_c__c_Type_gr_>` 
  *  :ref:`getVectorElementSize (bt:rtti::Type const) : int const <function-_at_ast_boost_c__c_getVectorElementSize_CE_ls_rtti_c__c_Type_gr_>` 
  *  :ref:`getVectorElementType (bt:rtti::Type const) : rtti::Type const <function-_at_ast_boost_c__c_getVectorElementType_CE_ls_rtti_c__c_Type_gr_>` 
  *  :ref:`getVectorOffset (bt:rtti::Type const;ident:string const) : int const <function-_at_ast_boost_c__c_getVectorOffset_CE_ls_rtti_c__c_Type_gr__Cs>` 

.. _function-_at_ast_boost_c__c_isVectorType_CE_ls_rtti_c__c_Type_gr_:

.. das:function:: isVectorType(typ: Type const)

isVectorType returns bool const

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+typ     + :ref:`rtti::Type <enum-rtti-Type>`  const+
+--------+------------------------------------------+


|function-ast_boost-isVectorType|

.. _function-_at_ast_boost_c__c_isExpression_CY_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_Cb:

.. das:function:: isExpression(t: TypeDeclPtr; top: bool const)

isExpression returns bool const

+--------+----------------------------------------+
+argument+argument type                           +
+========+========================================+
+t       + :ref:`TypeDeclPtr <alias-TypeDeclPtr>` +
+--------+----------------------------------------+
+top     +bool const                              +
+--------+----------------------------------------+


|function-ast_boost-isExpression|

.. _function-_at_ast_boost_c__c_is_same_or_inherited_C1_ls_CH_ls_ast_c__c_Structure_gr__gr__qm__C1_ls_CH_ls_ast_c__c_Structure_gr__gr__qm_:

.. das:function:: is_same_or_inherited(parent: Structure const? const; child: Structure const? const)

is_same_or_inherited returns bool const

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+parent  + :ref:`ast::Structure <handle-ast-Structure>`  const? const+
+--------+-----------------------------------------------------------+
+child   + :ref:`ast::Structure <handle-ast-Structure>`  const? const+
+--------+-----------------------------------------------------------+


|function-ast_boost-is_same_or_inherited|

.. _function-_at_ast_boost_c__c_is_class_method_CY_ls_StructurePtr_gr_1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_M_CY_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M:

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

.. _function-_at_ast_boost_c__c_find_arg_Cs_CH_ls_rtti_c__c_AnnotationArgumentList_gr_:

.. das:function:: find_arg(argn: string const; args: AnnotationArgumentList const)

find_arg returns  :ref:`RttiValue <alias-RttiValue>` 

.. warning:: 
  This function is deprecated.

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+argn    +string const                                                                    +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+


|function-ast_boost-find_arg|

.. _function-_at_ast_boost_c__c_find_arg_CH_ls_rtti_c__c_AnnotationArgumentList_gr__Cs:

.. das:function:: find_arg(args: AnnotationArgumentList const; argn: string const)

find_arg returns  :ref:`RttiValue <alias-RttiValue>` 

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+args    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+argn    +string const                                                                    +
+--------+--------------------------------------------------------------------------------+


|function-ast_boost-find_arg|

.. _function-_at_ast_boost_c__c_find_unique_function_C1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__Cs_Cb:

.. das:function:: find_unique_function(mod: Module? const; name: string const; canfail: bool const)

find_unique_function returns smart_ptr< :ref:`ast::Function <handle-ast-Function>` >

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const+
+--------+-------------------------------------------------+
+name    +string const                                     +
+--------+-------------------------------------------------+
+canfail +bool const                                       +
+--------+-------------------------------------------------+


|function-ast_boost-find_unique_function|

.. _function-_at_ast_boost_c__c_find_unique_generic_C1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__Cs_Cb:

.. das:function:: find_unique_generic(mod: Module? const; name: string const; canfail: bool const)

find_unique_generic returns smart_ptr< :ref:`ast::Function <handle-ast-Function>` >

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const+
+--------+-------------------------------------------------+
+name    +string const                                     +
+--------+-------------------------------------------------+
+canfail +bool const                                       +
+--------+-------------------------------------------------+


|function-ast_boost-find_unique_generic|

.. _function-_at_ast_boost_c__c_find_annotation_Cs_Cs:

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

.. _function-_at_ast_boost_c__c_get_for_source_index_C1_ls_H_ls_ast_c__c_ExprFor_gr__gr__qm_M_CY_ls_VariablePtr_gr_1_ls_H_ls_ast_c__c_Variable_gr__gr__qm_M:

.. das:function:: get_for_source_index(expr: smart_ptr<ExprFor> const; svar: VariablePtr)

get_for_source_index returns int const

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+expr    +smart_ptr< :ref:`ast::ExprFor <handle-ast-ExprFor>` > const+
+--------+-----------------------------------------------------------+
+svar    + :ref:`VariablePtr <alias-VariablePtr>`                    +
+--------+-----------------------------------------------------------+


|function-ast_boost-get_for_source_index|

.. _function-_at_ast_boost_c__c_get_for_source_index_C1_ls_H_ls_ast_c__c_ExprFor_gr__gr__qm_M_CY_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M:

.. das:function:: get_for_source_index(expr: smart_ptr<ExprFor> const; source: ExpressionPtr)

get_for_source_index returns int const

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+expr    +smart_ptr< :ref:`ast::ExprFor <handle-ast-ExprFor>` > const+
+--------+-----------------------------------------------------------+
+source  + :ref:`ExpressionPtr <alias-ExpressionPtr>`                +
+--------+-----------------------------------------------------------+


|function-ast_boost-get_for_source_index|

.. _function-_at_ast_boost_c__c_isCMRES_CY_ls_FunctionPtr_gr_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M:

.. das:function:: isCMRES(fun: FunctionPtr)

isCMRES returns bool const

+--------+----------------------------------------+
+argument+argument type                           +
+========+========================================+
+fun     + :ref:`FunctionPtr <alias-FunctionPtr>` +
+--------+----------------------------------------+


|function-ast_boost-isCMRES|

.. _function-_at_ast_boost_c__c_isCMRES_C1_ls_H_ls_ast_c__c_Function_gr__gr__qm_:

.. das:function:: isCMRES(fun: Function? const)

isCMRES returns bool const

+--------+---------------------------------------------------+
+argument+argument type                                      +
+========+===================================================+
+fun     + :ref:`ast::Function <handle-ast-Function>` ? const+
+--------+---------------------------------------------------+


|function-ast_boost-isCMRES|

.. _function-_at_ast_boost_c__c_isMakeLocal_CY_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M:

.. das:function:: isMakeLocal(expr: ExpressionPtr)

isMakeLocal returns bool const

+--------+--------------------------------------------+
+argument+argument type                               +
+========+============================================+
+expr    + :ref:`ExpressionPtr <alias-ExpressionPtr>` +
+--------+--------------------------------------------+


|function-ast_boost-isMakeLocal|

.. _function-_at_ast_boost_c__c_isExprCallFunc_CY_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M:

.. das:function:: isExprCallFunc(expr: ExpressionPtr)

isExprCallFunc returns bool const

+--------+--------------------------------------------+
+argument+argument type                               +
+========+============================================+
+expr    + :ref:`ExpressionPtr <alias-ExpressionPtr>` +
+--------+--------------------------------------------+


|function-ast_boost-isExprCallFunc|

.. _function-_at_ast_boost_c__c_get_workhorse_types:

.. das:function:: get_workhorse_types()

get_workhorse_types returns  :ref:`rtti::Type <enum-rtti-Type>` [31]

|function-ast_boost-get_workhorse_types|

.. _function-_at_ast_boost_c__c_find_argument_index_CY_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_Cs:

.. das:function:: find_argument_index(typ: TypeDeclPtr; name: string const)

find_argument_index returns int const

+--------+----------------------------------------+
+argument+argument type                           +
+========+========================================+
+typ     + :ref:`TypeDeclPtr <alias-TypeDeclPtr>` +
+--------+----------------------------------------+
+name    +string const                            +
+--------+----------------------------------------+


|function-ast_boost-find_argument_index|

.. _function-_at_ast_boost_c__c_isCMRESType_CY_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M:

.. das:function:: isCMRESType(blockT: TypeDeclPtr)

isCMRESType returns bool const

+--------+----------------------------------------+
+argument+argument type                           +
+========+========================================+
+blockT  + :ref:`TypeDeclPtr <alias-TypeDeclPtr>` +
+--------+----------------------------------------+


|function-ast_boost-isCMRESType|

.. _function-_at_ast_boost_c__c_getVectorElementCount_CE_ls_rtti_c__c_Type_gr_:

.. das:function:: getVectorElementCount(bt: Type const)

getVectorElementCount returns int const

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+bt      + :ref:`rtti::Type <enum-rtti-Type>`  const+
+--------+------------------------------------------+


|function-ast_boost-getVectorElementCount|

.. _function-_at_ast_boost_c__c_getVectorElementSize_CE_ls_rtti_c__c_Type_gr_:

.. das:function:: getVectorElementSize(bt: Type const)

getVectorElementSize returns int const

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+bt      + :ref:`rtti::Type <enum-rtti-Type>`  const+
+--------+------------------------------------------+


|function-ast_boost-getVectorElementSize|

.. _function-_at_ast_boost_c__c_getVectorElementType_CE_ls_rtti_c__c_Type_gr_:

.. das:function:: getVectorElementType(bt: Type const)

getVectorElementType returns  :ref:`rtti::Type <enum-rtti-Type>`  const

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+bt      + :ref:`rtti::Type <enum-rtti-Type>`  const+
+--------+------------------------------------------+


|function-ast_boost-getVectorElementType|

.. _function-_at_ast_boost_c__c_getVectorOffset_CE_ls_rtti_c__c_Type_gr__Cs:

.. das:function:: getVectorOffset(bt: Type const; ident: string const)

getVectorOffset returns int const

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+bt      + :ref:`rtti::Type <enum-rtti-Type>`  const+
+--------+------------------------------------------+
+ident   +string const                              +
+--------+------------------------------------------+


|function-ast_boost-getVectorOffset|

+++++++++++
Annotations
+++++++++++

  *  :ref:`append_annotation (mod_name:string const;ann_name:string const;args:array\<tuple\<argname:string;argvalue:variant\<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any\>\>\> const) : smart_ptr\<rtti::AnnotationDeclaration\> <function-_at_ast_boost_c__c_append_annotation_Cs_Cs_C1_ls_N_ls_argname;argvalue_gr_0_ls_s;Y_ls_RttiValue_gr_N_ls_tBool;tInt;tUInt;tInt64;tUInt64;tFloat;tDouble;tString;nothing_gr_0_ls_b;i;u;i64;u64;f;d;s;*_gr_V_gr_U_gr_A>` 
  *  :ref:`append_annotation (mod_name:string const;ann_name:string const) : smart_ptr\<rtti::AnnotationDeclaration\> <function-_at_ast_boost_c__c_append_annotation_Cs_Cs>` 
  *  :ref:`append_annotation (func:smart_ptr\<ast::Function\> -const;mod_name:string const;ann_name:string const) : void <function-_at_ast_boost_c__c_append_annotation_Y_ls_FunctionPtr_gr_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M_Cs_Cs>` 
  *  :ref:`append_annotation (blk:smart_ptr\<ast::ExprBlock\> -const;mod_name:string const;ann_name:string const) : void <function-_at_ast_boost_c__c_append_annotation_1_ls_H_ls_ast_c__c_ExprBlock_gr__gr__qm_M_Cs_Cs>` 
  *  :ref:`append_annotation (st:smart_ptr\<ast::Structure\> -const;mod_name:string const;ann_name:string const) : void <function-_at_ast_boost_c__c_append_annotation_1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_M_Cs_Cs>` 
  *  :ref:`append_annotation (func:smart_ptr\<ast::Function\> -const;mod_name:string const;ann_name:string const;args:array\<tuple\<argname:string;argvalue:variant\<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any\>\>\> const) : void <function-_at_ast_boost_c__c_append_annotation_Y_ls_FunctionPtr_gr_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M_Cs_Cs_C1_ls_N_ls_argname;argvalue_gr_0_ls_s;Y_ls_RttiValue_gr_N_ls_tBool;tInt;tUInt;tInt64;tUInt64;tFloat;tDouble;tString;nothing_gr_0_ls_b;i;u;i64;u64;f;d;s;*_gr_V_gr_U_gr_A>` 
  *  :ref:`append_annotation (blk:smart_ptr\<ast::ExprBlock\> -const;mod_name:string const;ann_name:string const;args:array\<tuple\<argname:string;argvalue:variant\<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any\>\>\> const) : void <function-_at_ast_boost_c__c_append_annotation_1_ls_H_ls_ast_c__c_ExprBlock_gr__gr__qm_M_Cs_Cs_C1_ls_N_ls_argname;argvalue_gr_0_ls_s;Y_ls_RttiValue_gr_N_ls_tBool;tInt;tUInt;tInt64;tUInt64;tFloat;tDouble;tString;nothing_gr_0_ls_b;i;u;i64;u64;f;d;s;*_gr_V_gr_U_gr_A>` 
  *  :ref:`append_annotation (st:smart_ptr\<ast::Structure\> -const;mod_name:string const;ann_name:string const;args:array\<tuple\<argname:string;argvalue:variant\<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any\>\>\> const) : void <function-_at_ast_boost_c__c_append_annotation_1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_M_Cs_Cs_C1_ls_N_ls_argname;argvalue_gr_0_ls_s;Y_ls_RttiValue_gr_N_ls_tBool;tInt;tUInt;tInt64;tUInt64;tFloat;tDouble;tString;nothing_gr_0_ls_b;i;u;i64;u64;f;d;s;*_gr_V_gr_U_gr_A>` 
  *  :ref:`add_annotation_argument (arguments:rtti::AnnotationArgumentList -const;argName:string const;val:bool const) : int const <function-_at_ast_boost_c__c_add_annotation_argument_H_ls_rtti_c__c_AnnotationArgumentList_gr__Cs_Cb>` 
  *  :ref:`add_annotation_argument (arguments:rtti::AnnotationArgumentList -const;argName:string const;val:int const) : int const <function-_at_ast_boost_c__c_add_annotation_argument_H_ls_rtti_c__c_AnnotationArgumentList_gr__Cs_Ci>` 
  *  :ref:`add_annotation_argument (arguments:rtti::AnnotationArgumentList -const;argName:string const;val:float const) : int const <function-_at_ast_boost_c__c_add_annotation_argument_H_ls_rtti_c__c_AnnotationArgumentList_gr__Cs_Cf>` 
  *  :ref:`add_annotation_argument (arguments:rtti::AnnotationArgumentList -const;argName:string const;val:string const) : int const <function-_at_ast_boost_c__c_add_annotation_argument_H_ls_rtti_c__c_AnnotationArgumentList_gr__Cs_Cs>` 
  *  :ref:`add_annotation_argument (arguments:rtti::AnnotationArgumentList -const;ann:rtti::AnnotationArgument const) : int const <function-_at_ast_boost_c__c_add_annotation_argument_H_ls_rtti_c__c_AnnotationArgumentList_gr__CH_ls_rtti_c__c_AnnotationArgument_gr_>` 

.. _function-_at_ast_boost_c__c_append_annotation_Cs_Cs_C1_ls_N_ls_argname;argvalue_gr_0_ls_s;Y_ls_RttiValue_gr_N_ls_tBool;tInt;tUInt;tInt64;tUInt64;tFloat;tDouble;tString;nothing_gr_0_ls_b;i;u;i64;u64;f;d;s;*_gr_V_gr_U_gr_A:

.. das:function:: append_annotation(mod_name: string const; ann_name: string const; args: array<tuple<argname:string;argvalue:variant<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any>>> const)

append_annotation returns smart_ptr< :ref:`rtti::AnnotationDeclaration <handle-rtti-AnnotationDeclaration>` >

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+mod_name+string const                                                                    +
+--------+--------------------------------------------------------------------------------+
+ann_name+string const                                                                    +
+--------+--------------------------------------------------------------------------------+
+args    +array<tuple<argname:string;argvalue: :ref:`RttiValue <alias-RttiValue>` >> const+
+--------+--------------------------------------------------------------------------------+


|function-ast_boost-append_annotation|

.. _function-_at_ast_boost_c__c_append_annotation_Cs_Cs:

.. das:function:: append_annotation(mod_name: string const; ann_name: string const)

append_annotation returns smart_ptr< :ref:`rtti::AnnotationDeclaration <handle-rtti-AnnotationDeclaration>` >

+--------+-------------+
+argument+argument type+
+========+=============+
+mod_name+string const +
+--------+-------------+
+ann_name+string const +
+--------+-------------+


|function-ast_boost-append_annotation|

.. _function-_at_ast_boost_c__c_append_annotation_Y_ls_FunctionPtr_gr_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M_Cs_Cs:

.. das:function:: append_annotation(func: FunctionPtr; mod_name: string const; ann_name: string const)

+--------+----------------------------------------+
+argument+argument type                           +
+========+========================================+
+func    + :ref:`FunctionPtr <alias-FunctionPtr>` +
+--------+----------------------------------------+
+mod_name+string const                            +
+--------+----------------------------------------+
+ann_name+string const                            +
+--------+----------------------------------------+


|function-ast_boost-append_annotation|

.. _function-_at_ast_boost_c__c_append_annotation_1_ls_H_ls_ast_c__c_ExprBlock_gr__gr__qm_M_Cs_Cs:

.. das:function:: append_annotation(blk: smart_ptr<ExprBlock>; mod_name: string const; ann_name: string const)

+--------+---------------------------------------------------------+
+argument+argument type                                            +
+========+=========================================================+
+blk     +smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` >+
+--------+---------------------------------------------------------+
+mod_name+string const                                             +
+--------+---------------------------------------------------------+
+ann_name+string const                                             +
+--------+---------------------------------------------------------+


|function-ast_boost-append_annotation|

.. _function-_at_ast_boost_c__c_append_annotation_1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_M_Cs_Cs:

.. das:function:: append_annotation(st: smart_ptr<Structure>; mod_name: string const; ann_name: string const)

+--------+---------------------------------------------------------+
+argument+argument type                                            +
+========+=========================================================+
+st      +smart_ptr< :ref:`ast::Structure <handle-ast-Structure>` >+
+--------+---------------------------------------------------------+
+mod_name+string const                                             +
+--------+---------------------------------------------------------+
+ann_name+string const                                             +
+--------+---------------------------------------------------------+


|function-ast_boost-append_annotation|

.. _function-_at_ast_boost_c__c_append_annotation_Y_ls_FunctionPtr_gr_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M_Cs_Cs_C1_ls_N_ls_argname;argvalue_gr_0_ls_s;Y_ls_RttiValue_gr_N_ls_tBool;tInt;tUInt;tInt64;tUInt64;tFloat;tDouble;tString;nothing_gr_0_ls_b;i;u;i64;u64;f;d;s;*_gr_V_gr_U_gr_A:

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

.. _function-_at_ast_boost_c__c_append_annotation_1_ls_H_ls_ast_c__c_ExprBlock_gr__gr__qm_M_Cs_Cs_C1_ls_N_ls_argname;argvalue_gr_0_ls_s;Y_ls_RttiValue_gr_N_ls_tBool;tInt;tUInt;tInt64;tUInt64;tFloat;tDouble;tString;nothing_gr_0_ls_b;i;u;i64;u64;f;d;s;*_gr_V_gr_U_gr_A:

.. das:function:: append_annotation(blk: smart_ptr<ExprBlock>; mod_name: string const; ann_name: string const; args: array<tuple<argname:string;argvalue:variant<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any>>> const)

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+blk     +smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` >                       +
+--------+--------------------------------------------------------------------------------+
+mod_name+string const                                                                    +
+--------+--------------------------------------------------------------------------------+
+ann_name+string const                                                                    +
+--------+--------------------------------------------------------------------------------+
+args    +array<tuple<argname:string;argvalue: :ref:`RttiValue <alias-RttiValue>` >> const+
+--------+--------------------------------------------------------------------------------+


|function-ast_boost-append_annotation|

.. _function-_at_ast_boost_c__c_append_annotation_1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_M_Cs_Cs_C1_ls_N_ls_argname;argvalue_gr_0_ls_s;Y_ls_RttiValue_gr_N_ls_tBool;tInt;tUInt;tInt64;tUInt64;tFloat;tDouble;tString;nothing_gr_0_ls_b;i;u;i64;u64;f;d;s;*_gr_V_gr_U_gr_A:

.. das:function:: append_annotation(st: smart_ptr<Structure>; mod_name: string const; ann_name: string const; args: array<tuple<argname:string;argvalue:variant<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any>>> const)

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+st      +smart_ptr< :ref:`ast::Structure <handle-ast-Structure>` >                       +
+--------+--------------------------------------------------------------------------------+
+mod_name+string const                                                                    +
+--------+--------------------------------------------------------------------------------+
+ann_name+string const                                                                    +
+--------+--------------------------------------------------------------------------------+
+args    +array<tuple<argname:string;argvalue: :ref:`RttiValue <alias-RttiValue>` >> const+
+--------+--------------------------------------------------------------------------------+


|function-ast_boost-append_annotation|

.. _function-_at_ast_boost_c__c_add_annotation_argument_H_ls_rtti_c__c_AnnotationArgumentList_gr__Cs_Cb:

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

.. _function-_at_ast_boost_c__c_add_annotation_argument_H_ls_rtti_c__c_AnnotationArgumentList_gr__Cs_Ci:

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

.. _function-_at_ast_boost_c__c_add_annotation_argument_H_ls_rtti_c__c_AnnotationArgumentList_gr__Cs_Cf:

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

.. _function-_at_ast_boost_c__c_add_annotation_argument_H_ls_rtti_c__c_AnnotationArgumentList_gr__Cs_Cs:

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

.. _function-_at_ast_boost_c__c_add_annotation_argument_H_ls_rtti_c__c_AnnotationArgumentList_gr__CH_ls_rtti_c__c_AnnotationArgument_gr_:

.. das:function:: add_annotation_argument(arguments: AnnotationArgumentList; ann: AnnotationArgument const)

add_annotation_argument returns int const

+---------+--------------------------------------------------------------------------+
+argument +argument type                                                             +
+=========+==========================================================================+
+arguments+ :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` +
+---------+--------------------------------------------------------------------------+
+ann      + :ref:`rtti::AnnotationArgument <handle-rtti-AnnotationArgument>`  const  +
+---------+--------------------------------------------------------------------------+


|function-ast_boost-add_annotation_argument|

+++++++++++++++++++++
Expression generation
+++++++++++++++++++++

  *  :ref:`override_method (str:smart_ptr\<ast::Structure\> -const;name:string const;funcName:string const) : bool const <function-_at_ast_boost_c__c_override_method_Y_ls_StructurePtr_gr_1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_M_Cs_Cs>` 
  *  :ref:`panic_expr_as () : void? <function-_at_ast_boost_c__c_panic_expr_as>` 
  *  :ref:`make_static_assert_false (text:string const;at:rtti::LineInfo const) : smart_ptr\<ast::ExprStaticAssert\> <function-_at_ast_boost_c__c_make_static_assert_false_Cs_CH_ls_rtti_c__c_LineInfo_gr_>` 
  *  :ref:`convert_to_expression (value:auto& ==const -const;at:rtti::LineInfo const) : auto <function-_at_ast_boost_c__c_convert_to_expression_&_eq_._CH_ls_rtti_c__c_LineInfo_gr_>` 
  *  :ref:`convert_to_expression (value:auto const ==const;at:rtti::LineInfo const) : auto <function-_at_ast_boost_c__c_convert_to_expression_C_eq_._CH_ls_rtti_c__c_LineInfo_gr_>` 
  *  :ref:`convert_to_expression (value:auto ==const -const) : auto <function-_at_ast_boost_c__c_convert_to_expression__eq_.>` 
  *  :ref:`convert_to_expression (value:auto const ==const) : auto <function-_at_ast_boost_c__c_convert_to_expression_C_eq_.>` 

.. _function-_at_ast_boost_c__c_override_method_Y_ls_StructurePtr_gr_1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_M_Cs_Cs:

.. das:function:: override_method(str: StructurePtr; name: string const; funcName: string const)

override_method returns bool const

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

.. _function-_at_ast_boost_c__c_panic_expr_as:

.. das:function:: panic_expr_as()

panic_expr_as returns void?

|function-ast_boost-panic_expr_as|

.. _function-_at_ast_boost_c__c_make_static_assert_false_Cs_CH_ls_rtti_c__c_LineInfo_gr_:

.. das:function:: make_static_assert_false(text: string const; at: LineInfo const)

make_static_assert_false returns smart_ptr< :ref:`ast::ExprStaticAssert <handle-ast-ExprStaticAssert>` >

+--------+----------------------------------------------------+
+argument+argument type                                       +
+========+====================================================+
+text    +string const                                        +
+--------+----------------------------------------------------+
+at      + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`  const+
+--------+----------------------------------------------------+


|function-ast_boost-make_static_assert_false|

.. _function-_at_ast_boost_c__c_convert_to_expression_&_eq_._CH_ls_rtti_c__c_LineInfo_gr_:

.. das:function:: convert_to_expression(value: auto& ==const; at: LineInfo const)

convert_to_expression returns auto

+--------+----------------------------------------------------+
+argument+argument type                                       +
+========+====================================================+
+value   +auto&!                                              +
+--------+----------------------------------------------------+
+at      + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`  const+
+--------+----------------------------------------------------+


|function-ast_boost-convert_to_expression|

.. _function-_at_ast_boost_c__c_convert_to_expression_C_eq_._CH_ls_rtti_c__c_LineInfo_gr_:

.. das:function:: convert_to_expression(value: auto const ==const; at: LineInfo const)

convert_to_expression returns auto

+--------+----------------------------------------------------+
+argument+argument type                                       +
+========+====================================================+
+value   +auto const!                                         +
+--------+----------------------------------------------------+
+at      + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`  const+
+--------+----------------------------------------------------+


|function-ast_boost-convert_to_expression|

.. _function-_at_ast_boost_c__c_convert_to_expression__eq_.:

.. das:function:: convert_to_expression(value: auto ==const)

convert_to_expression returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+value   +auto!        +
+--------+-------------+


|function-ast_boost-convert_to_expression|

.. _function-_at_ast_boost_c__c_convert_to_expression_C_eq_.:

.. das:function:: convert_to_expression(value: auto const ==const)

convert_to_expression returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+value   +auto const!  +
+--------+-------------+


|function-ast_boost-convert_to_expression|

++++++++
Visitors
++++++++

  *  :ref:`visit_finally (blk:ast::ExprBlock? const;adapter:smart_ptr\<ast::VisitorAdapter\> const) : void <function-_at_ast_boost_c__c_visit_finally_C1_ls_H_ls_ast_c__c_ExprBlock_gr__gr__qm__C1_ls_H_ls_ast_c__c_VisitorAdapter_gr__gr__qm_M>` 

.. _function-_at_ast_boost_c__c_visit_finally_C1_ls_H_ls_ast_c__c_ExprBlock_gr__gr__qm__C1_ls_H_ls_ast_c__c_VisitorAdapter_gr__gr__qm_M:

.. das:function:: visit_finally(blk: ExprBlock? const; adapter: smart_ptr<VisitorAdapter> const)

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+blk     + :ref:`ast::ExprBlock <handle-ast-ExprBlock>` ? const                    +
+--------+-------------------------------------------------------------------------+
+adapter +smart_ptr< :ref:`ast::VisitorAdapter <handle-ast-VisitorAdapter>` > const+
+--------+-------------------------------------------------------------------------+


|function-ast_boost-visit_finally|

+++++++++++++++
Type generation
+++++++++++++++

  *  :ref:`function_to_type (fn:smart_ptr\<ast::Function\> const) : smart_ptr\<ast::TypeDecl\> <function-_at_ast_boost_c__c_function_to_type_CY_ls_FunctionPtr_gr_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M>` 

.. _function-_at_ast_boost_c__c_function_to_type_CY_ls_FunctionPtr_gr_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M:

.. das:function:: function_to_type(fn: FunctionPtr)

function_to_type returns  :ref:`TypeDeclPtr <alias-TypeDeclPtr>` 

+--------+----------------------------------------+
+argument+argument type                           +
+========+========================================+
+fn      + :ref:`FunctionPtr <alias-FunctionPtr>` +
+--------+----------------------------------------+


|function-ast_boost-function_to_type|

+++++
Setup
+++++

  *  :ref:`setup_call_list (name:string const;at:rtti::LineInfo const;subblock:block\<(var fn:smart_ptr\<ast::Function\> -const):void\> const) : ast::ExprBlock? <function-_at_ast_boost_c__c_setup_call_list_Cs_CH_ls_rtti_c__c_LineInfo_gr__CN_ls_fn_gr_0_ls_Y_ls_FunctionPtr_gr_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`setup_call_list (name:string const;at:rtti::LineInfo const;isInit:bool const;isPrivate:bool const;isLateInit:bool const) : ast::ExprBlock? <function-_at_ast_boost_c__c_setup_call_list_Cs_CH_ls_rtti_c__c_LineInfo_gr__Cb_Cb_Cb>` 
  *  :ref:`setup_macro (name:string const;at:rtti::LineInfo const) : ast::ExprBlock? <function-_at_ast_boost_c__c_setup_macro_Cs_CH_ls_rtti_c__c_LineInfo_gr_>` 
  *  :ref:`setup_tag_annotation (name:string const;tag:string const;classPtr:auto -const) : auto <function-_at_ast_boost_c__c_setup_tag_annotation_Cs_Cs_.>` 

.. _function-_at_ast_boost_c__c_setup_call_list_Cs_CH_ls_rtti_c__c_LineInfo_gr__CN_ls_fn_gr_0_ls_Y_ls_FunctionPtr_gr_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M_gr_1_ls_v_gr__builtin_:

.. das:function:: setup_call_list(name: string const; at: LineInfo const; subblock: block<(var fn:smart_ptr<Function>):void> const)

setup_call_list returns  :ref:`ast::ExprBlock <handle-ast-ExprBlock>` ?

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+name    +string const                                                   +
+--------+---------------------------------------------------------------+
+at      + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`  const           +
+--------+---------------------------------------------------------------+
+subblock+block<(fn: :ref:`FunctionPtr <alias-FunctionPtr>` ):void> const+
+--------+---------------------------------------------------------------+


|function-ast_boost-setup_call_list|

.. _function-_at_ast_boost_c__c_setup_call_list_Cs_CH_ls_rtti_c__c_LineInfo_gr__Cb_Cb_Cb:

.. das:function:: setup_call_list(name: string const; at: LineInfo const; isInit: bool const; isPrivate: bool const; isLateInit: bool const)

setup_call_list returns  :ref:`ast::ExprBlock <handle-ast-ExprBlock>` ?

+----------+----------------------------------------------------+
+argument  +argument type                                       +
+==========+====================================================+
+name      +string const                                        +
+----------+----------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`  const+
+----------+----------------------------------------------------+
+isInit    +bool const                                          +
+----------+----------------------------------------------------+
+isPrivate +bool const                                          +
+----------+----------------------------------------------------+
+isLateInit+bool const                                          +
+----------+----------------------------------------------------+


|function-ast_boost-setup_call_list|

.. _function-_at_ast_boost_c__c_setup_macro_Cs_CH_ls_rtti_c__c_LineInfo_gr_:

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

.. _function-_at_ast_boost_c__c_setup_tag_annotation_Cs_Cs_.:

.. das:function:: setup_tag_annotation(name: string const; tag: string const; classPtr: auto)

setup_tag_annotation returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+name    +string const +
+--------+-------------+
+tag     +string const +
+--------+-------------+
+classPtr+auto         +
+--------+-------------+


|function-ast_boost-setup_tag_annotation|

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_ast_boost_c__c__rq_is_rq_BuiltInFunction_C1_ls_H_ls_ast_c__c_Function_gr__gr__qm_:

.. das:function:: operator `is`BuiltInFunction(foo: Function? const)

`is`BuiltInFunction returns bool const

+--------+---------------------------------------------------+
+argument+argument type                                      +
+========+===================================================+
+foo     + :ref:`ast::Function <handle-ast-Function>` ? const+
+--------+---------------------------------------------------+


|function-ast_boost-`is`BuiltInFunction|

.. _function-_at_ast_boost_c__c__rq_as_rq_BuiltInFunction_C1_ls_H_ls_ast_c__c_Function_gr__gr__qm_:

.. das:function:: operator `as`BuiltInFunction(foo: Function? const)

`as`BuiltInFunction returns  :ref:`ast::BuiltInFunction <handle-ast-BuiltInFunction>` ?

+--------+---------------------------------------------------+
+argument+argument type                                      +
+========+===================================================+
+foo     + :ref:`ast::Function <handle-ast-Function>` ? const+
+--------+---------------------------------------------------+


|function-ast_boost-`as`BuiltInFunction|

.. _function-_at_ast_boost_c__c__rq_is_rq_ExternalFnBase_C1_ls_H_ls_ast_c__c_Function_gr__gr__qm_:

.. das:function:: operator `is`ExternalFnBase(foo: Function? const)

`is`ExternalFnBase returns bool const

+--------+---------------------------------------------------+
+argument+argument type                                      +
+========+===================================================+
+foo     + :ref:`ast::Function <handle-ast-Function>` ? const+
+--------+---------------------------------------------------+


|function-ast_boost-`is`ExternalFnBase|

.. _function-_at_ast_boost_c__c__rq_as_rq_ExternalFnBase_C1_ls_H_ls_ast_c__c_Function_gr__gr__qm_:

.. das:function:: operator `as`ExternalFnBase(foo: Function? const)

`as`ExternalFnBase returns  :ref:`ast::ExternalFnBase <handle-ast-ExternalFnBase>` ?

+--------+---------------------------------------------------+
+argument+argument type                                      +
+========+===================================================+
+foo     + :ref:`ast::Function <handle-ast-Function>` ? const+
+--------+---------------------------------------------------+


|function-ast_boost-`as`ExternalFnBase|

.. _function-_at_ast_boost_c__c__rq_is_rq_FunctionAnnotation_C1_ls_H_ls_rtti_c__c_Annotation_gr__gr__qm_:

.. das:function:: operator `is`FunctionAnnotation(foo: Annotation? const)

`is`FunctionAnnotation returns bool const

+--------+---------------------------------------------------------+
+argument+argument type                                            +
+========+=========================================================+
+foo     + :ref:`rtti::Annotation <handle-rtti-Annotation>` ? const+
+--------+---------------------------------------------------------+


|function-ast_boost-`is`FunctionAnnotation|

.. _function-_at_ast_boost_c__c__rq_is_rq_FunctionAnnotation_C1_ls_H_ls_rtti_c__c_Annotation_gr__gr__qm_M:

.. das:function:: operator `is`FunctionAnnotation(foo: smart_ptr<Annotation> const)

`is`FunctionAnnotation returns bool const

+--------+-------------------------------------------------------------------+
+argument+argument type                                                      +
+========+===================================================================+
+foo     +smart_ptr< :ref:`rtti::Annotation <handle-rtti-Annotation>` > const+
+--------+-------------------------------------------------------------------+


|function-ast_boost-`is`FunctionAnnotation|

.. _function-_at_ast_boost_c__c__rq_as_rq_FunctionAnnotation_C1_ls_H_ls_rtti_c__c_Annotation_gr__gr__qm_:

.. das:function:: operator `as`FunctionAnnotation(foo: Annotation? const)

`as`FunctionAnnotation returns  :ref:`ast::FunctionAnnotation <handle-ast-FunctionAnnotation>` ?

+--------+---------------------------------------------------------+
+argument+argument type                                            +
+========+=========================================================+
+foo     + :ref:`rtti::Annotation <handle-rtti-Annotation>` ? const+
+--------+---------------------------------------------------------+


|function-ast_boost-`as`FunctionAnnotation|

.. _function-_at_ast_boost_c__c__rq_as_rq_FunctionAnnotation_C1_ls_H_ls_rtti_c__c_Annotation_gr__gr__qm_M:

.. das:function:: operator `as`FunctionAnnotation(foo: smart_ptr<Annotation> const)

`as`FunctionAnnotation returns  :ref:`ast::FunctionAnnotation <handle-ast-FunctionAnnotation>` ?

+--------+-------------------------------------------------------------------+
+argument+argument type                                                      +
+========+===================================================================+
+foo     +smart_ptr< :ref:`rtti::Annotation <handle-rtti-Annotation>` > const+
+--------+-------------------------------------------------------------------+


|function-ast_boost-`as`FunctionAnnotation|

.. _function-_at_ast_boost_c__c__rq_is_rq_StructureAnnotation_C1_ls_H_ls_rtti_c__c_Annotation_gr__gr__qm_:

.. das:function:: operator `is`StructureAnnotation(foo: Annotation? const)

`is`StructureAnnotation returns bool const

+--------+---------------------------------------------------------+
+argument+argument type                                            +
+========+=========================================================+
+foo     + :ref:`rtti::Annotation <handle-rtti-Annotation>` ? const+
+--------+---------------------------------------------------------+


|function-ast_boost-`is`StructureAnnotation|

.. _function-_at_ast_boost_c__c__rq_is_rq_StructureAnnotation_C1_ls_H_ls_rtti_c__c_Annotation_gr__gr__qm_M:

.. das:function:: operator `is`StructureAnnotation(foo: smart_ptr<Annotation> const)

`is`StructureAnnotation returns bool const

+--------+-------------------------------------------------------------------+
+argument+argument type                                                      +
+========+===================================================================+
+foo     +smart_ptr< :ref:`rtti::Annotation <handle-rtti-Annotation>` > const+
+--------+-------------------------------------------------------------------+


|function-ast_boost-`is`StructureAnnotation|

.. _function-_at_ast_boost_c__c__rq_as_rq_StructureAnnotation_C1_ls_H_ls_rtti_c__c_Annotation_gr__gr__qm_:

.. das:function:: operator `as`StructureAnnotation(foo: Annotation? const)

`as`StructureAnnotation returns  :ref:`ast::StructureAnnotation <handle-ast-StructureAnnotation>` ?

+--------+---------------------------------------------------------+
+argument+argument type                                            +
+========+=========================================================+
+foo     + :ref:`rtti::Annotation <handle-rtti-Annotation>` ? const+
+--------+---------------------------------------------------------+


|function-ast_boost-`as`StructureAnnotation|

.. _function-_at_ast_boost_c__c__rq_as_rq_StructureAnnotation_C1_ls_H_ls_rtti_c__c_Annotation_gr__gr__qm_M:

.. das:function:: operator `as`StructureAnnotation(foo: smart_ptr<Annotation> const)

`as`StructureAnnotation returns  :ref:`ast::StructureAnnotation <handle-ast-StructureAnnotation>` ?

+--------+-------------------------------------------------------------------+
+argument+argument type                                                      +
+========+===================================================================+
+foo     +smart_ptr< :ref:`rtti::Annotation <handle-rtti-Annotation>` > const+
+--------+-------------------------------------------------------------------+


|function-ast_boost-`as`StructureAnnotation|

.. _function-_at_ast_boost_c__c_walk_and_convert_C1_ls_Cu8_gr__qm__CY_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_CH_ls_rtti_c__c_LineInfo_gr_:

.. das:function:: walk_and_convert(data: uint8 const? const; info: TypeDeclPtr; at: LineInfo const)

walk_and_convert returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+----------------------------------------------------+
+argument+argument type                                       +
+========+====================================================+
+data    +uint8 const? const                                  +
+--------+----------------------------------------------------+
+info    + :ref:`TypeDeclPtr <alias-TypeDeclPtr>`             +
+--------+----------------------------------------------------+
+at      + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`  const+
+--------+----------------------------------------------------+


|function-ast_boost-walk_and_convert|

.. _function-_at_ast_boost_c__c__rq_is_rq_BuiltInFunction_C.:

.. das:function:: operator `is`BuiltInFunction(anything: auto const)

`is`BuiltInFunction returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+anything+auto const   +
+--------+-------------+


|function-ast_boost-`is`BuiltInFunction|

.. _function-_at_ast_boost_c__c__rq_is_rq_ExternalFnBase_C.:

.. das:function:: operator `is`ExternalFnBase(anything: auto const)

`is`ExternalFnBase returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+anything+auto const   +
+--------+-------------+


|function-ast_boost-`is`ExternalFnBase|

.. _function-_at_ast_boost_c__c__rq_is_rq_FunctionAnnotation_C.:

.. das:function:: operator `is`FunctionAnnotation(anything: auto const)

`is`FunctionAnnotation returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+anything+auto const   +
+--------+-------------+


|function-ast_boost-`is`FunctionAnnotation|

.. _function-_at_ast_boost_c__c__rq_is_rq_StructureAnnotation_C.:

.. das:function:: operator `is`StructureAnnotation(anything: auto const)

`is`StructureAnnotation returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+anything+auto const   +
+--------+-------------+


|function-ast_boost-`is`StructureAnnotation|


