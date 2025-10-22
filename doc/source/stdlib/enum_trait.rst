
.. _stdlib_enum_trait:

==================
Enumeration traits
==================

.. include:: detail/enum_trait.rst

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

Enumeration annotation which implements string constructor for enumeration.

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_enum_trait_c__c_string_C._%_ls_IsAnyEnumMacro_c_expect_any_enum(arg_eq_true)_gr_:

.. das:function:: string(arg: auto const)

string returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+arg     +auto const   +
+--------+-------------+


converts enum value to string
 usage: let s = string(EnumValue)

.. _function-_at_enum_trait_c__c_to_enum_CY_ls_EnumT_gr_._Cs:

.. das:function:: to_enum(ent: auto(EnumT) const; name: string const)

to_enum returns EnumT

+--------+-----------------+
+argument+argument type    +
+========+=================+
+ent     +auto(EnumT) const+
+--------+-----------------+
+name    +string const     +
+--------+-----------------+


converts string to enum value, returns defaultValue if not found
 usage: let e = to_enum(type<EnumType>,"EnumValueName", EnumType.DefaultValue)

.. _function-_at_enum_trait_c__c_to_enum_CY_ls_EnumT_gr_._Cs_CY_ls_EnumT_gr_L:

.. das:function:: to_enum(ent: auto(EnumT) const; name: string const; defaultValue: EnumT const)

to_enum returns EnumT

+------------+-----------------+
+argument    +argument type    +
+============+=================+
+ent         +auto(EnumT) const+
+------------+-----------------+
+name        +string const     +
+------------+-----------------+
+defaultValue+EnumT const      +
+------------+-----------------+


converts string to enum value, returns defaultValue if not found
 usage: let e = to_enum(type<EnumType>,"EnumValueName", EnumType.DefaultValue)

.. _function-_at_enum_trait_c__c_enum_to_table_CY_ls_EnumT_gr_.:

.. das:function:: enum_to_table(ent: auto(EnumT) const)

enum_to_table returns table<string;EnumT>

+--------+-----------------+
+argument+argument type    +
+========+=================+
+ent     +auto(EnumT) const+
+--------+-----------------+


converts enum type to array of tuples (name, value)
 usage: let t = enum_to_table(type<EnumType>)


