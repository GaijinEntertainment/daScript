options log=true, logStack=true, logNodes=true

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
    test_fixed_array()
    test_good_array()
    test_keys_values()
    return true
