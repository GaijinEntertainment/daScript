
.. _stdlib_strings:

===========================
String manipulation library
===========================

.. include:: detail/strings.rst

The string library implements string formatting, conversion, searching, and modification routines.

All functions and symbols are in "strings" module, use require to get access to it. ::

    require strings


++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-strings-StringBuilderWriter:

.. das:attribute:: StringBuilderWriter

|structure_annotation-strings-StringBuilderWriter|

++++++++++++++++++++++++++
Internal builtin functions
++++++++++++++++++++++++++

.. warning:: 
  This group of functions is hidden. It will not be in the final documentation.

  *  :ref:`builtin_strdup (arg0:any const) : void <function-_at_strings_c__c_builtin_strdup__hh_const_hh_any>` 
  *  :ref:`builtin_string_split (str:string const implicit;delimiter:string const implicit;block:block\<\> const implicit;context:__context const) : void <function-_at_strings_c__c_builtin_string_split__hh_const_hh__hh_implicit_hh_string__hh_const_hh__hh_implicit_hh_string__hh_const_hh__hh_implicit_hh__hh_block__hh_const_hh___context>` 
  *  :ref:`builtin_string_split_by_char (str:string const implicit;delimiter:string const implicit;block:block\<\> const implicit;context:__context const) : void <function-_at_strings_c__c_builtin_string_split_by_char__hh_const_hh__hh_implicit_hh_string__hh_const_hh__hh_implicit_hh_string__hh_const_hh__hh_implicit_hh__hh_block__hh_const_hh___context>` 

.. _function-_at_strings_c__c_builtin_strdup__hh_const_hh_any:

.. das:function:: builtin_strdup(arg0: any const)

.. warning:: 
  This is unsafe operation.

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +any const    +
+--------+-------------+


|function-strings-builtin_strdup|

.. _function-_at_strings_c__c_builtin_string_split__hh_const_hh__hh_implicit_hh_string__hh_const_hh__hh_implicit_hh_string__hh_const_hh__hh_implicit_hh__hh_block__hh_const_hh___context:

.. das:function:: builtin_string_split(str: string const implicit; delimiter: string const implicit; block: block<> const implicit)

+---------+----------------------+
+argument +argument type         +
+=========+======================+
+str      +string const implicit +
+---------+----------------------+
+delimiter+string const implicit +
+---------+----------------------+
+block    +block<> const implicit+
+---------+----------------------+


|function-strings-builtin_string_split|

.. _function-_at_strings_c__c_builtin_string_split_by_char__hh_const_hh__hh_implicit_hh_string__hh_const_hh__hh_implicit_hh_string__hh_const_hh__hh_implicit_hh__hh_block__hh_const_hh___context:

.. das:function:: builtin_string_split_by_char(str: string const implicit; delimiter: string const implicit; block: block<> const implicit)

+---------+----------------------+
+argument +argument type         +
+=========+======================+
+str      +string const implicit +
+---------+----------------------+
+delimiter+string const implicit +
+---------+----------------------+
+block    +block<> const implicit+
+---------+----------------------+


|function-strings-builtin_string_split_by_char|

++++++++++++++++
Character groups
++++++++++++++++

  *  :ref:`is_alpha (arg0:int const) : bool <function-_at_strings_c__c_is_alpha__hh_const_hh_int>` 
  *  :ref:`is_char_in_set (arg0:int const;arg1:uint const[8] implicit) : bool <function-_at_strings_c__c_is_char_in_set__hh_const_hh_int__hh_const_hh__hh_implicit_hh_uint_hh_8>` 
  *  :ref:`is_new_line (arg0:int const) : bool <function-_at_strings_c__c_is_new_line__hh_const_hh_int>` 
  *  :ref:`is_number (arg0:int const) : bool <function-_at_strings_c__c_is_number__hh_const_hh_int>` 
  *  :ref:`is_white_space (arg0:int const) : bool <function-_at_strings_c__c_is_white_space__hh_const_hh_int>` 

.. _function-_at_strings_c__c_is_alpha__hh_const_hh_int:

.. das:function:: is_alpha(arg0: int const)

is_alpha returns bool

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +int const    +
+--------+-------------+


|function-strings-is_alpha|

.. _function-_at_strings_c__c_is_char_in_set__hh_const_hh_int__hh_const_hh__hh_implicit_hh_uint_hh_8:

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

.. _function-_at_strings_c__c_is_new_line__hh_const_hh_int:

.. das:function:: is_new_line(arg0: int const)

is_new_line returns bool

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +int const    +
+--------+-------------+


|function-strings-is_new_line|

.. _function-_at_strings_c__c_is_number__hh_const_hh_int:

.. das:function:: is_number(arg0: int const)

is_number returns bool

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +int const    +
+--------+-------------+


|function-strings-is_number|

.. _function-_at_strings_c__c_is_white_space__hh_const_hh_int:

.. das:function:: is_white_space(arg0: int const)

is_white_space returns bool

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +int const    +
+--------+-------------+


|function-strings-is_white_space|

++++++++++++++++++
Character by index
++++++++++++++++++

  *  :ref:`character_at (str:string const implicit;idx:int const;context:__context const) : int <function-_at_strings_c__c_character_at__hh_const_hh__hh_implicit_hh_string__hh_const_hh_int__hh_const_hh___context>` 
  *  :ref:`character_uat (str:string const implicit;idx:int const) : int <function-_at_strings_c__c_character_uat__hh_const_hh__hh_implicit_hh_string__hh_const_hh_int>` 

