
.. _stdlib_$:

================
Built-in runtime
================

++++++++++++++++++
Handled structures
++++++++++++++++++

.. das:attribute:: StringBuilderWriter

to be documented


++++++++
Generics
++++++++

.. das:function:: binary_load(obj;data)

binary_load returns ::

 auto



function arguments are

+----+------------------+
+obj +auto -const       +
+----+------------------+
+data+array<uint8> const+
+----+------------------+



to be documented


.. das:function:: binary_save(obj;subexpr)

binary_save returns ::

 auto



function arguments are

+-------+-------------------------------------------+
+obj    +auto const                                 +
+-------+-------------------------------------------+
+subexpr+block<(data:array<uint8> const):void> const+
+-------+-------------------------------------------+



to be documented


.. das:function:: clone(a;b)

clone returns ::

 auto



function arguments are

+-+----------------------+
+a+array<auto(TT)> -const+
+-+----------------------+
+b+array<TT> const       +
+-+----------------------+



to be documented


.. das:function:: clone(a;b)

clone returns ::

 auto



function arguments are

+-+----------------------+
+a+array<auto(TT)> -const+
+-+----------------------+
+b+array<TT> const#      +
+-+----------------------+



to be documented


.. das:function:: clone(a;b)

clone returns ::

 auto



function arguments are

+-+-------------------------------+
+a+table<auto(KT);auto(VT)> -const+
+-+-------------------------------+
+b+table<KT;VT> const             +
+-+-------------------------------+



to be documented


.. das:function:: clone(a;b)

clone returns ::

 auto



function arguments are

+-+-------------------------------+
+a+table<auto(KT);auto(VT)> -const+
+-+-------------------------------+
+b+table<KT;VT> const#            +
+-+-------------------------------+



to be documented


.. das:function:: clone_dim(a;b)

clone_dim returns ::

 auto



function arguments are

+-+-----------+
+a+auto -const+
+-+-----------+
+b+auto const +
+-+-----------+



to be documented


.. das:function:: clone_dim(a;b)

clone_dim returns ::

 auto



function arguments are

+-+-----------+
+a+auto -const+
+-+-----------+
+b+auto const#+
+-+-----------+



to be documented


.. das:function:: clone_to_move(clone_src)

clone_to_move returns ::

 TT -const



function arguments are

+---------+--------------+
+clone_src+auto(TT) const+
+---------+--------------+



to be documented


.. das:function:: each(a)

each returns ::

 iterator<TT&>



.. warning::

 This is unsafe operation.

function arguments are

+-+---------------------+
+a+array<auto(TT)> const+
+-+---------------------+



to be documented


.. das:function:: each(a)

each returns ::

 iterator<TT&>



.. warning::

 This is unsafe operation.

function arguments are

+-+------------------+
+a+auto(TT) const[-1]+
+-+------------------+



to be documented


.. das:function:: each(lam)

each returns ::

 iterator<argT -&>



function arguments are

+---+------------------------------------------+
+lam+lambda<(arg:auto(argT) -const):bool> const+
+---+------------------------------------------+



to be documented


.. das:function:: each(rng)

each returns ::

 iterator<int>



function arguments are

+---+-----------+
+rng+range const+
+---+-----------+



to be documented


.. das:function:: each(str)

each returns ::

 iterator<int>



.. warning::

 This is unsafe operation.

function arguments are

+---+------------+
+str+string const+
+---+------------+



to be documented


.. das:function:: each_enum(tt)

each_enum returns ::

 iterator<TT -const -&>



function arguments are

+--+--------------+
+tt+auto(TT) const+
+--+--------------+



to be documented


.. das:function:: each_ref(lam)

each_ref returns ::

 iterator<argT&>



function arguments are

+---+-------------------------------------------+
+lam+lambda<(arg:auto(argT)? -const):bool> const+
+---+-------------------------------------------+



to be documented


.. das:function:: emplace(Arr;value;at)

emplace returns ::

 auto



function arguments are

+-----+------------------------+
+Arr  +array<auto(numT)> -const+
+-----+------------------------+
+value+numT -const             +
+-----+------------------------+
+at   +int const               +
+-----+------------------------+



to be documented


.. das:function:: erase(Arr;at)

erase returns ::

 auto



function arguments are

+---+------------------------+
+Arr+array<auto(numT)> -const+
+---+------------------------+
+at +int const               +
+---+------------------------+



to be documented


.. das:function:: erase(Tab;at)

erase returns ::

 bool



function arguments are

+---+-----------------------------------+
+Tab+table<auto(keyT);auto(valT)> -const+
+---+-----------------------------------+
+at +keyT const                         +
+---+-----------------------------------+



to be documented


.. das:function:: finalize(a)

finalize returns ::

 auto



function arguments are

+-+----------------------+
+a+array<auto(TT)> -const+
+-+----------------------+



to be documented


