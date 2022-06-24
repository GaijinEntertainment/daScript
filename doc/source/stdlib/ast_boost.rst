
.. _stdlib_ast_boost:

=========================
Boost package for the AST
=========================

.. include:: detail/ast_boost.rst

The AST boost module implements collection of helper macros and functions to accompany :ref:`AST <stdlib_ast>`.

All functions and symbols are in "ast_boost" module, use require to get access to it. ::

    require daslib/ast_boost


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

.. _struct-ast_boost-SetupFunctionAnnotatoin:

.. das:attribute:: SetupFunctionAnnotatoin : SetupAnyAnnotation

|class-ast_boost-SetupFunctionAnnotatoin|

it defines as follows

  | annotation_function_call : string
  | name                     : string

.. _struct-ast_boost-SetupBlockAnnotatoin:

.. das:attribute:: SetupBlockAnnotatoin : SetupAnyAnnotation

|class-ast_boost-SetupBlockAnnotatoin|

it defines as follows

  | annotation_function_call : string
  | name                     : string

.. _struct-ast_boost-SetupStructureAnnotatoin:

.. das:attribute:: SetupStructureAnnotatoin : SetupAnyAnnotation

|class-ast_boost-SetupStructureAnnotatoin|

it defines as follows

  | annotation_function_call : string
  | name                     : string

.. _struct-ast_boost-SetupEnumerationAnnotation:

.. das:attribute:: SetupEnumerationAnnotation : SetupAnyAnnotation

|class-ast_boost-SetupEnumerationAnnotation|

it defines as follows

  | annotation_function_call : string
  | name                     : string

.. _struct-ast_boost-SetupContractAnnotatoin:

.. das:attribute:: SetupContractAnnotatoin : SetupAnyAnnotation

|class-ast_boost-SetupContractAnnotatoin|

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

++++++++++
Containers
++++++++++

  *  :ref:`emplace_new (vec:$::dasvector`smart_ptr`Expression -const;ptr:smart_ptr\<ast::Expression\> -const) : void <function-_at_ast_boost_c__c_emplace_new_H_ls__builtin__c__c_dasvector_rq_smart_ptr_rq_Expression_gr__1_ls_H_ls_ast_c__c_Expression_gr__gr_?M>` 
  *  :ref:`emplace_new (vec:$::dasvector`smart_ptr`TypeDecl -const;ptr:smart_ptr\<ast::TypeDecl\> -const) : void <function-_at_ast_boost_c__c_emplace_new_H_ls__builtin__c__c_dasvector_rq_smart_ptr_rq_TypeDecl_gr__1_ls_H_ls_ast_c__c_TypeDecl_gr__gr_?M>` 
  *  :ref:`emplace_new (vec:$::dasvector`smart_ptr`Variable -const;ptr:smart_ptr\<ast::Variable\> -const) : void <function-_at_ast_boost_c__c_emplace_new_H_ls__builtin__c__c_dasvector_rq_smart_ptr_rq_Variable_gr__1_ls_H_ls_ast_c__c_Variable_gr__gr_?M>` 
  *  :ref:`emplace_new (vec:ast::MakeStruct -const;ptr:smart_ptr\<ast::MakeFieldDecl\> -const) : void <function-_at_ast_boost_c__c_emplace_new_H_ls_ast_c__c_MakeStruct_gr__1_ls_H_ls_ast_c__c_MakeFieldDecl_gr__gr_?M>` 

.. _function-_at_ast_boost_c__c_emplace_new_H_ls__builtin__c__c_dasvector_rq_smart_ptr_rq_Expression_gr__1_ls_H_ls_ast_c__c_Expression_gr__gr_?M:

.. das:function:: emplace_new(vec: dasvector`smart_ptr`Expression; ptr: smart_ptr<ast::Expression>)

+--------+------------------------------------------------------------------------------------------------+
+argument+argument type                                                                                   +
+========+================================================================================================+
+vec     + :ref:`builtin::dasvector`smart_ptr`Expression <handle-builtin-dasvector`smart_ptr`Expression>` +
+--------+------------------------------------------------------------------------------------------------+
+ptr     +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >                                     +
+--------+------------------------------------------------------------------------------------------------+


|function-ast_boost-emplace_new|

.. _function-_at_ast_boost_c__c_emplace_new_H_ls__builtin__c__c_dasvector_rq_smart_ptr_rq_TypeDecl_gr__1_ls_H_ls_ast_c__c_TypeDecl_gr__gr_?M:

.. das:function:: emplace_new(vec: dasvector`smart_ptr`TypeDecl; ptr: smart_ptr<ast::TypeDecl>)

