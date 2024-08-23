
.. _stdlib_rtti:

================================
Runtime type information library
================================

The RTTI module reflects runtime type information to Daslang.
It also exposes Daslang compiler infrastructure to Daslang runtime.

All functions and symbols are in "rtti" module, use require to get access to it. ::

    require rtti


++++++++++++
Type aliases
++++++++++++

.. _alias-ProgramFlags:

.. das:attribute:: ProgramFlags is a bitfield

:Fields: * **failToCompile** (0)

         * **_unsafe** (1)

         * **isCompiling** (2)

         * **isSimulating** (3)

         * **isCompilingMacros** (4)

         * **needMacroModule** (5)

         * **promoteToBuiltin** (6)

         * **isDependency** (7)

         * **macroException** (8)

// stub


.. _alias-context_category_flags:

.. das:attribute:: context_category_flags is a bitfield

:Fields: * **dead** (0)

         * **debug_context** (1)

         * **thread_clone** (2)

         * **job_clone** (3)

         * **opengl** (4)

         * **debugger_tick** (5)

         * **debugger_attached** (6)

         * **macro_context** (7)

         * **folding_context** (8)

         * **audio** (9)

// stub


.. _alias-TypeInfoFlags:

.. das:attribute:: TypeInfoFlags is a bitfield

:Fields: * **ref** (0)

         * **refType** (1)

         * **canCopy** (2)

         * **isPod** (3)

         * **isRawPod** (4)

         * **isConst** (5)

         * **isTemp** (6)

         * **isImplicit** (7)

         * **refValue** (8)

         * **hasInitValue** (9)

         * **isSmartPtr** (10)

         * **isSmartPtrNative** (11)

         * **isHandled** (12)

         * **heapGC** (13)

         * **stringHeapGC** (14)

         * **lockCheck** (15)

         * **isPrivate** (16)

// stub


.. _alias-StructInfoFlags:

.. das:attribute:: StructInfoFlags is a bitfield

:Fields: * **_class** (0)

         * **_lambda** (1)

         * **heapGC** (2)

         * **stringHeapGC** (3)

         * **lockCheck** (4)

// stub


.. _alias-ModuleFlags:

.. das:attribute:: ModuleFlags is a bitfield

:Fields: * **builtIn** (0)

         * **promoted** (1)

         * **isPublic** (2)

         * **isModule** (3)

         * **isSolidContext** (4)

         * **doNotAllowUnsafe** (5)

// stub


.. _alias-AnnotationDeclarationFlags:

.. das:attribute:: AnnotationDeclarationFlags is a bitfield

:Fields: * **inherited** (0)

// stub


.. _alias-RttiValue:

.. das:attribute:: RttiValue is a variant type

:Variants: * **tBool** : bool

           * **tInt** : int

           * **tUInt** : uint

           * **tInt64** : int64

           * **tUInt64** : uint64

           * **tFloat** : float

           * **tDouble** : double

           * **tString** : string

           * **nothing** : any

// stub


.. _alias-FileAccessPtr:

.. das:attribute:: FileAccessPtr = smart_ptr<FileAccess>

// stub


+++++++++
Constants
+++++++++

.. _global-rtti-FUNCINFO_INIT:

.. das:attribute:: FUNCINFO_INIT = 0x1

// stub


.. _global-rtti-FUNCINFO_BUILTIN:

.. das:attribute:: FUNCINFO_BUILTIN = 0x2

// stub


.. _global-rtti-FUNCINFO_PRIVATE:

.. das:attribute:: FUNCINFO_PRIVATE = 0x4

// stub


.. _global-rtti-FUNCINFO_SHUTDOWN:

.. das:attribute:: FUNCINFO_SHUTDOWN = 0x8

// stub


.. _global-rtti-FUNCINFO_LATE_INIT:

.. das:attribute:: FUNCINFO_LATE_INIT = 0x20

// stub


++++++++++++
Enumerations
++++++++++++

.. _enum-rtti-CompilationError:

.. das:attribute:: CompilationError

:Values: * **unspecified** = 0

         * **mismatching_parentheses** = 10001

         * **mismatching_curly_bracers** = 10002

         * **string_constant_exceeds_file** = 10003

         * **string_constant_exceeds_line** = 10004

         * **unexpected_close_comment** = 10005

         * **integer_constant_out_of_range** = 10006

         * **comment_contains_eof** = 10007

         * **invalid_escape_sequence** = 10008

         * **invalid_line_directive** = 10009

         * **syntax_error** = 20000

         * **malformed_ast** = 20001

         * **invalid_type** = 30101

         * **invalid_return_type** = 30102

         * **invalid_argument_type** = 30103

         * **invalid_structure_field_type** = 30104

         * **invalid_array_type** = 30105

         * **invalid_table_type** = 30106

         * **invalid_argument_count** = 30107

         * **invalid_variable_type** = 30108

         * **invalid_new_type** = 30109

         * **invalid_index_type** = 30110

         * **invalid_annotation** = 30111

         * **invalid_swizzle_mask** = 30112

         * **invalid_initialization_type** = 30113

         * **invalid_with_type** = 30114

         * **invalid_override** = 30115

         * **invalid_name** = 30116

         * **invalid_array_dimension** = 30117

         * **invalid_iteration_source** = 30118

         * **invalid_loop** = 30119

         * **invalid_label** = 30120

         * **invalid_enumeration** = 30121

         * **invalid_option** = 30122

         * **invalid_member_function** = 30123

         * **function_already_declared** = 30201

         * **argument_already_declared** = 30202

         * **local_variable_already_declared** = 30203

         * **global_variable_already_declared** = 30204

         * **structure_field_already_declared** = 30205

         * **structure_already_declared** = 30206

         * **structure_already_has_initializer** = 30207

         * **enumeration_already_declared** = 30208

         * **enumeration_value_already_declared** = 30209

         * **type_alias_already_declared** = 30210

         * **field_already_initialized** = 30211

         * **type_not_found** = 30301

         * **structure_not_found** = 30302

         * **operator_not_found** = 30303

         * **function_not_found** = 30304

         * **variable_not_found** = 30305

         * **handle_not_found** = 30306

         * **annotation_not_found** = 30307

         * **enumeration_not_found** = 30308

         * **enumeration_value_not_found** = 30309

         * **type_alias_not_found** = 30310

         * **bitfield_not_found** = 30311

         * **cant_initialize** = 30401

         * **cant_dereference** = 30501

         * **cant_index** = 30502

         * **cant_get_field** = 30503

         * **cant_write_to_const** = 30504

         * **cant_move_to_const** = 30505

         * **cant_write_to_non_reference** = 30506

         * **cant_copy** = 30507

         * **cant_move** = 30508

         * **cant_pass_temporary** = 30509

         * **condition_must_be_bool** = 30601

         * **condition_must_be_static** = 30602

         * **cant_pipe** = 30701

         * **invalid_block** = 30801

         * **return_or_break_in_finally** = 30802

         * **module_not_found** = 30901

         * **module_already_has_a_name** = 30902

         * **cant_new_handle** = 31001

         * **bad_delete** = 31002

         * **cant_infer_generic** = 31100

         * **cant_infer_missing_initializer** = 31101

         * **cant_infer_mismatching_restrictions** = 31102

         * **invalid_cast** = 31200

         * **incompatible_cast** = 31201

         * **unsafe** = 31300

         * **index_out_of_range** = 31400

         * **expecting_return_value** = 32101

         * **not_expecting_return_value** = 32102

         * **invalid_return_semantics** = 32103

         * **invalid_yield** = 32104

         * **typeinfo_reference** = 39901

         * **typeinfo_auto** = 39902

         * **typeinfo_undefined** = 39903

         * **typeinfo_dim** = 39904

         * **typeinfo_macro_error** = 39905

         * **static_assert_failed** = 40100

         * **run_failed** = 40101

         * **annotation_failed** = 40102

         * **concept_failed** = 40103

         * **not_all_paths_return_value** = 40200

         * **assert_with_side_effects** = 40201

         * **only_fast_aot_no_cpp_name** = 40202

         * **aot_side_effects** = 40203

         * **no_global_heap** = 40204

         * **no_global_variables** = 40205

         * **unused_function_argument** = 40206

         * **unsafe_function** = 40207

         * **too_many_infer_passes** = 41000

         * **missing_node** = 50100

// stub


.. _enum-rtti-Type:

.. das:attribute:: Type

:Values: * **none** = 0

         * **autoinfer** = 1

         * **alias** = 2

         * **option** = 3

         * **typeDecl** = 4

         * **typeMacro** = 5

         * **fakeContext** = 6

         * **fakeLineInfo** = 7

         * **anyArgument** = 8

         * **tVoid** = 9

         * **tBool** = 10

         * **tInt8** = 11

         * **tUInt8** = 12

         * **tInt16** = 13

         * **tUInt16** = 14

         * **tInt64** = 15

         * **tUInt64** = 16

         * **tInt** = 17

         * **tInt2** = 18

         * **tInt3** = 19

         * **tInt4** = 20

         * **tUInt** = 21

         * **tUInt2** = 22

         * **tUInt3** = 23

         * **tUInt4** = 24

         * **tFloat** = 25

         * **tFloat2** = 26

         * **tFloat3** = 27

         * **tFloat4** = 28

         * **tDouble** = 29

         * **tRange** = 30

         * **tURange** = 31

         * **tRange64** = 32

         * **tURange64** = 33

         * **tString** = 34

         * **tStructure** = 35

         * **tHandle** = 36

         * **tEnumeration** = 37

         * **tEnumeration8** = 38

         * **tEnumeration16** = 39

         * **tBitfield** = 40

         * **tPointer** = 41

         * **tFunction** = 42

         * **tLambda** = 43

         * **tIterator** = 44

         * **tArray** = 45

         * **tTable** = 46

         * **tBlock** = 47

         * **tTuple** = 48

         * **tVariant** = 49

// stub


.. _enum-rtti-RefMatters:

.. das:attribute:: RefMatters

:Values: * **no** = 0

         * **yes** = 1

// stub


.. _enum-rtti-ConstMatters:

.. das:attribute:: ConstMatters

:Values: * **no** = 0

         * **yes** = 1

// stub


.. _enum-rtti-TemporaryMatters:

.. das:attribute:: TemporaryMatters

:Values: * **no** = 0

         * **yes** = 1

// stub


++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-rtti-FileInfo:

.. das:attribute:: FileInfo

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **tabSize** : int

// stub


.. _handle-rtti-LineInfo:

.. das:attribute:: LineInfo

:Fields: * **fileInfo** :  :ref:`FileInfo <handle-rtti-FileInfo>` ?

         * **column** : uint

         * **line** : uint

         * **last_column** : uint

         * **last_line** : uint

