options log = true, optimize = false // , logInferPasses=true

struct Point
    x,y : int

[export]
def test
    let pP = new [[Point x=1,y=2]]
    print("pP={pP}\n")
    return true
/*
    let x = 2
    let y = 3.0
    let XX : int& = x
    let aa : array<int>
    let z <- def (a:int;b:float) : bool
        return (x==a) & (y==b) & (XX==x)
    ; return true
 */


