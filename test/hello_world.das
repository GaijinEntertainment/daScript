options log=true,optimize=false

def test
    let(a:string="comment")
        assert(true,a)          // even this is not a string constant
