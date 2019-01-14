options optimize=false

expect 30103:6   // invalid_argument_type

def test
    // assert(bool,comment)
    assert(1,"sure")            // 30103,   can only assert on boolean expressions
    assert(true,"blah")         // this one fine
    assert(true,1)              // even this is not a string constant
    let(a:string="comment")
        assert(true,a)          // even this is not a string constant
        debug(0,a)              // 30103,   not for debug either
        debug(a,"blah")         // this one good
    let(a:table<string,int>)
        erase(a,1)              // 30103,   key must be of the same type as table<key,...>
        erase(a,"2")            // this one ok
        find(a,1)               // 30103,   key must be of the same type as table<key,...>
        find(a,"1")             // this one fine

