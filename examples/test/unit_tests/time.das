def testClockArg(c:clock):clock
    let d = c
    return d

[export]
def test
    let a1,a2:clock
    let dt:float
    a1 = getClock()
    for z in range(0,3) // nada on my laptop
        let a,b:int[3]
        for x,y in a,b
            x += y
    a2 = getClock()
    assert(a1 <= a2)
    let a3 = testClockArg(a2)
    assert(a2 == a3)
    dt = a2 - a1
    assert(dt <= 1.0)
    return true
