// options log=true, printVarAccess=true, printRef=true

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
        let k = float(i*4)
        sumV += exp(rcp_est(float4(1.+k,2.+k,3.+k,4.+k)))
    return sumV.x + sumV.y + sumV.z + sumV.w

[export]
def test()
    profile(20,"exp loop") <|
        expLoop(1000000)
    profile(20,"exp loop, unrolled") <|
        expLoopU(1000000)
    profile(20,"exp loop, unrolled and vectorized") <|
        expLoopUV(1000000)
    profile(20,"c++ exp loop") <|
        testProfile::testExpLoop(1000000)
    return true
