// options log = true, optimize = true // , logInferPasses = true, logOptimizationPasses = true


enum Zen
    zero
    not_zero

[export]
def test
	let t = Zen zoo + 1
	assert ( Zen zoo == 0 )
    return true
