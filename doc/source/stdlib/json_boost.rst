
.. _stdlib_json_boost:

======================
Boost package for JSON
======================

.. include:: detail/json_boost.rst

The JSON boost module implements collection of helper macros and functions to accompany :ref:`JSON <stdlib_json>`.

All functions and symbols are in "json_boost" module, use require to get access to it. ::

    require daslib/json_boost


+++++++++++++
Reader macros
+++++++++++++

.. _call-macro-json_boost-json:

.. das:attribute:: json

|reader_macro-json_boost-json|

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_json_boost_c__c_JV_Cf:

.. das:function:: JV(v: float const)

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

+--------+-------------+
+argument+argument type+
+========+=============+
+v       +float const  +
+--------+-------------+


|function-json_boost-JV|

.. _function-_at_json_boost_c__c_JV_Ci:

.. das:function:: JV(v: int const)

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

+--------+-------------+
+argument+argument type+
+========+=============+
+v       +int const    +
+--------+-------------+


|function-json_boost-JV|

.. _function-_at_json_boost_c__c_JV_Ct:

.. das:function:: JV(v: bitfield const)

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

+--------+----------------+
+argument+argument type   +
+========+================+
+v       +bitfield<> const+
+--------+----------------+


|function-json_boost-JV|

.. _function-_at_json_boost_c__c_JV_Ci8:

.. das:function:: JV(val: int8 const)

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

+--------+-------------+
+argument+argument type+
+========+=============+
+val     +int8 const   +
+--------+-------------+


|function-json_boost-JV|

.. _function-_at_json_boost_c__c_JV_Cu8:

.. das:function:: JV(val: uint8 const)

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

+--------+-------------+
+argument+argument type+
+========+=============+
+val     +uint8 const  +
+--------+-------------+


|function-json_boost-JV|

.. _function-_at_json_boost_c__c_JV_Ci16:

.. das:function:: JV(val: int16 const)

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

+--------+-------------+
+argument+argument type+
+========+=============+
+val     +int16 const  +
+--------+-------------+


|function-json_boost-JV|

.. _function-_at_json_boost_c__c_JV_Cu16:

.. das:function:: JV(val: uint16 const)

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

+--------+-------------+
+argument+argument type+
+========+=============+
+val     +uint16 const +
+--------+-------------+


|function-json_boost-JV|

.. _function-_at_json_boost_c__c_JV_Cu:

.. das:function:: JV(val: uint const)

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

+--------+-------------+
+argument+argument type+
+========+=============+
+val     +uint const   +
+--------+-------------+


|function-json_boost-JV|

.. _function-_at_json_boost_c__c_JV_Ci64:

.. das:function:: JV(val: int64 const)

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

+--------+-------------+
+argument+argument type+
+========+=============+
+val     +int64 const  +
+--------+-------------+


|function-json_boost-JV|

.. _function-_at_json_boost_c__c_JV_Cu64:

.. das:function:: JV(val: uint64 const)

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

+--------+-------------+
+argument+argument type+
+========+=============+
+val     +uint64 const +
+--------+-------------+


|function-json_boost-JV|

.. _function-_at_json_boost_c__c_from_JV_C1_ls_S_ls_JsonValue_gr__gr_?_CY_ls_EnumT_gr_._CY_ls_EnumT_gr_L:

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

.. _function-_at_json_boost_c__c_from_JV_1_ls_S_ls_JsonValue_gr__gr_?_Cs_Cs:

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

.. _function-_at_json_boost_c__c_from_JV_1_ls_S_ls_JsonValue_gr__gr_?_Cb_Cb:

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

.. _function-_at_json_boost_c__c_from_JV_1_ls_S_ls_JsonValue_gr__gr_?_Cf_Cf:

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

.. _function-_at_json_boost_c__c_from_JV_1_ls_S_ls_JsonValue_gr__gr_?_Cd_Cd:

.. das:function:: from_JV(v: json::JsonValue?; ent: double const; defV: double const)