.. _function-_at_strings_c__c_character_at__hh_const_hh__hh_implicit_hh_string__hh_const_hh_int__hh_const_hh___context:

.. das:function:: character_at(str: string const implicit; idx: int const)

character_at returns int

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+str     +string const implicit+
+--------+---------------------+
+idx     +int const            +
+--------+---------------------+


|function-strings-character_at|

.. _function-_at_strings_c__c_character_uat__hh_const_hh__hh_implicit_hh_string__hh_const_hh_int:

.. das:function:: character_uat(str: string const implicit; idx: int const)

character_uat returns int

.. warning:: 
  This is unsafe operation.

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+str     +string const implicit+
+--------+---------------------+
+idx     +int const            +
+--------+---------------------+


|function-strings-character_uat|

+++++++++++++++++
String properties
+++++++++++++++++

  *  :ref:`empty (arg0:$::das_string const implicit) : bool <function-_at_strings_c__c_empty__hh_const_hh__hh_implicit_hh__hh_handle_hh_das_string>` 
  *  :ref:`empty (arg0:string const implicit) : bool <function-_at_strings_c__c_empty__hh_const_hh__hh_implicit_hh_string>` 
  *  :ref:`ends_with (str:$::das_string const implicit;cmp:string const implicit;context:__context const) : bool <function-_at_strings_c__c_ends_with__hh_const_hh__hh_implicit_hh__hh_handle_hh_das_string__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context>` 
  *  :ref:`ends_with (str:string const implicit;cmp:string const implicit;context:__context const) : bool <function-_at_strings_c__c_ends_with__hh_const_hh__hh_implicit_hh_string__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context>` 
  *  :ref:`length (str:$::das_string const implicit) : int <function-_at_strings_c__c_length__hh_const_hh__hh_implicit_hh__hh_handle_hh_das_string>` 
  *  :ref:`length (str:string const implicit;context:__context const) : int <function-_at_strings_c__c_length__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context>` 
  *  :ref:`starts_with (str:string const implicit;cmp:string const implicit;context:__context const) : bool <function-_at_strings_c__c_starts_with__hh_const_hh__hh_implicit_hh_string__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context>` 

.. _function-_at_strings_c__c_empty__hh_const_hh__hh_implicit_hh__hh_handle_hh_das_string:

.. das:function:: empty(arg0: das_string const implicit)

empty returns bool

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+arg0    + :ref:`builtin::das_string <handle-builtin-das_string>`  const implicit+
+--------+-----------------------------------------------------------------------+


|function-strings-empty|

.. _function-_at_strings_c__c_empty__hh_const_hh__hh_implicit_hh_string:

.. das:function:: empty(arg0: string const implicit)

empty returns bool

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+arg0    +string const implicit+
+--------+---------------------+


|function-strings-empty|

.. _function-_at_strings_c__c_ends_with__hh_const_hh__hh_implicit_hh__hh_handle_hh_das_string__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context:

.. das:function:: ends_with(str: das_string const implicit; cmp: string const implicit)

ends_with returns bool

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+str     + :ref:`builtin::das_string <handle-builtin-das_string>`  const implicit+
+--------+-----------------------------------------------------------------------+
+cmp     +string const implicit                                                  +
+--------+-----------------------------------------------------------------------+


|function-strings-ends_with|

.. _function-_at_strings_c__c_ends_with__hh_const_hh__hh_implicit_hh_string__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context:

.. das:function:: ends_with(str: string const implicit; cmp: string const implicit)

ends_with returns bool

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+str     +string const implicit+
+--------+---------------------+
+cmp     +string const implicit+
+--------+---------------------+


|function-strings-ends_with|

.. _function-_at_strings_c__c_length__hh_const_hh__hh_implicit_hh__hh_handle_hh_das_string:

.. das:function:: length(str: das_string const implicit)

length returns int

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+str     + :ref:`builtin::das_string <handle-builtin-das_string>`  const implicit+
+--------+-----------------------------------------------------------------------+


|function-strings-length|

.. _function-_at_strings_c__c_length__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context:

.. das:function:: length(str: string const implicit)

length returns int

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+str     +string const implicit+
+--------+---------------------+


|function-strings-length|

.. _function-_at_strings_c__c_starts_with__hh_const_hh__hh_implicit_hh_string__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context:

.. das:function:: starts_with(str: string const implicit; cmp: string const implicit)

starts_with returns bool

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+str     +string const implicit+
+--------+---------------------+
+cmp     +string const implicit+
+--------+---------------------+


|function-strings-starts_with|

