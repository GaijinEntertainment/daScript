// options log=true, logNodes=true, printVarAccess=true, printRef=true

require math
require testProfile

[sideeffects]
def expLoop(n)
    let sum = 0.
    for i in range(0, n)
        sum += exp(rcp_est(1. + float(i)))
    return sum

[sideeffects]
def expLoopU(n)
    let sum = 0.
    for i in range(0, n/4)
        let k = float(i*4)
        sum += exp(rcp_est(1. + k)) + exp(rcp_est(2. + k)) + exp(rcp_est(3. + k)) + exp(rcp_est(4. + k))
    return sum

[sideeffects]
def expLoopUV(n)
    let sumV = float4(0.)
    for i in range(0, n/4)
		sumV += exp(rcp_est(float4(i*16)+float4(1,2,3,4)))
    return sumV.x + sumV.y + sumV.z + sumV.w

def verify(f)
	let t = 1e+06
	let eps = 10.
	let q = abs(f-t)
	assert(q < eps)

[export]
def test()
	let f1 = 0.
    profile(20,"exp loop") <|
        f1 = expLoop(1000000)
	verify(f1)
	let f2 = 0.
    profile(20,"exp loop, unrolled") <|
        f2 = expLoopU(1000000)
	verify(f2)
	let f3 = 0.
    profile(20,"exp loop, unrolled and vectorized") <|
        f3 = expLoopUV(1000000)
	verify(f3)
	let f4 = 0.
    profile(20,"c++ exp loop") <|
        f4 = testProfile::testExpLoop(1000000)
	verify(f4)
    return true
