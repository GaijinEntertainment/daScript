// options log=true,logInferPasses=true,optimize=true

def join(a:array<auto(numt)>)
    let t : numt = numt(0)
    for x in a
        t += x
    return t

[export]
def test:bool
    let ( a:array<float> )
        push(a,1.0)
        push(a,2.0)
        push(a,3.0)
        let t = join(a)
        assert(t==6.0)
    return true
