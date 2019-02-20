int fibR(int n)
{
    if (n < 2) return n;
    return (fibR(n-2) + fibR(n-1));
}

int fibI(int n)
{
    int last = 0;
    int cur = 1;
    --n;
    while(n > 0)
    {
        --n;
        int tmp = cur;
        cur = last + cur;
        last = tmp;
    }
    return cur;
}


int main(int unused)
{
    print("fibR: ");
    print(fibR(31));
    print("\nfibI: ");
    print(fibI(6511134));
    print("\n");
    return 0;
}
