local function testAdds()
{
    local count = 0;
    for (local i = 0; i < 10000000; ++i)
        count = ::AddOne(count)
    return count
}

local profile_it
try profile_it = ::loadfile("profile.nut")() catch (e) profile_it = require("profile.nut")

print("\"native loop\", " + profile_it(20, function() {testAdds()}) + ", 20\n");
