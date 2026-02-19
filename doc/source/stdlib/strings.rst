
.. _stdlib_strings:

===========================
String manipulation library
===========================

.. das:module:: strings

The STRINGS module implements string formatting, conversion, searching, and modification
routines. It provides functions for building strings (``build_string``), parsing
(``to_int``, ``to_float``), character classification (``is_alpha``, ``is_number``),
and low-level string manipulation.

All functions and symbols are in "strings" module, use require to get access to it.

.. code-block:: das

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

  *  :ref:`is_char_in_set (Character: int; Charset: uint const[8]) : bool <function-strings_is_char_in_set_int_uint_const_lb_8_rb_>`
  *  :ref:`set_element (Character: int; Charset: uint const[8]) : int <function-strings_set_element_int_uint_const_lb_8_rb_>`
  *  :ref:`set_total (Charset: uint const[8]) : uint <function-strings_set_total_uint_const_lb_8_rb_>`

.. _function-strings_is_char_in_set_int_uint_const_lb_8_rb_:

.. das:function:: is_char_in_set(Character: int; Charset: uint const[8]) : bool

Returns true if the character given by its integer code is present in the 256-bit character set represented as a uint[8] array.


:Arguments: * **Character** : int

            * **Charset** : uint[8] implicit

.. _function-strings_set_element_int_uint_const_lb_8_rb_:

.. das:function:: set_element(Character: int; Charset: uint const[8]) : int

Returns the character code at the given element index within the 256-bit character set represented as a uint[8] array.


:Arguments: * **Character** : int

            * **Charset** : uint[8] implicit

.. _function-strings_set_total_uint_const_lb_8_rb_:

.. das:function:: set_total(Charset: uint const[8]) : uint

Returns the total number of characters present (bits set) in the 256-bit character set represented as a uint[8] array.


:Arguments: * **Charset** : uint[8] implicit


++++++++++++++++
Character groups
++++++++++++++++

  *  :ref:`is_alnum (Character: int) : bool <function-strings_is_alnum_int>`
  *  :ref:`is_alpha (Character: int) : bool <function-strings_is_alpha_int>`
  *  :ref:`is_hex (Character: int) : bool <function-strings_is_hex_int>`
  *  :ref:`is_new_line (Character: int) : bool <function-strings_is_new_line_int>`
  *  :ref:`is_number (Character: int) : bool <function-strings_is_number_int>`
  *  :ref:`is_tab_or_space (Character: int) : bool <function-strings_is_tab_or_space_int>`
  *  :ref:`is_white_space (Character: int) : bool <function-strings_is_white_space_int>`

.. _function-strings_is_alnum_int:

.. das:function:: is_alnum(Character: int) : bool

Returns true if the integer character code represents an alphanumeric ASCII character [A-Za-z0-9].


:Arguments: * **Character** : int

.. _function-strings_is_alpha_int:

.. das:function:: is_alpha(Character: int) : bool

Returns true if the integer character code represents an alphabetic ASCII character [A-Za-z].


:Arguments: * **Character** : int

.. _function-strings_is_hex_int:

.. das:function:: is_hex(Character: int) : bool

Returns true if the integer character code represents a hexadecimal digit [0-9A-Fa-f].


:Arguments: * **Character** : int

.. _function-strings_is_new_line_int:

.. das:function:: is_new_line(Character: int) : bool

Returns true if the integer character code is a newline character (\\n or \\r).


:Arguments: * **Character** : int

.. _function-strings_is_number_int:

.. das:function:: is_number(Character: int) : bool

Returns true if the integer character code represents a decimal digit [0-9].


:Arguments: * **Character** : int

.. _function-strings_is_tab_or_space_int:

.. das:function:: is_tab_or_space(Character: int) : bool

Returns true if the integer character code is a tab or space character.


:Arguments: * **Character** : int

.. _function-strings_is_white_space_int:

.. das:function:: is_white_space(Character: int) : bool

Returns true if the integer character code is a whitespace character (space, tab, newline, carriage return, etc.).


:Arguments: * **Character** : int


++++++++++++++++++
Character by index
++++++++++++++++++

  *  :ref:`character_at (str: string; idx: int) : int <function-strings_character_at_string_int>`
  *  :ref:`character_uat (str: string; idx: int) : int <function-strings_character_uat_string_int>`

.. _function-strings_character_at_string_int:

.. das:function:: character_at(str: string; idx: int) : int

Returns the integer character code of string `str` at the given index `idx`, with bounds checking.


:Arguments: * **str** : string implicit

            * **idx** : int

.. _function-strings_character_uat_string_int:

.. das:function:: character_uat(str: string; idx: int) : int

.. warning::
  This is unsafe operation.

Returns the integer character code of string `str` at the given index `idx` without performing bounds checking (unsafe).


:Arguments: * **str** : string implicit

            * **idx** : int


