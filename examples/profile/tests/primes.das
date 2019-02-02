// options log=true, printVarAccess=true, printRef=true

require testProfile

def isprime(n)
    for i in range(2, n)
        if (n % i == 0)
            return false
    return true

[sideeffects]
def primes(n)
    let count = 0
    for i in range(2, n+1)
        if (isprime(i))
            ++count
    return count

[sideeffects]
def primesI(n)
    let count = 0
    for i in range(2, n+1)
		count ++
		for j in range(2,i)
			if i % j == 0
				count --
				break
    return count

[export]
def test()
    profile(20,"primes loop") <|
        primes(14000)
    profile(20,"primes loop, inline") <|
        primesI(14000)
    profile(20,"primes loop C++") <|
        testProfile::testPrimes(14000)
    return true

