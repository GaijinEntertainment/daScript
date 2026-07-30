options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: color_button_hover — hover-target variant of color_button.
//          Renders a swatch and returns per-frame hover bool from
//          IsItemHovered(); no click state, no toggle.
// SHOWS:   three swatches with a status line that follows whichever is
//          currently hovered.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/color_button_hover.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/color_button_hover.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/color_button_hover.das
// =============================================================================

var private STATE_MESSAGE = "Hover any swatch."

[export]
def init() {
    harness_init("color_button_hover — hover target", 560, 320)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(500.0, 260.0), ImGuiCond.Always)
    window(CBH_WIN, (text = "color_button_hover", closable = false,
                     flags = ImGuiWindowFlags.None)) {
        let swatch_flags = (ImGuiColorEditFlags.NoTooltip | ImGuiColorEditFlags.NoDragDrop)
        let red = color_button_hover(CBH_RED,
            (desc_id = "##red", col = float4(0.85f, 0.20f, 0.20f, 1.0f),
             size = float2(96.0f, 64.0f), flags = swatch_flags))
        same_line(CBH_SL_1)
        let green = color_button_hover(CBH_GREEN,
            (desc_id = "##green", col = float4(0.20f, 0.85f, 0.30f, 1.0f),
             size = float2(96.0f, 64.0f), flags = swatch_flags))
        same_line(CBH_SL_2)
        let blue = color_button_hover(CBH_BLUE,
            (desc_id = "##blue", col = float4(0.20f, 0.40f, 0.85f, 1.0f),
             size = float2(96.0f, 64.0f), flags = swatch_flags))

        if (red) {
            STATE_MESSAGE = "Hovering: red"
        } elif (green) {
            STATE_MESSAGE = "Hovering: green"
        } elif (blue) {
            STATE_MESSAGE = "Hovering: blue"
        }
        text(CBH_STATUS, (text = STATE_MESSAGE))
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
