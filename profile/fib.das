def fibR(n:int):int
    if (n < 2)
        return n
    return fibR(n - 1) + fibR(n - 2)

def fibI(n:int):int
    let last:int = 0
    let cur:int = 1
    for i in range(0, n-1)
        let tmp:int = cur
        cur = last + cur
        last = tmp
    return cur

def test():bool
    profile(4,"fibbonacci loop") <|
        fibI(11111134)
    //profile(1,"fibbonacci recursive") <|
    //    fibR(34)
    return true

