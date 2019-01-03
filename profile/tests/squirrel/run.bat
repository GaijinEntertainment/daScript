
echo "Squirrel3 modified"
copy profile-modified.nut profile.nut
sq3-modified-64.exe fib.nut
sq3-modified-64.exe primes.nut
sq3-modified-64.exe particles.nut
sq3-modified-64.exe native.nut
sq3-modified-64.exe profile_try_catch.nut 

echo "Squirrel3 original"
copy profile-original.nut profile.nut
sq3-64.exe fib.nut
sq3-64.exe primes.nut
sq3-64.exe particles.nut
rem sq3-64.exe native.nut
