options log = true // , optimize = false, printVarAccess=true, printRef=true //

def addValue(a,blk)
    return invoke(blk,a)

[export]
def test
    let CNT = 0
    let counter = lambda() <| $ (extra:int) : int
        debug(__this,"in labda __this = ")
        let e1 = addValue(1) <| $ (extraBlock:int) : int
            debug(__this,"in block __this = ")
            return extraBlock + CNT
        return CNT++ + e1
    print("counter={invoke(counter,0)}\n")
    return true



