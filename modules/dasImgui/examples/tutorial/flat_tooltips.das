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
// TUTORIAL: flat_tooltips — boost v2 flat-form tooltip widgets.
//
// The container form `tooltip(IDENT) { ... }` (already in v1) wraps a full
// scope. For one-liner tooltips with no inner layout, use the flat forms:
//
//   set_tooltip(IDENT, (text = ...))         — always shows; rare
//   set_item_tooltip(IDENT, (text = ...))    — shows when previous item hovered
//
// set_item_tooltip is the canonical replacement for the imgui_demo HelpMarker
// idiom (call after the widget you want to annotate).
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/flat_tooltips.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/flat_tooltips.das
// =============================================================================

[export]
def init() {
    live_create_window("dasImgui flat tooltips", 720, 420)
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

    SetNextWindowPos(ImVec2(60.0, 60.0), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(600.0, 300.0), ImGuiCond.Always)
    window(TOOLTIP_WIN, (text = "Flat tooltips", closable = false,
                         flags = ImGuiWindowFlags.None)) {
        text("Hover the buttons - tooltips fire on item hover.")

        // set_item_tooltip — fires on the most-recently-submitted item.
        if (button(SAVE_BTN, (text = "Save"))) {
            print("save\n")
        }
        set_item_tooltip(SAVE_TIP, (text = "Persists current state to disk."))

        spacing(SP_BTNS)

        if (button(LOAD_BTN, (text = "Load"))) {
            print("load\n")
        }
        set_item_tooltip(LOAD_TIP, (text = "Restores the last saved state."))

        spacing(SP_HELP)

        // Classic "(?)" disabled marker + item_tooltip — replaces the old
        // HelpMarker / demo_help_marker helpers in v1 imgui_demo.
        text("Verbose option (with help marker):")
        same_line(SL_HINT)
        text_disabled(HELP_GLYPH, (text = "(?)"))
        set_item_tooltip(HELP_TIP, (text = "The imgui_demo HelpMarker idiom, in two lines."))
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
