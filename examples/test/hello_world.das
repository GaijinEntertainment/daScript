options log=true, logStack=true, logNodes=true
// options logInferPasses=true
// options printVarAccess=true

/*
def each ( rng : range ) : iterator<int>
    return _builtin_make_range_iterator(rng)

[unsafe]
def each ( a : auto(TT)[] ) : iterator<TT&>
    var parr : void? = reinterpret<void?>(addr(a[0]))
    return _builtin_make_fixed_array_iterator(parr,typeinfo(dim a),typeinfo(sizeof type TT))

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
*/

def test_each
    for r in each(range(0,5))
        print("r={r}\n")
    for fa in each([[auto 1.1; 2.2; 3.3]])
        print("fa={fa}\n")
    for ga in each(toarray([[auto "a";"b";"c"]]))
        print("ga={ga}\n")

def test_to_array
    //let ra <- toarray(each(range(0,5)))
    //print("ra={ra}\n")
    let fa <- toarray(each([[auto 1.1; 2.2; 3.3]]))
    print("fa={fa}\n")
    let ga <- toarray(each(toarray([[auto "a";"b";"c"]])))
    print("ga={ga}\n")

[unsafe,sideeffects]
def test_range
    let t : range
    let rng  = range(1,5)
    debug(rng)
    let rit : iterator<int> = _builtin_make_range_iterator(rng);
    var ikey : int
    var pkey : void? = reinterpret<void?>(addr(ikey))
    var loop = _builtin_iterator_first(rit,pkey)
    while loop
        print("rval = {ikey}\n")
        loop = _builtin_iterator_next(rit,pkey)
    _builtin_iterator_close(rit,pkey)

[unsafe,sideeffects]
def test_good_array
    let arr <- toarray([[int 1;2;3;4]])
    let ait : iterator<int&> = _builtin_make_good_array_iterator(arr,typeinfo(sizeof type int))
    var ikey : int?
    var pkey : void? = reinterpret<void?>(addr(ikey))
    var loop = _builtin_iterator_first(ait,pkey)
    while loop
        print("*ait = {deref(ikey)}\n")
        loop = _builtin_iterator_next(ait,pkey)
    _builtin_iterator_close(ait,pkey)

[unsafe,sideeffects]
def test_fixed_array
    var arr = [[int 1;2;3;4]]
    var parr : void? = reinterpret<void?>(addr(arr[0]))
    let fit : iterator<int&> = _builtin_make_fixed_array_iterator(parr,typeinfo(dim arr),typeinfo(sizeof type int))
    var ikey : int?
    var pkey : void? = reinterpret<void?>(addr(ikey))
    var loop = _builtin_iterator_first(fit,pkey)
    while loop
        print("*fit = {deref(ikey)}\n")
        loop = _builtin_iterator_next(fit,pkey)
    _builtin_iterator_close(fit,pkey)

[unsafe,sideeffects]
def test_keys_values
    let t <- totable([[auto 1=>"one"; 2=>"two"; 3=>"three"]])
    for k,v in keys(t),values(t)
        print("{k} => {v}\n")
    if true
        let itk : iterator<int const&> = keys(t)
        var ikey : int?
        var pkey : void? = reinterpret<void?>(addr(ikey))
        var loop = _builtin_iterator_first(itk,pkey)
        while loop
            print("*ikey = {deref(ikey)}\n")
            loop = _builtin_iterator_next(itk,pkey)
        _builtin_iterator_close(itk,pkey)
    if true
        let itv : iterator<string&> = values(t)
        var sval : string?
        var pval : void? = reinterpret<void?>(addr(sval))
        var loop = _builtin_iterator_first(itv,pval)
        while loop
            print("itv = {deref(sval)}\n")
            loop = _builtin_iterator_next(itv,pval)
        _builtin_iterator_close(itv,pval)

[export]
def test
    test_to_array()
    test_each()
    test_range()
    test_fixed_array()
    test_good_array()
    test_keys_values()
    return true

