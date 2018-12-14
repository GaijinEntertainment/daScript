def twice(b:block)
    invoke(b)
    invoke(b)

def twice_good(b:block<int>):int
    let(t:int)
        t += invoke(b)
        t += invoke(b)
        return t

def test:bool
    let(a,b:int=0)
        twice({ a++; })
        assert(a==2)
        b = twice_good({ a++; })
        assert(a==4 & b==(2+3))
    return true

