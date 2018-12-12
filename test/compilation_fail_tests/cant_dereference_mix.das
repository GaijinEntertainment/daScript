expect 30501:2    // cant_dereference

def test
    let(a:int)          // can't dereference not a pointer
        debug(a->)
    let(a:void?)        // can't dereference void pointer
        debug(a->)
