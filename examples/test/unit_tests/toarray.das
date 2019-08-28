[export]
def test
    // regular to-array
    let d = [[ int 1; 2; 3; 4 ]]
    let a <- toarray([[ int 1; 2; 3; 4 ]])
    for x,y in d,a
        assert(x==y)
    return true

