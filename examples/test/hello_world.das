// options log = true, logInferPasses = true, logOptimizationPasses = true

enum Numbers
    zero
    one
    two
    ten = 10

[export]
def test
    let t : Numbers
    print("t={t}\n")
    let q = Numbers one
    print("q={q}\n")
    return true
