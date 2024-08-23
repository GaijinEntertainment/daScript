
.. _stdlib_ast:

========================
AST manipulation library
========================

The AST module implements compilation time reflection for the Daslang syntax tree.

All functions and symbols are in "ast" module, use require to get access to it. ::

    require ast


++++++++++++
Type aliases
++++++++++++

.. _alias-TypeDeclFlags:

.. das:attribute:: TypeDeclFlags is a bitfield

properties of the `TypeDecl` object.

:Fields: * **ref** (0) - 1

         * **constant** (1) - 2

         * **temporary** (2) - 3

         * **_implicit** (3) - 4

         * **removeRef** (4) - 5

         * **removeConstant** (5) - 6

         * **removeDim** (6) - 7

         * **removeTemporary** (7) - 8

         * **explicitConst** (8) - 9

         * **aotAlias** (9) - 10

         * **smartPtr** (10) - 11

         * **smartPtrNative** (11) - 12

         * **isExplicit** (12) - 13

         * **isNativeDim** (13) - 14

         * **isTag** (14) - 15

         * **explicitRef** (15) - 16

         * **isPrivateAlias** (16) - 17

         * **autoToAlias** (17) - 18


.. _alias-FieldDeclarationFlags:

.. das:attribute:: FieldDeclarationFlags is a bitfield

:Fields: * **moveSemantics** (0)

         * **parentType** (1)

         * **capturedConstant** (2)

         * **generated** (3)

         * **capturedRef** (4)

         * **doNotDelete** (5)

         * **privateField** (6)

         * **_sealed** (7)

         * **implemented** (8)

// stub


.. _alias-StructureFlags:

.. das:attribute:: StructureFlags is a bitfield

:Fields: * **isClass** (0)

         * **genCtor** (1)

         * **cppLayout** (2)

         * **cppLayoutNotPod** (3)

         * **generated** (4)

         * **persistent** (5)

         * **isLambda** (6)

         * **privateStructure** (7)

         * **macroInterface** (8)

         * **_sealed** (9)

         * **skipLockCheck** (10)

         * **circular** (11)

         * **_generator** (12)

         * **hasStaticMembers** (13)

         * **hasStaticFunctions** (14)

         * **hasInitFields** (15)

         * **safeWhenUninitialized** (16)

// stub


.. _alias-ExprGenFlags:

.. das:attribute:: ExprGenFlags is a bitfield

:Fields: * **alwaysSafe** (0)

         * **generated** (1)

         * **userSaidItsSafe** (2)

// stub


.. _alias-ExprLetFlags:

.. das:attribute:: ExprLetFlags is a bitfield

:Fields: * **inScope** (0)

         * **hasEarlyOut** (1)

         * **itTupleExpansion** (2)

// stub


.. _alias-ExprFlags:

.. das:attribute:: ExprFlags is a bitfield

:Fields: * **constexpression** (0)

         * **noSideEffects** (1)

         * **noNativeSideEffects** (2)

         * **isForLoopSource** (3)

         * **isCallArgument** (4)

// stub


.. _alias-ExprPrintFlags:

.. das:attribute:: ExprPrintFlags is a bitfield

:Fields: * **topLevel** (0)

         * **argLevel** (1)

         * **bottomLevel** (2)

// stub


.. _alias-FunctionFlags:

.. das:attribute:: FunctionFlags is a bitfield

:Fields: * **builtIn** (0)

         * **policyBased** (1)

         * **callBased** (2)

         * **interopFn** (3)

         * **hasReturn** (4)

         * **copyOnReturn** (5)

         * **moveOnReturn** (6)

         * **exports** (7)

         * **init** (8)

         * **addr** (9)

         * **used** (10)

         * **fastCall** (11)

         * **knownSideEffects** (12)

         * **hasToRunAtCompileTime** (13)

         * **unsafeOperation** (14)

         * **unsafeDeref** (15)

         * **hasMakeBlock** (16)

         * **aotNeedPrologue** (17)

         * **noAot** (18)

         * **aotHybrid** (19)

         * **aotTemplate** (20)

         * **generated** (21)

         * **privateFunction** (22)

         * **_generator** (23)

         * **_lambda** (24)

         * **firstArgReturnType** (25)

         * **noPointerCast** (26)

         * **isClassMethod** (27)

         * **isTypeConstructor** (28)

         * **shutdown** (29)

         * **anyTemplate** (30)

         * **macroInit** (31)

// stub


.. _alias-MoreFunctionFlags:

.. das:attribute:: MoreFunctionFlags is a bitfield

:Fields: * **macroFunction** (0)

         * **needStringCast** (1)

         * **aotHashDeppendsOnArguments** (2)

         * **lateInit** (3)

         * **requestJit** (4)

         * **unsafeOutsideOfFor** (5)

         * **skipLockCheck** (6)

         * **safeImplicit** (7)

         * **deprecated** (8)

         * **aliasCMRES** (9)

         * **neverAliasCMRES** (10)

         * **addressTaken** (11)

         * **propertyFunction** (12)

         * **pinvoke** (13)

         * **jitOnly** (14)

         * **isStaticClassMethod** (15)

         * **requestNoJit** (16)

         * **jitContextAndLineInfo** (17)

         * **nodiscard** (18)

         * **captureString** (19)

         * **callCaptureString** (20)

         * **hasStringBuilder** (21)

// stub


.. _alias-FunctionSideEffectFlags:

.. das:attribute:: FunctionSideEffectFlags is a bitfield

:Fields: * **_unsafe** (0)

         * **userScenario** (1)

         * **modifyExternal** (2)

         * **modifyArgument** (3)

         * **accessGlobal** (4)

         * **invoke** (5)

// stub


.. _alias-VariableFlags:

.. das:attribute:: VariableFlags is a bitfield

:Fields: * **init_via_move** (0)

         * **init_via_clone** (1)

         * **used** (2)

         * **aliasCMRES** (3)

         * **marked_used** (4)

         * **global_shared** (5)

         * **do_not_delete** (6)

         * **generated** (7)

         * **capture_as_ref** (8)

         * **can_shadow** (9)

         * **private_variable** (10)

         * **tag** (11)

         * **global** (12)

         * **inScope** (13)

         * **no_capture** (14)

         * **early_out** (15)

         * **used_in_finally** (16)

         * **static_class_member** (17)

// stub


.. _alias-VariableAccessFlags:

.. das:attribute:: VariableAccessFlags is a bitfield

:Fields: * **access_extern** (0)

         * **access_get** (1)

         * **access_ref** (2)

         * **access_init** (3)

         * **access_pass** (4)

// stub


.. _alias-ExprBlockFlags:

.. das:attribute:: ExprBlockFlags is a bitfield

:Fields: * **isClosure** (0)

         * **hasReturn** (1)

         * **copyOnReturn** (2)

         * **moveOnReturn** (3)

         * **inTheLoop** (4)

         * **finallyBeforeBody** (5)

         * **finallyDisabled** (6)

         * **aotSkipMakeBlock** (7)

         * **aotDoNotSkipAnnotationData** (8)

         * **isCollapseable** (9)

         * **needCollapse** (10)

         * **hasMakeBlock** (11)

         * **hasEarlyOut** (12)

         * **forLoop** (13)

// stub


.. _alias-ExprAtFlags:

.. das:attribute:: ExprAtFlags is a bitfield

:Fields: * **r2v** (0)

         * **r2cr** (1)

         * **write** (2)

         * **no_promotion** (3)

// stub


.. _alias-ExprMakeLocalFlags:

.. das:attribute:: ExprMakeLocalFlags is a bitfield

:Fields: * **useStackRef** (0)

         * **useCMRES** (1)

         * **doesNotNeedSp** (2)

         * **doesNotNeedInit** (3)

         * **initAllFields** (4)

         * **alwaysAlias** (5)

// stub


.. _alias-ExprAscendFlags:

.. das:attribute:: ExprAscendFlags is a bitfield

:Fields: * **useStackRef** (0)

         * **needTypeInfo** (1)

         * **isMakeLambda** (2)

// stub


.. _alias-ExprCastFlags:

.. das:attribute:: ExprCastFlags is a bitfield

:Fields: * **upcastCast** (0)

         * **reinterpretCast** (1)

// stub


.. _alias-ExprVarFlags:

.. das:attribute:: ExprVarFlags is a bitfield

:Fields: * **local** (0)

         * **argument** (1)

         * **_block** (2)

         * **thisBlock** (3)

         * **r2v** (4)

         * **r2cr** (5)

         * **write** (6)

         * **under_clone** (7)

// stub


.. _alias-ExprMakeStructFlags:

.. das:attribute:: ExprMakeStructFlags is a bitfield

:Fields: * **useInitializer** (0)

         * **isNewHandle** (1)

         * **usedInitializer** (2)

         * **nativeClassInitializer** (3)

         * **isNewClass** (4)

         * **forceClass** (5)

         * **forceStruct** (6)

         * **forceVariant** (7)

         * **forceTuple** (8)

         * **alwaysUseInitializer** (9)

// stub


.. _alias-MakeFieldDeclFlags:

.. das:attribute:: MakeFieldDeclFlags is a bitfield

:Fields: * **moveSemantics** (0)

         * **cloneSemantics** (1)

// stub


.. _alias-ExprFieldDerefFlags:

.. das:attribute:: ExprFieldDerefFlags is a bitfield

:Fields: * **unsafeDeref** (0)

         * **ignoreCaptureConst** (1)

// stub


.. _alias-ExprFieldFieldFlags:

.. das:attribute:: ExprFieldFieldFlags is a bitfield

:Fields: * **r2v** (0)

         * **r2cr** (1)

         * **write** (2)

         * **no_promotion** (3)

         * **under_clone** (4)

// stub


.. _alias-ExprSwizzleFieldFlags:

.. das:attribute:: ExprSwizzleFieldFlags is a bitfield

:Fields: * **r2v** (0)

         * **r2cr** (1)

         * **write** (2)

// stub


.. _alias-ExprYieldFlags:

.. das:attribute:: ExprYieldFlags is a bitfield

:Fields: * **moveSemantics** (0)

         * **skipLockCheck** (1)

// stub


.. _alias-ExprReturnFlags:

.. das:attribute:: ExprReturnFlags is a bitfield

:Fields: * **moveSemantics** (0)

         * **returnReference** (1)

         * **returnInBlock** (2)

         * **takeOverRightStack** (3)

         * **returnCallCMRES** (4)

         * **returnCMRES** (5)

         * **fromYield** (6)

         * **fromComprehension** (7)

         * **skipLockCheck** (8)

// stub


.. _alias-ExprMakeBlockFlags:

.. das:attribute:: ExprMakeBlockFlags is a bitfield

:Fields: * **isLambda** (0)

         * **isLocalFunction** (1)

// stub


.. _alias-CopyFlags:

.. das:attribute:: CopyFlags is a bitfield

:Fields: * **allowCopyTemp** (0)

         * **takeOverRightStack** (1)

         * **promoteToClone** (2)

// stub


.. _alias-MoveFlags:

.. das:attribute:: MoveFlags is a bitfield

:Fields: * **skipLockCheck** (0)

         * **takeOverRightStack** (1)

// stub


.. _alias-IfFlags:

.. das:attribute:: IfFlags is a bitfield

:Fields: * **isStatic** (0)

         * **doNotFold** (1)

// stub


.. _alias-ExpressionPtr:

.. das:attribute:: ExpressionPtr = smart_ptr<Expression>

// stub


.. _alias-ProgramPtr:

.. das:attribute:: ProgramPtr = smart_ptr<Program>

// stub


.. _alias-TypeDeclPtr:

.. das:attribute:: TypeDeclPtr = smart_ptr<TypeDecl>

// stub


.. _alias-VectorTypeDeclPtr:

.. das:attribute:: VectorTypeDeclPtr = dasvector`smart_ptr`TypeDecl

// stub


.. _alias-EnumerationPtr:

.. das:attribute:: EnumerationPtr = smart_ptr<Enumeration>

// stub


.. _alias-StructurePtr:

.. das:attribute:: StructurePtr = smart_ptr<Structure>

// stub


.. _alias-FunctionPtr:

.. das:attribute:: FunctionPtr = smart_ptr<Function>

// stub


.. _alias-VariablePtr:

.. das:attribute:: VariablePtr = smart_ptr<Variable>

// stub


.. _alias-MakeFieldDeclPtr:

.. das:attribute:: MakeFieldDeclPtr = smart_ptr<MakeFieldDecl>

// stub


.. _alias-FunctionAnnotationPtr:

.. das:attribute:: FunctionAnnotationPtr = smart_ptr<FunctionAnnotation>

// stub


.. _alias-StructureAnnotationPtr:

.. das:attribute:: StructureAnnotationPtr = smart_ptr<StructureAnnotation>

// stub


.. _alias-EnumerationAnnotationPtr:

.. das:attribute:: EnumerationAnnotationPtr = smart_ptr<EnumerationAnnotation>

// stub


.. _alias-PassMacroPtr:

.. das:attribute:: PassMacroPtr = smart_ptr<PassMacro>

// stub


.. _alias-VariantMacroPtr:

.. das:attribute:: VariantMacroPtr = smart_ptr<VariantMacro>

// stub


.. _alias-ReaderMacroPtr:

.. das:attribute:: ReaderMacroPtr = smart_ptr<ReaderMacro>

// stub


.. _alias-CommentReaderPtr:

.. das:attribute:: CommentReaderPtr = smart_ptr<CommentReader>

// stub


.. _alias-CallMacroPtr:

.. das:attribute:: CallMacroPtr = smart_ptr<CallMacro>

// stub


.. _alias-TypeInfoMacroPtr:

.. das:attribute:: TypeInfoMacroPtr = smart_ptr<TypeInfoMacro>

// stub


.. _alias-ForLoopMacroPtr:

.. das:attribute:: ForLoopMacroPtr = smart_ptr<ForLoopMacro>

// stub


.. _alias-CaptureMacroPtr:

.. das:attribute:: CaptureMacroPtr = smart_ptr<CaptureMacro>

// stub


.. _alias-TypeMacroPtr:

.. das:attribute:: TypeMacroPtr = smart_ptr<TypeMacro>

// stub


.. _alias-SimulateMacroPtr:

.. das:attribute:: SimulateMacroPtr = smart_ptr<SimulateMacro>

// stub


++++++++++++
Enumerations
++++++++++++

.. _enum-ast-SideEffects:

.. das:attribute:: SideEffects

:Values: * **none** = 0

         * **unsafe** = 1

         * **userScenario** = 2

         * **modifyExternal** = 4

         * **accessExternal** = 4

         * **modifyArgument** = 8

         * **modifyArgumentAndExternal** = 12

         * **worstDefault** = 12

         * **accessGlobal** = 16

         * **invoke** = 32

         * **inferredSideEffects** = 56

// stub


.. _enum-ast-CaptureMode:

.. das:attribute:: CaptureMode

:Values: * **capture_any** = 0

         * **capture_by_copy** = 1

         * **capture_by_reference** = 2

         * **capture_by_clone** = 3

         * **capture_by_move** = 4

// stub


++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-ast-ModuleLibrary:

.. das:attribute:: ModuleLibrary

// stub


.. _handle-ast-Expression:

.. das:attribute:: Expression

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

// stub


.. _handle-ast-TypeDecl:

.. das:attribute:: TypeDecl

.. _function-_at_ast_c__c__dot__rq_canAot_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.canAot() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isExprType_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isExprType() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isSimpleType_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isSimpleType() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isArray_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isArray() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isGoodIteratorType_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isGoodIteratorType() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isGoodArrayType_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isGoodArrayType() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isGoodTableType_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isGoodTableType() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isGoodBlockType_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isGoodBlockType() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isGoodFunctionType_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isGoodFunctionType() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isGoodLambdaType_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isGoodLambdaType() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isGoodTupleType_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isGoodTupleType() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isGoodVariantType_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isGoodVariantType() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isVoid_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isVoid() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isRef_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isRef() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isRefType_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isRefType() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_canWrite_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.canWrite() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isAotAlias_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isAotAlias() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isShareable_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isShareable() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isIndex_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isIndex() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isBool_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isBool() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isInteger_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isInteger() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isSignedInteger_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isSignedInteger() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isUnsignedInteger_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isUnsignedInteger() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isSignedIntegerOrIntVec_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isSignedIntegerOrIntVec() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isUnsignedIntegerOrIntVec_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isUnsignedIntegerOrIntVec() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isFloatOrDouble_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isFloatOrDouble() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isNumeric_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isNumeric() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isNumericComparable_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isNumericComparable() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isPointer_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isPointer() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isSmartPointer_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isSmartPointer() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isVoidPointer_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isVoidPointer() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isIterator_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isIterator() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isEnum_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isEnum() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isEnumT_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isEnumT() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isHandle_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isHandle() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isStructure_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isStructure() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isClass_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isClass() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isFunction_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isFunction() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isTuple_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isTuple() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isVariant_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isVariant() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_sizeOf_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.sizeOf() : int

// stub


.. _function-_at_ast_c__c__dot__rq_countOf_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.countOf() : int

// stub


.. _function-_at_ast_c__c__dot__rq_alignOf_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.alignOf() : int

// stub


.. _function-_at_ast_c__c__dot__rq_baseSizeOf_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.baseSizeOf() : int

// stub


.. _function-_at_ast_c__c__dot__rq_stride_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.stride() : int

// stub


.. _function-_at_ast_c__c__dot__rq_tupleSize_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.tupleSize() : int

// stub


.. _function-_at_ast_c__c__dot__rq_tupleAlign_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.tupleAlign() : int

// stub


.. _function-_at_ast_c__c__dot__rq_variantSize_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.variantSize() : int

// stub


.. _function-_at_ast_c__c__dot__rq_variantAlign_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.variantAlign() : int

// stub


.. _function-_at_ast_c__c__dot__rq_canCopy_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.canCopy() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_canMove_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.canMove() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_canClone_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.canClone() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_canNew_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.canNew() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_canDeletePtr_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.canDeletePtr() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_canDelete_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.canDelete() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_needDelete_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.needDelete() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isPod_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isPod() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isRawPod_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isRawPod() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isNoHeapType_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isNoHeapType() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isWorkhorseType_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isWorkhorseType() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isPolicyType_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isPolicyType() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isVecPolicyType_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isVecPolicyType() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isReturnType_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isReturnType() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isCtorType_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isCtorType() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isRange_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isRange() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isString_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isString() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isConst_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isConst() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isFoldable_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isFoldable() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isAlias_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isAlias() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isAutoArrayResolved_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isAutoArrayResolved() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isAuto_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isAuto() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isAutoOrAlias_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isAutoOrAlias() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isVectorType_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isVectorType() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isBitfield_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isBitfield() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_isLocal_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.isLocal() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_hasClasses_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.hasClasses() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_hasNonTrivialCtor_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.hasNonTrivialCtor() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_hasNonTrivialDtor_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.hasNonTrivialDtor() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_hasNonTrivialCopy_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.hasNonTrivialCopy() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_canBePlacedInContainer_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.canBePlacedInContainer() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_vectorBaseType_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.vectorBaseType() : Type

