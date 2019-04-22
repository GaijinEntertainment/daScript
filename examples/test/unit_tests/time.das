[export]
def test
    var a1,a2:clock
    var dt:float
    a1 = getClock()
    for z in range(0,3) // nada on my laptop
        var a,b:int[3]
        for x,y in a,b
            x += y
    a2 = getClock()
    assert(a1 <= a2)
    dt = a2 - a1
    assert(dt <= 1.0)
    return true


