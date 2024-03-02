local function fibI(n)
{
    local last = 0;
    local cur = 1;
    n = n - 1;
    while(n)
    {
        --n;
        local tmp = cur;
        cur = last + cur;
        last = tmp;
    }
    return cur;
}
loadfile("profile.nut")()

print("\"fibonacci loop\", " + profile_it(20, function() {fibI(6511134)}) + ", 20\n");
