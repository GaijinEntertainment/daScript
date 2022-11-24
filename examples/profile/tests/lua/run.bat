echo "LuaJIT, no JIT"
luajit2_1_0beta3\luajit.exe -joff fibI.lua
luajit2_1_0beta3\luajit.exe -joff primes.lua
luajit2_1_0beta3\luajit.exe -joff particles.lua
luajit2_1_0beta3\luajit.exe -joff native.lua
luajit2_1_0beta3\luajit.exe -joff dict.lua 
luajit2_1_0beta3\luajit.exe -joff profile_try_catch.lua 
luajit2_1_0beta3\luajit.exe -joff exp.lua 
luajit2_1_0beta3\luajit.exe -joff nbodies.lua
luajit2_1_0beta3\luajit.exe -joff sha1.lua
luajit2_1_0beta3\luajit.exe -joff tree.lua
luajit2_1_0beta3\luajit.exe -joff table-sort.lua

echo "LuaJIT, JIT"
luajit2_1_0beta3\luajit.exe fibI.lua
luajit2_1_0beta3\luajit.exe primes.lua
luajit2_1_0beta3\luajit.exe particles.lua
luajit2_1_0beta3\luajit.exe native.lua
luajit2_1_0beta3\luajit.exe dict.lua 
luajit2_1_0beta3\luajit.exe profile_try_catch.lua 
luajit2_1_0beta3\luajit.exe exp.lua 
luajit2_1_0beta3\luajit.exe nbodies.lua
luajit2_1_0beta3\luajit.exe sha1.lua
luajit2_1_0beta3\luajit.exe tree.lua
luajit2_1_0beta3\luajit.exe table-sort.lua

echo "Lua (low res timer, no addOne)"
lua.exe fibI.lua
lua.exe primes.lua
lua.exe particles.lua
lua.exe dict.lua 
lua.exe profile_try_catch.lua 
lua.exe exp.lua 
lua.exe nbodies.lua
lua.exe sha1.lua
lua.exe tree.lua
lua.exe table-sort.lua