+++++++++++++++++
String properties
+++++++++++++++++

  *  :ref:`ends_with (str: das_string; cmp: string) : bool <function-strings_ends_with_das_string_string>`
  *  :ref:`ends_with (str: string; cmp: string) : bool <function-strings_ends_with_string_string>`
  *  :ref:`length (str: string) : int <function-strings_length_string>`
  *  :ref:`length (str: das_string) : int <function-strings_length_das_string>`
  *  :ref:`starts_with (str: string; cmp: string; cmpLen: uint) : bool <function-strings_starts_with_string_string_uint>`
  *  :ref:`starts_with (str: string; cmp: string) : bool <function-strings_starts_with_string_string>`
  *  :ref:`starts_with (str: string; offset: int; cmp: string) : bool <function-strings_starts_with_string_int_string>`
  *  :ref:`starts_with (str: string; offset: int; cmp: string; cmpLen: uint) : bool <function-strings_starts_with_string_int_string_uint>`


ends_with
^^^^^^^^^

.. _function-strings_ends_with_das_string_string:

.. das:function:: ends_with(str: das_string; cmp: string) : bool

Returns true if the string `str` ends with the substring `cmp`, false otherwise.


:Arguments: * **str** :  :ref:`das_string <handle-builtin-das_string>` implicit

            * **cmp** : string implicit

.. _function-strings_ends_with_string_string:

.. das:function:: ends_with(str: string; cmp: string) : bool

----


length
^^^^^^

.. _function-strings_length_string:

.. das:function:: length(str: string) : int

Returns the length of the string or das_string in characters as an int.


:Arguments: * **str** : string implicit

.. _function-strings_length_das_string:

.. das:function:: length(str: das_string) : int

----


starts_with
^^^^^^^^^^^

.. _function-strings_starts_with_string_string_uint:

.. das:function:: starts_with(str: string; cmp: string; cmpLen: uint) : bool

Returns true if the beginning of string `str` matches the string `cmp`, with optional `offset` and `cmpLen` parameters to control the comparison start position and length.


:Arguments: * **str** : string implicit

            * **cmp** : string implicit

            * **cmpLen** : uint

.. _function-strings_starts_with_string_string:

.. das:function:: starts_with(str: string; cmp: string) : bool

.. _function-strings_starts_with_string_int_string:

.. das:function:: starts_with(str: string; offset: int; cmp: string) : bool

.. _function-strings_starts_with_string_int_string_uint:

.. das:function:: starts_with(str: string; offset: int; cmp: string; cmpLen: uint) : bool


++++++++++++++
String builder
++++++++++++++

  *  :ref:`build_hash (block: block\<(StringBuilderWriter):void\>) : uint64 <function-strings_build_hash_block_ls_StringBuilderWriter_c_void_gr_>`
  *  :ref:`build_string (block: block\<(StringBuilderWriter):void\>) : string <function-strings_build_string_block_ls_StringBuilderWriter_c_void_gr_>`
  *  :ref:`format (format: string; value: int64) : string <function-strings_format_string_int64>`
  *  :ref:`format (format: string; value: double) : string <function-strings_format_string_double>`
  *  :ref:`format (writer: StringBuilderWriter; format: string; value: int) : StringBuilderWriter& <function-strings_format_StringBuilderWriter_string_int>`
  *  :ref:`format (writer: StringBuilderWriter; format: string; value: uint) : StringBuilderWriter& <function-strings_format_StringBuilderWriter_string_uint>`
  *  :ref:`format (writer: StringBuilderWriter; format: string; value: int64) : StringBuilderWriter& <function-strings_format_StringBuilderWriter_string_int64>`
  *  :ref:`format (format: string; value: uint64) : string <function-strings_format_string_uint64>`
  *  :ref:`format (writer: StringBuilderWriter; format: string; value: float) : StringBuilderWriter& <function-strings_format_StringBuilderWriter_string_float>`
  *  :ref:`format (writer: StringBuilderWriter; format: string; value: uint64) : StringBuilderWriter& <function-strings_format_StringBuilderWriter_string_uint64>`
  *  :ref:`format (writer: StringBuilderWriter; format: string; value: double) : StringBuilderWriter& <function-strings_format_StringBuilderWriter_string_double>`
  *  :ref:`format (format: string; value: int) : string <function-strings_format_string_int>`
  *  :ref:`format (format: string; value: uint) : string <function-strings_format_string_uint>`
  *  :ref:`format (format: string; value: float) : string <function-strings_format_string_float>`
  *  :ref:`write (writer: StringBuilderWriter; anything: any) : StringBuilderWriter& <function-strings_write_StringBuilderWriter_any>`
  *  :ref:`write_char (writer: StringBuilderWriter; ch: int) : StringBuilderWriter& <function-strings_write_char_StringBuilderWriter_int>`
  *  :ref:`write_chars (writer: StringBuilderWriter; ch: int; count: int) : StringBuilderWriter& <function-strings_write_chars_StringBuilderWriter_int_int>`
  *  :ref:`write_escape_string (writer: StringBuilderWriter; str: string) : StringBuilderWriter& <function-strings_write_escape_string_StringBuilderWriter_string>`

.. _function-strings_build_hash_block_ls_StringBuilderWriter_c_void_gr_:

.. das:function:: build_hash(block: block<(StringBuilderWriter):void>) : uint64

Computes a uint64 hash by streaming writes through a StringBuilderWriter passed to `block`, without allocating the full concatenated string.


