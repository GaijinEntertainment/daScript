
.. _stdlib_$:

================
Built-in runtime
================

++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-builtin-StringBuilderWriter:

.. das:attribute:: StringBuilderWriter

structure_annotation|$|StringBuilderWriter to be documented

+++++++++++++
Handled types
+++++++++++++

.. _handle-builtin-clock:

.. das:attribute:: clock

any_annotation|$|clock to be documented

.. _handle-builtin-das_string:

.. das:attribute:: das_string

any_annotation|$|das_string to be documented

.. _handle-builtin-float3x4:

.. das:attribute:: float3x4

any_annotation|$|float3x4 to be documented

.. _handle-builtin-float4x4:

.. das:attribute:: float4x4

any_annotation|$|float4x4 to be documented

.. das:function:: binary_load (obj:auto; data:array<uint8> const )  : auto

binary_load returns auto

arguments are

+----+------------------+
+obj +auto              +
+----+------------------+
+data+array<uint8> const+
+----+------------------+


function|$|binary_load to be documented

.. das:function:: binary_save (obj:auto const; subexpr:block<(data:array<uint8> const):void> const )  : auto

binary_save returns auto

arguments are

+-------+-------------------------------------------+
+obj    +auto const                                 +
+-------+-------------------------------------------+
+subexpr+block<(data:array<uint8> const):void> const+
+-------+-------------------------------------------+


function|$|binary_save to be documented

.. das:function:: clone (a:array<auto(TT)>; b:array<TT> const )  : auto

clone returns auto

arguments are

+-+---------------+
+a+array<auto(TT)>+
+-+---------------+
+b+array<TT> const+
+-+---------------+


function|$|clone to be documented

