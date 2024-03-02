int fibR(int n)
{
    if (n < 2) return n;
    return (fibR(n-2) + fibR(n-1));
}

int main(int unused)
{
    print("\"fibbonacci recursive\", ");
    int ctime = GetSystemTime();
    for ( int i=0; i!=20; ++i ) {
        int fr = fibR(31);
        if ( fr!=1346269 ) print("test failed!\n");
    }
    int time = (GetSystemTime()-ctime)/20;
    print (time/1000);
    print (".");
    print(time%1000);
    print(", 20\n");
    return 0;
}
