[export]
def test
    let CNT = 0
    let counter = lambda() <| $ (extra:int) : int
        return CNT++ + extra
    let x = invoke(counter,13)
    CNT = 100500
    let y = invoke(counter,13)
	assert(x==13 & y==14)
    return true

