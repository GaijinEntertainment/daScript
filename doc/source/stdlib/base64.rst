
.. _stdlib_base64:

============================
Base64 encoding and decoding
============================

The BASE64 module implements Base64 encoding and decoding.
It provides ``base64_encode`` and ``base64_decode`` for converting between binary data
(strings or ``array<uint8>``) and Base64 text representation.

All functions and symbols are in "base64" module, use require to get access to it. ::

    require daslib/base64

Example: ::

    require daslib/base64

        [export]
        def main() {
            let encoded = base64_encode("Hello, daslang!")
            print("encoded: {encoded}\n")
            let decoded = base64_decode(encoded)
            print("decoded: {decoded.text}\n")
        }
        // output:
        // encoded: SGVsbG8sIGRhU2NyaXB0IQ==
        // decoded: Hello, daslang!

++++++++
Encoding
++++++++

  *  :ref:`BASE64_ENCODE_OUT_SIZE (s: int) : int <function-base64_BASE64_ENCODE_OUT_SIZE_int>` 
  *  :ref:`base64_encode (_inp: string) : string <function-base64_base64_encode_string>` 
  *  :ref:`base64_encode (inp: array\<uint8\>|array\<uint8\>#) : auto <function-base64_base64_encode_array_ls_uint8_gr_array_ls_uint8_gr__hh_>` 

.. _function-base64_BASE64_ENCODE_OUT_SIZE_int:

.. das:function:: BASE64_ENCODE_OUT_SIZE(s: int) : int

Returns the encoded output size for binary data of length `s`.

:Arguments: * **s** : int


base64_encode
^^^^^^^^^^^^^

.. _function-base64_base64_encode_string:

.. das:function:: base64_encode(_inp: string) : string

Encodes a string to its Base64 text representation.

:Arguments: * **_inp** : string

.. _function-base64_base64_encode_array_ls_uint8_gr_array_ls_uint8_gr__hh_:

.. das:function:: base64_encode(inp: array<uint8>|array<uint8>#) : auto

----

++++++++
Decoding
++++++++

  *  :ref:`BASE64_DECODE_OUT_SIZE (s: int) : int <function-base64_BASE64_DECODE_OUT_SIZE_int>` 
  *  :ref:`base64_decode (_in: string) : tuple\<text:string;size:int\> <function-base64_base64_decode_string>` 
  *  :ref:`base64_decode (_in: string; var out: array\<uint8\>) : int <function-base64_base64_decode_string_array_ls_uint8_gr_>` 

.. _function-base64_BASE64_DECODE_OUT_SIZE_int:

.. das:function:: BASE64_DECODE_OUT_SIZE(s: int) : int

Returns the maximum decoded output size for a Base64 string of length `s`.

:Arguments: * **s** : int


base64_decode
^^^^^^^^^^^^^

.. _function-base64_base64_decode_string:

.. das:function:: base64_decode(_in: string) : tuple<text:string;size:int>

Decodes a Base64-encoded string. Returns a tuple of the decoded text and its byte length.

:Arguments: * **_in** : string

.. _function-base64_base64_decode_string_array_ls_uint8_gr_:

.. das:function:: base64_decode(_in: string; out: array<uint8>) : int

----


