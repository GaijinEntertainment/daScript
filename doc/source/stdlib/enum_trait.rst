
.. _stdlib_enum_trait:

==================
Enumeration traits
==================

The enum_trait module implements typeinfo traits for the enumerations.

All functions and symbols are in "enum_trait" module, use require to get access to it. ::

    require daslib/enum_trait

+++++++++++++++
Typeinfo macros
+++++++++++++++

.. _call-macro-enum_trait-enum_names:

.. das:attribute:: enum_names

Implements typeinfo(enum_names EnumOrEnumType) which returns array of strings with enumValue names.

.. _call-macro-enum_trait-enum_length:

.. das:attribute:: enum_length

Implements typeinfo(enum_length EnumOrEnumType) which returns total number of elements in enumeration.

++++++++++++++++++++
Handled enumerations
++++++++++++++++++++

.. _handle-enum_trait-string_to_enum:

.. das:attribute:: string_to_enum

|detail/enumeration_macro-enum_trait-string_to_enum|

+++++++++++++
Uncategorized
+++++++++++++

.. _function-enum_trait_string_auto:

.. das:function:: string(arg: auto) : auto

converts enum value to string
 usage: let s = string(EnumValue)

:Arguments: * **arg** : auto

.. _function-enum_trait_to_enum_autoEnumT_string:

.. das:function:: to_enum(ent: auto(EnumT); name: string) : EnumT

converts string to enum value, panics if not found
 usage: let e = to_enum(type<EnumType>,"EnumValueName")

:Arguments: * **ent** : auto(EnumT)

            * **name** : string

.. _function-enum_trait_to_enum_autoEnumT_string_EnumT:

.. das:function:: to_enum(ent: auto(EnumT); name: string; defaultValue: EnumT) : EnumT

converts string to enum value, returns defaultValue if not found
 usage: let e = to_enum(type<EnumType>,"EnumValueName", EnumType.DefaultValue)

:Arguments: * **ent** : auto(EnumT)

            * **name** : string

            * **defaultValue** : EnumT

.. _function-enum_trait_enum_to_table_autoEnumT:

.. das:function:: enum_to_table(ent: auto(EnumT)) : table<string, EnumT>

converts enum type to array of tuples (name, value)
 usage: let t = enum_to_table(type<EnumType>)

:Arguments: * **ent** : auto(EnumT)


