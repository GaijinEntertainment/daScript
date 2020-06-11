
.. _stdlib_$:

================
Built-in runtime
================

++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-builtin-StringBuilderWriter:

.. das:attribute:: StringBuilderWriter

structure_annotation|builtin|StringBuilderWriter to be documented

+++++++++++++
Handled types
+++++++++++++

.. _handle-builtin-clock:

.. das:attribute:: clock

any_annotation|builtin|clock to be documented

.. _handle-builtin-das_string:

.. das:attribute:: das_string

any_annotation|builtin|das_string to be documented

.. _handle-builtin-float3x4:

.. das:attribute:: float3x4

any_annotation|builtin|float3x4 to be documented

.. _handle-builtin-float4x4:

.. das:attribute:: float4x4

any_annotation|builtin|float4x4 to be documented

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at__builtin_::append__hh_handle_hh_das_string_hh_implicit_int_hh_const:

.. das:function:: append(arg0: das_string implicit; arg1: int const)

arguments are

+----+-----------------------------------------------------------------+
+arg0+ :ref:`builtin::das_string <handle-builtin-das_string>`  implicit+
+----+-----------------------------------------------------------------+
+arg1+int const                                                        +
+----+-----------------------------------------------------------------+


function|builtin|append to be documented

.. _function-_at__builtin_::append_string_hh_const_int_hh_const__hh_const:

.. das:function:: append(arg0: string const; arg1: int const)

append returns string

.. warning:: 
  This is unsafe operation.

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+


function|builtin|append to be documented

.. _function-_at__builtin_::as_string__hh_array_hh_uint8_hh_const_hh_implicit__hh_const:

.. das:function:: as_string(arg0: array<uint8> const implicit)

as_string returns string

arguments are

+----+---------------------------+
+arg0+array<uint8> const implicit+
+----+---------------------------+


function|builtin|as_string to be documented

.. _function-_at__builtin_::breakpoint:

.. das:function:: breakpoint()

function|builtin|breakpoint to be documented

.. _function-_at__builtin_::build_string__hh_block_hh__hh_handle_hh_StringBuilderWriter_hh_:void_hh_const_hh_implicit__hh_const:

.. das:function:: build_string(arg0: block<(StringBuilderWriter):void> const implicit)

build_string returns string

arguments are

+----+-------------------------------------------------------------------------------------------------------+
+arg0+block<( :ref:`builtin::StringBuilderWriter <handle-builtin-StringBuilderWriter>` ):void> const implicit+
+----+-------------------------------------------------------------------------------------------------------+


function|builtin|build_string to be documented

.. _function-_at__builtin_::builtin_strdup_any:

.. das:function:: builtin_strdup(arg0: any)

.. warning:: 
  This is unsafe operation.

arguments are

+----+---+
+arg0+any+
+----+---+


function|builtin|builtin_strdup to be documented

.. _function-_at__builtin_::builtin_string_split_string_hh_const_string_hh_const__hh_block_hh_const_hh_implicit__hh_const:

.. das:function:: builtin_string_split(arg0: string const; arg1: string const; arg2: block<> const implicit)

arguments are

+----+----------------------+
+arg0+string const          +
+----+----------------------+
+arg1+string const          +
+----+----------------------+
+arg2+block<> const implicit+
+----+----------------------+


function|builtin|builtin_string_split to be documented

.. _function-_at__builtin_::builtin_string_split_by_char_string_hh_const_string_hh_const__hh_block_hh_const_hh_implicit__hh_const:

.. das:function:: builtin_string_split_by_char(arg0: string const; arg1: string const; arg2: block<> const implicit)

arguments are

+----+----------------------+
+arg0+string const          +
+----+----------------------+
+arg1+string const          +
+----+----------------------+
+arg2+block<> const implicit+
+----+----------------------+


function|builtin|builtin_string_split_by_char to be documented

.. _function-_at__builtin_::capacity__hh_array_hh_const_hh_implicit:

.. das:function:: capacity(arg0: array const implicit)

capacity returns int

arguments are

+----+--------------------+
+arg0+array const implicit+
+----+--------------------+


function|builtin|capacity to be documented

.. _function-_at__builtin_::capacity__hh_table_hh_const_hh_implicit:

.. das:function:: capacity(arg0: table const implicit)

capacity returns int

arguments are

+----+--------------------+
+arg0+table const implicit+
+----+--------------------+


function|builtin|capacity to be documented

.. _function-_at__builtin_::character_at_string_hh_const_int_hh_const__hh_const:

.. das:function:: character_at(arg0: string const; arg1: int const)

character_at returns int

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+


function|builtin|character_at to be documented

.. _function-_at__builtin_::character_uat_string_hh_const_int_hh_const:

.. das:function:: character_uat(arg0: string const; arg1: int const)

character_uat returns int

.. warning:: 
  This is unsafe operation.

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+


function|builtin|character_uat to be documented

.. _function-_at__builtin_::chop_string_hh_const_int_hh_const_int_hh_const__hh_const:

.. das:function:: chop(arg0: string const; arg1: int const; arg2: int const)

chop returns string

.. warning:: 
  This is unsafe operation.

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+
+arg2+int const   +
+----+------------+


function|builtin|chop to be documented

.. _function-_at__builtin_::clear__hh_array_hh_implicit__hh_const:

.. das:function:: clear(arg0: array implicit)

arguments are

+----+--------------+
+arg0+array implicit+
+----+--------------+


function|builtin|clear to be documented

.. _function-_at__builtin_::clear__hh_table_hh_implicit__hh_const:

.. das:function:: clear(arg0: table implicit)

arguments are

+----+--------------+
+arg0+table implicit+
+----+--------------+


function|builtin|clear to be documented

.. _function-_at__builtin_::clone__hh_handle_hh_das_string_string_hh_const:

.. das:function:: clone(a: das_string; b: string const)

arguments are

+-+--------------------------------------------------------+
+a+ :ref:`builtin::das_string <handle-builtin-das_string>` +
+-+--------------------------------------------------------+
+b+string const                                            +
+-+--------------------------------------------------------+


function|builtin|clone to be documented

.. _function-_at__builtin_::clone_string_string_hh_const__hh_const:

.. das:function:: clone_string(arg0: string const)

clone_string returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|builtin|clone_string to be documented

.. _function-_at__builtin_::delete_string_string_hh_ref_hh_implicit__hh_const:

.. das:function:: delete_string(arg0: string& implicit)

.. warning:: 
  This is unsafe operation.

arguments are

+----+----------------+
+arg0+string& implicit+
+----+----------------+


function|builtin|delete_string to be documented

.. _function-_at__builtin_::dump_profile_info__hh_const:

.. das:function:: dump_profile_info()

function|builtin|dump_profile_info to be documented

.. _function-_at__builtin_::empty__hh_handle_hh_das_string_hh_const_hh_implicit:

.. das:function:: empty(arg0: das_string const implicit)

empty returns bool

arguments are

+----+-----------------------------------------------------------------------+
+arg0+ :ref:`builtin::das_string <handle-builtin-das_string>`  const implicit+
+----+-----------------------------------------------------------------------+


function|builtin|empty to be documented

.. _function-_at__builtin_::empty__hh_iterator_hh_const_hh_implicit:

.. das:function:: empty(arg0: iterator const implicit)

empty returns bool

arguments are

+----+-----------------------+
+arg0+iterator const implicit+
+----+-----------------------+


function|builtin|empty to be documented

.. _function-_at__builtin_::empty_string_hh_const:

.. das:function:: empty(arg0: string const)

empty returns bool

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|builtin|empty to be documented

.. _function-_at__builtin_::ends_with__hh_handle_hh_das_string_hh_const_hh_implicit_string_hh_const__hh_const:

.. das:function:: ends_with(arg0: das_string const implicit; arg1: string const)

ends_with returns bool

arguments are

+----+-----------------------------------------------------------------------+
+arg0+ :ref:`builtin::das_string <handle-builtin-das_string>`  const implicit+
+----+-----------------------------------------------------------------------+
+arg1+string const                                                           +
+----+-----------------------------------------------------------------------+


function|builtin|ends_with to be documented

.. _function-_at__builtin_::ends_with_string_hh_const_string_hh_const__hh_const:

.. das:function:: ends_with(arg0: string const; arg1: string const)

