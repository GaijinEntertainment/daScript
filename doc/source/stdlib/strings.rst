
.. _stdlib_strings:

===========================
String manipulation library
===========================

.. include:: detail/strings.rst

|module-strings|

++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-strings-StringBuilderWriter:

.. das:attribute:: StringBuilderWriter

|structure_annotation-strings-StringBuilderWriter|

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_strings_c__c_append__hh_handle_hh_das_string_hh_implicit_int_hh_const:

.. das:function:: append(arg0: das_string implicit; arg1: int const)

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+arg0    + :ref:`builtin::das_string <handle-builtin-das_string>`  implicit+
+--------+-----------------------------------------------------------------+
+arg1    +int const                                                        +
+--------+-----------------------------------------------------------------+


|function-strings-append|

.. _function-_at_strings_c__c_as_string__hh_array_hh_uint8_hh_const_hh_implicit__hh_const:

.. das:function:: as_string(arg0: array<uint8> const implicit)

as_string returns string

+--------+---------------------------+
+argument+argument type              +
+========+===========================+
+arg0    +array<uint8> const implicit+
+--------+---------------------------+


|function-strings-as_string|

.. _function-_at_strings_c__c_build_string__hh_block_hh__hh_handle_hh_StringBuilderWriter_hh__c_void_hh_const_hh_implicit__hh_const:

.. das:function:: build_string(arg0: block<(strings::StringBuilderWriter):void> const implicit)

build_string returns string

+--------+-------------------------------------------------------------------------------------------------------+
+argument+argument type                                                                                          +
+========+=======================================================================================================+
+arg0    +block<( :ref:`strings::StringBuilderWriter <handle-strings-StringBuilderWriter>` ):void> const implicit+
+--------+-------------------------------------------------------------------------------------------------------+


|function-strings-build_string|

.. _function-_at_strings_c__c_builtin_strdup_any:

.. das:function:: builtin_strdup(arg0: any)

.. warning:: 
  This is unsafe operation.

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +any          +
+--------+-------------+


|function-strings-builtin_strdup|

.. _function-_at_strings_c__c_builtin_string_split_string_hh_const_string_hh_const__hh_block_hh_const_hh_implicit__hh_const:

.. das:function:: builtin_string_split(arg0: string const; arg1: string const; arg2: block<> const implicit)

+--------+----------------------+
+argument+argument type         +
+========+======================+
+arg0    +string const          +
+--------+----------------------+
+arg1    +string const          +
+--------+----------------------+
+arg2    +block<> const implicit+
+--------+----------------------+


|function-strings-builtin_string_split|

.. _function-_at_strings_c__c_builtin_string_split_by_char_string_hh_const_string_hh_const__hh_block_hh_const_hh_implicit__hh_const:

.. das:function:: builtin_string_split_by_char(arg0: string const; arg1: string const; arg2: block<> const implicit)

+--------+----------------------+
+argument+argument type         +
+========+======================+
+arg0    +string const          +
+--------+----------------------+
+arg1    +string const          +
+--------+----------------------+
+arg2    +block<> const implicit+
+--------+----------------------+


|function-strings-builtin_string_split_by_char|

.. _function-_at_strings_c__c_character_at_string_hh_const_int_hh_const__hh_const:

.. das:function:: character_at(arg0: string const; arg1: int const)

character_at returns int

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+
+arg1    +int const    +
+--------+-------------+


|function-strings-character_at|

.. _function-_at_strings_c__c_character_uat_string_hh_const_int_hh_const:

.. das:function:: character_uat(arg0: string const; arg1: int const)

character_uat returns int

.. warning:: 
  This is unsafe operation.

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+
+arg1    +int const    +
+--------+-------------+


|function-strings-character_uat|

.. _function-_at_strings_c__c_chop_string_hh_const_int_hh_const_int_hh_const__hh_const:

.. das:function:: chop(arg0: string const; arg1: int const; arg2: int const)

chop returns string

.. warning:: 
  This is unsafe operation.

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+
+arg1    +int const    +
+--------+-------------+
+arg2    +int const    +
+--------+-------------+


|function-strings-chop|

.. _function-_at_strings_c__c_delete_string_string_hh_ref_hh_implicit__hh_const:

.. das:function:: delete_string(arg0: string& implicit)

.. warning:: 
  This is unsafe operation.

+--------+----------------+
+argument+argument type   +
+========+================+
+arg0    +string& implicit+
+--------+----------------+


