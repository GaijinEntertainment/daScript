options gen2

require imgui
require imgui_app
require glfw/glfw_boost
require opengl/opengl_boost
require live/glfw_live
require live/live_api
require live/live_commands
require live/live_vars
require live/opengl_live
require live_host
require imgui/imgui_live
require imgui/imgui_boost_runtime
require imgui/imgui_boost_v2
require imgui/imgui_widgets_builtin

// =============================================================================
// FEATURE: inputs_slider — every Phase 0b.2 Slider* widget in one window.
// SHOWS:   slider_float / slider_float2/3/4
//          slider_int   / slider_int2/3/4
//          vslider_float / vslider_int
// DRIVE:   curl -X POST -d '{"name":"imgui_set","args":{"target":"GAIN","value":0.7}}' \
//               localhost:9090/command
//          curl -X POST -d '{"name":"imgui_set","args":{"target":"WAVE_RGB","value":[0.1,0.5,0.9]}}' \
//               localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/inputs_slider.das
// LIVE:       daslang-live modules/dasImgui/examples/features/inputs_slider.das
// =============================================================================

[export]
def init() {
    live_create_window("Phase 0b.2 — slider inputs", 800, 800)
    live_imgui_init(live_window)
    var io & = unsafe(GetIO())
    io.FontGlobalScale = 1.5
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplOpenGL3_NewFrame()
    ImGui_ImplGlfw_NewFrame()
    NewFrame()

    SetNextWindowSize(ImVec2(720.0, 760.0), ImGuiCond.Always)
    if (Begin("Slider inputs — phase 0b.2", null, ImGuiWindowFlags.None)) {
        // ----- Float scalars -----
        Text("Float scalars")
        Separator()
        GAIN.bounds = (0.0f, 1.0f)
        slider_float(GAIN, (text = "Gain"))
        Text("gain = {GAIN.value}")

        // ----- Float vectors -----
        Text("Float vectors")
        Separator()
        UV_OFFSET.bounds = (-1.0f, 1.0f)
        slider_float2(UV_OFFSET, (text = "UV offset"))
        Text("uv = ({UV_OFFSET.value.x}, {UV_OFFSET.value.y})")

        WAVE_RGB.bounds = (0.0f, 1.0f)
        slider_float3(WAVE_RGB, (text = "Wave RGB"))
        Text("rgb = ({WAVE_RGB.value.x}, {WAVE_RGB.value.y}, {WAVE_RGB.value.z})")

        WAVE_RGBA.bounds = (0.0f, 1.0f)
        slider_float4(WAVE_RGBA, (text = "Wave RGBA"))
        Text("a = {WAVE_RGBA.value.w}")

        Spacing()

        // ----- Int scalars -----
        Text("Int scalars")
        Separator()
        LIVES.bounds = (0, 9)
        slider_int(LIVES, (text = "Lives"))
        Text("lives = {LIVES.value}")

        // ----- Int vectors -----
        Text("Int vectors")
        Separator()
        SHIFT_XY.bounds = (-8, 8)
        slider_int2(SHIFT_XY, (text = "Shift XY"))
        Text("shift = ({SHIFT_XY.value.x}, {SHIFT_XY.value.y})")

        TINT_BUDGET_RGB.bounds = (0, 255)
        slider_int3(TINT_BUDGET_RGB, (text = "Tint budget"))
        Text("budget = ({TINT_BUDGET_RGB.value.x}, {TINT_BUDGET_RGB.value.y}, {TINT_BUDGET_RGB.value.z})")

        FRAME_LTRB.bounds = (0, 512)
        slider_int4(FRAME_LTRB, (text = "Frame LTRB"))
        Text("frame = [{FRAME_LTRB.value.x}, {FRAME_LTRB.value.y}, {FRAME_LTRB.value.z}, {FRAME_LTRB.value.w}]")

        Spacing()

        // ----- Vertical sliders (side by side) -----
        Text("Vertical sliders")
        Separator()
        VOL_L.bounds = (0.0f, 1.0f)
        VOL_R.bounds = (0.0f, 1.0f)
        BUS_GAIN.bounds = (-12, 12)
        vslider_float(VOL_L, (text = "L"))
        SameLine(0.0f, 12.0f)
        vslider_float(VOL_R, (text = "R"))
        SameLine(0.0f, 12.0f)
        vslider_int(BUS_GAIN, (text = "Bus dB", size = float2(40.0f, 200.0f)))
        Text("levels: L={VOL_L.value}, R={VOL_R.value}, bus={BUS_GAIN.value}dB")
    }
    End()

    end_of_frame()
    Render()
    var w, h : int
    live_get_framebuffer_size(w, h)
    glViewport(0, 0, w, h)
    glClearColor(0.10f, 0.10f, 0.12f, 1.0f)
    glClear(GL_COLOR_BUFFER_BIT)
    ImGui_ImplOpenGL3_RenderDrawData(GetDrawData())

    live_end_frame()
}

[export]
def shutdown() {
    live_imgui_shutdown()
    live_destroy_window()
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