ends_with returns bool

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+string const+
+----+------------+


function|builtin|ends_with to be documented

.. _function-_at__builtin_::escape_string_hh_const__hh_const:

.. das:function:: escape(arg0: string const)

escape returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|builtin|escape to be documented

.. _function-_at__builtin_::find_string_hh_const_string_hh_const:

.. das:function:: find(arg0: string const; arg1: string const)

find returns int

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+string const+
+----+------------+


function|builtin|find to be documented

.. _function-_at__builtin_::find_string_hh_const_string_hh_const_int_hh_const__hh_const:

.. das:function:: find(arg0: string const; arg1: string const; arg2: int const)

find returns int

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+string const+
+----+------------+
+arg2+int const   +
+----+------------+


function|builtin|find to be documented

.. _function-_at__builtin_::find_first_of_string_hh_const_int_hh_const__hh_const:

.. das:function:: find_first_of(arg0: string const; arg1: int const)

find_first_of returns int

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+


function|builtin|find_first_of to be documented

.. _function-_at__builtin_::find_first_of_string_hh_const_string_hh_const__hh_const:

.. das:function:: find_first_of(arg0: string const; arg1: string const)

find_first_of returns int

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+string const+
+----+------------+


function|builtin|find_first_of to be documented

.. _function-_at__builtin_::float3x4:

.. das:function:: float3x4()

float3x4 returns  :ref:`builtin::float3x4 <handle-builtin-float3x4>` 

function|builtin|float3x4 to be documented

.. _function-_at__builtin_::float4x4:

.. das:function:: float4x4()

float4x4 returns  :ref:`builtin::float4x4 <handle-builtin-float4x4>` 

function|builtin|float4x4 to be documented

.. _function-_at__builtin_::format__hh_handle_hh_StringBuilderWriter_hh_implicit_string_hh_const_double_hh_const:

.. das:function:: format(arg0: StringBuilderWriter implicit; arg1: string const; arg2: double const)

arguments are

+----+-----------------------------------------------------------------------------------+
+arg0+ :ref:`builtin::StringBuilderWriter <handle-builtin-StringBuilderWriter>`  implicit+
+----+-----------------------------------------------------------------------------------+
+arg1+string const                                                                       +
+----+-----------------------------------------------------------------------------------+
+arg2+double const                                                                       +
+----+-----------------------------------------------------------------------------------+


function|builtin|format to be documented

.. _function-_at__builtin_::format__hh_handle_hh_StringBuilderWriter_hh_implicit_string_hh_const_float_hh_const:

.. das:function:: format(arg0: StringBuilderWriter implicit; arg1: string const; arg2: float const)

arguments are

+----+-----------------------------------------------------------------------------------+
+arg0+ :ref:`builtin::StringBuilderWriter <handle-builtin-StringBuilderWriter>`  implicit+
+----+-----------------------------------------------------------------------------------+
+arg1+string const                                                                       +
+----+-----------------------------------------------------------------------------------+
+arg2+float const                                                                        +
+----+-----------------------------------------------------------------------------------+


function|builtin|format to be documented

.. _function-_at__builtin_::format__hh_handle_hh_StringBuilderWriter_hh_implicit_string_hh_const_int_hh_const:

.. das:function:: format(arg0: StringBuilderWriter implicit; arg1: string const; arg2: int const)

arguments are

+----+-----------------------------------------------------------------------------------+
+arg0+ :ref:`builtin::StringBuilderWriter <handle-builtin-StringBuilderWriter>`  implicit+
+----+-----------------------------------------------------------------------------------+
+arg1+string const                                                                       +
+----+-----------------------------------------------------------------------------------+
+arg2+int const                                                                          +
+----+-----------------------------------------------------------------------------------+


function|builtin|format to be documented

.. _function-_at__builtin_::format__hh_handle_hh_StringBuilderWriter_hh_implicit_string_hh_const_int64_hh_const:

.. das:function:: format(arg0: StringBuilderWriter implicit; arg1: string const; arg2: int64 const)

arguments are

+----+-----------------------------------------------------------------------------------+
+arg0+ :ref:`builtin::StringBuilderWriter <handle-builtin-StringBuilderWriter>`  implicit+
+----+-----------------------------------------------------------------------------------+
+arg1+string const                                                                       +
+----+-----------------------------------------------------------------------------------+
+arg2+int64 const                                                                        +
+----+-----------------------------------------------------------------------------------+


function|builtin|format to be documented

.. _function-_at__builtin_::format__hh_handle_hh_StringBuilderWriter_hh_implicit_string_hh_const_uint_hh_const:

.. das:function:: format(arg0: StringBuilderWriter implicit; arg1: string const; arg2: uint const)

arguments are

+----+-----------------------------------------------------------------------------------+
+arg0+ :ref:`builtin::StringBuilderWriter <handle-builtin-StringBuilderWriter>`  implicit+
+----+-----------------------------------------------------------------------------------+
+arg1+string const                                                                       +
+----+-----------------------------------------------------------------------------------+
+arg2+uint const                                                                         +
+----+-----------------------------------------------------------------------------------+


function|builtin|format to be documented

.. _function-_at__builtin_::format__hh_handle_hh_StringBuilderWriter_hh_implicit_string_hh_const_uint64_hh_const:

.. das:function:: format(arg0: StringBuilderWriter implicit; arg1: string const; arg2: uint64 const)

arguments are

+----+-----------------------------------------------------------------------------------+
+arg0+ :ref:`builtin::StringBuilderWriter <handle-builtin-StringBuilderWriter>`  implicit+
+----+-----------------------------------------------------------------------------------+
+arg1+string const                                                                       +
+----+-----------------------------------------------------------------------------------+
+arg2+uint64 const                                                                       +
+----+-----------------------------------------------------------------------------------+


function|builtin|format to be documented

.. _function-_at__builtin_::format_string_hh_const_double_hh_const__hh_const:

.. das:function:: format(arg0: string const; arg1: double const)

format returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+double const+
+----+------------+


function|builtin|format to be documented

.. _function-_at__builtin_::format_string_hh_const_float_hh_const__hh_const:

.. das:function:: format(arg0: string const; arg1: float const)

format returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+float const +
+----+------------+


function|builtin|format to be documented

.. _function-_at__builtin_::format_string_hh_const_int_hh_const__hh_const:

.. das:function:: format(arg0: string const; arg1: int const)

format returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+


function|builtin|format to be documented

.. _function-_at__builtin_::format_string_hh_const_int64_hh_const__hh_const:

.. das:function:: format(arg0: string const; arg1: int64 const)

format returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int64 const +
+----+------------+


function|builtin|format to be documented

.. _function-_at__builtin_::format_string_hh_const_uint_hh_const__hh_const:

.. das:function:: format(arg0: string const; arg1: uint const)

format returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+uint const  +
+----+------------+


function|builtin|format to be documented

.. _function-_at__builtin_::format_string_hh_const_uint64_hh_const__hh_const:

.. das:function:: format(arg0: string const; arg1: uint64 const)

format returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+uint64 const+
+----+------------+


function|builtin|format to be documented

.. _function-_at__builtin_::gc0_reset:

.. das:function:: gc0_reset()

function|builtin|gc0_reset to be documented

.. _function-_at__builtin_::gc0_restore_ptr_string_hh_const__hh_const:

.. das:function:: gc0_restore_ptr(arg0: string const)

gc0_restore_ptr returns void?

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|builtin|gc0_restore_ptr to be documented

.. _function-_at__builtin_::gc0_restore_smart_ptr_string_hh_const__hh_const:

.. das:function:: gc0_restore_smart_ptr(arg0: string const)

gc0_restore_smart_ptr returns smart_ptr<void>

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|builtin|gc0_restore_smart_ptr to be documented

.. _function-_at__builtin_::gc0_save_ptr_string_hh_const__hh_ptr_hh_const_hh_implicit__hh_const__hh_const:

.. das:function:: gc0_save_ptr(arg0: string const; arg1: void? const implicit)

arguments are

+----+--------------------+
+arg0+string const        +
+----+--------------------+
+arg1+void? const implicit+
+----+--------------------+


function|builtin|gc0_save_ptr to be documented

.. _function-_at__builtin_::gc0_save_smart_ptr_string_hh_const__hh_smart_ptr_hh_void_hh_const_hh_implicit__hh_const__hh_const:

.. das:function:: gc0_save_smart_ptr(arg0: string const; arg1: smart_ptr<void> const implicit)

arguments are

+----+------------------------------+
+arg0+string const                  +
+----+------------------------------+
+arg1+smart_ptr<void> const implicit+
+----+------------------------------+


function|builtin|gc0_save_smart_ptr to be documented

.. _function-_at__builtin_::get_clock:

.. das:function:: get_clock()

get_clock returns  :ref:`builtin::clock <handle-builtin-clock>` 

function|builtin|get_clock to be documented

.. _function-_at__builtin_::get_das_root__hh_const:

.. das:function:: get_das_root()

get_das_root returns string

function|builtin|get_das_root to be documented

.. _function-_at__builtin_::hash_any:

.. das:function:: hash(arg0: any)

hash returns uint

arguments are

+----+---+
+arg0+any+
+----+---+


function|builtin|hash to be documented

.. _function-_at__builtin_::heap_bytes_allocated__hh_const:

.. das:function:: heap_bytes_allocated()

heap_bytes_allocated returns uint

function|builtin|heap_bytes_allocated to be documented

.. _function-_at__builtin_::heap_depth__hh_const:

.. das:function:: heap_depth()

heap_depth returns int

function|builtin|heap_depth to be documented

.. _function-_at__builtin_::heap_high_watermark__hh_const:

.. das:function:: heap_high_watermark()

heap_high_watermark returns uint

function|builtin|heap_high_watermark to be documented

.. _function-_at__builtin_::heap_report__hh_const:

.. das:function:: heap_report()

function|builtin|heap_report to be documented

.. _function-_at__builtin_::i_das_ptr_add__hh_ptr_hh_const_hh_implicit_int_hh_const_int_hh_const:

.. das:function:: i_das_ptr_add(arg0: void? const implicit; arg1: int const; arg2: int const)

i_das_ptr_add returns void?

.. warning:: 
  This is unsafe operation.

arguments are

+----+--------------------+
+arg0+void? const implicit+
+----+--------------------+
+arg1+int const           +
+----+--------------------+
+arg2+int const           +
+----+--------------------+


function|builtin|i_das_ptr_add to be documented

.. _function-_at__builtin_::i_das_ptr_dec__hh_ptr_hh_ref_hh_implicit_int_hh_const:

.. das:function:: i_das_ptr_dec(arg0: void?& implicit; arg1: int const)

.. warning:: 
  This is unsafe operation.

arguments are

+----+---------------+
+arg0+void?& implicit+
+----+---------------+
+arg1+int const      +
+----+---------------+


function|builtin|i_das_ptr_dec to be documented

.. _function-_at__builtin_::i_das_ptr_diff__hh_ptr_hh_const_hh_implicit__hh_ptr_hh_const_hh_implicit_int_hh_const:

.. das:function:: i_das_ptr_diff(arg0: void? const implicit; arg1: void? const implicit; arg2: int const)

i_das_ptr_diff returns int64

arguments are

+----+--------------------+
+arg0+void? const implicit+
+----+--------------------+
+arg1+void? const implicit+
+----+--------------------+
+arg2+int const           +
+----+--------------------+


function|builtin|i_das_ptr_diff to be documented

.. _function-_at__builtin_::i_das_ptr_inc__hh_ptr_hh_ref_hh_implicit_int_hh_const:

.. das:function:: i_das_ptr_inc(arg0: void?& implicit; arg1: int const)

.. warning:: 
  This is unsafe operation.

arguments are

+----+---------------+
+arg0+void?& implicit+
+----+---------------+
+arg1+int const      +
+----+---------------+


function|builtin|i_das_ptr_inc to be documented

.. _function-_at__builtin_::i_das_ptr_set_add__hh_ptr_hh_ref_hh_implicit_int_hh_const_int_hh_const:

.. das:function:: i_das_ptr_set_add(arg0: void?& implicit; arg1: int const; arg2: int const)

.. warning:: 
  This is unsafe operation.

arguments are

+----+---------------+
+arg0+void?& implicit+
+----+---------------+
+arg1+int const      +
+----+---------------+
+arg2+int const      +
+----+---------------+


function|builtin|i_das_ptr_set_add to be documented

.. _function-_at__builtin_::i_das_ptr_set_sub__hh_ptr_hh_ref_hh_implicit_int_hh_const_int_hh_const:

.. das:function:: i_das_ptr_set_sub(arg0: void?& implicit; arg1: int const; arg2: int const)

.. warning:: 
  This is unsafe operation.

arguments are

+----+---------------+
+arg0+void?& implicit+
+----+---------------+
+arg1+int const      +
+----+---------------+
+arg2+int const      +
+----+---------------+


function|builtin|i_das_ptr_set_sub to be documented

.. _function-_at__builtin_::i_das_ptr_sub__hh_ptr_hh_ref_hh_implicit_int_hh_const_int_hh_const:

.. das:function:: i_das_ptr_sub(arg0: void?& implicit; arg1: int const; arg2: int const)

i_das_ptr_sub returns void?

.. warning:: 
  This is unsafe operation.

arguments are

+----+---------------+
+arg0+void?& implicit+
+----+---------------+
+arg1+int const      +
+----+---------------+
+arg2+int const      +
+----+---------------+


function|builtin|i_das_ptr_sub to be documented

.. _function-_at__builtin_::identity__hh_handle_hh_float3x4_hh_implicit:

.. das:function:: identity(arg0: float3x4 implicit)

arguments are

+----+-------------------------------------------------------------+
+arg0+ :ref:`builtin::float3x4 <handle-builtin-float3x4>`  implicit+
+----+-------------------------------------------------------------+


function|builtin|identity to be documented

.. _function-_at__builtin_::identity__hh_handle_hh_float4x4_hh_implicit:

.. das:function:: identity(arg0: float4x4 implicit)

arguments are

+----+-------------------------------------------------------------+
+arg0+ :ref:`builtin::float4x4 <handle-builtin-float4x4>`  implicit+
+----+-------------------------------------------------------------+


function|builtin|identity to be documented

.. _function-_at__builtin_::inverse__hh_handle_hh_float3x4_hh_const_hh_implicit:

.. das:function:: inverse(arg0: float3x4 const implicit)

inverse returns  :ref:`builtin::float3x4 <handle-builtin-float3x4>` 

arguments are

+----+-------------------------------------------------------------------+
+arg0+ :ref:`builtin::float3x4 <handle-builtin-float3x4>`  const implicit+
+----+-------------------------------------------------------------------+


function|builtin|inverse to be documented

.. _function-_at__builtin_::is_alpha_int_hh_const:

.. das:function:: is_alpha(arg0: int const)

is_alpha returns bool

arguments are

+----+---------+
+arg0+int const+
+----+---------+


function|builtin|is_alpha to be documented

.. _function-_at__builtin_::is_char_in_set_int_hh_const_uint_hh_const_hh_implicit_hh_8:

.. das:function:: is_char_in_set(arg0: int const; arg1: uint const[8] implicit)

is_char_in_set returns bool

arguments are

+----+----------------------+
+arg0+int const             +
+----+----------------------+
+arg1+uint const[8] implicit+
+----+----------------------+


function|builtin|is_char_in_set to be documented

.. _function-_at__builtin_::is_compiling__hh_const:

.. das:function:: is_compiling()

is_compiling returns bool

function|builtin|is_compiling to be documented

.. _function-_at__builtin_::is_compiling_macros__hh_const:

.. das:function:: is_compiling_macros()

is_compiling_macros returns bool

function|builtin|is_compiling_macros to be documented

.. _function-_at__builtin_::is_compiling_macros_in_module_string_hh_const__hh_const:

.. das:function:: is_compiling_macros_in_module(arg0: string const)

is_compiling_macros_in_module returns bool

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|builtin|is_compiling_macros_in_module to be documented

.. _function-_at__builtin_::is_number_int_hh_const:

.. das:function:: is_number(arg0: int const)

is_number returns bool

arguments are

+----+---------+
+arg0+int const+
+----+---------+


function|builtin|is_number to be documented

.. _function-_at__builtin_::is_white_space_int_hh_const:

.. das:function:: is_white_space(arg0: int const)