from_JV returns auto

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+v       + :ref:`json::JsonValue <struct-json-JsonValue>` ?+
+--------+-------------------------------------------------+
+ent     +double const                                     +
+--------+-------------------------------------------------+
+defV    +double const                                     +
+--------+-------------------------------------------------+


|function-json_boost-from_JV|

.. _function-_at_json_boost_c__c_from_JV_1_ls_S_ls_JsonValue_gr__gr_?_Ci_Ci:

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

.. _function-_at_json_boost_c__c_from_JV_1_ls_S_ls_JsonValue_gr__gr_?_Cu_Cu:

.. das:function:: from_JV(v: json::JsonValue?; ent: uint const; defV: uint const)

from_JV returns auto

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+v       + :ref:`json::JsonValue <struct-json-JsonValue>` ?+
+--------+-------------------------------------------------+
+ent     +uint const                                       +
+--------+-------------------------------------------------+
+defV    +uint const                                       +
+--------+-------------------------------------------------+


|function-json_boost-from_JV|

.. _function-_at_json_boost_c__c_from_JV_1_ls_S_ls_JsonValue_gr__gr_?_Ci64_Ci64:

.. das:function:: from_JV(v: json::JsonValue?; ent: int64 const; defV: int64 const)

from_JV returns auto

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+v       + :ref:`json::JsonValue <struct-json-JsonValue>` ?+
+--------+-------------------------------------------------+
+ent     +int64 const                                      +
+--------+-------------------------------------------------+
+defV    +int64 const                                      +
+--------+-------------------------------------------------+


|function-json_boost-from_JV|

.. _function-_at_json_boost_c__c_from_JV_1_ls_S_ls_JsonValue_gr__gr_?_Cu64_Cu64:

.. das:function:: from_JV(v: json::JsonValue?; ent: uint64 const; defV: uint64 const)

from_JV returns auto

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+v       + :ref:`json::JsonValue <struct-json-JsonValue>` ?+
+--------+-------------------------------------------------+
+ent     +uint64 const                                     +
+--------+-------------------------------------------------+
+defV    +uint64 const                                     +
+--------+-------------------------------------------------+


|function-json_boost-from_JV|

.. _function-_at_json_boost_c__c_from_JV_1_ls_S_ls_JsonValue_gr__gr_?_Ci8_Ci8:

.. das:function:: from_JV(v: json::JsonValue?; ent: int8 const; defV: int8 const)

from_JV returns auto

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+v       + :ref:`json::JsonValue <struct-json-JsonValue>` ?+
+--------+-------------------------------------------------+
+ent     +int8 const                                       +
+--------+-------------------------------------------------+
+defV    +int8 const                                       +
+--------+-------------------------------------------------+


|function-json_boost-from_JV|

.. _function-_at_json_boost_c__c_from_JV_1_ls_S_ls_JsonValue_gr__gr_?_Cu8_Cu8:

.. das:function:: from_JV(v: json::JsonValue?; ent: uint8 const; defV: uint8 const)

from_JV returns auto

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+v       + :ref:`json::JsonValue <struct-json-JsonValue>` ?+
+--------+-------------------------------------------------+
+ent     +uint8 const                                      +
+--------+-------------------------------------------------+
+defV    +uint8 const                                      +
+--------+-------------------------------------------------+


|function-json_boost-from_JV|

.. _function-_at_json_boost_c__c_from_JV_1_ls_S_ls_JsonValue_gr__gr_?_Ci16_Ci16:

.. das:function:: from_JV(v: json::JsonValue?; ent: int16 const; defV: int16 const)

from_JV returns auto

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+v       + :ref:`json::JsonValue <struct-json-JsonValue>` ?+
+--------+-------------------------------------------------+
+ent     +int16 const                                      +
+--------+-------------------------------------------------+
+defV    +int16 const                                      +
+--------+-------------------------------------------------+


|function-json_boost-from_JV|

.. _function-_at_json_boost_c__c_from_JV_1_ls_S_ls_JsonValue_gr__gr_?_Cu16_Cu16:

.. das:function:: from_JV(v: json::JsonValue?; ent: uint16 const; defV: uint16 const)

from_JV returns auto

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+v       + :ref:`json::JsonValue <struct-json-JsonValue>` ?+
+--------+-------------------------------------------------+
+ent     +uint16 const                                     +
+--------+-------------------------------------------------+
+defV    +uint16 const                                     +
+--------+-------------------------------------------------+


|function-json_boost-from_JV|

.. _function-_at_json_boost_c__c_from_JV_1_ls_S_ls_JsonValue_gr__gr_?_Ct_Ct:

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

.. _function-_at_json_boost_c__c_JV_CY_ls_VecT_gr_.:

.. das:function:: JV(v: auto(VecT) const)

JV returns auto

+--------+----------------+
+argument+argument type   +
+========+================+
+v       +auto(VecT) const+
+--------+----------------+


|function-json_boost-JV|

.. _function-_at_json_boost_c__c_from_JV_1_ls_S_ls_JsonValue_gr__gr_?_CY_ls_VecT_gr_._CY_ls_VecT_gr_L:

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

.. _function-_at_json_boost_c__c_from_JV_1_ls_S_ls_JsonValue_gr__gr_?_C1_ls_Y_ls_KT_gr_._gr_2_ls_Y_ls_VT_gr_._gr_T:

.. das:function:: from_JV(v: json::JsonValue?; anything: table<auto(KT);auto(VT)> const)

from_JV returns auto

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+v       + :ref:`json::JsonValue <struct-json-JsonValue>` ?+
+--------+-------------------------------------------------+
+anything+table<auto(KT);auto(VT)> const                   +
+--------+-------------------------------------------------+


|function-json_boost-from_JV|

.. _function-_at_json_boost_c__c_from_JV_1_ls_S_ls_JsonValue_gr__gr_?_CY_ls_TT_gr_.:

.. das:function:: from_JV(v: json::JsonValue?; anything: auto(TT) const)

from_JV returns auto

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+v       + :ref:`json::JsonValue <struct-json-JsonValue>` ?+
+--------+-------------------------------------------------+
+anything+auto(TT) const                                   +
+--------+-------------------------------------------------+


|function-json_boost-from_JV|

.. _function-_at_json_boost_c__c_JV_C.:

.. das:function:: JV(value: auto const)

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

+--------+-------------+
+argument+argument type+
+========+=============+
+value   +auto const   +
+--------+-------------+


|function-json_boost-JV|

.. _function-_at_json_boost_c__c_JV_C._C.:

.. das:function:: JV(val1: auto const; val2: auto const)

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

+--------+-------------+
+argument+argument type+
+========+=============+
+val1    +auto const   +
+--------+-------------+
+val2    +auto const   +
+--------+-------------+


|function-json_boost-JV|

.. _function-_at_json_boost_c__c_JV_C._C._C.:

.. das:function:: JV(val1: auto const; val2: auto const; val3: auto const)

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

+--------+-------------+
+argument+argument type+
+========+=============+
+val1    +auto const   +
+--------+-------------+
+val2    +auto const   +
+--------+-------------+
+val3    +auto const   +
+--------+-------------+


|function-json_boost-JV|

.. _function-_at_json_boost_c__c_JV_C._C._C._C.:

.. das:function:: JV(val1: auto const; val2: auto const; val3: auto const; val4: auto const)

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

+--------+-------------+
+argument+argument type+
+========+=============+
+val1    +auto const   +
+--------+-------------+
+val2    +auto const   +
+--------+-------------+
+val3    +auto const   +
+--------+-------------+
+val4    +auto const   +
+--------+-------------+


|function-json_boost-JV|

.. _function-_at_json_boost_c__c_JV_C._C._C._C._C.:

.. das:function:: JV(val1: auto const; val2: auto const; val3: auto const; val4: auto const; val5: auto const)

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

+--------+-------------+
+argument+argument type+
+========+=============+
+val1    +auto const   +
+--------+-------------+
+val2    +auto const   +
+--------+-------------+
+val3    +auto const   +
+--------+-------------+
+val4    +auto const   +
+--------+-------------+
+val5    +auto const   +
+--------+-------------+


