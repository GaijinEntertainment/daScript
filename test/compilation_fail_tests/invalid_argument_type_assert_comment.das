expect 30103    // invalid_argument_type

def test
    assert(true,12)             // not a string constant
    let(a:string="comment")
        assert(true,a)          // even this is not a string constant
