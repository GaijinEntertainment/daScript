options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record main_menu_bar.apng against examples/tutorial/main_menu_bar.das.
//! Each menu is driven the human way and self-verified, so the recording doubles as a test.
//! menu_pick_voice opens the bar header then drops STRAIGHT DOWN the menu's column onto the
//! item -- a diagonal click on the item's center would clip a sibling header and ImGui would
//! hover-switch the open menu mid-travel. Each pick is checked against its widget's persistent
//! payload (a menu_item toggles its value, a menu_label counts clicks):
//!   File > Dark mode  -> MI_DARKMODE.value == true        (menu_item = toggle)
//!   File > Auto-save  -> MI_AUTOSAVE.value == true        (independent toggle)
//!   Edit > Undo       -> EDIT_UNDO.click_count == 1       (menu_label = action)

def header_below(var snap : JsonValue?; ident : string) : tuple<float; float> {
    var b = snap?["globals"]?[ident]?["bbox"]
    let cx = ((b?["x"] ?? 0.0f) + (b?["z"] ?? 0.0f)) * 0.5f
    let below = (b?["w"] ?? 0.0f) + 10.0f
    return (cx, below)
}

[export]
def main {
    with_recording_app("examples/tutorial/main_menu_bar.das",
                       "main_menu_bar.apng", 70) $(app) {
        let T_FILE = "MAIN_BAR/FILE_MENU"
        let T_EDIT = "MAIN_BAR/EDIT_MENU"
        let T_DARK = "MAIN_BAR/FILE_MENU/MI_DARKMODE"
        let T_AUTO = "MAIN_BAR/FILE_MENU/MI_AUTOSAVE"
        let T_UNDO = "MAIN_BAR/EDIT_MENU/EDIT_UNDO"

        var snap = wait_for_render(app, T_FILE, 10.0f)
        if (snap == null) {
            panic("{T_FILE} never rendered - wrong app running?")
        }

        // ---- Stage 1: File > Dark mode (menu_item toggle) ----
        move_to(app, header_below(snap, T_FILE), 700)
        hold_content(app, SETTLE_MS)
        let d1 = say_begin(app, "File > Dark mode", T_FILE,
            [voice = "A menu bar groups commands under headers along the top. Open File, click Dark mode, and the toggle switches on."])
        menu_pick_voice(app, d1, T_FILE, T_DARK)
        record_check_value(app, T_DARK, "value", true, 120)

        // ---- Stage 2: File > Auto-save (independent toggle) ----
        snap = snapshot(app)
        move_to(app, header_below(snap, T_FILE), 500)
        hold_content(app, SETTLE_MS)
        let d2 = say_begin(app, "each item keeps its own state", T_FILE,
            [voice = "Every menu item is its own toggle. Open File again and turn on Auto-save; Dark mode stays on, the two are independent."])
        menu_pick_voice(app, d2, T_FILE, T_AUTO)
        record_check_value(app, T_AUTO, "value", true, 120)

        // ---- Stage 3: Edit > Undo (menu_label action) ----
        snap = snapshot(app)
        move_to(app, header_below(snap, T_EDIT), 600)
        hold_content(app, SETTLE_MS)
        let d3 = say_begin(app, "labels count clicks, not toggles", T_EDIT,
            [voice = "Not every entry is a toggle. Undo is a menu label, an action. Click it and it just counts the click, with no on or off state."])
        menu_pick_voice(app, d3, T_EDIT, T_UNDO)
        record_check_value(app, T_UNDO, "click_count", 1, 120)
    }
}