is_white_space returns bool

arguments are

+----+---------+
+arg0+int const+
+----+---------+


function|builtin|is_white_space to be documented

.. _function-_at__builtin_::length__hh_array_hh_const_hh_implicit:

.. das:function:: length(arg0: array const implicit)

length returns int

arguments are

+----+--------------------+
+arg0+array const implicit+
+----+--------------------+


function|builtin|length to be documented

.. _function-_at__builtin_::length__hh_handle_hh_das_string_hh_implicit:

.. das:function:: length(arg0: das_string implicit)

length returns int

arguments are

+----+-----------------------------------------------------------------+
+arg0+ :ref:`builtin::das_string <handle-builtin-das_string>`  implicit+
+----+-----------------------------------------------------------------+


function|builtin|length to be documented

.. _function-_at__builtin_::length__hh_table_hh_const_hh_implicit:

.. das:function:: length(arg0: table const implicit)

length returns int

arguments are

+----+--------------------+
+arg0+table const implicit+
+----+--------------------+


function|builtin|length to be documented

.. _function-_at__builtin_::length_string_hh_const__hh_const:

.. das:function:: length(arg0: string const)

length returns int

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|builtin|length to be documented

.. _function-_at__builtin_::memcmp__hh_ptr_hh_const_hh_implicit__hh_ptr_hh_const_hh_implicit_int_hh_const:

.. das:function:: memcmp(arg0: void? const implicit; arg1: void? const implicit; arg2: int const)

memcmp returns int

.. warning:: 
  This is unsafe operation.

arguments are

+----+--------------------+
+arg0+void? const implicit+
+----+--------------------+
+arg1+void? const implicit+
+----+--------------------+
+arg2+int const           +
+----+--------------------+


function|builtin|memcmp to be documented

.. _function-_at__builtin_::panic_string_hh_const__hh_const:

.. das:function:: panic(arg0: string const)

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|builtin|panic to be documented

.. _function-_at__builtin_::peek__hh_handle_hh_das_string_hh_const_hh_implicit__hh_block_hh_string_hh_const_hh_temporary_hh_:void_hh_const_hh_implicit__hh_const:

