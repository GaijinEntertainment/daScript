expect 39901:2, 39902    //  sizeof_reference

def blah(a:auto(numt)&)
    return sizeof(type numt)    // 39901: szieof(ref) is prohibited

def test
    sizeof(type int&)           // 39901: szieof(ref) is prohibited
    let t = 1
    blah(t)
    sizeof(type auto)           // 39902: sizeof(auto) is undefined

