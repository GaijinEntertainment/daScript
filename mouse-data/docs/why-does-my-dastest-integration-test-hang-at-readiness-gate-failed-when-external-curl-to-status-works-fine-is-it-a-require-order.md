---
slug: why-does-my-dastest-integration-test-hang-at-readiness-gate-failed-when-external-curl-to-status-works-fine-is-it-a-require-order
title: why does my dastest integration test hang at "readiness gate FAILED" when external curl to /status works fine — is it a require-order issue in daslang-live?
created: 2026-05-16
last_verified: 2026-05-16
links: []
---

# Symptom

`dastest` integration tests hang at the `imgui_playwright` readiness gate:

```
[imgui_playwright] subprocess up, polling /status...
[imgui_playwright] readiness gate FAILED
```

External `curl http://localhost:9090/status` from a sibling shell returns 200 with proper status JSON throughout — only the popen parent's poll loop "can't see it". Reproduces on macOS and Linux; appears to NOT reproduce on Windows (which is the trap — see below).

# Root cause

**`ref_time_ticks()` returns nanoseconds on POSIX, but the wait-loop math assumes microseconds.**

`src/hal/performance_time.cpp` defines `ref_time_ticks()` per platform:

| Platform | Returns |
|---|---|
| Linux  | `tv_sec * 1e9 + tv_nsec` — **nanoseconds** |
| macOS  | `clock_gettime_nsec_np(CLOCK_MONOTONIC_RAW)` — **nanoseconds** |
| Windows | `QueryPerformanceCounter().QuadPart` — counter ticks, freq depends on hardware (often ~10 MHz, accidentally close to 1 MHz / microsecond scaling) |

`imgui_playwright`'s `wait_until_ready` (and other deadline loops) used:

```das
let deadline = ref_time_ticks() + int64(timeout_sec * 1000000.0f)
while (ref_time_ticks() < deadline) {
    GET("{base_url}/status") $(resp) { ... }
    sleep(READY_POLL_INTERVAL_MS)
}
```

That `* 1000000.0f` assumes ref-time is in microseconds. So:
- **Linux/macOS**: a "30s" deadline is `30 * 1e6 = 30 million nanoseconds = 30 milliseconds`. Loop fires 0-1 polls and exits. The `connect 127.0.0.1:9090 failed!` line is the one in-flight libhv connect attempt timing out — server health is fine; the loop just budgeted itself out of existence.
- **Windows**: QPC freq is hardware-dependent but on common runners works out near enough to 1 MHz that `* 1e6` lands in the "seconds" ballpark by accident, masking the bug.

# The Windows-only "require order" workaround is misleading

[#2677](https://github.com/GaijinEntertainment/daScript/issues/2677) and a prior version of this card blamed require-order — windowed-backend stack vs. live-host stack — claiming `[_macro] installing` in `live_api.das` calling `fork_debug_agent_context(@@debug_agent)` before GLFW init was the issue. That diagnosis was wrong. The reorder happened to nudge timings just enough on Windows for the (already-too-short) loop to occasionally win the race, which read as "fix". On POSIX, the same reorder changes nothing — the loop still exits in 30 ms regardless of require order.

If you see code in `imgui_harness.das` carrying a `// NOTE on require ordering` comment about live_api needing to come after the windowed stack: that comment is load-bearing only by accident on Windows. The real fix is in the timing math.

# Fix

Replace any `ref_time_ticks() + int64(seconds * 1000000.0f)` deadline pattern with platform-correct math. Two options:

```das
// Option A — use the elapsed-microsec helper (always microseconds, all platforms)
let t_start = ref_time_ticks()
let timeout_us = int(timeout_sec * 1000000.0f)
while (get_time_usec(t_start) < timeout_us) {
    ...
}

// Option B — compute deadline in nanoseconds, on POSIX
let deadline = ref_time_ticks() + int64(timeout_sec * 1000000000.0f)
// (DON'T do this without a per-platform branch — breaks Windows)
```

**Option A is the right one.** `get_time_usec(reft)` is defined per-platform in `performance_time.cpp` and always returns microseconds. Audit any other `ref_time_ticks() + ... * 1000000.0f` patterns in your codebase the same way.

# How to recognize this gotcha

- Test hangs at `readiness gate FAILED` (not at `body did not converge` or similar).
- External `curl` to `localhost:9090/status` works while the test hangs.
- Reproduces on macOS / Linux; "works" on Windows (deceptive — see above).
- Suspect any deadline loop using `ref_time_ticks() + ... * 1e6` — that's the smoking gun.

# Why this took a while to spot

The symptom looked exactly like a network or popen-inheritance bug:
- libhv server is up (curl proves it)
- The dastest process's libhv client errors with `connect 127.0.0.1:9090 failed!`
- It's specific to the popen parent

The actual chain was: the poll loop's deadline elapsed before the first GET even had a chance to retry. The single connect error in the log was libhv's first attempt timing out as the loop quit. Everything past that ("requires order", "fork_debug_agent_context", "libhv client init quirk on macOS") was downstream of misreading the symptom.

# Verification

After the fix, a clean local run of the dasImgui integration suite passes on macOS in seconds-per-test rather than 120s-test-timeout-per-test.

## Questions
- why does my dastest integration test hang at "readiness gate FAILED" when external curl to /status works fine — is it a require-order issue in daslang-live?
- on macOS / Linux, ref_time_ticks() returns nanoseconds — does any of my code assume microseconds?
- what units does ref_time_ticks() return per platform?