++++++++++++++
String builder
++++++++++++++

  *  :ref:`build_string (block:block\<(strings::StringBuilderWriter):void\> const implicit;context:__context const) : string <function-_at_strings_c__c_build_string__hh_const_hh__hh_implicit_hh__hh_block_hh__hh_handle_hh_StringBuilderWriter_hh__c_void__hh_const_hh___context>` 
  *  :ref:`format (arg0:string const implicit;arg1:double const;arg2:__context const) : string <function-_at_strings_c__c_format__hh_const_hh__hh_implicit_hh_string__hh_const_hh_double__hh_const_hh___context>` 
  *  :ref:`format (arg0:string const implicit;arg1:float const;arg2:__context const) : string <function-_at_strings_c__c_format__hh_const_hh__hh_implicit_hh_string__hh_const_hh_float__hh_const_hh___context>` 
  *  :ref:`format (arg0:string const implicit;arg1:int const;arg2:__context const) : string <function-_at_strings_c__c_format__hh_const_hh__hh_implicit_hh_string__hh_const_hh_int__hh_const_hh___context>` 
  *  :ref:`format (arg0:string const implicit;arg1:int64 const;arg2:__context const) : string <function-_at_strings_c__c_format__hh_const_hh__hh_implicit_hh_string__hh_const_hh_int64__hh_const_hh___context>` 
  *  :ref:`format (arg0:string const implicit;arg1:uint const;arg2:__context const) : string <function-_at_strings_c__c_format__hh_const_hh__hh_implicit_hh_string__hh_const_hh_uint__hh_const_hh___context>` 
  *  :ref:`format (arg0:string const implicit;arg1:uint64 const;arg2:__context const) : string <function-_at_strings_c__c_format__hh_const_hh__hh_implicit_hh_string__hh_const_hh_uint64__hh_const_hh___context>` 
  *  :ref:`format (writer:strings::StringBuilderWriter implicit;format:string const implicit;value:double const) : void <function-_at_strings_c__c_format__hh_implicit_hh__hh_handle_hh_StringBuilderWriter__hh_const_hh__hh_implicit_hh_string__hh_const_hh_double>` 
  *  :ref:`format (writer:strings::StringBuilderWriter implicit;format:string const implicit;value:float const) : void <function-_at_strings_c__c_format__hh_implicit_hh__hh_handle_hh_StringBuilderWriter__hh_const_hh__hh_implicit_hh_string__hh_const_hh_float>` 
  *  :ref:`format (writer:strings::StringBuilderWriter implicit;format:string const implicit;value:int const) : void <function-_at_strings_c__c_format__hh_implicit_hh__hh_handle_hh_StringBuilderWriter__hh_const_hh__hh_implicit_hh_string__hh_const_hh_int>` 
  *  :ref:`format (writer:strings::StringBuilderWriter implicit;format:string const implicit;value:int64 const) : void <function-_at_strings_c__c_format__hh_implicit_hh__hh_handle_hh_StringBuilderWriter__hh_const_hh__hh_implicit_hh_string__hh_const_hh_int64>` 
  *  :ref:`format (writer:strings::StringBuilderWriter implicit;format:string const implicit;value:uint const) : void <function-_at_strings_c__c_format__hh_implicit_hh__hh_handle_hh_StringBuilderWriter__hh_const_hh__hh_implicit_hh_string__hh_const_hh_uint>` 
  *  :ref:`format (writer:strings::StringBuilderWriter implicit;format:string const implicit;value:uint64 const) : void <function-_at_strings_c__c_format__hh_implicit_hh__hh_handle_hh_StringBuilderWriter__hh_const_hh__hh_implicit_hh_string__hh_const_hh_uint64>` 
  *  :ref:`write (writer:strings::StringBuilderWriter;anything:any const) : void <function-_at_strings_c__c_write__hh_handle_hh_StringBuilderWriter__hh_const_hh_any>` 
  *  :ref:`write_char (writer:strings::StringBuilderWriter implicit;ch:int const) : void <function-_at_strings_c__c_write_char__hh_implicit_hh__hh_handle_hh_StringBuilderWriter__hh_const_hh_int>` 
  *  :ref:`write_chars (writer:strings::StringBuilderWriter implicit;ch:int const;count:int const) : void <function-_at_strings_c__c_write_chars__hh_implicit_hh__hh_handle_hh_StringBuilderWriter__hh_const_hh_int__hh_const_hh_int>` 
  *  :ref:`write_escape_string (writer:strings::StringBuilderWriter implicit;str:string const implicit) : void <function-_at_strings_c__c_write_escape_string__hh_implicit_hh__hh_handle_hh_StringBuilderWriter__hh_const_hh__hh_implicit_hh_string>` 

.. _function-_at_strings_c__c_build_string__hh_const_hh__hh_implicit_hh__hh_block_hh__hh_handle_hh_StringBuilderWriter_hh__c_void__hh_const_hh___context:

.. das:function:: build_string(block: block<(strings::StringBuilderWriter):void> const implicit)

build_string returns string

+--------+-------------------------------------------------------------------------------------------------------+
+argument+argument type                                                                                          +
+========+=======================================================================================================+
+block   +block<( :ref:`strings::StringBuilderWriter <handle-strings-StringBuilderWriter>` ):void> const implicit+
+--------+-------------------------------------------------------------------------------------------------------+


|function-strings-build_string|

.. _function-_at_strings_c__c_format__hh_const_hh__hh_implicit_hh_string__hh_const_hh_double__hh_const_hh___context:

.. das:function:: format(arg0: string const implicit; arg1: double const)

format returns string

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+arg0    +string const implicit+
+--------+---------------------+
+arg1    +double const         +
+--------+---------------------+


|function-strings-format|

