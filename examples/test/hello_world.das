options log = true

/////////////
// functional

// filter with lambda
def filter(var src:iterator<auto(TT)>; blk:lambda<(what:TT):bool> )
    return <- generator<TT> () <| $ ()
        for w in src
            if invoke(blk,w)
                yield w
        return false

// filter with function
def filter(var src:iterator<auto(TT)>; blk:function<(what:TT):bool> )
    return <- generator<TT> () <| $ ()
        for w in src
            if invoke(blk,w)
                yield w
        return false

// map with lambda
def map(var src:iterator<auto(TT)>; blk:lambda<(what:TT):auto(QQ)>) : iterator<QQ>
    return <- generator<QQ> () <| $ ()
        for w in src
            yield invoke(blk,w)
        return false

// map with function
def map(var src:iterator<auto(TT)>; blk:function<(what:TT):auto(QQ)>) : iterator<QQ>
    return <- generator<QQ> () <| $ ()
        for w in src
            yield invoke(blk,w)
        return false

// reduce ref, any invokable
[unsafe,private]
def reduce_by_reference(it:iterator<auto(TT)>;functor)
    var left : TT delete const &
    var right : TT delete const &?
    var pkey : void? = reinterpret<void?>(addr(right))
    var loop = _builtin_iterator_first(it,pkey)
    if loop
        left = *right
        loop = _builtin_iterator_next(it,pkey)
        while loop
            left = invoke(functor,left,*right)
            loop = _builtin_iterator_next(it,pkey)
    else
        panic("can't reduce empty sequence")
    _builtin_iterator_close(it,pkey)
    return left

// reduce value, any invokable
[unsafe,private]
def reduce_by_value(it:iterator<auto(TT)>;functor)
    var left, right : TT delete &
    var pkey : void? = reinterpret<void?>(addr(right))
    var loop = _builtin_iterator_first(it,pkey)
    if loop
        left = right
        loop = _builtin_iterator_next(it,pkey)
        while loop
            left = invoke(functor,left,right)
            loop = _builtin_iterator_next(it,pkey)
    else
        panic("can't reduce empty sequence")
    _builtin_iterator_close(it,pkey)
    return left

// reduce with lambda
[unsafe]
def reduce(it:iterator<auto(TT)>;blk:lambda<(left,right:TT delete &):TT const delete &>)
    if typeinfo(is_ref type TT)
        return reduce_by_reference(it,blk)
    else
        return reduce_by_value(it,blk)

// reduce with function
[unsafe]
def reduce(it:iterator<auto(TT)>;blk:function<(left,right:TT delete &):TT const delete &>)
    if typeinfo(is_ref type TT)
        return reduce_by_reference(it,blk)
    else
        return reduce_by_value(it,blk)

// reduce with block
[unsafe]
def reduce(it:iterator<auto(TT)>;blk:block<(left,right:TT delete &):TT const delete &>)
    if typeinfo(is_ref type TT)
        return reduce_by_reference(it,blk)
    else
        return reduce_by_value(it,blk)

// summ of all elements
//  same as reduce(it, @(a,b) => a + b)
def sum(it:iterator<auto(TT)>)
    var temp : TT delete &
    for x in it
        temp += x
    return temp

// any
def any ( it )
    if !typeinfo(is_iterable it)
        concept_assert(false,"can't ANY non-iterable type")
        return false
    else
        for x in it
            if x
                return true
        return false

// all
def all ( it )
    if !typeinfo(is_iterable it)
        concept_assert(false,"can't ALL non-iterable type")
        return false
    else
        for x in it
            if !x
                return false
        return true

// islice
def islice( var src : iterator<auto(TT)>; start,stop : int ) : iterator<TT>
    return <- generator<TT> () <| $ ()
        for x,i in src,range(0,INT_MAX)  // todo: optimize similar to reduce
            if i>=stop
                return false
            elif i>=start
                yield x
        return false

// [[ start; start+step; start+2*step ]]
def count(var start,step:int) : iterator<int>
    return <- each() <| @(var result:int&) : bool
        result = start
        start += step
        return true

def is_equal(a,b)
    return a == b

def is_not_equal(a,b)
    return a == b

def not(x)
    return !x

def echo(var x;extra : string = "\n")
    print("{x}{extra}")
    return <- x

///////
// test

def is_even(x:int)
    return x % 2 == 0

def twice(x:int)
    return x + x

def add(a,b:int)
    return a + b

[sideeffects]
def verify_even(arr)
    for x,y in arr,range(0,50)
        assert(x==y*2)

[export]
def test
    // compehension
    verify_even() <| [[for x in range(0,100); x; where x % 2==0]]

    // array comprehension
    verify_even() <| [{for x in range(0,100); x; where x % 2==0}]

    (   // filter with function
            range(0,100)
        |>  each()
        |>  filter(@@is_even)
        |>  verify_even()
    )

    (   // filter with lambda
            range(0,100)
        |>  each()
        |>  filter( @(x) => x % 2==0 )
        |>  verify_even()
    )

    (   // map
            range(0,50)
        |>  each()
        |>  map( @@twice )
        |>  verify_even()
    )

    (   // map with lambbda
            range(0,50)
        |>  each()
        |>  map( @(x : int) => x + x )
        |>  verify_even()
    )


    (   // reduce non-ref iterator with block
            range(0,10)
        |>  each()
        |>  reduce( $(x,y) => x + y )
        |>  is_equal(45)
        |>  verify()
    )

    (   // reduce ref iterator with block
            [[ for x in range(0,10); x ]]
        |>  reduce( $(x,y) => x + y )
        |>  is_equal(45)
        |>  verify()
    )

    (   // reduce non-ref iterator with lambda
            range(0,10)
        |>  each()
        |>  reduce( @(x,y) => x + y )
        |>  is_equal(45)
        |>  verify()
    )

    (   // reduce non-ref iterator with function
            range(0,10)
        |>  each()
        |>  reduce( @@add )
        |>  is_equal(45)
        |>  verify()
    )

    (   // reduce single element sequence
            range(45,46)
        |>  each()
        |>  reduce( @@add )
        |>  is_equal(45)
        |>  verify()
    )

    (   // sum
            range(0,10)
        |>  each()
        |>  sum()
        |>  is_equal(45)
        |>  verify()
    )

    (   // any (pass)
            range(0,10)
        |>  each()
        |>  map ( @(x) : bool => x % 3 != 0 )
        |>  any()
        |>  verify()
    )

    (   // any (fail)
            range(0,10)
        |>  each()
        |>  map ( @(x) : bool => x > 30 )
        |>  any()
        |>  not()
        |>  verify()
    )

    (   // all (pass)
            range(0,10)
        |>  each()
        |>  map ( @(x) : bool => x < 100 )
        |>  all()
        |>  verify()
    )

    (   // all (fail)
            range(0,10)
        |>  each()
        |>  map ( @(x) : bool => x > 0 )
        // |>  to_array()   // if we uncomment this two, we can see intermediate values
        // |>  echo()
        |>  all()
        |>  not()
        |>  verify()
    )

    (   // islice
            range(0,10)
        |>  each()
        |>  islice(1,4)
        |>  sum()
        |>  is_equal(6)
        |>  verify()
    )

    (   // count
            count(0,10)
        |>  islice(0,10)
        |>  sum()
        |>  is_equal(450)
        |>  verify()
    )

    return true
