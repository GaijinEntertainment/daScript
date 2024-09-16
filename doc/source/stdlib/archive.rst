
.. _stdlib_archive:

=============================
General prupose serialization
=============================

The archive module implements general purpose serialization infrastructure.

All functions and symbols are in "archive" module, use require to get access to it. ::

    require daslib/archive

To correctly support serialization of the specific type, you need to define and implement `serialize` method for it.
For example this is how DECS implements component serialization: ::

    def public serialize ( var arch:Archive; var src:Component )
        arch |> serialize(src.name)
        arch |> serialize(src.hash)
        arch |> serialize(src.stride)
        arch |> serialize(src.info)
        invoke(src.info.serializer, arch, src.data)


++++++++++
Structures
++++++++++

.. _struct-archive-Archive:

.. das:attribute:: Archive

Archive is a combination of serialization stream, and state (version, and reading status).

:Fields: * **version** : uint

         * **reading** : bool

         * **stream** :  :ref:`Serializer <struct-archive-Serializer>` ?

Archive is a combination of serialization stream, and state (version, and reading status).

+++++++
Classes
+++++++

.. _struct-archive-Serializer:

.. das:attribute:: Serializer

Base class for serializers.


.. _struct-archive-MemSerializer:

.. das:attribute:: MemSerializer : Serializer

This serializer stores data in memory (in the array<uint8>)


.. das:function:: MemSerializer.write(bytes: void? implicit; size: int) : bool

Appends bytes at the end of the data.


:Arguments: * **bytes** : void? implicit

            * **size** : int

.. das:function:: MemSerializer.read(bytes: void? implicit; size: int) : bool

Reads bytes from data, advances the reading position.


:Arguments: * **bytes** : void? implicit

            * **size** : int

.. das:function:: MemSerializer.error(code: string)

Sets the last error code.


:Arguments: * **code** : string

.. das:function:: MemSerializer.OK() : bool

Implements 'OK' method, which returns true if the serializer is in a valid state.

.. das:function:: MemSerializer.extractData() : array<uint8>

Extract the data from the serializer.

.. das:function:: MemSerializer.getCopyOfData() : array<uint8>

Returns copy of the data from the seiralizer.

.. das:function:: MemSerializer.getLastError() : string

Returns last serialization error.

.. _function-_at_archive_c__c_MemSerializer:

.. das:function:: MemSerializer() : MemSerializer

Initialize the serializer for reading or writing.

.. _function-_at_archive_c__c_MemSerializer_C1_ls_u8_gr_A:

.. das:function:: MemSerializer(from: array<uint8>) : MemSerializer

|detail/function-archive-MemSerializer-MemSerializer-0xe5381da5d7f52677|

:Arguments: * **from** : array<uint8>

+++++++++++++
Serialization
+++++++++++++

  *  :ref:`serialize (var arch: Archive; var value: float3x3) <function-_at_archive_c__c_serialize_S_ls_archive_c__c_Archive_gr__H_ls_math_c__c_float3x3_gr_>` 
  *  :ref:`serialize (var arch: Archive; var value: float3x4) <function-_at_archive_c__c_serialize_S_ls_archive_c__c_Archive_gr__H_ls_math_c__c_float3x4_gr_>` 
  *  :ref:`serialize (var arch: Archive; var value: float4x4) <function-_at_archive_c__c_serialize_S_ls_archive_c__c_Archive_gr__H_ls_math_c__c_float4x4_gr_>` 
  *  :ref:`serialize (var arch: Archive; var value: string&) <function-_at_archive_c__c_serialize_S_ls_archive_c__c_Archive_gr__&s>` 
  *  :ref:`serialize_raw (var arch: Archive; var value: auto(TT)&) : auto <function-_at_archive_c__c_serialize_raw_S_ls_archive_c__c_Archive_gr__&Y_ls_TT_gr__dot_>` 
  *  :ref:`read_raw (var arch: Archive; var value: auto(TT)&) : auto <function-_at_archive_c__c_read_raw_S_ls_archive_c__c_Archive_gr__&Y_ls_TT_gr__dot_>` 
  *  :ref:`write_raw (var arch: Archive; var value: auto(TT)&) : auto <function-_at_archive_c__c_write_raw_S_ls_archive_c__c_Archive_gr__&Y_ls_TT_gr__dot_>` 
  *  :ref:`serialize (var arch: Archive; var value: auto(TT)&) : auto <function-_at_archive_c__c_serialize_S_ls_archive_c__c_Archive_gr__&Y_ls_TT_gr__dot__%_ls_((IsAnyEnumMacro_c_expect_any_enum(value_eq_true)||IsAnyWorkhorseNonPtrMacro_c_expect_any_workhorse_raw(value_eq_true))||IsValueHandle_c_expect_value_handle(value_eq_true))_gr_>` 
  *  :ref:`serialize (var arch: Archive; var value: auto(TT)&) : auto <function-_at_archive_c__c_serialize_S_ls_archive_c__c_Archive_gr__&Y_ls_TT_gr__dot__%_ls_IsAnyFunctionNonPtrMacro_c_expect_any_function(value_eq_true)_gr_>` 
  *  :ref:`serialize (var arch: Archive; var value: auto(TT)&) : auto <function-_at_archive_c__c_serialize_S_ls_archive_c__c_Archive_gr__&Y_ls_TT_gr__dot__%_ls_IsAnyStructMacro_c_expect_any_struct(value_eq_true)_gr_>` 
  *  :ref:`serialize (var arch: Archive; var value: auto(TT)&) : auto <function-_at_archive_c__c_serialize_S_ls_archive_c__c_Archive_gr__&Y_ls_TT_gr__dot__%_ls_IsAnyTupleNonPtrMacro_c_expect_any_tuple(value_eq_true)_gr_>` 
  *  :ref:`serialize (var arch: Archive; var value: auto(TT)&) : auto <function-_at_archive_c__c_serialize_S_ls_archive_c__c_Archive_gr__&Y_ls_TT_gr__dot__%_ls_IsAnyVariantNonPtrMacro_c_expect_any_variant(value_eq_true)_gr_>` 
  *  :ref:`serialize (var arch: Archive; var value: auto(TT)[]) : auto <function-_at_archive_c__c_serialize_S_ls_archive_c__c_Archive_gr___lb_-1_rb_Y_ls_TT_gr__dot_>` 
  *  :ref:`serialize (var arch: Archive; var value: array\<auto(TT)\>) : auto <function-_at_archive_c__c_serialize_S_ls_archive_c__c_Archive_gr__1_ls_Y_ls_TT_gr__dot__gr_A>` 
  *  :ref:`serialize (var arch: Archive; var value: table\<auto(KT);auto(VT)\>) : auto <function-_at_archive_c__c_serialize_S_ls_archive_c__c_Archive_gr__1_ls_Y_ls_KT_gr__dot__gr_2_ls_Y_ls_VT_gr__dot__gr_T>` 
  *  :ref:`serialize (var arch: Archive; var value: auto(TT)?) : auto <function-_at_archive_c__c_serialize_S_ls_archive_c__c_Archive_gr__1_ls_Y_ls_TT_gr__dot__gr__qm_>` 

.. _function-_at_archive_c__c_serialize_S_ls_archive_c__c_Archive_gr__H_ls_math_c__c_float3x3_gr_:

.. das:function:: serialize(arch: Archive; value: float3x3)

|detail/function-archive-serialize-0xf9aec3daab7c5940|

:Arguments: * **arch** :  :ref:`Archive <struct-archive-Archive>` 

            * **value** :  :ref:`float3x3 <handle-math-float3x3>` 

.. _function-_at_archive_c__c_serialize_S_ls_archive_c__c_Archive_gr__H_ls_math_c__c_float3x4_gr_:

.. das:function:: serialize(arch: Archive; value: float3x4)

|detail/function-archive-serialize-0xf9b3c3daab84d840|

:Arguments: * **arch** :  :ref:`Archive <struct-archive-Archive>` 

            * **value** :  :ref:`float3x4 <handle-math-float3x4>` 

.. _function-_at_archive_c__c_serialize_S_ls_archive_c__c_Archive_gr__H_ls_math_c__c_float4x4_gr_:

.. das:function:: serialize(arch: Archive; value: float4x4)

|detail/function-archive-serialize-0xe8b5c3da9d150b40|

:Arguments: * **arch** :  :ref:`Archive <struct-archive-Archive>` 

            * **value** :  :ref:`float4x4 <handle-math-float4x4>` 

.. _function-_at_archive_c__c_serialize_S_ls_archive_c__c_Archive_gr__&s:

.. das:function:: serialize(arch: Archive; value: string&)

|detail/function-archive-serialize-0xd50b4b46bcbe65c6|

:Arguments: * **arch** :  :ref:`Archive <struct-archive-Archive>` 

            * **value** : string&

.. _function-_at_archive_c__c_serialize_raw_S_ls_archive_c__c_Archive_gr__&Y_ls_TT_gr__dot_:

.. das:function:: serialize_raw(arch: Archive; value: auto(TT)&) : auto

Serialize raw data (straight up bytes for raw pod)


:Arguments: * **arch** :  :ref:`Archive <struct-archive-Archive>` 

            * **value** : auto(TT)&

.. _function-_at_archive_c__c_read_raw_S_ls_archive_c__c_Archive_gr__&Y_ls_TT_gr__dot_:

.. das:function:: read_raw(arch: Archive; value: auto(TT)&) : auto

Read raw data (straight up bytes for raw pod)


:Arguments: * **arch** :  :ref:`Archive <struct-archive-Archive>` 

            * **value** : auto(TT)&

.. _function-_at_archive_c__c_write_raw_S_ls_archive_c__c_Archive_gr__&Y_ls_TT_gr__dot_:

.. das:function:: write_raw(arch: Archive; value: auto(TT)&) : auto

Write raw data (straight up bytes for raw pod)


:Arguments: * **arch** :  :ref:`Archive <struct-archive-Archive>` 

            * **value** : auto(TT)&

.. _function-_at_archive_c__c_serialize_S_ls_archive_c__c_Archive_gr__&Y_ls_TT_gr__dot__%_ls_((IsAnyEnumMacro_c_expect_any_enum(value_eq_true)||IsAnyWorkhorseNonPtrMacro_c_expect_any_workhorse_raw(value_eq_true))||IsValueHandle_c_expect_value_handle(value_eq_true))_gr_:

.. das:function:: serialize(arch: Archive; value: auto(TT)&) : auto

Serializes structured data, based on the `value` type.


:Arguments: * **arch** :  :ref:`Archive <struct-archive-Archive>` 

            * **value** : auto(TT)&

.. _function-_at_archive_c__c_serialize_S_ls_archive_c__c_Archive_gr__&Y_ls_TT_gr__dot__%_ls_IsAnyFunctionNonPtrMacro_c_expect_any_function(value_eq_true)_gr_:

.. das:function:: serialize(arch: Archive; value: auto(TT)&) : auto

Serializes structured data, based on the `value` type.


:Arguments: * **arch** :  :ref:`Archive <struct-archive-Archive>` 

            * **value** : auto(TT)&

.. _function-_at_archive_c__c_serialize_S_ls_archive_c__c_Archive_gr__&Y_ls_TT_gr__dot__%_ls_IsAnyStructMacro_c_expect_any_struct(value_eq_true)_gr_:

.. das:function:: serialize(arch: Archive; value: auto(TT)&) : auto

Serializes structured data, based on the `value` type.


:Arguments: * **arch** :  :ref:`Archive <struct-archive-Archive>` 

            * **value** : auto(TT)&

.. _function-_at_archive_c__c_serialize_S_ls_archive_c__c_Archive_gr__&Y_ls_TT_gr__dot__%_ls_IsAnyTupleNonPtrMacro_c_expect_any_tuple(value_eq_true)_gr_:

.. das:function:: serialize(arch: Archive; value: auto(TT)&) : auto

Serializes structured data, based on the `value` type.


:Arguments: * **arch** :  :ref:`Archive <struct-archive-Archive>` 

            * **value** : auto(TT)&

.. _function-_at_archive_c__c_serialize_S_ls_archive_c__c_Archive_gr__&Y_ls_TT_gr__dot__%_ls_IsAnyVariantNonPtrMacro_c_expect_any_variant(value_eq_true)_gr_:

.. das:function:: serialize(arch: Archive; value: auto(TT)&) : auto

Serializes structured data, based on the `value` type.


:Arguments: * **arch** :  :ref:`Archive <struct-archive-Archive>` 

            * **value** : auto(TT)&

.. _function-_at_archive_c__c_serialize_S_ls_archive_c__c_Archive_gr___lb_-1_rb_Y_ls_TT_gr__dot_:

.. das:function:: serialize(arch: Archive; value: auto(TT)[]) : auto

|detail/function-archive-serialize-0x1e763b58a64a78b7|

:Arguments: * **arch** :  :ref:`Archive <struct-archive-Archive>` 

            * **value** : auto(TT)[-1]

.. _function-_at_archive_c__c_serialize_S_ls_archive_c__c_Archive_gr__1_ls_Y_ls_TT_gr__dot__gr_A:

.. das:function:: serialize(arch: Archive; value: array<auto(TT)>) : auto

|detail/function-archive-serialize-0x5f917fe97e94122c|

:Arguments: * **arch** :  :ref:`Archive <struct-archive-Archive>` 

            * **value** : array<auto(TT)>

.. _function-_at_archive_c__c_serialize_S_ls_archive_c__c_Archive_gr__1_ls_Y_ls_KT_gr__dot__gr_2_ls_Y_ls_VT_gr__dot__gr_T:

.. das:function:: serialize(arch: Archive; value: table<auto(KT);auto(VT)>) : auto

|detail/function-archive-serialize-0xf05cabbc98a4b483|

:Arguments: * **arch** :  :ref:`Archive <struct-archive-Archive>` 

            * **value** : table<auto(KT);auto(VT)>

.. _function-_at_archive_c__c_serialize_S_ls_archive_c__c_Archive_gr__1_ls_Y_ls_TT_gr__dot__gr__qm_:

.. das:function:: serialize(arch: Archive; value: auto(TT)?) : auto

|detail/function-archive-serialize-0x1ed2d758a6e7d5cb|

:Arguments: * **arch** :  :ref:`Archive <struct-archive-Archive>` 

            * **value** : auto(TT)?

++++++++++++++
Memory archive
++++++++++++++

  *  :ref:`mem_archive_save (var t: auto&) : auto <function-_at_archive_c__c_mem_archive_save_&_dot_>` 
  *  :ref:`mem_archive_load (var data: array\<uint8\>; var t: auto&; canfail: bool = false) : bool <function-_at_archive_c__c_mem_archive_load_1_ls_u8_gr_A_&_dot__Cb>` 

.. _function-_at_archive_c__c_mem_archive_save_&_dot_:

.. das:function:: mem_archive_save(t: auto&) : auto

Saves the object to a memory archive. Result is array<uint8> with the serialized data.


:Arguments: * **t** : auto&

.. _function-_at_archive_c__c_mem_archive_load_1_ls_u8_gr_A_&_dot__Cb:

.. das:function:: mem_archive_load(data: array<uint8>; t: auto&; canfail: bool = false) : bool

Loads the object from a memory archive. `data` is the array<uint8> with the serialized data, returned from `mem_archive_save`.


:Arguments: * **data** : array<uint8>

            * **t** : auto&

            * **canfail** : bool


