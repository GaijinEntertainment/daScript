options gen2
options _comment_hygiene = true

require daslib/safe_addr
require imgui/imgui_harness

// =============================================================================
// FEATURE: edit_tab_item — closable TabItem on a caller-owned visibility bool.
// SHOWS:   `edit_tab_item(safe_addr(open_bool), (id="..", text="..", flags=..))`
//          binds to an external bool*. The X close button writes
//          `*p_open = false`; when false, the entire tab (chrome + body) is
//          skipped. External siblings (the edit_checkbox row at the top)
//          drive the SAME bools — bidir-bound shared state.
// DRIVE:   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"MAIN_WIN/BAR/TAB_A","value":false}}' \
//              localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/edit_tab_item.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/edit_tab_item.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/edit_tab_item.das
// =============================================================================

var private g_tab_a_open : bool = true
var private g_tab_b_open : bool = true

[export]
def init() {
    harness_init("edit_tab_item — closable tab bound to bool", 640, 360)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(600.0, 320.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "edit_tab_item", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "Each tab's X button writes the bound bool. The checkboxes drive the same bools."))

        edit_checkbox(safe_addr(g_tab_a_open), (id = "CHECK_A", text = "show A"))
        same_line()
        edit_checkbox(safe_addr(g_tab_b_open), (id = "CHECK_B", text = "show B"))
        separator()

        tab_bar(BAR, (text = "MyBar",
                      flags = ImGuiTabBarFlags.FittingPolicyShrink)) {
            edit_tab_item(safe_addr(g_tab_a_open),
                          (id = "TAB_A", text = "alpha",
                           flags = ImGuiTabItemFlags.None)) {
                text(BODY_A, (text = "alpha tab body — close me with the X."))
            }
            edit_tab_item(safe_addr(g_tab_b_open),
                          (id = "TAB_B", text = "beta",
                           flags = ImGuiTabItemFlags.None)) {
                text(BODY_B, (text = "beta tab body — close me with the X."))
            }
        }
        text(STATUS, (text = "a={g_tab_a_open} b={g_tab_b_open}"))
    }

    harness_end_frame()
}

[export]
def shutdown() {
    harness_shutdown()
}

[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
