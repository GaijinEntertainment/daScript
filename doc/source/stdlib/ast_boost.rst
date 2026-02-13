
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

Type alias for ``smart_ptr<AnnotationDeclaration>``, used when constructing or attaching annotation declarations to functions, blocks, or structures.
.. _alias-DebugExpressionFlags:

.. das:attribute:: bitfield DebugExpressionFlags

:Fields: * **refCount** (0x1) - Bitfield controlling ``debug_expression`` output — currently has a single ``refCount`` flag that includes smart pointer reference counts in the dump.


++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-ast_boost-macro:

.. das:attribute:: macro

The ``[macro]`` function annotation — marks a function to run only during macro module compilation, gating its body behind ``is_compiling_macros``.
.. _handle-ast_boost-tag_function:

.. das:attribute:: tag_function

The ``[tag_function]`` function annotation — attaches named tags to a function so that ``[tag_function_macro]``-based annotations can discover and process it.
++++++++++++++
Variant macros
++++++++++++++

.. _call-macro-ast_boost-better_rtti_in_expr:

.. das:attribute:: better_rtti_in_expr

Variant macro that enables improved RTTI type matching in `is` and `as` expressions.
Varian macro better_rtti_in_expr

++++++++++++++++
Structure macros
++++++++++++++++

.. _handle-ast_boost-function_macro:

.. das:attribute:: function_macro

The ``[function_macro]`` structure annotation — registers an ``AstFunctionAnnotation`` subclass as a named function annotation available to the compiler.
.. _handle-ast_boost-block_macro:

.. das:attribute:: block_macro

The ``[block_macro]`` structure annotation — registers an ``AstBlockAnnotation`` subclass as a named block-level annotation available to the compiler.
.. _handle-ast_boost-structure_macro:

.. das:attribute:: structure_macro

The ``[structure_macro]`` structure annotation — registers an ``AstStructureAnnotation`` subclass as a named annotation applicable to structures and classes.
.. _handle-ast_boost-enumeration_macro:

.. das:attribute:: enumeration_macro

The ``[enumeration_macro]`` structure annotation — registers an ``AstEnumerationAnnotation`` subclass as a named annotation applicable to enumerations.
.. _handle-ast_boost-contract:

.. das:attribute:: contract

The ``[contract]`` structure annotation — registers an ``AstFunctionAnnotation`` subclass as a named function contract that validates arguments or return values.
.. _handle-ast_boost-reader_macro:

.. das:attribute:: reader_macro

The ``[reader_macro]`` structure annotation — registers an ``AstReaderMacro`` subclass as a named reader macro invoked by the ``%name~...~~`` syntax during parsing.
.. _handle-ast_boost-comment_reader:

.. das:attribute:: comment_reader

The ``[comment_reader]`` structure annotation — registers an ``AstCommentReader`` subclass as a named comment reader invoked during parsing.
.. _handle-ast_boost-call_macro:

.. das:attribute:: call_macro

The ``[call_macro]`` structure annotation — registers an ``AstCallMacro`` subclass as a named call-expression macro available during compilation.
.. _handle-ast_boost-typeinfo_macro:

.. das:attribute:: typeinfo_macro

The ``[typeinfo_macro]`` structure annotation — registers an ``AstTypeInfoMacro`` subclass as a named macro that extends the ``typeinfo(name ...)`` built-in.
.. _handle-ast_boost-variant_macro:

.. das:attribute:: variant_macro

The ``[variant_macro]`` structure annotation — registers an ``AstVariantMacro`` subclass as a named macro that can customize ``is``, ``as``, and ``?as`` variant operations.
.. _handle-ast_boost-for_loop_macro:

.. das:attribute:: for_loop_macro

The ``[for_loop_macro]`` structure annotation — registers an ``AstForLoopMacro`` subclass as a named macro that can transform ``for`` loop expressions.
.. _handle-ast_boost-capture_macro:

.. das:attribute:: capture_macro

The ``[capture_macro]`` structure annotation — registers an ``AstCaptureMacro`` subclass as a named capture macro that can customize lambda capture behavior.
.. _handle-ast_boost-type_macro:

.. das:attribute:: type_macro

The ``[type_macro]`` structure annotation — registers an ``AstTypeMacro`` subclass as a named macro that can intercept and transform type expressions.
.. _handle-ast_boost-simulate_macro:

.. das:attribute:: simulate_macro

The ``[simulate_macro]`` structure annotation — registers an ``AstSimulateMacro`` subclass as a named macro invoked during the simulation (code generation) phase.
.. _handle-ast_boost-tag_structure:

.. das:attribute:: tag_structure

The ``[tag_structure]`` structure annotation — attaches named boolean tags to a structure, allowing macro code to discover and process tagged structures.
.. _handle-ast_boost-tag_function_macro:

.. das:attribute:: tag_function_macro

The ``[tag_function_macro]`` structure annotation — registers an ``AstFunctionAnnotation`` that is automatically applied to every function carrying a matching ``[tag_function(tag)]`` tag.
.. _handle-ast_boost-infer_macro:

.. das:attribute:: infer_macro

The ``[infer_macro]`` structure annotation — registers an ``AstPassMacro`` subclass that is invoked during the type inference compilation pass.
.. _handle-ast_boost-dirty_infer_macro:

.. das:attribute:: dirty_infer_macro

The ``[dirty_infer_macro]`` structure annotation — registers an ``AstPassMacro`` subclass that is invoked during the dirty infer compilation pass.
.. _handle-ast_boost-optimization_macro:

.. das:attribute:: optimization_macro

The ``[optimization_macro]`` structure annotation — registers an ``AstPassMacro`` subclass that is invoked during the optimization compilation pass.
.. _handle-ast_boost-lint_macro:

.. das:attribute:: lint_macro

The ``[lint_macro]`` structure annotation — registers an ``AstPassMacro`` subclass that is invoked during the lint compilation pass.
.. _handle-ast_boost-global_lint_macro:

.. das:attribute:: global_lint_macro

The ``[global_lint_macro]`` structure annotation — registers an ``AstPassMacro`` subclass that is invoked during the global lint compilation pass.
+++++++
Classes
+++++++

.. _struct-ast_boost-MacroMacro:

.. das:attribute:: MacroMacro : AstFunctionAnnotation

Implements the ``[macro]`` function annotation, which wraps the function body so it only executes during macro module compilation.


.. _function-ast_boost_MacroMacro_rq_apply_MacroMacro_FunctionPtr_ModuleGroup_AnnotationArgumentList_das_string_0xab:

.. das:function:: MacroMacro.apply(func: FunctionPtr; group: ModuleGroup; args: AnnotationArgumentList; errors: das_string) : bool

Wraps the annotated function body in an ``is_compiling_macros`` guard and sets ``macroInit`` flag so it only runs during macro module compilation.

:Arguments: * **func** :  :ref:`FunctionPtr <alias-FunctionPtr>` 

            * **group** :  :ref:`ModuleGroup <handle-rtti-ModuleGroup>` 

            * **args** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

            * **errors** :  :ref:`das_string <handle-builtin-das_string>` 

.. _struct-ast_boost-TagFunctionAnnotation:

.. das:attribute:: TagFunctionAnnotation : AstFunctionAnnotation

Implements the ``[tag_function]`` function annotation, which attaches named boolean tags to functions so they can be discovered and processed by ``[tag_function_macro]``.


.. _function-ast_boost_TagFunctionAnnotation_rq_apply_TagFunctionAnnotation_FunctionPtr_ModuleGroup_AnnotationArgumentList_das_string_0xba:

.. das:function:: TagFunctionAnnotation.apply(func: FunctionPtr; group: ModuleGroup; args: AnnotationArgumentList; errors: das_string) : bool

