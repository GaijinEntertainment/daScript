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
// TUTORIAL: group — BeginGroup/EndGroup pure layout grouping.
//
//   group(IDENT) { body }
//
// Treats several widgets as a single layout unit. Two effects:
//
//   1. Same-line stacking — same_line() after the group's closing brace
//      continues from the group's right edge, not the last widget's. Useful
//      for laying out (label-block | value-block | edit-block) rows.
//
//   2. Hit-test the whole group — IsItemHovered() / IsItemActive() called
//      after EndGroup() report against the group's combined bbox. Lets a
//      tooltip attach to "anywhere inside the icon-plus-label cluster".
//
// GroupState is empty — no flags, no observables. Pure structural marker.
// The path push still happens, so a checkbox inside group(LEFT_GRP) is
// reachable at GRP_WIN/LEFT_GRP/L_WIRE.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/group.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/group.das
// =============================================================================

[export]
def init() {
    live_create_window("dasImgui group tutorial", 720, 480)
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
    window(GRP_WIN, (text = "group tutorial", closable = false,
                     flags = ImGuiWindowFlags.None)) {

        text("group(IDENT) takes a block and treats its widgets as one layout unit.")
        separator()

        // ---- A: two side-by-side groups via same_line ----
        text("A) Two groups + same_line: each block stacks vertically; same_line")
        text("   between them keeps the right block's top aligned with the left.")
        group(LEFT_GRP) {
            text("Left block")
            checkbox(L_WIRE, (text = "Wireframe"))
            checkbox(L_GRID, (text = "Show grid"))
            // Narrow the slider so LEFT_GRP leaves enough room for RIGHT_GRP
            // on the same row at FontGlobalScale 1.4 + 680 px content width.
            SetNextItemWidth(160.0f)
            slider_int(L_FRAMES, (text = "frame limit"))
        }
        same_line((spacing = 24.0f))
        group(RIGHT_GRP) {
            text("Right block")
            checkbox(R_VSYNC, (text = "VSync"))
            checkbox(R_FULL, (text = "Fullscreen"))
            small_button(R_APPLY, (text = "Apply"))
        }
        separator()

        // ---- B: group as a tooltip hit-test surface ----
        text("B) Hit-test a whole cluster - hover anywhere in the row below.")
        group(HOVER_GRP) {
            text("[i] Info")
            same_line()
            text("hover me anywhere in this row")
            same_line()
            small_button(HOVER_BTN, (text = "Action"))
        }
        if (IsItemHovered(ImGuiHoveredFlags.None)) {
            tooltip(GRP_TIP) {
                text("BeginTooltip while the whole group is hovered.")
                text("IsItemHovered() after EndGroup() reports against the combined bbox.")
            }
        }
        separator()

        // ---- C: group used as a "row" with right-aligned status ----
        text("C) The (label | value | button) row pattern.")
        group(ROW_GRP) {
            text("Speed:")
            same_line()
            slider_float(ROW_SPEED, (text = "##speed"))
            same_line()
            small_button(ROW_RESET, (text = "Reset"))
        }
        text("ROW_SPEED.value = {ROW_SPEED.value}")
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
