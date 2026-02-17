
.. _stdlib_enum_trait:

==================
Enumeration traits
==================

.. das:module:: enum_trait

The ENUM_TRAIT module provides reflection utilities for enumerations: iterating
over all values, converting between enum values and strings, and building
lookup tables. The ``[string_to_enum]`` annotation generates a string constructor
for the annotated enum type.

All functions and symbols are in "enum_trait" module, use require to get access to it. ::

    require daslib/enum_trait

Example: ::

    require daslib/enum_trait

        enum Color {
            red
            green
            blue
        }

        [export]
        def main() {
            print("{Color.green}\n")
            let c = to_enum(type<Color>, "blue")
            print("{c}\n")
            let bad = to_enum(type<Color>, "purple", Color.red)
            print("fallback = {bad}\n")
        }
        // output:
        // green
        // blue
        // fallback = red



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

Enumeration annotation which implements string constructor for enumeration.



+++++++++++++++++++++
Enumeration iteration
+++++++++++++++++++++

  *  :ref:`each (tt: auto(TT)) : iterator\<TT\> <function-enum_trait_each_autoTT_0x16>`

.. _function-enum_trait_each_autoTT_0x16:

.. das:function:: each(tt: auto(TT)) : iterator<TT>

Returns an iterator over all values of the given enumeration type.


:Arguments: * **tt** : auto(TT)


++++++++++++++++++++++
Enumeration conversion
++++++++++++++++++++++

  *  :ref:`enum_to_table (ent: auto(EnumT)) : table\<string, EnumT\> <function-enum_trait_enum_to_table_autoEnumT_0x41>`
  *  :ref:`string (arg: auto) : auto <function-enum_trait_string_auto_0x21>`
  *  :ref:`to_enum (ent: auto(EnumT); name: string) : EnumT <function-enum_trait_to_enum_autoEnumT_string_0x28>`
  *  :ref:`to_enum (ent: auto(EnumT); name: string; defaultValue: EnumT) : EnumT <function-enum_trait_to_enum_autoEnumT_string_EnumT_0x35>`

.. _function-enum_trait_enum_to_table_autoEnumT_0x41:

.. das:function:: enum_to_table(ent: auto(EnumT)) : table<string, EnumT>

converts enum type to a table of name => value pairs
 usage: let t = enum_to_table(type<EnumType>)


:Arguments: * **ent** : auto(EnumT)

.. _function-enum_trait_string_auto_0x21:

.. das:function:: string(arg: auto) : auto

converts enum value to string
 usage: let s = string(EnumValue)


:Arguments: * **arg** : auto


to_enum
^^^^^^^

.. _function-enum_trait_to_enum_autoEnumT_string_0x28:

.. das:function:: to_enum(ent: auto(EnumT); name: string) : EnumT

converts string to enum value, panics if not found
 usage: let e = to_enum(type<EnumType>,"EnumValueName")


:Arguments: * **ent** : auto(EnumT)

            * **name** : string

.. _function-enum_trait_to_enum_autoEnumT_string_EnumT_0x35:

.. das:function:: to_enum(ent: auto(EnumT); name: string; defaultValue: EnumT) : EnumT

----


