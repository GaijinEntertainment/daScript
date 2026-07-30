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
// TUTORIAL: selectable_hover — hover-target variant of selectable.
//
//   selectable(IDENT, (text = "Row"))        — toggleable (ToggleState).
//   selectable_hover(IDENT, (text = "Row"))  — hover only (EmptyMarkerState);
//                                               selected pinned to false;
//                                               returns true while hovered.
//
// Use the hover variant when the selectable row is the surface for a
// per-frame hover side-effect (e.g. "while hovering this cursor sample,
// set ImGui's mouse cursor to that style") and the toggle semantics of
// regular selectable would be incorrect.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/selectable_hover.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/selectable_hover.das
// =============================================================================

var private STATE_MESSAGE = "Hover any row."
var private SH_ROW : table<int; EmptyMarkerState>

let private ROWS = fixed_array("Arrow", "TextInput", "ResizeAll", "Hand", "NotAllowed")

[export]
def init() {
    live_create_window("dasImgui selectable_hover tutorial", 600, 380)
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

    SetNextWindowPos(ImVec2(40.0f, 40.0f), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(520.0f, 300.0f), ImGuiCond.Always)
    window(SH_WIN, (text = "selectable_hover", closable = false,
                    flags = ImGuiWindowFlags.None)) {
        text("Hover a row - STATE_MESSAGE follows the highlighted item.")
        separator()

        for (i in range(length(ROWS))) {
            let hovered = selectable_hover(SH_ROW[i], (text = "Row {i}: {ROWS[i]}"))
            if (hovered) {
                STATE_MESSAGE = "Hovering: {ROWS[i]}"
            }
        }

        separator()
        text(SH_STATUS, (text = STATE_MESSAGE))
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
