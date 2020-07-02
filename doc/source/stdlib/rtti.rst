
.. _stdlib_rtti:

================================
Runtime type information library
================================

.. include:: detail/rtti.rst

|module-rtti|

++++++++++++
Type aliases
++++++++++++

.. _alias-FileAccessPtr:

.. das:attribute:: FileAccessPtr = smart_ptr<rtti::FileAccess>

|typedef-rtti-FileAccessPtr|

.. _alias-ProgramFlags:

.. das:attribute:: ProgramFlags is a bitfield

+-----------------+---+-----+
+field            +bit+value+
+=================+===+=====+
+failToCompile    +0  +1    +
+-----------------+---+-----+
+unsafe           +1  +2    +
+-----------------+---+-----+
+isCompiling      +2  +4    +
+-----------------+---+-----+
+isSimulating     +3  +8    +
+-----------------+---+-----+
+isCompilingMacros+4  +16   +
+-----------------+---+-----+
+needMacroModule  +5  +32   +
+-----------------+---+-----+


|typedef-rtti-ProgramFlags|

.. _alias-RttiValue:

.. das:attribute:: RttiValueis a variant type

+-------+------+
+tBool  +bool  +
+-------+------+
+tInt   +int   +
+-------+------+
+tUInt  +uint  +
+-------+------+
+tInt64 +int64 +
+-------+------+
+tUInt64+uint64+
+-------+------+
+tFloat +float +
+-------+------+
+tDouble+double+
+-------+------+
+tString+string+
+-------+------+
+nothing+any   +
+-------+------+


|typedef-rtti-RttiValue|

+++++++++
Constants
+++++++++

.. _global-rtti-FUNCINFO_BUILTIN:

.. das:attribute:: FUNCINFO_BUILTIN = 0x2

|variable-rtti-FUNCINFO_BUILTIN|

.. _global-rtti-FUNCINFO_INIT:

.. das:attribute:: FUNCINFO_INIT = 0x1

|variable-rtti-FUNCINFO_INIT|

++++++++++++
Enumerations
++++++++++++

.. _enum-rtti-CompilationError:

.. das:attribute:: CompilationError

+-----------------------------------+-----+
+unspecified                        +0    +
+-----------------------------------+-----+
+mismatching_parentheses            +10001+
+-----------------------------------+-----+
+mismatching_curly_bracers          +10002+
+-----------------------------------+-----+
+string_constant_exceeds_file       +10003+
+-----------------------------------+-----+
+string_constant_exceeds_line       +10004+
+-----------------------------------+-----+
+unexpected_close_comment           +10005+
+-----------------------------------+-----+
+integer_constant_out_of_range      +10006+
+-----------------------------------+-----+
+comment_contains_eof               +10007+
+-----------------------------------+-----+
+invalid_escape_sequence            +10008+
+-----------------------------------+-----+
+invalid_line_directive             +10009+
+-----------------------------------+-----+
+syntax_error                       +20000+
+-----------------------------------+-----+
+invalid_type                       +30101+
+-----------------------------------+-----+
+invalid_return_type                +30102+
+-----------------------------------+-----+
+invalid_argument_type              +30103+
+-----------------------------------+-----+
+invalid_structure_field_type       +30104+
+-----------------------------------+-----+
+invalid_array_type                 +30105+
+-----------------------------------+-----+
+invalid_table_type                 +30106+
+-----------------------------------+-----+
+invalid_argument_count             +30107+
+-----------------------------------+-----+
+invalid_variable_type              +30108+
+-----------------------------------+-----+
+invalid_new_type                   +30109+
+-----------------------------------+-----+
+invalid_index_type                 +30110+
+-----------------------------------+-----+
+invalid_annotation                 +30111+
+-----------------------------------+-----+
+invalid_swizzle_mask               +30112+
+-----------------------------------+-----+
+invalid_initialization_type        +30113+
+-----------------------------------+-----+
+invalid_with_type                  +30114+
+-----------------------------------+-----+
+invalid_override                   +30115+
+-----------------------------------+-----+
+invalid_name                       +30116+
+-----------------------------------+-----+
+invalid_array_dimension            +30117+
+-----------------------------------+-----+
+invalid_iteration_source           +30118+
+-----------------------------------+-----+
+invalid_loop                       +30119+
+-----------------------------------+-----+
+invalid_label                      +30120+
+-----------------------------------+-----+
+invalid_enumeration                +30121+
+-----------------------------------+-----+
+invalid_option                     +30122+
+-----------------------------------+-----+
+invalid_member_function            +30123+
+-----------------------------------+-----+
+function_already_declared          +30201+
+-----------------------------------+-----+
+argument_already_declared          +30202+
+-----------------------------------+-----+
+local_variable_already_declared    +30203+
+-----------------------------------+-----+
+global_variable_already_declared   +30204+
+-----------------------------------+-----+
+structure_field_already_declared   +30205+
+-----------------------------------+-----+
+structure_already_declared         +30206+
+-----------------------------------+-----+
+structure_already_has_initializer  +30207+
+-----------------------------------+-----+
+enumeration_already_declared       +30208+
+-----------------------------------+-----+
+enumeration_value_already_declared +30209+
+-----------------------------------+-----+
+type_alias_already_declared        +30210+
+-----------------------------------+-----+
+field_already_initialized          +30211+
+-----------------------------------+-----+
+type_not_found                     +30301+
+-----------------------------------+-----+
+structure_not_found                +30302+
+-----------------------------------+-----+
+operator_not_found                 +30303+
+-----------------------------------+-----+
+function_not_found                 +30304+
+-----------------------------------+-----+
+variable_not_found                 +30305+
+-----------------------------------+-----+
+handle_not_found                   +30306+
+-----------------------------------+-----+
+annotation_not_found               +30307+
+-----------------------------------+-----+
+enumeration_not_found              +30308+
+-----------------------------------+-----+
+enumeration_value_not_found        +30309+
+-----------------------------------+-----+
+type_alias_not_found               +30310+
+-----------------------------------+-----+
+bitfield_not_found                 +30311+
+-----------------------------------+-----+
+cant_initialize                    +30401+
+-----------------------------------+-----+
+cant_dereference                   +30501+
+-----------------------------------+-----+
+cant_index                         +30502+
+-----------------------------------+-----+
+cant_get_field                     +30503+
+-----------------------------------+-----+
+cant_write_to_const                +30504+
+-----------------------------------+-----+
+cant_move_to_const                 +30505+
+-----------------------------------+-----+
+cant_write_to_non_reference        +30506+
+-----------------------------------+-----+
+cant_copy                          +30507+
+-----------------------------------+-----+
+cant_move                          +30508+
+-----------------------------------+-----+
+cant_pass_temporary                +30509+
+-----------------------------------+-----+
+condition_must_be_bool             +30601+
+-----------------------------------+-----+
+condition_must_be_static           +30602+
+-----------------------------------+-----+
+cant_pipe                          +30701+
+-----------------------------------+-----+
+invalid_block                      +30801+
+-----------------------------------+-----+
+return_or_break_in_finally         +30802+
+-----------------------------------+-----+
+module_not_found                   +30901+
+-----------------------------------+-----+
+module_already_has_a_name          +30902+
+-----------------------------------+-----+
+cant_new_handle                    +31001+
+-----------------------------------+-----+
+bad_delete                         +31002+
+-----------------------------------+-----+
+cant_infer_generic                 +31100+
+-----------------------------------+-----+
+cant_infer_missing_initializer     +31101+
+-----------------------------------+-----+
+cant_infer_mismatching_restrictions+31102+
+-----------------------------------+-----+
+invalid_cast                       +31200+
+-----------------------------------+-----+
+incompatible_cast                  +31201+
+-----------------------------------+-----+
+unsafe                             +31300+
+-----------------------------------+-----+
+index_out_of_range                 +31400+
+-----------------------------------+-----+
+expecting_return_value             +32101+
+-----------------------------------+-----+
+not_expecting_return_value         +32102+
+-----------------------------------+-----+
+invalid_return_semantics           +32103+
+-----------------------------------+-----+
+invalid_yield                      +32104+
+-----------------------------------+-----+
+typeinfo_reference                 +39901+
+-----------------------------------+-----+
+typeinfo_auto                      +39902+
+-----------------------------------+-----+
+typeinfo_undefined                 +39903+
+-----------------------------------+-----+
+typeinfo_dim                       +39904+
+-----------------------------------+-----+
+typeinfo_macro_error               +39905+
+-----------------------------------+-----+
+static_assert_failed               +40100+
+-----------------------------------+-----+
+run_failed                         +40101+
+-----------------------------------+-----+
+annotation_failed                  +40102+
+-----------------------------------+-----+
+concept_failed                     +40103+
+-----------------------------------+-----+
+not_all_paths_return_value         +40200+
+-----------------------------------+-----+
+assert_with_side_effects           +40201+
+-----------------------------------+-----+
+only_fast_aot_no_cpp_name          +40202+
+-----------------------------------+-----+
+aot_side_effects                   +40203+
+-----------------------------------+-----+
+no_global_heap                     +40204+
+-----------------------------------+-----+
+no_global_variables                +40205+
+-----------------------------------+-----+
+unused_function_argument           +40206+
+-----------------------------------+-----+
+unsafe_function                    +40207+
+-----------------------------------+-----+
+too_many_infer_passes              +41000+
+-----------------------------------+-----+
+missing_node                       +50100+
+-----------------------------------+-----+


