options log=true, logInferPasses=true, optimize=true

def test
    let a:array<int>
    push(a,1)
    push(a,2)
    let b = a
    print("\na={a} b={b}\n")
    assert(length(a)==0 & length(b)==2)
    return true
