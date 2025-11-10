
.. _stdlib__builtin:

================
Built-in runtime
================

Builtin module is automatically required by any other das file. It includes basic language infrastructure,
support for containers, heap, miscellaneous iterators, profiler, and interaction with host application.


++++++++++++
Type aliases
++++++++++++

.. _alias-print_flags:

.. das:attribute:: bitfield print_flags

This bitfield specifies how exactly values are to be printed

:Fields: * **escapeString** (0x1) - if string is to be escaped

         * **namesAndDimensions** (0x2) - names of the fields and dimensions of the arrays

         * **typeQualifiers** (0x4) - type qualifiers for the specific types like double and uint64

         * **refAddresses** (0x8) - addresses in hexadecimal of each reference value

         * **singleLine** (0x10) - human readable vs single line

         * **fixedPoint** (0x20) - always output fixed point precision for floating point values


+++++++++
Constants
+++++++++

.. _global-builtin-DAS_MAX_FUNCTION_ARGUMENTS:

.. das:attribute:: DAS_MAX_FUNCTION_ARGUMENTS = 32

 maximum number of arguments for the function. this is used to pre-allocate stack space for the function arguments

.. _global-builtin-INT_MIN:

.. das:attribute:: INT_MIN = -2147483648

 minimum possible value of 'int'

.. _global-builtin-INT_MAX:

.. das:attribute:: INT_MAX = 2147483647

 maximum possible value of 'int'

.. _global-builtin-UINT_MAX:

.. das:attribute:: UINT_MAX = 0xffffffff

 maximum possible value of 'uint'

.. _global-builtin-LONG_MIN:

.. das:attribute:: LONG_MIN = -9223372036854775808

 minimum possible value of 'int64'

.. _global-builtin-LONG_MAX:

.. das:attribute:: LONG_MAX = 9223372036854775807

 maximum possible value of 'int64'

.. _global-builtin-ULONG_MAX:

.. das:attribute:: ULONG_MAX = 0xffffffffffffffff

 minimum possible value of 'uint64'

.. _global-builtin-FLT_MIN:

.. das:attribute:: FLT_MIN = 1.1754944e-38f

 smallest possible non-zero value of 'float'. if u want minimum possible value use `-FLT_MAX`

.. _global-builtin-FLT_MAX:

.. das:attribute:: FLT_MAX = 3.4028235e+38f

 maximum possible value of 'float'

.. _global-builtin-DBL_MIN:

.. das:attribute:: DBL_MIN = 2.2250738585072014e-308lf

 smallest possible non-zero value of 'double'. if u want minimum possible value use `-DBL_MAX`

.. _global-builtin-DBL_MAX:

.. das:attribute:: DBL_MAX = 1.7976931348623157e+308lf

 maximum possible value of 'double'

.. _global-builtin-LOG_CRITICAL:

.. das:attribute:: LOG_CRITICAL = 50000

 indicates maximum log level. critial errors, panic, shutdown

.. _global-builtin-LOG_ERROR:

.. das:attribute:: LOG_ERROR = 40000

 indicates log level recoverable errors

.. _global-builtin-LOG_WARNING:

.. das:attribute:: LOG_WARNING = 30000

 indicates log level for API misuse, non-fatal errors

.. _global-builtin-LOG_INFO:

.. das:attribute:: LOG_INFO = 20000

 indicates log level for miscellaneous informative messages

.. _global-builtin-LOG_DEBUG:

.. das:attribute:: LOG_DEBUG = 10000

 indicates log level for debug messages

.. _global-builtin-LOG_TRACE:

.. das:attribute:: LOG_TRACE = 0

 indicates log level for the most noisy debug and tracing messages

.. _global-builtin-VEC_SEP:

.. das:attribute:: VEC_SEP = ","

 Read-only string constant which is used to separate elements of vectors. By default its ","

.. _global-builtin-print_flags_debugger:

.. das:attribute:: print_flags_debugger = bitfield(0xf)

 printing flags similar to those used by the 'debug' function

++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-builtin-HashBuilder:

.. das:attribute:: HashBuilder

 Helper structure to facilitate calculating hash values.


++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-builtin-marker:

.. das:attribute:: marker

 marker annotation is used to attach arbitrary marker values to a function (in form of annotation arguments). its typically used for implementation of macros

.. _handle-builtin-generic:

.. das:attribute:: generic

 indicates that the function is generic, regardless of its argument types. generic functions will be instanced in the calling module

.. _handle-builtin-_macro:

.. das:attribute:: _macro

 indicates that the function will be called during the macro pass, similar to `[init]`

.. _handle-builtin-macro_function:

.. das:attribute:: macro_function

 indicates that the function is part of the macro implementation, and will not be present in the final compiled context, unless explicitly called.

.. _handle-builtin-hint:

.. das:attribute:: hint

 Hints the compiler to use specific optimization.

.. _handle-builtin-jit:

.. das:attribute:: jit

 Explicitly marks (forces) function to be compiled with JIT compiler.

.. _handle-builtin-no_jit:

.. das:attribute:: no_jit

 Disables JIT compilation for the function.

.. _handle-builtin-nodiscard:

.. das:attribute:: nodiscard

 Marks function as nodiscard. Result of the function should be used.

.. _handle-builtin-deprecated:

.. das:attribute:: deprecated

 deprecated annotation is used to mark a function as deprecated. it will generate a warning during compilation, and will not be callable from the final compiled context

.. _handle-builtin-alias_cmres:

.. das:attribute:: alias_cmres

 indicates that function always aliases cmres (copy or move result), and cmres optimizations are disabled.

.. _handle-builtin-never_alias_cmres:

.. das:attribute:: never_alias_cmres

 indicates that function never aliases cmres (copy or move result), and cmres checks will not be performed

.. _handle-builtin-export:

.. das:attribute:: export

 indicates that function is to be exported to the final compiled context

.. _handle-builtin-pinvoke:

.. das:attribute:: pinvoke

 indicates that the function is a pinvoke function, and will be called via pinvoke machinery

.. _handle-builtin-no_lint:

.. das:attribute:: no_lint

 indicates that the lint pass should be skipped for the specific function

.. _handle-builtin-sideeffects:

.. das:attribute:: sideeffects

 indicates that the function should be treated as if it has side-effects. for example it will not be optimized out

.. _handle-builtin-run:

.. das:attribute:: run

 ensures that the function is always evaluated at compilation time

.. _handle-builtin-unsafe_operation:

.. das:attribute:: unsafe_operation

 indicates that function is unsafe, and will require `unsafe` keyword to be called

.. _handle-builtin-unsafe_outside_of_for:

.. das:attribute:: unsafe_outside_of_for

 Marks function as unsafe to be called outside of the sources `for` loop.

.. _handle-builtin-unsafe_when_not_clone_array:

.. das:attribute:: unsafe_when_not_clone_array

 Marks function as unsafe to be called outside of the `clone` of the array.

.. _handle-builtin-no_aot:

.. das:attribute:: no_aot

 indicates that the AOT will not be generated for this specific function

.. _handle-builtin-init:

.. das:attribute:: init

 indicates that the function would be called at the context initialization time

.. _handle-builtin-finalize:

.. das:attribute:: finalize

 indicates that the function would be called at the context shutdown time

.. _handle-builtin-hybrid:

.. das:attribute:: hybrid

 indicates that the function is likely candidate for later patching, and the AOT will generate hybrid calls to it - instead of direct calls. that way modyfing the function will not affect AOT of other functions.

.. _handle-builtin-unsafe_deref:

.. das:attribute:: unsafe_deref

 optimization, which indicates that pointer dereference, array and string indexing, and few other operations would not check for null or bounds

.. _handle-builtin-skip_lock_check:

.. das:attribute:: skip_lock_check

 optimization, which indicates that lock checks are not needed in this function.

.. _handle-builtin-unused_argument:

.. das:attribute:: unused_argument

 marks function arguments, which are unused. that way when code policies make unused arguments an error, a workaround can be provided

.. _handle-builtin-local_only:

.. das:attribute:: local_only

 indicates that function can only accept local `make` expressions, like [[make tuple]] and [[make structure]]

.. _handle-builtin-expect_any_vector:

.. das:attribute:: expect_any_vector

 indicates that function can only accept das::vector templates

.. _handle-builtin-expect_dim:

.. das:attribute:: expect_dim

 A contract to mark function argument to be a static array.

.. _handle-builtin-expect_ref:

.. das:attribute:: expect_ref

 A contract to mark function argument to be a reference.

.. _handle-builtin-type_function:

.. das:attribute:: type_function

 Marks function as a type function.

.. _handle-builtin-builtin_array_sort:

.. das:attribute:: builtin_array_sort

 indicates sort function for builtin 'sort' machinery. used internally

+++++++++++
Call macros
+++++++++++

.. _call-macro-builtin-make_function_unsafe:

.. das:attribute:: make_function_unsafe

 Marks function from which this is called from as unsafe.

.. _call-macro-builtin-concept_assert:

.. das:attribute:: concept_assert

 similar to regular `assert` function, but always happens at compilation time. it would also display the error message from where the asserted function was called from, not the assert line itself.

.. _call-macro-builtin-__builtin_table_set_insert:

.. das:attribute:: __builtin_table_set_insert

 part of internal implementation for `insert` of the sets (tables with keys only).

.. _call-macro-builtin-__builtin_table_key_exists:

.. das:attribute:: __builtin_table_key_exists

 part of internal implementation for `key_exists`

.. _call-macro-builtin-static_assert:

.. das:attribute:: static_assert

 similar to regular `assert` function, but always happens at compilation time

.. _call-macro-builtin-verify:

.. das:attribute:: verify

 assert for the expression with side effects. expression will not be optimized out if asserts are disabled

.. _call-macro-builtin-debug:

.. das:attribute:: debug

 prints value and returns that same value

.. _call-macro-builtin-assert:

.. das:attribute:: assert

 throws panic if first operand is false. can be disabled. second operand is error message

.. _call-macro-builtin-memzero:

.. das:attribute:: memzero

 initializes section of memory with '0'

.. _call-macro-builtin-__builtin_table_find:

.. das:attribute:: __builtin_table_find

 part of internal implementation for `find`

.. _call-macro-builtin-invoke:

.. das:attribute:: invoke

 invokes block, function, or lambda

.. _call-macro-builtin-__builtin_table_erase:

.. das:attribute:: __builtin_table_erase

 part of internal implementation for `erase`

+++++++++++++
Reader macros
+++++++++++++

.. _call-macro-builtin-_esc:

.. das:attribute:: _esc

 returns raw string input, without regards for escape sequences. For example %_esc\\n\\r%_esc will return 4 character string '\\','n','\\','r'

+++++++++++++++
Typeinfo macros
+++++++++++++++

.. _call-macro-builtin-rtti_classinfo:

.. das:attribute:: rtti_classinfo

 Generates TypeInfo for the class initialization.

+++++++++++++
Handled types
+++++++++++++

.. _handle-builtin-das_string:

.. das:attribute:: das_string

 das::string which is typically std::string or equivalent

.. _handle-builtin-clock:

.. das:attribute:: clock

 das::Time which is a wrapper around `time_t`

++++++++++++++++
Structure macros
++++++++++++++++

.. _handle-builtin-comment:

.. das:attribute:: comment

 [comment] macro, which does absolutely nothing but holds arguments.

.. _handle-builtin-no_default_initializer:

.. das:attribute:: no_default_initializer

 [no_default_initializer] specifies that structure will not have default initializer generated by the compiler.

.. _handle-builtin-macro_interface:

.. das:attribute:: macro_interface

 [macro_interface] specifies that class and its inherited children are used as a macro interfaces, and would not be exported by default.

.. _handle-builtin-skip_field_lock_check:

.. das:attribute:: skip_field_lock_check

 optimization, which indicates that the structure does not need lock checks.

.. _handle-builtin-cpp_layout:

.. das:attribute:: cpp_layout

 [cpp_layout] specifies that structure uses C++ memory layout rules, as oppose to native Daslang memory layout rules.

.. _handle-builtin-safe_when_uninitialized:

.. das:attribute:: safe_when_uninitialized

 Marks structure as safe to be used when uninitialized.

.. _handle-builtin-persistent:

.. das:attribute:: persistent

 [persistent] annotation specifies that structure is allocated (via new) on the C++ heap, as oppose to Daslang context heap.

