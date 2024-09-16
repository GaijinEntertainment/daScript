
.. _stdlib_ast_boost:

=========================
Boost package for the AST
=========================

The AST boost module implements collection of helper macros and functions to accompany :ref:`AST <stdlib_ast>`.

All functions and symbols are in "ast_boost" module, use require to get access to it. ::

    require daslib/ast_boost


++++++++++++
Type aliases
++++++++++++

.. _alias-AnnotationDeclarationPtr:

.. das:attribute:: AnnotationDeclarationPtr = smart_ptr<AnnotationDeclaration>

// stub


.. _alias-DebugExpressionFlags:

.. das:attribute:: DebugExpressionFlags is a bitfield

:Fields: * **refCount** (0)

// stub


++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-ast_boost-macro:

.. das:attribute:: macro

// stub


.. _handle-ast_boost-tag_function:

.. das:attribute:: tag_function

// stub


++++++++++++++
Variant macros
++++++++++++++

.. _call-macro-ast_boost-better_rtti_in_expr:

.. das:attribute:: better_rtti_in_expr

// stub


++++++++++++++++
Structure macros
++++++++++++++++

.. _handle-ast_boost-function_macro:

.. das:attribute:: function_macro

// stub


.. _handle-ast_boost-block_macro:

.. das:attribute:: block_macro

// stub


.. _handle-ast_boost-structure_macro:

.. das:attribute:: structure_macro

// stub


.. _handle-ast_boost-enumeration_macro:

.. das:attribute:: enumeration_macro

// stub


.. _handle-ast_boost-contract:

.. das:attribute:: contract

// stub


.. _handle-ast_boost-reader_macro:

.. das:attribute:: reader_macro

// stub


.. _handle-ast_boost-comment_reader:

.. das:attribute:: comment_reader

// stub


.. _handle-ast_boost-call_macro:

.. das:attribute:: call_macro

// stub


.. _handle-ast_boost-typeinfo_macro:

.. das:attribute:: typeinfo_macro

// stub


.. _handle-ast_boost-variant_macro:

.. das:attribute:: variant_macro

// stub


.. _handle-ast_boost-for_loop_macro:

.. das:attribute:: for_loop_macro

// stub


.. _handle-ast_boost-capture_macro:

.. das:attribute:: capture_macro

// stub


.. _handle-ast_boost-type_macro:

.. das:attribute:: type_macro

// stub


.. _handle-ast_boost-simulate_macro:

.. das:attribute:: simulate_macro

// stub


.. _handle-ast_boost-tag_structure:

.. das:attribute:: tag_structure

// stub


.. _handle-ast_boost-tag_function_macro:

.. das:attribute:: tag_function_macro

// stub


.. _handle-ast_boost-infer_macro:

.. das:attribute:: infer_macro

// stub


.. _handle-ast_boost-dirty_infer_macro:

.. das:attribute:: dirty_infer_macro

// stub


.. _handle-ast_boost-optimization_macro:

.. das:attribute:: optimization_macro

// stub


.. _handle-ast_boost-lint_macro:

.. das:attribute:: lint_macro

// stub


.. _handle-ast_boost-global_lint_macro:

.. das:attribute:: global_lint_macro

// stub


+++++++
Classes
+++++++

.. _struct-ast_boost-MacroMacro:

.. das:attribute:: MacroMacro : AstFunctionAnnotation

// stub


.. das:function:: MacroMacro.apply(func: FunctionPtr; group: ModuleGroup; args: AnnotationArgumentList; errors: das_string) : bool

// stub



:Arguments: * **func** :  :ref:`FunctionPtr <alias-FunctionPtr>` 

            * **group** :  :ref:`ModuleGroup <handle-rtti-ModuleGroup>` 

            * **args** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

            * **errors** :  :ref:`das_string <handle-builtin-das_string>` 

.. _struct-ast_boost-TagFunctionAnnotation:

.. das:attribute:: TagFunctionAnnotation : AstFunctionAnnotation

// stub


.. das:function:: TagFunctionAnnotation.apply(func: FunctionPtr; group: ModuleGroup; args: AnnotationArgumentList; errors: das_string) : bool

// stub



:Arguments: * **func** :  :ref:`FunctionPtr <alias-FunctionPtr>` 

            * **group** :  :ref:`ModuleGroup <handle-rtti-ModuleGroup>` 

            * **args** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

            * **errors** :  :ref:`das_string <handle-builtin-das_string>` 

.. _struct-ast_boost-TagStructureAnnotation:

.. das:attribute:: TagStructureAnnotation : AstStructureAnnotation

// stub


.. das:function:: TagStructureAnnotation.apply(str: StructurePtr; group: ModuleGroup; args: AnnotationArgumentList; errors: das_string) : bool

// stub



:Arguments: * **str** :  :ref:`StructurePtr <alias-StructurePtr>` 

            * **group** :  :ref:`ModuleGroup <handle-rtti-ModuleGroup>` 

            * **args** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

            * **errors** :  :ref:`das_string <handle-builtin-das_string>` 

.. _struct-ast_boost-SetupAnyAnnotation:

.. das:attribute:: SetupAnyAnnotation : AstStructureAnnotation

:Fields: * **annotation_function_call** : string = ""

         * **name** : string

// stub


.. das:function:: SetupAnyAnnotation.apply(st: StructurePtr; group: ModuleGroup; args: AnnotationArgumentList; errors: das_string) : bool

// stub



:Arguments: * **st** :  :ref:`StructurePtr <alias-StructurePtr>` 

            * **group** :  :ref:`ModuleGroup <handle-rtti-ModuleGroup>` 

            * **args** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

            * **errors** :  :ref:`das_string <handle-builtin-das_string>` 

.. das:function:: SetupAnyAnnotation.setup_call(st: StructurePtr; cll: smart_ptr<ExprCall>)

// stub



:Arguments: * **st** :  :ref:`StructurePtr <alias-StructurePtr>` 

            * **cll** : smart_ptr< :ref:`ExprCall <handle-ast-ExprCall>` >

.. _struct-ast_boost-SetupFunctionAnnotation:

.. das:attribute:: SetupFunctionAnnotation : SetupAnyAnnotation

// stub


.. _struct-ast_boost-SetupBlockAnnotation:

.. das:attribute:: SetupBlockAnnotation : SetupAnyAnnotation

// stub


.. _struct-ast_boost-SetupStructureAnnotation:

.. das:attribute:: SetupStructureAnnotation : SetupAnyAnnotation

// stub


.. _struct-ast_boost-SetupEnumerationAnnotation:

.. das:attribute:: SetupEnumerationAnnotation : SetupAnyAnnotation

// stub


.. _struct-ast_boost-SetupContractAnnotation:

.. das:attribute:: SetupContractAnnotation : SetupAnyAnnotation

// stub


.. _struct-ast_boost-SetupReaderMacro:

.. das:attribute:: SetupReaderMacro : SetupAnyAnnotation

// stub


.. _struct-ast_boost-SetupCommentReader:

.. das:attribute:: SetupCommentReader : SetupAnyAnnotation

// stub


.. _struct-ast_boost-SetupVariantMacro:

.. das:attribute:: SetupVariantMacro : SetupAnyAnnotation

// stub


.. _struct-ast_boost-SetupForLoopMacro:

.. das:attribute:: SetupForLoopMacro : SetupAnyAnnotation

// stub


.. _struct-ast_boost-SetupCaptureMacro:

.. das:attribute:: SetupCaptureMacro : SetupAnyAnnotation

// stub


.. _struct-ast_boost-SetupTypeMacro:

.. das:attribute:: SetupTypeMacro : SetupAnyAnnotation

// stub


.. _struct-ast_boost-SetupSimulateMacro:

.. das:attribute:: SetupSimulateMacro : SetupAnyAnnotation

// stub


.. _struct-ast_boost-SetupCallMacro:

.. das:attribute:: SetupCallMacro : SetupAnyAnnotation

// stub


.. _struct-ast_boost-SetupTypeInfoMacro:

.. das:attribute:: SetupTypeInfoMacro : SetupAnyAnnotation

// stub


.. _struct-ast_boost-SetupInferMacro:

.. das:attribute:: SetupInferMacro : SetupAnyAnnotation

// stub


.. _struct-ast_boost-SetupDirtyInferMacro:

.. das:attribute:: SetupDirtyInferMacro : SetupAnyAnnotation

// stub


.. _struct-ast_boost-SetupLintMacro:

.. das:attribute:: SetupLintMacro : SetupAnyAnnotation

// stub


.. _struct-ast_boost-SetupGlobalLintMacro:

.. das:attribute:: SetupGlobalLintMacro : SetupAnyAnnotation

// stub


.. _struct-ast_boost-SetupOptimizationMacro:

.. das:attribute:: SetupOptimizationMacro : SetupAnyAnnotation

// stub


.. _struct-ast_boost-TagFunctionMacro:

.. das:attribute:: TagFunctionMacro : SetupAnyAnnotation

:Fields: * **tag** : string

// stub


.. das:function:: TagFunctionMacro.apply(st: StructurePtr; group: ModuleGroup; args: AnnotationArgumentList; errors: das_string) : bool

// stub



:Arguments: * **st** :  :ref:`StructurePtr <alias-StructurePtr>` 

            * **group** :  :ref:`ModuleGroup <handle-rtti-ModuleGroup>` 

            * **args** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

            * **errors** :  :ref:`das_string <handle-builtin-das_string>` 

.. das:function:: TagFunctionMacro.setup_call(st: StructurePtr; cll: smart_ptr<ExprCall>)

// stub



:Arguments: * **st** :  :ref:`StructurePtr <alias-StructurePtr>` 

            * **cll** : smart_ptr< :ref:`ExprCall <handle-ast-ExprCall>` >

.. _struct-ast_boost-BetterRttiVisitor:

.. das:attribute:: BetterRttiVisitor : AstVariantMacro

// stub


.. das:function:: BetterRttiVisitor.visitExprIsVariant(prog: ProgramPtr; mod: Module?; expr: smart_ptr<ExprIsVariant>) : ExpressionPtr

// stub



:Arguments: * **prog** :  :ref:`ProgramPtr <alias-ProgramPtr>` 

            * **mod** :  :ref:`Module <handle-rtti-Module>` ?

            * **expr** : smart_ptr< :ref:`ExprIsVariant <handle-ast-ExprIsVariant>` >

.. das:function:: BetterRttiVisitor.visitExprAsVariant(prog: ProgramPtr; mod: Module?; expr: smart_ptr<ExprAsVariant>) : ExpressionPtr

// stub



:Arguments: * **prog** :  :ref:`ProgramPtr <alias-ProgramPtr>` 

            * **mod** :  :ref:`Module <handle-rtti-Module>` ?

            * **expr** : smart_ptr< :ref:`ExprAsVariant <handle-ast-ExprAsVariant>` >

.. das:function:: BetterRttiVisitor.visitExprSafeAsVariant(prog: ProgramPtr; mod: Module?; expr: smart_ptr<ExprSafeAsVariant>) : ExpressionPtr

// stub



:Arguments: * **prog** :  :ref:`ProgramPtr <alias-ProgramPtr>` 

            * **mod** :  :ref:`Module <handle-rtti-Module>` ?

            * **expr** : smart_ptr< :ref:`ExprSafeAsVariant <handle-ast-ExprSafeAsVariant>` >

++++++++++
Containers
++++++++++

  *  :ref:`emplace_new (var vec: dasvector`smart_ptr`Expression; var ptr: smart_ptr\<Expression\>) <function-_at_ast_boost_c__c_emplace_new_H_ls__builtin__c__c_dasvector_rq_smart_ptr_rq_Expression_gr__1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M>` 
  *  :ref:`emplace_new (var vec: dasvector`smart_ptr`TypeDecl; var ptr: smart_ptr\<TypeDecl\>) <function-_at_ast_boost_c__c_emplace_new_H_ls__builtin__c__c_dasvector_rq_smart_ptr_rq_TypeDecl_gr__1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M>` 
  *  :ref:`emplace_new (var vec: dasvector`smart_ptr`Variable; var ptr: smart_ptr\<Variable\>) <function-_at_ast_boost_c__c_emplace_new_H_ls__builtin__c__c_dasvector_rq_smart_ptr_rq_Variable_gr__1_ls_H_ls_ast_c__c_Variable_gr__gr__qm_M>` 
  *  :ref:`emplace_new (var vec: MakeStruct; var ptr: smart_ptr\<MakeFieldDecl\>) <function-_at_ast_boost_c__c_emplace_new_H_ls_ast_c__c_MakeStruct_gr__1_ls_H_ls_ast_c__c_MakeFieldDecl_gr__gr__qm_M>` 

