
.. _stdlib_ast_boost:

=========================
Boost package for the AST
=========================

The AST_BOOST module provides high-level utilities for working with the AST.
It includes helpers for creating expressions, types, and declarations,
quote-based AST construction, and common AST query and transformation
patterns used by macro authors.

All functions and symbols are in "ast_boost" module, use require to get access to it. ::

    require daslib/ast_boost

++++++++++++
Type aliases
++++++++++++

.. _alias-AnnotationDeclarationPtr:

.. das:attribute:: AnnotationDeclarationPtr = smart_ptr<AnnotationDeclaration>

Alias for smart_ptr<AnnotationDeclaration>

.. _alias-DebugExpressionFlags:

.. das:attribute:: bitfield DebugExpressionFlags

:Fields: * **refCount** (0x1) - Which things to print in debug_expression.


++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-ast_boost-macro:

.. das:attribute:: macro

MacroMacro function annotation.

.. _handle-ast_boost-tag_function:

.. das:attribute:: tag_function

TagFunctionAnnotation function annotation.

++++++++++++++
Variant macros
++++++++++++++

.. _call-macro-ast_boost-better_rtti_in_expr:

.. das:attribute:: better_rtti_in_expr

This macro is used to implement `is type`, `as type` and `?as type` runtime checks for the `Expression` class and its subclasses.

++++++++++++++++
Structure macros
++++++++++++++++

.. _handle-ast_boost-function_macro:

.. das:attribute:: function_macro

Turns AstFunctionAnnotation into a macro with the specified `name`.

.. _handle-ast_boost-block_macro:

.. das:attribute:: block_macro

Turns AstBlockAnnotation into a macro with the specified `name`.

.. _handle-ast_boost-structure_macro:

.. das:attribute:: structure_macro

Turns AstStructureAnnotation into a macro with the specified `name`.

.. _handle-ast_boost-enumeration_macro:

.. das:attribute:: enumeration_macro

Turns AstEnumerationAnnotation into a macro with the specified `name`.

.. _handle-ast_boost-contract:

.. das:attribute:: contract

Turns AstFunctionAnnotation into a contract macro with the specified `name`.

.. _handle-ast_boost-reader_macro:

.. das:attribute:: reader_macro

Turns AstReaderMacro into a macro with the specified `name`.

.. _handle-ast_boost-comment_reader:

.. das:attribute:: comment_reader

Turns AstCommentReader into a macro with the specified `name`.

.. _handle-ast_boost-call_macro:

.. das:attribute:: call_macro

Turns AstCallMacro into a macro with the specified `name`.

.. _handle-ast_boost-typeinfo_macro:

.. das:attribute:: typeinfo_macro

Turns AstTypeInfoMacro into a macro with the specified `name`.

.. _handle-ast_boost-variant_macro:

.. das:attribute:: variant_macro

Turns AstVariantMacro into a macro with the specified `name`.

.. _handle-ast_boost-for_loop_macro:

.. das:attribute:: for_loop_macro

Turns AstForLoopMacro into a macro with the specified `name`.

.. _handle-ast_boost-capture_macro:

.. das:attribute:: capture_macro

Turns AstCaptureMacro into a macro with the specified `name`.

.. _handle-ast_boost-type_macro:

.. das:attribute:: type_macro

Turns AstTypeMacro into a macro with the specified `name`.

.. _handle-ast_boost-simulate_macro:

.. das:attribute:: simulate_macro

Turns AstSimulateMacro into a macro with the specified `name`.

.. _handle-ast_boost-tag_structure:

.. das:attribute:: tag_structure

This macro implements [tag_structure] annotation, which allows to add tag (name) to a specific structure.

.. _handle-ast_boost-tag_function_macro:

.. das:attribute:: tag_function_macro

This macro implements [tag_function_macro] annotation, which allows to add an AstFunctionAnnotation to any function with a specific [tag_function(name)] tag.

.. _handle-ast_boost-infer_macro:

.. das:attribute:: infer_macro

Turns AstPassMacro into a macro with the specified 'name', which is called during the `infer` pass.

.. _handle-ast_boost-dirty_infer_macro:

.. das:attribute:: dirty_infer_macro

Turns AstPassMacro into a macro with the specified 'name', which is called during the `dirty infer` pass.

.. _handle-ast_boost-optimization_macro:

.. das:attribute:: optimization_macro

Turns AstPassMacro into a macro with the specified 'name', which is called during the `optimization` pass.

.. _handle-ast_boost-lint_macro:

.. das:attribute:: lint_macro

Turns AstPassMacro into a macro with the specified 'name', which is called during the `lint` pass.

.. _handle-ast_boost-global_lint_macro:

.. das:attribute:: global_lint_macro

Turns AstPassMacro into a macro with the specified 'name', which is called during the `global lint` pass.

+++++++
Classes
+++++++

.. _struct-ast_boost-MacroMacro:

.. das:attribute:: MacroMacro : AstFunctionAnnotation

This macro implements [macro] function annotation.


.. _function-ast_boost_MacroMacro_rq_apply_MacroMacro_FunctionPtr_ModuleGroup_AnnotationArgumentList_das_string:

.. das:function:: MacroMacro.apply(func: FunctionPtr; group: ModuleGroup; args: AnnotationArgumentList; errors: das_string) : bool

Implementation details of a function annotation macro.

:Arguments: * **func** :  :ref:`FunctionPtr <alias-FunctionPtr>` 

            * **group** :  :ref:`ModuleGroup <handle-rtti-ModuleGroup>` 

            * **args** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

            * **errors** :  :ref:`das_string <handle-builtin-das_string>` 

.. _struct-ast_boost-TagFunctionAnnotation:

.. das:attribute:: TagFunctionAnnotation : AstFunctionAnnotation

This annotation is used for tagging specific funcstion.


.. _function-ast_boost_TagFunctionAnnotation_rq_apply_TagFunctionAnnotation_FunctionPtr_ModuleGroup_AnnotationArgumentList_das_string:

.. das:function:: TagFunctionAnnotation.apply(func: FunctionPtr; group: ModuleGroup; args: AnnotationArgumentList; errors: das_string) : bool

Implementation details of a function annotation macro.

:Arguments: * **func** :  :ref:`FunctionPtr <alias-FunctionPtr>` 

            * **group** :  :ref:`ModuleGroup <handle-rtti-ModuleGroup>` 

            * **args** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

            * **errors** :  :ref:`das_string <handle-builtin-das_string>` 

.. _struct-ast_boost-TagStructureAnnotation:

.. das:attribute:: TagStructureAnnotation : AstStructureAnnotation

This annotation is used for tagging specific structure.


.. _function-ast_boost_TagStructureAnnotation_rq_apply_TagStructureAnnotation_StructurePtr_ModuleGroup_AnnotationArgumentList_das_string:

.. das:function:: TagStructureAnnotation.apply(str: StructurePtr; group: ModuleGroup; args: AnnotationArgumentList; errors: das_string) : bool

Implementation details of a function annotation macro.