:Arguments: * **block** : block<( :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`):void> implicit

.. _function-strings_build_string_block_ls_StringBuilderWriter_c_void_gr_:

.. das:function:: build_string(block: block<(StringBuilderWriter):void>) : string

Creates a StringBuilderWriter, passes it to `block` for writing, and returns the accumulated output as a string.


:Arguments: * **block** : block<( :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`):void> implicit


format
^^^^^^

.. _function-strings_format_string_int64:

.. das:function:: format(format: string; value: int64) : string

.. warning::
  This function is deprecated.

Formats a numeric value of type T using a C printf-style format string, either appending to a StringBuilderWriter and returning a reference to it, or returning the formatted result as a new string.


:Arguments: * **format** : string implicit

            * **value** : int64

.. _function-strings_format_string_double:

.. das:function:: format(format: string; value: double) : string

.. _function-strings_format_StringBuilderWriter_string_int:

.. das:function:: format(writer: StringBuilderWriter; format: string; value: int) : StringBuilderWriter&

.. _function-strings_format_StringBuilderWriter_string_uint:

.. das:function:: format(writer: StringBuilderWriter; format: string; value: uint) : StringBuilderWriter&

.. _function-strings_format_StringBuilderWriter_string_int64:

.. das:function:: format(writer: StringBuilderWriter; format: string; value: int64) : StringBuilderWriter&

.. _function-strings_format_string_uint64:

.. das:function:: format(format: string; value: uint64) : string

.. _function-strings_format_StringBuilderWriter_string_float:

.. das:function:: format(writer: StringBuilderWriter; format: string; value: float) : StringBuilderWriter&

.. _function-strings_format_StringBuilderWriter_string_uint64:

.. das:function:: format(writer: StringBuilderWriter; format: string; value: uint64) : StringBuilderWriter&

.. _function-strings_format_StringBuilderWriter_string_double:

.. das:function:: format(writer: StringBuilderWriter; format: string; value: double) : StringBuilderWriter&

.. _function-strings_format_string_int:

.. das:function:: format(format: string; value: int) : string

.. _function-strings_format_string_uint:

.. das:function:: format(format: string; value: uint) : string

.. _function-strings_format_string_float:

.. das:function:: format(format: string; value: float) : string

----

.. _function-strings_write_StringBuilderWriter_any:

.. das:function:: write(writer: StringBuilderWriter; anything: any) : StringBuilderWriter&

Writes the textual representation of any value into the StringBuilderWriter and returns a reference to the writer for chaining.


:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`

            * **anything** : any

.. _function-strings_write_char_StringBuilderWriter_int:

.. das:function:: write_char(writer: StringBuilderWriter; ch: int) : StringBuilderWriter&

Writes a single character specified by its integer code `ch` into the StringBuilderWriter and returns a reference to the writer.


:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>` implicit

            * **ch** : int

.. _function-strings_write_chars_StringBuilderWriter_int_int:

.. das:function:: write_chars(writer: StringBuilderWriter; ch: int; count: int) : StringBuilderWriter&

Writes the character specified by integer code `ch` repeated `count` times into the StringBuilderWriter and returns a reference to the writer.


:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>` implicit

            * **ch** : int

            * **count** : int

.. _function-strings_write_escape_string_StringBuilderWriter_string:

.. das:function:: write_escape_string(writer: StringBuilderWriter; str: string) : StringBuilderWriter&

Writes the escaped form of string `str` (with special characters converted to escape sequences) into the StringBuilderWriter and returns a reference to the writer.


:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>` implicit

            * **str** : string implicit


++++++++++++++++++++++++
das::string manipulation
++++++++++++++++++++++++

  *  :ref:`append (str: das_string; ch: int) <function-strings_append_das_string_int>`
  *  :ref:`resize (str: das_string; new_length: int) <function-strings_resize_das_string_int>`

.. _function-strings_append_das_string_int:

.. das:function:: append(str: das_string; ch: int)

Appends a single character specified by its integer code `ch` to the mutable das_string `str`.


:Arguments: * **str** :  :ref:`das_string <handle-builtin-das_string>` implicit

            * **ch** : int

.. _function-strings_resize_das_string_int:

.. das:function:: resize(str: das_string; new_length: int)

Resizes the mutable das_string `str` in place to `new_length` characters.


:Arguments: * **str** :  :ref:`das_string <handle-builtin-das_string>` implicit

            * **new_length** : int


