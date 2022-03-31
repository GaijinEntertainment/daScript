
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
+humanReadable     +4  +16   +
+------------------+---+-----+
+singleLine        +5  +32   +
+------------------+---+-----+


|typedef-builtin-print_flags|

+++++++++
Constants
+++++++++

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

.. das:attribute:: LONG_MIN = -2147483648

|variable-builtin-LONG_MIN|

.. _global-builtin-LONG_MAX:

.. das:attribute:: LONG_MAX = 2147483647

|variable-builtin-LONG_MAX|

.. _global-builtin-ULONG_MAX:

.. das:attribute:: ULONG_MAX = 0xffffffff

|variable-builtin-ULONG_MAX|

.. _global-builtin-FLT_MIN:

.. das:attribute:: FLT_MIN = 1.17549e-38f

|variable-builtin-FLT_MIN|

.. _global-builtin-FLT_MAX:

.. das:attribute:: FLT_MAX = 3.40282e+38f

|variable-builtin-FLT_MAX|

.. _global-builtin-DBL_MIN:

.. das:attribute:: DBL_MIN = 2.22507e-308lf

|variable-builtin-DBL_MIN|

.. _global-builtin-DBL_MAX:

.. das:attribute:: DBL_MAX = 1.79769e+308lf

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

.. _global-builtin-print_flags_debugger:

.. das:attribute:: print_flags_debugger = bitfield(0x1f)

|variable-builtin-print_flags_debugger|

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

.. _handle-builtin-export:

.. das:attribute:: export

|function_annotation-builtin-export|

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

.. _handle-builtin-unused_argument:

.. das:attribute:: unused_argument

|function_annotation-builtin-unused_argument|

.. _handle-builtin-local_only:

.. das:attribute:: local_only

|function_annotation-builtin-local_only|

.. _handle-builtin-expect_any_vector:

.. das:attribute:: expect_any_vector

|function_annotation-builtin-expect_any_vector|

.. _handle-builtin-builtin_array_sort:

.. das:attribute:: builtin_array_sort

|function_annotation-builtin-builtin_array_sort|

+++++++++++
Call macros
+++++++++++

.. _call-macro-builtin-concept_assert:

.. das:attribute:: concept_assert

|function_annotation-builtin-concept_assert|

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
Handled types
+++++++++++++

.. _handle-builtin-das_string:

.. das:attribute:: das_string

|any_annotation-builtin-das_string|

.. _handle-builtin-clock:

.. das:attribute:: clock

|any_annotation-builtin-clock|

.. _handle-builtin-dasvector`Error:

.. das:attribute:: dasvector`Error

|any_annotation-builtin-dasvector`Error|

.. _handle-builtin-dasvector`smart_ptr`TypeDecl:

.. das:attribute:: dasvector`smart_ptr`TypeDecl

|any_annotation-builtin-dasvector`smart_ptr`TypeDecl|

.. _handle-builtin-dasvector`das_string:

.. das:attribute:: dasvector`das_string

|any_annotation-builtin-dasvector`das_string|

.. _handle-builtin-dasvector`int:

.. das:attribute:: dasvector`int

|any_annotation-builtin-dasvector`int|

.. _handle-builtin-dasvector`smart_ptr`Expression:

.. das:attribute:: dasvector`smart_ptr`Expression

|any_annotation-builtin-dasvector`smart_ptr`Expression|

.. _handle-builtin-dasvector`FieldDeclaration:

.. das:attribute:: dasvector`FieldDeclaration

|any_annotation-builtin-dasvector`FieldDeclaration|

.. _handle-builtin-dasvector`EnumEntry:

.. das:attribute:: dasvector`EnumEntry

|any_annotation-builtin-dasvector`EnumEntry|

.. _handle-builtin-dasvector`smart_ptr`Variable:

.. das:attribute:: dasvector`smart_ptr`Variable

|any_annotation-builtin-dasvector`smart_ptr`Variable|

.. _handle-builtin-dasvector`InferHistory:

.. das:attribute:: dasvector`InferHistory

|any_annotation-builtin-dasvector`InferHistory|

.. _handle-builtin-dasvector`pair`uint`uint:

.. das:attribute:: dasvector`pair`uint`uint

|any_annotation-builtin-dasvector`pair`uint`uint|

.. _handle-builtin-dasvector`LineInfo:

.. das:attribute:: dasvector`LineInfo

|any_annotation-builtin-dasvector`LineInfo|

.. _handle-builtin-dasvector`smart_ptr`MakeStruct:

.. das:attribute:: dasvector`smart_ptr`MakeStruct

|any_annotation-builtin-dasvector`smart_ptr`MakeStruct|

.. _handle-builtin-dasvector`smart_ptr`MakeFieldDecl:

.. das:attribute:: dasvector`smart_ptr`MakeFieldDecl

|any_annotation-builtin-dasvector`smart_ptr`MakeFieldDecl|

.. _handle-builtin-dasvector`uint8:

.. das:attribute:: dasvector`uint8

|any_annotation-builtin-dasvector`uint8|

.. _handle-builtin-dasvector`CaptureEntry:

.. das:attribute:: dasvector`CaptureEntry

|any_annotation-builtin-dasvector`CaptureEntry|

.. _handle-builtin-dasvector`TestObjectFoo:

.. das:attribute:: dasvector`TestObjectFoo

|any_annotation-builtin-dasvector`TestObjectFoo|

++++++++++++++++++++++++++
Internal builtin functions
++++++++++++++++++++++++++

.. warning:: 
  This group of functions is hidden. It will not be in the final documentation.

  *  :ref:`builtin_get_command_line_arguments (arguments:array implicit) : void <function-_at__builtin__c__c_builtin_get_command_line_arguments_IA>` 

.. _function-_at__builtin__c__c_builtin_get_command_line_arguments_IA:

.. das:function:: builtin_get_command_line_arguments(arguments: array implicit)

+---------+--------------+
+argument +argument type +
+=========+==============+
+arguments+array implicit+
+---------+--------------+


|function-builtin-builtin_get_command_line_arguments|

++++++++++++++++++++++++++++
Internal pointer arithmetics
++++++++++++++++++++++++++++

.. warning:: 
  This group of functions is hidden. It will not be in the final documentation.

  *  :ref:`i_das_ptr_inc (arg0:void?& implicit;arg1:int const) : void <function-_at__builtin__c__c_i_das_ptr_inc_&I?_Ci>` 
  *  :ref:`i_das_ptr_dec (arg0:void?& implicit;arg1:int const) : void <function-_at__builtin__c__c_i_das_ptr_dec_&I?_Ci>` 
  *  :ref:`i_das_ptr_add (arg0:void? const implicit;arg1:int const;arg2:int const) : void? <function-_at__builtin__c__c_i_das_ptr_add_CI?_Ci_Ci>` 
  *  :ref:`i_das_ptr_sub (arg0:void?& implicit;arg1:int const;arg2:int const) : void? <function-_at__builtin__c__c_i_das_ptr_sub_&I?_Ci_Ci>` 
  *  :ref:`i_das_ptr_set_add (arg0:void?& implicit;arg1:int const;arg2:int const) : void <function-_at__builtin__c__c_i_das_ptr_set_add_&I?_Ci_Ci>` 
  *  :ref:`i_das_ptr_set_sub (arg0:void?& implicit;arg1:int const;arg2:int const) : void <function-_at__builtin__c__c_i_das_ptr_set_sub_&I?_Ci_Ci>` 
  *  :ref:`i_das_ptr_diff (arg0:void? const implicit;arg1:void? const implicit;arg2:int const) : int64 <function-_at__builtin__c__c_i_das_ptr_diff_CI?_CI?_Ci>` 

.. _function-_at__builtin__c__c_i_das_ptr_inc_&I?_Ci:

.. das:function:: i_das_ptr_inc(arg0: void?& implicit; arg1: int const)

.. warning:: 
  This is unsafe operation.

+--------+---------------+
+argument+argument type  +
+========+===============+
+arg0    +void?& implicit+
+--------+---------------+
+arg1    +int const      +
+--------+---------------+


|function-builtin-i_das_ptr_inc|

.. _function-_at__builtin__c__c_i_das_ptr_dec_&I?_Ci:

.. das:function:: i_das_ptr_dec(arg0: void?& implicit; arg1: int const)

.. warning:: 
  This is unsafe operation.

+--------+---------------+
+argument+argument type  +
+========+===============+
+arg0    +void?& implicit+
+--------+---------------+
+arg1    +int const      +
+--------+---------------+


|function-builtin-i_das_ptr_dec|

.. _function-_at__builtin__c__c_i_das_ptr_add_CI?_Ci_Ci:

.. das:function:: i_das_ptr_add(arg0: void? const implicit; arg1: int const; arg2: int const)

i_das_ptr_add returns void?

.. warning:: 
  This is unsafe operation.

+--------+--------------------+
+argument+argument type       +
+========+====================+
+arg0    +void? const implicit+
+--------+--------------------+
+arg1    +int const           +
+--------+--------------------+
+arg2    +int const           +
+--------+--------------------+


|function-builtin-i_das_ptr_add|

.. _function-_at__builtin__c__c_i_das_ptr_sub_&I?_Ci_Ci:

.. das:function:: i_das_ptr_sub(arg0: void?& implicit; arg1: int const; arg2: int const)

i_das_ptr_sub returns void?

.. warning:: 
  This is unsafe operation.

+--------+---------------+
+argument+argument type  +
+========+===============+
+arg0    +void?& implicit+
+--------+---------------+
+arg1    +int const      +
+--------+---------------+
+arg2    +int const      +
+--------+---------------+


|function-builtin-i_das_ptr_sub|

.. _function-_at__builtin__c__c_i_das_ptr_set_add_&I?_Ci_Ci:

.. das:function:: i_das_ptr_set_add(arg0: void?& implicit; arg1: int const; arg2: int const)

.. warning:: 
  This is unsafe operation.

+--------+---------------+
+argument+argument type  +
+========+===============+
+arg0    +void?& implicit+
+--------+---------------+
+arg1    +int const      +
+--------+---------------+
+arg2    +int const      +
+--------+---------------+


|function-builtin-i_das_ptr_set_add|

.. _function-_at__builtin__c__c_i_das_ptr_set_sub_&I?_Ci_Ci:

.. das:function:: i_das_ptr_set_sub(arg0: void?& implicit; arg1: int const; arg2: int const)

.. warning:: 
  This is unsafe operation.

+--------+---------------+
+argument+argument type  +
+========+===============+
+arg0    +void?& implicit+
+--------+---------------+
+arg1    +int const      +
+--------+---------------+
+arg2    +int const      +
+--------+---------------+


|function-builtin-i_das_ptr_set_sub|

.. _function-_at__builtin__c__c_i_das_ptr_diff_CI?_CI?_Ci:

.. das:function:: i_das_ptr_diff(arg0: void? const implicit; arg1: void? const implicit; arg2: int const)

i_das_ptr_diff returns int64

+--------+--------------------+
+argument+argument type       +
+========+====================+
+arg0    +void? const implicit+
+--------+--------------------+
+arg1    +void? const implicit+
+--------+--------------------+
+arg2    +int const           +
+--------+--------------------+


|function-builtin-i_das_ptr_diff|

+++++++++++++++++++++++++++++
Internal clone infrastructure
+++++++++++++++++++++++++++++

.. warning:: 
  This group of functions is hidden. It will not be in the final documentation.

  *  :ref:`clone (target:$::das_string implicit;src:string const implicit) : void <function-_at__builtin__c__c_clone_IH_ls__builtin__c__c_das_string_gr__CIs>` 
  *  :ref:`clone (target:string& implicit;src:$::das_string const implicit;context:__context const) : void <function-_at__builtin__c__c_clone_&Is_CIH_ls__builtin__c__c_das_string_gr__C_c>` 
  *  :ref:`clone_string (src:string const implicit;context:__context const) : string <function-_at__builtin__c__c_clone_string_CIs_C_c>` 
  *  :ref:`clone_to_move (clone_src:auto(TT) const implicit) : TT -const -# <function-_at__builtin__c__c_clone_to_move_CIY_ls_TT_gr_.>` 
  *  :ref:`clone_dim (a:auto -const;b:auto const implicit) : auto <function-_at__builtin__c__c_clone_dim_._CI.>` 
  *  :ref:`clone (a:array\<auto(TT)\> -const;b:array\<TT\> const) : auto <function-_at__builtin__c__c_clone_1_ls_Y_ls_TT_gr_._gr_A_C1_ls_Y_ls_TT_gr_L_gr_A>` 
  *  :ref:`clone (a:array\<auto(TT)\> -const;b:array\<TT\> const#) : auto <function-_at__builtin__c__c_clone_1_ls_Y_ls_TT_gr_._gr_A_C_hh_1_ls_Y_ls_TT_gr_L_gr_A>` 
  *  :ref:`clone (a:table\<auto(KT);auto(VT)\> -const;b:table\<KT;VT\> const implicit) : auto <function-_at__builtin__c__c_clone_1_ls_Y_ls_KT_gr_._gr_2_ls_Y_ls_VT_gr_._gr_T_CI1_ls_Y_ls_KT_gr_L_gr_2_ls_Y_ls_VT_gr_L_gr_T>` 
  *  :ref:`clone (a:table\<string;auto(VT)\> -const;b:table\<string;VT\> const implicit) : auto <function-_at__builtin__c__c_clone_1_ls_s_gr_2_ls_Y_ls_VT_gr_._gr_T_CI1_ls_s_gr_2_ls_Y_ls_VT_gr_L_gr_T>` 

