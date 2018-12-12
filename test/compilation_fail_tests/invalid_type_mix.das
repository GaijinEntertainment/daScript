expect 30101:5    // invalid_type

def test(a:void&)       // can't have void &
    let(a:int[10] &)    // can't have ref of array
        debug(a)
    let(a:int & &)      // can't have ref of ref
        debug(a)
    let(a:int & ?)      // can't have pointer to ref
        debug(a)
    let(a:int & [10])   // can't have array of ref
        debug(a)
