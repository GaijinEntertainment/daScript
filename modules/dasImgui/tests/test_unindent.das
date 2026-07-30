options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for boost v2 unindent rail.
//! Drives `examples/features/unindent.das` — verifies the indent/unindent
//! bracketing markers all register entries (markers emit per-frame
//! snapshot entries via EmptyMarkerState even though they don't render
//! anything themselves) and that the ImGui indent stack doesn't underflow
//! across the asymmetric 3-indent + 1-large-unindent pattern.

let FEATURE = "modules/dasImgui/examples/features/unindent.das"

[test]
def test_unindent_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/IND_1", 15.0f)
        t |> success(snap != null, "IND_1 renders")
        if (snap == null) return

        for (ident in ["IND_1", "IND_2", "UND_2", "UND_1",
                       "IND_A", "IND_B", "IND_C", "UND_BIG"]) {
            t |> success(widget_exists(snap, "MAIN_WIN/{ident}"),
                         "{ident} marker registered")
        }
    }
}
