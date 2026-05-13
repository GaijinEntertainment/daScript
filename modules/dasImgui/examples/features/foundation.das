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
// FEATURE: foundation — Phase 0b.0 [widget] foundation contract.
// SHOWS:   auto-emit (no pre-declared SAVE_BTN/SPEED/VOLUME globals) +
//          dotted-suffix flags (.PUBLIC, .NOTLIVE) + lazy serializer (per-kind
//          payload via g_serializers, materialized on imgui_snapshot only) +
//          unified L2/L3 dispatch (one g_dispatchers table for every kind).
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command
//          curl -X POST -d '{"name":"imgui_click","args":{"target":"SAVE_BTN"}}' \
//               localhost:9090/command
//          curl -X POST -d '{"name":"imgui_set","args":{"target":"SPEED","value":7}}' \
//               localhost:9090/command
//          curl -X POST -d '{"name":"imgui_set","args":{"target":"VOLUME","value":0.5}}' \
//               localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/foundation.das
// LIVE:       daslang-live modules/dasImgui/examples/features/foundation.das
// =============================================================================

[export]
def init() {
    //! Reload-aware. live_imgui_init is idempotent: cold start runs
    //! CreateContext + ImGui_Impl* setup; reload reuses the ctx pointer
    //! restored by imgui_live's [after_reload].
    live_create_window("Phase 0b.0 — foundation", 800, 600)
    live_imgui_init(live_window)
    var io & = unsafe(GetIO())
    io.FontGlobalScale = 1.5
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplOpenGL3_NewFrame()
    ImGui_ImplGlfw_NewFrame()
    NewFrame()

    SetNextWindowSize(ImVec2(720.0, 320.0), ImGuiCond.Always)
    if (Begin("Foundation — phase 0b.0", null, ImGuiWindowFlags.None)) {
        Text("Three [widget] decls — every global below is auto-emitted.")

        // Button — return form. Identifier `SAVE_BTN` becomes the
        // module-scope `var SAVE_BTN : ClickState` global on first compile.
        // Note `(text = "Save")` (parens, not square brackets): named-tuple
        // form which the [widget] call_macro destructures into positional
        // args.
        if (button(SAVE_BTN, (text = "Save"))) {
            print("save clicked, count={SAVE_BTN.click_count}\n")
        }
        Text("save clicks: {SAVE_BTN.click_count}")

        // Slider int — emits SliderStateInt SPEED, marked PUBLIC so a sibling
        // test module can read `SPEED.value` across the module boundary.
        slider_int(SPEED.PUBLIC, (text = "Speed (int)"))
        Text("speed = {SPEED.value}")

        // Slider float — emits SliderStateFloat VOLUME without @live; the
        // source-side initial value wins on reload (no preservation).
        slider_float(VOLUME.NOTLIVE, (text = "Volume (float)"))
        Text("volume = {VOLUME.value}")
    }
    End()

    end_of_frame()
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
    //! Reload-aware. live_imgui_shutdown skips during reload; tears down
    //! only on process exit.
    live_imgui_shutdown()
    live_destroy_window()
}

[export]
def main() {
    //! Standalone entrypoint. daslang-live drives init/update/shutdown
    //! directly and ignores main(); plain `daslang.exe <file>` runs this loop.
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
