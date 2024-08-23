
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

  *  :ref:`split (text: string implicit; delim: string implicit) : array\<string\> <function-_at_strings_boost_c__c_split_CIs_CIs>` 
  *  :ref:`split_by_chars (text: string implicit; delim: string implicit) : array\<string\> <function-_at_strings_boost_c__c_split_by_chars_CIs_CIs>` 
  *  :ref:`join (it: auto; separator: string implicit) : auto <function-_at_strings_boost_c__c_join_C_dot__CIs>` 
  *  :ref:`join (iterable: array\<auto(TT)\>; separator: string; blk: block\<(var writer:StringBuilderWriter;elem:TT const):void\>) : string <function-_at_strings_boost_c__c_join_C1_ls_Y_ls_TT_gr__dot__gr_A_Cs_CN_ls_writer;elem_gr_0_ls_H_ls_strings_c__c_StringBuilderWriter_gr_;CY_ls_TT_gr_L_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`join (iterable: iterator\<auto(TT)\>; separator: string; blk: block\<(var writer:StringBuilderWriter;elem:TT const):void\>) : string <function-_at_strings_boost_c__c_join_C1_ls_Y_ls_TT_gr__dot__gr_G_Cs_CN_ls_writer;elem_gr_0_ls_H_ls_strings_c__c_StringBuilderWriter_gr_;CY_ls_TT_gr_L_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`join (iterable: auto(TT)[]; separator: string; blk: block\<(var writer:StringBuilderWriter;elem:TT const):void\>) : string <function-_at_strings_boost_c__c_join_C_lb_-1_rb_Y_ls_TT_gr__dot__Cs_CN_ls_writer;elem_gr_0_ls_H_ls_strings_c__c_StringBuilderWriter_gr_;CY_ls_TT_gr_L_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`split (text: string implicit; delim: string implicit; blk: block\<(arg:array\<string\> const#):auto\>) : auto <function-_at_strings_boost_c__c_split_CIs_CIs_CN_ls_arg_gr_0_ls_C_hh_1_ls_s_gr_A_gr_1_ls__dot__gr__builtin_>` 
  *  :ref:`split_by_chars (text: string implicit; delim: string implicit; blk: block\<(arg:array\<string\> const#):auto\>) : auto <function-_at_strings_boost_c__c_split_by_chars_CIs_CIs_CN_ls_arg_gr_0_ls_C_hh_1_ls_s_gr_A_gr_1_ls__dot__gr__builtin_>` 

.. _function-_at_strings_boost_c__c_split_CIs_CIs:

.. das:function:: split(text: string implicit; delim: string implicit) : array<string>

// stub


:Arguments: * **text** : string implicit

            * **delim** : string implicit

.. _function-_at_strings_boost_c__c_split_by_chars_CIs_CIs:

.. das:function:: split_by_chars(text: string implicit; delim: string implicit) : array<string>

// stub


:Arguments: * **text** : string implicit

            * **delim** : string implicit

.. _function-_at_strings_boost_c__c_join_C_dot__CIs:

.. das:function:: join(it: auto; separator: string implicit) : auto

// stub


:Arguments: * **it** : auto

            * **separator** : string implicit

.. _function-_at_strings_boost_c__c_join_C1_ls_Y_ls_TT_gr__dot__gr_A_Cs_CN_ls_writer;elem_gr_0_ls_H_ls_strings_c__c_StringBuilderWriter_gr_;CY_ls_TT_gr_L_gr_1_ls_v_gr__builtin_:

.. das:function:: join(iterable: array<auto(TT)>; separator: string; blk: block<(var writer:StringBuilderWriter;elem:TT const):void>) : string

// stub


:Arguments: * **iterable** : array<auto(TT)>

            * **separator** : string

            * **blk** : block<(writer: :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>` ;elem:TT):void>

.. _function-_at_strings_boost_c__c_join_C1_ls_Y_ls_TT_gr__dot__gr_G_Cs_CN_ls_writer;elem_gr_0_ls_H_ls_strings_c__c_StringBuilderWriter_gr_;CY_ls_TT_gr_L_gr_1_ls_v_gr__builtin_:

.. das:function:: join(iterable: iterator<auto(TT)>; separator: string; blk: block<(var writer:StringBuilderWriter;elem:TT const):void>) : string

// stub


:Arguments: * **iterable** : iterator<auto(TT)>

            * **separator** : string

            * **blk** : block<(writer: :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>` ;elem:TT):void>

.. _function-_at_strings_boost_c__c_join_C_lb_-1_rb_Y_ls_TT_gr__dot__Cs_CN_ls_writer;elem_gr_0_ls_H_ls_strings_c__c_StringBuilderWriter_gr_;CY_ls_TT_gr_L_gr_1_ls_v_gr__builtin_:

.. das:function:: join(iterable: auto(TT)[]; separator: string; blk: block<(var writer:StringBuilderWriter;elem:TT const):void>) : string

// stub


:Arguments: * **iterable** : auto(TT)[-1]

            * **separator** : string

            * **blk** : block<(writer: :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>` ;elem:TT):void>

.. _function-_at_strings_boost_c__c_split_CIs_CIs_CN_ls_arg_gr_0_ls_C_hh_1_ls_s_gr_A_gr_1_ls__dot__gr__builtin_:

.. das:function:: split(text: string implicit; delim: string implicit; blk: block<(arg:array<string> const#):auto>) : auto

// stub


:Arguments: * **text** : string implicit

            * **delim** : string implicit

            * **blk** : block<(arg:array<string>#):auto>

.. _function-_at_strings_boost_c__c_split_by_chars_CIs_CIs_CN_ls_arg_gr_0_ls_C_hh_1_ls_s_gr_A_gr_1_ls__dot__gr__builtin_:

.. das:function:: split_by_chars(text: string implicit; delim: string implicit; blk: block<(arg:array<string> const#):auto>) : auto

// stub


:Arguments: * **text** : string implicit

            * **delim** : string implicit

            * **blk** : block<(arg:array<string>#):auto>

++++++++++
Formatting
++++++++++

  *  :ref:`wide (text: string implicit; width: int) : string <function-_at_strings_boost_c__c_wide_CIs_Ci>` 

.. _function-_at_strings_boost_c__c_wide_CIs_Ci:

.. das:function:: wide(text: string implicit; width: int) : string

// stub


:Arguments: * **text** : string implicit

            * **width** : int

+++++++++++++++++++++++
Queries and comparisons
+++++++++++++++++++++++

  *  :ref:`is_character_at (foo: array\<uint8\> implicit; idx: int; ch: int) : auto <function-_at_strings_boost_c__c_is_character_at_CI1_ls_u8_gr_A_Ci_Ci>` 
  *  :ref:`eq (a: string implicit; b: das_string) : auto <function-_at_strings_boost_c__c_eq_CIs_CH_ls__builtin__c__c_das_string_gr_>` 
  *  :ref:`eq (b: das_string; a: string implicit) : auto <function-_at_strings_boost_c__c_eq_CH_ls__builtin__c__c_das_string_gr__CIs>` 

.. _function-_at_strings_boost_c__c_is_character_at_CI1_ls_u8_gr_A_Ci_Ci:

.. das:function:: is_character_at(foo: array<uint8> implicit; idx: int; ch: int) : auto

// stub


:Arguments: * **foo** : array<uint8> implicit

            * **idx** : int

            * **ch** : int

.. _function-_at_strings_boost_c__c_eq_CIs_CH_ls__builtin__c__c_das_string_gr_:

.. das:function:: eq(a: string implicit; b: das_string) : auto

// stub


:Arguments: * **a** : string implicit

            * **b** :  :ref:`das_string <handle-builtin-das_string>` 

.. _function-_at_strings_boost_c__c_eq_CH_ls__builtin__c__c_das_string_gr__CIs:

.. das:function:: eq(b: das_string; a: string implicit) : auto

// stub


:Arguments: * **b** :  :ref:`das_string <handle-builtin-das_string>` 

            * **a** : string implicit

+++++++
Replace
+++++++

  *  :ref:`replace_multiple (source: string; replaces: array\<tuple\<text:string;replacement:string\>\>) : string <function-_at_strings_boost_c__c_replace_multiple_Cs_C1_ls_N_ls_text;replacement_gr_0_ls_s;s_gr_U_gr_A>` 

.. _function-_at_strings_boost_c__c_replace_multiple_Cs_C1_ls_N_ls_text;replacement_gr_0_ls_s;s_gr_U_gr_A:

.. das:function:: replace_multiple(source: string; replaces: array<tuple<text:string;replacement:string>>) : string

// stub


:Arguments: * **source** : string

            * **replaces** : array<tuple<text:string;replacement:string>>

++++++++++++++++++++
Levenshtein distance
++++++++++++++++++++

  *  :ref:`levenshtein_distance (s: string implicit; t: string implicit) : int <function-_at_strings_boost_c__c_levenshtein_distance_CIs_CIs>` 
  *  :ref:`levenshtein_distance_fast (s: string implicit; t: string implicit) : int <function-_at_strings_boost_c__c_levenshtein_distance_fast_CIs_CIs>` 

.. _function-_at_strings_boost_c__c_levenshtein_distance_CIs_CIs:

.. das:function:: levenshtein_distance(s: string implicit; t: string implicit) : int

// stub


:Arguments: * **s** : string implicit

            * **t** : string implicit

.. _function-_at_strings_boost_c__c_levenshtein_distance_fast_CIs_CIs:

.. das:function:: levenshtein_distance_fast(s: string implicit; t: string implicit) : int

// stub


:Arguments: * **s** : string implicit

            * **t** : string implicit

++++++++++++++++
Character traits
++++++++++++++++

  *  :ref:`is_hex (ch: int) : bool <function-_at_strings_boost_c__c_is_hex_Ci>` 
  *  :ref:`is_tab_or_space (ch: int) : bool <function-_at_strings_boost_c__c_is_tab_or_space_Ci>` 

.. _function-_at_strings_boost_c__c_is_hex_Ci:

.. das:function:: is_hex(ch: int) : bool

// stub


:Arguments: * **ch** : int

.. _function-_at_strings_boost_c__c_is_tab_or_space_Ci:

.. das:function:: is_tab_or_space(ch: int) : bool

// stub


:Arguments: * **ch** : int


