def test
    assert(true,"this should pass")
    let
        a:int
    in
        assert(sizeof(a)==4,"integer size must be 4, sizeof understands expressions and types")
        assert(sizeof(int)==4,"integer size must be 4")
