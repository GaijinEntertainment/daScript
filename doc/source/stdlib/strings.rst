
.. _stdlib_strings:

===========================
String manipulation library
===========================

The string library implements string formatting, conversion, searching, and modification routines.

All functions and symbols are in "strings" module, use require to get access to it. ::

    require strings


++++++++++++
Enumerations
++++++++++++

.. _enum-strings-ConversionResult:

.. das:attribute:: ConversionResult

:Values: * **ok** = 0

         * **invalid_argument** = 22

         * **out_of_range** = 34

// stub


++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-strings-StringBuilderWriter:

.. das:attribute:: StringBuilderWriter

// stub


+++++++++++++
Character set
+++++++++++++

  *  :ref:`is_char_in_set (Character: int; Charset: uint const[8] implicit) : bool <function-_at_strings_c__c_is_char_in_set_Ci_CI_lb_8_rb_u_C_c_C_l>` 
  *  :ref:`set_total (Charset: uint const[8] implicit) : uint <function-_at_strings_c__c_set_total_CI_lb_8_rb_u>` 
  *  :ref:`set_element (Character: int; Charset: uint const[8] implicit) : int <function-_at_strings_c__c_set_element_Ci_CI_lb_8_rb_u>` 

.. _function-_at_strings_c__c_is_char_in_set_Ci_CI_lb_8_rb_u_C_c_C_l:

.. das:function:: is_char_in_set(Character: int; Charset: uint const[8] implicit) : bool

// stub



:Arguments: * **Character** : int

            * **Charset** : uint[8] implicit

.. _function-_at_strings_c__c_set_total_CI_lb_8_rb_u:

.. das:function:: set_total(Charset: uint const[8] implicit) : uint

// stub



:Arguments: * **Charset** : uint[8] implicit

.. _function-_at_strings_c__c_set_element_Ci_CI_lb_8_rb_u:

.. das:function:: set_element(Character: int; Charset: uint const[8] implicit) : int

// stub



:Arguments: * **Character** : int

            * **Charset** : uint[8] implicit

++++++++++++++++
Character groups
++++++++++++++++

  *  :ref:`is_alpha (Character: int) : bool <function-_at_strings_c__c_is_alpha_Ci>` 
  *  :ref:`is_new_line (Character: int) : bool <function-_at_strings_c__c_is_new_line_Ci>` 
  *  :ref:`is_white_space (Character: int) : bool <function-_at_strings_c__c_is_white_space_Ci>` 
  *  :ref:`is_number (Character: int) : bool <function-_at_strings_c__c_is_number_Ci>` 

.. _function-_at_strings_c__c_is_alpha_Ci:

.. das:function:: is_alpha(Character: int) : bool

// stub



:Arguments: * **Character** : int

.. _function-_at_strings_c__c_is_new_line_Ci:

.. das:function:: is_new_line(Character: int) : bool

// stub



:Arguments: * **Character** : int

.. _function-_at_strings_c__c_is_white_space_Ci:

.. das:function:: is_white_space(Character: int) : bool

// stub



:Arguments: * **Character** : int

.. _function-_at_strings_c__c_is_number_Ci:

.. das:function:: is_number(Character: int) : bool

// stub



:Arguments: * **Character** : int

++++++++++++++++++
Character by index
++++++++++++++++++

  *  :ref:`character_at (str: string implicit; idx: int) : int <function-_at_strings_c__c_character_at_CIs_Ci_C_c_C_l>` 
  *  :ref:`character_uat (str: string implicit; idx: int) : int <function-_at_strings_c__c_character_uat_CIs_Ci>` 

.. _function-_at_strings_c__c_character_at_CIs_Ci_C_c_C_l:

.. das:function:: character_at(str: string implicit; idx: int) : int

// stub



:Arguments: * **str** : string implicit

            * **idx** : int

.. _function-_at_strings_c__c_character_uat_CIs_Ci:

.. das:function:: character_uat(str: string implicit; idx: int) : int

.. warning:: 
  This is unsafe operation.

// stub



:Arguments: * **str** : string implicit

            * **idx** : int

+++++++++++++++++
String properties
+++++++++++++++++

  *  :ref:`ends_with (str: string implicit; cmp: string implicit) : bool <function-_at_strings_c__c_ends_with_CIs_CIs_C_c>` 
  *  :ref:`ends_with (str: das_string implicit; cmp: string implicit) : bool <function-_at_strings_c__c_ends_with_CIH_ls__builtin__c__c_das_string_gr__CIs_C_c>` 
  *  :ref:`starts_with (str: string implicit; cmp: string implicit) : bool <function-_at_strings_c__c_starts_with_CIs_CIs_C_c>` 
  *  :ref:`starts_with (str: string implicit; cmp: string implicit; cmpLen: uint) : bool <function-_at_strings_c__c_starts_with_CIs_CIs_Cu_C_c>` 
  *  :ref:`starts_with (str: string implicit; offset: int; cmp: string implicit) : bool <function-_at_strings_c__c_starts_with_CIs_Ci_CIs_C_c>` 
  *  :ref:`starts_with (str: string implicit; offset: int; cmp: string implicit; cmpLen: uint) : bool <function-_at_strings_c__c_starts_with_CIs_Ci_CIs_Cu_C_c>` 
  *  :ref:`length (str: string implicit) : int <function-_at_strings_c__c_length_CIs_C_c>` 
  *  :ref:`length (str: das_string implicit) : int <function-_at_strings_c__c_length_CIH_ls__builtin__c__c_das_string_gr_>` 

.. _function-_at_strings_c__c_ends_with_CIs_CIs_C_c:

.. das:function:: ends_with(str: string implicit; cmp: string implicit) : bool

// stub



:Arguments: * **str** : string implicit

            * **cmp** : string implicit

.. _function-_at_strings_c__c_ends_with_CIH_ls__builtin__c__c_das_string_gr__CIs_C_c:

.. das:function:: ends_with(str: das_string implicit; cmp: string implicit) : bool

// stub



:Arguments: * **str** :  :ref:`das_string <handle-builtin-das_string>`  implicit

            * **cmp** : string implicit

.. _function-_at_strings_c__c_starts_with_CIs_CIs_C_c:

.. das:function:: starts_with(str: string implicit; cmp: string implicit) : bool

// stub



:Arguments: * **str** : string implicit

            * **cmp** : string implicit

.. _function-_at_strings_c__c_starts_with_CIs_CIs_Cu_C_c:

.. das:function:: starts_with(str: string implicit; cmp: string implicit; cmpLen: uint) : bool

// stub



:Arguments: * **str** : string implicit

            * **cmp** : string implicit

            * **cmpLen** : uint

.. _function-_at_strings_c__c_starts_with_CIs_Ci_CIs_C_c:

.. das:function:: starts_with(str: string implicit; offset: int; cmp: string implicit) : bool

// stub



:Arguments: * **str** : string implicit

            * **offset** : int

            * **cmp** : string implicit

.. _function-_at_strings_c__c_starts_with_CIs_Ci_CIs_Cu_C_c:

.. das:function:: starts_with(str: string implicit; offset: int; cmp: string implicit; cmpLen: uint) : bool

// stub



:Arguments: * **str** : string implicit

            * **offset** : int

            * **cmp** : string implicit

            * **cmpLen** : uint

.. _function-_at_strings_c__c_length_CIs_C_c:

.. das:function:: length(str: string implicit) : int

// stub



:Arguments: * **str** : string implicit

.. _function-_at_strings_c__c_length_CIH_ls__builtin__c__c_das_string_gr_:

.. das:function:: length(str: das_string implicit) : int

// stub



:Arguments: * **str** :  :ref:`das_string <handle-builtin-das_string>`  implicit

++++++++++++++
String builder
++++++++++++++

  *  :ref:`build_string (block: block\<(var arg0:StringBuilderWriter):void\> implicit) : string <function-_at_strings_c__c_build_string_CI0_ls_H_ls_strings_c__c_StringBuilderWriter_gr__gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`build_hash (block: block\<(var arg0:StringBuilderWriter):void\> implicit) : uint64 <function-_at_strings_c__c_build_hash_CI0_ls_H_ls_strings_c__c_StringBuilderWriter_gr__gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`write (writer: StringBuilderWriter; anything: any) : StringBuilderWriter& <function-_at_strings_c__c_write_H_ls_strings_c__c_StringBuilderWriter_gr___st_>` 
  *  :ref:`write_char (writer: StringBuilderWriter implicit; ch: int) : StringBuilderWriter& <function-_at_strings_c__c_write_char_IH_ls_strings_c__c_StringBuilderWriter_gr__Ci>` 
  *  :ref:`write_chars (writer: StringBuilderWriter implicit; ch: int; count: int) : StringBuilderWriter& <function-_at_strings_c__c_write_chars_IH_ls_strings_c__c_StringBuilderWriter_gr__Ci_Ci>` 
  *  :ref:`write_escape_string (writer: StringBuilderWriter implicit; str: string implicit) : StringBuilderWriter& <function-_at_strings_c__c_write_escape_string_IH_ls_strings_c__c_StringBuilderWriter_gr__CIs>` 
  *  :ref:`format (writer: StringBuilderWriter implicit; format: string implicit; value: int) : StringBuilderWriter& <function-_at_strings_c__c_format_IH_ls_strings_c__c_StringBuilderWriter_gr__CIs_Ci>` 
  *  :ref:`format (writer: StringBuilderWriter implicit; format: string implicit; value: uint) : StringBuilderWriter& <function-_at_strings_c__c_format_IH_ls_strings_c__c_StringBuilderWriter_gr__CIs_Cu>` 
  *  :ref:`format (writer: StringBuilderWriter implicit; format: string implicit; value: int64) : StringBuilderWriter& <function-_at_strings_c__c_format_IH_ls_strings_c__c_StringBuilderWriter_gr__CIs_Ci64>` 
  *  :ref:`format (writer: StringBuilderWriter implicit; format: string implicit; value: uint64) : StringBuilderWriter& <function-_at_strings_c__c_format_IH_ls_strings_c__c_StringBuilderWriter_gr__CIs_Cu64>` 
  *  :ref:`format (writer: StringBuilderWriter implicit; format: string implicit; value: float) : StringBuilderWriter& <function-_at_strings_c__c_format_IH_ls_strings_c__c_StringBuilderWriter_gr__CIs_Cf>` 
  *  :ref:`format (writer: StringBuilderWriter implicit; format: string implicit; value: double) : StringBuilderWriter& <function-_at_strings_c__c_format_IH_ls_strings_c__c_StringBuilderWriter_gr__CIs_Cd>` 
  *  :ref:`format (format: string implicit; value: int) : string <function-_at_strings_c__c_format_CIs_Ci_C_c_C_l>` 
  *  :ref:`format (format: string implicit; value: uint) : string <function-_at_strings_c__c_format_CIs_Cu_C_c_C_l>` 
  *  :ref:`format (format: string implicit; value: int64) : string <function-_at_strings_c__c_format_CIs_Ci64_C_c_C_l>` 
  *  :ref:`format (format: string implicit; value: uint64) : string <function-_at_strings_c__c_format_CIs_Cu64_C_c_C_l>` 
  *  :ref:`format (format: string implicit; value: float) : string <function-_at_strings_c__c_format_CIs_Cf_C_c_C_l>` 
  *  :ref:`format (format: string implicit; value: double) : string <function-_at_strings_c__c_format_CIs_Cd_C_c_C_l>` 

.. _function-_at_strings_c__c_build_string_CI0_ls_H_ls_strings_c__c_StringBuilderWriter_gr__gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: build_string(block: block<(var arg0:StringBuilderWriter):void> implicit) : string