|enumeration-rtti-CompilationError|

.. _enum-rtti-ConstMatters:

.. das:attribute:: ConstMatters

+---+-+
+no +0+
+---+-+
+yes+1+
+---+-+


|enumeration-rtti-ConstMatters|

.. _enum-rtti-RefMatters:

.. das:attribute:: RefMatters

+---+-+
+no +0+
+---+-+
+yes+1+
+---+-+


|enumeration-rtti-RefMatters|

.. _enum-rtti-TemporaryMatters:

.. das:attribute:: TemporaryMatters

+---+-+
+no +0+
+---+-+
+yes+1+
+---+-+


|enumeration-rtti-TemporaryMatters|

.. _enum-rtti-Type:

.. das:attribute:: Type

+--------------+--+
+none          +0 +
+--------------+--+
+autoinfer     +1 +
+--------------+--+
+alias         +2 +
+--------------+--+
+fakeContext   +3 +
+--------------+--+
+fakeLineInfo  +4 +
+--------------+--+
+anyArgument   +5 +
+--------------+--+
+tVoid         +6 +
+--------------+--+
+tBool         +7 +
+--------------+--+
+tInt64        +12+
+--------------+--+
+tUInt64       +13+
+--------------+--+
+tInt          +14+
+--------------+--+
+tInt2         +15+
+--------------+--+
+tInt3         +16+
+--------------+--+
+tInt4         +17+
+--------------+--+
+tUInt         +18+
+--------------+--+
+tUInt2        +19+
+--------------+--+
+tUInt3        +20+
+--------------+--+
+tUInt4        +21+
+--------------+--+
+tFloat        +22+
+--------------+--+
+tFloat2       +23+
+--------------+--+
+tFloat3       +24+
+--------------+--+
+tFloat4       +25+
+--------------+--+
+tDouble       +26+
+--------------+--+
+tRange        +27+
+--------------+--+
+tURange       +28+
+--------------+--+
+tString       +29+
+--------------+--+
+tStructure    +30+
+--------------+--+
+tHandle       +31+
+--------------+--+
+tEnumeration  +32+
+--------------+--+
+tPointer      +36+
+--------------+--+
+tFunction     +37+
+--------------+--+
+tLambda       +38+
+--------------+--+
+tIterator     +39+
+--------------+--+
+tArray        +40+
+--------------+--+
+tTable        +41+
+--------------+--+
+tBlock        +42+
+--------------+--+
+tInt8         +8 +
+--------------+--+
+tUInt8        +9 +
+--------------+--+
+tInt16        +10+
+--------------+--+
+tUInt16       +11+
+--------------+--+
+tTuple        +43+
+--------------+--+
+tEnumeration8 +33+
+--------------+--+
+tEnumeration16+34+
+--------------+--+
+tVariant      +44+
+--------------+--+
+tBitfield     +35+
+--------------+--+


|enumeration-rtti-Type|

++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-rtti-Annotation:

.. das:attribute:: Annotation

Annotation fields are

+-------+--------------------------------------------------------+
+_module+ :ref:`rtti::Module <handle-rtti-Module>` ?             +
+-------+--------------------------------------------------------+
+cppName+ :ref:`builtin::das_string <handle-builtin-das_string>` +
+-------+--------------------------------------------------------+
+name   + :ref:`builtin::das_string <handle-builtin-das_string>` +
+-------+--------------------------------------------------------+


Annotation properties are

+--------------------------+----+
+isTypeAnnotation          +bool+
+--------------------------+----+
+isBasicStructureAnnotation+bool+
+--------------------------+----+


|structure_annotation-rtti-Annotation|

.. _handle-rtti-AnnotationArgument:

.. das:attribute:: AnnotationArgument

AnnotationArgument fields are