:Arguments: * **str** :  :ref:`StructurePtr <alias-StructurePtr>` 

            * **group** :  :ref:`ModuleGroup <handle-rtti-ModuleGroup>` 

            * **args** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

            * **errors** :  :ref:`das_string <handle-builtin-das_string>` 

.. _struct-ast_boost-SetupAnyAnnotation:

.. das:attribute:: SetupAnyAnnotation : AstStructureAnnotation

This is base class for any annotation or macro setup.

:Fields: * **annotation_function_call** : string = "" - Function call name, which is used to setup any annotation.

         * **name** : string - Name of the annotation to setup.


.. _function-ast_boost_SetupAnyAnnotation_rq_apply_SetupAnyAnnotation_StructurePtr_ModuleGroup_AnnotationArgumentList_das_string:

.. das:function:: SetupAnyAnnotation.apply(st: StructurePtr; group: ModuleGroup; args: AnnotationArgumentList; errors: das_string) : bool

Implementation details for setting up any type of annotation in the ast_boost module.

:Arguments: * **st** :  :ref:`StructurePtr <alias-StructurePtr>` 

            * **group** :  :ref:`ModuleGroup <handle-rtti-ModuleGroup>` 

            * **args** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

            * **errors** :  :ref:`das_string <handle-builtin-das_string>` 

.. _function-ast_boost_SetupAnyAnnotation_rq_setup_call_SetupAnyAnnotation_StructurePtr_smart_ptr_ls_ExprCall_gr_:

.. das:function:: SetupAnyAnnotation.setup_call(st: StructurePtr; cll: smart_ptr<ExprCall>)

Implementation details for how the call is set up for any annotation.

:Arguments: * **st** :  :ref:`StructurePtr <alias-StructurePtr>` 

            * **cll** : smart_ptr< :ref:`ExprCall <handle-ast-ExprCall>` >

.. _struct-ast_boost-SetupFunctionAnnotation:

.. das:attribute:: SetupFunctionAnnotation : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_function_annotation" - This is base class for function annotation setup.


.. _struct-ast_boost-SetupBlockAnnotation:

.. das:attribute:: SetupBlockAnnotation : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_block_annotation" - This is base class for block annotation setup.


.. _struct-ast_boost-SetupStructureAnnotation:

.. das:attribute:: SetupStructureAnnotation : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_structure_annotation" - This is base class for structure annotation setup.


.. _struct-ast_boost-SetupEnumerationAnnotation:

.. das:attribute:: SetupEnumerationAnnotation : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_enumeration_annotation" - [enumration_macro] implementation.


.. _struct-ast_boost-SetupContractAnnotation:

.. das:attribute:: SetupContractAnnotation : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_contract_annotation" - This is base class for contract annotation setup.


.. _struct-ast_boost-SetupReaderMacro:

.. das:attribute:: SetupReaderMacro : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_reader_macro" - [reader_macro] implementation.


.. _struct-ast_boost-SetupCommentReader:

.. das:attribute:: SetupCommentReader : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_comment_reader" - [comment_reader] implementation.


.. _struct-ast_boost-SetupVariantMacro:

.. das:attribute:: SetupVariantMacro : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_variant_macro" - [variant_macro] implementation.


.. _struct-ast_boost-SetupForLoopMacro:

.. das:attribute:: SetupForLoopMacro : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_for_loop_macro" - [for_loop_macro] implementation.


.. _struct-ast_boost-SetupCaptureMacro:

.. das:attribute:: SetupCaptureMacro : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_capture_macro" - [capture_macro] implementation.


.. _struct-ast_boost-SetupTypeMacro:

.. das:attribute:: SetupTypeMacro : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_type_macro" - This is base class for type macro setup.


.. _struct-ast_boost-SetupSimulateMacro:

.. das:attribute:: SetupSimulateMacro : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_simulate_macro" - This is base class for a simulate macro.


.. _struct-ast_boost-SetupCallMacro:

.. das:attribute:: SetupCallMacro : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_call_macro" - [call_macro] implementation.


.. _struct-ast_boost-SetupTypeInfoMacro:

.. das:attribute:: SetupTypeInfoMacro : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_typeinfo_macro" - [typeinfo_macro] implementation.


.. _struct-ast_boost-SetupInferMacro:

.. das:attribute:: SetupInferMacro : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_infer_macro" - [infer_macro] implementation.


.. _struct-ast_boost-SetupDirtyInferMacro:

.. das:attribute:: SetupDirtyInferMacro : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_dirty_infer_macro" - [dirty_infer_macro] implementation.


.. _struct-ast_boost-SetupLintMacro:

.. das:attribute:: SetupLintMacro : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_lint_macro" - [lint_macro] implementation.


.. _struct-ast_boost-SetupGlobalLintMacro:

.. das:attribute:: SetupGlobalLintMacro : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_global_lint_macro" - [global_lint_macro] implementation.


.. _struct-ast_boost-SetupOptimizationMacro:

.. das:attribute:: SetupOptimizationMacro : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_optimization_macro" - [optimization_macro] implementation.


.. _struct-ast_boost-TagFunctionMacro:

.. das:attribute:: TagFunctionMacro : SetupAnyAnnotation

[tag_function_macro] implementation.

:Fields: * **annotation_function_call** : string = "setup_tag_annotation" - Name of the function call, which setups up the annotation.

         * **tag** : string - Name of the tag.


.. _function-ast_boost_TagFunctionMacro_rq_apply_TagFunctionMacro_StructurePtr_ModuleGroup_AnnotationArgumentList_das_string:

.. das:function:: TagFunctionMacro.apply(st: StructurePtr; group: ModuleGroup; args: AnnotationArgumentList; errors: das_string) : bool

Implementation details for setting up tag function annotation in the ast_boost module.

:Arguments: * **st** :  :ref:`StructurePtr <alias-StructurePtr>` 

            * **group** :  :ref:`ModuleGroup <handle-rtti-ModuleGroup>` 

            * **args** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

            * **errors** :  :ref:`das_string <handle-builtin-das_string>` 

.. _function-ast_boost_TagFunctionMacro_rq_setup_call_TagFunctionMacro_StructurePtr_smart_ptr_ls_ExprCall_gr_:

.. das:function:: TagFunctionMacro.setup_call(st: StructurePtr; cll: smart_ptr<ExprCall>)

Implementation details for how the call is set up for tag function annotation.

:Arguments: * **st** :  :ref:`StructurePtr <alias-StructurePtr>` 

            * **cll** : smart_ptr< :ref:`ExprCall <handle-ast-ExprCall>` >

.. _struct-ast_boost-BetterRttiVisitor:

.. das:attribute:: BetterRttiVisitor : AstVariantMacro

Implements `expr is type` and `expr as type` checks, using RTTI.


.. _function-ast_boost_BetterRttiVisitor_rq_visitExprIsVariant_BetterRttiVisitor_ProgramPtr_Module_q__smart_ptr_ls_ExprIsVariant_gr_:

.. das:function:: BetterRttiVisitor.visitExprIsVariant(prog: ProgramPtr; mod: Module?; expr: smart_ptr<ExprIsVariant>) : ExpressionPtr

For Expr* replaces 'is' with check of __rtti, and returns true if it matches.