// stub



:Arguments: * **block** : block<( :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>` ):void> implicit

.. _function-_at_strings_c__c_build_hash_CI0_ls_H_ls_strings_c__c_StringBuilderWriter_gr__gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: build_hash(block: block<(var arg0:StringBuilderWriter):void> implicit) : uint64

// stub



:Arguments: * **block** : block<( :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>` ):void> implicit

.. _function-_at_strings_c__c_write_H_ls_strings_c__c_StringBuilderWriter_gr___st_:

.. das:function:: write(writer: StringBuilderWriter; anything: any) : StringBuilderWriter&

// stub



:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>` 

            * **anything** : any

.. _function-_at_strings_c__c_write_char_IH_ls_strings_c__c_StringBuilderWriter_gr__Ci:

.. das:function:: write_char(writer: StringBuilderWriter implicit; ch: int) : StringBuilderWriter&

// stub



:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **ch** : int

.. _function-_at_strings_c__c_write_chars_IH_ls_strings_c__c_StringBuilderWriter_gr__Ci_Ci:

.. das:function:: write_chars(writer: StringBuilderWriter implicit; ch: int; count: int) : StringBuilderWriter&

// stub



:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **ch** : int

            * **count** : int

.. _function-_at_strings_c__c_write_escape_string_IH_ls_strings_c__c_StringBuilderWriter_gr__CIs:

.. das:function:: write_escape_string(writer: StringBuilderWriter implicit; str: string implicit) : StringBuilderWriter&

// stub



:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **str** : string implicit

.. _function-_at_strings_c__c_format_IH_ls_strings_c__c_StringBuilderWriter_gr__CIs_Ci:

.. das:function:: format(writer: StringBuilderWriter implicit; format: string implicit; value: int) : StringBuilderWriter&

.. warning:: 
  This function is deprecated.

// stub



:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **format** : string implicit

            * **value** : int

.. _function-_at_strings_c__c_format_IH_ls_strings_c__c_StringBuilderWriter_gr__CIs_Cu:

.. das:function:: format(writer: StringBuilderWriter implicit; format: string implicit; value: uint) : StringBuilderWriter&

.. warning:: 
  This function is deprecated.

// stub



:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **format** : string implicit

            * **value** : uint

.. _function-_at_strings_c__c_format_IH_ls_strings_c__c_StringBuilderWriter_gr__CIs_Ci64:

.. das:function:: format(writer: StringBuilderWriter implicit; format: string implicit; value: int64) : StringBuilderWriter&

.. warning:: 
  This function is deprecated.

// stub



:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **format** : string implicit

            * **value** : int64

.. _function-_at_strings_c__c_format_IH_ls_strings_c__c_StringBuilderWriter_gr__CIs_Cu64:

.. das:function:: format(writer: StringBuilderWriter implicit; format: string implicit; value: uint64) : StringBuilderWriter&

.. warning:: 
  This function is deprecated.

// stub



:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **format** : string implicit

            * **value** : uint64

.. _function-_at_strings_c__c_format_IH_ls_strings_c__c_StringBuilderWriter_gr__CIs_Cf:

.. das:function:: format(writer: StringBuilderWriter implicit; format: string implicit; value: float) : StringBuilderWriter&

.. warning:: 
  This function is deprecated.

// stub



:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **format** : string implicit

            * **value** : float

.. _function-_at_strings_c__c_format_IH_ls_strings_c__c_StringBuilderWriter_gr__CIs_Cd:

.. das:function:: format(writer: StringBuilderWriter implicit; format: string implicit; value: double) : StringBuilderWriter&

.. warning:: 
  This function is deprecated.

// stub



:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **format** : string implicit

            * **value** : double

.. _function-_at_strings_c__c_format_CIs_Ci_C_c_C_l:

.. das:function:: format(format: string implicit; value: int) : string

.. warning:: 
  This function is deprecated.

// stub



:Arguments: * **format** : string implicit

            * **value** : int

.. _function-_at_strings_c__c_format_CIs_Cu_C_c_C_l:

.. das:function:: format(format: string implicit; value: uint) : string

.. warning:: 
  This function is deprecated.

// stub



:Arguments: * **format** : string implicit

            * **value** : uint

.. _function-_at_strings_c__c_format_CIs_Ci64_C_c_C_l:

.. das:function:: format(format: string implicit; value: int64) : string

.. warning:: 
  This function is deprecated.

// stub



:Arguments: * **format** : string implicit

            * **value** : int64

.. _function-_at_strings_c__c_format_CIs_Cu64_C_c_C_l:

.. das:function:: format(format: string implicit; value: uint64) : string

.. warning:: 
  This function is deprecated.

// stub



:Arguments: * **format** : string implicit

            * **value** : uint64

.. _function-_at_strings_c__c_format_CIs_Cf_C_c_C_l:

.. das:function:: format(format: string implicit; value: float) : string

.. warning:: 
  This function is deprecated.

// stub



:Arguments: * **format** : string implicit

            * **value** : float

.. _function-_at_strings_c__c_format_CIs_Cd_C_c_C_l:

.. das:function:: format(format: string implicit; value: double) : string

.. warning:: 
  This function is deprecated.

// stub



:Arguments: * **format** : string implicit

            * **value** : double

++++++++++++++++++++++++
das::string manipulation
++++++++++++++++++++++++

  *  :ref:`append (str: das_string implicit; ch: int) <function-_at_strings_c__c_append_IH_ls__builtin__c__c_das_string_gr__Ci>` 
  *  :ref:`resize (str: das_string implicit; new_length: int) <function-_at_strings_c__c_resize_IH_ls__builtin__c__c_das_string_gr__Ci>` 

.. _function-_at_strings_c__c_append_IH_ls__builtin__c__c_das_string_gr__Ci:

.. das:function:: append(str: das_string implicit; ch: int)

// stub



:Arguments: * **str** :  :ref:`das_string <handle-builtin-das_string>`  implicit

            * **ch** : int

.. _function-_at_strings_c__c_resize_IH_ls__builtin__c__c_das_string_gr__Ci:

.. das:function:: resize(str: das_string implicit; new_length: int)

// stub



:Arguments: * **str** :  :ref:`das_string <handle-builtin-das_string>`  implicit

            * **new_length** : int

++++++++++++++++++++
String modifications
++++++++++++++++++++

  *  :ref:`repeat (str: string implicit; count: int) : string <function-_at_strings_c__c_repeat_CIs_Ci_C_c_C_l>` 
  *  :ref:`strip (str: string implicit) : string <function-_at_strings_c__c_strip_CIs_C_c_C_l>` 
  *  :ref:`strip_right (str: string implicit) : string <function-_at_strings_c__c_strip_right_CIs_C_c_C_l>` 
  *  :ref:`strip_left (str: string implicit) : string <function-_at_strings_c__c_strip_left_CIs_C_c_C_l>` 
  *  :ref:`chop (str: string implicit; start: int; length: int) : string <function-_at_strings_c__c_chop_CIs_Ci_Ci_C_c_C_l>` 
  *  :ref:`slice (str: string implicit; start: int; end: int) : string <function-_at_strings_c__c_slice_CIs_Ci_Ci_C_c_C_l>` 
  *  :ref:`slice (str: string implicit; start: int) : string <function-_at_strings_c__c_slice_CIs_Ci_C_c_C_l>` 
  *  :ref:`reverse (str: string implicit) : string <function-_at_strings_c__c_reverse_CIs_C_c_C_l>` 
  *  :ref:`to_upper (str: string implicit) : string <function-_at_strings_c__c_to_upper_CIs_C_c_C_l>` 
  *  :ref:`to_lower (str: string implicit) : string <function-_at_strings_c__c_to_lower_CIs_C_c_C_l>` 
  *  :ref:`to_lower_in_place (str: string implicit) : string <function-_at_strings_c__c_to_lower_in_place_CIs>` 
  *  :ref:`to_upper_in_place (str: string implicit) : string <function-_at_strings_c__c_to_upper_in_place_CIs>` 
  *  :ref:`escape (str: string implicit) : string <function-_at_strings_c__c_escape_CIs_C_c_C_l>` 
  *  :ref:`unescape (str: string implicit) : string <function-_at_strings_c__c_unescape_CIs_C_c_C_l>` 
  *  :ref:`safe_unescape (str: string implicit) : string <function-_at_strings_c__c_safe_unescape_CIs_C_c_C_l>` 
  *  :ref:`replace (str: string implicit; toSearch: string implicit; replace: string implicit) : string <function-_at_strings_c__c_replace_CIs_CIs_CIs_C_c_C_l>` 
  *  :ref:`rtrim (str: string implicit) : string <function-_at_strings_c__c_rtrim_CIs_C_c_C_l>` 
  *  :ref:`rtrim (str: string implicit; chars: string implicit) : string <function-_at_strings_c__c_rtrim_CIs_CIs_C_c_C_l>` 
  *  :ref:`ltrim (str: string implicit) : string <function-_at_strings_c__c_ltrim_CIs_C_c_C_l>` 
  *  :ref:`trim (str: string implicit) : string <function-_at_strings_c__c_trim_CIs_C_c_C_l>` 

.. _function-_at_strings_c__c_repeat_CIs_Ci_C_c_C_l:

.. das:function:: repeat(str: string implicit; count: int) : string

// stub



:Arguments: * **str** : string implicit

            * **count** : int

.. _function-_at_strings_c__c_strip_CIs_C_c_C_l:

.. das:function:: strip(str: string implicit) : string

// stub



:Arguments: * **str** : string implicit

.. _function-_at_strings_c__c_strip_right_CIs_C_c_C_l:

.. das:function:: strip_right(str: string implicit) : string

// stub



:Arguments: * **str** : string implicit

.. _function-_at_strings_c__c_strip_left_CIs_C_c_C_l:

.. das:function:: strip_left(str: string implicit) : string

// stub



:Arguments: * **str** : string implicit

.. _function-_at_strings_c__c_chop_CIs_Ci_Ci_C_c_C_l:

.. das:function:: chop(str: string implicit; start: int; length: int) : string

// stub



:Arguments: * **str** : string implicit

            * **start** : int

            * **length** : int

.. _function-_at_strings_c__c_slice_CIs_Ci_Ci_C_c_C_l:

.. das:function:: slice(str: string implicit; start: int; end: int) : string

// stub



:Arguments: * **str** : string implicit

            * **start** : int

            * **end** : int

.. _function-_at_strings_c__c_slice_CIs_Ci_C_c_C_l:

.. das:function:: slice(str: string implicit; start: int) : string

// stub



:Arguments: * **str** : string implicit

            * **start** : int

.. _function-_at_strings_c__c_reverse_CIs_C_c_C_l:

.. das:function:: reverse(str: string implicit) : string

// stub



:Arguments: * **str** : string implicit

.. _function-_at_strings_c__c_to_upper_CIs_C_c_C_l:

.. das:function:: to_upper(str: string implicit) : string

// stub



:Arguments: * **str** : string implicit

.. _function-_at_strings_c__c_to_lower_CIs_C_c_C_l:

.. das:function:: to_lower(str: string implicit) : string

// stub



:Arguments: * **str** : string implicit

.. _function-_at_strings_c__c_to_lower_in_place_CIs:

.. das:function:: to_lower_in_place(str: string implicit) : string

.. warning:: 
  This is unsafe operation.

// stub



:Arguments: * **str** : string implicit

.. _function-_at_strings_c__c_to_upper_in_place_CIs:

.. das:function:: to_upper_in_place(str: string implicit) : string

.. warning:: 
  This is unsafe operation.

// stub



:Arguments: * **str** : string implicit

.. _function-_at_strings_c__c_escape_CIs_C_c_C_l:

.. das:function:: escape(str: string implicit) : string

// stub



:Arguments: * **str** : string implicit

.. _function-_at_strings_c__c_unescape_CIs_C_c_C_l:

.. das:function:: unescape(str: string implicit) : string

// stub



:Arguments: * **str** : string implicit

.. _function-_at_strings_c__c_safe_unescape_CIs_C_c_C_l:

.. das:function:: safe_unescape(str: string implicit) : string

// stub



:Arguments: * **str** : string implicit

.. _function-_at_strings_c__c_replace_CIs_CIs_CIs_C_c_C_l:

.. das:function:: replace(str: string implicit; toSearch: string implicit; replace: string implicit) : string

// stub



:Arguments: * **str** : string implicit

            * **toSearch** : string implicit

            * **replace** : string implicit

.. _function-_at_strings_c__c_rtrim_CIs_C_c_C_l:

.. das:function:: rtrim(str: string implicit) : string

// stub



:Arguments: * **str** : string implicit

.. _function-_at_strings_c__c_rtrim_CIs_CIs_C_c_C_l:

.. das:function:: rtrim(str: string implicit; chars: string implicit) : string

// stub



:Arguments: * **str** : string implicit

            * **chars** : string implicit

.. _function-_at_strings_c__c_ltrim_CIs_C_c_C_l:

.. das:function:: ltrim(str: string implicit) : string

// stub



:Arguments: * **str** : string implicit

.. _function-_at_strings_c__c_trim_CIs_C_c_C_l:

.. das:function:: trim(str: string implicit) : string

// stub



:Arguments: * **str** : string implicit

+++++++++++++++++
Search substrings
+++++++++++++++++

  *  :ref:`find (str: string implicit; substr: string implicit; start: int) : int <function-_at_strings_c__c_find_CIs_CIs_Ci_C_c>` 
  *  :ref:`find (str: string implicit; substr: string implicit) : int <function-_at_strings_c__c_find_CIs_CIs>` 
  *  :ref:`find (str: string implicit; substr: int) : int <function-_at_strings_c__c_find_CIs_Ci_C_c>` 
  *  :ref:`find (str: string implicit; substr: int; start: int) : int <function-_at_strings_c__c_find_CIs_Ci_Ci_C_c>` 

.. _function-_at_strings_c__c_find_CIs_CIs_Ci_C_c:

.. das:function:: find(str: string implicit; substr: string implicit; start: int) : int

// stub



:Arguments: * **str** : string implicit

            * **substr** : string implicit

            * **start** : int

.. _function-_at_strings_c__c_find_CIs_CIs:

.. das:function:: find(str: string implicit; substr: string implicit) : int

// stub



:Arguments: * **str** : string implicit

            * **substr** : string implicit

.. _function-_at_strings_c__c_find_CIs_Ci_C_c:

.. das:function:: find(str: string implicit; substr: int) : int

// stub



:Arguments: * **str** : string implicit

            * **substr** : int

.. _function-_at_strings_c__c_find_CIs_Ci_Ci_C_c:

.. das:function:: find(str: string implicit; substr: int; start: int) : int

// stub



:Arguments: * **str** : string implicit

            * **substr** : int

            * **start** : int

++++++++++++++++++++++++++
String conversion routines
++++++++++++++++++++++++++

  *  :ref:`string (bytes: array\<uint8\> implicit) : string <function-_at_strings_c__c_string_CI1_ls_u8_gr_A_C_c_C_l>` 
  *  :ref:`to_char (char: int) : string <function-_at_strings_c__c_to_char_Ci_C_c_C_l>` 
  *  :ref:`int (str: string implicit) : int <function-_at_strings_c__c_int_CIs_C_c_C_l>` 
  *  :ref:`uint (str: string implicit) : uint <function-_at_strings_c__c_uint_CIs_C_c_C_l>` 
  *  :ref:`int64 (str: string implicit) : int64 <function-_at_strings_c__c_int64_CIs_C_c_C_l>` 
  *  :ref:`uint64 (str: string implicit) : uint64 <function-_at_strings_c__c_uint64_CIs_C_c_C_l>` 
  *  :ref:`float (str: string implicit) : float <function-_at_strings_c__c_float_CIs_C_c_C_l>` 
  *  :ref:`double (str: string implicit) : double <function-_at_strings_c__c_double_CIs_C_c_C_l>` 
  *  :ref:`to_int (value: string implicit; hex: bool = false) : int <function-_at_strings_c__c_to_int_CIs_Cb>` 
  *  :ref:`to_uint (value: string implicit; hex: bool = false) : uint <function-_at_strings_c__c_to_uint_CIs_Cb>` 
  *  :ref:`to_int64 (value: string implicit; hex: bool = false) : int64 <function-_at_strings_c__c_to_int64_CIs_Cb>` 
  *  :ref:`to_uint64 (value: string implicit; hex: bool = false) : uint64 <function-_at_strings_c__c_to_uint64_CIs_Cb>` 
  *  :ref:`to_float (value: string implicit) : float <function-_at_strings_c__c_to_float_CIs>` 
  *  :ref:`to_double (value: string implicit) : double <function-_at_strings_c__c_to_double_CIs>` 
  *  :ref:`int (str: string implicit; result: ConversionResult& implicit; offset: int& implicit; hex: bool = false) : int <function-_at_strings_c__c_int_CIs_&IE_ls_strings_c__c_ConversionResult_gr__&Ii_Cb>` 
  *  :ref:`uint (str: string implicit; result: ConversionResult& implicit; offset: int& implicit; hex: bool = false) : uint <function-_at_strings_c__c_uint_CIs_&IE_ls_strings_c__c_ConversionResult_gr__&Ii_Cb>` 
  *  :ref:`int64 (str: string implicit; result: ConversionResult& implicit; offset: int& implicit; hex: bool = false) : int64 <function-_at_strings_c__c_int64_CIs_&IE_ls_strings_c__c_ConversionResult_gr__&Ii_Cb>` 
  *  :ref:`uint64 (str: string implicit; result: ConversionResult& implicit; offset: int& implicit; hex: bool = false) : uint64 <function-_at_strings_c__c_uint64_CIs_&IE_ls_strings_c__c_ConversionResult_gr__&Ii_Cb>` 
  *  :ref:`float (str: string implicit; result: ConversionResult& implicit; offset: int& implicit) : float <function-_at_strings_c__c_float_CIs_&IE_ls_strings_c__c_ConversionResult_gr__&Ii>` 
  *  :ref:`double (str: string implicit; result: ConversionResult& implicit; offset: int& implicit) : double <function-_at_strings_c__c_double_CIs_&IE_ls_strings_c__c_ConversionResult_gr__&Ii>` 

.. _function-_at_strings_c__c_string_CI1_ls_u8_gr_A_C_c_C_l:

.. das:function:: string(bytes: array<uint8> implicit) : string

// stub



:Arguments: * **bytes** : array<uint8> implicit

.. _function-_at_strings_c__c_to_char_Ci_C_c_C_l:

.. das:function:: to_char(char: int) : string

// stub



:Arguments: * **char** : int

.. _function-_at_strings_c__c_int_CIs_C_c_C_l:

.. das:function:: int(str: string implicit) : int

// stub



:Arguments: * **str** : string implicit

.. _function-_at_strings_c__c_uint_CIs_C_c_C_l:

.. das:function:: uint(str: string implicit) : uint

// stub



:Arguments: * **str** : string implicit

.. _function-_at_strings_c__c_int64_CIs_C_c_C_l:

.. das:function:: int64(str: string implicit) : int64

// stub



:Arguments: * **str** : string implicit

.. _function-_at_strings_c__c_uint64_CIs_C_c_C_l:

.. das:function:: uint64(str: string implicit) : uint64

// stub



:Arguments: * **str** : string implicit

.. _function-_at_strings_c__c_float_CIs_C_c_C_l:

.. das:function:: float(str: string implicit) : float

// stub



:Arguments: * **str** : string implicit

.. _function-_at_strings_c__c_double_CIs_C_c_C_l:

.. das:function:: double(str: string implicit) : double

// stub



:Arguments: * **str** : string implicit

.. _function-_at_strings_c__c_to_int_CIs_Cb:

.. das:function:: to_int(value: string implicit; hex: bool = false) : int

// stub



:Arguments: * **value** : string implicit

            * **hex** : bool

.. _function-_at_strings_c__c_to_uint_CIs_Cb:

.. das:function:: to_uint(value: string implicit; hex: bool = false) : uint

// stub



:Arguments: * **value** : string implicit

            * **hex** : bool

.. _function-_at_strings_c__c_to_int64_CIs_Cb:

.. das:function:: to_int64(value: string implicit; hex: bool = false) : int64

// stub



:Arguments: * **value** : string implicit

            * **hex** : bool

.. _function-_at_strings_c__c_to_uint64_CIs_Cb:

.. das:function:: to_uint64(value: string implicit; hex: bool = false) : uint64

// stub



:Arguments: * **value** : string implicit

            * **hex** : bool

.. _function-_at_strings_c__c_to_float_CIs:

.. das:function:: to_float(value: string implicit) : float

// stub



:Arguments: * **value** : string implicit

.. _function-_at_strings_c__c_to_double_CIs:

.. das:function:: to_double(value: string implicit) : double

// stub



:Arguments: * **value** : string implicit

.. _function-_at_strings_c__c_int_CIs_&IE_ls_strings_c__c_ConversionResult_gr__&Ii_Cb:

.. das:function:: int(str: string implicit; result: ConversionResult& implicit; offset: int& implicit; hex: bool = false) : int

// stub



:Arguments: * **str** : string implicit

            * **result** :  :ref:`ConversionResult <enum-strings-ConversionResult>` & implicit

            * **offset** : int& implicit

            * **hex** : bool

.. _function-_at_strings_c__c_uint_CIs_&IE_ls_strings_c__c_ConversionResult_gr__&Ii_Cb:

.. das:function:: uint(str: string implicit; result: ConversionResult& implicit; offset: int& implicit; hex: bool = false) : uint

// stub



:Arguments: * **str** : string implicit

            * **result** :  :ref:`ConversionResult <enum-strings-ConversionResult>` & implicit

            * **offset** : int& implicit

            * **hex** : bool

.. _function-_at_strings_c__c_int64_CIs_&IE_ls_strings_c__c_ConversionResult_gr__&Ii_Cb:

.. das:function:: int64(str: string implicit; result: ConversionResult& implicit; offset: int& implicit; hex: bool = false) : int64

// stub



:Arguments: * **str** : string implicit

            * **result** :  :ref:`ConversionResult <enum-strings-ConversionResult>` & implicit

            * **offset** : int& implicit

            * **hex** : bool

.. _function-_at_strings_c__c_uint64_CIs_&IE_ls_strings_c__c_ConversionResult_gr__&Ii_Cb:

.. das:function:: uint64(str: string implicit; result: ConversionResult& implicit; offset: int& implicit; hex: bool = false) : uint64

// stub



:Arguments: * **str** : string implicit

            * **result** :  :ref:`ConversionResult <enum-strings-ConversionResult>` & implicit

            * **offset** : int& implicit

            * **hex** : bool

.. _function-_at_strings_c__c_float_CIs_&IE_ls_strings_c__c_ConversionResult_gr__&Ii:

.. das:function:: float(str: string implicit; result: ConversionResult& implicit; offset: int& implicit) : float

// stub



:Arguments: * **str** : string implicit

            * **result** :  :ref:`ConversionResult <enum-strings-ConversionResult>` & implicit

            * **offset** : int& implicit

.. _function-_at_strings_c__c_double_CIs_&IE_ls_strings_c__c_ConversionResult_gr__&Ii:

.. das:function:: double(str: string implicit; result: ConversionResult& implicit; offset: int& implicit) : double

// stub



:Arguments: * **str** : string implicit

            * **result** :  :ref:`ConversionResult <enum-strings-ConversionResult>` & implicit

            * **offset** : int& implicit

+++++++++++++++
String as array
+++++++++++++++

  *  :ref:`peek_data (str: string implicit; block: block\<(arg0:array\<uint8\> const#):void\> implicit) <function-_at_strings_c__c_peek_data_CIs_CI0_ls_C_hh_1_ls_u8_gr_A_gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`modify_data (str: string implicit; block: block\<(var arg0:array\<uint8\>#):void\> implicit) : string <function-_at_strings_c__c_modify_data_CIs_CI0_ls__hh_1_ls_u8_gr_A_gr_1_ls_v_gr__builtin__C_c_C_l>` 

.. _function-_at_strings_c__c_peek_data_CIs_CI0_ls_C_hh_1_ls_u8_gr_A_gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: peek_data(str: string implicit; block: block<(arg0:array<uint8> const#):void> implicit)

// stub



:Arguments: * **str** : string implicit

            * **block** : block<(array<uint8>#):void> implicit

.. _function-_at_strings_c__c_modify_data_CIs_CI0_ls__hh_1_ls_u8_gr_A_gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: modify_data(str: string implicit; block: block<(var arg0:array<uint8>#):void> implicit) : string

// stub



:Arguments: * **str** : string implicit

            * **block** : block<(array<uint8>#):void> implicit

+++++++++++++++++++++++++++
Low level memory allocation
+++++++++++++++++++++++++++

  *  :ref:`delete_string (str: string& implicit) <function-_at_strings_c__c_delete_string_&Is_C_c_C_l>` 
  *  :ref:`reserve_string_buffer (str: string implicit; length: int) : string <function-_at_strings_c__c_reserve_string_buffer_CIs_Ci_C_c>` 

.. _function-_at_strings_c__c_delete_string_&Is_C_c_C_l:

.. das:function:: delete_string(str: string& implicit)

.. warning:: 
  This is unsafe operation.

// stub



:Arguments: * **str** : string& implicit

.. _function-_at_strings_c__c_reserve_string_buffer_CIs_Ci_C_c:

.. das:function:: reserve_string_buffer(str: string implicit; length: int) : string

// stub



:Arguments: * **str** : string implicit

            * **length** : int

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_strings_c__c_fmt_IH_ls_strings_c__c_StringBuilderWriter_gr__CIs_Ci8_C_c_C_l:

.. das:function:: fmt(writer: StringBuilderWriter implicit; format: string implicit; value: int8) : StringBuilderWriter&

// stub



:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **format** : string implicit

            * **value** : int8

.. _function-_at_strings_c__c_fmt_IH_ls_strings_c__c_StringBuilderWriter_gr__CIs_Cu8_C_c_C_l:

.. das:function:: fmt(writer: StringBuilderWriter implicit; format: string implicit; value: uint8) : StringBuilderWriter&

// stub



:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **format** : string implicit

            * **value** : uint8

.. _function-_at_strings_c__c_fmt_IH_ls_strings_c__c_StringBuilderWriter_gr__CIs_Ci16_C_c_C_l:

.. das:function:: fmt(writer: StringBuilderWriter implicit; format: string implicit; value: int16) : StringBuilderWriter&

// stub



:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **format** : string implicit

            * **value** : int16

.. _function-_at_strings_c__c_fmt_IH_ls_strings_c__c_StringBuilderWriter_gr__CIs_Cu16_C_c_C_l:

.. das:function:: fmt(writer: StringBuilderWriter implicit; format: string implicit; value: uint16) : StringBuilderWriter&

// stub



:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **format** : string implicit

            * **value** : uint16

.. _function-_at_strings_c__c_fmt_IH_ls_strings_c__c_StringBuilderWriter_gr__CIs_Ci_C_c_C_l:

.. das:function:: fmt(writer: StringBuilderWriter implicit; format: string implicit; value: int) : StringBuilderWriter&

// stub



:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **format** : string implicit

            * **value** : int

.. _function-_at_strings_c__c_fmt_IH_ls_strings_c__c_StringBuilderWriter_gr__CIs_Cu_C_c_C_l:

.. das:function:: fmt(writer: StringBuilderWriter implicit; format: string implicit; value: uint) : StringBuilderWriter&

// stub



:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **format** : string implicit

            * **value** : uint

.. _function-_at_strings_c__c_fmt_IH_ls_strings_c__c_StringBuilderWriter_gr__CIs_Ci64_C_c_C_l:

.. das:function:: fmt(writer: StringBuilderWriter implicit; format: string implicit; value: int64) : StringBuilderWriter&

// stub



:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **format** : string implicit

            * **value** : int64

.. _function-_at_strings_c__c_fmt_IH_ls_strings_c__c_StringBuilderWriter_gr__CIs_Cu64_C_c_C_l:

.. das:function:: fmt(writer: StringBuilderWriter implicit; format: string implicit; value: uint64) : StringBuilderWriter&

// stub



:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **format** : string implicit

            * **value** : uint64

.. _function-_at_strings_c__c_fmt_IH_ls_strings_c__c_StringBuilderWriter_gr__CIs_Cf_C_c_C_l:

.. das:function:: fmt(writer: StringBuilderWriter implicit; format: string implicit; value: float) : StringBuilderWriter&

// stub



:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **format** : string implicit

            * **value** : float

.. _function-_at_strings_c__c_fmt_IH_ls_strings_c__c_StringBuilderWriter_gr__CIs_Cd_C_c_C_l:

.. das:function:: fmt(writer: StringBuilderWriter implicit; format: string implicit; value: double) : StringBuilderWriter&

// stub



:Arguments: * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit

            * **format** : string implicit

            * **value** : double

.. _function-_at_strings_c__c_int8_CIs_C_c_C_l:

.. das:function:: int8(str: string implicit) : int8

// stub



:Arguments: * **str** : string implicit

.. _function-_at_strings_c__c_uint8_CIs_C_c_C_l:

.. das:function:: uint8(str: string implicit) : uint8

// stub



:Arguments: * **str** : string implicit

.. _function-_at_strings_c__c_int16_CIs_C_c_C_l:

.. das:function:: int16(str: string implicit) : int16

// stub



:Arguments: * **str** : string implicit

.. _function-_at_strings_c__c_uint16_CIs_C_c_C_l:

.. das:function:: uint16(str: string implicit) : uint16

// stub



:Arguments: * **str** : string implicit

.. _function-_at_strings_c__c_to_int8_CIs_Cb:

.. das:function:: to_int8(value: string implicit; hex: bool = false) : int8

// stub



:Arguments: * **value** : string implicit

            * **hex** : bool

.. _function-_at_strings_c__c_to_uint8_CIs_Cb:

.. das:function:: to_uint8(value: string implicit; hex: bool = false) : uint8

// stub



:Arguments: * **value** : string implicit

            * **hex** : bool

.. _function-_at_strings_c__c_to_int16_CIs_Cb:

.. das:function:: to_int16(value: string implicit; hex: bool = false) : int16

// stub



:Arguments: * **value** : string implicit

            * **hex** : bool

.. _function-_at_strings_c__c_int8_CIs_&IE_ls_strings_c__c_ConversionResult_gr__&Ii_Cb:

.. das:function:: int8(str: string implicit; result: ConversionResult& implicit; offset: int& implicit; hex: bool = false) : int8

// stub



:Arguments: * **str** : string implicit

            * **result** :  :ref:`ConversionResult <enum-strings-ConversionResult>` & implicit

            * **offset** : int& implicit

            * **hex** : bool

.. _function-_at_strings_c__c_uint8_CIs_&IE_ls_strings_c__c_ConversionResult_gr__&Ii_Cb:

.. das:function:: uint8(str: string implicit; result: ConversionResult& implicit; offset: int& implicit; hex: bool = false) : uint8

// stub



:Arguments: * **str** : string implicit

            * **result** :  :ref:`ConversionResult <enum-strings-ConversionResult>` & implicit

            * **offset** : int& implicit

            * **hex** : bool

.. _function-_at_strings_c__c_int16_CIs_&IE_ls_strings_c__c_ConversionResult_gr__&Ii_Cb:

.. das:function:: int16(str: string implicit; result: ConversionResult& implicit; offset: int& implicit; hex: bool = false) : int16

// stub



:Arguments: * **str** : string implicit

            * **result** :  :ref:`ConversionResult <enum-strings-ConversionResult>` & implicit

            * **offset** : int& implicit

            * **hex** : bool

.. _function-_at_strings_c__c_uint16_CIs_&IE_ls_strings_c__c_ConversionResult_gr__&Ii_Cb:

.. das:function:: uint16(str: string implicit; result: ConversionResult& implicit; offset: int& implicit; hex: bool = false) : uint16

// stub



:Arguments: * **str** : string implicit

            * **result** :  :ref:`ConversionResult <enum-strings-ConversionResult>` & implicit

            * **offset** : int& implicit

            * **hex** : bool

.. _function-_at_strings_c__c_fmt_CIs_Ci8_C_c_C_l:

.. das:function:: fmt(format: string implicit; value: int8) : string

// stub



:Arguments: * **format** : string implicit

            * **value** : int8

.. _function-_at_strings_c__c_fmt_CIs_Cu8_C_c_C_l:

.. das:function:: fmt(format: string implicit; value: uint8) : string

// stub



:Arguments: * **format** : string implicit

            * **value** : uint8

.. _function-_at_strings_c__c_fmt_CIs_Ci16_C_c_C_l:

.. das:function:: fmt(format: string implicit; value: int16) : string

// stub



:Arguments: * **format** : string implicit

            * **value** : int16

.. _function-_at_strings_c__c_fmt_CIs_Cu16_C_c_C_l:

.. das:function:: fmt(format: string implicit; value: uint16) : string

// stub



:Arguments: * **format** : string implicit

            * **value** : uint16

.. _function-_at_strings_c__c_fmt_CIs_Ci_C_c_C_l:

.. das:function:: fmt(format: string implicit; value: int) : string

// stub



:Arguments: * **format** : string implicit

            * **value** : int

.. _function-_at_strings_c__c_fmt_CIs_Cu_C_c_C_l:

.. das:function:: fmt(format: string implicit; value: uint) : string

// stub



:Arguments: * **format** : string implicit

            * **value** : uint

.. _function-_at_strings_c__c_fmt_CIs_Ci64_C_c_C_l:

.. das:function:: fmt(format: string implicit; value: int64) : string

// stub



:Arguments: * **format** : string implicit

            * **value** : int64

.. _function-_at_strings_c__c_fmt_CIs_Cu64_C_c_C_l:

.. das:function:: fmt(format: string implicit; value: uint64) : string

// stub



:Arguments: * **format** : string implicit

            * **value** : uint64

.. _function-_at_strings_c__c_fmt_CIs_Cf_C_c_C_l:

.. das:function:: fmt(format: string implicit; value: float) : string

// stub



:Arguments: * **format** : string implicit

            * **value** : float

.. _function-_at_strings_c__c_fmt_CIs_Cd_C_c_C_l:

.. das:function:: fmt(format: string implicit; value: double) : string

// stub



:Arguments: * **format** : string implicit

            * **value** : double


