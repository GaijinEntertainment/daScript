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
    int ctime = GetSystemTime();
    int fr = fibR(31);
    int time = GetSystemTime()-ctime;
    print(fr);
    print("\ntime taken (msec): ");
    print (time);
    print("\n");
    
    print("\nfibI: ");
    ctime = GetSystemTime();
    int fi = fibI(6511134);
    time = GetSystemTime()-ctime;
    print(fi);
    print("\ntime taken (msec): ");
    print (time);
    print("\n");
    return 0;
}
