options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: display_image — image() widget against the ImGui font atlas texture.
// SHOWS:   ImageState payload carries size / uv0 / uv1 / tint_col / border_col
//          plus tex_id_int (reinterpret of texture handle as uint64 for
//          snapshot readability). Uses the font atlas texture from
//          GetIO().Fonts.TexRef since dasImgui standalone has no built-in
//          image loader; swap for any real texture handle on application
//          integration.
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command \
//              | jq '.globals."MAIN_WIN/IMG_BASIC".payload'
// STANDALONE: daslang.exe modules/dasImgui/examples/features/display_image.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/display_image.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/display_image.das
// =============================================================================

[export]
def init() {
    harness_init("display_image — boost v2 §3.2", 720, 420)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(680.0, 360.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "image() against font atlas",
                      closable = false, flags = ImGuiWindowFlags.None)) {
        let io & = unsafe(GetIO())
        if (io.Fonts.TexData == null) {
            text(LEAD, (text = "Font atlas not yet built — wait for first frame."))
        } else {
            text(LEAD, (text = "Three image() calls against the same atlas — varying size, uv, tint."))
            image(IMG_BASIC, (user_texture_id = io.Fonts.TexRef,
                              size = float2(128.0f, 128.0f),
                              uv0 = float2(0.0f, 0.0f),
                              uv1 = float2(1.0f, 1.0f),
                              tint_col = float4(1.0f, 1.0f, 1.0f, 1.0f),
                              border_col = float4(0.0f, 0.0f, 0.0f, 0.0f)))
            same_line(GAP)
            image(IMG_HALF, (user_texture_id = io.Fonts.TexRef,
                             size = float2(96.0f, 96.0f),
                             uv0 = float2(0.0f, 0.0f),
                             uv1 = float2(0.5f, 0.5f),
                             tint_col = float4(1.0f, 0.5f, 0.5f, 1.0f),
                             border_col = float4(0.0f, 0.0f, 0.0f, 0.0f)))
            same_line(GAP2)
            image(IMG_TINTED, (user_texture_id = io.Fonts.TexRef,
                               size = float2(64.0f, 64.0f),
                               uv0 = float2(0.0f, 0.0f),
                               uv1 = float2(1.0f, 1.0f),
                               tint_col = float4(0.0f, 0.95f, 0.5f, 1.0f),
                               border_col = float4(1.0f, 1.0f, 1.0f, 1.0f)))
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
