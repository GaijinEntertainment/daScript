function fibR(n)
{
    if (n < 2) return n;
    return (fibR(n-2) + fibR(n-1));
}

loadfile("profile.nut")()

print("\"fibonacci recursive\", " + profile_it(20, function() {fibR(31)}) + ", 20\n");
