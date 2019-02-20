const int iterations = 10000000;

int main(int unused)
{
    int count = 0, i;
    for (i = 0; i < iterations; ++i)
        count = AddOne(count);

    print("count:");
    print(count);
    print("\n");
    return 0;
}
