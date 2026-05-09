---
slug: how-do-i-identify-which-test-is-hanging-when-dastest-jit-times-out-in-ci
title: How do I identify which test is hanging when dastest -jit times out in CI?
created: 2026-05-09
last_verified: 2026-05-09
links: []
---

**Problem.** A test wedges (often LLVM JIT codegen on an unfamiliar arch) and dastest prints `[E] Test timed out after 600s` plus a stack trace, but no test name. With `--failures-only` (the default in CI), only failed tests log a name — and a timeout never gets the chance to log its own name.

**Diagnostic.** Drop `--failures-only` on the JIT invocation for the affected platform. dastest non-isolated mode runs files sequentially and emits `PASS uri ...` / `FAIL uri ...` after each `test_file()` returns. The most recent `PASS` line names the file that finished just before the hung one — the next file in the discovery order is the culprit.

In `release.yml` / `build.yml`, this looks like:

```yaml
linux_arm64)
  cd bin
  ./daslang _dasroot_/dastest/dastest.das -jit -- --color --timeout 900 --test ../tests
  # (no --failures-only on the JIT line; PASS lines now print)
  ./daslang _dasroot_/dastest/dastest.das -- --color --failures-only --timeout 900 --test ../tests
  ;;
```

**Why it works.** dastest.das at the loop in `dastest.das:369-388` does `for file in files { ... log::green("PASS {uri}") | log::red("FAIL {uri}") }`. The PASS branch is gated on `log::failuresOnly` from `test_args.failures_only`. Drop the flag, get the names.

**When NOT to do this everywhere.** PASS-flooding the log makes it harder to scan for the actual failing test on platforms where things work. Apply per-platform via the `case` statement on `${target}${architecture}`.

**Other anchors.** dastest's `--isolated-mode` runs each test in its own process and prints \"Running N tests in isolated mode with M threads\" plus per-process exit codes — that's the other path to per-test visibility, but slower.

## Questions
- How do I identify which test is hanging when dastest -jit times out in CI?