+--------+--------------------------------------------------------------------------------------------+
+argument+argument type                                                                               +
+========+============================================================================================+
+vec     + :ref:`builtin::dasvector`smart_ptr`TypeDecl <handle-builtin-dasvector`smart_ptr`TypeDecl>` +
+--------+--------------------------------------------------------------------------------------------+
+ptr     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                                     +
+--------+--------------------------------------------------------------------------------------------+


|function-ast_boost-emplace_new|

.. _function-_at_ast_boost_c__c_emplace_new_H_ls__builtin__c__c_dasvector_rq_smart_ptr_rq_Variable_gr__1_ls_H_ls_ast_c__c_Variable_gr__gr_?M:

.. das:function:: emplace_new(vec: dasvector`smart_ptr`Variable; ptr: smart_ptr<ast::Variable>)

+--------+--------------------------------------------------------------------------------------------+
+argument+argument type                                                                               +
+========+============================================================================================+
+vec     + :ref:`builtin::dasvector`smart_ptr`Variable <handle-builtin-dasvector`smart_ptr`Variable>` +
+--------+--------------------------------------------------------------------------------------------+
+ptr     +smart_ptr< :ref:`ast::Variable <handle-ast-Variable>` >                                     +
+--------+--------------------------------------------------------------------------------------------+


|function-ast_boost-emplace_new|

.. _function-_at_ast_boost_c__c_emplace_new_H_ls_ast_c__c_MakeStruct_gr__1_ls_H_ls_ast_c__c_MakeFieldDecl_gr__gr_?M:

.. das:function:: emplace_new(vec: MakeStruct; ptr: smart_ptr<ast::MakeFieldDecl>)

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
  *  :ref:`describe (ann:rtti::AnnotationDeclaration const) : string <function-_at_ast_boost_c__c_describe_CH_ls_rtti_c__c_AnnotationDeclaration_gr_>` 
  *  :ref:`describe (list:rtti::AnnotationList const) : string const <function-_at_ast_boost_c__c_describe_CH_ls_rtti_c__c_AnnotationList_gr_>` 
  *  :ref:`describe_function_short (func:smart_ptr\<ast::Function\> const) : string const <function-_at_ast_boost_c__c_describe_function_short_CY_ls_FunctionPtr_gr_1_ls_H_ls_ast_c__c_Function_gr__gr_?M>` 
  *  :ref:`describe_bitfield (bf:auto const;merger:string const) : auto <function-_at_ast_boost_c__c_describe_bitfield_C._Cs>` 

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

describe returns string

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

.. _function-_at_ast_boost_c__c_describe_function_short_CY_ls_FunctionPtr_gr_1_ls_H_ls_ast_c__c_Function_gr__gr_?M:

.. das:function:: describe_function_short(func: FunctionPtr)

describe_function_short returns string const

+--------+----------------------------------------+
+argument+argument type                           +
+========+========================================+
+func    + :ref:`FunctionPtr <alias-FunctionPtr>` +
+--------+----------------------------------------+


|function-ast_boost-describe_function_short|

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

+++++++
Queries
+++++++

  *  :ref:`isVectorType (typ:rtti::Type const) : bool <function-_at_ast_boost_c__c_isVectorType_CE_ls_rtti_c__c_Type_gr_>` 
  *  :ref:`isExpression (t:smart_ptr\<ast::TypeDecl\> const;top:bool const) : bool <function-_at_ast_boost_c__c_isExpression_CY_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr_?M_Cb>` 
  *  :ref:`is_same_or_inherited (parent:ast::Structure? const;child:ast::Structure? const) : bool const <function-_at_ast_boost_c__c_is_same_or_inherited_C1_ls_H_ls_ast_c__c_Structure_gr__gr_?_C1_ls_H_ls_ast_c__c_Structure_gr__gr_?>` 
  *  :ref:`is_class_method (cinfo:smart_ptr\<ast::Structure\> const;finfo:smart_ptr\<ast::TypeDecl\> const) : bool const <function-_at_ast_boost_c__c_is_class_method_CY_ls_StructurePtr_gr_1_ls_H_ls_ast_c__c_Structure_gr__gr_?M_CY_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr_?M>` 
  *  :ref:`find_arg (argn:string const;args:rtti::AnnotationArgumentList const) : variant\<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any\> <function-_at_ast_boost_c__c_find_arg_Cs_CH_ls_rtti_c__c_AnnotationArgumentList_gr_>` 
  *  :ref:`find_unique_function (mod:rtti::Module? const;name:string const;canfail:bool const) : smart_ptr\<ast::Function\> <function-_at_ast_boost_c__c_find_unique_function_C1_ls_H_ls_rtti_c__c_Module_gr__gr_?_Cs_Cb>` 
  *  :ref:`find_unique_generic (mod:rtti::Module? const;name:string const;canfail:bool const) : smart_ptr\<ast::Function\> <function-_at_ast_boost_c__c_find_unique_generic_C1_ls_H_ls_rtti_c__c_Module_gr__gr_?_Cs_Cb>` 
  *  :ref:`find_annotation (mod_name:string const;ann_name:string const) : rtti::Annotation const? <function-_at_ast_boost_c__c_find_annotation_Cs_Cs>` 
  *  :ref:`get_for_source_index (expr:smart_ptr\<ast::ExprFor\> const;svar:smart_ptr\<ast::Variable\> const) : int const <function-_at_ast_boost_c__c_get_for_source_index_C1_ls_H_ls_ast_c__c_ExprFor_gr__gr_?M_CY_ls_VariablePtr_gr_1_ls_H_ls_ast_c__c_Variable_gr__gr_?M>` 
  *  :ref:`get_for_source_index (expr:smart_ptr\<ast::ExprFor\> const;source:smart_ptr\<ast::Expression\> const) : int const <function-_at_ast_boost_c__c_get_for_source_index_C1_ls_H_ls_ast_c__c_ExprFor_gr__gr_?M_CY_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr_?M>` 

