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
// TUTORIAL: popup_modal — blocking modal dialog with optional X-button.
//
//   popup_modal(IDENT, (text = "title", closable = bool,
//                       flags = ImGuiWindowFlags....)) { body }
//
// Lifecycle is identical to popup() — OpenPopup + BeginPopupModal/EndPopup,
// driven by state.pending_open / pending_close — but ImGui ALSO:
//
//   1. Dims the parent window — visual backdrop says "deal with this first".
//   2. Blocks input outside the modal — clicks elsewhere are absorbed, not
//      passed through. ESC closes the modal (vanilla ImGui behavior).
//   3. With closable=true, renders an X-button wired to state.open.
//
// The boost wrapper threads three control surfaces through state.pending_*:
//   - app code        — `MODAL_X.pending_open = true`
//   - live commands   — imgui_open / imgui_close
//   - close-button    — ImGui flips state.open when X clicked or ESC pressed
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/popup_modal.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/popup_modal.das
//
// DRIVE (when running live):
//   curl -X POST -d '{"name":"imgui_open","args":{"target":"PM_WIN/CONFIRM_MODAL"}}' \
//        localhost:9090/command
// =============================================================================

var private CONFIRM_RESULT : string = "(no answer yet)"

[export]
def init() {
    live_create_window("dasImgui popup_modal tutorial", 720, 480)
    live_imgui_init(live_window)
    let io & = unsafe(GetIO())
    GetStyle().FontScaleMain = 1.4
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplGlfw_NewFrame()
    apply_synth_io_override()
    NewFrame()

    SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(680.0f, 440.0f), ImGuiCond.Always)
    window(PM_WIN, (text = "popup_modal tutorial", closable = false,
                    flags = ImGuiWindowFlags.None)) {

        text("Two modals - a Yes/No confirm and a closable settings dialog.")
        separator()

        // ---- A: trigger the confirm modal ----
        if (button(OPEN_CONFIRM, (text = "Delete file..."))) {
            CONFIRM_MODAL.pending_open = true
        }
        same_line((spacing = 12.0f))
        if (button(OPEN_SETTINGS, (text = "Open settings (X-button)"))) {
            SETTINGS_MODAL.pending_open = true
        }

        separator()
        text(STATUS_LINE, (text = "Last confirm answer: {CONFIRM_RESULT}"))
        text("SETTINGS_MODAL.open (closable) = {SETTINGS_MODAL.open}")

        // ---- Modal A: Yes/No confirm ----

        // closable=false → no X-button; Yes/No buttons + ESC close it (ESC is an
        // ImGui built-in — to disable it the app must intercept the key before
        // NewFrame()).
        popup_modal(CONFIRM_MODAL, (text = "Confirm delete",
                                    closable = false,
                                    flags = ImGuiWindowFlags.AlwaysAutoResize)) {
            text("Are you sure you want to delete the selected file?")
            separator()
            if (button(CONFIRM_YES, (text = "Yes"))) {
                CONFIRM_RESULT = "Yes"
                CONFIRM_MODAL.pending_close = true
            }
            same_line((spacing = 12.0f))
            if (button(CONFIRM_NO, (text = "No"))) {
                CONFIRM_RESULT = "No"
                CONFIRM_MODAL.pending_close = true
            }
        }

        // ---- Modal B: closable settings ----
        // closable=true → X-button on the title bar wired to state.open.
        popup_modal(SETTINGS_MODAL, (text = "Settings",
                                     closable = true,
                                     flags = ImGuiWindowFlags.AlwaysAutoResize)) {
            text("Background tasks")
            separator()
            checkbox(S_AUTOSAVE, (text = "Auto-save every 5 minutes"))
            checkbox(S_TELEMETRY, (text = "Send anonymous telemetry"))
            S_MAX_FPS.bounds = (30, 240)
            slider_int(S_MAX_FPS, (text = "Max FPS cap"))
            separator()
            if (button(S_APPLY, (text = "Apply"))) {
                SETTINGS_MODAL.pending_close = true
            }
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
