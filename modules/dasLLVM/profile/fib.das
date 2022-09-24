require dastest/testing_boost
require llvm/llvm_jit
require testProfile

[jit,sideeffects]
def fibR_jit(n:int)
    if (n < 2)
        return n
    return fibR_jit(n - 1) + fibR_jit(n - 2)

[jit,sideeffects]
def fibI_jit(n:int)
    var last = 0
    var cur = 1
    for i in range( n-1)
        let tmp = cur
        cur += last
        last = tmp
    return cur

[test]
def test_fibR_jit(t: T?)
    t |> success ( is_jit_function(@@fibR_jit) )
    var f3_j = fibR_jit(31)
    t |> equal( f3_j, 1346269)

[test]
def test_fibI_jit(t: T?)
    t |> success ( is_jit_function(@@fibI_jit) )
    var f3_j = fibI_jit(6511134)
    t |> equal( f3_j, 1781508648)

[export]
def main()
    var f4 = 0
    profile(20,"fibbonacci loop c++") <|
        f4 = testProfile::testFibI(6511134)
    assert(f4==1781508648)
    var f5 = 0
    profile(20,"fibbonacci recursive c++") <|
        f5 = testProfile::testFibR(31)
    assert(f5==1346269)

    // fibI_jit(5)

    if true
        var f1_j = 0
        profile(20,"fibbonacci loop - jit") <|
            f1_j = fibI_jit(6511134)
        assert(f1_j==1781508648)
        var f3_j = 0
        profile(20,"fibbonacci recursive - jit") <|
            f3_j = fibR_jit(31)
        assert(f3_j==1346269)