|function-strings-delete_string|

.. _function-_at_strings_c__c_double_string_hh_const__hh_const:

.. das:function:: double(arg0: string const)

double returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+


|function-strings-double|

.. _function-_at_strings_c__c_empty__hh_handle_hh_das_string_hh_const_hh_implicit:

.. das:function:: empty(arg0: das_string const implicit)

empty returns bool

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+arg0    + :ref:`builtin::das_string <handle-builtin-das_string>`  const implicit+
+--------+-----------------------------------------------------------------------+


|function-strings-empty|

.. _function-_at_strings_c__c_empty_string_hh_const:

.. das:function:: empty(arg0: string const)

empty returns bool

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+


|function-strings-empty|

.. _function-_at_strings_c__c_ends_with__hh_handle_hh_das_string_hh_const_hh_implicit_string_hh_const__hh_const:

.. das:function:: ends_with(arg0: das_string const implicit; arg1: string const)

ends_with returns bool

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+arg0    + :ref:`builtin::das_string <handle-builtin-das_string>`  const implicit+
+--------+-----------------------------------------------------------------------+
+arg1    +string const                                                           +
+--------+-----------------------------------------------------------------------+


|function-strings-ends_with|

.. _function-_at_strings_c__c_ends_with_string_hh_const_string_hh_const__hh_const:

.. das:function:: ends_with(arg0: string const; arg1: string const)

ends_with returns bool

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+
+arg1    +string const +
+--------+-------------+


|function-strings-ends_with|

.. _function-_at_strings_c__c_escape_string_hh_const__hh_const:

.. das:function:: escape(arg0: string const)

escape returns string

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+


|function-strings-escape|

.. _function-_at_strings_c__c_find_string_hh_const_string_hh_const:

.. das:function:: find(arg0: string const; arg1: string const)

find returns int

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+
+arg1    +string const +
+--------+-------------+


|function-strings-find|

.. _function-_at_strings_c__c_find_string_hh_const_string_hh_const_int_hh_const__hh_const:

.. das:function:: find(arg0: string const; arg1: string const; arg2: int const)

find returns int

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+
+arg1    +string const +
+--------+-------------+
+arg2    +int const    +
+--------+-------------+


|function-strings-find|

.. _function-_at_strings_c__c_find_first_of_string_hh_const_int_hh_const__hh_const:

.. das:function:: find_first_of(arg0: string const; arg1: int const)

find_first_of returns int

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+
+arg1    +int const    +
+--------+-------------+


|function-strings-find_first_of|

.. _function-_at_strings_c__c_find_first_of_string_hh_const_string_hh_const__hh_const:

.. das:function:: find_first_of(arg0: string const; arg1: string const)

find_first_of returns int

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+
+arg1    +string const +
+--------+-------------+


|function-strings-find_first_of|

.. _function-_at_strings_c__c_float_string_hh_const__hh_const:

.. das:function:: float(arg0: string const)

float returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+


|function-strings-float|

.. _function-_at_strings_c__c_format__hh_handle_hh_StringBuilderWriter_hh_implicit_string_hh_const_double_hh_const:

.. das:function:: format(arg0: StringBuilderWriter implicit; arg1: string const; arg2: double const)

+--------+-----------------------------------------------------------------------------------+
+argument+argument type                                                                      +
+========+===================================================================================+
+arg0    + :ref:`strings::StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit+
+--------+-----------------------------------------------------------------------------------+
+arg1    +string const                                                                       +
+--------+-----------------------------------------------------------------------------------+
+arg2    +double const                                                                       +
+--------+-----------------------------------------------------------------------------------+


|function-strings-format|

.. _function-_at_strings_c__c_format__hh_handle_hh_StringBuilderWriter_hh_implicit_string_hh_const_float_hh_const:

.. das:function:: format(arg0: StringBuilderWriter implicit; arg1: string const; arg2: float const)

+--------+-----------------------------------------------------------------------------------+
+argument+argument type                                                                      +
+========+===================================================================================+
+arg0    + :ref:`strings::StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit+
+--------+-----------------------------------------------------------------------------------+
+arg1    +string const                                                                       +
+--------+-----------------------------------------------------------------------------------+
+arg2    +float const                                                                        +
+--------+-----------------------------------------------------------------------------------+


|function-strings-format|

.. _function-_at_strings_c__c_format__hh_handle_hh_StringBuilderWriter_hh_implicit_string_hh_const_int_hh_const:

