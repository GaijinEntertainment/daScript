options log = true // , logInferPasses=true // , optimize = false, printVarAccess=true, printRef=true //

struct QQ
    x,y : int

[export]
def test
    let CNT = 0
    let counter = lambda() <| $ (extra:int) : int
        return CNT++ + extra
    let x = invoke(counter,13)
    CNT = 100500
    let y = invoke(counter,13)
	assert(x==13 & y==14)
    // let l : lambda<bool>
    // invoke(l)
    CNT = 1
    let counterQQ = lambda() <| $ (extra:int)
        return [[QQ x=CNT++, y=extra]]
    print("counterQQ={invoke(counterQQ,1)}\n")
    print("counterQQ={invoke(counterQQ,1)}\n")
    print("counterQQ={invoke(counterQQ,1)}\n")
    return true



