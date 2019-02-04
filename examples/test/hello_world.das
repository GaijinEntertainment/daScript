options log = true // , optimize = true // , logInferPasses = true, logOptimizationPasses = true

[export]
def test
    let t : int[10]
    for x in range(0,10)
        t[x] = x+1
        if x==5
            break
    finally
        t[9] = 13
        print("t={t}\n")
    print("t={t}\n")
    return true
