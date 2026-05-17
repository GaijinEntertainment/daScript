---
slug: imgui-playwright-windows-ci-16-post-libhv-stall
title: Why do imgui playwright tests hang at 120 seconds on Windows CI when they pass locally and on POSIX?
created: 2026-05-16
last_verified: 2026-05-16
links: []
---

**dasHV's libhv build on Windows CI stalls after exactly 16 POST /command connections per subprocess.** Local Windows works fine; only the GitHub-hosted `windows-latest` runner trips it. Discovered while resurrecting dasImgui integration tests in PR #38 (May 2026).

**Symptom**: a test does `with_imgui_app` → spawns daslang-live → fast burst of 1 GET /status + 16 POST /command (all 200 OK in <300 ms), then the libhv event loop **stops accepting new connections**. The 17th HTTP request hangs ~60 s, the test body never makes progress, popen kills the subprocess at the 120 s watchdog (`DAS_POPEN_TIMEOUT = 0x7FFFFF01 = 2147483393`).

**Confirm**: run with `DASLIVE_HV_LOG=stderr DASLIVE_HV_LOG_LEVEL=DEBUG` env vars. Count `[POST /command]=>[200 OK]` lines per subprocess pid in the captured stderr — if exactly 16, you've hit it. Healthy paths show many more.

**Verified-locally counterproof**: same dasImgui suite under `D:\Work\daScript\bin\Release\daslang.exe ... dastest.das -- --test ... --headless` on Win11 box runs 96/96 in ~6 min, including the tests that hang on CI. So it's CI-runner-specific (Windows Server, different scheduler, different IOCP / TCP loopback tuning, possibly Defender). NOT a libhv bug per se — the upstream libhv code on master is byte-identical to v1.3.4 and works fine on `libDaScriptDyn` linkage everywhere except GitHub `windows-latest`.

**Workarounds in dasImgui PR #38**:

1. **Halve HTTP traffic in polling helpers**. Old idiom `wait_until(d, 240) $(var snap) { let s = post_command(d, "imgui_key_status", null); return !(s?["playing"] ?? true) }` does **2 HTTP requests per iteration** (snapshot inside `wait_until` + the inner `post_command`). New helpers `wait_for_key_idle(d, 4.0f)` and `wait_for_mouse_idle(d, 4.0f)` in `widgets/imgui_playwright.das` do **1 POST per iteration** (status only). Use them whenever you only need "playing == false", not a full snapshot.

2. **Exclude high-POST tests on Windows-only** in `.github/workflows/tests.yml`. Conservative cutoff: any test estimated at ≥12 POSTs over its lifetime, leaves a 4-call safety margin under the 16-connection limit. Pattern:
   ```yaml
   EXTRA_EXCLUDES=""
   if [ "${{ matrix.os }}" = "windows-latest" ]; then
     EXTRA_EXCLUDES="--exclude inputs_drag --exclude inputs_numeric --exclude inputs_slider \
                     --exclude inputs_color --exclude inputs_choice --exclude inputs_text \
                     --exclude indexed_dynamic"
   fi
   ```

**Heuristic for "POST count"**: each `set_value(...)` is 1 POST. Each `wait_for_payload_value(...)` / `wait_for_int_value(...)` / `wait_until { post_command }` is 1-3 POSTs depending on how fast the answer converges. Tests with ≥10 `set_value + wait` pairs typically exceed 16 POSTs.

**Pre-existing "finally skipped on panic" cascade**: a panicking test in the body block was already known to skip `with_imgui_app`'s `/shutdown` cleanup → zombie subprocess on port 9090 → next test cascades. PR #38 also added `--headless-timeout-sec=N` self-exit to `imgui_harness` (see related card `imgui-harness-headless-timeout-sec-cascade-guard`) so a panicked subprocess can't outlive the popen watchdog.

**Proper fix is upstream** in daslang's libhv build for Windows IOCP. Track + re-include all 7 excluded tests when it lands.

## Questions
- Why do imgui playwright tests hang at 120 seconds on Windows CI when they pass locally and on POSIX?