+---------+--------------------------------------------------------+
+fValue   +float                                                   +
+---------+--------------------------------------------------------+
+iValue   +int                                                     +
+---------+--------------------------------------------------------+
+name     + :ref:`builtin::das_string <handle-builtin-das_string>` +
+---------+--------------------------------------------------------+
+sValue   + :ref:`builtin::das_string <handle-builtin-das_string>` +
+---------+--------------------------------------------------------+
+bValue   +bool                                                    +
+---------+--------------------------------------------------------+
+basicType+ :ref:`rtti::Type <enum-rtti-Type>`                     +
+---------+--------------------------------------------------------+


|structure_annotation-rtti-AnnotationArgument|

.. _handle-rtti-AnnotationDeclaration:

.. das:attribute:: AnnotationDeclaration

AnnotationDeclaration fields are

+----------+--------------------------------------------------------------------------+
+annotation+smart_ptr< :ref:`rtti::Annotation <handle-rtti-Annotation>` >             +
+----------+--------------------------------------------------------------------------+
+arguments + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` +
+----------+--------------------------------------------------------------------------+


|structure_annotation-rtti-AnnotationDeclaration|

.. _handle-rtti-BasicStructureAnnotation:

.. das:attribute:: BasicStructureAnnotation

BasicStructureAnnotation fields are

+-------+--------------------------------------------------------+
+cppName+ :ref:`builtin::das_string <handle-builtin-das_string>` +
+-------+--------------------------------------------------------+
+name   + :ref:`builtin::das_string <handle-builtin-das_string>` +
+-------+--------------------------------------------------------+


BasicStructureAnnotation properties are

+----------+---+
+fieldCount+int+
+----------+---+


|structure_annotation-rtti-BasicStructureAnnotation|

.. _handle-rtti-EnumInfo:

.. das:attribute:: EnumInfo

EnumInfo fields are

+-----+------+
+count+uint  +
+-----+------+
+name +string+
+-----+------+
+hash +uint  +
+-----+------+


|structure_annotation-rtti-EnumInfo|

.. _handle-rtti-EnumValueInfo:

.. das:attribute:: EnumValueInfo

EnumValueInfo fields are

+-----+------+
+value+int64 +
+-----+------+
+name +string+
+-----+------+


|structure_annotation-rtti-EnumValueInfo|

.. _handle-rtti-Error:

.. das:attribute:: Error

Error fields are

+-----+------------------------------------------------------------+
+fixme+ :ref:`builtin::das_string <handle-builtin-das_string>`     +
+-----+------------------------------------------------------------+
+at   + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`               +
+-----+------------------------------------------------------------+
+what + :ref:`builtin::das_string <handle-builtin-das_string>`     +
+-----+------------------------------------------------------------+
+extra+ :ref:`builtin::das_string <handle-builtin-das_string>`     +
+-----+------------------------------------------------------------+
+cerr + :ref:`rtti::CompilationError <enum-rtti-CompilationError>` +
+-----+------------------------------------------------------------+


|structure_annotation-rtti-Error|

.. _handle-rtti-FileAccess:

.. das:attribute:: FileAccess

|structure_annotation-rtti-FileAccess|

.. _handle-rtti-FileInfo:

.. das:attribute:: FileInfo

FileInfo fields are

+------------+--------------------------------------------------------+
+tabSize     +int                                                     +
+------------+--------------------------------------------------------+
+name        + :ref:`builtin::das_string <handle-builtin-das_string>` +
+------------+--------------------------------------------------------+
+sourceLength+uint                                                    +
+------------+--------------------------------------------------------+


FileInfo properties are

+------+------------+
+source+string const+
+------+------------+


|structure_annotation-rtti-FileInfo|

.. _handle-rtti-FuncInfo:

.. das:attribute:: FuncInfo

FuncInfo fields are

+---------+-----------------------------------------------+
+stackSize+uint                                           +
+---------+-----------------------------------------------+
+result   + :ref:`rtti::TypeInfo <handle-rtti-TypeInfo>` ?+
+---------+-----------------------------------------------+
+count    +uint                                           +
+---------+-----------------------------------------------+
+cppName  +string                                         +
+---------+-----------------------------------------------+
+name     +string                                         +
+---------+-----------------------------------------------+
+hash     +uint                                           +
+---------+-----------------------------------------------+
+flags    +uint                                           +
+---------+-----------------------------------------------+


|structure_annotation-rtti-FuncInfo|

.. _handle-rtti-LineInfo:

.. das:attribute:: LineInfo

LineInfo fields are

+-----------+-----------------------------------------------+
+last_column+uint                                           +
+-----------+-----------------------------------------------+
+line       +uint                                           +
+-----------+-----------------------------------------------+
+last_line  +uint                                           +
+-----------+-----------------------------------------------+
+column     +uint                                           +
+-----------+-----------------------------------------------+
+fileInfo   + :ref:`rtti::FileInfo <handle-rtti-FileInfo>` ?+
+-----------+-----------------------------------------------+


|structure_annotation-rtti-LineInfo|

.. _handle-rtti-Module:

.. das:attribute:: Module

Module fields are

+----+--------------------------------------------------------+
+name+ :ref:`builtin::das_string <handle-builtin-das_string>` +
+----+--------------------------------------------------------+


|structure_annotation-rtti-Module|

.. _handle-rtti-Program:

.. das:attribute:: Program

Program fields are

+------+------------------------------------------------------------+
+flags + :ref:`ProgramFlags <alias-ProgramFlags>`                   +
+------+------------------------------------------------------------+
+errors+ :ref:`rtti::dasvector`Error <handle-rtti-dasvector`Error>` +
+------+------------------------------------------------------------+


|structure_annotation-rtti-Program|

.. _handle-rtti-StructInfo:

.. das:attribute:: StructInfo

StructInfo fields are

+-----------+------+
+size       +uint  +
+-----------+------+
+initializer+int   +
+-----------+------+
+count      +uint  +
+-----------+------+
+name       +string+
+-----------+------+
+hash       +uint  +
+-----------+------+


|structure_annotation-rtti-StructInfo|

.. _handle-rtti-TypeAnnotation:

.. das:attribute:: TypeAnnotation

TypeAnnotation fields are

+-------+--------------------------------------------------------+
+_module+ :ref:`rtti::Module <handle-rtti-Module>` ?             +
+-------+--------------------------------------------------------+
+cppName+ :ref:`builtin::das_string <handle-builtin-das_string>` +
+-------+--------------------------------------------------------+
+name   + :ref:`builtin::das_string <handle-builtin-das_string>` +
+-------+--------------------------------------------------------+


|structure_annotation-rtti-TypeAnnotation|

.. _handle-rtti-TypeInfo:

.. das:attribute:: TypeInfo

TypeInfo fields are

