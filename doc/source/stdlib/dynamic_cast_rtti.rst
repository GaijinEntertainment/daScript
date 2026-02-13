
.. _stdlib_dynamic_cast_rtti:

=======================
Dynamic RTTI type casts
=======================

The DYNAMIC_CAST_RTTI module implements runtime dynamic casting between class
types using RTTI information. It provides safe downcasting with null results
on type mismatch, similar to C++ ``dynamic_cast``.

All functions and symbols are in "dynamic_cast_rtti" module, use require to get access to it. ::

    require daslib/dynamic_cast_rtti

++++++++++++++
Variant macros
++++++++++++++

.. _call-macro-dynamic_cast_rtti-ClassAsIs:

.. das:attribute:: ClassAsIs

|detail/variant_macro-dynamic_cast_rtti-ClassAsIs|

+++++++++++++
Dynamic casts
+++++++++++++

  *  :ref:`is_instance_of (instance: auto(TCLS)?; otherclass: auto(TT)) : auto <function-dynamic_cast_rtti_is_instance_of_autoTCLS_q__autoTT>` 

.. _function-dynamic_cast_rtti_is_instance_of_autoTCLS_q__autoTT:

.. das:function:: is_instance_of(instance: auto(TCLS)?; otherclass: auto(TT)) : auto

|detail/function-dynamic_cast_rtti-is_instance_of-0xc5fcab99b781e6a6|

:Arguments: * **instance** : auto(TCLS)?

            * **otherclass** : auto(TT)

+++++++++++++
Uncategorized
+++++++++++++

.. _function-dynamic_cast_rtti_dynamic_type_cast_auto_autoTT:

.. das:function:: dynamic_type_cast(instance: auto; otherclass: auto(TT)) : TT?

|detail/function-dynamic_cast_rtti-dynamic_type_cast-0x481c060aca6ae89a|

:Arguments: * **instance** : auto

            * **otherclass** : auto(TT)

.. _function-dynamic_cast_rtti_force_dynamic_type_cast_auto_autoTT:

.. das:function:: force_dynamic_type_cast(instance: auto; otherclass: auto(TT)) : TT?

|detail/function-dynamic_cast_rtti-force_dynamic_type_cast-0xa519cc5bb1f277b5|

:Arguments: * **instance** : auto

            * **otherclass** : auto(TT)


