
.. _stdlib__builtin:

================
Built-in runtime
================

.. include:: detail/builtin.rst

Builtin module is automatically required by any other das file. It includes basic language infrastructure,
support for containers, heap, miscellaneous iterators, profiler, and interaction with host application.

++++++++++++
Type aliases
++++++++++++

.. _alias-print_flags:

.. das:attribute:: print_flags is a bitfield

+------------------+---+-----+
+field             +bit+value+
+==================+===+=====+
+escapeString      +0  +1    +
+------------------+---+-----+
+namesAndDimensions+1  +2    +
+------------------+---+-----+
+typeQualifiers    +2  +4    +
+------------------+---+-----+
+refAddresses      +3  +8    +
+------------------+---+-----+
+singleLine        +4  +16   +
+------------------+---+-----+
+fixedPoint        +5  +32   +
+------------------+---+-----+


|typedef-builtin-print_flags|

+++++++++
Constants
+++++++++

.. _global-builtin-DAS_MAX_FUNCTION_ARGUMENTS:

.. das:attribute:: DAS_MAX_FUNCTION_ARGUMENTS = 32

|variable-builtin-DAS_MAX_FUNCTION_ARGUMENTS|

.. _global-builtin-INT_MIN:

.. das:attribute:: INT_MIN = -2147483648

|variable-builtin-INT_MIN|

.. _global-builtin-INT_MAX:

.. das:attribute:: INT_MAX = 2147483647

|variable-builtin-INT_MAX|

.. _global-builtin-UINT_MAX:

.. das:attribute:: UINT_MAX = 0xffffffff

|variable-builtin-UINT_MAX|

.. _global-builtin-LONG_MIN:

.. das:attribute:: LONG_MIN = -9223372036854775808

|variable-builtin-LONG_MIN|

.. _global-builtin-LONG_MAX:

.. das:attribute:: LONG_MAX = 9223372036854775807

|variable-builtin-LONG_MAX|

.. _global-builtin-ULONG_MAX:

.. das:attribute:: ULONG_MAX = 0xffffffffffffffff

|variable-builtin-ULONG_MAX|

.. _global-builtin-FLT_MIN:

.. das:attribute:: FLT_MIN = 1.1754944e-38f

|variable-builtin-FLT_MIN|

.. _global-builtin-FLT_MAX:

.. das:attribute:: FLT_MAX = 3.4028235e+38f

|variable-builtin-FLT_MAX|

.. _global-builtin-DBL_MIN:

.. das:attribute:: DBL_MIN = 2.2250738585072014e-308lf

|variable-builtin-DBL_MIN|

.. _global-builtin-DBL_MAX:

.. das:attribute:: DBL_MAX = 1.7976931348623157e+308lf

|variable-builtin-DBL_MAX|

.. _global-builtin-LOG_CRITICAL:

.. das:attribute:: LOG_CRITICAL = 50000

|variable-builtin-LOG_CRITICAL|

.. _global-builtin-LOG_ERROR:

.. das:attribute:: LOG_ERROR = 40000

|variable-builtin-LOG_ERROR|

.. _global-builtin-LOG_WARNING:

.. das:attribute:: LOG_WARNING = 30000

|variable-builtin-LOG_WARNING|

.. _global-builtin-LOG_INFO:

.. das:attribute:: LOG_INFO = 20000

|variable-builtin-LOG_INFO|

.. _global-builtin-LOG_DEBUG:

.. das:attribute:: LOG_DEBUG = 10000

|variable-builtin-LOG_DEBUG|

.. _global-builtin-LOG_TRACE:

.. das:attribute:: LOG_TRACE = 0

|variable-builtin-LOG_TRACE|

.. _global-builtin-VEC_SEP:

.. das:attribute:: VEC_SEP = ","

|variable-builtin-VEC_SEP|

.. _global-builtin-print_flags_debugger:

.. das:attribute:: print_flags_debugger = bitfield(0xf)

|variable-builtin-print_flags_debugger|

++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-builtin-HashBuilder:

.. das:attribute:: HashBuilder

|structure_annotation-builtin-HashBuilder|

++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-builtin-marker:

.. das:attribute:: marker

|function_annotation-builtin-marker|

.. _handle-builtin-generic:

.. das:attribute:: generic

|function_annotation-builtin-generic|

.. _handle-builtin-_macro:

.. das:attribute:: _macro

|function_annotation-builtin-_macro|

.. _handle-builtin-macro_function:

.. das:attribute:: macro_function

|function_annotation-builtin-macro_function|

.. _handle-builtin-hint:

.. das:attribute:: hint

|function_annotation-builtin-hint|

.. _handle-builtin-jit:

.. das:attribute:: jit

|function_annotation-builtin-jit|

.. _handle-builtin-no_jit:

.. das:attribute:: no_jit

|function_annotation-builtin-no_jit|

.. _handle-builtin-nodiscard:

.. das:attribute:: nodiscard

|function_annotation-builtin-nodiscard|

.. _handle-builtin-deprecated:

.. das:attribute:: deprecated

|function_annotation-builtin-deprecated|

.. _handle-builtin-alias_cmres:

.. das:attribute:: alias_cmres

|function_annotation-builtin-alias_cmres|

.. _handle-builtin-never_alias_cmres:

.. das:attribute:: never_alias_cmres

|function_annotation-builtin-never_alias_cmres|

.. _handle-builtin-export:

.. das:attribute:: export

|function_annotation-builtin-export|

.. _handle-builtin-pinvoke:

.. das:attribute:: pinvoke

|function_annotation-builtin-pinvoke|

.. _handle-builtin-no_lint:

.. das:attribute:: no_lint

|function_annotation-builtin-no_lint|

.. _handle-builtin-sideeffects:

.. das:attribute:: sideeffects

|function_annotation-builtin-sideeffects|

.. _handle-builtin-run:

.. das:attribute:: run

|function_annotation-builtin-run|

.. _handle-builtin-unsafe_operation:

.. das:attribute:: unsafe_operation

|function_annotation-builtin-unsafe_operation|

.. _handle-builtin-unsafe_outside_of_for:

.. das:attribute:: unsafe_outside_of_for

|function_annotation-builtin-unsafe_outside_of_for|

.. _handle-builtin-unsafe_when_not_clone_array:

.. das:attribute:: unsafe_when_not_clone_array

|function_annotation-builtin-unsafe_when_not_clone_array|

.. _handle-builtin-no_aot:

.. das:attribute:: no_aot

|function_annotation-builtin-no_aot|

.. _handle-builtin-init:

.. das:attribute:: init

|function_annotation-builtin-init|

.. _handle-builtin-finalize:

.. das:attribute:: finalize

|function_annotation-builtin-finalize|

.. _handle-builtin-hybrid:

.. das:attribute:: hybrid

|function_annotation-builtin-hybrid|

.. _handle-builtin-unsafe_deref:

.. das:attribute:: unsafe_deref

|function_annotation-builtin-unsafe_deref|

.. _handle-builtin-skip_lock_check:

.. das:attribute:: skip_lock_check

|function_annotation-builtin-skip_lock_check|

.. _handle-builtin-unused_argument:

.. das:attribute:: unused_argument

|function_annotation-builtin-unused_argument|

.. _handle-builtin-local_only:

.. das:attribute:: local_only

|function_annotation-builtin-local_only|

.. _handle-builtin-expect_any_vector:

.. das:attribute:: expect_any_vector

|function_annotation-builtin-expect_any_vector|

.. _handle-builtin-expect_dim:

.. das:attribute:: expect_dim

|function_annotation-builtin-expect_dim|

.. _handle-builtin-expect_ref:

.. das:attribute:: expect_ref

|function_annotation-builtin-expect_ref|

.. _handle-builtin-type_function:

.. das:attribute:: type_function

|function_annotation-builtin-type_function|

.. _handle-builtin-builtin_array_sort:

.. das:attribute:: builtin_array_sort

|function_annotation-builtin-builtin_array_sort|

+++++++++++
Call macros
+++++++++++

.. _call-macro-builtin-make_function_unsafe:

.. das:attribute:: make_function_unsafe

|function_annotation-builtin-make_function_unsafe|

.. _call-macro-builtin-concept_assert:

.. das:attribute:: concept_assert

|function_annotation-builtin-concept_assert|

.. _call-macro-builtin-__builtin_table_set_insert:

.. das:attribute:: __builtin_table_set_insert

|function_annotation-builtin-__builtin_table_set_insert|

.. _call-macro-builtin-__builtin_table_key_exists:

.. das:attribute:: __builtin_table_key_exists

|function_annotation-builtin-__builtin_table_key_exists|

.. _call-macro-builtin-static_assert:

.. das:attribute:: static_assert

|function_annotation-builtin-static_assert|

.. _call-macro-builtin-verify:

.. das:attribute:: verify

|function_annotation-builtin-verify|

.. _call-macro-builtin-debug:

.. das:attribute:: debug

|function_annotation-builtin-debug|

.. _call-macro-builtin-assert:

.. das:attribute:: assert

|function_annotation-builtin-assert|

.. _call-macro-builtin-memzero:

.. das:attribute:: memzero

|function_annotation-builtin-memzero|

.. _call-macro-builtin-__builtin_table_find:

.. das:attribute:: __builtin_table_find

|function_annotation-builtin-__builtin_table_find|

.. _call-macro-builtin-invoke:

.. das:attribute:: invoke

|function_annotation-builtin-invoke|

.. _call-macro-builtin-__builtin_table_erase:

.. das:attribute:: __builtin_table_erase

|function_annotation-builtin-__builtin_table_erase|

+++++++++++++
Reader macros
+++++++++++++

.. _call-macro-builtin-_esc:

.. das:attribute:: _esc

|reader_macro-builtin-_esc|

+++++++++++++++
Typeinfo macros
+++++++++++++++

.. _call-macro-builtin-rtti_classinfo:

.. das:attribute:: rtti_classinfo

|typeinfo_macro-builtin-rtti_classinfo|

+++++++++++++
Handled types
+++++++++++++

.. _handle-builtin-das_string:

.. das:attribute:: das_string

|any_annotation-builtin-das_string|

.. _handle-builtin-clock:

.. das:attribute:: clock

|any_annotation-builtin-clock|

++++++++++++++++
Structure macros
++++++++++++++++

.. _handle-builtin-comment:

.. das:attribute:: comment

|structure_macro-builtin-comment|

.. _handle-builtin-no_default_initializer:

.. das:attribute:: no_default_initializer

|structure_macro-builtin-no_default_initializer|

.. _handle-builtin-macro_interface:

.. das:attribute:: macro_interface

|structure_macro-builtin-macro_interface|

.. _handle-builtin-skip_field_lock_check:

.. das:attribute:: skip_field_lock_check

|structure_macro-builtin-skip_field_lock_check|

.. _handle-builtin-cpp_layout:

.. das:attribute:: cpp_layout

|structure_macro-builtin-cpp_layout|

.. _handle-builtin-safe_when_uninitialized:

.. das:attribute:: safe_when_uninitialized

|structure_macro-builtin-safe_when_uninitialized|

.. _handle-builtin-persistent:

.. das:attribute:: persistent

|structure_macro-builtin-persistent|

