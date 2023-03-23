require llvm/llvm_jit
// options log=true, log_nodes=true, print_var_access=true, print_ref=true

require math

[jit,sideeffects]
def expLoop(n)
    var sum = 0.
    for i in range(n)
        sum += exp(rcp_est(float(i+1)))
    return sum

[jit,sideeffects]
def expLoopU(n)
    var sum = 0.
    for i in range( n/4)
        let k = float(i*4)
        sum += exp(rcp_est(1. + k)) + exp(rcp_est(2. + k)) + exp(rcp_est(3. + k)) + exp(rcp_est(4. + k))
    return sum

[jit,sideeffects]
def expLoopUV(n)
    var sumV = float4(0.)
    for i in range(n/4)
		sumV += exp(rcp_est(float4(i*16)+float4(1,2,3,4)))
    return sumV.x + sumV.y + sumV.z + sumV.w

def verifyExp(f)
	let t = 1e+06
	let eps = 10.
	let q = abs(f-t)
	assert(q < eps)

[export]
def main
	var f1 = 0.
    profile(20,"exp loop - jit") <|
        f1 = expLoop(1000000)
	verifyExp(f1)
	var f2 = 0.
    profile(20,"exp loop, unrolled - jit") <|
        f2 = expLoopU(1000000)
	verifyExp(f2)
	var f3 = 0.
    profile(20,"exp loop, unrolled and vectorized - jit") <|
        f3 = expLoopUV(1000000)
	verifyExp(f3)
