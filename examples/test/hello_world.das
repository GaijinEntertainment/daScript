options logCpp=true

[sideeffects]
def fibR(n)
    if (n < 2)
        return n
	return fibR(n - 1) + fibR(n - 2)

[sideeffects]
def fibI(n)
    let last = 0
    let cur = 1
    for i in range(0, n-1)
        let tmp = cur
        cur += last
        last = tmp
    return cur

[export]
def test()
    profile(20,"fibbonacci loop") <|
        fibI(6511134)
    profile(20,"fibbonacci recursive") <|
        fibR(31)
    return true