.. _function-_at_ast_boost_c__c_emplace_new_H_ls__builtin__c__c_dasvector_rq_smart_ptr_rq_Expression_gr__1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M:

.. das:function:: emplace_new(vec: dasvector`smart_ptr`Expression; ptr: smart_ptr<Expression>)

// stub



:Arguments: * **vec** : vector<smart_ptr<Expression>>

            * **ptr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

.. _function-_at_ast_boost_c__c_emplace_new_H_ls__builtin__c__c_dasvector_rq_smart_ptr_rq_TypeDecl_gr__1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M:

.. das:function:: emplace_new(vec: dasvector`smart_ptr`TypeDecl; ptr: smart_ptr<TypeDecl>)

// stub



:Arguments: * **vec** : vector<smart_ptr<TypeDecl>>

            * **ptr** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

.. _function-_at_ast_boost_c__c_emplace_new_H_ls__builtin__c__c_dasvector_rq_smart_ptr_rq_Variable_gr__1_ls_H_ls_ast_c__c_Variable_gr__gr__qm_M:

.. das:function:: emplace_new(vec: dasvector`smart_ptr`Variable; ptr: smart_ptr<Variable>)

// stub



:Arguments: * **vec** : vector<smart_ptr<Variable>>

            * **ptr** : smart_ptr< :ref:`Variable <handle-ast-Variable>` >

.. _function-_at_ast_boost_c__c_emplace_new_H_ls_ast_c__c_MakeStruct_gr__1_ls_H_ls_ast_c__c_MakeFieldDecl_gr__gr__qm_M:

.. das:function:: emplace_new(vec: MakeStruct; ptr: smart_ptr<MakeFieldDecl>)

// stub



:Arguments: * **vec** :  :ref:`MakeStruct <handle-ast-MakeStruct>` 

            * **ptr** : smart_ptr< :ref:`MakeFieldDecl <handle-ast-MakeFieldDecl>` >

+++++++++++++++++++++++++++++++++++
Textual descriptions of the objects
+++++++++++++++++++++++++++++++++++

  *  :ref:`describe (list: AnnotationArgumentList) : string <function-_at_ast_boost_c__c_describe_CH_ls_rtti_c__c_AnnotationArgumentList_gr_>` 
  *  :ref:`describe (ann: AnnotationDeclaration) : string <function-_at_ast_boost_c__c_describe_CH_ls_rtti_c__c_AnnotationDeclaration_gr_>` 
  *  :ref:`describe (list: AnnotationList) : string <function-_at_ast_boost_c__c_describe_CH_ls_rtti_c__c_AnnotationList_gr_>` 
  *  :ref:`describe (vvar: VariablePtr) : string <function-_at_ast_boost_c__c_describe_CY_ls_VariablePtr_gr_1_ls_H_ls_ast_c__c_Variable_gr__gr__qm_M>` 
  *  :ref:`debug_expression (expr: ExpressionPtr; deFlags: DebugExpressionFlags = bitfield(0x0)) : string <function-_at_ast_boost_c__c_debug_expression_CY_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M_CY_ls_DebugExpressionFlags_gr_N_ls_refCount_gr_t>` 
  *  :ref:`debug_expression (expr: Expression?) : string <function-_at_ast_boost_c__c_debug_expression_C1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_>` 
  *  :ref:`describe (expr: Expression?) : string <function-_at_ast_boost_c__c_describe_C1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_>` 
  *  :ref:`describe_bitfield (bf: auto; merger: string = "") : auto <function-_at_ast_boost_c__c_describe_bitfield_C_dot__Cs>` 
  *  :ref:`describe_function_short (func: smart_ptr\<Function\>|Function?) : auto <function-_at_ast_boost_c__c_describe_function_short_C0_ls_CY_ls_FunctionPtr_gr_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M;C1_ls_H_ls_ast_c__c_Function_gr__gr__qm__gr_|>` 

.. _function-_at_ast_boost_c__c_describe_CH_ls_rtti_c__c_AnnotationArgumentList_gr_:

.. das:function:: describe(list: AnnotationArgumentList) : string

// stub



:Arguments: * **list** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

.. _function-_at_ast_boost_c__c_describe_CH_ls_rtti_c__c_AnnotationDeclaration_gr_:

.. das:function:: describe(ann: AnnotationDeclaration) : string

// stub



:Arguments: * **ann** :  :ref:`AnnotationDeclaration <handle-rtti-AnnotationDeclaration>` 

.. _function-_at_ast_boost_c__c_describe_CH_ls_rtti_c__c_AnnotationList_gr_:

.. das:function:: describe(list: AnnotationList) : string

// stub



:Arguments: * **list** :  :ref:`AnnotationList <handle-rtti-AnnotationList>` 

.. _function-_at_ast_boost_c__c_describe_CY_ls_VariablePtr_gr_1_ls_H_ls_ast_c__c_Variable_gr__gr__qm_M:

.. das:function:: describe(vvar: VariablePtr) : string

// stub



:Arguments: * **vvar** :  :ref:`VariablePtr <alias-VariablePtr>` 

.. _function-_at_ast_boost_c__c_debug_expression_CY_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M_CY_ls_DebugExpressionFlags_gr_N_ls_refCount_gr_t:

.. das:function:: debug_expression(expr: ExpressionPtr; deFlags: DebugExpressionFlags = bitfield(0x0)) : string

// stub



:Arguments: * **expr** :  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

            * **deFlags** :  :ref:`DebugExpressionFlags <alias-DebugExpressionFlags>` 

.. _function-_at_ast_boost_c__c_debug_expression_C1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_:

.. das:function:: debug_expression(expr: Expression?) : string

// stub



:Arguments: * **expr** :  :ref:`Expression <handle-ast-Expression>` ?

.. _function-_at_ast_boost_c__c_describe_C1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_:

.. das:function:: describe(expr: Expression?) : string

// stub



:Arguments: * **expr** :  :ref:`Expression <handle-ast-Expression>` ?