:Arguments: * **prog** :  :ref:`ProgramPtr <alias-ProgramPtr>` 

            * **mod** :  :ref:`Module <handle-rtti-Module>` ?

            * **expr** : smart_ptr< :ref:`ExprIsVariant <handle-ast-ExprIsVariant>` >

.. _function-ast_boost_BetterRttiVisitor_rq_visitExprAsVariant_BetterRttiVisitor_ProgramPtr_Module_q__smart_ptr_ls_ExprAsVariant_gr_:

.. das:function:: BetterRttiVisitor.visitExprAsVariant(prog: ProgramPtr; mod: Module?; expr: smart_ptr<ExprAsVariant>) : ExpressionPtr

For Expr* replaced 'as' with cast, which checks __rtti.

:Arguments: * **prog** :  :ref:`ProgramPtr <alias-ProgramPtr>` 

            * **mod** :  :ref:`Module <handle-rtti-Module>` ?

            * **expr** : smart_ptr< :ref:`ExprAsVariant <handle-ast-ExprAsVariant>` >

.. _function-ast_boost_BetterRttiVisitor_rq_visitExprSafeAsVariant_BetterRttiVisitor_ProgramPtr_Module_q__smart_ptr_ls_ExprSafeAsVariant_gr_:

.. das:function:: BetterRttiVisitor.visitExprSafeAsVariant(prog: ProgramPtr; mod: Module?; expr: smart_ptr<ExprSafeAsVariant>) : ExpressionPtr

For Expr* replaces 'as' with cast, which checks __rtti.

:Arguments: * **prog** :  :ref:`ProgramPtr <alias-ProgramPtr>` 

            * **mod** :  :ref:`Module <handle-rtti-Module>` ?

            * **expr** : smart_ptr< :ref:`ExprSafeAsVariant <handle-ast-ExprSafeAsVariant>` >

++++++++++
Containers
++++++++++

  *  :ref:`emplace_new (var vec: dasvector`smart_ptr`Expression; var ptr: smart_ptr\<Expression\>) <function-ast_boost_emplace_new_dasvector_rq_smart_ptr_rq_Expression_smart_ptr_ls_Expression_gr_>` 
  *  :ref:`emplace_new (var vec: dasvector`smart_ptr`TypeDecl; var ptr: smart_ptr\<TypeDecl\>) <function-ast_boost_emplace_new_dasvector_rq_smart_ptr_rq_TypeDecl_smart_ptr_ls_TypeDecl_gr_>` 
  *  :ref:`emplace_new (var vec: dasvector`smart_ptr`Variable; var ptr: smart_ptr\<Variable\>) <function-ast_boost_emplace_new_dasvector_rq_smart_ptr_rq_Variable_smart_ptr_ls_Variable_gr_>` 
  *  :ref:`emplace_new (var vec: MakeStruct; var ptr: smart_ptr\<MakeFieldDecl\>) <function-ast_boost_emplace_new_MakeStruct_smart_ptr_ls_MakeFieldDecl_gr_>` 

.. _function-ast_boost_emplace_new_dasvector_rq_smart_ptr_rq_Expression_smart_ptr_ls_Expression_gr_:

.. das:function:: emplace_new(vec: dasvector`smart_ptr`Expression; ptr: smart_ptr<Expression>)

Emplaces newly created object into the container without memory leak (i.e. correct ptr_ref_count).

:Arguments: * **vec** : vector<smart_ptr<Expression>>

            * **ptr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

.. _function-ast_boost_emplace_new_dasvector_rq_smart_ptr_rq_TypeDecl_smart_ptr_ls_TypeDecl_gr_:

.. das:function:: emplace_new(vec: dasvector`smart_ptr`TypeDecl; ptr: smart_ptr<TypeDecl>)

Emplaces newly created object into the container without memory leak (i.e. correct ptr_ref_count).

:Arguments: * **vec** : vector<smart_ptr<TypeDecl>>

            * **ptr** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

.. _function-ast_boost_emplace_new_dasvector_rq_smart_ptr_rq_Variable_smart_ptr_ls_Variable_gr_:

.. das:function:: emplace_new(vec: dasvector`smart_ptr`Variable; ptr: smart_ptr<Variable>)

Emplaces newly created object into the container without memory leak (i.e. correct ptr_ref_count).

:Arguments: * **vec** : vector<smart_ptr<Variable>>

            * **ptr** : smart_ptr< :ref:`Variable <handle-ast-Variable>` >

.. _function-ast_boost_emplace_new_MakeStruct_smart_ptr_ls_MakeFieldDecl_gr_:

.. das:function:: emplace_new(vec: MakeStruct; ptr: smart_ptr<MakeFieldDecl>)

Emplaces newly created object into the container without memory leak (i.e. correct ptr_ref_count).

:Arguments: * **vec** :  :ref:`MakeStruct <handle-ast-MakeStruct>` 

            * **ptr** : smart_ptr< :ref:`MakeFieldDecl <handle-ast-MakeFieldDecl>` >

+++++++++++++++++++++++++++++++++++
Textual descriptions of the objects
+++++++++++++++++++++++++++++++++++

  *  :ref:`describe (list: AnnotationArgumentList) : string <function-ast_boost_describe_AnnotationArgumentList>` 
  *  :ref:`describe (ann: AnnotationDeclaration) : string <function-ast_boost_describe_AnnotationDeclaration>` 
  *  :ref:`describe (list: AnnotationList) : string <function-ast_boost_describe_AnnotationList>` 
  *  :ref:`describe (vvar: VariablePtr) : string <function-ast_boost_describe_VariablePtr>` 
  *  :ref:`debug_expression (expr: ExpressionPtr; deFlags: DebugExpressionFlags = bitfield(0x0)) : string <function-ast_boost_debug_expression_ExpressionPtr_DebugExpressionFlags>` 
  *  :ref:`debug_expression (expr: Expression?) : string <function-ast_boost_debug_expression_Expression_q_>` 
  *  :ref:`describe (expr: Expression?) : string <function-ast_boost_describe_Expression_q_>` 
  *  :ref:`describe_bitfield (bf: auto; merger: string = "") : auto <function-ast_boost_describe_bitfield_auto_string>` 
  *  :ref:`describe_function_short (func: smart_ptr\<Function\>|Function?) : auto <function-ast_boost_describe_function_short_smart_ptr_ls_Function_gr_Function_q_>` 

.. _function-ast_boost_describe_AnnotationArgumentList:

.. das:function:: describe(list: AnnotationArgumentList) : string

Returns textual description of the object.

:Arguments: * **list** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

.. _function-ast_boost_describe_AnnotationDeclaration:

.. das:function:: describe(ann: AnnotationDeclaration) : string

Returns textual description of the object.

:Arguments: * **ann** :  :ref:`AnnotationDeclaration <handle-rtti-AnnotationDeclaration>` 

.. _function-ast_boost_describe_AnnotationList:

.. das:function:: describe(list: AnnotationList) : string

Returns textual description of the object.

:Arguments: * **list** :  :ref:`AnnotationList <handle-rtti-AnnotationList>` 

