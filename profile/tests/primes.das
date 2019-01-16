require testProfile

def isprime(n)
    for i in range(2, n)
        if (n % i == 0)
            return false
    return true

def primes(n)
    let count = 0
    for i in range(2, n+1)
        if (isprime(i))
            ++count
    return count;

[export]
def test()
    profile(20,"primes loop") <|
        primes(14000)
    profile(20,"primes loop C++") <|
        testProfile::testPrimes(14000)
    return true

