// options log=true,logInferPasses=true,optimize=false

def take_int(b:block<(arg:int):int>)
    return invoke(b,3)

def test
    let x = take_int() <| $(a)
        return a + 1
    assert(x==4)


