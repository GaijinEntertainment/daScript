True when ``program``'s symbol-use pass reached ``function``. The answer is the
program's, not the function's: a shared module's function is used by one program and not
by another compiled in the same process, and a compile nested inside this one leaves the
answer standing.
