options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record ``data_table.apng`` against ``examples/tutorial/data_table.das``.
//! Four voiced, self-verifying beats that drive ImGui's sortable-table API by
//! clicking the REAL column headers (the gesture the tutorial teaches, NOT a
//! programmatic re-sort) and asserting the row order actually changed:
//!   1. intro       - the grid boots sorted by Name ascending (DefaultSort).
//!   2. Name click  - flips ascending -> descending; DT_NAME[0] alpha -> zeta.
//!   3. Type click  - single-column re-sort replaces the key; DT_NAME[0] -> delta.
//!   4. Shift+Value - SortMulti adds a secondary key; DT_NAME[1] beta -> zeta
//!                    (Type stays primary, ties break by value).
//! Column headers aren't boost-registered widgets (TableHeadersRow is
//! ImGui-internal), so the click point is computed from each column's row-0
//! cell: x = cell-left + inset (headers are left-aligned, like the cells),
//! y = cell-top - header_dy. The sort is verified through the rendered cell
//! text (``payload.value``), which tracks ROWS after the in-place sort_rows().

let SHIFT_KEY = int(ImGuiKey.LeftShift)

def col_header(var snap : JsonValue?; cell : string; header_dy : float) : tuple<float; float> {
    let b = snap?["globals"]?[cell]?["bbox"]
    return ((b?["x"] ?? 0.0f) + 22.0f, (b?["y"] ?? 0.0f) - header_dy)
}

//! Click ``pt`` (a column header) under an already-posted voice line, then assert the sort
//! callback reordered ``verify_target`` to ``expected``; ``shift`` holds LeftShift for the
//! SortMulti beat. Caller posts say_begin first so prepare_recording scans the voice line.
def sort_under_voice(app : ImguiApp; dwell : uint; pt : tuple<float; float>;
                     shift : bool; verify_target : string; expected : string) {
    let anchor = record_frame_count(app)
    let lead = dwell > 1400u ? 650u : dwell / 3u
    hold_content(app, lead)
    if (shift) {
        post_command(app, "imgui_key_press", JV((key = SHIFT_KEY)))
    }
    var ev : array<JsonValue?>
    ev |> click_at(0, pt, 450)
    post_command(app, "imgui_mouse_play", JV((events = ev)))
    unsafe { delete ev; }
    let play = 450u + 400u   // travel-to-header + settle for the re-sort to land
    hold_content(app, play)
    if (shift) {
        post_command(app, "imgui_key_release", JV((key = SHIFT_KEY)))
    }
    record_check_value(app, verify_target, "value", expected, 180)
    hold_remainder_content(app, dwell, anchor)
}

[export]
def main {
    with_recording_app("examples/tutorial/data_table.das",
                       "data_table.apng", 55) $(app) {
        let T_NAME0 = "DT_WIN/DT_TABLE/DT_NAME[0]"
        let T_NAME1 = "DT_WIN/DT_TABLE/DT_NAME[1]"
        let T_KIND0 = "DT_WIN/DT_TABLE/DT_KIND[0]"
        let T_VAL0  = "DT_WIN/DT_TABLE/DT_VAL[0]"
        // Caption anchors: bottom-row cells. The narrate box scores "below" with zero overlaps
        // (empty region under the six rows), so it parks there instead of covering the clickable
        // header row; the connector still points at the sorted column.
        let C_NAME  = "DT_WIN/DT_TABLE/DT_NAME[5]"
        let C_KIND  = "DT_WIN/DT_TABLE/DT_KIND[5]"
        let C_VAL   = "DT_WIN/DT_TABLE/DT_VAL[5]"

        var snap = wait_for_widget(app, T_NAME0, 10.0f)
        if (snap == null) {
            panic("{T_NAME0} never rendered - wrong app running?")
        }

        // Header sits ~14px above row 0's top edge; the click x is the column's
        // row-0 cell left + a small inset (column-left is stable across sorts,
        // unlike the text-extent-dependent cell center).
        let header_dy = 14.0f
        let p_name_hdr = col_header(snap, T_NAME0, header_dy)
        let p_kind_hdr = col_header(snap, T_KIND0, header_dy)
        let p_val_hdr  = col_header(snap, T_VAL0, header_dy)

        // Light the modifier strip so the Shift pill is visible on the multi-sort beat.
        post_command(app, "imgui_key_hud", JV((enabled = true, show_modifiers = true)))

        // Warm up the capture pipeline before the first voice: cold-start frames render slower,
        // and counting them under the first beat skews convert's frame->time map. A settled intro
        // on the Name header absorbs them and parks the cursor where beat 2 clicks.
        move_to(app, p_name_hdr, 800)
        hold_content(app, 1800u)

        // ---- Beat 1: intro (no action) - the grid boots sorted by Name ascending ----
        say(app, "sortable grid - sorted by name", C_NAME,
            [voice = "This is a data table - a sortable grid wrapping ImGui's tables. Six rows, three columns, and it starts sorted by name."])

        // ---- Beat 2: click Name header - ascending flips to descending ----
        sort_under_voice(app, say_begin(app, "click header: flip the sort", C_NAME,
            [voice = "Click a column header to sort by it. Click Name again and ascending flips to descending - now Z to A."]),
            p_name_hdr, false, T_NAME0, "zeta")

        // ---- Beat 3: click Type header - single-column sort replaces the key ----
        sort_under_voice(app, say_begin(app, "click another column", C_KIND,
            [voice = "Click a different header and the whole grid re-sorts by that column. Now it is grouped by type, bool first."]),
            p_kind_hdr, false, T_NAME0, "delta")

        // ---- Beat 4: Shift+click Value header - SortMulti adds a secondary key ----
        sort_under_voice(app, say_begin(app, "Shift+click: multi-column sort", C_VAL,
            [voice = "Hold Shift and click a second header to add it as a secondary key. Type stays primary; within each type, rows now break ties by value."]),
            p_val_hdr, true, T_NAME1, "zeta")

        post_command(app, "imgui_key_hud", JV((enabled = false)))
    }
}
