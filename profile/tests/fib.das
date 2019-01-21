// options log=true,logInferPasses=true

require testProfile

def fibR(n)
    if (n < 2)
        return n
    else
        return fibR(n - 1) + fibR(n - 2)

[fastcall]
def fibRF(n) : int
    return (n < 2) ? n : (fibRF(n - 1) + fibRF(n - 2))

def fibI(n)
    let last = 0
    let cur = 1
    for i in range(0, n-1)
        let tmp = cur
        cur += last
        last = tmp
    return cur

def fibIU(n)
    let last = 0
    let cur = 1
    for i in range(0, (n-1)/4)
        let tmp1 = cur
        cur += last
        let tmp2 = cur
        cur += tmp1
        let tmp3 = cur
        cur += tmp2
        let tmp4 = cur
        cur += tmp3
        last = tmp4
    return cur

[export]
def test()
    profile(20,"fibbonacci loop") <|
        fibI(6511134)
    profile(20,"fibbonacci loop, unrolled") <|
        fibIU(6511134)
    profile(20,"fibbonacci recursive") <|
        fibR(31)
    profile(20,"fibbonacci recursive [fastcall]") <|
        fibRF(31)
    profile(20,"fibbonacci loop c++") <|
        testProfile::testFibI(6511134)
    profile(20,"fibbonacci recursive c++") <|
        testProfile::testFibR(31)
    return true

