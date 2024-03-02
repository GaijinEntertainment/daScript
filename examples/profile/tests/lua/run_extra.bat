echo "LuaJIT, no JIT"
luajit2_1_0beta3\luajit.exe -joff profile_try_catch.lua 
luajit2_1_0beta3\luajit.exe -joff sha1.lua
luajit2_1_0beta3\luajit.exe -joff table-sort.lua
luajit2_1_0beta3\luajit.exe -joff table-sort-cmp.lua

echo "LuaJIT, JIT"
luajit2_1_0beta3\luajit.exe profile_try_catch.lua 
luajit2_1_0beta3\luajit.exe sha1.lua
luajit2_1_0beta3\luajit.exe table-sort.lua
luajit2_1_0beta3\luajit.exe table-sort-cmp.lua

echo "Lua (low res timer, no addOne)"
lua.exe profile_try_catch.lua 
lua.exe sha1.lua
lua.exe table-sort.lua
