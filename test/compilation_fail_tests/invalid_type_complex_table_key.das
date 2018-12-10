expect 30106    // invalid_table_type

def test
    let(a:table<array<int>,string>)   // key has to be basic hashable type
        debug(a)