// stub


.. _handle-rtti-Context:

.. das:attribute:: Context

.. _function-_at_rtti_c__c__dot__rq_totalFunctions_CIH_ls_rtti_c__c_Context_gr_:

.. das:function:: Context implicit.totalFunctions() : int

// stub


.. _function-_at_rtti_c__c__dot__rq_totalVariables_CIH_ls_rtti_c__c_Context_gr_:

.. das:function:: Context implicit.totalVariables() : int

// stub


.. _function-_at_rtti_c__c__dot__rq_getCodeAllocatorId_IH_ls_rtti_c__c_Context_gr_:

.. das:function:: Context implicit.getCodeAllocatorId() : uint64

// stub


:Properties: * **totalFunctions** : int

             * **totalVariables** : int

             * **getCodeAllocatorId** : uint64

:Fields: * **breakOnException** : bool

         * **alwaysErrorOnException** : bool

         * **alwaysStackWalkOnException** : bool

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **category** :  :ref:`context_category_flags <alias-context_category_flags>` 

         * **exceptionAt** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **exception** : string

         * **last_exception** : string

         * **contextMutex** :  :ref:`recursive_mutex <handle-rtti-recursive_mutex>` ?

// stub


.. _handle-rtti-Error:

.. das:attribute:: Error

:Fields: * **what** :  :ref:`das_string <handle-builtin-das_string>` 

         * **extra** :  :ref:`das_string <handle-builtin-das_string>` 

         * **fixme** :  :ref:`das_string <handle-builtin-das_string>` 

         * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **cerr** :  :ref:`CompilationError <enum-rtti-CompilationError>` 

// stub


.. _handle-rtti-FileAccess:

.. das:attribute:: FileAccess

// stub


.. _handle-rtti-Module:

.. das:attribute:: Module

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **moduleFlags** :  :ref:`ModuleFlags <alias-ModuleFlags>` 

// stub


.. _handle-rtti-ModuleGroup:

.. das:attribute:: ModuleGroup

// stub


.. _handle-rtti-AnnotationArgument:

.. das:attribute:: AnnotationArgument

:Fields: * **basicType** :  :ref:`Type <enum-rtti-Type>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **sValue** :  :ref:`das_string <handle-builtin-das_string>` 

         * **iValue** : int

         * **fValue** : float

         * **bValue** : bool

         * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

// stub


.. _handle-rtti-Program:

.. das:attribute:: Program

:Fields: * **thisModuleName** :  :ref:`das_string <handle-builtin-das_string>` 

         * **errors** : vector<Error>

         * **flags** :  :ref:`ProgramFlags <alias-ProgramFlags>` 

         * **_options** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

// stub


.. _handle-rtti-Annotation:

.. das:attribute:: Annotation

.. _function-_at_rtti_c__c__dot__rq_isTypeAnnotation_CIH_ls_rtti_c__c_Annotation_gr_:

.. das:function:: Annotation implicit.isTypeAnnotation() : bool

// stub


.. _function-_at_rtti_c__c__dot__rq_isBasicStructureAnnotation_CIH_ls_rtti_c__c_Annotation_gr_:

.. das:function:: Annotation implicit.isBasicStructureAnnotation() : bool

// stub


.. _function-_at_rtti_c__c__dot__rq_isStructureAnnotation_CIH_ls_rtti_c__c_Annotation_gr_:

.. das:function:: Annotation implicit.isStructureAnnotation() : bool

// stub


.. _function-_at_rtti_c__c__dot__rq_isStructureTypeAnnotation_CIH_ls_rtti_c__c_Annotation_gr_:

.. das:function:: Annotation implicit.isStructureTypeAnnotation() : bool

// stub


.. _function-_at_rtti_c__c__dot__rq_isFunctionAnnotation_CIH_ls_rtti_c__c_Annotation_gr_:

.. das:function:: Annotation implicit.isFunctionAnnotation() : bool

// stub


.. _function-_at_rtti_c__c__dot__rq_isEnumerationAnnotation_CIH_ls_rtti_c__c_Annotation_gr_:

.. das:function:: Annotation implicit.isEnumerationAnnotation() : bool

// stub


:Properties: * **isTypeAnnotation** : bool

             * **isBasicStructureAnnotation** : bool

             * **isStructureAnnotation** : bool

             * **isStructureTypeAnnotation** : bool

             * **isFunctionAnnotation** : bool

             * **isEnumerationAnnotation** : bool

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **cppName** :  :ref:`das_string <handle-builtin-das_string>` 

         * **_module** :  :ref:`Module <handle-rtti-Module>` ?

// stub


.. _handle-rtti-AnnotationDeclaration:

.. das:attribute:: AnnotationDeclaration

:Fields: * **annotation** : smart_ptr< :ref:`Annotation <handle-rtti-Annotation>` >

         * **arguments** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

         * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **flags** :  :ref:`AnnotationDeclarationFlags <alias-AnnotationDeclarationFlags>` 

// stub


.. _handle-rtti-TypeAnnotation:

.. das:attribute:: TypeAnnotation

.. _function-_at_rtti_c__c__dot__rq_is_any_vector_CIH_ls_rtti_c__c_TypeAnnotation_gr_:

.. das:function:: TypeAnnotation implicit.is_any_vector() : bool

// stub


.. _function-_at_rtti_c__c__dot__rq_canMove_CIH_ls_rtti_c__c_TypeAnnotation_gr_:

.. das:function:: TypeAnnotation implicit.canMove() : bool

// stub


.. _function-_at_rtti_c__c__dot__rq_canCopy_CIH_ls_rtti_c__c_TypeAnnotation_gr_:

.. das:function:: TypeAnnotation implicit.canCopy() : bool

// stub


.. _function-_at_rtti_c__c__dot__rq_canClone_CIH_ls_rtti_c__c_TypeAnnotation_gr_:

.. das:function:: TypeAnnotation implicit.canClone() : bool

// stub


.. _function-_at_rtti_c__c__dot__rq_isPod_CIH_ls_rtti_c__c_TypeAnnotation_gr_:

.. das:function:: TypeAnnotation implicit.isPod() : bool

// stub


.. _function-_at_rtti_c__c__dot__rq_isRawPod_CIH_ls_rtti_c__c_TypeAnnotation_gr_:

.. das:function:: TypeAnnotation implicit.isRawPod() : bool

// stub


.. _function-_at_rtti_c__c__dot__rq_isRefType_CIH_ls_rtti_c__c_TypeAnnotation_gr_:

.. das:function:: TypeAnnotation implicit.isRefType() : bool

// stub


.. _function-_at_rtti_c__c__dot__rq_hasNonTrivialCtor_CIH_ls_rtti_c__c_TypeAnnotation_gr_:

.. das:function:: TypeAnnotation implicit.hasNonTrivialCtor() : bool

// stub


.. _function-_at_rtti_c__c__dot__rq_hasNonTrivialDtor_CIH_ls_rtti_c__c_TypeAnnotation_gr_:

.. das:function:: TypeAnnotation implicit.hasNonTrivialDtor() : bool

// stub


.. _function-_at_rtti_c__c__dot__rq_hasNonTrivialCopy_CIH_ls_rtti_c__c_TypeAnnotation_gr_:

.. das:function:: TypeAnnotation implicit.hasNonTrivialCopy() : bool

// stub


.. _function-_at_rtti_c__c__dot__rq_canBePlacedInContainer_CIH_ls_rtti_c__c_TypeAnnotation_gr_:

.. das:function:: TypeAnnotation implicit.canBePlacedInContainer() : bool

// stub


.. _function-_at_rtti_c__c__dot__rq_isLocal_CIH_ls_rtti_c__c_TypeAnnotation_gr_:

.. das:function:: TypeAnnotation implicit.isLocal() : bool

// stub


.. _function-_at_rtti_c__c__dot__rq_canNew_CIH_ls_rtti_c__c_TypeAnnotation_gr_:

.. das:function:: TypeAnnotation implicit.canNew() : bool

// stub


.. _function-_at_rtti_c__c__dot__rq_canDelete_CIH_ls_rtti_c__c_TypeAnnotation_gr_:

.. das:function:: TypeAnnotation implicit.canDelete() : bool

// stub


.. _function-_at_rtti_c__c__dot__rq_needDelete_CIH_ls_rtti_c__c_TypeAnnotation_gr_:

.. das:function:: TypeAnnotation implicit.needDelete() : bool

// stub


.. _function-_at_rtti_c__c__dot__rq_canDeletePtr_CIH_ls_rtti_c__c_TypeAnnotation_gr_:

.. das:function:: TypeAnnotation implicit.canDeletePtr() : bool

// stub


.. _function-_at_rtti_c__c__dot__rq_isIterable_CIH_ls_rtti_c__c_TypeAnnotation_gr_:

.. das:function:: TypeAnnotation implicit.isIterable() : bool

// stub


.. _function-_at_rtti_c__c__dot__rq_isShareable_CIH_ls_rtti_c__c_TypeAnnotation_gr_:

.. das:function:: TypeAnnotation implicit.isShareable() : bool

// stub


.. _function-_at_rtti_c__c__dot__rq_isSmart_CIH_ls_rtti_c__c_TypeAnnotation_gr_:

.. das:function:: TypeAnnotation implicit.isSmart() : bool

// stub


.. _function-_at_rtti_c__c__dot__rq_avoidNullPtr_CIH_ls_rtti_c__c_TypeAnnotation_gr_:

.. das:function:: TypeAnnotation implicit.avoidNullPtr() : bool

// stub


.. _function-_at_rtti_c__c__dot__rq_sizeOf_CIH_ls_rtti_c__c_TypeAnnotation_gr_:

.. das:function:: TypeAnnotation implicit.sizeOf() : uint64

// stub


.. _function-_at_rtti_c__c__dot__rq_alignOf_CIH_ls_rtti_c__c_TypeAnnotation_gr_:

.. das:function:: TypeAnnotation implicit.alignOf() : uint64

// stub


:Properties: * **is_any_vector** : bool

             * **canMove** : bool

             * **canCopy** : bool

             * **canClone** : bool

             * **isPod** : bool

             * **isRawPod** : bool

             * **isRefType** : bool

             * **hasNonTrivialCtor** : bool

             * **hasNonTrivialDtor** : bool

             * **hasNonTrivialCopy** : bool

             * **canBePlacedInContainer** : bool

             * **isLocal** : bool

             * **canNew** : bool

             * **canDelete** : bool

             * **needDelete** : bool

             * **canDeletePtr** : bool

             * **isIterable** : bool

             * **isShareable** : bool

             * **isSmart** : bool

             * **avoidNullPtr** : bool

             * **sizeOf** : uint64

             * **alignOf** : uint64

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **cppName** :  :ref:`das_string <handle-builtin-das_string>` 

         * **_module** :  :ref:`Module <handle-rtti-Module>` ?

// stub


.. _handle-rtti-BasicStructureAnnotation:

.. das:attribute:: BasicStructureAnnotation

.. _function-_at_rtti_c__c__dot__rq_fieldCount_CIH_ls_rtti_c__c_BasicStructureAnnotation_gr_:

.. das:function:: BasicStructureAnnotation implicit.fieldCount() : int

// stub


:Properties: * **fieldCount** : int

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **cppName** :  :ref:`das_string <handle-builtin-das_string>` 

// stub


.. _handle-rtti-EnumValueInfo:

.. das:attribute:: EnumValueInfo

:Fields: * **name** : string

         * **value** : int64

// stub


.. _handle-rtti-EnumInfo:

.. das:attribute:: EnumInfo

:Fields: * **name** : string

         * **module_name** : string

         * **fields** :  :ref:`EnumValueInfo <handle-rtti-EnumValueInfo>` ??

         * **count** : uint

         * **hash** : uint64

// stub


.. _handle-rtti-StructInfo:

.. das:attribute:: StructInfo

:Fields: * **name** : string

         * **module_name** : string

         * **fields** :  :ref:`VarInfo <handle-rtti-VarInfo>` ??

         * **hash** : uint64

         * **init_mnh** : uint64

         * **flags** :  :ref:`StructInfoFlags <alias-StructInfoFlags>` 

         * **count** : uint

         * **size** : uint

         * **firstGcField** : uint

// stub


.. _handle-rtti-TypeInfo:

.. das:attribute:: TypeInfo

.. _function-_at_rtti_c__c__dot__rq_enumType_CIH_ls_rtti_c__c_TypeInfo_gr_:

.. das:function:: TypeInfo implicit.enumType() : EnumInfo?

// stub


.. _function-_at_rtti_c__c__dot__rq_isRef_CIH_ls_rtti_c__c_TypeInfo_gr_:

.. das:function:: TypeInfo implicit.isRef() : bool

// stub


.. _function-_at_rtti_c__c__dot__rq_isRefType_CIH_ls_rtti_c__c_TypeInfo_gr_:

.. das:function:: TypeInfo implicit.isRefType() : bool

// stub


.. _function-_at_rtti_c__c__dot__rq_isRefValue_CIH_ls_rtti_c__c_TypeInfo_gr_:

.. das:function:: TypeInfo implicit.isRefValue() : bool

// stub


.. _function-_at_rtti_c__c__dot__rq_canCopy_CIH_ls_rtti_c__c_TypeInfo_gr_:

.. das:function:: TypeInfo implicit.canCopy() : bool

// stub


.. _function-_at_rtti_c__c__dot__rq_isPod_CIH_ls_rtti_c__c_TypeInfo_gr_:

.. das:function:: TypeInfo implicit.isPod() : bool

// stub


.. _function-_at_rtti_c__c__dot__rq_isRawPod_CIH_ls_rtti_c__c_TypeInfo_gr_:

.. das:function:: TypeInfo implicit.isRawPod() : bool

// stub


.. _function-_at_rtti_c__c__dot__rq_isConst_CIH_ls_rtti_c__c_TypeInfo_gr_:

.. das:function:: TypeInfo implicit.isConst() : bool

// stub


.. _function-_at_rtti_c__c__dot__rq_isTemp_CIH_ls_rtti_c__c_TypeInfo_gr_:

.. das:function:: TypeInfo implicit.isTemp() : bool

// stub


.. _function-_at_rtti_c__c__dot__rq_isImplicit_CIH_ls_rtti_c__c_TypeInfo_gr_:

.. das:function:: TypeInfo implicit.isImplicit() : bool

// stub


.. _function-_at_rtti_c__c__dot__rq_annotation_CIH_ls_rtti_c__c_TypeInfo_gr_:

.. das:function:: TypeInfo implicit.annotation() : TypeAnnotation?

// stub


.. _function-_at_rtti_c__c__dot__rq_structType_CIH_ls_rtti_c__c_TypeInfo_gr_:

.. das:function:: TypeInfo implicit.structType() : StructInfo?

// stub


:Properties: * **enumType** :  :ref:`EnumInfo <handle-rtti-EnumInfo>` ?

             * **isRef** : bool

             * **isRefType** : bool

             * **isRefValue** : bool

             * **canCopy** : bool

             * **isPod** : bool

             * **isRawPod** : bool

             * **isConst** : bool

             * **isTemp** : bool

             * **isImplicit** : bool

             * **annotation** :  :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>` ?

             * **structType** :  :ref:`StructInfo <handle-rtti-StructInfo>` ?

