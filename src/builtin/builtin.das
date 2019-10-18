options indenting=4
options remove_unused_symbols=false

def resize(var Arr:array<auto(numT)>;newSize:int)
    __builtin_array_resize(Arr,newSize,typeinfo(sizeof type numT))

def reserve(var Arr:array<auto(numT)>;newSize:int)
    __builtin_array_reserve(Arr,newSize,typeinfo(sizeof type numT))

def push(var Arr:array<auto(numT)>;value:numT const;at:int=-1)
    Arr[__builtin_array_push(Arr,at,typeinfo(sizeof type numT))] = value

def emplace(var Arr:array<auto(numT)>;value:numT;at:int=-1)
    Arr[__builtin_array_push(Arr,at,typeinfo(sizeof type numT))] <- value

def erase(var Arr:array<auto(numT)>;at:int)
    __builtin_array_erase(Arr,at,typeinfo(sizeof type numT))

def length(a:auto[]):int
    return typeinfo(dim a)

// table

def find(Tab:table<auto(keyT);auto(valT)>;at:keyT;blk:block<(p:valT?):void>)
    __builtin_table_lock(Tab)
    var val = __builtin_table_find(Tab,at)
    invoke(blk,val)
    __builtin_table_unlock(Tab)

[unsafe_operation,unsafe]
def find(Tab:table<auto(keyT);auto(valT)>;at:keyT):valT const?
    return reinterpret<valT const?>(__builtin_table_find(Tab,at))

def find_for_edit(var Tab:table<auto(keyT);auto(valT)>;at:keyT;blk:block<(var p:valT?):void>)
    __builtin_table_lock(Tab)
    var val = __builtin_table_find(Tab,at)
    invoke(blk,val)
    __builtin_table_unlock(Tab)

[unsafe_operation]
def find_for_edit(var Tab:table<auto(keyT);auto(valT)>;at:keyT):valT?
    return __builtin_table_find(Tab,at)

def erase(var Tab:table<auto(keyT);auto(valT)>;at:keyT):bool
    return __builtin_table_erase(Tab,at)

def key_exists(Tab:table<auto(keyT);auto(valT)>;at:keyT):bool
    return __builtin_table_key_exists(Tab,at)

def binary_save(obj; subexpr:block<(data:array<uint8>):void>)
    static_assert(typeinfo(is_ref_type obj),"can only serialize ref types")
    _builtin_binary_save(obj,subexpr)

def binary_load(var obj; data:array<uint8>)
    static_assert(typeinfo(is_ref_type obj),"can only serialize ref types")
    _builtin_binary_load(obj,data)

def clone_to_move(clone_src:auto(TT)) : TT delete const
    var clone_dest : TT
    clone_dest := clone_src
    return <- clone_dest

def clone_dim(var a;b)
	if typeinfo(is_dim a) & typeinfo(is_dim b) & typeinfo(dim a)==typeinfo(dim b)
		for aV,bV in a,b
			aV := bV
	else
		static_assert(false,"can't clone this array")

def clone(var a:array<auto(TT)>;b:array<TT>)
    resize(a,length(b))
    for aV,bV in a,b
        aV := bV

def clone(var a:table<auto(KT);auto(VT)>;b:table<KT;VT>)
    clear(a)
    for k,v in keys(b),values(b)
        a[k] := v

def clone(var a:das_string;b:string)
    set(a,b)

def make_clone(res:auto(TT)) : TT delete const
	if typeinfo(can_copy type TT)
		return res
	else
		var temp : TT
		temp := res
		return <- temp

def keys(a:table<auto(keyT);auto(valT)>) : iterator<keyT & const>
    return __builtin_table_keys(a,typeinfo(sizeof type keyT))

def values(a:table<auto(keyT);auto(valT)>) : iterator<valT &>
    return __builtin_table_values(a,typeinfo(sizeof type valT))

// functional

[generic]
def each ( rng : range ) : iterator<int>
    return _builtin_make_range_iterator(rng)

[unsafe,unsafe_operation]
def each ( a : auto(TT)[] ) : iterator<TT&>
    var parr : void? = reinterpret<void?>(addr(a[0]))
    return _builtin_make_fixed_array_iterator(parr,typeinfo(dim a),typeinfo(sizeof type TT))

[unsafe_operation]
def each ( a : array<auto(TT)> ) : iterator<TT&>
    return _builtin_make_good_array_iterator(a, typeinfo(sizeof type TT))

def each ( lam : lambda<(var arg:auto(argT)):bool> ) : iterator<argT delete &>
    return _builtin_make_lambda_iterator(lam)

[unsafe]
def to_array ( it : iterator<auto(TT)> ) : array<TT delete const &>
    var arr : array<TT delete const &>
    if typeinfo(is_ref type TT)
        var rkey : TT delete const &?
        var pkey : void? = reinterpret<void?>(addr(rkey))
        var loop = _builtin_iterator_first(it,pkey)
        while loop
            push(arr,deref(rkey))
            loop = _builtin_iterator_next(it,pkey)
        _builtin_iterator_close(it,pkey)
    else
        var rkey : TT
        var pkey : void? = reinterpret<void?>(addr(rkey))
        var loop = _builtin_iterator_first(it,pkey)
        while loop
            push(arr,rkey)
            loop = _builtin_iterator_next(it,pkey)
        _builtin_iterator_close(it,pkey)
    return <- arr

[unsafe]
def to_array(a:auto(TT)[]) : array<TT delete const>
    var arr : array<TT delete const>
    resize(arr,length(a))
    reinterpret<TT delete const[typeinfo(dim a)]>(addr(arr[0])) := a
    return <- arr

[unsafe]
def to_array_move(var a:auto(TT)[]) : array<TT delete const>
    var arr : array<TT delete const>
    if typeinfo(can_copy a)
        resize(arr,length(a))
        reinterpret<TT delete const[typeinfo(dim a)]>(addr(arr[0])) = a
    elif typeinfo(can_move a)
        resize(arr,length(a))
        reinterpret<TT delete const[typeinfo(dim a)]>(addr(arr[0])) <- a
    else
        static_assert(false,"this array can't be copied or moved")
    return <- arr

def to_table(a:tuple<auto(keyT);auto(valT)>[]) : table<keyT delete const;valT>
    var tab : table<keyT delete const;valT>
    for x in a
        tab[x._0] := x._1
    return <- tab

def to_table_move(var a:tuple<auto(keyT);auto(valT)>[]) : table<keyT delete const;valT>
    var tab : table<keyT delete const;valT>
    if typeinfo(can_copy type valT)
		for x in a
			tab[x._0] = x._1
	elif typeinfo(can_move type valT)
		for x in a
			tab[x._0] <- x._1
    else
        static_assert(false,"this array can't be copied or moved")
    return <- tab