.. _function-_at_strings_c__c_format__hh_const_hh__hh_implicit_hh_string__hh_const_hh_float__hh_const_hh___context:

.. das:function:: format(arg0: string const implicit; arg1: float const)

format returns string

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+arg0    +string const implicit+
+--------+---------------------+
+arg1    +float const          +
+--------+---------------------+


|function-strings-format|

.. _function-_at_strings_c__c_format__hh_const_hh__hh_implicit_hh_string__hh_const_hh_int__hh_const_hh___context:

.. das:function:: format(arg0: string const implicit; arg1: int const)

format returns string

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+arg0    +string const implicit+
+--------+---------------------+
+arg1    +int const            +
+--------+---------------------+


|function-strings-format|

.. _function-_at_strings_c__c_format__hh_const_hh__hh_implicit_hh_string__hh_const_hh_int64__hh_const_hh___context:

.. das:function:: format(arg0: string const implicit; arg1: int64 const)

format returns string

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+arg0    +string const implicit+
+--------+---------------------+
+arg1    +int64 const          +
+--------+---------------------+


|function-strings-format|

.. _function-_at_strings_c__c_format__hh_const_hh__hh_implicit_hh_string__hh_const_hh_uint__hh_const_hh___context:

.. das:function:: format(arg0: string const implicit; arg1: uint const)

format returns string

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+arg0    +string const implicit+
+--------+---------------------+
+arg1    +uint const           +
+--------+---------------------+


|function-strings-format|

.. _function-_at_strings_c__c_format__hh_const_hh__hh_implicit_hh_string__hh_const_hh_uint64__hh_const_hh___context:

.. das:function:: format(arg0: string const implicit; arg1: uint64 const)

format returns string

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+arg0    +string const implicit+
+--------+---------------------+
+arg1    +uint64 const         +
+--------+---------------------+


|function-strings-format|

.. _function-_at_strings_c__c_format__hh_implicit_hh__hh_handle_hh_StringBuilderWriter__hh_const_hh__hh_implicit_hh_string__hh_const_hh_double:

.. das:function:: format(writer: StringBuilderWriter implicit; format: string const implicit; value: double const)

+--------+-----------------------------------------------------------------------------------+
+argument+argument type                                                                      +
+========+===================================================================================+
+writer  + :ref:`strings::StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit+
+--------+-----------------------------------------------------------------------------------+
+format  +string const implicit                                                              +
+--------+-----------------------------------------------------------------------------------+
+value   +double const                                                                       +
+--------+-----------------------------------------------------------------------------------+


|function-strings-format|

.. _function-_at_strings_c__c_format__hh_implicit_hh__hh_handle_hh_StringBuilderWriter__hh_const_hh__hh_implicit_hh_string__hh_const_hh_float:

.. das:function:: format(writer: StringBuilderWriter implicit; format: string const implicit; value: float const)

+--------+-----------------------------------------------------------------------------------+
+argument+argument type                                                                      +
+========+===================================================================================+
+writer  + :ref:`strings::StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit+
+--------+-----------------------------------------------------------------------------------+
+format  +string const implicit                                                              +
+--------+-----------------------------------------------------------------------------------+
+value   +float const                                                                        +
+--------+-----------------------------------------------------------------------------------+


|function-strings-format|

.. _function-_at_strings_c__c_format__hh_implicit_hh__hh_handle_hh_StringBuilderWriter__hh_const_hh__hh_implicit_hh_string__hh_const_hh_int:

.. das:function:: format(writer: StringBuilderWriter implicit; format: string const implicit; value: int const)

+--------+-----------------------------------------------------------------------------------+
+argument+argument type                                                                      +
+========+===================================================================================+
+writer  + :ref:`strings::StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit+
+--------+-----------------------------------------------------------------------------------+
+format  +string const implicit                                                              +
+--------+-----------------------------------------------------------------------------------+
+value   +int const                                                                          +
+--------+-----------------------------------------------------------------------------------+


|function-strings-format|

.. _function-_at_strings_c__c_format__hh_implicit_hh__hh_handle_hh_StringBuilderWriter__hh_const_hh__hh_implicit_hh_string__hh_const_hh_int64:

.. das:function:: format(writer: StringBuilderWriter implicit; format: string const implicit; value: int64 const)

+--------+-----------------------------------------------------------------------------------+
+argument+argument type                                                                      +
+========+===================================================================================+
+writer  + :ref:`strings::StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit+
+--------+-----------------------------------------------------------------------------------+
+format  +string const implicit                                                              +
+--------+-----------------------------------------------------------------------------------+
+value   +int64 const                                                                        +
+--------+-----------------------------------------------------------------------------------+


|function-strings-format|

.. _function-_at_strings_c__c_format__hh_implicit_hh__hh_handle_hh_StringBuilderWriter__hh_const_hh__hh_implicit_hh_string__hh_const_hh_uint:

.. das:function:: format(writer: StringBuilderWriter implicit; format: string const implicit; value: uint const)

