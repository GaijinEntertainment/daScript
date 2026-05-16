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

(30s `wait_until_ready` timeout, then 120s popen drain timeout. External `curl http://localhost:9090/status` from a sibling shell returns 200 with proper status JSON throughout — only the popen parent's request loop can't see it.)

# Root cause

`live/live_api` was required BEFORE `imgui_app + glfw/glfw_boost + opengl/* + glfw_live + opengl_live` somewhere in the requirer chain (usually a wrapper module like `imgui/imgui_harness`). The `[_macro] installing` in `live_api.das` calls `fork_debug_agent_context(@@debug_agent)` at compile time. If that fork happens before GLFW is initialized in the live runtime, the resulting LiveApiServer becomes unreachable from a popen parent on Windows.

Filed: [#2677](https://github.com/GaijinEntertainment/daScript/issues/2677). Distinct from #2675 (`ANY("*")` route shadowing).

# Fix (mechanical)

In the requirer module (yours or a wrapper you control), reorder requires so the **windowed backend stack comes first**:

```das
// Windowed backend FIRST (correctness, not aesthetics).
require imgui
require imgui_app
require glfw/glfw_boost
require opengl/opengl_boost
require live/glfw_live
require live/opengl_live

// Live-host + boost-runtime stack AFTER.
require live/live_api
require live/live_commands
require live/live_vars
require live_host
require imgui/imgui_live
require imgui/imgui_boost_runtime
require imgui/imgui_boost_v2
require imgui/imgui_widgets_builtin
require imgui/imgui_containers_builtin
require imgui/imgui_visual_aids
```

This mirrors the canonical order every pre-`imgui_harness` example/test used verbatim. Reordering is a no-op for visibility / re-export semantics — purely a workaround for the install-time ordering bug.

# How to recognize this gotcha

- Test hangs at `readiness gate FAILED` (not at `body did not converge` or similar).
- External `curl` to `localhost:9090/status` works while the test hangs (proves the server is up — the popen parent specifically can't reach it).
- Always reproduces — not a flaky timing issue.
- ONLY triggers when run via `popen` (via `with_imgui_app` in `imgui_playwright`, or any `dastest` integration test). Direct `bin/Release/daslang-live.exe <script>.das` runs fine because there's no popen parent.

# Verification commands

After reordering, the full dasImgui suite passes:

```bash
bin/Release/daslang.exe dastest/dastest.das -- --test modules/dasImgui/tests/integration
# 110 tests, 110 passed, 0 failed in ~500s
```

# Why not "just fix it in daslang"

The bug is in dasLiveHost or libhv's interaction with [_macro]-driven debug-agent install ordering. Filed as #2677 for triage, but the fix isn't trivial — for now reorder requires in the consumer/wrapper, document the ordering as load-bearing, and move on.

## Questions
- why does my dastest integration test hang at "readiness gate FAILED" when external curl to /status works fine — is it a require-order issue in daslang-live?
