local function fibI(n) {
    local last = 1
    local cur = 0
    do {
        local tmp = cur;
        cur += last;
        last = tmp;
        --n;
    } while(n);
    return cur;
}
local profile_it
try profile_it = ::loadfile("profile.nut")() catch (e) profile_it = require("profile.nut")

print("\"fibonacci loop\", " + profile_it(20, function() {fibI(6511134)}) + ", 20\n");