.. das:function:: clone (a:array<auto(TT)>; b:array<TT> const# )  : auto

clone returns auto

arguments are

+-+----------------+
+a+array<auto(TT)> +
+-+----------------+
+b+array<TT> const#+
+-+----------------+


function|$|clone to be documented

.. das:function:: clone (a:table<auto(KT);auto(VT)>; b:table<KT;VT> const )  : auto

clone returns auto

arguments are

+-+------------------------+
+a+table<auto(KT);auto(VT)>+
+-+------------------------+
+b+table<KT;VT> const      +
+-+------------------------+


function|$|clone to be documented

.. das:function:: clone (a:table<auto(KT);auto(VT)>; b:table<KT;VT> const# )  : auto

clone returns auto

arguments are

+-+------------------------+
+a+table<auto(KT);auto(VT)>+
+-+------------------------+
+b+table<KT;VT> const#     +
+-+------------------------+


function|$|clone to be documented

.. das:function:: clone_dim (a:auto; b:auto const )  : auto

clone_dim returns auto

arguments are

+-+----------+
+a+auto      +
+-+----------+
+b+auto const+
+-+----------+


function|$|clone_dim to be documented

.. das:function:: clone_dim (a:auto; b:auto const# )  : auto

clone_dim returns auto

arguments are

+-+-----------+
+a+auto       +
+-+-----------+
+b+auto const#+
+-+-----------+


function|$|clone_dim to be documented

.. das:function:: clone_to_move (clone_src:auto(TT) const )  : TT

clone_to_move returns TT

arguments are

+---------+--------------+
+clone_src+auto(TT) const+
+---------+--------------+


function|$|clone_to_move to be documented

.. das:function:: each (a:array<auto(TT)> const )  : iterator<TT&>

each returns iterator<TT&>

.. warning:: 
  This is unsafe operation.

arguments are

+-+---------------------+
+a+array<auto(TT)> const+
+-+---------------------+


function|$|each to be documented

.. das:function:: each (a:auto(TT) const[-1] )  : iterator<TT&>

each returns iterator<TT&>

.. warning:: 
  This is unsafe operation.

arguments are

+-+------------------+
+a+auto(TT) const[-1]+
+-+------------------+


function|$|each to be documented

.. das:function:: each (lam:lambda<(arg:auto(argT) -const):bool> const )  : iterator<argT -&>

each returns iterator<argT>

arguments are

+---+-----------------------------------+
+lam+lambda<(arg:auto(argT)):bool> const+
+---+-----------------------------------+


function|$|each to be documented

.. das:function:: each (rng:range const )  : iterator<int>

each returns iterator<int>

arguments are

+---+-----------+
+rng+range const+
+---+-----------+


function|$|each to be documented

.. das:function:: each (str:string const )  : iterator<int>

each returns iterator<int>

.. warning:: 
  This is unsafe operation.

arguments are

+---+------------+
+str+string const+
+---+------------+


function|$|each to be documented

.. das:function:: each_enum (tt:auto(TT) const )  : iterator<TT -const -&>

each_enum returns iterator<TT>

arguments are

+--+--------------+
+tt+auto(TT) const+
+--+--------------+


function|$|each_enum to be documented

.. das:function:: each_ref (lam:lambda<(arg:auto(argT)? -const):bool> const )  : iterator<argT&>

each_ref returns iterator<argT&>

arguments are

+---+------------------------------------+
+lam+lambda<(arg:auto(argT)?):bool> const+
+---+------------------------------------+


function|$|each_ref to be documented

.. das:function:: emplace (Arr:array<auto(numT)>; value:numT; at:int const )  : auto

emplace returns auto

arguments are

+-----+-----------------+
+Arr  +array<auto(numT)>+
+-----+-----------------+
+value+numT             +
+-----+-----------------+
+at   +int const        +
+-----+-----------------+


function|$|emplace to be documented

.. das:function:: erase (Arr:array<auto(numT)>; at:int const )  : auto

erase returns auto

arguments are

+---+-----------------+
+Arr+array<auto(numT)>+
+---+-----------------+
+at +int const        +
+---+-----------------+


function|$|erase to be documented

.. das:function:: erase (Tab:table<auto(keyT);auto(valT)>; at:keyT const )  : bool

erase returns bool

arguments are

+---+----------------------------+
+Tab+table<auto(keyT);auto(valT)>+
+---+----------------------------+
+at +keyT const                  +
+---+----------------------------+


function|$|erase to be documented

.. das:function:: finalize (a:array<auto(TT)> )  : auto

finalize returns auto

arguments are

+-+---------------+
+a+array<auto(TT)>+
+-+---------------+


function|$|finalize to be documented

.. das:function:: finalize (a:table<auto(TK);auto(TV)> )  : auto

finalize returns auto

arguments are

+-+------------------------+
+a+table<auto(TK);auto(TV)>+
+-+------------------------+


function|$|finalize to be documented

.. das:function:: finalize_dim (a:auto(TT)[-1] )  : auto

finalize_dim returns auto

arguments are

+-+------------+
+a+auto(TT)[-1]+
+-+------------+


function|$|finalize_dim to be documented

.. das:function:: find (Tab:table<auto(keyT);auto(valT)> const; at:keyT const )  : valT const?

find returns valT const?

.. warning:: 
  This is unsafe operation.

arguments are

+---+----------------------------------+
+Tab+table<auto(keyT);auto(valT)> const+
+---+----------------------------------+
+at +keyT const                        +
+---+----------------------------------+


function|$|find to be documented

.. das:function:: find (Tab:table<auto(keyT);auto(valT)> const; at:keyT const; blk:block<(p:valT? const):void> const )  : auto

find returns auto

arguments are

+---+----------------------------------+
+Tab+table<auto(keyT);auto(valT)> const+
+---+----------------------------------+
+at +keyT const                        +
+---+----------------------------------+
+blk+block<(p:valT? const):void> const +
+---+----------------------------------+


function|$|find to be documented

.. das:function:: find (Tab:table<auto(keyT);auto(valT)> const#; at:keyT const )  : valT const?#

find returns valT const?#

arguments are

+---+-----------------------------------+
+Tab+table<auto(keyT);auto(valT)> const#+
+---+-----------------------------------+
+at +keyT const                         +
+---+-----------------------------------+


function|$|find to be documented

.. das:function:: find_for_edit (Tab:table<auto(keyT);auto(valT)>; at:keyT const )  : valT?

find_for_edit returns valT?

.. warning:: 
  This is unsafe operation.

arguments are

+---+----------------------------+
+Tab+table<auto(keyT);auto(valT)>+
+---+----------------------------+
+at +keyT const                  +
+---+----------------------------+


function|$|find_for_edit to be documented

.. das:function:: find_for_edit (Tab:table<auto(keyT);auto(valT)>; at:keyT const; blk:block<(p:valT? -const):void> const )  : auto

find_for_edit returns auto

arguments are

+---+----------------------------+
+Tab+table<auto(keyT);auto(valT)>+
+---+----------------------------+
+at +keyT const                  +
+---+----------------------------+
+blk+block<(p:valT?):void> const +
+---+----------------------------+


function|$|find_for_edit to be documented

.. das:function:: find_for_edit_if_exists (Tab:table<auto(keyT);auto(valT)>; at:keyT const; blk:block<(p:valT? -const):void> const )  : auto

find_for_edit_if_exists returns auto

arguments are

+---+----------------------------+
+Tab+table<auto(keyT);auto(valT)>+
+---+----------------------------+
+at +keyT const                  +
+---+----------------------------+
+blk+block<(p:valT?):void> const +
+---+----------------------------+


function|$|find_for_edit_if_exists to be documented

.. das:function:: find_if_exists (Tab:table<auto(keyT);auto(valT)> const; at:keyT const; blk:block<(p:valT? const):void> const )  : auto

find_if_exists returns auto

arguments are

+---+----------------------------------+
+Tab+table<auto(keyT);auto(valT)> const+
+---+----------------------------------+
+at +keyT const                        +
+---+----------------------------------+
+blk+block<(p:valT? const):void> const +
+---+----------------------------------+


function|$|find_if_exists to be documented

.. das:function:: find_index (arr:array<auto(TT)> const implicit; key:TT const )  : auto

find_index returns auto

arguments are

+---+------------------------------+
+arr+array<auto(TT)> const implicit+
+---+------------------------------+
+key+TT const                      +
+---+------------------------------+


function|$|find_index to be documented

.. das:function:: find_index (arr:auto(TT) const[-1] implicit; key:TT const )  : auto

find_index returns auto

arguments are

+---+---------------------------+
+arr+auto(TT) const[-1] implicit+
+---+---------------------------+
+key+TT const                   +
+---+---------------------------+


function|$|find_index to be documented

.. das:function:: find_index_if (arr:array<auto(TT)> const implicit; blk:block<(key:TT const):bool> const )  : auto

find_index_if returns auto

arguments are

+---+--------------------------------+
+arr+array<auto(TT)> const implicit  +
+---+--------------------------------+
+blk+block<(key:TT const):bool> const+
+---+--------------------------------+


function|$|find_index_if to be documented

.. das:function:: find_index_if (arr:auto(TT) const[-1] implicit; blk:block<(key:TT const):bool> const )  : auto

find_index_if returns auto

arguments are

+---+--------------------------------+
+arr+auto(TT) const[-1] implicit     +
+---+--------------------------------+
+blk+block<(key:TT const):bool> const+
+---+--------------------------------+


function|$|find_index_if to be documented

.. das:function:: get_ptr (src:smart_ptr<auto(TT)> const )  : TT?

get_ptr returns TT?

arguments are

+---+-------------------------+
+src+smart_ptr<auto(TT)> const+
+---+-------------------------+


function|$|get_ptr to be documented

.. das:function:: has_value (a:auto const; key:auto const )  : auto

has_value returns auto

arguments are

+---+----------+
+a  +auto const+
+---+----------+
+key+auto const+
+---+----------+


function|$|has_value to be documented

.. das:function:: intptr (p:void? const )  : uint64

intptr returns uint64

arguments are

+-+-----------+
+p+void? const+
+-+-----------+


function|$|intptr to be documented

.. das:function:: key_exists (Tab:table<auto(keyT);auto(valT)> const; at:keyT const )  : bool

key_exists returns bool

arguments are

+---+----------------------------------+
+Tab+table<auto(keyT);auto(valT)> const+
+---+----------------------------------+
+at +keyT const                        +
+---+----------------------------------+


function|$|key_exists to be documented

.. das:function:: keys (a:table<auto(keyT);auto(valT)> const )  : iterator<keyT const&>

keys returns iterator<keyT const&>

arguments are

+-+----------------------------------+
+a+table<auto(keyT);auto(valT)> const+
+-+----------------------------------+


function|$|keys to be documented

.. das:function:: length (a:auto const[-1] )  : int

length returns int

arguments are

+-+--------------+
+a+auto const[-1]+
+-+--------------+


function|$|length to be documented

.. das:function:: lock (Tab:table<auto(keyT);auto(valT)> const; blk:block<(t:table<keyT;valT> const#):void> const )  : auto

lock returns auto

arguments are

+---+---------------------------------------------+
+Tab+table<auto(keyT);auto(valT)> const           +
+---+---------------------------------------------+
+blk+block<(t:table<keyT;valT> const#):void> const+
+---+---------------------------------------------+


function|$|lock to be documented

.. das:function:: lock (Tab:table<auto(keyT);auto(valT)> const#; blk:block<(t:table<keyT;valT> const#):void> const )  : auto

lock returns auto

arguments are

+---+---------------------------------------------+
+Tab+table<auto(keyT);auto(valT)> const#          +
+---+---------------------------------------------+
+blk+block<(t:table<keyT;valT> const#):void> const+
+---+---------------------------------------------+


function|$|lock to be documented

.. das:function:: lock_forever (Tab:table<auto(keyT);auto(valT)> )  : table<keyT;valT>#

lock_forever returns table<keyT;valT>#

arguments are

+---+----------------------------+
+Tab+table<auto(keyT);auto(valT)>+
+---+----------------------------+


function|$|lock_forever to be documented

.. das:function:: make_clone (res:auto(TT) const )  : TT

make_clone returns TT

arguments are

+---+--------------+
+res+auto(TT) const+
+---+--------------+


function|$|make_clone to be documented

.. das:function:: next (it:iterator<auto(TT)> const; value:TT& )  : bool

next returns bool

arguments are

+-----+------------------------+
+it   +iterator<auto(TT)> const+
+-----+------------------------+
+value+TT&                     +
+-----+------------------------+


function|$|next to be documented

.. das:function:: nothing (it:iterator<auto(TT)> )  : iterator<TT>

nothing returns iterator<TT>

arguments are

+--+------------------+
+it+iterator<auto(TT)>+
+--+------------------+


function|$|nothing to be documented

.. das:function:: pop (Arr:array<auto(numT)> )  : auto

pop returns auto

arguments are

+---+-----------------+
+Arr+array<auto(numT)>+
+---+-----------------+


function|$|pop to be documented

.. das:function:: push (Arr:array<auto(numT)>; value:numT const; at:int const )  : auto

push returns auto

arguments are

+-----+-----------------+
+Arr  +array<auto(numT)>+
+-----+-----------------+
+value+numT const       +
+-----+-----------------+
+at   +int const        +
+-----+-----------------+


function|$|push to be documented

.. das:function:: push_clone (Arr:array<auto(numT)>; value:numT const; at:int const )  : auto

push_clone returns auto

arguments are

+-----+-----------------+
+Arr  +array<auto(numT)>+
+-----+-----------------+
+value+numT const       +
+-----+-----------------+
+at   +int const        +
+-----+-----------------+


function|$|push_clone to be documented

.. das:function:: reserve (Arr:array<auto(numT)>; newSize:int const )  : auto

reserve returns auto

arguments are

+-------+-----------------+
+Arr    +array<auto(numT)>+
+-------+-----------------+
+newSize+int const        +
+-------+-----------------+


function|$|reserve to be documented

.. das:function:: resize (Arr:array<auto(numT)>; newSize:int const )  : auto

resize returns auto

arguments are

+-------+-----------------+
+Arr    +array<auto(numT)>+
+-------+-----------------+
+newSize+int const        +
+-------+-----------------+


function|$|resize to be documented

.. das:function:: sort (a:array<auto(TT)> )  : auto

sort returns auto

arguments are

+-+---------------+
+a+array<auto(TT)>+
+-+---------------+


function|$|sort to be documented

.. das:function:: sort (a:array<auto(TT)>; cmp:block<(x:TT const;y:TT const):bool> const )  : auto

sort returns auto

arguments are

+---+-----------------------------------------+
+a  +array<auto(TT)>                          +
+---+-----------------------------------------+
+cmp+block<(x:TT const;y:TT const):bool> const+
+---+-----------------------------------------+


function|$|sort to be documented

.. das:function:: sort (a:auto(TT)[-1] )  : auto

sort returns auto

arguments are

+-+------------+
+a+auto(TT)[-1]+
+-+------------+


function|$|sort to be documented

.. das:function:: sort (a:auto(TT)[-1]; cmp:block<(x:TT const;y:TT const):bool> const )  : auto

sort returns auto

arguments are

+---+-----------------------------------------+
+a  +auto(TT)[-1]                             +
+---+-----------------------------------------+
+cmp+block<(x:TT const;y:TT const):bool> const+
+---+-----------------------------------------+


function|$|sort to be documented

.. das:function:: to_array (a:auto(TT) const[-1] )  : array<TT -const>

to_array returns array<TT>

arguments are

+-+------------------+
+a+auto(TT) const[-1]+
+-+------------------+


function|$|to_array to be documented

.. das:function:: to_array (it:iterator<auto(TT)> const )  : array<TT -const -&>

to_array returns array<TT>

arguments are

+--+------------------------+
+it+iterator<auto(TT)> const+
+--+------------------------+


function|$|to_array to be documented

.. das:function:: to_array_move (a:auto(TT)[-1] )  : array<TT -const>

to_array_move returns array<TT>

arguments are

+-+------------+
+a+auto(TT)[-1]+
+-+------------+


function|$|to_array_move to be documented

.. das:function:: to_table (a:tuple<auto(keyT);auto(valT)> const[-1] )  : table<keyT -const;valT>

to_table returns table<keyT;valT>

arguments are

+-+--------------------------------------+
+a+tuple<auto(keyT);auto(valT)> const[-1]+
+-+--------------------------------------+


function|$|to_table to be documented

.. das:function:: to_table_move (a:tuple<auto(keyT);auto(valT)>[-1] )  : table<keyT -const;valT>

to_table_move returns table<keyT;valT>

arguments are

+-+--------------------------------+
+a+tuple<auto(keyT);auto(valT)>[-1]+
+-+--------------------------------+


function|$|to_table_move to be documented

.. das:function:: values (a:table<auto(keyT);auto(valT)> const! )  : iterator<valT const&>

values returns iterator<valT const&>

arguments are

+-+-----------------------------------+
+a+table<auto(keyT);auto(valT)> const!+
+-+-----------------------------------+


function|$|values to be documented

.. das:function:: values (a:table<auto(keyT);auto(valT)>! )  : iterator<valT&>

values returns iterator<valT&>

arguments are

+-+-----------------------------+
+a+table<auto(keyT);auto(valT)>!+
+-+-----------------------------+


function|$|values to be documented

.. das:function:: append (arg0:das_string implicit; arg1:int const ) 

arguments are

+----+-----------------------------------------------------------------+
+arg0+ :ref:`builtin::das_string <handle-builtin-das_string>`  implicit+
+----+-----------------------------------------------------------------+
+arg1+int const                                                        +
+----+-----------------------------------------------------------------+


function|$|append to be documented

.. das:function:: append (arg0:string const; arg1:int const )  : string

append returns string

.. warning:: 
  This is unsafe operation.

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+


function|$|append to be documented

.. das:function:: as_string (arg0:array<uint8> const implicit )  : string

as_string returns string

arguments are

+----+---------------------------+
+arg0+array<uint8> const implicit+
+----+---------------------------+


function|$|as_string to be documented

.. das:function:: breakpoint ( ) 

arguments are

+


function|$|breakpoint to be documented

.. das:function:: build_string (arg0:block<(StringBuilderWriter):void> const implicit )  : string

build_string returns string

arguments are

+----+-------------------------------------------------------------------------------------------------------+
+arg0+block<( :ref:`builtin::StringBuilderWriter <handle-builtin-StringBuilderWriter>` ):void> const implicit+
+----+-------------------------------------------------------------------------------------------------------+


function|$|build_string to be documented

.. das:function:: builtin_strdup (arg0:any ) 

.. warning:: 
  This is unsafe operation.

arguments are

+----+---+
+arg0+any+
+----+---+


function|$|builtin_strdup to be documented

.. das:function:: builtin_string_split (arg0:string const; arg1:string const; arg2:block<> const implicit ) 

arguments are

+----+----------------------+
+arg0+string const          +
+----+----------------------+
+arg1+string const          +
+----+----------------------+
+arg2+block<> const implicit+
+----+----------------------+


function|$|builtin_string_split to be documented

.. das:function:: builtin_string_split_by_char (arg0:string const; arg1:string const; arg2:block<> const implicit ) 

arguments are

+----+----------------------+
+arg0+string const          +
+----+----------------------+
+arg1+string const          +
+----+----------------------+
+arg2+block<> const implicit+
+----+----------------------+


function|$|builtin_string_split_by_char to be documented

.. das:function:: capacity (arg0:array const implicit )  : int

capacity returns int

arguments are

+----+--------------------+
+arg0+array const implicit+
+----+--------------------+


function|$|capacity to be documented

.. das:function:: capacity (arg0:table const implicit )  : int

capacity returns int

arguments are

+----+--------------------+
+arg0+table const implicit+
+----+--------------------+


function|$|capacity to be documented

.. das:function:: character_at (arg0:string const; arg1:int const )  : int

character_at returns int

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+


function|$|character_at to be documented

.. das:function:: character_uat (arg0:string const; arg1:int const )  : int

character_uat returns int

.. warning:: 
  This is unsafe operation.

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+


function|$|character_uat to be documented

.. das:function:: chop (arg0:string const; arg1:int const; arg2:int const )  : string

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


function|$|chop to be documented

.. das:function:: clear (arg0:array implicit ) 

arguments are

+----+--------------+
+arg0+array implicit+
+----+--------------+


function|$|clear to be documented

.. das:function:: clear (arg0:table implicit ) 

arguments are

+----+--------------+
+arg0+table implicit+
+----+--------------+


function|$|clear to be documented

.. das:function:: clone (a:das_string; b:string const ) 

arguments are

+-+--------------------------------------------------------+
+a+ :ref:`builtin::das_string <handle-builtin-das_string>` +
+-+--------------------------------------------------------+
+b+string const                                            +
+-+--------------------------------------------------------+


function|$|clone to be documented

.. das:function:: clone_string (arg0:string const )  : string

clone_string returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|$|clone_string to be documented

.. das:function:: delete_string (arg0:string& implicit ) 

.. warning:: 
  This is unsafe operation.

arguments are

+----+----------------+
+arg0+string& implicit+
+----+----------------+


function|$|delete_string to be documented

.. das:function:: dump_profile_info ( ) 

arguments are

+


function|$|dump_profile_info to be documented

.. das:function:: empty (arg0:das_string const implicit )  : bool

empty returns bool

arguments are

+----+-----------------------------------------------------------------------+
+arg0+ :ref:`builtin::das_string <handle-builtin-das_string>`  const implicit+
+----+-----------------------------------------------------------------------+


function|$|empty to be documented

.. das:function:: empty (arg0:iterator const implicit )  : bool

empty returns bool

arguments are

+----+-----------------------+
+arg0+iterator const implicit+
+----+-----------------------+


function|$|empty to be documented

.. das:function:: empty (arg0:string const )  : bool

empty returns bool

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|$|empty to be documented

.. das:function:: ends_with (arg0:das_string const implicit; arg1:string const )  : bool

ends_with returns bool

arguments are

+----+-----------------------------------------------------------------------+
+arg0+ :ref:`builtin::das_string <handle-builtin-das_string>`  const implicit+
+----+-----------------------------------------------------------------------+
+arg1+string const                                                           +
+----+-----------------------------------------------------------------------+


function|$|ends_with to be documented

.. das:function:: ends_with (arg0:string const; arg1:string const )  : bool

ends_with returns bool

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+string const+
+----+------------+


function|$|ends_with to be documented

.. das:function:: escape (arg0:string const )  : string

escape returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|$|escape to be documented

.. das:function:: find (arg0:string const; arg1:string const )  : int

find returns int

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+string const+
+----+------------+


function|$|find to be documented

.. das:function:: find (arg0:string const; arg1:string const; arg2:int const )  : int

find returns int

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+string const+
+----+------------+
+arg2+int const   +
+----+------------+


function|$|find to be documented

.. das:function:: find_first_of (arg0:string const; arg1:int const )  : int

find_first_of returns int

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+


function|$|find_first_of to be documented

.. das:function:: find_first_of (arg0:string const; arg1:string const )  : int

find_first_of returns int

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+string const+
+----+------------+


function|$|find_first_of to be documented

.. das:function:: float3x4 ( )  : float3x4

float3x4 returns  :ref:`builtin::float3x4 <handle-builtin-float3x4>` 

arguments are

+


function|$|float3x4 to be documented

.. das:function:: float4x4 ( )  : float4x4

float4x4 returns  :ref:`builtin::float4x4 <handle-builtin-float4x4>` 

arguments are

+


function|$|float4x4 to be documented

.. das:function:: format (arg0:StringBuilderWriter implicit; arg1:string const; arg2:double const ) 

arguments are

+----+-----------------------------------------------------------------------------------+
+arg0+ :ref:`builtin::StringBuilderWriter <handle-builtin-StringBuilderWriter>`  implicit+
+----+-----------------------------------------------------------------------------------+
+arg1+string const                                                                       +
+----+-----------------------------------------------------------------------------------+
+arg2+double const                                                                       +
+----+-----------------------------------------------------------------------------------+


function|$|format to be documented

.. das:function:: format (arg0:StringBuilderWriter implicit; arg1:string const; arg2:float const ) 

arguments are

+----+-----------------------------------------------------------------------------------+
+arg0+ :ref:`builtin::StringBuilderWriter <handle-builtin-StringBuilderWriter>`  implicit+
+----+-----------------------------------------------------------------------------------+
+arg1+string const                                                                       +
+----+-----------------------------------------------------------------------------------+
+arg2+float const                                                                        +
+----+-----------------------------------------------------------------------------------+


function|$|format to be documented

.. das:function:: format (arg0:StringBuilderWriter implicit; arg1:string const; arg2:int const ) 

arguments are

+----+-----------------------------------------------------------------------------------+
+arg0+ :ref:`builtin::StringBuilderWriter <handle-builtin-StringBuilderWriter>`  implicit+
+----+-----------------------------------------------------------------------------------+
+arg1+string const                                                                       +
+----+-----------------------------------------------------------------------------------+
+arg2+int const                                                                          +
+----+-----------------------------------------------------------------------------------+


function|$|format to be documented

.. das:function:: format (arg0:StringBuilderWriter implicit; arg1:string const; arg2:int64 const ) 

arguments are

+----+-----------------------------------------------------------------------------------+
+arg0+ :ref:`builtin::StringBuilderWriter <handle-builtin-StringBuilderWriter>`  implicit+
+----+-----------------------------------------------------------------------------------+
+arg1+string const                                                                       +
+----+-----------------------------------------------------------------------------------+
+arg2+int64 const                                                                        +
+----+-----------------------------------------------------------------------------------+


function|$|format to be documented

.. das:function:: format (arg0:StringBuilderWriter implicit; arg1:string const; arg2:uint const ) 

arguments are

+----+-----------------------------------------------------------------------------------+
+arg0+ :ref:`builtin::StringBuilderWriter <handle-builtin-StringBuilderWriter>`  implicit+
+----+-----------------------------------------------------------------------------------+
+arg1+string const                                                                       +
+----+-----------------------------------------------------------------------------------+
+arg2+uint const                                                                         +
+----+-----------------------------------------------------------------------------------+


function|$|format to be documented

.. das:function:: format (arg0:StringBuilderWriter implicit; arg1:string const; arg2:uint64 const ) 

arguments are

+----+-----------------------------------------------------------------------------------+
+arg0+ :ref:`builtin::StringBuilderWriter <handle-builtin-StringBuilderWriter>`  implicit+
+----+-----------------------------------------------------------------------------------+
+arg1+string const                                                                       +
+----+-----------------------------------------------------------------------------------+
+arg2+uint64 const                                                                       +
+----+-----------------------------------------------------------------------------------+


function|$|format to be documented

.. das:function:: format (arg0:string const; arg1:double const )  : string

format returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+double const+
+----+------------+


function|$|format to be documented

.. das:function:: format (arg0:string const; arg1:float const )  : string

format returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+float const +
+----+------------+


function|$|format to be documented

.. das:function:: format (arg0:string const; arg1:int const )  : string

format returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+


function|$|format to be documented

.. das:function:: format (arg0:string const; arg1:int64 const )  : string

format returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int64 const +
+----+------------+


function|$|format to be documented

.. das:function:: format (arg0:string const; arg1:uint const )  : string

format returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+uint const  +
+----+------------+


function|$|format to be documented

.. das:function:: format (arg0:string const; arg1:uint64 const )  : string

format returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+uint64 const+
+----+------------+


function|$|format to be documented

.. das:function:: gc0_reset ( ) 

arguments are

+


function|$|gc0_reset to be documented

.. das:function:: gc0_restore_ptr (arg0:string const )  : void?

gc0_restore_ptr returns void?

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|$|gc0_restore_ptr to be documented

.. das:function:: gc0_restore_smart_ptr (arg0:string const )  : smart_ptr<void>

gc0_restore_smart_ptr returns smart_ptr<void>

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|$|gc0_restore_smart_ptr to be documented

.. das:function:: gc0_save_ptr (arg0:string const; arg1:void? const implicit ) 

arguments are

+----+--------------------+
+arg0+string const        +
+----+--------------------+
+arg1+void? const implicit+
+----+--------------------+


function|$|gc0_save_ptr to be documented

.. das:function:: gc0_save_smart_ptr (arg0:string const; arg1:smart_ptr<void> const implicit ) 

arguments are

+----+------------------------------+
+arg0+string const                  +
+----+------------------------------+
+arg1+smart_ptr<void> const implicit+
+----+------------------------------+


function|$|gc0_save_smart_ptr to be documented

.. das:function:: get_clock ( )  : clock

get_clock returns  :ref:`builtin::clock <handle-builtin-clock>` 

arguments are

+


function|$|get_clock to be documented

.. das:function:: get_das_root ( )  : string

get_das_root returns string

arguments are

+


function|$|get_das_root to be documented

.. das:function:: hash (arg0:any )  : uint

hash returns uint

arguments are

+----+---+
+arg0+any+
+----+---+


function|$|hash to be documented

.. das:function:: heap_bytes_allocated ( )  : uint

heap_bytes_allocated returns uint

arguments are

+


function|$|heap_bytes_allocated to be documented

.. das:function:: heap_depth ( )  : int

heap_depth returns int

arguments are

+


function|$|heap_depth to be documented

.. das:function:: heap_high_watermark ( )  : uint

heap_high_watermark returns uint

arguments are

+


function|$|heap_high_watermark to be documented

.. das:function:: heap_report ( ) 

arguments are

+


function|$|heap_report to be documented

.. das:function:: i_das_ptr_add (arg0:void? const implicit; arg1:int const; arg2:int const )  : void?

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


function|$|i_das_ptr_add to be documented

.. das:function:: i_das_ptr_dec (arg0:void?& implicit; arg1:int const ) 

.. warning:: 
  This is unsafe operation.

arguments are

+----+---------------+
+arg0+void?& implicit+
+----+---------------+
+arg1+int const      +
+----+---------------+


function|$|i_das_ptr_dec to be documented

.. das:function:: i_das_ptr_diff (arg0:void? const implicit; arg1:void? const implicit; arg2:int const )  : int64

i_das_ptr_diff returns int64

arguments are

+----+--------------------+
+arg0+void? const implicit+
+----+--------------------+
+arg1+void? const implicit+
+----+--------------------+
+arg2+int const           +
+----+--------------------+


function|$|i_das_ptr_diff to be documented

.. das:function:: i_das_ptr_inc (arg0:void?& implicit; arg1:int const ) 

.. warning:: 
  This is unsafe operation.

arguments are

+----+---------------+
+arg0+void?& implicit+
+----+---------------+
+arg1+int const      +
+----+---------------+


function|$|i_das_ptr_inc to be documented

.. das:function:: i_das_ptr_set_add (arg0:void?& implicit; arg1:int const; arg2:int const ) 

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


function|$|i_das_ptr_set_add to be documented

.. das:function:: i_das_ptr_set_sub (arg0:void?& implicit; arg1:int const; arg2:int const ) 

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


function|$|i_das_ptr_set_sub to be documented

.. das:function:: i_das_ptr_sub (arg0:void?& implicit; arg1:int const; arg2:int const )  : void?

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


function|$|i_das_ptr_sub to be documented

.. das:function:: identity (arg0:float3x4 implicit ) 

arguments are

+----+-------------------------------------------------------------+
+arg0+ :ref:`builtin::float3x4 <handle-builtin-float3x4>`  implicit+
+----+-------------------------------------------------------------+


function|$|identity to be documented

.. das:function:: identity (arg0:float4x4 implicit ) 

arguments are

+----+-------------------------------------------------------------+
+arg0+ :ref:`builtin::float4x4 <handle-builtin-float4x4>`  implicit+
+----+-------------------------------------------------------------+


function|$|identity to be documented

.. das:function:: inverse (arg0:float3x4 const implicit )  : float3x4

inverse returns  :ref:`builtin::float3x4 <handle-builtin-float3x4>` 

arguments are

+----+-------------------------------------------------------------------+
+arg0+ :ref:`builtin::float3x4 <handle-builtin-float3x4>`  const implicit+
+----+-------------------------------------------------------------------+


function|$|inverse to be documented

.. das:function:: is_alpha (arg0:int const )  : bool

is_alpha returns bool

arguments are

+----+---------+
+arg0+int const+
+----+---------+


function|$|is_alpha to be documented

.. das:function:: is_char_in_set (arg0:int const; arg1:uint const[8] implicit )  : bool

is_char_in_set returns bool

arguments are

+----+----------------------+
+arg0+int const             +
+----+----------------------+
+arg1+uint const[8] implicit+
+----+----------------------+


function|$|is_char_in_set to be documented

.. das:function:: is_compiling ( )  : bool

is_compiling returns bool

arguments are

+


function|$|is_compiling to be documented

.. das:function:: is_compiling_macros ( )  : bool

is_compiling_macros returns bool

arguments are

+


function|$|is_compiling_macros to be documented

.. das:function:: is_number (arg0:int const )  : bool

is_number returns bool

arguments are

+----+---------+
+arg0+int const+
+----+---------+


function|$|is_number to be documented

.. das:function:: is_white_space (arg0:int const )  : bool

is_white_space returns bool

arguments are

+----+---------+
+arg0+int const+
+----+---------+


function|$|is_white_space to be documented

.. das:function:: length (arg0:array const implicit )  : int

length returns int

arguments are

+----+--------------------+
+arg0+array const implicit+
+----+--------------------+


function|$|length to be documented

.. das:function:: length (arg0:das_string implicit )  : int

length returns int

arguments are

+----+-----------------------------------------------------------------+
+arg0+ :ref:`builtin::das_string <handle-builtin-das_string>`  implicit+
+----+-----------------------------------------------------------------+


function|$|length to be documented

.. das:function:: length (arg0:table const implicit )  : int

length returns int

arguments are

+----+--------------------+
+arg0+table const implicit+
+----+--------------------+


function|$|length to be documented

.. das:function:: length (arg0:string const )  : int

length returns int

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|$|length to be documented

.. das:function:: memcmp (arg0:void? const implicit; arg1:void? const implicit; arg2:int const )  : int

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


function|$|memcmp to be documented

.. das:function:: panic (arg0:string const ) 

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|$|panic to be documented

.. das:function:: peek (arg0:das_string const implicit; arg1:block<(string const#):void> const implicit ) 

arguments are

+----+-----------------------------------------------------------------------+
+arg0+ :ref:`builtin::das_string <handle-builtin-das_string>`  const implicit+
+----+-----------------------------------------------------------------------+
+arg1+block<(string const#):void> const implicit                             +
+----+-----------------------------------------------------------------------+


function|$|peek to be documented

.. das:function:: print (arg0:string const ) 

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|$|print to be documented

.. das:function:: profile (arg0:int const; arg1:string const; arg2:block<> const implicit )  : float

profile returns float

arguments are

+----+----------------------+
+arg0+int const             +
+----+----------------------+
+arg1+string const          +
+----+----------------------+
+arg2+block<> const implicit+
+----+----------------------+


function|$|profile to be documented

.. das:function:: repeat (arg0:string const; arg1:int const )  : string

repeat returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+


function|$|repeat to be documented

.. das:function:: replace (arg0:string const; arg1:string const; arg2:string const )  : string

replace returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+string const+
+----+------------+
+arg2+string const+
+----+------------+


function|$|replace to be documented

.. das:function:: reset_profiler ( ) 

arguments are

+


function|$|reset_profiler to be documented

.. das:function:: resize (arg0:das_string implicit; arg1:int const ) 

arguments are

+----+-----------------------------------------------------------------+
+arg0+ :ref:`builtin::das_string <handle-builtin-das_string>`  implicit+
+----+-----------------------------------------------------------------+
+arg1+int const                                                        +
+----+-----------------------------------------------------------------+


function|$|resize to be documented

.. das:function:: reverse (arg0:string const )  : string

reverse returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|$|reverse to be documented

.. das:function:: rotate (arg0:float3x4 const implicit; arg1:float3 const )  : float3

rotate returns float3

arguments are

+----+-------------------------------------------------------------------+
+arg0+ :ref:`builtin::float3x4 <handle-builtin-float3x4>`  const implicit+
+----+-------------------------------------------------------------------+
+arg1+float3 const                                                       +
+----+-------------------------------------------------------------------+


function|$|rotate to be documented

.. das:function:: set (arg0:das_string implicit; arg1:string const ) 

arguments are

+----+-----------------------------------------------------------------+
+arg0+ :ref:`builtin::das_string <handle-builtin-das_string>`  implicit+
+----+-----------------------------------------------------------------+
+arg1+string const                                                     +
+----+-----------------------------------------------------------------+


function|$|set to be documented

.. das:function:: set_variant_index (arg0:variant<> implicit; arg1:int const ) 

.. warning:: 
  This is unsafe operation.

arguments are

+----+------------------+
+arg0+variant<> implicit+
+----+------------------+
+arg1+int const         +
+----+------------------+


function|$|set_variant_index to be documented

.. das:function:: slice (arg0:string const; arg1:int const )  : string

slice returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+


function|$|slice to be documented

.. das:function:: slice (arg0:string const; arg1:int const; arg2:int const )  : string

slice returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+
+arg2+int const   +
+----+------------+


function|$|slice to be documented

.. das:function:: smart_ptr_clone (arg0:smart_ptr<void>& implicit; arg1:void? const implicit ) 

arguments are

+----+-------------------------+
+arg0+smart_ptr<void>& implicit+
+----+-------------------------+
+arg1+void? const implicit     +
+----+-------------------------+


function|$|smart_ptr_clone to be documented

.. das:function:: smart_ptr_clone (arg0:smart_ptr<void>& implicit; arg1:smart_ptr<void> const implicit ) 

arguments are

+----+------------------------------+
+arg0+smart_ptr<void>& implicit     +
+----+------------------------------+
+arg1+smart_ptr<void> const implicit+
+----+------------------------------+


function|$|smart_ptr_clone to be documented

.. das:function:: smart_ptr_use_count (arg0:smart_ptr<void> const implicit )  : uint

smart_ptr_use_count returns uint

arguments are

+----+------------------------------+
+arg0+smart_ptr<void> const implicit+
+----+------------------------------+


function|$|smart_ptr_use_count to be documented

.. das:function:: stackwalk ( ) 

arguments are

+


function|$|stackwalk to be documented

.. das:function:: starts_with (arg0:string const; arg1:string const )  : bool

starts_with returns bool

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+string const+
+----+------------+


function|$|starts_with to be documented

.. das:function:: string_heap_bytes_allocated ( )  : uint

string_heap_bytes_allocated returns uint

arguments are

+


function|$|string_heap_bytes_allocated to be documented

.. das:function:: string_heap_collect ( ) 

.. warning:: 
  This is unsafe operation.

arguments are

+


function|$|string_heap_collect to be documented

.. das:function:: string_heap_depth ( )  : int

string_heap_depth returns int

arguments are

+


function|$|string_heap_depth to be documented

.. das:function:: string_heap_high_watermark ( )  : uint

string_heap_high_watermark returns uint

arguments are

+


function|$|string_heap_high_watermark to be documented

.. das:function:: string_heap_report ( ) 

arguments are

+


function|$|string_heap_report to be documented

.. das:function:: strip (arg0:string const )  : string

strip returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|$|strip to be documented

.. das:function:: strip_left (arg0:string const )  : string

strip_left returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|$|strip_left to be documented

.. das:function:: strip_right (arg0:string const )  : string

strip_right returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|$|strip_right to be documented

.. das:function:: terminate ( ) 

arguments are

+


function|$|terminate to be documented

.. das:function:: to_char (arg0:int const )  : string

to_char returns string

arguments are

+----+---------+
+arg0+int const+
+----+---------+


function|$|to_char to be documented

.. das:function:: to_float (arg0:string const )  : float

to_float returns float

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|$|to_float to be documented

.. das:function:: to_int (arg0:string const )  : int

to_int returns int

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|$|to_int to be documented

.. das:function:: to_lower (arg0:string const )  : string

to_lower returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|$|to_lower to be documented

.. das:function:: to_lower_in_place (arg0:string const )  : string

to_lower_in_place returns string

.. warning:: 
  This is unsafe operation.

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|$|to_lower_in_place to be documented

.. das:function:: to_upper (arg0:string const )  : string

to_upper returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|$|to_upper to be documented

.. das:function:: to_upper_in_place (arg0:string const )  : string

to_upper_in_place returns string

.. warning:: 
  This is unsafe operation.

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|$|to_upper_in_place to be documented

.. das:function:: translation (arg0:float3 const )  : float4x4

translation returns  :ref:`builtin::float4x4 <handle-builtin-float4x4>` 

arguments are

+----+------------+
+arg0+float3 const+
+----+------------+


function|$|translation to be documented

.. das:function:: transpose (arg0:float4x4 const implicit )  : float4x4

transpose returns  :ref:`builtin::float4x4 <handle-builtin-float4x4>` 

arguments are

+----+-------------------------------------------------------------------+
+arg0+ :ref:`builtin::float4x4 <handle-builtin-float4x4>`  const implicit+
+----+-------------------------------------------------------------------+


function|$|transpose to be documented

.. das:function:: unescape (arg0:string const )  : string

unescape returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|$|unescape to be documented

.. das:function:: variant_index (arg0:variant<> const implicit )  : int

variant_index returns int

arguments are

+----+------------------------+
+arg0+variant<> const implicit+
+----+------------------------+


function|$|variant_index to be documented

.. das:function:: write (arg0:StringBuilderWriter; arg1:any ) 

arguments are

+----+--------------------------------------------------------------------------+
+arg0+ :ref:`builtin::StringBuilderWriter <handle-builtin-StringBuilderWriter>` +
+----+--------------------------------------------------------------------------+
+arg1+any                                                                       +
+----+--------------------------------------------------------------------------+


function|$|write to be documented

.. das:function:: write_char (arg0:StringBuilderWriter implicit; arg1:int const ) 

arguments are

+----+-----------------------------------------------------------------------------------+
+arg0+ :ref:`builtin::StringBuilderWriter <handle-builtin-StringBuilderWriter>`  implicit+
+----+-----------------------------------------------------------------------------------+
+arg1+int const                                                                          +
+----+-----------------------------------------------------------------------------------+


function|$|write_char to be documented

.. das:function:: write_chars (arg0:StringBuilderWriter implicit; arg1:int const; arg2:int const ) 

arguments are

+----+-----------------------------------------------------------------------------------+
+arg0+ :ref:`builtin::StringBuilderWriter <handle-builtin-StringBuilderWriter>`  implicit+
+----+-----------------------------------------------------------------------------------+
+arg1+int const                                                                          +
+----+-----------------------------------------------------------------------------------+
+arg2+int const                                                                          +
+----+-----------------------------------------------------------------------------------+


function|$|write_chars to be documented

.. das:function:: write_escape_string (arg0:StringBuilderWriter implicit; arg1:string const ) 

arguments are

+----+-----------------------------------------------------------------------------------+
+arg0+ :ref:`builtin::StringBuilderWriter <handle-builtin-StringBuilderWriter>`  implicit+
+----+-----------------------------------------------------------------------------------+
+arg1+string const                                                                       +
+----+-----------------------------------------------------------------------------------+


function|$|write_escape_string to be documented


