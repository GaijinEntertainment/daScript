options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for `align_text_to_frame_padding` — vertically aligns
//! the next text() baseline with a framed sibling (button/input/slider) on
//! the same row. Two rows: A (bare text + button, top-aligned) vs B (align
//! prefix + text + button, baseline-shifted). Verifies the widget kind and
//! the geometry contract: Row B's label/button vertical gap is smaller
//! than Row A's because the align prefix lifts LABEL_B into the frame's
//! vertical centre.

let FEATURE = "modules/dasImgui/examples/features/align_text_to_frame_padding.das"

[test]
def test_align_text_to_frame_padding_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_widget(d, "MAIN_WIN/LABEL_A", 15.0f)
        t |> success(snap != null, "LABEL_A registers")
        if (snap == null) return

        let align = find_widget(snap, "MAIN_WIN/ALIGN_B")
        t |> equal(align?["kind"] ?? "", "align_text_to_frame_padding",
                   "ALIGN_B.kind == 'align_text_to_frame_padding'")

        for (ident in ["LEAD", "BTN_A", "LABEL_B", "BTN_B"]) {
            t |> success(widget_exists(snap, "MAIN_WIN/{ident}"),
                         "{ident} registers")
        }

        let lab_a_y = find_widget(snap, "MAIN_WIN/LABEL_A")?["bbox"]?["y"] ?? 0.0f
        let lab_b_y = find_widget(snap, "MAIN_WIN/LABEL_B")?["bbox"]?["y"] ?? 0.0f
        let btn_a_y = find_widget(snap, "MAIN_WIN/BTN_A")?["bbox"]?["y"] ?? 0.0f
        let btn_b_y = find_widget(snap, "MAIN_WIN/BTN_B")?["bbox"]?["y"] ?? 0.0f

        // Row B is on a later row than Row A.
        t |> success(lab_b_y > lab_a_y,
                     "LABEL_B.y ({lab_b_y}) > LABEL_A.y ({lab_a_y})")

        // Row A: bare text sits at the line top — label.y == button.y. Row B:
        // align_text_to_frame_padding shifts LABEL_B's baseline down by FramePadding.y to align
        // with BTN_B's centre (label.y > button.y), so B's shift delta exceeds A's.
        let row_a_shift = lab_a_y - btn_a_y
        let row_b_shift = lab_b_y - btn_b_y
        t |> success(row_b_shift > row_a_shift,
                     "Row B label shifted down ({row_b_shift}) > Row A ({row_a_shift})")
        t |> success(row_b_shift > 0.0f,
                     "Row B label is below button top (shift {row_b_shift} > 0)")
    }
}
