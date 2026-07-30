options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require daslib/safe_addr

// =============================================================================
// FEATURE: edit_menu_item — MenuItem against a caller-owned bool? pointer.
// SHOWS:   edit_menu_item — the menu-context sibling of edit_checkbox. The
//          menu shows a check mark while *ptr is true and toggles on click.
//          Caller owns persistence; no @live preservation. Useful when an
//          app menu's toggles need to mutate plain app-side bools instead
//          of going through the [edit_widget] state struct.
// DRIVE:   curl -X POST -d '{"name":"imgui_click","args":{"target":"MENU_BAR/FILE_MENU/MI_DARK"}}' \
//              localhost:9090/command
//          (menu must be open first; see the main_menu_bar tutorial for the
//          synth-click pattern that opens menus from a recording driver)
// STANDALONE: daslang.exe modules/dasImgui/examples/features/edit_menu_item.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/edit_menu_item.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/edit_menu_item.das
// =============================================================================

var private g_dark_mode : bool = false
var private g_autosave : bool = true
var private g_show_grid : bool = false

[export]
def init() {
    harness_init("edit_menu_item — boost v2 §3.7", 720, 380)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    main_menu_bar(MENU_BAR) {
        menu(FILE_MENU, (text = "File", enabled = true)) {
            edit_menu_item(safe_addr(g_dark_mode), (id = "MI_DARK",
                                                    text = "Dark mode",
                                                    shortcut = "Ctrl+D"))
            edit_menu_item(safe_addr(g_autosave), (id = "MI_AUTO",
                                                   text = "Auto-save"))
        }
        menu(VIEW_MENU, (text = "View", enabled = true)) {
            edit_menu_item(safe_addr(g_show_grid), (id = "MI_GRID",
                                                    text = "Show grid",
                                                    shortcut = "G"))
        }
    }

    SetNextWindowPos(ImVec2(40.0, 60.0), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(640.0, 280.0), ImGuiCond.Always)
    window(BODY, (text = "Live state", closable = false,
                  flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "Click items in the menu bar — caller-owned bools flip."))
        text(VAL_DARK, (text = "g_dark_mode  = {g_dark_mode}"))
        text(VAL_AUTO, (text = "g_autosave   = {g_autosave}"))
        text(VAL_GRID, (text = "g_show_grid  = {g_show_grid}"))
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
