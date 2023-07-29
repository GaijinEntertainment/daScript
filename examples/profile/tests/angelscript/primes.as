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
    int ctime = GetSystemTime();
    int p = primes(N);
    int time = GetSystemTime()-ctime;
    print(p);
    print("\n");
    print("time taken (msec): ");
    print (time);
    print("\n");
    return 0;
}
