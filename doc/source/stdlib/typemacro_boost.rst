
.. _stdlib_typemacro_boost:

=========================================
Type macro and template structure support
=========================================

The TYPEMACRO_BOOST module provides infrastructure for defining type macros —
custom compile-time type transformations. Type macros allow introducing new
type syntax that expands into standard daScript types during compilation.

All functions and symbols are in "typemacro_boost" module, use require to get access to it. ::

    require daslib/typemacro_boost

++++++++++
Structures
++++++++++

.. _struct-typemacro_boost-TypeMacroTemplateArgument:

.. das:attribute:: TypeMacroTemplateArgument

|detail/structure-typemacro_boost-TypeMacroTemplateArgument|

++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-typemacro_boost-typemacro_function:

.. das:attribute:: typemacro_function

This macro converts function into a type macro.

.. _handle-typemacro_boost-typemacro_template_function:

.. das:attribute:: typemacro_template_function

This one converts function into a type macro that uses template arguments.
For example [typemacro_template(TFlatHashTable)] def makeFlatHashTable ( macroArgument, passArgument : TypeDeclPtr; KeyType, ValueType : TypeDeclPtr; hashFunctionName : string) : TypeDeclPtr { ... }
We generate the body that handles template argument inference and instantiation.

++++++++++++++++
Structure macros
++++++++++++++++

.. _handle-typemacro_boost-typemacro_documentation:

.. das:attribute:: typemacro_documentation

|detail/structure_macro-typemacro_boost-typemacro_documentation|

.. _handle-typemacro_boost-typemacro_template:

.. das:attribute:: typemacro_template

|detail/structure_macro-typemacro_boost-typemacro_template|

.. _handle-typemacro_boost-template_structure:

.. das:attribute:: template_structure

This macro creates typemacro function and associates it with the structure.
It also creates the typemacro_template_function to associate with it.
For example:
[template_structure(KeyType,ValueType)] struct template TFlatHashTable { ... }
creates:
1) [typemacro_function] def TFlatHashTable (macroArgument, passArgument : TypeDeclPtr; KeyType, ValueType : TypeDeclPtr) : TypeDeclPtr {
    return <- make`template`TFlatHashTable(macroArgument, passArgument, KeyType, ValueType)
 }
2) [typemacro_template_function(TFlatHashTable)] def make`template`TFlatHashTable (macroArgument, passArgument : TypeDeclPtr; KeyType, ValueType : TypeDeclPtr) : TypeDeclPtr {
    return <- default<TypeDeclPtr>
 }

++++++++++++
Enum helpers
++++++++++++

  *  :ref:`int64_to_enum (_enu: auto(ET); value: int64) : ET <function-typemacro_boost_int64_to_enum_autoET_int64>` 

.. _function-typemacro_boost_int64_to_enum_autoET_int64:

.. das:function:: int64_to_enum(_enu: auto(ET); value: int64) : ET

|detail/function-typemacro_boost-int64_to_enum-0xb329576f15617c62|

:Arguments: * **_enu** : auto(ET)

            * **value** : int64

+++++++++++++
Uncategorized
+++++++++++++

.. _function-typemacro_boost_get_string_const_ExpressionPtr:

.. das:function:: get_string_const(expr: ExpressionPtr) : string

|detail/function-typemacro_boost-get_string_const-0xf655065b8b02861a|

:Arguments: * **expr** :  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

.. _function-typemacro_boost_make_typemacro_template_instance_Structure_q__Structure_q__array_ls_tuple_ls_string;string_gr__gr_:

.. das:function:: make_typemacro_template_instance(instance_type: Structure?; template_type: Structure?; ex: array<tuple<string;string>> = array<tuple<string;string>>())

|detail/function-typemacro_boost-make_typemacro_template_instance-0x7d3d6e21a6335442|

:Arguments: * **instance_type** :  :ref:`Structure <handle-ast-Structure>` ?

            * **template_type** :  :ref:`Structure <handle-ast-Structure>` ?

            * **ex** : array<tuple<string;string>>

.. _function-typemacro_boost_is_typemacro_template_instance_TypeDeclPtr_TypeDeclPtr_array_ls_tuple_ls_string;string_gr__gr_:

.. das:function:: is_typemacro_template_instance(passArgument: TypeDeclPtr; templateType: TypeDeclPtr; extra: array<tuple<string;string>> = array<tuple<string;string>>()) : bool

template instance is determined by having parent == template.parent

:Arguments: * **passArgument** :  :ref:`TypeDeclPtr <alias-TypeDeclPtr>` 

            * **templateType** :  :ref:`TypeDeclPtr <alias-TypeDeclPtr>` 

            * **extra** : array<tuple<string;string>>

