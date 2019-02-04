// options log = true, optimize = true // , logInferPasses = true, logOptimizationPasses = true

let
    g = 5

[run]
def add13(n)
    g = n
    return n + 13

[export]
def test
    print("{add13(3)}\n")
    return true
