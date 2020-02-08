def filter(src:iterator<auto(TT)>; blk:lambda<(what:TT):bool> )
    return generator<TT> () <| $ ()
        for w in src
            if invoke(blk,w)
                yield w
        return false

def map(src:iterator<auto(TT)>; blk:lambda<(what:auto(TT)):auto(QQ)>) : iterator<QQ>
    return generator<QQ> () <| $ ()
        for w in src
            yield invoke(blk,w)
        return false

def testGen(total,mod:int)
    return (
        each(range(0,total))
        |> filter ( lambda($(x:int) : bool => (x%mod)==0) )
        |> map ( lambda($(x:int) : string => string(x)) )
        |> join(",")
    )

def each_a(r:range) : array<int>
    var a : array<int>
    reserve(a,r.y-r.x)
    for x in r
        push(a,x)
    return <- a

def map_a(src:array<auto(TT)>; blk:block<(what:auto(TT)):auto(QQ)>) : array<QQ>
    var a : array<QQ>
    reserve(a, length(src))
    for x in src
        push(a,invoke(blk,x))
    return <- a

def filter_a(src:array<auto(TT)>; blk:block<(what:TT):bool> ) : array<TT>
    var a : array<TT>
    for x in src
        if invoke(blk,x)
            push(a,x)
    return <- a

def testArr(total,mod:int)
    var in scope a1 <- each_a(range(0,total))
    var in scope a2 <- filter_a(a1,$(x) => (x%mod)==0)
    var in scope a3 <- map_a(a2, $(x:int):string => string(x))
    return join(a3,",")

def testLoop(total,mod:int)
    let st = build_string() <| $ (var writer)
        var skip_first = true
        for t in range(0,total)
            if (t % mod) == 0
                if skip_first
                    skip_first = false
                else
                    write(writer, ",")
                write(writer,string(t))
    return st

[export]
def test()
    var s1, s2, s3 : string
    let TOTAL = 1000000
    let MOD = 99961
    profile(20,"testLoop") <|
        s1 = testLoop(TOTAL,MOD)
    profile(20,"testGen") <|
        s2 = testGen(TOTAL,MOD)
    profile(20,"testArr") <|
        s3 = testArr(TOTAL,MOD)
    assert(s1==s2 & s1==s3 & s2==s3)
    return true