.. _function-_at_ast_boost_c__c_describe_bitfield_C_dot__Cs:

.. das:function:: describe_bitfield(bf: auto; merger: string = "") : auto

// stub



:Arguments: * **bf** : auto

            * **merger** : string

.. _function-_at_ast_boost_c__c_describe_function_short_C0_ls_CY_ls_FunctionPtr_gr_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M;C1_ls_H_ls_ast_c__c_Function_gr__gr__qm__gr_|:

.. das:function:: describe_function_short(func: smart_ptr<Function>|Function?) : auto

// stub



:Arguments: * **func** : option< :ref:`FunctionPtr <alias-FunctionPtr>` | :ref:`Function <handle-ast-Function>` ?>

+++++++
Queries
+++++++

  *  :ref:`isVectorType (typ: Type) : bool <function-_at_ast_boost_c__c_isVectorType_CE_ls_rtti_c__c_Type_gr_>` 
  *  :ref:`isExpression (t: TypeDeclPtr; top: bool = true) : bool <function-_at_ast_boost_c__c_isExpression_CY_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_Cb>` 
  *  :ref:`is_same_or_inherited (parent: Structure?; child: Structure?) : bool <function-_at_ast_boost_c__c_is_same_or_inherited_C1_ls_H_ls_ast_c__c_Structure_gr__gr__qm__C1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_>` 
  *  :ref:`is_class_method (cinfo: StructurePtr; finfo: TypeDeclPtr) : bool <function-_at_ast_boost_c__c_is_class_method_CY_ls_StructurePtr_gr_1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_M_CY_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M>` 
  *  :ref:`find_arg (argn: string; args: AnnotationArgumentList) : RttiValue <function-_at_ast_boost_c__c_find_arg_Cs_CH_ls_rtti_c__c_AnnotationArgumentList_gr_>` 
  *  :ref:`find_arg (args: AnnotationArgumentList; argn: string) : RttiValue <function-_at_ast_boost_c__c_find_arg_CH_ls_rtti_c__c_AnnotationArgumentList_gr__Cs>` 
  *  :ref:`find_unique_function (mod: Module?; name: string; canfail: bool = false) : smart_ptr\<Function\> <function-_at_ast_boost_c__c_find_unique_function_C1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__Cs_Cb>` 
  *  :ref:`find_unique_generic (mod: Module?; name: string; canfail: bool = false) : smart_ptr\<Function\> <function-_at_ast_boost_c__c_find_unique_generic_C1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__Cs_Cb>` 
  *  :ref:`find_annotation (mod_name: string; ann_name: string) : Annotation const? <function-_at_ast_boost_c__c_find_annotation_Cs_Cs>` 
  *  :ref:`get_for_source_index (expr: smart_ptr\<ExprFor\>; svar: VariablePtr) : int <function-_at_ast_boost_c__c_get_for_source_index_C1_ls_H_ls_ast_c__c_ExprFor_gr__gr__qm_M_CY_ls_VariablePtr_gr_1_ls_H_ls_ast_c__c_Variable_gr__gr__qm_M>` 
  *  :ref:`get_for_source_index (expr: smart_ptr\<ExprFor\>; source: ExpressionPtr) : int <function-_at_ast_boost_c__c_get_for_source_index_C1_ls_H_ls_ast_c__c_ExprFor_gr__gr__qm_M_CY_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M>` 
  *  :ref:`isCMRES (fun: FunctionPtr) : bool <function-_at_ast_boost_c__c_isCMRES_CY_ls_FunctionPtr_gr_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M>` 
  *  :ref:`isCMRES (fun: Function?) : bool <function-_at_ast_boost_c__c_isCMRES_C1_ls_H_ls_ast_c__c_Function_gr__gr__qm_>` 
  *  :ref:`isMakeLocal (expr: ExpressionPtr) : bool <function-_at_ast_boost_c__c_isMakeLocal_CY_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M>` 
  *  :ref:`get_workhorse_types () : Type[30] <function-_at_ast_boost_c__c_get_workhorse_types>` 
  *  :ref:`find_argument_index (typ: TypeDeclPtr; name: string) : int <function-_at_ast_boost_c__c_find_argument_index_CY_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_Cs>` 
  *  :ref:`isCMRESType (blockT: TypeDeclPtr) : bool <function-_at_ast_boost_c__c_isCMRESType_CY_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M>` 
  *  :ref:`getVectorElementCount (bt: Type) : int <function-_at_ast_boost_c__c_getVectorElementCount_CE_ls_rtti_c__c_Type_gr_>` 
  *  :ref:`getVectorElementSize (bt: Type) : int <function-_at_ast_boost_c__c_getVectorElementSize_CE_ls_rtti_c__c_Type_gr_>` 
  *  :ref:`getVectorElementType (bt: Type) : Type <function-_at_ast_boost_c__c_getVectorElementType_CE_ls_rtti_c__c_Type_gr_>` 
  *  :ref:`getVectorOffset (bt: Type; ident: string) : int <function-_at_ast_boost_c__c_getVectorOffset_CE_ls_rtti_c__c_Type_gr__Cs>` 

.. _function-_at_ast_boost_c__c_isVectorType_CE_ls_rtti_c__c_Type_gr_:

.. das:function:: isVectorType(typ: Type) : bool

// stub



:Arguments: * **typ** :  :ref:`Type <enum-rtti-Type>` 

.. _function-_at_ast_boost_c__c_isExpression_CY_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_Cb:

.. das:function:: isExpression(t: TypeDeclPtr; top: bool = true) : bool

// stub



:Arguments: * **t** :  :ref:`TypeDeclPtr <alias-TypeDeclPtr>` 

            * **top** : bool

.. _function-_at_ast_boost_c__c_is_same_or_inherited_C1_ls_H_ls_ast_c__c_Structure_gr__gr__qm__C1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_:

.. das:function:: is_same_or_inherited(parent: Structure?; child: Structure?) : bool

// stub



:Arguments: * **parent** :  :ref:`Structure <handle-ast-Structure>` ?

            * **child** :  :ref:`Structure <handle-ast-Structure>` ?

.. _function-_at_ast_boost_c__c_is_class_method_CY_ls_StructurePtr_gr_1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_M_CY_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M:

