options log=true, optimize=false

def interval(t1,t2:clock):float
    return t2 - t1

[export]
def test
    let(a1,a2:clock;dt:float)
        a1 = getClock()
        for z in range(0,30000) // ~1 sec on my laptop
            let(a,b:int[1000])
                for x,y in a,b
                    x += y
        a2 = getClock()
        assert(a1 <= a2)
        dt = interval(a1,a2)
        print("\na1 = {a1}\na2 = {a2}\ndt = {dt}\n")
    return true