+--------+-----------------------------------------------------------------------------------+
+argument+argument type                                                                      +
+========+===================================================================================+
+writer  + :ref:`strings::StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit+
+--------+-----------------------------------------------------------------------------------+
+format  +string const implicit                                                              +
+--------+-----------------------------------------------------------------------------------+
+value   +uint const                                                                         +
+--------+-----------------------------------------------------------------------------------+


|function-strings-format|

.. _function-_at_strings_c__c_format__hh_implicit_hh__hh_handle_hh_StringBuilderWriter__hh_const_hh__hh_implicit_hh_string__hh_const_hh_uint64:

.. das:function:: format(writer: StringBuilderWriter implicit; format: string const implicit; value: uint64 const)

+--------+-----------------------------------------------------------------------------------+
+argument+argument type                                                                      +
+========+===================================================================================+
+writer  + :ref:`strings::StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit+
+--------+-----------------------------------------------------------------------------------+
+format  +string const implicit                                                              +
+--------+-----------------------------------------------------------------------------------+
+value   +uint64 const                                                                       +
+--------+-----------------------------------------------------------------------------------+


|function-strings-format|

.. _function-_at_strings_c__c_write__hh_handle_hh_StringBuilderWriter__hh_const_hh_any:

.. das:function:: write(writer: StringBuilderWriter; anything: any const)

+--------+--------------------------------------------------------------------------+
+argument+argument type                                                             +
+========+==========================================================================+
+writer  + :ref:`strings::StringBuilderWriter <handle-strings-StringBuilderWriter>` +
+--------+--------------------------------------------------------------------------+
+anything+any const                                                                 +
+--------+--------------------------------------------------------------------------+


|function-strings-write|

.. _function-_at_strings_c__c_write_char__hh_implicit_hh__hh_handle_hh_StringBuilderWriter__hh_const_hh_int:

.. das:function:: write_char(writer: StringBuilderWriter implicit; ch: int const)

+--------+-----------------------------------------------------------------------------------+
+argument+argument type                                                                      +
+========+===================================================================================+
+writer  + :ref:`strings::StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit+
+--------+-----------------------------------------------------------------------------------+
+ch      +int const                                                                          +
+--------+-----------------------------------------------------------------------------------+


|function-strings-write_char|

.. _function-_at_strings_c__c_write_chars__hh_implicit_hh__hh_handle_hh_StringBuilderWriter__hh_const_hh_int__hh_const_hh_int:

.. das:function:: write_chars(writer: StringBuilderWriter implicit; ch: int const; count: int const)

+--------+-----------------------------------------------------------------------------------+
+argument+argument type                                                                      +
+========+===================================================================================+
+writer  + :ref:`strings::StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit+
+--------+-----------------------------------------------------------------------------------+
+ch      +int const                                                                          +
+--------+-----------------------------------------------------------------------------------+
+count   +int const                                                                          +
+--------+-----------------------------------------------------------------------------------+


|function-strings-write_chars|

.. _function-_at_strings_c__c_write_escape_string__hh_implicit_hh__hh_handle_hh_StringBuilderWriter__hh_const_hh__hh_implicit_hh_string:

.. das:function:: write_escape_string(writer: StringBuilderWriter implicit; str: string const implicit)

+--------+-----------------------------------------------------------------------------------+
+argument+argument type                                                                      +
+========+===================================================================================+
+writer  + :ref:`strings::StringBuilderWriter <handle-strings-StringBuilderWriter>`  implicit+
+--------+-----------------------------------------------------------------------------------+
+str     +string const implicit                                                              +
+--------+-----------------------------------------------------------------------------------+


|function-strings-write_escape_string|

++++++++++++++++++++++++
das::string manipulation
++++++++++++++++++++++++

  *  :ref:`append (str:$::das_string implicit;ch:int const) : void <function-_at_strings_c__c_append__hh_implicit_hh__hh_handle_hh_das_string__hh_const_hh_int>` 
  *  :ref:`resize (arg0:$::das_string implicit;arg1:int const) : void <function-_at_strings_c__c_resize__hh_implicit_hh__hh_handle_hh_das_string__hh_const_hh_int>` 

.. _function-_at_strings_c__c_append__hh_implicit_hh__hh_handle_hh_das_string__hh_const_hh_int:

.. das:function:: append(str: das_string implicit; ch: int const)

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+str     + :ref:`builtin::das_string <handle-builtin-das_string>`  implicit+
+--------+-----------------------------------------------------------------+
+ch      +int const                                                        +
+--------+-----------------------------------------------------------------+


|function-strings-append|

.. _function-_at_strings_c__c_resize__hh_implicit_hh__hh_handle_hh_das_string__hh_const_hh_int:

.. das:function:: resize(arg0: das_string implicit; arg1: int const)

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+arg0    + :ref:`builtin::das_string <handle-builtin-das_string>`  implicit+
+--------+-----------------------------------------------------------------+
+arg1    +int const                                                        +
+--------+-----------------------------------------------------------------+


|function-strings-resize|

