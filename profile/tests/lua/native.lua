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

io.write(string.format("native interop loop: %.8f\n", profile_it(20, function () testAdds() end)))

