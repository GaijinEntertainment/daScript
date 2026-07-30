options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for boost v2 §3.2 narrative text widgets.
//! Drives `examples/features/narrative_text.das` to confirm all five
//! narrative kinds (text / text_unformatted / text_wrapped / text_colored /
//! text_disabled) register and echo their rendered string under
//! payload.value.

let FEATURE = "modules/dasImgui/examples/features/narrative_text.das"

[test]
def test_narrative_text_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/NARRATIVE_LEAD", 15.0f)
        t |> success(snap != null, "NARRATIVE_LEAD renders")
        if (snap == null) return

        let kinds : array<tuple<string; string>> <- [
            ("NARRATIVE_LEAD", "text"),
            ("NARRATIVE_UNFORMATTED", "text_unformatted"),
            ("NARRATIVE_WRAPPED", "text_wrapped"),
            ("NARRATIVE_COLORED", "text_colored"),
            ("NARRATIVE_DISABLED", "text_disabled")
        ]
        for (k in kinds) {
            let (ident, expected_kind) = k
            let path = "MAIN_WIN/{ident}"
            t |> success(widget_exists(snap, path), "{path} renders")
            let v = widget_payload_field(snap, path, "value") ?? ""
            t |> success(!empty(v), "{path} payload.value non-empty")
        }
    }
}
