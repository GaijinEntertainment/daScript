---
slug: how-do-i-preserve-a-c-owned-resource-across-a-daslang-live-reload
title: How do I preserve a C++-owned resource (GLFW window, ImGui ctx, audio handle) across a daslang-live reload so the daslang-side state survives?
created: 2026-05-09
last_verified: 2026-05-09
links: []
---

Four-piece pattern, lifted from `modules/dasGlfw/dasglfw/glfw_live.das` and `modules/dasAudio/audio/audio_live.das`. The C++ DLL holds the resource (it doesn't unload across reload), but the daslang module global that points at it gets zeroed when the OLD context is destroyed. The fix is to roundtrip the pointer as `uint64` through daslang-live's persistent byte store.

**The four pieces (all in one daslang module):**

```daslang
require daslib/archive
require live_host

let private STORE_KEY = "__my_resource"

var public live_my_resource : MyResource?    // module-scope; daslang globals reset on reload

// 1) Idempotent setup. Cold start: real init. Reload: state-check returns early.
def public live_my_init(...) : MyResource? {
    if (live_my_resource != null) {
        return live_my_resource    // restored by [after_reload] before init() ran
    }
    live_my_resource = real_create(...)
    return live_my_resource
}

// 2) Reload-aware teardown. Skip during reload — the [before_reload] handler
//    will hand the pointer off to the new context instead.
def public live_my_shutdown() {
    if (live_my_resource != null && !is_reload()) {
        real_destroy(live_my_resource)
        live_my_resource = null
    }
}

// 3) Snapshot the pointer to the persistent store.
[before_reload]
def private save_my_resource() {
    if (live_my_resource == null) { return }
    var ser = new MemSerializer()
    var arch = Archive(reading = false, stream = ser)
    var ptr_val = unsafe(reinterpret<uint64>(live_my_resource))
    arch |> serialize(ptr_val)
    let data <- ser->extractData()
    live_store_bytes(STORE_KEY, data)
    unsafe { delete ser; }
}

// 4) Restore the pointer in the NEW context, before init() runs.
[after_reload]
def private restore_my_resource() {
    var data : array<uint8>
    if (live_load_bytes(STORE_KEY, data) && length(data) > 0) {
        var ser = new MemSerializer(data)
        var arch = Archive(reading = true, stream = ser)
        var ptr_val : uint64
        arch |> serialize(ptr_val)
        unsafe {
            live_my_resource = reinterpret<MyResource?>(ptr_val)
            delete ser
        }
        // Re-make-current / re-bind handles here if the resource has thread-affinity.
    }
}
```

**Why each piece:**

- **State-check guard on init** (not `if (!is_reload())`): `is_reload()` is true throughout reload (in init *and* shutdown — see the firing-order doc), but state-check is more honest because it ALSO handles the recompile-fail-back-to-old-ctx replay, where `[after_reload]` re-runs and init() re-runs against an already-populated global.
- **`is_reload()` guard on shutdown**: shutdown runs in the OLD context. Without the guard, you'd `real_destroy` the live resource just before the new context tries to reuse the pointer — instant use-after-free.
- **`[before_reload]` saves**: runs in OLD context, last chance to capture state before destruction.
- **`[after_reload]` restores**: runs in NEW context, BEFORE `init()` (line 511 vs 522 in `utils/daslang-live/main.cpp`). This is why the state-check in init can already see the restored value.

**Three in-tree examples, all live by this exact shape:**

- `modules/dasGlfw/dasglfw/glfw_live.das:54,94-123` — preserves GLFW window pointer; `restore_glfw_window` calls `glfwMakeContextCurrent` after deserializing.
- `modules/dasAudio/audio/audio_live.das:23-55` — preserves command-stream + sound-SID pointers via setter callbacks.
- `D:\DASPKG\dasImgui\widgets\imgui_live.das` — preserves `ImGuiContext?`; `restore_imgui_ctx` calls `SetCurrentContext` after deserializing.

**Full-reload caveat:** `daslang-live --full` (or `live_reload(full=true)`) clears `@live` variables (`dll_clear_live_vars` at main.cpp:500-502) but does **not** clear the persistent byte store. So preserved C++ resources survive even a "full" reload — same as the GLFW window. If you want a truly cold restart, kill the process.

**Surfaced 2026-05-09** during dasImgui Phase 0a, when I needed to make `init_imgui()` not leak the prior ImGui ctx on reload. The pattern transfers verbatim from glfw_live.

## Questions
- How do I preserve a C++-owned resource (GLFW window, ImGui ctx, audio handle) across a daslang-live reload so the daslang-side state survives?
- How do I keep my daslang module global pointing at a live C++ resource across hot-reload?
- What's the canonical [before_reload]/[after_reload] save/restore pattern with MemSerializer + live_store_bytes?
- Why use a state-check (`if (live_x != null) return`) on init instead of `if (!is_reload())`?
