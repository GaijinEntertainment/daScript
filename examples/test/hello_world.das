options logCpp=true, logNodes=false

require math

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

[export]
def test()
    let l1 = 0.
    let l2 = 0.
    let l3 = 0.
    profile(20,"exp loop") <|
        l1 = expLoop(1000000)
    profile(20,"exp loop, unrolled") <|
        l2 = expLoopU(1000000)
    profile(20,"exp loop, unrolled and vectorized") <|
        l3 = expLoopUV(1000000)
    print("{l1} {l2} {l3}\n")
    return true