.. das:function:: is_class_method(cinfo: StructurePtr; finfo: TypeDeclPtr) : bool

// stub



:Arguments: * **cinfo** :  :ref:`StructurePtr <alias-StructurePtr>` 

            * **finfo** :  :ref:`TypeDeclPtr <alias-TypeDeclPtr>` 

.. _function-_at_ast_boost_c__c_find_arg_Cs_CH_ls_rtti_c__c_AnnotationArgumentList_gr_:

.. das:function:: find_arg(argn: string; args: AnnotationArgumentList) : RttiValue

.. warning:: 
  This function is deprecated.

// stub



:Arguments: * **argn** : string

            * **args** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

.. _function-_at_ast_boost_c__c_find_arg_CH_ls_rtti_c__c_AnnotationArgumentList_gr__Cs:

.. das:function:: find_arg(args: AnnotationArgumentList; argn: string) : RttiValue

// stub



:Arguments: * **args** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

            * **argn** : string

.. _function-_at_ast_boost_c__c_find_unique_function_C1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__Cs_Cb:

.. das:function:: find_unique_function(mod: Module?; name: string; canfail: bool = false) : smart_ptr<Function>

// stub



:Arguments: * **mod** :  :ref:`Module <handle-rtti-Module>` ?

            * **name** : string

            * **canfail** : bool

.. _function-_at_ast_boost_c__c_find_unique_generic_C1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__Cs_Cb:

.. das:function:: find_unique_generic(mod: Module?; name: string; canfail: bool = false) : smart_ptr<Function>

// stub



:Arguments: * **mod** :  :ref:`Module <handle-rtti-Module>` ?

            * **name** : string

            * **canfail** : bool

.. _function-_at_ast_boost_c__c_find_annotation_Cs_Cs:

.. das:function:: find_annotation(mod_name: string; ann_name: string) : Annotation const?

// stub



:Arguments: * **mod_name** : string

            * **ann_name** : string

.. _function-_at_ast_boost_c__c_get_for_source_index_C1_ls_H_ls_ast_c__c_ExprFor_gr__gr__qm_M_CY_ls_VariablePtr_gr_1_ls_H_ls_ast_c__c_Variable_gr__gr__qm_M:

.. das:function:: get_for_source_index(expr: smart_ptr<ExprFor>; svar: VariablePtr) : int

// stub



:Arguments: * **expr** : smart_ptr< :ref:`ExprFor <handle-ast-ExprFor>` >

            * **svar** :  :ref:`VariablePtr <alias-VariablePtr>` 

.. _function-_at_ast_boost_c__c_get_for_source_index_C1_ls_H_ls_ast_c__c_ExprFor_gr__gr__qm_M_CY_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M:

.. das:function:: get_for_source_index(expr: smart_ptr<ExprFor>; source: ExpressionPtr) : int

// stub



:Arguments: * **expr** : smart_ptr< :ref:`ExprFor <handle-ast-ExprFor>` >

            * **source** :  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

.. _function-_at_ast_boost_c__c_isCMRES_CY_ls_FunctionPtr_gr_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M:

.. das:function:: isCMRES(fun: FunctionPtr) : bool

// stub



:Arguments: * **fun** :  :ref:`FunctionPtr <alias-FunctionPtr>` 

.. _function-_at_ast_boost_c__c_isCMRES_C1_ls_H_ls_ast_c__c_Function_gr__gr__qm_:

.. das:function:: isCMRES(fun: Function?) : bool

// stub



:Arguments: * **fun** :  :ref:`Function <handle-ast-Function>` ?

.. _function-_at_ast_boost_c__c_isMakeLocal_CY_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M:

.. das:function:: isMakeLocal(expr: ExpressionPtr) : bool

// stub



:Arguments: * **expr** :  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

.. _function-_at_ast_boost_c__c_get_workhorse_types:

.. das:function:: get_workhorse_types() : Type[30]

// stub


.. _function-_at_ast_boost_c__c_find_argument_index_CY_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_Cs:

.. das:function:: find_argument_index(typ: TypeDeclPtr; name: string) : int

// stub



:Arguments: * **typ** :  :ref:`TypeDeclPtr <alias-TypeDeclPtr>` 

            * **name** : string

.. _function-_at_ast_boost_c__c_isCMRESType_CY_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M:

.. das:function:: isCMRESType(blockT: TypeDeclPtr) : bool

// stub



:Arguments: * **blockT** :  :ref:`TypeDeclPtr <alias-TypeDeclPtr>` 

.. _function-_at_ast_boost_c__c_getVectorElementCount_CE_ls_rtti_c__c_Type_gr_:

.. das:function:: getVectorElementCount(bt: Type) : int

// stub



:Arguments: * **bt** :  :ref:`Type <enum-rtti-Type>` 

.. _function-_at_ast_boost_c__c_getVectorElementSize_CE_ls_rtti_c__c_Type_gr_:

.. das:function:: getVectorElementSize(bt: Type) : int

// stub



:Arguments: * **bt** :  :ref:`Type <enum-rtti-Type>` 

.. _function-_at_ast_boost_c__c_getVectorElementType_CE_ls_rtti_c__c_Type_gr_:

.. das:function:: getVectorElementType(bt: Type) : Type

// stub



:Arguments: * **bt** :  :ref:`Type <enum-rtti-Type>` 

.. _function-_at_ast_boost_c__c_getVectorOffset_CE_ls_rtti_c__c_Type_gr__Cs:

.. das:function:: getVectorOffset(bt: Type; ident: string) : int

// stub



:Arguments: * **bt** :  :ref:`Type <enum-rtti-Type>` 

            * **ident** : string

