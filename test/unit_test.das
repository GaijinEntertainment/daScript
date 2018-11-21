// this is where unit tests are
def test
    assert(true,"this should pass")
    // verifying if sizeof accepts expression, not just type
    let
        a:int
    in
        assert(sizeof(a)==4,"integer size must be 4, sizeof understands expressions and types")
    // verifying type sizes
    assert(sizeof(int)==4,"integer size must be 4")
