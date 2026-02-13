
.. _stdlib_type_traits:

=================
Type trait macros
=================

The TYPE_TRAITS module provides compile-time type introspection and manipulation.
It includes type queries (``is_numeric``, ``is_string``, ``is_pointer``),
type transformations, and generic programming utilities for writing
type-aware macros and functions.

All functions and symbols are in "type_traits" module, use require to get access to it. ::

    require daslib/type_traits

+++++++++++
Call macros
+++++++++++

.. _call-macro-type_traits-is_subclass_of:

.. das:attribute:: is_subclass_of

Converts to 'true' if the first type is a subclass of the second type.

+++++++++++++++
Typeinfo macros
+++++++++++++++

.. _call-macro-type_traits-safe_has_property:

.. das:attribute:: safe_has_property

this macro implements "has_property" type trait, which returns true when structure has a property

.. _call-macro-type_traits-fields_count:

.. das:attribute:: fields_count

this macro implements "fields_count" type trait, which returns total number of fields in the structure


