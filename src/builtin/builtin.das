options indenting=4
options remove_unused_symbols=false

let
    print_flags_debugger = (
            print_flags escapeString
        |   print_flags namesAndDimensions
        |   print_flags humanReadable
        |   print_flags typeQualifiers
        |   print_flags refAddresses
    )

[generic]
def intptr ( p : void? ) : uint64
	static_if typeinfo(sizeof p)==4
        unsafe
		    return uint64(reinterpret<uint> p)
	else
        unsafe
		    return reinterpret<uint64> p

[generic]
def intptr ( p : smart_ptr<auto> ) : uint64
	static_if typeinfo(sizeof p)==4
        unsafe
		    return uint64(reinterpret<uint> p)
	else
        unsafe
		    return reinterpret<uint64> p

def _move_with_lockcheck ( var a:auto(valA)&; var b:auto(valB)& )
    static_if typeinfo(need_lock_check type<valA>)
        _builtin_verify_locks(a)
    static_if typeinfo(need_lock_check type<valB>)
        _builtin_verify_locks(b)
    a <- b

def _return_with_lockcheck ( var a:auto(valT)& ) : valT&
    static_if typeinfo(need_lock_check type<valT>)
        _builtin_verify_locks(a)
    unsafe
        return a

def resize(var Arr:array<auto(numT)>;newSize:int)
    static_if typeinfo(need_lock_check type<numT>)
        _builtin_verify_locks(Arr)
    __builtin_array_resize(Arr,newSize,typeinfo(sizeof Arr[0]))

def reserve(var Arr:array<auto(numT)>;newSize:int)
    static_if typeinfo(need_lock_check type<numT>)
        _builtin_verify_locks(Arr)
    __builtin_array_reserve(Arr,newSize,typeinfo(sizeof Arr[0]))

def pop(var Arr:array<auto(numT)>)
    resize(Arr, length(Arr)-1)  // resize will throw if negative

def push(var Arr:array<auto(numT)>;value:numT const;at:int)
    static_if typeinfo(can_copy value)
        static_if typeinfo(need_lock_check type<numT>)
            _builtin_verify_locks(Arr)
        Arr[__builtin_array_push(Arr,at,typeinfo(sizeof Arr[0]))] = value
    else
        concept_assert(false,"can't push value, which can't be copied")

def push(var Arr:array<auto(numT)>;value:numT const)
    static_if typeinfo(can_copy value)
        static_if typeinfo(need_lock_check type<numT>)
            _builtin_verify_locks(Arr)
        Arr[__builtin_array_push_back(Arr,typeinfo(sizeof Arr[0]))] = value
    else
        concept_assert(false,"can't push value, which can't be copied")

def emplace(var Arr:array<auto(numT)>;var value:numT;at:int)
    static_if typeinfo(can_move value)
        static_if typeinfo(need_lock_check type<numT>)
            _builtin_verify_locks(Arr)
        Arr[__builtin_array_push(Arr,at,typeinfo(sizeof Arr[0]))] <- value
    else
        concept_assert(false,"can't emplace value, which can't be moved")

def emplace(var Arr:array<auto(numT)>;var value:numT)
    static_if typeinfo(can_move value)
        static_if typeinfo(need_lock_check type<numT>)
            _builtin_verify_locks(Arr)
        Arr[__builtin_array_push_back(Arr,typeinfo(sizeof Arr[0]))] <- value
    else
        concept_assert(false,"can't emplace value, which can't be moved")

def push_clone(var Arr:array<auto(numT)>;value:numT implicit;at:int)
    static_if typeinfo(can_clone value)
        static_if typeinfo(need_lock_check type<numT>)
            _builtin_verify_locks(Arr)
        Arr[__builtin_array_push_zero(Arr,at,typeinfo(sizeof Arr[0]))] := value
    else
        concept_assert(false,"can't push-clone value, which can't be moved")

