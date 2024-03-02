local function expLoop(n)
{
    local sum = 0
    local i;
    for (i = 0; i < n; ++i)
      sum += exp(1./(1.+i))
    return sum
}


loadfile("profile.nut")()

print("\"exp loop\", " + profile_it(20, function() {expLoop(1000000)}) + ", 20\n");
