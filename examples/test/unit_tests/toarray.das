[export,unsafe]
def test
    // regular to-array
    let d = [[ int 1; 2; 3; 4 ]]
    let a <- toarray([[ int 1; 2; 3; 4 ]])
    for x,y in d,a
        assert(x==y)
    // range
    for x,y in range(0,5),each(range(0,5))
        assert(x==y)
    for x,y in range(0,5),toarray(each(range(0,5)))
        assert(x==y)
    // static array
    for x,y in d,each(d)
        assert(x==y)
    for x,y in d,toarray(d)
        assert(x==y)
    for x,y in d,toarray(each(d))
        assert(x==y)
    // dynamic array
    for x,y in a,each(a)
        assert(x==y)
    for x,y in a,toarray(each(a))
        assert(x==y)
    return true

