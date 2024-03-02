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
    print("\"fibbonacci loop\", ");
    int ctime = GetSystemTime();
    for ( int i=0; i!=20; ++i ) {
        int fi = fibI(6511134);
        if ( fi!=1781508648 ) print("test failed\n");
    }
    int time = (GetSystemTime()-ctime)/20;
    print (time/1000);
    print (".");
    print(time%1000);
    print(", 20\n");
    return 0;
}