Validates that all ``[tag_function(...)]`` annotation arguments are tag names (boolean flags) and rejects any non-boolean arguments with an error.

:Arguments: * **func** :  :ref:`FunctionPtr <alias-FunctionPtr>` 

            * **group** :  :ref:`ModuleGroup <handle-rtti-ModuleGroup>` 

            * **args** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

            * **errors** :  :ref:`das_string <handle-builtin-das_string>` 

.. _struct-ast_boost-TagStructureAnnotation:

.. das:attribute:: TagStructureAnnotation : AstStructureAnnotation

Implements the ``[tag_structure]`` structure annotation, which attaches named boolean tags to structures for later discovery by macro code.


.. _function-ast_boost_TagStructureAnnotation_rq_apply_TagStructureAnnotation_StructurePtr_ModuleGroup_AnnotationArgumentList_das_string_0xc9:

.. das:function:: TagStructureAnnotation.apply(str: StructurePtr; group: ModuleGroup; args: AnnotationArgumentList; errors: das_string) : bool

Validates that all ``[tag_structure(...)]`` annotation arguments are tag names (boolean flags) and rejects any non-boolean arguments with an error.

:Arguments: * **str** :  :ref:`StructurePtr <alias-StructurePtr>` 

            * **group** :  :ref:`ModuleGroup <handle-rtti-ModuleGroup>` 

            * **args** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

            * **errors** :  :ref:`das_string <handle-builtin-das_string>` 

.. _struct-ast_boost-SetupAnyAnnotation:

.. das:attribute:: SetupAnyAnnotation : AstStructureAnnotation

This is base class for any annotation or macro setup.

:Fields: * **annotation_function_call** : string = "" - Function call name, which is used to setup any annotation.

         * **name** : string - Name of the annotation to setup.


.. _function-ast_boost_SetupAnyAnnotation_rq_apply_SetupAnyAnnotation_StructurePtr_ModuleGroup_AnnotationArgumentList_das_string_0x18f:

.. das:function:: SetupAnyAnnotation.apply(st: StructurePtr; group: ModuleGroup; args: AnnotationArgumentList; errors: das_string) : bool

Generates a macro-init function that constructs an instance of the annotated class and registers it with the compiler under the specified ``name``.

:Arguments: * **st** :  :ref:`StructurePtr <alias-StructurePtr>` 

            * **group** :  :ref:`ModuleGroup <handle-rtti-ModuleGroup>` 

            * **args** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

            * **errors** :  :ref:`das_string <handle-builtin-das_string>` 

.. _function-ast_boost_SetupAnyAnnotation_rq_setup_call_SetupAnyAnnotation_StructurePtr_smart_ptr_ls_ExprCall_gr__0x18b:

.. das:function:: SetupAnyAnnotation.setup_call(st: StructurePtr; cll: smart_ptr<ExprCall>)

Populates the registration call arguments — by default adds the annotation ``name`` as a string constant; overridden in subclasses to add extra parameters.

:Arguments: * **st** :  :ref:`StructurePtr <alias-StructurePtr>` 

            * **cll** : smart_ptr< :ref:`ExprCall <handle-ast-ExprCall>` >

.. _struct-ast_boost-SetupFunctionAnnotation:

.. das:attribute:: SetupFunctionAnnotation : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_function_annotation" - Base class for creating function annotations via the ``[function_macro]`` structure annotation; registers an ``AstFunctionAnnotation`` with the compiler.


.. _struct-ast_boost-SetupBlockAnnotation:

.. das:attribute:: SetupBlockAnnotation : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_block_annotation" - Base class for creating block annotations via the ``[block_macro]`` structure annotation; registers an ``AstBlockAnnotation`` with the compiler.


.. _struct-ast_boost-SetupStructureAnnotation:

.. das:attribute:: SetupStructureAnnotation : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_structure_annotation" - Base class for creating structure annotations via the ``[structure_macro]`` structure annotation; registers an ``AstStructureAnnotation`` with the compiler.


.. _struct-ast_boost-SetupEnumerationAnnotation:

.. das:attribute:: SetupEnumerationAnnotation : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_enumeration_annotation" - Base class for creating enumeration annotations via the ``[enumeration_macro]`` structure annotation; registers an ``AstEnumerationAnnotation`` with the compiler.


.. _struct-ast_boost-SetupContractAnnotation:

.. das:attribute:: SetupContractAnnotation : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_contract_annotation" - Base class for creating function contract annotations via the ``[contract]`` structure annotation; registers an ``AstFunctionAnnotation`` as a contract.


.. _struct-ast_boost-SetupReaderMacro:

.. das:attribute:: SetupReaderMacro : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_reader_macro" - Base class for creating reader macros via the ``[reader_macro]`` structure annotation; registers an ``AstReaderMacro`` with the compiler.


.. _struct-ast_boost-SetupCommentReader:

.. das:attribute:: SetupCommentReader : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_comment_reader" - Base class for creating comment readers via the ``[comment_reader]`` structure annotation; registers an ``AstCommentReader`` with the compiler.


.. _struct-ast_boost-SetupVariantMacro:

.. das:attribute:: SetupVariantMacro : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_variant_macro" - Base class for creating variant macros via the ``[variant_macro]`` structure annotation; registers an ``AstVariantMacro`` with the compiler.


.. _struct-ast_boost-SetupForLoopMacro:

.. das:attribute:: SetupForLoopMacro : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_for_loop_macro" - Base class for creating for-loop macros via the ``[for_loop_macro]`` structure annotation; registers an ``AstForLoopMacro`` with the compiler.


.. _struct-ast_boost-SetupCaptureMacro:

.. das:attribute:: SetupCaptureMacro : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_capture_macro" - Base class for creating capture macros via the ``[capture_macro]`` structure annotation; registers an ``AstCaptureMacro`` with the compiler.


.. _struct-ast_boost-SetupTypeMacro:

.. das:attribute:: SetupTypeMacro : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_type_macro" - Base class for creating type macros via the ``[type_macro]`` structure annotation; registers an ``AstTypeMacro`` with the compiler.


.. _struct-ast_boost-SetupSimulateMacro:

.. das:attribute:: SetupSimulateMacro : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_simulate_macro" - Base class for creating simulate macros via the ``[simulate_macro]`` structure annotation; registers an ``AstSimulateMacro`` with the compiler.


.. _struct-ast_boost-SetupCallMacro:

.. das:attribute:: SetupCallMacro : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_call_macro" - Base class for creating call macros via the ``[call_macro]`` structure annotation; registers an ``AstCallMacro`` with the compiler.


.. _struct-ast_boost-SetupTypeInfoMacro:

.. das:attribute:: SetupTypeInfoMacro : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_typeinfo_macro" - Base class for creating typeinfo macros via the ``[typeinfo_macro]`` structure annotation; registers an ``AstTypeInfoMacro`` with the compiler.


.. _struct-ast_boost-SetupInferMacro:

.. das:attribute:: SetupInferMacro : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_infer_macro" - Base class for creating infer pass macros via the ``[infer_macro]`` structure annotation; registers an ``AstPassMacro`` that runs during the type inference pass.


.. _struct-ast_boost-SetupDirtyInferMacro:

.. das:attribute:: SetupDirtyInferMacro : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_dirty_infer_macro" - Base class for creating dirty-infer pass macros via the ``[dirty_infer_macro]`` structure annotation; registers an ``AstPassMacro`` that runs during the dirty infer pass.


.. _struct-ast_boost-SetupLintMacro:

.. das:attribute:: SetupLintMacro : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_lint_macro" - Base class for creating lint pass macros via the ``[lint_macro]`` structure annotation; registers an ``AstPassMacro`` that runs during the lint pass.


.. _struct-ast_boost-SetupGlobalLintMacro:

