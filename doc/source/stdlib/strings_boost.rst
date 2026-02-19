
.. _stdlib_strings_boost:

=============================================
Boost package for string manipulation library
=============================================

.. das:module:: strings_boost

The STRINGS_BOOST module extends string handling with splitting, joining,
padding, character replacement, and edit distance computation.

All functions and symbols are in "strings_boost" module, use require to get access to it.

.. code-block:: das

    require daslib/strings_boost

Example:

.. code-block:: das

    require daslib/strings_boost

        [export]
        def main() {
            let parts = split("one,two,three", ",")
            print("split: {parts}\n")
            print("join: {join(parts, " | ")}\n")
            print("[{wide("hello", 10)}]\n")
            print("distance: {levenshtein_distance("kitten", "sitting")}\n")
        }
        // output:
        // split: [[ one; two; three]]
        // join: one | two | three
        // [hello     ]
        // distance: 3



++++++++++++++
Split and join
++++++++++++++

  *  :ref:`join (it: auto; separator: string) : auto <function-strings_boost_join_auto_string_0x1d>`
  *  :ref:`join (iterable: array\<auto(TT)\>; separator: string; blk: block\<(var writer:StringBuilderWriter;elem:TT):void\>) : string <function-strings_boost_join_array_ls_autoTT_gr__string_block_ls_var_writer_c_StringBuilderWriter;elem_c_TT_c_void_gr_>`
  *  :ref:`join (var it: iterator\<auto(TT)\>; separator: string) : auto <function-strings_boost_join_iterator_ls_autoTT_gr__string>`
  *  :ref:`join (var iterable: iterator\<auto(TT)\>; separator: string; blk: block\<(var writer:StringBuilderWriter;elem:TT):void\>) : string <function-strings_boost_join_iterator_ls_autoTT_gr__string_block_ls_var_writer_c_StringBuilderWriter;elem_c_TT_c_void_gr_>`
  *  :ref:`join (iterable: auto(TT)[]; separator: string; blk: block\<(var writer:StringBuilderWriter;elem:TT):void\>) : string <function-strings_boost_join_autoTT_lb__rb__string_block_ls_var_writer_c_StringBuilderWriter;elem_c_TT_c_void_gr__0x64>`
  *  :ref:`split (text: string; delim: string) : array\<string\> <function-strings_boost_split_string_string>`
  *  :ref:`split (text: string; delim: string; blk: block\<(arg:array\<string\>#):auto\>) : auto <function-strings_boost_split_string_string_block_ls_arg_c_array_ls_string_gr__hh__c_auto_gr_>`
  *  :ref:`split_by_chars (text: string; delim: string) : array\<string\> <function-strings_boost_split_by_chars_string_string>`
  *  :ref:`split_by_chars (text: string; delim: string; blk: block\<(arg:array\<string\>#):auto\>) : auto <function-strings_boost_split_by_chars_string_string_block_ls_arg_c_array_ls_string_gr__hh__c_auto_gr_>`


join
^^^^

.. _function-strings_boost_join_auto_string_0x1d:

.. das:function:: join(it: auto; separator: string) : auto

Joins the elements of an iterable into a single string using the specified separator.


:Arguments: * **it** : auto

            * **separator** : string implicit

.. _function-strings_boost_join_array_ls_autoTT_gr__string_block_ls_var_writer_c_StringBuilderWriter;elem_c_TT_c_void_gr_:

.. das:function:: join(iterable: array<auto(TT)>; separator: string; blk: block<(var writer:StringBuilderWriter;elem:TT):void>) : string

.. _function-strings_boost_join_iterator_ls_autoTT_gr__string:

.. das:function:: join(it: iterator<auto(TT)>; separator: string) : auto

.. _function-strings_boost_join_iterator_ls_autoTT_gr__string_block_ls_var_writer_c_StringBuilderWriter;elem_c_TT_c_void_gr_:

.. das:function:: join(iterable: iterator<auto(TT)>; separator: string; blk: block<(var writer:StringBuilderWriter;elem:TT):void>) : string

.. _function-strings_boost_join_autoTT_lb__rb__string_block_ls_var_writer_c_StringBuilderWriter;elem_c_TT_c_void_gr__0x64:

.. das:function:: join(iterable: auto(TT)[]; separator: string; blk: block<(var writer:StringBuilderWriter;elem:TT):void>) : string

----


split
^^^^^

.. _function-strings_boost_split_string_string:

.. das:function:: split(text: string; delim: string) : array<string>

Splits a string by the specified delimiter string and returns an array of substrings.


:Arguments: * **text** : string implicit

            * **delim** : string implicit

.. _function-strings_boost_split_string_string_block_ls_arg_c_array_ls_string_gr__hh__c_auto_gr_:

.. das:function:: split(text: string; delim: string; blk: block<(arg:array<string>#):auto>) : auto

----


split_by_chars
^^^^^^^^^^^^^^

.. _function-strings_boost_split_by_chars_string_string:

.. das:function:: split_by_chars(text: string; delim: string) : array<string>

Splits a string by the specified delimiter characters and returns an array of substrings.


:Arguments: * **text** : string implicit

            * **delim** : string implicit

.. _function-strings_boost_split_by_chars_string_string_block_ls_arg_c_array_ls_string_gr__hh__c_auto_gr_:

.. das:function:: split_by_chars(text: string; delim: string; blk: block<(arg:array<string>#):auto>) : auto


++++++++++
Formatting
++++++++++

  *  :ref:`capitalize (str: string) : string <function-strings_boost_capitalize_string>`
  *  :ref:`pad_left (str: string; width: int; ch: int = 32) : string <function-strings_boost_pad_left_string_int_int>`
  *  :ref:`pad_right (str: string; width: int; ch: int = 32) : string <function-strings_boost_pad_right_string_int_int>`
  *  :ref:`wide (text: string; width: int) : string <function-strings_boost_wide_string_int>`

.. _function-strings_boost_capitalize_string:

.. das:function:: capitalize(str: string) : string

Returns a copy of the string with the first character converted to uppercase. The rest of the string is unchanged.


:Arguments: * **str** : string

.. _function-strings_boost_pad_left_string_int_int:

.. das:function:: pad_left(str: string; width: int; ch: int = 32) : string

Pads the string with the character `ch` on the left to reach the specified minimum `width`. If the string is already at least `width` characters, it is returned unchanged.


:Arguments: * **str** : string

            * **width** : int

            * **ch** : int

.. _function-strings_boost_pad_right_string_int_int:

.. das:function:: pad_right(str: string; width: int; ch: int = 32) : string

Pads the string with the character `ch` on the right to reach the specified minimum `width`. If the string is already at least `width` characters, it is returned unchanged.


:Arguments: * **str** : string

            * **width** : int

            * **ch** : int

.. _function-strings_boost_wide_string_int:

.. das:function:: wide(text: string; width: int) : string

Pads the string with trailing spaces to reach the specified minimum width.


:Arguments: * **text** : string implicit

            * **width** : int


+++++++++++++++++++++++
Queries and comparisons
+++++++++++++++++++++++

  *  :ref:`contains (str: string; sub: string) : bool <function-strings_boost_contains_string_string>`
  *  :ref:`count (str: string; sub: string) : int <function-strings_boost_count_string_string>`
  *  :ref:`eq (b: das_string; a: string) : auto <function-strings_boost_eq_das_string_string>`
  *  :ref:`eq (a: string; b: das_string) : auto <function-strings_boost_eq_string_das_string>`
  *  :ref:`is_character_at (foo: array\<uint8\>; idx: int; ch: int) : auto <function-strings_boost_is_character_at_array_ls_uint8_gr__int_int>`
  *  :ref:`is_null_or_whitespace (str: string) : bool <function-strings_boost_is_null_or_whitespace_string>`

.. _function-strings_boost_contains_string_string:

.. das:function:: contains(str: string; sub: string) : bool

Returns true if `sub` is found anywhere within `str`.


:Arguments: * **str** : string

            * **sub** : string

.. _function-strings_boost_count_string_string:

.. das:function:: count(str: string; sub: string) : int

Counts non-overlapping occurrences of `sub` in `str`. Returns 0 if `sub` is empty or not found.


:Arguments: * **str** : string

            * **sub** : string


eq
^^

.. _function-strings_boost_eq_das_string_string:

.. das:function:: eq(b: das_string; a: string) : auto

Compares a ``string`` with a ``das_string`` for equality, returning ``true`` if they match.


:Arguments: * **b** :  :ref:`das_string <handle-builtin-das_string>`

            * **a** : string implicit

.. _function-strings_boost_eq_string_das_string:

.. das:function:: eq(a: string; b: das_string) : auto

----

.. _function-strings_boost_is_character_at_array_ls_uint8_gr__int_int:

.. das:function:: is_character_at(foo: array<uint8>; idx: int; ch: int) : auto

Returns ``true`` if the byte at the specified index in the array equals the given character code.


:Arguments: * **foo** : array<uint8> implicit

            * **idx** : int

            * **ch** : int

.. _function-strings_boost_is_null_or_whitespace_string:

.. das:function:: is_null_or_whitespace(str: string) : bool

Returns true if the string is null, empty, or contains only whitespace characters (space, tab, CR, LF).


:Arguments: * **str** : string


++++++
Search
++++++

  *  :ref:`last_index_of (str: string; sub: string; start: int) : int <function-strings_boost_last_index_of_string_string_int>`
  *  :ref:`last_index_of (str: string; sub: string) : int <function-strings_boost_last_index_of_string_string>`


last_index_of
^^^^^^^^^^^^^

.. _function-strings_boost_last_index_of_string_string_int:

.. das:function:: last_index_of(str: string; sub: string; start: int) : int

Returns the index of the last occurrence of `sub` in `str` searching only up to position `start` (exclusive), or -1 if not found.


:Arguments: * **str** : string

            * **sub** : string

            * **start** : int

.. _function-strings_boost_last_index_of_string_string:

.. das:function:: last_index_of(str: string; sub: string) : int


+++++++
Replace
+++++++

  *  :ref:`replace_multiple (source: string; replaces: array\<tuple\<text:string;replacement:string\>\>) : string <function-strings_boost_replace_multiple_string_array_ls_tuple_ls_text_c_string;replacement_c_string_gr__gr_>`

.. _function-strings_boost_replace_multiple_string_array_ls_tuple_ls_text_c_string;replacement_c_string_gr__gr_:

.. das:function:: replace_multiple(source: string; replaces: array<tuple<text:string;replacement:string>>) : string

Applies multiple find-and-replace substitutions to a string in a single pass.


:Arguments: * **source** : string

            * **replaces** : array<tuple<text:string;replacement:string>>


+++++++++++++++++
Prefix and suffix
+++++++++++++++++

  *  :ref:`trim_prefix (str: string; prefix: string) : string <function-strings_boost_trim_prefix_string_string>`
  *  :ref:`trim_suffix (str: string; suffix: string) : string <function-strings_boost_trim_suffix_string_string>`

.. _function-strings_boost_trim_prefix_string_string:

.. das:function:: trim_prefix(str: string; prefix: string) : string

Removes `prefix` from the beginning of `str` if present. Returns the string unchanged if it does not start with `prefix`.


:Arguments: * **str** : string

            * **prefix** : string

.. _function-strings_boost_trim_suffix_string_string:

.. das:function:: trim_suffix(str: string; suffix: string) : string

Removes `suffix` from the end of `str` if present. Returns the string unchanged if it does not end with `suffix`.


:Arguments: * **str** : string

            * **suffix** : string


++++++++++++++++++++
Levenshtein distance
++++++++++++++++++++

  *  :ref:`levenshtein_distance (s: string; t: string) : int <function-strings_boost_levenshtein_distance_string_string>`
  *  :ref:`levenshtein_distance_fast (s: string; t: string) : int <function-strings_boost_levenshtein_distance_fast_string_string>`

.. _function-strings_boost_levenshtein_distance_string_string:

.. das:function:: levenshtein_distance(s: string; t: string) : int

Computes the Levenshtein edit distance between two strings.


:Arguments: * **s** : string implicit

            * **t** : string implicit

.. _function-strings_boost_levenshtein_distance_fast_string_string:

.. das:function:: levenshtein_distance_fast(s: string; t: string) : int

Computes the Levenshtein edit distance between two strings using an optimized algorithm.


:Arguments: * **s** : string implicit

            * **t** : string implicit


