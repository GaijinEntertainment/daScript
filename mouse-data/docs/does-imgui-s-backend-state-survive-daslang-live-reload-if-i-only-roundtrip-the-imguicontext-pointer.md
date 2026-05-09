---
slug: does-imgui-s-backend-state-survive-daslang-live-reload-if-i-only-roundtrip-the-imguicontext-pointer
title: Does ImGui's backend state (ImGui_ImplGlfw, ImGui_ImplOpenGL3) survive a daslang-live reload if I only roundtrip the ImGuiContext pointer? Do I need to re-call ImGui_ImplGlfw_InitForOpenGL after reload?
created: 2026-05-09
last_verified: 2026-05-09
links: []
---

**Short answer: yes, the backend state survives, and no, you do NOT need to re-call `ImGui_ImplGlfw_InitForOpenGL` / `ImGui_ImplOpenGL3_Init` after reload.** Roundtripping just the `ImGuiContext?` pointer is enough.

**Why it works:** `ImGui_ImplGlfw_Init*` and `ImGui_ImplOpenGL3_Init` store their per-backend data inside the ImGuiContext itself (via `GetIO().BackendPlatformUserData` / `BackendRendererUserData` and similar IO fields). The C++ DLL holds the ctx; only the daslang-side pointer is lost on reload. When `[after_reload]` reinstates the pointer + calls `SetCurrentContext`, the entire backend wiring comes back with it.

**Empirically confirmed** during dasImgui Phase 0a smoke (2026-05-09):

- Cold start: 3 widgets (button, slider_int, slider_float) registered with hex_ids `82712633`, `4027432670`, `2735099280`.
- Two consecutive `live_reload`s.
- After each reload: identical hex_ids, identical bboxes, focus state preserved (Save button kept `focus: true` across reload), no font-atlas flicker, no `has_error`.
- The hex_ids matching is the strongest signal — they're hashes derived from the ImGui id stack at runtime, so identical hex_ids means the id-stack derivation took the same path, which means the same ctx with the same backend data.

**What you DO need (covered by the C++-resource preservation pattern):**

```daslang
[after_reload]
def private restore_imgui_ctx() {
    // ... deserialize the uint64 ptr ...
    if (live_imgui_ctx != null) {
        SetCurrentContext(live_imgui_ctx)    // ← this is sufficient
    }
}
```

`SetCurrentContext` is the only call needed in `[after_reload]`. Don't re-init the backends — that would either no-op (if the impls are smart) or double-init (if they're not), and either way you've burned a font-atlas rebuild for no reason.

**The Phase 0a plan flagged this as an open question** (the pattern was unknown ahead of the implementation; the smoke verified it). If you ever observe ImGui windows snapping to default positions or hover not firing after reload, the assumption broke — fall back to also calling `ImGui_ImplGlfw_InitForOpenGL` + `ImGui_ImplOpenGL3_Init` inside `restore_imgui_ctx` after `SetCurrentContext`. As of 2026-05-09 the fallback is unnecessary on Windows + OpenGL3 backend.

**Surfaced 2026-05-09** during dasImgui Phase 0a smoke; settles the "do we need MemSerializer for the full ctx state, or just the pointer?" question that the original API_REWORK §10 left open.

## Questions
- Does ImGui's backend state (ImGui_ImplGlfw, ImGui_ImplOpenGL3) survive a daslang-live reload if I only roundtrip the ImGuiContext pointer?
- Do I need to re-call ImGui_ImplGlfw_InitForOpenGL / ImGui_ImplOpenGL3_Init in [after_reload]?
- Does ImGui's font atlas / window positions / focus state survive daslang-live reload?
- Where does ImGui store its backend data (and why does ctx-pointer roundtrip preserve it)?
