options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/fio
require daslib/json public
require daslib/json_boost public

//! Fixture-driven feature coverage for `imgui_file_dialog`, driven through
//! `imgui_playwright`. Builds a temp directory with known files (alpha.png /
//! beta.jpg / gamma.txt + a subdir), drives the dialog there via the editable
//! path bar, then verifies the multi-extension filter (Images shows png+jpg,
//! hides txt), the Modified (date) column, and New Folder creation. Round-trips
//! are kept lean (≈11) to stay under the Windows libhv 16-POST-per-subprocess
//! ceiling. Cleans up the fixture. Headless on ubuntu/macOS like the rest.

let FIXTURE_NAME = "das_fd_dialog_test"

def private make_fixture() : string {
    var err = ""
    let root = normalize(path_join(temp_directory(err), FIXTURE_NAME))
    rmdir_rec_result(root)                          // clear any leftover from a prior run
    mkdir_rec(root)
    mkdir_rec(path_join(root, "subdir"))
    fwrite(path_join(root, "alpha.png"), "x")
    fwrite(path_join(root, "beta.jpg"), "x")
    fwrite(path_join(root, "gamma.txt"), "x")
    return root
}

// Type-cell values of the rows currently RENDERED (not merely registered), folders
// excluded — i.e. the set of file extensions the active filter is letting through.
def private visible_file_types(var snap : JsonValue?) : array<string> {
    var out : array<string>
    for (i in range(64)) {
        let cell = "FILEDLG/FILE_TABLE/TYPE_CELL[{i}]"
        if (!widget_rendered(snap, cell)) {
            continue
        }
        let v = widget_payload_field(snap, cell, "value") ?? ""
        if (!empty(v) && v != "Folder") {
            out |> push(v)
        }
    }
    return <- out
}

def private has_type(types : array<string>; t : string) : bool {
    for (x in types) {
        if (x == t) {
            return true
        }
    }
    return false
}

def private folder_count(var snap : JsonValue?) : int {
    var n = 0
    for (i in range(64)) {
        let cell = "FILEDLG/FILE_TABLE/TYPE_CELL[{i}]"
        if (widget_rendered(snap, cell) && (widget_payload_field(snap, cell, "value") ?? "") == "Folder") {
            n++
        }
    }
    return n
}

def private any_date_shown(var snap : JsonValue?) : bool {
    for (i in range(64)) {
        let cell = "FILEDLG/FILE_TABLE/DATE_CELL[{i}]"
        if (widget_rendered(snap, cell) && !empty(widget_payload_field(snap, cell, "value") ?? "")) {
            return true
        }
    }
    return false
}

[test]
def test_file_dialog_features(t : T?) {
    let fixture = make_fixture()
    with_imgui_app("modules/dasImgui/examples/tutorial/file_dialog.das") $(d) {
        var boot = wait_for_widget(d, "FDLG_WIN/BTN_OPEN", 15.0f)
        t |> success(boot != null, "demo + Open trigger render")
        if (boot != null) {
            click(d, "FDLG_WIN/BTN_OPEN")
            t |> success(wait_for_widget(d, "FILEDLG/PATH_FIELD", 10.0f) != null, "open dialog renders the path bar")

            // Jump to the fixture via the editable path bar, then switch to the
            // Images filter (index 1: daslang|Images|All files).
            force_set_value(d, "FILEDLG/PATH_FIELD", JV(fixture))
            click(d, "FILEDLG/GO")
            force_set_value(d, "FILEDLG/FILTER_COMBO", JV(1))

            // One wait covers: nav landed (png visible => we're in the fixture),
            // multi-extension filter (jpg visible, txt hidden), and dates rendering.
            var landed = wait_until_sec(d, 6.0f) $(var s) {
                var types <- visible_file_types(s)
                let ok = has_type(types, ".png") && has_type(types, ".jpg") && !has_type(types, ".txt")
                delete types
                return ok
            }
            t |> success(landed != null, "path bar navigates + Images filter shows png/jpg, hides txt")
            if (landed != null) {
                t |> success(any_date_shown(landed), "Modified column renders dates (format_time)")
            }

            // New Folder: the fixture has one folder (subdir); creating another makes two.
            click(d, "FILEDLG/NEWFOLDER")
            t |> success(wait_for_widget(d, "FILEDLG/NEWDIR/NEWDIR_NAME", 5.0f) != null,
                "New Folder opens the NEWDIR sub-modal")
            force_set_value(d, "FILEDLG/NEWDIR/NEWDIR_NAME", JV("fd_newdir"))
            click(d, "FILEDLG/NEWDIR/NEWDIR_OK")
            let created = wait_until_sec(d, 5.0f) $(var s) {
                return folder_count(s) >= 2
            } != null
            t |> success(created, "New Folder creates a directory that appears in the list")
        }
    }
    rmdir_rec_result(fixture)
}