+++++++++++
Annotations
+++++++++++

  *  :ref:`append_annotation (mod_name: string; ann_name: string; args: array\<tuple\<argname:string;argvalue:variant\<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any\>\>\>) : smart_ptr\<AnnotationDeclaration\> <function-_at_ast_boost_c__c_append_annotation_Cs_Cs_C1_ls_N_ls_argname;argvalue_gr_0_ls_s;FY_ls_RttiValue_gr_N_ls_tBool;tInt;tUInt;tInt64;tUInt64;tFloat;tDouble;tString;nothing_gr_0_ls_b;i;u;i64;u64;f;d;s;_st__gr_V_gr_U_gr_A>` 
  *  :ref:`append_annotation (mod_name: string; ann_name: string) : smart_ptr\<AnnotationDeclaration\> <function-_at_ast_boost_c__c_append_annotation_Cs_Cs>` 
  *  :ref:`append_annotation (var func: FunctionPtr; mod_name: string; ann_name: string) <function-_at_ast_boost_c__c_append_annotation_Y_ls_FunctionPtr_gr_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M_Cs_Cs>` 
  *  :ref:`append_annotation (var blk: smart_ptr\<ExprBlock\>; mod_name: string; ann_name: string) <function-_at_ast_boost_c__c_append_annotation_1_ls_H_ls_ast_c__c_ExprBlock_gr__gr__qm_M_Cs_Cs>` 
  *  :ref:`append_annotation (var st: smart_ptr\<Structure\>; mod_name: string; ann_name: string) <function-_at_ast_boost_c__c_append_annotation_1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_M_Cs_Cs>` 
  *  :ref:`append_annotation (var func: FunctionPtr; mod_name: string; ann_name: string; args: array\<tuple\<argname:string;argvalue:variant\<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any\>\>\>) <function-_at_ast_boost_c__c_append_annotation_Y_ls_FunctionPtr_gr_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M_Cs_Cs_C1_ls_N_ls_argname;argvalue_gr_0_ls_s;FY_ls_RttiValue_gr_N_ls_tBool;tInt;tUInt;tInt64;tUInt64;tFloat;tDouble;tString;nothing_gr_0_ls_b;i;u;i64;u64;f;d;s;_st__gr_V_gr_U_gr_A>` 
  *  :ref:`append_annotation (var blk: smart_ptr\<ExprBlock\>; mod_name: string; ann_name: string; args: array\<tuple\<argname:string;argvalue:variant\<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any\>\>\>) <function-_at_ast_boost_c__c_append_annotation_1_ls_H_ls_ast_c__c_ExprBlock_gr__gr__qm_M_Cs_Cs_C1_ls_N_ls_argname;argvalue_gr_0_ls_s;FY_ls_RttiValue_gr_N_ls_tBool;tInt;tUInt;tInt64;tUInt64;tFloat;tDouble;tString;nothing_gr_0_ls_b;i;u;i64;u64;f;d;s;_st__gr_V_gr_U_gr_A>` 
  *  :ref:`append_annotation (var st: smart_ptr\<Structure\>; mod_name: string; ann_name: string; args: array\<tuple\<argname:string;argvalue:variant\<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any\>\>\>) <function-_at_ast_boost_c__c_append_annotation_1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_M_Cs_Cs_C1_ls_N_ls_argname;argvalue_gr_0_ls_s;FY_ls_RttiValue_gr_N_ls_tBool;tInt;tUInt;tInt64;tUInt64;tFloat;tDouble;tString;nothing_gr_0_ls_b;i;u;i64;u64;f;d;s;_st__gr_V_gr_U_gr_A>` 
  *  :ref:`add_annotation_argument (var arguments: AnnotationArgumentList; argName: string; val: bool) : int <function-_at_ast_boost_c__c_add_annotation_argument_H_ls_rtti_c__c_AnnotationArgumentList_gr__Cs_Cb>` 
  *  :ref:`add_annotation_argument (var arguments: AnnotationArgumentList; argName: string; val: int) : int <function-_at_ast_boost_c__c_add_annotation_argument_H_ls_rtti_c__c_AnnotationArgumentList_gr__Cs_Ci>` 
  *  :ref:`add_annotation_argument (var arguments: AnnotationArgumentList; argName: string; val: float) : int <function-_at_ast_boost_c__c_add_annotation_argument_H_ls_rtti_c__c_AnnotationArgumentList_gr__Cs_Cf>` 
  *  :ref:`add_annotation_argument (var arguments: AnnotationArgumentList; argName: string; val: string) : int <function-_at_ast_boost_c__c_add_annotation_argument_H_ls_rtti_c__c_AnnotationArgumentList_gr__Cs_Cs>` 
  *  :ref:`add_annotation_argument (var arguments: AnnotationArgumentList; ann: AnnotationArgument) : int <function-_at_ast_boost_c__c_add_annotation_argument_H_ls_rtti_c__c_AnnotationArgumentList_gr__CH_ls_rtti_c__c_AnnotationArgument_gr_>` 

.. _function-_at_ast_boost_c__c_append_annotation_Cs_Cs_C1_ls_N_ls_argname;argvalue_gr_0_ls_s;FY_ls_RttiValue_gr_N_ls_tBool;tInt;tUInt;tInt64;tUInt64;tFloat;tDouble;tString;nothing_gr_0_ls_b;i;u;i64;u64;f;d;s;_st__gr_V_gr_U_gr_A:

.. das:function:: append_annotation(mod_name: string; ann_name: string; args: array<tuple<argname:string;argvalue:variant<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any>>>) : smart_ptr<AnnotationDeclaration>

// stub



:Arguments: * **mod_name** : string

            * **ann_name** : string

            * **args** : array<tuple<argname:string;argvalue: :ref:`RttiValue <alias-RttiValue>` >>

.. _function-_at_ast_boost_c__c_append_annotation_Cs_Cs:

.. das:function:: append_annotation(mod_name: string; ann_name: string) : smart_ptr<AnnotationDeclaration>

// stub



:Arguments: * **mod_name** : string

            * **ann_name** : string

.. _function-_at_ast_boost_c__c_append_annotation_Y_ls_FunctionPtr_gr_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M_Cs_Cs:

.. das:function:: append_annotation(func: FunctionPtr; mod_name: string; ann_name: string)

// stub



:Arguments: * **func** :  :ref:`FunctionPtr <alias-FunctionPtr>` 

            * **mod_name** : string

            * **ann_name** : string

.. _function-_at_ast_boost_c__c_append_annotation_1_ls_H_ls_ast_c__c_ExprBlock_gr__gr__qm_M_Cs_Cs:

.. das:function:: append_annotation(blk: smart_ptr<ExprBlock>; mod_name: string; ann_name: string)

// stub



:Arguments: * **blk** : smart_ptr< :ref:`ExprBlock <handle-ast-ExprBlock>` >

            * **mod_name** : string

            * **ann_name** : string

