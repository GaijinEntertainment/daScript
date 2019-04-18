options log = true, logInferPasses = true

let
    network : string

def boo()
    return 0

def boo(a:int=1;b:int)
    return a + b

def boo(a,b,c:int)
    return a + b + c

[export]
def test
    // regular infer
    let t1 = boo([a=2,b=3])
    assert(t1==5)
    let t2 = boo([b=3])
    assert(t2==4)
    let t3 = boo([a=1,b=2,c=3])
    assert(t3==6)
    return true

