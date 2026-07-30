options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: inputs_slider — every Phase 0b.2 Slider* widget in one window.
// SHOWS:   slider_float / slider_float2/3/4
//          slider_int   / slider_int2/3/4
//          vslider_float / vslider_int
// DRIVE:   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"MAIN_WIN/GAIN","value":0.7}}' \
//               localhost:9090/command
//          curl -X POST -d '{"name":"imgui_force_set","args":{"target":"MAIN_WIN/WAVE_RGB","value":[0.1,0.5,0.9]}}' \
//               localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/inputs_slider.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/inputs_slider.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/inputs_slider.das
// =============================================================================

[export]
def init() {
    harness_init("Phase 0b.2 — slider inputs", 800, 800)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(720.0, 760.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "Slider inputs — phase 0b.2", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        // ----- Float scalars -----
        text("Float scalars")
        separator()
        GAIN.bounds = (0.0f, 1.0f)
        slider_float(GAIN, (text = "Gain"))
        text("gain = {GAIN.value}")

        // ----- Float vectors -----
        text("Float vectors")
        separator()
        UV_OFFSET.bounds = (-1.0f, 1.0f)
        slider_float2(UV_OFFSET, (text = "UV offset"))
        text("uv = ({UV_OFFSET.value.x}, {UV_OFFSET.value.y})")

        WAVE_RGB.bounds = (0.0f, 1.0f)
        slider_float3(WAVE_RGB, (text = "Wave RGB"))
        text("rgb = ({WAVE_RGB.value.x}, {WAVE_RGB.value.y}, {WAVE_RGB.value.z})")

        WAVE_RGBA.bounds = (0.0f, 1.0f)
        slider_float4(WAVE_RGBA, (text = "Wave RGBA"))
        text("a = {WAVE_RGBA.value.w}")

        spacing()

        // ----- Int scalars -----
        text("Int scalars")
        separator()
        LIVES.bounds = (0, 9)
        slider_int(LIVES, (text = "Lives"))
        text("lives = {LIVES.value}")

        // ----- Int vectors -----
        text("Int vectors")
        separator()
        SHIFT_XY.bounds = (-8, 8)
        slider_int2(SHIFT_XY, (text = "Shift XY"))
        text("shift = ({SHIFT_XY.value.x}, {SHIFT_XY.value.y})")

        TINT_BUDGET_RGB.bounds = (0, 255)
        slider_int3(TINT_BUDGET_RGB, (text = "Tint budget"))
        text("budget = ({TINT_BUDGET_RGB.value.x}, {TINT_BUDGET_RGB.value.y}, {TINT_BUDGET_RGB.value.z})")

        FRAME_LTRB.bounds = (0, 512)
        slider_int4(FRAME_LTRB, (text = "Frame LTRB"))
        text("frame = [{FRAME_LTRB.value.x}, {FRAME_LTRB.value.y}, {FRAME_LTRB.value.z}, {FRAME_LTRB.value.w}]")

        spacing()

        // ----- Vertical sliders (side by side) -----
        text("Vertical sliders")
        separator()
        VOL_L.bounds = (0.0f, 1.0f)
        VOL_R.bounds = (0.0f, 1.0f)
        BUS_GAIN.bounds = (-12, 12)
        vslider_float(VOL_L, (text = "L"))
        same_line((spacing = 12.0f))
        vslider_float(VOL_R, (text = "R"))
        same_line((spacing = 12.0f))
        vslider_int(BUS_GAIN, (text = "Bus dB", size = float2(40.0f, 200.0f)))
        text("levels: L={VOL_L.value}, R={VOL_R.value}, bus={BUS_GAIN.value}dB")
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