.. _function-_at_ast_boost_c__c_isVectorType_CE_ls_rtti_c__c_Type_gr_:

.. das:function:: isVectorType(typ: Type const)

isVectorType returns bool

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+typ     + :ref:`rtti::Type <enum-rtti-Type>`  const+
+--------+------------------------------------------+


|function-ast_boost-isVectorType|

.. _function-_at_ast_boost_c__c_isExpression_CY_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr_?M_Cb:

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

.. _function-_at_ast_boost_c__c_is_same_or_inherited_C1_ls_H_ls_ast_c__c_Structure_gr__gr_?_C1_ls_H_ls_ast_c__c_Structure_gr__gr_?:

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

.. _function-_at_ast_boost_c__c_is_class_method_CY_ls_StructurePtr_gr_1_ls_H_ls_ast_c__c_Structure_gr__gr_?M_CY_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr_?M:

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

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+argn    +string const                                                                    +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+


|function-ast_boost-find_arg|

.. _function-_at_ast_boost_c__c_find_unique_function_C1_ls_H_ls_rtti_c__c_Module_gr__gr_?_Cs_Cb:

.. das:function:: find_unique_function(mod: rtti::Module? const; name: string const; canfail: bool const)

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

.. _function-_at_ast_boost_c__c_find_unique_generic_C1_ls_H_ls_rtti_c__c_Module_gr__gr_?_Cs_Cb:

.. das:function:: find_unique_generic(mod: rtti::Module? const; name: string const; canfail: bool const)

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

.. _function-_at_ast_boost_c__c_get_for_source_index_C1_ls_H_ls_ast_c__c_ExprFor_gr__gr_?M_CY_ls_VariablePtr_gr_1_ls_H_ls_ast_c__c_Variable_gr__gr_?M:

.. das:function:: get_for_source_index(expr: smart_ptr<ast::ExprFor> const; svar: VariablePtr)

get_for_source_index returns int const

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+expr    +smart_ptr< :ref:`ast::ExprFor <handle-ast-ExprFor>` > const+
+--------+-----------------------------------------------------------+
+svar    + :ref:`VariablePtr <alias-VariablePtr>`                    +
+--------+-----------------------------------------------------------+


|function-ast_boost-get_for_source_index|

.. _function-_at_ast_boost_c__c_get_for_source_index_C1_ls_H_ls_ast_c__c_ExprFor_gr__gr_?M_CY_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr_?M:

.. das:function:: get_for_source_index(expr: smart_ptr<ast::ExprFor> const; source: ExpressionPtr)

get_for_source_index returns int const

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+expr    +smart_ptr< :ref:`ast::ExprFor <handle-ast-ExprFor>` > const+
+--------+-----------------------------------------------------------+
+source  + :ref:`ExpressionPtr <alias-ExpressionPtr>`                +
+--------+-----------------------------------------------------------+


|function-ast_boost-get_for_source_index|

