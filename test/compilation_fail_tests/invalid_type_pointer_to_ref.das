expect 30101    // invalid_type

def test
    let(a:int & *)   // can't have pointer to ref
        debug(a)

