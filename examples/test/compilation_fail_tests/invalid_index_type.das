expect 30110:3    // invalid_index_type

def test
    let(a:int[10])
        a["blah"]   // index is int or uint
        a[0]        // this one ok
    let(a:array<string>)
        a["blah"]   // index is int or uint
        a[0]        // this one ok
    let(a:table<string,int>)
        a["blah"]   // this one ok
        a[0]        // index type mismatch
