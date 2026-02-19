
.. _stdlib_typemacro_boost:

=========================================
Type macro and template structure support
=========================================

.. das:module:: typemacro_boost

The TYPEMACRO_BOOST module provides infrastructure for defining type macros —
custom compile-time type transformations. Type macros allow introducing new
type syntax that expands into standard daslang types during compilation.

All functions and symbols are in "typemacro_boost" module, use require to get access to it.

.. code-block:: das

    require daslib/typemacro_boost



++++++++++
Structures
++++++++++

.. _struct-typemacro_boost-TypeMacroTemplateArgument:

.. das:attribute:: TypeMacroTemplateArgument

Holds a type macro template argument with its name and inferred type.

:Fields: * **name** : string - Name of the template argument.

         * **argument_type** :  :ref:`TypeDeclPtr <alias-TypeDeclPtr>` - Declared argument type from the template signature.

         * **inferred_type** :  :ref:`TypeDeclPtr <alias-TypeDeclPtr>` - Inferred concrete type after template instantiation.




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

Structure annotation that stores type macro documentation metadata.


.. _handle-typemacro_boost-typemacro_template:

.. das:attribute:: typemacro_template

Structure annotation that marks a struct as a type macro template instance.


.. _handle-typemacro_boost-template_structure:

.. das:attribute:: template_structure

This macro creates typemacro function and associates it with the structure.
It also creates the typemacro_template_function to associate with it.
For example::

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

  *  :ref:`int64_to_enum (_enu: auto(ET); value: int64) : ET <function-typemacro_boost_int64_to_enum_autoET_int64_0x16>`

.. _function-typemacro_boost_int64_to_enum_autoET_int64_0x16:

.. das:function:: int64_to_enum(_enu: auto(ET); value: int64) : ET

Converts an int64 value to the specified enum type via reinterpret cast.


:Arguments: * **_enu** : auto(ET)

            * **value** : int64


++++++++++++++++++++++++++++++++
Template structure instantiation
++++++++++++++++++++++++++++++++

  *  :ref:`is_typemacro_template_instance (passArgument: TypeDeclPtr; templateType: TypeDeclPtr; extra: array\<tuple\<string;string\>\> = array\<tuple\<string;string\>\>()) : bool <function-typemacro_boost_is_typemacro_template_instance_TypeDeclPtr_TypeDeclPtr_array_ls_tuple_ls_string;string_gr__gr_>`
  *  :ref:`make_typemacro_template_instance (instance_type: Structure?; template_type: Structure?; ex: array\<tuple\<string;string\>\> = array\<tuple\<string;string\>\>()) <function-typemacro_boost_make_typemacro_template_instance_Structure_q__Structure_q__array_ls_tuple_ls_string;string_gr__gr_>`
  *  :ref:`template_structure_name (base: Structure?; arguments: array\<TypeMacroTemplateArgument\>; extra: array\<tuple\<string;string\>\> = array\<tuple\<string;string\>\>()) : string <function-typemacro_boost_template_structure_name_Structure_q__array_ls_TypeMacroTemplateArgument_gr__array_ls_tuple_ls_string;string_gr__gr_>`

.. _function-typemacro_boost_is_typemacro_template_instance_TypeDeclPtr_TypeDeclPtr_array_ls_tuple_ls_string;string_gr__gr_:

.. das:function:: is_typemacro_template_instance(passArgument: TypeDeclPtr; templateType: TypeDeclPtr; extra: array<tuple<string;string>> = array<tuple<string;string>>()) : bool

template instance is determined by having parent == template.parent


:Arguments: * **passArgument** :  :ref:`TypeDeclPtr <alias-TypeDeclPtr>`

            * **templateType** :  :ref:`TypeDeclPtr <alias-TypeDeclPtr>`

            * **extra** : array<tuple<string;string>>

.. _function-typemacro_boost_make_typemacro_template_instance_Structure_q__Structure_q__array_ls_tuple_ls_string;string_gr__gr_:

.. das:function:: make_typemacro_template_instance(instance_type: Structure?; template_type: Structure?; ex: array<tuple<string;string>> = array<tuple<string;string>>())

Annotates a structure as a typemacro template instance of the given template type.


:Arguments: * **instance_type** :  :ref:`Structure <handle-ast-Structure>`?

            * **template_type** :  :ref:`Structure <handle-ast-Structure>`?

            * **ex** : array<tuple<string;string>>

.. _function-typemacro_boost_template_structure_name_Structure_q__array_ls_TypeMacroTemplateArgument_gr__array_ls_tuple_ls_string;string_gr__gr_:

.. das:function:: template_structure_name(base: Structure?; arguments: array<TypeMacroTemplateArgument>; extra: array<tuple<string;string>> = array<tuple<string;string>>()) : string

Builds a mangled template structure name from its base name and argument types.


:Arguments: * **base** :  :ref:`Structure <handle-ast-Structure>`?

            * **arguments** : array< :ref:`TypeMacroTemplateArgument <struct-typemacro_boost-TypeMacroTemplateArgument>`>

            * **extra** : array<tuple<string;string>>


