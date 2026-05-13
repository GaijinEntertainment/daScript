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
require imgui/imgui_containers_builtin

// =============================================================================
// FEATURE: output_text — Phase 1.4 text_show display widget (master plan §4.3a).
// SHOWS:   text_show(IDENT, value="...") and text_show(IDENT) forms. State
//          is a single string accessible via STATUS.value, surfaced through
//          the standard serializer so `expect_value` assertions work
//          end-to-end. The dispatcher accepts imgui_set so tests can drive
//          the value remotely.
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command
//          → globals."MAIN_WIN/STATUS_LINE".payload.value = "..." (echoes
//             whatever value is set, by source-init or remote imgui_set)
//          curl -X POST -d '{"name":"imgui_set","args":{"target":"MAIN_WIN/STATUS_LINE","value":"hello"}}' \
//               localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/output_text.das
// LIVE:       daslang-live modules/dasImgui/examples/features/output_text.das
// =============================================================================

[export]
def init() {
    live_create_window("Phase 1.4 — text_show", 800, 600)
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

    SetNextWindowSize(ImVec2(560.0, 220.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "text_show demo", closable = false,
                     flags = ImGuiWindowFlags.None)) {
        Text("Two forms of text_show:")

        // (1) source-init form — every frame state.value resets to the
        // literal "ready"; imgui_set still works between frames but the
        // next frame overwrites.
        text_show(BANNER, "ready")

        // (2) display-only form — initial state.value is "" until something
        // (the bump button below or imgui_set) writes it.
        text_show(STATUS_LINE)

        // Lets the demo loop touch STATUS_LINE programmatically.
        if (button(BUMP_BTN, (text = "bump status"))) {
            STATUS_LINE.value := "bumped at frame {g_frame}"
        }
    }

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
