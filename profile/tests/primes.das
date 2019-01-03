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
    profile(20,"primes loop") <|
        primes(14000)
    return true

