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
test file no lane names). `run_tests_llvm_tune_slow` - the full tuner runs in
`llvm_tune_scope.das` - runs only in the nightly extended checks; every other lane also runs per
PR. The generated index of every test here is `TESTS.md`; the shape rules a test file follows are
`tests/ARCHITECTURE.md` at the repo root, applied by `REVIEW.das` beside this file.

A file whose name starts with `_` (a program root a test builds or runs through a child daslang, a
client, a library a fixture requires) is a child-process fixture, never a test of its own.
