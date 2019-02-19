options log = true, optimize = false // , logInferPasses=true


[export]
def test
    let CNT = 0
    let counter <- def (extra:int) : int
        return CNT++ + extra
    ;
    let x = invoke(counter,13)
    let y = invoke(counter,13)
    print("x={x} y={y}\n")
    return true



