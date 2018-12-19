require UnitTest

def interval(t1,t2:clock):float
    return t2 - t1

def test:bool
    let(a1,a2:clock;dt:float)
        a1 = getClock()
        print(string(a1)+"\n")
        for z in range(0,30000) // ~1 sec on my laptop
            let(a,b:int[1000])
                for x,y in a,b
                    x += y
        a2 = getClock()
        assert(a1 <= a2)
        dt = interval(a1,a2)
        print("dt = " + string(dt) + "\n")
    let(a,b:int=2)
        testAdd(a,b)
        assert(a==4)
    return true
