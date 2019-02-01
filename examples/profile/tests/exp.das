// options log=true

require math
require testProfile

[sideeffects]
def expLoop(n)
    let sum = 0.
    for i in range(0, n)
        sum += exp(1./(1. + float(i)))
    return sum

[sideeffects]
def expLoopU(n)
    let sum = 0.
    for i in range(0, n/4)
        let k = float(i*4)
        sum += exp(1./(1. + k)) + exp(1./(2. + k)) + exp(1./(3. + k)) + exp(1./(4. + k))
    return sum

[export]
def test()
    profile(20,"exp loop") <|
        expLoop(1000000)
    profile(20,"exp loop, unrolled") <|
        expLoopU(1000000)
    profile(20,"c++ exp loop") <|
        testProfile::testExpLoop(1000000)
    return true
