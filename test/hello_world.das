options log=true,optimize=false

def test:bool
    let (x,y=2)
        assert(x+y==4)
    return true
