# dasLLVM tune tests

These module-owned tests cover the `llvm_tune` permutation, manifest, policy, tuner,
and re-execution integration paths. They are intentionally outside the core `tests/jit_tests`
sweep because several cases spawn nested `daslang` processes and full JIT compilations.

Run the suite explicitly from the repository root, on an LLVM-enabled build
(`-DDAS_LLVM_DISABLED=OFF`):

```text
bin/Release/daslang.exe dastest/dastest.das -jit -- --timing-outliers 10 --test modules/dasLLVM/tests
```

CI runs every test file here through the `das_lane_dastest_target` lanes in `../CMakeLists.txt`,
which list their files one by one; a new test file joins one of them (`../REVIEW.das` reports a
test file no lane names).

Files ending in `_client`, `_client_lib`, `_lib`, `_tuner`, or `_root` (a program root a test builds
or runs through a child daslang) are child-process fixtures rather than standalone test cases.