.. das:function:: format(arg0: StringBuilderWriter implicit; arg1: string const; arg2: int const)

+--------+-----------------------------------------------------------------------------------+
+argument+argument type                                                                      +
+========+===================================================================================+
+arg0    + :ref:`strings::StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit+
+--------+-----------------------------------------------------------------------------------+
+arg1    +string const                                                                       +
+--------+-----------------------------------------------------------------------------------+
+arg2    +int const                                                                          +
+--------+-----------------------------------------------------------------------------------+


|function-strings-format|

.. _function-_at_strings_c__c_format__hh_handle_hh_StringBuilderWriter_hh_implicit_string_hh_const_int64_hh_const:

.. das:function:: format(arg0: StringBuilderWriter implicit; arg1: string const; arg2: int64 const)

+--------+-----------------------------------------------------------------------------------+
+argument+argument type                                                                      +
+========+===================================================================================+
+arg0    + :ref:`strings::StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit+
+--------+-----------------------------------------------------------------------------------+
+arg1    +string const                                                                       +
+--------+-----------------------------------------------------------------------------------+
+arg2    +int64 const                                                                        +
+--------+-----------------------------------------------------------------------------------+


|function-strings-format|

.. _function-_at_strings_c__c_format__hh_handle_hh_StringBuilderWriter_hh_implicit_string_hh_const_uint_hh_const:

.. das:function:: format(arg0: StringBuilderWriter implicit; arg1: string const; arg2: uint const)

+--------+-----------------------------------------------------------------------------------+
+argument+argument type                                                                      +
+========+===================================================================================+
+arg0    + :ref:`strings::StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit+
+--------+-----------------------------------------------------------------------------------+
+arg1    +string const                                                                       +
+--------+-----------------------------------------------------------------------------------+
+arg2    +uint const                                                                         +
+--------+-----------------------------------------------------------------------------------+


|function-strings-format|

.. _function-_at_strings_c__c_format__hh_handle_hh_StringBuilderWriter_hh_implicit_string_hh_const_uint64_hh_const:

.. das:function:: format(arg0: StringBuilderWriter implicit; arg1: string const; arg2: uint64 const)

+--------+-----------------------------------------------------------------------------------+
+argument+argument type                                                                      +
+========+===================================================================================+
+arg0    + :ref:`strings::StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit+
+--------+-----------------------------------------------------------------------------------+
+arg1    +string const                                                                       +
+--------+-----------------------------------------------------------------------------------+
+arg2    +uint64 const                                                                       +
+--------+-----------------------------------------------------------------------------------+


|function-strings-format|

.. _function-_at_strings_c__c_format_string_hh_const_double_hh_const__hh_const:

.. das:function:: format(arg0: string const; arg1: double const)

format returns string

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+
+arg1    +double const +
+--------+-------------+


|function-strings-format|

.. _function-_at_strings_c__c_format_string_hh_const_float_hh_const__hh_const:

.. das:function:: format(arg0: string const; arg1: float const)

format returns string

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+
+arg1    +float const  +
+--------+-------------+


|function-strings-format|

.. _function-_at_strings_c__c_format_string_hh_const_int_hh_const__hh_const:

.. das:function:: format(arg0: string const; arg1: int const)

format returns string

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+
+arg1    +int const    +
+--------+-------------+


|function-strings-format|

.. _function-_at_strings_c__c_format_string_hh_const_int64_hh_const__hh_const:

.. das:function:: format(arg0: string const; arg1: int64 const)

format returns string

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+
+arg1    +int64 const  +
+--------+-------------+


|function-strings-format|

.. _function-_at_strings_c__c_format_string_hh_const_uint_hh_const__hh_const:

.. das:function:: format(arg0: string const; arg1: uint const)

format returns string

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+
+arg1    +uint const   +
+--------+-------------+


|function-strings-format|

.. _function-_at_strings_c__c_format_string_hh_const_uint64_hh_const__hh_const:

.. das:function:: format(arg0: string const; arg1: uint64 const)

format returns string

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+
+arg1    +uint64 const +
+--------+-------------+


|function-strings-format|

.. _function-_at_strings_c__c_int_string_hh_const__hh_const:

.. das:function:: int(arg0: string const)

int returns int

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+


|function-strings-int|

.. _function-_at_strings_c__c_is_alpha_int_hh_const:

.. das:function:: is_alpha(arg0: int const)

is_alpha returns bool

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +int const    +
+--------+-------------+


