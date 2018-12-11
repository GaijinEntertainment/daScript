expect 30105:2    // invalid_array_type

def test
    let(a:array<int&>)   // can't have array of ref
        debug(a)
    let(a:array<void>)   // can't have void array
        debug(a)

