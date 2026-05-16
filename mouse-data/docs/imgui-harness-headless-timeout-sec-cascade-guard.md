---
slug: imgui-harness-headless-timeout-sec-cascade-guard
title: How do I add a wall-clock self-exit timer to a daslang-live harness so a panicked test doesn't leave a zombie subprocess on the live-API port?
created: 2026-05-16
last_verified: 2026-05-16
links: []
---

**Problem**: daslang's `finally` block is skipped on panic. A test that does `with_imgui_app(F) $(d) { ... expect_value(...) ... }` and panics inside `expect_value`'s timeout never reaches the `/shutdown` POST that `with_imgui_app` would have sent in cleanup. The spawned `daslang-live` subprocess keeps running, holding port 9090. The next test that spawns errors out with "another instance of daslang-live is already running" (macOS) or hangs at drain for the full popen watchdog (Ubuntu) because its `/status` polls hit the zombie instead.

**Fix shape**: give the subprocess a wall-clock self-exit timer. Even if the parent never sends `/shutdown`, the script self-exits before the popen parent gives up, the port releases, and the next test starts cleanly.

**Implementation in dasImgui PR #38** (`widgets/imgui_harness.das` + `widgets/imgui_playwright.das`):

1. New CLI flag `--headless-timeout-sec=N` parsed via clargs alongside `--headless-frames=N`:
   ```daslang
   let raw_timeout = find_flag_raw_value(args, "--headless-timeout-sec")
   g_headless_max_uptime_sec = to_float(raw_timeout |> unwrap_or("0"))
   ```
   Default 0 = disabled (preserves standalone `daslang.exe foo.das -- --headless` usage).

2. Wall-clock check inside `harness_begin_frame()`, right next to the existing `--headless-frames` cap:
   ```daslang
   let now = get_uptime()
   if (g_headless_first_uptime < 0.0f) {
       g_headless_first_uptime = now
   }
   if (g_headless_max_uptime_sec > 0.0f && (now - g_headless_first_uptime) >= g_headless_max_uptime_sec) {
       print("[harness] headless timeout {g_headless_max_uptime_sec}s reached at uptime {now - g_headless_first_uptime}s — request_exit()\n")
       request_exit()
       return false
   }
   ```
   The print is the **only** log line kept in the cleaned-up harness — it fires at most once per subprocess and only when the safety net actually trips. Healthy runs are silent.

3. Playwright's `with_imgui_app_opt` appends `--headless-timeout-sec=(test_timeout_sec - 5)` to the spawned argv whenever `--headless` is forwarded. The −5 s margin gives the script time to finish the current frame, run `shutdown()`, and close the live-API port before the parent's popen watchdog fires:
   ```daslang
   if (playwright_wants_headless()) {
       argv |> push("--")
       argv |> push("--headless")
       let harness_budget = test_timeout_sec - 5.0f
       if (harness_budget > 5.0f) {
           argv |> push("--headless-timeout-sec={harness_budget}")
       }
   }
   ```

**Why it works on the cascade**: even when `expect_value` panics inside the body block, the daslang-live subprocess continues running its update loop. The next `harness_begin_frame()` call (called every frame from `update()`) notices `uptime > budget`, calls `request_exit()`, and the main loop terminates cleanly via `while (!exit_requested())`. `shutdown()` runs, port 9090 releases, popen's parent reads EOF, exit code is 0 — no cascade for the next test.

**Sizing rule**: set timeout less than the popen watchdog by enough margin to cover one frame + shutdown. 5 seconds is comfortable. If the popen watchdog is 120 s, harness timeout = 115 s.

**Limitation**: only fires from `harness_begin_frame`. If the script's `update()` is stuck inside something that never returns to the main loop, harness timeout never gets a chance. This is the right tradeoff — a stuck `update()` is a different bug class (real deadlock), and popen still kills the process at its watchdog.

**Cascade-guard pattern generalizes**: any long-running subprocess that owns a port (HTTP server, RPC endpoint, anything) and is spawned for a bounded test/check should have a wall-clock self-exit set slightly below the parent's kill-timeout. Cleanup-via-script always beats cleanup-via-SIGKILL.

## Questions
- How do I add a wall-clock self-exit timer to a daslang-live harness so a panicked test doesn't leave a zombie subprocess on the live-API port?
