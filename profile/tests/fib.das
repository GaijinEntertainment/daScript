// options log=true

def fibR(n:int):int
    if (n < 2)
        return n
    else
        return fibR(n - 1) + fibR(n - 2)

def fibI(n:int):int
    let last:int = 0
    let cur:int = 1
    for i in range(0, n-1)
        let tmp:int = cur
        cur += last
        last = tmp
    return cur

def test():bool
    profile(20,"fibbonacci loop") <|
        fibI(6511134)
    profile(20,"fibbonacci recursive") <|
        fibR(31)
    return true

