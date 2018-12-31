def isprime(n:int):bool
    for i in range(2, n)
        if (n % i == 0)
            return false
    return true



def primes(n:int):int
    let count:int = 0
    for i in range(2, n+1)
        if (isprime(i))
            ++count
    return count;


def test():bool
    profile(2,"primes loop") <|
        primes(50000)
    return true