++++++++++++++++++++
String modifications
++++++++++++++++++++

  *  :ref:`chop (str: string; start: int; length: int) : string <function-strings_chop_string_int_int>`
  *  :ref:`escape (str: string) : string <function-strings_escape_string>`
  *  :ref:`ltrim (str: string) : string <function-strings_ltrim_string>`
  *  :ref:`repeat (str: string; count: int) : string <function-strings_repeat_string_int>`
  *  :ref:`replace (str: string; toSearch: string; replace: string) : string <function-strings_replace_string_string_string>`
  *  :ref:`reverse (str: string) : string <function-strings_reverse_string>`
  *  :ref:`rtrim (str: string; chars: string) : string <function-strings_rtrim_string_string>`
  *  :ref:`rtrim (str: string) : string <function-strings_rtrim_string>`
  *  :ref:`safe_unescape (str: string) : string <function-strings_safe_unescape_string>`
  *  :ref:`slice (str: string; start: int) : string <function-strings_slice_string_int>`
  *  :ref:`slice (str: string; start: int; end: int) : string <function-strings_slice_string_int_int>`
  *  :ref:`strip (str: string) : string <function-strings_strip_string>`
  *  :ref:`strip_left (str: string) : string <function-strings_strip_left_string>`
  *  :ref:`strip_right (str: string) : string <function-strings_strip_right_string>`
  *  :ref:`to_lower (str: string) : string <function-strings_to_lower_string>`
  *  :ref:`to_lower_in_place (str: string) : string <function-strings_to_lower_in_place_string>`
  *  :ref:`to_upper (str: string) : string <function-strings_to_upper_string>`
  *  :ref:`to_upper_in_place (str: string) : string <function-strings_to_upper_in_place_string>`
  *  :ref:`trim (str: string) : string <function-strings_trim_string>`
  *  :ref:`unescape (str: string) : string <function-strings_unescape_string>`

.. _function-strings_chop_string_int_int:

.. das:function:: chop(str: string; start: int; length: int) : string

Returns a substring of `str` beginning at index `start` with the specified `length`.


:Arguments: * **str** : string implicit

            * **start** : int

            * **length** : int

.. _function-strings_escape_string:

.. das:function:: escape(str: string) : string

Returns a new string with special characters replaced by their printable escape sequences (e.g. newline becomes \\n).


:Arguments: * **str** : string implicit

.. _function-strings_ltrim_string:

.. das:function:: ltrim(str: string) : string

Returns a new string with leading whitespace characters removed from `str`.


:Arguments: * **str** : string implicit

.. _function-strings_repeat_string_int:

.. das:function:: repeat(str: string; count: int) : string

Returns a new string formed by concatenating `str` repeated `count` times.


:Arguments: * **str** : string implicit

            * **count** : int

.. _function-strings_replace_string_string_string:

.. das:function:: replace(str: string; toSearch: string; replace: string) : string

Returns a new string with all occurrences of substring `toSearch` in `str` replaced by the substring `replace`.


:Arguments: * **str** : string implicit

            * **toSearch** : string implicit

            * **replace** : string implicit

.. _function-strings_reverse_string:

.. das:function:: reverse(str: string) : string

Returns a new string with the characters of `str` in reverse order.


:Arguments: * **str** : string implicit


rtrim
^^^^^

.. _function-strings_rtrim_string_string:

.. das:function:: rtrim(str: string; chars: string) : string

Returns a new string with trailing whitespace removed from `str`, or with trailing characters from the specified `chars` set removed.


:Arguments: * **str** : string implicit

            * **chars** : string implicit

.. _function-strings_rtrim_string:

.. das:function:: rtrim(str: string) : string

----

.. _function-strings_safe_unescape_string:

.. das:function:: safe_unescape(str: string) : string

Unescapes a string by converting printable escape sequences back to their original characters (e.g. \\n becomes a newline), skipping invalid sequences instead of failing.


:Arguments: * **str** : string implicit


slice
^^^^^

.. _function-strings_slice_string_int:

.. das:function:: slice(str: string; start: int) : string

Returns a substring of `str` from index `start` to optional `end` (exclusive), where negative indices count from the end of the string.


:Arguments: * **str** : string implicit

            * **start** : int

.. _function-strings_slice_string_int_int:

.. das:function:: slice(str: string; start: int; end: int) : string

----

.. _function-strings_strip_string:

.. das:function:: strip(str: string) : string

Returns a new string with all leading and trailing whitespace characters removed from `str`.


:Arguments: * **str** : string implicit

.. _function-strings_strip_left_string:

.. das:function:: strip_left(str: string) : string

Returns a new string with all leading whitespace characters removed from `str`.


:Arguments: * **str** : string implicit

.. _function-strings_strip_right_string:

.. das:function:: strip_right(str: string) : string

Returns a new string with all trailing whitespace characters removed from `str`.


:Arguments: * **str** : string implicit

.. _function-strings_to_lower_string:

.. das:function:: to_lower(str: string) : string

Returns a new string with all characters of `str` converted to lower case.


:Arguments: * **str** : string implicit

.. _function-strings_to_lower_in_place_string:

.. das:function:: to_lower_in_place(str: string) : string

.. warning::
  This is unsafe operation.

Converts all characters of `str` to lower case in place and returns the modified string.


:Arguments: * **str** : string implicit

.. _function-strings_to_upper_string:

.. das:function:: to_upper(str: string) : string

Returns a new string with all characters of `str` converted to upper case.


:Arguments: * **str** : string implicit

.. _function-strings_to_upper_in_place_string:

.. das:function:: to_upper_in_place(str: string) : string

.. warning::
  This is unsafe operation.

Converts all characters of `str` to upper case in place and returns the modified string.


:Arguments: * **str** : string implicit

.. _function-strings_trim_string:

.. das:function:: trim(str: string) : string

Returns a new string with both leading and trailing whitespace characters removed from `str`.


:Arguments: * **str** : string implicit

.. _function-strings_unescape_string:

