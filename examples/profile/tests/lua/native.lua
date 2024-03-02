local ffi = require("ffi")
ffi.cdef[[
int addOne(int);
]]

local function testAdds()
    local count = 0
    for i = 1, 10000000 do
        count = ffi.C.addOne(count)
    end
    return count
end

loadfile("profile.lua")()

io.write(string.format("\"native loop\", %.8f, 20\n", profile_it(20, function () testAdds() end)))

