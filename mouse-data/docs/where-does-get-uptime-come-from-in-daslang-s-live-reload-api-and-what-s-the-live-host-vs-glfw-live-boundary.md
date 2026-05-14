---
slug: where-does-get-uptime-come-from-in-daslang-s-live-reload-api-and-what-s-the-live-host-vs-glfw-live-boundary
title: Where does `get_uptime()` come from in daslang's live-reload API, and what's the live_host vs glfw_live boundary?
created: 2026-05-13
last_verified: 2026-05-13
links: []
---

`get_uptime()` lives in **`live_host`** (the backend-agnostic core of the live-reload API), not in `glfw_boost` or `live/glfw_live`. It's exposed by `modules/dasLiveHost/src/dasLiveHost.cpp:137` (C++ `live_get_uptime()`) and registered on the `live_host` daslang module:

```cpp
// dasLiveHost.cpp
float live_get_uptime() { ... }
// in initDependencies:
addExtern<DAS_BIND_FUN(live_get_uptime)>(*this, lib, "get_uptime",
    SideEffects::accessGlobal, "das::live_get_uptime");
```

To use it: `require live_host` and call `get_uptime()`.

**Why this matters when carving backend-agnostic modules:** A common mistake is reaching for `require glfw/glfw_boost` to get `get_uptime` because that's where GLFW-coupled live code (like `modules/dasGlfw/dasglfw/glfw_live.das`) is *used*. But `glfw_live.das` doesn't *provide* `get_uptime` — it consumes it from `live_host` like everyone else. Adding `require glfw/glfw_boost` to a backend-agnostic file just to reach `get_uptime` re-couples the file to GLFW for nothing.

**The boundary:**
- `live_host` (C++ shared_module via `modules/dasLiveHost/`): backend-agnostic live API — `get_uptime`, `is_reload`, `live_store_bytes`, `live_load_bytes`, `[before_reload]`/`[after_reload]` annotation plumbing.
- `live/live_commands` (daslang): the `[live_command]` annotation and JSON-RPC dispatch — also backend-agnostic.
- `live/glfw_live` (daslang, requires GLFW): GLFW-specific synth-IO driver, window lifecycle integration.
- `glfw/glfw_boost` (daslang, requires GLFW): higher-level GLFW wrappers (window helpers etc.).

**Symptom of getting this wrong:** compile error[30341] `no matching functions or generics: get_uptime()` with the candidate-comment-listing `live_host::get_uptime is not visible directly from <your_module>`. Fix: replace any `require glfw/glfw_boost` (added "for `get_uptime`") with `require live_host`.

**Surfaced 2026-05-14** while splitting `widgets/imgui_live.das` synth driver into `widgets/imgui_live_core.das`. The original file had `require glfw/glfw_boost` and used `get_uptime()`; I assumed the symbol came from glfw_boost and copied the require into the new file. The compile-check error pointed straight at the right module.

## Questions
- Where does `get_uptime()` come from in daslang's live-reload API, and what's the live_host vs glfw_live boundary?
