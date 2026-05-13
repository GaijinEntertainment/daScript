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
require imgui/imgui_containers_builtin
require imgui/imgui_style_builtin

// =============================================================================
// FEATURE: style_override — §4.7 `with_style((key, val), ...)` { ... }.
// SHOWS:   - Color (ImGuiCol) and var (ImGuiStyleVar) tuples mixed in one call.
//          - Nested `with_style` blocks (inner additions stack on top of outer).
//          - Plain widgets after a `with_style` block see baseline style again
//            (proves pop count matches push count — no style leak).
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command
//          curl -X POST -d '{"name":"imgui_click","args":{"target":"STYLED_BTN"}}' \
//               localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/style_override.das
// LIVE:       daslang-live modules/dasImgui/examples/features/style_override.das
// =============================================================================

[export]
def init() {
    live_create_window("§4.7 — with_style", 640, 480)
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
    NewFrame()

    SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond.FirstUseEver)
    SetNextWindowSize(ImVec2(560.0f, 400.0f), ImGuiCond.FirstUseEver)
    window(STYLE_WIN, (text = "with_style demo", closable = false,
                       flags = ImGuiWindowFlags.None)) {
        Text("Mixed color + var, single block:")
        with_style((ImGuiCol.Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f)),
                   (ImGuiStyleVar.FrameRounding, 8.0f)) {
            button(STYLED_BTN, (text = "Red rounded"))
        }

        Separator()
        Text("Nested with_style — inner stacks on outer:")
        with_style((ImGuiCol.Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f))) {
            with_style((ImGuiCol.Button, ImVec4(0.2f, 0.4f, 0.8f, 1.0f))) {
                button(NESTED_STYLED_BTN, (text = "Yellow text + blue button"))
            }
        }

        Separator()
        Text("After with_style — baseline style restored:")
        // If push/pop counts were off, this button would render with a stale
        // color/rounding. ImGui's style stack auto-asserts unbalanced pops
        // at EndFrame so the live process would crash; absent that, visual
        // sanity says "looks like a normal button".
        button(UNSTYLED_BTN, (text = "Plain button"))
    }

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
