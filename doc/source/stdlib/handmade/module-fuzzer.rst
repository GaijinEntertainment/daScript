The FUZZER module implements fuzz testing infrastructure for daScript programs.
It generates random inputs for functions and verifies they do not crash or
produce unexpected errors, helping discover edge cases and robustness issues.

All functions and symbols are in "fuzzer" module, use require to get access to it. ::

    require daslib/fuzzer
