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
// FEATURE: inputs_color — every Phase 0b.2 Color* widget in one window.
// SHOWS:   color_edit3 / color_edit4 / color_picker3 / color_picker4 / color_button
// DRIVE:   curl -X POST -d '{"name":"imgui_set","args":{"target":"BG_COLOR","value":[0.2,0.5,0.9,1.0]}}' \
//               localhost:9090/command
//          curl -X POST -d '{"name":"imgui_click","args":{"target":"PALETTE_RED"}}' \
//               localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/inputs_color.das
// LIVE:       daslang-live modules/dasImgui/examples/features/inputs_color.das
// =============================================================================

[export]
def init() {
    live_create_window("Phase 0b.2 — color inputs", 800, 760)
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

    SetNextWindowSize(ImVec2(720.0, 720.0), ImGuiCond.Always)
    if (Begin("Color inputs — phase 0b.2", null, ImGuiWindowFlags.None)) {
        // ----- Editors (compact RGB / RGBA boxes) -----
        Text("Editors")
        Separator()
        color_edit3(TINT_RGB, (text = "Tint RGB"))
        Text("rgb = ({TINT_RGB.value.x}, {TINT_RGB.value.y}, {TINT_RGB.value.z})")

        color_edit4(BG_COLOR, (text = "Background"))
        Text("bg = ({BG_COLOR.value.x}, {BG_COLOR.value.y}, {BG_COLOR.value.z}, {BG_COLOR.value.w})")

        Spacing()

        // ----- Pickers (full popup-shaped wheels) -----
        Text("Pickers")
        Separator()
        color_picker3(SUN_RGB, (text = "Sun light"))
        Text("sun = ({SUN_RGB.value.x}, {SUN_RGB.value.y}, {SUN_RGB.value.z})")

        // ColorPicker4 with reference color (shows the [ref] swatch).
        // Named-tuple args are positional in SOURCE ORDER, so when
        // surfacing ref_col / has_ref_col we have to also pass the
        // intermediate `flags` slot (even at default).
        color_picker4(SHIRT_COLOR, (text = "Shirt",
                                    flags = ImGuiColorEditFlags.None,
                                    ref_col = float4(0.5f, 0.5f, 0.5f, 1.0f),
                                    has_ref_col = true))
        Text("shirt = ({SHIRT_COLOR.value.x}, {SHIRT_COLOR.value.y}, {SHIRT_COLOR.value.z}, {SHIRT_COLOR.value.w})")

        Spacing()

        // ----- ColorButton swatches (click triggers; color is per-call) -----
        Text("Palette (click swatches)")
        Separator()
        color_button(PALETTE_RED, (desc_id = "##red", col = float4(1.0f, 0.0f, 0.0f, 1.0f),
                                   size = float2(48.0f, 48.0f)))
        SameLine(0.0f, 8.0f)
        color_button(PALETTE_GREEN, (desc_id = "##green", col = float4(0.0f, 1.0f, 0.0f, 1.0f),
                                     size = float2(48.0f, 48.0f)))
        SameLine(0.0f, 8.0f)
        color_button(PALETTE_BLUE, (desc_id = "##blue", col = float4(0.0f, 0.0f, 1.0f, 1.0f),
                                    size = float2(48.0f, 48.0f)))
        Text("palette clicks: red={PALETTE_RED.click_count}, green={PALETTE_GREEN.click_count}, blue={PALETTE_BLUE.click_count}")
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