++++++++++++++++++++
String modifications
++++++++++++++++++++

  *  :ref:`chop (arg0:string const implicit;arg1:int const;arg2:int const;arg3:__context const) : string <function-_at_strings_c__c_chop__hh_const_hh__hh_implicit_hh_string__hh_const_hh_int__hh_const_hh_int__hh_const_hh___context>` 
  *  :ref:`escape (arg0:string const implicit;arg1:__context const) : string <function-_at_strings_c__c_escape__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context>` 
  *  :ref:`repeat (arg0:string const implicit;arg1:int const;arg2:__context const) : string <function-_at_strings_c__c_repeat__hh_const_hh__hh_implicit_hh_string__hh_const_hh_int__hh_const_hh___context>` 
  *  :ref:`replace (arg0:string const implicit;arg1:string const implicit;arg2:string const implicit;arg3:__context const) : string <function-_at_strings_c__c_replace__hh_const_hh__hh_implicit_hh_string__hh_const_hh__hh_implicit_hh_string__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context>` 
  *  :ref:`reverse (str:string const implicit;context:__context const) : string <function-_at_strings_c__c_reverse__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context>` 
  *  :ref:`rtrim (arg0:string const implicit;arg1:__context const) : string <function-_at_strings_c__c_rtrim__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context>` 
  *  :ref:`slice (str:string const implicit;start:int const;context:__context const) : string <function-_at_strings_c__c_slice__hh_const_hh__hh_implicit_hh_string__hh_const_hh_int__hh_const_hh___context>` 
  *  :ref:`slice (str:string const implicit;start:int const;end:int const;context:__context const) : string <function-_at_strings_c__c_slice__hh_const_hh__hh_implicit_hh_string__hh_const_hh_int__hh_const_hh_int__hh_const_hh___context>` 
  *  :ref:`strip (str:string const implicit;context:__context const) : string <function-_at_strings_c__c_strip__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context>` 
  *  :ref:`strip_left (str:string const implicit;context:__context const) : string <function-_at_strings_c__c_strip_left__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context>` 
  *  :ref:`strip_right (str:string const implicit;context:__context const) : string <function-_at_strings_c__c_strip_right__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context>` 
  *  :ref:`to_lower (str:string const implicit;context:__context const) : string <function-_at_strings_c__c_to_lower__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context>` 
  *  :ref:`to_lower_in_place (str:string const implicit) : string <function-_at_strings_c__c_to_lower_in_place__hh_const_hh__hh_implicit_hh_string>` 
  *  :ref:`to_upper (str:string const implicit;context:__context const) : string <function-_at_strings_c__c_to_upper__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context>` 
  *  :ref:`to_upper_in_place (str:string const implicit) : string <function-_at_strings_c__c_to_upper_in_place__hh_const_hh__hh_implicit_hh_string>` 
  *  :ref:`unescape (arg0:string const implicit;arg1:__context const) : string <function-_at_strings_c__c_unescape__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context>` 

.. _function-_at_strings_c__c_chop__hh_const_hh__hh_implicit_hh_string__hh_const_hh_int__hh_const_hh_int__hh_const_hh___context:

.. das:function:: chop(arg0: string const implicit; arg1: int const; arg2: int const)

chop returns string

.. warning:: 
  This is unsafe operation.

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+arg0    +string const implicit+
+--------+---------------------+
+arg1    +int const            +
+--------+---------------------+
+arg2    +int const            +
+--------+---------------------+


|function-strings-chop|

.. _function-_at_strings_c__c_escape__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context:

.. das:function:: escape(arg0: string const implicit)

escape returns string

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+arg0    +string const implicit+
+--------+---------------------+


|function-strings-escape|

.. _function-_at_strings_c__c_repeat__hh_const_hh__hh_implicit_hh_string__hh_const_hh_int__hh_const_hh___context:

.. das:function:: repeat(arg0: string const implicit; arg1: int const)

repeat returns string

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+arg0    +string const implicit+
+--------+---------------------+
+arg1    +int const            +
+--------+---------------------+


|function-strings-repeat|

.. _function-_at_strings_c__c_replace__hh_const_hh__hh_implicit_hh_string__hh_const_hh__hh_implicit_hh_string__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context:

.. das:function:: replace(arg0: string const implicit; arg1: string const implicit; arg2: string const implicit)

replace returns string

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+arg0    +string const implicit+
+--------+---------------------+
+arg1    +string const implicit+
+--------+---------------------+
+arg2    +string const implicit+
+--------+---------------------+


|function-strings-replace|

.. _function-_at_strings_c__c_reverse__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context:

.. das:function:: reverse(str: string const implicit)

reverse returns string

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+str     +string const implicit+
+--------+---------------------+


|function-strings-reverse|

.. _function-_at_strings_c__c_rtrim__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context:

.. das:function:: rtrim(arg0: string const implicit)

rtrim returns string

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+arg0    +string const implicit+
+--------+---------------------+


|function-strings-rtrim|

.. _function-_at_strings_c__c_slice__hh_const_hh__hh_implicit_hh_string__hh_const_hh_int__hh_const_hh___context:

.. das:function:: slice(str: string const implicit; start: int const)

slice returns string

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+str     +string const implicit+
+--------+---------------------+
+start   +int const            +
+--------+---------------------+


|function-strings-slice|

.. _function-_at_strings_c__c_slice__hh_const_hh__hh_implicit_hh_string__hh_const_hh_int__hh_const_hh_int__hh_const_hh___context:

.. das:function:: slice(str: string const implicit; start: int const; end: int const)

slice returns string

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+str     +string const implicit+
+--------+---------------------+
+start   +int const            +
+--------+---------------------+
+end     +int const            +
+--------+---------------------+


|function-strings-slice|

.. _function-_at_strings_c__c_strip__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context:

.. das:function:: strip(str: string const implicit)

strip returns string

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+str     +string const implicit+
+--------+---------------------+