// stub


.. _function-_at_ast_c__c__dot__rq_vectorDim_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.vectorDim() : int

// stub


.. _function-_at_ast_c__c__dot__rq_canInitWithZero_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.canInitWithZero() : bool

// stub


.. _function-_at_ast_c__c__dot__rq_rangeBaseType_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.rangeBaseType() : Type

// stub


.. _function-_at_ast_c__c__dot__rq_unsafeInit_CIH_ls_ast_c__c_TypeDecl_gr_:

.. das:function:: TypeDecl implicit.unsafeInit() : bool

// stub


:Properties: * **canAot** : bool

             * **isExprType** : bool

             * **isSimpleType** : bool

             * **isArray** : bool

             * **isGoodIteratorType** : bool

             * **isGoodArrayType** : bool

             * **isGoodTableType** : bool

             * **isGoodBlockType** : bool

             * **isGoodFunctionType** : bool

             * **isGoodLambdaType** : bool

             * **isGoodTupleType** : bool

             * **isGoodVariantType** : bool

             * **isVoid** : bool

             * **isRef** : bool

             * **isRefType** : bool

             * **canWrite** : bool

             * **isAotAlias** : bool

             * **isShareable** : bool

             * **isIndex** : bool

             * **isBool** : bool

             * **isInteger** : bool

             * **isSignedInteger** : bool

             * **isUnsignedInteger** : bool

             * **isSignedIntegerOrIntVec** : bool

             * **isUnsignedIntegerOrIntVec** : bool

             * **isFloatOrDouble** : bool

             * **isNumeric** : bool

             * **isNumericComparable** : bool

             * **isPointer** : bool

             * **isSmartPointer** : bool

             * **isVoidPointer** : bool

             * **isIterator** : bool

             * **isEnum** : bool

             * **isEnumT** : bool

             * **isHandle** : bool

             * **isStructure** : bool

             * **isClass** : bool

             * **isFunction** : bool

             * **isTuple** : bool

             * **isVariant** : bool

             * **sizeOf** : int

             * **countOf** : int

             * **alignOf** : int

             * **baseSizeOf** : int

             * **stride** : int

             * **tupleSize** : int

             * **tupleAlign** : int

             * **variantSize** : int

             * **variantAlign** : int

             * **canCopy** : bool

             * **canMove** : bool

             * **canClone** : bool

             * **canNew** : bool

             * **canDeletePtr** : bool

             * **canDelete** : bool

             * **needDelete** : bool

             * **isPod** : bool

             * **isRawPod** : bool

             * **isNoHeapType** : bool

             * **isWorkhorseType** : bool

             * **isPolicyType** : bool

             * **isVecPolicyType** : bool

             * **isReturnType** : bool

             * **isCtorType** : bool

             * **isRange** : bool

             * **isString** : bool

             * **isConst** : bool

             * **isFoldable** : bool

             * **isAlias** : bool

             * **isAutoArrayResolved** : bool

             * **isAuto** : bool

             * **isAutoOrAlias** : bool

             * **isVectorType** : bool

             * **isBitfield** : bool

             * **isLocal** : bool

             * **hasClasses** : bool

             * **hasNonTrivialCtor** : bool

             * **hasNonTrivialDtor** : bool

             * **hasNonTrivialCopy** : bool

             * **canBePlacedInContainer** : bool

             * **vectorBaseType** :  :ref:`Type <enum-rtti-Type>` 

             * **vectorDim** : int

             * **canInitWithZero** : bool

             * **rangeBaseType** :  :ref:`Type <enum-rtti-Type>` 

             * **unsafeInit** : bool

:Fields: * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **structType** :  :ref:`Structure <handle-ast-Structure>` ?

         * **enumType** :  :ref:`Enumeration <handle-ast-Enumeration>` ?

         * **annotation** :  :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>` ?

         * **firstType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **secondType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **argTypes** : vector<smart_ptr<TypeDecl>>

         * **argNames** : vector<das_string>

         * **dim** : vector<int>

         * **dimExpr** : vector<smart_ptr<Expression>>

         * **flags** :  :ref:`TypeDeclFlags <alias-TypeDeclFlags>` 

         * **alias** :  :ref:`das_string <handle-builtin-das_string>` 

         * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_module** :  :ref:`Module <handle-rtti-Module>` ?

// stub


.. _handle-ast-Structure:

.. das:attribute:: Structure

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **fields** : vector<FieldDeclaration>

         * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_module** :  :ref:`Module <handle-rtti-Module>` ?

         * **parent** :  :ref:`Structure <handle-ast-Structure>` ?

         * **annotations** :  :ref:`AnnotationList <handle-rtti-AnnotationList>` 

         * **flags** :  :ref:`StructureFlags <alias-StructureFlags>` 

// stub


.. _handle-ast-FieldDeclaration:

.. das:attribute:: FieldDeclaration

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **init** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **annotation** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

         * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **offset** : int

         * **flags** :  :ref:`FieldDeclarationFlags <alias-FieldDeclarationFlags>` 

// stub


.. _handle-ast-EnumEntry:

.. das:attribute:: EnumEntry

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **value** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

// stub


.. _handle-ast-Enumeration:

.. das:attribute:: Enumeration

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **cppName** :  :ref:`das_string <handle-builtin-das_string>` 

         * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **list** : vector<EnumEntry>

         * **_module** :  :ref:`Module <handle-rtti-Module>` ?

         * **external** : bool

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **annotations** :  :ref:`AnnotationList <handle-rtti-AnnotationList>` 

         * **isPrivate** : bool

// stub


.. _handle-ast-Function:

.. das:attribute:: Function

.. _function-_at_ast_c__c__dot__rq_origin_CIH_ls_ast_c__c_Function_gr_:

.. das:function:: Function implicit.origin() : Function?

// stub


.. _function-_at_ast_c__c__dot__rq_isGeneric_CIH_ls_ast_c__c_Function_gr_:

.. das:function:: Function implicit.isGeneric() : bool

// stub


:Properties: * **origin** :  :ref:`Function <handle-ast-Function>` ?

             * **isGeneric** : bool

:Fields: * **annotations** :  :ref:`AnnotationList <handle-rtti-AnnotationList>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Variable>>

         * **result** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **body** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **index** : int

         * **totalStackSize** : uint

         * **totalGenLabel** : int

         * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **atDecl** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_module** :  :ref:`Module <handle-rtti-Module>` ?

         * **classParent** :  :ref:`Structure <handle-ast-Structure>` ?

         * **flags** :  :ref:`FunctionFlags <alias-FunctionFlags>` 

         * **moreFlags** :  :ref:`MoreFunctionFlags <alias-MoreFunctionFlags>` 

         * **sideEffectFlags** :  :ref:`FunctionSideEffectFlags <alias-FunctionSideEffectFlags>` 

         * **inferStack** : vector<InferHistory>

         * **fromGeneric** : smart_ptr< :ref:`Function <handle-ast-Function>` >

         * **hash** : uint64

         * **aotHash** : uint64

// stub


.. _handle-ast-InferHistory:

.. das:attribute:: InferHistory

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **func** :  :ref:`Function <handle-ast-Function>` ?

// stub


.. _handle-ast-Variable:

.. das:attribute:: Variable

.. _function-_at_ast_c__c__dot__rq_isAccessUnused_CIH_ls_ast_c__c_Variable_gr_:

.. das:function:: Variable implicit.isAccessUnused() : bool

// stub


:Properties: * **isAccessUnused** : bool

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **_aka** :  :ref:`das_string <handle-builtin-das_string>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **init** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **source** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **index** : int

         * **stackTop** : uint

         * **_module** :  :ref:`Module <handle-rtti-Module>` ?

         * **initStackSize** : uint

         * **flags** :  :ref:`VariableFlags <alias-VariableFlags>` 

         * **access_flags** :  :ref:`VariableAccessFlags <alias-VariableAccessFlags>` 

         * **annotation** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

// stub


.. _handle-ast-AstContext:

.. das:attribute:: AstContext

:Fields: * **func** : smart_ptr< :ref:`Function <handle-ast-Function>` >

         * **_loop** : vector<smart_ptr<Expression>>

         * **blocks** : vector<smart_ptr<Expression>>

         * **scopes** : vector<smart_ptr<Expression>>

         * **_with** : vector<smart_ptr<Expression>>

// stub


.. _handle-ast-ExprBlock:

.. das:attribute:: ExprBlock

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **list** : vector<smart_ptr<Expression>>

         * **finalList** : vector<smart_ptr<Expression>>

         * **returnType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **arguments** : vector<smart_ptr<Variable>>

         * **stackTop** : uint

         * **stackVarTop** : uint

         * **stackVarBottom** : uint

         * **stackCleanVars** : vector<pair`uint`uint>

         * **maxLabelIndex** : int

         * **annotations** :  :ref:`AnnotationList <handle-rtti-AnnotationList>` 

         * **annotationData** : uint64

         * **annotationDataSid** : uint64

         * **blockFlags** :  :ref:`ExprBlockFlags <alias-ExprBlockFlags>` 

// stub


.. _handle-ast-ExprLet:

.. das:attribute:: ExprLet

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **variables** : vector<smart_ptr<Variable>>

         * **atInit** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **letFlags** :  :ref:`ExprLetFlags <alias-ExprLetFlags>` 

// stub


.. _handle-ast-ExprStringBuilder:

.. das:attribute:: ExprStringBuilder

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **elements** : vector<smart_ptr<Expression>>

         * **stringBuilderFlags** :  :ref:`StringBuilderFlags <alias-StringBuilderFlags>` 

// stub


.. _handle-ast-MakeFieldDecl:

.. das:attribute:: MakeFieldDecl

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **value** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **tag** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **flags** :  :ref:`MakeFieldDeclFlags <alias-MakeFieldDeclFlags>` 

// stub


.. _handle-ast-ExprNamedCall:

.. das:attribute:: ExprNamedCall

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **nonNamedArguments** : vector<smart_ptr<Expression>>

         * **arguments** :  :ref:`MakeStruct <handle-ast-MakeStruct>` 

         * **argumentsFailedToInfer** : bool

// stub


.. _handle-ast-ExprLooksLikeCall:

.. das:attribute:: ExprLooksLikeCall

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

// stub


.. _handle-ast-ExprCallFunc:

.. das:attribute:: ExprCallFunc

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **func** :  :ref:`Function <handle-ast-Function>` ?

         * **stackTop** : uint

// stub


.. _handle-ast-ExprNew:

.. das:attribute:: ExprNew

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **func** :  :ref:`Function <handle-ast-Function>` ?

         * **stackTop** : uint

         * **typeexpr** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **initializer** : bool

// stub


.. _handle-ast-ExprCall:

.. das:attribute:: ExprCall

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **func** :  :ref:`Function <handle-ast-Function>` ?

         * **stackTop** : uint

         * **doesNotNeedSp** : bool

         * **cmresAlias** : bool

         * **notDiscarded** : bool

// stub


.. _handle-ast-ExprPtr2Ref:

.. das:attribute:: ExprPtr2Ref

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **unsafeDeref** : bool

         * **assumeNoAlias** : bool

// stub


.. _handle-ast-ExprNullCoalescing:

.. das:attribute:: ExprNullCoalescing

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **unsafeDeref** : bool

         * **assumeNoAlias** : bool

         * **defaultValue** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

// stub


.. _handle-ast-ExprAt:

.. das:attribute:: ExprAt

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **index** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **atFlags** :  :ref:`ExprAtFlags <alias-ExprAtFlags>` 

// stub


.. _handle-ast-ExprSafeAt:

.. das:attribute:: ExprSafeAt

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **index** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **atFlags** :  :ref:`ExprAtFlags <alias-ExprAtFlags>` 

// stub


.. _handle-ast-ExprIs:

.. das:attribute:: ExprIs

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **typeexpr** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

// stub


.. _handle-ast-ExprOp:

.. das:attribute:: ExprOp

// stub


.. _handle-ast-ExprOp2:

.. das:attribute:: ExprOp2

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **func** :  :ref:`Function <handle-ast-Function>` ?

         * **stackTop** : uint

         * **op** :  :ref:`das_string <handle-builtin-das_string>` 

         * **left** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **right** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

// stub


.. _handle-ast-ExprOp3:

.. das:attribute:: ExprOp3

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **func** :  :ref:`Function <handle-ast-Function>` ?

         * **stackTop** : uint

         * **op** :  :ref:`das_string <handle-builtin-das_string>` 

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **left** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **right** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

// stub


.. _handle-ast-ExprCopy:

.. das:attribute:: ExprCopy

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **func** :  :ref:`Function <handle-ast-Function>` ?

         * **stackTop** : uint

         * **op** :  :ref:`das_string <handle-builtin-das_string>` 

         * **left** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **right** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **copy_flags** :  :ref:`CopyFlags <alias-CopyFlags>` 

// stub


.. _handle-ast-ExprMove:

.. das:attribute:: ExprMove

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **func** :  :ref:`Function <handle-ast-Function>` ?

         * **stackTop** : uint

         * **op** :  :ref:`das_string <handle-builtin-das_string>` 

         * **left** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **right** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **move_flags** :  :ref:`MoveFlags <alias-MoveFlags>` 

// stub


.. _handle-ast-ExprClone:

.. das:attribute:: ExprClone

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **func** :  :ref:`Function <handle-ast-Function>` ?

         * **stackTop** : uint

         * **op** :  :ref:`das_string <handle-builtin-das_string>` 

         * **left** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **right** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

// stub


.. _handle-ast-ExprWith:

.. das:attribute:: ExprWith

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **_with** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **body** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

// stub


.. _handle-ast-ExprAssume:

.. das:attribute:: ExprAssume

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **alias** :  :ref:`das_string <handle-builtin-das_string>` 

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

// stub


.. _handle-ast-ExprWhile:

.. das:attribute:: ExprWhile

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **cond** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **body** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

// stub


.. _handle-ast-ExprTryCatch:

.. das:attribute:: ExprTryCatch

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **try_block** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **catch_block** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

// stub


.. _handle-ast-ExprIfThenElse:

.. das:attribute:: ExprIfThenElse

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **cond** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **if_true** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **if_false** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **if_flags** :  :ref:`IfFlags <alias-IfFlags>` 

// stub


.. _handle-ast-ExprFor:

.. das:attribute:: ExprFor

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **iterators** : vector<das_string>

         * **iteratorsAka** : vector<das_string>

         * **iteratorsAt** : vector<LineInfo>

         * **iteratorsTags** : vector<smart_ptr<Expression>>

         * **iteratorVariables** : vector<smart_ptr<Variable>>

         * **sources** : vector<smart_ptr<Expression>>

         * **body** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **visibility** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **allowIteratorOptimization** : bool

         * **canShadow** : bool

// stub


.. _handle-ast-ExprMakeLocal:

.. das:attribute:: ExprMakeLocal

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **makeType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **stackTop** : uint

         * **extraOffset** : uint

         * **makeFlags** :  :ref:`ExprMakeLocalFlags <alias-ExprMakeLocalFlags>` 

// stub


.. _handle-ast-ExprMakeStruct:

.. das:attribute:: ExprMakeStruct

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **makeType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **stackTop** : uint

         * **extraOffset** : uint

         * **makeFlags** :  :ref:`ExprMakeLocalFlags <alias-ExprMakeLocalFlags>` 

         * **structs** : vector<smart_ptr<MakeStruct>>

         * **_block** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **constructor** :  :ref:`Function <handle-ast-Function>` ?

         * **makeStructFlags** :  :ref:`ExprMakeStructFlags <alias-ExprMakeStructFlags>` 

// stub


.. _handle-ast-ExprMakeVariant:

.. das:attribute:: ExprMakeVariant

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **makeType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **stackTop** : uint

         * **extraOffset** : uint

         * **makeFlags** :  :ref:`ExprMakeLocalFlags <alias-ExprMakeLocalFlags>` 

         * **variants** : vector<smart_ptr<MakeFieldDecl>>

// stub


.. _handle-ast-ExprMakeArray:

.. das:attribute:: ExprMakeArray

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **makeType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **stackTop** : uint

         * **extraOffset** : uint

         * **makeFlags** :  :ref:`ExprMakeLocalFlags <alias-ExprMakeLocalFlags>` 

         * **recordType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **values** : vector<smart_ptr<Expression>>

// stub


.. _handle-ast-ExprMakeTuple:

.. das:attribute:: ExprMakeTuple

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **makeType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **stackTop** : uint

         * **extraOffset** : uint

         * **makeFlags** :  :ref:`ExprMakeLocalFlags <alias-ExprMakeLocalFlags>` 

         * **recordType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **values** : vector<smart_ptr<Expression>>

         * **isKeyValue** : bool

// stub


.. _handle-ast-ExprArrayComprehension:

.. das:attribute:: ExprArrayComprehension

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **exprFor** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **exprWhere** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **generatorSyntax** : bool

         * **tableSyntax** : bool

// stub


.. _handle-ast-TypeInfoMacro:

.. das:attribute:: TypeInfoMacro

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **_module** :  :ref:`Module <handle-rtti-Module>` ?

// stub


.. _handle-ast-ExprTypeInfo:

.. das:attribute:: ExprTypeInfo

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **trait** :  :ref:`das_string <handle-builtin-das_string>` 

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **typeexpr** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **subtrait** :  :ref:`das_string <handle-builtin-das_string>` 

         * **extratrait** :  :ref:`das_string <handle-builtin-das_string>` 

         * **macro** :  :ref:`TypeInfoMacro <handle-ast-TypeInfoMacro>` ?

// stub


.. _handle-ast-ExprTypeDecl:

.. das:attribute:: ExprTypeDecl

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **typeexpr** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

// stub


.. _handle-ast-ExprLabel:

.. das:attribute:: ExprLabel

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **labelName** : int

         * **comment** :  :ref:`das_string <handle-builtin-das_string>` 

// stub


.. _handle-ast-ExprGoto:

.. das:attribute:: ExprGoto

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **labelName** : int

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

// stub


.. _handle-ast-ExprRef2Value:

.. das:attribute:: ExprRef2Value

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

// stub


.. _handle-ast-ExprRef2Ptr:

.. das:attribute:: ExprRef2Ptr

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

// stub


.. _handle-ast-ExprAddr:

.. das:attribute:: ExprAddr

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **target** :  :ref:`das_string <handle-builtin-das_string>` 

         * **funcType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **func** :  :ref:`Function <handle-ast-Function>` ?

// stub


.. _handle-ast-ExprAssert:

.. das:attribute:: ExprAssert

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **isVerify** : bool

// stub


.. _handle-ast-ExprQuote:

.. das:attribute:: ExprQuote

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

// stub


.. _handle-ast-ExprStaticAssert:

.. das:attribute:: ExprStaticAssert

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

// stub


.. _handle-ast-ExprDebug:

.. das:attribute:: ExprDebug

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

// stub


.. _handle-ast-ExprInvoke:

.. das:attribute:: ExprInvoke

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **stackTop** : uint

         * **doesNotNeedSp** : bool

         * **isInvokeMethod** : bool

// stub


.. _handle-ast-ExprErase:

.. das:attribute:: ExprErase

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

// stub


.. _handle-ast-ExprSetInsert:

.. das:attribute:: ExprSetInsert

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

// stub


.. _handle-ast-ExprFind:

.. das:attribute:: ExprFind

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

// stub


.. _handle-ast-ExprKeyExists:

.. das:attribute:: ExprKeyExists

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

// stub


.. _handle-ast-ExprAscend:

.. das:attribute:: ExprAscend

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **ascType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **stackTop** : uint

         * **ascendFlags** :  :ref:`ExprAscendFlags <alias-ExprAscendFlags>` 

// stub


.. _handle-ast-ExprCast:

.. das:attribute:: ExprCast

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **castType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **castFlags** :  :ref:`ExprCastFlags <alias-ExprCastFlags>` 

// stub


.. _handle-ast-ExprDelete:

.. das:attribute:: ExprDelete

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **sizeexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **native** : bool

// stub


.. _handle-ast-ExprVar:

.. das:attribute:: ExprVar

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **variable** : smart_ptr< :ref:`Variable <handle-ast-Variable>` >

         * **pBlock** :  :ref:`ExprBlock <handle-ast-ExprBlock>` ?

         * **argumentIndex** : int

         * **varFlags** :  :ref:`ExprVarFlags <alias-ExprVarFlags>` 

// stub


.. _handle-ast-ExprTag:

.. das:attribute:: ExprTag

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **value** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

// stub


.. _handle-ast-ExprSwizzle:

.. das:attribute:: ExprSwizzle

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **value** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **mask** :  :ref:`das_string <handle-builtin-das_string>` 

         * **fields** : vector<uint8>

         * **fieldFlags** :  :ref:`ExprSwizzleFieldFlags <alias-ExprSwizzleFieldFlags>` 

// stub


.. _handle-ast-ExprField:

.. das:attribute:: ExprField

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **value** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **atField** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **field** :  :ref:`FieldDeclaration <handle-ast-FieldDeclaration>` ?

         * **fieldIndex** : int

         * **annotation** : smart_ptr< :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>` >

         * **derefFlags** :  :ref:`ExprFieldDerefFlags <alias-ExprFieldDerefFlags>` 

         * **fieldFlags** :  :ref:`ExprFieldFieldFlags <alias-ExprFieldFieldFlags>` 

