expect 30305    // variable_not_found

def test:bool
    let(x:int=1)
        debug(x)
    let y:int=2
    debug(y)
    debug(x)        // can't locate variable
    return true