.. _function-ast_boost_describe_VariablePtr:

.. das:function:: describe(vvar: VariablePtr) : string

Returns textual description of the object.

:Arguments: * **vvar** :  :ref:`VariablePtr <alias-VariablePtr>` 

.. _function-ast_boost_debug_expression_ExpressionPtr_DebugExpressionFlags:

.. das:function:: debug_expression(expr: ExpressionPtr; deFlags: DebugExpressionFlags = bitfield(0x0)) : string

Gives hierarchical lisp-like textual representation of `expression` with all its subexpressions.

:Arguments: * **expr** :  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

            * **deFlags** :  :ref:`DebugExpressionFlags <alias-DebugExpressionFlags>` 

.. _function-ast_boost_debug_expression_Expression_q_:

.. das:function:: debug_expression(expr: Expression?) : string

Gives hierarchical lisp-like textual representation of `expression` with all its subexpressions.

:Arguments: * **expr** :  :ref:`Expression <handle-ast-Expression>` ?

.. _function-ast_boost_describe_Expression_q_:

.. das:function:: describe(expr: Expression?) : string

Returns textual description of the object.

:Arguments: * **expr** :  :ref:`Expression <handle-ast-Expression>` ?

.. _function-ast_boost_describe_bitfield_auto_string:

.. das:function:: describe_bitfield(bf: auto; merger: string = "") : auto

Returns textual description of the bitfield.

:Arguments: * **bf** : auto

            * **merger** : string

.. _function-ast_boost_describe_function_short_smart_ptr_ls_Function_gr_Function_q_:

.. das:function:: describe_function_short(func: smart_ptr<Function>|Function?) : auto

Gives short (name, arguments with types, result type) description of the function.

:Arguments: * **func** : option< :ref:`FunctionPtr <alias-FunctionPtr>` | :ref:`Function <handle-ast-Function>` ?>

+++++++
Queries
+++++++

  *  :ref:`isVectorType (typ: Type) : bool <function-ast_boost_isVectorType_Type>` 
  *  :ref:`isExpression (t: TypeDeclPtr; top: bool = true) : bool <function-ast_boost_isExpression_TypeDeclPtr_bool>` 
  *  :ref:`is_same_or_inherited (parent: Structure const?; child: Structure const?) : bool <function-ast_boost_is_same_or_inherited_Structure_const_q__Structure_const_q_>` 
  *  :ref:`is_class_method (cinfo: StructurePtr; finfo: TypeDeclPtr) : bool <function-ast_boost_is_class_method_StructurePtr_TypeDeclPtr>` 
  *  :ref:`find_arg (argn: string; args: AnnotationArgumentList) : RttiValue <function-ast_boost_find_arg_string_AnnotationArgumentList>` 
  *  :ref:`find_arg (args: AnnotationArgumentList; argn: string) : RttiValue <function-ast_boost_find_arg_AnnotationArgumentList_string>` 
  *  :ref:`find_unique_function (mod: Module?; name: string; canfail: bool = false) : smart_ptr\<Function\> <function-ast_boost_find_unique_function_Module_q__string_bool>` 
  *  :ref:`find_unique_generic (mod: Module?; name: string; canfail: bool = false) : smart_ptr\<Function\> <function-ast_boost_find_unique_generic_Module_q__string_bool>` 
  *  :ref:`find_annotation (mod_name: string; ann_name: string) : Annotation const? <function-ast_boost_find_annotation_string_string>` 
  *  :ref:`get_for_source_index (expr: smart_ptr\<ExprFor\>; svar: VariablePtr) : int <function-ast_boost_get_for_source_index_smart_ptr_ls_ExprFor_gr__VariablePtr>` 
  *  :ref:`get_for_source_index (expr: smart_ptr\<ExprFor\>; source: ExpressionPtr) : int <function-ast_boost_get_for_source_index_smart_ptr_ls_ExprFor_gr__ExpressionPtr>` 
  *  :ref:`isCMRES (fun: FunctionPtr) : bool <function-ast_boost_isCMRES_FunctionPtr>` 
  *  :ref:`isCMRES (fun: Function?) : bool <function-ast_boost_isCMRES_Function_q_>` 
  *  :ref:`isMakeLocal (expr: ExpressionPtr) : bool <function-ast_boost_isMakeLocal_ExpressionPtr>` 
  *  :ref:`isExprCallFunc (expr: ExpressionPtr) : bool <function-ast_boost_isExprCallFunc_ExpressionPtr>` 
  *  :ref:`get_workhorse_types () : Type[34] <function-ast_boost_get_workhorse_types>` 
  *  :ref:`find_argument_index (typ: TypeDeclPtr; name: string) : int <function-ast_boost_find_argument_index_TypeDeclPtr_string>` 
  *  :ref:`isCMRESType (blockT: TypeDeclPtr) : bool <function-ast_boost_isCMRESType_TypeDeclPtr>` 
  *  :ref:`getVectorElementCount (bt: Type) : int <function-ast_boost_getVectorElementCount_Type>` 
  *  :ref:`getVectorElementSize (bt: Type) : int <function-ast_boost_getVectorElementSize_Type>` 
  *  :ref:`getVectorElementType (bt: Type) : Type <function-ast_boost_getVectorElementType_Type>` 
  *  :ref:`getVectorOffset (bt: Type; ident: string) : int <function-ast_boost_getVectorOffset_Type_string>` 

.. _function-ast_boost_isVectorType_Type:

.. das:function:: isVectorType(typ: Type) : bool

Returns true if type is vector type, i.e. int2, float3, and such, including range and urange.

:Arguments: * **typ** :  :ref:`Type <enum-rtti-Type>` 

.. _function-ast_boost_isExpression_TypeDeclPtr_bool:

.. das:function:: isExpression(t: TypeDeclPtr; top: bool = true) : bool

Returns true if given object is derived from ast::Expression.

:Arguments: * **t** :  :ref:`TypeDeclPtr <alias-TypeDeclPtr>` 

            * **top** : bool

.. _function-ast_boost_is_same_or_inherited_Structure_const_q__Structure_const_q_:

.. das:function:: is_same_or_inherited(parent: Structure const?; child: Structure const?) : bool

Returns true if child is the same class as parent, or is inherited from the parent.

:Arguments: * **parent** :  :ref:`Structure <handle-ast-Structure>` ?

            * **child** :  :ref:`Structure <handle-ast-Structure>` ?

.. _function-ast_boost_is_class_method_StructurePtr_TypeDeclPtr:

.. das:function:: is_class_method(cinfo: StructurePtr; finfo: TypeDeclPtr) : bool

Returns true if field is a class method.

:Arguments: * **cinfo** :  :ref:`StructurePtr <alias-StructurePtr>` 

            * **finfo** :  :ref:`TypeDeclPtr <alias-TypeDeclPtr>` 

.. _function-ast_boost_find_arg_string_AnnotationArgumentList:

.. das:function:: find_arg(argn: string; args: AnnotationArgumentList) : RttiValue

.. warning:: 
  This function is deprecated.

Find argument in annotation argument list.

:Arguments: * **argn** : string

            * **args** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