.. das:function:: finalize(a)

finalize returns ::

 auto



function arguments are

+-+-------------------------------+
+a+table<auto(TK);auto(TV)> -const+
+-+-------------------------------+



to be documented


.. das:function:: finalize_dim(a)

finalize_dim returns ::

 auto



function arguments are

+-+-------------------+
+a+auto(TT)[-1] -const+
+-+-------------------+



to be documented


.. das:function:: find(Tab;at)

find returns ::

 valT const?



.. warning::

 This is unsafe operation.

function arguments are

+---+----------------------------------+
+Tab+table<auto(keyT);auto(valT)> const+
+---+----------------------------------+
+at +keyT const                        +
+---+----------------------------------+



to be documented


.. das:function:: find(Tab;at;blk)

find returns ::

 auto



function arguments are

+---+----------------------------------+
+Tab+table<auto(keyT);auto(valT)> const+
+---+----------------------------------+
+at +keyT const                        +
+---+----------------------------------+
+blk+block<(p:valT? const):void> const +
+---+----------------------------------+



to be documented


.. das:function:: find(Tab;at)

find returns ::

 valT const?#



function arguments are

+---+-----------------------------------+
+Tab+table<auto(keyT);auto(valT)> const#+
+---+-----------------------------------+
+at +keyT const                         +
+---+-----------------------------------+



to be documented


.. das:function:: find_for_edit(Tab;at)

find_for_edit returns ::

 valT?



.. warning::

 This is unsafe operation.

function arguments are

+---+-----------------------------------+
+Tab+table<auto(keyT);auto(valT)> -const+
+---+-----------------------------------+
+at +keyT const                         +
+---+-----------------------------------+



to be documented


.. das:function:: find_for_edit(Tab;at;blk)

find_for_edit returns ::

 auto



function arguments are

+---+-----------------------------------+
+Tab+table<auto(keyT);auto(valT)> -const+
+---+-----------------------------------+
+at +keyT const                         +
+---+-----------------------------------+
+blk+block<(p:valT? -const):void> const +
+---+-----------------------------------+



to be documented


.. das:function:: find_for_edit_if_exists(Tab;at;blk)

find_for_edit_if_exists returns ::

 auto



function arguments are

+---+-----------------------------------+
+Tab+table<auto(keyT);auto(valT)> -const+
+---+-----------------------------------+
+at +keyT const                         +
+---+-----------------------------------+
+blk+block<(p:valT? -const):void> const +
+---+-----------------------------------+



to be documented


.. das:function:: find_if_exists(Tab;at;blk)

find_if_exists returns ::

 auto



function arguments are

+---+----------------------------------+
+Tab+table<auto(keyT);auto(valT)> const+
+---+----------------------------------+
+at +keyT const                        +
+---+----------------------------------+
+blk+block<(p:valT? const):void> const +
+---+----------------------------------+



to be documented


.. das:function:: find_index(arr;key)

find_index returns ::

 auto



function arguments are

+---+------------------------------+
+arr+array<auto(TT)> const implicit+
+---+------------------------------+
+key+TT const                      +
+---+------------------------------+



to be documented


.. das:function:: find_index(arr;key)

find_index returns ::

 auto



function arguments are

+---+---------------------------+
+arr+auto(TT) const[-1] implicit+
+---+---------------------------+
+key+TT const                   +
+---+---------------------------+



to be documented


.. das:function:: find_index_if(arr;blk)

find_index_if returns ::

 auto



function arguments are

+---+--------------------------------+
+arr+array<auto(TT)> const implicit  +
+---+--------------------------------+
+blk+block<(key:TT const):bool> const+
+---+--------------------------------+



to be documented


.. das:function:: find_index_if(arr;blk)

find_index_if returns ::

 auto



function arguments are

+---+--------------------------------+
+arr+auto(TT) const[-1] implicit     +
+---+--------------------------------+
+blk+block<(key:TT const):bool> const+
+---+--------------------------------+



to be documented


.. das:function:: get_ptr(src)

get_ptr returns ::

 TT?



function arguments are

+---+-------------------------+
+src+smart_ptr<auto(TT)> const+
+---+-------------------------+



to be documented


.. das:function:: has_value(a;key)

has_value returns ::

 auto



function arguments are

+---+----------+
+a  +auto const+
+---+----------+
+key+auto const+
+---+----------+



to be documented


.. das:function:: intptr(p)

intptr returns ::

 uint64



function arguments are

+-+-----------+
+p+void? const+
+-+-----------+



to be documented


.. das:function:: key_exists(Tab;at)

key_exists returns ::

 bool



function arguments are

+---+----------------------------------+
+Tab+table<auto(keyT);auto(valT)> const+
+---+----------------------------------+
+at +keyT const                        +
+---+----------------------------------+



to be documented


.. das:function:: keys(a)

keys returns ::

 iterator<keyT const&>



function arguments are

