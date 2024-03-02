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
    print("\"primes loop\", ");
    int ctime = GetSystemTime();
    for ( int j=0; j!=20; ++j ) {
        int p = primes(N);
        if ( p!=1652 ) print("test failed!\n");
    }
    int time = (GetSystemTime()-ctime)/20;
    print (time/1000);
    print (".");
    print(time%1000);
    print(", 20\n");
    return 0;
}
