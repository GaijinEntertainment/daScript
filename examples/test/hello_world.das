// options log_infer_passes = true
// options log = true
// options log_nodes = true

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

 // reduce value, any invokable
[private]
def reduce_any(it:iterator<auto(TT)>;functor)
    var left : TT delete &
    var first = true
    for right in it
        if first
            first = false
            left = right
        else
            left = invoke(functor,left,right)
    if first
        panic("can't reduce empty sequence")
    return left

 // reduce with lambda
[unsafe]
def reduce(it:iterator<auto(TT)>;blk:lambda<(left,right:TT delete &):TT const delete &>)
    return reduce_any(it,blk)

 // reduce with function
[unsafe]
def reduce(it:iterator<auto(TT)>;blk:function<(left,right:TT delete &):TT const delete &>)
    return reduce_any(it,blk)

 // reduce with block
[unsafe]
def reduce(it:iterator<auto(TT)>;blk:block<(left,right:TT delete &):TT const delete &>)
    return reduce_any(it,blk)

// summ of all elements
//  same as reduce(it, @(a,b) => a + b)
def sum(it:iterator<auto(TT)>)
    var temp : TT delete &
    for x in it
        temp += x
    return temp

// any
def any ( it )
    static_if !typeinfo(is_iterable it)
        concept_assert(false,"can't ANY non-iterable type")
        return false
    else
        for x in it
            if x
                return true
        return false

// all
def all ( it )
    static_if !typeinfo(is_iterable it)
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
        for x,i in src,range(0,stop)
            if i>=start
                yield x
        return false

// [[ start; start+step; start+2*step ]]
def count(var start,step:int) : iterator<int>
    return <- each() <| @(var result:int&) : bool
        result = start
        start += step
        return true

// [[ value; value; value; .... count times ]]
def repeat(value : auto(TT); var count : int = 0)
    return <- generator<TT> () <| $ ()
        if count == 0
            while true
                yield value
        for x in range(0,count)
            yield value
        return false

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

def first(x)
    return x._0

[export,unsafe]
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

    (   // reduce ref type
            range(0,10)
        |>  each()
        |>  map( @(x : int) => [[auto x, 1.5]] )
        |>  reduce( $(x,y) => [[auto x._0 + y._0, 2.f]] )
        |>  first()
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
            [[bool true;false]]
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

    (   // infinite repeat
            repeat(10)
        |>  islice(0,5)
        |>  to_array()
        |>  length()
        |>  is_equal(5)
        |>  verify()
    )

    (   // limited repeat
            repeat(10,6)
        |>  to_array()
        |>  length()
        |>  is_equal(6)
        |>  verify()
    )

    return true