+++++++++++
Annotations
+++++++++++

  *  :ref:`append_annotation (func:smart_ptr\<ast::Function\> -const;mod_name:string const;ann_name:string const;args:array\<tuple\<argname:string;argvalue:variant\<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any\>\>\> const) : void <function-_at_ast_boost_c__c_append_annotation_Y_ls_FunctionPtr_gr_1_ls_H_ls_ast_c__c_Function_gr__gr_?M_Cs_Cs_C1_ls_N_ls_argname;argvalue_gr_0_ls_s;FY_ls_RttiValue_gr_N_ls_tBool;tInt;tUInt;tInt64;tUInt64;tFloat;tDouble;tString;nothing_gr_0_ls_b;i;u;i64;u64;f;d;s;*_gr_V_gr_U_gr_A>` 
  *  :ref:`add_annotation_argument (arguments:rtti::AnnotationArgumentList -const;argName:string const;val:bool const) : int const <function-_at_ast_boost_c__c_add_annotation_argument_H_ls_rtti_c__c_AnnotationArgumentList_gr__Cs_Cb>` 
  *  :ref:`add_annotation_argument (arguments:rtti::AnnotationArgumentList -const;argName:string const;val:int const) : int const <function-_at_ast_boost_c__c_add_annotation_argument_H_ls_rtti_c__c_AnnotationArgumentList_gr__Cs_Ci>` 
  *  :ref:`add_annotation_argument (arguments:rtti::AnnotationArgumentList -const;argName:string const;val:float const) : int const <function-_at_ast_boost_c__c_add_annotation_argument_H_ls_rtti_c__c_AnnotationArgumentList_gr__Cs_Cf>` 
  *  :ref:`add_annotation_argument (arguments:rtti::AnnotationArgumentList -const;argName:string const;val:string const) : int const <function-_at_ast_boost_c__c_add_annotation_argument_H_ls_rtti_c__c_AnnotationArgumentList_gr__Cs_Cs>` 

.. _function-_at_ast_boost_c__c_append_annotation_Y_ls_FunctionPtr_gr_1_ls_H_ls_ast_c__c_Function_gr__gr_?M_Cs_Cs_C1_ls_N_ls_argname;argvalue_gr_0_ls_s;FY_ls_RttiValue_gr_N_ls_tBool;tInt;tUInt;tInt64;tUInt64;tFloat;tDouble;tString;nothing_gr_0_ls_b;i;u;i64;u64;f;d;s;*_gr_V_gr_U_gr_A:

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

+++++++++++++++++++++
Expression generation
+++++++++++++++++++++

  *  :ref:`override_method (str:smart_ptr\<ast::Structure\> -const;name:string const;funcName:string const) : bool <function-_at_ast_boost_c__c_override_method_Y_ls_StructurePtr_gr_1_ls_H_ls_ast_c__c_Structure_gr__gr_?M_Cs_Cs>` 
  *  :ref:`panic_expr_as () : void? <function-_at_ast_boost_c__c_panic_expr_as>` 
  *  :ref:`make_static_assert_false (text:string const;at:rtti::LineInfo const) : smart_ptr\<ast::ExprStaticAssert\> <function-_at_ast_boost_c__c_make_static_assert_false_Cs_CH_ls_rtti_c__c_LineInfo_gr_>` 
  *  :ref:`convert_to_expression (value:auto& -const;at:rtti::LineInfo const) : auto <function-_at_ast_boost_c__c_convert_to_expression_&._CH_ls_rtti_c__c_LineInfo_gr_>` 

.. _function-_at_ast_boost_c__c_override_method_Y_ls_StructurePtr_gr_1_ls_H_ls_ast_c__c_Structure_gr__gr_?M_Cs_Cs:

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

.. _function-_at_ast_boost_c__c_convert_to_expression_&._CH_ls_rtti_c__c_LineInfo_gr_:

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

+++++
Setup
+++++

  *  :ref:`setup_call_list (name:string const;at:rtti::LineInfo const;isInit:bool const;isPrivate:bool const) : ast::ExprBlock? <function-_at_ast_boost_c__c_setup_call_list_Cs_CH_ls_rtti_c__c_LineInfo_gr__Cb_Cb>` 
  *  :ref:`setup_macro (name:string const;at:rtti::LineInfo const) : ast::ExprBlock? <function-_at_ast_boost_c__c_setup_macro_Cs_CH_ls_rtti_c__c_LineInfo_gr_>` 
  *  :ref:`setup_tag_annotation (name:string const;tag:string const;classPtr:auto const) : auto <function-_at_ast_boost_c__c_setup_tag_annotation_Cs_Cs_C.>` 

.. _function-_at_ast_boost_c__c_setup_call_list_Cs_CH_ls_rtti_c__c_LineInfo_gr__Cb_Cb:

.. das:function:: setup_call_list(name: string const; at: LineInfo const; isInit: bool const; isPrivate: bool const)

setup_call_list returns  :ref:`ast::ExprBlock <handle-ast-ExprBlock>` ?

+---------+----------------------------------------------------+
+argument +argument type                                       +
+=========+====================================================+
+name     +string const                                        +
+---------+----------------------------------------------------+
+at       + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`  const+
+---------+----------------------------------------------------+
+isInit   +bool const                                          +
+---------+----------------------------------------------------+
+isPrivate+bool const                                          +
+---------+----------------------------------------------------+


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

.. _function-_at_ast_boost_c__c_setup_tag_annotation_Cs_Cs_C.:

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