|function-strings-is_alpha|

.. _function-_at_strings_c__c_is_char_in_set_int_hh_const_uint_hh_const_hh_implicit_hh_8:

.. das:function:: is_char_in_set(arg0: int const; arg1: uint const[8] implicit)

is_char_in_set returns bool

+--------+----------------------+
+argument+argument type         +
+========+======================+
+arg0    +int const             +
+--------+----------------------+
+arg1    +uint const[8] implicit+
+--------+----------------------+


|function-strings-is_char_in_set|

.. _function-_at_strings_c__c_is_new_line_int_hh_const:

.. das:function:: is_new_line(arg0: int const)

is_new_line returns bool

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +int const    +
+--------+-------------+


|function-strings-is_new_line|

.. _function-_at_strings_c__c_is_number_int_hh_const:

.. das:function:: is_number(arg0: int const)

is_number returns bool

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +int const    +
+--------+-------------+


|function-strings-is_number|

.. _function-_at_strings_c__c_is_white_space_int_hh_const:

.. das:function:: is_white_space(arg0: int const)

is_white_space returns bool

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +int const    +
+--------+-------------+


|function-strings-is_white_space|

.. _function-_at_strings_c__c_length__hh_handle_hh_das_string_hh_implicit:

.. das:function:: length(arg0: das_string implicit)

length returns int

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+arg0    + :ref:`builtin::das_string <handle-builtin-das_string>`  implicit+
+--------+-----------------------------------------------------------------+


|function-strings-length|

.. _function-_at_strings_c__c_length_string_hh_const__hh_const:

.. das:function:: length(arg0: string const)

length returns int

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+


|function-strings-length|

.. _function-_at_strings_c__c_repeat_string_hh_const_int_hh_const__hh_const:

.. das:function:: repeat(arg0: string const; arg1: int const)

repeat returns string

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+
+arg1    +int const    +
+--------+-------------+


|function-strings-repeat|

.. _function-_at_strings_c__c_replace_string_hh_const_string_hh_const_string_hh_const__hh_const:

.. das:function:: replace(arg0: string const; arg1: string const; arg2: string const)

replace returns string

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+
+arg1    +string const +
+--------+-------------+
+arg2    +string const +
+--------+-------------+


|function-strings-replace|

.. _function-_at_strings_c__c_resize__hh_handle_hh_das_string_hh_implicit_int_hh_const:

.. das:function:: resize(arg0: das_string implicit; arg1: int const)

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+arg0    + :ref:`builtin::das_string <handle-builtin-das_string>`  implicit+
+--------+-----------------------------------------------------------------+
+arg1    +int const                                                        +
+--------+-----------------------------------------------------------------+


|function-strings-resize|

.. _function-_at_strings_c__c_reverse_string_hh_const__hh_const:

.. das:function:: reverse(arg0: string const)

reverse returns string

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+


|function-strings-reverse|

.. _function-_at_strings_c__c_slice_string_hh_const_int_hh_const__hh_const:

.. das:function:: slice(arg0: string const; arg1: int const)

slice returns string

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+
+arg1    +int const    +
+--------+-------------+


|function-strings-slice|

.. _function-_at_strings_c__c_slice_string_hh_const_int_hh_const_int_hh_const__hh_const:

.. das:function:: slice(arg0: string const; arg1: int const; arg2: int const)

slice returns string

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+
+arg1    +int const    +
+--------+-------------+
+arg2    +int const    +
+--------+-------------+


|function-strings-slice|

.. _function-_at_strings_c__c_starts_with_string_hh_const_string_hh_const__hh_const:

.. das:function:: starts_with(arg0: string const; arg1: string const)

starts_with returns bool

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+
+arg1    +string const +
+--------+-------------+


|function-strings-starts_with|

.. _function-_at_strings_c__c_string__hh_array_hh_uint8_hh_const_hh_implicit__hh_const:

.. das:function:: string(arg0: array<uint8> const implicit)

string returns string

+--------+---------------------------+
+argument+argument type              +
+========+===========================+
+arg0    +array<uint8> const implicit+
+--------+---------------------------+


|function-strings-string|

.. _function-_at_strings_c__c_strip_string_hh_const__hh_const:

.. das:function:: strip(arg0: string const)

strip returns string

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+


|function-strings-strip|

.. _function-_at_strings_c__c_strip_left_string_hh_const__hh_const:

.. das:function:: strip_left(arg0: string const)

strip_left returns string

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+


