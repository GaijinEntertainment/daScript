# preflight

Run CI's gates locally before pushing. The CI-lane ↔ gate mapping and the
manual commands this tool automates live in
[skills/preflight.md](https://github.com/GaijinEntertainment/daScript/blob/master/skills/preflight.md).

```bash
# fast tier (seconds): format --verify, lint changed .das, clang frontend pass
# on changed C++ (full src+tests-cpp sweep when a header changed)
daslang utils/preflight/main.das

# full tier: adds dasgen freshness, CI-only-das compile sweep, the six doc
# gates, ctest -L small, interpreter/JIT/AOT suites, sequence smoke
daslang utils/preflight/main.das -- --full

# subset / introspection
daslang utils/preflight/main.das -- --list-gates
daslang utils/preflight/main.das -- --only docs,ci-das
daslang utils/preflight/main.das -- --skip tests-aot --full
```

Cross-platform (Windows / macOS / Linux+WSL): subprocesses go through
`popen_argv` (no shell) — except the sequence gate, which by design runs CI's
own smoke scripts under pwsh/bash. The C++ pass uses `clang-cl /Zs` on
Windows (preferring the VS-bundled clang — the same binary CI's ClangCL
toolset uses) and `clang -fsyntax-only` elsewhere; both are frontend-only
(parse + semantic analysis + template instantiation, no codegen), which keeps
even the full ~160-TU header-change sweep at ~15-30 s. A gate whose host tool
or module is missing reports `SKIP` with an install/rebuild hint instead of
passing silently. Exit code is non-zero when any gate fails.

`ci_only_das.txt` lists the in-repo das surface that no default local build
compiles (dasOpenGL today); see the header comment there before adding
entries — surfaces that pull external daspkg packages belong to the
`sequence` gate, not the compile sweep.
