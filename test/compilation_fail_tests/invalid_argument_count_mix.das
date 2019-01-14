expect 30107:6    // invalid_argument_count

def test
    // assert 1 or 2 arguments
    assert()
    assert(false,"blah",1)
    // debug 1 or 2 arguments
    debug()
    debug(0,"blah",1,"")
    // hash 1 argument only
    hash()
    hash(1,2)