.. das:attribute:: SetupGlobalLintMacro : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_global_lint_macro" - Base class for creating global lint pass macros via the ``[global_lint_macro]`` structure annotation; registers an ``AstPassMacro`` that runs during the global lint pass.


.. _struct-ast_boost-SetupOptimizationMacro:

.. das:attribute:: SetupOptimizationMacro : SetupAnyAnnotation

:Fields: * **annotation_function_call** : string = "add_new_optimization_macro" - Base class for creating optimization pass macros via the ``[optimization_macro]`` structure annotation; registers an ``AstPassMacro`` that runs during the optimization pass.


.. _struct-ast_boost-TagFunctionMacro:

.. das:attribute:: TagFunctionMacro : SetupAnyAnnotation

[tag_function_macro] implementation.

:Fields: * **annotation_function_call** : string = "setup_tag_annotation" - Name of the function call, which setups up the annotation.

         * **tag** : string - Name of the tag.


.. _function-ast_boost_TagFunctionMacro_rq_apply_TagFunctionMacro_StructurePtr_ModuleGroup_AnnotationArgumentList_das_string_0x20e:

.. das:function:: TagFunctionMacro.apply(st: StructurePtr; group: ModuleGroup; args: AnnotationArgumentList; errors: das_string) : bool

Extends ``SetupAnyAnnotation`` apply to extract the required ``tag`` argument and register a ``setup_tag_annotation`` call that links the annotation to tagged functions.

:Arguments: * **st** :  :ref:`StructurePtr <alias-StructurePtr>` 

            * **group** :  :ref:`ModuleGroup <handle-rtti-ModuleGroup>` 

            * **args** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

            * **errors** :  :ref:`das_string <handle-builtin-das_string>` 

.. _function-ast_boost_TagFunctionMacro_rq_setup_call_TagFunctionMacro_StructurePtr_smart_ptr_ls_ExprCall_gr__0x209:

.. das:function:: TagFunctionMacro.setup_call(st: StructurePtr; cll: smart_ptr<ExprCall>)

Overrides the default ``setup_call`` to pass both the annotation ``name`` and the ``tag`` string as arguments to ``setup_tag_annotation``.

:Arguments: * **st** :  :ref:`StructurePtr <alias-StructurePtr>` 

            * **cll** : smart_ptr< :ref:`ExprCall <handle-ast-ExprCall>` >

.. _struct-ast_boost-BetterRttiVisitor:

.. das:attribute:: BetterRttiVisitor : AstVariantMacro

An ``AstVariantMacro`` that replaces ``is``, ``as``, and ``?as`` variant operations on ``Expression`` subclasses with runtime ``__rtti`` string checks and casts.


.. _function-ast_boost_BetterRttiVisitor_rq_visitExprIsVariant_BetterRttiVisitor_ProgramPtr_Module_q__smart_ptr_ls_ExprIsVariant_gr__0x250:

.. das:function:: BetterRttiVisitor.visitExprIsVariant(prog: ProgramPtr; mod: Module?; expr: smart_ptr<ExprIsVariant>) : ExpressionPtr

Visitor override that replaces ``expr is Type`` on ``Expression`` subclasses with an ``__rtti`` string comparison, returning ``true`` if the runtime type matches.

:Arguments: * **prog** :  :ref:`ProgramPtr <alias-ProgramPtr>` 

            * **mod** :  :ref:`Module <handle-rtti-Module>` ?

            * **expr** : smart_ptr< :ref:`ExprIsVariant <handle-ast-ExprIsVariant>` >

.. _function-ast_boost_BetterRttiVisitor_rq_visitExprAsVariant_BetterRttiVisitor_ProgramPtr_Module_q__smart_ptr_ls_ExprAsVariant_gr__0x25a:

.. das:function:: BetterRttiVisitor.visitExprAsVariant(prog: ProgramPtr; mod: Module?; expr: smart_ptr<ExprAsVariant>) : ExpressionPtr

Visitor override that replaces ``expr as Type`` on ``Expression`` subclasses with an RTTI-checked cast via ``__rtti``, panicking on mismatch.

:Arguments: * **prog** :  :ref:`ProgramPtr <alias-ProgramPtr>` 

            * **mod** :  :ref:`Module <handle-rtti-Module>` ?

            * **expr** : smart_ptr< :ref:`ExprAsVariant <handle-ast-ExprAsVariant>` >

.. _function-ast_boost_BetterRttiVisitor_rq_visitExprSafeAsVariant_BetterRttiVisitor_ProgramPtr_Module_q__smart_ptr_ls_ExprSafeAsVariant_gr__0x261:

.. das:function:: BetterRttiVisitor.visitExprSafeAsVariant(prog: ProgramPtr; mod: Module?; expr: smart_ptr<ExprSafeAsVariant>) : ExpressionPtr

Visitor override that replaces ``expr ?as Type`` on ``Expression`` subclasses with an RTTI-checked cast via ``__rtti``, returning ``null`` on mismatch instead of panicking.

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

Moves a newly created ``smart_ptr`` (``Expression``, ``TypeDecl``, ``Variable``, or ``MakeFieldDecl``) into a vector container with correct reference counting.

:Arguments: * **vec** : vector<smart_ptr<Expression>>

            * **ptr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

.. _function-ast_boost_emplace_new_dasvector_rq_smart_ptr_rq_TypeDecl_smart_ptr_ls_TypeDecl_gr_:

.. das:function:: emplace_new(vec: dasvector`smart_ptr`TypeDecl; ptr: smart_ptr<TypeDecl>)

Moves a newly created ``smart_ptr`` (``Expression``, ``TypeDecl``, ``Variable``, or ``MakeFieldDecl``) into a vector container with correct reference counting.

:Arguments: * **vec** : vector<smart_ptr<TypeDecl>>

            * **ptr** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

.. _function-ast_boost_emplace_new_dasvector_rq_smart_ptr_rq_Variable_smart_ptr_ls_Variable_gr_:

.. das:function:: emplace_new(vec: dasvector`smart_ptr`Variable; ptr: smart_ptr<Variable>)

Moves a newly created ``smart_ptr`` (``Expression``, ``TypeDecl``, ``Variable``, or ``MakeFieldDecl``) into a vector container with correct reference counting.

:Arguments: * **vec** : vector<smart_ptr<Variable>>

            * **ptr** : smart_ptr< :ref:`Variable <handle-ast-Variable>` >

.. _function-ast_boost_emplace_new_MakeStruct_smart_ptr_ls_MakeFieldDecl_gr_:

.. das:function:: emplace_new(vec: MakeStruct; ptr: smart_ptr<MakeFieldDecl>)

Moves a newly created ``smart_ptr`` (``Expression``, ``TypeDecl``, ``Variable``, or ``MakeFieldDecl``) into a vector container with correct reference counting.

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
  *  :ref:`describe_bitfield (bf: auto; merger: string = "") : auto <function-ast_boost_describe_bitfield_auto_string_0x3d>` 
  *  :ref:`describe_function_short (func: smart_ptr\<Function\>|Function?) : auto <function-ast_boost_describe_function_short_smart_ptr_ls_Function_gr_Function_q_>` 

.. _function-ast_boost_describe_AnnotationArgumentList:

.. das:function:: describe(list: AnnotationArgumentList) : string

Returns a human-readable textual representation of an AST object (``AnnotationArgumentList``, ``AnnotationDeclaration``, ``AnnotationList``, ``Variable``, or ``Expression``).

:Arguments: * **list** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

.. _function-ast_boost_describe_AnnotationDeclaration:

.. das:function:: describe(ann: AnnotationDeclaration) : string

