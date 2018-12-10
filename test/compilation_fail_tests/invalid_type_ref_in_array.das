expect 30105    // invalid_array_type

def test
    let(a:array<int&>)   // can't have array of ref
        debug(a)

