expect 30107:8    // invalid_argument_count

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
    // push(array,value) or push(array,value,at)
    let(a:array<int>)
        push(a)
        push(a,1)       // this one fine
        push(a,1,0)     // this one too
        push(a,1,0,11)
