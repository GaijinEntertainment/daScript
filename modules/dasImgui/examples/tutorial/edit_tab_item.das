options gen2
options _comment_hygiene = true

require daslib/safe_addr
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
// TUTORIAL: edit_tab_item — caller-owned visibility bool for a tab.
//
//   edit_tab_item(safe_addr(open_bool), (id = "..", text = "..",        // bidir
//                                         flags = ...)) {                //   bind
//       <tab body>
//   }
//
// External-pointer sibling of `tab_item` (which owns visibility on its
// TabItemState). When `*p_open == false` the tab is skipped entirely
// (no header chrome, no body). The X close button on the header writes
// `*p_open = false` — bidir-bound.
//
// Use this when a SECOND UI surface (a checkbox row, a menu toggle,
// imgui_force_set on the external bool) needs to drive the same visibility
// flag the X writes. `tab_item`'s internal `state.open` isn't exposed
// for write outside the widget, so a shared external bool is the
// canonical pattern.
//
// All edit_* rails require an `id = "<literal>"` argument — that's the
// snapshot path. The first positional arg is the bool? pointer (passed
// via safe_addr for own globals, unsafe(addr(...)) for array elements
// or fields).
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/edit_tab_item.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/edit_tab_item.das
//
// DRIVE (when running live):
//   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"ET_WIN/ET_BAR/ET_TAB_A","value":false}}' \
//        localhost:9090/command
// =============================================================================

var private g_tab_a_open : bool = true
var private g_tab_b_open : bool = true
var private g_tab_c_open : bool = true

[export]
def init() {
    live_create_window("dasImgui edit_tab_item tutorial", 760, 560)
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
    SetNextWindowSize(ImVec2(720.0f, 520.0f), ImGuiCond.Always)
    window(ET_WIN, (text = "edit_tab_item tutorial", closable = false,
                    flags = ImGuiWindowFlags.None)) {

        text("Caller-owned visibility bool - the X button on each tab writes it.")
        text(ET_HINT, (text = "Checkboxes below also drive the SAME bools. Bidir-bound on g_tab_*_open."))
        separator()

        // ---- External controls — second UI surface driving the same bools ----
        text("External controls (mirror the X-button state):")
        edit_checkbox(safe_addr(g_tab_a_open), (id = "ET_CHECK_A", text = "show A"))
        same_line(ET_SL1)
        edit_checkbox(safe_addr(g_tab_b_open), (id = "ET_CHECK_B", text = "show B"))
        same_line(ET_SL2)
        edit_checkbox(safe_addr(g_tab_c_open), (id = "ET_CHECK_C", text = "show C"))
        spacing()
        separator()

        // ---- The tab bar — three edit_tab_item instances ----
        tab_bar(ET_BAR, (text = "MyBar",
                          flags = ImGuiTabBarFlags.FittingPolicyShrink)) {
            edit_tab_item(safe_addr(g_tab_a_open),
                          (id = "ET_TAB_A", text = "alpha",
                           flags = ImGuiTabItemFlags.None)) {
                text(ET_BODY_A, (text = "alpha tab body - close me with the X."))
            }
            edit_tab_item(safe_addr(g_tab_b_open),
                          (id = "ET_TAB_B", text = "beta",
                           flags = ImGuiTabItemFlags.None)) {
                text(ET_BODY_B, (text = "beta tab body - close me with the X."))
            }
            edit_tab_item(safe_addr(g_tab_c_open),
                          (id = "ET_TAB_C", text = "gamma",
                           flags = ImGuiTabItemFlags.None)) {
                text(ET_BODY_C, (text = "gamma tab body - close me with the X."))
            }
        }
        spacing()
        text("g_tab_a_open = {g_tab_a_open}, g_tab_b_open = {g_tab_b_open}, g_tab_c_open = {g_tab_c_open}")
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
