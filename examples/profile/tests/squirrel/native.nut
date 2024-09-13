function testAdds()
{
    local count = 0;
    for (local i = 0; i < 10000000; ++i)
        count = ::AddOne(count)
    return count
}

local profile_it
try {
  profile_it = getroottable()["loadfile"]("profile.nut")()
  if (profile_it == null)
    throw "no loadfile"
} catch(e) profile_it = require("profile.nut")

print("\"native loop\", " + profile_it(20, function() {testAdds()}) + ", 20\n");
