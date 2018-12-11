expect 30108:2    // invalid_variable_type

def test
    let(a:int&)         // local variable can't be &
        debug(a)
    let(a:void)         // local variable can't be void
        debug(a)