|function-strings-strip_left|

.. _function-_at_strings_c__c_strip_right_string_hh_const__hh_const:

.. das:function:: strip_right(arg0: string const)

strip_right returns string

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+


|function-strings-strip_right|

.. _function-_at_strings_c__c_to_char_int_hh_const__hh_const:

.. das:function:: to_char(arg0: int const)

to_char returns string

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +int const    +
+--------+-------------+


|function-strings-to_char|

.. _function-_at_strings_c__c_to_float_string_hh_const:

.. das:function:: to_float(arg0: string const)

to_float returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+


|function-strings-to_float|

.. _function-_at_strings_c__c_to_int_string_hh_const:

.. das:function:: to_int(arg0: string const)

to_int returns int

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+


|function-strings-to_int|

.. _function-_at_strings_c__c_to_lower_string_hh_const__hh_const:

.. das:function:: to_lower(arg0: string const)

to_lower returns string

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+


|function-strings-to_lower|

.. _function-_at_strings_c__c_to_lower_in_place_string_hh_const:

.. das:function:: to_lower_in_place(arg0: string const)

to_lower_in_place returns string

.. warning:: 
  This is unsafe operation.

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+


|function-strings-to_lower_in_place|

.. _function-_at_strings_c__c_to_upper_string_hh_const__hh_const:

.. das:function:: to_upper(arg0: string const)

to_upper returns string

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+


|function-strings-to_upper|

.. _function-_at_strings_c__c_to_upper_in_place_string_hh_const:

.. das:function:: to_upper_in_place(arg0: string const)

to_upper_in_place returns string

.. warning:: 
  This is unsafe operation.

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+


|function-strings-to_upper_in_place|

.. _function-_at_strings_c__c_uint_string_hh_const__hh_const:

.. das:function:: uint(arg0: string const)

uint returns uint

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+


|function-strings-uint|

.. _function-_at_strings_c__c_unescape_string_hh_const__hh_const:

.. das:function:: unescape(arg0: string const)

unescape returns string

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +string const +
+--------+-------------+


|function-strings-unescape|

.. _function-_at_strings_c__c_write__hh_handle_hh_StringBuilderWriter_any:

.. das:function:: write(arg0: StringBuilderWriter; arg1: any)

+--------+--------------------------------------------------------------------------+
+argument+argument type                                                             +
+========+==========================================================================+
+arg0    + :ref:`strings::StringBuilderWriter <handle-strings-StringBuilderWriter>` +
+--------+--------------------------------------------------------------------------+
+arg1    +any                                                                       +
+--------+--------------------------------------------------------------------------+


|function-strings-write|

.. _function-_at_strings_c__c_write_char__hh_handle_hh_StringBuilderWriter_hh_implicit_int_hh_const:

.. das:function:: write_char(arg0: StringBuilderWriter implicit; arg1: int const)

+--------+-----------------------------------------------------------------------------------+
+argument+argument type                                                                      +
+========+===================================================================================+
+arg0    + :ref:`strings::StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit+
+--------+-----------------------------------------------------------------------------------+
+arg1    +int const                                                                          +
+--------+-----------------------------------------------------------------------------------+


|function-strings-write_char|

.. _function-_at_strings_c__c_write_chars__hh_handle_hh_StringBuilderWriter_hh_implicit_int_hh_const_int_hh_const:

.. das:function:: write_chars(arg0: StringBuilderWriter implicit; arg1: int const; arg2: int const)

+--------+-----------------------------------------------------------------------------------+
+argument+argument type                                                                      +
+========+===================================================================================+
+arg0    + :ref:`strings::StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit+
+--------+-----------------------------------------------------------------------------------+
+arg1    +int const                                                                          +
+--------+-----------------------------------------------------------------------------------+
+arg2    +int const                                                                          +
+--------+-----------------------------------------------------------------------------------+


|function-strings-write_chars|

.. _function-_at_strings_c__c_write_escape_string__hh_handle_hh_StringBuilderWriter_hh_implicit_string_hh_const:

.. das:function:: write_escape_string(arg0: StringBuilderWriter implicit; arg1: string const)

+--------+-----------------------------------------------------------------------------------+
+argument+argument type                                                                      +
+========+===================================================================================+
+arg0    + :ref:`strings::StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit+
+--------+-----------------------------------------------------------------------------------+
+arg1    +string const                                                                       +
+--------+-----------------------------------------------------------------------------------+


|function-strings-write_escape_string|


