options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Boost v2 narrative widgets: bullets, separators, text kinds.

[test(description = "bullet and bullet_text widgets register in the snapshot and bullet_text echoes its string as payload.value", timeout = 1200)]
def test_narrative_bullet_smoke(t : T?) {
    with_imgui_app("modules/dasImgui/examples/features/narrative_bullet.das") $(d) {
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

[test(description = "separator and separator_text register, separator_text mirrors its text, and label_text carries both its key and its value", timeout = 1200)]
def test_narrative_separator_smoke(t : T?) {
    with_imgui_app("modules/dasImgui/examples/features/narrative_separator.das") $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/LABEL_FPS", 15.0f)
        t |> success(snap != null, "LABEL_FPS renders")
        if (snap == null) return

        t |> success(widget_exists(snap, "MAIN_WIN/SEP_AFTER_LEAD"),
                     "separator SEP_AFTER_LEAD")
        t |> success(widget_exists(snap, "MAIN_WIN/SECTION_RUNTIME"),
                     "separator_text SECTION_RUNTIME")
        let runtime_v = widget_payload_field(snap, "MAIN_WIN/SECTION_RUNTIME", "value") ?? ""
        t |> equal(runtime_v, "Runtime", "separator_text payload mirrors text=")

        let fps_k = widget_payload_field(snap, "MAIN_WIN/LABEL_FPS", "key") ?? ""
        let fps_v = widget_payload_field(snap, "MAIN_WIN/LABEL_FPS", "value") ?? ""
        t |> equal(fps_k, "frames per second", "label_text key")
        t |> equal(fps_v, "60", "label_text value")
    }
}

[test(description = "all five narrative text kinds (text, text_unformatted, text_wrapped, text_colored, text_disabled) render and echo their string as payload.value", timeout = 1200)]
def test_narrative_text_smoke(t : T?) {
    with_imgui_app("modules/dasImgui/examples/features/narrative_text.das") $(d) {
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
