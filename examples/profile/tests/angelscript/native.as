const int iterations = 10000000;

int main(int unused)
{
    int ctime = GetSystemTime();
    for ( int j=0; j!=20; ++j ) {
        int count = 0, i;
        for (i = 0; i < iterations; ++i)
            count = AddOne(count);
        if ( count != 10000000 ) print("test failed\n");
    }
    int time = (GetSystemTime()-ctime)/20;
    print("\"native loop\", ");
    print (time/1000);
    print (".");
    print(time%1000);
    print(", 20\n");
    return 0;
}
