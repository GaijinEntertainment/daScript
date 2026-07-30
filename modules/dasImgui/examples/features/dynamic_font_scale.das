options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require daslib/safe_addr

// =============================================================================
// FEATURE: dynamic_font_scale — imgui 1.92 dynamic-font scaling, two levers.
// SHOWS:   (1) GetStyle().FontScaleMain — the global font scale, driven live
//              from a slider (the 1.92 replacement for io.FontGlobalScale /
//              the removed SetWindowFontScale). Scaling re-rasterizes the atlas
//              on demand instead of bilinear-stretching a fixed-size bitmap.
//          (2) with_font(font, size) — the boost wrapper now forwards a per-push
//              pixel size to PushFont, so the SAME font renders at several sizes
//              in one frame. `size = 0.0f` (the default) keeps the current size,
//              so `with_font(font) { ... }` stays a pure font swap.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/dynamic_font_scale.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/dynamic_font_scale.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/dynamic_font_scale.das
// =============================================================================

var private g_global_scale : float = 1.0f

[export]
def init() {
    harness_init("dynamic_font_scale — imgui 1.92 dynamic fonts", 760, 520)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    // FontScaleMain is the per-frame global font scale; assign it before any
    // text is submitted so the whole frame picks it up.
    GetStyle().FontScaleMain = g_global_scale

    SetNextWindowSize(ImVec2(720.0, 460.0), ImGuiCond.Always)
    window(FONT_WIN, (text = "dynamic_font_scale", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "imgui 1.92 re-rasterizes fonts on demand — scaling is crisp, not stretched."))
        separator()

        // ---- Lever 1: global scale ----
        text("FontScaleMain (whole-frame global scale):")
        edit_slider_float(safe_addr(g_global_scale), (id = "GLOBAL_SCALE",
                                                      text = "global scale",
                                                      v_min = 0.5f, v_max = 2.5f))
        text(SCALE_ECHO, (text = "every glyph this frame is scaled by the value above"))
        separator()

        // ---- Lever 2: per-push size via with_font(font, size) ----
        text("with_font(font, size) — same font, three sizes, one frame:")
        let f = GetFont()
        with_font(f, 16.0f) {
            text(SIZE_16, (text = "16 px — with_font(f, 16.0f)"))
        }
        with_font(f, 24.0f) {
            text(SIZE_24, (text = "24 px — with_font(f, 24.0f)"))
        }
        with_font(f, 32.0f) {
            text(SIZE_32, (text = "32 px — with_font(f, 32.0f)"))
        }
        separator()

        with_font(f) {
            text(SWAP_ONLY, (text = "with_font(f) with no size — pure swap, current size kept"))
        }
    }

    harness_end_frame()
}

[export]
def shutdown() {
    harness_shutdown()
}

[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