++++++++++
Containers
++++++++++

  *  :ref:`clear (array:array implicit;context:__context const;at:__lineInfo const) : void <function-_at__builtin__c__c_clear_IA_C_c_C_l>` 
  *  :ref:`length (array:array const implicit) : int const <function-_at__builtin__c__c_length_CIA>` 
  *  :ref:`capacity (array:array const implicit) : int const <function-_at__builtin__c__c_capacity_CIA>` 
  *  :ref:`empty (iterator:iterator const implicit) : bool const <function-_at__builtin__c__c_empty_CIG>` 
  *  :ref:`length (table:table const implicit) : int const <function-_at__builtin__c__c_length_CIT>` 
  *  :ref:`capacity (table:table const implicit) : int const <function-_at__builtin__c__c_capacity_CIT>` 
  *  :ref:`empty (str:string const implicit) : bool const <function-_at__builtin__c__c_empty_CIs>` 
  *  :ref:`empty (str:$::das_string const implicit) : bool const <function-_at__builtin__c__c_empty_CIH_ls__builtin__c__c_das_string_gr_>` 
  *  :ref:`resize (Arr:array\<auto(numT)\> -const;newSize:int const) : auto <function-_at__builtin__c__c_resize_1_ls_Y_ls_numT_gr_._gr_A_Ci>` 
  *  :ref:`resize_and_init (Arr:array\<auto(numT)\> -const;newSize:int const) : auto <function-_at__builtin__c__c_resize_and_init_1_ls_Y_ls_numT_gr_._gr_A_Ci>` 
  *  :ref:`resize_and_init (Arr:array\<auto(numT)\> -const;newSize:int const;initValue:numT const) : auto <function-_at__builtin__c__c_resize_and_init_1_ls_Y_ls_numT_gr_._gr_A_Ci_CY_ls_numT_gr_L>` 
  *  :ref:`resize_no_init (Arr:array\<auto(numT)\> -const;newSize:int const) : auto <function-_at__builtin__c__c_resize_no_init_1_ls_Y_ls_numT_gr_._gr_A_Ci>` 
  *  :ref:`reserve (Arr:array\<auto(numT)\> -const;newSize:int const) : auto <function-_at__builtin__c__c_reserve_1_ls_Y_ls_numT_gr_._gr_A_Ci>` 
  *  :ref:`pop (Arr:array\<auto(numT)\> -const) : auto <function-_at__builtin__c__c_pop_1_ls_Y_ls_numT_gr_._gr_A>` 
  *  :ref:`push (Arr:array\<auto(numT)\> -const;value:numT const ==const -#;at:int const) : auto <function-_at__builtin__c__c_push_1_ls_Y_ls_numT_gr_._gr_A_C_eq_Y_ls_numT_gr_L_Ci>` 
  *  :ref:`push (Arr:array\<auto(numT)\> -const;value:numT ==const -const -#;at:int const) : auto <function-_at__builtin__c__c_push_1_ls_Y_ls_numT_gr_._gr_A__eq_Y_ls_numT_gr_L_Ci>` 
  *  :ref:`push (Arr:array\<auto(numT)\> -const;value:numT const ==const -#) : auto <function-_at__builtin__c__c_push_1_ls_Y_ls_numT_gr_._gr_A_C_eq_Y_ls_numT_gr_L>` 
  *  :ref:`push (Arr:array\<auto(numT)\> -const;value:numT ==const -const -#) : auto <function-_at__builtin__c__c_push_1_ls_Y_ls_numT_gr_._gr_A__eq_Y_ls_numT_gr_L>` 
  *  :ref:`push (Arr:array\<auto(numT)\> -const;varr:array\<numT -#\> const ==const) : auto <function-_at__builtin__c__c_push_1_ls_Y_ls_numT_gr_._gr_A_C_eq_1_ls_Y_ls_numT_gr_L_gr_A>` 
  *  :ref:`push (Arr:array\<auto(numT)\> -const;varr:array\<numT -#\> ==const -const) : auto <function-_at__builtin__c__c_push_1_ls_Y_ls_numT_gr_._gr_A__eq_1_ls_Y_ls_numT_gr_L_gr_A>` 
  *  :ref:`push (Arr:array\<auto(numT)\> -const;varr:numT const[] -#) : auto <function-_at__builtin__c__c_push_1_ls_Y_ls_numT_gr_._gr_A_C_lb_-1_rb_Y_ls_numT_gr_L>` 
  *  :ref:`push (Arr:array\<auto(numT)[]\> -const;varr:numT const[] ==const -#) : auto <function-_at__builtin__c__c_push_1_ls__lb_-1_rb_Y_ls_numT_gr_._gr_A_C_eq__lb_-1_rb_Y_ls_numT_gr_L>` 
  *  :ref:`push (Arr:array\<auto(numT)[]\> -const;varr:numT[] ==const -const -#) : auto <function-_at__builtin__c__c_push_1_ls__lb_-1_rb_Y_ls_numT_gr_._gr_A__eq__lb_-1_rb_Y_ls_numT_gr_L>` 
  *  :ref:`emplace (Arr:array\<auto(numT)\> -const;value:numT& -const -#;at:int const) : auto <function-_at__builtin__c__c_emplace_1_ls_Y_ls_numT_gr_._gr_A_&Y_ls_numT_gr_L_Ci>` 
  *  :ref:`emplace (Arr:array\<auto(numT)\> -const;value:numT& -const -#) : auto <function-_at__builtin__c__c_emplace_1_ls_Y_ls_numT_gr_._gr_A_&Y_ls_numT_gr_L>` 
  *  :ref:`emplace (Arr:array\<auto(numT)\> -const;value:numT[] -const -#) : auto <function-_at__builtin__c__c_emplace_1_ls_Y_ls_numT_gr_._gr_A__lb_-1_rb_Y_ls_numT_gr_L>` 
  *  :ref:`emplace (Arr:array\<auto(numT)[]\> -const;value:numT[] -const -#) : auto <function-_at__builtin__c__c_emplace_1_ls__lb_-1_rb_Y_ls_numT_gr_._gr_A__lb_-1_rb_Y_ls_numT_gr_L>` 
  *  :ref:`push_clone (Arr:array\<auto(numT)\> -const;value:numT const ==const|numT const# ==const const;at:int const) : auto <function-_at__builtin__c__c_push_clone_1_ls_Y_ls_numT_gr_._gr_A_C0_ls_C_eq_Y_ls_numT_gr_L;C_hh__eq_Y_ls_numT_gr_L_gr_|_Ci>` 
  *  :ref:`push_clone (Arr:array\<auto(numT)\> -const;value:numT ==const -const|numT# ==const -const -const;at:int const) : auto <function-_at__builtin__c__c_push_clone_1_ls_Y_ls_numT_gr_._gr_A_0_ls__eq_Y_ls_numT_gr_L;_hh__eq_Y_ls_numT_gr_L_gr_|_Ci>` 
  *  :ref:`push_clone (Arr:array\<auto(numT)\> -const;value:numT const ==const|numT const# ==const const) : auto <function-_at__builtin__c__c_push_clone_1_ls_Y_ls_numT_gr_._gr_A_C0_ls_C_eq_Y_ls_numT_gr_L;C_hh__eq_Y_ls_numT_gr_L_gr_|>` 
  *  :ref:`push_clone (Arr:array\<auto(numT)\> -const;value:numT ==const -const|numT# ==const -const -const) : auto <function-_at__builtin__c__c_push_clone_1_ls_Y_ls_numT_gr_._gr_A_0_ls__eq_Y_ls_numT_gr_L;_hh__eq_Y_ls_numT_gr_L_gr_|>` 
  *  :ref:`push_clone (Arr:array\<auto(numT)\> -const;varr:numT const[] ==const) : auto <function-_at__builtin__c__c_push_clone_1_ls_Y_ls_numT_gr_._gr_A_C_eq__lb_-1_rb_Y_ls_numT_gr_L>` 
  *  :ref:`push_clone (Arr:array\<auto(numT)\> -const;varr:numT[] ==const -const) : auto <function-_at__builtin__c__c_push_clone_1_ls_Y_ls_numT_gr_._gr_A__eq__lb_-1_rb_Y_ls_numT_gr_L>` 
  *  :ref:`push_clone (Arr:array\<auto(numT)[]\> -const;varr:numT const[] ==const) : auto <function-_at__builtin__c__c_push_clone_1_ls__lb_-1_rb_Y_ls_numT_gr_._gr_A_C_eq__lb_-1_rb_Y_ls_numT_gr_L>` 
  *  :ref:`push_clone (Arr:array\<auto(numT)[]\> -const;varr:numT[] -const) : auto <function-_at__builtin__c__c_push_clone_1_ls__lb_-1_rb_Y_ls_numT_gr_._gr_A__lb_-1_rb_Y_ls_numT_gr_L>` 
  *  :ref:`push_clone (A:auto(CT) -const -#;b:auto(TT) const|auto(TT) const# const) : auto <function-_at__builtin__c__c_push_clone_Y_ls_CT_gr_._C0_ls_CY_ls_TT_gr_.;C_hh_Y_ls_TT_gr_._gr_|>` 
  *  :ref:`back (a:array\<auto(TT)\> ==const -const) : TT& <function-_at__builtin__c__c_back__eq_1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`back (a:array\<auto(TT)\># ==const -const) : TT&# <function-_at__builtin__c__c_back__hh__eq_1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`back (a:array\<auto(TT)\> const ==const) : TT const& <function-_at__builtin__c__c_back_C_eq_1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`back (a:array\<auto(TT)\> const# ==const) : TT const&# <function-_at__builtin__c__c_back_C_hh__eq_1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`back (arr:auto(TT) ==const -const) : auto& <function-_at__builtin__c__c_back__eq_Y_ls_TT_gr_.>` 
  *  :ref:`back (arr:auto(TT) const ==const) : auto const& <function-_at__builtin__c__c_back_C_eq_Y_ls_TT_gr_.>` 
  *  :ref:`erase (Arr:array\<auto(numT)\> -const;at:int const) : auto <function-_at__builtin__c__c_erase_1_ls_Y_ls_numT_gr_._gr_A_Ci>` 
  *  :ref:`erase (Arr:array\<auto(numT)\> -const;at:int const;count:int const) : auto <function-_at__builtin__c__c_erase_1_ls_Y_ls_numT_gr_._gr_A_Ci_Ci>` 
  *  :ref:`erase_if (arr:array\<auto(TT)\> -const;blk:block\<(key:TT const):bool\> const|block\<(var key:TT& -const):bool\> const const) : auto <function-_at__builtin__c__c_erase_if_1_ls_Y_ls_TT_gr_._gr_A_C0_ls_CN_ls_key_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_;CN_ls_key_gr_0_ls_&Y_ls_TT_gr_L_gr_1_ls_b_gr__builtin__gr_|>` 
  *  :ref:`remove_value (arr:array\<auto(TT)\> -const|array\<auto(TT)\># -const -const;key:TT const) : bool <function-_at__builtin__c__c_remove_value_0_ls_1_ls_Y_ls_TT_gr_._gr_A;_hh_1_ls_Y_ls_TT_gr_._gr_A_gr_|_CY_ls_TT_gr_L>` 
  *  :ref:`length (a:auto const|auto const# const) : int <function-_at__builtin__c__c_length_C0_ls_C.;C_hh_._gr_|>` 
  *  :ref:`empty (a:array\<auto\> const|array\<auto\> const# const) : bool <function-_at__builtin__c__c_empty_C0_ls_C1_ls_._gr_A;C_hh_1_ls_._gr_A_gr_|>` 
  *  :ref:`empty (a:table\<auto;auto\> const|table\<auto;auto\> const# const) : bool <function-_at__builtin__c__c_empty_C0_ls_C1_ls_._gr_2_ls_._gr_T;C_hh_1_ls_._gr_2_ls_._gr_T_gr_|>` 
  *  :ref:`reserve (Tab:table\<auto(keyT);auto\> -const;newSize:int const) : auto <function-_at__builtin__c__c_reserve_1_ls_Y_ls_keyT_gr_._gr_2_ls_._gr_T_Ci>` 
  *  :ref:`get (Tab:table\<auto(keyT);auto(valT)\> const# ==const;at:keyT const|keyT const# const;blk:block\<(p:valT const&#):void\> const) : auto <function-_at__builtin__c__c_get_C_hh__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_CN_ls_p_gr_0_ls_C&_hh_Y_ls_valT_gr_L_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`get (Tab:table\<auto(keyT);auto(valT)\> const ==const;at:keyT const|keyT const# const;blk:block\<(p:valT const&):void\> const) : auto <function-_at__builtin__c__c_get_C_eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_CN_ls_p_gr_0_ls_C&Y_ls_valT_gr_L_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`get (Tab:table\<auto(keyT);auto(valT)\># ==const -const;at:keyT const|keyT const# const;blk:block\<(var p:valT&# -const):void\> const) : auto <function-_at__builtin__c__c_get__hh__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_CN_ls_p_gr_0_ls_&_hh_Y_ls_valT_gr_L_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`get (Tab:table\<auto(keyT);auto(valT)\> ==const -const;at:keyT const|keyT const# const;blk:block\<(var p:valT& -const):void\> const) : auto <function-_at__builtin__c__c_get__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_CN_ls_p_gr_0_ls_&Y_ls_valT_gr_L_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`get (Tab:table\<auto(keyT);auto(valT)[]\> const# ==const;at:keyT const|keyT const# const;blk:block\<(p:valT const[-2]&#):void\> const) : auto <function-_at__builtin__c__c_get_C_hh__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls__lb_-1_rb_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_CN_ls_p_gr_0_ls_C&_hh__lb_-2_rb_Y_ls_valT_gr_L_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`get (Tab:table\<auto(keyT);auto(valT)[]\> const ==const;at:keyT const|keyT const# const;blk:block\<(p:valT const[-2]&):void\> const) : auto <function-_at__builtin__c__c_get_C_eq_1_ls_Y_ls_keyT_gr_._gr_2_ls__lb_-1_rb_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_CN_ls_p_gr_0_ls_C&_lb_-2_rb_Y_ls_valT_gr_L_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`get (Tab:table\<auto(keyT);auto(valT)[]\># ==const -const;at:keyT const|keyT const# const;blk:block\<(var p:valT[-2]&# -const):void\> const) : auto <function-_at__builtin__c__c_get__hh__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls__lb_-1_rb_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_CN_ls_p_gr_0_ls_&_hh__lb_-2_rb_Y_ls_valT_gr_L_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`get (Tab:table\<auto(keyT);auto(valT)[]\> ==const -const;at:keyT const|keyT const# const;blk:block\<(var p:valT[-2]& -const):void\> const) : auto <function-_at__builtin__c__c_get__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls__lb_-1_rb_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_CN_ls_p_gr_0_ls_&_lb_-2_rb_Y_ls_valT_gr_L_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`get (Tab:table\<auto(keyT);void\> const;at:keyT const|keyT const# const;blk:block\<(var p:void? -const):void\> const) : auto <function-_at__builtin__c__c_get_C1_ls_Y_ls_keyT_gr_._gr_2_ls_v_gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_CN_ls_p_gr_0_ls_1_ls_v_gr__qm__gr_1_ls_v_gr__builtin_>` 
  *  :ref:`get_value (Tab:table\<auto(keyT);auto(valT)\> const ==const;at:keyT const|keyT const# const) : valT <function-_at__builtin__c__c_get_value_C_eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|>` 
  *  :ref:`get_value (Tab:table\<auto(keyT);smart_ptr\<auto(valT)\>\> -const;at:keyT const|keyT const# const) : smart_ptr\<valT\> <function-_at__builtin__c__c_get_value_1_ls_Y_ls_keyT_gr_._gr_2_ls_1_ls_Y_ls_valT_gr_._gr__qm_M_gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|>` 
  *  :ref:`get_value (Tab:table\<auto(keyT);auto(valT)\> -const;at:keyT const|keyT const# const) : valT <function-_at__builtin__c__c_get_value_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|>` 
  *  :ref:`get_value (Tab:table\<auto(keyT);auto(valT)[]\> -const;at:keyT const|keyT const# const) : valT[-2] <function-_at__builtin__c__c_get_value_1_ls_Y_ls_keyT_gr_._gr_2_ls__lb_-1_rb_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|>` 
  *  :ref:`erase (Tab:table\<auto(keyT);auto(valT)\> -const;at:string const#) : bool <function-_at__builtin__c__c_erase_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C_hh_s>` 
  *  :ref:`erase (Tab:table\<auto(keyT);auto(valT)\> -const;at:keyT const|keyT const# const) : bool <function-_at__builtin__c__c_erase_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|>` 
  *  :ref:`insert (Tab:table\<auto(keyT);void\> -const;at:keyT const|keyT const# const) : auto <function-_at__builtin__c__c_insert_1_ls_Y_ls_keyT_gr_._gr_2_ls_v_gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|>` 
  *  :ref:`insert_clone (Tab:table\<auto(keyT);auto(valT)\> -const;at:keyT const|keyT const# const;val:valT ==const -const|valT# ==const -const -const) : auto <function-_at__builtin__c__c_insert_clone_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_0_ls__eq_Y_ls_valT_gr_L;_hh__eq_Y_ls_valT_gr_L_gr_|>` 
  *  :ref:`insert_clone (Tab:table\<auto(keyT);auto(valT)\> -const;at:keyT const|keyT const# const;val:valT const ==const|valT const# ==const const) : auto <function-_at__builtin__c__c_insert_clone_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_C0_ls_C_eq_Y_ls_valT_gr_L;C_hh__eq_Y_ls_valT_gr_L_gr_|>` 
  *  :ref:`insert_clone (Tab:table\<auto(keyT);auto(valT)[]\> -const;at:keyT const|keyT const# const;val:valT[] ==const -const|valT[]# ==const -const -const) : auto <function-_at__builtin__c__c_insert_clone_1_ls_Y_ls_keyT_gr_._gr_2_ls__lb_-1_rb_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_0_ls__eq__lb_-1_rb_Y_ls_valT_gr_L;_hh__eq__lb_-1_rb_Y_ls_valT_gr_L_gr_|>` 
  *  :ref:`insert_clone (Tab:table\<auto(keyT);auto(valT)[]\> -const;at:keyT const|keyT const# const;val:valT const[] ==const|valT const[]# ==const const) : auto <function-_at__builtin__c__c_insert_clone_1_ls_Y_ls_keyT_gr_._gr_2_ls__lb_-1_rb_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_C0_ls_C_eq__lb_-1_rb_Y_ls_valT_gr_L;C_hh__eq__lb_-1_rb_Y_ls_valT_gr_L_gr_|>` 
  *  :ref:`insert (Tab:table\<auto(keyT);auto(valT)\> -const;at:keyT const|keyT const# const;val:valT ==const -const|valT# ==const -const -const) : auto <function-_at__builtin__c__c_insert_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_0_ls__eq_Y_ls_valT_gr_L;_hh__eq_Y_ls_valT_gr_L_gr_|>` 
  *  :ref:`insert (Tab:table\<auto(keyT);auto(valT)\> -const;at:keyT const|keyT const# const;val:valT const ==const|valT const# ==const const) : auto <function-_at__builtin__c__c_insert_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_C0_ls_C_eq_Y_ls_valT_gr_L;C_hh__eq_Y_ls_valT_gr_L_gr_|>` 
  *  :ref:`insert (Tab:table\<auto(keyT);auto(valT)[]\> -const;at:keyT const|keyT const# const;val:valT[] ==const -const|valT[]# ==const -const -const) : auto <function-_at__builtin__c__c_insert_1_ls_Y_ls_keyT_gr_._gr_2_ls__lb_-1_rb_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_0_ls__eq__lb_-1_rb_Y_ls_valT_gr_L;_hh__eq__lb_-1_rb_Y_ls_valT_gr_L_gr_|>` 
  *  :ref:`insert (Tab:table\<auto(keyT);auto(valT)[]\> -const;at:keyT const|keyT const# const;val:valT const[] ==const|valT const[]# ==const const) : auto <function-_at__builtin__c__c_insert_1_ls_Y_ls_keyT_gr_._gr_2_ls__lb_-1_rb_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_C0_ls_C_eq__lb_-1_rb_Y_ls_valT_gr_L;C_hh__eq__lb_-1_rb_Y_ls_valT_gr_L_gr_|>` 
  *  :ref:`emplace (Tab:table\<auto(keyT);auto(valT)\> -const;at:keyT const|keyT const# const;val:valT& -const -#) : auto <function-_at__builtin__c__c_emplace_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_&Y_ls_valT_gr_L>` 
  *  :ref:`emplace (Tab:table\<auto(keyT);smart_ptr\<auto(valT)\>\> -const;at:keyT const|keyT const# const;val:smart_ptr\<valT\>& -const) : auto <function-_at__builtin__c__c_emplace_1_ls_Y_ls_keyT_gr_._gr_2_ls_1_ls_Y_ls_valT_gr_._gr__qm_M_gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_&1_ls_Y_ls_valT_gr_L_gr__qm_M>` 
  *  :ref:`emplace (Tab:table\<auto(keyT);auto(valT)[]\> -const;at:keyT const|keyT const# const;val:valT[]& -const -#) : auto <function-_at__builtin__c__c_emplace_1_ls_Y_ls_keyT_gr_._gr_2_ls__lb_-1_rb_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_&_lb_-1_rb_Y_ls_valT_gr_L>` 
  *  :ref:`emplace_new (tab:table\<auto(kT);smart_ptr\<auto(vT)\>\> -const;key:kT const;value:smart_ptr\<vT\> -const) : auto <function-_at__builtin__c__c_emplace_new_1_ls_Y_ls_kT_gr_._gr_2_ls_1_ls_Y_ls_vT_gr_._gr__qm_M_gr_T_CY_ls_kT_gr_L_1_ls_Y_ls_vT_gr_L_gr__qm_M>` 
  *  :ref:`emplace (Tab:table\<auto;auto\> -const;key:auto const;value:auto const) : auto <function-_at__builtin__c__c_emplace_1_ls_._gr_2_ls_._gr_T_C._C._%_ls__ex_()_gr_>` 
  *  :ref:`emplace (a:array\<auto\> -const;value:auto const) : auto <function-_at__builtin__c__c_emplace_1_ls_._gr_A_C._%_ls__ex_()_gr_>` 
  *  :ref:`emplace_new (Arr:array\<smart_ptr\<auto(numT)\>\> -const;value:smart_ptr\<numT\> -const) : auto <function-_at__builtin__c__c_emplace_new_1_ls_1_ls_Y_ls_numT_gr_._gr__qm_M_gr_A_1_ls_Y_ls_numT_gr_L_gr__qm_M>` 
  *  :ref:`insert_default (tab:table\<auto(TT);auto(QQ)\> -const;key:TT const|TT const# const;value:QQ ==const -const|QQ# ==const -const -const) : void <function-_at__builtin__c__c_insert_default_1_ls_Y_ls_TT_gr_._gr_2_ls_Y_ls_QQ_gr_._gr_T_C0_ls_CY_ls_TT_gr_L;C_hh_Y_ls_TT_gr_L_gr_|_0_ls__eq_Y_ls_QQ_gr_L;_hh__eq_Y_ls_QQ_gr_L_gr_|>` 
  *  :ref:`insert_default (tab:table\<auto(keyT);auto(valT)\> -const;key:keyT const|keyT const# const;value:valT const ==const|valT const# ==const const) : void <function-_at__builtin__c__c_insert_default_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_C0_ls_C_eq_Y_ls_valT_gr_L;C_hh__eq_Y_ls_valT_gr_L_gr_|>` 
  *  :ref:`insert_default (tab:table\<auto(keyT);auto(valT)\> -const;key:keyT const|keyT const# const) : void <function-_at__builtin__c__c_insert_default_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|>` 
  *  :ref:`emplace_default (tab:table\<auto(keyT);auto(valT)\> -const;key:keyT const|keyT const# const) : void <function-_at__builtin__c__c_emplace_default_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|>` 
  *  :ref:`get_with_default (Tab:table\<auto(keyT);auto(valT)\> ==const -const;at:keyT const|keyT const# const;blk:block\<(var p:valT& -const):void\> const) : void <function-_at__builtin__c__c_get_with_default__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_CN_ls_p_gr_0_ls_&Y_ls_valT_gr_L_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`modify (Tab:table\<auto(keyT);auto(valT)\> ==const -const;at:keyT const|keyT const# const;blk:block\<(p:valT const&):valT\> const) : void <function-_at__builtin__c__c_modify__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_CN_ls_p_gr_0_ls_C&Y_ls_valT_gr_L_gr_1_ls_Y_ls_valT_gr_L_gr__builtin_>` 
  *  :ref:`key_exists (Tab:table\<auto(keyT);auto(valT)\> const|table\<auto(keyT);auto(valT)\> const# const;at:string const#) : bool <function-_at__builtin__c__c_key_exists_C0_ls_C1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T;C_hh_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_gr_|_C_hh_s>` 
  *  :ref:`key_exists (Tab:table\<auto(keyT);auto(valT)\> const|table\<auto(keyT);auto(valT)\> const# const;at:keyT const|keyT const# const) : bool <function-_at__builtin__c__c_key_exists_C0_ls_C1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T;C_hh_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_gr_|_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|>` 
  *  :ref:`copy_to_local (a:auto(TT) const) : TT -const <function-_at__builtin__c__c_copy_to_local_CY_ls_TT_gr_.>` 
  *  :ref:`move_to_local (a:auto(TT)& -const) : TT -const -& <function-_at__builtin__c__c_move_to_local_&Y_ls_TT_gr_.>` 
  *  :ref:`keys (a:table\<auto(keyT);auto(valT)\> const ==const|table\<auto(keyT);auto(valT)\> const# ==const const) : iterator\<keyT -const\> <function-_at__builtin__c__c_keys_C0_ls_C_eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T;C_hh__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_gr_|>` 
  *  :ref:`keys (a:table\<auto(keyT);auto(valT)\> ==const -const|table\<auto(keyT);auto(valT)\># ==const -const -const) : iterator\<keyT -const\> <function-_at__builtin__c__c_keys_0_ls__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T;_hh__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_gr_|>` 
  *  :ref:`values (a:table\<auto(keyT);void\> const ==const|table\<auto(keyT);void\> const# ==const const) : auto <function-_at__builtin__c__c_values_C0_ls_C_eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_v_gr_T;C_hh__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_v_gr_T_gr_|>` 
  *  :ref:`values (a:table\<auto(keyT);void\> ==const -const|table\<auto(keyT);void\># ==const -const -const) : auto <function-_at__builtin__c__c_values_0_ls__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_v_gr_T;_hh__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_v_gr_T_gr_|>` 
  *  :ref:`values (a:table\<auto(keyT);auto(valT)\> const ==const|table\<auto(keyT);auto(valT)\> const# ==const const) : iterator\<valT const&\> <function-_at__builtin__c__c_values_C0_ls_C_eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T;C_hh__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_gr_|>` 
  *  :ref:`values (a:table\<auto(keyT);auto(valT)\> ==const -const|table\<auto(keyT);auto(valT)\># ==const -const -const) : iterator\<valT&\> <function-_at__builtin__c__c_values_0_ls__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T;_hh__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_gr_|>` 
  *  :ref:`values (a:table\<auto(keyT);auto(valT)[]\> const ==const|table\<auto(keyT);auto(valT)[]\> const# ==const const) : iterator\<valT const[-2]&\> <function-_at__builtin__c__c_values_C0_ls_C_eq_1_ls_Y_ls_keyT_gr_._gr_2_ls__lb_-1_rb_Y_ls_valT_gr_._gr_T;C_hh__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls__lb_-1_rb_Y_ls_valT_gr_._gr_T_gr_|>` 
  *  :ref:`values (a:table\<auto(keyT);auto(valT)[]\> ==const -const|table\<auto(keyT);auto(valT)[]\># ==const -const -const) : iterator\<valT[-2]&\> <function-_at__builtin__c__c_values_0_ls__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls__lb_-1_rb_Y_ls_valT_gr_._gr_T;_hh__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls__lb_-1_rb_Y_ls_valT_gr_._gr_T_gr_|>` 
  *  :ref:`lock (Tab:table\<auto(keyT);auto(valT)\> const|table\<auto(keyT);auto(valT)\> const# const;blk:block\<(t:table\<keyT;valT\> const#):void\> const) : auto <function-_at__builtin__c__c_lock_C0_ls_C1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T;C_hh_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_gr_|_CN_ls_t_gr_0_ls_C_hh_1_ls_Y_ls_keyT_gr_L_gr_2_ls_Y_ls_valT_gr_L_gr_T_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`lock_forever (Tab:table\<auto(keyT);auto(valT)\> -const|table\<auto(keyT);auto(valT)\># -const -const) : table\<keyT;valT\># <function-_at__builtin__c__c_lock_forever_0_ls_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T;_hh_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_gr_|>` 
  *  :ref:`next (it:iterator\<auto(TT)\> -const;value:TT& -const) : bool <function-_at__builtin__c__c_next_1_ls_Y_ls_TT_gr_._gr_G_&Y_ls_TT_gr_L>` 
  *  :ref:`each (rng:range const) : iterator\<int\> <function-_at__builtin__c__c_each_Cr>` 
  *  :ref:`each (str:string const) : iterator\<int\> <function-_at__builtin__c__c_each_Cs>` 
  *  :ref:`each (a:auto(TT) const[]) : iterator\<TT&\> <function-_at__builtin__c__c_each_C_lb_-1_rb_Y_ls_TT_gr_.>` 
  *  :ref:`each (a:array\<auto(TT)\> const) : iterator\<TT&\> <function-_at__builtin__c__c_each_C1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`each (a:array\<auto(TT)\> const#) : iterator\<TT&#\> <function-_at__builtin__c__c_each_C_hh_1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`each (lam:lambda\<(var arg:auto(argT) -const):bool\> const) : iterator\<argT -&\> <function-_at__builtin__c__c_each_CN_ls_arg_gr_0_ls_Y_ls_argT_gr_._gr_1_ls_b_gr__at_>` 
  *  :ref:`each_ref (lam:lambda\<(var arg:auto(argT)? -const):bool\> const) : iterator\<argT&\> <function-_at__builtin__c__c_each_ref_CN_ls_arg_gr_0_ls_1_ls_Y_ls_argT_gr_._gr__qm__gr_1_ls_b_gr__at_>` 
  *  :ref:`each_enum (tt:auto(TT) const) : iterator\<TT -const -&\> <function-_at__builtin__c__c_each_enum_CY_ls_TT_gr_.>` 
  *  :ref:`nothing (it:iterator\<auto(TT)\> -const) : iterator\<TT\> <function-_at__builtin__c__c_nothing_1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`to_array (it:iterator\<auto(TT)\> -const) : array\<TT -const -&\> <function-_at__builtin__c__c_to_array_1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`to_array (a:auto(TT) const[]) : array\<TT -const\> <function-_at__builtin__c__c_to_array_C_lb_-1_rb_Y_ls_TT_gr_.>` 
  *  :ref:`to_array_move (a:auto(TT)[] -const) : array\<TT -const\> <function-_at__builtin__c__c_to_array_move__lb_-1_rb_Y_ls_TT_gr_.>` 
  *  :ref:`to_array_move (a:auto(TT) const ==const) : array\<TT -const\> <function-_at__builtin__c__c_to_array_move_C_eq_Y_ls_TT_gr_.>` 
  *  :ref:`to_array_move (a:auto(TT) ==const -const) : array\<TT -const\> <function-_at__builtin__c__c_to_array_move__eq_Y_ls_TT_gr_.>` 
  *  :ref:`to_table (a:tuple\<auto(keyT);auto(valT)\> const[]) : table\<keyT -const;valT\> <function-_at__builtin__c__c_to_table_C_lb_-1_rb_0_ls_Y_ls_keyT_gr_.;Y_ls_valT_gr_._gr_U>` 
  *  :ref:`to_table (a:auto(keyT) const[]) : table\<keyT -const;void\> <function-_at__builtin__c__c_to_table_C_lb_-1_rb_Y_ls_keyT_gr_.>` 
  *  :ref:`to_table_move (a:auto(keyT) const[]) : table\<keyT -const;void\> <function-_at__builtin__c__c_to_table_move_C_lb_-1_rb_Y_ls_keyT_gr_.>` 
  *  :ref:`to_table_move (a:array\<auto(keyT)\> const) : table\<keyT -const;void\> <function-_at__builtin__c__c_to_table_move_C1_ls_Y_ls_keyT_gr_._gr_A>` 
  *  :ref:`to_table_move (a:auto(keyT) const) : table\<keyT -const;void\> <function-_at__builtin__c__c_to_table_move_CY_ls_keyT_gr_.>` 
  *  :ref:`to_table_move (a:tuple\<auto(keyT);auto(valT)\> -const) : table\<keyT -const;valT\> <function-_at__builtin__c__c_to_table_move_0_ls_Y_ls_keyT_gr_.;Y_ls_valT_gr_._gr_U>` 
  *  :ref:`to_table_move (a:tuple\<auto(keyT);auto(valT)\>[] -const) : table\<keyT -const;valT\> <function-_at__builtin__c__c_to_table_move__lb_-1_rb_0_ls_Y_ls_keyT_gr_.;Y_ls_valT_gr_._gr_U>` 
  *  :ref:`to_table_move (a:array\<tuple\<auto(keyT);auto(valT)\>\> -const) : table\<keyT -const;valT\> <function-_at__builtin__c__c_to_table_move_1_ls_0_ls_Y_ls_keyT_gr_.;Y_ls_valT_gr_._gr_U_gr_A>` 
  *  :ref:`sort (a:auto(TT)[] -const|auto(TT)[]# -const -const) : auto <function-_at__builtin__c__c_sort_0_ls__lb_-1_rb_Y_ls_TT_gr_.;_hh__lb_-1_rb_Y_ls_TT_gr_._gr_|>` 
  *  :ref:`sort (a:array\<auto(TT)\> -const|array\<auto(TT)\># -const -const) : auto <function-_at__builtin__c__c_sort_0_ls_1_ls_Y_ls_TT_gr_._gr_A;_hh_1_ls_Y_ls_TT_gr_._gr_A_gr_|>` 
  *  :ref:`sort (a:auto(TT)[] -const|auto(TT)[]# -const -const;cmp:block\<(x:TT const;y:TT const):bool\> const) : auto <function-_at__builtin__c__c_sort_0_ls__lb_-1_rb_Y_ls_TT_gr_.;_hh__lb_-1_rb_Y_ls_TT_gr_._gr_|_CN_ls_x;y_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`sort (a:array\<auto(TT)\> -const|array\<auto(TT)\># -const -const;cmp:block\<(x:TT const;y:TT const):bool\> const) : auto <function-_at__builtin__c__c_sort_0_ls_1_ls_Y_ls_TT_gr_._gr_A;_hh_1_ls_Y_ls_TT_gr_._gr_A_gr_|_CN_ls_x;y_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`lock (a:array\<auto(TT)\> ==const -const|array\<auto(TT)\># ==const -const -const;blk:block\<(var x:array\<TT\># -const):auto\> const) : auto <function-_at__builtin__c__c_lock_0_ls__eq_1_ls_Y_ls_TT_gr_._gr_A;_hh__eq_1_ls_Y_ls_TT_gr_._gr_A_gr_|_CN_ls_x_gr_0_ls__hh_1_ls_Y_ls_TT_gr_L_gr_A_gr_1_ls_._gr__builtin_>` 
  *  :ref:`lock (a:array\<auto(TT)\> const ==const|array\<auto(TT)\> const# ==const const;blk:block\<(x:array\<TT\> const#):auto\> const) : auto <function-_at__builtin__c__c_lock_C0_ls_C_eq_1_ls_Y_ls_TT_gr_._gr_A;C_hh__eq_1_ls_Y_ls_TT_gr_._gr_A_gr_|_CN_ls_x_gr_0_ls_C_hh_1_ls_Y_ls_TT_gr_L_gr_A_gr_1_ls_._gr__builtin_>` 
  *  :ref:`find_index (arr:array\<auto(TT)\> const|array\<auto(TT)\> const# const;key:TT const) : auto <function-_at__builtin__c__c_find_index_C0_ls_C1_ls_Y_ls_TT_gr_._gr_A;C_hh_1_ls_Y_ls_TT_gr_._gr_A_gr_|_CY_ls_TT_gr_L>` 
  *  :ref:`find_index (arr:auto(TT) const[]|auto(TT) const[]# const;key:TT const) : auto <function-_at__builtin__c__c_find_index_C0_ls_C_lb_-1_rb_Y_ls_TT_gr_.;C_hh__lb_-1_rb_Y_ls_TT_gr_._gr_|_CY_ls_TT_gr_L>` 
  *  :ref:`find_index (arr:iterator\<auto(TT)\> -const;key:TT const -&) : auto <function-_at__builtin__c__c_find_index_1_ls_Y_ls_TT_gr_._gr_G_CY_ls_TT_gr_L>` 
  *  :ref:`find_index_if (arr:array\<auto(TT)\> const|array\<auto(TT)\> const# const;blk:block\<(key:TT const):bool\> const) : auto <function-_at__builtin__c__c_find_index_if_C0_ls_C1_ls_Y_ls_TT_gr_._gr_A;C_hh_1_ls_Y_ls_TT_gr_._gr_A_gr_|_CN_ls_key_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`find_index_if (arr:auto(TT) const[]|auto(TT) const[]# const;blk:block\<(key:TT const):bool\> const) : auto <function-_at__builtin__c__c_find_index_if_C0_ls_C_lb_-1_rb_Y_ls_TT_gr_.;C_hh__lb_-1_rb_Y_ls_TT_gr_._gr_|_CN_ls_key_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`find_index_if (arr:iterator\<auto(TT)\> -const;blk:block\<(key:TT const -&):bool\> const) : auto <function-_at__builtin__c__c_find_index_if_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_key_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`has_value (a:auto const;key:auto const) : auto <function-_at__builtin__c__c_has_value_C._C.>` 
  *  :ref:`has_value (a:iterator\<auto\> -const;key:auto const) : auto <function-_at__builtin__c__c_has_value_1_ls_._gr_G_C.>` 
  *  :ref:`subarray (a:auto(TT) const[];r:range const) : auto <function-_at__builtin__c__c_subarray_C_lb_-1_rb_Y_ls_TT_gr_._Cr>` 
  *  :ref:`subarray (a:auto(TT) const[];r:urange const) : auto <function-_at__builtin__c__c_subarray_C_lb_-1_rb_Y_ls_TT_gr_._Cz>` 
  *  :ref:`subarray (a:array\<auto(TT)\> const ==const;r:range const) : auto <function-_at__builtin__c__c_subarray_C_eq_1_ls_Y_ls_TT_gr_._gr_A_Cr>` 
  *  :ref:`subarray (a:array\<auto(TT)\> ==const -const;r:range const) : auto <function-_at__builtin__c__c_subarray__eq_1_ls_Y_ls_TT_gr_._gr_A_Cr>` 
  *  :ref:`subarray (a:array\<auto(TT)\> const;r:urange const) : auto <function-_at__builtin__c__c_subarray_C1_ls_Y_ls_TT_gr_._gr_A_Cz>` 
  *  :ref:`move_to_ref (a:auto& -const;b:auto -const) : auto <function-_at__builtin__c__c_move_to_ref_&._.>` 
  *  :ref:`clear (t:table\<auto(KT);auto(VT)\> -const) : auto <function-_at__builtin__c__c_clear_1_ls_Y_ls_KT_gr_._gr_2_ls_Y_ls_VT_gr_._gr_T>` 

.. _function-_at__builtin__c__c_clear_IA_C_c_C_l:

.. das:function:: clear(array: array implicit)

+--------+--------------+
+argument+argument type +
+========+==============+
+array   +array implicit+
+--------+--------------+


|function-builtin-clear|

.. _function-_at__builtin__c__c_length_CIA:

.. das:function:: length(array: array const implicit)

length returns int const

+--------+--------------------+
+argument+argument type       +
+========+====================+
+array   +array const implicit+
+--------+--------------------+


|function-builtin-length|

.. _function-_at__builtin__c__c_capacity_CIA:

.. das:function:: capacity(array: array const implicit)

capacity returns int const

+--------+--------------------+
+argument+argument type       +
+========+====================+
+array   +array const implicit+
+--------+--------------------+


|function-builtin-capacity|

.. _function-_at__builtin__c__c_empty_CIG:

.. das:function:: empty(iterator: iterator const implicit)

empty returns bool const

+--------+-----------------------+
+argument+argument type          +
+========+=======================+
+iterator+iterator const implicit+
+--------+-----------------------+


|function-builtin-empty|

.. _function-_at__builtin__c__c_length_CIT:

.. das:function:: length(table: table const implicit)

length returns int const

+--------+--------------------+
+argument+argument type       +
+========+====================+
+table   +table const implicit+
+--------+--------------------+


|function-builtin-length|

.. _function-_at__builtin__c__c_capacity_CIT:

.. das:function:: capacity(table: table const implicit)

capacity returns int const

+--------+--------------------+
+argument+argument type       +
+========+====================+
+table   +table const implicit+
+--------+--------------------+


|function-builtin-capacity|

.. _function-_at__builtin__c__c_empty_CIs:

.. das:function:: empty(str: string const implicit)

empty returns bool const

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+str     +string const implicit+
+--------+---------------------+


|function-builtin-empty|

.. _function-_at__builtin__c__c_empty_CIH_ls__builtin__c__c_das_string_gr_:

.. das:function:: empty(str: das_string const implicit)

empty returns bool const

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+str     + :ref:`builtin::das_string <handle-builtin-das_string>`  const implicit+
+--------+-----------------------------------------------------------------------+


|function-builtin-empty|

.. _function-_at__builtin__c__c_resize_1_ls_Y_ls_numT_gr_._gr_A_Ci:

.. das:function:: resize(Arr: array<auto(numT)>; newSize: int const)

resize returns auto

+--------+-----------------+
+argument+argument type    +
+========+=================+
+Arr     +array<auto(numT)>+
+--------+-----------------+
+newSize +int const        +
+--------+-----------------+


|function-builtin-resize|

.. _function-_at__builtin__c__c_resize_and_init_1_ls_Y_ls_numT_gr_._gr_A_Ci:

.. das:function:: resize_and_init(Arr: array<auto(numT)>; newSize: int const)

resize_and_init returns auto

+--------+-----------------+
+argument+argument type    +
+========+=================+
+Arr     +array<auto(numT)>+
+--------+-----------------+
+newSize +int const        +
+--------+-----------------+


|function-builtin-resize_and_init|

.. _function-_at__builtin__c__c_resize_and_init_1_ls_Y_ls_numT_gr_._gr_A_Ci_CY_ls_numT_gr_L:

.. das:function:: resize_and_init(Arr: array<auto(numT)>; newSize: int const; initValue: numT const)

resize_and_init returns auto

+---------+-----------------+
+argument +argument type    +
+=========+=================+
+Arr      +array<auto(numT)>+
+---------+-----------------+
+newSize  +int const        +
+---------+-----------------+
+initValue+numT const       +
+---------+-----------------+


|function-builtin-resize_and_init|

.. _function-_at__builtin__c__c_resize_no_init_1_ls_Y_ls_numT_gr_._gr_A_Ci:

.. das:function:: resize_no_init(Arr: array<auto(numT)>; newSize: int const)

resize_no_init returns auto

+--------+-----------------+
+argument+argument type    +
+========+=================+
+Arr     +array<auto(numT)>+
+--------+-----------------+
+newSize +int const        +
+--------+-----------------+


|function-builtin-resize_no_init|

.. _function-_at__builtin__c__c_reserve_1_ls_Y_ls_numT_gr_._gr_A_Ci:

.. das:function:: reserve(Arr: array<auto(numT)>; newSize: int const)

reserve returns auto

+--------+-----------------+
+argument+argument type    +
+========+=================+
+Arr     +array<auto(numT)>+
+--------+-----------------+
+newSize +int const        +
+--------+-----------------+


|function-builtin-reserve|

.. _function-_at__builtin__c__c_pop_1_ls_Y_ls_numT_gr_._gr_A:

.. das:function:: pop(Arr: array<auto(numT)>)

pop returns auto

+--------+-----------------+
+argument+argument type    +
+========+=================+
+Arr     +array<auto(numT)>+
+--------+-----------------+


|function-builtin-pop|

.. _function-_at__builtin__c__c_push_1_ls_Y_ls_numT_gr_._gr_A_C_eq_Y_ls_numT_gr_L_Ci:

.. das:function:: push(Arr: array<auto(numT)>; value: numT const ==const; at: int const)

push returns auto

+--------+-----------------+
+argument+argument type    +
+========+=================+
+Arr     +array<auto(numT)>+
+--------+-----------------+
+value   +numT const!      +
+--------+-----------------+
+at      +int const        +
+--------+-----------------+


|function-builtin-push|

.. _function-_at__builtin__c__c_push_1_ls_Y_ls_numT_gr_._gr_A__eq_Y_ls_numT_gr_L_Ci:

.. das:function:: push(Arr: array<auto(numT)>; value: numT ==const; at: int const)

push returns auto

+--------+-----------------+
+argument+argument type    +
+========+=================+
+Arr     +array<auto(numT)>+
+--------+-----------------+
+value   +numT!            +
+--------+-----------------+
+at      +int const        +
+--------+-----------------+


|function-builtin-push|

.. _function-_at__builtin__c__c_push_1_ls_Y_ls_numT_gr_._gr_A_C_eq_Y_ls_numT_gr_L:

.. das:function:: push(Arr: array<auto(numT)>; value: numT const ==const)

push returns auto

+--------+-----------------+
+argument+argument type    +
+========+=================+
+Arr     +array<auto(numT)>+
+--------+-----------------+
+value   +numT const!      +
+--------+-----------------+


|function-builtin-push|

.. _function-_at__builtin__c__c_push_1_ls_Y_ls_numT_gr_._gr_A__eq_Y_ls_numT_gr_L:

.. das:function:: push(Arr: array<auto(numT)>; value: numT ==const)

push returns auto

+--------+-----------------+
+argument+argument type    +
+========+=================+
+Arr     +array<auto(numT)>+
+--------+-----------------+
+value   +numT!            +
+--------+-----------------+


|function-builtin-push|

.. _function-_at__builtin__c__c_push_1_ls_Y_ls_numT_gr_._gr_A_C_eq_1_ls_Y_ls_numT_gr_L_gr_A:

.. das:function:: push(Arr: array<auto(numT)>; varr: array<numT> const ==const)

push returns auto

+--------+------------------+
+argument+argument type     +
+========+==================+
+Arr     +array<auto(numT)> +
+--------+------------------+
+varr    +array<numT> const!+
+--------+------------------+


|function-builtin-push|

.. _function-_at__builtin__c__c_push_1_ls_Y_ls_numT_gr_._gr_A__eq_1_ls_Y_ls_numT_gr_L_gr_A:

.. das:function:: push(Arr: array<auto(numT)>; varr: array<numT> ==const)

push returns auto

+--------+-----------------+
+argument+argument type    +
+========+=================+
+Arr     +array<auto(numT)>+
+--------+-----------------+
+varr    +array<numT>!     +
+--------+-----------------+


|function-builtin-push|

.. _function-_at__builtin__c__c_push_1_ls_Y_ls_numT_gr_._gr_A_C_lb_-1_rb_Y_ls_numT_gr_L:

.. das:function:: push(Arr: array<auto(numT)>; varr: numT const[])

push returns auto

+--------+-----------------+
+argument+argument type    +
+========+=================+
+Arr     +array<auto(numT)>+
+--------+-----------------+
+varr    +numT const[-1]   +
+--------+-----------------+


|function-builtin-push|

.. _function-_at__builtin__c__c_push_1_ls__lb_-1_rb_Y_ls_numT_gr_._gr_A_C_eq__lb_-1_rb_Y_ls_numT_gr_L:

.. das:function:: push(Arr: array<auto(numT)[]>; varr: numT const[] ==const)

push returns auto

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+Arr     +array<auto(numT)[-1]>+
+--------+---------------------+
+varr    +numT const[-1]!      +
+--------+---------------------+


|function-builtin-push|

.. _function-_at__builtin__c__c_push_1_ls__lb_-1_rb_Y_ls_numT_gr_._gr_A__eq__lb_-1_rb_Y_ls_numT_gr_L:

.. das:function:: push(Arr: array<auto(numT)[]>; varr: numT[] ==const)

push returns auto

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+Arr     +array<auto(numT)[-1]>+
+--------+---------------------+
+varr    +numT[-1]!            +
+--------+---------------------+


|function-builtin-push|

.. _function-_at__builtin__c__c_emplace_1_ls_Y_ls_numT_gr_._gr_A_&Y_ls_numT_gr_L_Ci:

.. das:function:: emplace(Arr: array<auto(numT)>; value: numT&; at: int const)

emplace returns auto

+--------+-----------------+
+argument+argument type    +
+========+=================+
+Arr     +array<auto(numT)>+
+--------+-----------------+
+value   +numT&            +
+--------+-----------------+
+at      +int const        +
+--------+-----------------+


|function-builtin-emplace|

.. _function-_at__builtin__c__c_emplace_1_ls_Y_ls_numT_gr_._gr_A_&Y_ls_numT_gr_L:

.. das:function:: emplace(Arr: array<auto(numT)>; value: numT&)

emplace returns auto

+--------+-----------------+
+argument+argument type    +
+========+=================+
+Arr     +array<auto(numT)>+
+--------+-----------------+
+value   +numT&            +
+--------+-----------------+


|function-builtin-emplace|

.. _function-_at__builtin__c__c_emplace_1_ls_Y_ls_numT_gr_._gr_A__lb_-1_rb_Y_ls_numT_gr_L:

.. das:function:: emplace(Arr: array<auto(numT)>; value: numT[])

emplace returns auto

+--------+-----------------+
+argument+argument type    +
+========+=================+
+Arr     +array<auto(numT)>+
+--------+-----------------+
+value   +numT[-1]         +
+--------+-----------------+


|function-builtin-emplace|

.. _function-_at__builtin__c__c_emplace_1_ls__lb_-1_rb_Y_ls_numT_gr_._gr_A__lb_-1_rb_Y_ls_numT_gr_L:

.. das:function:: emplace(Arr: array<auto(numT)[]>; value: numT[])

emplace returns auto

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+Arr     +array<auto(numT)[-1]>+
+--------+---------------------+
+value   +numT[-1]             +
+--------+---------------------+


|function-builtin-emplace|

.. _function-_at__builtin__c__c_push_clone_1_ls_Y_ls_numT_gr_._gr_A_C0_ls_C_eq_Y_ls_numT_gr_L;C_hh__eq_Y_ls_numT_gr_L_gr_|_Ci:

.. das:function:: push_clone(Arr: array<auto(numT)>; value: numT const ==const|numT const# ==const const; at: int const)

push_clone returns auto

+--------+-----------------+
+argument+argument type    +
+========+=================+
+Arr     +array<auto(numT)>+
+--------+-----------------+
+value   +option const     +
+--------+-----------------+
+at      +int const        +
+--------+-----------------+


|function-builtin-push_clone|

.. _function-_at__builtin__c__c_push_clone_1_ls_Y_ls_numT_gr_._gr_A_0_ls__eq_Y_ls_numT_gr_L;_hh__eq_Y_ls_numT_gr_L_gr_|_Ci:

.. das:function:: push_clone(Arr: array<auto(numT)>; value: numT ==const|numT# ==const; at: int const)

push_clone returns auto

+--------+-----------------+
+argument+argument type    +
+========+=================+
+Arr     +array<auto(numT)>+
+--------+-----------------+
+value   +option           +
+--------+-----------------+
+at      +int const        +
+--------+-----------------+


|function-builtin-push_clone|

.. _function-_at__builtin__c__c_push_clone_1_ls_Y_ls_numT_gr_._gr_A_C0_ls_C_eq_Y_ls_numT_gr_L;C_hh__eq_Y_ls_numT_gr_L_gr_|:

.. das:function:: push_clone(Arr: array<auto(numT)>; value: numT const ==const|numT const# ==const const)

push_clone returns auto

+--------+-----------------+
+argument+argument type    +
+========+=================+
+Arr     +array<auto(numT)>+
+--------+-----------------+
+value   +option const     +
+--------+-----------------+


|function-builtin-push_clone|

.. _function-_at__builtin__c__c_push_clone_1_ls_Y_ls_numT_gr_._gr_A_0_ls__eq_Y_ls_numT_gr_L;_hh__eq_Y_ls_numT_gr_L_gr_|:

.. das:function:: push_clone(Arr: array<auto(numT)>; value: numT ==const|numT# ==const)

push_clone returns auto

+--------+-----------------+
+argument+argument type    +
+========+=================+
+Arr     +array<auto(numT)>+
+--------+-----------------+
+value   +option           +
+--------+-----------------+


|function-builtin-push_clone|

.. _function-_at__builtin__c__c_push_clone_1_ls_Y_ls_numT_gr_._gr_A_C_eq__lb_-1_rb_Y_ls_numT_gr_L:

.. das:function:: push_clone(Arr: array<auto(numT)>; varr: numT const[] ==const)

push_clone returns auto

+--------+-----------------+
+argument+argument type    +
+========+=================+
+Arr     +array<auto(numT)>+
+--------+-----------------+
+varr    +numT const[-1]!  +
+--------+-----------------+


|function-builtin-push_clone|

.. _function-_at__builtin__c__c_push_clone_1_ls_Y_ls_numT_gr_._gr_A__eq__lb_-1_rb_Y_ls_numT_gr_L:

.. das:function:: push_clone(Arr: array<auto(numT)>; varr: numT[] ==const)

push_clone returns auto

+--------+-----------------+
+argument+argument type    +
+========+=================+
+Arr     +array<auto(numT)>+
+--------+-----------------+
+varr    +numT[-1]!        +
+--------+-----------------+


|function-builtin-push_clone|

.. _function-_at__builtin__c__c_push_clone_1_ls__lb_-1_rb_Y_ls_numT_gr_._gr_A_C_eq__lb_-1_rb_Y_ls_numT_gr_L:

.. das:function:: push_clone(Arr: array<auto(numT)[]>; varr: numT const[] ==const)

push_clone returns auto

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+Arr     +array<auto(numT)[-1]>+
+--------+---------------------+
+varr    +numT const[-1]!      +
+--------+---------------------+


|function-builtin-push_clone|

.. _function-_at__builtin__c__c_push_clone_1_ls__lb_-1_rb_Y_ls_numT_gr_._gr_A__lb_-1_rb_Y_ls_numT_gr_L:

.. das:function:: push_clone(Arr: array<auto(numT)[]>; varr: numT[])

push_clone returns auto

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+Arr     +array<auto(numT)[-1]>+
+--------+---------------------+
+varr    +numT[-1]             +
+--------+---------------------+


|function-builtin-push_clone|

.. _function-_at__builtin__c__c_push_clone_Y_ls_CT_gr_._C0_ls_CY_ls_TT_gr_.;C_hh_Y_ls_TT_gr_._gr_|:

.. das:function:: push_clone(A: auto(CT); b: auto(TT) const|auto(TT) const# const)

push_clone returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+A       +auto(CT)     +
+--------+-------------+
+b       +option const +
+--------+-------------+


|function-builtin-push_clone|

.. _function-_at__builtin__c__c_back__eq_1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: back(a: array<auto(TT)> ==const)

back returns TT&

+--------+----------------+
+argument+argument type   +
+========+================+
+a       +array<auto(TT)>!+
+--------+----------------+


|function-builtin-back|

.. _function-_at__builtin__c__c_back__hh__eq_1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: back(a: array<auto(TT)># ==const)

back returns TT&#

+--------+-----------------+
+argument+argument type    +
+========+=================+
+a       +array<auto(TT)>#!+
+--------+-----------------+


|function-builtin-back|

.. _function-_at__builtin__c__c_back_C_eq_1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: back(a: array<auto(TT)> const ==const)

back returns TT const&

+--------+----------------------+
+argument+argument type         +
+========+======================+
+a       +array<auto(TT)> const!+
+--------+----------------------+


|function-builtin-back|

.. _function-_at__builtin__c__c_back_C_hh__eq_1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: back(a: array<auto(TT)> const# ==const)

back returns TT const&#

+--------+-----------------------+
+argument+argument type          +
+========+=======================+
+a       +array<auto(TT)> const#!+
+--------+-----------------------+


|function-builtin-back|

.. _function-_at__builtin__c__c_back__eq_Y_ls_TT_gr_.:

.. das:function:: back(arr: auto(TT) ==const)

back returns auto&

+--------+-------------+
+argument+argument type+
+========+=============+
+arr     +auto(TT)!    +
+--------+-------------+


|function-builtin-back|

.. _function-_at__builtin__c__c_back_C_eq_Y_ls_TT_gr_.:

.. das:function:: back(arr: auto(TT) const ==const)

back returns auto const&

+--------+---------------+
+argument+argument type  +
+========+===============+
+arr     +auto(TT) const!+
+--------+---------------+


|function-builtin-back|

.. _function-_at__builtin__c__c_erase_1_ls_Y_ls_numT_gr_._gr_A_Ci:

.. das:function:: erase(Arr: array<auto(numT)>; at: int const)

erase returns auto

+--------+-----------------+
+argument+argument type    +
+========+=================+
+Arr     +array<auto(numT)>+
+--------+-----------------+
+at      +int const        +
+--------+-----------------+


|function-builtin-erase|

.. _function-_at__builtin__c__c_erase_1_ls_Y_ls_numT_gr_._gr_A_Ci_Ci:

.. das:function:: erase(Arr: array<auto(numT)>; at: int const; count: int const)

erase returns auto

+--------+-----------------+
+argument+argument type    +
+========+=================+
+Arr     +array<auto(numT)>+
+--------+-----------------+
+at      +int const        +
+--------+-----------------+
+count   +int const        +
+--------+-----------------+


|function-builtin-erase|

.. _function-_at__builtin__c__c_erase_if_1_ls_Y_ls_TT_gr_._gr_A_C0_ls_CN_ls_key_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_;CN_ls_key_gr_0_ls_&Y_ls_TT_gr_L_gr_1_ls_b_gr__builtin__gr_|:

.. das:function:: erase_if(arr: array<auto(TT)>; blk: block<(key:TT const):bool> const|block<(var key:TT&):bool> const const)

erase_if returns auto

+--------+---------------+
+argument+argument type  +
+========+===============+
+arr     +array<auto(TT)>+
+--------+---------------+
+blk     +option const   +
+--------+---------------+


|function-builtin-erase_if|

.. _function-_at__builtin__c__c_remove_value_0_ls_1_ls_Y_ls_TT_gr_._gr_A;_hh_1_ls_Y_ls_TT_gr_._gr_A_gr_|_CY_ls_TT_gr_L:

.. das:function:: remove_value(arr: array<auto(TT)>|array<auto(TT)>#; key: TT const)

remove_value returns bool

+--------+-------------+
+argument+argument type+
+========+=============+
+arr     +option       +
+--------+-------------+
+key     +TT const     +
+--------+-------------+


|function-builtin-remove_value|

.. _function-_at__builtin__c__c_length_C0_ls_C.;C_hh_._gr_|:

.. das:function:: length(a: auto const|auto const# const)

length returns int

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +option const +
+--------+-------------+


|function-builtin-length|

.. _function-_at__builtin__c__c_empty_C0_ls_C1_ls_._gr_A;C_hh_1_ls_._gr_A_gr_|:

.. das:function:: empty(a: array<auto> const|array<auto> const# const)

empty returns bool

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +option const +
+--------+-------------+


|function-builtin-empty|

.. _function-_at__builtin__c__c_empty_C0_ls_C1_ls_._gr_2_ls_._gr_T;C_hh_1_ls_._gr_2_ls_._gr_T_gr_|:

.. das:function:: empty(a: table<auto;auto> const|table<auto;auto> const# const)

empty returns bool

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +option const +
+--------+-------------+


|function-builtin-empty|

.. _function-_at__builtin__c__c_reserve_1_ls_Y_ls_keyT_gr_._gr_2_ls_._gr_T_Ci:

.. das:function:: reserve(Tab: table<auto(keyT);auto>; newSize: int const)

reserve returns auto

+--------+----------------------+
+argument+argument type         +
+========+======================+
+Tab     +table<auto(keyT);auto>+
+--------+----------------------+
+newSize +int const             +
+--------+----------------------+


|function-builtin-reserve|

.. _function-_at__builtin__c__c_get_C_hh__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_CN_ls_p_gr_0_ls_C&_hh_Y_ls_valT_gr_L_gr_1_ls_v_gr__builtin_:

.. das:function:: get(Tab: table<auto(keyT);auto(valT)> const# ==const; at: keyT const|keyT const# const; blk: block<(p:valT const&#):void> const)

get returns auto

+--------+------------------------------------+
+argument+argument type                       +
+========+====================================+
+Tab     +table<auto(keyT);auto(valT)> const#!+
+--------+------------------------------------+
+at      +option const                        +
+--------+------------------------------------+
+blk     +block<(p:valT const&#):void> const  +
+--------+------------------------------------+


|function-builtin-get|

.. _function-_at__builtin__c__c_get_C_eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_CN_ls_p_gr_0_ls_C&Y_ls_valT_gr_L_gr_1_ls_v_gr__builtin_:

.. das:function:: get(Tab: table<auto(keyT);auto(valT)> const ==const; at: keyT const|keyT const# const; blk: block<(p:valT const&):void> const)

get returns auto

+--------+-----------------------------------+
+argument+argument type                      +
+========+===================================+
+Tab     +table<auto(keyT);auto(valT)> const!+
+--------+-----------------------------------+
+at      +option const                       +
+--------+-----------------------------------+
+blk     +block<(p:valT const&):void> const  +
+--------+-----------------------------------+


|function-builtin-get|

.. _function-_at__builtin__c__c_get__hh__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_CN_ls_p_gr_0_ls_&_hh_Y_ls_valT_gr_L_gr_1_ls_v_gr__builtin_:

.. das:function:: get(Tab: table<auto(keyT);auto(valT)># ==const; at: keyT const|keyT const# const; blk: block<(var p:valT&#):void> const)

get returns auto

+--------+------------------------------+
+argument+argument type                 +
+========+==============================+
+Tab     +table<auto(keyT);auto(valT)>#!+
+--------+------------------------------+
+at      +option const                  +
+--------+------------------------------+
+blk     +block<(p:valT&#):void> const  +
+--------+------------------------------+


|function-builtin-get|

.. _function-_at__builtin__c__c_get__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_CN_ls_p_gr_0_ls_&Y_ls_valT_gr_L_gr_1_ls_v_gr__builtin_:

.. das:function:: get(Tab: table<auto(keyT);auto(valT)> ==const; at: keyT const|keyT const# const; blk: block<(var p:valT&):void> const)

get returns auto

+--------+-----------------------------+
+argument+argument type                +
+========+=============================+
+Tab     +table<auto(keyT);auto(valT)>!+
+--------+-----------------------------+
+at      +option const                 +
+--------+-----------------------------+
+blk     +block<(p:valT&):void> const  +
+--------+-----------------------------+


|function-builtin-get|

.. _function-_at__builtin__c__c_get_C_hh__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls__lb_-1_rb_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_CN_ls_p_gr_0_ls_C&_hh__lb_-2_rb_Y_ls_valT_gr_L_gr_1_ls_v_gr__builtin_:

.. das:function:: get(Tab: table<auto(keyT);auto(valT)[]> const# ==const; at: keyT const|keyT const# const; blk: block<(p:valT const[-2]&#):void> const)

get returns auto

+--------+----------------------------------------+
+argument+argument type                           +
+========+========================================+
+Tab     +table<auto(keyT);auto(valT)[-1]> const#!+
+--------+----------------------------------------+
+at      +option const                            +
+--------+----------------------------------------+
+blk     +block<(p:valT const[-2]&#):void> const  +
+--------+----------------------------------------+


|function-builtin-get|

.. _function-_at__builtin__c__c_get_C_eq_1_ls_Y_ls_keyT_gr_._gr_2_ls__lb_-1_rb_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_CN_ls_p_gr_0_ls_C&_lb_-2_rb_Y_ls_valT_gr_L_gr_1_ls_v_gr__builtin_:

.. das:function:: get(Tab: table<auto(keyT);auto(valT)[]> const ==const; at: keyT const|keyT const# const; blk: block<(p:valT const[-2]&):void> const)

get returns auto

+--------+---------------------------------------+
+argument+argument type                          +
+========+=======================================+
+Tab     +table<auto(keyT);auto(valT)[-1]> const!+
+--------+---------------------------------------+
+at      +option const                           +
+--------+---------------------------------------+
+blk     +block<(p:valT const[-2]&):void> const  +
+--------+---------------------------------------+


|function-builtin-get|

.. _function-_at__builtin__c__c_get__hh__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls__lb_-1_rb_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_CN_ls_p_gr_0_ls_&_hh__lb_-2_rb_Y_ls_valT_gr_L_gr_1_ls_v_gr__builtin_:

.. das:function:: get(Tab: table<auto(keyT);auto(valT)[]># ==const; at: keyT const|keyT const# const; blk: block<(var p:valT[-2]&#):void> const)

get returns auto

+--------+----------------------------------+
+argument+argument type                     +
+========+==================================+
+Tab     +table<auto(keyT);auto(valT)[-1]>#!+
+--------+----------------------------------+
+at      +option const                      +
+--------+----------------------------------+
+blk     +block<(p:valT[-2]&#):void> const  +
+--------+----------------------------------+


|function-builtin-get|

.. _function-_at__builtin__c__c_get__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls__lb_-1_rb_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_CN_ls_p_gr_0_ls_&_lb_-2_rb_Y_ls_valT_gr_L_gr_1_ls_v_gr__builtin_:

.. das:function:: get(Tab: table<auto(keyT);auto(valT)[]> ==const; at: keyT const|keyT const# const; blk: block<(var p:valT[-2]&):void> const)

get returns auto

+--------+---------------------------------+
+argument+argument type                    +
+========+=================================+
+Tab     +table<auto(keyT);auto(valT)[-1]>!+
+--------+---------------------------------+
+at      +option const                     +
+--------+---------------------------------+
+blk     +block<(p:valT[-2]&):void> const  +
+--------+---------------------------------+


|function-builtin-get|

.. _function-_at__builtin__c__c_get_C1_ls_Y_ls_keyT_gr_._gr_2_ls_v_gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_CN_ls_p_gr_0_ls_1_ls_v_gr__qm__gr_1_ls_v_gr__builtin_:

.. das:function:: get(Tab: table<auto(keyT);void> const; at: keyT const|keyT const# const; blk: block<(var p:void?):void> const)

get returns auto

+--------+----------------------------+
+argument+argument type               +
+========+============================+
+Tab     +table<auto(keyT);void> const+
+--------+----------------------------+
+at      +option const                +
+--------+----------------------------+
+blk     +block<(p:void?):void> const +
+--------+----------------------------+


|function-builtin-get|

.. _function-_at__builtin__c__c_get_value_C_eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|:

.. das:function:: get_value(Tab: table<auto(keyT);auto(valT)> const ==const; at: keyT const|keyT const# const)

get_value returns valT

+--------+-----------------------------------+
+argument+argument type                      +
+========+===================================+
+Tab     +table<auto(keyT);auto(valT)> const!+
+--------+-----------------------------------+
+at      +option const                       +
+--------+-----------------------------------+


|function-builtin-get_value|

.. _function-_at__builtin__c__c_get_value_1_ls_Y_ls_keyT_gr_._gr_2_ls_1_ls_Y_ls_valT_gr_._gr__qm_M_gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|:

.. das:function:: get_value(Tab: table<auto(keyT);smart_ptr<auto(valT)>>; at: keyT const|keyT const# const)

get_value returns smart_ptr<valT>

+--------+---------------------------------------+
+argument+argument type                          +
+========+=======================================+
+Tab     +table<auto(keyT);smart_ptr<auto(valT)>>+
+--------+---------------------------------------+
+at      +option const                           +
+--------+---------------------------------------+


|function-builtin-get_value|

.. _function-_at__builtin__c__c_get_value_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|:

.. das:function:: get_value(Tab: table<auto(keyT);auto(valT)>; at: keyT const|keyT const# const)

get_value returns valT

+--------+----------------------------+
+argument+argument type               +
+========+============================+
+Tab     +table<auto(keyT);auto(valT)>+
+--------+----------------------------+
+at      +option const                +
+--------+----------------------------+


|function-builtin-get_value|

.. _function-_at__builtin__c__c_get_value_1_ls_Y_ls_keyT_gr_._gr_2_ls__lb_-1_rb_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|:

.. das:function:: get_value(Tab: table<auto(keyT);auto(valT)[]>; at: keyT const|keyT const# const)

get_value returns valT[-2]

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+Tab     +table<auto(keyT);auto(valT)[-1]>+
+--------+--------------------------------+
+at      +option const                    +
+--------+--------------------------------+


|function-builtin-get_value|

.. _function-_at__builtin__c__c_erase_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C_hh_s:

.. das:function:: erase(Tab: table<auto(keyT);auto(valT)>; at: string const#)

erase returns bool

+--------+----------------------------+
+argument+argument type               +
+========+============================+
+Tab     +table<auto(keyT);auto(valT)>+
+--------+----------------------------+
+at      +string const#               +
+--------+----------------------------+


|function-builtin-erase|

.. _function-_at__builtin__c__c_erase_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|:

.. das:function:: erase(Tab: table<auto(keyT);auto(valT)>; at: keyT const|keyT const# const)

erase returns bool

+--------+----------------------------+
+argument+argument type               +
+========+============================+
+Tab     +table<auto(keyT);auto(valT)>+
+--------+----------------------------+
+at      +option const                +
+--------+----------------------------+


|function-builtin-erase|

.. _function-_at__builtin__c__c_insert_1_ls_Y_ls_keyT_gr_._gr_2_ls_v_gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|:

.. das:function:: insert(Tab: table<auto(keyT);void>; at: keyT const|keyT const# const)

insert returns auto

+--------+----------------------+
+argument+argument type         +
+========+======================+
+Tab     +table<auto(keyT);void>+
+--------+----------------------+
+at      +option const          +
+--------+----------------------+


|function-builtin-insert|

.. _function-_at__builtin__c__c_insert_clone_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_0_ls__eq_Y_ls_valT_gr_L;_hh__eq_Y_ls_valT_gr_L_gr_|:

.. das:function:: insert_clone(Tab: table<auto(keyT);auto(valT)>; at: keyT const|keyT const# const; val: valT ==const|valT# ==const)

insert_clone returns auto

+--------+----------------------------+
+argument+argument type               +
+========+============================+
+Tab     +table<auto(keyT);auto(valT)>+
+--------+----------------------------+
+at      +option const                +
+--------+----------------------------+
+val     +option                      +
+--------+----------------------------+


|function-builtin-insert_clone|

.. _function-_at__builtin__c__c_insert_clone_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_C0_ls_C_eq_Y_ls_valT_gr_L;C_hh__eq_Y_ls_valT_gr_L_gr_|:

.. das:function:: insert_clone(Tab: table<auto(keyT);auto(valT)>; at: keyT const|keyT const# const; val: valT const ==const|valT const# ==const const)

insert_clone returns auto

+--------+----------------------------+
+argument+argument type               +
+========+============================+
+Tab     +table<auto(keyT);auto(valT)>+
+--------+----------------------------+
+at      +option const                +
+--------+----------------------------+
+val     +option const                +
+--------+----------------------------+


|function-builtin-insert_clone|

.. _function-_at__builtin__c__c_insert_clone_1_ls_Y_ls_keyT_gr_._gr_2_ls__lb_-1_rb_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_0_ls__eq__lb_-1_rb_Y_ls_valT_gr_L;_hh__eq__lb_-1_rb_Y_ls_valT_gr_L_gr_|:

.. das:function:: insert_clone(Tab: table<auto(keyT);auto(valT)[]>; at: keyT const|keyT const# const; val: valT[] ==const|valT[]# ==const)

insert_clone returns auto

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+Tab     +table<auto(keyT);auto(valT)[-1]>+
+--------+--------------------------------+
+at      +option const                    +
+--------+--------------------------------+
+val     +option                          +
+--------+--------------------------------+


|function-builtin-insert_clone|

.. _function-_at__builtin__c__c_insert_clone_1_ls_Y_ls_keyT_gr_._gr_2_ls__lb_-1_rb_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_C0_ls_C_eq__lb_-1_rb_Y_ls_valT_gr_L;C_hh__eq__lb_-1_rb_Y_ls_valT_gr_L_gr_|:

.. das:function:: insert_clone(Tab: table<auto(keyT);auto(valT)[]>; at: keyT const|keyT const# const; val: valT const[] ==const|valT const[]# ==const const)

insert_clone returns auto

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+Tab     +table<auto(keyT);auto(valT)[-1]>+
+--------+--------------------------------+
+at      +option const                    +
+--------+--------------------------------+
+val     +option const                    +
+--------+--------------------------------+


|function-builtin-insert_clone|

.. _function-_at__builtin__c__c_insert_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_0_ls__eq_Y_ls_valT_gr_L;_hh__eq_Y_ls_valT_gr_L_gr_|:

.. das:function:: insert(Tab: table<auto(keyT);auto(valT)>; at: keyT const|keyT const# const; val: valT ==const|valT# ==const)

insert returns auto

+--------+----------------------------+
+argument+argument type               +
+========+============================+
+Tab     +table<auto(keyT);auto(valT)>+
+--------+----------------------------+
+at      +option const                +
+--------+----------------------------+
+val     +option                      +
+--------+----------------------------+


|function-builtin-insert|

.. _function-_at__builtin__c__c_insert_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_C0_ls_C_eq_Y_ls_valT_gr_L;C_hh__eq_Y_ls_valT_gr_L_gr_|:

.. das:function:: insert(Tab: table<auto(keyT);auto(valT)>; at: keyT const|keyT const# const; val: valT const ==const|valT const# ==const const)

insert returns auto

+--------+----------------------------+
+argument+argument type               +
+========+============================+
+Tab     +table<auto(keyT);auto(valT)>+
+--------+----------------------------+
+at      +option const                +
+--------+----------------------------+
+val     +option const                +
+--------+----------------------------+


|function-builtin-insert|

.. _function-_at__builtin__c__c_insert_1_ls_Y_ls_keyT_gr_._gr_2_ls__lb_-1_rb_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_0_ls__eq__lb_-1_rb_Y_ls_valT_gr_L;_hh__eq__lb_-1_rb_Y_ls_valT_gr_L_gr_|:

.. das:function:: insert(Tab: table<auto(keyT);auto(valT)[]>; at: keyT const|keyT const# const; val: valT[] ==const|valT[]# ==const)

insert returns auto

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+Tab     +table<auto(keyT);auto(valT)[-1]>+
+--------+--------------------------------+
+at      +option const                    +
+--------+--------------------------------+
+val     +option                          +
+--------+--------------------------------+


|function-builtin-insert|

.. _function-_at__builtin__c__c_insert_1_ls_Y_ls_keyT_gr_._gr_2_ls__lb_-1_rb_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_C0_ls_C_eq__lb_-1_rb_Y_ls_valT_gr_L;C_hh__eq__lb_-1_rb_Y_ls_valT_gr_L_gr_|:

.. das:function:: insert(Tab: table<auto(keyT);auto(valT)[]>; at: keyT const|keyT const# const; val: valT const[] ==const|valT const[]# ==const const)

insert returns auto

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+Tab     +table<auto(keyT);auto(valT)[-1]>+
+--------+--------------------------------+
+at      +option const                    +
+--------+--------------------------------+
+val     +option const                    +
+--------+--------------------------------+


|function-builtin-insert|

.. _function-_at__builtin__c__c_emplace_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_&Y_ls_valT_gr_L:

.. das:function:: emplace(Tab: table<auto(keyT);auto(valT)>; at: keyT const|keyT const# const; val: valT&)

emplace returns auto

+--------+----------------------------+
+argument+argument type               +
+========+============================+
+Tab     +table<auto(keyT);auto(valT)>+
+--------+----------------------------+
+at      +option const                +
+--------+----------------------------+
+val     +valT&                       +
+--------+----------------------------+


|function-builtin-emplace|

.. _function-_at__builtin__c__c_emplace_1_ls_Y_ls_keyT_gr_._gr_2_ls_1_ls_Y_ls_valT_gr_._gr__qm_M_gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_&1_ls_Y_ls_valT_gr_L_gr__qm_M:

.. das:function:: emplace(Tab: table<auto(keyT);smart_ptr<auto(valT)>>; at: keyT const|keyT const# const; val: smart_ptr<valT>&)

emplace returns auto

+--------+---------------------------------------+
+argument+argument type                          +
+========+=======================================+
+Tab     +table<auto(keyT);smart_ptr<auto(valT)>>+
+--------+---------------------------------------+
+at      +option const                           +
+--------+---------------------------------------+
+val     +smart_ptr<valT>&                       +
+--------+---------------------------------------+


|function-builtin-emplace|

.. _function-_at__builtin__c__c_emplace_1_ls_Y_ls_keyT_gr_._gr_2_ls__lb_-1_rb_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_&_lb_-1_rb_Y_ls_valT_gr_L:

.. das:function:: emplace(Tab: table<auto(keyT);auto(valT)[]>; at: keyT const|keyT const# const; val: valT[]&)

emplace returns auto

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+Tab     +table<auto(keyT);auto(valT)[-1]>+
+--------+--------------------------------+
+at      +option const                    +
+--------+--------------------------------+
+val     +valT[-1]&                       +
+--------+--------------------------------+


|function-builtin-emplace|

.. _function-_at__builtin__c__c_emplace_new_1_ls_Y_ls_kT_gr_._gr_2_ls_1_ls_Y_ls_vT_gr_._gr__qm_M_gr_T_CY_ls_kT_gr_L_1_ls_Y_ls_vT_gr_L_gr__qm_M:

.. das:function:: emplace_new(tab: table<auto(kT);smart_ptr<auto(vT)>>; key: kT const; value: smart_ptr<vT>)

emplace_new returns auto

+--------+-----------------------------------+
+argument+argument type                      +
+========+===================================+
+tab     +table<auto(kT);smart_ptr<auto(vT)>>+
+--------+-----------------------------------+
+key     +kT const                           +
+--------+-----------------------------------+
+value   +smart_ptr<vT>                      +
+--------+-----------------------------------+


|function-builtin-emplace_new|

.. _function-_at__builtin__c__c_emplace_1_ls_._gr_2_ls_._gr_T_C._C._%_ls__ex_()_gr_:

.. das:function:: emplace(Tab: table<auto;auto>; key: auto const; value: auto const)

emplace returns auto

+--------+----------------+
+argument+argument type   +
+========+================+
+Tab     +table<auto;auto>+
+--------+----------------+
+key     +auto const      +
+--------+----------------+
+value   +auto const      +
+--------+----------------+


|function-builtin-emplace|

.. _function-_at__builtin__c__c_emplace_1_ls_._gr_A_C._%_ls__ex_()_gr_:

.. das:function:: emplace(a: array<auto>; value: auto const)

emplace returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +array<auto>  +
+--------+-------------+
+value   +auto const   +
+--------+-------------+


|function-builtin-emplace|

.. _function-_at__builtin__c__c_emplace_new_1_ls_1_ls_Y_ls_numT_gr_._gr__qm_M_gr_A_1_ls_Y_ls_numT_gr_L_gr__qm_M:

.. das:function:: emplace_new(Arr: array<smart_ptr<auto(numT)>>; value: smart_ptr<numT>)

emplace_new returns auto

+--------+----------------------------+
+argument+argument type               +
+========+============================+
+Arr     +array<smart_ptr<auto(numT)>>+
+--------+----------------------------+
+value   +smart_ptr<numT>             +
+--------+----------------------------+


|function-builtin-emplace_new|

.. _function-_at__builtin__c__c_insert_default_1_ls_Y_ls_TT_gr_._gr_2_ls_Y_ls_QQ_gr_._gr_T_C0_ls_CY_ls_TT_gr_L;C_hh_Y_ls_TT_gr_L_gr_|_0_ls__eq_Y_ls_QQ_gr_L;_hh__eq_Y_ls_QQ_gr_L_gr_|:

.. das:function:: insert_default(tab: table<auto(TT);auto(QQ)>; key: TT const|TT const# const; value: QQ ==const|QQ# ==const)

+--------+------------------------+
+argument+argument type           +
+========+========================+
+tab     +table<auto(TT);auto(QQ)>+
+--------+------------------------+
+key     +option const            +
+--------+------------------------+
+value   +option                  +
+--------+------------------------+


|function-builtin-insert_default|

.. _function-_at__builtin__c__c_insert_default_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_C0_ls_C_eq_Y_ls_valT_gr_L;C_hh__eq_Y_ls_valT_gr_L_gr_|:

.. das:function:: insert_default(tab: table<auto(keyT);auto(valT)>; key: keyT const|keyT const# const; value: valT const ==const|valT const# ==const const)

+--------+----------------------------+
+argument+argument type               +
+========+============================+
+tab     +table<auto(keyT);auto(valT)>+
+--------+----------------------------+
+key     +option const                +
+--------+----------------------------+
+value   +option const                +
+--------+----------------------------+


|function-builtin-insert_default|

.. _function-_at__builtin__c__c_insert_default_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|:

.. das:function:: insert_default(tab: table<auto(keyT);auto(valT)>; key: keyT const|keyT const# const)

+--------+----------------------------+
+argument+argument type               +
+========+============================+
+tab     +table<auto(keyT);auto(valT)>+
+--------+----------------------------+
+key     +option const                +
+--------+----------------------------+


|function-builtin-insert_default|

.. _function-_at__builtin__c__c_emplace_default_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|:

.. das:function:: emplace_default(tab: table<auto(keyT);auto(valT)>; key: keyT const|keyT const# const)

+--------+----------------------------+
+argument+argument type               +
+========+============================+
+tab     +table<auto(keyT);auto(valT)>+
+--------+----------------------------+
+key     +option const                +
+--------+----------------------------+


|function-builtin-emplace_default|

.. _function-_at__builtin__c__c_get_with_default__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_CN_ls_p_gr_0_ls_&Y_ls_valT_gr_L_gr_1_ls_v_gr__builtin_:

.. das:function:: get_with_default(Tab: table<auto(keyT);auto(valT)> ==const; at: keyT const|keyT const# const; blk: block<(var p:valT&):void> const)

+--------+-----------------------------+
+argument+argument type                +
+========+=============================+
+Tab     +table<auto(keyT);auto(valT)>!+
+--------+-----------------------------+
+at      +option const                 +
+--------+-----------------------------+
+blk     +block<(p:valT&):void> const  +
+--------+-----------------------------+


|function-builtin-get_with_default|

.. _function-_at__builtin__c__c_modify__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_CN_ls_p_gr_0_ls_C&Y_ls_valT_gr_L_gr_1_ls_Y_ls_valT_gr_L_gr__builtin_:

.. das:function:: modify(Tab: table<auto(keyT);auto(valT)> ==const; at: keyT const|keyT const# const; blk: block<(p:valT const&):valT> const)

+--------+---------------------------------+
+argument+argument type                    +
+========+=================================+
+Tab     +table<auto(keyT);auto(valT)>!    +
+--------+---------------------------------+
+at      +option const                     +
+--------+---------------------------------+
+blk     +block<(p:valT const&):valT> const+
+--------+---------------------------------+


|function-builtin-modify|

.. _function-_at__builtin__c__c_key_exists_C0_ls_C1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T;C_hh_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_gr_|_C_hh_s:

.. das:function:: key_exists(Tab: table<auto(keyT);auto(valT)> const|table<auto(keyT);auto(valT)> const# const; at: string const#)

key_exists returns bool

+--------+-------------+
+argument+argument type+
+========+=============+
+Tab     +option const +
+--------+-------------+
+at      +string const#+
+--------+-------------+


|function-builtin-key_exists|

.. _function-_at__builtin__c__c_key_exists_C0_ls_C1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T;C_hh_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_gr_|_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|:

.. das:function:: key_exists(Tab: table<auto(keyT);auto(valT)> const|table<auto(keyT);auto(valT)> const# const; at: keyT const|keyT const# const)

key_exists returns bool

+--------+-------------+
+argument+argument type+
+========+=============+
+Tab     +option const +
+--------+-------------+
+at      +option const +
+--------+-------------+


|function-builtin-key_exists|

.. _function-_at__builtin__c__c_copy_to_local_CY_ls_TT_gr_.:

.. das:function:: copy_to_local(a: auto(TT) const)

copy_to_local returns TT

+--------+--------------+
+argument+argument type +
+========+==============+
+a       +auto(TT) const+
+--------+--------------+


|function-builtin-copy_to_local|

.. _function-_at__builtin__c__c_move_to_local_&Y_ls_TT_gr_.:

.. das:function:: move_to_local(a: auto(TT)&)

move_to_local returns TT

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +auto(TT)&    +
+--------+-------------+


|function-builtin-move_to_local|

.. _function-_at__builtin__c__c_keys_C0_ls_C_eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T;C_hh__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_gr_|:

.. das:function:: keys(a: table<auto(keyT);auto(valT)> const ==const|table<auto(keyT);auto(valT)> const# ==const const)

keys returns iterator<keyT>

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +option const +
+--------+-------------+


|function-builtin-keys|

.. _function-_at__builtin__c__c_keys_0_ls__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T;_hh__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_gr_|:

.. das:function:: keys(a: table<auto(keyT);auto(valT)> ==const|table<auto(keyT);auto(valT)># ==const)

keys returns iterator<keyT>

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +option       +
+--------+-------------+


|function-builtin-keys|

.. _function-_at__builtin__c__c_values_C0_ls_C_eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_v_gr_T;C_hh__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_v_gr_T_gr_|:

.. das:function:: values(a: table<auto(keyT);void> const ==const|table<auto(keyT);void> const# ==const const)

values returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +option const +
+--------+-------------+


|function-builtin-values|

.. _function-_at__builtin__c__c_values_0_ls__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_v_gr_T;_hh__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_v_gr_T_gr_|:

.. das:function:: values(a: table<auto(keyT);void> ==const|table<auto(keyT);void># ==const)

values returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +option       +
+--------+-------------+


|function-builtin-values|

.. _function-_at__builtin__c__c_values_C0_ls_C_eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T;C_hh__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_gr_|:

.. das:function:: values(a: table<auto(keyT);auto(valT)> const ==const|table<auto(keyT);auto(valT)> const# ==const const)

values returns iterator<valT const&>

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +option const +
+--------+-------------+


|function-builtin-values|

.. _function-_at__builtin__c__c_values_0_ls__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T;_hh__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_gr_|:

.. das:function:: values(a: table<auto(keyT);auto(valT)> ==const|table<auto(keyT);auto(valT)># ==const)

values returns iterator<valT&>

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +option       +
+--------+-------------+


|function-builtin-values|

.. _function-_at__builtin__c__c_values_C0_ls_C_eq_1_ls_Y_ls_keyT_gr_._gr_2_ls__lb_-1_rb_Y_ls_valT_gr_._gr_T;C_hh__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls__lb_-1_rb_Y_ls_valT_gr_._gr_T_gr_|:

.. das:function:: values(a: table<auto(keyT);auto(valT)[]> const ==const|table<auto(keyT);auto(valT)[]> const# ==const const)

values returns iterator<valT const[-2]&>

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +option const +
+--------+-------------+


|function-builtin-values|

.. _function-_at__builtin__c__c_values_0_ls__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls__lb_-1_rb_Y_ls_valT_gr_._gr_T;_hh__eq_1_ls_Y_ls_keyT_gr_._gr_2_ls__lb_-1_rb_Y_ls_valT_gr_._gr_T_gr_|:

.. das:function:: values(a: table<auto(keyT);auto(valT)[]> ==const|table<auto(keyT);auto(valT)[]># ==const)

values returns iterator<valT[-2]&>

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +option       +
+--------+-------------+


|function-builtin-values|

.. _function-_at__builtin__c__c_lock_C0_ls_C1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T;C_hh_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_gr_|_CN_ls_t_gr_0_ls_C_hh_1_ls_Y_ls_keyT_gr_L_gr_2_ls_Y_ls_valT_gr_L_gr_T_gr_1_ls_v_gr__builtin_:

.. das:function:: lock(Tab: table<auto(keyT);auto(valT)> const|table<auto(keyT);auto(valT)> const# const; blk: block<(t:table<keyT;valT> const#):void> const)

lock returns auto

+--------+---------------------------------------------+
+argument+argument type                                +
+========+=============================================+
+Tab     +option const                                 +
+--------+---------------------------------------------+
+blk     +block<(t:table<keyT;valT> const#):void> const+
+--------+---------------------------------------------+


|function-builtin-lock|

.. _function-_at__builtin__c__c_lock_forever_0_ls_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T;_hh_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_gr_|:

.. das:function:: lock_forever(Tab: table<auto(keyT);auto(valT)>|table<auto(keyT);auto(valT)>#)

lock_forever returns table<keyT;valT>#

+--------+-------------+
+argument+argument type+
+========+=============+
+Tab     +option       +
+--------+-------------+


|function-builtin-lock_forever|

.. _function-_at__builtin__c__c_next_1_ls_Y_ls_TT_gr_._gr_G_&Y_ls_TT_gr_L:

.. das:function:: next(it: iterator<auto(TT)>; value: TT&)

next returns bool

+--------+------------------+
+argument+argument type     +
+========+==================+
+it      +iterator<auto(TT)>+
+--------+------------------+
+value   +TT&               +
+--------+------------------+


|function-builtin-next|

.. _function-_at__builtin__c__c_each_Cr:

.. das:function:: each(rng: range const)

each returns iterator<int>

+--------+-------------+
+argument+argument type+
+========+=============+
+rng     +range const  +
+--------+-------------+


|function-builtin-each|

.. _function-_at__builtin__c__c_each_Cs:

.. das:function:: each(str: string const)

each returns iterator<int>

+--------+-------------+
+argument+argument type+
+========+=============+
+str     +string const +
+--------+-------------+


|function-builtin-each|

.. _function-_at__builtin__c__c_each_C_lb_-1_rb_Y_ls_TT_gr_.:

.. das:function:: each(a: auto(TT) const[])

each returns iterator<TT&>

+--------+------------------+
+argument+argument type     +
+========+==================+
+a       +auto(TT) const[-1]+
+--------+------------------+


|function-builtin-each|

.. _function-_at__builtin__c__c_each_C1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: each(a: array<auto(TT)> const)

each returns iterator<TT&>

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+a       +array<auto(TT)> const+
+--------+---------------------+


|function-builtin-each|

.. _function-_at__builtin__c__c_each_C_hh_1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: each(a: array<auto(TT)> const#)

each returns iterator<TT&#>

+--------+----------------------+
+argument+argument type         +
+========+======================+
+a       +array<auto(TT)> const#+
+--------+----------------------+


|function-builtin-each|

.. _function-_at__builtin__c__c_each_CN_ls_arg_gr_0_ls_Y_ls_argT_gr_._gr_1_ls_b_gr__at_:

.. das:function:: each(lam: lambda<(var arg:auto(argT)):bool> const)

each returns iterator<argT>

+--------+-----------------------------------+
+argument+argument type                      +
+========+===================================+
+lam     +lambda<(arg:auto(argT)):bool> const+
+--------+-----------------------------------+


|function-builtin-each|

.. _function-_at__builtin__c__c_each_ref_CN_ls_arg_gr_0_ls_1_ls_Y_ls_argT_gr_._gr__qm__gr_1_ls_b_gr__at_:

.. das:function:: each_ref(lam: lambda<(var arg:auto(argT)?):bool> const)

each_ref returns iterator<argT&>

+--------+------------------------------------+
+argument+argument type                       +
+========+====================================+
+lam     +lambda<(arg:auto(argT)?):bool> const+
+--------+------------------------------------+


|function-builtin-each_ref|

.. _function-_at__builtin__c__c_each_enum_CY_ls_TT_gr_.:

.. das:function:: each_enum(tt: auto(TT) const)

each_enum returns iterator<TT>

+--------+--------------+
+argument+argument type +
+========+==============+
+tt      +auto(TT) const+
+--------+--------------+


|function-builtin-each_enum|

.. _function-_at__builtin__c__c_nothing_1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: nothing(it: iterator<auto(TT)>)

nothing returns iterator<TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+it      +iterator<auto(TT)>+
+--------+------------------+


|function-builtin-nothing|

.. _function-_at__builtin__c__c_to_array_1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: to_array(it: iterator<auto(TT)>)

to_array returns array<TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+it      +iterator<auto(TT)>+
+--------+------------------+


|function-builtin-to_array|

.. _function-_at__builtin__c__c_to_array_C_lb_-1_rb_Y_ls_TT_gr_.:

.. das:function:: to_array(a: auto(TT) const[])

to_array returns array<TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+a       +auto(TT) const[-1]+
+--------+------------------+


|function-builtin-to_array|

.. _function-_at__builtin__c__c_to_array_move__lb_-1_rb_Y_ls_TT_gr_.:

.. das:function:: to_array_move(a: auto(TT)[])

to_array_move returns array<TT>

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +auto(TT)[-1] +
+--------+-------------+


|function-builtin-to_array_move|

.. _function-_at__builtin__c__c_to_array_move_C_eq_Y_ls_TT_gr_.:

.. das:function:: to_array_move(a: auto(TT) const ==const)

to_array_move returns array<TT>

+--------+---------------+
+argument+argument type  +
+========+===============+
+a       +auto(TT) const!+
+--------+---------------+


|function-builtin-to_array_move|

.. _function-_at__builtin__c__c_to_array_move__eq_Y_ls_TT_gr_.:

.. das:function:: to_array_move(a: auto(TT) ==const)

to_array_move returns array<TT>

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +auto(TT)!    +
+--------+-------------+


|function-builtin-to_array_move|

.. _function-_at__builtin__c__c_to_table_C_lb_-1_rb_0_ls_Y_ls_keyT_gr_.;Y_ls_valT_gr_._gr_U:

.. das:function:: to_table(a: tuple<auto(keyT);auto(valT)> const[])

to_table returns table<keyT;valT>

+--------+--------------------------------------+
+argument+argument type                         +
+========+======================================+
+a       +tuple<auto(keyT);auto(valT)> const[-1]+
+--------+--------------------------------------+


|function-builtin-to_table|

.. _function-_at__builtin__c__c_to_table_C_lb_-1_rb_Y_ls_keyT_gr_.:

.. das:function:: to_table(a: auto(keyT) const[])

to_table returns table<keyT;void>

+--------+--------------------+
+argument+argument type       +
+========+====================+
+a       +auto(keyT) const[-1]+
+--------+--------------------+


|function-builtin-to_table|

.. _function-_at__builtin__c__c_to_table_move_C_lb_-1_rb_Y_ls_keyT_gr_.:

.. das:function:: to_table_move(a: auto(keyT) const[])

to_table_move returns table<keyT;void>

+--------+--------------------+
+argument+argument type       +
+========+====================+
+a       +auto(keyT) const[-1]+
+--------+--------------------+


|function-builtin-to_table_move|

.. _function-_at__builtin__c__c_to_table_move_C1_ls_Y_ls_keyT_gr_._gr_A:

.. das:function:: to_table_move(a: array<auto(keyT)> const)

to_table_move returns table<keyT;void>

+--------+-----------------------+
+argument+argument type          +
+========+=======================+
+a       +array<auto(keyT)> const+
+--------+-----------------------+


|function-builtin-to_table_move|

.. _function-_at__builtin__c__c_to_table_move_CY_ls_keyT_gr_.:

.. das:function:: to_table_move(a: auto(keyT) const)

to_table_move returns table<keyT;void>

+--------+----------------+
+argument+argument type   +
+========+================+
+a       +auto(keyT) const+
+--------+----------------+


|function-builtin-to_table_move|

.. _function-_at__builtin__c__c_to_table_move_0_ls_Y_ls_keyT_gr_.;Y_ls_valT_gr_._gr_U:

.. das:function:: to_table_move(a: tuple<auto(keyT);auto(valT)>)

to_table_move returns table<keyT;valT>

+--------+----------------------------+
+argument+argument type               +
+========+============================+
+a       +tuple<auto(keyT);auto(valT)>+
+--------+----------------------------+


|function-builtin-to_table_move|

.. _function-_at__builtin__c__c_to_table_move__lb_-1_rb_0_ls_Y_ls_keyT_gr_.;Y_ls_valT_gr_._gr_U:

.. das:function:: to_table_move(a: tuple<auto(keyT);auto(valT)>[])

to_table_move returns table<keyT;valT>

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+a       +tuple<auto(keyT);auto(valT)>[-1]+
+--------+--------------------------------+


|function-builtin-to_table_move|

.. _function-_at__builtin__c__c_to_table_move_1_ls_0_ls_Y_ls_keyT_gr_.;Y_ls_valT_gr_._gr_U_gr_A:

.. das:function:: to_table_move(a: array<tuple<auto(keyT);auto(valT)>>)

to_table_move returns table<keyT;valT>

+--------+-----------------------------------+
+argument+argument type                      +
+========+===================================+
+a       +array<tuple<auto(keyT);auto(valT)>>+
+--------+-----------------------------------+


|function-builtin-to_table_move|

.. _function-_at__builtin__c__c_sort_0_ls__lb_-1_rb_Y_ls_TT_gr_.;_hh__lb_-1_rb_Y_ls_TT_gr_._gr_|:

.. das:function:: sort(a: auto(TT)[]|auto(TT)[]#)

sort returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +option       +
+--------+-------------+


|function-builtin-sort|

.. _function-_at__builtin__c__c_sort_0_ls_1_ls_Y_ls_TT_gr_._gr_A;_hh_1_ls_Y_ls_TT_gr_._gr_A_gr_|:

.. das:function:: sort(a: array<auto(TT)>|array<auto(TT)>#)

sort returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +option       +
+--------+-------------+


|function-builtin-sort|

.. _function-_at__builtin__c__c_sort_0_ls__lb_-1_rb_Y_ls_TT_gr_.;_hh__lb_-1_rb_Y_ls_TT_gr_._gr_|_CN_ls_x;y_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: sort(a: auto(TT)[]|auto(TT)[]#; cmp: block<(x:TT const;y:TT const):bool> const)

sort returns auto

+--------+-----------------------------------------+
+argument+argument type                            +
+========+=========================================+
+a       +option                                   +
+--------+-----------------------------------------+
+cmp     +block<(x:TT const;y:TT const):bool> const+
+--------+-----------------------------------------+


|function-builtin-sort|

.. _function-_at__builtin__c__c_sort_0_ls_1_ls_Y_ls_TT_gr_._gr_A;_hh_1_ls_Y_ls_TT_gr_._gr_A_gr_|_CN_ls_x;y_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: sort(a: array<auto(TT)>|array<auto(TT)>#; cmp: block<(x:TT const;y:TT const):bool> const)

sort returns auto

+--------+-----------------------------------------+
+argument+argument type                            +
+========+=========================================+
+a       +option                                   +
+--------+-----------------------------------------+
+cmp     +block<(x:TT const;y:TT const):bool> const+
+--------+-----------------------------------------+


|function-builtin-sort|

.. _function-_at__builtin__c__c_lock_0_ls__eq_1_ls_Y_ls_TT_gr_._gr_A;_hh__eq_1_ls_Y_ls_TT_gr_._gr_A_gr_|_CN_ls_x_gr_0_ls__hh_1_ls_Y_ls_TT_gr_L_gr_A_gr_1_ls_._gr__builtin_:

.. das:function:: lock(a: array<auto(TT)> ==const|array<auto(TT)># ==const; blk: block<(var x:array<TT>#):auto> const)

lock returns auto

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+a       +option                          +
+--------+--------------------------------+
+blk     +block<(x:array<TT>#):auto> const+
+--------+--------------------------------+


|function-builtin-lock|

.. _function-_at__builtin__c__c_lock_C0_ls_C_eq_1_ls_Y_ls_TT_gr_._gr_A;C_hh__eq_1_ls_Y_ls_TT_gr_._gr_A_gr_|_CN_ls_x_gr_0_ls_C_hh_1_ls_Y_ls_TT_gr_L_gr_A_gr_1_ls_._gr__builtin_:

.. das:function:: lock(a: array<auto(TT)> const ==const|array<auto(TT)> const# ==const const; blk: block<(x:array<TT> const#):auto> const)

lock returns auto

+--------+--------------------------------------+
+argument+argument type                         +
+========+======================================+
+a       +option const                          +
+--------+--------------------------------------+
+blk     +block<(x:array<TT> const#):auto> const+
+--------+--------------------------------------+


|function-builtin-lock|

.. _function-_at__builtin__c__c_find_index_C0_ls_C1_ls_Y_ls_TT_gr_._gr_A;C_hh_1_ls_Y_ls_TT_gr_._gr_A_gr_|_CY_ls_TT_gr_L:

.. das:function:: find_index(arr: array<auto(TT)> const|array<auto(TT)> const# const; key: TT const)

find_index returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+arr     +option const +
+--------+-------------+
+key     +TT const     +
+--------+-------------+


|function-builtin-find_index|

.. _function-_at__builtin__c__c_find_index_C0_ls_C_lb_-1_rb_Y_ls_TT_gr_.;C_hh__lb_-1_rb_Y_ls_TT_gr_._gr_|_CY_ls_TT_gr_L:

.. das:function:: find_index(arr: auto(TT) const[]|auto(TT) const[]# const; key: TT const)

find_index returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+arr     +option const +
+--------+-------------+
+key     +TT const     +
+--------+-------------+


|function-builtin-find_index|

.. _function-_at__builtin__c__c_find_index_1_ls_Y_ls_TT_gr_._gr_G_CY_ls_TT_gr_L:

.. das:function:: find_index(arr: iterator<auto(TT)>; key: TT const)

find_index returns auto

+--------+------------------+
+argument+argument type     +
+========+==================+
+arr     +iterator<auto(TT)>+
+--------+------------------+
+key     +TT const          +
+--------+------------------+


|function-builtin-find_index|

.. _function-_at__builtin__c__c_find_index_if_C0_ls_C1_ls_Y_ls_TT_gr_._gr_A;C_hh_1_ls_Y_ls_TT_gr_._gr_A_gr_|_CN_ls_key_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: find_index_if(arr: array<auto(TT)> const|array<auto(TT)> const# const; blk: block<(key:TT const):bool> const)

find_index_if returns auto

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+arr     +option const                    +
+--------+--------------------------------+
+blk     +block<(key:TT const):bool> const+
+--------+--------------------------------+


|function-builtin-find_index_if|

.. _function-_at__builtin__c__c_find_index_if_C0_ls_C_lb_-1_rb_Y_ls_TT_gr_.;C_hh__lb_-1_rb_Y_ls_TT_gr_._gr_|_CN_ls_key_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: find_index_if(arr: auto(TT) const[]|auto(TT) const[]# const; blk: block<(key:TT const):bool> const)

find_index_if returns auto

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+arr     +option const                    +
+--------+--------------------------------+
+blk     +block<(key:TT const):bool> const+
+--------+--------------------------------+


|function-builtin-find_index_if|

.. _function-_at__builtin__c__c_find_index_if_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_key_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: find_index_if(arr: iterator<auto(TT)>; blk: block<(key:TT const):bool> const)

find_index_if returns auto

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+arr     +iterator<auto(TT)>              +
+--------+--------------------------------+
+blk     +block<(key:TT const):bool> const+
+--------+--------------------------------+


|function-builtin-find_index_if|

.. _function-_at__builtin__c__c_has_value_C._C.:

.. das:function:: has_value(a: auto const; key: auto const)

has_value returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +auto const   +
+--------+-------------+
+key     +auto const   +
+--------+-------------+


|function-builtin-has_value|

.. _function-_at__builtin__c__c_has_value_1_ls_._gr_G_C.:

.. das:function:: has_value(a: iterator<auto>; key: auto const)

has_value returns auto

+--------+--------------+
+argument+argument type +
+========+==============+
+a       +iterator<auto>+
+--------+--------------+
+key     +auto const    +
+--------+--------------+


|function-builtin-has_value|

.. _function-_at__builtin__c__c_subarray_C_lb_-1_rb_Y_ls_TT_gr_._Cr:

.. das:function:: subarray(a: auto(TT) const[]; r: range const)

subarray returns auto

+--------+------------------+
+argument+argument type     +
+========+==================+
+a       +auto(TT) const[-1]+
+--------+------------------+
+r       +range const       +
+--------+------------------+


|function-builtin-subarray|

.. _function-_at__builtin__c__c_subarray_C_lb_-1_rb_Y_ls_TT_gr_._Cz:

.. das:function:: subarray(a: auto(TT) const[]; r: urange const)

subarray returns auto

+--------+------------------+
+argument+argument type     +
+========+==================+
+a       +auto(TT) const[-1]+
+--------+------------------+
+r       +urange const      +
+--------+------------------+


|function-builtin-subarray|

.. _function-_at__builtin__c__c_subarray_C_eq_1_ls_Y_ls_TT_gr_._gr_A_Cr:

.. das:function:: subarray(a: array<auto(TT)> const ==const; r: range const)

subarray returns auto

+--------+----------------------+
+argument+argument type         +
+========+======================+
+a       +array<auto(TT)> const!+
+--------+----------------------+
+r       +range const           +
+--------+----------------------+


|function-builtin-subarray|

.. _function-_at__builtin__c__c_subarray__eq_1_ls_Y_ls_TT_gr_._gr_A_Cr:

.. das:function:: subarray(a: array<auto(TT)> ==const; r: range const)

subarray returns auto

+--------+----------------+
+argument+argument type   +
+========+================+
+a       +array<auto(TT)>!+
+--------+----------------+
+r       +range const     +
+--------+----------------+


|function-builtin-subarray|

.. _function-_at__builtin__c__c_subarray_C1_ls_Y_ls_TT_gr_._gr_A_Cz:

.. das:function:: subarray(a: array<auto(TT)> const; r: urange const)

subarray returns auto

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+a       +array<auto(TT)> const+
+--------+---------------------+
+r       +urange const         +
+--------+---------------------+


|function-builtin-subarray|

.. _function-_at__builtin__c__c_move_to_ref_&._.:

.. das:function:: move_to_ref(a: auto&; b: auto)

move_to_ref returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +auto&        +
+--------+-------------+
+b       +auto         +
+--------+-------------+


|function-builtin-move_to_ref|

.. _function-_at__builtin__c__c_clear_1_ls_Y_ls_KT_gr_._gr_2_ls_Y_ls_VT_gr_._gr_T:

.. das:function:: clear(t: table<auto(KT);auto(VT)>)

clear returns auto

+--------+------------------------+
+argument+argument type           +
+========+========================+
+t       +table<auto(KT);auto(VT)>+
+--------+------------------------+


|function-builtin-clear|

++++++++++++++++++++++++
das::string manipulation
++++++++++++++++++++++++

  *  :ref:`peek (src:$::das_string const implicit;block:block\<(arg0:string const#):void\> const implicit;context:__context const;line:__lineInfo const) : void <function-_at__builtin__c__c_peek_CIH_ls__builtin__c__c_das_string_gr__CI0_ls_C_hh_s_gr_1_ls_v_gr__builtin__C_c_C_l>` 

.. _function-_at__builtin__c__c_peek_CIH_ls__builtin__c__c_das_string_gr__CI0_ls_C_hh_s_gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: peek(src: das_string const implicit; block: block<(arg0:string const#):void> const implicit)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+src     + :ref:`builtin::das_string <handle-builtin-das_string>`  const implicit+
+--------+-----------------------------------------------------------------------+
+block   +block<(string const#):void> const implicit                             +
+--------+-----------------------------------------------------------------------+


|function-builtin-peek|

++++++++++++++
Heap reporting
++++++++++++++

  *  :ref:`heap_allocation_stats (context:__context const) : urange64 const <function-_at__builtin__c__c_heap_allocation_stats_C_c>` 
  *  :ref:`heap_allocation_count (context:__context const) : uint64 const <function-_at__builtin__c__c_heap_allocation_count_C_c>` 
  *  :ref:`string_heap_allocation_stats (context:__context const) : urange64 const <function-_at__builtin__c__c_string_heap_allocation_stats_C_c>` 
  *  :ref:`string_heap_allocation_count (context:__context const) : uint64 const <function-_at__builtin__c__c_string_heap_allocation_count_C_c>` 
  *  :ref:`heap_bytes_allocated (context:__context const) : uint64 const <function-_at__builtin__c__c_heap_bytes_allocated_C_c>` 
  *  :ref:`heap_depth (context:__context const) : int const <function-_at__builtin__c__c_heap_depth_C_c>` 
  *  :ref:`string_heap_bytes_allocated (context:__context const) : uint64 const <function-_at__builtin__c__c_string_heap_bytes_allocated_C_c>` 
  *  :ref:`string_heap_depth (context:__context const) : int const <function-_at__builtin__c__c_string_heap_depth_C_c>` 
  *  :ref:`heap_collect (string_heap:bool const;validate:bool const;context:__context const;at:__lineInfo const) : void <function-_at__builtin__c__c_heap_collect_Cb_Cb_C_c_C_l>` 
  *  :ref:`string_heap_report (context:__context const;line:__lineInfo const) : void <function-_at__builtin__c__c_string_heap_report_C_c_C_l>` 
  *  :ref:`heap_report (context:__context const;line:__lineInfo const) : void <function-_at__builtin__c__c_heap_report_C_c_C_l>` 
  *  :ref:`memory_report (errorsOnly:bool const;context:__context const;lineinfo:__lineInfo const) : void <function-_at__builtin__c__c_memory_report_Cb_C_c_C_l>` 

.. _function-_at__builtin__c__c_heap_allocation_stats_C_c:

.. das:function:: heap_allocation_stats()

heap_allocation_stats returns urange64 const

|function-builtin-heap_allocation_stats|

.. _function-_at__builtin__c__c_heap_allocation_count_C_c:

.. das:function:: heap_allocation_count()

heap_allocation_count returns uint64 const

|function-builtin-heap_allocation_count|

.. _function-_at__builtin__c__c_string_heap_allocation_stats_C_c:

.. das:function:: string_heap_allocation_stats()

string_heap_allocation_stats returns urange64 const

|function-builtin-string_heap_allocation_stats|

.. _function-_at__builtin__c__c_string_heap_allocation_count_C_c:

.. das:function:: string_heap_allocation_count()

string_heap_allocation_count returns uint64 const

|function-builtin-string_heap_allocation_count|

.. _function-_at__builtin__c__c_heap_bytes_allocated_C_c:

.. das:function:: heap_bytes_allocated()

heap_bytes_allocated returns uint64 const

|function-builtin-heap_bytes_allocated|

.. _function-_at__builtin__c__c_heap_depth_C_c:

.. das:function:: heap_depth()

heap_depth returns int const

|function-builtin-heap_depth|

.. _function-_at__builtin__c__c_string_heap_bytes_allocated_C_c:

.. das:function:: string_heap_bytes_allocated()

string_heap_bytes_allocated returns uint64 const

|function-builtin-string_heap_bytes_allocated|

.. _function-_at__builtin__c__c_string_heap_depth_C_c:

.. das:function:: string_heap_depth()

string_heap_depth returns int const

|function-builtin-string_heap_depth|

.. _function-_at__builtin__c__c_heap_collect_Cb_Cb_C_c_C_l:

.. das:function:: heap_collect(string_heap: bool const; validate: bool const)

.. warning:: 
  This is unsafe operation.

+-----------+-------------+
+argument   +argument type+
+===========+=============+
+string_heap+bool const   +
+-----------+-------------+
+validate   +bool const   +
+-----------+-------------+


|function-builtin-heap_collect|

.. _function-_at__builtin__c__c_string_heap_report_C_c_C_l:

.. das:function:: string_heap_report()

|function-builtin-string_heap_report|

.. _function-_at__builtin__c__c_heap_report_C_c_C_l:

.. das:function:: heap_report()

|function-builtin-heap_report|

.. _function-_at__builtin__c__c_memory_report_Cb_C_c_C_l:

.. das:function:: memory_report(errorsOnly: bool const)

+----------+-------------+
+argument  +argument type+
+==========+=============+
+errorsOnly+bool const   +
+----------+-------------+


|function-builtin-memory_report|

++++++++++++++++++
GC0 infrastructure
++++++++++++++++++

  *  :ref:`gc0_save_ptr (name:string const implicit;data:void? const implicit;context:__context const;line:__lineInfo const) : void <function-_at__builtin__c__c_gc0_save_ptr_CIs_CI_qm__C_c_C_l>` 
  *  :ref:`gc0_save_smart_ptr (name:string const implicit;data:smart_ptr\<void\> const implicit;context:__context const;line:__lineInfo const) : void <function-_at__builtin__c__c_gc0_save_smart_ptr_CIs_CI1_ls_v_gr__qm_M_C_c_C_l>` 
  *  :ref:`gc0_restore_ptr (name:string const implicit;context:__context const) : void? <function-_at__builtin__c__c_gc0_restore_ptr_CIs_C_c>` 
  *  :ref:`gc0_restore_smart_ptr (name:string const implicit;context:__context const) : smart_ptr\<void\> <function-_at__builtin__c__c_gc0_restore_smart_ptr_CIs_C_c>` 
  *  :ref:`gc0_reset () : void <function-_at__builtin__c__c_gc0_reset>` 

.. _function-_at__builtin__c__c_gc0_save_ptr_CIs_CI_qm__C_c_C_l:

.. das:function:: gc0_save_ptr(name: string const implicit; data: void? const implicit)

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+name    +string const implicit+
+--------+---------------------+
+data    +void? const implicit +
+--------+---------------------+


|function-builtin-gc0_save_ptr|

.. _function-_at__builtin__c__c_gc0_save_smart_ptr_CIs_CI1_ls_v_gr__qm_M_C_c_C_l:

.. das:function:: gc0_save_smart_ptr(name: string const implicit; data: smart_ptr<void> const implicit)

+--------+------------------------------+
+argument+argument type                 +
+========+==============================+
+name    +string const implicit         +
+--------+------------------------------+
+data    +smart_ptr<void> const implicit+
+--------+------------------------------+


|function-builtin-gc0_save_smart_ptr|

.. _function-_at__builtin__c__c_gc0_restore_ptr_CIs_C_c:

.. das:function:: gc0_restore_ptr(name: string const implicit)

gc0_restore_ptr returns void?

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+name    +string const implicit+
+--------+---------------------+


|function-builtin-gc0_restore_ptr|

.. _function-_at__builtin__c__c_gc0_restore_smart_ptr_CIs_C_c:

.. das:function:: gc0_restore_smart_ptr(name: string const implicit)

gc0_restore_smart_ptr returns smart_ptr<void>

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+name    +string const implicit+
+--------+---------------------+


|function-builtin-gc0_restore_smart_ptr|

.. _function-_at__builtin__c__c_gc0_reset:

.. das:function:: gc0_reset()

|function-builtin-gc0_reset|

++++++++++++++++++++++++
Smart ptr infrastructure
++++++++++++++++++++++++

  *  :ref:`move_new (dest:smart_ptr\<void\>& implicit;src:smart_ptr\<void\> const implicit;context:__context const;at:__lineInfo const) : void <function-_at__builtin__c__c_move_new_&I1_ls_v_gr__qm_M_CI1_ls_v_gr__qm_M_C_c_C_l>` 
  *  :ref:`move (dest:smart_ptr\<void\>& implicit;src:void? const implicit;context:__context const;at:__lineInfo const) : void <function-_at__builtin__c__c_move_&I1_ls_v_gr__qm_M_CI_qm__C_c_C_l>` 
  *  :ref:`move (dest:smart_ptr\<void\>& implicit;src:smart_ptr\<void\>& implicit;context:__context const;at:__lineInfo const) : void <function-_at__builtin__c__c_move_&I1_ls_v_gr__qm_M_&I1_ls_v_gr__qm_M_C_c_C_l>` 
  *  :ref:`smart_ptr_clone (dest:smart_ptr\<void\>& implicit;src:void? const implicit;context:__context const;at:__lineInfo const) : void <function-_at__builtin__c__c_smart_ptr_clone_&I1_ls_v_gr__qm_M_CI_qm__C_c_C_l>` 
  *  :ref:`smart_ptr_clone (dest:smart_ptr\<void\>& implicit;src:smart_ptr\<void\> const implicit;context:__context const;at:__lineInfo const) : void <function-_at__builtin__c__c_smart_ptr_clone_&I1_ls_v_gr__qm_M_CI1_ls_v_gr__qm_M_C_c_C_l>` 
  *  :ref:`smart_ptr_use_count (ptr:smart_ptr\<void\> const implicit;context:__context const;at:__lineInfo const) : uint const <function-_at__builtin__c__c_smart_ptr_use_count_CI1_ls_v_gr__qm_M_C_c_C_l>` 
  *  :ref:`smart_ptr_is_valid (dest:smart_ptr\<void\> const implicit) : bool const <function-_at__builtin__c__c_smart_ptr_is_valid_CI1_ls_v_gr__qm_M>` 
  *  :ref:`get_ptr (src:smart_ptr\<auto(TT)\> const ==const) : TT? const <function-_at__builtin__c__c_get_ptr_C_eq_1_ls_Y_ls_TT_gr_._gr__qm_M>` 
  *  :ref:`get_ptr (src:smart_ptr\<auto(TT)\> ==const -const) : TT? <function-_at__builtin__c__c_get_ptr__eq_1_ls_Y_ls_TT_gr_._gr__qm_M>` 
  *  :ref:`get_const_ptr (src:smart_ptr\<auto(TT)\> const) : TT? const <function-_at__builtin__c__c_get_const_ptr_C1_ls_Y_ls_TT_gr_._gr__qm_M>` 
  *  :ref:`add_ptr_ref (src:smart_ptr\<auto(TT)\> const) : smart_ptr\<TT\> <function-_at__builtin__c__c_add_ptr_ref_C1_ls_Y_ls_TT_gr_._gr__qm_M>` 

.. _function-_at__builtin__c__c_move_new_&I1_ls_v_gr__qm_M_CI1_ls_v_gr__qm_M_C_c_C_l:

.. das:function:: move_new(dest: smart_ptr<void>& implicit; src: smart_ptr<void> const implicit)

+--------+------------------------------+
+argument+argument type                 +
+========+==============================+
+dest    +smart_ptr<void>& implicit     +
+--------+------------------------------+
+src     +smart_ptr<void> const implicit+
+--------+------------------------------+


|function-builtin-move_new|

.. _function-_at__builtin__c__c_move_&I1_ls_v_gr__qm_M_CI_qm__C_c_C_l:

.. das:function:: move(dest: smart_ptr<void>& implicit; src: void? const implicit)

+--------+-------------------------+
+argument+argument type            +
+========+=========================+
+dest    +smart_ptr<void>& implicit+
+--------+-------------------------+
+src     +void? const implicit     +
+--------+-------------------------+


|function-builtin-move|

.. _function-_at__builtin__c__c_move_&I1_ls_v_gr__qm_M_&I1_ls_v_gr__qm_M_C_c_C_l:

.. das:function:: move(dest: smart_ptr<void>& implicit; src: smart_ptr<void>& implicit)

+--------+-------------------------+
+argument+argument type            +
+========+=========================+
+dest    +smart_ptr<void>& implicit+
+--------+-------------------------+
+src     +smart_ptr<void>& implicit+
+--------+-------------------------+


|function-builtin-move|

.. _function-_at__builtin__c__c_smart_ptr_clone_&I1_ls_v_gr__qm_M_CI_qm__C_c_C_l:

.. das:function:: smart_ptr_clone(dest: smart_ptr<void>& implicit; src: void? const implicit)

+--------+-------------------------+
+argument+argument type            +
+========+=========================+
+dest    +smart_ptr<void>& implicit+
+--------+-------------------------+
+src     +void? const implicit     +
+--------+-------------------------+


|function-builtin-smart_ptr_clone|

.. _function-_at__builtin__c__c_smart_ptr_clone_&I1_ls_v_gr__qm_M_CI1_ls_v_gr__qm_M_C_c_C_l:

.. das:function:: smart_ptr_clone(dest: smart_ptr<void>& implicit; src: smart_ptr<void> const implicit)

+--------+------------------------------+
+argument+argument type                 +
+========+==============================+
+dest    +smart_ptr<void>& implicit     +
+--------+------------------------------+
+src     +smart_ptr<void> const implicit+
+--------+------------------------------+


|function-builtin-smart_ptr_clone|

.. _function-_at__builtin__c__c_smart_ptr_use_count_CI1_ls_v_gr__qm_M_C_c_C_l:

.. das:function:: smart_ptr_use_count(ptr: smart_ptr<void> const implicit)

smart_ptr_use_count returns uint const

+--------+------------------------------+
+argument+argument type                 +
+========+==============================+
+ptr     +smart_ptr<void> const implicit+
+--------+------------------------------+


|function-builtin-smart_ptr_use_count|

.. _function-_at__builtin__c__c_smart_ptr_is_valid_CI1_ls_v_gr__qm_M:

.. das:function:: smart_ptr_is_valid(dest: smart_ptr<void> const implicit)

smart_ptr_is_valid returns bool const

+--------+------------------------------+
+argument+argument type                 +
+========+==============================+
+dest    +smart_ptr<void> const implicit+
+--------+------------------------------+


|function-builtin-smart_ptr_is_valid|

.. _function-_at__builtin__c__c_get_ptr_C_eq_1_ls_Y_ls_TT_gr_._gr__qm_M:

.. das:function:: get_ptr(src: smart_ptr<auto(TT)> const ==const)

get_ptr returns TT? const

+--------+--------------------------+
+argument+argument type             +
+========+==========================+
+src     +smart_ptr<auto(TT)> const!+
+--------+--------------------------+


|function-builtin-get_ptr|

.. _function-_at__builtin__c__c_get_ptr__eq_1_ls_Y_ls_TT_gr_._gr__qm_M:

.. das:function:: get_ptr(src: smart_ptr<auto(TT)> ==const)

get_ptr returns TT?

+--------+--------------------+
+argument+argument type       +
+========+====================+
+src     +smart_ptr<auto(TT)>!+
+--------+--------------------+


|function-builtin-get_ptr|

.. _function-_at__builtin__c__c_get_const_ptr_C1_ls_Y_ls_TT_gr_._gr__qm_M:

.. das:function:: get_const_ptr(src: smart_ptr<auto(TT)> const)

get_const_ptr returns TT? const

+--------+-------------------------+
+argument+argument type            +
+========+=========================+
+src     +smart_ptr<auto(TT)> const+
+--------+-------------------------+


|function-builtin-get_const_ptr|

.. _function-_at__builtin__c__c_add_ptr_ref_C1_ls_Y_ls_TT_gr_._gr__qm_M:

.. das:function:: add_ptr_ref(src: smart_ptr<auto(TT)> const)

add_ptr_ref returns smart_ptr<TT>

+--------+-------------------------+
+argument+argument type            +
+========+=========================+
+src     +smart_ptr<auto(TT)> const+
+--------+-------------------------+


|function-builtin-add_ptr_ref|

++++++++++++++++++++
Macro infrastructure
++++++++++++++++++++

  *  :ref:`is_compiling () : bool const <function-_at__builtin__c__c_is_compiling>` 
  *  :ref:`is_compiling_macros () : bool const <function-_at__builtin__c__c_is_compiling_macros>` 
  *  :ref:`is_compiling_macros_in_module (name:string const implicit) : bool const <function-_at__builtin__c__c_is_compiling_macros_in_module_CIs>` 
  *  :ref:`is_reporting_compilation_errors () : bool const <function-_at__builtin__c__c_is_reporting_compilation_errors>` 
  *  :ref:`is_in_completion () : bool const <function-_at__builtin__c__c_is_in_completion>` 
  *  :ref:`is_folding () : bool const <function-_at__builtin__c__c_is_folding>` 

.. _function-_at__builtin__c__c_is_compiling:

.. das:function:: is_compiling()

is_compiling returns bool const

|function-builtin-is_compiling|

.. _function-_at__builtin__c__c_is_compiling_macros:

.. das:function:: is_compiling_macros()

is_compiling_macros returns bool const

|function-builtin-is_compiling_macros|

.. _function-_at__builtin__c__c_is_compiling_macros_in_module_CIs:

.. das:function:: is_compiling_macros_in_module(name: string const implicit)

is_compiling_macros_in_module returns bool const

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+name    +string const implicit+
+--------+---------------------+


|function-builtin-is_compiling_macros_in_module|

.. _function-_at__builtin__c__c_is_reporting_compilation_errors:

.. das:function:: is_reporting_compilation_errors()

is_reporting_compilation_errors returns bool const

|function-builtin-is_reporting_compilation_errors|

.. _function-_at__builtin__c__c_is_in_completion:

.. das:function:: is_in_completion()

is_in_completion returns bool const

|function-builtin-is_in_completion|

.. _function-_at__builtin__c__c_is_folding:

.. das:function:: is_folding()

is_folding returns bool const

|function-builtin-is_folding|

++++++++
Profiler
++++++++

  *  :ref:`reset_profiler (context:__context const) : void <function-_at__builtin__c__c_reset_profiler_C_c>` 
  *  :ref:`dump_profile_info (context:__context const) : void <function-_at__builtin__c__c_dump_profile_info_C_c>` 
  *  :ref:`collect_profile_info (context:__context const;at:__lineInfo const) : string const <function-_at__builtin__c__c_collect_profile_info_C_c_C_l>` 
  *  :ref:`profile (count:int const;category:string const implicit;block:block\<\> const implicit;context:__context const;line:__lineInfo const) : float const <function-_at__builtin__c__c_profile_Ci_CIs_CI_builtin__C_c_C_l>` 

.. _function-_at__builtin__c__c_reset_profiler_C_c:

.. das:function:: reset_profiler()

|function-builtin-reset_profiler|

.. _function-_at__builtin__c__c_dump_profile_info_C_c:

.. das:function:: dump_profile_info()

|function-builtin-dump_profile_info|

.. _function-_at__builtin__c__c_collect_profile_info_C_c_C_l:

.. das:function:: collect_profile_info()

collect_profile_info returns string const

|function-builtin-collect_profile_info|

.. _function-_at__builtin__c__c_profile_Ci_CIs_CI_builtin__C_c_C_l:

.. das:function:: profile(count: int const; category: string const implicit; block: block<> const implicit)

profile returns float const

+--------+----------------------+
+argument+argument type         +
+========+======================+
+count   +int const             +
+--------+----------------------+
+category+string const implicit +
+--------+----------------------+
+block   +block<> const implicit+
+--------+----------------------+


|function-builtin-profile|

++++++++++++++++++++
System infastructure
++++++++++++++++++++

  *  :ref:`get_das_root (context:__context const;at:__lineInfo const) : string const <function-_at__builtin__c__c_get_das_root_C_c_C_l>` 
  *  :ref:`panic (text:string const implicit;context:__context const;at:__lineInfo const) : void <function-_at__builtin__c__c_panic_CIs_C_c_C_l>` 
  *  :ref:`print (text:string const implicit;context:__context const;at:__lineInfo const) : void <function-_at__builtin__c__c_print_CIs_C_c_C_l>` 
  *  :ref:`error (text:string const implicit;context:__context const;at:__lineInfo const) : void <function-_at__builtin__c__c_error_CIs_C_c_C_l>` 
  *  :ref:`sprint (value:any;flags:bitfield\<escapeString;namesAndDimensions;typeQualifiers;refAddresses;singleLine;fixedPoint\> const) : string const <function-_at__builtin__c__c_sprint_*_CY_ls_print_flags_gr_N_ls_escapeString;namesAndDimensions;typeQualifiers;refAddresses;singleLine;fixedPoint_gr_t>` 
  *  :ref:`sprint_json (value:any;humanReadable:bool const) : string const <function-_at__builtin__c__c_sprint_json_*_Cb>` 
  *  :ref:`terminate (context:__context const;at:__lineInfo const) : void <function-_at__builtin__c__c_terminate_C_c_C_l>` 
  *  :ref:`breakpoint () : void <function-_at__builtin__c__c_breakpoint>` 
  *  :ref:`stackwalk (args:bool const;vars:bool const;context:__context const;lineinfo:__lineInfo const) : void <function-_at__builtin__c__c_stackwalk_Cb_Cb_C_c_C_l>` 
  *  :ref:`is_intern_strings (context:__context const) : bool const <function-_at__builtin__c__c_is_intern_strings_C_c>` 
  *  :ref:`is_in_aot () : bool const <function-_at__builtin__c__c_is_in_aot>` 
  *  :ref:`to_log (level:int const;text:string const implicit;context:__context const;at:__lineInfo const) : void <function-_at__builtin__c__c_to_log_Ci_CIs_C_c_C_l>` 
  *  :ref:`to_compiler_log (text:string const implicit;context:__context const;at:__lineInfo const) : void <function-_at__builtin__c__c_to_compiler_log_CIs_C_c_C_l>` 
  *  :ref:`eval_main_loop (block:block\<bool\> const implicit;context:__context const;at:__lineInfo const) : void <function-_at__builtin__c__c_eval_main_loop_CI1_ls_b_gr__builtin__C_c_C_l>` 
  *  :ref:`aot_enabled (context:__context const;at:__lineInfo const) : bool const <function-_at__builtin__c__c_aot_enabled_C_c_C_l>` 

.. _function-_at__builtin__c__c_get_das_root_C_c_C_l:

.. das:function:: get_das_root()

get_das_root returns string const

|function-builtin-get_das_root|

.. _function-_at__builtin__c__c_panic_CIs_C_c_C_l:

.. das:function:: panic(text: string const implicit)

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+text    +string const implicit+
+--------+---------------------+


|function-builtin-panic|

.. _function-_at__builtin__c__c_print_CIs_C_c_C_l:

.. das:function:: print(text: string const implicit)

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+text    +string const implicit+
+--------+---------------------+


|function-builtin-print|

.. _function-_at__builtin__c__c_error_CIs_C_c_C_l:

.. das:function:: error(text: string const implicit)

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+text    +string const implicit+
+--------+---------------------+


|function-builtin-error|

.. _function-_at__builtin__c__c_sprint_*_CY_ls_print_flags_gr_N_ls_escapeString;namesAndDimensions;typeQualifiers;refAddresses;singleLine;fixedPoint_gr_t:

.. das:function:: sprint(value: any; flags: print_flags)

sprint returns string const

+--------+----------------------------------------+
+argument+argument type                           +
+========+========================================+
+value   +any                                     +
+--------+----------------------------------------+
+flags   + :ref:`print_flags <alias-print_flags>` +
+--------+----------------------------------------+


|function-builtin-sprint|

.. _function-_at__builtin__c__c_sprint_json_*_Cb:

.. das:function:: sprint_json(value: any; humanReadable: bool const)

sprint_json returns string const

+-------------+-------------+
+argument     +argument type+
+=============+=============+
+value        +any          +
+-------------+-------------+
+humanReadable+bool const   +
+-------------+-------------+


|function-builtin-sprint_json|

.. _function-_at__builtin__c__c_terminate_C_c_C_l:

.. das:function:: terminate()

|function-builtin-terminate|

.. _function-_at__builtin__c__c_breakpoint:

.. das:function:: breakpoint()

|function-builtin-breakpoint|

.. _function-_at__builtin__c__c_stackwalk_Cb_Cb_C_c_C_l:

.. das:function:: stackwalk(args: bool const; vars: bool const)

+--------+-------------+
+argument+argument type+
+========+=============+
+args    +bool const   +
+--------+-------------+
+vars    +bool const   +
+--------+-------------+


|function-builtin-stackwalk|

.. _function-_at__builtin__c__c_is_intern_strings_C_c:

.. das:function:: is_intern_strings()

is_intern_strings returns bool const

|function-builtin-is_intern_strings|

.. _function-_at__builtin__c__c_is_in_aot:

.. das:function:: is_in_aot()

is_in_aot returns bool const

|function-builtin-is_in_aot|

.. _function-_at__builtin__c__c_to_log_Ci_CIs_C_c_C_l:

.. das:function:: to_log(level: int const; text: string const implicit)

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+level   +int const            +
+--------+---------------------+
+text    +string const implicit+
+--------+---------------------+


|function-builtin-to_log|

.. _function-_at__builtin__c__c_to_compiler_log_CIs_C_c_C_l:

.. das:function:: to_compiler_log(text: string const implicit)

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+text    +string const implicit+
+--------+---------------------+


|function-builtin-to_compiler_log|

.. _function-_at__builtin__c__c_eval_main_loop_CI1_ls_b_gr__builtin__C_c_C_l:

.. das:function:: eval_main_loop(block: block<bool> const implicit)

+--------+----------------------+
+argument+argument type         +
+========+======================+
+block   +block<> const implicit+
+--------+----------------------+


|function-builtin-eval_main_loop|

.. _function-_at__builtin__c__c_aot_enabled_C_c_C_l:

.. das:function:: aot_enabled()

aot_enabled returns bool const

|function-builtin-aot_enabled|

+++++++++++++++++++
Memory manipulation
+++++++++++++++++++

  *  :ref:`variant_index (arg0:variant\<\> const implicit) : int const <function-_at__builtin__c__c_variant_index_CIV>` 
  *  :ref:`set_variant_index (variant:variant\<\> implicit;index:int const) : void <function-_at__builtin__c__c_set_variant_index_IV_Ci>` 
  *  :ref:`hash (data:any) : uint64 const <function-_at__builtin__c__c_hash_*>` 
  *  :ref:`hash (data:string const implicit) : uint64 const <function-_at__builtin__c__c_hash_CIs>` 
  *  :ref:`hash (value:int8 const) : uint64 const <function-_at__builtin__c__c_hash_Ci8>` 
  *  :ref:`hash (value:uint8 const) : uint64 const <function-_at__builtin__c__c_hash_Cu8>` 
  *  :ref:`hash (value:int16 const) : uint64 const <function-_at__builtin__c__c_hash_Ci16>` 
  *  :ref:`hash (value:uint16 const) : uint64 const <function-_at__builtin__c__c_hash_Cu16>` 
  *  :ref:`hash (value:int const) : uint64 const <function-_at__builtin__c__c_hash_Ci>` 
  *  :ref:`hash (value:uint const) : uint64 const <function-_at__builtin__c__c_hash_Cu>` 
  *  :ref:`hash (value:int64 const) : uint64 const <function-_at__builtin__c__c_hash_Ci64>` 
  *  :ref:`hash (value:uint64 const) : uint64 const <function-_at__builtin__c__c_hash_Cu64>` 
  *  :ref:`hash (value:void? const implicit) : uint64 const <function-_at__builtin__c__c_hash_CI_qm_>` 
  *  :ref:`hash (value:float const) : uint64 const <function-_at__builtin__c__c_hash_Cf>` 
  *  :ref:`hash (value:double const) : uint64 const <function-_at__builtin__c__c_hash_Cd>` 
  *  :ref:`hash (value:$::das_string const implicit) : uint64 const <function-_at__builtin__c__c_hash_CIH_ls__builtin__c__c_das_string_gr_>` 
  *  :ref:`memcpy (left:void? const implicit;right:void? const implicit;size:int const) : void <function-_at__builtin__c__c_memcpy_CI_qm__CI_qm__Ci>` 
  *  :ref:`memcmp (left:void? const implicit;right:void? const implicit;size:int const) : int const <function-_at__builtin__c__c_memcmp_CI_qm__CI_qm__Ci>` 
  *  :ref:`intptr (p:void? const) : uint64 <function-_at__builtin__c__c_intptr_C1_ls_v_gr__qm_>` 
  *  :ref:`intptr (p:smart_ptr\<auto\> const) : uint64 <function-_at__builtin__c__c_intptr_C1_ls_._gr__qm_M>` 
  *  :ref:`lock_data (a:array\<auto(TT)\> ==const -const|array\<auto(TT)\># ==const -const -const;blk:block\<(var p:TT?# -const;s:int const):auto\> const) : auto <function-_at__builtin__c__c_lock_data_0_ls__eq_1_ls_Y_ls_TT_gr_._gr_A;_hh__eq_1_ls_Y_ls_TT_gr_._gr_A_gr_|_CN_ls_p;s_gr_0_ls__hh_1_ls_Y_ls_TT_gr_L_gr__qm_;Ci_gr_1_ls_._gr__builtin_>` 
  *  :ref:`lock_data (a:array\<auto(TT)\> const ==const|array\<auto(TT)\> const# ==const const;blk:block\<(p:TT const? const#;s:int const):auto\> const) : auto <function-_at__builtin__c__c_lock_data_C0_ls_C_eq_1_ls_Y_ls_TT_gr_._gr_A;C_hh__eq_1_ls_Y_ls_TT_gr_._gr_A_gr_|_CN_ls_p;s_gr_0_ls_C_hh_1_ls_CY_ls_TT_gr_L_gr__qm_;Ci_gr_1_ls_._gr__builtin_>` 
  *  :ref:`map_to_array (data:void? const;len:int const;blk:block\<(var arg:array\<auto(TT)\># -const):auto\> const) : auto <function-_at__builtin__c__c_map_to_array_C1_ls_v_gr__qm__Ci_CN_ls_arg_gr_0_ls__hh_1_ls_Y_ls_TT_gr_._gr_A_gr_1_ls_._gr__builtin_>` 
  *  :ref:`map_to_ro_array (data:void? const;len:int const;blk:block\<(arg:array\<auto(TT)\> const#):auto\> const) : auto <function-_at__builtin__c__c_map_to_ro_array_C1_ls_v_gr__qm__Ci_CN_ls_arg_gr_0_ls_C_hh_1_ls_Y_ls_TT_gr_._gr_A_gr_1_ls_._gr__builtin_>` 

.. _function-_at__builtin__c__c_variant_index_CIV:

.. das:function:: variant_index(arg0: variant<> const implicit)

variant_index returns int const

+--------+------------------------+
+argument+argument type           +
+========+========================+
+arg0    +variant<> const implicit+
+--------+------------------------+


|function-builtin-variant_index|

.. _function-_at__builtin__c__c_set_variant_index_IV_Ci:

.. das:function:: set_variant_index(variant: variant<> implicit; index: int const)

.. warning:: 
  This is unsafe operation.

+--------+------------------+
+argument+argument type     +
+========+==================+
+variant +variant<> implicit+
+--------+------------------+
+index   +int const         +
+--------+------------------+


|function-builtin-set_variant_index|

.. _function-_at__builtin__c__c_hash_*:

.. das:function:: hash(data: any)

hash returns uint64 const

+--------+-------------+
+argument+argument type+
+========+=============+
+data    +any          +
+--------+-------------+


|function-builtin-hash|

.. _function-_at__builtin__c__c_hash_CIs:

.. das:function:: hash(data: string const implicit)

hash returns uint64 const

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+data    +string const implicit+
+--------+---------------------+


|function-builtin-hash|

.. _function-_at__builtin__c__c_hash_Ci8:

.. das:function:: hash(value: int8 const)

hash returns uint64 const

+--------+-------------+
+argument+argument type+
+========+=============+
+value   +int8 const   +
+--------+-------------+


|function-builtin-hash|

.. _function-_at__builtin__c__c_hash_Cu8:

.. das:function:: hash(value: uint8 const)

hash returns uint64 const

+--------+-------------+
+argument+argument type+
+========+=============+
+value   +uint8 const  +
+--------+-------------+


|function-builtin-hash|

.. _function-_at__builtin__c__c_hash_Ci16:

.. das:function:: hash(value: int16 const)

hash returns uint64 const

+--------+-------------+
+argument+argument type+
+========+=============+
+value   +int16 const  +
+--------+-------------+


|function-builtin-hash|

.. _function-_at__builtin__c__c_hash_Cu16:

.. das:function:: hash(value: uint16 const)

hash returns uint64 const

+--------+-------------+
+argument+argument type+
+========+=============+
+value   +uint16 const +
+--------+-------------+


|function-builtin-hash|

.. _function-_at__builtin__c__c_hash_Ci:

.. das:function:: hash(value: int const)

hash returns uint64 const

+--------+-------------+
+argument+argument type+
+========+=============+
+value   +int const    +
+--------+-------------+


|function-builtin-hash|

.. _function-_at__builtin__c__c_hash_Cu:

.. das:function:: hash(value: uint const)

hash returns uint64 const

+--------+-------------+
+argument+argument type+
+========+=============+
+value   +uint const   +
+--------+-------------+


|function-builtin-hash|

.. _function-_at__builtin__c__c_hash_Ci64:

.. das:function:: hash(value: int64 const)

hash returns uint64 const

+--------+-------------+
+argument+argument type+
+========+=============+
+value   +int64 const  +
+--------+-------------+


|function-builtin-hash|

.. _function-_at__builtin__c__c_hash_Cu64:

.. das:function:: hash(value: uint64 const)

hash returns uint64 const

+--------+-------------+
+argument+argument type+
+========+=============+
+value   +uint64 const +
+--------+-------------+


|function-builtin-hash|

.. _function-_at__builtin__c__c_hash_CI_qm_:

.. das:function:: hash(value: void? const implicit)

hash returns uint64 const

+--------+--------------------+
+argument+argument type       +
+========+====================+
+value   +void? const implicit+
+--------+--------------------+


|function-builtin-hash|

.. _function-_at__builtin__c__c_hash_Cf:

.. das:function:: hash(value: float const)

hash returns uint64 const

+--------+-------------+
+argument+argument type+
+========+=============+
+value   +float const  +
+--------+-------------+


|function-builtin-hash|

.. _function-_at__builtin__c__c_hash_Cd:

.. das:function:: hash(value: double const)

hash returns uint64 const

+--------+-------------+
+argument+argument type+
+========+=============+
+value   +double const +
+--------+-------------+


|function-builtin-hash|

.. _function-_at__builtin__c__c_hash_CIH_ls__builtin__c__c_das_string_gr_:

.. das:function:: hash(value: das_string const implicit)

hash returns uint64 const

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+value   + :ref:`builtin::das_string <handle-builtin-das_string>`  const implicit+
+--------+-----------------------------------------------------------------------+


|function-builtin-hash|

.. _function-_at__builtin__c__c_memcpy_CI_qm__CI_qm__Ci:

.. das:function:: memcpy(left: void? const implicit; right: void? const implicit; size: int const)

.. warning:: 
  This is unsafe operation.

+--------+--------------------+
+argument+argument type       +
+========+====================+
+left    +void? const implicit+
+--------+--------------------+
+right   +void? const implicit+
+--------+--------------------+
+size    +int const           +
+--------+--------------------+


|function-builtin-memcpy|

.. _function-_at__builtin__c__c_memcmp_CI_qm__CI_qm__Ci:

.. das:function:: memcmp(left: void? const implicit; right: void? const implicit; size: int const)

memcmp returns int const

.. warning:: 
  This is unsafe operation.

+--------+--------------------+
+argument+argument type       +
+========+====================+
+left    +void? const implicit+
+--------+--------------------+
+right   +void? const implicit+
+--------+--------------------+
+size    +int const           +
+--------+--------------------+


|function-builtin-memcmp|

.. _function-_at__builtin__c__c_intptr_C1_ls_v_gr__qm_:

.. das:function:: intptr(p: void? const)

intptr returns uint64

+--------+-------------+
+argument+argument type+
+========+=============+
+p       +void? const  +
+--------+-------------+


|function-builtin-intptr|

.. _function-_at__builtin__c__c_intptr_C1_ls_._gr__qm_M:

.. das:function:: intptr(p: smart_ptr<auto> const)

intptr returns uint64

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+p       +smart_ptr<auto> const+
+--------+---------------------+


|function-builtin-intptr|

.. _function-_at__builtin__c__c_lock_data_0_ls__eq_1_ls_Y_ls_TT_gr_._gr_A;_hh__eq_1_ls_Y_ls_TT_gr_._gr_A_gr_|_CN_ls_p;s_gr_0_ls__hh_1_ls_Y_ls_TT_gr_L_gr__qm_;Ci_gr_1_ls_._gr__builtin_:

.. das:function:: lock_data(a: array<auto(TT)> ==const|array<auto(TT)># ==const; blk: block<(var p:TT?#;s:int const):auto> const)

lock_data returns auto

+--------+--------------------------------------+
+argument+argument type                         +
+========+======================================+
+a       +option                                +
+--------+--------------------------------------+
+blk     +block<(p:TT?#;s:int const):auto> const+
+--------+--------------------------------------+


|function-builtin-lock_data|

.. _function-_at__builtin__c__c_lock_data_C0_ls_C_eq_1_ls_Y_ls_TT_gr_._gr_A;C_hh__eq_1_ls_Y_ls_TT_gr_._gr_A_gr_|_CN_ls_p;s_gr_0_ls_C_hh_1_ls_CY_ls_TT_gr_L_gr__qm_;Ci_gr_1_ls_._gr__builtin_:

.. das:function:: lock_data(a: array<auto(TT)> const ==const|array<auto(TT)> const# ==const const; blk: block<(p:TT const? const#;s:int const):auto> const)

lock_data returns auto

+--------+--------------------------------------------------+
+argument+argument type                                     +
+========+==================================================+
+a       +option const                                      +
+--------+--------------------------------------------------+
+blk     +block<(p:TT const? const#;s:int const):auto> const+
+--------+--------------------------------------------------+


|function-builtin-lock_data|

.. _function-_at__builtin__c__c_map_to_array_C1_ls_v_gr__qm__Ci_CN_ls_arg_gr_0_ls__hh_1_ls_Y_ls_TT_gr_._gr_A_gr_1_ls_._gr__builtin_:

.. das:function:: map_to_array(data: void? const; len: int const; blk: block<(var arg:array<auto(TT)>#):auto> const)

map_to_array returns auto

.. warning:: 
  This is unsafe operation.

+--------+----------------------------------------+
+argument+argument type                           +
+========+========================================+
+data    +void? const                             +
+--------+----------------------------------------+
+len     +int const                               +
+--------+----------------------------------------+
+blk     +block<(arg:array<auto(TT)>#):auto> const+
+--------+----------------------------------------+


|function-builtin-map_to_array|

.. _function-_at__builtin__c__c_map_to_ro_array_C1_ls_v_gr__qm__Ci_CN_ls_arg_gr_0_ls_C_hh_1_ls_Y_ls_TT_gr_._gr_A_gr_1_ls_._gr__builtin_:

.. das:function:: map_to_ro_array(data: void? const; len: int const; blk: block<(arg:array<auto(TT)> const#):auto> const)

map_to_ro_array returns auto

.. warning:: 
  This is unsafe operation.

+--------+----------------------------------------------+
+argument+argument type                                 +
+========+==============================================+
+data    +void? const                                   +
+--------+----------------------------------------------+
+len     +int const                                     +
+--------+----------------------------------------------+
+blk     +block<(arg:array<auto(TT)> const#):auto> const+
+--------+----------------------------------------------+


|function-builtin-map_to_ro_array|

+++++++++++++++++
Binary serializer
+++++++++++++++++

  *  :ref:`binary_save (obj:auto const;subexpr:block\<(data:array\<uint8\> const#):void\> const) : auto <function-_at__builtin__c__c_binary_save_C._CN_ls_data_gr_0_ls_C_hh_1_ls_u8_gr_A_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`binary_load (obj:auto -const;data:array\<uint8\> const implicit) : auto <function-_at__builtin__c__c_binary_load_._CI1_ls_u8_gr_A>` 

.. _function-_at__builtin__c__c_binary_save_C._CN_ls_data_gr_0_ls_C_hh_1_ls_u8_gr_A_gr_1_ls_v_gr__builtin_:

.. das:function:: binary_save(obj: auto const; subexpr: block<(data:array<uint8> const#):void> const)

binary_save returns auto

+--------+--------------------------------------------+
+argument+argument type                               +
+========+============================================+
+obj     +auto const                                  +
+--------+--------------------------------------------+
+subexpr +block<(data:array<uint8> const#):void> const+
+--------+--------------------------------------------+


|function-builtin-binary_save|

.. _function-_at__builtin__c__c_binary_load_._CI1_ls_u8_gr_A:

.. das:function:: binary_load(obj: auto; data: array<uint8> const implicit)

binary_load returns auto

+--------+---------------------------+
+argument+argument type              +
+========+===========================+
+obj     +auto                       +
+--------+---------------------------+
+data    +array<uint8> const implicit+
+--------+---------------------------+


|function-builtin-binary_load|

+++++++++++++++++++++
Path and command line
+++++++++++++++++++++

  *  :ref:`get_command_line_arguments () : array\<string\> <function-_at__builtin__c__c_get_command_line_arguments>` 

.. _function-_at__builtin__c__c_get_command_line_arguments:

.. das:function:: get_command_line_arguments()

get_command_line_arguments returns array<string>

|function-builtin-get_command_line_arguments|

+++++++++++++
Time and date
+++++++++++++

  *  :ref:`get_clock () : $::clock <function-_at__builtin__c__c_get_clock>` 
  *  :ref:`mktime (year:int const;month:int const;mday:int const;hour:int const;min:int const;sec:int const) : $::clock <function-_at__builtin__c__c_mktime_Ci_Ci_Ci_Ci_Ci_Ci>` 
  *  :ref:`ref_time_ticks () : int64 const <function-_at__builtin__c__c_ref_time_ticks>` 
  *  :ref:`get_time_usec (ref:int64 const) : int const <function-_at__builtin__c__c_get_time_usec_Ci64>` 
  *  :ref:`get_time_nsec (ref:int64 const) : int64 const <function-_at__builtin__c__c_get_time_nsec_Ci64>` 

.. _function-_at__builtin__c__c_get_clock:

.. das:function:: get_clock()

get_clock returns  :ref:`builtin::clock <handle-builtin-clock>` 

|function-builtin-get_clock|

.. _function-_at__builtin__c__c_mktime_Ci_Ci_Ci_Ci_Ci_Ci:

.. das:function:: mktime(year: int const; month: int const; mday: int const; hour: int const; min: int const; sec: int const)

mktime returns  :ref:`builtin::clock <handle-builtin-clock>` 

+--------+-------------+
+argument+argument type+
+========+=============+
+year    +int const    +
+--------+-------------+
+month   +int const    +
+--------+-------------+
+mday    +int const    +
+--------+-------------+
+hour    +int const    +
+--------+-------------+
+min     +int const    +
+--------+-------------+
+sec     +int const    +
+--------+-------------+


|function-builtin-mktime|

.. _function-_at__builtin__c__c_ref_time_ticks:

.. das:function:: ref_time_ticks()

ref_time_ticks returns int64 const

|function-builtin-ref_time_ticks|

.. _function-_at__builtin__c__c_get_time_usec_Ci64:

.. das:function:: get_time_usec(ref: int64 const)

get_time_usec returns int const

+--------+-------------+
+argument+argument type+
+========+=============+
+ref     +int64 const  +
+--------+-------------+


|function-builtin-get_time_usec|

.. _function-_at__builtin__c__c_get_time_nsec_Ci64:

.. das:function:: get_time_nsec(ref: int64 const)

get_time_nsec returns int64 const

+--------+-------------+
+argument+argument type+
+========+=============+
+ref     +int64 const  +
+--------+-------------+


|function-builtin-get_time_nsec|

+++++++++++++
Lock checking
+++++++++++++

  *  :ref:`lock_count (array:array const implicit) : int const <function-_at__builtin__c__c_lock_count_CIA>` 
  *  :ref:`set_verify_array_locks (array:array implicit;check:bool const) : bool const <function-_at__builtin__c__c_set_verify_array_locks_IA_Cb>` 
  *  :ref:`set_verify_table_locks (table:table implicit;check:bool const) : bool const <function-_at__builtin__c__c_set_verify_table_locks_IT_Cb>` 

.. _function-_at__builtin__c__c_lock_count_CIA:

.. das:function:: lock_count(array: array const implicit)

lock_count returns int const

+--------+--------------------+
+argument+argument type       +
+========+====================+
+array   +array const implicit+
+--------+--------------------+


|function-builtin-lock_count|

.. _function-_at__builtin__c__c_set_verify_array_locks_IA_Cb:

.. das:function:: set_verify_array_locks(array: array implicit; check: bool const)

set_verify_array_locks returns bool const

.. warning:: 
  This is unsafe operation.

+--------+--------------+
+argument+argument type +
+========+==============+
+array   +array implicit+
+--------+--------------+
+check   +bool const    +
+--------+--------------+


|function-builtin-set_verify_array_locks|

.. _function-_at__builtin__c__c_set_verify_table_locks_IT_Cb:

.. das:function:: set_verify_table_locks(table: table implicit; check: bool const)

set_verify_table_locks returns bool const

.. warning:: 
  This is unsafe operation.

+--------+--------------+
+argument+argument type +
+========+==============+
+table   +table implicit+
+--------+--------------+
+check   +bool const    +
+--------+--------------+


|function-builtin-set_verify_table_locks|

+++++++++++++++++++++++
Lock checking internals
+++++++++++++++++++++++

  *  :ref:`_move_with_lockcheck (a:auto(valA)& -const;b:auto(valB)& -const) : auto <function-_at__builtin__c__c__move_with_lockcheck_&Y_ls_valA_gr_._&Y_ls_valB_gr_.>` 
  *  :ref:`_return_with_lockcheck (a:auto(valT)& ==const -const) : auto& <function-_at__builtin__c__c__return_with_lockcheck_&_eq_Y_ls_valT_gr_.>` 
  *  :ref:`_return_with_lockcheck (a:auto(valT) const& ==const) : auto& <function-_at__builtin__c__c__return_with_lockcheck_C&_eq_Y_ls_valT_gr_.>` 
  *  :ref:`_at_with_lockcheck (Tab:table\<auto(keyT);auto(valT)\> -const;at:keyT const|keyT const# const) : valT& <function-_at__builtin__c__c__at_with_lockcheck_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|>` 

.. _function-_at__builtin__c__c__move_with_lockcheck_&Y_ls_valA_gr_._&Y_ls_valB_gr_.:

.. das:function:: _move_with_lockcheck(a: auto(valA)&; b: auto(valB)&)

_move_with_lockcheck returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +auto(valA)&  +
+--------+-------------+
+b       +auto(valB)&  +
+--------+-------------+


|function-builtin-_move_with_lockcheck|

.. _function-_at__builtin__c__c__return_with_lockcheck_&_eq_Y_ls_valT_gr_.:

.. das:function:: _return_with_lockcheck(a: auto(valT)& ==const)

_return_with_lockcheck returns auto&

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +auto(valT)&! +
+--------+-------------+


|function-builtin-_return_with_lockcheck|

.. _function-_at__builtin__c__c__return_with_lockcheck_C&_eq_Y_ls_valT_gr_.:

.. das:function:: _return_with_lockcheck(a: auto(valT) const& ==const)

_return_with_lockcheck returns auto&

+--------+------------------+
+argument+argument type     +
+========+==================+
+a       +auto(valT) const&!+
+--------+------------------+


|function-builtin-_return_with_lockcheck|

.. _function-_at__builtin__c__c__at_with_lockcheck_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|:

.. das:function:: _at_with_lockcheck(Tab: table<auto(keyT);auto(valT)>; at: keyT const|keyT const# const)

_at_with_lockcheck returns valT&

+--------+----------------------------+
+argument+argument type               +
+========+============================+
+Tab     +table<auto(keyT);auto(valT)>+
+--------+----------------------------+
+at      +option const                +
+--------+----------------------------+


|function-builtin-_at_with_lockcheck|

++++++++++++++
Bit operations
++++++++++++++

  *  :ref:`clz (bits:uint const) : uint const <function-_at__builtin__c__c_clz_Cu>` 
  *  :ref:`clz (bits:uint64 const) : uint64 const <function-_at__builtin__c__c_clz_Cu64>` 
  *  :ref:`ctz (bits:uint const) : uint const <function-_at__builtin__c__c_ctz_Cu>` 
  *  :ref:`ctz (bits:uint64 const) : uint64 const <function-_at__builtin__c__c_ctz_Cu64>` 
  *  :ref:`popcnt (bits:uint const) : uint const <function-_at__builtin__c__c_popcnt_Cu>` 
  *  :ref:`popcnt (bits:uint64 const) : uint64 const <function-_at__builtin__c__c_popcnt_Cu64>` 
  *  :ref:`mul128 (a:uint64 const;b:uint64 const) : urange64 const <function-_at__builtin__c__c_mul128_Cu64_Cu64>` 
  *  :ref:`__bit_set (value:bitfield& implicit;mask:bitfield const;on:bool const) : void <function-_at__builtin__c__c___bit_set_&It_Ct_Cb>` 
  *  :ref:`__bit_set (value:bitfield8:uint8& implicit;mask:bitfield8:uint8 const;on:bool const) : void <function-_at__builtin__c__c___bit_set_&It8_Ct8_Cb>` 
  *  :ref:`__bit_set (value:bitfield16:uint16& implicit;mask:bitfield16:uint16 const;on:bool const) : void <function-_at__builtin__c__c___bit_set_&It16_Ct16_Cb>` 
  *  :ref:`__bit_set (value:bitfield64:uint64& implicit;mask:bitfield64:uint64 const;on:bool const) : void <function-_at__builtin__c__c___bit_set_&It64_Ct64_Cb>` 

.. _function-_at__builtin__c__c_clz_Cu:

.. das:function:: clz(bits: uint const)

clz returns uint const

+--------+-------------+
+argument+argument type+
+========+=============+
+bits    +uint const   +
+--------+-------------+


|function-builtin-clz|

.. _function-_at__builtin__c__c_clz_Cu64:

.. das:function:: clz(bits: uint64 const)

clz returns uint64 const

+--------+-------------+
+argument+argument type+
+========+=============+
+bits    +uint64 const +
+--------+-------------+


|function-builtin-clz|

.. _function-_at__builtin__c__c_ctz_Cu:

.. das:function:: ctz(bits: uint const)

ctz returns uint const

+--------+-------------+
+argument+argument type+
+========+=============+
+bits    +uint const   +
+--------+-------------+


|function-builtin-ctz|

.. _function-_at__builtin__c__c_ctz_Cu64:

.. das:function:: ctz(bits: uint64 const)

ctz returns uint64 const

+--------+-------------+
+argument+argument type+
+========+=============+
+bits    +uint64 const +
+--------+-------------+


|function-builtin-ctz|

.. _function-_at__builtin__c__c_popcnt_Cu:

.. das:function:: popcnt(bits: uint const)

popcnt returns uint const

+--------+-------------+
+argument+argument type+
+========+=============+
+bits    +uint const   +
+--------+-------------+


|function-builtin-popcnt|

.. _function-_at__builtin__c__c_popcnt_Cu64:

.. das:function:: popcnt(bits: uint64 const)

popcnt returns uint64 const

+--------+-------------+
+argument+argument type+
+========+=============+
+bits    +uint64 const +
+--------+-------------+


|function-builtin-popcnt|

.. _function-_at__builtin__c__c_mul128_Cu64_Cu64:

.. das:function:: mul128(a: uint64 const; b: uint64 const)

mul128 returns urange64 const

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +uint64 const +
+--------+-------------+
+b       +uint64 const +
+--------+-------------+


|function-builtin-mul128|

.. _function-_at__builtin__c__c___bit_set_&It_Ct_Cb:

.. das:function:: __bit_set(value: bitfield& implicit; mask: bitfield const; on: bool const)

+--------+--------------------+
+argument+argument type       +
+========+====================+
+value   +bitfield<>& implicit+
+--------+--------------------+
+mask    +bitfield<> const    +
+--------+--------------------+
+on      +bool const          +
+--------+--------------------+


|function-builtin-__bit_set|

.. _function-_at__builtin__c__c___bit_set_&It8_Ct8_Cb:

.. das:function:: __bit_set(value: bitfield8:uint8& implicit; mask: bitfield8:uint8 const; on: bool const)

+--------+-------------------+
+argument+argument type      +
+========+===================+
+value   +bitfield8& implicit+
+--------+-------------------+
+mask    +bitfield8 const    +
+--------+-------------------+
+on      +bool const         +
+--------+-------------------+


|function-builtin-__bit_set|

.. _function-_at__builtin__c__c___bit_set_&It16_Ct16_Cb:

.. das:function:: __bit_set(value: bitfield16:uint16& implicit; mask: bitfield16:uint16 const; on: bool const)

+--------+--------------------+
+argument+argument type       +
+========+====================+
+value   +bitfield16& implicit+
+--------+--------------------+
+mask    +bitfield16 const    +
+--------+--------------------+
+on      +bool const          +
+--------+--------------------+


|function-builtin-__bit_set|

.. _function-_at__builtin__c__c___bit_set_&It64_Ct64_Cb:

.. das:function:: __bit_set(value: bitfield64:uint64& implicit; mask: bitfield64:uint64 const; on: bool const)

+--------+--------------------+
+argument+argument type       +
+========+====================+
+value   +bitfield64& implicit+
+--------+--------------------+
+mask    +bitfield64 const    +
+--------+--------------------+
+on      +bool const          +
+--------+--------------------+


|function-builtin-__bit_set|

+++++++++
Intervals
+++++++++

  *  :ref:`interval (arg0:int const;arg1:int const) : range const <function-_at__builtin__c__c_interval_Ci_Ci>` 
  *  :ref:`interval (arg0:uint const;arg1:uint const) : urange const <function-_at__builtin__c__c_interval_Cu_Cu>` 
  *  :ref:`interval (arg0:int64 const;arg1:int64 const) : range64 const <function-_at__builtin__c__c_interval_Ci64_Ci64>` 
  *  :ref:`interval (arg0:uint64 const;arg1:uint64 const) : urange64 const <function-_at__builtin__c__c_interval_Cu64_Cu64>` 

.. _function-_at__builtin__c__c_interval_Ci_Ci:

.. das:function:: interval(arg0: int const; arg1: int const)

interval returns range const

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +int const    +
+--------+-------------+
+arg1    +int const    +
+--------+-------------+


|function-builtin-interval|

.. _function-_at__builtin__c__c_interval_Cu_Cu:

.. das:function:: interval(arg0: uint const; arg1: uint const)

interval returns urange const

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +uint const   +
+--------+-------------+
+arg1    +uint const   +
+--------+-------------+


|function-builtin-interval|

.. _function-_at__builtin__c__c_interval_Ci64_Ci64:

.. das:function:: interval(arg0: int64 const; arg1: int64 const)

interval returns range64 const

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +int64 const  +
+--------+-------------+
+arg1    +int64 const  +
+--------+-------------+


|function-builtin-interval|

.. _function-_at__builtin__c__c_interval_Cu64_Cu64:

.. das:function:: interval(arg0: uint64 const; arg1: uint64 const)

interval returns urange64 const

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +uint64 const +
+--------+-------------+
+arg1    +uint64 const +
+--------+-------------+


|function-builtin-interval|

++++
RTTI
++++

  *  :ref:`class_rtti_size (ptr:void? const implicit) : int const <function-_at__builtin__c__c_class_rtti_size_CI_qm_>` 

.. _function-_at__builtin__c__c_class_rtti_size_CI_qm_:

.. das:function:: class_rtti_size(ptr: void? const implicit)

class_rtti_size returns int const

+--------+--------------------+
+argument+argument type       +
+========+====================+
+ptr     +void? const implicit+
+--------+--------------------+


|function-builtin-class_rtti_size|

+++++++++++++++++
Lock verification
+++++++++++++++++

  *  :ref:`set_verify_context_locks (check:bool const;context:__context const) : bool const <function-_at__builtin__c__c_set_verify_context_locks_Cb_C_c>` 

.. _function-_at__builtin__c__c_set_verify_context_locks_Cb_C_c:

.. das:function:: set_verify_context_locks(check: bool const)

set_verify_context_locks returns bool const

.. warning:: 
  This is unsafe operation.

+--------+-------------+
+argument+argument type+
+========+=============+
+check   +bool const   +
+--------+-------------+


|function-builtin-set_verify_context_locks|

+++++++++++++++++++++++++++++++
Initialization and finalization
+++++++++++++++++++++++++++++++

  *  :ref:`using (arg0:block\<(var arg0:$::das_string explicit):void\> const implicit) : void <function-_at__builtin__c__c_using_CI0_ls_XH_ls__builtin__c__c_das_string_gr__gr_1_ls_v_gr__builtin_>` 

.. _function-_at__builtin__c__c_using_CI0_ls_XH_ls__builtin__c__c_das_string_gr__gr_1_ls_v_gr__builtin_:

.. das:function:: using(arg0: block<(var arg0:das_string):void> const implicit)

+--------+-------------------------------------------------------------------------------------+
+argument+argument type                                                                        +
+========+=====================================================================================+
+arg0    +block<( :ref:`builtin::das_string <handle-builtin-das_string>` ):void> const implicit+
+--------+-------------------------------------------------------------------------------------+


|function-builtin-using|

++++++++++
Algorithms
++++++++++

  *  :ref:`count (start:int const;step:int const;context:__context const;at:__lineInfo const) : iterator\<int\> <function-_at__builtin__c__c_count_Ci_Ci_C_c_C_l>` 
  *  :ref:`ucount (start:uint const;step:uint const;context:__context const;at:__lineInfo const) : iterator\<uint\> <function-_at__builtin__c__c_ucount_Cu_Cu_C_c_C_l>` 
  *  :ref:`iter_range (foo:auto const) : auto <function-_at__builtin__c__c_iter_range_C.>` 
  *  :ref:`swap (a:auto(TT)& -const;b:auto(TT)& -const) : auto <function-_at__builtin__c__c_swap_&Y_ls_TT_gr_._&Y_ls_TT_gr_.>` 

.. _function-_at__builtin__c__c_count_Ci_Ci_C_c_C_l:

.. das:function:: count(start: int const; step: int const)

count returns iterator<int>

+--------+-------------+
+argument+argument type+
+========+=============+
+start   +int const    +
+--------+-------------+
+step    +int const    +
+--------+-------------+


|function-builtin-count|

.. _function-_at__builtin__c__c_ucount_Cu_Cu_C_c_C_l:

.. das:function:: ucount(start: uint const; step: uint const)

ucount returns iterator<uint>

+--------+-------------+
+argument+argument type+
+========+=============+
+start   +uint const   +
+--------+-------------+
+step    +uint const   +
+--------+-------------+


|function-builtin-ucount|

.. _function-_at__builtin__c__c_iter_range_C.:

.. das:function:: iter_range(foo: auto const)

iter_range returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+foo     +auto const   +
+--------+-------------+


|function-builtin-iter_range|

.. _function-_at__builtin__c__c_swap_&Y_ls_TT_gr_._&Y_ls_TT_gr_.:

.. das:function:: swap(a: auto(TT)&; b: auto(TT)&)

swap returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +auto(TT)&    +
+--------+-------------+
+b       +auto(TT)&    +
+--------+-------------+


|function-builtin-swap|

++++++
Memset
++++++

  *  :ref:`memset8 (left:void? const implicit;value:uint8 const;count:int const) : void <function-_at__builtin__c__c_memset8_CI_qm__Cu8_Ci>` 
  *  :ref:`memset16 (left:void? const implicit;value:uint16 const;count:int const) : void <function-_at__builtin__c__c_memset16_CI_qm__Cu16_Ci>` 
  *  :ref:`memset32 (left:void? const implicit;value:uint const;count:int const) : void <function-_at__builtin__c__c_memset32_CI_qm__Cu_Ci>` 
  *  :ref:`memset64 (left:void? const implicit;value:uint64 const;count:int const) : void <function-_at__builtin__c__c_memset64_CI_qm__Cu64_Ci>` 
  *  :ref:`memset128 (left:void? const implicit;value:uint4 const;count:int const) : void <function-_at__builtin__c__c_memset128_CI_qm__Cu4_Ci>` 

.. _function-_at__builtin__c__c_memset8_CI_qm__Cu8_Ci:

.. das:function:: memset8(left: void? const implicit; value: uint8 const; count: int const)

.. warning:: 
  This is unsafe operation.

+--------+--------------------+
+argument+argument type       +
+========+====================+
+left    +void? const implicit+
+--------+--------------------+
+value   +uint8 const         +
+--------+--------------------+
+count   +int const           +
+--------+--------------------+


|function-builtin-memset8|

.. _function-_at__builtin__c__c_memset16_CI_qm__Cu16_Ci:

.. das:function:: memset16(left: void? const implicit; value: uint16 const; count: int const)

.. warning:: 
  This is unsafe operation.

+--------+--------------------+
+argument+argument type       +
+========+====================+
+left    +void? const implicit+
+--------+--------------------+
+value   +uint16 const        +
+--------+--------------------+
+count   +int const           +
+--------+--------------------+


|function-builtin-memset16|

.. _function-_at__builtin__c__c_memset32_CI_qm__Cu_Ci:

.. das:function:: memset32(left: void? const implicit; value: uint const; count: int const)

.. warning:: 
  This is unsafe operation.

+--------+--------------------+
+argument+argument type       +
+========+====================+
+left    +void? const implicit+
+--------+--------------------+
+value   +uint const          +
+--------+--------------------+
+count   +int const           +
+--------+--------------------+


|function-builtin-memset32|

.. _function-_at__builtin__c__c_memset64_CI_qm__Cu64_Ci:

.. das:function:: memset64(left: void? const implicit; value: uint64 const; count: int const)

.. warning:: 
  This is unsafe operation.

+--------+--------------------+
+argument+argument type       +
+========+====================+
+left    +void? const implicit+
+--------+--------------------+
+value   +uint64 const        +
+--------+--------------------+
+count   +int const           +
+--------+--------------------+


|function-builtin-memset64|

.. _function-_at__builtin__c__c_memset128_CI_qm__Cu4_Ci:

.. das:function:: memset128(left: void? const implicit; value: uint4 const; count: int const)

.. warning:: 
  This is unsafe operation.

+--------+--------------------+
+argument+argument type       +
+========+====================+
+left    +void? const implicit+
+--------+--------------------+
+value   +uint4 const         +
+--------+--------------------+
+count   +int const           +
+--------+--------------------+


|function-builtin-memset128|

++++++
Malloc
++++++

  *  :ref:`malloc (size:uint64 const) : void? <function-_at__builtin__c__c_malloc_Cu64>` 
  *  :ref:`free (ptr:void? const implicit) : void <function-_at__builtin__c__c_free_CI_qm_>` 
  *  :ref:`malloc_usable_size (ptr:void? const implicit) : uint64 const <function-_at__builtin__c__c_malloc_usable_size_CI_qm_>` 

.. _function-_at__builtin__c__c_malloc_Cu64:

.. das:function:: malloc(size: uint64 const)

malloc returns void?

.. warning:: 
  This is unsafe operation.

+--------+-------------+
+argument+argument type+
+========+=============+
+size    +uint64 const +
+--------+-------------+


|function-builtin-malloc|

.. _function-_at__builtin__c__c_free_CI_qm_:

.. das:function:: free(ptr: void? const implicit)

.. warning:: 
  This is unsafe operation.

+--------+--------------------+
+argument+argument type       +
+========+====================+
+ptr     +void? const implicit+
+--------+--------------------+


|function-builtin-free|

.. _function-_at__builtin__c__c_malloc_usable_size_CI_qm_:

.. das:function:: malloc_usable_size(ptr: void? const implicit)

malloc_usable_size returns uint64 const

.. warning:: 
  This is unsafe operation.

+--------+--------------------+
+argument+argument type       +
+========+====================+
+ptr     +void? const implicit+
+--------+--------------------+


|function-builtin-malloc_usable_size|

+++++++++++++++++++
Compilation and AOT
+++++++++++++++++++

  *  :ref:`set_aot () : void <function-_at__builtin__c__c_set_aot>` 
  *  :ref:`reset_aot () : void <function-_at__builtin__c__c_reset_aot>` 
  *  :ref:`compiling_file_name () : string const <function-_at__builtin__c__c_compiling_file_name>` 
  *  :ref:`compiling_module_name () : string const <function-_at__builtin__c__c_compiling_module_name>` 

.. _function-_at__builtin__c__c_set_aot:

.. das:function:: set_aot()

|function-builtin-set_aot|

.. _function-_at__builtin__c__c_reset_aot:

.. das:function:: reset_aot()

|function-builtin-reset_aot|

.. _function-_at__builtin__c__c_compiling_file_name:

.. das:function:: compiling_file_name()

compiling_file_name returns string const

|function-builtin-compiling_file_name|

.. _function-_at__builtin__c__c_compiling_module_name:

.. das:function:: compiling_module_name()

compiling_module_name returns string const

|function-builtin-compiling_module_name|


