options logCpp=true, log=true, logNodes=true

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
    print("fibI={fibI(6511134)}\n")
    print("fibR={fibR(31)}\n")
    return true