.. das:function:: unescape(str: string) : string

Returns a new string with printable escape sequences converted back to their original characters (e.g. \\n becomes a newline).


:Arguments: * **str** : string implicit


+++++++++++++++++
Search substrings
+++++++++++++++++

  *  :ref:`find (str: string; substr: string) : int <function-strings_find_string_string>`
  *  :ref:`find (str: string; substr: string; start: int) : int <function-strings_find_string_string_int>`
  *  :ref:`find (str: string; substr: int; start: int) : int <function-strings_find_string_int_int>`
  *  :ref:`find (str: string; substr: int) : int <function-strings_find_string_int>`


find
^^^^

.. _function-strings_find_string_string:

.. das:function:: find(str: string; substr: string) : int

Returns the first index at which `substr` (string or character code) occurs in `str`, optionally searching from `start`, or -1 if not found.


:Arguments: * **str** : string implicit

            * **substr** : string implicit

.. _function-strings_find_string_string_int:

.. das:function:: find(str: string; substr: string; start: int) : int

.. _function-strings_find_string_int_int:

.. das:function:: find(str: string; substr: int; start: int) : int

.. _function-strings_find_string_int:

.. das:function:: find(str: string; substr: int) : int


+++++++++++++++++
String comparison
+++++++++++++++++

  *  :ref:`compare_ignore_case (a: string; b: string) : int <function-strings_compare_ignore_case_string_string>`

.. _function-strings_compare_ignore_case_string_string:

.. das:function:: compare_ignore_case(a: string; b: string) : int

Performs case-insensitive string comparison. Returns 0 if strings are equal, a negative value if `a` is less than `b`, or a positive value if `a` is greater than `b`.


:Arguments: * **a** : string implicit

            * **b** : string implicit


++++++++++++++++++++++++++
String conversion routines
++++++++++++++++++++++++++

  *  :ref:`double (str: string; result: ConversionResult&; offset: int&) : double <function-strings_double_string_ConversionResult_ref__int_ref_>`
  *  :ref:`double (str: string) : double <function-strings_double_string>`
  *  :ref:`float (str: string) : float <function-strings_float_string>`
  *  :ref:`float (str: string; result: ConversionResult&; offset: int&) : float <function-strings_float_string_ConversionResult_ref__int_ref_>`
  *  :ref:`fmt (writer: StringBuilderWriter; format: string; value: int64) : StringBuilderWriter& <function-strings_fmt_StringBuilderWriter_string_int64>`
  *  :ref:`fmt (writer: StringBuilderWriter; format: string; value: uint) : StringBuilderWriter& <function-strings_fmt_StringBuilderWriter_string_uint>`
  *  :ref:`fmt (writer: StringBuilderWriter; format: string; value: uint64) : StringBuilderWriter& <function-strings_fmt_StringBuilderWriter_string_uint64>`
  *  :ref:`fmt (writer: StringBuilderWriter; format: string; value: uint16) : StringBuilderWriter& <function-strings_fmt_StringBuilderWriter_string_uint16>`
  *  :ref:`fmt (writer: StringBuilderWriter; format: string; value: uint8) : StringBuilderWriter& <function-strings_fmt_StringBuilderWriter_string_uint8>`
  *  :ref:`fmt (writer: StringBuilderWriter; format: string; value: int8) : StringBuilderWriter& <function-strings_fmt_StringBuilderWriter_string_int8>`
  *  :ref:`fmt (writer: StringBuilderWriter; format: string; value: int16) : StringBuilderWriter& <function-strings_fmt_StringBuilderWriter_string_int16>`
  *  :ref:`fmt (writer: StringBuilderWriter; format: string; value: int) : StringBuilderWriter& <function-strings_fmt_StringBuilderWriter_string_int>`
  *  :ref:`fmt (writer: StringBuilderWriter; format: string; value: double) : StringBuilderWriter& <function-strings_fmt_StringBuilderWriter_string_double>`
  *  :ref:`fmt (writer: StringBuilderWriter; format: string; value: float) : StringBuilderWriter& <function-strings_fmt_StringBuilderWriter_string_float>`
  *  :ref:`int (str: string) : int <function-strings_int_string>`
  *  :ref:`int (str: string; result: ConversionResult&; offset: int&; hex: bool = false) : int <function-strings_int_string_ConversionResult_ref__int_ref__bool>`
  *  :ref:`int16 (str: string; result: ConversionResult&; offset: int&; hex: bool = false) : int16 <function-strings_int16_string_ConversionResult_ref__int_ref__bool>`
  *  :ref:`int16 (str: string) : int16 <function-strings_int16_string>`
  *  :ref:`int64 (str: string) : int64 <function-strings_int64_string>`
  *  :ref:`int64 (str: string; result: ConversionResult&; offset: int&; hex: bool = false) : int64 <function-strings_int64_string_ConversionResult_ref__int_ref__bool>`
  *  :ref:`int8 (str: string) : int8 <function-strings_int8_string>`
  *  :ref:`int8 (str: string; result: ConversionResult&; offset: int&; hex: bool = false) : int8 <function-strings_int8_string_ConversionResult_ref__int_ref__bool>`
  *  :ref:`string (bytes: array\<uint8\>) : string <function-strings_string_array_ls_uint8_gr_>`
  *  :ref:`to_char (char: int) : string <function-strings_to_char_int>`
  *  :ref:`to_cpp_float (value: float) : string <function-strings_to_cpp_float_float>`
  *  :ref:`to_double (value: string) : double <function-strings_to_double_string>`
  *  :ref:`to_float (value: string) : float <function-strings_to_float_string>`
  *  :ref:`to_int (value: string; hex: bool = false) : int <function-strings_to_int_string_bool>`
  *  :ref:`to_int16 (value: string; hex: bool = false) : int16 <function-strings_to_int16_string_bool>`
  *  :ref:`to_int64 (value: string; hex: bool = false) : int64 <function-strings_to_int64_string_bool>`
  *  :ref:`to_int8 (value: string; hex: bool = false) : int8 <function-strings_to_int8_string_bool>`
  *  :ref:`to_uint (value: string; hex: bool = false) : uint <function-strings_to_uint_string_bool>`
  *  :ref:`to_uint16 (value: string; hex: bool = false) : uint16 <function-strings_to_uint16_string_bool>`
  *  :ref:`to_uint64 (value: string; hex: bool = false) : uint64 <function-strings_to_uint64_string_bool>`
  *  :ref:`to_uint8 (value: string; hex: bool = false) : uint8 <function-strings_to_uint8_string_bool>`
  *  :ref:`uint (str: string) : uint <function-strings_uint_string>`
  *  :ref:`uint (str: string; result: ConversionResult&; offset: int&; hex: bool = false) : uint <function-strings_uint_string_ConversionResult_ref__int_ref__bool>`
  *  :ref:`uint16 (str: string) : uint16 <function-strings_uint16_string>`
  *  :ref:`uint16 (str: string; result: ConversionResult&; offset: int&; hex: bool = false) : uint16 <function-strings_uint16_string_ConversionResult_ref__int_ref__bool>`
  *  :ref:`uint64 (str: string) : uint64 <function-strings_uint64_string>`
  *  :ref:`uint64 (str: string; result: ConversionResult&; offset: int&; hex: bool = false) : uint64 <function-strings_uint64_string_ConversionResult_ref__int_ref__bool>`
  *  :ref:`uint8 (str: string; result: ConversionResult&; offset: int&; hex: bool = false) : uint8 <function-strings_uint8_string_ConversionResult_ref__int_ref__bool>`
  *  :ref:`uint8 (str: string) : uint8 <function-strings_uint8_string>`