|function-strings-strip|

.. _function-_at_strings_c__c_strip_left__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context:

.. das:function:: strip_left(str: string const implicit)

strip_left returns string

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+str     +string const implicit+
+--------+---------------------+


|function-strings-strip_left|

.. _function-_at_strings_c__c_strip_right__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context:

.. das:function:: strip_right(str: string const implicit)

strip_right returns string

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+str     +string const implicit+
+--------+---------------------+


|function-strings-strip_right|

.. _function-_at_strings_c__c_to_lower__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context:

.. das:function:: to_lower(str: string const implicit)

to_lower returns string

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+str     +string const implicit+
+--------+---------------------+


|function-strings-to_lower|

.. _function-_at_strings_c__c_to_lower_in_place__hh_const_hh__hh_implicit_hh_string:

.. das:function:: to_lower_in_place(str: string const implicit)

to_lower_in_place returns string

.. warning:: 
  This is unsafe operation.

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+str     +string const implicit+
+--------+---------------------+


|function-strings-to_lower_in_place|

.. _function-_at_strings_c__c_to_upper__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context:

.. das:function:: to_upper(str: string const implicit)

to_upper returns string

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+str     +string const implicit+
+--------+---------------------+


|function-strings-to_upper|

.. _function-_at_strings_c__c_to_upper_in_place__hh_const_hh__hh_implicit_hh_string:

.. das:function:: to_upper_in_place(str: string const implicit)

to_upper_in_place returns string

.. warning:: 
  This is unsafe operation.

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+str     +string const implicit+
+--------+---------------------+


|function-strings-to_upper_in_place|

.. _function-_at_strings_c__c_unescape__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context:

.. das:function:: unescape(arg0: string const implicit)

unescape returns string

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+arg0    +string const implicit+
+--------+---------------------+


|function-strings-unescape|

+++++++++++++++++
Search substrings
+++++++++++++++++

  *  :ref:`find (str:string const implicit;substr:string const implicit) : int <function-_at_strings_c__c_find__hh_const_hh__hh_implicit_hh_string__hh_const_hh__hh_implicit_hh_string>` 
  *  :ref:`find (str:string const implicit;substr:string const implicit;start:int const;context:__context const) : int <function-_at_strings_c__c_find__hh_const_hh__hh_implicit_hh_string__hh_const_hh__hh_implicit_hh_string__hh_const_hh_int__hh_const_hh___context>` 
  *  :ref:`find (str:string const implicit;substr:int const;context:__context const) : int <function-_at_strings_c__c_find__hh_const_hh__hh_implicit_hh_string__hh_const_hh_int__hh_const_hh___context>` 

.. _function-_at_strings_c__c_find__hh_const_hh__hh_implicit_hh_string__hh_const_hh__hh_implicit_hh_string:

.. das:function:: find(str: string const implicit; substr: string const implicit)

find returns int

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+str     +string const implicit+
+--------+---------------------+
+substr  +string const implicit+
+--------+---------------------+


|function-strings-find|

.. _function-_at_strings_c__c_find__hh_const_hh__hh_implicit_hh_string__hh_const_hh__hh_implicit_hh_string__hh_const_hh_int__hh_const_hh___context:

.. das:function:: find(str: string const implicit; substr: string const implicit; start: int const)

find returns int

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+str     +string const implicit+
+--------+---------------------+
+substr  +string const implicit+
+--------+---------------------+
+start   +int const            +
+--------+---------------------+


|function-strings-find|

.. _function-_at_strings_c__c_find__hh_const_hh__hh_implicit_hh_string__hh_const_hh_int__hh_const_hh___context:

.. das:function:: find(str: string const implicit; substr: int const)

find returns int

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+str     +string const implicit+
+--------+---------------------+
+substr  +int const            +
+--------+---------------------+


|function-strings-find|

++++++++++++++++++++++++++
String conversion routines
++++++++++++++++++++++++++

  *  :ref:`double (arg0:string const implicit;arg1:__context const) : double <function-_at_strings_c__c_double__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context>` 
  *  :ref:`float (arg0:string const implicit;arg1:__context const) : float <function-_at_strings_c__c_float__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context>` 
  *  :ref:`int (arg0:string const implicit;arg1:__context const) : int <function-_at_strings_c__c_int__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context>` 
  *  :ref:`string (bytes:array\<uint8\> const implicit;context:__context const) : string <function-_at_strings_c__c_string__hh_const_hh__hh_implicit_hh__hh_array_hh_uint8__hh_const_hh___context>` 
  *  :ref:`to_char (arg0:int const;arg1:__context const) : string <function-_at_strings_c__c_to_char__hh_const_hh_int__hh_const_hh___context>` 
  *  :ref:`to_float (arg0:string const implicit) : float <function-_at_strings_c__c_to_float__hh_const_hh__hh_implicit_hh_string>` 
  *  :ref:`to_int (arg0:string const implicit) : int <function-_at_strings_c__c_to_int__hh_const_hh__hh_implicit_hh_string>` 
  *  :ref:`uint (arg0:string const implicit;arg1:__context const) : uint <function-_at_strings_c__c_uint__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context>` 

.. _function-_at_strings_c__c_double__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context:

.. das:function:: double(arg0: string const implicit)

