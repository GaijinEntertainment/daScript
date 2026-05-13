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

// =============================================================================
// Phase 0b — define_widget with auto-emit + dotted-suffix flags.
// No pre-declared globals: the [widget] call_macro emits SAVE_BTN/SPEED/VOLUME
// as module-scope globals on first use (state type inferred from the function
// signature). Sliders default to bounds (0,0) post-emit — locked range is fine
// for the foundation gate; per-call init knobs land in a later phase.
//
// Dotted-suffix flags exercised here (API_REWORK §4.5):
//   - SPEED.PUBLIC   — cross-module-readable (drops the implicit private flag).
//   - VOLUME.NOTLIVE — skips the @live annotation; source-side default wins on reload.
//
// Drive it from a second terminal (only meaningful under daslang-live):
//   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command
//   curl -X POST -d '{"name":"imgui_click","args":{"target":"SAVE_BTN"}}' \
//        localhost:9090/command
//   curl -X POST -d '{"name":"imgui_set","args":{"target":"SPEED","value":7}}' \
//        localhost:9090/command
// =============================================================================

[export]
def init() {
    //! Reload-aware. `live_imgui_init` is idempotent: cold start runs
    //! CreateContext + ImGui_Impl* setup; reload reuses the preserved
    //! ctx pointer (restored by imgui_live's [after_reload]).
    live_create_window("Phase 0a — define_widget", 800, 600)
    live_imgui_init(live_window)
    var io & = unsafe(GetIO())
    io.FontGlobalScale = 2.0
}

[export]
def update() {
    if (!live_begin_frame()) {
        return
    }
    begin_frame()  // imgui_boost_runtime: clears registry, bumps frame counter

    ImGui_ImplOpenGL3_NewFrame()
    ImGui_ImplGlfw_NewFrame()
    NewFrame()

    SetNextWindowSize(ImVec2(720.0, 320.0), ImGuiCond.Always)
    if (Begin("Phase 0a", null, ImGuiWindowFlags.None)) {
        Text("define_widget demo — every widget below comes from a [widget] decl.")

        // Button — return form, identifier becomes the daslang global SAVE_BTN.
        // Note `(text = "Save")` (parens, not square brackets): named-tuple
        // form, which the [widget] call_macro destructures into positional
        // args. Square-bracket `[name=val]` builds an ExprNamedCall and
        // bypasses call_macro dispatch — see API_REWORK §4.5.
        if (button(SAVE_BTN, (text = "Save"))) {
            print("save clicked, count={SAVE_BTN.click_count}\n")
        }
        Text("save clicks: {SAVE_BTN.click_count}")

        // Slider int — emits SliderStateInt SPEED, marked PUBLIC so a
        // sibling test module can read SPEED.value across the module boundary.
        slider_int(SPEED.PUBLIC, (text = "Speed (int)"))
        Text("speed = {SPEED.value}")

        // Slider float — emits SliderStateFloat VOLUME without @live; the
        // source-side initial value wins on reload (no preservation).
        slider(VOLUME.NOTLIVE, (text = "Volume (float)"))
        Text("volume = {VOLUME.value}")
    }
    End()

    Render()
    var w, h : int
    live_get_framebuffer_size(w, h)
    glViewport(0, 0, w, h)
    glClearColor(0.10f, 0.10f, 0.12f, 1.0f)
    glClear(GL_COLOR_BUFFER_BIT)
    ImGui_ImplOpenGL3_RenderDrawData(GetDrawData())

    live_end_frame()
}

[export]
def shutdown() {
    //! Reload-aware. `live_imgui_shutdown` skips during reload; tears
    //! down only on process exit.
    live_imgui_shutdown()
    live_destroy_window()
}

[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