Returns a human-readable textual representation of an AST object (``AnnotationArgumentList``, ``AnnotationDeclaration``, ``AnnotationList``, ``Variable``, or ``Expression``).

:Arguments: * **ann** :  :ref:`AnnotationDeclaration <handle-rtti-AnnotationDeclaration>` 

.. _function-ast_boost_describe_AnnotationList:

.. das:function:: describe(list: AnnotationList) : string

Returns a human-readable textual representation of an AST object (``AnnotationArgumentList``, ``AnnotationDeclaration``, ``AnnotationList``, ``Variable``, or ``Expression``).

:Arguments: * **list** :  :ref:`AnnotationList <handle-rtti-AnnotationList>` 

.. _function-ast_boost_describe_VariablePtr:

.. das:function:: describe(vvar: VariablePtr) : string

Returns a human-readable textual representation of an AST object (``AnnotationArgumentList``, ``AnnotationDeclaration``, ``AnnotationList``, ``Variable``, or ``Expression``).

:Arguments: * **vvar** :  :ref:`VariablePtr <alias-VariablePtr>` 

.. _function-ast_boost_debug_expression_ExpressionPtr_DebugExpressionFlags:

.. das:function:: debug_expression(expr: ExpressionPtr; deFlags: DebugExpressionFlags = bitfield(0x0)) : string

Returns a hierarchical, Lisp-like textual dump of an ``ExpressionPtr`` tree showing RTTI types, field values, and nested sub-expressions for debugging.

:Arguments: * **expr** :  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

            * **deFlags** :  :ref:`DebugExpressionFlags <alias-DebugExpressionFlags>` 

.. _function-ast_boost_debug_expression_Expression_q_:

.. das:function:: debug_expression(expr: Expression?) : string

Returns a hierarchical, Lisp-like textual dump of an ``ExpressionPtr`` tree showing RTTI types, field values, and nested sub-expressions for debugging.

:Arguments: * **expr** :  :ref:`Expression <handle-ast-Expression>` ?

.. _function-ast_boost_describe_Expression_q_:

.. das:function:: describe(expr: Expression?) : string

Returns a human-readable textual representation of an AST object (``AnnotationArgumentList``, ``AnnotationDeclaration``, ``AnnotationList``, ``Variable``, or ``Expression``).

:Arguments: * **expr** :  :ref:`Expression <handle-ast-Expression>` ?

.. _function-ast_boost_describe_bitfield_auto_string_0x3d:

.. das:function:: describe_bitfield(bf: auto; merger: string = "") : auto

Returns a textual representation of the set bits in a bitfield value, listing the names of all active flags joined by the specified ``merger`` string.

:Arguments: * **bf** : auto

            * **merger** : string

.. _function-ast_boost_describe_function_short_smart_ptr_ls_Function_gr_Function_q_:

.. das:function:: describe_function_short(func: smart_ptr<Function>|Function?) : auto

Returns a compact signature string for a function in the form ``name (arg:Type; ...) : ReturnType``.

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

Returns ``true`` if the given ``Type`` is a vector, range, or urange type (``int2``..``int4``, ``uint2``..``uint4``, ``float2``..``float4``, ``range``, ``urange``, ``range64``, ``urange64``).

:Arguments: * **typ** :  :ref:`Type <enum-rtti-Type>` 

.. _function-ast_boost_isExpression_TypeDeclPtr_bool:

.. das:function:: isExpression(t: TypeDeclPtr; top: bool = true) : bool

Returns ``true`` if the given ``TypeDeclPtr`` refers to an ``ast`` module handled type whose name starts with ``Expr``, including pointer-to-expression types.

:Arguments: * **t** :  :ref:`TypeDeclPtr <alias-TypeDeclPtr>` 

            * **top** : bool

.. _function-ast_boost_is_same_or_inherited_Structure_const_q__Structure_const_q_:

.. das:function:: is_same_or_inherited(parent: Structure const?; child: Structure const?) : bool

Returns ``true`` if ``child`` is the same ``Structure`` as ``parent`` or is transitively inherited from ``parent`` by walking the parent chain.

:Arguments: * **parent** :  :ref:`Structure <handle-ast-Structure>` ?

            * **child** :  :ref:`Structure <handle-ast-Structure>` ?

.. _function-ast_boost_is_class_method_StructurePtr_TypeDeclPtr:

.. das:function:: is_class_method(cinfo: StructurePtr; finfo: TypeDeclPtr) : bool

Returns ``true`` if a ``TypeDeclPtr`` field represents a class method — a non-dim ``tFunction`` whose first argument is the class structure (or a parent of it).

:Arguments: * **cinfo** :  :ref:`StructurePtr <alias-StructurePtr>` 

            * **finfo** :  :ref:`TypeDeclPtr <alias-TypeDeclPtr>` 

.. _function-ast_boost_find_arg_string_AnnotationArgumentList:

.. das:function:: find_arg(argn: string; args: AnnotationArgumentList) : RttiValue

.. warning:: 
  This function is deprecated.

Searches an ``AnnotationArgumentList`` for an argument by name and returns its ``RttiValue``; returns ``nothing`` if the argument is not present.

:Arguments: * **argn** : string

            * **args** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

.. _function-ast_boost_find_arg_AnnotationArgumentList_string:

.. das:function:: find_arg(args: AnnotationArgumentList; argn: string) : RttiValue

Searches an ``AnnotationArgumentList`` for an argument by name and returns its ``RttiValue``; returns ``nothing`` if the argument is not present.

:Arguments: * **args** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

            * **argn** : string

.. _function-ast_boost_find_unique_function_Module_q__string_bool:

.. das:function:: find_unique_function(mod: Module?; name: string; canfail: bool = false) : smart_ptr<Function>

Searches the compiling program for exactly one non-generic function with the given name; returns it or ``null`` if zero or multiple matches exist.

:Arguments: * **mod** :  :ref:`Module <handle-rtti-Module>` ?

            * **name** : string

            * **canfail** : bool

.. _function-ast_boost_find_unique_generic_Module_q__string_bool:

.. das:function:: find_unique_generic(mod: Module?; name: string; canfail: bool = false) : smart_ptr<Function>

Searches the compiling program for exactly one generic function with the given name; returns it or ``null`` if zero or multiple matches exist.

:Arguments: * **mod** :  :ref:`Module <handle-rtti-Module>` ?

            * **name** : string

            * **canfail** : bool

.. _function-ast_boost_find_annotation_string_string:

.. das:function:: find_annotation(mod_name: string; ann_name: string) : Annotation const?

Looks up an ``Annotation`` by name within the specified module during compilation and returns a pointer to it, or ``null`` if not found.

:Arguments: * **mod_name** : string

            * **ann_name** : string

.. _function-ast_boost_get_for_source_index_smart_ptr_ls_ExprFor_gr__VariablePtr:

.. das:function:: get_for_source_index(expr: smart_ptr<ExprFor>; svar: VariablePtr) : int

Returns the zero-based index of a given iterator variable or source expression within a ``for`` loop's source list, or ``-1`` if not found.

:Arguments: * **expr** : smart_ptr< :ref:`ExprFor <handle-ast-ExprFor>` >

            * **svar** :  :ref:`VariablePtr <alias-VariablePtr>` 

.. _function-ast_boost_get_for_source_index_smart_ptr_ls_ExprFor_gr__ExpressionPtr:

.. das:function:: get_for_source_index(expr: smart_ptr<ExprFor>; source: ExpressionPtr) : int

Returns the zero-based index of a given iterator variable or source expression within a ``for`` loop's source list, or ``-1`` if not found.

:Arguments: * **expr** : smart_ptr< :ref:`ExprFor <handle-ast-ExprFor>` >

            * **source** :  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

.. _function-ast_boost_isCMRES_FunctionPtr:

.. das:function:: isCMRES(fun: FunctionPtr) : bool

Returns ``true`` if a ``Function`` returns its result via copy-or-move-result-on-stack (CMRES) semantics rather than through a register.

:Arguments: * **fun** :  :ref:`FunctionPtr <alias-FunctionPtr>` 

.. _function-ast_boost_isCMRES_Function_q_:

.. das:function:: isCMRES(fun: Function?) : bool

Returns ``true`` if a ``Function`` returns its result via copy-or-move-result-on-stack (CMRES) semantics rather than through a register.

:Arguments: * **fun** :  :ref:`Function <handle-ast-Function>` ?

.. _function-ast_boost_isMakeLocal_ExpressionPtr:

.. das:function:: isMakeLocal(expr: ExpressionPtr) : bool

Returns ``true`` if the expression is any ``ExprMakeLocal`` subclass: ``ExprMakeStruct``, ``ExprMakeArray``, ``ExprMakeTuple``, or ``ExprMakeVariant``.

:Arguments: * **expr** :  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

.. _function-ast_boost_isExprCallFunc_ExpressionPtr:

.. das:function:: isExprCallFunc(expr: ExpressionPtr) : bool

Returns ``true`` if the expression's RTTI tag is ``ExprCallFunc``, ``ExprOp``, ``ExprNew``, or ``ExprCall`` — i.e., any function-call-like expression.

:Arguments: * **expr** :  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

.. _function-ast_boost_get_workhorse_types:

.. das:function:: get_workhorse_types() : Type[34]

Returns a fixed array of all commonly used ``Type`` values — booleans, strings, pointers, numeric scalars, enumerations, bitfields, vectors, and ranges.

.. _function-ast_boost_find_argument_index_TypeDeclPtr_string:

.. das:function:: find_argument_index(typ: TypeDeclPtr; name: string) : int

Searches the ``argNames`` of a ``TypeDeclPtr`` (tuple or variant) for the given name and returns its zero-based index, or ``-1`` if not found.

:Arguments: * **typ** :  :ref:`TypeDeclPtr <alias-TypeDeclPtr>` 

            * **name** : string

.. _function-ast_boost_isCMRESType_TypeDeclPtr:

.. das:function:: isCMRESType(blockT: TypeDeclPtr) : bool

Returns ``true`` if a ``TypeDeclPtr`` represents a reference type without an explicit ``ref`` flag, meaning it will use copy-or-move-on-stack semantics.

:Arguments: * **blockT** :  :ref:`TypeDeclPtr <alias-TypeDeclPtr>` 

.. _function-ast_boost_getVectorElementCount_Type:

.. das:function:: getVectorElementCount(bt: Type) : int

Returns the number of scalar elements in a vector ``Type`` (e.g., 2 for ``float2``/``range``, 3 for ``float3``, 4 for ``float4``), or 0 for non-vector types.

:Arguments: * **bt** :  :ref:`Type <enum-rtti-Type>` 

.. _function-ast_boost_getVectorElementSize_Type:

.. das:function:: getVectorElementSize(bt: Type) : int

Returns the byte size of a single scalar element in a vector ``Type`` — 8 for ``range64``/``urange64``, 4 for all other vector types.

:Arguments: * **bt** :  :ref:`Type <enum-rtti-Type>` 

.. _function-ast_boost_getVectorElementType_Type:

.. das:function:: getVectorElementType(bt: Type) : Type

Returns the scalar ``Type`` of each element in a vector type (e.g., ``tFloat`` for ``float2``, ``tInt`` for ``int3`` and ``range``, ``tInt64`` for ``range64``).

:Arguments: * **bt** :  :ref:`Type <enum-rtti-Type>` 

.. _function-ast_boost_getVectorOffset_Type_string:

.. das:function:: getVectorOffset(bt: Type; ident: string) : int

Returns the zero-based element index for a named swizzle component (``x``/``y``/``z``/``w`` or ``r``/``g``/``b``/``a``) within a vector ``Type``, or ``-1`` if out of bounds.

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

Creates an ``AnnotationDeclaration`` for the named annotation (with optional typed arguments) and attaches it to a ``Function``, ``ExprBlock``, or ``Structure``.

:Arguments: * **mod_name** : string

            * **ann_name** : string

            * **args** : array<tuple<argname:string;argvalue: :ref:`RttiValue <alias-RttiValue>` >>

.. _function-ast_boost_append_annotation_string_string:

.. das:function:: append_annotation(mod_name: string; ann_name: string) : smart_ptr<AnnotationDeclaration>

Creates an ``AnnotationDeclaration`` for the named annotation (with optional typed arguments) and attaches it to a ``Function``, ``ExprBlock``, or ``Structure``.

:Arguments: * **mod_name** : string

            * **ann_name** : string

.. _function-ast_boost_append_annotation_FunctionPtr_string_string:

.. das:function:: append_annotation(func: FunctionPtr; mod_name: string; ann_name: string)

Creates an ``AnnotationDeclaration`` for the named annotation (with optional typed arguments) and attaches it to a ``Function``, ``ExprBlock``, or ``Structure``.

:Arguments: * **func** :  :ref:`FunctionPtr <alias-FunctionPtr>` 

            * **mod_name** : string

            * **ann_name** : string

.. _function-ast_boost_append_annotation_smart_ptr_ls_ExprBlock_gr__string_string:

.. das:function:: append_annotation(blk: smart_ptr<ExprBlock>; mod_name: string; ann_name: string)

Creates an ``AnnotationDeclaration`` for the named annotation (with optional typed arguments) and attaches it to a ``Function``, ``ExprBlock``, or ``Structure``.

:Arguments: * **blk** : smart_ptr< :ref:`ExprBlock <handle-ast-ExprBlock>` >

            * **mod_name** : string

            * **ann_name** : string

.. _function-ast_boost_append_annotation_smart_ptr_ls_Structure_gr__string_string:

.. das:function:: append_annotation(st: smart_ptr<Structure>; mod_name: string; ann_name: string)

Creates an ``AnnotationDeclaration`` for the named annotation (with optional typed arguments) and attaches it to a ``Function``, ``ExprBlock``, or ``Structure``.

:Arguments: * **st** : smart_ptr< :ref:`Structure <handle-ast-Structure>` >

            * **mod_name** : string

            * **ann_name** : string

.. _function-ast_boost_append_annotation_FunctionPtr_string_string_array_ls_tuple_ls_argname_c_string;argvalue_c_variant_ls_tBool_c_bool;tInt_c_int;tUInt_c_uint;tInt64_c_int64;tUInt64_c_uint64;tFloat_c_float;tDouble_c_double;tString_c_string;nothing_c_any_gr__gr__gr_:

.. das:function:: append_annotation(func: FunctionPtr; mod_name: string; ann_name: string; args: array<tuple<argname:string;argvalue:variant<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any>>>)

Creates an ``AnnotationDeclaration`` for the named annotation (with optional typed arguments) and attaches it to a ``Function``, ``ExprBlock``, or ``Structure``.

:Arguments: * **func** :  :ref:`FunctionPtr <alias-FunctionPtr>` 

            * **mod_name** : string

            * **ann_name** : string

            * **args** : array<tuple<argname:string;argvalue: :ref:`RttiValue <alias-RttiValue>` >>

.. _function-ast_boost_append_annotation_smart_ptr_ls_ExprBlock_gr__string_string_array_ls_tuple_ls_argname_c_string;argvalue_c_variant_ls_tBool_c_bool;tInt_c_int;tUInt_c_uint;tInt64_c_int64;tUInt64_c_uint64;tFloat_c_float;tDouble_c_double;tString_c_string;nothing_c_any_gr__gr__gr_:

.. das:function:: append_annotation(blk: smart_ptr<ExprBlock>; mod_name: string; ann_name: string; args: array<tuple<argname:string;argvalue:variant<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any>>>)

Creates an ``AnnotationDeclaration`` for the named annotation (with optional typed arguments) and attaches it to a ``Function``, ``ExprBlock``, or ``Structure``.

:Arguments: * **blk** : smart_ptr< :ref:`ExprBlock <handle-ast-ExprBlock>` >

            * **mod_name** : string

            * **ann_name** : string

            * **args** : array<tuple<argname:string;argvalue: :ref:`RttiValue <alias-RttiValue>` >>

.. _function-ast_boost_append_annotation_smart_ptr_ls_Structure_gr__string_string_array_ls_tuple_ls_argname_c_string;argvalue_c_variant_ls_tBool_c_bool;tInt_c_int;tUInt_c_uint;tInt64_c_int64;tUInt64_c_uint64;tFloat_c_float;tDouble_c_double;tString_c_string;nothing_c_any_gr__gr__gr_:

.. das:function:: append_annotation(st: smart_ptr<Structure>; mod_name: string; ann_name: string; args: array<tuple<argname:string;argvalue:variant<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any>>>)

Creates an ``AnnotationDeclaration`` for the named annotation (with optional typed arguments) and attaches it to a ``Function``, ``ExprBlock``, or ``Structure``.

:Arguments: * **st** : smart_ptr< :ref:`Structure <handle-ast-Structure>` >

            * **mod_name** : string

            * **ann_name** : string

            * **args** : array<tuple<argname:string;argvalue: :ref:`RttiValue <alias-RttiValue>` >>

.. _function-ast_boost_add_annotation_argument_AnnotationArgumentList_string_bool:

.. das:function:: add_annotation_argument(arguments: AnnotationArgumentList; argName: string; val: bool) : int

Adds a typed annotation argument (``bool``, ``int``, ``float``, ``string``, or ``AnnotationArgument``) to an ``AnnotationArgumentList`` and returns the new argument index.

:Arguments: * **arguments** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

            * **argName** : string

            * **val** : bool

.. _function-ast_boost_add_annotation_argument_AnnotationArgumentList_string_int:

.. das:function:: add_annotation_argument(arguments: AnnotationArgumentList; argName: string; val: int) : int

Adds a typed annotation argument (``bool``, ``int``, ``float``, ``string``, or ``AnnotationArgument``) to an ``AnnotationArgumentList`` and returns the new argument index.

:Arguments: * **arguments** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

            * **argName** : string

            * **val** : int

.. _function-ast_boost_add_annotation_argument_AnnotationArgumentList_string_float:

.. das:function:: add_annotation_argument(arguments: AnnotationArgumentList; argName: string; val: float) : int

Adds a typed annotation argument (``bool``, ``int``, ``float``, ``string``, or ``AnnotationArgument``) to an ``AnnotationArgumentList`` and returns the new argument index.

:Arguments: * **arguments** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

            * **argName** : string

            * **val** : float

.. _function-ast_boost_add_annotation_argument_AnnotationArgumentList_string_string:

.. das:function:: add_annotation_argument(arguments: AnnotationArgumentList; argName: string; val: string) : int

Adds a typed annotation argument (``bool``, ``int``, ``float``, ``string``, or ``AnnotationArgument``) to an ``AnnotationArgumentList`` and returns the new argument index.

:Arguments: * **arguments** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

            * **argName** : string

            * **val** : string

.. _function-ast_boost_add_annotation_argument_AnnotationArgumentList_AnnotationArgument:

.. das:function:: add_annotation_argument(arguments: AnnotationArgumentList; ann: AnnotationArgument) : int

Adds a typed annotation argument (``bool``, ``int``, ``float``, ``string``, or ``AnnotationArgument``) to an ``AnnotationArgumentList`` and returns the new argument index.

:Arguments: * **arguments** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

            * **ann** :  :ref:`AnnotationArgument <handle-rtti-AnnotationArgument>` 

+++++++++++++++++++++
Expression generation
+++++++++++++++++++++

  *  :ref:`override_method (var str: StructurePtr; name: string; funcName: string) : bool <function-ast_boost_override_method_StructurePtr_string_string>` 
  *  :ref:`panic_expr_as () : void? <function-ast_boost_panic_expr_as>` 
  *  :ref:`make_static_assert_false (text: string; at: LineInfo) : smart_ptr\<ExprStaticAssert\> <function-ast_boost_make_static_assert_false_string_LineInfo>` 
  *  :ref:`convert_to_expression (var value: auto& ==const; at: LineInfo) : auto <function-ast_boost_convert_to_expression__auto__eq__eq_const_LineInfo_0x393>` 
  *  :ref:`convert_to_expression (value: auto ==const; at: LineInfo) : auto <function-ast_boost_convert_to_expression_auto__eq__eq_const_LineInfo_0x39c>` 
  *  :ref:`convert_to_expression (var value: auto ==const) : auto <function-ast_boost_convert_to_expression__auto__eq__eq_const_0x3a5>` 
  *  :ref:`convert_to_expression (value: auto ==const) : auto <function-ast_boost_convert_to_expression_auto__eq__eq_const_0x3aa>` 

.. _function-ast_boost_override_method_StructurePtr_string_string:

.. das:function:: override_method(str: StructurePtr; name: string; funcName: string) : bool

Replaces the initializer of a field named ``name`` in the given structure with an ``@@funcName`` function address cast, effectively overriding that class method.

:Arguments: * **str** :  :ref:`StructurePtr <alias-StructurePtr>` 

            * **name** : string

            * **funcName** : string

.. _function-ast_boost_panic_expr_as:

.. das:function:: panic_expr_as() : void?

Helper function that panics with ``"invalid 'as' expression or null pointer dereference"`` and returns ``null`` — used as the failure branch in ``as`` variant casts.

.. _function-ast_boost_make_static_assert_false_string_LineInfo:

.. das:function:: make_static_assert_false(text: string; at: LineInfo) : smart_ptr<ExprStaticAssert>

Creates an ``ExprStaticAssert`` expression node that always fails at compile time with the given error message text.

:Arguments: * **text** : string

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

.. _function-ast_boost_convert_to_expression__auto__eq__eq_const_LineInfo_0x393:

.. das:function:: convert_to_expression(value: auto& ==const; at: LineInfo) : auto

Converts a runtime value of any supported type to an equivalent AST ``ExpressionPtr`` that would produce that value when compiled, using ``typeinfo`` for reflection.

:Arguments: * **value** : auto&!

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

.. _function-ast_boost_convert_to_expression_auto__eq__eq_const_LineInfo_0x39c:

.. das:function:: convert_to_expression(value: auto ==const; at: LineInfo) : auto

Converts a runtime value of any supported type to an equivalent AST ``ExpressionPtr`` that would produce that value when compiled, using ``typeinfo`` for reflection.

:Arguments: * **value** : auto!

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

.. _function-ast_boost_convert_to_expression__auto__eq__eq_const_0x3a5:

.. das:function:: convert_to_expression(value: auto ==const) : auto

Converts a runtime value of any supported type to an equivalent AST ``ExpressionPtr`` that would produce that value when compiled, using ``typeinfo`` for reflection.

:Arguments: * **value** : auto!

.. _function-ast_boost_convert_to_expression_auto__eq__eq_const_0x3aa:

.. das:function:: convert_to_expression(value: auto ==const) : auto

Converts a runtime value of any supported type to an equivalent AST ``ExpressionPtr`` that would produce that value when compiled, using ``typeinfo`` for reflection.

:Arguments: * **value** : auto!