double returns double

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+arg0    +string const implicit+
+--------+---------------------+


|function-strings-double|

.. _function-_at_strings_c__c_float__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context:

.. das:function:: float(arg0: string const implicit)

float returns float

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+arg0    +string const implicit+
+--------+---------------------+


|function-strings-float|

.. _function-_at_strings_c__c_int__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context:

.. das:function:: int(arg0: string const implicit)

int returns int

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+arg0    +string const implicit+
+--------+---------------------+


|function-strings-int|

.. _function-_at_strings_c__c_string__hh_const_hh__hh_implicit_hh__hh_array_hh_uint8__hh_const_hh___context:

.. das:function:: string(bytes: array<uint8> const implicit)

string returns string

+--------+---------------------------+
+argument+argument type              +
+========+===========================+
+bytes   +array<uint8> const implicit+
+--------+---------------------------+


|function-strings-string|

.. _function-_at_strings_c__c_to_char__hh_const_hh_int__hh_const_hh___context:

.. das:function:: to_char(arg0: int const)

to_char returns string

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +int const    +
+--------+-------------+


|function-strings-to_char|

.. _function-_at_strings_c__c_to_float__hh_const_hh__hh_implicit_hh_string:

.. das:function:: to_float(arg0: string const implicit)

to_float returns float

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+arg0    +string const implicit+
+--------+---------------------+


|function-strings-to_float|

.. _function-_at_strings_c__c_to_int__hh_const_hh__hh_implicit_hh_string:

.. das:function:: to_int(arg0: string const implicit)

to_int returns int

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+arg0    +string const implicit+
+--------+---------------------+


|function-strings-to_int|

.. _function-_at_strings_c__c_uint__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context:

.. das:function:: uint(arg0: string const implicit)

uint returns uint

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+arg0    +string const implicit+
+--------+---------------------+


|function-strings-uint|

+++++++++++++++
String as array
+++++++++++++++

  *  :ref:`modify_data (str:string const implicit;block:block\<(array\<uint8\>#):void\> const implicit;context:__context const) : string <function-_at_strings_c__c_modify_data__hh_const_hh__hh_implicit_hh_string__hh_const_hh__hh_implicit_hh__hh_block_hh__hh_temporary_hh__hh_array_hh_uint8_hh__c_void__hh_const_hh___context>` 
  *  :ref:`peek_data (str:string const implicit;block:block\<(array\<uint8\> const#):void\> const implicit;context:__context const) : void <function-_at_strings_c__c_peek_data__hh_const_hh__hh_implicit_hh_string__hh_const_hh__hh_implicit_hh__hh_block_hh__hh_const_hh__hh_temporary_hh__hh_array_hh_uint8_hh__c_void__hh_const_hh___context>` 

.. _function-_at_strings_c__c_modify_data__hh_const_hh__hh_implicit_hh_string__hh_const_hh__hh_implicit_hh__hh_block_hh__hh_temporary_hh__hh_array_hh_uint8_hh__c_void__hh_const_hh___context:

.. das:function:: modify_data(str: string const implicit; block: block<(array<uint8>#):void> const implicit)

modify_data returns string

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+str     +string const implicit                     +
+--------+------------------------------------------+
+block   +block<(array<uint8>#):void> const implicit+
+--------+------------------------------------------+


|function-strings-modify_data|

.. _function-_at_strings_c__c_peek_data__hh_const_hh__hh_implicit_hh_string__hh_const_hh__hh_implicit_hh__hh_block_hh__hh_const_hh__hh_temporary_hh__hh_array_hh_uint8_hh__c_void__hh_const_hh___context:

.. das:function:: peek_data(str: string const implicit; block: block<(array<uint8> const#):void> const implicit)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+str     +string const implicit                           +
+--------+------------------------------------------------+
+block   +block<(array<uint8> const#):void> const implicit+
+--------+------------------------------------------------+


|function-strings-peek_data|

+++++++++++++++++++++++++++
Low level memory allocation
+++++++++++++++++++++++++++

  *  :ref:`delete_string (str:string& implicit;context:__context const) : void <function-_at_strings_c__c_delete_string__hh_ref_hh__hh_implicit_hh_string__hh_const_hh___context>` 
  *  :ref:`reserve_string_buffer (arg0:string const implicit;arg1:int const;arg2:__context const) : string <function-_at_strings_c__c_reserve_string_buffer__hh_const_hh__hh_implicit_hh_string__hh_const_hh_int__hh_const_hh___context>` 

.. _function-_at_strings_c__c_delete_string__hh_ref_hh__hh_implicit_hh_string__hh_const_hh___context:

.. das:function:: delete_string(str: string& implicit)

.. warning:: 
  This is unsafe operation.

+--------+----------------+
+argument+argument type   +
+========+================+
+str     +string& implicit+
+--------+----------------+


|function-strings-delete_string|

.. _function-_at_strings_c__c_reserve_string_buffer__hh_const_hh__hh_implicit_hh_string__hh_const_hh_int__hh_const_hh___context:

.. das:function:: reserve_string_buffer(arg0: string const implicit; arg1: int const)

reserve_string_buffer returns string

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+arg0    +string const implicit+
+--------+---------------------+
+arg1    +int const            +
+--------+---------------------+


|function-strings-reserve_string_buffer|


