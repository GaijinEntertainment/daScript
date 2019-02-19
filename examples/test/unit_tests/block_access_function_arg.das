def addValue(a,blk)
    return invoke(blk,a)

[export]
def test
    let CNT = 0
    let counter = lambda() <| $ (extra:int) : int
        let e1 = addValue(1) <| $ (extraBlock:int) : int
            return extraBlock + CNT
        return CNT++ + e1
    assert(invoke(counter,0)==1)
    return true

