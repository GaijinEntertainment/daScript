
.. _stdlib_json_boost:

======================
Boost package for JSON
======================

The JSON boost module implements collection of helper macros and functions to accompany :ref:`JSON <stdlib_json>`.

All functions and symbols are in "json_boost" module, use require to get access to it. ::

    require daslib/json_boost

++++++++++
Structures
++++++++++

.. _struct-json_boost-JsonFieldState:

.. das:attribute:: JsonFieldState

|detail/structure-json_boost-JsonFieldState|

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

  *  :ref:`from_JV (v: JsonValue const?; ent: auto(EnumT); defV: EnumT = EnumT()) : EnumT <function-json_boost_from_JV_JsonValue_const_q__autoEnumT_EnumT>` 
  *  :ref:`from_JV (v: JsonValue const?; ent: string; defV: string = "") : auto <function-json_boost_from_JV_JsonValue_const_q__string_string>` 
  *  :ref:`from_JV (v: JsonValue const?; ent: bool; defV: bool = false) : auto <function-json_boost_from_JV_JsonValue_const_q__bool_bool>` 
  *  :ref:`from_JV (v: JsonValue const?; ent: float; defV: float = 0f) : auto <function-json_boost_from_JV_JsonValue_const_q__float_float>` 
  *  :ref:`from_JV (v: JsonValue const?; ent: double; defV: double = 0lf) : auto <function-json_boost_from_JV_JsonValue_const_q__double_double>` 
  *  :ref:`from_JV (v: JsonValue const?; ent: int; defV: int = 0) : auto <function-json_boost_from_JV_JsonValue_const_q__int_int>` 
  *  :ref:`from_JV (v: JsonValue const?; ent: uint; defV: uint = 0x0) : auto <function-json_boost_from_JV_JsonValue_const_q__uint_uint>` 
  *  :ref:`from_JV (v: JsonValue const?; ent: int64; defV: int64 = 0) : auto <function-json_boost_from_JV_JsonValue_const_q__int64_int64>` 
  *  :ref:`from_JV (v: JsonValue const?; ent: uint64; defV: uint64 = 0x0) : auto <function-json_boost_from_JV_JsonValue_const_q__uint64_uint64>` 
  *  :ref:`from_JV (v: JsonValue const?; ent: int8; defV: int8 = int8(0)) : auto <function-json_boost_from_JV_JsonValue_const_q__int8_int8>` 
  *  :ref:`from_JV (v: JsonValue const?; ent: uint8; defV: uint8 = uint8(0)) : auto <function-json_boost_from_JV_JsonValue_const_q__uint8_uint8>` 
  *  :ref:`from_JV (v: JsonValue const?; ent: int16; defV: int16 = int16(0)) : auto <function-json_boost_from_JV_JsonValue_const_q__int16_int16>` 
  *  :ref:`from_JV (v: JsonValue const?; ent: uint16; defV: uint16 = uint16(0)) : auto <function-json_boost_from_JV_JsonValue_const_q__uint16_uint16>` 
  *  :ref:`from_JV (v: JsonValue const?; ent: bitfield; defV: bitfield = bitfield()) : auto <function-json_boost_from_JV_JsonValue_const_q__bitfield_bitfield>` 
  *  :ref:`from_JV (v: JsonValue const?; ent: bitfield8; defV: bitfield8 = bitfield8()) : auto <function-json_boost_from_JV_JsonValue_const_q__bitfield8_bitfield8>` 
  *  :ref:`from_JV (v: JsonValue const?; ent: bitfield16; defV: bitfield16 = bitfield16()) : auto <function-json_boost_from_JV_JsonValue_const_q__bitfield16_bitfield16>` 
  *  :ref:`from_JV (v: JsonValue const?; ent: bitfield64; defV: bitfield64 = bitfield64()) : auto <function-json_boost_from_JV_JsonValue_const_q__bitfield64_bitfield64>` 
  *  :ref:`JV (v: auto(VecT)) : auto <function-json_boost_JV_autoVecT>` 
  *  :ref:`from_JV (v: JsonValue const?; ent: auto(VecT); defV: VecT = VecT()) : auto <function-json_boost_from_JV_JsonValue_const_q__autoVecT_VecT>` 
  *  :ref:`from_JV (v: JsonValue const?; anything: table\<auto(KT), auto(VT)\>) : auto <function-json_boost_from_JV_JsonValue_const_q__table_ls_autoKT,_autoVT_gr_>` 
  *  :ref:`from_JV (v: JsonValue const?; anything: auto(TT)) : auto <function-json_boost_from_JV_JsonValue_const_q__autoTT>` 
  *  :ref:`JV (value: auto(TT)) : JsonValue? <function-json_boost_JV_autoTT>` 
  *  :ref:`JV (val1: auto; val2: auto) : JsonValue? <function-json_boost_JV_auto_auto>` 
  *  :ref:`JV (val1: auto; val2: auto; val3: auto) : JsonValue? <function-json_boost_JV_auto_auto_auto>` 
  *  :ref:`JV (val1: auto; val2: auto; val3: auto; val4: auto) : JsonValue? <function-json_boost_JV_auto_auto_auto_auto>` 
  *  :ref:`JV (val1: auto; val2: auto; val3: auto; val4: auto; val5: auto) : JsonValue? <function-json_boost_JV_auto_auto_auto_auto_auto>` 
  *  :ref:`JV (val1: auto; val2: auto; val3: auto; val4: auto; val5: auto; val6: auto) : JsonValue? <function-json_boost_JV_auto_auto_auto_auto_auto_auto>` 
  *  :ref:`JV (val1: auto; val2: auto; val3: auto; val4: auto; val5: auto; val6: auto; val7: auto) : JsonValue? <function-json_boost_JV_auto_auto_auto_auto_auto_auto_auto>` 
  *  :ref:`JV (val1: auto; val2: auto; val3: auto; val4: auto; val5: auto; val6: auto; val7: auto; val8: auto) : JsonValue? <function-json_boost_JV_auto_auto_auto_auto_auto_auto_auto_auto>` 
  *  :ref:`JV (val1: auto; val2: auto; val3: auto; val4: auto; val5: auto; val6: auto; val7: auto; val8: auto; val9: auto) : JsonValue? <function-json_boost_JV_auto_auto_auto_auto_auto_auto_auto_auto_auto>` 
  *  :ref:`JV (val1: auto; val2: auto; val3: auto; val4: auto; val5: auto; val6: auto; val7: auto; val8: auto; val9: auto; val10: auto) : JsonValue? <function-json_boost_JV_auto_auto_auto_auto_auto_auto_auto_auto_auto_auto>` 

.. _function-json_boost_from_JV_JsonValue_const_q__autoEnumT_EnumT:

.. das:function:: from_JV(v: JsonValue const?; ent: auto(EnumT); defV: EnumT = EnumT()) : EnumT

Parse a JSON value and return the corresponding native value.

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : auto(EnumT)

            * **defV** : EnumT

.. _function-json_boost_from_JV_JsonValue_const_q__string_string:

.. das:function:: from_JV(v: JsonValue const?; ent: string; defV: string = "") : auto

|detail/function-json_boost-from_JV-0x1080523b9338903f|

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : string

            * **defV** : string

.. _function-json_boost_from_JV_JsonValue_const_q__bool_bool:

.. das:function:: from_JV(v: JsonValue const?; ent: bool; defV: bool = false) : auto

|detail/function-json_boost-from_JV-0x67cf2f8d54cc7543|

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : bool

            * **defV** : bool

.. _function-json_boost_from_JV_JsonValue_const_q__float_float:

.. das:function:: from_JV(v: JsonValue const?; ent: float; defV: float = 0f) : auto

|detail/function-json_boost-from_JV-0xc821a9b21ad126d5|

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : float

            * **defV** : float

.. _function-json_boost_from_JV_JsonValue_const_q__double_double:

.. das:function:: from_JV(v: JsonValue const?; ent: double; defV: double = 0lf) : auto

|detail/function-json_boost-from_JV-0xe9527bea44c4873b|

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : double

            * **defV** : double

.. _function-json_boost_from_JV_JsonValue_const_q__int_int:

.. das:function:: from_JV(v: JsonValue const?; ent: int; defV: int = 0) : auto

|detail/function-json_boost-from_JV-0x407181960b848fc2|

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : int

            * **defV** : int

.. _function-json_boost_from_JV_JsonValue_const_q__uint_uint:

.. das:function:: from_JV(v: JsonValue const?; ent: uint; defV: uint = 0x0) : auto

|detail/function-json_boost-from_JV-0xd0144f1be44eabbb|

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : uint

            * **defV** : uint

.. _function-json_boost_from_JV_JsonValue_const_q__int64_int64:

.. das:function:: from_JV(v: JsonValue const?; ent: int64; defV: int64 = 0) : auto

|detail/function-json_boost-from_JV-0xea5fb9f58444c1c6|

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : int64

            * **defV** : int64

.. _function-json_boost_from_JV_JsonValue_const_q__uint64_uint64:

.. das:function:: from_JV(v: JsonValue const?; ent: uint64; defV: uint64 = 0x0) : auto

|detail/function-json_boost-from_JV-0x89431295255aba0d|

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : uint64

            * **defV** : uint64

.. _function-json_boost_from_JV_JsonValue_const_q__int8_int8:

.. das:function:: from_JV(v: JsonValue const?; ent: int8; defV: int8 = int8(0)) : auto

|detail/function-json_boost-from_JV-0xdbba02e7ee9b416b|

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : int8

            * **defV** : int8

.. _function-json_boost_from_JV_JsonValue_const_q__uint8_uint8:

.. das:function:: from_JV(v: JsonValue const?; ent: uint8; defV: uint8 = uint8(0)) : auto

|detail/function-json_boost-from_JV-0xaa5560b71f74ce17|

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : uint8

            * **defV** : uint8

.. _function-json_boost_from_JV_JsonValue_const_q__int16_int16:

.. das:function:: from_JV(v: JsonValue const?; ent: int16; defV: int16 = int16(0)) : auto

|detail/function-json_boost-from_JV-0xced8a824ebb881b3|

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : int16

            * **defV** : int16

.. _function-json_boost_from_JV_JsonValue_const_q__uint16_uint16:

.. das:function:: from_JV(v: JsonValue const?; ent: uint16; defV: uint16 = uint16(0)) : auto

|detail/function-json_boost-from_JV-0xb4bffee811fcc1a1|

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : uint16

            * **defV** : uint16

.. _function-json_boost_from_JV_JsonValue_const_q__bitfield_bitfield:

.. das:function:: from_JV(v: JsonValue const?; ent: bitfield; defV: bitfield = bitfield()) : auto

|detail/function-json_boost-from_JV-0x654fbc7070f320ab|

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : bitfield<>

            * **defV** : bitfield<>

.. _function-json_boost_from_JV_JsonValue_const_q__bitfield8_bitfield8:

.. das:function:: from_JV(v: JsonValue const?; ent: bitfield8; defV: bitfield8 = bitfield8()) : auto

|detail/function-json_boost-from_JV-0x8fe8ce969449eaae|

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : bitfield8

            * **defV** : bitfield8

.. _function-json_boost_from_JV_JsonValue_const_q__bitfield16_bitfield16:

.. das:function:: from_JV(v: JsonValue const?; ent: bitfield16; defV: bitfield16 = bitfield16()) : auto

|detail/function-json_boost-from_JV-0xd34da4be972b5693|

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : bitfield16

            * **defV** : bitfield16

.. _function-json_boost_from_JV_JsonValue_const_q__bitfield64_bitfield64:

.. das:function:: from_JV(v: JsonValue const?; ent: bitfield64; defV: bitfield64 = bitfield64()) : auto

|detail/function-json_boost-from_JV-0x1831a132d18cc8bb|

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : bitfield64

            * **defV** : bitfield64

.. _function-json_boost_JV_autoVecT:

.. das:function:: JV(v: auto(VecT)) : auto

Creates `JsonValue` out of value.

:Arguments: * **v** : auto(VecT)

.. _function-json_boost_from_JV_JsonValue_const_q__autoVecT_VecT:

.. das:function:: from_JV(v: JsonValue const?; ent: auto(VecT); defV: VecT = VecT()) : auto

|detail/function-json_boost-from_JV-0xedbc1a3ae2c98f22|

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : auto(VecT)

            * **defV** : VecT

.. _function-json_boost_from_JV_JsonValue_const_q__table_ls_autoKT,_autoVT_gr_:

.. das:function:: from_JV(v: JsonValue const?; anything: table<auto(KT), auto(VT)>) : auto

|detail/function-json_boost-from_JV-0x938da3ecfbb9e88b|

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **anything** : table<auto(KT);auto(VT)>

.. _function-json_boost_from_JV_JsonValue_const_q__autoTT:

.. das:function:: from_JV(v: JsonValue const?; anything: auto(TT)) : auto

|detail/function-json_boost-from_JV-0x50ad119df7c0f518|

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **anything** : auto(TT)

.. _function-json_boost_JV_autoTT:

.. das:function:: JV(value: auto(TT)) : JsonValue?

|detail/function-json_boost-JV-0x5855138962357584|

:Arguments: * **value** : auto(TT)

.. _function-json_boost_JV_auto_auto:

.. das:function:: JV(val1: auto; val2: auto) : JsonValue?

|detail/function-json_boost-JV-0xc137e989bbb9f085|

:Arguments: * **val1** : auto

            * **val2** : auto

.. _function-json_boost_JV_auto_auto_auto:

.. das:function:: JV(val1: auto; val2: auto; val3: auto) : JsonValue?

|detail/function-json_boost-JV-0xd68c83f79eecdfc8|

:Arguments: * **val1** : auto

            * **val2** : auto

            * **val3** : auto

.. _function-json_boost_JV_auto_auto_auto_auto:

.. das:function:: JV(val1: auto; val2: auto; val3: auto; val4: auto) : JsonValue?

|detail/function-json_boost-JV-0x49047c6da3d7b76d|

:Arguments: * **val1** : auto

            * **val2** : auto

            * **val3** : auto

            * **val4** : auto

.. _function-json_boost_JV_auto_auto_auto_auto_auto:

.. das:function:: JV(val1: auto; val2: auto; val3: auto; val4: auto; val5: auto) : JsonValue?

|detail/function-json_boost-JV-0xe277e18712e40a30|

:Arguments: * **val1** : auto

            * **val2** : auto

            * **val3** : auto

            * **val4** : auto

            * **val5** : auto

.. _function-json_boost_JV_auto_auto_auto_auto_auto_auto:

.. das:function:: JV(val1: auto; val2: auto; val3: auto; val4: auto; val5: auto; val6: auto) : JsonValue?

|detail/function-json_boost-JV-0xddcc842ba1ee4855|

:Arguments: * **val1** : auto

            * **val2** : auto

            * **val3** : auto

            * **val4** : auto

            * **val5** : auto

            * **val6** : auto

.. _function-json_boost_JV_auto_auto_auto_auto_auto_auto_auto:

.. das:function:: JV(val1: auto; val2: auto; val3: auto; val4: auto; val5: auto; val6: auto; val7: auto) : JsonValue?

|detail/function-json_boost-JV-0x8adaa36fa917f3f8|

:Arguments: * **val1** : auto

            * **val2** : auto

            * **val3** : auto

            * **val4** : auto

            * **val5** : auto

            * **val6** : auto

            * **val7** : auto

.. _function-json_boost_JV_auto_auto_auto_auto_auto_auto_auto_auto:

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

.. _function-json_boost_JV_auto_auto_auto_auto_auto_auto_auto_auto_auto:

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

.. _function-json_boost_JV_auto_auto_auto_auto_auto_auto_auto_auto_auto_auto:

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

.. _function-json_boost__q__JsonValue_const_q___eq__eq_const_string:

.. das:function:: JsonValue const? ==const?[](a: JsonValue const? ==const; key: string) : JsonValue?

Returns the value of the key in the JSON object, if it exists.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?!

            * **key** : string

.. _function-json_boost__q___JsonValue_q___eq__eq_const_string:

.. das:function:: JsonValue? ==const?[](a: JsonValue? ==const; key: string) : JsonValue?

Returns the value of the key in the JSON object, if it exists.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?!

            * **key** : string

.. _function-json_boost__q__dot__JsonValue_const_q___eq__eq_const_string:

.. das:function:: JsonValue const? ==const?.(a: JsonValue const? ==const; key: string) : JsonValue?

Returns the value of the key in the JSON object, if it exists.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?!

            * **key** : string

.. _function-json_boost__q__dot___JsonValue_q___eq__eq_const_string:

.. das:function:: JsonValue? ==const?.(a: JsonValue? ==const; key: string) : JsonValue?

Returns the value of the key in the JSON object, if it exists.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?!

            * **key** : string

.. _function-json_boost__q__JsonValue_const_q___eq__eq_const_int:

.. das:function:: JsonValue const? ==const?[](a: JsonValue const? ==const; idx: int) : JsonValue?

Returns the value of the index in the JSON array, if it exists.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?!

            * **idx** : int

.. _function-json_boost__q___JsonValue_q___eq__eq_const_int:

.. das:function:: JsonValue? ==const?[](a: JsonValue? ==const; idx: int) : JsonValue?

Returns the value of the index in the JSON array, if it exists.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?!

            * **idx** : int

.. _function-json_boost__q__q__JsonValue_const_q__double:

.. das:function:: JsonValue const???(a: JsonValue const?; val: double) : double

Returns the value of the JSON object, if it exists, otherwise returns the default value.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **val** : double

.. _function-json_boost__q__q__JsonValue_const_q__float:

.. das:function:: JsonValue const???(a: JsonValue const?; val: float) : float

Returns the value of the JSON object, if it exists, otherwise returns the default value.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **val** : float

.. _function-json_boost__q__q__JsonValue_const_q__int8:

.. das:function:: JsonValue const???(a: JsonValue const?; val: int8) : int8

Returns the value of the JSON object, if it exists, otherwise returns the default value.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **val** : int8

.. _function-json_boost__q__q__JsonValue_const_q__int16:

.. das:function:: JsonValue const???(a: JsonValue const?; val: int16) : int16

Returns the value of the JSON object, if it exists, otherwise returns the default value.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **val** : int16

.. _function-json_boost__q__q__JsonValue_const_q__int:

.. das:function:: JsonValue const???(a: JsonValue const?; val: int) : int

Returns the value of the JSON object, if it exists, otherwise returns the default value.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **val** : int

.. _function-json_boost__q__q__JsonValue_const_q__int64:

.. das:function:: JsonValue const???(a: JsonValue const?; val: int64) : int64

Returns the value of the JSON object, if it exists, otherwise returns the default value.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **val** : int64

.. _function-json_boost__q__q__JsonValue_const_q__uint8:

.. das:function:: JsonValue const???(a: JsonValue const?; val: uint8) : uint8

Returns the value of the JSON object, if it exists, otherwise returns the default value.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **val** : uint8

.. _function-json_boost__q__q__JsonValue_const_q__uint16:

.. das:function:: JsonValue const???(a: JsonValue const?; val: uint16) : uint16

Returns the value of the JSON object, if it exists, otherwise returns the default value.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **val** : uint16

.. _function-json_boost__q__q__JsonValue_const_q__uint:

.. das:function:: JsonValue const???(a: JsonValue const?; val: uint) : uint

Returns the value of the JSON object, if it exists, otherwise returns the default value.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **val** : uint

.. _function-json_boost__q__q__JsonValue_const_q__uint64:

.. das:function:: JsonValue const???(a: JsonValue const?; val: uint64) : uint64

Returns the value of the JSON object, if it exists, otherwise returns the default value.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **val** : uint64

.. _function-json_boost__q__q__JsonValue_const_q__bool:

.. das:function:: JsonValue const???(a: JsonValue const?; val: bool) : bool

Returns the value of the JSON object, if it exists, otherwise returns the default value.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **val** : bool

.. _function-json_boost__q__q__JsonValue_const_q__string:

.. das:function:: JsonValue const???(a: JsonValue const?; val: string) : string

Returns the value of the JSON object, if it exists, otherwise returns the default value.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **val** : string

.. _function-json_boost__q__dot__rq_value_JsonValue_const_q___eq__eq_const:

.. das:function:: JsonValue const? ==const?.`value(a: JsonValue const? ==const) : variant<_object:table<string;JsonValue?>;_array:array<JsonValue?>;_string:string;_number:double;_longint:int64;_bool:bool;_null:void?> const?

Returns the value of the JSON object, if it exists.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?!

.. _function-json_boost__q__dot__rq_value__JsonValue_q___eq__eq_const:

.. das:function:: JsonValue? ==const?.`value(a: JsonValue? ==const) : variant<_object:table<string;JsonValue?>;_array:array<JsonValue?>;_string:string;_number:double;_longint:int64;_bool:bool;_null:void?>?

Returns the value of the JSON object, if it exists.

:Arguments: * **a** :  :ref:`JsonValue <struct-json-JsonValue>` ?!

.. _function-json_boost_parse_json_annotation_string_array_ls_tuple_ls_name_c_string;data_c_variant_ls_tBool_c_bool;tInt_c_int;tUInt_c_uint;tInt64_c_int64;tUInt64_c_uint64;tFloat_c_float;tDouble_c_double;tString_c_string;nothing_c_any_gr__gr__gr_:

.. das:function:: parse_json_annotation(name: string; annotation: array<tuple<name:string;data:variant<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any>>>) : JsonFieldState

|detail/function-json_boost-parse_json_annotation-0x104e176c5c060230|

:Arguments: * **name** : string

            * **annotation** : array<tuple<name:string;data: :ref:`RttiValue <alias-RttiValue>` >>


