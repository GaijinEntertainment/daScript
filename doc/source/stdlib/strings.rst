
.. _stdlib_strings:

===========================
String manipulation library
===========================

The STRINGS module implements string formatting, conversion, searching, and modification
routines. It provides functions for building strings (``build_string``), parsing
(``to_int``, ``to_float``), character classification (``is_alpha``, ``is_number``),
and low-level string manipulation.

All functions and symbols are in "strings" module, use require to get access to it. ::

    require strings

++++++++++++
Enumerations
++++++++++++

.. _enum-strings-ConversionResult:

.. das:attribute:: ConversionResult

Result of conversion from string to number.

:Values: * **ok** = 0 - Successful conversion

         * **invalid_argument** = 22 - Argument is not a valid number

         * **out_of_range** = 34 - Argument is out of range for the target type


++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-strings-StringBuilderWriter:

.. das:attribute:: StringBuilderWriter

Object representing a string builder. Its significantly faster to write data to the string builder and than convert it to a string, as oppose to using sequences of string concatenations.


+++++++++++++
Character set
+++++++++++++

  *  :ref:`is_char_in_set (Character: int; Charset: uint const[8] implicit) : bool <function-strings_is_char_in_set_int_uint_const8_implicit>` 
  *  :ref:`set_total (Charset: uint const[8] implicit) : uint <function-strings_set_total_uint_const8_implicit>` 
  *  :ref:`set_element (Character: int; Charset: uint const[8] implicit) : int <function-strings_set_element_int_uint_const8_implicit>` 

.. _function-strings_is_char_in_set_int_uint_const8_implicit:

.. das:function:: is_char_in_set(Character: int; Charset: uint const[8] implicit) : bool

Returns true if character bit is set in the set (of 256 bits in uint32[8]).

:Arguments: * **Character** : int

            * **Charset** : uint[8] implicit

.. _function-strings_set_total_uint_const8_implicit:

.. das:function:: set_total(Charset: uint const[8] implicit) : uint

Total number of elements in the character set.

:Arguments: * **Charset** : uint[8] implicit

.. _function-strings_set_element_int_uint_const8_implicit:

.. das:function:: set_element(Character: int; Charset: uint const[8] implicit) : int

Gen character set element by element index (not character index).

:Arguments: * **Character** : int

            * **Charset** : uint[8] implicit

++++++++++++++++
Character groups
++++++++++++++++

  *  :ref:`is_alpha (Character: int) : bool <function-strings_is_alpha_int>` 
  *  :ref:`is_alnum (Character: int) : bool <function-strings_is_alnum_int>` 
  *  :ref:`is_hex (Character: int) : bool <function-strings_is_hex_int>` 
  *  :ref:`is_tab_or_space (Character: int) : bool <function-strings_is_tab_or_space_int>` 
  *  :ref:`is_new_line (Character: int) : bool <function-strings_is_new_line_int>` 
  *  :ref:`is_white_space (Character: int) : bool <function-strings_is_white_space_int>` 
  *  :ref:`is_number (Character: int) : bool <function-strings_is_number_int>` 

.. _function-strings_is_alpha_int:

.. das:function:: is_alpha(Character: int) : bool

Returns true if character is [A-Za-z].

:Arguments: * **Character** : int

.. _function-strings_is_alnum_int:

.. das:function:: is_alnum(Character: int) : bool

Returns true if character is alphanumeric [A-Za-z0-9].

:Arguments: * **Character** : int

.. _function-strings_is_hex_int:

.. das:function:: is_hex(Character: int) : bool

Returns true if character is hexadecimal [0-9A-Fa-f].

:Arguments: * **Character** : int

.. _function-strings_is_tab_or_space_int:

.. das:function:: is_tab_or_space(Character: int) : bool

Returns true if character is tab or space [ \t].

:Arguments: * **Character** : int

.. _function-strings_is_new_line_int:

.. das:function:: is_new_line(Character: int) : bool

Returns true if character is '\n' or '\r'.

:Arguments: * **Character** : int

.. _function-strings_is_white_space_int:

.. das:function:: is_white_space(Character: int) : bool

Returns true if character is [ \t\n\r].

:Arguments: * **Character** : int

.. _function-strings_is_number_int:

.. das:function:: is_number(Character: int) : bool

Returns true if character is [0-9].

:Arguments: * **Character** : int

++++++++++++++++++
Character by index
++++++++++++++++++

  *  :ref:`character_at (str: string implicit; idx: int) : int <function-strings_character_at_string_implicit_int>` 
  *  :ref:`character_uat (str: string implicit; idx: int) : int <function-strings_character_uat_string_implicit_int>` 

.. _function-strings_character_at_string_implicit_int:

.. das:function:: character_at(str: string implicit; idx: int) : int

Returns character of the string 'str' at index 'idx'.

:Arguments: * **str** : string implicit

            * **idx** : int

.. _function-strings_character_uat_string_implicit_int:

.. das:function:: character_uat(str: string implicit; idx: int) : int

.. warning:: 
  This is unsafe operation.

Returns character of the string 'str' at index 'idx'. This function does not check bounds of index.

:Arguments: * **str** : string implicit

            * **idx** : int

+++++++++++++++++
String properties
+++++++++++++++++

  *  :ref:`ends_with (str: string implicit; cmp: string implicit) : bool <function-strings_ends_with_string_implicit_string_implicit>` 
  *  :ref:`ends_with (str: das_string implicit; cmp: string implicit) : bool <function-strings_ends_with_das_string_implicit_string_implicit>` 
  *  :ref:`starts_with (str: string implicit; cmp: string implicit) : bool <function-strings_starts_with_string_implicit_string_implicit>` 
  *  :ref:`starts_with (str: string implicit; cmp: string implicit; cmpLen: uint) : bool <function-strings_starts_with_string_implicit_string_implicit_uint>` 
  *  :ref:`starts_with (str: string implicit; offset: int; cmp: string implicit) : bool <function-strings_starts_with_string_implicit_int_string_implicit>` 
  *  :ref:`starts_with (str: string implicit; offset: int; cmp: string implicit; cmpLen: uint) : bool <function-strings_starts_with_string_implicit_int_string_implicit_uint>` 
  *  :ref:`length (str: string implicit) : int <function-strings_length_string_implicit>` 
  *  :ref:`length (str: das_string implicit) : int <function-strings_length_das_string_implicit>` 