double
^^^^^^

.. _function-strings_double_string_ConversionResult_ref__int_ref_:

.. das:function:: double(str: string; result: ConversionResult&; offset: int&) : double

Converts a string to a double value, panicking on failure; an overload accepts `result` and `offset` output parameters to report the ConversionResult status and parsed position instead of panicking.


:Arguments: * **str** : string implicit

            * **result** :  :ref:`ConversionResult <enum-strings-ConversionResult>`\ & implicit

            * **offset** : int\ & implicit

.. _function-strings_double_string:

.. das:function:: double(str: string) : double

----


float
^^^^^

.. _function-strings_float_string:

.. das:function:: float(str: string) : float

Converts a string to a float value, panicking on failure; an overload accepts `result` and `offset` output parameters to report the ConversionResult status and parsed position instead of panicking.


:Arguments: * **str** : string implicit

.. _function-strings_float_string_ConversionResult_ref__int_ref_:

.. das:function:: float(str: string; result: ConversionResult&; offset: int&) : float

----


fmt
^^^

.. _function-strings_fmt_StringBuilderWriter_string_int64:

.. das:function:: fmt(writer: StringBuilderWriter; format: string; value: int64) : StringBuilderWriter&

Formats a numeric value of type T into the StringBuilderWriter using a libfmt/C++20 std::format format string and returns a reference to the writer.


:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>` implicit

            * **format** : string implicit

            * **value** : int64

.. _function-strings_fmt_StringBuilderWriter_string_uint:

.. das:function:: fmt(writer: StringBuilderWriter; format: string; value: uint) : StringBuilderWriter&

.. _function-strings_fmt_StringBuilderWriter_string_uint64:

.. das:function:: fmt(writer: StringBuilderWriter; format: string; value: uint64) : StringBuilderWriter&

.. _function-strings_fmt_StringBuilderWriter_string_uint16:

.. das:function:: fmt(writer: StringBuilderWriter; format: string; value: uint16) : StringBuilderWriter&

.. _function-strings_fmt_StringBuilderWriter_string_uint8:

.. das:function:: fmt(writer: StringBuilderWriter; format: string; value: uint8) : StringBuilderWriter&

.. _function-strings_fmt_StringBuilderWriter_string_int8:

.. das:function:: fmt(writer: StringBuilderWriter; format: string; value: int8) : StringBuilderWriter&

.. _function-strings_fmt_StringBuilderWriter_string_int16:

.. das:function:: fmt(writer: StringBuilderWriter; format: string; value: int16) : StringBuilderWriter&

.. _function-strings_fmt_StringBuilderWriter_string_int:

.. das:function:: fmt(writer: StringBuilderWriter; format: string; value: int) : StringBuilderWriter&

.. _function-strings_fmt_StringBuilderWriter_string_double:

.. das:function:: fmt(writer: StringBuilderWriter; format: string; value: double) : StringBuilderWriter&

.. _function-strings_fmt_StringBuilderWriter_string_float:

.. das:function:: fmt(writer: StringBuilderWriter; format: string; value: float) : StringBuilderWriter&

----


int
^^^

.. _function-strings_int_string:

.. das:function:: int(str: string) : int

Converts a string to an int, panicking on failure; an overload accepts `result`, `offset`, and optional `hex` flag to report the ConversionResult status and parsed position instead of panicking.


:Arguments: * **str** : string implicit

.. _function-strings_int_string_ConversionResult_ref__int_ref__bool:

.. das:function:: int(str: string; result: ConversionResult&; offset: int&; hex: bool = false) : int

----


int16
^^^^^

.. _function-strings_int16_string_ConversionResult_ref__int_ref__bool:

.. das:function:: int16(str: string; result: ConversionResult&; offset: int&; hex: bool = false) : int16

Converts a string to an int16, panicking on failure; an overload accepts `result`, `offset`, and optional `hex` flag to report the ConversionResult status and parsed position instead of panicking.


:Arguments: * **str** : string implicit

            * **result** :  :ref:`ConversionResult <enum-strings-ConversionResult>`\ & implicit

            * **offset** : int\ & implicit

            * **hex** : bool

.. _function-strings_int16_string:

.. das:function:: int16(str: string) : int16

----


int64
^^^^^

.. _function-strings_int64_string:

.. das:function:: int64(str: string) : int64

Converts a string to an int64, panicking on failure; an overload accepts `result`, `offset`, and optional `hex` flag to report the ConversionResult status and parsed position instead of panicking.


:Arguments: * **str** : string implicit

.. _function-strings_int64_string_ConversionResult_ref__int_ref__bool:

.. das:function:: int64(str: string; result: ConversionResult&; offset: int&; hex: bool = false) : int64

----


int8
^^^^

.. _function-strings_int8_string:

.. das:function:: int8(str: string) : int8

Converts a string to an int8, panicking on failure; an overload accepts `result`, `offset`, and optional `hex` flag to report the ConversionResult status and parsed position instead of panicking.


:Arguments: * **str** : string implicit

.. _function-strings_int8_string_ConversionResult_ref__int_ref__bool:

.. das:function:: int8(str: string; result: ConversionResult&; offset: int&; hex: bool = false) : int8

----

.. _function-strings_string_array_ls_uint8_gr_:

.. das:function:: string(bytes: array<uint8>) : string

Constructs and returns a new string from the contents of a uint8 byte array.


:Arguments: * **bytes** : array<uint8> implicit

.. _function-strings_to_char_int:

.. das:function:: to_char(char: int) : string

Converts an integer character code to a single-character string.


:Arguments: * **char** : int

.. _function-strings_to_cpp_float_float:

.. das:function:: to_cpp_float(value: float) : string

Converts a float value to its string representation using C++ fmt::format_to, correctly handling special constants like FLT_MIN and FLT_MAX.


:Arguments: * **value** : float

.. _function-strings_to_double_string:

.. das:function:: to_double(value: string) : double

Converts a string to a double value, returning 0.0lf if the conversion fails.


:Arguments: * **value** : string implicit

.. _function-strings_to_float_string:

.. das:function:: to_float(value: string) : float

Converts a string to a float value, returning 0.0 if the conversion fails.


:Arguments: * **value** : string implicit

.. _function-strings_to_int_string_bool:

.. das:function:: to_int(value: string; hex: bool = false) : int

Converts a string to an int value with optional hexadecimal parsing when `hex` is true, returning 0 if the conversion fails.


:Arguments: * **value** : string implicit

            * **hex** : bool

.. _function-strings_to_int16_string_bool:

.. das:function:: to_int16(value: string; hex: bool = false) : int16

Converts a string to an int16 value with optional hexadecimal parsing when `hex` is true, returning 0 if the conversion fails.


:Arguments: * **value** : string implicit

            * **hex** : bool

.. _function-strings_to_int64_string_bool:

.. das:function:: to_int64(value: string; hex: bool = false) : int64

Converts a string to an int64 value with optional hexadecimal parsing when `hex` is true, returning 0l if the conversion fails.


:Arguments: * **value** : string implicit

            * **hex** : bool

.. _function-strings_to_int8_string_bool:

.. das:function:: to_int8(value: string; hex: bool = false) : int8

Converts a string to an int8 value with optional hexadecimal parsing when `hex` is true, returning 0 if the conversion fails.


:Arguments: * **value** : string implicit

            * **hex** : bool

.. _function-strings_to_uint_string_bool:

.. das:function:: to_uint(value: string; hex: bool = false) : uint

Converts a string to a uint value with optional hexadecimal parsing when `hex` is true, returning 0u if the conversion fails.


:Arguments: * **value** : string implicit

            * **hex** : bool

.. _function-strings_to_uint16_string_bool:

.. das:function:: to_uint16(value: string; hex: bool = false) : uint16

Converts a string to a uint16 value. Returns 0 if conversion fails. When `hex` is true, parses the string as hexadecimal.


:Arguments: * **value** : string implicit

            * **hex** : bool

.. _function-strings_to_uint64_string_bool:

.. das:function:: to_uint64(value: string; hex: bool = false) : uint64

Converts a string to a uint64 value with optional hexadecimal parsing when `hex` is true, returning 0ul if the conversion fails.


:Arguments: * **value** : string implicit

            * **hex** : bool

.. _function-strings_to_uint8_string_bool:

.. das:function:: to_uint8(value: string; hex: bool = false) : uint8

Converts a string to a uint8 value with optional hexadecimal parsing when `hex` is true, returning 0u if the conversion fails.


:Arguments: * **value** : string implicit

            * **hex** : bool


uint
^^^^

.. _function-strings_uint_string:

.. das:function:: uint(str: string) : uint

Converts a string to a uint, panicking on failure; an overload accepts `result`, `offset`, and optional `hex` flag to report the ConversionResult status and parsed position instead of panicking.


:Arguments: * **str** : string implicit

.. _function-strings_uint_string_ConversionResult_ref__int_ref__bool:

.. das:function:: uint(str: string; result: ConversionResult&; offset: int&; hex: bool = false) : uint

----


uint16
^^^^^^

.. _function-strings_uint16_string:

.. das:function:: uint16(str: string) : uint16

Converts a string to a uint16, panicking on failure; an overload accepts `result`, `offset`, and optional `hex` flag to report the ConversionResult status and parsed position instead of panicking.


:Arguments: * **str** : string implicit

.. _function-strings_uint16_string_ConversionResult_ref__int_ref__bool:

.. das:function:: uint16(str: string; result: ConversionResult&; offset: int&; hex: bool = false) : uint16

----


uint64
^^^^^^

.. _function-strings_uint64_string:

.. das:function:: uint64(str: string) : uint64

Converts a string to a uint64, panicking on failure; an overload accepts `result`, `offset`, and optional `hex` flag to report the ConversionResult status and parsed position instead of panicking.


:Arguments: * **str** : string implicit

.. _function-strings_uint64_string_ConversionResult_ref__int_ref__bool:

.. das:function:: uint64(str: string; result: ConversionResult&; offset: int&; hex: bool = false) : uint64

----


uint8
^^^^^

.. _function-strings_uint8_string_ConversionResult_ref__int_ref__bool:

.. das:function:: uint8(str: string; result: ConversionResult&; offset: int&; hex: bool = false) : uint8

Converts a string to a uint8, panicking on failure; an overload accepts `result`, `offset`, and optional `hex` flag to report the ConversionResult status and parsed position instead of panicking.


:Arguments: * **str** : string implicit

            * **result** :  :ref:`ConversionResult <enum-strings-ConversionResult>`\ & implicit

            * **offset** : int\ & implicit

            * **hex** : bool

.. _function-strings_uint8_string:

.. das:function:: uint8(str: string) : uint8


+++++++++++++++
String as array
+++++++++++++++

  *  :ref:`modify_data (str: string; block: block\<(array\<uint8\>#):void\>) : string <function-strings_modify_data_string_block_ls_array_ls_uint8_gr__hh__c_void_gr_>`
  *  :ref:`peek_data (str: string; block: block\<(array\<uint8\>#):void\>) <function-strings_peek_data_string_block_ls_array_ls_uint8_gr__hh__c_void_gr_>`

.. _function-strings_modify_data_string_block_ls_array_ls_uint8_gr__hh__c_void_gr_:

.. das:function:: modify_data(str: string; block: block<(array<uint8>#):void>) : string

Maps the raw bytes of string `str` into a temporary uint8 array, passes it to `block` for in-place reading and writing, and returns the modified string.


:Arguments: * **str** : string implicit

            * **block** : block<(array<uint8>\ #):void> implicit

.. _function-strings_peek_data_string_block_ls_array_ls_uint8_gr__hh__c_void_gr_:

.. das:function:: peek_data(str: string; block: block<(array<uint8>#):void>)

Maps the raw bytes of string `str` into a temporary read-only uint8 array and passes it to `block` for inspection.


:Arguments: * **str** : string implicit

            * **block** : block<(array<uint8>\ #):void> implicit


+++++++++++++++++++++++++++
Low level memory allocation
+++++++++++++++++++++++++++

  *  :ref:`delete_string (str: string&) : bool <function-strings_delete_string_string_ref_>`
  *  :ref:`reserve_string_buffer (str: string; length: int) : string <function-strings_reserve_string_buffer_string_int>`

.. _function-strings_delete_string_string_ref_:

.. das:function:: delete_string(str: string&) : bool

.. warning::
  This is unsafe operation.

Frees the string `str` from the heap and clears the reference, returning true on success; unsafe because existing aliases become dangling pointers.


:Arguments: * **str** : string\ & implicit

.. _function-strings_reserve_string_buffer_string_int:

.. das:function:: reserve_string_buffer(str: string; length: int) : string

Allocates a copy of the string data on the heap with at least `length` bytes reserved and returns the new string.


:Arguments: * **str** : string implicit

            * **length** : int


