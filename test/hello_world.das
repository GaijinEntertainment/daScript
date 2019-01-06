options log=true // ,logInferPasses=false,logOptimizationPasses=true,optimize=true

def fold(a:array<auto(numt)>;from:numt)
    for x in a
        from += x
    static_assert(sizeof(type numt)==4,"only works for small types")
    return from

def test:bool
    let ( a:array<uint64>;b:uint64 )
        let q = fold(a,b)
    return true
