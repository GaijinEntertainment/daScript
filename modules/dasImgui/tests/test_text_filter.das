options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for `text_filter` + `passes_filter` — incl/excl
//! filter rail over `ImGuiTextFilter::Draw`. The widget itself registers
//! as `text_filter` kind; with an empty initial filter, all 10 LOG_ROW
//! entries pass and register. Filter-mutation tests are out of scope —
//! text_filter has no imgui_force_set dispatcher today.

let FEATURE = "modules/dasImgui/examples/features/text_filter.das"

[test]
def test_text_filter_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_widget(d, "MAIN_WIN/FILT", 15.0f)
        t |> success(snap != null, "FILT registers")
        if (snap == null) return

        let filt = find_widget(snap, "MAIN_WIN/FILT")
        t |> equal(filt?["kind"] ?? "", "text_filter",
                   "FILT.kind == 'text_filter'")

        // Initial state — empty filter, every line passes. All 10 LOG_ROW[i]
        // entries register as text widgets under MAIN_WIN.
        for (i in range(10)) {
            t |> success(widget_exists(snap, "MAIN_WIN/LOG_ROW[{i}]"),
                         "LOG_ROW[{i}] renders with empty filter")
        }

        // Surrounding siblings register.
        t |> success(widget_exists(snap, "MAIN_WIN/LEAD"), "LEAD registers")
        t |> success(widget_exists(snap, "MAIN_WIN/SEP"),  "SEP registers")

        // TODO(text_filter dispatcher): when imgui_force_set lands for text_filter
        // kind (or a typing-into-FILT path opens up), assert post-filter row
        // counts: filter='INFO' → 4 rows; filter='-DEBUG' → 8 rows.
    }
}
