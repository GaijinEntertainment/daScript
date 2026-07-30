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
require imgui/imgui_visual_aids

// =============================================================================
// TUTORIAL: layout_primitives — boost v2 cursor-positioning widgets.
//
// Layout markers move the imgui cursor without rendering anything. They share
// EmptyMarkerState — payload is `{}`, but the snapshot still records the
// widget existed, so playwright can assert "spacing#3 was rendered".
//
// Covers: same_line / new_line / spacing / dummy
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/layout_primitives.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/layout_primitives.das
// =============================================================================

[export]
def init() {
    live_create_window("dasImgui layout primitives", 1000, 640)
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

    SetNextWindowPos(ImVec2(60.0, 60.0), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(600.0, 360.0), ImGuiCond.Always)
    window(LAYOUT_WIN, (text = "Layout primitives", closable = false,
                        flags = ImGuiWindowFlags.None)) {
        text("Three buttons on one row - same_line() keeps them inline.")
        if (button(BTN_A, (text = "A"))) {
            print("A\n")
        }
        same_line(SL_AB)
        if (button(BTN_B, (text = "B"))) {
            print("B\n")
        }
        same_line(SL_BC)
        if (button(BTN_C, (text = "C"))) {
            print("C\n")
        }

        spacing(SP_TOP)
        text("spacing() inserts a small vertical gap (1 line).")

        spacing(SP_MID)
        spacing(SP_MID2)
        spacing(SP_MID3)
        text("Three spacing() calls stack; cheap if you don't want a Dummy.")

        new_line(NL_INSERT)
        text("new_line() == one full text-line of vertical space.")

        dummy((size = float2(0.0f, 40.0f)))
        text("dummy(size) reserves arbitrary cursor space.")
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
