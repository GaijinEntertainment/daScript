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
    live_create_window("visual aids tour", 1000, 640)
    live_imgui_init(live_window)
    DisableIniPersistence()
    let io & = unsafe(GetIO())
    GetStyle().FontScaleMain = 1.5
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplGlfw_NewFrame()
    apply_synth_io_override()
    NewFrame()

    // ===== Subject window — the demoed widgets =====
    SetNextWindowPos(float2(40.0f, 40.0f), ImGuiCond.FirstUseEver)
    SetNextWindowSize(float2(440.0f, 360.0f), ImGuiCond.FirstUseEver)
    window(SUBJECT_WIN, (text = "Demoed widgets", closable = false,
                         flags = ImGuiWindowFlags.None)) {
        text("These widgets get highlighted, narrated, typed-into, etc.")
        separator(VA_SEP_1)
        text_show(STATUS, (value = "ready"))
        VOLUME.bounds = (0.0f, 100.0f)
        slider_float(VOLUME, (text = "Volume"))
        if (button(SAVE_BTN, (text = "Save"))) {
            print("save clicked, count={SAVE_BTN.click_count}\n")
        }
        separator(VA_SEP_2)
        input_text(NAME_INPUT, (text = "Name"))
        text("buffer = {NAME_INPUT.value}")
    }

    // ===== Controls window — driver buttons for the aids =====
    SetNextWindowPos(float2(520.0f, 40.0f), ImGuiCond.FirstUseEver)
    SetNextWindowSize(float2(440.0f, 560.0f), ImGuiCond.FirstUseEver)
    window(CONTROLS_WIN, (text = "Visual aids controls", closable = false,
                          flags = ImGuiWindowFlags.None)) {
        text("Highlight a widget (yellow rect, 60 frames):")
        separator(VA_SEP_3)
        if (button(BTN_HIGHLIGHT_STATUS, (text = "highlight STATUS"))) {
            highlight("SUBJECT_WIN/STATUS")
        }
        if (button(BTN_HIGHLIGHT_VOLUME, (text = "highlight VOLUME"))) {
            highlight("SUBJECT_WIN/VOLUME")
        }
        if (button(BTN_HIGHLIGHT_SAVE, (text = "highlight SAVE_BTN"))) {
            highlight("SUBJECT_WIN/SAVE_BTN")
        }
        if (button(BTN_HIGHLIGHT_ALL, (text = "highlight all three"))) {
            highlight("SUBJECT_WIN/STATUS")
            highlight("SUBJECT_WIN/VOLUME")
            highlight("SUBJECT_WIN/SAVE_BTN")
        }

        spacing(VA_SP_1)
        text("Mouse trail (fading dots following cursor):")
        separator(VA_SEP_4)
        if (button(BTN_TRAIL_ON, (text = "mouse trail ON"))) {
            mouse_trail(true)
        }
        if (button(BTN_TRAIL_OFF, (text = "mouse trail OFF"))) {
            mouse_trail(false)
        }

        spacing(VA_SP_2)
        text("Narrate callout (overlay box + connector line):")
        separator(VA_SEP_5)
        if (button(BTN_NARRATE_VOLUME, (text = "narrate VOLUME"))) {
            narrate("Drag this to set volume.", "SUBJECT_WIN/VOLUME")
        }
        if (button(BTN_NARRATE_SAVE, (text = "narrate SAVE_BTN"))) {
            narrate("Click here to save.", "SUBJECT_WIN/SAVE_BTN")
        }
        if (button(BTN_NARRATE_FLOATING, (text = "narrate floating"))) {
            narrate("Floating overlay, no target widget.")
        }

        spacing(VA_SP_3)
        text("Auto-highlight on command (flag, currently {imgui_auto_highlight_on_command}):")
        separator(VA_SEP_6)
        if (button(BTN_AUTO_ON, (text = "auto-highlight ON"))) {
            imgui_auto_highlight_on_command = true
        }
        if (button(BTN_AUTO_OFF, (text = "auto-highlight OFF"))) {
            imgui_auto_highlight_on_command = false
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
