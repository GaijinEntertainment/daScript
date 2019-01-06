// options log=true,logInferPasses=true,optimize=false
options log=true

def madd(a,b,c)
    return a*b+c

def test
    let x = madd(1,2,3)
    assert(x==5)
    let y = madd(1.0,2.0,3.0)
    assert(y==5.0)
    return true