++++++++++
Containers
++++++++++

  *  :ref:`clear (array: array\<anything\>) <function-builtin_clear_array_ls_anything_gr_>` 
  *  :ref:`length (array: array\<anything\>) : int <function-builtin_length_array_ls_anything_gr_>` 
  *  :ref:`capacity (array: array\<anything\>) : int <function-builtin_capacity_array_ls_anything_gr_>` 
  *  :ref:`empty (iterator: iterator implicit) : bool <function-builtin_empty_iterator_implicit>` 
  *  :ref:`length (table: table\<anything, anything\>) : int <function-builtin_length_table_ls_anything,_anything_gr_>` 
  *  :ref:`capacity (table: table\<anything, anything\>) : int <function-builtin_capacity_table_ls_anything,_anything_gr_>` 
  *  :ref:`empty (str: string implicit) : bool <function-builtin_empty_string_implicit>` 
  *  :ref:`empty (str: das_string implicit) : bool <function-builtin_empty_das_string_implicit>` 
  *  :ref:`resize (var Arr: array\<auto(numT)\>; newSize: int) : auto <function-builtin_resize_array_ls_autonumT_gr__int>` 
  *  :ref:`resize_and_init (var Arr: array\<auto(numT)\>; newSize: int) : auto <function-builtin_resize_and_init_array_ls_autonumT_gr__int>` 
  *  :ref:`resize_and_init (var Arr: array\<auto(numT)\>; newSize: int; initValue: numT) : auto <function-builtin_resize_and_init_array_ls_autonumT_gr__int_numT>` 
  *  :ref:`resize_no_init (var Arr: array\<auto(numT)\>; newSize: int) : auto <function-builtin_resize_no_init_array_ls_autonumT_gr__int>` 
  *  :ref:`reserve (var Arr: array\<auto(numT)\>; newSize: int) : auto <function-builtin_reserve_array_ls_autonumT_gr__int>` 
  *  :ref:`pop (var Arr: array\<auto(numT)\>) : auto <function-builtin_pop_array_ls_autonumT_gr_>` 
  *  :ref:`push (var Arr: array\<auto(numT)\>; value: numT ==const; at: int) : auto <function-builtin_push_array_ls_autonumT_gr__numT__eq__eq_const_int>` 
  *  :ref:`push (var Arr: array\<auto(numT)\>; var value: numT ==const; at: int) : auto <function-builtin_push_array_ls_autonumT_gr___numT__eq__eq_const_int>` 
  *  :ref:`push (var Arr: array\<auto(numT)\>; value: numT ==const) : auto <function-builtin_push_array_ls_autonumT_gr__numT__eq__eq_const>` 
  *  :ref:`push (var Arr: array\<auto(numT)\>; var value: numT ==const) : auto <function-builtin_push_array_ls_autonumT_gr___numT__eq__eq_const>` 
  *  :ref:`push (var Arr: array\<auto(numT)\>; varr: array\<numT\>) : auto <function-builtin_push_array_ls_autonumT_gr__array_ls_numT_gr_>` 
  *  :ref:`push (var Arr: array\<auto(numT)\>; var varr: array\<numT\>) : auto <function-builtin_push_array_ls_autonumT_gr___array_ls_numT_gr_>` 
  *  :ref:`push (var Arr: array\<auto(numT)\>; varr: numT[]) : auto <function-builtin_push_array_ls_autonumT_gr__numT>` 
  *  :ref:`push (var Arr: array\<auto(numT)[]\>; varr: numT const[] ==const) : auto <function-builtin_push_array_ls_autonumT_gr__numT_const__eq__eq_const>` 
  *  :ref:`push (var Arr: array\<auto(numT)[]\>; var varr: numT[] ==const) : auto <function-builtin_push_array_ls_autonumT_gr___numT__eq__eq_const>` 
  *  :ref:`emplace (var Arr: array\<auto(numT)\>; var value: numT&; at: int) : auto <function-builtin_emplace_array_ls_autonumT_gr__numT_int>` 
  *  :ref:`emplace (var Arr: array\<auto(numT)\>; var value: numT&) : auto <function-builtin_emplace_array_ls_autonumT_gr__numT>` 
  *  :ref:`emplace (var Arr: array\<auto(numT)\>; var value: numT[]) : auto <function-builtin_emplace_array_ls_autonumT_gr__numT>` 
  *  :ref:`emplace (var Arr: array\<auto(numT)[]\>; var value: numT[]) : auto <function-builtin_emplace_array_ls_autonumT_gr__numT>` 
  *  :ref:`push_clone (var Arr: array\<auto(numT)\>; value: numT ==const|numT const# ==const; at: int) : auto <function-builtin_push_clone_array_ls_autonumT_gr__numT__eq__eq_constnumT_const_hh___eq__eq_const_int>` 
  *  :ref:`push_clone (var Arr: array\<auto(numT)\>; var value: numT ==const|numT# ==const; at: int) : auto <function-builtin_push_clone_array_ls_autonumT_gr__numT__eq__eq_constnumT_hh___eq__eq_const_int>` 
  *  :ref:`push_clone (var Arr: array\<auto(numT)\>; value: numT ==const|numT const# ==const) : auto <function-builtin_push_clone_array_ls_autonumT_gr__numT__eq__eq_constnumT_const_hh___eq__eq_const>` 
  *  :ref:`push_clone (var Arr: array\<auto(numT)\>; var value: numT ==const|numT# ==const) : auto <function-builtin_push_clone_array_ls_autonumT_gr__numT__eq__eq_constnumT_hh___eq__eq_const>` 
  *  :ref:`push_clone (var Arr: array\<auto(numT)\>; varr: numT const[] ==const) : auto <function-builtin_push_clone_array_ls_autonumT_gr__numT_const__eq__eq_const>` 
  *  :ref:`push_clone (var Arr: array\<auto(numT)\>; var varr: numT[] ==const) : auto <function-builtin_push_clone_array_ls_autonumT_gr___numT__eq__eq_const>` 
  *  :ref:`push_clone (var Arr: array\<auto(numT)[]\>; varr: numT const[] ==const) : auto <function-builtin_push_clone_array_ls_autonumT_gr__numT_const__eq__eq_const>` 
  *  :ref:`push_clone (var Arr: array\<auto(numT)[]\>; var varr: numT[]) : auto <function-builtin_push_clone_array_ls_autonumT_gr__numT>` 
  *  :ref:`push_clone (var A: auto(CT); b: auto(TT)|auto(TT)#) : auto <function-builtin_push_clone_autoCT_autoTTautoTT_hh_>` 
  *  :ref:`back (var a: array\<auto(TT)\>) : TT& <function-builtin_back__array_ls_autoTT_gr_>` 
  *  :ref:`back (var a: array\<auto(TT)\>#) : TT&# <function-builtin_back__array_ls_autoTT_gr__hh_>` 
  *  :ref:`back (a: array\<auto(TT)\>) : TT <function-builtin_back_array_ls_autoTT_gr_>` 
  *  :ref:`back (a: array\<auto(TT)\>#) : TT const&# <function-builtin_back_array_ls_autoTT_gr__hh_>` 
  *  :ref:`back (var arr: auto(TT) ==const) : auto& <function-builtin_back__autoTT__eq__eq_const>` 
  *  :ref:`back (arr: auto(TT) ==const) : auto <function-builtin_back_autoTT__eq__eq_const>` 
  *  :ref:`erase (var Arr: array\<auto(numT)\>; at: int) : auto <function-builtin_erase_array_ls_autonumT_gr__int>` 
  *  :ref:`erase (var Arr: array\<auto(numT)\>; at: int; count: int) : auto <function-builtin_erase_array_ls_autonumT_gr__int_int>` 
  *  :ref:`erase_if (var arr: array\<auto(TT)\>; blk: block\<(key:TT const):bool\>|block\<(var key:TT&):bool\>) : auto <function-builtin_erase_if_array_ls_autoTT_gr__block_ls_key_c_TT_const_c_bool_gr_block_ls_var_key_c_TT_c_bool_gr_>` 
  *  :ref:`remove_value (var arr: array\<auto(TT)\>|array\<auto(TT)\>#; key: TT) : bool <function-builtin_remove_value_array_ls_autoTT_gr_array_ls_autoTT_gr__hh__TT>` 
  *  :ref:`length (a: auto|auto#) : int <function-builtin_length_autoauto_hh_>` 
  *  :ref:`empty (a: array\<auto\>|array\<auto\>#) : bool <function-builtin_empty_array_ls_auto_gr_array_ls_auto_gr__hh_>` 
  *  :ref:`empty (a: table\<auto;auto\>|table\<auto;auto\>#) : bool <function-builtin_empty_table_ls_auto;auto_gr_table_ls_auto;auto_gr__hh_>` 
  *  :ref:`reserve (var Tab: table\<auto(keyT), auto\>; newSize: int) : auto <function-builtin_reserve_table_ls_autokeyT,_auto_gr__int>` 
  *  :ref:`get (Tab: table\<auto(keyT), auto(valT)\>#; at: keyT|keyT#; blk: block\<(p:valT const&#):void\>) : auto <function-builtin_get_table_ls_autokeyT,_autovalT_gr__hh__keyTkeyT_hh__block_ls_p_c_valT_const_hh__c_void_gr_>` 
  *  :ref:`get (Tab: table\<auto(keyT), auto(valT)\>; at: keyT|keyT#; blk: block\<(p:valT):void\>) : auto <function-builtin_get_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__block_ls_p_c_valT_c_void_gr_>` 
  *  :ref:`get (var Tab: table\<auto(keyT), auto(valT)\>#; at: keyT|keyT#; blk: block\<(var p:valT&#):void\>) : auto <function-builtin_get__table_ls_autokeyT,_autovalT_gr__hh__keyTkeyT_hh__block_ls_var_p_c_valT_hh__c_void_gr_>` 
  *  :ref:`get (var Tab: table\<auto(keyT), auto(valT)\>; at: keyT|keyT#; blk: block\<(var p:valT&):void\>) : auto <function-builtin_get__table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__block_ls_var_p_c_valT_c_void_gr_>` 
  *  :ref:`get (Tab: table\<auto(keyT), auto(valT)[]\>#; at: keyT|keyT#; blk: block\<(p:valT const[-2]&#):void\>) : auto <function-builtin_get_table_ls_autokeyT,_autovalT_gr__hh__keyTkeyT_hh__block_ls_p_c_valT_const-2_hh__c_void_gr_>` 
  *  :ref:`get (Tab: table\<auto(keyT), auto(valT)[]\>; at: keyT|keyT#; blk: block\<(p:valT const[-2]&):void\>) : auto <function-builtin_get_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__block_ls_p_c_valT_const-2_c_void_gr_>` 
  *  :ref:`get (var Tab: table\<auto(keyT), auto(valT)[]\>#; at: keyT|keyT#; blk: block\<(var p:valT[-2]&#):void\>) : auto <function-builtin_get__table_ls_autokeyT,_autovalT_gr__hh__keyTkeyT_hh__block_ls_var_p_c_valT-2_hh__c_void_gr_>` 
  *  :ref:`get (var Tab: table\<auto(keyT), auto(valT)[]\>; at: keyT|keyT#; blk: block\<(var p:valT[-2]&):void\>) : auto <function-builtin_get__table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__block_ls_var_p_c_valT-2_c_void_gr_>` 
  *  :ref:`get (Tab: table\<auto(keyT), void\>; at: keyT|keyT#; blk: block\<(var p:void?):void\>) : auto <function-builtin_get_table_ls_autokeyT,_void_gr__keyTkeyT_hh__block_ls_var_p_c_void_q__c_void_gr_>` 
  *  :ref:`get_value (Tab: table\<auto(keyT), auto(valT)\>; at: keyT|keyT#) : valT <function-builtin_get_value_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh_>` 
  *  :ref:`get_value (var Tab: table\<auto(keyT), smart_ptr\<auto(valT)\>\>; at: keyT|keyT#) : smart_ptr\<valT\> <function-builtin_get_value_table_ls_autokeyT,_smart_ptr_ls_autovalT_gr__gr__keyTkeyT_hh_>` 
  *  :ref:`get_value (var Tab: table\<auto(keyT), auto(valT)\>; at: keyT|keyT#) : valT <function-builtin_get_value_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh_>` 
  *  :ref:`get_value (var Tab: table\<auto(keyT), auto(valT)[]\>; at: keyT|keyT#) : valT[-2] <function-builtin_get_value_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh_>` 
  *  :ref:`erase (var Tab: table\<auto(keyT), auto(valT)\>; at: string#) : bool <function-builtin_erase_table_ls_autokeyT,_autovalT_gr__string_hh_>` 
  *  :ref:`erase (var Tab: table\<auto(keyT), auto(valT)\>; at: keyT|keyT#) : bool <function-builtin_erase_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh_>` 
  *  :ref:`insert (var Tab: table\<auto(keyT), void\>; at: keyT|keyT#) : auto <function-builtin_insert_table_ls_autokeyT,_void_gr__keyTkeyT_hh_>` 
  *  :ref:`insert_clone (var Tab: table\<auto(keyT), auto(valT)\>; at: keyT|keyT#; var val: valT ==const|valT# ==const) : auto <function-builtin_insert_clone_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT__eq__eq_constvalT_hh___eq__eq_const>` 
  *  :ref:`insert_clone (var Tab: table\<auto(keyT), auto(valT)\>; at: keyT|keyT#; val: valT ==const|valT const# ==const) : auto <function-builtin_insert_clone_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT__eq__eq_constvalT_const_hh___eq__eq_const>` 
  *  :ref:`insert_clone (var Tab: table\<auto(keyT), auto(valT)[]\>; at: keyT|keyT#; var val: valT[] ==const|valT[]# ==const) : auto <function-builtin_insert_clone_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT__eq__eq_constvalT_hh___eq__eq_const>` 
  *  :ref:`insert_clone (var Tab: table\<auto(keyT), auto(valT)[]\>; at: keyT|keyT#; val: valT const[] ==const|valT const[]# ==const) : auto <function-builtin_insert_clone_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT_const__eq__eq_constvalT_const_hh___eq__eq_const>` 
  *  :ref:`insert (var Tab: table\<auto(keyT), auto(valT)\>; at: keyT|keyT#; var val: valT ==const|valT# ==const) : auto <function-builtin_insert_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT__eq__eq_constvalT_hh___eq__eq_const>` 
  *  :ref:`insert (var Tab: table\<auto(keyT), auto(valT)\>; at: keyT|keyT#; val: valT ==const|valT const# ==const) : auto <function-builtin_insert_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT__eq__eq_constvalT_const_hh___eq__eq_const>` 
  *  :ref:`insert (var Tab: table\<auto(keyT), auto(valT)[]\>; at: keyT|keyT#; var val: valT[] ==const|valT[]# ==const) : auto <function-builtin_insert_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT__eq__eq_constvalT_hh___eq__eq_const>` 
  *  :ref:`insert (var Tab: table\<auto(keyT), auto(valT)[]\>; at: keyT|keyT#; val: valT const[] ==const|valT const[]# ==const) : auto <function-builtin_insert_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT_const__eq__eq_constvalT_const_hh___eq__eq_const>` 
  *  :ref:`emplace (var Tab: table\<auto(keyT), auto(valT)\>; at: keyT|keyT#; var val: valT&) : auto <function-builtin_emplace_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT>` 
  *  :ref:`emplace (var Tab: table\<auto(keyT), smart_ptr\<auto(valT)\>\>; at: keyT|keyT#; var val: smart_ptr\<valT\>&) : auto <function-builtin_emplace_table_ls_autokeyT,_smart_ptr_ls_autovalT_gr__gr__keyTkeyT_hh__smart_ptr_ls_valT_gr_>` 
  *  :ref:`emplace (var Tab: table\<auto(keyT), auto(valT)[]\>; at: keyT|keyT#; var val: valT[]&) : auto <function-builtin_emplace_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT>` 
  *  :ref:`emplace_new (var tab: table\<auto(kT), smart_ptr\<auto(vT)\>\>; key: kT; var value: smart_ptr\<vT\>) : auto <function-builtin_emplace_new_table_ls_autokT,_smart_ptr_ls_autovT_gr__gr__kT_smart_ptr_ls_vT_gr_>` 
  *  :ref:`emplace (var Tab: table\<auto, auto\>; key: auto; value: auto) : auto <function-builtin_emplace_table_ls_auto,_auto_gr__auto_auto>` 
  *  :ref:`emplace (var a: array\<auto\>; value: auto) : auto <function-builtin_emplace_array_ls_auto_gr__auto>` 
  *  :ref:`emplace_new (var Arr: array\<smart_ptr\<auto(numT)\>\>; var value: smart_ptr\<numT\>) : auto <function-builtin_emplace_new_array_ls_smart_ptr_ls_autonumT_gr__gr__smart_ptr_ls_numT_gr_>` 
  *  :ref:`insert_default (var tab: table\<auto(TT), auto(QQ)\>; key: TT|TT#; var value: QQ ==const|QQ# ==const) <function-builtin_insert_default_table_ls_autoTT,_autoQQ_gr__TTTT_hh__QQ__eq__eq_constQQ_hh___eq__eq_const>` 
  *  :ref:`insert_default (var tab: table\<auto(keyT), auto(valT)\>; key: keyT|keyT#; value: valT ==const|valT const# ==const) <function-builtin_insert_default_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT__eq__eq_constvalT_const_hh___eq__eq_const>` 
  *  :ref:`insert_default (var tab: table\<auto(keyT), auto(valT)\>; key: keyT|keyT#) <function-builtin_insert_default_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh_>` 
  *  :ref:`emplace_default (var tab: table\<auto(keyT), auto(valT)\>; key: keyT|keyT#) <function-builtin_emplace_default_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh_>` 
  *  :ref:`get_with_default (var Tab: table\<auto(keyT), auto(valT)\>; at: keyT|keyT#; blk: block\<(var p:valT&):void\>) <function-builtin_get_with_default__table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__block_ls_var_p_c_valT_c_void_gr_>` 
  *  :ref:`modify (var Tab: table\<auto(keyT), auto(valT)\>; at: keyT|keyT#; blk: block\<(p:valT):valT\>) <function-builtin_modify__table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__block_ls_p_c_valT_c_valT_gr_>` 
  *  :ref:`key_exists (Tab: table\<auto(keyT);auto(valT)\>|table\<auto(keyT);auto(valT)\>#; at: string#) : bool <function-builtin_key_exists_table_ls_autokeyT;autovalT_gr_table_ls_autokeyT;autovalT_gr__hh__string_hh_>` 
  *  :ref:`key_exists (Tab: table\<auto(keyT);auto(valT)\>|table\<auto(keyT);auto(valT)\>#; at: keyT|keyT#) : bool <function-builtin_key_exists_table_ls_autokeyT;autovalT_gr_table_ls_autokeyT;autovalT_gr__hh__keyTkeyT_hh_>` 
  *  :ref:`copy_to_local (a: auto(TT)) : TT <function-builtin_copy_to_local_autoTT>` 
  *  :ref:`move_to_local (var a: auto(TT)&) : TT <function-builtin_move_to_local_autoTT>` 
  *  :ref:`keys (a: table\<auto(keyT);auto(valT)\> ==const|table\<auto(keyT);auto(valT)\> const# ==const) : iterator\<keyT\> <function-builtin_keys_table_ls_autokeyT;autovalT_gr___eq__eq_consttable_ls_autokeyT;autovalT_gr__const_hh___eq__eq_const>` 
  *  :ref:`keys (var a: table\<auto(keyT);auto(valT)\> ==const|table\<auto(keyT);auto(valT)\># ==const) : iterator\<keyT\> <function-builtin_keys_table_ls_autokeyT;autovalT_gr___eq__eq_consttable_ls_autokeyT;autovalT_gr__hh___eq__eq_const>` 
  *  :ref:`values (a: table\<auto(keyT);void\> ==const|table\<auto(keyT);void\> const# ==const) : auto <function-builtin_values_table_ls_autokeyT;void_gr___eq__eq_consttable_ls_autokeyT;void_gr__const_hh___eq__eq_const>` 
  *  :ref:`values (var a: table\<auto(keyT);void\> ==const|table\<auto(keyT);void\># ==const) : auto <function-builtin_values_table_ls_autokeyT;void_gr___eq__eq_consttable_ls_autokeyT;void_gr__hh___eq__eq_const>` 
  *  :ref:`values (a: table\<auto(keyT);auto(valT)\> ==const|table\<auto(keyT);auto(valT)\> const# ==const) : iterator\<valT const&\> <function-builtin_values_table_ls_autokeyT;autovalT_gr___eq__eq_consttable_ls_autokeyT;autovalT_gr__const_hh___eq__eq_const>` 
  *  :ref:`values (var a: table\<auto(keyT);auto(valT)\> ==const|table\<auto(keyT);auto(valT)\># ==const) : iterator\<valT&\> <function-builtin_values_table_ls_autokeyT;autovalT_gr___eq__eq_consttable_ls_autokeyT;autovalT_gr__hh___eq__eq_const>` 
  *  :ref:`values (a: table\<auto(keyT);auto(valT)[]\> ==const|table\<auto(keyT);auto(valT)[]\> const# ==const) : iterator\<valT const[-2]&\> <function-builtin_values_table_ls_autokeyT;autovalT_gr___eq__eq_consttable_ls_autokeyT;autovalT_gr__const_hh___eq__eq_const>` 
  *  :ref:`values (var a: table\<auto(keyT);auto(valT)[]\> ==const|table\<auto(keyT);auto(valT)[]\># ==const) : iterator\<valT[-2]&\> <function-builtin_values_table_ls_autokeyT;autovalT_gr___eq__eq_consttable_ls_autokeyT;autovalT_gr__hh___eq__eq_const>` 
  *  :ref:`lock (Tab: table\<auto(keyT);auto(valT)\>|table\<auto(keyT);auto(valT)\>#; blk: block\<(t:table\<keyT, valT\>#):void\>) : auto <function-builtin_lock_table_ls_autokeyT;autovalT_gr_table_ls_autokeyT;autovalT_gr__hh__block_ls_t_c_table_ls_keyT,_valT_gr__hh__c_void_gr_>` 
  *  :ref:`lock_forever (var Tab: table\<auto(keyT);auto(valT)\>|table\<auto(keyT);auto(valT)\>#) : table\<keyT, valT\># <function-builtin_lock_forever_table_ls_autokeyT;autovalT_gr_table_ls_autokeyT;autovalT_gr__hh_>` 
  *  :ref:`next (var it: iterator\<auto(TT)\>; var value: TT&) : bool <function-builtin_next_iterator_ls_autoTT_gr__TT>` 
  *  :ref:`each (rng: range) : iterator\<int\> <function-builtin_each_range>` 
  *  :ref:`each (str: string) : iterator\<int\> <function-builtin_each_string>` 
  *  :ref:`each (a: auto(TT)[]) : iterator\<TT&\> <function-builtin_each_autoTT>` 
  *  :ref:`each (a: array\<auto(TT)\>) : iterator\<TT&\> <function-builtin_each_array_ls_autoTT_gr_>` 
  *  :ref:`each (a: array\<auto(TT)\>#) : iterator\<TT&#\> <function-builtin_each_array_ls_autoTT_gr__hh_>` 
  *  :ref:`each (lam: lambda\<(var arg:auto(argT)):bool\>) : iterator\<argT\> <function-builtin_each_lambda_ls_var_arg_c_autoargT_c_bool_gr_>` 
  *  :ref:`each_ref (lam: lambda\<(var arg:auto(argT)?):bool\>) : iterator\<argT&\> <function-builtin_each_ref_lambda_ls_var_arg_c_autoargT_q__c_bool_gr_>` 
  *  :ref:`each_enum (tt: auto(TT)) : iterator\<TT\> <function-builtin_each_enum_autoTT>` 
  *  :ref:`nothing (var it: iterator\<auto(TT)\>) : iterator\<TT\> <function-builtin_nothing_iterator_ls_autoTT_gr_>` 
  *  :ref:`to_array (var it: iterator\<auto(TT)\>) : array\<TT\> <function-builtin_to_array_iterator_ls_autoTT_gr_>` 
  *  :ref:`to_array (a: auto(TT)[]) : array\<TT\> <function-builtin_to_array_autoTT>` 
  *  :ref:`to_array_move (var a: auto(TT)[]) : array\<TT\> <function-builtin_to_array_move_autoTT>` 
  *  :ref:`to_array_move (a: auto(TT) ==const) : array\<TT\> <function-builtin_to_array_move_autoTT__eq__eq_const>` 
  *  :ref:`to_array_move (var a: auto(TT) ==const) : array\<TT\> <function-builtin_to_array_move__autoTT__eq__eq_const>` 
  *  :ref:`to_table (a: tuple\<auto(keyT);auto(valT)\>[]) : table\<keyT, valT\> <function-builtin_to_table_tuple_ls_autokeyT;autovalT_gr_>` 
  *  :ref:`to_table (a: auto(keyT)[]) : table\<keyT, void\> <function-builtin_to_table_autokeyT>` 
  *  :ref:`to_table_move (a: auto(keyT)[]) : table\<keyT, void\> <function-builtin_to_table_move_autokeyT>` 
  *  :ref:`to_table_move (a: array\<auto(keyT)\>) : table\<keyT, void\> <function-builtin_to_table_move_array_ls_autokeyT_gr_>` 
  *  :ref:`to_table_move (a: auto(keyT)) : table\<keyT, void\> <function-builtin_to_table_move_autokeyT>` 
  *  :ref:`to_table_move (var a: tuple\<auto(keyT);auto(valT)\>) : table\<keyT, valT\> <function-builtin_to_table_move_tuple_ls_autokeyT;autovalT_gr_>` 
  *  :ref:`to_table_move (var a: tuple\<auto(keyT);auto(valT)\>[]) : table\<keyT, valT\> <function-builtin_to_table_move_tuple_ls_autokeyT;autovalT_gr_>` 
  *  :ref:`to_table_move (var a: array\<tuple\<auto(keyT);auto(valT)\>\>) : table\<keyT, valT\> <function-builtin_to_table_move_array_ls_tuple_ls_autokeyT;autovalT_gr__gr_>` 
  *  :ref:`sort (var a: auto(TT)[]|auto(TT)[]#) : auto <function-builtin_sort_autoTTautoTT_hh_>` 
  *  :ref:`sort (var a: array\<auto(TT)\>|array\<auto(TT)\>#) : auto <function-builtin_sort_array_ls_autoTT_gr_array_ls_autoTT_gr__hh_>` 
  *  :ref:`sort (var a: auto(TT)[]|auto(TT)[]#; cmp: block\<(x:TT;y:TT):bool\>) : auto <function-builtin_sort_autoTTautoTT_hh__block_ls_x_c_TT;y_c_TT_c_bool_gr_>` 
  *  :ref:`sort (var a: array\<auto(TT)\>|array\<auto(TT)\>#; cmp: block\<(x:TT;y:TT):bool\>) : auto <function-builtin_sort_array_ls_autoTT_gr_array_ls_autoTT_gr__hh__block_ls_x_c_TT;y_c_TT_c_bool_gr_>` 
  *  :ref:`lock (var a: array\<auto(TT)\> ==const|array\<auto(TT)\># ==const; blk: block\<(var x:array\<TT\>#):auto\>) : auto <function-builtin_lock_array_ls_autoTT_gr___eq__eq_constarray_ls_autoTT_gr__hh___eq__eq_const_block_ls_var_x_c_array_ls_TT_gr__hh__c_auto_gr_>` 
  *  :ref:`lock (a: array\<auto(TT)\> ==const|array\<auto(TT)\> const# ==const; blk: block\<(x:array\<TT\>#):auto\>) : auto <function-builtin_lock_array_ls_autoTT_gr___eq__eq_constarray_ls_autoTT_gr__const_hh___eq__eq_const_block_ls_x_c_array_ls_TT_gr__hh__c_auto_gr_>` 
  *  :ref:`find_index (arr: array\<auto(TT)\>|array\<auto(TT)\>#; key: TT) : auto <function-builtin_find_index_array_ls_autoTT_gr_array_ls_autoTT_gr__hh__TT>` 
  *  :ref:`find_index (arr: auto(TT)[]|auto(TT)[]#; key: TT) : auto <function-builtin_find_index_autoTTautoTT_hh__TT>` 
  *  :ref:`find_index (var arr: iterator\<auto(TT)\>; key: TT) : auto <function-builtin_find_index_iterator_ls_autoTT_gr__TT>` 
  *  :ref:`find_index_if (arr: array\<auto(TT)\>|array\<auto(TT)\>#; blk: block\<(key:TT):bool\>) : auto <function-builtin_find_index_if_array_ls_autoTT_gr_array_ls_autoTT_gr__hh__block_ls_key_c_TT_c_bool_gr_>` 
  *  :ref:`find_index_if (arr: auto(TT)[]|auto(TT)[]#; blk: block\<(key:TT):bool\>) : auto <function-builtin_find_index_if_autoTTautoTT_hh__block_ls_key_c_TT_c_bool_gr_>` 
  *  :ref:`find_index_if (var arr: iterator\<auto(TT)\>; blk: block\<(key:TT):bool\>) : auto <function-builtin_find_index_if_iterator_ls_autoTT_gr__block_ls_key_c_TT_c_bool_gr_>` 
  *  :ref:`has_value (a: auto; key: auto) : auto <function-builtin_has_value_auto_auto>` 
  *  :ref:`has_value (var a: iterator\<auto\>; key: auto) : auto <function-builtin_has_value_iterator_ls_auto_gr__auto>` 
  *  :ref:`subarray (a: auto(TT)[]; r: range) : auto <function-builtin_subarray_autoTT_range>` 
  *  :ref:`subarray (a: auto(TT)[]; r: urange) : auto <function-builtin_subarray_autoTT_urange>` 
  *  :ref:`subarray (a: array\<auto(TT)\>; r: range) : auto <function-builtin_subarray_array_ls_autoTT_gr__range>` 
  *  :ref:`subarray (var a: array\<auto(TT)\>; r: range) : auto <function-builtin_subarray__array_ls_autoTT_gr__range>` 
  *  :ref:`subarray (a: array\<auto(TT)\>; r: urange) : auto <function-builtin_subarray_array_ls_autoTT_gr__urange>` 
  *  :ref:`move_to_ref (var a: auto&; var b: auto) : auto <function-builtin_move_to_ref_auto_auto>` 
  *  :ref:`clear (var t: table\<auto(KT), auto(VT)\>) : auto <function-builtin_clear_table_ls_autoKT,_autoVT_gr_>` 

