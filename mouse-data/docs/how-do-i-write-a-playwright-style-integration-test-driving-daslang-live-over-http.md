---
slug: how-do-i-write-a-playwright-style-integration-test-driving-daslang-live-over-http
title: How do I write a playwright-style integration test that drives a daslang-live host over HTTP?
created: 2026-05-10
last_verified: 2026-05-10
links: []
---

**Pattern:** spawn `daslang-live <feature>.das` as a subprocess via `popen_argv`, talk to its `live_api` HTTP server (default port 9090) using dashv's fire-and-forget client, and `POST /shutdown` when done. Wrap the lifecycle in a `with_live_app(path) <| $(d) { ... }` helper so test bodies just see a `LiveDriver`.

Reference impl: `modules/dasImgui/tests/integration/live_driver.das`. Tests in same dir.

**Subprocess primitives** (`daslib/fio`):
- `popen_argv(args : array<string>; timeout_sec : float; scope : block<(f : FILE?) : void>) : int` — runs the body while subprocess is alive; on body return, drains pipe, waits for exit, returns exit code. `popen_timed_out` is the timeout sentinel.
- `fread_to_eof(f)` — drain pipe so the child can fully exit. Call AFTER `POST /shutdown`.

**HTTP client** (`dashv`): free functions `GET(url) $(resp) { ... }`, `POST(url, body, headers) $(resp) { ... }`. Block receives `var resp : HttpResponse?` with `.body` (das_string), `.status_code`. The block-arg form is preferred over `<|` (lint STYLE001).

**daslang-live binary location** — mirror `get_das_exe()` (`daslib/command_line`): take its dir via `dir_name()`, `path_join` the platform-appropriate `daslang-live[.exe]`. Don't hard-code `bin/Release/`.

**Wire protocol:**
- `POST /command` body: `{"name":"<cmd>","args":{...}}`. Inner `args` is what reaches the `[live_command] def cmd(input : JsonValue?) : JsonValue?` handler.
- `GET /status` is the readiness gate. Returns 200 once HV is up, but **a 200 doesn't mean a frame has rendered** — for that, poll `imgui_snapshot` until the expected widget ident appears.

**The playwright analogy is exact:**
| Playwright | dasImgui playwright |
|---|---|
| `await page.click("#btn")` | `imgui_click {"target":"SAVE_BTN"}` |
| `await page.fill("input", "v")` | `imgui_set {"target":"SPEED","value":7}` |
| `await page.waitForSelector(...)` | `wait_for_widget(d, ident, timeout)` |
| `await expect(span).toHaveText("1")` | `wait_for_int_value(d, target, field, expected, timeout)` |
| `await page.waitForFunction(...)` | `wait_until(d, timeout) $(snap) => predicate` |
| `await page.reload()` | `reload(d)` (POST /reload + poll /status until has_error=false) |

**Never write `sleep()` in tests.** The internal poll loop sleeps inside `wait_until` (50 ms) — that's invisible to test authors. Test bodies use `wait_for_int_value` etc. to converge on observable state, with a per-call timeout. Auto-retry beats fixed-sleep on every axis (correctness, speed, robustness).

**Module declaration gotcha:** `module live_driver shared public` REQUIRES all transitive `require`s be `shared`. `daslib/command_line` isn't shared — drop `shared` and use plain `module live_driver public` for test harnesses.

**JSON traversal gotcha:** `snap?["globals"]?["MISSING_KEY"]` returns `JV(null)`, not literal-null pointer. So `result == null` is false even when the key is absent. Use a `widget_exists` helper that checks for a known field (e.g. `kind`) instead of a null-pointer test.

**Const-narrowing gotcha:** Helpers that take `JsonValue?` as input and chain `?[]` on it must declare the param as `var snap : JsonValue?`, not `snap : JsonValue?` — otherwise daslang infers the local as `auto const&` and `?[string]` resolves to the array-index overload, failing with "index type must be 'int'/'uint'".

**Found 2026-05-09**, dasImgui Phase 0b.1.5 (test harness landing). Caught a real bug along the way: `widget_finalize`'s "rescue serializer from prior frame's entry" optimization broke because `begin_frame()` clears `g_registry` every frame; fixed by moving serializers to a parallel `g_serializers` table.

## Questions
- How do I write a playwright-style integration test that drives a daslang-live host over HTTP?
