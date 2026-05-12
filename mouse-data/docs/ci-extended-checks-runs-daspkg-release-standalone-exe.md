---
slug: ci-extended-checks-runs-daspkg-release-standalone-exe
title: What does the CI `extended_checks` lane actually run, and why do shared-module problems only surface there?
created: 2026-05-12
last_verified: 2026-05-12
links: []
---

CI's `extended_checks (linux, 64)` and `(darwin15, arm64)` jobs (defined in the workflow that fires alongside the standard build matrix) do a thing the regular `dastest` / `test_aot` jobs don't: they build a **standalone exe** via `daspkg release`.

Concretely:
1. `cmake --build ./build --config Release --target dasModuleGlfw dasModuleLiveHost dasModuleHV dasModuleAudio dasModulePUGIXML dasModuleStbImage --parallel` — bring up the runtime-loaded dynamic modules.
2. Run `examples/games/sequence/ci_smoke_test.sh` (linux: under `xvfb-run -a` for headless display; macOS: bare).
3. The smoke test does `daspkg install` + `daspkg release` against the `examples/games/sequence` package. `daspkg release` invokes `daslang -exe` which JIT-compiles the entire transitive `require` closure into a standalone exe.

This is the **only** path that exercises:
- Shared-module dependency type checks (`error[20115]` — shared module requiring native-path is rejected at exe-build time but not at dastest time).
- `daslang -exe` LLVM JIT lowering of every transitively-required module.
- AOT-on-the-fly for daslib + every dynamic module.

If the standard `dastest` and `test_aot` pass but `extended_checks` fails with `error[20115]: ... incorrect dependency type`, the issue is almost certainly an inappropriate cross-type require introduced somewhere in the changed module's `.das_module` setup. Local repro: `bin/daslang -exe -output /tmp/foo.exe examples/games/sequence/main.das`.

Adjacent gotcha — `extended_checks` is also where CI's stricter `das-fmt` runs (see `skills/make_pr.md` step 5 / `feedback_ci_das_fmt_differs.md`).

## Questions
- What does the CI `extended_checks` lane actually run, and why do shared-module problems only surface there?
