// options log=true,logInferPasses=false,logOptimizationPasses=true,optimize=true

expect 40100

def fold(a:array<auto(numt)>;from:numt)
    for x in a
        from += x
    static_assert(typeinfo(sizeof type numt)==4,"only works for small types")
    return from

def join(a:array<auto(numt)>)
    let b = numt(0)
    return fold(a,b)

def test(msg)
    let a:array<uint64>
    let q = join(a)
    return string(q) + msg

def test:bool
    print(test("this"))
    return true

