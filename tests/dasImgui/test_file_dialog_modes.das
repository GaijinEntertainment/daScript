options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require imgui
require daslib/json public
require daslib/json_boost public
require strings

//! Mode + keyboard coverage for `imgui_file_dialog`, driven through
//! `imgui_playwright` against the tutorial demo — no filesystem fixture needed:
//!   - Save mode pre-fills the name field from `default_name`;
//!   - Enter (keyboard) confirms the save;
//!   - pick_dir mode hides the name field and "Select Folder" confirms a directory.
//! Headless on ubuntu/macOS like the rest of the suite.

def private wait_result(app : ImguiApp; needle : string) : bool {
    return wait_until_sec(app, 5.0f) $(var snap) {
        return find(widget_payload_field(snap, "FDLG_WIN/RESULT", "value") ?? "", needle) >= 0
    } != null
}

[test]
def test_file_dialog_modes(t : T?) {
    with_imgui_app("modules/dasImgui/examples/tutorial/file_dialog.das") $(d) {
        var boot = wait_for_widget(d, "FDLG_WIN/BTN_SAVE", 15.0f)
        t |> success(boot != null, "demo + Save As trigger render")
        if (boot == null) {
            return
        }
        t |> success(widget_exists(boot, "FDLG_WIN/BTN_PICKDIR"), "Pick Folder trigger present")

        // --- save mode: default_name pre-fills the name field ---
        click(d, "FDLG_WIN/BTN_SAVE")
        let prefilled = wait_until_sec(d, 10.0f) $(var s) {
            return (widget_payload_field(s, "FILEDLG/NAME_FIELD", "value") ?? "") == "untitled.das"
        } != null
        t |> success(prefilled, "save dialog opens with default_name 'untitled.das' pre-filled")

        // --- keyboard: Enter confirms the save ---
        force_set_value(d, "FILEDLG/NAME_FIELD", JV("kbd_enter_test"))
        key_tap(d, int(ImGuiKey.Enter))
        t |> success(wait_result(d, "kbd_enter_test"), "Enter confirms the save (keyboard)")

        // --- pick_dir mode: folders-only, no name field, Select Folder confirms a dir ---

        // Use wait_for_VISIBLE, not wait_for_widget: FILE_TABLE/BTN_OK were registered by the earlier
        // save dialog, so wait_for_widget returns on the stale entry and the BTN_OK click then drops
        // silently into a not-yet-laid-out target (the macOS-CI failure; ubuntu's cadence masked it).
        click(d, "FDLG_WIN/BTN_PICKDIR")
        var pd = wait_for_visible(d, "FILEDLG/FILE_TABLE", 10.0f)
        t |> success(pd != null, "pick-folder dialog renders the table")
        if (pd != null) {
            t |> success(!widget_rendered(pd, "FILEDLG/NAME_FIELD"), "pick_dir hides the name field")
            t |> success(widget_exists(pd, "FILEDLG/BTN_OK"), "Select Folder button present")
        }
        // gate the click on its target being painted this frame, else the host-side resolve misses
        t |> success(wait_for_visible(d, "FILEDLG/BTN_OK", 5.0f) != null, "Select Folder button painted")
        click(d, "FILEDLG/BTN_OK")
        let confirmed = wait_until_sec(d, 5.0f) $(var s) {
            return !widget_rendered(s, "FILEDLG/FILE_TABLE")
        } != null
        t |> success(confirmed, "Select Folder confirms the current directory and closes the dialog")
    }
}