.. _function-typemacro_boost_infer_struct_aliases_Structure_q__array_ls_TypeMacroTemplateArgument_gr_:

.. das:function:: infer_struct_aliases(structType: Structure?; args: array<TypeMacroTemplateArgument>) : bool

|detail/function-typemacro_boost-infer_struct_aliases-0x62f823d768318fa5|

:Arguments: * **structType** :  :ref:`Structure <handle-ast-Structure>` ?

            * **args** : array< :ref:`TypeMacroTemplateArgument <struct-typemacro_boost-TypeMacroTemplateArgument>` >

.. _function-typemacro_boost_infer_template_types_TypeDeclPtr_array_ls_TypeMacroTemplateArgument_gr_:

.. das:function:: infer_template_types(passArgument: TypeDeclPtr; args: array<TypeMacroTemplateArgument>) : TypeDeclPtr

|detail/function-typemacro_boost-infer_template_types-0x6b9504bdcdfb49e0|

:Arguments: * **passArgument** :  :ref:`TypeDeclPtr <alias-TypeDeclPtr>` 

            * **args** : array< :ref:`TypeMacroTemplateArgument <struct-typemacro_boost-TypeMacroTemplateArgument>` >

.. _function-typemacro_boost_add_structure_aliases_Structure_q__array_ls_TypeMacroTemplateArgument_gr_:

.. das:function:: add_structure_aliases(structType: Structure?; args: array<TypeMacroTemplateArgument>)

|detail/function-typemacro_boost-add_structure_aliases-0x85c39211ad0ddd27|

:Arguments: * **structType** :  :ref:`Structure <handle-ast-Structure>` ?

            * **args** : array< :ref:`TypeMacroTemplateArgument <struct-typemacro_boost-TypeMacroTemplateArgument>` >

.. _function-typemacro_boost_verify_arguments_array_ls_TypeMacroTemplateArgument_gr_:

.. das:function:: verify_arguments(args: array<TypeMacroTemplateArgument>) : bool

|detail/function-typemacro_boost-verify_arguments-0x5efed42c9ffac12e|

:Arguments: * **args** : array< :ref:`TypeMacroTemplateArgument <struct-typemacro_boost-TypeMacroTemplateArgument>` >

.. _function-typemacro_boost_template_structure_name_Structure_q__array_ls_TypeMacroTemplateArgument_gr__array_ls_tuple_ls_string;string_gr__gr_:

.. das:function:: template_structure_name(base: Structure?; arguments: array<TypeMacroTemplateArgument>; extra: array<tuple<string;string>> = array<tuple<string;string>>()) : string

|detail/function-typemacro_boost-template_structure_name-0xf61df8860f12ad24|

:Arguments: * **base** :  :ref:`Structure <handle-ast-Structure>` ?

            * **arguments** : array< :ref:`TypeMacroTemplateArgument <struct-typemacro_boost-TypeMacroTemplateArgument>` >

            * **extra** : array<tuple<string;string>>

.. _function-typemacro_boost_is_custom_work_done_Structure_q_:

.. das:function:: is_custom_work_done(structType: Structure?) : bool

|detail/function-typemacro_boost-is_custom_work_done-0xdd682cd4b0218df|

:Arguments: * **structType** :  :ref:`Structure <handle-ast-Structure>` ?

.. _function-typemacro_boost_mark_custom_work_done_Structure_q_:

.. das:function:: mark_custom_work_done(structType: Structure?)

|detail/function-typemacro_boost-mark_custom_work_done-0x886dbd95ecf9e7fa|

:Arguments: * **structType** :  :ref:`Structure <handle-ast-Structure>` ?

.. _function-typemacro_boost_typemacro_argument_auto_int_autoExprConstType_autoValueT:

.. das:function:: typemacro_argument(dimExpr: auto; index: int; constType: auto(ExprConstType); defaultValue: auto(ValueT)) : ValueT

|detail/function-typemacro_boost-typemacro_argument-0xcacf3c534f7cab81|

:Arguments: * **dimExpr** : auto

            * **index** : int

            * **constType** : auto(ExprConstType)

            * **defaultValue** : auto(ValueT)

.. _function-typemacro_boost_typemacro_argument_auto_int_ExprConstString_autoValueT:

.. das:function:: typemacro_argument(dimExpr: auto; index: int; constType: ExprConstString; defaultValue: auto(ValueT)) : ValueT

|detail/function-typemacro_boost-typemacro_argument-0x278212e4a29dbe02|

:Arguments: * **dimExpr** : auto

            * **index** : int

            * **constType** :  :ref:`ExprConstString <handle-ast-ExprConstString>` 

            * **defaultValue** : auto(ValueT)