.. _function-_at_ast_boost_c__c_append_annotation_1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_M_Cs_Cs:

.. das:function:: append_annotation(st: smart_ptr<Structure>; mod_name: string; ann_name: string)

// stub



:Arguments: * **st** : smart_ptr< :ref:`Structure <handle-ast-Structure>` >

            * **mod_name** : string

            * **ann_name** : string

.. _function-_at_ast_boost_c__c_append_annotation_Y_ls_FunctionPtr_gr_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M_Cs_Cs_C1_ls_N_ls_argname;argvalue_gr_0_ls_s;FY_ls_RttiValue_gr_N_ls_tBool;tInt;tUInt;tInt64;tUInt64;tFloat;tDouble;tString;nothing_gr_0_ls_b;i;u;i64;u64;f;d;s;_st__gr_V_gr_U_gr_A:

.. das:function:: append_annotation(func: FunctionPtr; mod_name: string; ann_name: string; args: array<tuple<argname:string;argvalue:variant<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any>>>)

// stub



:Arguments: * **func** :  :ref:`FunctionPtr <alias-FunctionPtr>` 

            * **mod_name** : string

            * **ann_name** : string

            * **args** : array<tuple<argname:string;argvalue: :ref:`RttiValue <alias-RttiValue>` >>

.. _function-_at_ast_boost_c__c_append_annotation_1_ls_H_ls_ast_c__c_ExprBlock_gr__gr__qm_M_Cs_Cs_C1_ls_N_ls_argname;argvalue_gr_0_ls_s;FY_ls_RttiValue_gr_N_ls_tBool;tInt;tUInt;tInt64;tUInt64;tFloat;tDouble;tString;nothing_gr_0_ls_b;i;u;i64;u64;f;d;s;_st__gr_V_gr_U_gr_A:

.. das:function:: append_annotation(blk: smart_ptr<ExprBlock>; mod_name: string; ann_name: string; args: array<tuple<argname:string;argvalue:variant<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any>>>)

// stub



:Arguments: * **blk** : smart_ptr< :ref:`ExprBlock <handle-ast-ExprBlock>` >

            * **mod_name** : string

            * **ann_name** : string

            * **args** : array<tuple<argname:string;argvalue: :ref:`RttiValue <alias-RttiValue>` >>

.. _function-_at_ast_boost_c__c_append_annotation_1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_M_Cs_Cs_C1_ls_N_ls_argname;argvalue_gr_0_ls_s;FY_ls_RttiValue_gr_N_ls_tBool;tInt;tUInt;tInt64;tUInt64;tFloat;tDouble;tString;nothing_gr_0_ls_b;i;u;i64;u64;f;d;s;_st__gr_V_gr_U_gr_A:

.. das:function:: append_annotation(st: smart_ptr<Structure>; mod_name: string; ann_name: string; args: array<tuple<argname:string;argvalue:variant<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any>>>)

// stub



:Arguments: * **st** : smart_ptr< :ref:`Structure <handle-ast-Structure>` >

            * **mod_name** : string

            * **ann_name** : string

            * **args** : array<tuple<argname:string;argvalue: :ref:`RttiValue <alias-RttiValue>` >>

.. _function-_at_ast_boost_c__c_add_annotation_argument_H_ls_rtti_c__c_AnnotationArgumentList_gr__Cs_Cb:

.. das:function:: add_annotation_argument(arguments: AnnotationArgumentList; argName: string; val: bool) : int

// stub



:Arguments: * **arguments** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

            * **argName** : string

            * **val** : bool

.. _function-_at_ast_boost_c__c_add_annotation_argument_H_ls_rtti_c__c_AnnotationArgumentList_gr__Cs_Ci:

.. das:function:: add_annotation_argument(arguments: AnnotationArgumentList; argName: string; val: int) : int

// stub



:Arguments: * **arguments** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

            * **argName** : string

            * **val** : int

.. _function-_at_ast_boost_c__c_add_annotation_argument_H_ls_rtti_c__c_AnnotationArgumentList_gr__Cs_Cf:

.. das:function:: add_annotation_argument(arguments: AnnotationArgumentList; argName: string; val: float) : int

// stub



:Arguments: * **arguments** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

            * **argName** : string

            * **val** : float

.. _function-_at_ast_boost_c__c_add_annotation_argument_H_ls_rtti_c__c_AnnotationArgumentList_gr__Cs_Cs:

.. das:function:: add_annotation_argument(arguments: AnnotationArgumentList; argName: string; val: string) : int

// stub



:Arguments: * **arguments** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

            * **argName** : string

            * **val** : string

.. _function-_at_ast_boost_c__c_add_annotation_argument_H_ls_rtti_c__c_AnnotationArgumentList_gr__CH_ls_rtti_c__c_AnnotationArgument_gr_:

.. das:function:: add_annotation_argument(arguments: AnnotationArgumentList; ann: AnnotationArgument) : int

// stub



:Arguments: * **arguments** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

            * **ann** :  :ref:`AnnotationArgument <handle-rtti-AnnotationArgument>` 

+++++++++++++++++++++
Expression generation
+++++++++++++++++++++

  *  :ref:`override_method (var str: StructurePtr; name: string; funcName: string) : bool <function-_at_ast_boost_c__c_override_method_Y_ls_StructurePtr_gr_1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_M_Cs_Cs>` 
  *  :ref:`panic_expr_as () : void? <function-_at_ast_boost_c__c_panic_expr_as>` 
  *  :ref:`make_static_assert_false (text: string; at: LineInfo) : smart_ptr\<ExprStaticAssert\> <function-_at_ast_boost_c__c_make_static_assert_false_Cs_CH_ls_rtti_c__c_LineInfo_gr_>` 
  *  :ref:`convert_to_expression (var value: auto& ==const; at: LineInfo) : auto <function-_at_ast_boost_c__c_convert_to_expression_&_eq__dot__CH_ls_rtti_c__c_LineInfo_gr_>` 
  *  :ref:`convert_to_expression (value: auto const ==const; at: LineInfo) : auto <function-_at_ast_boost_c__c_convert_to_expression_C_eq__dot__CH_ls_rtti_c__c_LineInfo_gr_>` 
  *  :ref:`convert_to_expression (var value: auto ==const) : auto <function-_at_ast_boost_c__c_convert_to_expression__eq__dot_>` 
  *  :ref:`convert_to_expression (value: auto const ==const) : auto <function-_at_ast_boost_c__c_convert_to_expression_C_eq__dot_>` 