.. _function-ast_boost_find_arg_AnnotationArgumentList_string:

.. das:function:: find_arg(args: AnnotationArgumentList; argn: string) : RttiValue

Find argument in annotation argument list.

:Arguments: * **args** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

            * **argn** : string

.. _function-ast_boost_find_unique_function_Module_q__string_bool:

.. das:function:: find_unique_function(mod: Module?; name: string; canfail: bool = false) : smart_ptr<Function>

Returns unique function of that specific name, or null if there is none or more than one.

:Arguments: * **mod** :  :ref:`Module <handle-rtti-Module>` ?

            * **name** : string

            * **canfail** : bool

.. _function-ast_boost_find_unique_generic_Module_q__string_bool:

.. das:function:: find_unique_generic(mod: Module?; name: string; canfail: bool = false) : smart_ptr<Function>

Returns unique generic function of that specific name, or null if there is none or more than one.

:Arguments: * **mod** :  :ref:`Module <handle-rtti-Module>` ?

            * **name** : string

            * **canfail** : bool

.. _function-ast_boost_find_annotation_string_string:

.. das:function:: find_annotation(mod_name: string; ann_name: string) : Annotation const?

Finds annotation in the module.

:Arguments: * **mod_name** : string

            * **ann_name** : string

.. _function-ast_boost_get_for_source_index_smart_ptr_ls_ExprFor_gr__VariablePtr:

.. das:function:: get_for_source_index(expr: smart_ptr<ExprFor>; svar: VariablePtr) : int

Find index of the for loop source variable.

:Arguments: * **expr** : smart_ptr< :ref:`ExprFor <handle-ast-ExprFor>` >

            * **svar** :  :ref:`VariablePtr <alias-VariablePtr>` 

.. _function-ast_boost_get_for_source_index_smart_ptr_ls_ExprFor_gr__ExpressionPtr:

.. das:function:: get_for_source_index(expr: smart_ptr<ExprFor>; source: ExpressionPtr) : int

Find index of the for loop source variable.

:Arguments: * **expr** : smart_ptr< :ref:`ExprFor <handle-ast-ExprFor>` >

            * **source** :  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

.. _function-ast_boost_isCMRES_FunctionPtr:

.. das:function:: isCMRES(fun: FunctionPtr) : bool

Returns true if function returns result by copy-or-move on the stack, as oppose to through the register ABI.

:Arguments: * **fun** :  :ref:`FunctionPtr <alias-FunctionPtr>` 

.. _function-ast_boost_isCMRES_Function_q_:

.. das:function:: isCMRES(fun: Function?) : bool

Returns true if function returns result by copy-or-move on the stack, as oppose to through the register ABI.

:Arguments: * **fun** :  :ref:`Function <handle-ast-Function>` ?

.. _function-ast_boost_isMakeLocal_ExpressionPtr:

.. das:function:: isMakeLocal(expr: ExpressionPtr) : bool

Returns true if Expression is inherited from ExprMakeLocal, i.e. ExprMakeArray, ExprMakeStruct, ExprMakeTuple, or ExprMakeVariant.

:Arguments: * **expr** :  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

.. _function-ast_boost_isExprCallFunc_ExpressionPtr:

.. das:function:: isExprCallFunc(expr: ExpressionPtr) : bool

Returns true if expression is ExprCallFunction.

:Arguments: * **expr** :  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

.. _function-ast_boost_get_workhorse_types:

.. das:function:: get_workhorse_types() : Type[34]

Returns array which contains all `workhorse` base types.

.. _function-ast_boost_find_argument_index_TypeDeclPtr_string:

.. das:function:: find_argument_index(typ: TypeDeclPtr; name: string) : int

Returns index of the specific argument name, or -1 if its not found.

:Arguments: * **typ** :  :ref:`TypeDeclPtr <alias-TypeDeclPtr>` 

            * **name** : string

.. _function-ast_boost_isCMRESType_TypeDeclPtr:

.. das:function:: isCMRESType(blockT: TypeDeclPtr) : bool

Returns true if type is copy-or-move on the stack, as oppose to through the register ABI.

:Arguments: * **blockT** :  :ref:`TypeDeclPtr <alias-TypeDeclPtr>` 

.. _function-ast_boost_getVectorElementCount_Type:

.. das:function:: getVectorElementCount(bt: Type) : int

Number of elements in the vector type, for example 3 for float3.

:Arguments: * **bt** :  :ref:`Type <enum-rtti-Type>` 

.. _function-ast_boost_getVectorElementSize_Type:

.. das:function:: getVectorElementSize(bt: Type) : int

Size of individual element in the vector type, for example 4 in float2 and 8 in range64.

:Arguments: * **bt** :  :ref:`Type <enum-rtti-Type>` 

.. _function-ast_boost_getVectorElementType_Type:

.. das:function:: getVectorElementType(bt: Type) : Type

Type of individual element in the vector type, for example float in float2.

:Arguments: * **bt** :  :ref:`Type <enum-rtti-Type>` 

.. _function-ast_boost_getVectorOffset_Type_string:

.. das:function:: getVectorOffset(bt: Type; ident: string) : int

Offset of the element in the vector type, for example 4 for "y" in float2.

:Arguments: * **bt** :  :ref:`Type <enum-rtti-Type>` 

            * **ident** : string