.. _function-_at__builtin__c__c_clone_IH_ls__builtin__c__c_das_string_gr__CIs:

.. das:function:: clone(target: das_string implicit; src: string const implicit)

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+target  + :ref:`builtin::das_string <handle-builtin-das_string>`  implicit+
+--------+-----------------------------------------------------------------+
+src     +string const implicit                                            +
+--------+-----------------------------------------------------------------+


|function-builtin-clone|

.. _function-_at__builtin__c__c_clone_&Is_CIH_ls__builtin__c__c_das_string_gr__C_c:

.. das:function:: clone(target: string& implicit; src: das_string const implicit)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+target  +string& implicit                                                       +
+--------+-----------------------------------------------------------------------+
+src     + :ref:`builtin::das_string <handle-builtin-das_string>`  const implicit+
+--------+-----------------------------------------------------------------------+


|function-builtin-clone|

.. _function-_at__builtin__c__c_clone_string_CIs_C_c:

.. das:function:: clone_string(src: string const implicit)

clone_string returns string

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+src     +string const implicit+
+--------+---------------------+


|function-builtin-clone_string|

.. _function-_at__builtin__c__c_clone_to_move_CIY_ls_TT_gr_.:

.. das:function:: clone_to_move(clone_src: auto(TT) const implicit)

clone_to_move returns TT

+---------+-----------------------+
+argument +argument type          +
+=========+=======================+
+clone_src+auto(TT) const implicit+
+---------+-----------------------+


|function-builtin-clone_to_move|

.. _function-_at__builtin__c__c_clone_dim_._CI.:

.. das:function:: clone_dim(a: auto; b: auto const implicit)

clone_dim returns auto

+--------+-------------------+
+argument+argument type      +
+========+===================+
+a       +auto               +
+--------+-------------------+
+b       +auto const implicit+
+--------+-------------------+


|function-builtin-clone_dim|

.. _function-_at__builtin__c__c_clone_1_ls_Y_ls_TT_gr_._gr_A_C1_ls_Y_ls_TT_gr_L_gr_A:

.. das:function:: clone(a: array<auto(TT)>; b: array<TT> const)

clone returns auto

+--------+---------------+
+argument+argument type  +
+========+===============+
+a       +array<auto(TT)>+
+--------+---------------+
+b       +array<TT> const+
+--------+---------------+


|function-builtin-clone|

.. _function-_at__builtin__c__c_clone_1_ls_Y_ls_TT_gr_._gr_A_C_hh_1_ls_Y_ls_TT_gr_L_gr_A:

.. das:function:: clone(a: array<auto(TT)>; b: array<TT> const#)

clone returns auto

+--------+----------------+
+argument+argument type   +
+========+================+
+a       +array<auto(TT)> +
+--------+----------------+
+b       +array<TT> const#+
+--------+----------------+


|function-builtin-clone|

.. _function-_at__builtin__c__c_clone_1_ls_Y_ls_KT_gr_._gr_2_ls_Y_ls_VT_gr_._gr_T_CI1_ls_Y_ls_KT_gr_L_gr_2_ls_Y_ls_VT_gr_L_gr_T:

.. das:function:: clone(a: table<auto(KT);auto(VT)>; b: table<KT;VT> const implicit)

clone returns auto

+--------+---------------------------+
+argument+argument type              +
+========+===========================+
+a       +table<auto(KT);auto(VT)>   +
+--------+---------------------------+
+b       +table<KT;VT> const implicit+
+--------+---------------------------+


|function-builtin-clone|

.. _function-_at__builtin__c__c_clone_1_ls_s_gr_2_ls_Y_ls_VT_gr_._gr_T_CI1_ls_s_gr_2_ls_Y_ls_VT_gr_L_gr_T:

.. das:function:: clone(a: table<string;auto(VT)>; b: table<string;VT> const implicit)

clone returns auto

+--------+-------------------------------+
+argument+argument type                  +
+========+===============================+
+a       +table<string;auto(VT)>         +
+--------+-------------------------------+
+b       +table<string;VT> const implicit+
+--------+-------------------------------+


|function-builtin-clone|

++++++++++++++++++++++++++++++++
Internal finalize infrastructure
++++++++++++++++++++++++++++++++

.. warning:: 
  This group of functions is hidden. It will not be in the final documentation.

  *  :ref:`finalize_dim (a:auto(TT)[-1] -const) : auto <function-_at__builtin__c__c_finalize_dim_[-1]Y_ls_TT_gr_.>` 
  *  :ref:`finalize (a:array\<auto(TT)\> -const) : auto <function-_at__builtin__c__c_finalize_1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`finalize (a:table\<auto(TK);auto(TV)\> -const) : auto <function-_at__builtin__c__c_finalize_1_ls_Y_ls_TK_gr_._gr_2_ls_Y_ls_TV_gr_._gr_T>` 

.. _function-_at__builtin__c__c_finalize_dim_[-1]Y_ls_TT_gr_.:

.. das:function:: finalize_dim(a: auto(TT)[-1])

finalize_dim returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +auto(TT)[-1] +
+--------+-------------+


|function-builtin-finalize_dim|

.. _function-_at__builtin__c__c_finalize_1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: finalize(a: array<auto(TT)>)

finalize returns auto

+--------+---------------+
+argument+argument type  +
+========+===============+
+a       +array<auto(TT)>+
+--------+---------------+


|function-builtin-finalize|

.. _function-_at__builtin__c__c_finalize_1_ls_Y_ls_TK_gr_._gr_2_ls_Y_ls_TV_gr_._gr_T:

.. das:function:: finalize(a: table<auto(TK);auto(TV)>)

finalize returns auto

+--------+------------------------+
+argument+argument type           +
+========+========================+
+a       +table<auto(TK);auto(TV)>+
+--------+------------------------+


|function-builtin-finalize|

++++++++++
Containers
++++++++++

  *  :ref:`clear (array:array implicit;context:__context const) : void <function-_at__builtin__c__c_clear_IA_C_c>` 
  *  :ref:`length (array:array const implicit) : int <function-_at__builtin__c__c_length_CIA>` 
  *  :ref:`capacity (array:array const implicit) : int <function-_at__builtin__c__c_capacity_CIA>` 
  *  :ref:`empty (iterator:iterator const implicit) : bool <function-_at__builtin__c__c_empty_CIG>` 
  *  :ref:`clear (table:table implicit;context:__context const) : void <function-_at__builtin__c__c_clear_IT_C_c>` 
  *  :ref:`length (table:table const implicit) : int <function-_at__builtin__c__c_length_CIT>` 
  *  :ref:`capacity (table:table const implicit) : int <function-_at__builtin__c__c_capacity_CIT>` 
  *  :ref:`resize (Arr:array\<auto(numT)\> -const;newSize:int const) : auto <function-_at__builtin__c__c_resize_1_ls_Y_ls_numT_gr_._gr_A_Ci>` 
  *  :ref:`reserve (Arr:array\<auto(numT)\> -const;newSize:int const) : auto <function-_at__builtin__c__c_reserve_1_ls_Y_ls_numT_gr_._gr_A_Ci>` 
  *  :ref:`pop (Arr:array\<auto(numT)\> -const) : auto <function-_at__builtin__c__c_pop_1_ls_Y_ls_numT_gr_._gr_A>` 
  *  :ref:`push (Arr:array\<auto(numT)\> -const;value:numT const;at:int const) : auto <function-_at__builtin__c__c_push_1_ls_Y_ls_numT_gr_._gr_A_CY_ls_numT_gr_L_Ci>` 
  *  :ref:`push (Arr:array\<auto(numT)\> -const;value:numT const) : auto <function-_at__builtin__c__c_push_1_ls_Y_ls_numT_gr_._gr_A_CY_ls_numT_gr_L>` 
  *  :ref:`push (Arr:array\<auto(numT)\> -const;varr:array\<numT\> const) : auto <function-_at__builtin__c__c_push_1_ls_Y_ls_numT_gr_._gr_A_C1_ls_Y_ls_numT_gr_L_gr_A>` 
  *  :ref:`push (Arr:array\<auto(numT)\> -const;varr:numT const[-1]) : auto <function-_at__builtin__c__c_push_1_ls_Y_ls_numT_gr_._gr_A_C[-1]Y_ls_numT_gr_L>` 
  *  :ref:`emplace (Arr:array\<auto(numT)\> -const;value:numT -const;at:int const) : auto <function-_at__builtin__c__c_emplace_1_ls_Y_ls_numT_gr_._gr_A_Y_ls_numT_gr_L_Ci>` 
  *  :ref:`emplace (Arr:array\<auto(numT)\> -const;value:numT -const) : auto <function-_at__builtin__c__c_emplace_1_ls_Y_ls_numT_gr_._gr_A_Y_ls_numT_gr_L>` 
  *  :ref:`push_clone (Arr:array\<auto(numT)\> -const;value:numT const implicit;at:int const) : auto <function-_at__builtin__c__c_push_clone_1_ls_Y_ls_numT_gr_._gr_A_CIY_ls_numT_gr_L_Ci>` 
  *  :ref:`push_clone (Arr:array\<auto(numT)\> -const;value:numT const implicit) : auto <function-_at__builtin__c__c_push_clone_1_ls_Y_ls_numT_gr_._gr_A_CIY_ls_numT_gr_L>` 
  *  :ref:`push_clone (A:auto(CT) -const -#;b:auto(TT) const implicit) : auto <function-_at__builtin__c__c_push_clone_Y_ls_CT_gr_._CIY_ls_TT_gr_.>` 
  *  :ref:`erase (Arr:array\<auto(numT)\> -const;at:int const) : auto <function-_at__builtin__c__c_erase_1_ls_Y_ls_numT_gr_._gr_A_Ci>` 
  *  :ref:`length (a:auto const[-1]) : int <function-_at__builtin__c__c_length_C[-1].>` 
  *  :ref:`empty (a:array\<auto\> const) : bool <function-_at__builtin__c__c_empty_C1_ls_._gr_A>` 
  *  :ref:`find (Tab:table\<auto(keyT);auto(valT)\> const;at:keyT const implicit;blk:block\<(p:valT? const):void\> const) : auto <function-_at__builtin__c__c_find_C1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_CIY_ls_keyT_gr_L_CN_ls_p_gr_0_ls_C1_ls_Y_ls_valT_gr_L_gr_?_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`find_if_exists (Tab:table\<auto(keyT);auto(valT)\> const;at:keyT const implicit;blk:block\<(p:valT? const):void\> const) : auto <function-_at__builtin__c__c_find_if_exists_C1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_CIY_ls_keyT_gr_L_CN_ls_p_gr_0_ls_C1_ls_Y_ls_valT_gr_L_gr_?_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`find (Tab:table\<auto(keyT);auto(valT)\> const;at:keyT const implicit) : valT const? <function-_at__builtin__c__c_find_C1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_CIY_ls_keyT_gr_L>` 
  *  :ref:`find (Tab:table\<auto(keyT);auto(valT)\> const#;at:keyT const implicit) : valT const?# <function-_at__builtin__c__c_find_C_hh_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_CIY_ls_keyT_gr_L>` 
  *  :ref:`find_for_edit (Tab:table\<auto(keyT);auto(valT)\> -const;at:keyT const implicit;blk:block\<(p:valT? -const):void\> const) : auto <function-_at__builtin__c__c_find_for_edit_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_CIY_ls_keyT_gr_L_CN_ls_p_gr_0_ls_1_ls_Y_ls_valT_gr_L_gr_?_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`find_for_edit_if_exists (Tab:table\<auto(keyT);auto(valT)\> -const;at:keyT const implicit;blk:block\<(p:valT? -const):void\> const) : auto <function-_at__builtin__c__c_find_for_edit_if_exists_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_CIY_ls_keyT_gr_L_CN_ls_p_gr_0_ls_1_ls_Y_ls_valT_gr_L_gr_?_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`find_for_edit (Tab:table\<auto(keyT);auto(valT)\> -const;at:keyT const implicit) : valT? <function-_at__builtin__c__c_find_for_edit_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_CIY_ls_keyT_gr_L>` 
  *  :ref:`erase (Tab:table\<auto(keyT);auto(valT)\> -const;at:keyT const implicit) : bool <function-_at__builtin__c__c_erase_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_CIY_ls_keyT_gr_L>` 
  *  :ref:`key_exists (Tab:table\<auto(keyT);auto(valT)\> const;at:keyT const implicit) : bool <function-_at__builtin__c__c_key_exists_C1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_CIY_ls_keyT_gr_L>` 
  *  :ref:`keys (a:table\<auto(keyT);auto(valT)\> const implicit) : iterator\<keyT const&\> <function-_at__builtin__c__c_keys_CI1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T>` 
  *  :ref:`values (a:table\<auto(keyT);auto(valT)\> const implicit =const) : iterator\<valT const&\> <function-_at__builtin__c__c_values_CI_eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T>` 
  *  :ref:`values (a:table\<auto(keyT);auto(valT)\> implicit =const -const) : iterator\<valT&\> <function-_at__builtin__c__c_values_I_eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T>` 
  *  :ref:`lock (Tab:table\<auto(keyT);auto(valT)\> const;blk:block\<(t:table\<keyT;valT\> const#):void\> const) : auto <function-_at__builtin__c__c_lock_C1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_CN_ls_t_gr_0_ls_C_hh_1_ls_Y_ls_keyT_gr_L_gr_2_ls_Y_ls_valT_gr_L_gr_T_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`lock (Tab:table\<auto(keyT);auto(valT)\> const#;blk:block\<(t:table\<keyT;valT\> const#):void\> const) : auto <function-_at__builtin__c__c_lock_C_hh_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_CN_ls_t_gr_0_ls_C_hh_1_ls_Y_ls_keyT_gr_L_gr_2_ls_Y_ls_valT_gr_L_gr_T_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`lock_forever (Tab:table\<auto(keyT);auto(valT)\> -const) : table\<keyT;valT\># <function-_at__builtin__c__c_lock_forever_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T>` 
  *  :ref:`next (it:iterator\<auto(TT)\> const;value:TT& -const) : bool <function-_at__builtin__c__c_next_C1_ls_Y_ls_TT_gr_._gr_G_&Y_ls_TT_gr_L>` 
  *  :ref:`length (vec:auto const) : auto <function-_at__builtin__c__c_length_C.>` 
  *  :ref:`each (rng:range const) : iterator\<int\> <function-_at__builtin__c__c_each_Cr>` 
  *  :ref:`each (str:string const) : iterator\<int\> <function-_at__builtin__c__c_each_Cs>` 
  *  :ref:`each (a:auto(TT) const[-1]) : iterator\<TT&\> <function-_at__builtin__c__c_each_C[-1]Y_ls_TT_gr_.>` 
  *  :ref:`each (a:array\<auto(TT)\> const) : iterator\<TT&\> <function-_at__builtin__c__c_each_C1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`each (lam:lambda\<(arg:auto(argT) -const):bool\> const) : iterator\<argT -&\> <function-_at__builtin__c__c_each_CN_ls_arg_gr_0_ls_Y_ls_argT_gr_._gr_1_ls_b_gr__at_>` 
  *  :ref:`each_ref (lam:lambda\<(arg:auto(argT)? -const):bool\> const) : iterator\<argT&\> <function-_at__builtin__c__c_each_ref_CN_ls_arg_gr_0_ls_1_ls_Y_ls_argT_gr_._gr_?_gr_1_ls_b_gr__at_>` 
  *  :ref:`each_enum (tt:auto(TT) const) : iterator\<TT -const -&\> <function-_at__builtin__c__c_each_enum_CY_ls_TT_gr_.>` 
  *  :ref:`nothing (it:iterator\<auto(TT)\> -const) : iterator\<TT\> <function-_at__builtin__c__c_nothing_1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`to_array (it:iterator\<auto(TT)\> const) : array\<TT -const -&\> <function-_at__builtin__c__c_to_array_C1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`to_array (a:auto(TT) const[-1]) : array\<TT -const\> <function-_at__builtin__c__c_to_array_C[-1]Y_ls_TT_gr_.>` 
  *  :ref:`to_array_move (a:auto(TT)[-1] -const) : array\<TT -const\> <function-_at__builtin__c__c_to_array_move_[-1]Y_ls_TT_gr_.>` 
  *  :ref:`to_array_move (a:auto(TT) -const) : array\<TT -const\> <function-_at__builtin__c__c_to_array_move_Y_ls_TT_gr_.>` 
  *  :ref:`to_table (a:tuple\<auto(keyT);auto(valT)\> const[-1]) : table\<keyT -const;valT\> <function-_at__builtin__c__c_to_table_C[-1]0_ls_Y_ls_keyT_gr_.;Y_ls_valT_gr_._gr_U>` 
  *  :ref:`to_table_move (a:tuple\<auto(keyT);auto(valT)\>[-1] -const) : table\<keyT -const;valT\> <function-_at__builtin__c__c_to_table_move_[-1]0_ls_Y_ls_keyT_gr_.;Y_ls_valT_gr_._gr_U>` 
  *  :ref:`sort (a:auto(TT)[-1] implicit -const) : auto <function-_at__builtin__c__c_sort_I[-1]Y_ls_TT_gr_.>` 
  *  :ref:`sort (a:array\<auto(TT)\> implicit -const) : auto <function-_at__builtin__c__c_sort_I1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`sort (a:auto(TT)[-1] implicit -const;cmp:block\<(x:TT const;y:TT const):bool\> const) : auto <function-_at__builtin__c__c_sort_I[-1]Y_ls_TT_gr_._CN_ls_x;y_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`sort (a:array\<auto(TT)\> implicit -const;cmp:block\<(x:TT const;y:TT const):bool\> const) : auto <function-_at__builtin__c__c_sort_I1_ls_Y_ls_TT_gr_._gr_A_CN_ls_x;y_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`lock (a:array\<auto(TT)\> =const -const;blk:block\<(x:array\<TT\># implicit -const):auto\> const) : auto <function-_at__builtin__c__c_lock__eq_1_ls_Y_ls_TT_gr_._gr_A_CN_ls_x_gr_0_ls__hh_I1_ls_Y_ls_TT_gr_L_gr_A_gr_1_ls_._gr__builtin_>` 
  *  :ref:`lock (a:array\<auto(TT)\> const =const;blk:block\<(x:array\<TT\> const# implicit):auto\> const) : auto <function-_at__builtin__c__c_lock_C_eq_1_ls_Y_ls_TT_gr_._gr_A_CN_ls_x_gr_0_ls_C_hh_I1_ls_Y_ls_TT_gr_L_gr_A_gr_1_ls_._gr__builtin_>` 
  *  :ref:`find_index (arr:array\<auto(TT)\> const implicit;key:TT const) : auto <function-_at__builtin__c__c_find_index_CI1_ls_Y_ls_TT_gr_._gr_A_CY_ls_TT_gr_L>` 
  *  :ref:`find_index (arr:auto(TT) const[-1] implicit;key:TT const) : auto <function-_at__builtin__c__c_find_index_CI[-1]Y_ls_TT_gr_._CY_ls_TT_gr_L>` 
  *  :ref:`find_index (arr:iterator\<auto(TT)\> const;key:TT const -&) : auto <function-_at__builtin__c__c_find_index_C1_ls_Y_ls_TT_gr_._gr_G_CY_ls_TT_gr_L>` 
  *  :ref:`find_index_if (arr:array\<auto(TT)\> const implicit;blk:block\<(key:TT const):bool\> const) : auto <function-_at__builtin__c__c_find_index_if_CI1_ls_Y_ls_TT_gr_._gr_A_CN_ls_key_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`find_index_if (arr:auto(TT) const[-1] implicit;blk:block\<(key:TT const):bool\> const) : auto <function-_at__builtin__c__c_find_index_if_CI[-1]Y_ls_TT_gr_._CN_ls_key_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`find_index_if (arr:iterator\<auto(TT)\> const;blk:block\<(key:TT const -&):bool\> const) : auto <function-_at__builtin__c__c_find_index_if_C1_ls_Y_ls_TT_gr_._gr_G_CN_ls_key_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`has_value (a:auto const;key:auto const) : auto <function-_at__builtin__c__c_has_value_C._C.>` 

.. _function-_at__builtin__c__c_clear_IA_C_c:

.. das:function:: clear(array: array implicit)

+--------+--------------+
+argument+argument type +
+========+==============+
+array   +array implicit+
+--------+--------------+


|function-builtin-clear|

.. _function-_at__builtin__c__c_length_CIA:

.. das:function:: length(array: array const implicit)

length returns int

+--------+--------------------+
+argument+argument type       +
+========+====================+
+array   +array const implicit+
+--------+--------------------+


|function-builtin-length|

.. _function-_at__builtin__c__c_capacity_CIA:

.. das:function:: capacity(array: array const implicit)

capacity returns int

+--------+--------------------+
+argument+argument type       +
+========+====================+
+array   +array const implicit+
+--------+--------------------+


|function-builtin-capacity|

.. _function-_at__builtin__c__c_empty_CIG:

.. das:function:: empty(iterator: iterator const implicit)

empty returns bool

+--------+-----------------------+
+argument+argument type          +
+========+=======================+
+iterator+iterator const implicit+
+--------+-----------------------+


|function-builtin-empty|

.. _function-_at__builtin__c__c_clear_IT_C_c:

.. das:function:: clear(table: table implicit)

+--------+--------------+
+argument+argument type +
+========+==============+
+table   +table implicit+
+--------+--------------+


|function-builtin-clear|

.. _function-_at__builtin__c__c_length_CIT:

.. das:function:: length(table: table const implicit)

length returns int

+--------+--------------------+
+argument+argument type       +
+========+====================+
+table   +table const implicit+
+--------+--------------------+


|function-builtin-length|

.. _function-_at__builtin__c__c_capacity_CIT:

.. das:function:: capacity(table: table const implicit)

capacity returns int

+--------+--------------------+
+argument+argument type       +
+========+====================+
+table   +table const implicit+
+--------+--------------------+


|function-builtin-capacity|

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

.. _function-_at__builtin__c__c_push_1_ls_Y_ls_numT_gr_._gr_A_CY_ls_numT_gr_L_Ci:

.. das:function:: push(Arr: array<auto(numT)>; value: numT const; at: int const)

push returns auto

+--------+-----------------+
+argument+argument type    +
+========+=================+
+Arr     +array<auto(numT)>+
+--------+-----------------+
+value   +numT const       +
+--------+-----------------+
+at      +int const        +
+--------+-----------------+


|function-builtin-push|

.. _function-_at__builtin__c__c_push_1_ls_Y_ls_numT_gr_._gr_A_CY_ls_numT_gr_L:

.. das:function:: push(Arr: array<auto(numT)>; value: numT const)

push returns auto

+--------+-----------------+
+argument+argument type    +
+========+=================+
+Arr     +array<auto(numT)>+
+--------+-----------------+
+value   +numT const       +
+--------+-----------------+


|function-builtin-push|

.. _function-_at__builtin__c__c_push_1_ls_Y_ls_numT_gr_._gr_A_C1_ls_Y_ls_numT_gr_L_gr_A:

.. das:function:: push(Arr: array<auto(numT)>; varr: array<numT> const)

push returns auto

+--------+-----------------+
+argument+argument type    +
+========+=================+
+Arr     +array<auto(numT)>+
+--------+-----------------+
+varr    +array<numT> const+
+--------+-----------------+


|function-builtin-push|

.. _function-_at__builtin__c__c_push_1_ls_Y_ls_numT_gr_._gr_A_C[-1]Y_ls_numT_gr_L:

.. das:function:: push(Arr: array<auto(numT)>; varr: numT const[-1])

push returns auto

+--------+-----------------+
+argument+argument type    +
+========+=================+
+Arr     +array<auto(numT)>+
+--------+-----------------+
+varr    +numT const[-1]   +
+--------+-----------------+


|function-builtin-push|

.. _function-_at__builtin__c__c_emplace_1_ls_Y_ls_numT_gr_._gr_A_Y_ls_numT_gr_L_Ci:

.. das:function:: emplace(Arr: array<auto(numT)>; value: numT; at: int const)

emplace returns auto

+--------+-----------------+
+argument+argument type    +
+========+=================+
+Arr     +array<auto(numT)>+
+--------+-----------------+
+value   +numT             +
+--------+-----------------+
+at      +int const        +
+--------+-----------------+


|function-builtin-emplace|

.. _function-_at__builtin__c__c_emplace_1_ls_Y_ls_numT_gr_._gr_A_Y_ls_numT_gr_L:

.. das:function:: emplace(Arr: array<auto(numT)>; value: numT)

emplace returns auto

+--------+-----------------+
+argument+argument type    +
+========+=================+
+Arr     +array<auto(numT)>+
+--------+-----------------+
+value   +numT             +
+--------+-----------------+


|function-builtin-emplace|

.. _function-_at__builtin__c__c_push_clone_1_ls_Y_ls_numT_gr_._gr_A_CIY_ls_numT_gr_L_Ci:

.. das:function:: push_clone(Arr: array<auto(numT)>; value: numT const implicit; at: int const)

push_clone returns auto

+--------+-------------------+
+argument+argument type      +
+========+===================+
+Arr     +array<auto(numT)>  +
+--------+-------------------+
+value   +numT const implicit+
+--------+-------------------+
+at      +int const          +
+--------+-------------------+


|function-builtin-push_clone|

.. _function-_at__builtin__c__c_push_clone_1_ls_Y_ls_numT_gr_._gr_A_CIY_ls_numT_gr_L:

.. das:function:: push_clone(Arr: array<auto(numT)>; value: numT const implicit)

push_clone returns auto

+--------+-------------------+
+argument+argument type      +
+========+===================+
+Arr     +array<auto(numT)>  +
+--------+-------------------+
+value   +numT const implicit+
+--------+-------------------+


|function-builtin-push_clone|

.. _function-_at__builtin__c__c_push_clone_Y_ls_CT_gr_._CIY_ls_TT_gr_.:

.. das:function:: push_clone(A: auto(CT); b: auto(TT) const implicit)

push_clone returns auto

+--------+-----------------------+
+argument+argument type          +
+========+=======================+
+A       +auto(CT)               +
+--------+-----------------------+
+b       +auto(TT) const implicit+
+--------+-----------------------+


|function-builtin-push_clone|

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

.. _function-_at__builtin__c__c_length_C[-1].:

.. das:function:: length(a: auto const[-1])

length returns int

+--------+--------------+
+argument+argument type +
+========+==============+
+a       +auto const[-1]+
+--------+--------------+


|function-builtin-length|

.. _function-_at__builtin__c__c_empty_C1_ls_._gr_A:

.. das:function:: empty(a: array<auto> const)

empty returns bool

+--------+-----------------+
+argument+argument type    +
+========+=================+
+a       +array<auto> const+
+--------+-----------------+


|function-builtin-empty|

.. _function-_at__builtin__c__c_find_C1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_CIY_ls_keyT_gr_L_CN_ls_p_gr_0_ls_C1_ls_Y_ls_valT_gr_L_gr_?_gr_1_ls_v_gr__builtin_:

.. das:function:: find(Tab: table<auto(keyT);auto(valT)> const; at: keyT const implicit; blk: block<(p:valT? const):void> const)

find returns auto

+--------+----------------------------------+
+argument+argument type                     +
+========+==================================+
+Tab     +table<auto(keyT);auto(valT)> const+
+--------+----------------------------------+
+at      +keyT const implicit               +
+--------+----------------------------------+
+blk     +block<(p:valT? const):void> const +
+--------+----------------------------------+


|function-builtin-find|

.. _function-_at__builtin__c__c_find_if_exists_C1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_CIY_ls_keyT_gr_L_CN_ls_p_gr_0_ls_C1_ls_Y_ls_valT_gr_L_gr_?_gr_1_ls_v_gr__builtin_:

.. das:function:: find_if_exists(Tab: table<auto(keyT);auto(valT)> const; at: keyT const implicit; blk: block<(p:valT? const):void> const)

find_if_exists returns auto

+--------+----------------------------------+
+argument+argument type                     +
+========+==================================+
+Tab     +table<auto(keyT);auto(valT)> const+
+--------+----------------------------------+
+at      +keyT const implicit               +
+--------+----------------------------------+
+blk     +block<(p:valT? const):void> const +
+--------+----------------------------------+


|function-builtin-find_if_exists|

.. _function-_at__builtin__c__c_find_C1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_CIY_ls_keyT_gr_L:

.. das:function:: find(Tab: table<auto(keyT);auto(valT)> const; at: keyT const implicit)

find returns valT const?

+--------+----------------------------------+
+argument+argument type                     +
+========+==================================+
+Tab     +table<auto(keyT);auto(valT)> const+
+--------+----------------------------------+
+at      +keyT const implicit               +
+--------+----------------------------------+


|function-builtin-find|

.. _function-_at__builtin__c__c_find_C_hh_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_CIY_ls_keyT_gr_L:

.. das:function:: find(Tab: table<auto(keyT);auto(valT)> const#; at: keyT const implicit)

find returns valT const?#

+--------+-----------------------------------+
+argument+argument type                      +
+========+===================================+
+Tab     +table<auto(keyT);auto(valT)> const#+
+--------+-----------------------------------+
+at      +keyT const implicit                +
+--------+-----------------------------------+


|function-builtin-find|

.. _function-_at__builtin__c__c_find_for_edit_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_CIY_ls_keyT_gr_L_CN_ls_p_gr_0_ls_1_ls_Y_ls_valT_gr_L_gr_?_gr_1_ls_v_gr__builtin_:

.. das:function:: find_for_edit(Tab: table<auto(keyT);auto(valT)>; at: keyT const implicit; blk: block<(p:valT? -const):void> const)

find_for_edit returns auto

+--------+----------------------------+
+argument+argument type               +
+========+============================+
+Tab     +table<auto(keyT);auto(valT)>+
+--------+----------------------------+
+at      +keyT const implicit         +
+--------+----------------------------+
+blk     +block<(p:valT?):void> const +
+--------+----------------------------+


|function-builtin-find_for_edit|

.. _function-_at__builtin__c__c_find_for_edit_if_exists_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_CIY_ls_keyT_gr_L_CN_ls_p_gr_0_ls_1_ls_Y_ls_valT_gr_L_gr_?_gr_1_ls_v_gr__builtin_:

.. das:function:: find_for_edit_if_exists(Tab: table<auto(keyT);auto(valT)>; at: keyT const implicit; blk: block<(p:valT? -const):void> const)

find_for_edit_if_exists returns auto

+--------+----------------------------+
+argument+argument type               +
+========+============================+
+Tab     +table<auto(keyT);auto(valT)>+
+--------+----------------------------+
+at      +keyT const implicit         +
+--------+----------------------------+
+blk     +block<(p:valT?):void> const +
+--------+----------------------------+


|function-builtin-find_for_edit_if_exists|

.. _function-_at__builtin__c__c_find_for_edit_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_CIY_ls_keyT_gr_L:

.. das:function:: find_for_edit(Tab: table<auto(keyT);auto(valT)>; at: keyT const implicit)

find_for_edit returns valT?

.. warning:: 
  This is unsafe operation.

+--------+----------------------------+
+argument+argument type               +
+========+============================+
+Tab     +table<auto(keyT);auto(valT)>+
+--------+----------------------------+
+at      +keyT const implicit         +
+--------+----------------------------+


|function-builtin-find_for_edit|

.. _function-_at__builtin__c__c_erase_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_CIY_ls_keyT_gr_L:

.. das:function:: erase(Tab: table<auto(keyT);auto(valT)>; at: keyT const implicit)

erase returns bool

+--------+----------------------------+
+argument+argument type               +
+========+============================+
+Tab     +table<auto(keyT);auto(valT)>+
+--------+----------------------------+
+at      +keyT const implicit         +
+--------+----------------------------+


|function-builtin-erase|

.. _function-_at__builtin__c__c_key_exists_C1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_CIY_ls_keyT_gr_L:

.. das:function:: key_exists(Tab: table<auto(keyT);auto(valT)> const; at: keyT const implicit)

key_exists returns bool

+--------+----------------------------------+
+argument+argument type                     +
+========+==================================+
+Tab     +table<auto(keyT);auto(valT)> const+
+--------+----------------------------------+
+at      +keyT const implicit               +
+--------+----------------------------------+


|function-builtin-key_exists|

.. _function-_at__builtin__c__c_keys_CI1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T:

.. das:function:: keys(a: table<auto(keyT);auto(valT)> const implicit)

keys returns iterator<keyT const&>

+--------+-------------------------------------------+
+argument+argument type                              +
+========+===========================================+
+a       +table<auto(keyT);auto(valT)> const implicit+
+--------+-------------------------------------------+


|function-builtin-keys|

.. _function-_at__builtin__c__c_values_CI_eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T:

.. das:function:: values(a: table<auto(keyT);auto(valT)> const implicit =const)

values returns iterator<valT const&>

+--------+--------------------------------------------+
+argument+argument type                               +
+========+============================================+
+a       +table<auto(keyT);auto(valT)> const implicit!+
+--------+--------------------------------------------+


|function-builtin-values|

.. _function-_at__builtin__c__c_values_I_eq_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T:

.. das:function:: values(a: table<auto(keyT);auto(valT)> implicit =const)

values returns iterator<valT&>

+--------+--------------------------------------+
+argument+argument type                         +
+========+======================================+
+a       +table<auto(keyT);auto(valT)> implicit!+
+--------+--------------------------------------+


|function-builtin-values|

.. _function-_at__builtin__c__c_lock_C1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_CN_ls_t_gr_0_ls_C_hh_1_ls_Y_ls_keyT_gr_L_gr_2_ls_Y_ls_valT_gr_L_gr_T_gr_1_ls_v_gr__builtin_:

.. das:function:: lock(Tab: table<auto(keyT);auto(valT)> const; blk: block<(t:table<keyT;valT> const#):void> const)

lock returns auto

+--------+---------------------------------------------+
+argument+argument type                                +
+========+=============================================+
+Tab     +table<auto(keyT);auto(valT)> const           +
+--------+---------------------------------------------+
+blk     +block<(t:table<keyT;valT> const#):void> const+
+--------+---------------------------------------------+


|function-builtin-lock|

.. _function-_at__builtin__c__c_lock_C_hh_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_CN_ls_t_gr_0_ls_C_hh_1_ls_Y_ls_keyT_gr_L_gr_2_ls_Y_ls_valT_gr_L_gr_T_gr_1_ls_v_gr__builtin_:

.. das:function:: lock(Tab: table<auto(keyT);auto(valT)> const#; blk: block<(t:table<keyT;valT> const#):void> const)

lock returns auto

+--------+---------------------------------------------+
+argument+argument type                                +
+========+=============================================+
+Tab     +table<auto(keyT);auto(valT)> const#          +
+--------+---------------------------------------------+
+blk     +block<(t:table<keyT;valT> const#):void> const+
+--------+---------------------------------------------+


|function-builtin-lock|

.. _function-_at__builtin__c__c_lock_forever_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T:

.. das:function:: lock_forever(Tab: table<auto(keyT);auto(valT)>)

lock_forever returns table<keyT;valT>#

+--------+----------------------------+
+argument+argument type               +
+========+============================+
+Tab     +table<auto(keyT);auto(valT)>+
+--------+----------------------------+


|function-builtin-lock_forever|

.. _function-_at__builtin__c__c_next_C1_ls_Y_ls_TT_gr_._gr_G_&Y_ls_TT_gr_L:

.. das:function:: next(it: iterator<auto(TT)> const; value: TT&)

next returns bool

+--------+------------------------+
+argument+argument type           +
+========+========================+
+it      +iterator<auto(TT)> const+
+--------+------------------------+
+value   +TT&                     +
+--------+------------------------+


|function-builtin-next|

.. _function-_at__builtin__c__c_length_C.:

.. das:function:: length(vec: auto const)

length returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+vec     +auto const   +
+--------+-------------+


|function-builtin-length|

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

.. warning:: 
  This is unsafe operation.

+--------+-------------+
+argument+argument type+
+========+=============+
+str     +string const +
+--------+-------------+


|function-builtin-each|

.. _function-_at__builtin__c__c_each_C[-1]Y_ls_TT_gr_.:

.. das:function:: each(a: auto(TT) const[-1])

each returns iterator<TT&>

.. warning:: 
  This is unsafe operation.

+--------+------------------+
+argument+argument type     +
+========+==================+
+a       +auto(TT) const[-1]+
+--------+------------------+


|function-builtin-each|

.. _function-_at__builtin__c__c_each_C1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: each(a: array<auto(TT)> const)

each returns iterator<TT&>

.. warning:: 
  This is unsafe operation.

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+a       +array<auto(TT)> const+
+--------+---------------------+


|function-builtin-each|

.. _function-_at__builtin__c__c_each_CN_ls_arg_gr_0_ls_Y_ls_argT_gr_._gr_1_ls_b_gr__at_:

.. das:function:: each(lam: lambda<(arg:auto(argT) -const):bool> const)

each returns iterator<argT>

+--------+-----------------------------------+
+argument+argument type                      +
+========+===================================+
+lam     +lambda<(arg:auto(argT)):bool> const+
+--------+-----------------------------------+


|function-builtin-each|

.. _function-_at__builtin__c__c_each_ref_CN_ls_arg_gr_0_ls_1_ls_Y_ls_argT_gr_._gr_?_gr_1_ls_b_gr__at_:

.. das:function:: each_ref(lam: lambda<(arg:auto(argT)? -const):bool> const)

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

.. _function-_at__builtin__c__c_to_array_C1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: to_array(it: iterator<auto(TT)> const)

to_array returns array<TT>

+--------+------------------------+
+argument+argument type           +
+========+========================+
+it      +iterator<auto(TT)> const+
+--------+------------------------+


|function-builtin-to_array|

.. _function-_at__builtin__c__c_to_array_C[-1]Y_ls_TT_gr_.:

.. das:function:: to_array(a: auto(TT) const[-1])

to_array returns array<TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+a       +auto(TT) const[-1]+
+--------+------------------+


|function-builtin-to_array|

.. _function-_at__builtin__c__c_to_array_move_[-1]Y_ls_TT_gr_.:

.. das:function:: to_array_move(a: auto(TT)[-1])

to_array_move returns array<TT>

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +auto(TT)[-1] +
+--------+-------------+


|function-builtin-to_array_move|

.. _function-_at__builtin__c__c_to_array_move_Y_ls_TT_gr_.:

.. das:function:: to_array_move(a: auto(TT))

to_array_move returns array<TT>

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +auto(TT)     +
+--------+-------------+


|function-builtin-to_array_move|

.. _function-_at__builtin__c__c_to_table_C[-1]0_ls_Y_ls_keyT_gr_.;Y_ls_valT_gr_._gr_U:

.. das:function:: to_table(a: tuple<auto(keyT);auto(valT)> const[-1])

to_table returns table<keyT;valT>

+--------+--------------------------------------+
+argument+argument type                         +
+========+======================================+
+a       +tuple<auto(keyT);auto(valT)> const[-1]+
+--------+--------------------------------------+


|function-builtin-to_table|

.. _function-_at__builtin__c__c_to_table_move_[-1]0_ls_Y_ls_keyT_gr_.;Y_ls_valT_gr_._gr_U:

.. das:function:: to_table_move(a: tuple<auto(keyT);auto(valT)>[-1])

to_table_move returns table<keyT;valT>

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+a       +tuple<auto(keyT);auto(valT)>[-1]+
+--------+--------------------------------+


|function-builtin-to_table_move|

.. _function-_at__builtin__c__c_sort_I[-1]Y_ls_TT_gr_.:

.. das:function:: sort(a: auto(TT)[-1] implicit)

sort returns auto

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+a       +auto(TT)[-1] implicit+
+--------+---------------------+


|function-builtin-sort|

.. _function-_at__builtin__c__c_sort_I1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: sort(a: array<auto(TT)> implicit)

sort returns auto

+--------+------------------------+
+argument+argument type           +
+========+========================+
+a       +array<auto(TT)> implicit+
+--------+------------------------+


|function-builtin-sort|

.. _function-_at__builtin__c__c_sort_I[-1]Y_ls_TT_gr_._CN_ls_x;y_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: sort(a: auto(TT)[-1] implicit; cmp: block<(x:TT const;y:TT const):bool> const)

sort returns auto

+--------+-----------------------------------------+
+argument+argument type                            +
+========+=========================================+
+a       +auto(TT)[-1] implicit                    +
+--------+-----------------------------------------+
+cmp     +block<(x:TT const;y:TT const):bool> const+
+--------+-----------------------------------------+


|function-builtin-sort|

.. _function-_at__builtin__c__c_sort_I1_ls_Y_ls_TT_gr_._gr_A_CN_ls_x;y_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: sort(a: array<auto(TT)> implicit; cmp: block<(x:TT const;y:TT const):bool> const)

sort returns auto

+--------+-----------------------------------------+
+argument+argument type                            +
+========+=========================================+
+a       +array<auto(TT)> implicit                 +
+--------+-----------------------------------------+
+cmp     +block<(x:TT const;y:TT const):bool> const+
+--------+-----------------------------------------+


|function-builtin-sort|

.. _function-_at__builtin__c__c_lock__eq_1_ls_Y_ls_TT_gr_._gr_A_CN_ls_x_gr_0_ls__hh_I1_ls_Y_ls_TT_gr_L_gr_A_gr_1_ls_._gr__builtin_:

.. das:function:: lock(a: array<auto(TT)> =const; blk: block<(x:array<TT># implicit -const):auto> const)

lock returns auto

+--------+-----------------------------------------+
+argument+argument type                            +
+========+=========================================+
+a       +array<auto(TT)>!                         +
+--------+-----------------------------------------+
+blk     +block<(x:array<TT># implicit):auto> const+
+--------+-----------------------------------------+


|function-builtin-lock|

.. _function-_at__builtin__c__c_lock_C_eq_1_ls_Y_ls_TT_gr_._gr_A_CN_ls_x_gr_0_ls_C_hh_I1_ls_Y_ls_TT_gr_L_gr_A_gr_1_ls_._gr__builtin_:

.. das:function:: lock(a: array<auto(TT)> const =const; blk: block<(x:array<TT> const# implicit):auto> const)

lock returns auto

+--------+-----------------------------------------------+
+argument+argument type                                  +
+========+===============================================+
+a       +array<auto(TT)> const!                         +
+--------+-----------------------------------------------+
+blk     +block<(x:array<TT> const# implicit):auto> const+
+--------+-----------------------------------------------+


|function-builtin-lock|

.. _function-_at__builtin__c__c_find_index_CI1_ls_Y_ls_TT_gr_._gr_A_CY_ls_TT_gr_L:

.. das:function:: find_index(arr: array<auto(TT)> const implicit; key: TT const)

find_index returns auto

+--------+------------------------------+
+argument+argument type                 +
+========+==============================+
+arr     +array<auto(TT)> const implicit+
+--------+------------------------------+
+key     +TT const                      +
+--------+------------------------------+


|function-builtin-find_index|

.. _function-_at__builtin__c__c_find_index_CI[-1]Y_ls_TT_gr_._CY_ls_TT_gr_L:

.. das:function:: find_index(arr: auto(TT) const[-1] implicit; key: TT const)

find_index returns auto

+--------+---------------------------+
+argument+argument type              +
+========+===========================+
+arr     +auto(TT) const[-1] implicit+
+--------+---------------------------+
+key     +TT const                   +
+--------+---------------------------+


|function-builtin-find_index|

.. _function-_at__builtin__c__c_find_index_C1_ls_Y_ls_TT_gr_._gr_G_CY_ls_TT_gr_L:

.. das:function:: find_index(arr: iterator<auto(TT)> const; key: TT const)

find_index returns auto

+--------+------------------------+
+argument+argument type           +
+========+========================+
+arr     +iterator<auto(TT)> const+
+--------+------------------------+
+key     +TT const                +
+--------+------------------------+


|function-builtin-find_index|

.. _function-_at__builtin__c__c_find_index_if_CI1_ls_Y_ls_TT_gr_._gr_A_CN_ls_key_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: find_index_if(arr: array<auto(TT)> const implicit; blk: block<(key:TT const):bool> const)

find_index_if returns auto

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+arr     +array<auto(TT)> const implicit  +
+--------+--------------------------------+
+blk     +block<(key:TT const):bool> const+
+--------+--------------------------------+


|function-builtin-find_index_if|

.. _function-_at__builtin__c__c_find_index_if_CI[-1]Y_ls_TT_gr_._CN_ls_key_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: find_index_if(arr: auto(TT) const[-1] implicit; blk: block<(key:TT const):bool> const)

find_index_if returns auto

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+arr     +auto(TT) const[-1] implicit     +
+--------+--------------------------------+
+blk     +block<(key:TT const):bool> const+
+--------+--------------------------------+


|function-builtin-find_index_if|

.. _function-_at__builtin__c__c_find_index_if_C1_ls_Y_ls_TT_gr_._gr_G_CN_ls_key_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: find_index_if(arr: iterator<auto(TT)> const; blk: block<(key:TT const -&):bool> const)

find_index_if returns auto

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+arr     +iterator<auto(TT)> const        +
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

++++++++++++++++++++++++
das::string manipulation
++++++++++++++++++++++++

  *  :ref:`peek (src:$::das_string const implicit;block:block\<(string const#):void\> const implicit;context:__context const;line:__lineInfo const) : void <function-_at__builtin__c__c_peek_CIH_ls__builtin__c__c_das_string_gr__CI0_ls_C_hh_s_gr_1_ls_v_gr__builtin__C_c_C_l>` 

.. _function-_at__builtin__c__c_peek_CIH_ls__builtin__c__c_das_string_gr__CI0_ls_C_hh_s_gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: peek(src: das_string const implicit; block: block<(string const#):void> const implicit)

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

  *  :ref:`heap_bytes_allocated (context:__context const) : uint64 <function-_at__builtin__c__c_heap_bytes_allocated_C_c>` 
  *  :ref:`heap_depth (context:__context const) : int <function-_at__builtin__c__c_heap_depth_C_c>` 
  *  :ref:`string_heap_bytes_allocated (context:__context const) : uint64 <function-_at__builtin__c__c_string_heap_bytes_allocated_C_c>` 
  *  :ref:`string_heap_depth (context:__context const) : int <function-_at__builtin__c__c_string_heap_depth_C_c>` 
  *  :ref:`string_heap_collect (validate:bool const;context:__context const;at:__lineInfo const) : void <function-_at__builtin__c__c_string_heap_collect_Cb_C_c_C_l>` 
  *  :ref:`heap_collect (string_heap:bool const;validate:bool const;context:__context const;at:__lineInfo const) : void <function-_at__builtin__c__c_heap_collect_Cb_Cb_C_c_C_l>` 
  *  :ref:`string_heap_report (context:__context const;line:__lineInfo const) : void <function-_at__builtin__c__c_string_heap_report_C_c_C_l>` 
  *  :ref:`heap_report (context:__context const;line:__lineInfo const) : void <function-_at__builtin__c__c_heap_report_C_c_C_l>` 

.. _function-_at__builtin__c__c_heap_bytes_allocated_C_c:

.. das:function:: heap_bytes_allocated()

heap_bytes_allocated returns uint64

|function-builtin-heap_bytes_allocated|

.. _function-_at__builtin__c__c_heap_depth_C_c:

.. das:function:: heap_depth()

heap_depth returns int

|function-builtin-heap_depth|

.. _function-_at__builtin__c__c_string_heap_bytes_allocated_C_c:

.. das:function:: string_heap_bytes_allocated()

string_heap_bytes_allocated returns uint64

|function-builtin-string_heap_bytes_allocated|

.. _function-_at__builtin__c__c_string_heap_depth_C_c:

.. das:function:: string_heap_depth()

string_heap_depth returns int

|function-builtin-string_heap_depth|

.. _function-_at__builtin__c__c_string_heap_collect_Cb_C_c_C_l:

.. das:function:: string_heap_collect(validate: bool const)

.. warning:: 
  This is unsafe operation.

+--------+-------------+
+argument+argument type+
+========+=============+
+validate+bool const   +
+--------+-------------+


|function-builtin-string_heap_collect|

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

++++++++++++++++++
GC0 infrastructure
++++++++++++++++++

  *  :ref:`gc0_save_ptr (name:string const implicit;data:void? const implicit;context:__context const;line:__lineInfo const) : void <function-_at__builtin__c__c_gc0_save_ptr_CIs_CI?_C_c_C_l>` 
  *  :ref:`gc0_save_smart_ptr (name:string const implicit;data:smart_ptr\<void\> const implicit;context:__context const;line:__lineInfo const) : void <function-_at__builtin__c__c_gc0_save_smart_ptr_CIs_CI1_ls_v_gr_?M_C_c_C_l>` 
  *  :ref:`gc0_restore_ptr (name:string const implicit;context:__context const) : void? <function-_at__builtin__c__c_gc0_restore_ptr_CIs_C_c>` 
  *  :ref:`gc0_restore_smart_ptr (name:string const implicit;context:__context const) : smart_ptr\<void\> <function-_at__builtin__c__c_gc0_restore_smart_ptr_CIs_C_c>` 
  *  :ref:`gc0_reset () : void <function-_at__builtin__c__c_gc0_reset>` 

.. _function-_at__builtin__c__c_gc0_save_ptr_CIs_CI?_C_c_C_l:

.. das:function:: gc0_save_ptr(name: string const implicit; data: void? const implicit)

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+name    +string const implicit+
+--------+---------------------+
+data    +void? const implicit +
+--------+---------------------+


|function-builtin-gc0_save_ptr|

.. _function-_at__builtin__c__c_gc0_save_smart_ptr_CIs_CI1_ls_v_gr_?M_C_c_C_l:

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

  *  :ref:`smart_ptr_clone (dest:smart_ptr\<void\>& implicit;src:void? const implicit) : void <function-_at__builtin__c__c_smart_ptr_clone_&I1_ls_v_gr_?M_CI?>` 
  *  :ref:`smart_ptr_clone (dest:smart_ptr\<void\>& implicit;src:smart_ptr\<void\> const implicit) : void <function-_at__builtin__c__c_smart_ptr_clone_&I1_ls_v_gr_?M_CI1_ls_v_gr_?M>` 
  *  :ref:`smart_ptr_use_count (ptr:smart_ptr\<void\> const implicit) : uint <function-_at__builtin__c__c_smart_ptr_use_count_CI1_ls_v_gr_?M>` 
  *  :ref:`get_ptr (src:smart_ptr\<auto(TT)\> const) : TT? <function-_at__builtin__c__c_get_ptr_C1_ls_Y_ls_TT_gr_._gr_?M>` 

.. _function-_at__builtin__c__c_smart_ptr_clone_&I1_ls_v_gr_?M_CI?:

.. das:function:: smart_ptr_clone(dest: smart_ptr<void>& implicit; src: void? const implicit)

+--------+-------------------------+
+argument+argument type            +
+========+=========================+
+dest    +smart_ptr<void>& implicit+
+--------+-------------------------+
+src     +void? const implicit     +
+--------+-------------------------+


|function-builtin-smart_ptr_clone|

.. _function-_at__builtin__c__c_smart_ptr_clone_&I1_ls_v_gr_?M_CI1_ls_v_gr_?M:

.. das:function:: smart_ptr_clone(dest: smart_ptr<void>& implicit; src: smart_ptr<void> const implicit)

+--------+------------------------------+
+argument+argument type                 +
+========+==============================+
+dest    +smart_ptr<void>& implicit     +
+--------+------------------------------+
+src     +smart_ptr<void> const implicit+
+--------+------------------------------+


|function-builtin-smart_ptr_clone|

.. _function-_at__builtin__c__c_smart_ptr_use_count_CI1_ls_v_gr_?M:

.. das:function:: smart_ptr_use_count(ptr: smart_ptr<void> const implicit)

smart_ptr_use_count returns uint

+--------+------------------------------+
+argument+argument type                 +
+========+==============================+
+ptr     +smart_ptr<void> const implicit+
+--------+------------------------------+


|function-builtin-smart_ptr_use_count|

.. _function-_at__builtin__c__c_get_ptr_C1_ls_Y_ls_TT_gr_._gr_?M:

.. das:function:: get_ptr(src: smart_ptr<auto(TT)> const)

get_ptr returns TT?

+--------+-------------------------+
+argument+argument type            +
+========+=========================+
+src     +smart_ptr<auto(TT)> const+
+--------+-------------------------+


|function-builtin-get_ptr|

++++++++++++++++++++
Macro infrastructure
++++++++++++++++++++

  *  :ref:`is_compiling (context:__context const) : bool <function-_at__builtin__c__c_is_compiling_C_c>` 
  *  :ref:`is_compiling_macros (context:__context const) : bool <function-_at__builtin__c__c_is_compiling_macros_C_c>` 
  *  :ref:`is_compiling_macros_in_module (name:string const implicit;context:__context const) : bool <function-_at__builtin__c__c_is_compiling_macros_in_module_CIs_C_c>` 

.. _function-_at__builtin__c__c_is_compiling_C_c:

.. das:function:: is_compiling()

is_compiling returns bool

|function-builtin-is_compiling|

.. _function-_at__builtin__c__c_is_compiling_macros_C_c:

.. das:function:: is_compiling_macros()

is_compiling_macros returns bool

|function-builtin-is_compiling_macros|

.. _function-_at__builtin__c__c_is_compiling_macros_in_module_CIs_C_c:

.. das:function:: is_compiling_macros_in_module(name: string const implicit)

is_compiling_macros_in_module returns bool

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+name    +string const implicit+
+--------+---------------------+


|function-builtin-is_compiling_macros_in_module|

++++++++
Profiler
++++++++

  *  :ref:`reset_profiler (context:__context const) : void <function-_at__builtin__c__c_reset_profiler_C_c>` 
  *  :ref:`dump_profile_info (context:__context const) : void <function-_at__builtin__c__c_dump_profile_info_C_c>` 
  *  :ref:`collect_profile_info (context:__context const) : string <function-_at__builtin__c__c_collect_profile_info_C_c>` 
  *  :ref:`profile (count:int const;category:string const implicit;block:block\<\> const implicit;context:__context const;line:__lineInfo const) : float <function-_at__builtin__c__c_profile_Ci_CIs_CI_builtin__C_c_C_l>` 

.. _function-_at__builtin__c__c_reset_profiler_C_c:

.. das:function:: reset_profiler()

|function-builtin-reset_profiler|

.. _function-_at__builtin__c__c_dump_profile_info_C_c:

.. das:function:: dump_profile_info()

|function-builtin-dump_profile_info|

.. _function-_at__builtin__c__c_collect_profile_info_C_c:

.. das:function:: collect_profile_info()

collect_profile_info returns string

|function-builtin-collect_profile_info|

.. _function-_at__builtin__c__c_profile_Ci_CIs_CI_builtin__C_c_C_l:

.. das:function:: profile(count: int const; category: string const implicit; block: block<> const implicit)

profile returns float

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

  *  :ref:`get_das_root (context:__context const) : string <function-_at__builtin__c__c_get_das_root_C_c>` 
  *  :ref:`panic (text:string const implicit;context:__context const;at:__lineInfo const) : void <function-_at__builtin__c__c_panic_CIs_C_c_C_l>` 
  *  :ref:`print (text:string const implicit;context:__context const) : void <function-_at__builtin__c__c_print_CIs_C_c>` 
  *  :ref:`sprint (value:any const;flags:bitfield\<escapeString;namesAndDimensions;typeQualifiers;refAddresses;humanReadable;singleLine\> const) : string <function-_at__builtin__c__c_sprint_C*_CY_ls_print_flags_gr_N_ls_escapeString;namesAndDimensions;typeQualifiers;refAddresses;humanReadable;singleLine_gr_t>` 
  *  :ref:`terminate (context:__context const) : void <function-_at__builtin__c__c_terminate_C_c>` 
  *  :ref:`breakpoint () : void <function-_at__builtin__c__c_breakpoint>` 
  *  :ref:`stackwalk (args:bool const;vars:bool const;context:__context const;lineinfo:__lineInfo const) : void <function-_at__builtin__c__c_stackwalk_Cb_Cb_C_c_C_l>` 
  *  :ref:`is_in_aot () : bool <function-_at__builtin__c__c_is_in_aot>` 

.. _function-_at__builtin__c__c_get_das_root_C_c:

.. das:function:: get_das_root()

get_das_root returns string

|function-builtin-get_das_root|

.. _function-_at__builtin__c__c_panic_CIs_C_c_C_l:

.. das:function:: panic(text: string const implicit)

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+text    +string const implicit+
+--------+---------------------+


|function-builtin-panic|

.. _function-_at__builtin__c__c_print_CIs_C_c:

.. das:function:: print(text: string const implicit)

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+text    +string const implicit+
+--------+---------------------+


|function-builtin-print|

.. _function-_at__builtin__c__c_sprint_C*_CY_ls_print_flags_gr_N_ls_escapeString;namesAndDimensions;typeQualifiers;refAddresses;humanReadable;singleLine_gr_t:

.. das:function:: sprint(value: any const; flags: print_flags)

sprint returns string

+--------+----------------------------------------+
+argument+argument type                           +
+========+========================================+
+value   +any const                               +
+--------+----------------------------------------+
+flags   + :ref:`print_flags <alias-print_flags>` +
+--------+----------------------------------------+


|function-builtin-sprint|

.. _function-_at__builtin__c__c_terminate_C_c:

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

.. _function-_at__builtin__c__c_is_in_aot:

.. das:function:: is_in_aot()

is_in_aot returns bool

|function-builtin-is_in_aot|

+++++++++++++++++++
Memory manipulation
+++++++++++++++++++

  *  :ref:`variant_index (arg0:variant\<\> const implicit) : int <function-_at__builtin__c__c_variant_index_CIV>` 
  *  :ref:`set_variant_index (variant:variant\<\> implicit;index:int const) : void <function-_at__builtin__c__c_set_variant_index_IV_Ci>` 
  *  :ref:`hash (data:any const) : uint64 <function-_at__builtin__c__c_hash_C*>` 
  *  :ref:`memcpy (left:void? const implicit;right:void? const implicit;size:int const) : void <function-_at__builtin__c__c_memcpy_CI?_CI?_Ci>` 
  *  :ref:`memcmp (left:void? const implicit;right:void? const implicit;size:int const) : int <function-_at__builtin__c__c_memcmp_CI?_CI?_Ci>` 
  *  :ref:`intptr (p:void? const) : uint64 <function-_at__builtin__c__c_intptr_C1_ls_v_gr_?>` 
  *  :ref:`intptr (p:smart_ptr\<auto\> const) : uint64 <function-_at__builtin__c__c_intptr_C1_ls_._gr_?M>` 
  *  :ref:`lock_data (a:array\<auto(TT)\> =const -const;blk:block\<(p:TT?# implicit -const;s:int const):auto\> const) : auto <function-_at__builtin__c__c_lock_data__eq_1_ls_Y_ls_TT_gr_._gr_A_CN_ls_p;s_gr_0_ls__hh_I1_ls_Y_ls_TT_gr_L_gr_?;Ci_gr_1_ls_._gr__builtin_>` 
  *  :ref:`lock_data (a:array\<auto(TT)\> const =const;blk:block\<(p:TT const? const# implicit;s:int const):auto\> const) : auto <function-_at__builtin__c__c_lock_data_C_eq_1_ls_Y_ls_TT_gr_._gr_A_CN_ls_p;s_gr_0_ls_C_hh_I1_ls_CY_ls_TT_gr_L_gr_?;Ci_gr_1_ls_._gr__builtin_>` 
  *  :ref:`map_to_array (data:void? const;len:int const;blk:block\<(arg:array\<auto(TT)\># -const):auto\> const) : auto <function-_at__builtin__c__c_map_to_array_C1_ls_v_gr_?_Ci_CN_ls_arg_gr_0_ls__hh_1_ls_Y_ls_TT_gr_._gr_A_gr_1_ls_._gr__builtin_>` 
  *  :ref:`map_to_ro_array (data:void? const;len:int const;blk:block\<(arg:array\<auto(TT)\> const#):auto\> const) : auto <function-_at__builtin__c__c_map_to_ro_array_C1_ls_v_gr_?_Ci_CN_ls_arg_gr_0_ls_C_hh_1_ls_Y_ls_TT_gr_._gr_A_gr_1_ls_._gr__builtin_>` 

.. _function-_at__builtin__c__c_variant_index_CIV:

.. das:function:: variant_index(arg0: variant<> const implicit)

variant_index returns int

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

.. _function-_at__builtin__c__c_hash_C*:

.. das:function:: hash(data: any const)

hash returns uint64

+--------+-------------+
+argument+argument type+
+========+=============+
+data    +any const    +
+--------+-------------+


|function-builtin-hash|

.. _function-_at__builtin__c__c_memcpy_CI?_CI?_Ci:

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

.. _function-_at__builtin__c__c_memcmp_CI?_CI?_Ci:

.. das:function:: memcmp(left: void? const implicit; right: void? const implicit; size: int const)

memcmp returns int

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

.. _function-_at__builtin__c__c_intptr_C1_ls_v_gr_?:

.. das:function:: intptr(p: void? const)

intptr returns uint64

+--------+-------------+
+argument+argument type+
+========+=============+
+p       +void? const  +
+--------+-------------+


|function-builtin-intptr|

.. _function-_at__builtin__c__c_intptr_C1_ls_._gr_?M:

.. das:function:: intptr(p: smart_ptr<auto> const)

intptr returns uint64

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+p       +smart_ptr<auto> const+
+--------+---------------------+


|function-builtin-intptr|

.. _function-_at__builtin__c__c_lock_data__eq_1_ls_Y_ls_TT_gr_._gr_A_CN_ls_p;s_gr_0_ls__hh_I1_ls_Y_ls_TT_gr_L_gr_?;Ci_gr_1_ls_._gr__builtin_:

.. das:function:: lock_data(a: array<auto(TT)> =const; blk: block<(p:TT?# implicit -const;s:int const):auto> const)

lock_data returns auto

+--------+-----------------------------------------------+
+argument+argument type                                  +
+========+===============================================+
+a       +array<auto(TT)>!                               +
+--------+-----------------------------------------------+
+blk     +block<(p:TT?# implicit;s:int const):auto> const+
+--------+-----------------------------------------------+


|function-builtin-lock_data|

.. _function-_at__builtin__c__c_lock_data_C_eq_1_ls_Y_ls_TT_gr_._gr_A_CN_ls_p;s_gr_0_ls_C_hh_I1_ls_CY_ls_TT_gr_L_gr_?;Ci_gr_1_ls_._gr__builtin_:

.. das:function:: lock_data(a: array<auto(TT)> const =const; blk: block<(p:TT const? const# implicit;s:int const):auto> const)

lock_data returns auto

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+a       +array<auto(TT)> const!                                     +
+--------+-----------------------------------------------------------+
+blk     +block<(p:TT const? const# implicit;s:int const):auto> const+
+--------+-----------------------------------------------------------+


|function-builtin-lock_data|

.. _function-_at__builtin__c__c_map_to_array_C1_ls_v_gr_?_Ci_CN_ls_arg_gr_0_ls__hh_1_ls_Y_ls_TT_gr_._gr_A_gr_1_ls_._gr__builtin_:

.. das:function:: map_to_array(data: void? const; len: int const; blk: block<(arg:array<auto(TT)># -const):auto> const)

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

.. _function-_at__builtin__c__c_map_to_ro_array_C1_ls_v_gr_?_Ci_CN_ls_arg_gr_0_ls_C_hh_1_ls_Y_ls_TT_gr_._gr_A_gr_1_ls_._gr__builtin_:

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

  *  :ref:`binary_save (obj:auto const;subexpr:block\<(data:array\<uint8\> const):void\> const) : auto <function-_at__builtin__c__c_binary_save_C._CN_ls_data_gr_0_ls_C1_ls_u8_gr_A_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`binary_load (obj:auto -const;data:array\<uint8\> const) : auto <function-_at__builtin__c__c_binary_load_._C1_ls_u8_gr_A>` 

.. _function-_at__builtin__c__c_binary_save_C._CN_ls_data_gr_0_ls_C1_ls_u8_gr_A_gr_1_ls_v_gr__builtin_:

.. das:function:: binary_save(obj: auto const; subexpr: block<(data:array<uint8> const):void> const)

binary_save returns auto

+--------+-------------------------------------------+
+argument+argument type                              +
+========+===========================================+
+obj     +auto const                                 +
+--------+-------------------------------------------+
+subexpr +block<(data:array<uint8> const):void> const+
+--------+-------------------------------------------+


|function-builtin-binary_save|

.. _function-_at__builtin__c__c_binary_load_._C1_ls_u8_gr_A:

.. das:function:: binary_load(obj: auto; data: array<uint8> const)

binary_load returns auto

+--------+------------------+
+argument+argument type     +
+========+==================+
+obj     +auto              +
+--------+------------------+
+data    +array<uint8> const+
+--------+------------------+


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
  *  :ref:`ref_time_ticks () : int64 <function-_at__builtin__c__c_ref_time_ticks>` 
  *  :ref:`get_time_usec (arg0:int64 const) : int <function-_at__builtin__c__c_get_time_usec_Ci64>` 

.. _function-_at__builtin__c__c_get_clock:

.. das:function:: get_clock()

get_clock returns  :ref:`builtin::clock <handle-builtin-clock>` 

|function-builtin-get_clock|

.. _function-_at__builtin__c__c_ref_time_ticks:

.. das:function:: ref_time_ticks()

ref_time_ticks returns int64

|function-builtin-ref_time_ticks|

.. _function-_at__builtin__c__c_get_time_usec_Ci64:

.. das:function:: get_time_usec(arg0: int64 const)

get_time_usec returns int

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +int64 const  +
+--------+-------------+


|function-builtin-get_time_usec|

++++++++++
Algorithms
++++++++++

  *  :ref:`iter_range (foo:auto const) : auto <function-_at__builtin__c__c_iter_range_C.>` 
  *  :ref:`swap (a:auto(TT)& -const;b:auto(TT)& -const) : auto <function-_at__builtin__c__c_swap_&Y_ls_TT_gr_._&Y_ls_TT_gr_.>` 

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

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at__builtin__c__c_interval_Ci_Ci:

.. das:function:: interval(arg0: int const; arg1: int const)

interval returns range

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

interval returns urange

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +uint const   +
+--------+-------------+
+arg1    +uint const   +
+--------+-------------+


|function-builtin-interval|

.. _function-_at__builtin__c__c_lock_count_CIA:

.. das:function:: lock_count(array: array const implicit)

lock_count returns int

+--------+--------------------+
+argument+argument type       +
+========+====================+
+array   +array const implicit+
+--------+--------------------+


|function-builtin-lock_count|

.. _function-_at__builtin__c__c_error_CIs_C_c:

.. das:function:: error(text: string const implicit)

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+text    +string const implicit+
+--------+---------------------+


|function-builtin-error|

.. _function-_at__builtin__c__c_memory_report_Cb_C_c_C_l:

.. das:function:: memory_report(errorsOnly: bool const)

+----------+-------------+
+argument  +argument type+
+==========+=============+
+errorsOnly+bool const   +
+----------+-------------+


|function-builtin-memory_report|

.. _function-_at__builtin__c__c_class_rtti_size_CI?:

.. das:function:: class_rtti_size(ptr: void? const implicit)

class_rtti_size returns int

+--------+--------------------+
+argument+argument type       +
+========+====================+
+ptr     +void? const implicit+
+--------+--------------------+


|function-builtin-class_rtti_size|

.. _function-_at__builtin__c__c_to_log_Ci_CIs:

.. das:function:: to_log(arg0: int const; arg1: string const implicit)

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+arg0    +int const            +
+--------+---------------------+
+arg1    +string const implicit+
+--------+---------------------+


|function-builtin-to_log|

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

.. das:function:: _return_with_lockcheck(a: auto(valT)& =const)

_return_with_lockcheck returns valT&

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +auto(valT)&! +
+--------+-------------+


|function-builtin-_return_with_lockcheck|

.. _function-_at__builtin__c__c__return_with_lockcheck_C&_eq_Y_ls_valT_gr_.:

.. das:function:: _return_with_lockcheck(a: auto(valT) const& =const)

_return_with_lockcheck returns valT&

+--------+------------------+
+argument+argument type     +
+========+==================+
+a       +auto(valT) const&!+
+--------+------------------+


|function-builtin-_return_with_lockcheck|

.. _function-_at__builtin__c__c_back__eq_1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: back(a: array<auto(TT)> =const)

back returns TT&

+--------+----------------+
+argument+argument type   +
+========+================+
+a       +array<auto(TT)>!+
+--------+----------------+


|function-builtin-back|

.. _function-_at__builtin__c__c_back_C_eq_1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: back(a: array<auto(TT)> const =const)

back returns TT const&

+--------+----------------------+
+argument+argument type         +
+========+======================+
+a       +array<auto(TT)> const!+
+--------+----------------------+


|function-builtin-back|

.. _function-_at__builtin__c__c__at_with_lockcheck_1_ls_Y_ls_keyT_gr_._gr_2_ls_Y_ls_valT_gr_._gr_T_CIY_ls_keyT_gr_L:

.. das:function:: _at_with_lockcheck(Tab: table<auto(keyT);auto(valT)>; at: keyT const implicit)

_at_with_lockcheck returns valT&

+--------+----------------------------+
+argument+argument type               +
+========+============================+
+Tab     +table<auto(keyT);auto(valT)>+
+--------+----------------------------+
+at      +keyT const implicit         +
+--------+----------------------------+


|function-builtin-_at_with_lockcheck|

.. _function-_at__builtin__c__c_get_const_ptr_C1_ls_Y_ls_TT_gr_._gr_?M:

.. das:function:: get_const_ptr(src: smart_ptr<auto(TT)> const)

get_const_ptr returns TT? const

+--------+-------------------------+
+argument+argument type            +
+========+=========================+
+src     +smart_ptr<auto(TT)> const+
+--------+-------------------------+


|function-builtin-get_const_ptr|

.. _function-_at__builtin__c__c_subarray_C[-1]Y_ls_TT_gr_._Cr:

.. das:function:: subarray(a: auto(TT) const[-1]; r: range const)

subarray returns auto

+--------+------------------+
+argument+argument type     +
+========+==================+
+a       +auto(TT) const[-1]+
+--------+------------------+
+r       +range const       +
+--------+------------------+


|function-builtin-subarray|

.. _function-_at__builtin__c__c_subarray_C[-1]Y_ls_TT_gr_._Cz:

.. das:function:: subarray(a: auto(TT) const[-1]; r: urange const)

subarray returns auto

+--------+------------------+
+argument+argument type     +
+========+==================+
+a       +auto(TT) const[-1]+
+--------+------------------+
+r       +urange const      +
+--------+------------------+


|function-builtin-subarray|

.. _function-_at__builtin__c__c_subarray_C1_ls_Y_ls_TT_gr_._gr_A_Cr:

.. das:function:: subarray(a: array<auto(TT)> const; r: range const)

subarray returns auto

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+a       +array<auto(TT)> const+
+--------+---------------------+
+r       +range const          +
+--------+---------------------+


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


