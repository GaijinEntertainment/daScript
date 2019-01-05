options optimize=false

expect 30103:13   // invalid_argument_type

def test
    // assert(bool,comment)
    assert(1,"sure")            // 30103,   can only assert on boolean expressions
    assert(true,"blah")         // this one fine
    assert(true,1)              // even this is not a string constant
    let(a:string="comment")
        assert(true,a)          // even this is not a string constant
        debug(0,a)              // 30103,   not for debug either
        debug(a,"blah")         // this one good
    let(a:array<int>)
        push(1, 1)              // 30103,   must push to array
        push(a,"a")             // 30103,   must push the same type
        push(a, 1, "at")        // 30103,   index must be index
        push(a, 1)              // this one ok
        push(a, 1, 2)           // this one good too
        erase(1,1)              // 30103,   first argument must be fully qualified array or table
        erase(a,"blah")         // 30303,   size must be int or uint
        erase(a,1)              // this one ok
        find(1,1)               // 30103,   first argument must be fully qualified array or table
        find(a,"1")             // 30103,   alue must be of the same type as array<value>
        find(a,1)               // this one fine
    let(a:table<string,int>)
        erase(a,1)              // 30103,   key must be of the same type as table<key,...>
        erase(a,"2")            // this one ok
        find(a,1)               // 30103,   key must be of the same type as table<key,...>
        find(a,"1")             // this one fine