.. _function-builtin_clear_array_ls_anything_gr_:

.. das:function:: clear(array: array<anything>)

 clear will clear whole table or array `arg`. The size of `arg` after clear is 0.

:Arguments: * **array** : array implicit

.. _function-builtin_length_array_ls_anything_gr_:

.. das:function:: length(array: array<anything>) : int

 length will return current size of table or array `arg`.

:Arguments: * **array** : array implicit

.. _function-builtin_capacity_array_ls_anything_gr_:

.. das:function:: capacity(array: array<anything>) : int

 capacity will return current capacity of table or array `arg`. Capacity is the count of elements, allocating (or pushing) until that size won't cause reallocating dynamic heap.

:Arguments: * **array** : array implicit

.. _function-builtin_empty_iterator_implicit:

.. das:function:: empty(iterator: iterator implicit) : bool

 returns true if iterator is empty, i.e. would not produce any more values or uninitialized

:Arguments: * **iterator** : iterator implicit

.. _function-builtin_length_table_ls_anything,_anything_gr_:

.. das:function:: length(table: table<anything, anything>) : int

 length will return current size of table or array `arg`.

:Arguments: * **table** : table implicit

.. _function-builtin_capacity_table_ls_anything,_anything_gr_:

.. das:function:: capacity(table: table<anything, anything>) : int

 capacity will return current capacity of table or array `arg`. Capacity is the count of elements, allocating (or pushing) until that size won't cause reallocating dynamic heap.

:Arguments: * **table** : table implicit

.. _function-builtin_empty_string_implicit:

.. das:function:: empty(str: string implicit) : bool

 returns true if iterator is empty, i.e. would not produce any more values or uninitialized

:Arguments: * **str** : string implicit

.. _function-builtin_empty_das_string_implicit:

.. das:function:: empty(str: das_string implicit) : bool

 returns true if iterator is empty, i.e. would not produce any more values or uninitialized

:Arguments: * **str** :  :ref:`das_string <handle-builtin-das_string>`  implicit

.. _function-builtin_resize_array_ls_autonumT_gr__int:

.. das:function:: resize(Arr: array<auto(numT)>; newSize: int) : auto

 Resize will resize `array_arg` array to a new size of `new_size`. If new_size is bigger than current, new elements will be zeroed.

:Arguments: * **Arr** : array<auto(numT)>

            * **newSize** : int

.. _function-builtin_resize_and_init_array_ls_autonumT_gr__int:

.. das:function:: resize_and_init(Arr: array<auto(numT)>; newSize: int) : auto

 Resizes array and initializes new elements.

:Arguments: * **Arr** : array<auto(numT)>

            * **newSize** : int

.. _function-builtin_resize_and_init_array_ls_autonumT_gr__int_numT:

.. das:function:: resize_and_init(Arr: array<auto(numT)>; newSize: int; initValue: numT) : auto

 Resizes array and initializes new elements.

:Arguments: * **Arr** : array<auto(numT)>

            * **newSize** : int

            * **initValue** : numT

.. _function-builtin_resize_no_init_array_ls_autonumT_gr__int:

.. das:function:: resize_no_init(Arr: array<auto(numT)>; newSize: int) : auto

 Resize will resize `array_arg` array to a new size of `new_size`. If new_size is bigger than current, new elements will be left uninitialized.

:Arguments: * **Arr** : array<auto(numT)>

            * **newSize** : int

.. _function-builtin_reserve_array_ls_autonumT_gr__int:

.. das:function:: reserve(Arr: array<auto(numT)>; newSize: int) : auto

 makes sure array has sufficient amount of memory to hold specified number of elements. reserving arrays will speed up pushing to it

:Arguments: * **Arr** : array<auto(numT)>

            * **newSize** : int

.. _function-builtin_pop_array_ls_autonumT_gr_:

.. das:function:: pop(Arr: array<auto(numT)>) : auto

 removes last element of the array

:Arguments: * **Arr** : array<auto(numT)>

.. _function-builtin_push_array_ls_autonumT_gr__numT__eq__eq_const_int:

.. das:function:: push(Arr: array<auto(numT)>; value: numT ==const; at: int) : auto

 push will push to dynamic array `array_arg` the content of `value`. `value` has to be of the same type (or const reference to same type) as array values. if `at` is provided `value` will be pushed at index `at`, otherwise to the end of array. The `content` of value will be copied (assigned) to it.

:Arguments: * **Arr** : array<auto(numT)>

            * **value** : numT!

            * **at** : int

.. _function-builtin_push_array_ls_autonumT_gr___numT__eq__eq_const_int:

.. das:function:: push(Arr: array<auto(numT)>; value: numT ==const; at: int) : auto

 push will push to dynamic array `array_arg` the content of `value`. `value` has to be of the same type (or const reference to same type) as array values. if `at` is provided `value` will be pushed at index `at`, otherwise to the end of array. The `content` of value will be copied (assigned) to it.

:Arguments: * **Arr** : array<auto(numT)>

            * **value** : numT!

            * **at** : int

.. _function-builtin_push_array_ls_autonumT_gr__numT__eq__eq_const:

.. das:function:: push(Arr: array<auto(numT)>; value: numT ==const) : auto

 push will push to dynamic array `array_arg` the content of `value`. `value` has to be of the same type (or const reference to same type) as array values. if `at` is provided `value` will be pushed at index `at`, otherwise to the end of array. The `content` of value will be copied (assigned) to it.

:Arguments: * **Arr** : array<auto(numT)>

            * **value** : numT!

.. _function-builtin_push_array_ls_autonumT_gr___numT__eq__eq_const:

.. das:function:: push(Arr: array<auto(numT)>; value: numT ==const) : auto

 push will push to dynamic array `array_arg` the content of `value`. `value` has to be of the same type (or const reference to same type) as array values. if `at` is provided `value` will be pushed at index `at`, otherwise to the end of array. The `content` of value will be copied (assigned) to it.

:Arguments: * **Arr** : array<auto(numT)>

            * **value** : numT!

.. _function-builtin_push_array_ls_autonumT_gr__array_ls_numT_gr_:

.. das:function:: push(Arr: array<auto(numT)>; varr: array<numT>) : auto

 push will push to dynamic array `array_arg` the content of `value`. `value` has to be of the same type (or const reference to same type) as array values. if `at` is provided `value` will be pushed at index `at`, otherwise to the end of array. The `content` of value will be copied (assigned) to it.

:Arguments: * **Arr** : array<auto(numT)>

            * **varr** : array<numT>!

.. _function-builtin_push_array_ls_autonumT_gr___array_ls_numT_gr_:

.. das:function:: push(Arr: array<auto(numT)>; varr: array<numT>) : auto

 push will push to dynamic array `array_arg` the content of `value`. `value` has to be of the same type (or const reference to same type) as array values. if `at` is provided `value` will be pushed at index `at`, otherwise to the end of array. The `content` of value will be copied (assigned) to it.

:Arguments: * **Arr** : array<auto(numT)>

            * **varr** : array<numT>!

.. _function-builtin_push_array_ls_autonumT_gr__numT:

.. das:function:: push(Arr: array<auto(numT)>; varr: numT[]) : auto

 push will push to dynamic array `array_arg` the content of `value`. `value` has to be of the same type (or const reference to same type) as array values. if `at` is provided `value` will be pushed at index `at`, otherwise to the end of array. The `content` of value will be copied (assigned) to it.

:Arguments: * **Arr** : array<auto(numT)>

            * **varr** : numT[-1]

.. _function-builtin_push_array_ls_autonumT_gr__numT_const__eq__eq_const:

.. das:function:: push(Arr: array<auto(numT)[]>; varr: numT const[] ==const) : auto

 push will push to dynamic array `array_arg` the content of `value`. `value` has to be of the same type (or const reference to same type) as array values. if `at` is provided `value` will be pushed at index `at`, otherwise to the end of array. The `content` of value will be copied (assigned) to it.

:Arguments: * **Arr** : array<auto(numT)[-1]>

            * **varr** : numT[-1]!

.. _function-builtin_push_array_ls_autonumT_gr___numT__eq__eq_const:

.. das:function:: push(Arr: array<auto(numT)[]>; varr: numT[] ==const) : auto

 push will push to dynamic array `array_arg` the content of `value`. `value` has to be of the same type (or const reference to same type) as array values. if `at` is provided `value` will be pushed at index `at`, otherwise to the end of array. The `content` of value will be copied (assigned) to it.

:Arguments: * **Arr** : array<auto(numT)[-1]>

            * **varr** : numT[-1]!

.. _function-builtin_emplace_array_ls_autonumT_gr__numT_int:

.. das:function:: emplace(Arr: array<auto(numT)>; value: numT&; at: int) : auto

 emplace will push to dynamic array `array_arg` the content of `value`. `value` has to be of the same type (or const reference to same type) as array values. if `at` is provided `value` will be pushed at index `at`, otherwise to the end of array. The `content` of value will be moved (<-) to it.

:Arguments: * **Arr** : array<auto(numT)>

            * **value** : numT&

            * **at** : int

.. _function-builtin_emplace_array_ls_autonumT_gr__numT:

.. das:function:: emplace(Arr: array<auto(numT)>; value: numT&) : auto

 emplace will push to dynamic array `array_arg` the content of `value`. `value` has to be of the same type (or const reference to same type) as array values. if `at` is provided `value` will be pushed at index `at`, otherwise to the end of array. The `content` of value will be moved (<-) to it.

:Arguments: * **Arr** : array<auto(numT)>

            * **value** : numT&

.. _function-builtin_emplace_array_ls_autonumT_gr__numT:

.. das:function:: emplace(Arr: array<auto(numT)>; value: numT[]) : auto

 emplace will push to dynamic array `array_arg` the content of `value`. `value` has to be of the same type (or const reference to same type) as array values. if `at` is provided `value` will be pushed at index `at`, otherwise to the end of array. The `content` of value will be moved (<-) to it.

:Arguments: * **Arr** : array<auto(numT)>

            * **value** : numT[-1]

.. _function-builtin_emplace_array_ls_autonumT_gr__numT:

.. das:function:: emplace(Arr: array<auto(numT)[]>; value: numT[]) : auto

 emplace will push to dynamic array `array_arg` the content of `value`. `value` has to be of the same type (or const reference to same type) as array values. if `at` is provided `value` will be pushed at index `at`, otherwise to the end of array. The `content` of value will be moved (<-) to it.

:Arguments: * **Arr** : array<auto(numT)[-1]>

            * **value** : numT[-1]

.. _function-builtin_push_clone_array_ls_autonumT_gr__numT__eq__eq_constnumT_const_hh___eq__eq_const_int:

