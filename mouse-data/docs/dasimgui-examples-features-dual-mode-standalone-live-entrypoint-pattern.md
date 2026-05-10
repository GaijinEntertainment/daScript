---
slug: dasimgui-examples-features-dual-mode-standalone-live-entrypoint-pattern
title: dasImgui examples/features/*.das — what's the dual-mode standalone+live entrypoint pattern every file must follow?
created: 2026-05-10
last_verified: 2026-05-10
links: []
---

Every file under `modules/dasImgui/examples/features/` MUST run both ways with no edits:

1. **Standalone:** `daslang.exe modules/dasImgui/examples/features/<name>.das` — runs the GLFW window + render loop via `[export] def main()`. No live_api server, no `/reload`, but the GUI is fully functional. Gives the file its "executable doc" value.
2. **Live:** `daslang-live modules/dasImgui/examples/features/<name>.das` — daslang-live ignores `main()`, drives init/update/shutdown directly, spins up live_api on port 9090, enables `/reload` and `/command` for `imgui_snapshot` / `imgui_click` / `imgui_set` etc.

**The contract** (boilerplate, copy-pasted across all files):
```das
[export] def init() {
    live_create_window("...", 800, 600)
    live_imgui_init(live_window)
}
[export] def update() {
    if (!live_begin_frame()) { return }
    begin_frame()  // imgui_boost_runtime — clears registry
    ImGui_ImplOpenGL3_NewFrame(); ImGui_ImplGlfw_NewFrame(); NewFrame()
    if (Begin("...")) { /* widgets */ }
    End()
    Render()
    var w, h : int; live_get_framebuffer_size(w, h)
    glViewport(0, 0, w, h)
    glClearColor(0.10f, 0.10f, 0.12f, 1.0f); glClear(GL_COLOR_BUFFER_BIT)
    ImGui_ImplOpenGL3_RenderDrawData(GetDrawData())
    live_end_frame()
}
[export] def shutdown() { live_imgui_shutdown(); live_destroy_window() }
[export] def main() {
    //! Standalone entrypoint. daslang-live drives init/update/shutdown directly.
    init(); while (!exit_requested()) { update() }; shutdown()
}
```

**Required `require` list** (same for every features/ file):
```
options gen2
require imgui
require imgui_app
require glfw/glfw_boost
require opengl/opengl_boost
require live/glfw_live
require live/live_api
require live/live_commands
require live/live_vars
require live/opengl_live
require live_host
require imgui/imgui_live
require imgui/imgui_boost_runtime
require imgui/imgui_boost_v2
require imgui/imgui_widgets_builtin
```

**File-header comment block convention** (grep-keys for tutorial generation):
```
// FEATURE: <name>
// SHOWS:   <one-line>
// DRIVE:   <curl recipe — only meaningful under daslang-live>
// STANDALONE: daslang.exe modules/dasImgui/examples/features/<name>.das
// LIVE:       daslang-live modules/dasImgui/examples/features/<name>.das
```

**Reference impl:** `modules/dasImgui/examples/features/triggers.das` (Phase 0b.1) and `modules/dasImgui/example/phase0a_define_widget/main.das` (Phase 0a — same pattern, predates the examples/features/ dir convention).

NOTE the singular `example/` (phase-marker apps) vs plural `examples/features/` (the 0b runnable feature catalogue) — both coexist.

## Questions
- dasImgui examples/features/*.das — what's the dual-mode standalone+live entrypoint pattern every file must follow?
