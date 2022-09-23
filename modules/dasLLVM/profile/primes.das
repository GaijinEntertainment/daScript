require dastest/testing_boost
require llvm/llvm_jit
require testProfile

[jit_llvm,sideeffects]
def isprime_jit(n:int)
    for i in range(2, n)
        if (n % i == 0)
            return false
    return true

[jit_llvm,sideeffects]
def primes_jit(n:int)
    var count = 0
    for i in range(2, n+1)
        if isprime_jit(i)
            ++count
    return count

[jit_llvm,sideeffects]
def primesI_jit(n:int)
    var count = 0
    for i in range(2, n+1)
		count ++
		for j in range(2,i)
			if i % j == 0
				count --
				break
    return count

[test]
def test_primes(t: T?)
    if true
        t |> success ( is_jit_function(@@primes_jit) )
        t |> success ( is_jit_function(@@primesI_jit) )
        for vv in range(1,14)
            let v = vv * 100
            let R = testProfile::testPrimes(v)
            let pj = primes_jit(v)
            t |> equal( R, pj)
            let pji  = primesI_jit(v)
            t |> equal( R, pji)

[export]
def main()
	var f3 = 0
    profile(20,"primes loop C++") <|
        f3 = testProfile::testPrimes(14000)
	assert(f3==1652)
    if true
        var f1j = 0
        profile(20,"primes loop - jit") <|
            f1j = primes_jit(14000)
        assert(f1j==1652)
        var f2j = 0
        profile(20,"primes loop, inline - jit") <|
            f2j = primesI_jit(14000)
        assert(f2j==1652)