+++++++++++
Annotations
+++++++++++

  *  :ref:`append_annotation (mod_name: string; ann_name: string; args: array\<tuple\<argname:string;argvalue:variant\<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any\>\>\>) : smart_ptr\<AnnotationDeclaration\> <function-ast_boost_append_annotation_string_string_array_ls_tuple_ls_argname_c_string;argvalue_c_variant_ls_tBool_c_bool;tInt_c_int;tUInt_c_uint;tInt64_c_int64;tUInt64_c_uint64;tFloat_c_float;tDouble_c_double;tString_c_string;nothing_c_any_gr__gr__gr_>` 
  *  :ref:`append_annotation (mod_name: string; ann_name: string) : smart_ptr\<AnnotationDeclaration\> <function-ast_boost_append_annotation_string_string>` 
  *  :ref:`append_annotation (var func: FunctionPtr; mod_name: string; ann_name: string) <function-ast_boost_append_annotation_FunctionPtr_string_string>` 
  *  :ref:`append_annotation (var blk: smart_ptr\<ExprBlock\>; mod_name: string; ann_name: string) <function-ast_boost_append_annotation_smart_ptr_ls_ExprBlock_gr__string_string>` 
  *  :ref:`append_annotation (var st: smart_ptr\<Structure\>; mod_name: string; ann_name: string) <function-ast_boost_append_annotation_smart_ptr_ls_Structure_gr__string_string>` 
  *  :ref:`append_annotation (var func: FunctionPtr; mod_name: string; ann_name: string; args: array\<tuple\<argname:string;argvalue:variant\<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any\>\>\>) <function-ast_boost_append_annotation_FunctionPtr_string_string_array_ls_tuple_ls_argname_c_string;argvalue_c_variant_ls_tBool_c_bool;tInt_c_int;tUInt_c_uint;tInt64_c_int64;tUInt64_c_uint64;tFloat_c_float;tDouble_c_double;tString_c_string;nothing_c_any_gr__gr__gr_>` 
  *  :ref:`append_annotation (var blk: smart_ptr\<ExprBlock\>; mod_name: string; ann_name: string; args: array\<tuple\<argname:string;argvalue:variant\<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any\>\>\>) <function-ast_boost_append_annotation_smart_ptr_ls_ExprBlock_gr__string_string_array_ls_tuple_ls_argname_c_string;argvalue_c_variant_ls_tBool_c_bool;tInt_c_int;tUInt_c_uint;tInt64_c_int64;tUInt64_c_uint64;tFloat_c_float;tDouble_c_double;tString_c_string;nothing_c_any_gr__gr__gr_>` 
  *  :ref:`append_annotation (var st: smart_ptr\<Structure\>; mod_name: string; ann_name: string; args: array\<tuple\<argname:string;argvalue:variant\<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any\>\>\>) <function-ast_boost_append_annotation_smart_ptr_ls_Structure_gr__string_string_array_ls_tuple_ls_argname_c_string;argvalue_c_variant_ls_tBool_c_bool;tInt_c_int;tUInt_c_uint;tInt64_c_int64;tUInt64_c_uint64;tFloat_c_float;tDouble_c_double;tString_c_string;nothing_c_any_gr__gr__gr_>` 
  *  :ref:`add_annotation_argument (var arguments: AnnotationArgumentList; argName: string; val: bool) : int <function-ast_boost_add_annotation_argument_AnnotationArgumentList_string_bool>` 
  *  :ref:`add_annotation_argument (var arguments: AnnotationArgumentList; argName: string; val: int) : int <function-ast_boost_add_annotation_argument_AnnotationArgumentList_string_int>` 
  *  :ref:`add_annotation_argument (var arguments: AnnotationArgumentList; argName: string; val: float) : int <function-ast_boost_add_annotation_argument_AnnotationArgumentList_string_float>` 
  *  :ref:`add_annotation_argument (var arguments: AnnotationArgumentList; argName: string; val: string) : int <function-ast_boost_add_annotation_argument_AnnotationArgumentList_string_string>` 
  *  :ref:`add_annotation_argument (var arguments: AnnotationArgumentList; ann: AnnotationArgument) : int <function-ast_boost_add_annotation_argument_AnnotationArgumentList_AnnotationArgument>` 

.. _function-ast_boost_append_annotation_string_string_array_ls_tuple_ls_argname_c_string;argvalue_c_variant_ls_tBool_c_bool;tInt_c_int;tUInt_c_uint;tInt64_c_int64;tUInt64_c_uint64;tFloat_c_float;tDouble_c_double;tString_c_string;nothing_c_any_gr__gr__gr_:

.. das:function:: append_annotation(mod_name: string; ann_name: string; args: array<tuple<argname:string;argvalue:variant<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any>>>) : smart_ptr<AnnotationDeclaration>

Appends function annotation to the function given its name and arguments.

:Arguments: * **mod_name** : string

            * **ann_name** : string

            * **args** : array<tuple<argname:string;argvalue: :ref:`RttiValue <alias-RttiValue>` >>

.. _function-ast_boost_append_annotation_string_string:

.. das:function:: append_annotation(mod_name: string; ann_name: string) : smart_ptr<AnnotationDeclaration>

Appends function annotation to the function given its name and arguments.

:Arguments: * **mod_name** : string

            * **ann_name** : string

.. _function-ast_boost_append_annotation_FunctionPtr_string_string:

.. das:function:: append_annotation(func: FunctionPtr; mod_name: string; ann_name: string)

Appends function annotation to the function given its name and arguments.

:Arguments: * **func** :  :ref:`FunctionPtr <alias-FunctionPtr>` 

            * **mod_name** : string

            * **ann_name** : string

.. _function-ast_boost_append_annotation_smart_ptr_ls_ExprBlock_gr__string_string:

.. das:function:: append_annotation(blk: smart_ptr<ExprBlock>; mod_name: string; ann_name: string)

Appends function annotation to the function given its name and arguments.

:Arguments: * **blk** : smart_ptr< :ref:`ExprBlock <handle-ast-ExprBlock>` >

            * **mod_name** : string

            * **ann_name** : string

.. _function-ast_boost_append_annotation_smart_ptr_ls_Structure_gr__string_string:

.. das:function:: append_annotation(st: smart_ptr<Structure>; mod_name: string; ann_name: string)

Appends function annotation to the function given its name and arguments.

:Arguments: * **st** : smart_ptr< :ref:`Structure <handle-ast-Structure>` >

            * **mod_name** : string

            * **ann_name** : string

.. _function-ast_boost_append_annotation_FunctionPtr_string_string_array_ls_tuple_ls_argname_c_string;argvalue_c_variant_ls_tBool_c_bool;tInt_c_int;tUInt_c_uint;tInt64_c_int64;tUInt64_c_uint64;tFloat_c_float;tDouble_c_double;tString_c_string;nothing_c_any_gr__gr__gr_:

.. das:function:: append_annotation(func: FunctionPtr; mod_name: string; ann_name: string; args: array<tuple<argname:string;argvalue:variant<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any>>>)

Appends function annotation to the function given its name and arguments.

:Arguments: * **func** :  :ref:`FunctionPtr <alias-FunctionPtr>` 

            * **mod_name** : string

            * **ann_name** : string

            * **args** : array<tuple<argname:string;argvalue: :ref:`RttiValue <alias-RttiValue>` >>

.. _function-ast_boost_append_annotation_smart_ptr_ls_ExprBlock_gr__string_string_array_ls_tuple_ls_argname_c_string;argvalue_c_variant_ls_tBool_c_bool;tInt_c_int;tUInt_c_uint;tInt64_c_int64;tUInt64_c_uint64;tFloat_c_float;tDouble_c_double;tString_c_string;nothing_c_any_gr__gr__gr_:

.. das:function:: append_annotation(blk: smart_ptr<ExprBlock>; mod_name: string; ann_name: string; args: array<tuple<argname:string;argvalue:variant<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any>>>)

Appends function annotation to the function given its name and arguments.

:Arguments: * **blk** : smart_ptr< :ref:`ExprBlock <handle-ast-ExprBlock>` >

            * **mod_name** : string

            * **ann_name** : string

            * **args** : array<tuple<argname:string;argvalue: :ref:`RttiValue <alias-RttiValue>` >>

.. _function-ast_boost_append_annotation_smart_ptr_ls_Structure_gr__string_string_array_ls_tuple_ls_argname_c_string;argvalue_c_variant_ls_tBool_c_bool;tInt_c_int;tUInt_c_uint;tInt64_c_int64;tUInt64_c_uint64;tFloat_c_float;tDouble_c_double;tString_c_string;nothing_c_any_gr__gr__gr_:

.. das:function:: append_annotation(st: smart_ptr<Structure>; mod_name: string; ann_name: string; args: array<tuple<argname:string;argvalue:variant<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any>>>)

Appends function annotation to the function given its name and arguments.

