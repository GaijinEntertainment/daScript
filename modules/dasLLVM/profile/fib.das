require llvm/llvm_jit

[jit,sideeffects]
def fibR_jit(n)
    if n < 2
        return n
    return fibR_jit(n - 1) + fibR_jit(n - 2)

[jit,sideeffects]
def fibI_jit(n:int)
    var last = 0
    var cur = 1
    for i in range(n-1)
        let tmp = cur
        cur += last
        last = tmp
    return cur

[export]
def main()
    var f1_j = 0
    profile(20,"fibbonacci loop - jit") <|
        f1_j = fibI_jit(6511134)
    assert(f1_j==1781508648)
    var f3_j = 0
    profile(20,"fibbonacci recursive - jit") <|
        f3_j = fibR_jit(31)
    assert(f3_j==1346269)

