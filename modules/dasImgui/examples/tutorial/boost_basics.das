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
// TUTORIAL: boost_basics — the smallest dasImgui v2 program.
//
// One window. One slider drives a float. One button increments a counter.
// State lives in daslang globals (@live by default, so values survive reload).
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/boost_basics.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/boost_basics.das
//
// DRIVE (when running live):
//   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command
//   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"BASICS_WIN/VOLUME","value":0.75}}' \
//        localhost:9090/command
//   curl -X POST -d '{"name":"imgui_click","args":{"target":"BASICS_WIN/BUMP_BTN"}}' \
//        localhost:9090/command
// =============================================================================

[export]
def init() {
    live_create_window("dasImgui boost basics", 960, 560)
    live_imgui_init(live_window)
    let io & = unsafe(GetIO())
    GetStyle().FontScaleMain = 1.5
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplGlfw_NewFrame()
    apply_synth_io_override()
    NewFrame()

    SetNextWindowPos(ImVec2(60.0, 60.0), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(560.0, 240.0), ImGuiCond.Always)
    window(BASICS_WIN, (text = "Boost basics", closable = false,
                        flags = ImGuiWindowFlags.None)) {
        // Slider — VOLUME state lives in a generated daslang global.
        VOLUME.bounds = (0.0f, 1.0f)
        slider_float(VOLUME, (text = "Volume"))
        text("volume = {VOLUME.value}")

        spacing(BB_SP_1)

        // Button — BUMP_BTN.click_count is updated on every click.
        if (button(BUMP_BTN, (text = "Bump"))) {
            print("bump clicked\n")
        }
        text("bumps = {BUMP_BTN.click_count}")
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