.. _function-_at_ast_boost_c__c_override_method_Y_ls_StructurePtr_gr_1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_M_Cs_Cs:

.. das:function:: override_method(str: StructurePtr; name: string; funcName: string) : bool

// stub



:Arguments: * **str** :  :ref:`StructurePtr <alias-StructurePtr>` 

            * **name** : string

            * **funcName** : string

.. _function-_at_ast_boost_c__c_panic_expr_as:

.. das:function:: panic_expr_as() : void?

// stub


.. _function-_at_ast_boost_c__c_make_static_assert_false_Cs_CH_ls_rtti_c__c_LineInfo_gr_:

.. das:function:: make_static_assert_false(text: string; at: LineInfo) : smart_ptr<ExprStaticAssert>

// stub



:Arguments: * **text** : string

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

.. _function-_at_ast_boost_c__c_convert_to_expression_&_eq__dot__CH_ls_rtti_c__c_LineInfo_gr_:

.. das:function:: convert_to_expression(value: auto& ==const; at: LineInfo) : auto

// stub



:Arguments: * **value** : auto&!

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

.. _function-_at_ast_boost_c__c_convert_to_expression_C_eq__dot__CH_ls_rtti_c__c_LineInfo_gr_:

.. das:function:: convert_to_expression(value: auto const ==const; at: LineInfo) : auto

// stub



:Arguments: * **value** : auto!

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

.. _function-_at_ast_boost_c__c_convert_to_expression__eq__dot_:

.. das:function:: convert_to_expression(value: auto ==const) : auto

// stub



:Arguments: * **value** : auto!

.. _function-_at_ast_boost_c__c_convert_to_expression_C_eq__dot_:

.. das:function:: convert_to_expression(value: auto const ==const) : auto

// stub



:Arguments: * **value** : auto!

++++++++
Visitors
++++++++

  *  :ref:`visit_finally (blk: ExprBlock?; adapter: smart_ptr\<VisitorAdapter\>) <function-_at_ast_boost_c__c_visit_finally_C1_ls_H_ls_ast_c__c_ExprBlock_gr__gr__qm__C1_ls_H_ls_ast_c__c_VisitorAdapter_gr__gr__qm_M>` 

.. _function-_at_ast_boost_c__c_visit_finally_C1_ls_H_ls_ast_c__c_ExprBlock_gr__gr__qm__C1_ls_H_ls_ast_c__c_VisitorAdapter_gr__gr__qm_M:

.. das:function:: visit_finally(blk: ExprBlock?; adapter: smart_ptr<VisitorAdapter>)

// stub



:Arguments: * **blk** :  :ref:`ExprBlock <handle-ast-ExprBlock>` ?

            * **adapter** : smart_ptr< :ref:`VisitorAdapter <handle-ast-VisitorAdapter>` >

+++++++++++++++
Type generation
+++++++++++++++

  *  :ref:`function_to_type (fn: FunctionPtr) : TypeDeclPtr <function-_at_ast_boost_c__c_function_to_type_CY_ls_FunctionPtr_gr_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M>` 

.. _function-_at_ast_boost_c__c_function_to_type_CY_ls_FunctionPtr_gr_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M:

.. das:function:: function_to_type(fn: FunctionPtr) : TypeDeclPtr

// stub



:Arguments: * **fn** :  :ref:`FunctionPtr <alias-FunctionPtr>` 

+++++
Setup
+++++

  *  :ref:`setup_call_list (name: string; at: LineInfo; subblock: block\<(var fn:smart_ptr\<Function\>):void\>) : ExprBlock? <function-_at_ast_boost_c__c_setup_call_list_Cs_CH_ls_rtti_c__c_LineInfo_gr__CN_ls_fn_gr_0_ls_Y_ls_FunctionPtr_gr_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`setup_call_list (name: string; at: LineInfo; isInit: bool = false; isPrivate: bool = true; isLateInit: bool = false) : ExprBlock? <function-_at_ast_boost_c__c_setup_call_list_Cs_CH_ls_rtti_c__c_LineInfo_gr__Cb_Cb_Cb>` 
  *  :ref:`setup_macro (name: string; at: LineInfo) : ExprBlock? <function-_at_ast_boost_c__c_setup_macro_Cs_CH_ls_rtti_c__c_LineInfo_gr_>` 
  *  :ref:`setup_tag_annotation (name: string; tag: string; classPtr: auto) : auto <function-_at_ast_boost_c__c_setup_tag_annotation_Cs_Cs_C_dot_>` 

.. _function-_at_ast_boost_c__c_setup_call_list_Cs_CH_ls_rtti_c__c_LineInfo_gr__CN_ls_fn_gr_0_ls_Y_ls_FunctionPtr_gr_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M_gr_1_ls_v_gr__builtin_:

.. das:function:: setup_call_list(name: string; at: LineInfo; subblock: block<(var fn:smart_ptr<Function>):void>) : ExprBlock?

// stub



:Arguments: * **name** : string

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

            * **subblock** : block<(fn: :ref:`FunctionPtr <alias-FunctionPtr>` ):void>

.. _function-_at_ast_boost_c__c_setup_call_list_Cs_CH_ls_rtti_c__c_LineInfo_gr__Cb_Cb_Cb:

.. das:function:: setup_call_list(name: string; at: LineInfo; isInit: bool = false; isPrivate: bool = true; isLateInit: bool = false) : ExprBlock?

// stub



:Arguments: * **name** : string

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

            * **isInit** : bool

            * **isPrivate** : bool

            * **isLateInit** : bool

.. _function-_at_ast_boost_c__c_setup_macro_Cs_CH_ls_rtti_c__c_LineInfo_gr_:

.. das:function:: setup_macro(name: string; at: LineInfo) : ExprBlock?

// stub



:Arguments: * **name** : string

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

.. _function-_at_ast_boost_c__c_setup_tag_annotation_Cs_Cs_C_dot_:

.. das:function:: setup_tag_annotation(name: string; tag: string; classPtr: auto) : auto

// stub



:Arguments: * **name** : string

            * **tag** : string

            * **classPtr** : auto


