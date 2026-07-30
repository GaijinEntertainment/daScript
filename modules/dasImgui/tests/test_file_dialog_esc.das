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

//! Integration smoke: ESC cancels the file dialog. Opens the dialog (no field focused),
//! presses Escape, and asserts the demo's RESULT echoes "cancelled" — the keyboard
//! equivalent of the Cancel button — and that the dialog closes. Headless-safe (synthetic
//! key over the live API), like the rest of the file_dialog suite.

let FILE_DIALOG_FEATURE = "modules/dasImgui/examples/tutorial/file_dialog.das"

[test]
def test_file_dialog_esc(t : T?) {
    with_imgui_app(FILE_DIALOG_FEATURE) $(d) {
        var snap = wait_for_widget(d, "FDLG_WIN/BTN_OPEN", 15.0f)
        t |> success(snap != null, "demo window + Open trigger render")
        if (snap == null) {
            return
        }

        click(d, "FDLG_WIN/BTN_OPEN")
        t |> success(wait_for_widget(d, "FILEDLG/FILE_TABLE", 10.0f) != null,
                     "open dialog renders the file table")

        // ESC cancels — no name/path field is focused on open, so the modal must close itself.
        key_tap(d, int(ImGuiKey.Escape))
        t |> success(wait_until_sec(d, 5.0f) $(var s) {
                return find(widget_payload_field(s, "FDLG_WIN/RESULT", "value") ?? "", "cancelled") >= 0
            } != null,
            "ESC returns the cancelled result")
        t |> success(wait_until_sec(d, 5.0f) $(var s) {
                return !widget_rendered(s, "FILEDLG/FILE_TABLE")
            } != null,
            "ESC closes the dialog (table stops rendering)")
    }
}