.. das:function:: peek(arg0: das_string const implicit; arg1: block<(string const#):void> const implicit)

arguments are

+----+-----------------------------------------------------------------------+
+arg0+ :ref:`builtin::das_string <handle-builtin-das_string>`  const implicit+
+----+-----------------------------------------------------------------------+
+arg1+block<(string const#):void> const implicit                             +
+----+-----------------------------------------------------------------------+


function|builtin|peek to be documented

.. _function-_at__builtin_::print_string_hh_const__hh_const:

.. das:function:: print(arg0: string const)

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|builtin|print to be documented

.. _function-_at__builtin_::profile_int_hh_const_string_hh_const__hh_block_hh_const_hh_implicit__hh_const:

.. das:function:: profile(arg0: int const; arg1: string const; arg2: block<> const implicit)

profile returns float

arguments are

+----+----------------------+
+arg0+int const             +
+----+----------------------+
+arg1+string const          +
+----+----------------------+
+arg2+block<> const implicit+
+----+----------------------+


function|builtin|profile to be documented

.. _function-_at__builtin_::repeat_string_hh_const_int_hh_const__hh_const:

.. das:function:: repeat(arg0: string const; arg1: int const)

repeat returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+


function|builtin|repeat to be documented

.. _function-_at__builtin_::replace_string_hh_const_string_hh_const_string_hh_const__hh_const:

.. das:function:: replace(arg0: string const; arg1: string const; arg2: string const)

replace returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+string const+
+----+------------+
+arg2+string const+
+----+------------+


function|builtin|replace to be documented

.. _function-_at__builtin_::reset_profiler__hh_const:

.. das:function:: reset_profiler()

function|builtin|reset_profiler to be documented

.. _function-_at__builtin_::resize__hh_handle_hh_das_string_hh_implicit_int_hh_const:

.. das:function:: resize(arg0: das_string implicit; arg1: int const)

arguments are

+----+-----------------------------------------------------------------+
+arg0+ :ref:`builtin::das_string <handle-builtin-das_string>`  implicit+
+----+-----------------------------------------------------------------+
+arg1+int const                                                        +
+----+-----------------------------------------------------------------+


function|builtin|resize to be documented

.. _function-_at__builtin_::reverse_string_hh_const__hh_const:

.. das:function:: reverse(arg0: string const)

reverse returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|builtin|reverse to be documented

.. _function-_at__builtin_::rotate__hh_handle_hh_float3x4_hh_const_hh_implicit_float3_hh_const:

.. das:function:: rotate(arg0: float3x4 const implicit; arg1: float3 const)

rotate returns float3

arguments are

+----+-------------------------------------------------------------------+
+arg0+ :ref:`builtin::float3x4 <handle-builtin-float3x4>`  const implicit+
+----+-------------------------------------------------------------------+
+arg1+float3 const                                                       +
+----+-------------------------------------------------------------------+


function|builtin|rotate to be documented

.. _function-_at__builtin_::set__hh_handle_hh_das_string_hh_implicit_string_hh_const:

.. das:function:: set(arg0: das_string implicit; arg1: string const)

arguments are

+----+-----------------------------------------------------------------+
+arg0+ :ref:`builtin::das_string <handle-builtin-das_string>`  implicit+
+----+-----------------------------------------------------------------+
+arg1+string const                                                     +
+----+-----------------------------------------------------------------+


function|builtin|set to be documented

.. _function-_at__builtin_::set_variant_index__hh_variant_hh_implicit_int_hh_const:

.. das:function:: set_variant_index(arg0: variant<> implicit; arg1: int const)

.. warning:: 
  This is unsafe operation.

arguments are

+----+------------------+
+arg0+variant<> implicit+
+----+------------------+
+arg1+int const         +
+----+------------------+


function|builtin|set_variant_index to be documented

.. _function-_at__builtin_::slice_string_hh_const_int_hh_const__hh_const:

.. das:function:: slice(arg0: string const; arg1: int const)

slice returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+


function|builtin|slice to be documented

.. _function-_at__builtin_::slice_string_hh_const_int_hh_const_int_hh_const__hh_const:

.. das:function:: slice(arg0: string const; arg1: int const; arg2: int const)

slice returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+
+arg2+int const   +
+----+------------+


function|builtin|slice to be documented

.. _function-_at__builtin_::smart_ptr_clone__hh_smart_ptr_hh_void_hh_ref_hh_implicit__hh_ptr_hh_const_hh_implicit:

.. das:function:: smart_ptr_clone(arg0: smart_ptr<void>& implicit; arg1: void? const implicit)

arguments are

+----+-------------------------+
+arg0+smart_ptr<void>& implicit+
+----+-------------------------+
+arg1+void? const implicit     +
+----+-------------------------+


function|builtin|smart_ptr_clone to be documented

.. _function-_at__builtin_::smart_ptr_clone__hh_smart_ptr_hh_void_hh_ref_hh_implicit__hh_smart_ptr_hh_void_hh_const_hh_implicit:

.. das:function:: smart_ptr_clone(arg0: smart_ptr<void>& implicit; arg1: smart_ptr<void> const implicit)

arguments are

+----+------------------------------+
+arg0+smart_ptr<void>& implicit     +
+----+------------------------------+
+arg1+smart_ptr<void> const implicit+
+----+------------------------------+


function|builtin|smart_ptr_clone to be documented

.. _function-_at__builtin_::smart_ptr_use_count__hh_smart_ptr_hh_void_hh_const_hh_implicit:

.. das:function:: smart_ptr_use_count(arg0: smart_ptr<void> const implicit)

smart_ptr_use_count returns uint

arguments are

+----+------------------------------+
+arg0+smart_ptr<void> const implicit+
+----+------------------------------+


function|builtin|smart_ptr_use_count to be documented

.. _function-_at__builtin_::stackwalk__hh_const__hh_const:

.. das:function:: stackwalk()

function|builtin|stackwalk to be documented

.. _function-_at__builtin_::starts_with_string_hh_const_string_hh_const__hh_const:

.. das:function:: starts_with(arg0: string const; arg1: string const)

starts_with returns bool

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+string const+
+----+------------+


function|builtin|starts_with to be documented

.. _function-_at__builtin_::string_heap_bytes_allocated__hh_const:

.. das:function:: string_heap_bytes_allocated()

string_heap_bytes_allocated returns uint

function|builtin|string_heap_bytes_allocated to be documented

.. _function-_at__builtin_::string_heap_collect__hh_const__hh_const:

.. das:function:: string_heap_collect()

.. warning:: 
  This is unsafe operation.

function|builtin|string_heap_collect to be documented

.. _function-_at__builtin_::string_heap_depth__hh_const:

.. das:function:: string_heap_depth()

string_heap_depth returns int

function|builtin|string_heap_depth to be documented

.. _function-_at__builtin_::string_heap_high_watermark__hh_const:

.. das:function:: string_heap_high_watermark()

string_heap_high_watermark returns uint

function|builtin|string_heap_high_watermark to be documented

.. _function-_at__builtin_::string_heap_report__hh_const:

.. das:function:: string_heap_report()

function|builtin|string_heap_report to be documented

.. _function-_at__builtin_::strip_string_hh_const__hh_const:

.. das:function:: strip(arg0: string const)

strip returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|builtin|strip to be documented

.. _function-_at__builtin_::strip_left_string_hh_const__hh_const:

.. das:function:: strip_left(arg0: string const)

strip_left returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|builtin|strip_left to be documented

.. _function-_at__builtin_::strip_right_string_hh_const__hh_const:

.. das:function:: strip_right(arg0: string const)

strip_right returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|builtin|strip_right to be documented

.. _function-_at__builtin_::terminate__hh_const:

.. das:function:: terminate()

function|builtin|terminate to be documented

.. _function-_at__builtin_::to_char_int_hh_const__hh_const:

.. das:function:: to_char(arg0: int const)

to_char returns string

arguments are

+----+---------+
+arg0+int const+
+----+---------+


function|builtin|to_char to be documented

.. _function-_at__builtin_::to_float_string_hh_const:

.. das:function:: to_float(arg0: string const)

to_float returns float

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|builtin|to_float to be documented

.. _function-_at__builtin_::to_int_string_hh_const:

.. das:function:: to_int(arg0: string const)

to_int returns int

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|builtin|to_int to be documented

.. _function-_at__builtin_::to_lower_string_hh_const__hh_const:

.. das:function:: to_lower(arg0: string const)

to_lower returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|builtin|to_lower to be documented

.. _function-_at__builtin_::to_lower_in_place_string_hh_const:

.. das:function:: to_lower_in_place(arg0: string const)

to_lower_in_place returns string

.. warning:: 
  This is unsafe operation.

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|builtin|to_lower_in_place to be documented

.. _function-_at__builtin_::to_upper_string_hh_const__hh_const:

.. das:function:: to_upper(arg0: string const)

to_upper returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|builtin|to_upper to be documented

.. _function-_at__builtin_::to_upper_in_place_string_hh_const:

.. das:function:: to_upper_in_place(arg0: string const)

to_upper_in_place returns string

.. warning:: 
  This is unsafe operation.

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|builtin|to_upper_in_place to be documented

.. _function-_at__builtin_::translation_float3_hh_const:

.. das:function:: translation(arg0: float3 const)

translation returns  :ref:`builtin::float4x4 <handle-builtin-float4x4>` 

arguments are

+----+------------+
+arg0+float3 const+
+----+------------+


function|builtin|translation to be documented

.. _function-_at__builtin_::transpose__hh_handle_hh_float4x4_hh_const_hh_implicit:

.. das:function:: transpose(arg0: float4x4 const implicit)

transpose returns  :ref:`builtin::float4x4 <handle-builtin-float4x4>` 

arguments are

+----+-------------------------------------------------------------------+
+arg0+ :ref:`builtin::float4x4 <handle-builtin-float4x4>`  const implicit+
+----+-------------------------------------------------------------------+


function|builtin|transpose to be documented

.. _function-_at__builtin_::unescape_string_hh_const__hh_const:

.. das:function:: unescape(arg0: string const)

unescape returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|builtin|unescape to be documented

.. _function-_at__builtin_::variant_index__hh_variant_hh_const_hh_implicit:

.. das:function:: variant_index(arg0: variant<> const implicit)

variant_index returns int

arguments are

+----+------------------------+
+arg0+variant<> const implicit+
+----+------------------------+


function|builtin|variant_index to be documented

.. _function-_at__builtin_::write__hh_handle_hh_StringBuilderWriter_any:

.. das:function:: write(arg0: StringBuilderWriter; arg1: any)

arguments are

+----+--------------------------------------------------------------------------+
+arg0+ :ref:`builtin::StringBuilderWriter <handle-builtin-StringBuilderWriter>` +
+----+--------------------------------------------------------------------------+
+arg1+any                                                                       +
+----+--------------------------------------------------------------------------+


function|builtin|write to be documented

.. _function-_at__builtin_::write_char__hh_handle_hh_StringBuilderWriter_hh_implicit_int_hh_const:

.. das:function:: write_char(arg0: StringBuilderWriter implicit; arg1: int const)

arguments are

+----+-----------------------------------------------------------------------------------+
+arg0+ :ref:`builtin::StringBuilderWriter <handle-builtin-StringBuilderWriter>`  implicit+
+----+-----------------------------------------------------------------------------------+
+arg1+int const                                                                          +
+----+-----------------------------------------------------------------------------------+


function|builtin|write_char to be documented

.. _function-_at__builtin_::write_chars__hh_handle_hh_StringBuilderWriter_hh_implicit_int_hh_const_int_hh_const:

.. das:function:: write_chars(arg0: StringBuilderWriter implicit; arg1: int const; arg2: int const)

arguments are

+----+-----------------------------------------------------------------------------------+
+arg0+ :ref:`builtin::StringBuilderWriter <handle-builtin-StringBuilderWriter>`  implicit+
+----+-----------------------------------------------------------------------------------+
+arg1+int const                                                                          +
+----+-----------------------------------------------------------------------------------+
+arg2+int const                                                                          +
+----+-----------------------------------------------------------------------------------+


function|builtin|write_chars to be documented

.. _function-_at__builtin_::write_escape_string__hh_handle_hh_StringBuilderWriter_hh_implicit_string_hh_const:

.. das:function:: write_escape_string(arg0: StringBuilderWriter implicit; arg1: string const)

arguments are

+----+-----------------------------------------------------------------------------------+
+arg0+ :ref:`builtin::StringBuilderWriter <handle-builtin-StringBuilderWriter>`  implicit+
+----+-----------------------------------------------------------------------------------+
+arg1+string const                                                                       +
+----+-----------------------------------------------------------------------------------+


function|builtin|write_escape_string to be documented

.. _function-_at__builtin_::binary_load__hh_auto__hh_array_hh_uint8_hh_const:

.. das:function:: binary_load(obj: auto; data: array<uint8> const)

binary_load returns auto

arguments are

+----+------------------+
+obj +auto              +
+----+------------------+
+data+array<uint8> const+
+----+------------------+


function|builtin|binary_load to be documented

.. _function-_at__builtin_::binary_save__hh_auto_hh_const__hh_block_hh__hh_array_hh_uint8_hh_const_hh__rq_data_hh_:void_hh_const:

.. das:function:: binary_save(obj: auto const; subexpr: block<(data:array<uint8> const):void> const)

binary_save returns auto

arguments are

+-------+-------------------------------------------+
+obj    +auto const                                 +
+-------+-------------------------------------------+
+subexpr+block<(data:array<uint8> const):void> const+
+-------+-------------------------------------------+


function|builtin|binary_save to be documented

.. _function-_at__builtin_::clone__hh_array_hh__hh_auto__hh_array_hh__hh_alias_hh_TT_hh_const:

.. das:function:: clone(a: array<auto(TT)>; b: array<TT> const)

clone returns auto

arguments are

+-+---------------+
+a+array<auto(TT)>+
+-+---------------+
+b+array<TT> const+
+-+---------------+


function|builtin|clone to be documented

.. _function-_at__builtin_::clone__hh_array_hh__hh_auto__hh_array_hh__hh_alias_hh_TT_hh_const_hh_temporary:

.. das:function:: clone(a: array<auto(TT)>; b: array<TT> const#)

clone returns auto

arguments are

+-+----------------+
+a+array<auto(TT)> +
+-+----------------+
+b+array<TT> const#+
+-+----------------+


function|builtin|clone to be documented

.. _function-_at__builtin_::clone__hh_table_hh__hh_auto_hh__hh_auto__hh_table_hh__hh_alias_hh_KT_hh__hh_alias_hh_VT_hh_const:

.. das:function:: clone(a: table<auto(KT);auto(VT)>; b: table<KT;VT> const)

clone returns auto

arguments are

+-+------------------------+
+a+table<auto(KT);auto(VT)>+
+-+------------------------+
+b+table<KT;VT> const      +
+-+------------------------+


function|builtin|clone to be documented

.. _function-_at__builtin_::clone__hh_table_hh__hh_auto_hh__hh_auto__hh_table_hh__hh_alias_hh_KT_hh__hh_alias_hh_VT_hh_const_hh_temporary:

.. das:function:: clone(a: table<auto(KT);auto(VT)>; b: table<KT;VT> const#)

clone returns auto

arguments are

+-+------------------------+
+a+table<auto(KT);auto(VT)>+
+-+------------------------+
+b+table<KT;VT> const#     +
+-+------------------------+


function|builtin|clone to be documented

.. _function-_at__builtin_::clone_dim__hh_auto__hh_auto_hh_const:

.. das:function:: clone_dim(a: auto; b: auto const)

clone_dim returns auto

arguments are

+-+----------+
+a+auto      +
+-+----------+
+b+auto const+
+-+----------+


function|builtin|clone_dim to be documented

.. _function-_at__builtin_::clone_dim__hh_auto__hh_auto_hh_const_hh_temporary:

.. das:function:: clone_dim(a: auto; b: auto const#)

clone_dim returns auto

arguments are

+-+-----------+
+a+auto       +
+-+-----------+
+b+auto const#+
+-+-----------+


function|builtin|clone_dim to be documented

.. _function-_at__builtin_::clone_to_move__hh_auto_hh_const:

.. das:function:: clone_to_move(clone_src: auto(TT) const)

clone_to_move returns TT

arguments are

+---------+--------------+
+clone_src+auto(TT) const+
+---------+--------------+


function|builtin|clone_to_move to be documented

.. _function-_at__builtin_::each__hh_array_hh__hh_auto_hh_const:

.. das:function:: each(a: array<auto(TT)> const)

each returns iterator<TT&>

.. warning:: 
  This is unsafe operation.

arguments are

+-+---------------------+
+a+array<auto(TT)> const+
+-+---------------------+


function|builtin|each to be documented

.. _function-_at__builtin_::each__hh_auto_hh_const_hh_-1:

.. das:function:: each(a: auto(TT) const[-1])

each returns iterator<TT&>

.. warning:: 
  This is unsafe operation.

arguments are

+-+------------------+
+a+auto(TT) const[-1]+
+-+------------------+


function|builtin|each to be documented

.. _function-_at__builtin_::each__hh_lambda_hh__hh_auto_hh__rq_arg_hh_:bool_hh_const:

.. das:function:: each(lam: lambda<(arg:auto(argT) -const):bool> const)

each returns iterator<argT>

arguments are

+---+-----------------------------------+
+lam+lambda<(arg:auto(argT)):bool> const+
+---+-----------------------------------+


function|builtin|each to be documented

.. _function-_at__builtin_::each_range_hh_const:

.. das:function:: each(rng: range const)

each returns iterator<int>

arguments are

+---+-----------+
+rng+range const+
+---+-----------+


function|builtin|each to be documented

.. _function-_at__builtin_::each_string_hh_const:

.. das:function:: each(str: string const)

each returns iterator<int>

.. warning:: 
  This is unsafe operation.

arguments are

+---+------------+
+str+string const+
+---+------------+


function|builtin|each to be documented

.. _function-_at__builtin_::each_enum__hh_auto_hh_const:

.. das:function:: each_enum(tt: auto(TT) const)

each_enum returns iterator<TT>

arguments are

+--+--------------+
+tt+auto(TT) const+
+--+--------------+


function|builtin|each_enum to be documented

.. _function-_at__builtin_::each_ref__hh_lambda_hh__hh_ptr_hh__hh_auto_hh__rq_arg_hh_:bool_hh_const:

.. das:function:: each_ref(lam: lambda<(arg:auto(argT)? -const):bool> const)

each_ref returns iterator<argT&>

arguments are

+---+------------------------------------+
+lam+lambda<(arg:auto(argT)?):bool> const+
+---+------------------------------------+


function|builtin|each_ref to be documented

.. _function-_at__builtin_::emplace__hh_array_hh__hh_auto__hh_alias_hh_numT_int_hh_const:

.. das:function:: emplace(Arr: array<auto(numT)>; value: numT; at: int const)

emplace returns auto

arguments are

+-----+-----------------+
+Arr  +array<auto(numT)>+
+-----+-----------------+
+value+numT             +
+-----+-----------------+
+at   +int const        +
+-----+-----------------+


function|builtin|emplace to be documented

.. _function-_at__builtin_::erase__hh_array_hh__hh_auto_int_hh_const:

.. das:function:: erase(Arr: array<auto(numT)>; at: int const)

erase returns auto

arguments are

+---+-----------------+
+Arr+array<auto(numT)>+
+---+-----------------+
+at +int const        +
+---+-----------------+


function|builtin|erase to be documented

.. _function-_at__builtin_::erase__hh_table_hh__hh_auto_hh__hh_auto__hh_alias_hh_keyT_hh_const:

.. das:function:: erase(Tab: table<auto(keyT);auto(valT)>; at: keyT const)

erase returns bool

arguments are

+---+----------------------------+
+Tab+table<auto(keyT);auto(valT)>+
+---+----------------------------+
+at +keyT const                  +
+---+----------------------------+


function|builtin|erase to be documented

.. _function-_at__builtin_::finalize__hh_array_hh__hh_auto:

.. das:function:: finalize(a: array<auto(TT)>)

finalize returns auto

arguments are

+-+---------------+
+a+array<auto(TT)>+
+-+---------------+


function|builtin|finalize to be documented

.. _function-_at__builtin_::finalize__hh_table_hh__hh_auto_hh__hh_auto:

.. das:function:: finalize(a: table<auto(TK);auto(TV)>)

finalize returns auto

arguments are

+-+------------------------+
+a+table<auto(TK);auto(TV)>+
+-+------------------------+


function|builtin|finalize to be documented

.. _function-_at__builtin_::finalize_dim__hh_auto_hh_-1:

.. das:function:: finalize_dim(a: auto(TT)[-1])

finalize_dim returns auto

arguments are

+-+------------+
+a+auto(TT)[-1]+
+-+------------+


function|builtin|finalize_dim to be documented

.. _function-_at__builtin_::find__hh_table_hh__hh_auto_hh__hh_auto_hh_const__hh_alias_hh_keyT_hh_const:

.. das:function:: find(Tab: table<auto(keyT);auto(valT)> const; at: keyT const)

find returns valT const?

.. warning:: 
  This is unsafe operation.

arguments are

+---+----------------------------------+
+Tab+table<auto(keyT);auto(valT)> const+
+---+----------------------------------+
+at +keyT const                        +
+---+----------------------------------+


function|builtin|find to be documented

.. _function-_at__builtin_::find__hh_table_hh__hh_auto_hh__hh_auto_hh_const__hh_alias_hh_keyT_hh_const__hh_block_hh__hh_ptr_hh__hh_alias_hh_valT_hh_const_hh__rq_p_hh_:void_hh_const:

.. das:function:: find(Tab: table<auto(keyT);auto(valT)> const; at: keyT const; blk: block<(p:valT? const):void> const)

find returns auto

arguments are

+---+----------------------------------+
+Tab+table<auto(keyT);auto(valT)> const+
+---+----------------------------------+
+at +keyT const                        +
+---+----------------------------------+
+blk+block<(p:valT? const):void> const +
+---+----------------------------------+


function|builtin|find to be documented

.. _function-_at__builtin_::find__hh_table_hh__hh_auto_hh__hh_auto_hh_const_hh_temporary__hh_alias_hh_keyT_hh_const:

.. das:function:: find(Tab: table<auto(keyT);auto(valT)> const#; at: keyT const)

find returns valT const?#

arguments are

+---+-----------------------------------+
+Tab+table<auto(keyT);auto(valT)> const#+
+---+-----------------------------------+
+at +keyT const                         +
+---+-----------------------------------+


function|builtin|find to be documented

.. _function-_at__builtin_::find_for_edit__hh_table_hh__hh_auto_hh__hh_auto__hh_alias_hh_keyT_hh_const:

.. das:function:: find_for_edit(Tab: table<auto(keyT);auto(valT)>; at: keyT const)

find_for_edit returns valT?

.. warning:: 
  This is unsafe operation.

arguments are

+---+----------------------------+
+Tab+table<auto(keyT);auto(valT)>+
+---+----------------------------+
+at +keyT const                  +
+---+----------------------------+


function|builtin|find_for_edit to be documented

.. _function-_at__builtin_::find_for_edit__hh_table_hh__hh_auto_hh__hh_auto__hh_alias_hh_keyT_hh_const__hh_block_hh__hh_ptr_hh__hh_alias_hh_valT_hh__rq_p_hh_:void_hh_const:

.. das:function:: find_for_edit(Tab: table<auto(keyT);auto(valT)>; at: keyT const; blk: block<(p:valT? -const):void> const)

find_for_edit returns auto

arguments are

+---+----------------------------+
+Tab+table<auto(keyT);auto(valT)>+
+---+----------------------------+
+at +keyT const                  +
+---+----------------------------+
+blk+block<(p:valT?):void> const +
+---+----------------------------+


function|builtin|find_for_edit to be documented

.. _function-_at__builtin_::find_for_edit_if_exists__hh_table_hh__hh_auto_hh__hh_auto__hh_alias_hh_keyT_hh_const__hh_block_hh__hh_ptr_hh__hh_alias_hh_valT_hh__rq_p_hh_:void_hh_const:

.. das:function:: find_for_edit_if_exists(Tab: table<auto(keyT);auto(valT)>; at: keyT const; blk: block<(p:valT? -const):void> const)

find_for_edit_if_exists returns auto

arguments are

+---+----------------------------+
+Tab+table<auto(keyT);auto(valT)>+
+---+----------------------------+
+at +keyT const                  +
+---+----------------------------+
+blk+block<(p:valT?):void> const +
+---+----------------------------+


function|builtin|find_for_edit_if_exists to be documented

.. _function-_at__builtin_::find_if_exists__hh_table_hh__hh_auto_hh__hh_auto_hh_const__hh_alias_hh_keyT_hh_const__hh_block_hh__hh_ptr_hh__hh_alias_hh_valT_hh_const_hh__rq_p_hh_:void_hh_const:

.. das:function:: find_if_exists(Tab: table<auto(keyT);auto(valT)> const; at: keyT const; blk: block<(p:valT? const):void> const)

find_if_exists returns auto

arguments are

+---+----------------------------------+
+Tab+table<auto(keyT);auto(valT)> const+
+---+----------------------------------+
+at +keyT const                        +
+---+----------------------------------+
+blk+block<(p:valT? const):void> const +
+---+----------------------------------+


function|builtin|find_if_exists to be documented

.. _function-_at__builtin_::find_index__hh_array_hh__hh_auto_hh_const_hh_implicit__hh_alias_hh_TT_hh_const:

.. das:function:: find_index(arr: array<auto(TT)> const implicit; key: TT const)

find_index returns auto

arguments are

+---+------------------------------+
+arr+array<auto(TT)> const implicit+
+---+------------------------------+
+key+TT const                      +
+---+------------------------------+


function|builtin|find_index to be documented

.. _function-_at__builtin_::find_index__hh_auto_hh_const_hh_implicit_hh_-1__hh_alias_hh_TT_hh_const:

.. das:function:: find_index(arr: auto(TT) const[-1] implicit; key: TT const)

find_index returns auto

arguments are

+---+---------------------------+
+arr+auto(TT) const[-1] implicit+
+---+---------------------------+
+key+TT const                   +
+---+---------------------------+


function|builtin|find_index to be documented

.. _function-_at__builtin_::find_index_if__hh_array_hh__hh_auto_hh_const_hh_implicit__hh_block_hh__hh_alias_hh_TT_hh_const_hh__rq_key_hh_:bool_hh_const:

.. das:function:: find_index_if(arr: array<auto(TT)> const implicit; blk: block<(key:TT const):bool> const)

find_index_if returns auto

arguments are

+---+--------------------------------+
+arr+array<auto(TT)> const implicit  +
+---+--------------------------------+
+blk+block<(key:TT const):bool> const+
+---+--------------------------------+


function|builtin|find_index_if to be documented

.. _function-_at__builtin_::find_index_if__hh_auto_hh_const_hh_implicit_hh_-1__hh_block_hh__hh_alias_hh_TT_hh_const_hh__rq_key_hh_:bool_hh_const:

.. das:function:: find_index_if(arr: auto(TT) const[-1] implicit; blk: block<(key:TT const):bool> const)

find_index_if returns auto

arguments are

+---+--------------------------------+
+arr+auto(TT) const[-1] implicit     +
+---+--------------------------------+
+blk+block<(key:TT const):bool> const+
+---+--------------------------------+


function|builtin|find_index_if to be documented

.. _function-_at__builtin_::get_ptr__hh_smart_ptr_hh__hh_auto_hh_const:

.. das:function:: get_ptr(src: smart_ptr<auto(TT)> const)

get_ptr returns TT?

arguments are

+---+-------------------------+
+src+smart_ptr<auto(TT)> const+
+---+-------------------------+


function|builtin|get_ptr to be documented

.. _function-_at__builtin_::has_value__hh_auto_hh_const__hh_auto_hh_const:

.. das:function:: has_value(a: auto const; key: auto const)

has_value returns auto

arguments are

+---+----------+
+a  +auto const+
+---+----------+
+key+auto const+
+---+----------+


function|builtin|has_value to be documented

.. _function-_at__builtin_::intptr__hh_ptr_hh_void_hh_const:

.. das:function:: intptr(p: void? const)

intptr returns uint64

arguments are

+-+-----------+
+p+void? const+
+-+-----------+


function|builtin|intptr to be documented

.. _function-_at__builtin_::key_exists__hh_table_hh__hh_auto_hh__hh_auto_hh_const__hh_alias_hh_keyT_hh_const:

.. das:function:: key_exists(Tab: table<auto(keyT);auto(valT)> const; at: keyT const)

key_exists returns bool

arguments are

+---+----------------------------------+
+Tab+table<auto(keyT);auto(valT)> const+
+---+----------------------------------+
+at +keyT const                        +
+---+----------------------------------+


function|builtin|key_exists to be documented

.. _function-_at__builtin_::keys__hh_table_hh__hh_auto_hh__hh_auto_hh_const:

.. das:function:: keys(a: table<auto(keyT);auto(valT)> const)

keys returns iterator<keyT const&>

arguments are

+-+----------------------------------+
+a+table<auto(keyT);auto(valT)> const+
+-+----------------------------------+


function|builtin|keys to be documented

.. _function-_at__builtin_::length__hh_auto_hh_const_hh_-1:

.. das:function:: length(a: auto const[-1])

length returns int

arguments are

+-+--------------+
+a+auto const[-1]+
+-+--------------+


function|builtin|length to be documented

.. _function-_at__builtin_::lock__hh_table_hh__hh_auto_hh__hh_auto_hh_const__hh_block_hh__hh_table_hh__hh_alias_hh_keyT_hh__hh_alias_hh_valT_hh_const_hh_temporary_hh__rq_t_hh_:void_hh_const:

.. das:function:: lock(Tab: table<auto(keyT);auto(valT)> const; blk: block<(t:table<keyT;valT> const#):void> const)

lock returns auto

arguments are

+---+---------------------------------------------+
+Tab+table<auto(keyT);auto(valT)> const           +
+---+---------------------------------------------+
+blk+block<(t:table<keyT;valT> const#):void> const+
+---+---------------------------------------------+


function|builtin|lock to be documented

.. _function-_at__builtin_::lock__hh_table_hh__hh_auto_hh__hh_auto_hh_const_hh_temporary__hh_block_hh__hh_table_hh__hh_alias_hh_keyT_hh__hh_alias_hh_valT_hh_const_hh_temporary_hh__rq_t_hh_:void_hh_const:

.. das:function:: lock(Tab: table<auto(keyT);auto(valT)> const#; blk: block<(t:table<keyT;valT> const#):void> const)

lock returns auto

arguments are

+---+---------------------------------------------+
+Tab+table<auto(keyT);auto(valT)> const#          +
+---+---------------------------------------------+
+blk+block<(t:table<keyT;valT> const#):void> const+
+---+---------------------------------------------+


function|builtin|lock to be documented

.. _function-_at__builtin_::lock_forever__hh_table_hh__hh_auto_hh__hh_auto:

.. das:function:: lock_forever(Tab: table<auto(keyT);auto(valT)>)

lock_forever returns table<keyT;valT>#

arguments are

+---+----------------------------+
+Tab+table<auto(keyT);auto(valT)>+
+---+----------------------------+


function|builtin|lock_forever to be documented

.. _function-_at__builtin_::make_clone__hh_auto_hh_const:

.. das:function:: make_clone(res: auto(TT) const)

make_clone returns TT

arguments are

+---+--------------+
+res+auto(TT) const+
+---+--------------+


function|builtin|make_clone to be documented

.. _function-_at__builtin_::next__hh_iterator_hh__hh_auto_hh_const__hh_alias_hh_TT_hh_ref:

.. das:function:: next(it: iterator<auto(TT)> const; value: TT&)

next returns bool

arguments are

+-----+------------------------+
+it   +iterator<auto(TT)> const+
+-----+------------------------+
+value+TT&                     +
+-----+------------------------+


function|builtin|next to be documented

.. _function-_at__builtin_::nothing__hh_iterator_hh__hh_auto:

.. das:function:: nothing(it: iterator<auto(TT)>)

nothing returns iterator<TT>

arguments are

+--+------------------+
+it+iterator<auto(TT)>+
+--+------------------+


function|builtin|nothing to be documented

.. _function-_at__builtin_::pop__hh_array_hh__hh_auto:

.. das:function:: pop(Arr: array<auto(numT)>)

pop returns auto

arguments are

+---+-----------------+
+Arr+array<auto(numT)>+
+---+-----------------+


function|builtin|pop to be documented

.. _function-_at__builtin_::push__hh_array_hh__hh_auto__hh_alias_hh_numT_hh_const_int_hh_const:

.. das:function:: push(Arr: array<auto(numT)>; value: numT const; at: int const)

push returns auto

arguments are

+-----+-----------------+
+Arr  +array<auto(numT)>+
+-----+-----------------+
+value+numT const       +
+-----+-----------------+
+at   +int const        +
+-----+-----------------+


function|builtin|push to be documented

.. _function-_at__builtin_::push_clone__hh_array_hh__hh_auto__hh_alias_hh_numT_hh_const_int_hh_const:

.. das:function:: push_clone(Arr: array<auto(numT)>; value: numT const; at: int const)

push_clone returns auto

arguments are

+-----+-----------------+
+Arr  +array<auto(numT)>+
+-----+-----------------+
+value+numT const       +
+-----+-----------------+
+at   +int const        +
+-----+-----------------+


function|builtin|push_clone to be documented

.. _function-_at__builtin_::reserve__hh_array_hh__hh_auto_int_hh_const:

.. das:function:: reserve(Arr: array<auto(numT)>; newSize: int const)

reserve returns auto

arguments are

+-------+-----------------+
+Arr    +array<auto(numT)>+
+-------+-----------------+
+newSize+int const        +
+-------+-----------------+


function|builtin|reserve to be documented

.. _function-_at__builtin_::resize__hh_array_hh__hh_auto_int_hh_const:

.. das:function:: resize(Arr: array<auto(numT)>; newSize: int const)

resize returns auto

arguments are

+-------+-----------------+
+Arr    +array<auto(numT)>+
+-------+-----------------+
+newSize+int const        +
+-------+-----------------+


function|builtin|resize to be documented

.. _function-_at__builtin_::sort__hh_array_hh__hh_auto:

.. das:function:: sort(a: array<auto(TT)>)

sort returns auto

arguments are

+-+---------------+
+a+array<auto(TT)>+
+-+---------------+


function|builtin|sort to be documented

.. _function-_at__builtin_::sort__hh_array_hh__hh_auto__hh_block_hh__hh_alias_hh_TT_hh_const_hh__hh_alias_hh_TT_hh_const_hh__rq_x_hh__rq_y_hh_:bool_hh_const:

.. das:function:: sort(a: array<auto(TT)>; cmp: block<(x:TT const;y:TT const):bool> const)

sort returns auto

arguments are

+---+-----------------------------------------+
+a  +array<auto(TT)>                          +
+---+-----------------------------------------+
+cmp+block<(x:TT const;y:TT const):bool> const+
+---+-----------------------------------------+


function|builtin|sort to be documented

.. _function-_at__builtin_::sort__hh_auto_hh_-1:

.. das:function:: sort(a: auto(TT)[-1])

sort returns auto

arguments are

+-+------------+
+a+auto(TT)[-1]+
+-+------------+


function|builtin|sort to be documented

.. _function-_at__builtin_::sort__hh_auto_hh_-1__hh_block_hh__hh_alias_hh_TT_hh_const_hh__hh_alias_hh_TT_hh_const_hh__rq_x_hh__rq_y_hh_:bool_hh_const:

.. das:function:: sort(a: auto(TT)[-1]; cmp: block<(x:TT const;y:TT const):bool> const)

sort returns auto

arguments are

+---+-----------------------------------------+
+a  +auto(TT)[-1]                             +
+---+-----------------------------------------+
+cmp+block<(x:TT const;y:TT const):bool> const+
+---+-----------------------------------------+


function|builtin|sort to be documented

.. _function-_at__builtin_::to_array__hh_auto_hh_const_hh_-1:

.. das:function:: to_array(a: auto(TT) const[-1])

to_array returns array<TT>

arguments are

+-+------------------+
+a+auto(TT) const[-1]+
+-+------------------+


function|builtin|to_array to be documented

.. _function-_at__builtin_::to_array__hh_iterator_hh__hh_auto_hh_const:

.. das:function:: to_array(it: iterator<auto(TT)> const)

to_array returns array<TT>

arguments are

+--+------------------------+
+it+iterator<auto(TT)> const+
+--+------------------------+


function|builtin|to_array to be documented

.. _function-_at__builtin_::to_array_move__hh_auto_hh_-1:

.. das:function:: to_array_move(a: auto(TT)[-1])

to_array_move returns array<TT>

arguments are

+-+------------+
+a+auto(TT)[-1]+
+-+------------+


function|builtin|to_array_move to be documented

.. _function-_at__builtin_::to_table__hh_tuple_hh__hh_auto_hh__hh_auto_hh_const_hh_-1:

.. das:function:: to_table(a: tuple<auto(keyT);auto(valT)> const[-1])

to_table returns table<keyT;valT>

arguments are

+-+--------------------------------------+
+a+tuple<auto(keyT);auto(valT)> const[-1]+
+-+--------------------------------------+


function|builtin|to_table to be documented

.. _function-_at__builtin_::to_table_move__hh_tuple_hh__hh_auto_hh__hh_auto_hh_-1:

.. das:function:: to_table_move(a: tuple<auto(keyT);auto(valT)>[-1])

to_table_move returns table<keyT;valT>

arguments are

+-+--------------------------------+
+a+tuple<auto(keyT);auto(valT)>[-1]+
+-+--------------------------------+


function|builtin|to_table_move to be documented

.. _function-_at__builtin_::values__hh_table_hh__hh_auto_hh__hh_auto_hh_const_hh_explicitconst:

.. das:function:: values(a: table<auto(keyT);auto(valT)> const!)

values returns iterator<valT const&>

arguments are

+-+-----------------------------------+
+a+table<auto(keyT);auto(valT)> const!+
+-+-----------------------------------+


function|builtin|values to be documented

.. _function-_at__builtin_::values__hh_table_hh__hh_auto_hh__hh_auto_hh_explicitconst:

.. das:function:: values(a: table<auto(keyT);auto(valT)>!)

values returns iterator<valT&>

arguments are

+-+-----------------------------+
+a+table<auto(keyT);auto(valT)>!+
+-+-----------------------------+


function|builtin|values to be documented


