The function ``program``'s whole-program facts prove the method call ``invoke`` always reaches,
so a backend emits it as a direct call of that function; null when the call stays an invoke.
The facts are the program's, filled when it simulates - a shared module's call has its own
answer in every program that requires it.
