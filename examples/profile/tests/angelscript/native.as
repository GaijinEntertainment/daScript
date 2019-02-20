const int iterations = 10000000;

int main(int unused)
{
    int ctime = GetSystemTime();
    int count = 0, i;
    for (i = 0; i < iterations; ++i)
        count = AddOne(count);
    int time=GetSystemTime()-ctime;
    print("count:");
    print(count);
    print("\n");
    print("time taken (msec): ");
    print (time);
    print("\n");
    return 0;
}