++++++++
Visitors
++++++++

  *  :ref:`visit_finally (blk: ExprBlock?; adapter: smart_ptr\<VisitorAdapter\>) <function-ast_boost_visit_finally_ExprBlock_q__smart_ptr_ls_VisitorAdapter_gr_>` 

.. _function-ast_boost_visit_finally_ExprBlock_q__smart_ptr_ls_VisitorAdapter_gr_:

.. das:function:: visit_finally(blk: ExprBlock?; adapter: smart_ptr<VisitorAdapter>)

Invokes the given ``VisitorAdapter`` on the ``finally`` section of an ``ExprBlock``, allowing macro visitors to inspect or transform finally-block code.

:Arguments: * **blk** :  :ref:`ExprBlock <handle-ast-ExprBlock>` ?

            * **adapter** : smart_ptr< :ref:`VisitorAdapter <handle-ast-VisitorAdapter>` >

+++++++++++++++
Type generation
+++++++++++++++

  *  :ref:`function_to_type (fn: FunctionPtr) : TypeDeclPtr <function-ast_boost_function_to_type_FunctionPtr>` 

.. _function-ast_boost_function_to_type_FunctionPtr:

.. das:function:: function_to_type(fn: FunctionPtr) : TypeDeclPtr

Constructs a ``TypeDeclPtr`` of ``tFunction`` base type from a ``FunctionPtr``, capturing its argument types and names plus the return type.

:Arguments: * **fn** :  :ref:`FunctionPtr <alias-FunctionPtr>` 

++++++++++
Type casts
++++++++++

  *  :ref:`Function?`is`BuiltInFunction (foo: Function?) : bool <function-ast_boost__rq_is_rq_BuiltInFunction_Function_q_>` 
  *  :ref:`Function?`as`BuiltInFunction (foo: Function?) : BuiltInFunction? <function-ast_boost__rq_as_rq_BuiltInFunction_Function_q_>` 
  *  :ref:`Function?`is`ExternalFnBase (foo: Function?) : bool <function-ast_boost__rq_is_rq_ExternalFnBase_Function_q_>` 
  *  :ref:`Function?`as`ExternalFnBase (foo: Function?) : ExternalFnBase? <function-ast_boost__rq_as_rq_ExternalFnBase_Function_q_>` 
  *  :ref:`Annotation?`is`FunctionAnnotation (foo: Annotation?) : bool <function-ast_boost__rq_is_rq_FunctionAnnotation_Annotation_q_>` 
  *  :ref:`smart_ptr\<Annotation\>`is`FunctionAnnotation (foo: smart_ptr\<Annotation\>) : bool <function-ast_boost__rq_is_rq_FunctionAnnotation_smart_ptr_ls_Annotation_gr_>` 
  *  :ref:`Annotation?`as`FunctionAnnotation (foo: Annotation?) : FunctionAnnotation? <function-ast_boost__rq_as_rq_FunctionAnnotation_Annotation_q_>` 
  *  :ref:`smart_ptr\<Annotation\>`as`FunctionAnnotation (foo: smart_ptr\<Annotation\>) : FunctionAnnotation? <function-ast_boost__rq_as_rq_FunctionAnnotation_smart_ptr_ls_Annotation_gr_>` 
  *  :ref:`Annotation?`is`StructureAnnotation (foo: Annotation?) : bool <function-ast_boost__rq_is_rq_StructureAnnotation_Annotation_q_>` 
  *  :ref:`smart_ptr\<Annotation\>`is`StructureAnnotation (foo: smart_ptr\<Annotation\>) : bool <function-ast_boost__rq_is_rq_StructureAnnotation_smart_ptr_ls_Annotation_gr_>` 
  *  :ref:`Annotation?`as`StructureAnnotation (foo: Annotation?) : StructureAnnotation? <function-ast_boost__rq_as_rq_StructureAnnotation_Annotation_q_>` 
  *  :ref:`smart_ptr\<Annotation\>`as`StructureAnnotation (foo: smart_ptr\<Annotation\>) : StructureAnnotation? <function-ast_boost__rq_as_rq_StructureAnnotation_smart_ptr_ls_Annotation_gr_>` 
  *  :ref:`walk_and_convert (data: uint8 const?; info: TypeDeclPtr; at: LineInfo) : ExpressionPtr <function-ast_boost_walk_and_convert_uint8_const_q__TypeDeclPtr_LineInfo>` 
  *  :ref:`auto`is`BuiltInFunction (anything: auto) : auto <function-ast_boost__rq_is_rq_BuiltInFunction_auto_0x26a>` 
  *  :ref:`auto`is`ExternalFnBase (anything: auto) : auto <function-ast_boost__rq_is_rq_ExternalFnBase_auto_0x27a>` 
  *  :ref:`auto`is`FunctionAnnotation (anything: auto) : auto <function-ast_boost__rq_is_rq_FunctionAnnotation_auto_0x28a>` 
  *  :ref:`auto`is`StructureAnnotation (anything: auto) : auto <function-ast_boost__rq_is_rq_StructureAnnotation_auto_0x2a6>` 

.. _function-ast_boost__rq_is_rq_BuiltInFunction_Function_q_:

.. das:function:: Function?`is`BuiltInFunction(foo: Function?) : bool

Returns ``true`` if the given ``Function?`` has the ``builtIn`` flag set, indicating it is a ``BuiltInFunction``; returns ``false`` for any other type.

:Arguments: * **foo** :  :ref:`Function <handle-ast-Function>` ?

.. _function-ast_boost__rq_as_rq_BuiltInFunction_Function_q_:

.. das:function:: Function?`as`BuiltInFunction(foo: Function?) : BuiltInFunction?

Casts a ``Function?`` to ``BuiltInFunction?`` via ``reinterpret``, verifying the target is a built-in function first (panics otherwise).

:Arguments: * **foo** :  :ref:`Function <handle-ast-Function>` ?

.. _function-ast_boost__rq_is_rq_ExternalFnBase_Function_q_:

.. das:function:: Function?`is`ExternalFnBase(foo: Function?) : bool

Returns ``true`` if the given ``Function?`` is both ``builtIn`` and has the ``propertyFunction`` flag, indicating it is an ``ExternalFnBase``; returns ``false`` otherwise.

:Arguments: * **foo** :  :ref:`Function <handle-ast-Function>` ?

.. _function-ast_boost__rq_as_rq_ExternalFnBase_Function_q_:

.. das:function:: Function?`as`ExternalFnBase(foo: Function?) : ExternalFnBase?

Casts a ``Function?`` to ``ExternalFnBase?`` via ``reinterpret``, verifying it is a property-flagged built-in function first (panics otherwise).

:Arguments: * **foo** :  :ref:`Function <handle-ast-Function>` ?

.. _function-ast_boost__rq_is_rq_FunctionAnnotation_Annotation_q_:

.. das:function:: Annotation?`is`FunctionAnnotation(foo: Annotation?) : bool

Returns ``true`` if the given ``Annotation?`` or ``smart_ptr<Annotation>`` is a ``FunctionAnnotation`` according to its ``isFunctionAnnotation`` property.

:Arguments: * **foo** :  :ref:`Annotation <handle-rtti-Annotation>` ?

.. _function-ast_boost__rq_is_rq_FunctionAnnotation_smart_ptr_ls_Annotation_gr_:

.. das:function:: smart_ptr<Annotation>`is`FunctionAnnotation(foo: smart_ptr<Annotation>) : bool

Returns ``true`` if the given ``Annotation?`` or ``smart_ptr<Annotation>`` is a ``FunctionAnnotation`` according to its ``isFunctionAnnotation`` property.

:Arguments: * **foo** : smart_ptr< :ref:`Annotation <handle-rtti-Annotation>` >

.. _function-ast_boost__rq_as_rq_FunctionAnnotation_Annotation_q_:

