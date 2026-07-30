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
require imgui/imgui_style_builtin
require imgui/imgui_visual_aids

// =============================================================================
// TUTORIAL: with_style — scoped color/metric overrides for a sub-tree.
//
// ImGui's style stack is one of its main customization knobs: PushStyleColor
// changes a color until the matching PopStyleColor; PushStyleVar changes a
// metric (rounding, padding, spacing, ...) until the matching PopStyleVar.
// Hand-balancing the pushes and pops is bug-prone — miss a pop and the
// next frame asserts on EndFrame. The `with_style((key, val), ...) { ... }`
// boost macro brackets the push/pop pair around a block:
//
//   with_style((ImGuiCol.Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f)),
//              (ImGuiStyleVar.FrameRounding, 8.0f)) {
//       button(STYLED_BTN, (text = "Red rounded"))
//   }
//
// Each `(key, value)` tuple is dispatched at compile time by overload
// resolution — ImGuiCol keys go to PushStyleColor, ImGuiStyleVar keys to
// PushStyleVar. After the block, the matching pops fire in one bulk call.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/with_style.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/with_style.das
//
// DRIVE (when running live):
//   curl -X POST -d '{"name":"imgui_snapshot"}'                                                       localhost:9090/command
//   curl -X POST -d '{"name":"imgui_click","args":{"target":"STYLE_WIN/STYLED_BTN"}}'                  localhost:9090/command
//   curl -X POST -d '{"name":"imgui_click","args":{"target":"STYLE_WIN/NESTED_STYLED_BTN"}}'           localhost:9090/command
// =============================================================================

[export]
def init() {
    live_create_window("dasImgui with_style tutorial", 720, 520)
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

    SetNextWindowPos(ImVec2(30.0f, 30.0f), ImGuiCond.FirstUseEver)
    SetNextWindowSize(ImVec2(640.0f, 440.0f), ImGuiCond.FirstUseEver)
    window(STYLE_WIN, (text = "with_style", closable = false,
                       flags = ImGuiWindowFlags.None)) {
        text("Mixed colors + var, single block:")
        // A full button restyle sets all three interaction states - base,
        // hovered, active - so the color holds while the pointer is over it.
        with_style((ImGuiCol.Button, ImVec4(0.85f, 0.20f, 0.20f, 1.0f)),
                   (ImGuiCol.ButtonHovered, ImVec4(0.95f, 0.30f, 0.30f, 1.0f)),
                   (ImGuiCol.ButtonActive, ImVec4(0.70f, 0.12f, 0.12f, 1.0f)),
                   (ImGuiStyleVar.FrameRounding, 8.0f)) {
            button(STYLED_BTN, (text = "Red rounded"))
        }

        separator(WS_SEP_1)
        text("Nested with_style - inner stacks on outer:")
        with_style((ImGuiCol.Text, ImVec4(1.0f, 0.95f, 0.30f, 1.0f))) {
            text("Yellow text inherited from outer scope.")
            with_style((ImGuiCol.Button, ImVec4(0.20f, 0.45f, 0.85f, 1.0f)),
                       (ImGuiCol.ButtonHovered, ImVec4(0.30f, 0.55f, 0.95f, 1.0f)),
                       (ImGuiCol.ButtonActive, ImVec4(0.12f, 0.35f, 0.70f, 1.0f)),
                       (ImGuiStyleVar.FrameRounding, 4.0f)) {
                // Both the outer yellow text and the inner blue/rounded
                // button are in scope.
                button(NESTED_STYLED_BTN, (text = "Blue button + yellow text"))
            }
        }

        separator(WS_SEP_2)
        text("After with_style - baseline style restored.")
        // If push/pop counts were off, this button would render with stale
        // color/rounding, and ImGui asserts on a stack imbalance at EndFrame
        // (crash). The boost macro guarantees the bulk pop_style_n always fires.
        button(UNSTYLED_BTN, (text = "Plain button"))
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
