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
require daslib/clargs
require strings

require imgui_demo

// =============================================================================
// EXAMPLE: imgui_demo — daslang port of Dear ImGui's imgui_demo.cpp.
// SHOWS:   complete tour of the dasImgui boost surface as a single host file
//          dispatching into per-section sub-modules under imgui_demo/.
// STANDALONE: daslang.exe modules/dasImgui/examples/imgui_demo/main.das
// SMOKE:      daslang.exe modules/dasImgui/examples/imgui_demo/main.das -- --headless-frames=30
//             (auto-exit after N frames; useful for CI smoke runs / repro)
// LIVE:       daslang-live modules/dasImgui/examples/imgui_demo/main.das
// =============================================================================

// `--headless-frames=N` — exit after rendering N frames. Matches the same flag
// name used by `imgui_harness`'s headless rail so test runners stay uniform
// across the harness/live-host split. 0 = run until window-close.
var private g_max_frames = 0
var private g_frame_count = 0
var private g_args_parsed = false

def private ensure_args_parsed() {
    return if (g_args_parsed)
    g_args_parsed = true
    let args <- get_user_args()
    let raw = find_flag_raw_value(args, "--headless-frames")
    g_max_frames = to_int(raw |> unwrap_or("0"))
}

[export]
def init() {
    ensure_args_parsed()
    live_create_window("Dear ImGui Demo (daslang port)", 1280, 720)
    live_imgui_init(live_window)
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplGlfw_NewFrame()
    NewFrame()

    RunImGuiDemo()

    end_of_frame()
    Render()
    var w, h : int
    live_get_framebuffer_size(w, h)
    glViewport(0, 0, w, h)
    glClearColor(0.10f, 0.10f, 0.12f, 1.0f)
    glClear(GL_COLOR_BUFFER_BIT)
    live_imgui_render()

    live_end_frame()

    // Frame-cap exit (smoke-test path). Bump AFTER the frame so the very
    // first iteration always renders at least one full frame.
    g_frame_count++
    if (g_max_frames > 0 && g_frame_count >= g_max_frames) {
        request_exit()
    }
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
