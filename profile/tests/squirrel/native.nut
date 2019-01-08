local function testAdds()
{
    local count = 0;
    for (local i = 0; i < 10000000; ++i)
        count = ::AddOne(count);
    return count
}

loadfile("profile.nut")()

print("native loop: " + profile_it(20, function() {testAdds()}) + "\n");
