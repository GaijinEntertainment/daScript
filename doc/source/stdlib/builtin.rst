
.. _stdlib_$:

================
Built-in runtime
================

++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-builtin-StringBuilderWriter:

.. das:attribute:: StringBuilderWriter

|structure_annotation-builtin-StringBuilderWriter|

+++++++++++++
Handled types
+++++++++++++

.. _handle-builtin-clock:

.. das:attribute:: clock

|any_annotation-builtin-clock|

.. _handle-builtin-das_string:

.. das:attribute:: das_string

|any_annotation-builtin-das_string|

.. _handle-builtin-float3x4:

.. das:attribute:: float3x4

|any_annotation-builtin-float3x4|

.. _handle-builtin-float4x4:

.. das:attribute:: float4x4

|any_annotation-builtin-float4x4|

++++++++++++++++++++++++++
Internal builtin functions
++++++++++++++++++++++++++

.. warning:: 
  This group of functions is hidden. It will not be in the final documentation.

  *  :ref:`builtin_strdup (arg0:any) : void <function-_at__builtin__c__c_builtin_strdup_any>` 
  *  :ref:`builtin_string_split (arg0:string const;arg1:string const;arg2:block\<\> const implicit;arg3: const) : void <function-_at__builtin__c__c_builtin_string_split_string_hh_const_string_hh_const__hh_block_hh_const_hh_implicit__hh_const>` 
  *  :ref:`builtin_string_split_by_char (arg0:string const;arg1:string const;arg2:block\<\> const implicit;arg3: const) : void <function-_at__builtin__c__c_builtin_string_split_by_char_string_hh_const_string_hh_const__hh_block_hh_const_hh_implicit__hh_const>` 

.. _function-_at__builtin__c__c_builtin_strdup_any:

.. das:function:: builtin_strdup(arg0: any)

.. warning:: 
  This is unsafe operation.

arguments are

+----+---+
+arg0+any+
+----+---+


|function-builtin-builtin_strdup|

.. _function-_at__builtin__c__c_builtin_string_split_string_hh_const_string_hh_const__hh_block_hh_const_hh_implicit__hh_const:

.. das:function:: builtin_string_split(arg0: string const; arg1: string const; arg2: block<> const implicit)

arguments are

+----+----------------------+
+arg0+string const          +
+----+----------------------+
+arg1+string const          +
+----+----------------------+
+arg2+block<> const implicit+
+----+----------------------+


|function-builtin-builtin_string_split|

.. _function-_at__builtin__c__c_builtin_string_split_by_char_string_hh_const_string_hh_const__hh_block_hh_const_hh_implicit__hh_const:

.. das:function:: builtin_string_split_by_char(arg0: string const; arg1: string const; arg2: block<> const implicit)

arguments are

+----+----------------------+
+arg0+string const          +
+----+----------------------+
+arg1+string const          +
+----+----------------------+
+arg2+block<> const implicit+
+----+----------------------+


|function-builtin-builtin_string_split_by_char|

++++++++++++++++++++++++++++
Internal pointer arithmetics
++++++++++++++++++++++++++++

.. warning:: 
  This group of functions is hidden. It will not be in the final documentation.

  *  :ref:`i_das_ptr_add (arg0:void? const implicit;arg1:int const;arg2:int const) : void? <function-_at__builtin__c__c_i_das_ptr_add__hh_ptr_hh_const_hh_implicit_int_hh_const_int_hh_const>` 
  *  :ref:`i_das_ptr_dec (arg0:void?& implicit;arg1:int const) : void <function-_at__builtin__c__c_i_das_ptr_dec__hh_ptr_hh_ref_hh_implicit_int_hh_const>` 
  *  :ref:`i_das_ptr_diff (arg0:void? const implicit;arg1:void? const implicit;arg2:int const) : int64 <function-_at__builtin__c__c_i_das_ptr_diff__hh_ptr_hh_const_hh_implicit__hh_ptr_hh_const_hh_implicit_int_hh_const>` 
  *  :ref:`i_das_ptr_inc (arg0:void?& implicit;arg1:int const) : void <function-_at__builtin__c__c_i_das_ptr_inc__hh_ptr_hh_ref_hh_implicit_int_hh_const>` 
  *  :ref:`i_das_ptr_set_add (arg0:void?& implicit;arg1:int const;arg2:int const) : void <function-_at__builtin__c__c_i_das_ptr_set_add__hh_ptr_hh_ref_hh_implicit_int_hh_const_int_hh_const>` 
  *  :ref:`i_das_ptr_set_sub (arg0:void?& implicit;arg1:int const;arg2:int const) : void <function-_at__builtin__c__c_i_das_ptr_set_sub__hh_ptr_hh_ref_hh_implicit_int_hh_const_int_hh_const>` 
  *  :ref:`i_das_ptr_sub (arg0:void?& implicit;arg1:int const;arg2:int const) : void? <function-_at__builtin__c__c_i_das_ptr_sub__hh_ptr_hh_ref_hh_implicit_int_hh_const_int_hh_const>` 

.. _function-_at__builtin__c__c_i_das_ptr_add__hh_ptr_hh_const_hh_implicit_int_hh_const_int_hh_const:

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


|function-builtin-i_das_ptr_add|

.. _function-_at__builtin__c__c_i_das_ptr_dec__hh_ptr_hh_ref_hh_implicit_int_hh_const:

.. das:function:: i_das_ptr_dec(arg0: void?& implicit; arg1: int const)

.. warning:: 
  This is unsafe operation.

arguments are

+----+---------------+
+arg0+void?& implicit+
+----+---------------+
+arg1+int const      +
+----+---------------+


|function-builtin-i_das_ptr_dec|

.. _function-_at__builtin__c__c_i_das_ptr_diff__hh_ptr_hh_const_hh_implicit__hh_ptr_hh_const_hh_implicit_int_hh_const:

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


|function-builtin-i_das_ptr_diff|

.. _function-_at__builtin__c__c_i_das_ptr_inc__hh_ptr_hh_ref_hh_implicit_int_hh_const:

.. das:function:: i_das_ptr_inc(arg0: void?& implicit; arg1: int const)

.. warning:: 
  This is unsafe operation.

arguments are

+----+---------------+
+arg0+void?& implicit+
+----+---------------+
+arg1+int const      +
+----+---------------+


|function-builtin-i_das_ptr_inc|

.. _function-_at__builtin__c__c_i_das_ptr_set_add__hh_ptr_hh_ref_hh_implicit_int_hh_const_int_hh_const:

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


|function-builtin-i_das_ptr_set_add|

.. _function-_at__builtin__c__c_i_das_ptr_set_sub__hh_ptr_hh_ref_hh_implicit_int_hh_const_int_hh_const:

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


|function-builtin-i_das_ptr_set_sub|

.. _function-_at__builtin__c__c_i_das_ptr_sub__hh_ptr_hh_ref_hh_implicit_int_hh_const_int_hh_const:

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


|function-builtin-i_das_ptr_sub|

+++++++++++++++++++++++++++++
Internal clone infrastructure
+++++++++++++++++++++++++++++

.. warning:: 
  This group of functions is hidden. It will not be in the final documentation.

  *  :ref:`clone (a:$::das_string -const;b:string const) : void <function-_at__builtin__c__c_clone__hh_handle_hh_das_string_string_hh_const>` 
  *  :ref:`clone_string (arg0:string const;arg1: const) : string <function-_at__builtin__c__c_clone_string_string_hh_const__hh_const>` 
  *  :ref:`clone (a:array\<auto(TT)\> -const;b:array\<TT\> const) : auto <function-_at__builtin__c__c_clone__hh_array_hh__hh_auto__hh_array_hh__hh_alias_hh_TT_hh_const>` 
  *  :ref:`clone (a:array\<auto(TT)\> -const;b:array\<TT\> const#) : auto <function-_at__builtin__c__c_clone__hh_array_hh__hh_auto__hh_array_hh__hh_alias_hh_TT_hh_const_hh_temporary>` 
  *  :ref:`clone (a:table\<auto(KT);auto(VT)\> -const;b:table\<KT;VT\> const) : auto <function-_at__builtin__c__c_clone__hh_table_hh__hh_auto_hh__hh_auto__hh_table_hh__hh_alias_hh_KT_hh__hh_alias_hh_VT_hh_const>` 
  *  :ref:`clone (a:table\<auto(KT);auto(VT)\> -const;b:table\<KT;VT\> const#) : auto <function-_at__builtin__c__c_clone__hh_table_hh__hh_auto_hh__hh_auto__hh_table_hh__hh_alias_hh_KT_hh__hh_alias_hh_VT_hh_const_hh_temporary>` 
  *  :ref:`clone_dim (a:auto -const;b:auto const) : auto <function-_at__builtin__c__c_clone_dim__hh_auto__hh_auto_hh_const>` 
  *  :ref:`clone_dim (a:auto -const;b:auto const#) : auto <function-_at__builtin__c__c_clone_dim__hh_auto__hh_auto_hh_const_hh_temporary>` 
  *  :ref:`clone_to_move (clone_src:auto(TT) const) : TT -const <function-_at__builtin__c__c_clone_to_move__hh_auto_hh_const>` 

.. _function-_at__builtin__c__c_clone__hh_handle_hh_das_string_string_hh_const:

.. das:function:: clone(a: das_string; b: string const)

arguments are

+-+--------------------------------------------------------+
+a+ :ref:`builtin::das_string <handle-builtin-das_string>` +
+-+--------------------------------------------------------+
+b+string const                                            +
+-+--------------------------------------------------------+


|function-builtin-clone|

.. _function-_at__builtin__c__c_clone_string_string_hh_const__hh_const:

.. das:function:: clone_string(arg0: string const)

clone_string returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+


|function-builtin-clone_string|

.. _function-_at__builtin__c__c_clone__hh_array_hh__hh_auto__hh_array_hh__hh_alias_hh_TT_hh_const:

.. das:function:: clone(a: array<auto(TT)>; b: array<TT> const)

clone returns auto

arguments are

+-+---------------+
+a+array<auto(TT)>+
+-+---------------+
+b+array<TT> const+
+-+---------------+


|function-builtin-clone|

.. _function-_at__builtin__c__c_clone__hh_array_hh__hh_auto__hh_array_hh__hh_alias_hh_TT_hh_const_hh_temporary:

.. das:function:: clone(a: array<auto(TT)>; b: array<TT> const#)

clone returns auto

arguments are

+-+----------------+
+a+array<auto(TT)> +
+-+----------------+
+b+array<TT> const#+
+-+----------------+


|function-builtin-clone|

.. _function-_at__builtin__c__c_clone__hh_table_hh__hh_auto_hh__hh_auto__hh_table_hh__hh_alias_hh_KT_hh__hh_alias_hh_VT_hh_const:

.. das:function:: clone(a: table<auto(KT);auto(VT)>; b: table<KT;VT> const)

clone returns auto

arguments are

+-+------------------------+
+a+table<auto(KT);auto(VT)>+
+-+------------------------+
+b+table<KT;VT> const      +
+-+------------------------+


|function-builtin-clone|

.. _function-_at__builtin__c__c_clone__hh_table_hh__hh_auto_hh__hh_auto__hh_table_hh__hh_alias_hh_KT_hh__hh_alias_hh_VT_hh_const_hh_temporary:

.. das:function:: clone(a: table<auto(KT);auto(VT)>; b: table<KT;VT> const#)

clone returns auto

arguments are

+-+------------------------+
+a+table<auto(KT);auto(VT)>+
+-+------------------------+
+b+table<KT;VT> const#     +
+-+------------------------+


|function-builtin-clone|

.. _function-_at__builtin__c__c_clone_dim__hh_auto__hh_auto_hh_const:

.. das:function:: clone_dim(a: auto; b: auto const)

clone_dim returns auto

arguments are

+-+----------+
+a+auto      +
+-+----------+
+b+auto const+
+-+----------+


|function-builtin-clone_dim|

.. _function-_at__builtin__c__c_clone_dim__hh_auto__hh_auto_hh_const_hh_temporary:

.. das:function:: clone_dim(a: auto; b: auto const#)

clone_dim returns auto

arguments are

+-+-----------+
+a+auto       +
+-+-----------+
+b+auto const#+
+-+-----------+


|function-builtin-clone_dim|

.. _function-_at__builtin__c__c_clone_to_move__hh_auto_hh_const:

.. das:function:: clone_to_move(clone_src: auto(TT) const)

clone_to_move returns TT

arguments are

+---------+--------------+
+clone_src+auto(TT) const+
+---------+--------------+


|function-builtin-clone_to_move|

++++++++++++++++++++++++++++++++
Internal finalize infrastructure
++++++++++++++++++++++++++++++++

.. warning:: 
  This group of functions is hidden. It will not be in the final documentation.

  *  :ref:`finalize (a:array\<auto(TT)\> -const) : auto <function-_at__builtin__c__c_finalize__hh_array_hh__hh_auto>` 
  *  :ref:`finalize (a:table\<auto(TK);auto(TV)\> -const) : auto <function-_at__builtin__c__c_finalize__hh_table_hh__hh_auto_hh__hh_auto>` 
  *  :ref:`finalize_dim (a:auto(TT)[-1] -const) : auto <function-_at__builtin__c__c_finalize_dim__hh_auto_hh_-1>` 

.. _function-_at__builtin__c__c_finalize__hh_array_hh__hh_auto:

.. das:function:: finalize(a: array<auto(TT)>)

finalize returns auto

arguments are

+-+---------------+
+a+array<auto(TT)>+
+-+---------------+


|function-builtin-finalize|

.. _function-_at__builtin__c__c_finalize__hh_table_hh__hh_auto_hh__hh_auto:

.. das:function:: finalize(a: table<auto(TK);auto(TV)>)

finalize returns auto

arguments are

+-+------------------------+
+a+table<auto(TK);auto(TV)>+
+-+------------------------+


|function-builtin-finalize|

.. _function-_at__builtin__c__c_finalize_dim__hh_auto_hh_-1:

.. das:function:: finalize_dim(a: auto(TT)[-1])

finalize_dim returns auto

arguments are

+-+------------+
+a+auto(TT)[-1]+
+-+------------+


|function-builtin-finalize_dim|

++++++++++
Containers
++++++++++

  *  :ref:`capacity (arg0:array const implicit) : int <function-_at__builtin__c__c_capacity__hh_array_hh_const_hh_implicit>` 
  *  :ref:`capacity (arg0:table const implicit) : int <function-_at__builtin__c__c_capacity__hh_table_hh_const_hh_implicit>` 
  *  :ref:`clear (arg0:array implicit;arg1: const) : void <function-_at__builtin__c__c_clear__hh_array_hh_implicit__hh_const>` 
  *  :ref:`clear (arg0:table implicit;arg1: const) : void <function-_at__builtin__c__c_clear__hh_table_hh_implicit__hh_const>` 
  *  :ref:`find (arg0:string const;arg1:string const) : int <function-_at__builtin__c__c_find_string_hh_const_string_hh_const>` 
  *  :ref:`find (arg0:string const;arg1:string const;arg2:int const;arg3: const) : int <function-_at__builtin__c__c_find_string_hh_const_string_hh_const_int_hh_const__hh_const>` 
  *  :ref:`length (arg0:array const implicit) : int <function-_at__builtin__c__c_length__hh_array_hh_const_hh_implicit>` 
  *  :ref:`length (arg0:$::das_string implicit) : int <function-_at__builtin__c__c_length__hh_handle_hh_das_string_hh_implicit>` 
  *  :ref:`length (arg0:table const implicit) : int <function-_at__builtin__c__c_length__hh_table_hh_const_hh_implicit>` 
  *  :ref:`length (arg0:string const;arg1: const) : int <function-_at__builtin__c__c_length_string_hh_const__hh_const>` 
  *  :ref:`resize (arg0:$::das_string implicit;arg1:int const) : void <function-_at__builtin__c__c_resize__hh_handle_hh_das_string_hh_implicit_int_hh_const>` 
  *  :ref:`each (a:array\<auto(TT)\> const) : iterator\<TT&\> <function-_at__builtin__c__c_each__hh_array_hh__hh_auto_hh_const>` 
  *  :ref:`each (a:auto(TT) const[-1]) : iterator\<TT&\> <function-_at__builtin__c__c_each__hh_auto_hh_const_hh_-1>` 
  *  :ref:`each (lam:lambda\<(arg:auto(argT) -const):bool\> const) : iterator\<argT -&\> <function-_at__builtin__c__c_each__hh_lambda_hh__hh_auto_hh__rq_arg_hh__c_bool_hh_const>` 
  *  :ref:`each (rng:range const) : iterator\<int\> <function-_at__builtin__c__c_each_range_hh_const>` 
  *  :ref:`each (str:string const) : iterator\<int\> <function-_at__builtin__c__c_each_string_hh_const>` 
  *  :ref:`each_enum (tt:auto(TT) const) : iterator\<TT -const -&\> <function-_at__builtin__c__c_each_enum__hh_auto_hh_const>` 
  *  :ref:`each_ref (lam:lambda\<(arg:auto(argT)? -const):bool\> const) : iterator\<argT&\> <function-_at__builtin__c__c_each_ref__hh_lambda_hh__hh_ptr_hh__hh_auto_hh__rq_arg_hh__c_bool_hh_const>` 
  *  :ref:`emplace (Arr:array\<auto(numT)\> -const;value:numT -const;at:int const) : auto <function-_at__builtin__c__c_emplace__hh_array_hh__hh_auto__hh_alias_hh_numT_int_hh_const>` 
  *  :ref:`erase (Arr:array\<auto(numT)\> -const;at:int const) : auto <function-_at__builtin__c__c_erase__hh_array_hh__hh_auto_int_hh_const>` 
  *  :ref:`erase (Tab:table\<auto(keyT);auto(valT)\> -const;at:keyT const) : bool <function-_at__builtin__c__c_erase__hh_table_hh__hh_auto_hh__hh_auto__hh_alias_hh_keyT_hh_const>` 
  *  :ref:`find (Tab:table\<auto(keyT);auto(valT)\> const;at:keyT const) : valT const? <function-_at__builtin__c__c_find__hh_table_hh__hh_auto_hh__hh_auto_hh_const__hh_alias_hh_keyT_hh_const>` 
  *  :ref:`find (Tab:table\<auto(keyT);auto(valT)\> const;at:keyT const;blk:block\<(p:valT? const):void\> const) : auto <function-_at__builtin__c__c_find__hh_table_hh__hh_auto_hh__hh_auto_hh_const__hh_alias_hh_keyT_hh_const__hh_block_hh__hh_ptr_hh__hh_alias_hh_valT_hh_const_hh__rq_p_hh__c_void_hh_const>` 
  *  :ref:`find (Tab:table\<auto(keyT);auto(valT)\> const#;at:keyT const) : valT const?# <function-_at__builtin__c__c_find__hh_table_hh__hh_auto_hh__hh_auto_hh_const_hh_temporary__hh_alias_hh_keyT_hh_const>` 
  *  :ref:`find_for_edit (Tab:table\<auto(keyT);auto(valT)\> -const;at:keyT const) : valT? <function-_at__builtin__c__c_find_for_edit__hh_table_hh__hh_auto_hh__hh_auto__hh_alias_hh_keyT_hh_const>` 
  *  :ref:`find_for_edit (Tab:table\<auto(keyT);auto(valT)\> -const;at:keyT const;blk:block\<(p:valT? -const):void\> const) : auto <function-_at__builtin__c__c_find_for_edit__hh_table_hh__hh_auto_hh__hh_auto__hh_alias_hh_keyT_hh_const__hh_block_hh__hh_ptr_hh__hh_alias_hh_valT_hh__rq_p_hh__c_void_hh_const>` 
  *  :ref:`find_for_edit_if_exists (Tab:table\<auto(keyT);auto(valT)\> -const;at:keyT const;blk:block\<(p:valT? -const):void\> const) : auto <function-_at__builtin__c__c_find_for_edit_if_exists__hh_table_hh__hh_auto_hh__hh_auto__hh_alias_hh_keyT_hh_const__hh_block_hh__hh_ptr_hh__hh_alias_hh_valT_hh__rq_p_hh__c_void_hh_const>` 
  *  :ref:`find_if_exists (Tab:table\<auto(keyT);auto(valT)\> const;at:keyT const;blk:block\<(p:valT? const):void\> const) : auto <function-_at__builtin__c__c_find_if_exists__hh_table_hh__hh_auto_hh__hh_auto_hh_const__hh_alias_hh_keyT_hh_const__hh_block_hh__hh_ptr_hh__hh_alias_hh_valT_hh_const_hh__rq_p_hh__c_void_hh_const>` 
  *  :ref:`find_index (arr:array\<auto(TT)\> const implicit;key:TT const) : auto <function-_at__builtin__c__c_find_index__hh_array_hh__hh_auto_hh_const_hh_implicit__hh_alias_hh_TT_hh_const>` 
  *  :ref:`find_index (arr:auto(TT) const[-1] implicit;key:TT const) : auto <function-_at__builtin__c__c_find_index__hh_auto_hh_const_hh_implicit_hh_-1__hh_alias_hh_TT_hh_const>` 
  *  :ref:`find_index_if (arr:array\<auto(TT)\> const implicit;blk:block\<(key:TT const):bool\> const) : auto <function-_at__builtin__c__c_find_index_if__hh_array_hh__hh_auto_hh_const_hh_implicit__hh_block_hh__hh_alias_hh_TT_hh_const_hh__rq_key_hh__c_bool_hh_const>` 
  *  :ref:`find_index_if (arr:auto(TT) const[-1] implicit;blk:block\<(key:TT const):bool\> const) : auto <function-_at__builtin__c__c_find_index_if__hh_auto_hh_const_hh_implicit_hh_-1__hh_block_hh__hh_alias_hh_TT_hh_const_hh__rq_key_hh__c_bool_hh_const>` 
  *  :ref:`has_value (a:auto const;key:auto const) : auto <function-_at__builtin__c__c_has_value__hh_auto_hh_const__hh_auto_hh_const>` 
  *  :ref:`key_exists (Tab:table\<auto(keyT);auto(valT)\> const;at:keyT const) : bool <function-_at__builtin__c__c_key_exists__hh_table_hh__hh_auto_hh__hh_auto_hh_const__hh_alias_hh_keyT_hh_const>` 
  *  :ref:`keys (a:table\<auto(keyT);auto(valT)\> const) : iterator\<keyT const&\> <function-_at__builtin__c__c_keys__hh_table_hh__hh_auto_hh__hh_auto_hh_const>` 
  *  :ref:`length (a:auto const[-1]) : int <function-_at__builtin__c__c_length__hh_auto_hh_const_hh_-1>` 
  *  :ref:`lock (Tab:table\<auto(keyT);auto(valT)\> const;blk:block\<(t:table\<keyT;valT\> const#):void\> const) : auto <function-_at__builtin__c__c_lock__hh_table_hh__hh_auto_hh__hh_auto_hh_const__hh_block_hh__hh_table_hh__hh_alias_hh_keyT_hh__hh_alias_hh_valT_hh_const_hh_temporary_hh__rq_t_hh__c_void_hh_const>` 
  *  :ref:`lock (Tab:table\<auto(keyT);auto(valT)\> const#;blk:block\<(t:table\<keyT;valT\> const#):void\> const) : auto <function-_at__builtin__c__c_lock__hh_table_hh__hh_auto_hh__hh_auto_hh_const_hh_temporary__hh_block_hh__hh_table_hh__hh_alias_hh_keyT_hh__hh_alias_hh_valT_hh_const_hh_temporary_hh__rq_t_hh__c_void_hh_const>` 
  *  :ref:`lock_forever (Tab:table\<auto(keyT);auto(valT)\> -const) : table\<keyT;valT\># <function-_at__builtin__c__c_lock_forever__hh_table_hh__hh_auto_hh__hh_auto>` 
  *  :ref:`next (it:iterator\<auto(TT)\> const;value:TT& -const) : bool <function-_at__builtin__c__c_next__hh_iterator_hh__hh_auto_hh_const__hh_alias_hh_TT_hh_ref>` 
  *  :ref:`nothing (it:iterator\<auto(TT)\> -const) : iterator\<TT\> <function-_at__builtin__c__c_nothing__hh_iterator_hh__hh_auto>` 
  *  :ref:`pop (Arr:array\<auto(numT)\> -const) : auto <function-_at__builtin__c__c_pop__hh_array_hh__hh_auto>` 
  *  :ref:`push (Arr:array\<auto(numT)\> -const;value:numT const;at:int const) : auto <function-_at__builtin__c__c_push__hh_array_hh__hh_auto__hh_alias_hh_numT_hh_const_int_hh_const>` 
  *  :ref:`push_clone (Arr:array\<auto(numT)\> -const;value:numT const;at:int const) : auto <function-_at__builtin__c__c_push_clone__hh_array_hh__hh_auto__hh_alias_hh_numT_hh_const_int_hh_const>` 
  *  :ref:`reserve (Arr:array\<auto(numT)\> -const;newSize:int const) : auto <function-_at__builtin__c__c_reserve__hh_array_hh__hh_auto_int_hh_const>` 
  *  :ref:`resize (Arr:array\<auto(numT)\> -const;newSize:int const) : auto <function-_at__builtin__c__c_resize__hh_array_hh__hh_auto_int_hh_const>` 
  *  :ref:`sort (a:array\<auto(TT)\> -const) : auto <function-_at__builtin__c__c_sort__hh_array_hh__hh_auto>` 
  *  :ref:`sort (a:array\<auto(TT)\> -const;cmp:block\<(x:TT const;y:TT const):bool\> const) : auto <function-_at__builtin__c__c_sort__hh_array_hh__hh_auto__hh_block_hh__hh_alias_hh_TT_hh_const_hh__hh_alias_hh_TT_hh_const_hh__rq_x_hh__rq_y_hh__c_bool_hh_const>` 
  *  :ref:`sort (a:auto(TT)[-1] -const) : auto <function-_at__builtin__c__c_sort__hh_auto_hh_-1>` 
  *  :ref:`sort (a:auto(TT)[-1] -const;cmp:block\<(x:TT const;y:TT const):bool\> const) : auto <function-_at__builtin__c__c_sort__hh_auto_hh_-1__hh_block_hh__hh_alias_hh_TT_hh_const_hh__hh_alias_hh_TT_hh_const_hh__rq_x_hh__rq_y_hh__c_bool_hh_const>` 
  *  :ref:`to_array (a:auto(TT) const[-1]) : array\<TT -const\> <function-_at__builtin__c__c_to_array__hh_auto_hh_const_hh_-1>` 
  *  :ref:`to_array (it:iterator\<auto(TT)\> const) : array\<TT -const -&\> <function-_at__builtin__c__c_to_array__hh_iterator_hh__hh_auto_hh_const>` 
  *  :ref:`to_array_move (a:auto(TT)[-1] -const) : array\<TT -const\> <function-_at__builtin__c__c_to_array_move__hh_auto_hh_-1>` 
  *  :ref:`to_table (a:tuple\<auto(keyT);auto(valT)\> const[-1]) : table\<keyT -const;valT\> <function-_at__builtin__c__c_to_table__hh_tuple_hh__hh_auto_hh__hh_auto_hh_const_hh_-1>` 
  *  :ref:`to_table_move (a:tuple\<auto(keyT);auto(valT)\>[-1] -const) : table\<keyT -const;valT\> <function-_at__builtin__c__c_to_table_move__hh_tuple_hh__hh_auto_hh__hh_auto_hh_-1>` 
  *  :ref:`values (a:table\<auto(keyT);auto(valT)\> const!) : iterator\<valT const&\> <function-_at__builtin__c__c_values__hh_table_hh__hh_auto_hh__hh_auto_hh_const_hh_explicitconst>` 
  *  :ref:`values (a:table\<auto(keyT);auto(valT)\>! -const) : iterator\<valT&\> <function-_at__builtin__c__c_values__hh_table_hh__hh_auto_hh__hh_auto_hh_explicitconst>` 

.. _function-_at__builtin__c__c_capacity__hh_array_hh_const_hh_implicit:

.. das:function:: capacity(arg0: array const implicit)

capacity returns int

arguments are

+----+--------------------+
+arg0+array const implicit+
+----+--------------------+


|function-builtin-capacity|

.. _function-_at__builtin__c__c_capacity__hh_table_hh_const_hh_implicit:

.. das:function:: capacity(arg0: table const implicit)

capacity returns int

arguments are

+----+--------------------+
+arg0+table const implicit+
+----+--------------------+


|function-builtin-capacity|

.. _function-_at__builtin__c__c_clear__hh_array_hh_implicit__hh_const:

.. das:function:: clear(arg0: array implicit)

arguments are

+----+--------------+
+arg0+array implicit+
+----+--------------+


|function-builtin-clear|

.. _function-_at__builtin__c__c_clear__hh_table_hh_implicit__hh_const:

.. das:function:: clear(arg0: table implicit)

arguments are

+----+--------------+
+arg0+table implicit+
+----+--------------+


|function-builtin-clear|

.. _function-_at__builtin__c__c_find_string_hh_const_string_hh_const:

.. das:function:: find(arg0: string const; arg1: string const)

find returns int

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+string const+
+----+------------+


|function-builtin-find|

.. _function-_at__builtin__c__c_find_string_hh_const_string_hh_const_int_hh_const__hh_const:

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


|function-builtin-find|

.. _function-_at__builtin__c__c_length__hh_array_hh_const_hh_implicit:

.. das:function:: length(arg0: array const implicit)

length returns int

arguments are

+----+--------------------+
+arg0+array const implicit+
+----+--------------------+


|function-builtin-length|

.. _function-_at__builtin__c__c_length__hh_handle_hh_das_string_hh_implicit:

.. das:function:: length(arg0: das_string implicit)

length returns int

arguments are

+----+-----------------------------------------------------------------+
+arg0+ :ref:`builtin::das_string <handle-builtin-das_string>`  implicit+
+----+-----------------------------------------------------------------+


|function-builtin-length|

.. _function-_at__builtin__c__c_length__hh_table_hh_const_hh_implicit:

.. das:function:: length(arg0: table const implicit)

length returns int

arguments are

+----+--------------------+
+arg0+table const implicit+
+----+--------------------+


|function-builtin-length|

.. _function-_at__builtin__c__c_length_string_hh_const__hh_const:

.. das:function:: length(arg0: string const)

length returns int

arguments are

+----+------------+
+arg0+string const+
+----+------------+


|function-builtin-length|

.. _function-_at__builtin__c__c_resize__hh_handle_hh_das_string_hh_implicit_int_hh_const:

.. das:function:: resize(arg0: das_string implicit; arg1: int const)

arguments are

+----+-----------------------------------------------------------------+
+arg0+ :ref:`builtin::das_string <handle-builtin-das_string>`  implicit+
+----+-----------------------------------------------------------------+
+arg1+int const                                                        +
+----+-----------------------------------------------------------------+


|function-builtin-resize|

.. _function-_at__builtin__c__c_each__hh_array_hh__hh_auto_hh_const:

.. das:function:: each(a: array<auto(TT)> const)

each returns iterator<TT&>

.. warning:: 
  This is unsafe operation.

arguments are

+-+---------------------+
+a+array<auto(TT)> const+
+-+---------------------+


|function-builtin-each|

.. _function-_at__builtin__c__c_each__hh_auto_hh_const_hh_-1:

.. das:function:: each(a: auto(TT) const[-1])

each returns iterator<TT&>

.. warning:: 
  This is unsafe operation.

arguments are

+-+------------------+
+a+auto(TT) const[-1]+
+-+------------------+


|function-builtin-each|

.. _function-_at__builtin__c__c_each__hh_lambda_hh__hh_auto_hh__rq_arg_hh__c_bool_hh_const:

.. das:function:: each(lam: lambda<(arg:auto(argT) -const):bool> const)

each returns iterator<argT>

arguments are

+---+-----------------------------------+
+lam+lambda<(arg:auto(argT)):bool> const+
+---+-----------------------------------+


|function-builtin-each|

.. _function-_at__builtin__c__c_each_range_hh_const:

.. das:function:: each(rng: range const)

each returns iterator<int>

arguments are

+---+-----------+
+rng+range const+
+---+-----------+


|function-builtin-each|

.. _function-_at__builtin__c__c_each_string_hh_const:

.. das:function:: each(str: string const)

each returns iterator<int>

.. warning:: 
  This is unsafe operation.

arguments are

+---+------------+
+str+string const+
+---+------------+


|function-builtin-each|

.. _function-_at__builtin__c__c_each_enum__hh_auto_hh_const:

.. das:function:: each_enum(tt: auto(TT) const)

each_enum returns iterator<TT>

arguments are

+--+--------------+
+tt+auto(TT) const+
+--+--------------+


|function-builtin-each_enum|

.. _function-_at__builtin__c__c_each_ref__hh_lambda_hh__hh_ptr_hh__hh_auto_hh__rq_arg_hh__c_bool_hh_const:

.. das:function:: each_ref(lam: lambda<(arg:auto(argT)? -const):bool> const)

each_ref returns iterator<argT&>

arguments are

+---+------------------------------------+
+lam+lambda<(arg:auto(argT)?):bool> const+
+---+------------------------------------+


|function-builtin-each_ref|

.. _function-_at__builtin__c__c_emplace__hh_array_hh__hh_auto__hh_alias_hh_numT_int_hh_const:

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


|function-builtin-emplace|

.. _function-_at__builtin__c__c_erase__hh_array_hh__hh_auto_int_hh_const:

.. das:function:: erase(Arr: array<auto(numT)>; at: int const)

erase returns auto

arguments are

+---+-----------------+
+Arr+array<auto(numT)>+
+---+-----------------+
+at +int const        +
+---+-----------------+


|function-builtin-erase|

.. _function-_at__builtin__c__c_erase__hh_table_hh__hh_auto_hh__hh_auto__hh_alias_hh_keyT_hh_const:

.. das:function:: erase(Tab: table<auto(keyT);auto(valT)>; at: keyT const)

erase returns bool

arguments are

+---+----------------------------+
+Tab+table<auto(keyT);auto(valT)>+
+---+----------------------------+
+at +keyT const                  +
+---+----------------------------+


|function-builtin-erase|

.. _function-_at__builtin__c__c_find__hh_table_hh__hh_auto_hh__hh_auto_hh_const__hh_alias_hh_keyT_hh_const:

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


|function-builtin-find|

.. _function-_at__builtin__c__c_find__hh_table_hh__hh_auto_hh__hh_auto_hh_const__hh_alias_hh_keyT_hh_const__hh_block_hh__hh_ptr_hh__hh_alias_hh_valT_hh_const_hh__rq_p_hh__c_void_hh_const:

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


|function-builtin-find|

.. _function-_at__builtin__c__c_find__hh_table_hh__hh_auto_hh__hh_auto_hh_const_hh_temporary__hh_alias_hh_keyT_hh_const:

.. das:function:: find(Tab: table<auto(keyT);auto(valT)> const#; at: keyT const)

find returns valT const?#

arguments are

+---+-----------------------------------+
+Tab+table<auto(keyT);auto(valT)> const#+
+---+-----------------------------------+
+at +keyT const                         +
+---+-----------------------------------+


|function-builtin-find|

.. _function-_at__builtin__c__c_find_for_edit__hh_table_hh__hh_auto_hh__hh_auto__hh_alias_hh_keyT_hh_const:

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


|function-builtin-find_for_edit|

.. _function-_at__builtin__c__c_find_for_edit__hh_table_hh__hh_auto_hh__hh_auto__hh_alias_hh_keyT_hh_const__hh_block_hh__hh_ptr_hh__hh_alias_hh_valT_hh__rq_p_hh__c_void_hh_const:

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


|function-builtin-find_for_edit|

.. _function-_at__builtin__c__c_find_for_edit_if_exists__hh_table_hh__hh_auto_hh__hh_auto__hh_alias_hh_keyT_hh_const__hh_block_hh__hh_ptr_hh__hh_alias_hh_valT_hh__rq_p_hh__c_void_hh_const:

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


|function-builtin-find_for_edit_if_exists|

.. _function-_at__builtin__c__c_find_if_exists__hh_table_hh__hh_auto_hh__hh_auto_hh_const__hh_alias_hh_keyT_hh_const__hh_block_hh__hh_ptr_hh__hh_alias_hh_valT_hh_const_hh__rq_p_hh__c_void_hh_const:

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


|function-builtin-find_if_exists|

.. _function-_at__builtin__c__c_find_index__hh_array_hh__hh_auto_hh_const_hh_implicit__hh_alias_hh_TT_hh_const:

.. das:function:: find_index(arr: array<auto(TT)> const implicit; key: TT const)

find_index returns auto

arguments are

+---+------------------------------+
+arr+array<auto(TT)> const implicit+
+---+------------------------------+
+key+TT const                      +
+---+------------------------------+


|function-builtin-find_index|

.. _function-_at__builtin__c__c_find_index__hh_auto_hh_const_hh_implicit_hh_-1__hh_alias_hh_TT_hh_const:

.. das:function:: find_index(arr: auto(TT) const[-1] implicit; key: TT const)

find_index returns auto

arguments are

+---+---------------------------+
+arr+auto(TT) const[-1] implicit+
+---+---------------------------+
+key+TT const                   +
+---+---------------------------+


|function-builtin-find_index|

.. _function-_at__builtin__c__c_find_index_if__hh_array_hh__hh_auto_hh_const_hh_implicit__hh_block_hh__hh_alias_hh_TT_hh_const_hh__rq_key_hh__c_bool_hh_const:

.. das:function:: find_index_if(arr: array<auto(TT)> const implicit; blk: block<(key:TT const):bool> const)

find_index_if returns auto

arguments are

+---+--------------------------------+
+arr+array<auto(TT)> const implicit  +
+---+--------------------------------+
+blk+block<(key:TT const):bool> const+
+---+--------------------------------+


|function-builtin-find_index_if|

.. _function-_at__builtin__c__c_find_index_if__hh_auto_hh_const_hh_implicit_hh_-1__hh_block_hh__hh_alias_hh_TT_hh_const_hh__rq_key_hh__c_bool_hh_const:

.. das:function:: find_index_if(arr: auto(TT) const[-1] implicit; blk: block<(key:TT const):bool> const)

find_index_if returns auto

arguments are

+---+--------------------------------+
+arr+auto(TT) const[-1] implicit     +
+---+--------------------------------+
+blk+block<(key:TT const):bool> const+
+---+--------------------------------+


|function-builtin-find_index_if|

.. _function-_at__builtin__c__c_has_value__hh_auto_hh_const__hh_auto_hh_const:

.. das:function:: has_value(a: auto const; key: auto const)

has_value returns auto

arguments are

+---+----------+
+a  +auto const+
+---+----------+
+key+auto const+
+---+----------+


|function-builtin-has_value|

.. _function-_at__builtin__c__c_key_exists__hh_table_hh__hh_auto_hh__hh_auto_hh_const__hh_alias_hh_keyT_hh_const:

.. das:function:: key_exists(Tab: table<auto(keyT);auto(valT)> const; at: keyT const)

key_exists returns bool

arguments are

+---+----------------------------------+
+Tab+table<auto(keyT);auto(valT)> const+
+---+----------------------------------+
+at +keyT const                        +
+---+----------------------------------+


|function-builtin-key_exists|

.. _function-_at__builtin__c__c_keys__hh_table_hh__hh_auto_hh__hh_auto_hh_const:

.. das:function:: keys(a: table<auto(keyT);auto(valT)> const)

keys returns iterator<keyT const&>

arguments are

+-+----------------------------------+
+a+table<auto(keyT);auto(valT)> const+
+-+----------------------------------+


|function-builtin-keys|

.. _function-_at__builtin__c__c_length__hh_auto_hh_const_hh_-1:

.. das:function:: length(a: auto const[-1])

length returns int

arguments are

+-+--------------+
+a+auto const[-1]+
+-+--------------+


|function-builtin-length|

.. _function-_at__builtin__c__c_lock__hh_table_hh__hh_auto_hh__hh_auto_hh_const__hh_block_hh__hh_table_hh__hh_alias_hh_keyT_hh__hh_alias_hh_valT_hh_const_hh_temporary_hh__rq_t_hh__c_void_hh_const:

.. das:function:: lock(Tab: table<auto(keyT);auto(valT)> const; blk: block<(t:table<keyT;valT> const#):void> const)

lock returns auto

arguments are

+---+---------------------------------------------+
+Tab+table<auto(keyT);auto(valT)> const           +
+---+---------------------------------------------+
+blk+block<(t:table<keyT;valT> const#):void> const+
+---+---------------------------------------------+


|function-builtin-lock|

.. _function-_at__builtin__c__c_lock__hh_table_hh__hh_auto_hh__hh_auto_hh_const_hh_temporary__hh_block_hh__hh_table_hh__hh_alias_hh_keyT_hh__hh_alias_hh_valT_hh_const_hh_temporary_hh__rq_t_hh__c_void_hh_const:

.. das:function:: lock(Tab: table<auto(keyT);auto(valT)> const#; blk: block<(t:table<keyT;valT> const#):void> const)

lock returns auto

arguments are

+---+---------------------------------------------+
+Tab+table<auto(keyT);auto(valT)> const#          +
+---+---------------------------------------------+
+blk+block<(t:table<keyT;valT> const#):void> const+
+---+---------------------------------------------+


|function-builtin-lock|

.. _function-_at__builtin__c__c_lock_forever__hh_table_hh__hh_auto_hh__hh_auto:

.. das:function:: lock_forever(Tab: table<auto(keyT);auto(valT)>)

lock_forever returns table<keyT;valT>#

arguments are

+---+----------------------------+
+Tab+table<auto(keyT);auto(valT)>+
+---+----------------------------+


|function-builtin-lock_forever|

.. _function-_at__builtin__c__c_next__hh_iterator_hh__hh_auto_hh_const__hh_alias_hh_TT_hh_ref:

.. das:function:: next(it: iterator<auto(TT)> const; value: TT&)

next returns bool

arguments are

+-----+------------------------+
+it   +iterator<auto(TT)> const+
+-----+------------------------+
+value+TT&                     +
+-----+------------------------+


|function-builtin-next|

.. _function-_at__builtin__c__c_nothing__hh_iterator_hh__hh_auto:

.. das:function:: nothing(it: iterator<auto(TT)>)

nothing returns iterator<TT>

arguments are

+--+------------------+
+it+iterator<auto(TT)>+
+--+------------------+


|function-builtin-nothing|

.. _function-_at__builtin__c__c_pop__hh_array_hh__hh_auto:

.. das:function:: pop(Arr: array<auto(numT)>)

pop returns auto

arguments are

+---+-----------------+
+Arr+array<auto(numT)>+
+---+-----------------+


|function-builtin-pop|

.. _function-_at__builtin__c__c_push__hh_array_hh__hh_auto__hh_alias_hh_numT_hh_const_int_hh_const:

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


|function-builtin-push|

.. _function-_at__builtin__c__c_push_clone__hh_array_hh__hh_auto__hh_alias_hh_numT_hh_const_int_hh_const:

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


|function-builtin-push_clone|

.. _function-_at__builtin__c__c_reserve__hh_array_hh__hh_auto_int_hh_const:

.. das:function:: reserve(Arr: array<auto(numT)>; newSize: int const)

reserve returns auto

arguments are

+-------+-----------------+
+Arr    +array<auto(numT)>+
+-------+-----------------+
+newSize+int const        +
+-------+-----------------+


|function-builtin-reserve|

.. _function-_at__builtin__c__c_resize__hh_array_hh__hh_auto_int_hh_const:

.. das:function:: resize(Arr: array<auto(numT)>; newSize: int const)

resize returns auto

arguments are

+-------+-----------------+
+Arr    +array<auto(numT)>+
+-------+-----------------+
+newSize+int const        +
+-------+-----------------+


|function-builtin-resize|

.. _function-_at__builtin__c__c_sort__hh_array_hh__hh_auto:

.. das:function:: sort(a: array<auto(TT)>)

sort returns auto

arguments are

+-+---------------+
+a+array<auto(TT)>+
+-+---------------+


|function-builtin-sort|

.. _function-_at__builtin__c__c_sort__hh_array_hh__hh_auto__hh_block_hh__hh_alias_hh_TT_hh_const_hh__hh_alias_hh_TT_hh_const_hh__rq_x_hh__rq_y_hh__c_bool_hh_const:

.. das:function:: sort(a: array<auto(TT)>; cmp: block<(x:TT const;y:TT const):bool> const)

sort returns auto

arguments are

+---+-----------------------------------------+
+a  +array<auto(TT)>                          +
+---+-----------------------------------------+
+cmp+block<(x:TT const;y:TT const):bool> const+
+---+-----------------------------------------+


|function-builtin-sort|

.. _function-_at__builtin__c__c_sort__hh_auto_hh_-1:

.. das:function:: sort(a: auto(TT)[-1])

sort returns auto

arguments are

+-+------------+
+a+auto(TT)[-1]+
+-+------------+


|function-builtin-sort|

.. _function-_at__builtin__c__c_sort__hh_auto_hh_-1__hh_block_hh__hh_alias_hh_TT_hh_const_hh__hh_alias_hh_TT_hh_const_hh__rq_x_hh__rq_y_hh__c_bool_hh_const:

.. das:function:: sort(a: auto(TT)[-1]; cmp: block<(x:TT const;y:TT const):bool> const)

sort returns auto

arguments are

+---+-----------------------------------------+
+a  +auto(TT)[-1]                             +
+---+-----------------------------------------+
+cmp+block<(x:TT const;y:TT const):bool> const+
+---+-----------------------------------------+


|function-builtin-sort|

.. _function-_at__builtin__c__c_to_array__hh_auto_hh_const_hh_-1:

.. das:function:: to_array(a: auto(TT) const[-1])

to_array returns array<TT>

arguments are

+-+------------------+
+a+auto(TT) const[-1]+
+-+------------------+


|function-builtin-to_array|

.. _function-_at__builtin__c__c_to_array__hh_iterator_hh__hh_auto_hh_const:

.. das:function:: to_array(it: iterator<auto(TT)> const)

to_array returns array<TT>

arguments are

+--+------------------------+
+it+iterator<auto(TT)> const+
+--+------------------------+


|function-builtin-to_array|

.. _function-_at__builtin__c__c_to_array_move__hh_auto_hh_-1:

.. das:function:: to_array_move(a: auto(TT)[-1])

to_array_move returns array<TT>

arguments are

+-+------------+
+a+auto(TT)[-1]+
+-+------------+


|function-builtin-to_array_move|

.. _function-_at__builtin__c__c_to_table__hh_tuple_hh__hh_auto_hh__hh_auto_hh_const_hh_-1:

.. das:function:: to_table(a: tuple<auto(keyT);auto(valT)> const[-1])

to_table returns table<keyT;valT>

arguments are

+-+--------------------------------------+
+a+tuple<auto(keyT);auto(valT)> const[-1]+
+-+--------------------------------------+


|function-builtin-to_table|

.. _function-_at__builtin__c__c_to_table_move__hh_tuple_hh__hh_auto_hh__hh_auto_hh_-1:

.. das:function:: to_table_move(a: tuple<auto(keyT);auto(valT)>[-1])

to_table_move returns table<keyT;valT>

arguments are

+-+--------------------------------+
+a+tuple<auto(keyT);auto(valT)>[-1]+
+-+--------------------------------+


|function-builtin-to_table_move|

.. _function-_at__builtin__c__c_values__hh_table_hh__hh_auto_hh__hh_auto_hh_const_hh_explicitconst:

.. das:function:: values(a: table<auto(keyT);auto(valT)> const!)

values returns iterator<valT const&>

arguments are

+-+-----------------------------------+
+a+table<auto(keyT);auto(valT)> const!+
+-+-----------------------------------+


|function-builtin-values|

.. _function-_at__builtin__c__c_values__hh_table_hh__hh_auto_hh__hh_auto_hh_explicitconst:

.. das:function:: values(a: table<auto(keyT);auto(valT)>!)

values returns iterator<valT&>

arguments are

+-+-----------------------------+
+a+table<auto(keyT);auto(valT)>!+
+-+-----------------------------+


|function-builtin-values|

++++++++++++++++++++
Character set groups
++++++++++++++++++++

  *  :ref:`is_alpha (arg0:int const) : bool <function-_at__builtin__c__c_is_alpha_int_hh_const>` 
  *  :ref:`is_char_in_set (arg0:int const;arg1:uint const[8] implicit) : bool <function-_at__builtin__c__c_is_char_in_set_int_hh_const_uint_hh_const_hh_implicit_hh_8>` 
  *  :ref:`is_number (arg0:int const) : bool <function-_at__builtin__c__c_is_number_int_hh_const>` 
  *  :ref:`is_white_space (arg0:int const) : bool <function-_at__builtin__c__c_is_white_space_int_hh_const>` 

.. _function-_at__builtin__c__c_is_alpha_int_hh_const:

.. das:function:: is_alpha(arg0: int const)

is_alpha returns bool

arguments are

+----+---------+
+arg0+int const+
+----+---------+


|function-builtin-is_alpha|

.. _function-_at__builtin__c__c_is_char_in_set_int_hh_const_uint_hh_const_hh_implicit_hh_8:

.. das:function:: is_char_in_set(arg0: int const; arg1: uint const[8] implicit)

is_char_in_set returns bool

arguments are

+----+----------------------+
+arg0+int const             +
+----+----------------------+
+arg1+uint const[8] implicit+
+----+----------------------+


|function-builtin-is_char_in_set|

.. _function-_at__builtin__c__c_is_number_int_hh_const:

.. das:function:: is_number(arg0: int const)

is_number returns bool

arguments are

+----+---------+
+arg0+int const+
+----+---------+


|function-builtin-is_number|

.. _function-_at__builtin__c__c_is_white_space_int_hh_const:

.. das:function:: is_white_space(arg0: int const)

is_white_space returns bool

arguments are

+----+---------+
+arg0+int const+
+----+---------+


|function-builtin-is_white_space|

+++++++++++++++++++
String manipulation
+++++++++++++++++++

  *  :ref:`append (arg0:$::das_string implicit;arg1:int const) : void <function-_at__builtin__c__c_append__hh_handle_hh_das_string_hh_implicit_int_hh_const>` 
  *  :ref:`append (arg0:string const;arg1:int const;arg2: const) : string <function-_at__builtin__c__c_append_string_hh_const_int_hh_const__hh_const>` 
  *  :ref:`as_string (arg0:array\<uint8\> const implicit;arg1: const) : string <function-_at__builtin__c__c_as_string__hh_array_hh_uint8_hh_const_hh_implicit__hh_const>` 
  *  :ref:`build_string (arg0:block\<($::StringBuilderWriter):void\> const implicit;arg1: const) : string <function-_at__builtin__c__c_build_string__hh_block_hh__hh_handle_hh_StringBuilderWriter_hh__c_void_hh_const_hh_implicit__hh_const>` 
  *  :ref:`character_at (arg0:string const;arg1:int const;arg2: const) : int <function-_at__builtin__c__c_character_at_string_hh_const_int_hh_const__hh_const>` 
  *  :ref:`character_uat (arg0:string const;arg1:int const) : int <function-_at__builtin__c__c_character_uat_string_hh_const_int_hh_const>` 
  *  :ref:`chop (arg0:string const;arg1:int const;arg2:int const;arg3: const) : string <function-_at__builtin__c__c_chop_string_hh_const_int_hh_const_int_hh_const__hh_const>` 
  *  :ref:`delete_string (arg0:string& implicit;arg1: const) : void <function-_at__builtin__c__c_delete_string_string_hh_ref_hh_implicit__hh_const>` 
  *  :ref:`empty (arg0:$::das_string const implicit) : bool <function-_at__builtin__c__c_empty__hh_handle_hh_das_string_hh_const_hh_implicit>` 
  *  :ref:`empty (arg0:iterator const implicit) : bool <function-_at__builtin__c__c_empty__hh_iterator_hh_const_hh_implicit>` 
  *  :ref:`empty (arg0:string const) : bool <function-_at__builtin__c__c_empty_string_hh_const>` 
  *  :ref:`ends_with (arg0:$::das_string const implicit;arg1:string const;arg2: const) : bool <function-_at__builtin__c__c_ends_with__hh_handle_hh_das_string_hh_const_hh_implicit_string_hh_const__hh_const>` 
  *  :ref:`ends_with (arg0:string const;arg1:string const;arg2: const) : bool <function-_at__builtin__c__c_ends_with_string_hh_const_string_hh_const__hh_const>` 
  *  :ref:`escape (arg0:string const;arg1: const) : string <function-_at__builtin__c__c_escape_string_hh_const__hh_const>` 
  *  :ref:`find_first_of (arg0:string const;arg1:int const;arg2: const) : int <function-_at__builtin__c__c_find_first_of_string_hh_const_int_hh_const__hh_const>` 
  *  :ref:`find_first_of (arg0:string const;arg1:string const;arg2: const) : int <function-_at__builtin__c__c_find_first_of_string_hh_const_string_hh_const__hh_const>` 
  *  :ref:`format (arg0:$::StringBuilderWriter implicit;arg1:string const;arg2:double const) : void <function-_at__builtin__c__c_format__hh_handle_hh_StringBuilderWriter_hh_implicit_string_hh_const_double_hh_const>` 
  *  :ref:`format (arg0:$::StringBuilderWriter implicit;arg1:string const;arg2:float const) : void <function-_at__builtin__c__c_format__hh_handle_hh_StringBuilderWriter_hh_implicit_string_hh_const_float_hh_const>` 
  *  :ref:`format (arg0:$::StringBuilderWriter implicit;arg1:string const;arg2:int const) : void <function-_at__builtin__c__c_format__hh_handle_hh_StringBuilderWriter_hh_implicit_string_hh_const_int_hh_const>` 
  *  :ref:`format (arg0:$::StringBuilderWriter implicit;arg1:string const;arg2:int64 const) : void <function-_at__builtin__c__c_format__hh_handle_hh_StringBuilderWriter_hh_implicit_string_hh_const_int64_hh_const>` 
  *  :ref:`format (arg0:$::StringBuilderWriter implicit;arg1:string const;arg2:uint const) : void <function-_at__builtin__c__c_format__hh_handle_hh_StringBuilderWriter_hh_implicit_string_hh_const_uint_hh_const>` 
  *  :ref:`format (arg0:$::StringBuilderWriter implicit;arg1:string const;arg2:uint64 const) : void <function-_at__builtin__c__c_format__hh_handle_hh_StringBuilderWriter_hh_implicit_string_hh_const_uint64_hh_const>` 
  *  :ref:`format (arg0:string const;arg1:double const;arg2: const) : string <function-_at__builtin__c__c_format_string_hh_const_double_hh_const__hh_const>` 
  *  :ref:`format (arg0:string const;arg1:float const;arg2: const) : string <function-_at__builtin__c__c_format_string_hh_const_float_hh_const__hh_const>` 
  *  :ref:`format (arg0:string const;arg1:int const;arg2: const) : string <function-_at__builtin__c__c_format_string_hh_const_int_hh_const__hh_const>` 
  *  :ref:`format (arg0:string const;arg1:int64 const;arg2: const) : string <function-_at__builtin__c__c_format_string_hh_const_int64_hh_const__hh_const>` 
  *  :ref:`format (arg0:string const;arg1:uint const;arg2: const) : string <function-_at__builtin__c__c_format_string_hh_const_uint_hh_const__hh_const>` 
  *  :ref:`format (arg0:string const;arg1:uint64 const;arg2: const) : string <function-_at__builtin__c__c_format_string_hh_const_uint64_hh_const__hh_const>` 
  *  :ref:`peek (arg0:$::das_string const implicit;arg1:block\<(string const#):void\> const implicit;arg2: const) : void <function-_at__builtin__c__c_peek__hh_handle_hh_das_string_hh_const_hh_implicit__hh_block_hh_string_hh_const_hh_temporary_hh__c_void_hh_const_hh_implicit__hh_const>` 
  *  :ref:`repeat (arg0:string const;arg1:int const;arg2: const) : string <function-_at__builtin__c__c_repeat_string_hh_const_int_hh_const__hh_const>` 
  *  :ref:`replace (arg0:string const;arg1:string const;arg2:string const;arg3: const) : string <function-_at__builtin__c__c_replace_string_hh_const_string_hh_const_string_hh_const__hh_const>` 
  *  :ref:`reverse (arg0:string const;arg1: const) : string <function-_at__builtin__c__c_reverse_string_hh_const__hh_const>` 
  *  :ref:`set (arg0:$::das_string implicit;arg1:string const) : void <function-_at__builtin__c__c_set__hh_handle_hh_das_string_hh_implicit_string_hh_const>` 
  *  :ref:`slice (arg0:string const;arg1:int const;arg2: const) : string <function-_at__builtin__c__c_slice_string_hh_const_int_hh_const__hh_const>` 
  *  :ref:`slice (arg0:string const;arg1:int const;arg2:int const;arg3: const) : string <function-_at__builtin__c__c_slice_string_hh_const_int_hh_const_int_hh_const__hh_const>` 
  *  :ref:`starts_with (arg0:string const;arg1:string const;arg2: const) : bool <function-_at__builtin__c__c_starts_with_string_hh_const_string_hh_const__hh_const>` 
  *  :ref:`strip (arg0:string const;arg1: const) : string <function-_at__builtin__c__c_strip_string_hh_const__hh_const>` 
  *  :ref:`strip_left (arg0:string const;arg1: const) : string <function-_at__builtin__c__c_strip_left_string_hh_const__hh_const>` 
  *  :ref:`strip_right (arg0:string const;arg1: const) : string <function-_at__builtin__c__c_strip_right_string_hh_const__hh_const>` 
  *  :ref:`to_char (arg0:int const;arg1: const) : string <function-_at__builtin__c__c_to_char_int_hh_const__hh_const>` 
  *  :ref:`to_float (arg0:string const) : float <function-_at__builtin__c__c_to_float_string_hh_const>` 
  *  :ref:`to_int (arg0:string const) : int <function-_at__builtin__c__c_to_int_string_hh_const>` 
  *  :ref:`to_lower (arg0:string const;arg1: const) : string <function-_at__builtin__c__c_to_lower_string_hh_const__hh_const>` 
  *  :ref:`to_lower_in_place (arg0:string const) : string <function-_at__builtin__c__c_to_lower_in_place_string_hh_const>` 
  *  :ref:`to_upper (arg0:string const;arg1: const) : string <function-_at__builtin__c__c_to_upper_string_hh_const__hh_const>` 
  *  :ref:`to_upper_in_place (arg0:string const) : string <function-_at__builtin__c__c_to_upper_in_place_string_hh_const>` 
  *  :ref:`unescape (arg0:string const;arg1: const) : string <function-_at__builtin__c__c_unescape_string_hh_const__hh_const>` 

.. _function-_at__builtin__c__c_append__hh_handle_hh_das_string_hh_implicit_int_hh_const:

.. das:function:: append(arg0: das_string implicit; arg1: int const)

arguments are

+----+-----------------------------------------------------------------+
+arg0+ :ref:`builtin::das_string <handle-builtin-das_string>`  implicit+
+----+-----------------------------------------------------------------+
+arg1+int const                                                        +
+----+-----------------------------------------------------------------+


|function-builtin-append|

.. _function-_at__builtin__c__c_append_string_hh_const_int_hh_const__hh_const:

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


|function-builtin-append|

.. _function-_at__builtin__c__c_as_string__hh_array_hh_uint8_hh_const_hh_implicit__hh_const:

.. das:function:: as_string(arg0: array<uint8> const implicit)

as_string returns string

arguments are

+----+---------------------------+
+arg0+array<uint8> const implicit+
+----+---------------------------+


|function-builtin-as_string|

.. _function-_at__builtin__c__c_build_string__hh_block_hh__hh_handle_hh_StringBuilderWriter_hh__c_void_hh_const_hh_implicit__hh_const:

.. das:function:: build_string(arg0: block<(StringBuilderWriter):void> const implicit)

build_string returns string

arguments are

+----+-------------------------------------------------------------------------------------------------------+
+arg0+block<( :ref:`builtin::StringBuilderWriter <handle-builtin-StringBuilderWriter>` ):void> const implicit+
+----+-------------------------------------------------------------------------------------------------------+


|function-builtin-build_string|

.. _function-_at__builtin__c__c_character_at_string_hh_const_int_hh_const__hh_const:

.. das:function:: character_at(arg0: string const; arg1: int const)

character_at returns int

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+


|function-builtin-character_at|

.. _function-_at__builtin__c__c_character_uat_string_hh_const_int_hh_const:

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


|function-builtin-character_uat|

.. _function-_at__builtin__c__c_chop_string_hh_const_int_hh_const_int_hh_const__hh_const:

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


|function-builtin-chop|

.. _function-_at__builtin__c__c_delete_string_string_hh_ref_hh_implicit__hh_const:

.. das:function:: delete_string(arg0: string& implicit)

.. warning:: 
  This is unsafe operation.

arguments are

+----+----------------+
+arg0+string& implicit+
+----+----------------+


|function-builtin-delete_string|

.. _function-_at__builtin__c__c_empty__hh_handle_hh_das_string_hh_const_hh_implicit:

.. das:function:: empty(arg0: das_string const implicit)

empty returns bool

arguments are

+----+-----------------------------------------------------------------------+
+arg0+ :ref:`builtin::das_string <handle-builtin-das_string>`  const implicit+
+----+-----------------------------------------------------------------------+


|function-builtin-empty|

.. _function-_at__builtin__c__c_empty__hh_iterator_hh_const_hh_implicit:

.. das:function:: empty(arg0: iterator const implicit)

empty returns bool

arguments are

+----+-----------------------+
+arg0+iterator const implicit+
+----+-----------------------+


|function-builtin-empty|

.. _function-_at__builtin__c__c_empty_string_hh_const:

.. das:function:: empty(arg0: string const)

empty returns bool

arguments are

+----+------------+
+arg0+string const+
+----+------------+


|function-builtin-empty|

.. _function-_at__builtin__c__c_ends_with__hh_handle_hh_das_string_hh_const_hh_implicit_string_hh_const__hh_const:

.. das:function:: ends_with(arg0: das_string const implicit; arg1: string const)

ends_with returns bool

arguments are

+----+-----------------------------------------------------------------------+
+arg0+ :ref:`builtin::das_string <handle-builtin-das_string>`  const implicit+
+----+-----------------------------------------------------------------------+
+arg1+string const                                                           +
+----+-----------------------------------------------------------------------+


|function-builtin-ends_with|

.. _function-_at__builtin__c__c_ends_with_string_hh_const_string_hh_const__hh_const:

.. das:function:: ends_with(arg0: string const; arg1: string const)

ends_with returns bool

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+string const+
+----+------------+


|function-builtin-ends_with|

.. _function-_at__builtin__c__c_escape_string_hh_const__hh_const:

.. das:function:: escape(arg0: string const)

escape returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+


|function-builtin-escape|

.. _function-_at__builtin__c__c_find_first_of_string_hh_const_int_hh_const__hh_const:

.. das:function:: find_first_of(arg0: string const; arg1: int const)

find_first_of returns int

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+


|function-builtin-find_first_of|

.. _function-_at__builtin__c__c_find_first_of_string_hh_const_string_hh_const__hh_const:

.. das:function:: find_first_of(arg0: string const; arg1: string const)

find_first_of returns int

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+string const+
+----+------------+


|function-builtin-find_first_of|

.. _function-_at__builtin__c__c_format__hh_handle_hh_StringBuilderWriter_hh_implicit_string_hh_const_double_hh_const:

.. das:function:: format(arg0: StringBuilderWriter implicit; arg1: string const; arg2: double const)

arguments are

+----+-----------------------------------------------------------------------------------+
+arg0+ :ref:`builtin::StringBuilderWriter <handle-builtin-StringBuilderWriter>`  implicit+
+----+-----------------------------------------------------------------------------------+
+arg1+string const                                                                       +
+----+-----------------------------------------------------------------------------------+
+arg2+double const                                                                       +
+----+-----------------------------------------------------------------------------------+


|function-builtin-format|

.. _function-_at__builtin__c__c_format__hh_handle_hh_StringBuilderWriter_hh_implicit_string_hh_const_float_hh_const:

.. das:function:: format(arg0: StringBuilderWriter implicit; arg1: string const; arg2: float const)

arguments are

+----+-----------------------------------------------------------------------------------+
+arg0+ :ref:`builtin::StringBuilderWriter <handle-builtin-StringBuilderWriter>`  implicit+
+----+-----------------------------------------------------------------------------------+
+arg1+string const                                                                       +
+----+-----------------------------------------------------------------------------------+
+arg2+float const                                                                        +
+----+-----------------------------------------------------------------------------------+


|function-builtin-format|

.. _function-_at__builtin__c__c_format__hh_handle_hh_StringBuilderWriter_hh_implicit_string_hh_const_int_hh_const:

.. das:function:: format(arg0: StringBuilderWriter implicit; arg1: string const; arg2: int const)

arguments are

+----+-----------------------------------------------------------------------------------+
+arg0+ :ref:`builtin::StringBuilderWriter <handle-builtin-StringBuilderWriter>`  implicit+
+----+-----------------------------------------------------------------------------------+
+arg1+string const                                                                       +
+----+-----------------------------------------------------------------------------------+
+arg2+int const                                                                          +
+----+-----------------------------------------------------------------------------------+


|function-builtin-format|

.. _function-_at__builtin__c__c_format__hh_handle_hh_StringBuilderWriter_hh_implicit_string_hh_const_int64_hh_const:

.. das:function:: format(arg0: StringBuilderWriter implicit; arg1: string const; arg2: int64 const)

arguments are

+----+-----------------------------------------------------------------------------------+
+arg0+ :ref:`builtin::StringBuilderWriter <handle-builtin-StringBuilderWriter>`  implicit+
+----+-----------------------------------------------------------------------------------+
+arg1+string const                                                                       +
+----+-----------------------------------------------------------------------------------+
+arg2+int64 const                                                                        +
+----+-----------------------------------------------------------------------------------+


|function-builtin-format|

.. _function-_at__builtin__c__c_format__hh_handle_hh_StringBuilderWriter_hh_implicit_string_hh_const_uint_hh_const:

.. das:function:: format(arg0: StringBuilderWriter implicit; arg1: string const; arg2: uint const)

arguments are

+----+-----------------------------------------------------------------------------------+
+arg0+ :ref:`builtin::StringBuilderWriter <handle-builtin-StringBuilderWriter>`  implicit+
+----+-----------------------------------------------------------------------------------+
+arg1+string const                                                                       +
+----+-----------------------------------------------------------------------------------+
+arg2+uint const                                                                         +
+----+-----------------------------------------------------------------------------------+


|function-builtin-format|

.. _function-_at__builtin__c__c_format__hh_handle_hh_StringBuilderWriter_hh_implicit_string_hh_const_uint64_hh_const:

.. das:function:: format(arg0: StringBuilderWriter implicit; arg1: string const; arg2: uint64 const)

arguments are

+----+-----------------------------------------------------------------------------------+
+arg0+ :ref:`builtin::StringBuilderWriter <handle-builtin-StringBuilderWriter>`  implicit+
+----+-----------------------------------------------------------------------------------+
+arg1+string const                                                                       +
+----+-----------------------------------------------------------------------------------+
+arg2+uint64 const                                                                       +
+----+-----------------------------------------------------------------------------------+


|function-builtin-format|

.. _function-_at__builtin__c__c_format_string_hh_const_double_hh_const__hh_const:

.. das:function:: format(arg0: string const; arg1: double const)

format returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+double const+
+----+------------+


|function-builtin-format|

.. _function-_at__builtin__c__c_format_string_hh_const_float_hh_const__hh_const:

.. das:function:: format(arg0: string const; arg1: float const)

format returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+float const +
+----+------------+


|function-builtin-format|

.. _function-_at__builtin__c__c_format_string_hh_const_int_hh_const__hh_const:

.. das:function:: format(arg0: string const; arg1: int const)

format returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+


|function-builtin-format|

.. _function-_at__builtin__c__c_format_string_hh_const_int64_hh_const__hh_const:

.. das:function:: format(arg0: string const; arg1: int64 const)

format returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int64 const +
+----+------------+


|function-builtin-format|

.. _function-_at__builtin__c__c_format_string_hh_const_uint_hh_const__hh_const:

.. das:function:: format(arg0: string const; arg1: uint const)

format returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+uint const  +
+----+------------+


|function-builtin-format|

.. _function-_at__builtin__c__c_format_string_hh_const_uint64_hh_const__hh_const:

.. das:function:: format(arg0: string const; arg1: uint64 const)

format returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+uint64 const+
+----+------------+


|function-builtin-format|

.. _function-_at__builtin__c__c_peek__hh_handle_hh_das_string_hh_const_hh_implicit__hh_block_hh_string_hh_const_hh_temporary_hh__c_void_hh_const_hh_implicit__hh_const:

.. das:function:: peek(arg0: das_string const implicit; arg1: block<(string const#):void> const implicit)

arguments are

+----+-----------------------------------------------------------------------+
+arg0+ :ref:`builtin::das_string <handle-builtin-das_string>`  const implicit+
+----+-----------------------------------------------------------------------+
+arg1+block<(string const#):void> const implicit                             +
+----+-----------------------------------------------------------------------+


|function-builtin-peek|

.. _function-_at__builtin__c__c_repeat_string_hh_const_int_hh_const__hh_const:

.. das:function:: repeat(arg0: string const; arg1: int const)

repeat returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+


|function-builtin-repeat|

.. _function-_at__builtin__c__c_replace_string_hh_const_string_hh_const_string_hh_const__hh_const:

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


|function-builtin-replace|

.. _function-_at__builtin__c__c_reverse_string_hh_const__hh_const:

.. das:function:: reverse(arg0: string const)

reverse returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+


|function-builtin-reverse|

.. _function-_at__builtin__c__c_set__hh_handle_hh_das_string_hh_implicit_string_hh_const:

.. das:function:: set(arg0: das_string implicit; arg1: string const)

arguments are

+----+-----------------------------------------------------------------+
+arg0+ :ref:`builtin::das_string <handle-builtin-das_string>`  implicit+
+----+-----------------------------------------------------------------+
+arg1+string const                                                     +
+----+-----------------------------------------------------------------+


|function-builtin-set|

.. _function-_at__builtin__c__c_slice_string_hh_const_int_hh_const__hh_const:

.. das:function:: slice(arg0: string const; arg1: int const)

slice returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+


|function-builtin-slice|

.. _function-_at__builtin__c__c_slice_string_hh_const_int_hh_const_int_hh_const__hh_const:

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


|function-builtin-slice|

.. _function-_at__builtin__c__c_starts_with_string_hh_const_string_hh_const__hh_const:

.. das:function:: starts_with(arg0: string const; arg1: string const)

starts_with returns bool

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+string const+
+----+------------+


|function-builtin-starts_with|

.. _function-_at__builtin__c__c_strip_string_hh_const__hh_const:

.. das:function:: strip(arg0: string const)

strip returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+


|function-builtin-strip|

.. _function-_at__builtin__c__c_strip_left_string_hh_const__hh_const:

.. das:function:: strip_left(arg0: string const)

strip_left returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+


|function-builtin-strip_left|

.. _function-_at__builtin__c__c_strip_right_string_hh_const__hh_const:

.. das:function:: strip_right(arg0: string const)

strip_right returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+


|function-builtin-strip_right|

.. _function-_at__builtin__c__c_to_char_int_hh_const__hh_const:

.. das:function:: to_char(arg0: int const)

to_char returns string

arguments are

+----+---------+
+arg0+int const+
+----+---------+


|function-builtin-to_char|

.. _function-_at__builtin__c__c_to_float_string_hh_const:

.. das:function:: to_float(arg0: string const)

to_float returns float

arguments are

+----+------------+
+arg0+string const+
+----+------------+


|function-builtin-to_float|

.. _function-_at__builtin__c__c_to_int_string_hh_const:

.. das:function:: to_int(arg0: string const)

to_int returns int

arguments are

+----+------------+
+arg0+string const+
+----+------------+


|function-builtin-to_int|

.. _function-_at__builtin__c__c_to_lower_string_hh_const__hh_const:

.. das:function:: to_lower(arg0: string const)

to_lower returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+


|function-builtin-to_lower|

.. _function-_at__builtin__c__c_to_lower_in_place_string_hh_const:

.. das:function:: to_lower_in_place(arg0: string const)

to_lower_in_place returns string

.. warning:: 
  This is unsafe operation.

arguments are

+----+------------+
+arg0+string const+
+----+------------+


|function-builtin-to_lower_in_place|

.. _function-_at__builtin__c__c_to_upper_string_hh_const__hh_const:

.. das:function:: to_upper(arg0: string const)

to_upper returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+


|function-builtin-to_upper|

.. _function-_at__builtin__c__c_to_upper_in_place_string_hh_const:

.. das:function:: to_upper_in_place(arg0: string const)

to_upper_in_place returns string

.. warning:: 
  This is unsafe operation.

arguments are

+----+------------+
+arg0+string const+
+----+------------+


|function-builtin-to_upper_in_place|

.. _function-_at__builtin__c__c_unescape_string_hh_const__hh_const:

.. das:function:: unescape(arg0: string const)

unescape returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+


|function-builtin-unescape|

++++++++++++++
String builder
++++++++++++++

  *  :ref:`write (arg0:$::StringBuilderWriter;arg1:any) : void <function-_at__builtin__c__c_write__hh_handle_hh_StringBuilderWriter_any>` 
  *  :ref:`write_char (arg0:$::StringBuilderWriter implicit;arg1:int const) : void <function-_at__builtin__c__c_write_char__hh_handle_hh_StringBuilderWriter_hh_implicit_int_hh_const>` 
  *  :ref:`write_chars (arg0:$::StringBuilderWriter implicit;arg1:int const;arg2:int const) : void <function-_at__builtin__c__c_write_chars__hh_handle_hh_StringBuilderWriter_hh_implicit_int_hh_const_int_hh_const>` 
  *  :ref:`write_escape_string (arg0:$::StringBuilderWriter implicit;arg1:string const) : void <function-_at__builtin__c__c_write_escape_string__hh_handle_hh_StringBuilderWriter_hh_implicit_string_hh_const>` 

.. _function-_at__builtin__c__c_write__hh_handle_hh_StringBuilderWriter_any:

.. das:function:: write(arg0: StringBuilderWriter; arg1: any)

arguments are

+----+--------------------------------------------------------------------------+
+arg0+ :ref:`builtin::StringBuilderWriter <handle-builtin-StringBuilderWriter>` +
+----+--------------------------------------------------------------------------+
+arg1+any                                                                       +
+----+--------------------------------------------------------------------------+


|function-builtin-write|

.. _function-_at__builtin__c__c_write_char__hh_handle_hh_StringBuilderWriter_hh_implicit_int_hh_const:

.. das:function:: write_char(arg0: StringBuilderWriter implicit; arg1: int const)

arguments are

+----+-----------------------------------------------------------------------------------+
+arg0+ :ref:`builtin::StringBuilderWriter <handle-builtin-StringBuilderWriter>`  implicit+
+----+-----------------------------------------------------------------------------------+
+arg1+int const                                                                          +
+----+-----------------------------------------------------------------------------------+


|function-builtin-write_char|

.. _function-_at__builtin__c__c_write_chars__hh_handle_hh_StringBuilderWriter_hh_implicit_int_hh_const_int_hh_const:

.. das:function:: write_chars(arg0: StringBuilderWriter implicit; arg1: int const; arg2: int const)

arguments are

+----+-----------------------------------------------------------------------------------+
+arg0+ :ref:`builtin::StringBuilderWriter <handle-builtin-StringBuilderWriter>`  implicit+
+----+-----------------------------------------------------------------------------------+
+arg1+int const                                                                          +
+----+-----------------------------------------------------------------------------------+
+arg2+int const                                                                          +
+----+-----------------------------------------------------------------------------------+


|function-builtin-write_chars|

.. _function-_at__builtin__c__c_write_escape_string__hh_handle_hh_StringBuilderWriter_hh_implicit_string_hh_const:

.. das:function:: write_escape_string(arg0: StringBuilderWriter implicit; arg1: string const)

arguments are

+----+-----------------------------------------------------------------------------------+
+arg0+ :ref:`builtin::StringBuilderWriter <handle-builtin-StringBuilderWriter>`  implicit+
+----+-----------------------------------------------------------------------------------+
+arg1+string const                                                                       +
+----+-----------------------------------------------------------------------------------+


|function-builtin-write_escape_string|

++++++++++++++
Heap reporting
++++++++++++++

  *  :ref:`heap_bytes_allocated (arg0: const) : uint <function-_at__builtin__c__c_heap_bytes_allocated__hh_const>` 
  *  :ref:`heap_depth (arg0: const) : int <function-_at__builtin__c__c_heap_depth__hh_const>` 
  *  :ref:`heap_high_watermark (arg0: const) : uint <function-_at__builtin__c__c_heap_high_watermark__hh_const>` 
  *  :ref:`heap_report (arg0: const) : void <function-_at__builtin__c__c_heap_report__hh_const>` 
  *  :ref:`string_heap_bytes_allocated (arg0: const) : uint <function-_at__builtin__c__c_string_heap_bytes_allocated__hh_const>` 
  *  :ref:`string_heap_collect (arg0: const;arg1: const) : void <function-_at__builtin__c__c_string_heap_collect__hh_const__hh_const>` 
  *  :ref:`string_heap_depth (arg0: const) : int <function-_at__builtin__c__c_string_heap_depth__hh_const>` 
  *  :ref:`string_heap_high_watermark (arg0: const) : uint <function-_at__builtin__c__c_string_heap_high_watermark__hh_const>` 
  *  :ref:`string_heap_report (arg0: const) : void <function-_at__builtin__c__c_string_heap_report__hh_const>` 

.. _function-_at__builtin__c__c_heap_bytes_allocated__hh_const:

.. das:function:: heap_bytes_allocated()

heap_bytes_allocated returns uint

|function-builtin-heap_bytes_allocated|

.. _function-_at__builtin__c__c_heap_depth__hh_const:

.. das:function:: heap_depth()

heap_depth returns int

|function-builtin-heap_depth|

.. _function-_at__builtin__c__c_heap_high_watermark__hh_const:

.. das:function:: heap_high_watermark()

heap_high_watermark returns uint

|function-builtin-heap_high_watermark|

.. _function-_at__builtin__c__c_heap_report__hh_const:

.. das:function:: heap_report()

|function-builtin-heap_report|

.. _function-_at__builtin__c__c_string_heap_bytes_allocated__hh_const:

.. das:function:: string_heap_bytes_allocated()

string_heap_bytes_allocated returns uint

|function-builtin-string_heap_bytes_allocated|

.. _function-_at__builtin__c__c_string_heap_collect__hh_const__hh_const:

.. das:function:: string_heap_collect()

.. warning:: 
  This is unsafe operation.

|function-builtin-string_heap_collect|

.. _function-_at__builtin__c__c_string_heap_depth__hh_const:

.. das:function:: string_heap_depth()

string_heap_depth returns int

|function-builtin-string_heap_depth|

.. _function-_at__builtin__c__c_string_heap_high_watermark__hh_const:

.. das:function:: string_heap_high_watermark()

string_heap_high_watermark returns uint

|function-builtin-string_heap_high_watermark|

.. _function-_at__builtin__c__c_string_heap_report__hh_const:

.. das:function:: string_heap_report()

|function-builtin-string_heap_report|

++++++++++++++++++++++
Vector and matrix math
++++++++++++++++++++++

  *  :ref:`float3x4 () : $::float3x4 <function-_at__builtin__c__c_float3x4>` 
  *  :ref:`float4x4 () : $::float4x4 <function-_at__builtin__c__c_float4x4>` 
  *  :ref:`identity (arg0:$::float3x4 implicit) : void <function-_at__builtin__c__c_identity__hh_handle_hh_float3x4_hh_implicit>` 
  *  :ref:`identity (arg0:$::float4x4 implicit) : void <function-_at__builtin__c__c_identity__hh_handle_hh_float4x4_hh_implicit>` 
  *  :ref:`inverse (arg0:$::float3x4 const implicit) : $::float3x4 <function-_at__builtin__c__c_inverse__hh_handle_hh_float3x4_hh_const_hh_implicit>` 
  *  :ref:`rotate (arg0:$::float3x4 const implicit;arg1:float3 const) : float3 <function-_at__builtin__c__c_rotate__hh_handle_hh_float3x4_hh_const_hh_implicit_float3_hh_const>` 
  *  :ref:`translation (arg0:float3 const) : $::float4x4 <function-_at__builtin__c__c_translation_float3_hh_const>` 
  *  :ref:`transpose (arg0:$::float4x4 const implicit) : $::float4x4 <function-_at__builtin__c__c_transpose__hh_handle_hh_float4x4_hh_const_hh_implicit>` 

.. _function-_at__builtin__c__c_float3x4:

.. das:function:: float3x4()

float3x4 returns  :ref:`builtin::float3x4 <handle-builtin-float3x4>` 

|function-builtin-float3x4|

.. _function-_at__builtin__c__c_float4x4:

.. das:function:: float4x4()

float4x4 returns  :ref:`builtin::float4x4 <handle-builtin-float4x4>` 

|function-builtin-float4x4|

.. _function-_at__builtin__c__c_identity__hh_handle_hh_float3x4_hh_implicit:

.. das:function:: identity(arg0: float3x4 implicit)

arguments are

+----+-------------------------------------------------------------+
+arg0+ :ref:`builtin::float3x4 <handle-builtin-float3x4>`  implicit+
+----+-------------------------------------------------------------+


|function-builtin-identity|

.. _function-_at__builtin__c__c_identity__hh_handle_hh_float4x4_hh_implicit:

.. das:function:: identity(arg0: float4x4 implicit)

arguments are

+----+-------------------------------------------------------------+
+arg0+ :ref:`builtin::float4x4 <handle-builtin-float4x4>`  implicit+
+----+-------------------------------------------------------------+


|function-builtin-identity|

.. _function-_at__builtin__c__c_inverse__hh_handle_hh_float3x4_hh_const_hh_implicit:

.. das:function:: inverse(arg0: float3x4 const implicit)

inverse returns  :ref:`builtin::float3x4 <handle-builtin-float3x4>` 

arguments are

+----+-------------------------------------------------------------------+
+arg0+ :ref:`builtin::float3x4 <handle-builtin-float3x4>`  const implicit+
+----+-------------------------------------------------------------------+


|function-builtin-inverse|

.. _function-_at__builtin__c__c_rotate__hh_handle_hh_float3x4_hh_const_hh_implicit_float3_hh_const:

.. das:function:: rotate(arg0: float3x4 const implicit; arg1: float3 const)

rotate returns float3

arguments are

+----+-------------------------------------------------------------------+
+arg0+ :ref:`builtin::float3x4 <handle-builtin-float3x4>`  const implicit+
+----+-------------------------------------------------------------------+
+arg1+float3 const                                                       +
+----+-------------------------------------------------------------------+


|function-builtin-rotate|

.. _function-_at__builtin__c__c_translation_float3_hh_const:

.. das:function:: translation(arg0: float3 const)

translation returns  :ref:`builtin::float4x4 <handle-builtin-float4x4>` 

arguments are

+----+------------+
+arg0+float3 const+
+----+------------+


|function-builtin-translation|

.. _function-_at__builtin__c__c_transpose__hh_handle_hh_float4x4_hh_const_hh_implicit:

.. das:function:: transpose(arg0: float4x4 const implicit)

transpose returns  :ref:`builtin::float4x4 <handle-builtin-float4x4>` 

arguments are

+----+-------------------------------------------------------------------+
+arg0+ :ref:`builtin::float4x4 <handle-builtin-float4x4>`  const implicit+
+----+-------------------------------------------------------------------+


|function-builtin-transpose|

++++++++++++++++++
GC0 infrastructure
++++++++++++++++++

  *  :ref:`gc0_reset () : void <function-_at__builtin__c__c_gc0_reset>` 
  *  :ref:`gc0_restore_ptr (arg0:string const;arg1: const) : void? <function-_at__builtin__c__c_gc0_restore_ptr_string_hh_const__hh_const>` 
  *  :ref:`gc0_restore_smart_ptr (arg0:string const;arg1: const) : smart_ptr\<void\> <function-_at__builtin__c__c_gc0_restore_smart_ptr_string_hh_const__hh_const>` 
  *  :ref:`gc0_save_ptr (arg0:string const;arg1:void? const implicit;arg2: const;arg3: const) : void <function-_at__builtin__c__c_gc0_save_ptr_string_hh_const__hh_ptr_hh_const_hh_implicit__hh_const__hh_const>` 
  *  :ref:`gc0_save_smart_ptr (arg0:string const;arg1:smart_ptr\<void\> const implicit;arg2: const;arg3: const) : void <function-_at__builtin__c__c_gc0_save_smart_ptr_string_hh_const__hh_smart_ptr_hh_void_hh_const_hh_implicit__hh_const__hh_const>` 

.. _function-_at__builtin__c__c_gc0_reset:

.. das:function:: gc0_reset()

|function-builtin-gc0_reset|

.. _function-_at__builtin__c__c_gc0_restore_ptr_string_hh_const__hh_const:

.. das:function:: gc0_restore_ptr(arg0: string const)

gc0_restore_ptr returns void?

arguments are

+----+------------+
+arg0+string const+
+----+------------+


|function-builtin-gc0_restore_ptr|

.. _function-_at__builtin__c__c_gc0_restore_smart_ptr_string_hh_const__hh_const:

.. das:function:: gc0_restore_smart_ptr(arg0: string const)

gc0_restore_smart_ptr returns smart_ptr<void>

arguments are

+----+------------+
+arg0+string const+
+----+------------+


|function-builtin-gc0_restore_smart_ptr|

.. _function-_at__builtin__c__c_gc0_save_ptr_string_hh_const__hh_ptr_hh_const_hh_implicit__hh_const__hh_const:

.. das:function:: gc0_save_ptr(arg0: string const; arg1: void? const implicit)

arguments are

+----+--------------------+
+arg0+string const        +
+----+--------------------+
+arg1+void? const implicit+
+----+--------------------+


|function-builtin-gc0_save_ptr|

.. _function-_at__builtin__c__c_gc0_save_smart_ptr_string_hh_const__hh_smart_ptr_hh_void_hh_const_hh_implicit__hh_const__hh_const:

.. das:function:: gc0_save_smart_ptr(arg0: string const; arg1: smart_ptr<void> const implicit)

arguments are

+----+------------------------------+
+arg0+string const                  +
+----+------------------------------+
+arg1+smart_ptr<void> const implicit+
+----+------------------------------+


|function-builtin-gc0_save_smart_ptr|

++++++++++++++++++++++++
Smart ptr infrastructure
++++++++++++++++++++++++

  *  :ref:`smart_ptr_clone (arg0:smart_ptr\<void\>& implicit;arg1:void? const implicit) : void <function-_at__builtin__c__c_smart_ptr_clone__hh_smart_ptr_hh_void_hh_ref_hh_implicit__hh_ptr_hh_const_hh_implicit>` 
  *  :ref:`smart_ptr_clone (arg0:smart_ptr\<void\>& implicit;arg1:smart_ptr\<void\> const implicit) : void <function-_at__builtin__c__c_smart_ptr_clone__hh_smart_ptr_hh_void_hh_ref_hh_implicit__hh_smart_ptr_hh_void_hh_const_hh_implicit>` 
  *  :ref:`smart_ptr_use_count (arg0:smart_ptr\<void\> const implicit) : uint <function-_at__builtin__c__c_smart_ptr_use_count__hh_smart_ptr_hh_void_hh_const_hh_implicit>` 
  *  :ref:`get_ptr (src:smart_ptr\<auto(TT)\> const) : TT? <function-_at__builtin__c__c_get_ptr__hh_smart_ptr_hh__hh_auto_hh_const>` 

.. _function-_at__builtin__c__c_smart_ptr_clone__hh_smart_ptr_hh_void_hh_ref_hh_implicit__hh_ptr_hh_const_hh_implicit:

.. das:function:: smart_ptr_clone(arg0: smart_ptr<void>& implicit; arg1: void? const implicit)

arguments are

+----+-------------------------+
+arg0+smart_ptr<void>& implicit+
+----+-------------------------+
+arg1+void? const implicit     +
+----+-------------------------+


|function-builtin-smart_ptr_clone|

.. _function-_at__builtin__c__c_smart_ptr_clone__hh_smart_ptr_hh_void_hh_ref_hh_implicit__hh_smart_ptr_hh_void_hh_const_hh_implicit:

.. das:function:: smart_ptr_clone(arg0: smart_ptr<void>& implicit; arg1: smart_ptr<void> const implicit)

arguments are

+----+------------------------------+
+arg0+smart_ptr<void>& implicit     +
+----+------------------------------+
+arg1+smart_ptr<void> const implicit+
+----+------------------------------+


|function-builtin-smart_ptr_clone|

.. _function-_at__builtin__c__c_smart_ptr_use_count__hh_smart_ptr_hh_void_hh_const_hh_implicit:

.. das:function:: smart_ptr_use_count(arg0: smart_ptr<void> const implicit)

smart_ptr_use_count returns uint

arguments are

+----+------------------------------+
+arg0+smart_ptr<void> const implicit+
+----+------------------------------+


|function-builtin-smart_ptr_use_count|

.. _function-_at__builtin__c__c_get_ptr__hh_smart_ptr_hh__hh_auto_hh_const:

.. das:function:: get_ptr(src: smart_ptr<auto(TT)> const)

get_ptr returns TT?

arguments are

+---+-------------------------+
+src+smart_ptr<auto(TT)> const+
+---+-------------------------+


|function-builtin-get_ptr|

++++++++++++++++++++
Macro infrastructure
++++++++++++++++++++

  *  :ref:`is_compiling (arg0: const) : bool <function-_at__builtin__c__c_is_compiling__hh_const>` 
  *  :ref:`is_compiling_macros (arg0: const) : bool <function-_at__builtin__c__c_is_compiling_macros__hh_const>` 
  *  :ref:`is_compiling_macros_in_module (arg0:string const;arg1: const) : bool <function-_at__builtin__c__c_is_compiling_macros_in_module_string_hh_const__hh_const>` 

.. _function-_at__builtin__c__c_is_compiling__hh_const:

.. das:function:: is_compiling()

is_compiling returns bool

|function-builtin-is_compiling|

.. _function-_at__builtin__c__c_is_compiling_macros__hh_const:

.. das:function:: is_compiling_macros()

is_compiling_macros returns bool

|function-builtin-is_compiling_macros|

.. _function-_at__builtin__c__c_is_compiling_macros_in_module_string_hh_const__hh_const:

.. das:function:: is_compiling_macros_in_module(arg0: string const)

is_compiling_macros_in_module returns bool

arguments are

+----+------------+
+arg0+string const+
+----+------------+


|function-builtin-is_compiling_macros_in_module|

++++++++
Profiler
++++++++

  *  :ref:`dump_profile_info (arg0: const) : void <function-_at__builtin__c__c_dump_profile_info__hh_const>` 
  *  :ref:`profile (arg0:int const;arg1:string const;arg2:block\<\> const implicit;arg3: const) : float <function-_at__builtin__c__c_profile_int_hh_const_string_hh_const__hh_block_hh_const_hh_implicit__hh_const>` 
  *  :ref:`reset_profiler (arg0: const) : void <function-_at__builtin__c__c_reset_profiler__hh_const>` 

.. _function-_at__builtin__c__c_dump_profile_info__hh_const:

.. das:function:: dump_profile_info()

|function-builtin-dump_profile_info|

.. _function-_at__builtin__c__c_profile_int_hh_const_string_hh_const__hh_block_hh_const_hh_implicit__hh_const:

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


|function-builtin-profile|

.. _function-_at__builtin__c__c_reset_profiler__hh_const:

.. das:function:: reset_profiler()

|function-builtin-reset_profiler|

++++++++++++++++++++
System infastructure
++++++++++++++++++++

  *  :ref:`breakpoint () : void <function-_at__builtin__c__c_breakpoint>` 
  *  :ref:`get_clock () : $::clock <function-_at__builtin__c__c_get_clock>` 
  *  :ref:`get_das_root (arg0: const) : string <function-_at__builtin__c__c_get_das_root__hh_const>` 
  *  :ref:`panic (arg0:string const;arg1: const) : void <function-_at__builtin__c__c_panic_string_hh_const__hh_const>` 
  *  :ref:`print (arg0:string const;arg1: const) : void <function-_at__builtin__c__c_print_string_hh_const__hh_const>` 
  *  :ref:`stackwalk (arg0: const;arg1: const) : void <function-_at__builtin__c__c_stackwalk__hh_const__hh_const>` 
  *  :ref:`terminate (arg0: const) : void <function-_at__builtin__c__c_terminate__hh_const>` 

.. _function-_at__builtin__c__c_breakpoint:

.. das:function:: breakpoint()

|function-builtin-breakpoint|

.. _function-_at__builtin__c__c_get_clock:

.. das:function:: get_clock()

get_clock returns  :ref:`builtin::clock <handle-builtin-clock>` 

|function-builtin-get_clock|

.. _function-_at__builtin__c__c_get_das_root__hh_const:

.. das:function:: get_das_root()

get_das_root returns string

|function-builtin-get_das_root|

.. _function-_at__builtin__c__c_panic_string_hh_const__hh_const:

.. das:function:: panic(arg0: string const)

arguments are

+----+------------+
+arg0+string const+
+----+------------+


|function-builtin-panic|

.. _function-_at__builtin__c__c_print_string_hh_const__hh_const:

.. das:function:: print(arg0: string const)

arguments are

+----+------------+
+arg0+string const+
+----+------------+


|function-builtin-print|

.. _function-_at__builtin__c__c_stackwalk__hh_const__hh_const:

.. das:function:: stackwalk()

|function-builtin-stackwalk|

.. _function-_at__builtin__c__c_terminate__hh_const:

.. das:function:: terminate()

|function-builtin-terminate|

+++++++++++++++++++
Memory manipulation
+++++++++++++++++++

  *  :ref:`hash (arg0:any) : uint <function-_at__builtin__c__c_hash_any>` 
  *  :ref:`memcmp (arg0:void? const implicit;arg1:void? const implicit;arg2:int const) : int <function-_at__builtin__c__c_memcmp__hh_ptr_hh_const_hh_implicit__hh_ptr_hh_const_hh_implicit_int_hh_const>` 
  *  :ref:`set_variant_index (arg0:variant\<\> implicit;arg1:int const) : void <function-_at__builtin__c__c_set_variant_index__hh_variant_hh_implicit_int_hh_const>` 
  *  :ref:`variant_index (arg0:variant\<\> const implicit) : int <function-_at__builtin__c__c_variant_index__hh_variant_hh_const_hh_implicit>` 
  *  :ref:`intptr (p:void? const) : uint64 <function-_at__builtin__c__c_intptr__hh_ptr_hh_void_hh_const>` 

.. _function-_at__builtin__c__c_hash_any:

.. das:function:: hash(arg0: any)

hash returns uint

arguments are

+----+---+
+arg0+any+
+----+---+


|function-builtin-hash|

.. _function-_at__builtin__c__c_memcmp__hh_ptr_hh_const_hh_implicit__hh_ptr_hh_const_hh_implicit_int_hh_const:

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


|function-builtin-memcmp|

.. _function-_at__builtin__c__c_set_variant_index__hh_variant_hh_implicit_int_hh_const:

.. das:function:: set_variant_index(arg0: variant<> implicit; arg1: int const)

.. warning:: 
  This is unsafe operation.

arguments are

+----+------------------+
+arg0+variant<> implicit+
+----+------------------+
+arg1+int const         +
+----+------------------+


|function-builtin-set_variant_index|

.. _function-_at__builtin__c__c_variant_index__hh_variant_hh_const_hh_implicit:

.. das:function:: variant_index(arg0: variant<> const implicit)

variant_index returns int

arguments are

+----+------------------------+
+arg0+variant<> const implicit+
+----+------------------------+


|function-builtin-variant_index|

.. _function-_at__builtin__c__c_intptr__hh_ptr_hh_void_hh_const:

.. das:function:: intptr(p: void? const)

intptr returns uint64

arguments are

+-+-----------+
+p+void? const+
+-+-----------+


|function-builtin-intptr|

+++++++++++++++++
Binary serializer
+++++++++++++++++

  *  :ref:`binary_load (obj:auto -const;data:array\<uint8\> const) : auto <function-_at__builtin__c__c_binary_load__hh_auto__hh_array_hh_uint8_hh_const>` 
  *  :ref:`binary_save (obj:auto const;subexpr:block\<(data:array\<uint8\> const):void\> const) : auto <function-_at__builtin__c__c_binary_save__hh_auto_hh_const__hh_block_hh__hh_array_hh_uint8_hh_const_hh__rq_data_hh__c_void_hh_const>` 

.. _function-_at__builtin__c__c_binary_load__hh_auto__hh_array_hh_uint8_hh_const:

.. das:function:: binary_load(obj: auto; data: array<uint8> const)

binary_load returns auto

arguments are

+----+------------------+
+obj +auto              +
+----+------------------+
+data+array<uint8> const+
+----+------------------+


|function-builtin-binary_load|

.. _function-_at__builtin__c__c_binary_save__hh_auto_hh_const__hh_block_hh__hh_array_hh_uint8_hh_const_hh__rq_data_hh__c_void_hh_const:

.. das:function:: binary_save(obj: auto const; subexpr: block<(data:array<uint8> const):void> const)

binary_save returns auto

arguments are

+-------+-------------------------------------------+
+obj    +auto const                                 +
+-------+-------------------------------------------+
+subexpr+block<(data:array<uint8> const):void> const+
+-------+-------------------------------------------+


|function-builtin-binary_save|


