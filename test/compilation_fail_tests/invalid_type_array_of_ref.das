expect 30101    // invalid_type

def test
    let(a:int & [10])   // can't have array of ref
        debug(a)
