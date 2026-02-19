
.. _stdlib_dynamic_cast_rtti:

=======================
Dynamic RTTI type casts
=======================

.. das:module:: dynamic_cast_rtti

The DYNAMIC_CAST_RTTI module implements runtime dynamic casting between class
types using RTTI information. It provides safe downcasting with null results
on type mismatch, similar to C++ ``dynamic_cast``.

All functions and symbols are in "dynamic_cast_rtti" module, use require to get access to it.

.. code-block:: das

    require daslib/dynamic_cast_rtti



++++++++++++++
Variant macros
++++++++++++++

.. _call-macro-dynamic_cast_rtti-ClassAsIs:

.. das:attribute:: ClassAsIs

Variant macro that implements class dynamic casting via `is` and `as`.



+++++++++++++
Dynamic casts
+++++++++++++

  *  :ref:`dynamic_type_cast (instance: auto; otherclass: auto(TT)) : TT? <function-dynamic_cast_rtti_dynamic_type_cast_auto_autoTT_0x2d>`
  *  :ref:`force_dynamic_type_cast (instance: auto; otherclass: auto(TT)) : TT? <function-dynamic_cast_rtti_force_dynamic_type_cast_auto_autoTT_0x44>`
  *  :ref:`is_instance_of (instance: auto(TCLS)?; otherclass: auto(TT)) : auto <function-dynamic_cast_rtti_is_instance_of_autoTCLS_q__autoTT_0x16>`

.. _function-dynamic_cast_rtti_dynamic_type_cast_auto_autoTT_0x2d:

.. das:function:: dynamic_type_cast(instance: auto; otherclass: auto(TT)) : TT?

Casts a class instance to the target type using RTTI, returns null if the cast fails.


:Arguments: * **instance** : auto

            * **otherclass** : auto(TT)

.. _function-dynamic_cast_rtti_force_dynamic_type_cast_auto_autoTT_0x44:

.. das:function:: force_dynamic_type_cast(instance: auto; otherclass: auto(TT)) : TT?

Casts a class instance to the target type using RTTI, panics if the cast fails.


:Arguments: * **instance** : auto

            * **otherclass** : auto(TT)

.. _function-dynamic_cast_rtti_is_instance_of_autoTCLS_q__autoTT_0x16:

.. das:function:: is_instance_of(instance: auto(TCLS)?; otherclass: auto(TT)) : auto

Returns true if the class instance is an instance of the specified class using RTTI.


:Arguments: * **instance** : auto(TCLS)?

            * **otherclass** : auto(TT)


