options optimize=false

expect 30103:13   // invalid_argument_type

def test
    // assert(bool,comment)
    assert(1,"sure")            // can only assert on boolean expressions
    assert(true,"blah")         // this one fine
    assert(true,1)              // even this is not a string constant
    let(a:string="comment")
        assert(true,a)          // even this is not a string constant
        debug(0,a)              // not for debug either
        debug(a,"blah")         // this one good
    let(a:array<int>)
        push(1, 1)              // must push to array
        push(a,"a")             // must push the same type
        push(a, 1, "at")        // index must be index
        push(a, 1)              // this one ok
        push(a, 1, 2)           // this one good too
        erase(1,1)
        erase(a,"blah")
        erase(a,1)
        find(1,1)
        find(a,"1")
        find(a,1)               // this one fine
    let(a:table<string,int>)
        erase(a,1)
        erase(a,"2")
        find(a,1)
        find(a,"1")             // this one fine

