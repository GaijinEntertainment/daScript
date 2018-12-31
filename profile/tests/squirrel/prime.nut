function isprime(n)
{
    local i;
    for (i = 2; i < n; ++i)
        if (n % i == 0)
            return false;
    return true;
}



function primes(n)
{
    local count = 0;
    local i;
    for (i = 2; i <= n; ++i)
        if (isprime(i))
            ++count;
    return count;
}




local N = 50000;
print("primes: " + primes(N) + "\n");
