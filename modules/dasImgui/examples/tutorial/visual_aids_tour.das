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
require imgui/imgui_visual_aids

// =============================================================================
// TUTORIAL: visual_aids_tour — the keeper reference example for Phase 4/5.
//
// Main window holds four widgets being demonstrated:
//   - STATUS    : a text_show display (label-style)
//   - VOLUME    : a slider_float (interactive input)
//   - SAVE_BTN  : a button (trigger)
//   - NAME_INPUT: an input_text (typing target for the keyboard tour)
//
// Side "Controls" window has buttons that drive the visual aids:
//   - Highlight each demoed widget
//   - Toggle the mouse trail
//   - Pop a narrate callout pointing at VOLUME / SAVE_BTN
//   - Toggle auto-highlight-on-command (highlights every remote action)
//
// DRIVE (also exposed via curl on localhost:9090):
//   curl -X POST -d '{"name":"imgui_highlight","args":{"target":"VOLUME"}}' localhost:9090/command
//   curl -X POST -d '{"name":"imgui_mouse_trail","args":{"enabled":true}}' localhost:9090/command
//   curl -X POST -d '{"name":"imgui_narrate","args":{"text":"click me","target":"SAVE_BTN","frames":180}}' localhost:9090/command
//   curl -X POST -d '{"name":"screenshot","args":{"file":"visual_aids_tour.png"}}' localhost:9090/command
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/visual_aids_tour.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/visual_aids_tour.das
// =============================================================================

[export]
def init() {
    live_create_window("Phase 4 — visual aids tour", 1000, 640)
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
    apply_synth_io_override()
    NewFrame()

    // ===== Main window — the demoed widgets =====
    SetNextWindowPos(float2(40.0f, 40.0f), ImGuiCond.FirstUseEver)
    SetNextWindowSize(float2(440.0f, 360.0f), ImGuiCond.FirstUseEver)
    if (Begin("Demoed widgets", null, ImGuiWindowFlags.None)) {
        Text("These widgets get highlighted, narrated, typed-into, etc.")
        Separator()
        text_show(STATUS, (value = "ready"))
        VOLUME.bounds = (0.0f, 100.0f)
        slider_float(VOLUME, (text = "Volume"))
        if (button(SAVE_BTN, (text = "Save"))) {
            print("save clicked, count={SAVE_BTN.click_count}\n")
        }
        Separator()
        input_text(NAME_INPUT, (text = "Name"))
        Text("buffer = {NAME_INPUT.value}")
    }
    End()

    // ===== Controls window — driver buttons for the aids =====
    SetNextWindowPos(float2(520.0f, 40.0f), ImGuiCond.FirstUseEver)
    SetNextWindowSize(float2(440.0f, 560.0f), ImGuiCond.FirstUseEver)
    if (Begin("Visual aids controls", null, ImGuiWindowFlags.None)) {
        Text("Highlight a widget (yellow rect, 60 frames):")
        Separator()
        if (button(BTN_HIGHLIGHT_STATUS, (text = "highlight STATUS"))) {
            highlight("STATUS")
        }
        if (button(BTN_HIGHLIGHT_VOLUME, (text = "highlight VOLUME"))) {
            highlight("VOLUME")
        }
        if (button(BTN_HIGHLIGHT_SAVE, (text = "highlight SAVE_BTN"))) {
            highlight("SAVE_BTN")
        }
        if (button(BTN_HIGHLIGHT_ALL, (text = "highlight all three"))) {
            highlight("STATUS")
            highlight("VOLUME")
            highlight("SAVE_BTN")
        }

        Spacing()
        Text("Mouse trail (fading dots following cursor):")
        Separator()
        if (button(BTN_TRAIL_ON, (text = "mouse trail ON"))) {
            mouse_trail(true)
        }
        if (button(BTN_TRAIL_OFF, (text = "mouse trail OFF"))) {
            mouse_trail(false)
        }

        Spacing()
        Text("Narrate callout (overlay box + connector line):")
        Separator()
        if (button(BTN_NARRATE_VOLUME, (text = "narrate VOLUME"))) {
            narrate("Drag this to set volume.", "VOLUME")
        }
        if (button(BTN_NARRATE_SAVE, (text = "narrate SAVE_BTN"))) {
            narrate("Click here to save.", "SAVE_BTN")
        }
        if (button(BTN_NARRATE_FLOATING, (text = "narrate floating"))) {
            narrate("Floating overlay, no target widget.")
        }

        Spacing()
        Text("Auto-highlight on command (flag, currently {imgui_auto_highlight_on_command}):")
        Separator()
        if (button(BTN_AUTO_ON, (text = "auto-highlight ON"))) {
            imgui_auto_highlight_on_command = true
        }
        if (button(BTN_AUTO_OFF, (text = "auto-highlight OFF"))) {
            imgui_auto_highlight_on_command = false
        }
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
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
