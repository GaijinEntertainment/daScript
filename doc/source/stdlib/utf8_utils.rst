
.. _stdlib_utf8_utils:

===============
UTF-8 utilities
===============

// stub


+++++++++
Constants
+++++++++

.. _global-utf8_utils-UTF8_ACCEPT:

.. das:attribute:: UTF8_ACCEPT = 0x0

|detail/variable-utf8_utils-UTF8_ACCEPT|

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_utf8_utils_c__c_utf16_to_utf32_Cu_Cu:

.. das:function:: utf16_to_utf32(high: uint; low: uint) : uint

|detail/function-utf8_utils-utf16_to_utf32-0xce3eab4402448264|

:Arguments: * **high** : uint

            * **low** : uint

.. _function-_at_utf8_utils_c__c_utf8_encode_1_ls_u8_gr_A_Cu:

.. das:function:: utf8_encode(dest_array: array<uint8>; ch: uint)

|detail/function-utf8_utils-utf8_encode-0xeb1e923308ceb5e2|

:Arguments: * **dest_array** : array<uint8>

            * **ch** : uint

.. _function-_at_utf8_utils_c__c_utf8_encode_Cu:

.. das:function:: utf8_encode(ch: uint) : array<uint8>

Converts a codepoint (UTF-32 symbol) to the UTF-8 byte array


:Arguments: * **ch** : uint

.. _function-_at_utf8_utils_c__c_utf8_encode_1_ls_u8_gr_A_CI1_ls_u_gr_A:

.. das:function:: utf8_encode(dest_array: array<uint8>; source_utf32_string: array<uint> implicit)

|detail/function-utf8_utils-utf8_encode-0xf81b625b14d5373b|

:Arguments: * **dest_array** : array<uint8>

            * **source_utf32_string** : array<uint> implicit

.. _function-_at_utf8_utils_c__c_utf8_encode_CI1_ls_u_gr_A:

.. das:function:: utf8_encode(source_utf32_string: array<uint> implicit) : array<uint8>

Converts UTF-32 string to UTF-8 and returns it as a UTF-8 byte array


:Arguments: * **source_utf32_string** : array<uint> implicit

.. _function-_at_utf8_utils_c__c_utf8_length_CI1_ls_u8_gr_A:

.. das:function:: utf8_length(utf8_string: array<uint8> implicit) : int

Returns the number of characters in the UTF-8 string


:Arguments: * **utf8_string** : array<uint8> implicit

.. _function-_at_utf8_utils_c__c_utf8_length_Cs:

.. das:function:: utf8_length(utf8_string: string) : int

Returns the number of characters in the UTF-8 string


:Arguments: * **utf8_string** : string

.. _function-_at_utf8_utils_c__c_is_first_byte_of_utf8_char_Cu8:

.. das:function:: is_first_byte_of_utf8_char(ch: uint8) : bool

|detail/function-utf8_utils-is_first_byte_of_utf8_char-0xde083daff4aefb23|

:Arguments: * **ch** : uint8

.. _function-_at_utf8_utils_c__c_contains_utf8_bom_CI1_ls_u8_gr_A:

.. das:function:: contains_utf8_bom(utf8_string: array<uint8> implicit) : bool

|detail/function-utf8_utils-contains_utf8_bom-0x5338b80fe95120f2|

:Arguments: * **utf8_string** : array<uint8> implicit

.. _function-_at_utf8_utils_c__c_contains_utf8_bom_Cs:

.. das:function:: contains_utf8_bom(utf8_string: string) : bool

|detail/function-utf8_utils-contains_utf8_bom-0xf749642b9629daa7|

:Arguments: * **utf8_string** : string

.. _function-_at_utf8_utils_c__c_is_utf8_string_valid_CI1_ls_u8_gr_A:

.. das:function:: is_utf8_string_valid(utf8_string: array<uint8> implicit) : bool

|detail/function-utf8_utils-is_utf8_string_valid-0xcb9de658a2efc49c|

:Arguments: * **utf8_string** : array<uint8> implicit

.. _function-_at_utf8_utils_c__c_is_utf8_string_valid_Cs:

.. das:function:: is_utf8_string_valid(utf8_string: string) : bool

|detail/function-utf8_utils-is_utf8_string_valid-0xfa05575fb077659e|

:Arguments: * **utf8_string** : string

.. _function-_at_utf8_utils_c__c_utf8_decode_1_ls_u_gr_A_CI1_ls_u8_gr_A:

.. das:function:: utf8_decode(dest_utf32_string: array<uint>; source_utf8_string: array<uint8> implicit)

|detail/function-utf8_utils-utf8_decode-0xda9c3205aaef64d3|

:Arguments: * **dest_utf32_string** : array<uint>

            * **source_utf8_string** : array<uint8> implicit

.. _function-_at_utf8_utils_c__c_utf8_decode_CI1_ls_u8_gr_A:

.. das:function:: utf8_decode(source_utf8_string: array<uint8> implicit) : array<uint>

Converts UTF-8 string to UTF-32 and returns it as an array of codepoints (UTF-32 string)


:Arguments: * **source_utf8_string** : array<uint8> implicit

.. _function-_at_utf8_utils_c__c_utf8_decode_Cs:

.. das:function:: utf8_decode(source_utf8_string: string) : array<uint>

Converts UTF-8 string to UTF-32 and returns it as an array of codepoints (UTF-32 string)


:Arguments: * **source_utf8_string** : string

.. _function-_at_utf8_utils_c__c_utf8_decode_1_ls_u_gr_A_Cs:

.. das:function:: utf8_decode(dest_utf32_string: array<uint>; source_utf8_string: string)

Converts UTF-8 string to UTF-32 and appends it to the array of codepoints (UTF-32 string)


:Arguments: * **dest_utf32_string** : array<uint>

            * **source_utf8_string** : string

.. _function-_at_utf8_utils_c__c_decode_unicode_escape_Cs:

.. das:function:: decode_unicode_escape(str: string) : string

|detail/function-utf8_utils-decode_unicode_escape-0xb8b396646ee69cae|

:Arguments: * **str** : string


