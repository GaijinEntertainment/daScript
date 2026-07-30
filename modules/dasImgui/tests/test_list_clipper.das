options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public
require strings

//! Integration smoke for `list_clipper` — virtual-list culling rail. The
//! feature renders 1000 rows inside a 400×360 scrollable child. The clipper
//! should submit ONLY the visible chunk (~20-30 rows at default font); rows
//! outside the viewport never reach widget submission. Proven by counting
//! globals keys with the SCROLL_AREA/CLIPPED_ROW[ prefix.

let FEATURE = "modules/dasImgui/examples/features/list_clipper.das"

[test]
def test_list_clipper_culling(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_widget(d, "MAIN_WIN/SCROLL_AREA", 15.0f)
        t |> success(snap != null, "SCROLL_AREA child registers")
        if (snap == null) return

        t |> success(widget_exists(snap, "MAIN_WIN/LEAD"), "LEAD registers")
        t |> success(widget_exists(snap, "MAIN_WIN/SCROLL_AREA/CLIPPED_ROW[0]"),
                     "CLIPPED_ROW[0] in initial visible chunk")

        // Count globals keys matching the clipped-row prefix. list_clipper is
        // imperative (not a [widget]), so we walk the globals map directly.
        let g = snap?["globals"]
        var clipped_count = 0
        if (g != null) {
            let obj & = unsafe(g as _object)
            for (k in keys(obj)) {
                if (k |> starts_with("MAIN_WIN/SCROLL_AREA/CLIPPED_ROW[")) {
                    clipped_count++
                }
            }
        }
        t |> success(clipped_count > 5,
                     "list_clipper rendered something ({clipped_count} rows of 1000)")
        t |> success(clipped_count < 200,
                     "list_clipper culling active ({clipped_count} rows << 1000)")
    }
}
