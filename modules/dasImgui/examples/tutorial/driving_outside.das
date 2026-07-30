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

let PRESETS : array<string> <- ["Calm", "Mellow", "Active", "Frantic"]
var PRESET_ROW : table<int; ClickState>

// nolint:STYLE014 — file-header tutorial banner (curl driving recipes) displaced below decls
// =============================================================================
// TUTORIAL: driving_outside — every boost widget is also a JSON endpoint.
//
// Previous tutorials wrote the daslang side. This one is the inverse view:
// what the JSON command surface looks like as a programming model in its
// own right. Every widget the boost layer ships registers a path in the
// telemetry tree; that path is also addressable from outside via the
// [live_command] HTTP endpoints `imgui_snapshot` / `imgui_force_set` /
// `imgui_click` / `imgui_open` / `imgui_close` / `imgui_focus`.
//
// The target app is small — slider + button + input + popup + combo — so
// the driving recipes are easy to spot. Every interaction the user could
// perform via mouse/keyboard has a curl equivalent.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/driving_outside.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/driving_outside.das
//
// DRIVE (curl recipes — pair these with the RST walkthrough):
//
//   # Snapshot the world (always the first read in any driver)
//   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command
//
//   # imgui_force_set — drive a slider value
//   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"DRIVE_WIN/SPEED","value":7}}' \
//        localhost:9090/command
//
//   # imgui_click — fire a button
//   curl -X POST -d '{"name":"imgui_click","args":{"target":"DRIVE_WIN/RUN_BTN"}}' \
//        localhost:9090/command
//
//   # imgui_force_set — string into a text input
//   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"DRIVE_WIN/USER","value":"Alice"}}' \
//        localhost:9090/command
//
//   # imgui_force_set — combo by selected-index
//   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"DRIVE_WIN/PRESET","value":2}}' \
//        localhost:9090/command
//
//   # imgui_open / imgui_close — popups, closable windows, tabs
//   curl -X POST -d '{"name":"imgui_open","args":{"target":"DRIVE_WIN/STATUS_POPUP"}}' \
//        localhost:9090/command
//   curl -X POST -d '{"name":"imgui_close","args":{"target":"DRIVE_WIN/STATUS_POPUP"}}' \
//        localhost:9090/command
// =============================================================================

[export]
def init() {
    live_create_window("dasImgui driving_outside tutorial", 880, 620)
    live_imgui_init(live_window)
    // Deterministic FirstUseEver layout for the recording: disable imgui.ini so a prior
    // session's window pos/size can't drift the framing. Tutorial-scoped on purpose.
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

    SetNextWindowPos(ImVec2(30.0f, 30.0f), ImGuiCond.FirstUseEver)
    SetNextWindowSize(ImVec2(640.0f, 460.0f), ImGuiCond.FirstUseEver)
    window(DRIVE_WIN, (text = "driving_outside", closable = false,
                       flags = ImGuiWindowFlags.None)) {

        // ---- A text input — driven by `imgui_force_set` with a string value ----
        input_text(USER, (text = "User"))
        text("USER.value = \"{USER.value}\"")

        separator(DR_SEP_1)

        // ---- A slider — driven by `imgui_force_set` with a number value ----
        slider_int(SPEED, (text = "Speed"))
        text("SPEED.value = {SPEED.value}")

        separator(DR_SEP_2)

        // ---- A combo — driven by `imgui_force_set` with the selected index ----
        var preset_label = "(none)"
        if (PRESET.value >= 0 && PRESET.value < length(PRESETS)) {
            preset_label = PRESETS[PRESET.value]
        }
        combo_select(PRESET, (text = "Preset",
                              preview_value = preset_label,
                              flags = ImGuiComboFlags.None)) {
            for (i in range(length(PRESETS))) {
                let is_sel = (i == PRESET.value)
                if (selectable_label(PRESET_ROW[i], PRESETS[i], is_sel)) {
                    PRESET.value = i
                }
            }
        }
        text("PRESET = {preset_label} (idx {PRESET.value})")

        separator(DR_SEP_3)

        // ---- A button — fired by `imgui_click` ----
        if (button(RUN_BTN, (text = "Run"))) {}
        text("RUN_BTN.click_count = {RUN_BTN.click_count}")

        separator(DR_SEP_4)

        // ---- A popup — opened/closed via `imgui_open` / `imgui_close` ----
        text("STATUS_POPUP - driven by imgui_open / imgui_close.")
        popup(STATUS_POPUP, (text = "StatusPopup",
                             flags = ImGuiWindowFlags.None)) {
            text("Driven from outside via imgui_open.")
            separator(DR_SEP_5)
            text("RUN_BTN.click_count = {RUN_BTN.click_count}")
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