++++++++++++++++++++++
Type inference helpers
++++++++++++++++++++++

  *  :ref:`add_structure_aliases (structType: Structure?; var args: array\<TypeMacroTemplateArgument\>) <function-typemacro_boost_add_structure_aliases_Structure_q__array_ls_TypeMacroTemplateArgument_gr_>`
  *  :ref:`infer_struct_aliases (structType: Structure?; var args: array\<TypeMacroTemplateArgument\>) : bool <function-typemacro_boost_infer_struct_aliases_Structure_q__array_ls_TypeMacroTemplateArgument_gr_>`
  *  :ref:`infer_template_types (passArgument: TypeDeclPtr; var args: array\<TypeMacroTemplateArgument\>) : TypeDeclPtr <function-typemacro_boost_infer_template_types_TypeDeclPtr_array_ls_TypeMacroTemplateArgument_gr_>`
  *  :ref:`verify_arguments (var args: array\<TypeMacroTemplateArgument\>) : bool <function-typemacro_boost_verify_arguments_array_ls_TypeMacroTemplateArgument_gr_>`

.. _function-typemacro_boost_add_structure_aliases_Structure_q__array_ls_TypeMacroTemplateArgument_gr_:

.. das:function:: add_structure_aliases(structType: Structure?; args: array<TypeMacroTemplateArgument>)

Adds all template argument type aliases to a structure.


:Arguments: * **structType** :  :ref:`Structure <handle-ast-Structure>`?

            * **args** : array< :ref:`TypeMacroTemplateArgument <struct-typemacro_boost-TypeMacroTemplateArgument>`>

.. _function-typemacro_boost_infer_struct_aliases_Structure_q__array_ls_TypeMacroTemplateArgument_gr_:

.. das:function:: infer_struct_aliases(structType: Structure?; args: array<TypeMacroTemplateArgument>) : bool

Infers structure alias types for all template arguments from a structure definition.


:Arguments: * **structType** :  :ref:`Structure <handle-ast-Structure>`?

            * **args** : array< :ref:`TypeMacroTemplateArgument <struct-typemacro_boost-TypeMacroTemplateArgument>`>

.. _function-typemacro_boost_infer_template_types_TypeDeclPtr_array_ls_TypeMacroTemplateArgument_gr_:

.. das:function:: infer_template_types(passArgument: TypeDeclPtr; args: array<TypeMacroTemplateArgument>) : TypeDeclPtr

Infers and validates template argument types against a pass argument, returning the resolved type.


:Arguments: * **passArgument** :  :ref:`TypeDeclPtr <alias-TypeDeclPtr>`

            * **args** : array< :ref:`TypeMacroTemplateArgument <struct-typemacro_boost-TypeMacroTemplateArgument>`>

.. _function-typemacro_boost_verify_arguments_array_ls_TypeMacroTemplateArgument_gr_:

.. das:function:: verify_arguments(args: array<TypeMacroTemplateArgument>) : bool

Verifies that all template arguments have been fully inferred (no remaining auto or alias types).


:Arguments: * **args** : array< :ref:`TypeMacroTemplateArgument <struct-typemacro_boost-TypeMacroTemplateArgument>`>


++++++++++++++++++++++
String constant access
++++++++++++++++++++++

  *  :ref:`get_string_const (expr: ExpressionPtr) : string <function-typemacro_boost_get_string_const_ExpressionPtr>`

.. _function-typemacro_boost_get_string_const_ExpressionPtr:

.. das:function:: get_string_const(expr: ExpressionPtr) : string

Extracts a string constant value or function address name from an expression.


:Arguments: * **expr** :  :ref:`ExpressionPtr <alias-ExpressionPtr>`


+++++++++++++
Work tracking
+++++++++++++

  *  :ref:`is_custom_work_done (structType: Structure?) : bool <function-typemacro_boost_is_custom_work_done_Structure_q_>`
  *  :ref:`mark_custom_work_done (var structType: Structure?) <function-typemacro_boost_mark_custom_work_done_Structure_q_>`

.. _function-typemacro_boost_is_custom_work_done_Structure_q_:

.. das:function:: is_custom_work_done(structType: Structure?) : bool

Returns true if custom work has already been performed on the template structure.


:Arguments: * **structType** :  :ref:`Structure <handle-ast-Structure>`?

.. _function-typemacro_boost_mark_custom_work_done_Structure_q_:

.. das:function:: mark_custom_work_done(structType: Structure?)

Marks the template structure's custom work as complete in its annotation.


:Arguments: * **structType** :  :ref:`Structure <handle-ast-Structure>`?


++++++++++++++++++++
Type macro arguments
++++++++++++++++++++

  *  :ref:`typemacro_argument (dimExpr: auto; index: int; var constType: ExprConstString; var defaultValue: auto(ValueT)) : ValueT <function-typemacro_boost_typemacro_argument_auto_int_ExprConstString_autoValueT_0x51>`
  *  :ref:`typemacro_argument (dimExpr: auto; index: int; var constType: auto(ExprConstType); var defaultValue: auto(ValueT)) : ValueT <function-typemacro_boost_typemacro_argument_auto_int_autoExprConstType_autoValueT_0x44>`


typemacro_argument
^^^^^^^^^^^^^^^^^^

.. _function-typemacro_boost_typemacro_argument_auto_int_ExprConstString_autoValueT_0x51:

.. das:function:: typemacro_argument(dimExpr: auto; index: int; constType: ExprConstString; defaultValue: auto(ValueT)) : ValueT

Extracts a string constant or function address argument at the given index from a type macro's dimension expressions.


:Arguments: * **dimExpr** : auto

            * **index** : int

            * **constType** :  :ref:`ExprConstString <handle-ast-ExprConstString>`

            * **defaultValue** : auto(ValueT)

.. _function-typemacro_boost_typemacro_argument_auto_int_autoExprConstType_autoValueT_0x44:

.. das:function:: typemacro_argument(dimExpr: auto; index: int; constType: auto(ExprConstType); defaultValue: auto(ValueT)) : ValueT