|function-json_boost-JV|

.. _function-_at_json_boost_c__c_JV_C._C._C._C._C._C.:

.. das:function:: JV(val1: auto const; val2: auto const; val3: auto const; val4: auto const; val5: auto const; val6: auto const)

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

+--------+-------------+
+argument+argument type+
+========+=============+
+val1    +auto const   +
+--------+-------------+
+val2    +auto const   +
+--------+-------------+
+val3    +auto const   +
+--------+-------------+
+val4    +auto const   +
+--------+-------------+
+val5    +auto const   +
+--------+-------------+
+val6    +auto const   +
+--------+-------------+


|function-json_boost-JV|

.. _function-_at_json_boost_c__c_JV_C._C._C._C._C._C._C.:

.. das:function:: JV(val1: auto const; val2: auto const; val3: auto const; val4: auto const; val5: auto const; val6: auto const; val7: auto const)

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

+--------+-------------+
+argument+argument type+
+========+=============+
+val1    +auto const   +
+--------+-------------+
+val2    +auto const   +
+--------+-------------+
+val3    +auto const   +
+--------+-------------+
+val4    +auto const   +
+--------+-------------+
+val5    +auto const   +
+--------+-------------+
+val6    +auto const   +
+--------+-------------+
+val7    +auto const   +
+--------+-------------+


|function-json_boost-JV|

.. _function-_at_json_boost_c__c_JV_C._C._C._C._C._C._C._C.:

.. das:function:: JV(val1: auto const; val2: auto const; val3: auto const; val4: auto const; val5: auto const; val6: auto const; val7: auto const; val8: auto const)

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

+--------+-------------+
+argument+argument type+
+========+=============+
+val1    +auto const   +
+--------+-------------+
+val2    +auto const   +
+--------+-------------+
+val3    +auto const   +
+--------+-------------+
+val4    +auto const   +
+--------+-------------+
+val5    +auto const   +
+--------+-------------+
+val6    +auto const   +
+--------+-------------+
+val7    +auto const   +
+--------+-------------+
+val8    +auto const   +
+--------+-------------+


|function-json_boost-JV|

.. _function-_at_json_boost_c__c_JV_C._C._C._C._C._C._C._C._C.:

.. das:function:: JV(val1: auto const; val2: auto const; val3: auto const; val4: auto const; val5: auto const; val6: auto const; val7: auto const; val8: auto const; val9: auto const)

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

+--------+-------------+
+argument+argument type+
+========+=============+
+val1    +auto const   +
+--------+-------------+
+val2    +auto const   +
+--------+-------------+
+val3    +auto const   +
+--------+-------------+
+val4    +auto const   +
+--------+-------------+
+val5    +auto const   +
+--------+-------------+
+val6    +auto const   +
+--------+-------------+
+val7    +auto const   +
+--------+-------------+
+val8    +auto const   +
+--------+-------------+
+val9    +auto const   +
+--------+-------------+


|function-json_boost-JV|

.. _function-_at_json_boost_c__c_JV_C._C._C._C._C._C._C._C._C._C.:

.. das:function:: JV(val1: auto const; val2: auto const; val3: auto const; val4: auto const; val5: auto const; val6: auto const; val7: auto const; val8: auto const; val9: auto const; val10: auto const)

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

+--------+-------------+
+argument+argument type+
+========+=============+
+val1    +auto const   +
+--------+-------------+
+val2    +auto const   +
+--------+-------------+
+val3    +auto const   +
+--------+-------------+
+val4    +auto const   +
+--------+-------------+
+val5    +auto const   +
+--------+-------------+
+val6    +auto const   +
+--------+-------------+
+val7    +auto const   +
+--------+-------------+
+val8    +auto const   +
+--------+-------------+
+val9    +auto const   +
+--------+-------------+
+val10   +auto const   +
+--------+-------------+


|function-json_boost-JV|


