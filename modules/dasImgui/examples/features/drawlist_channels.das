options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: drawlist channels family — channels_split / channels_set_current /
//          channels_merge.
// SHOWS:   Split the window drawlist into 2 channels; submit the foreground
//          content first (channel 1), then go BACK and paint the background
//          card behind it (channel 0). After channels_merge, channel 0
//          renders first (under) and channel 1 on top. Without channels you
//          would need to know all bbox sizes UPFRONT to paint the
//          background before the content.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/drawlist_channels.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/drawlist_channels.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/drawlist_channels.das
// =============================================================================

[export]
def init() {
    harness_init("drawlist channels — split / merge", 520, 300)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(480.0, 260.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "drawlist_channels", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "Yellow card painted on channel 0 (background) AFTER the white text on channel 1 (foreground)."))

        let origin = GetCursorScreenPos()
        let card_bg = rgba(220u, 220u, 100u, 220u)
        let white   = rgba(255u, 255u, 255u, 255u)

        with_window_drawlist() $(var dl) {
            dl |> channels_split(2)

            // ---- Channel 1 (foreground): submit content first to measure ----
            dl |> channels_set_current(1)
            let text_pos = float2(origin.x + 24.0f, origin.y + 20.0f)
            dl |> add_text(text_pos, white, "FOREGROUND TEXT — painted before the bg")
            let txt_sz = CalcTextSize("FOREGROUND TEXT — painted before the bg")

            // ---- Channel 0 (background): paint the card behind ----
            dl |> channels_set_current(0)
            dl |> add_rect_filled(
                float2(text_pos.x - 12.0f, text_pos.y - 8.0f),
                float2(text_pos.x + txt_sz.x + 12.0f, text_pos.y + txt_sz.y + 8.0f),
                card_bg, 4.0f, ImDrawFlags.None)

            dl |> channels_merge()
        }

        dummy(CARD_SLOT, (size = float2(460.0f, 80.0f)))
        text(NOTE, (text = "(Without channels_split, the foreground text would render BENEATH the later card_bg rect — wrong z-order.)"))
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