.. das:function:: Annotation?`as`FunctionAnnotation(foo: Annotation?) : FunctionAnnotation?

Casts an ``Annotation?`` or ``smart_ptr<Annotation>`` to ``FunctionAnnotation?`` via ``reinterpret``, verifying the annotation kind first (panics otherwise).

:Arguments: * **foo** :  :ref:`Annotation <handle-rtti-Annotation>` ?

.. _function-ast_boost__rq_as_rq_FunctionAnnotation_smart_ptr_ls_Annotation_gr_:

.. das:function:: smart_ptr<Annotation>`as`FunctionAnnotation(foo: smart_ptr<Annotation>) : FunctionAnnotation?

Casts an ``Annotation?`` or ``smart_ptr<Annotation>`` to ``FunctionAnnotation?`` via ``reinterpret``, verifying the annotation kind first (panics otherwise).

:Arguments: * **foo** : smart_ptr< :ref:`Annotation <handle-rtti-Annotation>` >

.. _function-ast_boost__rq_is_rq_StructureAnnotation_Annotation_q_:

.. das:function:: Annotation?`is`StructureAnnotation(foo: Annotation?) : bool

Returns ``true`` if the given ``Annotation?`` or ``smart_ptr<Annotation>`` is a ``StructureAnnotation`` according to its ``isStructureAnnotation`` property.

:Arguments: * **foo** :  :ref:`Annotation <handle-rtti-Annotation>` ?

.. _function-ast_boost__rq_is_rq_StructureAnnotation_smart_ptr_ls_Annotation_gr_:

.. das:function:: smart_ptr<Annotation>`is`StructureAnnotation(foo: smart_ptr<Annotation>) : bool

Returns ``true`` if the given ``Annotation?`` or ``smart_ptr<Annotation>`` is a ``StructureAnnotation`` according to its ``isStructureAnnotation`` property.

:Arguments: * **foo** : smart_ptr< :ref:`Annotation <handle-rtti-Annotation>` >

.. _function-ast_boost__rq_as_rq_StructureAnnotation_Annotation_q_:

.. das:function:: Annotation?`as`StructureAnnotation(foo: Annotation?) : StructureAnnotation?

Casts an ``Annotation?`` or ``smart_ptr<Annotation>`` to ``StructureAnnotation?`` via ``reinterpret``, verifying the annotation kind first (panics otherwise).

:Arguments: * **foo** :  :ref:`Annotation <handle-rtti-Annotation>` ?

.. _function-ast_boost__rq_as_rq_StructureAnnotation_smart_ptr_ls_Annotation_gr_:

.. das:function:: smart_ptr<Annotation>`as`StructureAnnotation(foo: smart_ptr<Annotation>) : StructureAnnotation?

Casts an ``Annotation?`` or ``smart_ptr<Annotation>`` to ``StructureAnnotation?`` via ``reinterpret``, verifying the annotation kind first (panics otherwise).

:Arguments: * **foo** : smart_ptr< :ref:`Annotation <handle-rtti-Annotation>` >

.. _function-ast_boost_walk_and_convert_uint8_const_q__TypeDeclPtr_LineInfo:

.. das:function:: walk_and_convert(data: uint8 const?; info: TypeDeclPtr; at: LineInfo) : ExpressionPtr

Recursively walks raw data bytes using a ``TypeDeclPtr`` schema and builds an equivalent AST expression tree that would reproduce that data when compiled.

:Arguments: * **data** : uint8?

            * **info** :  :ref:`TypeDeclPtr <alias-TypeDeclPtr>` 

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

.. _function-ast_boost__rq_is_rq_BuiltInFunction_auto_0x26a:

.. das:function:: auto`is`BuiltInFunction(anything: auto) : auto

Returns ``true`` if the given ``Function?`` has the ``builtIn`` flag set, indicating it is a ``BuiltInFunction``; returns ``false`` for any other type.

:Arguments: * **anything** : auto

.. _function-ast_boost__rq_is_rq_ExternalFnBase_auto_0x27a:

.. das:function:: auto`is`ExternalFnBase(anything: auto) : auto

Returns ``true`` if the given ``Function?`` is both ``builtIn`` and has the ``propertyFunction`` flag, indicating it is an ``ExternalFnBase``; returns ``false`` otherwise.

:Arguments: * **anything** : auto

.. _function-ast_boost__rq_is_rq_FunctionAnnotation_auto_0x28a:

.. das:function:: auto`is`FunctionAnnotation(anything: auto) : auto

Returns ``true`` if the given ``Annotation?`` or ``smart_ptr<Annotation>`` is a ``FunctionAnnotation`` according to its ``isFunctionAnnotation`` property.

:Arguments: * **anything** : auto

.. _function-ast_boost__rq_is_rq_StructureAnnotation_auto_0x2a6:

.. das:function:: auto`is`StructureAnnotation(anything: auto) : auto

Returns ``true`` if the given ``Annotation?`` or ``smart_ptr<Annotation>`` is a ``StructureAnnotation`` according to its ``isStructureAnnotation`` property.

:Arguments: * **anything** : auto

+++++
Setup
+++++

  *  :ref:`setup_call_list (name: string; at: LineInfo; subblock: block\<(var fn:FunctionPtr):void\>) : ExprBlock? <function-ast_boost_setup_call_list_string_LineInfo_block_ls_var_fn_c_FunctionPtr_c_void_gr_>` 
  *  :ref:`setup_call_list (name: string; at: LineInfo; isInit: bool = false; isPrivate: bool = true; isLateInit: bool = false) : ExprBlock? <function-ast_boost_setup_call_list_string_LineInfo_bool_bool_bool>` 
  *  :ref:`setup_macro (name: string; at: LineInfo) : ExprBlock? <function-ast_boost_setup_macro_string_LineInfo>` 
  *  :ref:`setup_tag_annotation (name: string; tag: string; var classPtr: auto) : auto <function-ast_boost_setup_tag_annotation_string_string_auto_0x111>` 

.. _function-ast_boost_setup_call_list_string_LineInfo_block_ls_var_fn_c_FunctionPtr_c_void_gr_:

.. das:function:: setup_call_list(name: string; at: LineInfo; subblock: block<(var fn:FunctionPtr):void>) : ExprBlock?

Creates or locates a compilation-phase setup function (``__setup_macros``) and returns its body ``ExprBlock`` so callers can append registration calls to it.

:Arguments: * **name** : string

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

            * **subblock** : block<(fn: :ref:`FunctionPtr <alias-FunctionPtr>` ):void>

.. _function-ast_boost_setup_call_list_string_LineInfo_bool_bool_bool:

.. das:function:: setup_call_list(name: string; at: LineInfo; isInit: bool = false; isPrivate: bool = true; isLateInit: bool = false) : ExprBlock?

Creates or locates a compilation-phase setup function (``__setup_macros``) and returns its body ``ExprBlock`` so callers can append registration calls to it.

:Arguments: * **name** : string

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

            * **isInit** : bool

            * **isPrivate** : bool

            * **isLateInit** : bool

.. _function-ast_boost_setup_macro_string_LineInfo:

.. das:function:: setup_macro(name: string; at: LineInfo) : ExprBlock?

Creates or locates a macro initialization function (``__setup_macros``) guarded by ``is_compiling_macros`` and returns its body block for appending macro registration code.

:Arguments: * **name** : string

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

.. _function-ast_boost_setup_tag_annotation_string_string_auto_0x111:

.. das:function:: setup_tag_annotation(name: string; tag: string; classPtr: auto) : auto

Creates an ``AstFunctionAnnotation`` instance and automatically applies it to every function that carries a matching ``[tag_function(tag)]`` annotation in the module.

:Arguments: * **name** : string

            * **tag** : string

            * **classPtr** : auto