+----------+---------------------------------------------------+
+argTypes  + :ref:`rtti::TypeInfo <handle-rtti-TypeInfo>` ??   +
+----------+---------------------------------------------------+
+secondType+ :ref:`rtti::TypeInfo <handle-rtti-TypeInfo>` ?    +
+----------+---------------------------------------------------+
+dimSize   +uint                                               +
+----------+---------------------------------------------------+
+enumType  + :ref:`rtti::EnumInfo <handle-rtti-EnumInfo>` ?    +
+----------+---------------------------------------------------+
+hash      +uint                                               +
+----------+---------------------------------------------------+
+argNames  +string?                                            +
+----------+---------------------------------------------------+
+argCount  +uint                                               +
+----------+---------------------------------------------------+
+basicType + :ref:`rtti::Type <enum-rtti-Type>`                +
+----------+---------------------------------------------------+
+firstType + :ref:`rtti::TypeInfo <handle-rtti-TypeInfo>` ?    +
+----------+---------------------------------------------------+
+structType+ :ref:`rtti::StructInfo <handle-rtti-StructInfo>` ?+
+----------+---------------------------------------------------+


TypeInfo properties are

+----------+-----------------------------------------------------------+
+isConst   +bool                                                       +
+----------+-----------------------------------------------------------+
+annotation+ :ref:`rtti::TypeAnnotation <handle-rtti-TypeAnnotation>` ?+
+----------+-----------------------------------------------------------+
+isRawPod  +bool                                                       +
+----------+-----------------------------------------------------------+
+isRef     +bool                                                       +
+----------+-----------------------------------------------------------+
+isImplicit+bool                                                       +
+----------+-----------------------------------------------------------+
+isRefType +bool                                                       +
+----------+-----------------------------------------------------------+
+isPod     +bool                                                       +
+----------+-----------------------------------------------------------+
+isTemp    +bool                                                       +
+----------+-----------------------------------------------------------+
+isRefValue+bool                                                       +
+----------+-----------------------------------------------------------+
+canCopy   +bool                                                       +
+----------+-----------------------------------------------------------+


|structure_annotation-rtti-TypeInfo|

.. _handle-rtti-VarInfo:

.. das:attribute:: VarInfo

VarInfo fields are

+--------------------+---------------------------------------------------------------------------------+
+argTypes            + :ref:`rtti::TypeInfo <handle-rtti-TypeInfo>` ??                                 +
+--------------------+---------------------------------------------------------------------------------+
+value               +any                                                                              +
+--------------------+---------------------------------------------------------------------------------+
+secondType          + :ref:`rtti::TypeInfo <handle-rtti-TypeInfo>` ?                                  +
+--------------------+---------------------------------------------------------------------------------+
+dimSize             +uint                                                                             +
+--------------------+---------------------------------------------------------------------------------+
+enumType            + :ref:`rtti::EnumInfo <handle-rtti-EnumInfo>` ?                                  +
+--------------------+---------------------------------------------------------------------------------+
+name                +string                                                                           +
+--------------------+---------------------------------------------------------------------------------+
+hash                +uint                                                                             +
+--------------------+---------------------------------------------------------------------------------+
+argNames            +string?                                                                          +
+--------------------+---------------------------------------------------------------------------------+
+argCount            +uint                                                                             +
+--------------------+---------------------------------------------------------------------------------+
+sValue              +string                                                                           +
+--------------------+---------------------------------------------------------------------------------+
+offset              +uint                                                                             +
+--------------------+---------------------------------------------------------------------------------+
+basicType           + :ref:`rtti::Type <enum-rtti-Type>`                                              +
+--------------------+---------------------------------------------------------------------------------+
+firstType           + :ref:`rtti::TypeInfo <handle-rtti-TypeInfo>` ?                                  +
+--------------------+---------------------------------------------------------------------------------+
+annotation_arguments+ :ref:`rtti::AnnotationArguments <handle-rtti-AnnotationArguments>`  const? const+
+--------------------+---------------------------------------------------------------------------------+
+structType          + :ref:`rtti::StructInfo <handle-rtti-StructInfo>` ?                              +
+--------------------+---------------------------------------------------------------------------------+


VarInfo properties are

+----------+-----------------------------------------------------------+
+isConst   +bool                                                       +
+----------+-----------------------------------------------------------+
+annotation+ :ref:`rtti::TypeAnnotation <handle-rtti-TypeAnnotation>` ?+
+----------+-----------------------------------------------------------+
+isRawPod  +bool                                                       +
+----------+-----------------------------------------------------------+
+isRef     +bool                                                       +
+----------+-----------------------------------------------------------+
+isImplicit+bool                                                       +
+----------+-----------------------------------------------------------+
+isRefType +bool                                                       +
+----------+-----------------------------------------------------------+
+isPod     +bool                                                       +
+----------+-----------------------------------------------------------+
+isTemp    +bool                                                       +
+----------+-----------------------------------------------------------+
+isRefValue+bool                                                       +
+----------+-----------------------------------------------------------+
+canCopy   +bool                                                       +
+----------+-----------------------------------------------------------+


|structure_annotation-rtti-VarInfo|

+++++++++++++
Handled types
+++++++++++++

.. _handle-rtti-AnnotationArgumentList:

.. das:attribute:: AnnotationArgumentList

|any_annotation-rtti-AnnotationArgumentList|

.. _handle-rtti-AnnotationArguments:

.. das:attribute:: AnnotationArguments

|any_annotation-rtti-AnnotationArguments|

.. _handle-rtti-AnnotationList:

.. das:attribute:: AnnotationList

|any_annotation-rtti-AnnotationList|

.. _handle-rtti-dasvector`Error:

.. das:attribute:: dasvector`Error

