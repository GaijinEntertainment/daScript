options removeUnusedSymbols=false

def resize(var Arr:array<auto(numT)>;newSize:int)
    __builtin_array_resize(Arr,newSize,typeinfo(sizeof type numT))

def reserve(var Arr:array<auto(numT)>;newSize:int)
    __builtin_array_reserve(Arr,newSize,typeinfo(sizeof type numT))

def push(var Arr:array<auto(numT)>;value:numT;at:int=-1)
    Arr[__builtin_array_push(Arr,at,typeinfo(sizeof type numT))] = value

def emplace(var Arr:array<auto(numT)>;value:numT;at:int=-1)
    Arr[__builtin_array_push(Arr,at,typeinfo(sizeof type numT))] <- value

def erase(var Arr:array<auto(numT)>;at:int)
    __builtin_array_erase(Arr,at,typeinfo(sizeof type numT))

def length(a:auto[]):int
    return typeinfo(dim a)

// table

def find(var Tab:table<auto(keyT),auto(valT)>;at:keyT;blk:block<(var p:valT?):void>)
    __builtin_table_lock(Tab)
    var val = __builtin_table_find(Tab,at)
    invoke(blk,val)
    __builtin_table_unlock(Tab)

def erase(var Tab:table<auto(keyT),auto(valT)>;at:keyT):bool
    return __builtin_table_erase(Tab,at)

def key_exists(var Tab:table<auto(keyT),auto(valT)>;at:keyT):bool
    return __builtin_table_key_exists(Tab,at)

def binary_save(obj; subexpr:block<(data:string):void>)
    static_assert(typeinfo(is_ref_type obj),"can only serialize ref types")
    _builtin_binary_save(obj,subexpr)

def binary_load(var obj; data:string)
    static_assert(typeinfo(is_ref_type obj),"can only serialize ref types")
    _builtin_binary_load(obj,data)

def clone(var a:array<auto(TT)>;b:array<TT>)
    resize(a,length(b))
    for aV,bV in a,b
        aV := bV

def clone(var a:table<auto(KT),auto(VT)>;b:table<KT,VT>)
    clear(a)
    for k,v in keys(b),values(b)
        a[k] := v

def clone(var a:das_string;b:string)
    set(a,b)

def keys(a:table<auto(keyT),auto(valT)>) : iterator<keyT & const>
    return __builtin_table_keys(a,typeinfo(sizeof type keyT))

def values(a:table<auto(keyT),auto(valT)>) : iterator<valT &>
    return __builtin_table_values(a,typeinfo(sizeof type valT))

// functional

[generic]
def each ( rng : range ) : iterator<int>
    return _builtin_make_range_iterator(rng)

[unsafe,unsafeoperation]
def each ( a : auto(TT)[] ) : iterator<TT&>
    var parr : void? = reinterpret<void?>(addr(a[0]))
    return _builtin_make_fixed_array_iterator(parr,typeinfo(dim a),typeinfo(sizeof type TT))

[unsafeoperation]
def each ( a : array<auto(TT)> ) : iterator<TT&>
    return _builtin_make_good_array_iterator(a, typeinfo(sizeof type TT))

[unsafe]
def toarray ( it : iterator<auto(TT)> ) : array<TT delete const &>
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
def toarray(a:auto(TT)[]) : array<TT delete const>
    var arr : array<TT delete const>
    if typeinfo(can_copy a)
        resize(arr,length(a))
        reinterpret<TT delete const[typeinfo(dim a)]>(addr(arr[0])) = reinterpret<TT[typeinfo(dim a)]>(addr(a[0]))
        // for i in range(0,length(a))
        //    arr[i] = a[i]
    else
        static_assert(false,"can't copy this array")
    return <- arr

def totable(a:tuple<auto(keyT);auto(valT)>[]) : table<keyT delete const,valT>
    var tab : table<keyT delete const,valT>
    for x in a
        tab[x._0] = x._1
    return <- tab



