
.. _stdlib_utf8_utils:

===============
UTF-8 utilities
===============

The UTF8_UTILS module provides Unicode UTF-8 string utilities including
character iteration, codepoint extraction, byte length calculation, and
validation of UTF-8 encoded text.

All functions and symbols are in "utf8_utils" module, use require to get access to it. ::

    require daslib/utf8_utils

+++++++++
Constants
+++++++++

.. _global-utf8_utils-s_utf8d:

.. das:attribute:: s_utf8d = fixed_array<uint>(0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x8, 0x8, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0xa, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 0x3, 0x3, 0xb, 0x6, 0x6, 0x6, 0x5, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x0, 0xc, 0x18, 0x24, 0x3c, 0x60, 0x54, 0xc, 0xc, 0xc, 0x30, 0x48, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0x0, 0xc, 0xc, 0xc, 0xc, 0xc, 0x0, 0xc, 0x0, 0xc, 0xc, 0xc, 0x18, 0xc, 0xc, 0xc, 0xc, 0xc, 0x18, 0xc, 0x18, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0x18, 0xc, 0xc, 0xc, 0xc, 0xc, 0x18, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0x18, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0x24, 0xc, 0x24, 0xc, 0xc, 0xc, 0x24, 0xc, 0xc, 0xc, 0xc, 0xc, 0x24, 0xc, 0x24, 0xc, 0xc, 0xc, 0x24, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc)

|detail/Variable-utf8_utils-s_utf8d|

.. _global-utf8_utils-UTF8_ACCEPT:

.. das:attribute:: UTF8_ACCEPT = 0x0

|detail/Variable-utf8_utils-UTF8_ACCEPT|

+++++++++++++++++++++
Encoding and decoding
+++++++++++++++++++++

  *  :ref:`utf16_to_utf32 (high: uint; low: uint) : uint <function-utf8_utils_utf16_to_utf32_uint_uint>` 
  *  :ref:`utf8_encode (var dest_array: array\<uint8\>; ch: uint) <function-utf8_utils_utf8_encode_array_ls_uint8_gr__uint>` 
  *  :ref:`utf8_encode (ch: uint) : array\<uint8\> <function-utf8_utils_utf8_encode_uint>` 
  *  :ref:`utf8_encode (var dest_array: array\<uint8\>; source_utf32_string: array\<uint\>) <function-utf8_utils_utf8_encode_array_ls_uint8_gr__array_ls_uint_gr_>` 
  *  :ref:`utf8_encode (source_utf32_string: array\<uint\>) : array\<uint8\> <function-utf8_utils_utf8_encode_array_ls_uint_gr_>` 
  *  :ref:`utf8_decode (var dest_utf32_string: array\<uint\>; source_utf8_string: array\<uint8\>) <function-utf8_utils_utf8_decode_array_ls_uint_gr__array_ls_uint8_gr_>` 
  *  :ref:`utf8_decode (source_utf8_string: array\<uint8\>) : array\<uint\> <function-utf8_utils_utf8_decode_array_ls_uint8_gr_>` 
  *  :ref:`utf8_decode (source_utf8_string: string) : array\<uint\> <function-utf8_utils_utf8_decode_string>` 
  *  :ref:`utf8_decode (var dest_utf32_string: array\<uint\>; source_utf8_string: string) <function-utf8_utils_utf8_decode_array_ls_uint_gr__string>` 

.. _function-utf8_utils_utf16_to_utf32_uint_uint:

.. das:function:: utf16_to_utf32(high: uint; low: uint) : uint

|detail/function-utf8_utils-utf16_to_utf32-0xce3eab4402448264|

:Arguments: * **high** : uint

            * **low** : uint

.. _function-utf8_utils_utf8_encode_array_ls_uint8_gr__uint:

.. das:function:: utf8_encode(dest_array: array<uint8>; ch: uint)

Converts a codepoint (UTF-32 symbol) to UTF-8 and appends it to the UTF-8 byte array

:Arguments: * **dest_array** : array<uint8>

            * **ch** : uint

.. _function-utf8_utils_utf8_encode_uint:

.. das:function:: utf8_encode(ch: uint) : array<uint8>

Converts a codepoint (UTF-32 symbol) to the UTF-8 byte array

:Arguments: * **ch** : uint

.. _function-utf8_utils_utf8_encode_array_ls_uint8_gr__array_ls_uint_gr_:

.. das:function:: utf8_encode(dest_array: array<uint8>; source_utf32_string: array<uint>)

Converts UTF-32 string to UTF-8 and appends it to the UTF-8 byte array

:Arguments: * **dest_array** : array<uint8>

            * **source_utf32_string** : array<uint> implicit

.. _function-utf8_utils_utf8_encode_array_ls_uint_gr_:

.. das:function:: utf8_encode(source_utf32_string: array<uint>) : array<uint8>

Converts UTF-32 string to UTF-8 and returns it as a UTF-8 byte array

:Arguments: * **source_utf32_string** : array<uint> implicit

.. _function-utf8_utils_utf8_decode_array_ls_uint_gr__array_ls_uint8_gr_:

.. das:function:: utf8_decode(dest_utf32_string: array<uint>; source_utf8_string: array<uint8>)

Converts UTF-8 string to UTF-32 and appends it to the array of codepoints (UTF-32 string)

:Arguments: * **dest_utf32_string** : array<uint>

            * **source_utf8_string** : array<uint8> implicit

.. _function-utf8_utils_utf8_decode_array_ls_uint8_gr_:

.. das:function:: utf8_decode(source_utf8_string: array<uint8>) : array<uint>

Converts UTF-8 string to UTF-32 and returns it as an array of codepoints (UTF-32 string)

:Arguments: * **source_utf8_string** : array<uint8> implicit

.. _function-utf8_utils_utf8_decode_string:

.. das:function:: utf8_decode(source_utf8_string: string) : array<uint>

Converts UTF-8 string to UTF-32 and returns it as an array of codepoints (UTF-32 string)

:Arguments: * **source_utf8_string** : string

.. _function-utf8_utils_utf8_decode_array_ls_uint_gr__string:

.. das:function:: utf8_decode(dest_utf32_string: array<uint>; source_utf8_string: string)

Converts UTF-8 string to UTF-32 and appends it to the array of codepoints (UTF-32 string)

:Arguments: * **dest_utf32_string** : array<uint>

            * **source_utf8_string** : string

+++++++++++++
Uncategorized
+++++++++++++

.. _function-utf8_utils_utf8_length_array_ls_uint8_gr_:

.. das:function:: utf8_length(utf8_string: array<uint8>) : int

Returns the number of characters in the UTF-8 string

:Arguments: * **utf8_string** : array<uint8> implicit

.. _function-utf8_utils_utf8_length_string:

.. das:function:: utf8_length(utf8_string: string) : int

Returns the number of characters in the UTF-8 string

:Arguments: * **utf8_string** : string

.. _function-utf8_utils_is_first_byte_of_utf8_char_uint8:

.. das:function:: is_first_byte_of_utf8_char(ch: uint8) : bool

|detail/function-utf8_utils-is_first_byte_of_utf8_char-0xde083daff4aefb23|

:Arguments: * **ch** : uint8

.. _function-utf8_utils_contains_utf8_bom_array_ls_uint8_gr_:

.. das:function:: contains_utf8_bom(utf8_string: array<uint8>) : bool

|detail/function-utf8_utils-contains_utf8_bom-0x5bd957db87460665|

:Arguments: * **utf8_string** : array<uint8> implicit

.. _function-utf8_utils_contains_utf8_bom_string:

.. das:function:: contains_utf8_bom(utf8_string: string) : bool

|detail/function-utf8_utils-contains_utf8_bom-0xf749642b9629daa7|

:Arguments: * **utf8_string** : string

.. _function-utf8_utils_is_utf8_string_valid_array_ls_uint8_gr_:

.. das:function:: is_utf8_string_valid(utf8_string: array<uint8>) : bool

|detail/function-utf8_utils-is_utf8_string_valid-0x2dcc5c1b2e5f2a36|

:Arguments: * **utf8_string** : array<uint8> implicit

.. _function-utf8_utils_is_utf8_string_valid_string:

.. das:function:: is_utf8_string_valid(utf8_string: string) : bool

|detail/function-utf8_utils-is_utf8_string_valid-0xfa05575fb077659e|

:Arguments: * **utf8_string** : string

.. _function-utf8_utils_decode_unicode_escape_string:

.. das:function:: decode_unicode_escape(str: string) : string

|detail/function-utf8_utils-decode_unicode_escape-0xb8b396646ee69cae|

:Arguments: * **str** : string


