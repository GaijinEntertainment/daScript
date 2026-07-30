options gen2
options _comment_hygiene = true

require math
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
// TUTORIAL: toggles — three forms of pick-this-not-that input.
//
//   checkbox(IDENT, (text = "..."))           — single bool, click flips.
//   radio_button(IDENT, (text = "..."))       — single bool, circle-rendered.
//                                                Same ToggleState as checkbox;
//                                                difference is purely visual.
//   radio_button_int(IDENT, (text = "...",    — N call sites SHARE one
//                            v_button = N))     RadioIntState. Click writes
//                                                state.value = v_button — the
//                                                canonical "pick one of N".
//
// All three accept imgui_force_set: checkbox / radio_button take bool, radio_button_int
// takes int (the v_button to select).
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/toggles.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/toggles.das
//
// DRIVE (when running live):
//   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"TG_WIN/T_CHECK","value":true}}' \
//        localhost:9090/command
//   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"TG_WIN/T_MODE","value":2}}' \
//        localhost:9090/command
// =============================================================================

[export]
def init() {
    live_create_window("dasImgui toggles tutorial", 760, 560)
    live_imgui_init(live_window)
    let io & = unsafe(GetIO())
    GetStyle().FontScaleMain = 1.4
}

let MODE_LABELS = fixed_array("Off", "Manual", "Auto")

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplGlfw_NewFrame()
    apply_synth_io_override()
    NewFrame()

    SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(720.0f, 520.0f), ImGuiCond.Always)
    window(TG_WIN, (text = "toggles tutorial", closable = false,
                    flags = ImGuiWindowFlags.None)) {

        text("Three shapes of pick-this-not-that.")
        text(T_HINT, (text = "checkbox + radio_button share ToggleState. radio_button_int is the grouped form."))
        separator()

        // ---- Stage 1: checkbox — single bool, square widget ----
        checkbox(T_CHECK, (text = "Enabled"))
        text("T_CHECK.value = {T_CHECK.value}")
        spacing()

        // ---- Stage 2: radio_button (bool form) — circle-rendered checkbox ----
        radio_button(T_RADIO_BOOL, (text = "Subscribe"))
        text("T_RADIO_BOOL.value = {T_RADIO_BOOL.value}  // same ToggleState as checkbox")
        spacing()

        // ---- Stage 3: radio_button_int — three sites, ONE shared state ----
        text("Operation mode:")
        radio_button_int(T_MODE, (text = "Off",    v_button = 0)); same_line(T_SL1)
        radio_button_int(T_MODE, (text = "Manual", v_button = 1)); same_line(T_SL2)
        radio_button_int(T_MODE, (text = "Auto",   v_button = 2))
        text("T_MODE.value = {T_MODE.value} ('{MODE_LABELS[clamp(T_MODE.value, 0, 2)]}')")
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