:Fields: * **firstType** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ?

         * **secondType** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ?

         * **argTypes** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ??

         * **argNames** : string?

         * **hash** : uint64

         * **basicType** :  :ref:`Type <enum-rtti-Type>` 

         * **flags** :  :ref:`TypeInfoFlags <alias-TypeInfoFlags>` 

         * **size** : uint

         * **argCount** : uint

         * **dimSize** : uint

// stub


.. _handle-rtti-VarInfo:

.. das:attribute:: VarInfo

:Fields: * **firstType** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ?

         * **secondType** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ?

         * **argTypes** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ??

         * **argNames** : string?

         * **hash** : uint64

         * **basicType** :  :ref:`Type <enum-rtti-Type>` 

         * **flags** :  :ref:`TypeInfoFlags <alias-TypeInfoFlags>` 

         * **size** : uint

         * **argCount** : uint

         * **dimSize** : uint

         * **value** : any

         * **sValue** : string

         * **name** : string

         * **annotation_arguments** :  :ref:`AnnotationArguments <handle-rtti-AnnotationArguments>` ?

         * **offset** : uint

         * **nextGcField** : uint

// stub


.. _handle-rtti-LocalVariableInfo:

.. das:attribute:: LocalVariableInfo

:Fields: * **firstType** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ?

         * **secondType** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ?

         * **argTypes** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ??

         * **argNames** : string?

         * **hash** : uint64

         * **basicType** :  :ref:`Type <enum-rtti-Type>` 

         * **flags** :  :ref:`TypeInfoFlags <alias-TypeInfoFlags>` 

         * **size** : uint

         * **argCount** : uint

         * **dimSize** : uint

         * **visibility** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **name** : string

         * **stackTop** : uint

         * **localFlags** :  :ref:`LocalVariableInfoFlags <alias-LocalVariableInfoFlags>` 

// stub


.. _handle-rtti-FuncInfo:

.. das:attribute:: FuncInfo

:Fields: * **name** : string

         * **cppName** : string

         * **result** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ?

         * **locals** :  :ref:`LocalVariableInfo <handle-rtti-LocalVariableInfo>` ??

         * **globals** :  :ref:`VarInfo <handle-rtti-VarInfo>` ??

         * **hash** : uint64

         * **flags** : uint

         * **count** : uint

         * **stackSize** : uint

         * **localCount** : uint

         * **globalCount** : uint

// stub


.. _handle-rtti-SimFunction:

.. das:attribute:: SimFunction

.. _function-_at_rtti_c__c__dot__rq_lineInfo_CIH_ls_rtti_c__c_SimFunction_gr_:

.. das:function:: SimFunction implicit.lineInfo() : LineInfo const?

// stub


:Properties: * **lineInfo** :  :ref:`LineInfo <handle-rtti-LineInfo>` ?

:Fields: * **name** : string

         * **mangledName** : string

         * **debugInfo** :  :ref:`FuncInfo <handle-rtti-FuncInfo>` ?

         * **mangledNameHash** : uint64

         * **stackSize** : uint

         * **flags** :  :ref:`SimFunctionFlags <alias-SimFunctionFlags>` 

// stub


.. _handle-rtti-CodeOfPolicies:

.. das:attribute:: CodeOfPolicies

:Fields: * **aot** : bool

         * **aot_module** : bool

         * **completion** : bool

         * **export_all** : bool

         * **always_report_candidates_threshold** : int

         * **stack** : uint

         * **intern_strings** : bool

         * **persistent_heap** : bool

         * **multiple_contexts** : bool

         * **heap_size_hint** : uint

         * **string_heap_size_hint** : uint

         * **solid_context** : bool

         * **macro_context_persistent_heap** : bool

         * **macro_context_collect** : bool

         * **max_static_variables_size** : uint64

         * **max_heap_allocated** : uint64

         * **max_string_heap_allocated** : uint64

         * **rtti** : bool

         * **no_unsafe** : bool

         * **local_ref_is_unsafe** : bool

         * **no_global_variables** : bool

         * **no_global_variables_at_all** : bool

         * **no_global_heap** : bool

         * **only_fast_aot** : bool

         * **aot_order_side_effects** : bool

         * **no_unused_function_arguments** : bool

         * **no_unused_block_arguments** : bool

         * **smart_pointer_by_value_unsafe** : bool

         * **allow_block_variable_shadowing** : bool

         * **allow_local_variable_shadowing** : bool

         * **allow_shared_lambda** : bool

         * **ignore_shared_modules** : bool

         * **default_module_public** : bool

         * **no_deprecated** : bool

         * **no_aliasing** : bool

         * **strict_smart_pointers** : bool

         * **no_init** : bool

         * **strict_unsafe_delete** : bool

         * **no_members_functions_in_struct** : bool

         * **no_local_class_members** : bool

         * **report_invisible_functions** : bool

         * **report_private_functions** : bool

         * **strict_properties** : bool

         * **no_optimizations** : bool

         * **fail_on_no_aot** : bool

         * **fail_on_lack_of_aot_export** : bool

         * **no_fast_call** : bool

         * **debugger** : bool

         * **debug_module** :  :ref:`das_string <handle-builtin-das_string>` 

         * **profiler** : bool

         * **profile_module** :  :ref:`das_string <handle-builtin-das_string>` 

         * **jit** : bool

         * **jit_module** :  :ref:`das_string <handle-builtin-das_string>` 

         * **threadlock_context** : bool

// stub


+++++++++++++++
Typeinfo macros
+++++++++++++++

.. _call-macro-rtti-rtti_typeinfo:

.. das:attribute:: rtti_typeinfo

// stub


+++++++++++++
Handled types
+++++++++++++

.. _handle-rtti-recursive_mutex:

.. das:attribute:: recursive_mutex

// stub


.. _handle-rtti-AnnotationArguments:

.. das:attribute:: AnnotationArguments

// stub


.. _handle-rtti-AnnotationArgumentList:

.. das:attribute:: AnnotationArgumentList

// stub


