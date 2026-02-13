
.. _stdlib_json_boost:

======================
Boost package for JSON
======================

The JSON_BOOST module extends JSON support with operator overloads for convenient
field access (``?[]``), null-coalescing (``??``), and automatic struct-to-JSON
conversion macros (``from_JsValue``, ``to_JsValue``).

All functions and symbols are in "json_boost" module, use require to get access to it. ::

    require daslib/json_boost

Example: ::

    require daslib/json_boost

        [export]
        def main() {
            let data = "\{ \"name\": \"Alice\", \"age\": 30 \}"
            var error = ""
            var js <- read_json(data, error)
            if (error == "") {
                let name = js?.name ?? "?"
                print("name = {name}\n")
                let age = js?.age ?? -1
                print("age = {age}\n")
            }
            unsafe {
                delete js
            }
        }
        // output:
        // name = Alice
        // age = 30

++++++++++
Structures
++++++++++

.. _struct-json_boost-JsonFieldState:

.. das:attribute:: JsonFieldState

:Fields: * **argName** : string - name of the field in JSON

         * **enumAsInt** : bool - whether to parse enum as integer

         * **unescape** : bool - whether to unescape strings

         * **embed** : bool - whether to embed the field

         * **optional** : bool - whether the field is optional


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
  *  :ref:`from_JV (v: JsonValue const?; ent: bitfield8:uint8\<\>; defV: bitfield8 = bitfield8()) : auto <function-json_boost_from_JV_JsonValue_const_q__bitfield8_c_uint8_ls__gr__bitfield8>` 
  *  :ref:`from_JV (v: JsonValue const?; ent: bitfield16:uint16\<\>; defV: bitfield16 = bitfield16()) : auto <function-json_boost_from_JV_JsonValue_const_q__bitfield16_c_uint16_ls__gr__bitfield16>` 
  *  :ref:`from_JV (v: JsonValue const?; ent: bitfield64:uint64\<\>; defV: bitfield64 = bitfield64()) : auto <function-json_boost_from_JV_JsonValue_const_q__bitfield64_c_uint64_ls__gr__bitfield64>` 
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

Parse a JSON value and return the corresponding native value.

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : string

            * **defV** : string

.. _function-json_boost_from_JV_JsonValue_const_q__bool_bool:

.. das:function:: from_JV(v: JsonValue const?; ent: bool; defV: bool = false) : auto

Parse a JSON value and return the corresponding native value.

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : bool

            * **defV** : bool

.. _function-json_boost_from_JV_JsonValue_const_q__float_float:

.. das:function:: from_JV(v: JsonValue const?; ent: float; defV: float = 0f) : auto

Parse a JSON value and return the corresponding native value.

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : float

            * **defV** : float

.. _function-json_boost_from_JV_JsonValue_const_q__double_double:

.. das:function:: from_JV(v: JsonValue const?; ent: double; defV: double = 0lf) : auto

Parse a JSON value and return the corresponding native value.

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : double

            * **defV** : double

.. _function-json_boost_from_JV_JsonValue_const_q__int_int:

.. das:function:: from_JV(v: JsonValue const?; ent: int; defV: int = 0) : auto

Parse a JSON value and return the corresponding native value.

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : int

            * **defV** : int

.. _function-json_boost_from_JV_JsonValue_const_q__uint_uint:

.. das:function:: from_JV(v: JsonValue const?; ent: uint; defV: uint = 0x0) : auto

Parse a JSON value and return the corresponding native value.

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : uint

            * **defV** : uint

.. _function-json_boost_from_JV_JsonValue_const_q__int64_int64:

.. das:function:: from_JV(v: JsonValue const?; ent: int64; defV: int64 = 0) : auto

Parse a JSON value and return the corresponding native value.

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : int64

            * **defV** : int64

.. _function-json_boost_from_JV_JsonValue_const_q__uint64_uint64:

.. das:function:: from_JV(v: JsonValue const?; ent: uint64; defV: uint64 = 0x0) : auto

Parse a JSON value and return the corresponding native value.

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : uint64

            * **defV** : uint64

.. _function-json_boost_from_JV_JsonValue_const_q__int8_int8:

.. das:function:: from_JV(v: JsonValue const?; ent: int8; defV: int8 = int8(0)) : auto

Parse a JSON value and return the corresponding native value.

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : int8

            * **defV** : int8

.. _function-json_boost_from_JV_JsonValue_const_q__uint8_uint8:

.. das:function:: from_JV(v: JsonValue const?; ent: uint8; defV: uint8 = uint8(0)) : auto

Parse a JSON value and return the corresponding native value.

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : uint8

            * **defV** : uint8

.. _function-json_boost_from_JV_JsonValue_const_q__int16_int16:

.. das:function:: from_JV(v: JsonValue const?; ent: int16; defV: int16 = int16(0)) : auto

Parse a JSON value and return the corresponding native value.

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : int16

            * **defV** : int16

.. _function-json_boost_from_JV_JsonValue_const_q__uint16_uint16:

.. das:function:: from_JV(v: JsonValue const?; ent: uint16; defV: uint16 = uint16(0)) : auto

Parse a JSON value and return the corresponding native value.

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : uint16

            * **defV** : uint16

.. _function-json_boost_from_JV_JsonValue_const_q__bitfield_bitfield:

.. das:function:: from_JV(v: JsonValue const?; ent: bitfield; defV: bitfield = bitfield()) : auto

Parse a JSON value and return the corresponding native value.

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : bitfield<>

            * **defV** : bitfield<>

.. _function-json_boost_from_JV_JsonValue_const_q__bitfield8_c_uint8_ls__gr__bitfield8:

.. das:function:: from_JV(v: JsonValue const?; ent: bitfield8:uint8<>; defV: bitfield8 = bitfield8()) : auto

Parse a JSON value and return the corresponding native value.

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : bitfield : uint8<>

            * **defV** : bitfield8

.. _function-json_boost_from_JV_JsonValue_const_q__bitfield16_c_uint16_ls__gr__bitfield16:

.. das:function:: from_JV(v: JsonValue const?; ent: bitfield16:uint16<>; defV: bitfield16 = bitfield16()) : auto

Parse a JSON value and return the corresponding native value.

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : bitfield : uint16<>

            * **defV** : bitfield16

.. _function-json_boost_from_JV_JsonValue_const_q__bitfield64_c_uint64_ls__gr__bitfield64:

.. das:function:: from_JV(v: JsonValue const?; ent: bitfield64:uint64<>; defV: bitfield64 = bitfield64()) : auto

Parse a JSON value and return the corresponding native value.

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : bitfield : uint64<>

            * **defV** : bitfield64

.. _function-json_boost_JV_autoVecT:

.. das:function:: JV(v: auto(VecT)) : auto

Creates `JsonValue` out of value.

:Arguments: * **v** : auto(VecT)

.. _function-json_boost_from_JV_JsonValue_const_q__autoVecT_VecT:

.. das:function:: from_JV(v: JsonValue const?; ent: auto(VecT); defV: VecT = VecT()) : auto

Parse a JSON value and return the corresponding vector value.

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **ent** : auto(VecT)

            * **defV** : VecT

.. _function-json_boost_from_JV_JsonValue_const_q__table_ls_autoKT,_autoVT_gr_:

.. das:function:: from_JV(v: JsonValue const?; anything: table<auto(KT), auto(VT)>) : auto

Parse a JSON value and return the corresponding table value.

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **anything** : table<auto(KT);auto(VT)>

.. _function-json_boost_from_JV_JsonValue_const_q__autoTT:

.. das:function:: from_JV(v: JsonValue const?; anything: auto(TT)) : auto

Parse a JSON value and return the corresponding value of any type.
This is the main dispatch function that handles various types.

:Arguments: * **v** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **anything** : auto(TT)

.. _function-json_boost_JV_autoTT:

.. das:function:: JV(value: auto(TT)) : JsonValue?

Creates `JsonValue` out of value.
This is the main dispatch function that handles various types.

:Arguments: * **value** : auto(TT)

.. _function-json_boost_JV_auto_auto:

.. das:function:: JV(val1: auto; val2: auto) : JsonValue?

Creates array of two JsonValues.

:Arguments: * **val1** : auto

            * **val2** : auto

.. _function-json_boost_JV_auto_auto_auto:

.. das:function:: JV(val1: auto; val2: auto; val3: auto) : JsonValue?

Creates array of three JsonValues.

:Arguments: * **val1** : auto

            * **val2** : auto

            * **val3** : auto

.. _function-json_boost_JV_auto_auto_auto_auto:

.. das:function:: JV(val1: auto; val2: auto; val3: auto; val4: auto) : JsonValue?

Creates array of four JsonValues.

:Arguments: * **val1** : auto

            * **val2** : auto

            * **val3** : auto

            * **val4** : auto

.. _function-json_boost_JV_auto_auto_auto_auto_auto:

.. das:function:: JV(val1: auto; val2: auto; val3: auto; val4: auto; val5: auto) : JsonValue?

Creates array of five JsonValues.

:Arguments: * **val1** : auto

            * **val2** : auto

            * **val3** : auto

            * **val4** : auto

            * **val5** : auto

.. _function-json_boost_JV_auto_auto_auto_auto_auto_auto:

.. das:function:: JV(val1: auto; val2: auto; val3: auto; val4: auto; val5: auto; val6: auto) : JsonValue?

Creates array of six JsonValues.

:Arguments: * **val1** : auto

            * **val2** : auto

            * **val3** : auto

            * **val4** : auto

            * **val5** : auto

            * **val6** : auto

.. _function-json_boost_JV_auto_auto_auto_auto_auto_auto_auto:

.. das:function:: JV(val1: auto; val2: auto; val3: auto; val4: auto; val5: auto; val6: auto; val7: auto) : JsonValue?

Creates array of seven JsonValues.

:Arguments: * **val1** : auto

            * **val2** : auto

            * **val3** : auto

            * **val4** : auto

            * **val5** : auto

            * **val6** : auto

            * **val7** : auto

.. _function-json_boost_JV_auto_auto_auto_auto_auto_auto_auto_auto:

.. das:function:: JV(val1: auto; val2: auto; val3: auto; val4: auto; val5: auto; val6: auto; val7: auto; val8: auto) : JsonValue?

Creates array of eight JsonValues.

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

Creates array of nine JsonValues.

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

Creates array of ten JsonValues.

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

Parse JSON field annotations and return the corresponding JsonFieldState.

:Arguments: * **name** : string

            * **annotation** : array<tuple<name:string;data: :ref:`RttiValue <alias-RttiValue>` >>