.. _function-strings_ends_with_string_implicit_string_implicit:

.. das:function:: ends_with(str: string implicit; cmp: string implicit) : bool

returns `true` if the end of the string `str`  matches a the string `cmp` otherwise returns `false`

:Arguments: * **str** : string implicit

            * **cmp** : string implicit

.. _function-strings_ends_with_das_string_implicit_string_implicit:

.. das:function:: ends_with(str: das_string implicit; cmp: string implicit) : bool

returns `true` if the end of the string `str`  matches a the string `cmp` otherwise returns `false`

:Arguments: * **str** :  :ref:`das_string <handle-builtin-das_string>`  implicit

            * **cmp** : string implicit

.. _function-strings_starts_with_string_implicit_string_implicit:

.. das:function:: starts_with(str: string implicit; cmp: string implicit) : bool

returns `true` if the beginning of the string `str` matches the string `cmp`; otherwise returns `false`

:Arguments: * **str** : string implicit

            * **cmp** : string implicit

.. _function-strings_starts_with_string_implicit_string_implicit_uint:

.. das:function:: starts_with(str: string implicit; cmp: string implicit; cmpLen: uint) : bool

returns `true` if the beginning of the string `str` matches the string `cmp`; otherwise returns `false`

:Arguments: * **str** : string implicit

            * **cmp** : string implicit

            * **cmpLen** : uint

.. _function-strings_starts_with_string_implicit_int_string_implicit:

.. das:function:: starts_with(str: string implicit; offset: int; cmp: string implicit) : bool

returns `true` if the beginning of the string `str` matches the string `cmp`; otherwise returns `false`

:Arguments: * **str** : string implicit

            * **offset** : int

            * **cmp** : string implicit

.. _function-strings_starts_with_string_implicit_int_string_implicit_uint:

.. das:function:: starts_with(str: string implicit; offset: int; cmp: string implicit; cmpLen: uint) : bool

returns `true` if the beginning of the string `str` matches the string `cmp`; otherwise returns `false`

:Arguments: * **str** : string implicit

            * **offset** : int

            * **cmp** : string implicit

            * **cmpLen** : uint

.. _function-strings_length_string_implicit:

.. das:function:: length(str: string implicit) : int

Return length of string

:Arguments: * **str** : string implicit

.. _function-strings_length_das_string_implicit:

.. das:function:: length(str: das_string implicit) : int

Return length of string

:Arguments: * **str** :  :ref:`das_string <handle-builtin-das_string>`  implicit

++++++++++++++
String builder
++++++++++++++

  *  :ref:`build_string (block: block\<(StringBuilderWriter):void\>) : string <function-strings_build_string_block_ls_StringBuilderWriter_c_void_gr_>` 
  *  :ref:`build_hash (block: block\<(StringBuilderWriter):void\>) : uint64 <function-strings_build_hash_block_ls_StringBuilderWriter_c_void_gr_>` 
  *  :ref:`write (writer: StringBuilderWriter; anything: any) : StringBuilderWriter& <function-strings_write_StringBuilderWriter_any>` 
  *  :ref:`write_char (writer: StringBuilderWriter implicit; ch: int) : StringBuilderWriter& <function-strings_write_char_StringBuilderWriter_implicit_int>` 
  *  :ref:`write_chars (writer: StringBuilderWriter implicit; ch: int; count: int) : StringBuilderWriter& <function-strings_write_chars_StringBuilderWriter_implicit_int_int>` 
  *  :ref:`write_escape_string (writer: StringBuilderWriter implicit; str: string implicit) : StringBuilderWriter& <function-strings_write_escape_string_StringBuilderWriter_implicit_string_implicit>` 
  *  :ref:`format (writer: StringBuilderWriter implicit; format: string implicit; value: int) : StringBuilderWriter& <function-strings_format_StringBuilderWriter_implicit_string_implicit_int>` 
  *  :ref:`format (writer: StringBuilderWriter implicit; format: string implicit; value: uint) : StringBuilderWriter& <function-strings_format_StringBuilderWriter_implicit_string_implicit_uint>` 
  *  :ref:`format (writer: StringBuilderWriter implicit; format: string implicit; value: int64) : StringBuilderWriter& <function-strings_format_StringBuilderWriter_implicit_string_implicit_int64>` 
  *  :ref:`format (writer: StringBuilderWriter implicit; format: string implicit; value: uint64) : StringBuilderWriter& <function-strings_format_StringBuilderWriter_implicit_string_implicit_uint64>` 
  *  :ref:`format (writer: StringBuilderWriter implicit; format: string implicit; value: float) : StringBuilderWriter& <function-strings_format_StringBuilderWriter_implicit_string_implicit_float>` 
  *  :ref:`format (writer: StringBuilderWriter implicit; format: string implicit; value: double) : StringBuilderWriter& <function-strings_format_StringBuilderWriter_implicit_string_implicit_double>` 
  *  :ref:`format (format: string implicit; value: int) : string <function-strings_format_string_implicit_int>` 
  *  :ref:`format (format: string implicit; value: uint) : string <function-strings_format_string_implicit_uint>` 
  *  :ref:`format (format: string implicit; value: int64) : string <function-strings_format_string_implicit_int64>` 
  *  :ref:`format (format: string implicit; value: uint64) : string <function-strings_format_string_implicit_uint64>` 
  *  :ref:`format (format: string implicit; value: float) : string <function-strings_format_string_implicit_float>` 
  *  :ref:`format (format: string implicit; value: double) : string <function-strings_format_string_implicit_double>` 

.. _function-strings_build_string_block_ls_StringBuilderWriter_c_void_gr_:

.. das:function:: build_string(block: block<(StringBuilderWriter):void>) : string

Create StringBuilderWriter and pass it to the block. Upon completion of a block, return whatever was written as string.

:Arguments: * **block** : block<( :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>` ):void> implicit

.. _function-strings_build_hash_block_ls_StringBuilderWriter_c_void_gr_:

.. das:function:: build_hash(block: block<(StringBuilderWriter):void>) : uint64

Build hash of the string (as oppose to building entire string).

:Arguments: * **block** : block<( :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>` ):void> implicit

.. _function-strings_write_StringBuilderWriter_any:

.. das:function:: write(writer: StringBuilderWriter; anything: any) : StringBuilderWriter&

Returns textual representation of the value.

:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>` 

            * **anything** : any

.. _function-strings_write_char_StringBuilderWriter_implicit_int:

.. das:function:: write_char(writer: StringBuilderWriter implicit; ch: int) : StringBuilderWriter&

Writes character into StringBuilderWriter.

:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **ch** : int

.. _function-strings_write_chars_StringBuilderWriter_implicit_int_int:

.. das:function:: write_chars(writer: StringBuilderWriter implicit; ch: int; count: int) : StringBuilderWriter&

Writes multiple characters into StringBuilderWriter.

:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **ch** : int

            * **count** : int

.. _function-strings_write_escape_string_StringBuilderWriter_implicit_string_implicit:

.. das:function:: write_escape_string(writer: StringBuilderWriter implicit; str: string implicit) : StringBuilderWriter&

Writes escaped string into StringBuilderWriter.

:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **str** : string implicit

.. _function-strings_format_StringBuilderWriter_implicit_string_implicit_int:

.. das:function:: format(writer: StringBuilderWriter implicit; format: string implicit; value: int) : StringBuilderWriter&

.. warning:: 
  This function is deprecated.

Converts value to string given specified format (that of C printf).

:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **format** : string implicit

            * **value** : int

.. _function-strings_format_StringBuilderWriter_implicit_string_implicit_uint:

.. das:function:: format(writer: StringBuilderWriter implicit; format: string implicit; value: uint) : StringBuilderWriter&

.. warning:: 
  This function is deprecated.

Converts value to string given specified format (that of C printf).

:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **format** : string implicit

            * **value** : uint

.. _function-strings_format_StringBuilderWriter_implicit_string_implicit_int64:

.. das:function:: format(writer: StringBuilderWriter implicit; format: string implicit; value: int64) : StringBuilderWriter&

.. warning:: 
  This function is deprecated.

Converts value to string given specified format (that of C printf).

:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **format** : string implicit

            * **value** : int64

.. _function-strings_format_StringBuilderWriter_implicit_string_implicit_uint64:

.. das:function:: format(writer: StringBuilderWriter implicit; format: string implicit; value: uint64) : StringBuilderWriter&

.. warning:: 
  This function is deprecated.

Converts value to string given specified format (that of C printf).

:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **format** : string implicit

            * **value** : uint64

.. _function-strings_format_StringBuilderWriter_implicit_string_implicit_float:

.. das:function:: format(writer: StringBuilderWriter implicit; format: string implicit; value: float) : StringBuilderWriter&

.. warning:: 
  This function is deprecated.

Converts value to string given specified format (that of C printf).

:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **format** : string implicit

            * **value** : float

.. _function-strings_format_StringBuilderWriter_implicit_string_implicit_double:

.. das:function:: format(writer: StringBuilderWriter implicit; format: string implicit; value: double) : StringBuilderWriter&

.. warning:: 
  This function is deprecated.

Converts value to string given specified format (that of C printf).

:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **format** : string implicit

            * **value** : double

.. _function-strings_format_string_implicit_int:

.. das:function:: format(format: string implicit; value: int) : string

.. warning:: 
  This function is deprecated.

Converts value to string given specified format (that of C printf).

:Arguments: * **format** : string implicit

            * **value** : int

.. _function-strings_format_string_implicit_uint:

.. das:function:: format(format: string implicit; value: uint) : string

.. warning:: 
  This function is deprecated.

Converts value to string given specified format (that of C printf).

:Arguments: * **format** : string implicit

            * **value** : uint

.. _function-strings_format_string_implicit_int64:

.. das:function:: format(format: string implicit; value: int64) : string

.. warning:: 
  This function is deprecated.

Converts value to string given specified format (that of C printf).

:Arguments: * **format** : string implicit

            * **value** : int64

.. _function-strings_format_string_implicit_uint64:

.. das:function:: format(format: string implicit; value: uint64) : string

.. warning:: 
  This function is deprecated.

Converts value to string given specified format (that of C printf).

:Arguments: * **format** : string implicit

            * **value** : uint64

.. _function-strings_format_string_implicit_float:

.. das:function:: format(format: string implicit; value: float) : string

.. warning:: 
  This function is deprecated.

Converts value to string given specified format (that of C printf).

:Arguments: * **format** : string implicit

            * **value** : float

.. _function-strings_format_string_implicit_double:

.. das:function:: format(format: string implicit; value: double) : string

.. warning:: 
  This function is deprecated.

Converts value to string given specified format (that of C printf).

:Arguments: * **format** : string implicit

            * **value** : double

++++++++++++++++++++++++
das::string manipulation
++++++++++++++++++++++++

  *  :ref:`append (str: das_string implicit; ch: int) <function-strings_append_das_string_implicit_int>` 
  *  :ref:`resize (str: das_string implicit; new_length: int) <function-strings_resize_das_string_implicit_int>` 

.. _function-strings_append_das_string_implicit_int:

.. das:function:: append(str: das_string implicit; ch: int)

Appends single character `ch` to das::string `str`.

:Arguments: * **str** :  :ref:`das_string <handle-builtin-das_string>`  implicit

            * **ch** : int

.. _function-strings_resize_das_string_implicit_int:

.. das:function:: resize(str: das_string implicit; new_length: int)

Resize string, i.e make it specified length.

:Arguments: * **str** :  :ref:`das_string <handle-builtin-das_string>`  implicit

            * **new_length** : int

++++++++++++++++++++
String modifications
++++++++++++++++++++

  *  :ref:`repeat (str: string implicit; count: int) : string <function-strings_repeat_string_implicit_int>` 
  *  :ref:`strip (str: string implicit) : string <function-strings_strip_string_implicit>` 
  *  :ref:`strip_right (str: string implicit) : string <function-strings_strip_right_string_implicit>` 
  *  :ref:`strip_left (str: string implicit) : string <function-strings_strip_left_string_implicit>` 
  *  :ref:`chop (str: string implicit; start: int; length: int) : string <function-strings_chop_string_implicit_int_int>` 
  *  :ref:`slice (str: string implicit; start: int; end: int) : string <function-strings_slice_string_implicit_int_int>` 
  *  :ref:`slice (str: string implicit; start: int) : string <function-strings_slice_string_implicit_int>` 
  *  :ref:`reverse (str: string implicit) : string <function-strings_reverse_string_implicit>` 
  *  :ref:`to_upper (str: string implicit) : string <function-strings_to_upper_string_implicit>` 
  *  :ref:`to_lower (str: string implicit) : string <function-strings_to_lower_string_implicit>` 
  *  :ref:`to_lower_in_place (str: string implicit) : string <function-strings_to_lower_in_place_string_implicit>` 
  *  :ref:`to_upper_in_place (str: string implicit) : string <function-strings_to_upper_in_place_string_implicit>` 
  *  :ref:`escape (str: string implicit) : string <function-strings_escape_string_implicit>` 
  *  :ref:`unescape (str: string implicit) : string <function-strings_unescape_string_implicit>` 
  *  :ref:`safe_unescape (str: string implicit) : string <function-strings_safe_unescape_string_implicit>` 
  *  :ref:`replace (str: string implicit; toSearch: string implicit; replace: string implicit) : string <function-strings_replace_string_implicit_string_implicit_string_implicit>` 
  *  :ref:`rtrim (str: string implicit) : string <function-strings_rtrim_string_implicit>` 
  *  :ref:`rtrim (str: string implicit; chars: string implicit) : string <function-strings_rtrim_string_implicit_string_implicit>` 
  *  :ref:`ltrim (str: string implicit) : string <function-strings_ltrim_string_implicit>` 
  *  :ref:`trim (str: string implicit) : string <function-strings_trim_string_implicit>` 

.. _function-strings_repeat_string_implicit_int:

.. das:function:: repeat(str: string implicit; count: int) : string

Repeat string specified number of times, and return the result.

:Arguments: * **str** : string implicit

            * **count** : int

.. _function-strings_strip_string_implicit:

.. das:function:: strip(str: string implicit) : string

Strips white-space-only characters that might appear at the beginning or end of the given string and returns the new stripped string.

:Arguments: * **str** : string implicit

.. _function-strings_strip_right_string_implicit:

.. das:function:: strip_right(str: string implicit) : string

Strips white-space-only characters that might appear at the end of the given string and returns the new stripped string.

:Arguments: * **str** : string implicit

.. _function-strings_strip_left_string_implicit:

.. das:function:: strip_left(str: string implicit) : string

Strips white-space-only characters that might appear at the beginning of the given string and returns the new stripped string.

:Arguments: * **str** : string implicit

.. _function-strings_chop_string_implicit_int_int:

.. das:function:: chop(str: string implicit; start: int; length: int) : string

Return all part of the strings starting at start and ending at start + length.

:Arguments: * **str** : string implicit

            * **start** : int

            * **length** : int

.. _function-strings_slice_string_implicit_int_int:

.. das:function:: slice(str: string implicit; start: int; end: int) : string

Return all part of the strings starting at start and ending by end. Start can be negative (-1 means "1 from the end").

:Arguments: * **str** : string implicit

            * **start** : int

            * **end** : int

.. _function-strings_slice_string_implicit_int:

.. das:function:: slice(str: string implicit; start: int) : string

Return all part of the strings starting at start and ending by end. Start can be negative (-1 means "1 from the end").

:Arguments: * **str** : string implicit

            * **start** : int

.. _function-strings_reverse_string_implicit:

.. das:function:: reverse(str: string implicit) : string

Return reversed string

:Arguments: * **str** : string implicit

.. _function-strings_to_upper_string_implicit:

.. das:function:: to_upper(str: string implicit) : string

Return all upper case string

:Arguments: * **str** : string implicit

.. _function-strings_to_lower_string_implicit:

.. das:function:: to_lower(str: string implicit) : string

Return all lower case string

:Arguments: * **str** : string implicit

.. _function-strings_to_lower_in_place_string_implicit:

.. das:function:: to_lower_in_place(str: string implicit) : string

.. warning:: 
  This is unsafe operation.

Modify string in place to be all lower case

:Arguments: * **str** : string implicit

.. _function-strings_to_upper_in_place_string_implicit:

.. das:function:: to_upper_in_place(str: string implicit) : string

.. warning:: 
  This is unsafe operation.

Modify string in place to be all upper case string

:Arguments: * **str** : string implicit

.. _function-strings_escape_string_implicit:

.. das:function:: escape(str: string implicit) : string

Escape string so that escape sequences are printable, for example converting "\n" into "\\n".

:Arguments: * **str** : string implicit

.. _function-strings_unescape_string_implicit:

.. das:function:: unescape(str: string implicit) : string

Unescape string i.e reverse effects of `escape`. For example "\\n" is converted to "\n".

:Arguments: * **str** : string implicit

.. _function-strings_safe_unescape_string_implicit:

.. das:function:: safe_unescape(str: string implicit) : string

Unescape string i.e reverse effects of `escape`. For example "\\n" is converted to "\n".

:Arguments: * **str** : string implicit

.. _function-strings_replace_string_implicit_string_implicit_string_implicit:

.. das:function:: replace(str: string implicit; toSearch: string implicit; replace: string implicit) : string

Replace all occurances of the stubstring in the string with another substring.

:Arguments: * **str** : string implicit

            * **toSearch** : string implicit

            * **replace** : string implicit

.. _function-strings_rtrim_string_implicit:

.. das:function:: rtrim(str: string implicit) : string

Removes trailing white space.

:Arguments: * **str** : string implicit

.. _function-strings_rtrim_string_implicit_string_implicit:

.. das:function:: rtrim(str: string implicit; chars: string implicit) : string

Removes trailing white space.

:Arguments: * **str** : string implicit

            * **chars** : string implicit

.. _function-strings_ltrim_string_implicit:

.. das:function:: ltrim(str: string implicit) : string

Removes leading white space.

:Arguments: * **str** : string implicit

.. _function-strings_trim_string_implicit:

.. das:function:: trim(str: string implicit) : string

Removes leading and trailing white space.

:Arguments: * **str** : string implicit

+++++++++++++++++
Search substrings
+++++++++++++++++

  *  :ref:`find (str: string implicit; substr: string implicit; start: int) : int <function-strings_find_string_implicit_string_implicit_int>` 
  *  :ref:`find (str: string implicit; substr: string implicit) : int <function-strings_find_string_implicit_string_implicit>` 
  *  :ref:`find (str: string implicit; substr: int) : int <function-strings_find_string_implicit_int>` 
  *  :ref:`find (str: string implicit; substr: int; start: int) : int <function-strings_find_string_implicit_int_int>` 

.. _function-strings_find_string_implicit_string_implicit_int:

.. das:function:: find(str: string implicit; substr: string implicit; start: int) : int

Return index where substr can be found within str (starting from optional 'start' at), or -1 if not found

:Arguments: * **str** : string implicit

            * **substr** : string implicit

            * **start** : int

.. _function-strings_find_string_implicit_string_implicit:

.. das:function:: find(str: string implicit; substr: string implicit) : int

Return index where substr can be found within str (starting from optional 'start' at), or -1 if not found

:Arguments: * **str** : string implicit

            * **substr** : string implicit

.. _function-strings_find_string_implicit_int:

.. das:function:: find(str: string implicit; substr: int) : int

Return index where substr can be found within str (starting from optional 'start' at), or -1 if not found

:Arguments: * **str** : string implicit

            * **substr** : int

.. _function-strings_find_string_implicit_int_int:

.. das:function:: find(str: string implicit; substr: int; start: int) : int

Return index where substr can be found within str (starting from optional 'start' at), or -1 if not found

:Arguments: * **str** : string implicit

            * **substr** : int

            * **start** : int

++++++++++++++++++++++++++
String conversion routines
++++++++++++++++++++++++++

  *  :ref:`fmt (writer: StringBuilderWriter implicit; format: string implicit; value: int8) : StringBuilderWriter& <function-strings_fmt_StringBuilderWriter_implicit_string_implicit_int8>` 
  *  :ref:`fmt (writer: StringBuilderWriter implicit; format: string implicit; value: uint8) : StringBuilderWriter& <function-strings_fmt_StringBuilderWriter_implicit_string_implicit_uint8>` 
  *  :ref:`fmt (writer: StringBuilderWriter implicit; format: string implicit; value: int16) : StringBuilderWriter& <function-strings_fmt_StringBuilderWriter_implicit_string_implicit_int16>` 
  *  :ref:`fmt (writer: StringBuilderWriter implicit; format: string implicit; value: uint16) : StringBuilderWriter& <function-strings_fmt_StringBuilderWriter_implicit_string_implicit_uint16>` 
  *  :ref:`fmt (writer: StringBuilderWriter implicit; format: string implicit; value: int) : StringBuilderWriter& <function-strings_fmt_StringBuilderWriter_implicit_string_implicit_int>` 
  *  :ref:`fmt (writer: StringBuilderWriter implicit; format: string implicit; value: uint) : StringBuilderWriter& <function-strings_fmt_StringBuilderWriter_implicit_string_implicit_uint>` 
  *  :ref:`fmt (writer: StringBuilderWriter implicit; format: string implicit; value: int64) : StringBuilderWriter& <function-strings_fmt_StringBuilderWriter_implicit_string_implicit_int64>` 
  *  :ref:`fmt (writer: StringBuilderWriter implicit; format: string implicit; value: uint64) : StringBuilderWriter& <function-strings_fmt_StringBuilderWriter_implicit_string_implicit_uint64>` 
  *  :ref:`fmt (writer: StringBuilderWriter implicit; format: string implicit; value: float) : StringBuilderWriter& <function-strings_fmt_StringBuilderWriter_implicit_string_implicit_float>` 
  *  :ref:`fmt (writer: StringBuilderWriter implicit; format: string implicit; value: double) : StringBuilderWriter& <function-strings_fmt_StringBuilderWriter_implicit_string_implicit_double>` 
  *  :ref:`string (bytes: array\<uint8\>) : string <function-strings_string_array_ls_uint8_gr_>` 
  *  :ref:`to_char (char: int) : string <function-strings_to_char_int>` 
  *  :ref:`int8 (str: string implicit) : int8 <function-strings_int8_string_implicit>` 
  *  :ref:`uint8 (str: string implicit) : uint8 <function-strings_uint8_string_implicit>` 
  *  :ref:`int16 (str: string implicit) : int16 <function-strings_int16_string_implicit>` 
  *  :ref:`uint16 (str: string implicit) : uint16 <function-strings_uint16_string_implicit>` 
  *  :ref:`int (str: string implicit) : int <function-strings_int_string_implicit>` 
  *  :ref:`uint (str: string implicit) : uint <function-strings_uint_string_implicit>` 
  *  :ref:`int64 (str: string implicit) : int64 <function-strings_int64_string_implicit>` 
  *  :ref:`uint64 (str: string implicit) : uint64 <function-strings_uint64_string_implicit>` 
  *  :ref:`float (str: string implicit) : float <function-strings_float_string_implicit>` 
  *  :ref:`double (str: string implicit) : double <function-strings_double_string_implicit>` 
  *  :ref:`to_int8 (value: string implicit; hex: bool = false) : int8 <function-strings_to_int8_string_implicit_bool>` 
  *  :ref:`to_uint8 (value: string implicit; hex: bool = false) : uint8 <function-strings_to_uint8_string_implicit_bool>` 
  *  :ref:`to_int16 (value: string implicit; hex: bool = false) : int16 <function-strings_to_int16_string_implicit_bool>` 
  *  :ref:`to_int (value: string implicit; hex: bool = false) : int <function-strings_to_int_string_implicit_bool>` 
  *  :ref:`to_uint (value: string implicit; hex: bool = false) : uint <function-strings_to_uint_string_implicit_bool>` 
  *  :ref:`to_int64 (value: string implicit; hex: bool = false) : int64 <function-strings_to_int64_string_implicit_bool>` 
  *  :ref:`to_uint64 (value: string implicit; hex: bool = false) : uint64 <function-strings_to_uint64_string_implicit_bool>` 
  *  :ref:`to_cpp_float (value: float) : string <function-strings_to_cpp_float_float>` 
  *  :ref:`to_float (value: string implicit) : float <function-strings_to_float_string_implicit>` 
  *  :ref:`to_double (value: string implicit) : double <function-strings_to_double_string_implicit>` 
  *  :ref:`int8 (str: string implicit; result: ConversionResult& implicit; offset: int& implicit; hex: bool = false) : int8 <function-strings_int8_string_implicit_ConversionResult_implicit_int_implicit_bool>` 
  *  :ref:`uint8 (str: string implicit; result: ConversionResult& implicit; offset: int& implicit; hex: bool = false) : uint8 <function-strings_uint8_string_implicit_ConversionResult_implicit_int_implicit_bool>` 
  *  :ref:`int16 (str: string implicit; result: ConversionResult& implicit; offset: int& implicit; hex: bool = false) : int16 <function-strings_int16_string_implicit_ConversionResult_implicit_int_implicit_bool>` 
  *  :ref:`uint16 (str: string implicit; result: ConversionResult& implicit; offset: int& implicit; hex: bool = false) : uint16 <function-strings_uint16_string_implicit_ConversionResult_implicit_int_implicit_bool>` 
  *  :ref:`int (str: string implicit; result: ConversionResult& implicit; offset: int& implicit; hex: bool = false) : int <function-strings_int_string_implicit_ConversionResult_implicit_int_implicit_bool>` 
  *  :ref:`uint (str: string implicit; result: ConversionResult& implicit; offset: int& implicit; hex: bool = false) : uint <function-strings_uint_string_implicit_ConversionResult_implicit_int_implicit_bool>` 
  *  :ref:`int64 (str: string implicit; result: ConversionResult& implicit; offset: int& implicit; hex: bool = false) : int64 <function-strings_int64_string_implicit_ConversionResult_implicit_int_implicit_bool>` 
  *  :ref:`uint64 (str: string implicit; result: ConversionResult& implicit; offset: int& implicit; hex: bool = false) : uint64 <function-strings_uint64_string_implicit_ConversionResult_implicit_int_implicit_bool>` 
  *  :ref:`float (str: string implicit; result: ConversionResult& implicit; offset: int& implicit) : float <function-strings_float_string_implicit_ConversionResult_implicit_int_implicit>` 
  *  :ref:`double (str: string implicit; result: ConversionResult& implicit; offset: int& implicit) : double <function-strings_double_string_implicit_ConversionResult_implicit_int_implicit>` 

.. _function-strings_fmt_StringBuilderWriter_implicit_string_implicit_int8:

.. das:function:: fmt(writer: StringBuilderWriter implicit; format: string implicit; value: int8) : StringBuilderWriter&

Converts value to string given specified format (that of libfmt or C++20 std::format).

:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **format** : string implicit

            * **value** : int8

.. _function-strings_fmt_StringBuilderWriter_implicit_string_implicit_uint8:

.. das:function:: fmt(writer: StringBuilderWriter implicit; format: string implicit; value: uint8) : StringBuilderWriter&

Converts value to string given specified format (that of libfmt or C++20 std::format).

:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **format** : string implicit

            * **value** : uint8

.. _function-strings_fmt_StringBuilderWriter_implicit_string_implicit_int16:

.. das:function:: fmt(writer: StringBuilderWriter implicit; format: string implicit; value: int16) : StringBuilderWriter&

Converts value to string given specified format (that of libfmt or C++20 std::format).

:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **format** : string implicit

            * **value** : int16

.. _function-strings_fmt_StringBuilderWriter_implicit_string_implicit_uint16:

.. das:function:: fmt(writer: StringBuilderWriter implicit; format: string implicit; value: uint16) : StringBuilderWriter&

Converts value to string given specified format (that of libfmt or C++20 std::format).

:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **format** : string implicit

            * **value** : uint16

.. _function-strings_fmt_StringBuilderWriter_implicit_string_implicit_int:

.. das:function:: fmt(writer: StringBuilderWriter implicit; format: string implicit; value: int) : StringBuilderWriter&

Converts value to string given specified format (that of libfmt or C++20 std::format).

:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **format** : string implicit

            * **value** : int

.. _function-strings_fmt_StringBuilderWriter_implicit_string_implicit_uint:

.. das:function:: fmt(writer: StringBuilderWriter implicit; format: string implicit; value: uint) : StringBuilderWriter&

Converts value to string given specified format (that of libfmt or C++20 std::format).

:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **format** : string implicit

            * **value** : uint

.. _function-strings_fmt_StringBuilderWriter_implicit_string_implicit_int64:

.. das:function:: fmt(writer: StringBuilderWriter implicit; format: string implicit; value: int64) : StringBuilderWriter&

Converts value to string given specified format (that of libfmt or C++20 std::format).

:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **format** : string implicit

            * **value** : int64

.. _function-strings_fmt_StringBuilderWriter_implicit_string_implicit_uint64:

.. das:function:: fmt(writer: StringBuilderWriter implicit; format: string implicit; value: uint64) : StringBuilderWriter&

Converts value to string given specified format (that of libfmt or C++20 std::format).

:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **format** : string implicit

            * **value** : uint64

.. _function-strings_fmt_StringBuilderWriter_implicit_string_implicit_float:

.. das:function:: fmt(writer: StringBuilderWriter implicit; format: string implicit; value: float) : StringBuilderWriter&

Converts value to string given specified format (that of libfmt or C++20 std::format).

:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **format** : string implicit

            * **value** : float

.. _function-strings_fmt_StringBuilderWriter_implicit_string_implicit_double:

.. das:function:: fmt(writer: StringBuilderWriter implicit; format: string implicit; value: double) : StringBuilderWriter&

Converts value to string given specified format (that of libfmt or C++20 std::format).

:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **format** : string implicit

            * **value** : double

.. _function-strings_string_array_ls_uint8_gr_:

.. das:function:: string(bytes: array<uint8>) : string

Return string from the byte array.

:Arguments: * **bytes** : array<uint8> implicit

.. _function-strings_to_char_int:

.. das:function:: to_char(char: int) : string

Convert character to string.

:Arguments: * **char** : int

.. _function-strings_int8_string_implicit:

.. das:function:: int8(str: string implicit) : int8

Converts string to int8. In case of error panic.

:Arguments: * **str** : string implicit

.. _function-strings_uint8_string_implicit:

.. das:function:: uint8(str: string implicit) : uint8

Convert string to uint8. In case of error panic.

:Arguments: * **str** : string implicit

.. _function-strings_int16_string_implicit:

.. das:function:: int16(str: string implicit) : int16

Converts string to int16. In case of error panic.

:Arguments: * **str** : string implicit

.. _function-strings_uint16_string_implicit:

.. das:function:: uint16(str: string implicit) : uint16

Convert string to uint16. In case of error panic.

:Arguments: * **str** : string implicit

.. _function-strings_int_string_implicit:

.. das:function:: int(str: string implicit) : int

Converts string to integer. In case of error panic.

:Arguments: * **str** : string implicit

.. _function-strings_uint_string_implicit:

.. das:function:: uint(str: string implicit) : uint

Convert string to uint. In case of error panic.

:Arguments: * **str** : string implicit

.. _function-strings_int64_string_implicit:

.. das:function:: int64(str: string implicit) : int64

Converts string to int64. In case of error panic.

:Arguments: * **str** : string implicit

.. _function-strings_uint64_string_implicit:

.. das:function:: uint64(str: string implicit) : uint64

Convert string to uint64. In case of error panic.

:Arguments: * **str** : string implicit

.. _function-strings_float_string_implicit:

.. das:function:: float(str: string implicit) : float

Converts string to float. In case of error panic.

:Arguments: * **str** : string implicit

.. _function-strings_double_string_implicit:

.. das:function:: double(str: string implicit) : double

Converts string to double. In case of error panic.

:Arguments: * **str** : string implicit

.. _function-strings_to_int8_string_implicit_bool:

.. das:function:: to_int8(value: string implicit; hex: bool = false) : int8

Convert string to int8. In case of error returns 0

:Arguments: * **value** : string implicit

            * **hex** : bool

.. _function-strings_to_uint8_string_implicit_bool:

.. das:function:: to_uint8(value: string implicit; hex: bool = false) : uint8

Convert string to uint8. In case of error returns 0u

:Arguments: * **value** : string implicit

            * **hex** : bool

.. _function-strings_to_int16_string_implicit_bool:

.. das:function:: to_int16(value: string implicit; hex: bool = false) : int16

Convert string to int16. In case of error returns 0

:Arguments: * **value** : string implicit

            * **hex** : bool

.. _function-strings_to_int_string_implicit_bool:

.. das:function:: to_int(value: string implicit; hex: bool = false) : int

Convert string to int. In case of error returns 0

:Arguments: * **value** : string implicit

            * **hex** : bool

.. _function-strings_to_uint_string_implicit_bool:

.. das:function:: to_uint(value: string implicit; hex: bool = false) : uint

Convert string to uint. In case of error returns 0u

:Arguments: * **value** : string implicit

            * **hex** : bool

.. _function-strings_to_int64_string_implicit_bool:

.. das:function:: to_int64(value: string implicit; hex: bool = false) : int64

Convert string to int64. In case of error returns 0l

:Arguments: * **value** : string implicit

            * **hex** : bool

.. _function-strings_to_uint64_string_implicit_bool:

.. das:function:: to_uint64(value: string implicit; hex: bool = false) : uint64

Convert string to uint64. In case of error returns 0ul

:Arguments: * **value** : string implicit

            * **hex** : bool

.. _function-strings_to_cpp_float_float:

.. das:function:: to_cpp_float(value: float) : string

Convert string to float using C++ fmt::format_to, while also recognizing FLT_MIN, FLT_MAX, etc.

:Arguments: * **value** : float

.. _function-strings_to_float_string_implicit:

.. das:function:: to_float(value: string implicit) : float

Convert string to float. In case of error returns 0.0

:Arguments: * **value** : string implicit

.. _function-strings_to_double_string_implicit:

.. das:function:: to_double(value: string implicit) : double

Convert string to double. In case of error returns 0.0lf

:Arguments: * **value** : string implicit

.. _function-strings_int8_string_implicit_ConversionResult_implicit_int_implicit_bool:

.. das:function:: int8(str: string implicit; result: ConversionResult& implicit; offset: int& implicit; hex: bool = false) : int8

Converts string to int8. In case of error panic.

:Arguments: * **str** : string implicit

            * **result** :  :ref:`ConversionResult <enum-strings-ConversionResult>` & implicit

            * **offset** : int& implicit

            * **hex** : bool

.. _function-strings_uint8_string_implicit_ConversionResult_implicit_int_implicit_bool:

.. das:function:: uint8(str: string implicit; result: ConversionResult& implicit; offset: int& implicit; hex: bool = false) : uint8

Convert string to uint8. In case of error panic.

:Arguments: * **str** : string implicit

            * **result** :  :ref:`ConversionResult <enum-strings-ConversionResult>` & implicit

            * **offset** : int& implicit

            * **hex** : bool

.. _function-strings_int16_string_implicit_ConversionResult_implicit_int_implicit_bool:

.. das:function:: int16(str: string implicit; result: ConversionResult& implicit; offset: int& implicit; hex: bool = false) : int16

Converts string to int16. In case of error panic.

:Arguments: * **str** : string implicit

            * **result** :  :ref:`ConversionResult <enum-strings-ConversionResult>` & implicit

            * **offset** : int& implicit

            * **hex** : bool

.. _function-strings_uint16_string_implicit_ConversionResult_implicit_int_implicit_bool:

.. das:function:: uint16(str: string implicit; result: ConversionResult& implicit; offset: int& implicit; hex: bool = false) : uint16

Convert string to uint16. In case of error panic.

:Arguments: * **str** : string implicit

            * **result** :  :ref:`ConversionResult <enum-strings-ConversionResult>` & implicit

            * **offset** : int& implicit

            * **hex** : bool

.. _function-strings_int_string_implicit_ConversionResult_implicit_int_implicit_bool:

.. das:function:: int(str: string implicit; result: ConversionResult& implicit; offset: int& implicit; hex: bool = false) : int

Converts string to integer. In case of error panic.

:Arguments: * **str** : string implicit

            * **result** :  :ref:`ConversionResult <enum-strings-ConversionResult>` & implicit

            * **offset** : int& implicit

            * **hex** : bool

.. _function-strings_uint_string_implicit_ConversionResult_implicit_int_implicit_bool:

.. das:function:: uint(str: string implicit; result: ConversionResult& implicit; offset: int& implicit; hex: bool = false) : uint

Convert string to uint. In case of error panic.

:Arguments: * **str** : string implicit

            * **result** :  :ref:`ConversionResult <enum-strings-ConversionResult>` & implicit

            * **offset** : int& implicit

            * **hex** : bool

.. _function-strings_int64_string_implicit_ConversionResult_implicit_int_implicit_bool:

.. das:function:: int64(str: string implicit; result: ConversionResult& implicit; offset: int& implicit; hex: bool = false) : int64

Converts string to int64. In case of error panic.

:Arguments: * **str** : string implicit

            * **result** :  :ref:`ConversionResult <enum-strings-ConversionResult>` & implicit

            * **offset** : int& implicit

            * **hex** : bool

.. _function-strings_uint64_string_implicit_ConversionResult_implicit_int_implicit_bool:

.. das:function:: uint64(str: string implicit; result: ConversionResult& implicit; offset: int& implicit; hex: bool = false) : uint64

Convert string to uint64. In case of error panic.

:Arguments: * **str** : string implicit

            * **result** :  :ref:`ConversionResult <enum-strings-ConversionResult>` & implicit

            * **offset** : int& implicit

            * **hex** : bool

.. _function-strings_float_string_implicit_ConversionResult_implicit_int_implicit:

.. das:function:: float(str: string implicit; result: ConversionResult& implicit; offset: int& implicit) : float

Converts string to float. In case of error panic.

:Arguments: * **str** : string implicit

            * **result** :  :ref:`ConversionResult <enum-strings-ConversionResult>` & implicit

            * **offset** : int& implicit

.. _function-strings_double_string_implicit_ConversionResult_implicit_int_implicit:

.. das:function:: double(str: string implicit; result: ConversionResult& implicit; offset: int& implicit) : double

Converts string to double. In case of error panic.

:Arguments: * **str** : string implicit

            * **result** :  :ref:`ConversionResult <enum-strings-ConversionResult>` & implicit

            * **offset** : int& implicit

+++++++++++++++
String as array
+++++++++++++++

  *  :ref:`peek_data (str: string implicit; block: block\<(array\<uint8\>#):void\>) <function-strings_peek_data_string_implicit_block_ls_array_ls_uint8_gr__hh__c_void_gr_>` 
  *  :ref:`modify_data (str: string implicit; block: block\<(array\<uint8\>#):void\>) : string <function-strings_modify_data_string_implicit_block_ls_array_ls_uint8_gr__hh__c_void_gr_>` 

.. _function-strings_peek_data_string_implicit_block_ls_array_ls_uint8_gr__hh__c_void_gr_:

.. das:function:: peek_data(str: string implicit; block: block<(array<uint8>#):void>)

Passes temporary array which is mapped to the string data to a block as read-only.

:Arguments: * **str** : string implicit

            * **block** : block<(array<uint8>#):void> implicit

.. _function-strings_modify_data_string_implicit_block_ls_array_ls_uint8_gr__hh__c_void_gr_:

.. das:function:: modify_data(str: string implicit; block: block<(array<uint8>#):void>) : string

Passes temporary array which is mapped to the string data to a block for both reading and writing.

:Arguments: * **str** : string implicit

            * **block** : block<(array<uint8>#):void> implicit

+++++++++++++++++++++++++++
Low level memory allocation
+++++++++++++++++++++++++++

  *  :ref:`delete_string (str: string& implicit) : bool <function-strings_delete_string_string_implicit>` 
  *  :ref:`reserve_string_buffer (str: string implicit; length: int) : string <function-strings_reserve_string_buffer_string_implicit_int>` 

.. _function-strings_delete_string_string_implicit:

.. das:function:: delete_string(str: string& implicit) : bool

.. warning:: 
  This is unsafe operation.

Removes string from the string heap. This is unsafe because it will free the memory and all dangling strings will be broken.

:Arguments: * **str** : string& implicit

.. _function-strings_reserve_string_buffer_string_implicit_int:

.. das:function:: reserve_string_buffer(str: string implicit; length: int) : string

Allocate copy of the string data on the heap.

:Arguments: * **str** : string implicit

            * **length** : int


