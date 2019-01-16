echo "LuaJit, no jit"
luajit.exe -joff fibI.lua
luajit.exe -joff primes.lua
luajit.exe -joff particles.lua
luajit.exe -joff native.lua
luajit.exe -joff dict.lua 
luajit.exe -joff profile_try_catch.lua 

echo "LuaJit, jit"
luajit.exe fibI.lua
luajit.exe primes.lua
luajit.exe particles.lua
luajit.exe native.lua
luajit.exe dict.lua 
luajit.exe profile_try_catch.lua 

echo "Lua (low res timer, no addOne)"
lua.exe fibI.lua
lua.exe primes.lua
lua.exe particles.lua
lua.exe dict.lua 
lua.exe profile_try_catch.lua 