.. _handle-rtti-AnnotationList:

.. das:attribute:: AnnotationList

// stub


+++++++++++++++++++++++++++++++
Initialization and finalization
+++++++++++++++++++++++++++++++

  *  :ref:`LineInfo () : LineInfo <function-_at_rtti_c__c_LineInfo>` 
  *  :ref:`LineInfo (arg0: FileInfo? implicit; arg1: int; arg2: int; arg3: int; arg4: int) : LineInfo <function-_at_rtti_c__c_LineInfo_CI1_ls_H_ls_rtti_c__c_FileInfo_gr__gr__qm__Ci_Ci_Ci_Ci>` 
  *  :ref:`using (arg0: block\<(var arg0:recursive_mutex):void\> implicit) <function-_at_rtti_c__c_using_CI0_ls_XH_ls_rtti_c__c_recursive_mutex_gr__gr_1_ls_v_gr__builtin_>` 
  *  :ref:`CodeOfPolicies () : CodeOfPolicies <function-_at_rtti_c__c_CodeOfPolicies>` 
  *  :ref:`using (arg0: block\<(var arg0:CodeOfPolicies):void\> implicit) <function-_at_rtti_c__c_using_CI0_ls_XH_ls_rtti_c__c_CodeOfPolicies_gr__gr_1_ls_v_gr__builtin_>` 
  *  :ref:`using (arg0: block\<(var arg0:ModuleGroup):void\> implicit) <function-_at_rtti_c__c_using_CI0_ls_XH_ls_rtti_c__c_ModuleGroup_gr__gr_1_ls_v_gr__builtin_>` 
  *  :ref:`RttiValue_nothing () : auto <function-_at_rtti_c__c_RttiValue_nothing>` 

.. _function-_at_rtti_c__c_LineInfo:

.. das:function:: LineInfo() : LineInfo

// stub


.. _function-_at_rtti_c__c_LineInfo_CI1_ls_H_ls_rtti_c__c_FileInfo_gr__gr__qm__Ci_Ci_Ci_Ci:

.. das:function:: LineInfo(arg0: FileInfo? implicit; arg1: int; arg2: int; arg3: int; arg4: int) : LineInfo

// stub


:Arguments: * **arg0** :  :ref:`FileInfo <handle-rtti-FileInfo>` ? implicit

            * **arg1** : int

            * **arg2** : int

            * **arg3** : int

            * **arg4** : int

.. _function-_at_rtti_c__c_using_CI0_ls_XH_ls_rtti_c__c_recursive_mutex_gr__gr_1_ls_v_gr__builtin_:

.. das:function:: using(arg0: block<(var arg0:recursive_mutex):void> implicit)

// stub


:Arguments: * **arg0** : block<( :ref:`recursive_mutex <handle-rtti-recursive_mutex>` ):void> implicit

.. _function-_at_rtti_c__c_CodeOfPolicies:

.. das:function:: CodeOfPolicies() : CodeOfPolicies

// stub


.. _function-_at_rtti_c__c_using_CI0_ls_XH_ls_rtti_c__c_CodeOfPolicies_gr__gr_1_ls_v_gr__builtin_:

.. das:function:: using(arg0: block<(var arg0:CodeOfPolicies):void> implicit)

// stub


:Arguments: * **arg0** : block<( :ref:`CodeOfPolicies <handle-rtti-CodeOfPolicies>` ):void> implicit

.. _function-_at_rtti_c__c_using_CI0_ls_XH_ls_rtti_c__c_ModuleGroup_gr__gr_1_ls_v_gr__builtin_:

.. das:function:: using(arg0: block<(var arg0:ModuleGroup):void> implicit)

// stub


:Arguments: * **arg0** : block<( :ref:`ModuleGroup <handle-rtti-ModuleGroup>` ):void> implicit

.. _function-_at_rtti_c__c_RttiValue_nothing:

.. das:function:: RttiValue_nothing() : auto

// stub


+++++++++++
Type access
+++++++++++

  *  :ref:`get_dim (typeinfo: TypeInfo implicit; index: int) : int <function-_at_rtti_c__c_get_dim_CIH_ls_rtti_c__c_TypeInfo_gr__Ci_C_c_C_l>` 
  *  :ref:`get_dim (typeinfo: VarInfo implicit; index: int) : int <function-_at_rtti_c__c_get_dim_CIH_ls_rtti_c__c_VarInfo_gr__Ci_C_c_C_l>` 
  *  :ref:`builtin_is_same_type (a: TypeInfo const? implicit; b: TypeInfo const? implicit; refMatters: RefMatters; cosntMatters: ConstMatters; tempMatters: TemporaryMatters; topLevel: bool) : bool <function-_at_rtti_c__c_builtin_is_same_type_CI1_ls_CH_ls_rtti_c__c_TypeInfo_gr__gr__qm__CI1_ls_CH_ls_rtti_c__c_TypeInfo_gr__gr__qm__CE_ls_rtti_c__c_RefMatters_gr__CE_ls_rtti_c__c_ConstMatters_gr__CE_ls_rtti_c__c_TemporaryMatters_gr__Cb>` 
  *  :ref:`get_type_size (type: TypeInfo? implicit) : int <function-_at_rtti_c__c_get_type_size_CI1_ls_H_ls_rtti_c__c_TypeInfo_gr__gr__qm_>` 
  *  :ref:`get_type_align (type: TypeInfo? implicit) : int <function-_at_rtti_c__c_get_type_align_CI1_ls_H_ls_rtti_c__c_TypeInfo_gr__gr__qm_>` 
  *  :ref:`is_compatible_cast (from: StructInfo const? implicit; to: StructInfo const? implicit) : bool <function-_at_rtti_c__c_is_compatible_cast_CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm_>` 
  *  :ref:`get_das_type_name (type: Type) : string <function-_at_rtti_c__c_get_das_type_name_CE_ls_rtti_c__c_Type_gr__C_c_C_l>` 
  *  :ref:`is_same_type (a: TypeInfo; b: TypeInfo; refMatters: RefMatters = rtti::RefMatters yes; constMatters: ConstMatters = rtti::ConstMatters yes; temporaryMatters: TemporaryMatters = rtti::TemporaryMatters yes; topLevel: bool = true) : auto <function-_at_rtti_c__c_is_same_type_CH_ls_rtti_c__c_TypeInfo_gr__CH_ls_rtti_c__c_TypeInfo_gr__CE_ls_rtti_c__c_RefMatters_gr__CE_ls_rtti_c__c_ConstMatters_gr__CE_ls_rtti_c__c_TemporaryMatters_gr__Cb>` 
  *  :ref:`is_compatible_cast (a: StructInfo; b: StructInfo) : auto <function-_at_rtti_c__c_is_compatible_cast_CH_ls_rtti_c__c_StructInfo_gr__CH_ls_rtti_c__c_StructInfo_gr_>` 
  *  :ref:`each_dim (info: TypeInfo) : auto <function-_at_rtti_c__c_each_dim_CH_ls_rtti_c__c_TypeInfo_gr_>` 
  *  :ref:`each_dim (info: VarInfo) : auto <function-_at_rtti_c__c_each_dim_CH_ls_rtti_c__c_VarInfo_gr_>` 
  *  :ref:`arg_types (info: TypeInfo) : auto <function-_at_rtti_c__c_arg_types_CH_ls_rtti_c__c_TypeInfo_gr_>` 
  *  :ref:`arg_types (info: VarInfo) : auto <function-_at_rtti_c__c_arg_types_CH_ls_rtti_c__c_VarInfo_gr_>` 
  *  :ref:`arg_names (info: TypeInfo) : auto <function-_at_rtti_c__c_arg_names_CH_ls_rtti_c__c_TypeInfo_gr_>` 
  *  :ref:`arg_names (info: VarInfo) : auto <function-_at_rtti_c__c_arg_names_CH_ls_rtti_c__c_VarInfo_gr_>` 

.. _function-_at_rtti_c__c_get_dim_CIH_ls_rtti_c__c_TypeInfo_gr__Ci_C_c_C_l:

.. das:function:: get_dim(typeinfo: TypeInfo implicit; index: int) : int

// stub


:Arguments: * **typeinfo** :  :ref:`TypeInfo <handle-rtti-TypeInfo>`  implicit

            * **index** : int

.. _function-_at_rtti_c__c_get_dim_CIH_ls_rtti_c__c_VarInfo_gr__Ci_C_c_C_l:

.. das:function:: get_dim(typeinfo: VarInfo implicit; index: int) : int

// stub


:Arguments: * **typeinfo** :  :ref:`VarInfo <handle-rtti-VarInfo>`  implicit

            * **index** : int

.. _function-_at_rtti_c__c_builtin_is_same_type_CI1_ls_CH_ls_rtti_c__c_TypeInfo_gr__gr__qm__CI1_ls_CH_ls_rtti_c__c_TypeInfo_gr__gr__qm__CE_ls_rtti_c__c_RefMatters_gr__CE_ls_rtti_c__c_ConstMatters_gr__CE_ls_rtti_c__c_TemporaryMatters_gr__Cb:

.. das:function:: builtin_is_same_type(a: TypeInfo const? implicit; b: TypeInfo const? implicit; refMatters: RefMatters; cosntMatters: ConstMatters; tempMatters: TemporaryMatters; topLevel: bool) : bool

// stub


:Arguments: * **a** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ? implicit

            * **b** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ? implicit

            * **refMatters** :  :ref:`RefMatters <enum-rtti-RefMatters>` 

            * **cosntMatters** :  :ref:`ConstMatters <enum-rtti-ConstMatters>` 

            * **tempMatters** :  :ref:`TemporaryMatters <enum-rtti-TemporaryMatters>` 

            * **topLevel** : bool

.. _function-_at_rtti_c__c_get_type_size_CI1_ls_H_ls_rtti_c__c_TypeInfo_gr__gr__qm_:

.. das:function:: get_type_size(type: TypeInfo? implicit) : int

// stub


:Arguments: * **type** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ? implicit

.. _function-_at_rtti_c__c_get_type_align_CI1_ls_H_ls_rtti_c__c_TypeInfo_gr__gr__qm_:

.. das:function:: get_type_align(type: TypeInfo? implicit) : int

// stub


:Arguments: * **type** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ? implicit

.. _function-_at_rtti_c__c_is_compatible_cast_CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm_:

.. das:function:: is_compatible_cast(from: StructInfo const? implicit; to: StructInfo const? implicit) : bool

// stub


:Arguments: * **from** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

            * **to** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-_at_rtti_c__c_get_das_type_name_CE_ls_rtti_c__c_Type_gr__C_c_C_l:

.. das:function:: get_das_type_name(type: Type) : string

// stub


