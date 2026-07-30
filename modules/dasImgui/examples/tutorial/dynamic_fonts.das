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
require daslib/safe_addr

// =============================================================================
// TUTORIAL: dynamic_fonts — imgui 1.92 dynamic-font scaling, two levers.
//
// imgui 1.92 reworked fonts to re-rasterize on demand: changing a font's
// scale no longer bilinear-stretches a fixed bitmap, it draws crisp glyphs at
// the requested pixel size. The boost layer exposes that through two surfaces:
//
//   GetStyle().FontScaleMain = s        — whole-frame global scale. The 1.92
//                                          replacement for io.FontGlobalScale and
//                                          the removed SetWindowFontScale. Set it
//                                          before submitting any text.
//
//   with_font(font, size) { ... }        — per-push pixel size. The wrapper now
//                                          forwards `size` to PushFont, so one
//                                          font renders at several sizes in a
//                                          single frame. `size = 0.0f` (default)
//                                          keeps the current size, so
//                                          with_font(font) { ... } stays a pure
//                                          font swap.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/dynamic_fonts.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/dynamic_fonts.das
// =============================================================================

var private g_global_scale : float = 1.0f

[export]
def init() {
    live_create_window("dasImgui dynamic_fonts tutorial", 820, 560)
    live_imgui_init(live_window)
    let io & = unsafe(GetIO())
    GetStyle().FontScaleMain = 1.2
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplGlfw_NewFrame()
    apply_synth_io_override()
    NewFrame()

    // FontScaleMain is the per-frame global scale; the slider below feeds it.
    // Assign it before any text is submitted so the whole frame picks it up.
    GetStyle().FontScaleMain = g_global_scale

    SetNextWindowPos(ImVec2(40.0f, 40.0f), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(740.0f, 480.0f), ImGuiCond.Always)
    window(DF_WIN, (text = "dynamic_fonts", closable = false,
                    flags = ImGuiWindowFlags.None)) {
        text("imgui 1.92 re-rasterizes fonts on demand - scaling stays crisp.")
        separator()

        // ---- Lever 1: whole-frame global scale ----
        text("Drag to rescale every glyph this frame (FontScaleMain):")
        edit_slider_float(safe_addr(g_global_scale),
            (id = "GLOBAL_SCALE", text = "global scale", v_min = 0.5f, v_max = 2.5f))
        separator()

        // ---- Lever 2: per-push size via with_font(font, size) ----
        text("with_font(font, size) - one font, three sizes, one frame:")
        let f = GetFont()
        with_font(f, 16.0f) {
            text(SIZE_16, (text = "16 px - with_font(f, 16.0f)"))
        }
        with_font(f, 24.0f) {
            text(SIZE_24, (text = "24 px - with_font(f, 24.0f)"))
        }
        with_font(f, 32.0f) {
            text(SIZE_32, (text = "32 px - with_font(f, 32.0f)"))
        }
        separator()

        with_font(f) {
            text(SWAP_ONLY, (text = "with_font(f) with no size - pure swap, size kept"))
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