def push_clone(var Arr:array<auto(numT)>;value:numT implicit)
    static_if typeinfo(can_clone value)
        static_if typeinfo(need_lock_check type<numT>)
            _builtin_verify_locks(Arr)
        Arr[__builtin_array_push_back_zero(Arr,typeinfo(sizeof Arr[0]))] := value
    else
        concept_assert(false,"can't push-clone value, which can't be moved")

def push_clone ( var A : auto(CT) -# -const; b : auto(TT) implicit )
    static_if !typeinfo(can_clone type<TT>)
        concept_assert(false,"can't push_clone type which can't be cloned")
    static_elif !typeinfo(can_be_placed_in_container type<TT>)
        concept_assert(false,"can't push_clone type which can't be placed in a container")
    static_elif !typeinfo(builtin_function_exists @@ <(var a:CT -# -const;var b:TT -# -const):void> _::emplace)
        concept_assert(false,"can't push_clone, missing emplace")
    else
        unsafe
            var cb : TT -# -const := b
            _::emplace(A, cb)

def back ( var a : array<auto(TT)> -const =const ) : TT &
    let l = length(a)
    if l==0
        panic("back empty array")
    unsafe
        return a[l-1]

def back ( a : array<auto(TT)> const =const ) : TT const &
    let l = length(a)
    if l==0
        panic("back empty array")
    unsafe
        return a[l-1]

def erase(var Arr:array<auto(numT)>;at:int)
    static_if typeinfo(need_lock_check type<numT>)
        _builtin_verify_locks(Arr)
    __builtin_array_erase(Arr,at,typeinfo(sizeof Arr[0]))

[unused_argument(a)]
def length(a:auto[]):int
    return typeinfo(dim a)

def empty(a:array<auto>) : bool
    return length(a)==0

// table

def find(Tab:table<auto(keyT);auto(valT)>;at:keyT implicit;blk:block<(p:valT?):void>)
    __builtin_table_lock(Tab)
    var val = __builtin_table_find(Tab,at)
    invoke(blk,val)
    __builtin_table_unlock(Tab)

def find_if_exists(Tab:table<auto(keyT);auto(valT)>;at:keyT implicit;blk:block<(p:valT?):void>)
    var val = __builtin_table_find(Tab,at)
    if val != null
        __builtin_table_lock(Tab)
        invoke(blk,val)
        __builtin_table_unlock(Tab)
        return true
    else
        return false

def find(Tab:table<auto(keyT);auto(valT)>;at:keyT implicit):valT const?
    unsafe
        return reinterpret<valT const?>(__builtin_table_find(Tab,at))

def find(Tab:table<auto(keyT);auto(valT)>#;at:keyT implicit):valT const?#
    unsafe
        return reinterpret<valT const?#>(__builtin_table_find(Tab,at))

def find_for_edit(var Tab:table<auto(keyT);auto(valT)>;at:keyT implicit;blk:block<(var p:valT?):void>)
    __builtin_table_lock(Tab)
    var val = __builtin_table_find(Tab,at)
    invoke(blk,val)
    __builtin_table_unlock(Tab)

def find_for_edit_if_exists(var Tab:table<auto(keyT);auto(valT)>;at:keyT implicit;blk:block<(var p:valT?):void>)
    var val = __builtin_table_find(Tab,at)
    if val != null
        __builtin_table_lock(Tab)
        invoke(blk,val)
        __builtin_table_unlock(Tab)

[unsafe_operation]
def find_for_edit(var Tab:table<auto(keyT);auto(valT)>;at:keyT implicit):valT?
    return __builtin_table_find(Tab,at)

def _at_with_lockcheck ( var Tab:table<auto(keyT);auto(valT)>; at:keyT implicit ) : valT &
    static_if typeinfo(need_lock_check type<valT>)
        _builtin_verify_locks(Tab)
    unsafe
        return Tab[at]

def erase(var Tab:table<auto(keyT);auto(valT)>;at:keyT implicit):bool
    if typeinfo(need_lock_check type<valT>)
        _builtin_verify_locks(Tab)
    return __builtin_table_erase(Tab,at)

def key_exists(Tab:table<auto(keyT);auto(valT)>;at:keyT implicit):bool
    return __builtin_table_key_exists(Tab,at)

def binary_save(obj; subexpr:block<(data:array<uint8>):void>)
    concept_assert(typeinfo(is_ref_type obj),"can only serialize ref types")
    _builtin_binary_save(obj,subexpr)

def binary_load(var obj; data:array<uint8>)
    concept_assert(typeinfo(is_ref_type obj),"can only serialize ref types")
    _builtin_binary_load(obj,data)

def clone_to_move(clone_src:auto(TT) implicit) : TT -const -#
    unsafe
        var clone_dest : TT - #
        clone_dest := clone_src
        return <- clone_dest

def clone_dim(var a;b:auto implicit)
    static_if typeinfo(is_dim a) && typeinfo(is_dim b) && typeinfo(dim a)==typeinfo(dim b)
        for aV,bV in a,b
            aV := bV
    else
        concept_assert(false,"can't clone this array")

def clone(var a:array<auto(TT)>;b:array<TT>)
    resize(a,length(b))
    for aV,bV in a,b
        aV := bV

def clone(var a:array<auto(TT)>;b:array<TT>#)
    resize(a,length(b))
    for aV,bV in a,b
        aV := bV

def clone(var a:table<auto(KT);auto(VT)>;b:table<KT;VT> implicit)
    clear(a)
    for k,v in keys(b),values(b)
        a[k] := v

def clone(var a:table<string;auto(VT)>;b:table<string;VT> implicit)
    clear(a)
    for k,v in keys(b),values(b)
        let kk := k
        a[kk] := v

def keys(a:table<auto(keyT);auto(valT)> implicit) : iterator<keyT & const>
    var it : iterator<keyT & const>
    __builtin_table_keys(it,a,typeinfo(sizeof type<keyT>))
    return <- it

def values(a:table<auto(keyT);auto(valT)> =const implicit) : iterator<valT & const>
    var it : iterator<valT & const>
    __builtin_table_values(it,a,typeinfo(sizeof type<valT>))
    return <- it

def values(var a:table<auto(keyT);auto(valT)> =const implicit) : iterator<valT &>
    var it : iterator<valT &>
    __builtin_table_values(it,a,typeinfo(sizeof type<valT>))
    return <- it

def finalize_dim(var a : auto(TT)[])
    static_if typeinfo(can_delete type<TT>)
        for aV in a
            unsafe
                delete aV
    else
        concept_assert(false,"can't finalize this array")

def finalize(var a:array<auto(TT)>)
    static_if typeinfo(can_delete type<TT>)
        for aV in a
            unsafe
                delete aV
    __builtin_array_free(a,typeinfo(sizeof a[0]))

def finalize(var a:table<auto(TK);auto(TV)>)
    static_if typeinfo(can_delete type<TV>)
        for aV in values(a)
            unsafe
                delete aV
    __builtin_table_free(a,typeinfo(sizeof type<TK>),typeinfo(sizeof type<TV>))

// temporal

def lock ( Tab : table<auto(keyT);auto(valT)>; blk : block<(t : table<keyT;valT>#):void >  )
    __builtin_table_lock(Tab)
    unsafe
        invoke(blk,reinterpret<table<auto(keyT);auto(valT)> const implicit>(Tab))
    __builtin_table_unlock(Tab)

def lock ( Tab : table<auto(keyT);auto(valT)>#; blk : block<(t : table<keyT;valT>#):void >  )
    invoke(blk,Tab)

def lock_forever ( var Tab : table<auto(keyT);auto(valT)> ) : table<keyT;valT> #
    __builtin_table_lock(Tab)
    unsafe
        return <- reinterpret<table<keyT;valT> #>(Tab)

// functional

def next ( it:iterator<auto(TT)>; var value : TT& ) : bool
    static_if !typeinfo(can_copy type<TT>)
        concept_assert(false, "requires type which can be copied")
    static_elif typeinfo(is_ref_value type<TT>)
        var pValue : TT - & ?
        unsafe
            if _builtin_iterator_iterate(it, addr(pValue))
                value = *pValue
                return true
            else
                return false
    else
        unsafe
            return _builtin_iterator_iterate(it, addr(value))

[expect_any_vector(vec)]
def length ( vec )
    return vec . . length

[generic]
def each ( rng : range ) : iterator<int>
    var it : iterator<int>
    _builtin_make_range_iterator(it,rng)
    return <- it

def iter_range ( foo )
    concept_assert(typeinfo(is_iterable foo),"requires iterable")
    return range(_::length(foo)) // assuming length(foo) exists

[generic,unsafe_operation]
def each ( str : string ) : iterator<int>
    var it : iterator<int>
    _builtin_make_string_iterator(it,str)
    return <- it

[unsafe_operation]
def each ( a : auto(TT)[] ) : iterator<TT&>
    unsafe
        var parr : void? = reinterpret<void?>(addr(a[0]))
        var it : iterator<TT&>
        _builtin_make_fixed_array_iterator(it,parr,typeinfo(dim a),typeinfo(sizeof a[0]))
        return <- it

[unsafe_operation]
def each ( a : array<auto(TT)> ) : iterator<TT&>
    var it : iterator<TT&>
    _builtin_make_good_array_iterator(it, a, typeinfo(sizeof a[0]))
    return <- it

def each ( lam : lambda<(var arg:auto(argT)):bool> ) : iterator<argT -&>
    concept_assert(typeinfo(is_ref type<argT>),"lamda-to-iterator argument has to be reference; expecting value& or reference type")
    var it : iterator<argT -&>
    _builtin_make_lambda_iterator(it,lam,typeinfo(sizeof type<argT>))
    unsafe
        return <- it

def each_ref ( lam : lambda<(var arg:auto(argT)?):bool> ) : iterator<argT &>
    var it : iterator<argT &>
    _builtin_make_lambda_iterator(it,lam,typeinfo(sizeof type<void?>))
    unsafe
        return <- it

[unused_argument(tt)]
def each_enum(tt:auto(TT)) : iterator<TT -const -&>
    concept_assert(typeinfo(is_enum tt),"expecting each_enum(any enum value)")
    var iter : iterator<TT -const -&>
    _builtin_make_enum_iterator(iter)
    unsafe
        return <- iter

def nothing( var it : iterator<auto(TT)> ) : iterator<TT>
    _builtin_make_nil_iterator(it)
    return <- it

def to_array ( it : iterator<auto(TT)> ) : array<TT -const -&>
    var arr : array<TT -const -&>
    for x in it
        push_clone(arr,x)
    return <- arr

def to_array(a:auto(TT)[]) : array<TT -const>
    var arr : array<TT -const>
    resize(arr,length(a))
    unsafe
        reinterpret<TT -const[typeinfo(dim a)]>(addr(arr[0])) := a
    return <- arr

def to_array_move(var a:auto(TT)[]) : array<TT -const>
    var arr : array<TT -const>
    static_if typeinfo(can_copy a)
        resize(arr,length(a))
        unsafe
            reinterpret<TT -const[typeinfo(dim a)]>(addr(arr[0])) = a
    static_elif typeinfo(can_move a)
        resize(arr,length(a))
        unsafe
            reinterpret<TT -const[typeinfo(dim a)]>(addr(arr[0])) <- a
    else
        concept_assert(false,"this array can't be copied or moved")
    return <- arr

def to_array_move(var a:auto(TT)) : array<TT -const>
    var arr : array<TT -const>
    static_if typeinfo(can_copy a)
        arr |> push(a)
    static_elif typeinfo(can_move a)
        arr |> emplace(a)
    else
        concept_assert(false,"this array can't be copied or moved")
    return <- arr

def to_table(a:tuple<auto(keyT);auto(valT)>[]) : table<keyT -const;valT>
    var tab : table<keyT -const;valT>
    for x in a
        tab[x._0] := x._1
    return <- tab

def to_table_move(var a:tuple<auto(keyT);auto(valT)>[]) : table<keyT -const;valT>
    var tab : table<keyT -const;valT>
    static_if typeinfo(can_copy type<valT>)
        for x in a
            tab[x._0] = x._1
    static_elif typeinfo(can_move type<valT>)
        for x in a
            tab[x._0] <- x._1
    else
        concept_assert(false,"this array can't be copied or moved")
    return <- tab

def sort ( var a : auto(TT)[] implicit )
    static_if typeinfo(is_numeric_comparable type<TT>)
        unsafe
            __builtin_sort ( addr(a[0]), length(a) )    // there is numeric specialization
    static_elif typeinfo(is_vector type<TT>)
        concept_assert(false,"there is no default compare function for vectors")
    static_elif typeinfo(is_string type<TT>)
        unsafe
            __builtin_sort_string ( addr(a[0]), length(a) )     // there is string specialization
    else
        unsafe
            static_if typeinfo(is_ref_type type<TT>)
                __builtin_sort_dim_any_cblock ( a, typeinfo(sizeof a[0]), length(a) ) <| $ ( x, y  : TT ) : bool
                    return x < y
            else
                __builtin_sort_dim_any_ref_cblock ( a, typeinfo(sizeof a[0]), length(a) ) <| $ ( x, y  : TT ) : bool
                    return x < y

def sort ( var a : array<auto(TT)> implicit )
    static_if typeinfo(is_numeric_comparable type<TT>)
        __builtin_array_lock(a)
        unsafe
            __builtin_sort ( addr(a[0]), length(a) )    // there is numeric specialization
        __builtin_array_unlock(a)
    static_elif typeinfo(is_vector type<TT>)
        concept_assert(false,"there is no default compare function for vectors")
    static_elif typeinfo(is_string type<TT>)
        __builtin_array_lock(a)
        unsafe
            __builtin_sort_string ( addr(a[0]), length(a) )     // there is string specialization
        __builtin_array_unlock(a)
    else
        unsafe
            static_if typeinfo(is_ref_type type<TT>)
                __builtin_sort_array_any_cblock ( a, typeinfo(sizeof a[0]), length(a) ) <| $ ( x, y  : TT ) : bool
                    return x < y
            else
                __builtin_sort_array_any_ref_cblock ( a, typeinfo(sizeof a[0]), length(a) ) <| $ ( x, y  : TT ) : bool
                    return x < y

[builtin_array_sort]
def sort ( var a : auto(TT)[] implicit; cmp : block<(x,y:TT):bool> )
    static_if typeinfo(is_numeric_comparable type<TT>)
        unsafe
            __builtin_sort_cblock_dim ( a, typeinfo(sizeof a[0]), length(a), cmp )
    static_elif typeinfo(is_vector type<TT>)
        unsafe
            __builtin_sort_cblock_dim ( a, typeinfo(sizeof a[0]), length(a), cmp )
    static_elif typeinfo(is_string type<TT>)
        unsafe
            __builtin_sort_cblock_dim ( a, typeinfo(sizeof a[0]), length(a), cmp )
    else
        unsafe
            static_if typeinfo(is_ref_type type<TT>)
                __builtin_sort_dim_any_cblock ( a, typeinfo(sizeof a[0]), length(a), cmp )
            else
                __builtin_sort_dim_any_ref_cblock ( a, typeinfo(sizeof a[0]), length(a), cmp )

[builtin_array_sort]
def sort ( var a : array<auto(TT)> implicit; cmp : block<(x,y:TT):bool> )
    static_if typeinfo(is_numeric_comparable type<TT>)
        unsafe
            __builtin_sort_cblock_array ( a, typeinfo(sizeof a[0]), length(a), cmp )
    static_elif typeinfo(is_vector type<TT>)
        unsafe
            __builtin_sort_cblock_array ( a, typeinfo(sizeof a[0]), length(a), cmp )
    static_elif typeinfo(is_string type<TT>)
        unsafe
            __builtin_sort_cblock_array ( a, typeinfo(sizeof a[0]), length(a), cmp )
    else
        unsafe
            static_if typeinfo(is_ref_type type<TT>)
                __builtin_sort_array_any_cblock ( a, typeinfo(sizeof a[0]), length(a), cmp )
            else
                __builtin_sort_array_any_ref_cblock ( a, typeinfo(sizeof a[0]), length(a), cmp )

def lock ( var a : array<auto(TT)> = const; blk : block<(var x : array<TT># implicit)> )
    __builtin_array_lock(a)
    invoke(blk, a)
    __builtin_array_unlock(a)

def lock ( a : array<auto(TT)> = const; blk : block<(x : array<TT># implicit)> )
    __builtin_array_lock(a)
    invoke(blk, a)
    __builtin_array_unlock(a)

def lock_data ( var a : array<auto(TT)> = const; blk : block<(var p:TT?# implicit;s:int)> )
    let len = length(a)
    if len != 0
        __builtin_array_lock(a)
        unsafe
            invoke(blk, addr(a[0]), len)
        __builtin_array_unlock(a)
    else
        var nullT : TT?
        invoke(blk, nullT, 0)

def lock_data ( a : array<auto(TT)> = const; blk : block<(p:TT const?# implicit;s:int)> )
    let len = length(a)
    if len != 0
        __builtin_array_lock(a)
        unsafe
            invoke(blk, addr(a[0]), len)
        __builtin_array_unlock(a)
    else
        invoke(blk, null, 0)

def find_index ( arr : array<auto(TT)> implicit; key : TT )
    for i in range(length(arr))
        if arr[i]==key
            return i
    return -1

def find_index ( arr : auto(TT)[] implicit; key : TT )
    for i in range(length(arr))
        if arr[i]==key
            return i
    return -1

def find_index ( arr : iterator<auto(TT)>; key : TT - & )
    for o,i in arr,range(INT_MAX)
        if o == key
            return i
    return -1

def find_index_if ( arr : array<auto(TT)> implicit; blk : block<(key:TT):bool> )
    for i in range(length(arr))
        if invoke(blk,arr[i])
            return i
    return -1

def find_index_if ( arr : auto(TT)[] implicit; blk : block<(key:TT):bool> )
    for i in range(length(arr))
        if invoke(blk,arr[i])
            return i
    return -1

def find_index_if ( arr : iterator<auto(TT)>; blk : block<(key:TT - &):bool> )
    for o,i in arr,range(INT_MAX)
        if invoke(blk,o)
            return i
    return -1

def has_value ( a; key )
    static_if typeinfo(is_iterable a)
        for t in a
            if t == key
                return true
        return false
    else
        concept_assert(false,"can't iterate over source. has_value is meaningless")
        return false

def get_ptr ( src : smart_ptr<auto(TT)> ) : TT?
    unsafe
        return reinterpret<TT?> src


def get_const_ptr ( src : smart_ptr<auto(TT)> ) : TT ? const
    unsafe
        return reinterpret<TT ? const> src

[generic]
def get_command_line_arguments : array<string>
    var args : array<string>
    builtin_get_command_line_arguments(args)
    return <- args

[unsafe_operation]
def map_to_array ( data:void?; len:int; blk:block<(var arg:array<auto(TT)>#)> )
    assert(data != null,"expecting data")
    assert(len > 0,"at least 1 element")
    let elem_size = typeinfo(sizeof type<TT>)
    let num_elem = len / elem_size
    unsafe
        _builtin_temp_array(data, num_elem, blk)

[unsafe_operation]
def map_to_ro_array ( data:void?; len:int; blk:block<(arg:array<auto(TT)>#)> )
    assert(data != null,"expecting data")
    assert(len > 0,"at least 1 element")
    let elem_size = typeinfo(sizeof type<TT>)
    let num_elem = len / elem_size
    unsafe
        _builtin_temp_array(data, num_elem, blk)

def swap ( var a,b:auto(TT)& )
    var t : TT - & <- a
    a <- b
    b <- t
