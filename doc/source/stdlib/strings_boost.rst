
.. _stdlib_strings_boost:

=============================================
Boost package for string manipulation library
=============================================

The STRINGS boost module implements collection of helper macros and functions to accompany :ref:`STRINGS <stdlib_strings>`.

All functions and symbols are in "strings_boost" module, use require to get access to it. ::

    require daslib/strings_boost

++++++++++++++
Split and join
++++++++++++++

  *  :ref:`split (text: string implicit; delim: string implicit) : array\<string\> <function-strings_boost_split_string_implicit_string_implicit>` 
  *  :ref:`split_by_chars (text: string implicit; delim: string implicit) : array\<string\> <function-strings_boost_split_by_chars_string_implicit_string_implicit>` 
  *  :ref:`join (it: auto; separator: string implicit) : auto <function-strings_boost_join_auto_string_implicit>` 
  *  :ref:`join (var it: iterator\<auto(TT)\>; separator: string implicit) : auto <function-strings_boost_join_iterator_ls_autoTT_gr__string_implicit>` 
  *  :ref:`join (iterable: array\<auto(TT)\>; separator: string; blk: block\<(var writer:StringBuilderWriter;elem:TT):void\>) : string <function-strings_boost_join_array_ls_autoTT_gr__string_block_ls_var_writer_c_StringBuilderWriter;elem_c_TT_c_void_gr_>` 
  *  :ref:`join (var iterable: iterator\<auto(TT)\>; separator: string; blk: block\<(var writer:StringBuilderWriter;elem:TT):void\>) : string <function-strings_boost_join_iterator_ls_autoTT_gr__string_block_ls_var_writer_c_StringBuilderWriter;elem_c_TT_c_void_gr_>` 
  *  :ref:`join (iterable: auto(TT)[]; separator: string; blk: block\<(var writer:StringBuilderWriter;elem:TT):void\>) : string <function-strings_boost_join_autoTT_string_block_ls_var_writer_c_StringBuilderWriter;elem_c_TT_c_void_gr_>` 
  *  :ref:`split (text: string implicit; delim: string implicit; blk: block\<(arg:array\<string\>#):auto\>) : auto <function-strings_boost_split_string_implicit_string_implicit_block_ls_arg_c_array_ls_string_gr__hh__c_auto_gr_>` 
  *  :ref:`split_by_chars (text: string implicit; delim: string implicit; blk: block\<(arg:array\<string\>#):auto\>) : auto <function-strings_boost_split_by_chars_string_implicit_string_implicit_block_ls_arg_c_array_ls_string_gr__hh__c_auto_gr_>` 

.. _function-strings_boost_split_string_implicit_string_implicit:

.. das:function:: split(text: string implicit; delim: string implicit) : array<string>

|detail/function-strings_boost-split-0x98c88ba73f93b208|

:Arguments: * **text** : string implicit

            * **delim** : string implicit

.. _function-strings_boost_split_by_chars_string_implicit_string_implicit:

.. das:function:: split_by_chars(text: string implicit; delim: string implicit) : array<string>

|detail/function-strings_boost-split_by_chars-0x50d77ec70e92537d|

:Arguments: * **text** : string implicit

            * **delim** : string implicit

.. _function-strings_boost_join_auto_string_implicit:

.. das:function:: join(it: auto; separator: string implicit) : auto

|detail/function-strings_boost-join-0x41e62613d7875e24|

:Arguments: * **it** : auto

            * **separator** : string implicit

.. _function-strings_boost_join_iterator_ls_autoTT_gr__string_implicit:

.. das:function:: join(it: iterator<auto(TT)>; separator: string implicit) : auto

|detail/function-strings_boost-join-0x4544055f6291437c|

:Arguments: * **it** : iterator<auto(TT)>

            * **separator** : string implicit

.. _function-strings_boost_join_array_ls_autoTT_gr__string_block_ls_var_writer_c_StringBuilderWriter;elem_c_TT_c_void_gr_:

.. das:function:: join(iterable: array<auto(TT)>; separator: string; blk: block<(var writer:StringBuilderWriter;elem:TT):void>) : string

|detail/function-strings_boost-join-0xa8d0711c81d56f64|

:Arguments: * **iterable** : array<auto(TT)>

            * **separator** : string

            * **blk** : block<(writer: :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>` ;elem:TT):void>

.. _function-strings_boost_join_iterator_ls_autoTT_gr__string_block_ls_var_writer_c_StringBuilderWriter;elem_c_TT_c_void_gr_:

.. das:function:: join(iterable: iterator<auto(TT)>; separator: string; blk: block<(var writer:StringBuilderWriter;elem:TT):void>) : string

|detail/function-strings_boost-join-0x7596deabac971650|

:Arguments: * **iterable** : iterator<auto(TT)>

            * **separator** : string

            * **blk** : block<(writer: :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>` ;elem:TT):void>

.. _function-strings_boost_join_autoTT_string_block_ls_var_writer_c_StringBuilderWriter;elem_c_TT_c_void_gr_:

.. das:function:: join(iterable: auto(TT)[]; separator: string; blk: block<(var writer:StringBuilderWriter;elem:TT):void>) : string

|detail/function-strings_boost-join-0xf3e1ee03922f20b1|

:Arguments: * **iterable** : auto(TT)[-1]

            * **separator** : string

            * **blk** : block<(writer: :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>` ;elem:TT):void>

.. _function-strings_boost_split_string_implicit_string_implicit_block_ls_arg_c_array_ls_string_gr__hh__c_auto_gr_:

.. das:function:: split(text: string implicit; delim: string implicit; blk: block<(arg:array<string>#):auto>) : auto

|detail/function-strings_boost-split-0x6e8403f09615ca42|

:Arguments: * **text** : string implicit

            * **delim** : string implicit

            * **blk** : block<(arg:array<string>#):auto>

.. _function-strings_boost_split_by_chars_string_implicit_string_implicit_block_ls_arg_c_array_ls_string_gr__hh__c_auto_gr_:

.. das:function:: split_by_chars(text: string implicit; delim: string implicit; blk: block<(arg:array<string>#):auto>) : auto

|detail/function-strings_boost-split_by_chars-0xb26a143cca9b4599|

:Arguments: * **text** : string implicit

            * **delim** : string implicit

            * **blk** : block<(arg:array<string>#):auto>

++++++++++
Formatting
++++++++++

  *  :ref:`wide (text: string implicit; width: int) : string <function-strings_boost_wide_string_implicit_int>` 

.. _function-strings_boost_wide_string_implicit_int:

.. das:function:: wide(text: string implicit; width: int) : string

|detail/function-strings_boost-wide-0xd7e9426ac00a0273|

:Arguments: * **text** : string implicit

            * **width** : int

+++++++++++++++++++++++
Queries and comparisons
+++++++++++++++++++++++

  *  :ref:`is_character_at (foo: array\<uint8\>; idx: int; ch: int) : auto <function-strings_boost_is_character_at_array_ls_uint8_gr__int_int>` 
  *  :ref:`eq (a: string implicit; b: das_string) : auto <function-strings_boost_eq_string_implicit_das_string>` 
  *  :ref:`eq (b: das_string; a: string implicit) : auto <function-strings_boost_eq_das_string_string_implicit>` 

.. _function-strings_boost_is_character_at_array_ls_uint8_gr__int_int:

.. das:function:: is_character_at(foo: array<uint8>; idx: int; ch: int) : auto

|detail/function-strings_boost-is_character_at-0x178db01adc75a5e3|

:Arguments: * **foo** : array<uint8> implicit

            * **idx** : int

            * **ch** : int

.. _function-strings_boost_eq_string_implicit_das_string:

.. das:function:: eq(a: string implicit; b: das_string) : auto

|detail/function-strings_boost-eq-0xbd40acbc40406243|

:Arguments: * **a** : string implicit

            * **b** :  :ref:`das_string <handle-builtin-das_string>` 

.. _function-strings_boost_eq_das_string_string_implicit:

.. das:function:: eq(b: das_string; a: string implicit) : auto

|detail/function-strings_boost-eq-0xc6812cfa1c95c52b|

:Arguments: * **b** :  :ref:`das_string <handle-builtin-das_string>` 

            * **a** : string implicit

+++++++
Replace
+++++++

  *  :ref:`replace_multiple (source: string; replaces: array\<tuple\<text:string;replacement:string\>\>) : string <function-strings_boost_replace_multiple_string_array_ls_tuple_ls_text_c_string;replacement_c_string_gr__gr_>` 

.. _function-strings_boost_replace_multiple_string_array_ls_tuple_ls_text_c_string;replacement_c_string_gr__gr_:

.. das:function:: replace_multiple(source: string; replaces: array<tuple<text:string;replacement:string>>) : string

|detail/function-strings_boost-replace_multiple-0x40ddf96517436d79|

:Arguments: * **source** : string

            * **replaces** : array<tuple<text:string;replacement:string>>

++++++++++++++++++++
Levenshtein distance
++++++++++++++++++++

  *  :ref:`levenshtein_distance (s: string implicit; t: string implicit) : int <function-strings_boost_levenshtein_distance_string_implicit_string_implicit>` 
  *  :ref:`levenshtein_distance_fast (s: string implicit; t: string implicit) : int <function-strings_boost_levenshtein_distance_fast_string_implicit_string_implicit>` 

.. _function-strings_boost_levenshtein_distance_string_implicit_string_implicit:

.. das:function:: levenshtein_distance(s: string implicit; t: string implicit) : int

|detail/function-strings_boost-levenshtein_distance-0x92499c3cd3c81949|

:Arguments: * **s** : string implicit

            * **t** : string implicit

.. _function-strings_boost_levenshtein_distance_fast_string_implicit_string_implicit:

.. das:function:: levenshtein_distance_fast(s: string implicit; t: string implicit) : int

|detail/function-strings_boost-levenshtein_distance_fast-0x86bb381a3ba281ed|

:Arguments: * **s** : string implicit

            * **t** : string implicit


