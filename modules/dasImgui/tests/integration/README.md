# dasImgui integration tests

Two complementary test surfaces live in this directory.

## 1. dastest integration suite — daslang-native, full coverage

`test_*.das` files exercised by daslang's `dastest` runner. Each test spawns
`daslang-live` against a feature app under `../../examples/features/` (or
`../../examples/save_demo/`), drives it via `require imgui/imgui_playwright`,
and asserts behavior. This is the canonical regression suite.

Run from the daScript repo root:

```
daslang.exe dastest/dastest.das -- --test modules/dasImgui/tests/integration
```

Expect ~28 tests, ~130s wall time on a developer box (one daslang-live
subprocess per test, ~4-5s each).

## 2. Curl smoke — mechanism-level, tool-agnostic

`smoke_curl.ps1` (Windows) and `smoke_curl.sh` (POSIX) drive
`examples/save_demo/main.das` entirely via raw HTTP. NO playwright module,
NO dastest. They cover one happy path: spawn → ready → snapshot → click
SAVE_BTN → poll STATUS_TEXT.value == "saved" → shutdown.

```
powershell -ExecutionPolicy Bypass -File modules/dasImgui/tests/integration/smoke_curl.ps1
bash       modules/dasImgui/tests/integration/smoke_curl.sh
```

`smoke_curl.sh` requires `curl` and `jq` on PATH.

### Which one to run when

- **Most days**: the dastest suite. Full surface, fast feedback per test.
- **A dastest failure that doesn't make sense**: run the curl smoke. If it
  PASSes, the bug is somewhere in `widgets/imgui_playwright.das` or the
  test's wait/assertion logic. If the smoke FAILs too, the bug is deeper
  — daslang-live's dispatch, dasImgui's server-side `imgui_*` commands,
  or the playwright transport's HTTP layer.
- **Onboarding a non-daslang tool** (LLM-driven UI tester, headless probe,
  custom IDE integration, etc.): the curl smoke is the documented shape.
  Read it as the contract for "what a third-party client should do."

## Notes

- Tests are dastest-discovered by `test_*.das` filename. Files starting
  with `_` are skipped (see `feedback_dastest_underscore_skip`).
- The harness is `require imgui/imgui_playwright public`, declared in
  `widgets/imgui_playwright.das` and registered via `.das_module`.
- Feature apps used by `type_text`, `drag`, or other coroutine-based
  commands MUST call `advance_coroutines()` each frame — see the three
  `io_synth_*` / `await_quiescent` examples and `save_demo` for the pattern.
