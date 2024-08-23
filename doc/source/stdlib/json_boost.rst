
.. _stdlib_json_boost:

======================
Boost package for JSON
======================

The JSON boost module implements collection of helper macros and functions to accompany :ref:`JSON <stdlib_json>`.

All functions and symbols are in "json_boost" module, use require to get access to it. ::

    require daslib/json_boost


+++++++++++++
Reader macros
+++++++++++++

.. _call-macro-json_boost-json:

.. das:attribute:: json

This macro implements embedding of the JSON object into the program::
  var jsv = %json~
  {
    "name": "main_window",
    "value": 500,
    "size": [1,2,3]
  } %%

++++++++++++++
Variant macros
++++++++++++++

.. _call-macro-json_boost-better_json:

.. das:attribute:: better_json

This macro is used to implement `is json_value` and `as json_value` runtime checks.
It essencially substitutes `value as name` with `value.value as name` and `value is name` with `value.value is name`.

++++++++++++++++
Value conversion
++++++++++++++++

  *  :ref:`JV (v: float) : JsonValue? <function-_at_json_boost_c__c_JV_Cf>` 
  *  :ref:`JV (v: int) : JsonValue? <function-_at_json_boost_c__c_JV_Ci>` 
  *  :ref:`JV (v: bitfield) : JsonValue? <function-_at_json_boost_c__c_JV_Ct>` 
  *  :ref:`JV (val: int8) : JsonValue? <function-_at_json_boost_c__c_JV_Ci8>` 
  *  :ref:`JV (val: uint8) : JsonValue? <function-_at_json_boost_c__c_JV_Cu8>` 
  *  :ref:`JV (val: int16) : JsonValue? <function-_at_json_boost_c__c_JV_Ci16>` 
  *  :ref:`JV (val: uint16) : JsonValue? <function-_at_json_boost_c__c_JV_Cu16>` 
  *  :ref:`JV (val: uint) : JsonValue? <function-_at_json_boost_c__c_JV_Cu>` 
  *  :ref:`JV (val: int64) : JsonValue? <function-_at_json_boost_c__c_JV_Ci64>` 
  *  :ref:`JV (val: uint64) : JsonValue? <function-_at_json_boost_c__c_JV_Cu64>` 
  *  :ref:`from_JV (v: JsonValue?; ent: auto(EnumT); defV: EnumT = [[]]) : EnumT <function-_at_json_boost_c__c_from_JV_C1_ls_XS_ls_json_c__c_JsonValue_gr__gr__qm__CY_ls_EnumT_gr__dot__CY_ls_EnumT_gr_L_%_ls_IsAnyEnumMacro_c_expect_any_enum(ent_eq_true)_gr_>` 
  *  :ref:`from_JV (var v: JsonValue?; ent: string; defV: string = "") : auto <function-_at_json_boost_c__c_from_JV_1_ls_XS_ls_json_c__c_JsonValue_gr__gr__qm__Cs_Cs>` 
  *  :ref:`from_JV (var v: JsonValue?; ent: bool; defV: bool = false) : auto <function-_at_json_boost_c__c_from_JV_1_ls_XS_ls_json_c__c_JsonValue_gr__gr__qm__Cb_Cb>` 
  *  :ref:`from_JV (var v: JsonValue?; ent: float; defV: float = 0f) : auto <function-_at_json_boost_c__c_from_JV_1_ls_XS_ls_json_c__c_JsonValue_gr__gr__qm__Cf_Cf>` 
  *  :ref:`from_JV (var v: JsonValue?; ent: double; defV: double = 0lf) : auto <function-_at_json_boost_c__c_from_JV_1_ls_XS_ls_json_c__c_JsonValue_gr__gr__qm__Cd_Cd>` 
  *  :ref:`from_JV (var v: JsonValue?; ent: int; defV: int = 0) : auto <function-_at_json_boost_c__c_from_JV_1_ls_XS_ls_json_c__c_JsonValue_gr__gr__qm__Ci_Ci>` 
  *  :ref:`from_JV (var v: JsonValue?; ent: uint; defV: uint = 0x0) : auto <function-_at_json_boost_c__c_from_JV_1_ls_XS_ls_json_c__c_JsonValue_gr__gr__qm__Cu_Cu>` 
  *  :ref:`from_JV (var v: JsonValue?; ent: int64; defV: int64 = 0) : auto <function-_at_json_boost_c__c_from_JV_1_ls_XS_ls_json_c__c_JsonValue_gr__gr__qm__Ci64_Ci64>` 
  *  :ref:`from_JV (var v: JsonValue?; ent: uint64; defV: uint64 = 0x0) : auto <function-_at_json_boost_c__c_from_JV_1_ls_XS_ls_json_c__c_JsonValue_gr__gr__qm__Cu64_Cu64>` 
  *  :ref:`from_JV (var v: JsonValue?; ent: int8; defV: int8 = int8(0)) : auto <function-_at_json_boost_c__c_from_JV_1_ls_XS_ls_json_c__c_JsonValue_gr__gr__qm__Ci8_Ci8>` 
  *  :ref:`from_JV (var v: JsonValue?; ent: uint8; defV: uint8 = uint8(0)) : auto <function-_at_json_boost_c__c_from_JV_1_ls_XS_ls_json_c__c_JsonValue_gr__gr__qm__Cu8_Cu8>` 
  *  :ref:`from_JV (var v: JsonValue?; ent: int16; defV: int16 = int16(0)) : auto <function-_at_json_boost_c__c_from_JV_1_ls_XS_ls_json_c__c_JsonValue_gr__gr__qm__Ci16_Ci16>` 
  *  :ref:`from_JV (var v: JsonValue?; ent: uint16; defV: uint16 = uint16(0)) : auto <function-_at_json_boost_c__c_from_JV_1_ls_XS_ls_json_c__c_JsonValue_gr__gr__qm__Cu16_Cu16>` 
  *  :ref:`from_JV (var v: JsonValue?; ent: bitfield; defV: bitfield = [[]]) : auto <function-_at_json_boost_c__c_from_JV_1_ls_XS_ls_json_c__c_JsonValue_gr__gr__qm__Ct_Ct>` 
  *  :ref:`JV (v: auto(VecT)) : auto <function-_at_json_boost_c__c_JV_CY_ls_VecT_gr__dot__%_ls_IsAnyVectorType_c_expect_any_vector_type(v_eq_true)_gr_>` 
  *  :ref:`from_JV (var v: JsonValue?; ent: auto(VecT); defV: VecT = [[]]) : auto <function-_at_json_boost_c__c_from_JV_1_ls_XS_ls_json_c__c_JsonValue_gr__gr__qm__CY_ls_VecT_gr__dot__CY_ls_VecT_gr_L_%_ls_IsAnyVectorType_c_expect_any_vector_type(ent_eq_true)_gr_>` 
  *  :ref:`from_JV (var v: JsonValue?; anything: table\<auto(KT);auto(VT)\>) : auto <function-_at_json_boost_c__c_from_JV_1_ls_XS_ls_json_c__c_JsonValue_gr__gr__qm__C1_ls_Y_ls_KT_gr__dot__gr_2_ls_Y_ls_VT_gr__dot__gr_T>` 
  *  :ref:`from_JV (var v: JsonValue?; anything: auto(TT)) : auto <function-_at_json_boost_c__c_from_JV_1_ls_XS_ls_json_c__c_JsonValue_gr__gr__qm__CY_ls_TT_gr__dot_>` 
  *  :ref:`JV (value: auto) : JsonValue? <function-_at_json_boost_c__c_JV_C_dot_>` 
  *  :ref:`JV (val1: auto; val2: auto) : JsonValue? <function-_at_json_boost_c__c_JV_C_dot__C_dot_>` 
  *  :ref:`JV (val1: auto; val2: auto; val3: auto) : JsonValue? <function-_at_json_boost_c__c_JV_C_dot__C_dot__C_dot_>` 
  *  :ref:`JV (val1: auto; val2: auto; val3: auto; val4: auto) : JsonValue? <function-_at_json_boost_c__c_JV_C_dot__C_dot__C_dot__C_dot_>` 
  *  :ref:`JV (val1: auto; val2: auto; val3: auto; val4: auto; val5: auto) : JsonValue? <function-_at_json_boost_c__c_JV_C_dot__C_dot__C_dot__C_dot__C_dot_>` 
  *  :ref:`JV (val1: auto; val2: auto; val3: auto; val4: auto; val5: auto; val6: auto) : JsonValue? <function-_at_json_boost_c__c_JV_C_dot__C_dot__C_dot__C_dot__C_dot__C_dot_>` 
  *  :ref:`JV (val1: auto; val2: auto; val3: auto; val4: auto; val5: auto; val6: auto; val7: auto) : JsonValue? <function-_at_json_boost_c__c_JV_C_dot__C_dot__C_dot__C_dot__C_dot__C_dot__C_dot_>` 
  *  :ref:`JV (val1: auto; val2: auto; val3: auto; val4: auto; val5: auto; val6: auto; val7: auto; val8: auto) : JsonValue? <function-_at_json_boost_c__c_JV_C_dot__C_dot__C_dot__C_dot__C_dot__C_dot__C_dot__C_dot_>` 
  *  :ref:`JV (val1: auto; val2: auto; val3: auto; val4: auto; val5: auto; val6: auto; val7: auto; val8: auto; val9: auto) : JsonValue? <function-_at_json_boost_c__c_JV_C_dot__C_dot__C_dot__C_dot__C_dot__C_dot__C_dot__C_dot__C_dot_>` 
  *  :ref:`JV (val1: auto; val2: auto; val3: auto; val4: auto; val5: auto; val6: auto; val7: auto; val8: auto; val9: auto; val10: auto) : JsonValue? <function-_at_json_boost_c__c_JV_C_dot__C_dot__C_dot__C_dot__C_dot__C_dot__C_dot__C_dot__C_dot__C_dot_>` 

