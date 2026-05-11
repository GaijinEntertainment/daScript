---
slug: how-do-i-write-a-playwright-style-integration-test-driving-daslang-live-over-http
title: How do I write a playwright-style integration test that drives a daslang-live host over HTTP?
created: 2026-05-10
last_verified: 2026-05-10
links: []
---

**Pattern:** spawn `daslang-live <feature>.das` as a subprocess via `popen_argv`, talk to its `live_api` HTTP server (default port 9090) using a `Transport` lambda (or dashv directly for lifecycle endpoints), and `POST /shutdown` when done. Wrap the lifecycle in a `with_imgui_app(path) <| $(app) { ... }` helper so test bodies just see an `ImguiApp`.

Reference impl: `modules/dasImgui/widgets/imgui_playwright.das` (Phase 3 — public sibling module). Tests in `modules/dasImgui/tests/integration/test_*.das`. Spec example: `modules/dasImgui/examples/save_demo/`.

Historical note: through Phase 2 this lived privately at `tests/integration/live_driver.das` with type `LiveDriver` and `with_live_app`. Phase 3 promoted to public `require imgui/imgui_playwright`, renamed to `ImguiApp` / `with_imgui_app`, and added the assertion family below.

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
| `await page.click("#btn")` | `click(app, "SAVE_BTN")` (→ `imgui_click`) |
| `await page.fill("input", "v")` | `set_value(app, "SPEED", JV(7))` (→ `imgui_set`) |
| `await page.type("input", "x")` | `type_text(app, "NAME", "hello")` — auto-focuses target first |
| `await page.waitForSelector(...)` | `wait_for_widget(app, ident, timeout)` |
| `await expect(span).toHaveText("1")` | `wait_for_int_value(app, target, field, expected, timeout)` — returns bool |
| `expect(...).toBe(...)` (hard-fail) | `expect_value(app, target, field, expected, timeout)` — panics on timeout |
| `expect(elem).toBeVisible()` | `expect_render(app, ident, timeout)` — panics with registered-widget list |
| `await page.waitForFunction(...)` | `wait_until(app, timeout) $(snap) => predicate` |
| `await page.waitForLoadState("networkidle")` | `await_quiescent(app, timeout)` — panics on timeout |
| `await page.reload()` | `reload(app)` (POST /reload + poll /status until has_error=false) |

**Never write `sleep()` in tests.** The internal poll loop sleeps inside `wait_until` (50 ms) — that's invisible to test authors. Test bodies use `wait_for_int_value` / `expect_value` to converge on observable state, with a per-call timeout. Auto-retry beats fixed-sleep on every axis.

**`expect_*` vs `wait_for_*`** — the `expect_*` family (Phase 3) panics on timeout with a focused failure message (want / got / widget kind / rendered). The `wait_for_*` family (Phase 2) returns bool. Pick `expect_*` when a failure means "the test is broken"; pick `wait_for_*` when you want soft-success boolean control flow (`if (wait_for_int_value(...)) { ... } else { ... }`).

**`type_text` auto-focus** — Phase 3's `type_text` posts `imgui_focus` first, polls `globals[target].focus == true`, then sends UTF-8 chars via `imgui_type_text`. **Feature apps using `type_text` (or `drag`) MUST call `advance_coroutines()` each frame** — the char-streaming is a Phase 2 coroutine. Without that, chars sit in queue forever and `expect_value` after will fail with `got: ""`. See `examples/features/io_synth_text.das` for the per-frame pattern.

**Module declaration gotcha:** `module imgui_playwright shared public` REQUIRES all transitive `require`s be `shared`. `daslib/command_line` isn't shared — drop `shared` and use plain `module imgui_playwright public` for test harnesses. The error surfaces at full-suite link time (`error[20115]: Shared module ... has incorrect dependency type. Can't require X because its not shared`), NOT at single-file `compile_check`. Re-bit me at Phase 3.6.

**JSON traversal gotcha:** `snap?["globals"]?["MISSING_KEY"]` returns `JV(null)`, not literal-null pointer. So `result == null` is false even when the key is absent. Use a `widget_exists` helper that checks for a known field (e.g. `kind`) instead of a null-pointer test.

**Const-narrowing gotcha:** Helpers that take `JsonValue?` as input and chain `?[]` on it must declare the param as `var snap : JsonValue?`, not `snap : JsonValue?` — otherwise daslang infers the local as `auto const&` and `?[string]` resolves to the array-index overload, failing with "index type must be 'int'/'uint'".

**Found 2026-05-09**, dasImgui Phase 0b.1.5 (test harness landing). Caught a real bug along the way: `widget_finalize`'s "rescue serializer from prior frame's entry" optimization broke because `begin_frame()` clears `g_registry` every frame; fixed by moving serializers to a parallel `g_serializers` table.

## Questions
- How do I write a playwright-style integration test that drives a daslang-live host over HTTP?
