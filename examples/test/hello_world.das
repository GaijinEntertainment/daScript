options log = true

struct Foo
	a : int

[export]
def test
	let y : Foo
	let x = y + 1
	print("x = {x}\n")
	let t = [{int 1;2;3}]
	print("t = {t}\n")
	return true

/*
options log_infer_passes = true

require functional

[unsafe]
def islice2( var src : iterator<auto(TT)>; start,stop : int )
    return <- generator<TT const &> () <| $ ()
        for x,i in src,range(0,stop)
            if i>=start
                yield x
        return false

[private,unsafe]
def filter_any2(var src:iterator<auto(TT)>; blk )
    static_if !typeinfo(is_ref_type type TT)
        return <- generator<TT delete &> () <| $ ()
            for w in src
                if invoke(blk,w)
                    yield w
            return false
    else
        return <- generator<TT const &> () <| $ ()
            for w in src
                if invoke(blk,w)
                    yield w
            return false

def filter2(var src:iterator<auto(TT)>; blk:lambda<(what:TT delete &):bool> )
    return <- filter_any2(src,blk)

// filter with function
def filter2(var src:iterator<auto(TT)>; blk:function<(what:TT delete &):bool> )
    return <- filter_any2(src,blk)

[export]
def test
    (   // filter and map with moveable type
            [[for x in range(0,100); [{int x;2}] ]]
        |>  filter2( @(x) => x[0] % 2==0 )
        |>  echo()
    )

    // (   // islice
    //         range(0,10)
    //     |>  each()
    //     |>  islice2(1,4)
    //     |>  sum()
    //     |>  is_equal(6)
    //     |>  verify()
    // )


    return true
*/