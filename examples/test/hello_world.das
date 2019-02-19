options log = true // , optimize = false, printVarAccess=true, printRef=true // , logInferPasses=true


[export]
def test
    let CNT = 0
    let counter <- def (extra:int) : int
        return CNT++ + extra
    ;
    let x = invoke(counter,13)
    let y = invoke(counter,13)
	assert(x==13 & y==14)
    return true



