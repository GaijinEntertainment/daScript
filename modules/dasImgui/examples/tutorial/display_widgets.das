options gen2
options _comment_hygiene = true

require math
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
// TUTORIAL: display_widgets — boost v2 visual-output widgets.
//
// progress_bar wraps ImGui::ProgressBar with a ProgressBarState payload
// carrying {fraction, size, overlay}. image wraps ImGui::Image with full
// uv0/uv1/tint_col/border_col + a uint64 reinterpret of the texture handle
// for snapshot readability.
//
// The image() example uses the ImGui font atlas texture (always available),
// so this tutorial runs without any external asset.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/display_widgets.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/display_widgets.das
//
// DRIVE (when running live):
//   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command \
//        | jq '.globals."DISPLAY_WIN".payload'
// =============================================================================

var private g_phase : float = 0.0f

[export]
def init() {
    live_create_window("dasImgui display widgets", 940, 680)
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

    g_phase += 0.01f
    let driven_frac = 0.5f + 0.5f * sin(g_phase)

    SetNextWindowPos(ImVec2(60.0, 60.0), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(680.0, 420.0), ImGuiCond.Always)
    window(DISPLAY_WIN, (text = "Display widgets", closable = false,
                         flags = ImGuiWindowFlags.None)) {
        separator_text("progress_bar")

        // Static bar - 33% with auto-formatted overlay.
        progress_bar(PB_STATIC, (fraction = 0.33f,
                                 size = float2(-1.0f, 0.0f),
                                 overlay = "33%"))

        // Driven bar - sine-wave fraction, default overlay.
        progress_bar(PB_DRIVEN, (fraction = driven_frac,
                                 size = float2(-1.0f, 0.0f),
                                 overlay = ""))

        // Fixed-width bar with custom overlay text.
        progress_bar(PB_FIXED, (fraction = 0.75f,
                                size = float2(240.0f, 0.0f),
                                overlay = "loading..."))

        separator_text("image")

        // Use the ImGui font atlas as our texture — always built by frame 1.
        let io & = unsafe(GetIO())
        if (io.Fonts.TexData == null) {
            text("Font atlas not ready — wait for first frame.")
        } else {
            text("image() against the font atlas - varying uv + tint per call.")
            image(IMG_PLAIN, (user_texture_id = io.Fonts.TexRef,
                              size = float2(96.0f, 96.0f),
                              uv0 = float2(0.0f, 0.0f),
                              uv1 = float2(1.0f, 1.0f),
                              tint_col = float4(1.0f, 1.0f, 1.0f, 1.0f),
                              border_col = float4(0.0f, 0.0f, 0.0f, 0.0f)))
            same_line(SL_IMG)
            image(IMG_TINT, (user_texture_id = io.Fonts.TexRef,
                             size = float2(96.0f, 96.0f),
                             uv0 = float2(0.0f, 0.0f),
                             uv1 = float2(1.0f, 1.0f),
                             tint_col = float4(0.3f, 0.9f, 0.6f, 1.0f),
                             border_col = float4(1.0f, 1.0f, 1.0f, 0.5f)))
        }
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
