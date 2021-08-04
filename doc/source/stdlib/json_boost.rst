
.. _stdlib_json_boost:

======================
Boost package for JSON
======================

.. include:: detail/json_boost.rst

The JSON boost module implements collection of helper macros and functions to accompany :ref:`JSON <stdlib_json>`.

All functions and symbols are in "json_boost" module, use require to get access to it. ::

    require daslib/json_boost


+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_json_boost_c__c_JV__hh_const_hh__hh_bitfield:

.. das:function:: JV(v: bitfield const)

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

+--------+----------------+
+argument+argument type   +
+========+================+
+v       +bitfield<> const+
+--------+----------------+


|function-json_boost-JV|

.. _function-_at_json_boost_c__c_JV__hh_const_hh_float:

.. das:function:: JV(v: float const)

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

+--------+-------------+
+argument+argument type+
+========+=============+
+v       +float const  +
+--------+-------------+


|function-json_boost-JV|

.. _function-_at_json_boost_c__c_JV__hh_const_hh_int:

.. das:function:: JV(v: int const)

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

+--------+-------------+
+argument+argument type+
+========+=============+
+v       +int const    +
+--------+-------------+


|function-json_boost-JV|

.. _function-_at_json_boost_c__c_JV__hh_const_hh__hh_auto:

.. das:function:: JV(ent: auto const)

JV returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+ent     +auto const   +
+--------+-------------+


|function-json_boost-JV|

.. _function-_at_json_boost_c__c_JV__hh_const_hh__hh_auto_hh_VecT:

.. das:function:: JV(v: auto(VecT) const)

JV returns auto

+--------+----------------+
+argument+argument type   +
+========+================+
+v       +auto(VecT) const+
+--------+----------------+


|function-json_boost-JV|

.. _function-_at_json_boost_c__c_from_JV__hh_const_hh__hh_ptr_hh_JsonValue__hh_const_hh__hh_auto_hh_EnumT__hh_const_hh__hh_alias_hh_EnumT:

.. das:function:: from_JV(v: json::JsonValue? const; ent: auto(EnumT) const; defV: EnumT const)

from_JV returns EnumT

+--------+-------------------------------------------------------+
+argument+argument type                                          +
+========+=======================================================+
+v       + :ref:`json::JsonValue <struct-json-JsonValue>` ? const+
+--------+-------------------------------------------------------+
+ent     +auto(EnumT) const                                      +
+--------+-------------------------------------------------------+
+defV    +EnumT const                                            +
+--------+-------------------------------------------------------+


|function-json_boost-from_JV|

.. _function-_at_json_boost_c__c_from_JV__hh_ptr_hh_JsonValue__hh_const_hh__hh_auto_hh_VecT__hh_const_hh__hh_alias_hh_VecT:

.. das:function:: from_JV(v: json::JsonValue?; ent: auto(VecT) const; defV: VecT const)

from_JV returns auto

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+v       + :ref:`json::JsonValue <struct-json-JsonValue>` ?+
+--------+-------------------------------------------------+
+ent     +auto(VecT) const                                 +
+--------+-------------------------------------------------+
+defV    +VecT const                                       +
+--------+-------------------------------------------------+


|function-json_boost-from_JV|

.. _function-_at_json_boost_c__c_from_JV__hh_ptr_hh_JsonValue__hh_const_hh__hh_bitfield__hh_const_hh__hh_bitfield:

.. das:function:: from_JV(v: json::JsonValue?; ent: bitfield const; defV: bitfield const)

from_JV returns auto

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+v       + :ref:`json::JsonValue <struct-json-JsonValue>` ?+
+--------+-------------------------------------------------+
+ent     +bitfield<> const                                 +
+--------+-------------------------------------------------+
+defV    +bitfield<> const                                 +
+--------+-------------------------------------------------+


|function-json_boost-from_JV|

.. _function-_at_json_boost_c__c_from_JV__hh_ptr_hh_JsonValue__hh_const_hh_bool__hh_const_hh_bool:

.. das:function:: from_JV(v: json::JsonValue?; ent: bool const; defV: bool const)

from_JV returns auto

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+v       + :ref:`json::JsonValue <struct-json-JsonValue>` ?+
+--------+-------------------------------------------------+
+ent     +bool const                                       +
+--------+-------------------------------------------------+
+defV    +bool const                                       +
+--------+-------------------------------------------------+


|function-json_boost-from_JV|

.. _function-_at_json_boost_c__c_from_JV__hh_ptr_hh_JsonValue__hh_const_hh_float__hh_const_hh_float:

.. das:function:: from_JV(v: json::JsonValue?; ent: float const; defV: float const)

from_JV returns auto

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+v       + :ref:`json::JsonValue <struct-json-JsonValue>` ?+
+--------+-------------------------------------------------+
+ent     +float const                                      +
+--------+-------------------------------------------------+
+defV    +float const                                      +
+--------+-------------------------------------------------+


|function-json_boost-from_JV|

.. _function-_at_json_boost_c__c_from_JV__hh_ptr_hh_JsonValue__hh_const_hh_int__hh_const_hh_int:

.. das:function:: from_JV(v: json::JsonValue?; ent: int const; defV: int const)

from_JV returns auto

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+v       + :ref:`json::JsonValue <struct-json-JsonValue>` ?+
+--------+-------------------------------------------------+
+ent     +int const                                        +
+--------+-------------------------------------------------+
+defV    +int const                                        +
+--------+-------------------------------------------------+


|function-json_boost-from_JV|

.. _function-_at_json_boost_c__c_from_JV__hh_ptr_hh_JsonValue__hh_const_hh_string__hh_const_hh_string:

.. das:function:: from_JV(v: json::JsonValue?; ent: string const; defV: string const)

from_JV returns auto

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+v       + :ref:`json::JsonValue <struct-json-JsonValue>` ?+
+--------+-------------------------------------------------+
+ent     +string const                                     +
+--------+-------------------------------------------------+
+defV    +string const                                     +
+--------+-------------------------------------------------+


|function-json_boost-from_JV|


