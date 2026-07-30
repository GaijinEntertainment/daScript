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
// TUTORIAL: state_telemetry — how widget state lives in daslang, not ImGui.
//
// Each boost widget macro emits a module-scope global named by the first
// argument. The global is a typed state struct (ClickState, SliderStateInt,
// SliderStateFloat, ToggleState, ...) that holds the widget's value, plus
// any pending overrides queued by external drivers. Three immediate wins:
//
//   1. Auto-emit:        the variable is declared once, by the macro, on
//                        first compile. No "var SAVE_BTN : ClickState;"
//                        boilerplate to keep in sync with the call site.
//   2. Read anywhere:    SAVE_BTN.click_count, SPEED.value are plain
//                        globals — readable from any module that requires
//                        this one (assuming PUBLIC visibility).
//   3. Dotted flags:     IDENT.PUBLIC / IDENT.PRIVATE / IDENT.NOTLIVE
//                        modify visibility / live-reload behavior on the
//                        emitted global without claiming new syntactic
//                        positions in the call.
//
// Plus `imgui_snapshot` — the registry serializes every registered widget
// to JSON: kind, bbox, hex_id, payload (value / click_count / ...). That's
// the surface external drivers, integration tests, and visual aids see.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/state_telemetry.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/state_telemetry.das
//
// DRIVE (when running live):
//   curl -X POST -d '{"name":"imgui_snapshot"}'                                                       localhost:9090/command
//   curl -X POST -d '{"name":"imgui_click","args":{"target":"STATE_WIN/SAVE_BTN"}}'                    localhost:9090/command
//   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"STATE_WIN/SPEED","value":7}}'               localhost:9090/command
//   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"STATE_WIN/STATUS_TEXT","value":"saved"}}'   localhost:9090/command
// =============================================================================

[export]
def init() {
    live_create_window("dasImgui state_telemetry tutorial", 1040, 720)
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

    SetNextWindowPos(ImVec2(30.0f, 30.0f), ImGuiCond.FirstUseEver)
    SetNextWindowSize(ImVec2(640.0f, 460.0f), ImGuiCond.FirstUseEver)
    window(STATE_WIN, (text = "state & telemetry", closable = false,
                       flags = ImGuiWindowFlags.None)) {

        // ---- Auto-emit + read-anywhere ----

        // No top-of-file `var SAVE_BTN : ClickState` — the macro emits the global
        // on first `button(SAVE_BTN, ...)`. Read its fields directly: .click_count
        // (cumulative, @live-preserved) and .clicked (true the frame it fired).
        text("Auto-emit: SAVE_BTN is the macro-emitted global.")
        if (button(SAVE_BTN, (text = "Save"))) {
            // `button(...)` returns bool — clicked-this-frame. Same info
            // as SAVE_BTN.clicked, just inline.
        }
        text("SAVE_BTN.click_count = {SAVE_BTN.click_count}")

        separator(ST_SEP_1)

        // ---- Dotted flags ----

        // SPEED.PUBLIC: emit the global `variable public` (not the default private)
        // so requiring modules can read SPEED.value. Path stays "SPEED" — flags
        // don't leak into the registry path.

        // VOLUME.NOTLIVE: skip @live on the emitted global, so on reload the
        // source-side initial value wins (useful when you change bounds and want
        // them to take effect immediately rather than be preserved).
        text("Dotted flags tune visibility and live-reload behavior:")
        slider_int(SPEED.PUBLIC,   (text = "Speed (int, PUBLIC)"))
        slider_float(VOLUME.NOTLIVE, (text = "Volume (NOTLIVE)"))
        text("SPEED.value = {SPEED.value}   VOLUME.value = {VOLUME.value}")

        separator(ST_SEP_2)

        // ---- text_show: app-side value mirror ----

        // text_show is the read-only mirror of text_input: its state.value string
        // is displayed, imgui_force_set drives it from outside, and the snapshot
        // exposes it under payload.value so tests can assert computed strings.
        text("text_show - app-driven status reaches the snapshot:")
        text_show(STATUS_TEXT)

        // The bump button writes a computed string into STATUS_TEXT.value.
        // Both `:= "..."` (clone-string) and external imgui_force_set work; the
        // snapshot reflects whichever ran most recently.
        if (button(BUMP_STATUS, (text = "bump status"))) {
            STATUS_TEXT.value := "saved at frame {get_uptime()}"
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
