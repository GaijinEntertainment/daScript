
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

:Fields: * **version** : uint - Version of the archive format.

         * **reading** : bool - True if the archive is for reading, false for writing.

         * **stream** :  :ref:`Serializer <struct-archive-Serializer>` ? - Serialization stream.


+++++++
Classes
+++++++

.. _struct-archive-Serializer:

.. das:attribute:: Serializer

Base class for serializers.


.. _struct-archive-MemSerializer:

.. das:attribute:: MemSerializer : Serializer

This serializer stores data in memory (in the array<uint8>)


.. _function-archive_MemSerializer_rq_write_MemSerializer_void_q__implicit_int:

.. das:function:: MemSerializer.write(bytes: void? implicit; size: int) : bool

Appends bytes at the end of the data.

:Arguments: * **bytes** : void? implicit

            * **size** : int

.. _function-archive_MemSerializer_rq_read_MemSerializer_void_q__implicit_int:

.. das:function:: MemSerializer.read(bytes: void? implicit; size: int) : bool

Reads bytes from data, advances the reading position.

:Arguments: * **bytes** : void? implicit

            * **size** : int

.. _function-archive_MemSerializer_rq_error_MemSerializer_string:

.. das:function:: MemSerializer.error(code: string)

Sets the last error code.

:Arguments: * **code** : string

.. _function-archive_MemSerializer_rq_OK_MemSerializer:

.. das:function:: MemSerializer.OK() : bool

Implements 'OK' method, which returns true if the serializer is in a valid state.

.. _function-archive_MemSerializer_rq_extractData_MemSerializer:

.. das:function:: MemSerializer.extractData() : array<uint8>

Extract the data from the serializer.

.. _function-archive_MemSerializer_rq_getCopyOfData_MemSerializer:

.. das:function:: MemSerializer.getCopyOfData() : array<uint8>

Returns copy of the data from the seiralizer.

.. _function-archive_MemSerializer_rq_getLastError_MemSerializer:

.. das:function:: MemSerializer.getLastError() : string

Returns last serialization error.

.. _function-archive_MemSerializer:

.. das:function:: MemSerializer() : MemSerializer

Initialize the serializer for reading or writing.

.. _function-archive_MemSerializer_array_ls_uint8_gr_:

.. das:function:: MemSerializer(from: array<uint8>) : MemSerializer

|detail/function-archive-MemSerializer-MemSerializer-0xe5381da5d7f52677|

:Arguments: * **from** : array<uint8>

+++++++++++++
Serialization
+++++++++++++

  *  :ref:`serialize (var arch: Archive; var value: float3x3) <function-archive_serialize_Archive_float3x3>` 
  *  :ref:`serialize (var arch: Archive; var value: float3x4) <function-archive_serialize_Archive_float3x4>` 
  *  :ref:`serialize (var arch: Archive; var value: float4x4) <function-archive_serialize_Archive_float4x4>` 
  *  :ref:`serialize (var arch: Archive; var value: string&) <function-archive_serialize_Archive_string>` 
  *  :ref:`serialize_raw (var arch: Archive; var value: auto(TT)&) : auto <function-archive_serialize_raw_Archive_autoTT>` 
  *  :ref:`read_raw (var arch: Archive; var value: auto(TT)&) : auto <function-archive_read_raw_Archive_autoTT>` 
  *  :ref:`write_raw (var arch: Archive; var value: auto(TT)&) : auto <function-archive_write_raw_Archive_autoTT>` 
  *  :ref:`serialize (var arch: Archive; var value: auto(TT)&) : auto <function-archive_serialize_Archive_autoTT>` 
  *  :ref:`serialize (var arch: Archive; var value: auto(TT)&) : auto <function-archive_serialize_Archive_autoTT>` 
  *  :ref:`serialize (var arch: Archive; var value: auto(TT)&) : auto <function-archive_serialize_Archive_autoTT>` 
  *  :ref:`serialize (var arch: Archive; var value: auto(TT)&) : auto <function-archive_serialize_Archive_autoTT>` 
  *  :ref:`serialize (var arch: Archive; var value: auto(TT)&) : auto <function-archive_serialize_Archive_autoTT>` 
  *  :ref:`serialize (var arch: Archive; var value: auto(TT)[]) : auto <function-archive_serialize_Archive_autoTT>` 
  *  :ref:`serialize (var arch: Archive; var value: array\<auto(TT)\>) : auto <function-archive_serialize_Archive_array_ls_autoTT_gr_>` 
  *  :ref:`serialize (var arch: Archive; var value: table\<auto(KT), auto(VT)\>) : auto <function-archive_serialize_Archive_table_ls_autoKT,_autoVT_gr_>` 
  *  :ref:`serialize (var arch: Archive; var value: auto(TT)?) : auto <function-archive_serialize_Archive_autoTT_q_>` 

.. _function-archive_serialize_Archive_float3x3:

.. das:function:: serialize(arch: Archive; value: float3x3)

|detail/function-archive-serialize-0x36bc5c6c5b9ab5d7|

:Arguments: * **arch** :  :ref:`Archive <struct-archive-Archive>` 

            * **value** :  :ref:`float3x3 <handle-math-float3x3>` 

.. _function-archive_serialize_Archive_float3x4:

.. das:function:: serialize(arch: Archive; value: float3x4)

|detail/function-archive-serialize-0x36c15c6c5ba334d7|

:Arguments: * **arch** :  :ref:`Archive <struct-archive-Archive>` 

            * **value** :  :ref:`float3x4 <handle-math-float3x4>` 