.. _function-_at_json_boost_c__c_JV_Cf:

.. das:function:: JV(v: float) : JsonValue?

|detail/function-json_boost-JV-0x57f12a4b472e2cfa|

:Arguments: * **v** : float

.. _function-_at_json_boost_c__c_JV_Ci:

.. das:function:: JV(v: int) : JsonValue?

|detail/function-json_boost-JV-0xd6b497ad2b89b138|

:Arguments: * **v** : int

.. _function-_at_json_boost_c__c_JV_Ct:

.. das:function:: JV(v: bitfield) : JsonValue?

|detail/function-json_boost-JV-0xdc04f9425da5182e|

:Arguments: * **v** : bitfield<>

.. _function-_at_json_boost_c__c_JV_Ci8:

.. das:function:: JV(val: int8) : JsonValue?

|detail/function-json_boost-JV-0xd67c97ad2b2a8938|

:Arguments: * **val** : int8

.. _function-_at_json_boost_c__c_JV_Cu8:

.. das:function:: JV(val: uint8) : JsonValue?

|detail/function-json_boost-JV-0x1707d332fa20c62e|

:Arguments: * **val** : uint8

.. _function-_at_json_boost_c__c_JV_Ci16:

.. das:function:: JV(val: int16) : JsonValue?

|detail/function-json_boost-JV-0xb808c9406d7d01ca|

:Arguments: * **val** : int16

.. _function-_at_json_boost_c__c_JV_Cu16:

.. das:function:: JV(val: uint16) : JsonValue?

|detail/function-json_boost-JV-0x16d1cc32f9c4f849|

:Arguments: * **val** : uint16

.. _function-_at_json_boost_c__c_JV_Cu:

.. das:function:: JV(val: uint) : JsonValue?

|detail/function-json_boost-JV-0xccada1ad234bfe22|

:Arguments: * **val** : uint

.. _function-_at_json_boost_c__c_JV_Ci64:

.. das:function:: JV(val: int64) : JsonValue?

|detail/function-json_boost-JV-0xb4a2c7406a99d564|

:Arguments: * **val** : int64

.. _function-_at_json_boost_c__c_JV_Cu64:

.. das:function:: JV(val: uint64) : JsonValue?

|detail/function-json_boost-JV-0x16d3cd32f9c85ffc|

:Arguments: * **val** : uint64

.. _function-_at_json_boost_c__c_from_JV_C1_ls_XS_ls_json_c__c_JsonValue_gr__gr__qm__CY_ls_EnumT_gr__dot__CY_ls_EnumT_gr_L_%_ls_IsAnyEnumMacro_c_expect_any_enum(ent_eq_true)_gr_:

.. das:function:: from_JV(v: JsonValue?; ent: auto(EnumT); defV: EnumT = [[]]) : EnumT

Parse a JSON value and return the corresponding native value.

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : auto(EnumT)

            * **defV** : EnumT

.. _function-_at_json_boost_c__c_from_JV_1_ls_XS_ls_json_c__c_JsonValue_gr__gr__qm__Cs_Cs:

.. das:function:: from_JV(v: JsonValue?; ent: string; defV: string = "") : auto

|detail/function-json_boost-from_JV-0x5ebbaa0bd0039185|

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : string

            * **defV** : string

.. _function-_at_json_boost_c__c_from_JV_1_ls_XS_ls_json_c__c_JsonValue_gr__gr__qm__Cb_Cb:

.. das:function:: from_JV(v: JsonValue?; ent: bool; defV: bool = false) : auto

|detail/function-json_boost-from_JV-0x3f72388d1d47eb49|

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : bool

            * **defV** : bool

.. _function-_at_json_boost_c__c_from_JV_1_ls_XS_ls_json_c__c_JsonValue_gr__gr__qm__Cf_Cf:

.. das:function:: from_JV(v: JsonValue?; ent: float; defV: float = 0f) : auto

|detail/function-json_boost-from_JV-0xdd2b0182200233ff|

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : float

            * **defV** : float

.. _function-_at_json_boost_c__c_from_JV_1_ls_XS_ls_json_c__c_JsonValue_gr__gr__qm__Cd_Cd:

.. das:function:: from_JV(v: JsonValue?; ent: double; defV: double = 0lf) : auto

|detail/function-json_boost-from_JV-0x8a04be7aa32d0ee9|

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : double

            * **defV** : double

.. _function-_at_json_boost_c__c_from_JV_1_ls_XS_ls_json_c__c_JsonValue_gr__gr__qm__Ci_Ci:

.. das:function:: from_JV(v: JsonValue?; ent: int; defV: int = 0) : auto

|detail/function-json_boost-from_JV-0xf010e7feab187c60|

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : int

            * **defV** : int

.. _function-_at_json_boost_c__c_from_JV_1_ls_XS_ls_json_c__c_JsonValue_gr__gr__qm__Cu_Cu:

.. das:function:: from_JV(v: JsonValue?; ent: uint; defV: uint = 0x0) : auto

|detail/function-json_boost-from_JV-0x4eff717d1595ee71|

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : uint

            * **defV** : uint

.. _function-_at_json_boost_c__c_from_JV_1_ls_XS_ls_json_c__c_JsonValue_gr__gr__qm__Ci64_Ci64:

.. das:function:: from_JV(v: JsonValue?; ent: int64; defV: int64 = 0) : auto

|detail/function-json_boost-from_JV-0x1eb69015029118c8|

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : int64

            * **defV** : int64

.. _function-_at_json_boost_c__c_from_JV_1_ls_XS_ls_json_c__c_JsonValue_gr__gr__qm__Cu64_Cu64:

.. das:function:: from_JV(v: JsonValue?; ent: uint64; defV: uint64 = 0x0) : auto

|detail/function-json_boost-from_JV-0x68e3fe66e8dfc1c3|

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : uint64

            * **defV** : uint64

.. _function-_at_json_boost_c__c_from_JV_1_ls_XS_ls_json_c__c_JsonValue_gr__gr__qm__Ci8_Ci8:

.. das:function:: from_JV(v: JsonValue?; ent: int8; defV: int8 = int8(0)) : auto

|detail/function-json_boost-from_JV-0x80206cca1356d4f9|

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : int8

            * **defV** : int8

.. _function-_at_json_boost_c__c_from_JV_1_ls_XS_ls_json_c__c_JsonValue_gr__gr__qm__Cu8_Cu8:

.. das:function:: from_JV(v: JsonValue?; ent: uint8; defV: uint8 = uint8(0)) : auto

|detail/function-json_boost-from_JV-0x4fae4ef61e6068d5|

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : uint8

            * **defV** : uint8

.. _function-_at_json_boost_c__c_from_JV_1_ls_XS_ls_json_c__c_JsonValue_gr__gr__qm__Ci16_Ci16:

.. das:function:: from_JV(v: JsonValue?; ent: int16; defV: int16 = int16(0)) : auto

|detail/function-json_boost-from_JV-0x11fdbe4f91603c45|

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : int16

            * **defV** : int16

.. _function-_at_json_boost_c__c_from_JV_1_ls_XS_ls_json_c__c_JsonValue_gr__gr__qm__Cu16_Cu16:

.. das:function:: from_JV(v: JsonValue?; ent: uint16; defV: uint16 = uint16(0)) : auto

|detail/function-json_boost-from_JV-0x1eee800a651388af|

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : uint16

            * **defV** : uint16

.. _function-_at_json_boost_c__c_from_JV_1_ls_XS_ls_json_c__c_JsonValue_gr__gr__qm__Ct_Ct:

.. das:function:: from_JV(v: JsonValue?; ent: bitfield; defV: bitfield = [[]]) : auto

|detail/function-json_boost-from_JV-0x5e37354677e9a379|

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : bitfield<>

            * **defV** : bitfield<>

.. _function-_at_json_boost_c__c_JV_CY_ls_VecT_gr__dot__%_ls_IsAnyVectorType_c_expect_any_vector_type(v_eq_true)_gr_:

.. das:function:: JV(v: auto(VecT)) : auto

Creates `JsonValue` out of value.

:Arguments: * **v** : auto(VecT)

.. _function-_at_json_boost_c__c_from_JV_1_ls_XS_ls_json_c__c_JsonValue_gr__gr__qm__CY_ls_VecT_gr__dot__CY_ls_VecT_gr_L_%_ls_IsAnyVectorType_c_expect_any_vector_type(ent_eq_true)_gr_:

.. das:function:: from_JV(v: JsonValue?; ent: auto(VecT); defV: VecT = [[]]) : auto

|detail/function-json_boost-from_JV-0x64e205fb0c4ff548|

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : auto(VecT)

            * **defV** : VecT

.. _function-_at_json_boost_c__c_from_JV_1_ls_XS_ls_json_c__c_JsonValue_gr__gr__qm__C1_ls_Y_ls_KT_gr__dot__gr_2_ls_Y_ls_VT_gr__dot__gr_T:

.. das:function:: from_JV(v: JsonValue?; anything: table<auto(KT);auto(VT)>) : auto

|detail/function-json_boost-from_JV-0x8cc34b69b0fc1b2d|

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **anything** : table<auto(KT);auto(VT)>

.. _function-_at_json_boost_c__c_from_JV_1_ls_XS_ls_json_c__c_JsonValue_gr__gr__qm__CY_ls_TT_gr__dot_:

.. das:function:: from_JV(v: JsonValue?; anything: auto(TT)) : auto

|detail/function-json_boost-from_JV-0x1b04f7e9055ef822|

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **anything** : auto(TT)

.. _function-_at_json_boost_c__c_JV_C_dot_:

.. das:function:: JV(value: auto) : JsonValue?

|detail/function-json_boost-JV-0x2ba887ad739e17c0|

:Arguments: * **value** : auto

.. _function-_at_json_boost_c__c_JV_C_dot__C_dot_:

.. das:function:: JV(val1: auto; val2: auto) : JsonValue?

|detail/function-json_boost-JV-0xc137e989bbb9f085|

:Arguments: * **val1** : auto

            * **val2** : auto

.. _function-_at_json_boost_c__c_JV_C_dot__C_dot__C_dot_:

.. das:function:: JV(val1: auto; val2: auto; val3: auto) : JsonValue?

|detail/function-json_boost-JV-0xd68c83f79eecdfc8|

:Arguments: * **val1** : auto

            * **val2** : auto

            * **val3** : auto

.. _function-_at_json_boost_c__c_JV_C_dot__C_dot__C_dot__C_dot_:

.. das:function:: JV(val1: auto; val2: auto; val3: auto; val4: auto) : JsonValue?

|detail/function-json_boost-JV-0x49047c6da3d7b76d|

:Arguments: * **val1** : auto

            * **val2** : auto

            * **val3** : auto

            * **val4** : auto

.. _function-_at_json_boost_c__c_JV_C_dot__C_dot__C_dot__C_dot__C_dot_:

.. das:function:: JV(val1: auto; val2: auto; val3: auto; val4: auto; val5: auto) : JsonValue?

|detail/function-json_boost-JV-0xe277e18712e40a30|

:Arguments: * **val1** : auto

            * **val2** : auto

            * **val3** : auto

            * **val4** : auto

            * **val5** : auto

.. _function-_at_json_boost_c__c_JV_C_dot__C_dot__C_dot__C_dot__C_dot__C_dot_:

.. das:function:: JV(val1: auto; val2: auto; val3: auto; val4: auto; val5: auto; val6: auto) : JsonValue?

|detail/function-json_boost-JV-0xddcc842ba1ee4855|

:Arguments: * **val1** : auto

            * **val2** : auto

            * **val3** : auto

            * **val4** : auto

            * **val5** : auto

            * **val6** : auto

.. _function-_at_json_boost_c__c_JV_C_dot__C_dot__C_dot__C_dot__C_dot__C_dot__C_dot_:

.. das:function:: JV(val1: auto; val2: auto; val3: auto; val4: auto; val5: auto; val6: auto; val7: auto) : JsonValue?

|detail/function-json_boost-JV-0x8adaa36fa917f3f8|

:Arguments: * **val1** : auto

            * **val2** : auto

            * **val3** : auto

            * **val4** : auto

            * **val5** : auto

            * **val6** : auto

            * **val7** : auto

.. _function-_at_json_boost_c__c_JV_C_dot__C_dot__C_dot__C_dot__C_dot__C_dot__C_dot__C_dot_:

.. das:function:: JV(val1: auto; val2: auto; val3: auto; val4: auto; val5: auto; val6: auto; val7: auto; val8: auto) : JsonValue?

|detail/function-json_boost-JV-0x56600c00fe8e60fd|

:Arguments: * **val1** : auto

            * **val2** : auto

            * **val3** : auto

            * **val4** : auto

            * **val5** : auto

            * **val6** : auto

            * **val7** : auto

            * **val8** : auto

.. _function-_at_json_boost_c__c_JV_C_dot__C_dot__C_dot__C_dot__C_dot__C_dot__C_dot__C_dot__C_dot_:

.. das:function:: JV(val1: auto; val2: auto; val3: auto; val4: auto; val5: auto; val6: auto; val7: auto; val8: auto; val9: auto) : JsonValue?

|detail/function-json_boost-JV-0xf5983efced57f820|

:Arguments: * **val1** : auto

            * **val2** : auto

            * **val3** : auto

            * **val4** : auto

            * **val5** : auto

            * **val6** : auto

            * **val7** : auto

            * **val8** : auto

            * **val9** : auto

.. _function-_at_json_boost_c__c_JV_C_dot__C_dot__C_dot__C_dot__C_dot__C_dot__C_dot__C_dot__C_dot__C_dot_:

.. das:function:: JV(val1: auto; val2: auto; val3: auto; val4: auto; val5: auto; val6: auto; val7: auto; val8: auto; val9: auto; val10: auto) : JsonValue?

|detail/function-json_boost-JV-0x5da5b1a5c07329e5|

:Arguments: * **val1** : auto

            * **val2** : auto

            * **val3** : auto

            * **val4** : auto

            * **val5** : auto

            * **val6** : auto

            * **val7** : auto

            * **val8** : auto

            * **val9** : auto

            * **val10** : auto

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_json_boost_c__c__qm__lb__rb__C_eq_1_ls_S_ls_json_c__c_JsonValue_gr__gr__qm__Cs:

.. das:function:: JsonValue? const ==const?[](a: JsonValue? const ==const; key: string) : JsonValue?

Returns the value of the key in the JSON object, if it exists.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?!

            * **key** : string

.. _function-_at_json_boost_c__c__qm__lb__rb___eq_1_ls_S_ls_json_c__c_JsonValue_gr__gr__qm__Cs:

.. das:function:: JsonValue? ==const?[](a: JsonValue? ==const; key: string) : JsonValue?

Returns the value of the key in the JSON object, if it exists.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?!

            * **key** : string

.. _function-_at_json_boost_c__c__qm__dot__C_eq_1_ls_S_ls_json_c__c_JsonValue_gr__gr__qm__Cs:

.. das:function:: JsonValue? const ==const?.(a: JsonValue? const ==const; key: string) : JsonValue?

Returns the value of the key in the JSON object, if it exists.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?!

            * **key** : string

.. _function-_at_json_boost_c__c__qm__dot___eq_1_ls_S_ls_json_c__c_JsonValue_gr__gr__qm__Cs:

.. das:function:: JsonValue? ==const?.(a: JsonValue? ==const; key: string) : JsonValue?

Returns the value of the key in the JSON object, if it exists.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?!

            * **key** : string

.. _function-_at_json_boost_c__c__qm__lb__rb__C_eq_1_ls_S_ls_json_c__c_JsonValue_gr__gr__qm__Ci:

.. das:function:: JsonValue? const ==const?[](a: JsonValue? const ==const; idx: int) : JsonValue?

Returns the value of the index in the JSON array, if it exists.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?!

            * **idx** : int

.. _function-_at_json_boost_c__c__qm__lb__rb___eq_1_ls_S_ls_json_c__c_JsonValue_gr__gr__qm__Ci:

.. das:function:: JsonValue? ==const?[](a: JsonValue? ==const; idx: int) : JsonValue?

Returns the value of the index in the JSON array, if it exists.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?!

            * **idx** : int

.. _function-_at_json_boost_c__c__qm__qm__C1_ls_S_ls_json_c__c_JsonValue_gr__gr__qm__Cd:

.. das:function:: JsonValue???(a: JsonValue?; val: double) : double

Returns the value of the JSON object, if it exists, otherwise returns the default value.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **val** : double

.. _function-_at_json_boost_c__c__qm__qm__C1_ls_S_ls_json_c__c_JsonValue_gr__gr__qm__Cf:

.. das:function:: JsonValue???(a: JsonValue?; val: float) : float

Returns the value of the JSON object, if it exists, otherwise returns the default value.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **val** : float

.. _function-_at_json_boost_c__c__qm__qm__C1_ls_S_ls_json_c__c_JsonValue_gr__gr__qm__Ci8:

.. das:function:: JsonValue???(a: JsonValue?; val: int8) : int8

Returns the value of the JSON object, if it exists, otherwise returns the default value.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **val** : int8

.. _function-_at_json_boost_c__c__qm__qm__C1_ls_S_ls_json_c__c_JsonValue_gr__gr__qm__Ci16:

.. das:function:: JsonValue???(a: JsonValue?; val: int16) : int16

Returns the value of the JSON object, if it exists, otherwise returns the default value.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **val** : int16

.. _function-_at_json_boost_c__c__qm__qm__C1_ls_S_ls_json_c__c_JsonValue_gr__gr__qm__Ci:

.. das:function:: JsonValue???(a: JsonValue?; val: int) : int

Returns the value of the JSON object, if it exists, otherwise returns the default value.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **val** : int

.. _function-_at_json_boost_c__c__qm__qm__C1_ls_S_ls_json_c__c_JsonValue_gr__gr__qm__Ci64:

.. das:function:: JsonValue???(a: JsonValue?; val: int64) : int64

Returns the value of the JSON object, if it exists, otherwise returns the default value.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **val** : int64

.. _function-_at_json_boost_c__c__qm__qm__C1_ls_S_ls_json_c__c_JsonValue_gr__gr__qm__Cu8:

.. das:function:: JsonValue???(a: JsonValue?; val: uint8) : uint8

Returns the value of the JSON object, if it exists, otherwise returns the default value.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **val** : uint8

.. _function-_at_json_boost_c__c__qm__qm__C1_ls_S_ls_json_c__c_JsonValue_gr__gr__qm__Cu16:

.. das:function:: JsonValue???(a: JsonValue?; val: uint16) : uint16

Returns the value of the JSON object, if it exists, otherwise returns the default value.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **val** : uint16

.. _function-_at_json_boost_c__c__qm__qm__C1_ls_S_ls_json_c__c_JsonValue_gr__gr__qm__Cu:

.. das:function:: JsonValue???(a: JsonValue?; val: uint) : uint

Returns the value of the JSON object, if it exists, otherwise returns the default value.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **val** : uint

.. _function-_at_json_boost_c__c__qm__qm__C1_ls_S_ls_json_c__c_JsonValue_gr__gr__qm__Cu64:

.. das:function:: JsonValue???(a: JsonValue?; val: uint64) : uint64

Returns the value of the JSON object, if it exists, otherwise returns the default value.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **val** : uint64

.. _function-_at_json_boost_c__c__qm__qm__C1_ls_S_ls_json_c__c_JsonValue_gr__gr__qm__Cb:

.. das:function:: JsonValue???(a: JsonValue?; val: bool) : bool

Returns the value of the JSON object, if it exists, otherwise returns the default value.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **val** : bool

.. _function-_at_json_boost_c__c__qm__qm__C1_ls_S_ls_json_c__c_JsonValue_gr__gr__qm__Cs:

.. das:function:: JsonValue???(a: JsonValue?; val: string) : string

Returns the value of the JSON object, if it exists, otherwise returns the default value.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **val** : string

.. _function-_at_json_boost_c__c__qm__dot__rq_value_C_eq_1_ls_S_ls_json_c__c_JsonValue_gr__gr__qm_:

.. das:function:: JsonValue? const ==const?.`value(a: JsonValue? const ==const) : variant<_object:table<string;JsonValue?>;_array:array<JsonValue?>;_string:string;_number:double;_bool:bool;_null:void?> const?

Returns the value of the JSON object, if it exists.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?!

.. _function-_at_json_boost_c__c__qm__dot__rq_value__eq_1_ls_S_ls_json_c__c_JsonValue_gr__gr__qm_:

.. das:function:: JsonValue? ==const?.`value(a: JsonValue? ==const) : variant<_object:table<string;JsonValue?>;_array:array<JsonValue?>;_string:string;_number:double;_bool:bool;_null:void?>?

Returns the value of the JSON object, if it exists.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?!