:Arguments: * **st** : smart_ptr< :ref:`Structure <handle-ast-Structure>` >

            * **mod_name** : string

            * **ann_name** : string

            * **args** : array<tuple<argname:string;argvalue: :ref:`RttiValue <alias-RttiValue>` >>

.. _function-ast_boost_add_annotation_argument_AnnotationArgumentList_string_bool:

.. das:function:: add_annotation_argument(arguments: AnnotationArgumentList; argName: string; val: bool) : int

Adds annotation argument to the argument list.

:Arguments: * **arguments** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

            * **argName** : string

            * **val** : bool

.. _function-ast_boost_add_annotation_argument_AnnotationArgumentList_string_int:

.. das:function:: add_annotation_argument(arguments: AnnotationArgumentList; argName: string; val: int) : int

Adds annotation argument to the argument list.

:Arguments: * **arguments** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

            * **argName** : string

            * **val** : int

.. _function-ast_boost_add_annotation_argument_AnnotationArgumentList_string_float:

.. das:function:: add_annotation_argument(arguments: AnnotationArgumentList; argName: string; val: float) : int

Adds annotation argument to the argument list.

:Arguments: * **arguments** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

            * **argName** : string

            * **val** : float

.. _function-ast_boost_add_annotation_argument_AnnotationArgumentList_string_string:

.. das:function:: add_annotation_argument(arguments: AnnotationArgumentList; argName: string; val: string) : int

Adds annotation argument to the argument list.

:Arguments: * **arguments** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

            * **argName** : string

            * **val** : string

.. _function-ast_boost_add_annotation_argument_AnnotationArgumentList_AnnotationArgument:

.. das:function:: add_annotation_argument(arguments: AnnotationArgumentList; ann: AnnotationArgument) : int

Adds annotation argument to the argument list.

:Arguments: * **arguments** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

            * **ann** :  :ref:`AnnotationArgument <handle-rtti-AnnotationArgument>` 

+++++++++++++++++++++
Expression generation
+++++++++++++++++++++

  *  :ref:`override_method (var str: StructurePtr; name: string; funcName: string) : bool <function-ast_boost_override_method_StructurePtr_string_string>` 
  *  :ref:`panic_expr_as () : void? <function-ast_boost_panic_expr_as>` 
  *  :ref:`make_static_assert_false (text: string; at: LineInfo) : smart_ptr\<ExprStaticAssert\> <function-ast_boost_make_static_assert_false_string_LineInfo>` 
  *  :ref:`convert_to_expression (var value: auto& ==const; at: LineInfo) : auto <function-ast_boost_convert_to_expression__auto__eq__eq_const_LineInfo>` 
  *  :ref:`convert_to_expression (value: auto ==const; at: LineInfo) : auto <function-ast_boost_convert_to_expression_auto__eq__eq_const_LineInfo>` 
  *  :ref:`convert_to_expression (var value: auto ==const) : auto <function-ast_boost_convert_to_expression__auto__eq__eq_const>` 
  *  :ref:`convert_to_expression (value: auto ==const) : auto <function-ast_boost_convert_to_expression_auto__eq__eq_const>` 

.. _function-ast_boost_override_method_StructurePtr_string_string:

.. das:function:: override_method(str: StructurePtr; name: string; funcName: string) : bool

Override class method `name` with new function.

:Arguments: * **str** :  :ref:`StructurePtr <alias-StructurePtr>` 

            * **name** : string

            * **funcName** : string

.. _function-ast_boost_panic_expr_as:

.. das:function:: panic_expr_as() : void?

Function call which panics with "invalid 'as' expression or null pointer dereference" message.

.. _function-ast_boost_make_static_assert_false_string_LineInfo:

.. das:function:: make_static_assert_false(text: string; at: LineInfo) : smart_ptr<ExprStaticAssert>

Creates `static_assert(false,text)` expression.

:Arguments: * **text** : string

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

.. _function-ast_boost_convert_to_expression__auto__eq__eq_const_LineInfo:

.. das:function:: convert_to_expression(value: auto& ==const; at: LineInfo) : auto

Converts value to expression, which generates this value.

:Arguments: * **value** : auto&!

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

.. _function-ast_boost_convert_to_expression_auto__eq__eq_const_LineInfo:

.. das:function:: convert_to_expression(value: auto ==const; at: LineInfo) : auto

Converts value to expression, which generates this value.

:Arguments: * **value** : auto!

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

.. _function-ast_boost_convert_to_expression__auto__eq__eq_const:

.. das:function:: convert_to_expression(value: auto ==const) : auto

Converts value to expression, which generates this value.

:Arguments: * **value** : auto!

.. _function-ast_boost_convert_to_expression_auto__eq__eq_const:

.. das:function:: convert_to_expression(value: auto ==const) : auto

Converts value to expression, which generates this value.

:Arguments: * **value** : auto!

++++++++
Visitors
++++++++

  *  :ref:`visit_finally (blk: ExprBlock?; adapter: smart_ptr\<VisitorAdapter\>) <function-ast_boost_visit_finally_ExprBlock_q__smart_ptr_ls_VisitorAdapter_gr_>` 

.. _function-ast_boost_visit_finally_ExprBlock_q__smart_ptr_ls_VisitorAdapter_gr_:

.. das:function:: visit_finally(blk: ExprBlock?; adapter: smart_ptr<VisitorAdapter>)

Calls visitor on the `finally` section of the block.

:Arguments: * **blk** :  :ref:`ExprBlock <handle-ast-ExprBlock>` ?

            * **adapter** : smart_ptr< :ref:`VisitorAdapter <handle-ast-VisitorAdapter>` >

+++++++++++++++
Type generation
+++++++++++++++

  *  :ref:`function_to_type (fn: FunctionPtr) : TypeDeclPtr <function-ast_boost_function_to_type_FunctionPtr>` 

.. _function-ast_boost_function_to_type_FunctionPtr:

.. das:function:: function_to_type(fn: FunctionPtr) : TypeDeclPtr

Returns TypeDeclPtr of the tFunction type, based on the provided function.

:Arguments: * **fn** :  :ref:`FunctionPtr <alias-FunctionPtr>` 

+++++
Setup
+++++

  *  :ref:`setup_call_list (name: string; at: LineInfo; subblock: block\<(var fn:FunctionPtr):void\>) : ExprBlock? <function-ast_boost_setup_call_list_string_LineInfo_block_ls_var_fn_c_FunctionPtr_c_void_gr_>` 
  *  :ref:`setup_call_list (name: string; at: LineInfo; isInit: bool = false; isPrivate: bool = true; isLateInit: bool = false) : ExprBlock? <function-ast_boost_setup_call_list_string_LineInfo_bool_bool_bool>` 
  *  :ref:`setup_macro (name: string; at: LineInfo) : ExprBlock? <function-ast_boost_setup_macro_string_LineInfo>` 
  *  :ref:`setup_tag_annotation (name: string; tag: string; var classPtr: auto) : auto <function-ast_boost_setup_tag_annotation_string_string_auto>` 

.. _function-ast_boost_setup_call_list_string_LineInfo_block_ls_var_fn_c_FunctionPtr_c_void_gr_:

