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
require imgui/imgui_scope_builtin
require imgui/imgui_visual_aids

// =============================================================================
// TUTORIAL: log_capture — capture rendered widget text with with_log.
//
// 1.92 renamed the log-target enum: ImGuiLogType -> ImGuiLogFlags, and the
// values gained an Output prefix (TTY -> OutputTTY, Clipboard -> OutputClipboard,
// File -> OutputFile, Buffer -> OutputBuffer). The boost with_log wrapper now
// takes an ImGuiLogFlags:
//
//   with_log(ImGuiLogFlags.OutputClipboard, -1) {
//       text("captured")            // every widget's rendered text is logged
//   }
//
// LogBegin asserts logging is not already active, so with_log must NOT nest.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/log_capture.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/log_capture.das
// =============================================================================

var private g_capture_count = 0
var private g_last_clip = ""
var private g_capture_requested = false

[export]
def init() {
    live_create_window("dasImgui log_capture tutorial", 820, 600)
    live_imgui_init(live_window)
    let io & = unsafe(GetIO())
    GetStyle().FontScaleMain = 1.3
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplGlfw_NewFrame()
    apply_synth_io_override()
    NewFrame()

    SetNextWindowPos(ImVec2(40.0f, 40.0f), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(740.0f, 520.0f), ImGuiCond.Always)
    window(LC_WIN, (text = "log_capture", closable = false,
                    flags = ImGuiWindowFlags.None)) {
        text("with_log(ImGuiLogFlags.OutputClipboard, -1) captures rendered widget")
        text("text to the clipboard. Click to capture, then read it back below.")
        separator()

        if (button(LC_BTN, (text = "Capture report to clipboard"))) {
            g_capture_requested = true
        }
        separator()

        // The report renders every frame. On a capture frame, wrap it in a
        // clipboard log so each widget's text lands in the clipboard, then read
        // it straight back with GetClipboardText.
        let capture_now = g_capture_requested
        g_capture_requested = false
        if (capture_now) {
            g_capture_count++
            with_log(ImGuiLogFlags.OutputClipboard, -1) {
                text("=== Daslang Widget Report ===")
                text("Section: Fruits")
                text("  Apple, Banana, Cherry")
                LogRenderedText("(injected via LogRenderedText - capture #{g_capture_count})")
            }
            g_last_clip := GetClipboardText()
        } else {
            text("=== Daslang Widget Report ===")
            text("Section: Fruits")
            text("  Apple, Banana, Cherry")
        }

        separator()
        text("Captures so far: {g_capture_count}. The injected line below appears ONLY")
        text("in the capture, never in the on-screen report above:")
        child(LC_VIEW, (text = "clip", size = float2(660.0f, 200.0f),
                        child_flags = ImGuiChildFlags.Borders,
                        window_flags = ImGuiWindowFlags.None)) {
            text(LC_TEXT, (text = g_last_clip))
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
