options log = true, optimize = false // , logInferPasses=true

[export]
def test
    let x = 2
    let y = 3.0
    let XX : int& = x
    let z = def (a:int;b:float) : bool
        return (x==a) & (y==b) & (XX==x)
    ; return true


