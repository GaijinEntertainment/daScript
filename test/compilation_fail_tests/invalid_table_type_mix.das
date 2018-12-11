expect 30106:2    // invalid_table_type

def test
    let(a:table<int&,string>)           // can't have table key ref
        debug(a)
    let(a:table<array<int>,string>)     // key has to be basic hashable type
        debug(a)
    let(a:void)
        debug(a)

