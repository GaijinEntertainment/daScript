local exp
try exp = ::exp catch (e) exp = require("math").exp

local function expLoop(n)
{
    local sum = 0
    local i;
    for (i = 0; i < n; ++i)
      sum += exp(1./(1.+i))
    return sum
}


local profile_it
try profile_it = ::loadfile("profile.nut")() catch (e) profile_it = require("profile.nut")

print("\"exp loop\", " + profile_it(20, function() {expLoop(1000000)}) + ", 20\n");