:Arguments: * **type** :  :ref:`Type <enum-rtti-Type>` 

.. _function-_at_rtti_c__c_is_same_type_CH_ls_rtti_c__c_TypeInfo_gr__CH_ls_rtti_c__c_TypeInfo_gr__CE_ls_rtti_c__c_RefMatters_gr__CE_ls_rtti_c__c_ConstMatters_gr__CE_ls_rtti_c__c_TemporaryMatters_gr__Cb:

.. das:function:: is_same_type(a: TypeInfo; b: TypeInfo; refMatters: RefMatters = rtti::RefMatters yes; constMatters: ConstMatters = rtti::ConstMatters yes; temporaryMatters: TemporaryMatters = rtti::TemporaryMatters yes; topLevel: bool = true) : auto

// stub


:Arguments: * **a** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` 

            * **b** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` 

            * **refMatters** :  :ref:`RefMatters <enum-rtti-RefMatters>` 

            * **constMatters** :  :ref:`ConstMatters <enum-rtti-ConstMatters>` 

            * **temporaryMatters** :  :ref:`TemporaryMatters <enum-rtti-TemporaryMatters>` 

            * **topLevel** : bool

.. _function-_at_rtti_c__c_is_compatible_cast_CH_ls_rtti_c__c_StructInfo_gr__CH_ls_rtti_c__c_StructInfo_gr_:

.. das:function:: is_compatible_cast(a: StructInfo; b: StructInfo) : auto

// stub


:Arguments: * **a** :  :ref:`StructInfo <handle-rtti-StructInfo>` 

            * **b** :  :ref:`StructInfo <handle-rtti-StructInfo>` 

.. _function-_at_rtti_c__c_each_dim_CH_ls_rtti_c__c_TypeInfo_gr_:

.. das:function:: each_dim(info: TypeInfo) : auto

// stub


:Arguments: * **info** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` 

.. _function-_at_rtti_c__c_each_dim_CH_ls_rtti_c__c_VarInfo_gr_:

.. das:function:: each_dim(info: VarInfo) : auto

// stub


:Arguments: * **info** :  :ref:`VarInfo <handle-rtti-VarInfo>` 

.. _function-_at_rtti_c__c_arg_types_CH_ls_rtti_c__c_TypeInfo_gr_:

.. das:function:: arg_types(info: TypeInfo) : auto

// stub


:Arguments: * **info** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` 

.. _function-_at_rtti_c__c_arg_types_CH_ls_rtti_c__c_VarInfo_gr_:

.. das:function:: arg_types(info: VarInfo) : auto

// stub


:Arguments: * **info** :  :ref:`VarInfo <handle-rtti-VarInfo>` 

.. _function-_at_rtti_c__c_arg_names_CH_ls_rtti_c__c_TypeInfo_gr_:

.. das:function:: arg_names(info: TypeInfo) : auto

// stub


:Arguments: * **info** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` 

.. _function-_at_rtti_c__c_arg_names_CH_ls_rtti_c__c_VarInfo_gr_:

.. das:function:: arg_names(info: VarInfo) : auto

// stub


:Arguments: * **info** :  :ref:`VarInfo <handle-rtti-VarInfo>` 

+++++++++++++++++++
Rtti context access
+++++++++++++++++++

  *  :ref:`get_total_functions (context: Context implicit) : int <function-_at_rtti_c__c_get_total_functions_IH_ls_rtti_c__c_Context_gr_>` 
  *  :ref:`get_total_variables (context: Context implicit) : int <function-_at_rtti_c__c_get_total_variables_IH_ls_rtti_c__c_Context_gr_>` 
  *  :ref:`get_function_info (context: any; index: int) : FuncInfo const& <function-_at_rtti_c__c_get_function_info__st__Ci>` 
  *  :ref:`get_variable_info (context: any; index: int) : VarInfo const& <function-_at_rtti_c__c_get_variable_info__st__Ci>` 
  *  :ref:`get_variable_value (varInfo: VarInfo implicit) : RttiValue <function-_at_rtti_c__c_get_variable_value_CIH_ls_rtti_c__c_VarInfo_gr_>` 
  *  :ref:`get_function_info (context: Context implicit; function: function\<\>) : FuncInfo const? <function-_at_rtti_c__c_get_function_info_IH_ls_rtti_c__c_Context_gr__C_at__at_>` 
  *  :ref:`get_function_by_mnh (context: Context implicit; MNH: uint64) : function\<\> <function-_at_rtti_c__c_get_function_by_mnh_IH_ls_rtti_c__c_Context_gr__Cu64>` 
  *  :ref:`get_line_info () : LineInfo <function-_at_rtti_c__c_get_line_info_C_l>` 
  *  :ref:`get_line_info (depth: int) : LineInfo <function-_at_rtti_c__c_get_line_info_Ci_C_c_C_l>` 
  *  :ref:`this_context () : Context& <function-_at_rtti_c__c_this_context_C_c>` 
  *  :ref:`context_for_each_function (blk: block\<(info:FuncInfo const):void\>) : auto <function-_at_rtti_c__c_context_for_each_function_CN_ls_info_gr_0_ls_CH_ls_rtti_c__c_FuncInfo_gr__gr_1_ls_v_gr__builtin_>` 
  *  :ref:`context_for_each_variable (blk: block\<(info:VarInfo const):void\>) : auto <function-_at_rtti_c__c_context_for_each_variable_CN_ls_info_gr_0_ls_CH_ls_rtti_c__c_VarInfo_gr__gr_1_ls_v_gr__builtin_>` 
  *  :ref:`class_info (cl: auto) : StructInfo const? <function-_at_rtti_c__c_class_info_C_dot_>` 
  *  :ref:`type_info (vinfo: LocalVariableInfo) : TypeInfo const? <function-_at_rtti_c__c_type_info_CH_ls_rtti_c__c_LocalVariableInfo_gr_>` 
  *  :ref:`type_info (vinfo: VarInfo) : TypeInfo const? <function-_at_rtti_c__c_type_info_CH_ls_rtti_c__c_VarInfo_gr_>` 

.. _function-_at_rtti_c__c_get_total_functions_IH_ls_rtti_c__c_Context_gr_:

.. das:function:: get_total_functions(context: Context implicit) : int

// stub


:Arguments: * **context** :  :ref:`Context <handle-rtti-Context>`  implicit

.. _function-_at_rtti_c__c_get_total_variables_IH_ls_rtti_c__c_Context_gr_:

.. das:function:: get_total_variables(context: Context implicit) : int

// stub


:Arguments: * **context** :  :ref:`Context <handle-rtti-Context>`  implicit

.. _function-_at_rtti_c__c_get_function_info__st__Ci:

.. das:function:: get_function_info(context: any; index: int) : FuncInfo const&

// stub


:Arguments: * **context** : any

            * **index** : int

.. _function-_at_rtti_c__c_get_variable_info__st__Ci:

.. das:function:: get_variable_info(context: any; index: int) : VarInfo const&

// stub


:Arguments: * **context** : any

            * **index** : int

.. _function-_at_rtti_c__c_get_variable_value_CIH_ls_rtti_c__c_VarInfo_gr_:

.. das:function:: get_variable_value(varInfo: VarInfo implicit) : RttiValue

// stub


:Arguments: * **varInfo** :  :ref:`VarInfo <handle-rtti-VarInfo>`  implicit

.. _function-_at_rtti_c__c_get_function_info_IH_ls_rtti_c__c_Context_gr__C_at__at_:

.. das:function:: get_function_info(context: Context implicit; function: function<>) : FuncInfo const?

// stub


:Arguments: * **context** :  :ref:`Context <handle-rtti-Context>`  implicit

            * **function** : function<void>

.. _function-_at_rtti_c__c_get_function_by_mnh_IH_ls_rtti_c__c_Context_gr__Cu64:

.. das:function:: get_function_by_mnh(context: Context implicit; MNH: uint64) : function<>

// stub


:Arguments: * **context** :  :ref:`Context <handle-rtti-Context>`  implicit

            * **MNH** : uint64

.. _function-_at_rtti_c__c_get_line_info_C_l:

.. das:function:: get_line_info() : LineInfo

// stub


.. _function-_at_rtti_c__c_get_line_info_Ci_C_c_C_l:

.. das:function:: get_line_info(depth: int) : LineInfo

// stub


:Arguments: * **depth** : int

.. _function-_at_rtti_c__c_this_context_C_c:

.. das:function:: this_context() : Context&

// stub


.. _function-_at_rtti_c__c_context_for_each_function_CN_ls_info_gr_0_ls_CH_ls_rtti_c__c_FuncInfo_gr__gr_1_ls_v_gr__builtin_:

.. das:function:: context_for_each_function(blk: block<(info:FuncInfo const):void>) : auto

// stub


:Arguments: * **blk** : block<(info: :ref:`FuncInfo <handle-rtti-FuncInfo>` ):void>

.. _function-_at_rtti_c__c_context_for_each_variable_CN_ls_info_gr_0_ls_CH_ls_rtti_c__c_VarInfo_gr__gr_1_ls_v_gr__builtin_:

.. das:function:: context_for_each_variable(blk: block<(info:VarInfo const):void>) : auto

// stub


:Arguments: * **blk** : block<(info: :ref:`VarInfo <handle-rtti-VarInfo>` ):void>

.. _function-_at_rtti_c__c_class_info_C_dot_:

.. das:function:: class_info(cl: auto) : StructInfo const?

// stub


:Arguments: * **cl** : auto

.. _function-_at_rtti_c__c_type_info_CH_ls_rtti_c__c_LocalVariableInfo_gr_:

.. das:function:: type_info(vinfo: LocalVariableInfo) : TypeInfo const?

// stub


:Arguments: * **vinfo** :  :ref:`LocalVariableInfo <handle-rtti-LocalVariableInfo>` 

.. _function-_at_rtti_c__c_type_info_CH_ls_rtti_c__c_VarInfo_gr_:

.. das:function:: type_info(vinfo: VarInfo) : TypeInfo const?

// stub


:Arguments: * **vinfo** :  :ref:`VarInfo <handle-rtti-VarInfo>` 

++++++++++++++
Program access
++++++++++++++

  *  :ref:`get_this_module (program: smart_ptr\<Program\> implicit) : Module? <function-_at_rtti_c__c_get_this_module_CI1_ls_H_ls_rtti_c__c_Program_gr__gr__qm_M>` 
  *  :ref:`get_module (name: string implicit) : Module? <function-_at_rtti_c__c_get_module_CIs>` 
  *  :ref:`program_for_each_module (program: smart_ptr\<Program\> implicit; block: block\<(var arg0:Module?):void\> implicit) <function-_at_rtti_c__c_program_for_each_module_CI1_ls_H_ls_rtti_c__c_Program_gr__gr__qm_M_CI0_ls_1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`program_for_each_registered_module (block: block\<(var arg0:Module?):void\> implicit) <function-_at_rtti_c__c_program_for_each_registered_module_CI0_ls_1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__gr_1_ls_v_gr__builtin__C_c_C_l>` 