.. _function-archive_serialize_Archive_float4x4:

.. das:function:: serialize(arch: Archive; value: float4x4)

|detail/function-archive-serialize-0x4ec35c6c70387bd7|

:Arguments: * **arch** :  :ref:`Archive <struct-archive-Archive>` 

            * **value** :  :ref:`float4x4 <handle-math-float4x4>` 

.. _function-archive_serialize_Archive_string:

.. das:function:: serialize(arch: Archive; value: string&)

|detail/function-archive-serialize-0x1064f15a69990e2d|

:Arguments: * **arch** :  :ref:`Archive <struct-archive-Archive>` 

            * **value** : string&

.. _function-archive_serialize_raw_Archive_autoTT:

.. das:function:: serialize_raw(arch: Archive; value: auto(TT)&) : auto

Serialize raw data (straight up bytes for raw pod)

:Arguments: * **arch** :  :ref:`Archive <struct-archive-Archive>` 

            * **value** : auto(TT)&

.. _function-archive_read_raw_Archive_autoTT:

.. das:function:: read_raw(arch: Archive; value: auto(TT)&) : auto

Read raw data (straight up bytes for raw pod)

:Arguments: * **arch** :  :ref:`Archive <struct-archive-Archive>` 

            * **value** : auto(TT)&

.. _function-archive_write_raw_Archive_autoTT:

.. das:function:: write_raw(arch: Archive; value: auto(TT)&) : auto

Write raw data (straight up bytes for raw pod)

:Arguments: * **arch** :  :ref:`Archive <struct-archive-Archive>` 

            * **value** : auto(TT)&

.. _function-archive_serialize_Archive_autoTT:

.. das:function:: serialize(arch: Archive; value: auto(TT)&) : auto

Serializes structured data, based on the `value` type.

:Arguments: * **arch** :  :ref:`Archive <struct-archive-Archive>` 

            * **value** : auto(TT)&

.. _function-archive_serialize_Archive_autoTT:

.. das:function:: serialize(arch: Archive; value: auto(TT)&) : auto

Serializes structured data, based on the `value` type.

:Arguments: * **arch** :  :ref:`Archive <struct-archive-Archive>` 

            * **value** : auto(TT)&

.. _function-archive_serialize_Archive_autoTT:

.. das:function:: serialize(arch: Archive; value: auto(TT)&) : auto

Serializes structured data, based on the `value` type.

:Arguments: * **arch** :  :ref:`Archive <struct-archive-Archive>` 

            * **value** : auto(TT)&

.. _function-archive_serialize_Archive_autoTT:

.. das:function:: serialize(arch: Archive; value: auto(TT)&) : auto

Serializes structured data, based on the `value` type.

:Arguments: * **arch** :  :ref:`Archive <struct-archive-Archive>` 

            * **value** : auto(TT)&

.. _function-archive_serialize_Archive_autoTT:

.. das:function:: serialize(arch: Archive; value: auto(TT)&) : auto

Serializes structured data, based on the `value` type.

:Arguments: * **arch** :  :ref:`Archive <struct-archive-Archive>` 

            * **value** : auto(TT)&

.. _function-archive_serialize_Archive_autoTT:

.. das:function:: serialize(arch: Archive; value: auto(TT)[]) : auto

|detail/function-archive-serialize-0xc22a77853d491dca|

:Arguments: * **arch** :  :ref:`Archive <struct-archive-Archive>` 

            * **value** : auto(TT)[-1]

.. _function-archive_serialize_Archive_array_ls_autoTT_gr_:

.. das:function:: serialize(arch: Archive; value: array<auto(TT)>) : auto

|detail/function-archive-serialize-0x6bda914b651b2d2f|

:Arguments: * **arch** :  :ref:`Archive <struct-archive-Archive>` 

            * **value** : array<auto(TT)>

.. _function-archive_serialize_Archive_table_ls_autoKT,_autoVT_gr_:

.. das:function:: serialize(arch: Archive; value: table<auto(KT), auto(VT)>) : auto

|detail/function-archive-serialize-0xa62df472ba362ee1|

:Arguments: * **arch** :  :ref:`Archive <struct-archive-Archive>` 

            * **value** : table<auto(KT);auto(VT)>

.. _function-archive_serialize_Archive_autoTT_q_:

.. das:function:: serialize(arch: Archive; value: auto(TT)?) : auto

|detail/function-archive-serialize-0xc207d3853d0e411e|

:Arguments: * **arch** :  :ref:`Archive <struct-archive-Archive>` 

            * **value** : auto(TT)?

++++++++++++++
Memory archive
++++++++++++++

  *  :ref:`mem_archive_save (var t: auto&) : auto <function-archive_mem_archive_save_auto>` 
  *  :ref:`mem_archive_load (var data: array\<uint8\>; var t: auto&; canfail: bool = false) : bool <function-archive_mem_archive_load_array_ls_uint8_gr__auto_bool>` 

.. _function-archive_mem_archive_save_auto:

.. das:function:: mem_archive_save(t: auto&) : auto

Saves the object to a memory archive. Result is array<uint8> with the serialized data.

:Arguments: * **t** : auto&

.. _function-archive_mem_archive_load_array_ls_uint8_gr__auto_bool:

.. das:function:: mem_archive_load(data: array<uint8>; t: auto&; canfail: bool = false) : bool

Loads the object from a memory archive. `data` is the array<uint8> with the serialized data, returned from `mem_archive_save`.

:Arguments: * **data** : array<uint8>

            * **t** : auto&

            * **canfail** : bool


