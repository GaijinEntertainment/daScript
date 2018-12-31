const iterations = 1000000000;


local count = 0;
for (local i = 0; i < iterations; ++i)
    count = ::AddOne(count);


print("Count: " + count + "\n");
