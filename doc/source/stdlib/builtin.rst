
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

.. das:function:: binary_load(obj:auto; data:array<uint8> const)

binary_load returns auto



function arguments are

+----+------------------+
+obj +auto              +
+----+------------------+
+data+array<uint8> const+
+----+------------------+



to be documented


.. das:function:: binary_save(obj:auto const; subexpr:block<(data:array<uint8> const):void> const)

binary_save returns auto



function arguments are

+-------+-------------------------------------------+
+obj    +auto const                                 +
+-------+-------------------------------------------+
+subexpr+block<(data:array<uint8> const):void> const+
+-------+-------------------------------------------+



to be documented


.. das:function:: clone(a:array<auto(TT)>; b:array<TT> const)

clone returns auto



function arguments are

+-+---------------+
+a+array<auto(TT)>+
+-+---------------+
+b+array<TT> const+
+-+---------------+



to be documented


.. das:function:: clone(a:array<auto(TT)>; b:array<TT> const#)

clone returns auto



function arguments are

+-+----------------+
+a+array<auto(TT)> +
+-+----------------+
+b+array<TT> const#+
+-+----------------+



to be documented


.. das:function:: clone(a:table<auto(KT);auto(VT)>; b:table<KT;VT> const)

clone returns auto



function arguments are

+-+------------------------+
+a+table<auto(KT);auto(VT)>+
+-+------------------------+
+b+table<KT;VT> const      +
+-+------------------------+



to be documented


.. das:function:: clone(a:table<auto(KT);auto(VT)>; b:table<KT;VT> const#)

clone returns auto



function arguments are

+-+------------------------+
+a+table<auto(KT);auto(VT)>+
+-+------------------------+
+b+table<KT;VT> const#     +
+-+------------------------+



to be documented


.. das:function:: clone_dim(a:auto; b:auto const)

clone_dim returns auto



function arguments are

+-+----------+
+a+auto      +
+-+----------+
+b+auto const+
+-+----------+



to be documented


.. das:function:: clone_dim(a:auto; b:auto const#)

clone_dim returns auto



function arguments are

+-+-----------+
+a+auto       +
+-+-----------+
+b+auto const#+
+-+-----------+



to be documented


.. das:function:: clone_to_move(clone_src:auto(TT) const)

clone_to_move returns TT



function arguments are

+---------+--------------+
+clone_src+auto(TT) const+
+---------+--------------+



to be documented


.. das:function:: each(a:array<auto(TT)> const)

each returns iterator<TT&>



.. warning::

 This is unsafe operation.

function arguments are

+-+---------------------+
+a+array<auto(TT)> const+
+-+---------------------+



to be documented


.. das:function:: each(a:auto(TT) const[-1])

each returns iterator<TT&>



.. warning::

 This is unsafe operation.

function arguments are

+-+------------------+
+a+auto(TT) const[-1]+
+-+------------------+



to be documented


.. das:function:: each(lam:lambda<(arg:auto(argT) -const):bool> const)

each returns iterator<argT -&>



function arguments are

+---+------------------------------------------+
+lam+lambda<(arg:auto(argT) -const):bool> const+
+---+------------------------------------------+



to be documented


.. das:function:: each(rng:range const)

each returns iterator<int>



function arguments are

+---+-----------+
+rng+range const+
+---+-----------+



to be documented


.. das:function:: each(str:string const)

each returns iterator<int>



.. warning::

 This is unsafe operation.

function arguments are

+---+------------+
+str+string const+
+---+------------+



to be documented


.. das:function:: each_enum(tt:auto(TT) const)

each_enum returns iterator<TT -const -&>



function arguments are

+--+--------------+
+tt+auto(TT) const+
+--+--------------+



to be documented


.. das:function:: each_ref(lam:lambda<(arg:auto(argT)? -const):bool> const)

each_ref returns iterator<argT&>



function arguments are

+---+-------------------------------------------+
+lam+lambda<(arg:auto(argT)? -const):bool> const+
+---+-------------------------------------------+



to be documented


.. das:function:: emplace(Arr:array<auto(numT)>; value:numT; at:int const)

emplace returns auto



function arguments are

+-----+-----------------+
+Arr  +array<auto(numT)>+
+-----+-----------------+
+value+numT             +
+-----+-----------------+
+at   +int const        +
+-----+-----------------+



to be documented


.. das:function:: erase(Arr:array<auto(numT)>; at:int const)

erase returns auto



function arguments are

+---+-----------------+
+Arr+array<auto(numT)>+
+---+-----------------+
+at +int const        +
+---+-----------------+



to be documented


.. das:function:: erase(Tab:table<auto(keyT);auto(valT)>; at:keyT const)

erase returns bool



function arguments are

+---+----------------------------+
+Tab+table<auto(keyT);auto(valT)>+
+---+----------------------------+
+at +keyT const                  +
+---+----------------------------+



to be documented


.. das:function:: finalize(a:array<auto(TT)>)

finalize returns auto



function arguments are

+-+---------------+
+a+array<auto(TT)>+
+-+---------------+



to be documented


.. das:function:: finalize(a:table<auto(TK);auto(TV)>)

finalize returns auto



function arguments are

+-+------------------------+
+a+table<auto(TK);auto(TV)>+
+-+------------------------+



to be documented


.. das:function:: finalize_dim(a:auto(TT)[-1])

finalize_dim returns auto



function arguments are

+-+------------+
+a+auto(TT)[-1]+
+-+------------+



to be documented


.. das:function:: find(Tab:table<auto(keyT);auto(valT)> const; at:keyT const)

find returns valT const?



.. warning::

 This is unsafe operation.

function arguments are

+---+----------------------------------+
+Tab+table<auto(keyT);auto(valT)> const+
+---+----------------------------------+
+at +keyT const                        +
+---+----------------------------------+



to be documented


.. das:function:: find(Tab:table<auto(keyT);auto(valT)> const; at:keyT const; blk:block<(p:valT? const):void> const)

find returns auto



function arguments are

+---+----------------------------------+
+Tab+table<auto(keyT);auto(valT)> const+
+---+----------------------------------+
+at +keyT const                        +
+---+----------------------------------+
+blk+block<(p:valT? const):void> const +
+---+----------------------------------+



to be documented


.. das:function:: find(Tab:table<auto(keyT);auto(valT)> const#; at:keyT const)

find returns valT const?#



function arguments are

+---+-----------------------------------+
+Tab+table<auto(keyT);auto(valT)> const#+
+---+-----------------------------------+
+at +keyT const                         +
+---+-----------------------------------+



to be documented


.. das:function:: find_for_edit(Tab:table<auto(keyT);auto(valT)>; at:keyT const)

find_for_edit returns valT?



.. warning::

 This is unsafe operation.

function arguments are

+---+----------------------------+
+Tab+table<auto(keyT);auto(valT)>+
+---+----------------------------+
+at +keyT const                  +
+---+----------------------------+



to be documented


.. das:function:: find_for_edit(Tab:table<auto(keyT);auto(valT)>; at:keyT const; blk:block<(p:valT? -const):void> const)

find_for_edit returns auto



function arguments are

+---+----------------------------------+
+Tab+table<auto(keyT);auto(valT)>      +
+---+----------------------------------+
+at +keyT const                        +
+---+----------------------------------+
+blk+block<(p:valT? -const):void> const+
+---+----------------------------------+



to be documented


.. das:function:: find_for_edit_if_exists(Tab:table<auto(keyT);auto(valT)>; at:keyT const; blk:block<(p:valT? -const):void> const)

find_for_edit_if_exists returns auto



function arguments are

+---+----------------------------------+
+Tab+table<auto(keyT);auto(valT)>      +
+---+----------------------------------+
+at +keyT const                        +
+---+----------------------------------+
+blk+block<(p:valT? -const):void> const+
+---+----------------------------------+



to be documented


.. das:function:: find_if_exists(Tab:table<auto(keyT);auto(valT)> const; at:keyT const; blk:block<(p:valT? const):void> const)

find_if_exists returns auto



function arguments are

+---+----------------------------------+
+Tab+table<auto(keyT);auto(valT)> const+
+---+----------------------------------+
+at +keyT const                        +
+---+----------------------------------+
+blk+block<(p:valT? const):void> const +
+---+----------------------------------+



to be documented


.. das:function:: find_index(arr:array<auto(TT)> const implicit; key:TT const)

find_index returns auto



function arguments are

+---+------------------------------+
+arr+array<auto(TT)> const implicit+
+---+------------------------------+
+key+TT const                      +
+---+------------------------------+



to be documented


.. das:function:: find_index(arr:auto(TT) const[-1] implicit; key:TT const)

find_index returns auto



function arguments are

+---+---------------------------+
+arr+auto(TT) const[-1] implicit+
+---+---------------------------+
+key+TT const                   +
+---+---------------------------+



to be documented


.. das:function:: find_index_if(arr:array<auto(TT)> const implicit; blk:block<(key:TT const):bool> const)

find_index_if returns auto



function arguments are

+---+--------------------------------+
+arr+array<auto(TT)> const implicit  +
+---+--------------------------------+
+blk+block<(key:TT const):bool> const+
+---+--------------------------------+



to be documented


.. das:function:: find_index_if(arr:auto(TT) const[-1] implicit; blk:block<(key:TT const):bool> const)

find_index_if returns auto



function arguments are

+---+--------------------------------+
+arr+auto(TT) const[-1] implicit     +
+---+--------------------------------+
+blk+block<(key:TT const):bool> const+
+---+--------------------------------+



to be documented


.. das:function:: get_ptr(src:smart_ptr<auto(TT)> const)

get_ptr returns TT?



function arguments are

+---+-------------------------+
+src+smart_ptr<auto(TT)> const+
+---+-------------------------+



to be documented


.. das:function:: has_value(a:auto const; key:auto const)

has_value returns auto



function arguments are

+---+----------+
+a  +auto const+
+---+----------+
+key+auto const+
+---+----------+



to be documented


.. das:function:: intptr(p:void? const)

intptr returns uint64



function arguments are

+-+-----------+
+p+void? const+
+-+-----------+



to be documented


.. das:function:: key_exists(Tab:table<auto(keyT);auto(valT)> const; at:keyT const)

key_exists returns bool



function arguments are

+---+----------------------------------+
+Tab+table<auto(keyT);auto(valT)> const+
+---+----------------------------------+
+at +keyT const                        +
+---+----------------------------------+



to be documented


.. das:function:: keys(a:table<auto(keyT);auto(valT)> const)

keys returns iterator<keyT const&>



function arguments are

+-+----------------------------------+
+a+table<auto(keyT);auto(valT)> const+
+-+----------------------------------+



to be documented


.. das:function:: length(a:auto const[-1])

length returns int



function arguments are

+-+--------------+
+a+auto const[-1]+
+-+--------------+



to be documented


.. das:function:: lock(Tab:table<auto(keyT);auto(valT)> const; blk:block<(t:table<keyT;valT> const#):void> const)

lock returns auto



function arguments are

+---+---------------------------------------------+
+Tab+table<auto(keyT);auto(valT)> const           +
+---+---------------------------------------------+
+blk+block<(t:table<keyT;valT> const#):void> const+
+---+---------------------------------------------+



to be documented


.. das:function:: lock(Tab:table<auto(keyT);auto(valT)> const#; blk:block<(t:table<keyT;valT> const#):void> const)

lock returns auto



function arguments are

+---+---------------------------------------------+
+Tab+table<auto(keyT);auto(valT)> const#          +
+---+---------------------------------------------+
+blk+block<(t:table<keyT;valT> const#):void> const+
+---+---------------------------------------------+



to be documented


.. das:function:: lock_forever(Tab:table<auto(keyT);auto(valT)>)

lock_forever returns table<keyT;valT>#



function arguments are

+---+----------------------------+
+Tab+table<auto(keyT);auto(valT)>+
+---+----------------------------+



to be documented


.. das:function:: make_clone(res:auto(TT) const)

make_clone returns TT



function arguments are

+---+--------------+
+res+auto(TT) const+
+---+--------------+



to be documented


.. das:function:: next(it:iterator<auto(TT)> const; value:TT&)

next returns bool



function arguments are

+-----+------------------------+
+it   +iterator<auto(TT)> const+
+-----+------------------------+
+value+TT&                     +
+-----+------------------------+



to be documented


.. das:function:: nothing(it:iterator<auto(TT)>)

nothing returns iterator<TT>



function arguments are

+--+------------------+
+it+iterator<auto(TT)>+
+--+------------------+



to be documented


.. das:function:: pop(Arr:array<auto(numT)>)

pop returns auto



function arguments are

+---+-----------------+
+Arr+array<auto(numT)>+
+---+-----------------+



to be documented


.. das:function:: push(Arr:array<auto(numT)>; value:numT const; at:int const)

push returns auto



function arguments are

+-----+-----------------+
+Arr  +array<auto(numT)>+
+-----+-----------------+
+value+numT const       +
+-----+-----------------+
+at   +int const        +
+-----+-----------------+



to be documented


.. das:function:: push_clone(Arr:array<auto(numT)>; value:numT const; at:int const)

push_clone returns auto



function arguments are

+-----+-----------------+
+Arr  +array<auto(numT)>+
+-----+-----------------+
+value+numT const       +
+-----+-----------------+
+at   +int const        +
+-----+-----------------+



to be documented


.. das:function:: reserve(Arr:array<auto(numT)>; newSize:int const)

reserve returns auto



function arguments are

+-------+-----------------+
+Arr    +array<auto(numT)>+
+-------+-----------------+
+newSize+int const        +
+-------+-----------------+



to be documented


.. das:function:: resize(Arr:array<auto(numT)>; newSize:int const)

resize returns auto



function arguments are

+-------+-----------------+
+Arr    +array<auto(numT)>+
+-------+-----------------+
+newSize+int const        +
+-------+-----------------+



to be documented


.. das:function:: sort(a:array<auto(TT)>)

sort returns auto



function arguments are

+-+---------------+
+a+array<auto(TT)>+
+-+---------------+



to be documented


.. das:function:: sort(a:array<auto(TT)>; cmp:block<(x:TT const;y:TT const):bool> const)

sort returns auto



function arguments are

+---+-----------------------------------------+
+a  +array<auto(TT)>                          +
+---+-----------------------------------------+
+cmp+block<(x:TT const;y:TT const):bool> const+
+---+-----------------------------------------+



to be documented


.. das:function:: sort(a:auto(TT)[-1])

sort returns auto



function arguments are

+-+------------+
+a+auto(TT)[-1]+
+-+------------+



to be documented


.. das:function:: sort(a:auto(TT)[-1]; cmp:block<(x:TT const;y:TT const):bool> const)

sort returns auto



function arguments are

+---+-----------------------------------------+
+a  +auto(TT)[-1]                             +
+---+-----------------------------------------+
+cmp+block<(x:TT const;y:TT const):bool> const+
+---+-----------------------------------------+



to be documented


.. das:function:: to_array(a:auto(TT) const[-1])

to_array returns array<TT -const>



function arguments are

+-+------------------+
+a+auto(TT) const[-1]+
+-+------------------+



to be documented


.. das:function:: to_array(it:iterator<auto(TT)> const)

to_array returns array<TT -const -&>



function arguments are

+--+------------------------+
+it+iterator<auto(TT)> const+
+--+------------------------+



to be documented


.. das:function:: to_array_move(a:auto(TT)[-1])

to_array_move returns array<TT -const>



function arguments are

+-+------------+
+a+auto(TT)[-1]+
+-+------------+



to be documented


.. das:function:: to_table(a:tuple<auto(keyT);auto(valT)> const[-1])

to_table returns table<keyT -const;valT>



function arguments are

+-+--------------------------------------+
+a+tuple<auto(keyT);auto(valT)> const[-1]+
+-+--------------------------------------+



to be documented


.. das:function:: to_table_move(a:tuple<auto(keyT);auto(valT)>[-1])

to_table_move returns table<keyT -const;valT>



function arguments are

+-+--------------------------------+
+a+tuple<auto(keyT);auto(valT)>[-1]+
+-+--------------------------------+



to be documented


.. das:function:: values(a:table<auto(keyT);auto(valT)> const!)

values returns iterator<valT const&>



function arguments are

+-+-----------------------------------+
+a+table<auto(keyT);auto(valT)> const!+
+-+-----------------------------------+



to be documented


.. das:function:: values(a:table<auto(keyT);auto(valT)>!)

values returns iterator<valT&>



function arguments are

+-+-----------------------------+
+a+table<auto(keyT);auto(valT)>!+
+-+-----------------------------+



to be documented


+++++++++
Functions
+++++++++

.. das:function:: append(arg0:das_string implicit; arg1:int const)



function arguments are

+----+-------------------+
+arg0+das_string implicit+
+----+-------------------+
+arg1+int const          +
+----+-------------------+



to be documented


.. das:function:: append(arg0:string const; arg1:int const)

append returns string



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


.. das:function:: as_string(arg0:array<uint8> const implicit)

as_string returns string



function arguments are

+----+---------------------------+
+arg0+array<uint8> const implicit+
+----+---------------------------+
+arg1+ const                     +
+----+---------------------------+



to be documented


.. das:function:: breakpoint




to be documented


.. das:function:: build_string(arg0:block<(StringBuilderWriter):void> const implicit)

build_string returns string



function arguments are

+----+------------------------------------------------+
+arg0+block<(StringBuilderWriter):void> const implicit+
+----+------------------------------------------------+
+arg1+ const                                          +
+----+------------------------------------------------+



to be documented


.. das:function:: builtin_strdup(arg0:any)



.. warning::

 This is unsafe operation.

function arguments are

+----+---+
+arg0+any+
+----+---+



to be documented


.. das:function:: builtin_string_split(arg0:string const; arg1:string const; arg2:block<> const implicit)



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


.. das:function:: builtin_string_split_by_char(arg0:string const; arg1:string const; arg2:block<> const implicit)



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


.. das:function:: capacity(arg0:array const implicit)

capacity returns int



function arguments are

+----+--------------------+
+arg0+array const implicit+
+----+--------------------+



to be documented


.. das:function:: capacity(arg0:table const implicit)

capacity returns int



function arguments are

+----+--------------------+
+arg0+table const implicit+
+----+--------------------+



to be documented


.. das:function:: character_at(arg0:string const; arg1:int const)

character_at returns int



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+
+arg2+ const      +
+----+------------+



to be documented


.. das:function:: character_uat(arg0:string const; arg1:int const)

character_uat returns int



.. warning::

 This is unsafe operation.

function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+



to be documented


.. das:function:: chop(arg0:string const; arg1:int const; arg2:int const)

chop returns string



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


.. das:function:: clear(arg0:array implicit)



function arguments are

+----+--------------+
+arg0+array implicit+
+----+--------------+
+arg1+ const        +
+----+--------------+



to be documented


.. das:function:: clear(arg0:table implicit)



function arguments are

+----+--------------+
+arg0+table implicit+
+----+--------------+
+arg1+ const        +
+----+--------------+



to be documented


.. das:function:: clone(a:das_string; b:string const)



function arguments are

+-+------------+
+a+das_string  +
+-+------------+
+b+string const+
+-+------------+



to be documented


.. das:function:: clone_string(arg0:string const)

clone_string returns string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+ const      +
+----+------------+



to be documented


.. das:function:: delete_string(arg0:string& implicit)



.. warning::

 This is unsafe operation.

function arguments are

+----+----------------+
+arg0+string& implicit+
+----+----------------+
+arg1+ const          +
+----+----------------+



to be documented


.. das:function:: dump_profile_info()



function arguments are

+----+------+
+arg0+ const+
+----+------+



to be documented


.. das:function:: empty(arg0:das_string const implicit)

empty returns bool



function arguments are

+----+-------------------------+
+arg0+das_string const implicit+
+----+-------------------------+



to be documented


.. das:function:: empty(arg0:iterator const implicit)

empty returns bool



function arguments are

+----+-----------------------+
+arg0+iterator const implicit+
+----+-----------------------+



to be documented


.. das:function:: empty(arg0:string const)

empty returns bool



function arguments are

+----+------------+
+arg0+string const+
+----+------------+



to be documented


.. das:function:: ends_with(arg0:das_string const implicit; arg1:string const)

ends_with returns bool



function arguments are

+----+-------------------------+
+arg0+das_string const implicit+
+----+-------------------------+
+arg1+string const             +
+----+-------------------------+
+arg2+ const                   +
+----+-------------------------+



to be documented


.. das:function:: ends_with(arg0:string const; arg1:string const)

ends_with returns bool



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+string const+
+----+------------+
+arg2+ const      +
+----+------------+



to be documented


.. das:function:: escape(arg0:string const)

escape returns string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+ const      +
+----+------------+



to be documented


.. das:function:: find(arg0:string const; arg1:string const)

find returns int



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+string const+
+----+------------+



to be documented


.. das:function:: find(arg0:string const; arg1:string const; arg2:int const)

find returns int



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


.. das:function:: find_first_of(arg0:string const; arg1:int const)

find_first_of returns int



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+
+arg2+ const      +
+----+------------+



to be documented


.. das:function:: find_first_of(arg0:string const; arg1:string const)

find_first_of returns int



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

float3x4 returns float3x4




to be documented


.. das:function:: float4x4

float4x4 returns float4x4




to be documented


.. das:function:: format(arg0:StringBuilderWriter implicit; arg1:string const; arg2:double const)



function arguments are

+----+----------------------------+
+arg0+StringBuilderWriter implicit+
+----+----------------------------+
+arg1+string const                +
+----+----------------------------+
+arg2+double const                +
+----+----------------------------+



to be documented


.. das:function:: format(arg0:StringBuilderWriter implicit; arg1:string const; arg2:float const)



function arguments are

+----+----------------------------+
+arg0+StringBuilderWriter implicit+
+----+----------------------------+
+arg1+string const                +
+----+----------------------------+
+arg2+float const                 +
+----+----------------------------+



to be documented


.. das:function:: format(arg0:StringBuilderWriter implicit; arg1:string const; arg2:int const)



function arguments are

+----+----------------------------+
+arg0+StringBuilderWriter implicit+
+----+----------------------------+
+arg1+string const                +
+----+----------------------------+
+arg2+int const                   +
+----+----------------------------+



to be documented


.. das:function:: format(arg0:StringBuilderWriter implicit; arg1:string const; arg2:int64 const)



function arguments are

+----+----------------------------+
+arg0+StringBuilderWriter implicit+
+----+----------------------------+
+arg1+string const                +
+----+----------------------------+
+arg2+int64 const                 +
+----+----------------------------+



to be documented


.. das:function:: format(arg0:StringBuilderWriter implicit; arg1:string const; arg2:uint const)



function arguments are

+----+----------------------------+
+arg0+StringBuilderWriter implicit+
+----+----------------------------+
+arg1+string const                +
+----+----------------------------+
+arg2+uint const                  +
+----+----------------------------+



to be documented


.. das:function:: format(arg0:StringBuilderWriter implicit; arg1:string const; arg2:uint64 const)



function arguments are

+----+----------------------------+
+arg0+StringBuilderWriter implicit+
+----+----------------------------+
+arg1+string const                +
+----+----------------------------+
+arg2+uint64 const                +
+----+----------------------------+



to be documented


.. das:function:: format(arg0:string const; arg1:double const)

format returns string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+double const+
+----+------------+
+arg2+ const      +
+----+------------+



to be documented


.. das:function:: format(arg0:string const; arg1:float const)

format returns string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+float const +
+----+------------+
+arg2+ const      +
+----+------------+



to be documented


.. das:function:: format(arg0:string const; arg1:int const)

format returns string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+
+arg2+ const      +
+----+------------+



to be documented


.. das:function:: format(arg0:string const; arg1:int64 const)

format returns string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int64 const +
+----+------------+
+arg2+ const      +
+----+------------+



to be documented


.. das:function:: format(arg0:string const; arg1:uint const)

format returns string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+uint const  +
+----+------------+
+arg2+ const      +
+----+------------+



to be documented


.. das:function:: format(arg0:string const; arg1:uint64 const)

format returns string



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


.. das:function:: gc0_restore_ptr(arg0:string const)

gc0_restore_ptr returns void?



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+ const      +
+----+------------+



to be documented


.. das:function:: gc0_restore_smart_ptr(arg0:string const)

gc0_restore_smart_ptr returns smart_ptr<void>



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+ const      +
+----+------------+



to be documented


.. das:function:: gc0_save_ptr(arg0:string const; arg1:void? const implicit)



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


.. das:function:: gc0_save_smart_ptr(arg0:string const; arg1:smart_ptr<void> const implicit)



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

get_clock returns clock




to be documented


.. das:function:: get_das_root()

get_das_root returns string



function arguments are

+----+------+
+arg0+ const+
+----+------+



to be documented


.. das:function:: hash(arg0:any)

hash returns uint



function arguments are

+----+---+
+arg0+any+
+----+---+



to be documented


.. das:function:: heap_bytes_allocated()

heap_bytes_allocated returns uint



function arguments are

+----+------+
+arg0+ const+
+----+------+



to be documented


.. das:function:: heap_depth()

heap_depth returns int



function arguments are

+----+------+
+arg0+ const+
+----+------+



to be documented


.. das:function:: heap_high_watermark()

heap_high_watermark returns uint



function arguments are

+----+------+
+arg0+ const+
+----+------+



to be documented


.. das:function:: heap_report()



function arguments are

+----+------+
+arg0+ const+
+----+------+



to be documented


.. das:function:: i_das_ptr_add(arg0:void? const implicit; arg1:int const; arg2:int const)

i_das_ptr_add returns void?



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


.. das:function:: i_das_ptr_dec(arg0:void?& implicit; arg1:int const)



.. warning::

 This is unsafe operation.

function arguments are

+----+---------------+
+arg0+void?& implicit+
+----+---------------+
+arg1+int const      +
+----+---------------+



to be documented


.. das:function:: i_das_ptr_diff(arg0:void? const implicit; arg1:void? const implicit; arg2:int const)

i_das_ptr_diff returns int64



function arguments are

+----+--------------------+
+arg0+void? const implicit+
+----+--------------------+
+arg1+void? const implicit+
+----+--------------------+
+arg2+int const           +
+----+--------------------+



to be documented


.. das:function:: i_das_ptr_inc(arg0:void?& implicit; arg1:int const)



.. warning::

 This is unsafe operation.

function arguments are

+----+---------------+
+arg0+void?& implicit+
+----+---------------+
+arg1+int const      +
+----+---------------+



to be documented


.. das:function:: i_das_ptr_set_add(arg0:void?& implicit; arg1:int const; arg2:int const)



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


.. das:function:: i_das_ptr_set_sub(arg0:void?& implicit; arg1:int const; arg2:int const)



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


.. das:function:: i_das_ptr_sub(arg0:void?& implicit; arg1:int const; arg2:int const)

i_das_ptr_sub returns void?



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


.. das:function:: identity(arg0:float3x4 implicit)



function arguments are

+----+-----------------+
+arg0+float3x4 implicit+
+----+-----------------+



to be documented


.. das:function:: identity(arg0:float4x4 implicit)



function arguments are

+----+-----------------+
+arg0+float4x4 implicit+
+----+-----------------+



to be documented


.. das:function:: inverse(arg0:float3x4 const implicit)

inverse returns float3x4



function arguments are

+----+-----------------------+
+arg0+float3x4 const implicit+
+----+-----------------------+



to be documented


.. das:function:: is_alpha(arg0:int const)

is_alpha returns bool



function arguments are

+----+---------+
+arg0+int const+
+----+---------+



to be documented


.. das:function:: is_char_in_set(arg0:int const; arg1:uint const[8] implicit)

is_char_in_set returns bool



function arguments are

+----+----------------------+
+arg0+int const             +
+----+----------------------+
+arg1+uint const[8] implicit+
+----+----------------------+



to be documented


.. das:function:: is_compiling()

is_compiling returns bool



function arguments are

+----+------+
+arg0+ const+
+----+------+



to be documented


.. das:function:: is_compiling_macros()

is_compiling_macros returns bool



function arguments are

+----+------+
+arg0+ const+
+----+------+



to be documented


.. das:function:: is_number(arg0:int const)

is_number returns bool



function arguments are

+----+---------+
+arg0+int const+
+----+---------+



to be documented


.. das:function:: is_white_space(arg0:int const)

is_white_space returns bool



function arguments are

+----+---------+
+arg0+int const+
+----+---------+



to be documented


.. das:function:: length(arg0:array const implicit)

length returns int



function arguments are

+----+--------------------+
+arg0+array const implicit+
+----+--------------------+



to be documented


.. das:function:: length(arg0:das_string implicit)

length returns int



function arguments are

+----+-------------------+
+arg0+das_string implicit+
+----+-------------------+



to be documented


.. das:function:: length(arg0:table const implicit)

length returns int



function arguments are

+----+--------------------+
+arg0+table const implicit+
+----+--------------------+



to be documented


.. das:function:: length(arg0:string const)

length returns int



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+ const      +
+----+------------+



to be documented


.. das:function:: memcmp(arg0:void? const implicit; arg1:void? const implicit; arg2:int const)

memcmp returns int



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


.. das:function:: panic(arg0:string const)



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+ const      +
+----+------------+



to be documented


.. das:function:: peek(arg0:das_string const implicit; arg1:block<(string const#):void> const implicit)



function arguments are

+----+------------------------------------------+
+arg0+das_string const implicit                 +
+----+------------------------------------------+
+arg1+block<(string const#):void> const implicit+
+----+------------------------------------------+
+arg2+ const                                    +
+----+------------------------------------------+



to be documented


.. das:function:: print(arg0:string const)



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+ const      +
+----+------------+



to be documented


.. das:function:: profile(arg0:int const; arg1:string const; arg2:block<> const implicit)

profile returns float



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


.. das:function:: repeat(arg0:string const; arg1:int const)

repeat returns string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+
+arg2+ const      +
+----+------------+



to be documented


.. das:function:: replace(arg0:string const; arg1:string const; arg2:string const)

replace returns string



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


.. das:function:: reset_profiler()



function arguments are

+----+------+
+arg0+ const+
+----+------+



to be documented


.. das:function:: resize(arg0:das_string implicit; arg1:int const)



function arguments are

+----+-------------------+
+arg0+das_string implicit+
+----+-------------------+
+arg1+int const          +
+----+-------------------+



to be documented


.. das:function:: reverse(arg0:string const)

reverse returns string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+ const      +
+----+------------+



to be documented


.. das:function:: rotate(arg0:float3x4 const implicit; arg1:float3 const)

rotate returns float3



function arguments are

+----+-----------------------+
+arg0+float3x4 const implicit+
+----+-----------------------+
+arg1+float3 const           +
+----+-----------------------+



to be documented


.. das:function:: set(arg0:das_string implicit; arg1:string const)



function arguments are

+----+-------------------+
+arg0+das_string implicit+
+----+-------------------+
+arg1+string const       +
+----+-------------------+



to be documented


.. das:function:: set_variant_index(arg0:variant<> implicit; arg1:int const)



.. warning::

 This is unsafe operation.

function arguments are

+----+------------------+
+arg0+variant<> implicit+
+----+------------------+
+arg1+int const         +
+----+------------------+



to be documented


.. das:function:: slice(arg0:string const; arg1:int const)

slice returns string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+int const   +
+----+------------+
+arg2+ const      +
+----+------------+



to be documented


.. das:function:: slice(arg0:string const; arg1:int const; arg2:int const)

slice returns string



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


.. das:function:: smart_ptr_clone(arg0:smart_ptr<void>& implicit; arg1:void? const implicit)



function arguments are

+----+-------------------------+
+arg0+smart_ptr<void>& implicit+
+----+-------------------------+
+arg1+void? const implicit     +
+----+-------------------------+



to be documented


.. das:function:: smart_ptr_clone(arg0:smart_ptr<void>& implicit; arg1:smart_ptr<void> const implicit)



function arguments are

+----+------------------------------+
+arg0+smart_ptr<void>& implicit     +
+----+------------------------------+
+arg1+smart_ptr<void> const implicit+
+----+------------------------------+



to be documented


.. das:function:: smart_ptr_use_count(arg0:smart_ptr<void> const implicit)

smart_ptr_use_count returns uint



function arguments are

+----+------------------------------+
+arg0+smart_ptr<void> const implicit+
+----+------------------------------+



to be documented


.. das:function:: stackwalk()



function arguments are

+----+------+
+arg0+ const+
+----+------+
+arg1+ const+
+----+------+



to be documented


.. das:function:: starts_with(arg0:string const; arg1:string const)

starts_with returns bool



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+string const+
+----+------------+
+arg2+ const      +
+----+------------+



to be documented


.. das:function:: string_heap_bytes_allocated()

string_heap_bytes_allocated returns uint



function arguments are

+----+------+
+arg0+ const+
+----+------+



to be documented


.. das:function:: string_heap_collect()



.. warning::

 This is unsafe operation.

function arguments are

+----+------+
+arg0+ const+
+----+------+
+arg1+ const+
+----+------+



to be documented


.. das:function:: string_heap_depth()

string_heap_depth returns int



function arguments are

+----+------+
+arg0+ const+
+----+------+



to be documented


.. das:function:: string_heap_high_watermark()

string_heap_high_watermark returns uint



function arguments are

+----+------+
+arg0+ const+
+----+------+



to be documented


.. das:function:: string_heap_report()



function arguments are

+----+------+
+arg0+ const+
+----+------+



to be documented


.. das:function:: strip(arg0:string const)

strip returns string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+ const      +
+----+------------+



to be documented


.. das:function:: strip_left(arg0:string const)

strip_left returns string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+ const      +
+----+------------+



to be documented


.. das:function:: strip_right(arg0:string const)

strip_right returns string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+ const      +
+----+------------+



to be documented


.. das:function:: terminate()



function arguments are

+----+------+
+arg0+ const+
+----+------+



to be documented


.. das:function:: to_char(arg0:int const)

to_char returns string



function arguments are

+----+---------+
+arg0+int const+
+----+---------+
+arg1+ const   +
+----+---------+



to be documented


.. das:function:: to_float(arg0:string const)

to_float returns float



function arguments are

+----+------------+
+arg0+string const+
+----+------------+



to be documented


.. das:function:: to_int(arg0:string const)

to_int returns int



function arguments are

+----+------------+
+arg0+string const+
+----+------------+



to be documented


.. das:function:: to_lower(arg0:string const)

to_lower returns string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+ const      +
+----+------------+



to be documented


.. das:function:: to_lower_in_place(arg0:string const)

to_lower_in_place returns string



.. warning::

 This is unsafe operation.

function arguments are

+----+------------+
+arg0+string const+
+----+------------+



to be documented


.. das:function:: to_upper(arg0:string const)

to_upper returns string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+ const      +
+----+------------+



to be documented


.. das:function:: to_upper_in_place(arg0:string const)

to_upper_in_place returns string



.. warning::

 This is unsafe operation.

function arguments are

+----+------------+
+arg0+string const+
+----+------------+



to be documented


.. das:function:: translation(arg0:float3 const)

translation returns float4x4



function arguments are

+----+------------+
+arg0+float3 const+
+----+------------+



to be documented


.. das:function:: transpose(arg0:float4x4 const implicit)

transpose returns float4x4



function arguments are

+----+-----------------------+
+arg0+float4x4 const implicit+
+----+-----------------------+



to be documented


.. das:function:: unescape(arg0:string const)

unescape returns string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+ const      +
+----+------------+



to be documented


.. das:function:: variant_index(arg0:variant<> const implicit)

variant_index returns int



function arguments are

+----+------------------------+
+arg0+variant<> const implicit+
+----+------------------------+



to be documented


.. das:function:: write(arg0:StringBuilderWriter; arg1:any)



function arguments are

+----+-------------------+
+arg0+StringBuilderWriter+
+----+-------------------+
+arg1+any                +
+----+-------------------+



to be documented


.. das:function:: write_char(arg0:StringBuilderWriter implicit; arg1:int const)



function arguments are

+----+----------------------------+
+arg0+StringBuilderWriter implicit+
+----+----------------------------+
+arg1+int const                   +
+----+----------------------------+



to be documented


.. das:function:: write_chars(arg0:StringBuilderWriter implicit; arg1:int const; arg2:int const)



function arguments are

+----+----------------------------+
+arg0+StringBuilderWriter implicit+
+----+----------------------------+
+arg1+int const                   +
+----+----------------------------+
+arg2+int const                   +
+----+----------------------------+



to be documented


.. das:function:: write_escape_string(arg0:StringBuilderWriter implicit; arg1:string const)



function arguments are

+----+----------------------------+
+arg0+StringBuilderWriter implicit+
+----+----------------------------+
+arg1+string const                +
+----+----------------------------+



to be documented



