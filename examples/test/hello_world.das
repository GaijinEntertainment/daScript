// options log = true, logCpp=true
// options logInferPasses=true

let
    TOTAL = 5

[export]
def test
    var a : int[TOTAL+3]
    for t in range(0,TOTAL+3)
        a[t] = t
    print("a={a}\n")
    return true
