
echo "AngelScript"
as.exe fib.as
as.exe primes.as
as.exe particles.as 
rem sq3-modified-64.exe nbodies.nut

echo "AngelScript JiT"
asjit.exe fib.as
asjit.exe primes.as
asjit.exe particles.as 
rem sq3-64.exe nbodies.nut

