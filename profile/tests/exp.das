require math
def expLoop(n)
    let sum = 0.
    for i in range(0, n)
        sum += exp(1./(1. + float(i)))
    return sum

[export]
def test()
    profile(20,"exp loop") <|
        expLoop(1000000)
    return true
