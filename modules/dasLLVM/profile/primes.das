require llvm/llvm_jit

[jit,sideeffects]
def isprime_jit(n:int)
    for i in range(2, n)
        if (n % i == 0)
            return false
    return true

[jit,sideeffects]
def primes_jit(n:int)
    var count = 0
    for i in range(2, n+1)
        if isprime_jit(i)
            ++count
    return count

[jit,sideeffects]
def primesI_jit(n:int)
    var count = 0
    for i in range(2, n+1)
		count ++
		for j in range(2,i)
			if i % j == 0
				count --
				break
    return count

[export]
def main()
    if true
        var f1j = 0
        profile(20,"primes loop - jit") <|
            f1j = primes_jit(14000)
        assert(f1j==1652)
        var f2j = 0
        profile(20,"primes loop, inline - jit") <|
            f2j = primesI_jit(14000)
        assert(f2j==1652)

