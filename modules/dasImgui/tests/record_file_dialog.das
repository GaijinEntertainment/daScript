options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record ``file_dialog.apng`` against ``examples/tutorial/file_dialog.das``.
//! Four voiced, self-verifying beats showing the native New / Open / Save / Pick
//! dialog in Save mode (so it never depends on which files happen to exist on the
//! recording host):
//!   1. intro       - click "Save As..." to open the dialog.
//!   2. default name - the name field is pre-filled from ``default_name`` (verified
//!                     against ``untitled.das``); narrate the path bar + the
//!                     sortable Name / Size / Type / Modified columns.
//!   3. filter      - pick the multi-extension Images filter (combo_pick asserts
//!                     ``FILTER_COMBO.value == 1``).
//!   4. New Folder  - click New Folder; the NEWDIR sub-modal opens.

def widget_bbox(var snap : JsonValue?; ident : string) : float4 {
    var b = snap?["globals"]?[ident]?["bbox"]
    return float4(b?["x"] ?? 0.0f, b?["y"] ?? 0.0f, b?["z"] ?? 0.0f, b?["w"] ?? 0.0f)
}

def widget_center(var snap : JsonValue?; ident : string) : tuple<float; float> {
    let b = widget_bbox(snap, ident)
    return ((b.x + b.z) * 0.5f, (b.y + b.w) * 0.5f)
}

[export]
def main {
    with_recording_app("examples/tutorial/file_dialog.das",
                       "file_dialog.apng", 60) $(app) {
        let T_SAVE = "FDLG_WIN/BTN_SAVE"
        var snap = wait_for_widget(app, T_SAVE, 10.0f)
        if (snap == null) {
            panic("{T_SAVE} never registered - wrong app running?")
        }
        move_to(app, widget_center(snap, T_SAVE), 600)
        hold_content(app, SETTLE_MS)

        // ---- Beat 1: intro - open the Save dialog ----
        hold_through_voice(app, say_begin(app, "a native file dialog", T_SAVE,
            [voice = "This is a native file dialog, built entirely from daslang widgets - open a file, save one, make a new one, or pick a folder. Let's save."]),
            [T_SAVE])

        let dlg = wait_for_widget(app, "FILEDLG/FILE_TABLE", 8.0f)
        if (dlg == null) {
            panic("save dialog never opened")
        }

        // ---- Beat 2: default name + the path bar and sortable columns ----
        say(app, "a suggested name, ready to edit", "FILEDLG/NAME_FIELD",
            [voice = "It opens with a suggested name already filled in. Across the top, a clickable breadcrumb and an editable path bar - type any path, or another drive, and press Enter to jump there. The list sorts by name, size, type, or last modified."])
        record_check_value(app, "FILEDLG/NAME_FIELD", "value", "untitled.das", 180)

        // ---- Beat 3: the multi-extension filter ----
        combo_pick_voice(app, say_begin(app, "one filter, many extensions", "FILEDLG/FILTER_COMBO",
            [voice = "A single filter can match several extensions at once. This Images filter takes png, jpeg, and jpg together, hiding everything else."]),
            "FILEDLG/FILTER_COMBO", 1)

        // ---- Beat 4: New Folder ----
        move_to(app, widget_center(snapshot(app), "FILEDLG/NEWFOLDER"), 700)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "make a folder, right here", "FILEDLG/NEWFOLDER",
            [voice = "And if you need a new folder, the New Folder button makes one without ever leaving the dialog."]),
            ["FILEDLG/NEWFOLDER"])
    }
}