// stub


.. _handle-ast-ExprSafeField:

.. das:attribute:: ExprSafeField

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **value** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **atField** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **field** :  :ref:`FieldDeclaration <handle-ast-FieldDeclaration>` ?

         * **fieldIndex** : int

         * **annotation** : smart_ptr< :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>` >

         * **derefFlags** :  :ref:`ExprFieldDerefFlags <alias-ExprFieldDerefFlags>` 

         * **fieldFlags** :  :ref:`ExprFieldFieldFlags <alias-ExprFieldFieldFlags>` 

         * **skipQQ** : bool

// stub


.. _handle-ast-ExprIsVariant:

.. das:attribute:: ExprIsVariant

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **value** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **atField** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **field** :  :ref:`FieldDeclaration <handle-ast-FieldDeclaration>` ?

         * **fieldIndex** : int

         * **annotation** : smart_ptr< :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>` >

         * **derefFlags** :  :ref:`ExprFieldDerefFlags <alias-ExprFieldDerefFlags>` 

         * **fieldFlags** :  :ref:`ExprFieldFieldFlags <alias-ExprFieldFieldFlags>` 

// stub


.. _handle-ast-ExprAsVariant:

.. das:attribute:: ExprAsVariant

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **value** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **atField** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **field** :  :ref:`FieldDeclaration <handle-ast-FieldDeclaration>` ?

         * **fieldIndex** : int

         * **annotation** : smart_ptr< :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>` >

         * **derefFlags** :  :ref:`ExprFieldDerefFlags <alias-ExprFieldDerefFlags>` 

         * **fieldFlags** :  :ref:`ExprFieldFieldFlags <alias-ExprFieldFieldFlags>` 

// stub


.. _handle-ast-ExprSafeAsVariant:

.. das:attribute:: ExprSafeAsVariant

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **value** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **atField** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **field** :  :ref:`FieldDeclaration <handle-ast-FieldDeclaration>` ?

         * **fieldIndex** : int

         * **annotation** : smart_ptr< :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>` >

         * **derefFlags** :  :ref:`ExprFieldDerefFlags <alias-ExprFieldDerefFlags>` 

         * **fieldFlags** :  :ref:`ExprFieldFieldFlags <alias-ExprFieldFieldFlags>` 

         * **skipQQ** : bool

// stub


.. _handle-ast-ExprOp1:

.. das:attribute:: ExprOp1

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **func** :  :ref:`Function <handle-ast-Function>` ?

         * **stackTop** : uint

         * **op** :  :ref:`das_string <handle-builtin-das_string>` 

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

// stub


.. _handle-ast-ExprReturn:

.. das:attribute:: ExprReturn

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **returnFlags** :  :ref:`ExprReturnFlags <alias-ExprReturnFlags>` 

         * **stackTop** : uint

         * **refStackTop** : uint

         * **block** :  :ref:`ExprBlock <handle-ast-ExprBlock>` ?

// stub


.. _handle-ast-ExprYield:

.. das:attribute:: ExprYield

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **returnFlags** :  :ref:`ExprYieldFlags <alias-ExprYieldFlags>` 

// stub


.. _handle-ast-ExprBreak:

.. das:attribute:: ExprBreak

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

// stub


.. _handle-ast-ExprContinue:

.. das:attribute:: ExprContinue

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

// stub


.. _handle-ast-ExprConst:

.. das:attribute:: ExprConst

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

// stub


.. _handle-ast-ExprFakeContext:

.. das:attribute:: ExprFakeContext

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

// stub


.. _handle-ast-ExprFakeLineInfo:

.. das:attribute:: ExprFakeLineInfo

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : void?

// stub


.. _handle-ast-ExprConstPtr:

.. das:attribute:: ExprConstPtr

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : void?

// stub


.. _handle-ast-ExprConstInt8:

.. das:attribute:: ExprConstInt8

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : int8

// stub


.. _handle-ast-ExprConstInt16:

.. das:attribute:: ExprConstInt16

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : int16

// stub


.. _handle-ast-ExprConstInt64:

.. das:attribute:: ExprConstInt64

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : int64

// stub


.. _handle-ast-ExprConstInt:

.. das:attribute:: ExprConstInt

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : int

// stub


.. _handle-ast-ExprConstInt2:

.. das:attribute:: ExprConstInt2

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : int2

// stub


.. _handle-ast-ExprConstInt3:

.. das:attribute:: ExprConstInt3

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : int3

// stub


.. _handle-ast-ExprConstInt4:

.. das:attribute:: ExprConstInt4

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : int4

// stub


.. _handle-ast-ExprConstUInt8:

.. das:attribute:: ExprConstUInt8

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : uint8

// stub


.. _handle-ast-ExprConstUInt16:

.. das:attribute:: ExprConstUInt16

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : uint16

// stub


.. _handle-ast-ExprConstUInt64:

.. das:attribute:: ExprConstUInt64

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : uint64

// stub


.. _handle-ast-ExprConstUInt:

.. das:attribute:: ExprConstUInt

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : uint

// stub


.. _handle-ast-ExprConstUInt2:

.. das:attribute:: ExprConstUInt2

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : uint2

// stub


.. _handle-ast-ExprConstUInt3:

.. das:attribute:: ExprConstUInt3

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : uint3

// stub


.. _handle-ast-ExprConstUInt4:

.. das:attribute:: ExprConstUInt4

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : uint4

// stub


.. _handle-ast-ExprConstRange:

.. das:attribute:: ExprConstRange

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : range

// stub


.. _handle-ast-ExprConstURange:

.. das:attribute:: ExprConstURange

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : urange

// stub


.. _handle-ast-ExprConstRange64:

.. das:attribute:: ExprConstRange64

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : range64

// stub


.. _handle-ast-ExprConstURange64:

.. das:attribute:: ExprConstURange64

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : urange64

// stub


.. _handle-ast-ExprConstFloat:

.. das:attribute:: ExprConstFloat

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : float

// stub


.. _handle-ast-ExprConstFloat2:

.. das:attribute:: ExprConstFloat2

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : float2

// stub


.. _handle-ast-ExprConstFloat3:

.. das:attribute:: ExprConstFloat3

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : float3

// stub


.. _handle-ast-ExprConstFloat4:

.. das:attribute:: ExprConstFloat4

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : float4

// stub


.. _handle-ast-ExprConstDouble:

.. das:attribute:: ExprConstDouble

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : double

// stub


.. _handle-ast-ExprConstBool:

.. das:attribute:: ExprConstBool

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : bool

// stub


.. _handle-ast-CaptureEntry:

.. das:attribute:: CaptureEntry

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **mode** :  :ref:`CaptureMode <enum-ast-CaptureMode>` 

// stub


.. _handle-ast-ExprMakeBlock:

.. das:attribute:: ExprMakeBlock

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **capture** : vector<CaptureEntry>

         * **_block** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **stackTop** : uint

         * **mmFlags** :  :ref:`ExprMakeBlockFlags <alias-ExprMakeBlockFlags>` 

// stub


.. _handle-ast-ExprMakeGenerator:

.. das:attribute:: ExprMakeGenerator

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **iterType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **capture** : vector<CaptureEntry>

// stub


.. _handle-ast-ExprMemZero:

.. das:attribute:: ExprMemZero

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

// stub


.. _handle-ast-ExprConstEnumeration:

.. das:attribute:: ExprConstEnumeration

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **enumType** : smart_ptr< :ref:`Enumeration <handle-ast-Enumeration>` >

         * **value** :  :ref:`das_string <handle-builtin-das_string>` 

// stub


.. _handle-ast-ExprConstBitfield:

.. das:attribute:: ExprConstBitfield

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : bitfield<>

         * **bitfieldType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

// stub


.. _handle-ast-ExprConstString:

.. das:attribute:: ExprConstString

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** :  :ref:`das_string <handle-builtin-das_string>` 

// stub


.. _handle-ast-ExprUnsafe:

.. das:attribute:: ExprUnsafe

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **body** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

// stub


.. _handle-ast-VisitorAdapter:

.. das:attribute:: VisitorAdapter

// stub


.. _handle-ast-FunctionAnnotation:

.. das:attribute:: FunctionAnnotation

// stub


.. _handle-ast-StructureAnnotation:

.. das:attribute:: StructureAnnotation

// stub


.. _handle-ast-EnumerationAnnotation:

.. das:attribute:: EnumerationAnnotation

// stub


.. _handle-ast-PassMacro:

.. das:attribute:: PassMacro

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` 

// stub


.. _handle-ast-ReaderMacro:

.. das:attribute:: ReaderMacro

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **_module** :  :ref:`Module <handle-rtti-Module>` ?

// stub


.. _handle-ast-CommentReader:

.. das:attribute:: CommentReader

// stub


.. _handle-ast-CallMacro:

.. das:attribute:: CallMacro

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **_module** :  :ref:`Module <handle-rtti-Module>` ?

// stub


.. _handle-ast-VariantMacro:

.. das:attribute:: VariantMacro

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` 

// stub


.. _handle-ast-ForLoopMacro:

.. das:attribute:: ForLoopMacro

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` 

// stub


.. _handle-ast-CaptureMacro:

.. das:attribute:: CaptureMacro

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` 

// stub


.. _handle-ast-TypeMacro:

.. das:attribute:: TypeMacro

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` 

// stub


.. _handle-ast-SimulateMacro:

.. das:attribute:: SimulateMacro

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` 

// stub


.. _handle-ast-ExprReader:

.. das:attribute:: ExprReader

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **macro** : smart_ptr< :ref:`ReaderMacro <handle-ast-ReaderMacro>` >

         * **sequence** :  :ref:`das_string <handle-builtin-das_string>` 

// stub


.. _handle-ast-ExprCallMacro:

.. das:attribute:: ExprCallMacro

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **macro** :  :ref:`CallMacro <handle-ast-CallMacro>` ?

// stub


+++++++++++
Call macros
+++++++++++

.. _call-macro-ast-quote:

.. das:attribute:: quote

// stub


+++++++++++++++
Typeinfo macros
+++++++++++++++

.. _call-macro-ast-ast_typedecl:

.. das:attribute:: ast_typedecl

// stub


.. _call-macro-ast-ast_function:

.. das:attribute:: ast_function

// stub


+++++++++++++
Handled types
+++++++++++++

.. _handle-ast-MakeStruct:

.. das:attribute:: MakeStruct

// stub


+++++++
Classes
+++++++

.. _struct-ast-AstFunctionAnnotation:

.. das:attribute:: AstFunctionAnnotation

// stub


.. _struct-ast-AstBlockAnnotation:

.. das:attribute:: AstBlockAnnotation

// stub


.. _struct-ast-AstStructureAnnotation:

.. das:attribute:: AstStructureAnnotation

// stub


.. _struct-ast-AstPassMacro:

.. das:attribute:: AstPassMacro

// stub


.. _struct-ast-AstVariantMacro:

.. das:attribute:: AstVariantMacro

// stub


.. _struct-ast-AstForLoopMacro:

.. das:attribute:: AstForLoopMacro

// stub


.. _struct-ast-AstCaptureMacro:

.. das:attribute:: AstCaptureMacro

// stub


.. _struct-ast-AstTypeMacro:

.. das:attribute:: AstTypeMacro

// stub


.. _struct-ast-AstSimulateMacro:

.. das:attribute:: AstSimulateMacro

// stub


.. _struct-ast-AstReaderMacro:

.. das:attribute:: AstReaderMacro

// stub


.. _struct-ast-AstCommentReader:

.. das:attribute:: AstCommentReader

// stub


.. _struct-ast-AstCallMacro:

.. das:attribute:: AstCallMacro

// stub


.. _struct-ast-AstTypeInfoMacro:

.. das:attribute:: AstTypeInfoMacro

// stub


.. _struct-ast-AstEnumerationAnnotation:

.. das:attribute:: AstEnumerationAnnotation

// stub


.. _struct-ast-AstVisitor:

.. das:attribute:: AstVisitor

// stub


+++++++++++++++
Call generation
+++++++++++++++

  *  :ref:`make_call (at: LineInfo implicit; name: string implicit) : smart_ptr\<Expression\> <function-_at_ast_c__c_make_call_CIH_ls_rtti_c__c_LineInfo_gr__CIs>` 

.. _function-_at_ast_c__c_make_call_CIH_ls_rtti_c__c_LineInfo_gr__CIs:

.. das:function:: make_call(at: LineInfo implicit; name: string implicit) : smart_ptr<Expression>

// stub


:Arguments: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>`  implicit

            * **name** : string implicit

+++++++++++++++
Visitor pattern
+++++++++++++++

  *  :ref:`visit (program: smart_ptr\<Program\> implicit; adapter: smart_ptr\<VisitorAdapter\> implicit) <function-_at_ast_c__c_visit_CI1_ls_H_ls_rtti_c__c_Program_gr__gr__qm_M_CI1_ls_H_ls_ast_c__c_VisitorAdapter_gr__gr__qm_M_C_c_C_l>` 
  *  :ref:`visit_modules (program: smart_ptr\<Program\> implicit; adapter: smart_ptr\<VisitorAdapter\> implicit) <function-_at_ast_c__c_visit_modules_CI1_ls_H_ls_rtti_c__c_Program_gr__gr__qm_M_CI1_ls_H_ls_ast_c__c_VisitorAdapter_gr__gr__qm_M_C_c_C_l>` 
  *  :ref:`visit (function: smart_ptr\<Function\> implicit; adapter: smart_ptr\<VisitorAdapter\> implicit) <function-_at_ast_c__c_visit_CI1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M_CI1_ls_H_ls_ast_c__c_VisitorAdapter_gr__gr__qm_M_C_c_C_l>` 
  *  :ref:`visit (expression: smart_ptr\<Expression\> implicit; adapter: smart_ptr\<VisitorAdapter\> implicit) : smart_ptr\<Expression\> <function-_at_ast_c__c_visit_CI1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M_CI1_ls_H_ls_ast_c__c_VisitorAdapter_gr__gr__qm_M_C_c_C_l>` 
  *  :ref:`visit_finally (expression: smart_ptr\<ExprBlock\> implicit; adapter: smart_ptr\<VisitorAdapter\> implicit) <function-_at_ast_c__c_visit_finally_CI1_ls_H_ls_ast_c__c_ExprBlock_gr__gr__qm_M_CI1_ls_H_ls_ast_c__c_VisitorAdapter_gr__gr__qm_M_C_c_C_l>` 

.. _function-_at_ast_c__c_visit_CI1_ls_H_ls_rtti_c__c_Program_gr__gr__qm_M_CI1_ls_H_ls_ast_c__c_VisitorAdapter_gr__gr__qm_M_C_c_C_l:

.. das:function:: visit(program: smart_ptr<Program> implicit; adapter: smart_ptr<VisitorAdapter> implicit)

// stub


:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>` > implicit

            * **adapter** : smart_ptr< :ref:`VisitorAdapter <handle-ast-VisitorAdapter>` > implicit

.. _function-_at_ast_c__c_visit_modules_CI1_ls_H_ls_rtti_c__c_Program_gr__gr__qm_M_CI1_ls_H_ls_ast_c__c_VisitorAdapter_gr__gr__qm_M_C_c_C_l:

.. das:function:: visit_modules(program: smart_ptr<Program> implicit; adapter: smart_ptr<VisitorAdapter> implicit)

// stub


:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>` > implicit

            * **adapter** : smart_ptr< :ref:`VisitorAdapter <handle-ast-VisitorAdapter>` > implicit

.. _function-_at_ast_c__c_visit_CI1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M_CI1_ls_H_ls_ast_c__c_VisitorAdapter_gr__gr__qm_M_C_c_C_l:

.. das:function:: visit(function: smart_ptr<Function> implicit; adapter: smart_ptr<VisitorAdapter> implicit)

// stub


:Arguments: * **function** : smart_ptr< :ref:`Function <handle-ast-Function>` > implicit

            * **adapter** : smart_ptr< :ref:`VisitorAdapter <handle-ast-VisitorAdapter>` > implicit

.. _function-_at_ast_c__c_visit_CI1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M_CI1_ls_H_ls_ast_c__c_VisitorAdapter_gr__gr__qm_M_C_c_C_l:

.. das:function:: visit(expression: smart_ptr<Expression> implicit; adapter: smart_ptr<VisitorAdapter> implicit) : smart_ptr<Expression>

// stub


:Arguments: * **expression** : smart_ptr< :ref:`Expression <handle-ast-Expression>` > implicit

            * **adapter** : smart_ptr< :ref:`VisitorAdapter <handle-ast-VisitorAdapter>` > implicit

.. _function-_at_ast_c__c_visit_finally_CI1_ls_H_ls_ast_c__c_ExprBlock_gr__gr__qm_M_CI1_ls_H_ls_ast_c__c_VisitorAdapter_gr__gr__qm_M_C_c_C_l:

.. das:function:: visit_finally(expression: smart_ptr<ExprBlock> implicit; adapter: smart_ptr<VisitorAdapter> implicit)

// stub


:Arguments: * **expression** : smart_ptr< :ref:`ExprBlock <handle-ast-ExprBlock>` > implicit

            * **adapter** : smart_ptr< :ref:`VisitorAdapter <handle-ast-VisitorAdapter>` > implicit

+++++++++++++++++++++
Expression generation
+++++++++++++++++++++

  *  :ref:`force_generated (expression: smart_ptr\<Expression\> const& implicit; value: bool) <function-_at_ast_c__c_force_generated_C&I1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M_Cb>` 
  *  :ref:`get_expression_annotation (expr: Expression? implicit) : Annotation? <function-_at_ast_c__c_get_expression_annotation_CI1_ls_H_ls_ast_c__c_Expression_gr__gr__qm__C_c_C_l>` 
  *  :ref:`make_type_info_structure (ctx: Context implicit; type: smart_ptr\<TypeDecl\> implicit) : TypeInfo? <function-_at_ast_c__c_make_type_info_structure_IH_ls_rtti_c__c_Context_gr__CI1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_W_C_c_C_l>` 

.. _function-_at_ast_c__c_force_generated_C&I1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M_Cb:

.. das:function:: force_generated(expression: smart_ptr<Expression> const& implicit; value: bool)

// stub


:Arguments: * **expression** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >& implicit

            * **value** : bool

.. _function-_at_ast_c__c_get_expression_annotation_CI1_ls_H_ls_ast_c__c_Expression_gr__gr__qm__C_c_C_l:

.. das:function:: get_expression_annotation(expr: Expression? implicit) : Annotation?

// stub


:Arguments: * **expr** :  :ref:`Expression <handle-ast-Expression>` ? implicit

.. _function-_at_ast_c__c_make_type_info_structure_IH_ls_rtti_c__c_Context_gr__CI1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_W_C_c_C_l:

.. das:function:: make_type_info_structure(ctx: Context implicit; type: smart_ptr<TypeDecl> implicit) : TypeInfo?

// stub


:Arguments: * **ctx** :  :ref:`Context <handle-rtti-Context>`  implicit

            * **type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

++++++++++++++++++
Adapter generation
++++++++++++++++++

  *  :ref:`make_visitor (class: void? implicit; info: StructInfo const? implicit) : smart_ptr\<VisitorAdapter\> <function-_at_ast_c__c_make_visitor_CI_qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__C_c>` 
  *  :ref:`make_function_annotation (name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr\<FunctionAnnotation\> <function-_at_ast_c__c_make_function_annotation_CIs_CI_qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__C_c>` 
  *  :ref:`make_block_annotation (name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr\<FunctionAnnotation\> <function-_at_ast_c__c_make_block_annotation_CIs_CI_qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__C_c>` 
  *  :ref:`make_structure_annotation (name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr\<StructureAnnotation\> <function-_at_ast_c__c_make_structure_annotation_CIs_CI_qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__C_c>` 
  *  :ref:`make_enumeration_annotation (name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr\<EnumerationAnnotation\> <function-_at_ast_c__c_make_enumeration_annotation_CIs_CI_qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__C_c>` 
  *  :ref:`make_pass_macro (name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr\<PassMacro\> <function-_at_ast_c__c_make_pass_macro_CIs_CI_qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__C_c>` 
  *  :ref:`make_reader_macro (name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr\<ReaderMacro\> <function-_at_ast_c__c_make_reader_macro_CIs_CI_qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__C_c>` 
  *  :ref:`make_comment_reader (class: void? implicit; info: StructInfo const? implicit) : smart_ptr\<CommentReader\> <function-_at_ast_c__c_make_comment_reader_CI_qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__C_c>` 
  *  :ref:`make_call_macro (name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr\<CallMacro\> <function-_at_ast_c__c_make_call_macro_CIs_CI_qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__C_c>` 
  *  :ref:`make_typeinfo_macro (name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr\<TypeInfoMacro\> <function-_at_ast_c__c_make_typeinfo_macro_CIs_CI_qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__C_c>` 
  *  :ref:`make_variant_macro (name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr\<VariantMacro\> <function-_at_ast_c__c_make_variant_macro_CIs_CI_qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__C_c>` 
  *  :ref:`make_for_loop_macro (name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr\<ForLoopMacro\> <function-_at_ast_c__c_make_for_loop_macro_CIs_CI_qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__C_c>` 
  *  :ref:`make_capture_macro (name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr\<CaptureMacro\> <function-_at_ast_c__c_make_capture_macro_CIs_CI_qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__C_c>` 
  *  :ref:`make_type_macro (name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr\<TypeMacro\> <function-_at_ast_c__c_make_type_macro_CIs_CI_qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__C_c>` 
  *  :ref:`make_simulate_macro (name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr\<SimulateMacro\> <function-_at_ast_c__c_make_simulate_macro_CIs_CI_qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__C_c>` 
  *  :ref:`make_clone_structure (structure: Structure? implicit) : smart_ptr\<Function\> <function-_at_ast_c__c_make_clone_structure_CI1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_>` 
  *  :ref:`make_function_annotation (name: string; someClassPtr: auto) : FunctionAnnotationPtr <function-_at_ast_c__c_make_function_annotation_Cs_C_dot_>` 
  *  :ref:`make_block_annotation (name: string; someClassPtr: auto) : FunctionAnnotationPtr <function-_at_ast_c__c_make_block_annotation_Cs_C_dot_>` 
  *  :ref:`make_structure_annotation (name: string; someClassPtr: auto) : StructureAnnotationPtr <function-_at_ast_c__c_make_structure_annotation_Cs_C_dot_>` 
  *  :ref:`make_enumeration_annotation (name: string; someClassPtr: auto) : EnumerationAnnotationPtr <function-_at_ast_c__c_make_enumeration_annotation_Cs_C_dot_>` 
  *  :ref:`make_visitor (someClass: auto) : smart_ptr\<VisitorAdapter\> <function-_at_ast_c__c_make_visitor_C_dot_>` 
  *  :ref:`make_reader_macro (name: string; someClassPtr: auto) : ReaderMacroPtr <function-_at_ast_c__c_make_reader_macro_Cs_C_dot_>` 
  *  :ref:`make_comment_reader (name: string; someClassPtr: auto) : CommentReaderPtr <function-_at_ast_c__c_make_comment_reader_Cs_C_dot_>` 
  *  :ref:`make_call_macro (name: string; someClassPtr: auto) : CallMacroPtr <function-_at_ast_c__c_make_call_macro_Cs_C_dot_>` 
  *  :ref:`make_typeinfo_macro (name: string; someClassPtr: auto) : TypeInfoMacroPtr <function-_at_ast_c__c_make_typeinfo_macro_Cs_C_dot_>` 
  *  :ref:`make_pass_macro (name: string; someClassPtr: auto) : PassMacroPtr <function-_at_ast_c__c_make_pass_macro_Cs_C_dot_>` 
  *  :ref:`make_variant_macro (name: string; someClassPtr: auto) : VariantMacroPtr <function-_at_ast_c__c_make_variant_macro_Cs_C_dot_>` 
  *  :ref:`make_for_loop_macro (name: string; someClassPtr: auto) : ForLoopMacroPtr <function-_at_ast_c__c_make_for_loop_macro_Cs_C_dot_>` 
  *  :ref:`make_capture_macro (name: string; someClassPtr: auto) : CaptureMacroPtr <function-_at_ast_c__c_make_capture_macro_Cs_C_dot_>` 
  *  :ref:`make_type_macro (name: string; someClassPtr: auto) : TypeMacroPtr <function-_at_ast_c__c_make_type_macro_Cs_C_dot_>` 
  *  :ref:`make_simulate_macro (name: string; someClassPtr: auto) : SimulateMacroPtr <function-_at_ast_c__c_make_simulate_macro_Cs_C_dot_>` 

.. _function-_at_ast_c__c_make_visitor_CI_qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__C_c:

.. das:function:: make_visitor(class: void? implicit; info: StructInfo const? implicit) : smart_ptr<VisitorAdapter>

// stub


:Arguments: * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-_at_ast_c__c_make_function_annotation_CIs_CI_qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__C_c:

.. das:function:: make_function_annotation(name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr<FunctionAnnotation>

// stub


:Arguments: * **name** : string implicit

            * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-_at_ast_c__c_make_block_annotation_CIs_CI_qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__C_c:

.. das:function:: make_block_annotation(name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr<FunctionAnnotation>

// stub


:Arguments: * **name** : string implicit

            * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-_at_ast_c__c_make_structure_annotation_CIs_CI_qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__C_c:

.. das:function:: make_structure_annotation(name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr<StructureAnnotation>

// stub


:Arguments: * **name** : string implicit

            * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-_at_ast_c__c_make_enumeration_annotation_CIs_CI_qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__C_c:

.. das:function:: make_enumeration_annotation(name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr<EnumerationAnnotation>

// stub


:Arguments: * **name** : string implicit

            * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-_at_ast_c__c_make_pass_macro_CIs_CI_qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__C_c:

.. das:function:: make_pass_macro(name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr<PassMacro>

// stub


:Arguments: * **name** : string implicit

            * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-_at_ast_c__c_make_reader_macro_CIs_CI_qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__C_c:

.. das:function:: make_reader_macro(name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr<ReaderMacro>

// stub


:Arguments: * **name** : string implicit

            * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-_at_ast_c__c_make_comment_reader_CI_qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__C_c:

.. das:function:: make_comment_reader(class: void? implicit; info: StructInfo const? implicit) : smart_ptr<CommentReader>

// stub


:Arguments: * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-_at_ast_c__c_make_call_macro_CIs_CI_qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__C_c:

.. das:function:: make_call_macro(name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr<CallMacro>

// stub


:Arguments: * **name** : string implicit

            * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-_at_ast_c__c_make_typeinfo_macro_CIs_CI_qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__C_c:

.. das:function:: make_typeinfo_macro(name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr<TypeInfoMacro>

// stub


:Arguments: * **name** : string implicit

            * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-_at_ast_c__c_make_variant_macro_CIs_CI_qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__C_c:

.. das:function:: make_variant_macro(name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr<VariantMacro>

// stub


:Arguments: * **name** : string implicit

            * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-_at_ast_c__c_make_for_loop_macro_CIs_CI_qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__C_c:

.. das:function:: make_for_loop_macro(name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr<ForLoopMacro>

// stub


:Arguments: * **name** : string implicit

            * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-_at_ast_c__c_make_capture_macro_CIs_CI_qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__C_c:

.. das:function:: make_capture_macro(name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr<CaptureMacro>

// stub


:Arguments: * **name** : string implicit

            * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-_at_ast_c__c_make_type_macro_CIs_CI_qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__C_c:

.. das:function:: make_type_macro(name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr<TypeMacro>

// stub


:Arguments: * **name** : string implicit

            * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-_at_ast_c__c_make_simulate_macro_CIs_CI_qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__C_c:

.. das:function:: make_simulate_macro(name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr<SimulateMacro>

// stub


:Arguments: * **name** : string implicit

            * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-_at_ast_c__c_make_clone_structure_CI1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_:

.. das:function:: make_clone_structure(structure: Structure? implicit) : smart_ptr<Function>

// stub


:Arguments: * **structure** :  :ref:`Structure <handle-ast-Structure>` ? implicit

.. _function-_at_ast_c__c_make_function_annotation_Cs_C_dot_:

.. das:function:: make_function_annotation(name: string; someClassPtr: auto) : FunctionAnnotationPtr

// stub


:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-_at_ast_c__c_make_block_annotation_Cs_C_dot_:

.. das:function:: make_block_annotation(name: string; someClassPtr: auto) : FunctionAnnotationPtr

// stub


:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-_at_ast_c__c_make_structure_annotation_Cs_C_dot_:

.. das:function:: make_structure_annotation(name: string; someClassPtr: auto) : StructureAnnotationPtr

// stub


:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-_at_ast_c__c_make_enumeration_annotation_Cs_C_dot_:

.. das:function:: make_enumeration_annotation(name: string; someClassPtr: auto) : EnumerationAnnotationPtr

// stub


:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-_at_ast_c__c_make_visitor_C_dot_:

.. das:function:: make_visitor(someClass: auto) : smart_ptr<VisitorAdapter>

// stub


:Arguments: * **someClass** : auto

.. _function-_at_ast_c__c_make_reader_macro_Cs_C_dot_:

.. das:function:: make_reader_macro(name: string; someClassPtr: auto) : ReaderMacroPtr

// stub


:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-_at_ast_c__c_make_comment_reader_Cs_C_dot_:

.. das:function:: make_comment_reader(name: string; someClassPtr: auto) : CommentReaderPtr

// stub


:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-_at_ast_c__c_make_call_macro_Cs_C_dot_:

.. das:function:: make_call_macro(name: string; someClassPtr: auto) : CallMacroPtr

// stub


:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-_at_ast_c__c_make_typeinfo_macro_Cs_C_dot_:

.. das:function:: make_typeinfo_macro(name: string; someClassPtr: auto) : TypeInfoMacroPtr

// stub


:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-_at_ast_c__c_make_pass_macro_Cs_C_dot_:

.. das:function:: make_pass_macro(name: string; someClassPtr: auto) : PassMacroPtr

// stub


:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-_at_ast_c__c_make_variant_macro_Cs_C_dot_:

.. das:function:: make_variant_macro(name: string; someClassPtr: auto) : VariantMacroPtr

// stub


:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-_at_ast_c__c_make_for_loop_macro_Cs_C_dot_:

.. das:function:: make_for_loop_macro(name: string; someClassPtr: auto) : ForLoopMacroPtr

// stub


:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-_at_ast_c__c_make_capture_macro_Cs_C_dot_:

.. das:function:: make_capture_macro(name: string; someClassPtr: auto) : CaptureMacroPtr

// stub


:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-_at_ast_c__c_make_type_macro_Cs_C_dot_:

.. das:function:: make_type_macro(name: string; someClassPtr: auto) : TypeMacroPtr

// stub


:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-_at_ast_c__c_make_simulate_macro_Cs_C_dot_:

.. das:function:: make_simulate_macro(name: string; someClassPtr: auto) : SimulateMacroPtr

// stub


:Arguments: * **name** : string

            * **someClassPtr** : auto

+++++++++++++++++++
Adapter application
+++++++++++++++++++

  *  :ref:`add_function_annotation (module: Module? implicit; annotation: smart_ptr\<FunctionAnnotation\>& implicit) <function-_at_ast_c__c_add_function_annotation_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_FunctionAnnotation_gr__gr__qm_W_C_c_C_l>` 
  *  :ref:`add_function_annotation (function: smart_ptr\<Function\> implicit; annotation: smart_ptr\<FunctionAnnotation\>& implicit) <function-_at_ast_c__c_add_function_annotation_CI1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M_&I1_ls_H_ls_ast_c__c_FunctionAnnotation_gr__gr__qm_W_C_c_C_l>` 
  *  :ref:`add_function_annotation (function: smart_ptr\<Function\> implicit; annotation: smart_ptr\<AnnotationDeclaration\>& implicit) <function-_at_ast_c__c_add_function_annotation_CI1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M_&I1_ls_H_ls_rtti_c__c_AnnotationDeclaration_gr__gr__qm_M_C_c_C_l>` 
  *  :ref:`add_block_annotation (block: smart_ptr\<ExprBlock\> implicit; annotation: smart_ptr\<FunctionAnnotation\>& implicit) <function-_at_ast_c__c_add_block_annotation_CI1_ls_H_ls_ast_c__c_ExprBlock_gr__gr__qm_M_&I1_ls_H_ls_ast_c__c_FunctionAnnotation_gr__gr__qm_W_C_c_C_l>` 
  *  :ref:`add_block_annotation (block: smart_ptr\<ExprBlock\> implicit; annotation: smart_ptr\<AnnotationDeclaration\>& implicit) <function-_at_ast_c__c_add_block_annotation_CI1_ls_H_ls_ast_c__c_ExprBlock_gr__gr__qm_M_&I1_ls_H_ls_rtti_c__c_AnnotationDeclaration_gr__gr__qm_M_C_c_C_l>` 
  *  :ref:`add_structure_annotation (module: Module? implicit; annotation: smart_ptr\<StructureAnnotation\>& implicit) <function-_at_ast_c__c_add_structure_annotation_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_StructureAnnotation_gr__gr__qm_W_C_c_C_l>` 
  *  :ref:`add_structure_annotation (structure: smart_ptr\<Structure\> implicit; annotation: smart_ptr\<StructureAnnotation\>& implicit) <function-_at_ast_c__c_add_structure_annotation_CI1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_M_&I1_ls_H_ls_ast_c__c_StructureAnnotation_gr__gr__qm_W_C_c_C_l>` 
  *  :ref:`add_structure_annotation (structure: smart_ptr\<Structure\> implicit; annotation: smart_ptr\<AnnotationDeclaration\>& implicit) <function-_at_ast_c__c_add_structure_annotation_CI1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_M_&I1_ls_H_ls_rtti_c__c_AnnotationDeclaration_gr__gr__qm_M_C_c_C_l>` 
  *  :ref:`add_enumeration_annotation (module: Module? implicit; annotation: smart_ptr\<EnumerationAnnotation\>& implicit) <function-_at_ast_c__c_add_enumeration_annotation_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_EnumerationAnnotation_gr__gr__qm_W_C_c_C_l>` 
  *  :ref:`add_infer_macro (module: Module? implicit; annotation: smart_ptr\<PassMacro\>& implicit) <function-_at_ast_c__c_add_infer_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_PassMacro_gr__gr__qm_W_C_c>` 
  *  :ref:`add_dirty_infer_macro (module: Module? implicit; annotation: smart_ptr\<PassMacro\>& implicit) <function-_at_ast_c__c_add_dirty_infer_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_PassMacro_gr__gr__qm_W_C_c>` 
  *  :ref:`add_lint_macro (module: Module? implicit; annotation: smart_ptr\<PassMacro\>& implicit) <function-_at_ast_c__c_add_lint_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_PassMacro_gr__gr__qm_W_C_c>` 
  *  :ref:`add_global_lint_macro (module: Module? implicit; annotation: smart_ptr\<PassMacro\>& implicit) <function-_at_ast_c__c_add_global_lint_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_PassMacro_gr__gr__qm_W_C_c>` 
  *  :ref:`add_optimization_macro (module: Module? implicit; annotation: smart_ptr\<PassMacro\>& implicit) <function-_at_ast_c__c_add_optimization_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_PassMacro_gr__gr__qm_W_C_c>` 
  *  :ref:`add_reader_macro (module: Module? implicit; annotation: smart_ptr\<ReaderMacro\>& implicit) <function-_at_ast_c__c_add_reader_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_ReaderMacro_gr__gr__qm_W_C_c_C_l>` 
  *  :ref:`add_comment_reader (module: Module? implicit; reader: smart_ptr\<CommentReader\>& implicit) <function-_at_ast_c__c_add_comment_reader_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_CommentReader_gr__gr__qm_W_C_c_C_l>` 
  *  :ref:`add_call_macro (module: Module? implicit; annotation: smart_ptr\<CallMacro\>& implicit) <function-_at_ast_c__c_add_call_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_CallMacro_gr__gr__qm_W_C_c_C_l>` 
  *  :ref:`add_typeinfo_macro (module: Module? implicit; annotation: smart_ptr\<TypeInfoMacro\>& implicit) <function-_at_ast_c__c_add_typeinfo_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_TypeInfoMacro_gr__gr__qm_W_C_c_C_l>` 
  *  :ref:`add_variant_macro (module: Module? implicit; annotation: smart_ptr\<VariantMacro\>& implicit) <function-_at_ast_c__c_add_variant_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_VariantMacro_gr__gr__qm_W_C_c>` 
  *  :ref:`add_for_loop_macro (module: Module? implicit; annotation: smart_ptr\<ForLoopMacro\>& implicit) <function-_at_ast_c__c_add_for_loop_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_ForLoopMacro_gr__gr__qm_W_C_c>` 
  *  :ref:`add_capture_macro (module: Module? implicit; annotation: smart_ptr\<CaptureMacro\>& implicit) <function-_at_ast_c__c_add_capture_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_CaptureMacro_gr__gr__qm_W_C_c>` 
  *  :ref:`add_type_macro (module: Module? implicit; annotation: smart_ptr\<TypeMacro\>& implicit) <function-_at_ast_c__c_add_type_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_TypeMacro_gr__gr__qm_W_C_c_C_l>` 
  *  :ref:`add_simulate_macro (module: Module? implicit; annotation: smart_ptr\<SimulateMacro\>& implicit) <function-_at_ast_c__c_add_simulate_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_SimulateMacro_gr__gr__qm_W_C_c>` 
  *  :ref:`add_module_option (module: Module? implicit; option: string implicit; type: Type) <function-_at_ast_c__c_add_module_option_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CIs_CE_ls_rtti_c__c_Type_gr__C_c_C_l>` 
  *  :ref:`add_new_block_annotation (name: string; someClassPtr: auto) : auto <function-_at_ast_c__c_add_new_block_annotation_Cs_C_dot_>` 
  *  :ref:`add_new_function_annotation (name: string; someClassPtr: auto) : auto <function-_at_ast_c__c_add_new_function_annotation_Cs_C_dot_>` 
  *  :ref:`add_new_contract_annotation (name: string; someClassPtr: auto) : auto <function-_at_ast_c__c_add_new_contract_annotation_Cs_C_dot_>` 
  *  :ref:`add_new_structure_annotation (name: string; someClassPtr: auto) : auto <function-_at_ast_c__c_add_new_structure_annotation_Cs_C_dot_>` 
  *  :ref:`add_new_enumeration_annotation (name: string; someClassPtr: auto) : auto <function-_at_ast_c__c_add_new_enumeration_annotation_Cs_C_dot_>` 
  *  :ref:`add_new_variant_macro (name: string; someClassPtr: auto) : auto <function-_at_ast_c__c_add_new_variant_macro_Cs_C_dot_>` 
  *  :ref:`add_new_for_loop_macro (name: string; someClassPtr: auto) : auto <function-_at_ast_c__c_add_new_for_loop_macro_Cs_C_dot_>` 
  *  :ref:`add_new_capture_macro (name: string; someClassPtr: auto) : auto <function-_at_ast_c__c_add_new_capture_macro_Cs_C_dot_>` 
  *  :ref:`add_new_type_macro (name: string; someClassPtr: auto) : auto <function-_at_ast_c__c_add_new_type_macro_Cs_C_dot_>` 
  *  :ref:`add_new_simulate_macro (name: string; someClassPtr: auto) : auto <function-_at_ast_c__c_add_new_simulate_macro_Cs_C_dot_>` 
  *  :ref:`add_new_reader_macro (name: string; someClassPtr: auto) : auto <function-_at_ast_c__c_add_new_reader_macro_Cs_C_dot_>` 
  *  :ref:`add_new_comment_reader (name: string; someClassPtr: auto) : auto <function-_at_ast_c__c_add_new_comment_reader_Cs_C_dot_>` 
  *  :ref:`add_new_call_macro (name: string; someClassPtr: auto) : auto <function-_at_ast_c__c_add_new_call_macro_Cs_C_dot_>` 
  *  :ref:`add_new_typeinfo_macro (name: string; someClassPtr: auto) : auto <function-_at_ast_c__c_add_new_typeinfo_macro_Cs_C_dot_>` 
  *  :ref:`add_new_infer_macro (name: string; someClassPtr: auto) : auto <function-_at_ast_c__c_add_new_infer_macro_Cs_C_dot_>` 
  *  :ref:`add_new_dirty_infer_macro (name: string; someClassPtr: auto) : auto <function-_at_ast_c__c_add_new_dirty_infer_macro_Cs_C_dot_>` 
  *  :ref:`add_new_lint_macro (name: string; someClassPtr: auto) : auto <function-_at_ast_c__c_add_new_lint_macro_Cs_C_dot_>` 
  *  :ref:`add_new_global_lint_macro (name: string; someClassPtr: auto) : auto <function-_at_ast_c__c_add_new_global_lint_macro_Cs_C_dot_>` 
  *  :ref:`add_new_optimization_macro (name: string; someClassPtr: auto) : auto <function-_at_ast_c__c_add_new_optimization_macro_Cs_C_dot_>` 

.. _function-_at_ast_c__c_add_function_annotation_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_FunctionAnnotation_gr__gr__qm_W_C_c_C_l:

.. das:function:: add_function_annotation(module: Module? implicit; annotation: smart_ptr<FunctionAnnotation>& implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **annotation** : smart_ptr< :ref:`FunctionAnnotation <handle-ast-FunctionAnnotation>` >& implicit

.. _function-_at_ast_c__c_add_function_annotation_CI1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M_&I1_ls_H_ls_ast_c__c_FunctionAnnotation_gr__gr__qm_W_C_c_C_l:

.. das:function:: add_function_annotation(function: smart_ptr<Function> implicit; annotation: smart_ptr<FunctionAnnotation>& implicit)

// stub


:Arguments: * **function** : smart_ptr< :ref:`Function <handle-ast-Function>` > implicit

            * **annotation** : smart_ptr< :ref:`FunctionAnnotation <handle-ast-FunctionAnnotation>` >& implicit

.. _function-_at_ast_c__c_add_function_annotation_CI1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M_&I1_ls_H_ls_rtti_c__c_AnnotationDeclaration_gr__gr__qm_M_C_c_C_l:

.. das:function:: add_function_annotation(function: smart_ptr<Function> implicit; annotation: smart_ptr<AnnotationDeclaration>& implicit)

// stub


:Arguments: * **function** : smart_ptr< :ref:`Function <handle-ast-Function>` > implicit

            * **annotation** : smart_ptr< :ref:`AnnotationDeclaration <handle-rtti-AnnotationDeclaration>` >& implicit

.. _function-_at_ast_c__c_add_block_annotation_CI1_ls_H_ls_ast_c__c_ExprBlock_gr__gr__qm_M_&I1_ls_H_ls_ast_c__c_FunctionAnnotation_gr__gr__qm_W_C_c_C_l:

.. das:function:: add_block_annotation(block: smart_ptr<ExprBlock> implicit; annotation: smart_ptr<FunctionAnnotation>& implicit)

// stub


:Arguments: * **block** : smart_ptr< :ref:`ExprBlock <handle-ast-ExprBlock>` > implicit

            * **annotation** : smart_ptr< :ref:`FunctionAnnotation <handle-ast-FunctionAnnotation>` >& implicit

.. _function-_at_ast_c__c_add_block_annotation_CI1_ls_H_ls_ast_c__c_ExprBlock_gr__gr__qm_M_&I1_ls_H_ls_rtti_c__c_AnnotationDeclaration_gr__gr__qm_M_C_c_C_l:

.. das:function:: add_block_annotation(block: smart_ptr<ExprBlock> implicit; annotation: smart_ptr<AnnotationDeclaration>& implicit)

// stub


:Arguments: * **block** : smart_ptr< :ref:`ExprBlock <handle-ast-ExprBlock>` > implicit

            * **annotation** : smart_ptr< :ref:`AnnotationDeclaration <handle-rtti-AnnotationDeclaration>` >& implicit

.. _function-_at_ast_c__c_add_structure_annotation_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_StructureAnnotation_gr__gr__qm_W_C_c_C_l:

.. das:function:: add_structure_annotation(module: Module? implicit; annotation: smart_ptr<StructureAnnotation>& implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **annotation** : smart_ptr< :ref:`StructureAnnotation <handle-ast-StructureAnnotation>` >& implicit

.. _function-_at_ast_c__c_add_structure_annotation_CI1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_M_&I1_ls_H_ls_ast_c__c_StructureAnnotation_gr__gr__qm_W_C_c_C_l:

.. das:function:: add_structure_annotation(structure: smart_ptr<Structure> implicit; annotation: smart_ptr<StructureAnnotation>& implicit)

// stub


:Arguments: * **structure** : smart_ptr< :ref:`Structure <handle-ast-Structure>` > implicit

            * **annotation** : smart_ptr< :ref:`StructureAnnotation <handle-ast-StructureAnnotation>` >& implicit

.. _function-_at_ast_c__c_add_structure_annotation_CI1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_M_&I1_ls_H_ls_rtti_c__c_AnnotationDeclaration_gr__gr__qm_M_C_c_C_l:

.. das:function:: add_structure_annotation(structure: smart_ptr<Structure> implicit; annotation: smart_ptr<AnnotationDeclaration>& implicit)

// stub


:Arguments: * **structure** : smart_ptr< :ref:`Structure <handle-ast-Structure>` > implicit

            * **annotation** : smart_ptr< :ref:`AnnotationDeclaration <handle-rtti-AnnotationDeclaration>` >& implicit

.. _function-_at_ast_c__c_add_enumeration_annotation_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_EnumerationAnnotation_gr__gr__qm_W_C_c_C_l:

.. das:function:: add_enumeration_annotation(module: Module? implicit; annotation: smart_ptr<EnumerationAnnotation>& implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **annotation** : smart_ptr< :ref:`EnumerationAnnotation <handle-ast-EnumerationAnnotation>` >& implicit

.. _function-_at_ast_c__c_add_infer_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_PassMacro_gr__gr__qm_W_C_c:

.. das:function:: add_infer_macro(module: Module? implicit; annotation: smart_ptr<PassMacro>& implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **annotation** : smart_ptr< :ref:`PassMacro <handle-ast-PassMacro>` >& implicit

.. _function-_at_ast_c__c_add_dirty_infer_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_PassMacro_gr__gr__qm_W_C_c:

.. das:function:: add_dirty_infer_macro(module: Module? implicit; annotation: smart_ptr<PassMacro>& implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **annotation** : smart_ptr< :ref:`PassMacro <handle-ast-PassMacro>` >& implicit

.. _function-_at_ast_c__c_add_lint_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_PassMacro_gr__gr__qm_W_C_c:

.. das:function:: add_lint_macro(module: Module? implicit; annotation: smart_ptr<PassMacro>& implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **annotation** : smart_ptr< :ref:`PassMacro <handle-ast-PassMacro>` >& implicit

.. _function-_at_ast_c__c_add_global_lint_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_PassMacro_gr__gr__qm_W_C_c:

.. das:function:: add_global_lint_macro(module: Module? implicit; annotation: smart_ptr<PassMacro>& implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **annotation** : smart_ptr< :ref:`PassMacro <handle-ast-PassMacro>` >& implicit

.. _function-_at_ast_c__c_add_optimization_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_PassMacro_gr__gr__qm_W_C_c:

.. das:function:: add_optimization_macro(module: Module? implicit; annotation: smart_ptr<PassMacro>& implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **annotation** : smart_ptr< :ref:`PassMacro <handle-ast-PassMacro>` >& implicit

.. _function-_at_ast_c__c_add_reader_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_ReaderMacro_gr__gr__qm_W_C_c_C_l:

.. das:function:: add_reader_macro(module: Module? implicit; annotation: smart_ptr<ReaderMacro>& implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **annotation** : smart_ptr< :ref:`ReaderMacro <handle-ast-ReaderMacro>` >& implicit

.. _function-_at_ast_c__c_add_comment_reader_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_CommentReader_gr__gr__qm_W_C_c_C_l:

.. das:function:: add_comment_reader(module: Module? implicit; reader: smart_ptr<CommentReader>& implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **reader** : smart_ptr< :ref:`CommentReader <handle-ast-CommentReader>` >& implicit

.. _function-_at_ast_c__c_add_call_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_CallMacro_gr__gr__qm_W_C_c_C_l:

.. das:function:: add_call_macro(module: Module? implicit; annotation: smart_ptr<CallMacro>& implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **annotation** : smart_ptr< :ref:`CallMacro <handle-ast-CallMacro>` >& implicit

.. _function-_at_ast_c__c_add_typeinfo_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_TypeInfoMacro_gr__gr__qm_W_C_c_C_l:

.. das:function:: add_typeinfo_macro(module: Module? implicit; annotation: smart_ptr<TypeInfoMacro>& implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **annotation** : smart_ptr< :ref:`TypeInfoMacro <handle-ast-TypeInfoMacro>` >& implicit

.. _function-_at_ast_c__c_add_variant_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_VariantMacro_gr__gr__qm_W_C_c:

.. das:function:: add_variant_macro(module: Module? implicit; annotation: smart_ptr<VariantMacro>& implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **annotation** : smart_ptr< :ref:`VariantMacro <handle-ast-VariantMacro>` >& implicit

.. _function-_at_ast_c__c_add_for_loop_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_ForLoopMacro_gr__gr__qm_W_C_c:

.. das:function:: add_for_loop_macro(module: Module? implicit; annotation: smart_ptr<ForLoopMacro>& implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **annotation** : smart_ptr< :ref:`ForLoopMacro <handle-ast-ForLoopMacro>` >& implicit

.. _function-_at_ast_c__c_add_capture_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_CaptureMacro_gr__gr__qm_W_C_c:

.. das:function:: add_capture_macro(module: Module? implicit; annotation: smart_ptr<CaptureMacro>& implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **annotation** : smart_ptr< :ref:`CaptureMacro <handle-ast-CaptureMacro>` >& implicit

.. _function-_at_ast_c__c_add_type_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_TypeMacro_gr__gr__qm_W_C_c_C_l:

.. das:function:: add_type_macro(module: Module? implicit; annotation: smart_ptr<TypeMacro>& implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **annotation** : smart_ptr< :ref:`TypeMacro <handle-ast-TypeMacro>` >& implicit

.. _function-_at_ast_c__c_add_simulate_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_SimulateMacro_gr__gr__qm_W_C_c:

.. das:function:: add_simulate_macro(module: Module? implicit; annotation: smart_ptr<SimulateMacro>& implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **annotation** : smart_ptr< :ref:`SimulateMacro <handle-ast-SimulateMacro>` >& implicit

.. _function-_at_ast_c__c_add_module_option_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CIs_CE_ls_rtti_c__c_Type_gr__C_c_C_l:

.. das:function:: add_module_option(module: Module? implicit; option: string implicit; type: Type)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **option** : string implicit

            * **type** :  :ref:`Type <enum-rtti-Type>` 

.. _function-_at_ast_c__c_add_new_block_annotation_Cs_C_dot_:

.. das:function:: add_new_block_annotation(name: string; someClassPtr: auto) : auto

// stub


:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-_at_ast_c__c_add_new_function_annotation_Cs_C_dot_:

.. das:function:: add_new_function_annotation(name: string; someClassPtr: auto) : auto

// stub


:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-_at_ast_c__c_add_new_contract_annotation_Cs_C_dot_:

.. das:function:: add_new_contract_annotation(name: string; someClassPtr: auto) : auto

// stub


:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-_at_ast_c__c_add_new_structure_annotation_Cs_C_dot_:

.. das:function:: add_new_structure_annotation(name: string; someClassPtr: auto) : auto

// stub


:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-_at_ast_c__c_add_new_enumeration_annotation_Cs_C_dot_:

.. das:function:: add_new_enumeration_annotation(name: string; someClassPtr: auto) : auto

// stub


:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-_at_ast_c__c_add_new_variant_macro_Cs_C_dot_:

.. das:function:: add_new_variant_macro(name: string; someClassPtr: auto) : auto

// stub


:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-_at_ast_c__c_add_new_for_loop_macro_Cs_C_dot_:

.. das:function:: add_new_for_loop_macro(name: string; someClassPtr: auto) : auto

// stub


:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-_at_ast_c__c_add_new_capture_macro_Cs_C_dot_:

.. das:function:: add_new_capture_macro(name: string; someClassPtr: auto) : auto

// stub


:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-_at_ast_c__c_add_new_type_macro_Cs_C_dot_:

.. das:function:: add_new_type_macro(name: string; someClassPtr: auto) : auto

// stub


:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-_at_ast_c__c_add_new_simulate_macro_Cs_C_dot_:

.. das:function:: add_new_simulate_macro(name: string; someClassPtr: auto) : auto

// stub


:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-_at_ast_c__c_add_new_reader_macro_Cs_C_dot_:

.. das:function:: add_new_reader_macro(name: string; someClassPtr: auto) : auto

// stub


:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-_at_ast_c__c_add_new_comment_reader_Cs_C_dot_:

.. das:function:: add_new_comment_reader(name: string; someClassPtr: auto) : auto

// stub


:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-_at_ast_c__c_add_new_call_macro_Cs_C_dot_:

.. das:function:: add_new_call_macro(name: string; someClassPtr: auto) : auto

// stub


:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-_at_ast_c__c_add_new_typeinfo_macro_Cs_C_dot_:

.. das:function:: add_new_typeinfo_macro(name: string; someClassPtr: auto) : auto

// stub


:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-_at_ast_c__c_add_new_infer_macro_Cs_C_dot_:

.. das:function:: add_new_infer_macro(name: string; someClassPtr: auto) : auto

// stub


:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-_at_ast_c__c_add_new_dirty_infer_macro_Cs_C_dot_:

.. das:function:: add_new_dirty_infer_macro(name: string; someClassPtr: auto) : auto

// stub


:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-_at_ast_c__c_add_new_lint_macro_Cs_C_dot_:

.. das:function:: add_new_lint_macro(name: string; someClassPtr: auto) : auto

// stub


:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-_at_ast_c__c_add_new_global_lint_macro_Cs_C_dot_:

.. das:function:: add_new_global_lint_macro(name: string; someClassPtr: auto) : auto

// stub


:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-_at_ast_c__c_add_new_optimization_macro_Cs_C_dot_:

.. das:function:: add_new_optimization_macro(name: string; someClassPtr: auto) : auto

// stub


:Arguments: * **name** : string

            * **someClassPtr** : auto

+++++++++++++++++++++++++
Adding objects to objects
+++++++++++++++++++++++++

  *  :ref:`add_enumeration_entry (enum: smart_ptr\<Enumeration\> implicit; name: string implicit) : int <function-_at_ast_c__c_add_enumeration_entry_CI1_ls_H_ls_ast_c__c_Enumeration_gr__gr__qm_W_CIs>` 
  *  :ref:`add_function (module: Module? implicit; function: smart_ptr\<Function\>& implicit) : bool <function-_at_ast_c__c_add_function_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_Function_gr__gr__qm_W_C_c_C_l>` 
  *  :ref:`add_generic (module: Module? implicit; function: smart_ptr\<Function\>& implicit) : bool <function-_at_ast_c__c_add_generic_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_Function_gr__gr__qm_W_C_c_C_l>` 
  *  :ref:`add_variable (module: Module? implicit; variable: smart_ptr\<Variable\>& implicit) : bool <function-_at_ast_c__c_add_variable_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_Variable_gr__gr__qm_W_C_c_C_l>` 
  *  :ref:`add_keyword (module: Module? implicit; keyword: string implicit; needOxfordComma: bool) : bool <function-_at_ast_c__c_add_keyword_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CIs_Cb_C_c_C_l>` 
  *  :ref:`add_type_function (module: Module? implicit; keyword: string implicit) : bool <function-_at_ast_c__c_add_type_function_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CIs_C_c_C_l>` 
  *  :ref:`add_structure (module: Module? implicit; structure: smart_ptr\<Structure\>& implicit) : bool <function-_at_ast_c__c_add_structure_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_W>` 
  *  :ref:`add_alias (module: Module? implicit; structure: smart_ptr\<TypeDecl\>& implicit) : bool <function-_at_ast_c__c_add_alias_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_W>` 
  *  :ref:`add_module_require (module: Module? implicit; publicModule: Module? implicit; pub: bool) : bool <function-_at_ast_c__c_add_module_require_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__Cb>` 

.. _function-_at_ast_c__c_add_enumeration_entry_CI1_ls_H_ls_ast_c__c_Enumeration_gr__gr__qm_W_CIs:

.. das:function:: add_enumeration_entry(enum: smart_ptr<Enumeration> implicit; name: string implicit) : int

// stub


:Arguments: * **enum** : smart_ptr< :ref:`Enumeration <handle-ast-Enumeration>` > implicit

            * **name** : string implicit

.. _function-_at_ast_c__c_add_function_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_Function_gr__gr__qm_W_C_c_C_l:

.. das:function:: add_function(module: Module? implicit; function: smart_ptr<Function>& implicit) : bool

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **function** : smart_ptr< :ref:`Function <handle-ast-Function>` >& implicit

.. _function-_at_ast_c__c_add_generic_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_Function_gr__gr__qm_W_C_c_C_l:

.. das:function:: add_generic(module: Module? implicit; function: smart_ptr<Function>& implicit) : bool

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **function** : smart_ptr< :ref:`Function <handle-ast-Function>` >& implicit

.. _function-_at_ast_c__c_add_variable_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_Variable_gr__gr__qm_W_C_c_C_l:

.. das:function:: add_variable(module: Module? implicit; variable: smart_ptr<Variable>& implicit) : bool

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **variable** : smart_ptr< :ref:`Variable <handle-ast-Variable>` >& implicit

.. _function-_at_ast_c__c_add_keyword_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CIs_Cb_C_c_C_l:

.. das:function:: add_keyword(module: Module? implicit; keyword: string implicit; needOxfordComma: bool) : bool

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **keyword** : string implicit

            * **needOxfordComma** : bool

.. _function-_at_ast_c__c_add_type_function_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CIs_C_c_C_l:

.. das:function:: add_type_function(module: Module? implicit; keyword: string implicit) : bool

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **keyword** : string implicit

.. _function-_at_ast_c__c_add_structure_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_W:

.. das:function:: add_structure(module: Module? implicit; structure: smart_ptr<Structure>& implicit) : bool

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **structure** : smart_ptr< :ref:`Structure <handle-ast-Structure>` >& implicit

.. _function-_at_ast_c__c_add_alias_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_W:

.. das:function:: add_alias(module: Module? implicit; structure: smart_ptr<TypeDecl>& implicit) : bool

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **structure** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >& implicit

.. _function-_at_ast_c__c_add_module_require_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__Cb:

.. das:function:: add_module_require(module: Module? implicit; publicModule: Module? implicit; pub: bool) : bool

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **publicModule** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **pub** : bool

+++++++++++++++++++++++++
Program and module access
+++++++++++++++++++++++++

  *  :ref:`this_program () : smart_ptr\<Program\> <function-_at_ast_c__c_this_program_C_c>` 
  *  :ref:`this_module () : Module? <function-_at_ast_c__c_this_module_C_c_C_l>` 
  *  :ref:`compiling_program () : smart_ptr\<Program\> <function-_at_ast_c__c_compiling_program_C_c_C_l>` 
  *  :ref:`compiling_module () : Module? <function-_at_ast_c__c_compiling_module_C_c_C_l>` 

.. _function-_at_ast_c__c_this_program_C_c:

.. das:function:: this_program() : smart_ptr<Program>

// stub


.. _function-_at_ast_c__c_this_module_C_c_C_l:

.. das:function:: this_module() : Module?

// stub


.. _function-_at_ast_c__c_compiling_program_C_c_C_l:

.. das:function:: compiling_program() : smart_ptr<Program>

// stub


.. _function-_at_ast_c__c_compiling_module_C_c_C_l:

.. das:function:: compiling_module() : Module?

// stub


+++++++++++++++++++++++++++++++++++
Textual descriptions of the objects
+++++++++++++++++++++++++++++++++++

  *  :ref:`describe_typedecl (type: smart_ptr\<TypeDecl\> implicit; extra: bool; contracts: bool; module: bool) : string <function-_at_ast_c__c_describe_typedecl_CI1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_Cb_Cb_Cb_C_c_C_l>` 
  *  :ref:`describe_typedecl_cpp (type: smart_ptr\<TypeDecl\> implicit; substitueRef: bool; skipRef: bool; skipConst: bool; redundantConst: bool) : string <function-_at_ast_c__c_describe_typedecl_cpp_CI1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_Cb_Cb_Cb_Cb_C_c_C_l>` 
  *  :ref:`describe_expression (expression: smart_ptr\<Expression\> implicit) : string <function-_at_ast_c__c_describe_expression_CI1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M_C_c_C_l>` 
  *  :ref:`describe_function (function: smart_ptr\<Function\> implicit) : string <function-_at_ast_c__c_describe_function_CI1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M_C_c_C_l>` 
  *  :ref:`das_to_string (type: Type) : string <function-_at_ast_c__c_das_to_string_CE_ls_rtti_c__c_Type_gr__C_c_C_l>` 
  *  :ref:`describe (decl: smart_ptr\<TypeDecl\>; extra: bool = true; contracts: bool = true; modules: bool = true) : auto <function-_at_ast_c__c_describe_C1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_Cb_Cb_Cb>` 
  *  :ref:`describe_cpp (decl: smart_ptr\<TypeDecl\>; substitureRef: bool = false; skipRef: bool = false; skipConst: bool = false; redundantConst: bool = true) : auto <function-_at_ast_c__c_describe_cpp_C1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_Cb_Cb_Cb_Cb>` 
  *  :ref:`describe (expr: smart_ptr\<Expression\>) : auto <function-_at_ast_c__c_describe_C1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M>` 
  *  :ref:`describe (expr: smart_ptr\<Function\>) : auto <function-_at_ast_c__c_describe_C1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M>` 

.. _function-_at_ast_c__c_describe_typedecl_CI1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_Cb_Cb_Cb_C_c_C_l:

.. das:function:: describe_typedecl(type: smart_ptr<TypeDecl> implicit; extra: bool; contracts: bool; module: bool) : string

// stub


:Arguments: * **type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

            * **extra** : bool

            * **contracts** : bool

            * **module** : bool

.. _function-_at_ast_c__c_describe_typedecl_cpp_CI1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_Cb_Cb_Cb_Cb_C_c_C_l:

.. das:function:: describe_typedecl_cpp(type: smart_ptr<TypeDecl> implicit; substitueRef: bool; skipRef: bool; skipConst: bool; redundantConst: bool) : string

// stub


:Arguments: * **type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

            * **substitueRef** : bool

            * **skipRef** : bool

            * **skipConst** : bool

            * **redundantConst** : bool

.. _function-_at_ast_c__c_describe_expression_CI1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M_C_c_C_l:

.. das:function:: describe_expression(expression: smart_ptr<Expression> implicit) : string

// stub


:Arguments: * **expression** : smart_ptr< :ref:`Expression <handle-ast-Expression>` > implicit

.. _function-_at_ast_c__c_describe_function_CI1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M_C_c_C_l:

.. das:function:: describe_function(function: smart_ptr<Function> implicit) : string

// stub


:Arguments: * **function** : smart_ptr< :ref:`Function <handle-ast-Function>` > implicit

.. _function-_at_ast_c__c_das_to_string_CE_ls_rtti_c__c_Type_gr__C_c_C_l:

.. das:function:: das_to_string(type: Type) : string

// stub


:Arguments: * **type** :  :ref:`Type <enum-rtti-Type>` 

.. _function-_at_ast_c__c_describe_C1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_Cb_Cb_Cb:

.. das:function:: describe(decl: smart_ptr<TypeDecl>; extra: bool = true; contracts: bool = true; modules: bool = true) : auto

// stub


:Arguments: * **decl** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

            * **extra** : bool

            * **contracts** : bool

            * **modules** : bool

.. _function-_at_ast_c__c_describe_cpp_C1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_Cb_Cb_Cb_Cb:

.. das:function:: describe_cpp(decl: smart_ptr<TypeDecl>; substitureRef: bool = false; skipRef: bool = false; skipConst: bool = false; redundantConst: bool = true) : auto

// stub


:Arguments: * **decl** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

            * **substitureRef** : bool

            * **skipRef** : bool

            * **skipConst** : bool

            * **redundantConst** : bool

.. _function-_at_ast_c__c_describe_C1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M:

.. das:function:: describe(expr: smart_ptr<Expression>) : auto

// stub


:Arguments: * **expr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

.. _function-_at_ast_c__c_describe_C1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M:

.. das:function:: describe(expr: smart_ptr<Function>) : auto

// stub


:Arguments: * **expr** : smart_ptr< :ref:`Function <handle-ast-Function>` >

+++++++++
Searching
+++++++++

  *  :ref:`find_module_via_rtti (program: smart_ptr\<Program\> implicit; name: string implicit) : Module? <function-_at_ast_c__c_find_module_via_rtti_CI1_ls_H_ls_rtti_c__c_Program_gr__gr__qm_W_CIs_C_c_C_l>` 
  *  :ref:`find_module_function_via_rtti (module: Module? implicit; function: function\<\>) : smart_ptr\<Function\> <function-_at_ast_c__c_find_module_function_via_rtti_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__C_at__at__C_c_C_l>` 
  *  :ref:`find_variable (module: Module? implicit; variable: string implicit) : smart_ptr\<Variable\> <function-_at_ast_c__c_find_variable_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CIs>` 
  *  :ref:`find_matching_variable (program: Program? implicit; function: Function? implicit; name: string implicit; seePrivate: bool; block: block\<(var arg0:array\<smart_ptr\<Variable\>\>#):void\> implicit) <function-_at_ast_c__c_find_matching_variable_CI1_ls_H_ls_rtti_c__c_Program_gr__gr__qm__CI1_ls_H_ls_ast_c__c_Function_gr__gr__qm__CIs_Cb_CI0_ls__hh_1_ls_1_ls_H_ls_ast_c__c_Variable_gr__gr__qm_W_gr_A_gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`find_bitfield_name (bit: smart_ptr\<TypeDecl\> implicit; value: bitfield) : string <function-_at_ast_c__c_find_bitfield_name_CI1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_Ct_C_c_C_l>` 
  *  :ref:`find_enum_value (enum: smart_ptr\<Enumeration\> implicit; value: string implicit) : int64 <function-_at_ast_c__c_find_enum_value_CI1_ls_H_ls_ast_c__c_Enumeration_gr__gr__qm_W_CIs>` 
  *  :ref:`find_enum_value (enum: Enumeration? implicit; value: string implicit) : int64 <function-_at_ast_c__c_find_enum_value_CI1_ls_H_ls_ast_c__c_Enumeration_gr__gr__qm__CIs>` 
  *  :ref:`find_structure_field (structPtr: Structure? implicit; field: string implicit) : FieldDeclaration? <function-_at_ast_c__c_find_structure_field_CI1_ls_H_ls_ast_c__c_Structure_gr__gr__qm__CIs_C_c_C_l>` 
  *  :ref:`find_unique_structure (program: smart_ptr\<Program\> implicit; name: string implicit) : Structure? <function-_at_ast_c__c_find_unique_structure_CI1_ls_H_ls_rtti_c__c_Program_gr__gr__qm_M_CIs_C_c_C_l>` 
  *  :ref:`find_module (prog: smart_ptr\<Program\>; name: string) : Module? <function-_at_ast_c__c_find_module_C1_ls_H_ls_rtti_c__c_Program_gr__gr__qm_M_Cs>` 
  *  :ref:`find_module (name: string) : Module? <function-_at_ast_c__c_find_module_Cs>` 
  *  :ref:`find_compiling_module (name: string) : Module? <function-_at_ast_c__c_find_compiling_module_Cs>` 

.. _function-_at_ast_c__c_find_module_via_rtti_CI1_ls_H_ls_rtti_c__c_Program_gr__gr__qm_W_CIs_C_c_C_l:

.. das:function:: find_module_via_rtti(program: smart_ptr<Program> implicit; name: string implicit) : Module?

// stub


:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>` > implicit

            * **name** : string implicit

.. _function-_at_ast_c__c_find_module_function_via_rtti_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__C_at__at__C_c_C_l:

.. das:function:: find_module_function_via_rtti(module: Module? implicit; function: function<>) : smart_ptr<Function>

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **function** : function<void>

.. _function-_at_ast_c__c_find_variable_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CIs:

.. das:function:: find_variable(module: Module? implicit; variable: string implicit) : smart_ptr<Variable>

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **variable** : string implicit

.. _function-_at_ast_c__c_find_matching_variable_CI1_ls_H_ls_rtti_c__c_Program_gr__gr__qm__CI1_ls_H_ls_ast_c__c_Function_gr__gr__qm__CIs_Cb_CI0_ls__hh_1_ls_1_ls_H_ls_ast_c__c_Variable_gr__gr__qm_W_gr_A_gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: find_matching_variable(program: Program? implicit; function: Function? implicit; name: string implicit; seePrivate: bool; block: block<(var arg0:array<smart_ptr<Variable>>#):void> implicit)

// stub


:Arguments: * **program** :  :ref:`Program <handle-rtti-Program>` ? implicit

            * **function** :  :ref:`Function <handle-ast-Function>` ? implicit

            * **name** : string implicit

            * **seePrivate** : bool

            * **block** : block<(array<smart_ptr< :ref:`Variable <handle-ast-Variable>` >>#):void> implicit

.. _function-_at_ast_c__c_find_bitfield_name_CI1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_Ct_C_c_C_l:

.. das:function:: find_bitfield_name(bit: smart_ptr<TypeDecl> implicit; value: bitfield) : string

// stub


:Arguments: * **bit** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

            * **value** : bitfield<>

.. _function-_at_ast_c__c_find_enum_value_CI1_ls_H_ls_ast_c__c_Enumeration_gr__gr__qm_W_CIs:

.. das:function:: find_enum_value(enum: smart_ptr<Enumeration> implicit; value: string implicit) : int64

// stub


:Arguments: * **enum** : smart_ptr< :ref:`Enumeration <handle-ast-Enumeration>` > implicit

            * **value** : string implicit

.. _function-_at_ast_c__c_find_enum_value_CI1_ls_H_ls_ast_c__c_Enumeration_gr__gr__qm__CIs:

.. das:function:: find_enum_value(enum: Enumeration? implicit; value: string implicit) : int64

// stub


:Arguments: * **enum** :  :ref:`Enumeration <handle-ast-Enumeration>` ? implicit

            * **value** : string implicit

.. _function-_at_ast_c__c_find_structure_field_CI1_ls_H_ls_ast_c__c_Structure_gr__gr__qm__CIs_C_c_C_l:

.. das:function:: find_structure_field(structPtr: Structure? implicit; field: string implicit) : FieldDeclaration?

// stub


:Arguments: * **structPtr** :  :ref:`Structure <handle-ast-Structure>` ? implicit

            * **field** : string implicit

.. _function-_at_ast_c__c_find_unique_structure_CI1_ls_H_ls_rtti_c__c_Program_gr__gr__qm_M_CIs_C_c_C_l:

.. das:function:: find_unique_structure(program: smart_ptr<Program> implicit; name: string implicit) : Structure?

// stub


:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>` > implicit

            * **name** : string implicit

.. _function-_at_ast_c__c_find_module_C1_ls_H_ls_rtti_c__c_Program_gr__gr__qm_M_Cs:

.. das:function:: find_module(prog: smart_ptr<Program>; name: string) : Module?

// stub


:Arguments: * **prog** : smart_ptr< :ref:`Program <handle-rtti-Program>` >

            * **name** : string

.. _function-_at_ast_c__c_find_module_Cs:

.. das:function:: find_module(name: string) : Module?

// stub


:Arguments: * **name** : string

.. _function-_at_ast_c__c_find_compiling_module_Cs:

.. das:function:: find_compiling_module(name: string) : Module?

// stub


:Arguments: * **name** : string

+++++++++
Iterating
+++++++++

  *  :ref:`for_each_module (program: Program? implicit; block: block\<(var arg0:Module?):void\> implicit) <function-_at_ast_c__c_for_each_module_CI1_ls_H_ls_rtti_c__c_Program_gr__gr__qm__CI0_ls_1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`for_each_function (module: Module? implicit; name: string implicit; block: block\<(var arg0:smart_ptr\<Function\>):void\> implicit) <function-_at_ast_c__c_for_each_function_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CIs_CI0_ls_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_W_gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`for_each_generic (module: Module? implicit; name: string implicit; block: block\<(var arg0:smart_ptr\<Function\>):void\> implicit) <function-_at_ast_c__c_for_each_generic_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CIs_CI0_ls_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_W_gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`any_table_foreach (table: void? implicit; keyStride: int; valueStride: int; block: block\<(var arg0:void?;var arg1:void?):void\> implicit) <function-_at_ast_c__c_any_table_foreach_CI_qm__Ci_Ci_CI0_ls__qm_;_qm__gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`any_array_foreach (array: void? implicit; stride: int; block: block\<(var arg0:void?):void\> implicit) <function-_at_ast_c__c_any_array_foreach_CI_qm__Ci_CI0_ls__qm__gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`for_each_typedef (module: Module? implicit; block: block\<(var arg0:string#;var arg1:smart_ptr\<TypeDecl\>):void\> implicit) <function-_at_ast_c__c_for_each_typedef_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI0_ls__hh_s;1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_W_gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`for_each_enumeration (module: Module? implicit; block: block\<(var arg0:smart_ptr\<Enumeration\>):void\> implicit) <function-_at_ast_c__c_for_each_enumeration_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI0_ls_1_ls_H_ls_ast_c__c_Enumeration_gr__gr__qm_W_gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`for_each_structure (module: Module? implicit; block: block\<(var arg0:smart_ptr\<Structure\>):void\> implicit) <function-_at_ast_c__c_for_each_structure_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI0_ls_1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_W_gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`for_each_generic (module: Module? implicit; block: block\<(var arg0:smart_ptr\<Function\>):void\> implicit) <function-_at_ast_c__c_for_each_generic_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI0_ls_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_W_gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`for_each_global (module: Module? implicit; block: block\<(var arg0:smart_ptr\<Variable\>):void\> implicit) <function-_at_ast_c__c_for_each_global_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI0_ls_1_ls_H_ls_ast_c__c_Variable_gr__gr__qm_W_gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`for_each_call_macro (module: Module? implicit; block: block\<(var arg0:string#):void\> implicit) <function-_at_ast_c__c_for_each_call_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI0_ls__hh_s_gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`for_each_reader_macro (module: Module? implicit; block: block\<(var arg0:string#):void\> implicit) <function-_at_ast_c__c_for_each_reader_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI0_ls__hh_s_gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`for_each_variant_macro (module: Module? implicit; block: block\<(var arg0:smart_ptr\<VariantMacro\>):void\> implicit) <function-_at_ast_c__c_for_each_variant_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI0_ls_1_ls_H_ls_ast_c__c_VariantMacro_gr__gr__qm_W_gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`for_each_for_loop_macro (module: Module? implicit; block: block\<(var arg0:smart_ptr\<ForLoopMacro\>):void\> implicit) <function-_at_ast_c__c_for_each_for_loop_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI0_ls_1_ls_H_ls_ast_c__c_ForLoopMacro_gr__gr__qm_W_gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`for_each_typeinfo_macro (module: Module? implicit; block: block\<(var arg0:smart_ptr\<TypeInfoMacro\>):void\> implicit) <function-_at_ast_c__c_for_each_typeinfo_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI0_ls_1_ls_H_ls_ast_c__c_TypeInfoMacro_gr__gr__qm_W_gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`for_each_field (annotation: BasicStructureAnnotation implicit; block: block\<(var arg0:string;var arg1:string;var arg2:smart_ptr\<TypeDecl\>;var arg3:uint):void\> implicit) <function-_at_ast_c__c_for_each_field_CIH_ls_rtti_c__c_BasicStructureAnnotation_gr__CI0_ls_s;s;1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_W;u_gr_1_ls_v_gr__builtin__C_c_C_l>` 

.. _function-_at_ast_c__c_for_each_module_CI1_ls_H_ls_rtti_c__c_Program_gr__gr__qm__CI0_ls_1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: for_each_module(program: Program? implicit; block: block<(var arg0:Module?):void> implicit)

// stub


:Arguments: * **program** :  :ref:`Program <handle-rtti-Program>` ? implicit

            * **block** : block<( :ref:`Module <handle-rtti-Module>` ?):void> implicit

.. _function-_at_ast_c__c_for_each_function_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CIs_CI0_ls_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_W_gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: for_each_function(module: Module? implicit; name: string implicit; block: block<(var arg0:smart_ptr<Function>):void> implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **name** : string implicit

            * **block** : block<(smart_ptr< :ref:`Function <handle-ast-Function>` >):void> implicit

.. _function-_at_ast_c__c_for_each_generic_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CIs_CI0_ls_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_W_gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: for_each_generic(module: Module? implicit; name: string implicit; block: block<(var arg0:smart_ptr<Function>):void> implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **name** : string implicit

            * **block** : block<(smart_ptr< :ref:`Function <handle-ast-Function>` >):void> implicit

.. _function-_at_ast_c__c_any_table_foreach_CI_qm__Ci_Ci_CI0_ls__qm_;_qm__gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: any_table_foreach(table: void? implicit; keyStride: int; valueStride: int; block: block<(var arg0:void?;var arg1:void?):void> implicit)

// stub


:Arguments: * **table** : void? implicit

            * **keyStride** : int

            * **valueStride** : int

            * **block** : block<(void?;void?):void> implicit

.. _function-_at_ast_c__c_any_array_foreach_CI_qm__Ci_CI0_ls__qm__gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: any_array_foreach(array: void? implicit; stride: int; block: block<(var arg0:void?):void> implicit)

// stub


:Arguments: * **array** : void? implicit

            * **stride** : int

            * **block** : block<(void?):void> implicit

.. _function-_at_ast_c__c_for_each_typedef_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI0_ls__hh_s;1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_W_gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: for_each_typedef(module: Module? implicit; block: block<(var arg0:string#;var arg1:smart_ptr<TypeDecl>):void> implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **block** : block<(string#;smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >):void> implicit

.. _function-_at_ast_c__c_for_each_enumeration_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI0_ls_1_ls_H_ls_ast_c__c_Enumeration_gr__gr__qm_W_gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: for_each_enumeration(module: Module? implicit; block: block<(var arg0:smart_ptr<Enumeration>):void> implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **block** : block<(smart_ptr< :ref:`Enumeration <handle-ast-Enumeration>` >):void> implicit

.. _function-_at_ast_c__c_for_each_structure_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI0_ls_1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_W_gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: for_each_structure(module: Module? implicit; block: block<(var arg0:smart_ptr<Structure>):void> implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **block** : block<(smart_ptr< :ref:`Structure <handle-ast-Structure>` >):void> implicit

.. _function-_at_ast_c__c_for_each_generic_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI0_ls_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_W_gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: for_each_generic(module: Module? implicit; block: block<(var arg0:smart_ptr<Function>):void> implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **block** : block<(smart_ptr< :ref:`Function <handle-ast-Function>` >):void> implicit

.. _function-_at_ast_c__c_for_each_global_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI0_ls_1_ls_H_ls_ast_c__c_Variable_gr__gr__qm_W_gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: for_each_global(module: Module? implicit; block: block<(var arg0:smart_ptr<Variable>):void> implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **block** : block<(smart_ptr< :ref:`Variable <handle-ast-Variable>` >):void> implicit

.. _function-_at_ast_c__c_for_each_call_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI0_ls__hh_s_gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: for_each_call_macro(module: Module? implicit; block: block<(var arg0:string#):void> implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **block** : block<(string#):void> implicit

.. _function-_at_ast_c__c_for_each_reader_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI0_ls__hh_s_gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: for_each_reader_macro(module: Module? implicit; block: block<(var arg0:string#):void> implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **block** : block<(string#):void> implicit

.. _function-_at_ast_c__c_for_each_variant_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI0_ls_1_ls_H_ls_ast_c__c_VariantMacro_gr__gr__qm_W_gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: for_each_variant_macro(module: Module? implicit; block: block<(var arg0:smart_ptr<VariantMacro>):void> implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **block** : block<(smart_ptr< :ref:`VariantMacro <handle-ast-VariantMacro>` >):void> implicit

.. _function-_at_ast_c__c_for_each_for_loop_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI0_ls_1_ls_H_ls_ast_c__c_ForLoopMacro_gr__gr__qm_W_gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: for_each_for_loop_macro(module: Module? implicit; block: block<(var arg0:smart_ptr<ForLoopMacro>):void> implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **block** : block<(smart_ptr< :ref:`ForLoopMacro <handle-ast-ForLoopMacro>` >):void> implicit

.. _function-_at_ast_c__c_for_each_typeinfo_macro_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI0_ls_1_ls_H_ls_ast_c__c_TypeInfoMacro_gr__gr__qm_W_gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: for_each_typeinfo_macro(module: Module? implicit; block: block<(var arg0:smart_ptr<TypeInfoMacro>):void> implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **block** : block<(smart_ptr< :ref:`TypeInfoMacro <handle-ast-TypeInfoMacro>` >):void> implicit

.. _function-_at_ast_c__c_for_each_field_CIH_ls_rtti_c__c_BasicStructureAnnotation_gr__CI0_ls_s;s;1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_W;u_gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: for_each_field(annotation: BasicStructureAnnotation implicit; block: block<(var arg0:string;var arg1:string;var arg2:smart_ptr<TypeDecl>;var arg3:uint):void> implicit)

// stub


:Arguments: * **annotation** :  :ref:`BasicStructureAnnotation <handle-rtti-BasicStructureAnnotation>`  implicit

            * **block** : block<(string;string;smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >;uint):void> implicit

+++++++
Cloning
+++++++

  *  :ref:`clone_structure (structure: Structure const? implicit) : smart_ptr\<Structure\> <function-_at_ast_c__c_clone_structure_CI1_ls_CH_ls_ast_c__c_Structure_gr__gr__qm_>` 
  *  :ref:`clone_expression (expression: smart_ptr\<Expression\> implicit) : smart_ptr\<Expression\> <function-_at_ast_c__c_clone_expression_CI1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_W>` 
  *  :ref:`clone_function (function: smart_ptr\<Function\> implicit) : smart_ptr\<Function\> <function-_at_ast_c__c_clone_function_CI1_ls_H_ls_ast_c__c_Function_gr__gr__qm_W>` 
  *  :ref:`clone_variable (variable: smart_ptr\<Variable\> implicit) : smart_ptr\<Variable\> <function-_at_ast_c__c_clone_variable_CI1_ls_H_ls_ast_c__c_Variable_gr__gr__qm_W>` 
  *  :ref:`clone_type (type: smart_ptr\<TypeDecl\> implicit) : smart_ptr\<TypeDecl\> <function-_at_ast_c__c_clone_type_CI1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_W>` 
  *  :ref:`clone_function (fn: Function?) : FunctionPtr <function-_at_ast_c__c_clone_function_C1_ls_H_ls_ast_c__c_Function_gr__gr__qm_>` 

.. _function-_at_ast_c__c_clone_structure_CI1_ls_CH_ls_ast_c__c_Structure_gr__gr__qm_:

.. das:function:: clone_structure(structure: Structure const? implicit) : smart_ptr<Structure>

// stub


:Arguments: * **structure** :  :ref:`Structure <handle-ast-Structure>` ? implicit

.. _function-_at_ast_c__c_clone_expression_CI1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_W:

.. das:function:: clone_expression(expression: smart_ptr<Expression> implicit) : smart_ptr<Expression>

// stub


:Arguments: * **expression** : smart_ptr< :ref:`Expression <handle-ast-Expression>` > implicit

.. _function-_at_ast_c__c_clone_function_CI1_ls_H_ls_ast_c__c_Function_gr__gr__qm_W:

.. das:function:: clone_function(function: smart_ptr<Function> implicit) : smart_ptr<Function>

// stub


:Arguments: * **function** : smart_ptr< :ref:`Function <handle-ast-Function>` > implicit

.. _function-_at_ast_c__c_clone_variable_CI1_ls_H_ls_ast_c__c_Variable_gr__gr__qm_W:

.. das:function:: clone_variable(variable: smart_ptr<Variable> implicit) : smart_ptr<Variable>

// stub


:Arguments: * **variable** : smart_ptr< :ref:`Variable <handle-ast-Variable>` > implicit

.. _function-_at_ast_c__c_clone_type_CI1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_W:

.. das:function:: clone_type(type: smart_ptr<TypeDecl> implicit) : smart_ptr<TypeDecl>

// stub


:Arguments: * **type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

.. _function-_at_ast_c__c_clone_function_C1_ls_H_ls_ast_c__c_Function_gr__gr__qm_:

.. das:function:: clone_function(fn: Function?) : FunctionPtr

// stub


:Arguments: * **fn** :  :ref:`Function <handle-ast-Function>` ?

++++++++++++
Mangled name
++++++++++++

  *  :ref:`parse_mangled_name (txt: string implicit; lib: ModuleGroup implicit; thisModule: Module? implicit) : smart_ptr\<TypeDecl\> <function-_at_ast_c__c_parse_mangled_name_CIs_IH_ls_rtti_c__c_ModuleGroup_gr__CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__C_c_C_l>` 
  *  :ref:`get_mangled_name (function: smart_ptr\<Function\> implicit) : string <function-_at_ast_c__c_get_mangled_name_CI1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M_C_c_C_l>` 
  *  :ref:`get_mangled_name (type: smart_ptr\<TypeDecl\> implicit) : string <function-_at_ast_c__c_get_mangled_name_CI1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_C_c_C_l>` 
  *  :ref:`get_mangled_name (variable: smart_ptr\<Variable\> implicit) : string <function-_at_ast_c__c_get_mangled_name_CI1_ls_H_ls_ast_c__c_Variable_gr__gr__qm_M_C_c_C_l>` 
  *  :ref:`get_mangled_name (variable: smart_ptr\<ExprBlock\> implicit) : string <function-_at_ast_c__c_get_mangled_name_CI1_ls_H_ls_ast_c__c_ExprBlock_gr__gr__qm_M_C_c_C_l>` 
  *  :ref:`get_mangled_name (fn: Function?) : string <function-_at_ast_c__c_get_mangled_name_C1_ls_H_ls_ast_c__c_Function_gr__gr__qm_>` 
  *  :ref:`get_mangled_name (decl: TypeDecl?) : string <function-_at_ast_c__c_get_mangled_name_C1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_>` 
  *  :ref:`get_mangled_name (decl: Variable?) : string <function-_at_ast_c__c_get_mangled_name_C1_ls_H_ls_ast_c__c_Variable_gr__gr__qm_>` 
  *  :ref:`get_mangled_name (decl: ExprBlock?) : string <function-_at_ast_c__c_get_mangled_name_C1_ls_H_ls_ast_c__c_ExprBlock_gr__gr__qm_>` 

.. _function-_at_ast_c__c_parse_mangled_name_CIs_IH_ls_rtti_c__c_ModuleGroup_gr__CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__C_c_C_l:

.. das:function:: parse_mangled_name(txt: string implicit; lib: ModuleGroup implicit; thisModule: Module? implicit) : smart_ptr<TypeDecl>

// stub


:Arguments: * **txt** : string implicit

            * **lib** :  :ref:`ModuleGroup <handle-rtti-ModuleGroup>`  implicit

            * **thisModule** :  :ref:`Module <handle-rtti-Module>` ? implicit

.. _function-_at_ast_c__c_get_mangled_name_CI1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M_C_c_C_l:

.. das:function:: get_mangled_name(function: smart_ptr<Function> implicit) : string

// stub


:Arguments: * **function** : smart_ptr< :ref:`Function <handle-ast-Function>` > implicit

.. _function-_at_ast_c__c_get_mangled_name_CI1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_C_c_C_l:

.. das:function:: get_mangled_name(type: smart_ptr<TypeDecl> implicit) : string

// stub


:Arguments: * **type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

.. _function-_at_ast_c__c_get_mangled_name_CI1_ls_H_ls_ast_c__c_Variable_gr__gr__qm_M_C_c_C_l:

.. das:function:: get_mangled_name(variable: smart_ptr<Variable> implicit) : string

// stub


:Arguments: * **variable** : smart_ptr< :ref:`Variable <handle-ast-Variable>` > implicit

.. _function-_at_ast_c__c_get_mangled_name_CI1_ls_H_ls_ast_c__c_ExprBlock_gr__gr__qm_M_C_c_C_l:

.. das:function:: get_mangled_name(variable: smart_ptr<ExprBlock> implicit) : string

// stub


:Arguments: * **variable** : smart_ptr< :ref:`ExprBlock <handle-ast-ExprBlock>` > implicit

.. _function-_at_ast_c__c_get_mangled_name_C1_ls_H_ls_ast_c__c_Function_gr__gr__qm_:

.. das:function:: get_mangled_name(fn: Function?) : string

// stub


:Arguments: * **fn** :  :ref:`Function <handle-ast-Function>` ?

.. _function-_at_ast_c__c_get_mangled_name_C1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_:

.. das:function:: get_mangled_name(decl: TypeDecl?) : string

// stub


:Arguments: * **decl** :  :ref:`TypeDecl <handle-ast-TypeDecl>` ?

.. _function-_at_ast_c__c_get_mangled_name_C1_ls_H_ls_ast_c__c_Variable_gr__gr__qm_:

.. das:function:: get_mangled_name(decl: Variable?) : string

// stub


:Arguments: * **decl** :  :ref:`Variable <handle-ast-Variable>` ?

.. _function-_at_ast_c__c_get_mangled_name_C1_ls_H_ls_ast_c__c_ExprBlock_gr__gr__qm_:

.. das:function:: get_mangled_name(decl: ExprBlock?) : string

// stub


:Arguments: * **decl** :  :ref:`ExprBlock <handle-ast-ExprBlock>` ?

+++++++++++++++
Size and offset
+++++++++++++++

  *  :ref:`get_variant_field_offset (variant: smart_ptr\<TypeDecl\> implicit; index: int) : int <function-_at_ast_c__c_get_variant_field_offset_CI1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_Ci_C_c_C_l>` 
  *  :ref:`get_tuple_field_offset (typle: smart_ptr\<TypeDecl\> implicit; index: int) : int <function-_at_ast_c__c_get_tuple_field_offset_CI1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_Ci_C_c_C_l>` 
  *  :ref:`any_array_size (array: void? implicit) : int <function-_at_ast_c__c_any_array_size_CI_qm_>` 
  *  :ref:`any_table_size (table: void? implicit) : int <function-_at_ast_c__c_any_table_size_CI_qm_>` 
  *  :ref:`get_handled_type_field_offset (type: smart_ptr\<TypeAnnotation\> implicit; field: string implicit) : uint <function-_at_ast_c__c_get_handled_type_field_offset_CI1_ls_H_ls_rtti_c__c_TypeAnnotation_gr__gr__qm_M_CIs_C_c_C_l>` 

.. _function-_at_ast_c__c_get_variant_field_offset_CI1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_Ci_C_c_C_l:

.. das:function:: get_variant_field_offset(variant: smart_ptr<TypeDecl> implicit; index: int) : int

// stub


:Arguments: * **variant** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

            * **index** : int

.. _function-_at_ast_c__c_get_tuple_field_offset_CI1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_Ci_C_c_C_l:

.. das:function:: get_tuple_field_offset(typle: smart_ptr<TypeDecl> implicit; index: int) : int

// stub


:Arguments: * **typle** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

            * **index** : int

.. _function-_at_ast_c__c_any_array_size_CI_qm_:

.. das:function:: any_array_size(array: void? implicit) : int

// stub


:Arguments: * **array** : void? implicit

.. _function-_at_ast_c__c_any_table_size_CI_qm_:

.. das:function:: any_table_size(table: void? implicit) : int

// stub


:Arguments: * **table** : void? implicit

.. _function-_at_ast_c__c_get_handled_type_field_offset_CI1_ls_H_ls_rtti_c__c_TypeAnnotation_gr__gr__qm_M_CIs_C_c_C_l:

.. das:function:: get_handled_type_field_offset(type: smart_ptr<TypeAnnotation> implicit; field: string implicit) : uint

// stub


:Arguments: * **type** : smart_ptr< :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>` > implicit

            * **field** : string implicit

+++++++++++
Evaluations
+++++++++++

  *  :ref:`eval_single_expression (expr: smart_ptr\<Expression\> const& implicit; ok: bool& implicit) : float4 <function-_at_ast_c__c_eval_single_expression_C&I1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_W_&Ib>` 

.. _function-_at_ast_c__c_eval_single_expression_C&I1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_W_&Ib:

.. das:function:: eval_single_expression(expr: smart_ptr<Expression> const& implicit; ok: bool& implicit) : float4

.. warning:: 
  This is unsafe operation.

// stub


:Arguments: * **expr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >& implicit

            * **ok** : bool& implicit

+++++++++++++++
Error reporting
+++++++++++++++

  *  :ref:`macro_error (porogram: smart_ptr\<Program\> implicit; at: LineInfo implicit; message: string implicit) <function-_at_ast_c__c_macro_error_CI1_ls_H_ls_rtti_c__c_Program_gr__gr__qm_W_CIH_ls_rtti_c__c_LineInfo_gr__CIs_C_c_C_l>` 

.. _function-_at_ast_c__c_macro_error_CI1_ls_H_ls_rtti_c__c_Program_gr__gr__qm_W_CIH_ls_rtti_c__c_LineInfo_gr__CIs_C_c_C_l:

.. das:function:: macro_error(porogram: smart_ptr<Program> implicit; at: LineInfo implicit; message: string implicit)

// stub


:Arguments: * **porogram** : smart_ptr< :ref:`Program <handle-rtti-Program>` > implicit

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>`  implicit

            * **message** : string implicit

++++++++++++++++++++
Location and context
++++++++++++++++++++

  *  :ref:`force_at (expression: smart_ptr\<Expression\> const& implicit; at: LineInfo implicit) <function-_at_ast_c__c_force_at_C&I1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M_CIH_ls_rtti_c__c_LineInfo_gr_>` 
  *  :ref:`collect_dependencies (function: smart_ptr\<Function\> implicit; block: block\<(var arg0:array\<Function?\>;var arg1:array\<Variable?\>):void\> implicit) <function-_at_ast_c__c_collect_dependencies_CI1_ls_H_ls_ast_c__c_Function_gr__gr__qm_W_CI0_ls_1_ls_1_ls_H_ls_ast_c__c_Function_gr__gr__qm__gr_A;1_ls_1_ls_H_ls_ast_c__c_Variable_gr__gr__qm__gr_A_gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`get_ast_context (program: smart_ptr\<Program\> implicit; expression: smart_ptr\<Expression\> implicit; block: block\<(var arg0:bool;var arg1:AstContext):void\> implicit) <function-_at_ast_c__c_get_ast_context_CI1_ls_H_ls_rtti_c__c_Program_gr__gr__qm_M_CI1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M_CI0_ls_b;H_ls_ast_c__c_AstContext_gr__gr_1_ls_v_gr__builtin__C_c_C_l>` 

.. _function-_at_ast_c__c_force_at_C&I1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M_CIH_ls_rtti_c__c_LineInfo_gr_:

.. das:function:: force_at(expression: smart_ptr<Expression> const& implicit; at: LineInfo implicit)

// stub


:Arguments: * **expression** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >& implicit

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>`  implicit

.. _function-_at_ast_c__c_collect_dependencies_CI1_ls_H_ls_ast_c__c_Function_gr__gr__qm_W_CI0_ls_1_ls_1_ls_H_ls_ast_c__c_Function_gr__gr__qm__gr_A;1_ls_1_ls_H_ls_ast_c__c_Variable_gr__gr__qm__gr_A_gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: collect_dependencies(function: smart_ptr<Function> implicit; block: block<(var arg0:array<Function?>;var arg1:array<Variable?>):void> implicit)

// stub


:Arguments: * **function** : smart_ptr< :ref:`Function <handle-ast-Function>` > implicit

            * **block** : block<(array< :ref:`Function <handle-ast-Function>` ?>;array< :ref:`Variable <handle-ast-Variable>` ?>):void> implicit

.. _function-_at_ast_c__c_get_ast_context_CI1_ls_H_ls_rtti_c__c_Program_gr__gr__qm_M_CI1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M_CI0_ls_b;H_ls_ast_c__c_AstContext_gr__gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: get_ast_context(program: smart_ptr<Program> implicit; expression: smart_ptr<Expression> implicit; block: block<(var arg0:bool;var arg1:AstContext):void> implicit)

// stub


:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>` > implicit

            * **expression** : smart_ptr< :ref:`Expression <handle-ast-Expression>` > implicit

            * **block** : block<(bool; :ref:`AstContext <handle-ast-AstContext>` ):void> implicit

+++++++++++
Use queries
+++++++++++

  *  :ref:`get_use_global_variables (func: smart_ptr\<Function\> implicit; block: block\<(var arg0:smart_ptr\<Variable\>):void\> implicit) <function-_at_ast_c__c_get_use_global_variables_CI1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M_CI0_ls_1_ls_H_ls_ast_c__c_Variable_gr__gr__qm_W_gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`get_use_functions (func: smart_ptr\<Function\> implicit; block: block\<(var arg0:smart_ptr\<Function\>):void\> implicit) <function-_at_ast_c__c_get_use_functions_CI1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M_CI0_ls_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_W_gr_1_ls_v_gr__builtin__C_c_C_l>` 

.. _function-_at_ast_c__c_get_use_global_variables_CI1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M_CI0_ls_1_ls_H_ls_ast_c__c_Variable_gr__gr__qm_W_gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: get_use_global_variables(func: smart_ptr<Function> implicit; block: block<(var arg0:smart_ptr<Variable>):void> implicit)

// stub


:Arguments: * **func** : smart_ptr< :ref:`Function <handle-ast-Function>` > implicit

            * **block** : block<(smart_ptr< :ref:`Variable <handle-ast-Variable>` >):void> implicit

.. _function-_at_ast_c__c_get_use_functions_CI1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M_CI0_ls_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_W_gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: get_use_functions(func: smart_ptr<Function> implicit; block: block<(var arg0:smart_ptr<Function>):void> implicit)

// stub


:Arguments: * **func** : smart_ptr< :ref:`Function <handle-ast-Function>` > implicit

            * **block** : block<(smart_ptr< :ref:`Function <handle-ast-Function>` >):void> implicit

+++
Log
+++

  *  :ref:`to_compilation_log (text: string implicit) <function-_at_ast_c__c_to_compilation_log_CIs_C_c_C_l>` 

.. _function-_at_ast_c__c_to_compilation_log_CIs_C_c_C_l:

.. das:function:: to_compilation_log(text: string implicit)

// stub


:Arguments: * **text** : string implicit

+++++++
Removal
+++++++

  *  :ref:`remove_structure (module: Module? implicit; structure: smart_ptr\<Structure\>& implicit) : bool <function-_at_ast_c__c_remove_structure_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_W>` 

.. _function-_at_ast_c__c_remove_structure_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__&I1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_W:

.. das:function:: remove_structure(module: Module? implicit; structure: smart_ptr<Structure>& implicit) : bool

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **structure** : smart_ptr< :ref:`Structure <handle-ast-Structure>` >& implicit

++++++++++
Properties
++++++++++

  *  :ref:`get_current_search_module (program: Program? implicit; function: Function? implicit; moduleName: string implicit) : Module? <function-_at_ast_c__c_get_current_search_module_CI1_ls_H_ls_rtti_c__c_Program_gr__gr__qm__CI1_ls_H_ls_ast_c__c_Function_gr__gr__qm__CIs>` 
  *  :ref:`can_access_global_variable (variable: smart_ptr\<Variable\> const& implicit; module: Module? implicit; thisModule: Module? implicit) : bool <function-_at_ast_c__c_can_access_global_variable_C&I1_ls_H_ls_ast_c__c_Variable_gr__gr__qm_W_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm_>` 
  *  :ref:`is_temp_type (type: smart_ptr\<TypeDecl\> implicit; refMatters: bool) : bool <function-_at_ast_c__c_is_temp_type_CI1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_W_Cb>` 
  *  :ref:`is_same_type (leftType: smart_ptr\<TypeDecl\> implicit; rightType: smart_ptr\<TypeDecl\> implicit; refMatters: RefMatters; constMatters: ConstMatters; tempMatters: TemporaryMatters) : bool <function-_at_ast_c__c_is_same_type_CI1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_W_CI1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_W_CE_ls_rtti_c__c_RefMatters_gr__CE_ls_rtti_c__c_ConstMatters_gr__CE_ls_rtti_c__c_TemporaryMatters_gr__C_c_C_l>` 
  *  :ref:`get_underlying_value_type (type: smart_ptr\<TypeDecl\> implicit) : smart_ptr\<TypeDecl\> <function-_at_ast_c__c_get_underlying_value_type_CI1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_C_c_C_l>` 
  *  :ref:`get_handled_type_field_type (type: smart_ptr\<TypeAnnotation\> implicit; field: string implicit) : TypeInfo? <function-_at_ast_c__c_get_handled_type_field_type_CI1_ls_H_ls_rtti_c__c_TypeAnnotation_gr__gr__qm_M_CIs_C_c_C_l>` 
  *  :ref:`get_handled_type_field_type_declaration (type: smart_ptr\<TypeAnnotation\> implicit; field: string implicit; isConst: bool) : smart_ptr\<TypeDecl\> <function-_at_ast_c__c_get_handled_type_field_type_declaration_CI1_ls_H_ls_rtti_c__c_TypeAnnotation_gr__gr__qm_M_CIs_Cb_C_c_C_l>` 
  *  :ref:`has_field (type: smart_ptr\<TypeDecl\> implicit; fieldName: string implicit; constant: bool) : bool <function-_at_ast_c__c_has_field_CI1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_W_CIs_Cb>` 
  *  :ref:`get_field_type (type: smart_ptr\<TypeDecl\> implicit; fieldName: string implicit; constant: bool) : smart_ptr\<TypeDecl\> <function-_at_ast_c__c_get_field_type_CI1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_W_CIs_Cb>` 
  *  :ref:`is_visible_directly (from_module: Module? implicit; which_module: Module? implicit) : bool <function-_at_ast_c__c_is_visible_directly_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm_>` 
  *  :ref:`is_expr_like_call (expression: smart_ptr\<Expression\> const& implicit) : bool <function-_at_ast_c__c_is_expr_like_call_C&I1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_W>` 
  *  :ref:`is_expr_const (expression: smart_ptr\<Expression\> const& implicit) : bool <function-_at_ast_c__c_is_expr_const_C&I1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_W>` 
  *  :ref:`get_function_aot_hash (fun: Function const? implicit) : uint64 <function-_at_ast_c__c_get_function_aot_hash_CI1_ls_CH_ls_ast_c__c_Function_gr__gr__qm_>` 

.. _function-_at_ast_c__c_get_current_search_module_CI1_ls_H_ls_rtti_c__c_Program_gr__gr__qm__CI1_ls_H_ls_ast_c__c_Function_gr__gr__qm__CIs:

.. das:function:: get_current_search_module(program: Program? implicit; function: Function? implicit; moduleName: string implicit) : Module?

// stub


:Arguments: * **program** :  :ref:`Program <handle-rtti-Program>` ? implicit

            * **function** :  :ref:`Function <handle-ast-Function>` ? implicit

            * **moduleName** : string implicit

.. _function-_at_ast_c__c_can_access_global_variable_C&I1_ls_H_ls_ast_c__c_Variable_gr__gr__qm_W_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm_:

.. das:function:: can_access_global_variable(variable: smart_ptr<Variable> const& implicit; module: Module? implicit; thisModule: Module? implicit) : bool

// stub


:Arguments: * **variable** : smart_ptr< :ref:`Variable <handle-ast-Variable>` >& implicit

            * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **thisModule** :  :ref:`Module <handle-rtti-Module>` ? implicit

.. _function-_at_ast_c__c_is_temp_type_CI1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_W_Cb:

.. das:function:: is_temp_type(type: smart_ptr<TypeDecl> implicit; refMatters: bool) : bool

// stub


:Arguments: * **type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

            * **refMatters** : bool

.. _function-_at_ast_c__c_is_same_type_CI1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_W_CI1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_W_CE_ls_rtti_c__c_RefMatters_gr__CE_ls_rtti_c__c_ConstMatters_gr__CE_ls_rtti_c__c_TemporaryMatters_gr__C_c_C_l:

.. das:function:: is_same_type(leftType: smart_ptr<TypeDecl> implicit; rightType: smart_ptr<TypeDecl> implicit; refMatters: RefMatters; constMatters: ConstMatters; tempMatters: TemporaryMatters) : bool

// stub


:Arguments: * **leftType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

            * **rightType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

            * **refMatters** :  :ref:`RefMatters <enum-rtti-RefMatters>` 

            * **constMatters** :  :ref:`ConstMatters <enum-rtti-ConstMatters>` 

            * **tempMatters** :  :ref:`TemporaryMatters <enum-rtti-TemporaryMatters>` 

.. _function-_at_ast_c__c_get_underlying_value_type_CI1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_C_c_C_l:

.. das:function:: get_underlying_value_type(type: smart_ptr<TypeDecl> implicit) : smart_ptr<TypeDecl>

// stub


:Arguments: * **type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

.. _function-_at_ast_c__c_get_handled_type_field_type_CI1_ls_H_ls_rtti_c__c_TypeAnnotation_gr__gr__qm_M_CIs_C_c_C_l:

.. das:function:: get_handled_type_field_type(type: smart_ptr<TypeAnnotation> implicit; field: string implicit) : TypeInfo?

// stub


:Arguments: * **type** : smart_ptr< :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>` > implicit

            * **field** : string implicit

.. _function-_at_ast_c__c_get_handled_type_field_type_declaration_CI1_ls_H_ls_rtti_c__c_TypeAnnotation_gr__gr__qm_M_CIs_Cb_C_c_C_l:

.. das:function:: get_handled_type_field_type_declaration(type: smart_ptr<TypeAnnotation> implicit; field: string implicit; isConst: bool) : smart_ptr<TypeDecl>

// stub


:Arguments: * **type** : smart_ptr< :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>` > implicit

            * **field** : string implicit

            * **isConst** : bool

.. _function-_at_ast_c__c_has_field_CI1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_W_CIs_Cb:

.. das:function:: has_field(type: smart_ptr<TypeDecl> implicit; fieldName: string implicit; constant: bool) : bool

// stub


:Arguments: * **type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

            * **fieldName** : string implicit

            * **constant** : bool

.. _function-_at_ast_c__c_get_field_type_CI1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_W_CIs_Cb:

.. das:function:: get_field_type(type: smart_ptr<TypeDecl> implicit; fieldName: string implicit; constant: bool) : smart_ptr<TypeDecl>

// stub


:Arguments: * **type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

            * **fieldName** : string implicit

            * **constant** : bool

.. _function-_at_ast_c__c_is_visible_directly_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm_:

.. das:function:: is_visible_directly(from_module: Module? implicit; which_module: Module? implicit) : bool

// stub


:Arguments: * **from_module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **which_module** :  :ref:`Module <handle-rtti-Module>` ? implicit

.. _function-_at_ast_c__c_is_expr_like_call_C&I1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_W:

.. das:function:: is_expr_like_call(expression: smart_ptr<Expression> const& implicit) : bool

// stub


:Arguments: * **expression** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >& implicit

.. _function-_at_ast_c__c_is_expr_const_C&I1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_W:

.. das:function:: is_expr_const(expression: smart_ptr<Expression> const& implicit) : bool

// stub


:Arguments: * **expression** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >& implicit

.. _function-_at_ast_c__c_get_function_aot_hash_CI1_ls_CH_ls_ast_c__c_Function_gr__gr__qm_:

.. das:function:: get_function_aot_hash(fun: Function const? implicit) : uint64

// stub


:Arguments: * **fun** :  :ref:`Function <handle-ast-Function>` ? implicit

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_ast_c__c_infer_generic_type_CI1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_CI1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_Cb_Cb:

.. das:function:: infer_generic_type(type: smart_ptr<TypeDecl> implicit; passType: smart_ptr<TypeDecl> implicit; topLevel: bool; isPassType: bool) : smart_ptr<TypeDecl>

// stub


:Arguments: * **type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

            * **passType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

            * **topLevel** : bool

            * **isPassType** : bool

.. _function-_at_ast_c__c_update_alias_map_CI1_ls_H_ls_rtti_c__c_Program_gr__gr__qm_M_CI1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_CI1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_C_c_C_l:

.. das:function:: update_alias_map(program: smart_ptr<Program> implicit; argType: smart_ptr<TypeDecl> implicit; passType: smart_ptr<TypeDecl> implicit)

// stub


:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>` > implicit

            * **argType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

            * **passType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit


