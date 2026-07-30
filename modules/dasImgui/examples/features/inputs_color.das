options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: inputs_color — every Phase 0b.2 Color* widget in one window.
// SHOWS:   color_edit3 / color_edit4 / color_picker3 / color_picker4 / color_button
// DRIVE:   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"MAIN_WIN/BG_COLOR","value":[0.2,0.5,0.9,1.0]}}' \
//               localhost:9090/command
//          curl -X POST -d '{"name":"imgui_click","args":{"target":"MAIN_WIN/PALETTE_RED"}}' \
//               localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/inputs_color.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/inputs_color.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/inputs_color.das
// =============================================================================

[export]
def init() {
    harness_init("Phase 0b.2 — color inputs", 800, 760)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(720.0, 720.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "Color inputs — phase 0b.2", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        // ----- Editors (compact RGB / RGBA boxes) -----
        text("Editors")
        separator()
        color_edit3(TINT_RGB, (text = "Tint RGB"))
        text("rgb = ({TINT_RGB.value.x}, {TINT_RGB.value.y}, {TINT_RGB.value.z})")

        color_edit4(BG_COLOR, (text = "Background"))
        text("bg = ({BG_COLOR.value.x}, {BG_COLOR.value.y}, {BG_COLOR.value.z}, {BG_COLOR.value.w})")

        spacing()

        // ----- Pickers (full popup-shaped wheels) -----
        text("Pickers")
        separator()
        color_picker3(SUN_RGB, (text = "Sun light"))
        text("sun = ({SUN_RGB.value.x}, {SUN_RGB.value.y}, {SUN_RGB.value.z})")

        // ColorPicker4 with reference color (shows the [ref] swatch). Named-tuple
        // args are positional in SOURCE ORDER, so surfacing ref_col / has_ref_col
        // means also passing the intermediate `flags` slot (even at default).
        color_picker4(SHIRT_COLOR, (text = "Shirt",
                                    flags = ImGuiColorEditFlags.None,
                                    ref_col = float4(0.5f, 0.5f, 0.5f, 1.0f),
                                    has_ref_col = true))
        text("shirt = ({SHIRT_COLOR.value.x}, {SHIRT_COLOR.value.y}, {SHIRT_COLOR.value.z}, {SHIRT_COLOR.value.w})")

        spacing()

        // ----- ColorButton swatches (click triggers; color is per-call) -----
        text("Palette (click swatches)")
        separator()
        color_button(PALETTE_RED, (desc_id = "##red", col = float4(1.0f, 0.0f, 0.0f, 1.0f),
                                   size = float2(48.0f, 48.0f)))
        same_line((spacing = 8.0f))
        color_button(PALETTE_GREEN, (desc_id = "##green", col = float4(0.0f, 1.0f, 0.0f, 1.0f),
                                     size = float2(48.0f, 48.0f)))
        same_line((spacing = 8.0f))
        color_button(PALETTE_BLUE, (desc_id = "##blue", col = float4(0.0f, 0.0f, 1.0f, 1.0f),
                                    size = float2(48.0f, 48.0f)))
        text("palette clicks: red={PALETTE_RED.click_count}, green={PALETTE_GREEN.click_count}, blue={PALETTE_BLUE.click_count}")
    }

    harness_end_frame()
}

[export]
def shutdown() {
    harness_shutdown()
}

[export]
def main() {
    //! Standalone entrypoint. daslang-live drives init/update/shutdown
    //! directly and ignores main(); plain `daslang.exe <file>` runs this loop.
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