|any_annotation-rtti-dasvector`Error|

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_rtti_c__c_basic_struct_for_each_field__hh_handle_hh_BasicStructureAnnotation_hh_const_hh_implicit__hh_block_hh_string_hh_string_hh__hh_handle_hh_TypeInfo_hh_const_hh_uint_hh__c_void_hh_const_hh_implicit__hh_const:

.. das:function:: basic_struct_for_each_field(arg0: BasicStructureAnnotation const implicit; arg1: block<(string;string;rtti::TypeInfo const;uint):void> const implicit)

+--------+----------------------------------------------------------------------------------------------------+
+argument+argument type                                                                                       +
+========+====================================================================================================+
+arg0    + :ref:`rtti::BasicStructureAnnotation <handle-rtti-BasicStructureAnnotation>`  const implicit       +
+--------+----------------------------------------------------------------------------------------------------+
+arg1    +block<(string;string; :ref:`rtti::TypeInfo <handle-rtti-TypeInfo>`  const;uint):void> const implicit+
+--------+----------------------------------------------------------------------------------------------------+


|function-rtti-basic_struct_for_each_field|

.. _function-_at_rtti_c__c_builtin_is_same_type__hh_ptr_hh__hh_handle_hh_TypeInfo_hh_const_hh_const_hh_implicit__hh_ptr_hh__hh_handle_hh_TypeInfo_hh_const_hh_const_hh_implicit__hh_enum_hh_rtti_c__c_RefMatters_hh_int_hh_const__hh_enum_hh_rtti_c__c_ConstMatters_hh_int_hh_const__hh_enum_hh_rtti_c__c_TemporaryMatters_hh_int_hh_const_bool_hh_const:

.. das:function:: builtin_is_same_type(arg0: rtti::TypeInfo const? const implicit; arg1: rtti::TypeInfo const? const implicit; arg2: RefMatters const; arg3: ConstMatters const; arg4: TemporaryMatters const; arg5: bool const)

builtin_is_same_type returns bool

+--------+--------------------------------------------------------------------+
+argument+argument type                                                       +
+========+====================================================================+
+arg0    + :ref:`rtti::TypeInfo <handle-rtti-TypeInfo>`  const? const implicit+
+--------+--------------------------------------------------------------------+
+arg1    + :ref:`rtti::TypeInfo <handle-rtti-TypeInfo>`  const? const implicit+
+--------+--------------------------------------------------------------------+
+arg2    + :ref:`rtti::RefMatters <enum-rtti-RefMatters>`  const              +
+--------+--------------------------------------------------------------------+
+arg3    + :ref:`rtti::ConstMatters <enum-rtti-ConstMatters>`  const          +
+--------+--------------------------------------------------------------------+
+arg4    + :ref:`rtti::TemporaryMatters <enum-rtti-TemporaryMatters>`  const  +
+--------+--------------------------------------------------------------------+
+arg5    +bool const                                                          +
+--------+--------------------------------------------------------------------+


|function-rtti-builtin_is_same_type|

.. _function-_at_rtti_c__c_compile_string_hh_const_string_hh_const__hh_block_hh_bool_hh__hh_smart_ptr_hh__hh_handle_hh_Program_hh__hh_handle_hh_das_string_hh_const_hh__c_void_hh_const_hh_implicit__hh_const:

.. das:function:: compile(arg0: string const; arg1: string const; arg2: block<(bool;smart_ptr<rtti::Program>;das_string const):void> const implicit)

+--------+--------------------------------------------------------------------------------------------------------------------------------------------------------+
+argument+argument type                                                                                                                                           +
+========+========================================================================================================================================================+
+arg0    +string const                                                                                                                                            +
+--------+--------------------------------------------------------------------------------------------------------------------------------------------------------+
+arg1    +string const                                                                                                                                            +
+--------+--------------------------------------------------------------------------------------------------------------------------------------------------------+
+arg2    +block<(bool;smart_ptr< :ref:`rtti::Program <handle-rtti-Program>` >; :ref:`builtin::das_string <handle-builtin-das_string>`  const):void> const implicit+
+--------+--------------------------------------------------------------------------------------------------------------------------------------------------------+


|function-rtti-compile|

.. _function-_at_rtti_c__c_compile_file_string_hh_const__hh_smart_ptr_hh__hh_handle_hh_FileAccess_hh_const_hh_implicit__hh_block_hh_bool_hh__hh_smart_ptr_hh__hh_handle_hh_Program_hh__hh_handle_hh_das_string_hh_const_hh__c_void_hh_const_hh_implicit__hh_const:

.. das:function:: compile_file(arg0: string const; arg1: smart_ptr<rtti::FileAccess> const implicit; arg2: block<(bool;smart_ptr<rtti::Program>;das_string const):void> const implicit)

+--------+--------------------------------------------------------------------------------------------------------------------------------------------------------+
+argument+argument type                                                                                                                                           +
+========+========================================================================================================================================================+
+arg0    +string const                                                                                                                                            +
+--------+--------------------------------------------------------------------------------------------------------------------------------------------------------+
+arg1    +smart_ptr< :ref:`rtti::FileAccess <handle-rtti-FileAccess>` > const implicit                                                                            +
+--------+--------------------------------------------------------------------------------------------------------------------------------------------------------+
+arg2    +block<(bool;smart_ptr< :ref:`rtti::Program <handle-rtti-Program>` >; :ref:`builtin::das_string <handle-builtin-das_string>`  const):void> const implicit+
+--------+--------------------------------------------------------------------------------------------------------------------------------------------------------+


|function-rtti-compile_file|

.. _function-_at_rtti_c__c_get_annotation_argument_value__hh_handle_hh_AnnotationArgument_hh_const_hh_implicit__hh_const:

.. das:function:: get_annotation_argument_value(arg0: AnnotationArgument const implicit)

get_annotation_argument_value returns  :ref:`RttiValue <alias-RttiValue>` 

+--------+---------------------------------------------------------------------------------+
+argument+argument type                                                                    +
+========+=================================================================================+
+arg0    + :ref:`rtti::AnnotationArgument <handle-rtti-AnnotationArgument>`  const implicit+
+--------+---------------------------------------------------------------------------------+


|function-rtti-get_annotation_argument_value|

.. _function-_at_rtti_c__c_get_das_type_name__hh_enum_hh_rtti_c__c_Type_hh_int_hh_const__hh_const:

.. das:function:: get_das_type_name(arg0: Type const)

get_das_type_name returns string

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+arg0    + :ref:`rtti::Type <enum-rtti-Type>`  const+
+--------+------------------------------------------+


|function-rtti-get_das_type_name|

.. _function-_at_rtti_c__c_get_dim__hh_handle_hh_TypeInfo_hh_const_hh_implicit_int_hh_const__hh_const:

.. das:function:: get_dim(arg0: TypeInfo const implicit; arg1: int const)

get_dim returns int

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+arg0    + :ref:`rtti::TypeInfo <handle-rtti-TypeInfo>`  const implicit+
+--------+-------------------------------------------------------------+
+arg1    +int const                                                    +
+--------+-------------------------------------------------------------+


|function-rtti-get_dim|

.. _function-_at_rtti_c__c_get_dim__hh_handle_hh_VarInfo_hh_const_hh_implicit_int_hh_const__hh_const:

.. das:function:: get_dim(arg0: VarInfo const implicit; arg1: int const)

get_dim returns int

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+arg0    + :ref:`rtti::VarInfo <handle-rtti-VarInfo>`  const implicit+
+--------+-----------------------------------------------------------+
+arg1    +int const                                                  +
+--------+-----------------------------------------------------------+


|function-rtti-get_dim|

.. _function-_at_rtti_c__c_get_function_info_int:

.. das:function:: get_function_info(arg0: int)

get_function_info returns  :ref:`rtti::FuncInfo <handle-rtti-FuncInfo>`  const&

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +int          +
+--------+-------------+


|function-rtti-get_function_info|

.. _function-_at_rtti_c__c_get_line_info__hh_const:

.. das:function:: get_line_info()

get_line_info returns  :ref:`rtti::LineInfo <handle-rtti-LineInfo>` 

|function-rtti-get_line_info|

.. _function-_at_rtti_c__c_get_module_string_hh_const:

.. das:function:: get_module(arg0: string const)

get_module returns  :ref:`rtti::Module <handle-rtti-Module>` ?

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+


|function-rtti-get_module|

.. _function-_at_rtti_c__c_get_this_module__hh_smart_ptr_hh__hh_handle_hh_Program_hh_const_hh_implicit:

.. das:function:: get_this_module(arg0: smart_ptr<rtti::Program> const implicit)

get_this_module returns  :ref:`rtti::Module <handle-rtti-Module>` ?

+--------+----------------------------------------------------------------------+
+argument+argument type                                                         +
+========+======================================================================+
+arg0    +smart_ptr< :ref:`rtti::Program <handle-rtti-Program>` > const implicit+
+--------+----------------------------------------------------------------------+


|function-rtti-get_this_module|

.. _function-_at_rtti_c__c_get_total_functions__hh_const:

.. das:function:: get_total_functions()

get_total_functions returns int

|function-rtti-get_total_functions|

.. _function-_at_rtti_c__c_get_total_variables__hh_const:

.. das:function:: get_total_variables()

get_total_variables returns int

|function-rtti-get_total_variables|

.. _function-_at_rtti_c__c_get_variable_info_int:

.. das:function:: get_variable_info(arg0: int)

get_variable_info returns  :ref:`rtti::VarInfo <handle-rtti-VarInfo>`  const&

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +int          +
+--------+-------------+


|function-rtti-get_variable_info|

.. _function-_at_rtti_c__c_get_variable_value__hh_handle_hh_VarInfo_hh_const_hh_implicit:

.. das:function:: get_variable_value(arg0: VarInfo const implicit)

get_variable_value returns  :ref:`RttiValue <alias-RttiValue>` 

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+arg0    + :ref:`rtti::VarInfo <handle-rtti-VarInfo>`  const implicit+
+--------+-----------------------------------------------------------+


|function-rtti-get_variable_value|

.. _function-_at_rtti_c__c_is_compatible_cast__hh_ptr_hh__hh_handle_hh_StructInfo_hh_const_hh_const_hh_implicit__hh_ptr_hh__hh_handle_hh_StructInfo_hh_const_hh_const_hh_implicit:

.. das:function:: is_compatible_cast(arg0: rtti::StructInfo const? const implicit; arg1: rtti::StructInfo const? const implicit)

is_compatible_cast returns bool

+--------+------------------------------------------------------------------------+
+argument+argument type                                                           +
+========+========================================================================+
+arg0    + :ref:`rtti::StructInfo <handle-rtti-StructInfo>`  const? const implicit+
+--------+------------------------------------------------------------------------+
+arg1    + :ref:`rtti::StructInfo <handle-rtti-StructInfo>`  const? const implicit+
+--------+------------------------------------------------------------------------+


|function-rtti-is_compatible_cast|

.. _function-_at_rtti_c__c_make_file_access_string_hh_const__hh_const:

.. das:function:: make_file_access(arg0: string const)

make_file_access returns smart_ptr< :ref:`rtti::FileAccess <handle-rtti-FileAccess>` >

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+


|function-rtti-make_file_access|

.. _function-_at_rtti_c__c_module_for_each_annotation__hh_ptr_hh__hh_handle_hh_Module_hh_const_hh_implicit__hh_block_hh__hh_handle_hh_Annotation_hh_const_hh__c_void_hh_const_hh_implicit__hh_const:

.. das:function:: module_for_each_annotation(arg0: rtti::Module? const implicit; arg1: block<(rtti::Annotation const):void> const implicit)

+--------+-------------------------------------------------------------------------------------+
+argument+argument type                                                                        +
+========+=====================================================================================+
+arg0    + :ref:`rtti::Module <handle-rtti-Module>` ? const implicit                           +
+--------+-------------------------------------------------------------------------------------+
+arg1    +block<( :ref:`rtti::Annotation <handle-rtti-Annotation>`  const):void> const implicit+
+--------+-------------------------------------------------------------------------------------+


|function-rtti-module_for_each_annotation|

.. _function-_at_rtti_c__c_module_for_each_enumeration__hh_ptr_hh__hh_handle_hh_Module_hh_const_hh_implicit__hh_block_hh__hh_handle_hh_EnumInfo_hh_const_hh__c_void_hh_const_hh_implicit__hh_const:

.. das:function:: module_for_each_enumeration(arg0: rtti::Module? const implicit; arg1: block<(rtti::EnumInfo const):void> const implicit)

+--------+---------------------------------------------------------------------------------+
+argument+argument type                                                                    +
+========+=================================================================================+
+arg0    + :ref:`rtti::Module <handle-rtti-Module>` ? const implicit                       +
+--------+---------------------------------------------------------------------------------+
+arg1    +block<( :ref:`rtti::EnumInfo <handle-rtti-EnumInfo>`  const):void> const implicit+
+--------+---------------------------------------------------------------------------------+


|function-rtti-module_for_each_enumeration|

.. _function-_at_rtti_c__c_module_for_each_function__hh_ptr_hh__hh_handle_hh_Module_hh_const_hh_implicit__hh_block_hh__hh_handle_hh_FuncInfo_hh_const_hh__c_void_hh_const_hh_implicit__hh_const:

.. das:function:: module_for_each_function(arg0: rtti::Module? const implicit; arg1: block<(rtti::FuncInfo const):void> const implicit)

+--------+---------------------------------------------------------------------------------+
+argument+argument type                                                                    +
+========+=================================================================================+
+arg0    + :ref:`rtti::Module <handle-rtti-Module>` ? const implicit                       +
+--------+---------------------------------------------------------------------------------+
+arg1    +block<( :ref:`rtti::FuncInfo <handle-rtti-FuncInfo>`  const):void> const implicit+
+--------+---------------------------------------------------------------------------------+


|function-rtti-module_for_each_function|

.. _function-_at_rtti_c__c_module_for_each_generic__hh_ptr_hh__hh_handle_hh_Module_hh_const_hh_implicit__hh_block_hh__hh_handle_hh_FuncInfo_hh_const_hh__c_void_hh_const_hh_implicit__hh_const:

.. das:function:: module_for_each_generic(arg0: rtti::Module? const implicit; arg1: block<(rtti::FuncInfo const):void> const implicit)

+--------+---------------------------------------------------------------------------------+
+argument+argument type                                                                    +
+========+=================================================================================+
+arg0    + :ref:`rtti::Module <handle-rtti-Module>` ? const implicit                       +
+--------+---------------------------------------------------------------------------------+
+arg1    +block<( :ref:`rtti::FuncInfo <handle-rtti-FuncInfo>`  const):void> const implicit+
+--------+---------------------------------------------------------------------------------+


|function-rtti-module_for_each_generic|

.. _function-_at_rtti_c__c_module_for_each_global__hh_ptr_hh__hh_handle_hh_Module_hh_const_hh_implicit__hh_block_hh__hh_handle_hh_VarInfo_hh_const_hh__c_void_hh_const_hh_implicit__hh_const:

.. das:function:: module_for_each_global(arg0: rtti::Module? const implicit; arg1: block<(rtti::VarInfo const):void> const implicit)

+--------+-------------------------------------------------------------------------------+
+argument+argument type                                                                  +
+========+===============================================================================+
+arg0    + :ref:`rtti::Module <handle-rtti-Module>` ? const implicit                     +
+--------+-------------------------------------------------------------------------------+
+arg1    +block<( :ref:`rtti::VarInfo <handle-rtti-VarInfo>`  const):void> const implicit+
+--------+-------------------------------------------------------------------------------+


|function-rtti-module_for_each_global|

.. _function-_at_rtti_c__c_module_for_each_structure__hh_ptr_hh__hh_handle_hh_Module_hh_const_hh_implicit__hh_block_hh__hh_handle_hh_StructInfo_hh_const_hh__c_void_hh_const_hh_implicit__hh_const:

.. das:function:: module_for_each_structure(arg0: rtti::Module? const implicit; arg1: block<(rtti::StructInfo const):void> const implicit)

+--------+-------------------------------------------------------------------------------------+
+argument+argument type                                                                        +
+========+=====================================================================================+
+arg0    + :ref:`rtti::Module <handle-rtti-Module>` ? const implicit                           +
+--------+-------------------------------------------------------------------------------------+
+arg1    +block<( :ref:`rtti::StructInfo <handle-rtti-StructInfo>`  const):void> const implicit+
+--------+-------------------------------------------------------------------------------------+


|function-rtti-module_for_each_structure|

.. _function-_at_rtti_c__c_program_for_each_module__hh_smart_ptr_hh__hh_handle_hh_Program_hh_const_hh_implicit__hh_block_hh__hh_ptr_hh__hh_handle_hh_Module_hh__c_void_hh_const_hh_implicit__hh_const:

.. das:function:: program_for_each_module(arg0: smart_ptr<rtti::Program> const implicit; arg1: block<(rtti::Module?):void> const implicit)

+--------+------------------------------------------------------------------------+
+argument+argument type                                                           +
+========+========================================================================+
+arg0    +smart_ptr< :ref:`rtti::Program <handle-rtti-Program>` > const implicit  +
+--------+------------------------------------------------------------------------+
+arg1    +block<( :ref:`rtti::Module <handle-rtti-Module>` ?):void> const implicit+
+--------+------------------------------------------------------------------------+


|function-rtti-program_for_each_module|

.. _function-_at_rtti_c__c_program_for_each_registered_module__hh_block_hh__hh_ptr_hh__hh_handle_hh_Module_hh__c_void_hh_const_hh_implicit__hh_const:

.. das:function:: program_for_each_registered_module(arg0: block<(rtti::Module?):void> const implicit)

+--------+------------------------------------------------------------------------+
+argument+argument type                                                           +
+========+========================================================================+
+arg0    +block<( :ref:`rtti::Module <handle-rtti-Module>` ?):void> const implicit+
+--------+------------------------------------------------------------------------+


|function-rtti-program_for_each_registered_module|

.. _function-_at_rtti_c__c_rtti_builtin_structure_for_each_annotation__hh_handle_hh_StructInfo_hh_const_hh_implicit__hh_block_hh_const_hh_implicit__hh_const:

.. das:function:: rtti_builtin_structure_for_each_annotation(arg0: StructInfo const implicit; arg1: block<> const implicit)

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+arg0    + :ref:`rtti::StructInfo <handle-rtti-StructInfo>`  const implicit+
+--------+-----------------------------------------------------------------+
+arg1    +block<> const implicit                                           +
+--------+-----------------------------------------------------------------+


|function-rtti-rtti_builtin_structure_for_each_annotation|

.. _function-_at_rtti_c__c_set_file_source__hh_smart_ptr_hh__hh_handle_hh_FileAccess_hh_const_hh_implicit_string_hh_const_string_hh_const__hh_const:

.. das:function:: set_file_source(arg0: smart_ptr<rtti::FileAccess> const implicit; arg1: string const; arg2: string const)

set_file_source returns bool

+--------+----------------------------------------------------------------------------+
+argument+argument type                                                               +
+========+============================================================================+
+arg0    +smart_ptr< :ref:`rtti::FileAccess <handle-rtti-FileAccess>` > const implicit+
+--------+----------------------------------------------------------------------------+
+arg1    +string const                                                                +
+--------+----------------------------------------------------------------------------+
+arg2    +string const                                                                +
+--------+----------------------------------------------------------------------------+


|function-rtti-set_file_source|

.. _function-_at_rtti_c__c_RttiValue_nothing:

.. das:function:: RttiValue_nothing()

RttiValue_nothing returns auto

|function-rtti-RttiValue_nothing|

.. _function-_at_rtti_c__c_arg_names__hh_handle_hh_TypeInfo_hh_const:

.. das:function:: arg_names(info: TypeInfo const)

arg_names returns auto

+--------+----------------------------------------------------+
+argument+argument type                                       +
+========+====================================================+
+info    + :ref:`rtti::TypeInfo <handle-rtti-TypeInfo>`  const+
+--------+----------------------------------------------------+


|function-rtti-arg_names|

.. _function-_at_rtti_c__c_arg_names__hh_handle_hh_VarInfo_hh_const:

.. das:function:: arg_names(info: VarInfo const)

arg_names returns auto

+--------+--------------------------------------------------+
+argument+argument type                                     +
+========+==================================================+
+info    + :ref:`rtti::VarInfo <handle-rtti-VarInfo>`  const+
+--------+--------------------------------------------------+


|function-rtti-arg_names|

.. _function-_at_rtti_c__c_arg_types__hh_handle_hh_TypeInfo_hh_const:

.. das:function:: arg_types(info: TypeInfo const)

arg_types returns auto

+--------+----------------------------------------------------+
+argument+argument type                                       +
+========+====================================================+
+info    + :ref:`rtti::TypeInfo <handle-rtti-TypeInfo>`  const+
+--------+----------------------------------------------------+


|function-rtti-arg_types|

.. _function-_at_rtti_c__c_arg_types__hh_handle_hh_VarInfo_hh_const:

.. das:function:: arg_types(info: VarInfo const)

arg_types returns auto

+--------+--------------------------------------------------+
+argument+argument type                                     +
+========+==================================================+
+info    + :ref:`rtti::VarInfo <handle-rtti-VarInfo>`  const+
+--------+--------------------------------------------------+


|function-rtti-arg_types|

.. _function-_at_rtti_c__c_class_info__hh_auto_hh_const:

.. das:function:: class_info(cl: auto const)

class_info returns  :ref:`rtti::StructInfo <handle-rtti-StructInfo>`  const?

+--------+-------------+
+argument+argument type+
+========+=============+
+cl      +auto const   +
+--------+-------------+


|function-rtti-class_info|

.. _function-_at_rtti_c__c_context_for_each_function__hh_block_hh__hh_handle_hh_FuncInfo_hh_const_hh__rq_info_hh__c_void_hh_const:

.. das:function:: context_for_each_function(blk: block<(info:rtti::FuncInfo const):void> const)

context_for_each_function returns auto

+--------+-----------------------------------------------------------------------------+
+argument+argument type                                                                +
+========+=============================================================================+
+blk     +block<(info: :ref:`rtti::FuncInfo <handle-rtti-FuncInfo>`  const):void> const+
+--------+-----------------------------------------------------------------------------+


|function-rtti-context_for_each_function|

.. _function-_at_rtti_c__c_context_for_each_variable__hh_block_hh__hh_handle_hh_VarInfo_hh_const_hh__rq_info_hh__c_void_hh_const:

.. das:function:: context_for_each_variable(blk: block<(info:rtti::VarInfo const):void> const)

context_for_each_variable returns auto

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+blk     +block<(info: :ref:`rtti::VarInfo <handle-rtti-VarInfo>`  const):void> const+
+--------+---------------------------------------------------------------------------+


|function-rtti-context_for_each_variable|

.. _function-_at_rtti_c__c_each_dim__hh_handle_hh_TypeInfo_hh_const:

.. das:function:: each_dim(info: TypeInfo const)

each_dim returns auto

+--------+----------------------------------------------------+
+argument+argument type                                       +
+========+====================================================+
+info    + :ref:`rtti::TypeInfo <handle-rtti-TypeInfo>`  const+
+--------+----------------------------------------------------+


|function-rtti-each_dim|

.. _function-_at_rtti_c__c_each_dim__hh_handle_hh_VarInfo_hh_const:

.. das:function:: each_dim(info: VarInfo const)

each_dim returns auto

+--------+--------------------------------------------------+
+argument+argument type                                     +
+========+==================================================+
+info    + :ref:`rtti::VarInfo <handle-rtti-VarInfo>`  const+
+--------+--------------------------------------------------+


|function-rtti-each_dim|

.. _function-_at_rtti_c__c_is_compatible_cast__hh_handle_hh_StructInfo_hh_const__hh_handle_hh_StructInfo_hh_const:

.. das:function:: is_compatible_cast(a: StructInfo const; b: StructInfo const)

is_compatible_cast returns auto

+--------+--------------------------------------------------------+
+argument+argument type                                           +
+========+========================================================+
+a       + :ref:`rtti::StructInfo <handle-rtti-StructInfo>`  const+
+--------+--------------------------------------------------------+
+b       + :ref:`rtti::StructInfo <handle-rtti-StructInfo>`  const+
+--------+--------------------------------------------------------+


|function-rtti-is_compatible_cast|

.. _function-_at_rtti_c__c_is_same_type__hh_handle_hh_TypeInfo_hh_const__hh_handle_hh_TypeInfo_hh_const__hh_enum_hh_rtti_c__c_RefMatters_hh_int_hh_const__hh_enum_hh_rtti_c__c_ConstMatters_hh_int_hh_const__hh_enum_hh_rtti_c__c_TemporaryMatters_hh_int_hh_const_bool_hh_const:

.. das:function:: is_same_type(a: TypeInfo const; b: TypeInfo const; refMatters: RefMatters const; constMatters: ConstMatters const; temporaryMatters: TemporaryMatters const; topLevel: bool const)

is_same_type returns auto

+----------------+------------------------------------------------------------------+
+argument        +argument type                                                     +
+================+==================================================================+
+a               + :ref:`rtti::TypeInfo <handle-rtti-TypeInfo>`  const              +
+----------------+------------------------------------------------------------------+
+b               + :ref:`rtti::TypeInfo <handle-rtti-TypeInfo>`  const              +
+----------------+------------------------------------------------------------------+
+refMatters      + :ref:`rtti::RefMatters <enum-rtti-RefMatters>`  const            +
+----------------+------------------------------------------------------------------+
+constMatters    + :ref:`rtti::ConstMatters <enum-rtti-ConstMatters>`  const        +
+----------------+------------------------------------------------------------------+
+temporaryMatters+ :ref:`rtti::TemporaryMatters <enum-rtti-TemporaryMatters>`  const+
+----------------+------------------------------------------------------------------+
+topLevel        +bool const                                                        +
+----------------+------------------------------------------------------------------+


|function-rtti-is_same_type|

.. _function-_at_rtti_c__c_structure_for_each_annotation__hh_handle_hh_StructInfo_hh_const__hh_block_hh__hh_handle_hh_Annotation_hh_const_hh__hh_handle_hh_AnnotationArguments_hh_const_hh__rq_ann_hh__rq_args_hh__c_void_hh_const:

.. das:function:: structure_for_each_annotation(st: StructInfo const; subexpr: block<(ann:rtti::Annotation const;args:rtti::AnnotationArguments const):void> const)

structure_for_each_annotation returns auto

+--------+----------------------------------------------------------------------------------------------------------------------------------------------------------------+
+argument+argument type                                                                                                                                                   +
+========+================================================================================================================================================================+
+st      + :ref:`rtti::StructInfo <handle-rtti-StructInfo>`  const                                                                                                        +
+--------+----------------------------------------------------------------------------------------------------------------------------------------------------------------+
+subexpr +block<(ann: :ref:`rtti::Annotation <handle-rtti-Annotation>`  const;args: :ref:`rtti::AnnotationArguments <handle-rtti-AnnotationArguments>`  const):void> const+
+--------+----------------------------------------------------------------------------------------------------------------------------------------------------------------+


|function-rtti-structure_for_each_annotation|


