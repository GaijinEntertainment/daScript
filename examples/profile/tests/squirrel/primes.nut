local function isprime(n)
{
    for (local i = 2; i < n; ++i)
        if (n % i == 0)
            return false;
    return true;
}



local function primes(n)
{
    local count = 0;
    for (local i = 2; i <= n; ++i)
        if (isprime(i))
            ++count;
    return count;
}


local profile_it
try profile_it = ::loadfile("profile.nut")() catch (e) profile_it = require("profile.nut")

print("\"primes loop\", " + profile_it(20, function() {primes(14000)}) + ", 20\n");
