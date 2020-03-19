// options log_infer_passes = true
// options log = true
// options log_nodes = true
// options log_cpp = true

require functional

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

