bool isprime(int n)
{
    int i;
    for (i = 2; i < n; ++i)
        if (n % i == 0)
            return false;
    return true;
}

int primes(int n)
{
    int count = 0;
    int i;
    for (i = 2; i <= n; ++i)
        if (isprime(i))
            ++count;
    return count;
}

int main(int unused)
{
    const int N = 14000;
    print("primes: ");
    print(primes(N));
    print("\n");
    return 0;
}