.. das:function:: setup_call_list(name: string; at: LineInfo; subblock: block<(var fn:FunctionPtr):void>) : ExprBlock?

Create new function which will contain collection of calls. Returns body block to where the call is to be appended.

:Arguments: * **name** : string

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

            * **subblock** : block<(fn: :ref:`FunctionPtr <alias-FunctionPtr>` ):void>

.. _function-ast_boost_setup_call_list_string_LineInfo_bool_bool_bool:

.. das:function:: setup_call_list(name: string; at: LineInfo; isInit: bool = false; isPrivate: bool = true; isLateInit: bool = false) : ExprBlock?

Create new function which will contain collection of calls. Returns body block to where the call is to be appended.

:Arguments: * **name** : string

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

            * **isInit** : bool

            * **isPrivate** : bool

            * **isLateInit** : bool

.. _function-ast_boost_setup_macro_string_LineInfo:

.. das:function:: setup_macro(name: string; at: LineInfo) : ExprBlock?

Setup macro initialization function, which will only be called during compilation of this module. Returns body block to where the macro initialization is to be appended.

:Arguments: * **name** : string

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

.. _function-ast_boost_setup_tag_annotation_string_string_auto:

.. das:function:: setup_tag_annotation(name: string; tag: string; classPtr: auto) : auto

Creates annotation and applies it to all tagged functions given tag.

:Arguments: * **name** : string

            * **tag** : string

            * **classPtr** : auto

+++++++++++++
Uncategorized
+++++++++++++

.. _function-ast_boost__rq_is_rq_BuiltInFunction_Function_q_:

.. das:function:: Function?`is`BuiltInFunction(foo: Function?) : bool

Returns true if foo is BuiltInFunction.

:Arguments: * **foo** :  :ref:`Function <handle-ast-Function>` ?

.. _function-ast_boost__rq_as_rq_BuiltInFunction_Function_q_:

.. das:function:: Function?`as`BuiltInFunction(foo: Function?) : BuiltInFunction?

Returns the BuiltInFunction or panics.

:Arguments: * **foo** :  :ref:`Function <handle-ast-Function>` ?

.. _function-ast_boost__rq_is_rq_ExternalFnBase_Function_q_:

.. das:function:: Function?`is`ExternalFnBase(foo: Function?) : bool

Returns true if foo is ExternalFnBase.

:Arguments: * **foo** :  :ref:`Function <handle-ast-Function>` ?

.. _function-ast_boost__rq_as_rq_ExternalFnBase_Function_q_:

.. das:function:: Function?`as`ExternalFnBase(foo: Function?) : ExternalFnBase?

Returns ExternalFnBase or panics.

:Arguments: * **foo** :  :ref:`Function <handle-ast-Function>` ?

.. _function-ast_boost__rq_is_rq_FunctionAnnotation_Annotation_q_:

.. das:function:: Annotation?`is`FunctionAnnotation(foo: Annotation?) : bool

Returns true if foo is FunctionAnnotation.

:Arguments: * **foo** :  :ref:`Annotation <handle-rtti-Annotation>` ?

.. _function-ast_boost__rq_is_rq_FunctionAnnotation_smart_ptr_ls_Annotation_gr_:

.. das:function:: smart_ptr<Annotation>`is`FunctionAnnotation(foo: smart_ptr<Annotation>) : bool

Returns true if foo is FunctionAnnotation.

:Arguments: * **foo** : smart_ptr< :ref:`Annotation <handle-rtti-Annotation>` >

.. _function-ast_boost__rq_as_rq_FunctionAnnotation_Annotation_q_:

.. das:function:: Annotation?`as`FunctionAnnotation(foo: Annotation?) : FunctionAnnotation?

Returns FunctionAnnotation or panics.

:Arguments: * **foo** :  :ref:`Annotation <handle-rtti-Annotation>` ?

.. _function-ast_boost__rq_as_rq_FunctionAnnotation_smart_ptr_ls_Annotation_gr_:

.. das:function:: smart_ptr<Annotation>`as`FunctionAnnotation(foo: smart_ptr<Annotation>) : FunctionAnnotation?

Returns FunctionAnnotation or panics.

:Arguments: * **foo** : smart_ptr< :ref:`Annotation <handle-rtti-Annotation>` >

.. _function-ast_boost__rq_is_rq_StructureAnnotation_Annotation_q_:

.. das:function:: Annotation?`is`StructureAnnotation(foo: Annotation?) : bool

Returns true if foo is StructureAnnotation.

:Arguments: * **foo** :  :ref:`Annotation <handle-rtti-Annotation>` ?

.. _function-ast_boost__rq_is_rq_StructureAnnotation_smart_ptr_ls_Annotation_gr_:

.. das:function:: smart_ptr<Annotation>`is`StructureAnnotation(foo: smart_ptr<Annotation>) : bool

Returns true if foo is StructureAnnotation.

:Arguments: * **foo** : smart_ptr< :ref:`Annotation <handle-rtti-Annotation>` >

.. _function-ast_boost__rq_as_rq_StructureAnnotation_Annotation_q_:

.. das:function:: Annotation?`as`StructureAnnotation(foo: Annotation?) : StructureAnnotation?

Returns StructureAnnotation or panics.

:Arguments: * **foo** :  :ref:`Annotation <handle-rtti-Annotation>` ?

.. _function-ast_boost__rq_as_rq_StructureAnnotation_smart_ptr_ls_Annotation_gr_:

.. das:function:: smart_ptr<Annotation>`as`StructureAnnotation(foo: smart_ptr<Annotation>) : StructureAnnotation?

Returns StructureAnnotation or panics.

:Arguments: * **foo** : smart_ptr< :ref:`Annotation <handle-rtti-Annotation>` >

.. _function-ast_boost_walk_and_convert_uint8_const_q__TypeDeclPtr_LineInfo:

.. das:function:: walk_and_convert(data: uint8 const?; info: TypeDeclPtr; at: LineInfo) : ExpressionPtr

Walks data and converts it to an AST which generates such data.

:Arguments: * **data** : uint8?

            * **info** :  :ref:`TypeDeclPtr <alias-TypeDeclPtr>` 

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

.. _function-ast_boost__rq_is_rq_BuiltInFunction_auto:

.. das:function:: auto`is`BuiltInFunction(anything: auto) : auto

Returns true if foo is BuiltInFunction.

:Arguments: * **anything** : auto

.. _function-ast_boost__rq_is_rq_ExternalFnBase_auto:

.. das:function:: auto`is`ExternalFnBase(anything: auto) : auto

Returns true if foo is ExternalFnBase.

:Arguments: * **anything** : auto

.. _function-ast_boost__rq_is_rq_FunctionAnnotation_auto:

.. das:function:: auto`is`FunctionAnnotation(anything: auto) : auto

Returns true if foo is FunctionAnnotation.

:Arguments: * **anything** : auto

.. _function-ast_boost__rq_is_rq_StructureAnnotation_auto:

.. das:function:: auto`is`StructureAnnotation(anything: auto) : auto

Returns true if foo is StructureAnnotation.

:Arguments: * **anything** : auto


