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
// TUTORIAL: widgets_tour — one of every common boost widget on a single panel.
//
// Frame as an audio-settings dialog so each widget reads contextually:
//   USER_NAME  : input_text   — who's listening
//   VOLUME     : slider_float — master volume
//   MUTED      : checkbox     — mute toggle
//   QUALITY    : combo        — codec quality preset
//   TINT       : color_edit3  — accent color for the UI
//   SAVE_BTN   : button       — apply the settings
//
// Read alongside :ref:`tutorial_boost_basics` for the frame-loop shape; this
// tutorial only adds widgets on top of that scaffold.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/widgets_tour.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/widgets_tour.das
//
// DRIVE (when running live):
//   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"AUDIO_WIN/USER_NAME","value":"Boris"}}'        localhost:9090/command
//   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"AUDIO_WIN/VOLUME","value":0.75}}'              localhost:9090/command
//   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"AUDIO_WIN/MUTED","value":true}}'               localhost:9090/command
//   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"AUDIO_WIN/QUALITY","value":2}}'                localhost:9090/command
//   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"AUDIO_WIN/TINT","value":{"x":0.2,"y":0.8,"z":0.4}}}' localhost:9090/command
//   curl -X POST -d '{"name":"imgui_click","args":{"target":"AUDIO_WIN/SAVE_BTN"}}'                       localhost:9090/command
// =============================================================================

[export]
def init() {
    live_create_window("dasImgui widgets tour", 1024, 720)
    live_imgui_init(live_window)
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

    // Sized for content with breathing room on the right + below for narrate
    // post-its to appear without overlapping the panel.
    SetNextWindowPos(ImVec2(60.0, 60.0), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(720.0, 540.0), ImGuiCond.Always)
    window(AUDIO_WIN, (text = "Audio settings", closable = false,
                       flags = ImGuiWindowFlags.None)) {
        // Text input — buffer + string-mirror managed by the state struct.
        input_text(USER_NAME, (text = "Your name"))

        spacing(WT_SP_1)

        // Slider — float state with [0, 1] bounds set per-frame.
        VOLUME.bounds = (0.0f, 1.0f)
        slider_float(VOLUME, (text = "Master volume"))

        // Checkbox — bool state, toggles on click.
        checkbox(MUTED, (text = "Mute"))

        spacing(WT_SP_2)

        // Combo — int state indexing into the items array.
        combo(QUALITY, (text = "Quality",
                        items <- ["Low", "Medium", "High", "Ultra"]))

        spacing(WT_SP_3)

        // Color editor — three-channel RGB, opens a picker on click.
        color_edit3(TINT, (text = "Accent color"))

        spacing(WT_SP_4)
        separator(WT_SEP_1)
        spacing(WT_SP_5)

        // Button — true on the frame the click registers; click_count accumulates.
        if (button(SAVE_BTN, (text = "Save settings"))) {
            print("save clicked: name={USER_NAME.value} vol={VOLUME.value} muted={MUTED.value} q={QUALITY.value}\n")
        }

        spacing(WT_SP_6)
        text("saves so far: {SAVE_BTN.click_count}")
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