+-+----------------------------------+
+a+table<auto(keyT);auto(valT)> const+
+-+----------------------------------+



to be documented


.. das:function:: length(a)

length returns ::

 int



function arguments are

+-+--------------+
+a+auto const[-1]+
+-+--------------+



to be documented


.. das:function:: lock(Tab;blk)

lock returns ::

 auto



function arguments are

+---+---------------------------------------------+
+Tab+table<auto(keyT);auto(valT)> const           +
+---+---------------------------------------------+
+blk+block<(t:table<keyT;valT> const#):void> const+
+---+---------------------------------------------+



to be documented


.. das:function:: lock(Tab;blk)

lock returns ::

 auto



function arguments are

+---+---------------------------------------------+
+Tab+table<auto(keyT);auto(valT)> const#          +
+---+---------------------------------------------+
+blk+block<(t:table<keyT;valT> const#):void> const+
+---+---------------------------------------------+



to be documented


.. das:function:: lock_forever(Tab)

lock_forever returns ::

 table<keyT;valT>#



function arguments are

+---+-----------------------------------+
+Tab+table<auto(keyT);auto(valT)> -const+
+---+-----------------------------------+



to be documented


.. das:function:: make_clone(res)

make_clone returns ::

 TT -const



function arguments are

+---+--------------+
+res+auto(TT) const+
+---+--------------+



to be documented


.. das:function:: next(it;value)

next returns ::

 bool



function arguments are

+-----+------------------------+
+it   +iterator<auto(TT)> const+
+-----+------------------------+
+value+TT& -const              +
+-----+------------------------+



to be documented


.. das:function:: nothing(it)

nothing returns ::

 iterator<TT>



function arguments are

+--+-------------------------+
+it+iterator<auto(TT)> -const+
+--+-------------------------+



to be documented


.. das:function:: pop(Arr)

pop returns ::

 auto



function arguments are

+---+------------------------+
+Arr+array<auto(numT)> -const+
+---+------------------------+



to be documented


.. das:function:: push(Arr;value;at)

push returns ::

 auto



function arguments are

+-----+------------------------+
+Arr  +array<auto(numT)> -const+
+-----+------------------------+
+value+numT const              +
+-----+------------------------+
+at   +int const               +
+-----+------------------------+



to be documented


.. das:function:: push_clone(Arr;value;at)

push_clone returns ::

 auto



function arguments are

+-----+------------------------+
+Arr  +array<auto(numT)> -const+
+-----+------------------------+
+value+numT const              +
+-----+------------------------+
+at   +int const               +
+-----+------------------------+



to be documented


.. das:function:: reserve(Arr;newSize)

reserve returns ::

 auto



function arguments are

+-------+------------------------+
+Arr    +array<auto(numT)> -const+
+-------+------------------------+
+newSize+int const               +
+-------+------------------------+



to be documented


.. das:function:: resize(Arr;newSize)

resize returns ::

 auto



function arguments are

+-------+------------------------+
+Arr    +array<auto(numT)> -const+
+-------+------------------------+
+newSize+int const               +
+-------+------------------------+



to be documented


.. das:function:: sort(a)

sort returns ::

 auto



function arguments are

+-+----------------------+
+a+array<auto(TT)> -const+
+-+----------------------+



to be documented


.. das:function:: sort(a;cmp)

sort returns ::

 auto



function arguments are

+---+-----------------------------------------+
+a  +array<auto(TT)> -const                   +
+---+-----------------------------------------+
+cmp+block<(x:TT const;y:TT const):bool> const+
+---+-----------------------------------------+



to be documented


.. das:function:: sort(a)

sort returns ::

 auto



function arguments are

+-+-------------------+
+a+auto(TT)[-1] -const+
+-+-------------------+



to be documented


.. das:function:: sort(a;cmp)

sort returns ::

 auto



function arguments are

+---+-----------------------------------------+
+a  +auto(TT)[-1] -const                      +
+---+-----------------------------------------+
+cmp+block<(x:TT const;y:TT const):bool> const+
+---+-----------------------------------------+



to be documented


.. das:function:: to_array(a)

to_array returns ::

 array<TT -const>



function arguments are

+-+------------------+
+a+auto(TT) const[-1]+
+-+------------------+



to be documented


.. das:function:: to_array(it)

to_array returns ::

 array<TT -const -&>



function arguments are

+--+------------------------+
+it+iterator<auto(TT)> const+
+--+------------------------+



to be documented


.. das:function:: to_array_move(a)

to_array_move returns ::

 array<TT -const>



function arguments are

+-+-------------------+
+a+auto(TT)[-1] -const+
+-+-------------------+



to be documented


.. das:function:: to_table(a)

to_table returns ::

 table<keyT -const;valT>



function arguments are

+-+--------------------------------------+
+a+tuple<auto(keyT);auto(valT)> const[-1]+
+-+--------------------------------------+



to be documented


.. das:function:: to_table_move(a)

to_table_move returns ::

 table<keyT -const;valT>



function arguments are

+-+---------------------------------------+
+a+tuple<auto(keyT);auto(valT)>[-1] -const+
+-+---------------------------------------+



to be documented


.. das:function:: values(a)

values returns ::

 iterator<valT const&>



function arguments are

+-+-----------------------------------+
+a+table<auto(keyT);auto(valT)> const!+
+-+-----------------------------------+



to be documented


.. das:function:: values(a)

values returns ::

 iterator<valT&>



function arguments are

+-+------------------------------------+
+a+table<auto(keyT);auto(valT)>! -const+
+-+------------------------------------+



to be documented


+++++++++
Functions
+++++++++

.. das:function:: append(arg0;arg1)



function arguments are

+----+----------------------+
+arg0+$::das_string implicit+
+----+----------------------+
+arg1+int const             +
+----+----------------------+



to be documented


.. das:function:: append(arg0;arg1;arg2)

append returns ::

 string



.. warning::

 This is unsafe operation.

function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+
+arg2+ const      +
+----+------------+



to be documented


.. das:function:: as_string(arg0;arg1)

as_string returns ::

 string



function arguments are

+----+---------------------------+
+arg0+array<uint8> const implicit+
+----+---------------------------+
+arg1+ const                     +
+----+---------------------------+



to be documented


.. das:function:: breakpoint




to be documented


.. das:function:: build_string(arg0;arg1)

build_string returns ::

 string



function arguments are

+----+---------------------------------------------------+
+arg0+block<($::StringBuilderWriter):void> const implicit+
+----+---------------------------------------------------+
+arg1+ const                                             +
+----+---------------------------------------------------+



to be documented


.. das:function:: builtin_strdup(arg0)



.. warning::

 This is unsafe operation.

function arguments are

+----+---+
+arg0+any+
+----+---+



to be documented


.. das:function:: builtin_string_split(arg0;arg1;arg2;arg3)



function arguments are

+----+----------------------+
+arg0+string const          +
+----+----------------------+
+arg1+string const          +
+----+----------------------+
+arg2+block<> const implicit+
+----+----------------------+
+arg3+ const                +
+----+----------------------+



to be documented


.. das:function:: builtin_string_split_by_char(arg0;arg1;arg2;arg3)



function arguments are

+----+----------------------+
+arg0+string const          +
+----+----------------------+
+arg1+string const          +
+----+----------------------+
+arg2+block<> const implicit+
+----+----------------------+
+arg3+ const                +
+----+----------------------+



to be documented


.. das:function:: capacity(arg0)

capacity returns ::

 int



function arguments are

+----+--------------------+
+arg0+array const implicit+
+----+--------------------+



to be documented


.. das:function:: capacity(arg0)

capacity returns ::

 int



function arguments are

+----+--------------------+
+arg0+table const implicit+
+----+--------------------+



to be documented


.. das:function:: character_at(arg0;arg1;arg2)

character_at returns ::

 int



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+
+arg2+ const      +
+----+------------+



to be documented


.. das:function:: character_uat(arg0;arg1)

character_uat returns ::

 int



.. warning::

 This is unsafe operation.

function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+



to be documented


.. das:function:: chop(arg0;arg1;arg2;arg3)

chop returns ::

 string



.. warning::

 This is unsafe operation.

function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+
+arg2+int const   +
+----+------------+
+arg3+ const      +
+----+------------+



to be documented


.. das:function:: clear(arg0;arg1)



function arguments are

+----+--------------+
+arg0+array implicit+
+----+--------------+
+arg1+ const        +
+----+--------------+



to be documented


.. das:function:: clear(arg0;arg1)



function arguments are

+----+--------------+
+arg0+table implicit+
+----+--------------+
+arg1+ const        +
+----+--------------+



to be documented


.. das:function:: clone(a;b)



function arguments are

+-+--------------------+
+a+$::das_string -const+
+-+--------------------+
+b+string const        +
+-+--------------------+



to be documented


.. das:function:: clone_string(arg0;arg1)

clone_string returns ::

 string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+ const      +
+----+------------+



to be documented


.. das:function:: delete_string(arg0;arg1)



.. warning::

 This is unsafe operation.

function arguments are

+----+----------------+
+arg0+string& implicit+
+----+----------------+
+arg1+ const          +
+----+----------------+



to be documented


.. das:function:: dump_profile_info(arg0)



function arguments are

+----+------+
+arg0+ const+
+----+------+



to be documented


.. das:function:: empty(arg0)

empty returns ::

 bool



function arguments are

+----+----------------------------+
+arg0+$::das_string const implicit+
+----+----------------------------+



to be documented


.. das:function:: empty(arg0)

empty returns ::

 bool



function arguments are

+----+-----------------------+
+arg0+iterator const implicit+
+----+-----------------------+



to be documented


.. das:function:: empty(arg0)

empty returns ::

 bool



function arguments are

+----+------------+
+arg0+string const+
+----+------------+



to be documented


.. das:function:: ends_with(arg0;arg1;arg2)

ends_with returns ::

 bool



function arguments are

+----+----------------------------+
+arg0+$::das_string const implicit+
+----+----------------------------+
+arg1+string const                +
+----+----------------------------+
+arg2+ const                      +
+----+----------------------------+



to be documented


.. das:function:: ends_with(arg0;arg1;arg2)

ends_with returns ::

 bool



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+string const+
+----+------------+
+arg2+ const      +
+----+------------+



to be documented


.. das:function:: escape(arg0;arg1)

escape returns ::

 string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+ const      +
+----+------------+



to be documented


.. das:function:: find(arg0;arg1)

find returns ::

 int



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+string const+
+----+------------+



to be documented


.. das:function:: find(arg0;arg1;arg2;arg3)

find returns ::

 int



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+string const+
+----+------------+
+arg2+int const   +
+----+------------+
+arg3+ const      +
+----+------------+



to be documented


.. das:function:: find_first_of(arg0;arg1;arg2)

find_first_of returns ::

 int



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+
+arg2+ const      +
+----+------------+



to be documented


.. das:function:: find_first_of(arg0;arg1;arg2)

find_first_of returns ::

 int



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+string const+
+----+------------+
+arg2+ const      +
+----+------------+



to be documented


.. das:function:: float3x4

float3x4 returns ::

 $::float3x4




to be documented


.. das:function:: float4x4

float4x4 returns ::

 $::float4x4




to be documented


.. das:function:: format(arg0;arg1;arg2)



function arguments are

+----+-------------------------------+
+arg0+$::StringBuilderWriter implicit+
+----+-------------------------------+
+arg1+string const                   +
+----+-------------------------------+
+arg2+double const                   +
+----+-------------------------------+



to be documented


.. das:function:: format(arg0;arg1;arg2)



function arguments are

+----+-------------------------------+
+arg0+$::StringBuilderWriter implicit+
+----+-------------------------------+
+arg1+string const                   +
+----+-------------------------------+
+arg2+float const                    +
+----+-------------------------------+



to be documented


.. das:function:: format(arg0;arg1;arg2)



function arguments are

+----+-------------------------------+
+arg0+$::StringBuilderWriter implicit+
+----+-------------------------------+
+arg1+string const                   +
+----+-------------------------------+
+arg2+int const                      +
+----+-------------------------------+



to be documented


.. das:function:: format(arg0;arg1;arg2)



function arguments are

+----+-------------------------------+
+arg0+$::StringBuilderWriter implicit+
+----+-------------------------------+
+arg1+string const                   +
+----+-------------------------------+
+arg2+int64 const                    +
+----+-------------------------------+



to be documented


.. das:function:: format(arg0;arg1;arg2)



function arguments are

+----+-------------------------------+
+arg0+$::StringBuilderWriter implicit+
+----+-------------------------------+
+arg1+string const                   +
+----+-------------------------------+
+arg2+uint const                     +
+----+-------------------------------+



to be documented


.. das:function:: format(arg0;arg1;arg2)



function arguments are

+----+-------------------------------+
+arg0+$::StringBuilderWriter implicit+
+----+-------------------------------+
+arg1+string const                   +
+----+-------------------------------+
+arg2+uint64 const                   +
+----+-------------------------------+



to be documented


.. das:function:: format(arg0;arg1;arg2)

format returns ::

 string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+double const+
+----+------------+
+arg2+ const      +
+----+------------+



to be documented


.. das:function:: format(arg0;arg1;arg2)

format returns ::

 string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+float const +
+----+------------+
+arg2+ const      +
+----+------------+



to be documented


.. das:function:: format(arg0;arg1;arg2)

format returns ::

 string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+
+arg2+ const      +
+----+------------+



to be documented


.. das:function:: format(arg0;arg1;arg2)

format returns ::

 string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int64 const +
+----+------------+
+arg2+ const      +
+----+------------+



to be documented


.. das:function:: format(arg0;arg1;arg2)

format returns ::

 string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+uint const  +
+----+------------+
+arg2+ const      +
+----+------------+



to be documented


.. das:function:: format(arg0;arg1;arg2)

format returns ::

 string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+uint64 const+
+----+------------+
+arg2+ const      +
+----+------------+



to be documented


.. das:function:: gc0_reset




to be documented


.. das:function:: gc0_restore_ptr(arg0;arg1)

gc0_restore_ptr returns ::

 void?



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+ const      +
+----+------------+



to be documented


.. das:function:: gc0_restore_smart_ptr(arg0;arg1)

gc0_restore_smart_ptr returns ::

 smart_ptr<void>



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+ const      +
+----+------------+



to be documented


.. das:function:: gc0_save_ptr(arg0;arg1;arg2;arg3)



function arguments are

+----+--------------------+
+arg0+string const        +
+----+--------------------+
+arg1+void? const implicit+
+----+--------------------+
+arg2+ const              +
+----+--------------------+
+arg3+ const              +
+----+--------------------+



to be documented


.. das:function:: gc0_save_smart_ptr(arg0;arg1;arg2;arg3)



function arguments are

+----+------------------------------+
+arg0+string const                  +
+----+------------------------------+
+arg1+smart_ptr<void> const implicit+
+----+------------------------------+
+arg2+ const                        +
+----+------------------------------+
+arg3+ const                        +
+----+------------------------------+



to be documented


.. das:function:: get_clock

get_clock returns ::

 $::clock




to be documented


.. das:function:: get_das_root(arg0)

get_das_root returns ::

 string



function arguments are

+----+------+
+arg0+ const+
+----+------+



to be documented


.. das:function:: hash(arg0)

hash returns ::

 uint



function arguments are

+----+---+
+arg0+any+
+----+---+



to be documented


.. das:function:: heap_bytes_allocated(arg0)

heap_bytes_allocated returns ::

 uint



function arguments are

+----+------+
+arg0+ const+
+----+------+



to be documented


.. das:function:: heap_depth(arg0)

heap_depth returns ::

 int



function arguments are

+----+------+
+arg0+ const+
+----+------+



to be documented


.. das:function:: heap_high_watermark(arg0)

heap_high_watermark returns ::

 uint



function arguments are

+----+------+
+arg0+ const+
+----+------+



to be documented


.. das:function:: heap_report(arg0)



function arguments are

+----+------+
+arg0+ const+
+----+------+



to be documented


.. das:function:: i_das_ptr_add(arg0;arg1;arg2)

i_das_ptr_add returns ::

 void?



.. warning::

 This is unsafe operation.

function arguments are

+----+--------------------+
+arg0+void? const implicit+
+----+--------------------+
+arg1+int const           +
+----+--------------------+
+arg2+int const           +
+----+--------------------+



to be documented


.. das:function:: i_das_ptr_dec(arg0;arg1)



.. warning::

 This is unsafe operation.

function arguments are

+----+---------------+
+arg0+void?& implicit+
+----+---------------+
+arg1+int const      +
+----+---------------+



to be documented


.. das:function:: i_das_ptr_diff(arg0;arg1;arg2)

i_das_ptr_diff returns ::

 int64



function arguments are

+----+--------------------+
+arg0+void? const implicit+
+----+--------------------+
+arg1+void? const implicit+
+----+--------------------+
+arg2+int const           +
+----+--------------------+



to be documented


.. das:function:: i_das_ptr_inc(arg0;arg1)



.. warning::

 This is unsafe operation.

function arguments are

+----+---------------+
+arg0+void?& implicit+
+----+---------------+
+arg1+int const      +
+----+---------------+



to be documented


.. das:function:: i_das_ptr_set_add(arg0;arg1;arg2)



.. warning::

 This is unsafe operation.

function arguments are

+----+---------------+
+arg0+void?& implicit+
+----+---------------+
+arg1+int const      +
+----+---------------+
+arg2+int const      +
+----+---------------+



to be documented


.. das:function:: i_das_ptr_set_sub(arg0;arg1;arg2)



.. warning::

 This is unsafe operation.

function arguments are

+----+---------------+
+arg0+void?& implicit+
+----+---------------+
+arg1+int const      +
+----+---------------+
+arg2+int const      +
+----+---------------+



to be documented


.. das:function:: i_das_ptr_sub(arg0;arg1;arg2)

i_das_ptr_sub returns ::

 void?



.. warning::

 This is unsafe operation.

function arguments are

+----+---------------+
+arg0+void?& implicit+
+----+---------------+
+arg1+int const      +
+----+---------------+
+arg2+int const      +
+----+---------------+



to be documented


.. das:function:: identity(arg0)



function arguments are

+----+--------------------+
+arg0+$::float3x4 implicit+
+----+--------------------+



to be documented


.. das:function:: identity(arg0)



function arguments are

+----+--------------------+
+arg0+$::float4x4 implicit+
+----+--------------------+



to be documented


.. das:function:: inverse(arg0)

inverse returns ::

 $::float3x4



function arguments are

+----+--------------------------+
+arg0+$::float3x4 const implicit+
+----+--------------------------+



to be documented


.. das:function:: is_alpha(arg0)

is_alpha returns ::

 bool



function arguments are

+----+---------+
+arg0+int const+
+----+---------+



to be documented


.. das:function:: is_char_in_set(arg0;arg1)

is_char_in_set returns ::

 bool



function arguments are

+----+----------------------+
+arg0+int const             +
+----+----------------------+
+arg1+uint const[8] implicit+
+----+----------------------+



to be documented


.. das:function:: is_compiling(arg0)

is_compiling returns ::

 bool



function arguments are

+----+------+
+arg0+ const+
+----+------+



to be documented


.. das:function:: is_compiling_macros(arg0)

is_compiling_macros returns ::

 bool



function arguments are

+----+------+
+arg0+ const+
+----+------+



to be documented


.. das:function:: is_number(arg0)

is_number returns ::

 bool



function arguments are

+----+---------+
+arg0+int const+
+----+---------+



to be documented


.. das:function:: is_white_space(arg0)

is_white_space returns ::

 bool



function arguments are

+----+---------+
+arg0+int const+
+----+---------+



to be documented


.. das:function:: length(arg0)

length returns ::

 int



function arguments are

+----+--------------------+
+arg0+array const implicit+
+----+--------------------+



to be documented


.. das:function:: length(arg0)

length returns ::

 int



function arguments are

+----+----------------------+
+arg0+$::das_string implicit+
+----+----------------------+



to be documented


.. das:function:: length(arg0)

length returns ::

 int



function arguments are

+----+--------------------+
+arg0+table const implicit+
+----+--------------------+



to be documented


.. das:function:: length(arg0;arg1)

length returns ::

 int



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+ const      +
+----+------------+



to be documented


.. das:function:: memcmp(arg0;arg1;arg2)

memcmp returns ::

 int



.. warning::

 This is unsafe operation.

function arguments are

+----+--------------------+
+arg0+void? const implicit+
+----+--------------------+
+arg1+void? const implicit+
+----+--------------------+
+arg2+int const           +
+----+--------------------+



to be documented


.. das:function:: panic(arg0;arg1)



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+ const      +
+----+------------+



to be documented


.. das:function:: peek(arg0;arg1;arg2)



function arguments are

+----+------------------------------------------+
+arg0+$::das_string const implicit              +
+----+------------------------------------------+
+arg1+block<(string const#):void> const implicit+
+----+------------------------------------------+
+arg2+ const                                    +
+----+------------------------------------------+



to be documented


.. das:function:: print(arg0;arg1)



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+ const      +
+----+------------+



to be documented


.. das:function:: profile(arg0;arg1;arg2;arg3)

profile returns ::

 float



function arguments are

+----+----------------------+
+arg0+int const             +
+----+----------------------+
+arg1+string const          +
+----+----------------------+
+arg2+block<> const implicit+
+----+----------------------+
+arg3+ const                +
+----+----------------------+



to be documented


.. das:function:: repeat(arg0;arg1;arg2)

repeat returns ::

 string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+
+arg2+ const      +
+----+------------+



to be documented


.. das:function:: replace(arg0;arg1;arg2;arg3)

replace returns ::

 string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+string const+
+----+------------+
+arg2+string const+
+----+------------+
+arg3+ const      +
+----+------------+



to be documented


.. das:function:: reset_profiler(arg0)



function arguments are

+----+------+
+arg0+ const+
+----+------+



to be documented


.. das:function:: resize(arg0;arg1)



function arguments are

+----+----------------------+
+arg0+$::das_string implicit+
+----+----------------------+
+arg1+int const             +
+----+----------------------+



to be documented


.. das:function:: reverse(arg0;arg1)

reverse returns ::

 string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+ const      +
+----+------------+



to be documented


.. das:function:: rotate(arg0;arg1)

rotate returns ::

 float3



function arguments are

+----+--------------------------+
+arg0+$::float3x4 const implicit+
+----+--------------------------+
+arg1+float3 const              +
+----+--------------------------+



to be documented


.. das:function:: set(arg0;arg1)



function arguments are

+----+----------------------+
+arg0+$::das_string implicit+
+----+----------------------+
+arg1+string const          +
+----+----------------------+



to be documented


.. das:function:: set_variant_index(arg0;arg1)



.. warning::

 This is unsafe operation.

function arguments are

+----+------------------+
+arg0+variant<> implicit+
+----+------------------+
+arg1+int const         +
+----+------------------+



to be documented


.. das:function:: slice(arg0;arg1;arg2)

slice returns ::

 string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+
+arg2+ const      +
+----+------------+



to be documented


.. das:function:: slice(arg0;arg1;arg2;arg3)

slice returns ::

 string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+
+arg2+int const   +
+----+------------+
+arg3+ const      +
+----+------------+



to be documented


.. das:function:: smart_ptr_clone(arg0;arg1)



function arguments are

+----+-------------------------+
+arg0+smart_ptr<void>& implicit+
+----+-------------------------+
+arg1+void? const implicit     +
+----+-------------------------+



to be documented


.. das:function:: smart_ptr_clone(arg0;arg1)



function arguments are

+----+------------------------------+
+arg0+smart_ptr<void>& implicit     +
+----+------------------------------+
+arg1+smart_ptr<void> const implicit+
+----+------------------------------+



to be documented


.. das:function:: smart_ptr_use_count(arg0)

smart_ptr_use_count returns ::

 uint



function arguments are

+----+------------------------------+
+arg0+smart_ptr<void> const implicit+
+----+------------------------------+



to be documented


.. das:function:: stackwalk(arg0;arg1)



function arguments are

+----+------+
+arg0+ const+
+----+------+
+arg1+ const+
+----+------+



to be documented


.. das:function:: starts_with(arg0;arg1;arg2)

starts_with returns ::

 bool



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+string const+
+----+------------+
+arg2+ const      +
+----+------------+



to be documented


.. das:function:: string_heap_bytes_allocated(arg0)

string_heap_bytes_allocated returns ::

 uint



function arguments are

+----+------+
+arg0+ const+
+----+------+



to be documented


.. das:function:: string_heap_collect(arg0;arg1)



.. warning::

 This is unsafe operation.

function arguments are

+----+------+
+arg0+ const+
+----+------+
+arg1+ const+
+----+------+



to be documented


.. das:function:: string_heap_depth(arg0)

string_heap_depth returns ::

 int



function arguments are

+----+------+
+arg0+ const+
+----+------+



to be documented


.. das:function:: string_heap_high_watermark(arg0)

string_heap_high_watermark returns ::

 uint



function arguments are

+----+------+
+arg0+ const+
+----+------+



to be documented


.. das:function:: string_heap_report(arg0)



function arguments are

+----+------+
+arg0+ const+
+----+------+



to be documented


.. das:function:: strip(arg0;arg1)

strip returns ::

 string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+ const      +
+----+------------+



to be documented


.. das:function:: strip_left(arg0;arg1)

strip_left returns ::

 string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+ const      +
+----+------------+



to be documented


.. das:function:: strip_right(arg0;arg1)

strip_right returns ::

 string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+ const      +
+----+------------+



to be documented


.. das:function:: terminate(arg0)



function arguments are

+----+------+
+arg0+ const+
+----+------+



to be documented


.. das:function:: to_char(arg0;arg1)

to_char returns ::

 string



function arguments are

+----+---------+
+arg0+int const+
+----+---------+
+arg1+ const   +
+----+---------+



to be documented


.. das:function:: to_float(arg0)

to_float returns ::

 float



function arguments are

+----+------------+
+arg0+string const+
+----+------------+



to be documented


.. das:function:: to_int(arg0)

to_int returns ::

 int



function arguments are

+----+------------+
+arg0+string const+
+----+------------+



to be documented


.. das:function:: to_lower(arg0;arg1)

to_lower returns ::

 string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+ const      +
+----+------------+



to be documented


.. das:function:: to_lower_in_place(arg0)

to_lower_in_place returns ::

 string



.. warning::

 This is unsafe operation.

function arguments are

+----+------------+
+arg0+string const+
+----+------------+



to be documented


.. das:function:: to_upper(arg0;arg1)

to_upper returns ::

 string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+ const      +
+----+------------+



to be documented


.. das:function:: to_upper_in_place(arg0)

to_upper_in_place returns ::

 string



.. warning::

 This is unsafe operation.

function arguments are

+----+------------+
+arg0+string const+
+----+------------+



to be documented


.. das:function:: translation(arg0)

translation returns ::

 $::float4x4



function arguments are

+----+------------+
+arg0+float3 const+
+----+------------+



to be documented


.. das:function:: transpose(arg0)

transpose returns ::

 $::float4x4



function arguments are

+----+--------------------------+
+arg0+$::float4x4 const implicit+
+----+--------------------------+



to be documented


.. das:function:: unescape(arg0;arg1)

unescape returns ::

 string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+ const      +
+----+------------+



to be documented


.. das:function:: variant_index(arg0)

variant_index returns ::

 int



function arguments are

+----+------------------------+
+arg0+variant<> const implicit+
+----+------------------------+



to be documented


.. das:function:: write(arg0;arg1)



function arguments are

+----+----------------------+
+arg0+$::StringBuilderWriter+
+----+----------------------+
+arg1+any                   +
+----+----------------------+



to be documented


.. das:function:: write_char(arg0;arg1)



function arguments are

+----+-------------------------------+
+arg0+$::StringBuilderWriter implicit+
+----+-------------------------------+
+arg1+int const                      +
+----+-------------------------------+



to be documented


.. das:function:: write_chars(arg0;arg1;arg2)



function arguments are

+----+-------------------------------+
+arg0+$::StringBuilderWriter implicit+
+----+-------------------------------+
+arg1+int const                      +
+----+-------------------------------+
+arg2+int const                      +
+----+-------------------------------+



to be documented


.. das:function:: write_escape_string(arg0;arg1)



function arguments are

+----+-------------------------------+
+arg0+$::StringBuilderWriter implicit+
+----+-------------------------------+
+arg1+string const                   +
+----+-------------------------------+



to be documented