.. _function-_at_rtti_c__c_get_this_module_CI1_ls_H_ls_rtti_c__c_Program_gr__gr__qm_M:

.. das:function:: get_this_module(program: smart_ptr<Program> implicit) : Module?

// stub


:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>` > implicit

.. _function-_at_rtti_c__c_get_module_CIs:

.. das:function:: get_module(name: string implicit) : Module?

// stub


:Arguments: * **name** : string implicit

.. _function-_at_rtti_c__c_program_for_each_module_CI1_ls_H_ls_rtti_c__c_Program_gr__gr__qm_M_CI0_ls_1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: program_for_each_module(program: smart_ptr<Program> implicit; block: block<(var arg0:Module?):void> implicit)

// stub


:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>` > implicit

            * **block** : block<( :ref:`Module <handle-rtti-Module>` ?):void> implicit

.. _function-_at_rtti_c__c_program_for_each_registered_module_CI0_ls_1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: program_for_each_registered_module(block: block<(var arg0:Module?):void> implicit)

// stub


:Arguments: * **block** : block<( :ref:`Module <handle-rtti-Module>` ?):void> implicit

+++++++++++++
Module access
+++++++++++++

  *  :ref:`module_for_each_structure (module: Module? implicit; block: block\<(arg0:StructInfo const):void\> implicit) <function-_at_rtti_c__c_module_for_each_structure_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI0_ls_CH_ls_rtti_c__c_StructInfo_gr__gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`module_for_each_enumeration (module: Module? implicit; block: block\<(arg0:EnumInfo const):void\> implicit) <function-_at_rtti_c__c_module_for_each_enumeration_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI0_ls_CH_ls_rtti_c__c_EnumInfo_gr__gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`module_for_each_function (module: Module? implicit; block: block\<(arg0:FuncInfo const):void\> implicit) <function-_at_rtti_c__c_module_for_each_function_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI0_ls_CH_ls_rtti_c__c_FuncInfo_gr__gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`module_for_each_generic (module: Module? implicit; block: block\<(arg0:FuncInfo const):void\> implicit) <function-_at_rtti_c__c_module_for_each_generic_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI0_ls_CH_ls_rtti_c__c_FuncInfo_gr__gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`module_for_each_global (module: Module? implicit; block: block\<(arg0:VarInfo const):void\> implicit) <function-_at_rtti_c__c_module_for_each_global_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI0_ls_CH_ls_rtti_c__c_VarInfo_gr__gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`module_for_each_annotation (module: Module? implicit; block: block\<(arg0:Annotation const):void\> implicit) <function-_at_rtti_c__c_module_for_each_annotation_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI0_ls_CH_ls_rtti_c__c_Annotation_gr__gr_1_ls_v_gr__builtin__C_c_C_l>` 

.. _function-_at_rtti_c__c_module_for_each_structure_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI0_ls_CH_ls_rtti_c__c_StructInfo_gr__gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: module_for_each_structure(module: Module? implicit; block: block<(arg0:StructInfo const):void> implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **block** : block<( :ref:`StructInfo <handle-rtti-StructInfo>` ):void> implicit

.. _function-_at_rtti_c__c_module_for_each_enumeration_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI0_ls_CH_ls_rtti_c__c_EnumInfo_gr__gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: module_for_each_enumeration(module: Module? implicit; block: block<(arg0:EnumInfo const):void> implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **block** : block<( :ref:`EnumInfo <handle-rtti-EnumInfo>` ):void> implicit

.. _function-_at_rtti_c__c_module_for_each_function_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI0_ls_CH_ls_rtti_c__c_FuncInfo_gr__gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: module_for_each_function(module: Module? implicit; block: block<(arg0:FuncInfo const):void> implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **block** : block<( :ref:`FuncInfo <handle-rtti-FuncInfo>` ):void> implicit

.. _function-_at_rtti_c__c_module_for_each_generic_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI0_ls_CH_ls_rtti_c__c_FuncInfo_gr__gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: module_for_each_generic(module: Module? implicit; block: block<(arg0:FuncInfo const):void> implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **block** : block<( :ref:`FuncInfo <handle-rtti-FuncInfo>` ):void> implicit

.. _function-_at_rtti_c__c_module_for_each_global_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI0_ls_CH_ls_rtti_c__c_VarInfo_gr__gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: module_for_each_global(module: Module? implicit; block: block<(arg0:VarInfo const):void> implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **block** : block<( :ref:`VarInfo <handle-rtti-VarInfo>` ):void> implicit

.. _function-_at_rtti_c__c_module_for_each_annotation_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI0_ls_CH_ls_rtti_c__c_Annotation_gr__gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: module_for_each_annotation(module: Module? implicit; block: block<(arg0:Annotation const):void> implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **block** : block<( :ref:`Annotation <handle-rtti-Annotation>` ):void> implicit

+++++++++++++++++
Annotation access
+++++++++++++++++

  *  :ref:`get_annotation_argument_value (info: AnnotationArgument implicit) : RttiValue <function-_at_rtti_c__c_get_annotation_argument_value_CIH_ls_rtti_c__c_AnnotationArgument_gr__C_c_C_l>` 
  *  :ref:`add_annotation_argument (annotation: AnnotationArgumentList implicit; name: string implicit) : int <function-_at_rtti_c__c_add_annotation_argument_IH_ls_rtti_c__c_AnnotationArgumentList_gr__CIs>` 

.. _function-_at_rtti_c__c_get_annotation_argument_value_CIH_ls_rtti_c__c_AnnotationArgument_gr__C_c_C_l:

.. das:function:: get_annotation_argument_value(info: AnnotationArgument implicit) : RttiValue

// stub


:Arguments: * **info** :  :ref:`AnnotationArgument <handle-rtti-AnnotationArgument>`  implicit

.. _function-_at_rtti_c__c_add_annotation_argument_IH_ls_rtti_c__c_AnnotationArgumentList_gr__CIs:

.. das:function:: add_annotation_argument(annotation: AnnotationArgumentList implicit; name: string implicit) : int

// stub


:Arguments: * **annotation** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  implicit

            * **name** : string implicit

++++++++++++++++++++++++++
Compilation and simulation
++++++++++++++++++++++++++

  *  :ref:`compile (module_name: string implicit; codeText: string implicit; codeOfPolicies: CodeOfPolicies implicit; block: block\<(var arg0:bool;var arg1:smart_ptr\<Program\>;arg2:das_string const):void\> implicit) <function-_at_rtti_c__c_compile_CIs_CIs_CIH_ls_rtti_c__c_CodeOfPolicies_gr__CI0_ls_b;1_ls_H_ls_rtti_c__c_Program_gr__gr__qm_W;CH_ls__builtin__c__c_das_string_gr__gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`compile (module_name: string implicit; codeText: string implicit; codeOfPolicies: CodeOfPolicies implicit; exportAll: bool; block: block\<(var arg0:bool;var arg1:smart_ptr\<Program\>;arg2:das_string const):void\> implicit) <function-_at_rtti_c__c_compile_CIs_CIs_CIH_ls_rtti_c__c_CodeOfPolicies_gr__Cb_CI0_ls_b;1_ls_H_ls_rtti_c__c_Program_gr__gr__qm_W;CH_ls__builtin__c__c_das_string_gr__gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`compile_file (module_name: string implicit; fileAccess: smart_ptr\<FileAccess\> implicit; moduleGroup: ModuleGroup? implicit; codeOfPolicies: CodeOfPolicies implicit; block: block\<(var arg0:bool;var arg1:smart_ptr\<Program\>;arg2:das_string const):void\> implicit) <function-_at_rtti_c__c_compile_file_CIs_CI1_ls_H_ls_rtti_c__c_FileAccess_gr__gr__qm_W_CI1_ls_H_ls_rtti_c__c_ModuleGroup_gr__gr__qm__CIH_ls_rtti_c__c_CodeOfPolicies_gr__CI0_ls_b;1_ls_H_ls_rtti_c__c_Program_gr__gr__qm_W;CH_ls__builtin__c__c_das_string_gr__gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`for_each_expected_error (program: smart_ptr\<Program\> implicit; block: block\<(var arg0:CompilationError;var arg1:int):void\> implicit) <function-_at_rtti_c__c_for_each_expected_error_CI1_ls_H_ls_rtti_c__c_Program_gr__gr__qm_W_CI0_ls_E_ls_rtti_c__c_CompilationError_gr_;i_gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`for_each_require_declaration (program: smart_ptr\<Program\> implicit; block: block\<(var arg0:Module?;arg1:string const#;arg2:string const#;var arg3:bool;arg4:LineInfo const&):void\> implicit) <function-_at_rtti_c__c_for_each_require_declaration_CI1_ls_H_ls_rtti_c__c_Program_gr__gr__qm_W_CI0_ls_1_ls_H_ls_rtti_c__c_Module_gr__gr__qm_;C_hh_s;C_hh_s;b;C&H_ls_rtti_c__c_LineInfo_gr__gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`simulate (program: smart_ptr\<Program\> const& implicit; block: block\<(var arg0:bool;var arg1:smart_ptr\<Context\>;var arg2:das_string):void\> implicit) <function-_at_rtti_c__c_simulate_C&I1_ls_H_ls_rtti_c__c_Program_gr__gr__qm_W_CI0_ls_b;1_ls_H_ls_rtti_c__c_Context_gr__gr__qm_W;H_ls__builtin__c__c_das_string_gr__gr_1_ls_v_gr__builtin__C_c_C_l>` 

.. _function-_at_rtti_c__c_compile_CIs_CIs_CIH_ls_rtti_c__c_CodeOfPolicies_gr__CI0_ls_b;1_ls_H_ls_rtti_c__c_Program_gr__gr__qm_W;CH_ls__builtin__c__c_das_string_gr__gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: compile(module_name: string implicit; codeText: string implicit; codeOfPolicies: CodeOfPolicies implicit; block: block<(var arg0:bool;var arg1:smart_ptr<Program>;arg2:das_string const):void> implicit)

// stub


:Arguments: * **module_name** : string implicit

            * **codeText** : string implicit

            * **codeOfPolicies** :  :ref:`CodeOfPolicies <handle-rtti-CodeOfPolicies>`  implicit

            * **block** : block<(bool;smart_ptr< :ref:`Program <handle-rtti-Program>` >; :ref:`das_string <handle-builtin-das_string>` ):void> implicit

.. _function-_at_rtti_c__c_compile_CIs_CIs_CIH_ls_rtti_c__c_CodeOfPolicies_gr__Cb_CI0_ls_b;1_ls_H_ls_rtti_c__c_Program_gr__gr__qm_W;CH_ls__builtin__c__c_das_string_gr__gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: compile(module_name: string implicit; codeText: string implicit; codeOfPolicies: CodeOfPolicies implicit; exportAll: bool; block: block<(var arg0:bool;var arg1:smart_ptr<Program>;arg2:das_string const):void> implicit)

// stub


:Arguments: * **module_name** : string implicit

            * **codeText** : string implicit

            * **codeOfPolicies** :  :ref:`CodeOfPolicies <handle-rtti-CodeOfPolicies>`  implicit

            * **exportAll** : bool

            * **block** : block<(bool;smart_ptr< :ref:`Program <handle-rtti-Program>` >; :ref:`das_string <handle-builtin-das_string>` ):void> implicit

.. _function-_at_rtti_c__c_compile_file_CIs_CI1_ls_H_ls_rtti_c__c_FileAccess_gr__gr__qm_W_CI1_ls_H_ls_rtti_c__c_ModuleGroup_gr__gr__qm__CIH_ls_rtti_c__c_CodeOfPolicies_gr__CI0_ls_b;1_ls_H_ls_rtti_c__c_Program_gr__gr__qm_W;CH_ls__builtin__c__c_das_string_gr__gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: compile_file(module_name: string implicit; fileAccess: smart_ptr<FileAccess> implicit; moduleGroup: ModuleGroup? implicit; codeOfPolicies: CodeOfPolicies implicit; block: block<(var arg0:bool;var arg1:smart_ptr<Program>;arg2:das_string const):void> implicit)

// stub


:Arguments: * **module_name** : string implicit

            * **fileAccess** : smart_ptr< :ref:`FileAccess <handle-rtti-FileAccess>` > implicit

            * **moduleGroup** :  :ref:`ModuleGroup <handle-rtti-ModuleGroup>` ? implicit

            * **codeOfPolicies** :  :ref:`CodeOfPolicies <handle-rtti-CodeOfPolicies>`  implicit

            * **block** : block<(bool;smart_ptr< :ref:`Program <handle-rtti-Program>` >; :ref:`das_string <handle-builtin-das_string>` ):void> implicit

.. _function-_at_rtti_c__c_for_each_expected_error_CI1_ls_H_ls_rtti_c__c_Program_gr__gr__qm_W_CI0_ls_E_ls_rtti_c__c_CompilationError_gr_;i_gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: for_each_expected_error(program: smart_ptr<Program> implicit; block: block<(var arg0:CompilationError;var arg1:int):void> implicit)

// stub


:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>` > implicit

            * **block** : block<( :ref:`CompilationError <enum-rtti-CompilationError>` ;int):void> implicit

.. _function-_at_rtti_c__c_for_each_require_declaration_CI1_ls_H_ls_rtti_c__c_Program_gr__gr__qm_W_CI0_ls_1_ls_H_ls_rtti_c__c_Module_gr__gr__qm_;C_hh_s;C_hh_s;b;C&H_ls_rtti_c__c_LineInfo_gr__gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: for_each_require_declaration(program: smart_ptr<Program> implicit; block: block<(var arg0:Module?;arg1:string const#;arg2:string const#;var arg3:bool;arg4:LineInfo const&):void> implicit)

// stub


:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>` > implicit

            * **block** : block<( :ref:`Module <handle-rtti-Module>` ?;string#;string#;bool; :ref:`LineInfo <handle-rtti-LineInfo>` &):void> implicit

.. _function-_at_rtti_c__c_simulate_C&I1_ls_H_ls_rtti_c__c_Program_gr__gr__qm_W_CI0_ls_b;1_ls_H_ls_rtti_c__c_Context_gr__gr__qm_W;H_ls__builtin__c__c_das_string_gr__gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: simulate(program: smart_ptr<Program> const& implicit; block: block<(var arg0:bool;var arg1:smart_ptr<Context>;var arg2:das_string):void> implicit)

// stub


:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>` >& implicit

            * **block** : block<(bool;smart_ptr< :ref:`Context <handle-rtti-Context>` >; :ref:`das_string <handle-builtin-das_string>` ):void> implicit

+++++++++++
File access
+++++++++++

  *  :ref:`make_file_access (project: string implicit) : smart_ptr\<FileAccess\> <function-_at_rtti_c__c_make_file_access_CIs_C_c_C_l>` 
  *  :ref:`set_file_source (access: smart_ptr\<FileAccess\> implicit; fileName: string implicit; text: string implicit) : bool <function-_at_rtti_c__c_set_file_source_CI1_ls_H_ls_rtti_c__c_FileAccess_gr__gr__qm_M_CIs_CIs_C_c_C_l>` 
  *  :ref:`add_file_access_root (access: smart_ptr\<FileAccess\> implicit; mod: string implicit; path: string implicit) : bool <function-_at_rtti_c__c_add_file_access_root_CI1_ls_H_ls_rtti_c__c_FileAccess_gr__gr__qm_W_CIs_CIs>` 

.. _function-_at_rtti_c__c_make_file_access_CIs_C_c_C_l:

.. das:function:: make_file_access(project: string implicit) : smart_ptr<FileAccess>

// stub


:Arguments: * **project** : string implicit

.. _function-_at_rtti_c__c_set_file_source_CI1_ls_H_ls_rtti_c__c_FileAccess_gr__gr__qm_M_CIs_CIs_C_c_C_l:

.. das:function:: set_file_source(access: smart_ptr<FileAccess> implicit; fileName: string implicit; text: string implicit) : bool

// stub


:Arguments: * **access** : smart_ptr< :ref:`FileAccess <handle-rtti-FileAccess>` > implicit

            * **fileName** : string implicit

            * **text** : string implicit

.. _function-_at_rtti_c__c_add_file_access_root_CI1_ls_H_ls_rtti_c__c_FileAccess_gr__gr__qm_W_CIs_CIs:

.. das:function:: add_file_access_root(access: smart_ptr<FileAccess> implicit; mod: string implicit; path: string implicit) : bool

// stub


:Arguments: * **access** : smart_ptr< :ref:`FileAccess <handle-rtti-FileAccess>` > implicit

            * **mod** : string implicit

            * **path** : string implicit

++++++++++++++++
Structure access
++++++++++++++++

  *  :ref:`rtti_builtin_structure_for_each_annotation (struct: StructInfo implicit; block: block\<\> implicit) <function-_at_rtti_c__c_rtti_builtin_structure_for_each_annotation_CIH_ls_rtti_c__c_StructInfo_gr__CI_builtin__C_c_C_l>` 
  *  :ref:`basic_struct_for_each_field (annotation: BasicStructureAnnotation implicit; block: block\<(var arg0:string;var arg1:string;arg2:TypeInfo const;var arg3:uint):void\> implicit) <function-_at_rtti_c__c_basic_struct_for_each_field_CIH_ls_rtti_c__c_BasicStructureAnnotation_gr__CI0_ls_s;s;CH_ls_rtti_c__c_TypeInfo_gr_;u_gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`basic_struct_for_each_parent (annotation: BasicStructureAnnotation implicit; block: block\<(var arg0:Annotation?):void\> implicit) <function-_at_rtti_c__c_basic_struct_for_each_parent_CIH_ls_rtti_c__c_BasicStructureAnnotation_gr__CI0_ls_1_ls_H_ls_rtti_c__c_Annotation_gr__gr__qm__gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`structure_for_each_annotation (st: StructInfo; subexpr: block\<(ann:Annotation const;args:AnnotationArguments const):void\>) : auto <function-_at_rtti_c__c_structure_for_each_annotation_CH_ls_rtti_c__c_StructInfo_gr__CN_ls_ann;args_gr_0_ls_CH_ls_rtti_c__c_Annotation_gr_;CH_ls_rtti_c__c_AnnotationArguments_gr__gr_1_ls_v_gr__builtin_>` 

.. _function-_at_rtti_c__c_rtti_builtin_structure_for_each_annotation_CIH_ls_rtti_c__c_StructInfo_gr__CI_builtin__C_c_C_l:

.. das:function:: rtti_builtin_structure_for_each_annotation(struct: StructInfo implicit; block: block<> implicit)

// stub


:Arguments: * **struct** :  :ref:`StructInfo <handle-rtti-StructInfo>`  implicit

            * **block** : block<void> implicit

.. _function-_at_rtti_c__c_basic_struct_for_each_field_CIH_ls_rtti_c__c_BasicStructureAnnotation_gr__CI0_ls_s;s;CH_ls_rtti_c__c_TypeInfo_gr_;u_gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: basic_struct_for_each_field(annotation: BasicStructureAnnotation implicit; block: block<(var arg0:string;var arg1:string;arg2:TypeInfo const;var arg3:uint):void> implicit)

// stub


:Arguments: * **annotation** :  :ref:`BasicStructureAnnotation <handle-rtti-BasicStructureAnnotation>`  implicit

            * **block** : block<(string;string; :ref:`TypeInfo <handle-rtti-TypeInfo>` ;uint):void> implicit

.. _function-_at_rtti_c__c_basic_struct_for_each_parent_CIH_ls_rtti_c__c_BasicStructureAnnotation_gr__CI0_ls_1_ls_H_ls_rtti_c__c_Annotation_gr__gr__qm__gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: basic_struct_for_each_parent(annotation: BasicStructureAnnotation implicit; block: block<(var arg0:Annotation?):void> implicit)

// stub


:Arguments: * **annotation** :  :ref:`BasicStructureAnnotation <handle-rtti-BasicStructureAnnotation>`  implicit

            * **block** : block<( :ref:`Annotation <handle-rtti-Annotation>` ?):void> implicit

.. _function-_at_rtti_c__c_structure_for_each_annotation_CH_ls_rtti_c__c_StructInfo_gr__CN_ls_ann;args_gr_0_ls_CH_ls_rtti_c__c_Annotation_gr_;CH_ls_rtti_c__c_AnnotationArguments_gr__gr_1_ls_v_gr__builtin_:

.. das:function:: structure_for_each_annotation(st: StructInfo; subexpr: block<(ann:Annotation const;args:AnnotationArguments const):void>) : auto

// stub


:Arguments: * **st** :  :ref:`StructInfo <handle-rtti-StructInfo>` 

            * **subexpr** : block<(ann: :ref:`Annotation <handle-rtti-Annotation>` ;args: :ref:`AnnotationArguments <handle-rtti-AnnotationArguments>` ):void>

+++++++++++++++++++++++++
Data walking and printing
+++++++++++++++++++++++++

  *  :ref:`sprint_data (data: void? implicit; type: TypeInfo const? implicit; flags: bitfield) : string <function-_at_rtti_c__c_sprint_data_CI_qm__CI1_ls_CH_ls_rtti_c__c_TypeInfo_gr__gr__qm__Ct_C_c_C_l>` 
  *  :ref:`sprint_data (data: float4; type: TypeInfo const? implicit; flags: bitfield) : string <function-_at_rtti_c__c_sprint_data_Cf4_CI1_ls_CH_ls_rtti_c__c_TypeInfo_gr__gr__qm__Ct_C_c_C_l>` 
  *  :ref:`describe (type: TypeInfo const? implicit) : string <function-_at_rtti_c__c_describe_CI1_ls_CH_ls_rtti_c__c_TypeInfo_gr__gr__qm__C_c_C_l>` 
  *  :ref:`describe (lineinfo: LineInfo implicit; fully: bool = false) : string <function-_at_rtti_c__c_describe_CIH_ls_rtti_c__c_LineInfo_gr__Cb_C_c_C_l>` 
  *  :ref:`get_mangled_name (type: TypeInfo const? implicit) : string <function-_at_rtti_c__c_get_mangled_name_CI1_ls_CH_ls_rtti_c__c_TypeInfo_gr__gr__qm__C_c_C_l>` 

.. _function-_at_rtti_c__c_sprint_data_CI_qm__CI1_ls_CH_ls_rtti_c__c_TypeInfo_gr__gr__qm__Ct_C_c_C_l:

.. das:function:: sprint_data(data: void? implicit; type: TypeInfo const? implicit; flags: bitfield) : string

// stub


:Arguments: * **data** : void? implicit

            * **type** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ? implicit

            * **flags** : bitfield<>

.. _function-_at_rtti_c__c_sprint_data_Cf4_CI1_ls_CH_ls_rtti_c__c_TypeInfo_gr__gr__qm__Ct_C_c_C_l:

.. das:function:: sprint_data(data: float4; type: TypeInfo const? implicit; flags: bitfield) : string

// stub


:Arguments: * **data** : float4

            * **type** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ? implicit

            * **flags** : bitfield<>

.. _function-_at_rtti_c__c_describe_CI1_ls_CH_ls_rtti_c__c_TypeInfo_gr__gr__qm__C_c_C_l:

.. das:function:: describe(type: TypeInfo const? implicit) : string

// stub


:Arguments: * **type** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ? implicit

.. _function-_at_rtti_c__c_describe_CIH_ls_rtti_c__c_LineInfo_gr__Cb_C_c_C_l:

.. das:function:: describe(lineinfo: LineInfo implicit; fully: bool = false) : string

// stub


:Arguments: * **lineinfo** :  :ref:`LineInfo <handle-rtti-LineInfo>`  implicit

            * **fully** : bool

.. _function-_at_rtti_c__c_get_mangled_name_CI1_ls_CH_ls_rtti_c__c_TypeInfo_gr__gr__qm__C_c_C_l:

.. das:function:: get_mangled_name(type: TypeInfo const? implicit) : string

// stub


:Arguments: * **type** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ? implicit

++++++++++++++++++++++++++++++
Function and mangled name hash
++++++++++++++++++++++++++++++

  *  :ref:`get_function_by_mangled_name_hash (src: uint64) : function\<\> <function-_at_rtti_c__c_get_function_by_mangled_name_hash_Cu64_C_c>` 
  *  :ref:`get_function_by_mangled_name_hash (src: uint64; context: Context implicit) : function\<\> <function-_at_rtti_c__c_get_function_by_mangled_name_hash_Cu64_IH_ls_rtti_c__c_Context_gr_>` 
  *  :ref:`get_function_mangled_name_hash (src: function\<\>) : uint64 <function-_at_rtti_c__c_get_function_mangled_name_hash_C_at__at__C_c>` 
  *  :ref:`get_function_address (MNH: uint64; at: Context implicit) : uint64 <function-_at_rtti_c__c_get_function_address_Cu64_IH_ls_rtti_c__c_Context_gr_>` 

.. _function-_at_rtti_c__c_get_function_by_mangled_name_hash_Cu64_C_c:

.. das:function:: get_function_by_mangled_name_hash(src: uint64) : function<>

// stub


:Arguments: * **src** : uint64

.. _function-_at_rtti_c__c_get_function_by_mangled_name_hash_Cu64_IH_ls_rtti_c__c_Context_gr_:

.. das:function:: get_function_by_mangled_name_hash(src: uint64; context: Context implicit) : function<>

// stub


:Arguments: * **src** : uint64

            * **context** :  :ref:`Context <handle-rtti-Context>`  implicit

.. _function-_at_rtti_c__c_get_function_mangled_name_hash_C_at__at__C_c:

.. das:function:: get_function_mangled_name_hash(src: function<>) : uint64

// stub


:Arguments: * **src** : function<void>

.. _function-_at_rtti_c__c_get_function_address_Cu64_IH_ls_rtti_c__c_Context_gr_:

.. das:function:: get_function_address(MNH: uint64; at: Context implicit) : uint64

// stub


:Arguments: * **MNH** : uint64

            * **at** :  :ref:`Context <handle-rtti-Context>`  implicit

+++++++++++++++++++++++++
Context and mutex locking
+++++++++++++++++++++++++

  *  :ref:`lock_this_context (block: block\<void\> implicit) <function-_at_rtti_c__c_lock_this_context_CI1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`lock_context (lock_context: Context implicit; block: block\<void\> implicit) <function-_at_rtti_c__c_lock_context_IH_ls_rtti_c__c_Context_gr__CI1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`lock_mutex (mutex: recursive_mutex implicit; block: block\<void\> implicit) <function-_at_rtti_c__c_lock_mutex_IH_ls_rtti_c__c_recursive_mutex_gr__CI1_ls_v_gr__builtin__C_c_C_l>` 

.. _function-_at_rtti_c__c_lock_this_context_CI1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: lock_this_context(block: block<void> implicit)

// stub


:Arguments: * **block** : block<void> implicit

.. _function-_at_rtti_c__c_lock_context_IH_ls_rtti_c__c_Context_gr__CI1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: lock_context(lock_context: Context implicit; block: block<void> implicit)

// stub


:Arguments: * **lock_context** :  :ref:`Context <handle-rtti-Context>`  implicit

            * **block** : block<void> implicit

.. _function-_at_rtti_c__c_lock_mutex_IH_ls_rtti_c__c_recursive_mutex_gr__CI1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: lock_mutex(mutex: recursive_mutex implicit; block: block<void> implicit)

// stub


:Arguments: * **mutex** :  :ref:`recursive_mutex <handle-rtti-recursive_mutex>`  implicit

            * **block** : block<void> implicit

+++++++++++++++++++
Runtime data access
+++++++++++++++++++

  *  :ref:`get_table_key_index (table: void? implicit; key: any; baseType: Type; valueTypeSize: int) : int <function-_at_rtti_c__c_get_table_key_index_CI_qm___st__CE_ls_rtti_c__c_Type_gr__Ci_C_c_C_l>` 

.. _function-_at_rtti_c__c_get_table_key_index_CI_qm___st__CE_ls_rtti_c__c_Type_gr__Ci_C_c_C_l:

.. das:function:: get_table_key_index(table: void? implicit; key: any; baseType: Type; valueTypeSize: int) : int

// stub


:Arguments: * **table** : void? implicit

            * **key** : any

            * **baseType** :  :ref:`Type <enum-rtti-Type>` 

            * **valueTypeSize** : int

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_rtti_c__c_module_for_each_dependency_CI1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__CI0_ls_1_ls_H_ls_rtti_c__c_Module_gr__gr__qm_;b_gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: module_for_each_dependency(module: Module? implicit; block: block<(var arg0:Module?;var arg1:bool):void> implicit)

// stub


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **block** : block<( :ref:`Module <handle-rtti-Module>` ?;bool):void> implicit

.. _function-_at_rtti_c__c_get_tuple_field_offset_CI1_ls_H_ls_rtti_c__c_TypeInfo_gr__gr__qm__Ci:

.. das:function:: get_tuple_field_offset(type: TypeInfo? implicit; index: int) : int

// stub


:Arguments: * **type** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ? implicit

            * **index** : int

.. _function-_at_rtti_c__c_get_variant_field_offset_CI1_ls_H_ls_rtti_c__c_TypeInfo_gr__gr__qm__Ci:

.. das:function:: get_variant_field_offset(type: TypeInfo? implicit; index: int) : int

// stub


:Arguments: * **type** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ? implicit

            * **index** : int

.. _function-_at_rtti_c__c_each_I_eq_H_ls_rtti_c__c_FuncInfo_gr__C_c_C_l:

.. das:function:: each(info: FuncInfo implicit ==const) : iterator<VarInfo&>

// stub


:Arguments: * **info** :  :ref:`FuncInfo <handle-rtti-FuncInfo>`  implicit!

.. _function-_at_rtti_c__c_each_CI_eq_H_ls_rtti_c__c_FuncInfo_gr__C_c_C_l:

.. das:function:: each(info: FuncInfo const implicit ==const) : iterator<VarInfo const&>

// stub


:Arguments: * **info** :  :ref:`FuncInfo <handle-rtti-FuncInfo>`  implicit!

.. _function-_at_rtti_c__c_each_I_eq_H_ls_rtti_c__c_StructInfo_gr__C_c_C_l:

.. das:function:: each(info: StructInfo implicit ==const) : iterator<VarInfo&>

// stub


:Arguments: * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>`  implicit!

.. _function-_at_rtti_c__c_each_CI_eq_H_ls_rtti_c__c_StructInfo_gr__C_c_C_l:

.. das:function:: each(info: StructInfo const implicit ==const) : iterator<VarInfo const&>

// stub


:Arguments: * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>`  implicit!

.. _function-_at_rtti_c__c_each_I_eq_H_ls_rtti_c__c_EnumInfo_gr__C_c_C_l:

.. das:function:: each(info: EnumInfo implicit ==const) : iterator<EnumValueInfo&>

// stub


:Arguments: * **info** :  :ref:`EnumInfo <handle-rtti-EnumInfo>`  implicit!

.. _function-_at_rtti_c__c_each_CI_eq_H_ls_rtti_c__c_EnumInfo_gr__C_c_C_l:

.. das:function:: each(info: EnumInfo const implicit ==const) : iterator<EnumValueInfo const&>

// stub


:Arguments: * **info** :  :ref:`EnumInfo <handle-rtti-EnumInfo>`  implicit!


