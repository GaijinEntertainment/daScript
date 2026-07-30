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
// TUTORIAL: color_button_hover — hover-target variant of color_button.
//
//   color_button(IDENT, (col = ...))        — click trigger (ClickState).
//   color_button_hover(IDENT, (col = ...))  — hover only (EmptyMarkerState);
//                                              returns true while ImGui
//                                              considers the swatch hovered.
//
// Use the hover variant when the swatch is the surface on which a
// hover-driven side-effect is mounted (e.g. "while hovering this swatch,
// override io.WantCaptureMouse"); the click form would force the caller to
// distinguish click vs hover at every site.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/color_button_hover.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/color_button_hover.das
// =============================================================================

var private STATE_MESSAGE = "Hover any swatch."

[export]
def init() {
    live_create_window("dasImgui color_button_hover tutorial", 640, 380)
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
    SetNextWindowSize(ImVec2(560.0f, 300.0f), ImGuiCond.Always)
    window(CBH_WIN, (text = "color_button_hover", closable = false,
                     flags = ImGuiWindowFlags.None)) {
        text("color_button_hover returns true while ImGui considers the")
        text("just-drawn swatch hovered. No click state, no toggle.")
        separator()

        let swatch_flags = (ImGuiColorEditFlags.NoTooltip | ImGuiColorEditFlags.NoDragDrop)
        let red = color_button_hover(CBH_RED,
            (desc_id = "##red", col = float4(0.85f, 0.20f, 0.20f, 1.0f),
             size = float2(96.0f, 64.0f), flags = swatch_flags))
        same_line(CBH_SL_1)
        let green = color_button_hover(CBH_GREEN,
            (desc_id = "##green", col = float4(0.20f, 0.85f, 0.30f, 1.0f),
             size = float2(96.0f, 64.0f), flags = swatch_flags))
        same_line(CBH_SL_2)
        let blue = color_button_hover(CBH_BLUE,
            (desc_id = "##blue", col = float4(0.20f, 0.40f, 0.85f, 1.0f),
             size = float2(96.0f, 64.0f), flags = swatch_flags))

        if (red) {
            STATE_MESSAGE = "Hovering: red"
        } elif (green) {
            STATE_MESSAGE = "Hovering: green"
        } elif (blue) {
            STATE_MESSAGE = "Hovering: blue"
        }

        text(CBH_STATUS, (text = STATE_MESSAGE))
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
