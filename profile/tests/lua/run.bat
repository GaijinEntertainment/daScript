echo "LuaJit, no jit"
luajit2_1_0beta3\luajit.exe -joff fibI.lua
luajit2_1_0beta3\luajit.exe -joff primes.lua
luajit2_1_0beta3\luajit.exe -joff particles.lua
luajit2_1_0beta3\luajit.exe -joff native.lua
luajit2_1_0beta3\luajit.exe -joff dict.lua 
luajit2_1_0beta3\luajit.exe -joff profile_try_catch.lua 

echo "LuaJit, jit"
luajit2_1_0beta3\luajit.exe fibI.lua
luajit2_1_0beta3\luajit.exe primes.lua
luajit2_1_0beta3\luajit.exe particles.lua
luajit2_1_0beta3\luajit.exe native.lua
luajit2_1_0beta3\luajit.exe dict.lua 
luajit2_1_0beta3\luajit.exe profile_try_catch.lua 

echo "Lua (low res timer, no addOne)"
lua.exe fibI.lua
lua.exe primes.lua
lua.exe particles.lua
lua.exe dict.lua 
lua.exe profile_try_catch.lua 
