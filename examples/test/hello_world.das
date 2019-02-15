options log=true, logInferPasses=true

def add(a,b:int):int
    return a + b

[export]
def test
    let fn = @add
    let t = invoke(fn, 1, 2)
    debug(t,"\nt=")
    assert(t==3)
    return true


