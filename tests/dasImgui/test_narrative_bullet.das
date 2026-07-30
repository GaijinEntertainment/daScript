options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for boost v2 §3.2 bullet / bullet_text.
//! Drives `examples/features/narrative_bullet.das`.

let FEATURE = "modules/dasImgui/examples/features/narrative_bullet.das"

[test]
def test_narrative_bullet_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/BULLET_LIST_FAST", 15.0f)
        t |> success(snap != null, "BULLET_LIST_FAST renders")
        if (snap == null) return

        t |> success(widget_exists(snap, "MAIN_WIN/BULLET_LIST_FAST"),
                     "bullet_text BULLET_LIST_FAST")
        t |> success(widget_exists(snap, "MAIN_WIN/BULLET_LIST_TYPED"),
                     "bullet_text BULLET_LIST_TYPED")
        t |> success(widget_exists(snap, "MAIN_WIN/MARKER_ONE"),
                     "bullet MARKER_ONE")
        t |> success(widget_exists(snap, "MAIN_WIN/MARKER_TWO"),
                     "bullet MARKER_TWO")

        let v = widget_payload_field(snap, "MAIN_WIN/BULLET_LIST_FAST", "value") ?? ""
        t |> success(!empty(v), "bullet_text payload.value non-empty")
    }
}
