options gen2
options _comment_hygiene = true

require imgui
require imgui_app
require opengl/opengl_boost
require live/glfw_live
require live/live_api
require live/live_commands
require live/live_vars
require live_host
require imgui/imgui_live
require imgui/imgui_boost_runtime
require imgui/imgui_boost_v2
require imgui/imgui_widgets_builtin
require imgui/imgui_containers_builtin
require imgui/imgui_scope_builtin
require imgui/imgui_visual_aids

// =============================================================================
// TUTORIAL: with_tab_stop — stateless scope wrapper around PushTabStop /
//           PopTabStop. While the wrapped block is active, every widget
//           inside is skipped by TAB / Shift+Tab focus cycling. Useful for
//           help markers or decorative inputs that shouldn't steal keyboard
//           focus when navigating an editable form.
//
//   with_tab_stop(false) { widgets_to_skip... }
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/with_tab_stop.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/with_tab_stop.das
// =============================================================================

[export]
def init() {
    live_create_window("dasImgui with_tab_stop tutorial", 760, 460)
    live_imgui_init(live_window)
    let io & = unsafe(GetIO())
    GetStyle().FontScaleMain = 1.4
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplGlfw_NewFrame()
    apply_synth_io_override()
    NewFrame()

    SetNextWindowPos(ImVec2(40.0f, 40.0f), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(720.0f, 400.0f), ImGuiCond.Always)
    window(TS_WIN, (text = "with_tab_stop", closable = false,
                    flags = ImGuiWindowFlags.None)) {
        text("Press TAB / Shift+TAB to cycle focus.")
        text("The middle field is wrapped in with_tab_stop(false) - TAB skips it.")
        separator()

        input_text(TS_FIELD_A, (text = "Field A"))
        input_text(TS_FIELD_B, (text = "Field B"))

        with_tab_stop(false) {
            input_text(TS_FIELD_SKIP,
                (text = "Skipped by TAB", init = "(unreachable via keyboard)"))
        }

        input_text(TS_FIELD_C, (text = "Field C"))
        input_text(TS_FIELD_D, (text = "Field D"))
    }

    end_of_frame()
    Render()
    var w, h : int
    live_get_framebuffer_size(w, h)
    glViewport(0, 0, w, h)
    glClearColor(0.10f, 0.10f, 0.12f, 1.0f)
    glClear(GL_COLOR_BUFFER_BIT)
    live_imgui_render()

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