.. das:function:: push_clone(Arr: array<auto(numT)>; value: numT ==const|numT const# ==const; at: int) : auto

 similar to `push`, only values would be cloned instead of copied

:Arguments: * **Arr** : array<auto(numT)>

            * **value** : option<numT!|numT#!>

            * **at** : int

.. _function-builtin_push_clone_array_ls_autonumT_gr__numT__eq__eq_constnumT_hh___eq__eq_const_int:

.. das:function:: push_clone(Arr: array<auto(numT)>; value: numT ==const|numT# ==const; at: int) : auto

 similar to `push`, only values would be cloned instead of copied

:Arguments: * **Arr** : array<auto(numT)>

            * **value** : option<numT!|numT#!>

            * **at** : int

.. _function-builtin_push_clone_array_ls_autonumT_gr__numT__eq__eq_constnumT_const_hh___eq__eq_const:

.. das:function:: push_clone(Arr: array<auto(numT)>; value: numT ==const|numT const# ==const) : auto

 similar to `push`, only values would be cloned instead of copied

:Arguments: * **Arr** : array<auto(numT)>

            * **value** : option<numT!|numT#!>

.. _function-builtin_push_clone_array_ls_autonumT_gr__numT__eq__eq_constnumT_hh___eq__eq_const:

.. das:function:: push_clone(Arr: array<auto(numT)>; value: numT ==const|numT# ==const) : auto

 similar to `push`, only values would be cloned instead of copied

:Arguments: * **Arr** : array<auto(numT)>

            * **value** : option<numT!|numT#!>

.. _function-builtin_push_clone_array_ls_autonumT_gr__numT_const__eq__eq_const:

.. das:function:: push_clone(Arr: array<auto(numT)>; varr: numT const[] ==const) : auto

 similar to `push`, only values would be cloned instead of copied

:Arguments: * **Arr** : array<auto(numT)>

            * **varr** : numT[-1]!

.. _function-builtin_push_clone_array_ls_autonumT_gr___numT__eq__eq_const:

.. das:function:: push_clone(Arr: array<auto(numT)>; varr: numT[] ==const) : auto

 similar to `push`, only values would be cloned instead of copied

:Arguments: * **Arr** : array<auto(numT)>

            * **varr** : numT[-1]!

.. _function-builtin_push_clone_array_ls_autonumT_gr__numT_const__eq__eq_const:

.. das:function:: push_clone(Arr: array<auto(numT)[]>; varr: numT const[] ==const) : auto

 similar to `push`, only values would be cloned instead of copied

:Arguments: * **Arr** : array<auto(numT)[-1]>

            * **varr** : numT[-1]!

.. _function-builtin_push_clone_array_ls_autonumT_gr__numT:

.. das:function:: push_clone(Arr: array<auto(numT)[]>; varr: numT[]) : auto

 similar to `push`, only values would be cloned instead of copied

:Arguments: * **Arr** : array<auto(numT)[-1]>

            * **varr** : numT[-1]

.. _function-builtin_push_clone_autoCT_autoTTautoTT_hh_:

.. das:function:: push_clone(A: auto(CT); b: auto(TT)|auto(TT)#) : auto

 similar to `push`, only values would be cloned instead of copied

:Arguments: * **A** : auto(CT)

            * **b** : option<auto(TT)|auto(TT)#>

.. _function-builtin_back__array_ls_autoTT_gr_:

.. das:function:: back(a: array<auto(TT)>) : TT&

 returns last element of the array

:Arguments: * **a** : array<auto(TT)>!

.. _function-builtin_back__array_ls_autoTT_gr__hh_:

.. das:function:: back(a: array<auto(TT)>#) : TT&#

 returns last element of the array

:Arguments: * **a** : array<auto(TT)>#!

.. _function-builtin_back_array_ls_autoTT_gr_:

.. das:function:: back(a: array<auto(TT)>) : TT

 returns last element of the array

:Arguments: * **a** : array<auto(TT)>!

.. _function-builtin_back_array_ls_autoTT_gr__hh_:

.. das:function:: back(a: array<auto(TT)>#) : TT const&#

 returns last element of the array

:Arguments: * **a** : array<auto(TT)>#!

.. _function-builtin_back__autoTT__eq__eq_const:

.. das:function:: back(arr: auto(TT) ==const) : auto&

 returns last element of the array

:Arguments: * **arr** : auto(TT)!

.. _function-builtin_back_autoTT__eq__eq_const:

.. das:function:: back(arr: auto(TT) ==const) : auto

 returns last element of the array

:Arguments: * **arr** : auto(TT)!

.. _function-builtin_erase_array_ls_autonumT_gr__int:

.. das:function:: erase(Arr: array<auto(numT)>; at: int) : auto

 erase will erase `at` index element in `arg` array.

:Arguments: * **Arr** : array<auto(numT)>

            * **at** : int

.. _function-builtin_erase_array_ls_autonumT_gr__int_int:

.. das:function:: erase(Arr: array<auto(numT)>; at: int; count: int) : auto

 erase will erase `at` index element in `arg` array.

:Arguments: * **Arr** : array<auto(numT)>

            * **at** : int

            * **count** : int

.. _function-builtin_erase_if_array_ls_autoTT_gr__block_ls_key_c_TT_const_c_bool_gr_block_ls_var_key_c_TT_c_bool_gr_:

.. das:function:: erase_if(arr: array<auto(TT)>; blk: block<(key:TT const):bool>|block<(var key:TT&):bool>) : auto

 Erases element from the array if it satisfies the condition.

:Arguments: * **arr** : array<auto(TT)>

            * **blk** : option<block<(key:TT):bool>|block<(key:TT&):bool>>

.. _function-builtin_remove_value_array_ls_autoTT_gr_array_ls_autoTT_gr__hh__TT:

.. das:function:: remove_value(arr: array<auto(TT)>|array<auto(TT)>#; key: TT) : bool

 removes first occurance of the key from the array.

:Arguments: * **arr** : option<array<auto(TT)>|array<auto(TT)>#>

            * **key** : TT

.. _function-builtin_length_autoauto_hh_:

.. das:function:: length(a: auto|auto#) : int

 length will return current size of table or array `arg`.

:Arguments: * **a** : option<auto|auto#>

.. _function-builtin_empty_array_ls_auto_gr_array_ls_auto_gr__hh_:

.. das:function:: empty(a: array<auto>|array<auto>#) : bool

 returns true if iterator is empty, i.e. would not produce any more values or uninitialized

:Arguments: * **a** : option<array<auto>|array<auto>#>

.. _function-builtin_empty_table_ls_auto;auto_gr_table_ls_auto;auto_gr__hh_:

.. das:function:: empty(a: table<auto;auto>|table<auto;auto>#) : bool

 returns true if iterator is empty, i.e. would not produce any more values or uninitialized

:Arguments: * **a** : option<table<auto;auto>|table<auto;auto>#>

.. _function-builtin_reserve_table_ls_autokeyT,_auto_gr__int:

.. das:function:: reserve(Tab: table<auto(keyT), auto>; newSize: int) : auto

 makes sure array has sufficient amount of memory to hold specified number of elements. reserving arrays will speed up pushing to it

:Arguments: * **Tab** : table<auto(keyT);auto>

            * **newSize** : int

.. _function-builtin_get_table_ls_autokeyT,_autovalT_gr__hh__keyTkeyT_hh__block_ls_p_c_valT_const_hh__c_void_gr_:

.. das:function:: get(Tab: table<auto(keyT), auto(valT)>#; at: keyT|keyT#; blk: block<(p:valT const&#):void>) : auto

 will execute `block_arg` with argument reference-to-value in `table_arg` referencing value indexed by `key`. Will return false if `key` doesn't exist in `table_arg`, otherwise true.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)>#!

            * **at** : option<keyT|keyT#>

            * **blk** : block<(p:valT&#):void>

.. _function-builtin_get_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__block_ls_p_c_valT_c_void_gr_:

.. das:function:: get(Tab: table<auto(keyT), auto(valT)>; at: keyT|keyT#; blk: block<(p:valT):void>) : auto

 will execute `block_arg` with argument reference-to-value in `table_arg` referencing value indexed by `key`. Will return false if `key` doesn't exist in `table_arg`, otherwise true.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)>!

            * **at** : option<keyT|keyT#>

            * **blk** : block<(p:valT&):void>

.. _function-builtin_get__table_ls_autokeyT,_autovalT_gr__hh__keyTkeyT_hh__block_ls_var_p_c_valT_hh__c_void_gr_:

.. das:function:: get(Tab: table<auto(keyT), auto(valT)>#; at: keyT|keyT#; blk: block<(var p:valT&#):void>) : auto

 will execute `block_arg` with argument reference-to-value in `table_arg` referencing value indexed by `key`. Will return false if `key` doesn't exist in `table_arg`, otherwise true.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)>#!

            * **at** : option<keyT|keyT#>

            * **blk** : block<(p:valT&#):void>

.. _function-builtin_get__table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__block_ls_var_p_c_valT_c_void_gr_:

.. das:function:: get(Tab: table<auto(keyT), auto(valT)>; at: keyT|keyT#; blk: block<(var p:valT&):void>) : auto

 will execute `block_arg` with argument reference-to-value in `table_arg` referencing value indexed by `key`. Will return false if `key` doesn't exist in `table_arg`, otherwise true.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)>!

            * **at** : option<keyT|keyT#>

            * **blk** : block<(p:valT&):void>

.. _function-builtin_get_table_ls_autokeyT,_autovalT_gr__hh__keyTkeyT_hh__block_ls_p_c_valT_const-2_hh__c_void_gr_:

.. das:function:: get(Tab: table<auto(keyT), auto(valT)[]>#; at: keyT|keyT#; blk: block<(p:valT const[-2]&#):void>) : auto

 will execute `block_arg` with argument reference-to-value in `table_arg` referencing value indexed by `key`. Will return false if `key` doesn't exist in `table_arg`, otherwise true.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)[-1]>#!

            * **at** : option<keyT|keyT#>

            * **blk** : block<(p:valT[-2]&#):void>

.. _function-builtin_get_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__block_ls_p_c_valT_const-2_c_void_gr_:

.. das:function:: get(Tab: table<auto(keyT), auto(valT)[]>; at: keyT|keyT#; blk: block<(p:valT const[-2]&):void>) : auto

 will execute `block_arg` with argument reference-to-value in `table_arg` referencing value indexed by `key`. Will return false if `key` doesn't exist in `table_arg`, otherwise true.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)[-1]>!

            * **at** : option<keyT|keyT#>

            * **blk** : block<(p:valT[-2]&):void>

.. _function-builtin_get__table_ls_autokeyT,_autovalT_gr__hh__keyTkeyT_hh__block_ls_var_p_c_valT-2_hh__c_void_gr_:

.. das:function:: get(Tab: table<auto(keyT), auto(valT)[]>#; at: keyT|keyT#; blk: block<(var p:valT[-2]&#):void>) : auto

 will execute `block_arg` with argument reference-to-value in `table_arg` referencing value indexed by `key`. Will return false if `key` doesn't exist in `table_arg`, otherwise true.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)[-1]>#!

            * **at** : option<keyT|keyT#>

            * **blk** : block<(p:valT[-2]&#):void>

.. _function-builtin_get__table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__block_ls_var_p_c_valT-2_c_void_gr_:

.. das:function:: get(Tab: table<auto(keyT), auto(valT)[]>; at: keyT|keyT#; blk: block<(var p:valT[-2]&):void>) : auto

 will execute `block_arg` with argument reference-to-value in `table_arg` referencing value indexed by `key`. Will return false if `key` doesn't exist in `table_arg`, otherwise true.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)[-1]>!

            * **at** : option<keyT|keyT#>

            * **blk** : block<(p:valT[-2]&):void>

.. _function-builtin_get_table_ls_autokeyT,_void_gr__keyTkeyT_hh__block_ls_var_p_c_void_q__c_void_gr_:

.. das:function:: get(Tab: table<auto(keyT), void>; at: keyT|keyT#; blk: block<(var p:void?):void>) : auto

 will execute `block_arg` with argument reference-to-value in `table_arg` referencing value indexed by `key`. Will return false if `key` doesn't exist in `table_arg`, otherwise true.

:Arguments: * **Tab** : table<auto(keyT);void>

            * **at** : option<keyT|keyT#>

            * **blk** : block<(p:void?):void>

.. _function-builtin_get_value_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh_:

.. das:function:: get_value(Tab: table<auto(keyT), auto(valT)>; at: keyT|keyT#) : valT

 gets the value from the table.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)>!

            * **at** : option<keyT|keyT#>

.. _function-builtin_get_value_table_ls_autokeyT,_smart_ptr_ls_autovalT_gr__gr__keyTkeyT_hh_:

.. das:function:: get_value(Tab: table<auto(keyT), smart_ptr<auto(valT)>>; at: keyT|keyT#) : smart_ptr<valT>

 gets the value from the table.

:Arguments: * **Tab** : table<auto(keyT);smart_ptr<auto(valT)>>

            * **at** : option<keyT|keyT#>

.. _function-builtin_get_value_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh_:

.. das:function:: get_value(Tab: table<auto(keyT), auto(valT)>; at: keyT|keyT#) : valT

 gets the value from the table.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)>

            * **at** : option<keyT|keyT#>

.. _function-builtin_get_value_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh_:

.. das:function:: get_value(Tab: table<auto(keyT), auto(valT)[]>; at: keyT|keyT#) : valT[-2]

 gets the value from the table.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)[-1]>

            * **at** : option<keyT|keyT#>

.. _function-builtin_erase_table_ls_autokeyT,_autovalT_gr__string_hh_:

.. das:function:: erase(Tab: table<auto(keyT), auto(valT)>; at: string#) : bool

 erase will erase `at` index element in `arg` array.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)>

            * **at** : string#

.. _function-builtin_erase_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh_:

.. das:function:: erase(Tab: table<auto(keyT), auto(valT)>; at: keyT|keyT#) : bool

 erase will erase `at` index element in `arg` array.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)>

            * **at** : option<keyT|keyT#>

.. _function-builtin_insert_table_ls_autokeyT,_void_gr__keyTkeyT_hh_:

.. das:function:: insert(Tab: table<auto(keyT), void>; at: keyT|keyT#) : auto

 inserts key into the set (table with no values) `Tab`

:Arguments: * **Tab** : table<auto(keyT);void>

            * **at** : option<keyT|keyT#>

.. _function-builtin_insert_clone_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT__eq__eq_constvalT_hh___eq__eq_const:

.. das:function:: insert_clone(Tab: table<auto(keyT), auto(valT)>; at: keyT|keyT#; val: valT ==const|valT# ==const) : auto

 inserts cloned key into the table

:Arguments: * **Tab** : table<auto(keyT);auto(valT)>

            * **at** : option<keyT|keyT#>

            * **val** : option<valT!|valT#!>

.. _function-builtin_insert_clone_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT__eq__eq_constvalT_const_hh___eq__eq_const:

.. das:function:: insert_clone(Tab: table<auto(keyT), auto(valT)>; at: keyT|keyT#; val: valT ==const|valT const# ==const) : auto

 inserts cloned key into the table

:Arguments: * **Tab** : table<auto(keyT);auto(valT)>

            * **at** : option<keyT|keyT#>

            * **val** : option<valT!|valT#!>

.. _function-builtin_insert_clone_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT__eq__eq_constvalT_hh___eq__eq_const:

.. das:function:: insert_clone(Tab: table<auto(keyT), auto(valT)[]>; at: keyT|keyT#; val: valT[] ==const|valT[]# ==const) : auto

 inserts cloned key into the table

:Arguments: * **Tab** : table<auto(keyT);auto(valT)[-1]>

            * **at** : option<keyT|keyT#>

            * **val** : option<valT[-1]!|valT[-1]#!>

.. _function-builtin_insert_clone_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT_const__eq__eq_constvalT_const_hh___eq__eq_const:

.. das:function:: insert_clone(Tab: table<auto(keyT), auto(valT)[]>; at: keyT|keyT#; val: valT const[] ==const|valT const[]# ==const) : auto

 inserts cloned key into the table

:Arguments: * **Tab** : table<auto(keyT);auto(valT)[-1]>

            * **at** : option<keyT|keyT#>

            * **val** : option<valT[-1]!|valT[-1]#!>

.. _function-builtin_insert_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT__eq__eq_constvalT_hh___eq__eq_const:

.. das:function:: insert(Tab: table<auto(keyT), auto(valT)>; at: keyT|keyT#; val: valT ==const|valT# ==const) : auto

 inserts key into the set (table with no values) `Tab`

:Arguments: * **Tab** : table<auto(keyT);auto(valT)>

            * **at** : option<keyT|keyT#>

            * **val** : option<valT!|valT#!>

.. _function-builtin_insert_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT__eq__eq_constvalT_const_hh___eq__eq_const:

.. das:function:: insert(Tab: table<auto(keyT), auto(valT)>; at: keyT|keyT#; val: valT ==const|valT const# ==const) : auto

 inserts key into the set (table with no values) `Tab`

:Arguments: * **Tab** : table<auto(keyT);auto(valT)>

            * **at** : option<keyT|keyT#>

            * **val** : option<valT!|valT#!>

.. _function-builtin_insert_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT__eq__eq_constvalT_hh___eq__eq_const:

.. das:function:: insert(Tab: table<auto(keyT), auto(valT)[]>; at: keyT|keyT#; val: valT[] ==const|valT[]# ==const) : auto

 inserts key into the set (table with no values) `Tab`

:Arguments: * **Tab** : table<auto(keyT);auto(valT)[-1]>

            * **at** : option<keyT|keyT#>

            * **val** : option<valT[-1]!|valT[-1]#!>

.. _function-builtin_insert_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT_const__eq__eq_constvalT_const_hh___eq__eq_const:

.. das:function:: insert(Tab: table<auto(keyT), auto(valT)[]>; at: keyT|keyT#; val: valT const[] ==const|valT const[]# ==const) : auto

 inserts key into the set (table with no values) `Tab`

:Arguments: * **Tab** : table<auto(keyT);auto(valT)[-1]>

            * **at** : option<keyT|keyT#>

            * **val** : option<valT[-1]!|valT[-1]#!>

.. _function-builtin_emplace_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT:

.. das:function:: emplace(Tab: table<auto(keyT), auto(valT)>; at: keyT|keyT#; val: valT&) : auto

 emplace will push to dynamic array `array_arg` the content of `value`. `value` has to be of the same type (or const reference to same type) as array values. if `at` is provided `value` will be pushed at index `at`, otherwise to the end of array. The `content` of value will be moved (<-) to it.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)>

            * **at** : option<keyT|keyT#>

            * **val** : valT&

.. _function-builtin_emplace_table_ls_autokeyT,_smart_ptr_ls_autovalT_gr__gr__keyTkeyT_hh__smart_ptr_ls_valT_gr_:

.. das:function:: emplace(Tab: table<auto(keyT), smart_ptr<auto(valT)>>; at: keyT|keyT#; val: smart_ptr<valT>&) : auto

 emplace will push to dynamic array `array_arg` the content of `value`. `value` has to be of the same type (or const reference to same type) as array values. if `at` is provided `value` will be pushed at index `at`, otherwise to the end of array. The `content` of value will be moved (<-) to it.

:Arguments: * **Tab** : table<auto(keyT);smart_ptr<auto(valT)>>

            * **at** : option<keyT|keyT#>

            * **val** : smart_ptr<valT>&

.. _function-builtin_emplace_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT:

.. das:function:: emplace(Tab: table<auto(keyT), auto(valT)[]>; at: keyT|keyT#; val: valT[]&) : auto

 emplace will push to dynamic array `array_arg` the content of `value`. `value` has to be of the same type (or const reference to same type) as array values. if `at` is provided `value` will be pushed at index `at`, otherwise to the end of array. The `content` of value will be moved (<-) to it.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)[-1]>

            * **at** : option<keyT|keyT#>

            * **val** : valT[-1]&

.. _function-builtin_emplace_new_table_ls_autokT,_smart_ptr_ls_autovT_gr__gr__kT_smart_ptr_ls_vT_gr_:

.. das:function:: emplace_new(tab: table<auto(kT), smart_ptr<auto(vT)>>; key: kT; value: smart_ptr<vT>) : auto

 constructs a new element in-place in the table, set, or array

:Arguments: * **tab** : table<auto(kT);smart_ptr<auto(vT)>>

            * **key** : kT

            * **value** : smart_ptr<vT>

.. _function-builtin_emplace_table_ls_auto,_auto_gr__auto_auto:

.. das:function:: emplace(Tab: table<auto, auto>; key: auto; value: auto) : auto

 emplace will push to dynamic array `array_arg` the content of `value`. `value` has to be of the same type (or const reference to same type) as array values. if `at` is provided `value` will be pushed at index `at`, otherwise to the end of array. The `content` of value will be moved (<-) to it.

:Arguments: * **Tab** : table<auto;auto>

            * **key** : auto

            * **value** : auto

.. _function-builtin_emplace_array_ls_auto_gr__auto:

.. das:function:: emplace(a: array<auto>; value: auto) : auto

 emplace will push to dynamic array `array_arg` the content of `value`. `value` has to be of the same type (or const reference to same type) as array values. if `at` is provided `value` will be pushed at index `at`, otherwise to the end of array. The `content` of value will be moved (<-) to it.

:Arguments: * **a** : array<auto>

            * **value** : auto

.. _function-builtin_emplace_new_array_ls_smart_ptr_ls_autonumT_gr__gr__smart_ptr_ls_numT_gr_:

.. das:function:: emplace_new(Arr: array<smart_ptr<auto(numT)>>; value: smart_ptr<numT>) : auto

 constructs a new element in-place in the table, set, or array

:Arguments: * **Arr** : array<smart_ptr<auto(numT)>>

            * **value** : smart_ptr<numT>

.. _function-builtin_insert_default_table_ls_autoTT,_autoQQ_gr__TTTT_hh__QQ__eq__eq_constQQ_hh___eq__eq_const:

.. das:function:: insert_default(tab: table<auto(TT), auto(QQ)>; key: TT|TT#; value: QQ ==const|QQ# ==const)

 inserts key into the table or set, if it doesn't already exist

:Arguments: * **tab** : table<auto(TT);auto(QQ)>

            * **key** : option<TT|TT#>

            * **value** : option<QQ!|QQ#!>

.. _function-builtin_insert_default_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT__eq__eq_constvalT_const_hh___eq__eq_const:

.. das:function:: insert_default(tab: table<auto(keyT), auto(valT)>; key: keyT|keyT#; value: valT ==const|valT const# ==const)

 inserts key into the table or set, if it doesn't already exist

:Arguments: * **tab** : table<auto(keyT);auto(valT)>

            * **key** : option<keyT|keyT#>

            * **value** : option<valT!|valT#!>

.. _function-builtin_insert_default_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh_:

.. das:function:: insert_default(tab: table<auto(keyT), auto(valT)>; key: keyT|keyT#)

 inserts key into the table or set, if it doesn't already exist

:Arguments: * **tab** : table<auto(keyT);auto(valT)>

            * **key** : option<keyT|keyT#>

.. _function-builtin_emplace_default_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh_:

.. das:function:: emplace_default(tab: table<auto(keyT), auto(valT)>; key: keyT|keyT#)

 constructs a new default element in-place in the table, set, or array, if it doesn't already exist

:Arguments: * **tab** : table<auto(keyT);auto(valT)>

            * **key** : option<keyT|keyT#>

.. _function-builtin_get_with_default__table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__block_ls_var_p_c_valT_c_void_gr_:

.. das:function:: get_with_default(Tab: table<auto(keyT), auto(valT)>; at: keyT|keyT#; blk: block<(var p:valT&):void>)

 gets the value from the table, add it with a default value if it does not exist. Callback is invoked with the value.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)>!

            * **at** : option<keyT|keyT#>

            * **blk** : block<(p:valT&):void>

.. _function-builtin_modify__table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__block_ls_p_c_valT_c_valT_gr_:

.. das:function:: modify(Tab: table<auto(keyT), auto(valT)>; at: keyT|keyT#; blk: block<(p:valT):valT>)

 modifies the value in the table, if it exists. Callback is invoked with the value.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)>!

            * **at** : option<keyT|keyT#>

            * **blk** : block<(p:valT&):valT>

.. _function-builtin_key_exists_table_ls_autokeyT;autovalT_gr_table_ls_autokeyT;autovalT_gr__hh__string_hh_:

.. das:function:: key_exists(Tab: table<auto(keyT);auto(valT)>|table<auto(keyT);auto(valT)>#; at: string#) : bool

 will return true if element `key` exists in table `table_arg`.

:Arguments: * **Tab** : option<table<auto(keyT);auto(valT)>|table<auto(keyT);auto(valT)>#>

            * **at** : string#

.. _function-builtin_key_exists_table_ls_autokeyT;autovalT_gr_table_ls_autokeyT;autovalT_gr__hh__keyTkeyT_hh_:

.. das:function:: key_exists(Tab: table<auto(keyT);auto(valT)>|table<auto(keyT);auto(valT)>#; at: keyT|keyT#) : bool

 will return true if element `key` exists in table `table_arg`.

:Arguments: * **Tab** : option<table<auto(keyT);auto(valT)>|table<auto(keyT);auto(valT)>#>

            * **at** : option<keyT|keyT#>

.. _function-builtin_copy_to_local_autoTT:

.. das:function:: copy_to_local(a: auto(TT)) : TT

 copies value and returns it as local value on stack. used to work around aliasing issues

:Arguments: * **a** : auto(TT)

.. _function-builtin_move_to_local_autoTT:

.. das:function:: move_to_local(a: auto(TT)&) : TT

 moves value and returns it as local value on stack. used to work around aliasing issues

:Arguments: * **a** : auto(TT)&

.. _function-builtin_keys_table_ls_autokeyT;autovalT_gr___eq__eq_consttable_ls_autokeyT;autovalT_gr__const_hh___eq__eq_const:

.. das:function:: keys(a: table<auto(keyT);auto(valT)> ==const|table<auto(keyT);auto(valT)> const# ==const) : iterator<keyT>

 returns iterator to all keys of the table

:Arguments: * **a** : option<table<auto(keyT);auto(valT)>!|table<auto(keyT);auto(valT)>#!>

.. _function-builtin_keys_table_ls_autokeyT;autovalT_gr___eq__eq_consttable_ls_autokeyT;autovalT_gr__hh___eq__eq_const:

.. das:function:: keys(a: table<auto(keyT);auto(valT)> ==const|table<auto(keyT);auto(valT)># ==const) : iterator<keyT>

 returns iterator to all keys of the table

:Arguments: * **a** : option<table<auto(keyT);auto(valT)>!|table<auto(keyT);auto(valT)>#!>

.. _function-builtin_values_table_ls_autokeyT;void_gr___eq__eq_consttable_ls_autokeyT;void_gr__const_hh___eq__eq_const:

.. das:function:: values(a: table<auto(keyT);void> ==const|table<auto(keyT);void> const# ==const) : auto

 returns iterator to all values of the table

:Arguments: * **a** : option<table<auto(keyT);void>!|table<auto(keyT);void>#!>

.. _function-builtin_values_table_ls_autokeyT;void_gr___eq__eq_consttable_ls_autokeyT;void_gr__hh___eq__eq_const:

.. das:function:: values(a: table<auto(keyT);void> ==const|table<auto(keyT);void># ==const) : auto

 returns iterator to all values of the table

:Arguments: * **a** : option<table<auto(keyT);void>!|table<auto(keyT);void>#!>

.. _function-builtin_values_table_ls_autokeyT;autovalT_gr___eq__eq_consttable_ls_autokeyT;autovalT_gr__const_hh___eq__eq_const:

.. das:function:: values(a: table<auto(keyT);auto(valT)> ==const|table<auto(keyT);auto(valT)> const# ==const) : iterator<valT const&>

 returns iterator to all values of the table

:Arguments: * **a** : option<table<auto(keyT);auto(valT)>!|table<auto(keyT);auto(valT)>#!>

.. _function-builtin_values_table_ls_autokeyT;autovalT_gr___eq__eq_consttable_ls_autokeyT;autovalT_gr__hh___eq__eq_const:

.. das:function:: values(a: table<auto(keyT);auto(valT)> ==const|table<auto(keyT);auto(valT)># ==const) : iterator<valT&>

 returns iterator to all values of the table

:Arguments: * **a** : option<table<auto(keyT);auto(valT)>!|table<auto(keyT);auto(valT)>#!>

.. _function-builtin_values_table_ls_autokeyT;autovalT_gr___eq__eq_consttable_ls_autokeyT;autovalT_gr__const_hh___eq__eq_const:

.. das:function:: values(a: table<auto(keyT);auto(valT)[]> ==const|table<auto(keyT);auto(valT)[]> const# ==const) : iterator<valT const[-2]&>

 returns iterator to all values of the table

:Arguments: * **a** : option<table<auto(keyT);auto(valT)[-1]>!|table<auto(keyT);auto(valT)[-1]>#!>

.. _function-builtin_values_table_ls_autokeyT;autovalT_gr___eq__eq_consttable_ls_autokeyT;autovalT_gr__hh___eq__eq_const:

.. das:function:: values(a: table<auto(keyT);auto(valT)[]> ==const|table<auto(keyT);auto(valT)[]># ==const) : iterator<valT[-2]&>

 returns iterator to all values of the table

:Arguments: * **a** : option<table<auto(keyT);auto(valT)[-1]>!|table<auto(keyT);auto(valT)[-1]>#!>

.. _function-builtin_lock_table_ls_autokeyT;autovalT_gr_table_ls_autokeyT;autovalT_gr__hh__block_ls_t_c_table_ls_keyT,_valT_gr__hh__c_void_gr_:

.. das:function:: lock(Tab: table<auto(keyT);auto(valT)>|table<auto(keyT);auto(valT)>#; blk: block<(t:table<keyT, valT>#):void>) : auto

 locks array or table for the duration of the block invocation, so that it can't be resized. values can't be pushed or popped, etc.

:Arguments: * **Tab** : option<table<auto(keyT);auto(valT)>|table<auto(keyT);auto(valT)>#>

            * **blk** : block<(t:table<keyT;valT>#):void>

.. _function-builtin_lock_forever_table_ls_autokeyT;autovalT_gr_table_ls_autokeyT;autovalT_gr__hh_:

.. das:function:: lock_forever(Tab: table<auto(keyT);auto(valT)>|table<auto(keyT);auto(valT)>#) : table<keyT, valT>#

 locks array or table forever

:Arguments: * **Tab** : option<table<auto(keyT);auto(valT)>|table<auto(keyT);auto(valT)>#>

.. _function-builtin_next_iterator_ls_autoTT_gr__TT:

.. das:function:: next(it: iterator<auto(TT)>; value: TT&) : bool

 returns next element in the iterator as the 'value'. result is true if there is element returned, or false if iterator is null or empty

:Arguments: * **it** : iterator<auto(TT)>

            * **value** : TT&

.. _function-builtin_each_range:

.. das:function:: each(rng: range) : iterator<int>

 returns iterator, which iterates though each element of the object. object can be range, static or dynamic array, another iterator.

:Arguments: * **rng** : range

.. _function-builtin_each_string:

.. das:function:: each(str: string) : iterator<int>

 returns iterator, which iterates though each element of the object. object can be range, static or dynamic array, another iterator.

:Arguments: * **str** : string

.. _function-builtin_each_autoTT:

.. das:function:: each(a: auto(TT)[]) : iterator<TT&>

 returns iterator, which iterates though each element of the object. object can be range, static or dynamic array, another iterator.

:Arguments: * **a** : auto(TT)[-1]

.. _function-builtin_each_array_ls_autoTT_gr_:

.. das:function:: each(a: array<auto(TT)>) : iterator<TT&>

 returns iterator, which iterates though each element of the object. object can be range, static or dynamic array, another iterator.

:Arguments: * **a** : array<auto(TT)>

.. _function-builtin_each_array_ls_autoTT_gr__hh_:

.. das:function:: each(a: array<auto(TT)>#) : iterator<TT&#>

 returns iterator, which iterates though each element of the object. object can be range, static or dynamic array, another iterator.

:Arguments: * **a** : array<auto(TT)>#

.. _function-builtin_each_lambda_ls_var_arg_c_autoargT_c_bool_gr_:

.. das:function:: each(lam: lambda<(var arg:auto(argT)):bool>) : iterator<argT>

 returns iterator, which iterates though each element of the object. object can be range, static or dynamic array, another iterator.

:Arguments: * **lam** : lambda<(arg:auto(argT)):bool>

.. _function-builtin_each_ref_lambda_ls_var_arg_c_autoargT_q__c_bool_gr_:

.. das:function:: each_ref(lam: lambda<(var arg:auto(argT)?):bool>) : iterator<argT&>

 similar to each, but iterator returns references instead of values

:Arguments: * **lam** : lambda<(arg:auto(argT)?):bool>

.. _function-builtin_each_enum_autoTT:

.. das:function:: each_enum(tt: auto(TT)) : iterator<TT>

 iterates over each element in the enumeration

:Arguments: * **tt** : auto(TT)

.. _function-builtin_nothing_iterator_ls_autoTT_gr_:

.. das:function:: nothing(it: iterator<auto(TT)>) : iterator<TT>

 returns empty iterator

:Arguments: * **it** : iterator<auto(TT)>

.. _function-builtin_to_array_iterator_ls_autoTT_gr_:

.. das:function:: to_array(it: iterator<auto(TT)>) : array<TT>

 will convert argument (static array, iterator, another dynamic array) to an array. argument elements will be cloned

:Arguments: * **it** : iterator<auto(TT)>

.. _function-builtin_to_array_autoTT:

.. das:function:: to_array(a: auto(TT)[]) : array<TT>

 will convert argument (static array, iterator, another dynamic array) to an array. argument elements will be cloned

:Arguments: * **a** : auto(TT)[-1]

.. _function-builtin_to_array_move_autoTT:

.. das:function:: to_array_move(a: auto(TT)[]) : array<TT>

 will convert argument (static array, iterator, another dynamic array) to an array. argument elements will be copied or moved

:Arguments: * **a** : auto(TT)[-1]

.. _function-builtin_to_array_move_autoTT__eq__eq_const:

.. das:function:: to_array_move(a: auto(TT) ==const) : array<TT>

 will convert argument (static array, iterator, another dynamic array) to an array. argument elements will be copied or moved

:Arguments: * **a** : auto(TT)!

.. _function-builtin_to_array_move__autoTT__eq__eq_const:

.. das:function:: to_array_move(a: auto(TT) ==const) : array<TT>

 will convert argument (static array, iterator, another dynamic array) to an array. argument elements will be copied or moved

:Arguments: * **a** : auto(TT)!

.. _function-builtin_to_table_tuple_ls_autokeyT;autovalT_gr_:

.. das:function:: to_table(a: tuple<auto(keyT);auto(valT)>[]) : table<keyT, valT>

 will convert an array of key-value tuples into a table<key;value> type. arguments will be cloned

:Arguments: * **a** : tuple<auto(keyT);auto(valT)>[-1]

.. _function-builtin_to_table_autokeyT:

.. das:function:: to_table(a: auto(keyT)[]) : table<keyT, void>

 will convert an array of key-value tuples into a table<key;value> type. arguments will be cloned

:Arguments: * **a** : auto(keyT)[-1]

.. _function-builtin_to_table_move_autokeyT:

.. das:function:: to_table_move(a: auto(keyT)[]) : table<keyT, void>

 will convert an array of key-value tuples into a table<key;value> type. arguments will be copied or moved

:Arguments: * **a** : auto(keyT)[-1]

.. _function-builtin_to_table_move_array_ls_autokeyT_gr_:

.. das:function:: to_table_move(a: array<auto(keyT)>) : table<keyT, void>

 will convert an array of key-value tuples into a table<key;value> type. arguments will be copied or moved

:Arguments: * **a** : array<auto(keyT)>

.. _function-builtin_to_table_move_autokeyT:

.. das:function:: to_table_move(a: auto(keyT)) : table<keyT, void>

 will convert an array of key-value tuples into a table<key;value> type. arguments will be copied or moved

:Arguments: * **a** : auto(keyT)

.. _function-builtin_to_table_move_tuple_ls_autokeyT;autovalT_gr_:

.. das:function:: to_table_move(a: tuple<auto(keyT);auto(valT)>) : table<keyT, valT>

 will convert an array of key-value tuples into a table<key;value> type. arguments will be copied or moved

:Arguments: * **a** : tuple<auto(keyT);auto(valT)>

.. _function-builtin_to_table_move_tuple_ls_autokeyT;autovalT_gr_:

.. das:function:: to_table_move(a: tuple<auto(keyT);auto(valT)>[]) : table<keyT, valT>

 will convert an array of key-value tuples into a table<key;value> type. arguments will be copied or moved

:Arguments: * **a** : tuple<auto(keyT);auto(valT)>[-1]

.. _function-builtin_to_table_move_array_ls_tuple_ls_autokeyT;autovalT_gr__gr_:

.. das:function:: to_table_move(a: array<tuple<auto(keyT);auto(valT)>>) : table<keyT, valT>

 will convert an array of key-value tuples into a table<key;value> type. arguments will be copied or moved

:Arguments: * **a** : array<tuple<auto(keyT);auto(valT)>>

.. _function-builtin_sort_autoTTautoTT_hh_:

.. das:function:: sort(a: auto(TT)[]|auto(TT)[]#) : auto

 sorts an array in ascending order.

:Arguments: * **a** : option<auto(TT)[-1]|auto(TT)[-1]#>

.. _function-builtin_sort_array_ls_autoTT_gr_array_ls_autoTT_gr__hh_:

.. das:function:: sort(a: array<auto(TT)>|array<auto(TT)>#) : auto

 sorts an array in ascending order.

:Arguments: * **a** : option<array<auto(TT)>|array<auto(TT)>#>

.. _function-builtin_sort_autoTTautoTT_hh__block_ls_x_c_TT;y_c_TT_c_bool_gr_:

.. das:function:: sort(a: auto(TT)[]|auto(TT)[]#; cmp: block<(x:TT;y:TT):bool>) : auto

 sorts an array in ascending order.

:Arguments: * **a** : option<auto(TT)[-1]|auto(TT)[-1]#>

            * **cmp** : block<(x:TT;y:TT):bool>

.. _function-builtin_sort_array_ls_autoTT_gr_array_ls_autoTT_gr__hh__block_ls_x_c_TT;y_c_TT_c_bool_gr_:

.. das:function:: sort(a: array<auto(TT)>|array<auto(TT)>#; cmp: block<(x:TT;y:TT):bool>) : auto

 sorts an array in ascending order.

:Arguments: * **a** : option<array<auto(TT)>|array<auto(TT)>#>

            * **cmp** : block<(x:TT;y:TT):bool>

.. _function-builtin_lock_array_ls_autoTT_gr___eq__eq_constarray_ls_autoTT_gr__hh___eq__eq_const_block_ls_var_x_c_array_ls_TT_gr__hh__c_auto_gr_:

.. das:function:: lock(a: array<auto(TT)> ==const|array<auto(TT)># ==const; blk: block<(var x:array<TT>#):auto>) : auto

 locks array or table for the duration of the block invocation, so that it can't be resized. values can't be pushed or popped, etc.

:Arguments: * **a** : option<array<auto(TT)>!|array<auto(TT)>#!>

            * **blk** : block<(x:array<TT>#):auto>

.. _function-builtin_lock_array_ls_autoTT_gr___eq__eq_constarray_ls_autoTT_gr__const_hh___eq__eq_const_block_ls_x_c_array_ls_TT_gr__hh__c_auto_gr_:

.. das:function:: lock(a: array<auto(TT)> ==const|array<auto(TT)> const# ==const; blk: block<(x:array<TT>#):auto>) : auto

 locks array or table for the duration of the block invocation, so that it can't be resized. values can't be pushed or popped, etc.

:Arguments: * **a** : option<array<auto(TT)>!|array<auto(TT)>#!>

            * **blk** : block<(x:array<TT>#):auto>

.. _function-builtin_find_index_array_ls_autoTT_gr_array_ls_autoTT_gr__hh__TT:

.. das:function:: find_index(arr: array<auto(TT)>|array<auto(TT)>#; key: TT) : auto

 returns index of they key in the array

:Arguments: * **arr** : option<array<auto(TT)>|array<auto(TT)>#>

            * **key** : TT

.. _function-builtin_find_index_autoTTautoTT_hh__TT:

.. das:function:: find_index(arr: auto(TT)[]|auto(TT)[]#; key: TT) : auto

 returns index of they key in the array

:Arguments: * **arr** : option<auto(TT)[-1]|auto(TT)[-1]#>

            * **key** : TT

.. _function-builtin_find_index_iterator_ls_autoTT_gr__TT:

.. das:function:: find_index(arr: iterator<auto(TT)>; key: TT) : auto

 returns index of they key in the array

:Arguments: * **arr** : iterator<auto(TT)>

            * **key** : TT

.. _function-builtin_find_index_if_array_ls_autoTT_gr_array_ls_autoTT_gr__hh__block_ls_key_c_TT_c_bool_gr_:

.. das:function:: find_index_if(arr: array<auto(TT)>|array<auto(TT)>#; blk: block<(key:TT):bool>) : auto

 returns index of the key in the array, where key is checked via compare block

:Arguments: * **arr** : option<array<auto(TT)>|array<auto(TT)>#>

            * **blk** : block<(key:TT):bool>

.. _function-builtin_find_index_if_autoTTautoTT_hh__block_ls_key_c_TT_c_bool_gr_:

.. das:function:: find_index_if(arr: auto(TT)[]|auto(TT)[]#; blk: block<(key:TT):bool>) : auto

 returns index of the key in the array, where key is checked via compare block

:Arguments: * **arr** : option<auto(TT)[-1]|auto(TT)[-1]#>

            * **blk** : block<(key:TT):bool>

.. _function-builtin_find_index_if_iterator_ls_autoTT_gr__block_ls_key_c_TT_c_bool_gr_:

.. das:function:: find_index_if(arr: iterator<auto(TT)>; blk: block<(key:TT):bool>) : auto

 returns index of the key in the array, where key is checked via compare block

:Arguments: * **arr** : iterator<auto(TT)>

            * **blk** : block<(key:TT):bool>

.. _function-builtin_has_value_auto_auto:

.. das:function:: has_value(a: auto; key: auto) : auto

 returns true if iterable `a` (array, dim, etc) contains `key`

:Arguments: * **a** : auto

            * **key** : auto

.. _function-builtin_has_value_iterator_ls_auto_gr__auto:

.. das:function:: has_value(a: iterator<auto>; key: auto) : auto

 returns true if iterable `a` (array, dim, etc) contains `key`

:Arguments: * **a** : iterator<auto>

            * **key** : auto

.. _function-builtin_subarray_autoTT_range:

.. das:function:: subarray(a: auto(TT)[]; r: range) : auto

 returns new array which is copy of a slice of range of the source array

:Arguments: * **a** : auto(TT)[-1]

            * **r** : range

.. _function-builtin_subarray_autoTT_urange:

.. das:function:: subarray(a: auto(TT)[]; r: urange) : auto

 returns new array which is copy of a slice of range of the source array

:Arguments: * **a** : auto(TT)[-1]

            * **r** : urange

.. _function-builtin_subarray_array_ls_autoTT_gr__range:

.. das:function:: subarray(a: array<auto(TT)>; r: range) : auto

 returns new array which is copy of a slice of range of the source array

:Arguments: * **a** : array<auto(TT)>!

            * **r** : range

.. _function-builtin_subarray__array_ls_autoTT_gr__range:

.. das:function:: subarray(a: array<auto(TT)>; r: range) : auto

 returns new array which is copy of a slice of range of the source array

:Arguments: * **a** : array<auto(TT)>!

            * **r** : range

.. _function-builtin_subarray_array_ls_autoTT_gr__urange:

.. das:function:: subarray(a: array<auto(TT)>; r: urange) : auto

 returns new array which is copy of a slice of range of the source array

:Arguments: * **a** : array<auto(TT)>

            * **r** : urange

.. _function-builtin_move_to_ref_auto_auto:

.. das:function:: move_to_ref(a: auto&; b: auto) : auto

 moves `b` into `a`. if `b` is value, it will be copied to `a` instead

:Arguments: * **a** : auto&

            * **b** : auto

.. _function-builtin_clear_table_ls_autoKT,_autoVT_gr_:

.. das:function:: clear(t: table<auto(KT), auto(VT)>) : auto

 clear will clear whole table or array `arg`. The size of `arg` after clear is 0.

:Arguments: * **t** : table<auto(KT);auto(VT)>

++++++++++++++++++++++++
das::string manipulation
++++++++++++++++++++++++

  *  :ref:`peek (src: das_string implicit; block: block\<(string#):void\>) <function-builtin_peek_das_string_implicit_block_ls_string_hh__c_void_gr_>` 

.. _function-builtin_peek_das_string_implicit_block_ls_string_hh__c_void_gr_:

.. das:function:: peek(src: das_string implicit; block: block<(string#):void>)

 returns contents of the das::string as temporary string value. this is fastest way to access contents of das::string as string

:Arguments: * **src** :  :ref:`das_string <handle-builtin-das_string>`  implicit

            * **block** : block<(string#):void> implicit

++++++++++++++
Heap reporting
++++++++++++++

  *  :ref:`heap_allocation_stats () : urange64 <function-builtin_heap_allocation_stats>` 
  *  :ref:`heap_allocation_count () : uint64 <function-builtin_heap_allocation_count>` 
  *  :ref:`string_heap_allocation_stats () : urange64 <function-builtin_string_heap_allocation_stats>` 
  *  :ref:`string_heap_allocation_count () : uint64 <function-builtin_string_heap_allocation_count>` 
  *  :ref:`heap_bytes_allocated () : uint64 <function-builtin_heap_bytes_allocated>` 
  *  :ref:`heap_depth () : int <function-builtin_heap_depth>` 
  *  :ref:`string_heap_bytes_allocated () : uint64 <function-builtin_string_heap_bytes_allocated>` 
  *  :ref:`string_heap_depth () : int <function-builtin_string_heap_depth>` 
  *  :ref:`heap_collect (string_heap: bool = true; validate: bool = false) <function-builtin_heap_collect_bool_bool>` 
  *  :ref:`string_heap_report () <function-builtin_string_heap_report>` 
  *  :ref:`heap_report () <function-builtin_heap_report>` 
  *  :ref:`memory_report (errorsOnly: bool) <function-builtin_memory_report_bool>` 

.. _function-builtin_heap_allocation_stats:

.. das:function:: heap_allocation_stats() : urange64

 Returns heap allocation statistics (bytes allocated and bytes deleted).

.. _function-builtin_heap_allocation_count:

.. das:function:: heap_allocation_count() : uint64

 Returns heap allocation count (total number of allocations).

.. _function-builtin_string_heap_allocation_stats:

.. das:function:: string_heap_allocation_stats() : urange64

 Returns string heap allocation statistics (bytes allocated and bytes deleted).

.. _function-builtin_string_heap_allocation_count:

.. das:function:: string_heap_allocation_count() : uint64

 Returns string heap allocation count (total number of allocations).

.. _function-builtin_heap_bytes_allocated:

.. das:function:: heap_bytes_allocated() : uint64

 will return bytes allocated on heap (i.e. really used, not reserved)

.. _function-builtin_heap_depth:

.. das:function:: heap_depth() : int

 returns number of generations in the regular heap

.. _function-builtin_string_heap_bytes_allocated:

.. das:function:: string_heap_bytes_allocated() : uint64

 returns number of bytes allocated in the string heap

.. _function-builtin_string_heap_depth:

.. das:function:: string_heap_depth() : int

 returns number of generations in the string heap

.. _function-builtin_heap_collect_bool_bool:

.. das:function:: heap_collect(string_heap: bool = true; validate: bool = false)

.. warning:: 
  This is unsafe operation.

 calls garbage collection on the regular heap

:Arguments: * **string_heap** : bool

            * **validate** : bool

.. _function-builtin_string_heap_report:

.. das:function:: string_heap_report()

 reports string heap usage and allocations

.. _function-builtin_heap_report:

.. das:function:: heap_report()

 reports heap usage and allocations

.. _function-builtin_memory_report_bool:

.. das:function:: memory_report(errorsOnly: bool)

 reports memory allocation, optionally GC errors only

:Arguments: * **errorsOnly** : bool

++++++++++++++++++
GC0 infrastructure
++++++++++++++++++

  *  :ref:`gc0_save_ptr (name: string implicit; data: void? implicit) <function-builtin_gc0_save_ptr_string_implicit_void_q__implicit>` 
  *  :ref:`gc0_save_smart_ptr (name: string implicit; data: smart_ptr\<void\> implicit) <function-builtin_gc0_save_smart_ptr_string_implicit_smart_ptr_ls_void_gr__implicit>` 
  *  :ref:`gc0_restore_ptr (name: string implicit) : void? <function-builtin_gc0_restore_ptr_string_implicit>` 
  *  :ref:`gc0_restore_smart_ptr (name: string implicit) : smart_ptr\<void\> <function-builtin_gc0_restore_smart_ptr_string_implicit>` 
  *  :ref:`gc0_reset () <function-builtin_gc0_reset>` 

.. _function-builtin_gc0_save_ptr_string_implicit_void_q__implicit:

.. das:function:: gc0_save_ptr(name: string implicit; data: void? implicit)

 saves pointer to gc0 storage by specifying `name`

:Arguments: * **name** : string implicit

            * **data** : void? implicit

.. _function-builtin_gc0_save_smart_ptr_string_implicit_smart_ptr_ls_void_gr__implicit:

.. das:function:: gc0_save_smart_ptr(name: string implicit; data: smart_ptr<void> implicit)

 saves smart_ptr to gc0 storage by specifying `name`

:Arguments: * **name** : string implicit

            * **data** : smart_ptr<void> implicit

.. _function-builtin_gc0_restore_ptr_string_implicit:

.. das:function:: gc0_restore_ptr(name: string implicit) : void?

 restores pointer from gc0 storage by `name`

:Arguments: * **name** : string implicit

.. _function-builtin_gc0_restore_smart_ptr_string_implicit:

.. das:function:: gc0_restore_smart_ptr(name: string implicit) : smart_ptr<void>

 restores smart_ptr from gc0 storage `name`

:Arguments: * **name** : string implicit

.. _function-builtin_gc0_reset:

.. das:function:: gc0_reset()

 resets gc0 storage. stored pointers will no longer be accessible

++++++++++++++++++++++++
Smart ptr infrastructure
++++++++++++++++++++++++

  *  :ref:`move_new (dest: smart_ptr\<void\>& implicit; src: smart_ptr\<void\> implicit) <function-builtin_move_new_smart_ptr_ls_void_gr__implicit_smart_ptr_ls_void_gr__implicit>` 
  *  :ref:`move (dest: smart_ptr\<void\>& implicit; src: void? implicit) <function-builtin_move_smart_ptr_ls_void_gr__implicit_void_q__implicit>` 
  *  :ref:`move (dest: smart_ptr\<void\>& implicit; src: smart_ptr\<void\>& implicit) <function-builtin_move_smart_ptr_ls_void_gr__implicit_smart_ptr_ls_void_gr__implicit>` 
  *  :ref:`smart_ptr_clone (dest: smart_ptr\<void\>& implicit; src: void? implicit) <function-builtin_smart_ptr_clone_smart_ptr_ls_void_gr__implicit_void_q__implicit>` 
  *  :ref:`smart_ptr_clone (dest: smart_ptr\<void\>& implicit; src: smart_ptr\<void\> implicit) <function-builtin_smart_ptr_clone_smart_ptr_ls_void_gr__implicit_smart_ptr_ls_void_gr__implicit>` 
  *  :ref:`smart_ptr_use_count (ptr: smart_ptr\<void\> implicit) : uint <function-builtin_smart_ptr_use_count_smart_ptr_ls_void_gr__implicit>` 
  *  :ref:`smart_ptr_is_valid (dest: smart_ptr\<void\> implicit) : bool <function-builtin_smart_ptr_is_valid_smart_ptr_ls_void_gr__implicit>` 
  *  :ref:`get_ptr (src: smart_ptr\<auto(TT)\> ==const) : TT? <function-builtin_get_ptr_smart_ptr_ls_autoTT_gr___eq__eq_const>` 
  *  :ref:`get_ptr (var src: smart_ptr\<auto(TT)\> ==const) : TT? <function-builtin_get_ptr__smart_ptr_ls_autoTT_gr___eq__eq_const>` 
  *  :ref:`get_const_ptr (src: smart_ptr\<auto(TT)\>) : TT? <function-builtin_get_const_ptr_smart_ptr_ls_autoTT_gr_>` 
  *  :ref:`add_ptr_ref (src: smart_ptr\<auto(TT)\>) : smart_ptr\<TT\> <function-builtin_add_ptr_ref_smart_ptr_ls_autoTT_gr_>` 

.. _function-builtin_move_new_smart_ptr_ls_void_gr__implicit_smart_ptr_ls_void_gr__implicit:

.. das:function:: move_new(dest: smart_ptr<void>& implicit; src: smart_ptr<void> implicit)

 Moves the new [[...]] value into smart_ptr.

:Arguments: * **dest** : smart_ptr<void>& implicit

            * **src** : smart_ptr<void> implicit

.. _function-builtin_move_smart_ptr_ls_void_gr__implicit_void_q__implicit:

.. das:function:: move(dest: smart_ptr<void>& implicit; src: void? implicit)

 Moves one smart_ptr into another. Semantic equivalent of move(a,b) => a := null, a <- b

:Arguments: * **dest** : smart_ptr<void>& implicit

            * **src** : void? implicit

.. _function-builtin_move_smart_ptr_ls_void_gr__implicit_smart_ptr_ls_void_gr__implicit:

.. das:function:: move(dest: smart_ptr<void>& implicit; src: smart_ptr<void>& implicit)

 Moves one smart_ptr into another. Semantic equivalent of move(a,b) => a := null, a <- b

:Arguments: * **dest** : smart_ptr<void>& implicit

            * **src** : smart_ptr<void>& implicit

.. _function-builtin_smart_ptr_clone_smart_ptr_ls_void_gr__implicit_void_q__implicit:

.. das:function:: smart_ptr_clone(dest: smart_ptr<void>& implicit; src: void? implicit)

 clones smart_ptr, internal use-count is incremented

:Arguments: * **dest** : smart_ptr<void>& implicit

            * **src** : void? implicit

.. _function-builtin_smart_ptr_clone_smart_ptr_ls_void_gr__implicit_smart_ptr_ls_void_gr__implicit:

.. das:function:: smart_ptr_clone(dest: smart_ptr<void>& implicit; src: smart_ptr<void> implicit)

 clones smart_ptr, internal use-count is incremented

:Arguments: * **dest** : smart_ptr<void>& implicit

            * **src** : smart_ptr<void> implicit

.. _function-builtin_smart_ptr_use_count_smart_ptr_ls_void_gr__implicit:

.. das:function:: smart_ptr_use_count(ptr: smart_ptr<void> implicit) : uint

 returns internal use-count for the smart_ptr

:Arguments: * **ptr** : smart_ptr<void> implicit

.. _function-builtin_smart_ptr_is_valid_smart_ptr_ls_void_gr__implicit:

.. das:function:: smart_ptr_is_valid(dest: smart_ptr<void> implicit) : bool

 checks if smart pointer points to a valid data.

:Arguments: * **dest** : smart_ptr<void> implicit

.. _function-builtin_get_ptr_smart_ptr_ls_autoTT_gr___eq__eq_const:

.. das:function:: get_ptr(src: smart_ptr<auto(TT)> ==const) : TT?

 returns regular pointer from the smart_ptr

:Arguments: * **src** : smart_ptr<auto(TT)>!

.. _function-builtin_get_ptr__smart_ptr_ls_autoTT_gr___eq__eq_const:

.. das:function:: get_ptr(src: smart_ptr<auto(TT)> ==const) : TT?

 returns regular pointer from the smart_ptr

:Arguments: * **src** : smart_ptr<auto(TT)>!

.. _function-builtin_get_const_ptr_smart_ptr_ls_autoTT_gr_:

.. das:function:: get_const_ptr(src: smart_ptr<auto(TT)>) : TT?

 return constant pointer from regular pointer

:Arguments: * **src** : smart_ptr<auto(TT)>

.. _function-builtin_add_ptr_ref_smart_ptr_ls_autoTT_gr_:

.. das:function:: add_ptr_ref(src: smart_ptr<auto(TT)>) : smart_ptr<TT>

 increases reference count of the smart pointer.

:Arguments: * **src** : smart_ptr<auto(TT)>

++++++++++++++++++++
Macro infrastructure
++++++++++++++++++++

  *  :ref:`is_compiling () : bool <function-builtin_is_compiling>` 
  *  :ref:`is_compiling_macros () : bool <function-builtin_is_compiling_macros>` 
  *  :ref:`is_compiling_macros_in_module (name: string implicit) : bool <function-builtin_is_compiling_macros_in_module_string_implicit>` 
  *  :ref:`is_reporting_compilation_errors () : bool <function-builtin_is_reporting_compilation_errors>` 
  *  :ref:`is_in_completion () : bool <function-builtin_is_in_completion>` 
  *  :ref:`is_folding () : bool <function-builtin_is_folding>` 

.. _function-builtin_is_compiling:

.. das:function:: is_compiling() : bool

 returns true if context is being compiled

.. _function-builtin_is_compiling_macros:

.. das:function:: is_compiling_macros() : bool

 returns true if context is being compiled and the compiler is currently executing macro pass

.. _function-builtin_is_compiling_macros_in_module_string_implicit:

.. das:function:: is_compiling_macros_in_module(name: string implicit) : bool

 returns true if context is being compiled, its macro pass, and its in the specific module

:Arguments: * **name** : string implicit

.. _function-builtin_is_reporting_compilation_errors:

.. das:function:: is_reporting_compilation_errors() : bool

 returns true if context failed to compile, and infer pass is reporting compilation errors

.. _function-builtin_is_in_completion:

.. das:function:: is_in_completion() : bool

 returns true if compiler is currently generating completion, i.e. lexical representation of the program for the text editor's text completion system.

.. _function-builtin_is_folding:

.. das:function:: is_folding() : bool

 returns true if context is beeing folded, i.e during constant folding pass

++++++++
Profiler
++++++++

  *  :ref:`reset_profiler () <function-builtin_reset_profiler>` 
  *  :ref:`dump_profile_info () <function-builtin_dump_profile_info>` 
  *  :ref:`collect_profile_info () : string <function-builtin_collect_profile_info>` 
  *  :ref:`profile (count: int; category: string implicit; block: block\<():void\>) : float <function-builtin_profile_int_string_implicit_block_ls__c_void_gr_>` 

.. _function-builtin_reset_profiler:

.. das:function:: reset_profiler()

 resets counters in the built-in profiler

.. _function-builtin_dump_profile_info:

.. das:function:: dump_profile_info()

 dumps use counts of all lines collected by built-in profiler

.. _function-builtin_collect_profile_info:

.. das:function:: collect_profile_info() : string

 enabling collecting of the use counts by built-in profiler

.. _function-builtin_profile_int_string_implicit_block_ls__c_void_gr_:

.. das:function:: profile(count: int; category: string implicit; block: block<():void>) : float

 profiles specified block by evaluating it `count` times and returns minimal time spent in the block in seconds, as well as prints it.

:Arguments: * **count** : int

            * **category** : string implicit

            * **block** : block<void> implicit

++++++++++++++++++++
System infastructure
++++++++++++++++++++

  *  :ref:`get_das_root () : string <function-builtin_get_das_root>` 
  *  :ref:`panic (text: string implicit) <function-builtin_panic_string_implicit>` 
  *  :ref:`print (text: string implicit) <function-builtin_print_string_implicit>` 
  *  :ref:`error (text: string implicit) <function-builtin_error_string_implicit>` 
  *  :ref:`sprint (value: any; flags: print_flags) : string <function-builtin_sprint_any_print_flags>` 
  *  :ref:`sprint_json (value: any; humanReadable: bool) : string <function-builtin_sprint_json_any_bool>` 
  *  :ref:`terminate () <function-builtin_terminate>` 
  *  :ref:`breakpoint () <function-builtin_breakpoint>` 
  *  :ref:`stackwalk (args: bool = true; vars: bool = true) <function-builtin_stackwalk_bool_bool>` 
  *  :ref:`is_intern_strings () : bool <function-builtin_is_intern_strings>` 
  *  :ref:`is_in_aot () : bool <function-builtin_is_in_aot>` 
  *  :ref:`to_log (level: int; text: string implicit) <function-builtin_to_log_int_string_implicit>` 
  *  :ref:`to_compiler_log (text: string implicit) <function-builtin_to_compiler_log_string_implicit>` 
  *  :ref:`eval_main_loop (block: block\<():void\>) <function-builtin_eval_main_loop_block_ls__c_void_gr_>` 
  *  :ref:`aot_enabled () : bool <function-builtin_aot_enabled>` 

.. _function-builtin_get_das_root:

.. das:function:: get_das_root() : string

 returns path to where `daslib` and other libraries exist. this is typically root folder of the Daslang main repository

.. _function-builtin_panic_string_implicit:

.. das:function:: panic(text: string implicit)

 will cause panic. The program will be determinated if there is no recover. Panic is not a error handling mechanism and can not be used as such. It is indeed panic, fatal error. It is not supposed that program can completely correctly recover from panic, recover construction is provided so program can try to correcly shut-down or report fatal error. If there is no recover withing script, it will be called in calling eval (in C++ callee code).

:Arguments: * **text** : string implicit

.. _function-builtin_print_string_implicit:

.. das:function:: print(text: string implicit)

 outputs string into current context log output

:Arguments: * **text** : string implicit

.. _function-builtin_error_string_implicit:

.. das:function:: error(text: string implicit)

 similar to 'print' but outputs to context error output

:Arguments: * **text** : string implicit

.. _function-builtin_sprint_any_print_flags:

.. das:function:: sprint(value: any; flags: print_flags) : string

 similar to 'print' but returns string instead of printing it

:Arguments: * **value** : any

            * **flags** :  :ref:`print_flags <alias-print_flags>` 

.. _function-builtin_sprint_json_any_bool:

.. das:function:: sprint_json(value: any; humanReadable: bool) : string

 similar to 'write_json' but skips intermediate representation. this is faster but less flexible

:Arguments: * **value** : any

            * **humanReadable** : bool

.. _function-builtin_terminate:

.. das:function:: terminate()

 terminates current context execution

.. _function-builtin_breakpoint:

.. das:function:: breakpoint()

 breakpoint will call os_debugbreakpoint, which is link-time unresolved dependency. It's supposed to call breakpoint in debugger tool, as sample implementation does.

.. _function-builtin_stackwalk_bool_bool:

.. das:function:: stackwalk(args: bool = true; vars: bool = true)

 stackwalk prints call stack and local variables values

:Arguments: * **args** : bool

            * **vars** : bool

.. _function-builtin_is_intern_strings:

.. das:function:: is_intern_strings() : bool

 returns true if string interning is enabled

.. _function-builtin_is_in_aot:

.. das:function:: is_in_aot() : bool

 returns true if compiler is currently generating AOT

.. _function-builtin_to_log_int_string_implicit:

.. das:function:: to_log(level: int; text: string implicit)

 similar to print but output goes to the logging infrastructure. `arg0` specifies log level, i.e. LOG_... constants

:Arguments: * **level** : int

            * **text** : string implicit

.. _function-builtin_to_compiler_log_string_implicit:

.. das:function:: to_compiler_log(text: string implicit)

 Output text to compiler log, usually from the macro.

:Arguments: * **text** : string implicit

.. _function-builtin_eval_main_loop_block_ls__c_void_gr_:

.. das:function:: eval_main_loop(block: block<():void>)

 executes main loop for the application. has specific implementation in EMSCRIPTEN, otherwise invoke until false.

:Arguments: * **block** : block<void> implicit

.. _function-builtin_aot_enabled:

.. das:function:: aot_enabled() : bool

 Returns true if AOT is enabled.

+++++++++++++++++++
Memory manipulation
+++++++++++++++++++

  *  :ref:`variant_index (arg0: variant\<\> implicit) : int <function-builtin_variant_index_variant_ls__gr__implicit>` 
  *  :ref:`set_variant_index (variant: variant\<\> implicit; index: int) <function-builtin_set_variant_index_variant_ls__gr__implicit_int>` 
  *  :ref:`hash (data: any) : uint64 <function-builtin_hash_any>` 
  *  :ref:`hash (data: string implicit) : uint64 <function-builtin_hash_string_implicit>` 
  *  :ref:`hash (value: int8) : uint64 <function-builtin_hash_int8>` 
  *  :ref:`hash (value: uint8) : uint64 <function-builtin_hash_uint8>` 
  *  :ref:`hash (value: int16) : uint64 <function-builtin_hash_int16>` 
  *  :ref:`hash (value: uint16) : uint64 <function-builtin_hash_uint16>` 
  *  :ref:`hash (value: int) : uint64 <function-builtin_hash_int>` 
  *  :ref:`hash (value: uint) : uint64 <function-builtin_hash_uint>` 
  *  :ref:`hash (value: int64) : uint64 <function-builtin_hash_int64>` 
  *  :ref:`hash (value: uint64) : uint64 <function-builtin_hash_uint64>` 
  *  :ref:`hash (value: void? implicit) : uint64 <function-builtin_hash_void_q__implicit>` 
  *  :ref:`hash (value: float) : uint64 <function-builtin_hash_float>` 
  *  :ref:`hash (value: double) : uint64 <function-builtin_hash_double>` 
  *  :ref:`hash (value: das_string implicit) : uint64 <function-builtin_hash_das_string_implicit>` 
  *  :ref:`memcpy (left: void? implicit; right: void? implicit; size: int) <function-builtin_memcpy_void_q__implicit_void_q__implicit_int>` 
  *  :ref:`memcmp (left: void? implicit; right: void? implicit; size: int) : int <function-builtin_memcmp_void_q__implicit_void_q__implicit_int>` 
  *  :ref:`intptr (p: void?) : uint64 <function-builtin_intptr_void_q_>` 
  *  :ref:`intptr (p: smart_ptr\<auto\>) : uint64 <function-builtin_intptr_smart_ptr_ls_auto_gr_>` 
  *  :ref:`lock_data (var a: array\<auto(TT)\> ==const|array\<auto(TT)\># ==const; blk: block\<(var p:TT?#;s:int):auto\>) : auto <function-builtin_lock_data_array_ls_autoTT_gr___eq__eq_constarray_ls_autoTT_gr__hh___eq__eq_const_block_ls_var_p_c_TT_q__hh_;s_c_int_c_auto_gr_>` 
  *  :ref:`lock_data (a: array\<auto(TT)\> ==const|array\<auto(TT)\> const# ==const; blk: block\<(p:TT const?#;s:int):auto\>) : auto <function-builtin_lock_data_array_ls_autoTT_gr___eq__eq_constarray_ls_autoTT_gr__const_hh___eq__eq_const_block_ls_p_c_TT_const_q__hh_;s_c_int_c_auto_gr_>` 
  *  :ref:`map_to_array (data: void?; len: int; blk: block\<(var arg:array\<auto(TT)\>#):auto\>) : auto <function-builtin_map_to_array_void_q__int_block_ls_var_arg_c_array_ls_autoTT_gr__hh__c_auto_gr_>` 
  *  :ref:`map_to_ro_array (data: void?; len: int; blk: block\<(arg:array\<auto(TT)\>#):auto\>) : auto <function-builtin_map_to_ro_array_void_q__int_block_ls_arg_c_array_ls_autoTT_gr__hh__c_auto_gr_>` 

.. _function-builtin_variant_index_variant_ls__gr__implicit:

.. das:function:: variant_index(arg0: variant<> implicit) : int

 returns internal index of the variant value

:Arguments: * **arg0** : variant<> implicit

.. _function-builtin_set_variant_index_variant_ls__gr__implicit_int:

.. das:function:: set_variant_index(variant: variant<> implicit; index: int)

.. warning:: 
  This is unsafe operation.

 sets internal index of the variant value

:Arguments: * **variant** : variant<> implicit

            * **index** : int

.. _function-builtin_hash_any:

.. das:function:: hash(data: any) : uint64

 returns hash value of the `data`. current implementation uses FNV64a hash.

:Arguments: * **data** : any

.. _function-builtin_hash_string_implicit:

.. das:function:: hash(data: string implicit) : uint64

 returns hash value of the `data`. current implementation uses FNV64a hash.

:Arguments: * **data** : string implicit

.. _function-builtin_hash_int8:

.. das:function:: hash(value: int8) : uint64

 returns hash value of the `data`. current implementation uses FNV64a hash.

:Arguments: * **value** : int8

.. _function-builtin_hash_uint8:

.. das:function:: hash(value: uint8) : uint64

 returns hash value of the `data`. current implementation uses FNV64a hash.

:Arguments: * **value** : uint8

.. _function-builtin_hash_int16:

.. das:function:: hash(value: int16) : uint64

 returns hash value of the `data`. current implementation uses FNV64a hash.

:Arguments: * **value** : int16

.. _function-builtin_hash_uint16:

.. das:function:: hash(value: uint16) : uint64

 returns hash value of the `data`. current implementation uses FNV64a hash.

:Arguments: * **value** : uint16

.. _function-builtin_hash_int:

.. das:function:: hash(value: int) : uint64

 returns hash value of the `data`. current implementation uses FNV64a hash.

:Arguments: * **value** : int

.. _function-builtin_hash_uint:

.. das:function:: hash(value: uint) : uint64

 returns hash value of the `data`. current implementation uses FNV64a hash.

:Arguments: * **value** : uint

.. _function-builtin_hash_int64:

.. das:function:: hash(value: int64) : uint64

 returns hash value of the `data`. current implementation uses FNV64a hash.

:Arguments: * **value** : int64

.. _function-builtin_hash_uint64:

.. das:function:: hash(value: uint64) : uint64

 returns hash value of the `data`. current implementation uses FNV64a hash.

:Arguments: * **value** : uint64

.. _function-builtin_hash_void_q__implicit:

.. das:function:: hash(value: void? implicit) : uint64

 returns hash value of the `data`. current implementation uses FNV64a hash.

:Arguments: * **value** : void? implicit

.. _function-builtin_hash_float:

.. das:function:: hash(value: float) : uint64

 returns hash value of the `data`. current implementation uses FNV64a hash.

:Arguments: * **value** : float

.. _function-builtin_hash_double:

.. das:function:: hash(value: double) : uint64

 returns hash value of the `data`. current implementation uses FNV64a hash.

:Arguments: * **value** : double

.. _function-builtin_hash_das_string_implicit:

.. das:function:: hash(value: das_string implicit) : uint64

 returns hash value of the `data`. current implementation uses FNV64a hash.

:Arguments: * **value** :  :ref:`das_string <handle-builtin-das_string>`  implicit

.. _function-builtin_memcpy_void_q__implicit_void_q__implicit_int:

.. das:function:: memcpy(left: void? implicit; right: void? implicit; size: int)

.. warning:: 
  This is unsafe operation.

 copies `size` bytes of memory from `right` to `left`

:Arguments: * **left** : void? implicit

            * **right** : void? implicit

            * **size** : int

.. _function-builtin_memcmp_void_q__implicit_void_q__implicit_int:

.. das:function:: memcmp(left: void? implicit; right: void? implicit; size: int) : int

.. warning:: 
  This is unsafe operation.

 similar to C 'memcmp', compares `size` bytes of `left`` and `right` memory. returns -1 if left is less, 1 if left is greater, and 0 if left is same as right

:Arguments: * **left** : void? implicit

            * **right** : void? implicit

            * **size** : int

.. _function-builtin_intptr_void_q_:

.. das:function:: intptr(p: void?) : uint64

 returns int64 representation of a pointer

:Arguments: * **p** : void?

.. _function-builtin_intptr_smart_ptr_ls_auto_gr_:

.. das:function:: intptr(p: smart_ptr<auto>) : uint64

 returns int64 representation of a pointer

:Arguments: * **p** : smart_ptr<auto>

.. _function-builtin_lock_data_array_ls_autoTT_gr___eq__eq_constarray_ls_autoTT_gr__hh___eq__eq_const_block_ls_var_p_c_TT_q__hh_;s_c_int_c_auto_gr_:

.. das:function:: lock_data(a: array<auto(TT)> ==const|array<auto(TT)># ==const; blk: block<(var p:TT?#;s:int):auto>) : auto

 locks array and invokes block with a pointer to array's data

:Arguments: * **a** : option<array<auto(TT)>!|array<auto(TT)>#!>

            * **blk** : block<(p:TT?#;s:int):auto>

.. _function-builtin_lock_data_array_ls_autoTT_gr___eq__eq_constarray_ls_autoTT_gr__const_hh___eq__eq_const_block_ls_p_c_TT_const_q__hh_;s_c_int_c_auto_gr_:

.. das:function:: lock_data(a: array<auto(TT)> ==const|array<auto(TT)> const# ==const; blk: block<(p:TT const?#;s:int):auto>) : auto

 locks array and invokes block with a pointer to array's data

:Arguments: * **a** : option<array<auto(TT)>!|array<auto(TT)>#!>

            * **blk** : block<(p:TT?#;s:int):auto>

.. _function-builtin_map_to_array_void_q__int_block_ls_var_arg_c_array_ls_autoTT_gr__hh__c_auto_gr_:

.. das:function:: map_to_array(data: void?; len: int; blk: block<(var arg:array<auto(TT)>#):auto>) : auto

.. warning:: 
  This is unsafe operation.

 builds temporary array from the specified memory

:Arguments: * **data** : void?

            * **len** : int

            * **blk** : block<(arg:array<auto(TT)>#):auto>

.. _function-builtin_map_to_ro_array_void_q__int_block_ls_arg_c_array_ls_autoTT_gr__hh__c_auto_gr_:

.. das:function:: map_to_ro_array(data: void?; len: int; blk: block<(arg:array<auto(TT)>#):auto>) : auto

.. warning:: 
  This is unsafe operation.

 same as `map_to_array` but array is read-only

:Arguments: * **data** : void?

            * **len** : int

            * **blk** : block<(arg:array<auto(TT)>#):auto>

+++++++++++++++++
Binary serializer
+++++++++++++++++

  *  :ref:`binary_save (obj: auto; subexpr: block\<(data:array\<uint8\>#):void\>) : auto <function-builtin_binary_save_auto_block_ls_data_c_array_ls_uint8_gr__hh__c_void_gr_>` 
  *  :ref:`binary_load (var obj: auto; data: array\<uint8\>) : auto <function-builtin_binary_load_auto_array_ls_uint8_gr_>` 

.. _function-builtin_binary_save_auto_block_ls_data_c_array_ls_uint8_gr__hh__c_void_gr_:

.. das:function:: binary_save(obj: auto; subexpr: block<(data:array<uint8>#):void>) : auto

 saves any data to array<uint8>. obsolete, use daslib/archive instead

:Arguments: * **obj** : auto

            * **subexpr** : block<(data:array<uint8>#):void>

.. _function-builtin_binary_load_auto_array_ls_uint8_gr_:

.. das:function:: binary_load(obj: auto; data: array<uint8>) : auto

 loads any data from array<uint8>. obsolete, use daslib/archive instead

:Arguments: * **obj** : auto

            * **data** : array<uint8> implicit

+++++++++++++++++++++
Path and command line
+++++++++++++++++++++

  *  :ref:`get_command_line_arguments () : array\<string\> <function-builtin_get_command_line_arguments>` 

.. _function-builtin_get_command_line_arguments:

.. das:function:: get_command_line_arguments() : array<string>

 returns array of command line arguments.

+++++++++++++
Time and date
+++++++++++++

  *  :ref:`get_clock () : clock <function-builtin_get_clock>` 
  *  :ref:`mktime (year: int; month: int; mday: int; hour: int; min: int; sec: int) : clock <function-builtin_mktime_int_int_int_int_int_int>` 
  *  :ref:`ref_time_ticks () : int64 <function-builtin_ref_time_ticks>` 
  *  :ref:`get_time_usec (ref: int64) : int <function-builtin_get_time_usec_int64>` 
  *  :ref:`get_time_nsec (ref: int64) : int64 <function-builtin_get_time_nsec_int64>` 

.. _function-builtin_get_clock:

.. das:function:: get_clock() : clock

 return a current calendar time. The value returned generally represents the number of seconds since 00:00 hours, Jan 1, 1970 UTC (i.e., the current unix timestamp).

.. _function-builtin_mktime_int_int_int_int_int_int:

.. das:function:: mktime(year: int; month: int; mday: int; hour: int; min: int; sec: int) : clock

 Converts calendar time to time since epoch.

:Arguments: * **year** : int

            * **month** : int

            * **mday** : int

            * **hour** : int

            * **min** : int

            * **sec** : int

.. _function-builtin_ref_time_ticks:

.. das:function:: ref_time_ticks() : int64

 returns current time in ticks

.. _function-builtin_get_time_usec_int64:

.. das:function:: get_time_usec(ref: int64) : int

 returns time interval in usec, since the specified `reft` (usually from `ref_time_ticks`)

:Arguments: * **ref** : int64

.. _function-builtin_get_time_nsec_int64:

.. das:function:: get_time_nsec(ref: int64) : int64

 returns time interval in nsec, since the specified `reft` (usually from `ref_time_ticks`)

:Arguments: * **ref** : int64

+++++++++++++
Lock checking
+++++++++++++

  *  :ref:`lock_count (array: array\<anything\>) : int <function-builtin_lock_count_array_ls_anything_gr_>` 
  *  :ref:`set_verify_array_locks (array: array\<anything\>; check: bool) : bool <function-builtin_set_verify_array_locks_array_ls_anything_gr__bool>` 
  *  :ref:`set_verify_table_locks (table: table\<anything, anything\>; check: bool) : bool <function-builtin_set_verify_table_locks_table_ls_anything,_anything_gr__bool>` 

.. _function-builtin_lock_count_array_ls_anything_gr_:

.. das:function:: lock_count(array: array<anything>) : int

 returns internal lock count for the array or table

:Arguments: * **array** : array implicit

.. _function-builtin_set_verify_array_locks_array_ls_anything_gr__bool:

.. das:function:: set_verify_array_locks(array: array<anything>; check: bool) : bool

.. warning:: 
  This is unsafe operation.

 runtime optimization, which indicates that the array does not need lock checks.

:Arguments: * **array** : array implicit

            * **check** : bool

.. _function-builtin_set_verify_table_locks_table_ls_anything,_anything_gr__bool:

.. das:function:: set_verify_table_locks(table: table<anything, anything>; check: bool) : bool

.. warning:: 
  This is unsafe operation.

 runtime optimization, which indicates that the table does not need lock checks.

:Arguments: * **table** : table implicit

            * **check** : bool

+++++++++++++++++++++++
Lock checking internals
+++++++++++++++++++++++

  *  :ref:`_move_with_lockcheck (var a: auto(valA)&; var b: auto(valB)&) : auto <function-builtin__move_with_lockcheck_autovalA_autovalB>` 
  *  :ref:`_return_with_lockcheck (var a: auto(valT)& ==const) : auto& <function-builtin__return_with_lockcheck__autovalT__eq__eq_const>` 
  *  :ref:`_return_with_lockcheck (a: auto(valT) const& ==const) : auto& <function-builtin__return_with_lockcheck_autovalT_const__eq__eq_const>` 
  *  :ref:`_at_with_lockcheck (var Tab: table\<auto(keyT), auto(valT)\>; at: keyT|keyT#) : valT& <function-builtin__at_with_lockcheck_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh_>` 

.. _function-builtin__move_with_lockcheck_autovalA_autovalB:

.. das:function:: _move_with_lockcheck(a: auto(valA)&; b: auto(valB)&) : auto

 moves `b` into `a`, checks if `a` or `b` is locked, recursively for each lockable element of a and b

:Arguments: * **a** : auto(valA)&

            * **b** : auto(valB)&

.. _function-builtin__return_with_lockcheck__autovalT__eq__eq_const:

.. das:function:: _return_with_lockcheck(a: auto(valT)& ==const) : auto&

 returns `a`. check if `a` is locked, recursively for each lockable element of a

:Arguments: * **a** : auto(valT)&!

.. _function-builtin__return_with_lockcheck_autovalT_const__eq__eq_const:

.. das:function:: _return_with_lockcheck(a: auto(valT) const& ==const) : auto&

 returns `a`. check if `a` is locked, recursively for each lockable element of a

:Arguments: * **a** : auto(valT)&!

.. _function-builtin__at_with_lockcheck_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh_:

.. das:function:: _at_with_lockcheck(Tab: table<auto(keyT), auto(valT)>; at: keyT|keyT#) : valT&

 returns element of the table `Tab`, also checks if `Tab` is locked, recursively for each lockable element of `Tab`

:Arguments: * **Tab** : table<auto(keyT);auto(valT)>

            * **at** : option<keyT|keyT#>

++++++++++++++
Bit operations
++++++++++++++

  *  :ref:`clz (bits: uint) : uint <function-builtin_clz_uint>` 
  *  :ref:`clz (bits: uint64) : uint64 <function-builtin_clz_uint64>` 
  *  :ref:`ctz (bits: uint) : uint <function-builtin_ctz_uint>` 
  *  :ref:`ctz (bits: uint64) : uint64 <function-builtin_ctz_uint64>` 
  *  :ref:`popcnt (bits: uint) : uint <function-builtin_popcnt_uint>` 
  *  :ref:`popcnt (bits: uint64) : uint64 <function-builtin_popcnt_uint64>` 
  *  :ref:`mul128 (a: uint64; b: uint64) : urange64 <function-builtin_mul128_uint64_uint64>` 
  *  :ref:`__bit_set (value: bitfield& implicit; mask: bitfield; on: bool) <function-builtin___bit_set_bitfield_implicit_bitfield_bool>` 
  *  :ref:`__bit_set (value: bitfield8:uint8\<\>& implicit; mask: bitfield8:uint8\<\>; on: bool) <function-builtin___bit_set_bitfield8_c_uint8_ls__gr__implicit_bitfield8_c_uint8_ls__gr__bool>` 
  *  :ref:`__bit_set (value: bitfield16:uint16\<\>& implicit; mask: bitfield16:uint16\<\>; on: bool) <function-builtin___bit_set_bitfield16_c_uint16_ls__gr__implicit_bitfield16_c_uint16_ls__gr__bool>` 
  *  :ref:`__bit_set (value: bitfield64:uint64\<\>& implicit; mask: bitfield64:uint64\<\>; on: bool) <function-builtin___bit_set_bitfield64_c_uint64_ls__gr__implicit_bitfield64_c_uint64_ls__gr__bool>` 

.. _function-builtin_clz_uint:

.. das:function:: clz(bits: uint) : uint

 count leading zeros

:Arguments: * **bits** : uint

.. _function-builtin_clz_uint64:

.. das:function:: clz(bits: uint64) : uint64

 count leading zeros

:Arguments: * **bits** : uint64

.. _function-builtin_ctz_uint:

.. das:function:: ctz(bits: uint) : uint

 count trailing zeros

:Arguments: * **bits** : uint

.. _function-builtin_ctz_uint64:

.. das:function:: ctz(bits: uint64) : uint64

 count trailing zeros

:Arguments: * **bits** : uint64

.. _function-builtin_popcnt_uint:

.. das:function:: popcnt(bits: uint) : uint

 count number of set bits

:Arguments: * **bits** : uint

.. _function-builtin_popcnt_uint64:

.. das:function:: popcnt(bits: uint64) : uint64

 count number of set bits

:Arguments: * **bits** : uint64

.. _function-builtin_mul128_uint64_uint64:

.. das:function:: mul128(a: uint64; b: uint64) : urange64

 Multiplies two 64 bit values and returns 128 bit result in form of two 64 bit values (low and high) as urange64.

:Arguments: * **a** : uint64

            * **b** : uint64

.. _function-builtin___bit_set_bitfield_implicit_bitfield_bool:

.. das:function:: __bit_set(value: bitfield& implicit; mask: bitfield; on: bool)

 Sets the specified bit in the bitfield.

:Arguments: * **value** : bitfield<>& implicit

            * **mask** : bitfield<>

            * **on** : bool

.. _function-builtin___bit_set_bitfield8_c_uint8_ls__gr__implicit_bitfield8_c_uint8_ls__gr__bool:

.. das:function:: __bit_set(value: bitfield8:uint8<>& implicit; mask: bitfield8:uint8<>; on: bool)

// stub
def def public __bit_set(var value:bitfield8:uint8<>& implicit; mask:bitfield8:uint8<> const; on:bool const



:Arguments: * **value** : bitfield : uint8<>& implicit

            * **mask** : bitfield : uint8<>

            * **on** : bool

.. _function-builtin___bit_set_bitfield16_c_uint16_ls__gr__implicit_bitfield16_c_uint16_ls__gr__bool:

.. das:function:: __bit_set(value: bitfield16:uint16<>& implicit; mask: bitfield16:uint16<>; on: bool)

// stub
def def public __bit_set(var value:bitfield16:uint16<>& implicit; mask:bitfield16:uint16<> const; on:bool const



:Arguments: * **value** : bitfield : uint16<>& implicit

            * **mask** : bitfield : uint16<>

            * **on** : bool

.. _function-builtin___bit_set_bitfield64_c_uint64_ls__gr__implicit_bitfield64_c_uint64_ls__gr__bool:

.. das:function:: __bit_set(value: bitfield64:uint64<>& implicit; mask: bitfield64:uint64<>; on: bool)

// stub
def def public __bit_set(var value:bitfield64:uint64<>& implicit; mask:bitfield64:uint64<> const; on:bool const



:Arguments: * **value** : bitfield : uint64<>& implicit

            * **mask** : bitfield : uint64<>

            * **on** : bool

+++++++++
Intervals
+++++++++

  *  :ref:`interval (arg0: int; arg1: int) : range <function-builtin_interval_int_int>` 
  *  :ref:`interval (arg0: uint; arg1: uint) : urange <function-builtin_interval_uint_uint>` 
  *  :ref:`interval (arg0: int64; arg1: int64) : range64 <function-builtin_interval_int64_int64>` 
  *  :ref:`interval (arg0: uint64; arg1: uint64) : urange64 <function-builtin_interval_uint64_uint64>` 

.. _function-builtin_interval_int_int:

.. das:function:: interval(arg0: int; arg1: int) : range

 returns range('arg0','arg1')

:Arguments: * **arg0** : int

            * **arg1** : int

.. _function-builtin_interval_uint_uint:

.. das:function:: interval(arg0: uint; arg1: uint) : urange

 returns range('arg0','arg1')

:Arguments: * **arg0** : uint

            * **arg1** : uint

.. _function-builtin_interval_int64_int64:

.. das:function:: interval(arg0: int64; arg1: int64) : range64

 returns range('arg0','arg1')

:Arguments: * **arg0** : int64

            * **arg1** : int64

.. _function-builtin_interval_uint64_uint64:

.. das:function:: interval(arg0: uint64; arg1: uint64) : urange64

 returns range('arg0','arg1')

:Arguments: * **arg0** : uint64

            * **arg1** : uint64

++++
RTTI
++++

  *  :ref:`class_rtti_size (ptr: void? implicit) : int <function-builtin_class_rtti_size_void_q__implicit>` 

.. _function-builtin_class_rtti_size_void_q__implicit:

.. das:function:: class_rtti_size(ptr: void? implicit) : int

 returns size of specific TypeInfo for the class

:Arguments: * **ptr** : void? implicit

+++++++++++++++++
Lock verification
+++++++++++++++++

  *  :ref:`set_verify_context_locks (check: bool) : bool <function-builtin_set_verify_context_locks_bool>` 

.. _function-builtin_set_verify_context_locks_bool:

.. das:function:: set_verify_context_locks(check: bool) : bool

.. warning:: 
  This is unsafe operation.

 Enables or disables array or table lock runtime verification per context

:Arguments: * **check** : bool

+++++++++++++++++++++++++++++++
Initialization and finalization
+++++++++++++++++++++++++++++++

  *  :ref:`using (arg0: block\<(das_string):void\>) <function-builtin_using_block_ls_das_string_c_void_gr_>` 

.. _function-builtin_using_block_ls_das_string_c_void_gr_:

.. das:function:: using(arg0: block<(das_string):void>)

 Creates temporary das_string.

:Arguments: * **arg0** : block<( :ref:`das_string <handle-builtin-das_string>` ):void> implicit

++++++++++
Algorithms
++++++++++

  *  :ref:`count (start: int = 0; step: int = 1) : iterator\<int\> <function-builtin_count_int_int>` 
  *  :ref:`ucount (start: uint = 0x0; step: uint = 0x1) : iterator\<uint\> <function-builtin_ucount_uint_uint>` 
  *  :ref:`iter_range (foo: auto) : auto <function-builtin_iter_range_auto>` 
  *  :ref:`swap (var a: auto(TT)&; var b: auto(TT)&) : auto <function-builtin_swap_autoTT_autoTT>` 

.. _function-builtin_count_int_int:

.. das:function:: count(start: int = 0; step: int = 1) : iterator<int>

 returns iterator which iterates from `start` value by incrementing it by `step` value. It is the intended way to have counter together with other values in the `for` loop.

:Arguments: * **start** : int

            * **step** : int

.. _function-builtin_ucount_uint_uint:

.. das:function:: ucount(start: uint = 0x0; step: uint = 0x1) : iterator<uint>

 returns iterator which iterates from `start` value by incrementing it by `step` value. It is the intended way to have counter together with other values in the `for` loop.

:Arguments: * **start** : uint

            * **step** : uint

.. _function-builtin_iter_range_auto:

.. das:function:: iter_range(foo: auto) : auto

 returns range(`foo`)

:Arguments: * **foo** : auto

.. _function-builtin_swap_autoTT_autoTT:

.. das:function:: swap(a: auto(TT)&; b: auto(TT)&) : auto

 swaps two values `a` and 'b'

:Arguments: * **a** : auto(TT)&

            * **b** : auto(TT)&

++++++
Memset
++++++

  *  :ref:`memset8 (left: void? implicit; value: uint8; count: int) <function-builtin_memset8_void_q__implicit_uint8_int>` 
  *  :ref:`memset16 (left: void? implicit; value: uint16; count: int) <function-builtin_memset16_void_q__implicit_uint16_int>` 
  *  :ref:`memset32 (left: void? implicit; value: uint; count: int) <function-builtin_memset32_void_q__implicit_uint_int>` 
  *  :ref:`memset64 (left: void? implicit; value: uint64; count: int) <function-builtin_memset64_void_q__implicit_uint64_int>` 
  *  :ref:`memset128 (left: void? implicit; value: uint4; count: int) <function-builtin_memset128_void_q__implicit_uint4_int>` 

.. _function-builtin_memset8_void_q__implicit_uint8_int:

.. das:function:: memset8(left: void? implicit; value: uint8; count: int)

.. warning:: 
  This is unsafe operation.

 Effecitvely C memset.

:Arguments: * **left** : void? implicit

            * **value** : uint8

            * **count** : int

.. _function-builtin_memset16_void_q__implicit_uint16_int:

.. das:function:: memset16(left: void? implicit; value: uint16; count: int)

.. warning:: 
  This is unsafe operation.

 Similar to memset, but fills values with 16 bit words.

:Arguments: * **left** : void? implicit

            * **value** : uint16

            * **count** : int

.. _function-builtin_memset32_void_q__implicit_uint_int:

.. das:function:: memset32(left: void? implicit; value: uint; count: int)

.. warning:: 
  This is unsafe operation.

 Similar to memset, but fills values with 32 bit words.

:Arguments: * **left** : void? implicit

            * **value** : uint

            * **count** : int

.. _function-builtin_memset64_void_q__implicit_uint64_int:

.. das:function:: memset64(left: void? implicit; value: uint64; count: int)

.. warning:: 
  This is unsafe operation.

 Similar to memset, but fills values with 64 bit words.

:Arguments: * **left** : void? implicit

            * **value** : uint64

            * **count** : int

.. _function-builtin_memset128_void_q__implicit_uint4_int:

.. das:function:: memset128(left: void? implicit; value: uint4; count: int)

.. warning:: 
  This is unsafe operation.

 Similar to memset, but fills values with 128 bit vector type values.

:Arguments: * **left** : void? implicit

            * **value** : uint4

            * **count** : int

++++++
Malloc
++++++

  *  :ref:`malloc (size: uint64) : void? <function-builtin_malloc_uint64>` 
  *  :ref:`free (ptr: void? implicit) <function-builtin_free_void_q__implicit>` 
  *  :ref:`malloc_usable_size (ptr: void? implicit) : uint64 <function-builtin_malloc_usable_size_void_q__implicit>` 

.. _function-builtin_malloc_uint64:

.. das:function:: malloc(size: uint64) : void?

.. warning:: 
  This is unsafe operation.

 C-style malloc

:Arguments: * **size** : uint64

.. _function-builtin_free_void_q__implicit:

.. das:function:: free(ptr: void? implicit)

.. warning:: 
  This is unsafe operation.

 C-style free to be coupled with C-style malloc

:Arguments: * **ptr** : void? implicit

.. _function-builtin_malloc_usable_size_void_q__implicit:

.. das:function:: malloc_usable_size(ptr: void? implicit) : uint64

.. warning:: 
  This is unsafe operation.

 returns size of the allocated memory block

:Arguments: * **ptr** : void? implicit

+++++++++++++++++++
Compilation and AOT
+++++++++++++++++++

  *  :ref:`set_aot () <function-builtin_set_aot>` 
  *  :ref:`reset_aot () <function-builtin_reset_aot>` 
  *  :ref:`compiling_file_name () : string <function-builtin_compiling_file_name>` 
  *  :ref:`compiling_module_name () : string <function-builtin_compiling_module_name>` 

.. _function-builtin_set_aot:

.. das:function:: set_aot()

 Notifies compiler that AOT is being generated.

.. _function-builtin_reset_aot:

.. das:function:: reset_aot()

 Notifies compiler that AOT is no longer being generated.

.. _function-builtin_compiling_file_name:

.. das:function:: compiling_file_name() : string

 returns name of the file currently being compiled.

.. _function-builtin_compiling_module_name:

.. das:function:: compiling_module_name() : string

 returns name of the module currently being compiled.


