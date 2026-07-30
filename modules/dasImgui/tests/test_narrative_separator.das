options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for boost v2 §3.2 separator / separator_text / label_text.
//! Drives `examples/features/narrative_separator.das`.

let FEATURE = "modules/dasImgui/examples/features/narrative_separator.das"

[test]
def test_narrative_separator_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/LABEL_FPS", 15.0f)
        t |> success(snap != null, "LABEL_FPS renders")
        if (snap == null) return

        t |> success(widget_exists(snap, "MAIN_WIN/SEP_AFTER_LEAD"),
                     "separator SEP_AFTER_LEAD")
        t |> success(widget_exists(snap, "MAIN_WIN/SECTION_RUNTIME"),
                     "separator_text SECTION_RUNTIME")
        let runtime_v = widget_payload_field(snap, "MAIN_WIN/SECTION_RUNTIME", "value") ?? ""
        t |> equal(runtime_v, "Runtime", "separator_text payload mirrors text=")

        // label_text carries TWO strings — key + value
        let fps_k = widget_payload_field(snap, "MAIN_WIN/LABEL_FPS", "key") ?? ""
        let fps_v = widget_payload_field(snap, "MAIN_WIN/LABEL_FPS", "value") ?? ""
        t |> equal(fps_k, "frames per second", "label_text key")
        t |> equal(fps_v, "60", "label_text value")
    }
}
